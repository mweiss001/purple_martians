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

#include "mwPlayer.h"
#include "mwEnemy.h"
#include "mwItem.h"
#include "mwLevel.h"
#include "mwShot.h"



void mwLoop::proc_level_done_mode(void)
{
//   mLog.addf(LOG_OTH_level_done, 0, "[%4d] Level Done Mode:%d\n", frame_num, mPlayer.syn[0].level_done_mode);

   //-------------------------------------
   // start of final level rocket cutscene
   //-------------------------------------
   if (mPlayer.syn[0].level_done_mode == 30) // setup for players seek and zoom out
   {
      mLog.addf(LOG_OTH_level_done, 0, "[%4d] Level Done Mode:%d - Setup for players seek and zoom out\n", frame_num, mPlayer.syn[0].level_done_mode);

      mLevel.add_play_data_record(mLevel.play_level, 1);


      mPlayer.syn[0].level_done_timer = 0; // immediate next mode
      cutscene_original_zoom = mDisplay.scale_factor_current;

      mDisplay.set_custom_scale_factor((float)(mDisplay.SCREEN_H - BORDER_WIDTH*2)/2000, 100);

      // bring other netgame players home
      int c = mPlayer.syn[0].level_done_player; // captain of the ship!
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

   if (mPlayer.syn[0].level_done_mode == 29) // players seek and zoom out
   {
      mLog.addf(LOG_OTH_level_done, 0, "[%4d] Level Done Mode:%d - Players seek and zoom out\n", frame_num, mPlayer.syn[0].level_done_mode);
      for (int p=0; p<NUM_PLAYERS; p++)
         if ((mPlayer.syn[p].active) && (mPlayer.syn[p].paused_type != 3))
         {
            mPlayer.syn[p].x += mPlayer.syn[p].xinc;
            mPlayer.syn[p].y += mPlayer.syn[p].yinc;
         }
   }

   if (mPlayer.syn[0].level_done_mode == 28) // set up for rocket move
   {
      mLog.addf(LOG_OTH_level_done, 0, "[%4d] Level Done Mode:%d - Setup for rocket move\n", frame_num, mPlayer.syn[0].level_done_mode);
      // create bitmap of the background
      if (!cutscene_background) cutscene_background = al_create_bitmap(2000, 2000);
      al_set_target_bitmap(cutscene_background);
      al_clear_to_color(al_map_rgba(0,0,0,0));
      al_draw_bitmap(mBitmap.level_buffer, 0, 0, 0);

      // erase the rocket area
      al_draw_filled_rectangle(20, 0, 380, 1980, mColor.Black);
      al_convert_mask_to_alpha(cutscene_background, mColor.Black);

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
      mScreen.init_level_background();

      cutscene_accel = 1.0;
      cutscene_bg_x =  0.0;

   }

   if (mPlayer.syn[0].level_done_mode == 27) // rocket move
   {
      mLog.addf(LOG_OTH_level_done, 0, "[%4d] Level Done Mode:%d - Rocket move\n", frame_num, mPlayer.syn[0].level_done_mode);
      mScreen.get_new_background(1);

      cutscene_bg_x += cutscene_accel;
      cutscene_accel += 0.07;
      al_draw_bitmap(cutscene_background, 0, cutscene_bg_x, 0);

      mEnemy.draw_enemies();
      mPlayer.draw_players();
      mItem.draw_items();

      mScreen.draw_scaled_level_region_to_display(0);
      mScreen.draw_screen_overlay();

      al_flip_display();

   }
   if (mPlayer.syn[0].level_done_mode == 26)
   {
      mLog.addf(LOG_OTH_level_done, 0, "[%4d] Level Done Mode:%d - Setup for zoom in\n", frame_num, mPlayer.syn[0].level_done_mode);
      mDisplay.set_custom_scale_factor(cutscene_original_zoom, 100); // set up for zoom in
   }
   if (mPlayer.syn[0].level_done_mode == 25)
   {
      mLog.addf(LOG_OTH_level_done, 0, "[%4d] Level Done Mode:%d - Zoom in\n", frame_num, mPlayer.syn[0].level_done_mode);
   }
   if (mPlayer.syn[0].level_done_mode == 24) // jump to level done
   {
      mLog.addf(LOG_OTH_level_done, 0, "[%4d] Level Done Mode:%d - Jump to level done\n", frame_num, mPlayer.syn[0].level_done_mode);
      mPlayer.syn[0].level_done_mode = 6;
      mPlayer.syn[0].level_done_timer = 0;
      mPlayer.syn[0].level_done_next_level = 1; // always go to overworld after beating the game
   }

   if (mPlayer.syn[0].level_done_mode == 9) // pause players and set up exit xyincs
   {
      mLog.addf(LOG_OTH_level_done, 0, "[%4d] Level Done Mode:%d - pause player and setup exit xyincs\n", frame_num, mPlayer.syn[0].level_done_mode);
      mScreen.add_player_text_overlay(mPlayer.syn[0].level_done_player, 2);
      mLevel.add_play_data_record(mLevel.play_level, 1);

      for (int p=0; p<NUM_PLAYERS; p++)
         if ((mPlayer.syn[p].active) && (mPlayer.syn[p].paused_type != 3))
         {
            mPlayer.syn[p].paused = 5; // set player paused

            // get distance between player and exit
            float dx = mPlayer.syn[0].level_done_x - mPlayer.syn[p].x;
            float dy = mPlayer.syn[0].level_done_y - mPlayer.syn[p].y;

            // get move
            mPlayer.syn[p].xinc = dx/60;
            mPlayer.syn[p].yinc = dy/60;

            // set left right direction
            if (mPlayer.syn[p].xinc > 0) mPlayer.syn[p].left_right = 1;
            if (mPlayer.syn[p].xinc < 0) mPlayer.syn[p].left_right = 0;
         }
   }
   if (mPlayer.syn[0].level_done_mode == 8) // players seek exit
   {
      mLog.addf(LOG_OTH_level_done, 0, "[%4d] Level Done Mode:%d - players seek exit\n", frame_num, mPlayer.syn[0].level_done_mode);
      float fade = 0.3 + (float) mPlayer.syn[0].level_done_timer / 85; // 1 to .3 in 60 frames
      if (mSound.sound_on) al_set_mixer_gain(mSound.st_mixer, ((float)mSound.st_scaler / 9) * fade);
      for (int p=0; p<NUM_PLAYERS; p++)
         if ((mPlayer.syn[p].active) && (mPlayer.syn[p].paused_type != 3))
         {
            mPlayer.syn[p].x += mPlayer.syn[p].xinc;
            mPlayer.syn[p].y += mPlayer.syn[p].yinc;
         }
   }
   if (mPlayer.syn[0].level_done_mode == 7) // shrink and rotate
   {
      mLog.addf(LOG_OTH_level_done, 0, "[%4d] Level Done Mode:%d - players shrink and rotate\n", frame_num, mPlayer.syn[0].level_done_mode);
      for (int p=0; p<NUM_PLAYERS; p++)
         if ((mPlayer.syn[p].active) && (mPlayer.syn[p].paused_type != 3))
         {
            mPlayer.syn[p].draw_scale -= 0.05;
            mPlayer.syn[p].draw_rot -= 8;
         }
   }
   if (mPlayer.syn[0].level_done_mode == 5) // skippable 15s timeout
   {
      mLog.addf(LOG_OTH_level_done, 0, "[%4d] Level Done Mode:%d - skippable 15s timeout\n", frame_num, mPlayer.syn[0].level_done_mode);
      if (!mNetgame.ima_client)
      {
         if (have_all_players_acknowledged()) mPlayer.syn[0].level_done_timer = 0; // skip
      }
   }

   if (mPlayer.syn[0].level_done_mode == 2) // delay to load next level
   {
      mLog.addf(LOG_OTH_level_done, 0, "[%4d] Level Done Mode:%d - delay to load next level\n", frame_num, mPlayer.syn[0].level_done_mode);
   }



   if (--mPlayer.syn[0].level_done_timer <= 0) // time to change to next level_done_mode
   {
      mPlayer.syn[0].level_done_mode--;

      if (mPlayer.syn[0].level_done_mode == 30) mPlayer.syn[0].level_done_timer = 0;   // set up for player move and zoom out
      if (mPlayer.syn[0].level_done_mode == 29) mPlayer.syn[0].level_done_timer = 100; // player move and zoom out
      if (mPlayer.syn[0].level_done_mode == 28) mPlayer.syn[0].level_done_timer = 0;   // set up for rocket move
      if (mPlayer.syn[0].level_done_mode == 27) mPlayer.syn[0].level_done_timer = 240; // rocket move
      if (mPlayer.syn[0].level_done_mode == 26) mPlayer.syn[0].level_done_timer = 0;   // set up for zoom in
      if (mPlayer.syn[0].level_done_mode == 25) mPlayer.syn[0].level_done_timer = 100; // zoom in
      if (mPlayer.syn[0].level_done_mode == 24) mPlayer.syn[0].level_done_timer = 0;   // jump to mode 6


      if (mPlayer.syn[0].level_done_mode == 8) mPlayer.syn[0].level_done_timer = 60;  // players seek exit
      if (mPlayer.syn[0].level_done_mode == 7) mPlayer.syn[0].level_done_timer = 20;  // players shrink and rotate into exit
      if (mPlayer.syn[0].level_done_mode == 6) mPlayer.syn[0].level_done_timer = 0;
      if (mPlayer.syn[0].level_done_mode == 5) mPlayer.syn[0].level_done_timer = 600; // skippable 15s delay;
      if (mPlayer.syn[0].level_done_mode == 4) mPlayer.syn[0].level_done_timer = 0;
      if (mPlayer.syn[0].level_done_mode == 3) mPlayer.syn[0].level_done_timer = 0;
      if (mPlayer.syn[0].level_done_mode == 2) mPlayer.syn[0].level_done_timer = 10;  // delay to load next level
      if (mPlayer.syn[0].level_done_mode == 1)
      {
         mLog.addf(LOG_OTH_level_done, 0, "[%4d] Level Done Mode:%d - Load lext level\n", frame_num, mPlayer.syn[0].level_done_mode);
         state[0] = PM_PROGRAM_STATE_NEXT_LEVEL;
      }
   }
}


