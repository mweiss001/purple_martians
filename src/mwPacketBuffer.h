// mwPacketBuffer.h

#include "mwRollingAverage.h"
#include "mwTally.h"

struct packet_buffer
{
   int active;
   int type;
   double timestamp;
   int p;
   int packetsize;
   int packetpos;
   char data[PACKET_BUFFER_SIZE];
};

class mwPacketBuffer
{
   public:
   mwRollingAverage RA[8];

   /*
#define PM_NETGAME_PACKET_TYPE_CDAT 1
#define PM_NETGAME_PACKET_TYPE_STAK 2
#define PM_NETGAME_PACKET_TYPE_CJON 3
#define PM_NETGAME_PACKET_TYPE_CJRC 4
#define PM_NETGAME_PACKET_TYPE_RCTL 5
#define PM_NETGAME_PACKET_TYPE_SFAK 6
#define PM_NETGAME_PACKET_TYPE_CRFL 7
#define PM_NETGAME_PACKET_TYPE_CLOG 8

#define PM_NETGAME_PACKET_TYPE_STDF 11
#define PM_NETGAME_PACKET_TYPE_SJON 12
#define PM_NETGAME_PACKET_TYPE_SJRC 13
#define PM_NETGAME_PACKET_TYPE_SNFO 14
#define PM_NETGAME_PACKET_TYPE_SFIL 15
#define PM_NETGAME_PACKET_TYPE_SRRF 16

*/

   const char* packet_names[20] =
   {
      "",
      "cdat",
      "stak",
      "cjon",
      "cjrc",
      "rctl",
      "sfak",
      "crfl",
      "clog",
      "",
      "",
      "stdf",
      "sjon",
      "sjrc",
      "snfo",
      "sfil",
      "srrf"
   };






   mwPacketBuffer(); // default constructor
   ~mwPacketBuffer(); // default deconstructor
   void init_packet_buffer();

   void PacketName(char *data, int &pos, const char *name);
   int PacketRead(char *data, const char *id);

   void PacketAddStringN(char *data, int &pos, char* s);
   void PacketReadStringN(int i, char* s);
   void PacketReadStringN(char *data, int &pos, char* s);

   void PacketPutByte(char *data, int &pos, char b);
   char PacketGetByte(char *data, int &pos);
   char PacketGetByte(int i);

   void PacketPutInt32(char *data, int &pos, int d);
   int PacketGetInt32(char *data, int &pos);
   int PacketGetInt32(int i);

   void PacketPutDouble(char *data, int &pos, double d);
   double PacketGetDouble(char *data, int &pos);
   double PacketGetDouble(int i);

   struct packet_buffer rx_buf[200];

   void check_for_packets();

   void add_to_rx_buffer_single(char *data, int p);
   int find_empty_rx_packet_buffer();
   void rx_and_proc();
   void proc_rx_buffer();

   float get_max_dsync();

   void showData(char *data, int sz);


};

extern mwPacketBuffer mPacketBuffer;

