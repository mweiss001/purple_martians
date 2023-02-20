// mwWindow.cpp
#include "pm.h"
#include "mwWindow.h"
#include "mwWindowManager.h"
#include "mwDisplay.h"
#include "mwFont.h"
#include "e_tile_helper.h"
#include "mwColor.h"
#include "mwInput.h"
//#include "z_menu.h"
#include "mwMenu.h"
#include "e_edit_selection.h"
#include "e_fnx.h"
#include "e_group_edit.h"
#include "e_object_viewer.h"
#include "e_window_common.h"
#include "z_screen.h"



mwWindow::mwWindow()
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

void mwWindow::set_title(const char* st)
{
   sprintf(title, "%s", st);
}

void mwWindow::set_pos(int sx, int sy)
{
   x1 = sx;
   y1 = sy;
   x2 = x1 + w;
   y2 = y1 + h;
}

void mwWindow::set_size(int sw, int sh)
{
   w = sw;
   h = sh;
   x2 = x1 + w;
   y2 = y1 + h;
}

int mwWindow::detect_mouse(void)
{
   if ((mI.mouse_x >= x1) && (mI.mouse_x <= x2) && (mI.mouse_y >= y1) && (mI.mouse_y <= y2) && (!hidden)) return 1;
   else return 0;
}

void mwWindow::check_offscreen(void)
{
   int change = 0;
   if (x1<1) {x1=1; change=1;}
   if (y1<1) {y1=1; change=1;}
   if (x1>mwD.SCREEN_W-100) {x1=mwD.SCREEN_W-100; change=1;}
   if (y1>mwD.SCREEN_H-100) {y1=mwD.SCREEN_H-100; change=1;}
   if (change) set_pos(x1, y1);
}

void mwWindow::process_mouse(void)
{
   if (!hidden)
   {
      if (mI.mouse_b[1][0])
      {
         // moveable and mouse on title bar
         if ((moveable) && (mI.mouse_x > x1) && (mI.mouse_x < x2) && (mI.mouse_y > y1) && (mI.mouse_y < y1+8))
         {
            int mxo = mI.mouse_x - x1; // get offset from mouse position to window x, y
            int myo = mI.mouse_y - y1;
            moving = 1;
            while (mI.mouse_b[1][0])
            {
               set_pos(mI.mouse_x-mxo, mI.mouse_y-myo);
               mwWM.redraw_level_editor_background(0);
               get_new_screen_buffer(3, 0, 0);
               mwWM.cycle_windows(1); // draw only
            }
            moving = 0;
         }
         // resizable and mouse on lower right corner
         if ((resizable) && (mI.mouse_x > x2-10) && (mI.mouse_x < x2) && (mI.mouse_y > y2-10) && (mI.mouse_y < y2))
         {
            int mxo = mI.mouse_x - x2; // get offset from mouse position to window x, y
            int myo = mI.mouse_y - y2;
            moving = 1;
            while (mI.mouse_b[1][0])
            {
               x2 = mI.mouse_x-mxo;
               y2 = mI.mouse_y-myo;
               w = x2 - x1;
               h = y2 - y1;
               mwWM.redraw_level_editor_background(0);
               get_new_screen_buffer(3, 0, 0);
               mwWM.cycle_windows(1); // draw only
            }
         }
         moving = 0;
      }

      // mouse b2 anywhere on window
      if ((mI.mouse_b[2][0]) && (mI.mouse_x > x1) && (mI.mouse_x < x2) && (mI.mouse_y > y1) && (mI.mouse_y < y2))
      {

         if (index == 3) // filter window
         {
            sprintf(mMenu.menu_string[0],"Filters");
            sprintf(mMenu.menu_string[1],"---------------");
            sprintf(mMenu.menu_string[2],"All On");
            sprintf(mMenu.menu_string[3],"All Items On");
            sprintf(mMenu.menu_string[4],"All Enemies On");
            sprintf(mMenu.menu_string[5],"All Off");
            sprintf(mMenu.menu_string[6],"All Items Off");
            sprintf(mMenu.menu_string[7],"All Enemies Off");
            sprintf(mMenu.menu_string[8],"end");
            switch (mMenu.pmenu(6, 13))
            {
                case 2:
                   for (int i=0; i<5; i++)
                      for (int j=0; j<20; j++)
                         mwWM.obj_filter[i][j] = 1;
                break;
                case 3:
                   for (int j=0; j<20; j++)
                      mwWM.obj_filter[2][j] = 1;
                break;
                case 4:
                   for (int j=0; j<20; j++)
                      mwWM.obj_filter[3][j] = 1;
                break;
                case 5:
                for (int i=0; i<5; i++)
                   for (int j=0; j<20; j++)
                      mwWM.obj_filter[i][j] = 0;
                break;
                case 6:
                   for (int j=0; j<20; j++)
                      mwWM.obj_filter[2][j] = 0;
                break;
                case 7:
                   for (int j=0; j<20; j++)
                      mwWM.obj_filter[3][j] = 0;
                break;
            }

            if ((mwWM.mW[3].filter_mode == 3) && (mwWM.mW[4].copy_mode))
            {
              es_save_selection(0);
              es_draw_fsel();
            }
         }
         if (index == 1) // status window
         {
            sprintf(mMenu.menu_string[0],"Status Window");
            sprintf(mMenu.menu_string[1],"--------------");

            if (mwWM.mW[1].show_flag_details) sprintf(mMenu.menu_string[2],"Hide Block Flags");
            else                   sprintf(mMenu.menu_string[2],"Show Block Flags");

            if (mwWM.mW[1].show_non_default_blocks) sprintf(mMenu.menu_string[3],"Hide Non-Default Blocks");
            else                         sprintf(mMenu.menu_string[3],"Show Non-Default Blocks");

            sprintf(mMenu.menu_string[4],"end");
            switch (mMenu.pmenu(6, 13))
            {
                case 2: mwWM.mW[1].show_flag_details =! mwWM.mW[1].show_flag_details; break;
                case 3: mwWM.mW[1].show_non_default_blocks =! mwWM.mW[1].show_non_default_blocks; init_level_background(0); break;
            }
         }
         if (index == 5) // ge list
         {
            sprintf(mMenu.menu_string[0],"Group Edit Object List");
            sprintf(mMenu.menu_string[1],"----------------------");

            if (mwWM.mW[5].show_sel_frame)
            {
                sprintf(mMenu.menu_string[2],"Hide Selection");
                sprintf(mMenu.menu_string[3],"Add Filtered Selection To List");
                sprintf(mMenu.menu_string[4],"Add Selection To List And Set Filters");
                sprintf(mMenu.menu_string[5],"end");

                  switch (mMenu.pmenu(6, 13))
                  {
                      case 2: mwWM.mW[5].show_sel_frame = 0; break;
                      case 3: ge_add_selection_to_list(0); break;
                      case 4: ge_add_selection_to_list(1); break;
                  }
            }
            else
            {
               sprintf(mMenu.menu_string[2],"Show Selection");
               sprintf(mMenu.menu_string[3],"end");
               if (mMenu.pmenu(6, 13) == 2) mwWM.mW[5].show_sel_frame = 1;
            }
         }
      }
   }
}

void mwWindow::draw(int draw_only)
{
   // erase background
   if ((!hidden) && (index != 8)) al_draw_filled_rectangle(x1, y1, x2, y2, mC.pc[0]);

   // default window
   if ((index != 1) && (index != 2) && (index != 3) && (index != 4) && (index != 5) && (index != 6) && (index != 7) && (index != 8) && (index != 9))
   {
      // frame window
      al_draw_rectangle(x1, y1, x2, y2, mC.pc[color], 1);

      // frame title bar
      al_draw_rectangle(x1, y1, x2, y1+11, mC.pc[color], 1);

      // title
      al_draw_textf(mF.pr8, mC.pc[color], x1+2, y1+2, 0, "title:%s", title);

      al_draw_textf(mF.pr8, mC.pc[color], x1+2, y1+12, 0, "index:%d", index);

      al_draw_textf(mF.pr8, mC.pc[color], x1+2, y1+22, 0, "layer:%d", layer);
      al_draw_textf(mF.pr8, mC.pc[color], x1+2, y1+30, 0, "focus:%d", have_focus);

      al_draw_textf(mF.pr8, mC.pc[color], x1+2, y1+38, 0, "x1:%d x2:%d w:%d", x1, x2, w);
      al_draw_textf(mF.pr8, mC.pc[color], x1+2, y1+46, 0, "y1:%d y2:%d h:%d", y1, y2, h);

      al_draw_rectangle(x1, y1, x2, y2, mC.pc[10], 1);

      if (have_focus)
      {
         // frame entire window
         al_draw_rectangle(x1, y1, x2, y2, mC.pc[10], 1);

         // frame title bar
         if (moveable)
         {
            if ((mI.mouse_x > x1) && (mI.mouse_x < x2) && (mI.mouse_y > y1) && (mI.mouse_y < y1+8))
               al_draw_rectangle(x1, y1, x2, y1+11, mC.pc[10], 1);
         }

         // lower right corner
         if (resizable)
         {
            if ((mI.mouse_x > x2-10) && (mI.mouse_x < x2) && (mI.mouse_y > y2-10) && (mI.mouse_y < y2)) // lower right corner for resize
               al_draw_rectangle(x2-10, y2-10, x2, y2, mC.pc[10], 1);
         }
      }
   }


   int d = 1;
   if (have_focus) d = 0;
   if (moving) d = 1;
   if (draw_only) d = 1;

   if (index == 1) cm_draw_status_window(x1, x2, y1, y2, d, have_focus);
   if (index == 2) cm_draw_selection_window(x1, x2, y1, y2, d, have_focus);
   if (index == 3) // filter window
   {
      int fc = 15;
      if (have_focus) fc = 10;
      al_draw_rectangle(x1, y1, x2, y2, mC.pc[fc], 1); // frame entire window
      y2 = 1 + cm_draw_filter_buttons(x1+1, x2-1, y1+1, filter_mode, d);
      h = y2 - y1;
   }
   if (index == 4) // edit selection
   {
      titlex("Edit Selection", 15, 13, x1, x2, y1+1);
      int mow = mwWM.is_mouse_on_any_window();
      es_pointer_text(x1+1, x2-1, y1+20, mow);

      int sy2 = es_draw_buttons(x1+1, x2-1, y1+110, d);
      set_size(w, sy2-y1-1);

      int fc = 13;
      if (have_focus) fc = 10;
      al_draw_rectangle(x1, y1, x2, y2, mC.pc[fc], 1); // frame entire window
   }
   if (index == 5) // ge list
   {
      ge_remove_obj_list_filtered_items();
      int ew, eh;
      ge_show_obj_list(x1, y1, &ew, &eh, d);
      set_size(ew, eh);
   }
   if (index == 6) // ge controls
   {
      int ew, eh, nc = ge_show_controls(x1, y1+20, &ew, &eh, hidden, d);
      set_size(ew, eh+20);
      if (!nc) hidden = 1;
      else hidden = 0;
      if (!hidden)
      {
         titlex("Group Edit Controls", 15, 13, x1, x2, y1+1);
         int fc = 13;
         if (have_focus) fc = 10;
         al_draw_rectangle(x1, y1, x2, y2, mC.pc[fc], 1); // frame entire window
      }
   }
   if (index == 7) // object viewer
   {
      ov_get_size();
      ov_title(x1, x2, y1, y2, legend_line);
      ov_draw_buttons(x1, y1, x2, y2, d);
   }
   if (index == 8) // top menu
   {
      cm_process_menu_bar(d);
   }
   if (index == 9) // tile helper
   {
      titlex("Tile Helper", 15, 13, x1, x2, y1+1);
      int fc = 13+64;
      if (have_focus) fc = 13;
      al_draw_rectangle(x1, y1, x2, y2, mC.pc[fc], 1); // frame entire window
      th_draw_buttons(x1+1, x2-1, y1+60, d);
   }
}
