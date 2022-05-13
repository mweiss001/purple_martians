#include "pm.h"




int mw_get_max_layer(void)
{
   int max_layer = 0;
   for (int a=0; a<NUM_MW; a++)
      if ((mW[a].active) && (mW[a].layer > max_layer)) max_layer = mW[a].layer;
   return max_layer;
}

// iterate windows by layers down from max, drawing and detecting mouse as we go
int mw_cycle_windows(int draw_only)
{
   int mouse_on_window = 0;
   for (int b = mw_get_max_layer(); b>=0; b--)
      for (int a=0; a<NUM_MW; a++) // draw any window at that layer
         if ((mW[a].active) && (mW[a].layer == b))
         {
            mW[a].draw();
            if ((!draw_only) && (mW[a].detect_mouse())) mouse_on_window = a;
         }

   if (!draw_only)
   {
      // set all window to not have focus
      for (int a=0; a<NUM_MW; a++)
         if (mW[a].active) mW[a].have_focus = 0;

      // this one does
      if (mouse_on_window) mW[mouse_on_window].set_focus(mouse_on_window);

      // process window that has focus
      for (int a=0; a<NUM_MW; a++)
         if ((mW[a].active) && (mW[a].have_focus)) mW[a].process();

   }
   return mouse_on_window;
}

mWindow::mWindow()
{
   x1 = 0;
   y1 = 0;
   x2 = 0;
   y2 = 0;
   w = 0;
   h = 0;
   color = 15;
   title[0] = 0;
   active = 0;
   have_focus = 0;
   layer = 0;
   moveable = 1;
   moving = 0;
   resizable = 0;
}

void mWindow::process(void)
{
   if (mouse_b1)
   {
      // moveable and mouse on title bar
      if ((moveable) && (mouse_x > x1) && (mouse_x < x2) && (mouse_y > y1) && (mouse_y < y1+8))
      {
         int mxo = mouse_x - x1; // get offset from mouse position to window x, y
         int myo = mouse_y - y1;
         moving = 1;
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
            mw_cycle_windows(1); // draw only
         }
         moving = 0;
      }
      // resizable and mouse on lower right corner
      if ((resizable) && (mouse_x > x2-10) && (mouse_x < x2) && (mouse_y > y2-10) && (mouse_y < y2))
      {
         int mxo = mouse_x - x2; // get offset from mouse position to window x, y
         int myo = mouse_y - y2;
         moving = 1;
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
            mw_cycle_windows(1); // draw only
         }
      }
      moving = 0;
   }


   // mouse b2 anywhere on window
   if ((mouse_b2) && (mouse_x > x1) && (mouse_x < x2) && (mouse_y > y1) && (mouse_y < y2))
   {
      if (index == 3) // filter window
      {
         sprintf(global_string[6][0],"Filters");
         sprintf(global_string[6][1],"---------------");
         sprintf(global_string[6][2],"All On");
         sprintf(global_string[6][3],"All Items On");
         sprintf(global_string[6][4],"All Enemies On");
         sprintf(global_string[6][5],"All Off");
         sprintf(global_string[6][6],"All Items Off");
         sprintf(global_string[6][7],"All Enemies Off");
         sprintf(global_string[6][8],"end");
         switch (pmenu(6, 13))
         {
             case 2:
                for (int i=0; i<5; i++)
                   for (int j=0; j<20; j++)
                      obj_filter[i][j] = 1;
                break;
             case 3:
                for (int j=0; j<20; j++)
                   obj_filter[2][j] = 1;
             break;
             case 4:
                for (int j=0; j<20; j++)
                   obj_filter[3][j] = 1;
             break;
             case 5:
             for (int i=0; i<5; i++)
                for (int j=0; j<20; j++)
                   obj_filter[i][j] = 0;
             break;
             case 6:
                for (int j=0; j<20; j++)
                   obj_filter[2][j] = 0;
             break;
             case 7:
                for (int j=0; j<20; j++)
                   obj_filter[3][j] = 0;
             break;
         }
      }

      if (index == 1) // filter window
      {
         sprintf(global_string[6][0],"Status Windows");
         sprintf(global_string[6][1],"---------------");

         if (show_flag_details) sprintf(global_string[6][2],"Hide Block Flags");
         else                   sprintf(global_string[6][2],"Show Block Flags");

         if (show_non_default_blocks) sprintf(global_string[6][3],"Hide Non-Default Blocks");
         else                         sprintf(global_string[6][3],"Show Non-Default Blocks");

         sprintf(global_string[6][4],"end");
         switch (pmenu(6, 13))
         {
             case 2: show_flag_details =! show_flag_details; break;
             case 3: show_non_default_blocks =! show_non_default_blocks; break;
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

   mdw_toggle(x2-10, y1+2, x2-2, y1+10, 1000, 0,0,0,0,0,0,0,1,0,0,d, ft_window_collapsed,  "-", "+", tc1, tc2, -1, -1);



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

   // default window
   if ((index != 1) && (index != 2) && (index != 3))
   {
      // frame window
      al_draw_rectangle(x1, y1, x2, y2, palette_color[color], 1);

      // frame title bar
      al_draw_rectangle(x1, y1, x2, y1+11, palette_color[color], 1);

      // title
      al_draw_textf(font, palette_color[color], x1+2, y1+2, 0, title);

      al_draw_textf(font, palette_color[color], x1+2, y1+22, 0, "layer:%d", layer);
      al_draw_textf(font, palette_color[color], x1+2, y1+30, 0, "focus:%d", have_focus);

//   al_draw_textf(font, palette_color[color], x1+2, y1+14, 0, "x1:%d x2:%d w:%d", x1, x2, w);
//   al_draw_textf(font, palette_color[color], x1+2, y1+22, 0, "y1:%d y2:%d h:%d", y1, y2, h);

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
   }






   if (index == 2) // selection window
   {
      // title bar background color
      al_draw_filled_rectangle(x1, y1, x2, y1 + 11, palette_color[9+192]);

      // frame title bar
      al_draw_rectangle(x1, y1, x2, y1+11, palette_color[9], 1);

      al_draw_textf(font, palette_color[9],  x1+2,   y1+2, 0, "Selection Window");

//      int mow = 0;
//      for (int a=0; a<NUM_MW; a++)
//         if ((mW[a].active) && (mW[a].detect_mouse())) mow = 1;

      al_draw_rectangle(x1, y1, x2, y2, palette_color[9], 1); // frame entire window

      if (have_focus)
      {
         if (moveable) // frame title bar
         {
            if ((mouse_x > x1) && (mouse_x < x2) && (mouse_y > y1) && (mouse_y < y1+8))
               al_draw_rectangle(x1, y1, x2, y1+11, palette_color[14], 1);
         }
      }

      int d = 1;
      if (have_focus) d = 0;
      if (moving) d = 1;
      if (mdw_buttont(x2-10, y1+1, x2-2, y1+10, 0,0,0,0, 0,-1,9,0, 0,0,0,d,"X")) select_window_active = 0;
      if (mdw_buttont(x2-22, y1+1, x2-14, y1+10, 0,0,0,0, 0,-1,9,0, 0,0,0,d,"?")) help("Selection Window");
      if (mdw_buttont(x2-153, y1+1, x2-105, y1+10, 0,0,0,0, 0,-1,9,0, 0,1,0,d,"Blocks")) select_window_block_on = !select_window_block_on;
      if (mdw_buttont(x2-90,  y1+1, x2-34,  y1+10, 0,0,0,0, 0,-1,9,0, 0,1,0,d,"Special")) select_window_special_on = !select_window_special_on;







      int c = 13;  // first y line of sub-windows;

      // set special start y
      if (select_window_special_on)
      {
         select_window_special_y = c;
         c = 16 + c + select_window_num_special_lines*20;
      }
      // set special start y
      if (select_window_block_on)
      {
         select_window_block_y = c;
         c = 16 + c + swnbl_cur*20;
      }
      select_window_text_y = c;
      h = select_window_h = select_window_text_y;
      y2 = y1+h;

      int sys = y1 + select_window_special_y;
      int syb = y1 + select_window_block_y;
      int syt = y1 + select_window_text_y;

      int vx = (mouse_x-x1)/20; // column

      if (select_window_special_on)
      {
         al_draw_rectangle(x1, sys, x2, sys+12, palette_color[9], 1);
         al_draw_text(font, palette_color[9], x1+2, sys+2, 0, "Special Items");

         if (mdw_buttont(x2-9, sys+2, x2-1, sys+10, 0,0,0,0, 0,-1,9,0, 0,0,0,d,"X")) select_window_special_on = 0;

         if (mdw_buttont(x2-41, sys+2, x2-33, sys+10, 0,0,0,0, 0,-1,9,0, 0,0,0,d,"+"))
            if (++select_window_num_special_lines > 4 ) select_window_num_special_lines = 4;

         if (mdw_buttont(x2-25, sys+2, x2-17, sys+10, 0,0,0,0, 0,-1,9,0, 0,0,0,d,"-"))
            if (--select_window_num_special_lines < 1 )
            {
               select_window_num_special_lines++;
               select_window_special_on = 0;
            }
      }

      if (select_window_block_on)
      {
         al_draw_rectangle(x1, syb, x2, syb+12, palette_color[9], 1);
         al_draw_text(font, palette_color[9], x1+2, syb+2, 0, "Block Selection");


         if (mdw_buttont(x2-9, syb+2, x2-1, syb+10, 0,0,0,0, 0,-1,9,0, 0,0,0,d,"X")) select_window_block_on = 0;

         if (mdw_buttont(x2-41, syb+2, x2-33, syb+10, 0,0,0,0, 0,-1,9,0, 0,0,0,d,"+"))
            if (++swnbl_cur > swnbl) swnbl_cur = swnbl;

         if (mdw_buttont(x2-25, syb+2, x2-17, syb+10, 0,0,0,0, 0,-1,9,0, 0,0,0,d,"-"))
            if (--swnbl_cur < 1 )
            {
               swnbl_cur++;
               select_window_block_on = 0;
            }



         // draw special block
         for (c=0; c<16*select_window_num_special_lines; c++)
         {
            int tn = PDEi[c][1]; // default is the tile in PDEi[c][1]
            if (tn > 999) tn = zz[5][tn-1000]; // ans
            al_draw_bitmap(tile[tn], x1+(c-((c/16)*16) )*20+1, y1+14+select_window_special_y+1+(c/16*20), 0 );
         }



      }




         // check for mouse on special window
         if ((select_window_special_on) && (mouse_y > 15 + sys) && (mouse_y < 16 + sys + select_window_num_special_lines * 20))
         {
            int vy = (mouse_y-sys-15)/20; // row
            int ret = vy*16+vx;
            int tl = 0; // text lines
            if (ret < 100) // dont try to show anything above PDE[99]
            {
               // set  text length (number of lines)
               for (int x=0; x<20; x++)
                  if (strncmp(PDEt[ret][x],"<end>", 5) == 0) tl = x;
               if (tl<5) tl = 5;

                // remove line endings
               for (int x=0; x<20; x++)
                  for (int z=0; z<40; z++)
                  {
                     if (PDEt[ret][x][z] == 10) PDEt[ret][x][z] = 32;
                     if (PDEt[ret][x][z] == 13) PDEt[ret][x][z] = 32;
                  }

               // erase and frame
               al_draw_filled_rectangle(x1, syt, x2, 12+syt+3+(8*tl), palette_color[0]);
               al_draw_rectangle(x1, syt, x2, 12+syt+3+(8*tl), palette_color[9], 1);

               // title and frame
               al_draw_rectangle(x1, syt, x2, syt+12, palette_color[9], 1);
               al_draw_text(font, palette_color[9], x1+2, syt+2, 0, "Description ");

               // draw text for this pde
               stext_draw_flag=1;
               for (int x=0; x<tl; x++)
                  al_draw_text(font, palette_color[15], x1+2, y1 + select_window_text_y+14+(x*8), 0, PDEt[ret][x]);

               if (mouse_b1)
               {
                  while (mouse_b1) proc_controllers();     // wait for release
                  int pn = PDEi[ret][0];
                  if (pn < 200)
                  {
                     draw_item_type = 5;
                     draw_item_num = ret;
                  }
                  if (pn > 199) // Creator
                  {
                     switch (pn)
                     {
                        case 200: create_obj(2, 1, 0);  break; // type 200 - door
                        case 201: create_obj(2, 5, 0);  break; // type 201 - start
                        case 202: create_obj(2, 3, 0);  break; // type 202 - exit
                        case 203: create_obj(2, 4, 0);  break; // type 203 - key
                        case 204: create_obj(3, 7, 0);  break; // type 204 - pod
                        case 206: create_obj(2, 10,0);  break; // type 206 - msg
                        case 207: create_obj(3, 9, 0);  break; // type 207 - cloner
                        case 208: if (create_lift()) object_viewerw(4, num_lifts-1); break; // type 208 - lift
                        case 209: create_door(1);       break; // type 209 - 1 way al_fixed exit door
                        case 210: create_door(2);       break; // type 210 - 1 way linked exit door
                        case 211: create_door(3);       break; // type 211 - 2 way door set
                        case 212: create_obj(3, 10, 0); break; // type 212 - field
                        case 213: create_obj(2, 9, 0);  break; // type 213 - trigger
                        case 214: create_obj(2, 16, 0); break; // type 214 - block manip
                        case 215: create_obj(2, 17, 0); break; // type 215 - block damage
                     }
                  } // end of if creator
               } // end of if (mouse_b & 1)
            } // end of ret < 99
         }  // end of mouse on special
         else stext_draw_flag = 0; // mouse not on special
































   }









   if (index == 1) // status window
   {
      // title bar background color
      al_draw_filled_rectangle(x1, y1, x2, y1 + 11, palette_color[9+192]);

      // frame title bar
      al_draw_rectangle(x1, y1, x2, y1+11, palette_color[9], 1);

      al_draw_textf(font, palette_color[9],  x1+2,   y1+2, 0, "Status Window    level:%d ",last_level_loaded);
      al_draw_textf(font, palette_color[15], x1+186, y1+2, 0, "%d ",last_level_loaded);

      int mow = 0;
      for (int a=0; a<NUM_MW; a++)
         if ((mW[a].active) && (mW[a].detect_mouse())) mow = 1;

      if (!mow)
      {
         int gx=0, gy=0, hx=0, hy=0;
         ovw_get_block_position_on_map(&gx, &gy, &hx, &hy);
         al_draw_textf(font, palette_color[15], x1+222, y1+2, 0, "x:%-2d y:%-2d ", gx, gy);
      }
      else
      {
         al_draw_textf(font, palette_color[15], x1+222, y1+2, 0, "x:-- y:-- ");
      }

      al_draw_text( font, palette_color[9],  x1+222, y1+2, 0, "x:");
      al_draw_text( font, palette_color[9],  x1+262, y1+2, 0, "y:");


      al_draw_rectangle(x1, y1, x2, y2, palette_color[9], 1); // frame entire window

      if (have_focus)
      {
         if (moveable) // frame title bar
         {
            if ((mouse_x > x1) && (mouse_x < x2) && (mouse_y > y1) && (mouse_y < y1+8))
               al_draw_rectangle(x1, y1, x2, y1+11, palette_color[14], 1);
         }
      }

      int d = 1;
      if (have_focus) d = 0;
      if (moving) d = 1;
      if (mdw_buttont(x2-10, y1+1, x2-2, y1+10, 0,0,0,0, 0,-1,9,0, 0,0,0,d,"X")) status_window_active = 0;
      if (mdw_buttont(x2-22, y1+1, x2-14, y1+10, 0,0,0,0, 0,-1,9,0, 0,0,0,d,"?")) help("Status Window");

      // draw item area
      al_draw_rectangle(x1, y1 + 12, x1 + 160, y2, palette_color[9], 1);
      al_draw_text(font, palette_color[15], x1 + 24,  y1 + 14, 0, "Draw Item   ");
      al_draw_text(font, palette_color[14], x1 + 100, y1 + 14, 0, "mouse");
      al_draw_text(font, palette_color[14], x1 + 143, y1 + 14, 0, "b1");
      em_draw_item_info(                    x1 + 2,   y1 + 21, 9, draw_item_type, draw_item_num);

      // draw item flags
      if ((draw_item_type == 1) && (show_flag_details)) draw_flags(x1+4, y1+47, &draw_item_num, &mow, 0, 1, 0);


      // view item area
      al_draw_rectangle(x1 + 160, y1 + 12, x2, y2, palette_color[9], 1);
      al_draw_text(font, palette_color[15], x1 + 184, y1 + 14, 0, "View Item ");
      al_draw_text(font, palette_color[14], x1 + 261, y1 + 14, 0, "mouse");
      al_draw_text(font, palette_color[14], x1 + 303, y1 + 14, 0, "b2");
      em_draw_item_info(                    x1 + 162, y1 + 21, 9, point_item_type, point_item_num);

      if ((point_item_type == 1) && (show_flag_details)) draw_flags(x1+164, y1+47, &point_item_num, &mow, 1, 0, 1);
   }


   if (index == 3) // filter window
   {
      int fc = 15;
      if (have_focus) fc = 10;
      al_draw_rectangle(x1, y1, x2, y2, palette_color[fc], 1); // frame entire window
      y2 = 1 + mw_draw_filter_buttons(x1+1, x2-1, y1+1, 1, have_focus, moving);
      h = y2 - y1;
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

