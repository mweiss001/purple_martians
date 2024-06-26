// mwItemKeySwitch.cpp
#include "pm.h"
#include "mwItem.h"
#include "mwPlayer.h"
#include "mwLevel.h"
#include "mwGameEvent.h"
#include "mwBitmap.h"
#include "mwColor.h"
#include "mwLoop.h"
#include "mwDisplay.h"
#include "mwTriggerEvent.h"



int mwItem::draw_hider(int i, int x, int y, int tile)
{
   if (mLoop.level_editor_running) al_draw_bitmap(mBitmap.tile[476], x, y, 0); // draw item shape in level editor, invisible when game running
   return 1;
}

void mwItem::proc_hider(int i)
{
   int et = item[i][1];                 // number of pm_event trigger we are looking for
   int trig = mTriggerEvent.event[et];  // is the trigger event set?
   if (et == 0) trig = 0;               // if event is zero, ignore
   if (trig)
   {
      if (item[i][3] == 2) item[i][2] = 0;
      if (item[i][3] == 3) item[i][2] = 1;
      if (item[i][3] == 4) item[i][2] = !item[i][2];
      if (item[i][3] == 5) item[i][2] = 0;
   }
   else if (item[i][3] == 5) item[i][2] = 1;
}




void mwItem::erase_hider_areas(void)
{
   al_set_target_bitmap(mBitmap.level_buffer);
   for (int i=0; i<500; i++)
      if ((item[i][0] == 19) && (item[i][2]))
      {
         int x1 = item[i][6];
         int y1 = item[i][7];
         int x2 = x1 + item[i][8];
         int y2 = y1 + item[i][9];
         al_draw_filled_rectangle(x1, y1, x2, y2, mColor.Black);
      }
}




int mwItem::draw_key(int i, int x, int y, int tile)
{
   int el = item[i][11];
   if (el > 0) // moving key
   {
      if (el < 10) // moving key in final sequence
      {
         // moving key in final stage gets static shape not ans
         int tile = item[i][1];                   // get tile
         if (tile > 999) tile = mBitmap.zz[5][tile-1000];   // get first tile in ans

         // stretch the key
         float sc = 1 + 5*((10 - (float)item[i][11]) / 10);
         float rot = (float) item[i][10] / 1000;
         al_draw_scaled_rotated_bitmap(mBitmap.tile[tile],10, 10, x+10, y+10, sc, sc, rot, 0);

         // draw a collapsing rectangle
         int x1 = item[i][6];
         int y1 = item[i][7];
         int x2 = (item[i][6] + item[i][8]);
         int y2 = (item[i][7] + item[i][9]);

         int xw = x2-x1;
         int yh = y2-y1;
         float xinc = xw/8;
         float yinc = yh/8;

         float seq = 9 - item[i][11]; // starts at 0, goes to 8
         int xo = (int)(seq * xinc / 2);
         int yo = (int)(seq * yinc / 2);
         al_draw_rectangle(x1+xo, y1+yo, x2-xo, y2-yo, mColor.White, 1);
      }
      else
      {
         float rot = (float) item[i][10] / 1000;
         al_draw_rotated_bitmap(mBitmap.tile[tile], 10, 10, x+10, y+10, rot, 0);
      }
      return 1;
   }
   return 0;
}

void mwItem::proc_key_collision(int p, int i)
{
   if (item[i][11] == 0) // only collide if not already moving
   {
      float x2 = (item[i][6] + item[i][8] / 2) -10;       // get the center of the block range
      float y2 = (item[i][7] + item[i][9] / 2) -10;
      float xlen = x2 - itemf[i][0];                      // distance between block range and key
      float ylen = y2 - itemf[i][1];
      float hy_dist = sqrt(pow(xlen, 2) + pow(ylen, 2));  // hypotenuse distance
      float speed = 8;                                    // speed
      float scaler = hy_dist / speed;                     // get scaler (time, time = dist/speed)
      float xinc = xlen / scaler;                         // calc xinc
      float yinc = ylen / scaler;                         // calc yinc
      itemf[i][2] = xinc;
      itemf[i][3] = yinc;
      item[i][10] = atan2(yinc, xinc) * 1000;
      int num_steps = scaler;
      item[i][11] = num_steps + 10;                       // add 10 for final sequence
      mGameEvent.add(20, 0, 0, p, item[i][1], 0, 0);
   }
}

void mwItem::proc_key_block_range(int i, int action)
{
   int x1 = item[i][6] / 20;
   int y1 = item[i][7] / 20;
   int x2 = x1 + item[i][8] / 20;
   int y2 = y1 + item[i][9] / 20;

   if (x1 < 0) x1 = 0;
   if (x2 > 100) x2 = 100;
   if (y1 < 0) y1 = 0;
   if (y2 > 100) y2 = 100;

   for (int x=x1; x<x2; x++)
      for (int y=y1; y<y2; y++)
      {
         if (item[i][12]) // matching keyed blocks only
         {
            int key = item[i][1] - 1039;
            if (((mLevel.l[x][y]&1023) == 188 + key) || ((mLevel.l[x][y]&1023) == 204 + key) || ((mLevel.l[x][y]&1023) == 220 + key))
            {
               if (action == 1) mLevel.change_block(x, y, 0);
               if (action == 2) bomb_block_crosshairs(x, y);
            }
         }
         else // all blocks in range
         {
            if (action == 1) mLevel.change_block(x, y, 0);
            if (action == 2) bomb_block_crosshairs(x, y);
         }
      }
}

void mwItem::proc_moving_key(int i)
{
   // do the incs until the last 10 frames, which are for final sequence
   if (item[i][11] > 10)
   {
      itemf[i][0] += itemf[i][2];  // xinc
      itemf[i][1] += itemf[i][3];  // yinc
   }
   item[i][11]--;
   if (item[i][11] == 0)
   {
      item[i][0] = 0; // remove the key
      proc_key_block_range(i, 1);
   }
}


void mwItem::proc_switch_collision(int p, int i)
{
   if (item[i][11] < mLoop.frame_num) // if not lockout
   {
      float px = mPlayer.syn[p].x;
      float py = mPlayer.syn[p].y;
      float ix = itemf[i][0];
      float iy = itemf[i][1];
      if ( (px > ix-12) && (px < ix+12) && (py > iy-16) && (py < iy-8) && (mPlayer.syn[p].yinc > 0) )  // falling
      {
         item[i][11] = mLoop.frame_num + 4; // switch lockout for next 4 frames
         mGameEvent.add(21, 0, 0, p, item[i][1], 0, 0);
         if (item[i][1] > 111) item[i][1] -= 16;
         else item[i][1] += 16;
         al_set_target_bitmap(mBitmap.level_background);
         proc_switch_block_range(i, 1);
      }
   }
}


void mwItem::proc_switch_block_range(int i, int action)
{
   int x1 = item[i][6] / 20;
   int y1 = item[i][7] / 20;
   int x2 = x1 + item[i][8] / 20;
   int y2 = y1 + item[i][9] / 20;

   if (x1 < 0) x1 = 0;
   if (x2 > 100) x2 = 100;
   if (y1 < 0) y1 = 0;
   if (y2 > 100) y2 = 100;

   if (action == 1)
   {
      al_set_target_bitmap(mBitmap.level_background);
      for (int x=x1; x<x2; x++)
         for (int y=y1; y<y2; y++)
         {
            int tn = mLevel.l[x][y]&1023;
            if ((item[i][10] & 0b0001) && (tn == 172)) mLevel.change_block(x, y, 7);  // green  solid to empty
            if ((item[i][10] & 0b0010) && (tn == 173)) mLevel.change_block(x, y, 8);  // red    solid to empty
            if ((item[i][10] & 0b0100) && (tn == 174)) mLevel.change_block(x, y, 9);  // blue   solid to empty
            if ((item[i][10] & 0b1000) && (tn == 175)) mLevel.change_block(x, y, 10); // purple solid to empty
            if ((item[i][10] & 0b0001) && (tn ==  7))  mLevel.change_block(x, y, (172 | PM_BTILE_ALL_SOLID)); // green  empty to solid
            if ((item[i][10] & 0b0010) && (tn ==  8))  mLevel.change_block(x, y, (173 | PM_BTILE_ALL_SOLID)); // red    empty to solid
            if ((item[i][10] & 0b0100) && (tn ==  9))  mLevel.change_block(x, y, (174 | PM_BTILE_ALL_SOLID)); // blue   empty to solid
            if ((item[i][10] & 0b1000) && (tn == 10))  mLevel.change_block(x, y, (175 | PM_BTILE_ALL_SOLID)); // purple empty to solid
         }
      al_set_target_backbuffer(mDisplay.display);

   }

   // this method sets block then calls init_level..
//   if (action == 1)
//   {
//      for (int x=x1; x<x2; x++)
//         for (int y=y1; y<y2; y++)
//         {
//            int tn = l[x][y]&1023;
//
//            if ((item[i][10] & 0b0001) && (tn == 172)) l[x][y] = 7;  // green  solid to empty
//            if ((item[i][10] & 0b0010) && (tn == 173)) l[x][y] = 8;  // red    solid to empty
//            if ((item[i][10] & 0b0100) && (tn == 174)) l[x][y] = 9;  // blue   solid to empty
//            if ((item[i][10] & 0b1000) && (tn == 175)) l[x][y] = 10; // purple solid to empty
//            if ((item[i][10] & 0b0001) && (tn ==  7))  l[x][y] = (172 | PM_BTILE_ALL_SOLID); // green  empty to solid
//            if ((item[i][10] & 0b0010) && (tn ==  8))  l[x][y] = (173 | PM_BTILE_ALL_SOLID); // red    empty to solid
//            if ((item[i][10] & 0b0100) && (tn ==  9))  l[x][y] = (174 | PM_BTILE_ALL_SOLID); // blue   empty to solid
//            if ((item[i][10] & 0b1000) && (tn == 10))  l[x][y] = (175 | PM_BTILE_ALL_SOLID); // purple empty to solid
//         }
//      if (action == 1) init_level_background();
//   }

   if (action == 2)
   {
      for (int x=x1; x<x2; x++)
         for (int y=y1; y<y2; y++)
         {
            int tn = mLevel.l[x][y]&1023;
            if ((item[i][10] & 0b0001) && ( (tn == 7)  || (tn == 172)) ) bomb_block_crosshairs(x, y);
            if ((item[i][10] & 0b0010) && ( (tn == 8)  || (tn == 173)) ) bomb_block_crosshairs(x, y);
            if ((item[i][10] & 0b0100) && ( (tn == 9)  || (tn == 174)) ) bomb_block_crosshairs(x, y);
            if ((item[i][10] & 0b1000) && ( (tn == 10) || (tn == 175)) ) bomb_block_crosshairs(x, y);
         }
   }
}

