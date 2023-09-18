// mwTally.cpp

#include "pm.h"
#include "mwTally.h"
#include <limits>


mwTally mTally[8];

mwTally mTally_late_cdats_last_sec[8];
mwTally mTally_game_move_dsync_avg_last_sec[8];

mwTally mTally_client_loc_plr_cor_last_sec[8];
mwTally mTally_client_rmt_plr_cor_last_sec[8];


mwTally::mwTally()
{
   initialize();
}

void mwTally::initialize(void)
{
   num = 0;
   tally = 0;
   mn = std::numeric_limits<double>::max();
   mx = std::numeric_limits<double>::lowest();
}




// return tally and optionally reset
double mwTally::get_tally(int reset)
{
   double tmp = tally;
   if (reset) initialize();
   return tmp;
}

// return average and optionally reset
double mwTally::get_avg(int reset)
{
   double avg = 0;
   if (num) avg = tally / num;
   if (reset) initialize();
   return avg;
}

// return max and optionally reset
double mwTally::get_max(int reset)
{
   double tmx = 0;
   if (num) tmx = mx;
   if (reset) initialize();
   return tmx;
}

// return min and optionally reset
double mwTally::get_min(int reset)
{
   double tmn = 0;
   if (num) tmn = mn;
   if (reset) initialize();
   return tmn;
}



void mwTally::add_data(double d)
{
   tally += d;
   num++;

   if (d > mx) mx = d;
   if (d < mn) mn = d;
}

