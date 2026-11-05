// mwTransitions.cpp

#include "pm.h"
#include "mwTransitions.h"
#include "mwColor.h"
#include "mwDemoMode.h"
#include "mwDisplay.h"
#include "mwFont.h"
#include "mwGameMoves.h"
#include "mwGameState.h"
#include "mwInput.h"
#include "mwLevel.h"
#include "mwLog.h"
#include "mwLoop.h"
#include "mwMain.h"
#include "mwNetgame.h"
#include "mwScreen.h"
#include "mwSound.h"

mwTransitions mTransitions;



void mwTransitions::set_transition_speed()
{
   int val = 40;

   if (mGameState.transition_speed == 0) // fastest
   {
      val = 2;
   }
   if (mGameState.transition_speed == 1) // hasty
   {
      val = 16;
   }
   if (mGameState.transition_speed == 2) // normal
   {
      val = 32;
   }
   if (mGameState.transition_speed == 3) // relaxed
   {
      val = 64;
   }

   gate_enter_seek_time = val;
   gate_enter_rotate_time = val;
   //gate_enter_num_rotations = 4;
   //gate_enter_player_final_scale = .1;
   gate_enter_transition_time = val;


   level_done_seek_time = val;
   level_done_rotate_time = val;
   //level_done_num_rotations = 4;
   //level_done_player_final_scale = .4;
   level_done_transition_time = val;

   // min_scale = 0.0001;
   // scale_compound_interest_mode = 0;

   // these are set in level done mode 6 and used in mode 5
   // controls the player moving from level icon to gate
   // -----------------------------------------------------
   player_exit_transition_time = val;
   //p_num_rotations = 4;

   transition_num_steps = val;
   // transition_delay;


}
























void mwTransitions::proc_level_done_mode()
{
   set_transition_speed();
   //printf("[%4d] Level Done Mode:%d timer:%d\n", mLoop.frame_num, mGameState.level_done_mode, mGameState.level_done_timer);

   //-------------------------------------
   // start of final level rocket cutscene
   //-------------------------------------
   if (mGameState.level_done_mode == 40) // setup for players seek and zoom out
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
      mLevel.mPlayer.syn[c].xinc = 0;
      mLevel.mPlayer.syn[c].yinc = 0;

      // each player has its own home place on the ship
      int xh = 120;
      int yh = 260;
      for (int p=0; p< NUM_PLAYERS; p++)
         if ((mLevel.mPlayer.syn[p].active) && (p != c) && (mLevel.mPlayer.syn[p].paused_type != 3)) // all active players except captain
         {
            // distance to home position
            float dx = xh - mLevel.mPlayer.syn[p].x;
            float dy = yh - mLevel.mPlayer.syn[p].y;

            // break into 100 steps
            mLevel.mPlayer.syn[p].xinc = dx / 100;
            mLevel.mPlayer.syn[p].yinc = dy / 100;

           // set left right direction
           if (mLevel.mPlayer.syn[p].xinc > 0) mLevel.mPlayer.syn[p].left_right = 1;
           if (mLevel.mPlayer.syn[p].xinc < 0) mLevel.mPlayer.syn[p].left_right = 0;

           xh += 20; // next home position
         }
   }



   //-------------------------------------
   // players seek and zoom out
   //-------------------------------------
   if (mGameState.level_done_mode == 39)
   {
      if (mGameState.level_done_timer == 100) mLog.add(LOG_OTH_LEVEL_DONE, mGameState.level_done_mode, -1, 0,0,0,0,0,0,0,0,0,0, "Players seek and zoom out");
      for (int p=0; p<NUM_PLAYERS; p++)
         if ((mLevel.mPlayer.syn[p].active) && (mLevel.mPlayer.syn[p].paused_type != 3))
         {
            mLevel.mPlayer.syn[p].x += mLevel.mPlayer.syn[p].xinc;
            mLevel.mPlayer.syn[p].y += mLevel.mPlayer.syn[p].yinc;
         }
   }

   //-------------------------------------
   // set up for rocket move
   //-------------------------------------
   if (mGameState.level_done_mode == 38)
   {
      mLog.add(LOG_OTH_LEVEL_DONE, mGameState.level_done_mode, -1, 0,0,0,0,0,0,0,0,0,0, "Setup for rocket move");

      if (!mMain.headless_server)
      {
         // create bitmap of the background
         if (!cutscene_background) cutscene_background = al_create_bitmap(2000, 2000);
         al_set_target_bitmap(cutscene_background);
         al_clear_to_color(al_map_rgba(0,0,0,0));
         al_draw_bitmap(mLevel.level_buffer, 0, 0, 0);

         // erase the rocket area
         al_draw_filled_rectangle(20, 0, 360, 1980, mColor.Black);
         al_convert_mask_to_alpha(cutscene_background, mColor.Black);
      }


      // actually erase everything else from level
      for (int i=0; i<100; i++) if (mLevel.mEnemy.Ei[i][0] != 19) mLevel.mEnemy.Ei[i][0] = 0; // enemies (except crew)
      for (int i=0; i<500; i++) if (mLevel.mItem.item[i][0] != 6) mLevel.mItem.item[i][0] = 0; // items (except orb)
      mLevel.mShot.clear_shots();

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
         mLevel.init_level_background();
         cutscene_accel = 1.0;
         cutscene_bg_x =  0.0;
      }
   }


   //-------------------------------------
   // rocket move
   //-------------------------------------
   if (mGameState.level_done_mode == 37)
   {
      if (mGameState.level_done_timer == 240) mLog.add(LOG_OTH_LEVEL_DONE, mGameState.level_done_mode, -1, 0,0,0,0,0,0,0,0,0,0, "Rocket move");
      if (!mMain.headless_server)
      {
         mLevel.get_new_background();
         cutscene_bg_x += cutscene_accel;
         cutscene_accel += 0.07;
         al_draw_bitmap(cutscene_background, 0, cutscene_bg_x, 0);
         mLevel.mEnemy.draw_enemies();
         mLevel.mPlayer.draw_players(1);
         al_set_target_bitmap(mLevel.level_buffer);
         mLevel.mItem.draw_items();
         mLevel.draw_scaled_level_region_to_display();
         mScreen.draw_screen_overlay();
         al_flip_display();
      }
   }

   //-------------------------------------
   // setup for zoom in
   //-------------------------------------
   if (mGameState.level_done_mode == 36)
   {
      mLog.add(LOG_OTH_LEVEL_DONE, mGameState.level_done_mode, -1, 0,0,0,0,0,0,0,0,0,0, "Setup for zoom in");
      if (!mMain.headless_server)
      {
         mDisplay.set_custom_scale_factor(cutscene_original_zoom, 100); // set up for zoom in
      }
   }

   //-------------------------------------
   // zoom in
   //-------------------------------------
   if (mGameState.level_done_mode == 35)
   {
      if (mGameState.level_done_timer == 100) mLog.add(LOG_OTH_LEVEL_DONE, mGameState.level_done_mode, -1, 0,0,0,0,0,0,0,0,0,0, "Zoom in");
   }

   //-------------------------------------
   // jump to level done skippable timer
   //-------------------------------------
   if (mGameState.level_done_mode == 34)
   {
      mLog.add(LOG_OTH_LEVEL_DONE, mGameState.level_done_mode, -1, 0,0,0,0,0,0,0,0,0,0, "Jump to level done");
      mGameState.level_done_mode = 9;
      mGameState.level_done_timer = 0;

      // always go to overworld after beating the game
      mGameState.level_done_next_level = 1;

      // set so we will be in front of rocket level
      mLevel.mPlayer.syn[0].overworld_last_touched_gate_level = 64;




   }






// -----------------------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------------------------
// gate enter
// -----------------------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------------------------



   // entry point
   // -------------------------------------
   if (mGameState.level_done_mode == 29)
   {
      mLog.add(LOG_OTH_LEVEL_DONE, mGameState.level_done_mode, -1, 0,0,0,0,0,0,0,0,0,0, "Entry point");
      mGameState.level_done_timer = 0;
   }

   // partially open door
   // -------------------------------------
   if (mGameState.level_done_mode == 28)
   {
      // only log once
      if (mGameState.level_done_timer == 10) mLog.add(LOG_OTH_LEVEL_DONE, mGameState.level_done_mode, -1, 0,0,0,0,0,0,0,0,0,0, "Partially open door");
   }


   // set up players seek gate
   // -------------------------------------
   if (mGameState.level_done_mode == 27)
   {

      mLog.add(LOG_OTH_LEVEL_DONE, mGameState.level_done_mode, -1, 0,0,0,0,0,0,0,0,0,0, "Set up players seek gate");

      // get gate item
      int gate_item = mGameState.gate_enter_item;

      // load next level into l2
      int next_level = mLevel.mItem.item[gate_item][6];
      l2.load_level_l2(next_level);

      // get size and position of hovered level icon (relative to mLevel)
      float size = mLevel.mItem.item[gate_item][11];
      float lix1 = mLevel.mItem.item[gate_item][4] - size/2 + 10 - 10;
      float liy1 = mLevel.mItem.item[gate_item][5] - size   + 19 - 10;


      // player's start position in l2 (centered)
      float l2_px = l2.mPlayer.syn[0].x+10;
      float l2_py = l2.mPlayer.syn[0].y+10;

      // scale l2 start position by l2 icon size
      float l2_px_scaled = l2_px / 2000 * size;
      float l2_py_scaled = l2_py / 2000 * size;

      // 0-2000 position in mLevel of the next level's start position in the level icon
      float sx = lix1 + l2_px_scaled;
      float sy = liy1 + l2_py_scaled;


      // set players paused and setup incs
      for (int p=0; p<NUM_PLAYERS; p++)
         if ((mLevel.mPlayer.syn[p].active) && (mLevel.mPlayer.syn[p].paused_type != 3))
         {
            mLevel.mPlayer.syn[p].paused = 5; // set player paused

            // get distance between players current position and new position
            float dx = sx - mLevel.mPlayer.syn[p].x;
            float dy = sy - mLevel.mPlayer.syn[p].y;

            // set incs
            mLevel.mPlayer.syn[p].xinc = dx / gate_enter_seek_time;
            mLevel.mPlayer.syn[p].yinc = dy / gate_enter_seek_time;

            // set left right direction
            if (mLevel.mPlayer.syn[p].xinc > 0) mLevel.mPlayer.syn[p].left_right = 1;
            if (mLevel.mPlayer.syn[p].xinc < 0) mLevel.mPlayer.syn[p].left_right = 0;
         }

   }

   // players seek gate
   // -------------------------------------
   if (mGameState.level_done_mode == 26)
   {
      // only log once
      if (mGameState.level_done_timer == gate_enter_seek_time) mLog.add(LOG_OTH_LEVEL_DONE, mGameState.level_done_mode, -1, 0,0,0,0,0,0,0,0,0,0, "Players seek gate");



      for (int p=0; p<NUM_PLAYERS; p++)
         if ((mLevel.mPlayer.syn[p].active) && (mLevel.mPlayer.syn[p].paused_type != 3))
         {
            mLevel.mPlayer.syn[p].x += mLevel.mPlayer.syn[p].xinc;
            mLevel.mPlayer.syn[p].y += mLevel.mPlayer.syn[p].yinc;
         }
   }


   // shrink and rotate
   // -------------------------------------
   if (mGameState.level_done_mode == 24)
   {
      // only log once
      if (mGameState.level_done_timer == gate_enter_rotate_time) mLog.add(LOG_OTH_LEVEL_DONE, mGameState.level_done_mode, -1, 0,0,0,0,0,0,0,0,0,0, "Shrink and rotate");



      float scale_inc = (1.0 - gate_enter_player_final_scale) / gate_enter_rotate_time;
      float rot_inc = gate_enter_num_rotations * (ALLEGRO_PI * 2) / gate_enter_rotate_time;

      for (int p=0; p<NUM_PLAYERS; p++)
         if ((mLevel.mPlayer.syn[p].active) && (mLevel.mPlayer.syn[p].paused_type != 3))
         {
            mLevel.mPlayer.syn[p].draw_scale -= scale_inc;
            mLevel.mPlayer.syn[p].draw_rot -= rot_inc;
         }

      // pause at end...
      //if (mGameState.level_done_timer == 1) mInput.tsw();
   }

   // setup for transition
   // -------------------------------------
   if (mGameState.level_done_mode == 23)
   {

      mLog.add(LOG_OTH_LEVEL_DONE, mGameState.level_done_mode, -1, 0,0,0,0,0,0,0,0,0,0, "Setup for transition");


      // get gate item
      int i = mGameState.gate_enter_item;

      // load next level into l2
      l2.valid_level_loaded = 0; // force reload
      int next_level = mLevel.mItem.item[i][6];
      l2.load_level_l2(next_level);

      // redraw to draw player also
      if (!mMain.headless_server) l2.draw_level3(l2.level_buffer, next_level, 0, 0, 2000, 1, 1, 1, 1, 1, 0);


      // get size and position of hovered level icon (relative to mLevel)
      float size = mLevel.mItem.item[i][11];
      float lix1 = mLevel.mItem.item[i][4] - size/2 + 10;
      float liy1 = mLevel.mItem.item[i][5] - size   + 19;

      // player's start position in l2
      float l2_px = l2.mPlayer.syn[0].x;
      float l2_py = l2.mPlayer.syn[0].y;

      // scale l2 start position by l2 icon size
      float l2_px_scaled = l2_px / 2000 * size;
      float l2_py_scaled = l2_py / 2000 * size;

      // 0-2000 position in mLevel of the next level's start position in the level icon
      float sx = lix1 + l2_px_scaled;
      float sy = liy1 + l2_py_scaled;

      // convert to actual screen position
      float ex1=0, ey1=0;
      mLevel.calc_actual_screen_position(sx, sy, ex1, ey1);

      // set transition initial
      transition_initial_scale = mDisplay.scale_factor_current * (size / 2000.0);
      transition_initial_x = ex1;
      transition_initial_y = ey1;


      // convert l2 players start position to actual screen position
      l2.calc_actual_screen_position(l2_px, l2_py, ex1, ey1);

      // set transition final
      transition_final_scale = mDisplay.scale_factor_current;
      transition_final_x = ex1;
      transition_final_y = ey1;



      // get scale multiplier using compound interest formula
      transition_scale_multiplier = 1.0 + pow((transition_final_scale/transition_initial_scale), (1.0/gate_enter_transition_time) ) - 1;

      // get x and y incs
      transition_x_inc = (transition_final_x - transition_initial_x) / gate_enter_transition_time;
      transition_y_inc = (transition_final_y - transition_initial_y) / gate_enter_transition_time;

      // set initial values
      transition_current_x = transition_initial_x;
      transition_current_y = transition_initial_y;
      transition_current_scale = transition_initial_scale;

      mGameState.level_done_timer = gate_enter_transition_time;
      mGameState.level_done_mode = 22;
   }


   // do transition
   // -------------------------------------
   if (mGameState.level_done_mode == 22)
   {
      // only log once
      if (mGameState.level_done_timer == gate_enter_transition_time) mLog.add(LOG_OTH_LEVEL_DONE, mGameState.level_done_mode, -1, 0,0,0,0,0,0,0,0,0,0, "Do transition");


      if (!mMain.headless_server)
      {
         al_set_target_backbuffer(mDisplay.display);
         al_clear_to_color(al_map_rgb(0,0,0));

         mLevel.draw_scaled_level_region_to_display();
         mScreen.draw_screen_frame();

         l2.draw_level_centered_on_player_pos(transition_current_x, transition_current_y, transition_current_scale);
         al_flip_display();
      }
      transition_current_x += transition_x_inc;
      transition_current_y += transition_y_inc;
      transition_current_scale *= transition_scale_multiplier;
   }


























   // -----------------------------------
   // normal level done entry point
   // -----------------------------------




   // -----------------------------------
   // setup for players seek exit
   // -----------------------------------
   if (mGameState.level_done_mode == 12)
   {
      mLog.add(LOG_OTH_LEVEL_DONE, mGameState.level_done_mode, -1, 0,0,0,0,0,0,0,0,0,0, "Pause players and setup seek exit");
      mScreen.add_player_text_overlay(mGameState.level_done_player, 2);

      mLevel.add_play_data_record(mLevel.play_level, 1);

      if ((mGameMoves.autosave_game_on_level_done) && (mNetgame.ima_client) && (!mLoop.ff_state)) mNetgame.client_send_crfl();

      for (int p=0; p<NUM_PLAYERS; p++)
         if ((mLevel.mPlayer.syn[p].active) && (mLevel.mPlayer.syn[p].paused_type != 3))
         {
            mLevel.mPlayer.syn[p].paused = 5; // set player paused

            // get distance between player and exit
            float dx = mGameState.level_done_x - mLevel.mPlayer.syn[p].x;
            float dy = mGameState.level_done_y - mLevel.mPlayer.syn[p].y;

            // get move
            mLevel.mPlayer.syn[p].xinc = dx / level_done_seek_time;
            mLevel.mPlayer.syn[p].yinc = dy / level_done_seek_time;

            // set left right direction
            if (mLevel.mPlayer.syn[p].xinc > 0) mLevel.mPlayer.syn[p].left_right = 1;
            if (mLevel.mPlayer.syn[p].xinc < 0) mLevel.mPlayer.syn[p].left_right = 0;
         }

      mGameState.level_done_mode = 11;
      mGameState.level_done_timer = level_done_seek_time;
      mLog.add(LOG_OTH_LEVEL_DONE, mGameState.level_done_mode, -1, 0,0,0,0,0,0,0,0,0,0, "Players seek exit");
   }

   // -----------------------------------
   // players seek exit
   // -----------------------------------
   if (mGameState.level_done_mode == 11)
   {
      // fade sound
      float sound_fade = 0.3 + (float) mGameState.level_done_timer / level_done_seek_time;
      if (mSound.sound_on) al_set_mixer_gain(mSound.st_mixer, ((float)mSound.st_scaler / 9) * sound_fade);

      for (int p=0; p<NUM_PLAYERS; p++)
         if ((mLevel.mPlayer.syn[p].active) && (mLevel.mPlayer.syn[p].paused_type != 3))
         {
            mLevel.mPlayer.syn[p].x += mLevel.mPlayer.syn[p].xinc;
            mLevel.mPlayer.syn[p].y += mLevel.mPlayer.syn[p].yinc;
         }
   }


   // -----------------------------------
   // players shrink and rotate into exit
   // -----------------------------------
   if (mGameState.level_done_mode == 10)
   {
      // only log once
      if (mGameState.level_done_timer == level_done_rotate_time) mLog.add(LOG_OTH_LEVEL_DONE, mGameState.level_done_mode, -1, 0,0,0,0,0,0,0,0,0,0, "Players shrink and rotate");

      float scale_inc = (1.0 - level_done_player_final_scale) / level_done_rotate_time;
      float rot_inc = level_done_num_rotations * (ALLEGRO_PI * 2) / level_done_rotate_time;

      for (int p=0; p<NUM_PLAYERS; p++)
         if ((mLevel.mPlayer.syn[p].active) && (mLevel.mPlayer.syn[p].paused_type != 3))
         {
            mLevel.mPlayer.syn[p].draw_scale -= scale_inc;
            mLevel.mPlayer.syn[p].draw_rot -= rot_inc;
         }
   }


   // -----------------------------------
   // skippable 15s timeout
   // -----------------------------------
   if (mGameState.level_done_mode == 8)
   {
      // only log once per second
      if (mGameState.level_done_timer % 40 == 0)
      {
         int time_left = (mGameState.level_done_timer/40) - 1;
         mLog.add(LOG_OTH_LEVEL_DONE, mGameState.level_done_mode, -1, time_left,0,0,0,0,0,0,0,0,0, "Skippable 15s timeout ");
      }

      // pause when level done in demo mode
      if (mDemoMode.play_mode_active)
      {
         if (mGameState.level_done_timer == 999) mGameState.level_done_timer++;
      }
      // skip if all players have acknowledged
      else if (!mNetgame.ima_client)
      {
         if (mLevel.mPlayer.have_all_players_acknowledged()) mGameState.level_done_timer = 1; // skip
      }
   }

   // -----------------------------------
   // setup for transition
   // -----------------------------------
   if (mGameState.level_done_mode == 7)
   {
      mLog.add(LOG_OTH_LEVEL_DONE, mGameState.level_done_mode, -1, 0,0,0,0,0,0,0,0,0,0, "Setup for transition");

      // if demo mode playing, skip to the end
      if (mDemoMode.play_mode_active)
      {
         mGameState.level_done_mode = 1;
         return;
      }


      // player's position in current level (centered)
      float px = mLevel.mPlayer.syn[0].x + 10;
      float py = mLevel.mPlayer.syn[0].y + 10;


      // get initial position
      // -------------------------------------

      // get absolute screen position of player in current level
      transition_initial_scale = mDisplay.scale_factor_current;
      transition_initial_x = mLevel.screen_display_x + (px - mLevel.level_display_region_x) * transition_initial_scale;
      transition_initial_y = mLevel.screen_display_y + (py - mLevel.level_display_region_y) * transition_initial_scale;

      // default final position
      // -------------------------------------
      transition_final_scale = transition_initial_scale;
      transition_final_x = transition_initial_x;
      transition_final_y = transition_initial_y;

      int next_lev = mGameState.level_done_next_level;
      int curr_lev = mLevel.last_level_loaded;


      // --------------------------------------------------------------
      // level to level transition (no overworld)
      // --------------------------------------------------------------
      if (next_lev != 1)
      {

         // load the next level into l2
         l2.load_level_l2(next_lev);

         // draw l2 on its own level_buffer with players
         if (!mMain.headless_server) l2.draw_level3(l2.level_buffer, next_lev, 0, 0, 2000, 1, 1, 1, 1, 1, 0);

         // convert l2 players start position to actual screen position
         float ex1=0, ey1=0;
         l2.calc_actual_screen_position(l2.mPlayer.syn[0].x, l2.mPlayer.syn[0].y, ex1, ey1);

         // set transition final
         transition_final_x = ex1;
         transition_final_y = ey1;

         if (scale_compound_interest_mode) transition_scale_multiplier = 1.0 + pow((min_scale/transition_initial_scale), (1.0/level_done_transition_time) ) - 1;

         mGameState.level_done_timer = level_done_transition_time;
         mGameState.level_done_mode = 4;

      }


      // --------------------------------------------------------------
      // level to overworld transition
      // --------------------------------------------------------------
      if (next_lev == 1) // overworld
      {
         // set all players last touched gate
         for (int p=0; p<NUM_PLAYERS; p++)
            l2.mPlayer.syn[p].overworld_last_touched_gate_level = curr_lev;

         // load the next level (overworld) into l2
         l2.load_level_l2(next_lev);

         // draw l2 on its own level_buffer without players
         if (!mMain.headless_server) l2.draw_level3(l2.level_buffer, 1, 0, 0, 2000, 1, 1, 1, 1, 0, 0);

         // draw mLevel on its own level_buffer with players
         if (!mMain.headless_server) mLevel.draw_level2(mLevel.level_buffer, 0, 0, 2000, 1, 1, 1, 1, 1, 0);

         // find the gate item for current level
         int gate_index = l2.mItem.find_gate_for_level(curr_lev);
         if (gate_index == -1)
         {
            printf("mGameState.level_done_mode == 4 --  could not find gate with level:%d\n", curr_lev);
            return;
         }

         // get position of hovered level icon
         float size = l2.mItem.item[gate_index][11];
         float lix1 = l2.mItem.item[gate_index][4] - size/2 + 10;
         float liy1 = l2.mItem.item[gate_index][5] - size   + 19;

         // find actual screen position
         float ex1=0, ey1=0;
         l2.calc_actual_screen_position(lix1, liy1, ex1, ey1);

         // get new scaled size
         float esize = size * mDisplay.scale_factor_current;

         // add player position
         transition_final_x = ex1 + px / 2000 * esize;
         transition_final_y = ey1 + py / 2000 * esize;
         transition_final_scale = esize / 2000;

         // get scale multiplier using compound interest formula
         transition_scale_multiplier = 1.0 + pow((transition_final_scale/transition_initial_scale), (1.0/level_done_transition_time) ) - 1;

         mGameState.level_done_mode = 6;
         mGameState.level_done_timer = level_done_transition_time;


      //    if (0)
      //    {
      //       // debug
      //       // ---------------------------------------
      //       al_set_target_backbuffer(mDisplay.display);
      //       al_draw_text(mFont.pr16, mColor.White, mDisplay.SCREEN_W/2, mDisplay.SCREEN_H/2, ALLEGRO_ALIGN_CENTER, "SHOW FIRST CALCS" );
      //
      //       // show initial
      //       al_draw_circle(transition_initial_x, transition_initial_y, 12, mColor.Green, 1);
      //       al_draw_text(mFont.pixl, mColor.White, transition_initial_x, transition_initial_y+20, ALLEGRO_ALIGN_CENTER, "INITIAL" );
      //
      //       // show initial
      //       al_draw_circle(transition_final_x, transition_final_y, 12, mColor.Red, 1);
      //       al_draw_text(mFont.pixl, mColor.White, transition_final_x, transition_final_y+20, ALLEGRO_ALIGN_CENTER, "FINAL" );
      //
      //       // connect with line
      //       al_draw_line(transition_initial_x, transition_initial_y, epx, epy, mColor.Yellow, 2);
      //
      //       al_flip_display();
      //       mInput.tsw();
      //
      //
      //
      //       al_clear_to_color(al_map_rgb(0,0,0));
      //       mLevel.draw_scaled_level_buffer_centered_on_pos(transition_initial_x, transition_initial_y, px, py, transition_initial_scale);
      //
      //       al_draw_text(mFont.pr16, mColor.White, mDisplay.SCREEN_W/2, mDisplay.SCREEN_H/2, ALLEGRO_ALIGN_CENTER, "SHOW CENTERED ON INITIAL" );
      //
      //       // show initial
      //       al_draw_circle(transition_initial_x, transition_initial_y, 12, mColor.Green, 1);
      //       al_draw_text(mFont.pixl, mColor.White, transition_initial_x, transition_initial_y+20, ALLEGRO_ALIGN_CENTER, "INITIAL" );
      //
      //       // show initial
      //       al_draw_circle(transition_final_x, transition_final_y, 12, mColor.Red, 1);
      //       al_draw_text(mFont.pixl, mColor.White, transition_final_x, transition_final_y+20, ALLEGRO_ALIGN_CENTER, "FINAL" );
      //
      //       // connect with line
      //       al_draw_line(transition_initial_x, transition_initial_y, epx, epy, mColor.Yellow, 2);
      //
      //       al_flip_display();
      //       mInput.tsw();
      //
      //
      //       // debug show final
      //
      //       al_clear_to_color(al_map_rgb(0,0,0));
      //       l2.draw_scaled_level_region_to_display();
      //
      //       // draw the hovered gate
      //       l2.mItem.draw_level_icon_direct_to_screen(gate_index);
      //
      //       al_draw_text(mFont.pr16, mColor.White, mDisplay.SCREEN_W/2, mDisplay.SCREEN_H/2, ALLEGRO_ALIGN_CENTER, "SHOW ON L2 OVERWORLD" );
      //
      //       al_draw_circle(transition_initial_x, transition_initial_y, 12, mColor.Green, 1);
      //       al_draw_text(mFont.pixl, mColor.White, transition_initial_x, transition_initial_y+20, ALLEGRO_ALIGN_CENTER, "INITIAL" );
      //
      //       al_draw_circle(transition_final_x, transition_final_y, 12, mColor.Red, 1);
      //       al_draw_text(mFont.pixl, mColor.White, transition_final_x, transition_final_y+20, ALLEGRO_ALIGN_CENTER, "FINAL" );
      //
      //       al_draw_line(transition_initial_x, transition_initial_y, epx, epy, mColor.Yellow, 2);
      //
      //       al_flip_display();
      //       mInput.tsw();
      //
      //       al_clear_to_color(al_map_rgb(0,0,0));
      //       mLevel.draw_scaled_level_buffer_centered_on_pos(transition_final_x, transition_final_y, px, py, transition_final_scale);
      //
      //       al_draw_text(mFont.pr16, mColor.White, mDisplay.SCREEN_W/2, mDisplay.SCREEN_H/2, ALLEGRO_ALIGN_CENTER, "SHOW CENTERED ON FINAL" );
      //
      //       // show initial
      //       al_draw_circle(transition_initial_x, transition_initial_y, 12, mColor.Green, 1);
      //       al_draw_text(mFont.pixl, mColor.White, transition_initial_x, transition_initial_y+20, ALLEGRO_ALIGN_CENTER, "INITIAL" );
      //
      //       // show initial
      //       al_draw_circle(transition_final_x, transition_final_y, 12, mColor.Red, 1);
      //       al_draw_text(mFont.pixl, mColor.White, transition_final_x, transition_final_y+20, ALLEGRO_ALIGN_CENTER, "FINAL" );
      //
      //       // connect with line
      //       al_draw_line(transition_initial_x, transition_initial_y, epx, epy, mColor.Yellow, 2);
      //
      //       al_flip_display();
      //       mInput.tsw();
      //    }
         // printf("initial: %f   %f   %f \n", transition_initial_x, transition_initial_y, transition_initial_scale);
         // printf("final:   %f   %f   %f \n", transition_final_x, transition_final_y, transition_final_scale);
         // printf("inc:     %f   %f   %f \n", transition_x_inc, transition_y_inc, transition_scale_multiplier);

      }

      // get x and y incs
      transition_x_inc = (transition_final_x - transition_initial_x) / level_done_transition_time;
      transition_y_inc = (transition_final_y - transition_initial_y) / level_done_transition_time;

      // set initial values
      transition_current_x = transition_initial_x;
      transition_current_y = transition_initial_y;
      transition_current_scale = transition_initial_scale;
   }

   // --------------------------------------------
   // level to overworld transition
   // --------------------------------------------
   if (mGameState.level_done_mode == 6)
   {
      // only log once
      if (mGameState.level_done_timer == level_done_transition_time) mLog.add(LOG_OTH_LEVEL_DONE, mGameState.level_done_mode, -1, 0,0,0,0,0,0,0,0,0,0, "Level to overworld transition");



      // do the increments
      transition_current_x += transition_x_inc;
      transition_current_y += transition_y_inc;
      transition_current_scale *= transition_scale_multiplier;

      // draw
      if (!mMain.headless_server)
      {
         al_set_target_backbuffer(mDisplay.display);
         al_clear_to_color(al_map_rgb(0,0,0));

         // draw the overworld as background
         l2.draw_scaled_level_region_to_display();

         // draw the current level shrinking into place as the level icon on overworld
         mLevel.draw_scaled_level_buffer_centered_on_pos(transition_current_x, transition_current_y, mLevel.mPlayer.syn[0].x+10, mLevel.mPlayer.syn[0].y+10, transition_current_scale);

         // frame
         mScreen.draw_screen_frame();

         al_flip_display();
      }

      //if (mGameState.level_done_timer == level_done_transition_time) mInput.tsw(); // pause on first frame
      //if (mGameState.level_done_timer == 1) mInput.tsw(); // pause on last frame


      // last time in this mode, setup for next mode (move players from overworld icon to gate)
      if (mGameState.level_done_timer == 1)
      {
         // final player position in overworld (on gate icon for current level)
         px_final = l2.mPlayer.syn[0].x;
         py_final = l2.mPlayer.syn[0].y;

         // calculate initial player position

         // search l2 to find gate item for current level
         int gate_item = l2.mItem.find_gate_for_level(mLevel.last_level_loaded);

         // get size and position of hovered level icon
         float size = l2.mItem.item[gate_item][11];
         float lix1 = l2.mItem.item[gate_item][4] - size/2 + 10 - 10;
         float liy1 = l2.mItem.item[gate_item][5] - size   + 19 - 10;

         // player's current position in mLevel scaled by l2 hovered level icon size
         float px = mLevel.mPlayer.syn[0].x / 2000 * size;
         float py = mLevel.mPlayer.syn[0].y / 2000 * size;

         // 0-2000 position in l2 of the next level's start position in the level icon

         // initial player position
         px_initial = lix1 + px;
         py_initial = liy1 + py;

         // set players paused and setup incs
         for (int p=0; p<NUM_PLAYERS; p++)
            if ((l2.mPlayer.syn[p].active) && (l2.mPlayer.syn[p].paused_type != 3))
            {
               l2.mPlayer.syn[p].paused = 5; // set player paused

               // set incs
               px_inc = (px_final - px_initial) / player_exit_transition_time;
               py_inc = (py_final - py_initial) / player_exit_transition_time;

               // set left right direction
               if (l2.mPlayer.syn[p].xinc > 0) l2.mPlayer.syn[p].left_right = 1;
               if (l2.mPlayer.syn[p].xinc < 0) l2.mPlayer.syn[p].left_right = 0;
            }


         // get initial player scale from mLevel, scaled by level icon size
         pscale_initial = mLevel.mPlayer.syn[0].draw_scale / 2000 * size;
         pscale_final = 1.0;
         pscale_inc = (pscale_final - pscale_initial) / player_exit_transition_time;

         // get rotation increment
         prot_inc = p_num_rotations * (ALLEGRO_PI * 2) / player_exit_transition_time;

         // set current to initial
         px_current = px_initial;
         py_current = py_initial;
         pscale_current = pscale_initial;

         mGameState.level_done_mode = 5;
         mGameState.level_done_timer = player_exit_transition_time;

      }
   }

   // --------------------------------------------
   // move players from overworld icon to gate
   // --------------------------------------------
   if (mGameState.level_done_mode == 5)
   {
      // only log once
      if (mGameState.level_done_timer == player_exit_transition_time) mLog.add(LOG_OTH_LEVEL_DONE, mGameState.level_done_mode, -1, 0,0,0,0,0,0,0,0,0,0, "Move players from overworld icon to gate");
      // do the incs
      px_current += px_inc;
      py_current += py_inc;
      pscale_current += pscale_inc;


      // set player variables to final before drawing background (overworld)
      for (int p=0; p<NUM_PLAYERS; p++)
         if ((l2.mPlayer.syn[p].active) && (l2.mPlayer.syn[p].paused_type != 3))
         {
            l2.mPlayer.syn[p].x = px_final;
            l2.mPlayer.syn[p].y = py_final;
         }

      if (!mMain.headless_server)
      {
         // redraw l2 on its own level_buffer
         l2.draw_level3(l2.level_buffer, 1, 0, 0, 2000, 1, 1, 1, 1, 0, 0);

         // draw hovered level icon frame and title on l2_buffer
         int i = l2.mItem.find_gate_for_level(mLevel.last_level_loaded);
         l2.mItem.draw_gate_hover_info(i);

         // draw l2 as background
         al_set_target_backbuffer(mDisplay.display);
         l2.draw_scaled_level_region_to_display();

         // draw direct to screen, hovered level icon in overworld for current level
         l2.mItem.draw_level_icon_direct_to_screen(i);

      }

      // set player variables to current before drawing players
      for (int p=0; p<NUM_PLAYERS; p++)
         if ((l2.mPlayer.syn[p].active) && (l2.mPlayer.syn[p].paused_type != 3))
         {
            l2.mPlayer.syn[p].x = px_current;
            l2.mPlayer.syn[p].y = py_current;
            l2.mPlayer.syn[p].draw_scale = pscale_current;
            l2.mPlayer.syn[p].draw_rot -= prot_inc;
         }

      if (!mMain.headless_server)
      {
         l2.mPlayer.draw_players_direct_to_screen();
         mScreen.draw_screen_frame();
         al_flip_display();
      }

     //if (mGameState.level_done_timer == player_exit_transition_time) mInput.tsw(); // pause on first frame
   }




   // --------------------------------------------
   // level to zero transition
   // --------------------------------------------
   if (mGameState.level_done_mode == 4)
   {

      // only log once
      if (mGameState.level_done_timer == level_done_transition_time) mLog.add(LOG_OTH_LEVEL_DONE, mGameState.level_done_mode, -1, 0,0,0,0,0,0,0,0,0,0, "Level to zero transition");


      //if (mGameState.level_done_timer == level_done_transition_time) mInput.tsw(); // pause on first frame

      transition_current_x += transition_x_inc / 2;
      transition_current_y += transition_y_inc / 2;

      if (scale_compound_interest_mode) transition_current_scale *= transition_scale_multiplier;
      else
      {
         float ratio = (float)mGameState.level_done_timer / (float)(level_done_transition_time);
         transition_current_scale = ratio * transition_initial_scale;
      }

      al_set_target_backbuffer(mDisplay.display);
      al_clear_to_color(al_map_rgb(0,0,0));
      mLevel.draw_scaled_level_buffer_centered_on_pos(transition_current_x, transition_current_y, mLevel.mPlayer.syn[0].x+10, mLevel.mPlayer.syn[0].y+10, transition_current_scale);
      al_flip_display();

//      if (mGameState.level_done_timer == level_done_transition_time) mInput.tsw(); // pause on first frame
//      if (mGameState.level_done_timer == 1) mInput.tsw(); // pause on last frame


      // last time in this mode, setup for next mode
      if (mGameState.level_done_timer == 1)
      {
         // get scale multiplier using compound interest formula
         if (scale_compound_interest_mode) transition_scale_multiplier = 1.0 + pow((transition_final_scale/min_scale), (1.0/level_done_transition_time) ) - 1;

         mGameState.level_done_mode = 3;
         mGameState.level_done_timer = level_done_transition_time;
      }
   }


   // zero to level transition
   if (mGameState.level_done_mode == 3)
   {
      // only log once
      if (mGameState.level_done_timer == level_done_transition_time) mLog.add(LOG_OTH_LEVEL_DONE, mGameState.level_done_mode, -1, 0,0,0,0,0,0,0,0,0,0, "Zero to level transition");


      //if (mGameState.level_done_timer == level_done_transition_time) mInput.tsw(); // pause on first frame
      transition_current_x += transition_x_inc / 2;
      transition_current_y += transition_y_inc / 2;

      if (scale_compound_interest_mode) transition_current_scale *= transition_scale_multiplier;
      else
      {
         float ratio = (float)mGameState.level_done_timer / (float)(level_done_transition_time);
         transition_current_scale = (1-ratio) * transition_initial_scale;
      }

      al_set_target_backbuffer(mDisplay.display);
      al_clear_to_color(al_map_rgb(0,0,0));
      l2.draw_scaled_level_buffer_centered_on_pos(transition_current_x, transition_current_y, l2.mPlayer.syn[0].x+10, l2.mPlayer.syn[0].y+10, transition_current_scale);
      al_flip_display();

//      if (mGameState.level_done_timer == level_done_transition_time) mInput.tsw(); // pause on first frame
//      if (mGameState.level_done_timer == 1) mInput.tsw(); // pause on last frame

      // if done, set next mode
      if (mGameState.level_done_timer == 1)
      {
         mGameState.level_done_mode = 2;
      }
   }

   bool enter_gate = 0;

   // client does not set these, server does, and they are synced to client
   if (!mNetgame.ima_client)
   {
      if (--mGameState.level_done_timer <= 0)
      {
         mGameState.level_done_mode--;
         if (mGameState.level_done_mode == 40) mGameState.level_done_timer = 0;   // set up for player move and zoom out
         if (mGameState.level_done_mode == 39) mGameState.level_done_timer = 100; // player move and zoom out
         if (mGameState.level_done_mode == 38) mGameState.level_done_timer = 0;   // set up for rocket move
         if (mGameState.level_done_mode == 37) mGameState.level_done_timer = 240; // rocket move
         if (mGameState.level_done_mode == 36) mGameState.level_done_timer = 0;   // set up for zoom in
         if (mGameState.level_done_mode == 35) mGameState.level_done_timer = 100; // zoom in
         if (mGameState.level_done_mode == 34) mGameState.level_done_timer = 0;


         if (mGameState.level_done_mode == 28) mGameState.level_done_timer = 10; // partially open door
         if (mGameState.level_done_mode == 27) mGameState.level_done_timer = 0;  // setup for players seek gate
         if (mGameState.level_done_mode == 26) mGameState.level_done_timer = gate_enter_seek_time; // players seek gate
         if (mGameState.level_done_mode == 25) mGameState.level_done_timer = 0; // setup players shrink and rotate
         if (mGameState.level_done_mode == 24) mGameState.level_done_timer = gate_enter_rotate_time; // players shrink and rotate
         if (mGameState.level_done_mode == 21) enter_gate = 1;


         if (mGameState.level_done_mode == 10) mGameState.level_done_timer = level_done_rotate_time;



         if (mGameState.level_done_mode == 8)
         {
            if (mDemoMode.play_mode_active)
            {
               if (mScreen.demo_controls_pause_when_done) mGameState.level_done_timer = 999; // special value to mark wait forever
               else mGameState.level_done_timer = 160; // skippable 4s delay;
            }
            else mGameState.level_done_timer = 600; // skippable 15s delay;
         }

         if (mGameState.level_done_mode == 4)
         {
            if (mGameState.level_done_next_level == 1) mGameState.level_done_mode = 1; // game to overworld mode, skip to final
         }
      }
   }


   if (enter_gate)
   {

      mLog.add(LOG_OTH_LEVEL_DONE, mGameState.level_done_mode, -1, 0,0,0,0,0,0,0,0,0,0, "Enter gate");

      int lev = mLevel.mItem.item[mGameState.gate_enter_item][6];
      mGameState.level_done_next_level = lev;
      // server sets last touched gate for all clients
      if (mNetgame.ima_server)
      {
         for (int pp=1; pp<8; pp++)
            if (mLevel.mPlayer.syn[pp].active) mLevel.mPlayer.syn[pp].overworld_last_touched_gate_level = lev;
      }

      mLoop.quit_action = 2;
      mLoop.done_action = 2;


      mGameState.level_done_mode = 1;

   }





   if (mGameState.level_done_mode == 1)
   {
      mLog.add(LOG_OTH_LEVEL_DONE, mGameState.level_done_mode, -1, 0,0,0,0,0,0,0,0,0,0, "Load next level");
      mLoop.state[0] = PM_PROGRAM_STATE_NEXT_LEVEL;
   }

}











// blocking transitions (270)
// -------------------------------------
void mwTransitions::transition_cutscene(int initial, int final, bool show_player)
{
   set_transition_speed();
   // printf("transition_cutscene(i:%d f:%d\n", i, f);

   if (mMain.headless_server) return;
   if (mNetgame.active()) return;
   if ((!initial) && (!final)) return; // if both are zero

   mLog.add(LOG_OTH_TRANSITIONS, 0, -1, initial, final);

   transition_initial_x = 0;
   transition_initial_y = 0;
   transition_initial_scale = 1;

   transition_final_x = 0;
   transition_final_y = 0;
   transition_final_scale = 1;


   mLevel.set_screen_display_variables();
   mLevel.set_level_display_region_xy();
   mScreen.set_map_var();

   // player's position in level
   float px = mLevel.mPlayer.syn[mLevel.mPlayer.active_local_player].x;
   float py = mLevel.mPlayer.syn[mLevel.mPlayer.active_local_player].y;

   // redraw to add players
   if (show_player) mLevel.draw_level2(mLevel.level_buffer, 0, 0, 2000, 1, 1, 1, 1, 1, 0);
   else             mLevel.draw_level2(mLevel.level_buffer, 0, 0, 2000, 1, 1, 1, 1, 0, 0);

   if (initial == 0) // nothing
   {
      transition_initial_x = mDisplay.SCREEN_W/2;
      transition_initial_y = mDisplay.SCREEN_H/2;
      transition_initial_scale = 0.005;
   }
   if (final == 0) // nothing
   {
      transition_final_x = mDisplay.SCREEN_W/2;
      transition_final_y = mDisplay.SCREEN_H/2;
      transition_final_scale = 0.005;
   }
   if (initial == 1) // game
   {
      transition_initial_scale = mDisplay.scale_factor_current;
      transition_initial_x = mLevel.screen_display_x + (px - mLevel.level_display_region_x) * transition_initial_scale;
      transition_initial_y = mLevel.screen_display_y + (py - mLevel.level_display_region_y) * transition_initial_scale;
   }
   if (final == 1) // game
   {
      transition_final_scale = mDisplay.scale_factor_current;
      transition_final_x = mLevel.screen_display_x + (px - mLevel.level_display_region_x) * transition_final_scale;
      transition_final_y = mLevel.screen_display_y + (py - mLevel.level_display_region_y) * transition_final_scale;
   }
   if (initial == 2) // menu
   {
      transition_initial_scale = (float)mScreen.menu_level_display_size / 2000;
      transition_initial_x = mScreen.menu_level_display_x + px * transition_initial_scale;
      transition_initial_y = mScreen.menu_level_display_y + py * transition_initial_scale;
   }
   if (final == 2) // menu
   {
       // this is to show gate icon and normal level icon
       // they are normally hidden when gate is hovered
      if (mLevel.last_level_loaded == 1)
      {
         mLevel.mPlayer.syn[0].hovered_gate = -1;
         mLevel.draw_level2(mLevel.level_buffer, 0, 0, 2000, 1, 1, 1, 1, 1, 0);
      }

      transition_final_scale = (float)mScreen.menu_level_display_size / 2000;
      transition_final_x = mScreen.menu_level_display_x + px * transition_final_scale;
      transition_final_y = mScreen.menu_level_display_y + py * transition_final_scale;
   }
   if (initial == 3) // gate
   {
      // this will only ever be used for single player starting demo from overworld gate
      // mLevel already has next level loaded

      // get level
      int lev = mLevel.last_level_loaded;


      //
      // float old_level_display_region_x = mLevel.level_display_region_x;
      // float old_level_display_region_y = mLevel.level_display_region_y;
      //
      //
      //
      // mLevel.valid_level_loaded = 0;
      // mLevel.load_level_by_number(lev, 1, 1);
      //
      // mLevel.level_display_region_x = old_level_display_region_x;
      // mLevel.level_display_region_y = old_level_display_region_y;
      //
      //
      // // draw mLevel on its own level_buffer without players
      // mLevel.draw_level2(mLevel.level_buffer, 0, 0, 2000, 1, 1, 1, 1, 0, 0);
      //
      //




      // load l2 with overworld to get the player's position in the level icon
      l2.valid_level_loaded = 0;

      l2.load_level_l2(1);

      // draw l2 on its own level_buffer without players
      l2.draw_level3(l2.level_buffer, 1, 0, 0, 2000, 1, 1, 1, 1, 0, 0);

      int gate_index = l2.mItem.find_gate_for_level(lev);

      // get size and position of hovered level icon (relative to mLevel)
      float size = l2.mItem.item[gate_index][11];
      float lix1 = l2.mItem.item[gate_index][4] - size/2 + 10;
      float liy1 = l2.mItem.item[gate_index][5] - size   + 19;

      // player's start position in l2
      float l2_px = l2.mPlayer.syn[0].x;
      float l2_py = l2.mPlayer.syn[0].y;

      // scale l2 start position by l2 icon size
      float l2_px_scaled = l2_px / 2000 * size;
      float l2_py_scaled = l2_py / 2000 * size;

      // 0-2000 position in mLevel of the next level's start position in the level icon
      lix1 += l2_px_scaled;
      liy1 += l2_py_scaled;

      // convert to actual screen position
      l2.calc_actual_screen_position(lix1, liy1, transition_initial_x, transition_initial_y);

      transition_initial_scale = mDisplay.scale_factor_current / 2000 * size;

   }
   if (final == 3) // gate
   {
      // this is used for single player quit level to overworld and demo quit to overworld
      // load overworld level into l2 and find out where player will be

      // set gate level last touched, so player's position in l2 is set by that gate
      l2.mPlayer.syn[0].overworld_last_touched_gate_level = mLevel.last_level_loaded;

      // force reload
      l2.valid_level_loaded = 0;

      // load overworld into l2
      l2.load_level_l2(1);

      // find the gate item for current level
      int gate_index = l2.mItem.find_gate_for_level(mLevel.last_level_loaded);

      // get position of hovered level icon
      float size = l2.mItem.item[gate_index][11];
      float lix1 = l2.mItem.item[gate_index][4] - size/2 + 10;
      float liy1 = l2.mItem.item[gate_index][5] - size   + 19;

      // add scaled player position from mLevel
      lix1 += px / 2000 * size;
      liy1 += py / 2000 * size;

      // get actual screen position
      l2.calc_actual_screen_position(lix1, liy1, transition_final_x, transition_final_y);

      // set scale
      transition_final_scale = mDisplay.scale_factor_current / 2000 * size;
   }

   // set incs
   transition_x_inc = (transition_final_x - transition_initial_x) / transition_num_steps;
   transition_y_inc = (transition_final_y - transition_initial_y) / transition_num_steps;

   // set scale multiplier using compound interest formula
   transition_scale_multiplier = 1.0 + pow((transition_final_scale/transition_initial_scale), (1.0/transition_num_steps) ) - 1;


   for (int s=0; s<transition_num_steps; s++)
   {
      transition_initial_scale *= transition_scale_multiplier;
      transition_initial_x += transition_x_inc;
      transition_initial_y += transition_y_inc;

      al_set_target_backbuffer(mDisplay.display);
      al_clear_to_color(al_map_rgb(0,0,0));
      mLevel.draw_level_centered_on_player_pos(transition_initial_x, transition_initial_y, transition_initial_scale);
      mScreen.draw_screen_overlay();
      al_flip_display();
      al_rest(0.025);
   }
}








   //debug draw
   //printf("do_transition(%f %f %f %f %f %f %f %f\n", fmxi, fmyi, fmxf, fmyf, sci, scf, num_steps, delay);
   //
   // al_set_target_backbuffer(mDisplay.display);
   // al_clear_to_color(al_map_rgb(0,0,0));
   //
   // al_draw_text(mFont.pr16, mColor.White, mDisplay.SCREEN_W/2, mDisplay.SCREEN_H/2, ALLEGRO_ALIGN_CENTER, "DEBUG" );
   //
   // al_draw_circle(fmxi, fmyi, 12, mColor.Green, 1);
   // al_draw_text(mFont.pixl, mColor.White, fmxi, fmyi+20, ALLEGRO_ALIGN_CENTER, "INITIAL" );
   //
   // al_draw_circle(fmxf, fmyf, 12, mColor.Red, 1);
   // al_draw_text(mFont.pixl, mColor.White, fmxf, fmyf+20, ALLEGRO_ALIGN_CENTER, "FINAL" );
   //
   // al_draw_line(fmxi, fmyi, fmxf, fmyf, mColor.Yellow, 2);
   //
   // al_flip_display();
   // mInput.tsw();



   // // debug draw initial
   // al_clear_to_color(al_map_rgb(0,0,0));
   // mLevel.draw_level_centered_on_player_pos(fmxi, fmyi, sci);
   // al_draw_text(mFont.pr16, mColor.White, mDisplay.SCREEN_W/2, mDisplay.SCREEN_H/2, ALLEGRO_ALIGN_CENTER, "INITIAL" );
   // al_flip_display();
   // mInput.tsw();
   //
   // // debug draw final
   // al_clear_to_color(al_map_rgb(0,0,0));
   // mLevel.draw_level_centered_on_player_pos(fmxf, fmyf, scf);
   // al_draw_text(mFont.pr16, mColor.White, mDisplay.SCREEN_W/2, mDisplay.SCREEN_H/2, ALLEGRO_ALIGN_CENTER, "FINAL" );
   // al_flip_display();
   // mInput.tsw();

   //mLevel.draw_level2(NULL, 0, 0, 0, 1, 1, 1, 1, 0, 0); // redraw entire level in case only region has been drawn
   //al_set_target_backbuffer(mDisplay.display);



