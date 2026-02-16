// mwScreenOverlayDemoControls.cpp

#include "pm.h"
#include "mwScreen.h"
#include "mwPlayer.h"
#include "mwDemoMode.h"
#include "mwDisplay.h"
#include "mwFont.h"
#include "mwBitmap.h"
#include "mwWidget.h"
#include "mwColor.h"
#include "mwConfig.h"
#include "mwDrawSequence.h"
#include "mwInput.h"
#include "mwLoop.h"
#include "mwMiscFnx.h"
#include "mwEventQueue.h"
#include "mwGameMoves.h"
#include "mwGmInfo.h"



void mwScreen::draw_demo_controls_overlay_play_stop_button(int xm, int x1, int x2, int ym, int y1, int y2, int r, int pc, int sc)
{
   if (demo_controls_paused)
   {
      if (mWidget.mButton(xm, x1, x2, ym, y1, y2, r,  1,3,1,  pc+192,pc+80,15, 15,0, "Play")) demo_controls_paused = 0;
   }
   else
   {
      if (mWidget.mButton(xm, x1, x2, ym, y1, y2,  r,  1,3,1,  sc+192,sc+80,15, 15,0, "Stop")) demo_controls_paused = 1;
   }
}


void mwScreen::draw_demo_controls_overlay_play_stop_button_small(int xm, int x1, int x2, int ym, int y1, int y2, int r, int pc, int sc)
{
   if (demo_controls_paused)
   {
      if (mWidget.mButtonSmallText(xm, x1, x2, ym, y1, y2, r,  1,3,1,  pc+192,pc+80,15, 15,0, "Play")) demo_controls_paused = 0;
   }
   else
   {
      if (mWidget.mButtonSmallText(xm, x1, x2, ym, y1, y2,  r,  1,3,1,  sc+192,sc+80,15, 15,0, "Stop")) demo_controls_paused = 1;
   }
}


void mwScreen::toggle_demo_controls_small_text()
{
   demo_controls_small_text = !demo_controls_small_text;
   mConfig.save_config();
}


void mwScreen::set_demo_controls_tray_mode(bool val)
{
   demo_controls_tray_mode = val;
   mConfig.save_config();
}

void mwScreen::adj_demo_controls_mode(int adj)
{
   demo_controls_mode+= adj;
   if (demo_controls_mode < 0) demo_controls_mode = 0;
   if (demo_controls_mode > 2) demo_controls_mode = 2;
   mConfig.save_config();
}



int mwScreen::draw_demo_controls_overlay_titlebar(int x1, int y1, int x2, int y2)
{
   const char *txt = "Running Saved Game";

   // division between right hand buttons and the rest of the title bar
   int rx = x2-50;
   if (demo_controls_small_text) rx = x2-36;

   // default frame color
   int fc = 15;

   int mouse_on_bar = 0;
   if ((mInput.mouse_x > x1) && (mInput.mouse_x < rx) && (mInput.mouse_y > y1) && (mInput.mouse_y < y2))
   {
      mouse_on_bar = 1;
      fc = 14;
   }

   if (drag_move_demo_controls_active) fc = 10;

   al_draw_rectangle(x1, y2, x2, y2, mColor.pc[fc], 1);


   if (demo_controls_small_text)
   {
      float tx = x1 + 2;
      float ty = y1 + ((y2-y1) - 13)/2;
      al_draw_text(mFont.pixl, mColor.pc[15], tx, ty, 0, txt);

      int w = 9;
      int s = 8;

      al_draw_line(x2-w, y1, x2-w, y2, mColor.pc[fc], 1);
      if (mWidget.mButtonSmallText(2,s,x2-0,  0,y1,y2,  0,  0,0,3,  0,0,15, 10,0, "-")) set_demo_controls_tray_mode(true);

      al_draw_line(x2-w*2, y1, x2-w*2, y2, mColor.pc[fc], 1);
      if (mWidget.mButtonSmallText(2,s,x2-0-w,  0,y1,y2,  0,  0,0,3,  0,0,15, 10,0, "T")) toggle_demo_controls_small_text();

      al_draw_line(rx, y1, rx, y2, mColor.pc[fc], 1);
      if (mWidget.mButtonSmallText(1,rx+1,s,  0,y1,y2,  0,  0,0,3,  0,0,15, 10,0, "-")) adj_demo_controls_mode(-1);
      al_draw_text(mFont.pixl, mColor.pc[15], rx+8, y1-3, 0, "I");
      if (mWidget.mButtonSmallText(1,rx+9,s,  0,y1,y2,  0,  0,0,3,  0,0,15, 10,0, "+")) adj_demo_controls_mode(1);

   }
   else
   {
      float tx = x1 + 2;
      float ty = y1 + ((y2-y1) - 8)/2;
      al_draw_text(mFont.pr8, mColor.pc[15], tx, ty, 0, txt);

      int w = 12;
      int s = 10;

      al_draw_line(x2-w, y1, x2-w, y2, mColor.pc[fc], 1);
      if (mWidget.mButton(2,s,x2-1,  0,y1,y2,  0,  0,0,3,  0,0,15, 10,0, "-")) set_demo_controls_tray_mode(true);

      al_draw_line(x2-w*2, y1, x2-w*2, y2, mColor.pc[fc], 1);
      if (mWidget.mButton(2,s,x2-1-w,  0,y1,y2,  0,  0,0,3,  0,0,15, 10,0, "t")) toggle_demo_controls_small_text();

      al_draw_line(rx, y1, rx, y2, mColor.pc[fc], 1);
      if (mWidget.mButton(1,rx+1,s,  0,y1,y2,  0,  0,0,3,  0,0,15, 10,0, "-")) adj_demo_controls_mode(-1);
      al_draw_text(mFont.pr8, mColor.pc[15], rx+10, y1+1, 0, "i");
      if (mWidget.mButton(1,rx+16,s,  0,y1,y2,  0,  0,0,3,  0,0,15, 10,0, "+")) adj_demo_controls_mode(1);

   }


   // is mouse on bar?
   if (mouse_on_bar && !drag_move_demo_controls_active)
   {
      if (mInput.mouse_b[1][0]) // is mouse button pressed
      {
         int mxo = mInput.mouse_x - demo_controls_overlay_x;
         int myo = mInput.mouse_y - demo_controls_overlay_y;
         drag_move_demo_controls_active = 1;
         while (mInput.mouse_b[1][0]) // wait for release
         {
            demo_controls_overlay_x = mInput.mouse_x - mxo;
            demo_controls_overlay_y = mInput.mouse_y - myo;
            mEventQueue.proc(1);
            mDrawSequence.ds_draw(0, 1);
         }
         drag_move_demo_controls_active = 0;
         mConfig.save_config();
      }
   }
   return fc;
}


void mwScreen::draw_demo_controls_overlay()
{
   if (demo_controls_tray_mode)
   {
      draw_demo_controls_overlay_bottom_line();
      // expand "+" button in fixed position in lower left border corner
      int ym = mDisplay.SCREEN_H - 11;
      if (mWidget.mButton(1,3,10,  0,ym,ym+10,  1,  0,0,1,  12+192,12+80,15, 15,0, "+")) set_demo_controls_tray_mode(false);
   }
   else
   {
      // check limits to ensure control window is not offscreen
      int xul = mDisplay.SCREEN_W - BORDER_WIDTH - 60; // x upper limit
      int yul = mDisplay.SCREEN_H - BORDER_WIDTH; // y upper limit
      if (demo_controls_overlay_x < 0) demo_controls_overlay_x = 0;
      if (demo_controls_overlay_x > xul) demo_controls_overlay_x = xul;
      if (demo_controls_overlay_y < 0) demo_controls_overlay_y = 0;
      if (demo_controls_overlay_y > yul) demo_controls_overlay_y = yul;

      bool show_transport = true;
      bool show_timeline = true;
      bool show_speed = true;
      bool show_pwd = true;
      bool show_filename = true;
      bool show_elapsed = false;


      if (demo_controls_mode == 0)
      {
         show_pwd = 1;
         show_transport = 1;
         show_timeline = 0;
         show_speed = 0;
         show_filename = 0;
         show_elapsed = 0;
      }

      if (demo_controls_mode == 1)
      {
         show_pwd = 1;
         show_transport = 1;
         show_timeline = 1;
         show_speed = 0;
         show_filename = 0;
         show_elapsed = 0;
      }
      if (demo_controls_mode == 2)
      {
         show_pwd = 1;
         show_transport = 1;
         show_timeline = 1;
         show_speed = 1;
         show_filename = 1;
         show_elapsed = 1;
      }

      // line height
      int bts = 10;
      //  bts += mLoop.pct_x;

      int control_spacing = 1;
      int frame_spacing = 1;

      // child line spacing
      int cls = 0;
      //cls += mLoop.pct_y;


      // frame y values
      int y1 = demo_controls_overlay_y;

      // bottom line of title bar
      int yt = y1+10;

      bool smallText = 0;
      if (demo_controls_small_text) // small text version
      {
         smallText = 1;
         bts = 7;
         yt = y1+7;
      }

      //control_spacing -= mLoop.pct_y;
      //frame_spacing -= mLoop.pct_x;


      // transport control sizes
      int transport_ls = cls;
      int transport_width;
      int transport_height;
      draw_demo_controls_overlay_transport(0, 0, bts, transport_ls, smallText, 1, transport_width, transport_height); // get sizes only

      // timeline control sizes (width is copied from transport width)
      int timeline_ls = cls;
      int timeline_width;
      int timeline_height;
      draw_demo_controls_overlay_timeline_tracks(0, transport_width, 0, bts, timeline_ls, smallText, 1, timeline_width, timeline_height); // get sizes only

      // speed control sizes
      int speed_ls = cls;
      int speed_width;
      int speed_height;
      draw_demo_controls_overlay_speed(0, 0, bts, speed_ls, smallText, 1, speed_width, speed_height); // get sizes only
      int speed_xa = (transport_width - speed_width)/2; // center speed controls

      // text line sizes
      int text_ls = cls;
      int text_height = bts + cls*2;

      // frame x values (fixed, based on transport width)
      int x1 = demo_controls_overlay_x;
      int w = transport_width + frame_spacing * 2;
      int x2 = x1 + w;


      // frame y values
      int h = yt-y1; // titlebar height
      h += frame_spacing * 2;
      if (show_transport) h+= transport_height + control_spacing;
      if (show_timeline)  h+= timeline_height  + control_spacing;
      if (show_speed)     h+= speed_height     + control_spacing;
      if (show_pwd)       h+= text_height      + control_spacing;
      if (show_filename)  h+= text_height      + control_spacing;
      if (show_elapsed)   h+= text_height      + control_spacing;

      // there is always one less control_spacing than controls
      h-=control_spacing;

      int y2 = y1 + h;

      // clear background
      al_draw_filled_rectangle(x1, y1, x2, y2, mColor.pc[0]);

      // inner x and y starting position
      int xa = x1 + frame_spacing;
      int ya = yt + frame_spacing;

      if (show_filename)
      {
         std::stringstream ss;
         //ss << mGameMoves.last_loaded_gm_filename; // full path

         // filename only
         ALLEGRO_PATH *path = al_create_path(mGameMoves.last_loaded_gm_filename);
         const char* fname = al_get_path_filename(path);
         ss << "file:" << fname;
         //ss << fname;

         draw_demo_controls_overlay_textline(xa, xa+transport_width, ya, bts, text_ls, 15, 1, smallText, ss.str().c_str());
         ya += text_height+control_spacing;
      }

      if (show_elapsed)
      {
         int xb = xa+transport_width;
         int xc = (xa+xb)/2;
         std::stringstream ss1;
         float pc1 = mLoop.frame_num * 100 / mGmInfo.lastFrame;
         ss1 << "time: [" << pc1 << "%] "<< mLoop.frame_num << "/" << mGmInfo.lastFrame;
         draw_demo_controls_overlay_textline(xa, xc-1, ya, bts, text_ls, 15, 2, smallText, ss1.str().c_str());

         std::stringstream ss2;
         float pc2 = (mGameMoves.current_pos * 100 / mGameMoves.entry_pos);
         ss2 << "moves: [" << pc2 << "%] "<< mGameMoves.current_pos << "/" << mGameMoves.entry_pos;
         draw_demo_controls_overlay_textline(xc, xb, ya, bts, text_ls, 15, 2, smallText, ss2.str().c_str());
         ya += text_height+control_spacing;
      }
      if (show_pwd)
      {
         draw_demo_controls_overlay_pwd_nrd(xa, xa+transport_width, ya, bts, text_ls, smallText);
         ya += text_height+control_spacing;
      }
      if (show_transport)
      {
         draw_demo_controls_overlay_transport(xa, ya, bts, transport_ls, smallText, 0, transport_width, transport_height);
         ya += (transport_height+control_spacing); // update running y position
      }
      if (show_speed)
      {
         draw_demo_controls_overlay_speed(xa+speed_xa, ya, bts, speed_ls, smallText, 0, speed_width, speed_height);
         ya += (speed_height+control_spacing); // update running y position
      }
      if (show_timeline)
      {
         draw_demo_controls_overlay_timeline_tracks(xa, xa+transport_width, ya, bts, timeline_ls, smallText, 0, timeline_width, timeline_height);
         ya += (timeline_height+control_spacing); // update running y position
      }

      // get frame color after drawing titlebar
      int fc = draw_demo_controls_overlay_titlebar(x1, y1, x2, yt);

      // draw frame last
      al_draw_rectangle(x1, y1, x2, y2, mColor.pc[fc], 1);
   }
}


// button helper
bool bh(int &rx, int y1, int y2, int ls, int bc, const char* txt, bool smallText)
{
   int w = (strlen(txt)+1)*8;
   if (smallText) w = (strlen(txt)+1)*4;

   int x1 = rx;
   int x2 = rx + w;
   rx += w + ls;
   if (!smallText && mWidget.mButton(0, x1, x2,  0,y1,y2,  0,  1,3,1,  bc+192,bc+80,15, 15,0, txt)) return true;
   if (smallText && mWidget.mButtonSmallText(0, x1, x2,  0,y1,y2,  0,  1,3,1,  bc+192,bc+80,15, 15,0, txt)) return true;
   return false;
}


void mwScreen::draw_demo_controls_overlay_textline(int fx1, int fx2, int fy1, int bts, int ls, int tc, int fc, bool smallText, const char * txt)
{
   int fy2 = fy1 + bts + ls*2;

   // clear and draw frame
   al_draw_filled_rectangle(fx1, fy1, fx2, fy2, mColor.pc[0]);
   al_draw_rectangle(       fx1, fy1, fx2, fy2, mColor.pc[fc], 1);

   // common y1 and y2 for all buttons
   int y1 = fy1 + ls;
   int y2 = y1 + bts;

   if (smallText)
   {
      // get centered text position
      float tx = fx1 + 2;
      float ty = y1 + ((y2-y1) - 13)/2;
      al_draw_text(mFont.pixl, mColor.pc[tc], tx, ty, 0, txt);
   }
   else
   {
      // get centered text position
      float tx = fx1 + 2;
      float ty = y1 + ((y2-y1) - 8)/2;
      al_draw_text(mFont.pr8, mColor.pc[tc], tx, ty, 0, txt);
   }
}

void mwScreen::draw_demo_controls_overlay_pwd_nrd(int fx1, int fx2, int fy1, int bts, int ls, bool smallText)
{
   int y1 = fy1 + ls;
   int y2 = y1 + bts;

   int pwd_col = 6;
   int nrd_col = 12;

   if (smallText)
   {
      mWidget.mCheckBoxSmallText(1,fx1, 78,  0,y1,y2, pwd_col, demo_controls_pause_when_done, "Pause when done", 15, pwd_col);
      if (mDemoMode.play_mode == 2 && mWidget.mButtonSmallText(2,70,fx2,  0,y1,y2,  0,  1,3,3,  nrd_col+192,nrd_col+96,15, 15,0, "Next Random Demo")) mDemoMode.continuous_random_next_level();
   }
   else
   {
      mWidget.mCheckBox(1,fx1, 138,  0,y1,y2, pwd_col, demo_controls_pause_when_done, "Pause when done", 15, pwd_col);
      if (mDemoMode.play_mode == 2 && mWidget.mButton(2,136,fx2,  0,y1,y2,  0,  1,3,3,  nrd_col+192,nrd_col+96,15, 15,0, "Next Random Demo")) mDemoMode.continuous_random_next_level();
   }
}



void mwScreen::draw_demo_controls_overlay_transport(int fx1, int fy1, int bts, int ls, bool smallText, bool sizeOnly, int &w, int &h)
{
   w = (smallText ? 214 : 418) + ls*2;
   h = bts+ls*2;
   if (sizeOnly) return;

   int fx2 = fx1 + w;
   int fy2 = fy1 + h;

   // clear and draw frame
   al_draw_filled_rectangle(fx1, fy1, fx2, fy2, mColor.pc[0]);
   al_draw_rectangle(       fx1, fy1, fx2, fy2, mColor.pc[15], 1);

   // common y1 and y2 for all buttons
   int y1 = fy1 + ls;
   int y2 = y1 + bts;

   int bc = 12; // button color

   // get current and last frame
   int f = mLoop.frame_num;
   int lf = mGmInfo.lastFrame;

   // get initial f to determine if this function modifies it
   int initial_f = f;

   // running x position
   int rx = fx1+ls;

   int s = 1;

   if (bh(rx, y1, y2, s, bc, "start", smallText)) f  = 0;
   if (bh(rx, y1, y2, s, bc, "-60s",  smallText)) f -= 2400;
   if (bh(rx, y1, y2, s, bc, "-10s",  smallText)) f -= 400;
   if (bh(rx, y1, y2, s, bc, "-1s",   smallText)) f -= 40;
   if (bh(rx, y1, y2, s, bc, "-1f",   smallText)) f -= 1;

   if (smallText)
   {
      draw_demo_controls_overlay_play_stop_button_small(0,rx,rx+20,  0,y1,y2,  0,  11, 4);
      rx+=20+s;
   }
   else
   {
      draw_demo_controls_overlay_play_stop_button(0,rx,rx+40,  0,y1,y2,  0,  11, 4);
      rx+=40+s;
   }

   if (bh(rx, y1, y2, s, bc, "+1f",  smallText)) f += 1;
   if (bh(rx, y1, y2, s, bc, "+1s",  smallText)) f += 40;
   if (bh(rx, y1, y2, s, bc, "+10s", smallText)) f += 400;
   if (bh(rx, y1, y2, s, bc, "+60s", smallText)) f += 2400;
   if (bh(rx, y1, y2, s, bc, "end",  smallText)) f  = lf;

   // changed by this function
   if (f != initial_f)
   {
      // enforce limits
      if (f < 0) f = 0;
      if (f > lf) f = lf;

      mLoop.frame_num = f;
      mDemoMode.seek_to_frame(f, 1);
   }
}

// speed adjust helper
void sh(bool inc_dec, float per)
{
   float adj = mLoop.frame_speed * per;
   if (adj < 1) adj = 1;

   if (inc_dec) // inc
   {
      mLoop.frame_speed += adj;
      if (mLoop.frame_speed > 200) mLoop.frame_speed = 200;
   }
   else // dec
   {
      mLoop.frame_speed -= adj;
      if (mLoop.frame_speed < 4) mLoop.frame_speed = 4;
   }
}


void mwScreen::draw_demo_controls_overlay_speed(int fx1, int fy1, int bts, int ls, bool smallText, bool sizeOnly, int &w, int &h)
{
   w = (smallText ? 178 : 350) + ls*2;
   h = bts + ls*2;
   if (sizeOnly) return;

   // frame x2 and y2
   int fx2 = fx1 + w;
   int fy2 = fy1 + h;

   // clear and draw frame
   al_draw_filled_rectangle(fx1, fy1, fx2, fy2, mColor.pc[0]);
   al_draw_rectangle(       fx1, fy1, fx2, fy2, mColor.pc[15], 1);

   // common y1 and y2 for all buttons
   int y1 = fy1 + ls;
   int y2 = fy2 - ls;

   int bc = 13; // button color

   // get initial frame_speed to determine if this function modifies it
   int old_frame_speed = mLoop.frame_speed;

   // running x position
   int rx = fx1+ls;

   if (bh(rx, y1, y2, 1, bc, "min"  , smallText)) mLoop.frame_speed = 4;
   if (bh(rx, y1, y2, 1, bc, "-50%" , smallText)) sh(0, .5);
   if (bh(rx, y1, y2, 1, bc, "-10%" , smallText)) sh(0, .1);

   // display button in the middle with click to reset to default
   char msg[80];
   sprintf(msg, "Frame Speed:%d", mLoop.frame_speed);
   if (bh(rx, y1, y2, 1, bc, msg, smallText)) mLoop.frame_speed = 40;

   if (bh(rx, y1, y2, 1, bc, "+50%", smallText)) sh(1, .5);
   if (bh(rx, y1, y2, 1, bc, "+10%", smallText)) sh(1, .1);
   if (bh(rx, y1, y2, 1, bc, "max" , smallText)) mLoop.frame_speed = 200;

   if (old_frame_speed != mLoop.frame_speed) mEventQueue.adjust_fps_timer(mLoop.frame_speed);
}


void mwScreen::draw_demo_controls_overlay_timeline_tracks(int x1, int x2, int y1, int bts, int ls, bool smallText, bool sizeOnly, int &w, int &h)
{
   int baseFrameColor = 15;

   // height increase for active track
   int activeTrackHeightAdjust = 4;

   int size = mGmInfo.gmPlayerInfo.size();

   // vertical spacing between tracks
   int vs = ls;
   // can't be zero or tracks will overwrite each other
   if (vs == 0) vs = 1;

   w = x2-x1;
   h = (bts+ls)*size + ls + activeTrackHeightAdjust;
   if (ls == 0) h+= (size-1);

   if (sizeOnly) return;

   int y2 = y1 + h;

   al_draw_filled_rectangle(x1, y1, x2, y2, mColor.pc[0]);
   al_draw_rectangle(       x1, y1, x2, y2, mColor.pc[baseFrameColor], 1);

   mColor.process_flash_color();

   // get start frame and last frame
   float sf = 0;
   float lf = mGmInfo.lastFrame;

   // starting y position
   float ry1 = y1 + ls;

   // full x range of inner rect
   float ix1 = x1 + ls;
   float ix2 = x2 - ls;

   int mouse_on_bar = -1;

   for (auto& r : mGmInfo.gmPlayerInfo)
   {
      int p = r.playerNum;
      int c = r.playerCol;

      int frameColor = c;

      // set ry1
      float ry2 = ry1 + bts;

      if (p == mPlayer.active_local_player)
      {
         ry2 += activeTrackHeightAdjust;
         frameColor = baseFrameColor;
      }

      // map track start and end to screen positions
      float rx1 = map_range((float)r.startFrame, sf, lf,  ix1, ix2);
      float rx2 = map_range((float)r.endFrame,   sf, lf,  ix1, ix2);

      // is mouse on bar?
      if ((mInput.mouse_x > rx1) && (mInput.mouse_x < rx2) && (mInput.mouse_y > ry1) && (mInput.mouse_y < ry2))
      {
         mouse_on_bar = p;
         frameColor = mColor.flash_color;
      }

      // fill the background
      al_draw_filled_rectangle(rx1, ry1, rx2, ry2, mColor.pc[c+144]);

      // frame
      al_draw_rectangle(rx1, ry1, rx2, ry2, mColor.pc[frameColor], 1);

      // highlight for active player
      if (p == mPlayer.active_local_player) al_draw_rectangle(rx1+1, ry1+1, rx2-1, ry2-1, mColor.pc[frameColor], 1);

      // tile position
      int ty1 = ry1 + 1;
      int ty2 = ry2 - 1;
      int th = ty2 - ty1;
      int tx1 = rx1 + 1;
      int tx2 = tx1 + th;

      // erase tile background
      al_draw_filled_rectangle(tx1, ty1, tx2, ty2, mColor.pc[c+192]);

      // draw tile rhs border line
      al_draw_line(tx2+1, ry1, tx2+1, ty2+1, mColor.pc[frameColor], 1);

      // draw tile rhs border line
      if (p == mPlayer.active_local_player) al_draw_line(tx2, ry1, tx2, ty2, mColor.pc[frameColor], 1);

      // draw tile
      al_draw_scaled_bitmap(mBitmap.player_tile[c][1], 0, 0, 20, 20, tx1, ty1, th, th, 0);

      
      // draw vertical line at deaths
      for (auto& d : r.deaths)
      {
         float dx = map_range((float)d, sf, lf,  ix1, ix2);
         al_draw_line(dx, ry1, dx, ry2, mColor.pc[10], 1);
      }

      // draw vertical line at purple coins
      for (auto& d : r.purpleCoins)
      {
         float dx = map_range((float)d, sf, lf,  ix1, ix2);
         al_draw_line(dx, ry1, dx, ry2, mColor.pc[8], 1);
      }

      if (smallText)
      {
         float txtx = tx2 + 4;
         float txty = ry1 + ((ry2-ry1) - 13)/2;
         al_draw_textf(mFont.pixl, mColor.pc[15], txtx, txty, 0, "%s", r.playerName.c_str());
      }
      else
      {
         float txtx = tx2 + 4;
         float txty = ry1 + ((ry2-ry1) - 8)/2;
         al_draw_textf(mFont.pr8, mColor.pc[15], txtx, txty, 0, "%s", r.playerName.c_str());
      }
      // next bar position
      ry1 = ry2+vs; 
   }

   // don't let currentFrame exceed lastFrame
   int currentFrame = mLoop.frame_num;
   if (currentFrame > mGmInfo.lastFrame) currentFrame = mGmInfo.lastFrame;

   // draw vertical line at current frame
   float fx = map_range((float)currentFrame, sf, lf,  ix1, ix2);
   al_draw_line(fx, y1, fx, y2, mColor.pc[15], 1);

   // is mouse on frame?
   if ((mInput.mouse_x > x1) && (mInput.mouse_x < x2) && (mInput.mouse_y > y1) && (mInput.mouse_y < y2))
   {
      // draw vertical line at mouse pos frame
      float fx = (float) mInput.mouse_x;
      al_draw_line(fx, y1, fx, y2, mColor.pc[14], 1);

      // is mouse button pressed?
      if (mInput.mouse_b[1][0])
      {
         while (mInput.mouse_b[1][0]) mEventQueue.proc(1); // wait for release

         // map screen x position to frame num
         float f = map_range( (float) mInput.mouse_x, ix1, ix2, sf, lf);

         // set frame num and seek to frame
         mLoop.frame_num = f;
         mDemoMode.seek_to_frame(f, 1);

         // if mouse_on_bar set active local player to that bar
         if (mouse_on_bar != -1) mDemoMode.set_active_local_player(mouse_on_bar);
      }
   }
}

void mwScreen::draw_demo_controls_overlay_bottom_line()
{
   int s = 6;
  // s += mLoop.pct_x;


   // running x position
   int x = BORDER_WIDTH+2;

   int y2 = mDisplay.SCREEN_H - 1;
   int h = 10;

   int ppc_length = 40;
   int pc = 11; // play color
   int sc = 4;  // stop color

   draw_demo_controls_overlay_play_stop_button(0,x,x+ppc_length,  2,h,y2,  1,  pc, sc);
   x += ppc_length + s; // update running position

   int nrd_length = 40;
   //  nrd_length += mLoop.pct_x;
   if (mDemoMode.play_mode == 2)
   {
      if (mWidget.mButton(0,x,x+nrd_length,  2,h,y2,  1,  1,3,1,  12+192,12+80,15, 15,0, "Next")) mDemoMode.continuous_random_next_level();
      x += nrd_length + s; // update running position
   }


   int tly = mDisplay.SCREEN_H - 11;
   int w = draw_demo_controls_overlay_players_small(x, tly);

   // if width is zero do not add spacing
   if (w) x+= w+s;


   draw_demo_controls_overlay_timeline(x, tly, mDisplay.SCREEN_W-BORDER_WIDTH - 60, tly+10, 9+160);

}


int mwScreen::draw_demo_controls_overlay_players_small(int x, int y)
{
   int line_spacing = 0;

   int size = 9;
   // size += mLoop.pct_y;

   int rx = x; // running x position

   // count current number of players
   int num_players = 0;
   for (int p=0; p<NUM_PLAYERS; p++)
      if (mPlayer.is_player_active_and_not_hidden(p)) num_players++;

   if (num_players > 1)
   {
      for (int p=0; p<NUM_PLAYERS; p++)
      {
         if (mPlayer.is_player_active_and_not_hidden(p))
         {
            int highlight = 0;
            if (p == mPlayer.active_local_player) highlight = 1;
            if (mWidget.mButtonPlayerTile(rx, y, size, mPlayer.syn[p].color, 0, 15, highlight)) mDemoMode.set_active_local_player(p);
            rx += size + line_spacing;
         }
      }
   }
   return rx-x;
}

void mwScreen::draw_demo_controls_overlay_timeline(int x1, int y1, int x2, int y2, int col)
{
   // clear background and draw frame
   al_draw_filled_rectangle(x1, y1, x2, y2, mColor.pc[col]);
   al_draw_rectangle(       x1, y1, x2, y2, mColor.pc[15], 1);

   // get start frame and last frame
   float sf = 0;
   float lf = mGmInfo.lastFrame;

   // don't let currentFrame exceed lastFrame
   int currentFrame = mLoop.frame_num;
   if (currentFrame > mGmInfo.lastFrame) currentFrame = mGmInfo.lastFrame;

   // get nicely formatted time string
   char msg[80];
   //sprintf(msg, "Frame:%d", currentFrame);
   sprintf(msg, "%s", mMiscFnx.chrms(currentFrame, msg));

   // get centered text position
   float tx = (x1+x2)/2;
   float ty = y1 + ((y2-y1) - 8)/2;
   al_draw_text(mFont.pr8, mColor.pc[15], tx, ty, ALLEGRO_ALIGN_CENTRE, msg);


   // draw vertical line at current frame
   float fx = map_range((float)currentFrame, sf, lf,  (float)x1, (float)x2);
   al_draw_line(fx, y1, fx, y2, mColor.pc[15], 1);

   // is mouse on frame?
   if ((mInput.mouse_x > x1) && (mInput.mouse_x < x2) && (mInput.mouse_y > y1) && (mInput.mouse_y < y2))
   {
      // draw vertical line at mouse pos frame
      float fx = (float) mInput.mouse_x;
      al_draw_line(fx, y1, fx, y2, mColor.pc[14], 1);

      // is mouse button pressed?
      if (mInput.mouse_b[1][0])
      {
         while (mInput.mouse_b[1][0]) mEventQueue.proc(1); // wait for release

         // map screen x position to frame num
         float f = map_range( (float) mInput.mouse_x, (float)x1, (float)x2, sf, lf);

         // set frame num and seek to frame
         mLoop.frame_num = f;
         mDemoMode.seek_to_frame(f, 1);
      }
   }
}

