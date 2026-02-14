// mwDemoMode.cpp

#include "pm.h"
#include "mwDemoMode.h"
#include "mwLoop.h"
#include "mwLevel.h"
#include "mwPlayer.h"
#include "mwGameMoves.h"
#include "mwInput.h"
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

void mwDemoMode::initialize()
{
   pass = 0;
   prev_index = -1;
   play_mode = 0;
}


// this callback function has to be outside the class
int fill_demo_array(ALLEGRO_FS_ENTRY *fs, void *)
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


// fills the filename array for random selection
int mwDemoMode::load_demo_file_array()
{
   if (!files_for_random_loaded)
   {
      num_demo_filenames = 0;

      // iterate levels in demo folder and put in filename array
//      al_for_each_fs_entry(al_create_fs_entry("savegame/demo"), fill_demo_array, nullptr);
      al_for_each_fs_entry(al_create_fs_entry("C:/Users/m/Desktop/test_demo"), fill_demo_array, nullptr);

   }
   if (num_demo_filenames == 0)
   {
      printf("No demo files found.\n");
      play_mode = 0;
      return 0;
   }
   files_for_random_loaded = 1;
   //printf("\nDemo mode. List of demo files found\n");
   //for (int i=0; i< num_demo_filenames; i++)
   //   printf("%s\n", al_get_fs_entry_name(demo_FS_filenames[i]));
   return 1;
}

// very fast, no drawing
void mwDemoMode::play_demo_for_stats()
{
   mLevel.load_level(mLevel.play_level, 0, 0);    // load level

   for (int p=0; p<NUM_PLAYERS; p++)
   {
      mPlayer.init_player(p, 1);      // full reset
      mItem.set_player_start_pos(p);  // get starting position for all players, active or not
   }
   mPlayer.syn[0].active = 1;
   mPlayer.syn[0].control_method = PM_PLAYER_CONTROL_METHOD_DEMO_MODE;

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
}


void mwDemoMode::play_all_demos_and_save_stats(const int x, const int y)
{
   if (!load_demo_file_array()) return;

   // save initial state
   const int old_demo_debug_running_demo_saves_level_data = demo_debug_running_demo_saves_level_data; // get initial state
   const int old_sound_on = mSound.sound_on;

   demo_debug_running_demo_saves_level_data = 1; // force save stats
   mSound.sound_on = 0; // no sound


   for (int i=0; i<num_demo_filenames; i++)
   {
      mGameMoves.load_gm(al_get_fs_entry_name(demo_FS_filenames[i]));
      play_demo_for_stats();
      mScreen.draw_percent_bar(x, y, 200, 20, (i+1)*100 / num_demo_filenames);
      al_flip_display();
   }

   // restore initial state
   demo_debug_running_demo_saves_level_data = old_demo_debug_running_demo_saves_level_data;
   mSound.sound_on = old_sound_on;

   // restore local player color and name
   mConfig.load_config();
}



int mwDemoMode::load_random_demo()
{
   if (!load_demo_file_array())
   {
      play_mode = 0;
      return 0;
   }

   int index;

   //bool debug_print = false;
   //if (debug_print) printf("\n----------------------------------\n");
   //if (debug_print) printf("------Pass:%d--Prev Lev:%d--------\n", pass, prev_index);

   if (num_demo_filenames > 1)
   {
      // have all levels this pass been played?
      int all_played = 1;
      for (int i=0; i< num_demo_filenames; i++)
      {
         //if (debug_print) printf("demo_played[%d] - %d \n", i, demo_played[i]);
         if (demo_played[i] < pass) all_played = 0;
      }

      if (all_played == 1)
      {
         pass++; // next pass
         //if (debug_print) printf("All levels have been played this pass - next pass:%d\n", pass);
      }

      int timeout = 0;

      index = -1;
      while (index < 0)
      {
         index = rand() % num_demo_filenames;      // get random index
         //if (debug_print) printf("New random level index:%d", index);

         if (demo_played[index] >= pass) // already been played this pass
         {
            //if (debug_print) printf("  -  already been played this pass\n");
            index = -1;
         }
         if (prev_index == index)         // just previously played
         {
            //if (debug_print) printf("  -  just previously played\n");
            index = -1;
         }
         timeout++;
         if (timeout > 100)
         {
            //if (debug_print) printf("\nCould not find not played level after 1000 random iterations, choosing first index and moving on\n");
            index = 0;
         }
      }
      demo_played[index] = pass;
      prev_index = index;
   }
   else index = 0;

   //if (debug_print) printf("  ----------------  selected!\n");

   if (mGameMoves.load_gm(al_get_fs_entry_name(demo_FS_filenames[index])))
   {
      //char msg[64];
      //if (debug_print) printf("Demo Mode random file chooser - pass:[%d] lev:[%2d] %s\n", pass, mLevel.play_level, mMiscFnx.chrms(mDemoMode.last_frame, msg));
      return 1;
   }
   play_mode = 0;
   return 0;
}




void mwDemoMode::check_valid_active_local_player()
{
   if (mPlayer.is_player_active_and_not_hidden(mPlayer.active_local_player)) return;

   // not valid, search for first one that is valid
   for (int p=0; p<NUM_PLAYERS; p++)
   {
      if (mPlayer.is_player_active_and_not_hidden(p))
      {
         mPlayer.active_local_player = p;
         return;
      }
   }
   printf("Error in demo mode, no active non-hidden players found\n");
   mPlayer.active_local_player = 0;
}

void mwDemoMode::set_active_local_player(int p)
{
   // if p is active and not hidden, set active_local_player to p
   if (mPlayer.is_player_active_and_not_hidden(p)) mPlayer.active_local_player = p;

   mDrawSequence.ds_draw(0, 1);

}



// checks for keys while demo is running
int mwDemoMode::key_check()
{
   // switch pov to new player
   if (mInput.key[ALLEGRO_KEY_0][0]) set_active_local_player(0);
   if (mInput.key[ALLEGRO_KEY_1][0]) set_active_local_player(1);
   if (mInput.key[ALLEGRO_KEY_2][0]) set_active_local_player(2);
   if (mInput.key[ALLEGRO_KEY_3][0]) set_active_local_player(3);
   if (mInput.key[ALLEGRO_KEY_4][0]) set_active_local_player(4);
   if (mInput.key[ALLEGRO_KEY_5][0]) set_active_local_player(5);
   if (mInput.key[ALLEGRO_KEY_6][0]) set_active_local_player(6);
   if (mInput.key[ALLEGRO_KEY_7][0]) set_active_local_player(7);

   if (mInput.key[ALLEGRO_KEY_N][0] && play_mode == 2) return 2; // next level for random mode

   int key_used = 0;

   // if waiting for continue at the end of a demo
   if (mPlayer.syn[0].level_done_mode == 5)
   {
      // any key
      for (int k = 0; k < ALLEGRO_KEY_MAX; k++)
         if (mInput.key[k][0])
         {
            key_used = 1;
            mPlayer.syn[0].level_done_mode = 1;
         }
   }


   if (!key_used) // if key not used
   {
      int key_cancel = 0;

      // any key
      //for (int k = 0; k < ALLEGRO_KEY_MAX; k++) if (mInput.key[k][0]) key_cancel = 1;

      // only these keys
      if ((mInput.key[ALLEGRO_KEY_ESCAPE][0]) || (mInput.key[ALLEGRO_KEY_ENTER][0]) || (mInput.key[ALLEGRO_KEY_SPACE][0])) key_cancel = 1;

      if (key_cancel)
      {
         mInput.key[ALLEGRO_KEY_ESCAPE][0] = false; // eat this keypress and pretend it never happened! (to avoid triggering upstream)
         return 1;
      }
   }
   return 0;
}



void mwDemoMode::run_single_from_menu()
{
   play_mode = 1;
   if ((mGameMoves.load_demo_level(mLevel.play_level)) && (mLoop.load_and_setup_level(mLevel.play_level, 4))) run_loop(2, 1, mLevel.play_level);
   play_mode = 0;
}

void mwDemoMode::run_single_from_settings()
{
   play_mode = 1;
   const int original_level = mLevel.play_level;
   if ((mGameMoves.load_gm_file_select()) && (mLoop.load_and_setup_level(mLevel.play_level, 4))) run_loop(0, 1, original_level);
   play_mode = 0;
}

void mwDemoMode::run_single_from_cmd_fn(const char* fn)
{
   play_mode = 1;
   if (mGameMoves.load_gm(fn) && mLoop.load_and_setup_level(mLevel.play_level, 4)) run_loop(0, 0, mLevel.play_level);
   play_mode = 0;
}

void mwDemoMode::run_single_from_cmd_prompt()
{
   play_mode = 1;
   if ((mGameMoves.load_gm_file_select()) && (mLoop.load_and_setup_level(mLevel.play_level, 4))) run_loop(0, 0, mLevel.play_level);
   play_mode = 0;
}

// this takes care of its own loading and running of overworld when done
void mwDemoMode::run_single_from_gate(const int lev)
{
   play_mode = 1;
   if ((mGameMoves.load_demo_level(lev)) && (mLoop.load_and_setup_level(mLevel.play_level, 4))) run_loop(3, 1, -1);
   mLevel.play_level = 1;
   mLoop.load_and_setup_level(1, 0);
   mLoop.state[0] = PM_PROGRAM_STATE_MAIN_GAME_LOOP;
   play_mode = 0;
}



void mwDemoMode::seek_to_frame(const int frame, const int draw)
{
   // printf("seek_to_frame - lev:%d\n", mLevel.play_level);
//   mGameMoves.load_gm(mGameMoves.last_loaded_gm_filename);

   mGameMoves.current_pos = 0;



   mLoop.load_and_setup_level(mLevel.play_level, 4);
   mPlayer.syn[0].active = 1;
   mPlayer.syn[0].control_method = PM_PLAYER_CONTROL_METHOD_DEMO_MODE; // to ensure that all added players are this mode also
   mLoop.ff_state = 1;
   mLoop.frame_num = 0;
   while (mLoop.frame_num < frame)
   {
      mLoop.frame_num++;
      mGameMoves.proc();
      mLoop.move_frame();
   }
   if (draw) mDrawSequence.ds_draw(0, 0);
   mLoop.ff_state = 0;
}




// this takes care of advancing the frame and drawing
// also pause and controls
void mwDemoMode::frame_advance()
{
   mEventQueue.program_update = 0;
   mDisplay.proc_scale_factor_change();

   check_valid_active_local_player();

   if (mDemoMode.controls_paused) mDrawSequence.ds_draw(0, 1);
   else
   {
      mLoop.frame_num++;
      mBitmap.update_animation();
      mGameMoves.proc();
      mLoop.move_frame();
      mDrawSequence.ds_draw(0, 1);
   }
}

bool mwDemoMode::check_level_done()
{
   // 10s past last game move
   if (mPlayer.syn[0].level_done_mode == 0 && mLoop.frame_num > last_frame + 400)
   {
      // skip players seek exit, jump to mode 5
      mPlayer.syn[0].level_done_mode = 5;
   }

   // divert before normal next level mode
   if (mPlayer.syn[0].level_done_mode == 1) return true;
   return false;
}




// this is used in all modes that end after the level is done
void mwDemoMode::run_loop(const int ti, const int tf, const int restore_level)
{
   mScreen.transition_cutscene(ti, tf); // initial to final (game)
   while (play_mode)
   {
      mEventQueue.proc(1);
      if (mEventQueue.program_update)
      {
         frame_advance();
         if (key_check() || check_level_done()) play_mode = 0;
      }
   }
   common_exit();
   mScreen.transition_cutscene(tf, ti); // initial (game) to final

   if (restore_level != -1)
   {
      mLevel.play_level = restore_level;
      mLoop.load_and_setup_level(mLevel.play_level, 5);
   }
}


void mwDemoMode::common_exit()
{
   mSound.stop_sound();
   mPlayer.active_local_player = 0;
   controls_paused = 0;
   mConfig.load_config();  // restore player color and name and speed
   mLoop.frame_speed = 40;
   mEventQueue.adjust_fps_timer(mLoop.frame_speed);
}


void mwDemoMode::continuous_random_next_level()
{
   mSound.stop_sound();
   mScreen.transition_cutscene(1, 0); // game to nothing
   if (!load_random_demo()) return;
   mLoop.load_and_setup_level(mLevel.play_level, 4);
   mScreen.transition_cutscene(0, 1); // nothing to game
}

void mwDemoMode::run_continuous_random()
{
   const int original_level = mLevel.play_level;
   play_mode = 2;
   int done = 0;
   while (!done)
   {
      if (!load_random_demo()) return;
      if (mLoop.load_and_setup_level(mLevel.play_level, 4))
      {
         //mScreen.transition_cutscene(0, 1); // nothing to game
         while (play_mode)
         {
            mEventQueue.proc(1);
            if (mEventQueue.program_update)
            {
               frame_advance();
               const int k = key_check();
               if (k == 1) { play_mode = 0; done = 1; } // quit
               if (k == 2 || check_level_done()) continuous_random_next_level();
            }
         }
         //mScreen.transition_cutscene(1, 0); // game to nothing
         common_exit();
         mLevel.play_level = original_level;
         mLoop.load_and_setup_level(mLevel.play_level, 5);
      }
   }
}


