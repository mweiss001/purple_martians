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

   newest_state_frame_num = -1;
   newest_state_index = -1;
   newest_state = NULL;

   oldest_state_frame_num = -1;
   oldest_state_index = -1;
   oldest_state = NULL;

   last_ack_state_frame_num = -1;
   last_ack_state_index = -1;
   last_ack_state = NULL;
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

// internal class use only
void mwStateHistory::_set_newest_and_oldest(void)
{
   int mn = std::numeric_limits<int>::max();
   int mx = std::numeric_limits<int>::min();

   int mn_indx = -1;
   int mx_indx = -1;

   for (int i=0; i<NUM_HISTORY_STATES; i++)
   {
      int fn = history_state_frame_num[i];

      if (fn > -1) // ignore all unset states
      {
         if (fn < mn)
         {
            mn = fn;
            mn_indx = i;
         }
         if (fn > mx)
         {
            mx = fn;
            mx_indx = i;
         }
      }
   }
   if (mn_indx > -1)
   {
      oldest_state_frame_num = mn;
      oldest_state_index = mn_indx;
      oldest_state = history_state[mn_indx];
   }

   if (mx_indx > -1)
   {
      newest_state_frame_num = mx;
      newest_state_index = mx_indx;
      newest_state = history_state[mx_indx];
   }
}



// the server is making a dif to send to a client and needs a base to build it on
void mwStateHistory::get_last_ack_state(char* base, int& base_frame_num)
{
   int indx = last_ack_state_index;
   if (indx > -1)
   {
      memcpy(base, history_state[indx], STATE_SIZE);
      base_frame_num = history_state_frame_num[indx];
   }




}




// the server has just received a stak packet acknowledging frame_num
// if we have a state that matches that frame_num, set last_ack variables
// if we do not, then reset them
void mwStateHistory::set_ack_state(int frame_num)
{
   // see if we have a state with this frame_num
   int indx = -1;
   for (int i=0; i<NUM_HISTORY_STATES; i++) if (frame_num == history_state_frame_num[i]) indx = i;
   if (indx > -1)
   {
      last_ack_state_frame_num = frame_num;
      last_ack_state_index = indx;
      last_ack_state = history_state[indx];
   }
   else
   {
      last_ack_state_frame_num = -1;
      last_ack_state_index = -1;
      last_ack_state = NULL;
   }
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

// if a state already exists with exact frame number, overwrite it
// if not replace the oldest frame number (include empty -1's so they get used first)
// this is the only way that anything gets added or changed
void mwStateHistory::add_state(int frame_num)
{
   int indx = -1;
   // if a state already exists with exact frame number, use that index and overwrite it
   for (int i=0; i<NUM_HISTORY_STATES; i++) if (history_state_frame_num[i] == frame_num) indx = i;

   if (indx == -1)
   {
      // find lowest frame number, include -1 so they will be used first if any exist
      int mn = std::numeric_limits<int>::max();
      for (int i=0; i<NUM_HISTORY_STATES; i++)
         if (history_state_frame_num[i] < mn)
         {
            mn = history_state_frame_num[i];
            indx = i;
         }
   }
   if (indx > -1)
   {
      mNetgame.game_vars_to_state(history_state[indx]);
      history_state_frame_num[indx] = frame_num;
   }
   else show_states("mwStateHistory::add_state(%d) did not add state?...wtf!\n");

   _set_newest_and_oldest();
}
