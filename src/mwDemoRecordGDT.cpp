// mwDemoRecordGDT.cpp
#include "pm.h"
#include "mwDemoMode.h"
#include "mwDemoRecord.h"
#include "mwGameMoves.h"
#include "mwLevel.h"

void mwDemoRecord::gdt(void)
{
   if (!mDemoMode.load_demo_file_array()) return;

   double t1 = al_get_time();

   for (int i=0; i<mDemoMode.num_demo_filenames; i++)
   {
      //printf("index:%d\n",i);
      mGameMoves.load_gm(al_get_fs_entry_name(mDemoMode.demo_FS_filenames[i]), false);

//      printf("GDT lev:[%2d] --------------------------------\n", mLevel.play_level);

      printf("GDT lev:[%2d] ----", mLevel.play_level);


/*
      for (int x=0; x<mGameMoves.entry_pos; x++)
         if (mGameMoves.arr[x][1] == 0) // PM_GAMEMOVE_TYPE_LEVEL_START)
         {
            printf("level start found %d %d - \n", x, mGameMoves.entry_pos);

//            mGameMoves.entry_pos--;
//            mGameMoves.HEADER_num_entries = mGameMoves.entry_pos;

//            mGameMoves.gm_remove(x);

//            mGameMoves.clear_single(x);

//            printf("removed %d\n", mGameMoves.entry_pos);

         }

//      mGameMoves.gm_sort();

//      mGameMoves.save_gm(current_loaded_demo_file, 0);

//      printf("saved %d\n", mGameMoves.entry_pos);

      find_level_done();
      if (level_done_frame)
      {
         // erase past level done
         for (int x=0; x<mGameMoves.entry_pos; x++)
            if (mGameMoves.arr[x][0] > level_done_frame) mGameMoves.clear_single(x);

         find_fire_held_sections(-1);
         remove_unused_fire_held_sections(0);
         mGameMoves.remove_doubled_moves();
      }
*/


        // show all player active game moves

//      int active_found = 0;
//      int active_found2 = 0;
//      for (int x=0; x<mGameMoves.entry_pos; x++)
//      {
//         if (mGameMoves.arr[x][1] == PM_GAMEMOVE_TYPE_PLAYER_ACTIVE)
//         {
//            //printf("[%d] p:%d active\n", mGameMoves.arr[x][0], mGameMoves.arr[x][2]);
//            active_found++;
//         }
//
//         if (mGameMoves.arr[x][1] & PM_GAMEMOVE_TYPE_PLAYER_ACTIVE_FLAG)
//         {
//            //printf("[%d] p:%d active\n", mGameMoves.arr[x][0], mGameMoves.arr[x][2]);
//            active_found2++;
//         }
//
//
//
//
//      }
//      printf("active found:%d %d\n", active_found, active_found2);
//


//      // find time bewteen level done and ack
//      find_level_done();
//      printf("[%d] level done\n", level_done_frame);
//      int ack = 0;
//      for (int x=0; x<mGameMoves.entry_pos; x++)
//         if (mGameMoves.arr[x][1] == PM_GAMEMOVE_TYPE_LEVEL_DONE_ACK)
//         {
//            ack = mGameMoves.arr[x][0];
//            printf("[%d] p:%d ack\n", mGameMoves.arr[x][0], mGameMoves.arr[x][2]);
//         }
//      if (!ack) printf("no ack\n");
//      else      printf("Time between level done and ack  -------  [%d]\n", ack - level_done_frame);
//

//      // find time bewteen level done and ack
//      find_level_done();
//      if (!level_done_frame) printf(" level not done ");
//      int ack = 0;
//      for (int x=0; x<mGameMoves.entry_pos; x++)
//         if (mGameMoves.arr[x][1] == PM_GAMEMOVE_TYPE_LEVEL_DONE_ACK)
//         {
//            ack = mGameMoves.arr[x][0];
//            //printf("[%d] p:%d ack\n", mGameMoves.arr[x][0], mGameMoves.arr[x][2]);
//         }
//      if (ack) printf(" ack ");
//      printf("\n");

      // add shot config and save
      //mGameMoves.add_game_move_shot_config(1);
      //mGameMoves.save_gm(current_loaded_demo_file, 0);
      // change shot config
//      for (int x=0; x<mGameMoves.entry_pos; x++)
//         if (mGameMoves.arr[x][1] == PM_GAMEMOVE_TYPE_SHOT_CONFIG)
//         {
//            mGameMoves.arr[x][2] = 0;
//         }
//      mGameMoves.save_gm(current_loaded_demo_file, 0);
      // show shot config
//      printf("pvs:%d pvs:%d dmg:%d\n", mPlayer.syn[0].player_vs_player_shots, mPlayer.syn[0].player_vs_self_shots, mPlayer.syn[0].player_vs_player_shot_damage);

//      mGameMoves.save_gm(current_loaded_demo_file, 0);
   }
   float et = (float) ( (al_get_time() - t1) * 1);
   printf("\ngdt took %f seconds to process %d files\n", et, mDemoMode.num_demo_filenames);
}







