// mwDemoMode.cpp

#include "pm.h"
#include "mwDemoMode.h"
#include "mwLoop.h"
#include "mwLevel.h"
#include "mwPlayer.h"
#include "mwGameMoves.h"
#include "mwInput.h"

mwDemoMode mDemoMode;

mwDemoMode::mwDemoMode()
{
   initialize();
}


void mwDemoMode::initialize(void)
{
   countdown_reset = 2400;
   pass = 0;
   prev_index = -1;
   mode = 0;
   restore_mode = 0;
   restore_level = 1;

   demo_debug_complete_level_on_gate_with_fire = 0;
   demo_debug_running_demo_saves_level_data = 0;
   demo_debug_convert_playback_to_record_with_fire = 0;
   demo_debug_super_fast_mode_F2 = 0;


}

// this has to be outside the class
int fill_demo_array(ALLEGRO_FS_ENTRY *fs, void * extra)
{
   if (mDemoMode.num_demo_filenames > 99) return 0; // only get 100 max
   mDemoMode.demo_FS_filenames[mDemoMode.num_demo_filenames] = al_create_fs_entry(al_get_fs_entry_name(fs));
   mDemoMode.demo_played[mDemoMode.num_demo_filenames] = 0;
   mDemoMode.num_demo_filenames++;
   return ALLEGRO_FOR_EACH_FS_ENTRY_OK;
}
int mwDemoMode::load_random_demo(void)
{
   int debug_print = 0;
   if (!files_for_random_loaded)
   {
      countdown_reset = 2400;
      num_demo_filenames = 0;
      char fname[1024];
      sprintf(fname, "savegame/demo");

      //printf("fname:%s\n", fname);
      // convert to 'ALLEGRO_FS_ENTRY' (also makes fully qualified path)
      ALLEGRO_FS_ENTRY *FS_fname = al_create_fs_entry(fname);
      //sprintf(fname, "%s", al_get_fs_entry_name(FS_fname));
      //printf("FS_fname:%s\n", fname);

      // iterate levels in demo folder and put in filename array
      al_for_each_fs_entry(FS_fname, fill_demo_array, NULL);

      //printf("\nDemo mode. List of demo files found\n");
      //for (int i=0; i< num_demo_filenames; i++)
         //printf("%s\n", al_get_fs_entry_name(demo_FS_filenames[i]));

      files_for_random_loaded = 1;
   }
   if (num_demo_filenames == 0)
   {
      printf("No demo files found.\n");
      mode = 0;
      return 0;
   }

   int index;

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
      if (debug_print) printf("Demo Mode random file chooser - pass:%d index:%d level:%d\n", pass, index, mLevel.play_level);
      printf("demo rnd - pass:%d index:%d level:%d  lf:%d\n", pass, index, mLevel.play_level, mDemoMode.last_frame);
      return 1;
   }
   else
   {
      mode = 0;
      return 0;
   }
}

void mwDemoMode::key_check(int p)
{
   if (mInput.key[ALLEGRO_KEY_0][0]) mPlayer.active_local_player = 0;
   if (mInput.key[ALLEGRO_KEY_1][0]) mPlayer.active_local_player = 1;
   if (mInput.key[ALLEGRO_KEY_2][0]) mPlayer.active_local_player = 2;
   if (mInput.key[ALLEGRO_KEY_3][0]) mPlayer.active_local_player = 3;
   if (mInput.key[ALLEGRO_KEY_4][0]) mPlayer.active_local_player = 4;
   if (mInput.key[ALLEGRO_KEY_5][0]) mPlayer.active_local_player = 5;
   if (mInput.key[ALLEGRO_KEY_6][0]) mPlayer.active_local_player = 6;
   if (mInput.key[ALLEGRO_KEY_7][0]) mPlayer.active_local_player = 7;

   // dont let alp be an inactive player
   while (!mPlayer.syn[mPlayer.active_local_player].active) // if alp not active
      if (++mPlayer.active_local_player > 7) mPlayer.active_local_player = 0;


   if ((mInput.key[ALLEGRO_KEY_ESCAPE][0]) || (mInput.key[ALLEGRO_KEY_ENTER][0]) || (mInput.key[ALLEGRO_KEY_SPACE][0]))
   {
      mInput.key[ALLEGRO_KEY_ESCAPE][0] = 0;
      mLoop.state[0] = 32; // demo level cleanup and exit
   }

   if ((mInput.key[ALLEGRO_KEY_N][0]) && (mode == 2)) mLoop.state[0] = 12; // skip to next random level

   if (mLoop.frame_num > last_frame + 160) mLoop.state[0] = 12; // if games_moves doesn't end with level_done kill it after 4 seconds





   if (demo_debug_convert_playback_to_record_with_fire)
   {
      // in single demo mode, 'C' key will allow player to take over and start playing level from current position
      if ((mode == 1) && (mInput.key[ALLEGRO_KEY_C][0]))
      {
         mode = 0;
         mPlayer.syn[0].control_method = 0; // change to single player mode

         // erase all game moves that have a higher frame number than current
         for (int x=0; x<GAME_MOVES_SIZE; x++)
         {
            if (mGameMoves.arr[x][0] >= mLoop.frame_num)
               for (int y=0; y<4; y++)
                  mGameMoves.arr[x][y] = 0;
         }

         // set new entry pos by finding index of max frame
         int max_i = 0, max_f = 0;
         for (int x=0; x<GAME_MOVES_SIZE; x++)
            if (mGameMoves.arr[x][0] > max_f)
            {
               max_f = mGameMoves.arr[x][0];
               max_i = x;
            }
         mGameMoves.entry_pos = max_i + 1;
      }
   }
}

