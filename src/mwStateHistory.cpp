// mwStateHistory.cpp

#include "pm.h"
#include "mwStateHistory.h"
#include "mwLoop.h"
#include "mwNetgame.h"
#include <limits>
#include "mwLog.h"
#include "mwPlayer.h"


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

// called whenever adding state
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
         if (fn < mn) // new minimum
         {
            mn = fn;
            mn_indx = i;
         }
         if (fn > mx) // new maximum
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


// called when the server is making a dif to send to a client and needs a base to build it on
void mwStateHistory::get_last_ack_state(char* base, int& base_frame_num)
{
   int indx = last_ack_state_index;
   if (indx > -1)
   {
      memcpy(base, history_state[indx], STATE_SIZE);
      base_frame_num = history_state_frame_num[indx];
   }
}


// called when the server receives stak packet acknowledging frame_num
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



// used by server to apply a rewind state that matches sent frame number
// does nothing if:
// sent frame_num less than 1
// sent frame_num is the same as current frame

// if sent frame is not found in history, use oldest frame (if valid)

// ??????
// if that happens, you should also set dirty frame to oldest frame (if valid) or -1 if not ???????


void mwStateHistory::apply_rewind_state(int frame_num)
{
   if (frame_num < 1) return;

   // how many frames to rewind and replay
   int ff = mPlayer.loc[0].rewind = mLoop.frame_num - frame_num;

   // if same frame as current frame, do nothing
   if (ff == 0)
   {
      mLog.log_add_prefixed_text(LOG_NET_stdf_rewind, -1, "stdf rewind [none]\n");
      mLog.add_log_net_db_row(LOG_NET_stdf_rewind, 0, 0,   "stdf rewind [none]");

      return;
   }

   // find index of matching frame
   int indx = -1;
   for (int i=0; i<NUM_HISTORY_STATES; i++) if (frame_num == history_state_frame_num[i]) indx = i;


   char msg[200];


   if (indx == -1)
   {
      indx = oldest_state_index;
//      if (indx == -1) mLog.log_add_prefixed_textf(LOG_NET_stdf_rewind, -1, "stdf rewind [%d] not found - oldest frame not valid\n", frame_num);
//      else            mLog.log_add_prefixed_textf(LOG_NET_stdf_rewind, -1, "stdf rewind [%d] not found - using oldest frame [%d]\n", frame_num, history_state_frame_num[indx]);


      if (indx == -1) sprintf(msg, "stdf rewind [%d] not found - oldest frame not valid", frame_num);
      else            sprintf(msg, "stdf rewind [%d] not found - using oldest frame [%d]", frame_num, history_state_frame_num[indx]);


//      char not_found[200];
//      char why[200];
//      sprintf(not_found, "stdf rewind [%d] not found - ", frame_num);
//      indx = oldest_state_index;
//      if (indx == -1) sprintf(why, "oldest frame not valid");
//      else            sprintf(why, "using oldest frame [%d]", history_state_frame_num[indx]);
//      mLog.log_add_prefixed_textf(LOG_NET_stdf, -1, "%s %s\n", not_found, why);

//      mLog.log_add_prefixed_textf(LOG_NET_stdf, 0, "stdf rewind [%d] not found - ", frame_num);
//      indx = oldest_state_index;
//      if (indx == -1) mLog.log_append_text(LOG_NET_stdf, "oldest frame not valid\n");
//      else mLog.log_append_textf(LOG_NET_stdf, "using oldest frame [%d]\n", history_state_frame_num[indx]);

   }

   if (indx > -1)
   {

      sprintf(msg, "stdf rewind to:%d [%d]\n", frame_num, -ff);

//      mLog.log_add_prefixed_textf(LOG_NET_stdf_rewind, -1, "stdf rewind to:%d [%d]\n", frame_num, -ff);

      mNetgame.state_to_game_vars(history_state[indx]);
      mLoop.frame_num = history_state_frame_num[indx];

      // fast forward and save rewind states
      for (int i=0; i<ff; i++)
      {
         mLoop.loop_frame(1);
         add_state(mLoop.frame_num);
      }
   }



   mLog.log_add_prefixed_textf(LOG_NET_stdf_rewind, -1, "%s\n", msg);
   mLog.add_log_net_db_row(LOG_NET_stdf_rewind, 0, 0,    "%s", msg);





}







// called when client needs a base state to apply a dif to
// searches for a match for passed frame_num
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





// for state exchange:
// server calls after making new state for sending
// client calls after applying new state

// for server rewind:
// called after making new state for sending
// called when ff after rewind after each ff to update states

// in mwLoop: called to set initial state
// called in program state 20 - server new game
// called in setup common after level load if ima server



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

   // check if we just invalidated last ack state by adding a new state
   if (last_ack_state_frame_num > -1) set_ack_state(last_ack_state_frame_num);
}
