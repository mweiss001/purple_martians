// mwDemoRecord.cpp
#include "pm.h"
#include "mwBitmap.h"
#include "mwColor.h"
#include "mwDemoMode.h"
#include "mwDemoRecord.h"
#include "mwEventQueue.h"
#include "mwFont.h"
#include "mwGameMoves.h"
#include "mwGmInfo.h"
#include "mwInput.h"
#include "mwLoop.h"
#include "mwMenu.h"
#include "mwMiscFnx.h"
#include "mwPlayer.h"
#include "mwScreen.h"
#include "mwWidget.h"

void mwDemoRecord::draw_timeline(mwWindow w)
{
   int d = w.disable_input;
   int ya = w.rect.y1 + 1;
   int tbx = w.rect.x2-60;
   int cb_spacing = 30;

   mWidget.mCheckBoxWithToolTip(1, tbx, 10, 1, ya, 10, 0, timeline_level_done_color,    "l", 15, 15, d, "level done");
   tbx-=cb_spacing;
   mWidget.mCheckBoxWithToolTip(1, tbx, 10, 1, ya, 10, 0, timeline_show_player_icons,   "i", 15, 15, d, "icons");
   tbx-=cb_spacing;
   mWidget.mCheckBoxWithToolTip(1, tbx, 10, 1, ya, 10, 0, timeline_purple_coins_color,  "p", 15, 15, d, "purple coins");
   tbx-=cb_spacing;
   mWidget.mCheckBoxWithToolTip(1, tbx, 10, 1, ya, 10, 0, timeline_enemy_hit_color,     "h", 15, 15, d, "hits");
   tbx-=cb_spacing;
   mWidget.mCheckBoxWithToolTip(1, tbx, 10, 1, ya, 10, 0, timeline_player_deaths_color, "d", 15, 15, d, "deaths");
   tbx-=cb_spacing;
   mWidget.mCheckBoxWithToolTip(1, tbx, 10, 1, ya, 10, 0, time_format,                  "t", 15, 15, d, "time format");

   if (timeline_purple_coins_color) timeline_purple_coins_color = 8;
   if (timeline_enemy_hit_color) timeline_enemy_hit_color = 14;
   if (timeline_player_deaths_color) timeline_player_deaths_color = 10;
   if (timeline_level_done_color) timeline_level_done_color = 11;

   // make a smaller rect for timeline widget (below title bar, and leave space for adjustment in br corner)
   mwRect<int> timeline_rect = mwRect<int>::fromX1Y1X2Y2(w.rect.x1+2, w.rect.y1+14, w.rect.x2-4, w.rect.y2 );

   // determine track height from window height and number of tracks
   float activeTrackHeightAdjust = 4;
   float track_spacing = 2;
   float height = timeline_rect.h - activeTrackHeightAdjust - track_spacing*2;
   float num_tracks = mGmInfo.gmPlayerInfo.size();
   float lh = height / num_tracks; // line height
   float bts = lh - track_spacing;

   int gmInfo_index = -1;
   if (draw_timeline_tracks(timeline_rect.x1, timeline_rect.x2, timeline_rect.y1, bts, track_spacing, 0, 0, timeline_rect.w, timeline_rect.h, gmInfo_index, w.disable_input)) set_active_section(gmInfo_index);
}


bool mwDemoRecord::draw_timeline_tracks(int x1, int x2, int y1, float bts, float ls, bool smallText, bool sizeOnly, int &w, int &h, int &gmInfo_index, int display_only)
{



   int baseFrameColor = 15;

   // height increase for active track
   int activeTrackHeightAdjust = 4;

   int size = mGmInfo.gmPlayerInfo.size();

   // vertical spacing between tracks
   float vs = ls;
   // can't be zero or tracks will overwrite each other
   if (vs == 0) vs = 1;


   w = x2-x1;
   h = (bts+ls)*size + ls + activeTrackHeightAdjust;
   if (ls == 0) h+= (size-1);

   if (sizeOnly) return false;

   int y2 = y1 + h;

   al_draw_filled_rectangle(x1, y1, x2, y2, mColor.pc[0]);
   al_draw_rectangle(       x1, y1, x2, y2, mColor.pc[baseFrameColor], 1);

   mColor.process_flash_color();

   // get start frame and last frame
   float sf = 0;
   float lf = timeline_last_frame;


   // starting y position
   float ry1 = y1 + ls;

   // full x range of inner rect
   float ix1 = x1 + ls;
   float ix2 = x2 - ls;

   int mouse_on_bar = -1;



   for (int i = 0; i < (int) mGmInfo.gmPlayerInfo.size(); i++)
   {
      struct gmPlayerInfoRecord r = mGmInfo.gmPlayerInfo[i];


      int p = r.playerNum;
      int c = r.playerCol;

      int frameColor = c;

      //      int highlight_color = mColor.flash_color;

      int highlight_color = 15;


      // set ry1
      float ry2 = ry1 + bts;

      if (p == mPlayer.active_local_player)
      {
         ry2 += activeTrackHeightAdjust;
         frameColor = baseFrameColor;
      }


      // get track start and end pos as float
      float rsf = r.startFrame;
      float ref = r.endFrame;

      // if end pos is not fixed amd less than lf, make it lf
      if (r.noEnd) ref = lf;

      // map track start and end to screen positions
      float rx1 = mMiscFnx.map_range<float>(rsf, sf, lf,  ix1, ix2);
      float rx2 = mMiscFnx.map_range<float>(ref, sf, lf,  ix1, ix2);


      // make rectangle for bar
      mwRect<int> bar = mwRect<int>::fromX1Y1X2Y2(rx1, ry1, rx2, ry2);

      // is mouse on bar?
      if (!display_only && bar.contains(mInput.mouse_x, mInput.mouse_y))
      {
         mouse_on_bar = p;
         gmInfo_index = i;
         frameColor = highlight_color;
      }

      // fill the background
      bar.draw_filled_rectangle(mColor.pc[c+144]);



      // frame
      bar.draw_rectangle(mColor.pc[frameColor], 1);
      if (p == mPlayer.active_local_player) bar.draw_rectangle(mColor.pc[frameColor], 1, -1);

      // if noEnd, erase rhs frame line
      if (r.noEnd)
      {
         if (p == mPlayer.active_local_player) al_draw_filled_rectangle(bar.x2-1.5, bar.y1+1.5, bar.x2+0.5, bar.y2-1.5, mColor.pc[c+144]);
         else al_draw_filled_rectangle(bar.x2-1, bar.y1+0.5, bar.x2+0.5, bar.y2-0.5, mColor.pc[c+144]);
      }

      // notch area with no game moves

      float lmfx = mMiscFnx.map_range<float>((float)r.lastMoveFrame, sf, lf,  ix1, ix2);

      mwRect<int> ngR = mwRect<int>::fromX1Y1X2Y2(lmfx, bar.y1+4, bar.x2, bar.y2-4);

      ngR.draw_filled_rectangle(mColor.pc[c+192]);


      // show record armed area on track
      if ((p == mPlayer.active_local_player) && (record_punch_in_armed))
      {
         // translate mLoop.frame_num and mGmInfo.lastFrame to screen positions
         float fx1 = mMiscFnx.map_range<float>((float)mLoop.frame_num, sf, lf,  ix1, ix2);
         float fx2 = mMiscFnx.map_range<float>((float)mGmInfo.lastFrame, sf, lf,  ix1, ix2);

         // draw hatched rectangle
         mMiscFnx.rectangle_with_diagonal_lines(fx1, bar.y1, fx2, bar.y2, 8, 10, 10+128, 1);

         // text box
         mwRect<int> ratR = mwRect<int>::fromX1Y1WH(fx1+4, bar.y1+3, 52, 8);
         ratR.draw_filled_rectangle(mColor.Black);
         ratR.draw_rectangle(mColor.Red, 0);
         al_draw_text(mFont.pixl, mColor.pc[10], fx1+5, bar.y1, 0, "Record Armed");

      }


      int rx = bar.x1+2; // running x pos, in case we don't draw tile


      if (timeline_show_player_icons)
      {
         // make rect for tile
         mwRect<int> tile = mwRect<int>::fromX1Y1X2Y2(bar.x1+1, bar.y1+1, bar.x1+bar.h-2, bar.y2-1);

         rx += tile.w;

         // erase tile background
         tile.draw_filled_rectangle(mColor.pc[c+192]);

         // draw tile rhs border line
         al_draw_line(tile.x2+1, bar.y1, tile.x2+1, bar.y2, mColor.pc[frameColor], 1);

         // draw tile rhs border line
         if (p == mPlayer.active_local_player) al_draw_line(tile.x2, bar.y1, tile.x2, bar.y2, mColor.pc[frameColor], 1);

         // draw tile
         al_draw_scaled_bitmap(mBitmap.player_tile[c][1], 0, 0, 20, 20, tile.x1, tile.y1, tile.h, tile.w, 0);
      }



      // make rect for text
      mwRect<int> text = mwRect<int>::fromX1Y1X2Y2(rx, bar.y1+1, bar.x1+bar.h-2, bar.y2-1);


      // if in demo record mode draw text externally
      if (mLoop.state[1] == PM_PROGRAM_STATE_DEMO_RECORD) draw_timeline_section_text(text, i);
      else
      {
         if (smallText)
         {
            float txtx = rx + 4;
            float txty = ry1 + ((ry2-ry1) - 13)/2;
            al_draw_textf(mFont.pixl, mColor.pc[15], txtx, txty, 0, "%s", r.playerName.c_str());
         }
         else
         {
            float txtx = rx + 4;
            float txty = ry1 + ((ry2-ry1) - 8)/2;
            al_draw_textf(mFont.pr8, mColor.pc[15], txtx, txty, 0, "%s", r.playerName.c_str());
         }
      }

      if (timeline_player_deaths_color)
      {
         // draw vertical line at deaths
         for (auto& d : r.playerDeaths)
         {
            float dx = mMiscFnx.map_range<float>((float)d, sf, lf,  ix1, ix2);
            al_draw_line(dx, ry1, dx, ry2, mColor.pc[timeline_player_deaths_color], 1);
         }
      }

      if (timeline_purple_coins_color)
      {
         // draw vertical line at purple coins
         for (auto& d : r.coinsCollected)
         {
            float dx = mMiscFnx.map_range<float>((float)d, sf, lf,  ix1, ix2);
            al_draw_line(dx, ry1, dx, ry2, mColor.pc[timeline_purple_coins_color], 1);
         }
      }

      if (timeline_enemy_hit_color)
      {
         for (auto& d : r.enemyHits)
         {
            float dx = mMiscFnx.map_range<float>((float)d.frame, sf, lf,  ix1, ix2);
            int color = 10; // kill by default
            if (d.hitsLeft) color = timeline_enemy_hit_color; // hit
            al_draw_line(dx, ry1, dx, ry2, mColor.pc[color], 1);
         }
      }

      // next bar position
      ry1 = ry2+vs;
   }

   // don't let currentFrame exceed lastFrame
   int currentFrame = mLoop.frame_num;
//   if (currentFrame > mGmInfo.lastFrame) currentFrame = mGmInfo.lastFrame;


   // draw vertical line at current frame
   float fx = mMiscFnx.map_range<float>((float)currentFrame, sf, lf,  ix1, ix2);
   al_draw_line(fx, y1, fx, y2, mColor.pc[15], 1);

   // add time text label below
   draw_timeline_time_box(fx, y1, y2, timeline_display_time_labels, currentFrame, 15);





   if ((timeline_level_done_color) && (mGmInfo.levelDoneFrame != -1))
   {
      // draw vertical line at Level done
      fx = mMiscFnx.map_range<float>((float) mGmInfo.levelDoneFrame, sf, lf,  ix1, ix2);
      al_draw_line(fx, y1, fx, y2, mColor.pc[timeline_level_done_color], 1);

      // add time text label below
      draw_timeline_time_box(fx, y1, y2, timeline_display_time_labels, mGmInfo.levelDoneFrame, timeline_level_done_color);
   }


   // is mouse on frame?
   if ((mInput.mouse_x > x1) && (mInput.mouse_x < x2) && (mInput.mouse_y > y1) && (mInput.mouse_y < y2) && (!display_only))
   {
      // get float version of mouse x
      fx = (float) mInput.mouse_x;

      // map mouse screen x position to frame num
      float f = mMiscFnx.map_range<float>( fx, ix1, ix2, sf, lf);

      // if in demo record mode process right click menu
      if (mLoop.state[1] == PM_PROGRAM_STATE_DEMO_RECORD) proc_timeline_context_menu(gmInfo_index, f);


      if (timeline_mouse_pos_color)
      {
         // draw vertical line at mouse pos frame
         al_draw_line(fx, y1, fx, y2, mColor.pc[timeline_mouse_pos_color], 1);

         // add text label below indicator
         draw_timeline_time_box(fx, y1, y2, timeline_display_time_labels, f, timeline_mouse_pos_color);

      }




      // is mouse button pressed?
      if (mInput.mouse_b[1][0])
      {
         // wait for release
         while (mInput.mouse_b[1][0]) mEventQueue.proc(1);

         // seek to frame
         mDemoMode.seek_to_frame(f, 1);

         // if mouse_on_bar set active local player to that bar
         if (mouse_on_bar != -1) mDemoMode.set_active_local_player(mouse_on_bar);

         return true;
      }
   }
   return false;
}






void mwDemoRecord::draw_timeline_section_text(mwRect<int> rect, int gmInfo_index)
{
   //rect.draw_filled_rectangle(mColor.Green);

   int i = gmInfo_index;
   int p   = mGmInfo.gmPlayerInfo[i].playerNum;
   //int col = mGmInfo.gmPlayerInfo[i].playerCol;
   int f1  = mGmInfo.gmPlayerInfo[i].startFrame;
   int f2  = mGmInfo.gmPlayerInfo[i].endFrame;

   std::string name = mGmInfo.gmPlayerInfo[i].playerName;

   char text_range[256];
   char m1[64], m2[64];
   sprintf(text_range, "Range:%s to %s", gettf(f1, m1), gettf(f2, m2));

   char text_deaths[256];
   sprintf(text_deaths, "Deaths:%d", (int)mGmInfo.gmPlayerInfo[i].playerDeaths.size());

   char text_coins[256];
   sprintf(text_coins, "Purple Coins:%d", (int)mGmInfo.gmPlayerInfo[i].coinsCollected.size());

   char text_lmf[256];
   sprintf(text_lmf, "last move:%d", mGmInfo.gmPlayerInfo[i].lastMoveFrame);

   char text_ne[256];
   sprintf(text_ne, "no end:%d", mGmInfo.gmPlayerInfo[i].noEnd);



   char text_index[256];
   sprintf(text_index, "I:%d", i);

   char text_player_num[256];
   sprintf(text_player_num, "P%d", p);

   char text_player_name[256];
   sprintf(text_player_name, "%s", name.c_str());



   if (0) // one line
   {
      float txtx = rect.x1 + 2;
      float txty = rect.y1 + (rect.h - 8) / 2;
      char msg[256];
      //   sprintf(msg, "%d - P%d - [%s]", i, p, name.c_str());

      sprintf(msg, "%d - P%d - [%s] s:%d e:%d", i, p, name.c_str(), f1, f2);

      al_draw_text(mFont.pr8, mColor.pc[15], txtx, txty, 0, msg);
   }

   if (0) // two lines
   {
      float txtx = rect.x1 + 2;

      float txtys = (rect.h - 16) / 3;

      float txty1 = rect.y1 + txtys;
      float txty2 = txty1 + txtys + 8;

      char msg[256];
      sprintf(msg, "%s - %s - [%s]" , text_index, text_player_num, text_player_name);
      al_draw_text(mFont.pr8, mColor.pc[15], txtx, txty1, 0, msg);

      sprintf(msg, "%s - %s - %s" , text_range, text_deaths, text_coins);
      al_draw_text(mFont.pr8, mColor.pc[15], txtx, txty2, 0, msg);

   }

   if (0) // two lines small text
   {
      float txtx = rect.x1 + 2;

      float txtys = (rect.h - 10) / 3;

      float txty1 = rect.y1 + txtys - 4;
      float txty2 = txty1 + txtys + 5;

      char msg[256];
      sprintf(msg, "%s - %s - [%s]" , text_index, text_player_num, text_player_name);
      al_draw_text(mFont.pixl, mColor.pc[15], txtx, txty1, 0, msg);

      sprintf(msg, "%s - %s - %s" , text_range, text_deaths, text_coins);
      al_draw_text(mFont.pixl, mColor.pc[15], txtx, txty2, 0, msg);
   }


   if (1) // three lines small text
   {
      float txtx = rect.x1 + 2;

      float txtys = (rect.h - 15) / 4;

      float txty1 = rect.y1 + txtys - 4;
      float txty2 = txty1 + txtys + 5;
      float txty3 = txty2 + txtys + 5;


      char msg[256];
      sprintf(msg, "%s - %s - [%s]" , text_index, text_player_num, text_player_name);
      al_draw_text(mFont.pixl, mColor.pc[15], txtx, txty1, 0, msg);

      sprintf(msg, "%s" , text_range);
      al_draw_text(mFont.pixl, mColor.pc[15], txtx, txty2, 0, msg);

//      sprintf(msg, "%s - %s" , text_deaths, text_coins);

      sprintf(msg, "%s - %s" , text_lmf, text_ne);

      al_draw_text(mFont.pixl, mColor.pc[15], txtx, txty3, 0, msg);

   }
}


void mwDemoRecord::draw_timeline_time_box(float xc, int y1, int y2, int y_offset, int f, int color)
{
   char m1[64];
   gettf(f, m1);

   int bts = 10;

   if (y_offset < 0)
   {
      int y = y2;
      al_draw_line(xc, y, xc, y-y_offset, mColor.pc[color], 1);
      y-=y_offset;
      mWidget.mButton(5, xc, 0, 1, y, bts,     2, 1, 1, 3,    0, color, 15, 15, -1, m1, 1);
   }
   if (y_offset > 0)
   {
      int y = y1;
      al_draw_line(xc, y, xc, y-y_offset, mColor.pc[color], 1);
      y-=(y_offset+bts);
      mWidget.mButton(5, xc, 0, 1, y, bts,     2, 1, 1, 3,    0, color, 15, 15, -1, m1, 1);
   }
}



void mwDemoRecord::delete_section(int gmInfo_index)
{
   int p   = mGmInfo.gmPlayerInfo[gmInfo_index].playerNum;
   int f1  = mGmInfo.gmPlayerInfo[gmInfo_index].startFrame;
   int f2  = mGmInfo.gmPlayerInfo[gmInfo_index].endFrame;

   for (int x=0; x<mGameMoves.entry_pos; x++)
      if (mGameMoves.arr[x][0] >= f1 && mGameMoves.arr[x][0] < f2)
         if (mGameMoves.does_game_move_contain_player(x, p)) mGameMoves.clear_single(x);

   mGameMoves.gm_sort();
   refresh();
}


void mwDemoRecord::proc_timeline_context_menu(int gmInfo_index, int mouse_frame)
{
   if (mInput.mouse_b[2][0])
   {
      int i = gmInfo_index;
      int p   = mGmInfo.gmPlayerInfo[i].playerNum;
      //int f1  = mGmInfo.gmPlayerInfo[i].startFrame;
      //int f2  = mGmInfo.gmPlayerInfo[i].endFrame;

      sprintf(mMenu.menu_string[0],"Timeline Context  Menu");
      sprintf(mMenu.menu_string[1],"----------------------");
      sprintf(mMenu.menu_string[2],"Change Player Name and Color");
      sprintf(mMenu.menu_string[3],"Delete Section");
      sprintf(mMenu.menu_string[4],"Add new section at current pos");
      sprintf(mMenu.menu_string[5],"end");
      int mp = mMenu.pmenu(5, 13, -12, 1);

      if (mp == 2)  change_player_color_and_name_dialog(gmInfo_index);
      if (mp == 3 && p != 0) delete_section(gmInfo_index); // delete section -- but don't delete player 0
      if (mp == 4) add_new_section_dialog(); // add new section

   }
}


