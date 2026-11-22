// mwLevelData.cpp

#include "pm.h"
#include "mwLevel.h"
#include "mwFont.h"
#include "mwBitmap.h"
#include "mwColor.h"
#include "mwMiscFnx.h"
#include "mwDisplay.h"
#include "mwLoop.h"

void mwLevel::show_level_stats_row(int i, int x1, int x2, int draw, int &max_x, int &y)
{

   bool print = 0;


   char msg[80];

   int x = x1;
   vli = 0;

   int show_purple_coins = 1;
   int show_best_time = 1;
   int show_coin_time = 1;
   int show_par_time = 1;
   int show_min_deaths = 0;
   int show_max_kills = 0;
   int show_times_played = 0;
   int show_time_played = 0;
   int show_worst_time = 0;
   int show_average_time = 0;
   int show_times_beat = 0;
   int show_times_quit = 0;

   int show_enemies_left = 0;


   if (row_type == 1) // all columns
   {
      show_min_deaths = 1;
      show_max_kills = 1;
      show_times_played = 1;
      show_time_played = 1;
      show_worst_time = 1;
      show_average_time = 1;
      show_times_beat = 1;
      show_times_quit = 1;
   }




   int lev = 0;
   int header = 0;


   int total = 0;

   if (i == -1) // header
   {
      header = 1;
      lev = -1;
   }

   int y_offset = 0;
   if (header) y_offset = 4;


   if (i == 100) // totals
   {
      total = 1;
      lev = -1;
   }

   vline[vli++] = x; // first vline
   x += 8; // first text spacer


   // // set level and area
   // if ((i > -1) && (i < 100))
   //    if (area_array[i][1]) lev = area_array[i][0];

   // WTF???

   if ((i > -1) && (i < 100)) lev = i;



   // level name
   int width = 20*8;

   int print_width = 24;

   if (draw)
   {

      if (header && print) printf("-------------------------------------------------------------\n");


      if (header) strcpy(msg, "Level");
      else if (total) strcpy(msg, "Totals");
      else if (lev > 0)
      {
         if (level_stats_show_level_number) sprintf(msg, "%2d %s", lev, data[lev].level_name); // prepend level number
         else                               sprintf(msg, "%s",          data[lev].level_name); // just level name
      }
      al_draw_text( mFont.pr8, mColor.pc[15], x, y+y_offset,   0, msg);

      if (print) printf("%-*s", print_width, msg);

   }
   x += width + 8;
   vline[vli++] = x-4;


   int status = data[lev].status;
   int col = level_status_color[status];

   if (lev != -1)
   {
      tally[0][0]++; // num levels

      if ((status == 2) || (status == 3)) tally[0][1]++; // complete
      if (status == 3) tally[0][2]++; // perfect

      tally[0][3]  += data[lev].times_played;
      tally[0][11] += data[lev].times_beat;
      tally[0][12] += data[lev].times_quit;
      tally[0][6]  += data[lev].min_respawns;
      tally[0][7]  += data[lev].max_enemies_killed;
      tally[0][4]  += data[lev].tot_coins;
      tally[0][5]  += data[lev].max_coins_collected;
      tally[0][8]  += data[lev].time_par;
      tally[0][9]  += data[lev].time_best;
      tally[0][13] += data[lev].time_best_all_coins;
      tally[0][10] += data[lev].time_total;

//      tally[0][14] += data[lev].min_enemies_left;




   }


   // level status
   width = 9*8;
   print_width = 9;

   if (draw)
   {
      col = 15;
      if (header) strcpy(msg, "Status");
      else if (total)
      {
         strcpy(msg, "-");
         if (tally[0][1] == tally[0][0]) { sprintf(msg, "Complete"); col = 12; }
         if (tally[0][2] == tally[0][0]) { sprintf(msg, "Perfect"); col = 8; }
      }
      else
      {
         strcpy(msg, mLevel.level_status_text[mLevel.data[lev].status]);
         if (status == 3) col = 8;
      }

      al_draw_text( mFont.pr8, mColor.pc[col], x+width/2, y+y_offset, ALLEGRO_ALIGN_CENTER, msg);

      if (print) printf("%-*s", print_width, msg);


   }
   x += width + 8;
   vline[vli++] = x-4;



   if (show_purple_coins)
   {
      width = 7*8;
      print_width = 8;
      col = 15;

      if (draw)
      {
         if (header)
         {
            al_draw_text(mFont.pr8, mColor.pc[col], x+width/2, y,   ALLEGRO_ALIGN_CENTER, "Purple");
            al_draw_text(mFont.pr8, mColor.pc[col], x+width/2, y+8, ALLEGRO_ALIGN_CENTER, "Coins");
            strcpy(msg, "Coins");
         }
         else
         {
            if (total)
            {
               sprintf(msg, "%d/%d", tally[0][5], tally[0][4]);
               if (tally[0][5] == tally[0][4]) col = 8;
            }
            else
            {
               sprintf(msg, "%d/%d", data[lev].max_coins_collected, data[lev].tot_coins);
               if (status == 3) col = 8;
            }
            al_draw_text(mFont.pr8, mColor.pc[col], x+width/2, y+y_offset*2, ALLEGRO_ALIGN_CENTER, msg);
         }
         if (print) printf("%-*s", print_width, msg);
      }
      x += width + 8;
      vline[vli++] = x-4;
   }


   if (show_best_time)
   {
      width = 10 * 8;
      print_width = 18;
      col = 15;

      if (draw)
      {
         if (header)
         {
//            al_draw_text(mFont.pr8, mColor.pc[15], x+width/2, y,   ALLEGRO_ALIGN_CENTER, "Best");
//            al_draw_text(mFont.pr8, mColor.pc[15], x+width/2, y+8, ALLEGRO_ALIGN_CENTER, "Time");
            al_draw_text(mFont.pr8, mColor.pc[15], x+width/2, y,   ALLEGRO_ALIGN_CENTER, "Best Time");
            al_draw_text(mFont.pr8, mColor.pc[15], x+width/2, y+8, ALLEGRO_ALIGN_CENTER, "Overall");
            strcpy(msg, "Best Time Overall");
         }

         else
         {
            if (total) sprintf(msg, "%s", mMiscFnx.chrms(tally[0][9], msg));
            else
            {
               if ((data[lev].time_best == 0) || (data[lev].status < 2)) strcpy(msg, "n/a");
               else sprintf(msg, "%s", mMiscFnx.chrms(data[lev].time_best, msg));
            }
            al_draw_text(mFont.pr8, mColor.pc[col], x+width/2, y, ALLEGRO_ALIGN_CENTER, msg);
         }
         //if (print) printf("%-*s", print_width, msg);

      }
      x += width + 8;
      vline[vli++] = x-4;
   }

   if (show_coin_time)
   {
      width = 10 * 8;
      print_width = 10;
      col = 15;
      if (draw)
      {
         if (header)
         {
//            al_draw_text(mFont.pr8, mColor.pc[15], x+width/2, y,   ALLEGRO_ALIGN_CENTER, "Coin");
//            al_draw_text(mFont.pr8, mColor.pc[15], x+width/2, y+8, ALLEGRO_ALIGN_CENTER, "Time");
            al_draw_text(mFont.pr8, mColor.pc[15], x+width/2, y,   ALLEGRO_ALIGN_CENTER, "Best Time");
            al_draw_text(mFont.pr8, mColor.pc[15], x+width/2, y+8, ALLEGRO_ALIGN_CENTER, "All Coins");
//            strcpy(msg, "Best Time All Coins");

            strcpy(msg, "Best Time");

         }
         else
         {
            if (total) sprintf(msg, "%s", mMiscFnx.chrms(tally[0][13], msg));
            else
            {
               if ((data[lev].time_best_all_coins == 0) || (data[lev].status < 2)) strcpy(msg, "n/a");
               else
               {
                  if (data[lev].time_best_all_coins < data[lev].time_par) col = 8;

                  sprintf(msg, "%s", mMiscFnx.chrms(data[lev].time_best_all_coins, msg));
               }
               al_draw_text(mFont.pr8, mColor.pc[col], x+width/2, y, ALLEGRO_ALIGN_CENTER, msg);
            }
         }
         if (print) printf("%-*s", print_width, msg);
      }
      x += width + 8;
      vline[vli++] = x-4;
   }



   if (show_enemies_left)
   {
      width = 8 * 8;
      print_width = 13;
      col = 15;
      if (draw)
      {
         if (header)
         {
            al_draw_text(mFont.pr8, mColor.pc[15], x+width/2, y,   ALLEGRO_ALIGN_CENTER, "Enemies");
            al_draw_text(mFont.pr8, mColor.pc[15], x+width/2, y+8, ALLEGRO_ALIGN_CENTER, "Left");
            strcpy(msg, "Enemies Left");
         }
         else
         {
            if (total) sprintf(msg, "%d", tally[0][14]);
            else       sprintf(msg, "%d", data[lev].min_enemies_left);
            al_draw_text(mFont.pr8, mColor.pc[col], x+width/2, y, ALLEGRO_ALIGN_CENTER, msg);
         }
         if (print) printf("%-*s", print_width, msg);
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
         else            al_draw_textf(mFont.pr8, mColor.pc[15], x+width/2, y,   ALLEGRO_ALIGN_CENTER, "%d", data[lev].times_played);
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
         else            al_draw_textf(mFont.pr8, mColor.pc[15], x+width/2, y,   ALLEGRO_ALIGN_CENTER, "%d", data[lev].times_beat);
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
         else            al_draw_textf(mFont.pr8, mColor.pc[15], x+width/2, y,   ALLEGRO_ALIGN_CENTER, "%d", data[lev].times_quit);
      }
      x += width+8;
      vline[vli++] = x-4;
   }

   if (show_time_played)
   {
      width = 9 * 8;
      print_width = 11;
      col = 15;

      if (draw)
      {
         if (header)
         {
            al_draw_text(mFont.pr8, mColor.pc[15], x+width/2, y,   ALLEGRO_ALIGN_CENTER, "Total");
            al_draw_text(mFont.pr8, mColor.pc[15], x+width/2, y+8, ALLEGRO_ALIGN_CENTER, "Time");
            strcpy(msg, "Total Time");
         }
         else
         {
            if (total) sprintf(msg, "%s", mMiscFnx.chrms(tally[0][10], msg));
            else sprintf(msg, "%s", mMiscFnx.chrms(data[lev].time_total, msg));
            al_draw_text( mFont.pr8, mColor.pc[col], x+width/2, y+y_offset, ALLEGRO_ALIGN_CENTER, msg);


         }
         if (print) printf("%-*s", print_width, msg);


         // show frames
         // if (total) printf(" %d", tally[0][10]);
         // else printf(" %d", data[lev].time_total);

         if (total) printf("Total Time:%d\n", tally[0][10]);




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
         else if (!total) al_draw_text(mFont.pr8, mColor.pc[15], x+width/2, y, ALLEGRO_ALIGN_CENTER, mMiscFnx.chrms(data[lev].time_worst, msg));
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
         else if (!total) al_draw_text(mFont.pr8, mColor.pc[15], x+width/2, y, ALLEGRO_ALIGN_CENTER, mMiscFnx.chrms(data[lev].time_average, msg));
      }
      x += width+8;
      vline[vli++] = x-4;
   }

   if (draw && print) printf("\n");




   if (x > max_x) max_x = x; // max line length

   y+=8; // next line
   if (i == -1) y+=8; // extra line for column header
}

void mwLevel::show_level_stats_title_and_header(int x1, int x2, int& gy, int& ty, int& max_x, int& y, int draw, int col, const char* title)
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
   show_level_stats_row(-1, x1, x2, draw, max_x, y);
   y+=2;
}


void mwLevel::show_level_stats_totals(int x1, int x2, int gy, int ty, int& max_x, int& y, int draw, int col)
{
   show_level_stats_row(100, x1, x2, draw, max_x, y);
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

void mwLevel::show_level_stats_outline_with_thicker_lines(int x1, int x2, int gy, int y, int c)
{
   al_draw_rounded_rectangle(x1, gy-16, x2, y-8, 1, 1, mColor.pc[c], 3);
   al_draw_line(x1, gy, x2, gy, mColor.pc[c], 2);
   al_draw_line(x1, gy+20, x2, gy+20, mColor.pc[c], 2);
   al_draw_line(x1, y-24, x2, y-24, mColor.pc[c], 2);
}


void mwLevel::show_level_stats_grand_total(int x1, int x2, int &y, int draw, int bco, int &gy, int &ty, int &max_x)
{
   for (int i=0; i<16; i++) tally[0][i] = tally[1][i]; // copy totals to group so I can show them
   int ac = 3; // area color
   if (draw) al_draw_filled_rectangle(x1, y, x2, y+50, mColor.pc[ac+bco]); // background color
   show_level_stats_title_and_header(x1, x2, gy, ty, max_x, y, draw, ac, "Grand Totals");
   if (draw) al_draw_line(x1, y, x2, y, mColor.pc[ac], 1);
   y +=4;
   show_level_stats_totals(x1, x2, gy, ty, max_x, y, draw, ac);
   if (draw) show_level_stats_outline_with_thicker_lines(x1, x2, gy, y, ac);
}



void mwLevel::show_level_stats_main_area(int x1, int x2, int &y, int draw, int bco, int &gy, int &ty, int &max_x)
{
   // base y size
   int y_size = 52;

   // for every visible area, add 8 for every level in the area and add 4 for padding
   for (int area=2; area<7; area++)
      if (overworld_area_array[area].visible)
      {
         for (int i=0; i<100; i++)
            if (data[i].area == area) y_size+=8;
         y_size+=4;
      }

   // area color
   int ac = 4;

   // fill with background color
   if (draw) al_draw_filled_rectangle(x1, y, x2, y+y_size, mColor.pc[ac+bco]); // background


   // draw title and column header
   show_level_stats_title_and_header(x1, x2, gy, ty, max_x, y, draw, ac, "Main Game");

   // line before levels
   if (draw) al_draw_line(x1, y, x2, y, mColor.pc[ac], 1);
   y+=4;

   // draw rows
   for (int area=2; area<7; area++)
      if (overworld_area_array[area].visible)
      {
         //int c = overworld_area_array[area].color;

         for (int i=0; i<100; i++)
            if (area_array[i][1] == area)
               show_level_stats_row(area_array[i][0], x1, x2, draw, max_x, y);

         // draw line separating areas
         if (draw) al_draw_line(x1, y+2, x2, y+2, mColor.pc[ac], 1);
         y+=4;
      }

   y+=2;

   // draw totals
   show_level_stats_totals(x1, x2, gy, ty, max_x, y, draw, ac);

   // draw thicker outline
   if (draw) show_level_stats_outline_with_thicker_lines(x1, x2, gy, y, ac);

   // adjust y for next section
   y-=5;
}




void mwLevel::show_level_stats_area(int area, int x1, int x2, int &y, int draw, int bco, int &gy, int &ty, int &max_x)
{
   // base y size
   int y_size = 58;

   // add 8 for every level
   for (int i=0; i<100; i++)
      if (data[i].area == area) y_size+=8;

   // area color
   int ac = overworld_area_array[area].color;

   // fill with background color
   if (draw) al_draw_filled_rectangle(x1, y, x2, y+y_size, mColor.pc[ac+bco]);

   // draw title and column header
   show_level_stats_title_and_header(x1, x2, gy, ty, max_x, y, draw, ac, overworld_area_array[area].name);

   // line before levels
   if (draw) al_draw_line(x1, y, x2, y, mColor.pc[ac], 1);
   y+=4;

   // draw rows
   for (int i=0; i<100; i++)
      if (area_array[i][1] == area)
         show_level_stats_row(area_array[i][0], x1, x2, draw, max_x, y);

   // line after levels
   if (draw) al_draw_line(x1, y+4, x2, y+4, mColor.pc[ac], 1);
   y+=8;

   // draw totals
   show_level_stats_totals(x1, x2, gy, ty, max_x, y, draw, ac);

   // draw thicker outline
   if (draw) show_level_stats_outline_with_thicker_lines(x1, x2, gy, y, ac);

   // adjust y for next section
   y-=5;
}


void mwLevel::show_level_stats(int xc, int y1, int msg_type)
{
    //printf("show_level_stats\n");
   //double t = al_get_time();
   if (level_stats_bmp_msg_type != msg_type)
   {
      // run draw to get w and h
      draw_level_stats(level_stats_bmp_w, level_stats_bmp_h, 0, msg_type);


      // recreate bitmap
      al_destroy_bitmap(level_stats_bitmap);
      level_stats_bitmap = al_create_bitmap(level_stats_bmp_w, level_stats_bmp_h);
      al_set_target_bitmap(level_stats_bitmap);
      al_clear_to_color(mColor.pc[0]);



      // draw
      draw_level_stats(level_stats_bmp_w, level_stats_bmp_h, 1, msg_type);



      level_stats_bmp_msg_type = msg_type;
   }

   if (level_stats_bmp_msg_type == msg_type)
   {
      if (msg_type == 1) al_set_target_backbuffer(mDisplay.display);
      if (msg_type == 3)  al_set_target_bitmap(level_buffer);
      if (msg_type > 20)  al_set_target_bitmap(level_buffer);
      al_draw_bitmap(level_stats_bitmap, xc-level_stats_bmp_w/2, y1, 0);
   }
   //printf("%d - time to show level stats: %f us\n", mLoop.frame_num, (al_get_time() - t)*1000000);
}


void mwLevel::draw_level_stats(int &w, int &h, int draw, int msg_type)
{
   //printf("draw_level_stats draw:%d  type:%d\n", draw, msg_type);


// // debug show all levels in order
//    int jnk;
//    int jnk1;
//    for (int lev=2; lev<100; lev++)
//       show_level_stats_row(lev, 0, 0, 1, jnk, jnk1);



   int x1 = 2;   // these are based on a frame width of 2
   int x2 = w-2;
   int y = 2;

   int ty = 0; // frame and title set ty, then later totals draw text there
   int gy = y; // keeps track of the top of section for drawing frames later

   int max_x = 0;

   vli = 0;
   for (int i=0; i<20; i++)
      vline[i] = 0;

   for (int i=0; i<16; i++)
      for (int j=0; j<2; j++)
         tally[j][i] = 0;


   int bco = 224;  // -16; // + 16 * mLoop.pct_y; // background color offset

   if (msg_type == 1) // all
   {
      row_type = 1;
      show_level_stats_area(1, x1, x2, y, draw, bco, gy, ty, max_x); // training
      show_level_stats_main_area(x1, x2, y, draw, bco, gy, ty, max_x); // main
      show_level_stats_area(7, x1, x2, y, draw, bco, gy, ty, max_x); // extra
      show_level_stats_area(8, x1, x2, y, draw, bco, gy, ty, max_x); // demo
      show_level_stats_grand_total(x1, x2, y, draw, bco, gy, ty, max_x); // grand totals
   }

   if (msg_type == 3) // overworld all
   {
      row_type = 0;
      if (overworld_area_array[1].visible) show_level_stats_area(1, x1, x2, y, draw, bco, gy, ty, max_x); // training

      // only show main if any area in main is visible
      int show_main = 0;
      for (int area=2; area<7; area++)
         if (overworld_area_array[area].visible) show_main = 1;
      if (show_main) show_level_stats_main_area(x1, x2, y, draw, bco, gy, ty, max_x);

//      if (overworld_area_array[7].visible) show_level_stats_area(7, x1, x2, y, draw, bco, gy, ty, max_x); // extra
//      if (overworld_area_array[8].visible) show_level_stats_area(8, x1, x2, y, draw, bco, gy, ty, max_x); // demo
      show_level_stats_grand_total(x1, x2, y, draw, bco, gy, ty, max_x); // grand totals

   }

   if (msg_type > 20) // show single area
   {
      row_type = 0;
      int area = msg_type - 20;
      show_level_stats_area(area, x1, x2, y, draw, bco, gy, ty, max_x);
      y+=5; // add space between sections
   }

   y-=8;

   // set actual w and h
   w = max_x - x1;
   h = y+1;
}
