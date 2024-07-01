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

   mwPacketBuffer(); // default constructor
   ~mwPacketBuffer(); // default deconstructor
   void init_packet_buffer(void);

   void PacketName(char *data, int &pos, const char *name);
   int PacketRead(char *data, const char *id);

   void PacketAddString(char *data, int &pos, char* s);
   void PacketReadString(char *data, int &pos, char* s);
   void PacketReadString(int i, char* s);

   void PacketAddStringN(char *data, int &pos, char* s);
   void PacketReadStringN(int i, char* s);


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

   void check_for_packets(void);

   void add_to_rx_buffer_single(char *data, int p);
   int find_empty_rx_packet_buffer(void);
   void rx_and_proc(void);
   void proc_rx_buffer(void);

   float get_max_dsync(void);

};

extern mwPacketBuffer mPacketBuffer;

