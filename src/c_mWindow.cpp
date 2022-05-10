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


}


void mWindow::process(void)
{


   // move with mouse drag on title bar
   if ((mouse_on_window == 2) && (mouse_b1))
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
   if ((mouse_on_window == 3) && (mouse_b1))
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

   // adjust size from lower right corner
   if (mouse_on_window == 3) al_draw_rectangle(x2-10, y2-10, x2, y2, palette_color[color], 1);

}




void mWindow::detect_mouse(void)
{
   mouse_on_window = 0;
   color = 15;

   if ((mouse_x > x1) && (mouse_x < x2) && (mouse_y > y1) && (mouse_y < y2)) // entire window
   {
      mouse_on_window = 1;
      color = 14;
   }


   if ((mouse_x > x1) && (mouse_x < x2) && (mouse_y > y1) && (mouse_y < y1+8)) // title bar
   {
      mouse_on_window = 2;
      color = 10;
   }

   if ((mouse_x > x2-10) && (mouse_x < x2) && (mouse_y > y2-10) && (mouse_y < y2)) // lower right corner for resize
   {
      mouse_on_window = 3;
      color = 10;
   }





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

