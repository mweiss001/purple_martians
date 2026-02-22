// mwStatusBuffer.h

#ifndef PM_MWSTATUSBUFFER_H
#define PM_MWSTATUSBUFFER_H

#include "sqlite3.h"

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

   int beginTransactionError{};

   int oldFrame = 0;

   std::vector<client_status_buffer_row> client_status_buffer_rows;


};
extern mwStatusBuffer mStatusBuffer;

#endif //PM_MWSTATUSBUFFER_H