// mwDrawSequence.cpp

#include "pm.h"
#include "z_log.h"
#include "z_player.h"
#include "mwDrawSequence.h"
#include <limits>

#include "mwRollingAverage.h"


mwDrawSequence mwDS;

mwDrawSequence::mwDrawSequence()
{
   initialize();
}

void mwDrawSequence::initialize(void)
{
   for (int i=0; i<20; i++)
   {
      seq[i][0] = 1; // always on now
      seq[i][1] = 0;
      time_cur[i] = 0;
      time_avg[i] = 0;
      time_min[i] = 0;
      time_max[i] = 0;
      RA[i].initialize();
   }

   sprintf(name_long[0], "get_new_background");
   sprintf(name_long[1], "draw_lifts");
   sprintf(name_long[2], "draw_items");
   sprintf(name_long[3], "draw_enemies");
   sprintf(name_long[4], "draw_ebullets");
   sprintf(name_long[5], "draw_pbullets");
   sprintf(name_long[6], "draw_players");
   sprintf(name_long[7], "get_new_screen_buffer");
   sprintf(name_long[8], "draw_screen_overlay");
   sprintf(name_long[9], "al_flip_display");
   sprintf(name_long[10], "total draw time");
}

void mwDrawSequence::add(int s, double v)
{
   RA[s].add_data(v);
   // calc();
}


void mwDrawSequence::calc(void)
{
   // if the total draw time > 70% cpu or 18ms then split into two groups

   // test static split

   // 0 = every frame
   // 1 = odd only
   // 2 - even only

   // these are always on
   seq[0][1] = 0;
   seq[7][1] = 0;
   seq[9][1] = 0;

   // statically set mode to odd or even
   seq[1][1] = 0;
   seq[2][1] = 0;
   seq[3][1] = 0;
   seq[4][1] = 0;
   seq[5][1] = 0;
   seq[6][1] = 0;
   seq[8][1] = 0;

   // set actual based on current frame num
   int oe = frame_num % 2; // odd/even  0 = even 1 = odd

   for (int i=0; i<10; i++)
   {
      seq[i][0] = 1;                                    // on by default
      if ((seq[i][1] == 1) && (oe == 0)) seq[i][0] = 0; // odd draw only
      if ((seq[i][1] == 2) && (oe == 1)) seq[i][0] = 0; // even draw only
   }

}

void mwDrawSequence::draw(void)
{
   double t1, t2, t3, t4, t5, t6, t7, t8, t9, t10;
   t0 = al_get_time();

   if (seq[0][0]) get_new_background(1);
   t1 = al_get_time(); add(0, t1-t0);

   if (seq[1][0]) draw_lifts();
   t2 = al_get_time(); add(1, t2-t1);

   if (seq[2][0]) draw_items();
   t3 = al_get_time(); add(2, t3-t2);

   if (seq[3][0]) draw_enemies();
   t4 = al_get_time(); add(3, t4-t3);

   if (seq[4][0]) draw_ebullets();
   t5 = al_get_time(); add(4, t5-t4);

   if (seq[5][0]) draw_pbullets();
   t6 = al_get_time(); add(5, t6-t5);

   if (seq[6][0]) draw_players();
   t7 = al_get_time(); add(6, t7-t6);

   get_new_screen_buffer(0, 0, 0);
   t8 = al_get_time(); add(7, t8-t7);

   if (seq[8][0]) draw_screen_overlay();
   t9 = al_get_time(); add(8, t9-t8);

   al_flip_display();
   t10 = al_get_time(); add(9, t10-t9); add(10, t10-t0);

   if (LOG_TMR_draw_all)
   {
      sprintf(msg, "tmst d-bkgr:[%0.4f] d-lift:[%0.4f] d-item:[%0.4f] d-enem:[%0.4f] d-ebul:[%0.4f] d-pbul:[%0.4f] d-plyr:[%0.4f] d-buff:[%0.4f] d-ovrl:[%0.4f] d-flip:[%0.4f] d-totl:[%0.4f]\n",
      (t1-t0)*1000, (t2-t1)*1000, (t3-t2)*1000, (t4-t3)*1000, (t5-t4)*1000, (t6-t5)*1000, (t7-t6)*1000, (t8-t7)*1000, (t9-t8)*1000, (t10-t9)*1000, (t10-t0)*1000);
      //printf("\n%s\n", msg);
      add_log_entry2(44, 0, msg);
   }
   if (LOG_TMR_draw_tot) add_log_TMR(t10-t0, "draw", 0);
}

char * mwDrawSequence::get_line(int s)
{
   sprintf(msg, "%3.3f - %s", RA[s].avg*1000, name_long[s]);
   return msg;
}

void mwDrawSequence::show_text(int x, int y)
{
   al_draw_filled_rectangle(x, y, x+240, y+100, palette_color[0]);
   for (int i=0; i<11; i++)
      al_draw_textf(font, palette_color[15], 1+x, 1+y+i*9, 0, "%s", get_line(i));

//
//   al_draw_filled_rectangle(x, y, x+340, y+90, palette_color[0]);
//
//   double tot = 0;
//   int col = 15;
//   // show both
//   for (int i=0; i<10; i++)
//   {
//      col = 15;
//      if ((i == 0) || (i == 7) || (i == 9)) col = 13;
//      tot += RA[i].avg;
//      al_draw_textf(font, palette_color[col], x, y+i*8, 0, "%3.3f", RA[i].avg*1000);
//   }
//   al_draw_textf(font, palette_color[15], x, y+10*8, 0, "%3.3f", tot*1000);
//
//   // show odd only
//   tot = 0;
//   for (int i=0; i<10; i++)
//   {
//      col = 15;
//      if ((i == 0) || (i == 7) || (i == 9)) col = 13;
//      if ((seq[i][1] == 1) || (col == 13))
//      {
//         al_draw_textf(font, palette_color[col], x+60, y+i*8, 0, "%3.3f", RA[i].avg*1000);
//         tot += RA[i].avg;
//      }
//   }
//   al_draw_textf(font, palette_color[15], x+60, y+10*8, 0, "%3.3f", tot*1000);
//
//   // show even only
//   tot = 0;
//   for (int i=0; i<10; i++)
//   {
//      col = 15;
//      if ((i == 0) || (i == 7) || (i == 9)) col = 13;
//      if ((seq[i][1] == 2) || (col == 13))
//      {
//         al_draw_textf(font, palette_color[col], x+120, y+i*8, 0, "%3.3f", RA[i].avg*1000);
//         tot += RA[i].avg;
//      }
//   }
//   al_draw_textf(font, palette_color[15], x+120, y+10*8, 0, "%3.3f", tot*1000);
//
//
//   // show names
//   for (int i=0; i<11; i++)
//   {
//      col = 15;
//      if ((i == 0) || (i == 7) || (i == 9)) col = 13;
//      al_draw_textf(font, palette_color[col], x+180, y+i*8, 0, "- %s", name_long[i]);
//   }
//








}


