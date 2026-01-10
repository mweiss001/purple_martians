// mwSql.h

#include "sqlite3.h"


class mwSql
{
   public:
   mwSql();

   int open_database(void);
   void create_tables(void);

   int execute_sql_and_return_one_int(const char* sql, sqlite3 *db);
   void execute_sql(const char* sql, sqlite3 *db);

   sqlite3 *db_sessions = NULL;
   sqlite3 *db_logs     = NULL;

};

extern mwSql mSql;
