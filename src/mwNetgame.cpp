// mwNetgame.cpp

#include "pm.h"
#include "mwNetgame.h"
#include "mwPlayer.h"
#include "mwLift.h"
#include "mwTriggerEvent.h"
#include "mwItem.h"
#include "mwEnemy.h"
#include "mwLevel.h"
#include "mwShot.h"
#include "mwLog.h"
#include "mwTally.h"
#include "mwGameMoves.h"
#include "mwLoop.h"
#include "mwInput.h"
#include "mwRollingAverage.h"
#include "mwDisplay.h"
#include "mwFont.h"
#include "mwBitmap.h"
#include "mwTimeStamp.h"
#include "mwColor.h"
#include "mwEventQueue.h"
#include "mwConfig.h"
#include "mwScreen.h"


mwNetgame mNetgame;

mwNetgame::mwNetgame()
{
   NetworkDriver = -1;
   ima_server = 0;
   ima_client = 0;
   sprintf(m_serveraddress, "192.168.1.2");
   TCP = 0;
   zlib_cmp = 7;

   ServerConn = NULL;
   ServerChannel = NULL;

   tmaj_i = 0;

   ClientNum = 0;
   ListenConn = NULL;                      // listening connection
   ClientConn[MAX_CLIENTS] = {NULL, };     // array of connections for each client
   ListenChannel = NULL;                   // listen channel
   ClientChannel[MAX_CLIENTS] = {NULL, };  // array of channels for each client
}



// initialize libnet and setup a driver to use. Returns 0 on success.
int mwNetgame::NetworkInit(void)
{
   NET_DRIVERLIST drivers;
   NET_DRIVERNAME *drivernames;

   net_init(); // initialize libnet
   net_loadconfig(NULL); // configure libnet

   // detect drivers in the internet class
   drivers = net_detectdrivers(net_classes[NET_CLASS_INET].drivers);
   drivernames = net_getdrivernames(drivers);

   NetworkDriver = drivernames[0].num; // use first detected driver

   free(drivernames);

   // valid driver?
   if(NetworkDriver >= 0)
   {
      if(net_initdriver(NetworkDriver)) return 0;
      else
      {
         // Error: Couldn't initialize network driver
         return -1;
      }
   }
   else
   {
      // Error: Found no driver in the internet network class
      return -1;
   }
}

void mwNetgame::init_packet_buffer(void)
{
   for (int i=0; i<200; i++)
   {
      packet_buffers[i].active = 0;
      packet_buffers[i].type = 0;
      packet_buffers[i].timestamp = 0;
      packet_buffers[i].who = 0;
      packet_buffers[i].packetsize = 0;
      packet_buffers[i].data[0] = 0;
   }
}


void mwNetgame::set_packetpos(int pos)
{
   packetpos = pos;
}

int mwNetgame::get_packetpos(void)
{
   return packetpos;
}



// create a new packet
void mwNetgame::Packet(const char *id)
{
	strncpy(packetbuffer, id, 4);
	packetsize = 4;
}

// check if a packet with the given id can be read
int mwNetgame::PacketRead(const char *id)
{
	if(packetsize >= 4 && !strncmp(packetbuffer, id, 4))
   {
		packetpos = 4;
		return 1;
	}
	return 0;
}

// append a byte to the packet
void mwNetgame::PacketAddByte(char b)
{
	packetbuffer[packetsize] = b;
	packetsize++;
}

// get the next byte from the packet
char mwNetgame::PacketGetByte(void)
{
	char b = packetbuffer[packetpos];
	packetpos++;
	return b;
}



void mwNetgame::PacketAddString(char* s)
{
   for (int i=0; i<15; i++) PacketAddByte(s[i]); // copy first 15 char only
   PacketAddByte(0); // for safety terminate with NULL in case string is longer than 15
}

void mwNetgame::PacketReadString(char* s)
{
   for (int i=0; i<16; i++) s[i] = PacketGetByte();
}

void mwNetgame::PacketPutDouble(double d)
{
   memcpy(packetbuffer + packetsize, &d, 8);
	packetsize+=8;
}

double mwNetgame::PacketGetDouble(void)
{
   double d = 0;
   memcpy(&d, packetbuffer + packetpos, 8);
	packetpos+=8;
	return d;
}

void mwNetgame::PacketPut1ByteInt(int b)
{
   unsigned char lo_b = (unsigned char) (b);
   PacketAddByte(lo_b);
}
void mwNetgame::PacketPut2ByteInt(int b)
{
   unsigned char hi_b = (unsigned char) (b/256);
   PacketAddByte(hi_b);

   unsigned char lo_b = (unsigned char) (b - (hi_b*256));
   PacketAddByte(lo_b);
}
void mwNetgame::PacketPut3ByteInt(int b)
{
   int t = b;

   unsigned char sh_b = (unsigned char) (t/65536);
   PacketAddByte(sh_b);
   t -= sh_b * 65536;

   unsigned char hi_b = (unsigned char) (t/256);
   PacketAddByte(hi_b);
   t -= hi_b * 256;

   unsigned char lo_b = (unsigned char) t;
   PacketAddByte(lo_b);
}

void mwNetgame::PacketPut4ByteInt(int b)
{
   int t = b;

   unsigned char uh_b = (unsigned char) (t/16777216);
   PacketAddByte(uh_b);
   t -= uh_b * 16777216;

   unsigned char sh_b = (unsigned char) (t/65536);
   PacketAddByte(sh_b);
   t -= sh_b * 65536;

   unsigned char hi_b = (unsigned char) (t/256);
   PacketAddByte(hi_b);
   t -= hi_b * 256;

   unsigned char lo_b = (unsigned char) t;
   PacketAddByte(lo_b);
}

int mwNetgame::PacketGet1ByteInt(void)
{
   unsigned char byte_lo = (unsigned char)PacketGetByte();
   int b = byte_lo;
	return b;
}
int mwNetgame::PacketGet2ByteInt(void)
{
   unsigned char byte_ho = (unsigned char)PacketGetByte();
   unsigned char byte_lo = (unsigned char)PacketGetByte();
   int b = (byte_ho * 256) + byte_lo;
   return b;
}
int mwNetgame::PacketGet3ByteInt(void)
{
   unsigned char byte_sh = (unsigned char)PacketGetByte();
   unsigned char byte_ho = (unsigned char)PacketGetByte();
   unsigned char byte_lo = (unsigned char)PacketGetByte();
   int b = (byte_sh*65536) + (byte_ho*256) + byte_lo;
   return b;
}
int mwNetgame::PacketGet4ByteInt(void)
{
   unsigned char byte_uh = (unsigned char)PacketGetByte();
   unsigned char byte_sh = (unsigned char)PacketGetByte();
   unsigned char byte_ho = (unsigned char)PacketGetByte();
   unsigned char byte_lo = (unsigned char)PacketGetByte();
   int b = (byte_uh*16777216) + (byte_sh*65536) + (byte_ho*256) + byte_lo;
   return b;
}


void mwNetgame::game_vars_to_state(char * b)
{
   int sz = 0, offset = 0;
   offset += sz; sz = sizeof(mPlayer.syn);         memcpy(b+offset, mPlayer.syn,         sz);
   offset += sz; sz = sizeof(mEnemy.Ei);           memcpy(b+offset, mEnemy.Ei,           sz);
   offset += sz; sz = sizeof(mEnemy.Ef);           memcpy(b+offset, mEnemy.Ef,           sz);
   offset += sz; sz = sizeof(mItem.item);          memcpy(b+offset, mItem.item,          sz);
   offset += sz; sz = sizeof(mItem.itemf);         memcpy(b+offset, mItem.itemf,         sz);
   offset += sz; sz = sizeof(mLift.cur);           memcpy(b+offset, mLift.cur,           sz);
   offset += sz; sz = sizeof(mLevel.l);            memcpy(b+offset, mLevel.l,            sz);
   offset += sz; sz = sizeof(mShot.p);             memcpy(b+offset, mShot.p,             sz);
   offset += sz; sz = sizeof(mShot.e);             memcpy(b+offset, mShot.e,             sz);
   offset += sz; sz = sizeof(mTriggerEvent.event); memcpy(b+offset, mTriggerEvent.event, sz);
}

void mwNetgame::state_to_game_vars(char * b)
{
   int sz = 0, offset = 0;
   sz = sizeof(mPlayer.syn);         memcpy(mPlayer.syn,         b+offset, sz); offset += sz;
   sz = sizeof(mEnemy.Ei);           memcpy(mEnemy.Ei,           b+offset, sz); offset += sz;
   sz = sizeof(mEnemy.Ef);           memcpy(mEnemy.Ef,           b+offset, sz); offset += sz;
   sz = sizeof(mItem.item);          memcpy(mItem.item,          b+offset, sz); offset += sz;
   sz = sizeof(mItem.itemf);         memcpy(mItem.itemf,         b+offset, sz); offset += sz;
   sz = sizeof(mLift.cur);           memcpy(mLift.cur,           b+offset, sz); offset += sz;
   sz = sizeof(mLevel.l);            memcpy(mLevel.l,            b+offset, sz); offset += sz;
   sz = sizeof(mShot.p);             memcpy(mShot.p,             b+offset, sz); offset += sz;
   sz = sizeof(mShot.e);             memcpy(mShot.e,             b+offset, sz); offset += sz;
   sz = sizeof(mTriggerEvent.event); memcpy(mTriggerEvent.event, b+offset, sz); offset += sz;
}

void mwNetgame::get_state_dif(char *a, char *b, char *c, int size)
{
   for (int i=0; i<size; i++) c[i] = a[i] - b[i];
}

void mwNetgame::apply_state_dif(char *a, char *c, int size)
{
   for (int i=0; i<size; i++) a[i] -= c[i];
}

void mwNetgame::reset_states(void)
{
   // reset base state on client
   memset(client_state_base, 0, STATE_SIZE);
   client_state_base_frame_num = 0;  // mLoop.frame_num id

   // reset dif buffer and mLoop.frame_num
   memset(client_state_dif, 0, STATE_SIZE);
   client_state_dif_src = -1; // -1 will never match a mLoop.frame_num
   client_state_dif_dst = -1;

   // reset buffer and frame_nums used to build compressed state from packets
   memset(client_state_buffer, 0, STATE_SIZE);
   for (int i=0; i<16; i++)
      client_state_buffer_pieces[i] = -1;

   // reset server's client states
   for (int i=0; i<8; i++) reset_client_state(i);

   // reset server's rewind states
   for (int i=0; i<NUM_REWIND_STATES; i++)
   {
      memset(srv_rewind_state[i], 0, STATE_SIZE);
      srv_rewind_state_frame_num[i] = -1;
   }


}

void mwNetgame::reset_client_state(int p) // server calls this when client quits
{
   memset(srv_client_state[p][0], 0, STATE_SIZE);
   memset(srv_client_state[p][1], 0, STATE_SIZE);
   srv_client_state_frame_num[p][0] = 0;  // src
   srv_client_state_frame_num[p][1] = -3; // dst
}

