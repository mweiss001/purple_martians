// mwPacketBuffer.cpp

#include "pm.h"
#include "mwPacketBuffer.h"
#include "mwNetgame.h"
#include "mwLoop.h"
#include "mwLog.h"
#include "mwPlayer.h"
#include "mwTally.h"
#include "mwRollingAverage.h"
#include "mwEventQueue.h"


#include <thread>
#include <chrono>


mwPacketBuffer mPacketBuffer;

mwPacketBuffer::mwPacketBuffer() // default constructor
{
   init_packet_buffer();
   rx_mutex = al_create_mutex();
   if (thread_type == 2)
   {
      t1 = std::thread(&mwPacketBuffer::c_rx_thread_func, this);
      thread_running = 1;
   }
}


mwPacketBuffer::~mwPacketBuffer() // default deconstructor
{
   if (thread_type == 2)
   {
      thread_running = 0;
      t1.join();
   }
}


void mwPacketBuffer::init_packet_buffer(void)
{
   for (int i=0; i<200; i++)
   {
      rx_buf[i].active = 0;
      rx_buf[i].type = 0;
      rx_buf[i].timestamp = 0;
      rx_buf[i].who = 0;
      rx_buf[i].packetsize = 0;
      rx_buf[i].packetpos = 0;
      rx_buf[i].data[0] = 0;
   }
   for (int p=0; p<NUM_PLAYERS; p++) RA[p].initialize(8);
}

void *mwPacketBuffer::al_rx_thread_func(ALLEGRO_THREAD *thr, void *arg)
{
   while (1)
   {
////      static double t0 = al_get_time();
////      double t1 = al_get_time();
////      if (t1-t0 > 0.001)
////      {
////         t0 = t1;
////         mPacketBuffer.add_to_rx_buffer();
////      }

      al_rest(0.001);
      mPacketBuffer.add_to_rx_buffer();
   }
}



void mwPacketBuffer::c_rx_thread_func(void)
{
   while (thread_running)
   {
      if (thread_working) mPacketBuffer.add_to_rx_buffer();
//      std::this_thread::sleep_for(std::chrono::nanoseconds(100));
//      std::this_thread::sleep_for(std::chrono::microseconds(1));
      std::this_thread::sleep_for(std::chrono::milliseconds(1));
   }
}




void mwPacketBuffer::start_packet_thread(void)
{
   if (thread_type == 1)
   {
      int junk = 0;
      rx_thread = al_create_thread(al_rx_thread_func, &junk);
      al_start_thread(rx_thread);
   }
   if (thread_type == 2) thread_working = 1;
}

void mwPacketBuffer::stop_packet_thread(void)
{
   if (thread_type == 1) al_set_thread_should_stop(rx_thread);
   if (thread_type == 2) thread_working = 0;
}


void mwPacketBuffer::rx_and_proc(void)
{
   if (!thread_type) add_to_rx_buffer();
   proc_rx_buffer();
}


// processes a single packets from the rx buffer
void mwPacketBuffer::proc_rx_buffer_single(int i)
{
//   double bt = al_get_time() - rx_buf[i].timestamp; // time spent in buffer
//   printf("[%4d] proc packet:%d size:%4d buffer_time:%8.1f us\n", mLoop.frame_num, i, rx_buf[i].packetsize, bt*1000000);

   rx_buf[i].packetpos = 4;
   if (mNetgame.ima_server)
   {
      if (rx_buf[i].type == 1) mNetgame.server_proc_cdat_packet(i);
      if (rx_buf[i].type == 2) mNetgame.server_proc_stak_packet(i);
      if (rx_buf[i].type == 3) mNetgame.server_proc_cjon_packet(i);
      if (rx_buf[i].type == 4) mNetgame.server_proc_cjrc_packet(i);
      if (rx_buf[i].type == 5) mNetgame.server_proc_rctl_packet(i);

   }
   if (mNetgame.ima_client)
   {
      if (rx_buf[i].type == 11) mNetgame.client_process_stdf_packet(i);
      if (rx_buf[i].type == 12) mNetgame.client_process_sjon_packet(i);
      if (rx_buf[i].type == 13) mNetgame.client_process_sjrc_packet(i);
      if (rx_buf[i].type == 14) mNetgame.client_process_snfo_packet(i);
   }
   rx_buf[i].active = 0;
}

// processes all packets waiting in rx buffer
void mwPacketBuffer::proc_rx_buffer(void)
{
   lock_mutex();

   for (int i=0; i<199; i++)
      if (rx_buf[i].active) proc_rx_buffer_single(i);

   unlock_mutex();
}


// call from loop - many places
// only does something if not already called from thread
void mwPacketBuffer::check_for_packets(void)
{
   if ((thread_type == 0) && ((mNetgame.ima_server) || (mNetgame.ima_client))) add_to_rx_buffer();
}






// receives all waiting packets and puts them in the rx buffer
void mwPacketBuffer::add_to_rx_buffer(void)
{

   if (mNetgame.is_data_waiting())
   {

      double t0 = al_get_time();

      lock_mutex();

      char data[1024] = {0};

      int who = -1;
      if (mNetgame.ima_server)
         while(mNetgame.ServerReceive(data, &who))
            add_to_rx_buffer_single(data, who);

      if (mNetgame.ima_client)
         while (mNetgame.ClientReceive(data))
            add_to_rx_buffer_single(data, who);


      unlock_mutex();

      mTally[1].add_data(al_get_time() - t0);
   }
}


void mwPacketBuffer::add_to_rx_buffer_single(char *data, int who)
{
   // process these immediately and do not add to buffer
   if (PacketRead(data, "ping"))
   {
      mNetgame.server_proc_ping_packet(data, who);
      return;
   }
   if (PacketRead(data, "pang"))
   {
      mNetgame.server_proc_pang_packet(data, who);
      return;
   }
   if (PacketRead(data, "pong"))
   {
      mNetgame.client_proc_pong_packet(data);
      return;
   }


   int type = 0;

   if (PacketRead(data, "cdat")) type = 1;
   if (PacketRead(data, "stak")) type = 2;
   if (PacketRead(data, "cjon")) type = 3;
   if (PacketRead(data, "cjrc")) type = 4;
   if (PacketRead(data, "rctl")) type = 5;

   if (PacketRead(data, "stdf")) type = 11;
   if (PacketRead(data, "sjon")) type = 12;
   if (PacketRead(data, "sjrc")) type = 13;
   if (PacketRead(data, "snfo")) type = 14;


   if (type)
   {
      int indx = find_empty_rx_packet_buffer();
      if (indx != -1)
      {
         //printf("%d stored rx packet:%d size:%d type:%d\n", mLoop.frame_num, indx, mNetgame.packetsize, type);
         rx_buf[indx].active = 1;
         rx_buf[indx].type = type;
         rx_buf[indx].timestamp = al_get_time();
         rx_buf[indx].who = who;
         memcpy(rx_buf[indx].data, data, 1024);
      }
   }
}

int mwPacketBuffer::find_empty_rx_packet_buffer(void)
{
   for (int i=0; i<200; i++) if (!rx_buf[i].active) return i;

   mLog.add(LOG_NET, 0, "rx packet buffer full!\n");
   printf("[%d] rx packet buffer full!\n", mLoop.frame_num);

   // count types of packets in buffer
   int cdat_count = 0;
   int stak_count = 0;
   int cjon_count = 0;
   int cjrc_count = 0;
   int rctl_count = 0;
   int stdf_count = 0;
   int sjon_count = 0;
   int sjrc_count = 0;
   int snfo_count = 0;
   int all_count = 0;

   for (int i=0; i<200; i++)
      if (!rx_buf[i].active)
      {
         all_count++;
         if (rx_buf[i].type == 1)  cdat_count++;
         if (rx_buf[i].type == 2)  stak_count++;
         if (rx_buf[i].type == 3)  cjon_count++;
         if (rx_buf[i].type == 4)  cjrc_count++;
         if (rx_buf[i].type == 5)  rctl_count++;
         if (rx_buf[i].type == 11) stdf_count++;
         if (rx_buf[i].type == 12) sjon_count++;
         if (rx_buf[i].type == 13) sjrc_count++;
         if (rx_buf[i].type == 14) snfo_count++;
      }
   mLog.addf(LOG_NET, 0, "[%d] cdat\n", cdat_count);
   mLog.addf(LOG_NET, 0, "[%d] stak\n", stak_count);
   mLog.addf(LOG_NET, 0, "[%d] cjon\n", cjon_count);
   mLog.addf(LOG_NET, 0, "[%d] cjrc\n", cjrc_count);
   mLog.addf(LOG_NET, 0, "[%d] rctl\n", rctl_count);
   mLog.addf(LOG_NET, 0, "[%d] stdf\n", stdf_count);
   mLog.addf(LOG_NET, 0, "[%d] sjon\n", sjon_count);
   mLog.addf(LOG_NET, 0, "[%d] sjrc\n", sjrc_count);
   mLog.addf(LOG_NET, 0, "[%d] snfo\n", snfo_count);

   return -1;
}

void mwPacketBuffer::lock_mutex(void)
{
   if (thread_type == 1) al_lock_mutex(rx_mutex);
   if (thread_type == 2) m.lock();
}

void mwPacketBuffer::unlock_mutex(void)
{
   if (thread_type == 1) al_unlock_mutex(rx_mutex);
   if (thread_type == 2) m.unlock();
}


float mwPacketBuffer::get_max_dsync(void)
{
   float max_dsync = -1000;
   lock_mutex();

   // iterate all stdf packets, calc dysnc and max dysnc
   for (int i=0; i<200; i++)
      if ((rx_buf[i].active) && (rx_buf[i].type == 11))
      {
         int dst = 0;
         memcpy(&dst, rx_buf[i].data+8, 4);

         // calc dysnc
         float csync = (float)(dst - mLoop.frame_num) * 0.025;   // crude integer sync based on frame numbers
         float dsync = al_get_time() - mPacketBuffer.rx_buf[i].timestamp + csync;  // add time between now and when the packet was received into packet buffer

         if (dsync > max_dsync) max_dsync = dsync;
      }

   unlock_mutex();
   return max_dsync;
}





void mwPacketBuffer::PacketName(char *data, int &pos, const char *name)
{
   sprintf(data, "%s", name);
   pos = 4;
}

// check if a packet has the given id
int mwPacketBuffer::PacketRead(char *data, const char *id )
{
   if (!strncmp(data, id, 4)) return 1;
	return 0;
}


char mwPacketBuffer::PacketGetByte(int i)
{
	char b = rx_buf[i].data[rx_buf[i].packetpos];
	rx_buf[i].packetpos++;
	return b;
}

void mwPacketBuffer::PacketAddByte(char *data, int &pos, char b)
{
	data[pos] = b;
	pos++;
}

void mwPacketBuffer::PacketAddString(char *data, int &pos, char* s)
{
   for (int a=0; a<15; a++) PacketAddByte(data, pos, s[a]); // copy first 15 char only
   PacketAddByte(data, pos, 0);                             // for safety terminate with NULL in case string is longer than 15
}

void mwPacketBuffer::PacketReadString(int i, char* s)
{
   for (int a=0; a<16; a++) s[a] = PacketGetByte(i);
}

double mwPacketBuffer::PacketGetDouble(int i)
{
   return PacketGetDouble(rx_buf[i].data, rx_buf[i].packetpos);
}

double mwPacketBuffer::PacketGetDouble(char *data, int &pos)
{
   double d = 0;
   memcpy(&d, data + pos, 8);
	pos+=8;
	return d;
}



void mwPacketBuffer::PacketPutDouble(char *data, int &pos, double d)
{
   memcpy(data + pos, &d, 8);
	pos+=8;
}

void mwPacketBuffer::PacketPutInt4(char *data, int &pos, int d)
{
   memcpy(data + pos, &d, 4);
	pos+=4;
}

void mwPacketBuffer::PacketPutInt2(char *data, int &pos, int b)
{
   unsigned char hi_b = (unsigned char) (b/256);
   PacketAddByte(data, pos, hi_b);

   unsigned char lo_b = (unsigned char) (b - (hi_b*256));
   PacketAddByte(data, pos, lo_b);
}

void mwPacketBuffer::PacketPutInt1(char *data, int &pos, int b)
{
   unsigned char lo_b = (unsigned char) (b);
   PacketAddByte(data, pos, lo_b);
}

int mwPacketBuffer::PacketGetInt4(int i)
{
   int d = 0;
   memcpy(&d, rx_buf[i].data + rx_buf[i].packetpos, 4);
	rx_buf[i].packetpos+=4;
	return d;
}

int mwPacketBuffer::PacketGetInt2(int i)
{
   unsigned char byte_ho = (unsigned char)PacketGetByte(i);
   unsigned char byte_lo = (unsigned char)PacketGetByte(i);
   int b = (byte_ho * 256) + byte_lo;
   return b;
}

int mwPacketBuffer::PacketGetInt1(int i)
{
   unsigned char byte_lo = (unsigned char)PacketGetByte(i);
   int b = byte_lo;
	return b;
}
