// n_server.cpp

#include "pm.h"
#ifdef NETPLAY

// n_network.h
extern int NetworkDriver;

// n_packet.h
extern char packetbuffer[1024];
extern int packetsize;

// these are never referenced outside of this file
#define MAX_CLIENTS 32
int ClientNum = 0;
NET_CONN *ListenConn = NULL;                         // listening connection
NET_CONN *ClientConn[MAX_CLIENTS] = {NULL, };        // array of connections for each client
NET_CHANNEL *ListenChannel = NULL;                   // listen channel
NET_CHANNEL *ClientChannel[MAX_CLIENTS] = {NULL, };  // array of channels for each client

int ServerInitNetwork() // Initialize the server
{
	if(NetworkInit())
   {
      sprintf(msg, "Error: failed to initialize network\n");
      printf("%s", msg);
      m_err(msg);
      if (L_LOGGING_NETPLAY) add_log_entry2(10, 0, msg);
      return -1;
   }
  if (TCP)
  {
   	ListenConn = net_openconn(NetworkDriver, "");
   	if(!ListenConn)
      {
         sprintf(msg, "Error: failed to open listening connection\n");
         printf("%s", msg);
         m_err(msg);
         if (L_LOGGING_NETPLAY) add_log_entry2(10, 0, msg);
         return -1;
      }
   	if(net_listen(ListenConn))
      {
         sprintf(msg, "Error: cannot listen\n");
         printf("%s", msg);
         m_err(msg);
         if (L_LOGGING_NETPLAY) add_log_entry2(10, 0, msg);
         return -1;
      }
      sprintf(msg, "Network initialized - connection mode (TCP)");
   }
   else // UDP
   {
      // open the listening channel;
      if (!(ListenChannel = net_openchannel(NetworkDriver, "")))
      {
         sprintf(msg, "Error: failed to open listening channel\n");
         printf("%s", msg);
         m_err(msg);
         if (L_LOGGING_NETPLAY) add_log_entry2(10, 0, msg);
         return -1;
      }

      if (net_assigntarget(ListenChannel, ""))
      {
         sprintf(msg, "Error: failed to assign target to listening channel\n");
         printf("%s", msg);
         m_err(msg);
         if (L_LOGGING_NETPLAY) add_log_entry2(10, 0, msg);
   		net_closechannel(ListenChannel);
   		return -1;
   	}
      sprintf(msg, "Network initialized - channel mode (UDP)");
   } // end of UDP

   printf("%s\n", msg);
   if (L_LOGGING_NETPLAY) add_log_entry_position_text(10, 0, 76, 10, msg, "|", " ");
	return 0;
}


int ServerListen() // Check for connecting clients (0 = ok, got new connection, 1 = no new connection yet
{
  if (TCP)
  {
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
            add_log_entry_centered_text(11, 0, 76, "", "+", "-");
            add_log_entry_position_text(11, 0, 76, 10, msg, "|", " ");
            add_log_entry_centered_text(11, 0, 76, "", "+", "-");
         }
      }
   	return 0;
   } // end of if TCP
   else // UDP
   {
   	char address[32];
   	packetsize = net_receive(ListenChannel, packetbuffer, 1024, address);
      if ((packetsize) && (PacketRead("1234")))
      {
         if (!(ClientChannel[ClientNum] = net_openchannel(NetworkDriver, NULL)))
         {
            sprintf(msg, "Error: failed to open channel for %s\n", address);
            printf("%s", msg);
            if (L_LOGGING_NETPLAY) add_log_entry2(10, 0, msg);
   			return 1;
         }
   		if (net_assigntarget (ClientChannel[ClientNum], address))
         {
            sprintf(msg, "Error: couldn't assign target `%s' to channel\n",address);
            printf("%s", msg);
            if (L_LOGGING_NETPLAY)  add_log_entry2(10, 0, msg);
   			net_closechannel (ClientChannel[ClientNum]);
   			return 1;
   		}
         sprintf(msg, "Server opened channel for `%s'",address);
         printf("%s\n", msg);
         if (L_LOGGING_NETPLAY)
         {
            add_log_entry_centered_text(11, 0, 76, "", "+", "-");
            add_log_entry_position_text(11, 0, 76, 10, msg, "|", " ");
            add_log_entry_centered_text(11, 0, 76, "", "+", "-");
         }
         char bufr[32] = "5678";
         net_send (ClientChannel[ClientNum], bufr, strlen (bufr) + 1);
         ClientNum++;
         return 0; // got new connection
      }
      return 1;
   } // end of UDP
}


// Receive data from a client, and store in provided array
// (must have room for 1024 bytes). Returns the size of the stored data
int ServerReceive(void *data, int *sender)
{
   if (TCP)
   {
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
   } // end of if TCP
   else // UDP
   {
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
   } // end of UDP
	return 0;
}

void ServerBroadcast(void *data, int len)
{
   for(int n = 0; n < ClientNum; n++)
   {
      if (TCP)
      {
         if (ClientConn[n]) net_send_rdm(ClientConn[n], data, len);
      }
      else
      {
         if (ClientChannel[n]) net_send(ClientChannel[n], data, len);
      }
   }
}

// send data to a specific client
void ServerSendTo(void *data, int len, int who, int player)
{
   if (TCP) net_send_rdm(ClientConn[who], data, len);
   else     net_send(ClientChannel[who], data, len);

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

void ServerExitNetwork() // Shut the server down
{
   sprintf(msg, "Shutting down the server\n");
   printf("%s", msg);
   if (L_LOGGING_NETPLAY) add_log_entry2(10, 0, msg);
   if (TCP)
   {
      for(int n = 0; n < ClientNum; n++)
         if(ClientConn[n])
         {
            net_closeconn(ClientConn[n]);
            ClientConn[n] = 0;
         }
   	if(ListenConn) net_closeconn(ListenConn);
   	ListenConn = NULL;
   }
   else // UDP
   {
      for(int n=0; n<ClientNum; n++)
         if (ClientChannel[n])
         {
             net_closechannel(ClientChannel[n]);
             ClientChannel[n] = 0;
         }
   	if (ListenChannel) net_closechannel(ListenChannel);
   	ListenChannel = NULL;
   }
}

// ---------------------------------------------------------------------------------------------------------------
// ***************************************************************************************************************
//----------------------------------------------------------------------------------------------------------------

int server_init(void)
{

   if (L_LOGGING_NETPLAY)
   {
      add_log_entry_centered_text(10, 0, 76, "", "+", "-");

      sprintf(msg, "Server mode started");
      add_log_entry_position_text(10, 0, 76, 10, msg, "|", " ");
      printf("%s\n", msg);

      sprintf(msg, "Server hostname:    [%s]", local_hostname);
      add_log_entry_position_text(10, 0, 76, 10, msg, "|", " ");
      printf("%s\n", msg);

      sprintf(msg, "Level:              [%d]", play_level);
      add_log_entry_position_text(10, 0, 76, 10, msg, "|", " ");
      printf("%s\n", msg);

      // show time and date stamp
      char tmsg[80];
      struct tm *timenow;
      time_t now = time(NULL);
      timenow = localtime(&now);
      strftime(tmsg, sizeof(tmsg), "%Y-%m-%d  %H:%M:%S", timenow);
      sprintf(msg, "Date and time:      [%s]",tmsg);
      printf("%s\n", msg);

      add_log_entry_position_text(10, 0, 76, 10, msg, "|", " ");
   }

   if (ServerInitNetwork())
   {
      sprintf(msg, "Could find no internet driver for server\n");
      printf("%s", msg);
      m_err(msg);
      if (L_LOGGING_NETPLAY) add_log_entry2(10, 0, msg);
      return 0;
   }

   sprintf(msg, "Server initialized");
   printf("%s\n", msg);
   if (L_LOGGING_NETPLAY)
   {
      add_log_entry_position_text(10, 0, 76, 10, msg, "|", " ");
      add_log_entry_centered_text(10, 0, 76, "", "+", "-");
   }


   // still needed or client dies at joining
   Packet("JUNK");
   ServerBroadcast(packetbuffer, packetsize);


   frame_num = 0;  // just in case its not
   players[0].active = 1;
   players[0].control_method = 3; // server_local_control
   ima_server = 1;
   sprintf(players1[0].hostname, "%s", local_hostname);

   return 1;
}

void server_exit(void)
{
   ServerExitNetwork(); // Shut the server down
   ima_server = 0;

   // reset player data
   for (int p=0; p<NUM_PLAYERS; p++) init_player(p, 1);
   players[0].active = 1;
}

// send stdf to a specific player
void server_send_stdf(int p)
{
   char dif[STATE_SIZE];
   char cmp[STATE_SIZE];

   // set dif dest to current frame_num
   srv_client_state_frame_num[p][1] = frame_num;

   // get current state
   game_vars_to_state(srv_client_state[p][1]);

   // make a new dif from base and current
   get_state_dif(srv_client_state[p][0], srv_client_state[p][1], dif, STATE_SIZE);

   // compress dif to cmp
   uLongf destLen= sizeof(cmp);
   compress2((Bytef*)cmp, (uLongf*)&destLen, (Bytef*)dif, sizeof(dif), zlib_cmp);
   int cmp_size = destLen;

   // break compressed dif into smaller pieces
   int num_packets = (cmp_size / 1000) + 1;

   players1[p].cmp_dif_size = cmp_size;
   players1[p].num_dif_packets = num_packets;

   if (L_LOGGING_NETPLAY_stdf)
   {
      float cr = (float)cmp_size*100 / (float)STATE_SIZE; // compression ratio
      sprintf(msg, "tx stdf p:%d [src:%d dst:%d] cmp:%d ratio:%3.2f [%d packets needed]\n",
                            p, srv_client_state_frame_num[p][0], srv_client_state_frame_num[p][1], cmp_size, cr, num_packets);
      add_log_entry2(27, p, msg);
      //printf("%s", msg);
   }
   int start_byte = 0;
   for (int packet_num=0; packet_num < num_packets; packet_num++)
   {
      int packet_data_size = 1000;// default size
      if (start_byte + packet_data_size > cmp_size)
          packet_data_size = cmp_size - start_byte; // last piece is smaller


      if (L_LOGGING_NETPLAY_stdf_all_packets)
      {
         sprintf(msg, "tx stdf p:%d piece [%d of %d] [%d to %d] st:%4d sz:%4d\n",
                       p, packet_num+1, num_packets, srv_client_state_frame_num[p][0], srv_client_state_frame_num[p][1], start_byte, packet_data_size);
         add_log_entry2(28, p, msg);
      }

//         printf("size:%d   pack_seq:%d  seq_end:%d st:%d  sz:%d\n", cp, pk, num_packets, start_byte, ssz);

      Packet("stdf");
      PacketPut4ByteInt(srv_client_state_frame_num[p][0]); // src frame_num
      PacketPut4ByteInt(srv_client_state_frame_num[p][1]); // dst frame_num
      PacketPut1ByteInt(packet_num);
      PacketPut1ByteInt(num_packets);
      PacketPut4ByteInt(start_byte);
      PacketPut4ByteInt(packet_data_size);
      memcpy(packetbuffer+packetsize, cmp+start_byte, packet_data_size);
      packetsize += packet_data_size;

      ServerSendTo(packetbuffer, packetsize, players1[p].who, p);

      start_byte+=1000;
   }
}

void server_send_stdf(void)
{
   // figure out when and what players to send stdf to
   if (frame_num > 1)
   {
      if (frame_num % stdf_freq == 0)
      {
         int p = players1[0].n_stdf; // get last player we sent to
         int not_found = 0;
         do
         {
            if (++p > 7) p = 1; // only look at 1-7
            not_found++;
         } while ((players[p].control_method != 2) && (not_found < 8));
         if (not_found == 8) p = 0;   // if no clients found set to 0 so no send will happen
         players1[0].n_stdf = p;      // set last player we sent to
         if (p) server_send_stdf(p);  // send
         //printf("[%4d] p:%d\n", frame_num, p);
      }

      // send if player has just joined
      for (int p=1; p<NUM_PLAYERS; p++)
         if ((!players[p].active) && (players[p].control_method == 2) && (!players1[p].join_stdf_sent))
         {
            server_send_stdf(p);
            players1[p].join_stdf_sent = 1;
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

            // to prevent multiple resends of exact same data
            if ((players1[p].server_last_sdat_sent_start != start_entry) || // different start pos
               (players1[p].server_last_sdat_sent_num != num_entries))      // different num entries
            {
               players1[p].server_last_sdat_sent_start = start_entry;
               players1[p].server_last_sdat_sent_num = num_entries;
               players1[p].server_last_sdat_sent_frame_num = frame_num;

               Packet("sdat");
               PacketPut4ByteInt(frame_num);
               PacketPut4ByteInt(start_entry);
               PacketPut1ByteInt(num_entries);

               for (int x=start_entry; x<start_entry + num_entries; x++)
               {
                  PacketPut4ByteInt(game_moves[x][0]); // frame_num
                  PacketPut1ByteInt(game_moves[x][1]); // type
                  PacketPut1ByteInt(game_moves[x][2]); // data 1
                  PacketPut1ByteInt(game_moves[x][3]); // data 2
               }
               ServerSendTo(packetbuffer, packetsize, players1[p].who, p);

               sprintf(msg,"tx sdat p:%d [strt:%d num:%d]\n", p, start_entry, num_entries);
               if (L_LOGGING_NETPLAY_sdat) add_log_entry2(37, p, msg);
            }
         }

         // send even if no data, every 20 frames for sync
         if (frame_num > players1[p].server_last_sdat_sent_frame_num + 19)
         {
            players1[p].server_last_sdat_sent_frame_num = frame_num;
            int start_entry = players1[p].game_move_entry_pos;

            Packet("sdat");
            PacketPut4ByteInt(frame_num);
            PacketPut4ByteInt(start_entry);
            PacketPut1ByteInt(0);
            ServerSendTo(packetbuffer, packetsize, players1[p].who, p);

            sprintf(msg,"tx sdat p:%d [strt:%d num:%d] (sync only)\n", p, start_entry, 0);
            if (L_LOGGING_NETPLAY_sdat) add_log_entry2(37, p, msg);
         }
      }
}


void server_proc_player_drop(void)
{
   // check to see if we need to drop clients
   for (int p=1; p<NUM_PLAYERS; p++)   // server only; skip p[0]
      if (players[p].control_method == 2)
      {
         if ((players[p].active) && (players1[p].server_sync > 100))
         {
            //printf("[%4d] server_sync:[%4d] drop p:%d \n", frame_num, players1[p].server_sync, p);
            add_game_move(frame_num + 4, 1, p, 71); // make client inactive (reason sync > 100)

            sprintf(msg,"Server dropped player:%d (server sync > 100)", p);
            if (L_LOGGING_NETPLAY) add_log_entry_header(10, p, msg, 1);
         }
         if (players1[p].server_last_sdak_rx_frame_num + 100 < frame_num)
         {
            //printf("[%4d][%4d] drop p:%d \n", frame_num, players1[p].server_last_sdak_rx_frame_num, p);
            add_game_move(frame_num + 4, 1, p, 71); // make client inactive (reason no sdak for 100 frames)

            sprintf(msg,"Server dropped player:%d (last sdat rx > 100)", p);
            if (L_LOGGING_NETPLAY) add_log_entry_header(10, p, msg, 1);
         }
      }
}



void server_proc_cdat_packet(void)
{
   int p = PacketGet1ByteInt();
   int fn = PacketGet4ByteInt();
   int cm = PacketGet1ByteInt();

   // how far ahead is the client's frame_num for this move, compared to server's frame_num
   int c_sync = players1[p].server_game_move_sync = fn - frame_num;

   // keep track of the minimum c_sync
   if (c_sync < players1[p].server_game_move_sync_min) players1[p].server_game_move_sync_min = c_sync;

   char tmsg1[100];
   sprintf(tmsg1,"rx cdat p:%d client_pc:[%d] c_sync:[%d]", p, fn, c_sync );
   char tmsg2[100];
   sprintf(tmsg2,"\n");

   // this special case here is to fix bug that occurs when server_lead_frames > 0, and quit move doesn't get synced back to the client
   if (cm == 127) // client quit
   {
      add_game_move(fn+2, 5, p, cm);  // put in future
      sprintf(tmsg2,"<-- player:%d quit\n", p);
   }
   else
   {
      if (c_sync >= 0) add_game_move(fn, 5, p, cm); // add to game_move array
      else                                          // unless late, then drop and inc error
      {
         players1[p].server_game_move_sync_err++;
         sprintf(tmsg2, "<--ERROR! (total errors:%d)\n", players1[p].server_game_move_sync_err);

         Packet("serr"); // server error
         PacketPut1ByteInt(1); // error type 1
         PacketPut4ByteInt(frame_num);
         PacketPut4ByteInt(c_sync);
         PacketPut4ByteInt(players1[p].server_game_move_sync_err);
         ServerSendTo(packetbuffer, packetsize, players1[p].who, p);
      }
   }
   sprintf(msg, "%s %s", tmsg1, tmsg2);
   if (L_LOGGING_NETPLAY_cdat) add_log_entry2(35, p, msg);
}


void server_proc_stak_packet(void)
{
   // client acknowledged up to this state
   int p = PacketGet1ByteInt();
   int dif_corr = PacketGet1ByteInt();
   int ack_pc = PacketGet4ByteInt();

   players1[p].dif_corr += dif_corr;

   char tmsg1[80];
   char tmsg2[80];

   sprintf(tmsg1, "rx stak p:%d ack frame:%d corr:%d total_corr:%d", p, ack_pc, dif_corr, players1[p].dif_corr );

   if (ack_pc == srv_client_state_frame_num[p][1]) // check to make sure we have a copy of acknowledged state
   {
      // acknowledged state is out new base state
      memcpy(srv_client_state[p][0], srv_client_state[p][1], STATE_SIZE); // copy 1 to 0
      srv_client_state_frame_num[p][0] = ack_pc; // new frame_num
      sprintf(tmsg2, "set new base");
   }
   else // we don't have a copy of acknowledged state !!!
   {
      sprintf(tmsg2, "failed to set new base! cl:%d ", srv_client_state_frame_num[p][1]);

      // reset base to all zero
      memset(srv_client_state[p][0], 0, STATE_SIZE);

      // set base frame_num to 0
      srv_client_state_frame_num[p][0] = 0;

   }

   sprintf(msg, "%s %s\n", tmsg1, tmsg2);
   if (L_LOGGING_NETPLAY_stdf) add_log_entry2(27, p, msg);
}


void server_proc_sdak_packet(void)
{
   int p = PacketGet1ByteInt();
   int client_fn = PacketGet4ByteInt();
   int new_entry_pos = PacketGet4ByteInt();
   players1[p].stdf_late = PacketGet4ByteInt();
   players1[p].frames_skipped = PacketGet4ByteInt();

   // mark as rx'ed by setting new entry pos
   players1[p].game_move_entry_pos = new_entry_pos;

   // set server_sync in player struct
   players1[p].server_sync = frame_num - client_fn;

   // this is used to see if client is still alive
   players1[p].server_last_sdak_rx_frame_num = frame_num;

   sprintf(msg,"rx sdak p:%d nep:[%d] server_sync:[%d] frames skipped:%d\n", p, new_entry_pos, players1[p].server_sync, players1[p].frames_skipped);
   if (L_LOGGING_NETPLAY_sdak) add_log_entry2(39, p, msg);

   if (--players1[p].made_active_holdoff < 0) players1[p].made_active_holdoff = 0;

   if ((!players1[p].made_active_holdoff) && (players[p].active == 0) && (players[p].control_method == 2) && (players1[p].server_sync < 4) && (players1[p].server_sync > -2))
   {
      players1[p].made_active_holdoff = 6;
      add_game_move(frame_num + 4, 1, p, players[p].color);
      sprintf(msg,"Player:%d has locked and will become active in 4 frames!\n", p);
      if (L_LOGGING_NETPLAY_JOIN) add_log_entry2(11, p, msg);
   }
}


void server_proc_CJON_packet(int who)
{
   char temp_name[16];
   int color = PacketGet1ByteInt();
   PacketReadString(temp_name);

   if (L_LOGGING_NETPLAY_JOIN)
   {
      sprintf(msg,"Server received join request from %s requesting color:%d", temp_name, color);
      add_log_entry_centered_text(11, 0, 76, "", "+", "-");
      add_log_entry_position_text(11, 0, 76, 10, msg, "|", " ");
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
         sprintf(msg, "Reply sent: 'SERVER FULL'");
         add_log_entry_position_text(11, 0, 76, 10, msg, "|", " ");
         add_log_entry_centered_text(11, 0, 76, "", "+", "-");
      }

      Packet("SJON");    // reply with SJON
      PacketPut2ByteInt(0);
      PacketPut4ByteInt(0);
      PacketPut4ByteInt(0);
      PacketPut1ByteInt(0);
      PacketPut1ByteInt(99); // send SJON with player 99 to indicate server full
      PacketPut1ByteInt(0);
      PacketPut1ByteInt(0);
      PacketPut1ByteInt(0);
      PacketPut1ByteInt(0);
      ServerSendTo(packetbuffer, packetsize, who, 0);
   }
   else // empty player slot found, proceed with join
   {
      // try to use requested color, unless already used by another player
      while (is_player_color_used(color)) if (++color > 15) color = 1;

      init_player(cn, 1);
      players[cn].color = color;
      players[cn].control_method = 2; //server client view only
      players1[cn].who = who;
      players1[cn].server_last_sdak_rx_frame_num = frame_num + 200;
      players1[cn].game_move_entry_pos = game_move_entry_pos; // so server wont try to sync any moves less than this
      sprintf(players1[cn].hostname, "%s", temp_name);

      Packet("SJON"); // reply with SJON
      PacketPut2ByteInt(play_level);
      PacketPut4ByteInt(frame_num);
      PacketPut4ByteInt(game_move_entry_pos);
      PacketPut1ByteInt(frame_speed);
      PacketPut1ByteInt(cn);
      PacketPut1ByteInt(color);
      PacketPut1ByteInt(deathmatch_pbullets);
      PacketPut1ByteInt(deathmatch_pbullets_damage);
      PacketPut1ByteInt(suicide_pbullets);
      ServerSendTo(packetbuffer, packetsize, who, cn);

      if (L_LOGGING_NETPLAY_JOIN)
      {
         sprintf(msg,"Server replied with join invitation:");
         add_log_entry_position_text(11, 0, 76, 10, msg, "|", " ");
         sprintf(msg,"Level:[%d]", play_level);
         add_log_entry_position_text(11, 0, 76, 10, msg, "|", " ");
         sprintf(msg,"Frame Rate:[%d]", frame_speed);
         add_log_entry_position_text(11, 0, 76, 10, msg, "|", " ");
         sprintf(msg,"Player Number:[%d]", cn);
         add_log_entry_position_text(11, 0, 76, 10, msg, "|", " ");
         sprintf(msg,"Player Color:[%d]", color);
         add_log_entry_position_text(11, 0, 76, 10, msg, "|", " ");
         sprintf(msg,"Server frame_num:[%d]", frame_num);
         add_log_entry_position_text(11, 0, 76, 10, msg, "|", " ");
         sprintf(msg,"Server Game Moves:[%d]", game_move_entry_pos);
         add_log_entry_position_text(11, 0, 76, 10, msg, "|", " ");
         sprintf(msg,"Deathmatch player bullets:[%d]", deathmatch_pbullets);
         add_log_entry_position_text(11, 0, 76, 10, msg, "|", " ");
         sprintf(msg,"Deathmatch player bullet damage:[%d]", deathmatch_pbullets_damage);
         add_log_entry_position_text(11, 0, 76, 10, msg, "|", " ");
         sprintf(msg,"Suicide player bullets:[%d]", suicide_pbullets);
         add_log_entry_position_text(11, 0, 76, 10, msg, "|", " ");
         add_log_entry_centered_text(11, 0, 76, "", "+", "-");
      }
   }
}

void server_control() // this is the main server loop to process packet send and receive
{
   if (frame_num == 0) reset_states(); // for stdf

   ServerListen();      // listen for new client connections

   int who;
   while((packetsize = ServerReceive(packetbuffer, &who)))
   {
      if(PacketRead("cdat")) server_proc_cdat_packet();
      if(PacketRead("stak")) server_proc_stak_packet();
      if(PacketRead("sdak")) server_proc_sdak_packet();
      if(PacketRead("CJON")) server_proc_CJON_packet(who);
  }

   server_send_sdat();  // send sdats to sync game_move data to clients
   server_send_stdf();  // send dif states to ensure clients have same state

   server_proc_player_drop();  // check to see if we need to drop clients

   for(int p=0; p<NUM_PLAYERS; p++) if (players[p].active) process_bandwidth_counters(p);
}

void server_local_control(int p)
{
   int fn = frame_num + control_lead_frames;               // add control_lead_frames to frame_num
   set_comp_move_from_player_key_check(p);
   if (players1[p].comp_move != players1[p].old_comp_move) // players controls have changed
   {
      players1[p].old_comp_move = players1[p].comp_move;
      add_game_move(fn, 5, p, players1[p].comp_move);
   }
   if (level_done) add_game_move(fn, 6, 0, 0);             // insert level done into game move
}
#endif
