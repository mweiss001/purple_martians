// mwItemKeySwitch.cpp
#include "pm.h"
#include "mwItems.h"
#include "mwPlayers.h"
#include "mwLevel.h"
#include "z_screen_overlay.h"
#include "mwBitmap.h"
#include "mwColor.h"
#include "mwProgramState.h"
#include "mwDisplay.h"


int mwItems::draw_key(int i, int x, int y, int tile)
{
   int el = mItem.item[i][11];
   if (el > 0) // moving key
   {
      if (el < 10) // moving key in final sequence
      {
         // moving key in final stage gets static shape not ans
         int tile = mItem.item[i][1];                   // get tile
         if (tile > 999) tile = mwB.zz[5][tile-1000];   // get first tile in ans

         // stretch the key
         float sc = 1 + 5*((10 - (float)mItem.item[i][11]) / 10);
         float rot = (float) mItem.item[i][10] / 1000;
         al_draw_scaled_rotated_bitmap(mwB.tile[tile],10, 10, x+10, y+10, sc, sc, rot, 0);

         // draw a collapsing rectangle
         int x1 = mItem.item[i][6];
         int y1 = mItem.item[i][7];
         int x2 = (mItem.item[i][6] + mItem.item[i][8]);
         int y2 = (mItem.item[i][7] + mItem.item[i][9]);

         int xw = x2-x1;
         int yh = y2-y1;
         float xinc = xw/8;
         float yinc = yh/8;

         float seq = 9 - mItem.item[i][11]; // starts at 0, goes to 8
         int xo = (int)(seq * xinc / 2);
         int yo = (int)(seq * yinc / 2);
         al_draw_rectangle(x1+xo, y1+yo, x2-xo, y2-yo, mC.White, 1);
      }
      else
      {
         float rot = (float) mItem.item[i][10] / 1000;
         al_draw_rotated_bitmap(mwB.tile[tile], 10, 10, x+10, y+10, rot, 0);
      }
      return 1;
   }
   return 0;
}

void mwItems::proc_key_collision(int p, int i)
{
   if (mItem.item[i][11] == 0) // only collide if not already moving
   {
      float x2 = (mItem.item[i][6] + mItem.item[i][8] / 2) -10;       // get the center of the block range
      float y2 = (mItem.item[i][7] + mItem.item[i][9] / 2) -10;
      float xlen = x2 - itemf[i][0];                      // distance between block range and key
      float ylen = y2 - itemf[i][1];
      float hy_dist = sqrt(pow(xlen, 2) + pow(ylen, 2));  // hypotenuse distance
      float speed = 8;                                    // speed
      float scaler = hy_dist / speed;                     // get scaler (time, time = dist/speed)
      float xinc = xlen / scaler;                         // calc xinc
      float yinc = ylen / scaler;                         // calc yinc
      itemf[i][2] = xinc;
      itemf[i][3] = yinc;
      mItem.item[i][10] = atan2(yinc, xinc) * 1000;
      int num_steps = scaler;
      mItem.item[i][11] = num_steps + 10;                       // add 10 for final sequence
      game_event(2, 0, 0, p, i, 0, 0);
   }
}

void mwItems::proc_key_block_range(int i, int action)
{
   int x1 = mItem.item[i][6] / 20;
   int y1 = mItem.item[i][7] / 20;
   int x2 = (mItem.item[i][6] + mItem.item[i][8]) / 20;
   int y2 = (mItem.item[i][7] + mItem.item[i][9]) / 20;

   for (int x=x1; x<x2; x++)
      for (int y=y1; y<y2; y++)
      {
         if (mItem.item[i][12]) // matching keyed blocks only
         {
            int key = mItem.item[i][1] - 1039;
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

void mwItems::proc_moving_key(int i)
{
   // do the incs until the last 10 frames, which are for final sequence
   if (mItem.item[i][11] > 10)
   {
      itemf[i][0] += itemf[i][2];  // xinc
      itemf[i][1] += itemf[i][3];  // yinc
   }
   mItem.item[i][11]--;
   if (mItem.item[i][11] == 0)
   {
      mItem.item[i][0] = 0; // remove the key
      proc_key_block_range(i, 1);
    }
}


void mwItems::proc_switch_collision(int p, int i)
{
   if (mItem.item[i][11] < mwPS.frame_num) // if not lockout
   {
      float px = mPlayer.syn[p].x;
      float py = mPlayer.syn[p].y;
      float ix = itemf[i][0];
      float iy = itemf[i][1];
      if ( (px > ix-12) && (px < ix+12) && (py > iy-16) && (py < iy-8) && (mPlayer.syn[p].yinc > 0) )  // falling
      {
         mItem.item[i][11] = mwPS.frame_num + 4; // switch lockout for next 4 frames
         game_event(30, 0, 0, p, i, 0, 0);

         if (mItem.item[i][1] > 111) mItem.item[i][1] -= 16;
         else mItem.item[i][1] += 16;

         al_set_target_bitmap(mwB.level_background);
         proc_switch_block_range(i, 1);
      }
   }
}


void mwItems::proc_switch_block_range(int i, int action)
{
   int x1 = mItem.item[i][6] / 20;
   int y1 = mItem.item[i][7] / 20;
   int x2 = (mItem.item[i][6] + mItem.item[i][8]) / 20;
   int y2 = (mItem.item[i][7] + mItem.item[i][9]) / 20;


   if (action == 1)
   {
      al_set_target_bitmap(mwB.level_background);
      for (int x=x1; x<x2; x++)
         for (int y=y1; y<y2; y++)
         {
            int tn = mLevel.l[x][y]&1023;
            if ((mItem.item[i][10] & 0b0001) && (tn == 172)) mLevel.change_block(x, y, 7);  // green  solid to empty
            if ((mItem.item[i][10] & 0b0010) && (tn == 173)) mLevel.change_block(x, y, 8);  // red    solid to empty
            if ((mItem.item[i][10] & 0b0100) && (tn == 174)) mLevel.change_block(x, y, 9);  // blue   solid to empty
            if ((mItem.item[i][10] & 0b1000) && (tn == 175)) mLevel.change_block(x, y, 10); // purple solid to empty
            if ((mItem.item[i][10] & 0b0001) && (tn ==  7))  mLevel.change_block(x, y, (172 | PM_BTILE_ALL_SOLID)); // green  empty to solid
            if ((mItem.item[i][10] & 0b0010) && (tn ==  8))  mLevel.change_block(x, y, (173 | PM_BTILE_ALL_SOLID)); // red    empty to solid
            if ((mItem.item[i][10] & 0b0100) && (tn ==  9))  mLevel.change_block(x, y, (174 | PM_BTILE_ALL_SOLID)); // blue   empty to solid
            if ((mItem.item[i][10] & 0b1000) && (tn == 10))  mLevel.change_block(x, y, (175 | PM_BTILE_ALL_SOLID)); // purple empty to solid
         }
      al_set_target_backbuffer(display);
   }

   // this method sets block then calls init_level..
//   if (action == 1)
//   {
//      for (int x=x1; x<x2; x++)
//         for (int y=y1; y<y2; y++)
//         {
//            int tn = l[x][y]&1023;
//
//            if ((mItem.item[i][10] & 0b0001) && (tn == 172)) l[x][y] = 7;  // green  solid to empty
//            if ((mItem.item[i][10] & 0b0010) && (tn == 173)) l[x][y] = 8;  // red    solid to empty
//            if ((mItem.item[i][10] & 0b0100) && (tn == 174)) l[x][y] = 9;  // blue   solid to empty
//            if ((mItem.item[i][10] & 0b1000) && (tn == 175)) l[x][y] = 10; // purple solid to empty
//            if ((mItem.item[i][10] & 0b0001) && (tn ==  7))  l[x][y] = (172 | PM_BTILE_ALL_SOLID); // green  empty to solid
//            if ((mItem.item[i][10] & 0b0010) && (tn ==  8))  l[x][y] = (173 | PM_BTILE_ALL_SOLID); // red    empty to solid
//            if ((mItem.item[i][10] & 0b0100) && (tn ==  9))  l[x][y] = (174 | PM_BTILE_ALL_SOLID); // blue   empty to solid
//            if ((mItem.item[i][10] & 0b1000) && (tn == 10))  l[x][y] = (175 | PM_BTILE_ALL_SOLID); // purple empty to solid
//         }
//      if (action == 1) init_level_background(0);
//   }


   if (action == 2)
   {
      for (int x=x1; x<x2; x++)
         for (int y=y1; y<y2; y++)
         {
            int tn = mLevel.l[x][y]&1023;
            if ((mItem.item[i][10] & 0b0001) && ( (tn == 7)  || (tn == 172)) ) bomb_block_crosshairs(x, y);
            if ((mItem.item[i][10] & 0b0010) && ( (tn == 8)  || (tn == 173)) ) bomb_block_crosshairs(x, y);
            if ((mItem.item[i][10] & 0b0100) && ( (tn == 9)  || (tn == 174)) ) bomb_block_crosshairs(x, y);
            if ((mItem.item[i][10] & 0b1000) && ( (tn == 10) || (tn == 175)) ) bomb_block_crosshairs(x, y);
         }
   }
}
