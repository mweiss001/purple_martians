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
#include "mwNetgame.h"


// -----------------------------------------------------------------------
// Start
// -----------------------------------------------------------------------

int mwItem::draw_start(int i, int x, int y, int shape)
{
   // is this start the spawn point for the active local player?
   if (item[i][7] == mPlayer.syn[mPlayer.active_local_player].spawn_point_index) shape = mBitmap.zz[0][21]; // show spinning
   else                                                                          shape = mBitmap.zz[0][11]; // show static
   al_draw_bitmap(mBitmap.tile[shape], x, y, 0);
   if (mLoop.level_editor_running) al_draw_textf(mFont.pixl, mColor.pc[12], x+10, y-4, ALLEGRO_ALIGN_CENTER, "%d", item[i][7]); // put start seq number but only in lev editor
   return 1;
}


void mwItem::proc_start_collision(int p, int i)
{
   int cpi = mPlayer.syn[p].spawn_point_index;  // current index for player
   int npi = item[i][7];                        // new spawn point index

   // count number of starts and put in array indexed by start index
   int ns = 0;
   int s[8] = {0};
   for (int i=0; i<500; i++)
      if (item[i][0] == 5)
      {
         ns++;
         s[item[i][7]] = i; // save index of this start
      }

   if (ns > 1)
   {
      // get these from start with index 0
      int mode = item[s[0]][6];
      int iio  = item[s[0]][8]; // increase index only

      if ((!iio) || ((iio) && (npi > cpi))) // if not iio, or iio and higher index
      {
         if (mode == 2) // check point common
         {
            for (p=0; p<8; p++)
               mPlayer.syn[p].spawn_point_index = item[i][7]; // set new spawn point for all players
         }
         if (mode == 3) // check point individual
         {
            mPlayer.syn[p].spawn_point_index = item[i][7]; // set new spawn point for this player
         }
      }
   }
}


void mwItem::set_player_start_pos(int p)
{
   //printf("set player:%d start pos\n", p);

   int item_to_get_start_from = -1;

   // in overworld level, player starts at the last gate they touched
   if (mLevel.play_level == 1)
   {
      for (int i=0; i<500; i++)
         if ((item[i][0] == 18) && (item[i][6] == mPlayer.syn[p].overworld_last_touched_gate)) item_to_get_start_from = i;


      // if (item_to_get_start_from == -1)
      // {
      //    printf("Player:%d entering overworld, but failed to find gate for level:%d\n", p, mPlayer.syn[p].overworld_last_touched_gate);
      // }

   }

   if (item_to_get_start_from == -1) // start item not found yet....
   {
      // count the starts and put them in an array
      int ns = 0;
      int s[8] = {0};
      for (int i=0; i<500; i++)
         if (item[i][0] == 5)
         {
            ns++;
            s[item[i][7]] = i; // save index of this start
         }

      if (ns == 0) printf("Error: no start found.\n");
      else
      {
         // syn[p].spawn_point_index is saved for every player
         // it is the current index into the array of starts

         // if only one start, that has to be the one we use
         if (ns == 1) mPlayer.syn[p].spawn_point_index = 0;

         // if more than one start....
         if (ns > 1)
         {
            int mode = item[s[0]][6];
            if (mode == 0)
            {
               printf("Lev:%d - Error: in start mode:0 there should be only one start.. all other starts are ignored.\n", mLevel.play_level);
               mPlayer.syn[p].spawn_point_index = 0;
            }
            if (mode == 1) // team start
            {
               if (p % 2) mPlayer.syn[p].spawn_point_index = 1; // odd
               else       mPlayer.syn[p].spawn_point_index = 0; // even
            }
            if ((mode == 2) || (mode == 3)) // check point common and individual
            {
               //if (!cont) syn[p].spawn_point_index = 0; // initial
            }
         }
         // translate spawn_point index into an item we can get a position from
         item_to_get_start_from = s[mPlayer.syn[p].spawn_point_index];
      }
   }
   // if after all that we do not have an item, pick a default place to start the player
   if (item_to_get_start_from == -1)
   {
      mPlayer.syn[p].x = 100;
      mPlayer.syn[p].y = 100;
   }
   // set the player's position from the item position
   else
   {
      mPlayer.syn[p].x = itemf[item_to_get_start_from][0];
      mPlayer.syn[p].y = itemf[item_to_get_start_from][1];
   }
}






// -----------------------------------------------------------------------
// Exit
// -----------------------------------------------------------------------

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

void mwItem::proc_exit_collision(int p, int i)
{
   int exit_enemys_left = mEnemy.num_enemy - item[i][8];
   if (exit_enemys_left <= 0)
   {
      if ((mPlayer.syn[0].level_done_mode == 0) && (!mNetgame.ima_client)) // only trigger from mode 0 and client can never locally exit
      {
         mPlayer.syn[0].level_done_mode = 9;
         mPlayer.syn[0].level_done_timer = 0;
         mPlayer.syn[0].level_done_x = itemf[i][0];
         mPlayer.syn[0].level_done_y = itemf[i][1];
         mPlayer.syn[0].level_done_player = p;
         mPlayer.syn[0].level_done_frame = mLoop.frame_num;

         if (!mMain.classic_mode) mPlayer.syn[0].level_done_next_level = 1;                             // in story mode all exits return to overworld
         else mPlayer.syn[0].level_done_next_level = mLevel.get_next_level(mLevel.play_level, 199, 1);  // otherwise do next chron level
         mGameEvent.add(4, 0, 0, 0, 0, 0, 0);
      }
   }
   else mGameEvent.add(23, 0, 0, p, i, exit_enemys_left, 0); // not enough dead yet
}





















// -----------------------------------------------------------------------
// Gate
// -----------------------------------------------------------------------

void mwItem::proc_gate_collision(int p, int i)
{
   mPlayer.syn[p].marked_gate = i;
   set_gate_level_icon_position(i);
   int lev = item[i][6];

   if (mPlayer.syn[p].overworld_last_touched_gate != lev)
   {
      mPlayer.syn[p].overworld_last_touched_gate = lev;
      if (p == 0) mConfig.save_config(PM_CFG_SAVE_OVERWORLD_GATE_LAST_TOUCHED); // clients do not save to config
   }

    // debug set level complete
   if ((mDemoMode.demo_debug_complete_level_on_gate_with_fire) && (mPlayer.syn[p].fire) && (mLevel.data[lev].status < 2))
   {
      mLevel.add_play_data_record(lev, 2);
      mLevel.load_level(1, 0, 0);
   }

   int status = mLevel.data[lev].status;
   if ((mNetgame.ima_server) || (mNetgame.ima_client))
   {
      // temporarily change all not complete status to complete status
      if (status < 2) status = 2;
   }

   // if UP pressed, enter gate (client can never locally enter gate)
   if ((mPlayer.syn[p].up) && (status > 0) && (!mNetgame.ima_client) )
   {

      // server to set last touched gate for all clients
      if (mNetgame.ima_server)
      {
         for (int pp=1; pp<8; pp++)
            if (mPlayer.syn[pp].active)
            {
               mPlayer.syn[pp].overworld_last_touched_gate = lev;
            }
      }



      // immediate next level to gate level
      mPlayer.syn[0].level_done_mode = 3;
      mPlayer.syn[0].level_done_timer = 0;
      mPlayer.syn[0].level_done_next_level = lev;

      mLoop.quit_action = 2;
      mLoop.done_action = 2;
   }

   // if DOWN pressed, cycle display pages
   if (mPlayer.if_players_ctrl_just_pressed(p, PM_COMPMOVE_DOWN)) item[i][8]++;

   // enforce page limits
   if ((status == 0) && (item[i][8] > 1)) item[i][8] = 0;
   if ((status == 1) && (item[i][8] > 2)) item[i][8] = 0;
   if ((status > 1)  && (item[i][8] > 4)) item[i][8] = 0;

   // no demo page in netgame
   if (((mNetgame.ima_server) || (mNetgame.ima_client)) && (item[i][8] == 4)) item[i][8] = 0;
}


int mwItem::draw_gate(int i, int x, int y, int custom)
{
   if (custom) al_draw_bitmap(mBitmap.tile[139], x, y, 0 );
   else
   {
      int xc = x+10; // center of tile
      int lev = item[i][6];
      int col = mLevel.data[lev].status_color;
      int status = mLevel.data[lev].status;

      char stat_txt[16];
      sprintf(stat_txt, "%s", mLevel.data[lev].status_text);


      // in netgame, temporarily change all not complete status to complete status
      if ((mNetgame.ima_server) || (mNetgame.ima_client))
      {
         if (status < 2)
         {
            status = 2;
            col = 12;
            sprintf(stat_txt, "%s", "Complete");
         }
      }


      al_draw_scaled_bitmap(mBitmap.tile[127+col], 0, 0, 20, 20, x-10, y-20, 40, 40, 0); // draw the gate tile

      mScreen.draw_framed_text(xc, y-19, 1, mFont.pixl, col, 15, stat_txt); // draw status text

      if (status == 0) al_draw_scaled_bitmap(mBitmap.tile[366], 0, 0, 20, 20, x-11, y-24, 40, 40, 0); // show lock

      // show icon for purple coin achievement
      if (mLevel.data[lev].max_purple_coins_collected == mLevel.data[lev].tot_purple_coins)
         al_draw_scaled_bitmap(mBitmap.tile[197], 0, 0, 19, 19, x-5, y-10, 12, 12, 0); // show purple coin

      // show icon for par time achievement
      if ((mLevel.data[lev].time_best_all_coins > 0) && (mLevel.data[lev].time_best_all_coins < mLevel.data[lev].time_par))
         al_draw_scaled_bitmap(mBitmap.tile[542], 3, 3, 14, 14, x+12, y-11, 14, 14, 0); // show clock


      // al_draw_textf(mFont.pr8, mColor.pc[15], xc+30, y, ALLEGRO_ALIGN_CENTER, "%d", lev); // draw the level number (optional, comment out for release)

      // draw level icon with frame
      int ls = 100;
      int lx = xc-ls/2;  // level icon x1
      int ly = y-130;    // level icon y pos
      al_draw_filled_rectangle(lx, ly, lx+ls+1, ly+ls+1, mColor.pc[0]);      // clear
      al_draw_rectangle(       lx, ly, lx+ls+1, ly+ls+1, mColor.pc[col], 1); // frame

      if (mLevel.level_icon_100[lev]) al_draw_bitmap(mLevel.level_icon_100[lev], lx, ly, 0);

      ly+=101;

      // draw and frame the level name
      mScreen.draw_framed_text(xc, ly, 1, mFont.pixl, col, 15, mLevel.data[lev].level_name);
      ly+=8;
   }
   return 1;
}


void mwItem::draw_gate_info(int i)
{
   int x = item[i][4];
   int y = item[i][5];
   int xc = x + 10; // center of tile
   int lev = item[i][6];

   int bs = 200; // level icon size
   int by = y+35; // info y start pos

   if (by > 1900) by = 1721; // special case for gates on bottom rom to show info above instead


   int status = mLevel.data[lev].status;
   int col = mLevel.data[lev].status_color;

   if ((mNetgame.ima_server) || (mNetgame.ima_client))
   {
      // temporarily change all not complete status to complete status
      if (status < 2)
      {
         status = 2;
         col = 12;
      }
   }

   mScreen.draw_framed_text(xc, by, 1, mFont.pr8, col, 15, mLevel.data[lev].level_name); // draw and frame the level name

   if (status == 0)
   {
      al_draw_textf(mFont.pixl, mColor.pc[15], xc, y+18, ALLEGRO_ALIGN_CENTER, "DOWN - Cycle Info");
      if (item[i][8] == 0) show_page(0, xc, bs, by, lev, col); // level icon map
      if (item[i][8] == 1) show_page(7, xc, bs, by, lev, col); // not completed general
   }
   if (status == 1)
   {
      al_draw_textf(mFont.pixl, mColor.pc[15], xc, y+18, ALLEGRO_ALIGN_CENTER, "UP - Start Level");
      al_draw_textf(mFont.pixl, mColor.pc[15], xc, y+24, ALLEGRO_ALIGN_CENTER, "DOWN - Cycle Info");
      if (item[i][8] == 0) show_page( 0, xc, bs, by, lev, col); // level icon map
      if (item[i][8] == 1) show_page( 7, xc, bs, by, lev, col); // not completed general
      if (item[i][8] == 2) show_page(12, xc, bs, by, lev, col); // demo
   }
   if (status > 1)
   {
      al_draw_textf(mFont.pixl, mColor.pc[15], xc, y+18, ALLEGRO_ALIGN_CENTER, "UP - Start Level");
      al_draw_textf(mFont.pixl, mColor.pc[15], xc, y+24, ALLEGRO_ALIGN_CENTER, "DOWN - Cycle Info");
      if (item[i][8] == 0) show_page( 0, xc, bs, by, lev, col); // level icon map
      if (item[i][8] == 1) show_page( 1, xc, bs, by, lev, col); // general
      if (item[i][8] == 2) show_page( 2, xc, bs, by, lev, col); // times
      if (item[i][8] == 3) show_page( 3, xc, bs, by, lev, col); // purple coins
      if (item[i][8] == 4) show_page(12, xc, bs, by, lev, col); // demo
   }
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
   mScreen.gate_transition_x = (x - mScreen.level_display_region_x) * mDisplay.scale_factor_current + mScreen.screen_display_x;
   mScreen.gate_transition_y = (y - mScreen.level_display_region_y) * mDisplay.scale_factor_current + mScreen.screen_display_y;
   mScreen.gate_transition_wx = mScreen.level_display_region_x;
   mScreen.gate_transition_wy = mScreen.level_display_region_y;
   mScreen.gate_transition_scale = mDisplay.scale_factor_current;
}


// puts left justified txt1, right justified txt2, and ... in between
void mwItem::draw_line(int x1, int x2, int y, const char * txt1, const char * txt2, int col)
{
   // make my own copy of txt1 to append "."
   char lt[200];
   strcpy(lt, txt1);

   int mtl = (x2-x1)/8; // max text length
   int tl = strlen(txt1);
   int bs = mtl-tl; // blank spaces to fill
   for (int i=0; i<bs; i++) strcat(lt, ".");

   al_draw_text(mFont.pr8, mColor.pc[15], x1+1, y, 0, lt);

   tl = strlen(txt2)*8;
   al_draw_filled_rectangle(x2-tl-4, y, x2, y+8, mColor.pc[0]);

   al_draw_text(mFont.pr8, mColor.pc[col], x2-tl-1, y, 0, txt2);
}




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




char * mwItem::chrd(int v, char* ft)
{
   sprintf(ft, "%d", v);
   return ft;
}

char * mwItem::chrd(int v1, int v2, char* ft)
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

   float yp = by+11.5; // top of frame

   if (page == 0) // level icon map
   {
      al_draw_rectangle(bx, yp, bx+bs+1, by+bs+13, mColor.pc[col], 1);
      al_draw_bitmap(mLevel.level_icon_200[lev], bx, by+12, 0);
   }

   if (page == 1) // general
   {
      int yb = yp+(6*yi)+25;
      al_draw_filled_rectangle(bx, yp, bx2+1, yb, mColor.pc[0]);
      al_draw_rectangle(bx, yp, bx2+1, yb, mColor.pc[col], 1);
      al_draw_bitmap(mBitmap.tile[127+col], bx+1, (int)yp+2, 0);
      al_draw_textf(mFont.pr8, mColor.pc[15], bx+24, (int)yp+8, 0, "General");
      al_draw_line(bx, yp+23, bx2+1, yp+23, mColor.pc[col], 1);
      yp+=25;
      draw_line(bx, bx2, (int)yp, "Min Time Overall",  chrms(mLevel.data[lev].time_best,           msg), 15); yp+=yi;
      draw_line(bx, bx2, (int)yp, "Min Time w/coins",  chrms(mLevel.data[lev].time_best_all_coins, msg), 15); yp+=yi;
      draw_line(bx, bx2, (int)yp, "Min Player Deaths",  chrd(mLevel.data[lev].min_respawns,         msg), 15); yp+=yi;
      draw_line(bx, bx2, (int)yp, "Max Purple Coins",   chrd(mLevel.data[lev].max_purple_coins_collected, mLevel.data[lev].tot_purple_coins, msg), 15); yp+=yi;
      draw_line(bx, bx2, (int)yp, "Max Enemies Killed", chrd(mLevel.data[lev].max_enemies_killed,   msg), 15); yp+=yi;
      draw_line(bx, bx2, (int)yp, "Min Enemies Left",   chrd(mLevel.data[lev].min_enemies_left,     msg), 15); yp+=yi;
   }

   if (page == 2) // times
   {
      int yb = yp+(9*yi)+25;
      al_draw_filled_rectangle(bx, yp, bx2+1, yb, mColor.pc[0]);
      al_draw_rectangle(bx, yp, bx2+1, yb, mColor.pc[col], 1);
      al_draw_bitmap(mBitmap.tile[542], bx+1, (int)yp+2, 0); // show clock
      al_draw_textf(mFont.pr8, mColor.pc[15], bx+24, (int)yp+8, 0, "Time to Complete");
      al_draw_line(bx, yp+23, bx2+1, yp+23, mColor.pc[col], 1);
      yp+=25;
      draw_line(bx, bx2, (int)yp, "Par Time",          chrms(mLevel.data[lev].time_par,            msg), 15); yp+=yi;
      draw_line(bx, bx2, (int)yp, "Min Time Overall",  chrms(mLevel.data[lev].time_best,           msg), 15); yp+=yi;
      draw_line(bx, bx2, (int)yp, "Min Time w/coins",  chrms(mLevel.data[lev].time_best_all_coins, msg), 15); yp+=yi;
      draw_line(bx, bx2, (int)yp, "Max Time",          chrms(mLevel.data[lev].time_worst,          msg), 15); yp+=yi;
      draw_line(bx, bx2, (int)yp, "Avg Time",          chrms(mLevel.data[lev].time_average,        msg), 15); yp+=yi;
      draw_line(bx, bx2, (int)yp, "Total Play Time",   chrms(mLevel.data[lev].time_total,          msg), 15); yp+=yi;
      draw_line(bx, bx2, (int)yp, "Times Played",      chrd(mLevel.data[lev].times_played,         msg), 15); yp+=yi;
      draw_line(bx, bx2, (int)yp, "Times Completed",   chrd(mLevel.data[lev].times_beat,           msg), 15); yp+=yi;
      draw_line(bx, bx2, (int)yp, "Times Quit",        chrd(mLevel.data[lev].times_quit,           msg), 15); yp+=yi;
   }

   if (page == 3) // purple coins
   {
      int yb = yp+(2*yi)+25;
      al_draw_filled_rectangle(bx, yp, bx2+1, yb, mColor.pc[0]);
      al_draw_rectangle(bx, yp, bx2+1, yb, mColor.pc[col], 1);
      al_draw_bitmap(mBitmap.tile[197], bx+1, (int)yp+2, 0); // show purple coin
      al_draw_textf(mFont.pr8, mColor.pc[15], bx+24, (int)yp+8, 0, "Purple Coins");
      al_draw_line(bx, yp+23, bx2+1, yp+23, mColor.pc[col], 1);
      yp+=25;
      draw_line(bx, bx2, (int)yp, "Collected", chrd(mLevel.data[lev].max_purple_coins_collected, msg), 15); yp+=yi;
      draw_line(bx, bx2, (int)yp, "Total",     chrd(mLevel.data[lev].tot_purple_coins,           msg), 15); yp+=yi;
   }

   if (page == 7) // not completed general
   {
      int yb = yp+(3*yi)+25;
      al_draw_filled_rectangle(bx, yp, bx2+1, yb, mColor.pc[0]);
      al_draw_rectangle(bx, yp, bx2+1, yb, mColor.pc[col], 1);
      al_draw_bitmap(mBitmap.tile[127+col], bx+1, (int)yp+2, 0);
      al_draw_textf(mFont.pr8, mColor.pc[15], bx+28, (int) yp+8, 0, "General");
      al_draw_line(bx, yp+23, bx2+1, yp+23, mColor.pc[col], 1);
      yp+=25;
      draw_line(bx, bx2, (int)yp, "Par Time",     chrms(mLevel.data[lev].time_par,        msg), 15); yp+=yi;
      draw_line(bx, bx2, (int)yp, "Purple Coins", chrd(mLevel.data[lev].tot_purple_coins, msg), 15); yp+=yi;
      draw_line(bx, bx2, (int)yp, "Times Played", chrd(mLevel.data[lev].times_played,     msg), 15); yp+=yi;
   }

   if (page == 12) // run demo for level
   {

      if ((mLevel.data[lev].times_beat == 0) && (mLevel.data[lev].times_played > 2)) // if not beat and started > 2 time
      {
         int yb = yp+(4*yi)+25;
         al_draw_filled_rectangle(bx, yp, bx2+1, yb, mColor.pc[0]);
         al_draw_rectangle(bx, yp, bx2+1, yb, mColor.pc[col], 1);
         al_draw_bitmap(mBitmap.tile[401], bx+1, (int)yp+2, 0); // show player
         al_draw_textf(mFont.pr8, mColor.pc[15], bx+28, (int)yp+8, 0, "Run Demo");
         al_draw_line(bx, yp+23, bx2+1, yp+23, mColor.pc[col], 1);
         yp+=25;
         al_draw_textf(mFont.pr8, mColor.pc[15], xc,(int)yp, ALLEGRO_ALIGN_CENTER, "type 'demo' to run demo"); yp+=yi*2;
         al_draw_textf(mFont.pr8, mColor.pc[15], xc,(int)yp, ALLEGRO_ALIGN_CENTER, "type 'cheat' to let demo"); yp+=yi;
         al_draw_textf(mFont.pr8, mColor.pc[15], xc,(int)yp, ALLEGRO_ALIGN_CENTER, "complete level for you"); yp+=yi;

         if (mLevel.skc_trigger_demo)
         {
            mLevel.skc_trigger_demo = 0;
            mDemoMode.run_single_from_gate(lev);
         }
      }
      else
      {
         int yb = yp+(1*yi)+25;
         al_draw_filled_rectangle(bx, yp, bx2+1, yb, mColor.pc[0]);
         al_draw_rectangle(bx, yp, bx2+1, yb, mColor.pc[col], 1);
         al_draw_bitmap(mBitmap.tile[401], bx+1, (int)yp+2, 0); // show player
         al_draw_textf(mFont.pr8, mColor.pc[15], bx+28, (int)yp+8, 0, "Run Demo");
         al_draw_line(bx, yp+23, bx2+1, yp+23, mColor.pc[col], 1);
         yp+=25;
         al_draw_textf(mFont.pr8, mColor.pc[15], xc,(int)yp, ALLEGRO_ALIGN_CENTER, "type 'demo' to run demo"); yp+=yi;

         if (mLevel.skc_trigger_demo)
         {
            mLevel.skc_trigger_demo = 0;
            if (mLevel.skc_trigger_demo_cheat) mLevel.skc_trigger_demo_cheat = 0; // do nothing if trying to cheat here
            else mDemoMode.run_single_from_gate(lev);
         }

      }
   }
}

