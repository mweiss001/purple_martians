// n_server.cpp
#include "pm.h"
#include "z_log.h"
#include "z_player.h"
#include "n_netgame.h"
#include "n_packet.h"
#include "mwTally.h"
#include "mwTimeStamp.h"

// n_network.h
extern int NetworkDriver;

// these are never referenced outside of this file
#define MAX_CLIENTS 32
int ClientNum = 0;
NET_CONN *ListenConn = NULL;                         // listening connection
NET_CONN *ClientConn[MAX_CLIENTS] = {NULL, };        // array of connections for each client
NET_CHANNEL *ListenChannel = NULL;                   // listen channel
NET_CHANNEL *ClientChannel[MAX_CLIENTS] = {NULL, };  // array of channels for each client



int ServerInitNetwork() // Initialize the server
{
   init_packet_buffer();
   if(NetworkInit())
   {
      sprintf(msg, "Error: failed to initialize network\n");
      printf("%s", msg);
      m_err(msg);
      if (LOG_NET) add_log_entry2(10, 0, msg);
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
         if (LOG_NET) add_log_entry2(10, 0, msg);
         return -1;
      }
      if(net_listen(ListenConn))
      {
         sprintf(msg, "Error: cannot listen\n");
         printf("%s", msg);
         m_err(msg);
         if (LOG_NET) add_log_entry2(10, 0, msg);
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
         if (LOG_NET) add_log_entry2(10, 0, msg);
         return -1;
      }
      if (net_assigntarget(ListenChannel, ""))
      {
         sprintf(msg, "Error: failed to assign target to listening channel\n");
         printf("%s", msg);
         m_err(msg);
         if (LOG_NET) add_log_entry2(10, 0, msg);
         net_closechannel(ListenChannel);
         return -1;
      }
      sprintf(msg, "Network initialized - channel mode (UDP)");
      // printf("Local address of channel%s\n", net_getlocaladdress (ListenChannel));
   }
   printf("%s\n", msg);
   if (LOG_NET) add_log_entry_position_text(10, 0, 76, 10, msg, "|", " ");
   return 0;
}


void ServerListen()
{
   if (TCP) // listen for connections
   {
      NET_CONN *newconn;
      newconn = net_poll_listen (ListenConn);
      if(newconn == NULL) return;
      else
      {
         ClientConn[ClientNum] = newconn;
         ClientNum++;
         sprintf(msg, "Connection received from %s", net_getpeer (newconn));
         printf("%s\n", msg);

         if (LOG_NET)
         {
            add_log_entry_centered_text(10, 0, 76, "", "+", "-");
            add_log_entry_position_text(10, 0, 76, 10, msg, "|", " ");
            add_log_entry_centered_text(10, 0, 76, "", "+", "-");
         }
      }
   }
   else // UDP - listen for channels
   {
      char address[32];
      packetsize = net_receive(ListenChannel, packetbuffer, 1024, address);
      if (packetsize)
      {
         set_packetpos(0);
         if (PacketRead("1234"))
         {
            sprintf(msg, "Server received initial 1234 packet from '%s'",address);
            // printf("%s\n", msg);

            if (!(ClientChannel[ClientNum] = net_openchannel(NetworkDriver, NULL)))
            {
               sprintf(msg, "Error: failed to open channel for %s\n", address);
               printf("%s", msg);
               if (LOG_NET) add_log_entry2(10, 0, msg);
               return;
            }
            if (net_assigntarget (ClientChannel[ClientNum], address))
            {
               sprintf(msg, "Error: couldn't assign target `%s' to channel\n", address);
               printf("%s", msg);
               if (LOG_NET)  add_log_entry2(10, 0, msg);
               net_closechannel (ClientChannel[ClientNum]);
               return;
            }
            sprintf(msg, "Server opened channel for `%s' and sent reply", address);
            printf("%s\n", msg);
            if (LOG_NET)
            {
               add_log_entry_centered_text(10, 0, 76, "", "+", "-");
               add_log_entry_position_text(10, 0, 76, 10, msg, "|", " ");
               add_log_entry_centered_text(10, 0, 76, "", "+", "-");
            }
            Packet("5678");
            ServerSendTo(packetbuffer, packetsize, ClientNum, 0);
            ClientNum++;
         }
      }
   }
}

// Receive data from a client, and store in provided array
// (must have room for 1024 bytes). Returns the size of the stored data
int ServerReceive(void *data, int *sender)
{
   if (TCP)
   {
      for (int n=0; n<ClientNum; n++)
      {
   	   if (ClientConn[n])
         {
   		   int len = net_receive_rdm(ClientConn[n], data, 1024);
   			if (len > 0)
            {
               // add to server's counts
               players1[0].rx_current_bytes_for_this_frame += len;
               players1[0].rx_current_packets_for_this_frame++;
               // use n (who) to get player number
               for (int p=0; p<NUM_PLAYERS; p++)
                  if ((players[p].active) && (players1[p].who == n))
                  {
                     // add to client's counts
                     players1[p].rx_current_bytes_for_this_frame += len;
                     players1[p].rx_current_packets_for_this_frame++;
                  }
   				if (sender) *sender = n;
   				return len;
   			}
   		}
      }
   } // end of if TCP
   else // UDP
   {
      for (int n=0; n<ClientNum; n++)
      {
   		if (ClientChannel[n])
         {
            int len = net_receive(ClientChannel[n], data, 1024, NULL);
   			if (len > 0)
            {
               // add to server's counts
               players1[0].rx_current_bytes_for_this_frame += len;
               players1[0].rx_current_packets_for_this_frame++;
               // use n (who) to get player number
               for (int p=0; p<NUM_PLAYERS; p++)
                  if ((players[p].active) && (players1[p].who == n))
                  {
                     // add to client's counts
                     players1[p].rx_current_bytes_for_this_frame += len;
                     players1[p].rx_current_packets_for_this_frame++;
                  }
   				if (sender) *sender = n;
   				return len;
   			}
   		}
      }
   } // end of UDP
	return 0;
}

void ServerBroadcast(void *data, int len)
{
   for (int n=0; n<ClientNum; n++)
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
   // add to server's counts
   players1[0].tx_current_bytes_for_this_frame += len;
   players1[0].tx_current_packets_for_this_frame++;
   // add to client's counts
   players1[player].tx_current_bytes_for_this_frame += len;
   players1[player].tx_current_packets_for_this_frame++;
}
void server_flush(void)
{
   int who;
   while (ServerReceive(packetbuffer, &who));
}

void ServerExitNetwork() // Shut the server down
{
   sprintf(msg, "Shutting down the server");
   printf("%s\n", msg);
   if (LOG_NET) add_log_entry_header(10, 0, msg, 1);
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
   if (LOG_NET)
   {
      log_versions();
      add_log_entry_centered_text(10, 0, 76, "", "+", "-");

      sprintf(msg, "Server mode started");
      add_log_entry_position_text(10, 0, 76, 10, msg, "|", " ");
      printf("\n%s\n", msg);

      sprintf(msg, "Server hostname:    [%s]", local_hostname);
      add_log_entry_position_text(10, 0, 76, 10, msg, "|", " ");
      printf("%s\n", msg);

      sprintf(msg, "Level:              [%d]", play_level);
      add_log_entry_position_text(10, 0, 76, 10, msg, "|", " ");
      printf("%s\n", msg);
   }

   if (ServerInitNetwork())
   {
      sprintf(msg, "Could not find internet driver!");
      printf("\n%s\n\n", msg);
      m_err(msg);
      if (LOG_NET) add_log_entry2(10, 0, msg);
      return 0;
   }

   sprintf(msg, "Server successfully initialized");
   printf("%s\n\n", msg);
   if (LOG_NET)
   {
      add_log_entry_position_text(10, 0, 76, 10, msg, "|", " ");
      add_log_entry_centered_text(10, 0, 76, "", "+", "-");
   }

   // still needed or client dies at joining
   Packet("JUNK");
   ServerBroadcast(packetbuffer, packetsize);

   ima_server = 1;
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





void server_rewind(void)
{
   int s1 = players1[0].server_state_freq;

   if (frame_num >= srv_client_state_frame_num[0][1] + s1)    // is it time to create a new state?
   {
      int ff = frame_num - srv_client_state_frame_num[0][1];  // almost always equals s1, unless s1 has changed

      // rewind and fast forward from last state to apply late client input

      if (LOG_NET_stdf)
      {
         sprintf(msg, "stdf rewind to:%d\n", srv_client_state_frame_num[0][1]);
         //printf(msg);
         add_log_entry2(27, 0, msg);
      }

      if (LOG_TMR_rwnd) t0 = al_get_time();

      state_to_game_vars(srv_client_state[0][1]);   // apply rewind state
      frame_num = srv_client_state_frame_num[0][1]; // set rewind frame num

      loop_frame(ff);

      if (LOG_TMR_rwnd) add_log_TMR(al_get_time() - t0, "rwnd", 0);

      players1[0].server_send_dif = 1;
   }
}

void server_create_new_state(void)
{
   if (players1[0].server_send_dif)
   {
      players1[0].server_send_dif = 0;

      // save state as a base for next rewind
      game_vars_to_state(srv_client_state[0][1]);
      srv_client_state_frame_num[0][1] = frame_num+1;

      if (LOG_NET_stdf)
      {
         sprintf(msg, "stdf saved server state[1]:%d\n", frame_num);
         //printf(msg);
         add_log_entry2(27, 0, msg);
      }

      // send to all clients
      for (int p=1; p<NUM_PLAYERS; p++)
         if ((players[p].control_method == 2) || (players[p].control_method == 8)) server_send_dif(p);
   }
}


void server_send_dif(int p) // send dif to a specific client
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

   if (LOG_NET_stdf)
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

      if (LOG_NET_stdf_all_packets)
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
      PacketPut1ByteInt(players1[p].late_cdats_last_sec);
      memcpy(packetbuffer+packetsize, cmp+start_byte, packet_data_size);
      packetsize += packet_data_size;
      ServerSendTo(packetbuffer, packetsize, players1[p].who, p);
      start_byte+=1000;
   }
}




void server_proc_player_drop(void)
{
   // check to see if we need to drop clients
   for (int p=1; p<NUM_PLAYERS; p++)   // server only; skip p[0]
      if (players[p].control_method == 2)
      {


         if (players1[p].server_last_stak_rx_frame_num + 100 < frame_num)
         {
            //printf("[%4d][%4d] drop p:%d \n", frame_num, players1[p].server_last_stak_rx_frame_num, p);
            add_game_move(frame_num + 4, 2, p, 71); // make client inactive (reason no stak for 100 frames)

            sprintf(msg,"Server dropped player:%d (last stak rx > 100)", p);
            if (LOG_NET) add_log_entry_header(10, p, msg, 1);
         }
      }

   // check to see if we are approaching limit of game_moves array and do something if we are
   if (game_move_entry_pos > (GAME_MOVES_SIZE - 100))
   {
      sprintf(msg,"Server Approaching %d Game Moves! - Shutting Down", GAME_MOVES_SIZE);
      if (LOG_NET) add_log_entry_header(10, 0, msg, 0);

      // insert state inactive special move
      add_game_move2(frame_num + 8, 2, 0, 64); // type 2 - player inactive
   }
}


void server_proc_cdat_packet(double timestamp)
{
   int p = PacketGet1ByteInt();
   int cdat_frame_num = PacketGet4ByteInt();
   int cm = PacketGet1ByteInt();

   players1[p].client_cdat_packets_tx++;

   // calculate game_move_sync
   players1[p].server_game_move_sync = cdat_frame_num - frame_num;

   // calculate game_move_dsync
   players1[p].game_move_dsync = ( (double) players1[p].server_game_move_sync * 0.025) + mwTS.timestamp_frame_start - timestamp;

   mwT_game_move_dsync_avg_last_sec[p].add_data(players1[p].game_move_dsync); // add to average tally


   // check to see if earlier than the last stdf state
   if (cdat_frame_num < srv_client_state_frame_num[0][1])
   {
      players1[p].late_cdats++;
      mwT_late_cdats_last_sec[p].add_data(1); // add to tally
      sprintf(msg, "rx cdat p:%d fn:[%d] sync:[%d] late - droppped\n", p, cdat_frame_num, players1[p].server_game_move_sync);
   }
   else
   {
      add_game_move(cdat_frame_num, 5, p, cm); // add to game_move array
      sprintf(msg, "rx cdat p:%d fn:[%d] sync:[%d] gmep:[%d] - entered\n", p, cdat_frame_num, players1[p].server_game_move_sync, game_move_entry_pos);
   }
   if (LOG_NET_cdat) add_log_entry2(35, p, msg);
}

void server_lock_client(int p)
{
   if ((!players[p].active) && (players[p].control_method == 2)) // inactive client chasing for lock
   {
      if ((players1[p].dsync > -0.2) && (players1[p].dsync < 0.03)) players1[p].sync_stabilization_holdoff++; // -200 to +30
      else players1[p].sync_stabilization_holdoff = 0;
   }
   if (players1[p].sync_stabilization_holdoff > 20) // we have been stable for over 20 frames
   {
      add_game_move(frame_num + 4, 1, p, players[p].color);
      players1[p].sync_stabilization_holdoff = 0;

      sprintf(msg,"Player:%d has locked and will become active in 4 frames!", p);
      if (LOG_NET_join) add_log_entry_header(11, 0, msg, 0);
   }
}

void server_proc_stak_packet(double timestamp)
{
   int p                                    = PacketGet1ByteInt();
   int ack_frame_num                        = PacketGet4ByteInt();
   int client_frame_num                     = PacketGet4ByteInt();
   players1[p].client_chase_fps             = PacketGetDouble();
   players1[p].dsync                        = PacketGetDouble();





   // calculate stak_sync
   int stak_sync = frame_num - srv_client_state_frame_num[0][1];

   // calculate stak_dsync
   players1[p].stak_dsync = ( (double) stak_sync * 0.025) + mwTS.timestamp_frame_start - timestamp;

   server_lock_client(p);

   char tmsg1[80];
   char tmsg2[80];

   // this is used to see if client is still alive
   players1[p].server_last_stak_rx_frame_num = frame_num;

   sprintf(tmsg1, "rx stak d[%4.1f] c[%4.1f] a:%d c:%d", players1[p].dsync*1000, players1[p].client_chase_fps, ack_frame_num, client_frame_num );

   if (ack_frame_num == srv_client_state_frame_num[p][1]) // check to make sure we have a copy of acknowledged state
   {
      // acknowledged state is new base state
      memcpy(srv_client_state[p][0], srv_client_state[p][1], STATE_SIZE);  // copy 1 to 0
      srv_client_state_frame_num[p][0] = srv_client_state_frame_num[p][1];
      sprintf(tmsg2, "set new base");
   }
   else // we don't have a copy of acknowledged state !!!
   {
      sprintf(tmsg2, "fail base set:%d", srv_client_state_frame_num[p][1]);
      memset(srv_client_state[p][0], 0, STATE_SIZE); // reset base to all zero
      srv_client_state_frame_num[p][0] = 0;
      players1[p].client_base_resets++;
   }

   sprintf(msg, "%s %s\n", tmsg1, tmsg2);
   if (LOG_NET_server_rx_stak) add_log_entry2(33, p, msg);
}


void server_proc_cjon_packet(int who)
{
   char temp_name[16];
   int color = PacketGet1ByteInt();
   PacketReadString(temp_name);

   if (LOG_NET_join)
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
      if (LOG_NET_join)
      {
         sprintf(msg, "Reply sent: 'SERVER FULL'");
         add_log_entry_position_text(11, 0, 76, 10, msg, "|", " ");
         add_log_entry_centered_text(11, 0, 76, "", "+", "-");
      }

      Packet("sjon");    // reply with sjon
      PacketPut2ByteInt(0);
      PacketPut4ByteInt(0);
      PacketPut1ByteInt(0);
      PacketPut1ByteInt(99); // send sjon with player 99 to indicate server full
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
      PacketPut1ByteInt(cn);
      PacketPut1ByteInt(color);
      PacketPut1ByteInt(deathmatch_pbullets);
      PacketPut2ByteInt(deathmatch_pbullets_damage+1000);
      PacketPut1ByteInt(suicide_pbullets);
      ServerSendTo(packetbuffer, packetsize, who, cn);

      if (LOG_NET_join)
      {
         sprintf(msg,"Server replied with join invitation:");
         add_log_entry_position_text(11, 0, 76, 10, msg, "|", " ");
         sprintf(msg,"Level:[%d]", play_level);
         add_log_entry_position_text(11, 0, 76, 10, msg, "|", " ");
         sprintf(msg,"Player Number:[%d]", cn);
         add_log_entry_position_text(11, 0, 76, 10, msg, "|", " ");
         sprintf(msg,"Player Color:[%d]", color);
         add_log_entry_position_text(11, 0, 76, 10, msg, "|", " ");
         sprintf(msg,"Server frame_num:[%d]", frame_num);
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


int get_player_num_from_who(int who)
{
   for(int p=0; p<NUM_PLAYERS; p++)
      if (players1[p].who == who) return p;
   return -1;
}

void server_fast_packet_loop(void)
{
   int who;
   while((packetsize = ServerReceive(packetbuffer, &who)))
   {
      //printf("got packet - size:%d\n", packetsize);
      double timestamp = al_get_time();
      if (PacketRead("ping"))
      {
         int p = get_player_num_from_who(who);
         if (p != -1)
         {
            //printf("rx ping from p:%d - tx pong\n", p);
            double t0 = PacketGetDouble();
            double t1 = al_get_time();
            Packet("pong");
            PacketPutDouble(t0);
            PacketPutDouble(t1);
            ServerSendTo(packetbuffer, packetsize, players1[p].who, p);
         }
      }

      if (PacketRead("pang"))
      {
         int p = get_player_num_from_who(who);
         if (p != -1)
         {
            double t0 = PacketGetDouble();
            double t1 = al_get_time();
            players1[p].ping = t1 - t0;
            //printf("rx pang from p:%d [%3.1f ms]\n", p, players1[p].ping*1000);
            mwT[4].add_data(players1[p].ping); // add to max tally
//            if (players1[p].ping > players1[0].server_max_client_ping) players1[0].server_max_client_ping = players1[p].ping;
         }
      }

      int type = 0;
      if(PacketRead("cdat")) type = 1;
      if(PacketRead("stak")) type = 2;
      if(PacketRead("cjon")) type = 3;
      if (type)
      {
         for (int i=0; i<200; i++)
            if (!packet_buffers[i].active) // find empty
            {
               //printf("%d stored packet:%d size:%d type:%d\n", frame_num, i, packetsize, type);
               packet_buffers[i].active = 1;
               packet_buffers[i].type = type;
               packet_buffers[i].timestamp = timestamp;
               packet_buffers[i].who = who;
               packet_buffers[i].packetsize = packetsize;
               memcpy(packet_buffers[i].data, packetbuffer, 1024);
               break;
            }
      }
   }
}

void server_read_packet_buffer(void)
{
   for (int i=0; i<200; i++)
      if (packet_buffers[i].active)
      {
         //printf("%d read packet:%d  size:%d \n", frame_num, i, packet_buffers[i].packetsize);

         memcpy(packetbuffer, packet_buffers[i].data, 1024);
         packetsize = packet_buffers[i].packetsize;

         set_packetpos(4);

         if (packet_buffers[i].type == 1) server_proc_cdat_packet(packet_buffers[i].timestamp);
         if (packet_buffers[i].type == 2) server_proc_stak_packet(packet_buffers[i].timestamp);
         if (packet_buffers[i].type == 3) server_proc_cjon_packet(packet_buffers[i].who);

         packet_buffers[i].active = 0;
      }
}

void server_control()
{
   ServerListen();               // listen for new client connections
   server_read_packet_buffer();
   server_rewind();              // to replay and apply late client input
   server_proc_player_drop();    // check to see if we need to drop clients
   if (LOG_NET_player_array) log_player_array2();
   for (int p=0; p<NUM_PLAYERS; p++) if (players[p].active) process_bandwidth_counters(p);
}
