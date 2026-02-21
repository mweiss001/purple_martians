// mwStatusBuffer.h

#ifndef PM_MWSTATUSBUFFER_H
#define PM_MWSTATUSBUFFER_H

#include "sqlite3.h"

struct server_status_buffer_row
{
   std::uint64_t timestamp;
   int frame;
   int level;
   int moves;
   int enemies;
   int uptime;
};

struct client_status_buffer_row
{
   std::uint64_t timestamp;
   int frame;
   int p;
   int color;
   std::string name;
   std::string hostname;
   int cpu;
   int sync;
   int ping;
   int lcor;
   int rcor;
   int rwnd;
   int difs;
   int tkbs;
};




class mwStatusBuffer
{
   public:

   mwStatusBuffer();

   void init();
   void add();

   private:


   std::vector<server_status_buffer_row> server_status_buffer_rows;
   std::vector<client_status_buffer_row> client_status_buffer_rows;









};
extern mwStatusBuffer mStatusBuffer;

#endif //PM_MWSTATUSBUFFER_H