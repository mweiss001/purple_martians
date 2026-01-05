// mwSql.h

#include "sqlite3.h"

class mwSql
{
   public:
   mwSql();

   int open_database(void);
   void create_tables(void);


   int insert_bstatement(int account_type, const char* date, int balance, int prev_balance);
   int does_bstatement_exist(int acct, const char* date);

   void show_all_statements(void);

   void fill_table_create_sql();


   int execute_sql_and_return_one_int(const char* sql);

   void execute_sql(const char* sql);



   sqlite3 *db;

   char database_filename[500];

   char *table_create_sql[20];



};
extern mwSql mSql;






