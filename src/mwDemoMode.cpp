// mwDemoMode.cpp

#include "pm.h"
#include "mwDemoMode.h"
#include "mwProgramState.h"
#include "mwLevel.h"

#include "mwGameMovesArray.h"




mwDemoMode mwDM;

int fill_demo_array(ALLEGRO_FS_ENTRY *fs, void * extra)
{
   if (mwDM.num_demo_filenames > 99) return 0; // only get 100 max
   mwDM.demo_FS_filenames[mwDM.num_demo_filenames] = al_create_fs_entry(al_get_fs_entry_name(fs));
   mwDM.demo_played[mwDM.num_demo_filenames] = 0;
   mwDM.num_demo_filenames++;
   return ALLEGRO_FOR_EACH_FS_ENTRY_OK;
}

void mwDemoMode::demo_mode(void)
{
   if (!demo_mode_loaded)
   {
      demo_mode_countdown_reset = 2400;

      num_demo_filenames = 0;

      char fname[1024];
      sprintf(fname, "savegame/demo");

      //printf("fname:%s\n", fname);

      // convert to 'ALLEGRO_FS_ENTRY' (also makes fully qualified path)
      ALLEGRO_FS_ENTRY *FS_fname = al_create_fs_entry(fname);
      sprintf(fname, "%s", al_get_fs_entry_name(FS_fname));

      //printf("FS_fname:%s\n", fname);

      // iterate levels in demo folder and put in filename array
      al_for_each_fs_entry(FS_fname, fill_demo_array, NULL);


      //printf("\nDemo mode. List of demo files found\n");
   //   for (int i=0; i< num_demo_filenames; i++)
   //     printf("%s\n", al_get_fs_entry_name(demo_FS_filenames[i]));

      demo_mode_loaded = 1;
   }

   demo_mode_on = 1;


   int debug_print = 0;

   if (num_demo_filenames == 0)
   {
      printf("No demo files found.\n");
      demo_mode_on = 0;
   }

   int prev_lev = -1, lev, pass = 1;

   if (num_demo_filenames > 1)
   {
      // have all levels this pass been played?
      int all_played = 1;
      for (int i=0; i< num_demo_filenames; i++)
         if (demo_played[i] < pass) all_played = 0;
      if (all_played == 1) pass++; // next pass

      lev = -1;
      while (lev < 0)
      {
         lev = rand() % num_demo_filenames;      // get random index

//            printf("Pass:%d Level:%d\n", pass, lev);

         if (debug_print) printf("\nNew random level:%d", lev);

         if (demo_played[lev] >= pass) // already been played this pass
         {
            if (debug_print) printf("  -  already been played this pass");
            lev = -1;
         }
         if (prev_lev == lev)         // just previously played
         {
            if (debug_print) printf("  -  just previously played");
            lev = -1;
         }
         if (debug_print) printf("\n");


         if (debug_print) for (int i=0; i< num_demo_filenames; i++) printf("demo_played[%d] - %d \n", i, demo_played[i]);

//            if (debug_print) for (int i=0; i< num_demo_filenames; i++) printf("demo_played[%d] - %d \n", i, demo_played[i]);

      }
      demo_played[lev] = pass;
      prev_lev = lev;
   }
   else lev = 0;

   if (mwGMA.load_gm(al_get_fs_entry_name(demo_FS_filenames[lev])))
   {
      printf("pass:%d - playing demo level:%d\n", pass, mLevel.play_level);
      mwPS.new_program_state = 14;
      mwPS.old_program_state = 2;
   }
   else
   {
      demo_mode_on = 0;
      mwPS.new_program_state = 1;
      mwPS.old_program_state = 1;
   }
}













