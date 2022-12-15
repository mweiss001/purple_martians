// n_server.cpp

#include "pm.h"

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
            ClientConn[n] = NULL;
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
             ClientChannel[n] = NULL;
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
      log_versions();
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

   players[0].control_method = 3; // server_local_control
   ima_server = 1;
   strncpy(players1[0].hostname, local_hostname, 16);

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


// send stdf to a specific client
void server_send_stdf(int p)
{
   // if last_ack_state_frame == 0 set base to all zeros
   if (srv_client_state_frame_num[p][0] == 0) memset(srv_client_state[p][0], 0, STATE_SIZE);

   char dif[STATE_SIZE];
   char cmp[STATE_SIZE];

   // put current state in client's state slot 1
   game_vars_to_state(srv_client_state[p][1]);

   // put current frame_num
   srv_client_state_frame_num[p][1] = frame_num;

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
   }
   int start_byte = 0;
   for (int packet_num=0; packet_num < num_packets; packet_num++)
   {
      int packet_data_size = 1000; // default size
      if (start_byte + packet_data_size > cmp_size) packet_data_size = cmp_size - start_byte; // last piece is smaller

      if (L_LOGGING_NETPLAY_stdf_all_packets)
      {
         sprintf(msg, "tx stdf piece [%d of %d] [%d to %d] st:%4d sz:%4d\n",
                       packet_num+1, num_packets, srv_client_state_frame_num[p][0], srv_client_state_frame_num[p][1], start_byte, packet_data_size);
         add_log_entry2(28, p, msg);
      }

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
   stdf_freq = 3; // overide for testing

    // is it time to make a new dif and send to clients?
   if (frame_num % stdf_freq == 0)
   {
      if (srv_stdf_state_frame_num[0] > 0) // skip rewind and replay the first time
      {

         // rewind and fast forward from last stdf state to apply missed game moves received late

         int saved_current_frame_num = frame_num; // save old current frame

         players1[0].server_rewind_frames = frame_num - srv_stdf_state_frame_num[0];
         if (players1[0].server_rewind_frames > players1[0].server_rewind_frames_max)
            players1[0].server_rewind_frames_max = players1[0].server_rewind_frames;


         frame_num = srv_stdf_state_frame_num[0]; // rewind frame num
         state_to_game_vars(srv_stdf_state[0]);   // rewind state

         while (frame_num < saved_current_frame_num) loop_frame(); // fast forward
      }

      // rewind and replay is done, now save this state as a base for next time
      game_vars_to_state(srv_stdf_state[0]);
      srv_stdf_state_frame_num[0] = frame_num;

      // send dif based on this state to all clients
      for (int p=1; p<NUM_PLAYERS; p++)
         if (players[p].control_method == 2) server_send_stdf(p);
   }
}

//void server_send_stdf(void)
//{
//   // 1 1234567
//   // 2 1357 246
//   // 3 147 25 36
//   // 4 15 26 37 4
//   // 5 16 27 3 4 5
//   // 6 17 2 3 4 5
//   // 7 1 2 3 4 5 6 7
//   // 8 1 2 3 4 5 6 7 X
//
//   stdf_freq = 1; // overide for testing
//   int stagger = 0;
//   for (int p=1; p<NUM_PLAYERS; p++)
//      if (players[p].control_method == 2)
//      {
//         if ((stagger % stdf_freq == 0) && (frame_num % stdf_freq == 0)) server_send_stdf(p);
//         if ((stagger % stdf_freq == 1) && (frame_num % stdf_freq == 1)) server_send_stdf(p);
//         if ((stagger % stdf_freq == 2) && (frame_num % stdf_freq == 2)) server_send_stdf(p);
//         if ((stagger % stdf_freq == 3) && (frame_num % stdf_freq == 3)) server_send_stdf(p);
//         if ((stagger % stdf_freq == 4) && (frame_num % stdf_freq == 4)) server_send_stdf(p);
//         if ((stagger % stdf_freq == 5) && (frame_num % stdf_freq == 5)) server_send_stdf(p);
//         if ((stagger % stdf_freq == 6) && (frame_num % stdf_freq == 6)) server_send_stdf(p);
//         stagger++;
//      }
//}


// old method
//void server_send_stdf(void)
//{
//   // figure out when and what players to send stdf to
//   if ((frame_num > 1) && (level_done_mode == 0))
//   {
//      if (frame_num % stdf_freq == 0)
//      {
//         int p = players1[0].n_stdf; // get last player we sent to
//         int not_found = 0;
//         int loop_done = 0;
//         while (!loop_done)
//         {
//            if (++p > 7) p = 1; // only look at 1-7
//            not_found++;
////            if ((players[p].active) && (players[p].control_method == 2)) loop_done = 1;
//            if (players[p].control_method == 2) loop_done = 1;
//            if (players[p].control_method == 8) loop_done = 1;
//            if (not_found > 8) // no clients found
//            {
//               loop_done = 1;
//               p = 0; // set to 0 so no send will happen
//            }
//         }
//         players1[0].n_stdf = p;      // set last player we sent to
//         if (p) server_send_stdf(p);  // send
//         //printf("[%4d] p:%d\n", frame_num, p);
//      }
//   }
//}

/*
   stdf_freq - frames in between stdf sends
   only one client is sent to at a time
   it finds the last one sent to, then searches for the next one to send to
*/


void server_proc_player_drop(void)
{
   // check to see if we need to drop clients
   for (int p=1; p<NUM_PLAYERS; p++)   // server only; skip p[0]
      if (players[p].control_method == 2)
      {
         if ((players[p].active) && (players1[p].server_sync > 100))
         {
            //printf("[%4d] server_sync:[%4d] drop p:%d \n", frame_num, players1[p].server_sync, p);
            add_game_move(frame_num + 4, 1, p, 70); // make client inactive (reason sync > 100)

            sprintf(msg,"Server dropped player:%d (server sync > 100)", p);
            if (L_LOGGING_NETPLAY) add_log_entry_header(10, p, msg, 1);
         }
         if (players1[p].server_last_stak_rx_frame_num + 100 < frame_num)
         {
            //printf("[%4d][%4d] drop p:%d \n", frame_num, players1[p].server_last_sdak_rx_frame_num, p);
            add_game_move(frame_num + 4, 1, p, 71); // make client inactive (reason no sdak for 100 frames)

            sprintf(msg,"Server dropped player:%d (last sdak rx > 100)", p);
            if (L_LOGGING_NETPLAY) add_log_entry_header(10, p, msg, 1);
         }
      }
}


void server_proc_cdat_packet(void)
{
   int p = PacketGet1ByteInt();
   int cdat_frame_num = PacketGet4ByteInt();
   int cm = PacketGet1ByteInt();
   players1[p].server_game_move_sync = cdat_frame_num - frame_num;

   add_game_move(cdat_frame_num, 5, p, cm); // add to game_move array

   sprintf(msg, "rx cdat p:%d fn:[%d] sync:[%d] gmep:[%d]\n", p, cdat_frame_num, players1[p].server_game_move_sync, game_move_entry_pos);
   if (L_LOGGING_NETPLAY_cdat) add_log_entry2(35, p, msg);
}

void server_lock_client(int p)
{
   if ((!players[p].active) && (players[p].control_method == 2)) // inactive client chasing for lock
   {
      if ((players1[p].server_sync > -2) && (players1[p].server_sync < 5)) players1[p].sync_stabilization_holdoff++;
      else players1[p].sync_stabilization_holdoff = 0;
   }
   if (players1[p].sync_stabilization_holdoff > 20) // we have been stable for over 20 frames
   {
      add_game_move(frame_num + 4, 1, p, players[p].color);
      players1[p].sync_stabilization_holdoff = 0;

      sprintf(msg,"Player:%d has locked and will become active in 4 frames!", p);
      if (L_LOGGING_NETPLAY_JOIN) add_log_entry_header(11, 0, msg, 0);
   }
}

void server_proc_stak_packet(void)
{
   int p                        = PacketGet1ByteInt();
   int ack_frame_num            = PacketGet4ByteInt();
   int client_frame_num         = PacketGet4ByteInt();
   players1[p].frames_skipped   = PacketGet4ByteInt();
   players1[p].client_chase_fps = PacketGet4ByteInt();


   players1[p].server_sync = frame_num - client_frame_num;
   server_lock_client(p);

   int sfn1 = ack_frame_num;
   int cfn = client_frame_num;
   int sfn2 = frame_num;

   players1[p].sc_sync = sfn1 - cfn; // packet stdf from s to c  sfn1-cfn
   players1[p].cs_sync = cfn - sfn2; // packet stak from c to s  cfn-sfn
   players1[p].rt_sync = sfn2 - sfn1; // round trip to server back to server through client via stdf and stak

   char tmsg1[80];
   char tmsg2[80];

   // this is used to see if client is still alive
   players1[p].server_last_stak_rx_frame_num = frame_num;


   sprintf(tmsg1, "rx stak sync:[%d] chase:[%d] ack_fn:%d cli_fn:%d", players1[p].server_sync, players1[p].client_chase_fps, ack_frame_num, client_frame_num );

   if (ack_frame_num == srv_client_state_frame_num[p][1]) // check to make sure we have a copy of acknowledged state
   {
      // acknowledged state is new base state
      memcpy(srv_client_state[p][0], srv_client_state[p][1], STATE_SIZE); // copy 1 to 0
      srv_client_state_frame_num[p][0] = ack_frame_num; // new frame_num
      sprintf(tmsg2, "set new base");
   }
   else // we don't have a copy of acknowledged state !!!
   {
      sprintf(tmsg2, "failed to set base! cl:%d ", srv_client_state_frame_num[p][1]);
      memset(srv_client_state[p][0], 0, STATE_SIZE); // reset base to all zero
      srv_client_state_frame_num[p][0] = 0;
      players1[p].client_base_resets++;
   }

   sprintf(msg, "%s %s\n", tmsg1, tmsg2);
   if (L_LOGGING_NETPLAY_stdf) add_log_entry2(30, p, msg);
}

void server_proc_cjon_packet(int who)
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
   // find empty player slot
   int cn = 99;
   for (int p=0; p<NUM_PLAYERS; p++)
      if ((players[p].active == 0) && (players[p].control_method == 0))
      {
         cn = p;
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

      Packet("sjon");    // reply with sjon
      PacketPut2ByteInt(0);
      PacketPut4ByteInt(0);
      PacketPut4ByteInt(0);
      PacketPut1ByteInt(0);
      PacketPut1ByteInt(99); // send sjon with player 99 to indicate server full
      PacketPut1ByteInt(0);
      PacketPut1ByteInt(0);
      PacketPut2ByteInt(0);
      PacketPut1ByteInt(0);
      ServerSendTo(packetbuffer, packetsize, who, 0);
   }
   else // empty player slot found, proceed with join
   {
      // try to use requested color, unless already used by another player
      while (is_player_color_used(color)) if (++color > 15) color = 1;

      init_player(cn, 1);


      set_player_start_pos(cn, 0);


      players[cn].color = color;
      players[cn].control_method = 2; //server client view only
      players1[cn].who = who;
      players1[cn].server_last_stak_rx_frame_num = frame_num + 200;
      sprintf(players1[cn].hostname, "%s", temp_name);

      add_game_move(frame_num, 3, cn, color); // add a game move type 3 to mark client started join

      Packet("sjon"); // reply with sjon
      PacketPut2ByteInt(play_level);
      PacketPut4ByteInt(frame_num);
      PacketPut4ByteInt(game_move_entry_pos);
      PacketPut1ByteInt(frame_speed);
      PacketPut1ByteInt(cn);
      PacketPut1ByteInt(color);
      PacketPut1ByteInt(deathmatch_pbullets);
      PacketPut2ByteInt(deathmatch_pbullets_damage+1000);
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
   if (L_LOGGING_NETPLAY_PLAYER_ARRAY) log_player_array2();

   if (frame_num == 0) reset_states(); // for stdf
   ServerListen();      // listen for new client connections
   int who;
   while((packetsize = ServerReceive(packetbuffer, &who)))
   {
      if(PacketRead("cdat")) server_proc_cdat_packet();
      if(PacketRead("stak")) server_proc_stak_packet();
      if(PacketRead("cjon")) server_proc_cjon_packet(who);
   }
   server_send_stdf();  // send dif states to ensure clients have same state
   server_proc_player_drop();  // check to see if we need to drop clients
   for (int p=0; p<NUM_PLAYERS; p++) if (players[p].active) process_bandwidth_counters(p);

   // check to see if we are approaching limit of game_moves array and do something if we are
   if (game_move_entry_pos > (GAME_MOVES_SIZE - 100))
   {
      sprintf(msg,"Server Approaching %d Game Moves! - Shutting Down", GAME_MOVES_SIZE);
      if (L_LOGGING_NETPLAY) add_log_entry_header(10, 0, msg, 0);

      // insert state inactive special move
      game_moves[game_move_entry_pos][0] = frame_num + 8; // add frames so server has time to sync back to clients before dropping
      game_moves[game_move_entry_pos][1] = 1;     // type 1; player state
      game_moves[game_move_entry_pos][2] = 0;     // player num
      game_moves[game_move_entry_pos][3] = 64;    // inactive
      game_move_entry_pos++;
   }

}

void server_local_control(int p)
{
   set_comp_move_from_player_key_check(p);
   if (players1[p].fake_keypress_mode) players1[p].comp_move = rand() % 64;
   if (level_done_mode == 8) start_level_done(p, 80, 800);
   if (level_done_mode == 9) start_level_done(p, 10, 40);
   if ((level_done_mode == 0) || (level_done_mode == 5))  // only allow player input in these modes
   {
      if (players1[p].comp_move != players1[p].old_comp_move) // players controls have changed
      {
         players1[p].old_comp_move = players1[p].comp_move;
         add_game_move(frame_num, 5, p, players1[p].comp_move);
      }
   }
}



