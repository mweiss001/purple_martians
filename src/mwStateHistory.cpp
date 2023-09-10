// mwStateHistory.cpp

#include "pm.h"
#include "mwStateHistory.h"
#include "mwLoop.h"
#include "mwNetgame.h"



mwStateHistory::mwStateHistory()
{
//   initialize();
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

   // show rewind states
   for (int i=0; i<NUM_HISTORY_STATES; i++)
      printf("[%4d] ", history_state_frame_num[i]);
   vprintf(format, args);
   va_end(args);
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

void mwStateHistory::load_earliest_state(void)
{
   if (mLoop.frame_num > 0)
   {
      int indx = find_earliest_state(0);
      if (indx > -1)
      {
         memcpy(mNetgame.state[0][1], history_state[indx], STATE_SIZE);
         mNetgame.state_frame_num[0][1] = history_state_frame_num[indx];
        // show_rewind_states(" -  indx:%d  frame:%d  loading server rewind state\n", indx, srv_rewind_state_frame_num[indx]);
      }
//      else show_rewind_states(" -  indx:%d  frame:%d  ERROR! loading server rewind state\n", indx, srv_rewind_state_frame_num[indx]);
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





