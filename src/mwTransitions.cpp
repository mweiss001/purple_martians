// mwTransitions.cpp

#include "pm.h"
#include "mwTransitions.h"

#include "mwBitmap.h"
#include "mwColor.h"
#include "mwDemoMode.h"
#include "mwDisplay.h"
#include "mwDrawSequence.h"
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
#include "mwShot.h"
#include "mwSound.h"

mwTransitions mTransitions;

mwTransitions::mwTransitions()
{
   //initialize();
}

void mwTransitions::initialize()
{

}


void mwTransitions::proc_level_done_mode()
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

   if (mGameState.level_done_mode == 29) // players seek and zoom out
   {
      if (mGameState.level_done_timer == 100) mLog.add(LOG_OTH_LEVEL_DONE, mGameState.level_done_mode, -1, 0,0,0,0,0,0,0,0,0,0, "Players seek and zoom out");
      for (int p=0; p<NUM_PLAYERS; p++)
         if ((mLevel.mPlayer.syn[p].active) && (mLevel.mPlayer.syn[p].paused_type != 3))
         {
            mLevel.mPlayer.syn[p].x += mLevel.mPlayer.syn[p].xinc;
            mLevel.mPlayer.syn[p].y += mLevel.mPlayer.syn[p].yinc;
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
         al_draw_bitmap(mLevel.level_buffer, 0, 0, 0);

         // erase the rocket area
         al_draw_filled_rectangle(20, 0, 360, 1980, mColor.Black);
         al_convert_mask_to_alpha(cutscene_background, mColor.Black);
      }


      // actually erase everything else from level
      for (int i=0; i<100; i++) if (mLevel.mEnemy.Ei[i][0] != 19) mLevel.mEnemy.Ei[i][0] = 0; // enemies (except crew)
      for (int i=0; i<500; i++) if (mLevel.mItem.item[i][0] != 6) mLevel.mItem.item[i][0] = 0; // items (except orb)
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
         mLevel.init_level_background();
         cutscene_accel = 1.0;
         cutscene_bg_x =  0.0;
      }
   }

   if (mGameState.level_done_mode == 27) // rocket move
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
            mLevel.mPlayer.syn[p].xinc = dx/60;
            mLevel.mPlayer.syn[p].yinc = dy/60;

            // set left right direction
            if (mLevel.mPlayer.syn[p].xinc > 0) mLevel.mPlayer.syn[p].left_right = 1;
            if (mLevel.mPlayer.syn[p].xinc < 0) mLevel.mPlayer.syn[p].left_right = 0;
         }
   }
   if (mGameState.level_done_mode == 8) // players seek exit
   {
      if (mGameState.level_done_timer == 100) mLog.add(LOG_OTH_LEVEL_DONE, mGameState.level_done_mode, -1, 0,0,0,0,0,0,0,0,0,0, "Players seek exit");
      float fade = 0.3 + (float) mGameState.level_done_timer / 85; // 1 to .3 in 60 frames
      if (mSound.sound_on) al_set_mixer_gain(mSound.st_mixer, ((float)mSound.st_scaler / 9) * fade);
      for (int p=0; p<NUM_PLAYERS; p++)
         if ((mLevel.mPlayer.syn[p].active) && (mLevel.mPlayer.syn[p].paused_type != 3))
         {
            mLevel.mPlayer.syn[p].x += mLevel.mPlayer.syn[p].xinc;
            mLevel.mPlayer.syn[p].y += mLevel.mPlayer.syn[p].yinc;
         }
   }
   if (mGameState.level_done_mode == 7) // shrink and rotate
   {
      if (mGameState.level_done_timer == 20) mLog.add(LOG_OTH_LEVEL_DONE, mGameState.level_done_mode, -1, 0,0,0,0,0,0,0,0,0,0, "Players shrink and rotate");
      for (int p=0; p<NUM_PLAYERS; p++)
         if ((mLevel.mPlayer.syn[p].active) && (mLevel.mPlayer.syn[p].paused_type != 3))
         {
            mLevel.mPlayer.syn[p].draw_scale -= 0.05;
            mLevel.mPlayer.syn[p].draw_rot -= 8;
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
            mLoop.state[0] = PM_PROGRAM_STATE_NEXT_LEVEL;
         }
      }
}


int mwTransitions::have_all_players_acknowledged()
{
   int ret = 1; // yes by default, if any have not then change to no

   for (int p=0; p<NUM_PLAYERS; p++)
   {
      if ((mLevel.mPlayer.syn[p].active) && (mLevel.mPlayer.syn[p].paused_type != 3))
      {
         if (mGameMoves.has_player_acknowledged(p))
         {
            mLevel.mPlayer.syn[p].level_done_ack = 1;
         }
         else
         {
            mLevel.mPlayer.syn[p].level_done_ack = 0;
            ret = 0;
         }
      }
   }
   return ret;
}














void mwTransitions::proc_gate_enter_mode()
{
   int enter_gate = 0;

   if (mGameState.gate_enter_mode == 9) mGameState.gate_enter_timer = 0; // entry point

   // pause players and set up gate seek xyincs
   if (mGameState.gate_enter_mode == 7)
   {
      int i = mGameState.gate_enter_item;

      int lev = mLevel.mItem.item[i][6];


      int x = mLevel.mItem.item[i][4];
      int y = mLevel.mItem.item[i][5];

      // level_icon size
      float size = mLevel.mItem.item[i][11];

      // level icon origin
      float lix = x-size/2 + 10;
      float liy = y-size   + 20;

      // load l2 if not already loaded and get start position
      l2.load_level_l2(lev);

      mTransitions.gate_enter_next_level_start_x = l2.mPlayer.syn[0].x;
      mTransitions.gate_enter_next_level_start_y = l2.mPlayer.syn[0].y;
      mTransitions.gate_enter_level_icon_size = size;

      for (int p=0; p<NUM_PLAYERS; p++)
         if ((mLevel.mPlayer.syn[p].active) && (mLevel.mPlayer.syn[p].paused_type != 3))
         {
            mLevel.mPlayer.syn[p].paused = 5; // set player paused

            // new level start position scaled by level_icon size
            float px = ((mTransitions.gate_enter_next_level_start_x+10) * mTransitions.gate_enter_level_icon_size) / 2000;
            float py = ((mTransitions.gate_enter_next_level_start_y+10) * mTransitions.gate_enter_level_icon_size) / 2000;

            // get screen position of start position
            float sx = lix + px - 9;
            float sy = liy + py - 9;

            // get distance between players current position and new position
            float dx = sx - mLevel.mPlayer.syn[p].x;
            float dy = sy - mLevel.mPlayer.syn[p].y;

            // get move
            mLevel.mPlayer.syn[p].xinc = dx/gate_enter_seek_time;
            mLevel.mPlayer.syn[p].yinc = dy/gate_enter_seek_time;

            // set left right direction
            if (mLevel.mPlayer.syn[p].xinc > 0) mLevel.mPlayer.syn[p].left_right = 1;
            if (mLevel.mPlayer.syn[p].xinc < 0) mLevel.mPlayer.syn[p].left_right = 0;
         }
   }


   // players seek gate
   if (mGameState.gate_enter_mode == 6)
   {
      for (int p=0; p<NUM_PLAYERS; p++)
         if ((mLevel.mPlayer.syn[p].active) && (mLevel.mPlayer.syn[p].paused_type != 3))
         {
            mLevel.mPlayer.syn[p].x += mLevel.mPlayer.syn[p].xinc;
            mLevel.mPlayer.syn[p].y += mLevel.mPlayer.syn[p].yinc;
         }
   }

   if (mGameState.gate_enter_mode == 5) // setup shrink, rotate and seek start pos of icon
   {

   }


   // shrink, rotate and seek start pos of icon
   if (mGameState.gate_enter_mode == 4)
   {
      float scale_inc = (1.0 - gate_enter_player_final_scale) / gate_enter_rotate_time;
      float rot_inc = gate_enter_num_rotations * (ALLEGRO_PI * 2) / gate_enter_rotate_time;
      for (int p=0; p<NUM_PLAYERS; p++)
         if ((mLevel.mPlayer.syn[p].active) && (mLevel.mPlayer.syn[p].paused_type != 3))
         {
            mLevel.mPlayer.syn[p].draw_scale -= scale_inc;
            mLevel.mPlayer.syn[p].draw_rot -= rot_inc;
         }


      // pause at end...
      ///if (mGameState.gate_enter_timer == 1) mInput.tsw();


   }

   // setup for transition
   if (mGameState.gate_enter_mode == 3)
   {
      // redraw to draw player also
      int lev = mLevel.mItem.item[mGameState.gate_enter_item][6];
      l2.draw_level3(l2.level_buffer, lev, 0, 0, 2000, 1, 1, 1, 1, 1, 0);



      // player's start position in next level
      float px = mTransitions.gate_enter_next_level_start_x;
      float py = mTransitions.gate_enter_next_level_start_y;

      transition_initial_scale = mDisplay.scale_factor_current * (gate_enter_level_icon_size / 2000.0);
      transition_initial_x = gate_transition_x + px * transition_initial_scale + 1;
      transition_initial_y = gate_transition_y + py * transition_initial_scale + 2;


      transition_final_scale = mDisplay.scale_factor_current;
      transition_final_x = l2.screen_display_x + (px - l2.level_display_region_x) * transition_final_scale;
      transition_final_y = l2.screen_display_y + (py - l2.level_display_region_y) * transition_final_scale;


      // get scale multiplier using compound interest formula
      transition_scale_multiplier = 1.0 + pow((transition_final_scale/transition_initial_scale), (1.0/transition_steps) ) - 1;

      // get x and y incs
      transition_x_inc = (transition_final_x - transition_initial_x) / transition_steps;
      transition_y_inc = (transition_final_y - transition_initial_y) / transition_steps;

      // set initial values
      transition_current_x = transition_initial_x;
      transition_current_y = transition_initial_y;
      transition_current_scale = transition_initial_scale;

      mGameState.gate_enter_timer = transition_steps;

      mGameState.gate_enter_mode = 2;

   }

   // do transition
   if (mGameState.gate_enter_mode == 2)
   {
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

      // pause on first frame
//      if (mGameState.gate_enter_timer == transition_steps) mInput.tsw();

   }



   if (!mNetgame.ima_client) // clients do not set this locally, must come from server
      if (--mGameState.gate_enter_timer <= 0)
      {
         mGameState.gate_enter_mode--;
         if (mGameState.gate_enter_mode == 8) mGameState.gate_enter_timer = 10; // partially open door
         if (mGameState.gate_enter_mode == 7) mGameState.gate_enter_timer = 0;  // setup for players seek gate
         if (mGameState.gate_enter_mode == 6) mGameState.gate_enter_timer = gate_enter_seek_time; // players seek gate
         if (mGameState.gate_enter_mode == 5) mGameState.gate_enter_timer = 0; // setup players shrink and rotate
         if (mGameState.gate_enter_mode == 4) mGameState.gate_enter_timer = gate_enter_rotate_time; // players shrink and rotate
//         if (mGameState.gate_enter_mode == 3) mGameState.gate_enter_timer = 0;  // setup for transition
//         if (mGameState.gate_enter_mode == 2) mGameState.gate_enter_timer = 0;  // do transition
         if (mGameState.gate_enter_mode == 1) enter_gate = 1;
      }

   if (enter_gate)
   {
      int i = mGameState.gate_enter_item;
      int lev = mLevel.mItem.item[i][6];

      // server sets last touched gate for all clients
      if (mNetgame.ima_server)
      {
         for (int pp=1; pp<8; pp++)
            if (mLevel.mPlayer.syn[pp].active) mLevel.mPlayer.syn[pp].overworld_last_touched_gate = lev;
      }

      // // immediate next level
      // mGameState.level_done_next_level = lev;
      // mLog.add(LOG_OTH_LEVEL_DONE, mGameState.level_done_mode, -1, 0,0,0,0,0,0,0,0,0,0, "Load next level");
      // state[0] = PM_PROGRAM_STATE_NEXT_LEVEL;

      // immediate next level

      // mGameState.level_done_mode = 3;
      // mGameState.level_done_timer = 0;
      mGameState.level_done_next_level = lev;

      mLog.add(LOG_OTH_LEVEL_DONE, mGameState.level_done_mode, -1, 0,0,0,0,0,0,0,0,0,0, "Load next level");
      mLoop.state[0] = PM_PROGRAM_STATE_NEXT_LEVEL;

  //    mInput.tsw();


      mLoop.quit_action = 2;
      mLoop.done_action = 2;

      mGameState.gate_enter_mode = 0;
   }
}






void mwTransitions::transition_cutscene(int i, int f)
{
   return;


   //if (!mMain.headless_server)
   //if (!mNetgame.active())
   {
      mLog.add(LOG_OTH_TRANSITIONS, 0, -1, i, f);

      if ((i) || (f)) // only if not both 0
      {
         int num_steps = transition_num_steps;
         float delay = (float)transition_delay/1000;

         float fmxi=0;
         float fmyi=0;
         float fmsi=1;

         float fmxf=0;
         float fmyf=0;
         float fmsf=1;

         if (mMain.headless_server)
         {
            do_transition(fmxi, fmyi, fmxf, fmyf, fmsi, fmsf, num_steps, delay);
            return;
         }

         mLevel.set_screen_display_variables();
         mLevel.set_level_display_region_xy();
         mScreen.set_map_var();


         // player's position in level
         float px = mLevel.mPlayer.syn[mLevel.mPlayer.active_local_player].x;
         float py = mLevel.mPlayer.syn[mLevel.mPlayer.active_local_player].y;

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
            fmxi = mLevel.screen_display_x + (px - mLevel.level_display_region_x) * fmsi;
            fmyi = mLevel.screen_display_y + (py - mLevel.level_display_region_y) * fmsi;
         }
         if (f == 1) // game
         {
            fmsf = mDisplay.scale_factor_current;
            fmxf = mLevel.screen_display_x + (px - mLevel.level_display_region_x) * fmsf;
            fmyf = mLevel.screen_display_y + (py - mLevel.level_display_region_y) * fmsf;
         }
         if (i == 2) // menu
         {
            fmsi = (float)mScreen.menu_level_display_size / 2000;
            fmxi = mScreen.menu_level_display_x + px * fmsi;
            fmyi = mScreen.menu_level_display_y + py * fmsi;
         }
         if (f == 2) // menu
         {
            fmsf = (float)mScreen.menu_level_display_size / 2000;
            fmxf = mScreen.menu_level_display_x + px * fmsf;
            fmyf = mScreen.menu_level_display_y + py * fmsf;
         }
         if (i == 3) // gate
         {
            fmsi = mDisplay.scale_factor_current * (gate_enter_level_icon_size / 2000.0); // level icon size = 200;
            fmxi = gate_transition_x + px * fmsi;
            fmyi = gate_transition_y + py * fmsi;
         }
         if (f == 3) // gate
         {
            // restore viewport so transition lines up
            mLevel.level_display_region_x = gate_transition_wx;
            mLevel.level_display_region_y = gate_transition_wy;
            //mDisplay.set_scale_factor(mScreen.gate_transition_scale, 1); // disabled for now, I want to keep scale if changed while playing

            fmsf = mDisplay.scale_factor_current * (gate_enter_level_icon_size / 2000.0); // level icon size = 200;
            fmxf = gate_transition_x + px * fmsf;
            fmyf = gate_transition_y + py * fmsf;
         }
         do_transition(fmxi, fmyi, fmxf, fmyf, fmsi, fmsf, num_steps, delay);
      }
   }
}


void mwTransitions::do_transition(float fmxi, float fmyi, float fmxf, float fmyf, float sci, float scf, float num_steps, float delay)
{
   return;


   printf("do_transition(%f %f %f %f %f %f %f %f\n", fmxi, fmyi, fmxf, fmyf, sci, scf, num_steps, delay);

   if (!mMain.headless_server)
   {
      mLevel.draw_level2(NULL, 0, 0, 0, 1, 1, 1, 1, 1, 0); // redraw entire level in case only region has been drawn
      al_set_target_backbuffer(mDisplay.display);
   }

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
      mLevel.draw_level_centered_on_player_pos(fmxi, fmyi, sci);
      mScreen.draw_screen_overlay();
      al_flip_display();

      al_rest(delay);
   }


}





void mwTransitions::set_transition_mode(int i, int f, int num_steps)
{

   if (!mMain.headless_server)
   {
      mLevel.draw_level2(NULL, 0, 0, 0, 1, 1, 1, 1, 1, 0); // redraw entire level in case only region has been drawn
      al_set_target_backbuffer(mDisplay.display);
   }

   mLevel.set_screen_display_variables();
   mLevel.set_level_display_region_xy();
   mScreen.set_map_var();

   transition_initial_x = 0;
   transition_initial_y = 0;
   transition_initial_scale = 1;

   transition_final_x = 0;
   transition_final_y = 0;
   transition_final_scale = 1;

   // player's position in level
   float px = mLevel.mPlayer.syn[mLevel.mPlayer.active_local_player].x;
   float py = mLevel.mPlayer.syn[mLevel.mPlayer.active_local_player].y;

   if (i == 0) // nothing
   {
      transition_initial_x = mDisplay.SCREEN_W/2;
      transition_initial_y = mDisplay.SCREEN_H/2;
      transition_initial_scale = 0.005;
   }
   if (f == 0) // nothing
   {
      transition_final_x = mDisplay.SCREEN_W/2;
      transition_final_y = mDisplay.SCREEN_H/2;
      transition_final_scale = 0.005;
   }
   if (i == 1) // game
   {
      transition_initial_x = mDisplay.scale_factor_current;
      transition_initial_y = mLevel.screen_display_x + (px - mLevel.level_display_region_x) * transition_initial_scale;
   }
   if (f == 1) // game
   {
      transition_final_scale = mDisplay.scale_factor_current;
      transition_final_x = mLevel.screen_display_x + (px - mLevel.level_display_region_x) * transition_final_scale;
      transition_final_y = mLevel.screen_display_y + (py - mLevel.level_display_region_y) * transition_final_scale;
   }
   if (i == 2) // menu
   {
      transition_initial_x = mScreen.menu_level_display_x + px * transition_initial_scale;
      transition_initial_y = mScreen.menu_level_display_y + py * transition_initial_scale;
   }
   if (f == 2) // menu
   {
      transition_final_scale = (float)mScreen.menu_level_display_size / 2000;
      transition_final_x = mScreen.menu_level_display_x + px * transition_final_scale;
      transition_final_y = mScreen.menu_level_display_y + py * transition_final_scale;
   }
   if (i == 3) // gate
   {
      transition_initial_scale = mDisplay.scale_factor_current * (gate_enter_level_icon_size / 2000.0); // level icon size = 200;
      transition_initial_x = gate_transition_x + px * transition_initial_scale;
      transition_initial_y = gate_transition_y + py * transition_initial_scale;
   }
   if (f == 3) // gate
   {
      // restore viewport so transition lines up
      mLevel.level_display_region_x = gate_transition_wx;
      mLevel.level_display_region_y = gate_transition_wy;
      //mDisplay.set_scale_factor(mScreen.gate_transition_scale, 1); // disabled for now, I want to keep scale if changed while playing

      transition_final_scale = mDisplay.scale_factor_current * (gate_enter_level_icon_size / 2000.0); // level icon size = 200;
      transition_final_x = gate_transition_x + px * transition_final_scale;
      transition_final_y = gate_transition_y + py * transition_final_scale;
   }



   // get scale multiplier using compound interest formula
   transition_scale_multiplier = 1.0 + pow((transition_final_scale/transition_initial_scale), (1.0/num_steps) ) - 1;

   // get x and y incs
   transition_x_inc = (transition_final_x - transition_initial_x) / num_steps;
   transition_y_inc = (transition_final_y - transition_initial_y) / num_steps;

   // set initial values
   transition_current_x = transition_initial_x;
   transition_current_y = transition_initial_y;
   transition_current_scale = transition_initial_scale;


   mGameState.transition_mode = 1;
   mGameState.transition_timer = num_steps;


   printf("new transition(i: %d   f:%d   steps:%d\n", i, f, num_steps);

   printf("initial: %f   %f   %f \n", transition_initial_x, transition_initial_y, transition_initial_scale);
   printf("final:   %f   %f   %f \n", transition_final_x, transition_final_y, transition_final_scale);
   printf("inc:     %f   %f   %f \n", transition_x_inc, transition_y_inc, transition_scale_multiplier);

}



void mwTransitions::proc_transition_mode()
{
   if (mGameState.transition_mode == 1) // do transition
   {

      printf("proc_transition_mode() - %d  x:%f  y:%f  scale:%f\n", mGameState.transition_timer, transition_current_x, transition_current_y, transition_current_scale);

      if (--mGameState.transition_timer <= 0) mGameState.transition_mode = 0;

      transition_current_x += transition_x_inc;
      transition_current_y += transition_y_inc;

      transition_current_scale *= transition_scale_multiplier;

      if (!mMain.headless_server)
      {




         al_set_target_backbuffer(mDisplay.display);
         al_clear_to_color(al_map_rgb(0,0,0));
         mScreen.draw_screen_frame();

         mLevel.draw_level_centered_on_player_pos(transition_current_x, transition_current_y, transition_current_scale);

         al_flip_display();


         //   al_draw_text(mFont.pr16, mColor.White, mDisplay.SCREEN_W/2, mDisplay.SCREEN_H/2, ALLEGRO_ALIGN_CENTER, "INITIAL" );
         //   al_flip_display();
         if (mGameState.transition_timer == 99) mInput.tsw();





      }
   }
}










