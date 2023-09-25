// mwQuickGraph2.cpp

#include "pm.h"
#include "mwQuickGraph2.h"
#include "mwFont.h"
#include "mwColor.h"
#include "mwPlayer.h"



mwQuickGraph2 mQuickGraph2[10];

mwQuickGraph2::mwQuickGraph2()
{


}

void mwQuickGraph2::initialize(float w, float h, float dmin, float dmax, const char* nm, int ty, int c1, int c2, int act)
{
   width = w;
   height = h;
   data_min = dmin;
   data_max = dmax;
   data_rng = dmax-dmin;
   type = ty;
   col1 = c1;
   col2 = c2;
   sprintf(name, "%s", nm);

   active = act;


   bco = 224;

   if (bmp) al_destroy_bitmap(bmp);
   bmp = al_create_bitmap(width, height);
   al_set_target_bitmap(bmp);
   al_clear_to_color(mColor.pc[col1+bco]);

   entry_pos = 0;
   last_entry_pos = 0;

   int hist_depth = 8;
   if (type == 9) hist_depth = 200;
   RA.initialize(hist_depth);

   for (int i=0; i<10; i++)
   {
      series[i].active = 0;
      series[i].color = col2;
      series[i].last_data = 0;
   }
}


void mwQuickGraph2::set_size(float w, float h, int rebuild_bitmap)
{
   width = w;
   height = h;

   if (rebuild_bitmap)
   {
      if (bmp) al_destroy_bitmap(bmp);
      bmp = al_create_bitmap(width, height);
      al_set_target_bitmap(bmp);
      al_clear_to_color(mColor.pc[col1+bco]);
   }

}

void mwQuickGraph2::set_pos(float xp, float yp)
{
   x = xp;
   y = yp;
}









void mwQuickGraph2::change_range(float new_min, float new_max)
{
   // redraw existing data....

   // make a temp bitmap and copy existing
   ALLEGRO_BITMAP * temp = al_create_bitmap(width, height);
   al_set_target_bitmap(temp);
   al_clear_to_color(mColor.pc[col1+bco]);
   al_draw_bitmap(bmp, 0, 0, 0);

   // stretch draw tmp to bmp
   al_set_target_bitmap(bmp);
   al_clear_to_color(mColor.pc[col1+bco]);

   // new range and range multiplier
   float new_rng = new_max - new_min;
   float rm = new_rng / data_rng;

   // offset in min_value between old and new
   float min_off = new_min - data_min;

   // value per pixel
   float vpp = height / new_rng;

   float dh = height / rm;                    // new height
   float dy = height - dh + min_off * vpp;    // new y pos to draw at so offsets line up
   al_draw_scaled_bitmap(temp, 0, 0, width, height, 0, dy, width, dh, 0);

   //printf("old_rng:%4.2f  new_rng:%4.2f  rm:%4.3f  dy:%4.2f  dh:%4.2f min_off:%4.2f vpp:%4.2f\n", data_rng, new_rng, rm, dy, dh, min_off, vpp);

   al_destroy_bitmap(temp);

   // apply new values
   data_min = new_min;
   data_max = new_max;
   data_rng = data_max-data_min;
}


void mwQuickGraph2::autorange(void)
{
   if (type == 9) // overlay cpu
   {
      if (data_max == 100)
      {
         if (RA.avg < 40) change_range(0, 50);
      }
      else if (data_max == 50)
      {
         if (RA.avg > 45) change_range(0, 100);
         if (RA.avg < 20) change_range(0, 25);
      }
      else if (data_max == 25)
      {
         if (RA.avg > 20) change_range(0, 50);
      }
   }

   if (type == 2) // dsync
   {
      // only allow change once per s
      if (mPlayer.loc[0].srv_frame_num % 40 == 0)
      {
         int new_min = data_min;
         int new_max = data_max;

         if (RA.mx > data_max) new_max = RA.mx + 5;
         if (RA.mn < data_min) new_min = RA.mn - 5;

         if (RA.mx < data_max - 5) new_max = RA.mx + 5;
         if (RA.mn > data_min + 5) new_min = RA.mn - 5;

         if (new_min < -150) new_min = -80;
         if (new_max > 50) new_max = 20;
         // if ((new_min != data_min) || (new_max != data_max))  change_range(new_min, new_max);
      }
   }
}


void mwQuickGraph2::new_entry_pos(void)
{
   last_entry_pos = entry_pos;
   if (++entry_pos > width) entry_pos = 0;
   autorange();
}





void mwQuickGraph2::add_data(int s, float d)
{
   al_set_target_bitmap(bmp);

   int x1 = entry_pos;        // current line
   int x2 = last_entry_pos;   // last line
   int x3 = x1+1;             // next line





   // erase old line
   al_draw_line(x1, 0, x3, height, mColor.pc[col1+bco], 1);

   // convert ranges
   float y1 = height - (d -                   data_min) / data_rng * height;
   float y2 = height - (series[s].last_data - data_min) / data_rng * height;

   // if data is out of bounds clamp to edges
   if (y1 <= 1) y1 = 2;
   if (y1 >= height) y1 = height-1;

   if (y2 <= 1) y2 = 2;
   if (y2 >= height) y2 = height-1;

   // draw new line (unless it wraps)
   if (x1 > x2) al_draw_line(x1, y1, x2, y2, mColor.pc[series[s].color], 1);

   series[s].last_data = d;

   RA.add_data(d);

}

void mwQuickGraph2::draw_graph(void)
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


   al_draw_rectangle(x, y, x+width, y+height, mColor.pc[col1], 1);

   al_draw_textf(mFont.pixl, mColor.pc[15], x+width+2, y-6,          0, "%2.0f", data_max);
   al_draw_textf(mFont.pixl, mColor.pc[15], x+width+2, y+height/2-6, 0, "%2.0f", data_min + data_rng/2);
   al_draw_textf(mFont.pixl, mColor.pc[15], x+width+2, y+height-6,   0, "%0.0f", data_min);

   if (type == 0) al_draw_textf(mFont.pr8, mColor.pc[col2], x+width/2, y+height/2-4,  ALLEGRO_ALIGN_CENTER, "%s", name);

   if (type == 1) al_draw_textf(mFont.pr8, mColor.pc[col2], x+width/2, y+height/2-4,  ALLEGRO_ALIGN_CENTER, "%s:%0.0f%%", name, series[0].last_data);
   if (type == 9) al_draw_textf(mFont.pr8, mColor.pc[col2], x+width/2, y+height/2-4,  ALLEGRO_ALIGN_CENTER, "%s:%0.0f%%", name, series[0].last_data);



//   if (type == 1) al_draw_textf(mFont.pr8, mColor.pc[15], x+width/2, y+height/2-4+10,  ALLEGRO_ALIGN_CENTER, "MN:%4.2f MX:%4.2f AV:%4.2f", RA.mn, RA.mx, RA.avg );


}

