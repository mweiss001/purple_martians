// mwStatusBuffer.h

#ifndef PM_MWSTATUSBUFFER_H
#define PM_MWSTATUSBUFFER_H

#include "sqlite3.h"

class mwStatusBuffer
{
   public:

   mwStatusBuffer();


   void init();
   void add();

   int dumpFramePeriod = 20;

   private:

   void dump();

   int beginTransactionError{};

   int oldFrame = 0;




   std::vector<client_status_buffer_row> client_status_buffer_rows;


};
extern mwStatusBuffer mStatusBuffer;

#endif //PM_MWSTATUSBUFFER_H