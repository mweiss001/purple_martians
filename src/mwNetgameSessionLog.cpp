// mwNetgameSessionLog.cpp

#include "pm.h"
#include "mwNetgame.h"
#include "mwPlayer.h"
#include "mwMiscFnx.h"
#include "mwLog.h"
#include "mwSql.h"
#include "mwGameMoves.h"




void mwNetgame::session_close(int p, int er)
{
   if (!mLog.log_types[LOG_NET_session].action) return;
   char endreason[256];
   if (er == 0) sprintf(endreason, "unknown");
   if (er == 1) sprintf(endreason, "server_full");
   if (er == 2) sprintf(endreason, "client_quit");
   if (er == 4) sprintf(endreason, "comm_loss");
   if (er == 6) sprintf(endreason, "inactive");
   if (er == 8) sprintf(endreason, "server quit");

   session_update(p, endreason);
   mPlayer.loc[p].session_id = 0;
}

// this is called every frame from server_control() to check and clean up active sessions
void mwNetgame::session_check_active()
{
   if (!mLog.log_types[LOG_NET_session].action) return;
   for (int p=1; p<8; p++)
      if ((mPlayer.loc[p].session_id) && (!mPlayer.syn[p].active)) session_close(p, 6); // session still active, but player is not active anymore
}

void mwNetgame::session_flush_active_at_server_exit()
{
   if (!mLog.log_types[LOG_NET_session].action) return;
   for (int p=1; p<8; p++)
      if (mPlayer.loc[p].session_id)
         session_close(p, 8);
}

void mwNetgame::session_save_active_at_level_done()
{
   if (!mLog.log_types[LOG_NET_session].action) return;
   for (int p=1; p<8; p++)
      if (mPlayer.loc[p].session_id)
      {
         mPlayer.syn[p].stat_next_levels++;
         if (p == mPlayer.syn[0].level_done_player) mPlayer.syn[p].stat_exits++;
         session_update(p);
      }
}

void mwNetgame::session_add(const char* address, const char* hostname, int p, int endreason)
{
   if (!mLog.log_types[LOG_NET_session].action) return;
   // get timestamp
   struct tm *timenow;
   time_t now = time(nullptr);
   timenow = localtime(&now);
   char ts[20];
   strftime(ts, sizeof(ts), "%Y%m%d-%H%M%S", timenow);


   // make a copy of address so we can play with it
   char ad[32];
   strcpy(ad, address);
   char ip[16];
   // split address at ':'  everything before is ip, everything after is port
   int c;
   for (c=0; c<(int)strlen(ad); c++)
      if (ad[c] == ':')
      {
         strncpy(ip, ad, c);
         ip[c] = 0;
         break;
      }
   mMiscFnx.chop_first_x_char(ad, c+1);
   int port = atoi(ad);

   char sql[500];
   sprintf(sql,
   "INSERT INTO sessions (dt_start, ip, port, hostname, player_num, player_color,         player_name, endreason) VALUES('%s', '%s', %d, '%s', %d, %d, '%s', 'open') RETURNING id;" ,
                          ts,       ip, port, hostname, p,          mPlayer.syn[p].color, mPlayer.syn[p].name);

   mPlayer.loc[p].session_id = mSql.execute_sql_and_return_one_int(sql, mSql.db_sessions);

   mGameMoves.create_gm_session_link(mPlayer.loc[p].session_id);

   // close immediatley if server full endreason
   if (endreason == 1) session_close(p, endreason);
}



#include <iomanip>

// called only when closing session or level done
// adds all the player tallies, bandwidth tallies
// if passed nullptr for endreason, session is still open
void mwNetgame::session_update(int p, char * m_endreason)
{
   if (!mLog.log_types[LOG_NET_session].action) return;


   int sid = mPlayer.loc[p].session_id;
   if (!sid)
   {
      printf("session_update_sql(%d, %s) - session_id is zero! exiting\n", p, m_endreason);
      return;
   }

   char endreason[128];
   strcpy(endreason, "open");
   if (m_endreason != nullptr) strcpy(endreason, m_endreason);


   int cdats_rx=0;
   int next_levels=0;
   int exits=0;
   int respawns=0;
   int shots_fired=0;
   int enemy_hits=0;
   int player_hits=0;
   int self_hits=0;
   int purple_coins=0;

   int tx_bytes_total=0;
   int tx_bytes_avg_per_sec=0;
   int tx_bytes_max_per_frame=0;
   int rx_bytes_total=0;
   int rx_bytes_avg_per_sec=0;
   int rx_bytes_max_per_frame=0;
   int tx_packets_total=0;
   int tx_packets_avg_per_sec=0;
   int tx_packets_max_per_frame=0;
   int rx_packets_total=0;
   int rx_packets_avg_per_sec=0;
   int rx_packets_max_per_frame=0;

   char dt_start[80];

   // prepare statement
   sqlite3_stmt* stmt;
   const char* sql = "SELECT \
                        cdat_rx                   , \
                        next_levels               , \
                        exits_activated           , \
                        respawns                  , \
                        shots_fired               , \
                        enemy_hits                , \
                        player_hits               , \
                        self_hits                 , \
                        purple_coins              , \
                        tx_bytes_total            , \
                        tx_bytes_avg_per_sec      , \
                        tx_bytes_max_per_frame    , \
                        rx_bytes_total            , \
                        rx_bytes_avg_per_sec      , \
                        rx_bytes_max_per_frame    , \
                        tx_packets_total          , \
                        tx_packets_avg_per_sec    , \
                        tx_packets_max_per_frame  , \
                        rx_packets_total          , \
                        rx_packets_avg_per_sec    , \
                        rx_packets_max_per_frame  , \
                        dt_start FROM sessions WHERE id=?";

   sqlite3_prepare_v2(mSql.db_sessions, sql, -1, &stmt, nullptr);

   // bind parameters
   sqlite3_bind_int(stmt, 1, sid);

   // execute and step through results
   int step_result = sqlite3_step(stmt);
   if (step_result == SQLITE_ROW)
   {
      cdats_rx                    = sqlite3_column_int(stmt, 0);
      next_levels                 = sqlite3_column_int(stmt, 1);
      exits                       = sqlite3_column_int(stmt, 2);
      respawns                    = sqlite3_column_int(stmt, 3);
      shots_fired                 = sqlite3_column_int(stmt, 4);
      enemy_hits                  = sqlite3_column_int(stmt, 5);
      player_hits                 = sqlite3_column_int(stmt, 6);
      self_hits                   = sqlite3_column_int(stmt, 7);
      purple_coins                = sqlite3_column_int(stmt, 8);
      tx_bytes_total              = sqlite3_column_int(stmt, 9);
      tx_bytes_avg_per_sec        = sqlite3_column_int(stmt, 10);
      tx_bytes_max_per_frame      = sqlite3_column_int(stmt, 11);
      rx_bytes_total              = sqlite3_column_int(stmt, 12);
      rx_bytes_avg_per_sec        = sqlite3_column_int(stmt, 13);
      rx_bytes_max_per_frame      = sqlite3_column_int(stmt, 14);
      tx_packets_total            = sqlite3_column_int(stmt, 15);
      tx_packets_avg_per_sec      = sqlite3_column_int(stmt, 16);
      tx_packets_max_per_frame    = sqlite3_column_int(stmt, 17);
      rx_packets_total            = sqlite3_column_int(stmt, 18);
      rx_packets_avg_per_sec      = sqlite3_column_int(stmt, 19);
      rx_packets_max_per_frame    = sqlite3_column_int(stmt, 20);
      sprintf(dt_start, "%s", sqlite3_column_text(stmt, 21));
   }
   else if (step_result == SQLITE_DONE) printf("No row found for the given ID.\n");
   else fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(mSql.db_sessions));
   sqlite3_finalize(stmt);

   // now that I have all that....

   // create at tm struct from start date
   struct tm timestart = {};
   std::stringstream ss(dt_start);
   ss >> std::get_time(&timestart, "%Y%m%d-%H%M%S");
   if (ss.fail()) printf("Error parsing time\n");
   time_t st = mktime(&timestart);

   char ts[20];
   strftime(ts, sizeof(ts), "%Y%m%d-%H%M%S", &timestart);

   // create a tm struct from now
   time_t now = time(nullptr);
   struct tm *timenow;
   timenow = localtime(&now);
   char dt_end[20];
   strftime(dt_end, sizeof(dt_end), "%Y%m%d-%H%M%S", timenow);

   int duration = difftime(now, st);

   //printf("update session id:%d  ts:%s te:%s dur:%d\n", sid, ts, dt_end, duration);

   cdats_rx     += mPlayer.loc[p].client_cdat_packets_tx;
   respawns     += mPlayer.syn[p].stat_respawns;
   shots_fired  += mPlayer.syn[p].stat_shots_fired;
   enemy_hits   += mPlayer.syn[p].stat_enemy_hits;
   player_hits  += mPlayer.syn[p].stat_player_hits;
   self_hits    += mPlayer.syn[p].stat_self_hits;
   purple_coins += mPlayer.syn[p].stat_purple_coins;
   next_levels  += mPlayer.syn[p].stat_next_levels;
   exits        += mPlayer.syn[p].stat_exits;

   tx_bytes_total     += mPlayer.loc[p].tx_total_bytes;
   tx_packets_total   += mPlayer.loc[p].tx_total_packets;
   rx_bytes_total     += mPlayer.loc[p].rx_total_bytes;
   rx_packets_total   += mPlayer.loc[p].rx_total_packets;

   if (mPlayer.loc[p].tx_max_bytes_per_frame   > tx_bytes_max_per_frame)   tx_bytes_max_per_frame   = mPlayer.loc[p].tx_max_bytes_per_frame;
   if (mPlayer.loc[p].rx_max_bytes_per_frame   > rx_bytes_max_per_frame)   rx_bytes_max_per_frame   = mPlayer.loc[p].rx_max_bytes_per_frame;
   if (mPlayer.loc[p].tx_max_packets_per_frame > tx_packets_max_per_frame) tx_packets_max_per_frame = mPlayer.loc[p].tx_max_packets_per_frame;
   if (mPlayer.loc[p].rx_max_packets_per_frame > rx_packets_max_per_frame) rx_packets_max_per_frame = mPlayer.loc[p].rx_max_packets_per_frame;

   // average per seconds = totals / duration

   if (duration > 0)
   {
      tx_bytes_avg_per_sec   = tx_bytes_total   / duration;
      rx_bytes_avg_per_sec   = rx_bytes_total   / duration;
      tx_packets_avg_per_sec = tx_packets_total / duration;
      rx_packets_avg_per_sec = rx_packets_total / duration;
   }

   // now update sessions with all this data

   // prepare statement
   const char* sql2 ="UPDATE sessions SET \
      cdat_rx                  =?,\
      next_levels              =?,\
      exits_activated          =?,\
      respawns                 =?,\
      shots_fired              =?,\
      enemy_hits               =?,\
      player_hits              =?,\
      self_hits                =?,\
      purple_coins             =?,\
      tx_bytes_total           =?,\
      tx_bytes_avg_per_sec     =?,\
      tx_bytes_max_per_frame   =?,\
      rx_bytes_total           =?,\
      rx_bytes_avg_per_sec     =?,\
      rx_bytes_max_per_frame   =?,\
      tx_packets_total         =?,\
      tx_packets_avg_per_sec   =?,\
      tx_packets_max_per_frame =?,\
      rx_packets_total         =?,\
      rx_packets_avg_per_sec   =?,\
      rx_packets_max_per_frame =?,\
      duration                 =?,\
      dt_end                   =?,\
      endreason                =? WHERE id=?";

   sqlite3_prepare_v2(mSql.db_sessions, sql2, -1, &stmt, nullptr);

   // bind parameters

   sqlite3_bind_int(stmt,   1,  cdats_rx);
   sqlite3_bind_int(stmt,   2,  next_levels);
   sqlite3_bind_int(stmt,   3,  exits);
   sqlite3_bind_int(stmt,   4,  respawns);
   sqlite3_bind_int(stmt,   5,  shots_fired);
   sqlite3_bind_int(stmt,   6,  enemy_hits);
   sqlite3_bind_int(stmt,   7,  player_hits);
   sqlite3_bind_int(stmt,   8,  self_hits);
   sqlite3_bind_int(stmt,   9,  purple_coins);
   sqlite3_bind_int(stmt,  10,  tx_bytes_total);
   sqlite3_bind_int(stmt,  11,  tx_bytes_avg_per_sec);
   sqlite3_bind_int(stmt,  12,  tx_bytes_max_per_frame);
   sqlite3_bind_int(stmt,  13,  rx_bytes_total);
   sqlite3_bind_int(stmt,  14,  rx_bytes_avg_per_sec);
   sqlite3_bind_int(stmt,  15,  rx_bytes_max_per_frame);
   sqlite3_bind_int(stmt,  16,  tx_packets_total);
   sqlite3_bind_int(stmt,  17,  tx_packets_avg_per_sec);
   sqlite3_bind_int(stmt,  18,  tx_packets_max_per_frame);
   sqlite3_bind_int(stmt,  19,  rx_packets_total);
   sqlite3_bind_int(stmt,  20,  rx_packets_avg_per_sec);
   sqlite3_bind_int(stmt,  21,  rx_packets_max_per_frame);
   sqlite3_bind_int(stmt,  22,  duration);
   sqlite3_bind_text(stmt, 23,  dt_end, -1, SQLITE_TRANSIENT);
   sqlite3_bind_text(stmt, 24,  endreason, -1, SQLITE_TRANSIENT);
   sqlite3_bind_int(stmt,  25,  sid);

   // execute and step through results
   step_result = sqlite3_step(stmt);

}




