// e_editor_main.cpp
#include "pm.h"

void cm_get_block_position_on_map()
{
   // x, y in 0-99 scale
   // the mouse position past the border width is how far we are into the scaled map
   float mx1 = mouse_x-BORDER_WIDTH;
   float my1 = mouse_y-BORDER_WIDTH;

   // divide that by bs to get how many blocks we are into the map
   float mx2 = mx1 / (scale_factor_current * 20);
   float my2 = my1 / (scale_factor_current * 20);
   // get block position of WX
   float mx3 = (float)WX / 20;
   float my3 = (float)WY / 20;

   // add
   float mx4 = mx3 + mx2;
   float my4 = my3 + my2;

   gx = (int) mx4;
   gy = (int) my4;

   if (gx < 0)  gx = 0;
   if (gy < 0)  gy = 0;
   if (gx > 99) gx = 99;
   if (gy > 99) gy = 99;

   // hx, hy in 0-1999 scale
   // the mouse position past the border width is how far we are into the scaled map
   mx1 = mouse_x-BORDER_WIDTH;
   my1 = mouse_y-BORDER_WIDTH;

   // scale
   mx2 = mx1 / scale_factor_current;
   my2 = my1 / scale_factor_current;

   // get position of WX
   mx3 = (float)WX;
   my3 = (float)WY;

   // add
   mx4 = mx3 + mx2;
   my4 = my3 + my2;

   hx = (int) mx4;
   hy = (int) my4;

   if (hx < 0)    hx = 0;
   if (hy < 0)    hy = 0;
   if (hx > 1999) hx = 1999;
   if (hy > 1999) hy = 1999;
}

void cm_process_scrolledge(void)
{
   int bw = BORDER_WIDTH;
   int scrolledge = 10;
   int scroll_amount = 20;

   if (mouse_x < scrolledge) WX-=scroll_amount;           // scroll left
   if (mouse_x > SCREEN_W-scrolledge) WX+=scroll_amount;  // scroll right
   if (mouse_y < scrolledge) WY-=scroll_amount;           // scroll up
   if (mouse_y > SCREEN_H-scrolledge) WY+=scroll_amount;  // scroll down

      // find the size of the source screen from actual screen size and scaler
   int SW = (int)( (float)(SCREEN_W - bw *2) / scale_factor_current);
   int SH = (int)( (float)(SCREEN_H - bw *2) / scale_factor_current);
   if (SW > 2000) SW = 2000;
   if (SH > 2000) SH = 2000;

   // correct for edges
   if (WX < 0) WX = 0;
   if (WY < 0) WY = 0;
   if (WX > (2000 - SW)) WX = 2000 - SW;
   if (WY > (2000 - SH)) WY = 2000 - SH;

   // used by get_new_background to only get what is needed
   level_display_region_x = WX;
   level_display_region_y = WY;
   level_display_region_w = SW;
   level_display_region_h = SH;

}


// this function draws a box at full scale on level buffer
// even if the top left and bottom right corners are switched
// used by zfs, ge and em
void cm_show_level_buffer_block_rect(int x1, int y1, int x2, int y2, int color, const char * text)
{
   if (x1 > x2) swap_int(&x1, &x2);
   if (y1 > y2) swap_int(&y1, &y2);
   int dstx = x1*20;
   if (dstx == 0) dstx = 1;
   int dsty = y1*20;
   if (dsty == 0) dsty = 1;
   al_draw_rectangle(dstx, dsty, (x2*20)+19, (y2*20)+19, palette_color[14], 1);
   al_draw_text(font, palette_color[color], x1*20+2, y1*20-11,  0, text);
}


// used by zfs, ge and em
void cm_get_new_box(void) // keep the mouse !!
{
   bx2 = bx1 = gx; // set both corners to initial position
   by2 = by1 = gy;
   while (mouse_b1)
   {
      bx2 = gx;
      by2 = gy;
      cm_redraw_level_editor_background(0);
      cm_show_level_buffer_block_rect(bx1, by1, bx2, by2, 14, "selection");
      get_new_screen_buffer(3, 0, 0);
   }
   if (bx1 > bx2) swap_int(&bx1, &bx2); // swap if wrong order
   if (by1 > by2) swap_int(&by1, &by2);
}

void cm_redraw_level_editor_background(int mode)
{
   int old_lem = level_editor_mode;
   level_editor_mode = mode;
   cm_redraw_level_editor_background();
   level_editor_mode = old_lem;
}

void cm_redraw_level_editor_background(void)
{
   process_flash_color();

   int mouse_on_window = is_mouse_on_any_window();

   if ((!mouse_on_window) || (level_editor_mode == 0))
   {
      cm_get_block_position_on_map();
      cm_process_scrolledge();
   }

   al_flip_display();
   proc_scale_factor_change();
   proc_controllers();
   proc_frame_delay();


   if ((level_editor_mode == 4) && (mW[7].obt == 4)) init_level_background(); // to draw new lift lines


   init_level_background();
   get_new_background(0);
   draw_lifts();
   draw_items();
   draw_enemies();

   if (level_editor_mode == 1) // edit menu
   {
      if (!mouse_on_window) em_show_draw_item_cursor();
   }

   if (level_editor_mode == 2) // zfs
   {
      // alway show selection
      cm_show_level_buffer_block_rect(bx1, by1, bx2, by2, 14, "selection");

      // only show these if mouse not in window
      if (!mouse_on_window)
      {
         if (mW[4].brf_mode) crosshairs_full(gx*20+10, gy*20+10, 15, 1);
         if (mW[4].copy_mode)
         {
            int sw = bx2-bx1;
            int sh = by2-by1;
            al_draw_bitmap(ft_bmp, gx*20, gy*20, 0);
            cm_show_level_buffer_block_rect(gx, gy, gx+sw, gy+sh, 10, "paste");
         }
      }
   }
   if (level_editor_mode == 3) // ge
   {

      // mark objects on map that are capable of being added to list
      for (int i=0; i<500; i++)
      {
         int type = (item[i][0]);
         if ((type) && (obj_filter[2][type]))
         {
            int sox = item[i][4];
            int soy = item[i][5];
            al_draw_rectangle(sox, soy, sox+20, soy+20, palette_color[13], 1);
         }
      }
      for (int e=0; e<100; e++)
      {
         int type = (Ei[e][0]);
         if ((type) && (obj_filter[3][type]))
         {
            int sox = al_fixtoi(Efi[e][0]);
            int soy = al_fixtoi(Efi[e][1]);
            al_draw_rectangle(sox, soy, sox+20, soy+20, palette_color[13], 1);
         }
      }

      // is mouse on obj already in list?
      if (!mouse_on_window)
         for (int i=0; i<NUM_OBJ; i++)
         {
            obj_list[i][2] = 0; // turn off highlight by default
            if (obj_list[i][0])
            {
               int typ = obj_list[i][0];
               int num = obj_list[i][1];
               int hix=0, hiy=0;

               if (typ == 2) // item
               {
                  hix = item[num][4]/20;
                  hiy = item[num][5]/20;
               }
               if (typ == 3) // enemy
               {
                  hix = al_fixtoi(Efi[num][0]/20);
                  hiy = al_fixtoi(Efi[num][1]/20);
               }
               if ((gx == hix) && (gy == hiy)) obj_list[i][2] = 1; // turn on highlight for this list item
            }
         }

      // mark objects on map that have already been added to list
      for (int i=0; i<NUM_OBJ; i++)
      {
         if (obj_list[i][0])
         {
            int typ = obj_list[i][0];
            int num = obj_list[i][1];
            int hx=0, hy=0;
            if (typ == 2)
            {
               hx = item[num][4];
               hy = item[num][5];
            }
            if (typ == 3)
            {
               hx = al_fixtoi(Efi[num][0]);
               hy = al_fixtoi(Efi[num][1]);
            }


            if (obj_list[i][2]) al_draw_rectangle(hx-2, hy-2, hx+20+2, hy+20+2, palette_color[flash_color], 1);
            else                al_draw_rectangle(hx, hy, hx+20, hy+20,         palette_color[10], 1);
         }

      }

   //   sprintf(msg, "selection");
   //   sprintf(msg, "selection bx1:%d by1:%d bx2:%d by2:%d", bx1, by1, bx2, by2);
   //   if (mW[5].show_sel_frame) cm_show_level_buffer_block_rect(bx1, by1, bx2, by2, 14, msg);
      if (mW[5].show_sel_frame) cm_show_level_buffer_block_rect(bx1, by1, bx2, by2, 14, "selection");
      else if (!mouse_on_window) crosshairs_full(gx*20+10, gy*20+10, 15, 1);
   }
   if (level_editor_mode == 4) // ov
   {
      // if current object is message, show all messages
      if ((mW[7].obt == 2) && (item[mW[7].num][0] == 10))
      {
         for (int i=0; i<500; i++)
            if (item[i][0] == 10) draw_pop_message(i);
      }

      // if mouse on legend line, show highlight
      process_flash_color();
      mW[7].legend_line = 0;
      int y1_legend = mW[7].y2 - 34 + (5-mW[7].num_legend_lines)*8; // legend pos
      int y2_legend = y1_legend + (mW[7].num_legend_lines-1)*8;
      if ((mouse_x > mW[7].x1) && (mouse_x < mW[7].x2) && (mouse_y > y1_legend) && (mouse_y < y2_legend)) // is mouse on legend
         mW[7].legend_line = ((mouse_y - y1_legend) / 8) + 1; // which legend line are we on?

      ovw_draw_overlays(mW[7].legend_line);

   }
   if (level_editor_mode) get_new_screen_buffer(3, 0, 0);
}




int cm_draw_filter_buttons(int x1, int x2, int y1, int mode, int have_focus, int moving)
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

   if (mW[3].collapsed) tl = -1;

   int y2 = y1+tl*bts+fs*2; // pre calc

   // erase background
   al_draw_filled_rectangle(x1, y1, x2, y2, palette_color[0]);

   // draw frame around filter buttons
   int ci = 16; //color inc
   for (int q=0; q<fs; q++)
      al_draw_rectangle(x1+q, y1+q, x2-q, y2-q, palette_color[12+32+(q*ci)], 1);
   al_draw_text(font, palette_color[15], (x1+x2)/2, y1+2, ALLEGRO_ALIGN_CENTER, "Filters");

   mdw_toggle(x2-10, y1+2, x2-2, y1+10, 1000, 0,0,0,0,0,0,0,1,0,0,d, mW[3].collapsed,  "-", "+", tc1, tc2, -1, -1);


   // detect mouse click before toggles, but don't do anything until after the toggles change
   int refresh_selection = 0;
   if ((mode == 3) && (mW[4].copy_mode) && (mouse_b1)) refresh_selection = 1;



   if (!mW[3].collapsed)
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
   if (refresh_selection)
   {
      zfs_save_selection(0);
      zfs_draw_fsel();
   }
   return y2;
}



void em_set_swbl(void)
{
   mW[2].swbn = 0;
   for (int c=0; c<NUM_SPRITES; c++)
   {
      swbl[c][0] = swbl[c][1] = 0;                    // erase
      if (sa[c][0] & PM_BTILE_SHOW_SELECT_WIN)
      {
         swbl[mW[2].swbn][0] = c | sa[c][0];                // add to list with default flags
         swbl[mW[2].swbn][0] &= ~PM_BTILE_SHOW_SELECT_WIN;  // clear flag
         mW[2].swbn++;
      }
   }
   mW[2].swnbl = (mW[2].swbn / 16) + 1;
   if (mW[2].swnbl_cur == 0) mW[2].swnbl_cur = mW[2].swnbl; // initial only
}

void em_set_block_range(void)
{
   int draw_item_flags = mW[1].draw_item_num & PM_BTILE_MOST_FLAGS;
   int fsd[20][20] = {0};

   // purple pipe with open center
   fsd[0][0] = 576; // trigger blocks start
   fsd[0][1] = 592; // trigger block end
   fsd[0][9] = -1;  // default shape
   fsd[0][10] = 576; // upper left corner
   fsd[0][11] = 577; // upper right corner
   fsd[0][12] = 578; // lower left corner
   fsd[0][13] = 579; // lower right corner
   fsd[0][14] = 580; // left vertical through
   fsd[0][15] = 581; // right vertical through
   fsd[0][16] = 582; // upper horizontal through
   fsd[0][17] = 583; // lower horizontal through

   // purple pipe with solid center
   fsd[1][0] = 512; // trigger blocks start
   fsd[1][1] = 528; // trigger block end
   fsd[1][9] = 528;  // default shape
   fsd[1][10] = 512; // upper left corner
   fsd[1][11] = 513; // upper right corner
   fsd[1][12] = 514; // lower left corner
   fsd[1][13] = 515; // lower right corner
   fsd[1][14] = 516; // left vertical through
   fsd[1][15] = 517; // right vertical through
   fsd[1][16] = 518; // upper horizontal through
   fsd[1][17] = 519; // lower horizontal through

   // wires
   fsd[2][0] = 605; // trigger blocks start
   fsd[2][1] = 624; // trigger block end
   fsd[2][9] = -1;  // default shape
   fsd[2][10] = 608; // upper left corner
   fsd[2][11] = 609; // upper right corner
   fsd[2][12] = 610; // lower left corner
   fsd[2][13] = 611; // lower right corner
   fsd[2][14] = 612; // left vertical through
   fsd[2][15] = 612; // right vertical through
   fsd[2][16] = 614; // upper horizontal through
   fsd[2][17] = 614; // lower horizontal through

   // brick with corners
   fsd[3][0] = 640; // trigger blocks start
   fsd[3][1] = 656; // trigger block end
   fsd[3][9] = 656; // default shape
   fsd[3][10] = 640; // upper left corner
   fsd[3][11] = 641; // upper right corner
   fsd[3][12] = 642; // lower left corner
   fsd[3][13] = 643; // lower right corner
   fsd[3][14] = 644; // left vertical through
   fsd[3][15] = 645; // right vertical through
   fsd[3][16] = 646; // upper horizontal through
   fsd[3][17] = 647; // lower horizontal through

   // rainbows
   fsd[4][0] = 672; // trigger blocks start
   fsd[4][1] = 679; // trigger block end
   fsd[4][9] = -1;  // default shape
   fsd[4][10] = 672; // upper left corner
   fsd[4][11] = 673; // upper right corner
   fsd[4][12] = 674; // lower left corner
   fsd[4][13] = 675; // lower right corner
   fsd[4][14] = 676; // left vertical through
   fsd[4][15] = 677; // right vertical through
   fsd[4][16] = 678; // upper horizontal through
   fsd[4][17] = 679; // lower horizontal through

   // rainbows 2
   fsd[5][0] = 704; // trigger blocks start
   fsd[5][1] = 711; // trigger block end
   fsd[5][9] = -1;  // default shape
   fsd[5][10] = 704; // upper left corner
   fsd[5][11] = 705; // upper right corner
   fsd[5][12] = 706; // lower left corner
   fsd[5][13] = 707; // lower right corner
   fsd[5][14] = 708; // left vertical through
   fsd[5][15] = 709; // right vertical through
   fsd[5][16] = 710; // upper horizontal through
   fsd[5][17] = 711; // lower horizontal through

   for (int i=0; i<20; i++)
      for (int j=0; j<20; j++)
         fsd[i][j] |= draw_item_flags;


   int fsx[20][5] = {0};

   // purple pipes
   fsx[0][0] = 576; // trigger blocks start
   fsx[0][1] = 592; // trigger block end
   fsx[0][2] = 582; // middle
   fsx[0][3] = 590; // left end
   fsx[0][4] = 588; // right end

   // wires
   fsx[1][0] = 608; // trigger blocks start
   fsx[1][1] = 625; // trigger block end
   fsx[1][2] = 614; // middle
   fsx[1][3] = 622; // left end
   fsx[1][4] = 620; // right end

   // lined platform
   fsx[2][0] = 161; // trigger blocks start
   fsx[2][1] = 163; // trigger block end
   fsx[2][2] = 162; // middle
   fsx[2][3] = 161; // left end
   fsx[2][4] = 163; // right end

   // brown bricks
   fsx[3][0] = 177; // trigger blocks start
   fsx[3][1] = 179; // trigger block end
   fsx[3][2] = 178; // middle
   fsx[3][3] = 177; // left end
   fsx[3][4] = 179; // right end

   // semi-solid screen
   fsx[4][0] = 33; // trigger blocks start
   fsx[4][1] = 39; // trigger block end
   fsx[4][2] = 37; // middle
   fsx[4][3] = 36; // left end
   fsx[4][4] = 38; // right end

   for (int i=0; i<20; i++)
      for (int j=0; j<5; j++)
         fsx[i][j] |= draw_item_flags;


   int fsy[20][5] = {0};

   // purple pipes
   fsy[0][0] = 576; // trigger blocks start
   fsy[0][1] = 592; // trigger block end
   fsy[0][2] = 580; // middle
   fsy[0][3] = 591; // upper end
   fsy[0][4] = 589; // lower end

   // wires
   fsy[1][0] = 608; // trigger blocks start
   fsy[1][1] = 625; // trigger block end
   fsy[1][2] = 612; // middle
   fsy[1][3] = 623; // upper end
   fsy[1][4] = 621; // lower end

   // semi-solid screen
   fsy[4][0] = 33; // trigger blocks start
   fsy[4][1] = 39; // trigger block end
   fsy[4][2] = 34; // middle
   fsy[4][3] = 33; // upper end
   fsy[4][4] = 35; // lower end

   for (int i=0; i<20; i++)
      for (int j=0; j<5; j++)
         fsy[i][j] |= draw_item_flags;


   if ((bx2==bx1) && (by2==by1)) l[bx1][by1] = mW[1].draw_item_num; // single block 1 x 1

   if ((bx2==bx1) && (by2-by1>0)) // vertical line 1 x >1
   {
      int a = bx1;
      for (int b=by1; b<by2+1; b++) // cycle the range
      {
         l[a][b] = mW[1].draw_item_num; // set draw item as default
         for (int x=0; x<20; x++)
            if (fsy[x][0]&1023)
            {
               if (((mW[1].draw_item_num&1023) >= (fsy[x][0]&1023)) && ((mW[1].draw_item_num&1023) <= (fsy[x][1]&1023)))
               {
                  l[a][b] = fsy[x][2]; // default
                  if (b == by1) l[a][b] = fsy[x][3]; // left end cap
                  if (b == by2) l[a][b] = fsy[x][4]; // right end cap
               }
            }
      }
   }
   if ((bx2-bx1>0) && (by2==by1)) // horizontal line >1 x 1
   {
      int b = by1;
      for (int a=bx1; a<bx2+1; a++) // cycle the range
      {
         l[a][b] = mW[1].draw_item_num; // set draw item as default
         for (int x=0; x<20; x++)
         {
            if (fsx[x][0]&1023)
            {
               if (((mW[1].draw_item_num&1023) >= (fsx[x][0]&1023)) && ((mW[1].draw_item_num&1023) <= (fsx[x][1]&1023)))
               {
                  l[a][b] = fsx[x][2]; // default
                  if (a == bx1) l[a][b] = fsx[x][3]; // left end cap
                  if (a == bx2) l[a][b] = fsx[x][4]; // right end cap
               }
            }
         }
      }
   }
   if ((bx2-bx1>0) && (by2-by1>0)) // box shape with corners >1 x >1
   {
      int special_handler = 0;
      for (int a=bx1; a<bx2+1; a++)       // cycle the range
         for (int b=by1; b<by2+1; b++)
            for (int x=0; x<20; x++)
               if (fsd[x][0]&1023)
               {
                  if (((mW[1].draw_item_num&1023) >= (fsd[x][0]&1023)) && ((mW[1].draw_item_num&1023) <= (fsd[x][1]&1023)))
                  {
                     special_handler = 1;

                     if (fsd[x][9] != -1) l[a][b] = fsd[x][9];   // default block
                     if (b == by1)        l[a][b] = fsd[x][16];  // upper horizontal through
                     if (b == by2)        l[a][b] = fsd[x][17];  // lower horizontal through


                     if (a == bx1)
                     {
                        if (b == by1)      l[a][b] = fsd[x][10];  // upper-right corner
                        else if (b == by2) l[a][b] = fsd[x][12];  // lower-right corner
                        else               l[a][b] = fsd[x][14];  // right vertical through
                     }
                     if (a == bx2)
                     {
                        if (b == by1)      l[a][b] = fsd[x][11];  // upper-left corner
                        else if (b == by2) l[a][b] = fsd[x][13];  // lower-left corner
                        else               l[a][b] = fsd[x][15];  // left vertical through
                     }
                  }
                  if (!special_handler) l[a][b] = mW[1].draw_item_num;

             } // end of cycle block range
   } // end of box shape with corners
}

char* em_get_text_description_of_block_based_on_flags(int flags)
{
   sprintf(msg, "Empty");  // default

   if (flags & PM_BTILE_SOLID_PLAYER)     sprintf(msg, "Solid");
   if (flags & PM_BTILE_SEMISOLID_PLAYER) sprintf(msg, "Semi-Solid");
   if (flags & PM_BTILE_BREAKABLE_PBUL)   sprintf(msg, "Breakable");
   if (flags & PM_BTILE_BOMBABLE)         sprintf(msg, "Bombable");
   if (flags & PM_BTILE_LADDER_MOVE)      sprintf(msg, "Ladder");
   if (flags & PM_BTILE_ROPE_MOVE)        sprintf(msg, "Rope");
   return msg;
}

void em_show_draw_item_cursor(void)
{
   int x = gx;
   int y = gy;
   if (mW[1].point_item_type > -1) // if mouse pointer on window, do not show draw item
   {
      int type = mW[1].draw_item_type;
      int num = mW[1].draw_item_num;
      switch (type)
      {
         case 1: // block
            if (mW[1].show_non_default_blocks) draw_block_non_default_flags(num, x*20, y*20);
            else al_draw_bitmap(btile[num&1023], x*20, y*20, 0);
         break;
         case 2: // item
            draw_item(num, 1, x*20, y*20);
         break;
         case 3: // enemy
            draw_enemy(num, 1, x*20, y*20);
         break;
         case 5: // PDE
            int a = PDEi[num][1]; // bmp or ans
            if (a > 999) a = zz[5][a-1000]; // ans
            al_draw_bitmap(tile[a], x*20, y*20, 0);
         break;
      }
      al_draw_rectangle(x*20, y*20, x*20+21, y*20+21, palette_color[15], 1);
   }
}

void em_show_item_info(int x, int y, int color, int type, int num)
{
   int a, b;
   switch (type)
   {
      case 1:
         if (mW[1].show_non_default_blocks) draw_block_non_default_flags(num, x, y);
         else al_draw_bitmap(btile[num&1023], x, y, 0);
         al_draw_textf(font, palette_color[color], x+22, y+2, 0, "Block #%d",num&1023);
         al_draw_textf(font, palette_color[color], x+22, y+12, 0, "%s", em_get_text_description_of_block_based_on_flags(num) );
      break;
      case 2:
         draw_item(num, 1, x, y);
         a = item[num][0]; // type
         al_draw_textf(font, palette_color[color], x+22, y+2, 0, "%s", item_name[a]);
         al_draw_textf(font, palette_color[color], x+22, y+12, 0, "%d of %d", 1+num - item_first_num[a], item_num_of_type[a]);
      break;
      case 3:
         draw_enemy(num, 1, x, y);
         a = Ei[num][0]; // type
         al_draw_textf(font, palette_color[color], x+22, y+2, 0, "%s", enemy_name[a]);
         al_draw_textf(font, palette_color[color], x+22, y+12, 0, "%d of %d", 1+num - e_first_num[a], e_num_of_type[a]);
      break;
      case 4:
      {
         int col = lifts[num].color;
         int width = lifts[num].width;
         if (width > 140) width = 140;
         for (a=0; a<10; a++)
            al_draw_rectangle(x+a, y+a, x+(width)-1-a, y+19-a, palette_color[col+((9-a)*16)], 1 );
         al_draw_text(font, palette_color[col+160], x+(width/2), y+6, ALLEGRO_ALIGN_CENTER, lifts[num].lift_name);
      }
      break;
      case 5:
         a = PDEi[num][1]; // bmp or ans
         if (a < NUM_SPRITES) b = a; // bmp
         if (a > 999) b = zz[5][a-1000]; // ans
         al_draw_bitmap(tile[b], x, y, 0);
         a = Ei[num][0]; // type
         al_draw_text(font, palette_color[color], x+22, y+2, 0, "Special Item");
         al_draw_textf(font, palette_color[color], x+22, y+12, 0, "%s", PDEt[num][1]);
      break;
   }
}

void em_find_point_item(void)
{
   // find point item
   mW[1].point_item_type = 1; // block by default
   mW[1].point_item_num = l[gx][gy];

   int max_ob = 20;                  // max objects to find
   int ob = 0;                       // objects found
   int mo[max_ob][2];                // array of objects found
   for (int a=0; a<max_ob; a++)          // clear array
   {
       mo[a][0] = 0;
       mo[a][1] = 1;
   }
   for (int i=0; i<500; i++) // check for item
      if ((item[i][0]) && (ob < max_ob))
      {
         int x = item[i][4];
         int y = item[i][5];
         if ( (hx >= x) && (hx <= x+19) && (hy > y) && (hy < y+19) && (ob < max_ob))
         {
             mo[ob][0] = 2;
             mo[ob][1] = i;
             ob++;
         }
      }
   for (int e=0; e<100; e++) // check for enemy
      if ((Ei[e][0]) && (ob < max_ob))
      {
         int x = al_fixtoi(Efi[e][0]);
         int y = al_fixtoi(Efi[e][1]);
         if ( (hx >= x) && (hx <= x+19) && (hy > y) && (hy < y+19) && (ob < max_ob))
         {
             mo[ob][0] = 3;
             mo[ob][1] = e;
             ob++;
         }
      }
   for (int l=0; l<num_lifts; l++) // check for lifts
   {
      int x = lifts[l].x1;
      int y = lifts[l].y1;
      if ( (hx >= x) && (hx <= x+19) && (hy > y) && (hy < y+19) && (ob < max_ob))
      {
          mo[ob][0] = 4;
          mo[ob][1] = l;
          ob++;
      }
   }
   //al_draw_textf(font, palette_color[15], 100, 100, 0, "mouse is on: %d objects", ob);
   //for (int a=0; a<ob; a++) al_draw_textf(font, palette_color[15], 100, 108+a*8, 0, "%d %d ", mo[a][0], mo[a][1]);

   // which one is the winner?
   if (ob)
   {
      int mm = mouse_x % 20;         // mouse position relative to block boundary
      int ss = 20/ob;                // step space
      int of = mm / ss;              // convert to offset into ob array
      mW[1].point_item_type = mo[of][0];
      mW[1].point_item_num  = mo[of][1];
      //al_draw_textf(font, palette_color[11], 100, 92, 0, "mm:%2d ss:%2d of:%2d  ", mm, ss, of);
   }
}

int em_process_mouse(void)
{
   int quit=0;

   if (mouse_b1)
   {
      // don't allow drag draw selection unless draw type is block
      if (mW[1].draw_item_type != 1) while (mouse_b1) proc_controllers();

      int din = mW[1].draw_item_num; // shorter variable name
      switch (mW[1].draw_item_type)
      {
         case 1:  // block
         {
            bx1 = gx;
            by1 = gy;
            cm_get_new_box();
            em_set_block_range();
         }
         break;
         case 2:  // item
         {
            int type = item[din][0];
            int ofx = gx*20 - item[din][4]; // get offset of move in 2000 format
            int ofy = gy*20 - item[din][5];
            int c = get_empty_item(type); // get a place to put it
            if (c == -1)  break;
            for (int b=0; b<16; b++) item[c][b] = item[din][b]; // copy from draw item
            item[c][4] += ofx; // adjust with offsets
            item[c][5] += ofy;
            if ((type == 4) || (type == 9) || (type == 16) || (type == 17)) // move range for key, trig, manip, damage
            {
               item[c][6] += ofx; // adjust with offsets
               item[c][7] += ofy;
            }
            if (type == 10)
            {
               item[c][10] += ofx; // adjust with offsets
               item[c][11] += ofy;
               strcpy(pmsgtext[c], pmsgtext[din]); // msg
            }
            sort_item(1);
         }
         break;
         case 3:    // enemy
         {
            int type = Ei[din][0];

            int ofx = gx*20 - al_fixtoi(Efi[din][0]); // get offset of move in 2000 format
            int ofy = gy*20 - al_fixtoi(Efi[din][1]);

            int c = get_empty_enemy(type); // get a place to put it
            if (c == -1)  break;
            for (int x=0; x<32; x++) Ei[c][x]  = Ei[din][x];
            for (int x=0; x<16; x++) Efi[c][x] = Efi[din][x];

            Efi[c][0] += al_itofix(ofx);  // apply offsets
            Efi[c][1] += al_itofix(ofy);


            if (type == 7) // podzilla
            {
               if ((key[ALLEGRO_KEY_LSHIFT]) || (key[ALLEGRO_KEY_RSHIFT])) // move stuff also
               //if (al_show_native_message_box(display, "Move?", "Move podzilla's extended position too?", NULL, NULL, ALLEGRO_MESSAGEBOX_YES_NO | ALLEGRO_MESSAGEBOX_QUESTION ) == 1)
               {
                   Efi[c][5] = Efi[din][5] + al_itofix(ofx);
                   Efi[c][6] = Efi[din][6] + al_itofix(ofy);
               }
               if ((key[ALLEGRO_KEY_LSHIFT]) || (key[ALLEGRO_KEY_RSHIFT])) // move stuff also
               //if (al_show_native_message_box(display, "Move?", "Move podzilla's trigger box too?", NULL, NULL, ALLEGRO_MESSAGEBOX_YES_NO | ALLEGRO_MESSAGEBOX_QUESTION ) == 1)
               {
                  Ei[c][11] = Ei[din][11] + ofx;
                  Ei[c][12] = Ei[din][12] + ofy;
               }
               recalc_pod(c);
            }
            if (type == 9) // cloner
            {
               //if (al_show_native_message_box(display, "Move?", "Move cloner's source and destination boxes too?", NULL, NULL, ALLEGRO_MESSAGEBOX_YES_NO | ALLEGRO_MESSAGEBOX_QUESTION ) == 1)
               if ((key[ALLEGRO_KEY_LSHIFT]) || (key[ALLEGRO_KEY_RSHIFT])) // move stuff also
               {
                  Ei[c][15] = Ei[din][15] + ofx;
                  Ei[c][16] = Ei[din][16] + ofy;
                  Ei[c][17] = Ei[din][17] + ofx;
                  Ei[c][18] = Ei[din][18] + ofy;
               }
               //if (al_show_native_message_box(display, "Move?", "Move cloner's trigger box too?", NULL, NULL, ALLEGRO_MESSAGEBOX_YES_NO | ALLEGRO_MESSAGEBOX_QUESTION ) == 1)
               if ((key[ALLEGRO_KEY_LSHIFT]) || (key[ALLEGRO_KEY_RSHIFT])) // move stuff also
               {
                  Ei[c][11] = Ei[din][11] + ofx;
                  Ei[c][12] = Ei[din][12] + ofy;
               }
            }
            sort_enemy();
         }
         break;
         case 5: // Special
         if ((PDEi[din][0] > 99) && (PDEi[din][0] < 200)) // PDE item
         {
            int d = get_empty_item(); // get a place to put it
            if (d == -1)  break;
            // copy from pde
            for (int x=0; x<16; x++) // item
               item[d][x] = PDEi[din][x];
            item[d][0] -= 100;
            item[d][4] = gx*20;
            item[d][5] = gy*20;
            if (item[d][0] == 4)
            {
               itemf[d][0] = al_itofix(item[d][4]);
               itemf[d][1] = al_itofix(item[d][5]);
               get_block_range("Block Range", &item[d][6], &item[d][7], &item[d][8], &item[d][9], 1);
            }
            sort_item(1);
         }
         if (PDEi[din][0] < 99) // PDE enemy
         {
            int d = get_empty_enemy(); // get a place to put it
            if (d == -1)  break;
            for (int x=0; x<32; x++) Ei[d][x]  = PDEi[din][x];
            for (int x=0; x<16; x++) Efi[d][x] = PDEfx[din][x];
            Efi[d][0] = al_itofix(gx*20);  // set new x,y
            Efi[d][1] = al_itofix(gy*20);
            sort_enemy();
         }
         break;
      } // end of switch case
   } // end of mouse_b1
   if (mouse_b2)
   {
      switch (mW[1].point_item_type)
      {
         case 1:
            sprintf(global_string[2][2], "Copy Block    ");
            sprintf(global_string[2][3], "              ");
            sprintf(global_string[2][4], "                ");
         break;
         case 2:
            sprintf(global_string[2][2], "Copy %s  ",  item_name[item[mW[1].point_item_num][0]]);
            sprintf(global_string[2][3], "View %s  ",  item_name[item[mW[1].point_item_num][0]]);
            sprintf(global_string[2][4], "Delete %s ", item_name[item[mW[1].point_item_num][0]]);
         break;
         case 3:
            sprintf(global_string[2][2], "Copy %s  ",  (const char *)enemy_name[Ei[mW[1].point_item_num][0]]);
            sprintf(global_string[2][3], "View %s  ",  (const char *)enemy_name[Ei[mW[1].point_item_num][0]]);
            sprintf(global_string[2][4], "Delete %s ", (const char *)enemy_name[Ei[mW[1].point_item_num][0]]);
         break;
         case 4:
            sprintf(global_string[2][2], "              ");
            sprintf(global_string[2][3], "View Lift '%s'",   lifts[mW[1].point_item_num].lift_name);
            sprintf(global_string[2][4], "Delete Lift '%s'", lifts[mW[1].point_item_num].lift_name);
         break;
      }

      switch (pmenu(2, 0))
      {
         case 2:  // copy
            if (mW[1].point_item_type < 4)
            {
               mW[1].draw_item_type = mW[1].point_item_type;
               mW[1].draw_item_num  = mW[1].point_item_num;
            }
         break;
         case 3:  // view
            if (mW[1].point_item_type > 1) object_viewerw(mW[1].point_item_type, mW[1].point_item_num);
         break;
         case 4:  // delete
            switch (mW[1].point_item_type)
            {
               case 1: // delete block
                    l[gx][gy] = 0;
               break;
               case 2: // delete item
                  if ((mW[1].draw_item_type == 2) && (mW[1].draw_item_num == mW[1].point_item_num)) // are you deleting the draw item?
                  {
                     mW[1].draw_item_type = 1;
                     mW[1].draw_item_num = 0;
                  }
                  erase_item(mW[1].point_item_num);
                  sort_item(1);
               break;
               case 3: // delete enemy
                  if ((mW[1].draw_item_type == 3) && (mW[1].draw_item_num == mW[1].point_item_num)) // are you deleting the draw item?
                  {
                     mW[1].draw_item_type = 1;
                     mW[1].draw_item_num = 0;
                  }
                  erase_enemy(mW[1].point_item_num);
                  sort_enemy();
               break;
               case 4: // delete lift
                  erase_lift(mW[1].point_item_num);
               break;
            }
         break;
         case 5: break; // menu divider
         case 6: // zoom full screen
         {
            int draw_item;
            if (mW[1].draw_item_type == 1) draw_item = mW[1].draw_item_num;
            else draw_item = 0;
            zoom_full_screen(draw_item);
         }
         break;
         case 7: group_edit(); break;
         case 8: mW[1].active = 1; break; // status_window
         case 9: mW[2].active = 1; break; // select_window
         case 11: // new level
         if (al_show_native_message_box(display, "New Level", "Clicking OK will create a new blank level", NULL, NULL, ALLEGRO_MESSAGEBOX_OK_CANCEL) == 1)
         {
            zero_level_data();
            save_level_prompt();
         }
         load_level(last_level_loaded, 0); // blind load
         break;
         case 12: // load level
            load_level_prompt();
            sort_enemy();
            sort_item(1);
         break;
         case 13: save_level_prompt(); break; // save level
         case 14: if (save_level_prompt()) quit=1; break; // save and exit
         case 15: help("Level Editor Basics"); break;// help
         case 16: quit = 1; break; // exit
         case 18: predefined_enemies(); break;
         case 19: global_level(); break;
         case 20: level_viewer(); break;
         case 21: animation_sequence_editor(); break;
         case 22: copy_tiles(); break;
         case 23: edit_btile_attributes(); break;

      } // end of switch case
   } // end of mouse_b2
   return quit;
}

int em_process_keypress(void)
{
   int quit = 0;
   if (key[ALLEGRO_KEY_B])
   {
      while (key[ALLEGRO_KEY_B]) proc_controllers();
      printf("load bookmark\n");
      if (bookmark_level == last_level_loaded)
      {
         if ((bookmark_obj == 2) && (item[bookmark_num]))       object_viewerw(2, bookmark_num);
         if ((bookmark_obj == 3) && (Ei[bookmark_num]))         object_viewerw(3, bookmark_num);
         if ((bookmark_obj == 4) && (bookmark_num < num_lifts)) object_viewerw(4, bookmark_num);
      }
   }
   if (key[ALLEGRO_KEY_I])
   {
      while (key[ALLEGRO_KEY_I]) proc_controllers();
      show_all_items();
   }
   if (key[ALLEGRO_KEY_E])
   {
      while (key[ALLEGRO_KEY_E]) proc_controllers();
      show_all_enemies();
   }
   if (key[ALLEGRO_KEY_L])
   {
      while (key[ALLEGRO_KEY_L]) proc_controllers();
      show_all_lifts();
   }
   if (key[ALLEGRO_KEY_S])
   {
      while (key[ALLEGRO_KEY_S]) proc_controllers();
      level_check();
   }
   if (key[ALLEGRO_KEY_P])
   {
      while (key[ALLEGRO_KEY_P]) proc_controllers();
      show_all_pmsg();
   }
   if (key[ALLEGRO_KEY_ESCAPE])
   {
      while (key[ALLEGRO_KEY_ESCAPE]) proc_controllers();
      if (al_show_native_message_box(display, "Save?", "Save before exit?", NULL, NULL, ALLEGRO_MESSAGEBOX_OK_CANCEL) == 1) save_level(last_level_loaded);
      quit = 1;
   }
   return quit;
}

int edit_menu(int el)
{
   set_windows(0);
   load_mW();
   if (!el) load_level_prompt(); // load prompt
   else load_level(el, 0);       // blind load
   al_show_mouse_cursor(display);
   level_editor_running = 1;
   resume_allowed = 0;
   mW[1].draw_item_type = 1;
   mW[1].draw_item_num  = 0;
   load_PDE();
   sort_enemy();
   sort_item(1);
   em_set_swbl();
   set_frame_nums(0);
   for (int k = ALLEGRO_KEY_A; k < ALLEGRO_KEY_MAX; k++) key[k] = 0; // clear_key array
   int quit=0;

   if (autoload_bookmark)
   {
      printf("load bookmark\n");
      if (bookmark_level == last_level_loaded)
      {
         if ((bookmark_obj == 2) && (item[bookmark_num]))       object_viewerw(2, bookmark_num);
         if ((bookmark_obj == 3) && (Ei[bookmark_num]))         object_viewerw(3, bookmark_num);
         if ((bookmark_obj == 4) && (bookmark_num < num_lifts)) object_viewerw(4, bookmark_num);
      }
   }
   while (!quit)
   {
      cm_redraw_level_editor_background();
      if (!mw_cycle_windows(0)) quit = em_process_mouse();
      quit = em_process_keypress();
   }
   level_editor_running = 0;
   al_hide_mouse_cursor(display);
   save_mW();
   return last_level_loaded;
}

