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
   moveable = 1;
   moving = 0;
   resizable = 0;
}


void mWindow::process(void)
{
   if (moveable)
   {
      // move with mouse drag on title bar
      if ((mouse_x > x1) && (mouse_x < x2) && (mouse_y > y1) && (mouse_y < y1+8) && (mouse_b1))
      {
         int mxo = mouse_x - x1; // get offset from mouse position to window x, y
         int myo = mouse_y - y1;
         while (mouse_b1)
         {
            moving = 1;
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
      moving = 0;
   }
   if (resizable)
   {// resize with mouse drag on lower right corner
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


int mw_draw_filter_buttons(int x1, int x2, int y1, int mode, int have_focus, int moving)
{
   int d = 1;
   if (have_focus) d = 0;
   if (moving) d = 1;


   int fs = 12;   // frame size
   int y = y1+fs; // button y position

   int bts = 12;    // button size
   int a = 0;       // keep track of button y spacing

   int tc1 = 15;    // text color 1
   int tc2 = 15;    // text color 2
   int fc1 = 15+64; // frame color 1
   int fc2 = 4;     // frame color 2

   int tl=0; // text_lines
   if (mode == 1) tl = 25;
   if (mode == 2) tl = 27;
   if (mode == 3) tl = 29;

   if (ft_window_collapsed) tl = -1;

   int y2 = y1+tl*bts+fs*2; // pre calc

   // erase background
   al_draw_filled_rectangle(x1, y1, x2, y2, palette_color[0]);

   // draw frame around filter buttons
   int ci = 16; //color inc
   for (int q=0; q<fs; q++)
      al_draw_rectangle(x1+q, y1+q, x2-q, y2-q, palette_color[12+32+(q*ci)], 1);
   al_draw_text(font, palette_color[15], (x1+x2)/2, y1+2, ALLEGRO_ALIGN_CENTER, "Filters");

   if (!ft_window_collapsed)
   {
      if (mode > 2) // add blocks and flags
      {
         mdw_toggle(x1+fs, y+a*bts, x2-fs, y+(a+1)*bts-2, 1000, 0,0,0,0,0,0,0,1,0,0,d, obj_filter[1][1],  "Blocks", "Blocks", tc1, tc2, fc1, fc2); a++;
         mdw_toggle(x1+fs, y+a*bts, x2-fs, y+(a+1)*bts-2, 1000, 0,0,0,0,0,0,0,1,0,0,d, obj_filter[1][2],  "Flags",  "Flags",  tc1, tc2, fc1, fc2); a++;
      }
      if (mode > 1) // add lifts
      {
         mdw_toggle(x1+fs, y+a*bts, x2-fs, y+(a+1)*bts-2, 1000, 0,0,0,0,0,0,0,1,0,0,d, obj_filter[4][1],  "Lifts",  "Lifts",  tc1, tc2, fc1, fc2); a++;
         a++;
      }
      mdw_toggle(x1+fs, y+a*bts, x2-fs, y+(a+1)*bts-2, 1000, 0,0,0,0,0,0,0,1,0,0,d, obj_filter[3][3],  "Arcwgn", "Arcwgn", tc1, tc2, fc1, fc2); a++;
      mdw_toggle(x1+fs, y+a*bts, x2-fs, y+(a+1)*bts-2, 1000, 0,0,0,0,0,0,0,1,0,0,d, obj_filter[3][4],  "Bouncr", "Bouncr", tc1, tc2, fc1, fc2); a++;
      mdw_toggle(x1+fs, y+a*bts, x2-fs, y+(a+1)*bts-2, 1000, 0,0,0,0,0,0,0,1,0,0,d, obj_filter[3][6],  "Cannon", "Cannon", tc1, tc2, fc1, fc2); a++;
      mdw_toggle(x1+fs, y+a*bts, x2-fs, y+(a+1)*bts-2, 1000, 0,0,0,0,0,0,0,1,0,0,d, obj_filter[3][7],  "Podzil", "Podzil", tc1, tc2, fc1, fc2); a++;
      mdw_toggle(x1+fs, y+a*bts, x2-fs, y+(a+1)*bts-2, 1000, 0,0,0,0,0,0,0,1,0,0,d, obj_filter[3][8],  "Trakbt", "Trakbt", tc1, tc2, fc1, fc2); a++;
      mdw_toggle(x1+fs, y+a*bts, x2-fs, y+(a+1)*bts-2, 1000, 0,0,0,0,0,0,0,1,0,0,d, obj_filter[3][9],  "Cloner", "Cloner", tc1, tc2, fc1, fc2); a++;
      mdw_toggle(x1+fs, y+a*bts, x2-fs, y+(a+1)*bts-2, 1000, 0,0,0,0,0,0,0,1,0,0,d, obj_filter[3][10], "Field",  "Field",  tc1, tc2, fc1, fc2); a++;
      mdw_toggle(x1+fs, y+a*bts, x2-fs, y+(a+1)*bts-2, 1000, 0,0,0,0,0,0,0,1,0,0,d, obj_filter[3][11], "Blk Wk", "Blk Wk", tc1, tc2, fc1, fc2); a++;
      mdw_toggle(x1+fs, y+a*bts, x2-fs, y+(a+1)*bts-2, 1000, 0,0,0,0,0,0,0,1,0,0,d, obj_filter[3][12], "Flappr", "Flappr", tc1, tc2, fc1, fc2); a++;
      a++;
      mdw_toggle(x1+fs, y+a*bts, x2-fs, y+(a+1)*bts-2, 1000, 0,0,0,0,0,0,0,1,0,0,d, obj_filter[2][1],  "Door",   "Door",   tc1, tc2, fc1, fc2); a++;
      mdw_toggle(x1+fs, y+a*bts, x2-fs, y+(a+1)*bts-2, 1000, 0,0,0,0,0,0,0,1,0,0,d, obj_filter[2][2],  "Bonus",  "Bonus",  tc1, tc2, fc1, fc2); a++;
      mdw_toggle(x1+fs, y+a*bts, x2-fs, y+(a+1)*bts-2, 1000, 0,0,0,0,0,0,0,1,0,0,d, obj_filter[2][3],  "Exit",   "Exit",   tc1, tc2, fc1, fc2); a++;
      mdw_toggle(x1+fs, y+a*bts, x2-fs, y+(a+1)*bts-2, 1000, 0,0,0,0,0,0,0,1,0,0,d, obj_filter[2][4],  "Key",    "Key",    tc1, tc2, fc1, fc2); a++;
      mdw_toggle(x1+fs, y+a*bts, x2-fs, y+(a+1)*bts-2, 1000, 0,0,0,0,0,0,0,1,0,0,d, obj_filter[2][5],  "Start",  "Start",  tc1, tc2, fc1, fc2); a++;
      mdw_toggle(x1+fs, y+a*bts, x2-fs, y+(a+1)*bts-2, 1000, 0,0,0,0,0,0,0,1,0,0,d, obj_filter[2][7],  "Mine",   "Mine",   tc1, tc2, fc1, fc2); a++;
      mdw_toggle(x1+fs, y+a*bts, x2-fs, y+(a+1)*bts-2, 1000, 0,0,0,0,0,0,0,1,0,0,d, obj_filter[2][8],  "Bomb",   "Bomb",   tc1, tc2, fc1, fc2); a++;
      mdw_toggle(x1+fs, y+a*bts, x2-fs, y+(a+1)*bts-2, 1000, 0,0,0,0,0,0,0,1,0,0,d, obj_filter[2][9],  "Triggr", "Triggr", tc1, tc2, fc1, fc2); a++;
      mdw_toggle(x1+fs, y+a*bts, x2-fs, y+(a+1)*bts-2, 1000, 0,0,0,0,0,0,0,1,0,0,d, obj_filter[2][10], "Messge", "Messge", tc1, tc2, fc1, fc2); a++;
      mdw_toggle(x1+fs, y+a*bts, x2-fs, y+(a+1)*bts-2, 1000, 0,0,0,0,0,0,0,1,0,0,d, obj_filter[2][11], "Rocket", "Rocket", tc1, tc2, fc1, fc2); a++;
      mdw_toggle(x1+fs, y+a*bts, x2-fs, y+(a+1)*bts-2, 1000, 0,0,0,0,0,0,0,1,0,0,d, obj_filter[2][12], "Warp",   "Warp",   tc1, tc2, fc1, fc2); a++;
      mdw_toggle(x1+fs, y+a*bts, x2-fs, y+(a+1)*bts-2, 1000, 0,0,0,0,0,0,0,1,0,0,d, obj_filter[2][14], "Switch", "Switch", tc1, tc2, fc1, fc2); a++;
      mdw_toggle(x1+fs, y+a*bts, x2-fs, y+(a+1)*bts-2, 1000, 0,0,0,0,0,0,0,1,0,0,d, obj_filter[2][15], "Spring", "Spring", tc1, tc2, fc1, fc2); a++;
      mdw_toggle(x1+fs, y+a*bts, x2-fs, y+(a+1)*bts-2, 1000, 0,0,0,0,0,0,0,1,0,0,d, obj_filter[2][16], "Blk Mn", "Blk Mn", tc1, tc2, fc1, fc2); a++;
      mdw_toggle(x1+fs, y+a*bts, x2-fs, y+(a+1)*bts-2, 1000, 0,0,0,0,0,0,0,1,0,0,d, obj_filter[2][17], "Blk Dm", "Blk Dm", tc1, tc2, fc1, fc2); a++;

   }
   return y2;
}

void mWindow::draw(void)
{
   // erase background
   al_draw_filled_rectangle(x1, y1, x2, y2, palette_color[0]);

   // frame window
   al_draw_rectangle(x1, y1, x2, y2, palette_color[color], 1);


   if (index != 3)
   {
      // frame title bar
      al_draw_rectangle(x1, y1, x2, y1+11, palette_color[color], 1);

      // title
      al_draw_textf(font, palette_color[color], x1+2, y1+2, 0, title);

      al_draw_textf(font, palette_color[color], x1+2, y1+22, 0, "layer:%d", layer);
      al_draw_textf(font, palette_color[color], x1+2, y1+30, 0, "focus:%d", have_focus);

//   al_draw_textf(font, palette_color[color], x1+2, y1+14, 0, "x1:%d x2:%d w:%d", x1, x2, w);
//   al_draw_textf(font, palette_color[color], x1+2, y1+22, 0, "y1:%d y2:%d h:%d", y1, y2, h);

   }





   if (have_focus)
   {
      // frame entire window
      al_draw_rectangle(x1, y1, x2, y2, palette_color[10], 1);

      // frame title bar
      if (moveable)
      {
         if ((mouse_x > x1) && (mouse_x < x2) && (mouse_y > y1) && (mouse_y < y1+8))
            al_draw_rectangle(x1, y1, x2, y1+11, palette_color[10], 1);
      }

      // lower right corner
      if (resizable)
      {
         if ((mouse_x > x2-10) && (mouse_x < x2) && (mouse_y > y2-10) && (mouse_y < y2)) // lower right corner for resize
            al_draw_rectangle(x2-10, y2-10, x2, y2, palette_color[10], 1);
      }
   }

   if (index == 3) y2 = 1 + mw_draw_filter_buttons(x1+1, x2-1, y1+1, 1, have_focus, moving);











}

int mWindow::detect_mouse(void)
{
   moving = 0;
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

