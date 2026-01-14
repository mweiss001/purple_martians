// mwSql.h

#include "sqlite3.h"


class mwSql
{
   public:
   mwSql();

   int init();

   int open_database();
   void create_tables();

   int execute_sql_and_return_one_int(const char* sql, sqlite3 *db);
   void execute_sql(const char* sql, sqlite3 *db);

   sqlite3 *db_sessions = nullptr;
   sqlite3 *db_logs     = nullptr;

};

extern mwSql mSql;
