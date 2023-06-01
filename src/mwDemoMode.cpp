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
   demo_mode_countdown_reset = 2400;
   pass = 0;
   prev_lev = -1;
   restore_mode = 0;
   restore_level = 1;
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

void mwDemoMode::load_random_demo(void)
{
   int debug_print = 0;
   if (!demo_mode_loaded)
   {
      demo_mode_countdown_reset = 2400;
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

      demo_mode_loaded = 1;
   }
   if (num_demo_filenames == 0)
   {
      printf("No demo files found.\n");
      demo_mode_on = 0;
   }

   int lev;

   if (debug_print) printf("\n----------------------------------\n");
   if (debug_print) printf("------Pass:%d--Prev Lev:%d--------\n", pass, prev_lev);

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

      lev = -1;
      while (lev < 0)
      {
         lev = rand() % num_demo_filenames;      // get random index
         if (debug_print) printf("New random level:%d", lev);

         if (demo_played[lev] >= pass) // already been played this pass
         {
            if (debug_print) printf("  -  already been played this pass\n");
            lev = -1;
         }
         if (prev_lev == lev)         // just previously played
         {
            if (debug_print) printf("  -  just previously played\n");
            lev = -1;
         }
//         if (debug_print) printf("\n");
         timeout++;
         if (timeout > 100) lev = 0;
      }
      demo_played[lev] = pass;
      prev_lev = lev;
   }
   else lev = 0;

   if (debug_print) printf("  ----------------  selected!\n");

   if (mGameMoves.load_gm(al_get_fs_entry_name(demo_FS_filenames[lev])))
   {
      if (debug_print) printf("pass:%d - playing demo level:%d\n", pass, mLevel.play_level);
      mLoop.new_program_state = 31;
   }
   else
   {
      demo_mode_on = 0;
      mLoop.new_program_state = 1;
      mLoop.old_program_state = 1;
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

   if ((mInput.key[ALLEGRO_KEY_N][0]) && (demo_mode_on))
      mLoop.new_program_state = 12; // skip to next demo mode level

   if ((mInput.key[ALLEGRO_KEY_ESCAPE][0]) || (mInput.key[ALLEGRO_KEY_ENTER][0]) || (mInput.key[ALLEGRO_KEY_SPACE][0]))
   {
      mInput.key[ALLEGRO_KEY_ESCAPE][0] = 0;
      mLoop.new_program_state = 32; // demo level cleanup and exit
   }

// if I also want to use escape here, I need to wait for release or esc will be passed to next state


   // if games_moves doesn't end with level_done kill it after 4 seconds
   if (mLoop.frame_num > demo_mode_last_frame + 160)
      mLoop.new_program_state = 12; // demo level timeout

}




