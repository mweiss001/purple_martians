#include "pm.h"


mWindow::mWindow()
{
   x1 = 0;
   y1 = 0;
   x2 = 0;
   y2 = 0;
   w = 0;
   h = 0;
   mouse_on_window = 0;
   color = 15;
   sprintf(title, "");
   active = 0;
   have_focus = 0;
   layer = 0;
}


void mWindow::process(void)
{
   // move with mouse drag on title bar
   if ((mouse_x > x1) && (mouse_x < x2) && (mouse_y > y1) && (mouse_y < y1+8) && (mouse_b1))
   {
      int mxo = mouse_x - x1; // get offset from mouse position to window x, y
      int myo = mouse_y - y1;
      while (mouse_b1)
      {
         set_pos(mouse_x-mxo, mouse_y-myo);
         proc_controllers();
         al_flip_display();
         proc_scale_factor_change();
         proc_controllers();
         proc_frame_delay();
         get_new_background(0);
         draw_lifts();
         draw_items();
         draw_enemies();
         get_new_screen_buffer(3, 0, 0);
         draw();
      }
   }

   // resize with mouse drag on lower right corner
   if ((mouse_x > x2-10) && (mouse_x < x2) && (mouse_y > y2-10) && (mouse_y < y2) && (mouse_b1)) // lower right corner for resize
   {
      int mxo = mouse_x - x2; // get offset from mouse position to window x, y
      int myo = mouse_y - y2;
      while (mouse_b1)
      {
         x2 = mouse_x-mxo;
         y2 = mouse_y-myo;
         w = x2 - x1;
         h = y2 - y1;
         proc_controllers();
         al_flip_display();
         proc_scale_factor_change();
         proc_controllers();
         proc_frame_delay();
         get_new_background(0);
         draw_lifts();
         draw_items();
         draw_enemies();
         get_new_screen_buffer(3, 0, 0);
         draw();
      }
   }
}


void mWindow::set_focus(int n)
{
   mW[n].have_focus = 1;

   // detect if this window is not top window
   if (mW[n].layer !=0)
   {
      // get old layer number of window we are promoting
      int on = mW[n].layer;

      // slide everything up one layer (add 1 to all layers)
      for (int a=0; a<NUM_MW; a++)
         if (mW[a].active) mW[a].layer++;

      // set new top layer
      mW[n].layer = 0;

      // remove gaps in layers
      for (int b=on+2; b<NUM_MW+10; b++)
         for (int a=0; a<NUM_MW; a++)
            if ((mW[a].active) && (mW[a].layer == b)) mW[a].layer--;
   }
}




void mWindow::draw(void)
{
   // erase background
   al_draw_filled_rectangle(x1, y1, x2, y2, palette_color[0]);

   // frame window
   al_draw_rectangle(x1, y1, x2, y2, palette_color[color], 1);

   // frame title bar
   al_draw_rectangle(x1, y1, x2, y1+11, palette_color[color], 1);

   // title
   al_draw_textf(font, palette_color[color], x1+2, y1+2, 0, title);


//   al_draw_textf(font, palette_color[color], x1+2, y1+14, 0, "x1:%d x2:%d w:%d", x1, x2, w);
//   al_draw_textf(font, palette_color[color], x1+2, y1+22, 0, "y1:%d y2:%d h:%d", y1, y2, h);

   al_draw_textf(font, palette_color[color], x1+2, y1+22, 0, "layer:%d", layer);
   al_draw_textf(font, palette_color[color], x1+2, y1+30, 0, "focus:%d", have_focus);

   if (have_focus)
   {
      // lower right corner
      if ((mouse_x > x2-10) && (mouse_x < x2) && (mouse_y > y2-10) && (mouse_y < y2)) // lower right corner for resize
         al_draw_rectangle(x2-10, y2-10, x2, y2, palette_color[10], 1);

      // title bar
      if ((mouse_x > x1) && (mouse_x < x2) && (mouse_y > y1) && (mouse_y < y1+8))
         al_draw_rectangle(x1, y1, x2, y1+11, palette_color[10], 1);

      // entire window
      al_draw_rectangle(x1, y1, x2, y2, palette_color[10], 1);
   }
}




int mWindow::detect_mouse(void)
{
   if ((mouse_x > x1) && (mouse_x < x2) && (mouse_y > y1) && (mouse_y < y2)) return 1;
   else return 0;
}


void mWindow::set_title(const char* st)
{
   sprintf(title, "%s", st);
}


void mWindow::set_pos(int sx, int sy)
{
   x1 = sx;
   y1 = sy;
   x2 = x1 + w;
   y2 = y1 + h;
}
void mWindow::set_size(int sw, int sh)
{
   w = sw;
   h = sh;
   x2 = x1 + w;
   y2 = y1 + h;
}

