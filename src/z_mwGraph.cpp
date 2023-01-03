// z_mwGraph.cpp

#include "pm.h"


mwGraph::mwGraph()
{
   initialize();
}

void mwGraph::initialize(void)
{

   screen_x1 = 0;
   screen_y1 = 0;
   screen_x2 = 0;
   screen_y2 = 0;

   for (int i=0; i<10000; i++)
   {
      data[i][0] = 0;
      data[i][1] = 0;
   }
   data_points = 0;

   x_data_min = 0;
   x_data_max = 0;
   x_data_rng = 0;
   y_data_min = 0;
   y_data_max = 0;
   y_data_rng = 0;

   x_axis_min = 0;
   x_axis_max = 0;
   x_axis_rng = 0;
   y_axis_min = 0;
   y_axis_max = 0;
   y_axis_rng = 0;

   x_axis_lock_scroll = 0;
   x_axis_lock_zoom = 0;

   y_axis_lock_scroll = 0;
   y_axis_zoom_lock = 0;

}


void mwGraph::draw()
{
   // erase background
   al_draw_filled_rectangle(screen_x1, screen_y1, screen_x2, screen_y2, palette_color[0]);

   // frame
   al_draw_rectangle(screen_x1, screen_y1, screen_x2, screen_y2, palette_color[15], 1);

   if (show_title)
   {
      int xc = screen_x1+screen_w/2;
      int tl = 8 + strlen(title)*8;
      al_draw_textf(font, palette_color[title_color], xc, screen_y1-11, ALLEGRO_ALIGN_CENTER, "%s", title);
      al_draw_rounded_rectangle(xc - tl/2, screen_y1-14, xc + tl/2, screen_y1-1, 1, 1, palette_color[15], 1);
   }

   label_axis();

//   al_draw_filled_rectangle(              10, 10, 600, 30, palette_color[0]); // erase background
//   al_draw_textf(font, palette_color[15], 10, 10, 0, "x_axis_min:%5.1f  x_axis_max:%5.1f  x_axis_rng:%5.1f", x_axis_min, x_axis_max, x_axis_rng);
//   al_draw_textf(font, palette_color[15], 10, 18, 0, "y_axis_min:%5.1f  y_axis_max:%5.1f  y_axis_rng:%5.1f", y_axis_min, y_axis_max, y_axis_rng);

   int col = 10;
   float x=0, ox=0, y=0, oy=0;
   bool first_time = 1;
   al_set_clipping_rectangle(screen_x1 * display_transform_double, screen_y1 * display_transform_double, screen_w * display_transform_double, screen_h * display_transform_double);
   for (int i=0; i<data_points; i++)
   {
      x = screen_x1 + (data[i][0]-x_axis_min) * screen_w / x_axis_rng;
      y = screen_y2 - (data[i][1]-y_axis_min) * screen_h / y_axis_rng;
      if (first_time)
      {
         first_time = 0;
         ox = x;
         oy = y;
      }
      al_draw_line(x, y, ox, oy, palette_color[col], 1);
      al_draw_filled_circle(x, y, 2, palette_color[col]);
      ox = x;
      oy = y;
   }
   al_reset_clipping_rectangle();

}


void mwGraph::autorange_axis(int x, int y)
{
   if (x)
   {
      x_axis_min = x_data_min;
      x_axis_max = x_data_max;
      x_axis_rng = x_axis_max - x_axis_min;
   }
   if (y == 1)
   {
      y_axis_min = y_data_min;
      y_axis_max = y_data_max;
      y_axis_rng = y_axis_max - y_axis_min;
   }
   if (y == 2)
   {
      // get min and max of displayed date

      double y_min = 0;
      double y_max = 0;
      double y_rng = 0;

      if (data_points)
      {

         int matched = 0;

         for (int i=0; i<data_points; i++)
            if ((data[i][0] >= x_axis_min) && (data[i][0] <= x_axis_max))
            {
               if (matched == 0) y_min = y_max = data[i][1];
               if (data[i][1] < y_min) y_min = data[i][1];
               if (data[i][1] > y_max) y_max = data[i][1];
               matched++;
            }

         if (matched > 1) // don't change anything if less than 2 data points in range
         {
            y_rng = y_max - y_min;
            y_axis_min = y_min - y_rng*.05;
            y_axis_max = y_max + y_rng*.05;
            y_axis_rng = y_axis_max - y_axis_min;

            enforce_axis_limits();
         }
      }
   }
}

void mwGraph::set_range_axis(double x_min, double x_max, double y_min, double y_max)
{
   x_axis_min = x_min;
   x_axis_max = x_max;
   y_axis_max = y_max;
   y_axis_min = y_min;
   enforce_axis_limits();
}



void mwGraph::enforce_axis_limits(int type)
{
   if (type == 1) // ensure that axis_min is always less than axis_max
   {
      if (x_axis_min > x_axis_max)
      {
         double tmp = x_axis_min;
         x_axis_min = x_axis_max;
         x_axis_max = tmp;
      }
      if (y_axis_min > y_axis_max)
      {
         double tmp = y_axis_min;
         y_axis_min = y_axis_max;
         y_axis_max = tmp;
      }
   }
   if (type == 2) // ensure that neither axis_min or axis_max is beyond data range
   {
      double xll = x_data_min; // lower limit
      double xul = x_data_max; // upper limit
      if (x_axis_min < xll) x_axis_min = xll;
      if (x_axis_max < xll) x_axis_max = xll;
      if (x_axis_min > xul) x_axis_min = xul;
      if (x_axis_max > xul) x_axis_max = xul;

      double yll = y_data_min; // lower limit
      double yul = y_data_max; // upper limit
      if (y_axis_min < yll) y_axis_min = yll;
      if (y_axis_max < yll) y_axis_max = yll;
      if (y_axis_min > yul) y_axis_min = yul;
      if (y_axis_max > yul) y_axis_max = yul;
   }
   if (type == 3) // set range
   {
      x_axis_rng = x_axis_max - x_axis_min;
      y_axis_rng = y_axis_max - y_axis_min;
   }
}



void mwGraph::enforce_axis_limits(void)
{
   x_axis_rng_min = 10;
   y_axis_rng_min = 10;

   enforce_axis_limits(1); // reverse order
   enforce_axis_limits(2); // min max limits
   enforce_axis_limits(3); // set range

   if (x_axis_rng < x_axis_rng_min)
   {
      int done = 0;
      // try to adjust min first
      if ((!done) && ((x_axis_min - x_axis_rng_min) >= x_data_min))
      {
         x_axis_min -= x_axis_rng_min;
         done = 1;
      }
      // if that didn't work, try adjusting max
      if ((!done) && ((x_axis_max + x_axis_rng_min) <= x_data_max))
      {
         x_axis_max += x_axis_rng_min;
         done = 1;
      }
      enforce_axis_limits(); // set and test again
   }
}




int mwGraph::convert_gxy_to_sxy(double gx, double gy, int &sx, int &sy)
{
   sx = screen_x1 + ((gx - x_axis_min) / x_axis_rng)*screen_w;
   sy = screen_y2 - ((gy - y_axis_min) / y_axis_rng)*screen_h;

   if ((sx > screen_x1) && (sx < screen_x2) && (sy > screen_y1) && (sy < screen_y2)) return 1;
   return 0; // sx, sy  is not on graph
}

int mwGraph::convert_sxy_to_gxy(int sx, int sy, double &gx, double &gy)
{
   if ((sx > screen_x1) && (sx < screen_x2) && (sy > screen_y1) && (sy < screen_y2))
   {
      // first lose the screen offset and invert y
      double mx1 = mouse_x - screen_x1;
      double my1 = screen_h - (mouse_y - screen_y1);

      gx = (mx1 / screen_w) * x_axis_rng + x_axis_min;
      gy = (my1 / screen_h) * y_axis_rng + y_axis_min;
      return 1;
   }
   return 0; // sx, sy  is not on graph
}

// if sx,sy is out of bounds, set gx, gy to limits
void mwGraph::convert_sxy_to_gxy_clamp(int sx, int sy, double &gx, double &gy)
{
   // first lose the screen offset and invert y
   double mx1 = mouse_x - screen_x1;
   double my1 = screen_h - (mouse_y - screen_y1);

   gx = (mx1 / screen_w) * x_axis_rng + x_axis_min;
   gy = (my1 / screen_h) * y_axis_rng + y_axis_min;

   if (gx < x_axis_min) gx = x_axis_min;
   if (gx > x_axis_max) gx = x_axis_max;

   if (gy < y_axis_min) gy = y_axis_min;
   if (gy > y_axis_max) gy = y_axis_max;
}

double round_to_nearest(double in, double nr)
{
   double tmp = fmod(in, nr);
   return nr + (in - tmp);
}

void mwGraph::label_axis(void)
{
   // label axis
//   al_draw_textf(f3, palette_color[15], screen_x1, screen_y2+2, ALLEGRO_ALIGN_CENTER, "%3.0f", x_axis_min);
//   al_draw_textf(f3, palette_color[15], screen_x2, screen_y2+2, ALLEGRO_ALIGN_CENTER, "%3.0f", x_axis_max);

//   al_draw_textf(f3, palette_color[15], screen_x1-12, screen_y2-6, 0, "%-3.0f", y_axis_min);
//   al_draw_textf(f3, palette_color[15], screen_x1-12, screen_y1-6, 0, "%-3.0f", y_axis_max);


   // draw gridlines


   // how many to draw?
   // pick a span and see how many...
   double x_gl_span = .00001;
   while (x_axis_rng/x_gl_span > 40) x_gl_span *=10;

//   al_draw_textf(font, palette_color[15], screen_x1-12, screen_y2+40, 0, "gspan:%-3.5f", x_gl_span);

   // erase background
   al_draw_filled_rectangle(screen_x1-10, screen_y2+1, screen_x2+10, screen_y2+11, palette_color[0]);

   double gx2 = round_to_nearest(x_axis_min, x_gl_span);
//   al_draw_textf(font, palette_color[15], screen_x1-12, screen_y2+48, 0, "x_axis_min:%f gx2:%f", x_axis_min, gx2);
   for (double i=gx2; i<=x_axis_max; i+=x_gl_span)
   {
      int lx = 0, ly = 0;
      convert_gxy_to_sxy(i, 9999, lx, ly);
      al_draw_line(lx, screen_y1, lx, screen_y2+4, palette_color[15+128], 1);
      al_draw_textf(f3, palette_color[15], lx, screen_y2+2, ALLEGRO_ALIGN_CENTER, "%3.0f", i);
   }

//
//   double gx2 = round_to_nearest(x_axis_min, x_gl_span);
//   al_draw_textf(font, palette_color[15], screen_x1-12, screen_y2+48, 0, "x_axis_min:%f gx2:%f", x_axis_min, gx2);
//   for (double i=gx2; i<x_axis_max; i+=x_gl_span)
//   {
//      int lx = 0, ly = 0;
//      convert_gxy_to_sxy(i, 9999, lx, ly);
//      al_draw_line(lx, screen_y1, lx, screen_y2+4, palette_color[15+128], 1);
//      al_draw_textf(f3, palette_color[15], lx, screen_y2+2, ALLEGRO_ALIGN_CENTER, "%3.0f", i);
//   }
//
//

   double y_gl_span = .00001;
   while (y_axis_rng/y_gl_span > 40) y_gl_span *=10;

   // erase background
   al_draw_filled_rectangle(screen_x1-40, screen_y1-8, screen_x1-1, screen_y2+11, palette_color[0]);


//   y_gl_span *=10;
   double gy2 = round_to_nearest(y_axis_min, y_gl_span);
   for (double i=gy2; i<y_axis_max; i+=y_gl_span)
   {
      int lx = 0, ly = 0;
      convert_gxy_to_sxy(9999, i, lx, ly);
      al_draw_line(screen_x1-4, ly, screen_x2, ly, palette_color[15+128], 0);
      al_draw_textf(f3, palette_color[15], screen_x1-18, ly-7, ALLEGRO_ALIGN_CENTER, "%6.0f", i);
   }

//   y_gl_span /=10;

//   gy2 = round_to_nearest(y_axis_min, y_gl_span);
//   for (double i=gy2; i<y_axis_max; i+=y_gl_span)
//   {
//      int lx = 0, ly = 0;
//      convert_gxy_to_sxy(9999, i, lx, ly);
//      al_draw_line(screen_x1-4, ly, screen_x2, ly, palette_color[15+128+64], 0);
//      al_draw_textf(f3, palette_color[15], screen_x1-18, ly-7, ALLEGRO_ALIGN_CENTER, "%6.0f", i);
//   }
//





}

void mwGraph::calc_data_range(void)
{
   x_data_min = x_data_max = x_data_rng = 0;
   y_data_min = y_data_max = y_data_rng = 0;

   if (data_points)
   {
      x_data_min = x_data_max = data[0][0];
      y_data_min = y_data_max = data[0][1];

      for (int i=0; i<data_points; i++)
      {
         if (data[i][0] < x_data_min) x_data_min = data[i][0];
         if (data[i][0] > x_data_max) x_data_max = data[i][0];
         if (data[i][1] < y_data_min) y_data_min = data[i][1];
         if (data[i][1] > y_data_max) y_data_max = data[i][1];
      }
   }
   x_data_rng = x_data_max - x_data_min;
   y_data_rng = y_data_max - y_data_min;
}

void mwGraph::set_screen_pos(int x1, int y1, int x2, int y2)
{
   screen_x1 = x1;
   screen_y1 = y1;

   screen_x2 = x2;
   screen_y2 = y2;

   screen_w = screen_x2 - screen_x1;
   screen_h = screen_y2 - screen_y1;
}

void mwGraph::add_data_point(double x, double y)
{
   data[data_points][0] = x;
   data[data_points][1] = y;
   data_points++;
}

void mwGraph::fill_test_data(int type)
{
   if (type == 1)
   {
      for (int i=0; i<50; i++) add_data_point(i-25, i*2+25);
   }
}




void mwGraph::adjust_axis(int xy, int nx, int id)
{
   if (xy) // x axis
   {
      if (nx) // min
      {
         if (id) set_range_axis(x_axis_min*=1.1, x_axis_max, y_axis_min, y_axis_max); // increment
            else set_range_axis(x_axis_min*=0.9, x_axis_max, y_axis_min, y_axis_max); // decrement
      }
      else // max
      {
         if (id) set_range_axis(x_axis_min, x_axis_max*=1.1, y_axis_min, y_axis_max); // increment
         else    set_range_axis(x_axis_min, x_axis_max*=0.9, y_axis_min, y_axis_max); // decrement
      }
   }
   else // y axis
   {
   }
}








int mwGraph::draw_x_scrollbar(void)
{
   int sbx1 = x_axis_scrollbar_x1;
   int sbx2 = x_axis_scrollbar_x2;
   int sbw  = x_axis_scrollbar_w;
   int sby1 = x_axis_scrollbar_y1;
   int sby2 = x_axis_scrollbar_y2;

   // erase and frame
   al_draw_filled_rectangle(sbx1, sby1, sbx2, sby2, palette_color[0]);
   al_draw_rectangle(       sbx1, sby1, sbx2, sby2, palette_color[15+96], 1);

   // show current view vs entire data set
   // get ratio of x_axis_min to x_data_rng, muliply by scroll bar width and add scroolbar offset
   x_axis_scrollbar_bar_x1 = ((x_axis_min - x_data_min) / x_data_rng) * sbw + sbx1;
   x_axis_scrollbar_bar_x2 = ((x_axis_max - x_data_min) / x_data_rng) * sbw + sbx1;

   int sbbx1 = x_axis_scrollbar_bar_x1;
   int sbbx2 = x_axis_scrollbar_bar_x2;

   int mx = mouse_x;
   int my = mouse_y;

   al_draw_filled_rectangle(sbbx1+1, sby1+1, sbbx2-1, sby2-1, palette_color[15+160]);

   if ((my > sby1) && (my < sby2))
   {
      if ((mx > sbx1)    && (mx < sbx2))
      {
         al_draw_rectangle(sbx1, sby1, sbx2, sby2, palette_color[15], 1); // on entire scrollbar
         if (mouse_b[2][0]) return 1;
      }
      if ((mx > sbbx1)   && (mx < sbbx1+5)) { al_draw_rectangle(sbbx1, sby1, sbbx1, sby2, palette_color[15], 1); al_set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_RESIZE_W); return 6; } // on scrollbar bar min adjust
      if ((mx < sbbx2)   && (mx > sbbx2-5)) { al_draw_rectangle(sbbx2, sby1, sbbx2, sby2, palette_color[15], 1); al_set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_RESIZE_W); return 5; } // on scrollbar bar max adjust
      if ((mx < sbbx1+1) && (mx > sbx1))    { al_draw_rectangle(mx,    sby1, mx,    sby2, palette_color[15], 1); return 4; } // on scrollbar between bar and data min
      if ((mx > sbbx2-1) && (mx < sbx2))    { al_draw_rectangle(mx,    sby1, mx,    sby2, palette_color[15], 1); return 3; } // on scrollbar between bar and data max
      if ((mx > sbbx1)   && (mx < sbbx2))   { al_draw_rectangle(sbbx1, sby1, sbbx2, sby2, palette_color[15], 1); al_set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_MOVE); return 2; } // on scrollbar bar
   }
   return 0;
}

void mwGraph::proc_x_scrollbar(void)
{
   x_axis_scrollbar_x1 = screen_x1;
   x_axis_scrollbar_x2 = screen_x2;
   x_axis_scrollbar_w  = screen_w;
   x_axis_scrollbar_y1 = screen_y2+32;
   x_axis_scrollbar_y2 = screen_y2+48;

   int sb = draw_x_scrollbar();

   if (sb == 1)
   {
      sprintf(global_string[6][0],"X Axis Menu");
      sprintf(global_string[6][1],"---------------");
      sprintf(global_string[6][2],"Reset X Axis Range");
      sprintf(global_string[6][3],"Reset Entire Graph");
//      if (y_axis_zoom_lock == 1) sprintf(global_string[6][4],"Lock Range:ON");
//      else                       sprintf(global_string[6][4],"Lock Range:OFF");
//      if (y_axis_zoom_lock == 2) sprintf(global_string[6][5],"Auto Range:ON");
//      else                       sprintf(global_string[6][5],"Auto Range:OFF");
      sprintf(global_string[6][4],"end");

      switch (pmenu(6, 13))
      {
         case 2: autorange_axis(1, 0); break;
         case 3: autorange_axis(1, 1); break;

//         case 4: // lock and unlock
//            if      (y_axis_zoom_lock == 1) y_axis_zoom_lock = 0; // if locked then unlock
//            else if (y_axis_zoom_lock == 0) y_axis_zoom_lock = 1; // if unlocked then lock
//         break;
//
//         case 5: // auto on and off
//            if   (y_axis_zoom_lock == 2) y_axis_zoom_lock = 0; // if auto then unlock
//            else                         y_axis_zoom_lock = 2; // if anything else then set auto
//         break;
      }
   }














   if (mouse_b[1][0])
   {
      double old_x_axis_min = x_axis_min;
      double old_x_axis_max = x_axis_max;
      double x_scaler = x_data_rng / x_axis_scrollbar_w;
      int rmx = mouse_x;

      if (sb == 2) // drag adjust axis mix and max
      {
         while (mouse_b[1][0])
         {
            double mdx = (rmx - mouse_x) * x_scaler;
            x_axis_min = old_x_axis_min - mdx;
            x_axis_max = old_x_axis_max - mdx;
            enforce_axis_limits();
            draw();
            draw_x_scrollbar();
            al_flip_display();
            proc_controllers();
         }
      }
      if (sb == 6) // drag adjust axis_min
      {
         while (mouse_b[1][0])
         {
            double mdx = (rmx - mouse_x) * x_scaler;
            x_axis_min = old_x_axis_min - mdx;
            enforce_axis_limits();
            draw();
            draw_x_scrollbar();
            al_flip_display();
            proc_controllers();
         }
      }
      if (sb == 5) // drag adjust axis_max
      {
         while (mouse_b[1][0])
         {
            double mdx = (rmx - mouse_x) * x_scaler;
            x_axis_max = old_x_axis_max - mdx;
            enforce_axis_limits();
            draw();
            draw_x_scrollbar();
            al_flip_display();
            proc_controllers();
         }
      }
      if (sb == 4) // set new pos (jump to new pos between data_min and axis_min )
      {
         double mdx = (x_axis_scrollbar_bar_x1 - mouse_x) * x_scaler;
         x_axis_min = old_x_axis_min - mdx;
         x_axis_max = old_x_axis_max - mdx;
         enforce_axis_limits();
      }
      if (sb == 3) // adjust x_axis_max
      {
         double mdx = (x_axis_scrollbar_bar_x2 - mouse_x) * x_scaler;
         x_axis_max = old_x_axis_max - mdx;
         enforce_axis_limits();
      }
   }
}



int mwGraph::draw_y_scrollbar(void)
{
   int sby1 = y_axis_scrollbar_y1;
   int sby2 = y_axis_scrollbar_y2;
   int sbh  = y_axis_scrollbar_h;
   int sbx1 = y_axis_scrollbar_x1;
   int sbx2 = y_axis_scrollbar_x2;

   // erase and frame
   al_draw_filled_rectangle(sbx1, sby1, sbx2, sby2, palette_color[0]);
   al_draw_rectangle(       sbx1, sby1, sbx2, sby2, palette_color[15+96], 1);

   // show current view vs entire data set
   // get ratio of x_axis_min to x_data_rng, muliply by scroll bar width and add scroolbar offset
   y_axis_scrollbar_bar_y1 = sby2 - ((y_axis_max - y_data_min) / y_data_rng) * sbh;
   y_axis_scrollbar_bar_y2 = sby2 - ((y_axis_min - y_data_min) / y_data_rng) * sbh; // min data

   int sbby1 = y_axis_scrollbar_bar_y1;
   int sbby2 = y_axis_scrollbar_bar_y2;

   int mx = mouse_x;
   int my = mouse_y;

   al_draw_filled_rectangle(sbx1+1, sbby1+1, sbx2-1, sbby2-1, palette_color[15+160]);

   if ((mx > sbx1) && (mx < sbx2))
   {
      if ((my > sby1)    && (my < sby2))
      {
         al_draw_rectangle(sbx1, sby1,  sbx2, sby2,  palette_color[15], 1); // on entire scrollbar
         if (mouse_b[2][0]) return 1;
      }
      if ((my > sbby2-5) && (my < sbby2) && (!y_axis_zoom_lock)) { al_draw_rectangle(sbx1, sbby2, sbx2, sbby2, palette_color[15], 1); al_set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_RESIZE_N); return 6; } // on scrollbar bar min adjust
      if ((my < sbby1+5) && (my > sbby1) && (!y_axis_zoom_lock)) { al_draw_rectangle(sbx1, sbby1, sbx2, sbby1, palette_color[15], 1); al_set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_RESIZE_N); return 5; } // on scrollbar bar max adjust
      if ((my > sbby2-1) && (my < sby2))  { al_draw_rectangle(sbx1, my,    sbx2, my,    palette_color[15], 1); return 4; } // on scrollbar between bar and data min
      if ((my < sbby1+1) && (my > sby1)  && (!y_axis_zoom_lock)) { al_draw_rectangle(sbx1, my,    sbx2, my,    palette_color[15], 1); return 3; } // on scrollbar between bar and data max
      if ((my < sbby2)   && (my > sbby1)) { al_draw_rectangle(sbx1, sbby1, sbx2, sbby2, palette_color[15], 1); al_set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_MOVE); return 2; }     // on scrollbar bar
   }
   return 0;
}

void mwGraph::proc_y_scrollbar(void)
{
   y_axis_scrollbar_x1 = screen_x1 - 80;
   y_axis_scrollbar_x2 = screen_x1 - 60;
   y_axis_scrollbar_y1 = screen_y1;
   y_axis_scrollbar_y2 = screen_y2;
   y_axis_scrollbar_h  = screen_h;

   if (y_axis_zoom_lock == 2) autorange_axis(0, 2);

   int ya = y_axis_scrollbar_y2+4;
   int yb = y_axis_scrollbar_x2 - y_axis_scrollbar_x1; // to make it square
   int xa = y_axis_scrollbar_x1;
   int xb = y_axis_scrollbar_x2;
   if (y_axis_zoom_lock == 0) sprintf(msg, "U");
   if (y_axis_zoom_lock == 1) sprintf(msg, "L");
   if (y_axis_zoom_lock == 2) sprintf(msg, "A");
   if (mdw_buttont(xa, ya, xb, yb, 0,0,0,0,   0,15+64,15, 0,  1,0,1,0, msg)) y_axis_zoom_lock++;
   if (y_axis_zoom_lock > 2) y_axis_zoom_lock = 0;

   int sb = draw_y_scrollbar();

   if (sb == 1)
   {
      sprintf(global_string[6][0],"Y Axis Menu");
      sprintf(global_string[6][1],"---------------");
      sprintf(global_string[6][2],"Reset Y Axis Range");
      sprintf(global_string[6][3],"Reset Entire Graph");
      if (y_axis_zoom_lock == 1) sprintf(global_string[6][4],"Lock Range:ON");
      else                       sprintf(global_string[6][4],"Lock Range:OFF");
      if (y_axis_zoom_lock == 2) sprintf(global_string[6][5],"Auto Range:ON");
      else                       sprintf(global_string[6][5],"Auto Range:OFF");
      sprintf(global_string[6][6],"end");

      switch (pmenu(6, 13))
      {
         case 2: autorange_axis(0, 1); break;
         case 3: autorange_axis(1, 1); break;

         case 4: // lock and unlock
            if      (y_axis_zoom_lock == 1) y_axis_zoom_lock = 0; // if locked then unlock
            else if (y_axis_zoom_lock == 0) y_axis_zoom_lock = 1; // if unlocked then lock
         break;

         case 5: // auto on and off
            if   (y_axis_zoom_lock == 2) y_axis_zoom_lock = 0; // if auto then unlock
            else                         y_axis_zoom_lock = 2; // if anything else then set auto
         break;
      }
   }


   if (mouse_b[1][0])
   {
      double old_y_axis_min = y_axis_min;
      double old_y_axis_max = y_axis_max;
      double y_scaler = y_data_rng / y_axis_scrollbar_h;
      int rmy = mouse_y;

      if (sb == 2) // drag adjust axis mix and max
      {
         while (mouse_b[1][0])
         {
            double mdy = (mouse_y - rmy) * y_scaler;
            double new_y_axis_min = old_y_axis_min - mdy;
            double new_y_axis_max = old_y_axis_max - mdy;

            if (y_axis_zoom_lock)
            {
               if (new_y_axis_min < y_data_min) // tried to scroll past edge
               {
                  new_y_axis_min = y_data_min;
                  new_y_axis_max = new_y_axis_min + y_axis_rng;
               }
               if (new_y_axis_max > y_data_max) // tried to scroll past edge
               {
                  new_y_axis_max = y_data_max;
                  new_y_axis_min = new_y_axis_max - y_axis_rng;
               }
            }
            y_axis_min = new_y_axis_min;
            y_axis_max = new_y_axis_max;

            enforce_axis_limits();
            draw();
            draw_y_scrollbar();
            al_flip_display();
            proc_controllers();
         }
      }
      if (sb == 6) // drag adjust axis_min
      {
         while (mouse_b[1][0])
         {
            double mdy = (mouse_y - rmy) * y_scaler;
            y_axis_min = old_y_axis_min - mdy;

            enforce_axis_limits();
            draw();
            draw_y_scrollbar();
            al_flip_display();
            proc_controllers();
         }
      }
      if (sb == 5) // drag adjust axis_min
      {
         while (mouse_b[1][0])
         {
            double mdy = (mouse_y - rmy) * y_scaler;
            y_axis_max = old_y_axis_max - mdy;

            enforce_axis_limits();
            draw();
            draw_y_scrollbar();
            al_flip_display();
            proc_controllers();
         }
      }
      if (sb == 4) // set new pos (jump to new pos between data_min and axis_min )
      {
         double mdy = (mouse_y - y_axis_scrollbar_bar_y2) * y_scaler;
         y_axis_min = old_y_axis_min - mdy;
         y_axis_max = old_y_axis_max - mdy;
         enforce_axis_limits();
      }
      if (sb == 3) // adjust y_axis_max
      {
         double mdy = (mouse_y - y_axis_scrollbar_bar_y1) * y_scaler;
         y_axis_max = old_y_axis_max - mdy;
         enforce_axis_limits();
      }
   }
}






char* mwGraph::x_axis_get_val_text(double val)
{
//   sprintf(msg, "%4.0f frames", val);
   sprintf(msg, "frame:%2.0f", val);
   return msg;
}

char* mwGraph::y_axis_get_val_text(double val)
{
   sprintf(msg, "%4.0f kB/s", val);
   return msg;
}


int mwGraph::find_closest_point_to_mouse(void)
{
   double min_dist = 99;
   int min_dist_index = -1;
   int first_time = 1;
   int sx = 0, sy = 0;

   for (int i=0; i<data_points; i++)
      if ((data[i][0] >= x_axis_min) && (data[i][0] <= x_axis_max))
         if (convert_gxy_to_sxy(data[i][0], data[i][1], sx, sy))
         {
            double dist = sqrt( pow(mouse_x-sx, 2) + pow(mouse_y- sy, 2) );
            if (first_time == 1)
            {
               min_dist = dist;
               min_dist_index = i;
               first_time = 0;
            }
            if (dist < min_dist)
            {
               min_dist = dist;
               min_dist_index = i;
            }
         }
   if (min_dist < 10) return min_dist_index;
   else return -1;
}


void mwGraph::draw_point_data(int x, int y, double mx, double my, int color, ALLEGRO_FONT *f)
{
   int max_width = 0;
   int height = 0;
   int h2 = 0;


   int bx, by, bw, bh;
   al_get_text_dimensions(f, x_axis_get_val_text(mx), &bx, &by, &bw, &bh);
   if (bw > max_width) max_width = bw;
   h2 = bh;
   height += bh+1;

   al_get_text_dimensions(f, y_axis_get_val_text(my), &bx, &by, &bw, &bh);
   if (bw > max_width) max_width = bw;
   height += bh+1;


   al_draw_filled_rectangle(x, y, x+max_width+4, y+height+5, palette_color[0]); // erase old

   al_draw_rectangle(x, y, x+max_width+4, y+height+5, palette_color[color], 1);
   if (f == f3) y-=4;
   al_draw_textf(f, palette_color[color], x+1, y+2,    0, "%s", x_axis_get_val_text(mx));
   al_draw_textf(f, palette_color[color], x+1, y+h2+5, 0, "%s", y_axis_get_val_text(my));
}

void mwGraph::process_mouse_on_graph(void)
{


   // processs mouse on the graph area
   double mx1, my1, mx2, my2;
   if (convert_sxy_to_gxy(mouse_x, mouse_y, mx1, my1))
   {
      mx2 = mx1;
      my2 = my1;


      float x = 0;
      float y = 0;

      int col = 15;

      int a = find_closest_point_to_mouse();
      if (a != -1)
      {
         x = screen_x1 + (data[a][0]-x_axis_min) * screen_w / x_axis_rng;
         y = screen_y2 - (data[a][1]-y_axis_min) * screen_h / y_axis_rng;
         col = 14;
         al_draw_circle(x, y, 4, palette_color[col], 1);
         draw_point_data(x+5, y+5, data[a][0], data[a][1], col, font);
      }
      else
      {
         col = 15;
         x = mouse_x;
         y = mouse_y;
         draw_point_data(x+5, y+5, mx1, my1, col, f3);
      }

      al_set_clipping_rectangle(screen_x1 * display_transform_double, screen_y1 * display_transform_double, screen_w * display_transform_double, screen_h * display_transform_double);
      al_draw_line(screen_x1, y, screen_x2, y, palette_color[col+64], 0);
      al_draw_line(x, screen_y1, x, screen_y2, palette_color[col+64], 0);
      al_reset_clipping_rectangle();



      if (mouse_b[1][0])
      {
         if ((key[ALLEGRO_KEY_LSHIFT][0]) || (key[ALLEGRO_KEY_RSHIFT][0])) // shift drag to set new zoom area
         {
            int rmx = mouse_x;
            int rmy = mouse_y;
            while (mouse_b[1][0])
            {
               proc_controllers();
               convert_sxy_to_gxy_clamp(mouse_x, mouse_y, mx2, my2);
               {
                  draw();
                  al_set_clipping_rectangle(screen_x1 * display_transform_double, screen_y1 * display_transform_double, screen_w * display_transform_double, screen_h * display_transform_double);
                  al_draw_textf(font, palette_color[14], rmx, rmy -10, 0, "[SHIFT] + draw box zooms to new area");
                  al_draw_rectangle(rmx, rmy, mouse_x, mouse_y, palette_color[14], 1);
                  al_reset_clipping_rectangle();
                  al_flip_display();
               }
            }
            // mouse release...set new graph position
            if (y_axis_zoom_lock) set_range_axis(mx1, mx2, y_axis_min, y_axis_max);
            else                  set_range_axis(mx1, mx2, my1,        my2);


         }
         else // normal mouse drag to slide graph
         {
            double old_x_axis_min = x_axis_min;
            double old_x_axis_max = x_axis_max;
            int rmx = mouse_x;
            double x_scaler =  x_axis_rng / screen_w; // throughout the loop, range will never change

            double old_y_axis_min = y_axis_min;
            double old_y_axis_max = y_axis_max;
            int rmy = mouse_y;
            double y_scaler =  y_axis_rng / screen_h; // throughout the loop, range will never change

            while (mouse_b[1][0])
            {
               proc_controllers();
               if (convert_sxy_to_gxy(mouse_x, mouse_y, mx2, my2))
               {
                  double mdx = (mouse_x - rmx) * x_scaler;
                  x_axis_min = old_x_axis_min - mdx;
                  x_axis_max = old_x_axis_max - mdx;



                  double mdy = (mouse_y - rmy) * y_scaler;
                  double new_y_axis_min = old_y_axis_min + mdy;
                  double new_y_axis_max = old_y_axis_max + mdy;
                  if (y_axis_zoom_lock)
                  {
                     if (new_y_axis_min < y_data_min) // tried to scroll past edge
                     {
                        new_y_axis_min = y_data_min;
                        new_y_axis_max = new_y_axis_min + y_axis_rng;
                     }
                     if (new_y_axis_max > y_data_max) // tried to scroll past edge
                     {
                        new_y_axis_max = y_data_max;
                        new_y_axis_min = new_y_axis_max - y_axis_rng;
                     }
                  }

                  y_axis_min = new_y_axis_min;
                  y_axis_max = new_y_axis_max;


                  enforce_axis_limits();
                  draw();
                  al_flip_display();
               }
            }
         }
      }

      if (mouse_dz) // zoom with mouse wheel, centered on mouse position
      {
         if (mouse_dz < 0)
         {
            double old_xoffset = mx1 - x_axis_min;    // old distance from current position to min
            double new_xoffset = old_xoffset * 1.1;   // scale to new
            x_axis_min = mx1 - new_xoffset;           // set min from original position and new offset
            x_axis_rng *= 1.1;                        // convert range
            x_axis_max = x_axis_min + x_axis_rng;     // set new max

            if (!y_axis_zoom_lock)
            {
               double old_yoffset = my1 - y_axis_min;    // old distance from current position to min
               double new_yoffset = old_yoffset * 1.1;   // scale to new
               y_axis_min = my1 - new_yoffset;           // set min from original position and new offset
               y_axis_rng *= 1.1;                        // convert range
               y_axis_max = y_axis_min + y_axis_rng;     // set new max
            }
         }
         if (mouse_dz > 0)
         {
            double old_offset = mx1 - x_axis_min;     // old distance from current position to min
            double new_offset = old_offset * 0.9;     // scale to new
            x_axis_min = mx1 - new_offset;            // set min from original position and new offset
            x_axis_rng *= 0.9;                        // convert range
            x_axis_max = x_axis_min + x_axis_rng;     // set new max

            if (!y_axis_zoom_lock)
            {
               double old_yoffset = my1 - y_axis_min;    // old distance from current position to min
               double new_yoffset = old_yoffset * 0.9;   // scale to new
               y_axis_min = my1 - new_yoffset;           // set min from original position and new offset
               y_axis_rng *= 0.9;                        // convert range
               y_axis_max = y_axis_min + y_axis_rng;     // set new max
            }
         }
         mouse_dz = 0;
         enforce_axis_limits();
      }
   }
}


void mwGraph::process_input(void)
{
   al_set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);
   proc_x_scrollbar();
   proc_y_scrollbar();
   process_mouse_on_graph();
}








