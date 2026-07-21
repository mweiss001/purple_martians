// mwDemoRecord.cpp
#include "pm.h"
#include "mwDemoRecord.h"
#include "mwDemoMode.h"
#include "mwColor.h"
#include "mwDrawSequence.h"
#include "mwFont.h"
#include "mwGameMoves.h"
#include "mwInput.h"
#include "mwItem.h"
#include "mwLevel.h"
#include "mwLoop.h"
#include "mwMenu.h"
#include "mwPlayer.h"
#include "mwWidget.h"

void mwDemoRecord::draw_transport_controls(mwRect<int> rect, int disable_input, int have_focus)
{
   int c = 13;
   rect.clear_frame_title(mColor.pc[0], mColor.pc[c], mColor.pc[15], 12, ALLEGRO_ALIGN_CENTER, mFont.pr8, "Transport");

   int xa = rect.x1+3;
   int xa2 = rect.x2-3;
   int ya = rect.y1 + 15;
   int d = disable_input;
   int f = current_frame_num;

   if (mWidget.buttont(xa,         ya, xa+40, 32, 0,0,0,0, 0,10,15, 0,  1,0,0,d, "Rew")) f = 1;
   if (play)
   {
      if (mWidget.buttont(xa + 44, ya, xa+94, 32, 0,0,0,0, 0,10,15, 0,  1,0,0,d, "Stop")) stop_transport();
   }
   else
   {
      if (mWidget.buttont(xa +44,  ya, xa+94, 32, 0,0,0,0, 0,11,15, 0,  1,0,0,d, "Play")) play = 1;
   }

   if (record)
   {
      if (mWidget.buttont(xa+98,   ya, xa2,   32, 0,0,0,0, 0,10,15, 0,  1,0,0,d, "Record")) stop_transport();
   }
   else
   {
      if (mWidget.buttont(xa+98,   ya, xa2,   32, 0,0,0,0, 0,14,15, 0,  1,0,0,d, "Record")) start_record();
   }

   int xc = (xa+98 + xa2) / 2;
   if (record_punch_in_armed) al_draw_text(mFont.pixl, mColor.pc[10], xc, ya-4, ALLEGRO_ALIGN_CENTER, "punch-in armed");

   al_draw_filled_rectangle(xc-18, ya+23, xc+17, ya+30, mColor.pc[0]);
   al_draw_textf(mFont.pixl, mColor.pc[record_player_color], xc, ya+19, ALLEGRO_ALIGN_CENTER, "Player:%d", record_player_number);

   if ((mInput.mouse_x > xc-18) && (mInput.mouse_x < xc+17) && (mInput.mouse_y > ya+23) && (mInput.mouse_y < ya+30))
   {
      int oldrpn = record_player_number;
      change_player_num_menu(record_player_number);
      if (oldrpn != record_player_number)
      {
         mPlayer.active_local_player = record_player_number;
         record_player_color = mPlayer.syn[record_player_number].color;
      }
   }
   if ((mInput.mouse_x > xa+98) && (mInput.mouse_x < xa2) && (mInput.mouse_y > ya) && (mInput.mouse_y < ya+32)) proc_transport_menu();

   ya+=34;

   c = 9+128;
   mWidget.slideri(xa, ya, xa2, 16,  0,0,0,0,  0,c,15,15,  0,0,1,d, f, mDemoMode.last_frame+1000, 0, 1, "Current Frame:");
   ya+=1;

   int bp = 5; // button padding
   int bs = 2; // button spacing

   int x7 = xa;
   int x8 = x7 + bp + 8*3;
   if (mWidget.buttont(x7, ya, x8, 16,  0,0,0,0,  0,c,15, 0,  1,0,0,d, "-80")) f-=80;
   x7 = x8 + bs; x8 = x7 + bp + 8*3;
   if (mWidget.buttont(x7, ya, x8, 16,  0,0,0,0,  0,c,15, 0,  1,0,0,d, "-10")) f-=10;
   x7 = x8 + bs; x8 = x7 + bp + 8*2;
   if (mWidget.buttont(x7, ya, x8, 16,  0,0,0,0,  0,c,15, 0,  1,0,0,d, "-1")) f--;

   x8 = xa2;  x7 = x8 - (bp + 8*3);
   if (mWidget.buttont(x7, ya, x8, 16,  0,0,0,0,  0,c,15, 0,  1,0,0,d, "+80")) f+=80;
   x8 = x7 - bs; x7 = x8 - (bp + 8*3);
   if (mWidget.buttont(x7, ya, x8, 16,  0,0,0,0,  0,c,15, 0,  1,0,0,d, "+10")) f+=10;
   x8 = x7 - bs; x7 = x8 - (bp + 8*2);
   if (mWidget.buttont(x7, ya, x8, 16,  0,0,0,0,  0,c,15, 0,  1,0,0,d, "+1")) f++;

   if (f < 0) f = 0;

   if (f != current_frame_num)
   {
      current_frame_num = f;
      seek_to_frame(f, 1);
   }
}

void mwDemoRecord::proc_transport_menu(void)
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


void mwDemoRecord::seek_to_frame(int frame, int draw)
{
   mLevel.load_level(mLevel.play_level, 0, 0);    // load level

   for (int p=0; p<NUM_PLAYERS; p++)
   {
      mPlayer.init_player(p, 1);        // full reset
      mItem.set_player_start_pos(p);  // get starting position for all players, active or not
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



void mwDemoRecord::stop_transport(void)
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


void mwDemoRecord::start_record()
{
   play = 1;
   record = 1;

   int p = record_player_number;

   // erase all game moves for this player that have a frame number equal or higher than current frame number
   for (int x=0; x<mGameMoves.entry_pos; x++)
      if (mGameMoves.arr[x][0] >= mLoop.frame_num)
      {
         int do_clear = 0;
         if (mGameMoves.does_game_move_contain_player(x, p)) do_clear = 1;
         if (mGameMoves.arr[x][1] == PM_GAMEMOVE_TYPE_SHOT_CONFIG) do_clear = 0; // never erase PM_GAMEMOVE_TYPE_SHOT_CONFIG
         if (do_clear) mGameMoves.clear_single(x);
      }

   // check if there still is an active game move after erasing
   if (!mGameMoves.find_first_active_game_move_for_player(p, 0, mGameMoves.entry_pos))
   {
      int spi = mPlayer.syn[p].spawn_point_index; // save spawn_point_index
      mPlayer.init_player(p, 1);                  // full player reset
      mPlayer.syn[p].spawn_point_index = spi;     // restore spawn_point_index
      mItem.set_player_start_pos(p);              // set starting position

      mPlayer.syn[p].active = 1;
      mPlayer.syn[p].color = record_player_color;

      // insert 'active' game move
      int fn = mLoop.frame_num;
      if (fn < 1) fn = 1;

      // this is compatible with the new PM_GAMEMOVE_TYPE_PLAYER_ACTIVE 20260330
      mGameMoves.add_game_move(fn, PM_GAMEMOVE_TYPE_PLAYER_ACTIVE, p, mPlayer.syn[p].color);
   }

   mPlayer.active_local_player = p;
   mPlayer.syn[p].control_method = PM_PLAYER_CONTROL_METHOD_SINGLE_PLAYER;
}

