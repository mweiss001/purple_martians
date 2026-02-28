#include "pm.h"
#include "mwLoop.h"
#include "mwThreadSafeQueue.h"

#include "mwNetgame.h"
#include "mwSql.h"


mwThreadSafeQueue mThreadSafeQueue;

mwThreadSafeQueue::mwThreadSafeQueue()
{

   // start thread
   m_thread = std::jthread(std::bind_front(&mwThreadSafeQueue::sqlite_consumer, this));

   time_ra.initialize(10);

}


void mwThreadSafeQueue::add()
{
   // get timestamp as msec since epoch
   std::uint64_t timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

   // get current frame
   int frame = mLoop.frame_num;

   std::lock_guard<std::mutex> lock(m);

   // add queue row for each active player
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
         q.push(row);

         //printf("pushed row: %d  qs:%d \n", frame, (int) q.size());

      }

//   if (q.size() > 20) cv.notify_one(); // Notify consumer that items are available
   cv.notify_one(); // Notify consumer that items are available

}



bool mwThreadSafeQueue::pop(client_status_buffer_row &row)
{
   std::unique_lock<std::mutex> lock(m);

   // wait until queue is not empty
   cv.wait(lock, [this]{ return !q.empty(); });

   if (q.empty())  return false;
   row = q.front();
   q.pop();
   return true;
}

/*

   // Wait until queue is not empty or the producer is finished
   cv.wait(lock, [this]{ return !q.empty() || finished; });

   if (q.empty() && finished)
   {
      return false; // Signal end of work
   }
   row = q.front();
   q.pop();
   return true;

*/


/*

void mwThreadSafeQueue::complete_work()
{
   std::lock_guard<std::mutex> lock(m);
   finished = true;
   cv.notify_all(); // Notify all waiting consumers to stop
}
*/


// this is run in its own thread
// fills q2 and dumps q2 when filled

void mwThreadSafeQueue::sqlite_consumer(std::stop_token stoken)
{
   client_status_buffer_row row;

   sqlite3 *db;

   char filename[256];
   sprintf(filename, "%s", "data/client_status.db");
   if (sqlite3_open(filename, &db))   { printf("Can't open database %s\n", filename);  }

   mSql.execute_sql("PRAGMA journal_mode = WAL", db);

   char sql[2000];
   strcpy(sql, "CREATE TABLE IF NOT EXISTS client_status( \
               id            INTEGER PRIMARY KEY, \
               timestamp     INT, \
               frame         INT, \
               pl_num        INT, \
               pl_col        INT, \
               pl_name       TEXT, \
               pl_host       TEXT, \
               cpu           INT, \
               sync          INT, \
               ping          INT, \
               lcor          INT, \
               rcor          INT, \
               rewind        INT, \
               difs          INT, \
               tkbs          INT ); ");
   mSql.execute_sql(sql, db);
   strcpy(sql, "CREATE INDEX IF NOT EXISTS timestamp_index ON client_status(timestamp DESC)");
   mSql.execute_sql(sql, db);

   sqlite3_stmt *client_status_insert_stmt = nullptr;

   const char* sql2 = "INSERT INTO client_status VALUES(NULL, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)";
   int rc2 = sqlite3_prepare_v2(db, sql2, -1, &client_status_insert_stmt, nullptr);

   if (rc2 != SQLITE_OK) printf("Failed to prepare statement: %s\n", sqlite3_errmsg(db));

   // setup done

   // this is the loop that continually runs in the thread
   // I have tried to insert in a batch, wrapped in a transaction but could not get it to work
   // the while loop 'while (pop(row))' blocks until it has a row to return
   // nothing outside that is ever executed


   while (!stoken.stop_requested())
   {
      printf("client_status row insert thread outer loop\n");
      std::this_thread::sleep_for(std::chrono::milliseconds(1000));

      while (pop(row))
      {

/*
         double t0 = al_get_time();

         sqlite3_reset(      client_status_insert_stmt);
         sqlite3_bind_int64( client_status_insert_stmt, 1,  row.timestamp);
         sqlite3_bind_int(   client_status_insert_stmt, 2,  row.frame);
         sqlite3_bind_int(   client_status_insert_stmt, 3,  row.p);
         sqlite3_bind_int(   client_status_insert_stmt, 4,  row.color);
         sqlite3_bind_text(  client_status_insert_stmt, 5,  row.name.c_str(),     -1, SQLITE_TRANSIENT);
         sqlite3_bind_text(  client_status_insert_stmt, 6,  row.hostname.c_str(), -1, SQLITE_TRANSIENT);
         sqlite3_bind_int(   client_status_insert_stmt, 7,  row.cpu);
         sqlite3_bind_int(   client_status_insert_stmt, 8,  row.sync);
         sqlite3_bind_int(   client_status_insert_stmt, 9,  row.ping);
         sqlite3_bind_int(   client_status_insert_stmt, 10, row.lcor);
         sqlite3_bind_int(   client_status_insert_stmt, 11, row.rcor);
         sqlite3_bind_int(   client_status_insert_stmt, 12, row.rwnd);
         sqlite3_bind_int(   client_status_insert_stmt, 13, row.difs);
         sqlite3_bind_int(   client_status_insert_stmt, 14, row.tkbs);

         if (sqlite3_step(   client_status_insert_stmt) != SQLITE_DONE) printf("Error: %s\n", sqlite3_errmsg(db));

         double et = (al_get_time() - t0) * 1000;
         printf("Inserted item: %d  -  %f\n", row.frame, et);

  */

         q2.push_back(row);

         //printf("added to q2: %d\n", row.frame);

         if ((int) q2.size() >= mNetgame.server_insert_client_status_batch_size_target) dumpQ2(db, client_status_insert_stmt);



      }
   }
}



void mwThreadSafeQueue::dumpQ2(sqlite3 *db, sqlite3_stmt *client_status_insert_stmt)
{
   double t0 = al_get_time();
   int count = q2.size();

   mSql.execute_sql("BEGIN TRANSACTION;", db);

   for (auto row : q2)
   {
      /*

   while (!q2.empty())
   {


      // get row from front and remove
      client_status_buffer_row row = q2.front();
      q2.pop();
*/


      sqlite3_reset(      client_status_insert_stmt);
      sqlite3_bind_int64( client_status_insert_stmt, 1,  row.timestamp);
      sqlite3_bind_int(   client_status_insert_stmt, 2,  row.frame);
      sqlite3_bind_int(   client_status_insert_stmt, 3,  row.p);
      sqlite3_bind_int(   client_status_insert_stmt, 4,  row.color);
      sqlite3_bind_text(  client_status_insert_stmt, 5,  row.name.c_str(),     -1, SQLITE_TRANSIENT);
      sqlite3_bind_text(  client_status_insert_stmt, 6,  row.hostname.c_str(), -1, SQLITE_TRANSIENT);
      sqlite3_bind_int(   client_status_insert_stmt, 7,  row.cpu);
      sqlite3_bind_int(   client_status_insert_stmt, 8,  row.sync);
      sqlite3_bind_int(   client_status_insert_stmt, 9,  row.ping);
      sqlite3_bind_int(   client_status_insert_stmt, 10, row.lcor);
      sqlite3_bind_int(   client_status_insert_stmt, 11, row.rcor);
      sqlite3_bind_int(   client_status_insert_stmt, 12, row.rwnd);
      sqlite3_bind_int(   client_status_insert_stmt, 13, row.difs);
      sqlite3_bind_int(   client_status_insert_stmt, 14, row.tkbs);
      if (sqlite3_step(   client_status_insert_stmt) != SQLITE_DONE) printf("Error: %s\n", sqlite3_errmsg(db));
//      count++;
   }

   q2.clear();

   mSql.execute_sql("COMMIT;", db);

   // add time to insert (ms) to rolling average
   time_ra.add_data((al_get_time() - t0)*1000);

   // count, avg, max (ns)
   mNetgame.server_insert_client_status_batch_size_actual = count;
   mNetgame.server_insert_client_status_batch_time_avg = time_ra.avg*1000; // ns
   mNetgame.server_insert_client_status_batch_time_max = time_ra.max*1000; // ns

   double et = time_ra.last_input;
   int qs = q.size();
   int q2s = q2.size();
   printf("%d - Inserted %d rows in %0.2f - %0.4f per row - %f  %f q1:%d q2:%d\n", mLoop.frame_num, count, et, et/count, time_ra.avg, time_ra.max, qs, q2s);

}

