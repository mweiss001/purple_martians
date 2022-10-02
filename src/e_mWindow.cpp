// e_mWindows.cpp
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
   int swb = SCREEN_W-bw;
   int shb = SCREEN_H-bw;

   if (mouse_y > shb) WY+=(mouse_y - shb)*2; // scroll down
   if (mouse_x > swb) WX+=(mouse_x - swb)*2; // scroll right
   if (mouse_x < bw)  WX-=(bw - mouse_x)*2;  // scroll left
   if (mouse_y < 4)   WY-=(4  - mouse_y)*7;  // scroll up (is different because of menu)

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
// blocks while mouse b1 is pressed
void cm_get_new_box(void)
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
   int old_lem = mW[8].level_editor_mode;
   mW[8].level_editor_mode = mode;
   cm_redraw_level_editor_background();
   mW[8].level_editor_mode = old_lem;
}


void cm_process_mouse(void)
{
   if (mW[8].level_editor_mode == 1) em_process_mouse();
   if (mW[8].level_editor_mode == 2) zfs_process_mouse();
   if (mW[8].level_editor_mode == 3) ge_process_mouse();
   if (mW[8].level_editor_mode == 4) ovw_process_mouse();
   if (mW[8].level_editor_mode == 9) th_process_mouse();
}

void cm_process_keypress(void)
{
   if (mW[8].level_editor_mode == 1) em_process_keypress();
   if (mW[8].level_editor_mode == 4) ovw_process_keypress();
   if ((mW[8].level_editor_mode == 2) || (mW[8].level_editor_mode == 3) || (mW[8].level_editor_mode == 9)) // zfs, ge or th
   {
      while (key[ALLEGRO_KEY_ESCAPE]) { proc_controllers(); mW[8].active = 0; }
   }
   if (mW[8].active == 0)
   {
      int ret = exit_level_editor_dialog();

      if (ret == 0) save_level(last_level_loaded); // save and exit

      if (ret == 2) mW[8].active = 1; // cancel


//
//      if (al_show_native_message_box(display, "Save?", "Save before exit?", NULL, NULL, ALLEGRO_MESSAGEBOX_OK_CANCEL) == 1)
//         save_level(last_level_loaded);



   }
}



void cm_redraw_level_editor_background(void)
{
   process_flash_color();

   int mouse_on_window = is_mouse_on_any_window();

   cm_process_scrolledge();

   if ((!mouse_on_window) || (mW[8].level_editor_mode == 0) || (mW[8].level_editor_mode == 4)) cm_get_block_position_on_map();

   al_flip_display();
   proc_scale_factor_change();
   proc_controllers();
   proc_frame_delay();

   if (frame_num <  al_get_timer_count(fps_timer)) set_frame_nums(frame_num); // set fps_timer count to frame_num

   if (draw_frame)
   {
      get_new_background(0);
      draw_lifts();
      draw_items();
      draw_enemies();
   }
   else al_set_target_bitmap(level_buffer);


   if (mW[8].level_editor_mode == 1) // edit menu
   {
      if (!mouse_on_window) em_show_draw_item_cursor();
   }

   if (mW[8].level_editor_mode == 2) // zfs
   {
      // show selection
      if (!mW[4].copy_mode) cm_show_level_buffer_block_rect(bx1, by1, bx2, by2, 14, "selection");

      // only show if mouse not on window
      if (!mouse_on_window)
      {
         if (mW[4].brf_mode) crosshairs_full(gx*20+10, gy*20+10, 15, 1);
         if (mW[4].copy_mode)
         {
            if (ft_bmp)
            {
               al_draw_bitmap(ft_bmp, gx*20, gy*20, 0);
               cm_show_level_buffer_block_rect(gx, gy, gx+mW[4].sw-1, gy+mW[4].sh-1, 10, "paste");
            }
            else mW[4].copy_mode = 0;
         }
      }
   }
   if (mW[8].level_editor_mode == 3) // ge
   {
      int x=0, y=0;

      // show selection frame
      if (mW[5].show_sel_frame) cm_show_level_buffer_block_rect(bx1, by1, bx2, by2, 14, "selection");
      else if (!mouse_on_window) crosshairs_full(gx*20+10, gy*20+10, 15, 1);

      // mark objects on map that are capable of being added to list
      for (int i=0; i<500; i++)
      {
         int type = (item[i][0]);
         if ((type) && (obj_filter[2][type]))
         {
            x = item[i][4];
            y = item[i][5];
            al_draw_rectangle(x, y, x+20, y+20, palette_color[13], 1);
         }
      }
      for (int e=0; e<100; e++)
      {
         int type = (Ei[e][0]);
         if ((type) && (obj_filter[3][type]))
         {
            x = al_fixtoi(Efi[e][0]);
            y = al_fixtoi(Efi[e][1]);
            al_draw_rectangle(x, y, x+20, y+20, palette_color[13], 1);
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
               if (typ == 2) // item
               {
                  x = item[num][4]/20;
                  y = item[num][5]/20;
               }
               if (typ == 3) // enemy
               {
                  x = al_fixtoi(Efi[num][0]/20);
                  y = al_fixtoi(Efi[num][1]/20);
               }
               if ((gx == x) && (gy == y)) obj_list[i][2] = 1; // turn on highlight for this list item
            }
         }

      // mark objects on map that have already been added to list
      for (int i=0; i<NUM_OBJ; i++)
      {
         if (obj_list[i][0])
         {
            int typ = obj_list[i][0];
            int num = obj_list[i][1];
            if (typ == 2)
            {
               x = item[num][4];
               y = item[num][5];
            }
            if (typ == 3)
            {
               x = al_fixtoi(Efi[num][0]);
               y = al_fixtoi(Efi[num][1]);
            }
            if (obj_list[i][2]) al_draw_rectangle(x-2, y-2, x+20+2, y+20+2, palette_color[flash_color], 1); // highlight
            else                al_draw_rectangle(x,   y,   x+20,   y+20,   palette_color[10], 1);
         }
      }
   }

   if (mW[8].level_editor_mode == 4) // ov
   {
      // if current object is message, show all messages
      if ((mW[7].obt == 2) && (item[mW[7].num][0] == 10))
      {
         for (int i=0; i<500; i++)
            if (item[i][0] == 10) draw_pop_message(i, 0, 0, 0, 0, 0, msg);
      }

      // if mouse on legend line, show highlight
      mW[7].legend_line = 0;
      int y1_legend = mW[7].y2 - 34 + (5-mW[7].num_legend_lines)*8; // legend pos
      int y2_legend = y1_legend + (mW[7].num_legend_lines-1)*8;
      if ((mouse_x > mW[7].x1) && (mouse_x < mW[7].x2) && (mouse_y > y1_legend) && (mouse_y < y2_legend)) // is mouse on legend
         mW[7].legend_line = ((mouse_y - y1_legend) / 8) + 1; // which legend line are we on?

      ovw_draw_overlays(mW[7].legend_line);

   }


   if (mW[8].level_editor_mode == 9) // th
   {
      // show marked blocks
      for (int x=0; x<100; x++)
         for (int y=0; y<100; y++)
         {
            if (thl[x][y])
            {
               //int col = 10;
               int c = flash_color+64;
               int c2 = flash_color2+64;
               al_draw_rectangle(x*20+0.5, y*20+0.5, x*20+20, y*20+20,   palette_color[c2], 0);
               al_draw_line(x*20, y*20, x*20+20, y*20+20,   palette_color[c], 0);
               al_draw_line(x*20+20, y*20, x*20, y*20+20,   palette_color[c], 0);





            }
         }
   }


   if (mW[8].level_editor_mode) get_new_screen_buffer(3, 0, 0);
}


void cm_process_menu_bar(int have_focus, int moving, int draw_only)
{

  al_set_target_backbuffer(display);


   mW[8].set_pos(0, 0);
   mW[8].set_size(SCREEN_W, BORDER_WIDTH);

   int x1 = BORDER_WIDTH;
   int y1 = 0;

   int by1 = y1+4;
   int bts = 10;

   int d = 1;
   if (have_focus) d = 0;
   if (moving) d = 1;
   if (draw_only) d = 1;

//   sprintf(msg, "FPS set:%d act:%d", frame_speed, actual_fps);
//   sprintf(msg, "frame skip:[%d]  FPS set:%d act:%d", frames_skipped_last_second, frame_speed, actual_fps);
//   al_draw_text(font, palette_color[15], SCREEN_W - (strlen(msg)+2) * 8, 2, 0, msg);


   if (mdw_buttont(x1, by1, x1+32, bts, 0,0,0,0, 0,-1,15,0, 0,0,0,d, "File"))
   {
      strcpy (global_string[5][0],"File"); // PD sub menu
      strcpy (global_string[5][1],"New");
      strcpy (global_string[5][2],"Load");
      strcpy (global_string[5][3],"Reload");
      strcpy (global_string[5][4],"Save");
      strcpy (global_string[5][5],"Save As");
      strcpy (global_string[5][6],"Exit");
      strcpy (global_string[5][7],"end");
      int ret = tmenu(5, 1, x1, by1-1);
      if (ret == 1)
      {
         if (al_show_native_message_box(display, "New Level", "Clicking OK will create a new blank level", NULL, NULL, ALLEGRO_MESSAGEBOX_OK_CANCEL) == 1)
         {
            zero_level_data();
            save_level_prompt();
         }
         load_level(last_level_loaded, 0);
      }
      if (ret == 2) load_level_prompt();
      if (ret == 3) load_level(last_level_loaded, 0);
      if (ret == 4) save_level(last_level_loaded);
      if (ret == 5) save_level_prompt();
      if (ret == 6) mW[8].active = 0;
   }
   x1 += 44;


   if (mdw_buttont(x1, by1, x1+32, bts, 0,0,0,0, 0,-1,15,0, 0,0,0,d, "View"))
   {
      strcpy (global_string[5][0],"View");
      strcpy (global_string[5][1],"Toggle Fullscreen       F12");
      strcpy (global_string[5][2],"Zoom Out                 F5");
      strcpy (global_string[5][3],"Zoom In                  F6");
      strcpy (global_string[5][4],"Reset Zoom            F5+F6");
      sprintf(global_string[5][5],"Text Double:Auto");
      sprintf(global_string[5][6],"Text Double:1");
      sprintf(global_string[5][7],"Text Double:2");
      sprintf(global_string[5][8],"Text Double:3");
      strcpy (global_string[5][9],"end");
      int ret = tmenu(5, 1, x1, by1-1);
      if (ret == 1)
      {
         if (fullscreen) proc_display_change_fromfs();
         else            proc_display_change_tofs();
      }
      if (ret == 2) set_scale_factor(scale_factor * .90, 0);
      if (ret == 3) set_scale_factor(scale_factor * 1.1, 0);
      if (ret == 4) set_scale_factor(1.0, 0);
      if (ret == 5) set_saved_display_transform(0);
      if (ret == 6) set_saved_display_transform(1);
      if (ret == 7) set_saved_display_transform(2);
      if (ret == 8) set_saved_display_transform(3);
   }
   x1 += 44;

   if (mdw_buttont(x1, by1, x1+40, bts, 0,0,0,0, 0,-1,15,0, 0,0,0,d, "Lists"))
   {
      strcpy (global_string[5][0],"Lists");

      strcpy (global_string[5][1],"List all Items");
      strcpy (global_string[5][2],"List all Enemies");
      strcpy (global_string[5][3],"List all Lifts");
      strcpy (global_string[5][4],"List all pmsg");
      strcpy (global_string[5][5],"List all Events");
      strcpy (global_string[5][6],"Level Check");
      strcpy (global_string[5][7],"Show Level Data");
      strcpy (global_string[5][8],"end");
      int ret = tmenu(5, 1, x1, by1-1);
      if (ret == 1) show_all_items();
      if (ret == 2) show_all_enemies();
      if (ret == 3) show_all_lifts();
      if (ret == 4) show_all_pmsg();
      if (ret == 5) show_all_events();
      if (ret == 6) level_check();
      if (ret == 7) show_level_data();

   }
   x1 += 52;


   if (mdw_buttont(x1, by1, x1+64, bts, 0,0,0,0, 0,-1,15,0, 0,0,0,d, "Advanced"))
   {
      strcpy (global_string[5][0],"Advanced");
      strcpy (global_string[5][1],"Predefined Enemy Editor");
      strcpy (global_string[5][2],"Global Level Thingy!!");
      strcpy (global_string[5][3],"Level Viewer!");
      strcpy (global_string[5][4],"Animation Sequence Editor");
      strcpy (global_string[5][5],"Copy Tiles");
      strcpy (global_string[5][6],"Default Flag Editor");
      strcpy (global_string[5][7],"end");
      int ret = tmenu(5, 1, x1, by1-1);
      if (ret == 1) predefined_enemies();
      if (ret == 2) global_level();
      if (ret == 3) level_viewer();
      if (ret == 4) animation_sequence_editor();
      if (ret == 5) copy_tiles();
      if (ret == 6) edit_btile_attributes();
   }
   x1 += 76;

   if (mdw_buttont(x1, by1, x1+32, bts, 0,0,0,0, 0,-1,15,0, 0,0,0,d, "Help"))
   {
      strcpy (global_string[5][0],"Help");
      strcpy (global_string[5][1],"Level Editor Basics");
      strcpy (global_string[5][2],"Credits");
      strcpy (global_string[5][3],"end");
      int ret = tmenu(5, 1, x1, by1-1);
      if (ret == 1) help("Level Editor Basics");
      if (ret == 2) help("Credits");
   }
   x1 += 44;

   x1+= 12;

   if (mW[8].level_editor_mode == 1) sprintf(msg, "Mode:Main Edit");
   if (mW[8].level_editor_mode == 2) sprintf(msg, "Mode:Zoom Fullscreen");
   if (mW[8].level_editor_mode == 3) sprintf(msg, "Mode:Group Edit");
   if (mW[8].level_editor_mode == 4) sprintf(msg, "Mode:Object Viewer");
   if (mW[8].level_editor_mode == 9) sprintf(msg, "Mode:Tile Helper");

   if (mdw_buttont(x1, by1, x1+140, bts, 0,0,0,0, 0,-1,15,0, 0,1,0,d, msg))
   {
      strcpy (global_string[5][0],msg);
      strcpy (global_string[5][1],"Mode:Main Edit");
      strcpy (global_string[5][2],"Mode:Zoom Fullscreen");
      strcpy (global_string[5][3],"Mode:Group Edit");
      strcpy (global_string[5][4],"Mode:Object Viewer");
      strcpy (global_string[5][5],"Mode:Tile Helper");
      strcpy (global_string[5][6],"end");
      int ret = tmenu(5, 1, x1+4, by1-1);
      if (ret == 1) set_windows(1);
      if (ret == 2) set_windows(2);
      if (ret == 3) set_windows(3);
      if (ret == 4) set_windows(4);
      if (ret == 5) set_windows(9);
   }



   // status display in the lower right border
   int y2 = SCREEN_H-BORDER_WIDTH+3;
   x1 = SCREEN_W-172;
   al_draw_textf(font, palette_color[9],  x1+2,  y2, 0, "Level:");
   al_draw_textf(font, palette_color[15], x1+50, y2, 0, "%d ",last_level_loaded);
   x1 += 80;

   al_draw_text( font, palette_color[9],  x1,    y2, 0, "x:");
   al_draw_text( font, palette_color[9],  x1+40, y2, 0, "y:");
   if (is_mouse_on_any_window()) al_draw_textf(font, palette_color[15], x1, y2, 0, "  --   -- ");
   else                          al_draw_textf(font, palette_color[15], x1, y2, 0, "  %-2d   %-2d ", gx, gy);

   x1 = SCREEN_W-400;
   al_draw_textf(font, palette_color[9],  x1,    y2, 0, "Zoom:");
   al_draw_textf(font, palette_color[15],  x1+40, y2, 0, "%3.1f", scale_factor);
   x1 +=80;

   al_draw_textf(font, palette_color[9],  x1,   y2, 0, "Text Double:");
   if (saved_display_transform_double == 0) al_draw_textf(font, palette_color[15],  x1+96,   y2, 0, "Auto");
   else                                     al_draw_textf(font, palette_color[15],  x1+96,   y2, 0, "%d", saved_display_transform_double);

}






int cm_draw_filter_buttons(int x1, int x2, int y1, int mode, int have_focus, int moving)
{
   int d = 1;
   if (have_focus) d = 0;
   if (moving) d = 1;

   int fs = 12;   // frame size
   int y = y1+fs; // button y position

   int bts = 12;    // button size

   int tc1 = 15;    // text color 1
   int tc2 = 15;    // text color 2
   int fc1 = 15+64; // frame color 1
   int fc2 = 4;     // frame color 2

   int tl=0; // text_lines
   if (mode == 1) tl = 26*bts-bts/2;
   if (mode == 2) tl = 28*bts-bts;
   if (mode == 3) tl = 30*bts-bts;

   if (mW[3].collapsed) tl = -bts+2;

   int y2 = y1+tl+fs*2-2; // pre calc

   // erase background
   al_draw_filled_rectangle(x1, y1, x2, y2, palette_color[0]);

   // draw frame around filter buttons
   int ci = 16; //color inc
   for (int q=0; q<fs; q++)
      al_draw_rectangle(x1+q, y1+q, x2-q, y2-q, palette_color[12+32+(q*ci)], 1);
   al_draw_text(font, palette_color[15], (x1+x2)/2, y1+2, ALLEGRO_ALIGN_CENTER, "Filters");

   int by1 = y1+2;
   mdw_toggle(x2-10, by1, x2-2, 8, 0,0,0,0, 0,0,0,0, 1,0,0,d, mW[3].collapsed,  "-", "+", tc1, tc2, -1, -1);

   // detect mouse click before toggles, but don't do anything until after the toggles change
   int refresh_selection = 0;
   if ((mode == 3) && (mW[4].copy_mode) && (mouse_b1)) refresh_selection = 1;

   if (!mW[3].collapsed)
   {
      if (mode > 2) // add blocks and flags
      {
         mdw_toggle(x1+fs, y, x2-fs, bts, 1, 0,0,0,0,0,0,0,1,0,1,d, obj_filter[1][1],  "Blocks", "Blocks", tc1, tc2, fc1, fc2);
         mdw_toggle(x1+fs, y, x2-fs, bts, 1, 0,0,0,0,0,0,0,1,0,1,d, obj_filter[1][2],  "Flags",  "Flags",  tc1, tc2, fc1, fc2);
      }
      if (mode > 1) // add lifts
      {
         mdw_toggle(x1+fs, y, x2-fs, bts, 1, 0,0,0,0,0,0,0,1,0,1,d, obj_filter[4][1],  "Lifts",  "Lifts",  tc1, tc2, fc1, fc2);
         y+=bts/2;
      }



      mdw_toggle(x1+fs, y, x2-fs, bts, 0,0,0,0,0,0,0,0,1,0,1,d, obj_filter[3][3],  "Arcwgn", "Arcwgn", tc1, tc2, fc1, fc2);
      mdw_toggle(x1+fs, y, x2-fs, bts, 0,0,0,0,0,0,0,0,1,0,1,d, obj_filter[3][4],  "Bouncr", "Bouncr", tc1, tc2, fc1, fc2);
      mdw_toggle(x1+fs, y, x2-fs, bts, 0,0,0,0,0,0,0,0,1,0,1,d, obj_filter[3][5],  "Jmpwrm", "Jmpwrm", tc1, tc2, fc1, fc2);
      mdw_toggle(x1+fs, y, x2-fs, bts, 0,0,0,0,0,0,0,0,1,0,1,d, obj_filter[3][6],  "Cannon", "Cannon", tc1, tc2, fc1, fc2);
      mdw_toggle(x1+fs, y, x2-fs, bts, 0,0,0,0,0,0,0,0,1,0,1,d, obj_filter[3][7],  "Podzil", "Podzil", tc1, tc2, fc1, fc2);
      mdw_toggle(x1+fs, y, x2-fs, bts, 0,0,0,0,0,0,0,0,1,0,1,d, obj_filter[3][8],  "Trakbt", "Trakbt", tc1, tc2, fc1, fc2);
      mdw_toggle(x1+fs, y, x2-fs, bts, 0,0,0,0,0,0,0,0,1,0,1,d, obj_filter[3][9],  "Cloner", "Cloner", tc1, tc2, fc1, fc2);
      mdw_toggle(x1+fs, y, x2-fs, bts, 0,0,0,0,0,0,0,0,1,0,1,d, obj_filter[3][11], "Blk Wk", "Blk Wk", tc1, tc2, fc1, fc2);
      mdw_toggle(x1+fs, y, x2-fs, bts, 0,0,0,0,0,0,0,0,1,0,1,d, obj_filter[3][12], "Flappr", "Flappr", tc1, tc2, fc1, fc2);
      y+=bts/2;
      mdw_toggle(x1+fs, y, x2-fs, bts, 0,0,0,0,0,0,0,0,1,0,1,d, obj_filter[2][1],  "Door",   "Door",   tc1, tc2, fc1, fc2);
      mdw_toggle(x1+fs, y, x2-fs, bts, 0,0,0,0,0,0,0,0,1,0,1,d, obj_filter[2][2],  "Bonus",  "Bonus",  tc1, tc2, fc1, fc2);
      mdw_toggle(x1+fs, y, x2-fs, bts, 0,0,0,0,0,0,0,0,1,0,1,d, obj_filter[2][3],  "Exit",   "Exit",   tc1, tc2, fc1, fc2);
      mdw_toggle(x1+fs, y, x2-fs, bts, 0,0,0,0,0,0,0,0,1,0,1,d, obj_filter[2][4],  "Key",    "Key",    tc1, tc2, fc1, fc2);
      mdw_toggle(x1+fs, y, x2-fs, bts, 0,0,0,0,0,0,0,0,1,0,1,d, obj_filter[2][5],  "Start",  "Start",  tc1, tc2, fc1, fc2);
      mdw_toggle(x1+fs, y, x2-fs, bts, 0,0,0,0,0,0,0,0,1,0,1,d, obj_filter[2][6],  "Orb",    "Orb",    tc1, tc2, fc1, fc2);
      mdw_toggle(x1+fs, y, x2-fs, bts, 0,0,0,0,0,0,0,0,1,0,1,d, obj_filter[2][7],  "Mine",   "Mine",   tc1, tc2, fc1, fc2);
      mdw_toggle(x1+fs, y, x2-fs, bts, 0,0,0,0,0,0,0,0,1,0,1,d, obj_filter[2][8],  "Bomb",   "Bomb",   tc1, tc2, fc1, fc2);
      mdw_toggle(x1+fs, y, x2-fs, bts, 0,0,0,0,0,0,0,0,1,0,1,d, obj_filter[2][9],  "Triggr", "Triggr", tc1, tc2, fc1, fc2);
      mdw_toggle(x1+fs, y, x2-fs, bts, 0,0,0,0,0,0,0,0,1,0,1,d, obj_filter[2][10], "Messge", "Messge", tc1, tc2, fc1, fc2);
      mdw_toggle(x1+fs, y, x2-fs, bts, 0,0,0,0,0,0,0,0,1,0,1,d, obj_filter[2][11], "Rocket", "Rocket", tc1, tc2, fc1, fc2);
      mdw_toggle(x1+fs, y, x2-fs, bts, 0,0,0,0,0,0,0,0,1,0,1,d, obj_filter[2][12], "Warp",   "Warp",   tc1, tc2, fc1, fc2);
      mdw_toggle(x1+fs, y, x2-fs, bts, 0,0,0,0,0,0,0,0,1,0,1,d, obj_filter[2][14], "Switch", "Switch", tc1, tc2, fc1, fc2);
      mdw_toggle(x1+fs, y, x2-fs, bts, 0,0,0,0,0,0,0,0,1,0,1,d, obj_filter[2][15], "Spring", "Spring", tc1, tc2, fc1, fc2);
      mdw_toggle(x1+fs, y, x2-fs, bts, 0,0,0,0,0,0,0,0,1,0,1,d, obj_filter[2][16], "Blk Mn", "Blk Mn", tc1, tc2, fc1, fc2);
      mdw_toggle(x1+fs, y, x2-fs, bts, 0,0,0,0,0,0,0,0,1,0,1,d, obj_filter[2][17], "Blk Dm", "Blk Dm", tc1, tc2, fc1, fc2);
   }


   if (refresh_selection) zfs_draw_fsel();
   return y2;
}


void cm_draw_status_window(int x1, int x2, int y1, int y2, int have_focus, int moving)
{
   // frame entire window
   al_draw_rectangle(x1, y1, x2, y2, palette_color[9], 1);

   // frame draw item
   al_draw_rectangle(x1, y1 + 11, x1 + 160, y2, palette_color[9], 1);

   // frame view item
   al_draw_rectangle(x1 + 160, y1 + 11, x2, y2, palette_color[9], 1);

   // title bar background color
   al_draw_filled_rectangle(x1, y1, x2, y1 + 11, palette_color[9+192]);

   // frame title bar
   int c = 9;
   if ((have_focus) && (mouse_x > x1) && (mouse_x < x2) && (mouse_y > y1) && (mouse_y < y1+8)) c = 14; // highlight to indicate move is possible
   al_draw_rectangle(x1, y1, x2, y1+11, palette_color[c], 1);


   al_draw_textf(font, palette_color[9],  x1+2,   y1+2, 0, "Status Window   level:%d ",last_level_loaded);
   al_draw_textf(font, palette_color[15], x1+178, y1+2, 0, "%d ",last_level_loaded);

   int mow = is_mouse_on_any_window();
   if (mow)
   {
      al_draw_textf(font, palette_color[15], x1+222, y1+2, 0, "x:-- y:-- ");
      mW[1].point_item_type = -1;
   }
   else
   {
      al_draw_textf(font, palette_color[15], x1+222, y1+2, 0, "x:%-2d y:%-2d ", gx, gy);
      em_find_point_item();
   }

   al_draw_text( font, palette_color[9],  x1+222, y1+2, 0, "x:");
   al_draw_text( font, palette_color[9],  x1+262, y1+2, 0, "y:");

   int d = 1;
   if (have_focus) d = 0;
   if (moving) d = 1;

   int by1 = y1+1;
   if (mdw_buttont(x2-10, by1, x2-2,  9, 0,0,0,0, 0,-1,9,0, 0,0,0,d,"X")) mW[1].active = 0;
   if (mdw_buttont(x2-22, by1, x2-14, 9, 0,0,0,0, 0,-1,9,0, 0,0,0,d,"?")) help("Status Window");


   // draw item area
   al_draw_text(font, palette_color[15], x1 + 24,  y1 + 13, 0, "Draw Item   ");
   al_draw_text(font, palette_color[14], x1 + 100, y1 + 13, 0, "mouse");
   al_draw_text(font, palette_color[14], x1 + 143, y1 + 13, 0, "b1");
   em_show_item_info(                    x1 + 2,   y1 + 20, 9, mW[1].draw_item_type, mW[1].draw_item_num);
   if ((mW[1].draw_item_type == 1) && (mW[1].show_flag_details)) draw_flags(x1+4, y1+47, &mW[1].draw_item_num, &mow, 0, 1, 0); // flags

   // view item area
   al_draw_text(font, palette_color[15], x1 + 184, y1 + 13, 0, "View Item ");
   al_draw_text(font, palette_color[14], x1 + 261, y1 + 13, 0, "mouse");
   al_draw_text(font, palette_color[14], x1 + 303, y1 + 13, 0, "b2");
   em_show_item_info(                    x1 + 162, y1 + 20, 9, mW[1].point_item_type, mW[1].point_item_num);
   if ((mW[1].point_item_type == 1) && (mW[1].show_flag_details)) draw_flags(x1+164, y1+47, &mW[1].point_item_num, &mow, 1, 0, 1); // flags
}


void cm_draw_selection_window(int x1, int x2, int y1, int y2, int have_focus, int moving)
{
   // frame entire window
   al_draw_rectangle(x1, y1, x2, y2, palette_color[9], 1);

   // title bar background color
   al_draw_filled_rectangle(x1, y1, x2, y1 + 11, palette_color[9+192]);

   int d = 1;
   if (have_focus) d = 0;
   if (moving) d = 1;


   int by1 = y1+2;

   if (mdw_buttont(x2-10,  by1, x2-2,   9, 0,0,0,0, 0,-1,9,0, 0,0,0,d, "X"))       mW[2].active = 0;
   if (mdw_buttont(x2-22,  by1, x2-14,  9, 0,0,0,0, 0,-1,9,0, 0,0,0,d, "?"))       help("Selection Window");
   if (mdw_buttont(x2-153, by1, x2-105, 9, 0,0,0,0, 0,-1,9,0, 0,1,0,d, "Blocks"))  mW[2].select_window_block_on = !mW[2].select_window_block_on;
   if (mdw_buttont(x2-90,  by1, x2-34,  9, 0,0,0,0, 0,-1,9,0, 0,1,0,d, "Special")) mW[2].select_window_special_on = !mW[2].select_window_special_on;

   int c = 11;  // first y line of sub-windows;

   // set special start y
   int select_window_special_y = 0;
   if (mW[2].select_window_special_on)
   {
      select_window_special_y = c;
      c = 16 + c + mW[2].select_window_num_special_lines*20;
   }

   // set block start y
   int select_window_block_y = 0;
   if (mW[2].select_window_block_on)
   {
      select_window_block_y = c;
      c = 16 + c + mW[2].swnbl_cur*20;
   }

   // set text start y
   int select_window_text_y = c;

   // set entire window h
   mW[2].h = select_window_text_y;

   // set entire window y2
   mW[2].y2 = y1+mW[2].h;

   int sys = y1 + select_window_special_y;
   int syb = y1 + select_window_block_y;
   int syt = y1 + select_window_text_y;


   int vx = (mouse_x-x1)/20; // column

   if (vx < 0) vx = 0;
   if (vx > 15) vx = 15;


   if (mW[2].select_window_special_on)
   {
      by1 = sys+2;
      al_draw_rectangle(x1, sys, x2, sys+12, palette_color[9], 1);
      al_draw_text(font, palette_color[9], x1+2, sys+2, 0, "Special Items");

      if (mdw_buttont(x2-9, by1, x2-1, 9, 0,0,0,0, 0,-1,9,0, 0,0,0,d,"X")) mW[2].select_window_special_on = 0;

      if (mdw_buttont(x2-41, by1, x2-33, 9, 0,0,0,0, 0,-1,9,0, 0,0,0,d,"+"))
         if (++mW[2].select_window_num_special_lines > 4 ) mW[2].select_window_num_special_lines = 4;

      if (mdw_buttont(x2-25, by1, x2-17, 9, 0,0,0,0, 0,-1,9,0, 0,0,0,d,"-"))
         if (--mW[2].select_window_num_special_lines < 1 )
         {
            mW[2].select_window_num_special_lines++;
            mW[2].select_window_special_on = 0;
         }
      // draw special blocks
      for (c=0; c<16*mW[2].select_window_num_special_lines; c++)
      {
         int tn = PDEi[c][1]; // default is the tile in PDEi[c][1]
         if (tn > 999) tn = zz[0][tn-1000]; // ans
         al_draw_bitmap(tile[tn], x1+(c-((c/16)*16) )*20+1, y1+14+select_window_special_y+1+(c/16*20), 0 );

         if ((PDEi[c][0] == 108) && (PDEi[c][11])) al_draw_bitmap(tile[440], x1+(c-((c/16)*16) )*20+1, y1+14+select_window_special_y+1+(c/16*20), 0); // bomb sticky spikes

      }
   }


   // frame title bar
   int col = 9;
   if ((have_focus) && (mouse_x > x1) && (mouse_x < x2) && (mouse_y > y1) && (mouse_y < y1+8)) col = 14; // highlight to indicate move is possible
   al_draw_rectangle(x1, y1, x2, y1+11, palette_color[col], 1);
   al_draw_textf(font, palette_color[9],  x1+2,   y1+2, 0, "Selection Window");



   if (mW[2].select_window_block_on)
   {
      by1 = syb+2;
      al_draw_rectangle(x1, syb, x2, syb+12, palette_color[9], 1);
      al_draw_text(font, palette_color[9], x1+2, syb+2, 0, "Block Selection");

      if (mdw_buttont(x2-9, by1, x2-1, 9, 0,0,0,0, 0,-1,9,0, 0,0,0,d,"X")) mW[2].select_window_block_on = 0;

      if (mdw_buttont(x2-41, by1, x2-33, 9, 0,0,0,0, 0,-1,9,0, 0,0,0,d,"+"))
         if (++mW[2].swnbl_cur > mW[2].swnbl) mW[2].swnbl_cur = mW[2].swnbl;

      if (mdw_buttont(x2-25, by1, x2-17, 9, 0,0,0,0, 0,-1,9,0, 0,0,0,d,"-"))
         if (--mW[2].swnbl_cur < 1 )
         {
            mW[2].swnbl_cur++;
            mW[2].select_window_block_on = 0;
         }
      // draw blocks
      for (c=0; c<16*mW[2].swnbl_cur; c++)
         al_draw_bitmap(btile[swbl[c][0] & 1023], x1+(c-((c/16)*16) )*20+1, y1+select_window_block_y+1+14+(c/16*20), 0 );
   }
   if ((have_focus) && (!moving))
   {
      // check for mouse on special window
      if ((mW[2].select_window_special_on) && (mouse_y > 15 + sys) && (mouse_y < 16 + sys + mW[2].select_window_num_special_lines * 20))
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
            for (int x=0; x<tl; x++)
               al_draw_text(font, palette_color[15], x1+2, y1 + select_window_text_y+14+(x*8), 0, PDEt[ret][x]);

            if (mouse_b1)
            {
               while (mouse_b1) proc_controllers();     // wait for release
               int pn = PDEi[ret][0];
               if (pn < 200)
               {
                  mW[1].draw_item_type = 5;
                  mW[1].draw_item_num = ret;
               }
               if (pn > 199) // Creator
               {
                  switch (pn)
                  {
                     case 200: create_obj(2, 1, 0);  break; // door
                     case 201: create_obj(2, 5, 0);  break; // start
                     case 202: create_obj(2, 3, 0);  break; // exit
                     case 204: create_obj(3, 7, 0);  break; // pod
                     case 206: create_obj(2, 10,0);  break; // msg
                     case 207: create_obj(3, 9, 0);  break; // cloner
                     case 208: create_lift();        break; // lift
                     case 209: create_door(1);       break; // one way al_fixed exit door
                     case 210: create_door(2);       break; // one way linked exit door
                     case 211: create_door(3);       break; // two way door set
                     case 213: create_obj(2, 9, 0);  break; // trigger
                     case 214: create_obj(2, 16, 0); break; // block manip
                     case 215: create_obj(2, 17, 0); break; // block damage
                     case 216: create_obj(3, 13, 0); break; // vinepod
                  }
               }
            }
         }
      }

      // check for mouse on block window
      if ( (mW[2].select_window_block_on) && (mouse_y > 14 + syb) && (mouse_y < 14 + syb + mW[2].swnbl_cur * 20))
      {
         int vy = (mouse_y-syb-14)/20; // row
         int ret = vy*16+vx;
         int tl = 3; // text lines
         int syt2 = syt+15+(8*tl);
         if (mW[1].show_flag_details) syt2 += 140;
         ret = swbl[ret][0];
         al_draw_filled_rectangle(x1, syt, x2, syt2, palette_color[0]); // erase
         al_draw_rectangle(x1, syt, x2, syt2, palette_color[9], 1);     // frame
         al_draw_rectangle(x1, syt, x2, syt+12, palette_color[9], 1); // title and frame
         al_draw_text(font, palette_color[9], x1+2, syt+2,  0, "Description");

         // draw text for this block
         em_get_text_description_of_block_based_on_flags(ret);
         al_draw_text (font, palette_color[15], x1+2, syt+14, 0, "---------------------");
         al_draw_textf(font, palette_color[15], x1+2, syt+22, 0, "Block %d - %s ", ret&1023, msg);
         al_draw_text (font, palette_color[15], x1+2, syt+30, 0, "---------------------");

         int junk;
         if (mW[1].show_flag_details) draw_flags(x1+4, syt+38, &ret, &junk, 1, 0, 1);

         if ((mouse_b1) || (mouse_b2))
         {
            while (mouse_b1) proc_controllers(); // wait for release
            mW[1].draw_item_type = 1;
            mW[1].draw_item_num = ret;
         }
      }
   }
}

void set_windows(int mode)
{
   al_set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);
   if (!mode)
   {
      mode = 1;

      mW[1].set_pos(100, 100);
      mW[1].set_size(320, 41);
      mW[1].set_title("Status Window");
      mW[1].active = 1;
      mW[1].show_flag_details = 1;
      mW[1].show_non_default_blocks = 1;
      mW[1].index = 1;
      mW[1].layer = 0;

      mW[2].set_pos(100, 300);
      mW[2].set_size(322, 100);
      mW[2].set_title("Selection Window");
      mW[2].active = 1;
      mW[2].index = 2;
      mW[2].layer = 1;

      mW[3].set_pos(500, 100);
      mW[3].set_size(82, 100);
      mW[3].set_title("Filters");
      mW[3].active = 0;
      mW[3].index = 3;
      mW[3].layer = 2;

      mW[4].set_pos(700, 100);
      mW[4].set_size(160, 250);
      mW[4].set_title("zfs");
      mW[4].active = 0;
      mW[4].index = 4;
      mW[4].layer = 3;

      mW[5].set_pos(400, 400);
      mW[5].set_size(160, 269);
      mW[5].set_title("ge list");
      mW[5].active = 0;
      mW[5].index = 5;
      mW[5].layer = 4;

      mW[6].set_pos(600, 60);
      mW[6].set_size(100, 100);
      mW[6].set_title("ge controls");
      mW[6].active = 0;
      mW[6].index = 6;
      mW[6].layer = 5;

      mW[7].set_pos(200, 60);
      mW[7].set_size(300, 300);
      mW[7].set_title("viewer");
      mW[7].active = 0;
      mW[7].index = 7;
      mW[7].layer = 6;

      mW[8].set_pos(0, 0);
      mW[8].set_size(SCREEN_W, BORDER_WIDTH);
      mW[8].set_title("top menu");
      mW[8].active = 1;
      mW[8].moveable = 0;
      mW[8].level_editor_mode = 1;
      mW[8].index = 8;
      mW[8].layer = 7;

      mW[9].set_pos(100, 300);
      mW[9].set_size(320, 328);
      mW[9].set_title("Tile Helper");

      mW[9].th_add_del = 0;
      mW[9].th_match = 0;
      mW[9].th_group = 0;
      mW[9].th_sel = 0;

      mW[9].active = 0;
      mW[9].index = 9;
      mW[9].layer = 8;



   }

   if (mode == 1) // edit menu
   {
      mW[1].active = 1; // status
      mW[2].active = 1; // select
      mW[3].active = 0; // filter
      mW[4].active = 0; // zfs
      mW[5].active = 0; // ge list
      mW[6].active = 0; // ge controls
      mW[7].active = 0; // viewer
      mW[8].level_editor_mode = 1;
      mW[9].active = 0; // tile helper
   }

   if (mode == 2) // zfs
   {
      mW[1].active = 0; // status
      mW[2].active = 0; // select
      mW[3].active = 1; // filter
      mW[3].filter_mode = 3;
      mW[4].active = 1; // zfs
      mW[4].copy_mode = 0;
      mW[4].brf_mode = 0;
      mW[5].active = 0; // ge list
      mW[6].active = 0; // ge controls
      mW[7].active = 0; // viewer
      mW[8].level_editor_mode = 2;
      mW[9].active = 0; // tile helper
   }
   if (mode == 3) // group edit
   {
      mW[1].active = 0; // status
      mW[2].active = 0; // select
      mW[3].active = 1; // filter
      mW[3].filter_mode = 1;
      mW[4].active = 0; // zfs
      mW[5].active = 1; // ge list
      mW[5].show_sel_frame = 1;
      mW[6].active = 1; // ge controls
      mW[7].active = 0; // viewer
      mW[8].level_editor_mode = 3;
      ge_init_data();
      mW[9].active = 0; // tile helper
   }
   if (mode == 4) // object viewer
   {
      mW[1].active = 0; // status
      mW[2].active = 0; // select
      mW[3].active = 1; // filter
      mW[3].filter_mode = 2;
      mW[4].active = 0; // zfs
      mW[5].active = 0; // ge list
      mW[6].active = 0; // ge controls
      mW[7].active = 1; // viewer
      mW[8].level_editor_mode = 4;
      mW[9].active = 0; // tile helper
   }

   if (mode == 9) // tile helper
   {
      mW[1].active = 0; // status
      mW[2].active = 0; // select
      mW[3].active = 0; // filter
      mW[4].active = 0; // zfs
      mW[5].active = 0; // ge list
      mW[6].active = 0; // ge controls
      mW[7].active = 0; // viewer
      mW[8].level_editor_mode = 9;
      mW[9].active = 1;
   }
}


int is_mouse_on_any_window(void)
{
   int mow = 0;
   for (int a=0; a<NUM_MW; a++)
      if ((mW[a].active) && (mW[a].detect_mouse())) mow = 1;

   if (mouse_x < BORDER_WIDTH) mow = 1;
   if (mouse_y < BORDER_WIDTH) mow = 1;

   if (mouse_x > SCREEN_W - BORDER_WIDTH) mow = 1;
   if (mouse_y > SCREEN_H - BORDER_WIDTH) mow = 1;


   return mow;
}


int mw_get_max_layer(void)
{
   int max_layer = 0;
   for (int a=0; a<NUM_MW; a++)
      if ((mW[a].active) && (mW[a].layer > max_layer)) max_layer = mW[a].layer;
   return max_layer;
}


// iterate windows by layers down from max, drawing and detecting mouse as we go
// draw_only is set only when redrawing in the middle of a mouse drag move
int mw_cycle_windows(int draw_only)
{
   for (int a=0; a<NUM_MW; a++) mW[a].check_offscreen();

   int mouse_on_window = 0;
   for (int b = mw_get_max_layer(); b>=0; b--)
      for (int a=0; a<NUM_MW; a++) // draw any window at that layer
         if ((mW[a].active) && (mW[a].layer == b))
         {
            mW[a].draw(draw_only);
            if ((!draw_only) && (mW[a].detect_mouse())) mouse_on_window = a;
         }
  // at this point, all windows have been drawn in order
  // and 'mouse_on_window' is set to the top window that has the mouse on it

   // allows focus to change
   // allows mouse drag move to start
   // allows mouse b2 pop up menu
   if (!draw_only)
   {
      // set all window to not have focus
      for (int a=0; a<NUM_MW; a++)
         if (mW[a].active) mW[a].have_focus = 0;

      // this one does
      if (mouse_on_window) mW[mouse_on_window].set_focus(mouse_on_window);

      // process window that has focus
      for (int a=0; a<NUM_MW; a++)
         if ((mW[a].active) && (mW[a].have_focus)) mW[a].process_mouse();

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

   select_window_block_on = 1;
   select_window_special_on = 1;
   select_window_num_special_lines = 4;

   obt = 0;
   num = 0;
   viewer_lock = 0;


   copy_mode = 0;
   brf_mode = 0;


   collapsed = 0;

   have_focus = 0;
   layer = 0;
   hidden = 0;
   moveable = 1;
   moving = 0;
   resizable = 0;
   filter_mode = 1;
}


void mWindow::check_offscreen(void)
{
   int change = 0;
   if (x1<1) {x1=1; change=1;}
   if (y1<1) {y1=1; change=1;}
   if (x1>SCREEN_W-100) {x1=SCREEN_W-100; change=1;}
   if (y1>SCREEN_H-100) {y1=SCREEN_H-100; change=1;}
   if (change) set_pos(x1, y1);
}

void mWindow::process_mouse(void)
{
   if (!hidden)
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
               cm_redraw_level_editor_background(0);
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
               cm_redraw_level_editor_background(0);
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

            if ((mW[3].filter_mode == 3) && (mW[4].copy_mode))
            {
              zfs_save_selection(0);
              zfs_draw_fsel();
            }
         }
         if (index == 1) // status window
         {
            sprintf(global_string[6][0],"Status Window");
            sprintf(global_string[6][1],"--------------");

            if (mW[1].show_flag_details) sprintf(global_string[6][2],"Hide Block Flags");
            else                   sprintf(global_string[6][2],"Show Block Flags");

            if (mW[1].show_non_default_blocks) sprintf(global_string[6][3],"Hide Non-Default Blocks");
            else                         sprintf(global_string[6][3],"Show Non-Default Blocks");

            sprintf(global_string[6][4],"end");
            switch (pmenu(6, 13))
            {
                case 2: mW[1].show_flag_details =! mW[1].show_flag_details; break;
                case 3: mW[1].show_non_default_blocks =! mW[1].show_non_default_blocks; init_level_background(); break;
            }
         }
         if (index == 5) // ge list
         {
            sprintf(global_string[6][0],"Group Edit Object List");
            sprintf(global_string[6][1],"----------------------");

            if (mW[5].show_sel_frame)
            {
                sprintf(global_string[6][2],"Hide Selection");
                sprintf(global_string[6][3],"Add Filtered Selection To List");
                sprintf(global_string[6][4],"Add Selection To List And Set Filters");
                sprintf(global_string[6][5],"end");

                  switch (pmenu(6, 13))
                  {
                      case 2: mW[5].show_sel_frame = 0; break;
                      case 3: ge_add_selection_to_list(0); break;
                      case 4: ge_add_selection_to_list(1); break;
                  }
            }
            else
            {
               sprintf(global_string[6][2],"Show Selection");
               sprintf(global_string[6][3],"end");
               if (pmenu(6, 13) == 2) mW[5].show_sel_frame = 1;
            }
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

void mWindow::draw(int draw_only)
{
   // erase background
   if ((!hidden) && (index != 8)) al_draw_filled_rectangle(x1, y1, x2, y2, palette_color[0]);

   // default window
   if ((index != 1) && (index != 2) && (index != 3) && (index != 4) && (index != 5) && (index != 6) && (index != 7) && (index != 8) && (index != 9))
   {
      // frame window
      al_draw_rectangle(x1, y1, x2, y2, palette_color[color], 1);

      // frame title bar
      al_draw_rectangle(x1, y1, x2, y1+11, palette_color[color], 1);

      // title
      al_draw_textf(font, palette_color[color], x1+2, y1+2, 0, "title:%s", title);

      al_draw_textf(font, palette_color[color], x1+2, y1+12, 0, "index:%d", index);

      al_draw_textf(font, palette_color[color], x1+2, y1+22, 0, "layer:%d", layer);
      al_draw_textf(font, palette_color[color], x1+2, y1+30, 0, "focus:%d", have_focus);


   al_draw_textf(font, palette_color[color], x1+2, y1+38, 0, "x1:%d x2:%d w:%d", x1, x2, w);
   al_draw_textf(font, palette_color[color], x1+2, y1+46, 0, "y1:%d y2:%d h:%d", y1, y2, h);


      al_draw_rectangle(x1, y1, x2, y2, palette_color[10], 1);



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


   if (index == 1) cm_draw_status_window(x1, x2, y1, y2, have_focus, moving);
   if (index == 2) cm_draw_selection_window(x1, x2, y1, y2, have_focus, moving);
   if (index == 3) // filter window
   {
      int fc = 15;
      if (have_focus) fc = 10;
      al_draw_rectangle(x1, y1, x2, y2, palette_color[fc], 1); // frame entire window
      y2 = 1 + cm_draw_filter_buttons(x1+1, x2-1, y1+1, filter_mode, have_focus, moving);
      h = y2 - y1;
   }
   if (index == 4) // zfs
   {
      titlex("Zoom Full Screen", 15, 13, x1, x2, y1+1);
      int mow = is_mouse_on_any_window();
      zfs_pointer_text(x1+1, x2-1, y1+20, mow);

      int sy2 = zfs_draw_buttons(x1+1, x2-1, y1+110, have_focus, moving);
      set_size(w, sy2-y1-1);

      int fc = 13;
      if (have_focus) fc = 10;
      al_draw_rectangle(x1, y1, x2, y2, palette_color[fc], 1); // frame entire window
   }

   if (index == 9) // tile helper
   {
      titlex("Tile Helper", 15, 13, x1, x2, y1+1);
   //   int mow = is_mouse_on_any_window();

      int fc = 13+64;
      if (have_focus) fc = 13;
      al_draw_rectangle(x1, y1, x2, y2, palette_color[fc], 1); // frame entire window

      th_draw_buttons(x1+1, x2-1, y1+60, have_focus, moving);

   //   al_draw_textf(font, palette_color[color], x1+2, y1-38, 0, "w:%d h:%d", w, h);


   }






   if (index == 5) // ge list
   {
      ge_remove_obj_list_filtered_items();
      int ew, eh;
      ge_show_obj_list(x1, y1, &ew, &eh, have_focus, moving);
      set_size(ew, eh);
   }
   if (index == 6) // ge controls
   {
      int ew, eh, nc = ge_show_controls(x1, y1+20, &ew, &eh, have_focus, moving, hidden, draw_only);
      set_size(ew, eh+20);
      if (!nc) hidden = 1;
      else hidden = 0;
      if (!hidden)
      {
         titlex("Group Edit Controls", 15, 13, x1, x2, y1+1);
         int fc = 13;
         if (have_focus) fc = 10;
         al_draw_rectangle(x1, y1, x2, y2, palette_color[fc], 1); // frame entire window
      }
   }
   if (index == 7) // object viewer
   {
      ovw_get_size();
      ovw_title(x1, x2, y1, y2, legend_line);
      ovw_draw_buttons(x1, y1, x2, y2, have_focus, moving, draw_only);
   }

   if (index == 8) // top menu
   {
      cm_process_menu_bar(have_focus, moving, draw_only);
   }




}
int mWindow::detect_mouse(void)
{
   if ((mouse_x >= x1) && (mouse_x <= x2) && (mouse_y >= y1) && (mouse_y <= y2) && (!hidden)) return 1;
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

