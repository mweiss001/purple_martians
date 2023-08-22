// mwScreen.cpp

#include "pm.h"
#include "mwScreen.h"
#include "mwWindowManager.h"
#include "mwPlayer.h"
#include "mwLogo.h"
#include "mwDemoMode.h"
#include "mwDisplay.h"
#include "mwFont.h"
#include "mwBitmap.h"
#include "mwLift.h"
#include "mwColor.h"
#include "mwLoop.h"
#include "mwItem.h"
#include "mwEnemy.h"
#include "mwLevel.h"
#include "mwShot.h"
#include "mwInput.h"
#include "mwMain.h"
#include "mwNetgame.h"
#include "mwMenu.h"

mwScreen mScreen;

void mwScreen::get_new_background(int full)
{
   al_set_target_bitmap(mBitmap.level_buffer);
   if (full) al_draw_bitmap(mBitmap.level_background, 0, 0, 0);
   else
   {
      // this only grabs the visible region, in the interests of speed
      int x = level_display_region_x - 20; if (x < 0) x = 0;
      int y = level_display_region_y - 20; if (y < 0) y = 0;
      int w = level_display_region_w + 40; if (x+w > 2000) w = 2000-x;
      int h = level_display_region_h + 40; if (y+h > 2000) h = 2000-y;
      al_draw_bitmap_region(mBitmap.level_background, x, y, w, h, x, y, 0);
   }
}


void mwScreen::transition_cutscene(int i, int f, int debug_print)
{
   if ((!mNetgame.ima_server) && (!mNetgame.ima_client) && (!mDisplay.no_display))
   {
      if (debug_print)
      {
         const char* tcn[5] = {"nothing", "game", "menu", "gate"};
         printf("transition from %s to %s\n", tcn[i], tcn[f]);
      }


      int num_steps = transition_num_steps;
      float delay = (float)transition_delay/1000;

      if (mLoop.super_fast_mode)
      {
         num_steps = 1;
         delay = 0;
      }

      float fmxi=0;
      float fmyi=0;
      float fmsi=1;

      float fmxf=0;
      float fmyf=0;
      float fmsf=1;

      set_screen_display_variables();
      set_level_display_region_xy();
      set_map_var();


      // player's position in level
      float px = mPlayer.syn[mPlayer.active_local_player].x;
      float py = mPlayer.syn[mPlayer.active_local_player].y;

      if (i == 0) // nothing
      {
         fmxi = mDisplay.SCREEN_W/2;
         fmyi = mDisplay.SCREEN_H/2;
         fmsi = 0.005;
      }
      if (f == 0) // nothing
      {
         fmxf = mDisplay.SCREEN_W/2;
         fmyf = mDisplay.SCREEN_H/2;
         fmsf = 0.005;
      }
      if (i == 1) // game
      {
         fmsi = mDisplay.scale_factor_current;
         fmxi = screen_display_x + (px - level_display_region_x) * fmsi;
         fmyi = screen_display_y + (py - level_display_region_y) * fmsi;
      }
      if (f == 1) // game
      {
         fmsf = mDisplay.scale_factor_current;
         fmxf = screen_display_x + (px - level_display_region_x) * fmsf;
         fmyf = screen_display_y + (py - level_display_region_y) * fmsf;
      }
      if (i == 2) // menu
      {
         fmsi = (float)menu_level_display_size / 2000;
         fmxi = menu_level_display_x + px * fmsi;
         fmyi = menu_level_display_y + py * fmsi;
      }
      if (f == 2) // menu
      {
         fmsf = (float)menu_level_display_size / 2000;
         fmxf = menu_level_display_x + px * fmsf;
         fmyf = menu_level_display_y + py * fmsf;
      }
      if (i == 3) // gate
      {
         fmsi = mDisplay.scale_factor_current * (200.0 / 2000.0); // level icon size = 200;
         fmxi = gate_transition_x + px * fmsi;
         fmyi = gate_transition_y + py * fmsi;
      }
      if (f == 3) // gate
      {
         // restore viewport so transition lines up
         level_display_region_x = mScreen.gate_transition_wx;
         level_display_region_y = mScreen.gate_transition_wy;
         //mDisplay.set_scale_factor(mScreen.gate_transition_scale, 1); // disabled for now, I want to keep scale if changed while playing

         fmsf = mDisplay.scale_factor_current * (200.0 / 2000.0); // level icon size = 200;
         fmxf = gate_transition_x + px * fmsf;
         fmyf = gate_transition_y + py * fmsf;
      }
      do_transition(fmxi, fmyi, fmxf, fmyf, fmsi, fmsf, num_steps, delay);
   }
}

void mwScreen::draw_screen_frame(void)
{
   int c = mPlayer.syn[mPlayer.active_local_player].color;
   for (int x=0; x<BORDER_WIDTH; x++)
      al_draw_rectangle(x+0.5f, x+0.5f, (mDisplay.SCREEN_W-1-x)+0.5f, (mDisplay.SCREEN_H-1-x)+0.5f,  mColor.pc[c + (x * 16)], 1);
}

void mwScreen::do_transition(float fmxi, float fmyi, float fmxf, float fmyf, float sci, float scf, float num_steps, float delay)
{

   draw_level2(NULL, 0, 0, 0, 1, 1, 1, 1, 1); // redraw entire level in case only region has been drawn
   al_set_target_backbuffer(mDisplay.display);

   // get scale multiplier using compound interest formula
   float per = 1.0 + pow((scf/sci), (1.0/num_steps) ) - 1;

   float fmxinc = (fmxf - fmxi) / num_steps;
   float fmyinc = (fmyf - fmyi) / num_steps;

//   // debug draw initial
//   al_clear_to_color(al_map_rgb(0,0,0));
//   draw_screen_frame();
//   draw_level_centered_on_player_pos(fmxi, fmyi, sci);
//   al_draw_text(mFont.pr16, mColor.White, mDisplay.SCREEN_W/2, mDisplay.SCREEN_H/2, ALLEGRO_ALIGN_CENTER, "INITIAL" );
//   al_flip_display();
//   mInput.tsw();

   for (int s=0; s<num_steps; s++)
   {
      sci *= per;
      fmxi += fmxinc;
      fmyi += fmyinc;
      al_clear_to_color(al_map_rgb(0,0,0));
      draw_screen_frame();
      draw_level_centered_on_player_pos(fmxi, fmyi, sci);
      al_flip_display();
      al_rest(delay);
   }
}


void mwScreen::set_screen_display_variables(void)
{
// ----------------------------------------------------------------------
// step 1 - determine x, y, width and height to draw on the screen
// if width or height is bigger than scaled level, clamp to that size
// also shift x and y to center, but only if not in level editor
// ----------------------------------------------------------------------

   // default place and size to draw on screen
   int bw = BORDER_WIDTH;
   screen_display_x = bw;
   screen_display_y = bw;
   screen_display_w = mDisplay.SCREEN_W-bw*2;
   screen_display_h = mDisplay.SCREEN_H-bw*2;

   float sls = mDisplay.scale_factor_current * 2000;                      // get the scaled size of the entire level

   if (screen_display_w > sls) // is screen_display_w greater than entire level width?
   {
      float a = screen_display_w - sls;                          // how much greater?
      screen_display_w = sls;                                    // new screen_display_w = sls
      if (!mLoop.level_editor_running) screen_display_x += a/2;  // new screen_display_x draw xpos
   }

   if (screen_display_h > sls) // is screen_display_h greater than entire level height?
   {
      float a = screen_display_h - sls;                          // how much greater?
      screen_display_h = sls;                                    // new screen_display_h = sls
      if (!mLoop.level_editor_running) screen_display_y += a/2;  // new screen_display_y draw ypos
   }

// ----------------------------------------------------------------------
// step 2 - now that we know the screen buffer width and height
// scale that to find out the size of the region to grab from level buffer
// clamp to not grab more than entire level
// ----------------------------------------------------------------------
   level_display_region_w = (float) screen_display_w / mDisplay.scale_factor_current;
   level_display_region_h = (float) screen_display_h / mDisplay.scale_factor_current;
   if (level_display_region_w > 2000) level_display_region_w = 2000;
   if (level_display_region_h > 2000) level_display_region_h = 2000;
}


void mwScreen::set_level_display_region_xy(void)
{
// ----------------------------------------------------------------------
// use active local player to find where to grab the region from level buffer
// sets:
// mDisplay.level_display_region_x
// mDisplay.level_display_region_y
// ----------------------------------------------------------------------

   // shorter variable names
   int p = mPlayer.active_local_player;
   float px = mPlayer.syn[p].x + 10;
   float py = mPlayer.syn[p].y + 10;
   int w = level_display_region_w;
   int h = level_display_region_h;

   float pvx = mPlayer.syn[p].xinc;
   float pvy = mPlayer.syn[p].yinc;

   if (viewport_mode == 0) // this method always has the player in the middle of the screen
   {
      level_display_region_x = px - w/2 - 10;
      level_display_region_y = py - h/2 - 10;
   }
   else // scroll hysteresis (a rectangle in the middle of the screen where there is no scroll)
   {
      // if this is active, dont do any others
      if ((viewport_look_rocket) && (mPlayer.is_player_riding_rocket(p)))
      {
         level_display_region_x += mPlayer.syn[p].xinc * 1.8;
         level_display_region_y += mPlayer.syn[p].yinc * 1.8;
      }
      else
      {
         if (viewport_look_player_motion)
         {
            float im = 0.05;
            level_display_region_xinc += pvx * im;
            level_display_region_yinc += pvy * im;
         }

         if (viewport_look_up_down)
         {
            float shift_speed = .5;
            if (mPlayer.syn[p].up)   level_display_region_yinc -= shift_speed;
            if (mPlayer.syn[p].down) level_display_region_yinc += shift_speed;
         }

         if (viewport_look_player_facing_left_right)
         {
            float shift_speed = 0.2;
            if (mPlayer.syn[p].left_right) level_display_region_xinc += shift_speed;
            else                           level_display_region_xinc -= shift_speed;
         }
      }


      // maximum incs
      float mxi = 6;
      if (level_display_region_xinc > mxi)  level_display_region_xinc = mxi;
      if (level_display_region_xinc < -mxi) level_display_region_xinc = -mxi;

      float myi = 12;
      if (pvy < -8) myi = abs(pvy) * 1.5;
      if (level_display_region_yinc > myi)  level_display_region_yinc = myi;
      if (level_display_region_yinc < -myi) level_display_region_yinc = -myi;

      // inc decays
      if ((viewport_look_player_facing_left_right == 0) || (ldr_xmn_h) || (ldr_xmx_h))
      {
         if (pvx == 0) level_display_region_xinc *= 0.9; // decay when not moving
         if ((level_display_region_xinc > 0) && (pvx < 0)) level_display_region_xinc *= 0.2; // decay faster when switching direction
         if ((level_display_region_xinc < 0) && (pvx > 0)) level_display_region_xinc *= 0.2; // decay faster when switching direction
      }

      if (pvy == 0) level_display_region_yinc *= 0.9; // decay when not moving
      if ((level_display_region_yinc > 0) && (pvy < 0)) level_display_region_yinc *= 0.2; // decay faster when switching direction
      if ((level_display_region_yinc < 0) && (pvy > 0)) level_display_region_yinc *= 0.2; // decay faster when switching direction


      // apply x and y increments
      level_display_region_x += level_display_region_xinc;
      level_display_region_y += level_display_region_yinc;




      // get hyst variables
      float x_size = w * viewport_x_div/2;
      float y_size = h * viewport_y_div/2;

      // adjust to hyst borders
      float re = px - w/2 - x_size; // right edge
      ldr_xmx_h = 0;
      if (level_display_region_x <= re) // hit right edge (or equal to)
      {
         level_display_region_x = re;  // clamp to max
         ldr_xmx_h = 1;
      }
      float le = px - w/2 + x_size; // left edge
      ldr_xmn_h = 0;
      if (level_display_region_x >= le) // hit left edge (or equal to)
      {
         level_display_region_x = le;  // clamp to min
         ldr_xmn_h = 1;
      }
      float be = py - h/2 - y_size; // bottom edge
      ldr_ymx_h = 0;
      if (level_display_region_y <= be) // hit bottom edge (or equal to)
      {
         level_display_region_y = be;  // clamp to max
         ldr_ymx_h = 1;
      }
      float te = py - h/2 + y_size; // top edge
      ldr_ymn_h = 0;
      if (level_display_region_y >= te) // hit top edge (or equal to)
      {
         level_display_region_y = te;  // clamp to min
         ldr_ymn_h = 1;
      }
   }




   // clamp to entire level borders (to ensure viewport region is not out of bounds)
   ldr_xmn_a = 0;
   if (level_display_region_x <= 0)
   {
      level_display_region_x = 0;
//      level_display_region_xinc = 0;
      ldr_xmn_a = 1;
   }
   ldr_xmx_a = 0;
   if (level_display_region_x >= 2000 - w)
   {
      level_display_region_x = 2000 - w;
  //    level_display_region_xinc = 0;
      ldr_xmx_a = 1;
   }
   ldr_ymn_a = 0;
   if (level_display_region_y <= 0)
   {
      level_display_region_y = 0;
    //  level_display_region_yinc = 0;
      ldr_ymn_a = 1;
   }
   ldr_ymx_a = 0;
   if (level_display_region_y >= 2000 - h)
   {
      level_display_region_y = 2000 - h;
   //   level_display_region_yinc = 0;
      ldr_ymx_a = 1;
   }
}

void mwScreen::draw_scaled_level_region_to_display(int type)
{
   set_screen_display_variables();
   if (type != 3) set_level_display_region_xy();

   al_set_target_backbuffer(mDisplay.display);
   al_clear_to_color(al_map_rgb(0,0,0));
   draw_screen_frame();

   int ldrx = level_display_region_x;
   int ldry = level_display_region_y;
   int ldrw = level_display_region_w;
   int ldrh = level_display_region_h;

   // draw the level region from level buffer to display
   al_draw_scaled_bitmap(mBitmap.level_buffer, ldrx, ldry, ldrw, ldrh, screen_display_x, screen_display_y, screen_display_w, screen_display_h, 0);


   // show viewport hysteresis rectangle
   if ((viewport_show_hyst) && (viewport_mode != 0))
   {
      int col = 12;

      int x_size = ldrw * viewport_x_div/2;
      int y_size = ldrh * viewport_y_div/2;
      float hx1 = mDisplay.SCREEN_W/2 - x_size * mDisplay.scale_factor_current;
      float hx2 = mDisplay.SCREEN_W/2 + x_size * mDisplay.scale_factor_current;
      float hy1 = mDisplay.SCREEN_H/2 - y_size * mDisplay.scale_factor_current;
      float hy2 = mDisplay.SCREEN_H/2 + y_size * mDisplay.scale_factor_current;
      al_draw_rectangle(hx1, hy1, hx2, hy2, mColor.pc[col], 0);

      // if clamped by hyst region show line in different color
      col = 10;
      if (ldr_xmn_h) al_draw_line(hx1, hy1, hx1, hy2, mColor.pc[col], 1 );
      if (ldr_xmx_h) al_draw_line(hx2, hy1, hx2, hy2, mColor.pc[col], 1 );
      if (ldr_ymn_h) al_draw_line(hx1, hy1, hx2, hy1, mColor.pc[col], 1 );
      if (ldr_ymx_h) al_draw_line(hx1, hy2, hx2, hy2, mColor.pc[col], 1 );

      // if clamped by entire region show line in  in different color and one pixel bigger
      col = 14;
      if (ldr_xmn_a) al_draw_line(hx1-1, hy1, hx1-1, hy2, mColor.pc[col], 1 );
      if (ldr_xmx_a) al_draw_line(hx2+1, hy1, hx2+1, hy2, mColor.pc[col], 1 );
      if (ldr_ymn_a) al_draw_line(hx1, hy1-1, hx2, hy1-1, mColor.pc[col], 1 );
      if (ldr_ymx_a) al_draw_line(hx1, hy2+1, hx2, hy2+1, mColor.pc[col], 1 );
   }


   // in level editor mode, if the level is smaller than the screen edges, draw thin lines to show where it ends...
   if (type == 3)
   {
      int c = mPlayer.syn[mPlayer.active_local_player].color;
      int bw = BORDER_WIDTH;
      int sbx = screen_display_x;
      int sby = screen_display_y;
      int sbw = mDisplay.SCREEN_W-bw*2; // recalc because these have been modified
      int sbh = mDisplay.SCREEN_H-bw*2;
      int xdraw = 0;
      int ydraw = 0;
      int xl=mDisplay.SCREEN_W-bw; // default screen edge positions
      int yl=mDisplay.SCREEN_H-bw;
      float sls = mDisplay.scale_factor_current * 2000; // sls = scaled level size
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
   }
}

// to show in settings
void mwScreen::draw_hyst_rect(void)
{
   int bw = BORDER_WIDTH;
   float x_size = (mDisplay.SCREEN_W-bw*2) * viewport_x_div/2; // larger number is smaller window
   float y_size = (mDisplay.SCREEN_H-bw*2) * viewport_y_div/2;
   float hx1 = mDisplay.SCREEN_W/2 - x_size;
   float hx2 = mDisplay.SCREEN_W/2 + x_size;
   float hy1 = mDisplay.SCREEN_H/2 - y_size;
   float hy2 = mDisplay.SCREEN_H/2 + y_size;
   al_draw_rectangle(hx1, hy1, hx2, hy2, mColor.pc[10], 0);
}





void mwScreen::set_map_var(void)
{
   // set menu x and y
   menu_x = mDisplay.SCREEN_W/2;
   menu_y = 34;

   // get menu width and height
   int menu_num = 2;
   if (mMain.classic_mode) menu_num = 1;
   mMenu.get_zmenu_width_and_height(menu_num, menu_w, menu_h);

   // the ypos of level_display and level_info are the same and are directly under the menu
   menu_level_info_y = menu_level_display_y = menu_y + menu_h + 4;

   // level_info width is fixed at 200 or 25 char
   menu_level_info_w = 24 * 8;

   if (!mMain.classic_mode) menu_level_info_w = 0;




   // determine the size of the level_display

   // get the available y space left for the level display
   int y_size = mDisplay.SCREEN_H - menu_level_display_y - BORDER_WIDTH;

   // get the available x space left for the level display
   int x_size = mDisplay.SCREEN_W - menu_level_info_w - BORDER_WIDTH*2 - 4;

   // then use the minimum to set level display size
   if (y_size < x_size) menu_level_display_size = y_size;
   else menu_level_display_size = x_size;

   // now find out where to put everything..
   int total_width = menu_level_info_w + menu_level_display_size;
   menu_level_display_x = mDisplay.SCREEN_W/2 - total_width / 2;
   menu_level_info_x = menu_level_display_x + menu_level_display_size + 1;



   // splash logo scale
   float min_d = mDisplay.SCREEN_H;  // find miniumum dimension
   if (mDisplay.SCREEN_W < mDisplay.SCREEN_H) min_d = mDisplay.SCREEN_W;
   splash_logo_scale = min_d / 450; // 400 is the exact size, make it bigger for padding

   // splash logo position
   splash_logo_x = mDisplay.SCREEN_W/2;
   splash_logo_y = mDisplay.SCREEN_H/2;


   // menu logo scale
   // same size as menu h
   menu_logo_scale = (float)menu_h / 500; // 400 is the exact size, make it bigger for padding

   menu_logo_y = menu_y + menu_h/2;

   menu_logo_x = menu_x - menu_w/2 - menu_logo_scale*200 - 5;


//   // menu logo scale
//   // same size as info block width
//   menu_logo_scale = (float)menu_level_info_w / 500; // 400 is the exact size, make it bigger for padding
//
//   // menu logo position
//
//   menu_logo_x = menu_level_info_x + menu_level_info_w/2;
//
//   menu_logo_y = mDisplay.SCREEN_H - BORDER_WIDTH - menu_level_info_w/2;




//   // menu logo scale
//   float sp = menu_level_display_x - BORDER_WIDTH;    // how much space do I have between the map and the screen edge?
//   menu_logo_scale = sp / 500; // 400 is the exact size, make it bigger for padding
//
//   // menu logo position
//   //menu_logo_x = BORDER_WIDTH + sp/2; // left hand side
//   menu_logo_x = mDisplay.SCREEN_W - BORDER_WIDTH - sp/2; // right hand side
//   menu_logo_y = menu_level_display_y + menu_logo_scale * 200; // align top of logo with top of map




   // transition from splash logo to menu logo
   splash_logo_scale_dec = (splash_logo_scale - menu_logo_scale) / 320;
   splash_logo_x_dec     = (splash_logo_x     - menu_logo_x)     / 320;
   splash_logo_y_dec     = (splash_logo_y     - menu_logo_y)     / 320;






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


void mwScreen::init_level_background(void) // fill level_background with block tiles
{
   //double t0 = al_get_time();
   //printf("init_level_background\n");
   al_set_target_bitmap(mBitmap.level_background);
   al_clear_to_color(al_map_rgb(0,0,0));
   for (int x=0; x<100; x++)
      for (int y=0; y<100; y++)
      {
         int tile = mLevel.l[x][y] & 1023;
         if (tile) al_draw_bitmap(mBitmap.btile[tile], x*20, y*20, 0);
         if ((mLoop.level_editor_running) && (mWM.mW[1].show_non_default_blocks)) mark_non_default_block(x, y);
      }

   // if level 1 draw all the gates
   if (mLevel.play_level == 1)
   {
      for (int i=0; i<500; i++)
         if (mItem.item[i][0] == 18) mItem.draw_gate(i, mItem.item[i][4], mItem.item[i][5], 0);
   }
   char msg[256];
   // draw all messages that are always on
   for (int i=0; i<500; i++)
      if ((mItem.item[i][0] == 10) && (mItem.item[i][2] & PM_ITEM_PMSG_SHOW_ALWAYS))
         mItem.draw_pop_message(i, 2, 0, 0, 0, 0, msg);

   // printf("init_level_background time:%f\n", al_get_time() - t0);
}


void mwScreen::draw_level2(ALLEGRO_BITMAP *b, int mx, int my, int ms, int blocks, int items, int enemies, int lifts, int players)
{
   if (mLevel.valid_level_loaded)
   {
      if (blocks) get_new_background(1);
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

   // first erase hidden regions (only in not in level editor)
   if (!mLoop.level_editor_running) mItem.erase_hider_areas();

   // draw gate info
   for (int p=0; p<NUM_PLAYERS; p++)
      if ((mPlayer.syn[p].active) && (mPlayer.syn[p].marked_gate != -1))
      {
         mItem.draw_gate_info(mPlayer.syn[p].marked_gate);
      }

   for (int i=0; i<500; i++)
      if ((mItem.item[i][0] == 10) && (!strncmp(mItem.pmsgtext[i], "Level Statistics", 16))) mItem.draw_message(i, 0, 0, 0);

   if (b == NULL) al_set_target_backbuffer(mDisplay.display);
   else al_set_target_bitmap(b);
   al_draw_scaled_bitmap(mBitmap.level_buffer, 0, 0, 2000, 2000, mx, my, ms, ms, 0);
}


// used only in transitions
void mwScreen::draw_level_centered_on_player_pos(int screen_x, int screen_y, float scale_factor)
{
   // use scale factor to determine scaled size of level
   int sz = scale_factor * 2000;

   // find where to grab the source screen from based on the players position
   int px = mPlayer.syn[mPlayer.active_local_player].x;
   int py = mPlayer.syn[mPlayer.active_local_player].y;

   int mgx = screen_x - (px * scale_factor);  // start x pos on level
   int mgy = screen_y - (py * scale_factor);  // start y pos on level


   al_draw_scaled_bitmap(mBitmap.level_buffer, 0, 0, 2000, 2000, mgx, mgy, sz, sz, 0);
}

void mwScreen::draw_level_map_under_menu(void) // used only in menu
{
   int sz = menu_level_display_size;
   int x = menu_level_display_x;
   int y = menu_level_display_y;

   draw_level2(NULL, x, y, sz, 1, 1, 1, 1, 1);

   if (mMain.classic_mode) al_draw_rectangle(x, y, x+sz, y+sz, mColor.pc[15], 1);


   /*
   int text_x = mDisplay.SCREEN_W / 2;
   int text_y = menu_level_display_y - 16;
   int lev = mLevel.last_level_loaded;
   if (mMain.classic_mode)
   {
      text_y += 8;
      al_draw_textf(mFont.pr8, mColor.pc[11], text_x, text_y, ALLEGRO_ALIGN_CENTRE, " Level %d %s", lev, mLevel.data[lev].level_name);

      al_draw_textf(mFont.pr8, mColor.pc[11], text_x, text_y, ALLEGRO_ALIGN_CENTRE, " Level %d ", lev );
      text_y += 8;
      if (mLevel.resume_allowed) al_draw_text(mFont.pr8, mColor.pc[14], text_x, text_y, ALLEGRO_ALIGN_CENTRE, "  (paused)  ");
      else if (mLevel.valid_level_loaded) al_draw_text(mFont.pr8, mColor.pc[9], text_x, text_y, ALLEGRO_ALIGN_CENTRE, "  start level  ");
      else al_draw_text(mFont.pr8, mColor.pc[10], text_x, text_y, ALLEGRO_ALIGN_CENTRE, "  not found !  ");


   }
   else // story mode
   {
      //if (lev == 1) al_draw_text(mFont.pr8, mColor.pc[15], text_x, text_y+8, ALLEGRO_ALIGN_CENTRE, "Overworld");
   }
   */
}


void mwScreen::draw_level_info(void) // used only in menu
{
   char msg[80];
   int lev = mLevel.last_level_loaded;

   int x1 = menu_level_info_x;
   int x2 = x1 + menu_level_info_w;
   int xc = (x1+x2)/2;
   int y = menu_level_info_y;

   y+=2;

   al_draw_textf(mFont.pr8, mColor.pc[15], xc, y, ALLEGRO_ALIGN_CENTER, "Level %d", lev); y+=8;

   y+=1; al_draw_line(x1-1, y, x2+1, y, mColor.pc[15], 1); y+=2;

   al_draw_textf(mFont.pr8, mColor.pc[15], xc, y, ALLEGRO_ALIGN_CENTER, "%s", mLevel.data[lev].level_name); y+=8;

   if (lev > 1)
   {

      y+=1; al_draw_line(x1-1, y, x2+1, y, mColor.pc[15], 1); y+=2;

      mItem.draw_line(x1, x2, y, "Status",                mLevel.data[lev].status_text, mLevel.data[lev].status_color); y+=9;

      if (mLevel.data[lev].status < 2)
      {
         mItem.draw_line(x1, x2, y, "Par Time",           mItem.chrms( mLevel.data[lev].time_par,  msg),        15); y+=9;
         mItem.draw_line(x1, x2, y, "Purple Coins",       mItem.chrd(  mLevel.data[lev].tot_purple_coins, msg), 15); y+=9;
      }
      else // completed
      {
         if (mLevel.data[lev].time_best < mLevel.data[lev].time_par) mItem.draw_line(x1, x2, y, "Time < Par",  "Yes", 8);
         else                                                        mItem.draw_line(x1, x2, y, "Time < Par",  "No",  15);
         y+=9;


         int mpc = mLevel.data[lev].max_purple_coins_collected;
         int tpc = mLevel.data[lev].tot_purple_coins;
         int pcc = 15;
         if (mpc >= tpc) pcc = 8;
         mItem.draw_line(x1, x2, y, "Purple Coins",   mItem.chrd(mpc, tpc, msg), pcc); y+=9;


         y+=1; al_draw_line(x1-1, y, x2+1, y, mColor.pc[15], 1); y+=2;



         mItem.draw_line(x1, x2, y, "Par Time",           mItem.chrms( mLevel.data[lev].time_par,            msg), 15); y+=9;
         mItem.draw_line(x1, x2, y, "Min Time Overall",   mItem.chrms( mLevel.data[lev].time_best,           msg), 15); y+=9;
         mItem.draw_line(x1, x2, y, "Min Time w/coins",   mItem.chrms( mLevel.data[lev].time_best_all_coins, msg), 15); y+=9;
         mItem.draw_line(x1, x2, y, "Worst Time",         mItem.chrms( mLevel.data[lev].time_worst,          msg), 15); y+=9;
         mItem.draw_line(x1, x2, y, "Average Time",       mItem.chrms( mLevel.data[lev].time_average,        msg), 15); y+=9;
         mItem.draw_line(x1, x2, y, "Total Play Time",    mItem.chrms( mLevel.data[lev].time_total,          msg), 15); y+=9;
         y+=1; al_draw_line(x1-1, y, x2+1, y, mColor.pc[15], 1); y+=2;

         mItem.draw_line(x1, x2, y, "Times Played",       mItem.chrd(  mLevel.data[lev].times_played,        msg), 15); y+=9;
         mItem.draw_line(x1, x2, y, "Times Completed",    mItem.chrd(  mLevel.data[lev].times_beat,          msg), 15); y+=9;
         mItem.draw_line(x1, x2, y, "Times Quit",         mItem.chrd(  mLevel.data[lev].times_quit,          msg), 15); y+=9;

         y+=1; al_draw_line(x1-1, y, x2+1, y, mColor.pc[15], 1); y+=2;

         mItem.draw_line(x1, x2, y, "Min Player Deaths",  mItem.chrd(  mLevel.data[lev].min_respawns,        msg), 15); y+=9;
         mItem.draw_line(x1, x2, y, "Max Enemies Killed", mItem.chrd(  mLevel.data[lev].max_enemies_killed,  msg), 15); y+=9;
         mItem.draw_line(x1, x2, y, "Min Enemies Left",   mItem.chrd(  mLevel.data[lev].min_enemies_left,    msg), 15); y+=9;


      }
   }
   al_draw_rectangle(x1-1, menu_level_info_y, x2+1, y+1, mColor.pc[15], 1);
}


void mwScreen::draw_line_of_players(void)
{
   // draw a line of players on each side of menu


   float sc = (float)menu_h / 20; // initial scale has player the same height as menu
   float y = menu_y+sc*10;        // initial y position has player lined up with menu

   int xo = menu_w/2 + sc*10;           // offset player by the width of the menu and half player size
   float x1 = mDisplay.SCREEN_W/2 - xo; // initial x position
   float x2 = mDisplay.SCREEN_W/2 + xo; // initial x position
   int c = mPlayer.syn[mPlayer.active_local_player].color; // initial color is active local player's color

   int np = 0; // count players drawn

   int first = 1;

   while ((x1 > 30) && (np < 12))
   {
//      printf("c:%d sc:%f x2:%f y:%f\n", c, sc, x2, y);


      if (!first) al_draw_scaled_rotated_bitmap(mBitmap.player_tile[c][1], 10, 10, x1, y, sc, sc, 0, 0);
      al_draw_scaled_rotated_bitmap(mBitmap.player_tile[c][1], 10, 10, x2, y, sc, sc, 0, ALLEGRO_FLIP_HORIZONTAL);

      float sp = 10*sc + 20; // spacing based on scale
      x1 -= sp;
      x2 += sp;

      sc *= .75;           // reduce scale

      if (++c > 15) c = 1; // cycle through players colors

      np++;

      first = 0;

   }
}

void mwScreen::frame_and_title(void)
{
   int c = mPlayer.syn[mPlayer.active_local_player].color;
   int tc = mColor.get_contrasting_color(c);

   draw_screen_frame();

   // draw the version text centered on the bottom of the border
   al_draw_textf(mFont.pr8, mColor.pc[tc], mDisplay.SCREEN_W/2, mDisplay.SCREEN_H-10, ALLEGRO_ALIGN_CENTRE, "Version %s", mLoop.pm_version_string);

   // draw the title on top on the border
   draw_title(mDisplay.SCREEN_W/2, 2, 322, 32, c);

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



void mwScreen::draw_framed_text(int xc, int y, ALLEGRO_FONT *f, int col, const char* txt)
{
   // get text dimensions
   int bx, by, bw, bh;
   mFont.mw_get_text_dimensions(f, txt, bx, by, bw, bh);

   int xs = bw/2 + 2;
   int x1 = xc-xs;
   int x2 = xc+xs;

   float tfy = y-by+2;
   float fy1 = y+0.5;
   float fy2 = y+bh+3.5;

   // draw
   al_draw_filled_rectangle(       x1, fy1, x2, fy2, mColor.pc[0]);      // clear background
   al_draw_rectangle(              x1, fy1, x2, fy2, mColor.pc[col], 1); // frame
   al_draw_textf(f, mColor.pc[15], xc, tfy, ALLEGRO_ALIGN_CENTER, "%s", txt);
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
   if (type == 3) opa = mDemoMode.overlay_opacity;
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

