// mw_ItemStartExitWarp.cpp

#include "pm.h"
#include "mwItem.h"
#include "mwPlayer.h"
#include "mwBitmap.h"
#include "mwColor.h"
#include "mwLoop.h"
#include "mwEnemy.h"
#include "mwLevel.h"
#include "mwGameEvent.h"
#include "mwFont.h"

int mwItem::draw_start(int i, int x, int y, int shape)
{
   if ((mLevel.number_of_starts > 1) && (mLoop.level_editor_running)) // put start seq number, but only in lev editor
   {
      al_draw_bitmap(mBitmap.tile[shape], x, y, 0);
      al_draw_textf(mFont.pixl, mColor.pc[12], x+10, y-4, ALLEGRO_ALIGN_CENTER, "%d", mItem.item[i][7]);
      return 1;
   }
   return 0;
}

int mwItem::draw_exit(int i, int x, int y, int shape)
{
   al_draw_bitmap(mBitmap.tile[399], x, y, 0); // 'exit' text not shown
   if (mLoop.frame_num % 60 > 30)
      al_draw_text(mFont.pixl, mColor.pc[10], x+11, y-2, ALLEGRO_ALIGN_CENTER, "EXIT");

   int exit_enemys_left = mEnemy.num_enemy - mItem.item[i][8];
   if (exit_enemys_left > 0) // locked
   {
      al_draw_bitmap(mBitmap.tile[366], x, y, 0); // show lock
      if (mLoop.frame_num % 60 < 30)
         al_draw_textf(mFont.pixl, mColor.pc[14], x+11, y-2, ALLEGRO_ALIGN_CENTER, "%d", exit_enemys_left);

   }
   return 1;
}

void mwItem::proc_start_collision(int p, int i)
{
   int ns = 0; // count number of starts
   int s[8] = {0};
   for (int i=0; i<500; i++)
      if (mItem.item[i][0] == 5)
      {
         ns++;
         s[mItem.item[i][7]] = i; // save index of this start
      }
/*
   if (ns == 0)
   {
      printf("Error: no start found.\n");
      mPlayer.syn[p].x = 20;
      mPlayer.syn[p].y = 20;;
   }

   if (ns == 1)
   {
      mPlayer.syn[p].spawn_point_index = 0;
      int ps = s[mPlayer.syn[p].spawn_point_index];
      mPlayer.syn[p].x = itemf[ps][0];
      mPlayer.syn[p].y = itemf[ps][1];
   }
*/

   if (ns > 1)
   {
      int mode = mItem.item[s[0]][6];

      if (mode == 2) // check point common
      {
         for (p=0; p<8; p++)
            mPlayer.syn[p].spawn_point_index = mItem.item[i][7]; // set new spawn point for all players

         // mark this one as active and all others as not
         for (int ii=0; ii<500; ii++)
            if (mItem.item[ii][0] == 5)
            {
               if (mItem.item[ii][7] == mItem.item[i][7]) mItem.item[ii][1] = 1021;
               else mItem.item[ii][1] = 1011;
            }
      }

      if (mode == 3) // check point individual
      {
         mPlayer.syn[p].spawn_point_index = mItem.item[i][7]; // set new spawn point for this player
      }

   }
//   mPlayer.syn[p].spawn_point_index = mItem.item[i][7]; // set new spawn point
//   // mark this one as active and all others as not
//   for (int ii=0; ii<500; ii++)
//      if (mItem.item[ii][0] == 5)
//      {
//         if (mItem.item[ii][7] == mItem.item[i][7]) mItem.item[ii][1] = 1021;
//         else mItem.item[ii][1] = 1011;
//      }
}

void mwItem::proc_exit_collision(int p, int i)
{
   int exit_enemys_left = mEnemy.num_enemy - item[i][8];
   if (exit_enemys_left <= 0)
   {
      if (mPlayer.syn[0].level_done_mode == 0)
      {
         mPlayer.syn[0].level_done_mode = 9;
         mPlayer.syn[0].level_done_timer = 0;
         mPlayer.syn[0].level_done_x = itemf[i][0];
         mPlayer.syn[0].level_done_y = itemf[i][1];
         mPlayer.syn[0].level_done_player = p;
         mPlayer.syn[0].level_done_next_level = mLevel.play_level + 1;
         mGameEvent.add(4, 0, 0, 0, 0, 0, 0);
      }
   }
   else mGameEvent.add(23, 0, 0, p, i, exit_enemys_left, 0); // not enough dead yet
}

void mwItem::proc_warp_collision(int p, int i)
{
   if (mPlayer.syn[0].level_done_mode == 0)
   {
      if (mLevel.play_level > 1) mLevel.warp_level_location = mLevel.play_level;
      mPlayer.syn[0].level_done_mode = 3;
      mPlayer.syn[0].level_done_timer = 0;
      mPlayer.syn[0].level_done_x = itemf[i][0];
      mPlayer.syn[0].level_done_y = itemf[i][1];
      mPlayer.syn[0].level_done_player = p;
      mPlayer.syn[0].level_done_next_level = item[i][8];
      mGameEvent.add(4, 0, 0, p, i, 0, 0);
   }
}


void mwItem::proc_gate_collision(int p, int i)
{
   if (mPlayer.syn[p].up)
   {
      mPlayer.syn[0].level_done_mode = 3;
      mPlayer.syn[0].level_done_timer = 0;
      mPlayer.syn[0].level_done_next_level = item[i][6];
   }
}


int mwItem::draw_gate(int i, int x, int y, int custom)
{
   al_draw_bitmap(mBitmap.tile[940], x, y, 0);
   al_draw_textf(mFont.pixl, mColor.pc[15], x+10, y+4, ALLEGRO_ALIGN_CENTER, "%d", item[i][6]);
   return 1;
}




























