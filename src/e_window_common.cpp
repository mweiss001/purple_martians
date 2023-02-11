// e_window_common.cpp

#include "pm.h"
#include "mwWindow.h"
#include "mwWindowManager.h"

void cm_process_menu_bar(int d)
{
   al_set_target_backbuffer(display);

   mwWM.mW[8].set_pos(0, 0);
   mwWM.mW[8].set_size(SCREEN_W, BORDER_WIDTH);

   int x1 = BORDER_WIDTH;
   int y1 = 0;
   int by1 = y1+4;
   int bts = 10;
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
      if (ret == 6) mwWM.active = 0;
      al_set_target_backbuffer(display);
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

      if (autosave_level_editor_state) sprintf(global_string[5][9],"Autosave State:ON ");
      else                             sprintf(global_string[5][9],"Autosave State:OFF");
      sprintf(global_string[5][10],"Reset State");

      strcpy (global_string[5][11],"end");
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
      if (ret == 9) autosave_level_editor_state = ! autosave_level_editor_state;
      if (ret == 10) { mwWM.set_windows(0); save_mW(); }
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

   if (mwWM.level_editor_mode == 1) sprintf(msg, "Mode:Main Edit");
   if (mwWM.level_editor_mode == 2) sprintf(msg, "Mode:Edit Selection");
   if (mwWM.level_editor_mode == 3) sprintf(msg, "Mode:Group Edit");
   if (mwWM.level_editor_mode == 4) sprintf(msg, "Mode:Object Viewer");
   if (mwWM.level_editor_mode == 9) sprintf(msg, "Mode:Tile Helper");

   if (mdw_buttont(x1, by1, x1+140, bts, 0,0,0,0, 0,-1,15,0, 0,1,0,d, msg))
   {
      strcpy (global_string[5][0],msg);
      strcpy (global_string[5][1],"Mode:Main Edit");
      strcpy (global_string[5][2],"Mode:Edit Selection");
      strcpy (global_string[5][3],"Mode:Group Edit");
      strcpy (global_string[5][4],"Mode:Object Viewer");
      strcpy (global_string[5][5],"Mode:Tile Helper");
      strcpy (global_string[5][6],"end");
      int ret = tmenu(5, 1, x1+4, by1-1);
      if (ret == 1) mwWM.set_windows(1);
      if (ret == 2) mwWM.set_windows(2);
      if (ret == 3) mwWM.set_windows(3);
      if (ret == 4) mwWM.set_windows(4);
      if (ret == 5) mwWM.set_windows(9);
   }

   // status display in the lower right border
   int y2 = SCREEN_H-BORDER_WIDTH+3;
   x1 = SCREEN_W-172;
   al_draw_textf(font, palette_color[9],  x1+2,  y2, 0, "Level:");
   al_draw_textf(font, palette_color[15], x1+50, y2, 0, "%d ",last_level_loaded);
   x1 += 80;

   al_draw_text( font, palette_color[9],  x1,    y2, 0, "x:");
   al_draw_text( font, palette_color[9],  x1+40, y2, 0, "y:");
   if (mwWM.is_mouse_on_any_window()) al_draw_textf(font, palette_color[15], x1, y2, 0, "  --   -- ");
   else                          al_draw_textf(font, palette_color[15], x1, y2, 0, "  %-2d   %-2d ", gx, gy);

   x1 = SCREEN_W-400;
   al_draw_textf(font, palette_color[9],  x1,    y2, 0, "Zoom:");
   al_draw_textf(font, palette_color[15],  x1+40, y2, 0, "%3.1f", scale_factor);
   x1 +=80;

   al_draw_textf(font, palette_color[9],  x1,   y2, 0, "Text Double:");
   if (saved_display_transform_double == 0) al_draw_textf(font, palette_color[15],  x1+96,   y2, 0, "Auto");
   else                                     al_draw_textf(font, palette_color[15],  x1+96,   y2, 0, "%d", saved_display_transform_double);
}

int cm_draw_filter_buttons(int x1, int x2, int y1, int mode, int d)
{

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

   if (mwWM.mW[3].collapsed) tl = -bts+2;

   int y2 = y1+tl+fs*2-2; // pre calc

   // erase background
   al_draw_filled_rectangle(x1, y1, x2, y2, palette_color[0]);

   // draw frame around filter buttons
   int ci = 16; //color inc
   for (int q=0; q<fs; q++)
      al_draw_rectangle(x1+q, y1+q, x2-q, y2-q, palette_color[12+32+(q*ci)], 1);
   al_draw_text(font, palette_color[15], (x1+x2)/2, y1+2, ALLEGRO_ALIGN_CENTER, "Filters");

   int by1 = y1+2;
   mdw_toggle(x2-10, by1, x2-2, 8, 0,0,0,0, 0,0,0,0, 1,0,0,d, mwWM.mW[3].collapsed,  "-", "+", tc1, tc2, -1, -1);

   // detect mouse click before toggles, but don't do anything until after the toggles change
   int refresh_selection = 0;
   if ((mode == 3) && (mwWM.mW[4].copy_mode) && (mouse_b[1][0])) refresh_selection = 1;

   if (!mwWM.mW[3].collapsed)
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
   if (refresh_selection) es_draw_fsel();
   return y2;
}

void cm_draw_status_window(int x1, int x2, int y1, int y2, int d, int have_focus)
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

   int mow = mwWM.is_mouse_on_any_window();
   if (mow)
   {
      al_draw_textf(font, palette_color[15], x1+222, y1+2, 0, "x:-- y:-- ");
      mwWM.mW[1].point_item_type = -1;
   }
   else
   {
      al_draw_textf(font, palette_color[15], x1+222, y1+2, 0, "x:%-2d y:%-2d ", gx, gy);
      em_find_point_item();
   }

   al_draw_text( font, palette_color[9],  x1+222, y1+2, 0, "x:");
   al_draw_text( font, palette_color[9],  x1+262, y1+2, 0, "y:");

   int by1 = y1+1;
   if (mdw_buttont(x2-10, by1, x2-2,  9, 0,0,0,0, 0,-1,9,0, 0,0,0,d,"X")) mwWM.mW[1].active = 0;
   if (mdw_buttont(x2-22, by1, x2-14, 9, 0,0,0,0, 0,-1,9,0, 0,0,0,d,"?")) help("Status Window");


   // draw item area
   al_draw_text(font, palette_color[15], x1 + 24,  y1 + 13, 0, "Draw Item   ");
   al_draw_text(font, palette_color[14], x1 + 100, y1 + 13, 0, "mouse");
   al_draw_text(font, palette_color[14], x1 + 143, y1 + 13, 0, "b1");
   em_show_item_info(                    x1 + 2,   y1 + 20, 9, mwWM.mW[1].draw_item_type, mwWM.mW[1].draw_item_num);
   if ((mwWM.mW[1].draw_item_type == 1) && (mwWM.mW[1].show_flag_details)) draw_flags(x1+4, y1+47, &mwWM.mW[1].draw_item_num, &mow, 0, 1, 0); // flags

   // view item area
   al_draw_text(font, palette_color[15], x1 + 184, y1 + 13, 0, "View Item ");
   al_draw_text(font, palette_color[14], x1 + 261, y1 + 13, 0, "mouse");
   al_draw_text(font, palette_color[14], x1 + 303, y1 + 13, 0, "b2");
   em_show_item_info(                    x1 + 162, y1 + 20, 9, mwWM.mW[1].point_item_type, mwWM.mW[1].point_item_num);
   if ((mwWM.mW[1].point_item_type == 1) && (mwWM.mW[1].show_flag_details)) draw_flags(x1+164, y1+47, &mwWM.mW[1].point_item_num, &mow, 1, 0, 1); // flags
}

void cm_draw_selection_window(int x1, int x2, int y1, int y2, int d, int have_focus)
{
   // frame entire window
   al_draw_rectangle(x1, y1, x2, y2, palette_color[9], 1);

   // title bar background color
   al_draw_filled_rectangle(x1, y1, x2, y1 + 11, palette_color[9+192]);

   int by1 = y1+2;

   if (mdw_buttont(x2-10,  by1, x2-2,   9, 0,0,0,0, 0,-1,9,0, 0,0,0,d, "X"))       mwWM.mW[2].active = 0;
   if (mdw_buttont(x2-22,  by1, x2-14,  9, 0,0,0,0, 0,-1,9,0, 0,0,0,d, "?"))       help("Selection Window");
   if (mdw_buttont(x2-153, by1, x2-105, 9, 0,0,0,0, 0,-1,9,0, 0,1,0,d, "Blocks"))  mwWM.mW[2].select_window_block_on = !mwWM.mW[2].select_window_block_on;
   if (mdw_buttont(x2-90,  by1, x2-34,  9, 0,0,0,0, 0,-1,9,0, 0,1,0,d, "Special")) mwWM.mW[2].select_window_special_on = !mwWM.mW[2].select_window_special_on;

   int c = 11;  // first y line of sub-windows;

   // set special start y
   int select_window_special_y = 0;
   if (mwWM.mW[2].select_window_special_on)
   {
      select_window_special_y = c;
      c = 16 + c + mwWM.mW[2].select_window_num_special_lines*20;
   }

   // set block start y
   int select_window_block_y = 0;
   if (mwWM.mW[2].select_window_block_on)
   {
      select_window_block_y = c;
      c = 16 + c + mwWM.mW[2].swnbl_cur*20;
   }
   // set text start y
   int select_window_text_y = c;

   // set entire window h
   mwWM.mW[2].h = select_window_text_y;

   // set entire window y2
   mwWM.mW[2].y2 = y1+mwWM.mW[2].h;

   int sys = y1 + select_window_special_y;
   int syb = y1 + select_window_block_y;
   int syt = y1 + select_window_text_y;


   int vx = (mouse_x-x1)/20; // column

   if (vx < 0) vx = 0;
   if (vx > 15) vx = 15;


   if (mwWM.mW[2].select_window_special_on)
   {
      by1 = sys+2;
      al_draw_rectangle(x1, sys, x2, sys+12, palette_color[9], 1);
      al_draw_text(font, palette_color[9], x1+2, sys+2, 0, "Special Items");

      if (mdw_buttont(x2-9, by1, x2-1, 9, 0,0,0,0, 0,-1,9,0, 0,0,0,d,"X")) mwWM.mW[2].select_window_special_on = 0;

      if (mdw_buttont(x2-41, by1, x2-33, 9, 0,0,0,0, 0,-1,9,0, 0,0,0,d,"+"))
         if (++mwWM.mW[2].select_window_num_special_lines > 4 ) mwWM.mW[2].select_window_num_special_lines = 4;

      if (mdw_buttont(x2-25, by1, x2-17, 9, 0,0,0,0, 0,-1,9,0, 0,0,0,d,"-"))
         if (--mwWM.mW[2].select_window_num_special_lines < 1 )
         {
            mwWM.mW[2].select_window_num_special_lines++;
            mwWM.mW[2].select_window_special_on = 0;
         }
      // draw special blocks
      for (c=0; c<16*mwWM.mW[2].select_window_num_special_lines; c++)
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

   if (mwWM.mW[2].select_window_block_on)
   {
      by1 = syb+2;
      al_draw_rectangle(x1, syb, x2, syb+12, palette_color[9], 1);
      al_draw_text(font, palette_color[9], x1+2, syb+2, 0, "Block Selection");

      if (mdw_buttont(x2-9, by1, x2-1, 9, 0,0,0,0, 0,-1,9,0, 0,0,0,d,"X")) mwWM.mW[2].select_window_block_on = 0;

      if (mdw_buttont(x2-41, by1, x2-33, 9, 0,0,0,0, 0,-1,9,0, 0,0,0,d,"+"))
         if (++mwWM.mW[2].swnbl_cur > mwWM.mW[2].swnbl) mwWM.mW[2].swnbl_cur = mwWM.mW[2].swnbl;

      if (mdw_buttont(x2-25, by1, x2-17, 9, 0,0,0,0, 0,-1,9,0, 0,0,0,d,"-"))
         if (--mwWM.mW[2].swnbl_cur < 1 )
         {
            mwWM.mW[2].swnbl_cur++;
            mwWM.mW[2].select_window_block_on = 0;
         }
      // draw blocks
      for (c=0; c<16*mwWM.mW[2].swnbl_cur; c++)
         al_draw_bitmap(btile[swbl[c][0] & 1023], x1+(c-((c/16)*16) )*20+1, y1+select_window_block_y+1+14+(c/16*20), 0 );
   }
   if (!d)
   {
      // check for mouse on special window
      if ((mwWM.mW[2].select_window_special_on) && (mouse_y > 15 + sys) && (mouse_y < 16 + sys + mwWM.mW[2].select_window_num_special_lines * 20))
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

            if (mouse_b[1][0])
            {
               while (mouse_b[1][0]) proc_event_queue();     // wait for release
               int pn = PDEi[ret][0];
               if (pn < 200)
               {
                  mwWM.mW[1].draw_item_type = 5;
                  mwWM.mW[1].draw_item_num = ret;
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
      if ( (mwWM.mW[2].select_window_block_on) && (mouse_y > 14 + syb) && (mouse_y < 14 + syb + mwWM.mW[2].swnbl_cur * 20))
      {
         int vy = (mouse_y-syb-14)/20; // row
         int ret = vy*16+vx;
         int tl = 3; // text lines
         int syt2 = syt+15+(8*tl);
         if (mwWM.mW[1].show_flag_details) syt2 += 140;
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
         if (mwWM.mW[1].show_flag_details) draw_flags(x1+4, syt+38, &ret, &junk, 1, 0, 1);

         if ((mouse_b[1][0]) || (mouse_b[2][0]))
         {
            while (mouse_b[1][0]) proc_event_queue(); // wait for release
            mwWM.mW[1].draw_item_type = 1;
            mwWM.mW[1].draw_item_num = ret;
         }
      }
   }
}
