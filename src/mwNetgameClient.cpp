// mwNetgameClient.cpp

#include "pm.h"
#include "mwNetgame.h"
#include "mwLog.h"
#include "mwPlayer.h"
#include "mwRollingAverage.h"
#include "mwDisplay.h"
#include "mwFont.h"
#include "mwBitmap.h"
#include "mwTimeStamp.h"
#include "mwColor.h"
#include "mwInput.h"
#include "mwEventQueue.h"
#include "mwLevel.h"
#include "mwConfig.h"
#include "mwLoop.h"
#include "mwScreen.h"
#include "mwShot.h"
#include "mwTally.h"
#include "mwQuickGraph2.h"





int mwNetgame::ClientInitNetwork(const char *serveraddress)
{
	if(NetworkInit())
   {
      mLog.add_fw(LOG_error, 0, 76, 10, "|", " ", "Error: failed to initialize network");
      return -1;
   }
   if (TCP)
   {
      ServerConn = net_openconn(NetworkDriver, NULL);
      if (!ServerConn)
      {
         mLog.add_fw(LOG_error, 0, 76, 10, "|", " ", "Error: Client failed to create netconnection (TCP)");
         return 0;
      }
      if (net_connect(ServerConn, serveraddress))
      {
         mLog.add_fwf(LOG_error, 0, 76, 10, "|", " ", "Error: Client failed to set netconnection target: server[%s] (TCP)", serveraddress);
         net_closeconn(ServerConn);
   		return 0;
   	}
      mLog.add_fwf(LOG_NET, 0, 76, 10, "|", " ", "Client network (TCP) initialized -- server[%s]", serveraddress);
   }
   else // UDP
   {
      ServerChannel = net_openchannel(NetworkDriver, "");
      if (ServerChannel == NULL)
      {
         mLog.add_fw(LOG_error, 0, 76, 10, "|", " ", "Error: Client failed to create netchannel (UDP)");
         return 0;
      }
      if (net_assigntarget(ServerChannel, serveraddress))
      {
         mLog.add_fwf(LOG_error, 0, 76, 10, "|", " ", "Error: Client failed to set netchannel target: server[%s] (UDP)", serveraddress);
         return 0;
      }
      mLog.add_fwf(LOG_NET, 0, 76, 10, "|", " ", "Client network (UDP) initialized -- server:[%s]", serveraddress);
      //mLog.add_fwf(LOG_NET, 0, 76, 10, "|", " ", "Local address%s", net_getlocaladdress(ServerChannel));
   }


   // Check for reply from server
   int tries = 1;        // number of times to try
   float try_delay = 1;  // delay between tries
   int got_reply = 0;

   while (!got_reply)
   {
      mLog.add_fwf(LOG_NET, 0, 76, 10, "|", " ", "ClientCheckResponse %d", tries);
      int ccr = ClientCheckResponse();
      if (ccr == 1)
      {
         got_reply = 1;
         mLog.add_fw(LOG_NET, 0, 76, 10, "|", " ", "Got reply from server");
      }
      if (ccr == 0)
      {
         al_rest(try_delay);
         if (++tries > 2)
         {
            mLog.add_fw(LOG_error, 0, 76, 10, "|", " ", "Did not get reply from server");

            mLog.add_fw(LOG_NET, 0, 76, 10, "+", "-", "");
            return 0;
         }
      }
      if (ccr == -1)
      {
         mLog.add_fw(LOG_error, 0, 76, 10, "|", " ", "TCP response Error!");
         mLog.add_fw(LOG_NET, 0, 76, 10, "+", "-", "");
         return 0;
      }
      if (ccr == -2)
      {
         mLog.add_fw(LOG_error, 0, 76, 10, "|", " ", "Cancelled");
         mLog.add_fw(LOG_NET, 0, 76, 10, "+", "-", "");
         return 0;
      }
   }
   return 1;
}

int mwNetgame::ClientCheckResponse(void) // check for a repsonse from the server
{
   if (TCP)
   {
   	int x = net_poll_connect(ServerConn);
      if (x == 0) return 0; // no response yet
   	if (x >  0) return 1; // good response
   	if (x <  0) // error
      {
   		net_closeconn(ServerConn);
   		return -1;
   	}
   }
   else // UDP
   {
      char address[32];
      Packet("1234");
      ClientSend(packetbuffer, packetsize);
      mLog.add_fw(LOG_NET, 0, 76, 10, "|", " ", "Sent initial packet (1234) to server");
      mLog.add_fw(LOG_NET, 0, 76, 10, "|", " ", "Waiting for reply");

      int done = 10;
      while (done)
      {
         mEventQueue.proc(1);
         if (mInput.key[ALLEGRO_KEY_ESCAPE][1]) return -2;
         packetsize = net_receive(ServerChannel, packetbuffer, 1024, address);
         if (packetsize)
         {
            set_packetpos(0);
            if (PacketRead("5678"))
            {
               mLog.add_fw(LOG_NET, 0, 76, 10, "|", " ", "Got Response");
               net_assigntarget(ServerChannel, address);
               return 1;
            }
         }
         al_rest(.1);
         done--;
      }
      mLog.add_fw(LOG_NET, 0, 76, 10, "|", " ", "No Response");
   }
   return 0;
}


// Receive data from the server, and store in the provided array
// (must have room for 1024 bytes). Returns the size of the stored data.
int mwNetgame::ClientReceive(void *data)
{
   int len;
   if (TCP) len = net_receive_rdm(ServerConn, data, 1024);
   else    	len = net_receive(ServerChannel, data, 1024, NULL);
   if (len > 0)
   {
      mPlayer.loc[mPlayer.active_local_player].rx_current_bytes_for_this_frame+= len;
      mPlayer.loc[mPlayer.active_local_player].rx_current_packets_for_this_frame++;
   }
	return len;
}
void mwNetgame::ClientSend(void *data, int len)
{
   if (TCP) net_send_rdm(ServerConn, data, len);
   else     net_send(ServerChannel, data, len);
   mPlayer.loc[mPlayer.active_local_player].tx_current_bytes_for_this_frame+= len;
   mPlayer.loc[mPlayer.active_local_player].tx_current_packets_for_this_frame++;
}

void mwNetgame::client_flush(void)
{
   while ((packetsize = ClientReceive(packetbuffer)));
}


void mwNetgame::ClientExitNetwork(void)
{
   mLog.add_header(LOG_NET, 0, 0, "Shutting down the client network");
   if (TCP)
   {
   	if(ServerConn) net_closeconn(ServerConn);
   	ServerConn = NULL;
   }
   else // UDP
   {
   	if(ServerChannel) net_closechannel(ServerChannel);
   	ServerChannel = NULL;
   }
   net_shutdown();
}


// ---------------------------------------------------------------------------------------------------------------
// ***************************************************************************************************************
//----------------------------------------------------------------------------------------------------------------

int mwNetgame::client_init(void)
{
   mLog.log_versions();
   mLog.add_fw (LOG_NET, 0, 76, 10, "+", "-", "");
   mLog.add_fwf(LOG_NET, 0, 76, 10, "|", " ", "Client mode started on localhost:[%s]", mLoop.local_hostname);

    // initialize driver with server address
   if (!ClientInitNetwork(m_serveraddress)) return 0;

   mRollingAverage[1].initialize(8); // ping rolling average
   mRollingAverage[2].initialize(8); // dsync rolling average

   Packet("cjon");
   PacketPutInt1(mPlayer.syn[0].color); // requested color
   PacketAddString(mLoop.local_hostname);
   ClientSend(packetbuffer, packetsize);

   mLog.add_fwf(LOG_NET, 0, 76, 10, "|", " ", "Client sent join request to server with player color:[%2d]", mPlayer.syn[0].color);
   mLog.add_fw (LOG_NET, 0, 76, 10, "+", "-", "");
   return 1;
}



void mwNetgame::client_send_cjrc_packet(void)
{
   Packet("cjrc");
   ClientSend(packetbuffer, packetsize);
}

void mwNetgame::client_send_rctl_packet(int type, double val)
{
   mLoop.remote_frames_since_last_rctl_sent = 0;
   Packet("rctl");
   PacketPutInt4(type);
   PacketPutDouble(val);
   ClientSend(packetbuffer, packetsize);
}

void mwNetgame::client_process_sjon_packet(void)
{
   int pl = PacketGetInt2();   // play level
   int server_sjon_frame_num  =  PacketGetInt4();
   int p = PacketGetInt1();      // client player number
   int color = PacketGetInt1();  // client player color

   if (p == 99) // server full, join denied
   {
      mLog.add(LOG_error, 0, "Server replied with 'SERVER FULL'\n");
      mLoop.state[0] = 25;
   }
   else // join allowed
   {
      mPlayer.active_local_player = p;

      mPlayer.syn[p].active = 1;

      mPlayer.syn[p].control_method = 4;

      mPlayer.syn[p].color = color;
      strncpy(mPlayer.loc[0].hostname, m_serveraddress, 16);
      strncpy(mPlayer.loc[p].hostname, mLoop.local_hostname, 16);
      ima_client = 1;

      mLevel.play_level = pl;

      mLog.add_fwf(LOG_NET,               0, 76, 10, "|", " ", "Client received join invitation from server");
      mLog.add_fwf(LOG_NET_join_details,  0, 76, 10, "|", " ", "Level:[%d]", mLevel.play_level);
      mLog.add_fwf(LOG_NET_join_details,  0, 76, 10, "|", " ", "Player Number:[%d]", p);
      mLog.add_fwf(LOG_NET_join_details,  0, 76, 10, "|", " ", "Player Color:[%d]", color);
      mLog.add_fwf(LOG_NET_join_details,  0, 76, 10, "|", " ", "Server Frame Num:[%d]", server_sjon_frame_num);
      mLog.add_fwf(LOG_NET,               0, 76, 10, "+", "-", "");
      mLoop.state[0] = 22;
   }
}

void mwNetgame::client_exit(void)
{
   ClientExitNetwork();
   ima_client = 0;
   // reset player data
   for (int p=0; p<NUM_PLAYERS; p++) mPlayer.init_player(p, 1);
   mPlayer.syn[0].active = 1; // local_control
   mPlayer.active_local_player = 0;
   mConfig.load_config(); // to restore color and other settings
}

void mwNetgame::client_send_ping(void)
{
   Packet("ping");
   PacketPutDouble(al_get_time());
   ClientSend(packetbuffer, packetsize);
   // printf("%d ping server\n", mLoop.frame_num);
}


void mwNetgame::client_process_snfo_packets(void)
{
   while ((mNetgame.packetsize = mNetgame.ClientReceive(mNetgame.packetbuffer)))
   {
      if (mNetgame.PacketRead("snfo"))
      {
         int fn = PacketGetInt4(); // frame_num
         int seq = PacketGetInt1();
         int max_seq = PacketGetInt1();
         int sb = PacketGetInt4();
         int sz = PacketGetInt4();

         // printf("rx snfo piece [%d of %d] frame:[%d] st:%4d sz:%4d\n", seq+1, max_seq, fn, sb, sz);

         // do some checks, because one time i recieved: rx snfo piece [121 of 218] frame:[994] st:1616929891 sz:1180829280
         int bad_data = 0;
         if ((max_seq < 1) || (max_seq > 6)) bad_data = 1;
         if ((seq < 0) || (seq > 6) || (seq > max_seq)) bad_data = 1;
         if ((sb < 0) || (sb > 5000)) bad_data = 1;
         if ((sz < 0) || (sz > 1000)) bad_data = 1;

         if (bad_data) printf("rx snfo piece [%d of %d] frame:[%d] st:%4d sz:%4d  --- Bad Data!\n", seq+1, max_seq, fn, sb, sz);
         else
         {
            //printf("rx snfo piece [%d of %d] frame:[%d] st:%4d sz:%4d\n", seq+1, max_seq, fn, sb, sz);

            memcpy(client_state_buffer + sb, packetbuffer+18, sz);    // put the piece of data in the buffer
            client_state_buffer_pieces[seq] = fn;                     // mark it with frame_num

            int complete = 1;                                         // did we just get the last packet? (yes by default)
            for (int i=0; i< max_seq; i++)
               if (client_state_buffer_pieces[i] != fn) complete = 0; // no, if any piece not at latest frame_num

            if (complete)
            {
               //printf("rx snfo complete - frame:[%d]\n", fn);
               char dmp[5400];
               // uncompress
               uLongf destLen = sizeof(dmp);
               uncompress((Bytef*)dmp, (uLongf*)&destLen, (Bytef*)client_state_buffer, sizeof(client_state_buffer));

               // copy to variables
               int sz=0, offset=0;
               sz = sizeof(mPlayer.syn); memcpy(mPlayer.syn, dmp+offset, sz); offset += sz;
               sz = sizeof(mPlayer.loc); memcpy(mPlayer.loc, dmp+offset, sz); offset += sz;

               // dsync, ping graphs
               for (int i=1; i<8; i++) // cycle all clients
                  if (mPlayer.syn[i].active)
                  {
                     mQuickGraph2[1].series[i].active = 1;
                     mQuickGraph2[1].series[i].color = mPlayer.syn[i].color;
                     mQuickGraph2[1].add_data(i, mPlayer.loc[i].dsync*1000);

                     mQuickGraph2[2].series[i].active = 1;
                     mQuickGraph2[2].series[i].color = mPlayer.syn[i].color;
                     mQuickGraph2[2].add_data(i, mPlayer.loc[i].ping*1000);

                     mQuickGraph2[3].series[i].active = 1;
                     mQuickGraph2[3].series[i].color = mPlayer.syn[i].color;
                     mQuickGraph2[3].add_data(i, mPlayer.loc[i].cmp_dif_size);


                     mQuickGraph2[6].series[i].active = 1;
                     mQuickGraph2[6].series[i].color = mPlayer.syn[i].color;
                     mQuickGraph2[6].add_data(i, mPlayer.loc[i].client_loc_plr_cor);

                     mQuickGraph2[7].series[i].active = 1;
                     mQuickGraph2[7].series[i].color = mPlayer.syn[i].color;
                     mQuickGraph2[7].add_data(i, mPlayer.loc[i].client_rmt_plr_cor);

                  }


               for (int i=0; i<8; i++) // cycle all players
                  if (mPlayer.syn[i].active)
                  {
                     mQuickGraph2[0].series[i].active = 1;
                     mQuickGraph2[0].series[i].color = mPlayer.syn[i].color;
                     mQuickGraph2[0].add_data(i, mPlayer.loc[i].cpu);

                     mQuickGraph2[4].series[i].active = 1;
                     mQuickGraph2[4].series[i].color = mPlayer.syn[i].color;
                     mQuickGraph2[4].add_data(i, mPlayer.loc[i].tx_bytes_per_tally/1000);

                     mQuickGraph2[5].series[i].active = 1;
                     mQuickGraph2[5].series[i].color = mPlayer.syn[i].color;
                     mQuickGraph2[5].add_data(i, mPlayer.loc[i].rewind);
                  }
               mQuickGraph2[0].new_entry_pos();
               mQuickGraph2[1].new_entry_pos();
               mQuickGraph2[2].new_entry_pos();
               mQuickGraph2[3].new_entry_pos();
               mQuickGraph2[4].new_entry_pos();
               mQuickGraph2[5].new_entry_pos();
               mQuickGraph2[6].new_entry_pos();
               mQuickGraph2[7].new_entry_pos();
            }
         }
      }
   }
}

void mwNetgame::client_process_stdf_packet(double timestamp)
{
   int p = mPlayer.active_local_player;
   int src = PacketGetInt4();
   int dst = PacketGetInt4();
   int seq = PacketGetInt1();
   int max_seq = PacketGetInt1();
   int sb = PacketGetInt4();
   int sz = PacketGetInt4();

   mLog.addf(LOG_NET_stdf_packets, p, "rx stdf piece [%d of %d] [%d to %d] st:%4d sz:%4d\n", seq+1, max_seq, src, dst, sb, sz);

   mPlayer.loc[p].client_last_stdf_rx_frame_num = mLoop.frame_num;      // client keeps track of last stdf rx'd and quits if too long


   memcpy(client_state_buffer + sb, packetbuffer+22, sz);     // put the piece of data in the buffer
   client_state_buffer_pieces[seq] = dst;                     // mark it with destination mLoop.frame_num
   int complete = 1;                                          // did we just get the last packet? (yes by default)
   for (int i=0; i< max_seq; i++)
      if (client_state_buffer_pieces[i] != dst) complete = 0; // no, if any piece not at latest frame_num

   if (complete)
   {
      // uncompress client_state_buffer to dif
      uLongf destLen = sizeof(client_state_dif);
      uncompress((Bytef*)client_state_dif, (uLongf*)&destLen, (Bytef*)client_state_buffer, sizeof(client_state_buffer));

      if (destLen == STATE_SIZE)
      {
         mLog.addf(LOG_NET_stdf, p, "rx dif complete [%d to %d] dsync[%3.1fms] - uncompressed\n", src, dst, mPlayer.loc[p].dsync*1000);
         client_state_dif_src = src; // mark dif data with new src and dst
         client_state_dif_dst = dst;
      }
      else
      {
         mLog.addf(LOG_NET_stdf, p, "rx dif complete [%d to %d] dsync[%3.1f] - bad uncompress\n", src, dst, mPlayer.loc[p].dsync*1000);
         client_state_dif_src = -1; // mark dif data as bad
         client_state_dif_dst = -1;
      }
   }
}


void mwNetgame::client_apply_dif(void)
{
   int p = mPlayer.active_local_player;

   mLog.addf(LOG_NET_dif_applied, p, "----- Apply dif [%d to %d] ", client_state_dif_src, client_state_dif_dst);

   // check if dif is valid
   if ((client_state_dif_src == -1) || (client_state_dif_dst == -1))
   {
      mLog.app(LOG_NET_dif_not_applied, "[not applied] [dif not valid]\n");
      return;
   }

   // check if dif_dest has already been applied (check if dif_dest is less than or equal to newest_state_frame_num)
   if (client_state_dif_dst <= mStateHistory[p].newest_state_frame_num)
   {
      mLog.app(LOG_NET_dif_not_applied, "[not applied] [not newer than last dif applied]\n");
      return;
   }

   // compare dif destination to current frame number
   int ff = mPlayer.loc[p].rewind = mLoop.frame_num - client_state_dif_dst;
   char tmsg[64];
   if (ff == 0) sprintf(tmsg, "exact frame match [%d]", mLoop.frame_num);
   if (ff > 0)  sprintf(tmsg, "rewound [%d] frames", ff);
   if (ff < 0)
   {
      if (mLoop.frame_num == 0) sprintf(tmsg, "initial state");
      else
      {
         mLog.appf(LOG_NET_dif_not_applied, "[not applied] [dst is %d frames in the future!]\n", client_state_dif_src, client_state_dif_dst, -ff);
         return;
      }
   }

   // if we got this far, we know that:
   // - dif is valid
   // - dif destination is not in the future
   // - dif destination is newer than last applied dif

   // now check if we have a base state that matches dif source
   char base[STATE_SIZE] = {0};
   int base_frame_num = 0;

   // finds and sets base matching 'client_state_dif_src' -- if not found, leaves base as is (zero)
   mStateHistory[p].get_base_state(base, base_frame_num, client_state_dif_src);

   if (base_frame_num == 0) mPlayer.loc[p].client_base_resets++;

   if ((base_frame_num == 0) && (client_state_dif_src != 0))
   {
      int fn = mStateHistory[p].newest_state_frame_num;
      mLog.appf(LOG_NET_dif_not_applied, "[not applied] [base not found] - resending stak [%d]\n", fn);
      client_send_stak(fn);
      return;
   }



   // ------------------------------------------------
   // save things before applying dif
   // ------------------------------------------------

   // make a copy of level array l[][]
   int old_l[100][100];
   memcpy(old_l, mLevel.l, sizeof(mLevel.l));

   // make a copy of players' pos
   for (int pp=0; pp<NUM_PLAYERS; pp++)
      if (mPlayer.syn[pp].active)
      {
         mPlayer.loc[pp].old_x = mPlayer.syn[pp].x;
         mPlayer.loc[pp].old_y = mPlayer.syn[pp].y;
      }


   // apply dif to base
   apply_state_dif(base, client_state_dif, STATE_SIZE);

   // copy to game vars and set new frame number
   state_to_game_vars(base);
   mLoop.frame_num = client_state_dif_dst;

   // keep track of frame number when last client dif was applied
   mPlayer.loc[p].client_last_dif_applied = mLoop.frame_num;

   // save to history
   mStateHistory[p].add_state(mLoop.frame_num);
   //mStateHistory[p].show_states("save frame:%d to history\n", mLoop.frame_num);


   // add log entry
   mLog.appf(LOG_NET_dif_not_applied, "[applied] [%s]\n", tmsg);
   // ------------------------------------------------
   // restore things after applying dif
   // ------------------------------------------------

   // fix control methods
   mPlayer.syn[0].control_method = 2; // on client, server is always control method 2
   if (mPlayer.syn[p].control_method == 2) mPlayer.syn[p].control_method = 4;
   if (mPlayer.syn[p].control_method == 8) mLoop.state[0] = 1; // server quit


   // compare old_l to l and redraw changed tiles
   // double t0 = al_get_time();
   al_set_target_bitmap(mBitmap.level_background);
   for (int x=0; x<100; x++)
      for (int y=0; y<100; y++)
         if (mLevel.l[x][y] != old_l[x][y])
         {
            // printf("dif at x:%d y:%d\n", x, y);
            al_draw_filled_rectangle(x*20, y*20, x*20+20, y*20+20, mColor.pc[0]);
            al_draw_bitmap(mBitmap.btile[mLevel.l[x][y] & 1023], x*20, y*20, 0);
         }
   // mLog.add_log_TMR(al_get_time() - t0, "oldl", 0);

   // ------------------------------------------------
   // if we rewound time, play it back
   // ------------------------------------------------
   if (ff) mLoop.loop_frame(ff);

   // send acknowledgement
   client_send_stak(client_state_dif_dst);

   // ------------------------------------------------
   // calc players' corrections
   // ------------------------------------------------
   mPlayer.loc[p].client_rmt_plr_cor = 0; // reset max remote
   for (int pp=0; pp<NUM_PLAYERS; pp++)
      if (mPlayer.syn[pp].active)
      {
         float cor = sqrt(pow((mPlayer.loc[pp].old_x - mPlayer.syn[pp].x), 2) + pow((mPlayer.loc[pp].old_y - mPlayer.syn[pp].y), 2));  // hypotenuse distance
         if (p == pp) mPlayer.loc[p].client_loc_plr_cor = cor; // save local cor
         else if (cor > mPlayer.loc[p].client_rmt_plr_cor) mPlayer.loc[p].client_rmt_plr_cor = cor; // save max remote cor
      }

   // add data to tally
   mTally_client_loc_plr_cor_last_sec[p].add_data(mPlayer.loc[p].client_loc_plr_cor);
   mTally_client_rmt_plr_cor_last_sec[p].add_data(mPlayer.loc[p].client_rmt_plr_cor);
}


void mwNetgame::client_send_stak(int ack_frame)
{
   int p = mPlayer.active_local_player;
   Packet("stak");
   PacketPutInt1(p);
   PacketPutInt4(ack_frame);
   PacketPutInt4(mLoop.frame_num);
   PacketPutDouble(mPlayer.loc[p].client_chase_fps);
   PacketPutDouble(mPlayer.loc[p].dsync_avg);
   PacketPutInt1(mPlayer.loc[p].rewind);
   PacketPutDouble(mPlayer.loc[p].client_loc_plr_cor);
   PacketPutDouble(mPlayer.loc[p].client_rmt_plr_cor);
   PacketPutDouble(mPlayer.loc[p].cpu);

   ClientSend(packetbuffer, packetsize);
   mLog.addf(LOG_NET_stak, p, "tx stak p:%d ack:[%d] cur:[%d]\n", p, ack_frame, mLoop.frame_num);
}


void mwNetgame::client_timer_adjust(void)
{
   // iterate all stdf packets, calc dysnc, then get max dysnc
   float max_dsync = -1000;
   int stdf_count = 0;

   // iterate all stdf packets stored in the packet buffer
   for (int i=0; i<200; i++)
      if ((packet_buffers[i].active) && (packet_buffers[i].type == 1))
      {
         stdf_count++;
         int dst = 0;
         memcpy(&dst, packet_buffers[i].data+8, 4);

         // calc dysnc
         float csync = (float)(dst - mLoop.frame_num) * 0.025;   // crude integer sync based on frame numbers
         float dsync = al_get_time() - packet_buffers[i].timestamp + csync;  // add time between now and when the packet was received into packet buffer

         if (dsync > max_dsync) max_dsync = dsync;
      }

   int p = mPlayer.active_local_player;
   if (max_dsync > -1000)
   {
      mPlayer.loc[p].dsync = max_dsync;
      mRollingAverage[2].add_data(mPlayer.loc[p].dsync);    // send to rolling average
      mPlayer.loc[p].dsync_avg = mRollingAverage[2].avg;    // get average

      // in mode 2 only - get offset from server
      if (client_chase_offset_mode == 2) client_chase_offset = mPlayer.syn[0].client_chase_offset;

      // set point
      float sp = client_chase_offset;

      // error = set point - measured value
      float err = sp - mPlayer.loc[p].dsync_avg;

      // instantaneous error adjustment (proportional)
      float p_adj = err * 80;

      // cumulative error adjust (integral)
      tmaj_i += err;
      float i_clamp = 5;
      if (tmaj_i >  i_clamp) tmaj_i =  i_clamp;
      if (tmaj_i < -i_clamp) tmaj_i = -i_clamp;
      tmaj_i *= 0.95; // decay
      float i_adj = tmaj_i * 0;

      // combine to get total adjust
      float t_adj = p_adj + i_adj;

      // adjust speed
      float fps_chase = mLoop.frame_speed - t_adj;

      if (fps_chase < 10) fps_chase = 10; // never let this go negative
      if (fps_chase > 70) fps_chase = 70;

      al_set_timer_speed(mEventQueue.fps_timer, (1 / fps_chase));

      mPlayer.loc[p].client_chase_fps = fps_chase;

      mLog.add_tmrf(LOG_TMR_client_timer_adj, 0, "dsc:[%5.2f] dsa:[%5.2f] sp:[%5.2f] er:[%6.2f] pa:[%6.2f] ia:[%6.2f] ta:[%6.2f]\n", mPlayer.loc[p].dsync*1000, mPlayer.loc[p].dsync_avg*1000, sp*1000, err*1000, p_adj, i_adj, t_adj);
      mLog.addf(LOG_NET_timer_adjust, p, "timer adjust dsc[%5.1f] dsa[%5.1f] off[%3.1f] chs[%3.3f]\n", mPlayer.loc[p].dsync*1000, mPlayer.loc[p].dsync_avg*1000, sp*1000, fps_chase);
   }
   // else mLog.addf(LOG_NET_timer_adjust, p, "timer adjust no stdf packets this frame dsc[%5.1f] dsa[%5.1f]\n", mPlayer.loc[p].dsync*1000, mPlayer.loc[p].dsync_avg*1000);
}

void mwNetgame::client_proc_player_drop(void)
{
   int p = mPlayer.active_local_player;
   if (mPlayer.syn[p].control_method == 8)
   {
      mPlayer.loc[p].quit_reason = 92;
      mLog.log_ending_stats_client(LOG_NET_ending_stats, p);
      mScreen.rtextout_centre(mFont.bltn, NULL, mDisplay.SCREEN_W/2, mDisplay.SCREEN_H/2, 10, -2, 1, "SERVER ENDED GAME!");
      al_flip_display();
      mInput.tsw();
      mLoop.state[0] = 1;
   }

   int lsf = mPlayer.loc[p].client_last_stdf_rx_frame_num;
   if ((mLoop.frame_num > 0) && (lsf > 0)) // check to see if server connection is lost
   {
      int ss = mLoop.frame_num - lsf;
      if (ss > 120)
      {
         mPlayer.loc[p].quit_reason = 75;
         mLog.add_fwf(LOG_NET, p, 76, 10, "+", "-", "");
         mLog.add_fwf(LOG_NET, p, 76, 10, "|", " ", "Local Player Client %d Lost Server Connection!", p);
         mLog.add_fwf(LOG_NET, p, 76, 10, "|", " ", "mLoop.frame_num:[%d] last_stdf_rx:[%d] dif:[%d]", mLoop.frame_num, lsf, ss);
         mLog.add_fwf(LOG_NET, p, 76, 10, "+", "-", "");
         mLog.log_ending_stats_client(LOG_NET_ending_stats, p);

         mScreen.rtextout_centre(mFont.bltn, NULL, mDisplay.SCREEN_W/2, mDisplay.SCREEN_H/2, 10, -2, 1, "LOST SERVER CONNECTION!");
         al_flip_display();
         mInput.tsw();
         mLoop.state[0] = 1;
      }
   }
}


void mwNetgame::client_fast_packet_loop(void)
{
   int p = mPlayer.active_local_player;

   while ((packetsize = ClientReceive(packetbuffer)))
   {
      double timestamp = al_get_time();

      if (PacketRead("pong"))
      {
         double t0 = PacketGetDouble();
         double t1 = PacketGetDouble();
         double t2 = al_get_time();

         Packet("pang");
         PacketPutDouble(t1);
         ClientSend(packetbuffer, packetsize);

         mPlayer.loc[p].ping = t2 - t0;

         //printf("%d rx pong [%3.1f ms] - send pang\n", mLoop.frame_num, mPlayer.loc[p].ping * 1000);

         mRollingAverage[1].add_data(mPlayer.loc[p].ping); // send to rolling average
         mPlayer.loc[p].ping_avg = mRollingAverage[1].avg;

         // adjust client chase offset based on ping time
         if (client_chase_offset_mode == 1) client_chase_offset = - mPlayer.loc[p].ping_avg + client_chase_offset_auto_offset;

         mLog.addf(LOG_NET_client_ping, p, "ping [%3.2f] avg[%3.2f]\n", mPlayer.loc[p].ping*1000, mPlayer.loc[p].ping_avg*1000);
         if (mLoop.frame_num) mLog.add_tmrf(LOG_TMR_client_ping, p, "ping:[%5.2f] pavg:[%5.2f]\n", mPlayer.loc[p].ping*1000, mPlayer.loc[p].ping_avg*1000);
      }

      int type = 0;
      if (PacketRead("stdf")) type = 1;
      if (PacketRead("sjon")) type = 2;
      // printf("type:%d\n", type);
      if (type)
      {
         // find empty
         int indx = -1;
         for (int i=0; i<200; i++) if (!packet_buffers[i].active)
         {
            indx = i;
            break;
         }


         if (indx == -1)
         {
            mLog.add(LOG_NET, 0, "Packet buffer full!\n");
            printf("[%d] Packet buffer full!\n", mLoop.frame_num);

            // count types of packets in buffer
            int stdf_count = 0;
            int sjon_count = 0;
            int all_count = 0;

            for (int i=0; i<200; i++)
               if (!packet_buffers[i].active)
               {
                  all_count++;
                  if (packet_buffers[i].type == 1) stdf_count++;
                  if (packet_buffers[i].type == 2) sjon_count++;
               }
            mLog.addf(LOG_NET, 0, "[%d] stdf\n", stdf_count);
            mLog.addf(LOG_NET, 0, "[%d] sjon\n", sjon_count);
            mLog.addf(LOG_NET, 0, "[%d] all\n", all_count);

            printf("[%d] stdf\n", stdf_count);
            printf("[%d] sjon\n", sjon_count);
            printf("[%d] all\n", all_count);
         }
         else
         {
            //printf("%d stored packet:%d size:%d type:%d\n", mLoop.frame_num, i, packetsize, type);
            packet_buffers[indx].active = 1;
            packet_buffers[indx].type = type;
            packet_buffers[indx].timestamp = timestamp;
            packet_buffers[indx].packetsize = packetsize;
            memcpy(packet_buffers[indx].data, packetbuffer, 1024);
         }
      }
   }
}

// process all packets stored in the packet buffer
void mwNetgame::client_read_packet_buffer(void)
{
   for (int i=0; i<200; i++)
      if (packet_buffers[i].active)
      {
         memcpy(packetbuffer, packet_buffers[i].data, 1024);
         packetsize = packet_buffers[i].packetsize;
         set_packetpos(4);
         if (packet_buffers[i].type == 1) client_process_stdf_packet(packet_buffers[i].timestamp);
         if (packet_buffers[i].type == 2) client_process_sjon_packet();
         packet_buffers[i].active = 0;
      }
}

void mwNetgame::client_control(void)
{
   client_timer_adjust();
   client_read_packet_buffer();
   client_apply_dif();
   client_proc_player_drop();
   process_bandwidth_counters(mPlayer.active_local_player);
}

