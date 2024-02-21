// mwDemoMode.cpp

#include "pm.h"
#include "mwDemoMode.h"
#include "mwLoop.h"
#include "mwLevel.h"
#include "mwPlayer.h"
#include "mwGameMoves.h"
#include "mwInput.h"
#include "mwMiscFnx.h"
#include "mwEventQueue.h"

#include "mwItem.h"

#include "mwDisplay.h"
#include "mwScreen.h"

#include "mwDrawSequence.h"
#include "mwBitmap.h"

#include "mwConfig.h"
#include "mwSound.h"


mwDemoMode mDemoMode;

mwDemoMode::mwDemoMode()
{
   initialize();
}

void mwDemoMode::initialize(void)
{
   pass = 0;
   prev_index = -1;
   mode = 0;
}


// this callback function has to be outside the class
int fill_demo_array(ALLEGRO_FS_ENTRY *fs, void * extra)
{
   if (mDemoMode.num_demo_filenames > 99) return ALLEGRO_FOR_EACH_FS_ENTRY_STOP; // only get 100 max
   if (!(al_get_fs_entry_mode(fs) & ALLEGRO_FILEMODE_ISDIR))
   {
      mDemoMode.demo_FS_filenames[mDemoMode.num_demo_filenames] = al_create_fs_entry(al_get_fs_entry_name(fs));
      mDemoMode.demo_played[mDemoMode.num_demo_filenames] = 0;
      mDemoMode.num_demo_filenames++;
   }
   return ALLEGRO_FOR_EACH_FS_ENTRY_OK;
}


int mwDemoMode::load_demo_file_array(void)
{
   if (!files_for_random_loaded)
   {
      num_demo_filenames = 0;

      // iterate levels in demo folder and put in filename array
      al_for_each_fs_entry(al_create_fs_entry("savegame/demo"), fill_demo_array, NULL);
   }
   if (num_demo_filenames == 0)
   {
      printf("No demo files found.\n");
      mode = 0;
      return 0;
   }
   else
   {
      files_for_random_loaded = 1;
      //printf("\nDemo mode. List of demo files found\n");
      //for (int i=0; i< num_demo_filenames; i++)
      //   printf("%s\n", al_get_fs_entry_name(demo_FS_filenames[i]));
      return 1;
   }
}


void mwDemoMode::play_demo_for_stats(void)
{
   mLevel.load_level(mLevel.play_level, 0, 0);    // load level

   for (int p=0; p<NUM_PLAYERS; p++)
   {
      mPlayer.init_player(p, 1);        // full reset
      mItem.set_player_start_pos(p);  // get starting position for all players, active or not
   }
   mPlayer.syn[0].active = 1;
   mPlayer.syn[0].control_method = PM_PLAYER_CONTROL_METHOD_DEMO_MODE; // to ensure that all added players are this mode also


   int os = mSound.sound_on;
   mSound.sound_on = 0;

   mLoop.frame_num = 0;
   int done = 0;
   while (!done)
   {
      if (mLoop.frame_num > last_frame+200) done = 1;
      if (mPlayer.syn[0].level_done_mode) done = 1;

      mLoop.frame_num++;
      mGameMoves.proc();
      mLoop.move_frame();
   }
   mSound.sound_on = os;
}




void mwDemoMode::play_all_demos_and_save_stats(int x, int y)
{
   if (!load_demo_file_array()) return;
   int old_demo_debug_running_demo_saves_level_data = demo_debug_running_demo_saves_level_data; // get initial state
   demo_debug_running_demo_saves_level_data = 1; // force save stats
   for (int i=0; i<num_demo_filenames; i++)
   {
      //printf("index:%d\n",i);
      mGameMoves.load_gm(al_get_fs_entry_name(demo_FS_filenames[i]));

      play_demo_for_stats();

      mScreen.draw_percent_bar(x, y, 200, 20, (i+1)*100 / num_demo_filenames);
      al_flip_display();
   }
   demo_debug_running_demo_saves_level_data = old_demo_debug_running_demo_saves_level_data; // restore initial state
}

int mwDemoMode::load_random_demo(void)
{
   if (!load_demo_file_array())
   {
      mode = 0;
      return 0;
   }

   int index;

   int debug_print = 0;
   if (debug_print) printf("\n----------------------------------\n");
   if (debug_print) printf("------Pass:%d--Prev Lev:%d--------\n", pass, prev_index);

   if (num_demo_filenames > 1)
   {
      // have all levels this pass been played?
      int all_played = 1;
      for (int i=0; i< num_demo_filenames; i++)
      {
         if (debug_print) printf("demo_played[%d] - %d \n", i, demo_played[i]);
         if (demo_played[i] < pass) all_played = 0;
      }

      if (all_played == 1)
      {
         pass++; // next pass
         if (debug_print) printf("All levels have been played this pass - next pass:%d\n", pass);
      }

      int timeout = 0;

      index = -1;
      while (index < 0)
      {
         index = rand() % num_demo_filenames;      // get random index
         if (debug_print) printf("New random level index:%d", index);

         if (demo_played[index] >= pass) // already been played this pass
         {
            if (debug_print) printf("  -  already been played this pass\n");
            index = -1;
         }
         if (prev_index == index)         // just previously played
         {
            if (debug_print) printf("  -  just previously played\n");
            index = -1;
         }
         timeout++;
         if (timeout > 100)
         {
            if (debug_print) printf("\nCould not find not played level after 1000 random iterations, choosing first index and moving on\n");
            index = 0;
         }
      }
      demo_played[index] = pass;
      prev_index = index;
   }
   else index = 0;

   if (debug_print) printf("  ----------------  selected!\n");

   if (mGameMoves.load_gm(al_get_fs_entry_name(demo_FS_filenames[index])))
   {
      char msg[64];
      if (debug_print) printf("Demo Mode random file chooser - pass:[%d] index:[%2d] level:[%2d]\n", pass, index, mLevel.play_level);
      printf("Demo Mode Random - pass:[%d] lev:[%2d] %s\n", pass, mLevel.play_level, mMiscFnx.chrms(mDemoMode.last_frame, msg));
      return 1;
   }
   else
   {
      mode = 0;
      return 0;
   }
}

int mwDemoMode::key_check(void)
{
   int key_used = 0;
   int new_player = -1;
   if (mInput.key[ALLEGRO_KEY_0][0]) new_player = 0;
   if (mInput.key[ALLEGRO_KEY_1][0]) new_player = 1;
   if (mInput.key[ALLEGRO_KEY_2][0]) new_player = 2;
   if (mInput.key[ALLEGRO_KEY_3][0]) new_player = 3;
   if (mInput.key[ALLEGRO_KEY_4][0]) new_player = 4;
   if (mInput.key[ALLEGRO_KEY_5][0]) new_player = 5;
   if (mInput.key[ALLEGRO_KEY_6][0]) new_player = 6;
   if (mInput.key[ALLEGRO_KEY_7][0]) new_player = 7;

   if ((!key_used) && (new_player != -1) && (new_player != mPlayer.active_local_player) && (mPlayer.syn[new_player].active))
   {
      mPlayer.active_local_player = new_player;
      key_used = 1;
   }

   // dont let alp be hidden player (for demo playback of headless server games
   if ((mPlayer.active_local_player == 0) && (mPlayer.syn[mPlayer.active_local_player].paused == 1) && (mPlayer.syn[mPlayer.active_local_player].paused_type == 3))
   {
      // set to next active player
      ++mPlayer.active_local_player;

      while (!mPlayer.syn[mPlayer.active_local_player].active)
         if (++mPlayer.active_local_player > 7)
         {
            mPlayer.active_local_player = 0;
            printf("Error in demo mode key check, no active non-hidden players found\n");
         }
   }

   if ((mInput.key[ALLEGRO_KEY_N][0]) && (mode == 2)) return 2; // next level for random mode

   if (!key_used) // if key not used
   {
      int key_cancel = 0;

      // any key
      //for (int k = 0; k < ALLEGRO_KEY_MAX; k++) if (mInput.key[k][0]) key_cancel = 1;

      // only these keys
      if ((mInput.key[ALLEGRO_KEY_ESCAPE][0]) || (mInput.key[ALLEGRO_KEY_ENTER][0]) || (mInput.key[ALLEGRO_KEY_SPACE][0])) key_cancel = 1;

      if (key_cancel)
      {
         mInput.key[ALLEGRO_KEY_ESCAPE][0] = 0; // eat this keypress and pretend it never happened! (to avoid triggering upstream)
         return 1;
      }
   }
   return 0;
}



void mwDemoMode::run_loop(int ti, int tf, int restore_level)
{
   mScreen.transition_cutscene(ti, tf); // initial to final (game)
   while (mode)
   {
      mEventQueue.proc(1);
      if (mEventQueue.program_update)
      {
         mEventQueue.program_update = 0;
         mDisplay.proc_scale_factor_change();
         mLoop.frame_num++;

         if (mPlayer.syn[0].level_done_mode)
         {
            if (mPlayer.syn[0].level_done_mode == 1) mode = 0;
            if (mLoop.frame_num > mPlayer.syn[0].level_done_frame + 280) mode = 0; // 280 frames past level done (80) for seek and 5s more
         }
         else if (mLoop.frame_num > last_frame + 400) mode = 0; // 10s past last game move

         // printf("%d lf:%d ldm:%d ldf:%d \n", mLoop.frame_num, last_frame, mPlayer.syn[0].level_done_mode, mPlayer.syn[0].level_done_frame );

         mBitmap.update_animation();
         if (key_check()) mode = 0;
         mGameMoves.proc();
         mLoop.move_frame();
         mDrawSequence.ds_draw(0, 1);
      }
   }

   mSound.stop_sound();
   mScreen.transition_cutscene(tf, ti); // initial (game) to final
   mConfig.load_config();               // restore player color

   if (restore_level != -1)
   {
      mLevel.play_level = restore_level;
      mLoop.load_and_setup_level(mLevel.play_level, 5);
   }
}

void mwDemoMode::run_single_from_menu(void)
{
   mode = 1;
   if ((mGameMoves.load_demo_level(mLevel.play_level)) && (mLoop.load_and_setup_level(mLevel.play_level, 4))) run_loop(2, 1, mLevel.play_level);
   mode = 0;
}

void mwDemoMode::run_single_from_settings(void)
{
   mode = 1;
   int original_level = mLevel.play_level;
   if ((mGameMoves.load_gm_file_select()) && (mLoop.load_and_setup_level(mLevel.play_level, 4))) run_loop(0, 1, original_level);
   mode = 0;
}

void mwDemoMode::run_single_from_cmd_fn(const char* fn)
{
   mode = 1;
   if ((mGameMoves.load_gm(fn)) && (mLoop.load_and_setup_level(mLevel.play_level, 4))) run_loop(0, 0, mLevel.play_level);
   mode = 0;
}

void mwDemoMode::run_single_from_cmd_prompt(void)
{
   mode = 1;
   if ((mGameMoves.load_gm_file_select()) && (mLoop.load_and_setup_level(mLevel.play_level, 4))) run_loop(0, 0, mLevel.play_level);
   mode = 0;
}

// this takes care of its own loading and running of overworld when done
void mwDemoMode::run_single_from_gate(int lev)
{
   mode = 1;
   if ((mGameMoves.load_demo_level(lev)) && (mLoop.load_and_setup_level(mLevel.play_level, 4))) run_loop(3, 1, -1);
   mLevel.play_level = 1;
   mLoop.load_and_setup_level(1, 0);
   mLoop.state[0] = PM_PROGRAM_STATE_MAIN_GAME_LOOP;
   mode = 0;
}



void mwDemoMode::run_continuous_random(void)
{
   int original_level = mLevel.play_level;
   mode = 2;
   int done = 0;
   while (!done)
   {
      if (!load_random_demo()) return;
      else
      {
         if (mLoop.load_and_setup_level(mLevel.play_level, 4))
         {
            //mScreen.transition_cutscene(0, 1); // nothing to game
            while (mode)
            {
               mEventQueue.proc(1);
               if (mEventQueue.program_update)
               {
                  mEventQueue.program_update = 0;
                  mDisplay.proc_scale_factor_change();
                  mLoop.frame_num++;
                  mBitmap.update_animation();
                  mGameMoves.proc();
                  mLoop.move_frame();
                  mDrawSequence.ds_draw(0, 1);

                  if (key_check() == 1) // quit
                  {
                     mode = 0;
                     done = 1;
                  }

                  int next_lev = 0;

                  if (mPlayer.syn[0].level_done_mode)
                  {
                     if (mLoop.frame_num > mPlayer.syn[0].level_done_frame + 280) next_lev = 1; // 280 frames past level done (80) for seek and 5s more
                  }
                  else if (mLoop.frame_num > last_frame + 400) next_lev = 1; // 10s past last game move


                  if (key_check() == 2) next_lev = 1; // next level

                  if (next_lev)
                  {
                     mSound.stop_sound();
                     mScreen.transition_cutscene(1, 0); // game to nothing
                     if (!load_random_demo()) return;
                     else mLoop.load_and_setup_level(mLevel.play_level, 4);
                     mScreen.transition_cutscene(0, 1); // nothing to game
                  }
               }
            }
            mSound.stop_sound();
            //mScreen.transition_cutscene(1, 0); // game to nothing
            mLevel.play_level = original_level;
            mLoop.load_and_setup_level(mLevel.play_level, 5);
         }
      }
   }
   mode = 0;
}


