// n_packet.cpp

#include "pm.h"

// global memory buffer for the current network packet
char packetbuffer[1024];
int packetsize;
static int packetpos;

// create a new packet
void Packet(const char *id)
{
	strncpy(packetbuffer, id, 4);
	packetsize = 4;
}

// check if a packet with the given id can be read
int PacketRead(const char *id)
{
	if(packetsize >= 4 && !strncmp(packetbuffer, id, 4))
   {
		packetpos = 4;
		return 1;
	}
	return 0;
}

// append a byte to the packet
void PacketAddByte(char b)
{
	packetbuffer[packetsize] = b;
	packetsize++;
}

// get the next byte from the packet
char PacketGetByte(void)
{
	char b = packetbuffer[packetpos];
	packetpos++;
	return b;
}



void PacketAddString(char* s)
{
   for (int i=0; i<15; i++) PacketAddByte(s[i]); // copy first 15 char only
   PacketAddByte(0); // for safety terminate with NULL in case string is longer than 15
}

void PacketReadString(char* s)
{
   for (int i=0; i<16; i++) s[i] = PacketGetByte();
}




void PacketPut1ByteInt(int b)
{
   unsigned char lo_b = (unsigned char) (b);
   PacketAddByte(lo_b);
}
void PacketPut2ByteInt(int b)
{
   unsigned char hi_b = (unsigned char) (b/256);
   PacketAddByte(hi_b);

   unsigned char lo_b = (unsigned char) (b - (hi_b*256));
   PacketAddByte(lo_b);
}
void PacketPut3ByteInt(int b)
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

void PacketPut4ByteInt(int b)
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

int PacketGet1ByteInt(void)
{
   unsigned char byte_lo = (unsigned char)PacketGetByte();
   int b = byte_lo;
	return b;
}
int PacketGet2ByteInt(void)
{
   unsigned char byte_ho = (unsigned char)PacketGetByte();
   unsigned char byte_lo = (unsigned char)PacketGetByte();
   int b = (byte_ho * 256) + byte_lo;
   return b;
}
int PacketGet3ByteInt(void)
{
   unsigned char byte_sh = (unsigned char)PacketGetByte();
   unsigned char byte_ho = (unsigned char)PacketGetByte();
   unsigned char byte_lo = (unsigned char)PacketGetByte();
   int b = (byte_sh*65536) + (byte_ho*256) + byte_lo;
   return b;
}
int PacketGet4ByteInt(void)
{
   unsigned char byte_uh = (unsigned char)PacketGetByte();
   unsigned char byte_sh = (unsigned char)PacketGetByte();
   unsigned char byte_ho = (unsigned char)PacketGetByte();
   unsigned char byte_lo = (unsigned char)PacketGetByte();
   int b = (byte_uh*16777216) + (byte_sh*65536) + (byte_ho*256) + byte_lo;
   return b;
}





