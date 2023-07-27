// mwLevelData.cpp

#include "pm.h"
#include "mwLevel.h"
#include "mwConfig.h"
#include "mwLift.h"
#include "mwItem.h"
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


void mwLevel::reset_level_data(void)
{
   clear_data();
   save_data();
   level_stats_bmp_msg_type = 0;        // to force recreation
   load_level(mLevel.play_level, 0, 0); // reload play level
}

void mwLevel::unlock_all_levels(void)
{
   for(int i=0; i<100; i++) data[i].unlocked = 1;
   for(int i=0; i<16; i++) area_locks[i] = 0;
   save_data();
   level_stats_bmp_msg_type = 0;        // to force recreation
   load_level(mLevel.play_level, 0, 0); // reload play level
}

void mwLevel::setup_data(void)
{
   load_data();
   load_level_icons();
}

void mwLevel::level_start_data(void)
{
//   printf("level_start_data() play_level:%d\n", play_level);
   level_data_purple_coins_collected = 0;
   level_data_player_respawns = 0;
   level_data_enemies_killed = 0;
}


void mwLevel::level_abort_data(int lev)
{
   if (lev != 1)
   {
      int i = play_data_num;
      // printf("save level_abort_data(%d) %d\n", lev, i);

      // add entry to play_data[] array
      play_data[i].level = lev;
      play_data[i].timer = mLoop.frame_num;
      play_data[i].player_respawns = level_data_player_respawns;
      play_data[i].enemies_killed = level_data_enemies_killed;
      play_data[i].purple_coins_collected = level_data_purple_coins_collected;
      play_data[i].enemies_left_alive_at_exit = mEnemy.num_enemy;
      play_data_num++;

      // only these can be updated by an incomplete level
      if (level_data_enemies_killed         > data[lev].max_enemies_killed)         data[lev].max_enemies_killed         = level_data_enemies_killed;
      if (level_data_purple_coins_collected > data[lev].max_purple_coins_collected) data[lev].max_purple_coins_collected = level_data_purple_coins_collected;
   }
   save_data();
}

void mwLevel::level_complete_data(int type, int lev)
{
   int save_data = 1;
   if (mPlayer.syn[mPlayer.active_local_player].control_method == 1) save_data = 0; // if running demo mode, don't save data
   if (mLevel.skc_trigger_demo_cheat) save_data = 1;                                // in cheat mode save data
   if (mDemoMode.demo_debug_running_demo_saves_level_data) save_data = 1;           // or if this is option is set

   if (save_data)
   {
      mLevel.skc_trigger_demo_cheat = 0;

      data[lev].unlocked = 1; // if for some reason it isn't already

      int ct = mLoop.frame_num; // completion time
      if (type == 1) ct = data[lev].par_time+400; // fake time (par time + 10s)


      // add entry to play_data[] array
      int i = play_data_num;
      // printf("save level_complete_data(%d) %d\n", lev, i);
      play_data[i].level = lev;
      play_data[i].timer = ct;
      play_data[i].completed = 1;
      play_data[i].player_respawns = level_data_player_respawns;
      play_data[i].enemies_killed = level_data_enemies_killed;
      play_data[i].purple_coins_collected = level_data_purple_coins_collected;
      play_data[i].enemies_left_alive_at_exit = mEnemy.num_enemy;
      play_data_num++;


      // completed for the first time?
      if (!data[lev].completed)
      {
         //printf("first time:%d\n", ct);
         data[lev].completed = 1;
         data[lev].best_time                  = ct;
         data[lev].min_respawns               = level_data_player_respawns;
         data[lev].max_enemies_killed         = level_data_enemies_killed;
         data[lev].max_purple_coins_collected = level_data_purple_coins_collected;
         data[lev].min_enemies_left           = mEnemy.num_enemy;
      }

      // check for new best values
      if (ct                                < data[lev].best_time)                  data[lev].best_time                  = ct;
      if (level_data_player_respawns        < data[lev].min_respawns)               data[lev].min_respawns               = level_data_player_respawns;
      if (level_data_enemies_killed         > data[lev].max_enemies_killed)         data[lev].max_enemies_killed         = level_data_enemies_killed;
      if (level_data_purple_coins_collected > data[lev].max_purple_coins_collected) data[lev].max_purple_coins_collected = level_data_purple_coins_collected;
      if (mEnemy.num_enemy                  < data[lev].min_enemies_left)           data[lev].min_enemies_left           = mEnemy.num_enemy;

      check_achievments();
   }
}


void mwLevel::check_achievments(void)
{
   // do the level unlocks by iterating levels with area_array, thay all should be in order
   for(int i=0; i<99; i++)
      if (data[area_array[i][0]].completed) data[area_array[i+1][0]].unlocked = 1;

   // do the area unlocks
   for(int a=1; a<10; a++) // iterate areas
   {
      int complete = 1; // area complete by default
      for(int j=0; j<100; j++)
         if ((area_array[j][1] == a) && (data[area_array[j][0]].completed == 0)) complete = 0;
      if (complete) area_locks[a+1] = 0; // open barrier next area
   }

   // special case for basic training levels
   if (data[83].completed)
   {
      area_locks[1] = 0;    // open barrier to next lower row
      data[2].unlocked = 1; // open first level in area 1


      area_locks[14] = 0;   // open barrier to area 14, advanced training
      unlock_all_level_in_area(14);
   }
   save_data();

   level_stats_bmp_msg_type = 0;

}












































void mwLevel::show_level_stats_row(int i, int x1, int x2, int draw, int &max_x, int &y, int vline[], int &vli, int tally[][16], int msg_type)
{
   char msg[80];

   int x = x1;
   vli = 0;

   int show_purple_coins = 1;
   int show_best_time = 1;
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
//      else if (lev>0) al_draw_textf(mFont.pr8, mColor.pc[15], x, y,   0, "%s", data[lev].level_name);
      else if (lev>0) al_draw_textf(mFont.pr8, mColor.pc[15], x, y,   0, "%2d %s", lev, data[lev].level_name);
   }
   x += width + 8;
   vline[vli++] = x-4;


   // get level status
   int col = 15, status;
   char stxt[80];

   int tc = 0; // number of times played
   int tb = 0; // number of times beat
   int tq = 0; // number of times quit

   int tta = 0; // total time spent playing
   int ttb = 0; // total time spent playing only if level completed


   int wt = 0; // worst time
   int mt = 0; // average time


   if (lev != -1)
   {

      tally[0][0]++; // num levels

      get_level_status(lev, status, col, stxt);

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

      tally[0][8] += data[lev].par_time;
      tally[0][9] += data[lev].best_time;
      tally[0][10] += tta; // total play time all



   }






   // level status
   width = 9*8;
   if (draw)
   {
      if (header) al_draw_text( mFont.pr8, mColor.pc[ 15], x+width/2, y+4, ALLEGRO_ALIGN_CENTER, "Status");
      else if (total)
      {
         sprintf(stxt, "-"); col = 15;
         if (tally[0][1] == tally[0][0]) { sprintf(stxt, "Complete"); col = 12; }
         if (tally[0][2] == tally[0][0]) { sprintf(stxt, "Perfect"); col = 8; }
         al_draw_text(mFont.pr8, mColor.pc[col],  x+width/2, y,   ALLEGRO_ALIGN_CENTER, stxt);
      }
      else al_draw_text(mFont.pr8, mColor.pc[col], x+width/2, y,   ALLEGRO_ALIGN_CENTER, stxt);
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
      width = 7 * 8;
      if (draw)
      {
         if (header)
         {
            al_draw_text(mFont.pr8, mColor.pc[15], x+width/2, y,   ALLEGRO_ALIGN_CENTER, "Best");
            al_draw_text(mFont.pr8, mColor.pc[15], x+width/2, y+8, ALLEGRO_ALIGN_CENTER, "Time");
         }
         else if (total) al_draw_text(mFont.pr8, mColor.pc[15], x+width/2, y, ALLEGRO_ALIGN_CENTER, mItem.chrms(tally[0][9], msg));
         else
         {
            if ((data[lev].best_time == 0) || (data[lev].completed == 0))
            {
               al_draw_text(mFont.pr8, mColor.pc[15], x+width/2, y, ALLEGRO_ALIGN_CENTER, "n/a");
            }
            else
            {
               int tc = 15;
               if (data[lev].best_time < data[lev].par_time) tc = 8;
               al_draw_text(mFont.pr8, mColor.pc[tc], x+width/2, y, ALLEGRO_ALIGN_CENTER, mItem.chrms(data[lev].best_time, msg));
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
         else if (total) al_draw_text(mFont.pr8, mColor.pc[15], x+width/2, y, ALLEGRO_ALIGN_CENTER, mItem.chrms(tally[0][8], msg));
         else            al_draw_text(mFont.pr8, mColor.pc[15], x+width/2, y, ALLEGRO_ALIGN_CENTER, mItem.chrms(data[lev].par_time, msg));
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
         else if (total) al_draw_text(mFont.pr8, mColor.pc[15], x+width/2, y, ALLEGRO_ALIGN_CENTER, mItem.chrms(tally[0][10], msg));
         else al_draw_text(mFont.pr8, mColor.pc[15], x+width/2, y, ALLEGRO_ALIGN_CENTER, mItem.chrms(tta, msg));
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
         else if (!total) al_draw_text(mFont.pr8, mColor.pc[15], x+width/2, y, ALLEGRO_ALIGN_CENTER, mItem.chrms(wt, msg));
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
         else if (!total) al_draw_text(mFont.pr8, mColor.pc[15], x+width/2, y, ALLEGRO_ALIGN_CENTER, mItem.chrms(mt, msg));
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
//      al_draw_filled_rectangle(x1, y, x2, y+ys, mColor.pc[col+192+32]); // background color
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



void mwLevel::get_level_status(int lev, int& status, int& col, char txt[])
{
   col = 10;   // default red for locked
   status = 0; // 0=locked, 1=ready, 2=complete, 3=perfect
   if (data[lev].unlocked)  status = 1; // ready
   if (data[lev].completed) status = 2; // completed
   if ((data[lev].max_purple_coins_collected == data[lev].tot_purple_coins) && ((data[lev].best_time < data[lev].par_time) && (data[lev].completed))) status = 3; // perfect

   if (status == 0) { sprintf(txt, "Locked");   col = 10; } // red
   if (status == 1) { sprintf(txt, "Ready");    col = 13; } // lt blue
   if (status == 2) { sprintf(txt, "Complete"); col = 12; } // dk blue
   if (status == 3) { sprintf(txt, "Perfect!"); col = 8;  } // purple
}


void mwLevel::sob_hline(int x1, int x2, int y, int a)
{
   for(int x=x1; x<x2+1; x++)
   {
      int block = 0;
      if (a)
      {
                      block = 582 | PM_BTILE_ALL_SOLID; // thru
         if (x == x1) block = 590 | PM_BTILE_ALL_SOLID;
         if (x == x2) block = 588 | PM_BTILE_ALL_SOLID;
      }
      mLevel.l[x][y] = block;
   }
}

void mwLevel::sob_vline(int x, int y1, int y2, int a)
{
   for(int y=y1; y<y2+1; y++)
   {
      int block = 0;
      if (a)
      {
                      block = 580 | PM_BTILE_ALL_SOLID; // thru
         if (y == y1) block = 591 | PM_BTILE_ALL_SOLID;
         if (y == y2) block = 589 | PM_BTILE_ALL_SOLID;
      }
      mLevel.l[x][y] = block;
   }
}

void mwLevel::sob_create_msg(const char* txt, int col, int x, int y, int w, int h)
{
   // check and delete any existing message with first 7 char matching txt 'Area xx'
   char msg[80];
   sprintf(msg, "%.7s", txt);

   int msg_id = find_msg(msg);
   if (msg_id != -1) mItem.erase_item(msg_id); // erase msg if it exists

   // create new message
   int i = mItem.get_empty_item(10);
   mItem.item[i][0] = 10;
   mItem.item[i][2] = 65;
   mItem.item[i][4] = x-10;
   mItem.item[i][5] = y-10;
   mItem.item[i][6] = x;
   mItem.item[i][7] = y;
   mItem.item[i][8] = w;
   mItem.item[i][9] = h;
   mMiscFnx.set_int_3216(mItem.item[i][13], 15, col); // set text and frame colors
   strcpy(mItem.pmsgtext[i], txt);
}


void mwLevel::sob_area_msg(int area, int x, int y)
{
   int w = 95;
   int h = 57;

   char msg1[20];
   char msg2[20];
   char msg3[20];
   char msg4[20];


   sprintf(msg1, "Area %d", area);

   if (area == 13) { sprintf(msg1, "Basic Training"); w = 130; }
   if (area == 14) { sprintf(msg1, "More Training");  w = 130; }
   if (area == 9)  { sprintf(msg1, "Final Area");     w = 110; }
   if (area == 16) { sprintf(msg1, "All Levels");     w = 110; }
   if (area == 17) { sprintf(msg1, "Game Levels");    w = 110; }
   if (area == 10) { sprintf(msg1, "Extra Levels");   w = 120; }
   if (area == 12) { sprintf(msg1, "Demo Levels");    w = 120; }


   int cmp = 0; // count how many levels in this area are complete
   int pct = 0; // count total purple coins in this area
   int pcc = 0; // count max purple coins clooected in this area
   int tmr = 0; // count how many levels in this area have below par time

   int nl = 0;
   for (int i=0; i<100; i++)
      if ((area_array[i][1] == area) || ((area == 16) && (area_array[i][1]) && (area_array[i][1] != 11)) || ((area == 17) && (area_array[i][1] > 0) && (area_array[i][1] < 10)  ))
      {
         nl++;
         int l = area_array[i][0];
         cmp += data[l].completed;
         pct += data[l].tot_purple_coins;
         pcc += data[l].max_purple_coins_collected;
         if ((data[l].best_time > 0) && (data[l].best_time < data[l].par_time) && (data[l].completed)) tmr++;
      }

   int col = 10;
   if ((area_locks[area]) && (area != 16) && (area != 17)) // locked
   {
      sprintf(msg2, "(Locked)");
      col = 10;
   }
   else
   {
      sprintf(msg2, "Done:%d/%d", cmp, nl);
      col = 13;

      if (cmp == nl)
      {
         sprintf(msg2, "Complete");
         col = 12;
      }
      if ((pcc == pct) && (tmr == nl))
      {
         sprintf(msg2, "Perfect!");
         col = 8;
      }
   }

   sprintf(msg3, "Coin:%d/%d", pcc, pct);
   //sprintf(msg3, "Coin:15/15");
   sprintf(msg4, "Time:%d/%d", tmr, nl);

   char msg[256];
   sprintf(msg, "%s\n%s\n%s\n%s", msg1, msg2, msg3, msg4);
   sob_create_msg(msg, col, x, y, w, h);
}















int mwLevel::find_msg(const char* str)
{
   int msg_id = -1;
   for(int i=0; i<500; i++)
      if ((mItem.item[i][0] == 10) && (!strncmp(mItem.pmsgtext[i], str, strlen(str)))) msg_id = i;
   return msg_id;
}







// only ever called from load level after loading level 1
void mwLevel::set_overworld_barriers(void)
{

   // --------------------------------
   // area 0
   // --------------------------------
   int msg_id = find_msg("Complete Basic");
   if (area_locks[1])   // locked between training levels and area 1,2
   {
      if (msg_id == -1) // create msg if it does not exist
      {
         int i = mItem.get_empty_item(10);
         mItem.item[i][0] = 10;
         mItem.item[i][2] = 11;
         mItem.item[i][4] = 980;
         mItem.item[i][5] = 160;
         mItem.item[i][6] = 867;
         mItem.item[i][7] = 183;
         mItem.item[i][8] = 266;
         mItem.item[i][9] = 15;
         mItem.item[i][13] = 851980;
         sprintf(mItem.pmsgtext[i], "Complete Basic Training");
      }
   }
   else if (msg_id != -1) mItem.erase_item(msg_id); // erase msg if it exists


   sob_area_msg(13, 625,  40); // basic training levels
   sob_area_msg(14, 1250, 40); // advanced training levels

   sob_area_msg(1,  745,  220);
   sob_area_msg(2,  1161, 220);
   sob_area_msg(3,  745,  400);
   sob_area_msg(4,  1161, 400);
   sob_area_msg(5,  745,  580);
   sob_area_msg(6,  1161, 580);
   sob_area_msg(7,  745,  760);
   sob_area_msg(8,  1161, 760);

   sob_area_msg(9,  725,  940);  // final area


   sob_area_msg(17, 1161, 940);  // main area levels
   sob_area_msg(16, 1161, 1010); // all levels

   sob_area_msg(10, 745,  1410); // extra levels
   sob_area_msg(12, 940,  1750); // demo levels


   // blocking lines of blocks
   sob_vline(60, 1,  8,  area_locks[14]);  // blocking area 14 (advanced training)

   sob_vline(56, 10, 17, area_locks[2]);   // blocking area 2
   sob_vline(56, 19, 26, area_locks[4]);   // blocking area 4
   sob_vline(56, 28, 35, area_locks[6]);   // blocking area 6
   sob_vline(56, 37, 44, area_locks[8]);   // blocking area 8

   sob_hline(45, 54, 9,  area_locks[1]);   // blocking between training levels and area 1, 2
   sob_hline(45, 54, 18, area_locks[3]);   // blocking between area 1,2 and area 3,4
   sob_hline(45, 54, 27, area_locks[5]);   // blocking between area 3,4 and area 5,6
   sob_hline(45, 54, 36, area_locks[7]);   // blocking between area 5,6 and area 7,8
   sob_hline(45, 54, 45, area_locks[9]);   // blocking between area 7,8 and area 9

   sob_vline(43, 46, 53, area_locks[10]);   // blocking between area 9 and the rest
   sob_vline(56, 46, 53, area_locks[10]);   // blocking between area 9 and the rest

   sob_vline(0,  46, 57, !area_locks[10]);   // edge pieces that I want invisible until 10 done
   sob_vline(99, 46, 84, !area_locks[10]);   // edge pieces that I want invisible until 10 done


   // how much of overworld level to hide
   int lowest_visible_y = 200; // default only top training row visible
   if (!area_locks[1]) lowest_visible_y = 380; // area 1 and 2 visible
   if (!area_locks[3]) lowest_visible_y = 560; // area 3 and 4 visible
   if (!area_locks[5]) lowest_visible_y = 740; // area 5 and 6 visible
   if (!area_locks[7]) lowest_visible_y = 920; // area 7 visible
   if (!area_locks[9]) lowest_visible_y = 1100; // area 9 visible
   if (!area_locks[10]) lowest_visible_y = 2000; // area 9 visible

   // remove all hiders
   for(int i=0; i<500; i++) if (mItem.item[i][0] == 19) mItem.item[i][0] = 0;

   // create hider
   int i = mItem.get_empty_item(19);
   mItem.item[i][0] = 19;
   mItem.item[i][1] = 0;
   mItem.item[i][2] = 1;
   mItem.item[i][3] = 1;
   mItem.item[i][4] = 40;
   mItem.item[i][5] = 40;
   mItem.item[i][7] = lowest_visible_y;
   mItem.item[i][8] = 2000;
   mItem.item[i][9] = 2000 - lowest_visible_y;

   save_level(1); // save overworld level
}



void mwLevel::faa_helper(int x1, int x2, int y1, int y2, int xasc, int &aai, int area)
{
   int res[50][3] = { 0 }; // results array
   int resi = 0;

   // find all gates in the area
   for(int i=0; i<100; i++)
      if ((mItem.item[i][0] == 18) && (mItem.item[i][4] >= x1) && (mItem.item[i][4] <= x2) && (mItem.item[i][5] >= y1) && (mItem.item[i][5] <= y2))
      {
         res[resi][0] = mItem.item[i][6]; // level num
         res[resi][1] = mItem.item[i][4]; // x
         res[resi][2] = mItem.item[i][5]; // y
         resi++;
      }

   // sort results by x position
   int swap_flag = 1;
   while (swap_flag)
   {
      swap_flag = 0;
      for(int i=0; i<resi-1; i++)
         if ( ((xasc == 1) && (res[i][1] > res[i+1][1])) || ((xasc == 0) && (res[i][1] < res[i+1][1])) )  // sort by x position, ascending or desc
         {
            swap_flag = 1;
            for (int k=0; k<3; k++)
            {
               res[49 ][k]  = res[i  ][k];  // use 49 for temp swap
               res[i  ][k]  = res[i+1][k];
               res[i+1][k]  = res[49 ][k];
            }
         }
   }

   // add to area array
   for (int i=0; i<resi; i++)
   {
      area_array[aai][0] = res[i][0];
      area_array[aai][1] = area;
      aai++;
   }
}



// searches the overworld level for gates in certain areas, then adds their levels to area array
void mwLevel::fill_area_array(void)
{
   // clear area array and index
   for (int i=0; i<100; i++)
   {
      area_array[i][0] = 0;
      area_array[i][1] = 0;
   }
   int aai = 0;

   faa_helper(0,    700,  0,    180,  0, aai, 13); // basic training levels
   faa_helper(1000, 2000, 0,    180,  1, aai, 14); // advanced training levels
   faa_helper(0,    1000, 200,  360,  0, aai, 1);  // area 1
   faa_helper(1000, 2000, 200,  360,  1, aai, 2);  // area 2
   faa_helper(0,    1000, 380,  540,  0, aai, 3);  // area 3
   faa_helper(1000, 2000, 380,  540,  1, aai, 4);  // area 4
   faa_helper(0,    1000, 560,  720,  0, aai, 5);  // area 5
   faa_helper(1000, 2000, 560,  720,  1, aai, 6);  // area 6
   faa_helper(0,    1000, 740,  900,  0, aai, 7);  // area 7
   faa_helper(1000, 2000, 740,  900,  1, aai, 8);  // area 8
   faa_helper(900,  1100, 920,  1080, 1, aai, 9);  // area 9 (final boss)
   faa_helper(0,    1000, 1160, 1700, 1, aai, 10); // area 10 (extra levels)
   faa_helper(1000, 2000, 1160, 1700, 1, aai, 11); // area 11 (muliplayer bomb toss levels)
   faa_helper(0,    2000, 1840, 2000, 1, aai, 12); // area 12 (advanced info levels)
//   for(int i=0; i<aai; i++) printf("area:%d level:%d\n", area_array[i][1], area_array[i][0]);
}


void mwLevel::unlock_all_level_in_area(int area)
{
   for(int i=0; i<100; i++) if (area_array[i][1] == area) data[area_array[i][0]].unlocked = 1;
}


void mwLevel::clear_data(void)
{
   for(int i=0; i<16; i++) area_locks[i] = 1; // set all locks

   area_locks[13] = 0; // basic training area unlocked

   load_level(1, 1, 0); // load overworld level (also sets overworld barriers)

   fill_area_array();

   overworld_level = 0; // no gate will be found and player will start from start block

   for(int i=0; i<100; i++)
   {
      strcpy(data[i].level_name, "");
      data[i].par_time = 0;
      data[i].unlocked = 0;
      data[i].completed = 0;
      data[i].best_time = 0;
      data[i].min_respawns = 0;
      data[i].max_purple_coins_collected = 0;
      data[i].max_enemies_killed = 0;
      data[i].tot_purple_coins = 0;
      data[i].min_enemies_left = 0;
      data[i].min_enemies_left_par = 0;
   }

   int i = 2;
   strcpy(data[i].level_name, "Switch Nest");
   data[i].par_time = 4800; // 2:00 demo 1:35

   i = 3;
   strcpy(data[i].level_name, "Blue Key Fall");
   data[i].par_time = 1400; // 35s demo 27.9

   i = 4;
   strcpy(data[i].level_name, "Switch Puzzle");
   data[i].par_time = 7200; // 3:00 demo 2:20

   i = 5;
   strcpy(data[i].level_name, "Kill Kill Kill");
   data[i].par_time = 6400; // 2:40 demo 2:18

   i = 6;
   strcpy(data[i].level_name, "Breakable");
   data[i].par_time = 9600; // 4:00 demo 2:58

   i = 7;
   strcpy(data[i].level_name, "Ranger Bob");
   data[i].par_time = 2000; // 50s demo 46.5

   i = 8;
   strcpy(data[i].level_name, "Switch Pit");
   data[i].par_time = 8800; // 3:40 demo 2:52

   i = 10;
   strcpy(data[i].level_name, "The Dead Zone");
   data[i].par_time = 11600; // 4:50 demo 3:15

   i = 11;
   strcpy(data[i].level_name, "Good and Evil");
   data[i].par_time = 19200; // 8:00 demo 4:45

   i = 12;
   strcpy(data[i].level_name, "Bucket of Bad");
   data[i].par_time = 16800; // 7:00 demo 6:10

   i = 13;
   strcpy(data[i].level_name, "Falling Arrows");
   data[i].par_time = 12400; // 5:10 demo 4:41

   i = 14;
   strcpy(data[i].level_name, "Block Puzzle");
   data[i].par_time = 400; // 10s demo 8.8

   i = 15;
   strcpy(data[i].level_name, "Smash");
   data[i].par_time = 1600; // 40s demo 33.7


   i = 16;
   strcpy(data[i].level_name, "Big Apple");
   data[i].par_time = 14400; // 6:00 demo 4:01

   i = 17;
   strcpy(data[i].level_name, "Little Rocket");
   data[i].par_time = 16800; // 7:00 demo 5:08

   i = 18;
   strcpy(data[i].level_name, "Incinerator");
   data[i].par_time = 12000; // 5:00 demo 4:04

   i = 19;
   strcpy(data[i].level_name, "Crazy Fox");
   data[i].par_time = 19200; // 8:00 demo 5:24

   i = 20;
   strcpy(data[i].level_name, "Strange Things");
   data[i].par_time = 6000; // 2:30 demo 2:22

   i = 21;
   strcpy(data[i].level_name, "Long One");
   data[i].par_time = 3600; // 1:30 demo 1:25

   i = 22;
   strcpy(data[i].level_name, "Rocket Stew");
   data[i].par_time = 4800; // 2:00 demo 1:28

   i = 23;
   strcpy(data[i].level_name, "Amazing");
   data[i].par_time = 21600; // 9:00 demo 7:57

   i = 24;
   strcpy(data[i].level_name, "Wendy");
   data[i].par_time = 6600; // 2:45 demo 2:26

   i = 26;
   strcpy(data[i].level_name, "Long Fall");
   data[i].par_time = 4800; // 2:00 demo 1:06

   i = 27;
   strcpy(data[i].level_name, "Gauntlet");
   data[i].par_time = 3600; // 1:30 demo 1:27 not done

   i = 28;
   strcpy(data[i].level_name, "Nowhere to Stand");
   data[i].par_time = 3600; // 1:30 demo 49.2

   i = 29;
   strcpy(data[i].level_name, "Brain Trust");
   data[i].par_time = 13200; // 5:30 demo 3:58

   i = 30;
   strcpy(data[i].level_name, "Coral Cave");
   data[i].par_time = 7200; // 3:00 demo 2:30

   i = 31;
   strcpy(data[i].level_name, "Beginnings");
   data[i].par_time = 19200; // 8:00 demo 6:03

   i = 32;
   strcpy(data[i].level_name, "Yellow Cake");
   data[i].par_time = 9600; // 4:00 demo 3:19

   i = 33;
   strcpy(data[i].level_name, "Kate's Gate");
   data[i].par_time = 6000; // 2:30 demo 2:06

   i = 34;
   strcpy(data[i].level_name, "Crush");
   data[i].par_time = 3600; // 1:30 demo 1:04

   i = 35;
   strcpy(data[i].level_name, "Bomb Toss");
   data[i].par_time = 13200; // 5:30 demo 3:37

   i = 38;
   strcpy(data[i].level_name, "Come Over");
   data[i].par_time = 3200; // 1:20 demo 1:06

   i = 39;
   strcpy(data[i].level_name, "Bomb Run");
   data[i].par_time = 2400; // 1:00 demo 55.7

   i = 40;
   strcpy(data[i].level_name, "Edward");
   data[i].par_time = 4200; // 1:45 demo 1:05

   i = 41;
   strcpy(data[i].level_name, "Ender");
   data[i].par_time = 9600; // 4:00 demo 3:27

   i = 42;
   strcpy(data[i].level_name, "Cloner Chain");
   data[i].par_time = 16800; // 7:00 demo 6:55 not done

   i = 43;
   strcpy(data[i].level_name, "Cathedral");
   data[i].par_time = 4800; // 2:00 demo 1:45

   i = 44;
   strcpy(data[i].level_name, "Too Many Lifts");
   data[i].par_time = 12000; // 5:00 demo 2:59

   i = 45;
   strcpy(data[i].level_name, "Chimney of Death");
   data[i].par_time = 1800; // :45 demo 0:37.9 not done

   i = 46;
   strcpy(data[i].level_name, "Bucket List");
   data[i].par_time = 2400; // 1:00 demo 38.8

   i = 47;
   strcpy(data[i].level_name, "Begin Again");
   data[i].par_time = 2400; // 1:00 demo 27.1

   i = 48;
   strcpy(data[i].level_name, "Mayhem");
   data[i].par_time = 2400; // 1:00 demo :43.7 not done

   i = 49;
   strcpy(data[i].level_name, "Rocket Square");
   data[i].par_time = 9600; // 4:00 demo 2:37

   i = 50;
   strcpy(data[i].level_name, "Welcome");
   data[i].par_time = 1600; // :40 demo 30.2

   i = 51;
   strcpy(data[i].level_name, "Cannon Bees");
   data[i].par_time = 6000; // 2:30 demo 2:21

   i = 52;
   strcpy(data[i].level_name, "Boules");
   data[i].par_time = 3600; // 1:30 demo 1:07

   i = 53;
   strcpy(data[i].level_name, "Lifts With Names");
   data[i].par_time = 8400; // 3:30 demo 3:12 not done

   i = 54;
   strcpy(data[i].level_name, "Old Training Level");
   data[i].par_time = 2400; // 1:00 demo :44.6 not done

   i = 55;
   strcpy(data[i].level_name, "Ancient Ruins");
   data[i].par_time = 14400; // 6:00 demo 5:15 not done

   i = 56;
   strcpy(data[i].level_name, "Zaiden");
   data[i].par_time = 12000; // 5:00 demo 2:44

   i = 57;
   strcpy(data[i].level_name, "Valentine");
   data[i].par_time = 12000; // 5:00 demo 2:43

   i = 60;
   strcpy(data[i].level_name, "Bomb Toss"); // same name as level 35...
   data[i].par_time = 2400; // 2:00 demo // not done

   i = 61;
   strcpy(data[i].level_name, "Bomb Toss 2");
   data[i].par_time = 2400; // 2:00 demo

   i = 64;
   strcpy(data[i].level_name, "Escape Rocket!");
   data[i].par_time = 16880; // 7:02 demo 4:30


   // ---------------------------------------
   // Test Levels
   // ---------------------------------------

//   i = 70;
//   strcpy(data[i].level_name, "Test Level 1");
//   data[i].par_time = 200;
//   data[i].unlocked = 1;
//
//   i = 71;
//   strcpy(data[i].level_name, "Test Level 2");
//   data[i].par_time = 240;



   // ---------------------------------------
   // Training Levels
   // ---------------------------------------

   i = 80;
   strcpy(data[i].level_name, "Training Level 1");
   data[i].par_time = 1200; // 30s demo 21.2
   data[i].unlocked = 1;

   i = 81;
   strcpy(data[i].level_name, "Training Level 2");
   data[i].par_time = 4800; // 2:00 demo 53.4

   i = 82;
   strcpy(data[i].level_name, "Training Level 3");
   data[i].par_time = 8400; // 3:30 demo 2:12

   i = 83;
   strcpy(data[i].level_name, "Training Level 4");
   data[i].par_time = 6000; // 2:30 demo 1:14

   i = 84;
   strcpy(data[i].level_name, "Training Level 5");
   data[i].par_time = 3600; // 1:30 demo 52.8

   i = 9;
   strcpy(data[i].level_name, "Bomb Intro");
   data[i].par_time = 6000; // 2:30 demo 1:35

   i = 25;
   strcpy(data[i].level_name, "One Cannon");
   data[i].par_time = 1000; // 25s demo 21.9

   i = 37;
   strcpy(data[i].level_name, "Sacrifice");
   data[i].par_time = 800; // :20 demo 13.3


   // ---------------------------------------
   // Advanced Information Levels
   // ---------------------------------------
   i = 85;
   strcpy(data[i].level_name, "Doors");
   data[i].par_time = 10800; // 4:30 demo 1:48

   i = 86;
   strcpy(data[i].level_name, "Triggers and Timers");
   data[i].par_time = 7200; // 3:00 demo 1:26

   i = 87;
   strcpy(data[i].level_name, "Block Manip");
   data[i].par_time = 2400; // 1:00 demo 28.4

   i = 88;
   strcpy(data[i].level_name, "Block Damage");
   data[i].par_time = 6000; // 2:30 demo 1:24

   i = 89;
   strcpy(data[i].level_name, "Orbs");
   data[i].par_time = 4800; // 2:00 demo 51.8

   i = 90;
   strcpy(data[i].level_name, "Archwagons");
   data[i].par_time = 4800; // 2:00 demo 1:51

   i = 91;
   strcpy(data[i].level_name, "Trakbots");
   data[i].par_time = 2400; // 1:00 demo 18.9

   i = 92;
   strcpy(data[i].level_name, "Bouncers and Cannons");
   data[i].par_time = 4800; // 2:00 demo 1:11

   i = 93;
   strcpy(data[i].level_name, "Cloners");
   data[i].par_time = 4800; // 2:00 demo 35.5

   i = 94;
   strcpy(data[i].level_name, "Vinepods");
   data[i].par_time = 4800; // 2:00 demo 37.7

   i = 95;
   strcpy(data[i].level_name, "Blokwalks");
   data[i].par_time = 4800; // 2:00 demo 36.4

   i = 96;
   strcpy(data[i].level_name, "Flappers");
   data[i].par_time = 2400; // 1:00 demo 3.9

   i = 97;
   strcpy(data[i].level_name, "Jumpworms");
   data[i].par_time = 4800; // 2:00 demo 1:18

   i = 98;
   strcpy(data[i].level_name, "Lifts");
   data[i].par_time = 7200; // 3:00 demo 1:11

   for(int i=0; i<100000; i++)
   {
      play_data[i].level = 0;
      play_data[i].start_timestamp=0;
      play_data[i].timer=0;
      play_data[i].completed = 0;
      play_data[i].enemies_killed = 0;
      play_data[i].enemies_left_alive_at_exit = 0;
      play_data[i].player_respawns = 0;
      play_data[i].purple_coins_collected = 0;
   }
   play_data_num = 0;

   unlock_all_level_in_area(10);
   unlock_all_level_in_area(11);
   unlock_all_level_in_area(12);

   // load all levels to get purple coin counts
   for (int p=0; p<100; p++)
   if (mLevel.load_level(p, 1, 1))
   {
      data[p].tot_purple_coins = 0;
      for (int i=0; i<500; i++)
         if ((mItem.item[i][0] == 2) && (mItem.item[i][6] == 3)) data[p].tot_purple_coins++;
   }





}


void mwLevel::load_data(void)
{
   FILE *fp =fopen("data/level_data.pm","rb");
   if (fp)
   {
      fread(data,           sizeof(data),          1, fp);
      fread(area_locks,     sizeof(area_locks),    1, fp);
      fread(area_array,     sizeof(area_array),    1, fp);
      fread(play_data,      sizeof(play_data),     1, fp);
      fread(&play_data_num, sizeof(play_data_num), 1, fp);
      fclose(fp);
      return;
   }
   else
   {
      printf("Error loading data/level_data.pm, recreating....\n");
      clear_data();
      save_data();
   }
}

void mwLevel::save_data(void)
{
   FILE *fp =fopen("data/level_data.pm","wb");
   if (fp)
   {
      fwrite(data,           sizeof(data),          1, fp);
      fwrite(area_locks,     sizeof(area_locks),    1, fp);
      fwrite(area_array,     sizeof(area_array),    1, fp);
      fwrite(play_data,      sizeof(play_data),     1, fp);
      fwrite(&play_data_num, sizeof(play_data_num), 1, fp);
      fclose(fp);
      return;
   }
   mInput.m_err("Error saving level_data.pm");
}



void mwLevel::create_level_icons(void)
{
   //double t0 = al_get_time();

   int sz1 = 100;
   ALLEGRO_BITMAP *tmp_100 = al_create_bitmap(sz1*10, sz1*10);
   al_set_target_bitmap(tmp_100);
   al_clear_to_color(al_map_rgba(0,0,0,0));

   int sz2 = 200;
   ALLEGRO_BITMAP *tmp_200 = al_create_bitmap(sz2*10, sz2*10);
   al_set_target_bitmap(tmp_200);
   al_clear_to_color(al_map_rgba(0,0,0,0));


   int x=0;
   int y=0;
   for (int i=0; i<100; i++)
   {
      if (mLevel.load_level(i, 0, 1))
      {
         mScreen.draw_level2(tmp_100,  x*sz1, y*sz1, sz1, 1, 1, 1, 1, 0);
         mScreen.draw_level2(tmp_200,  x*sz2, y*sz2, sz2, 1, 1, 1, 1, 0);
      }

      // show progress bar
      int pc = (i*100 + 1) / 100;
      al_set_target_backbuffer(mDisplay.display);
      //al_clear_to_color(al_map_rgb(0,0,0));
      mScreen.draw_percent_bar(mDisplay.SCREEN_W/2, mDisplay.SCREEN_H/2, mDisplay.SCREEN_W-200, 20, pc );
      al_draw_text(mFont.pr8, mColor.pc[15], mDisplay.SCREEN_W/2, mDisplay.SCREEN_H/2+6, ALLEGRO_ALIGN_CENTER, "Creating Level Icons");
      al_flip_display();

      if (++x > 9)
      {
         x = 0;
         y++;
      }
   }

   al_save_bitmap("data/level_icons_100.bmp", tmp_100);
   al_save_bitmap("data/level_icons_200.bmp", tmp_200);

   al_destroy_bitmap(tmp_100);
   al_destroy_bitmap(tmp_200);

   //printf("create_level_icons_time:%f\n", al_get_time() - t0);
   load_level_icons();
}



void mwLevel::load_level_icons(void)
{
   int sz = 100;
   ALLEGRO_BITMAP *tmp = al_load_bitmap("data/level_icons_100.bmp");
   if (!tmp)
   {
      printf("Error loading tiles from: level_icons_100.bmp - recreating\n");
      create_level_icons();
   }
   else
   {
      int x=0;
      int y=0;
      for (int i=0; i<100; i++)
      {
         if (!level_icon_100[i]) level_icon_100[i] = al_create_bitmap(sz, sz);

         al_set_target_bitmap(level_icon_100[i]);
         al_clear_to_color(al_map_rgba(0,0,0,0));

         al_draw_bitmap_region(tmp, x*sz, y*sz, sz, sz, 0, 0, 0);
         if (++x > 9)
         {
            x = 0;
            y++;
         }
     }
     al_destroy_bitmap(tmp);
   }

   sz = 200;
   tmp = al_load_bitmap("data/level_icons_200.bmp");
   if (!tmp)
   {
      printf("Error loading tiles from:level_icons_200.bmp - recreating\n");
      create_level_icons();
   }
   else
   {
      int x=0;
      int y=0;
      for (int i=0; i<100; i++)
      {
         if (!level_icon_200[i]) level_icon_200[i] = al_create_bitmap(sz, sz);

         al_set_target_bitmap(level_icon_200[i]);
         al_clear_to_color(al_map_rgba(0,0,0,0));

         al_draw_bitmap_region(tmp, x*sz, y*sz, sz, sz, 0, 0, 0);
         if (++x > 9)
         {
            x = 0;
            y++;
         }
     }
     al_destroy_bitmap(tmp);
   }
}

















