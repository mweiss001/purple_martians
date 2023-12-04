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
      printf("pvs:%d pvs:%d dmg:%d\n", mPlayer.syn[0].player_vs_player_shots, mPlayer.syn[0].player_vs_self_shots, mPlayer.syn[0].player_vs_player_shot_damage);


//      mGameMoves.save_gm(current_loaded_demo_file);



   }
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
      seek_to_frame(last_frame, 0);
      mScreen.draw_percent_bar(x, y, 200, 20, (i+1)*100 / num_demo_filenames);
      al_flip_display();
   }
   demo_debug_running_demo_saves_level_data = old_demo_debug_running_demo_saves_level_data; // restore initial state
}

int mwDemoMode::load_random_demo(void)
{
   if (!load_demo_file_array()) return 0;

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

void mwDemoMode::key_check(int p)
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

   if ((mInput.key[ALLEGRO_KEY_N][0]) && (mode == 2))
   {
       mLoop.state[0] = PM_PROGRAM_STATE_NEXT_LEVEL; // skip to next random level
       key_used = 1;
   }

   if (mLoop.frame_num > last_frame + 160) mLoop.state[0] = PM_PROGRAM_STATE_NEXT_LEVEL; // if games_moves doesn't end with level_done kill it after 4 seconds

   if (!key_used) // if key not used
   {
      int key_cancel = 0;

      // any key
      //for (int k = 0; k < ALLEGRO_KEY_MAX; k++) if (mInput.key[k][0]) key_cancel = 1;

      // only these keys
      if ((mInput.key[ALLEGRO_KEY_ESCAPE][0]) || (mInput.key[ALLEGRO_KEY_ENTER][0]) || (mInput.key[ALLEGRO_KEY_SPACE][0])) key_cancel = 1;

      if (key_cancel)
      {
         mInput.key[ALLEGRO_KEY_ESCAPE][0] = 0; // eat this keypress and pretend it neve happened!
         mLoop.state[0] = PM_PROGRAM_STATE_DEMO_QUIT_WITH_KEYPRESS; // demo level cleanup and exit
      }
   }
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

   mLoop.frame_num = 0;
   while (mLoop.frame_num < frame)
   {
      mLoop.frame_num++;
      mGameMoves.proc();
      mLoop.move_frame();
   }
   if (draw) mDrawSequence.ds_draw(0, 0);
}




void mwDemoMode::draw_gm_txt_lines_one(int x, int y)
{
   int num_lines = gm_list_lines;

   // ideally the current position should be in the middle of the list

   // find game move index closest to current frame
   int i_closest = 0;
   int dist = 999999;

   for (int i=0; i<tmp_entry_pos; i++)
   {
      int td = abs(tmp_arr[i][0] - mLoop.frame_num);
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
   if (gm2 > tmp_entry_pos) gm2 = tmp_entry_pos;

   // recalc num of lines (in case we are past the end)
   num_lines = gm2 - gm1;

   int ypos = y-num_lines*8;

   char msg[256];

   for (int i=gm1; i<gm2; i++)
   {
      int col = player_sections[current_section][2];

      if (tmp_arr[i][1] == PM_GAMEMOVE_TYPE_LEVEL_START) col = 15;
      if (tmp_arr[i][1] == PM_GAMEMOVE_TYPE_SHOT_CONFIG) col = 15;


      al_draw_filled_rectangle(x, ypos, x+336, ypos+8, mColor.pc[0]);

      al_draw_text(mFont.pr8, mColor.pc[col], x, ypos, 0, mGameMoves.get_gm_text2(i, tmp_arr[i][0], tmp_arr[i][1], tmp_arr[i][2], tmp_arr[i][3], msg));

      ypos+=8;
   }
}



void mwDemoMode::draw_gm_txt_lines(int x, int y)
{
   if (gm_list_all) draw_gm_txt_lines_all(x, y);
   else             draw_gm_txt_lines_one(x, y);
}


void mwDemoMode::draw_gm_txt_lines_all(int x, int y)
{
   int num_lines = gm_list_lines;

   // find game move index closest to current frame, but not less than
   int gmi = 0;
   for (int i=0; i<mGameMoves.entry_pos; i++)
      if (mGameMoves.arr[i][0] >= mLoop.frame_num)
      {
         gmi = i;
         break;
      }

   // go back half of num_lines (but don't go back past zero)
   int gm1 = gmi - num_lines/2;
   if (gm1 < 0) gm1 = 0;

   // set end of range
   int gm2 = gm1 + num_lines;
   if (gm2 > mGameMoves.entry_pos) gm2 = mGameMoves.entry_pos;

   // recalc num of lines (in case we are past the end)
   num_lines = gm2 - gm1;

   int ypos = y-num_lines*8;

   char msg[256];

   for (int i=gm1; i<gm2; i++)
   {
      int col = mPlayer.syn[mGameMoves.arr[i][2]].color;

      if (mGameMoves.arr[i][1] == PM_GAMEMOVE_TYPE_LEVEL_START) col = 15;
      if (mGameMoves.arr[i][1] == PM_GAMEMOVE_TYPE_SHOT_CONFIG) col = 15;

      al_draw_filled_rectangle(x, ypos, x+336, ypos+8, mColor.pc[0]);
      al_draw_text(mFont.pr8, mColor.pc[col], x, ypos, 0, mGameMoves.get_gm_text(i, msg));
      ypos+=8;
   }
}







float screen_pos_from_frame_num(float frame, float last_frame, float sb_x1, float sb_w)
{
   return sb_x1 + (float)sb_w * ((float)frame / (float)last_frame);
}

void mwDemoMode::mark_timeline_position(int frame, int x1, int y1, int sb_y2, int col)
{
   char msg[64];
   sprintf(msg, "%s", gettf(frame, msg));
   int len = strlen(msg);
   int xtw = 2 + len*4;
   al_draw_filled_rectangle(x1-xtw, y1-10, x1+xtw, y1+1, mColor.pc[0]);
   al_draw_rounded_rectangle(x1-xtw, y1-10, x1+xtw, y1+1, 1, 1, mColor.pc[col], 1);
   al_draw_line(x1, y1+1, x1, sb_y2, mColor.pc[col], 1);
   al_draw_text(mFont.pr8, mColor.pc[col], x1, y1-8, ALLEGRO_ALIGN_CENTER, msg);
}


void mwDemoMode::find_level_done(void)
{
   level_done_frame = 0;
   level_done_player = -1;

   seek_to_frame(0, 0);

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
}



void mwDemoMode::start_record(void)
{
   int p = record_player_number;
   int col = record_player_color;

   play = 1;
   record = 1;

   // erase all game moves for this player that have a higher frame number than current
   for (int x=0; x<GAME_MOVES_SIZE; x++)
      if ((mGameMoves.arr[x][2] == p) && (mGameMoves.arr[x][0] >= mLoop.frame_num)) mGameMoves.clear_single(x);

   if (!mPlayer.syn[p].active) // if player is not currently active
   {
      mPlayer.syn[p].color = col;

      // insert 'active' game move
      int fn = mLoop.frame_num;
      if (fn == 0) fn = 1;
      mGameMoves.add_game_move(fn, PM_GAMEMOVE_TYPE_PLAYER_ACTIVE, p, mPlayer.syn[p].color);


      int spi = mPlayer.syn[p].spawn_point_index;

      mPlayer.init_player(p, 1);        // full reset

      mPlayer.syn[p].spawn_point_index = spi;

      mPlayer.set_player_start_pos(p);  // set starting position

      mPlayer.syn[p].active = 1;

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
   sb_x1 = 20;
   sb_x2 = mDisplay.SCREEN_W - 20;
   sb_w = sb_x2 - sb_x1;
   sb_y2 = mDisplay.SCREEN_H - 14;

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



//   al_draw_textf(mFont.pr8, mColor.pc[15], (x1+w/2), y1+2, ALLEGRO_ALIGN_CENTER, "Armed:%d", record_punch_in_armed);


   return y1;
}


void mwDemoMode::draw_section_details_one_line(int i, int x, int y)
{
   int p   = player_sections[i][1];
   int col = player_sections[i][2];
   int f1  = player_sections[i][3];
   int f2  = player_sections[i][4];
   int af  = player_sections[i][12];

   char et[20] = {0};
   int end_type = player_sections[i][9];
   if (end_type == 0) sprintf(et, "end of file");
   if (end_type == 1) sprintf(et, "inactive");

   char hd[20] = {0};
   if (player_sections[i][13]) sprintf(hd, "[hidden]");

   int ld = player_sections[i][11]; // level done
   int dth = player_sections[i][10]; // deaths

   al_draw_textf(mFont.pr8, mColor.pc[col], x, y, 0, "p%d %d-%d af:%d ld:%d dth:%d %s %s", p, f1, f2, af, ld, dth, et, hd);
}



char * mwDemoMode::gettf(int frame, char* ft)
{
   if (tm_frame) sprintf(ft, "%d", frame);
   else sprintf(ft, "%s", mItem.chrms(frame, ft));
   return ft;
}




int mwDemoMode::draw_section_details(int i, int x1, int y1)
{
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

   int h = 200;
   //h +=  mLoop.pct_y;
   int y2 = y1 + h;

   al_draw_filled_rectangle(x1, y1, x2, y2, mColor.pc[0]);
   al_draw_rectangle(x1, y1, x2, y2, mColor.pc[col], 1);
   al_draw_line(x1, y1+12, x2, y1+12, mColor.pc[col], 1);
   al_draw_textf(mFont.pr8, mColor.pc[15], (x1+w/2), y1+2, ALLEGRO_ALIGN_CENTER, "Current Section");
   y1+=14;


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

   al_draw_textf(mFont.pr8, mColor.pc[15], x, y, 0, "Number of Game Moves:%d", player_sections[i][14]);
   y+=line_space;

   al_draw_textf(mFont.pr8, mColor.pc[15], x, y, 0, "Frame of Last Move:%d", player_sections[i][15]);
   y+=line_space;

   y+=2;

   al_draw_line(x1, y, x2, y, mColor.pc[col], 1);

   y+=2;

   if (mWidget.buttont(x1+4, y, x2-4, 16,  0,0,0,0,  0,10,15, 0,  1,0,1,0, "Del Inactive Game Move"))
   {
      for (int x=0; x<mGameMoves.entry_pos; x++)
         if ((mGameMoves.arr[x][1] == PM_GAMEMOVE_TYPE_PLAYER_INACTIVE) && (mGameMoves.arr[x][2] == p))
            if ((mGameMoves.arr[x][0] >= f1) && (mGameMoves.arr[x][0] <= f2)) mGameMoves.gm_remove(x);
      refresh();
   }
   if (mWidget.buttont(x1+4, y, x2-4, 16,  0,0,0,0,  0,10,15, 0,  1,0,1,0, "Add Inactive Game Move"))
   {
      for (int x=0; x<mGameMoves.entry_pos; x++)
         if ((mGameMoves.arr[x][2] == p) && (mGameMoves.arr[x][0] >= current_frame_num)) mGameMoves.clear_single(x);

      mGameMoves.add_game_move2(current_frame_num, PM_GAMEMOVE_TYPE_PLAYER_INACTIVE, p, 0);
      mGameMoves.gm_sort();
      refresh();
   }

   if (mWidget.buttont(x1+4, y, x2-4, 16,  0,0,0,0,  0,10,15, 0,  1,0,1,0, "Delete Section"))
   {
      if (p != 0) // dont delete player 0
      {
         for (int x=0; x<mGameMoves.entry_pos; x++)
            if (mGameMoves.arr[x][2] == p) mGameMoves.clear_single(x);
         mGameMoves.gm_sort();
         refresh();
      }
   }

   if (mWidget.buttont(x1+4, y, x2-4, 16,  0,0,0,0,  0,10,15, 0,  1,0,1,0, "Delete Past Level Done"))
   {
      for (int x=0; x<mGameMoves.entry_pos; x++)
         if (mGameMoves.arr[x][0] > level_done_frame) mGameMoves.clear_single(x);

      mGameMoves.gm_sort();
      refresh();
   }


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


   return y;
}




void mwDemoMode::set_active_section(int i)
{
   current_section = i;

   int p = player_sections[i][1];

   mPlayer.active_local_player = record_player_number = p;
   record_player_color = player_sections[i][2];


   // load all games move for this player into temp array
   tmp_entry_pos = 0;
   for (int i=0; i<mGameMoves.entry_pos; i++)
   {
      int add = 0;
      if (mGameMoves.arr[i][1] == PM_GAMEMOVE_TYPE_SHOT_CONFIG) add = 1;
      if (mGameMoves.arr[i][1] == PM_GAMEMOVE_TYPE_LEVEL_START) add = 1;
      if (mGameMoves.arr[i][2] == p) add = 1;

      if (add)
      {
         for (int j=0; j<4; j++)
            tmp_arr[tmp_entry_pos][j] = mGameMoves.arr[i][j];

         tmp_entry_pos++;

         if (tmp_entry_pos > 9999)
         {
            printf("tmp_arr ran out of room\n");
            return;
         }
      }
   }
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

         if ((mInput.mouse_x > x1) && (mInput.mouse_x < x2) && (mInput.mouse_y > y2) && (mInput.mouse_y < y1) && (mInput.mouse_b[1][0])) set_active_section(i);

         if (current_section == i) al_draw_rectangle(x1+1, y2+1, x2-1, y1-1, mColor.pc[col], 1);
      }
   al_draw_rectangle(sb_x1, sb_y1, sb_x2, sb_y2, mColor.pc[15], 1); // timeline frame

   mark_timeline_position(mLoop.frame_num, screen_pos_from_frame_num(mLoop.frame_num, last_frame, sb_x1, sb_w), sb_y1-3, sb_y2, 15); // mark current frame

   // mouse on timeline
   if ((mInput.mouse_x > sb_x1-2) && (mInput.mouse_x < sb_x2) && (mInput.mouse_y > sb_y1) && (mInput.mouse_y < sb_y2))
   {
      al_draw_rectangle(sb_x1, sb_y1, sb_x2, sb_y2, mColor.pc[13], 1); // highlight frame
      int mf = ((float) (mInput.mouse_x - sb_x1+1) / (float) sb_w) * (float)last_frame; // get frame num of pointer
      if (mf < 1) mf = 1;
      mark_timeline_position(mf, mInput.mouse_x, sb_y1-3, sb_y2, 13); // mark mouse position
      if (mInput.mouse_b[1][0])
      {
         while (mInput.mouse_b[1][0]) mEventQueue.proc(1);
         current_frame_num = mf;
         seek_to_frame(mf, 1); // seek to current frame
      }
   }

   if (!player_sections[current_section][0]) current_section = 0;


   // show level done
   if (sh_level_done)
   {
      char msg[80];
      if (!level_done_frame)  sprintf(msg, "Level Not Done");
      else
      {
         int x = screen_pos_from_frame_num(level_done_frame, last_frame, sb_x1, sb_w);
         int y = level_done_section_y;

         mark_timeline_position(level_done_frame, x, sb_y1-3, y, 11); // mark level done frame
         al_draw_filled_circle(x, y, 3, mColor.pc[11]);
         sprintf(msg, "Level Done");
      }
      int w = 12 + strlen(msg) * 8;
      int x = mDisplay.SCREEN_W - w - 14;
      int y = sb_y1-28;
      mWidget.buttont(x, y, x+w, 14,  0,0,0,0,  0,11+64,15, 0,  1,0,1,0, msg);
   }
}


void mwDemoMode::show_play_record_buttons(int x, int y)
{
   if (play)
   {
      if (mWidget.buttont(x,     y, x+60,  32,  0,0,0,0,  0,10,15, 0,  1,0,0,0, "Stop"))
      {
         play = 0;
         record = 0;
      }
   }
   else
   {
      if (mWidget.buttont(x,     y, x+60,  32,  0,0,0,0,  0,11,15, 0,  1,0,0,0, "Play"))
      {
         play = 1;
      }
   }

   if (record) mWidget.buttont(x+70, y, x+160, 32,  0,0,0,0,  0,10,15, 0,  1,0,0,0, "Recording");
   else if (   mWidget.buttont(x+70, y, x+160, 32,  0,0,0,0,  0,14,15, 0,  1,0,0,0, "Record")) start_record();

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

void mwDemoMode::demo_record(void)
{

   // for screen overlay common and remote local cpu
   mQuickGraph2[9].initialize(200,    36,    0,   50, "CPU",      9, 12, 13, 1);


   char msg[256];

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
         al_set_target_backbuffer(mDisplay.display);



         if (!record)
         {


         // 1st column
         int xa = 14;
         int ya = 14;
         int bts = 16;

         // show currently loaded file and prompt for load if clicked
         sprintf(msg, "File:%s", al_get_path_filename(al_create_path(current_loaded_demo_file))); // name only
         //sprintf(msg, "File:%s", current_loaded_demo_file); // full path
         if ((mWidget.buttont(xa, ya, xa+400, bts,  0,0,0,0,  0,10,15, 0,  1,0,0,0, msg)) && (!load_demo_record())) quit = 1;

         show_play_record_buttons(xa, 32);

         // draw record controls
         if (sh_rec_controls) ya = draw_record_settings(xa, 72);

         // draw current section details
         if (sh_section_details) draw_section_details(current_section, xa, ya+20);


//         if (mWidget.buttont(xa+200, ya, xa+280, bts,  0,0,0,0,  0,6,15, 0,  1,0,0,0, "sort")) mGameMoves.gm_sort();
//         if (mWidget.buttont(xa+300, ya, xa+380, bts,  0,0,0,0,  0,6,15, 0,  1,0,1,0, "insert")) mGameMoves.add_game_move2(4, 21, 22, 23);
//         if (mWidget.buttont(xa+300, ya, xa+380, bts,  0,0,0,0,  0,6,15, 0,  1,0,1,0, "remove")) mGameMoves.gm_remove(2);






         // 2nd column - save and refresh buttons
         int xa1 = 232;
         int ya1 = 32;
         int c1 = 4;
         if (mWidget.buttont(xa1, ya1, xa1+160, bts,  0,0,0,0,  0,c1,15, 0,  1,0,1,0, "save (overwrite)"))
         {
            mGameMoves.save_gm(current_loaded_demo_file);
            mGameMoves.load_gm(current_loaded_demo_file);
            refresh();
         }
         if (mWidget.buttont(xa1, ya1, xa1+160, bts,  0,0,0,0,  0,c1,15, 0,  1,0,1,0, "save (new copy)"))
         {
            mGameMoves.save_gm_make_fn("new copy");
            mGameMoves.load_gm(current_loaded_demo_file);
            refresh();
         }
         if (mWidget.buttont(xa1, ya1, xa1+160, bts,  0,0,0,0,  0,c1,15, 0,  1,0,1,0, "save (name prompt)"))
         {
            mGameMoves.save_gm_file_select();
            mGameMoves.load_gm(current_loaded_demo_file);
            refresh();
         }

         ya1+=4;
         if (mWidget.buttont(xa1+40, ya1, xa1+120, bts,  0,0,0,0,  0,10,15,0,  1,0,1,0, "Refresh")) refresh();

         ya1+=4;
         if (mWidget.buttont(xa1+40, ya1, xa1+120, bts,  0,0,0,0,  0,10,15,0,  1,0,1,0, "Reload")) reload();



         // 3rd column - checkbox settings
//         int xa2 = 420;
//         int ya2 = 32;


         int xa2 = 240;
         int ya2 = 132;

         bts = 10;


//         al_draw_filled_rectangle(xa2, ya2, xa2+200+mLoop.pct_x, ya2+200+mLoop.pct_y, mColor.pc[10+128]);
//         al_draw_filled_rectangle(xa2, ya2, xa2+200+mLoop.pct_x, ya2+200+mLoop.pct_y, mColor.pc[0]);

         al_draw_filled_rectangle(xa2, ya2, xa2+200, ya2+120, mColor.pc[0]);


         mWidget.toggle( xa2, ya2, xa2+60, 16,   0,0,0,0,  0,0,0,0, 1,0,1,0, tm_frame, "Time", "Frame", 15, 15, 11, 13);
         mWidget.togglec(xa2, ya2, xa2+40, bts,  0,0,0,0,  0,0,0,0, 1,0,1,0, sh_section_details, "Show section details", 15, 15);
         mWidget.togglec(xa2, ya2, xa2+40, bts,  0,0,0,0,  0,0,0,0, 1,0,1,0, sh_level_done,      "Show level done", 15, 15);
         mWidget.togglec(xa2, ya2, xa2+40, bts,  0,0,0,0,  0,0,0,0, 1,0,1,0, sh_gamemoves_list,  "Show game moves list", 15, 15);
         mWidget.togglec(xa2, ya2, xa2+40, bts,  0,0,0,0,  0,0,0,0, 1,0,1,0, gm_list_all,        "Show game moves from all players", 15, 15);
         mWidget.togglec(xa2, ya2, xa2+40, bts,  0,0,0,0,  0,0,0,0, 1,0,1,0, sh_cpu_graph,       "Show cpu graph", 15, 15);
         mWidget.togglec(xa2, ya2, xa2+40, bts,  0,0,0,0,  0,0,0,0, 1,0,1,0, sh_player_grid,     "Show player grid", 15, 15);
         mWidget.togglec(xa2, ya2, xa2+40, bts,  0,0,0,0,  0,0,0,0, 1,0,1,0, sh_rec_controls,    "Show record controls", 15, 15);
         mWidget.togglec(xa2, ya2, xa2+40, bts,  0,0,0,0,  0,0,0,0, 1,0,1,0, sh_timeline,        "Show timeline", 15, 15);

         mWidget.slideri(xa2, ya2, xa2+240, bts,  0,0,0,0,  0,12,15,15,  0,0,1,0, gm_list_lines, 100, 10, 1, "gm lines:");








         // debug player grid
         if (sh_player_grid) mScreen.drg_show(mDisplay.SCREEN_W-130, sb_y1-120);

         // game moves list
         if (sh_gamemoves_list) draw_gm_txt_lines(sb_x1, sb_y1-24);

         // timeline
         if (sh_timeline) draw_timeline();


         }

         proc_cpu_time(frame_start_timestamp);

         al_flip_display();

         if (mInput.key[ALLEGRO_KEY_ESCAPE][0])
         {
            while (mInput.key[ALLEGRO_KEY_ESCAPE][0]) mEventQueue.proc(1);
            if (play)
            {
               play = 0;
               record = 0;
               current_frame_num = mLoop.frame_num;
            }
            else quit = 1;
         }
      }


   }
   mLoop.state[0] = PM_PROGRAM_STATE_MENU;
   mLoop.quit_action = 1; // menu
}

