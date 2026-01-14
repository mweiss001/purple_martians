// mwSql.cpp

#include "pm.h"
#include "mwSql.h"

mwSql mSql;

mwSql::mwSql()
{
   open_database();
   create_tables();
}

int mwSql::open_database(void)
{
   char filename[256];
   sprintf(filename, "%s", "data/sessions.db");
   if (sqlite3_open(filename, &db_sessions))
   {
      printf("Can't open database %s\n", filename);
      sqlite3_close(db_sessions);
      return (0);
   }
   sprintf(filename, "%s", "data/logs.db");
   if (sqlite3_open(filename, &db_logs))
   {
      printf("Can't open database %s\n", filename);
      sqlite3_close(db_logs);
      return (0);
   }
   // printf("Databases open.\n");
   return (1);
}

void mwSql::create_tables(void)
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
   if (sqlite3_exec(db_sessions, sql, NULL, 0, NULL) != SQLITE_OK) printf("Error Creating Table 'sessions' \n");


   strcpy(sql, "CREATE TABLE IF NOT EXISTS gm( \
               muid           TEXT PRIMARY KEY, \
               filename       TEXT, \
               dt_start       TEXT, \
               dt_end         TEXT, \
               duration       INT,  \
               level          INT,  \
               num_entries    INT ); ");
   if (sqlite3_exec(db_sessions, sql, NULL, 0, NULL) != SQLITE_OK) printf("Error Creating Table 'gm' \n");


   strcpy(sql, "CREATE TABLE IF NOT EXISTS gm_sessions( \
               id                INTEGER PRIMARY KEY, \
               gm_muid           TEXT, \
               session_id        INT  ); ");
   if (sqlite3_exec(db_sessions, sql, NULL, 0, NULL) != SQLITE_OK) printf("Error Creating Table 'gm_sessions' \n");



   strcpy(sql, "CREATE TABLE IF NOT EXISTS logs( \
               id              INTEGER PRIMARY KEY, \
               msg_type        INT, \
               sub_type        INT, \
               created         TEXT, \
               agt             REAL, \
               frame_num       INT, \
               player          INT, \
               client          INT, \
               message         TEXT ); ");
   if (sqlite3_exec(db_logs, sql, NULL, 0, NULL) != SQLITE_OK) printf("Error Creating Table 'logs' \n");


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
   if (sqlite3_exec(db_sessions, sql, NULL, 0, NULL) != SQLITE_OK) printf("Error Creating Table 'status' \n");








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
   if (db == NULL)
   {
      printf("Error! Database not open.\n%s\n", sql);
      return 0;
   }
   char* messaggeError;
   int val = 0;
   if (sqlite3_exec(db, sql, single_value_callback, &val, &messaggeError) != SQLITE_OK) printf("Error: %s\n%s\n", messaggeError, sql);
   return val;
}

void mwSql::execute_sql(const char* sql, sqlite3 *db)
{
   if (db == NULL)
   {
      printf("Error! Database not open.\n%s\n", sql);
      return;
   }
   char* messaggeError;
   if (sqlite3_exec(db, sql, NULL, NULL, &messaggeError) != SQLITE_OK) printf("Error: %s\n%s\n", messaggeError, sql);
}







