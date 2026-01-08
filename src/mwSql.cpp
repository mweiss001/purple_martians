// mwSql.cpp

#include "pm.h"
#include "mwSql.h"


mwSql::mwSql()
{
   open_database();
   create_tables();
}

int mwSql::open_database(void)
{
   sprintf(database_filename, "%s", "data/database.db");

   if (sqlite3_open(database_filename, &db))
   {
      printf("Can't open database %s\n", database_filename);
      sqlite3_close(db);
      return (0);
   }
   // printf("Database %s open.\n", database_filename);
   return (1);
}

void mwSql::create_tables(void)
{
   char sql[2000];

   strcpy(sql, "CREATE TABLE IF NOT EXISTS sessions( \
               id                INTEGER PRIMARY KEY, \
               filename          VARCHAR(255), \
               dt_start          DATETIME, \
               dt_end            DATETIME, \
               duration          INT, \
               ip                VARCHAR(255), \
               port              INT, \
               hostname          VARCHAR(255), \
               endreason         VARCHAR(255),  \
               cdat_rx           INT, \
               player_num        INT, \
               player_name       VARCHAR(255), \
               player_color      INT, \
               next_levels       INT, \
               exits_activated   INT, \
               respawns          INT, \
               shots_fired       INT,  \
               enemy_hits        INT, \
               player_hits       INT, \
               self_hits         INT, \
               purple_coins      INT, \
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
   if (sqlite3_exec(db, sql, NULL, 0, NULL) != SQLITE_OK) printf("Error Creating Table 'sessions' \n");
   else printf("Table 'sessions' Created Successfully\n");

   strcpy(sql, "CREATE TABLE IF NOT EXISTS gm( \
               muid           TEXT PRIMARY KEY, \
               filename       TEXT, \
               dt_start       TEXT, \
               dt_end         TEXT, \
               duration       INT,  \
               level          INT,  \
               num_entries    INT ); ");
   if (sqlite3_exec(db, sql, NULL, 0, NULL) != SQLITE_OK) printf("Error Creating Table 'gm' \n");
   else printf("Table 'gm' Created Successfully\n");


   strcpy(sql, "CREATE TABLE IF NOT EXISTS gm_sessions( \
               id                INTEGER PRIMARY KEY, \
               gm_muid           TEXT, \
               session_id        INT  ); ");
   if (sqlite3_exec(db, sql, NULL, 0, NULL) != SQLITE_OK) printf("Error Creating Table 'gm_sessions' \n");
   else printf("Table 'gm_sessions' Created Successfully\n");





   strcpy(sql, "CREATE TABLE IF NOT EXISTS players( \
               id                INTEGER PRIMARY KEY, \
               player_num        INT, \
               player_name       VARCHAR(255), \
               player_color      INT ); ");
   if (sqlite3_exec(db, sql, NULL, 0, NULL) != SQLITE_OK) printf("Error Creating Table 'players' \n");
   else printf("Table 'players' Created Successfully\n");

   strcpy(sql, "CREATE TABLE IF NOT EXISTS gm_players( \
               id                INTEGER PRIMARY KEY, \
               gm_id             INT, \
               player_id         INT  ); ");
   if (sqlite3_exec(db, sql, NULL, 0, NULL) != SQLITE_OK) printf("Error Creating Table 'gm_players' \n");
   else printf("Table 'gm_players' Created Successfully\n");
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




int mwSql::execute_sql_and_return_one_int(const char* sql)
{
   char* messaggeError;
   int val;
   if (sqlite3_exec(db, sql, single_value_callback, &val, &messaggeError) != SQLITE_OK) printf("Error: %s\n%s\n", messaggeError, sql);
   return val;
}


void mwSql::execute_sql(const char* sql)
{
   char* messaggeError;
   if (sqlite3_exec(db, sql, NULL, NULL, &messaggeError) != SQLITE_OK) printf("Error: %s\n%s\n", messaggeError, sql);
}








void mwSql::show_all_statements(void)
{
   if (sqlite3_exec(db, "SELECT * FROM bstatements;", callback, NULL, NULL) != SQLITE_OK) printf("SELECT error\n");
}











int mwSql::insert_bstatement(int account_type, const char* date, int balance, int prev_balance)
{
   char msg[500];
   char* messaggeError;
   // does the record already exist?
   int cnt = does_bstatement_exist(account_type, date);
   if (cnt)
   {
      printf("ERROR! %d statement(s) already exist with acct_type:%d and date:%s\n", cnt, account_type, date);
   }
   else
   {
      sprintf(msg, "INSERT INTO bstatements VALUES(NULL, %d, '%s', %d, %d);", account_type, date, balance, prev_balance);
      //printf("%s\n", msg);
      if (sqlite3_exec(db, msg, NULL, 0, &messaggeError) != SQLITE_OK) printf("Error Inserting Record: %s\n%s\n", messaggeError, msg);
      else printf("%s\n", msg);
   }

   // figure out the id of the last added record and return it
   int bs_id;
   sprintf(msg, "SELECT id FROM bstatements WHERE account_type=%d AND date='%s';", account_type, date);
   if (sqlite3_exec(db, msg, single_value_callback, &bs_id, &messaggeError) != SQLITE_OK) printf("bstatement SELECT Error: %s\n%s\n", messaggeError, msg);
   //printf("-----> statement record id:%d\n", bs_id);


   return bs_id;
}


int mwSql::does_bstatement_exist(int acct, const char* date)
{
   int cnt;
   char msg[500];
   char* messaggeError;
   sprintf(msg, "SELECT COUNT(*) FROM bstatements WHERE account_type=%d AND date='%s';", acct, date);
   if (sqlite3_exec(db, msg, single_value_callback, &cnt, &messaggeError) != SQLITE_OK) printf("Error: %s\n%s\n", messaggeError, msg);
   return cnt; // returns the number of matches
}


















mwSql mSql;

