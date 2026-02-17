// mwNetgameServer.cpp

#include "pm.h"
#include "mwNetgame.h"
#include "mwPacketBuffer.h"
#include "mwLog.h"
#include "mwPlayer.h"
#include "mwGameMoves.h"
#include "mwLevel.h"
#include "mwLoop.h"
#include "mwInput.h"
#include "mwMain.h"
#include "mwConfig.h"
#include "mwItem.h"
#include "mwScreen.h"
#include "mwEnemy.h"
#include "mwMiscFnx.h"
#include "mwSql.h"



int mwNetgame::serverInitNetwork()
{
   mLog.log_versions();
   mLog.add_fw (LOG_NET, -1, 76, 10, "+", "-", "");
   mLog.add_fw (LOG_NET, -1, 76, 10, "|", " ", "Server mode started");
   mLog.add_fwf(LOG_NET, -1, 76, 10, "|", " ", "Server hostname:    [%s]", mLoop.local_hostname);
   mLog.add_fwf(LOG_NET, -1, 76, 10, "|", " ", "Level:              [%d]", mLevel.play_level);

   mLog.add_log_net_db_row(LOG_NET, 0, 0, "Server mode started - hostname:%s", mLoop.local_hostname);

   if (networkInit())
   {
      mLog.log_error("failed to initialize network");
      return 0;
   }

   // open the server channel
   char port[256];
   sprintf(port, ":%d", server_port);
   if (!(Channel = net_openchannel(networkDriver, port)))
   {
      mLog.log_error("failed to open server channel");
      return 0;
   }
   mLog.add_fw(LOG_NET, -1, 76, 10, "|", " ", "Server network initialized");
   mLog.add_fw(LOG_NET, -1, 76, 10, "+", "-", "");

   ima_server = 1;
   return 1;
}

void mwNetgame::serverExitNetwork()
{
   mLog.add_header(LOG_NET, -1, 0, "Shutting down the server network");

   mLog.add_log_net_db_row(LOG_NET, 0, 0, "Shutting down the server network");


   networkExit();

   ima_server = 0;

   // reset player data
   for (int p=0; p<NUM_PLAYERS; p++) mPlayer.init_player(p, 1);
   mPlayer.syn[0].active = 1;

   if (mLog.log_types[LOG_NET_session].action) session_flush_active_at_server_exit();
}




int mwNetgame::server_check_address(char * address)
{
   for (int n=0; n<8; n++)
      if ((mwChannels[n].channel_active) && (strcmp(address, mwChannels[n].channel_address) == 0)) return n;
   return -1;
}

// send data to a specific client
void mwNetgame::serverSendTo(void *data, int len, int p)
{
   // change the target of the ServerChannel
   if (net_assigntarget(Channel, mwChannels[p].channel_address))
   {
      char msg[512];
      sprintf(msg, "Error: couldn't assign target `%s' to ServerChannel\n", mwChannels[p].channel_address);
      mLog.log_error(msg, 0);
   }
   net_send(Channel, data, len);

   // add to server's counts
   mPlayer.loc[0].tx_current_bytes_for_this_frame += (len + 42);
   mPlayer.loc[0].tx_current_packets_for_this_frame++;

   // add to client's counts
   mPlayer.loc[p].tx_current_bytes_for_this_frame += (len + 42);
   mPlayer.loc[p].tx_current_packets_for_this_frame++;
}

void mwNetgame::headless_server_setup()
{
   mMain.classic_mode = 0;
   mLevel.unlock_all_levels();

   // make the hidden server player color taan (6)
   mPlayer.syn[0].color = 6;

   mLog.clear_all_log_actions();

   // always have the basic LOG_NET active and printing to console as well as saving to file
//   mLog.set_log_type_action(LOG_NET, LOG_ACTION_PRINT | LOG_ACTION_LOG);

  // always have session logging on
   mLog.set_log_type_action(LOG_NET_session, LOG_ACTION_LOG);
//
//   // add these for troubleshooting
//   mLog.set_log_type_action(LOG_NET_ending_stats,  LOG_ACTION_LOG);
//   mLog.set_log_type_action(LOG_NET_bandwidth,     LOG_ACTION_LOG);

//   mLog.set_log_type_action(LOG_NET_stdf_rewind,   LOG_ACTION_LOG);
//   mLog.set_log_type_action(LOG_NET_stdf_create,   LOG_ACTION_LOG);
//   mLog.set_log_type_action(LOG_NET_stdf,          LOG_ACTION_LOG);
//   mLog.set_log_type_action(LOG_NET_stdf_packets,  LOG_ACTION_LOG);
//
//   mLog.set_log_type_action(LOG_NET_dif_apply,     LOG_ACTION_LOG);
//
//   mLog.set_log_type_action(LOG_NET_cdat,          LOG_ACTION_LOG);
//   mLog.set_log_type_action(LOG_NET_stak,          LOG_ACTION_LOG);
//   mLog.set_log_type_action(LOG_NET_client_ping,   LOG_ACTION_LOG);
//   mLog.set_log_type_action(LOG_NET_timer_adjust,  LOG_ACTION_LOG);
//
//   mLog.set_log_type_action(LOG_NET_file_transfer, LOG_ACTION_LOG);

//   mLog.set_log_type_action(LOG_OTH_level_done,   LOG_ACTION_LOG);

   mLog.autosave_log_on_level_done = 1;
   mLog.autosave_log_on_level_quit = 1;
   mLog.autosave_log_on_program_exit = 1;

   // make sure we are always saving games
   mGameMoves.autosave_game_on_level_done = 1;
   mGameMoves.autosave_game_on_level_quit = 1;

//   mGameMoves.server_send_files_to_clients = 1;

   mConfig.save_config();
}


void mwNetgame::server_rewind()
{
   double t0 = al_get_time();

   // save values we don't want rewound
   float old_cco = mPlayer.syn[0].client_chase_offset;
   int pvps = mPlayer.syn[0].player_vs_player_shots;
   int pvpd = mPlayer.syn[0].player_vs_player_shot_damage;
   int pvss = mPlayer.syn[0].player_vs_self_shots;
   int sffk = mPlayer.syn[0].server_force_fakekey;
   int sfco = mPlayer.syn[0].server_force_client_offset;
   int lcd[8][2] = { 0 };
   for (int pp=0; pp<NUM_PLAYERS; pp++)
      if (mPlayer.syn[pp].active)
      {
         lcd[pp][0] = mPlayer.syn[pp].late_cdats;
         lcd[pp][1] = mPlayer.syn[pp].late_cdats_last_sec;
      }

   mStateHistory[0].apply_rewind_state(server_dirty_frame);

   // restore values we don't want reset
   mPlayer.syn[0].client_chase_offset = old_cco;
   mPlayer.syn[0].player_vs_player_shots = pvps;
   mPlayer.syn[0].player_vs_player_shot_damage = pvpd;
   mPlayer.syn[0].player_vs_self_shots = pvss;
   mPlayer.syn[0].server_force_fakekey = sffk;
   mPlayer.syn[0].server_force_client_offset = sfco;
   for (int pp=0; pp<NUM_PLAYERS; pp++)
      if (mPlayer.syn[pp].active)
      {
         mPlayer.syn[pp].late_cdats = lcd[pp][0];
         mPlayer.syn[pp].late_cdats_last_sec = lcd[pp][1];
      }

   mLog.add_tmr1(LOG_TMR_rwnd, "rwnd", al_get_time() - t0);
}

void mwNetgame::server_create_new_state()
{
   // this state is created at the end of the frame, after moves have been applied
   // because of that it is actually the starting point of the next frame
   // so it is sent and saved with frame_num + 1

   // it is done this way, rather than wait for the start of the next frame
   // because after the end of the frame, there is a pause before the next frame starts
   // and I want to send new state as soon as possible

   int frame_num = mLoop.frame_num + 1;

   // reset dirty_frame
   server_dirty_frame = frame_num;

   // this is the server rewind state, not the client base
   mStateHistory[0].add_state(frame_num);

   mLog.log_add_prefixed_textf(LOG_NET_stdf_create, -1, "stdf save state:%d\n", frame_num);
   mLog.add_log_net_db_row(LOG_NET_stdf_create, 0, 0,    "stdf save state:%d", frame_num);



    // send to all active clients
   server_send_dif(frame_num);
}

void mwNetgame::server_send_dif(int frame_num)
{
   for (int p=1; p<NUM_PLAYERS; p++)
      if ((mPlayer.syn[p].active) && ((mPlayer.syn[p].control_method == PM_PLAYER_CONTROL_METHOD_NETGAME_REMOTE) || (mPlayer.syn[p].control_method == PM_PLAYER_CONTROL_METHOD_CLIENT_ORPHAN)))
      {
         // save current state in history as base for next clients send
         mStateHistory[p].add_state(frame_num);

         // variables for base and dif
         char base[STATE_SIZE] = {0};
         int base_frame_num = 0;
         char dif[STATE_SIZE];

         // get client's most recent base state (the last one acknowledged to the server) if not found, leaves base as is (zero)
         mStateHistory[p].get_last_ack_state(base, base_frame_num);

         if (base_frame_num == 0) mPlayer.loc[p].client_base_resets++;

         // make a new dif from base and current
         get_state_dif(base, mStateHistory[p].newest_state, dif, STATE_SIZE);

         // break into packet and send to client
         server_send_compressed_dif(p, base_frame_num, frame_num, dif);
      }
}


void mwNetgame::server_send_compressed_dif(int p, int src, int dst, char* dif) // send dif to a client
{
   // compress dif to cmp
   char cmp[STATE_SIZE];
   uLongf destLen= sizeof(cmp);
   compress2((Bytef*)cmp, (uLongf*)&destLen, (Bytef*)dif, STATE_SIZE, zlib_cmp);
   int cmp_size = destLen;
   float cr = (float)cmp_size*100 / (float)STATE_SIZE; // compression ratio

   // break compressed dif into smaller pieces
   int num_packets = (cmp_size / PACKET_PAYLOAD_CHUNK_SIZE) + 1;

   mPlayer.loc[p].cmp_dif_size = cmp_size;
   mPlayer.loc[p].num_dif_packets = num_packets;



   mLog.log_add_prefixed_textf(LOG_NET_stdf, -1, "tx stdf p:%d [s:%d d:%d] cmp:%d ratio:%3.2f [%d packets needed]\n", p, src, dst, cmp_size, cr, num_packets);
   mLog.add_log_net_db_row(LOG_NET_stdf, 0, p,    "tx stdf p:%d [s:%d d:%d] cmp:%d ratio:%3.2f [%d packets needed]", p, src, dst, cmp_size, cr, num_packets);

   int start_byte = 0;
   for (int packet_num=0; packet_num < num_packets; packet_num++)
   {
      int packet_data_size = PACKET_PAYLOAD_CHUNK_SIZE; // default size
      if (start_byte + packet_data_size > cmp_size) packet_data_size = cmp_size - start_byte; // last piece is smaller

      mLog.log_add_prefixed_textf(LOG_NET_stdf_packets, -1, "tx stdf p:%d piece [%d of %d] [%d to %d] st:%4d sz:%4d slsn:%d\n", p, packet_num+1, num_packets, src, dst, start_byte, packet_data_size, server_lev_seq_num);
      mLog.add_log_net_db_row(LOG_NET_stdf_packets, 0, p,    "tx stdf p:%d piece [%d of %d] [%d to %d] st:%4d sz:%4d slsn:%d", p, packet_num+1, num_packets, src, dst, start_byte, packet_data_size, server_lev_seq_num);




      char data[PACKET_BUFFER_SIZE] = {0}; int pos;
      mPacketBuffer.PacketName(data, pos, "stdf");
      mPacketBuffer.PacketPutInt32(data, pos, packet_num);
      mPacketBuffer.PacketPutInt32(data, pos, num_packets);
      mPacketBuffer.PacketPutInt32(data, pos, start_byte);
      mPacketBuffer.PacketPutInt32(data, pos, packet_data_size);
      mPacketBuffer.PacketPutInt32(data, pos, dst);
      mPacketBuffer.PacketPutInt32(data, pos, src);
      mPacketBuffer.PacketPutInt32(data, pos, server_lev_seq_num);
      mPacketBuffer.PacketPutInt32(data, pos, mLevel.last_level_loaded);

      memcpy(data+pos, cmp+start_byte, packet_data_size);
      pos += packet_data_size;

      serverSendTo(data, pos, p);

      start_byte+=PACKET_PAYLOAD_CHUNK_SIZE;
   }
}


void mwNetgame::server_insert_status()
{
   double t0 = al_get_time();

   // start transaction
   char* messageError;
   if (sqlite3_exec(mSql.db_status, "BEGIN TRANSACTION;", nullptr, nullptr, &messageError) != SQLITE_OK) printf("Error: %s\n", messageError);

   std::string ts = mMiscFnx.timestamp("%Y%m%d-%H%M%S");

   sqlite3_reset    (mSql.server_status_insert_stmt);
   sqlite3_bind_text(mSql.server_status_insert_stmt, 1, ts.c_str(),     -1, SQLITE_TRANSIENT);
   sqlite3_bind_int (mSql.server_status_insert_stmt, 2, mLoop.frame_num);
   sqlite3_bind_int (mSql.server_status_insert_stmt, 3, mLevel.play_level);
   sqlite3_bind_int (mSql.server_status_insert_stmt, 4, mGameMoves.entry_pos);
   sqlite3_bind_int (mSql.server_status_insert_stmt, 5, mEnemy.num_enemy);
   sqlite3_bind_int (mSql.server_status_insert_stmt, 6, al_get_time());
   if (sqlite3_step(mSql.server_status_insert_stmt) != SQLITE_ROW)
   {
      printf("sql error, expecting row with newly inserted id: %s\n", sqlite3_errmsg(mSql.db_status));
      return;
   }

   int id = sqlite3_column_int(mSql.server_status_insert_stmt, 0);

   // step to done
   sqlite3_step(mSql.server_status_insert_stmt);


//   printf("id:%d sql:%s\n", id, sql);

   // insert active players into client_status with ss_id from server_status
   for (int p=0; p<NUM_PLAYERS; p++)
      if (mPlayer.syn[p].active)
      {

         sqlite3_reset(mSql.client_status_insert_stmt);
         sqlite3_bind_int   (mSql.client_status_insert_stmt, 1, id);
         sqlite3_bind_int   (mSql.client_status_insert_stmt, 2, p);
         sqlite3_bind_text  (mSql.client_status_insert_stmt, 3, mPlayer.syn[p].name,     -1, SQLITE_TRANSIENT);
         sqlite3_bind_text  (mSql.client_status_insert_stmt, 4, mPlayer.loc[p].hostname, -1, SQLITE_TRANSIENT);
         sqlite3_bind_double(mSql.client_status_insert_stmt, 5, mPlayer.loc[p].cpu);
         sqlite3_bind_double(mSql.client_status_insert_stmt, 6, mPlayer.loc[p].pdsync);
         sqlite3_bind_double(mSql.client_status_insert_stmt, 7, mPlayer.loc[p].ping);
         sqlite3_bind_double(mSql.client_status_insert_stmt, 8, mPlayer.loc[p].client_loc_plr_cor);
         sqlite3_bind_double(mSql.client_status_insert_stmt, 9, mPlayer.loc[p].client_rmt_plr_cor);
         sqlite3_bind_double(mSql.client_status_insert_stmt, 10, mPlayer.loc[p].rewind);
         sqlite3_bind_double(mSql.client_status_insert_stmt, 11, mPlayer.loc[p].cmp_dif_size);
         sqlite3_bind_double(mSql.client_status_insert_stmt, 12, mPlayer.loc[p].tx_bytes_per_tally);

         if (sqlite3_step(mSql.client_status_insert_stmt) != SQLITE_DONE) printf("Error: %s\n", sqlite3_errmsg(mSql.db_status));

//         printf(" p:%d sql:%s\n", p, sql);
      }

   if (sqlite3_exec(mSql.db_status, "COMMIT TRANSACTION;", nullptr, nullptr, &messageError) != SQLITE_OK) printf("Error: %s\n", messageError);

   printf("time to insert:%f\n", (al_get_time()-t0)*1000);
}




// called every 1s from loop
void mwNetgame::server_insert_status_row() // inserts row into status table
{

   std::string ts = mMiscFnx.timestamp("%Y%m%d-%H%M%S");
   int upt = al_get_time();
   int cli = server_num_clients;
   int lev = mLevel.play_level;
   int lvt = mLoop.frame_num;
   int mov = mGameMoves.entry_pos;
   int enm = mEnemy.num_enemy;
   int cpu = mRollingAverage[0].avg;

   char sql[1024];
   sprintf(sql, "INSERT INTO status VALUES ('%s', '%s', %d, %d, %d, %d, %d, %d, %d)",ts.c_str(), PM_VERSION, upt, cpu, cli, lev, lvt, mov, enm );
   //printf("%s\n", sql);
   mSql.execute_sql(sql, mSql.db_sessions);
}

void mwNetgame::server_process_db_control()
{
   // read from db and check if changed

   // check if key exists in db




   // sprintf(sql, "SELECT EXISTS(SELECT 1 FROM control WHERE key='%s'", key);
   // if (!mSql.execute_sql_and_return_one_int(sql, mSql.db_sessions))
   // {
   //    sprintf(sql, "INSERT INTO control VALUES (NULL, '%s', %d, 0", key, mPlayer.syn[0].player_vs_player_shots);
   //    mSql.execute_sql(sql, mSql.db_sessions);
   // }
   //
   // // check if changed by web
   // sprintf(sql, "SELECT val, mod FROM control WHERE key='%s'", key);


   const char* key = "pvp_shots";
   char sql[1024];
   sprintf(sql, "SELECT val, mod FROM control WHERE key='%s'", key);

   std::vector<int> v;
   int ret = mSql.execute_sql_and_return_first_row_as_vector_int(sql, mSql.db_sessions, v);
   if (ret == 2) printf("error: %s\n", sql);
   if (ret == 1)
   {
      printf("no results : %s\n", sql);
      sprintf(sql, "INSERT INTO control VALUES (NULL, '%s', %d, 0", key, mPlayer.syn[0].player_vs_player_shots);
      mSql.execute_sql(sql, mSql.db_sessions);
   }

   if (ret == 0)
   {
//      int val =  v[0];
      int mod =  v[1];

      if (mod == 1) // changed by web
      {
         // do something

      }

   }
   /*

   int new_pvp = mPlayer.syn[0].player_vs_player_shots;


   strcpy(sql, "CREATE TABLE IF NOT EXISTS control( \
               id          INTEGER PRIMARY KEY, \
               key         TEXT, \
               val         INT, \
               mod         INT ); ");

  */



}





void mwNetgame::server_send_snfo_packet() // send info to remote control
{
   // copy values into loc
   mPlayer.loc[0].srv_frame_num = mLoop.frame_num;
   mPlayer.loc[0].srv_total_game_moves = mGameMoves.entry_pos;
   mPlayer.loc[0].srv_level = mLevel.play_level;
   mPlayer.loc[0].srv_zlib_cmp = mNetgame.zlib_cmp;
   mPlayer.loc[0].srv_extra_packets_num = mNetgame.srv_exp_num;
   mPlayer.loc[0].srv_extra_packets_size = mNetgame.srv_exp_siz;
   mPlayer.loc[0].srv_uptime = al_get_time();
   sprintf(mPlayer.loc[0].srv_version, PM_VERSION);

   mPlayer.loc[0].srv_num_enemy = mEnemy.num_enemy;

   char src[5800];
   char dst[5800];

   int sz=0, offset=0;
   offset += sz; sz = sizeof(mPlayer.syn); memcpy(src + offset, mPlayer.syn, sz);
   offset += sz; sz = sizeof(mPlayer.loc); memcpy(src + offset, mPlayer.loc, sz);

   // compress src to dst
   uLongf destLen= sizeof(dst);
   compress2((Bytef*)dst, (uLongf*)&destLen, (Bytef*)src, sizeof(src), zlib_cmp);
   int dst_size = destLen;

   // break compressed dst into smaller pieces
   int num_packets = (dst_size / PACKET_PAYLOAD_CHUNK_SIZE) + 1;

   float cr = (float)dst_size*100 / (float)5400; // compression ratio
   printf("tx snfo fn:[%d] size:[%d] ratio:[%3.2f] [%d packets needed]\n", mLoop.frame_num, dst_size, cr, num_packets);

   int start_byte = 0;
   for (int packet_num=0; packet_num < num_packets; packet_num++)
   {
      int packet_data_size = PACKET_PAYLOAD_CHUNK_SIZE; // default size
      if (start_byte + packet_data_size > dst_size) packet_data_size = dst_size - start_byte; // last piece is smaller

      //printf("tx snfo piece fn:[%d] packet:[%d of %d]\n", mLoop.frame_num, packet_num+1, num_packets);

      char data[PACKET_BUFFER_SIZE] = {0}; int pos;
      mPacketBuffer.PacketName(data, pos, "snfo");
      mPacketBuffer.PacketPutInt32(data, pos, packet_num);
      mPacketBuffer.PacketPutInt32(data, pos, num_packets);
      mPacketBuffer.PacketPutInt32(data, pos, start_byte);
      mPacketBuffer.PacketPutInt32(data, pos, packet_data_size);
      mPacketBuffer.PacketPutInt32(data, pos, mLoop.frame_num);

      memcpy(data+pos, dst+start_byte, packet_data_size);
      pos += packet_data_size;

      serverSendTo(data, pos, 0);

      start_byte+=PACKET_PAYLOAD_CHUNK_SIZE;
   }
}

void mwNetgame::server_proc_rctl_packet(int i)
{
   server_remote_control_last_rctl_rx_frame = mLoop.frame_num;

   int type = mPacketBuffer.PacketGetInt32(i);
   double val = mPacketBuffer.PacketGetDouble(i);

   if (type == PM_RCTL_PACKET_TYPE_client_offset_adj)
   {
      mPlayer.syn[0].client_chase_offset += val;
      mConfig.save_config(PM_CFG_SAVE_NETGAME_CLIENT_CHASE_OFFSET);
   }

   if (type == PM_RCTL_PACKET_TYPE_zlib_compression_adj)
   {
      mNetgame.zlib_cmp += val;
      if (mNetgame.zlib_cmp < 0)  mNetgame.zlib_cmp = 0;
      if (mNetgame.zlib_cmp > 9)  mNetgame.zlib_cmp = 9;
   }

   if (type == PM_RCTL_PACKET_TYPE_extra_packet_num_adj)
   {
      mNetgame.srv_exp_num += val;
      if (mNetgame.srv_exp_num < 0)    mNetgame.srv_exp_num = 0;
      if (mNetgame.srv_exp_num > 500)  mNetgame.srv_exp_num = 500;
   }

   if (type == PM_RCTL_PACKET_TYPE_extra_packet_siz_adj)
   {
      mNetgame.srv_exp_siz += val;
      if (mNetgame.srv_exp_siz < 0)     mNetgame.srv_exp_siz = 0;
      if (mNetgame.srv_exp_siz > 1000)  mNetgame.srv_exp_siz = 1000;
   }


   // ----------------------------------------------------------------------------------------
   // shot config adjustments - do not apply directly, add a game move one frame in the future
   // ----------------------------------------------------------------------------------------
   int shot_config_change = 0;
   int new_dmg = mPlayer.syn[0].player_vs_player_shot_damage;
   int new_pvp = mPlayer.syn[0].player_vs_player_shots;
   int new_pvs = mPlayer.syn[0].player_vs_self_shots;

   if (type == PM_RCTL_PACKET_TYPE_pvp_shot_damage_adj)
   {
      shot_config_change = 1;
      new_dmg += val;
      if (new_dmg < -100) new_dmg = -100;
      if (new_dmg >  100) new_dmg =  100;
   }
   if (type == PM_RCTL_PACKET_TYPE_pvp_shots_toggle)
   {
      shot_config_change = 1;
      new_pvp = !new_pvp;
   }
   if (type == PM_RCTL_PACKET_TYPE_pvs_shots_toggle)
   {
      shot_config_change = 1;
      new_pvs = !new_pvs;
   }
   if (shot_config_change)
   {
      int sc = 0;
      if (new_pvp) sc |= 0b01;
      if (new_pvs) sc |= 0b10;
      mGameMoves.add_game_move2(mLoop.frame_num+1, PM_GAMEMOVE_TYPE_SHOT_CONFIG, sc, new_dmg);
      mConfig.save_config(PM_CFG_SAVE_NETGAME_SHOTS);
   }
   if (type == PM_RCTL_PACKET_TYPE_server_reload) server_reload((int)val);
   if (type == PM_RCTL_PACKET_TYPE_fakekey_toggle) mPlayer.syn[0].server_force_fakekey = !mPlayer.syn[0].server_force_fakekey;
   if (type == PM_RCTL_PACKET_TYPE_force_client_offset) mPlayer.syn[0].server_force_client_offset = !mPlayer.syn[0].server_force_client_offset;
}

void mwNetgame::server_proc_stak_packet(int i)
{
   double timestamp = mPacketBuffer.rx_buf[i].timestamp;

   int slsn                          = mPacketBuffer.PacketGetInt32(i);
   int p                             = mPacketBuffer.PacketGetInt32(i);
   int ack_frame_num                 = mPacketBuffer.PacketGetInt32(i);  // client has acknowledged getting and applying this base
   int client_frame_num              = mPacketBuffer.PacketGetInt32(i);

   char log_msg_txt1[128];
   sprintf(log_msg_txt1, "rx stak p:%d ack:[%d] cur:[%d] slsn:[%d]", p, ack_frame_num, client_frame_num, slsn);


   if (slsn != server_lev_seq_num)
   {
      mLog.log_add_prefixed_textf(LOG_NET_stak, -1, "%s - wrong slsn:[%d] - ignoring\n", log_msg_txt1, server_lev_seq_num);
      mLog.add_log_net_db_row(LOG_NET_stak, 0, p,    "%s - wrong slsn:[%d] - ignoring", log_msg_txt1, server_lev_seq_num);
      return;
   }


   mPlayer.loc[p].client_chase_fps   = mPacketBuffer.PacketGetDouble(i);
   mPlayer.loc[p].pdsync             = mPacketBuffer.PacketGetDouble(i);
   mPlayer.loc[p].rewind             = mPacketBuffer.PacketGetInt32(i);
   mPlayer.loc[p].client_loc_plr_cor = mPacketBuffer.PacketGetDouble(i);
   mPlayer.loc[p].client_rmt_plr_cor = mPacketBuffer.PacketGetDouble(i);
   mPlayer.loc[p].cpu                = mPacketBuffer.PacketGetDouble(i);

   // add lcor and rcor to tally object
   mTally_client_loc_plr_cor_last_sec[p].add_data(mPlayer.loc[p].client_loc_plr_cor);
   mTally_client_rmt_plr_cor_last_sec[p].add_data(mPlayer.loc[p].client_rmt_plr_cor);

   // calculate stak_sync
   int stak_sync = mLoop.frame_num - mStateHistory[p].newest_state_frame_num;

   // calculate stak_dsync
   mPlayer.loc[p].stak_dsync = ( (double) stak_sync * 0.025) + mLoop.frame_start_timestamp - timestamp;

   // this is used to see if client is still alive
   mPlayer.loc[p].server_last_stak_rx_frame_num = mLoop.frame_num;

   // make sure we have a copy of acknowledged state in history, finds and sets values, or sets to -1 if not found
   mStateHistory[p].set_ack_state(ack_frame_num);

   mLog.log_add_prefixed_textf(LOG_NET_stak, -1, "%s dsync:[%4.1f] chase:[%4.1f]\n", log_msg_txt1, mPlayer.loc[p].pdsync*1000, mPlayer.loc[p].client_chase_fps);
   mLog.add_log_net_db_row(LOG_NET_stak, 0, p,    "%s dsync:[%4.1f] chase:[%4.1f]", log_msg_txt1, mPlayer.loc[p].pdsync*1000, mPlayer.loc[p].client_chase_fps);

}


void mwNetgame::server_proc_player_drop()
{
   if (mLoop.frame_num > 300) // don't even try until this far into a level
   {
      int drop_frame_limit = mLoop.frame_num - 200;

      if (server_remote_control && server_remote_control_last_rctl_rx_frame < drop_frame_limit)
      {
         //printf("Server dropped remote control client due to no response\n");
         server_remote_control = 0;
         // is there any other cleanup I need to do here?
      }



      // check to see if we need to drop clients
      for (int p=1; p<NUM_PLAYERS; p++)
         if ((mPlayer.syn[p].active) && (mPlayer.syn[p].control_method == PM_PLAYER_CONTROL_METHOD_NETGAME_REMOTE) && (mPlayer.loc[p].server_last_stak_rx_frame_num < drop_frame_limit))
         {
            mGameMoves.add_game_move(mLoop.frame_num + 4, PM_GAMEMOVE_TYPE_PLAYER_INACTIVE, p, 71); // make client inactive (reason no stak for x frames)
            session_close(p, 4); // reason 4 - comm loss
            mLog.add_headerf(LOG_NET, -1, 1,       "Server dropped player:%d (last stak rx:%d)", p, mPlayer.loc[p].server_last_stak_rx_frame_num);
            mLog.add_log_net_db_row(LOG_NET, 0, p,  "Server dropped player:%d (last stak rx:%d)", p, mPlayer.loc[p].server_last_stak_rx_frame_num);
         }
   }
}

void mwNetgame::server_proc_limits()
{
   if (mPlayer.syn[0].level_done_mode == 0) // only trigger from level done mode 0
   {
      int reload = 0;

      // if headless server and not overworld and no clients, go back to overworld
      if ((mMain.headless_server) && (mLevel.play_level != 1))
      {
         if (server_num_clients == 0)
         {
            mLog.add_headerf(LOG_NET, -1, 1,       "Headless Server with no clients! - Reload");
            mLog.add_log_net_db_row(LOG_NET, 0, 0,  "Headless Server with no clients! - Reload");
            reload = 1;
         }
      }

      int gm_limit = GAME_MOVES_SIZE - 100;
      if (mGameMoves.entry_pos > gm_limit)
      {
         mLog.add_headerf(LOG_NET, -1, 1,       "Server Approaching %d Game Moves! - Reload", gm_limit);
         mLog.add_log_net_db_row(LOG_NET, 0, 0,  "Server Approaching %d Game Moves! - Reload", gm_limit);


         reload = 1;
      }

      int tm_limit = 40 * 60 * 60; // 40 fps x 60s x 60m = 144000 = 1h
      if (mLoop.frame_num > tm_limit)
      {
         mLog.add_headerf(LOG_NET, -1, 1,       "Server Approaching %d frames! - Reload", tm_limit);

         mLog.add_log_net_db_row(LOG_NET, 0, 0,  "Server Approaching %d frames! - Reload", tm_limit);

         reload = 1;
      }
      if (reload) server_reload(1); // to overworld
   }
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


void mwNetgame::server_proc_cdat_packet(int i)
{
   int slsn           = mPacketBuffer.PacketGetInt32(i);
   int p              = mPacketBuffer.PacketGetInt32(i);
   int cdat_frame_num = mPacketBuffer.PacketGetInt32(i);
   int cm             = mPacketBuffer.PacketGetInt32(i);
   double timestamp   = mPacketBuffer.rx_buf[i].timestamp;

   char log_msg_txt1[128];
   sprintf(log_msg_txt1, "rx cdat p:%d fn:[%d] sync:[%d] slsn:[%d]", p, cdat_frame_num, mPlayer.loc[p].server_game_move_sync, slsn);

   // reject cdats if server_lev_seq_num does not match
   if (slsn != server_lev_seq_num)
   {
      mLog.log_add_prefixed_textf(LOG_NET_cdat, -1, "%s - wrong slsn:[%d] - dropped\n", log_msg_txt1, server_lev_seq_num);
      mLog.add_log_net_db_row(LOG_NET_cdat, 0, p,    "%s - wrong slsn:[%d] - dropped", log_msg_txt1, server_lev_seq_num);
      return;
   }

   // determine the cutoff frame for late cdats
   int of = mStateHistory[0].oldest_state_frame_num;
   if (cdat_frame_num < of)
   {
      mPlayer.syn[p].late_cdats++;
      mTally_late_cdats_last_sec[p].add_data(1); // add to tally
      mLog.log_add_prefixed_textf(LOG_NET_cdat, -1, "%s late - dropped\n", log_msg_txt1);
      mLog.add_log_net_db_row(LOG_NET_cdat, 0, p,    "%s late - dropped", log_msg_txt1);
      return;
   }

   // add to game_move array
   mGameMoves.add_game_move(cdat_frame_num, PM_GAMEMOVE_TYPE_PLAYER_MOVE, p, cm);

   // update server dirty frame if cdat is earlier
   if (cdat_frame_num < server_dirty_frame) server_dirty_frame = cdat_frame_num;


   // calculate game_move_sync
   mPlayer.loc[p].server_game_move_sync = cdat_frame_num - mLoop.frame_num;

   // calculate game_move_dsync
   mPlayer.loc[p].game_move_dsync = ( (double) mPlayer.loc[p].server_game_move_sync * 0.025) + mLoop.frame_start_timestamp - timestamp;

   // add to average tally
   mTally_game_move_dsync_avg_last_sec[p].add_data(mPlayer.loc[p].game_move_dsync);

   mPlayer.loc[p].client_cdat_packets_tx++;

   mLog.log_add_prefixed_textf(LOG_NET_cdat, -1, "%s gmep:[%d] - entered\n", log_msg_txt1, mGameMoves.entry_pos);
   mLog.add_log_net_db_row(LOG_NET_cdat, 0, p,    "%s gmep:[%d] - entered", log_msg_txt1, mGameMoves.entry_pos);
}




// this send function is different because if server full, there is no client channel address setup, so we send sjon directly to the cjon address
void mwNetgame::server_send_sjon_packet(char* address, int level, int frame, int player_num, int player_color)
{
   char data[PACKET_BUFFER_SIZE] = {0}; int pos;
   mPacketBuffer.PacketName(data, pos, "sjon");
   mPacketBuffer.PacketPutInt32(data, pos, level);
   mPacketBuffer.PacketPutInt32(data, pos, frame);
   mPacketBuffer.PacketPutInt32(data, pos, player_num);
   mPacketBuffer.PacketPutInt32(data, pos, player_color);
   mPacketBuffer.PacketPutInt32(data, pos, server_lev_seq_num);

   if (net_assigntarget(Channel, address))
   {
      char msg[512];
      sprintf(msg, "Error: couldn't assign target `%s' to Channel\n", address);
      mLog.log_error(msg, 0);
   }
   net_send(Channel, data, pos);
}

void mwNetgame::server_proc_cjon_packet(char *data, char * address)
{
   int pos = 4;
   char playername[9];
   char hostname[17];
   int color  = mPacketBuffer.PacketGetInt32(data, pos);
   mPacketBuffer.PacketReadStringN(data, pos, playername);
   mPacketBuffer.PacketReadStringN(data, pos, hostname);
   mLog.add_fwf(LOG_NET, -1, 76, 10, "+", "-", "");
   mLog.add_fwf(LOG_NET, -1, 76, 10, "|", " ", "Server received join request from %s requesting color:%d", hostname, color);
   mLog.add_log_net_db_row(LOG_NET, 0, 0,       "Server received join request from %s requesting color:%d", hostname, color);

   int p = mPlayer.find_inactive_player();
   if (p == 99) // no inactive player found
   {
      // send sjon reply
      server_send_sjon_packet(address, 0, 0, 99, 0);

      // add session log
      session_add(address, hostname, 0, 1);

      mLog.add_fwf(LOG_NET, -1, 76, 10, "|", " ", "Reply sent: 'SERVER FULL'");
      mLog.add_log_net_db_row(LOG_NET, 0, 0,       "Reply sent: 'SERVER FULL'");

      mLog.add_fwf(LOG_NET, -1, 76, 10, "+", "-", "");
   }
   else // inactive player found, proceed with join
   {
      // initialize player
      mPlayer.init_player(p, 1);

      // set channel address
      mwChannels[p].channel_active = 1;
      strcpy(mwChannels[p].channel_address, address);

      // copy hostname to player struct
      strcpy(mPlayer.loc[p].hostname, hostname);

      // copy playername to player struct
      strcpy(mPlayer.syn[p].name, playername);

      // initialize state history
      mStateHistory[p].initialize();

      // to prevent dropping newly added player for the first 5s
      mPlayer.loc[p].server_last_stak_rx_frame_num = mLoop.frame_num + 200;

      // keep track of when client joined
      mPlayer.loc[p].join_frame = mLoop.frame_num;

      // try to use requested color, unless already used by another player
      color = mPlayer.get_new_client_color(color);

      // set the last touched gate level
      mPlayer.syn[p].overworld_last_touched_gate = mLevel.play_level;

      // add game move to make client active and set color
      mGameMoves.add_game_move(mLoop.frame_num, PM_GAMEMOVE_TYPE_PLAYER_ACTIVE, p, color);

      // game move will skip being applied because it's set to current frame, so force it to be applied right now
      mGameMoves.proc_game_move_player_active(p, color);

      // send sjon reply
      server_send_sjon_packet(address, mLevel.play_level, mLoop.frame_num, p, color);

      // start session log
      session_add(address, hostname, p, 0);



      mLog.add_fwf(LOG_NET,  -1, 76, 10, "|", " ", "Server replied with join invitation:");
      mLog.add_fwf(LOG_NET,  -1, 76, 10, "|", " ", "Level:[%d]", mLevel.play_level);
      mLog.add_fwf(LOG_NET,  -1, 76, 10, "|", " ", "Player Number:[%d]", p);
      mLog.add_fwf(LOG_NET,  -1, 76, 10, "|", " ", "Player Color:[%d]", color);
      mLog.add_fwf(LOG_NET,  -1, 76, 10, "|", " ", "Server Frame:[%d]", mLoop.frame_num);
      mLog.add_fwf(LOG_NET,  -1, 76, 10, "|", " ", "Server Level Sequence Num:[%d]", server_lev_seq_num);
      mLog.add_fwf(LOG_NET,  -1, 76, 10, "+", "-", "");

      mLog.add_log_net_db_row(LOG_NET, 0, p,       "Server replied with join invitation:");
      mLog.add_log_net_db_row(LOG_NET, 0, p,       "Level:[%d] Frame:[%d] SLSN:[%d]", mLevel.play_level, mLoop.frame_num, server_lev_seq_num);
      mLog.add_log_net_db_row(LOG_NET, 0, p,       "Player Number:[%d] Player Color:[%d]", p, color);


   }
}

void mwNetgame::server_proc_cjrc_packet(char *data, char * address)
{
   // set up channel
   mwChannels[0].channel_active = 1;
   strcpy(mwChannels[0].channel_address, address);

   mLog.add_fwf(LOG_NET, -1, 76, 10, "|", " ", "Server received remote control request");
   mLog.add_fwf(LOG_NET, -1, 76, 10, "|", " ", "Server replied with sjrc packet");

   server_remote_control = 1;
   server_remote_control_last_rctl_rx_frame = mLoop.frame_num;


   server_send_sjrc_packet(0);
}


void mwNetgame::server_send_sjrc_packet(int p)
{
   char data[PACKET_BUFFER_SIZE] = {}; int pos;
   mPacketBuffer.PacketName(data, pos, "sjrc");
   mPacketBuffer.PacketPutDouble(data, pos, 0);
   serverSendTo(data, pos, p);
}

void mwNetgame::server_send_srrf_packet(int p, int val)
{
   // printf("sending srrf packet with val:%d\n", val);
   char data[PACKET_BUFFER_SIZE] = {}; int pos;
   mPacketBuffer.PacketName(data, pos, "srrf");
   mPacketBuffer.PacketPutInt32(data, pos, val);
   serverSendTo(data, pos, p);
}


void mwNetgame::server_proc_ping_packet(char *data, int p)
{
   int pos = 4;
   double t0 = mPacketBuffer.PacketGetDouble(data, pos);
   double t1 = al_get_time();
   mPacketBuffer.PacketName(data, pos, "pong");
   mPacketBuffer.PacketPutDouble(data, pos, t0);
   mPacketBuffer.PacketPutDouble(data, pos, t1);
   serverSendTo(data, pos, p);
}

void mwNetgame::server_proc_pang_packet(char *data, int p)
{
   int pos = 4;
   double t0 = mPacketBuffer.PacketGetDouble(data, pos);
   mPacketBuffer.RA[p].add_data(al_get_time() - t0);
}


void mwNetgame::server_proc_clog_packet(int i)
{
   int type          = mPacketBuffer.PacketGetInt32(i);
   int sub_type      = mPacketBuffer.PacketGetInt32(i);
   int f             = mPacketBuffer.PacketGetInt32(i);
   double agt        = mPacketBuffer.PacketGetDouble(i);

   char msg[200];
   mPacketBuffer.PacketReadStringN(i, msg);

   int p = mPacketBuffer.rx_buf[i].p; // get player number from packet buffer info

   //printf ("Server rx clog - type:%d sub:%d f:%d agt:%f, msg:[%s]\n", type, sub_type, f, agt, msg);

   mLog.add_log_net_db_row2(type, sub_type, agt, f, p, 0, msg);
}



void mwNetgame::server_count_clients()
{
   server_num_clients = 0;
   for (int p=1; p<NUM_PLAYERS; p++)
      if (mPlayer.syn[p].active) server_num_clients++;
}



void mwNetgame::server_control()
{
   mPacketBuffer.proc_rx_buffer();        // process all packets in buffer
   server_rewind();                       // to replay and apply late client input
   server_proc_player_drop();             // check to see if we need to drop inactive clients
   server_proc_limits();                  // check to see if we need to reload level
   server_proc_files_to_send();

   session_check_active();




   if (server_remote_control) server_send_snfo_packet();

   server_insert_status();

   // send extra packets (testing and debug only)
   for (int i=0; i<srv_exp_num; i++)
      for (int p=1; p<NUM_PLAYERS; p++)
         if ((mPlayer.syn[p].active) && (mPlayer.syn[p].control_method == PM_PLAYER_CONTROL_METHOD_NETGAME_REMOTE))
         {
            char data[PACKET_BUFFER_SIZE] = {0}; int pos;
            mPacketBuffer.PacketName(data, pos, "extr");
            serverSendTo(data, pos + srv_exp_siz, p);
         }




   mLog.add_log_status_db_rows();

   for (int p=0; p<NUM_PLAYERS; p++) if (mPlayer.syn[p].active) process_bandwidth_counters(p);

   server_count_clients();


}
