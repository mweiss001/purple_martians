// mwQuickGraph2.cpp

#include "pm.h"
#include "mwQuickGraph2.h"
#include "mwFont.h"
#include "mwColor.h"


mwQuickGraph2 mQuickGraph2[10];

mwQuickGraph2::mwQuickGraph2()
{


}

void mwQuickGraph2::initialize(int w, int h, float dmin, float dmax, const char* nm, int nm_disp_type, int bgc)
{
   width = w;
   height = h;
   data_min = dmin;
   data_max = dmax;
   data_rng = dmax-dmin;

   sprintf(name, "%s", nm);

   name_display_type = nm_disp_type;


   bg_color = 224 + bgc;

   if (bmp) al_destroy_bitmap(bmp);
   bmp = al_create_bitmap(width, height);
   al_set_target_bitmap(bmp);
   al_clear_to_color(mColor.pc[bg_color]);

   entry_pos = 0;
   last_entry_pos = 0;


   for (int i=0; i<10; i++)
   {
      series[i].active = 0;
      series[i].color = 15;
      series[i].last_data = 0;
   }
}


void mwQuickGraph2::new_entry_pos(void)
{
   last_entry_pos = entry_pos;
   if (++entry_pos > width) entry_pos = 0;
}


void mwQuickGraph2::add_data(int s, float d)
{
   al_set_target_bitmap(bmp);

   int x1 = entry_pos;        // current line
   int x2 = last_entry_pos;   // last line
   int x3 = x1+1;             // next line

   // erase old line
   al_draw_line(x1, 0, x3, height, mColor.pc[bg_color], 1);

   // convert ranges
   float y1 = height - (d -                   data_min) / data_rng * height;
   float y2 = height - (series[s].last_data - data_min) / data_rng * height;

   // draw new line (unless it wraps)
   if (x1 > x2) al_draw_line(x1, y1, x2, y2, mColor.pc[series[s].color], 0);

   series[s].last_data = d;
}

void mwQuickGraph2::draw_graph(int x, int y)
{
   int x1 = 0;         // start
   int x2 = width;     // end
   int xe = entry_pos; // entry position

   // copy [xe-end] - to start
   al_draw_bitmap_region(bmp, xe, 0, x2, height, x, y, 0);

   // get width of that piece
   int xw = x2-xe;

   // copy [start-xe] - to start + xw
   al_draw_bitmap_region(bmp, x1, 0, xe, height, x+xw, y, 0);



   al_draw_rectangle(x, y, x+width, y+height, mColor.pc[15], 1);

   al_draw_textf(mFont.pixl, mColor.pc[15], x+width+2, y-6,          0, "%2.0f", data_max);
   al_draw_textf(mFont.pixl, mColor.pc[15], x+width+2, y+height/2-6, 0, "%2.0f", data_min + data_rng/2);
   al_draw_textf(mFont.pixl, mColor.pc[15], x+width+2, y+height-6,   0, "%0.0f", data_min);

   if (name_display_type == 0) al_draw_textf(mFont.pr8, mColor.pc[15], x+width/2, y+height/2-4,  ALLEGRO_ALIGN_CENTER, "%s", name);
   if (name_display_type == 1) al_draw_textf(mFont.pr8, mColor.pc[15], x+width/2, y+height/2-4,  ALLEGRO_ALIGN_CENTER, "%s:%0.0f%%", name, series[0].last_data);




}

