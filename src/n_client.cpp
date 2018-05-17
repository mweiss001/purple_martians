#include "pm.h"

#ifdef NETPLAY

//-----------------------------------------------------------------------------------------------------
//*****************************************************************************************************
//-----------------------------------------------------------------------------------------------------
NET_CONN *ServerConn = NULL;
NET_CHANNEL *ServerChannel;

int ClientInit(char *serveraddress)
{
	if(NetworkInit())
   {
      sprintf(msg, "Error: failed to initialize network\n");
      printf("%s", msg);
      m_err(msg);
      if (L_LOGGING_NETPLAY)
      {
         #ifdef LOGGING_NETPLAY
         add_log_entry2(10, 0, msg);
         #endif
      }
      return -1;
   }
   if (TCP)
   {
      #ifdef CONNECTION
      ServerConn = net_openconn(NetworkDriver, NULL);
      if(!ServerConn)
      {
         sprintf(msg, "Error: Unable to to open server connection\n");
         printf("%s", msg);
         m_err(msg);
         if (L_LOGGING_NETPLAY)
         {
            #ifdef LOGGING_NETPLAY
            add_log_entry2(10, 0, msg);
            #endif
         }
         return -1;
      }
      if(net_connect(ServerConn, serveraddress))
      {
         sprintf(msg, "Error: Can't connect to that address\n");
         printf("%s", msg);
         m_err(msg);
         if (L_LOGGING_NETPLAY)
         {
            #ifdef LOGGING_NETPLAY
            add_log_entry2(10, 0, msg);
            #endif
         }
         net_closeconn(ServerConn);
   		return -1;
   	}
      sprintf(msg, "Network initialized - connection mode (TCP)\n");
      #endif
   } // end of if TCP
   else // UDP
   {
      #ifdef CHANNEL
      ServerChannel = net_openchannel(NetworkDriver, "");
      if (ServerChannel == NULL)
      {
         sprintf(msg, "Error: failed to open netchannel\n");
         printf("%s", msg);
         m_err(msg);
         if (L_LOGGING_NETPLAY)
         {
            #ifdef LOGGING_NETPLAY
            add_log_entry2(10, 0, msg);
            #endif
         }
         return -1;
      }
      if (net_assigntarget(ServerChannel, serveraddress))
      {
         sprintf(msg, "Error: failed to set netchannel target\n");
         printf("%s", msg);
         m_err(msg);
         if (L_LOGGING_NETPLAY)
         {
            #ifdef LOGGING_NETPLAY
            add_log_entry2(10, 0, msg);
            #endif
         }
         return -1;
      }
      sprintf(msg, "Network initialized - channel mode (UDP)\n");
      #endif
   } // end of UDP

   if (L_LOGGING_NETPLAY)
   {
      #ifdef LOGGING_NETPLAY
      add_log_entry2(10, 0, msg);
      printf("%s", msg);
      #endif
   }
	return 0;
}

int ClientCheckResponse(void) // check for a repsonse from the server
{
   if (TCP)
   {
      #ifdef CONNECTION
   	int x = net_poll_connect(ServerConn);
   	if (x == 0) return 1;     // got response
   	if (x < 0)                // error
      {
   		net_closeconn(ServerConn);
   		return -1;
   	}
   	return 0;                 // no response yet
      #endif
   } // end of if TCP
   else // UDP
   {
      #ifdef CHANNEL
      char address[32];
      // send data to server
      Packet((char *)"1234");
      ClientSend(packetbuffer, packetsize);
      al_rest(0.1);
      // check for a response
      packetsize = net_receive(ServerChannel, packetbuffer, 1024, address);
      if (packetsize && (strcmp(packetbuffer, "5678") == 0)) // got response
      {
          net_assigntarget(ServerChannel, address);
          return 0;
      }
      else return 1; // no response yet
      #endif
   } // end of UDP
}


// Receive data from the server, and store them in the provided array
// (must have room for 1024 bytes). Returns the size of the stored data.
int ClientReceive(void *data)
{
   int len;
   if (TCP)
   {
      #ifdef CONNECTION
      len = net_receive_rdm(ServerConn, data, 1024);
      #endif
   }
   else // UDP
   {

      #ifdef CHANNEL
   	len = net_receive(ServerChannel, data, 1024, NULL);
      #endif
   }
   #ifdef NETPLAY_bandwidth_tracking
   players1[active_local_player].rx_current_bytes_for_this_frame+= len;
   players1[active_local_player].rx_current_packets_for_this_frame++;
   #endif
	return len;
}
void ClientSend(void *data, int len)
{
   if (TCP)
   {
      #ifdef CONNECTION
   	net_send_rdm(ServerConn, data, len);
      #endif
   }
   else // UDP
   {
      #ifdef CHANNEL
   	net_send(ServerChannel, data, len);
      #endif
   }
   #ifdef NETPLAY_bandwidth_tracking
   players1[active_local_player].tx_current_bytes_for_this_frame+= len;
   players1[active_local_player].tx_current_packets_for_this_frame++;
   #endif
}

void client_flush(void)
{
   while (ClientReceive(packetbuffer));
}

void ClientExit(void)
{
   if (TCP)
   {
      #ifdef CONNECTION
   	if(ServerConn) net_closeconn(ServerConn);
   	ServerConn = NULL;
      #endif
   } // end of if TCP
   else // UDP
   {
      #ifdef CHANNEL
   	if(ServerChannel) net_closechannel(ServerChannel);
   	ServerChannel = NULL;
      #endif
   } // end of UDP
}








// ---------------------------------------------------------------------------------------------------------------
// ***************************************************************************************************************
//----------------------------------------------------------------------------------------------------------------

void nc_client(void) // for specialized packet testing only
{
   if (ClientInit((char *)"192.168.1.170"))  printf("Client init failed\n");
//   if (ClientCheckResponse())        printf("No server response\n");
   while (ClientCheckResponse()); // wait until its zero        printf("No server response\n");
   int quit = 0;
   while (!quit)
   {
      if (key[ALLEGRO_KEY_ESCAPE]) quit = 1;
      packetsize = ClientReceive(packetbuffer);
      if (packetsize > 0)
      {
         if(PacketRead((char *)"peep"))
         {
            int pc = Packet2ByteRead();
//            printf("client rx peep:[%d]\n", pc);
            pc++;
            if (pc < 1000)
            {
//               printf("client tx poop:[%d]\n",pc);
               Packet((char *)"poop");
               PacketAdd2Bytes(pc);
               ClientSend(packetbuffer, packetsize);
            }
            else
            {
               int finish_time = clock(); // stop the timer
               int time = finish_time - log_timer;
               printf("time:[%d]ms\n", time);
            }
         }
      }
      if (key[ALLEGRO_KEY_1])
      {
         while (key[ALLEGRO_KEY_1]);
         log_timer = clock(); // start the timer...
         printf("Sending poop packet to server\n");
         Packet((char *)"poop");
         PacketAddByte(0);
         ClientSend(packetbuffer, packetsize);
      }
   }
}

// ---------------------------------------------------------------------------------------------------------------
// ***************************************************************************************************************
//----------------------------------------------------------------------------------------------------------------

extern char m_serveraddress[256];

int client_init_driver(void)
{
   //textout_centre_ex(screen, font, "Loading driver...",SCREEN_W/2, SCREEN_H/2, palette_color[13], 0);
   int init_good = 0;
   float try_delay = 0.02;
   int tries = 4; // first time 4 then 2 for next tries

   // read server address from config file, use default if not found
   //const char *server_IP = get_config_string("NETWORK", "server_IP", "192.168.1.2");
   //sprintf(m_serveraddress, "%s", server_IP);

   while (!init_good)
   {
      char tmsg[80];

      if (key[ALLEGRO_KEY_ESCAPE]) return 0;

      // initialize driver with server address
      if (ClientInit(m_serveraddress)) return 0;

      sprintf(msg, "Client initialized\n");
      sprintf(tmsg, "Connecting to server");
      printf("%s", msg);
      printf("%s", tmsg);
      if (L_LOGGING_NETPLAY)
      {
         #ifdef LOGGING_NETPLAY
         add_log_entry2(10, 0, msg);
         #endif
      }

      // Check for reply from server loop x times with rest each time
      // if delay is too short you will miss response
//      printf("looking for server:[%s]", m_serveraddress);
      while (tries-- > 0)
      {
         printf(".");
         if (key[ALLEGRO_KEY_ESCAPE]) return 0;
         al_rest(try_delay);
         if (ClientCheckResponse() == 0) // got a response
         {
             init_good = 1;
             tries = 0; // exit loop
             // save in config file
             //set_config_string("NETWORK", "server_IP", m_serveraddress);
         }
      }
      if (init_good)
      {
         sprintf(msg,"Got reply from server\n");
         printf("\n%s", msg);
         if (L_LOGGING_NETPLAY)
         {
            #ifdef LOGGING_NETPLAY
            add_log_entry2(10, active_local_player, msg);
            #endif
         }
         return 1;
      }
      else
      {
         sprintf(msg,"Did not get reply from server\n");
         printf("%s", msg);
         if (L_LOGGING_NETPLAY)
         {
            #ifdef LOGGING_NETPLAY
            add_log_entry2(10, active_local_player, msg);
            #endif
         }
         return 0;
      }
   }
   return 0;
}

int client_init_join(void)
{
   //textout_centre_ex(screen, font, "Joining..........",SCREEN_W/2, SCREEN_H/2, palette_color[13], 0);

   Packet((char *)"CJON");
   PacketAddByte(players[0].color); // request the color of current player 0
   PacketAddString(local_hostname);
   ClientSend(packetbuffer, packetsize);

   if (L_LOGGING_NETPLAY_JOIN)
   {
      #ifdef LOGGING_NETPLAY_JOIN
      sprintf(msg,"Client sent join request to server with player color:[%2d]", players[0].color);
      add_log_entry_centered_text(11, 0, 76, (char *)"", (char *)"+", (char *)"-");
      add_log_entry_position_text(11, 0, 76, 10, msg, (char *)"|", (char *)" ");
      add_log_entry_centered_text(11, 0, 76, (char *)"", (char *)"+", (char *)"-");
      #endif
   }

   int SJON = 0;
   while (!SJON) // run loop until SJON processed
   {
      al_rest(0.02);
      if (key[ALLEGRO_KEY_ESCAPE]) SJON = 98; // emergency exit
      packetsize = ClientReceive(packetbuffer);
      if ((packetsize) && (PacketRead((char *)"SJON"))) // sent by server to client wanting to join
      {
         SJON = 1;
         int pl = Packet2ByteRead();   // play level
         int server_SJON_passcount      =  Packet4ByteRead(); //
         int server_game_move_entry_pos =  Packet4ByteRead(); //
         int z = PacketGetByte();      // frame rate
         int cp = PacketGetByte();     // client player number
         int color = PacketGetByte();  // client player color
         deathmatch_pbullets = PacketGetByte();
         deathmatch_pbullets_damage = PacketGetByte();
         suicide_pbullets = PacketGetByte();


         if (cp == 99) //server full, join denied
         {
            SJON = 99;
         }
         else // join allowed
         {
            play_level = pl;
            set_passcount_timer_fps(z);

            players[0].active = 1;          // server is alway player 0 on the client
            players[0].control_method = 2; // reset from server local to remote view

            // set active local player stuff
            active_local_player = cp;
            init_player(cp, 1);
            players[cp].active = 0;          // active is not set here on purpose
            players[cp].control_method = 4; // client local player
            players[cp].color = color;
            players[cp].bitmap_index = color -1;
            players1[cp].game_move_entry_pos = server_game_move_entry_pos;

            if (L_LOGGING_NETPLAY_JOIN)
            {
               #ifdef LOGGING_NETPLAY_JOIN
               sprintf(msg,"Client received join invitation from server");
               add_log_entry_position_text(11, 0, 76, 10, msg, (char *)"|", (char *)" ");
               sprintf(msg,"Level:[%d]", play_level);
               add_log_entry_position_text(11, 0, 76, 10, msg, (char *)"|", (char *)" ");
               sprintf(msg,"Frame Rate:[%d]", z);
               add_log_entry_position_text(11, 0, 76, 10, msg, (char *)"|", (char *)" ");
               sprintf(msg,"Player Number:[%d]", cp);
               add_log_entry_position_text(11, 0, 76, 10, msg, (char *)"|", (char *)" ");
               sprintf(msg,"Player Color:[%d]", color);
               add_log_entry_position_text(11, 0, 76, 10, msg, (char *)"|", (char *)" ");
               sprintf(msg,"Server Passcount:[%d]", server_SJON_passcount);
               add_log_entry_position_text(11, 0, 76, 10, msg, (char *)"|", (char *)" ");
               sprintf(msg,"Server Game Moves:[%d]", server_game_move_entry_pos);
               add_log_entry_position_text(11, 0, 76, 10, msg, (char *)"|", (char *)" ");
               sprintf(msg,"Deathmatch player bullets:[%d]", deathmatch_pbullets);
               add_log_entry_position_text(11, 0, 76, 10, msg, (char *)"|", (char *)" ");
               sprintf(msg,"Deathmatch player bullet damage:[%d]", deathmatch_pbullets_damage);
               add_log_entry_position_text(11, 0, 76, 10, msg, (char *)"|", (char *)" ");
               sprintf(msg,"Suicide player bullets:[%d]", suicide_pbullets);
               add_log_entry_position_text(11, 0, 76, 10, msg, (char *)"|", (char *)" ");
               add_log_entry_centered_text(11, 0, 76, (char *)"", (char *)"+",(char *) "-");
               #endif
            }

            return 1;
         } // end of join allowed
      } // end of packet read
   } // end of while (!SJON)

   if (SJON == 98) // cancelled by player with ESC
   {
      sprintf(msg,"Client waiting for join cancelled by client\n");
      printf("%s", msg);
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
      sprintf(msg,"Server replied with 'SERVER FULL'\n");
      printf("%s", msg);
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
   ClientExit();
   ima_client = 0;
   players[active_local_player].control_method = 0; // local_control
}

int client_init(void)
{
   passcount = 0;  // just in case its not
   sprintf(msg, "Client mode started on host:[%s]",local_hostname);
   printf("%s\n", msg);
   if (L_LOGGING_NETPLAY)
   {
      #ifdef LOGGING_NETPLAY
      add_log_entry_centered_text(10, 0, 76, (char *)"", (char *)"+", (char *)"-");
      add_log_entry_position_text(10, 0, 76, 10, msg, (char *)"|", (char *)" ");
      #endif
   }
   sprintf(msg, "Looking for server:[%s]", m_serveraddress);
   printf("%s\n", msg);

   if (L_LOGGING_NETPLAY)
   {
      #ifdef LOGGING_NETPLAY
      add_log_entry_position_text(10, 0, 76, 10, msg, (char *)"|", (char *)" ");
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
      add_log_entry_position_text(10, 0, 76, 10, msg, (char *)"|", (char *)" ");
      add_log_entry_centered_text(10, 0, 76, (char *)"", (char *)"+", (char *)"-");
      #endif
   }
   for (int p=0; p<NUM_PLAYERS; p++)
   {
      init_player(p, 1);
      players[p].active = 0;
   }
   players[0].control_method = 2; // server player to show on client
   ima_client = 1;

   //  initialize driver
   int init_good = client_init_driver();

   // initialize join
   if ((init_good) && (!client_init_join())) init_good = 0;

   if (!init_good) // reset if fails
   {
      players[0].active = 1;
      ima_client = 0;
      sprintf(msg, "Error: Client initialization failed");
      printf("%s\n", msg);
      m_err(msg);
      if (L_LOGGING_NETPLAY)
      {
         #ifdef LOGGING_NETPLAY
         add_log_entry2(10, 0, msg);
         #endif
      }
   }
   return init_good;
}



void read_game_step_from_packet(int x, int clf_check)
{
   // read from the packet
   int g0 = Packet4ByteRead();
   int g1 = PacketGetByte();
   int g2 = PacketGetByte();
   int g3 = PacketGetByte();

   char tmsg1[80];
   char tmsg2[80];
   char tmsg3[80];
   char tmsg4[80];
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

      int cs;
      if (clf_check)
      {
         int p = active_local_player;
         cs = g0 - passcount;                    // calculate c_csync
         players1[p].c_sync = cs;                 // set in player struct
         if (cs < 0 ) players1[p].c_sync_err++;   // check for error
         if (cs < players1[p].c_sync_min)         // check and set min
            players1[p].c_sync_min = cs;
         if (cs == 0) sprintf(tmsg4,"<---[WARNING!]");
         if (cs < 0)  sprintf(tmsg4,"<---[ERROR!]");
         sprintf(tmsg3,"- frame lead:[%d]%s", cs, tmsg4);

      } // end of if clf check
   }  // end of if not duplicate

   if (L_LOGGING_NETPLAY_game_move)
   {
      #ifdef LOGGING_NETPLAY_game_move
      sprintf(msg,"%s %s %s\n", tmsg1, tmsg2, tmsg3);
      add_log_entry2(38, g2, msg);
      #endif
   }
}

void client_timer_adjust(void)
{
   int p = active_local_player;
   if (passcount == players1[p].last_sdat_lpc)  // only if just received sdat; use for sync
   {
      players1[p].server_sync = players1[p].last_sdat_fpc - passcount;
      int fps_chase = passcount_timer_fps + players1[p].server_sync - server_lead_frames;
      if (fps_chase < 4) fps_chase = 4; // never let this go negative
      al_set_timer_speed(fps_timer, ( 1 / (float) fps_chase));
      if (L_LOGGING_NETPLAY_client_timer_adjust)
      {
         #ifdef LOGGING_NETPLAY_client_timer_adjust
         sprintf(msg,"[timer adjust] server_sync:[%d] fps:[%d] server_lead_frames:%d\n", players1[p].server_sync, fps_chase, server_lead_frames);
         add_log_entry2(25, p, msg);
         #endif
      }
   }
}

int process_chdf_packet(void)
{
   int retval = 0;
   int p = PacketGetByte();
   int chdf_src_passcount = Packet4ByteRead();
   int chdf_dst_passcount = Packet4ByteRead();
   int chdf_seq = PacketGetByte();
   int chdf_max_seq = PacketGetByte();
   int chdf_sb = Packet4ByteRead();
   int chdf_sz = Packet4ByteRead();

   if (p != active_local_player)
   {
      sprintf(msg, "ERROR! rx chdf player:[%d] does not match local player:[%d]\n", p, active_local_player);
      printf("%s", msg);
      #ifdef LOGGING_NETPLAY
      add_log_entry2(10, p, msg);
      #endif
   }
   else
   {

      if (L_LOGGING_NETPLAY_chdf_all_packets)
      {
         #ifdef LOGGING_NETPLAY_chdf_all_packets
         sprintf(msg, "rx chdf piece [%d of %d] [%d to %d] st:%4d sz:%4d \n", chdf_seq+1, chdf_max_seq, chdf_src_passcount, chdf_dst_passcount, chdf_sb, chdf_sz);
         add_log_entry2(28, active_local_player, msg);
         #endif
      }
      memcpy(chdf+chdf_sb, packetbuffer+23, chdf_sz);  // put the data in the buffer
      chdf_pieces[chdf_seq] = chdf_dst_passcount;      // mark it with chdf_dst_passcount

      // did we just get the last packet?
      int good = 1; // yes by default
      for (int i=0; i< chdf_max_seq; i++)
         if (chdf_pieces[i] != chdf_dst_passcount) good = 0; // no, if any piece not at latest passcount

      if (good)
      {
         char tmsg1[80];
         char tmsg2[80];
         char tmsg3[80];

         players1[active_local_player].chdf_rx++;
         sprintf(tmsg1, "rx chdf complete [%d to %d]", chdf_src_passcount, chdf_dst_passcount);

         if (chdf_dst_passcount < passcount)
         {
            players1[active_local_player].chdf_late++;
            sprintf(tmsg2, "LATE!");
         }
         else
         {
            players1[active_local_player].chdf_on_time++;
            sprintf(tmsg2, "on time");
        }

         // decompress chdf to dif
         uLongf destLen= sizeof(dif);
         uncompress((Bytef*)dif, (uLongf*)&destLen, (Bytef*)chdf, sizeof(chdf));

         if (destLen == CHUNK_SIZE)
         {
            sprintf(tmsg3, "good decompress");
            dif_id[0] = chdf_src_passcount;
            dif_id[1] = chdf_dst_passcount;
            retval = 1;
         }
         else sprintf(tmsg3, "bad decompress");

         if (L_LOGGING_NETPLAY_chdf)
         {
            #ifdef LOGGING_NETPLAY_chdf
            sprintf(msg, "%s %s %s\n", tmsg1, tmsg2, tmsg3);
            add_log_entry2(27, active_local_player, msg);
            #endif
         }
      }
   }
   return retval;
}


void client_apply_diff()
{
   int p = active_local_player;
   if (L_LOGGING_NETPLAY_chdf_when_to_apply)
   {
      #ifdef LOGGING_NETPLAY_chdf_when_to_apply
      sprintf(msg, "base:[%d] - last dif:[%d to %d]\n", clientl_chdf_id,  dif_id[0], dif_id[1] );
      add_log_entry2(29, p, msg);
      #endif
   }
   // check to see if passcounts match and its time to apply dif
   if (passcount == dif_id[1]) // current passcount is dif target
   {

      if (clientl_chdf_id != dif_id[0])  // stored state is NOT dif source
      {
         // if server has sent dif from start (0), reset our base to 0
         if (dif_id[0] == 0)
         {
            if (L_LOGGING_NETPLAY_chdf)
            {
               #ifdef LOGGING_NETPLAY_chdf
               sprintf(msg, "Resetting client base state to zero\n");
               add_log_entry2(27, p, msg);
               #endif
            }
            memset(clientl_chdf, 0, CHUNK_SIZE);
            clientl_chdf_id = 0;
         }
         else
         {
            if (L_LOGGING_NETPLAY_chdf)
            {
               #ifdef LOGGING_NETPLAY_chdf
               sprintf(msg, "!! chdf cannot be applied (wrong client base) %d %d\n", clientl_chdf_id, dif_id[0]);
               add_log_entry2(27, p, msg);
               #endif
            }
         }
      }


      if (clientl_chdf_id == dif_id[0])  // stored state is dif source
      {

         char tmsg[80];
         int dif_corr = 0;

         // apply dif to base state
         apply_chunk_dif(clientl_chdf, dif, CHUNK_SIZE);


         if (dif_id[0] != 0) // check dif, except for passcount 0
         {


            // fix control methods to match server
            players[0].control_method = 3;
            players[p].control_method = 2;



            // make copy of current state
            char tmp[CHUNK_SIZE];
            state_to_chunk(tmp);

            // compare copy to modified base state
            if (memcmp(tmp, clientl_chdf, CHUNK_SIZE))
            {
               dif_corr = 1;
               players1[p].dif_corr++;

               sprintf(tmsg, "corrections applied - total corrections:%d", players1[p].dif_corr);

               // show dif
               show_chunk_dif(tmp, clientl_chdf);

               // copy modified base state to current game state
               chnk_to_state(clientl_chdf);

            }
            else
            {
              sprintf(tmsg, "no differences");
              dif_corr = 0;
            }
         }
         else // no dif checking,  just blindly copy (used for join)
         {
            sprintf(tmsg, "applied (no check)");

            // copy modified base state to current game state
            chnk_to_state(clientl_chdf);
         }
         clientl_chdf_id = passcount; // update client base passcount


         // restore control methods
         players[0].control_method = 2;
         players[p].control_method = 4;


         if (L_LOGGING_NETPLAY_chdf)
         {
            #ifdef LOGGING_NETPLAY_chdf
            sprintf(msg, "dif [%d to %d] %s\n", dif_id[0], dif_id[1], tmsg);
            add_log_entry2(27, p, msg);
            #endif
         }
         // send ack to server
         Packet((char *)"chak");
         PacketAddByte(p);
         PacketAddByte(dif_corr);
         PacketAdd4Bytes(passcount);
         ClientSend(packetbuffer, packetsize);

      }
   }
}

void client_block_until_good_chdf_received(void)
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
      if (key[ALLEGRO_KEY_F11]) fast_exit(65); // in case we get trapped here and need a way out
      if ((packetsize = ClientReceive(packetbuffer)))
      {
         if(PacketRead((char *)"chdf"))
         {
             done = process_chdf_packet();
         }
      }
   }

   // set passcounts
   passcount = dif_id[1];
//   extern int timer_passcount;
//   timer_passcount = passcount;
   al_set_timer_count(fps_timer, passcount);

   players1[p].last_sdat_lpc = passcount + 200;

   client_apply_diff();

   // redraw blocks just in case
   init_l2000();


   log_timer = clock(); // start timing for chase and lock
   if (L_LOGGING_NETPLAY_JOIN)
   {
      #ifdef LOGGING_NETPLAY_JOIN
      sprintf(msg, "Passcounts updated - starting chase and lock");
      add_log_entry_header(11, p, msg, 1);
       //printf("%s", msg);
      #endif
   }


   // check for other active clients and set their control methods to 2
   for (int pp=1; pp<NUM_PLAYERS; pp++)
      if (players[pp].active) players[pp].control_method = 2;

   players[p].control_method = 4; // set my own control method to 4


   // send ack up to this point (we don't care about previous game moves anymore)
   Packet((char *)"sdak");
   PacketAddByte(p);
   PacketAdd4Bytes(passcount);
   PacketAdd4Bytes(players1[p].game_move_entry_pos); // new entry pos
   PacketAdd4Bytes(players1[p].chdf_late);
   PacketAdd4Bytes(players1[p].frames_skipped);
   ClientSend(packetbuffer, packetsize);
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

   if (passcount % 40 == 0) // tally freq = 40 frames = 1s
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
   if ((passcount > 0) && (players1[p].last_sdat_lpc > 0))
   {
       int ss = passcount - players1[p].last_sdat_lpc;
       // printf("pc:%d lspc:%d  ss:%d\n", passcount, players1[p].last_sdat_lpc, ss);
       if (ss > 120)
       {
          sprintf(msg, "Player %d LOST SERVER CONNECTION", p);

          #ifdef LOGGING_NETPLAY
          add_log_entry_header(10, p, msg, 2);
          char tmsg[100];
          sprintf(tmsg, "passcount:[%d] last_sdat_rx:[%d] dif:[%d]", passcount, players1[p].last_sdat_lpc, ss);
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


   if (passcount == 0) client_block_until_good_chdf_received();

   process_bandwidth_counters(p);


   if (key[ALLEGRO_KEY_F11]) fast_exit(65); // in case we get trapped here and need a way out


   while ((packetsize = ClientReceive(packetbuffer))) // rx multiple per passcount
   {
      if (key[ALLEGRO_KEY_F11]) fast_exit(65); // in case we get trapped here and need a way out

      if(PacketRead((char *)"chdf")) process_chdf_packet();

      if(PacketRead((char *)"serr"))
      {
         int sp = PacketGetByte();
         int serr_type = PacketGetByte();
         int serr_passcount = Packet4ByteRead();
         int serr_c_sync = Packet4ByteRead();
         int serr_c_sync_err = Packet4ByteRead();

         if (sp != p)
         {
            sprintf(msg, "ERROR! rx serr player:[%d] does not match local player:[%d]\n", sp, p);
            printf("%s", msg);
            #ifdef LOGGING_NETPLAY
            add_log_entry2(10, p, msg);
            #endif
         }
         else
         {
            if (serr_type == 1) // error type (1 = cdat late)

            players1[p].serr_c_sync_err = serr_c_sync_err;
            players1[p].serr_display_timer = 120;

            sprintf(msg,"Error: server dropped cdat s_pc:%d  s_csync:%d  tot_err:%d\n", serr_passcount, serr_c_sync, serr_c_sync_err);
            if (L_LOGGING_NETPLAY_cdat)
            {
               #ifdef LOGGING_NETPLAY_cdat
               add_log_entry2(35, p, msg);
               #endif
            }
         }
      }
      if(PacketRead((char *)"sdat"))
      {
         char tmsg[20];
         int sp = PacketGetByte();
         players1[p].last_sdat_fpc = Packet4ByteRead();
         int start_entry = Packet4ByteRead();
         int num_entries = PacketGetByte();

         if (sp != p)
         {
            sprintf(msg, "ERROR! rx sdat player:[%d] does not match local player:[%d]\n", sp, p);
            printf("%s", msg);
            #ifdef LOGGING_NETPLAY
            add_log_entry2(10, p, msg);
            #endif
         }
         else
         {
            players1[p].sdat_total++ ; // total sdat packets rx'd


            players1[p].last_sdat_lpc = passcount; // client's local passcount of last sync packet

            if (num_entries == 0) sprintf(tmsg,"sync only");

            int nep = start_entry + num_entries; // new entry position
            if (nep > game_move_entry_pos) strcpy(tmsg,"new moves");
            else if (num_entries)
            {
               sprintf(tmsg,"no new moves");
               players1[p].sdat_skipped++ ; // total sdat packets skipped
               players1[p].moves_skipped += num_entries;  // moves skipped

            }
            if (L_LOGGING_NETPLAY_sdat)
            {
               #ifdef LOGGING_NETPLAY_sdat
               sprintf(msg,"rx sdat [server_frame:%d] - strt:%d num:%d - %s\n", players1[p].last_sdat_fpc, start_entry, num_entries, tmsg);
               add_log_entry2(37, p, msg);
               #endif
            }

            if (nep > game_move_entry_pos) // only enter if they are newer
            {
               for (int x=start_entry; x<nep; x++)
                  read_game_step_from_packet(x, 1);
               game_move_entry_pos = nep;
            }

            Packet((char *)"sdak");
            PacketAddByte(p);
            PacketAdd4Bytes(passcount);
            PacketAdd4Bytes(nep);
            PacketAdd4Bytes(players1[p].chdf_late);
            PacketAdd4Bytes(players1[p].frames_skipped);
            ClientSend(packetbuffer, packetsize);

            if (L_LOGGING_NETPLAY_sdak)
            {
               #ifdef LOGGING_NETPLAY_sdak
               sprintf(msg,"tx sdak nep:[%d] chdf_late:[%d] frames skipped:%d\n", nep, players1[p].chdf_late, players1[p].frames_skipped);
               add_log_entry2(39, p, msg);
               #endif
            }
         }

      } // end of packet read sdat
   } // end of packet rx loop

   client_apply_diff();

   client_timer_adjust();  // every frame

}

void client_local_control(int p)
{
   if (players1[p].fake_keypress_mode)
   {
      players1[p].comp_move = rand() % 64;
      if (key[ALLEGRO_KEY_F11]) fast_exit(65); // in case we get trapped here and need a way out
   }
   else set_comp_move_from_player_key_check(p);                // but don't set controls !!!


   if (players1[p].comp_move != players1[p].old_comp_move)  // players keys have changed
   {
      players1[p].old_comp_move = players1[p].comp_move;
      int fpc = passcount + control_lead_frames;  // add  to passcount
      int cm = players1[p].comp_move;

      Packet((char *)"cdat");
      PacketAddByte(p);
      PacketAdd4Bytes(fpc);
      PacketAddByte(cm);
      ClientSend(packetbuffer, packetsize);
      players1[p].cdat_packets_tx++;

//      while (key[ALLEGRO_KEY_ESCAPE]) proc_controllers(); // wait for release, but send cdat first

      if (L_LOGGING_NETPLAY_cdat)
      {
         #ifdef LOGGING_NETPLAY_cdat
         sprintf(msg,"tx cdat - move:%d\n", cm);
         add_log_entry2(35, active_local_player, msg);
         #endif
      }
   }
}

#endif

