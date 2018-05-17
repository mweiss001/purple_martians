#include "pm.h"
#ifdef NETPLAY


NET_CONN *ListenConn = NULL;                  // The listening connection
NET_CONN *ClientConn[MAX_CLIENTS] = {NULL, }; // A connection for every client

NET_CHANNEL *ListenChannel = NULL        ; // listen channel
NET_CHANNEL *ClientChannel[32] = {NULL, }; // array of channels

int ClientNum = 0;


// ---------------------------------------------------------------------------------------------------------------
// ***************************************************************************************************************
//----------------------------------------------------------------------------------------------------------------

int ServerListen() // Check for connecting clients (0 = ok, got new connection, 1 = no new connection yet
{
  if (TCP)
  {
     #ifdef CONNECTION
   	NET_CONN *newconn;
   	newconn = net_poll_listen (ListenConn);
   	if(newconn == NULL) return 1;
      else
      {
      	ClientConn[ClientNum] = newconn;
      	ClientNum++;
         sprintf(msg, "Connection received from %s", net_getpeer (newconn));
         printf("%s\n", msg);

         if (L_LOGGING_NETPLAY)
         {
            #ifdef LOGGING_NETPLAY
            add_log_entry_centered_text(11, 0, 76, (char *)"", (char *)"+", (char *)"-");
            add_log_entry_position_text(11, 0, 76, 10, msg, (char *)"|", (char *)" ");
            add_log_entry_centered_text(11, 0, 76, (char *)"", (char *)"+", (char *)"-");
            #endif
         }
      }
   	return 0;
      #endif
   } // end of if TCP
   else // UDP
   {
      #ifdef CHANNEL
   	char address[32];
   	packetsize = net_receive(ListenChannel, packetbuffer, 1024, address);
      if ((packetsize) && (PacketRead((char *)"1234")))
      {
         if (!(ClientChannel[ClientNum] = net_openchannel(NetworkDriver, NULL)))
         {
            sprintf(msg, "Error: failed to open channel for %s\n", address);
            printf("%s", msg);
            if (L_LOGGING_NETPLAY)
            {
               #ifdef LOGGING_NETPLAY
               add_log_entry2(10, 0, msg);
               #endif
            }
   			return 1;
         }
   		if (net_assigntarget (ClientChannel[ClientNum], address))
         {
            sprintf(msg, "Error: couldn't assign target `%s' to channel\n",address);
            printf("%s", msg);
            if (L_LOGGING_NETPLAY)
            {
               #ifdef LOGGING_NETPLAY
               add_log_entry2(10, 0, msg);
               #endif
            }
   			net_closechannel (ClientChannel[ClientNum]);
   			return 1;
   		}
         sprintf(msg, "Server opened channel for `%s'",address);
         printf("%s\n", msg);
         if (L_LOGGING_NETPLAY)
         {
            #ifdef LOGGING_NETPLAY
            add_log_entry_centered_text(11, 0, 76, (char *)"", (char *)"+", (char *)"-");
            add_log_entry_position_text(11, 0, 76, 10, msg, (char *)"|", (char *)" ");
            add_log_entry_centered_text(11, 0, 76, (char *)"", (char *)"+", (char *)"-");
            #endif
         }
         char bufr[32] = "5678";
         net_send (ClientChannel[ClientNum], bufr, strlen (bufr) + 1);
         ClientNum++;
         return 0; // got new connection
      }
      return 1;
      #endif
   } // end of UDP
}

int ServerInit() // Initialize the server
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
   	ListenConn = net_openconn(NetworkDriver, "");
   	if(!ListenConn)
      {
         sprintf(msg, "Error: failed to open listening connection\n");
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
   	if(net_listen(ListenConn))
      {
         sprintf(msg, "Error: cannot listen\n");
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
      sprintf(msg, "Network initialized - connection mode (TCP)");
      #endif
   } // end of if TCP
   else // UDP
   {
      #ifdef CHANNEL
      // open the listening channel;
      if (!(ListenChannel = net_openchannel(NetworkDriver, "")))
      {
         sprintf(msg, "Error: failed to open listening channel\n");
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

      if (net_assigntarget(ListenChannel, ""))
      {
         sprintf(msg, "Error: failed to assign target to listening channel\n");
         printf("%s", msg);
         m_err(msg);
         if (L_LOGGING_NETPLAY)
         {
            #ifdef LOGGING_NETPLAY
            add_log_entry2(10, 0, msg);
            #endif
         }
   		net_closechannel(ListenChannel);
   		return -1;
   	}
      sprintf(msg, "Network initialized - channel mode (UDP)");
      #endif
   } // end of UDP

   printf("%s\n", msg);
   if (L_LOGGING_NETPLAY)
   {
      #ifdef LOGGING_NETPLAY
      add_log_entry_position_text(10, 0, 76, 10, msg, (char *)"|", (char *)" ");
      #endif
   }
	return 0;
}

// Receive data from a client, and store in provided array
// (must have room for 1024 bytes). Returns the size of the stored data
int ServerReceive(void *data, int *sender)
{
   if (TCP)
   {
      #ifdef CONNECTION
      for(int n=0; n<ClientNum; n++)
      {
   	   if(ClientConn[n])
         {
   		   int l = net_receive_rdm(ClientConn[n], data, 1024);
   			if(l > 0)
            {
               #ifdef NETPLAY_bandwidth_tracking
               // add to server's counts
               players1[0].rx_current_bytes_for_this_frame += l;
               players1[0].rx_current_packets_for_this_frame++;
               // use n (who) to get player number
               for(int p=0; p<NUM_PLAYERS; p++)
                  if ((players[p].active) && (players1[p].who == n))
                  {
                     // add to client's counts
                     players1[p].rx_current_bytes_for_this_frame += l;
                     players1[p].rx_current_packets_for_this_frame++;
                  }
               #endif
   				if(sender) *sender = n;
   				return l;
   			}
   		}
      }
      #endif
   } // end of if TCP
   else // UDP
   {
      #ifdef CHANNEL
      for(int n=0; n<ClientNum; n++)
      {
   		if(ClientChannel[n])
         {
            int l = net_receive(ClientChannel[n], data, 1024, NULL);
   			if(l > 0)
            {
               #ifdef NETPLAY_bandwidth_tracking
               // add to server's counts
               players1[0].rx_current_bytes_for_this_frame += l;
               players1[0].rx_current_packets_for_this_frame++;
               // use n (who) to get player number
               for(int p=0; p<NUM_PLAYERS; p++)
                  if ((players[p].active) && (players1[p].who == n))
                  {
                     // add to client's counts
                     players1[p].rx_current_bytes_for_this_frame += l;
                     players1[p].rx_current_packets_for_this_frame++;
                  }
               #endif
   				if(sender) *sender = n;
   				return l;
   			}
   		}
      }
      #endif
   } // end of UDP
	return 0;
}
void ServerBroadcast(void *data, int len)
{
   for(int n = 0; n < ClientNum; n++)
   if (TCP)
   {
      #ifdef CONNECTION
		if(ClientConn[n]) net_send_rdm(ClientConn[n], data, len);
      #endif
   }
   else // UDP
   {
      #ifdef CHANNEL
		if(ClientChannel[n]) net_send(ClientChannel[n], data, len);
      #endif
   }
}

// send data to a specific client
void ServerSendTo(void *data, int len, int who, int player)
{
   if (TCP)
   {
      #ifdef CONNECTION
   	net_send_rdm(ClientConn[who], data, len);
      #endif
   }
   else // UDP
   {
      #ifdef CHANNEL
      net_send(ClientChannel[who], data, len);
      #endif
   }
   #ifdef NETPLAY_bandwidth_tracking
   // add to server's counts
   players1[0].tx_current_bytes_for_this_frame += len;
   players1[0].tx_current_packets_for_this_frame++;
   // add to client's counts
   players1[player].tx_current_bytes_for_this_frame += len;
   players1[player].tx_current_packets_for_this_frame++;
   #endif
}
void server_flush(void)
{
   int who;
   while (ServerReceive(packetbuffer, &who));
}

void ServerExit() // Shut the server down
{
   sprintf(msg, "Shutting down the server\n");
   printf("%s", msg);
   if (L_LOGGING_NETPLAY)
   {
      #ifdef LOGGING_NETPLAY
      add_log_entry2(10, 0, msg);
      #endif
   }
   if (TCP)
   {
      #ifdef CONNECTION
      for(int n = 0; n < ClientNum; n++)
         if(ClientConn[n])
         {
            net_closeconn(ClientConn[n]);
            ClientConn[n] = 0;
         }
   	if(ListenConn) net_closeconn(ListenConn);
   	ListenConn = NULL;
      #endif
   } // end of if TCP
   else // UDP
   {
      #ifdef CHANNEL
      for(int n=0; n<ClientNum; n++)
         if (ClientChannel[n])
         {
             net_closechannel(ClientChannel[n]);
             ClientChannel[n] = 0;
         }
   	if (ListenChannel) net_closechannel(ListenChannel);
   	ListenChannel = NULL;
      #endif
   } // end of UDP
}

// ---------------------------------------------------------------------------------------------------------------
// ***************************************************************************************************************
//----------------------------------------------------------------------------------------------------------------

void nc_server(void) // for specialized packet testing only
{
   static int who;
   if (ServerInit())  printf ("Error initializing server\n");

   int quit = 0;
   while (!quit)
   {
      if (key[ALLEGRO_KEY_ESCAPE]) quit = 1;
      ServerListen(); // listen for connections
      packetsize = ServerReceive(packetbuffer, &who); // get packets
      if (packetsize > 0)
      {
         if(PacketRead((char *)"poop"))
         {
            int pc = Packet2ByteRead();
//            printf("server rx poop:[%d]\n", pc);
            pc++;
//            printf("server tx peep:[%d]\n", pc);
            Packet((char *)"peep");
            PacketAdd2Bytes(pc);
            ServerSendTo(packetbuffer, packetsize, who, 0);
         }
      }
      if (key[ALLEGRO_KEY_0] && ClientNum)
      {
         while (key[ALLEGRO_KEY_0]);
         Packet((char *)"stoc");
         PacketAddByte(0);
         ServerSendTo(packetbuffer, packetsize, 0, 0);
      }
   }
}

// ---------------------------------------------------------------------------------------------------------------
// ***************************************************************************************************************
//----------------------------------------------------------------------------------------------------------------

int server_init(void)
{
   passcount = 0;  // just in case its not
   if (L_LOGGING_NETPLAY)
   {
      #ifdef LOGGING_NETPLAY
      add_log_entry_centered_text(10, 0, 76, (char *)"", (char *)"+", (char *)"-");

      sprintf(msg, "Server mode started");
      add_log_entry_position_text(10, 0, 76, 10, msg, (char *)"|", (char *)" ");
      printf("%s\n", msg);

      sprintf(msg, "Server hostname:    [%s]", local_hostname);
      add_log_entry_position_text(10, 0, 76, 10, msg, (char *)"|", (char *)" ");
      printf("%s\n", msg);

      sprintf(msg, "Level:              [%d]", play_level);
      add_log_entry_position_text(10, 0, 76, 10, msg, (char *)"|", (char *)" ");
      printf("%s\n", msg);

      // show time and date stamp
      char tmsg[80];
      struct tm *timenow;
      time_t now = time(NULL);
      timenow = localtime(&now);
      strftime(tmsg, sizeof(tmsg), "%Y-%m-%d  %H:%M:%S", timenow);
      sprintf(msg, "Date and time:      [%s]",tmsg);
      printf("%s\n", msg);

      add_log_entry_position_text(10, 0, 76, 10, msg, (char*)"|", (char*)" ");
      #endif
   }

   ima_server = 1;
   init_player(0, 1);
   players[0].control_method = 3; // server_local_control

   sprintf(players1[0].hostname, "%s", local_hostname);


   if (ServerInit())
   {
      sprintf(msg, "Could find no internet driver for server\n");
      printf("%s", msg);
      m_err(msg);
      if (L_LOGGING_NETPLAY)
      {
         #ifdef LOGGING_NETPLAY
         add_log_entry2(10, 0, msg);
         #endif
      }
      return 0;
   }

   // still needed or client dies at joining
   Packet((char*)"JUNK");
   ServerBroadcast(packetbuffer, packetsize);

   sprintf(msg, "Server initialized");
   printf("%s\n", msg);
   if (L_LOGGING_NETPLAY)
   {
      #ifdef LOGGING_NETPLAY
      add_log_entry_position_text(10, 0, 76, 10, msg, (char *)"|", (char *)" ");
      add_log_entry_centered_text(10, 0, 76, (char *)"", (char *)"+", (char *)"-");
      #endif
   }
   return 1;
}

void server_exit(void)
{
   ServerExit(); // Shut the server down
   ima_server = 0;

   // reset player data
   for (int p=0; p<NUM_PLAYERS; p++)
   {
      players[p].active = 0;
      players[p].paused = 0;
      players[p].control_method = 0;
      init_player(p, 3);
  }

   players[0].active = 1;
   players[0].control_method = 0; // local_control

}

// send chdf to a specific player
void server_send_chdf(int p)
{
   char dif[CHUNK_SIZE];
   char cmp[CHUNK_SIZE];

   // set dif dest to current passcount
   client_chdf_id[p][1] = passcount;

   // get current state
   state_to_chunk(client_chdf[p][1]);

   // make a new dif from base and current
   get_chunk_dif(client_chdf[p][0], client_chdf[p][1], dif, CHUNK_SIZE);

   // compress dif to cmp
   uLongf destLen= sizeof(cmp);
   compress2((Bytef*)cmp, (uLongf*)&destLen, (Bytef*)dif, sizeof(dif), zlib_cmp);
   int cp = destLen;

   // break it into smaller chunks here
   int num_packets = (cp / 1000) + 1;

   players1[p].cmp_dif_size = cp;
   players1[p].num_dif_packets = num_packets;

   if (L_LOGGING_NETPLAY_chdf)
   {
      #ifdef LOGGING_NETPLAY_chdf
      float cr = (float)cp*100 / (float)CHUNK_SIZE; // compression ratio
      sprintf(msg, "tx chdf p:%d [src:%d dst:%d] cmp:%d ratio:%3.2f [%d packets needed]\n",
                            p, client_chdf_id[p][0], client_chdf_id[p][1], cp, cr, num_packets);
      add_log_entry2(27, p, msg);
      //printf("%s", msg);
      #endif
   }
   int start_byte = 0;
   int end_byte = 0;

   for (int pk=0; pk <num_packets; pk++)
   {
      int ssz = 1000; // default seq size
      if (start_byte + ssz > cp) ssz = cp - start_byte; // last one is smaller

      if (end_byte > cp-1) end_byte = cp-1;

      if (L_LOGGING_NETPLAY_chdf_all_packets)
      {
         #ifdef LOGGING_NETPLAY_chdf_all_packets
         sprintf(msg, "tx chdf p:%d piece [%d of %d] [%d to %d] st:%4d sz:%4d\n",
                       p, pk+1, num_packets, client_chdf_id[p][0], client_chdf_id[p][1], start_byte, ssz);
         add_log_entry2(28, p, msg);
         #endif
      }
//         printf("size:%d   pack_seq:%d  seq_end:%d st:%d  sz:%d\n", cp, pk, num_packets, start_byte, ssz);

      Packet((char *)"chdf");
      PacketAddByte(p);
      PacketAdd4Bytes(client_chdf_id[p][0]); // src passcount
      PacketAdd4Bytes(client_chdf_id[p][1]); // dst passcount
      PacketAddByte(pk);
      PacketAddByte(num_packets);
      PacketAdd4Bytes(start_byte);
      PacketAdd4Bytes(ssz);
      memcpy(packetbuffer+packetsize, cmp+start_byte, ssz);
      packetsize += ssz;

      ServerSendTo(packetbuffer, packetsize, players1[p].who, p);

      start_byte+=1000;
   }
}

void server_send_chdf(void)
{
   // figure out when and what players to send chdf to
   if (passcount > 1)
   {
      if (passcount % chdf_freq == 0)
      {
         int p = players1[0].n_chdf; // get last player we sent to
         int not_found = 0;
         do
         {
            if (++p > 7) p = 1; // only look at 1-7
            not_found++;
         } while ((players[p].control_method != 2) && (not_found < 8));
         if (not_found == 8) p = 0;   // if no clients found set to 0 so no send will happen
         players1[0].n_chdf = p;      // set last player we sent to
         if (p) server_send_chdf(p);  // send
         //printf("[%4d] p:%d\n", passcount, p);
      }

      // send if player has just joined
      for (int p=1; p<NUM_PLAYERS; p++)
         if ((!players[p].active) && (players[p].control_method == 2) && (!players1[p].join_chdf_sent))
         {
            server_send_chdf(p);
            players1[p].join_chdf_sent = 1;
         }
   }
}

void server_send_sdat(void)
{
   for (int p=1; p<NUM_PLAYERS; p++)
      if (players[p].control_method == 2)
      {
         if (players1[p].game_move_entry_pos < game_move_entry_pos) // client needs more data
         {
            int start_entry = players1[p].game_move_entry_pos;
            int end_entry = game_move_entry_pos;
            int num_entries = end_entry - start_entry;
            if (num_entries > 100) num_entries = 100;

            if ((players1[p].server_last_sdat_sent_start != start_entry) || // dif start pos
               (players1[p].server_last_sdat_sent_num != num_entries))      // dif num entries
            {

               players1[p].server_last_sdat_sent_start = start_entry;
               players1[p].server_last_sdat_sent_num = num_entries;

               Packet((char *)"sdat");
               PacketAddByte(p);
               PacketAdd4Bytes(passcount);
               PacketAdd4Bytes(start_entry);
               PacketAddByte(num_entries);

               for (int x=start_entry; x<start_entry + num_entries; x++)
               {
                  PacketAdd4Bytes(game_moves[x][0]); // passcount
                  PacketAddByte(game_moves[x][1]); // type
                  PacketAddByte(game_moves[x][2]); // data 1
                  PacketAddByte(game_moves[x][3]); // data 2
               }
               ServerSendTo(packetbuffer, packetsize, players1[p].who, p);

               if (L_LOGGING_NETPLAY_sdat)
               {
                  #ifdef LOGGING_NETPLAY_sdat
                  sprintf(msg,"tx sdat p:%d [strt:%d num:%d]\n", p, start_entry, num_entries);
                  add_log_entry2(37, p, msg);
                  #endif
               }
            }
         }
         else if (passcount > players1[p].server_last_sdat_sent_frame + 19) // send even if no data, every 20 frames for sync
         {
            players1[p].server_last_sdat_sent_frame = passcount;
            int start_entry = players1[p].game_move_entry_pos;

            Packet((char *)"sdat");
            PacketAddByte(p);
            PacketAdd4Bytes(passcount);
            PacketAdd4Bytes(start_entry);
            PacketAddByte(0);
            ServerSendTo(packetbuffer, packetsize, players1[p].who, p);

            if (L_LOGGING_NETPLAY_sdat)
            {
               #ifdef LOGGING_NETPLAY_sdat
               sprintf(msg,"tx sdat p:%d [strt:%d num:%d] (sync only)\n", p, start_entry, 0);
               add_log_entry2(37, p, msg);
               #endif
            }
         }
      }
}


void proc_player_drop(void)
{
   // check to see if we need to drop clients
   for (int p=1; p<NUM_PLAYERS; p++)   // server only; skip p[0]
      if (players[p].control_method == 2)
      {
         if ((players[p].active) && (players1[p].server_sync > 100))
         {
            //printf("[%4d] server_sync:[%4d] drop p:%d \n", passcount, players1[p].server_sync, p);
            add_game_move(passcount + 4, 1, p, 71); // make client inactive (reason sync > 100)

            #ifdef LOGGING_NETPLAY
            sprintf(msg,"Server dropped player:%d (server sync > 100)", p);
            add_log_entry_header(10, p, msg, 1);
            #endif

         }

         if (players1[p].last_sdak_rx + 100 < passcount)
         {
            //printf("[%4d][%4d] drop p:%d \n", passcount, players1[p].last_sdak_rx, p);
            add_game_move(passcount + 4, 1, p, 71); // make client inactive (reason no sdak for 100 frames)

            #ifdef LOGGING_NETPLAY
            sprintf(msg,"Server dropped player:%d (last sdat rx > 100)", p);
            add_log_entry_header(10, p, msg, 1);
            #endif

         }
      }
}


void proc_server_check(void)
{
   for (int p1=1; p1<NUM_PLAYERS; p1++)
      for (int p2=1; p2<NUM_PLAYERS; p2++)
         if (p1 != p2)
         {
            int p1w = players1[p1].who;
            int p2w = players1[p2].who;
            if ((p1w != 99) && (p2w != 99))
               if (p1w == p2w) // we have a duplicate
               {
                  // do something here
                  sprintf(msg, "ERROR! duplicate whos for player:[%d] and player:[%d]\n", p1, p2);
                  printf("%s", msg);
                  #ifdef LOGGING_NETPLAY
                  add_log_entry2(10, 0, msg);
                  #endif
               }
         }
}


// check to see if this packet is from who its supposed to be and is valid
int check_packet_who(int p, int who, int type)
{
   if (players[p].control_method != 2)
   {
      //check if the player we received data for is an active client
      //sprintf(msg, "ERROR! rx cdat for player[%d].control__method[%d] != 2\n", p, players[p].control_method);
      //printf("%s", msg);
      //#ifdef LOGGING_NETPLAY
      //add_log_entry2(10, p, msg);
      //#endif
      return 0;
   }
   if ((players1[p].who != who) && (players1[p].who != 99))
   {
      char tmsg[80];
      if (type == 1) sprintf(tmsg, "cdat");
      if (type == 2) sprintf(tmsg, "sdak");
      if (type == 3) sprintf(tmsg, "chak");

      sprintf(msg, "ERROR! rx %s player[%d].who[%d] does not match packet who[%d]\n", tmsg, p, players1[p].who, who);
      printf("%s", msg);
      #ifdef LOGGING_NETPLAY
      add_log_entry2(10, p, msg);
      #endif
      return 0;
   }
   return 1;
}




void server_control() // this is the main server loop to process packet send and receive
{
   static int who;

   if (passcount == 0) reset_states(); // for chdf

   for(int p=0; p<NUM_PLAYERS; p++)
      if (players[p].active) process_bandwidth_counters(p);

   proc_player_drop();  // check to see if we need to drop clients

   proc_server_check(); // check for inconsistancies

   ServerListen();      // keep looking for new client connections
   server_send_sdat();  // send sdats to sync game_move data to clients
   server_send_chdf();  // send dif states to ensure clients have same state


   while((packetsize = ServerReceive(packetbuffer, &who)))
   {
      if(PacketRead((char *)"cdat"))
      {
         int p = PacketGetByte();
         int pc = Packet4ByteRead();
         int cm = PacketGetByte();

         if (check_packet_who(p, who, 1))
         {
            // how far ahead is the passcount for this move, compare to server passcount
            int c_sync = players1[p].c_sync = pc - passcount;

            // keep track of the minimum c_sync
            if (c_sync < players1[p].c_sync_min) players1[p].c_sync_min = c_sync;

            char tmsg1[100];
            sprintf(tmsg1,"rx cdat p:%d client_pc:[%d] c_sync:[%d]", p, pc, c_sync );
            char tmsg2[100];
            sprintf(tmsg2,"\n");

            // this special case here is to fix bug that occurs when server_lead_frames > 0, and quit move doesn't get synced back to the client
            if (cm == 127) // client quit
            {
               add_game_move(pc+2, 5, p, cm);  // put in future
               sprintf(tmsg2,"<-- player:%d quit\n", p);
            }
            else
            {
               if (c_sync >= 0) add_game_move(pc, 5, p, cm); // add to game_move array
               else                                          // unless late, then drop and inc error
               {
                  players1[p].c_sync_err++;
                  sprintf(tmsg2, "<--ERROR! (total errors:%d)\n", players1[p].c_sync_err);

                  Packet((char *)"serr"); // server error
                  PacketAddByte(p);
                  PacketAddByte(1); // error type 1
                  PacketAdd4Bytes(passcount);
                  PacketAdd4Bytes(c_sync);
                  PacketAdd4Bytes(players1[p].c_sync_err);
                  ServerSendTo(packetbuffer, packetsize, who, p);
               }
            }
            if (L_LOGGING_NETPLAY_cdat)
            {
               #ifdef LOGGING_NETPLAY_cdat
               sprintf(msg, "%s %s", tmsg1, tmsg2);
               add_log_entry2(35, p, msg);
               #endif
            }
         }
      }

      if(PacketRead((char *)"chak"))
      {
         // client acknowledged up to this state
         int p = PacketGetByte();
         int dif_corr = PacketGetByte();
         int ack_pc = Packet4ByteRead();

         if (check_packet_who(p, who, 3))

         {

            players1[p].dif_corr += dif_corr;

            char tmsg1[80];
            char tmsg2[80];

            sprintf(tmsg1, "rx chak p:%d ack frame:%d corr:%d total_corr:%d", p, ack_pc, dif_corr, players1[p].dif_corr );

            if (ack_pc == client_chdf_id[p][1]) // check to make sure we have a copy of acknowledged state
            {
               // acknowledged state is out new base state
               memcpy(client_chdf[p][0], client_chdf[p][1], CHUNK_SIZE); // copy 1 to 0
               client_chdf_id[p][0] = ack_pc; // new passcount
               sprintf(tmsg2, "set new base");
            }
            else // we don't have a copy of acknowledged state !!!
            {
               sprintf(tmsg2, "failed to set new base! cl:%d ", client_chdf_id[p][1]);

               // reset base to all zero
               memset(client_chdf[p][0], 0, CHUNK_SIZE);

               // set base passcount to 0
               client_chdf_id[p][0] = 0;

            }

            if (L_LOGGING_NETPLAY_chdf)
            {
               #ifdef LOGGING_NETPLAY_chdf
               sprintf(msg, "%s %s\n", tmsg1, tmsg2);
               add_log_entry2(27, p, msg);
               #endif
            }
         }
      }
      if(PacketRead((char *)"sdak"))
      {
         int p = PacketGetByte();
         int client_pc = Packet4ByteRead();
         int new_entry_pos = Packet4ByteRead();

         if (check_packet_who(p, who, 2))
         {

            players1[p].chdf_late = Packet4ByteRead();
            players1[p].frames_skipped = Packet4ByteRead();

            // mark as rx'ed by setting new entry pos
            players1[p].game_move_entry_pos = new_entry_pos;

            // set server_sync in player struct
            players1[p].server_sync = passcount - client_pc;

            // this is used to see if client is still alive
            players1[p].last_sdak_rx = passcount;

            if (L_LOGGING_NETPLAY_sdak)
            {
               #ifdef LOGGING_NETPLAY_sdak
               sprintf(msg,"rx sdak p:%d nep:[%d] server_sync:[%d] frames skipped:%d\n", p, new_entry_pos, players1[p].server_sync, players1[p].frames_skipped);
               add_log_entry2(39, p, msg);
               #endif
            }

            if (--players1[p].made_active_holdoff < 0) players1[p].made_active_holdoff = 0;

            if ((!players1[p].made_active_holdoff) && (players[p].active == 0) && (players[p].control_method == 2) && (players1[p].server_sync < 4) && (players1[p].server_sync > -2))
            {
               players1[p].made_active_holdoff = 6;
               add_game_move(passcount + 4, 1, p, players[p].color);

               if (L_LOGGING_NETPLAY_JOIN)
               {
                  #ifdef LOGGING_NETPLAY_JOIN
                  sprintf(msg,"Player:%d has locked and will become active in 4 frames!\n", p);
                  add_log_entry2(11, p, msg);
                  #endif
               }
            }
         }
      }
      if(PacketRead((char *)"CJON"))  // sent by client wanting to join
      {
         char temp_name[16];
         int req_color = PacketGetByte();
         PacketReadString(temp_name);

         if (L_LOGGING_NETPLAY_JOIN)
         {
            #ifdef LOGGING_NETPLAY_JOIN
            sprintf(msg,"Server received join request from %s requesting color:%d", temp_name, req_color);
            add_log_entry_centered_text(11, 0, 76, (char *)"", (char *)"+", (char *)"-");
            add_log_entry_position_text(11, 0, 76, 10, msg, (char *)"|", (char *)" ");
            #endif
         }
         int cn = 99;
         // find first empty player slot
         for (int q=0; q<NUM_PLAYERS; q++)
            if ((players[q].active == 0) && (players[q].control_method == 0))
            {
               cn = q;
               break;
            }
         if (cn == 99) // no empty player slots found
         {
            if (L_LOGGING_NETPLAY_JOIN)
            {
               #ifdef LOGGING_NETPLAY_JOIN
               sprintf(msg, "Reply sent: 'SERVER FULL'");
               add_log_entry_position_text(11, 0, 76, 10, msg, (char *)"|", (char *)" ");
               add_log_entry_centered_text(11, 0, 76, (char *)"", (char *)"+", (char *)"-");
               #endif
            }

            Packet((char *)"SJON");    // reply with SJON
            PacketAdd2Bytes(0);
            PacketAdd4Bytes(0);
            PacketAdd4Bytes(0);
            PacketAddByte(0);
            PacketAddByte(99); // send SJON with 99 to indicate server full
            PacketAddByte(0);
            PacketAddByte(0);
            PacketAddByte(0);
            ServerSendTo(packetbuffer, packetsize, who, 0); // reply to the same client (who) that we received from

         }
         else // empty player slot found, proceed with join
         {
            int color = req_color;  // try to use requested color, unless already used by another player
            int color_used;
            do
            {
               color_used = 0;
               for (int q=0; q<NUM_PLAYERS; q++)
                  if ((players[q].active) || (players[q].control_method == 9) || (players[q].control_method == 2))
                     if (color == players[q].color)
                     {
                        if (++color > 15) color = 1;
                        color_used = 1;
                      }
            } while (color_used);

            init_player(cn, 1);
            players[cn].color = color;
            players[cn].bitmap_index = color - 1;
            players[cn].active = 0; //server client view only
            players[cn].control_method = 2; //server client view only
            players1[cn].who = who;
            players1[cn].last_sdak_rx = passcount + 200;
            players1[cn].game_move_entry_pos = game_move_entry_pos; // so server wont try to sync any moves less than this

            sprintf(players1[cn].hostname, "%s", temp_name);

            Packet((char *)"SJON"); // reply with SJON
            PacketAdd2Bytes(play_level);
            PacketAdd4Bytes(passcount);
            PacketAdd4Bytes(game_move_entry_pos);
            PacketAddByte(passcount_timer_fps);
            PacketAddByte(cn);
            PacketAddByte(color);
            PacketAddByte(deathmatch_pbullets);
            PacketAddByte(deathmatch_pbullets_damage);
            PacketAddByte(suicide_pbullets);

            ServerSendTo(packetbuffer, packetsize, who, cn); // reply to the same client (who) that we received from

            if (L_LOGGING_NETPLAY_JOIN)
            {
               #ifdef LOGGING_NETPLAY_JOIN
               sprintf(msg,"Server replied with join invitation:");
               add_log_entry_position_text(11, 0, 76, 10, msg, (char *)"|", (char *)" ");
               sprintf(msg,"Level:[%d]", play_level);
               add_log_entry_position_text(11, 0, 76, 10, msg, (char *)"|", (char *)" ");
               sprintf(msg,"Frame Rate:[%d]", passcount_timer_fps);
               add_log_entry_position_text(11, 0, 76, 10, msg, (char *)"|", (char *)" ");
               sprintf(msg,"Player Number:[%d]", cn);
               add_log_entry_position_text(11, 0, 76, 10, msg, (char *)"|", (char *)" ");
               sprintf(msg,"Player Color:[%d]", color);
               add_log_entry_position_text(11, 0, 76, 10, msg, (char *)"|", (char *)" ");
               sprintf(msg,"Server Passcount:[%d]", passcount);
               add_log_entry_position_text(11, 0, 76, 10, msg, (char *)"|", (char *)" ");
               sprintf(msg,"Server Game Moves:[%d]", game_move_entry_pos);
               add_log_entry_position_text(11, 0, 76, 10, msg, (char *)"|", (char *)" ");
               sprintf(msg,"Deathmatch player bullets:[%d]", deathmatch_pbullets);
               add_log_entry_position_text(11, 0, 76, 10, msg, (char *)"|", (char *)" ");
               sprintf(msg,"Deathmatch player bullet damage:[%d]", deathmatch_pbullets_damage);
               add_log_entry_position_text(11, 0, 76, 10, msg, (char *)"|", (char *)" ");
               sprintf(msg,"Suicide player bullets:[%d]", suicide_pbullets);
               add_log_entry_position_text(11, 0, 76, 10, msg, (char *)"|", (char *)" ");
               add_log_entry_centered_text(11, 0, 76, (char *)"", (char *)"+", (char *)"-");
               #endif
            }
         }
      }
  } // end of receive loop
}

void server_local_control(int p)
{
   set_comp_move_from_player_key_check(p);                  // but don't set controls !!!
   int fpc = passcount + control_lead_frames;               // add CLF to passcount
   if (players1[p].comp_move != players1[p].old_comp_move)  // players keys have changed
   {
      players1[p].old_comp_move = players1[p].comp_move;
      int cm = players1[p].comp_move;
      add_game_move(fpc, 5, p, cm);
   }
   extern int level_done;
   if (level_done) add_game_move(fpc, 6, 0, 0);             // insert level done into game move
}
#endif
