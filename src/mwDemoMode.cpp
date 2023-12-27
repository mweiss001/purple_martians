// mwDemoMode.cpp

#include "pm.h"
#include "mwDemoMode.h"
#include "mwLoop.h"
#include "mwLevel.h"
#include "mwPlayer.h"
#include "mwGameMoves.h"
#include "mwInput.h"
#include "mwItem.h"
#include "mwEventQueue.h"

#include "mwDisplay.h"
#include "mwScreen.h"
#include "mwFont.h"
#include "mwColor.h"
#include "mwWidget.h"

#include "mwDrawSequence.h"
#include "mwBitmap.h"


#include "mwQuickGraph2.h"
#include "mwMenu.h"
#include "mwConfig.h"
#include "mwBottomMessage.h"
#include "mwTriggerEvent.h"
#include "mwShot.h"
#include "mwSound.h"


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


int mwDemoMode::load_demo_file_array(void)
{
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
   else return 1;
}

void mwDemoMode::gdt(void)
{
   if (!load_demo_file_array()) return;

   for (int i=0; i<num_demo_filenames; i++)
   {
      //printf("index:%d\n",i);
      mGameMoves.load_gm(al_get_fs_entry_name(demo_FS_filenames[i]));

//      printf("GDT lev:[%2d] --------------------------------\n", mLevel.play_level);

      printf("GDT lev:[%2d] ----", mLevel.play_level);



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



//        // show all player active game moves
//      printf("GDT lev:[%2d] -----", mLevel.play_level);
//      for (int x=0; x<mGameMoves.entry_pos; x++)
//         if (mGameMoves.arr[x][1] == PM_GAMEMOVE_TYPE_PLAYER_ACTIVE)
//         {
//            printf("[%d] p:%d active\n", mGameMoves.arr[x][0], mGameMoves.arr[x][2]);
//         }



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
//      printf("Time between level done and ack  -------  [%d]\n", ack - level_done_frame);

      // add shot config and save
      //mGameMoves.add_game_move_shot_config(1);
      //mGameMoves.save_gm(current_loaded_demo_file);



      // change shot config
//      for (int x=0; x<mGameMoves.entry_pos; x++)
//         if (mGameMoves.arr[x][1] == PM_GAMEMOVE_TYPE_SHOT_CONFIG)
//         {
//            mGameMoves.arr[x][2] = 0;
//         }
//      mGameMoves.save_gm(current_loaded_demo_file);






      // show shot config
//      printf("pvs:%d pvs:%d dmg:%d\n", mPlayer.syn[0].player_vs_player_shots, mPlayer.syn[0].player_vs_self_shots, mPlayer.syn[0].player_vs_player_shot_damage);


      mGameMoves.save_gm(current_loaded_demo_file);



   }
}



void mwDemoMode::play_demo_for_stats(void)
{
   mLevel.load_level(mLevel.play_level, 0, 0);    // load level

   for (int p=0; p<NUM_PLAYERS; p++)
   {
      mPlayer.init_player(p, 1);        // full reset
      mPlayer.set_player_start_pos(p);  // get starting position for all players, active or not
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

//      seek_to_frame(last_frame, 0);


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
      printf("Demo Mode Random - pass:[%d] lev:[%2d] %s\n", pass, mLevel.play_level, mItem.chrms(mDemoMode.last_frame, msg));
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
            if (mLoop.frame_num > mPlayer.syn[0].level_done_frame + 280) mode = 0; // 280 frames past level done (80) for seek and 5s more
         }
         else if (mLoop.frame_num > last_frame + 400) mode = 0; // 10s past last game move

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

   mLevel.play_level = restore_level;
   if (mLevel.play_level == 1)
   {
      mLoop.load_and_setup_level(restore_level, 0);
      mLoop.state[0] = PM_PROGRAM_STATE_MAIN_GAME_LOOP;
   }
   else mLoop.load_and_setup_level(restore_level, 5);
}

void mwDemoMode::run_single_from_menu(void)
{
   mode = 1;
   if ((mGameMoves.load_demo_level(mLevel.play_level)) && (mLoop.load_and_setup_level(mLevel.play_level, 4))) run_loop(2, 1, mLevel.play_level);
   mode = 0;
}

void mwDemoMode::run_single_from_settings(void)
{
   int original_level = mLevel.play_level;
   mode = 1;
   if ((mGameMoves.load_gm_file_select()) && (mLoop.load_and_setup_level(mLevel.play_level, 4))) run_loop(0, 1, original_level);
   mode = 0;
}

void mwDemoMode::run_single_from_gate(int lev)
{
   mode = 1;
   if ((mGameMoves.load_demo_level(lev)) && (mLoop.load_and_setup_level(mLevel.play_level, 4))) run_loop(3, 1, 1);
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






























void mwDemoMode::seek_to_frame(int frame, int draw)
{
   mLevel.load_level(mLevel.play_level, 0, 0);    // load level

   for (int p=0; p<NUM_PLAYERS; p++)
   {
      mPlayer.init_player(p, 1);        // full reset
      mPlayer.set_player_start_pos(p);  // get starting position for all players, active or not
   }
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





















void mwDemoMode::load_lnk_arr(void)
{
   lnk_entry_pos = 0;
   for (int i=0; i<mGameMoves.entry_pos; i++)
   {
      int add = 0;
      if (gm_list_all) add = 1;
      else
      {
         if (mGameMoves.arr[i][1] == PM_GAMEMOVE_TYPE_SHOT_CONFIG) add = 1;
         if (mGameMoves.arr[i][1] == PM_GAMEMOVE_TYPE_LEVEL_START) add = 1;
         if (mGameMoves.arr[i][2] == player_sections[current_section][1]) add = 1;
      }
      if (add)
      {
         lnk_arr[lnk_entry_pos] = i;
         if (++lnk_entry_pos > 9999)
         {
            printf("lnk_arr ran out of room\n");
            return;
         }
      }
   }
}



void mwDemoMode::draw_gm_txt_lines(int x, int y, int dump)
{
   int num_lines = gm_list_lines;

   // ideally the current position should be in the middle of the list

   // find game move index closest to current frame
   int i_closest = 0;
   int dist = 999999;

   for (int i=0; i<lnk_entry_pos; i++)
   {
      int td = abs(mGameMoves.arr[lnk_arr[i]][0] - mLoop.frame_num);
      if (td < dist)
      {
         dist = td;
         i_closest = i;
      }
   }

   // go back half of num_lines (but don't go back past zero)
   int gm1 = i_closest - num_lines/2;
   if (gm1 < 0) gm1 = 0;

   // set end of range
   int gm2 = gm1 + num_lines;
   if (gm2 > lnk_entry_pos) gm2 = lnk_entry_pos;

   // recalc num of lines (in case we are past the end)
//   num_lines = gm2 - gm1;
   y -= num_lines*8;

   char msg[256];

   int x1 = x;
   int x2 = x1+328+1;
   int y1 = y-1;
   int y2 = y1+num_lines*8+2;


   if (!dump)
   {

      // erase and frame main area
      al_draw_filled_rectangle(x1, y1, x2, y2, mColor.pc[0]);
      al_draw_rectangle(       x1, y1, x2, y2, mColor.pc[15], 1);

      // erase and frame bottom control bar
      al_draw_filled_rectangle(x1, y2, x2, y2+12, mColor.pc[0]);
      al_draw_rectangle(       x1, y2, x2, y2+12, mColor.pc[15], 1);

      int ya = y2+2;
      int old_gm_list_all = gm_list_all;
      mWidget.togglec(x1+2, ya, x1+30, 10,  0,0,0,0,  0,0,0,0, 1,0,0,0, gm_list_all, "all", 15, 15);
      if (old_gm_list_all != gm_list_all) load_lnk_arr();
      mWidget.slideri(x1+50, ya, x1+250, 10,  0,0,0,0,  0,12,15,15,  0,0,0,0, gm_list_lines, 100, 10, 1, "lines:");

      mWidget.togglec(x2-52, ya, x2-32, 10,  0,0,0,0,  0,0,0,0, 1,0,0,0, gm_list_mono, "mono", 15, 15);

   }

   for (int i=gm1; i<gm2; i++)
   {


      int gi = lnk_arr[i]; // get actual game move index from link translation table
      int f = mGameMoves.arr[gi][0];
      int t = mGameMoves.arr[gi][1];
      int p = mGameMoves.arr[gi][2];
      int v = mGameMoves.arr[gi][3];
      sprintf(msg, "%s", mGameMoves.get_gm_text2(i, f, t, p, v, msg));
      if (dump) printf("%s\n", msg);

      // line dimensions for background draw and mouse detection
      int tx2 = x+strlen(msg)*8;


      int text_col = mPlayer.syn[p].color;
      if (t == PM_GAMEMOVE_TYPE_LEVEL_START) text_col = 15;
      if (t == PM_GAMEMOVE_TYPE_SHOT_CONFIG) text_col = 15;
      if (gm_list_mono) text_col = 15;



      int bkg_col = 0;

      // if f == current frame higlight in blue
      // otherwise highlight both adjoining moves

      if (f == current_frame_num) bkg_col = 12+128;
      if (f < current_frame_num)
      {
         // get next frame number
         int ni = i+1;
         if (ni >= lnk_entry_pos-1) ni = lnk_entry_pos-1;
         int nf = mGameMoves.arr[lnk_arr[ni]][0];
         if (nf > current_frame_num) bkg_col = 12+128;
      }
      if (f > current_frame_num)
      {
         // get prev frame number
         int pi = i-1;
         if (pi < 0) pi = 0;
         int pf = mGameMoves.arr[lnk_arr[pi]][0];
         if (pf < current_frame_num) bkg_col = 12+128;
      }

      // determine if mouse is on line, and set background color if it is
      int mouse_on_line = 0;
      y2 = y+8;
      if ((mInput.mouse_x > x1) && (mInput.mouse_x < x2) && (mInput.mouse_y >= y) && (mInput.mouse_y < y2))
      {
         bkg_col = 10+128;
         mouse_on_line = 1;

         if (mInput.mouse_b[2][0])
         {
            mInput.mouse_y = y2+8;
            al_set_mouse_xy(mDisplay.display, mInput.mouse_x * mDisplay.display_transform_double, mInput.mouse_y * mDisplay.display_transform_double);
         }
      }

      al_draw_filled_rectangle(x1, y, tx2, y2, mColor.pc[bkg_col]); // background color
      al_draw_text(mFont.pr8, mColor.pc[text_col], x1, y, 0, msg);

      y = y2; // advance y pos for next line


      // this is purposely here, after drawing the line
      if (mouse_on_line)
      {
         proc_gm_list_menu(gi);
         if (mInput.mouse_b[1][0])
         {
            while (mInput.mouse_b[1][0]) mEventQueue.proc(1);
            current_frame_num = f;
            refresh();
         }
      }


   }
}


void mwDemoMode::copy_ptp(int sp, int dp)
{

   // erase all moves for this player
   for (int x=0; x<mGameMoves.entry_pos; x++)
   {
      int do_erase = 0;
      if (mGameMoves.arr[x][2] == dp) do_erase = 1; // player num match
      if (mGameMoves.arr[x][1] == PM_GAMEMOVE_TYPE_LEVEL_START) do_erase = 0;
      if (mGameMoves.arr[x][1] == PM_GAMEMOVE_TYPE_SHOT_CONFIG) do_erase = 0;
      if (do_erase) mGameMoves.clear_single(x);
   }

   for (int x=0; x<mGameMoves.entry_pos; x++)
   {
      int do_copy = 0;
      if (mGameMoves.arr[x][2] == sp) do_copy = 1;
      if (mGameMoves.arr[x][1] == PM_GAMEMOVE_TYPE_LEVEL_START) do_copy = 0;
      if (mGameMoves.arr[x][1] == PM_GAMEMOVE_TYPE_SHOT_CONFIG) do_copy = 0;

      if (do_copy)
      {

         if (mGameMoves.arr[x][1] == PM_GAMEMOVE_TYPE_PLAYER_ACTIVE)
            mGameMoves.add_game_move2(mGameMoves.arr[x][0], mGameMoves.arr[x][1], dp, mPlayer.syn[dp].color);

         else mGameMoves.add_game_move2(mGameMoves.arr[x][0], mGameMoves.arr[x][1], dp, mGameMoves.arr[x][3]);

//      record_player_color = mPlayer.syn[record_player_number].color;

//               if (mGameMoves.arr[x][1] == PM_GAMEMOVE_TYPE_PLAYER_ACTIVE)
         // fix color

      }

   }

   mGameMoves.gm_sort();
   refresh();

}



void mwDemoMode::proc_gm_list_menu(int gi)
{
   if (mInput.mouse_b[2][0])
   {
      int f = mGameMoves.arr[gi][0];
      int t = mGameMoves.arr[gi][1];
      int p = mGameMoves.arr[gi][2];
      int m = mGameMoves.arr[gi][3];

      int mi=0;

      sprintf(mMenu.menu_string[mi++],"Edit");
      sprintf(mMenu.menu_string[mi++],"Copy");
      sprintf(mMenu.menu_string[mi++],"Delete");
      sprintf(mMenu.menu_string[mi++],"Dump");
      sprintf(mMenu.menu_string[mi++],"Erase After");

      sprintf(mMenu.menu_string[mi++],"copy to p1");

      sprintf(mMenu.menu_string[mi++],"end");


      int mr = mMenu.pmenu(5, 13, -4, 99);

      mi=0;

      if (mr == mi++) // edit
      {
         edit_gm(gi);
      }
      if (mr == mi++) // copy
      {
         mGameMoves.add_game_move2(f, t, p, m);
         mGameMoves.gm_sort();
         refresh();
         set_active_section(current_section);
      }


      if (mr == mi++) // delete
      {
         mGameMoves.gm_remove(gi);
         mGameMoves.gm_sort();
         refresh();
         set_active_section(current_section);
      }

      if (mr == mi++) // dump
      {
         draw_gm_txt_lines(0, 0, 1);
      }

      if (mr == mi++) // erase after
      {
         for (int x=0; x<mGameMoves.entry_pos; x++)
            if ((mGameMoves.arr[x][2] == p) && (mGameMoves.arr[x][0] > f)) mGameMoves.clear_single(x);

         mGameMoves.gm_sort();
         refresh();
      }


      if (mr == mi++) // copy to p1
      {
         copy_ptp(p, 1);
         copy_ptp(p, 2);
         copy_ptp(p, 3);
         copy_ptp(p, 4);
      }





   }
}

void mwDemoMode::proc_edit_gm_type_menu(int & t)
{
   if (mInput.mouse_b[2][0])
   {
      mInput.mouse_y+=8;
      al_set_mouse_xy(mDisplay.display, mInput.mouse_x * mDisplay.display_transform_double, mInput.mouse_y * mDisplay.display_transform_double);

      sprintf(mMenu.menu_string[0],"PLAYER_MOVE");
      sprintf(mMenu.menu_string[1],"PLAYER_ACTIVE");
      sprintf(mMenu.menu_string[2],"PLAYER_INACTIVE");
      sprintf(mMenu.menu_string[3],"SHOT_CONFIG");
      sprintf(mMenu.menu_string[4],"PLAYER_HIDDEN");
      sprintf(mMenu.menu_string[5],"LEVEL_DONE_ACK");
      sprintf(mMenu.menu_string[6],"LEVEL_START");
      sprintf(mMenu.menu_string[7],"end");
      switch (mMenu.pmenu(5, 13, -4, 99))
      {
         case 0: t = PM_GAMEMOVE_TYPE_PLAYER_MOVE; break;
         case 1: t = PM_GAMEMOVE_TYPE_PLAYER_ACTIVE; break;
         case 2: t = PM_GAMEMOVE_TYPE_PLAYER_INACTIVE; break;
         case 3: t = PM_GAMEMOVE_TYPE_SHOT_CONFIG; break;
         case 4: t = PM_GAMEMOVE_TYPE_PLAYER_HIDDEN; break;
         case 5: t = PM_GAMEMOVE_TYPE_LEVEL_DONE_ACK; break;
         case 6: t = PM_GAMEMOVE_TYPE_LEVEL_START; break;
      }

      mInput.mouse_y-=8;
      al_set_mouse_xy(mDisplay.display, mInput.mouse_x * mDisplay.display_transform_double, mInput.mouse_y * mDisplay.display_transform_double);

   }
}

void mwDemoMode::proc_edit_gm_player_num_menu(int & p)
{
   if (mInput.mouse_b[2][0])
   {
      mInput.mouse_y+=8;
      al_set_mouse_xy(mDisplay.display, mInput.mouse_x * mDisplay.display_transform_double, mInput.mouse_y * mDisplay.display_transform_double);

      sprintf(mMenu.menu_string[0],"P0");
      sprintf(mMenu.menu_string[1],"P1");
      sprintf(mMenu.menu_string[2],"P2");
      sprintf(mMenu.menu_string[3],"P3");
      sprintf(mMenu.menu_string[4],"P4");
      sprintf(mMenu.menu_string[5],"P5");
      sprintf(mMenu.menu_string[6],"P6");
      sprintf(mMenu.menu_string[7],"P7");
      sprintf(mMenu.menu_string[8],"end");

      int mp = mMenu.pmenu(5, 13, -4, 99);

      if ((mp >= 0) && (mp < 8)) p = mp;

      mInput.mouse_y-=8;
      al_set_mouse_xy(mDisplay.display, mInput.mouse_x * mDisplay.display_transform_double, mInput.mouse_y * mDisplay.display_transform_double);

   }
}











void mwDemoMode::edit_gm(int gi)
{
   char msg[256];

   int quit = 0;

   int x1 = sb_x1-1;
   int y1 = mInput.mouse_y-6;

   int x2 = x1 + 329;
   int y2 = y1 + 46;

   int col = 14;

   int f = mGameMoves.arr[gi][0];
   int t = mGameMoves.arr[gi][1];
   int p = mGameMoves.arr[gi][2];
   int v = mGameMoves.arr[gi][3];

   while (!quit)
   {

//      x2 = x1 + 328 + mLoop.pct_x;


      // erase and frame
      al_draw_filled_rectangle(x1, y1, x2, y2, mColor.pc[col+208]);
      al_draw_rectangle(x1, y1, x2, y2, mColor.pc[col], 1);

      int xa = x1+2;
      int ya = y1+2;

      // show text line of game move being edited
      sprintf(msg, "%s", mGameMoves.get_gm_text2(gi, f, t, p, v, msg));
      al_draw_text(mFont.pr8, mColor.pc[15], xa-1, ya, 0, msg);
      ya+=9;

      al_draw_line(x1, ya, x2, ya, mColor.pc[col], 1);
      ya+=2;


      // common y's for all title bar stuff
      int tb_y1 = y1+1;
      int tb_y2 = tb_y1+9;

      int tb_x1 = x1+184;
      if (gi>999) tb_x1 += 8;

      if ((mInput.mouse_y >= tb_y1) && (mInput.mouse_y < tb_y2))
      {

         // player num pop-up menu
         if ((t != PM_GAMEMOVE_TYPE_LEVEL_START) && (t != PM_GAMEMOVE_TYPE_SHOT_CONFIG))
         {
            int p_x1 = tb_x1;
            int p_x2 = p_x1+16;
            if ((mInput.mouse_x > p_x1) && (mInput.mouse_x < p_x2))
            {
               al_draw_rectangle(p_x1, tb_y1, p_x2, tb_y2, mColor.pc[10], 1);
               proc_edit_gm_player_num_menu(p);
            }
         }

         // game move type pop-up menu
         int t_x1 = tb_x1 + 24;
         int t_x2;
         if (t == PM_GAMEMOVE_TYPE_PLAYER_MOVE)     { t_x2 = t_x1+34; }
         if (t == PM_GAMEMOVE_TYPE_PLAYER_ACTIVE)   { t_x2 = t_x1+50; }
         if (t == PM_GAMEMOVE_TYPE_PLAYER_INACTIVE) { t_x2 = t_x1+68; }
         if (t == PM_GAMEMOVE_TYPE_PLAYER_HIDDEN)   { t_x2 = t_x1+50; }
         if (t == PM_GAMEMOVE_TYPE_LEVEL_DONE_ACK)  { t_x2 = t_x1+26; }
         if (t == PM_GAMEMOVE_TYPE_SHOT_CONFIG)     { t_x1 -= 24; t_x2 = t_x1+42; }
         if (t == PM_GAMEMOVE_TYPE_LEVEL_START)     { t_x1 -= 24; t_x2 = t_x1+88; }
         if ((mInput.mouse_x > t_x1) && (mInput.mouse_x < t_x2))
         {
            al_draw_rectangle(t_x1, tb_y1, t_x2, tb_y2, mColor.pc[10], 1);
            proc_edit_gm_type_menu(t);
         }

         // toggle player controls for game move type 'PM_GAMEMOVE_TYPE_PLAYER_MOVE'
         if (t == PM_GAMEMOVE_TYPE_PLAYER_MOVE)
         {
            int c_x1 = tb_x1+72;
            int c_x2 = c_x1+48;
            if ((mInput.mouse_x > c_x1) && (mInput.mouse_x < c_x2))
            {
               int cn = (mInput.mouse_x - c_x1) / 8;
               int cnx = c_x1 + cn*8;

               al_draw_rectangle(cnx, tb_y1, cnx+8, tb_y2, mColor.pc[10], 1);

               if (mInput.mouse_b[1][0])
               {
                  while (mInput.mouse_b[1][0]) mEventQueue.proc(1);
                  v ^= (int) pow(2, cn);
               }
            }
         }
      }





      // frame number display and adjustment

      al_draw_textf(mFont.pr8, mColor.pc[9], (x1+x2)/2, ya+4, ALLEGRO_ALIGN_CENTER,  "Frame:%d", f);
      al_draw_textf(mFont.pr8, mColor.pc[15], (x1+x2)/2, ya+4, ALLEGRO_ALIGN_CENTER, "      %d", f);

      int c = 9+128;

      int bp = 10; // button padding
      int bs = 2; // button spacing

//      bp = 10 + mLoop.pct_x;

      int x7 = xa;
      int x8 = x7 + bp + 8*4;
      if (mWidget.buttont(x7, ya, x8, 16,  0,0,0,0,  0,c,15, 0,  1,0,0,0, "-100")) f-=100;
      x7 = x8 + bs; x8 = x7 + bp + 8*3;
      if (mWidget.buttont(x7, ya, x8, 16,  0,0,0,0,  0,c,15, 0,  1,0,0,0, "-10")) f-=10;
      x7 = x8 + bs; x8 = x7 + bp + 8*2;
      if (mWidget.buttont(x7, ya, x8, 16,  0,0,0,0,  0,c,15, 0,  1,0,0,0, "-1")) f--;


      x8 = x2 - 2;  x7 = x8 - (bp + 8*4);
      if (mWidget.buttont(x7, ya, x8, 16,  0,0,0,0,  0,c,15, 0,  1,0,0,0, "+100")) f+=100;
      x8 = x7 - bs; x7 = x8 - (bp + 8*3);
      if (mWidget.buttont(x7, ya, x8, 16,  0,0,0,0,  0,c,15, 0,  1,0,0,0, "+10")) f+=10;
      x8 = x7 - bs; x7 = x8 - (bp + 8*2);
      if (mWidget.buttont(x7, ya, x8, 16,  0,0,0,0,  0,c,15, 0,  1,0,0,0, "+1")) f++;

      ya+=17;

      mWidget.slideri(xa, ya, x2-2, 16,  0,0,0,0,  0,c,15,15,  0,0,1,0, f, last_frame+1000, 0, 1, "");

      if (f < 0) f = 0;



      if ((t == PM_GAMEMOVE_TYPE_LEVEL_START) || (t == PM_GAMEMOVE_TYPE_PLAYER_ACTIVE) || (t == PM_GAMEMOVE_TYPE_SHOT_CONFIG))
      {
         // erase and frame
         al_draw_filled_rectangle(x1, ya, x2, ya+18, mColor.pc[col+208]);
         al_draw_rectangle(x1, ya, x2, ya+18, mColor.pc[col], 1);
         ya+=2;
      }



      if (t == PM_GAMEMOVE_TYPE_LEVEL_START)
      {
         mWidget.slideri(xa, ya, x2-2, 16,  0,0,0,0,  0,12,15,15,  0,0,1,0, v, 100, 1, 1, "Level:");
      }


      if (t == PM_GAMEMOVE_TYPE_PLAYER_ACTIVE)
      {
         int cl = mWidget.colsel(xa, ya, x2-2, 16,  10,0,0,0,  0,12,15,15,  0,0,1,0);
         if (cl != -1) v = cl;
      }


      if (t == PM_GAMEMOVE_TYPE_SHOT_CONFIG)
      {
         mWidget.togglfc(xa,     ya, xa+20,  16, 0,0,0,0,  0,0,0,0,     1,0,0,0, p, 0b01, "Other Players", 15, 15);
         mWidget.togglfc(xa+128, ya, xa+148, 16, 0,0,0,0,  0,0,0,0,     1,0,0,0, p, 0b10, "Self", 15, 15);
         mWidget.slideri(xa+180, ya, x2-2,   16, 0,0,0,0,  0,12,15,15,  0,0,1,0, v, 100, -100, 1, "Damage:");
      }


      // erase and frame
      al_draw_filled_rectangle(x1, ya, x2, ya+24, mColor.pc[col+208]);
      al_draw_rectangle(x1, ya, x2, ya+24, mColor.pc[col], 1);
      ya+=3;


      int sp = 45;
      x7 = xa + sp;
      if (mWidget.buttont(x7, ya, x7+60, 20,  0,0,0,0,  0,6,15, 0,  1,0,0,0, "Reload"))
      {
         f = mGameMoves.arr[gi][0];
         t = mGameMoves.arr[gi][1];
         p = mGameMoves.arr[gi][2];
         v = mGameMoves.arr[gi][3];
      }
      x7 += 60 + sp;
      if (mWidget.buttont(x7, ya, x7+60, 20,  0,0,0,0,  0,14,15, 0,  1,0,0,0, "Commit"))
      {
         mGameMoves.arr[gi][0] = f;
         mGameMoves.arr[gi][1] = t;
         mGameMoves.arr[gi][2] = p;
         mGameMoves.arr[gi][3] = v;
         mGameMoves.gm_sort();
         refresh();
         set_active_section(current_section);
         quit = 1;
      }
      x7 += 60 + sp;
      if (mWidget.buttont(x7, ya, x7+60, 20,  0,0,0,0,  0,10,15, 0,  1,0,0,0, "Cancel")) quit = 1;



      al_draw_filled_rectangle(x1, ya+21, x2, ya+40, mColor.pc[0]);




      if (mInput.key[ALLEGRO_KEY_ESCAPE][0])
      {
         while (mInput.key[ALLEGRO_KEY_ESCAPE][0]) mEventQueue.proc(1);
         quit = 1;
      }
      al_flip_display();
      mEventQueue.proc(1);
   }
}






















































float screen_pos_from_frame_num(float frame, float last_frame, float sb_x1, float sb_w)
{
   return sb_x1 + (float)sb_w * ((float)frame / (float)last_frame);
}

void mwDemoMode::mark_timeline_position(int frame, int x1, int y1, int y2, int col)
{
   char msg[64];
//   al_draw_line(x1, y1+1, x1, sb_y2, mColor.pc[col], 1);
//   mScreen.draw_framed_text(x1, y1-11, 1, mFont.pr8, col, col, gettf(frame, msg));

   al_draw_line(x1, y1, x1, y2, mColor.pc[col], 1);

   mScreen.draw_framed_text(x1, y2+1, 1, mFont.pr8, col, col, gettf(frame, msg));


}

void mwDemoMode::find_level_done(void)
{
   level_done_frame = 0;
   level_done_player = -1;

   seek_to_frame(0, 0);

   mLoop.ff_state = 1;

   int done = 0;
   while (!done)
   {
      mLoop.frame_num++;
      mGameMoves.proc();
      mLoop.move_frame();
      if (mPlayer.syn[0].level_done_mode)
      {
         level_done_frame  = mPlayer.syn[0].level_done_frame;
         level_done_player = mPlayer.syn[0].level_done_player;
      }
      if (mLoop.frame_num > mDemoMode.last_frame + 100) done = 1;
   }

   mLoop.ff_state = 0;

}








void mwDemoMode::mark_player_carry(int p)
{
   for (int i=0; i<fhs_indx; i++)
      if ((mLoop.frame_num >= fhs[i][1]) && (mLoop.frame_num <= fhs[i][2]) && (fhs[i][0] == p)) fhs[i][3] = 5;

}

void mwDemoMode::mark_player_shot_used(int p, int f, int type)
{
   // mark the section that was active when this shot was fired
   for (int i=0; i<fhs_indx; i++)
      if ((p == fhs[i][0]) && (f >= fhs[i][1]) && (f <= fhs[i][2])) fhs[i][3] = type;
}

void mwDemoMode::remove_unused_fire_held_sections(int test)
{
   int sections_removed = 0;

   find_level_done();
   int aild = level_done_frame;

// exceptions
//   fhs[27][3] = 99;
//   fhs[76][3] = 99;
//   fhs[78][3] = 99;
//   fhs[184][3] = 99;
//   fhs[186][3] = 99;



   for (int i=0; i<fhs_indx; i++)
      if (fhs[i][3] == 0)
      {
         sections_removed++;
         int ild = level_done_frame;

         int p = fhs[i][0];
         int f1 = fhs[i][1];
         int f2 = fhs[i][2];
         for (int x=0; x<mGameMoves.entry_pos; x++)
            if ((mGameMoves.arr[x][1] == PM_GAMEMOVE_TYPE_PLAYER_MOVE) && (mGameMoves.arr[x][0] >= f1) && (mGameMoves.arr[x][0] < f2) && (mGameMoves.arr[x][2] == p)) mGameMoves.arr[x][3] &= ~PM_COMPMOVE_FIRE;

         if (test)
         {
            find_level_done();
            if (ild != level_done_frame)
            {
               printf("Removing section:%d  p:%d  %d-%d   ild:%d ld:%d ----  Error!\n", i, p, f1, f2, ild, level_done_frame);
            }
            printf("Removing section:%d  p:%d  %d-%d ld:%d\n", i, p, f1, f2, level_done_frame);
         }
         else printf("Removing section:%d  p:%d  %d-%d ld:%d\n", i, p, f1, f2, level_done_frame);

      }

   find_level_done();
   if (aild != level_done_frame)
   {
      printf("Error! - After removing all sections, level done has changed from %d to %d\n", aild, level_done_frame);
//      reload();
   }
   else printf("%d Sections removed\n", sections_removed);
}




void mwDemoMode::find_fire_held_sections(int p)
{
   for (int i=0; i<10000; i++)
      for (int j=0; j<4; j++)
         fhs[i][j] = 0;
   fhs_indx = 0;

   int sp = p;
   int ep = p+1;

    if (p == -1) // do all players
    {
       sp = 0;
       ep = NUM_PLAYERS;
    }


   for (p=sp; p<ep; p++)
   {
      int cf = 0;
      for (int x=0; x<mGameMoves.entry_pos; x++)
         if ((mGameMoves.arr[x][2] == p) && (mGameMoves.arr[x][1] == PM_GAMEMOVE_TYPE_PLAYER_MOVE))
         {
            int f = mGameMoves.arr[x][0]; // frame
            int c = mGameMoves.arr[x][3] & PM_COMPMOVE_FIRE; // comp move fire detect

            if ((cf == 0) && (c)) // fire section starts
            {
               cf = 1;
               fhs[fhs_indx][0] = p;
               fhs[fhs_indx][1] = f;
            }
            if ((cf == 1) && (!c)) // fire section ends
            {
               cf = 0;
               fhs[fhs_indx][2] = f;
               fhs_indx++;
            }
         }
   }

   // then play the level and detect when fire held sections are used

   seek_to_frame(0, 0);
   mLoop.ff_state = 1;
   int done = 0;
   while (!done)
   {
      mLoop.frame_num++;
      mGameMoves.proc();
      mLoop.move_frame();
      if (mLoop.frame_num > mDemoMode.last_frame + 100) done = 1;
   }
   mLoop.ff_state = 0;

   if (1)
   {
      int us = 0;
      int eh = 0;
      int ph = 0;
      int od = 0;
      int td = 0;
      int bb = 0;
      int pc = 0;

      for (int i=0; i<fhs_indx; i++)
      {
         if (fhs[i][3] == 0) us++;
         if (fhs[i][3] == 1) eh++;
         if (fhs[i][3] == 2) ph++;
         if (fhs[i][3] == 3) od++;
         if (fhs[i][3] == 4) td++;
         if (fhs[i][3] == 5) pc++;
         if (fhs[i][3] == 6) bb++;
      }

      printf("%d - total fire held sections ----------------\n", fhs_indx);
      printf("%d - unused fire held sections\n", us);
      printf("%d - enemy hits\n", eh);
      printf("%d - player hits\n", ph);
      printf("%d - breakable blocks\n", bb);
      printf("%d - orb detect\n", od);
      printf("%d - trg detect\n", td);
      printf("%d - player carry\n", pc);
   }
}





void mwDemoMode::stop_transport(void)
{
   if (play)
   {
      play = 0;
      current_frame_num = mLoop.frame_num;
      mDrawSequence.ds_draw(0, 0);
   }

   if (record)
   {
      record = 0;
      mGameMoves.gm_sort();
      refresh();
      set_active_section(current_section);
   }

}




void mwDemoMode::start_record(void)
{
   play = 1;
   record = 1;

   // erase all game moves for this player that have a frame number equal or higher than current frame number
   // also check if there still is PM_GAMEMOVE_TYPE_PLAYER_ACTIVE game move after erasing
   // never erase PM_GAMEMOVE_TYPE_SHOT_CONFIG
   int aa = 0;
   int p = record_player_number;
   for (int x=0; x<mGameMoves.entry_pos; x++)
      if ((mGameMoves.arr[x][2] == p) && (mGameMoves.arr[x][1] != PM_GAMEMOVE_TYPE_SHOT_CONFIG))
      {
         if ((mGameMoves.arr[x][0] >= mLoop.frame_num)) mGameMoves.clear_single(x); // erase all game moves that have a higher frame number than current
         if ((mGameMoves.arr[x][1] == PM_GAMEMOVE_TYPE_PLAYER_ACTIVE)) aa = 1;      // do we still have an active game move?
      }

   if (!aa) // player does not have a PM_GAMEMOVE_TYPE_PLAYER_ACTIVE game move
   {

      int spi = mPlayer.syn[p].spawn_point_index; // save spawn_point_index
      mPlayer.init_player(p, 1);                  // full player reset
      mPlayer.syn[p].spawn_point_index = spi;     // restore spawn_point_index
      mPlayer.set_player_start_pos(p);            // set starting position

      mPlayer.syn[p].active = 1;
      mPlayer.syn[p].color = record_player_color;

      // insert 'active' game move
      int fn = mLoop.frame_num;
      if (fn < 1) fn = 1;
      mGameMoves.add_game_move(fn, PM_GAMEMOVE_TYPE_PLAYER_ACTIVE, p, mPlayer.syn[p].color);

   }
   mPlayer.active_local_player = p;
   mPlayer.syn[p].control_method = PM_PLAYER_CONTROL_METHOD_SINGLE_PLAYER;
}


int mwDemoMode::load_demo_record(void)
{
   if (!mGameMoves.load_gm_file_select()) return 0;
   current_frame_num = 1;
   refresh();

   set_active_section(0);

   return 1;
}

void mwDemoMode::refresh(void)
{
   fill_player_sections();
   find_level_done();
   load_lnk_arr();    // load games moves into link array

   //get_more_player_section_info();
   //find_ack_moves(0);

   seek_to_frame(current_frame_num, 1);
}


void mwDemoMode::reload(void)
{
   mGameMoves.load_gm(current_loaded_demo_file);
   refresh();
}



void mwDemoMode::set_player_section(int i, int p, int c, int f1, int f2, int y, int end_type)
{
   player_sections[i][0] = 1; // active
   player_sections[i][1] = p;
   player_sections[i][2] = c;
   player_sections[i][3] = f1;
   player_sections[i][4] = f2;
   player_sections[i][5] = screen_pos_from_frame_num(f1, last_frame, sb_x1, sb_w);
   player_sections[i][6] = y;
   player_sections[i][7] = screen_pos_from_frame_num(f2, last_frame, sb_x1, sb_w);
   player_sections[i][8] = y - sb_size;
   player_sections[i][9] = end_type;
}

/*

   list all

   player_sections[i][0] = 1; // active
   player_sections[i][1] = p;
   player_sections[i][2] = c;
   player_sections[i][3] = f1;
   player_sections[i][4] = f2;
   player_sections[i][5] = screen_pos_from_frame_num(f1, last_frame, sb_x1, sb_w);
   player_sections[i][6] = y;
   player_sections[i][7] = screen_pos_from_frame_num(f2, last_frame, sb_x1, sb_w);
   player_sections[i][8] = y - sb_size;
   player_sections[i][9] = end_type;

   player_sections[i][10]; // deaths
   player_sections[i][11]; // level done
   player_sections[i][12]; // ack frame
   player_sections[i][13]; // hidden

   player_sections[i][14]; // number of game moves
   player_sections[i][15]; // frame of last game move

*/


void mwDemoMode::fill_player_sections(void)
{
   sb_x1 = 14;
   sb_x2 = mDisplay.SCREEN_W - 20;
   sb_w = sb_x2 - sb_x1;
   sb_y2 = mDisplay.SCREEN_H - 14;


   if (timeline_size == 5)
   {
      sb_x1 = mDisplay.SCREEN_W/2;
      sb_w = sb_x2 - sb_x1;

      sb_spacing = 1;
      sb_size = 10;
      sb_text = 1;
   }

   if (timeline_size == 4)
   {
      sb_x2 = mDisplay.SCREEN_W/5;
      sb_w = sb_x2 - sb_x1;

      sb_spacing = 1;
      sb_size = 10;
      sb_text = 1;
   }

   if (timeline_size == 3)
   {
      sb_spacing = 4;
      sb_size = 20;
      sb_text = 1;
   }

   if (timeline_size == 2)
   {
      sb_spacing = 2;
      sb_size = 10;
      sb_text = 1;
   }

   if (timeline_size == 1)
   {
      sb_spacing = 1;
      sb_size = 4;
      sb_text = 0;
   }



   // clear
   for (int i=0; i<20; i++)
      for (int j=0; j<20; j++) player_sections[i][j] = 0;

   int psi = 0;
   int y = sb_y2-sb_spacing;

   // find sections of active players
   for (int p=0; p<NUM_PLAYERS; p++)
   {
      int col; // = mPlayer.syn[p].color; // overwrite later
      int f1 = -1;
      int f2 = -1;

      player_sections[psi][14] = 0; // number of game moves
      player_sections[psi][15] = 0; // frame of last game move

      for (int x=0; x<mGameMoves.entry_pos; x++)
         if (mGameMoves.arr[x][2] == p)
         {
            player_sections[psi][14]++; // number of game moves

            player_sections[psi][15] = mGameMoves.arr[x][0]; // frame of last game move

            if (mGameMoves.arr[x][1] == PM_GAMEMOVE_TYPE_PLAYER_HIDDEN) player_sections[psi][13] = 1;

            if (mGameMoves.arr[x][1] == PM_GAMEMOVE_TYPE_PLAYER_ACTIVE)
            {
               f1 = mGameMoves.arr[x][0];
               col = mGameMoves.arr[x][3];
            }
            if (mGameMoves.arr[x][1] == PM_GAMEMOVE_TYPE_PLAYER_INACTIVE)
            {
               f2 = mGameMoves.arr[x][0];

               set_player_section(psi, p, col, f1, f2, y, 1);
               y -= (sb_size + sb_spacing);
               psi++;

               // reset pointers
               f1 = -1;
               f2 = -1;
            }
         }
      if ((f1 != -1) && (f2 == -1)) // active with no matching inactive
      {
         set_player_section(psi, p, col, f1, last_frame, y, 0);
         y -= (sb_size + sb_spacing);
         psi++;
      }
   }
   sb_y1 = y;
}

void mwDemoMode::get_more_player_section_info(void)
{
   for (int i=0; i<20; i++)
      if (player_sections[i][0])
      {
         int p   = player_sections[i][1];
         int f1  = player_sections[i][3];
         int f2  = player_sections[i][4];

         player_sections[i][10] = 0; // deaths
         player_sections[i][11] = 0; // level done

         seek_to_frame(f1, 0);
         while (mLoop.frame_num < f2)
         {
            mLoop.frame_num++;
            mGameMoves.proc();
            mLoop.move_frame();
            if ((player_sections[i][11] == 0) && (mPlayer.syn[0].level_done_mode) && (mPlayer.syn[0].level_done_player == p)) player_sections[i][11] = mLoop.frame_num;
            if ((mPlayer.syn[p].paused == 100) && (mPlayer.syn[p].paused_type == 1)) player_sections[i][10]++;
         }
      }
   seek_to_frame(current_frame_num, 1);
}

void mwDemoMode::find_ack_moves(int i)
{
   player_sections[i][12] = 0; // ack_move_frame

   // searches frame range of segment for ack moves
   int p   = player_sections[i][1];
   int f1  = player_sections[i][3];
   int f2  = player_sections[i][4];

   for (int x=f1; x<f2; x++)
      if ((mGameMoves.arr[x][2] == p) && (mGameMoves.arr[x][1] == PM_GAMEMOVE_TYPE_LEVEL_DONE_ACK))
         {
            player_sections[i][12] = mGameMoves.arr[x][0]; // ack_move_frame
            return; // quit immed after finding first ack
         }
}



void mwDemoMode::draw_section_details_one_line(int i, int x, int y)
{
   int p   = player_sections[i][1];
   int col = player_sections[i][2];
   int f1  = player_sections[i][3];
   int f2  = player_sections[i][4];

   char et[20] = {0};
   int end_type = player_sections[i][9];
   if (end_type == 0) sprintf(et, "end of file");
   if (end_type == 1) sprintf(et, "inactive");

   char hd[20] = {0};
   if (player_sections[i][13]) sprintf(hd, "[hidden]");

   char m1[64], m2[64];
   al_draw_textf(mFont.pr8, mColor.pc[col], x, y, 0, "p%d %s-%s %s %s", p, gettf(f1, m1), gettf(f2, m2), et, hd);

//   int dth = player_sections[i][10]; // deaths
//   int ld = player_sections[i][11]; // level done
//   int af  = player_sections[i][12];
//   al_draw_textf(mFont.pr8, mColor.pc[col], x, y, 0, "p%d %d-%d af:%d ld:%d dth:%d %s %s", p, f1, f2, af, ld, dth, et, hd);

}



void mwDemoMode::proc_section_details_menu(void)
{
   if (mInput.mouse_b[2][0])
   {
      int i = current_section;
      int p   = player_sections[i][1];
      int f1  = player_sections[i][3];
      int f2  = player_sections[i][4];

      sprintf(mMenu.menu_string[0],"Section Details Menu");
      sprintf(mMenu.menu_string[1],"--------------------");
      sprintf(mMenu.menu_string[2],"Delete Inactive Game Move");
      sprintf(mMenu.menu_string[3],"Add Inactive Game Move");
      sprintf(mMenu.menu_string[4],"Delete Section");
      sprintf(mMenu.menu_string[5],"Find Unused Fire Sections");
      sprintf(mMenu.menu_string[6],"Remove Unused Fire Sections");
      sprintf(mMenu.menu_string[7],"Remove Unused Fire Sections test");
      sprintf(mMenu.menu_string[8],"Remove Duplicate Game Move");

      sprintf(mMenu.menu_string[9],"end");

      int mp = mMenu.pmenu(5, 13, -12, 1);

      if (mp == 2)
      {
         for (int x=0; x<mGameMoves.entry_pos; x++)
            if ((mGameMoves.arr[x][1] == PM_GAMEMOVE_TYPE_PLAYER_INACTIVE) && (mGameMoves.arr[x][2] == p))
               if ((mGameMoves.arr[x][0] >= f1) && (mGameMoves.arr[x][0] <= f2)) mGameMoves.gm_remove(x);
         refresh();
      }
      if (mp == 3)
      {
         for (int x=0; x<mGameMoves.entry_pos; x++)
            if ((mGameMoves.arr[x][2] == p) && (mGameMoves.arr[x][0] >= current_frame_num)) mGameMoves.clear_single(x);

         mGameMoves.add_game_move2(current_frame_num, PM_GAMEMOVE_TYPE_PLAYER_INACTIVE, p, 0);
         mGameMoves.gm_sort();
         refresh();
      }
      if (mp == 4)
      {
         if (p != 0) // dont delete player 0
         {
            for (int x=0; x<mGameMoves.entry_pos; x++)
               if (mGameMoves.arr[x][2] == p) mGameMoves.clear_single(x);
            mGameMoves.gm_sort();
            refresh();
         }
      }
      if (mp == 5)
      {
         find_fire_held_sections(p);
         refresh();
      }

      if (mp == 6)
      {
         remove_unused_fire_held_sections(0);
         refresh();
      }

      if (mp == 7)
      {
         remove_unused_fire_held_sections(1);
         refresh();
      }


      if (mp == 8)
      {
         mGameMoves.remove_doubled_moves();
         refresh();
      }
   }
}




int mwDemoMode::draw_section_details(int x1, int y1)
{
   int i = current_section;
   int p   = player_sections[i][1];
   int col = player_sections[i][2];
   int f1  = player_sections[i][3];
   int f2  = player_sections[i][4];

   //int dth = player_sections[i][10]; // deaths
   //int ld = player_sections[i][11]; // level done
   //int af  = player_sections[i][12];

   char et[20] = {0};
   int end_type = player_sections[i][9];
   if (end_type == 0) sprintf(et, "end of file");
   if (end_type == 1) sprintf(et, "inactive");

//   char hd[20] = {0};
//   if (player_sections[i][13]) sprintf(hd, "[hidden]");


   int w = 208;
   //w +=  mLoop.pct_x;
   int x2 = x1 + w;

   int h = 109;
   //h +=  mLoop.pct_y;
   int y2 = y1 + h;

   if ((mInput.mouse_x > x1) && (mInput.mouse_x < x2) && (mInput.mouse_y > y1) && (mInput.mouse_y < y2)) proc_section_details_menu();


   al_draw_filled_rectangle(x1, y1, x2, y2, mColor.pc[0]);
   al_draw_rectangle(x1, y1, x2, y2, mColor.pc[col], 1);
   al_draw_line(x1, y1+12, x2, y1+12, mColor.pc[col], 1);
   al_draw_textf(mFont.pr8, mColor.pc[15], (x1+w/2), y1+2, ALLEGRO_ALIGN_CENTER, "Current Section");
   y1+=4;
   if (mWidget.buttont(x1+4,  y1, x1+12, 8,  0,0,0,0,  0,-1,15, 0,  1,0,0,0, "-")) set_active_section(current_section-1);
   if (mWidget.buttont(x2-12, y1, x2-4,  8,  0,0,0,0,  0,-1,15, 0,  1,0,0,0, "+")) set_active_section(current_section+1);
   y1+=10;



   int line_space = 10;

   int x = x1+4;
   int y = y1;



   al_draw_bitmap(mBitmap.player_tile[col][1], x+124, y1, 0 );
   al_draw_text(mFont.pr8, mColor.pc[col], x+146, y1+7, 0, mColor.color_name[col]);


   al_draw_textf(mFont.pr8, mColor.pc[15], x, y+1, 0, "Player Number:%d", p);
   y+=line_space+1;
   al_draw_text( mFont.pr8, mColor.pc[15], x, y, 0, "Player Color:");

   y+=line_space+1;

   al_draw_line(x1, y, x2, y, mColor.pc[col], 1);

   y+=3;

   al_draw_text(mFont.pr8, mColor.pc[15], (x1+w/2), y, ALLEGRO_ALIGN_CENTER, "Change Player Color");

   y+=line_space;

   int cl = mWidget.colsel(x, y, x+200, 16,  10,0,0,0,  0,12,15,15,  0,0,1,0);
   if (cl != -1)
   {
      for (int x=0; x<mGameMoves.entry_pos; x++)
         if ((mGameMoves.arr[x][1] == PM_GAMEMOVE_TYPE_PLAYER_ACTIVE) && (mGameMoves.arr[x][2] == p))
            if ((mGameMoves.arr[x][0] >= f1) && (mGameMoves.arr[x][0] <= f2)) mGameMoves.arr[x][3] = cl;
      refresh();
   }

   y+=2;

   al_draw_line(x1, y, x2, y, mColor.pc[col], 1);

   y+=2;

   char m1[64], m2[64];
   al_draw_textf(mFont.pr8, mColor.pc[15], x, y, 0, "Range:%s to %s", gettf(f1, m1), gettf(f2, m2));

   y+=line_space;

   al_draw_textf(mFont.pr8, mColor.pc[15], x, y, 0, "End Type:%s", et);

   y+=line_space;

   al_draw_textf(mFont.pr8, mColor.pc[15], x, y, 0, "Number of Moves:%d", player_sections[i][14]);
   y+=line_space;

   al_draw_textf(mFont.pr8, mColor.pc[15], x, y, 0, "Frame of Last Move:%d", player_sections[i][15]);
   y+=line_space;

   y+=2;

//      if (ld == 0) al_draw_textf(mFont.pr8, mColor.pc[col], x, y, 0, "Level Done:no");
//      else         al_draw_textf(mFont.pr8, mColor.pc[col], x, y, 0, "Level Done:%d", ld);
//      y+=line_space;
//
//      if (af == 0) al_draw_textf(mFont.pr8, mColor.pc[col], x, y, 0, "Acknowledge Level Done:no");
//      else         al_draw_textf(mFont.pr8, mColor.pc[col], x, y, 0, "Acknowledge Level Done:%d", af);
//      y+=line_space;
//
//      al_draw_textf(mFont.pr8, mColor.pc[col], x, y, 0, "Deaths:%d", dth);
//      y+=line_space;


   return y2;
}



void mwDemoMode::proc_file_details_menu(void)
{
   if (mInput.mouse_b[2][0])
   {
      sprintf(mMenu.menu_string[0],"File Details Menu");
      sprintf(mMenu.menu_string[1],"-----------------");
      sprintf(mMenu.menu_string[2],"Find Unused Fire Sections");
      sprintf(mMenu.menu_string[3],"Remove Unused Fire Sections");
      sprintf(mMenu.menu_string[4],"Remove Duplicate Game Move");
      sprintf(mMenu.menu_string[5],"Delete Past Level Done");
      sprintf(mMenu.menu_string[6],"Sort");
      sprintf(mMenu.menu_string[7],"GDT");
      sprintf(mMenu.menu_string[8],"end");

      int mp = mMenu.pmenu(5, 13, -12, 1);

      if (mp == 2)
      {
         find_fire_held_sections(-1);
         refresh();
      }

      if (mp == 3)
      {
         remove_unused_fire_held_sections(0);
         refresh();
      }

      if (mp == 4)
      {
         mGameMoves.remove_doubled_moves();
         refresh();
      }

      if (mp == 5) // delete past level done
      {
         for (int x=0; x<mGameMoves.entry_pos; x++)
            if (mGameMoves.arr[x][0] > level_done_frame) mGameMoves.clear_single(x);
         mGameMoves.gm_sort();
         refresh();
      }

      if (mp == 6) mGameMoves.gm_sort();


      if (mp == 7) gdt();



   }
}



int mwDemoMode::draw_file_details(int x1, int y1)
{
   int w = 208;
   //w +=  mLoop.pct_x;
   int x2 = x1 + w;

   int h = 109;
   //h +=  mLoop.pct_y;
   int y2 = y1 + h;

   int col = 10;

   if ((mInput.mouse_x > x1) && (mInput.mouse_x < x2) && (mInput.mouse_y > y1) && (mInput.mouse_y < y2)) proc_file_details_menu();

   al_draw_filled_rectangle(x1, y1, x2, y2, mColor.pc[0]);
   al_draw_rectangle(x1, y1, x2, y2, mColor.pc[col], 1);

   char msg[256];
   sprintf(msg, "Current File:%s", al_get_path_filename(al_create_path(current_loaded_demo_file)));
   int x2tl = x1 + 6 + strlen(msg) * 8;
   al_draw_filled_rectangle(x1, y1, x2tl, y1+12, mColor.pc[0]);
   al_draw_rectangle(x1, y1, x2tl, y1+12, mColor.pc[col], 1);
   al_draw_text(mFont.pr8, mColor.pc[15], x1+3, y1+2, 0, msg);


   int xa = x1+3;
   int xa2 = x2-3;

   int ya = y1+14;
   int bts = 16;

   int c = 9;

   if (mWidget.buttont(xa,      ya, xa+48,  bts,  0,0,0,0,  0,c,15,0,  1,0,0,0, "Load")) load_demo_record();
   if (mWidget.buttont(xa+50,   ya, xa+116, bts,  0,0,0,0,  0,c,15,0,  1,0,0,0, "Reload")) reload();
   if (mWidget.buttont(xa+118,  ya, xa2,    bts,  0,0,0,0,  0,c,15,0,  1,0,1,0, "Refresh")) refresh();

   char m1[64], m2[64];
   al_draw_textf(mFont.pr8, mColor.pc[15], xa, ya, 0, "Range:%s to %s", gettf(0, m1), gettf(last_frame, m2));
   ya+=10;

   al_draw_textf(mFont.pr8, mColor.pc[15], xa, ya, 0, "Number of Moves:%d", mGameMoves.entry_pos);
   ya+=10;

   if (level_done_frame) al_draw_textf(mFont.pr8, mColor.pc[11], xa, ya, 0, "Level Done:%s", gettf(level_done_frame, m2));
   else                  al_draw_textf(mFont.pr8, mColor.pc[11], xa, ya, 0, "Level Done:no");
   ya+=10;


   c = 4;
   if (mWidget.buttont(xa, ya, xa2, bts,  0,0,0,0,  0,c,15, 0,  1,0,1,0, "Save (overwrite current)"))
   {
      mGameMoves.save_gm(current_loaded_demo_file);
      mGameMoves.load_gm(current_loaded_demo_file);
      refresh();
   }
   if (mWidget.buttont(xa, ya, xa2, bts,  0,0,0,0,  0,c,15, 0,  1,0,1,0, "Save (auto new name)"))
   {
      mGameMoves.save_gm_make_fn("new copy");
      mGameMoves.load_gm(current_loaded_demo_file);
      refresh();
   }
   if (mWidget.buttont(xa, ya, xa2, bts,  0,0,0,0,  0,c,15, 0,  1,0,1,0, "Save (prompt for name)"))
   {
      mGameMoves.save_gm_file_select();
      mGameMoves.load_gm(current_loaded_demo_file);
      refresh();
   }

   return y2;
}














void mwDemoMode::set_active_section(int i)
{
   if ((i >= 0) && (player_sections[i][0]))
   {
      current_section = i;
      record_player_number = mPlayer.active_local_player = player_sections[current_section][1];
      record_player_color = player_sections[current_section][2];
      load_lnk_arr();    // load games moves into link array
   }
}


char * mwDemoMode::gettf(int frame, char* ft)
{
   if (tm_frame) sprintf(ft, "%d", frame);
   else sprintf(ft, "%s", mItem.chrms(frame, ft));
   return ft;
}






int mwDemoMode::draw_record_settings(int x1, int y1)
{
   int col = 10;

   int w = 148;
   //w +=  mLoop.pct_x;
   int x2 = x1 + w;

   int h = 88;
   //h +=  mLoop.pct_y;
   int y2 = y1 + h;

   al_draw_filled_rectangle(x1, y1, x2, y2, mColor.pc[0]);
   al_draw_rectangle(x1, y1, x2, y2, mColor.pc[col], 1);
   al_draw_line(x1, y1+12, x2, y1+12, mColor.pc[col], 1);
   al_draw_textf(mFont.pr8, mColor.pc[15], (x1+w/2), y1+2, ALLEGRO_ALIGN_CENTER, "Record Settings");
   y1+=14;

   int xa = x1+4;
   int xb = x2-4;

   int oldrpn = record_player_number;
   mWidget.slideri(xa, y1, xb, 16,  0,0,0,0,  0,12,15,15,  0,0,1,0, record_player_number, 7, 0, 1, "Player Number:");
   if (oldrpn != record_player_number)
   {
      mPlayer.active_local_player = record_player_number;
      record_player_color = mPlayer.syn[record_player_number].color;
   }

   int cl = mWidget.colsel(xa, y1, xb, 16,  10,0,0,0,  0,12,15,15,  0,0,1,0);
   if (cl != -1) record_player_color = cl;

   al_draw_bitmap(mBitmap.player_tile[record_player_color][1], xa, y1, 0 );
   al_draw_text(mFont.pr8, mColor.pc[record_player_color], xa+22, y1+7, 0, mColor.color_name[record_player_color]);

   y1+=24;

   al_draw_line(x1, y1, x2, y1, mColor.pc[col], 1);

   y1+=2;

   mWidget.toggle(x1+4, y1, x2-4, 16,   0,0,0,0,  0,0,0,0, 1,0,1,0, record_punch_in_armed, "Arm", "Armed", 15, 15, 9, 10);

   return y1;
}



void mwDemoMode::range_tools_set_fire_moves(int f, int clear_set)
{
   int i = current_section;
   int p = player_sections[i][1];

   // look for an existing game move for this frame and player
   int existing_move = 0;
   for (int x=0; x<mGameMoves.entry_pos; x++)
      if ((mGameMoves.arr[x][0] == f) && (mGameMoves.arr[x][1] == PM_GAMEMOVE_TYPE_PLAYER_MOVE) && (mGameMoves.arr[x][2] == p)) existing_move = x;

   if (existing_move)
   {
      if (clear_set) mGameMoves.arr[existing_move][3] |= PM_COMPMOVE_FIRE;
      else           mGameMoves.arr[existing_move][3] &= ~PM_COMPMOVE_FIRE;
   }
   else
   {
      // find previous game move for this player
      int prev_move_index = 0;
      int prev_move_frame = 0;

      // look for largest frame num that is less than f
      for (int x=0; x<mGameMoves.entry_pos; x++)
         if ((mGameMoves.arr[x][0] < f) && (mGameMoves.arr[x][1] == PM_GAMEMOVE_TYPE_PLAYER_MOVE) && (mGameMoves.arr[x][2] == p))
         {
            if (mGameMoves.arr[x][0] > prev_move_frame)
            {
               prev_move_frame = mGameMoves.arr[x][0];
               prev_move_index = x;
            }
         }

      int cm = 0;
      if (prev_move_index) cm = mGameMoves.arr[prev_move_index][3];

      if (clear_set) cm |= PM_COMPMOVE_FIRE;
      else           cm &= ~PM_COMPMOVE_FIRE;

      mGameMoves.add_game_move2(f, PM_GAMEMOVE_TYPE_PLAYER_MOVE, p, 0);
   }
}





void mwDemoMode::range_tools_do(int action)
{
   int i = current_section;
   int p = player_sections[i][1];

   for (int x=0; x<mGameMoves.entry_pos; x++)
      if ((mGameMoves.arr[x][0] >= rt_start_frame) && (mGameMoves.arr[x][0] <= rt_end_frame) && ((rt_all_players) || (mGameMoves.arr[x][2] == p)) )
      {
         if (action == 1) mGameMoves.clear_single(x);
         if (action == 2) mGameMoves.arr[x][0]++;
         if (action == 3) mGameMoves.arr[x][0]--;

         if (action == 4)
         {
            if (mGameMoves.arr[x][1] == PM_GAMEMOVE_TYPE_PLAYER_MOVE) mGameMoves.arr[x][3] &= ~ PM_COMPMOVE_FIRE;

         }




      }

   mGameMoves.gm_sort();
   refresh();


}



void mwDemoMode::proc_range_tools_menu(void)
{
   if (mInput.mouse_b[2][0])
   {
      sprintf(mMenu.menu_string[0],"Range Tools Menu");
      sprintf(mMenu.menu_string[1],"-----------------");
      sprintf(mMenu.menu_string[2],"Set Start to Current Time");
      sprintf(mMenu.menu_string[3],"Set Start to Frame 1");
      sprintf(mMenu.menu_string[4],"Set End to Current Time");
      sprintf(mMenu.menu_string[5],"Set End to Last Frame");
      sprintf(mMenu.menu_string[6],"-----------------");
      sprintf(mMenu.menu_string[7],"Delete Range");
      sprintf(mMenu.menu_string[8],"Shift Range +1");
      sprintf(mMenu.menu_string[9],"Shift Range -1");
      sprintf(mMenu.menu_string[10],"Remove FIRE from Range");
      sprintf(mMenu.menu_string[11],"Add FIRE to Range");
      sprintf(mMenu.menu_string[12],"end");

      int mp = mMenu.pmenu(5, 13, -12, 1);

      if (mp == 2) rt_start_frame = current_frame_num;
      if (mp == 3) rt_start_frame = 1;
      if (mp == 4) rt_end_frame = current_frame_num;
      if (mp == 5) rt_end_frame = last_frame;

      if (mp == 7) range_tools_do(1);
      if (mp == 8) range_tools_do(2);
      if (mp == 9) range_tools_do(3);
      if (mp == 10) range_tools_do(4);


      if (mp == 11)
      {
         range_tools_do(4); // delete all fire

         for (int f=rt_start_frame; f<=rt_end_frame; f+=4)
         {
            range_tools_set_fire_moves(f, 1);
            range_tools_set_fire_moves(f+1, 0);
         }

         mGameMoves.gm_sort();
         refresh();

      }
   }
}


int mwDemoMode::draw_range_tools(int x1, int y1)
{
   int col = 10;

   int w = 148;
   //w +=  mLoop.pct_x;
   int x2 = x1 + w;



   int h = 50;
   //h +=  mLoop.pct_y;
   int y2 = y1 + h;

   if ((mInput.mouse_x > x1) && (mInput.mouse_x < x2) && (mInput.mouse_y > y1) && (mInput.mouse_y < y2)) proc_range_tools_menu();

   al_draw_filled_rectangle(x1, y1, x2, y2, mColor.pc[0]);
   al_draw_rectangle(x1, y1, x2, y2, mColor.pc[col], 1);
   al_draw_line(x1, y1+12, x2, y1+12, mColor.pc[col], 1);
   al_draw_textf(mFont.pr8, mColor.pc[15], (x1+w/2), y1+2, ALLEGRO_ALIGN_CENTER, "Range Tools");

   y1+=14;

   int xa = x1+4;
   int xb = x2-4;

   al_draw_textf(mFont.pr8, mColor.pc[15], xa, y1, 0, "Start:%d", rt_start_frame);
   if (mWidget.buttont(xb-30, y1, xb, 10,  0,0,0,0,  0,10,15, 0,  1,0,0,0, "Set")) rt_start_frame = current_frame_num;


   y1+=10;
   al_draw_line(x1, y1, x2, y1, mColor.pc[col], 1);

   y1+=2;
   al_draw_textf(mFont.pr8, mColor.pc[15], xa, y1, 0, "End:%d", rt_end_frame);
   if (mWidget.buttont(xb-30, y1, xb, 10,  0,0,0,0,  0,10,15, 0,  1,0,0,0, "Set")) rt_end_frame = current_frame_num;

   y1+=10;
   al_draw_line(x1, y1, x2, y1, mColor.pc[col], 1);

   mWidget.togglec(xa, y1, xa+40, 16,  0,0,0,0,  0,0,0,0, 1,0,1,0, rt_all_players,  "All Players", 15, 15);

   return y1;
}






void mwDemoMode::draw_timeline(void)
{
   al_draw_filled_rectangle(sb_x1, sb_y1, sb_x2, sb_y2, mColor.pc[0]);

   int level_done_section_y = -1;

   for (int i=0; i<20; i++)
      if (player_sections[i][0])
      {
         int p   = player_sections[i][1];
         int col = player_sections[i][2];
         int x1  = player_sections[i][5];
         int y1  = player_sections[i][6];
         int x2  = player_sections[i][7];
         int y2  = player_sections[i][8];

         if (p == level_done_player) level_done_section_y = y1 + (y2-y1)/2;  // level done y position

         al_draw_rectangle(x1, y1, x2, y2, mColor.pc[col], 1);

         if (sb_text) draw_section_details_one_line(i, x1+8, y2+((y1-y2)-8)/2);

         if ((mInput.mouse_x > x1) && (mInput.mouse_x < x2) && (mInput.mouse_y >= y2) && (mInput.mouse_y <= y1))
         {
            if (mInput.mouse_b[1][0]) set_active_section(i);

            al_draw_filled_rectangle(x1-1, y1-1, x2+1, y2+1, mColor.pc[col]);

         }

         if (current_section == i) al_draw_rectangle(x1+1, y2+1, x2-1, y1-1, mColor.pc[col], 1);
      }

   al_draw_rectangle(sb_x1, sb_y1, sb_x2, sb_y2, mColor.pc[15], 1); // timeline frame

   // show level done
   if ((sh_level_done) && (level_done_frame))
   {
      int x = screen_pos_from_frame_num(level_done_frame, last_frame, sb_x1, sb_w);
      int y = level_done_section_y;
      mark_timeline_position(level_done_frame, x, y, sb_y2, 11); // mark level done frame
      al_draw_filled_circle(x, y, 3, mColor.pc[11]);
   }



//
//   // show level done
//   if (sh_level_done)
//   {
//      char msg[80];
//      int ty = sb_y2;
//      if (!level_done_frame)
//      {
////         sprintf(msg, "Not Done");
////         int x = sb_x2 - strlen(msg) * 4;
////         mScreen.draw_framed_text(x, ty, 1, mFont.pr8, 11+64, 11+64, msg);
//      }
//      else
//      {
////         sprintf(msg, "Exit");
////         int x = sb_x2 - strlen(msg) * 4;
////         mScreen.draw_framed_text(x, ty, 1, mFont.pr8, 11+64, 11+64, msg);
//
//         int x = screen_pos_from_frame_num(level_done_frame, last_frame, sb_x1, sb_w);
//         int y = level_done_section_y;
//         mark_timeline_position(level_done_frame, x, y, sb_y2, 11); // mark level done frame
//         al_draw_filled_circle(x, y, 3, mColor.pc[11]);
//      }
//   }


   mark_timeline_position(mLoop.frame_num, screen_pos_from_frame_num(mLoop.frame_num, last_frame, sb_x1, sb_w), sb_y1, sb_y2, 15); // mark current frame

   // mouse on timeline
   if ((mInput.mouse_x > sb_x1-2) && (mInput.mouse_x < sb_x2) && (mInput.mouse_y > sb_y1) && (mInput.mouse_y < sb_y2))
   {
      al_draw_rectangle(sb_x1, sb_y1, sb_x2, sb_y2, mColor.pc[13], 1); // highlight frame
      int mf = ((float) (mInput.mouse_x - sb_x1+1) / (float) sb_w) * (float)last_frame; // get frame num of pointer
      if (mf < 1) mf = 1;
      mark_timeline_position(mf, mInput.mouse_x, sb_y1, sb_y2, 13); // mark mouse position
      if (mInput.mouse_b[1][0])
      {
         while (mInput.mouse_b[1][0]) mEventQueue.proc(1);
         current_frame_num = mf;
         seek_to_frame(mf, 1); // seek to current frame
      }
   }

   if (!player_sections[current_section][0]) current_section = 0;

}










void mwDemoMode::proc_transport_menu(void)
{
   if (mInput.mouse_b[2][0])
   {
      sprintf(mMenu.menu_string[0],"Transport Menu");
      sprintf(mMenu.menu_string[1],"--------------");
      sprintf(mMenu.menu_string[2],"Punch-in Record Armed");
      sprintf(mMenu.menu_string[3],"Punch-in Record Disarmed");
      sprintf(mMenu.menu_string[4],"end");

      int mp = mMenu.pmenu(5, 13, -12, 1);
      if (mp == 2) record_punch_in_armed = 1;
      if (mp == 3) record_punch_in_armed = 0;
   }
}







int mwDemoMode::draw_transport_controls(int x1, int y1)
{
   int w = 192;
   int x2 = x1+w;

   int h = 82;
   //h +=  mLoop.pct_y;
   int y2 = y1+h;

//   if ((mInput.mouse_x > x1) && (mInput.mouse_x < x2) && (mInput.mouse_y > y1) && (mInput.mouse_y < y2)) proc_file_details_menu();

   int c = 15;
   al_draw_filled_rectangle(x1, y1, x2, y2, mColor.pc[0]);
   al_draw_rectangle(x1, y1, x2, y2, mColor.pc[c], 1);
   al_draw_line(x1, y1+12, x2, y1+12, mColor.pc[c], 1);
   al_draw_textf(mFont.pr8, mColor.pc[15], (x1+w/2), y1+2, ALLEGRO_ALIGN_CENTER, "Transport");

   int xa = x1+3;
   int xa2 = x2-3;
   int ya = y1 + 15;

   int f = current_frame_num;

   if (mWidget.buttont(xa,         ya, xa+40, 32, 0,0,0,0, 0,10,15, 0,  1,0,0,0, "Rew")) f = 1;
   if (play)
   {
      if (mWidget.buttont(xa + 44, ya, xa+94, 32, 0,0,0,0, 0,10,15, 0,  1,0,0,0, "Stop")) stop_transport();
   }
   else
   {
      if (mWidget.buttont(xa +44,  ya, xa+94, 32, 0,0,0,0, 0,11,15, 0,  1,0,0,0, "Play")) play = 1;
   }

   if (record)
   {
      if (mWidget.buttont(xa+98,   ya, xa2,   32, 0,0,0,0, 0,10,15, 0,  1,0,0,0, "Record")) stop_transport();
   }
   else
   {
      if (mWidget.buttont(xa+98,   ya, xa2,   32, 0,0,0,0, 0,14,15, 0,  1,0,0,0, "Record")) start_record();
   }

   int xc = (xa+98 + xa2) / 2;
   if (record_punch_in_armed) al_draw_text(mFont.pixl, mColor.pc[10], xc, ya-4, ALLEGRO_ALIGN_CENTER, "punch-in armed");

   al_draw_filled_rectangle(xc-18, ya+23, xc+17, ya+30, mColor.pc[0]);
   al_draw_textf(mFont.pixl, mColor.pc[record_player_color], xc, ya+19, ALLEGRO_ALIGN_CENTER, "Player:%d", record_player_number);

//   al_draw_scaled_bitmap(mBitmap.player_tile[record_player_color][1], 0, 0, 20, 20, xa2-18, ya+10, 10, 10, 0);
//   if ((mInput.mouse_x > xa2-18) && (mInput.mouse_x < xa2) && (mInput.mouse_y > ya+10) && (mInput.mouse_y < ya+20))

   if ((mInput.mouse_x > xc-18) && (mInput.mouse_x < xc+17) && (mInput.mouse_y > ya+23) && (mInput.mouse_y < ya+30))
   {
      int oldrpn = record_player_number;
      proc_edit_gm_player_num_menu(record_player_number);
      if (oldrpn != record_player_number)
      {
         mPlayer.active_local_player = record_player_number;
         record_player_color = mPlayer.syn[record_player_number].color;
      }
   }
   if ((mInput.mouse_x > xa+98) && (mInput.mouse_x < xa2) && (mInput.mouse_y > ya) && (mInput.mouse_y < ya+32)) proc_transport_menu();

   ya+=34;


   c = 9+128;
   mWidget.slideri(xa, ya, xa2, 16,  0,0,0,0,  0,c,15,15,  0,0,1,0, f, last_frame+1000, 0, 1, "Current Frame:");
   ya+=1;

   int bp = 5; // button padding
   int bs = 2; // button spacing

   int x7 = xa;
   int x8 = x7 + bp + 8*3;
   if (mWidget.buttont(x7, ya, x8, 16,  0,0,0,0,  0,c,15, 0,  1,0,0,0, "-80")) f-=80;
   x7 = x8 + bs; x8 = x7 + bp + 8*3;
   if (mWidget.buttont(x7, ya, x8, 16,  0,0,0,0,  0,c,15, 0,  1,0,0,0, "-10")) f-=10;
   x7 = x8 + bs; x8 = x7 + bp + 8*2;
   if (mWidget.buttont(x7, ya, x8, 16,  0,0,0,0,  0,c,15, 0,  1,0,0,0, "-1")) f--;

   x8 = xa2;  x7 = x8 - (bp + 8*3);
   if (mWidget.buttont(x7, ya, x8, 16,  0,0,0,0,  0,c,15, 0,  1,0,0,0, "+80")) f+=80;
   x8 = x7 - bs; x7 = x8 - (bp + 8*3);
   if (mWidget.buttont(x7, ya, x8, 16,  0,0,0,0,  0,c,15, 0,  1,0,0,0, "+10")) f+=10;
   x8 = x7 - bs; x7 = x8 - (bp + 8*2);
   if (mWidget.buttont(x7, ya, x8, 16,  0,0,0,0,  0,c,15, 0,  1,0,0,0, "+1")) f++;

   if (f < 0) f = 0;

   if (f != current_frame_num)
   {
      current_frame_num = f;
      seek_to_frame(f, 1);
   }
   return y2;
}

void mwDemoMode::proc_cpu_time(double frame_start_timestamp)
{
   // --------------------------------------------
   // measure time to process loop
   // --------------------------------------------
   double pt = al_get_time() - frame_start_timestamp;

   // convert to 'cpu', a percent of the timer frame period
   double frame_period = 1 / (double) mLoop.frame_speed;
   float cpu = (pt / frame_period) * 100;

   // store in local cpu variables
   mRollingAverage[0].add_data(cpu);
   mQuickGraph2[9].add_data(0, mRollingAverage[0].mx, 0);
   mQuickGraph2[9].new_entry_pos();

   if (sh_cpu_graph)
   {
      mQuickGraph2[9].set_pos(mDisplay.SCREEN_W-mQuickGraph2[9].width-28, mDisplay.SCREEN_H-mQuickGraph2[9].height-20);
      al_set_target_backbuffer(mDisplay.display);
      mQuickGraph2[9].draw_graph();
   }

}

int mwDemoMode::draw_overlay(void)
{
   if ((sh_overlay_in_play) && (play)) return 0;
   if ((sh_overlay_in_rec) && (record)) return 0;

   // 1st column
   int xa = 14;
   int ya = 14;
   int bts = 16;

   ya = draw_file_details(xa, ya) + 1;

   // draw current section details
   if (sh_section_details) ya = draw_section_details(xa, ya) + 1;

   // draw transport controls
   ya = draw_transport_controls(xa, ya) + 1;

   // draw record controls
   if (sh_rec_controls) ya = draw_record_settings(xa, ya) + 28;

   // draw record controls
   if (sh_range_tools) ya = draw_range_tools(xa, ya) + 28;



   // 2nd column - checkbox settings
   int xa2 = 240;
   int ya2 = 32;
   bts = 10;

   al_draw_filled_rectangle(xa2, ya2, xa2+200, ya2+100, mColor.pc[0]);

//   mWidget.toggle( xa2, ya2, xa2+60, 16,   0,0,0,0,  0,0,0,0, 1,0,1,0, tm_frame, "Time", "Frame", 15, 15, 11, 13);

   mWidget.togglec(xa2, ya2, xa2+40, bts,  0,0,0,0,  0,0,0,0, 1,0,1,0, tm_frame,           "Time/Frame format", 15, 15);

//   ya2+=2;

   mWidget.togglec(xa2, ya2, xa2+40, bts,  0,0,0,0,  0,0,0,0, 1,0,1,0, sh_gamemoves_list,  "Show game moves list", 15, 15);

   mWidget.togglec(xa2, ya2, xa2+40, bts,  0,0,0,0,  0,0,0,0, 1,0,1,0, sh_timeline,        "Show timeline", 15, 15);


   int old_tls = timeline_size;
   mWidget.slideri(xa2, ya2, xa2+180, bts,  0,0,0,0,  0,12,15,15,  0,0,1,0, timeline_size, 5, 1, 1, "Timeline size:");
   if (old_tls != timeline_size) refresh();

//   mWidget.togglec(xa2, ya2, xa2+40, bts,  0,0,0,0,  0,0,0,0, 1,0,1,0, sh_level_done,      "Show level done", 15, 15);

   mWidget.togglec(xa2, ya2, xa2+40, bts,  0,0,0,0,  0,0,0,0, 1,0,1,0, sh_section_details, "Show section details", 15, 15);
   mWidget.togglec(xa2, ya2, xa2+40, bts,  0,0,0,0,  0,0,0,0, 1,0,1,0, sh_rec_controls,    "Show record controls", 15, 15);


   mWidget.togglec(xa2, ya2, xa2+40, bts,  0,0,0,0,  0,0,0,0, 1,0,1,0, sh_cpu_graph,       "Show cpu graph", 15, 15);
   mWidget.togglec(xa2, ya2, xa2+40, bts,  0,0,0,0,  0,0,0,0, 1,0,1,0, sh_player_grid,     "Show player grid", 15, 15);

   mWidget.togglec(xa2, ya2, xa2+40, bts,  0,0,0,0,  0,0,0,0, 1,0,1,0, sh_range_tools,     "Show range tools", 15, 15);




   mWidget.togglec(xa2, ya2, xa2+40, bts,  0,0,0,0,  0,0,0,0, 1,0,1,0, sh_overlay_in_play, "Hide overlay when playing", 15, 15);
   mWidget.togglec(xa2, ya2, xa2+40, bts,  0,0,0,0,  0,0,0,0, 1,0,1,0, sh_overlay_in_rec,  "Hide overlay when recording", 15, 15);



   // debug player grid
   if (sh_player_grid) mScreen.drg_show(mDisplay.SCREEN_W-130, sb_y1-120);

   // game moves list
   if (sh_gamemoves_list) draw_gm_txt_lines(sb_x1, sb_y1-13, 0);

   // timeline
   if (sh_timeline) draw_timeline();

   return 0;

}




void mwDemoMode::demo_record(void)
{

   // for screen overlay common and remote local cpu
   mQuickGraph2[9].initialize(200,    36,    0,   50, "CPU",      9, 12, 13, 1);

   mLoop.frame_num = 0;

   record_player_number = 0;
   record_player_color = 8;
   record = 0;
   play = 0;
   level_done_frame = 0;

   current_section = 0;


   double frame_start_timestamp;

   int quit = 0;

   for (int i=0; i<20; i++)
      for (int j=0; j<20; j++) player_sections[i][j] = 0;

   if (!load_demo_record()) quit = 1;

   while (!quit)
   {
      mEventQueue.proc(1);

      if (mEventQueue.program_update)
      {
         mEventQueue.program_update = 0;
         frame_start_timestamp = al_get_time();

         mDisplay.proc_scale_factor_change();
         if (mLoop.frame_speed < 40) mDisplay.proc_scale_factor_change();
         if (mLoop.frame_speed < 30) mDisplay.proc_scale_factor_change();
         if (mLoop.frame_speed < 20) mDisplay.proc_scale_factor_change();

         if (play)
         {
            mLoop.frame_num++;

            current_frame_num = mLoop.frame_num;

            mBitmap.update_animation();
            mPlayer.proc_player_input();
            mGameMoves.proc();
            mLoop.move_frame();
            mDrawSequence.ds_draw(0, 0);

            if ((!record) && (record_punch_in_armed))
            {
               mPlayer.set_comp_move_from_player_key_check(0);
               if (mPlayer.loc[0].comp_move) start_record();
            }
         }
         else
         {
            al_clear_to_color(al_map_rgb(0, 0, 0));
            mScreen.draw_scaled_level_region_to_display(0);
            mScreen.draw_screen_overlay();
         }

         proc_cpu_time(frame_start_timestamp);

      }

      al_set_target_backbuffer(mDisplay.display);

      quit = draw_overlay();

      al_flip_display();

      if (mInput.key[ALLEGRO_KEY_ESCAPE][0])
      {
         while (mInput.key[ALLEGRO_KEY_ESCAPE][0]) mEventQueue.proc(1);
         if (play) stop_transport();
         else quit = 1;
      }
   }

   mPlayer.active_local_player = 0;
   mConfig.load_config(); // to restore colors and other settings

   mLoop.state[0] = PM_PROGRAM_STATE_MENU;
   mLoop.quit_action = 1; // menu
}



