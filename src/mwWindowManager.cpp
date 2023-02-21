// mwWindowManager.cpp
#include "pm.h"
#include "mwWindowManager.h"
#include "mwDisplay.h"
#include "mwFont.h"
#include "mwLift.h"
#include "e_tile_helper.h"
#include "mwPDE.h"
#include "mwColor.h"
#include "mwInput.h"
#include "mwEventQueue.h"
#include "mwBitmap.h"
#include "mwProgramState.h"
#include "mwItems.h"
#include "z_enemy.h"
#include "mwLevel.h"
#include "e_edit_selection.h"
#include "e_editor_main.h"
#include "e_fnx.h"
#include "e_group_edit.h"
#include "e_object_viewer.h"
#include "z_screen.h"



mwWindowManager mwWM;

extern ALLEGRO_BITMAP *ft_bmp;  //  file temp paste bmp

void mwWindowManager::initialize(int edit_level)
{
   set_windows(0);

   mW[1].draw_item_type = 1;
   mW[1].draw_item_num  = 0;

   bx1=10;  // global selection window
   by1=10;
   bx2=40;
   by2=30;




   // set all filters on
   for (int i=0; i<5; i++)
      for (int j=0; j<20; j++) obj_filter[i][j] = 1;

   if (edit_level) mLevel.load_level(edit_level, 0, 0); // load passed level
   else mLevel.load_level_prompt();                  // prompt for level


   mPDE.load();
   sort_enemy();
   mItem.sort_item(1);
   em_set_swbl();

   mI.initialize();
}

void mwWindowManager::loop(int edit_level)
{
   mwPS.level_editor_running = 1;
   al_show_mouse_cursor(display);
   initialize(edit_level);
   if (mwPS.autosave_level_editor_state) load_mW();
   active = 1;


   while (active)
   {
      redraw_level_editor_background();
      if (!cycle_windows(0)) process_mouse();
      process_keypress();
   }

   if (mwPS.autosave_level_editor_state) save_mW();
   mwPS.level_editor_running = 0;
   mLevel.resume_allowed = 0;
   al_hide_mouse_cursor(display);
}

void mwWindowManager::get_block_position_on_map(void)
{
   // x, y in 0-99 scale
   // the mouse position past the border width is how far we are into the scaled map
   float mx1 = mI.mouse_x-BORDER_WIDTH;
   float my1 = mI.mouse_y-BORDER_WIDTH;

   // divide that by bs to get how many blocks we are into the map
   float mx2 = mx1 / (mwD.scale_factor_current * 20);
   float my2 = my1 / (mwD.scale_factor_current * 20);
   // get block position of WX
   float mx3 = (float)mwD.WX / 20;
   float my3 = (float)mwD.WY / 20;

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
   mx1 = mI.mouse_x-BORDER_WIDTH;
   my1 = mI.mouse_y-BORDER_WIDTH;

   // scale
   mx2 = mx1 / mwD.scale_factor_current;
   my2 = my1 / mwD.scale_factor_current;

   // get position of WX
   mx3 = (float)mwD.WX;
   my3 = (float)mwD.WY;

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

void mwWindowManager::process_scrolledge(void)
{
   int bw = BORDER_WIDTH;
   int swb = mwD.SCREEN_W-bw;
   int shb = mwD.SCREEN_H-bw;

   if (mI.mouse_y > shb) mwD.WY+=(mI.mouse_y - shb)*2; // scroll down
   if (mI.mouse_x > swb) mwD.WX+=(mI.mouse_x - swb)*2; // scroll right
   if (mI.mouse_x < bw)  mwD.WX-=(bw - mI.mouse_x)*2;  // scroll left
   if (mI.mouse_y < 4)   mwD.WY-=(4  - mI.mouse_y)*7;  // scroll up (is different because of menu)

   // find the size of the source screen from actual screen size and scaler
   int SW = (int)( (float)(mwD.SCREEN_W - bw *2) / mwD.scale_factor_current);
   int SH = (int)( (float)(mwD.SCREEN_H - bw *2) / mwD.scale_factor_current);
   if (SW > 2000) SW = 2000;
   if (SH > 2000) SH = 2000;

   // correct for edges
   if (mwD.WX < 0) mwD.WX = 0;
   if (mwD.WY < 0) mwD.WY = 0;
   if (mwD.WX > (2000 - SW)) mwD.WX = 2000 - SW;
   if (mwD.WY > (2000 - SH)) mwD.WY = 2000 - SH;

   // used by get_new_background to only get what is needed
   mwD.level_display_region_x = mwD.WX;
   mwD.level_display_region_y = mwD.WY;
   mwD.level_display_region_w = SW;
   mwD.level_display_region_h = SH;
}

// this function draws a box at full scale on level buffer
// even if the top left and bottom right corners are switched
// used by se, ge and em
void mwWindowManager::show_level_buffer_block_rect(int x1, int y1, int x2, int y2, int color, const char * text)
{
   if (x1 > x2) swap_int(&x1, &x2);
   if (y1 > y2) swap_int(&y1, &y2);
   int dstx = x1*20;
   if (dstx == 0) dstx = 1;
   int dsty = y1*20;
   if (dsty == 0) dsty = 1;
   al_draw_rectangle(dstx, dsty, (x2*20)+19, (y2*20)+19, mC.pc[14], 1);
   al_draw_text(mF.pr8, mC.pc[color], x1*20+2, y1*20-11,  0, text);
}

// used by se, ge and em
// blocks while mouse b1 is pressed
void mwWindowManager::get_new_box(void)
{
   bx2 = bx1 = gx; // set both corners to initial position
   by2 = by1 = gy;
   while (mI.mouse_b[1][0])
   {
      bx2 = gx;
      by2 = gy;
      redraw_level_editor_background(0);
      show_level_buffer_block_rect(bx1, by1, bx2, by2, 14, "selection");
      get_new_screen_buffer(3, 0, 0);
   }
   if (bx1 > bx2) swap_int(&bx1, &bx2); // swap if wrong order
   if (by1 > by2) swap_int(&by1, &by2);
}

void mwWindowManager::process_mouse(void)
{
   if (level_editor_mode == 1) em_process_mouse();
   if (level_editor_mode == 2) es_process_mouse();
   if (level_editor_mode == 3) ge_process_mouse();
   if (level_editor_mode == 4) ov_process_mouse();
   if (level_editor_mode == 9) th_process_mouse();
}

void mwWindowManager::process_keypress(void)
{
   if (level_editor_mode == 4) ov_process_keypress();

   while (mI.key[ALLEGRO_KEY_ESCAPE][0])
   {
      active = 0;
      mwEQ.proc_event_queue();
   }

   if (active == 0)
   {
      int ret = exit_level_editor_dialog();
      if (ret == 0) mLevel.save_level(mLevel.last_level_loaded); // save and exit
      if (ret == 2) active = 1; // cancel
   }
}

void mwWindowManager::redraw_level_editor_background(int mode)
{
   int old_lem = level_editor_mode;
   level_editor_mode = mode;
   redraw_level_editor_background();
   level_editor_mode = old_lem;
}

int mwWindowManager::redraw_level_editor_background(void)
{
   char msg[1024];
   int drawn = 0;

   mwEQ.proc_event_queue();
   if (mwEQ.program_update)
   {
      mwEQ.program_update = 0;
      drawn = 1;

      al_flip_display();

      int mouse_on_window = is_mouse_on_any_window();
      if ((!mouse_on_window) || (level_editor_mode == 0) || (level_editor_mode == 4)) get_block_position_on_map();

      mC.process_flash_color();
      mwB.update_animation();

      process_scrolledge();
      mwD.proc_scale_factor_change();

      get_new_background(0);
      Lift.draw_lifts();
      mItem.draw_items();
      draw_enemies();


      if (level_editor_mode == 1) // edit menu
      {
         if (!mouse_on_window) em_show_draw_item_cursor();
      }

      if (level_editor_mode == 2) // selection edit
      {
         // show selection
         if (!mW[4].copy_mode) show_level_buffer_block_rect(mwWM.bx1, by1, bx2, by2, 14, "selection");

         // only show if mouse not on window
         if (!mouse_on_window)
         {
            if (mW[4].brf_mode) crosshairs_full(gx*20+10, gy*20+10, 15, 1);
            if (mW[4].copy_mode)
            {
               if (ft_bmp)
               {
                  al_draw_bitmap(ft_bmp, gx*20, gy*20, 0);
                  show_level_buffer_block_rect(gx, gy, gx+mW[4].sw-1, gy+mW[4].sh-1, 10, "paste");
               }
               else mW[4].copy_mode = 0;
            }
         }
      }
      if (level_editor_mode == 3) // ge
      {
         int x=0, y=0;

         // show selection frame
         if (mW[5].show_sel_frame) show_level_buffer_block_rect(bx1, by1, bx2, by2, 14, "selection");
         else if (!mouse_on_window) crosshairs_full(gx*20+10, gy*20+10, 15, 1);

         // mark objects on map that are capable of being added to list
         for (int i=0; i<500; i++)
         {
            int type = (mItem.item[i][0]);
            if ((type) && (obj_filter[2][type]))
            {
               x = mItem.item[i][4];
               y = mItem.item[i][5];
               al_draw_rectangle(x, y, x+20, y+20, mC.pc[13], 1);
            }
         }
         for (int e=0; e<100; e++)
         {
            int type = (Ei[e][0]);
            if ((type) && (obj_filter[3][type]))
            {
               x = Ef[e][0];
               y = Ef[e][1];
               al_draw_rectangle(x, y, x+20, y+20, mC.pc[13], 1);
            }
         }

         // is mouse on obj already in list?
         if (!mouse_on_window)
            for (int i=0; i<NUM_OBJ; i++)
            {
               mwWM.obj_list[i][2] = 0; // turn off highlight by default
               if (mwWM.obj_list[i][0])
               {
                  int typ = mwWM.obj_list[i][0];
                  int num = mwWM.obj_list[i][1];
                  if (typ == 2) // item
                  {
                     x = mItem.item[num][4]/20;
                     y = mItem.item[num][5]/20;
                  }
                  if (typ == 3) // enemy
                  {
                     x = Ef[num][0]/20;
                     y = Ef[num][1]/20;
                  }
                  if ((gx == x) && (gy == y)) mwWM.obj_list[i][2] = 1; // turn on highlight for this list item
               }
            }

         // mark objects on map that have already been added to list
         for (int i=0; i<NUM_OBJ; i++)
         {
            if (mwWM.obj_list[i][0])
            {
               int typ = mwWM.obj_list[i][0];
               int num = mwWM.obj_list[i][1];
               if (typ == 2)
               {
                  x = mItem.item[num][4];
                  y = mItem.item[num][5];
               }
               if (typ == 3)
               {
                  x = Ef[num][0];
                  y = Ef[num][1];
               }
               if (mwWM.obj_list[i][2]) al_draw_rectangle(x-2, y-2, x+20+2, y+20+2, mC.Flash1, 1); // highlight
               else                al_draw_rectangle(x,   y,   x+20,   y+20,   mC.pc[10], 1);
            }
         }
      }

      if (level_editor_mode == 4) // ov
      {
         // if current object is message, show all messages
         if ((mW[7].obt == 2) && (mItem.item[mW[7].num][0] == 10))
         {
            for (int i=0; i<500; i++)
               if (mItem.item[i][0] == 10) mItem.draw_pop_message(i, 0, 0, 0, 0, 0, msg);
         }

         // if mouse on legend line, show highlight
         mW[7].legend_line = 0;
         int y1_legend = mW[7].y2 - 34 + (5-mW[7].num_legend_lines)*8; // legend pos
         int y2_legend = y1_legend + (mW[7].num_legend_lines-1)*8;
         if ((mI.mouse_x > mW[7].x1) && (mI.mouse_x < mW[7].x2) && (mI.mouse_y > y1_legend) && (mI.mouse_y < y2_legend)) // is mouse on legend
            mW[7].legend_line = ((mI.mouse_y - y1_legend) / 8) + 1; // which legend line are we on?

         ov_draw_overlays(mW[7].legend_line);

      }


      if (level_editor_mode == 9) // th
      {
         // show marked blocks
         for (int x=0; x<100; x++)
            for (int y=0; y<100; y++)
            {
               if (thl[x][y])
               {
                  //int col = 10;
                  int c = mC.flash_color+64;
                  int c2 = mC.flash_color2+64;
                  al_draw_rectangle(x*20+0.5, y*20+0.5, x*20+20, y*20+20,   mC.pc[c2], 0);
                  al_draw_line(x*20, y*20, x*20+20, y*20+20,   mC.pc[c], 0);
                  al_draw_line(x*20+20, y*20, x*20, y*20+20,   mC.pc[c], 0);
               }
            }
      }
      if (level_editor_mode) get_new_screen_buffer(3, 0, 0);
   }
   return drawn;
}

void mwWindowManager::set_windows(int mode)
{
   al_set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);

   level_editor_mode = mode;
   if (!level_editor_mode)
   {
      level_editor_mode = 1;

      mW[1].index = 1;
      mW[1].layer = 0;
      mW[1].active = 1;
      mW[1].set_pos(100, 100);
      mW[1].set_size(320, 41);
      mW[1].set_title("Status Window");
      mW[1].show_flag_details = 1;
      mW[1].show_non_default_blocks = 1;

      mW[2].index = 2;
      mW[2].layer = 1;
      mW[2].active = 1;
      mW[2].set_pos(100, 300);
      mW[2].set_size(322, 100);
      mW[2].set_title("Selection Window");

      mW[3].index = 3;
      mW[3].layer = 2;
      mW[3].active = 0;
      mW[3].set_pos(500, 100);
      mW[3].set_size(82, 100);
      mW[3].set_title("Filters");

      mW[4].index = 4;
      mW[4].layer = 3;
      mW[4].active = 0;
      mW[4].set_pos(700, 100);
      mW[4].set_size(160, 250);
      mW[4].set_title("selection edit");

      mW[5].index = 5;
      mW[5].layer = 4;
      mW[5].active = 0;
      mW[5].set_pos(400, 400);
      mW[5].set_size(160, 269);
      mW[5].set_title("ge list");

      mW[6].index = 6;
      mW[6].layer = 5;
      mW[6].active = 0;
      mW[6].set_pos(600, 60);
      mW[6].set_size(100, 100);
      mW[6].set_title("ge controls");

      mW[7].index = 7;
      mW[7].layer = 6;
      mW[7].active = 0;
      mW[7].set_pos(200, 60);
      mW[7].set_size(300, 300);
      mW[7].set_title("viewer");

      mW[8].index = 8;
      mW[8].layer = 7;
      mW[8].active = 1;
      mW[8].set_pos(0, 0);
      mW[8].set_size(mwD.SCREEN_W, BORDER_WIDTH);
      mW[8].set_title("top menu");
      mW[8].moveable = 0;

      mW[9].index = 9;
      mW[9].layer = 8;
      mW[9].active = 0;
      mW[9].set_pos(100, 300);
      mW[9].set_size(320, 328);
      mW[9].set_title("Tile Helper");
      mW[9].th_add_del = 0;
      mW[9].th_match = 0;
      mW[9].th_group = 0;
      mW[9].th_sel = 0;
   }

   if (level_editor_mode == 1) // edit menu
   {
      mW[1].active = 1; // status
      mW[2].active = 1; // select
      mW[3].active = 0; // filter
      mW[4].active = 0; // selection edit
      mW[5].active = 0; // ge list
      mW[6].active = 0; // ge controls
      mW[7].active = 0; // viewer
      mW[9].active = 0; // tile helper
   }

   if (level_editor_mode == 2) // selection edit
   {
      mW[1].active = 0; // status
      mW[2].active = 0; // select
      mW[3].active = 1; // filter
      mW[3].filter_mode = 3;
      mW[4].active = 1; // selection edit
      mW[4].copy_mode = 0;
      mW[4].brf_mode = 0;
      mW[5].active = 0; // ge list
      mW[6].active = 0; // ge controls
      mW[7].active = 0; // viewer
      mW[9].active = 0; // tile helper
   }
   if (level_editor_mode == 3) // group edit
   {
      mW[1].active = 0; // status
      mW[2].active = 0; // select
      mW[3].active = 1; // filter
      mW[3].filter_mode = 1;
      mW[4].active = 0; // selection edit
      mW[5].active = 1; // ge list
      mW[5].show_sel_frame = 1;
      mW[6].active = 1; // ge controls
      mW[7].active = 0; // viewer
      ge_init_data();
      mW[9].active = 0; // tile helper
   }
   if (level_editor_mode == 4) // object viewer
   {
      mW[1].active = 0; // status
      mW[2].active = 0; // select
      mW[3].active = 1; // filter
      mW[3].filter_mode = 2;
      mW[4].active = 0; // selection edit
      mW[5].active = 0; // ge list
      mW[6].active = 0; // ge controls
      mW[7].active = 1; // viewer
      mW[9].active = 0; // tile helper
   }

   if (level_editor_mode == 9) // tile helper
   {
      mW[1].active = 0; // status
      mW[2].active = 0; // select
      mW[3].active = 0; // filter
      mW[4].active = 0; // selection edit
      mW[5].active = 0; // ge list
      mW[6].active = 0; // ge controls
      mW[7].active = 0; // viewer
      mW[9].active = 1;
   }





}

int mwWindowManager::is_mouse_on_any_window(void)
{
   int mow = 0;
   for (int a=0; a<NUM_MW; a++)
      if ((mW[a].active) && (mW[a].detect_mouse())) mow = 1;

   if (mI.mouse_x < BORDER_WIDTH) mow = 1;
   if (mI.mouse_y < BORDER_WIDTH) mow = 1;

   if (mI.mouse_x > mwD.SCREEN_W - BORDER_WIDTH) mow = 1;
   if (mI.mouse_y > mwD.SCREEN_H - BORDER_WIDTH) mow = 1;


   return mow;
}

void mwWindowManager::set_focus(int n)
{
   mwWM.mW[n].have_focus = 1;

   // detect if this window is not top window
   if (mwWM.mW[n].layer !=0)
   {
      // get old layer number of window we are promoting
      int on = mwWM.mW[n].layer;

      // slide everything down one layer (add 1 to all layers)
      for (int a=0; a<NUM_MW; a++)
         if (mwWM.mW[a].active) mwWM.mW[a].layer++;

      // set new top layer
      mwWM.mW[n].layer = 0;

      // remove gaps in layers
      for (int b=on+2; b<NUM_MW+10; b++) // cycle from old promoted layer down
         for (int a=0; a<NUM_MW; a++)    // cycle all windows
            if ((mwWM.mW[a].active) && (mwWM.mW[a].layer == b)) mwWM.mW[a].layer--; // move layer up
   }
}




int mwWindowManager::get_max_layer(void)
{
   int max_layer = 0;
   for (int a=0; a<NUM_MW; a++)
      if ((mW[a].active) && (mW[a].layer > max_layer)) max_layer = mW[a].layer;
   return max_layer;
}





// iterate windows by layers down from max, drawing and detecting mouse as we go
// draw_only is set only when redrawing in the middle of a mouse drag move
int mwWindowManager::cycle_windows(int draw_only)
{
   for (int a=0; a<NUM_MW; a++) mW[a].check_offscreen();

   int mouse_on_window = 0;
   for (int b = get_max_layer(); b>=0; b--)
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
      if (mouse_on_window) set_focus(mouse_on_window);

      // process window that has focus
      for (int a=0; a<NUM_MW; a++)
         if ((mW[a].active) && (mW[a].have_focus)) mW[a].process_mouse();

   }
   return mouse_on_window;
}


void mwWindowManager::save_mW(void)
{
   FILE *fp = fopen("bitmaps/mW.pm", "wb");
   if (fp)
   {
      fwrite(&mwWM, sizeof(mwWM), 1, fp);
      fclose(fp);
   }
   else printf("error saving mW.pm\n");
}

void mwWindowManager::load_mW(void)
{
   FILE *fp = fopen("bitmaps/mW.pm", "rb");
   if (fp)
   {
      fread(&mwWM, sizeof(mwWM), 1, fp);
      fclose(fp);
   }
   else
   {
      printf("error loading mW.pm -- using defaults\n");
   }

   // delete it...

   char sys_cmd[500];
   sprintf(sys_cmd, "del bitmaps\\mW.pm");
   //printf("%s\n",sys_cmd);
   system(sys_cmd);
}


