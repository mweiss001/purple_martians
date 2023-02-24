// e_window_common.cpp

#include "pm.h"
#include "e_window_common.h"
#include "mwWindow.h"
#include "mwWindowManager.h"
#include "mwDisplay.h"
#include "e_bitmap_tools.h"
#include "mwFont.h"
#include "mwBitmap.h"
#include "mwLift.h"
#include "mwWidgets.h"
#include "mwVisualLevel.h"
#include "mwPDE.h"
#include "mwColor.h"
#include "mwPMEvent.h"
#include "mwInput.h"
#include "mwEventQueue.h"
#include "mwProgramState.h"
#include "mwLevel.h"
#include "mwEnemy.h"
#include "mwItems.h"
#include "e_edit_selection.h"
#include "e_editor_main.h"
#include "e_glt.h"
#include "e_object_viewer.h"
#include "mwHelp.h"
#include "mwMenu.h"



void cm_process_menu_bar(int d)
{
   char msg[1024];
   al_set_target_backbuffer(display);

   mwWM.mW[8].set_pos(0, 0);
   mwWM.mW[8].set_size(mwD.SCREEN_W, BORDER_WIDTH);

   int x1 = BORDER_WIDTH;
   int y1 = 0;
   int by1 = y1+4;
   int bts = 10;
   if (mdw_buttont(x1, by1, x1+32, bts, 0,0,0,0, 0,-1,15,0, 0,0,0,d, "File"))
   {
      strcpy (mMenu.menu_string[0],"File"); // PD sub menu
      strcpy (mMenu.menu_string[1],"New");
      strcpy (mMenu.menu_string[2],"Load");
      strcpy (mMenu.menu_string[3],"Reload");
      strcpy (mMenu.menu_string[4],"Save");
      strcpy (mMenu.menu_string[5],"Save As");
      strcpy (mMenu.menu_string[6],"Exit");
      strcpy (mMenu.menu_string[7],"end");
      int ret = mMenu.tmenu(1, x1, by1-1);
      if (ret == 1)
      {
         if (al_show_native_message_box(display, "New Level", "Clicking OK will create a new blank level", NULL, NULL, ALLEGRO_MESSAGEBOX_OK_CANCEL) == 1)
         {
            mLevel.zero_level_data();
            mLevel.save_level_prompt();
         }
         mLevel.load_level(mLevel.last_level_loaded, 0, 0);
      }
      if (ret == 2) mLevel.load_level_prompt();
      if (ret == 3) mLevel.load_level(mLevel.last_level_loaded, 0, 0);
      if (ret == 4) mLevel.save_level(mLevel.last_level_loaded);
      if (ret == 5) mLevel.save_level_prompt();
      if (ret == 6) mwWM.active = 0;
      al_set_target_backbuffer(display);
   }
   x1 += 44;
   if (mdw_buttont(x1, by1, x1+32, bts, 0,0,0,0, 0,-1,15,0, 0,0,0,d, "View"))
   {
      strcpy (mMenu.menu_string[0],"View");
      strcpy (mMenu.menu_string[1],"Toggle Fullscreen       F12");
      strcpy (mMenu.menu_string[2],"Zoom Out                 F5");
      strcpy (mMenu.menu_string[3],"Zoom In                  F6");
      strcpy (mMenu.menu_string[4],"Reset Zoom            F5+F6");
      sprintf(mMenu.menu_string[5],"Text Double:Auto");
      sprintf(mMenu.menu_string[6],"Text Double:1");
      sprintf(mMenu.menu_string[7],"Text Double:2");
      sprintf(mMenu.menu_string[8],"Text Double:3");

      if (mwPS.autosave_level_editor_state) sprintf(mMenu.menu_string[9],"Autosave State:ON ");
      else                                  sprintf(mMenu.menu_string[9],"Autosave State:OFF");
      sprintf(mMenu.menu_string[10],"Reset State");

      strcpy (mMenu.menu_string[11],"end");
      int ret = mMenu.tmenu(1, x1, by1-1);
      if (ret == 1) mwD.toggle_fullscreen();
      if (ret == 2) mwD.set_scale_factor(mwD.scale_factor * .90, 0);
      if (ret == 3) mwD.set_scale_factor(mwD.scale_factor * 1.1, 0);
      if (ret == 4) mwD.set_scale_factor(1.0, 0);
      if (ret == 5) mwD.set_saved_display_transform(0);
      if (ret == 6) mwD.set_saved_display_transform(1);
      if (ret == 7) mwD.set_saved_display_transform(2);
      if (ret == 8) mwD.set_saved_display_transform(3);
      if (ret == 9) mwPS.autosave_level_editor_state = ! mwPS.autosave_level_editor_state;
      if (ret == 10) { mwWM.set_windows(0); mwWM.save_mW(); }
   }
   x1 += 44;

   if (mdw_buttont(x1, by1, x1+40, bts, 0,0,0,0, 0,-1,15,0, 0,0,0,d, "Lists"))
   {
      strcpy (mMenu.menu_string[0],"Lists");
      strcpy (mMenu.menu_string[1],"List all Items");
      strcpy (mMenu.menu_string[2],"List all Enemies");
      strcpy (mMenu.menu_string[3],"List all Lifts");
      strcpy (mMenu.menu_string[4],"List all pmsg");
      strcpy (mMenu.menu_string[5],"List all Events");
      strcpy (mMenu.menu_string[6],"Level Check");
      strcpy (mMenu.menu_string[7],"Show Level Data");
      strcpy (mMenu.menu_string[8],"end");
      int ret = mMenu.tmenu(1, x1, by1-1);
      if (ret == 1) mItem.show_all_items();
      if (ret == 2) mEnemy.show_all_enemies();
      if (ret == 3) mLift.show_all_lifts();
      if (ret == 4) mItem.show_all_pmsg();
      if (ret == 5) mwPME.show_all_events();
      if (ret == 6) mLevel.level_check();
      if (ret == 7) mLevel.show_level_data(40, 40, 1);

   }
   x1 += 52;


   if (mdw_buttont(x1, by1, x1+64, bts, 0,0,0,0, 0,-1,15,0, 0,0,0,d, "Advanced"))
   {
      strcpy (mMenu.menu_string[0],"Advanced");
      strcpy (mMenu.menu_string[1],"Predefined Enemy Editor");
      strcpy (mMenu.menu_string[2],"Global Level Thingy!!");
      strcpy (mMenu.menu_string[3],"Level Viewer!");
      strcpy (mMenu.menu_string[4],"Animation Sequence Editor");
      strcpy (mMenu.menu_string[5],"Copy Tiles");
      strcpy (mMenu.menu_string[6],"Default Flag Editor");
      strcpy (mMenu.menu_string[7],"end");
      int ret = mMenu.tmenu(1, x1, by1-1);
      if (ret == 1) mPDE.run();
      if (ret == 2) global_level();
      if (ret == 3) mVisualLevel.level_viewer();
      if (ret == 4) animation_sequence_editor();
      if (ret == 5) copy_tiles();
      if (ret == 6) edit_btile_attributes();
   }
   x1 += 76;

   if (mdw_buttont(x1, by1, x1+32, bts, 0,0,0,0, 0,-1,15,0, 0,0,0,d, "Help"))
   {
      strcpy (mMenu.menu_string[0],"Help");
      strcpy (mMenu.menu_string[1],"Level Editor Basics");
      strcpy (mMenu.menu_string[2],"Credits");
      strcpy (mMenu.menu_string[3],"end");
      int ret = mMenu.tmenu(1, x1, by1-1);
      if (ret == 1) mHelp.help("Level Editor Basics");
      if (ret == 2) mHelp.help("Credits");
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
      strcpy (mMenu.menu_string[0],msg);
      strcpy (mMenu.menu_string[1],"Mode:Main Edit");
      strcpy (mMenu.menu_string[2],"Mode:Edit Selection");
      strcpy (mMenu.menu_string[3],"Mode:Group Edit");
      strcpy (mMenu.menu_string[4],"Mode:Object Viewer");
      strcpy (mMenu.menu_string[5],"Mode:Tile Helper");
      strcpy (mMenu.menu_string[6],"end");
      int ret = mMenu.tmenu(1, x1+4, by1-1);
      if (ret == 1) mwWM.set_windows(1);
      if (ret == 2) mwWM.set_windows(2);
      if (ret == 3) mwWM.set_windows(3);
      if (ret == 4) mwWM.set_windows(4);
      if (ret == 5) mwWM.set_windows(9);
   }

   // status display in the lower right border
   int y2 = mwD.SCREEN_H-BORDER_WIDTH+3;
   x1 = mwD.SCREEN_W-172;
   al_draw_textf(mF.pr8, mC.pc[9],  x1+2,  y2, 0, "Level:");
   al_draw_textf(mF.pr8, mC.pc[15], x1+50, y2, 0, "%d ", mLevel.last_level_loaded);
   x1 += 80;

   al_draw_text( mF.pr8, mC.pc[9],  x1,    y2, 0, "x:");
   al_draw_text( mF.pr8, mC.pc[9],  x1+40, y2, 0, "y:");
   if (mwWM.is_mouse_on_any_window()) al_draw_textf(mF.pr8, mC.pc[15], x1, y2, 0, "  --   -- ");
   else                          al_draw_textf(mF.pr8, mC.pc[15], x1, y2, 0, "  %-2d   %-2d ", mwWM.gx, mwWM.gy);

   x1 = mwD.SCREEN_W-400;
   al_draw_textf(mF.pr8, mC.pc[9],  x1,    y2, 0, "Zoom:");
   al_draw_textf(mF.pr8, mC.pc[15],  x1+40, y2, 0, "%3.1f", mwD.scale_factor);
   x1 +=80;

   al_draw_textf(mF.pr8, mC.pc[9],  x1,   y2, 0, "Text Double:");
   if (mwD.saved_display_transform_double == 0) al_draw_textf(mF.pr8, mC.pc[15],  x1+96,   y2, 0, "Auto");
   else                                         al_draw_textf(mF.pr8, mC.pc[15],  x1+96,   y2, 0, "%d", mwD.saved_display_transform_double);
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
   al_draw_filled_rectangle(x1, y1, x2, y2, mC.pc[0]);

   // draw frame around filter buttons
   int ci = 16; //color inc
   for (int q=0; q<fs; q++)
      al_draw_rectangle(x1+q, y1+q, x2-q, y2-q, mC.pc[12+32+(q*ci)], 1);
   al_draw_text(mF.pr8, mC.pc[15], (x1+x2)/2, y1+2, ALLEGRO_ALIGN_CENTER, "Filters");

   int by1 = y1+2;
   mdw_toggle(x2-10, by1, x2-2, 8, 0,0,0,0, 0,0,0,0, 1,0,0,d, mwWM.mW[3].collapsed,  "-", "+", tc1, tc2, -1, -1);

   // detect mouse click before toggles, but don't do anything until after the toggles change
   int refresh_selection = 0;
   if ((mode == 3) && (mwWM.mW[4].copy_mode) && (mI.mouse_b[1][0])) refresh_selection = 1;

   if (!mwWM.mW[3].collapsed)
   {
      if (mode > 2) // add blocks and flags
      {
         mdw_toggle(x1+fs, y, x2-fs, bts, 1, 0,0,0,0,0,0,0,1,0,1,d, mwWM.obj_filter[1][1],  "Blocks", "Blocks", tc1, tc2, fc1, fc2);
         mdw_toggle(x1+fs, y, x2-fs, bts, 1, 0,0,0,0,0,0,0,1,0,1,d, mwWM.obj_filter[1][2],  "Flags",  "Flags",  tc1, tc2, fc1, fc2);
      }
      if (mode > 1) // add lifts
      {
         mdw_toggle(x1+fs, y, x2-fs, bts, 1, 0,0,0,0,0,0,0,1,0,1,d, mwWM.obj_filter[4][1],  "Lifts",  "Lifts",  tc1, tc2, fc1, fc2);
         y+=bts/2;
      }

      mdw_toggle(x1+fs, y, x2-fs, bts, 0,0,0,0,0,0,0,0,1,0,1,d, mwWM.obj_filter[3][3],  "Arcwgn", "Arcwgn", tc1, tc2, fc1, fc2);
      mdw_toggle(x1+fs, y, x2-fs, bts, 0,0,0,0,0,0,0,0,1,0,1,d, mwWM.obj_filter[3][4],  "Bouncr", "Bouncr", tc1, tc2, fc1, fc2);
      mdw_toggle(x1+fs, y, x2-fs, bts, 0,0,0,0,0,0,0,0,1,0,1,d, mwWM.obj_filter[3][5],  "Jmpwrm", "Jmpwrm", tc1, tc2, fc1, fc2);
      mdw_toggle(x1+fs, y, x2-fs, bts, 0,0,0,0,0,0,0,0,1,0,1,d, mwWM.obj_filter[3][6],  "Cannon", "Cannon", tc1, tc2, fc1, fc2);
      mdw_toggle(x1+fs, y, x2-fs, bts, 0,0,0,0,0,0,0,0,1,0,1,d, mwWM.obj_filter[3][7],  "Podzil", "Podzil", tc1, tc2, fc1, fc2);
      mdw_toggle(x1+fs, y, x2-fs, bts, 0,0,0,0,0,0,0,0,1,0,1,d, mwWM.obj_filter[3][8],  "Trakbt", "Trakbt", tc1, tc2, fc1, fc2);
      mdw_toggle(x1+fs, y, x2-fs, bts, 0,0,0,0,0,0,0,0,1,0,1,d, mwWM.obj_filter[3][9],  "Cloner", "Cloner", tc1, tc2, fc1, fc2);
      mdw_toggle(x1+fs, y, x2-fs, bts, 0,0,0,0,0,0,0,0,1,0,1,d, mwWM.obj_filter[3][11], "Blk Wk", "Blk Wk", tc1, tc2, fc1, fc2);
      mdw_toggle(x1+fs, y, x2-fs, bts, 0,0,0,0,0,0,0,0,1,0,1,d, mwWM.obj_filter[3][12], "Flappr", "Flappr", tc1, tc2, fc1, fc2);
      y+=bts/2;
      mdw_toggle(x1+fs, y, x2-fs, bts, 0,0,0,0,0,0,0,0,1,0,1,d, mwWM.obj_filter[2][1],  "Door",   "Door",   tc1, tc2, fc1, fc2);
      mdw_toggle(x1+fs, y, x2-fs, bts, 0,0,0,0,0,0,0,0,1,0,1,d, mwWM.obj_filter[2][2],  "Bonus",  "Bonus",  tc1, tc2, fc1, fc2);
      mdw_toggle(x1+fs, y, x2-fs, bts, 0,0,0,0,0,0,0,0,1,0,1,d, mwWM.obj_filter[2][3],  "Exit",   "Exit",   tc1, tc2, fc1, fc2);
      mdw_toggle(x1+fs, y, x2-fs, bts, 0,0,0,0,0,0,0,0,1,0,1,d, mwWM.obj_filter[2][4],  "Key",    "Key",    tc1, tc2, fc1, fc2);
      mdw_toggle(x1+fs, y, x2-fs, bts, 0,0,0,0,0,0,0,0,1,0,1,d, mwWM.obj_filter[2][5],  "Start",  "Start",  tc1, tc2, fc1, fc2);
      mdw_toggle(x1+fs, y, x2-fs, bts, 0,0,0,0,0,0,0,0,1,0,1,d, mwWM.obj_filter[2][6],  "Orb",    "Orb",    tc1, tc2, fc1, fc2);
      mdw_toggle(x1+fs, y, x2-fs, bts, 0,0,0,0,0,0,0,0,1,0,1,d, mwWM.obj_filter[2][7],  "Mine",   "Mine",   tc1, tc2, fc1, fc2);
      mdw_toggle(x1+fs, y, x2-fs, bts, 0,0,0,0,0,0,0,0,1,0,1,d, mwWM.obj_filter[2][8],  "Bomb",   "Bomb",   tc1, tc2, fc1, fc2);
      mdw_toggle(x1+fs, y, x2-fs, bts, 0,0,0,0,0,0,0,0,1,0,1,d, mwWM.obj_filter[2][9],  "Triggr", "Triggr", tc1, tc2, fc1, fc2);
      mdw_toggle(x1+fs, y, x2-fs, bts, 0,0,0,0,0,0,0,0,1,0,1,d, mwWM.obj_filter[2][10], "Messge", "Messge", tc1, tc2, fc1, fc2);
      mdw_toggle(x1+fs, y, x2-fs, bts, 0,0,0,0,0,0,0,0,1,0,1,d, mwWM.obj_filter[2][11], "Rocket", "Rocket", tc1, tc2, fc1, fc2);
      mdw_toggle(x1+fs, y, x2-fs, bts, 0,0,0,0,0,0,0,0,1,0,1,d, mwWM.obj_filter[2][12], "Warp",   "Warp",   tc1, tc2, fc1, fc2);
      mdw_toggle(x1+fs, y, x2-fs, bts, 0,0,0,0,0,0,0,0,1,0,1,d, mwWM.obj_filter[2][14], "Switch", "Switch", tc1, tc2, fc1, fc2);
      mdw_toggle(x1+fs, y, x2-fs, bts, 0,0,0,0,0,0,0,0,1,0,1,d, mwWM.obj_filter[2][15], "Spring", "Spring", tc1, tc2, fc1, fc2);
      mdw_toggle(x1+fs, y, x2-fs, bts, 0,0,0,0,0,0,0,0,1,0,1,d, mwWM.obj_filter[2][16], "Blk Mn", "Blk Mn", tc1, tc2, fc1, fc2);
      mdw_toggle(x1+fs, y, x2-fs, bts, 0,0,0,0,0,0,0,0,1,0,1,d, mwWM.obj_filter[2][17], "Blk Dm", "Blk Dm", tc1, tc2, fc1, fc2);
   }
   if (refresh_selection) es_draw_fsel();
   return y2;
}

void cm_draw_status_window(int x1, int x2, int y1, int y2, int d, int have_focus)
{
   // frame entire window
   al_draw_rectangle(x1, y1, x2, y2, mC.pc[9], 1);

   // frame draw item
   al_draw_rectangle(x1, y1 + 11, x1 + 160, y2, mC.pc[9], 1);

   // frame view item
   al_draw_rectangle(x1 + 160, y1 + 11, x2, y2, mC.pc[9], 1);

   // title bar background color
   al_draw_filled_rectangle(x1, y1, x2, y1 + 11, mC.pc[9+192]);

   // frame title bar
   int c = 9;
   if ((have_focus) && (mI.mouse_x > x1) && (mI.mouse_x < x2) && (mI.mouse_y > y1) && (mI.mouse_y < y1+8)) c = 14; // highlight to indicate move is possible
   al_draw_rectangle(x1, y1, x2, y1+11, mC.pc[c], 1);

   al_draw_textf(mF.pr8, mC.pc[9],  x1+2,   y1+2, 0, "Status Window   level:%d ", mLevel.last_level_loaded);
   al_draw_textf(mF.pr8, mC.pc[15], x1+178, y1+2, 0, "%d ", mLevel.last_level_loaded);

   int mow = mwWM.is_mouse_on_any_window();
   if (mow)
   {
      al_draw_textf(mF.pr8, mC.pc[15], x1+222, y1+2, 0, "x:-- y:-- ");
      mwWM.mW[1].point_item_type = -1;
   }
   else
   {
      al_draw_textf(mF.pr8, mC.pc[15], x1+222, y1+2, 0, "x:%-2d y:%-2d ", mwWM.gx, mwWM.gy);
      em_find_point_item();
   }

   al_draw_text( mF.pr8, mC.pc[9],  x1+222, y1+2, 0, "x:");
   al_draw_text( mF.pr8, mC.pc[9],  x1+262, y1+2, 0, "y:");

   int by1 = y1+1;
   if (mdw_buttont(x2-10, by1, x2-2,  9, 0,0,0,0, 0,-1,9,0, 0,0,0,d,"X")) mwWM.mW[1].active = 0;
   if (mdw_buttont(x2-22, by1, x2-14, 9, 0,0,0,0, 0,-1,9,0, 0,0,0,d,"?")) mHelp.help("Status Window");


   // draw item area
   al_draw_text(mF.pr8, mC.pc[15], x1 + 24,  y1 + 13, 0, "Draw Item   ");
   al_draw_text(mF.pr8, mC.pc[14], x1 + 100, y1 + 13, 0, "mouse");
   al_draw_text(mF.pr8, mC.pc[14], x1 + 143, y1 + 13, 0, "b1");
   em_show_item_info(                    x1 + 2,   y1 + 20, 9, mwWM.mW[1].draw_item_type, mwWM.mW[1].draw_item_num);
   if ((mwWM.mW[1].draw_item_type == 1) && (mwWM.mW[1].show_flag_details)) draw_flags(x1+4, y1+47, &mwWM.mW[1].draw_item_num, &mow, 0, 1, 0); // flags

   // view item area
   al_draw_text(mF.pr8, mC.pc[15], x1 + 184, y1 + 13, 0, "View Item ");
   al_draw_text(mF.pr8, mC.pc[14], x1 + 261, y1 + 13, 0, "mouse");
   al_draw_text(mF.pr8, mC.pc[14], x1 + 303, y1 + 13, 0, "b2");
   em_show_item_info(                    x1 + 162, y1 + 20, 9, mwWM.mW[1].point_item_type, mwWM.mW[1].point_item_num);
   if ((mwWM.mW[1].point_item_type == 1) && (mwWM.mW[1].show_flag_details)) draw_flags(x1+164, y1+47, &mwWM.mW[1].point_item_num, &mow, 1, 0, 1); // flags
}

void cm_draw_selection_window(int x1, int x2, int y1, int y2, int d, int have_focus)
{
   char msg[1024];
   // frame entire window
   al_draw_rectangle(x1, y1, x2, y2, mC.pc[9], 1);

   // title bar background color
   al_draw_filled_rectangle(x1, y1, x2, y1 + 11, mC.pc[9+192]);

   int by1 = y1+2;

   if (mdw_buttont(x2-10,  by1, x2-2,   9, 0,0,0,0, 0,-1,9,0, 0,0,0,d, "X"))       mwWM.mW[2].active = 0;
   if (mdw_buttont(x2-22,  by1, x2-14,  9, 0,0,0,0, 0,-1,9,0, 0,0,0,d, "?"))       mHelp.help("Selection Window");
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


   int vx = (mI.mouse_x-x1)/20; // column

   if (vx < 0) vx = 0;
   if (vx > 15) vx = 15;


   if (mwWM.mW[2].select_window_special_on)
   {
      by1 = sys+2;
      al_draw_rectangle(x1, sys, x2, sys+12, mC.pc[9], 1);
      al_draw_text(mF.pr8, mC.pc[9], x1+2, sys+2, 0, "Special Items");

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
         int tn = mPDE.PDEi[c][1]; // default is the mwB.tile in PDEi[c][1]
         if (tn > 999) tn = mwB.zz[0][tn-1000]; // ans
         al_draw_bitmap(mwB.tile[tn], x1+(c-((c/16)*16) )*20+1, y1+14+select_window_special_y+1+(c/16*20), 0 );

         if ((mPDE.PDEi[c][0] == 108) && (mPDE.PDEi[c][11])) al_draw_bitmap(mwB.tile[440], x1+(c-((c/16)*16) )*20+1, y1+14+select_window_special_y+1+(c/16*20), 0); // bomb sticky spikes

      }
   }

   // frame title bar
   int col = 9;
   if ((have_focus) && (mI.mouse_x > x1) && (mI.mouse_x < x2) && (mI.mouse_y > y1) && (mI.mouse_y < y1+8)) col = 14; // highlight to indicate move is possible
   al_draw_rectangle(x1, y1, x2, y1+11, mC.pc[col], 1);
   al_draw_textf(mF.pr8, mC.pc[9],  x1+2,   y1+2, 0, "Selection Window");

   if (mwWM.mW[2].select_window_block_on)
   {
      by1 = syb+2;
      al_draw_rectangle(x1, syb, x2, syb+12, mC.pc[9], 1);
      al_draw_text(mF.pr8, mC.pc[9], x1+2, syb+2, 0, "Block Selection");

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
         al_draw_bitmap(mwB.btile[mwWM.swbl[c][0] & 1023], x1+(c-((c/16)*16) )*20+1, y1+select_window_block_y+1+14+(c/16*20), 0 );
   }
   if (!d)
   {
      // check for mouse on special window
      if ((mwWM.mW[2].select_window_special_on) && (mI.mouse_y > 15 + sys) && (mI.mouse_y < 16 + sys + mwWM.mW[2].select_window_num_special_lines * 20))
      {
         int vy = (mI.mouse_y-sys-15)/20; // row
         int ret = vy*16+vx;
         int tl = 0; // text lines
         if (ret < 100) // dont try to show anything above PDE[99]
         {
            // set  text length (number of lines)
            for (int x=0; x<20; x++)
               if (strncmp(mPDE.PDEt[ret][x],"<end>", 5) == 0) tl = x;
            if (tl<5) tl = 5;

             // remove line endings
            for (int x=0; x<20; x++)
               for (int z=0; z<40; z++)
               {
                  if (mPDE.PDEt[ret][x][z] == 10) mPDE.PDEt[ret][x][z] = 32;
                  if (mPDE.PDEt[ret][x][z] == 13) mPDE.PDEt[ret][x][z] = 32;
               }

            // erase and frame
            al_draw_filled_rectangle(x1, syt, x2, 12+syt+3+(8*tl), mC.pc[0]);
            al_draw_rectangle(x1, syt, x2, 12+syt+3+(8*tl), mC.pc[9], 1);

            // title and frame
            al_draw_rectangle(x1, syt, x2, syt+12, mC.pc[9], 1);
            al_draw_text(mF.pr8, mC.pc[9], x1+2, syt+2, 0, "Description ");

            // draw text for this pde
            for (int x=0; x<tl; x++)
               al_draw_text(mF.pr8, mC.pc[15], x1+2, y1 + select_window_text_y+14+(x*8), 0, mPDE.PDEt[ret][x]);

            if (mI.mouse_b[1][0])
            {
               while (mI.mouse_b[1][0]) mwEQ.proc_event_queue();     // wait for release
               int pn = mPDE.PDEi[ret][0];
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
                     case 208: mLift.create_lift(); break; // lift
                     case 209: mItem.create_door(1);       break; // one way fixed exit door
                     case 210: mItem.create_door(2);       break; // one way linked exit door
                     case 211: mItem.create_door(3);       break; // two way door set
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
      if ( (mwWM.mW[2].select_window_block_on) && (mI.mouse_y > 14 + syb) && (mI.mouse_y < 14 + syb + mwWM.mW[2].swnbl_cur * 20))
      {
         int vy = (mI.mouse_y-syb-14)/20; // row
         int ret = vy*16+vx;
         int tl = 3; // text lines
         int syt2 = syt+15+(8*tl);
         if (mwWM.mW[1].show_flag_details) syt2 += 140;
         ret = mwWM.swbl[ret][0];
         al_draw_filled_rectangle(x1, syt, x2, syt2, mC.pc[0]); // erase
         al_draw_rectangle(x1, syt, x2, syt2, mC.pc[9], 1);     // frame
         al_draw_rectangle(x1, syt, x2, syt+12, mC.pc[9], 1); // title and frame
         al_draw_text(mF.pr8, mC.pc[9], x1+2, syt+2,  0, "Description");

         // draw text for this block
         em_get_text_description_of_block_based_on_flags(ret, msg);
         al_draw_text (mF.pr8, mC.pc[15], x1+2, syt+14, 0, "---------------------");
         al_draw_textf(mF.pr8, mC.pc[15], x1+2, syt+22, 0, "Block %d - %s ", ret&1023, msg);
         al_draw_text (mF.pr8, mC.pc[15], x1+2, syt+30, 0, "---------------------");

         int junk;
         if (mwWM.mW[1].show_flag_details) draw_flags(x1+4, syt+38, &ret, &junk, 1, 0, 1);

         if ((mI.mouse_b[1][0]) || (mI.mouse_b[2][0]))
         {
            while (mI.mouse_b[1][0]) mwEQ.proc_event_queue(); // wait for release
            mwWM.mW[1].draw_item_type = 1;
            mwWM.mW[1].draw_item_num = ret;
         }
      }
   }
}
