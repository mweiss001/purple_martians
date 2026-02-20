// mwStatusBuffer.h

#ifndef PM_MWSTATUSBUFFER_H
#define PM_MWSTATUSBUFFER_H

#include "sqlite3.h"

struct client_status_buffer
{
   int ss_id;
   std::string timestamp;
   int p;
   int color;
   std::string name;
   std::string hostname;
   int cpu;
   int pdsync;
   int ping;
   int client_loc_plr_cor;
   int client_rmt_plr_cor;
   int rewind;
   int cmp_dif_size;
   int tx_bytes_per_tally;
};

struct server_status_buffer
{
   std::string timestamp;
   int frame;
   int level;
   int moves;
   int enemies;
   int uptime;
};


class mwStatusBuffer
{

   public:
   mwStatusBuffer();




};
extern mwStatusBuffer mStatusBuffer;

#endif //PM_MWSTATUSBUFFER_H