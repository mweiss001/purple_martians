// n_client.cpp

#include "pm.h"

#ifdef NETPLAY

//-----------------------------------------------------------------------------------------------------
//*****************************************************************************************************
//-----------------------------------------------------------------------------------------------------
NET_CONN *ServerConn = NULL;
NET_CHANNEL *ServerChannel;

int ClientInitNetwork(const char *serveraddress)
{
	if(NetworkInit())
   {
      sprintf(msg, "Error: failed to initialize network");
      m_err(msg);
      if (L_LOGGING_NETPLAY)
      {
         #ifdef LOGGING_NETPLAY
         add_log_entry2(10, 0, msg);
         #endif
      }
      return 0;
   }
   if (TCP)
   {
      ServerConn = net_openconn(NetworkDriver, NULL);
      if(!ServerConn)
      {
         sprintf(msg, "Error: Client failed to create netconnection (TCP)");
         m_err(msg);
         if (L_LOGGING_NETPLAY)
         {
            #ifdef LOGGING_NETPLAY
            add_log_entry2(10, 0, msg);
            #endif
         }
         return 0;
      }
      if(net_connect(ServerConn, serveraddress))
      {
         sprintf(msg, "Error: Client failed to set netconnection target: server[%s] (TCP)", serveraddress);
         m_err(msg);
         if (L_LOGGING_NETPLAY)
         {
            #ifdef LOGGING_NETPLAY
            add_log_entry2(10, 0, msg);
            #endif
         }
         net_closeconn(ServerConn);
   		return 0;
   	}
      sprintf(msg, "Client network initialized: server[%s] (TCP)", serveraddress);
   } // end of if TCP
   else // UDP
   {
      ServerChannel = net_openchannel(NetworkDriver, "");
      if (ServerChannel == NULL)
      {
         sprintf(msg, "Error: Client failed to create netchannel (UDP)");
         m_err(msg);
         if (L_LOGGING_NETPLAY)
         {
            #ifdef LOGGING_NETPLAY
            add_log_entry2(10, 0, msg);
            #endif
         }
         return 0;
      }

      if (net_assigntarget(ServerChannel, serveraddress))
      {
         sprintf(msg, "Error: Client failed to set netchannel target: server[%s] (UDP)", serveraddress);
         m_err(msg);
         if (L_LOGGING_NETPLAY)
         {
            #ifdef LOGGING_NETPLAY
            add_log_entry2(10, 0, msg);
            #endif
         }
         return 0;
      }
      sprintf(msg, "Client network initialized: server[%s] (UDP)", serveraddress);
   } // end of UDP

   printf("%s\n", msg);
   if (L_LOGGING_NETPLAY)
   {
      #ifdef LOGGING_NETPLAY
      add_log_entry2(10, 0, msg);
      #endif
   }


   // Check for reply from server x times with rest each time
   // if delay is too short you will miss response
   int tries = 4;
   float try_delay = 0.02;
   int got_reply = 0;

   while (!got_reply)
   {
      if (key[ALLEGRO_KEY_ESCAPE]) return 0;
      if (ClientCheckResponse())
      {
         got_reply = 1;
         sprintf(msg,"Got reply from server");
         printf("%s\n", msg);
         if (L_LOGGING_NETPLAY)
         {
            #ifdef LOGGING_NETPLAY
            add_log_entry2(10, active_local_player, msg);
            #endif
         }
      }
      else
      {
         al_rest(try_delay);
         if (--tries < 0)
         {
            sprintf(msg,"Did not get reply from server");
            printf("%s\n", msg);
            if (L_LOGGING_NETPLAY)
            {
               #ifdef LOGGING_NETPLAY
               add_log_entry2(10, active_local_player, msg);
               #endif
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
      // send data to server
      Packet("1234");
      ClientSend(packetbuffer, packetsize);
      al_rest(0.1);
      // check for a response
      packetsize = net_receive(ServerChannel, packetbuffer, 1024, address);
      if (packetsize && (strcmp(packetbuffer, "5678") == 0)) // got response
      {
          net_assigntarget(ServerChannel, address);
          return 1;
      }
      else return 0; // no response yet
   }
   return 0;
}

// Receive data from the server, and store them in the provided array
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
}


// ---------------------------------------------------------------------------------------------------------------
// ***************************************************************************************************************
//----------------------------------------------------------------------------------------------------------------

int client_init(void)
{
   sprintf(msg, "Client mode started on host:[%s]",local_hostname);
   printf("%s\n", msg);
   if (L_LOGGING_NETPLAY)
   {
      #ifdef LOGGING_NETPLAY
      add_log_entry_centered_text(10, 0, 76, "", "+", "-");
      add_log_entry_position_text(10, 0, 76, 10, msg, "|", " ");
      #endif
   }

   // date and time stamp
   char tmsg[80];
   struct tm *timenow;
   time_t now = time(NULL);
   timenow = localtime(&now);
   strftime(tmsg, sizeof(tmsg), "%Y-%m-%d  %H:%M:%S", timenow);
   sprintf(msg, "Date and time:[%s]",tmsg);
   printf("%s\n", msg);

   if (L_LOGGING_NETPLAY)
   {
      #ifdef LOGGING_NETPLAY
      add_log_entry_position_text(10, 0, 76, 10, msg, "|", " ");
      add_log_entry_centered_text(10, 0, 76, "", "+", "-");
      #endif
   }

   // initialize driver with server address
   if (!ClientInitNetwork(m_serveraddress)) return 0;

   // try to join netgame
   return client_init_join();
}



int client_init_join(void)
{
   Packet("CJON");
   PacketPut1ByteInt(players[0].color); // request the color of current player 0
   PacketAddString(local_hostname);
   ClientSend(packetbuffer, packetsize);

   if (L_LOGGING_NETPLAY_JOIN)
   {
      #ifdef LOGGING_NETPLAY_JOIN
      sprintf(msg,"Client sent join request to server with player color:[%2d]", players[0].color);
      add_log_entry_centered_text(11, 0, 76, "", "+", "-");
      add_log_entry_position_text(11, 0, 76, 10, msg, "|", " ");
      add_log_entry_centered_text(11, 0, 76, "", "+", "-");
      #endif
   }

   int SJON = 0;
   while (!SJON) // run loop until SJON processed
   {
      al_rest(0.02);
      if (key[ALLEGRO_KEY_ESCAPE]) SJON = 98; // emergency exit
      if ((packetsize = ClientReceive(packetbuffer)) && (PacketRead("SJON")))
      {
         SJON = 1;
         int pl = PacketGet2ByteInt();   // play level
         int server_SJON_frame_num      =  PacketGet4ByteInt();
         int server_game_move_entry_pos =  PacketGet4ByteInt();
         int z = PacketGet1ByteInt();      // frame rate
         int cp = PacketGet1ByteInt();     // client player number
         int color = PacketGet1ByteInt();  // client player color
         int dmp = PacketGet1ByteInt();
         int dmd = PacketGet1ByteInt();
         int spb = PacketGet1ByteInt();

         if (cp == 99) SJON = 99; // server full, join denied
         else // join allowed
         {
            // initialize all players
            for (int p=0; p<NUM_PLAYERS; p++) init_player(p, 1);

            active_local_player = cp;
            players[cp].control_method = 4;
            players[cp].color = color;
            players[cp].bitmap_index = color -1;
            players1[cp].game_move_entry_pos = server_game_move_entry_pos;

            ima_client = 1;

            frame_num = 0;   // just in case its not
            play_level = pl;
            frame_speed = z;
            al_set_timer_speed(fps_timer, 1/(float)frame_speed);

            deathmatch_pbullets = dmp;
            deathmatch_pbullets_damage = dmd;
            suicide_pbullets = spb;

            if (L_LOGGING_NETPLAY_JOIN)
            {
               #ifdef LOGGING_NETPLAY_JOIN
               sprintf(msg,"Client received join invitation from server");
               add_log_entry_position_text(11, 0, 76, 10, msg, "|", " ");
               sprintf(msg,"Level:[%d]", play_level);
               add_log_entry_position_text(11, 0, 76, 10, msg, "|", " ");
               sprintf(msg,"Frame Rate:[%d]", z);
               add_log_entry_position_text(11, 0, 76, 10, msg, "|", " ");
               sprintf(msg,"Player Number:[%d]", cp);
               add_log_entry_position_text(11, 0, 76, 10, msg, "|", " ");
               sprintf(msg,"Player Color:[%d]", color);
               add_log_entry_position_text(11, 0, 76, 10, msg, "|", " ");
               sprintf(msg,"Server Frame Num:[%d]", server_SJON_frame_num);
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
               #endif
            }
            return 1;
         } // end of join allowed
      } // end of packet read
   } // end of while (!SJON)

   if (SJON == 98) // cancelled by player with ESC
   {
      sprintf(msg,"Client waiting for join cancelled by client");
      printf("%s\n", msg);
      if (L_LOGGING_NETPLAY_JOIN)
      {
         #ifdef LOGGING_NETPLAY_JOIN
         add_log_entry2(11, active_local_player, msg);
         #endif
      }
      return 0;
   }
   if (SJON == 99) // denied by server full
   {
      sprintf(msg,"Server replied with 'SERVER FULL'");
      printf("%s\n", msg);
      if (L_LOGGING_NETPLAY_JOIN)
      {
         #ifdef LOGGING_NETPLAY_JOIN
         add_log_entry2(11, active_local_player, msg);
         #endif
      }
      return 0;
   }
   return 1;
}

void client_exit(void)
{
   ClientExitNetwork();
   ima_client = 0;
   players[0].active = 1; // local_control
   active_local_player = 0;
}

void read_game_step_from_packet(int x, int clf_check)
{
   // read from the packet
   int g0 = PacketGet4ByteInt();
   int g1 = PacketGet1ByteInt();
   int g2 = PacketGet1ByteInt();
   int g3 = PacketGet1ByteInt();

   char tmsg1[80], tmsg2[80], tmsg3[80], tmsg4[80];
   sprintf(tmsg1,"rx move:%d [%d][%d][%d][%2d]", x, g0, g1, g2, g3);
   sprintf(tmsg3," ");
   sprintf(tmsg4," ");


   // check to see if already in game move array (duplicate)
   if ((game_moves[x][0] == g0) && (game_moves[x][1] == g1) && (game_moves[x][2] == g2) && (game_moves[x][3] == g3))
   {
      players1[active_local_player].moves_skipped++;  // moves skipped
      sprintf(tmsg2,"- already exists");
   }
   else // not duplicate
   {
      players1[active_local_player].moves_entered++; // moves entered
      sprintf(tmsg2,"- entering move");

      // put in the game array
      game_moves[x][0] = g0;
      game_moves[x][1] = g1;
      game_moves[x][2] = g2;
      game_moves[x][3] = g3;

      if (clf_check)
      {
         int p = active_local_player;
         int c_sync = g0 - frame_num;                                // calculate c_csync
         players1[p].client_game_move_sync = c_sync;                 // set in player struct
         if (c_sync < 0 )                                            // check for error
         {
            players1[p].client_game_move_sync_err++;                 // inc error count
            sprintf(tmsg4,"<---[ERROR!]");
         }
         if (c_sync == 0) sprintf(tmsg4,"<---[WARNING!]");
         if (c_sync < players1[p].client_game_move_sync_min)         // set c_cync min
            players1[p].client_game_move_sync_min = c_sync;

         sprintf(tmsg3,"- frame lead:[%d]%s", c_sync, tmsg4);
      }
   }  // end of if not duplicate

   if (L_LOGGING_NETPLAY_game_move)
   {
      #ifdef LOGGING_NETPLAY_game_move
      sprintf(msg,"%s %s %s\n", tmsg1, tmsg2, tmsg3);
      add_log_entry2(38, g2, msg);
      #endif
   }
}


int process_stdf_packet(void)
{
   int retval = 0;
   int p = active_local_player;

   int src = PacketGet4ByteInt();
   int dst = PacketGet4ByteInt();
   int seq = PacketGet1ByteInt();
   int max_seq = PacketGet1ByteInt();
   int sb = PacketGet4ByteInt();
   int sz = PacketGet4ByteInt();

   if (L_LOGGING_NETPLAY_stdf_all_packets)
   {
      #ifdef LOGGING_NETPLAY_stdf_all_packets
      sprintf(msg, "rx stdf piece [%d of %d] [%d to %d] st:%4d sz:%4d \n", seq+1, max_seq, src, dst, sb, sz);
      add_log_entry2(28, p, msg);
      #endif
   }

   memcpy(client_state_buffer + sb, packetbuffer+22, sz);  // put the piece of data in the buffer
   client_state_buffer_pieces[seq] = dst; // mark it with destination frame_num


   int complete = 1; // did we just get the last packet? (yes by default)
   for (int i=0; i< max_seq; i++)
      if (client_state_buffer_pieces[i] != dst) complete = 0; // no, if any piece not at latest frame_num

   if (complete)
   {
      char tmsg1[80], tmsg2[80], tmsg3[80];
      sprintf(tmsg1, "rx stdf complete [%d to %d]", src, dst);

      players1[p].stdf_rx++;
      if (dst < frame_num)
      {
         players1[p].stdf_late++;
         sprintf(tmsg2, "LATE!");
      }
      else
      {
         players1[p].stdf_on_time++;
         sprintf(tmsg2, "on time");
      }

      // decompress client_state_buffer to dif
      uLongf destLen = sizeof(client_state_dif);
      uncompress((Bytef*)client_state_dif, (uLongf*)&destLen, (Bytef*)client_state_buffer, sizeof(client_state_buffer));

      if (destLen == STATE_SIZE)
      {
         sprintf(tmsg3, "good decompress");
         client_state_dif_src = src; // mark dif data with new src and dst
         client_state_dif_dst = dst;
         retval = 1;
      }
      else
      {
         sprintf(tmsg3, "bad decompress");
         client_state_dif_src = -1; // mark dif data as bad
         client_state_dif_dst = -1;
      }
      if (L_LOGGING_NETPLAY_stdf)
      {
         #ifdef LOGGING_NETPLAY_stdf
         sprintf(msg, "%s %s %s\n", tmsg1, tmsg2, tmsg3);
         add_log_entry2(27, p, msg);
         #endif
      }
   }
   return retval;
}


void client_apply_diff()
{
   int p = active_local_player;
   if (L_LOGGING_NETPLAY_stdf_when_to_apply)
   {
      #ifdef LOGGING_NETPLAY_stdf_when_to_apply
      sprintf(msg, "base:[%d] - last dif:[%d to %d]\n", client_state_base_frame_num,  client_state_dif_src, client_state_dif_dst );
      add_log_entry2(29, p, msg);
      #endif
   }
   // check to see if frame_nums match and its time to apply dif
   if (frame_num == client_state_dif_dst) // current frame_num is dif destination
   {
      if (client_state_base_frame_num != client_state_dif_src)  // stored base state does NOT match dif source
      {
         // if server has sent dif from src == 0, reset our base to 0
         if (client_state_dif_src == 0)
         {
            memset(client_state_base, 0, STATE_SIZE);
            client_state_base_frame_num = 0;
            if (L_LOGGING_NETPLAY_stdf)
            {
               #ifdef LOGGING_NETPLAY_stdf
               sprintf(msg, "Resetting client base state to zero\n");
               add_log_entry2(27, p, msg);
               #endif
            }
         }
         else
         {
            if (L_LOGGING_NETPLAY_stdf)
            {
               #ifdef LOGGING_NETPLAY_stdf
               sprintf(msg, "!! stdf cannot be applied (wrong client base) %d %d\n", client_state_base_frame_num, client_state_dif_src);
               add_log_entry2(27, p, msg);
               #endif
            }
         }
      }

      if (client_state_base_frame_num == client_state_dif_src)  // stored base state matches dif source
      {
         char tmsg[80];
         int dif_corr = 0;

         // apply dif to base state
         apply_state_dif(client_state_base, client_state_dif, STATE_SIZE);

         if (client_state_dif_src != 0) // check dif, unless src == 0
         {
            // fix control methods to match server so dif check won't fail
            players[0].control_method = 3;
            players[p].control_method = 2;

            // make copy of current state
            char tmp[STATE_SIZE];
            game_vars_to_state(tmp);

            // compare copy to modified base state
            if (memcmp(tmp, client_state_base, STATE_SIZE))
            {
               dif_corr = 1;
               players1[p].dif_corr++;
               sprintf(tmsg, "corrections applied - total corrections:%d", players1[p].dif_corr);
               show_state_dif(tmp, client_state_base); // show dif
            }
            else
            {
              sprintf(tmsg, "no differences");
              dif_corr = 0;
            }
         }
         else sprintf(tmsg, "applied (no check)"); // no dif checking, just blindly apply (used for join)

         // copy modified base state to game_vars
         state_to_game_vars(client_state_base);

         // update client base frame_num
         client_state_base_frame_num = frame_num;

         // restore control methods
         players[0].control_method = 2;
         players[p].control_method = 4;


         if (L_LOGGING_NETPLAY_stdf)
         {
            #ifdef LOGGING_NETPLAY_stdf
            sprintf(msg, "dif [%d to %d] %s\n", client_state_dif_src, client_state_dif_dst, tmsg);
            add_log_entry2(27, p, msg);
            #endif
         }
         // send ack to server
         Packet("stak");
         PacketPut1ByteInt(p);
         PacketPut1ByteInt(dif_corr);
         PacketPut4ByteInt(frame_num);
         ClientSend(packetbuffer, packetsize);

      }
   }
}


void client_block_until_good_stdf_received(void)
{
   int p = active_local_player;

   sprintf(msg, "Waiting for game state from server");
   rtextout_centre(NULL, msg, SCREEN_W/2, SCREEN_H/2, 10, 2, 0, 1);
   if (L_LOGGING_NETPLAY_JOIN)
   {
      #ifdef LOGGING_NETPLAY_JOIN
      add_log_entry_header(11, p, msg, 1);
      #endif
   }

   reset_states();
   int done = 0;
   while (!done)
   {
      if ((packetsize = ClientReceive(packetbuffer)) && (PacketRead("stdf"))) done = process_stdf_packet();
      proc_controllers();
      if (key[ALLEGRO_KEY_F11]) fast_exit(65); // in case we get trapped here and need a way out
   }
   client_apply_diff();

   // set frame_num and timer frame_num
   frame_num = client_state_dif_dst;
   al_set_timer_count(fps_timer, frame_num);

   // set all active clients control methods to 2
   for (int pp=1; pp<NUM_PLAYERS; pp++)
      if (players[pp].active) players[pp].control_method = 2;

   // set local client control method to 4
   players[p].control_method = 4;

   // set holdoff 200 frames in future so client won't try to drop while syncing
   players1[p].client_last_sdat_rx_frame_num = frame_num + 200;

   // send ack up to this point so server won't try to send us previous game moves
   Packet("sdak");
   PacketPut1ByteInt(p);
   PacketPut4ByteInt(frame_num);
   PacketPut4ByteInt(players1[p].game_move_entry_pos); // new entry pos
   PacketPut4ByteInt(players1[p].stdf_late);
   PacketPut4ByteInt(players1[p].frames_skipped);
   ClientSend(packetbuffer, packetsize);

   log_timer = clock(); // start timing for chase and lock
   if (L_LOGGING_NETPLAY_JOIN)
   {
      #ifdef LOGGING_NETPLAY_JOIN
      sprintf(msg, "frame_nums updated - starting chase and lock");
      add_log_entry_header(11, p, msg, 1);
       //printf("%s", msg);
      #endif
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
         #ifdef LOGGING_NETPLAY_bandwidth
         // log tallies
         sprintf(msg, "bandwidth (B/s) TX cur:[%5d] max:[%5d] RX cur:[%5d] max:[%5d]\n", players1[p].tx_bytes_per_tally, players1[p].tx_max_bytes_per_tally, players1[p].rx_bytes_per_tally, players1[p].rx_max_bytes_per_tally);
         add_log_entry2(23, p, msg);
         sprintf(msg, "packets per s TX cur:[%3d] max:[%3d] RX cur:[%3d] max:[%3d]\n", players1[p].tx_packets_per_tally, players1[p].tx_max_packets_per_tally, players1[p].rx_packets_per_tally, players1[p].rx_max_packets_per_tally);
         add_log_entry2(24, p, msg);
         #endif
      }

      // reset tallies
      players1[p].tx_bytes_tally = 0;
      players1[p].tx_packets_tally = 0;
      players1[p].rx_bytes_tally = 0;
      players1[p].rx_packets_tally = 0;

      players1[p].moves_skipped_last_tally = players1[p].moves_skipped - players1[p].moves_skipped_tally;
      players1[p].moves_skipped_tally = players1[p].moves_skipped;
   }
   #endif
}




void client_control(void)
{
   int p = active_local_player;

   // check to see if game has gone bad and we need to quit
   if ((frame_num > 0) && (players1[p].client_last_sdat_rx_frame_num > 0))
   {
       int ss = frame_num - players1[p].client_last_sdat_rx_frame_num;
       // printf("pc:%d lspc:%d  ss:%d\n", frame_num, players1[p].client_last_sdat_rx_frame_num, ss);
       if (ss > 120)
       {
          sprintf(msg, "Player %d LOST SERVER CONNECTION", p);

          #ifdef LOGGING_NETPLAY
          add_log_entry_header(10, p, msg, 2);
          char tmsg[100];
          sprintf(tmsg, "frame_num:[%d] last_sdat_rx:[%d] dif:[%d]", frame_num, players1[p].client_last_sdat_rx_frame_num, ss);
          add_log_entry_header(10, p, tmsg, 1);
          #endif

          float stretch = ( (float)SCREEN_W / (strlen(msg)*8)) - 1; // (SCREEN_W / text length*8) -1
          int color = players[p].color;
          int y_pos = SCREEN_H/2;
          rtextout_centre(NULL, msg, SCREEN_W/2, y_pos, color, stretch, 0, 1);
          al_rest(1);
          tsw();
          fast_exit(75);
       }
   }


   if (frame_num == 0) client_block_until_good_stdf_received();

   process_bandwidth_counters(p);


   if (key[ALLEGRO_KEY_F11]) fast_exit(65); // in case we get trapped here and need a way out

   while ((packetsize = ClientReceive(packetbuffer))) // rx multiple per frame_num
   {
      if (key[ALLEGRO_KEY_F11]) fast_exit(65); // in case we get trapped here and need a way out

      if(PacketRead("stdf")) process_stdf_packet();

      if(PacketRead("serr"))
      {
         int serr_type = PacketGet1ByteInt();
         int serr_frame_num = PacketGet4ByteInt();
         int serr_c_sync = PacketGet4ByteInt();
         int serr_c_sync_err = PacketGet4ByteInt();

         if (serr_type == 1) // error type (1 = cdat late)

         players1[p].serr_c_sync_err = serr_c_sync_err;
         players1[p].serr_display_timer = 120;

         sprintf(msg,"Error: server dropped cdat s_pc:%d  s_csync:%d  tot_err:%d\n", serr_frame_num, serr_c_sync, serr_c_sync_err);
         if (L_LOGGING_NETPLAY_cdat)
         {
            #ifdef LOGGING_NETPLAY_cdat
            add_log_entry2(35, p, msg);
            #endif
         }
      }
      if(PacketRead("sdat"))
      {
         char tmsg[20];
         int sdat_frame_num = PacketGet4ByteInt();
         int start_entry = PacketGet4ByteInt();
         int num_entries = PacketGet1ByteInt();

         players1[p].client_sdat_packets_rx++; // total sdat packets rx'd

         // this used to be a function , client timer adjust
         players1[p].client_sync = sdat_frame_num - frame_num;
         int fps_chase = frame_speed + players1[p].client_sync - server_lead_frames;
         if (fps_chase < 4) fps_chase = 4; // never let this go negative
         al_set_timer_speed(fps_timer, ( 1 / (float) fps_chase));
         if (L_LOGGING_NETPLAY_client_timer_adjust)
         {
            #ifdef LOGGING_NETPLAY_client_timer_adjust
            sprintf(msg,"[timer adjust] client_sync:[%d] fps:[%d]\n", players1[p].client_sync, fps_chase);
            add_log_entry2(25, p, msg);
            #endif
         }

         if (num_entries == 0) sprintf(tmsg,"sync only");

         int nep = start_entry + num_entries; // new entry position
         if (nep > game_move_entry_pos) strcpy(tmsg,"new moves");
         else if (num_entries)
         {
            sprintf(tmsg,"no new moves");
            players1[p].client_sdat_packets_skipped++ ; // total sdat packets skipped
            players1[p].moves_skipped += num_entries;  // moves skipped

         }
         if (L_LOGGING_NETPLAY_sdat)
         {
            #ifdef LOGGING_NETPLAY_sdat
            sprintf(msg,"rx sdat [server_frame:%d] - strt:%d num:%d - %s\n", sdat_frame_num, start_entry, num_entries, tmsg);
            add_log_entry2(37, p, msg);
            #endif
         }

         if (nep > game_move_entry_pos) // only enter if they are newer
         {
            for (int x=start_entry; x<nep; x++)
               read_game_step_from_packet(x, 1);
            game_move_entry_pos = nep;
         }

         Packet("sdak"); // send acknowledgement of game moves received
         PacketPut1ByteInt(p);
         PacketPut4ByteInt(frame_num);
         PacketPut4ByteInt(nep);
         PacketPut4ByteInt(players1[p].stdf_late);
         PacketPut4ByteInt(players1[p].frames_skipped);
         ClientSend(packetbuffer, packetsize);

         if (L_LOGGING_NETPLAY_sdak)
         {
            #ifdef LOGGING_NETPLAY_sdak
            sprintf(msg,"tx sdak nep:[%d] stdf_late:[%d] frames skipped:%d\n", nep, players1[p].stdf_late, players1[p].frames_skipped);
            add_log_entry2(39, p, msg);
            #endif
         }
      } // end of packet read sdat
   } // end of packet rx loop

   client_apply_diff();

}

void client_local_control(int p)
{
   if (players1[p].fake_keypress_mode)
   {
      players1[p].comp_move = rand() % 64;
      if (key[ALLEGRO_KEY_F11]) fast_exit(65); // in case we get trapped here and need a way out
   }
   else set_comp_move_from_player_key_check(p);                // but don't set controls !!!


   if (players1[p].old_comp_move != players1[p].comp_move)  // players controls have changed
   {
      players1[p].old_comp_move = players1[p].comp_move;
      Packet("cdat");
      PacketPut1ByteInt(p);
      PacketPut4ByteInt(frame_num + control_lead_frames); // add control_lead_frames to frame_num
      PacketPut1ByteInt(players1[p].comp_move);
      ClientSend(packetbuffer, packetsize);
      players1[p].client_cdat_packets_tx++;

//      while (key[ALLEGRO_KEY_ESCAPE]) proc_controllers(); // wait for release, but send cdat first

      if (L_LOGGING_NETPLAY_cdat)
      {
         #ifdef LOGGING_NETPLAY_cdat
         sprintf(msg,"tx cdat - move:%d\n", players1[p].comp_move);
         add_log_entry2(35, active_local_player, msg);
         #endif
      }
   }
}

#endif

