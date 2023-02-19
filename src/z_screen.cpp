// z_screen.cpp

#include "pm.h"
#include "z_screen.h"
#include "mwWindow.h"
#include "mwWindowManager.h"
#include "z_player.h"
#include "mwLogo.h"
#include "mwBottomMessage.h"
#include "mwDemoMode.h"
#include "mwDisplay.h"
#include "mwFont.h"
#include "mwBitmap.h"
#include "z_lift.h"
#include "mwColor.h"
#include "mwProgramState.h"
#include "z_menu.h"
#include "z_item.h"
#include "z_enemy.h"
#include "z_level.h"
#include "z_fnx.h"
#include "mwShots.h"


void get_new_background(int full)
{
   al_set_target_bitmap(mwB.level_buffer);
   if (full) al_draw_bitmap(mwB.level_background, 0, 0, 0);
   else
   {
      // this only grabs the visible region, in the interests of speed
      int x = mwD.level_display_region_x - 20; if (x < 0) x = 0;
      int y = mwD.level_display_region_y - 20; if (y < 0) y = 0;
      int w = mwD.level_display_region_w + 40; if (x+w > 2000) w = 2000-x;
      int h = mwD.level_display_region_h + 40; if (y+h > 2000) h = 2000-y;
      al_draw_bitmap_region(mwB.level_background, x, y, w, h, x, y, 0);
   }
}

void stimp(void) // transition from menu to game
{
   draw_level2(NULL, 0, 0, 0, 1, 1, 1, 1, 1); // redraw entire level in case only region has been drawn
   al_set_target_backbuffer(display);

   int num_steps = 40;
   float delay = .01;

   // find the size of the source screen from actual screen size and scaler
   int bw = BORDER_WIDTH;
   int SW = (int)( (float)(mwD.SCREEN_W - bw *2) / mwD.scale_factor_current);
   int SH = (int)( (float)(mwD.SCREEN_H - bw *2) / mwD.scale_factor_current);

   if (SW > 2000) SW = 2000;
   if (SH > 2000) SH = 2000;

   // find where to grab the source screen from based on the players position
   int alp = active_local_player;
   int PX = players[alp].x + 10;
   int PY = players[alp].y + 10;

   // this method has a hysteresis rectangle in the middle of the screen where there is no scroll
   int x_size = SW / 18; // larger number is smaller window
   int y_size = SH / 18;
   if (mwD.WX < PX - SW/2 - x_size) mwD.WX = PX - SW/2 - x_size;
   if (mwD.WX > PX - SW/2 + x_size) mwD.WX = PX - SW/2 + x_size;
   if (mwD.WY < PY - SH/2 - y_size) mwD.WY = PY - SH/2 - y_size;
   if (mwD.WY > PY - SH/2 + y_size) mwD.WY = PY - SH/2 + y_size;

   // correct for edges
   if (mwD.WX < 0) mwD.WX = 0;
   if (mwD.WY < 0) mwD.WY = 0;
   if (mwD.WX > (2000 - SW)) mwD.WX = 2000 - SW;
   if (mwD.WY > (2000 - SH)) mwD.WY = 2000 - SH;

   // this is where the player will be when stimp is done and the level starts
   PX = players[alp].x;
   PY = players[alp].y;

   float px_final = (PX-mwD.WX) * mwD.scale_factor_current + bw;
   float py_final = (PY-mwD.WY) * mwD.scale_factor_current + bw;


   // offset if entire level is smaller than screen
   int sbw = mwD.SCREEN_W-bw*2;
   int sbh = mwD.SCREEN_H-bw*2;

   // how big is the entire level after scale factor is applied?
   int sls = (int) ((float)2000 * mwD.scale_factor_current); // sls = scaled level size

   // is the entire level smaller than the screen buffer width?
   if (sls < sbw)
   {
      int a = sbw - sls; // how much smaller?
      px_final += a/2;

   }
   // is the entire level smaller than the screen buffer height?
   if (sls < sbh)
   {
      int a = sbh - sls; // how much smaller?
      py_final += a/2;
   }



   // this is the menu map's position and size
   y_size = mwD.SCREEN_H-160;
   x_size = mwD.SCREEN_W-260;
   int size;
   if (y_size < x_size) size = y_size;
   else size = x_size;
   if (size < 10) size = 10;
   int mx = mwD.SCREEN_W/2-(size/2);
   int my = 140;

   // get the players position on the menu map
   int map_px = mx + PX * size / 2000;
   int map_py = my + PY * size / 2000;


   float sc = (float)size / 2000;
   float scf = mwD.scale_factor_current;
   // do the scale increment as a percent change so that the zoom speed is constant
   // use the compound interest formula to find the percent change per step I need
   float a, per = 1.000;
   do
   {
       per += .0001;
       a = sc * pow(per, num_steps);
   }
   while (a < scf);

   float fmx = (float)map_px;
   float fmxf = px_final;
   float fmxinc = (fmxf - fmx) / num_steps;

   float fmy = (float)map_py;
   float fmyf = py_final;
   float fmyinc = (fmyf - fmy) / num_steps;

   for (int steps = 0; steps<num_steps; steps++)
   {
      al_clear_to_color(al_map_rgb(0,0,0));
      draw_level_centered((int)fmx, (int)fmy, PX, PY,  sc);
      al_flip_display();
      sc *= per;
      fmx += fmxinc;
      fmy += fmyinc;
      al_rest(delay);
   }
}


void stamp(void) // transition from game to menu
{
   draw_level2(NULL, 0, 0, 0, 1, 1, 1, 1, 1); // redraw entire level in case only region has been drawn

   int num_steps = 40;
   float delay = .01;

   // find the size of the source screen from actual screen size and scaler
   int bw = BORDER_WIDTH;
   int SW = (int)( (float)(mwD.SCREEN_W - bw *2) / mwD.scale_factor_current);
   int SH = (int)( (float)(mwD.SCREEN_H - bw *2) / mwD.scale_factor_current);
   if (SW > 2000) SW = 2000;
   if (SH > 2000) SH = 2000;

   // find where to grab the source screen from based on the players position
   int alp = active_local_player;
   int PX = players[alp].x + 10;
   int PY = players[alp].y + 10;

   // this method has a hysteresis rectangle in the middle of the screem where there is no scroll
   int x_size = SW / 18; // larger number is smaller window
   int y_size = SH / 18;

   if (mwD.WX < PX - SW/2 - x_size) mwD.WX = PX - SW/2 - x_size;
   if (mwD.WX > PX - SW/2 + x_size) mwD.WX = PX - SW/2 + x_size;
   if (mwD.WY < PY - SH/2 - y_size) mwD.WY = PY - SH/2 - y_size;
   if (mwD.WY > PY - SH/2 + y_size) mwD.WY = PY - SH/2 + y_size;

   // correct for edges
   if (mwD.WX < 0) mwD.WX = 0;
   if (mwD.WY < 0) mwD.WY = 0;
   if (mwD.WX > (2000 - SW)) mwD.WX = 2000 - SW;
   if (mwD.WY > (2000 - SH)) mwD.WY = 2000 - SH;

   // this is where the player will be when stimp is done and the level starts
   PX = players[alp].x;
   PY = players[alp].y;

   float px_final = (PX-mwD.WX) * mwD.scale_factor_current + bw;
   float py_final = (PY-mwD.WY) * mwD.scale_factor_current + bw;

   // offset if entire level is smaller than screen
   int sbw = mwD.SCREEN_W-bw*2;
   int sbh = mwD.SCREEN_H-bw*2;

   // how big is the entire level after scale factor is applied?
   int sls = (int) ((float)2000 * mwD.scale_factor_current); // sls = scaled level size

   // is the entire level smaller than the screen buffer width?
   if (sls < sbw)
   {
      int a = sbw - sls; // how much smaller?
      px_final += a/2;

   }
   // is the entire level smaller than the screen buffer height?
   if (sls < sbh)
   {
      int a = sbh - sls; // how much smaller?
      py_final += a/2;
   }







   // this is the menu map's position and size
   y_size = mwD.SCREEN_H-160;
   x_size = mwD.SCREEN_W-260;
   int size;
   if (y_size < x_size) size = y_size;
   else size = x_size;
   if (size < 10) size = 10;
   int mx = mwD.SCREEN_W/2-(size/2);
   int my = 140;

   // get the players position on the menu map
   int map_px = mx + PX * size / 2000;
   int map_py = my + PY * size / 2000;

   float sc = mwD.scale_factor_current;
   float scf = (float)size / 2000;

   // do the scale increment as a percent change so that the zoom speed is constant
   // use the compound interest formula to find the percent change per step I need
   float a, per = 1.000;
   do
   {
       per -= .0001;
       a = sc * pow(per, num_steps);
   }
   while (a > scf);

   float fmx = px_final;
   float fmxf = (float)map_px;
   float fmxinc = (fmxf - fmx) / num_steps;

   float fmy = py_final;
   float fmyf = (float)map_py;
   float fmyinc = (fmyf - fmy) / num_steps;

   for (int steps = 0; steps<num_steps; steps++)
   {
      al_clear_to_color(al_map_rgb(0,0,0));
      draw_level_centered((int)fmx, (int)fmy, PX, PY,  sc);
      al_flip_display();
      sc *= per;
      fmx += fmxinc;
      fmy += fmyinc;
      al_rest(delay);
   }
}









// to show in settings
void draw_hyst_rect(void)
{
   int bw = BORDER_WIDTH;
   float x_size = (mwD.SCREEN_W-bw*2) * mwD.viewport_x_div/2; // larger number is smaller window
   float y_size = (mwD.SCREEN_H-bw*2) * mwD.viewport_y_div/2;
   float hx1 = mwD.SCREEN_W/2 - x_size;
   float hx2 = mwD.SCREEN_W/2 + x_size;
   float hy1 = mwD.SCREEN_H/2 - y_size;
   float hy2 = mwD.SCREEN_H/2 + y_size;
   al_draw_rectangle(hx1, hy1, hx2, hy2, mC.pc[10], 0);
}














void get_new_screen_buffer(int type, int x, int y)
{
   int p = active_local_player;
   int c = players[p].color;


   al_set_target_backbuffer(display);
   al_clear_to_color(al_map_rgb(0,0,0));

   // draw frame in local player's color
   for (int x = 0; x < BORDER_WIDTH; x++)
      al_draw_rectangle(x+0.5f, x+0.5f, (mwD.SCREEN_W-1-x)+0.5f, (mwD.SCREEN_H-1-x)+0.5f,  mC.pc[c + (x * 16)], 1);



   // default place and size to draw on screen_buffer
   int bw = BORDER_WIDTH;
   int sbx = bw;
   int sby = bw;
   int sbw = mwD.SCREEN_W-bw*2;
   int sbh = mwD.SCREEN_H-bw*2;

   // how big is the entire level after scale factor is applied?
   int sls = (int) ((float)2000 * mwD.scale_factor_current); // sls = scaled level size


   // is the entire level smaller than the screen buffer width?
   if (sls < sbw)
   {
      int a = sbw - sls; // how much smaller?
      sbw = sls;         // new screen_buffer blit width = sls
      if (!mwPS.level_editor_running) sbx += a/2;        // new screen_buffer blit xpos
   }

   // is the entire level smaller than the screen buffer height?
   if (sls < sbh)
   {
      int a = sbh - sls; // how much smaller?
      sbh = sls;         // new screen_buffer blit height = sls
      if (!mwPS.level_editor_running) sby += a/2;        // new screen_buffer blit ypos
   }

   // find the size of the source screen from actual screen size and scaler
   int SW = (int)( (float)(mwD.SCREEN_W - bw *2) / mwD.scale_factor_current);
   int SH = (int)( (float)(mwD.SCREEN_H - bw *2) / mwD.scale_factor_current);
   if (SW > 2000) SW = 2000;
   if (SH > 2000) SH = 2000;


   int PX = 0;
   int PY = 0;


   // find where to grab the source screen from based on the players position
   if (type == 0)
   {
      PX = players[p].x + 10;
      PY = players[p].y + 10;
   }
   if (type == 1)
   {
      PX = x;
      PY = y;
   }

   int x_size = 0, y_size = 0;

   if (type != 3)
   {
      if (mwD.viewport_mode == 0) // this method always has the player in the middle of the screen
      {
         mwD.WX = PX - SW/2 -10; // set window from PX, PY
         mwD.WY = PY - SH/2 -10;
      }
      else // scroll hysteresis (a rectangle in the middle of the screen where there is no scroll)
      {
         x_size = SW * mwD.viewport_x_div/2;
         y_size = SH * mwD.viewport_y_div/2;

         if (mwD.viewport_mode == 2) // like mode 1 but gradual move
         {
            int look_shift_speed = 4;

            if (players[p].left_right) mwD.WX_shift_speed+=.5;
            else mwD.WX_shift_speed-=.5;

            if (mwD.WX_shift_speed > 2)  mwD.WX_shift_speed = 2;
            if (mwD.WX_shift_speed < -2) mwD.WX_shift_speed = -2;

            mwD.WX+=mwD.WX_shift_speed;

            if (players[p].up) mwD.WY-=look_shift_speed;
            if (players[p].down) mwD.WY+=look_shift_speed;
         }
         if (mwD.WX < PX - SW/2 - x_size) mwD.WX = PX - SW/2 - x_size; // hit right edge
         if (mwD.WX > PX - SW/2 + x_size) mwD.WX = PX - SW/2 + x_size; // hit left edge
         if (mwD.WY < PY - SH/2 - y_size) mwD.WY = PY - SH/2 - y_size; // hit bottom edge
         if (mwD.WY > PY - SH/2 + y_size) mwD.WY = PY - SH/2 + y_size; // hit top edge

      }
   }

   // correct for edges
   int clamp_x0 = 0;
   int clamp_x1 = 0;
   int clamp_y0 = 0;
   int clamp_y1 = 0;
   if (mwD.WX < 0)           { mwD.WX = 0;         clamp_x0 = 1; }
   if (mwD.WY < 0)           { mwD.WY = 0;         clamp_y0 = 1; }
   if (mwD.WX > (2000 - SW)) { mwD.WX = 2000 - SW; clamp_x1 = 1; }
   if (mwD.WY > (2000 - SH)) { mwD.WY = 2000 - SH; clamp_y1 = 1; }

   // used by get_new_background to only get what is needed
   mwD.level_display_region_x = mwD.WX;
   mwD.level_display_region_y = mwD.WY;
   mwD.level_display_region_w = SW;
   mwD.level_display_region_h = SH;

   // this is what all the previous calculations have been building up to:
   al_draw_scaled_bitmap(mwB.level_buffer, mwD.WX, mwD.WY, SW, SH, sbx, sby, sbw, sbh, 0);

  //printf("WX:%d, WY:%d, SW:%d, SH:%d, sbx:%d, sby:%d, sbw:%d, sbh:%d\n", WX, WY, SW, SH, sbx, sby, sbw, sbh);



   if (mwD.viewport_show_hyst)
   {
      float hx1 = mwD.SCREEN_W/2 - x_size * mwD.scale_factor_current;
      float hx2 = mwD.SCREEN_W/2 + x_size * mwD.scale_factor_current;
      float hy1 = mwD.SCREEN_H/2 - y_size * mwD.scale_factor_current;
      float hy2 = mwD.SCREEN_H/2 + y_size * mwD.scale_factor_current;
      if (mwD.viewport_mode == 0) {hx2+=20* mwD.scale_factor_current; hy2+=20* mwD.scale_factor_current;}
      if (clamp_x0) hx1 = 0;
      if (clamp_y0) hy1 = 0;
      if (clamp_x1) hx2 = mwD.SCREEN_W;
      if (clamp_y1) hy2 = mwD.SCREEN_H;
      al_draw_rectangle(hx1, hy1, hx2, hy2, mC.pc[10], 0);
   }

   // in level editor mode, if the level is smaller than the screen edges, draw a thin line to show where it ends...
   if (type == 3)
   {
      sbw = mwD.SCREEN_W-bw*2; // recalc these beacuse they have been modified
      sbh = mwD.SCREEN_H-bw*2;
      int xdraw = 0;
      int ydraw = 0;
      int xl=mwD.SCREEN_W-bw; // default screen edge positions
      int yl=mwD.SCREEN_H-bw;
      if (sls < sbw)
      {
         xl = sbx+sls;
         xdraw = 1;
      }
      if (sls < sbh)
      {
         yl = sby+sls;
         ydraw = 1;
      }
      if (xdraw) al_draw_line(xl, bw, xl, yl, mC.pc[c], 0);
      if (ydraw) al_draw_line(bw, yl, xl, yl, mC.pc[c], 0);
      //al_draw_rectangle(sbx, sby, sbx+sbw, sby+sbh, mC.pc[c], 0);
   }

}

void set_map_var(void)
{
   // determine menu_map_size and position
   int y_size = mwD.SCREEN_H-160;
   int x_size = mwD.SCREEN_W-260;
   if (y_size < x_size) mwL.menu_map_size = y_size;
   else mwL.menu_map_size = x_size;
   if (mwL.menu_map_size < 10) mwL.menu_map_size = 10;
   mwL.menu_map_x = mwD.SCREEN_W/2-(mwL.menu_map_size/2);
   mwL.menu_map_y = 140;

   // splash screen logo position
   mwL.mdw_splash_logo_x = mwD.SCREEN_W/2;
   mwL.mdw_splash_logo_y = mwD.SCREEN_H/2;

   // splash screen logo size
   float min_d = mwD.SCREEN_H;  // find miniumum dimension
   if (mwD.SCREEN_W < mwD.SCREEN_H) min_d = mwD.SCREEN_W;
   mwL.mdw_splash_logo_scale = min_d / 500; // 400 is the exact size, make it bigger for padding

   // map screen logo position and size
   float sp = mwL.menu_map_x - BORDER_WIDTH;    // how much space do I have between the map and the screen edge?
   mwL.mdw_map_logo_scale = sp / 500; // 400 is the exact size, make it bigger for padding
   mwL.mdw_map_logo_x = BORDER_WIDTH + sp/2;
   mwL.mdw_map_logo_y = mwL.menu_map_y + mwL.mdw_map_logo_scale * 200; // align top of logo with top of map

   // this is the link from splash to map
   mwL.mdw_logo_scale_dec = (mwL.mdw_splash_logo_scale - mwL.mdw_map_logo_scale) / 320;
   mwL.mdw_logo_x_dec = (mwL.mdw_splash_logo_x - mwL.mdw_map_logo_x) / 320;
   mwL.mdw_logo_y_dec = (mwL.mdw_splash_logo_y - mwL.mdw_map_logo_y) / 320;

//   printf("slx %f sly %f\n", mdw_splash_logo_x, mdw_splash_logo_y );
//   printf("mlx %d mly %d\n", mdw_map_logo_x, mdw_map_logo_y );
//   printf("xdec %f ydec %f\n", mdw_logo_x_dec, mdw_logo_y_dec );
//   printf("xdec %f ydec %f\n", mdw_logo_x_dec*320, mdw_logo_y_dec*320 );
}


void mark_non_default_block(int x, int y)
{
   int c = l[x][y] & 1023;
   if ((mwB.sa[c][0] & PM_BTILE_MOST_FLAGS) != (l[x][y] & PM_BTILE_MOST_FLAGS))
   {
      al_draw_line(x*20, y*20, x*20+20, y*20+20, mC.pc[10], 1);
      al_draw_line(x*20+20, y*20, x*20, y*20+20, mC.pc[10], 1);
   }
}




void init_level_background2(int s, int e)
{
   al_set_target_bitmap(mwB.level_background);
   al_draw_filled_rectangle(s*20, 0, e*20, 2000, al_map_rgb(0,0,0));
   for (int x=s; x<e; x++)
      for (int y=0; y<100; y++)
      {
         al_draw_bitmap(mwB.btile[l[x][y] & 1023], x*20, y*20, 0);
         if ((mwPS.level_editor_running) && (mwWM.mW[1].show_non_default_blocks)) mark_non_default_block(x, y);
      }
}

void init_level_background(int type) // fill level_background with block tiles
{
   //printf("init_level_background\n");
   if (type == 0)
   {
      al_set_target_bitmap(mwB.level_background);
      al_clear_to_color(al_map_rgb(0,0,0));
      for (int x=0; x<100; x++)
         for (int y=0; y<100; y++)
         {
            al_draw_bitmap(mwB.btile[l[x][y] & 1023], x*20, y*20, 0);
            if ((mwPS.level_editor_running) && (mwWM.mW[1].show_non_default_blocks)) mark_non_default_block(x, y);
         }

   }

   if (type == 1)
   {
      if (mwPS.frame_num % 40 == 0)
      {
         al_set_target_bitmap(mwB.level_background);
         al_clear_to_color(al_map_rgb(0,0,0));
         for (int x=0; x<100; x++)
            for (int y=0; y<100; y++)
            {
               al_draw_bitmap(mwB.btile[l[x][y] & 1023], x*20, y*20, 0);
               if ((mwPS.level_editor_running) && (mwWM.mW[1].show_non_default_blocks)) mark_non_default_block(x, y);
            }
      }
   }
   if (type == 2)
   {
      int sq = mwPS.frame_num % 40;
      if (sq == 0)  init_level_background2(0,  10);
      if (sq == 4)  init_level_background2(10, 20);
      if (sq == 8)  init_level_background2(20, 30);
      if (sq == 12) init_level_background2(30, 40);
      if (sq == 16) init_level_background2(40, 50);
      if (sq == 20) init_level_background2(50, 60);
      if (sq == 24) init_level_background2(60, 70);
      if (sq == 28) init_level_background2(70, 80);
      if (sq == 32) init_level_background2(80, 90);
      if (sq == 36) init_level_background2(90, 100);
   }
}


void draw_level2(ALLEGRO_BITMAP *b, int mx, int my, int ms, int blocks, int items, int enemies, int lifts, int players)
{
   if (blocks) get_new_background(1);
   if (valid_level_loaded)
   {
      if (lifts)   draw_lifts();
      if (items)   draw_items();
      if (enemies) draw_enemies();
      if (resume_allowed)
      {
         if (players) draw_players();
         mwS.draw_eshots();
         mwS.draw_pshots();
      }
   }
   if (b == NULL) al_set_target_backbuffer(display);
   else al_set_target_bitmap(b);
   al_draw_scaled_bitmap(mwB.level_buffer, 0, 0, 2000, 2000, mx, my, ms, ms, 0);
}

void draw_level_centered(int screen_x, int screen_y, int level_x, int level_y, float scale_factor)
{
   // use scale factor to determine scaled size of level
   int size = (int)(scale_factor * 2000);
   int mgx = screen_x - (int) (level_x * scale_factor);  // start x pos on level
   int mgy = screen_y - (int) (level_y * scale_factor);  // start y pos on level
   al_draw_scaled_bitmap(mwB.level_buffer, 0, 0, 2000, 2000, mgx, mgy, size, size, 0);
}

void draw_level(void) // draws the map on the menu screen
{
   int blocks = 0;
   if (valid_level_loaded) blocks = 1;

   draw_level2(NULL, mwL.menu_map_x, mwL.menu_map_y, mwL.menu_map_size, blocks, 1, 1, 1, 1);

   int text_x = mwD.SCREEN_W / 2;
   int text_y = mwL.menu_map_y - 16;
   al_draw_textf(mF.pr8, mC.pc[11], text_x, text_y, ALLEGRO_ALIGN_CENTRE, " Level %d ", start_level );
   text_y += 8;

   if (resume_allowed) al_draw_text(mF.pr8, mC.pc[14], text_x, text_y, ALLEGRO_ALIGN_CENTRE, "  (paused)  ");
   else if (valid_level_loaded) al_draw_text(mF.pr8, mC.pc[9], text_x, text_y, ALLEGRO_ALIGN_CENTRE, "  start level  ");
   else al_draw_text(mF.pr8, mC.pc[10], text_x, text_y, ALLEGRO_ALIGN_CENTRE, "  not found !  ");
}


int get_contrasting_color(int color)
{
   if (color == 15) return 10;
   if (color == 14) return 2;
   if (color == 13) return 8;
//   if (color == 12) return 15;
//   if (color == 11) return 15;
//   if (color == 10) return 15;
//   if (color == 9) return 15;
//   if (color == 8) return 15;
//   if (color == 7) return 15;
//   if (color == 6) return 15;
//   if (color == 5) return 15;
//   if (color == 4) return 15;
//   if (color == 3) return 15;
//   if (color == 2) return 15;
//   if (color == 1) return 15;
//   if (color == 0) return 15;
   return 15;
}

void frame_and_title(int show_players)
{
   int p = active_local_player;
   int color = players[p].color;

   // draw the border
   for (int x = 0; x < BORDER_WIDTH; x++)
      al_draw_rectangle(x+0.5f, x+0.5f, (mwD.SCREEN_W-1-x)+0.5f, (mwD.SCREEN_H-1-x)+0.5f,  mC.pc[color + (x * 16)], 1);

   // draw the title on top on the border
   draw_title(mwD.SCREEN_W/2, 2, 322, 32, color);



   int tc = get_contrasting_color(color);


   // draw the version text centered on the bottom of the border
   al_draw_textf(mF.pr8, mC.pc[tc], mwD.SCREEN_W/2, mwD.SCREEN_H-10, ALLEGRO_ALIGN_CENTRE, "Version %s", mwPS.pm_version_string);

   if (show_players)
   {
      // draw a line of players on each side of menu
      color = players[p].color;               // initial color
      int lim = 12;                           // number of players to draw
      float flsc = 5;                         // initial scale
      float y_pos = 20;                       // initial y position
      float y_step = 20 / lim;                // inc to raise 20
      float x_pos = mwD.SCREEN_W/2 - 180;         // initial x position
      int dist = (int)x_pos - BORDER_WIDTH;   // how much space do I have?
      // how much space will it take based on how many to draw ???
      float x_sp = (dist - 240) / (lim-1);     // spacing to stretch row to screen edge
      for (float a=0; a<lim; a++)
      {
         int y = BORDER_WIDTH + (int)y_pos;  // y position
         y_pos -= y_step;                    // move up
         int x = (int)x_pos;                 // x position
         x_pos -= 11*flsc;                   // spacing based on scale
         x_pos -= x_sp;                      // extra spacing stretch row to edge of screen
         flsc *= .78;                        // reduce scale by 78%
         al_draw_scaled_rotated_bitmap(mwB.player_tile[color][1], 0, 0, x+10, y+10, flsc, flsc, 0, 0);
         if (++color > 15) color = 1;        // cycle through players colors
      }

      color = players[p].color;               // initial color
      flsc = 5;                               // initial scale
      y_pos = 20;                             // initial y position
      y_step = 20 / lim;                      // inc to raise 20
      x_pos = mwD.SCREEN_W/2 + 80;                // initial x position
      dist = (mwD.SCREEN_W - BORDER_WIDTH) - (int)x_pos;   // how much space do I have?
      // how much space will it take based on how many to draw ???
      x_sp = (dist - 360) / (lim-1);          // spacing to stretch row to screen edge
      for (float a=0; a<lim; a++)
      {
         int y = BORDER_WIDTH + (int)y_pos;   // y position
         y_pos -= y_step;                     // move up
         int x = (int)x_pos;                  // x position
         x_pos += 15.1*flsc;                  // spacing based on scale
         x_pos += x_sp;                       // extra spacing stretch row to edge of screen
         flsc *= .78;                         // reduce scale by 78%
         x_sp += .5;                          // hack to make things line up on right hand side of screen
         al_draw_scaled_rotated_bitmap(mwB.player_tile[color][1], 0, 0, x+10, y+10, flsc, flsc, 0, ALLEGRO_FLIP_HORIZONTAL);
         if (++color > 15) color = 1;        // cycle through players colors
      }
   }
}




void rtextout_centre(ALLEGRO_FONT *f, ALLEGRO_BITMAP *dbmp, const char *txt1, int x, int y, int col, float scale, float op)
{
   // draws stretched text
   // used in many places
   int sw = strlen(txt1) * 8;
   ALLEGRO_BITMAP *temp = al_create_bitmap(sw, 8);
   al_set_target_bitmap(temp);
   al_clear_to_color(al_map_rgba(0,0,0,0));

   al_draw_text(f, mC.pc[col], sw/2, 0, ALLEGRO_ALIGN_CENTRE, txt1);

   if (dbmp != NULL) al_set_target_bitmap(dbmp);
   else al_set_target_backbuffer(display);

   al_draw_tinted_scaled_rotated_bitmap(temp, al_map_rgba_f(op, op, op, op), sw/2, 4, x, y, scale, scale, 0, 0);
   al_destroy_bitmap(temp);
}






void draw_title(int x, int y, int w, int h, int color)
{
   sprintf(msg, "%s Martians!", mC.color_name[color]); // overwrite with color name
   int bbx1, bby1, bbw1, bbh1;
   al_get_text_dimensions(mF.sauc, msg, &bbx1, &bby1, &bbw1, &bbh1);
   if ((mwB.text_title_bitmaps_create) || (mwB.text_title_draw_color != color))
   {
      mwB.text_title_bitmaps_create = 0;
      mwB.text_title_draw_color = color;
      al_destroy_bitmap(mwB.text_title);
      mwB.text_title = al_create_bitmap(bbw1,bbh1);

      if(!mwB.text_title)
      {
         sprintf(msg, "Error creating text_title %d %d\n", bbw1,bbh1);
         m_err(msg);
      }
      //else printf("created text_title %d %d\n", bbw1,bbh1);
      al_set_target_bitmap(mwB.text_title);
      al_clear_to_color(al_map_rgba(0,0,0,0));
      al_draw_text(mF.sauc, mC.pc[color], 0-bbx1, 0-bby1, 0, msg);
   }
   al_set_target_backbuffer(display);
   al_draw_scaled_bitmap(mwB.text_title, 0, 0, bbw1, bbh1, x-w/2, y, w, h, 0);
}

void draw_large_text_overlay(int type, int color)
{
   char m1[80] = {0};
   char m2[80] = {0};
   int rebuild = 0;

   if ((type == 1) && (mwB.large_text_overlay_state != 1))
   {
      sprintf(m1, "PURPLE");
      sprintf(m2, "MARTIANS");
      mwB.large_text_overlay_state = 1;
      rebuild = 1;
   }

   if ((type == 2) && (mwB.large_text_overlay_state != 2))
   {
      color = players[active_local_player].color;
      sprintf(m1, "LEVEL");
      sprintf(m2, "DONE");
      mwB.large_text_overlay_state = 2;
      rebuild = 1;
   }

   if ((type == 3) && (mwB.large_text_overlay_state != 3))
   {
      sprintf(m1, "DEMO");
      sprintf(m2, "MODE");
      mwB.large_text_overlay_state = 3;
      rebuild = 1;
   }

   if (rebuild)
   {

      int bbx1, bby1, bbw1, bbh1;
      int bbx2, bby2, bbw2, bbh2;
      al_get_text_dimensions(mF.sauc, m1, &bbx1, &bby1, &bbw1, &bbh1);
      al_get_text_dimensions(mF.sauc, m2, &bbx2, &bby2, &bbw2, &bbh2);
      // get max w and h
      float bbw3 = bbw1; if (bbw2 > bbw1) bbw3 = bbw2;
      float bbh3 = bbh1; if (bbh2 > bbh1) bbh3 = bbh2;

   //   printf("m1:%s  m2:%s \n",m1, m2);
   //   printf("bbx1:%4d bby1:%4d bbw1:%4d bbh1:%4d\n",bbx1, bby1, bbw1, bbh1);
   //   printf("bbx2:%4d bby2:%4d bbw2:%4d bbh2:%4d\n",bbx2, bby2, bbw2, bbh2);


      ALLEGRO_BITMAP *t1 = al_create_bitmap(bbw3, bbh3);
      al_set_target_bitmap(t1);
      al_clear_to_color(al_map_rgba(0,0,0,0));
      al_draw_text(mF.sauc, mC.pc[color], 0-bbx1 + (bbw3-bbw1)/2, 0-bby1, 0, m1);

      ALLEGRO_BITMAP *t2 = al_create_bitmap(bbw3, bbh3);
      al_set_target_bitmap(t2);
      al_clear_to_color(al_map_rgba(0,0,0,0));
      al_draw_text(mF.sauc, mC.pc[color], 0-bbx2 + (bbw3-bbw2)/2, 0-bby2, 0, m2);

      int xs = mwD.SCREEN_W*7/8; // x size
      int ys = mwD.SCREEN_H*3/8; // y size

      int x1 = (mwD.SCREEN_W - xs)/2;
      int x2 = xs;

      int yu1 = mwD.SCREEN_H*1/4 - ys/2;
      int yu2 = ys;

      int yl1 = mwD.SCREEN_H*3/4 - ys/2;
      int yl2 = ys;

      al_destroy_bitmap(mwB.large_text_overlay_bitmap);
      mwB.large_text_overlay_bitmap = al_create_bitmap(mwD.SCREEN_W, mwD.SCREEN_H);

      al_set_target_bitmap(mwB.large_text_overlay_bitmap);
      al_clear_to_color(al_map_rgb(0,0,0));


      al_draw_scaled_bitmap(t1, 0, 0, bbw3, bbh3, x1, yu1, x2, yu2, 0);
      al_draw_scaled_bitmap(t2, 0, 0, bbw3, bbh3, x1, yl1, x2, yl2, 0);

      al_destroy_bitmap(t1);
      al_destroy_bitmap(t2);
   }


   float opa = 1.0;
   if (type == 2) opa = 0.5;
   if (type == 3) opa = mwDM.demo_mode_overlay_opacity;
//   if (type == 3) opa = 0.05;
   ALLEGRO_COLOR fc = al_map_rgba_f(opa, opa, opa, opa);
   al_set_target_backbuffer(display);
   al_draw_tinted_bitmap(mwB.large_text_overlay_bitmap, fc, 0, 0, 0);
}


























void draw_percent_barc(int cx, int y, int width, int height, int percent, int c1, int c2, int fc)
{
   int x = cx - width/2; // get x from center
   al_draw_filled_rectangle(x+0.5f, y+0.5f, x + width+0.5f, y + height+0.5f, mC.pc[c1]); //  all c1 to start
   if (percent > 0)
   {
      int w2 = (int) (width * ((float)percent/100)); // how much green
      al_draw_filled_rectangle(x+0.5f, y+0.5f, x + w2+0.5f, y + height+0.5f, mC.pc[c2]); //  then c2
   }

   if (fc) al_draw_rectangle(x-0.5f, y+0.5f, x+width+0.5f, y+height+0.5f, mC.pc[fc], 1); //  frame

}





void draw_percent_bar(int cx, int y, int width, int height, int percent)
{
   int x = cx - width/2; // get x from center
   al_draw_filled_rectangle(x+0.5f, y+0.5f, x + width+0.5f, y + height+0.5f, mC.pc[10]); //  all red to start
   if (percent > 0)
   {
      int w2 = (int) (width * ((float)percent/100)); // how much green
      al_draw_filled_rectangle(x+0.5f, y+0.5f, x + w2+0.5f, y + height+0.5f, mC.pc[11]); //  green
   }
   al_draw_rectangle(x+0.5f, y+0.5f, x+width+0.5f, y+height+0.5f, mC.pc[15], 1); //  white frame
}



void draw_percent_bar_line(int cx, int y, int width, int height, int rise, int color, int percent )
{
   int x = cx - width/2; // get x from center
   int w2 = (int) (width * ((float)percent/100)); // position
   al_draw_line(x+0.5f+w2, y+0.5f-rise, x+0.5f+w2, y+0.5f+height+rise, mC.pc[color], 1);
}



void draw_percent_bar_range(int cx, int y, int width, int height, int color, int start, int end)
{
   int x = cx - width/2; // get x from center
   int w1 = (int) (width * ((float)start/100));
   int w2 = (int) (width * ((float)end/100));
   al_draw_filled_rectangle(x+w1+0.5f, y+1.5f, x+w2+0.5f, y+height+0.5f, mC.pc[color]);
   al_draw_rectangle(x+0.5f, y+0.5f, x + width+0.5f, y + height+0.5f, mC.pc[15], 1); //  white frame
}
