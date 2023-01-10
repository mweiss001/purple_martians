// n_client.cpp

#include "pm.h"

// n_network.h
extern int NetworkDriver;

// these are never referenced outside of this file
NET_CONN *ServerConn = NULL;
NET_CHANNEL *ServerChannel;


int ClientInitNetwork(const char *serveraddress)
{
	if(NetworkInit())
   {
      sprintf(msg, "Error: failed to initialize network");
      m_err(msg);
      if (L_LOGGING_NETPLAY) add_log_entry_position_text(11, 0, 76, 10, msg, "|", " ");
      return 0;
   }
   if (TCP)
   {
      ServerConn = net_openconn(NetworkDriver, NULL);
      if(!ServerConn)
      {
         sprintf(msg, "Error: Client failed to create netconnection (TCP)");
         m_err(msg);
         if (L_LOGGING_NETPLAY) add_log_entry_position_text(11, 0, 76, 10, msg, "|", " ");
         return 0;
      }
      if(net_connect(ServerConn, serveraddress))
      {
         sprintf(msg, "Error: Client failed to set netconnection target: server[%s] (TCP)", serveraddress);
         m_err(msg);
         if (L_LOGGING_NETPLAY) add_log_entry_position_text(11, 0, 76, 10, msg, "|", " ");
         net_closeconn(ServerConn);
   		return 0;
   	}
      sprintf(msg, "Client network initialized: server[%s] (TCP)", serveraddress);
   }
   else // UDP
   {
      ServerChannel = net_openchannel(NetworkDriver, "");
      if (ServerChannel == NULL)
      {
         sprintf(msg, "Error: Client failed to create netchannel (UDP)");
         printf("%s\n", msg);
         m_err(msg);
         if (L_LOGGING_NETPLAY) add_log_entry_position_text(11, 0, 76, 10, msg, "|", " ");
         return 0;
      }
      if (net_assigntarget(ServerChannel, serveraddress))
      {
         sprintf(msg, "Error: Client failed to set netchannel target: server[%s] (UDP)", serveraddress);
         printf("%s\n", msg);
         m_err(msg);
         if (L_LOGGING_NETPLAY) add_log_entry_position_text(11, 0, 76, 10, msg, "|", " ");
         return 0;
      }
      sprintf(msg, "Client network (UDP) initialized: server:%s local address%s", serveraddress, net_getlocaladdress(ServerChannel));
      printf("%s\n", msg);
      if (L_LOGGING_NETPLAY) add_log_entry_position_text(11, 0, 76, 10, msg, "|", " ");
   }



   // Check for reply from server
   int tries = 1;        // number of times to try
   float try_delay = 1;  // delay between tries
   int got_reply = 0;

   while (!got_reply)
   {
      printf("ClientCheckResponse %d\n", tries);
      if (ClientCheckResponse())
      {
         got_reply = 1;
         sprintf(msg,"Got reply from server");
         printf("%s\n", msg);
         if (L_LOGGING_NETPLAY) add_log_entry_position_text(11, 0, 76, 10, msg, "|", " ");
      }
      else
      {
         al_rest(try_delay);
         if (++tries > 2)
         {
            sprintf(msg,"Did not get reply from server");
            m_err(msg);
            printf("%s\n", msg);
            if (L_LOGGING_NETPLAY)
            {
                add_log_entry_position_text(11, 0, 76, 10, msg, "|", " ");
                add_log_entry_centered_text(10, 0, 76, "", "+", "-");
            }
            return 0;
         }
      }
   }
   return 1;
}

int ClientCheckResponse(void) // check for a repsonse from the server
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
      printf("Sent initial packet 1234 to server\nWaiting for reply");

      int done = 10;
      while (done)
      {
         printf(".");
         packetsize = net_receive(ServerChannel, packetbuffer, 1024, address);
         if (packetsize)
         {
            set_packetpos(0);
            if (PacketRead("5678"))
            {
               printf("got response 5678: %s\n", address);
               net_assigntarget(ServerChannel, address);
               return 1;
            }
         }
         al_rest(.1);
         done--;
      }
      printf("no response\n");
   }
   return 0;
}


// Receive data from the server, and store in the provided array
// (must have room for 1024 bytes). Returns the size of the stored data.
int ClientReceive(void *data)
{
   int len;
   if (TCP) len = net_receive_rdm(ServerConn, data, 1024);
   else    	len = net_receive(ServerChannel, data, 1024, NULL);
   #ifdef NETPLAY_bandwidth_tracking
   players1[active_local_player].rx_current_bytes_for_this_frame+= len;
   players1[active_local_player].rx_current_packets_for_this_frame++;
   #endif
	return len;
}
void ClientSend(void *data, int len)
{
   if (TCP) net_send_rdm(ServerConn, data, len);
   else     net_send(ServerChannel, data, len);
   #ifdef NETPLAY_bandwidth_tracking
   players1[active_local_player].tx_current_bytes_for_this_frame+= len;
   players1[active_local_player].tx_current_packets_for_this_frame++;
   #endif
}

void client_flush(void)
{
   while ((packetsize = ClientReceive(packetbuffer)));
}

void ClientExitNetwork(void)
{
   sprintf(msg, "Shutting down the client network");
   printf("\n%s\n", msg);
   if (L_LOGGING_NETPLAY) add_log_entry_header(10, 0, msg, 1);
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

int client_init(void)
{
   if (L_LOGGING_NETPLAY) log_versions();

   sprintf(msg, "Client mode started on localhost:[%s]", local_hostname);
   printf("\n%s\n", msg);
   if (L_LOGGING_NETPLAY) add_log_entry_position_text(10, 0, 76, 10, msg, "|", " ");

    // initialize driver with server address
   if (!ClientInitNetwork(m_serveraddress)) return 0;

   ping_buffer_clear();

   printf("sending cjon\n");
   Packet("cjon");
   PacketPut1ByteInt(players[0].color); // requested color
   PacketAddString(local_hostname);
   ClientSend(packetbuffer, packetsize);

   if (L_LOGGING_NETPLAY_JOIN)
   {
      sprintf(msg,"Client sent join request to server with player color:[%2d]", players[0].color);
      add_log_entry_centered_text(11, 0, 76, "", "+", "-");
      add_log_entry_position_text(11, 0, 76, 10, msg, "|", " ");
      add_log_entry_centered_text(11, 0, 76, "", "+", "-");
   }
   return 1;
}

void client_process_sjon_packet(void)
{
   printf("Client received sjon\n");

   int pl = PacketGet2ByteInt();   // play level
   int server_sjon_frame_num  =  PacketGet4ByteInt();
   int server_game_move_entry_pos = PacketGet4ByteInt();
   int z = PacketGet1ByteInt();      // frame speed
   int p = PacketGet1ByteInt();      // client player number
   int color = PacketGet1ByteInt();  // client player color
   int dmp = PacketGet1ByteInt();    // deathmatch_pbullets
   int dmd = PacketGet2ByteInt();    // deathmatch_pbullets_damage
   int spb = PacketGet1ByteInt();    // suicide_pbullets

   if (p == 99) // server full, join denied
   {
      sprintf(msg,"Server replied with 'SERVER FULL'\n");
      printf("%s", msg);
      if (L_LOGGING_NETPLAY_JOIN) add_log_entry2(11, active_local_player, msg);
      new_program_state = 1;
   }
   else // join allowed
   {
      active_local_player = p;
      players[p].control_method = 4;
      players[p].color = color;
      strncpy(players1[0].hostname, m_serveraddress, 16);
      strncpy(players1[p].hostname, local_hostname, 16);
      ima_client = 1;

      play_level = pl;
      frame_speed = z;
      al_set_timer_speed(fps_timer, 1/(float)frame_speed);

      deathmatch_pbullets = dmp;
      deathmatch_pbullets_damage = dmd-1000;
      suicide_pbullets = spb;

      if (L_LOGGING_NETPLAY_JOIN)
      {
         sprintf(msg,"Client received join invitation from server");
         add_log_entry_position_text(11, 0, 76, 10, msg, "|", " ");
         sprintf(msg,"Level:[%d]", play_level);
         add_log_entry_position_text(11, 0, 76, 10, msg, "|", " ");
         sprintf(msg,"Frame Rate:[%d]", z);
         add_log_entry_position_text(11, 0, 76, 10, msg, "|", " ");
         sprintf(msg,"Player Number:[%d]", p);
         add_log_entry_position_text(11, 0, 76, 10, msg, "|", " ");
         sprintf(msg,"Player Color:[%d]", color);
         add_log_entry_position_text(11, 0, 76, 10, msg, "|", " ");
         sprintf(msg,"Server Frame Num:[%d]", server_sjon_frame_num);
         add_log_entry_position_text(11, 0, 76, 10, msg, "|", " ");
         sprintf(msg,"Server Game Moves:[%d]", server_game_move_entry_pos);
         add_log_entry_position_text(11, 0, 76, 10, msg, "|", " ");
         sprintf(msg,"Deathmatch player bullets:[%d]", deathmatch_pbullets);
         add_log_entry_position_text(11, 0, 76, 10, msg, "|", " ");
         sprintf(msg,"Deathmatch player bullet damage:[%d]", deathmatch_pbullets_damage);
         add_log_entry_position_text(11, 0, 76, 10, msg, "|", " ");
         sprintf(msg,"Suicide player bullets:[%d]", suicide_pbullets);
         add_log_entry_position_text(11, 0, 76, 10, msg, "|", " ");
         add_log_entry_centered_text(11, 0, 76, "", "+", "-");
      }
      new_program_state = 22;
   }
}

void client_exit(void)
{
   ClientExitNetwork();
   ima_client = 0;
   // reset player data
   for (int p=0; p<NUM_PLAYERS; p++) init_player(p, 1);
   players[0].active = 1; // local_control
   active_local_player = 0;
   load_config(); // to get color back
}

void client_send_stak(void)
{
   int p = active_local_player;
   Packet("stak");
   PacketPut1ByteInt(p);
   PacketPut4ByteInt(client_state_base_frame_num);
   PacketPut4ByteInt(frame_num);
   PacketPut4ByteInt(players1[p].frames_skipped);
   PacketPutDouble(players1[p].client_chase_fps);
   PacketPutDouble(players1[p].dsync);
   ClientSend(packetbuffer, packetsize);
}

void client_apply_diff(void)
{
   int p = active_local_player;

   if ((client_state_dif_dst > client_state_base_frame_num) || (frame_num == 0)) // stored dif with a new dest, or initial state (frame 0)
   {
      if (client_state_dif_src == 0) // if server has sent dif from src == 0, reset client base to 0
      {
         memset(client_state_base, 0, STATE_SIZE);
         client_state_base_frame_num = 0;
         if (L_LOGGING_NETPLAY_stdf) add_log_entry2(27, p, "Resetting client base state to zero\n");
      }

      if (client_state_base_frame_num != client_state_dif_src) // stored base state does NOT match dif source
      {
         sprintf(msg, "dif cannot be applied (wrong client base) %d %d\n", client_state_base_frame_num, client_state_dif_src);
         if (L_LOGGING_NETPLAY_stdf) add_log_entry2(29, p, msg);

         client_send_stak(); // resend ack to server with correct acknowledged base state
      }
      else // we have a matching base to apply dif
      {
         char tmsg[64];
         int ff = 0;
         if (frame_num == 0) sprintf(tmsg, "initial state\n");
         else if (client_state_dif_dst < frame_num)
         {
            ff = frame_num - client_state_dif_dst;
            sprintf(tmsg, "rewind [%d] frames\n", ff);
         }
         if (client_state_dif_dst == frame_num) sprintf(tmsg, "exact frame match [%d]\n", frame_num);

         // apply dif to base state
         apply_state_dif(client_state_base, client_state_dif, STATE_SIZE);

         // make a copy of the l[][]
         int old_l[100][100];
         memcpy(old_l, l, sizeof(l));

         // copy modified base state to game_vars
         state_to_game_vars(client_state_base);

         // compare old_l to l and redraw changed tiles
         al_set_target_bitmap(level_background);
         for (int x=0; x<100; x++)
            for (int y=0; y<100; y++)
               if (l[x][y] != old_l[x][y])
               {
                  // printf("dif at x:%d y:%d\n", x, y);
                  al_draw_filled_rectangle(x*20, y*20, x*20+20, y*20+20, palette_color[0]);
                  al_draw_bitmap(btile[l[x][y] & 1023], x*20, y*20, 0);
               }

         // fix control methods
         players[0].control_method = 2; // on client, server is mode 2
         if (players[p].control_method == 2) players[p].control_method = 4;
         if (players[p].control_method == 8) new_program_state = 1; // server quit

         // update frame_num and client base frame_num
         frame_num = client_state_base_frame_num = client_state_dif_dst;

         // for initial state only
         if (frame_num == 0) set_frame_nums(client_state_dif_dst);

         players1[p].client_last_dif_applied = frame_num;

         if (ff) loop_frame(ff); // if we rewound time, play it back

         client_send_stak();

         sprintf(msg, "dif [%d to %d] applied - %s", client_state_dif_src, client_state_dif_dst, tmsg);
         if (L_LOGGING_NETPLAY_stdf) add_log_entry2(29, p, msg);
      }
   }
   else
   {
      sprintf(msg, "dif [%d to %d] not applied - not newer than current [%d]\n", client_state_dif_src, client_state_dif_dst, client_state_base_frame_num);
      if (L_LOGGING_NETPLAY_stdf_when_to_apply) add_log_entry2(29, p, msg);
   }
}

void client_timer_adjust(void)
{
   int p = active_local_player;
   double chase_dsync = players1[p].dsync - players1[p].client_chase_offset; // adjust for target offset
   double fps_adj = chase_dsync * 100; // make the change bigger

   float fps_chase = frame_speed + fps_adj;
   if (fps_chase < 10) fps_chase = 10; // never let this go negative
   if (fps_chase > 70) fps_chase = 70;
   al_set_timer_speed(fps_timer, ( 1 / fps_chase));
   players1[p].client_chase_fps = fps_chase;

   sprintf(msg, "timer adjust dsync[%3.2f] offset[%3.2f] fps_chase[%3.2f]\n", players1[p].dsync*1000, players1[p].client_chase_offset*1000, fps_chase);
   if (L_LOGGING_NETPLAY_stdf_all_packets) add_log_entry2(36, p, msg);

}


void client_process_stdf_packet(double timestamp)
{
   int p = active_local_player;

   int src = PacketGet4ByteInt();
   int dst = PacketGet4ByteInt();
   int seq = PacketGet1ByteInt();
   int max_seq = PacketGet1ByteInt();
   int sb = PacketGet4ByteInt();
   int sz = PacketGet4ByteInt();


   sprintf(msg, "rx stdf piece [%d of %d] [%d to %d] st:%4d sz:%4d \n", seq+1, max_seq, src, dst, sb, sz);
   if (L_LOGGING_NETPLAY_stdf_all_packets) add_log_entry2(28, p, msg);

   players1[p].client_sync = dst - frame_num;               // crude integer sync based on frame numbers
   players1[p].client_last_stdf_rx_frame_num = frame_num;   // client keeps track of last stdf rx'd and quits if too long
   double dsync = al_get_time() - timestamp;                // time between when the packet was received into the packet buffer and now
   dsync += (double) players1[p].client_sync * 0.025;       // add to integer sync in case we are out by a lot
   players1[p].dsync = dsync;                               // save in local player array

   client_timer_adjust();

   memcpy(client_state_buffer + sb, packetbuffer+22, sz);  // put the piece of data in the buffer
   client_state_buffer_pieces[seq] = dst;                  // mark it with destination frame_num

   int complete = 1;                                          // did we just get the last packet? (yes by default)
   for (int i=0; i< max_seq; i++)
      if (client_state_buffer_pieces[i] != dst) complete = 0; // no, if any piece not at latest frame_num

   if (complete)
   {
      // decompress client_state_buffer to dif
      uLongf destLen = sizeof(client_state_dif);
      uncompress((Bytef*)client_state_dif, (uLongf*)&destLen, (Bytef*)client_state_buffer, sizeof(client_state_buffer));

      if (destLen == STATE_SIZE)
      {
         sprintf(msg, "rx dif complete [%d to %d] sync[%d] dsync[%3.1fms] - decompressed\n", src, dst, players1[p].client_sync, players1[p].dsync*1000);
         client_state_dif_src = src; // mark dif data with new src and dst
         client_state_dif_dst = dst;
      }
      else
      {
         sprintf(msg, "rx dif complete [%d to %d] sync[%d] - bad decompress\n", src, dst, players1[p].client_sync);
         client_state_dif_src = -1; // mark dif data as bad
         client_state_dif_dst = -1;
      }
      if (L_LOGGING_NETPLAY_stdf) add_log_entry2(27, p, msg);
   }
}

void process_bandwidth_counters(int p)
{
   #ifdef NETPLAY_bandwidth_tracking

   // get maximums per frame
   if (players1[p].tx_current_packets_for_this_frame > players1[p].tx_max_packets_per_frame)
      players1[p].tx_max_packets_per_frame = players1[p].tx_current_packets_for_this_frame;
   if (players1[p].tx_current_bytes_for_this_frame > players1[p].tx_max_bytes_per_frame)
      players1[p].tx_max_bytes_per_frame = players1[p].tx_current_bytes_for_this_frame;
   if (players1[p].rx_current_packets_for_this_frame > players1[p].rx_max_packets_per_frame)
      players1[p].rx_max_packets_per_frame = players1[p].rx_current_packets_for_this_frame;
   if (players1[p].rx_current_bytes_for_this_frame > players1[p].rx_max_bytes_per_frame)
      players1[p].rx_max_bytes_per_frame = players1[p].rx_current_bytes_for_this_frame;

   // get totals
   players1[p].tx_total_bytes += players1[p].tx_current_bytes_for_this_frame;
   players1[p].tx_total_packets += players1[p].tx_current_packets_for_this_frame;
   players1[p].rx_total_bytes += players1[p].rx_current_bytes_for_this_frame;
   players1[p].rx_total_packets += players1[p].rx_current_packets_for_this_frame;

   // add to tallies
   players1[p].tx_bytes_tally += players1[p].tx_current_bytes_for_this_frame;
   players1[p].tx_packets_tally += players1[p].tx_current_packets_for_this_frame;
   players1[p].rx_bytes_tally += players1[p].rx_current_bytes_for_this_frame;
   players1[p].rx_packets_tally += players1[p].rx_current_packets_for_this_frame;

   // reset counts for this frame
   players1[p].tx_current_bytes_for_this_frame = 0;
   players1[p].tx_current_packets_for_this_frame = 0;
   players1[p].rx_current_bytes_for_this_frame = 0;
   players1[p].rx_current_packets_for_this_frame = 0;

   if (frame_num % 40 == 0) // tally freq = 40 frames = 1s
   {
      // get maximums per tally
      if (players1[p].tx_bytes_per_tally > players1[p].tx_max_bytes_per_tally)
         players1[p].tx_max_bytes_per_tally = players1[p].tx_bytes_per_tally;
      if (players1[p].rx_bytes_per_tally > players1[p].rx_max_bytes_per_tally)
         players1[p].rx_max_bytes_per_tally = players1[p].rx_bytes_per_tally;
      if (players1[p].tx_packets_per_tally > players1[p].tx_max_packets_per_tally)
         players1[p].tx_max_packets_per_tally = players1[p].tx_packets_per_tally;
      if (players1[p].rx_packets_per_tally > players1[p].rx_max_packets_per_tally)
         players1[p].rx_max_packets_per_tally = players1[p].rx_packets_per_tally;

      // stick in variables for display
      players1[p].tx_bytes_per_tally = players1[p].tx_bytes_tally;
      players1[p].tx_packets_per_tally = players1[p].tx_packets_tally;
      players1[p].rx_bytes_per_tally = players1[p].rx_bytes_tally;
      players1[p].rx_packets_per_tally = players1[p].rx_packets_tally;

      if (L_LOGGING_NETPLAY_bandwidth)
      {
         // log tallies
         sprintf(msg, "bandwidth (B/s) TX cur:[%5d] max:[%5d] RX cur:[%5d] max:[%5d]\n", players1[p].tx_bytes_per_tally, players1[p].tx_max_bytes_per_tally, players1[p].rx_bytes_per_tally, players1[p].rx_max_bytes_per_tally);
         add_log_entry2(23, p, msg);
         sprintf(msg, "packets per s TX cur:[%3d] max:[%3d] RX cur:[%3d] max:[%3d]\n", players1[p].tx_packets_per_tally, players1[p].tx_max_packets_per_tally, players1[p].rx_packets_per_tally, players1[p].rx_max_packets_per_tally);
         add_log_entry2(24, p, msg);
      }

      // reset tallies
      players1[p].tx_bytes_tally = 0;
      players1[p].tx_packets_tally = 0;
      players1[p].rx_bytes_tally = 0;
      players1[p].rx_packets_tally = 0;

//      players1[p].moves_skipped_last_tally = players1[p].moves_skipped - players1[p].moves_skipped_tally;
//      players1[p].moves_skipped_tally = players1[p].moves_skipped;
   }
   #endif
}

void client_proc_player_drop(void)
{
   int p = active_local_player;

   if (players[p].control_method == 8)
   {
      sprintf(msg, "SERVER ENDED GAME!");
      float stretch = ( (float)SCREEN_W / (strlen(msg)*8)) - 1;
      rtextout_centre(NULL, msg, SCREEN_W/2, SCREEN_H/2, players[p].color, stretch, 0, 1);
      al_flip_display();
      al_rest(1);
      //tsw();

      players1[p].quit_reason = 92;
      log_ending_stats(p);
      new_program_state = 1;
   }

   int lsf = players1[p].client_last_stdf_rx_frame_num;
   if ((frame_num > 0) && (lsf > 0)) // check to see if server connection is lost
   {
      int ss = frame_num - lsf;
      if (ss > 120)
      {
         if (L_LOGGING_NETPLAY)
         {
            sprintf(msg, "Local Player Client %d Lost Server Connection!", p);
            add_log_entry_centered_text(10, p, 76, "", "+", "-");
            add_log_entry_position_text(10, p, 76, 10, msg, "|", " ");
            sprintf(msg, "frame_num:[%d] last_stdf_rx:[%d] dif:[%d]", frame_num, lsf, ss);
            add_log_entry_position_text(10, p, 76, 10, msg, "|", " ");
            add_log_entry_centered_text(10, p, 76, "", "+", "-");
         }

         sprintf(msg, "LOST SERVER CONNECTION!");
         float stretch = ( (float)SCREEN_W / (strlen(msg)*8)) - 1;
         rtextout_centre(NULL, msg, SCREEN_W/2, SCREEN_H/2, players[p].color, stretch, 0, 1);
         al_flip_display();
         al_rest(1);
         tsw();

         players1[p].quit_reason = 75;
         log_ending_stats(p);
         new_program_state = 25;
      }
   }
}




void client_fast_packet_loop(void)
{
   while ((packetsize = ClientReceive(packetbuffer)))
   {
      double timestamp = al_get_time();
      //printf("got packet size:%d\n", packetsize);
      int type = 0;
      if (PacketRead("stdf")) type = 1;
      if (PacketRead("sjon")) type = 2;

      if (PacketRead("pong"))
      {
         double t0 = PacketGetDouble();
         double t1 = PacketGetDouble();
         double t2 = al_get_time();
         players1[active_local_player].ping = t2 - t0;
         // printf("rx pong [%3.1f ms] - send pang\n", players1[active_local_player].ping * 1000);

         ping_array_add(players1[active_local_player].ping);

         Packet("pang");
         PacketPutDouble(t1);
         ClientSend(packetbuffer, packetsize);
      }

      // printf("type:%d\n", type);
      if (type)
      {
         for (int i=0; i<200; i++)
            if (!packet_buffers[i].active) // find empty
            {
               //printf("%d stored packet:%d size:%d type:%d\n", frame_num, i, packetsize, type);

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
void client_read_packet_buffer(void)
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

void client_control(void)
{
   client_read_packet_buffer();
   client_apply_diff();
   client_proc_player_drop();
   process_bandwidth_counters(active_local_player);
}
