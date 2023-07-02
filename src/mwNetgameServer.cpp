// mwNetgameServer.cpp

#include "pm.h"
#include "mwNetgame.h"
#include "mwLog.h"
#include "mwPlayer.h"
#include "mwTally.h"
#include "mwTimeStamp.h"
#include "mwGameMoves.h"
#include "mwLevel.h"
#include "mwLoop.h"
#include "mwShot.h"
#include "mwInput.h"

int mwNetgame::ServerInitNetwork() // Initialize the server
{
   char msg[1024];
   init_packet_buffer();
   if(NetworkInit())
   {
      sprintf(msg, "Error: failed to initialize network\n");
      printf("%s", msg);
      mInput.m_err(msg);
      if (mLog.LOG_NET) mLog.add_log_entry2(10, 0, msg);
      return -1;
   }
   if (TCP)
   {
      ListenConn = net_openconn(NetworkDriver, "");
      if(!ListenConn)
      {
         sprintf(msg, "Error: failed to open listening connection\n");
         printf("%s", msg);
         mInput.m_err(msg);
         if (mLog.LOG_NET) mLog.add_log_entry2(10, 0, msg);
         return -1;
      }
      if(net_listen(ListenConn))
      {
         sprintf(msg, "Error: cannot listen\n");
         printf("%s", msg);
         mInput.m_err(msg);
         if (mLog.LOG_NET) mLog.add_log_entry2(10, 0, msg);
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
         mInput.m_err(msg);
         if (mLog.LOG_NET) mLog.add_log_entry2(10, 0, msg);
         return -1;
      }
      if (net_assigntarget(ListenChannel, ""))
      {
         sprintf(msg, "Error: failed to assign target to listening channel\n");
         printf("%s", msg);
         mInput.m_err(msg);
         if (mLog.LOG_NET) mLog.add_log_entry2(10, 0, msg);
         net_closechannel(ListenChannel);
         return -1;
      }
      sprintf(msg, "Network initialized - channel mode (UDP)");
      // printf("Local address of channel%s\n", net_getlocaladdress (ListenChannel));
   }
   printf("%s\n", msg);
   if (mLog.LOG_NET) mLog.add_log_entry_position_text(10, 0, 76, 10, msg, "|", " ");
   return 0;
}


void mwNetgame::ServerListen()
{
   char msg[1024];
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

         if (mLog.LOG_NET)
         {
            mLog.add_log_entry_centered_text(10, 0, 76, "", "+", "-");
            mLog.add_log_entry_position_text(10, 0, 76, 10, msg, "|", " ");
            mLog.add_log_entry_centered_text(10, 0, 76, "", "+", "-");
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
               if (mLog.LOG_NET) mLog.add_log_entry2(10, 0, msg);
               return;
            }
            if (net_assigntarget (ClientChannel[ClientNum], address))
            {
               sprintf(msg, "Error: couldn't assign target `%s' to channel\n", address);
               printf("%s", msg);
               if (mLog.LOG_NET) mLog.add_log_entry2(10, 0, msg);
               net_closechannel (ClientChannel[ClientNum]);
               return;
            }
            sprintf(msg, "Server opened channel for `%s' and sent reply", address);
            printf("%s\n", msg);
            if (mLog.LOG_NET)
            {
               mLog.add_log_entry_centered_text(10, 0, 76, "", "+", "-");
               mLog.add_log_entry_position_text(10, 0, 76, 10, msg, "|", " ");
               mLog.add_log_entry_centered_text(10, 0, 76, "", "+", "-");
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
int mwNetgame::ServerReceive(void *data, int *sender)
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
               mPlayer.loc[0].rx_current_bytes_for_this_frame += len;
               mPlayer.loc[0].rx_current_packets_for_this_frame++;
               // use n (who) to get player number
               for (int p=0; p<NUM_PLAYERS; p++)
                  if ((mPlayer.syn[p].active) && (mPlayer.loc[p].who == n))
                  {
                     // add to client's counts
                     mPlayer.loc[p].rx_current_bytes_for_this_frame += len;
                     mPlayer.loc[p].rx_current_packets_for_this_frame++;
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
               mPlayer.loc[0].rx_current_bytes_for_this_frame += len;
               mPlayer.loc[0].rx_current_packets_for_this_frame++;
               // use n (who) to get player number
               for (int p=0; p<NUM_PLAYERS; p++)
                  if ((mPlayer.syn[p].active) && (mPlayer.loc[p].who == n))
                  {
                     // add to client's counts
                     mPlayer.loc[p].rx_current_bytes_for_this_frame += len;
                     mPlayer.loc[p].rx_current_packets_for_this_frame++;
                  }
   				if (sender) *sender = n;
   				return len;
   			}
   		}
      }
   } // end of UDP
	return 0;
}

void mwNetgame::ServerBroadcast(void *data, int len)
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
void mwNetgame::ServerSendTo(void *data, int len, int who, int player)
{
   if (TCP) net_send_rdm(ClientConn[who], data, len);
   else     net_send(ClientChannel[who], data, len);
   // add to server's counts
   mPlayer.loc[0].tx_current_bytes_for_this_frame += len;
   mPlayer.loc[0].tx_current_packets_for_this_frame++;
   // add to client's counts
   mPlayer.loc[player].tx_current_bytes_for_this_frame += len;
   mPlayer.loc[player].tx_current_packets_for_this_frame++;
}
void mwNetgame::server_flush(void)
{
   int who;
   while (ServerReceive(packetbuffer, &who));
}

void mwNetgame::ServerExitNetwork() // Shut the server down
{
   char msg[1024];
   sprintf(msg, "Shutting down the server");
   printf("%s\n", msg);
   if (mLog.LOG_NET) mLog.add_log_entry_header(10, 0, msg, 1);
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






int mwNetgame::server_init(void)
{
   char msg[1024];
   if (mLog.LOG_NET)
   {
      mLog.log_versions();
      mLog.add_log_entry_centered_text(10, 0, 76, "", "+", "-");

      sprintf(msg, "Server mode started");
      mLog.add_log_entry_position_text(10, 0, 76, 10, msg, "|", " ");
      printf("\n%s\n", msg);

      sprintf(msg, "Server hostname:    [%s]", mLoop.local_hostname);
      mLog.add_log_entry_position_text(10, 0, 76, 10, msg, "|", " ");
      printf("%s\n", msg);

      sprintf(msg, "Level:              [%d]", mLevel.play_level);
      mLog.add_log_entry_position_text(10, 0, 76, 10, msg, "|", " ");
      printf("%s\n", msg);
   }

   if (ServerInitNetwork())
   {
      sprintf(msg, "Could not find internet driver!");
      printf("\n%s\n\n", msg);
      mInput.m_err(msg);
      if (mLog.LOG_NET) mLog.add_log_entry2(10, 0, msg);
      return 0;
   }

   sprintf(msg, "Server successfully initialized");
   printf("%s\n\n", msg);
   if (mLog.LOG_NET)
   {
      mLog.add_log_entry_position_text(10, 0, 76, 10, msg, "|", " ");
      mLog.add_log_entry_centered_text(10, 0, 76, "", "+", "-");
   }

   // still needed or client dies at joining
   Packet("JUNK");
   ServerBroadcast(packetbuffer, packetsize);

   ima_server = 1;
   return 1;
}

void mwNetgame::server_exit(void)
{
   ServerExitNetwork(); // Shut the server down
   ima_server = 0;

   // reset player data
   for (int p=0; p<NUM_PLAYERS; p++) mPlayer.init_player(p, 1);
   mPlayer.syn[0].active = 1;
}





void mwNetgame::server_rewind(void)
{
   char msg[1024];
   int s1 = mPlayer.loc[0].server_state_freq;

   if (mLoop.frame_num >= srv_client_state_frame_num[0][1] + s1)    // is it time to create a new state?
   {
      int ff = mLoop.frame_num - srv_client_state_frame_num[0][1];  // almost always equals s1, unless s1 has changed

      // rewind and fast forward from last state to apply late client input

      if (mLog.LOG_NET_stdf)
      {
         sprintf(msg, "stdf rewind to:%d\n", srv_client_state_frame_num[0][1]);
         //printf(msg);
         mLog.add_log_entry2(27, 0, msg);
      }

      double t0 = al_get_time();

      state_to_game_vars(srv_client_state[0][1]);   // apply rewind state
      mLoop.frame_num = srv_client_state_frame_num[0][1]; // set rewind frame num

      mLoop.loop_frame(ff);

      if (mLog.LOG_TMR_rwnd) mLog.add_log_TMR(al_get_time() - t0, "rwnd", 0);

      mPlayer.loc[0].server_send_dif = 1;
   }
}

void mwNetgame::server_create_new_state(void)
{
   char msg[1024];
   if (mPlayer.loc[0].server_send_dif)
   {
      mPlayer.loc[0].server_send_dif = 0;

      // save state as a base for next rewind
      game_vars_to_state(srv_client_state[0][1]);
      srv_client_state_frame_num[0][1] = mLoop.frame_num+1;

      if (mLog.LOG_NET_stdf)
      {
         sprintf(msg, "stdf saved server state[1]:%d\n", mLoop.frame_num);
         //printf(msg);
         mLog.add_log_entry2(27, 0, msg);
      }

      // send to all clients
      for (int p=1; p<NUM_PLAYERS; p++)
         if ((mPlayer.syn[p].control_method == 2) || (mPlayer.syn[p].control_method == 8)) server_send_dif(p);
   }
}


void mwNetgame::server_send_dif(int p) // send dif to a specific client
{
   char msg[1024];
   // if last_ack_state_frame == 0 set base to all zeros
   if (srv_client_state_frame_num[p][0] == 0) memset(srv_client_state[p][0], 0, STATE_SIZE);

   char dif[STATE_SIZE];
   char cmp[STATE_SIZE];

   // put current state in client's state slot 1
   game_vars_to_state(srv_client_state[p][1]);

   // put current mLoop.frame_num
   srv_client_state_frame_num[p][1] = mLoop.frame_num;

   // make a new dif from base and current
   get_state_dif(srv_client_state[p][0], srv_client_state[p][1], dif, STATE_SIZE);

   // compress dif to cmp
   uLongf destLen= sizeof(cmp);
   compress2((Bytef*)cmp, (uLongf*)&destLen, (Bytef*)dif, sizeof(dif), zlib_cmp);
   int cmp_size = destLen;

   // break compressed dif into smaller pieces
   int num_packets = (cmp_size / 1000) + 1;

   mPlayer.loc[p].cmp_dif_size = cmp_size;
   mPlayer.loc[p].num_dif_packets = num_packets;

   if (mLog.LOG_NET_stdf)
   {
      float cr = (float)cmp_size*100 / (float)STATE_SIZE; // compression ratio
      sprintf(msg, "tx stdf p:%d [src:%d dst:%d] cmp:%d ratio:%3.2f [%d packets needed]\n",
                            p, srv_client_state_frame_num[p][0], srv_client_state_frame_num[p][1], cmp_size, cr, num_packets);
      mLog.add_log_entry2(27, p, msg);
   }
   int start_byte = 0;
   for (int packet_num=0; packet_num < num_packets; packet_num++)
   {
      int packet_data_size = 1000; // default size
      if (start_byte + packet_data_size > cmp_size) packet_data_size = cmp_size - start_byte; // last piece is smaller

      if (mLog.LOG_NET_stdf_all_packets)
      {
         sprintf(msg, "tx stdf piece [%d of %d] [%d to %d] st:%4d sz:%4d\n",
                       packet_num+1, num_packets, srv_client_state_frame_num[p][0], srv_client_state_frame_num[p][1], start_byte, packet_data_size);
         mLog.add_log_entry2(28, p, msg);
      }

      Packet("stdf");
      PacketPut4ByteInt(srv_client_state_frame_num[p][0]); // src mLoop.frame_num
      PacketPut4ByteInt(srv_client_state_frame_num[p][1]); // dst mLoop.frame_num
      PacketPut1ByteInt(packet_num);
      PacketPut1ByteInt(num_packets);
      PacketPut4ByteInt(start_byte);
      PacketPut4ByteInt(packet_data_size);
      PacketPut1ByteInt(mPlayer.loc[p].late_cdats_last_sec);
      memcpy(packetbuffer+packetsize, cmp+start_byte, packet_data_size);
      packetsize += packet_data_size;
      ServerSendTo(packetbuffer, packetsize, mPlayer.loc[p].who, p);
      start_byte+=1000;
   }
}




void mwNetgame::server_proc_player_drop(void)
{
   char msg[1024];
   // check to see if we need to drop clients
   for (int p=1; p<NUM_PLAYERS; p++)   // server only; skip p[0]
      if (mPlayer.syn[p].control_method == 2)
      {


         if (mPlayer.loc[p].server_last_stak_rx_frame_num + 100 < mLoop.frame_num)
         {
            //printf("[%4d][%4d] drop p:%d \n", mLoop.frame_num, mPlayer.loc[p].server_last_stak_rx_frame_num, p);
            mGameMoves.add_game_move(mLoop.frame_num + 4, 2, p, 71); // make client inactive (reason no stak for 100 frames)

            sprintf(msg,"Server dropped player:%d (last stak rx > 100)", p);
            if (mLog.LOG_NET) mLog.add_log_entry_header(10, p, msg, 1);
         }
      }

//   // check to see if we are approaching limit of game_moves array and do something if we are
//   if (mGameMoves.entry_pos > (GAME_MOVES_SIZE - 100))
//   {
//      sprintf(msg,"Server Approaching %d Game Moves! - Shutting Down", GAME_MOVES_SIZE);
//      if (mLog.LOG_NET) mLog.add_log_entry_header(10, 0, msg, 0);
//
//      // insert state inactive special move
//      mGameMoves.add_game_move2(mLoop.frame_num + 8, 2, 0, 64); // type 2 - player inactive
//   }


   int reload = 0;
   int gm_limit = GAME_MOVES_SIZE - 100;
   int tm_limit = 60 * 60 * 40; // 1H

//   int gm_limit = 20;
//   int tm_limit = 30 * 40;

   if (mGameMoves.entry_pos > gm_limit)
   {
      sprintf(msg,"Server Approaching %d Game Moves! - Reload", GAME_MOVES_SIZE);
      if (mLog.LOG_NET) mLog.add_log_entry_header(10, 0, msg, 0);
      reload = 1;
   }


   if (mLoop.frame_num > tm_limit)
   {
      sprintf(msg,"Server Approaching %d frames! - Reload", tm_limit);
      if (mLog.LOG_NET) mLog.add_log_entry_header(10, 0, msg, 0);
      reload = 1;
   }

   if (reload)
   {
      if (mPlayer.syn[0].level_done_mode == 0)
      {
         mPlayer.syn[0].level_done_mode = 3;
         mPlayer.syn[0].level_done_timer = 0;
         mPlayer.syn[0].level_done_next_level = 1;
         mLoop.quit_action = 1;
         mLoop.done_action = 1;
      }
   }

}


void mwNetgame::server_proc_cdat_packet(double timestamp)
{
   char msg[1024];
   int p = PacketGet1ByteInt();
   int cdat_frame_num = PacketGet4ByteInt();
   int cm = PacketGet1ByteInt();

   mPlayer.loc[p].client_cdat_packets_tx++;

   // calculate game_move_sync
   mPlayer.loc[p].server_game_move_sync = cdat_frame_num - mLoop.frame_num;

   // calculate game_move_dsync
   mPlayer.loc[p].game_move_dsync = ( (double) mPlayer.loc[p].server_game_move_sync * 0.025) + mTimeStamp.timestamp_frame_start - timestamp;

   mTally_game_move_dsync_avg_last_sec[p].add_data(mPlayer.loc[p].game_move_dsync); // add to average tally


   // check to see if earlier than the last stdf state
   if (cdat_frame_num < srv_client_state_frame_num[0][1])
   {
      mPlayer.loc[p].late_cdats++;
      mTally_late_cdats_last_sec[p].add_data(1); // add to tally
      sprintf(msg, "rx cdat p:%d fn:[%d] sync:[%d] late - droppped\n", p, cdat_frame_num, mPlayer.loc[p].server_game_move_sync);
   }
   else
   {
      mGameMoves.add_game_move(cdat_frame_num, 5, p, cm); // add to game_move array
      sprintf(msg, "rx cdat p:%d fn:[%d] sync:[%d] gmep:[%d] - entered\n", p, cdat_frame_num, mPlayer.loc[p].server_game_move_sync, mGameMoves.entry_pos);
   }
   if (mLog.LOG_NET_cdat) mLog.add_log_entry2(35, p, msg);
}

void mwNetgame::server_lock_client(int p)
{
   char msg[1024];
   if ((!mPlayer.syn[p].active) && (mPlayer.syn[p].control_method == 2)) // inactive client chasing for lock
   {
      if ((mPlayer.loc[p].dsync > -0.2) && (mPlayer.loc[p].dsync < 0.03)) mPlayer.loc[p].sync_stabilization_holdoff++; // -200 to +30
      else mPlayer.loc[p].sync_stabilization_holdoff = 0;
   }
   if (mPlayer.loc[p].sync_stabilization_holdoff > 20) // we have been stable for over 20 frames
   {
      mGameMoves.add_game_move(mLoop.frame_num + 4, 1, p, mPlayer.syn[p].color);
      mPlayer.loc[p].sync_stabilization_holdoff = 0;

      sprintf(msg,"Player:%d has locked and will become active in 4 frames!", p);
      if (mLog.LOG_NET_join) mLog.add_log_entry_header(11, 0, msg, 0);
   }
}

void mwNetgame::server_proc_stak_packet(double timestamp)
{
   char msg[1024];
   int p                                    = PacketGet1ByteInt();
   int ack_frame_num                        = PacketGet4ByteInt();
   int client_frame_num                     = PacketGet4ByteInt();
   mPlayer.loc[p].client_chase_fps             = PacketGetDouble();
   mPlayer.loc[p].dsync                        = PacketGetDouble();





   // calculate stak_sync
   int stak_sync = mLoop.frame_num - srv_client_state_frame_num[0][1];

   // calculate stak_dsync
   mPlayer.loc[p].stak_dsync = ( (double) stak_sync * 0.025) + mTimeStamp.timestamp_frame_start - timestamp;

   server_lock_client(p);

   char tmsg1[80];
   char tmsg2[80];

   // this is used to see if client is still alive
   mPlayer.loc[p].server_last_stak_rx_frame_num = mLoop.frame_num;

   sprintf(tmsg1, "rx stak d[%4.1f] c[%4.1f] a:%d c:%d", mPlayer.loc[p].dsync*1000, mPlayer.loc[p].client_chase_fps, ack_frame_num, client_frame_num );

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
      mPlayer.loc[p].client_base_resets++;
   }

   sprintf(msg, "%s %s\n", tmsg1, tmsg2);
   if (mLog.LOG_NET_server_rx_stak) mLog.add_log_entry2(33, p, msg);
}


void mwNetgame::server_proc_cjon_packet(int who)
{
   char msg[1024];
   char temp_name[16];
   int color = PacketGet1ByteInt();
   PacketReadString(temp_name);

   if (mLog.LOG_NET_join)
   {
      sprintf(msg,"Server received join request from %s requesting color:%d", temp_name, color);
      mLog.add_log_entry_centered_text(11, 0, 76, "", "+", "-");
      mLog.add_log_entry_position_text(11, 0, 76, 10, msg, "|", " ");
   }
   // find empty player slot
   int cn = 99;
   for (int p=0; p<NUM_PLAYERS; p++)
      if ((mPlayer.syn[p].active == 0) && (mPlayer.syn[p].control_method == 0))
      {
         cn = p;
         break;
      }
   if (cn == 99) // no empty player slots found
   {
      if (mLog.LOG_NET_join)
      {
         sprintf(msg, "Reply sent: 'SERVER FULL'");
         mLog.add_log_entry_position_text(11, 0, 76, 10, msg, "|", " ");
         mLog.add_log_entry_centered_text(11, 0, 76, "", "+", "-");
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
      while (mPlayer.is_player_color_used(color)) if (++color > 15) color = 1;
      mPlayer.init_player(cn, 1);
      mPlayer.set_player_start_pos(cn, 0);
      mPlayer.syn[cn].color = color;
      mPlayer.syn[cn].control_method = 2; //server client view only
      mPlayer.loc[cn].who = who;
      mPlayer.loc[cn].server_last_stak_rx_frame_num = mLoop.frame_num + 200;
      sprintf(mPlayer.loc[cn].hostname, "%s", temp_name);

      mGameMoves.add_game_move(mLoop.frame_num, 3, cn, color); // add a game move type 3 to mark client started join

      Packet("sjon"); // reply with sjon
      PacketPut2ByteInt(mLevel.play_level);
      PacketPut4ByteInt(mLoop.frame_num);
      PacketPut1ByteInt(cn);
      PacketPut1ByteInt(color);
      PacketPut1ByteInt(mShot.deathmatch_shots);
      PacketPut2ByteInt(mShot.deathmatch_shot_damage+1000);
      PacketPut1ByteInt(mShot.suicide_shots);
      ServerSendTo(packetbuffer, packetsize, who, cn);

      if (mLog.LOG_NET_join)
      {
         sprintf(msg,"Server replied with join invitation:");
         mLog.add_log_entry_position_text(11, 0, 76, 10, msg, "|", " ");
         sprintf(msg,"Level:[%d]", mLevel.play_level);
         mLog.add_log_entry_position_text(11, 0, 76, 10, msg, "|", " ");
         sprintf(msg,"Player Number:[%d]", cn);
         mLog.add_log_entry_position_text(11, 0, 76, 10, msg, "|", " ");
         sprintf(msg,"Player Color:[%d]", color);
         mLog.add_log_entry_position_text(11, 0, 76, 10, msg, "|", " ");
         sprintf(msg,"Server mLoop.frame_num:[%d]", mLoop.frame_num);
         mLog.add_log_entry_position_text(11, 0, 76, 10, msg, "|", " ");
         sprintf(msg,"Deathmatch player shots:[%d]", mShot.deathmatch_shots);
         mLog.add_log_entry_position_text(11, 0, 76, 10, msg, "|", " ");
         sprintf(msg,"Deathmatch player shot damage:[%d]", mShot.deathmatch_shot_damage);
         mLog.add_log_entry_position_text(11, 0, 76, 10, msg, "|", " ");
         sprintf(msg,"Suicide player shots:[%d]", mShot.suicide_shots);
         mLog.add_log_entry_position_text(11, 0, 76, 10, msg, "|", " ");
         mLog.add_log_entry_centered_text(11, 0, 76, "", "+", "-");
      }
   }
}


int mwNetgame::get_player_num_from_who(int who)
{
   for(int p=0; p<NUM_PLAYERS; p++)
      if (mPlayer.loc[p].who == who) return p;
   return -1;
}

void mwNetgame::server_fast_packet_loop(void)
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
            ServerSendTo(packetbuffer, packetsize, mPlayer.loc[p].who, p);
         }
      }

      if (PacketRead("pang"))
      {
         int p = get_player_num_from_who(who);
         if (p != -1)
         {
            double t0 = PacketGetDouble();
            double t1 = al_get_time();
            mPlayer.loc[p].ping = t1 - t0;
            //printf("rx pang from p:%d [%3.1f ms]\n", p, mPlayer.loc[p].ping*1000);
            mTally[4].add_data(mPlayer.loc[p].ping); // add to max tally
//            if (mPlayer.loc[p].ping > mPlayer.loc[0].server_max_client_ping) mPlayer.loc[0].server_max_client_ping = mPlayer.loc[p].ping;
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
               //printf("%d stored packet:%d size:%d type:%d\n", mLoop.frame_num, i, packetsize, type);
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

void mwNetgame::server_read_packet_buffer(void)
{
   for (int i=0; i<200; i++)
      if (packet_buffers[i].active)
      {
         //printf("%d read packet:%d  size:%d \n", mLoop.frame_num, i, packet_buffers[i].packetsize);

         memcpy(packetbuffer, packet_buffers[i].data, 1024);
         packetsize = packet_buffers[i].packetsize;

         set_packetpos(4);

         if (packet_buffers[i].type == 1) server_proc_cdat_packet(packet_buffers[i].timestamp);
         if (packet_buffers[i].type == 2) server_proc_stak_packet(packet_buffers[i].timestamp);
         if (packet_buffers[i].type == 3) server_proc_cjon_packet(packet_buffers[i].who);

         packet_buffers[i].active = 0;
      }
}

void mwNetgame::server_control()
{
   ServerListen();               // listen for new client connections
   server_read_packet_buffer();
   server_rewind();              // to replay and apply late client input
   server_proc_player_drop();    // check to see if we need to drop clients
   if (mLog.LOG_NET_player_array) mLog.log_player_array2();
   for (int p=0; p<NUM_PLAYERS; p++) if (mPlayer.syn[p].active) process_bandwidth_counters(p);
}















































