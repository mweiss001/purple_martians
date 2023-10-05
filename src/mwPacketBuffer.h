// mwPacketBuffer.h

#include "mwRollingAverage.h"

struct packet_buffer
{
   int active;
   int type;
   double timestamp;
   int who;
   int packetsize;
   int packetpos;
   char data[1024];
};



class mwPacketBuffer
{
   public:

   mwRollingAverage RA[8];

   mwPacketBuffer(); // default constructor
   void init_packet_buffer(void);

   float get_max_dsync(void);

   void start_packet_threads(void);
   void stop_packet_threads(void);

   int PT=1;
   int PM=1;

   int rx_buf_has_packets = 0;

   ALLEGRO_THREAD *rx_thread;
   ALLEGRO_THREAD *tx_thread;


   ALLEGRO_MUTEX *rx_mutex;
   ALLEGRO_MUTEX *tx_mutex;

   ALLEGRO_COND *rx_cond;



   static void *rx_thread_func(ALLEGRO_THREAD *thr, void *arg);
   static void *tx_thread_func(ALLEGRO_THREAD *thr, void *arg);



   void PacketName(char *data, int &pos, const char *name);

   void PacketAddByte(char *data, int &pos, char b);
   void PacketAddString(char *data, int &pos, char* s);
   void PacketPutDouble(char *data, int &pos, double d);
   void PacketPutInt4(char *data, int &pos, int d);
   void PacketPutInt2(char *data, int &pos, int b);
   void PacketPutInt1(char *data, int &pos, int b);

   char PacketGetByte(int i);
   void PacketReadString(int i, char* s);
   double PacketGetDouble(int i);
   int PacketGetInt4(int i);
   int PacketGetInt2(int i);
   int PacketGetInt1(int i);


   struct packet_buffer rx_buf[200];
   void add_to_rx_buffer(void);
   void add_to_rx_buffer_single(int who);
   int find_empty_rx_packet_buffer(void);
   void rx_and_proc(void);
   void proc_rx_buffer(void);
   void proc_rx_buffer_single(int i);



   void send_packet(char* data, int len, int who);

   struct packet_buffer tx_buf[200];
   void add_to_tx_buffer(char* data, int len, int who);
   int find_empty_tx_packet_buffer(void);
   void send_tx_buffer(void);


};

extern mwPacketBuffer mPacketBuffer;

