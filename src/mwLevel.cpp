// mwLevel.cpp

#include "pm.h"
#include "mwLevel.h"
#include "mwConfig.h"
#include "mwEnemy.h"
#include "mwLift.h"
#include "mwScreen.h"
#include "mwFont.h"
#include "mwInput.h"
#include "mwBitmap.h"
#include "mwColor.h"
#include "mwDemoRecord.h"
#include "mwDisplay.h"
#include "mwEditorMain.h"
#include "mwMiscFnx.h"
#include "mwLevelEditor.h"
#include "mwLoop.h"
#include "mwMain.h"
#include "mwNetgame.h"
#include "mwPlayer.h"
#include "mwVisualLevel.h"
#include "mwShot.h"
#include "mwTriggerEvent.h"


mwLevel mLevel;
mwLevel l2;

void mwLevel::create_bitmaps()
{
//   printf("mwLevel::create bitmaps()\n");

   // this bitmap format will be used for all bitmaps, it is never changed
   al_set_new_bitmap_format(ALLEGRO_PIXEL_FORMAT_ANY_32_WITH_ALPHA);

   // this bitmap format is used for all other bitmaps
   al_set_new_bitmap_flags(ALLEGRO_NO_PRESERVE_TEXTURE | ALLEGRO_VIDEO_BITMAP);

   if (!level_background) level_background = al_create_bitmap(2000, 2000);
   al_set_target_bitmap(level_background);
   al_clear_to_color(al_map_rgba(0,0,0,0));

   if (!level_buffer) level_buffer = al_create_bitmap(2000, 2000);
   al_set_target_bitmap(level_buffer);
   al_clear_to_color(al_map_rgba(0,0,0,0));
}



mwLevel::~mwLevel()
{
//   printf("mwLevel::destroy bitmaps()\n");

//   if (level_background) al_destroy_bitmap(level_background);
//   if (level_buffer) al_destroy_bitmap(level_buffer);
}




void mwLevel::set_start_level()
{
   set_start_level(start_level);
}


void mwLevel::set_start_level(int s)
{
   //printf("set start level%d\n", s);
   if (s < 1  ) s = 1;
   if (s > 399) s = 399;
   play_level = start_level = s;
   load_level_by_number(s, 0, 0);
   resume_allowed = 0;
   mConfig.save_config(PM_CFG_SAVE_START_LEVEL);
}

int mwLevel::level_exists(int level)
{
   char fn[256];
   sprintf(fn, "levels/level%03d.pml", level);
   if (al_filename_exists(fn)) return 1;
   return 0;
}


// returns the next level that exists after 'lev'
// if greater than max_lev:
// if wrap return 1
// if not wrap (clamp) return highest level less than max
int mwLevel::get_next_level(int lev, int max_lev, int wrap)
{
   int done = 0;
   while (!done)
   {
      if (level_exists(++lev)) done = 1;
      if (lev > max_lev)
      {
         if (wrap)
         {
            lev = 1;
            done = 1;
         }
         else
         {
            lev = get_prev_level(lev, max_lev, wrap);
            done = 1;
         }
      }
   }
   return lev;
}

// returns the first prev level that exists before 'lev'
// if less than 1
// if wrap return highest level less than max
// if not wrap (clamp) return level 1
int mwLevel::get_prev_level(int lev, int max_lev, int wrap)
{
   int done = 0;
   while (!done)
   {
      if (level_exists(--lev)) done = 1;
      if (lev < 1)
      {
         if (wrap)
         {
            lev = max_lev;
            lev = get_prev_level(lev, max_lev, wrap);
            done = 1;
         }
         else
         {
            lev = 1;
            done = 1;
         }
      }
   }
   return lev;
}




void mwLevel::change_block(int x, int y, int block)
{
   if ((x >= 0) && (x < 100) & (y >= 0) && (y < 100))
   {
      l[x][y] = block;
      if (!mMain.headless_server)
      {
         al_set_target_bitmap(level_background);
         al_draw_filled_rectangle(x*20, y*20, x*20+20, y*20+20, mColor.pc[0]);
         al_draw_bitmap(mBitmap.tile[block & PM_BTILE_TILENUM_MASK], x*20, y*20, 0);

         // check for overlays
         if (block & PM_BTILE_SHOW_OVERLAY) mScreen.draw_tile_overlay(block, x, y);
      }
   }
}

int mwLevel::is_block_empty(int x, int y, int test_block, int test_item, int test_enemy)
{
   int empty = 0; // default is not empty (because if x and y are not valid, I don't want to use it)
   if ((x >= 0) && (x < 100) & (y >= 0) && (y < 100)) // if valid x and y
   {
      empty = 1; // default is empty
      if (test_block) if (mLevel.l[x][y] & PM_BTILE_ALL_SOLID) empty = 0; // if any of the solids match, it is not empty
      if (test_enemy)
         for (int c=0; c<100; c++)
            if (mEnemy.Ei[c][0])
               if ((mEnemy.Ef[c][0] == x*20) && (mEnemy.Ef[c][1] == y*20)) empty = 0;
      if (test_item)
         for (int c=0; c<500; c++)
            if (mItem.item[c][0])
               if ((mItem.item[c][4] == x*20) && (mItem.item[c][5] == y*20)) empty = 0;
   }
   return empty;
}

#define PML_SIZE 139720

void mwLevel::pml_to_var(char * b) // for load level
{
   int sz = 0, offset = 0;
   sz = sizeof(l);                               memcpy(l,                               b+offset, sz); offset += sz;
   sz = sizeof(mItem.item);                      memcpy(mItem.item,                      b+offset, sz); offset += sz;
   sz = sizeof(mEnemy.Ei);                       memcpy(mEnemy.Ei,                       b+offset, sz); offset += sz;
   sz = sizeof(mEnemy.Ef);                       memcpy(mEnemy.Ef,                       b+offset, sz); offset += sz;
   sz = sizeof(mLift.cur);                       memcpy(mLift.cur,                       b+offset, sz); offset += sz;
   sz = sizeof(mLift.stp);                       memcpy(mLift.stp,                       b+offset, sz); offset += sz;
   sz = sizeof(mItem.mMessageText.messageTextA); memcpy(mItem.mMessageText.messageTextA, b+offset, sz); offset += sz;
   mItem.mMessageText.AtoV();
}

void mwLevel::var_to_pml(char * b) // for save level
{
   mItem.mMessageText.VtoA();
   int sz = 0, offset = 0;
   offset += sz; sz = sizeof(l);                               memcpy(b+offset, l,                               sz);
   offset += sz; sz = sizeof(mItem.item);                      memcpy(b+offset, mItem.item,                      sz);
   offset += sz; sz = sizeof(mEnemy.Ei);                       memcpy(b+offset, mEnemy.Ei,                       sz);
   offset += sz; sz = sizeof(mEnemy.Ef);                       memcpy(b+offset, mEnemy.Ef,                       sz);
   offset += sz; sz = sizeof(mLift.cur);                       memcpy(b+offset, mLift.cur,                       sz);
   offset += sz; sz = sizeof(mLift.stp);                       memcpy(b+offset, mLift.stp,                       sz);
   offset += sz; sz = sizeof(mItem.mMessageText.messageTextA); memcpy(b+offset, mItem.mMessageText.messageTextA, sz);
}



void mwLevel::init_level_background() // fill level_background with block tiles
{
   //double t0 = al_get_time();
   //printf("init_level_background\n");

   al_set_target_bitmap(level_background);
   al_clear_to_color(al_map_rgb(0,0,0));

   bool force_draw_tile_overlays = false;
   if (mLoop.level_editor_running && mEditorMain.show_tile_overlays) force_draw_tile_overlays = true;

   al_hold_bitmap_drawing(1);
   for (int x=0; x<100; x++)
      for (int y=0; y<100; y++)
      {
         int tile = l[x][y];

         // only draw tile if non-zero
         if (tile & PM_BTILE_TILENUM_MASK) al_draw_bitmap(mBitmap.tile[tile & PM_BTILE_TILENUM_MASK], x*20, y*20, 0);

         // draw tile overlays
         if ((tile & PM_BTILE_SHOW_OVERLAY) || (force_draw_tile_overlays)) mScreen.draw_tile_overlay(tile, x, y);

      }
   al_hold_bitmap_drawing(0);

   // mark tiles with non-default flags in level editor only
   // I do this outside the other l[100][100] iteration because if 'al_hold_bitmap_drawing()' is active, primitives will not draw
   if (mLoop.level_editor_running && mEditorMain.show_non_default_blocks)
      for (int x=0; x<100; x++)
         for (int y=0; y<100; y++)
            mScreen.mark_non_default_block(x, y, l[x][y]);

   // draw messages that are always on
   for (int i=0; i<500; i++)
      if ((mItem.item[i][0] == 10) && (mItem.item[i][2] & PM_ITEM_PMSG_SHOW_ALWAYS))
         mItem.draw_message(i, 2);

   //   printf("%3d init_level_background time:%f\n", mLevel.last_level_loaded, (al_get_time() - t0)*1000);

}


void mwLevel::get_new_background()
{
   al_set_target_bitmap(level_buffer);
   al_draw_bitmap(level_background, 0, 0, 0);
}

















// used only in transitions
void mwLevel::draw_level_centered_on_player_pos(int screen_x, int screen_y, float scale_factor)
{
   // get player's position in level (0-2000) and scale by scale factor
   float px = mPlayer.syn[mPlayer.active_local_player].x * scale_factor;
   float py = mPlayer.syn[mPlayer.active_local_player].y * scale_factor;

   // subtract player's position from screen position
   // this will get the origin of where to draw the scaled level
   // so that the players position will be at screen_x, screen_y
   int mgx = screen_x - px;  // start x pos on level
   int mgy = screen_y - py;  // start y pos on level

   // use scale factor to determine scaled size of level
   int sz = scale_factor * 2000;

   al_draw_scaled_bitmap(level_buffer, 0, 0, 2000, 2000, mgx, mgy, sz, sz, 0);
}


// converts ex, ey (0-2000) position within the level, to actual screen coordinates so we can draw directly on the screen
// pass it the center of the object's position, it will return the center also
void mwLevel::calc_actual_screen_position(float ex, float ey, float &ex1, float &ey1)
{
   ex1 = screen_display_x + (ex - level_display_region_x) * mDisplay.scale_factor_current;
   ey1 = screen_display_y + (ey - level_display_region_y) * mDisplay.scale_factor_current;
}





void mwLevel::set_screen_display_variables()
{
// ----------------------------------------------------------------------
// step 1 - determine x, y, width and height to draw on the screen
// if width or height is bigger than scaled level, clamp to that size
// also shift x and y to center, but only if not in level editor
// ----------------------------------------------------------------------

   // default place and size to draw on screen
   float bw = BORDER_WIDTH;

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


void mwLevel::set_level_display_region_xy()
{
// ----------------------------------------------------------------------
// use active local player to find where to grab the region from level buffer
// sets:
// level_display_region_x
// level_display_region_y
// ----------------------------------------------------------------------

   // shorter variable names
   int p = mPlayer.active_local_player;
   float px = mPlayer.syn[p].x + 10;
   float py = mPlayer.syn[p].y + 10;
   float w = level_display_region_w;
   float h = level_display_region_h;

   float pvx = mPlayer.syn[p].xinc;
   float pvy = mPlayer.syn[p].yinc;

//   printf("void mwLevel::set_level_display_region_xy() i:%d nl:%d p:%d  px: %.0f  py:%.0f  pvx: %.0f  pvy:%.0f  w:%.0f h:%.0f \n", instance, last_level_loaded, p, px, py, pvx, pvy, w, h);

   // printf("\nvoid mwLevel::set_level_display_region_xy() instance:%d level:%d frame:%d\n", instance, last_level_loaded, mLoop.frame_num);
   // printf("p:%d  px:%.0f  py:%.0f  pvx:%.0f  pvy:%.0f  w:%.0f  h:%.0f \n", p, px, py, pvx, pvy, w, h);
   //
   // printf("level_display_region_x:%.0f  level_display_region_y:%.0f\n", level_display_region_x, level_display_region_y);




   if (mScreen.viewport_mode == 0) // this method always has the player in the middle of the screen
   {
      level_display_region_x = px - w/2 - 10;
      level_display_region_y = py - h/2 - 10;
   }
   else // scroll hysteresis (a rectangle in the middle of the screen where there is no scroll)
   {
      // if this is active, don't do any others
      if ((mScreen.viewport_look_rocket) && (mPlayer.is_player_riding_rocket(p)))
      {
         level_display_region_x += pvx * 1.8;
         level_display_region_y += pvy * 1.8;
      }
      else
      {
         if (mScreen.viewport_look_player_motion)
         {
            float im = 0.05;
            level_display_region_xinc += pvx * im;
            level_display_region_yinc += pvy * im;
         }

         if (mScreen.viewport_look_up_down)
         {
            float shift_speed = .5;
            if (mPlayer.syn[p].up)   level_display_region_yinc -= shift_speed;
            if (mPlayer.syn[p].down) level_display_region_yinc += shift_speed;
         }

         if (mScreen.viewport_look_player_facing_left_right)
         {
            float shift_speed = 0.2;
            if (mPlayer.syn[p].left_right) level_display_region_xinc += shift_speed;
            else                           level_display_region_xinc -= shift_speed;
         }
      }

      // maximum xinc
      float mxi = 6;
      mxi = 2;

      // clamp to max xinc
      if (level_display_region_xinc > mxi)  level_display_region_xinc = mxi;
      if (level_display_region_xinc < -mxi) level_display_region_xinc = -mxi;


      // maximum yinc
      float myi = 12;
      myi = 2;

      // increase max yinc if moving up fast, like jumping on spring
      if (pvy < -8) myi = abs(pvy) * 1.5;

      // clamp to max yinc
      if (level_display_region_yinc > myi)  level_display_region_yinc = myi;
      if (level_display_region_yinc < -myi) level_display_region_yinc = -myi;


      // inc decays
      if ((mScreen.viewport_look_player_facing_left_right == 0) || (ldr_xmn_h) || (ldr_xmx_h))
      {
         if (pvx == 0)                                     level_display_region_xinc *= 0.9; // decay when not moving
         if ((pvx < 0) && (level_display_region_xinc > 0)) level_display_region_xinc *= 0.2; // decay faster when switching direction
         if ((pvx > 0) && (level_display_region_xinc < 0)) level_display_region_xinc *= 0.2; // decay faster when switching direction
      }

      if (pvy == 0)                                     level_display_region_yinc *= 0.9; // decay when not moving
      if ((pvy < 0) && (level_display_region_yinc > 0)) level_display_region_yinc *= 0.2; // decay faster when switching direction
      if ((pvy > 0) && (level_display_region_yinc < 0)) level_display_region_yinc *= 0.2; // decay faster when switching direction

      // apply x and y increments
      level_display_region_x += level_display_region_xinc;
      level_display_region_y += level_display_region_yinc;

      // get hyst variables
      float x_size = w * mScreen.viewport_x_div/2;
      float y_size = h * mScreen.viewport_y_div/2;


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

   //printf("\nmwLevel::load_level_l2(%d)\n", level);
//   printf("void mwLevel::set_level_display_region_xy() i:%d nl:%d  alp:%d  px1: %.0f  py1:%.0f   x:%.0f y:%.0f \n", instance, last_level_loaded, mPlayer.active_local_player, px, py, level_display_region_x, level_display_region_y);

   //printf("level_display_region_x:%.0f  level_display_region_y:%.0f\n", level_display_region_x, level_display_region_y);


}



void mwLevel::draw_scaled_level_region_to_display()
{
   set_screen_display_variables();

   if (!mLoop.level_editor_running) set_level_display_region_xy();

   if (mLoop.state[1] == PM_PROGRAM_STATE_DEMO_RECORD)
   {
      int sls = mDisplay.scale_factor_current * 2000;    // get the scaled size of the entire level

      int sdw = mDisplay.SCREEN_W-BORDER_WIDTH*2;
      if (sdw > sls) // is screen width greater than entire level width?
      {
         if (mDemoRecord.background_x_justify == 1) screen_display_x = BORDER_WIDTH; // left
         if (mDemoRecord.background_x_justify == 2) screen_display_x = mDisplay.SCREEN_W - sls - BORDER_WIDTH; // right
      }

      int sdh = mDisplay.SCREEN_H-BORDER_WIDTH*2;
      if (sdh > sls) // is screen height greater than entire level height?
      {
         if (mDemoRecord.background_y_justify == 1) screen_display_y = BORDER_WIDTH; // top
         if (mDemoRecord.background_y_justify == 2) screen_display_y = mDisplay.SCREEN_H - sls - BORDER_WIDTH; // bottom
      }
   }

   al_set_target_backbuffer(mDisplay.display);
   al_clear_to_color(al_map_rgb(0,0,0));

   // draw the level region from level buffer to display
   al_draw_scaled_bitmap(level_buffer, level_display_region_x, level_display_region_y, level_display_region_w, level_display_region_h, screen_display_x, screen_display_y, screen_display_w, screen_display_h, 0);


   // do not draw frame when game is running, only in level editor mode
   // it is drawn later in screen overlay to ensure it is on top of any direct draw objects
   if (mLoop.level_editor_running) mScreen.draw_screen_frame();

   // show viewport hysteresis rectangle
   if ((mScreen.viewport_show_hyst) && (mScreen.viewport_mode != 0))
   {
      int col = 12;

      float x_size = level_display_region_w * mScreen.viewport_x_div/2;
      float y_size = level_display_region_h * mScreen.viewport_y_div/2;
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

      // if clamped by entire region show line in different color and one pixel bigger
      col = 14;
      if (ldr_xmn_a) al_draw_line(hx1-1, hy1, hx1-1, hy2, mColor.pc[col], 1 );
      if (ldr_xmx_a) al_draw_line(hx2+1, hy1, hx2+1, hy2, mColor.pc[col], 1 );
      if (ldr_ymn_a) al_draw_line(hx1, hy1-1, hx2, hy1-1, mColor.pc[col], 1 );
      if (ldr_ymx_a) al_draw_line(hx1, hy2+1, hx2, hy2+1, mColor.pc[col], 1 );
   }


   // in level editor mode, if the level is smaller than the screen edges, draw thin lines to show where it ends...
   if (mLoop.level_editor_running)
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











void mwLevel::setup_players_after_level_load(int type)
{
   for (int p=0; p<NUM_PLAYERS; p++)
   {
      mPlayer.init_player(p, type);     // type 1 = full reset, type 2 = level done reset
      mItem.set_player_start_pos(p);    // get starting position for all players, active or not
   }
   mPlayer.syn[0].active = 1;
}




















int mwLevel::load_level_visual_select()
{
   int ret = mVisualLevel.visual_level_select(400);
   mLevelEditor.reset_variables_on_level_change();
   return ret;
}



int mwLevel::load_level_file_dialog()
{
   char filename[256];
   sprintf(filename,"levels\\");
   if (mMiscFnx.mw_file_select("Load Level", filename, ".pml", 0))
   {
      int ret = load_level(filename, 0, 0);
      mLevelEditor.reset_variables_on_level_change();
      return ret;
   }
   return 0; // user pressed cancel
}



void mwLevel::draw_level2(ALLEGRO_BITMAP *b, int mx, int my, int ms, int blocks, int items, int enemies, int lifts, int players, int shots)
{
   if (!valid_level_loaded) return;

   if (blocks)  get_new_background();
   if (lifts)   mLift.draw_lifts();
   if (items)   mItem.draw_items();
   if (enemies) mEnemy.draw_enemies();
   if (players) mPlayer.draw_players(1);
   if (shots)   mShot.draw_eshots();
   if (shots)   mShot.draw_pshots();

   if (!mNetgame.server_remote_control) // why am I testing for this? // probably because there is no point is doing it in remote mode
   {
      // first erase hidden regions (only in not in level editor)
      if (!mLoop.level_editor_running) mItem.erase_hider_areas();

      // this will show level stats on menu level
      //for (int i=0; i<500; i++)
      //   if ((mItem.item[i][0] == 10) && (!strncmp(mItem.pmsgtext[i], "Level Statistics", 16))) mItem.draw_pmsg(i, 0, 0, 0);

   }


   if (b == NULL) al_set_target_backbuffer(mDisplay.display);
   else al_set_target_bitmap(b);
   al_draw_scaled_bitmap(level_buffer, 0, 0, 2000, 2000, mx, my, ms, ms, 0);


}









int mwLevel::draw_level3(ALLEGRO_BITMAP *b, int level, int mx, int my, int ms, int blocks, int items, int enemies, int lifts, int players, int shots)
{
   if (load_level_l2(level))
   {
      draw_level2(b, mx, my, ms, blocks, items, enemies, lifts, players, shots);
      return 1;
   }
   return 0;
}



// this is used to load l2 only, skips stuff that normal load does
int mwLevel::load_level_l2(int level)
{
   instance = 1;
   //printf("\nmwLevel::load_level_l2(%d)\n", level);
   if (valid_level_loaded && last_level_loaded == level)
   {
      //printf("already loaded!!!\n");
      return 1;
   }
   if (load_level_by_number(level, 1, 0))
   {


      //printf("\nmwLevel::load_level_l2(%d)\n", level);
      //printf("loading!!!\n");
      valid_level_loaded = 1;
      mLift.lift_setup();
      for (int x=0; x<500; x++)
         if (mItem.item[x][0]) // only if active set x y
         {
            mItem.itemf[x][0] = mItem.item[x][4];
            mItem.itemf[x][1] = mItem.item[x][5];
         }
      setup_players_after_level_load(2);

      // copy the level_display_region variable from mLevel to l2
      // this is needed to make the transition final variables match what happens when next level is actually loaded
      level_display_region_x = mLevel.level_display_region_x;
      level_display_region_y = mLevel.level_display_region_y;
      level_display_region_w = mLevel.level_display_region_w;
      level_display_region_h = mLevel.level_display_region_h;

      // set the screen_display and level_display variables
      set_screen_display_variables();
      set_level_display_region_xy();

      if (!mMain.headless_server) init_level_background();
//      printf("l2 px1: %.0f  py1:%.0f \n", mPlayer.syn[0].x, mPlayer.syn[0].y);
//      printf("l2:%d  px1: %.0f  py1:%.0f \n", last_level_loaded, mPlayer.syn[0].x, mPlayer.syn[0].y);
      return 1;
   }
   return 0;
}








int mwLevel::load_level_by_number(int level_num, int load_only, int fail_silently)
{
   char filename[255];
   sprintf(filename, "levels/level%03d.pml", level_num);
   // printf("mwLevel::load_level_by_number(level_num:%d, load_only:%d, fail_silently:%d) - filename:%s\n", level_num, load_only, fail_silently, filename);

   return load_level(filename, load_only, fail_silently);
}

int mwLevel::load_level(const char* filename, int load_only, int fail_silently)
{
//   printf("\nmwLevel::load_level_filename(filename:%s, load_only:%d, fail_silently:%d)\n", filename, load_only, fail_silently);
   char msg[1024];
   FILE *fp = fopen(filename, "rb");
   if (!fp)
   {
      sprintf(msg, "Error opening %s", filename);
      if (!fail_silently) mInput.m_err(msg);
      return 0;
   }
   else  // file open !
   {
      // read the compressed data
      char cmp[PML_SIZE];
      fread(cmp, sizeof(cmp), 1, fp);
      fclose(fp);

      // decompress cmp to pml
      char pml[PML_SIZE];
      uLongf destLen = sizeof(pml);
      uncompress((Bytef*)pml, (uLongf*)&destLen, (Bytef*)cmp, sizeof(cmp));

      // copy to game variables
      zero_level_data();
      pml_to_var(pml);

      // get the level number from the filename
      // search filename for the string 'level' followed by three digits
      std::string fn(filename);

      int search_pos = 0;
      int level_num = 0;
      int done = 0;
      while (!done)
      {
         // find the substring
         size_t foundPos = fn.find("level", search_pos);

         // check if the substring was found
         if (foundPos != std::string::npos)
         {
            //printf("substring found at:%d\n", (int) foundPos);

            // convert next 3 char to int
            int num = atoi(fn.substr(foundPos+5, 3).c_str());

            if (num)
            {
               level_num = num;
               done = 1;
            }
            else search_pos +=5;
         }
         else
         {
            printf("error extracting level number from filename:%s\n", filename);
            return 0;
         }
      }


      prev_level_loaded = last_level_loaded;
      last_level_loaded = level_num;
      valid_level_loaded = 0;
      resume_allowed = 0;

      if (!load_only)
      {
         if (level_num == 1) set_overworld_barriers();

         valid_level_loaded = 1;
         mLift.lift_setup();
         for (int x=0; x<500; x++)
            if (mItem.item[x][0]) // only if active set x y
            {
               mItem.itemf[x][0] = mItem.item[x][4];
               mItem.itemf[x][1] = mItem.item[x][5];
            }

         if (mLoop.level_editor_running)
         {
            mItem.sort_item(1);
            mEnemy.sort_enemy();
         }

         level_check();

         mShot.clear_shots();

         mTriggerEvent.initialize();

         if (!mMain.headless_server) init_level_background(); // draw blocks on level_background

         mPlayer.init_player(0, 2);
         mItem.set_player_start_pos(0);

         mLoop.eco_draw = 0;


      }

//      printf("mLevel:%d  px1: %.0f  py1:%.0f \n", last_level_loaded, mPlayer.syn[0].x, mPlayer.syn[0].y);



      return 1;
   }
}




void mwLevel::save_level(int level_num)
{
   char filename[255];
   sprintf(filename, "levels/level%03d.pml", level_num);
   //printf("saving: %s\n", filename);


   if (make_backup_on_save_level && level_num != 1)
   {
      // rename existing file as backup

      // get timestamp
      char timestamp[256];
      time_t now = time(NULL);
      struct tm *timenow = localtime(&now);
      strftime(timestamp, sizeof(timestamp), "%Y%m%d-%H%M%S", timenow);

      char backup_filename[1024];
      sprintf(backup_filename, "levels/level%03d_%s.pml", level_num, timestamp);

      // std::rename returns 0 on success
      if (std::rename(filename, backup_filename) == 0) printf("File [%s] renamed successfully!\n", backup_filename);
      else                                             printf("Error renaming file [%s]\n", backup_filename);
   }

   last_level_loaded = level_num;
   level_check();

   mItem.sort_item(1);
   mEnemy.sort_enemy();
   mLift.lift_setup();

   // put variables in pml
   char pml[PML_SIZE];
   var_to_pml(pml);

   // compress pml to cmp
   char cmp[PML_SIZE];
   uLongf destLen= sizeof(cmp);
   compress2((Bytef*)cmp, (uLongf*)&destLen, (Bytef*)pml, sizeof(pml), -1);
   int cmp_size = destLen;


   // write cmp to file
   FILE *fp = fopen(filename,"wb");
   fwrite(cmp, cmp_size, 1, fp);
   fclose(fp);
}





int mwLevel::save_level_prompt()
{
   int num = last_level_loaded;

   char title[80];
   sprintf(title,"Save Level %d ",num);

   char lf[256];
   sprintf(lf,"levels\\level%03d.pml", num);
   if (mMiscFnx.mw_file_select(title, lf, ".pml", 1))
   {
      int len = strlen(lf);
      char g[10];
      g[0] = lf[len-7];
      g[1] = lf[len-6];
      g[2] = lf[len-5];
      g[3] = 0;
      num = atoi(g);
      save_level(num);
      return 1;
   }
   return 0; // user pressed cancel
}


int mwLevel::show_level_data(int x_pos, int y_pos, int type)
{
   if (type == 1) al_clear_to_color(al_map_rgb(0,0,0));

   int ey_pos = mEnemy.show_enemy_data(x_pos, y_pos);
   int iy_pos = mItem.item_data(x_pos+135, y_pos);
   ey_pos = ey_pos + 8;
   al_draw_textf(mFont.pr8, mColor.pc[15], x_pos, ey_pos, 0,"%d Lifts  ", mLift.get_num_lifts());
   ey_pos += 8;
   al_draw_text(mFont.pr8, mColor.pc[15], x_pos, ey_pos, 0, "-------");
   ey_pos += 8;

   if (type == 1)
   {
      al_flip_display();
      mInput.tsw();
   }

   if (iy_pos > ey_pos) return iy_pos;
   else return ey_pos;
}

void mwLevel::zero_level_data()
{
   for (int c=0; c<100; c++)    // blocks
      for (int x=0; x<100; x++)
           l[c][x] = 0;

   mItem.mMessageText.initialize();

   for (int c=0; c<500; c++)  // items
   {
      for (int x=0; x<16; x++)  mItem.item[c][x] = 0;
      for (int x=0; x<4; x++)   mItem.itemf[c][x] = 0;
   }
   mItem.sort_item(1);

   for (int c=0; c<100; c++)
   {
      for (int x=0; x<16; x++)  mEnemy.Ef[c][x] = 0; // enemy floats
      for (int x=0; x<32; x++)  mEnemy.Ei[c][x] = 0; // enemy ints
   }
   mEnemy.sort_enemy();

   for (int c=0; c<40; c++) // lifts
   {
      mLift.clear_lift(c);
      for (int x=0; x<40; x++)
         mLift.clear_lift_step(c,x);
   }
}

void mwLevel::level_check()
{
   int lev = last_level_loaded;

   // set number of starts - used for drawing, calculating spawn points, etc
   number_of_starts = 0;

   // number of purple coins
   data[lev].tot_coins = 0;

   // min val for exit enemy lock
   data[lev].min_enemies_left_par = 100;

   for (int i=0; i<500; i++)
   {
      if (mItem.item[i][0] == 5) number_of_starts++;
      if ((mItem.item[i][0] == 2) && (mItem.item[i][6] == 1)) data[lev].tot_coins++;
      if ((mItem.item[i][0] == 3) && (mItem.item[i][8] < data[lev].min_enemies_left_par)) data[lev].min_enemies_left_par = mItem.item[i][8];
   }

   char msg[256];
   int error = 0;

   // count number of starts
   number_of_starts = 0;
   int s[8] = {0};

   for (int i=0; i<500; i++)
      if (mItem.item[i][0] == 5)
      {
         int x = mItem.item[i][7]; // start index
         if (x < 0)
         {
            sprintf(msg, "Level %d has a start index less than 0 (%d)", lev, x);
            mInput.m_err(msg);
            error = 1;
         }
         if (x > 7)
         {
            sprintf(msg, "Level %d has a start index greater than 70 (%d)", lev, x);
            mInput.m_err(msg);
            error = 1;
         }
         if ((x >=0) && (x<8))
         {
            number_of_starts++;
            s[x]++; // save index of this start
         }
      }

   if (number_of_starts == 0)
   {
      sprintf(msg, "Level %d has no start block", lev);
      mInput.m_err(msg);
      error = 1;
   }

   if (number_of_starts > 8 )
   {
      sprintf(msg, "Level %d has more than 8 start blocks (%d)", lev, number_of_starts);
      mInput.m_err(msg);
      error = 1;
   }
   for (int i=0; i<8; i++)
      if (s[i] > 1)
      {
         sprintf(msg, "Level %d has duplicate start blocks with index %d", lev, i);
         mInput.m_err(msg);
         error = 1;
      }

   if (number_of_starts > 1)
      for (int i=1; i<number_of_starts; i++)
         if (s[i] == 0)
         {
            sprintf(msg, "Level %d has %d starts but no start with index %d", lev, number_of_starts, i);
            mInput.m_err(msg);
            error = 1;
         }
   if (error)
   {
      printf("Level Check Error: %s\n", msg);
      printf("number_of_starts:%d\n", number_of_starts);
      for (int i=0; i<8; i++)
         printf("%d %d\n", i, s[i]);
   }

}


