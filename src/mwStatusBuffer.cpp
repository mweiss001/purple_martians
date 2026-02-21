// mwStatusBuffer.cpp

#include "pm.h"
#include "mwStatusBuffer.h"

#include "mwEnemy.h"
#include "mwGameMoves.h"
#include "mwLevel.h"
#include "mwLoop.h"
#include "mwPlayer.h"
#include "mwSql.h"

mwStatusBuffer mStatusBuffer;

mwStatusBuffer::mwStatusBuffer()
{
   init();
}

void mwStatusBuffer::init()
{
   beginTransactionError = 0;
   server_status_buffer_rows.clear();
   client_status_buffer_rows.clear();
}


void mwStatusBuffer::add()
{
   // get timestamp as msec since epoch
   std::uint64_t timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
   int frame = mLoop.frame_num;

   // add row for server
   struct server_status_buffer_row row = { timestamp, frame, mLevel.play_level, mGameMoves.entry_pos, mEnemy.num_enemy, (int) al_get_time() };
   server_status_buffer_rows.push_back(row);

   // add rows for clients
   for (int p=0; p<NUM_PLAYERS; p++)
      if (mPlayer.syn[p].active)
      {
         int color = mPlayer.syn[p].color;
         std::string name = mPlayer.syn[p].name;
         std::string hostname = mPlayer.loc[p].hostname;
         int cpu = mPlayer.loc[p].cpu;
         int sync = mPlayer.loc[p].pdsync*1000;
         int ping = mPlayer.loc[p].ping*1000;
         int lcor = mPlayer.loc[p].client_loc_plr_cor;
         int rcor = mPlayer.loc[p].client_rmt_plr_cor;
         int rwnd = mPlayer.loc[p].rewind;
         int difs = mPlayer.loc[p].cmp_dif_size;
         int tkbs = mPlayer.loc[p].tx_bytes_per_tally;
         struct client_status_buffer_row row = {timestamp, frame, p, color, name, hostname, cpu, sync, ping,lcor, rcor, rwnd, difs, tkbs };
         client_status_buffer_rows.push_back(row);
      }

   // periodically dump to database
   if (server_status_buffer_rows.size() > 4)
   {
      char* messageError;

      // start transaction
      if (sqlite3_exec(mSql.db_status, "BEGIN TRANSACTION;", nullptr, nullptr, &messageError) != SQLITE_OK)
      {
         beginTransactionError++;
         printf("Error: %s\n", messageError);

         if (beginTransactionError > 10)
         {
            printf("\n10 begin transaction errors, resetting.\n\n");

            // Abort the transaction and undo all changes since BEGIN TRANSACTION
            sqlite3_exec(mSql.db_status, "ROLLBACK;", 0, 0, 0);

          //  printf("\nClearing %d rows in buffer.\n\n", (int)client_status_buffer_rows.size());

            init();

         }

         return;
      }
      beginTransactionError = 0;

      for (auto row: server_status_buffer_rows)
      {
         sqlite3_reset   (mSql.server_status_insert_stmt);
         sqlite3_bind_int64(mSql.server_status_insert_stmt, 1, row.timestamp);
         sqlite3_bind_int(mSql.server_status_insert_stmt, 2, row.frame);
         sqlite3_bind_int(mSql.server_status_insert_stmt, 3, row.level);
         sqlite3_bind_int(mSql.server_status_insert_stmt, 4, row.moves);
         sqlite3_bind_int(mSql.server_status_insert_stmt, 5, row.enemies);
         sqlite3_bind_int(mSql.server_status_insert_stmt, 6, row.uptime);
         if (sqlite3_step(mSql.server_status_insert_stmt) != SQLITE_DONE) printf("Error: %s\n", sqlite3_errmsg(mSql.db_status));
      }
      for (auto row: client_status_buffer_rows)
      {
         sqlite3_reset    (mSql.client_status_insert_stmt);
         sqlite3_bind_int64 (mSql.client_status_insert_stmt, 1,  row.timestamp);
         sqlite3_bind_int (mSql.client_status_insert_stmt, 2,  row.frame);
         sqlite3_bind_int (mSql.client_status_insert_stmt, 3,  row.p);
         sqlite3_bind_int (mSql.client_status_insert_stmt, 4,  row.color);
         sqlite3_bind_text(mSql.client_status_insert_stmt, 5,  row.name.c_str(),     -1, SQLITE_TRANSIENT);
         sqlite3_bind_text(mSql.client_status_insert_stmt, 6,  row.hostname.c_str(), -1, SQLITE_TRANSIENT);
         sqlite3_bind_int( mSql.client_status_insert_stmt, 7,  row.cpu);
         sqlite3_bind_int( mSql.client_status_insert_stmt, 8,  row.sync);
         sqlite3_bind_int( mSql.client_status_insert_stmt, 9,  row.ping);
         sqlite3_bind_int( mSql.client_status_insert_stmt, 10, row.lcor);
         sqlite3_bind_int( mSql.client_status_insert_stmt, 11, row.rcor);
         sqlite3_bind_int( mSql.client_status_insert_stmt, 12, row.rwnd);
         sqlite3_bind_int( mSql.client_status_insert_stmt, 13, row.difs);
         sqlite3_bind_int( mSql.client_status_insert_stmt, 14, row.tkbs);
         if (sqlite3_step( mSql.client_status_insert_stmt) != SQLITE_DONE) printf("Error: %s\n", sqlite3_errmsg(mSql.db_status));
      }

      if (sqlite3_exec(mSql.db_status, "COMMIT TRANSACTION;", nullptr, nullptr, &messageError) != SQLITE_OK) printf("Error: %s\n", messageError);

      init();

   }
}
