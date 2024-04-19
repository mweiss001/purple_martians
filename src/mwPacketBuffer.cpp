// mwPacketBuffer.cpp

#include "pm.h"
#include "mwPacketBuffer.h"
#include "mwNetgame.h"
#include "mwLoop.h"
#include "mwLog.h"
#include "mwPlayer.h"

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
      al_rest(0.001);
      mPacketBuffer.add_to_rx_buffer();
   }
}

void mwPacketBuffer::c_rx_thread_func(void)
{
   while (thread_running)
   {
      if (thread_working) mPacketBuffer.add_to_rx_buffer();
      std::this_thread::sleep_for(std::chrono::microseconds((int)thread_wait_microseconds));
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

// processes all packets waiting in rx buffer
void mwPacketBuffer::proc_rx_buffer(void)
{
   lock_mutex();
   for (int i=0; i<200; i++)
      if (rx_buf[i].active)
      {
         rx_buf[i].packetpos = 4;
         if (mNetgame.ima_server)
         {
            if (rx_buf[i].type == PM_NETGAME_PACKET_TYPE_CDAT) mNetgame.server_proc_cdat_packet(i);
            if (rx_buf[i].type == PM_NETGAME_PACKET_TYPE_STAK) mNetgame.server_proc_stak_packet(i);
            if (rx_buf[i].type == PM_NETGAME_PACKET_TYPE_RCTL) mNetgame.server_proc_rctl_packet(i);
            if (rx_buf[i].type == PM_NETGAME_PACKET_TYPE_SFAK) mNetgame.server_proc_sfak_packet(i);
            if (rx_buf[i].type == PM_NETGAME_PACKET_TYPE_CRFL) mNetgame.server_proc_crfl_packet(i);
         }
         if (mNetgame.ima_client)
         {
            if (rx_buf[i].type == PM_NETGAME_PACKET_TYPE_STDF) mNetgame.client_proc_stdf_packet(i);
            if (rx_buf[i].type == PM_NETGAME_PACKET_TYPE_SNFO) mNetgame.client_proc_snfo_packet(i);
            if (rx_buf[i].type == PM_NETGAME_PACKET_TYPE_SFIL) mNetgame.client_proc_sfil_packet(i);
         }
         rx_buf[i].active = 0;
      }
   unlock_mutex();
}

// call from loop - many places
// only does something if not already called from thread
void mwPacketBuffer::check_for_packets(void)
{
   if ((thread_type == 0) && ((mNetgame.ima_server) || (mNetgame.ima_client))) add_to_rx_buffer();
}


// this gets called once per second in the main loop
// to display how many times add_rx was called
void mwPacketBuffer::process_tally(void)
{
//   printf("add_rx_buf chek:%d tot:%0.3fms  avg:%0.3fus  proc:%d tot:%0.3fms  avg:%0.3fus twus:%f\n",
//          threadTally[0].num, threadTally[0].get_tally(0)*1000, threadTally[0].get_avg(0)*1000000,
//          threadTally[1].num, threadTally[1].get_tally(0)*1000, threadTally[1].get_avg(0)*1000000, thread_wait_microseconds);

//   int act_tps = threadTally[0].num; // actual threads per second
//   if (act_tps > 9000)
//   {
//      thread_wait_microseconds = thread_wait_microseconds * 1.05;
//   }
//
//   if (act_tps < 9100)
//   {
//      thread_wait_microseconds = thread_wait_microseconds * 0.95;
//   }
//
//
   // reset tallies
   threadTally[0].initialize();
   threadTally[1].initialize();
}


int mwPacketBuffer::is_data_waiting(void)
{
   double t0 = al_get_time();
   int data_waiting = 0;
   if ((mNetgame.ima_server) && (net_query(mNetgame.ServerChannel))) data_waiting = 1;
   if ((mNetgame.ima_client) && (net_query(mNetgame.ClientChannel))) data_waiting = 1;
   threadTally[0].add_data(al_get_time() - t0);
   return data_waiting;
}

// receives all waiting packets and puts them in the rx buffer
void mwPacketBuffer::add_to_rx_buffer(void)
{
   if (is_data_waiting())
   {
      double t0 = al_get_time();
      lock_mutex();
      char data[1024] = {0};
      char address[256];
      int who = -1;

      if (mNetgame.ima_server)
         while (int len = net_receive(mNetgame.ServerChannel, data, 1024, address))
         {

            who = mNetgame.server_check_address(address);
            if (who == -1) // unknown address
            {
               if (PacketRead(data, "cjon")) mNetgame.server_proc_cjon_packet(data, address); // setup channel and replay with sjon
               if (PacketRead(data, "cjrc")) mNetgame.server_proc_cjrc_packet(data, address); // setup channel and replay with sjrc
            }
            else
            {
               add_to_rx_buffer_single(data, who);

               mPlayer.loc[who].rx_current_bytes_for_this_frame += len;
               mPlayer.loc[who].rx_current_packets_for_this_frame++;

               mPlayer.loc[0].rx_current_bytes_for_this_frame += len;
               mPlayer.loc[0].rx_current_packets_for_this_frame++;
            }
         }

      if (mNetgame.ima_client)
         while (mNetgame.ClientReceive(data))
            add_to_rx_buffer_single(data, who);

      unlock_mutex();
      threadTally[1].add_data(al_get_time() - t0);
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
   if (PacketRead(data, "cdat")) type = PM_NETGAME_PACKET_TYPE_CDAT;
   if (PacketRead(data, "stak")) type = PM_NETGAME_PACKET_TYPE_STAK;
   if (PacketRead(data, "rctl")) type = PM_NETGAME_PACKET_TYPE_RCTL;
   if (PacketRead(data, "sfak")) type = PM_NETGAME_PACKET_TYPE_SFAK;
   if (PacketRead(data, "stdf")) type = PM_NETGAME_PACKET_TYPE_STDF;
   if (PacketRead(data, "snfo")) type = PM_NETGAME_PACKET_TYPE_SNFO;
   if (PacketRead(data, "sfil")) type = PM_NETGAME_PACKET_TYPE_SFIL;
   if (PacketRead(data, "crfl")) type = PM_NETGAME_PACKET_TYPE_CRFL;

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
   else printf("%d received unknown packet type!\n", mLoop.frame_num);
}

int mwPacketBuffer::find_empty_rx_packet_buffer(void)
{
   for (int i=0; i<200; i++) if (!rx_buf[i].active) return i;

   mLog.add(LOG_NET, 0, "rx packet buffer full!\n");
   printf("[%d] rx packet buffer full!\n", mLoop.frame_num);

   // count types of packets in buffer
   int cdat_count = 0;
   int stak_count = 0;
   int rctl_count = 0;
   int stdf_count = 0;
   int snfo_count = 0;
   int sfil_count = 0;
   int crfl_count = 0;
   int all_count = 0;

   for (int i=0; i<200; i++)
      if (!rx_buf[i].active)
      {
         all_count++;
         if (rx_buf[i].type == PM_NETGAME_PACKET_TYPE_CDAT) cdat_count++;
         if (rx_buf[i].type == PM_NETGAME_PACKET_TYPE_STAK) stak_count++;
         if (rx_buf[i].type == PM_NETGAME_PACKET_TYPE_RCTL) rctl_count++;
         if (rx_buf[i].type == PM_NETGAME_PACKET_TYPE_STDF) stdf_count++;
         if (rx_buf[i].type == PM_NETGAME_PACKET_TYPE_SNFO) snfo_count++;
         if (rx_buf[i].type == PM_NETGAME_PACKET_TYPE_SFIL) sfil_count++;
         if (rx_buf[i].type == PM_NETGAME_PACKET_TYPE_CRFL) crfl_count++;
      }
   mLog.addf(LOG_NET, 0, "[%d] cdat\n", cdat_count);
   mLog.addf(LOG_NET, 0, "[%d] stak\n", stak_count);
   mLog.addf(LOG_NET, 0, "[%d] rctl\n", rctl_count);
   mLog.addf(LOG_NET, 0, "[%d] stdf\n", stdf_count);
   mLog.addf(LOG_NET, 0, "[%d] snfo\n", snfo_count);
   mLog.addf(LOG_NET, 0, "[%d] sfil\n", sfil_count);
   mLog.addf(LOG_NET, 0, "[%d] crfl\n", crfl_count);

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
      if ((rx_buf[i].active) && (rx_buf[i].type == PM_NETGAME_PACKET_TYPE_STDF))
      {
         int dst = 0;
         memcpy(&dst, rx_buf[i].data+8, 4); // memcpy dst

         // calc dysnc
         float csync = (float)(dst - mLoop.frame_num) * 0.025;                     // crude integer sync based on frame numbers
         float dsync = csync + al_get_time() - mPacketBuffer.rx_buf[i].timestamp;  // add time between now and when the packet was received into packet buffer

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




// packet put and get byte functions
// ---------------------------------
void mwPacketBuffer::PacketPutByte(char *data, int &pos, char b)
{
	data[pos] = b;
	pos++;
}
char mwPacketBuffer::PacketGetByte(char *data, int &pos)
{
	char b = data[pos];
	pos++;
	return b;
}
char mwPacketBuffer::PacketGetByte(int i)
{
	char b = rx_buf[i].data[rx_buf[i].packetpos];
	rx_buf[i].packetpos++;
	return b;
}



// packet add and read string functions
// -----------------------------------
void mwPacketBuffer::PacketAddString(char *data, int &pos, char* s)
{
   for (int a=0; a<15; a++) PacketPutByte(data, pos, s[a]); // copy first 15 char only
   PacketPutByte(data, pos, 0);                             // for safety terminate with NULL in case string is longer than 15
}
void mwPacketBuffer::PacketReadString(char *data, int &pos, char* s)
{
   for (int a=0; a<16; a++) s[a] = PacketGetByte(data, pos);
}
void mwPacketBuffer::PacketReadString(int i, char* s)
{
   for (int a=0; a<16; a++) s[a] = PacketGetByte(i);
}


// packet put and get double functions
// -----------------------------------
void mwPacketBuffer::PacketPutDouble(char *data, int &pos, double d)
{
   memcpy(data + pos, &d, 8);
	pos+=8;
}
double mwPacketBuffer::PacketGetDouble(char *data, int &pos)
{
   double d = 0;
   memcpy(&d, data + pos, 8);
	pos+=8;
	return d;
}
double mwPacketBuffer::PacketGetDouble(int i)
{
   return PacketGetDouble(rx_buf[i].data, rx_buf[i].packetpos);
}



// packet put and get 4 byte int functions
// ---------------------------------------
void mwPacketBuffer::PacketPutInt4(char *data, int &pos, int d)
{
   memcpy(data + pos, &d, 4);
	pos+=4;
}
int mwPacketBuffer::PacketGetInt4(char *data, int &pos)
{
   int d = 0;
   memcpy(&d, data + pos, 4);
	pos+=4;
	return d;
}
int mwPacketBuffer::PacketGetInt4(int i)
{
   int d = 0;
   memcpy(&d, rx_buf[i].data + rx_buf[i].packetpos, 4);
	rx_buf[i].packetpos+=4;
	return d;
}












