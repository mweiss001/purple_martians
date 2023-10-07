// mwPacketBuffer.h

#include "mwRollingAverage.h"
#include <thread>
#include <mutex>


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

   ~mwPacketBuffer(); // default deconstructor


   void lock_mutex(void);
   void unlock_mutex(void);


   void init_packet_buffer(void);

   float get_max_dsync(void);

   void start_packet_thread(void);
   void stop_packet_thread(void);

   int PT=1;
   int PM=1;

   ALLEGRO_THREAD *rx_thread;
   ALLEGRO_MUTEX *rx_mutex;


   int thread_running = 0;

   int thread_working = 0;


   std::thread t1;
   std::mutex m;





   static void *rx_thread_func(ALLEGRO_THREAD *thr, void *arg);

   void PacketName(char *data, int &pos, const char *name);
   int PacketRead(char *data, const char *id );

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


   double PacketGetDouble(char *data, int &pos);


   void crx_thread_func(void);


   struct packet_buffer rx_buf[200];
   void add_to_rx_buffer(void);
   void add_to_rx_buffer_single(char *data, int who);
   int find_empty_rx_packet_buffer(void);
   void rx_and_proc(void);
   void proc_rx_buffer(void);
   void proc_rx_buffer_single(int i);
};

extern mwPacketBuffer mPacketBuffer;

