// mwStateHistory.h

#ifndef mwStateHistory_H
#define mwStateHistory_H


#define NUM_HISTORY_STATES 4


// this is such fucking bullshit! I don't want to define this twice!!
#define STATE_SIZE 112384

// I can't use this or I get stupid header file recursion
// #include "mwNetgame.h"







class mwStateHistory
{
   public:

   mwStateHistory(); // default constructor
   void initialize(void);


   char history_state[NUM_HISTORY_STATES][STATE_SIZE];

   int history_state_frame_num[NUM_HISTORY_STATES];

   void show_states(const char *format, ...);
   int find_earliest_state(int include_neg);
   void load_earliest_state(void);
   void add_state(int frame_num);


};

#endif // mwStateHistory_H
