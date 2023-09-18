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
#include "mwStateHistory.h"



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

void mwNetgame::PacketPutInt4(int d)
{
   memcpy(packetbuffer + packetsize, &d, 4);
	packetsize+=4;
}

int mwNetgame::PacketGetInt4(void)
{
   int d = 0;
   memcpy(&d, packetbuffer + packetpos, 4);
	packetpos+=4;
	return d;
}


void mwNetgame::PacketPutInt1(int b)
{
   unsigned char lo_b = (unsigned char) (b);
   PacketAddByte(lo_b);
}
void mwNetgame::PacketPutInt2(int b)
{
   unsigned char hi_b = (unsigned char) (b/256);
   PacketAddByte(hi_b);

   unsigned char lo_b = (unsigned char) (b - (hi_b*256));
   PacketAddByte(lo_b);
}
int mwNetgame::PacketGetInt1(void)
{
   unsigned char byte_lo = (unsigned char)PacketGetByte();
   int b = byte_lo;
	return b;
}
int mwNetgame::PacketGetInt2(void)
{
   unsigned char byte_ho = (unsigned char)PacketGetByte();
   unsigned char byte_lo = (unsigned char)PacketGetByte();
   int b = (byte_ho * 256) + byte_lo;
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
   // reset dif buffer and frame_nums
   memset(client_state_dif, 0, STATE_SIZE);
   client_state_dif_src = -1; // -1 will never match a frame_num
   client_state_dif_dst = -1;

   // reset buffer and frame_nums used to build compressed state from packets
   memset(client_state_buffer, 0, STATE_SIZE);
   for (int i=0; i<16; i++) client_state_buffer_pieces[i] = -1;

   for (int i=0; i<8; i++) mStateHistory[i].initialize();

   server_dirty_frame = -1;

}


void mwNetgame::process_bandwidth_counters(int p)
{
   // get maximums per frame
   if (mPlayer.loc[p].tx_current_packets_for_this_frame > mPlayer.loc[p].tx_max_packets_per_frame) mPlayer.loc[p].tx_max_packets_per_frame = mPlayer.loc[p].tx_current_packets_for_this_frame;
   if (mPlayer.loc[p].tx_current_bytes_for_this_frame >   mPlayer.loc[p].tx_max_bytes_per_frame)   mPlayer.loc[p].tx_max_bytes_per_frame =   mPlayer.loc[p].tx_current_bytes_for_this_frame;
   if (mPlayer.loc[p].rx_current_packets_for_this_frame > mPlayer.loc[p].rx_max_packets_per_frame) mPlayer.loc[p].rx_max_packets_per_frame = mPlayer.loc[p].rx_current_packets_for_this_frame;
   if (mPlayer.loc[p].rx_current_bytes_for_this_frame >   mPlayer.loc[p].rx_max_bytes_per_frame)   mPlayer.loc[p].rx_max_bytes_per_frame =   mPlayer.loc[p].rx_current_bytes_for_this_frame;

   // get totals
   mPlayer.loc[p].tx_total_bytes   += mPlayer.loc[p].tx_current_bytes_for_this_frame;
   mPlayer.loc[p].tx_total_packets += mPlayer.loc[p].tx_current_packets_for_this_frame;
   mPlayer.loc[p].rx_total_bytes   += mPlayer.loc[p].rx_current_bytes_for_this_frame;
   mPlayer.loc[p].rx_total_packets += mPlayer.loc[p].rx_current_packets_for_this_frame;

   // add to tallies
   mPlayer.loc[p].tx_bytes_tally   += mPlayer.loc[p].tx_current_bytes_for_this_frame;
   mPlayer.loc[p].tx_packets_tally += mPlayer.loc[p].tx_current_packets_for_this_frame;
   mPlayer.loc[p].rx_bytes_tally   += mPlayer.loc[p].rx_current_bytes_for_this_frame;
   mPlayer.loc[p].rx_packets_tally += mPlayer.loc[p].rx_current_packets_for_this_frame;

   // reset counts for this frame
   mPlayer.loc[p].tx_current_bytes_for_this_frame = 0;
   mPlayer.loc[p].tx_current_packets_for_this_frame = 0;
   mPlayer.loc[p].rx_current_bytes_for_this_frame = 0;
   mPlayer.loc[p].rx_current_packets_for_this_frame = 0;

   if (mLoop.frame_num % 40 == 0) // tally freq = 40 frames = 1s
   {
      // get maximums per tally
      if (mPlayer.loc[p].tx_bytes_per_tally >   mPlayer.loc[p].tx_max_bytes_per_tally)   mPlayer.loc[p].tx_max_bytes_per_tally =   mPlayer.loc[p].tx_bytes_per_tally;
      if (mPlayer.loc[p].rx_bytes_per_tally >   mPlayer.loc[p].rx_max_bytes_per_tally)   mPlayer.loc[p].rx_max_bytes_per_tally =   mPlayer.loc[p].rx_bytes_per_tally;
      if (mPlayer.loc[p].tx_packets_per_tally > mPlayer.loc[p].tx_max_packets_per_tally) mPlayer.loc[p].tx_max_packets_per_tally = mPlayer.loc[p].tx_packets_per_tally;
      if (mPlayer.loc[p].rx_packets_per_tally > mPlayer.loc[p].rx_max_packets_per_tally) mPlayer.loc[p].rx_max_packets_per_tally = mPlayer.loc[p].rx_packets_per_tally;

      // copy to display variables
      mPlayer.loc[p].tx_bytes_per_tally   = mPlayer.loc[p].tx_bytes_tally;
      mPlayer.loc[p].tx_packets_per_tally = mPlayer.loc[p].tx_packets_tally;
      mPlayer.loc[p].rx_bytes_per_tally   = mPlayer.loc[p].rx_bytes_tally;
      mPlayer.loc[p].rx_packets_per_tally = mPlayer.loc[p].rx_packets_tally;

      mLog.addf(LOG_NET_bandwidth, p, "bandwidth txb:[%d] rxb:[%d] txp:[%d] rxp:[%d]\n", mPlayer.loc[p].tx_bytes_per_tally, mPlayer.loc[p].rx_bytes_per_tally, mPlayer.loc[p].tx_packets_per_tally, mPlayer.loc[p].rx_packets_per_tally);

      // reset tallies
      mPlayer.loc[p].tx_bytes_tally = 0;
      mPlayer.loc[p].tx_packets_tally = 0;
      mPlayer.loc[p].rx_bytes_tally = 0;
      mPlayer.loc[p].rx_packets_tally = 0;
   }
}


