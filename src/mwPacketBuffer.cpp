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


mwPacketBuffer mPacketBuffer;

mwPacketBuffer::mwPacketBuffer()
{
   init_packet_buffer();
   rx_mutex = al_create_mutex();
   tx_mutex = al_create_mutex();

   rx_cond = al_create_cond();

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


void *mwPacketBuffer::rx_thread_func(ALLEGRO_THREAD *thr, void *arg)
{
   while (1)
   {
//      al_rest(0.001);
//      mPacketBuffer.add_to_rx_buffer();

      if (mEventQueue.thr_proc)
      {
         mPacketBuffer.add_to_rx_buffer();
         mEventQueue.thr_proc = 0;
      }
   }
}

void *mwPacketBuffer::tx_thread_func(ALLEGRO_THREAD *thr, void *arg)
{
   while (1)
   {
//      al_rest(0.001);
//      mPacketBuffer.send_tx_buffer();
//      if (mEventQueue.thr_proc)
//      {
//         mPacketBuffer.send_tx_buffer();
//         mEventQueue.thr_proc = 0;
//      }
   }
}

void mwPacketBuffer::start_packet_threads(void)
{
   if (PT)
   {
      int junk = 0;
      rx_thread = al_create_thread(rx_thread_func, &junk);
      al_start_thread(rx_thread);
//      tx_thread = al_create_thread(tx_thread_func, &junk);
//      al_start_thread(tx_thread);
   }
}

void mwPacketBuffer::stop_packet_threads(void)
{
   if (PT) al_set_thread_should_stop(rx_thread);


//   if (PT)
//   {
//      al_set_thread_should_stop(rx_thread);
//      al_set_thread_should_stop(tx_thread);
//   }
}





// sends all packets waiting in tx buffer
void mwPacketBuffer::send_tx_buffer(void)
{
//   if (PM) al_lock_mutex(tx_mutex);
//
//   for (int i=0; i<200; i++)
//      if (tx_buf[i].active)
//      {
//         if (mNetgame.ima_client)
//         {
//            if (mNetgame.TCP) net_send_rdm(mNetgame.ServerConn,    tx_buf[i].data, tx_buf[i].packetsize);
//            else              net_send(    mNetgame.ServerChannel, tx_buf[i].data, tx_buf[i].packetsize);
//            //mwNetgame::ClientSend(tx_buf[indx].data, tx_buf[i].packetsize);
//         }
//         if (mNetgame.ima_server)
//         {
//            if (mNetgame.TCP) net_send_rdm(mNetgame.ClientConn[   tx_buf[i].who], tx_buf[i].data, tx_buf[i].packetsize);
//            else              net_send(    mNetgame.ClientChannel[tx_buf[i].who], tx_buf[i].data, tx_buf[i].packetsize);
//
//            //mwNetgame::ServerSendTo(tx_buf[indx].data, tx_buf[indx].packetsize, tx_buf[i].who, );
//         }
//         tx_buf[i].active = 0;
//      }
//
//   if (PM) al_unlock_mutex(tx_mutex);
}


// send a packet immediatley
void mwPacketBuffer::send_packet(char* data, int len, int who)
{
   if (mNetgame.ima_client)
   {
      if (mNetgame.TCP) net_send_rdm(mNetgame.ServerConn,    data, len);
      else              net_send(    mNetgame.ServerChannel, data, len);
      //mwNetgame::ClientSend(data, len);
   }
   if (mNetgame.ima_server)
   {
      if (mNetgame.TCP) net_send_rdm(mNetgame.ClientConn[   who], data, len);
      else              net_send(    mNetgame.ClientChannel[who], data, len);

      //mwNetgame::ServerSendTo(data, len, who);
   }
}





// adds a packets to tx buffer
void mwPacketBuffer::add_to_tx_buffer(char* data, int len, int who)
{

   send_packet(data, len, who);


//   if (PM) al_lock_mutex(tx_mutex);
//
//   int indx = find_empty_tx_packet_buffer();
//   if (indx != -1)
//   {
//      //printf("%d stored tx packet:%d len:%d who:%d\n", mLoop.frame_num, indx, len, who);
//      tx_buf[indx].active = 1;
//      tx_buf[indx].packetsize = len;
//      tx_buf[indx].who = who;
//      memcpy(tx_buf[indx].data, data, len);
//   }
//   if (PM) al_unlock_mutex(tx_mutex);
//
//   if (!mPacketBuffer.PT) mPacketBuffer.send_tx_buffer();
//



}


void mwPacketBuffer::rx_and_proc(void)
{
   if (!PT) add_to_rx_buffer();
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
   double t0 = al_get_time();

   if (PM) al_lock_mutex(rx_mutex);

   mTally[3].add_data(al_get_time() - t0);

   if (rx_buf_has_packets)
      for (int i=0; i<199; i++)
         if (rx_buf[i].active) proc_rx_buffer_single(i);

   rx_buf_has_packets = 0;

   if (PM) al_unlock_mutex(rx_mutex);

   mTally[0].add_data(al_get_time() - t0);

}



// receives all waiting packets and puts them in the rx buffer
void mwPacketBuffer::add_to_rx_buffer(void)
{
   double t0 = al_get_time();

   if (PM) al_lock_mutex(rx_mutex);

   mTally[2].add_data(al_get_time() - t0);

   int who = -1;
   if (mNetgame.ima_server)
      while((mNetgame.packetsize = mNetgame.ServerReceive(mNetgame.packetbuffer, &who)))
         add_to_rx_buffer_single(who);

   if (mNetgame.ima_client)
      while ((mNetgame.packetsize = mNetgame.ClientReceive(mNetgame.packetbuffer)))
         add_to_rx_buffer_single(who);

   if (PM) al_unlock_mutex(rx_mutex);

   mTally[1].add_data(al_get_time() - t0);

}



void mwPacketBuffer::add_to_rx_buffer_single(int who)
{
   int type = 0;
   if (mNetgame.PacketRead("cdat")) type = 1;
   if (mNetgame.PacketRead("stak")) type = 2;
   if (mNetgame.PacketRead("cjon")) type = 3;
   if (mNetgame.PacketRead("cjrc")) type = 4;
   if (mNetgame.PacketRead("rctl")) type = 5;

   if (mNetgame.PacketRead("stdf")) type = 11;
   if (mNetgame.PacketRead("sjon")) type = 12;
   if (mNetgame.PacketRead("sjrc")) type = 13;
   if (mNetgame.PacketRead("snfo")) type = 14;


   // process these immediately
   if (mNetgame.PacketRead("ping"))
   {
      memcpy(rx_buf[199].data, mNetgame.packetbuffer, 1024);
      rx_buf[199].packetpos = 4;
      mNetgame.server_proc_ping_packet(199);
   }

   if (mNetgame.PacketRead("pang"))
   {
      memcpy(rx_buf[199].data, mNetgame.packetbuffer, 1024);
      rx_buf[199].packetpos = 4;
      mNetgame.server_proc_pang_packet(199);
   }

   if (mNetgame.PacketRead("pong"))
   {
      memcpy(rx_buf[199].data, mNetgame.packetbuffer, 1024);
      rx_buf[199].packetpos = 4;
      mNetgame.client_proc_pong_packet(199);
   }

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
         rx_buf[indx].packetsize = mNetgame.packetsize;
         memcpy(rx_buf[indx].data, mNetgame.packetbuffer, 1024);
         rx_buf_has_packets = 1;
      }
   }
}


int mwPacketBuffer::find_empty_tx_packet_buffer(void)
{
   for (int i=0; i<200; i++) if (!tx_buf[i].active) return i;

   mLog.add(LOG_NET, 0, "tx packet buffer full!\n");
   printf("[%d] tx packet buffer full!\n", mLoop.frame_num);
   return -1;
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

//   printf("[%d] cdat\n", cdat_count);
//   printf("[%d] stak\n", stak_count);
//   printf("[%d] cjon\n", cjon_count);
//   printf("[%d] cjrc\n", cjrc_count);
//   printf("[%d] rctl\n", rctl_count);
//   printf("[%d] stdf\n", stdf_count);
//   printf("[%d] sjon\n", sjon_count);
//   printf("[%d] sjrc\n", sjrc_count);
//   printf("[%d] snfo\n", snfo_count);
//   printf("[%d] all\n", all_count);

   return -1;

}




float mwPacketBuffer::get_max_dsync(void)
{
   if (PM) al_lock_mutex(rx_mutex);
   // iterate all stdf packets, calc dysnc, then get max dysnc
   float max_dsync = -1000;

   // iterate all stdf packets stored in the packet buffer
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
   if (PM) al_unlock_mutex(rx_mutex);
   return max_dsync;
}





void mwPacketBuffer::PacketName(char *data, int &pos, const char *name)
{
   sprintf(data, "%s", name);
   pos = 4;
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
   for (int i=0; i<15; i++) PacketAddByte(data, pos, s[i]); // copy first 15 char only
   PacketAddByte(data, pos, 0);                             // for safety terminate with NULL in case string is longer than 15
}

void mwPacketBuffer::PacketReadString(int i, char* s)
{
   for (int i=0; i<16; i++) s[i] = PacketGetByte(i);
}

double mwPacketBuffer::PacketGetDouble(int i)
{
   double d = 0;
   memcpy(&d, rx_buf[i].data + rx_buf[i].packetpos, 8);
	rx_buf[i].packetpos+=8;
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













