// mwLoop.cpp

#include "pm.h"

#include "mwLoop.h"
#include "mwSound.h"
#include "mwLog.h"
#include "mwNetgame.h"

#include "mwDisplay.h"
#include "mwScreen.h"
#include "mwBitmap.h"
#include "mwColor.h"
#include "mwDemoMode.h"

#include "mwPlayer.h"
#include "mwEnemy.h"
#include "mwItem.h"
#include "mwLevel.h"
#include "mwShot.h"

#include "mwGameMoves.h"
#include "mwGameState.h"
#include "mwMain.h"


void mwLoop::proc_level_done_mode()
{
//   mLog.addf(LOG_OTH_level_done, 0, "[%4d] Level Done Mode:%d\n", frame_num, mGameState.level_done_mode);

   //-------------------------------------
   // start of final level rocket cutscene
   //-------------------------------------
   if (mGameState.level_done_mode == 30) // setup for players seek and zoom out
   {
      mLog.add(LOG_OTH_LEVEL_DONE, mGameState.level_done_mode, -1, 0,0,0,0,0,0,0,0,0,0, "Setup for players seek and zoom out");


      mLevel.add_play_data_record(mLevel.play_level, 1);

      mGameState.level_done_timer = 0; // immediate next mode


      if (!mMain.headless_server)
      {
         cutscene_original_zoom = mDisplay.scale_factor_current;
         mDisplay.set_custom_scale_factor((float)(mDisplay.SCREEN_H - BORDER_WIDTH*2)/2000, 100);
      }


      // bring other netgame players home
      int c = mGameState.level_done_player; // captain of the ship!
      mPlayer.syn[c].xinc = 0;
      mPlayer.syn[c].yinc = 0;

      // each player has its own home place on the ship
      int xh = 120;
      int yh = 260;
      for (int p=0; p< NUM_PLAYERS; p++)
         if ((mPlayer.syn[p].active) && (p != c) && (mPlayer.syn[p].paused_type != 3)) // all active players except captain
         {
            // distance to home position
            float dx = xh - mPlayer.syn[p].x;
            float dy = yh - mPlayer.syn[p].y;

            // break into 100 steps
            mPlayer.syn[p].xinc = dx / 100;
            mPlayer.syn[p].yinc = dy / 100;

           // set left right direction
           if (mPlayer.syn[p].xinc > 0) mPlayer.syn[p].left_right = 1;
           if (mPlayer.syn[p].xinc < 0) mPlayer.syn[p].left_right = 0;

           xh += 20; // next home position
         }
   }

   if (mGameState.level_done_mode == 29) // players seek and zoom out
   {
      if (mGameState.level_done_timer == 100) mLog.add(LOG_OTH_LEVEL_DONE, mGameState.level_done_mode, -1, 0,0,0,0,0,0,0,0,0,0, "Players seek and zoom out");
      for (int p=0; p<NUM_PLAYERS; p++)
         if ((mPlayer.syn[p].active) && (mPlayer.syn[p].paused_type != 3))
         {
            mPlayer.syn[p].x += mPlayer.syn[p].xinc;
            mPlayer.syn[p].y += mPlayer.syn[p].yinc;
         }
   }

   if (mGameState.level_done_mode == 28) // set up for rocket move
   {
      mLog.add(LOG_OTH_LEVEL_DONE, mGameState.level_done_mode, -1, 0,0,0,0,0,0,0,0,0,0, "Setup for rocket move");

      if (!mMain.headless_server)
      {
         // create bitmap of the background
         if (!cutscene_background) cutscene_background = al_create_bitmap(2000, 2000);
         al_set_target_bitmap(cutscene_background);
         al_clear_to_color(al_map_rgba(0,0,0,0));
         al_draw_bitmap(mBitmap.level_buffer, 0, 0, 0);

         // erase the rocket area
         al_draw_filled_rectangle(20, 0, 360, 1980, mColor.Black);
         al_convert_mask_to_alpha(cutscene_background, mColor.Black);
      }


      // actually erase everything else from level
      for (int i=0; i<100; i++) if (mEnemy.Ei[i][0] != 19) mEnemy.Ei[i][0] = 0; // enemies (except crew)
      for (int i=0; i<500; i++) if (mItem.item[i][0] != 6) mItem.item[i][0] = 0; // items (except orb)
      mShot.clear_shots();

      // blocks
      for (int x=16; x<100; x++)
         for (int y=0; y<100; y++)
            mLevel.l[x][y] = 0;
      for (int x=0; x<3; x++)
         for (int y=0; y<100; y++)
            mLevel.l[x][y] = 0;
      for (int x=0; x<100; x++) mLevel.l[x][0] = 0; // top line
      for (int x=0; x<100; x++) mLevel.l[x][99] = 0; // bottom line

      if (!mMain.headless_server)
      {
         mScreen.init_level_background();
         cutscene_accel = 1.0;
         cutscene_bg_x =  0.0;
      }
   }

   if (mGameState.level_done_mode == 27) // rocket move
   {
      if (mGameState.level_done_timer == 240) mLog.add(LOG_OTH_LEVEL_DONE, mGameState.level_done_mode, -1, 0,0,0,0,0,0,0,0,0,0, "Rocket move");
      if (!mMain.headless_server)
      {
         mScreen.get_new_background(1);
         cutscene_bg_x += cutscene_accel;
         cutscene_accel += 0.07;
         al_draw_bitmap(cutscene_background, 0, cutscene_bg_x, 0);
         mEnemy.draw_enemies();
         mPlayer.draw_players();
         mItem.draw_items();
         mScreen.draw_scaled_level_region_to_display();
         mScreen.draw_screen_overlay();
         al_flip_display();
      }
   }
   if (mGameState.level_done_mode == 26)
   {
      mLog.add(LOG_OTH_LEVEL_DONE, mGameState.level_done_mode, -1, 0,0,0,0,0,0,0,0,0,0, "Setup for zoom in");
      if (!mMain.headless_server)
      {
         mDisplay.set_custom_scale_factor(cutscene_original_zoom, 100); // set up for zoom in
      }
   }
   if (mGameState.level_done_mode == 25)
   {
      if (mGameState.level_done_timer == 100) mLog.add(LOG_OTH_LEVEL_DONE, mGameState.level_done_mode, -1, 0,0,0,0,0,0,0,0,0,0, "Zoom in");
   }
   if (mGameState.level_done_mode == 24) // jump to level done
   {
      mLog.add(LOG_OTH_LEVEL_DONE, mGameState.level_done_mode, -1, 0,0,0,0,0,0,0,0,0,0, "Jump to level done");
      mGameState.level_done_mode = 6;
      mGameState.level_done_timer = 0;
      mGameState.level_done_next_level = 1; // always go to overworld after beating the game
   }

   if (mGameState.level_done_mode == 9) // pause players and set up exit xyincs
   {
      mLog.add(LOG_OTH_LEVEL_DONE, mGameState.level_done_mode, -1, 0,0,0,0,0,0,0,0,0,0, "Pause player and setup exit xyincs");
      mScreen.add_player_text_overlay(mGameState.level_done_player, 2);

      // added this check 20260930 - do not run if client or server
      if ((!mNetgame.ima_client) && (!mNetgame.ima_server)) mLevel.add_play_data_record(mLevel.play_level, 1);

      if ((mGameMoves.autosave_game_on_level_done) && (mNetgame.ima_client) && (!mLoop.ff_state)) mNetgame.client_send_crfl();

      for (int p=0; p<NUM_PLAYERS; p++)
         if ((mPlayer.syn[p].active) && (mPlayer.syn[p].paused_type != 3))
         {
            mPlayer.syn[p].paused = 5; // set player paused

            // get distance between player and exit
            float dx = mGameState.level_done_x - mPlayer.syn[p].x;
            float dy = mGameState.level_done_y - mPlayer.syn[p].y;

            // get move
            mPlayer.syn[p].xinc = dx/60;
            mPlayer.syn[p].yinc = dy/60;

            // set left right direction
            if (mPlayer.syn[p].xinc > 0) mPlayer.syn[p].left_right = 1;
            if (mPlayer.syn[p].xinc < 0) mPlayer.syn[p].left_right = 0;
         }
   }
   if (mGameState.level_done_mode == 8) // players seek exit
   {
      if (mGameState.level_done_timer == 100) mLog.add(LOG_OTH_LEVEL_DONE, mGameState.level_done_mode, -1, 0,0,0,0,0,0,0,0,0,0, "Players seek exit");
      float fade = 0.3 + (float) mGameState.level_done_timer / 85; // 1 to .3 in 60 frames
      if (mSound.sound_on) al_set_mixer_gain(mSound.st_mixer, ((float)mSound.st_scaler / 9) * fade);
      for (int p=0; p<NUM_PLAYERS; p++)
         if ((mPlayer.syn[p].active) && (mPlayer.syn[p].paused_type != 3))
         {
            mPlayer.syn[p].x += mPlayer.syn[p].xinc;
            mPlayer.syn[p].y += mPlayer.syn[p].yinc;
         }
   }
   if (mGameState.level_done_mode == 7) // shrink and rotate
   {
      if (mGameState.level_done_timer == 20) mLog.add(LOG_OTH_LEVEL_DONE, mGameState.level_done_mode, -1, 0,0,0,0,0,0,0,0,0,0, "Players shrink and rotate");
      for (int p=0; p<NUM_PLAYERS; p++)
         if ((mPlayer.syn[p].active) && (mPlayer.syn[p].paused_type != 3))
         {
            mPlayer.syn[p].draw_scale -= 0.05;
            mPlayer.syn[p].draw_rot -= 8;
         }
   }


   if (mGameState.level_done_mode == 5) // skippable 15s timeout
   {
      if (mGameState.level_done_timer % 40 == 0)
      {
         int time_left = (mGameState.level_done_timer/40) - 1;
         mLog.add(LOG_OTH_LEVEL_DONE, mGameState.level_done_mode, -1, time_left,0,0,0,0,0,0,0,0,0, "Skippable 15s timeout ");
      }

      if (mDemoMode.play_mode_active)
      {
         if (mGameState.level_done_timer == 999) mGameState.level_done_timer++;
      }
      else if (!mNetgame.ima_client)
      {
         if (have_all_players_acknowledged()) mGameState.level_done_timer = 0; // skip
      }
   }

   if (mGameState.level_done_mode == 2) // delay to load next level
   {
      mLog.add(LOG_OTH_LEVEL_DONE, mGameState.level_done_mode, -1, 0,0,0,0,0,0,0,0,0,0, "Delay to load next level");
   }

   if (!mNetgame.ima_client)
      if (--mGameState.level_done_timer <= 0)
      {
         mGameState.level_done_mode--;
         if (mGameState.level_done_mode == 30) mGameState.level_done_timer = 0;   // set up for player move and zoom out
         if (mGameState.level_done_mode == 29) mGameState.level_done_timer = 100; // player move and zoom out
         if (mGameState.level_done_mode == 28) mGameState.level_done_timer = 0;   // set up for rocket move
         if (mGameState.level_done_mode == 27) mGameState.level_done_timer = 240; // rocket move
         if (mGameState.level_done_mode == 26) mGameState.level_done_timer = 0;   // set up for zoom in
         if (mGameState.level_done_mode == 25) mGameState.level_done_timer = 100; // zoom in
         if (mGameState.level_done_mode == 24) mGameState.level_done_timer = 0;   // jump to mode 6
         if (mGameState.level_done_mode == 8) mGameState.level_done_timer = 60;  // players seek exit
         if (mGameState.level_done_mode == 7) mGameState.level_done_timer = 20;  // players shrink and rotate into exit
         if (mGameState.level_done_mode == 6) mGameState.level_done_timer = 0;   // not used
         if (mGameState.level_done_mode == 5)
         {
            if (mDemoMode.play_mode_active)
            {
               if (mScreen.demo_controls_pause_when_done) mGameState.level_done_timer = 999; // special value to mark wait forever
               else mGameState.level_done_timer = 160; // skippable 4s delay;
            }
            else mGameState.level_done_timer = 600; // skippable 15s delay;
         }
         if (mGameState.level_done_mode == 4) mGameState.level_done_timer = 0;  // not used
         if (mGameState.level_done_mode == 3) mGameState.level_done_timer = 0;  // not used
         if (mGameState.level_done_mode == 2) mGameState.level_done_timer = 0;  // delay to load next level (was 10, lets try without it as of 20240602)
         if (mGameState.level_done_mode == 1)
         {
            mLog.add(LOG_OTH_LEVEL_DONE, mGameState.level_done_mode, -1, 0,0,0,0,0,0,0,0,0,0, "Load next level");
            state[0] = PM_PROGRAM_STATE_NEXT_LEVEL;
         }
      }
}


