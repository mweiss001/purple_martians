// mwSql.h

#include "sqlite3.h"


class mwSql
{
   public:
   mwSql();

   int init();

   int open_database();
   void create_tables();
   void create_prepared_statements();

   static int execute_sql_and_return_one_int(const char* sql, sqlite3 *db);
   static void execute_sql(const char* sql, sqlite3 *db);

//   std::vector<int> execute_sql_and_return_first_row_as_vector_int(const char* sql, sqlite3 *db);

   int execute_sql_and_return_first_row_as_vector_int(const char *sql, sqlite3 *db, std::vector<int> &ret);

   int execute_sql_and_return_2d_vector_int(const char *sql, sqlite3 *db, std::vector<std::vector<int>> &matrix);


   sqlite3 *db_sessions = nullptr;

   sqlite3 *db_status = nullptr;

   sqlite3 *db_game_events = nullptr;

   //   sqlite3 *db_logs     = nullptr;


   sqlite3_stmt *server_status_insert_stmt = nullptr;
   sqlite3_stmt *client_status_insert_stmt = nullptr;





};

extern mwSql mSql;
