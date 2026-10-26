// mwScreen.cpp

#include "pm.h"
#include "mwScreen.h"
#include "mwDisplay.h"
#include "mwBitmap.h"
#include "mwFont.h"
#include "mwColor.h"
#include "mwDemoMode.h"
#include "mwLoop.h"
#include "mwMiscFnx.h"
#include "mwLevel.h"
#include "mwInput.h"
#include "mwMain.h"
#include "mwMenu.h"

mwScreen mScreen;


void mwScreen::draw_screen_frame()
{
   int c = mLevel.mPlayer.syn[mLevel.mPlayer.active_local_player].color;
   for (int x=0; x<BORDER_WIDTH; x++)
      al_draw_rectangle(x+0.5f, x+0.5f, (mDisplay.SCREEN_W-1-x)+0.5f, (mDisplay.SCREEN_H-1-x)+0.5f, mColor.pc[c + 64 + (x * 16)], 1);
}


// to show in settings
void mwScreen::draw_hyst_rect()
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



void mwScreen::set_map_var()
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

   // transition from splash logo to menu logo
   splash_logo_scale_dec = (splash_logo_scale - menu_logo_scale) / 320;
   splash_logo_x_dec     = (splash_logo_x     - menu_logo_x)     / 320;
   splash_logo_y_dec     = (splash_logo_y     - menu_logo_y)     / 320;
}


void mwScreen::mark_non_default_block(int x, int y, int tile)
{
   if ((mBitmap.tileFlags[tile & PM_BTILE_TILENUM_MASK] & PM_BTILE_ALL_FLAGS) != (tile & PM_BTILE_ALL_FLAGS))
   {
      al_draw_line(x*20, y*20, x*20+20, y*20+20, mColor.pc[10], 1);
      al_draw_line(x*20+20, y*20, x*20, y*20+20, mColor.pc[10], 1);
   }
}

void mwScreen::draw_tile_overlay(int tile, int x, int y)
{
   int draw_overlay = 0;
   int kb = (tile & 0b00000000000000001110000000000000) >> 13;
   if (kb > 3) draw_overlay = 1;
   if (tile & PM_BTILE_BOMBABLE) draw_overlay = 2;
   if ((tile & PM_BTILE_BREAKABLE_PSHOT) || (tile & PM_BTILE_BREAKABLE_ESHOT)) draw_overlay = 3;

   // only draw one overlay
   if (draw_overlay == 1) al_draw_bitmap(mBitmap.tile[kb+154-4], x*20, y*20, 0);
   if (draw_overlay == 2) al_draw_bitmap(mBitmap.tile[159],      x*20, y*20, 0);
   if (draw_overlay == 3) al_draw_bitmap(mBitmap.tile[158],      x*20, y*20, 0);
}



void mwScreen::draw_level_map_under_menu() // used only in menu
{
   int sz = menu_level_display_size;
   int x = menu_level_display_x;
   int y = menu_level_display_y;

   mLevel.draw_level2(NULL, x, y, sz, 1, 1, 1, 1, 1);

   if (mMain.classic_mode) al_draw_rectangle(x, y, x+sz, y+sz, mColor.pc[15], 1);
}



void mwScreen::draw_level_info() // used only in menu
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
   if (lev > 1 && lev < 100)
   {
      y+=1; al_draw_line(x1-1, y, x2+1, y, mColor.pc[15], 1); y+=2;
      mLevel.mItem.draw_line(x1, x2, y, "Status",                mLevel.level_status_text[mLevel.data[lev].status], mLevel.level_status_color[mLevel.data[lev].status]); y+=9;
      if (mLevel.data[lev].status < 2)
      {
         mLevel.mItem.draw_line(x1, x2, y, "Par Time",           mMiscFnx.chrms( mLevel.data[lev].time_par,  msg), 15); y+=9;
         mLevel.mItem.draw_line(x1, x2, y, "Purple Coins",       mLevel.mItem.chrd(     mLevel.data[lev].tot_coins, msg), 15); y+=9;
      }
      else // completed
      {
         if (mLevel.data[lev].time_best < mLevel.data[lev].time_par) mLevel.mItem.draw_line(x1, x2, y, "Time < Par",  "Yes", 8);
         else                                                        mLevel.mItem.draw_line(x1, x2, y, "Time < Par",  "No",  15);
         y+=9;
         int mpc = mLevel.data[lev].max_coins_collected;
         int tpc = mLevel.data[lev].tot_coins;
         int pcc = 15;
         if (mpc >= tpc) pcc = 8;
         mLevel.mItem.draw_line(x1, x2, y, "Purple Coins",   mLevel.mItem.chrd(mpc, tpc, msg), pcc); y+=9;

         y+=1; al_draw_line(x1-1, y, x2+1, y, mColor.pc[15], 1); y+=2;

         mLevel.mItem.draw_line(x1, x2, y, "Par Time",           mMiscFnx.chrms( mLevel.data[lev].time_par,            msg), 15); y+=9;
         mLevel.mItem.draw_line(x1, x2, y, "Min Time Overall",   mMiscFnx.chrms( mLevel.data[lev].time_best,           msg), 15); y+=9;
         mLevel.mItem.draw_line(x1, x2, y, "Min Time w/coins",   mMiscFnx.chrms( mLevel.data[lev].time_best_all_coins, msg), 15); y+=9;
         mLevel.mItem.draw_line(x1, x2, y, "Worst Time",         mMiscFnx.chrms( mLevel.data[lev].time_worst,          msg), 15); y+=9;
         mLevel.mItem.draw_line(x1, x2, y, "Average Time",       mMiscFnx.chrms( mLevel.data[lev].time_average,        msg), 15); y+=9;
         mLevel.mItem.draw_line(x1, x2, y, "Total Play Time",    mMiscFnx.chrms( mLevel.data[lev].time_total,          msg), 15); y+=9;
         y+=1; al_draw_line(x1-1, y, x2+1, y, mColor.pc[15], 1); y+=2;

         mLevel.mItem.draw_line(x1, x2, y, "Times Played",       mLevel.mItem.chrd(  mLevel.data[lev].times_played,        msg), 15); y+=9;
         mLevel.mItem.draw_line(x1, x2, y, "Times Completed",    mLevel.mItem.chrd(  mLevel.data[lev].times_beat,          msg), 15); y+=9;
         mLevel.mItem.draw_line(x1, x2, y, "Times Quit",         mLevel.mItem.chrd(  mLevel.data[lev].times_quit,          msg), 15); y+=9;

         y+=1; al_draw_line(x1-1, y, x2+1, y, mColor.pc[15], 1); y+=2;

         mLevel.mItem.draw_line(x1, x2, y, "Min Player Deaths",  mLevel.mItem.chrd(  mLevel.data[lev].min_respawns,        msg), 15); y+=9;
         mLevel.mItem.draw_line(x1, x2, y, "Max Enemies Killed", mLevel.mItem.chrd(  mLevel.data[lev].max_enemies_killed,  msg), 15); y+=9;
         mLevel.mItem.draw_line(x1, x2, y, "Min Enemies Left",   mLevel.mItem.chrd(  mLevel.data[lev].min_enemies_left,    msg), 15); y+=9;
      }
   }

   al_draw_rectangle(x1-1, menu_level_info_y, x2+1, y+1, mColor.pc[15], 1);
}


void mwScreen::draw_line_of_players()
{
   // draw a line of players on each side of menu

   float sc = (float)menu_h / 20; // initial scale has player the same height as menu
   float y = menu_y+sc*10;        // initial y position has player lined up with menu

   int xo = menu_w/2 + sc*10;           // offset player by the width of the menu and half player size
   float x1 = mDisplay.SCREEN_W/2 - xo; // initial x position
   float x2 = mDisplay.SCREEN_W/2 + xo; // initial x position
   int c = mLevel.mPlayer.syn[mLevel.mPlayer.active_local_player].color; // initial color is active local player's color

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


void mwScreen::frame_and_title()
{
   draw_screen_frame();

   // draw the version text centered on the bottom of the border
   al_draw_textf(mFont.pr8, mColor.White, mDisplay.SCREEN_W/2, mDisplay.SCREEN_H-10, ALLEGRO_ALIGN_CENTRE, "Version %s", mLoop.pm_version_string);

   // draw the title on top on the border
   draw_title(mDisplay.SCREEN_W/2, 2, 322, 32, mLevel.mPlayer.syn[mLevel.mPlayer.active_local_player].color);
}


void mwScreen::rtextout_centref(ALLEGRO_FONT *f, ALLEGRO_BITMAP *dbmp, int x, int y, int col, float scale, float op, const char *format, ...)
{
   char smsg[200];
   va_list args;
   va_start(args, format);
   vsprintf(smsg, format, args);
   va_end(args);
   rtextout_centre(f, dbmp, x, y, col, scale, op, smsg);
}

void mwScreen::rtextout_centre(ALLEGRO_FONT *f, ALLEGRO_BITMAP *dbmp, int x, int y, int col, float scale, float op, const char *txt)
{
   // draws stretched text
   // used in many places
   float sw = strlen(txt) * 8;

   // if passed -1, do auto scale
   if (scale == -1) scale = (float)mDisplay.SCREEN_W / (sw+16);
   else if (scale == -2) scale = (float)(mDisplay.SCREEN_W / sw) * 0.9;

   ALLEGRO_BITMAP *temp = al_create_bitmap(sw, 8);
   al_set_target_bitmap(temp);
   al_clear_to_color(al_map_rgba(0,0,0,0));

   al_draw_text(f, mColor.pc[col], sw/2, 0, ALLEGRO_ALIGN_CENTRE, txt);

   if (dbmp != NULL) al_set_target_bitmap(dbmp);
   else al_set_target_backbuffer(mDisplay.display);

   al_draw_tinted_scaled_rotated_bitmap(temp, al_map_rgba_f(op, op, op, op), sw/2, 4, x, y, scale, scale, 0, 0);
   al_destroy_bitmap(temp);
}


void mwScreen::draw_framed_text(int xc, int y, int round, ALLEGRO_FONT *f, int fc, int tc, const char* txt)
{
   int bx, by, bw, bh;
   mFont.mw_get_text_dimensions(f, txt, bx, by, bw, bh);

   int xs = bw/2 + 2;
   int x1 = xc-xs;
   int x2 = xc+xs;

   float tfy = y-by+2;
   float fy1 = y+0.5;
   float fy2 = y+bh+3.5;

   al_draw_filled_rectangle(       x1, fy1, x2, fy2, mColor.pc[0]);      // clear background
   al_draw_rounded_rectangle(      x1, fy1, x2, fy2, round, round, mColor.pc[fc], 1); // frame
   al_draw_textf(f, mColor.pc[tc], xc, tfy, ALLEGRO_ALIGN_CENTER, "%s", txt);
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
      color = mLevel.mPlayer.syn[mLevel.mPlayer.active_local_player].color;
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

void mwScreen::draw_percent_bar_line(int cx, int y, int width, int height, int rise, int color, int percent )
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

