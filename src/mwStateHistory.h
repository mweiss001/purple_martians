// mwStateHistory.h

#ifndef mwStateHistory_H
#define mwStateHistory_H

#define NUM_HISTORY_STATES 8



class mwStateHistory
{
   private:
   void _set_newest_and_oldest(void);

   public:
   mwStateHistory(); // default constructor
   void initialize(void);

   char history_state[NUM_HISTORY_STATES][STATE_SIZE];
   int history_state_frame_num[NUM_HISTORY_STATES];

   int newest_state_frame_num;
   int newest_state_index;
   char * newest_state;

   int oldest_state_frame_num;
   int oldest_state_index;
   char * oldest_state;

   int last_ack_state_frame_num;
   int last_ack_state_index;
   char * last_ack_state;

   void get_last_ack_state(char* base, int& base_frame_num);
   void set_ack_state(int frame_num);
   void get_base_state(char* base, int& base_frame_num, int frame_num);
   void show_states(const char *format, ...);
   void add_state(int frame_num);
};

#endif // mwStateHistory_H
