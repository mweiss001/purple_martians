// mwLevelData.cpp

#include "pm.h"
#include "mwLevel.h"
#include "mwConfig.h"
#include "mwLift.h"
#include "mwEnemy.h"
#include "mwScreen.h"
#include "mwFont.h"
#include "mwInput.h"
#include "mwBitmap.h"
#include "mwColor.h"
#include "mwMiscFnx.h"
#include "mwDisplay.h"
#include "mwLoop.h"
#include "mwPlayer.h"
#include "mwVisualLevel.h"
#include "mwDemoMode.h"

void mwLevel::show_level_stats_row(int i, int x1, int x2, int draw, int &max_x, int &y, int vline[], int &vli, int tally[][16], int msg_type)
{
   char msg[80];

   int x = x1;
   vli = 0;

   int show_purple_coins = 1;
   int show_best_time = 1;
   int show_coin_time = 1;
   int show_par_time = 1;
   int show_min_deaths = 1;
   int show_max_kills = 1;
   int show_times_played = 1;
   int show_time_played = 1;
   int show_worst_time = 1;
   int show_average_time = 1;
   int show_times_beat = 1;
   int show_times_quit = 1;

   if (msg_type > 2) // overworld
   {
      show_min_deaths = 0;
      show_max_kills = 0;
      show_times_played = 0;
      show_time_played = 0;
      show_worst_time = 0;
      show_average_time = 0;
      show_times_beat = 0;
      show_times_quit = 0;

   }

   int lev = 0;
   int header = 0;
   int total = 0;

   if (i == -1) // header
   {
      header = 1;
      lev = -1;
   }

   if (i == 100) // totals
   {
      total = 1;
      lev = -1;
   }

   vline[vli++] = x; // first vline
   x += 8; // first text spacer


   // set level and area
   if ((i > -1) && (i < 100))
      if (area_array[i][1]) lev = area_array[i][0];


   // level name
   int width = 20*8;
   if (draw)
   {
      if (header)     al_draw_text( mFont.pr8, mColor.pc[15], x, y+4, 0, "Level");
      else if (total) al_draw_text( mFont.pr8, mColor.pc[15], x, y,   0, "Totals");
      else if (lev>0) al_draw_textf(mFont.pr8, mColor.pc[15], x, y,   0, "%s", data[lev].level_name);
      //else if (lev>0) al_draw_textf(mFont.pr8, mColor.pc[15], x, y,   0, "%2d %s", lev, data[lev].level_name); // prepend level number
   }
   x += width + 8;
   vline[vli++] = x-4;

   int tc = 0; // number of times played
   int tb = 0; // number of times beat
   int tq = 0; // number of times quit

   int tta = 0; // total time spent playing
   int ttb = 0; // total time spent playing only if level completed

   int wt = 0; // worst time
   int mt = 0; // average time


   int status = mLevel.data[lev].status;
   int col = mLevel.data[lev].status_color;



   if (lev != -1)
   {
      tally[0][0]++; // num levels

      if ((status == 2) || (status == 3)) tally[0][1]++; // complete
      if (status == 3) tally[0][2]++; // perfect

      for (int i=0; i<play_data_num; i++)
         if (play_data[i].level == lev)
         {

            tc++; // all times played

            tta += play_data[i].timer;
            if (play_data[i].timer > wt) wt = play_data[i].timer;

            if (play_data[i].completed)
            {
               tb++;
               ttb += play_data[i].timer;
            }
            else tq++;
         }

      if (tb) mt = ttb/tb; // average time (only if level beat)

      tally[0][3] += tc; // plays
      tally[0][11] += tb; // beat
      tally[0][12] += tq; // quit
      tally[0][6] += data[lev].min_respawns;
      tally[0][7] += data[lev].max_enemies_killed;
      tally[0][4] += data[lev].tot_purple_coins;
      tally[0][5] += data[lev].max_purple_coins_collected;
      tally[0][8] += data[lev].time_par;
      tally[0][9] += data[lev].time_best;
      tally[0][13] += data[lev].time_best_all_coins;
      tally[0][10] += tta; // total play time all
   }



   // level status
   width = 9*8;
   if (draw)
   {
      char stxt[16];
      if (header) al_draw_text( mFont.pr8, mColor.pc[ 15], x+width/2, y+4, ALLEGRO_ALIGN_CENTER, "Status");
      else if (total)
      {
         sprintf(stxt, "-"); col = 15;
         if (tally[0][1] == tally[0][0]) { sprintf(stxt, "Complete"); col = 12; }
         if (tally[0][2] == tally[0][0]) { sprintf(stxt, "Perfect"); col = 8; }
         al_draw_text(mFont.pr8, mColor.pc[col],  x+width/2, y,   ALLEGRO_ALIGN_CENTER, stxt);
      }
      else al_draw_text(mFont.pr8, mColor.pc[col], x+width/2, y,   ALLEGRO_ALIGN_CENTER, mLevel.data[lev].status_text);
   }
   x += width + 8;
   vline[vli++] = x-4;




   if (show_purple_coins)
   {
      width = 7*8;
      if (draw)
      {
         if (header)
         {
            al_draw_text(mFont.pr8, mColor.pc[15], x+width/2, y,   ALLEGRO_ALIGN_CENTER, "Purple");
            al_draw_text(mFont.pr8, mColor.pc[15], x+width/2, y+8, ALLEGRO_ALIGN_CENTER, "Coins");
         }
         else if (total)
         {
            al_draw_textf(mFont.pr8, mColor.pc[15], x+width/2, y, ALLEGRO_ALIGN_CENTER, "%d/%d", tally[0][5], tally[0][4]);
         }
         else
         {
            int pc_col = 15;
            if (status == 3) pc_col = 8;
            al_draw_textf(mFont.pr8, mColor.pc[pc_col], x+width/2, y, ALLEGRO_ALIGN_CENTER, "%d/%d", data[lev].max_purple_coins_collected, data[lev].tot_purple_coins);
         }
      }
      x += width + 8;
      vline[vli++] = x-4;
   }


   if (show_best_time)
   {
      width = 10 * 8;
      if (draw)
      {
         if (header)
         {
//            al_draw_text(mFont.pr8, mColor.pc[15], x+width/2, y,   ALLEGRO_ALIGN_CENTER, "Best");
//            al_draw_text(mFont.pr8, mColor.pc[15], x+width/2, y+8, ALLEGRO_ALIGN_CENTER, "Time");
            al_draw_text(mFont.pr8, mColor.pc[15], x+width/2, y,   ALLEGRO_ALIGN_CENTER, "Best Time");
            al_draw_text(mFont.pr8, mColor.pc[15], x+width/2, y+8, ALLEGRO_ALIGN_CENTER, "Overall");


         }
         else if (total) al_draw_text(mFont.pr8, mColor.pc[15], x+width/2, y, ALLEGRO_ALIGN_CENTER, mMiscFnx.chrms(tally[0][9], msg));
         else
         {
            if ((data[lev].time_best == 0) || (data[lev].status < 2))
            {
               al_draw_text(mFont.pr8, mColor.pc[15], x+width/2, y, ALLEGRO_ALIGN_CENTER, "n/a");
            }
            else
            {
               al_draw_text(mFont.pr8, mColor.pc[15], x+width/2, y, ALLEGRO_ALIGN_CENTER, mMiscFnx.chrms(data[lev].time_best, msg));
            }
         }
      }
      x += width + 8;
      vline[vli++] = x-4;
   }

   if (show_coin_time)
   {
      width = 10 * 8;
      if (draw)
      {
         if (header)
         {
//            al_draw_text(mFont.pr8, mColor.pc[15], x+width/2, y,   ALLEGRO_ALIGN_CENTER, "Coin");
//            al_draw_text(mFont.pr8, mColor.pc[15], x+width/2, y+8, ALLEGRO_ALIGN_CENTER, "Time");
            al_draw_text(mFont.pr8, mColor.pc[15], x+width/2, y,   ALLEGRO_ALIGN_CENTER, "Best Time");
            al_draw_text(mFont.pr8, mColor.pc[15], x+width/2, y+8, ALLEGRO_ALIGN_CENTER, "All Coins");
         }
         else if (total) al_draw_text(mFont.pr8, mColor.pc[15], x+width/2, y, ALLEGRO_ALIGN_CENTER, mMiscFnx.chrms(tally[0][13], msg));
         else
         {
            if ((data[lev].time_best_all_coins == 0) || (data[lev].status < 2))
            {
               al_draw_text(mFont.pr8, mColor.pc[15], x+width/2, y, ALLEGRO_ALIGN_CENTER, "n/a");
            }
            else
            {
               int tc = 15;
               if (data[lev].time_best_all_coins < data[lev].time_par) tc = 8;
               al_draw_text(mFont.pr8, mColor.pc[tc], x+width/2, y, ALLEGRO_ALIGN_CENTER, mMiscFnx.chrms(data[lev].time_best_all_coins, msg));
            }
         }
      }
      x += width + 8;
      vline[vli++] = x-4;
   }





   if (show_par_time)
   {
      width = 7 * 8;
      if (draw)
      {
         if (header)
         {
            al_draw_text(mFont.pr8, mColor.pc[15], x+width/2, y,   ALLEGRO_ALIGN_CENTER, "Par");
            al_draw_text(mFont.pr8, mColor.pc[15], x+width/2, y+8, ALLEGRO_ALIGN_CENTER, "Time");
         }
         else if (total) al_draw_text(mFont.pr8, mColor.pc[15], x+width/2, y, ALLEGRO_ALIGN_CENTER, mMiscFnx.chrms(tally[0][8], msg));
         else            al_draw_text(mFont.pr8, mColor.pc[15], x+width/2, y, ALLEGRO_ALIGN_CENTER, mMiscFnx.chrms(data[lev].time_par, msg));
      }
      x += width + 8;
      vline[vli++] = x-4;
   }

   if (show_min_deaths)
   {
      width = 6 * 8;
      if (draw)
      {
         if (header)
         {
            al_draw_text(mFont.pr8, mColor.pc[15], x+width/2, y,   ALLEGRO_ALIGN_CENTER, "Min");
            al_draw_text(mFont.pr8, mColor.pc[15], x+width/2, y+8, ALLEGRO_ALIGN_CENTER, "Deaths");
         }
         else if (total) al_draw_textf(mFont.pr8, mColor.pc[15], x+width/2, y, ALLEGRO_ALIGN_CENTER, "%d", tally[0][6]);
         else            al_draw_textf(mFont.pr8, mColor.pc[15], x+width/2, y, ALLEGRO_ALIGN_CENTER, "%d", data[lev].min_respawns);
      }
      x += width + 8;
      vline[vli++] = x-4;
   }


   if (show_max_kills)
   {
      width = 7 * 8;
      if (draw)
      {
         if (header)
         {
            al_draw_text(mFont.pr8, mColor.pc[15], x+width/2, y,   ALLEGRO_ALIGN_CENTER, "Max");
            al_draw_text(mFont.pr8, mColor.pc[15], x+width/2, y+8, ALLEGRO_ALIGN_CENTER, "Kills");
         }
         else if (total) al_draw_textf(mFont.pr8, mColor.pc[15], x+width/2, y, ALLEGRO_ALIGN_CENTER, "%d", tally[0][7]);
         else            al_draw_textf(mFont.pr8, mColor.pc[15], x+width/2, y, ALLEGRO_ALIGN_CENTER, "%d", data[lev].max_enemies_killed);
      }
      x += width+8;
      vline[vli++] = x-4;
   }

   if (show_times_played)
   {
      width = 5 * 8;
      if (draw)
      {
         if (header)
         {
            al_draw_text(mFont.pr8, mColor.pc[15], x+width/2, y,   ALLEGRO_ALIGN_CENTER, "Total");
            al_draw_text(mFont.pr8, mColor.pc[15], x+width/2, y+8, ALLEGRO_ALIGN_CENTER, "Plays");
         }
         else if (total) al_draw_textf(mFont.pr8, mColor.pc[15], x+width/2, y,   ALLEGRO_ALIGN_CENTER, "%d", tally[0][3]);
         else            al_draw_textf(mFont.pr8, mColor.pc[15], x+width/2, y,   ALLEGRO_ALIGN_CENTER, "%d", tc);
      }
      x += width+8;
      vline[vli++] = x-4;
   }



   if (show_times_beat)
   {
      width = 5 * 8;
      if (draw)
      {
         if (header)
         {
            al_draw_text(mFont.pr8, mColor.pc[15], x+width/2, y,   ALLEGRO_ALIGN_CENTER, "Total");
            al_draw_text(mFont.pr8, mColor.pc[15], x+width/2, y+8, ALLEGRO_ALIGN_CENTER, "Beat");
         }
         else if (total) al_draw_textf(mFont.pr8, mColor.pc[15], x+width/2, y,   ALLEGRO_ALIGN_CENTER, "%d", tally[0][11]);
         else            al_draw_textf(mFont.pr8, mColor.pc[15], x+width/2, y,   ALLEGRO_ALIGN_CENTER, "%d", tb);
      }
      x += width+8;
      vline[vli++] = x-4;
   }

   if (show_times_quit)
   {
      width = 5 * 8;
      if (draw)
      {
         if (header)
         {
            al_draw_text(mFont.pr8, mColor.pc[15], x+width/2, y,   ALLEGRO_ALIGN_CENTER, "Total");
            al_draw_text(mFont.pr8, mColor.pc[15], x+width/2, y+8, ALLEGRO_ALIGN_CENTER, "Quit");
         }
         else if (total) al_draw_textf(mFont.pr8, mColor.pc[15], x+width/2, y,   ALLEGRO_ALIGN_CENTER, "%d", tally[0][12]);
         else            al_draw_textf(mFont.pr8, mColor.pc[15], x+width/2, y,   ALLEGRO_ALIGN_CENTER, "%d", tq);
      }
      x += width+8;
      vline[vli++] = x-4;
   }

   if (show_time_played)
   {
      width = 9 * 8;
      if (draw)
      {
         if (header)
         {
            al_draw_text(mFont.pr8, mColor.pc[15], x+width/2, y,   ALLEGRO_ALIGN_CENTER, "Total");
            al_draw_text(mFont.pr8, mColor.pc[15], x+width/2, y+8, ALLEGRO_ALIGN_CENTER, "Time");
         }
         else if (total) al_draw_text(mFont.pr8, mColor.pc[15], x+width/2, y, ALLEGRO_ALIGN_CENTER, mMiscFnx.chrms(tally[0][10], msg));
         else al_draw_text(mFont.pr8, mColor.pc[15], x+width/2, y, ALLEGRO_ALIGN_CENTER, mMiscFnx.chrms(tta, msg));
      }
      x += width+8;
      vline[vli++] = x-4;
   }


   if (show_worst_time)
   {
      width = 6 * 8;
      if (draw)
      {
         if (header)
         {
            al_draw_text(mFont.pr8, mColor.pc[15], x+width/2, y,   ALLEGRO_ALIGN_CENTER, "Worst");
            al_draw_text(mFont.pr8, mColor.pc[15], x+width/2, y+8, ALLEGRO_ALIGN_CENTER, "Time");
         }
         else if (!total) al_draw_text(mFont.pr8, mColor.pc[15], x+width/2, y, ALLEGRO_ALIGN_CENTER, mMiscFnx.chrms(wt, msg));
      }
      x += width+8;
      vline[vli++] = x-4;
   }

   if (show_average_time)
   {
      width = 7 * 8;
      if (draw)
      {
         if (header)
         {
            al_draw_text(mFont.pr8, mColor.pc[15], x+width/2, y,   ALLEGRO_ALIGN_CENTER, "Average");
            al_draw_text(mFont.pr8, mColor.pc[15], x+width/2, y+8, ALLEGRO_ALIGN_CENTER, "Time");
         }
         else if (!total) al_draw_text(mFont.pr8, mColor.pc[15], x+width/2, y, ALLEGRO_ALIGN_CENTER, mMiscFnx.chrms(mt, msg));
      }
      x += width+8;
      vline[vli++] = x-4;
   }

   if (x > max_x) max_x = x; // max line length

   y+=8; // next line
   if (i == -1) y+=8; // extra line for column header
}

void mwLevel::show_level_stats_title_and_header(int x1, int x2, int& gy, int& ty, int& max_x, int& y, int draw, int vline[], int &vli, int tally[][16], int col, const char* title, int msg_type)
{
   int ys = 16;
   ty = y + (ys-8) / 2; // save text y pos so we can add more text here later

   if (draw)
   {
      al_draw_rectangle(x1, y, x2, y+ys, mColor.pc[col], 1);            // frame
      al_draw_text(mFont.pr8, mColor.pc[15], x1+8, ty, 0, title);

      al_draw_line(x1+172, y, x1+172, y+ys, mColor.pc[col], 1);         // vlines for complete and perfect
      al_draw_line(x1+316, y, x1+316, y+ys, mColor.pc[col], 1);
   }

   gy = y += ys;

   // column header
   if (draw) al_draw_line(x1, y, x2, y, mColor.pc[col], 1);
   y+=2;
   show_level_stats_row(-1, x1, x2, draw, max_x, y, vline, vli, tally, msg_type);
   y+=2;
}


void mwLevel::show_level_stats_totals(int x1, int x2, int gy, int ty, int& max_x, int& y, int draw, int vline[], int &vli, int tally[][16], int col, int msg_type)
{
   show_level_stats_row(100, x1, x2, draw, max_x, y, vline, vli, tally, msg_type);
   y+=4;
   if (draw) al_draw_line(x1, y, x2, y, mColor.pc[col], 1);

   if (draw)
   {
      float lc = 100*(float)tally[0][1]/(float)tally[0][0];
      float lp = 100*(float)tally[0][2]/(float)tally[0][0];
      al_draw_textf(mFont.pr8, mColor.pc[15], x1+180, ty,   0, "Complete:%2.1f%%", lc);
      al_draw_textf(mFont.pr8, mColor.pc[15], x1+324, ty,   0, "Perfect:%2.1f%%", lp);
   }

   // draw vlines
   if (draw)
      for (int i=0; i<20; i++)
         if (vline[i]) al_draw_line(vline[i], gy, vline[i], y, mColor.pc[col], 1);

   y+=8;

   for (int i=0; i<16; i++)
   {
      tally[1][i] += tally[0][i];
      tally[0][i] = 0;
   }
}

void mwLevel::show_level_stats_outline_with_thicker_lines(int x1, int x2, int gy, int y, int draw, ALLEGRO_COLOR c)
{
   if (draw)
   {
      al_draw_rounded_rectangle(x1, gy-16, x2, y-8, 1, 1, c, 3);
      al_draw_line(x1, gy, x2, gy, c, 2);
      al_draw_line(x1, gy+20, x2, gy+20, c, 2);
      al_draw_line(x1, y-24, x2, y-24, c, 2);
   }
}



void mwLevel::draw_level_stats(int xc, int y1, int msg_type)
{
   if (level_stats_bmp_msg_type != msg_type)
   {
      // get w and h
      show_level_stats(level_stats_bmp_w, level_stats_bmp_h, 0, msg_type);

      // recreate bitmap
      al_destroy_bitmap(level_stats_bitmap);
      level_stats_bitmap = al_create_bitmap(level_stats_bmp_w, level_stats_bmp_h);
      al_set_target_bitmap(level_stats_bitmap);
      al_clear_to_color(mColor.pc[0]);

      // draw
      show_level_stats(level_stats_bmp_w, level_stats_bmp_h, 1, msg_type);

      level_stats_bmp_msg_type = msg_type;
   }

   if (level_stats_bmp_msg_type == msg_type)
   {
      if (msg_type == 1) al_set_target_backbuffer(mDisplay.display);
      if (msg_type == 3)  al_set_target_bitmap(mBitmap.level_buffer);
      al_draw_bitmap(level_stats_bitmap, xc-level_stats_bmp_w/2, y1, 0);
   }
}



void mwLevel::show_level_stats(int &w, int &h, int draw, int msg_type)
{
   int x1 = 2;   // these are based on a frame width of 2
   int x2 = w-2;
   int y = 2;

   int ty = 0; // frame and title set ty, then later totals draw text there
   int gy = y; // keeps track of the top of section for drawing frames later

   int vline[20] = { 0 };
   int vli = 0;
   int max_x = 0;
   int tally[2][16] = { 0 };

   //0 grp
   //1 tot
   //0  tot_lev
   //1  tot_lev_complete
   //2  tot_lev_perfect
   //3  tot_plays
   //4  tot_pc
   //5  tot_pcc
   //6  tot_min_deaths
   //7  tot_max_kills
   //8  tot_par_time
   //9  tot_best_time
   //10 tot_play_time
   //11 tot_plays_beat
   //12 tot_plays_quit
   //13  tot_best_time_all_coins

   int show_training = 1;
   int show_main = 1;
   int show_extra = 1;
   int show_demo = 1;
   int show_grand_totals = 1;

   if (msg_type == 1) // all
   {
      show_training = 1;
      show_main = 1;
      show_extra = 1;
      show_demo = 1;
      show_grand_totals = 1;
   }

   if (msg_type == 2) // main only
   {
      show_training = 0;
      show_main = 1;
      show_extra = 0;
      show_demo = 0;
      show_grand_totals = 0;
   }

   if (msg_type == 3) // overworld all
   {
      // by default show only training
      show_training = 1;
      show_main = 0;
      show_extra = 0;
      show_demo = 0;
      show_grand_totals = 0;

      if (!area_locks[1]) // unlocked between training levels and area 1,2
      {
         show_main = 1;
      }

      if (!area_locks[10]) // unlocked past main game
      {
         show_extra = 1;
         show_demo = 1;
         show_grand_totals = 1;
      }
   }

   if (msg_type == 4) // overworld train
   {
      show_training = 1;
      show_main = 0;
      show_extra = 0;
      show_demo = 0;
      show_grand_totals = 0;
   }

   if (msg_type == 5) // overworld main
   {
      show_training = 0;
      show_main = 1;
      show_extra = 0;
      show_demo = 0;
      show_grand_totals = 0;
   }

   int bco = 224; // + 16 * mLoop.pct_y; // background color offset

   // --------------------------------------
   // training levels
   // --------------------------------------
   if (show_training)
   {
      int ac = 11; // area color
      if (draw) al_draw_filled_rectangle(x1, y, x2, y+123, mColor.pc[ac+bco]); // background
      show_level_stats_title_and_header(x1, x2, gy, ty, max_x, y, draw, vline, vli, tally, ac, "Training Levels", msg_type);

      // training levels (13 and 14)
      if (draw) al_draw_line(x1, y, x2, y, mColor.pc[ac], 1);
      y+=4;
      for(int i=0; i<100; i++)
         if ((area_array[i][1] == 13) || (area_array[i][1] == 14))
            show_level_stats_row(i, x1, x2, draw, max_x, y, vline, vli, tally, msg_type);

      y+=4;
      if (draw) al_draw_line(x1, y, x2, y, mColor.pc[ac], 1);
      y+=4;
      show_level_stats_totals(x1, x2, gy, ty, max_x, y, draw, vline, vli, tally, ac, msg_type);
      show_level_stats_outline_with_thicker_lines(x1, x2, gy, y, draw, mColor.pc[ac]);

      if (show_main) y-=5; // no space betwen sections
   }

   // --------------------------------------
   // main game
   // --------------------------------------
   if (show_main)
   {
      int ac = 4; // area color

      int y_size = 52; // base size
      for(int a=1; a<10; a++)
         if ((!area_locks[a]) || (msg_type == 1))
         {
            for(int i=0; i<100; i++)
               if (area_array[i][1] == a) y_size+=8;
            y_size+=4;
         }
      if (draw) al_draw_filled_rectangle(x1, y, x2, y+y_size, mColor.pc[ac+bco]); // background

      show_level_stats_title_and_header(x1, x2, gy, ty, max_x, y, draw, vline, vli, tally, ac, "Main Game", msg_type);

      // levels
      if (draw) al_draw_line(x1, y, x2, y, mColor.pc[ac], 1);
      y+=4;
      for(int a=1; a<10; a++)
         if ((!area_locks[a]) || (msg_type == 1))
         {
            for(int i=0; i<100; i++)
               if (area_array[i][1] == a)
                  show_level_stats_row(i, x1, x2, draw, max_x, y, vline, vli, tally, msg_type);
            y+=2;
            if (draw) al_draw_line(x1, y, x2, y, mColor.pc[ac], 1);
            y+=2;
         }
      y+=2;
      show_level_stats_totals(x1, x2, gy, ty, max_x, y, draw, vline, vli, tally, ac, msg_type);
      show_level_stats_outline_with_thicker_lines(x1, x2, gy, y, draw, mColor.pc[ac]);
      if (show_extra) y-=5; // no space betwen sections
   }

   // --------------------------------------
   // extra levels
   // --------------------------------------
   if (show_extra)
   {
      int ac = 14; // area color
      if (draw) al_draw_filled_rectangle(x1, y, x2, y+154, mColor.pc[ac+bco]); // background color
      show_level_stats_title_and_header(x1, x2, gy, ty, max_x, y, draw, vline, vli, tally, ac, "Extra Levels", msg_type);

      // extra levels (10)
      if (draw) al_draw_line(x1, y, x2, y, mColor.pc[ac], 1);
      y+=4;
      for(int i=0; i<100; i++)
         if (area_array[i][1] == 10)
            show_level_stats_row(i, x1, x2, draw, max_x, y, vline, vli, tally, msg_type);

      y+=4;
      if (draw) al_draw_line(x1, y, x2, y, mColor.pc[ac], 1);
      y+=4;

      show_level_stats_totals(x1, x2, gy, ty, max_x, y, draw, vline, vli, tally, ac, msg_type);
      show_level_stats_outline_with_thicker_lines(x1, x2, gy, y, draw, mColor.pc[ac]);
      y-=5; // no space betwen sections
   }



   // --------------------------------------
   // demo levels
   // --------------------------------------
   if (show_demo)
   {
      int ac = 13; // area color
      if (draw) al_draw_filled_rectangle(x1, y, x2, y+170, mColor.pc[ac+bco]); // background color
      show_level_stats_title_and_header(x1, x2, gy, ty, max_x, y, draw, vline, vli, tally, ac, "Demo Levels", msg_type);

      if (draw) al_draw_line(x1, y, x2, y, mColor.pc[ac], 1);
      y+=4;
      for(int i=0; i<100; i++)
         if (area_array[i][1] == 12)
            show_level_stats_row(i, x1, x2, draw, max_x, y, vline, vli, tally, msg_type);

      y+=4;
      if (draw) al_draw_line(x1, y, x2, y, mColor.pc[ac], 1);
      y+=4;

      show_level_stats_totals(x1, x2, gy, ty, max_x, y, draw, vline, vli, tally, ac, msg_type);
      show_level_stats_outline_with_thicker_lines(x1, x2, gy, y, draw, mColor.pc[ac]);
      y-=5; // no space betwen sections
   }

   // --------------------------------------
   // grand totals
   // --------------------------------------
   if (show_grand_totals)
   {
      for (int i=0; i<16; i++) tally[0][i] = tally[1][i]; // copy totals to group so I can show them
      int ac = 3; // area color
      if (draw) al_draw_filled_rectangle(x1, y, x2, y+50, mColor.pc[ac+bco]); // background color
      show_level_stats_title_and_header(x1, x2, gy, ty, max_x, y, draw, vline, vli, tally, ac, "Grand Totals", msg_type);
      if (draw) al_draw_line(x1, y, x2, y, mColor.pc[ac], 1);
      y +=4;
      show_level_stats_totals(x1, x2, gy, ty, max_x, y, draw, vline, vli, tally, ac, msg_type);
      show_level_stats_outline_with_thicker_lines(x1, x2, gy, y, draw, mColor.pc[ac]);
   }
   y-=8;

   // set actual w and h
   w = max_x - x1;
   h = y+1;
}
