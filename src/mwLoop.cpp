// mwLoop.cpp

#include "pm.h"
#include "mwLoop.h"

#include "mwQuickGraph.h"
#include "mwSound.h"
#include "mwLog.h"
#include "mwSettings.h"
#include "mwPlayer.h"
#include "mwNetgame.h"
#include "mwTally.h"
#include "mwDrawSequence.h"
#include "mwLogo.h"
#include "mwBottomMessage.h"
#include "mwDemoMode.h"
#include "mwDisplay.h"
#include "mwTimeStamp.h"
#include "mwFont.h"
#include "mwLift.h"
#include "mwVisualLevel.h"
#include "mwGameMoves.h"
#include "mwTriggerEvent.h"
#include "mwInput.h"
#include "mwEventQueue.h"
#include "mwBitmap.h"
#include "mwMenu.h"
#include "mwHelp.h"
#include "mwItem.h"
#include "mwEnemy.h"
#include "mwLevel.h"

#include "mwScreen.h"
#include "mwShot.h"
#include "mwWindow.h"
#include "mwWindowManager.h"

#include "mwColor.h"
#include "mwMain.h"

#include "mwConfig.h"


mwLoop mLoop;

mwLoop::mwLoop()
{
   initialize();
}

void mwLoop::initialize(void)
{

   for (int i=0; i<8; i++) state[i]= 0;

   top_menu_sel = 0;
   main_loop_exit = 0;

   level_editor_running = 0;
   help_screens_running = 0;
   visual_level_select_running = 0;

   show_debug_overlay = 0;

   // temp testing variables
   pct_x = 0;
   pct_y = 0;

   // frame_speed, frames per second, mwPS.frame_num stuff
   frame_speed = 40;
   frame_num = 0;
   speed_control_lock = 1;
   eco_draw = 0;
}


void mwLoop::draw_frame(void)
{
   mDrawSequence.draw();
//   get_new_background(1);
//   draw_lifts();
//   draw_items();
//   draw_enemies();
//   draw_eshots();
//   draw_pshots();
//   draw_players();
//   draw_scaled_level_region_to_display(0);
//   draw_screen_overlay();
//   al_flip_display();
}

void mwLoop::move_frame(void)
{
   char msg[1024];
   double t0, t1, t2, t3, t4, t5, t6;
   if ((mLog.LOG_TMR_move_tot) || (mLog.LOG_TMR_move_all)) t0 = al_get_time();
   mShot.move_eshots();
   if (mLog.LOG_TMR_move_all) t1 = al_get_time();
   mShot.move_pshots();
   if (mLog.LOG_TMR_move_all) t2 = al_get_time();
   mLift.move_lifts(0);
   if (mLog.LOG_TMR_move_all) t3 = al_get_time();
   mPlayer.move_players();
   if (mLog.LOG_TMR_move_all) t4 = al_get_time();
   mEnemy.move_enemies();
   if (mLog.LOG_TMR_move_all) t5 = al_get_time();
   mItem.move_items();
   if (mLog.LOG_TMR_move_all)
   {
      t6 = al_get_time();
      sprintf(msg, "tmst m-esht:[%0.4f] m-psht:[%0.4f] m-lift:[%0.4f] m-plyr:[%0.4f] m-enem:[%0.4f] m-item:[%0.4f] m-totl:[%0.4f]\n",
      (t1-t0)*1000, (t2-t1)*1000, (t3-t2)*1000, (t4-t3)*1000, (t5-t4)*1000, (t6-t5)*1000, (t6-t0)*1000);
      //printf("\n%s\n", msg);
      mLog.add_log_entry2(44, 0, msg);
   }
   if (mLog.LOG_TMR_move_tot) mLog.add_log_TMR(al_get_time() - t0, "move", 0);
}


void mwLoop::loop_frame(int times) // used for fast forwarding after rewind
{
   ff_state = 1;
   for (int i=0; i<times; i++)
   {
      mGameMoves.proc();
      if (mPlayer.syn[0].level_done_mode) proc_level_done_mode();
      else move_frame();
      frame_num++;
   }
   ff_state = 0;
}

int mwLoop::have_all_players_acknowledged(void)
{
   int ret = 1; // yes by default
   for (int p=0; p<NUM_PLAYERS; p++)
   {
      if (mPlayer.syn[p].active)
      {
         if (mGameMoves.has_player_acknowledged(p))
         {
            mPlayer.syn[p].level_done_ack = 1;
         }
         else
         {
            mPlayer.syn[p].level_done_ack = 0;
            ret = 0;
         }
      }
   }
   return ret;
}

void mwLoop::game_menu(void)
{
   if (mMain.classic_mode)
   {
      if (!mLogo.splash_screen_done) { mLogo.splash_screen(); mLogo.splash_screen_done = 1; }
      if (!mLevel.resume_allowed) mLevel.set_start_level();
      if (top_menu_sel < 3) top_menu_sel = 3;
      while (top_menu_sel != 1)
      {
         top_menu_sel = mMenu.zmenu(1, top_menu_sel, 10);

         if  (top_menu_sel == 1)  { state[0] = 0;                                           return; } // exit

         if  (top_menu_sel == 2) // visual level select
         {
            int r = mVisualLevel.visual_level_select();
            if (r == 1) top_menu_sel = 3; // start new game
            if (r == 3) top_menu_sel = 8; // start level editor
         }
         if ((top_menu_sel == 4) && (mLevel.resume_allowed)) { state[0] = 13;                                       return; } // resume game
         if  (top_menu_sel == 3)  { state[0] = 10;  top_menu_sel = 4;                                               return; } // start new game
         if  (top_menu_sel == 5)  { state[0] = 20;                                                                  return; } // host network game
         if  (top_menu_sel == 6)  { state[0] = 24;                                                                  return; } // join network game
         if  (top_menu_sel == 7)  { state[0] = 3;                                                                   return; } // settings
         if  (top_menu_sel == 8)  { mLevel.set_start_level(mWM.mW[1].edit_menu(mLevel.start_level)); state[0] = 10; return; } // level editor
         if  (top_menu_sel == 9)  { state[0] = 30; mDemoMode.restore_mode = 1; mDemoMode.restore_level = mLevel.start_level;   return; } // demo mode
         if  (top_menu_sel == 10)                                                                                      mHelp.help(""); // help
         if ((top_menu_sel > 100) && (top_menu_sel < 200)) // right pressed on menu item
         {
            top_menu_sel -= 100;
            if (top_menu_sel == 2) mLevel.next_level(); // next level
         }
         if ((top_menu_sel > 200) && (top_menu_sel < 300)) // left pressed on menu item
         {
            top_menu_sel -= 200;
            if (top_menu_sel == 2) mLevel.prev_level(); // prev level
         }
      }
   }
   else // story mode
   {
      // are we in the overworld or an actual level?
      if (mLevel.last_level_loaded == 1) // overworld
      {
         //top_menu_sel = 2;
         if (top_menu_sel < 2) top_menu_sel = 2;
         if (top_menu_sel > 7 ) top_menu_sel = 7;

         while (top_menu_sel != 1)
         {
            top_menu_sel = mMenu.zmenu(2, top_menu_sel, 10);

            if  (top_menu_sel == 1)  { state[0] = 0;                                return; } // exit
            if  (top_menu_sel == 2)  { state[0] = 10;                           return; } // start new game
            if  (top_menu_sel == 3)  { state[0] = 20;                           return; } // host network game
            if  (top_menu_sel == 4)  { state[0] = 24;                           return; } // join network game
            if  (top_menu_sel == 5)  { state[0] = 3;                            return; } // settings
            if  (top_menu_sel == 6)  { state[0] = 30;  mDemoMode.restore_mode = 1; mDemoMode.restore_level = 1; return; } // demo mode
            if  (top_menu_sel == 7)                                                mHelp.help(""); // help

            while (top_menu_sel > 100) top_menu_sel-=100; // to account for left and right which are not used

         }
      }
//      else // regular level
//      {
//         mPlayer.syn[0].level_done_next_level = 1;
//         state[0] = 12;
//         return;  // back to overworld
////         //top_menu_sel = 2;
////         if (top_menu_sel < 2) top_menu_sel = 2;
////         while (top_menu_sel != 1)
////         {
////            top_menu_sel = mMenu.zmenu(3, top_menu_sel, 10);
////            if  (top_menu_sel == 1)  { state[1] = 0;                               return; } // exit
////            if  (top_menu_sel == 2)  { state[0] = 13;                          return; } // resume
////            if  (top_menu_sel == 3)  { mLevel.start_level = 1; state[0] = 10;  return; } // back to overworld
////            if  (top_menu_sel == 4)  { state[0] = 3;                           return; } // settings
////            if  (top_menu_sel == 5)                                                mHelp.help(""); // help
////         }
//      }
   }
}

// state[1]

/*

state =  0; // quit
state =  1; // menu
state =  3; // config

state = 10; // start new game
state = 11; // main game loop
state = 12; // level_done
state = 13; // resume

state = 19; // server exit
state = 20; // start server game
state = 21; // client wait for initial state
state = 22; // client load level and set up
state = 23; // client wait for sjon
state = 24; // client init network and send cjon
state = 25; // client exit and clean up network

state = 30; // start demo mode           (leads to 12)
state = 31; // setup and run demo level  (from 12 or manual load gm)
state = 32; // quit demo with keypress
*/


void mwLoop::proc_program_state(void)
{
   //printf("proc ps ps:%d nps:%d \n", state[1], state[0]);
   char msg[1024];


   // ----------------------------------------------------------
   // handle all the changes from one state to another
   // ----------------------------------------------------------
   if (state[0] != state[1])
   {
      printf("---------  Stage change from %2d to %2d  --------  ", state[1], state[0]);
      for (int i=0; i<8; i++) printf("%2d ", state[i]);
      printf(" --------  [%4d]\n", mLoop.frame_num);
      for (int i=7; i>0; i--) state[i] = state[i-1]; // slide all down (now state[0] == state[1])

      if (state[1] == 1) // game menu or fast exit
      {
         printf("[State 1 - Game Menu]\n");

         if ((!mMain.classic_mode) && (mLevel.play_level != 1)) // test if just going back to overworld with escape
         {
            mPlayer.syn[0].level_done_next_level = 1;
            state[1] = state[0] = 12; // overwrite!
         }
         else
         {
            if (mNetgame.ima_server) mNetgame.server_exit();
            if (mNetgame.ima_client) mNetgame.client_exit();

            if (mLog.autosave_log_on_game_exit) mLog.save_log_file();
            if (mLog.autosave_game_on_game_exit) mGameMoves.blind_save_game_moves(2);

            mSound.stop_sound();

            for (int i=2; i<7; i++)
            {
               if ((state[i] == 31) && (state[i+1] == 0)) state[0] = 0; // command line arg, demo level
               if ((state[i] == 10) && (state[i+1] == 0)) state[0] = 0; // command line arg, new game
            }

            if (state[0] != 0) // skip all this if exit has already been set
            {
               if (state[2] == 11)  mScreen.transition_cutscene(1, 2, 60, 0.02); // game to menu
               if (state[2] == 32)  mScreen.transition_cutscene(0, 2, 60, 0.02); // nothing to menu (from demo)
            }


         }
      }
   }





   if (state[0] == 0) main_loop_exit = 1; // quit
   if (state[0] == 1) game_menu();  // game menu (this blocks)
   if (state[0] == 3) mSettings.settings_pages(-1);  // this blocks


























   //---------------------------------------
   // 25 - client exit
   //---------------------------------------
   if (state[1] == 25) // client exit
   {
      mNetgame.client_exit();
      state[0] = 1;
   }

   //---------------------------------------
   // 24 - client new game
   //---------------------------------------
   if (state[1] == 24)
   {
      if (!mNetgame.client_init())
      {
         state[0] = 25;
         return;
      }

      // set up mwQuickGraph here
      // set up mwQuickGraph here
      mQuickGraph[0].initialize(1);
      mQuickGraph[1].initialize(2);
      mQuickGraph[2].initialize(2);

      for (int p=0; p<NUM_PLAYERS; p++) mPlayer.init_player(p, 1); // full reset
      mPlayer.syn[0].active = 1;

      state[0] = 23;
   }

   //---------------------------------------
   // 23 - client wait for join
   //---------------------------------------
   if (state[1] == 23)
   {
      mNetgame.client_fast_packet_loop();
      mNetgame.client_read_packet_buffer();
      if (mInput.key[ALLEGRO_KEY_ESCAPE][1]) state[0] = 25; // give them an escape option
   }

   //---------------------------------------
   // 22 - client level setup
   //---------------------------------------
   if (state[1] == 22)
   {
      //printf("client set up level\n");

      if (!mLevel.load_level(mLevel.play_level, 0, 0))
      {
         state[0] = 25;
         return;
      }


      for (int p=0; p<NUM_PLAYERS; p++)
         mPlayer.set_player_start_pos(p, 0);     // get starting position for all players, active or not


      mGameMoves.initialize();

      frame_num = 0;
      mNetgame.reset_states();
      mShot.clear_shots();
      mBottomMessage.initialize();
      mInput.initialize();
      mTriggerEvent.initialize();

      if (mLog.LOG_NET)
      {
         sprintf(msg,"LEVEL %d STARTED", mLevel.play_level);
         mLog.add_log_entry_header(10, 0, msg, 3);
      }

      show_player_join_quit_timer = 0;
      mSound.start_music(0);
      mTimeStamp.init_timestamps();
      state[0] = 21;
   }

   //---------------------------------------
   // 21 - client wait for intial state
   //---------------------------------------
   if (state[1] == 21)
   {
      mNetgame.client_fast_packet_loop();
      mNetgame.client_read_packet_buffer();
      mNetgame.client_apply_dif();

      if (mInput.key[ALLEGRO_KEY_ESCAPE][3]) state[0] = 25;

      sprintf(msg, "Waiting for game state from server");
      float stretch = ( (float)mDisplay.SCREEN_W / ((strlen(msg)+2)*8));
      mScreen.rtextout_centre(mFont.bltn, NULL, msg, mDisplay.SCREEN_W/2, mDisplay.SCREEN_H/2, 10, stretch, 1);

      al_flip_display();

      if (frame_num > 0)
      {
         printf("got initial state for frame:%d\n", frame_num);
         int p = mPlayer.active_local_player;

         // set holdoff 200 frames in future so client won't try to drop while syncing
         mPlayer.loc[p].client_last_stdf_rx_frame_num = frame_num + 200;

         if (mLog.LOG_NET_join) mLog.add_log_entry_header(11, p, "Game state updated - starting chase and lock", 1);
         state[0] = 11;
      }
   }

   //---------------------------------------
   // 20 - server new game
   //---------------------------------------
   if (state[1] == 20)
   {
      if (!mNetgame.server_init())
      {
         mNetgame.server_exit();
         state[0] = 19;
         return;
      }

      mLevel.set_start_level();

      // reset players
      for (int p=0; p<NUM_PLAYERS; p++)
      {
         mPlayer.init_player(p, 1);           // full reset (start modes 1, 2, 3, 9)
         mPlayer.set_player_start_pos(p, 0);  // get starting position for all players, active or not
      }

      mPlayer.syn[0].active = 1;
      mPlayer.syn[0].control_method = 3;
      strncpy(mPlayer.loc[0].hostname, local_hostname, 16);


      mGameMoves.initialize();
      frame_num = 0;
      mNetgame.reset_states();
      mShot.clear_shots();
      mBottomMessage.initialize();
      mInput.initialize();
      mTriggerEvent.initialize();

      mNetgame.game_vars_to_state(mNetgame.srv_client_state[0][1]);
      mNetgame.srv_client_state_frame_num[0][1] = frame_num;
      if (mLog.LOG_NET_stdf)
      {
         sprintf(msg, "stdf saved server state[1]:%d\n", frame_num);
         mLog.add_log_entry2(27, 0, msg);
      }

      mGameMoves.add_game_move(0, 0, 0, mLevel.play_level);       // [00] game_start

      // save colors in game moves array
      for (int p=0; p<NUM_PLAYERS; p++)
         if (mPlayer.syn[p].active) mGameMoves.add_game_move(0, 1, p, mPlayer.syn[p].color); // 1 - player_state and color

      if (mLog.LOG_NET)
      {
         sprintf(msg,"LEVEL %d STARTED", mLevel.play_level);
         mLog.add_log_entry_header(10, 0, msg, 3);
      }

      show_player_join_quit_timer = 0;
      mSound.start_music(0); // rewind and start theme
      mTimeStamp.init_timestamps();
      state[0] = 11;
   }

   //---------------------------------------
   // 19 - server exit
   //---------------------------------------
   if (state[1] == 19) // server exit
   {
      mNetgame.server_exit();
      state[0] = 1;
   }


































































   //---------------------------------------
   // 10 - single player new game
   //---------------------------------------
   if (state[1] == 10)
   {
      printf("[State 10 - New Game]  [lev:%d]\n", mLevel.play_level);

      // set up mwQuickGraph here
      mQuickGraph[0].initialize(1);
      mQuickGraph[1].initialize(2);

      mLevel.set_start_level(); // load level
      setup_players_after_level_load(1); // type 1 full reset
      mGameMoves.initialize();
      mGameMoves.add_game_move(0, 0, 0, mLevel.play_level);       // [00] game_start
      setup_common_after_level_load();
      mScreen.transition_cutscene(2, 1, 60, 0.02); // menu to game
   }

   //---------------------------------------
   // 12 - level done (more accurately load next level)
   //---------------------------------------
   if (state[1] == 12)
   {
      printf("[State 12 - Next Level]   [play lev:%d]  [next lev:%d]\n", mLevel.play_level, mPlayer.syn[0].level_done_next_level);

      for (int i=0; i<8; i++) printf("%2d ", state[i]);
      printf("\n");

      // if demo level just ended to get here, immediately send to 32
      if ((state[3] == 31) && (!mDemoMode.demo_mode_on)) // demo level just completed
      {
         state[0] = 32; // demo mode cleanup and exit
         return; // to exit immediately
      }

      if ((state[3] == 10) && (state[4] == 0)) // normal play level started from command line
      {
         printf("command line exit\n");
         state[0] = 0; // exit to dos
         return; // to exit immediately
      }

      mSound.stop_sound();

      mPlayer.syn[0].level_done_mode = 0;

      if (mLog.LOG_NET) { sprintf(msg,"NEXT LEVEL:%d", mPlayer.syn[0].level_done_next_level); mLog.add_log_entry_header(10, 0, msg, 3); }



      // -------------------------------
      // pre load transition
      // -------------------------------

      printf("pre-load transition\n");

      int gate_to_game = 0;
      int game_to_gate = 0;


      // if going to overworld from anything except menu, set game to gate
      if ((mPlayer.syn[0].level_done_next_level == 1) && (state[2] != 1))
      {
         game_to_gate = 1;
         printf("1game to gate set\n");
      }



      // did we just come from 32?
      if (state[2] == 32) // demo mode just ended
      {

      }
      else
      {
         // if going from overworld to any other level, assume gate is used...no transition until after load
         if ((mLevel.play_level == 1) && (mPlayer.syn[0].level_done_next_level != 1)) gate_to_game = 1;

         else if (mMain.classic_mode)
         {
            mScreen.transition_cutscene(1, 0, 90, 0.01); // game to nothing
         }
         else
         {
            mScreen.transition_cutscene(1, 3, 90, 0.01); // game to icon
            game_to_gate = 1;
            printf("2game to gate set\n");
         }
      }


      // if we get here in demo mode, either initially or after level done
      // call mDemoMode.load_random_demo() to load game moves for next demo level
      // this needs to be after the cutscene transitions
      if (mDemoMode.demo_mode_on)
      {
         mDemoMode.load_random_demo();
         return; // to exit immediately
      }



      if ((mLog.LOG_NET) && (mNetgame.ima_client)) mLog.log_ending_stats(mPlayer.active_local_player);
      if ((mLog.LOG_NET) && (mNetgame.ima_server)) mLog.log_ending_stats_server();

      if (mNetgame.ima_server) mNetgame.server_flush();
      if (mNetgame.ima_client) mNetgame.client_flush();

      mGameMoves.blind_save_game_moves(1);

      if (mLog.autosave_log_on_level_done) mLog.save_log_file();





      mLevel.play_level = mPlayer.syn[0].level_done_next_level;
      if (!mLevel.load_level(mLevel.play_level, 0, 0))
      {
         state[0] = 1;
         return;
      }
      setup_players_after_level_load(2); // type 2 next level reset
      mGameMoves.initialize();
      mGameMoves.add_game_move(0, 0, 0, mLevel.play_level);       // [00] game_start
      mNetgame.reset_states();
      setup_common_after_level_load();




      if (mNetgame.ima_server) // set server initial state
      {
         mPlayer.syn[0].control_method = 3;

         mNetgame.game_vars_to_state(mNetgame.srv_client_state[0][1]);
         mNetgame.srv_client_state_frame_num[0][1] = frame_num;

         if (mLog.LOG_NET_stdf)
         {
            //   printf("saved server state[1]:%d\n\n", frame_num);
            sprintf(msg, "stdf saved server state[1]:%d\n", frame_num);
            mLog.add_log_entry2(27, 0, msg);
         }
      }

      // save colors in game moves array
      for (int p=0; p<NUM_PLAYERS; p++)
         if (mPlayer.syn[p].active) mGameMoves.add_game_move(0, 1, p, mPlayer.syn[p].color); // [01] player_state and color

      if (mLog.LOG_NET)
      {
         sprintf(msg,"LEVEL %d STARTED", mLevel.play_level);
         mLog.add_log_entry_header(10, 0, msg, 3);
      }

// ----------------------------------------
// post-load transitions
// ----------------------------------------

      // reset the viewport so the cutscene lines up
      if (game_to_gate)
      {
         mDisplay.level_display_region_x = mScreen.gate_transition_wx;
         mDisplay.level_display_region_y = mScreen.gate_transition_wy;
         mDisplay.set_scale_factor(mScreen.gate_transition_scale, 1);
      }

      printf("post-load transition\n");
      if (gate_to_game) mScreen.transition_cutscene(3, 1, 90, 0.01); // gate to game

      // not just come from 32 and classic and not gate to game

      else if ((state[2] != 32) && (mMain.classic_mode)) mScreen.transition_cutscene(0, 1, 90, 0.01); // nothing to game


//            // did we just come from 32?
//      if (state[2] == 32) // demo mode just ended
//      {
//
//      }
//      else
//      {
//         if (gate_to_game) mScreen.transition_cutscene(3, 1, 90, 0.01); // gate to game
//         else
//         {
//            if (mMain.classic_mode) mScreen.transition_cutscene(0, 1, 90, 0.01); // nothing to game
//         }
//
//      }



   }


   //---------------------------------------
   // 13 - resume game
   //---------------------------------------
   if (state[1] == 13)
   {
      mSound.start_music(1); // resume theme
      mScreen.transition_cutscene(2, 1, 60, 0.02); // menu to game
      state[0] = 11;
   }


   //---------------------------------------
   // 30 - start demo mode
   //---------------------------------------
   if (state[1] == 30)
   {
      int drm = mDemoMode.restore_mode;
      printf("[State 30 - Start Demo Mode]  [drm:%d]\n", drm);
      mDemoMode.demo_mode_on = 1;
      state[0] = 12;
   }


   //---------------------------------------
   // 31 - setup and play demo level
   //---------------------------------------
   if (state[1] == 31)
   {
      int drm = mDemoMode.restore_mode;
      printf("[State 31 - Setup Demo Level]  [lev:%d]  [drm:%d]\n", mLevel.play_level, drm);

      if (!mLevel.load_level(mLevel.play_level, 0, 0))
      {
         state[0] = 1;
         return;
      }
      setup_players_after_level_load(1); // type 1 full reset,
      mPlayer.syn[0].control_method = 1; // rungame demo mode
      setup_common_after_level_load();

      if (drm == 0) mScreen.transition_cutscene(0, 1, 90, 0.01); // args (nothing to game)
      if (drm == 1) mScreen.transition_cutscene(0, 1, 90, 0.01); // demo mode random (nothing to game)
      if (drm == 2) mScreen.transition_cutscene(3, 1, 120, 0.02); // gate (gate to game)
      if (drm == 3) mScreen.transition_cutscene(0, 1, 90, 0.01); // settings (nothing to game)
   }


   //-------------------------------------------------------
   // 32 - demo level cleanup and exit to where we came from
   //-------------------------------------------------------
   if (state[1] == 32)
   {
      int drm = mDemoMode.restore_mode;
      printf("[State 32 - Quit and Cleanup Demo Dode]  [drm:%d]\n", drm);

      if (drm == 0) mScreen.transition_cutscene(1, 0, 90, 0.01); // args (game to nothing)
      if (drm == 1) mScreen.transition_cutscene(1, 0, 90, 0.01); // demo mode random (game to nothing)
      if (drm == 2) mScreen.transition_cutscene(1, 3, 120, 0.02); // gate (gate to game)
      if (drm == 3) mScreen.transition_cutscene(1, 0, 90, 0.01); // settings (game to nothing)

      // set demo mode off, even if not set
      mDemoMode.demo_mode_on = 0;

      // set all players inactive
      for (int p=0; p<NUM_PLAYERS; p++) mPlayer.syn[p].active = 0;

      // make player 0 active
      mPlayer.syn[0].active = 1;

      // reset to local control
      mPlayer.syn[0].control_method = 0;

      // restore player color
      mConfig.load();

      int rm = mDemoMode.restore_mode;
      if (rm == 0) // started from command line, exit
      {
         state[0] = 1;
         return;
      }
      if (rm == 1) // started from menu
      {
         mLevel.load_level(mDemoMode.restore_level, 0, 0);

         for (int p=0; p<NUM_PLAYERS; p++)
         {
            mPlayer.init_player(p, 1);            // full reset
            mPlayer.set_player_start_pos(p, 0);   // get starting position for all players, active or not
         }

         state[0] = 1;
         return;
      }

      if (rm == 2) // started from gate, send to overworld with next level
      {
         mPlayer.syn[0].level_done_next_level = 1;
         state[0] = 12;
         return;
      }

      if (rm == 3) // started from settings, restore old level, then back to settings
      {
         mLevel.load_level(mDemoMode.restore_level, 0, 0);
         state[0] = 3;
         return;
      }
   }
}


void mwLoop::setup_players_after_level_load(int type)
{
   for (int p=0; p<NUM_PLAYERS; p++)       // reset players
   {
      mPlayer.init_player(p, type);
      mPlayer.set_player_start_pos(p, 0);  // get starting position for all players, active or not
   }
   mPlayer.syn[0].active = 1;
}


void mwLoop::setup_common_after_level_load(void)
{
   mInput.initialize();
   mBottomMessage.initialize();
   mTriggerEvent.initialize();
   mShot.clear_shots();
   frame_num = 0;
   show_player_join_quit_timer = 0;
   mTimeStamp.init_timestamps();
   mSound.start_music(0); // rewind and start theme
   state[0] = 11;
}








void mwLoop::proc_level_done_mode(void)
{
   //-------------------------------------
   // start of final level rocket cutscene
   //-------------------------------------
   if (mPlayer.syn[0].level_done_mode == 30) // setup for players seek and zoom out

   {
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
         if ((mPlayer.syn[p].active) && (p != c)) // all active players except captain
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
      for (int p=0; p<NUM_PLAYERS; p++)
         if (mPlayer.syn[p].active)
         {
            mPlayer.syn[p].x += mPlayer.syn[p].xinc;
            mPlayer.syn[p].y += mPlayer.syn[p].yinc;
         }
   }

   if (mPlayer.syn[0].level_done_mode == 28) // set up for rocket move
   {
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
      for (int i=0; i<500; i++) mItem.item[i][0] = 0; // items
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
      mScreen.get_new_background(1);

      cutscene_bg_x += cutscene_accel;
      cutscene_accel += 0.07;
      al_draw_bitmap(cutscene_background, 0, cutscene_bg_x, 0);

      mEnemy.draw_enemies();
      mPlayer.draw_players();

      mScreen.draw_scaled_level_region_to_display(0);
      mScreen.draw_screen_overlay();

      al_flip_display();

//      if (mPlayer.syn[0].level_done_timer == 100) mDisplay.set_custom_scale_factor((float)(mDisplay.SCREEN_H - BORDER_WIDTH*2)/3000, 100);


   }
   if (mPlayer.syn[0].level_done_mode == 26) mDisplay.set_custom_scale_factor((float)(mDisplay.SCREEN_H - BORDER_WIDTH*2)/320, 100); // set up for zoom in
   if (mPlayer.syn[0].level_done_mode == 25) {} // zoom in
   if (mPlayer.syn[0].level_done_mode == 24) // jump to level done
   {
      mPlayer.syn[0].level_done_mode = 6;
      mPlayer.syn[0].level_done_timer = 0;
   }

   if (mPlayer.syn[0].level_done_mode == 9) // pause players and set up exit xyincs
   {
      mScreen.set_player_join_quit_display(mPlayer.syn[0].level_done_player, 2, 60);

      for (int p=0; p<NUM_PLAYERS; p++)
         if (mPlayer.syn[p].active)
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
      float fade = 0.3 + (float) mPlayer.syn[0].level_done_timer / 85; // 1 to .3 in 60 frames
      if (mSound.sound_on) al_set_mixer_gain(mSound.st_mixer, ((float)mSound.st_scaler / 9) * fade);
      for (int p=0; p<NUM_PLAYERS; p++)
         if (mPlayer.syn[p].active)
         {
            mPlayer.syn[p].x += mPlayer.syn[p].xinc;
            mPlayer.syn[p].y += mPlayer.syn[p].yinc;
         }
   }
   if (mPlayer.syn[0].level_done_mode == 7) // shrink and rotate
   {
      for (int p=0; p<NUM_PLAYERS; p++)
         if (mPlayer.syn[p].active)
         {
            mPlayer.syn[p].draw_scale -= 0.05;
            mPlayer.syn[p].draw_rot -= 8;
         }
   }
   if (mPlayer.syn[0].level_done_mode == 5) // skippable 15s timeout
   {
      if (!mNetgame.ima_client)
      {
         if (have_all_players_acknowledged()) mPlayer.syn[0].level_done_timer = 0; // skip
      }
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


      if (mPlayer.syn[0].level_done_mode == 8) mPlayer.syn[0].level_done_timer = 60; // players seek exit
      if (mPlayer.syn[0].level_done_mode == 7) mPlayer.syn[0].level_done_timer = 20; // players shrink and rotate into exit
      if (mPlayer.syn[0].level_done_mode == 6) mPlayer.syn[0].level_done_timer = 0;
      if (mPlayer.syn[0].level_done_mode == 5) mPlayer.syn[0].level_done_timer = 600; // skippable 15s delay;
      if (mPlayer.syn[0].level_done_mode == 4) mPlayer.syn[0].level_done_timer = 0;
      if (mPlayer.syn[0].level_done_mode == 3) mPlayer.syn[0].level_done_timer = 0;
      if (mPlayer.syn[0].level_done_mode == 2) mPlayer.syn[0].level_done_timer = 10;  // delay to load next level
      if (mPlayer.syn[0].level_done_mode == 1) state[0] = 12;              // load new level
   }
}



void mwLoop::main_loop(void)
{
   while (!main_loop_exit)
   {
      // ----------------------------------------------------------
      // process state and state changes
      // ----------------------------------------------------------
      proc_program_state();

      // ----------------------------------------------------------
      // process event queue
      // ----------------------------------------------------------
      mEventQueue.proc();


      // ----------------------------------------------------------
      // process fast packet loops
      // ----------------------------------------------------------
      if (mNetgame.ima_server) mNetgame.server_fast_packet_loop();
      if (mNetgame.ima_client) mNetgame.client_fast_packet_loop();

      // ----------------------------------------------------------
      // do things based on the 40 Hz fps_timer event
      // ----------------------------------------------------------
      if (mEventQueue.program_update)
      {
         mEventQueue.program_update = 0;

         if (state[1] == 11) // game loop running
         {
            frame_num++;
            mBitmap.update_animation();

            mSound.proc_sound();

            mTimeStamp.timestamp_frame_start = al_get_time();

            int ldm = mPlayer.syn[0].level_done_mode;

            if ((ldm == 30) || (ldm == 25)) mDisplay.proc_custom_scale_factor_change();
            else                            mDisplay.proc_scale_factor_change();

            if (mNetgame.ima_server) mNetgame.server_control();
            if (mNetgame.ima_client) mNetgame.client_control();

            mPlayer.proc_player_input();
            mGameMoves.proc();

            if (ldm) proc_level_done_mode();
            else move_frame();

            double t0 = al_get_time();
            mNetgame.server_create_new_state();
            if (mLog.LOG_TMR_sdif) mLog.add_log_TMR(al_get_time() - t0, "sdif", 0);

            if (ldm != 27) draw_frame();

            double pt = al_get_time() - mTimeStamp.timestamp_frame_start;
            if (mLog.LOG_TMR_cpu) mLog.add_log_TMR(pt, "cpu", 0);

            mRollingAverage[0].add_data((pt/0.025)*100);
            mQuickGraph[0].add_data(0, mRollingAverage[0].last_input);
            mQuickGraph[0].add_data(1, mRollingAverage[0].mn);
            mQuickGraph[0].add_data(2, mRollingAverage[0].mx);
            mQuickGraph[0].add_data(3, mRollingAverage[0].avg);

         }
      }


      // send client ping at this time in the loop, if flag is set
      if ((mPlayer.loc[mPlayer.active_local_player].client_ping_flag) && (mNetgame.ima_client))
      {
         mPlayer.loc[mPlayer.active_local_player].client_ping_flag = 0;
         mNetgame.client_send_ping();
      }

      // ----------------------------------------------------------
      // do things based on the 1 Hz sec_timer event
      // ----------------------------------------------------------
      if (mEventQueue.program_update_1s)
      {

         mEventQueue.program_update_1s = 0;
         if (state[1] == 11) // game loop running
         {
            if (mNetgame.ima_server)
            {
               // auto adjust server state frequency
               if (mPlayer.loc[0].server_state_freq_mode == 1) // 0 = manual, 1 = auto
               {
                  int mcp = mTally[4].get_max()*1000;
                  if (mcp > 100) mcp = 100;
                  mPlayer.loc[0].server_state_freq = 1 + mcp/25; // use max_client_ping to set server_state_freq
               }
               // tally late cdats and game move dsync
               for (int p=1; p<NUM_PLAYERS; p++)
                  if (mPlayer.syn[p].control_method == 2)
                  {
                     mPlayer.loc[p].late_cdats_last_sec = mTally_late_cdats_last_sec[p].get_tally();
                     mPlayer.loc[p].game_move_dsync_avg_last_sec = mTally_game_move_dsync_avg_last_sec[p].get_avg();
                  }
            }
         }
      }
   }
}
