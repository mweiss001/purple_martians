// mwStateHistory.cpp

#include "pm.h"
#include "mwStateHistory.h"
#include "mwLoop.h"
#include "mwNetgame.h"
#include <limits>


mwStateHistory::mwStateHistory()
{
   initialize();
}

void mwStateHistory::initialize(void)
{
   // reset states
   for (int i=0; i<NUM_HISTORY_STATES; i++)
   {
      memset(history_state[i], 0, STATE_SIZE);
      history_state_frame_num[i] = -1;
   }
}


void mwStateHistory::show_states(const char *format, ...)
{
    va_list args;
    va_start(args, format);

   // show states
   for (int i=0; i<NUM_HISTORY_STATES; i++)
      printf("[%4d] ", history_state_frame_num[i]);
   vprintf(format, args);
   va_end(args);
}


int mwStateHistory::get_most_recent_index(void)
{
   // find index with the highest frame number
   int mx = -1;
   int indx = -1;
   for (int i=0; i<NUM_HISTORY_STATES; i++)
   {
      int fn = history_state_frame_num[i];
      if (fn > mx)
      {
         mx = fn;
         indx = i;
      }
   }
   return indx;
}



void mwStateHistory::get_base_state(char* base, int& base_frame_num, int frame_num)
{
   if (frame_num == 0) return; // base 0 leave as is
   int indx = -1;
   for (int i=0; i<NUM_HISTORY_STATES; i++) if (frame_num == history_state_frame_num[i]) indx = i;
   if (indx > -1)
   {
      memcpy(base, history_state[indx], STATE_SIZE);
      base_frame_num = history_state_frame_num[indx];
   }
}


void mwStateHistory::get_most_recent_state(char* base, int& base_frame_num)
{
   int indx = get_most_recent_index();
   if (indx > -1)
   {
      memcpy(base, history_state[indx], STATE_SIZE);
      base_frame_num = history_state_frame_num[indx];
   }
}


















int mwStateHistory::find_newest_frame_number(void)
{
   // find highest frame number
   // minimum this will return is zero, it ignores any -1's

   int mx = 0;
   for (int i=0; i<NUM_HISTORY_STATES; i++)
      if (history_state_frame_num[i] > mx) mx = history_state_frame_num[i];
   return mx;
}


// include -1 state in minimum calc, unless ignore neg is set
// if all states are -1 then return -1
int mwStateHistory::find_earliest_state(int include_neg)
{
   // find lowest frame number
   int mn = std::numeric_limits<int>::max();
   int indx = -1;
   for (int i=0; i<NUM_HISTORY_STATES; i++)
   {
      int fn = history_state_frame_num[i];
      if ( ((fn > -1) || (include_neg)) && (fn < mn))
      {
         mn = fn;
         indx = i;
      }
   }
   return indx;
}

void mwStateHistory::load_earliest_state(char* base, int& base_frame_num)
{
   if (mLoop.frame_num > 0)
   {
      int indx = find_earliest_state(0);
      if (indx > -1)
      {
         memcpy(base, history_state[indx], STATE_SIZE);
         base_frame_num = history_state_frame_num[indx];
         //show_states(" -  indx:%d  frame:%d  loading state\n", indx, history_state_frame_num[indx]);
      }
      //else show_states(" -  indx:%d  frame:%d  ERROR! loading state\n", indx, history_state_frame_num[indx]);
   }
}


void mwStateHistory::add_state(int frame_num)
{
   // if a state already exists with exact frame number, overwrite it
   // if not, if we have an open -1 slot use that,
   // if not, use the oldest slot and overwrite it
   // used for both ff add and end of frame add

   // now for client add also

   int indx = -1;
   for (int i=0; i<NUM_HISTORY_STATES; i++) if (history_state_frame_num[i] == frame_num) indx = i;   // if a state already exists with exact frame number, overwrite it
   if (indx == -1) indx = find_earliest_state(1);  // use the oldest slot, if there are any -1 they will be used first

   if (indx > -1)
   {
      mNetgame.game_vars_to_state(history_state[indx]);
      history_state_frame_num[indx] = frame_num;
   }
}





