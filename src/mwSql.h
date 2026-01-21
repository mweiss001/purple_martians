// mwSql.h

#include "sqlite3.h"


class mwSql
{
   public:
   mwSql();

   int init();

   int open_database();
   void create_tables();

   static int execute_sql_and_return_one_int(const char* sql, sqlite3 *db);
   static void execute_sql(const char* sql, sqlite3 *db);

//   std::vector<int> execute_sql_and_return_first_row_as_vector_int(const char* sql, sqlite3 *db);

   int execute_sql_and_return_first_row_as_vector_int(const char *sql, sqlite3 *db, std::vector<int> &ret);


   sqlite3 *db_sessions = nullptr;
//   sqlite3 *db_logs     = nullptr;

};

extern mwSql mSql;
