// mwGmInfo.cpp

#include "pm.h"
#include "mwGmInfo.h"

#include "mwBitmap.h"
#include "mwColor.h"
#include "mwDemoMode.h"
#include "mwDrawSequence.h"
#include "mwEventQueue.h"
#include "mwFont.h"
#include "mwGameMoves.h"
#include "mwInput.h"
#include "mwItem.h"
#include "mwLevel.h"
#include "mwLoop.h"
#include "mwMiscFnx.h"
#include "mwPlayer.h"


mwGmInfo mGmInfo;


void mwGmInfo::clear()
{
   gmPlayerInfo.clear();

   lastFrame = -1;
   levelDoneFrame = -1;
   levelDonePlayer = -1;
}


// called at the end of load_gm
void mwGmInfo::fill()
{
   clear();

   lastFrame = mDemoMode.last_frame;
   findLevelDone();
   if (levelDoneFrame != -1) lastFrame = levelDoneFrame;

   int p0_hidden = 0;

   for (int x=0; x<mGameMoves.entry_pos; x++)
   {
      int f = mGameMoves.arr[x][0]; // frame
      int t = mGameMoves.arr[x][1]; // game move type

      if (t & PM_GAMEMOVE_TYPE_PLAYER_ACTIVE_FLAG)
      {
         int p, c;
         char name[9] = { 0 };
         mMiscFnx.gma_to_val(mGameMoves.arr[x][1], mGameMoves.arr[x][2], mGameMoves.arr[x][3], p, c, name);
         gmPlayerInfo.push_back({ p, c, name, f, 0    });
      }

      // this will only ever be player 0 in headless server mode
      if (t == PM_GAMEMOVE_TYPE_PLAYER_HIDDEN) p0_hidden = 1;

      if (t == PM_GAMEMOVE_TYPE_PLAYER_INACTIVE)
      {
         int p = mGameMoves.arr[x][2]; // player number
         // find existing that matches player num and has endFrame set to 0
         for (auto& r : gmPlayerInfo)
            if (r.playerNum == p && r.endFrame == 0) r.endFrame = f;
      }
   }

   // if last frame not set, set to last frame of gm
   for (auto& r : gmPlayerInfo)
      if (r.endFrame == 0) r.endFrame = lastFrame;

   if (p0_hidden)
   {
      for (int i=0; i<(int)gmPlayerInfo.size(); i++)
      {
         if (gmPlayerInfo[i].playerNum == 0)
         {
            gmPlayerInfo.erase(gmPlayerInfo.begin() + i);
            break;
         }
      }
   }


}


void mwGmInfo::showTimeline(int x1, int y1, int x2, int y2, int col)
{
   // clear background and draw frame
   al_draw_filled_rectangle(x1, y1, x2, y2, mColor.pc[col]);
   al_draw_rectangle(       x1, y1, x2, y2, mColor.pc[15], 1);

   // get start frame and last frame
   float sf = 0;
   float lf = lastFrame;

   // don't let currentFrame exceed lastFrame
   int currentFrame = mLoop.frame_num;
   if (currentFrame > lastFrame) currentFrame = lastFrame;

   // get nicely formatted time string
   char msg[80];
   //sprintf(msg, "Frame:%d", currentFrame);
   sprintf(msg, "%s", mMiscFnx.chrms(currentFrame, msg));

   // get centered text position
   float tx = (x1+x2)/2;
   float ty = y1 + ((y2-y1) - 8)/2;
   al_draw_text(mFont.pr8, mColor.pc[15], tx, ty, ALLEGRO_ALIGN_CENTRE, msg);


   // draw vertical line at current frame
   float fx = map_range((float)currentFrame, sf, lf,  (float)x1, (float)x2);
   al_draw_line(fx, y1, fx, y2, mColor.pc[15], 1);

   // is mouse on frame?
   if ((mInput.mouse_x > x1) && (mInput.mouse_x < x2) && (mInput.mouse_y > y1) && (mInput.mouse_y < y2))
   {
      // draw vertical line at mouse pos frame
      float fx = (float) mInput.mouse_x;
      al_draw_line(fx, y1, fx, y2, mColor.pc[14], 1);

      // is mouse button pressed?
      if (mInput.mouse_b[1][0])
      {
         while (mInput.mouse_b[1][0]) mEventQueue.proc(1); // wait for release

         // map screen x position to frame num
         float f = map_range( (float) mInput.mouse_x, (float)x1, (float)x2, sf, lf);

         // set frame num and seek to frame
         mLoop.frame_num = f;
         mDemoMode.seek_to_frame(f, 1);
      }
   }
}


void mwGmInfo::showTimelineTracks(int y2, int x2, int bts, int ls, int x1)
{
   mColor.process_flash_color();

   // get start frame and last frame
   float sf = 0;
   float lf = lastFrame;

   int height = ls + (bts+ls)*(int)gmPlayerInfo.size() + 2;

   int y1 = y2 - height;

   al_draw_filled_rectangle(x1, y1, x2, y2, mColor.pc[0]);
   al_draw_rectangle(       x1, y1, x2, y2, mColor.pc[15], 1);


   // starting y position
   float ry2 = y2 - ls;

   // full x range of inner rect
   float ix1 = x1 + ls;
   float ix2 = x2 - ls;


   int mouse_on_bar = -1;


   for (auto& r : gmPlayerInfo)
   {
      int p = r.playerNum;
      int c = r.playerCol;

      int frameColor = c;
      int frameWidth = 1;


      // set ry1
      float ry1 = ry2 - bts;

      if (p == mPlayer.active_local_player)
      {
         ry1-=2;
         frameColor = 15;
         frameWidth = 2;
      }



      // map frame start and end to screen positions
      float rx1 = map_range((float)r.startFrame, sf, lf,  ix1, ix2);
      float rx2 = map_range((float)r.endFrame,   sf, lf,  ix1, ix2);

      // fill the background
      al_draw_filled_rectangle(rx1, ry1, rx2, ry2, mColor.pc[c+144]);



      // tile position
      int ty1 = ry1+2;
      int ty2 = ry2-2;
      int th = ty2 - ty1;
      int tx1 = rx1 + 2;
      int tx2 = tx1 + th;

      // erase tile background
      al_draw_filled_rectangle(tx1, ty1, tx2, ty2, mColor.pc[c+192]);

      // draw tile
      al_draw_scaled_bitmap(mBitmap.player_tile[c][1], 0, 0, 20, 20, tx1, ty1, th, th, 0);


      // text position (centered vertically)
      float txtx = tx2 + 4;
      float txty = ry1 + ((ry2-ry1) - 8)/2;
      al_draw_textf(mFont.pr8, mColor.pc[15], txtx, txty, 0, "%s", r.playerName.c_str());





      // highlight for active player
      if (p == mPlayer.active_local_player) al_draw_rectangle(rx1, ry1, rx2, ry2, mColor.pc[15], 2);

      // is mouse on bar?
      if ((mInput.mouse_x > rx1) && (mInput.mouse_x < rx2) && (mInput.mouse_y > ry1) && (mInput.mouse_y < ry2))
      {
         mouse_on_bar = p;
         frameColor = mColor.flash_color;
      }

      // frame
      al_draw_rectangle(rx1, ry1, rx2, ry2, mColor.pc[frameColor], frameWidth);







      // next bar position
//      ry2 -= (bts + ls);

      ry2 = ry1 - ls;

   }

   // don't let currentFrame exceed lastFrame
   int currentFrame = mLoop.frame_num;
   if (currentFrame > lastFrame) currentFrame = lastFrame;

   // draw vertical line at current frame
   float fx = map_range((float)currentFrame, sf, lf,  ix1, ix2);
   al_draw_line(fx, y1, fx, y2, mColor.pc[15], 1);


/*
   // draw vertical line at level done
   fx = map_range((float)levelDoneFrame, sf, lf,  ix1, ix2);
   al_draw_line(fx, y1, fx, y2, mColor.pc[11], 1);
*/


   // is mouse on frame?
   if ((mInput.mouse_x > x1) && (mInput.mouse_x < x2) && (mInput.mouse_y > y1) && (mInput.mouse_y < y2))
   {
      // draw vertical line at mouse pos frame
      float fx = (float) mInput.mouse_x;
      al_draw_line(fx, y1, fx, y2, mColor.pc[14], 1);

      // is mouse button pressed?
      if (mInput.mouse_b[1][0])
      {
         while (mInput.mouse_b[1][0]) mEventQueue.proc(1); // wait for release

         // map screen x position to frame num
         float f = map_range( (float) mInput.mouse_x, ix1, ix2, sf, lf);

         // set frame num and seek to frame
         mLoop.frame_num = f;
         mDemoMode.seek_to_frame(f, 1);

         // if mouse_on_bar set active local player to that bar
         if (mouse_on_bar != -1) mDemoMode.set_active_local_player(mouse_on_bar);
      }
   }

}


void mwGmInfo::findLevelDone()
{
   levelDoneFrame = -1;
   levelDonePlayer = -1;

   mDemoMode.seek_to_frame(0, 0);

   mLoop.ff_state = 1;
   int done = 0;
   while (!done)
   {
      mLoop.frame_num++;
      mGameMoves.proc();
      mLoop.move_frame();
      if (mPlayer.syn[0].level_done_mode)
      {
         levelDoneFrame  = mPlayer.syn[0].level_done_frame;
         levelDonePlayer = mPlayer.syn[0].level_done_player;
         done = 1;
      }
      if (mLoop.frame_num > mDemoMode.last_frame + 400) done = 1;
   }
   mLoop.ff_state = 0;
   mDemoMode.seek_to_frame(0, 0);

}

