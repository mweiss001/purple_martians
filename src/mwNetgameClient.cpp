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
      mLog.add_fwf(LOG_NET_network_setup, 0, 76, 10, "|", " ", "Client network (TCP) initialized -- server[%s]", serveraddress);
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
      mLog.add_fwf(LOG_NET_network_setup, 0, 76, 10, "|", " ", "Client network (UDP) initialized -- server:[%s]", serveraddress);
      //mLog.add_fwf(LOG_NET_network_setup, 0, 76, 10, "|", " ", "Local address%s", net_getlocaladdress(ServerChannel));
   }


   // Check for reply from server
   int tries = 1;        // number of times to try
   float try_delay = 1;  // delay between tries
   int got_reply = 0;

   while (!got_reply)
   {
      mLog.add_fwf(LOG_NET_network_setup, 0, 76, 10, "|", " ", "ClientCheckResponse %d", tries);
      int ccr = ClientCheckResponse();
      if (ccr == 1)
      {
         got_reply = 1;
         mLog.add_fw(LOG_NET_network_setup, 0, 76, 10, "|", " ", "Got reply from server");
      }
      if (ccr == 0)
      {
         al_rest(try_delay);
         if (++tries > 2)
         {
            mLog.add_fw(LOG_error, 0, 76, 10, "|", " ", "Did not get reply from server");

            mLog.add_fw(LOG_NET_network_setup, 0, 76, 10, "+", "-", "");
            return 0;
         }
      }
      if (ccr == -1)
      {
         mLog.add_fw(LOG_error, 0, 76, 10, "|", " ", "TCP response Error!");
         mLog.add_fw(LOG_NET_network_setup, 0, 76, 10, "+", "-", "");
         return 0;
      }
      if (ccr == -2)
      {
         mLog.add_fw(LOG_error, 0, 76, 10, "|", " ", "Cancelled");
         mLog.add_fw(LOG_NET_network_setup, 0, 76, 10, "+", "-", "");
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
      mLog.add_fw(LOG_NET_network_setup, 0, 76, 10, "|", " ", "Sent initial packet (1234) to server");
      mLog.add_fw(LOG_NET_network_setup, 0, 76, 10, "|", " ", "Waiting for reply");

      int done = 10;
      while (done)
      {
         mEventQueue.proc();
         if (mInput.key[ALLEGRO_KEY_ESCAPE][1]) return -2;
         packetsize = net_receive(ServerChannel, packetbuffer, 1024, address);
         if (packetsize)
         {
            set_packetpos(0);
            if (PacketRead("5678"))
            {
               mLog.add_fw(LOG_NET_network_setup, 0, 76, 10, "|", " ", "Got Response");
               net_assigntarget(ServerChannel, address);
               return 1;
            }
         }
         al_rest(.1);
         done--;
      }
      mLog.add_fw(LOG_NET_network_setup, 0, 76, 10, "|", " ", "No Response");
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
   mLog.add_header(LOG_NET_network_setup, 0, 0, "Shutting down the client network");
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
   mLog.add_fw (LOG_NET_network_setup, 0, 76, 10, "+", "-", "");
   mLog.add_fwf(LOG_NET_network_setup, 0, 76, 10, "|", " ", "Client mode started on localhost:[%s]", mLoop.local_hostname);

    // initialize driver with server address
   if (!ClientInitNetwork(m_serveraddress)) return 0;

   mRollingAverage[1].initialize(); // ping rolling average
   mRollingAverage[2].initialize(); // dsync rolling average

   client_chase_offset = 0.0;
   client_chase_offset_auto_offset = -0.02;
   client_chase_offset_mode = 1; // 0 = manual, 1 = auto

   Packet("cjon");
   PacketPut1ByteInt(mPlayer.syn[0].color); // requested color
   PacketAddString(mLoop.local_hostname);
   ClientSend(packetbuffer, packetsize);

   mLog.add_fwf(LOG_NET_network_setup, 0, 76, 10, "|", " ", "Client sent join request to server with player color:[%2d]", mPlayer.syn[0].color);
   mLog.add_fw (LOG_NET_network_setup, 0, 76, 10, "+", "-", "");
   return 1;
}

void mwNetgame::client_process_sjon_packet(void)
{
   int pl = PacketGet2ByteInt();   // play level
   int server_sjon_frame_num  =  PacketGet4ByteInt();

   int p = PacketGet1ByteInt();      // client player number
   int color = PacketGet1ByteInt();  // client player color
   int ds = PacketGet1ByteInt();     // deathmatch_shots
   int dd = PacketGet2ByteInt();     // deathmatch_shot_damage
   int ss = PacketGet1ByteInt();     // suicide_shots

   if (p == 99) // server full, join denied
   {
      mLog.add(LOG_error, 0, "Server replied with 'SERVER FULL'\n");
      mLoop.state[0] = 25;
   }
   else // join allowed
   {
      mPlayer.active_local_player = p;
      mPlayer.syn[p].control_method = 4;
      mPlayer.syn[p].color = color;
      strncpy(mPlayer.loc[0].hostname, m_serveraddress, 16);
      strncpy(mPlayer.loc[p].hostname, mLoop.local_hostname, 16);
      ima_client = 1;

      mLevel.play_level = pl;

      mShot.deathmatch_shots = ds;
      mShot.deathmatch_shot_damage = dd-1000;
      mShot.suicide_shots = ss;

      mLog.add_fwf(LOG_NET_network_setup, 0, 76, 10, "|", " ", "Client received join invitation from server");
      mLog.add_fwf(LOG_NET_join_details,  0, 76, 10, "|", " ", "Level:[%d]", mLevel.play_level);
      mLog.add_fwf(LOG_NET_join_details,  0, 76, 10, "|", " ", "Player Number:[%d]", p);
      mLog.add_fwf(LOG_NET_join_details,  0, 76, 10, "|", " ", "Player Color:[%d]", color);
      mLog.add_fwf(LOG_NET_join_details,  0, 76, 10, "|", " ", "Server Frame Num:[%d]", server_sjon_frame_num);
      mLog.add_fwf(LOG_NET_join_details,  0, 76, 10, "|", " ", "Deathmatch player shots:[%d]", mShot.deathmatch_shots);
      mLog.add_fwf(LOG_NET_join_details,  0, 76, 10, "|", " ", "Deathmatch player shot damage:[%d]", mShot.deathmatch_shot_damage);
      mLog.add_fwf(LOG_NET_join_details,  0, 76, 10, "|", " ", "Suicide player shots:[%d]", mShot.suicide_shots);
      mLog.add_fwf(LOG_NET_network_setup, 0, 76, 10, "+", "-", "");

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




void mwNetgame::client_send_stak(void)
{
   int p = mPlayer.active_local_player;
   Packet("stak");
   PacketPut1ByteInt(p);
   PacketPut4ByteInt(client_state_base_frame_num);
   PacketPut4ByteInt(mLoop.frame_num);
   PacketPutDouble(mPlayer.loc[p].client_chase_fps);
   PacketPutDouble(mPlayer.loc[p].dsync_avg);
   ClientSend(packetbuffer, packetsize);
}

void mwNetgame::client_apply_dif(void)
{
   int p = mPlayer.active_local_player;
   if ((client_state_dif_src == -1) || (client_state_dif_dst == -1)) // check if valid dif
   {
      mLog.addf(LOG_NET_dif_not_applied, p, "dif is not valid - src:%d dst:%d\n", client_state_dif_src, client_state_dif_dst);
   }
   else
   {
      if ((client_state_dif_src - mLoop.frame_num > 100) && (mLoop.frame_num != 0))
      {
         mLog.addf(LOG_NET_dif_not_applied, p, "dif_src is > 100 frames in the future - src:%d mLoop.frame_num:%d\n", client_state_dif_src, mLoop.frame_num);
      }
      else
      {
         if ((client_state_dif_dst <= client_state_base_frame_num)) // dif destination is not newer than last applied dif
         {
            mLog.addf(LOG_NET_dif_not_applied, p, "dif [%d to %d] not applied - not newer than current [%d]\n", client_state_dif_src, client_state_dif_dst, client_state_base_frame_num);
         }
         else
         {
            if (client_state_dif_src == 0) // if server has sent dif from src == 0, reset client base to 0
            {
               memset(client_state_base, 0, STATE_SIZE);
               client_state_base_frame_num = 0;
               mPlayer.loc[p].client_base_resets++;
               mLog.addf(LOG_NET_dif_not_applied, p, "Resetting client base state to zero\n");
            }

            if (client_state_base_frame_num != client_state_dif_src) // stored base state does NOT match dif source
            {
               client_send_stak(); // resend ack to server with correct acknowledged base state
               mLog.addf(LOG_NET_dif_not_applied, p, "dif cannot be applied (wrong client base) %d %d\n", client_state_base_frame_num, client_state_dif_src);
            }
            else // we have a matching base to apply dif
            {
               int ff = mPlayer.loc[p].client_rewind = mLoop.frame_num - client_state_dif_dst; // dst compared to current frame_num

               char tmsg[64];
               if (ff == 0) sprintf(tmsg, "exact frame match [%d]\n", mLoop.frame_num);
               if (ff > 0)  sprintf(tmsg, "rewind [%d] frames\n", ff);
               if (ff < 0)  sprintf(tmsg, "early [%d] frames\n", -ff);
               if (mLoop.frame_num == 0) sprintf(tmsg, "initial state\n");
               if ((ff < 0) && (mLoop.frame_num != 0))
               {
                  mLog.addf(LOG_NET_dif_not_applied, p, "dif [%d to %d] not applied yet - [%d] early\n", client_state_dif_src, client_state_dif_dst, -ff);
               }
               else
               {
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



                  // apply dif to base state
                  apply_state_dif(client_state_base, client_state_dif, STATE_SIZE);

                  // copy modified base state to game_vars
                  state_to_game_vars(client_state_base);




                  // double t0 = al_get_time();

                  // compare old_l to l and redraw changed tiles
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


                  // fix control methods
                  mPlayer.syn[0].control_method = 2; // on client, server is mode 2
                  if (mPlayer.syn[p].control_method == 2) mPlayer.syn[p].control_method = 4;
                  if (mPlayer.syn[p].control_method == 8) mLoop.state[0] = 1; // server quit

                  // update mLoop.frame_num and client base mLoop.frame_num
                  mLoop.frame_num = client_state_base_frame_num = client_state_dif_dst;

                  mPlayer.loc[p].client_last_dif_applied = mLoop.frame_num;

                  if (ff) mLoop.loop_frame(ff); // if we rewound time, play it back


                  // calc players' correction
                  for (int pp=0; pp<NUM_PLAYERS; pp++)
                     if (mPlayer.syn[pp].active)
                     {
                        mPlayer.loc[pp].cor = sqrt(pow((mPlayer.loc[pp].old_x - mPlayer.syn[pp].x), 2) + pow((mPlayer.loc[pp].old_y - mPlayer.syn[pp].y), 2));  // hypotenuse distance
                        if (mPlayer.loc[pp].cor > mPlayer.loc[pp].cor_max)  mPlayer.loc[pp].cor_max = mPlayer.loc[pp].cor;
                     }


                  // reset max players correction
                  if (mLoop.frame_num > mPlayer.loc[p].cor_reset_frame)
                  {
                     mPlayer.loc[p].cor_reset_frame = mLoop.frame_num + 100;
                     for (int pp=0; pp<NUM_PLAYERS; pp++)
                        if (mPlayer.syn[pp].active) mPlayer.loc[pp].cor_max = 0;
                  }
                  client_send_stak();
                  //if (client_state_dif_src) mPlayer.loc[p].client_move_lag = mLoop.frame_num - client_state_dif_src;
                  mLog.addf(LOG_NET_dif_applied, p, "dif [%d to %d] applied - %s", client_state_dif_src, client_state_dif_dst, tmsg);
               }
            }
         }
      }
   }
}


void mwNetgame::client_timer_adjust(void)
{
   int p = mPlayer.active_local_player;

   float mva = mPlayer.loc[p].dsync_avg;            // measured value
   float mv = mPlayer.loc[p].dsync;                 // measured value

//   float sp = mPlayer.loc[p].client_chase_offset;   // set point

   float sp = client_chase_offset;   // set point

   float err = sp - mva;                          // error = set point - measured value

   float p_adj = err * 80; // instantaneous error adjustment (proportional)

   tmaj_i += err; // cumulative error adjust (integral)
   float i_clamp = 5;
   if (tmaj_i >  i_clamp) tmaj_i =  i_clamp;
   if (tmaj_i < -i_clamp) tmaj_i = -i_clamp;
   tmaj_i *= 0.95; // decay
   float i_adj = tmaj_i * 0;

   float t_adj = p_adj + i_adj; // total adj

   float fps_chase = mLoop.frame_speed - t_adj;

   if (fps_chase < 10) fps_chase = 10; // never let this go negative
   if (fps_chase > 70) fps_chase = 70;
   al_set_timer_speed(mEventQueue.fps_timer, (1 / fps_chase));
   mPlayer.loc[p].client_chase_fps = fps_chase;

   mLog.add_tmrf(LOG_TMR_client_timer_adj, 0, "mv:[%5.2f] ma:[%5.2f] sp:[%5.2f] er:[%6.2f] pa:[%6.2f] ia:[%6.2f] ta:[%6.2f]\n", mv*1000, mva*1000, sp*1000, err*1000, p_adj, i_adj, t_adj);
   mLog.addf(LOG_NET_timer_adjust, p, "timer adjust dsync[%3.2f] offset[%3.2f] fps_chase[%3.3f]\n", mPlayer.loc[p].dsync*1000, sp*1000, fps_chase);
}


void mwNetgame::client_process_stdf_packet(double timestamp)
{
   int p = mPlayer.active_local_player;

   int src = PacketGet4ByteInt();
   int dst = PacketGet4ByteInt();
   int seq = PacketGet1ByteInt();
   int max_seq = PacketGet1ByteInt();
   int sb = PacketGet4ByteInt();
   int sz = PacketGet4ByteInt();


   mLog.addf(LOG_NET_stdf_packets, p, "rx stdf piece [%d of %d] [%d to %d] st:%4d sz:%4d \n", seq+1, max_seq, src, dst, sb, sz);



   int client_sync = dst - mLoop.frame_num;                             // crude integer sync based on frame numbers

   if (mPlayer.loc[p].client_last_stdf_rx_frame_num != mLoop.frame_num)    // this is the first stdf received for this frame
   {
      mPlayer.loc[p].client_last_stdf_rx_frame_num = mLoop.frame_num;      // client keeps track of last stdf rx'd and quits if too long
      mPlayer.loc[p].dsync = al_get_time() - timestamp;              // time between when the packet was received into the packet buffer and now
      mPlayer.loc[p].dsync += (double) client_sync * 0.025;          // combine with client_sync

      mRollingAverage[2].add_data(mPlayer.loc[p].dsync); // send to rolling average
      mPlayer.loc[p].dsync_avg = mRollingAverage[2].avg;

      client_timer_adjust();
   }

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

void mwNetgame::process_bandwidth_counters(int p)
{
   // get maximums per frame
   if (mPlayer.loc[p].tx_current_packets_for_this_frame > mPlayer.loc[p].tx_max_packets_per_frame) mPlayer.loc[p].tx_max_packets_per_frame = mPlayer.loc[p].tx_current_packets_for_this_frame;
   if (mPlayer.loc[p].tx_current_bytes_for_this_frame >   mPlayer.loc[p].tx_max_bytes_per_frame)   mPlayer.loc[p].tx_max_bytes_per_frame =   mPlayer.loc[p].tx_current_bytes_for_this_frame;
   if (mPlayer.loc[p].rx_current_packets_for_this_frame > mPlayer.loc[p].rx_max_packets_per_frame) mPlayer.loc[p].rx_max_packets_per_frame = mPlayer.loc[p].rx_current_packets_for_this_frame;
   if (mPlayer.loc[p].rx_current_bytes_for_this_frame >   mPlayer.loc[p].rx_max_bytes_per_frame)   mPlayer.loc[p].rx_max_bytes_per_frame =   mPlayer.loc[p].rx_current_bytes_for_this_frame;

   // get totals
   mPlayer.loc[p].tx_total_bytes   += mPlayer.loc[p].tx_current_bytes_for_this_frame;
   mPlayer.loc[p].tx_total_packets += mPlayer.loc[p].tx_current_packets_for_this_frame;
   mPlayer.loc[p].rx_total_bytes   += mPlayer.loc[p].rx_current_bytes_for_this_frame;
   mPlayer.loc[p].rx_total_packets += mPlayer.loc[p].rx_current_packets_for_this_frame;

   // add to tallies
   mPlayer.loc[p].tx_bytes_tally   += mPlayer.loc[p].tx_current_bytes_for_this_frame;
   mPlayer.loc[p].tx_packets_tally += mPlayer.loc[p].tx_current_packets_for_this_frame;
   mPlayer.loc[p].rx_bytes_tally   += mPlayer.loc[p].rx_current_bytes_for_this_frame;
   mPlayer.loc[p].rx_packets_tally += mPlayer.loc[p].rx_current_packets_for_this_frame;

   // reset counts for this frame
   mPlayer.loc[p].tx_current_bytes_for_this_frame = 0;
   mPlayer.loc[p].tx_current_packets_for_this_frame = 0;
   mPlayer.loc[p].rx_current_bytes_for_this_frame = 0;
   mPlayer.loc[p].rx_current_packets_for_this_frame = 0;

   if (mLoop.frame_num % 40 == 0) // tally freq = 40 frames = 1s
   {
      // get maximums per tally
      if (mPlayer.loc[p].tx_bytes_per_tally >   mPlayer.loc[p].tx_max_bytes_per_tally)   mPlayer.loc[p].tx_max_bytes_per_tally =   mPlayer.loc[p].tx_bytes_per_tally;
      if (mPlayer.loc[p].rx_bytes_per_tally >   mPlayer.loc[p].rx_max_bytes_per_tally)   mPlayer.loc[p].rx_max_bytes_per_tally =   mPlayer.loc[p].rx_bytes_per_tally;
      if (mPlayer.loc[p].tx_packets_per_tally > mPlayer.loc[p].tx_max_packets_per_tally) mPlayer.loc[p].tx_max_packets_per_tally = mPlayer.loc[p].tx_packets_per_tally;
      if (mPlayer.loc[p].rx_packets_per_tally > mPlayer.loc[p].rx_max_packets_per_tally) mPlayer.loc[p].rx_max_packets_per_tally = mPlayer.loc[p].rx_packets_per_tally;

      // copy to display variables
      mPlayer.loc[p].tx_bytes_per_tally   = mPlayer.loc[p].tx_bytes_tally;
      mPlayer.loc[p].tx_packets_per_tally = mPlayer.loc[p].tx_packets_tally;
      mPlayer.loc[p].rx_bytes_per_tally   = mPlayer.loc[p].rx_bytes_tally;
      mPlayer.loc[p].rx_packets_per_tally = mPlayer.loc[p].rx_packets_tally;

      mLog.addf(LOG_NET_bandwidth, p, "bandwidth txb:[%d] rxb:[%d] txp:[%d] rxp:[%d]\n", mPlayer.loc[p].tx_bytes_per_tally, mPlayer.loc[p].rx_bytes_per_tally, mPlayer.loc[p].tx_packets_per_tally, mPlayer.loc[p].rx_packets_per_tally);

      // reset tallies
      mPlayer.loc[p].tx_bytes_tally = 0;
      mPlayer.loc[p].tx_packets_tally = 0;
      mPlayer.loc[p].rx_bytes_tally = 0;
      mPlayer.loc[p].rx_packets_tally = 0;
   }
}

void mwNetgame::client_proc_player_drop(void)
{
   int p = mPlayer.active_local_player;
   if (mPlayer.syn[p].control_method == 8)
   {
      mPlayer.loc[p].quit_reason = 92;
      mLog.log_ending_stats_client(LOG_NET_ending_stats, p);
      mLoop.state[0] = 1;
      mScreen.rtextout_centre(mFont.bltn, NULL, mDisplay.SCREEN_W/2, mDisplay.SCREEN_H/2, 10, -2, 1, "SERVER ENDED GAME!");
      al_flip_display();
      mInput.tsw();
   }

   int lsf = mPlayer.loc[p].client_last_stdf_rx_frame_num;
   if ((mLoop.frame_num > 0) && (lsf > 0)) // check to see if server connection is lost
   {
      int ss = mLoop.frame_num - lsf;
      if (ss > 120)
      {
         mPlayer.loc[p].quit_reason = 75;
         mLog.add_fwf(LOG_NET_network_setup, p, 76, 10, "+", "-", "");
         mLog.add_fwf(LOG_NET_network_setup, p, 76, 10, "|", " ", "Local Player Client %d Lost Server Connection!", p);
         mLog.add_fwf(LOG_NET_network_setup, p, 76, 10, "|", " ", "mLoop.frame_num:[%d] last_stdf_rx:[%d] dif:[%d]", mLoop.frame_num, lsf, ss);
         mLog.add_fwf(LOG_NET_network_setup, p, 76, 10, "+", "-", "");
         mLog.log_ending_stats_client(LOG_NET_ending_stats, p);
         mLoop.state[0] = 25;
         mScreen.rtextout_centre(mFont.bltn, NULL, mDisplay.SCREEN_W/2, mDisplay.SCREEN_H/2, 10, -2, 1, "LOST SERVER CONNECTION!");
         al_flip_display();
         mInput.tsw();
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
         mPlayer.loc[p].ping = t2 - t0;
         //printf("%d rx pong [%3.1f ms] - send pang\n", mLoop.frame_num, mPlayer.loc[p].ping * 1000);

         mRollingAverage[1].add_data(mPlayer.loc[p].ping); // send to rolling average
         mPlayer.loc[p].ping_avg = mRollingAverage[1].avg;


         // adjust client chase offset based on ping
         if (client_chase_offset_mode) // auto mode
         {
            client_chase_offset = - mPlayer.loc[p].ping_avg + client_chase_offset_auto_offset;
         }
         else
         {
            //mPlayer.loc[p].client_chase_offset = mPlayer.loc[p].client_chase_offset_auto_offset;
         }

         mLog.addf(LOG_NET_client_ping, p, "ping [%3.2f] avg[%3.2f]\n", mPlayer.loc[p].ping*1000, mPlayer.loc[p].ping_avg*1000);
         if (mLoop.frame_num) mLog.add_tmrf(LOG_TMR_client_ping, p, "ping:[%5.2f] pavg:[%5.2f]\n", mPlayer.loc[p].ping*1000, mPlayer.loc[p].ping_avg*1000);

         Packet("pang");
         PacketPutDouble(t1);
         ClientSend(packetbuffer, packetsize);
      }

      int type = 0;
      if (PacketRead("stdf")) type = 1;
      if (PacketRead("sjon")) type = 2;
      // printf("type:%d\n", type);
      if (type)
      {
         for (int i=0; i<200; i++)
            if (!packet_buffers[i].active) // find empty
            {
               //printf("%d stored packet:%d size:%d type:%d\n", mLoop.frame_num, i, packetsize, type);

               packet_buffers[i].active = 1;
               packet_buffers[i].type = type;
               packet_buffers[i].timestamp = timestamp;
               packet_buffers[i].packetsize = packetsize;
               memcpy(packet_buffers[i].data, packetbuffer, 1024);
               break;
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
   client_read_packet_buffer();
   client_apply_dif();
   client_proc_player_drop();
   process_bandwidth_counters(mPlayer.active_local_player);
}

