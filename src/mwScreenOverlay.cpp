// mwScreen.cpp

#include "pm.h"
#include "mwScreen.h"
#include "mwQuickGraph.h"
#include "mwLog.h"
#include "mwPlayers.h"
#include "mwNetgame.h"
#include "mwDrawSequence.h"
#include "mwSettings.h"
#include "mwBottomMessage.h"
#include "mwDemoMode.h"
#include "mwDisplay.h"
#include "mwFont.h"
#include "mwBitmap.h"
#include "mwWidgets.h"
#include "mwGameMoves.h"
#include "mwTimeStamp.h"
#include "mwColor.h"
#include "mwInput.h"
#include "mwLoop.h"
#include "mwEnemy.h"
#include "mwLevel.h"

void mwScreen::show_player_stat_box(int tx, int y, int p)
{

   al_draw_filled_rectangle(tx, y, tx+200, y+50, mColor.pc[0]);
   al_draw_rectangle(       tx, y, tx+200, y+50, mColor.pc[15], 1);
   al_draw_rectangle(       tx, y, tx+200, y+11, mColor.pc[15], 1);

   y+=2;

   int c = 15;

   if ((mNetgame.ima_server) || (mNetgame.ima_client))
      al_draw_textf(mFont.pr8, mColor.pc[c], tx+2, y, 0, "Player:%d [%s]", p, mPlayer.loc[p].hostname);
   else al_draw_textf(mFont.pr8, mColor.pc[c], tx+2, y, 0, "Player:%d", p);
   if (mPlayer.syn[0].level_done_mode == 5)
   {
      if (!mPlayer.syn[p].level_done_ack)
      {
         c = mColor.flash_color;
         int pay = 16;
         al_draw_textf(mFont.pr8, mColor.pc[c], tx+158, y+pay, 0, "press");
         al_draw_textf(mFont.pr8, mColor.pc[c], tx+158, y+pay+8, 0, " any");
         int tl = mPlayer.syn[0].level_done_timer/40;
         if (tl > 9) al_draw_textf(mFont.pr8, mColor.pc[c], tx+154, y+pay+18, 0, "  %2d", tl);
         else        al_draw_textf(mFont.pr8, mColor.pc[c], tx+158, y+pay+18, 0, "  %d", tl);
      }
      else al_draw_textf(mFont.pr8, mColor.pc[15], tx+158, y+20, 0, "ready");
   }

   y += 12;

   al_draw_bitmap(mBitmap.player_tile[mPlayer.syn[p].color][1], tx+2, y, 0); // draw shape
   c = 15;
   al_draw_textf(mFont.pr8, mColor.pc[15], tx+24, y, 0, "Deaths:%d", mPlayer.syn[p].stat_respawns); y+=8;
   al_draw_textf(mFont.pr8, mColor.pc[15], tx+24, y, 0, "Shots:%d", mPlayer.syn[p].stat_shots_fired); y+=8;
   al_draw_textf(mFont.pr8, mColor.pc[15], tx+24, y, 0, "Enemy Hits:%d", mPlayer.syn[p].stat_enemy_hits); y+=8;
   al_draw_textf(mFont.pr8, mColor.pc[15], tx+24, y, 0, "Purple Coins:%d/%d", mPlayer.syn[p].stat_purple_coins, mLevel.number_of_purple_coins); y+=8;

}

void mwScreen::show_level_done(void)
{
   if ((mPlayer.syn[0].level_done_mode > 3) && (mPlayer.syn[0].level_done_mode < 8))
   {
      draw_large_text_overlay(2, 0);
      mColor.process_flash_color();

      int x = mDisplay.SCREEN_W/2;
      int y = mDisplay.SCREEN_H/2;

      int np = 0; // number of active players
      for (int p=0; p<NUM_PLAYERS; p++) if (mPlayer.syn[p].active) np++;

      int tx=0, ty=0, pc=0;
      if (np == 1)
      {
         tx = x-100;
         ty = y-25;
         for (int p=0; p<NUM_PLAYERS; p++)
            if (mPlayer.syn[p].active)
            {
               show_player_stat_box(tx, ty, p);
               ty += 56;
            }
      }
      if (np == 2)
      {
         int tx = x-200;
         int ty = y-25;
         for (int p=0; p<NUM_PLAYERS; p++)
            if (mPlayer.syn[p].active)
            {
               show_player_stat_box(tx, ty, p);
               tx = x+5;
            }
      }
      if (np == 3)
      {
         for (int p=0; p<NUM_PLAYERS; p++)
            if (mPlayer.syn[p].active)
            {
               if (pc == 0) { tx = x-100; ty = y-80; }
               if (pc == 1) { tx = x-100; ty = y-25; }
               if (pc == 2) { tx = x-100; ty = y+30; }
               show_player_stat_box(tx, ty, p);
               pc++;
            }
      }
      if (np == 4)
      {
         for (int p=0; p<NUM_PLAYERS; p++)
            if (mPlayer.syn[p].active)
            {
               if (pc == 0) { tx = x+-200; ty = y-50; }
               if (pc == 1) { tx = x+5;    ty = y-50; }
               if (pc == 2) { tx = x+-200; ty = y+5; }
               if (pc == 3) { tx = x+5;    ty = y+5; }
               show_player_stat_box(tx, ty, p);
               pc++;
            }
      }
      if ((np == 5) || (np == 6))
      {
         for (int p=0; p<NUM_PLAYERS; p++)
            if (mPlayer.syn[p].active)
            {
               if (pc == 0) { tx = x-200; ty = y-80; }
               if (pc == 1) { tx = x+5;   ty = y-80; }
               if (pc == 2) { tx = x-200; ty = y-25; }
               if (pc == 3) { tx = x+5;   ty = y-25; }
               if (pc == 4) { tx = x-200; ty = y+30; }
               if (pc == 5) { tx = x+5;   ty = y+30; }
               show_player_stat_box(tx, ty, p);
               pc++;
            }
      }
      if ((np == 7) || (np == 8))
      {
         for (int p=0; p<NUM_PLAYERS; p++)
            if (mPlayer.syn[p].active)
            {
               if (pc == 0) { tx = x-200; ty = y-105; }
               if (pc == 1) { tx = x+5;   ty = y-105; }
               if (pc == 2) { tx = x-200; ty = y-50; }
               if (pc == 3) { tx = x+5;   ty = y-50; }
               if (pc == 4) { tx = x-200; ty = y+5; }
               if (pc == 5) { tx = x+5;   ty = y+5; }
               if (pc == 6) { tx = x-200; ty = y+60; }
               if (pc == 7) { tx = x+5;   ty = y+60; }
               show_player_stat_box(tx, ty, p);
               pc++;
            }
      }
   }
}

void mwScreen::set_player_join_quit_display(int p, int type, int time)
{
   mLoop.show_player_join_quit_timer = time;
   mLoop.show_player_join_quit_jq = type;
   mLoop.show_player_join_quit_player = p;
}

void mwScreen::show_player_join_quit(void)
{
   char msg[1024];
   if (mLoop.show_player_join_quit_timer)
   {
      int t =  mLoop.show_player_join_quit_timer--;
      int jq = mLoop.show_player_join_quit_jq;
      int p = mLoop.show_player_join_quit_player;
      int color = mPlayer.syn[p].color;

      if (jq == 0) sprintf(msg, "Player %d left the game!", p);
      if (jq == 1) sprintf(msg, "Player %d joined the game!", p);
      if (jq == 2) sprintf(msg, "Player %d found the exit!", p);
      if (jq == 3) sprintf(msg, "Player %d DIED!", p);

      float stretch = ( (float)mDisplay.SCREEN_W / (strlen(msg)*8)) - 1; // (mDisplay.SCREEN_W / text length*8) -1
      float ratio = (float)t / 60;

      int y_pos = mDisplay.SCREEN_H/2;
      int y_pos_move = mDisplay.SCREEN_H/2;

      if (ratio > .6)
      {
           float ra1 = 1 - ratio;                // starts at 0 and increases to .3
           float ra2 = ra1 * 2.5;                // starts at 0 and increases to .999
           stretch =  ra2 * stretch;
           y_pos = y_pos - y_pos_move + (int)(ra2 * y_pos_move);
      }
      if (ratio < .4)
      {
           float ra1 = .4 - ratio;               // starts at 0 and increases to .3
           float ra2 = ra1 * 2.5;                // starts at 0 and increases to .999
           stretch -=  ra2 * stretch;
           y_pos += (int)(ra2 * y_pos_move);
      }
      if (stretch < .1) stretch = .1;
      rtextout_centre(mFont.bltn, NULL, msg, mDisplay.SCREEN_W/2, y_pos, color, stretch, 0.5);
   }
}

void mwScreen::sdg_show_column(int col, int &x, int y)
{
   char msg[1024];
   y-=8;

   int color = 15;
   int color1 = 15;
   int color2 = 63 + 64;

   if (col == 1) // player number
   {
      al_draw_text(mFont.pr8, mColor.pc[color], x, y+=8, 0, "[p]");
      for (int p=0; p<NUM_PLAYERS; p++)
      {
         if (mPlayer.syn[p].active == 1) color = color1;
         if (mPlayer.syn[p].active == 0) color = color2;
         al_draw_textf(mFont.pr8, mColor.pc[color], x, y+=8, 0, "[%d]", p);
      }
      x+=3*8;
   }

   if (col == 2) // active
   {
      al_draw_text(mFont.pr8, mColor.pc[color], x, y+=8, 0, "[a]");
      for (int p=0; p<NUM_PLAYERS; p++)
      {
         if (mPlayer.syn[p].active == 1) color = color1;
         if (mPlayer.syn[p].active == 0) color = color2;
         al_draw_textf(mFont.pr8, mColor.pc[color], x, y+=8, 0, "[%d]", mPlayer.syn[p].active);
      }
      x+=3*8;
   }

   if (col == 3) // color
   {
      al_draw_text(mFont.pr8, mColor.pc[color], x, y+=8, 0, "[co]");
      for (int p=0; p<NUM_PLAYERS; p++)
      {
         if (mPlayer.syn[p].active == 1) color = mPlayer.syn[p].color;
         if (mPlayer.syn[p].active == 0) color = color2;

         al_draw_textf(mFont.pr8, mColor.pc[color], x, y+=8, 0, "[%2d]", mPlayer.syn[p].color);
      }
      x+=4*8;
   }

   if (col == 4) // control method
   {
      al_draw_text(mFont.pr8, mColor.pc[color], x, y+=8, 0, "[m]");
      for (int p=0; p<NUM_PLAYERS; p++)
      {
         if (mPlayer.syn[p].active == 1) color = color1;
         if (mPlayer.syn[p].active == 0) color = color2;
         al_draw_textf(mFont.pr8, mColor.pc[color], x, y+=8, 0, "[%d]", mPlayer.syn[p].control_method);
      }
      x+=3*8;
   }

   if (col == 5) // who
   {
      al_draw_text(mFont.pr8, mColor.pc[color], x, y+=8, 0, "[wh]");
      for (int p=0; p<NUM_PLAYERS; p++)
      {
         if (mPlayer.syn[p].active == 1) color = color1;
         if (mPlayer.syn[p].active == 0) color = color2;
         al_draw_textf(mFont.pr8, mColor.pc[color], x, y+=8, 0, "[%d]", mPlayer.loc[p].who);
      }
      x+=4*8;
   }

   if (col == 6) // server_state_freq
   {
      al_draw_text(mFont.pr8, mColor.pc[color], x, y+=8, 0, "[f]");
      for (int p=0; p<NUM_PLAYERS; p++)
      {
         if (p == 0) al_draw_textf(mFont.pr8, mColor.pc[color], x, y+=8, 0, "[%d]", mPlayer.loc[p].server_state_freq);
         else        al_draw_textf(mFont.pr8, mColor.pc[color], x, y+=8, 0, "[ ]");
      }
      x+=3*8;
   }

   if (col == 7) // client chase fps
   {
      al_draw_text(mFont.pr8, mColor.pc[color], x, y+=8, 0, "[chas]");
      for (int p=0; p<NUM_PLAYERS; p++)
      {
         if (mPlayer.syn[p].active == 1) color = color1;
         if (mPlayer.syn[p].active == 0) color = color2;
         al_draw_textf(mFont.pr8, mColor.pc[color], x, y+=8, 0, "[%4.1f]", mPlayer.loc[p].client_chase_fps);
      }
      x+=6*8;
   }

   if (col == 8) // server_game_move_sync
   {
      al_draw_text(mFont.pr8, mColor.pc[color], x, y+=8, 0, "[sgms]");
      for (int p=0; p<NUM_PLAYERS; p++)
      {
         if (mPlayer.syn[p].active == 1) color = color1;
         if (mPlayer.syn[p].active == 0) color = color2;
         al_draw_textf(mFont.pr8, mColor.pc[color], x, y+=8, 0, "[%4d]", mPlayer.loc[p].server_game_move_sync);
      }
      x+=6*8;
   }

   if (col == 9) // client_base_resets
   {
      al_draw_text(mFont.pr8, mColor.pc[color], x, y+=8, 0, "[base]");
      for (int p=0; p<NUM_PLAYERS; p++)
      {
         if (mPlayer.syn[p].active == 1) color = color1;
         if (mPlayer.syn[p].active == 0) color = color2;
         al_draw_textf(mFont.pr8, mColor.pc[color], x, y+=8, 0, "[%4d]", mPlayer.loc[p].client_base_resets);
      }
      x+=6*8;
   }

   if (col == 10) // dif src
   {
      al_draw_text(mFont.pr8, mColor.pc[color], x, y+=8, 0, "[start]");
      for (int p=0; p<NUM_PLAYERS; p++)
      {
         if (mPlayer.syn[p].active == 1) color = color1;
         if (mPlayer.syn[p].active == 0) color = color2;
         al_draw_textf(mFont.pr8, mColor.pc[color], x, y+=8, 0, "[%5d]", mNetgame.srv_client_state_frame_num[p][0]);
      }
      x+=7*8;
   }
   if (col == 11) // dif dst
   {
      al_draw_text(mFont.pr8, mColor.pc[color], x, y+=8, 0, "[destn]");
      for (int p=0; p<NUM_PLAYERS; p++)
      {
         if (mPlayer.syn[p].active == 1) color = color1;
         if (mPlayer.syn[p].active == 0) color = color2;
         al_draw_textf(mFont.pr8, mColor.pc[color], x, y+=8, 0, "[%5d]", mNetgame.srv_client_state_frame_num[p][1]);
      }
      x+=7*8;
   }
   if (col == 12) // number of packets
   {
      al_draw_text(mFont.pr8, mColor.pc[color], x, y+=8, 0, "[p]");
      for (int p=0; p<NUM_PLAYERS; p++)
      {
         if (mPlayer.syn[p].active == 1) color = color1;
         if (mPlayer.syn[p].active == 0) color = color2;
         al_draw_textf(mFont.pr8, mColor.pc[color], x, y+=8, 0, "[%d]", mPlayer.loc[p].num_dif_packets);
      }
      x+=3*8;
   }
   if (col == 13) // dif size
   {
      al_draw_text(mFont.pr8, mColor.pc[color], x, y+=8, 0, "[difs]");
      for (int p=0; p<NUM_PLAYERS; p++)
      {
         if (mPlayer.syn[p].active == 1) color = color1;
         if (mPlayer.syn[p].active == 0) color = color2;
         al_draw_textf(mFont.pr8, mColor.pc[color], x, y+=8, 0, "[%4d]", mPlayer.loc[p].cmp_dif_size);
      }
      x+=6*8;
   }
   if (col == 14) // tx kB/sec
   {
      al_draw_text(mFont.pr8, mColor.pc[color], x, y+=8, 0, "[tkbs]");
      for (int p=0; p<NUM_PLAYERS; p++)
      {
         if (mPlayer.syn[p].active == 1) color = color1;
         if (mPlayer.syn[p].active == 0) color = color2;
         al_draw_textf(mFont.pr8, mColor.pc[color], x, y+=8, 0, "[%4.1f]", (float)mPlayer.loc[p].tx_bytes_per_tally/1000);
      }
      x+=6*8;
   }

   if (col == 15) // rx kB/sec
   {
      al_draw_text(mFont.pr8, mColor.pc[color], x, y+=8, 0, "[rkbs]");
      for (int p=0; p<NUM_PLAYERS; p++)
      {
         if (mPlayer.syn[p].active == 1) color = color1;
         if (mPlayer.syn[p].active == 0) color = color2;
         al_draw_textf(mFont.pr8, mColor.pc[color], x, y+=8, 0, "[%4.1f]", (float)mPlayer.loc[p].rx_bytes_per_tally/1000);
      }
      x+=6*8;
   }

   if (col == 18) // server name and description
   {
      y+=8;
      for (int p=0; p<NUM_PLAYERS; p++)
      {
         y+=8;

         if (p == 0)
         {
            sprintf(msg, "[%s] <-- server (me!)", mPlayer.loc[p].hostname);
            al_draw_text(mFont.pr8, mColor.pc[mPlayer.syn[p].color], x, y, 0, msg);
         }
         if ((mPlayer.syn[p].active) && (mPlayer.syn[p].control_method == 2))
         {
            sprintf(msg, "[%s] <-- active client", mPlayer.loc[p].hostname);
            al_draw_text(mFont.pr8, mColor.pc[mPlayer.syn[p].color], x, y, 0, msg);
         }
         if ((!mPlayer.syn[p].active) && (mPlayer.syn[p].control_method == 2))
         {
            sprintf(msg, "[%s] <-- syncing client", mPlayer.loc[p].hostname);
            al_draw_text(mFont.pr8, mColor.pc[mPlayer.syn[p].color], x, y, 0, msg);
         }
         if (mPlayer.syn[p].control_method == 9)
         {
            sprintf(msg, "[%s] <-- used client", mPlayer.loc[p].hostname);
            al_draw_text(mFont.pr8, mColor.pc[mPlayer.syn[p].color], x, y, 0, msg);
         }
      }
   }

   if (col == 19) // client name and description
   {
      y+=8;
      for (int p=0; p<NUM_PLAYERS; p++)
      {
         y+=8;
         sprintf(msg, " "); // blank
         if (p == 0) sprintf(msg, "[%s] <-- server", mPlayer.loc[p].hostname);
         else
         {
            if ((mPlayer.syn[p].active) && (mPlayer.syn[p].control_method == 2)) sprintf(msg, "[%s] <-- active client", mPlayer.loc[p].hostname);
            if ((!mPlayer.syn[p].active) && (mPlayer.syn[p].control_method == 2)) sprintf(msg, "[%s] <-- syncing client", mPlayer.loc[p].hostname);
            if (mPlayer.syn[p].control_method == 9) sprintf(msg, "[%s] <-- used client", mPlayer.loc[p].hostname);
            if (p == mPlayer.active_local_player) sprintf(msg, "[%s] <-- local client (me)", mPlayer.loc[p].hostname);
         }
         al_draw_text(mFont.pr8, mColor.pc[mPlayer.syn[p].color], x, y, 0, msg);
      }
   }

   if (col == 20) // server stak_sync
   {
      al_draw_text(mFont.pr8, mColor.pc[color], x, y+=8, 0, "[stsy]");
      for (int p=0; p<NUM_PLAYERS; p++)
      {
         if (mPlayer.syn[p].active == 1) color = color1;
         if (mPlayer.syn[p].active == 0) color = color2;
         al_draw_textf(mFont.pr8, mColor.pc[color], x, y+=8, 0, "[%4.1f]", mPlayer.loc[p].stak_dsync*1000);
      }
      x+=6*8;
   }

   if (col == 23) // late cdats
   {
      al_draw_text(mFont.pr8, mColor.pc[color], x, y+=8, 0, "[lcdt]");
      for (int p=0; p<NUM_PLAYERS; p++)
      {
         if (mPlayer.syn[p].active == 1) color = color1;
         if (mPlayer.syn[p].active == 0) color = color2;
         al_draw_textf(mFont.pr8, mColor.pc[color], x, y+=8, 0, "[%4d]", mPlayer.loc[p].late_cdats);
      }
      x+=6*8;
   }

   if (col == 24) // late cdats last second
   {
      al_draw_text(mFont.pr8, mColor.pc[color], x, y+=8, 0, "[lcls]");
      for (int p=0; p<NUM_PLAYERS; p++)
      {
         if (mPlayer.syn[p].active == 1) color = color1;
         if (mPlayer.syn[p].active == 0) color = color2;
         al_draw_textf(mFont.pr8, mColor.pc[color], x, y+=8, 0, "[%4d]", mPlayer.loc[p].late_cdats_last_sec);
      }
      x+=6*8;
   }

   if (col == 25) // client dsync
   {
      al_draw_text(mFont.pr8, mColor.pc[color], x, y+=8, 0, "[cdsy]");
      for (int p=0; p<NUM_PLAYERS; p++)
      {
         if (mPlayer.syn[p].active == 1) color = color1;
         if (mPlayer.syn[p].active == 0) color = color2;
         al_draw_textf(mFont.pr8, mColor.pc[color], x, y+=8, 0, "[%4.0f]", mPlayer.loc[p].dsync*1000);
      }
      x+=6*8;
   }

   if (col == 26) // client game move sync avg last sec
   {
      al_draw_text(mFont.pr8, mColor.pc[color], x, y+=8, 0, "[gmav]");
      for (int p=0; p<NUM_PLAYERS; p++)
      {
         if (mPlayer.syn[p].active == 1) color = color1;
         if (mPlayer.syn[p].active == 0) color = color2;
         al_draw_textf(mFont.pr8, mColor.pc[color], x, y+=8, 0, "[%4.0f]", mPlayer.loc[p].game_move_dsync_avg_last_sec*1000);
      }
      x+=6*8;
   }

   if (col == 27) // ping
   {
      al_draw_text(mFont.pr8, mColor.pc[color], x, y+=8, 0, "[ping]");
      for (int p=0; p<NUM_PLAYERS; p++)
      {
         if (mPlayer.syn[p].active == 1) color = color1;
         if (mPlayer.syn[p].active == 0) color = color2;
         if (p == 0) al_draw_textf(mFont.pr8, mColor.pc[color], x, y+=8, 0, "[    ]");
         else        al_draw_textf(mFont.pr8, mColor.pc[color], x, y+=8, 0, "[%4.1f]", mPlayer.loc[p].ping*1000);
      }
      x+=6*8;
   }

   if (col == 28) // client rewind
   {
      al_draw_text(mFont.pr8, mColor.pc[color], x, y+=8, 0, "[crwd]");
      for (int p=0; p<NUM_PLAYERS; p++)
      {
         if (mPlayer.syn[p].active == 1) color = color1;
         if (mPlayer.syn[p].active == 0) color = color2;
         if (p == 0) al_draw_textf(mFont.pr8, mColor.pc[color], x, y+=8, 0, "[    ]");
         else        al_draw_textf(mFont.pr8, mColor.pc[color], x, y+=8, 0, "[%4d]", mPlayer.loc[p].client_rewind);
      }
      x+=6*8;
   }
}

void mwScreen::sdg_show(int x, int y) // server debug grid
{
   al_draw_filled_rectangle(x, y, x+880, y+73, mColor.pc[0]);
   y+=1;
   sdg_show_column(1, x, y); // player number
   sdg_show_column(2, x, y); // active
   sdg_show_column(3, x, y); // color
   sdg_show_column(4, x, y); // control method
//   sdg_show_column(5, x+1, y); // who
   sdg_show_column(6, x, y); // server_state_freq
//   sdg_show_column(7, x, y); // client chase fps
   sdg_show_column(8, x, y); // server_game_move_sync
   sdg_show_column(9, x, y); // client base resets
   sdg_show_column(23, x, y); // late cdats
   sdg_show_column(24, x, y); // late cdats last second
//   sdg_show_column(10, x+1, y); // dif src
//   sdg_show_column(11, x+1, y); // dif dst
   sdg_show_column(12, x, y); // number of packets
   sdg_show_column(13, x, y); // dif size
   sdg_show_column(14, x, y); // tx kB/sec
//   sdg_show_column(15, x, y); // rx kB/sec
   sdg_show_column(20, x, y); // stak_sysnc
   sdg_show_column(27, x, y); // ping
   sdg_show_column(25, x, y); //
   sdg_show_column(26, x, y); //
   sdg_show_column(18, x, y); // name and description (server version)
}

void mwScreen::cdg_show(int x, int y) // client debug grid
{
   al_draw_filled_rectangle(x, y, x+400, y+73, mColor.pc[0]);
   y+=1;
   sdg_show_column(1, x, y); // player number
   sdg_show_column(2, x, y); // active
   sdg_show_column(3, x, y); // color
   sdg_show_column(4, x, y); // control method
   sdg_show_column(9, x, y);  // client base resets
   sdg_show_column(28, x, y); // client rewind
   sdg_show_column(19, x, y); // name and description (client version)
}

void mwScreen::draw_common_debug_overlay(int p, int &cx, int &cy)
{
   double tt = 0;
   if (mLog.LOG_TMR_scrn_overlay) tt = al_get_time();

   if (mSettings.overlay_grid[1][mLoop.show_debug_overlay]) // display
   {
      al_draw_filled_rectangle(cx, cy, cx+168, cy+47, mColor.pc[0]); cy+=2;
      al_draw_textf(mFont.pr8, mColor.pc[15], cx+1, cy, 0, "display: %d x %d ", al_get_display_width(mDisplay.display), al_get_display_height(mDisplay.display)); cy+=9;
      al_draw_textf(mFont.pr8, mColor.pc[15], cx+1, cy, 0, "screen : %d x %d", mDisplay.SCREEN_W, mDisplay.SCREEN_H); cy+=9;
      if (mDisplay.saved_display_transform_double == 0) al_draw_textf(mFont.pr8, mColor.pc[15], cx+1, cy, 0, "double:%d [auto]", mDisplay.display_transform_double);
      else                                         al_draw_textf(mFont.pr8, mColor.pc[15], cx+1, cy, 0, "double:%d [static]", mDisplay.display_transform_double);
      cy+=9;
      al_draw_textf(mFont.pr8, mColor.pc[15], cx+1, cy, 0, "scale_factor:%3.2f", mDisplay.scale_factor); cy+=9;
      al_draw_textf(mFont.pr8, mColor.pc[15], cx+1, cy, 0, "frames per second:%d", mLoop.frame_speed); cy+=9;
      if (mLog.LOG_TMR_scrn_overlay) mLog.add_log_TMR(al_get_time() - tt, "scov_dbgcom", 0);
      cy +=4;
   }


   if (mSettings.overlay_grid[2][mLoop.show_debug_overlay])
   {
       mDrawSequence.show_text(cx, cy); // show draw profile times
       cy+=100;
   }



   if (mLog.LOG_TMR_scrn_overlay) tt = al_get_time();

   if (mSettings.overlay_grid[0][mLoop.show_debug_overlay]) // cpu graph
   {
      //sprintf(mQuickGraph[0].series[0].name, "");
              mQuickGraph[0].series[0].color = 13+128;
              mQuickGraph[0].series[0].active = 1;

      sprintf(mQuickGraph[0].series[1].name, "MIN");
              mQuickGraph[0].series[1].color = 9+32;
              mQuickGraph[0].series[1].active = 0;

      sprintf(mQuickGraph[0].series[2].name, "MAX");
              mQuickGraph[0].series[2].color = 10+32;
              mQuickGraph[0].series[2].active = 0;

      sprintf(mQuickGraph[0].series[3].name, "CPU");
              mQuickGraph[0].series[3].color = 13;
              mQuickGraph[0].series[3].active = 1;


      mQuickGraph[0].width = 200;
      mQuickGraph[0].height = 36;
      mQuickGraph[0].draw_graph(mDisplay.SCREEN_W-228, mDisplay.SCREEN_H-56);
   }


   if (mLog.LOG_TMR_scrn_overlay) mLog.add_log_TMR(al_get_time() - tt, "scov_CPU", 0);
}

void mwScreen::draw_server_debug_overlay(int p, int &cx, int &cy)
{
   double lts = 0;

   // ------------------------------------
   // ---   server debug grid
   // ------------------------------------
   if (mLog.LOG_TMR_scrn_overlay) lts = al_get_time();
   if (mSettings.overlay_grid[3][mLoop.show_debug_overlay])
   {
      sdg_show(cx, cy); // client debug grid
      cy +=77;
   }
   if (mLog.LOG_TMR_scrn_overlay) mLog.add_log_TMR(al_get_time() - lts, "scov_sgrid", 0);


   if (mSettings.overlay_grid[6][mLoop.show_debug_overlay]) // state freq adjust buttons
   {
      if (mPlayer.loc[0].server_state_freq_mode == 0) // 0 = manual, 1 = auto
      {
         // -----------------------------------------------------
         // server buttons to display and change s1 and s2
         // -----------------------------------------------------
         if (mLog.LOG_TMR_scrn_overlay) lts = al_get_time();

         int csx1 = mDisplay.SCREEN_W-BORDER_WIDTH-96;
         int csw = 80;
         int csx2 = csx1 + csw;

         int csy1 = mDisplay.SCREEN_H-36-60;
         int csh = 36;
         int csy2 = csy1 + csh;

         int color = 13;

         // non blocking buttons!

         al_show_mouse_cursor(mDisplay.display);

         al_draw_filled_rectangle(csx1, csy1, csx2, csy2, mColor.pc[color+224]); // erase background
         al_draw_rectangle(csx1, csy1, csx2, csy2, mColor.pc[color], 1);         // frame

         int ya = 0, btw = 16;
         int csy3 = csy1 + 3;

         ya = csy3;
         static int b1_pres = 0;
         if (mWidget.buttont_nb(csx1+2, ya, csx1+2+btw, 16,  0,0,0,0,  0,color,15, 0,  1,0,1,0, "-"))
         {
            if (b1_pres == 0) mPlayer.loc[p].server_state_freq--;
            if (mPlayer.loc[p].server_state_freq < 1) mPlayer.loc[p].server_state_freq = 1;
            b1_pres = 1;
         }
         else b1_pres = 0;

         ya = csy3;
         static int b2_pres = 0;
         if (mWidget.buttont_nb(csx2-btw-4, ya, csx2-2, 16,  0,0,0,0,  0,color,15, 0,  1,0,1,0, "+"))
         {
            if (b2_pres == 0) mPlayer.loc[p].server_state_freq++;
            b2_pres = 1;
         }
         else b2_pres = 0;
         al_draw_textf(mFont.pr8, mColor.pc[15], csx1+csw/2, csy3+2, ALLEGRO_ALIGN_CENTER, "s1:%d", mPlayer.loc[p].server_state_freq);

         csy3+=17;

         ya = csy3;
         static int b3_pres = 0;
         if (mWidget.buttont_nb(csx1+2, ya, csx1+2+btw, 16,  0,0,0,0,  0,color,15, 0,  1,0,1,0, "-"))
         {
            if (b3_pres == 0) mNetgame.zlib_cmp--;
            if (mNetgame.zlib_cmp < 1) mNetgame.zlib_cmp = 1;
            b3_pres = 3;
         }
         else b3_pres = 0;

         ya = csy3;
         static int b4_pres = 0;
         if (mWidget.buttont_nb(csx2-btw-4, ya, csx2-2, 16,  0,0,0,0,  0,color,15, 0,  1,0,1,0, "+"))
         {
            if (b4_pres == 0) mNetgame.zlib_cmp++;
            b4_pres = 1;
         }
         else b4_pres = 0;
         al_draw_textf(mFont.pr8, mColor.pc[15], csx1+csw/2, csy3+2, ALLEGRO_ALIGN_CENTER, "zc:%d", mNetgame.zlib_cmp);
      }

      if (mLog.LOG_TMR_scrn_overlay) mLog.add_log_TMR(al_get_time() - lts, "scov_sbutt", 0);

   }

   if (mSettings.overlay_grid[8][mLoop.show_debug_overlay])  // misc
   {
      al_draw_filled_rectangle(cx, cy, cx+204, cy+20, mColor.pc[0]); cy+=2;
      al_draw_textf(mFont.pr8, mColor.pc[15], cx+1, cy, 0, "total game moves:%d", mGameMoves.entry_pos); cy+=9;
      al_draw_textf(mFont.pr8, mColor.pc[15], cx+1, cy, 0, "state frequency:%d", mPlayer.loc[p].server_state_freq); cy+=9;
      cy +=4;
   }

   if (mSettings.overlay_grid[7][mLoop.show_debug_overlay])  // bandwidth stats
   {
      int nap = 0;
      for(int p=0; p<NUM_PLAYERS; p++) if (mPlayer.syn[p].active) nap++;

      al_draw_filled_rectangle(cx, cy, cx+604, cy+6+nap*18, mColor.pc[0]); cy+=2;

      for(int p=0; p<NUM_PLAYERS; p++)
         if (mPlayer.syn[p].active)
         {
            al_draw_textf(mFont.pr8, mColor.pc[15], cx, cy, 0, "p:%d bandwidth (B/s) TX cur:[%5d] max:[%5d] RX cur:[%5d] max:[%5d]",
            p, mPlayer.loc[p].tx_bytes_per_tally, mPlayer.loc[p].tx_max_bytes_per_tally, mPlayer.loc[p].rx_bytes_per_tally, mPlayer.loc[p].rx_max_bytes_per_tally);
            cy+=9;
         }

      cy+=4;
      for(int p=0; p<NUM_PLAYERS; p++)
         if (mPlayer.syn[p].active)
         {
            al_draw_textf(mFont.pr8, mColor.pc[15], cx, cy, 0, "p:%d packets   (p/s) TX cur:[%5d] max:[%5d] RX cur:[%5d] max:[%5d]",
            p, mPlayer.loc[p].tx_packets_per_tally, mPlayer.loc[p].tx_max_packets_per_tally, mPlayer.loc[p].rx_packets_per_tally, mPlayer.loc[p].rx_max_packets_per_tally);
            cy+=9;
         }

   }
}

void mwScreen::draw_demo_debug_overlay(int p, int &cx, int &cy)
{
   char msg[1024];
   double tt;

   if (mLog.LOG_TMR_scrn_overlay) tt = al_get_time();
   if (mDemoMode.demo_mode_on) draw_large_text_overlay(3, 15);
   if (mLog.LOG_TMR_scrn_overlay) mLog.add_log_TMR(al_get_time() - tt, "scov_demo_ov", 0);

   if (mSettings.overlay_grid[8][mLoop.show_debug_overlay]) // misc
   {
      int nap = 0; // number of active players
      for (int ap=0; ap<NUM_PLAYERS; ap++) if (mPlayer.syn[ap].active) nap++;

      al_draw_filled_rectangle(cx, cy, cx+150, cy+32+nap*9, mColor.pc[0]);

      al_draw_textf(mFont.pr8, mColor.pc[15], cx+1, cy+1, 0, "Running Saved Game "); cy+=9;
      al_draw_textf(mFont.pr8, mColor.pc[15], cx+1, cy+1, 0, "Time:[%d%%] ", mLoop.frame_num*100/mDemoMode.demo_mode_last_frame); cy+=9;
      al_draw_textf(mFont.pr8, mColor.pc[15], cx+1, cy+1, 0, "Moves:[%d%%] ", (mGameMoves.current_pos * 100) / mGameMoves.entry_pos); cy+=9;
      cy+=4;
      for (int ap=0; ap<NUM_PLAYERS; ap++)
         if (mPlayer.syn[ap].active)
         {
            if (ap == mPlayer.active_local_player) sprintf(msg, "Player:%d <- active", ap);
            else                           sprintf(msg, "Player:%d", ap);
            al_draw_text(mFont.pr8, mColor.pc[mPlayer.syn[ap].color], cx+1, cy+1, 0, msg);
            cy+=9;
         }
      cy+=4;
   }
}

void mwScreen::draw_client_debug_overlay(int p, int &cx, int &cy)
{
   char msg[1024];
   double tt = 0;

   if (!mPlayer.syn[p].active)
   {
      int color = mPlayer.syn[p].color;
      sprintf(msg, "Please wait for server syncronization");
      rtextout_centre(mFont.bltn, NULL, msg, mDisplay.SCREEN_W/2, mDisplay.SCREEN_H/2-32, color, 2, 1);

      sprintf(msg, "[%2.1f]", abs(mPlayer.loc[p].client_chase_offset - mPlayer.loc[p].dsync)*1000);
      rtextout_centre(mFont.bltn, NULL, msg, mDisplay.SCREEN_W/2, mDisplay.SCREEN_H/2, color, 4, 1);
   }
   if (mLog.LOG_TMR_scrn_overlay) mLog.add_log_TMR(al_get_time() - tt, "scov_client", 0);

   // ------------------------------------
   // ---   client debug grid
   // ------------------------------------
   if (mLog.LOG_TMR_scrn_overlay) tt = al_get_time();
   if (mSettings.overlay_grid[3][mLoop.show_debug_overlay])
   {
      cdg_show(cx, cy); // client debug grid
      cy +=78;
   }
   if (mLog.LOG_TMR_scrn_overlay) mLog.add_log_TMR(al_get_time() - tt, "scov_cgrid", 0);


   if (mSettings.overlay_grid[4][mLoop.show_debug_overlay])
   {
      // ----------------------------------
      // ping and sync graph
      // ----------------------------------
      if (mLog.LOG_TMR_scrn_overlay) tt = al_get_time();

      double ds = -mPlayer.loc[p].dsync    * 1000; // the current value of dsync for display
      double pa = mPlayer.loc[p].ping_avg * 1000;

      mQuickGraph[1].add_data(0, ds);
      mQuickGraph[1].add_data(1, pa);

      sprintf(mQuickGraph[1].series[0].name, "SYNC");
      mQuickGraph[1].series[0].color = 10;
      mQuickGraph[1].series[0].active = 1;

      sprintf(mQuickGraph[1].series[1].name, "PING");
      mQuickGraph[1].series[1].color = 14;
      mQuickGraph[1].series[1].active = 1;

      mQuickGraph[1].width = 200;
      mQuickGraph[1].height = 40;
      mQuickGraph[1].draw_graph(mDisplay.SCREEN_W-228, mDisplay.SCREEN_H-36-48-20);

      if (mLog.LOG_TMR_scrn_overlay) mLog.add_log_TMR(al_get_time() - tt, "scov_sync", 0);
   }

   if (mSettings.overlay_grid[5][mLoop.show_debug_overlay])
   {
      // ----------------------------------
      // client sync display and adjustment buttons
      // ----------------------------------
      if (mLog.LOG_TMR_scrn_overlay) tt = al_get_time();
      int csx1 = mDisplay.SCREEN_W - 160;
      int csw = 140;
      int csx2 = csx1 + csw;
      int csy1 = mDisplay.SCREEN_H-36-48-20-60;
      int csh = 52;
      int csy2 = csy1 + csh;
      int color = 13;
      double ts = mPlayer.loc[p].dsync; // the current value of dsync for display
      al_show_mouse_cursor(mDisplay.display);
      al_draw_filled_rectangle(csx1, csy1, csx2, csy2, mColor.pc[color+224]); // erase background
      al_draw_rectangle(csx1, csy1, csx2, csy2, mColor.pc[color], 1);    // frame
      al_draw_textf(mFont.pr8, mColor.pc[15], csx1+csw/2, csy1+2, ALLEGRO_ALIGN_CENTER, "Sync:%+3.0fms", ts*1000);
      // graph
      int csy3 = csy1 + 12;
      int csy4 = csy3 + 12;
      int csy5 = csy4 + 10;

      int csx3 = csx1 + 7;
      int csx4 = csx2 - 7;
      int csw2 = csx2 - csx1;



      al_draw_rectangle(csx3, csy3, csx4, csy4, mColor.pc[color], 1);
      al_draw_textf(mFont.pixl, mColor.pc[color], csx3, csy3-11, ALLEGRO_ALIGN_CENTER, "-50");
      al_draw_textf(mFont.pixl, mColor.pc[color], csx4, csy3-11, ALLEGRO_ALIGN_CENTER, "+25");

      // convert from -0.025 to +0.025 from 0 to csw2
      double gr = -0.050; // graph start
      double gs =  0.075; // graph range

      float dt = ((ts-gr)/gs) * csw2; // convert from range of (0.00-0.025) to (0 - csw)
      al_draw_filled_rectangle(csx3+dt-2, csy3, csx3+dt+2, csy4, mColor.pc[15]);

      // non blocking buttons!
      int ya = csy5 + 2;
      int btw = 16;

      static int b1_pres = 0;
      if (mWidget.buttont_nb(csx1+2, ya, csx1+2+btw, 16,  0,0,0,0,  0,color,15, 0,  1,0,0,0, "-"))
      {
         if (b1_pres == 0)
         {
            if (mPlayer.loc[p].client_chase_offset_mode) // auto mode - adjust offset from ping
            {
               if (mInput.CTRL()) mPlayer.loc[p].client_chase_offset_auto_offset -=0.01;
               else mPlayer.loc[p].client_chase_offset_auto_offset -=0.001;
            }
            else // manual mode - adjust offset directly
            {
               if (mInput.CTRL()) mPlayer.loc[p].client_chase_offset -=0.01;
               else mPlayer.loc[p].client_chase_offset -=0.001;
            }
         }
         b1_pres = 1;
      }
      else b1_pres = 0;

      static int b2_pres = 0;
      if (mWidget.buttont_nb(csx2-btw-2, ya, csx2-2, 16,  0,0,0,0,  0,color,15, 0,  1,0,0,0, "+"))
      {
         if (b2_pres == 0)
         {
            if (mPlayer.loc[p].client_chase_offset_mode) // auto mode - adjust offset from ping
            {
               if (mInput.CTRL()) mPlayer.loc[p].client_chase_offset_auto_offset +=0.01;
               else mPlayer.loc[p].client_chase_offset_auto_offset +=0.001;
            }
            else // manual mode - adjust offset directly
            {
               if (mInput.CTRL()) mPlayer.loc[p].client_chase_offset +=0.01;
               else mPlayer.loc[p].client_chase_offset +=0.001;
            }
         }
         b2_pres = 1;
      }
      else b2_pres = 0;


      static int b3_pres = 0;
      sprintf(msg, "Manual");
      if (mPlayer.loc[p].client_chase_offset_mode) sprintf(msg, "Automatic");

      if (mWidget.buttont_nb(csx1+23, ya, csx2-23, 16,  0,0,0,0,  0,color,15, 0,  1,0,0,0, msg))
      {
         if (b3_pres == 0)
         {
            mPlayer.loc[p].client_chase_offset_mode =  !mPlayer.loc[p].client_chase_offset_mode;
         }
         b3_pres = 1;
      }
      else b3_pres = 0;

      if (mPlayer.loc[p].client_chase_offset_mode)
         al_draw_textf(mFont.pr8, mColor.pc[15], csx1+csw/2, csy1+26, ALLEGRO_ALIGN_CENTER, "ping offset:%+3.0fms", mPlayer.loc[p].client_chase_offset_auto_offset*1000);
      else
         al_draw_textf(mFont.pr8, mColor.pc[15], csx1+csw/2, csy1+26, ALLEGRO_ALIGN_CENTER, "offset:%+3.0fms", mPlayer.loc[p].client_chase_offset*1000);

      if (mLog.LOG_TMR_scrn_overlay) mLog.add_log_TMR(al_get_time() - tt, "scov_cbut", 0);

   }


   if (mSettings.overlay_grid[8][mLoop.show_debug_overlay]) // misc
   {
      al_draw_filled_rectangle(cx, cy, cx+204, cy+38, mColor.pc[0]); cy+=2;
      al_draw_textf(mFont.pr8, mColor.pc[15], cx+1, cy, 0, "local moves:%d", mGameMoves.entry_pos); cy+=9;
      al_draw_textf(mFont.pr8, mColor.pc[15], cx+1, cy, 0, "move lag:%d", mPlayer.loc[p].client_move_lag);  cy+=9;
      al_draw_textf(mFont.pr8, mColor.pc[15], cx+1, cy, 0, "max x correction:%3.1f", mPlayer.loc[p].xcor_max);  cy+=9;
      al_draw_textf(mFont.pr8, mColor.pc[15], cx+1, cy, 0, "late cdats last second:%d", mPlayer.loc[p].late_cdats_last_sec);  cy+=9;
      cy +=4;
   }

   if (mSettings.overlay_grid[7][mLoop.show_debug_overlay]) // bandwidth stats
   {
      al_draw_filled_rectangle(cx, cy, cx+274, cy+60, mColor.pc[0]); cy+=2;

      al_draw_textf(mFont.pr8, mColor.pc[15], cx, cy, 0, "bandwidth (bytes per second)");   cy+=9;
      al_draw_textf(mFont.pr8, mColor.pc[15], cx, cy, 0, "TX currrent:[%d] max:[%d]", mPlayer.loc[p].tx_bytes_per_tally, mPlayer.loc[p].tx_max_bytes_per_tally);   cy+=9;
      al_draw_textf(mFont.pr8, mColor.pc[15], cx, cy, 0, "RX currrent:[%d] max:[%d]", mPlayer.loc[p].rx_bytes_per_tally, mPlayer.loc[p].rx_max_bytes_per_tally);   cy+=9;

      cy +=4;
      al_draw_textf(mFont.pr8, mColor.pc[15], cx, cy, 0, "packets per second");   cy+=9;
      al_draw_textf(mFont.pr8, mColor.pc[15], cx, cy, 0, "TX currrent:[%d] max:[%d]", mPlayer.loc[p].tx_packets_per_tally, mPlayer.loc[p].tx_max_packets_per_tally);   cy+=9;
      al_draw_textf(mFont.pr8, mColor.pc[15], cx, cy, 0, "RX currrent:[%d] max:[%d]", mPlayer.loc[p].rx_packets_per_tally, mPlayer.loc[p].rx_max_packets_per_tally);   cy+=9;
      cy +=4;
   }
}

void mwScreen::draw_top_frame(int p)
{
   char msg[1024];
   int tdx = BORDER_WIDTH;
   int tdy = 0;
   int tc = mColor.get_contrasting_color(mPlayer.syn[p].color);

   double tt = 0;

   // ----------------------------------
   // draw info on top frame
   // ----------------------------------
   if (mLog.LOG_TMR_scrn_overlay) tt = al_get_time();

   if (mDisplay.SCREEN_W < 600) sprintf(msg,"Lv:%d Tm:%d En:%d ",            mLevel.play_level, mLoop.frame_num/40, mEnemy.num_enemy); // special case for narrow screens
   else                sprintf(msg,"Level:%d | Time:%d | Enemies:%d  ", mLevel.play_level, mLoop.frame_num/40, mEnemy.num_enemy);
   al_draw_text(mFont.pr8, mColor.pc[tc], tdx, tdy+2,  0, msg);
   tdx += strlen(msg)*8;

   // draw health bar
   draw_percent_bar(tdx+44, tdy, 88, 10, (int)mPlayer.syn[p].health);
   al_draw_textf(mFont.pr8, mColor.pc[16], tdx+44, tdy+2, ALLEGRO_ALIGN_CENTRE,"Health:%-2d", (int) mPlayer.syn[p].health );
   tdx += 88;


   // draw purple coins
   al_draw_scaled_bitmap(mBitmap.tile[197], 0, 0, 19, 19, tdx+8, tdy+1, 10, 10, 0);
   // spin_shape(197, tdx+5, tdy-3, 0, 0, 19, 19, 0.6, 0.5, 60);
   al_draw_textf(mFont.pr8, mColor.pc[tc], tdx+17, tdy+2, 0, ":%d/%d", mPlayer.syn[mPlayer.active_local_player].stat_purple_coins, mLevel.number_of_purple_coins);




   if (mDisplay.show_scale_factor > 0)
   {
      al_draw_textf(mFont.pr8, mColor.pc[tc], mDisplay.SCREEN_W*2/3, 2, ALLEGRO_ALIGN_CENTER, "Scale:%-3.2f", mDisplay.scale_factor);
   }
   if (mDisplay.show_dtd > 0)
   {
      mDisplay.show_dtd --;
      if (mDisplay.saved_display_transform_double) al_draw_textf(mFont.pr8, mColor.pc[tc],mDisplay.SCREEN_W*4/5, 2, ALLEGRO_ALIGN_CENTER, "dtd:%d [f]", mDisplay.display_transform_double);
      else                                    al_draw_textf(mFont.pr8, mColor.pc[tc],mDisplay.SCREEN_W*4/5, 2, ALLEGRO_ALIGN_CENTER, "dtd:%d [a]", mDisplay.display_transform_double);
   }
   if (mLog.LOG_TMR_scrn_overlay) mLog.add_log_TMR(al_get_time() - tt, "scov_top_frm", 0);
}

void mwScreen::draw_bottom_frame(int p)
{
   char msg[1024];
   int bdy = mDisplay.SCREEN_H - 10;
   int bdx = BORDER_WIDTH;
   int ts = 0;  // text spacing

   int tc = mColor.get_contrasting_color(mPlayer.syn[p].color);




   // ----------------------------------
   // draw common info on bottom frame
   // ----------------------------------

   int bdx2 = mDisplay.SCREEN_W - BORDER_WIDTH;

   sprintf(msg, " cpu:% 2.0f%%", mRollingAverage[0].mx);
   bdx2 -= strlen(msg)*8;
   al_draw_text(mFont.pr8, mColor.pc[tc], bdx2, bdy, 0, msg);

   if (mLoop.show_debug_overlay)
   {
      sprintf(msg, " dbg:%d ", mLoop.show_debug_overlay);
      bdx2 -= strlen(msg)*8;
      al_draw_text(mFont.pr8, mColor.pc[tc], bdx2, bdy, 0, msg);
   }

   if (mLoop.eco_draw)
   {
      sprintf(msg, " eco ");
      bdx2 -= strlen(msg)*8;
      al_draw_text(mFont.pr8, mColor.pc[tc], bdx2, bdy, 0, msg);
   }

   if (mNetgame.ima_server)
   {
      sprintf(msg, "Netgame Server (%s) ", mPlayer.loc[p].hostname );
      al_draw_text(mFont.pr8, mColor.pc[tc], bdx, bdy, 0, msg);
      ts += strlen(msg)*8;

       // calculate num of clients
       int num_clients = 0;
       for (int p=0; p<NUM_PLAYERS; p++)
          if ((mPlayer.syn[p].active) && (mPlayer.syn[p].control_method == 2)) num_clients++;

      // sprintf(msg, "[clients:%d] [moves:%d]", num_clients, entry_pos);
      sprintf(msg, " clients:%d ", num_clients);
      al_draw_text(mFont.pr8, mColor.pc[tc], bdx + ts, bdy, 0, msg);
      ts += strlen(msg)*8;

      // total bandwidth
      int tb = mPlayer.loc[0].tx_bytes_per_tally + mPlayer.loc[0].rx_bytes_per_tally;
      if (tb < 1000) sprintf(msg, "(%dB/s)", tb);
      else
      {
         float ftb = (float)tb/1000;
         sprintf(msg, "(%3.1fkB/s)", ftb);
      }
      al_draw_text(mFont.pr8, mColor.pc[tc], bdx + ts, bdy, 0, msg);
      ts += strlen(msg)*8;
   }

   if (mNetgame.ima_client)
   {
      sprintf(msg, "Netgame Client (%s) ", mLoop.local_hostname );
      al_draw_text(mFont.pr8, mColor.pc[tc], bdx, bdy, 0, msg);
      ts += strlen(msg)*8;

      sprintf(msg, " ping:%3.1f ", mPlayer.loc[p].ping * 1000);
      al_draw_text(mFont.pr8, mColor.pc[tc], bdx + ts, bdy, 0, msg);
      ts += strlen(msg)*8;

      sprintf(msg, " sync:%3.1f ", mPlayer.loc[p].dsync*1000);
      al_draw_text(mFont.pr8, mColor.pc[tc], bdx + ts, bdy, 0, msg);
      ts += strlen(msg)*8;

      if (mPlayer.loc[p].late_cdats_last_sec)
      {
         sprintf(msg, "Warning! Late cdats:%d%% ", (mPlayer.loc[p].late_cdats_last_sec*100)/40);
         bdx2 -= strlen(msg)*8;
         al_draw_text(mFont.pr8, mColor.pc[mColor.flash_color], bdx2, bdy, 0, msg);
         mColor.process_flash_color();
         mColor.process_flash_color();
      }
   }
   if (mPlayer.syn[0].control_method == 1) // file play
   {
      sprintf(msg, "Running Saved Game ");
      al_draw_text(mFont.pr8, mColor.pc[tc], bdx, bdy, 0, msg);
      ts += strlen(msg)*8;

      sprintf(msg, "[%d%%] ", mLoop.frame_num*100/mDemoMode.demo_mode_last_frame);
      al_draw_text(mFont.pr8, mColor.pc[tc], bdx + ts, bdy, 0, msg);
      ts += strlen(msg)*8;
   }
}

void mwScreen::draw_screen_overlay(void)
{
   int p = mPlayer.active_local_player;
   int cx = BORDER_WIDTH;
   int cy = BORDER_WIDTH;

   al_set_target_backbuffer(mDisplay.display);

   double t0 = al_get_time();

   show_level_done();

   mBottomMessage.draw(0);

   show_player_join_quit();

   draw_top_frame(p);
   draw_bottom_frame(p);


   if (mNetgame.ima_server)                     draw_server_debug_overlay(p, cx, cy);
   if (mNetgame.ima_client)                     draw_client_debug_overlay(p, cx, cy);
   if (mPlayer.syn[0].control_method == 1) draw_demo_debug_overlay(p, cx, cy);

   draw_common_debug_overlay(p, cx, cy);
   if (mLog.LOG_TMR_scrn_overlay) mLog.add_log_TMR(al_get_time() - t0, "scrn_overlay_all", 0);
}