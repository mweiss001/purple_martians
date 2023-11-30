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

      printf("GDT lev:[%2d] --------------------------------\n", mLevel.play_level);

//      printf("GDT lev:[%2d] -----", mLevel.play_level);
//      for (int x=0; x<mGameMoves.entry_pos; x++)
//         if (mGameMoves.arr[x][1] == PM_GAMEMOVE_TYPE_PLAYER_ACTIVE)
//         {
//            printf("[%d] p:%d active\n", mGameMoves.arr[x][0], mGameMoves.arr[x][2]);
//         }


      find_level_done();

      printf("[%d] level done\n", level_done_frame);


      int ack = 0;
      for (int x=0; x<mGameMoves.entry_pos; x++)
         if (mGameMoves.arr[x][1] == PM_GAMEMOVE_TYPE_LEVEL_DONE_ACK)
         {
            ack = mGameMoves.arr[x][0];
            printf("[%d] p:%d ack\n", mGameMoves.arr[x][0], mGameMoves.arr[x][2]);
         }

      printf("Time between level done and ack  -------  [%d]\n", ack - level_done_frame);


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


float screen_pos_from_frame_num(float frame, float last_frame, float sb_x1, float sb_w)
{
   return sb_x1 + (float)sb_w * ((float)frame / (float)last_frame);
}


void mwDemoMode::seek_to_frame(int frame, int draw)
{
   mLevel.load_level(mLevel.play_level, 0, 0);    // load level

   for (int p=0; p<NUM_PLAYERS; p++)
   {
      mPlayer.init_player(p, 1);           // full reset
      mPlayer.set_player_start_pos(p, 0);  // get starting position for all players, active or not
   }
   mPlayer.syn[0].active = 1;
   mPlayer.syn[0].control_method = PM_PLAYER_CONTROL_METHOD_DEMO_MODE; // to ensure that all added players are this mode also

   mLoop.frame_num = 0;

//   mLoop.ff_state = 1;
   while (mLoop.frame_num < frame)
   {
      mLoop.frame_num++;
      mGameMoves.proc();
      mLoop.move_frame();
   }
//   mLoop.ff_state = 0;

   if (draw) mDrawSequence.draw(0, 0);
}


void draw_gm_txt_line(int x, int y, int i)
{
   int f = mGameMoves.arr[i][0]; // frame
   int t = mGameMoves.arr[i][1]; // type
   int p = mGameMoves.arr[i][2]; // player
   int v = mGameMoves.arr[i][3]; // value

   char msg[256];
   if (t == PM_GAMEMOVE_TYPE_LEVEL_START)     sprintf(msg,"-------------START (level:%d)------------- ", v);
   if (t == PM_GAMEMOVE_TYPE_PLAYER_ACTIVE)   sprintf(msg,"-------------PLAYER %d ACTIVE (color:%d)-- ", p, v);
   if (t == PM_GAMEMOVE_TYPE_PLAYER_INACTIVE) sprintf(msg,"-------------PLAYER %d INACTIVE------------", p);
   if (t == PM_GAMEMOVE_TYPE_MOVE)            sprintf(msg,"%s", mGameMoves.cmtos(mGameMoves.arr[i][3], msg));
   if (t == PM_GAMEMOVE_TYPE_LEVEL_DONE_ACK)  sprintf(msg,"-------------PLAYER %d ACKNOWLEDGE---------", p);

   int col = mPlayer.syn[mGameMoves.arr[i][2]].color;
   if (i == 0) col = 15;

   al_draw_textf(mFont.pr8, mColor.pc[col], x, y, 0, "[%3d][%5d][%d][%d][%2d]%s", i, f, t, p, v, msg);
}

void draw_gm_txt_lines(int x, int y, int num_lines)
{

   // find game move index closest to current frame
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

   for (int i=gm1; i<gm2; i++)
   {
      draw_gm_txt_line(x, ypos, i);
      ypos+=8;
   }
}


void mark_timeline_position(int frame, int x1, int y1, int sb_y2, int col)
{
   char msg[64];
   al_draw_line(x1, y1, x1, sb_y2, mColor.pc[col], 1); y1-=8;
   al_draw_textf(mFont.pr8, mColor.pc[col], x1, y1, ALLEGRO_ALIGN_CENTER, "%d", frame); y1-=8;
   al_draw_textf(mFont.pr8, mColor.pc[col], x1, y1, ALLEGRO_ALIGN_CENTER, "%s", mItem.chrms(frame, msg));
}


void set_player_colors_from_gm(void)
{
   for (int x=0; x<GAME_MOVES_SIZE; x++)
      if (mGameMoves.arr[x][1] == PM_GAMEMOVE_TYPE_PLAYER_ACTIVE) mPlayer.syn[mGameMoves.arr[x][2]].color = mGameMoves.arr[x][3];
}

void mwDemoMode::find_level_done(void)
{
   level_done_frame = 0;

   seek_to_frame(0, 0);
   mLoop.ff_state = 1;
   while (!level_done_frame)
   {
      mLoop.frame_num++;
      mGameMoves.proc();
      mLoop.move_frame();

      if (mPlayer.syn[0].level_done_mode) level_done_frame = mLoop.frame_num;
      if (mLoop.frame_num > mDemoMode.last_frame + 100) level_done_frame = -1;
   }
   mLoop.ff_state = 0;
   seek_to_frame(0, 1);
}



void mwDemoMode::start_record(void)
{
   int p = record_player_number;
   int col = record_player_color;

   play = 1;

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

      mPlayer.init_player(p, 1);           // full reset
      mPlayer.set_player_start_pos(p, 0);  // set starting position

      mPlayer.syn[p].active = 1;

   }
   mPlayer.active_local_player = p;
   mPlayer.syn[p].control_method = PM_PLAYER_CONTROL_METHOD_SINGLE_PLAYER;
}


int mwDemoMode::load_demo_record(void)
{
   if (!mGameMoves.load_gm_file_select()) return 0;
   set_player_colors_from_gm();

   fill_player_sections();

   seek_to_frame(0, 1);
   find_level_done();
   return 1;
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
         mLoop.ff_state = 1;
         while (mLoop.frame_num < f2)
         {
            mLoop.frame_num++;
            mGameMoves.proc();
            mLoop.move_frame();
            if ((player_sections[i][11] == 0) && (mPlayer.syn[0].level_done_mode) && (mPlayer.syn[0].level_done_player == p)) player_sections[i][11] = mLoop.frame_num;
            if ((mPlayer.syn[p].paused == 100) && (mPlayer.syn[p].paused_type == 1)) player_sections[i][10]++;
         }
         mLoop.ff_state = 0;
         //seek_to_frame(0, 1);
      }
}


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
      int col = mPlayer.syn[p].color;
      int f1 = -1;
      int f2 = -1;
      for (int x=0; x<mGameMoves.entry_pos; x++)
         if (mGameMoves.arr[x][2] == p)
         {
            if (mGameMoves.arr[x][1] == PM_GAMEMOVE_TYPE_PLAYER_ACTIVE)
            {
               f1 = mGameMoves.arr[x][0];
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

int mwDemoMode::draw_section_details(int i, int x, int y, int single_line)
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

   int ld = player_sections[i][11]; // level done
   int dth = player_sections[i][10]; // deaths

   if (single_line)
   {
      al_draw_textf(mFont.pr8, mColor.pc[col], x, y, 0, "p%d %d-%d af:%d ld:%d dth:%d %s ", p, f1, f2, af, ld, dth, et);
      y+=8;
   }
   else
   {
      col = 15;
      int line_space = 10;

      al_draw_textf(mFont.pr8, mColor.pc[col], x, y, 0, "Current Section");
      y+=line_space;

      al_draw_textf(mFont.pr8, mColor.pc[col], x, y, 0, "Player:%d", p);
      y+=line_space;

      al_draw_textf(mFont.pr8, mColor.pc[col], x, y, 0, "Start Frame:%d", f1);
      y+=line_space;

      al_draw_textf(mFont.pr8, mColor.pc[col], x, y, 0, "End Frame:%d", f2);
      y+=line_space;

      al_draw_textf(mFont.pr8, mColor.pc[col], x, y, 0, "End Type:%s", et);
      y+=line_space;

      if (ld == 0) al_draw_textf(mFont.pr8, mColor.pc[col], x, y, 0, "Level Done:no");
      else         al_draw_textf(mFont.pr8, mColor.pc[col], x, y, 0, "Level Done:%d", ld);
      y+=line_space;

      if (af == 0) al_draw_textf(mFont.pr8, mColor.pc[col], x, y, 0, "Acknowledge Level Done:no");
      else         al_draw_textf(mFont.pr8, mColor.pc[col], x, y, 0, "Acknowledge Level Done:%d", af);
      y+=line_space;

      al_draw_textf(mFont.pr8, mColor.pc[col], x, y, 0, "Deaths:%d", dth);
      y+=line_space;

   }


   return y;
}



void mwDemoMode::draw_timeline(void)
{
   for (int i=0; i<20; i++)
      if (player_sections[i][0])
      {
         int p   = player_sections[i][1];
         int col = player_sections[i][2];
         int x1  = player_sections[i][5];
         int y1  = player_sections[i][6];
         int x2  = player_sections[i][7];
         int y2  = player_sections[i][8];
         al_draw_rectangle(x1, y1, x2, y2, mColor.pc[col], 1);

         draw_section_details(i, x1+8, y2+((y1-y2)-8)/2, 1);

         if ((mInput.mouse_x > x1) && (mInput.mouse_x < x2) && (mInput.mouse_y > y2) && (mInput.mouse_y < y1) && (mInput.mouse_b[1][0]))
         {
            current_section = i;
            mPlayer.active_local_player = record_player_number = p;
            mPlayer.set_default_player_colors();
            record_player_color = mPlayer.syn[record_player_number].color;
         }
         if (current_section == i) al_draw_rectangle(x1+1, y2+1, x2-1, y1-1, mColor.pc[col], 1);
      }
   al_draw_rectangle(sb_x1, sb_y1, sb_x2, sb_y2, mColor.pc[15], 1); // timeline frame


   mark_timeline_position(mLoop.frame_num, screen_pos_from_frame_num(mLoop.frame_num, last_frame, sb_x1, sb_w), sb_y1-2, sb_y2, 15); // mark current frame
   if (level_done_frame != -1) mark_timeline_position(level_done_frame, screen_pos_from_frame_num(level_done_frame, last_frame, sb_x1, sb_w), sb_y1-2, sb_y2, 14); // mark level done frame

   // mouse on timeline
   if ((mInput.mouse_x > sb_x1) && (mInput.mouse_x < sb_x2) && (mInput.mouse_y > sb_y1) && (mInput.mouse_y < sb_y2))
   {
      al_draw_rectangle(sb_x1, sb_y1, sb_x2, sb_y2, mColor.pc[13], 1); // highlight frame
      int mf = ((float) (mInput.mouse_x - sb_x1) / (float) sb_w) * (float)last_frame; // get frame num of pointer
      mark_timeline_position(mf, mInput.mouse_x, sb_y1-20, sb_y2, 13); // mark mouse position
      if (mInput.mouse_b[1][0])
      {
         while (mInput.mouse_b[1][0]) mEventQueue.proc(1);
         seek_to_frame(mf, 1); // seek to current frame
      }
   }

   if (!player_sections[current_section][0]) current_section = 0;

}



void mwDemoMode::demo_record(void)
{
   char msg[256];

   mLoop.frame_num = 0;

   record_player_number = 0;
   record_player_color = 8;
   play = 0;
   level_done_frame = -1;

   current_section = 0;


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

         mDisplay.proc_scale_factor_change();

         if (play)
         {
            mLoop.frame_num++;
            mBitmap.update_animation();
            mPlayer.proc_player_input();
            mGameMoves.proc();
            mLoop.move_frame();
            mDrawSequence.draw(0, 1);
         }
         else
         {
            al_clear_to_color(al_map_rgb(0, 0, 0));
            mScreen.draw_scaled_level_region_to_display(0);
            mScreen.draw_screen_overlay();
         }

         al_set_target_backbuffer(mDisplay.display);

         int xa = 14;
         int xb = 300;
         int ya = 14;
         int bts = 16;




         sprintf(msg, "File:%s", al_get_path_filename(al_create_path(current_loaded_demo_file)));

//         sprintf(msg, "File:%s", current_loaded_demo_file);

         if (mWidget.buttont(xa, ya, xa+400, bts,  0,0,0,0,  0,10,15, 0,  1,0,1,0, msg))
            if (!load_demo_record()) quit = 1;

         bts = 32;
         if (play) { if (mWidget.buttont(xa,     ya, xa+80,  bts,  0,0,0,0,  0,10,15, 0,  1,0,0,0, "Stop")) play = 0; }
         else      { if (mWidget.buttont(xa,     ya, xa+80,  bts,  0,0,0,0,  0,11,15, 0,  1,0,0,0, "Play")) play = 1; }
                     if (mWidget.buttont(xa+100, ya, xa+180, bts,  0,0,0,0,  0,12,15, 0,  1,0,0,0, "Record")) start_record();

         bts = 16;
         if (mWidget.buttont(xa+200, ya, xa+280, bts,  0,0,0,0,  0,6,15, 0,  1,0,0,0, "sort")) mGameMoves.gm_sort();
         if (mWidget.buttont(xa+300, ya, xa+380, bts,  0,0,0,0,  0,6,15, 0,  1,0,1,0, "save as new file")) mGameMoves.autosave_gm(5);


         if (mWidget.buttont(xa+300, ya, xa+380, bts,  0,0,0,0,  0,6,15, 0,  1,0,1,0, "overwrite")) mGameMoves.save_gm(current_loaded_demo_file);


         // this is a group to select a player number and color
         ya+= 20;
         xb = xa+140;
         bts = 20;


         int oldrpn = record_player_number;
         mWidget.slideri(xa, ya, xb, bts,  0,0,0,0,  0,12,15,15,  0,0,1,0, record_player_number, 7, 0, 1, "Player Number:");
         int cl = mWidget.colsel(xa, ya, xb, bts,  10,0,0,0,  0,12,15,15,  0,0,1,0);
         if (cl != -1) record_player_color = cl;

         if ((mWidget.buttont(xa, ya, xb, bts,  0,0,0,0,  0,10,15, 0,  1,0,1,0, "Default Colors")) || (oldrpn != record_player_number))
         {
            mPlayer.active_local_player = record_player_number;
            mPlayer.set_default_player_colors();
            record_player_color = mPlayer.syn[record_player_number].color;
         }

         al_draw_bitmap(mBitmap.player_tile[record_player_color][1], xa, ya, 0 );
         al_draw_text(mFont.pr8, mColor.pc[record_player_color], xa+22, ya+7, 0, mColor.color_name[record_player_color]);

         ya += 24;

         if (mWidget.buttont(xa, ya, xb, bts,  0,0,0,0,  0,10,15, 0,  1,0,1,0, "Replace"))
         {
            for (int x=0; x<GAME_MOVES_SIZE; x++)
               if ((mGameMoves.arr[x][2] == record_player_number) && (mGameMoves.arr[x][1] == PM_GAMEMOVE_TYPE_PLAYER_ACTIVE)) mGameMoves.arr[x][3] = record_player_color;
            set_player_colors_from_gm();
         }

         bts = 16;
         sprintf(msg, "Level Done Frame:%d", level_done_frame);
         if (mWidget.buttont(xa, ya, xa+200, bts,  0,0,0,0,  0,10,15, 0,  1,0,1,0, msg)) find_level_done();

         //mScreen.drg_show(mDisplay.SCREEN_W-130, sb_y1-80); // demo record debug grid
//         draw_gm_txt_lines(sb_x1, sb_y1-24, 20); // show game moves

//         // draw section details of all active section
//         for (int i=0; i<20; i++)
//            if (player_sections[i][0]) ya = draw_section_details(i, xa, ya, 1);


         // draw current section details
         ya+=4;
         ya = draw_section_details(current_section, xa, ya, 0);



         if (mWidget.buttont(xa, ya, xb, bts,  0,0,0,0,  0,10,15, 0,  1,0,1,0, "Refresh"))
         {
            fill_player_sections(); // this is expensive...do it only when needed
            get_more_player_section_info();
            find_ack_moves(0);
         }


         draw_timeline();



         al_flip_display();
      }
      if (mInput.key[ALLEGRO_KEY_ESCAPE][0]) quit = 1;
   }
   mLoop.state[0] = PM_PROGRAM_STATE_MENU;
   mLoop.quit_action = 1; // menu
   mPlayer.set_default_player_colors();
}
