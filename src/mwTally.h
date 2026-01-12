// mwTally.h

#ifndef mwTally_H
#define mwTally_H


class mwTally
{
   public:

   mwTally(); // default constructor
   void initialize();

   double get_tally(int reset);
   double get_avg(int reset);
   double get_max(int reset);
   double get_min(int reset);

   void add_data(double d);

   double tally;

   int num;

   double avg;
   double mn;
   double mx;
};

extern mwTally mTally[8];





extern mwTally mTally_late_cdats_last_sec[8];
extern mwTally mTally_game_move_dsync_avg_last_sec[8];

extern mwTally mTally_client_loc_plr_cor_last_sec[8];
extern mwTally mTally_client_rmt_plr_cor_last_sec[8];

#endif
