// mwSql.cpp

#include "pm.h"
#include "mwSql.h"

mwSql mSql;

mwSql::mwSql() { }

int mwSql::init()
{
   open_database();
   create_tables();
   create_prepared_statements();

   return 1;
}


void mwSql::create_prepared_statements()
{
   const char* sql1 = "INSERT INTO server_status VALUES(NULL, ?, ?, ?, ?, ?, ?)";
   int rc1 = sqlite3_prepare_v2(db_status, sql1, -1, &server_status_insert_stmt, nullptr);
   if (rc1 != SQLITE_OK) printf("Failed to prepare statement: %s\n", sqlite3_errmsg(db_status));

   const char* sql2 = "INSERT INTO client_status VALUES(NULL, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)";
   int rc2 = sqlite3_prepare_v2(db_status, sql2, -1, &client_status_insert_stmt, nullptr);
   if (rc2 != SQLITE_OK) printf("Failed to prepare statement: %s\n", sqlite3_errmsg(db_status));
}


int mwSql::open_database()
{
   char filename[256];

   sprintf(filename, "%s", "data/sessions.db");
   if (sqlite3_open(filename, &db_sessions))
   {
      printf("Can't open database %s\n", filename);
      //sqlite3_close(db_sessions);
      return (0);
   }

   sprintf(filename, "%s", "data/game_events.db");
   if (sqlite3_open(filename, &db_game_events))
   {
      printf("Can't open database %s\n", filename);
      //sqlite3_close(db_game_events);
      return (0);
   }

   sprintf(filename, "%s", "data/status.db");
   if (sqlite3_open(filename, &db_status))
   {
      printf("Can't open database %s\n", filename);
      //sqlite3_close(db_status);
      return (0);
   }

//   execute_sql("PRAGMA journal_mode = WAL", db_status);


   // sprintf(filename, "%s", "data/logs.db");
   // if (sqlite3_open(filename, &db_logs))
   // {
   //    printf("Can't open database %s\n", filename);
   //    sqlite3_close(db_logs);
   //    return (0);
   // }
   // printf("Databases open.\n");
   return (1);
}

void mwSql::create_tables()
{
   char sql[2000];

   strcpy(sql, "CREATE TABLE IF NOT EXISTS sessions( \
               id                        INTEGER PRIMARY KEY, \
               dt_start                  DATETIME, \
               dt_end                    DATETIME, \
               duration                  INT, \
               ip                        TEXT, \
               port                      INT, \
               hostname                  TEXT, \
               endreason                 TEXT, \
               cdat_rx                   INT, \
               player_num                INT, \
               player_name               TEXT, \
               player_color              INT, \
               next_levels               INT, \
               exits_activated           INT, \
               respawns                  INT, \
               shots_fired               INT, \
               enemy_hits                INT, \
               player_hits               INT, \
               self_hits                 INT, \
               purple_coins              INT, \
               tx_bytes_total            INT, \
               tx_bytes_avg_per_sec      INT, \
               tx_bytes_max_per_frame    INT, \
               rx_bytes_total            INT, \
               rx_bytes_avg_per_sec      INT, \
               rx_bytes_max_per_frame    INT, \
               tx_packets_total          INT, \
               tx_packets_avg_per_sec    INT, \
               tx_packets_max_per_frame  INT, \
               rx_packets_total          INT, \
               rx_packets_avg_per_sec    INT, \
               rx_packets_max_per_frame  INT ); ");
   execute_sql(sql, db_sessions);


   strcpy(sql, "CREATE TABLE IF NOT EXISTS gm( \
               muid           TEXT PRIMARY KEY, \
               filename       TEXT, \
               dt_start       TEXT, \
               dt_end         TEXT, \
               duration       INT,  \
               level          INT,  \
               num_entries    INT ); ");
   execute_sql(sql, db_sessions);


   strcpy(sql, "CREATE TABLE IF NOT EXISTS gm_sessions( \
               id                INTEGER PRIMARY KEY, \
               gm_muid           TEXT, \
               session_id        INT  ); ");
   execute_sql(sql, db_sessions);

/*

   // strcpy(sql, "CREATE TABLE IF NOT EXISTS logs( \
   //             id              INTEGER PRIMARY KEY, \
   //             msg_type        INT, \
   //             sub_type        INT, \
   //             created         TEXT, \
   //             agt             REAL, \
   //             frame_num       INT, \
   //             player          INT, \
   //             client          INT, \
   //             message         TEXT ); ");
   // execute_sql(sql, db_sessions);
*/

   strcpy(sql, "CREATE TABLE IF NOT EXISTS status( \
               timestamp     TEXT PRIMARY KEY, \
               version       TEXT, \
               uptime        INT, \
               cpu           INT, \
               clients       INT, \
               level         INT, \
               level_time    INT, \
               moves         INT, \
               enemies       INT ); ");
   execute_sql(sql, db_sessions);




   strcpy(sql, "CREATE TABLE IF NOT EXISTS server_status( \
               id            INTEGER PRIMARY KEY, \
               timestamp     INT, \
               frame         INT, \
               level         INT, \
               moves         INT, \
               enemies       INT, \
               uptime        INT ); ");
   execute_sql(sql, db_status);


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
   execute_sql(sql, db_status);

   strcpy(sql, "CREATE INDEX IF NOT EXISTS timestamp_index ON client_status(timestamp DESC)");
   execute_sql(sql, db_status);

   strcpy(sql, "CREATE TABLE IF NOT EXISTS control( \
               id          INTEGER PRIMARY KEY, \
               key         TEXT, \
               val         INT, \
               mod         INT ); ");
   execute_sql(sql, db_sessions);


   strcpy(sql, "CREATE TABLE IF NOT EXISTS game_events( \
               id          INTEGER PRIMARY KEY, \
               frame       INT, \
               ev          INT, \
               x           INT, \
               y           INT, \
               p           INT, \
               z2          INT, \
               z3          INT, \
               z4          INT ); ");
   execute_sql(sql, db_game_events);


}








int single_value_callback(void *val, int count, char** data, char** columns)
{
   int *val_int = (int*)val; // convert void* to int* (make a copy of the pointer?)
   if (count != 1) printf("Error! More than one result when expecting only one.\n");
   const char *val_as_text = data[0];  // convert to string
   *val_int = atoi(val_as_text); // convert to int and store in *id_int
   return 0;
}

int callback(void* data, int argc, char** argv, char** azColName)
{
   for (int i=0; i<argc; i++)
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   printf("\n");
   return 0;
}

int mwSql::execute_sql_and_return_one_int(const char* sql, sqlite3 *db)
{
   if (db == nullptr)
   {
      printf("Error! Database not open.\n%s\n", sql);
      return 0;
   }
   char* messageError;
   int val = 0;
   if (sqlite3_exec(db, sql, single_value_callback, &val, &messageError) != SQLITE_OK) printf("Error: %s\n%s\n", messageError, sql);
   return val;
}

void mwSql::execute_sql(const char* sql, sqlite3 *db)
{
   if (db == nullptr)
   {
      printf("Error! Database not open.\n%s\n", sql);
      return;
   }
   char* messageError;
   if (sqlite3_exec(db, sql, nullptr, nullptr, &messageError) != SQLITE_OK) printf("Error: %s\n%s\n", messageError, sql);
}

// returns 0 on success
// 1 = no rows
// 2 = error
int mwSql::execute_sql_and_return_first_row_as_vector_int(const char *sql, sqlite3 *db, std::vector<int> &ret)
{
   if (db == nullptr)
   {
      printf("Error! Database not open.\n%s\n", sql);
      return 2;
   }

   sqlite3_stmt* stmt;
   int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
   if (rc != SQLITE_OK)
   {
      printf("Failed to prepare statement: %s\n", sqlite3_errmsg(db));
      return 2;
   }

   rc = sqlite3_step(stmt);
   if (rc == SQLITE_DONE)
   {
      printf("No rows found for: %s\n", sql);
      sqlite3_finalize(stmt);
      return 1;
   }

   if (rc == SQLITE_ROW)
   {
      int column_count = sqlite3_column_count(stmt);
      for (int i=0; i<column_count; i++)
         ret.push_back(sqlite3_column_int(stmt, i));
   }

   sqlite3_finalize(stmt);
   return 0;

}



// returns 0 on success
// 2 = error

int mwSql::execute_sql_and_return_2d_vector_int(const char *sql, sqlite3 *db, std::vector<std::vector<int>> &matrix )
{
   if (db == nullptr)
   {
      printf("Error! Database not open.\n%s\n", sql);
      return 2;
   }

   sqlite3_stmt* stmt;
   int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);


   if (rc != SQLITE_OK)
   {
      printf("Failed to prepare statement: %s\n %s\n", sql, sqlite3_errmsg(db));
      sqlite3_finalize(stmt);
      return 2;
   }

   while ((rc = sqlite3_step(stmt)) == SQLITE_ROW)
   {
      std::vector<int> row;
      for (int i=0; i<sqlite3_column_count(stmt); i++)
         row.push_back(sqlite3_column_int(stmt, i));
      matrix.push_back(row); // Add row to 2D vector
   }

   sqlite3_finalize(stmt);
   return 0;
}







































