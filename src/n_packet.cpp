#include "n_packet.h"
#include "pm.h"
#ifdef NETPLAY

/* A global memory buffer for the current network packet. */
char packetbuffer[1024];
int packetsize;

static int packetpos;

/* Create a new packet. */
void Packet(char *id) {
	strncpy(packetbuffer, id, 4);
	packetsize = 4;
}

/* Append a byte to the packet. */
void PacketAddByte(char b) {
	packetbuffer[packetsize] = b;
	packetsize++;
}

/* Check if currently a packet with the given id can be read. */
int PacketRead(char *id) {
	if(packetsize >= 4 && !strncmp(packetbuffer, id, 4)) {
		packetpos = 4;
		return 1;
	}
	return 0;
}

/* Get the next byte from the packet. */
char PacketGetByte(void) {
	char b = packetbuffer[packetpos];
	packetpos++;
	return b;
}

// ---------------------------------------------------------------------------------------------------------------
// ***************************************************************************************************************
//----------------------------------------------------------------------------------------------------------------


void PacketAddString(char* s)
{
   for (int i=0; i<15; i++) PacketAddByte(s[i]); // copy first 15 char
   PacketAddByte(0);                             // for safety terminate with NULL
}

void PacketReadString(char* s)
{
   for (int i=0; i<16; i++) s[i] = PacketGetByte();
}

void PacketAdd2Bytes(int b)
{
   unsigned char hi_b = (unsigned char) (b/256);
   PacketAddByte(hi_b);

   unsigned char lo_b = (unsigned char) (b - (hi_b*256));
   PacketAddByte(lo_b);
}

void PacketAdd3Bytes(int b)
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

void PacketAdd4Bytes(int b)
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

int Packet2ByteRead(void)
{
   unsigned char byte_ho = (unsigned char)PacketGetByte();
   unsigned char byte_lo = (unsigned char)PacketGetByte();
   int b = (byte_ho * 256) + byte_lo;
   return b;
}
int Packet3ByteRead(void)
{
   unsigned char byte_sh = (unsigned char)PacketGetByte();
   unsigned char byte_ho = (unsigned char)PacketGetByte();
   unsigned char byte_lo = (unsigned char)PacketGetByte();
   int b = (byte_sh*65536) + (byte_ho*256) + byte_lo;
   return b;
}
int Packet4ByteRead(void)
{
   unsigned char byte_uh = (unsigned char)PacketGetByte();
   unsigned char byte_sh = (unsigned char)PacketGetByte();
   unsigned char byte_ho = (unsigned char)PacketGetByte();
   unsigned char byte_lo = (unsigned char)PacketGetByte();
   int b = (byte_uh*16777216) + (byte_sh*65536) + (byte_ho*256) + byte_lo;
   return b;
}



#endif





