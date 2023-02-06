// mwTally.h

class mwTally
{
   public:

   mwTally(); // default constructor
   void initialize(void);

   double get_tally(void);
   double get_avg(void);
   double get_max(void);
   double get_min(void);

   void add_data(double d);

   double tally;

   int num;

   double avg;
   double mn;
   double mx;
};
extern mwTally mwT[8];
extern mwTally mwT_late_cdats_last_sec[8];
extern mwTally mwT_game_move_dsync_avg_last_sec[8];



