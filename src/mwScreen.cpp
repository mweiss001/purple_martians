// mwScreen.cpp

#include "pm.h"
#include "mwScreen.h"
#include "mwWindowManager.h"
#include "mwPlayers.h"
#include "mwLogo.h"
#include "mwDemoMode.h"
#include "mwDisplay.h"
#include "mwFont.h"
#include "mwBitmap.h"
#include "mwLift.h"
#include "mwColor.h"
#include "mwLoop.h"
#include "mwItems.h"
#include "mwEnemy.h"
#include "mwLevel.h"
#include "mwShots.h"
#include "mwInput.h"


mwScreen mScreen;


void mwScreen::get_new_background(int full)
{
   al_set_target_bitmap(mBitmap.level_buffer);
   if (full) al_draw_bitmap(mBitmap.level_background, 0, 0, 0);
   else
   {
      // this only grabs the visible region, in the interests of speed
      int x = mDisplay.level_display_region_x - 20; if (x < 0) x = 0;
      int y = mDisplay.level_display_region_y - 20; if (y < 0) y = 0;
      int w = mDisplay.level_display_region_w + 40; if (x+w > 2000) w = 2000-x;
      int h = mDisplay.level_display_region_h + 40; if (y+h > 2000) h = 2000-y;
      al_draw_bitmap_region(mBitmap.level_background, x, y, w, h, x, y, 0);
   }
}

void mwScreen::stimp(void) // transition from menu to game
{
   draw_level2(NULL, 0, 0, 0, 1, 1, 1, 1, 1); // redraw entire level in case only region has been drawn
   al_set_target_backbuffer(mDisplay.display);

   int num_steps = 60;
   float delay = .01;

   // find the size of the source screen from actual screen size and scaler
   int bw = BORDER_WIDTH;
   int SW = (int)( (float)(mDisplay.SCREEN_W - bw *2) / mDisplay.scale_factor_current);
   int SH = (int)( (float)(mDisplay.SCREEN_H - bw *2) / mDisplay.scale_factor_current);

   if (SW > 2000) SW = 2000;
   if (SH > 2000) SH = 2000;

   // find where to grab the source screen from based on the players position
   int alp = mPlayer.active_local_player;
   int PX = mPlayer.syn[alp].x + 10;
   int PY = mPlayer.syn[alp].y + 10;

   // this method has a hysteresis rectangle in the middle of the screen where there is no scroll
   int x_size = SW / 18; // larger number is smaller window
   int y_size = SH / 18;
   if (mDisplay.WX < PX - SW/2 - x_size) mDisplay.WX = PX - SW/2 - x_size;
   if (mDisplay.WX > PX - SW/2 + x_size) mDisplay.WX = PX - SW/2 + x_size;
   if (mDisplay.WY < PY - SH/2 - y_size) mDisplay.WY = PY - SH/2 - y_size;
   if (mDisplay.WY > PY - SH/2 + y_size) mDisplay.WY = PY - SH/2 + y_size;

   // correct for edges
   if (mDisplay.WX < 0) mDisplay.WX = 0;
   if (mDisplay.WY < 0) mDisplay.WY = 0;
   if (mDisplay.WX > (2000 - SW)) mDisplay.WX = 2000 - SW;
   if (mDisplay.WY > (2000 - SH)) mDisplay.WY = 2000 - SH;

   // this is where the player will be when stimp is done and the level starts
   PX = mPlayer.syn[alp].x;
   PY = mPlayer.syn[alp].y;

   float px_final = (PX-mDisplay.WX) * mDisplay.scale_factor_current + bw;
   float py_final = (PY-mDisplay.WY) * mDisplay.scale_factor_current + bw;


   // offset if entire level is smaller than screen
   int sbw = mDisplay.SCREEN_W-bw*2;
   int sbh = mDisplay.SCREEN_H-bw*2;

   // how big is the entire level after scale factor is applied?
   int sls = (int) ((float)2000 * mDisplay.scale_factor_current); // sls = scaled level size

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
   y_size = mDisplay.SCREEN_H-160;
   x_size = mDisplay.SCREEN_W-260;
   int size;
   if (y_size < x_size) size = y_size;
   else size = x_size;
   if (size < 10) size = 10;
   int mx = mDisplay.SCREEN_W/2-(size/2);
   int my = 140;

   // get the players position on the menu map
   int map_px = mx + PX * size / 2000;
   int map_py = my + PY * size / 2000;


   float sc = (float)size / 2000;
   float scf = mDisplay.scale_factor_current;
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


void mwScreen::stamp(void) // transition from game to menu
{
   draw_level2(NULL, 0, 0, 0, 1, 1, 1, 1, 1); // redraw entire level in case only region has been drawn

   int num_steps = 60;
   float delay = .01;

   // find the size of the source screen from actual screen size and scaler
   int bw = BORDER_WIDTH;
   int SW = (int)( (float)(mDisplay.SCREEN_W - bw *2) / mDisplay.scale_factor_current);
   int SH = (int)( (float)(mDisplay.SCREEN_H - bw *2) / mDisplay.scale_factor_current);
   if (SW > 2000) SW = 2000;
   if (SH > 2000) SH = 2000;

   // find where to grab the source screen from based on the players position
   int alp = mPlayer.active_local_player;
   int PX = mPlayer.syn[alp].x + 10;
   int PY = mPlayer.syn[alp].y + 10;

   // this method has a hysteresis rectangle in the middle of the screem where there is no scroll
   int x_size = SW / 18; // larger number is smaller window
   int y_size = SH / 18;

   if (mDisplay.WX < PX - SW/2 - x_size) mDisplay.WX = PX - SW/2 - x_size;
   if (mDisplay.WX > PX - SW/2 + x_size) mDisplay.WX = PX - SW/2 + x_size;
   if (mDisplay.WY < PY - SH/2 - y_size) mDisplay.WY = PY - SH/2 - y_size;
   if (mDisplay.WY > PY - SH/2 + y_size) mDisplay.WY = PY - SH/2 + y_size;

   // correct for edges
   if (mDisplay.WX < 0) mDisplay.WX = 0;
   if (mDisplay.WY < 0) mDisplay.WY = 0;
   if (mDisplay.WX > (2000 - SW)) mDisplay.WX = 2000 - SW;
   if (mDisplay.WY > (2000 - SH)) mDisplay.WY = 2000 - SH;

   // this is where the player will be when stimp is done and the level starts
   PX = mPlayer.syn[alp].x;
   PY = mPlayer.syn[alp].y;

   float px_final = (PX-mDisplay.WX) * mDisplay.scale_factor_current + bw;
   float py_final = (PY-mDisplay.WY) * mDisplay.scale_factor_current + bw;

   // offset if entire level is smaller than screen
   int sbw = mDisplay.SCREEN_W-bw*2;
   int sbh = mDisplay.SCREEN_H-bw*2;

   // how big is the entire level after scale factor is applied?
   int sls = (int) ((float)2000 * mDisplay.scale_factor_current); // sls = scaled level size

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
   y_size = mDisplay.SCREEN_H-160;
   x_size = mDisplay.SCREEN_W-260;
   int size;
   if (y_size < x_size) size = y_size;
   else size = x_size;
   if (size < 10) size = 10;
   int mx = mDisplay.SCREEN_W/2-(size/2);
   int my = 140;

   // get the players position on the menu map
   int map_px = mx + PX * size / 2000;
   int map_py = my + PY * size / 2000;

   float sc = mDisplay.scale_factor_current;
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
void mwScreen::draw_hyst_rect(void)
{
   int bw = BORDER_WIDTH;
   float x_size = (mDisplay.SCREEN_W-bw*2) * mDisplay.viewport_x_div/2; // larger number is smaller window
   float y_size = (mDisplay.SCREEN_H-bw*2) * mDisplay.viewport_y_div/2;
   float hx1 = mDisplay.SCREEN_W/2 - x_size;
   float hx2 = mDisplay.SCREEN_W/2 + x_size;
   float hy1 = mDisplay.SCREEN_H/2 - y_size;
   float hy2 = mDisplay.SCREEN_H/2 + y_size;
   al_draw_rectangle(hx1, hy1, hx2, hy2, mColor.pc[10], 0);
}

void mwScreen::get_new_screen_buffer(int type, int x, int y)
{
   int p = mPlayer.active_local_player;
   int c = mPlayer.syn[p].color;

   al_set_target_backbuffer(mDisplay.display);
   al_clear_to_color(al_map_rgb(0,0,0));

   // draw frame in local player's color
   for (int x = 0; x < BORDER_WIDTH; x++)
      al_draw_rectangle(x+0.5f, x+0.5f, (mDisplay.SCREEN_W-1-x)+0.5f, (mDisplay.SCREEN_H-1-x)+0.5f,  mColor.pc[c + (x * 16)], 1);

   // default place and size to draw on screen_buffer
   int bw = BORDER_WIDTH;
   int sbx = bw;
   int sby = bw;
   int sbw = mDisplay.SCREEN_W-bw*2;
   int sbh = mDisplay.SCREEN_H-bw*2;

   // how big is the entire level after scale factor is applied?
   int sls = (int) ((float)2000 * mDisplay.scale_factor_current); // sls = scaled level size


   // is the entire level smaller than the screen buffer width?
   if (sls < sbw)
   {
      int a = sbw - sls; // how much smaller?
      sbw = sls;         // new screen_buffer blit width = sls
      if (!mLoop.level_editor_running) sbx += a/2;        // new screen_buffer blit xpos
   }

   // is the entire level smaller than the screen buffer height?
   if (sls < sbh)
   {
      int a = sbh - sls; // how much smaller?
      sbh = sls;         // new screen_buffer blit height = sls
      if (!mLoop.level_editor_running) sby += a/2;        // new screen_buffer blit ypos
   }

   // find the size of the source screen from actual screen size and scaler
   int SW = (int)( (float)(mDisplay.SCREEN_W - bw *2) / mDisplay.scale_factor_current);
   int SH = (int)( (float)(mDisplay.SCREEN_H - bw *2) / mDisplay.scale_factor_current);
   if (SW > 2000) SW = 2000;
   if (SH > 2000) SH = 2000;


   int PX = 0;
   int PY = 0;


   // find where to grab the source screen from based on the players position
   if (type == 0)
   {
      PX = mPlayer.syn[p].x + 10;
      PY = mPlayer.syn[p].y + 10;
   }
   if (type == 1)
   {
      PX = x;
      PY = y;
   }

   int x_size = 0, y_size = 0;

   if (type != 3)
   {
      if (mDisplay.viewport_mode == 0) // this method always has the player in the middle of the screen
      {
         mDisplay.WX = PX - SW/2 -10; // set window from PX, PY
         mDisplay.WY = PY - SH/2 -10;
      }
      else // scroll hysteresis (a rectangle in the middle of the screen where there is no scroll)
      {
         x_size = SW * mDisplay.viewport_x_div/2;
         y_size = SH * mDisplay.viewport_y_div/2;

         if (mDisplay.viewport_mode == 2) // like mode 1 but gradual move
         {
            int look_shift_speed = 4;

            if (mPlayer.syn[p].left_right) mDisplay.WX_shift_speed+=.5;
            else mDisplay.WX_shift_speed-=.5;

            if (mDisplay.WX_shift_speed > 2)  mDisplay.WX_shift_speed = 2;
            if (mDisplay.WX_shift_speed < -2) mDisplay.WX_shift_speed = -2;

            mDisplay.WX+=mDisplay.WX_shift_speed;

            if (mPlayer.syn[p].up) mDisplay.WY-=look_shift_speed;
            if (mPlayer.syn[p].down) mDisplay.WY+=look_shift_speed;
         }
         if (mDisplay.WX < PX - SW/2 - x_size) mDisplay.WX = PX - SW/2 - x_size; // hit right edge
         if (mDisplay.WX > PX - SW/2 + x_size) mDisplay.WX = PX - SW/2 + x_size; // hit left edge
         if (mDisplay.WY < PY - SH/2 - y_size) mDisplay.WY = PY - SH/2 - y_size; // hit bottom edge
         if (mDisplay.WY > PY - SH/2 + y_size) mDisplay.WY = PY - SH/2 + y_size; // hit top edge

      }
   }

   // correct for edges
   int clamp_x0 = 0;
   int clamp_x1 = 0;
   int clamp_y0 = 0;
   int clamp_y1 = 0;
   if (mDisplay.WX < 0)           { mDisplay.WX = 0;         clamp_x0 = 1; }
   if (mDisplay.WY < 0)           { mDisplay.WY = 0;         clamp_y0 = 1; }
   if (mDisplay.WX > (2000 - SW)) { mDisplay.WX = 2000 - SW; clamp_x1 = 1; }
   if (mDisplay.WY > (2000 - SH)) { mDisplay.WY = 2000 - SH; clamp_y1 = 1; }

   // used by get_new_background to only get what is needed
   mDisplay.level_display_region_x = mDisplay.WX;
   mDisplay.level_display_region_y = mDisplay.WY;
   mDisplay.level_display_region_w = SW;
   mDisplay.level_display_region_h = SH;

   // this is what all the previous calculations have been building up to:
   al_draw_scaled_bitmap(mBitmap.level_buffer, mDisplay.WX, mDisplay.WY, SW, SH, sbx, sby, sbw, sbh, 0);

  //printf("WX:%d, WY:%d, SW:%d, SH:%d, sbx:%d, sby:%d, sbw:%d, sbh:%d\n", WX, WY, SW, SH, sbx, sby, sbw, sbh);



   if (mDisplay.viewport_show_hyst)
   {
      float hx1 = mDisplay.SCREEN_W/2 - x_size * mDisplay.scale_factor_current;
      float hx2 = mDisplay.SCREEN_W/2 + x_size * mDisplay.scale_factor_current;
      float hy1 = mDisplay.SCREEN_H/2 - y_size * mDisplay.scale_factor_current;
      float hy2 = mDisplay.SCREEN_H/2 + y_size * mDisplay.scale_factor_current;
      if (mDisplay.viewport_mode == 0) {hx2+=20* mDisplay.scale_factor_current; hy2+=20* mDisplay.scale_factor_current;}
      if (clamp_x0) hx1 = 0;
      if (clamp_y0) hy1 = 0;
      if (clamp_x1) hx2 = mDisplay.SCREEN_W;
      if (clamp_y1) hy2 = mDisplay.SCREEN_H;
      al_draw_rectangle(hx1, hy1, hx2, hy2, mColor.pc[10], 0);
   }

   // in level editor mode, if the level is smaller than the screen edges, draw a thin line to show where it ends...
   if (type == 3)
   {
      sbw = mDisplay.SCREEN_W-bw*2; // recalc these beacuse they have been modified
      sbh = mDisplay.SCREEN_H-bw*2;
      int xdraw = 0;
      int ydraw = 0;
      int xl=mDisplay.SCREEN_W-bw; // default screen edge positions
      int yl=mDisplay.SCREEN_H-bw;
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
      if (xdraw) al_draw_line(xl, bw, xl, yl, mColor.pc[c], 0);
      if (ydraw) al_draw_line(bw, yl, xl, yl, mColor.pc[c], 0);
      //al_draw_rectangle(sbx, sby, sbx+sbw, sby+sbh, mColor.pc[c], 0);
   }

}

void mwScreen::set_map_var(void)
{
   // determine menu_map_size and position
   int y_size = mDisplay.SCREEN_H-160;
   int x_size = mDisplay.SCREEN_W-260;
   if (y_size < x_size) mLogo.menu_map_size = y_size;
   else mLogo.menu_map_size = x_size;
   if (mLogo.menu_map_size < 10) mLogo.menu_map_size = 10;
   mLogo.menu_map_x = mDisplay.SCREEN_W/2-(mLogo.menu_map_size/2);
   mLogo.menu_map_y = 140;

   // splash screen logo position
   mLogo.mdw_splash_logo_x = mDisplay.SCREEN_W/2;
   mLogo.mdw_splash_logo_y = mDisplay.SCREEN_H/2;

   // splash screen logo size
   float min_d = mDisplay.SCREEN_H;  // find miniumum dimension
   if (mDisplay.SCREEN_W < mDisplay.SCREEN_H) min_d = mDisplay.SCREEN_W;
   mLogo.mdw_splash_logo_scale = min_d / 500; // 400 is the exact size, make it bigger for padding

   // map screen logo position and size
   float sp = mLogo.menu_map_x - BORDER_WIDTH;    // how much space do I have between the map and the screen edge?
   mLogo.mdw_map_logo_scale = sp / 500; // 400 is the exact size, make it bigger for padding
   mLogo.mdw_map_logo_x = BORDER_WIDTH + sp/2;
   mLogo.mdw_map_logo_y = mLogo.menu_map_y + mLogo.mdw_map_logo_scale * 200; // align top of logo with top of map

   // this is the link from splash to map
   mLogo.mdw_logo_scale_dec = (mLogo.mdw_splash_logo_scale - mLogo.mdw_map_logo_scale) / 320;
   mLogo.mdw_logo_x_dec = (mLogo.mdw_splash_logo_x - mLogo.mdw_map_logo_x) / 320;
   mLogo.mdw_logo_y_dec = (mLogo.mdw_splash_logo_y - mLogo.mdw_map_logo_y) / 320;

//   printf("slx %f sly %f\n", mdw_splash_logo_x, mdw_splash_logo_y );
//   printf("mlx %d mly %d\n", mdw_map_logo_x, mdw_map_logo_y );
//   printf("xdec %f ydec %f\n", mdw_logo_x_dec, mdw_logo_y_dec );
//   printf("xdec %f ydec %f\n", mdw_logo_x_dec*320, mdw_logo_y_dec*320 );
}


void mwScreen::mark_non_default_block(int x, int y)
{
   int c = mLevel.l[x][y] & 1023;
   if ((mBitmap.sa[c][0] & PM_BTILE_MOST_FLAGS) != (mLevel.l[x][y] & PM_BTILE_MOST_FLAGS))
   {
      al_draw_line(x*20, y*20, x*20+20, y*20+20, mColor.pc[10], 1);
      al_draw_line(x*20+20, y*20, x*20, y*20+20, mColor.pc[10], 1);
   }
}

void mwScreen::init_level_background2(int s, int e)
{
   al_set_target_bitmap(mBitmap.level_background);
   al_draw_filled_rectangle(s*20, 0, e*20, 2000, al_map_rgb(0,0,0));
   for (int x=s; x<e; x++)
      for (int y=0; y<100; y++)
      {
         al_draw_bitmap(mBitmap.btile[mLevel.l[x][y] & 1023], x*20, y*20, 0);
         if ((mLoop.level_editor_running) && (mwWM.mW[1].show_non_default_blocks)) mark_non_default_block(x, y);
      }
}

void mwScreen::init_level_background(int type) // fill level_background with block tiles
{
   //printf("init_level_background\n");
   if (type == 0)
   {
      al_set_target_bitmap(mBitmap.level_background);
      al_clear_to_color(al_map_rgb(0,0,0));
      for (int x=0; x<100; x++)
         for (int y=0; y<100; y++)
         {
            al_draw_bitmap(mBitmap.btile[mLevel.l[x][y] & 1023], x*20, y*20, 0);
            if ((mLoop.level_editor_running) && (mwWM.mW[1].show_non_default_blocks)) mark_non_default_block(x, y);
         }

   }

   if (type == 1)
   {
      if (mLoop.frame_num % 40 == 0)
      {
         al_set_target_bitmap(mBitmap.level_background);
         al_clear_to_color(al_map_rgb(0,0,0));
         for (int x=0; x<100; x++)
            for (int y=0; y<100; y++)
            {
               al_draw_bitmap(mBitmap.btile[mLevel.l[x][y] & 1023], x*20, y*20, 0);
               if ((mLoop.level_editor_running) && (mwWM.mW[1].show_non_default_blocks)) mark_non_default_block(x, y);
            }
      }
   }
   if (type == 2)
   {
      int sq = mLoop.frame_num % 40;
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


void mwScreen::draw_level2(ALLEGRO_BITMAP *b, int mx, int my, int ms, int blocks, int items, int enemies, int lifts, int players)
{
   if (blocks) get_new_background(1);
   if (mLevel.valid_level_loaded)
   {
      if (lifts)   mLift.draw_lifts();
      if (items)   mItem.draw_items();
      if (enemies) mEnemy.draw_enemies();
      if (mLevel.resume_allowed)
      {
         if (players) mPlayer.draw_players();
         mShot.draw_eshots();
         mShot.draw_pshots();
      }
   }
   if (b == NULL) al_set_target_backbuffer(mDisplay.display);
   else al_set_target_bitmap(b);
   al_draw_scaled_bitmap(mBitmap.level_buffer, 0, 0, 2000, 2000, mx, my, ms, ms, 0);
}

void mwScreen::draw_level_centered(int screen_x, int screen_y, int level_x, int level_y, float scale_factor)
{
   // use scale factor to determine scaled size of level
   int size = (int)(scale_factor * 2000);
   int mgx = screen_x - (int) (level_x * scale_factor);  // start x pos on level
   int mgy = screen_y - (int) (level_y * scale_factor);  // start y pos on level
   al_draw_scaled_bitmap(mBitmap.level_buffer, 0, 0, 2000, 2000, mgx, mgy, size, size, 0);
}

void mwScreen::draw_level(void) // draws the map on the menu screen
{
   int blocks = 0;
   if (mLevel.valid_level_loaded) blocks = 1;

   draw_level2(NULL, mLogo.menu_map_x, mLogo.menu_map_y, mLogo.menu_map_size, blocks, 1, 1, 1, 1);

   int text_x = mDisplay.SCREEN_W / 2;
   int text_y = mLogo.menu_map_y - 16;
   al_draw_textf(mFont.pr8, mColor.pc[11], text_x, text_y, ALLEGRO_ALIGN_CENTRE, " Level %d ", mLevel.start_level );
   text_y += 8;

   if (mLevel.resume_allowed) al_draw_text(mFont.pr8, mColor.pc[14], text_x, text_y, ALLEGRO_ALIGN_CENTRE, "  (paused)  ");
   else if (mLevel.valid_level_loaded) al_draw_text(mFont.pr8, mColor.pc[9], text_x, text_y, ALLEGRO_ALIGN_CENTRE, "  start level  ");
   else al_draw_text(mFont.pr8, mColor.pc[10], text_x, text_y, ALLEGRO_ALIGN_CENTRE, "  not found !  ");
}


void mwScreen::frame_and_title(int show_players)
{
   int p = mPlayer.active_local_player;
   int color = mPlayer.syn[p].color;

   // draw the border
   for (int x = 0; x < BORDER_WIDTH; x++)
      al_draw_rectangle(x+0.5f, x+0.5f, (mDisplay.SCREEN_W-1-x)+0.5f, (mDisplay.SCREEN_H-1-x)+0.5f,  mColor.pc[color + (x * 16)], 1);

   // draw the title on top on the border
   draw_title(mDisplay.SCREEN_W/2, 2, 322, 32, color);



   int tc = mColor.get_contrasting_color(color);


   // draw the version text centered on the bottom of the border
   al_draw_textf(mFont.pr8, mColor.pc[tc], mDisplay.SCREEN_W/2, mDisplay.SCREEN_H-10, ALLEGRO_ALIGN_CENTRE, "Version %s", mLoop.pm_version_string);

   if (show_players)
   {
      // draw a line of players on each side of menu
      color = mPlayer.syn[p].color;               // initial color
      int lim = 12;                           // number of players to draw
      float flsc = 5;                         // initial scale
      float y_pos = 20;                       // initial y position
      float y_step = 20 / lim;                // inc to raise 20
      float x_pos = mDisplay.SCREEN_W/2 - 180;         // initial x position
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
         al_draw_scaled_rotated_bitmap(mBitmap.player_tile[color][1], 0, 0, x+10, y+10, flsc, flsc, 0, 0);
         if (++color > 15) color = 1;        // cycle through players colors
      }

      color = mPlayer.syn[p].color;               // initial color
      flsc = 5;                               // initial scale
      y_pos = 20;                             // initial y position
      y_step = 20 / lim;                      // inc to raise 20
      x_pos = mDisplay.SCREEN_W/2 + 80;                // initial x position
      dist = (mDisplay.SCREEN_W - BORDER_WIDTH) - (int)x_pos;   // how much space do I have?
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
         al_draw_scaled_rotated_bitmap(mBitmap.player_tile[color][1], 0, 0, x+10, y+10, flsc, flsc, 0, ALLEGRO_FLIP_HORIZONTAL);
         if (++color > 15) color = 1;        // cycle through players colors
      }
   }
}

void mwScreen::rtextout_centre(ALLEGRO_FONT *f, ALLEGRO_BITMAP *dbmp, const char *txt1, int x, int y, int col, float scale, float op)
{
   // draws stretched text
   // used in many places
   int sw = strlen(txt1) * 8;
   ALLEGRO_BITMAP *temp = al_create_bitmap(sw, 8);
   al_set_target_bitmap(temp);
   al_clear_to_color(al_map_rgba(0,0,0,0));

   al_draw_text(f, mColor.pc[col], sw/2, 0, ALLEGRO_ALIGN_CENTRE, txt1);

   if (dbmp != NULL) al_set_target_bitmap(dbmp);
   else al_set_target_backbuffer(mDisplay.display);

   al_draw_tinted_scaled_rotated_bitmap(temp, al_map_rgba_f(op, op, op, op), sw/2, 4, x, y, scale, scale, 0, 0);
   al_destroy_bitmap(temp);
}

void mwScreen::draw_title(int x, int y, int w, int h, int color)
{
   char msg[1024];
   sprintf(msg, "%s Martians!", mColor.color_name[color]); // overwrite with color name
   int bbx1, bby1, bbw1, bbh1;
   al_get_text_dimensions(mFont.sauc, msg, &bbx1, &bby1, &bbw1, &bbh1);
   if ((mBitmap.text_title_bitmaps_create) || (mBitmap.text_title_draw_color != color))
   {
      mBitmap.text_title_bitmaps_create = 0;
      mBitmap.text_title_draw_color = color;
      al_destroy_bitmap(mBitmap.text_title);
      mBitmap.text_title = al_create_bitmap(bbw1,bbh1);

      if(!mBitmap.text_title)
      {
         sprintf(msg, "Error creating text_title %d %d\n", bbw1,bbh1);
         mInput.m_err(msg);
      }
      //else printf("created text_title %d %d\n", bbw1,bbh1);
      al_set_target_bitmap(mBitmap.text_title);
      al_clear_to_color(al_map_rgba(0,0,0,0));
      al_draw_text(mFont.sauc, mColor.pc[color], 0-bbx1, 0-bby1, 0, msg);
   }
   al_set_target_backbuffer(mDisplay.display);
   al_draw_scaled_bitmap(mBitmap.text_title, 0, 0, bbw1, bbh1, x-w/2, y, w, h, 0);
}

void mwScreen::draw_large_text_overlay(int type, int color)
{
   char m1[80] = {0};
   char m2[80] = {0};
   int rebuild = 0;

   if ((type == 1) && (mBitmap.large_text_overlay_state != 1))
   {
      sprintf(m1, "PURPLE");
      sprintf(m2, "MARTIANS");
      mBitmap.large_text_overlay_state = 1;
      rebuild = 1;
   }

   if ((type == 2) && (mBitmap.large_text_overlay_state != 2))
   {
      color = mPlayer.syn[mPlayer.active_local_player].color;
      sprintf(m1, "LEVEL");
      sprintf(m2, "DONE");
      mBitmap.large_text_overlay_state = 2;
      rebuild = 1;
   }

   if ((type == 3) && (mBitmap.large_text_overlay_state != 3))
   {
      sprintf(m1, "DEMO");
      sprintf(m2, "MODE");
      mBitmap.large_text_overlay_state = 3;
      rebuild = 1;
   }

   if (rebuild)
   {

      int bbx1, bby1, bbw1, bbh1;
      int bbx2, bby2, bbw2, bbh2;
      al_get_text_dimensions(mFont.sauc, m1, &bbx1, &bby1, &bbw1, &bbh1);
      al_get_text_dimensions(mFont.sauc, m2, &bbx2, &bby2, &bbw2, &bbh2);
      // get max w and h
      float bbw3 = bbw1; if (bbw2 > bbw1) bbw3 = bbw2;
      float bbh3 = bbh1; if (bbh2 > bbh1) bbh3 = bbh2;

   //   printf("m1:%s  m2:%s \n",m1, m2);
   //   printf("bbx1:%4d bby1:%4d bbw1:%4d bbh1:%4d\n",bbx1, bby1, bbw1, bbh1);
   //   printf("bbx2:%4d bby2:%4d bbw2:%4d bbh2:%4d\n",bbx2, bby2, bbw2, bbh2);


      ALLEGRO_BITMAP *t1 = al_create_bitmap(bbw3, bbh3);
      al_set_target_bitmap(t1);
      al_clear_to_color(al_map_rgba(0,0,0,0));
      al_draw_text(mFont.sauc, mColor.pc[color], 0-bbx1 + (bbw3-bbw1)/2, 0-bby1, 0, m1);

      ALLEGRO_BITMAP *t2 = al_create_bitmap(bbw3, bbh3);
      al_set_target_bitmap(t2);
      al_clear_to_color(al_map_rgba(0,0,0,0));
      al_draw_text(mFont.sauc, mColor.pc[color], 0-bbx2 + (bbw3-bbw2)/2, 0-bby2, 0, m2);

      int xs = mDisplay.SCREEN_W*7/8; // x size
      int ys = mDisplay.SCREEN_H*3/8; // y size

      int x1 = (mDisplay.SCREEN_W - xs)/2;
      int x2 = xs;

      int yu1 = mDisplay.SCREEN_H*1/4 - ys/2;
      int yu2 = ys;

      int yl1 = mDisplay.SCREEN_H*3/4 - ys/2;
      int yl2 = ys;

      al_destroy_bitmap(mBitmap.large_text_overlay_bitmap);
      mBitmap.large_text_overlay_bitmap = al_create_bitmap(mDisplay.SCREEN_W, mDisplay.SCREEN_H);

      al_set_target_bitmap(mBitmap.large_text_overlay_bitmap);
      al_clear_to_color(al_map_rgb(0,0,0));


      al_draw_scaled_bitmap(t1, 0, 0, bbw3, bbh3, x1, yu1, x2, yu2, 0);
      al_draw_scaled_bitmap(t2, 0, 0, bbw3, bbh3, x1, yl1, x2, yl2, 0);

      al_destroy_bitmap(t1);
      al_destroy_bitmap(t2);
   }


   float opa = 1.0;
   if (type == 2) opa = 0.5;
   if (type == 3) opa = mDemoMode.demo_mode_overlay_opacity;
//   if (type == 3) opa = 0.05;
   ALLEGRO_COLOR fc = al_map_rgba_f(opa, opa, opa, opa);
   al_set_target_backbuffer(mDisplay.display);
   al_draw_tinted_bitmap(mBitmap.large_text_overlay_bitmap, fc, 0, 0, 0);
}

void mwScreen::draw_percent_barc(int cx, int y, int width, int height, int percent, int c1, int c2, int fc)
{
   int x = cx - width/2; // get x from center
   al_draw_filled_rectangle(x+0.5f, y+0.5f, x + width+0.5f, y + height+0.5f, mColor.pc[c1]); //  all c1 to start
   if (percent > 0)
   {
      int w2 = (int) (width * ((float)percent/100)); // how much green
      al_draw_filled_rectangle(x+0.5f, y+0.5f, x + w2+0.5f, y + height+0.5f, mColor.pc[c2]); //  then c2
   }

   if (fc) al_draw_rectangle(x-0.5f, y+0.5f, x+width+0.5f, y+height+0.5f, mColor.pc[fc], 1); //  frame

}

void mwScreen::draw_percent_bar(int cx, int y, int width, int height, int percent)
{
   int x = cx - width/2; // get x from center
   al_draw_filled_rectangle(x+0.5f, y+0.5f, x + width+0.5f, y + height+0.5f, mColor.pc[10]); //  all red to start
   if (percent > 0)
   {
      int w2 = (int) (width * ((float)percent/100)); // how much green
      al_draw_filled_rectangle(x+0.5f, y+0.5f, x + w2+0.5f, y + height+0.5f, mColor.pc[11]); //  green
   }
   al_draw_rectangle(x+0.5f, y+0.5f, x+width+0.5f, y+height+0.5f, mColor.pc[15], 1); //  white frame
}


void mwScreen::draw_percent_barf(float x1, float y1, float x2, float y2, float percent)
{
   al_draw_filled_rectangle(x1, y1, x2, y2, mColor.pc[10]); //  all red to start
   if (percent > 0)
   {
      float width = x2-x1-2;
      float x3 = x1 + 1 + (width * percent/100);
      al_draw_filled_rectangle(x1+0.5, y1, x3, y2, mColor.pc[11]); //  green
   }
   al_draw_rectangle(x1, y1, x2, y2, mColor.pc[15], 1); //  white frame
}






void draw_percent_bar_line(int cx, int y, int width, int height, int rise, int color, int percent )
{
   int x = cx - width/2; // get x from center
   int w2 = (int) (width * ((float)percent/100)); // position
   al_draw_line(x+0.5f+w2, y+0.5f-rise, x+0.5f+w2, y+0.5f+height+rise, mColor.pc[color], 1);
}

void mwScreen::draw_percent_bar_range(int cx, int y, int width, int height, int color, int start, int end)
{
   int x = cx - width/2; // get x from center
   int w1 = (int) (width * ((float)start/100));
   int w2 = (int) (width * ((float)end/100));
   al_draw_filled_rectangle(x+w1+0.5f, y+1.5f, x+w2+0.5f, y+height+0.5f, mColor.pc[color]);
   al_draw_rectangle(x+0.5f, y+0.5f, x + width+0.5f, y + height+0.5f, mColor.pc[15], 1); //  white frame
}

