// mwNetgameServer.cpp

#include <limits>

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
#include "mwMain.h"
#include "mwRollingAverage.h"
#include "mwConfig.h"



int mwNetgame::ServerInitNetwork() // Initialize the server
{
   init_packet_buffer();
   if (NetworkInit())
   {
      mLog.add_fw(LOG_error, 0, 76, 10, "|", " ", "Error: failed to initialize network");
      return -1;
   }
   if (TCP)
   {
      ListenConn = net_openconn(NetworkDriver, "");
      if (!ListenConn)
      {
         mLog.add_fw(LOG_error, 0, 76, 10, "|", " ", "Error opening connection");
         return -1;
      }
      if (net_listen(ListenConn))
      {
         mLog.add_fw(LOG_error, 0, 76, 10, "|", " ", "Error listening on connection");
         return -1;
      }
      mLog.add_fw(LOG_NET, 0, 76, 10, "|", " ", "Network initialized - connection mode (TCP)");
   }
   else // UDP
   {
      // open the listening channel
      if (!(ListenChannel = net_openchannel(NetworkDriver, "")))
      {
         mLog.add_fw(LOG_error, 0, 76, 10, "|", " ", "Error opening listening channel");
         return -1;
      }
      if (net_assigntarget(ListenChannel, ""))
      {
         mLog.add_fw(LOG_error, 0, 76, 10, "|", " ", "Error assigning target to listening channel");
         net_closechannel(ListenChannel);
         return -1;
      }
      mLog.add_fw(LOG_NET, 0, 76, 10, "|", " ", "Network initialized - channel mode (UDP)");
   }
   return 0;
}


void mwNetgame::ServerListen()
{
   if (TCP) // listen for connections
   {
      NET_CONN *newconn;
      newconn = net_poll_listen(ListenConn);
      if (newconn == NULL) return;
      else
      {
         ClientConn[ClientNum] = newconn;
         ClientNum++;
         mLog.add_fw (LOG_NET, 0, 76, 10, "+", "-", "");
         mLog.add_fwf(LOG_NET, 0, 76, 10, "+", "-", "Connection received from %s", net_getpeer (newconn));
         mLog.add_fw (LOG_NET, 0, 76, 10, "+", "-", "");
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
            mLog.add_fwf(LOG_NET, 0, 76, 10, "|", " ", "Server received initial 1234 packet from '%s'",address);
            if (!(ClientChannel[ClientNum] = net_openchannel(NetworkDriver, NULL)))
            {
               mLog.add_fwf(LOG_error, 0, 76, 10, "|", "-", "Error: failed to open channel for %s\n", address);
               return;
            }
            if (net_assigntarget (ClientChannel[ClientNum], address))
            {
               mLog.add_fwf(LOG_error, 0, 76, 10, "|", "-", "Error: couldn't assign target `%s' to channel\n", address);
               net_closechannel (ClientChannel[ClientNum]);
               return;
            }
            Packet("5678");
            ServerSendTo(packetbuffer, packetsize, ClientNum, 0);
            ClientNum++;
            mLog.add_fwf(LOG_NET, 0, 76, 10, "|", " ", "Server opened channel for `%s' and sent reply", address);
         }
      }
   }
}

// Receive all waiting packets from all clients, and store in provided array
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
      if ((TCP)  && (ClientConn[n]))    net_send_rdm( ClientConn[n],    data, len);
      if ((!TCP) && (ClientChannel[n])) net_send(     ClientChannel[n], data, len);
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

   if (player != 99)
   {
      // add to client's counts
      mPlayer.loc[player].tx_current_bytes_for_this_frame += len;
      mPlayer.loc[player].tx_current_packets_for_this_frame++;
   }

}
void mwNetgame::server_flush(void)
{
   int who;
   while (ServerReceive(packetbuffer, &who));
}

void mwNetgame::ServerExitNetwork() // Shut the server down
{
   mLog.add_header(LOG_NET, 0, 0, "Shutting down the server network");
   if (TCP)
   {
      for (int n=0; n<ClientNum; n++)
         if (ClientConn[n])
         {
            net_closeconn(ClientConn[n]);
            ClientConn[n] = NULL;
         }
   	if (ListenConn) net_closeconn(ListenConn);
   	ListenConn = NULL;
   }
   else // UDP
   {
      for (int n=0; n<ClientNum; n++)
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
   mLog.log_versions();
   mLog.add_fw (LOG_NET, 0, 76, 10, "+", "-", "");
   mLog.add_fw (LOG_NET, 0, 76, 10, "|", " ", "Server mode started");
   mLog.add_fwf(LOG_NET, 0, 76, 10, "|", " ", "Server hostname:    [%s]", mLoop.local_hostname);
   mLog.add_fwf(LOG_NET, 0, 76, 10, "|", " ", "Level:              [%d]", mLevel.play_level);

   if (ServerInitNetwork())
   {
      mLog.add_fw(LOG_error, 0, 76, 10, "|", " ", "Error Initializing Server Network");
      return 0;
   }

   mLog.add_fw(LOG_NET, 0, 76, 10, "|", " ", "Server successfully initialized");
   mLog.add_fw(LOG_NET, 0, 76, 10, "+", "-", "");

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
   double t0 = al_get_time();
   if (mLoop.frame_num >= mStateHistory[0].newest_state_frame_num + server_state_freq - 1) // is it time to create a new state?
   {


      // save values we don't want rewound
      float old_cco = mPlayer.syn[0].client_chase_offset;
      int pvps = mPlayer.syn[0].player_vs_player_shots;
      int pvpd = mPlayer.syn[0].player_vs_player_shot_damage;
      int pvss = mPlayer.syn[0].player_vs_self_shots;
      int lcd[8][2] = { 0 };
      for (int pp=0; pp<NUM_PLAYERS; pp++)
         if (mPlayer.syn[pp].active)
         {
            lcd[pp][0] = mPlayer.syn[pp].late_cdats;
            lcd[pp][1] = mPlayer.syn[pp].late_cdats_last_sec;
         }


//      mStateHistory[0].apply_rewind_state(mStateHistory[0].oldest_state_frame_num);
      mStateHistory[0].apply_rewind_state(server_dirty_frame);
      mPlayer.loc[0].server_send_dif = 1; // set flag to create and send dif at the end of this frame


      // restore values we don't want reset
      mPlayer.syn[0].client_chase_offset = old_cco;
      mPlayer.syn[0].player_vs_player_shots = pvps;
      mPlayer.syn[0].player_vs_player_shot_damage = pvpd;
      mPlayer.syn[0].player_vs_self_shots = pvss;
      for (int pp=0; pp<NUM_PLAYERS; pp++)
         if (mPlayer.syn[pp].active)
         {
            mPlayer.syn[pp].late_cdats = lcd[pp][0];
            mPlayer.syn[pp].late_cdats_last_sec = lcd[pp][1];
         }



   }
   mLog.add_tmr1(LOG_TMR_rwnd, 0, "rwnd", al_get_time() - t0);
}

void mwNetgame::server_create_new_state(void)
{
   // this state is created at the end of the frame, after moves have been applied
   // because of that it is actually the starting point of the next frame
   // so it is sent and saved with frame_num + 1

   // it is done this way, rather than wait for the start of the next frame
   // because after the end of the frame, there is a pause before the next frame starts
   // and I want to send new state as soon as possible

   if (mPlayer.loc[0].server_send_dif)
   {

      int frame_num = mLoop.frame_num + 1;

      server_dirty_frame = frame_num;


      mPlayer.loc[0].server_send_dif = 0;

      // this is the server rewind state, not the client base
      mStateHistory[0].add_state(frame_num);

      mLog.addf(LOG_NET_stdf, 0, "stdf save state:%d\n", frame_num);
      server_send_dif(frame_num); // send to all clients
   }
}

void mwNetgame::server_send_dif(int frame_num) // send dif to all clients
{
  for (int p=1; p<NUM_PLAYERS; p++)
      if ((mPlayer.syn[p].control_method == 2) || (mPlayer.syn[p].control_method == 8))
      {
         // save current state in history as base for next clients send
         mStateHistory[p].add_state(frame_num);

         char base[STATE_SIZE] = {0};
         int base_frame_num = 0;

         // get client's most recent base state (the last one acknowledged to the server)
         // if not found, leaves base as is (zero)
         mStateHistory[p].get_last_ack_state(base, base_frame_num);

         if (base_frame_num == 0) mPlayer.loc[p].client_base_resets++;

         // make a new dif from base and current
         char dif[STATE_SIZE];
         get_state_dif(base, mStateHistory[p].newest_state, dif, STATE_SIZE);

         // break into packet and send to client
         server_send_compressed_dif(p, base_frame_num, frame_num, dif);
      }
}


void mwNetgame::server_send_compressed_dif(int p, int src, int dst, char* dif) // send dif to a client
{
   char cmp[STATE_SIZE];

   // compress dif to cmp
   uLongf destLen= sizeof(cmp);
   compress2((Bytef*)cmp, (uLongf*)&destLen, (Bytef*)dif, STATE_SIZE, zlib_cmp);
   int cmp_size = destLen;
   float cr = (float)cmp_size*100 / (float)STATE_SIZE; // compression ratio



   // break compressed dif into smaller pieces
   int num_packets = (cmp_size / 1000) + 1;

   mPlayer.loc[p].cmp_dif_size = cmp_size;
   mPlayer.loc[p].num_dif_packets = num_packets;

   mLog.addf(LOG_NET_stdf, p, "tx stdf p:%d [s:%d d:%d] cmp:%d ratio:%3.2f [%d packets needed]\n", p, src, dst, cmp_size, cr, num_packets);

   int start_byte = 0;
   for (int packet_num=0; packet_num < num_packets; packet_num++)
   {
      int packet_data_size = 1000; // default size
      if (start_byte + packet_data_size > cmp_size) packet_data_size = cmp_size - start_byte; // last piece is smaller

      mLog.addf(LOG_NET_stdf_packets, p, "tx stdf piece [%d of %d] [%d to %d] st:%4d sz:%4d\n", packet_num+1, num_packets, src, dst, start_byte, packet_data_size);

      Packet("stdf");
      PacketPutInt4(src); // src frame_num
      PacketPutInt4(dst); // dst frame_num
      PacketPutInt1(packet_num);
      PacketPutInt1(num_packets);
      PacketPutInt4(start_byte);
      PacketPutInt4(packet_data_size);
      memcpy(packetbuffer+packetsize, cmp+start_byte, packet_data_size);
      packetsize += packet_data_size;
      ServerSendTo(packetbuffer, packetsize, mPlayer.loc[p].who, p);
      start_byte+=1000;
   }
}



void mwNetgame::server_send_snfo(void) // send info to remote control
{
   // copy values into loc
   mPlayer.loc[0].srv_frame_num = mLoop.frame_num;
   mPlayer.loc[0].srv_stdf_freq = mNetgame.server_state_freq;
   mPlayer.loc[0].srv_total_game_moves = mGameMoves.entry_pos;
   mPlayer.loc[0].srv_level = mLevel.play_level;
   mPlayer.loc[0].srv_zlib_cmp = mNetgame.zlib_cmp;
   mPlayer.loc[0].srv_extra_packets_num = mNetgame.srv_exp_num;
   mPlayer.loc[0].srv_extra_packets_size = mNetgame.srv_exp_siz;

   char src[5400];
   char dst[5400];

   int sz=0, offset=0;
   offset += sz; sz = sizeof(mPlayer.syn); memcpy(src + offset, mPlayer.syn, sz);
   offset += sz; sz = sizeof(mPlayer.loc); memcpy(src + offset, mPlayer.loc, sz);

   // compress src to dst
   uLongf destLen= sizeof(dst);
   compress2((Bytef*)dst, (uLongf*)&destLen, (Bytef*)src, sizeof(src), zlib_cmp);
   int dst_size = destLen;

   // break compressed dst into smaller pieces
   int num_packets = (dst_size / 1000) + 1;

   //float cr = (float)dst_size*100 / (float)5400; // compression ratio
   //printf("tx snfo fn:[%d] size:[%d] ratio:[%3.2f] [%d packets needed]\n", mLoop.frame_num, dst_size, cr, num_packets);

   int start_byte = 0;
   for (int packet_num=0; packet_num < num_packets; packet_num++)
   {
      int packet_data_size = 1000; // default size
      if (start_byte + packet_data_size > dst_size) packet_data_size = dst_size - start_byte; // last piece is smaller

      // printf("tx snfo piece fn:[%d] packet:[%d of %d]\n", mLoop.frame_num, packet_num+1, num_packets);

      Packet("snfo");
      PacketPutInt4(mLoop.frame_num);       // frame_num
      PacketPutInt1(packet_num);
      PacketPutInt1(num_packets);
      PacketPutInt4(start_byte);
      PacketPutInt4(packet_data_size);
      memcpy(packetbuffer + packetsize, dst + start_byte, packet_data_size);
      packetsize += packet_data_size;
      ServerSendTo(packetbuffer, packetsize, mMain.server_remote_control_who, 99);
      start_byte+=1000;
   }
}







































void mwNetgame::server_proc_stak_packet(double timestamp)
{
   int p                             = PacketGetInt1();
   int ack_frame_num                 = PacketGetInt4(); // client has acknowledged getting and applying this base
   int client_frame_num              = PacketGetInt4();
   mPlayer.loc[p].client_chase_fps   = PacketGetDouble();
   mPlayer.loc[p].dsync              = PacketGetDouble();
   mPlayer.loc[p].rewind             = PacketGetInt1();
   mPlayer.loc[p].client_loc_plr_cor = PacketGetDouble();
   mPlayer.loc[p].client_rmt_plr_cor = PacketGetDouble();
   mPlayer.loc[p].cpu                = PacketGetDouble();

   mTally_client_loc_plr_cor_last_sec[p].add_data(mPlayer.loc[p].client_loc_plr_cor);
   mTally_client_rmt_plr_cor_last_sec[p].add_data(mPlayer.loc[p].client_rmt_plr_cor);

   // calculate stak_sync
   int stak_sync = mLoop.frame_num - mStateHistory[p].newest_state_frame_num;

   // calculate stak_dsync
   mPlayer.loc[p].stak_dsync = ( (double) stak_sync * 0.025) + mTimeStamp.timestamp_frame_start - timestamp;

   server_lock_client(p);

   // this is used to see if client is still alive
   mPlayer.loc[p].server_last_stak_rx_frame_num = mLoop.frame_num;

   // make sure we have a copy of acknowledged state in histroy, finds and sets values, or sets to -1 if not found
   mStateHistory[p].set_ack_state(ack_frame_num);

   mLog.addf(LOG_NET_stak, p, "rx stak p:%d ack:[%d] cur:[%d] dsync:[%4.1f] chase:[%4.1f]\n", p, ack_frame_num, client_frame_num, mPlayer.loc[p].dsync*1000, mPlayer.loc[p].client_chase_fps);
}


void mwNetgame::server_proc_player_drop(void)
{
   // check to see if we need to drop clients
   for (int p=1; p<NUM_PLAYERS; p++)
      if ((mPlayer.syn[p].control_method == 2) && (mPlayer.loc[p].server_last_stak_rx_frame_num + 100 < mLoop.frame_num))
      {
         mGameMoves.add_game_move(mLoop.frame_num + 4, 2, p, 71); // make client inactive (reason no stak for 100 frames)
         mLog.add_headerf(LOG_NET, p, 1, "Server dropped player:%d (last stak rx > 100)", p);
      }

   int reload = 0;
   int gm_limit = GAME_MOVES_SIZE - 100;
   int tm_limit = 60 * 60 * 40; // 1H

//   int gm_limit = 20;
//   int tm_limit = 30 * 40;

   if (mGameMoves.entry_pos > gm_limit)
   {
      mLog.add_headerf(LOG_NET, 0, 1, "Server Approaching %d Game Moves! - Reload", GAME_MOVES_SIZE);
      reload = 1;
   }
   if (mLoop.frame_num > tm_limit)
   {
      mLog.add_headerf(LOG_NET, 0, 1, "Server Approaching %d frames! - Reload", tm_limit);
      reload = 1;
   }
   if (reload) server_reload(1); // to overworld
}

void mwNetgame::server_reload(int level)
{
   //  1-99 level num
   //  0 nothing
   // -1 current level
   // -2 next level

   if ((level != 0) && (mPlayer.syn[0].level_done_mode == 0)) // only trigger from level done mode 0
   {
      if (level == -2) level = mLevel.get_next_level(mLevel.play_level, 199, 1);
      if (level == -1) level = mLevel.play_level;
      mPlayer.syn[0].level_done_mode = 3;
      mPlayer.syn[0].level_done_timer = 0;
      mPlayer.syn[0].level_done_next_level = level;
   }
}



void mwNetgame::server_proc_cdat_packet(double timestamp)
{
   int p = PacketGetInt1();
   int cdat_frame_num = PacketGetInt4();
   int cm = PacketGetInt1();

   mPlayer.loc[p].client_cdat_packets_tx++;

   // calculate game_move_sync
   mPlayer.loc[p].server_game_move_sync = cdat_frame_num - mLoop.frame_num;

   // calculate game_move_dsync
   mPlayer.loc[p].game_move_dsync = ( (double) mPlayer.loc[p].server_game_move_sync * 0.025) + mTimeStamp.timestamp_frame_start - timestamp;

   // add to average tally
   mTally_game_move_dsync_avg_last_sec[p].add_data(mPlayer.loc[p].game_move_dsync);

   // determine the cutoff frame for late cdats
   int of = mStateHistory[0].oldest_state_frame_num;

   if (cdat_frame_num < of)
   {
      mPlayer.syn[p].late_cdats++;
      mTally_late_cdats_last_sec[p].add_data(1); // add to tally
      mLog.addf(LOG_NET_cdat, p, "rx cdat p:%d fn:[%d] sync:[%d] late - droppped\n", p, cdat_frame_num, mPlayer.loc[p].server_game_move_sync);
   }
   else
   {
      mGameMoves.add_game_move(cdat_frame_num, 5, p, cm); // add to game_move array
      mLog.addf(LOG_NET_cdat, p, "rx cdat p:%d fn:[%d] sync:[%d] gmep:[%d] - entered\n", p, cdat_frame_num, mPlayer.loc[p].server_game_move_sync, mGameMoves.entry_pos);

      if (cdat_frame_num < server_dirty_frame) server_dirty_frame = cdat_frame_num;

   }

}

void mwNetgame::server_lock_client(int p)
{
   float dsync = mPlayer.loc[p].dsync * 1000;

   // printf("Server Lock Client:%d a:%d cm:%d dsync:%4.2f\n", p, mPlayer.syn[p].active, mPlayer.syn[p].control_method, dsync);

   if ((!mPlayer.syn[p].active) && (mPlayer.syn[p].control_method == 2)) // inactive client chasing for lock
   {
      if ((dsync > -200) && (dsync < 30)) mPlayer.loc[p].sync_stabilization_holdoff++;
      else mPlayer.loc[p].sync_stabilization_holdoff = 0;
   }
   if (mPlayer.loc[p].sync_stabilization_holdoff > 20) // we have been stable for over 20 frames
   {
      mGameMoves.add_game_move(mLoop.frame_num + 4, 1, p, mPlayer.syn[p].color);
      mPlayer.loc[p].sync_stabilization_holdoff = 0;
      mLog.add_headerf(LOG_NET, p, 0, "Player:%d has locked and will become active in 4 frames!", p);
   }
}



void mwNetgame::server_proc_cjon_packet(int who)
{
   char temp_name[16];
   int color = PacketGetInt1();
   PacketReadString(temp_name);

   mLog.add_fwf(LOG_NET, 0, 76, 10, "+", "-", "");
   mLog.add_fwf(LOG_NET, 0, 76, 10, "|", " ", "Server received join request from %s requesting color:%d", temp_name, color);

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
      mLog.add_fwf(LOG_NET, 0, 76, 10, "|", " ", "Reply sent: 'SERVER FULL'");
      mLog.add_fwf(LOG_NET, 0, 76, 10, "+", "-", "");

      Packet("sjon");    // reply with sjon
      PacketPutInt2(0);
      PacketPutInt4(0);
      PacketPutInt1(0);
      PacketPutInt1(99); // send sjon with player 99 to indicate server full
      PacketPutInt1(0);
      PacketPutInt2(0);
      PacketPutInt1(0);
      ServerSendTo(packetbuffer, packetsize, who, 0);
   }
   else // empty player slot found, proceed with join
   {
      // try to use requested color, unless already used by another player
      while (mPlayer.is_player_color_used(color)) if (++color > 15) color = 1;

      mPlayer.init_player(cn, 1); // full player reset

      mStateHistory[cn].initialize();


      mPlayer.set_player_start_pos(cn, 0);

      mPlayer.syn[cn].active = 1;

      mPlayer.syn[cn].color = color;
      mPlayer.syn[cn].control_method = 2; //server client view only
      mPlayer.loc[cn].who = who;
      mPlayer.loc[cn].server_last_stak_rx_frame_num = mLoop.frame_num + 200;
      sprintf(mPlayer.loc[cn].hostname, "%s", temp_name);

      mGameMoves.add_game_move(mLoop.frame_num, 3, cn, color); // add a game move type 3 to mark client started join

      Packet("sjon"); // reply with sjon
      PacketPutInt2(mLevel.play_level);
      PacketPutInt4(mLoop.frame_num);
      PacketPutInt1(cn);
      PacketPutInt1(color);

      ServerSendTo(packetbuffer, packetsize, who, cn);

      mLog.add_fwf(LOG_NET,               0, 76, 10, "|", " ", "Server replied with join invitation:");
      mLog.add_fwf(LOG_NET_join_details,  0, 76, 10, "|", " ", "Level:[%d]", mLevel.play_level);
      mLog.add_fwf(LOG_NET_join_details,  0, 76, 10, "|", " ", "Player Number:[%d]", cn);
      mLog.add_fwf(LOG_NET_join_details,  0, 76, 10, "|", " ", "Player Color:[%d]", color);
      mLog.add_fwf(LOG_NET_join_details,  0, 76, 10, "|", " ", "Server mLoop.frame_num:[%d]", mLoop.frame_num);
      mLog.add_fwf(LOG_NET,               0, 76, 10, "+", "-", "");
   }
}


int mwNetgame::get_player_num_from_who(int who)
{
   for(int p=0; p<NUM_PLAYERS; p++)
      if (mPlayer.loc[p].who == who) return p;
   return -1;
}





void mwNetgame::server_proc_rctl_packet(void)
{
   int type = PacketGetInt4();
   double val = PacketGetDouble();


   if (type == PM_RCTL_PACKET_TYPE_state_freq_adj)
   {
      mNetgame.server_state_freq += val;
      if (mNetgame.server_state_freq < 1)  mNetgame.server_state_freq = 1;
      if (mNetgame.server_state_freq > 9)  mNetgame.server_state_freq = 9;
   }

   if (type == PM_RCTL_PACKET_TYPE_client_offset_adj)
   {
      mPlayer.syn[0].client_chase_offset += val;
   }


   if (type == PM_RCTL_PACKET_TYPE_zlib_compression_adj)
   {
      mNetgame.zlib_cmp += val;
      if (mNetgame.zlib_cmp < 0)  mNetgame.zlib_cmp = 0;
      if (mNetgame.zlib_cmp > 9)  mNetgame.zlib_cmp = 9;
   }

   if (type == PM_RCTL_PACKET_TYPE_exra_packet_num_adj)
   {
      mNetgame.srv_exp_num += val;
      if (mNetgame.srv_exp_num < 0)    mNetgame.srv_exp_num = 0;
      if (mNetgame.srv_exp_num > 500)  mNetgame.srv_exp_num = 500;
   }

   if (type == PM_RCTL_PACKET_TYPE_exra_packet_siz_adj)
   {
      mNetgame.srv_exp_siz += val;
      if (mNetgame.srv_exp_siz < 0)     mNetgame.srv_exp_siz = 0;
      if (mNetgame.srv_exp_siz > 1000)  mNetgame.srv_exp_siz = 1000;
   }

   if (type == PM_RCTL_PACKET_TYPE_pvp_shot_damage_adj)
   {
      mPlayer.syn[0].player_vs_player_shot_damage += val;
      if (mPlayer.syn[0].player_vs_player_shot_damage < -100)  mPlayer.syn[0].player_vs_player_shot_damage = -100;
      if (mPlayer.syn[0].player_vs_player_shot_damage > 100)  mPlayer.syn[0].player_vs_player_shot_damage = 100;
   }

   if (type == PM_RCTL_PACKET_TYPE_server_reload) server_reload((int)val);


   if (type == PM_RCTL_PACKET_TYPE_pvp_shots_toggle) mPlayer.syn[0].player_vs_player_shots = !mPlayer.syn[0].player_vs_player_shots;

   if (type == PM_RCTL_PACKET_TYPE_pvs_shots_toggle) mPlayer.syn[0].player_vs_self_shots = !mPlayer.syn[0].player_vs_self_shots;

   mConfig.save_config();



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

      if (PacketRead("cjrc"))
      {
         mMain.server_remote_control = 1;
         mMain.server_remote_control_who = who;
         Packet("sjrc");
         ServerSendTo(packetbuffer, packetsize, who, 99);
      }

      if (PacketRead("rctl")) server_proc_rctl_packet();


      int type = 0;
      if(PacketRead("cdat")) type = 1;
      if(PacketRead("stak")) type = 2;
      if(PacketRead("cjon")) type = 3;
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
            int cdat_count = 0;
            int stak_count = 0;
            int cjon_count = 0;
            int all_count = 0;

            for (int i=0; i<200; i++)
               if (!packet_buffers[i].active)
               {
                  all_count++;
                  if (packet_buffers[i].type == 1) cdat_count++;
                  if (packet_buffers[i].type == 2) stak_count++;
                  if (packet_buffers[i].type == 3) cjon_count++;
               }
            mLog.addf(LOG_NET, 0, "[%d] cdat\n", cdat_count);
            mLog.addf(LOG_NET, 0, "[%d] stak\n", stak_count);
            mLog.addf(LOG_NET, 0, "[%d] cjon\n", cjon_count);
            mLog.addf(LOG_NET, 0, "[%d] all\n", all_count);

            printf("[%d] cdat\n", cdat_count);
            printf("[%d] stak\n", stak_count);
            printf("[%d] cjon\n", cjon_count);
            printf("[%d] all\n", all_count);
         }



         else
         {
            //printf("%d stored packet:%d size:%d type:%d\n", mLoop.frame_num, i, packetsize, type);
            packet_buffers[indx].active = 1;
            packet_buffers[indx].type = type;
            packet_buffers[indx].timestamp = timestamp;
            packet_buffers[indx].who = who;
            packet_buffers[indx].packetsize = packetsize;
            memcpy(packet_buffers[indx].data, packetbuffer, 1024);
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
   server_read_packet_buffer();  // process all packets in buffer
   server_rewind();              // to replay and apply late client input
   server_proc_player_drop();    // check to see if we need to drop clients

   if (mMain.server_remote_control) server_send_snfo();

   // send extra packets
   for (int i=0; i<srv_exp_num; i++)
      for (int p=1; p<NUM_PLAYERS; p++)
         if (mPlayer.syn[p].control_method == 2)
         {
            Packet("extr");
            packetsize += srv_exp_siz;
            ServerSendTo(packetbuffer, packetsize, mPlayer.loc[p].who, p);
         }

   mLog.log_player_array2(LOG_NET_player_array);

   for (int p=0; p<NUM_PLAYERS; p++) if (mPlayer.syn[p].active) process_bandwidth_counters(p);
}















































