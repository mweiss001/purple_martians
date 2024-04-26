// mwPacketBuffer.h

#include "mwRollingAverage.h"
#include "mwTally.h"

#include <thread>
#include <mutex>


struct packet_buffer
{
   int active;
   int type;
   double timestamp;
   int p;
   int packetsize;
   int packetpos;
   char data[1024];
};

class mwPacketBuffer
{
   public:
   mwRollingAverage RA[8];

   mwTally threadTally[2];

   mwPacketBuffer(); // default constructor
   ~mwPacketBuffer(); // default deconstructor
   void init_packet_buffer(void);

   // add to rx buffer thread type
   int thread_type = 0; // 0-none  1-Allegro  2-C++

   double thread_wait_microseconds = 1000;

   // Allegro Threads
   ALLEGRO_THREAD *rx_thread;
   ALLEGRO_MUTEX *rx_mutex;
   static void *al_rx_thread_func(ALLEGRO_THREAD *thr, void *arg);

   // C++ Threads
   std::thread t1;
   std::mutex m;
   int thread_running = 0;
   int thread_working = 0;
   void c_rx_thread_func(void);

   // Common to Allegro Threads and C++ Threads
   void start_packet_thread(void);
   void stop_packet_thread(void);
   void lock_mutex(void);
   void unlock_mutex(void);


   void PacketName(char *data, int &pos, const char *name);
   int PacketRead(char *data, const char *id);


   void PacketAddString(char *data, int &pos, char* s);
   void PacketReadString(char *data, int &pos, char* s);
   void PacketReadString(int i, char* s);

   void PacketPutByte(char *data, int &pos, char b);
   char PacketGetByte(char *data, int &pos);
   char PacketGetByte(int i);

   void PacketPutInt4(char *data, int &pos, int d);
   int PacketGetInt4(char *data, int &pos);
   int PacketGetInt4(int i);

   void PacketPutDouble(char *data, int &pos, double d);
   double PacketGetDouble(char *data, int &pos);
   double PacketGetDouble(int i);











   void process_tally(void);

   struct packet_buffer rx_buf[200];

   void check_for_packets(void);

   int is_data_waiting(void);

   void add_to_rx_buffer(void);
   void add_to_rx_buffer_single(char *data, int p);
   int find_empty_rx_packet_buffer(void);
   void rx_and_proc(void);
   void proc_rx_buffer(void);

   float get_max_dsync(void);

};

extern mwPacketBuffer mPacketBuffer;

