// mwDemoRecord.cpp
#include "pm.h"
#include "mwDemoRecord.h"
#include "mwDemoMode.h"
#include "mwColor.h"
#include "mwDrawSequence.h"
#include "mwFont.h"
#include "mwGameMoves.h"
#include "mwGmInfo.h"
#include "mwInput.h"
#include "mwItem.h"
#include "mwLevel.h"
#include "mwLoop.h"
#include "mwMenu.h"
#include "mwPlayer.h"
#include "mwScreen.h"
#include "mwWidget.h"

void mwDemoRecord::draw_transport_controls(mwWindow w)
{
   int xa = w.rect.x1+2;
   int xb = w.rect.x2-2;
   int ya = w.rect.y1 + 14;
   //int c = w.color;
   int d = w.disable_input;

   int bts = 20;
   draw_transport_controls_seek(xa, ya, bts, d);
   ya+=bts+2;



   bts = 32;
   int x7 = xa+120;
   int x8 = xa+280;
   if (play)
   {
      if (mWidget.buttont(xa,ya, x7-1, bts, 0,0,0,0, 0,10,15, 0,  1,0,0,d, "Stop")) stop_transport();
   }
   else
   {
      if (mWidget.buttont(xa,ya, x7-1, bts, 0,0,0,0, 0,11,15, 0,  1,0,0,d, "Play")) play = 1;
   }
   if (record_punch_in_armed)
   {
      if (mWidget.buttont(x7+1,ya, x8-1, bts, 0,0,0,0, 0,10,15, 0,  1,0,0,d, "Punch In Armed!")) record_punch_in_armed = 0;
   }
   else
   {
      if (mWidget.buttont(x7+1,ya, x8-1, bts, 0,0,0,0, 0,9,15, 0,  1,0,0,d, "Punch In Disarmed")) record_punch_in_armed = 1;
   }
   if (record)
   {
      if (mWidget.buttont(x8+1,ya, xb,   bts, 0,0,0,0, 0,10,15, 0,  1,0,0,d, "Record")) stop_transport();
   }
   else
   {
      if (mWidget.buttont(x8+1,ya, xb,   bts, 0,0,0,0, 0,14,15, 0,  1,0,0,d, "Record")) start_record();
   }

   int xc = (x8 + xb) / 2;
   if (record_punch_in_armed) al_draw_text(mFont.pixl, mColor.pc[10], xc, ya-4, ALLEGRO_ALIGN_CENTER, "punch-in armed");

   int plr = mPlayer.active_local_player;
   int clr = mPlayer.syn[plr].color;

   al_draw_filled_rectangle(xc-18, ya+23, xc+17, ya+30, mColor.pc[0]);
   al_draw_textf(mFont.pixl, mColor.pc[clr], xc, ya+19, ALLEGRO_ALIGN_CENTER, "Player:%d", plr);

   if ((mInput.mouse_x > xc-18) && (mInput.mouse_x < xc+17) && (mInput.mouse_y > ya+23) && (mInput.mouse_y < ya+30)) change_player_num_menu(mPlayer.active_local_player);

   ya+=bts+1;

   bts = 14;
   if (mWidget.mTrackInt(0, xa, xb, 1, ya, bts,   0,0,1,1,    0, 10, 15,  15, 0, 0,    1, mLoop.frame_num, mDemoMode.last_frame+1000, 0,  "Current Frame:", d)) seek_to_frame(mLoop.frame_num, 1);
   ya+=bts+3;


   bts = 20;
   int sw, sh;
   mScreen.draw_demo_controls_overlay_speed(w.rect.x1+15, ya, bts, 2, 0, 0, sw, sh);

}




// button helper
bool bh(int &rx, int y1, int y2, int ls, int bc, int d, const char* txt)
{
   int w = (strlen(txt)+1)*8;
   int x1 = rx;
   int x2 = rx + w;
   rx += w + ls;
   if (mWidget.mButton(0, x1, x2,  0,y1,y2,  0,  1,3,1,  bc+192,bc+80,15, 15,0, txt, d)) return true;
   return false;
}



void mwDemoRecord::draw_transport_controls_seek(int fx1, int fy1, int h, int d)
{
   int pad = 1;
   int w = 377+(pad*2);

   int fx2 = fx1+w;
   int fy2 = fy1+h;

   // clear and draw frame
   al_draw_filled_rectangle(fx1, fy1, fx2, fy2, mColor.Black);
   al_draw_rectangle(       fx1, fy1, fx2, fy2, mColor.White, 1);


   // common y1 and y2 for all buttons
   int y1 = fy1 + pad;
   int y2 = fy2 - pad;

   int bc = 12; // button color

   // get current and last frame
   int f = mLoop.frame_num;
   int lf = mGmInfo.lastFrame;

   // get initial f to determine if this function modifies it
   int initial_f = f;

   // running x position
   int rx = fx1+pad;

   int s = 1;

   if (bh(rx, y1, y2, s, bc, d, "start" )) f  = 0;
   if (bh(rx, y1, y2, s, bc, d, "-60s"  )) f -= 2400;
   if (bh(rx, y1, y2, s, bc, d, "-10s"  )) f -= 400;
   if (bh(rx, y1, y2, s, bc, d, "-1s"   )) f -= 40;
   if (bh(rx, y1, y2, s, bc, d, "-1f"   )) f -= 1;


   if (bh(rx, y1, y2, s, bc, d, "+1f"  )) f += 1;
   if (bh(rx, y1, y2, s, bc, d, "+1s"  )) f += 40;
   if (bh(rx, y1, y2, s, bc, d, "+10s" )) f += 400;
   if (bh(rx, y1, y2, s, bc, d, "+60s" )) f += 2400;
   if (bh(rx, y1, y2, s, bc, d, "end"  )) f  = lf;

   // changed by this function
   if (f != initial_f)
   {
      // enforce limits
      if (f < 0) f = 0;
      if (f > lf) f = lf;

      mLoop.frame_num = f;
      seek_to_frame(f, 1);
   }
}


void mwDemoRecord::seek_to_frame(int frame, int draw)
{
   mLevel.load_level(mLevel.play_level, 0, 0);    // load level

   for (int p=0; p<NUM_PLAYERS; p++)
   {
      mPlayer.init_player(p, 1);      // full reset
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

   int plr = mPlayer.active_local_player;

   // erase all game moves for this player that have a frame number equal or higher than current frame number
   for (int x=0; x<mGameMoves.entry_pos; x++)
      if (mGameMoves.arr[x][0] >= mLoop.frame_num)
      {
         int do_clear = 0;
         if (mGameMoves.does_game_move_contain_player(x, plr)) do_clear = 1;
         if (mGameMoves.arr[x][1] == PM_GAMEMOVE_TYPE_SHOT_CONFIG) do_clear = 0; // never erase PM_GAMEMOVE_TYPE_SHOT_CONFIG
         if (do_clear) mGameMoves.clear_single(x);
      }

   // check if there still is an active game move after erasing
   if (!mGameMoves.find_first_active_game_move_for_player(plr, 0, mGameMoves.entry_pos))
   {
      int spi = mPlayer.syn[plr].spawn_point_index; // save spawn_point_index
      mPlayer.init_player(plr, 1);                  // full player reset
      mPlayer.syn[plr].spawn_point_index = spi;     // restore spawn_point_index
      mItem.set_player_start_pos(plr);              // set starting position

      mPlayer.syn[plr].active = 1;

      // insert 'active' game move
      int fn = mLoop.frame_num;
      if (fn < 1) fn = 1;

      // this is compatible with the new PM_GAMEMOVE_TYPE_PLAYER_ACTIVE 20260330
      mGameMoves.add_game_move(fn, PM_GAMEMOVE_TYPE_PLAYER_ACTIVE, plr, mPlayer.syn[plr].color);
   }
   mPlayer.syn[plr].control_method = PM_PLAYER_CONTROL_METHOD_SINGLE_PLAYER;
}

