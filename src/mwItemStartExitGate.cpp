// mw_ItemStartExitGate.cpp

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
#include "mwGameMoves.h"
#include "mwMain.h"
#include "mwConfig.h"
#include "mwDemoMode.h"
#include "mwScreen.h"
#include "mwDisplay.h"



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

         if (mMain.classic_mode) mPlayer.syn[0].level_done_next_level = mLevel.get_next_level(mLevel.play_level);
         else                    mPlayer.syn[0].level_done_next_level = 1;

         mLevel.level_complete_data();

         mGameEvent.add(4, 0, 0, 0, 0, 0, 0);
      }
   }
   else mGameEvent.add(23, 0, 0, p, i, exit_enemys_left, 0); // not enough dead yet
}



// used to save absolute screen position of gate, so transition returning to overworld lines up
void mwItem::set_gate_level_icon_position(int i)
{
   // gate item locatiom
   int x = itemf[i][0];
   int y = itemf[i][1];

   // offset from gate item to level icon
   x += (10 - 100);
   y += (12 + 35);

   // convert to actual screen pos
   mScreen.gate_transition_x = (x - mDisplay.level_display_region_x) * mDisplay.scale_factor_current + mDisplay.screen_display_x;
   mScreen.gate_transition_y = (y - mDisplay.level_display_region_y) * mDisplay.scale_factor_current + mDisplay.screen_display_y;
   mScreen.gate_transition_wx = mDisplay.level_display_region_x;
   mScreen.gate_transition_wy = mDisplay.level_display_region_y;
   mScreen.gate_transition_scale = mDisplay.scale_factor_current;
}

void mwItem::proc_gate_collision(int p, int i)
{
   mPlayer.syn[p].marked_gate = i;
   set_gate_level_icon_position(i);
   int lev = item[i][6];

   // set current overworld level for active local player
   if (p == mPlayer.active_local_player)
   {
      if (mLevel.overworld_level != lev)
      {
         mLevel.overworld_level = lev;
         mConfig.save();
      }
   }


   // set level complete for testing
   if (mPlayer.syn[p].fire)
   {

      mLevel.data[item[i][6]].completed = 1;
      mLevel.level_complete_data();
      mLevel.load_level(1,0,1);
   }


   // immediate next level to gate level
   if ((mPlayer.syn[p].up) && (mLevel.data[item[i][6]].unlocked))
   {
      mPlayer.syn[0].level_done_mode = 3;
      mPlayer.syn[0].level_done_timer = 0;
      mPlayer.syn[0].level_done_next_level = item[i][6];

      mLoop.quit_action = 2;
      mLoop.done_action = 2;
   }

   // cycle display pages
   if (mPlayer.syn[p].down)
   {
      if (!item[i][8])   // if down not held
      {
         if (++mLevel.display_page > 5) mLevel.display_page = 0;

      }
      item[i][8] = 1;    // set down held
   }
   else item[i][8] = 0;  // set down not held

}


// puts left justified txt1, right justified txt2, and ... in between
void mwItem::draw_line(int x1, int x2, int y, const char * txt1, const char * txt2, int col)
{
   al_draw_text(mFont.pr8, mColor.pc[15], x1+1, y, 0, txt1);
   int tl = strlen(txt2)*8;
   al_draw_filled_rectangle(x2-tl, y, x2, y+8, mColor.pc[0]);
   al_draw_textf(mFont.pr8, mColor.pc[15], x2-tl-1, y, 0, txt2);
}

//// format time from frames to seconds or minutes
//char * mwItem::chrms(int time, char* ft)
//{
//   if (time < 2400) sprintf(ft, "%0.1fs", (float)time/40);
//   else
//   {
//      int m = time / 2400; // minutes
//      int rt = time - m*2400; // remaining portion that is less than 1 minute
//      //sprintf(ft, "%d:%04.1fs", m, (float)rt/40); // show 1 decimal on seconds
//      sprintf(ft, "%d:%02d", m, rt/40); // round to nearest second
//   }
//   return ft;
//}

// format time from frames to seconds or minutes

char * mwItem::chrms(int time, char* ft)
{
   // less than 1 minute
   if (time < 2400) sprintf(ft, "%0.1fs", (float)time/40);

   // less than 1 hour
   else if (time < 144000)
   {
      int m = time / 2400;    // minutes
      int rt = time - m*2400; // remaining portion that is less than 1 minute
      sprintf(ft, "%d:%02d", m, rt/40);
   }
   else
   {
      int h = time / 144000;    // hours
      int rt = time - h*144000; // remaining portion that is less than 1 hour
      int m = rt / 2400;        // minutes
      rt -= m*2400;             // remaining portion that is less than 1 minute
      sprintf(ft, "%d:%02d:%02d", h, m, rt/40);
   }
   return ft;
}






char * chrd(int v, char* ft)
{
   sprintf(ft, "%d", v);
   return ft;
}

char * chrd(int v1, int v2, char* ft)
{
   sprintf(ft, "%d/%d", v1, v2);
   return ft;
}

void mwItem::show_page(int page, int xc, int bs, int by, int lev, int col)
{
   char msg[80];
   int yi=10;

   int bx = xc-bs/2;  // level icon x1
   int bx2 = xc+bs/2; // level icon x2

   int tc = 0; // number of times completed
   int tt = 0; // total time spent playing
   int wt = 0; // worst time
   int bt = mLevel.data[lev].best_time; // best time
   int pt = mLevel.data[lev].par_time;  // par time

   for (int i=0; i<mLevel.play_data_num; i++)
      if (mLevel.play_data[i].level == lev)
      {
         tc++;
         tt += mLevel.play_data[i].timer;
         if (mLevel.play_data[i].timer > wt) wt = mLevel.play_data[i].timer;
      }

   int mt = 0;
   if (tc) mt = tt/tc; // average time

   if (page == 0) // level icon map
   {
      // frame and draw level icon
      al_draw_rectangle(bx, by+12, bx+bs+1, by+bs+13, mColor.pc[col], 1);
      al_draw_bitmap(mLevel.level_icon[lev], bx, by+12, 0);
   }

   if (page == 1) // general
   {
      int yp = by+12;
      int yb = yp+72;
      al_draw_filled_rectangle(bx, yp, bx2+1, yb, mColor.pc[0]);
      al_draw_rectangle(bx, yp, bx2+1, yb, mColor.pc[col], 1);
      al_draw_scaled_bitmap(mBitmap.tile[127+col], 0, 0, 20, 20, bx+2, yp+0, 20, 20, 0);
      al_draw_textf(mFont.pr8, mColor.pc[15], bx+24, yp+7, 0, "General");
      al_draw_line(bx, yp+20, bx2+1, yp+20, mColor.pc[col], 1);
      yp+=22;
      draw_line(bx, bx2, yp, "Best Time................", chrms(bt, msg), 15); yp+=yi;
      draw_line(bx, bx2, yp, "Min Player Deaths........", chrd(mLevel.data[lev].min_respawns, msg), 15); yp+=yi;
      draw_line(bx, bx2, yp, "Max Purple Coins.........", chrd(mLevel.data[lev].max_purple_coins_collected, mLevel.data[lev].tot_purple_coins, msg), 15); yp+=yi;
      draw_line(bx, bx2, yp, "Max Enemies Killed.......", chrd(mLevel.data[lev].max_enemies_killed, msg), 15); yp+=yi;
      draw_line(bx, bx2, yp, "Min Enemies Left.........", chrd(mLevel.data[lev].min_enemies_left, msg), 15); yp+=yi;
   }

   if (page == 2) // times
   {
      int yp = by+12;
      int yb = yp+82;
      al_draw_filled_rectangle(bx, yp, bx2+1, yb, mColor.pc[0]);
      al_draw_rectangle(bx, yp, bx2+1, yb, mColor.pc[col], 1);
      al_draw_scaled_bitmap(mBitmap.tile[542], 3, 3, 14, 14, bx+4, yp+3, 14, 14, 0); // show clock
      al_draw_textf(mFont.pr8, mColor.pc[15], bx+24, yp+7, 0, "Time to Complete");
      al_draw_line(bx, yp+20, bx2+1, yp+20, mColor.pc[col], 1);
      yp+=22;
      draw_line(bx, bx2, yp, "Par Time.................", chrms(pt, msg), 15); yp+=yi;
      draw_line(bx, bx2, yp, "Best Time................", chrms(bt, msg), 15); yp+=yi;
      draw_line(bx, bx2, yp, "Worst Time...............", chrms(wt, msg), 15); yp+=yi;
      draw_line(bx, bx2, yp, "Average Time.............", chrms(mt, msg), 15); yp+=yi;
      draw_line(bx, bx2, yp, "Total Play Time..........", chrms(tt, msg), 15); yp+=yi;
      draw_line(bx, bx2, yp, "Times Played.............", chrd(tc, msg), 15); yp+=yi;
   }

   if (page == 3) // purple coins
   {
      int yp = by+12;
      int yb = yp+42;
      al_draw_filled_rectangle(bx, yp, bx2+1, yb, mColor.pc[0]);
      al_draw_rectangle(bx, yp, bx2+1, yb, mColor.pc[col], 1);
      al_draw_scaled_bitmap(mBitmap.tile[197], 0, 0, 19, 19, bx+4, yp+4, 12, 12, 0); // show purple coin
      al_draw_textf(mFont.pr8, mColor.pc[15], bx+24, yp+6, 0, "Purple Coins");
      al_draw_line(bx, yp+20, bx2+1, yp+20, mColor.pc[col], 1);
      yp+=22;
      draw_line(bx, bx2, yp, "Collected................", chrd(mLevel.data[lev].max_purple_coins_collected, msg), 15); yp+=yi;
      draw_line(bx, bx2, yp, "Total....................", chrd(mLevel.data[lev].tot_purple_coins, msg), 15); yp+=yi;
   }

   if (page == 7) // not completed general
   {
      int yp = by+12;
      int yb = yp+42;
      al_draw_filled_rectangle(bx, yp, bx2+1, yb, mColor.pc[0]);
      al_draw_rectangle(bx, yp, bx2+1, yb, mColor.pc[col], 1);
      al_draw_scaled_bitmap(mBitmap.tile[127+col], 0, 0, 20, 20, bx+2, yp+0, 20, 20, 0);
      al_draw_textf(mFont.pr8, mColor.pc[15], bx+24, yp+7, 0, "General");
      al_draw_line(bx, yp+20, bx2+1, yp+20, mColor.pc[col], 1);
      yp+=22;
      draw_line(bx, bx2, yp, "Par Time.................", chrms(pt, msg), 15); yp+=yi;
      draw_line(bx, bx2, yp, "Purple Coins.............", chrd(mLevel.data[lev].tot_purple_coins, msg), 15); yp+=yi;
   }

   if (page == 12) // run demo for level
   {
      int yp = by+12;
      int yb = yp+32;
      al_draw_filled_rectangle(bx, yp, bx2+1, yb, mColor.pc[0]);
      al_draw_rectangle(bx, yp, bx2+1, yb, mColor.pc[col], 1);
      al_draw_scaled_bitmap(mBitmap.tile[401], 0, 0, 20, 20, bx+2, yp-4, 20, 20, 0); // show player
      al_draw_textf(mFont.pr8, mColor.pc[15], bx+24, yp+6, 0, "Run Demo");
      al_draw_line(bx, yp+20, bx2+1, yp+20, mColor.pc[col], 1);
      yp+=22;
      al_draw_textf(mFont.pr8, mColor.pc[15], xc, yp, ALLEGRO_ALIGN_CENTER, "type 'demo' to run demo"); yp+=yi;

//      if (mLevel.skc_trigger_demo)
//      {
//         mLevel.skc_trigger_demo = 0;
//         if (mGameMoves.load_gm(lev))
//         {
//            mDemoMode.mode = 1;
//            mDemoMode.restore_mode = 42;
//            mDemoMode.restore_level = lev;
//            mLoop.state[0] = 31;
//         }
//      }
//

   }

   // trigger on any page
   if (mLevel.skc_trigger_demo)
   {
      mLevel.skc_trigger_demo = 0;
      if (mGameMoves.load_gm(lev))
      {
         mDemoMode.mode = 1;
         mDemoMode.restore_mode = 42;
         mDemoMode.restore_level = lev;
         mLoop.state[0] = 31;
         mLoop.quit_action = 2;
         mLoop.done_action = 2;


      }
   }



}


void draw_framed_text(int xc, int y, ALLEGRO_FONT *f, int col, const char* txt)
{
   // get text dimensions
   int bx, by, bw, bh;
   mFont.mw_get_text_dimensions(f, txt, bx, by, bw, bh);

   int xs = bw/2 + 2;
   int x1 = xc-xs;
   int x2 = xc+xs;

   // draw
   al_draw_filled_rectangle(x1, y, x2, y+bh+3, mColor.pc[0]);     // clear
   al_draw_rectangle       (x1, y, x2, y+bh+3, mColor.pc[col], 1); // frame
   al_draw_textf(f, mColor.pc[15], xc, y-by+1, ALLEGRO_ALIGN_CENTER, "%s", txt);
}







void mwItem::draw_gate_info(int i)
{
   int x = item[i][4];
   int y = item[i][5];
   int xc = x + 10; // center of tile
   int lev = item[i][6];

   int col, status;
   char stxt[80];
   mLevel.get_level_status(lev, status, col, stxt);

   int bs = al_get_bitmap_width(mLevel.level_icon[lev]); // level icon size
   int by = y+35; // info y start pos
   if (by > 1900) by = y - 236;
   draw_framed_text(xc, by+1, mFont.pr8, col, mLevel.data[lev].level_name); // draw and frame the level name

   if (status == 0)
   {
      //al_draw_textf(mFont.pixl, mColor.pc[col], xc, y-30, ALLEGRO_ALIGN_CENTER, "Locked");
      al_draw_textf(mFont.pixl, mColor.pc[15], xc, y+18, ALLEGRO_ALIGN_CENTER, "DOWN - Cycle Info");
      if (mLevel.display_page == 0) show_page(0, xc, bs, by, lev, col); // level icon map
      if (mLevel.display_page == 1) show_page(7, xc, bs, by, lev, col); // not completed general
      if (mLevel.display_page > 1) mLevel.display_page = 0;
   }
   if (status == 1)
   {
      //al_draw_textf(mFont.pixl, mColor.pc[col], xc, y-31, ALLEGRO_ALIGN_CENTER, "Ready!");
      al_draw_textf(mFont.pixl, mColor.pc[15], xc, y+18, ALLEGRO_ALIGN_CENTER, "UP - Start Level");
      al_draw_textf(mFont.pixl, mColor.pc[15], xc, y+24, ALLEGRO_ALIGN_CENTER, "DOWN - Cycle Info");
      if (mLevel.display_page == 0) show_page( 0, xc, bs, by, lev, col); // level icon map
      if (mLevel.display_page == 1) show_page( 7, xc, bs, by, lev, col); // not completed general
      if (mLevel.display_page == 2) show_page(12, xc, bs, by, lev, col); // demo
      if (mLevel.display_page > 2) mLevel.display_page = 0;
   }
   if (status > 1)
   {
      //al_draw_textf(mFont.pixl, mColor.pc[col], xc, y-31, ALLEGRO_ALIGN_CENTER, "Complete!");
      al_draw_textf(mFont.pixl, mColor.pc[15], xc, y+18, ALLEGRO_ALIGN_CENTER, "UP - Start Level");
      al_draw_textf(mFont.pixl, mColor.pc[15], xc, y+24, ALLEGRO_ALIGN_CENTER, "DOWN - Cycle Info");
      if (mLevel.display_page == 0) show_page( 0, xc, bs, by, lev, col); // level icon map
      if (mLevel.display_page == 1) show_page( 1, xc, bs, by, lev, col); // general
      if (mLevel.display_page == 2) show_page( 2, xc, bs, by, lev, col); // times
      if (mLevel.display_page == 3) show_page( 3, xc, bs, by, lev, col); // purple coins
      if (mLevel.display_page == 4) show_page(12, xc, bs, by, lev, col); // demo
      if (mLevel.display_page > 4) mLevel.display_page = 0;
   }
}


int mwItem::draw_gate(int i, int x, int y, int custom)
{
   if (custom) al_draw_bitmap(mBitmap.tile[139], x, y, 0 );
   else
   {
      int xc = x+10; // center of tile
      int lev = item[i][6];

      int col, status;
      char stxt[80];
      mLevel.get_level_status(lev, status, col, stxt);

      al_draw_scaled_bitmap(mBitmap.tile[127+col], 0, 0, 20, 20, x-10, y-20, 40, 40, 0); // draw the gate tile

      draw_framed_text(xc, y-19, mFont.pixl, col, stxt); // draw status text

      if (status == 0) al_draw_scaled_bitmap(mBitmap.tile[366], 0, 0, 20, 20, x-11, y-24, 40, 40, 0); // show lock

      // show icon for purple coin achievement
      if (mLevel.data[lev].max_purple_coins_collected == mLevel.data[lev].tot_purple_coins)
         al_draw_scaled_bitmap(mBitmap.tile[197], 0, 0, 19, 19, x-5, y-10, 12, 12, 0); // show purple coin

      // show icon for par time achievement
      if ((mLevel.data[lev].best_time > 0) && (mLevel.data[lev].best_time < mLevel.data[lev].par_time) && (mLevel.data[lev].completed))
         al_draw_scaled_bitmap(mBitmap.tile[542], 3, 3, 14, 14, x+12, y-11, 14, 14, 0); // show clock

      al_draw_textf(mFont.pr8, mColor.pc[15], xc+30, y, ALLEGRO_ALIGN_CENTER, "%d", lev); // draw the level number (optional, comment out for release)

      // draw level icon 2 with frame
      int ls = mLevel.level_icon2_size;
      int lx = xc-ls/2;  // level icon x1
      int ly = y-130;    // level icon y pos
      al_draw_filled_rectangle(lx, ly, lx+ls+1, ly+ls+1, mColor.pc[0]);      // clear
      al_draw_rectangle(       lx, ly, lx+ls+1, ly+ls+1, mColor.pc[col], 1); // frame
      al_draw_bitmap(mLevel.level_icon2[lev], lx, ly, 0);
      ly+=101;

      // draw and frame the level name
      draw_framed_text(xc, ly, mFont.pixl, col, mLevel.data[lev].level_name);
      ly+=8;
   }


   return 1;
}







