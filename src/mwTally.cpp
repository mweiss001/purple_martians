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


// return tally and reset
double mwTally::get_tally(void)
{
   double tmp = tally;
   initialize();
   return tmp;
}

// return average and reset
double mwTally::get_avg(void)
{
   double avg = 0;
   if (num) avg = tally / num;
   initialize();
   return avg;
}

// return max and reset
double mwTally::get_max(void)
{
   double tmx = 0;
   if (num) tmx = mx;
   initialize();
   return tmx;
}

// return min and reset
double mwTally::get_min(void)
{
   double tmn = 0;
   if (num) tmn = mn;
   initialize();
   return tmn;
}



void mwTally::add_data(double d)
{
   tally += d;
   num++;

   if (d > mx) mx = d;
   if (d < mn) mn = d;
}

