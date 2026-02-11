// mwGmInfo.cpp

#include "pm.h"
#include "mwGmInfo.h"

#include "mwColor.h"
#include "mwDemoMode.h"
#include "mwEventQueue.h"
#include "mwFont.h"
#include "mwGameMoves.h"
#include "mwInput.h"
#include "mwLoop.h"
#include "mwMiscFnx.h"
#include "mwPlayer.h"


mwGmInfo mGmInfo;

void mwGmInfo::setTestData()
{
   gmPlayerInfo.clear();


   gmPlayerInfoRecord r;

   r.playerNum = 3;
   r.playerCol = 10;
   r.playerName = "oppo";
   r.startFrame = 0;
   r.endFrame = 145;

   gmPlayerInfo.push_back(r);

   gmPlayerInfo.push_back({ 4, 11, "poop", 0, 233    });


}



void mwGmInfo::fill()
{
   gmPlayerInfo.clear();

   int p0_hidden = 0;


   for (int x=0; x<mGameMoves.entry_pos; x++)
   {
      int f = mGameMoves.arr[x][0]; // frame
      int t = mGameMoves.arr[x][1]; // game move type
      int p = mGameMoves.arr[x][2]; // player number
      int c = 0;
      char name[9] = { 0 };


      if (t & PM_GAMEMOVE_TYPE_PLAYER_ACTIVE_FLAG) // new version with embedded name
      {
         mMiscFnx.gma_to_val(mGameMoves.arr[x][1], mGameMoves.arr[x][2], mGameMoves.arr[x][3], p, c, name);

         // add record
         gmPlayerInfo.push_back({ p, c, name, f, 0    });

      }

      // this will only ever be player 0 in headless server mode
      if (t == PM_GAMEMOVE_TYPE_PLAYER_HIDDEN) p0_hidden = 1;

      if (t == PM_GAMEMOVE_TYPE_PLAYER_INACTIVE)
      {
         // find existing that matches player num and has endFrame set to 0
         for (auto& r : gmPlayerInfo)
            if (r.playerNum == p && r.endFrame == 0) r.endFrame = f;
      }
   }

   // if last frame not set, set to last frame of gm
   for (auto& r : gmPlayerInfo)
      if (r.endFrame == 0) r.endFrame = mDemoMode.last_frame;



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
//   printTestData();

}



void mwGmInfo::draw(int x1, int y1, int x2, int bts)
{
   bts = 12;

   int ls = 4; // line_spacing

   float sf = 0; // start frame
   float lf = mDemoMode.last_frame;

   int size = (int)gmPlayerInfo.size();
   int y2 = y1 + ls + (bts+ls)*size;

   al_draw_filled_rectangle(x1, y1, x2, y2, mColor.pc[0]);
   al_draw_rectangle(       x1, y1, x2, y2, mColor.pc[15], 1);

   float ry1 = y1 + ls;


   // x range of inner rect
   float ix1 = x1 + ls;
   float ix2 = x2 - ls;


   int mouse_on_bar = -1;


   for (auto& r : gmPlayerInfo)
   {
      int p = r.playerNum;

      // set y2
      float ry2 = ry1 + bts;

      // map frame start and end to screen positions
      float rx1 = map_range((float)r.startFrame, sf, lf,  ix1, ix2);
      float rx2 = map_range((float)r.endFrame,   sf, lf,  ix1, ix2);

      // draw the bar
      al_draw_filled_rectangle(rx1, ry1, rx2, ry2, mColor.pc[r.playerCol]);

      // draw text centered vertically
      float ty = ry1 + ((ry2-ry1) - 8)/2;
      al_draw_textf(mFont.pr8, mColor.pc[0], rx1+4, ty, 0, "%s - %d", r.playerName.c_str(), r.playerNum);

      // draw highlight for active player
      if (p == mPlayer.active_local_player) al_draw_rectangle(rx1, ry1, rx2, ry2, mColor.pc[15], 2);

      // is mouse on bar?
      if ((mInput.mouse_x > rx1) && (mInput.mouse_x < rx2) && (mInput.mouse_y > ry1) && (mInput.mouse_y < ry2))
      {
         mouse_on_bar = p;
         // draw highlight for mouse on bar
         al_draw_rectangle(rx1, ry1, rx2, ry2, mColor.pc[15], 1);
      }
      // next bar position
      ry1 += bts + ls;
   }

   // draw vertical line at current frame
   float fx = map_range((float)mLoop.frame_num, sf, lf,  ix1, ix2);
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
         float f = map_range( (float) mInput.mouse_x, ix1, ix2, sf, lf);

         // set frame num and seek to frame
         mLoop.frame_num = f;
         mDemoMode.seek_to_frame(f, 1);

         // if mouse was on bar set active local player to that bar
         if (mouse_on_bar) mDemoMode.set_active_local_player(mouse_on_bar);
      }
   }
}





void mwGmInfo::printTestData()
{
   printf("Vector size:%d\n", (int)gmPlayerInfo.size());


   // Using a range-based for loop
   for (const auto& r : gmPlayerInfo)
   {
      printf("\nplayerNum:%d\n", r.playerNum);
      printf("playerCol:%d\n", r.playerCol);

      printf("playerName:%s\n", r.playerName.c_str());


      printf("startFrame:%d\n", r.startFrame);
      printf("endFrame:%d\n", r.endFrame);

   }
/*
   // Using a regular for loop
   for (int i=0; i<(int)gmPlayerInfo.size(); i++)
   {
      printf("\nplayerNum:%d\n", gmPlayerInfo[i].playerNum);
      printf("playerCol:%d\n", gmPlayerInfo[i].playerCol);
   }
*/


}
