// mwLoop.cpp

#include "pm.h"
#include "mwLoop.h"

#include "mwQuickGraph.h"
#include "mwQuickGraph2.h"
#include "mwSound.h"
#include "mwLog.h"
#include "mwSettings.h"

#include "mwNetgame.h"
#include "mwTally.h"
#include "mwDrawSequence.h"
#include "mwLogo.h"
#include "mwBottomMessage.h"
#include "mwDemoMode.h"

#include "mwDisplay.h"
#include "mwScreen.h"
#include "mwFont.h"
#include "mwColor.h"

#include "mwVisualLevel.h"
#include "mwGameMoves.h"
#include "mwTriggerEvent.h"
#include "mwInput.h"
#include "mwEventQueue.h"
#include "mwBitmap.h"
#include "mwMenu.h"
#include "mwHelp.h"

#include "mwPlayer.h"
#include "mwEnemy.h"
#include "mwItem.h"
#include "mwLift.h"
#include "mwShot.h"
#include "mwLevel.h"

#include "mwWindowManager.h"

#include "mwMain.h"
#include "mwConfig.h"
#include "mwPacketBuffer.h"





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


void mwLoop::move_frame(void)
{
   if (mPlayer.syn[0].level_done_mode) proc_level_done_mode();
   else
   {
      double t[8] = { 0 };
      t[0] = al_get_time();

      mLog.addf(LOG_OTH_move, 0, "[%4d]Move eshots\n", frame_num);
      mShot.move_eshots();      t[1] = al_get_time();

      mLog.addf(LOG_OTH_move, 0, "[%4d]Move pshots\n", frame_num);
      mShot.move_pshots();      t[2] = al_get_time();

      mLog.addf(LOG_OTH_move, 0, "[%4d]Move lifts\n", frame_num);
      mLift.move_lifts(0);      t[3] = al_get_time();

      mLog.addf(LOG_OTH_move, 0, "[%4d]Move players\n", frame_num);
      mPlayer.move_players();   t[4] = al_get_time();

      mLog.addf(LOG_OTH_move, 0, "[%4d]Move enemies\n", frame_num);
      mEnemy.move_enemies();    t[5] = al_get_time();

      mLog.addf(LOG_OTH_move, 0, "[%4d]Move items\n", frame_num);
      mItem.move_items();       t[6] = al_get_time();

      mLog.add_tmrf(LOG_TMR_move_all, 0, "m-esht:[%0.4f] m-psht:[%0.4f] m-lift:[%0.4f] m-plyr:[%0.4f] m-enem:[%0.4f] m-item:[%0.4f] m-totl:[%0.4f]\n",
                      (t[1]-t[0])*1000, (t[2]-t[1])*1000, (t[3]-t[2])*1000, (t[4]-t[3])*1000, (t[5]-t[4])*1000, (t[6]-t[5])*1000, (t[6]-t[0])*1000);

      mLog.add_tmr1(LOG_TMR_move_tot, 0, "move", al_get_time() - t[0]);
   }
}


void mwLoop::loop_frame(int times) // used for fast forwarding after rewind
{
   ff_state = 1;
   for (int i=0; i<times; i++)
   {
      mGameMoves.proc();
      move_frame();
      frame_num++;
   }
   ff_state = 0;
}


int mwLoop::have_all_players_acknowledged(void)
{
   int ret = 1; // yes by default
   for (int p=0; p<NUM_PLAYERS; p++)
   {
      if ((mPlayer.syn[p].active) && (mPlayer.syn[p].paused_type != 3))
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
   if (!mLogo.splash_screen_done) { mLogo.splash_screen(); mLogo.splash_screen_done = 1; }
   quit_action = 0;
   if (mMain.classic_mode)
   {
      if (!mLevel.resume_allowed) mLevel.set_start_level();
      if (top_menu_sel < 1) top_menu_sel = 1;
      while (top_menu_sel != -1)
      {
         top_menu_sel = mMenu.zmenu(1, top_menu_sel);

         if (top_menu_sel == -1)  // exit
         {
            state[0] = 0;
            return;
         }

         if (top_menu_sel == 7)  // skc demo
         {
            mLevel.skc_trigger_demo = 0;
            if (mGameMoves.load_gm(mLevel.play_level))
            {
               mDemoMode.mode = 1;
               mDemoMode.restore_mode = 22;
               mDemoMode.restore_level = mLevel.play_level;
               mLoop.state[0] = 31;
               quit_action = 1; // menu
               done_action = 1;
               top_menu_sel = 1;
               return;
            }
         }

         if  (top_menu_sel == 0) // visual level select
         {
            int r = mVisualLevel.visual_level_select();
            if (r == 1) top_menu_sel = 1; // start new game
            if (r == 3) top_menu_sel = 6; // start level editor
         }
         if (top_menu_sel == 1) // start new game
         {
            mLevel.play_level = mLevel.start_level;
            state[0] = 10;
            quit_action = 1;
            done_action = 1;
            top_menu_sel = 2; // resume
            return;
         }
         if ((top_menu_sel == 2) && (mLevel.resume_allowed))  // resume game
         {
            state[0] = 13;
            quit_action = 1;
            return;
         }
         if (top_menu_sel == 3)  // host network game
         {
            state[0] = 20;
            quit_action = 1;
            done_action = 1;
            return;
         }
         if (top_menu_sel == 4) // join network game
         {
            state[0] = 24;
            quit_action = 1;
            done_action = 1;
            return;
         }
         if (top_menu_sel == 5) // settings
         {
            state[0] = 3;
            mLoop.quit_action = 1; // menu
            return;
         }
         if (top_menu_sel == 6)  // level editor
         {
            mLevel.set_start_level(mWM.loop(mLevel.start_level));
            quit_action = 1; // menu
            state[0] = 10;
            return;
         }

         if (top_menu_sel == 8) mHelp.help("");

         if ((top_menu_sel >= 100) && (top_menu_sel < 200)) // right pressed on menu item
         {
            top_menu_sel -= 100;
            if (top_menu_sel == 0)
            {
               if (mLevel.resume_allowed) mLevel.add_play_data_record(mLevel.play_level, 0);
               mLevel.set_start_level(mLevel.get_next_level(mLevel.start_level, 10, 0));
            }
         }
         if ((top_menu_sel >= 200) && (top_menu_sel < 300)) // left pressed on menu item
         {
            top_menu_sel -= 200;
            if (top_menu_sel == 0)
            {
               if (mLevel.resume_allowed) mLevel.add_play_data_record(mLevel.play_level, 0);
               mLevel.set_start_level(mLevel.get_prev_level(mLevel.start_level, 10, 0));
            }
         }
      }
   }
   else // story mode
   {
      mLevel.set_start_level();
      if (top_menu_sel < 0) top_menu_sel = 0;
      if (top_menu_sel > 5 ) top_menu_sel = 5;

      while (top_menu_sel != -1)
      {
         top_menu_sel = mMenu.zmenu(2, top_menu_sel);

         if (top_menu_sel == -1)  // exit
         {
            state[0] = 0;
            return;
         }
         if (top_menu_sel == 0) // start new game
         {
            state[0] = 10;
            quit_action = 1; // menu
            done_action = 2; // overworld
            return;
         }
         if (top_menu_sel == 1)  // host network game
         {
            state[0] = 20;
            quit_action = 1;
            done_action = 1;
            return;
         }
         if (top_menu_sel == 2) // join network game
         {
            state[0] = 24;
            quit_action = 1;
            done_action = 1;
            return;
         }
         if (top_menu_sel == 3) // settings
         {
            state[0] = 3;
            mLoop.quit_action = 1; // menu
            return;
         }
         if (top_menu_sel == 4) // demo mode
         {
            mDemoMode.mode = 2;
            mDemoMode.restore_mode = 21;
            mDemoMode.restore_level = mLevel.start_level;
            state[0] = 12;
            mLoop.quit_action = 1; // menu
            mLoop.done_action = 5; // next rand level
            return;
         }

         if (top_menu_sel == 5) mHelp.help(""); // help

         while (top_menu_sel >= 100) top_menu_sel-=100; // to account for left and right which are not used
      }
   }
}


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

state = 31; // setup and run demo level  (from 12 or manual load gm)
state = 32; // quit demo with keypress

state = 40; server remote control setup
state = 41; server remote control run


*/


void mwLoop::proc_program_state(void)
{
  // printf("proc ps ps:%d nps:%d \n", state[1], state[0]);


   // ----------------------------------------------------------
   // handle all the changes from one state to another
   // ----------------------------------------------------------
   if (state[0] != state[1])
   {
      mLog.addf(LOG_OTH_program_state, 0, "[%4d] --- State change from %2d to %2d  ----  ", mLoop.frame_num, state[1], state[0]);
      for (int i=0; i<8; i++) mLog.appf(LOG_OTH_program_state, "%2d ", state[i]);
      mLog.appf(LOG_OTH_program_state, " --- [qa:%d] [da:%d]\n", quit_action, done_action);

      // slide all down (now state[0] == state[1])
      for (int i=7; i>0; i--) state[i] = state[i-1];

      if (state[1] == 1) // game menu or fast exit
      {
         mLog.add(LOG_OTH_program_state, 0, "[State 1 - Game Menu]\n");

         if (mNetgame.ima_server) mNetgame.ServerExitNetwork();
         if (mNetgame.ima_client) mNetgame.ClientExitNetwork();

         if (mLog.autosave_log_on_game_exit) mLog.save_log_file();
         if (mLog.autosave_game_on_game_exit) mGameMoves.blind_save_game_moves(2);

         mSound.stop_sound();

         if (quit_action == 0)
         {
            mScreen.transition_cutscene(1, 0); // game to nothing
            state[0] = 0; // exit program
            return;
         }

         if (quit_action == 1) // menu (already set)
         {
            // only do this if not returning from settings
            if (state[2] != 3) mScreen.transition_cutscene(1, 2); // game to menu
         }
         if (quit_action == 2)  // overworld
         {
            mLog.add(LOG_OTH_program_state, 0, "instead of menu, go to overworld\n");
            mPlayer.syn[0].level_done_next_level = 1;
            state[0] = 12;      // next level
            quit_action = 1;    // menu
            done_action = 2;    // overworld (should never trigger level done from overworld, no exits)
            mLevel.add_play_data_record(mLevel.play_level, 0);
            return;
         }

         if (quit_action == 3)  // settings
         {
            mLog.add(LOG_OTH_program_state, 0, "instead of menu, go to settings\n");
            state[0] = 3;
            quit_action = 1; // set new quit action to menu
            return;
         }
      }
   }

   if (state[1] == 0) main_loop_exit = 1; // quit
   if (state[1] == 1) game_menu();  // game menu (this blocks)
   if (state[1] == 3) mSettings.settings_pages(-1);  // this blocks

   //---------------------------------------
   // 25 - client exit
   //---------------------------------------
   if (state[1] == 25) // client exit
   {
      mLog.add(LOG_OTH_program_state, 0, "[State 25 - Client Exit]\n");
      mNetgame.ClientExitNetwork();
      quit_action = 1; // to prevent quitting clients from automatically going to overworld
      state[0] = 1;
   }

   //---------------------------------------
   // 24 - client new game
   //---------------------------------------
   if (state[1] == 24)
   {
      mLog.add(LOG_OTH_program_state, 0, "[State 24 - Client New Game]\n");

      mLog.log_versions();
      mLog.add_fw (LOG_NET, 0, 76, 10, "+", "-", "");
      mLog.add_fwf(LOG_NET, 0, 76, 10, "|", " ", "Client mode started on localhost:[%s]", mLoop.local_hostname);

      if (!mNetgame.ClientInitNetwork())
      {
         state[0] = 25;
         return;
      }

      mRollingAverage[1].initialize(8); // ping rolling average
      mRollingAverage[2].initialize(8); // dsync rolling average

      initialize_graphs();
      for (int p=0; p<NUM_PLAYERS; p++) mPlayer.init_player(p, 1); // full reset
      mPlayer.syn[0].active = 1;

      mNetgame.client_send_cjon_packet();

      mLog.add_fwf(LOG_NET, 0, 76, 10, "|", " ", "Client sent join request to server with player color:[%2d]", mPlayer.syn[0].color);
      mLog.add_fw (LOG_NET, 0, 76, 10, "+", "-", "");

      state[0] = 23;
   }

   //---------------------------------------
   // 23 - client wait for join
   //---------------------------------------
   if (state[1] == 23)
   {
      mLog.add(LOG_OTH_program_state, 0, "[State 23 - Client Wait For Join]\n");
      mPacketBuffer.rx_and_proc();
      if (mInput.key[ALLEGRO_KEY_ESCAPE][1]) state[0] = 25; // give them an escape option
   }

   //---------------------------------------
   // 22 - client level setup
   //---------------------------------------
   if (state[1] == 22)
   {
      mLog.add(LOG_OTH_program_state, 0, "[State 22 - Client Level Setup]\n");

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

      mLog.add_headerf(LOG_NET, 0, 1, "LEVEL %d STARTED", mLevel.play_level);

      mScreen.player_text_overlay_timer = 0;
      mSound.start_music(0);
      state[0] = 21;
   }

   //---------------------------------------
   // 21 - client wait for intial state
   //---------------------------------------
   if (state[1] == 21)
   {
      mLog.add(LOG_OTH_program_state, 0, "[State 21 - Client Wait for Initial State]\n");

      mScreen.rtextout_centre(mFont.bltn, NULL, mDisplay.SCREEN_W/2, mDisplay.SCREEN_H/2, 10, -2, 1, "Waiting for game state from server");
      al_flip_display();
      if (mInput.key[ALLEGRO_KEY_ESCAPE][3]) state[0] = 25;

      mPacketBuffer.rx_and_proc();

      mNetgame.client_apply_dif();

      if (frame_num > 0)
      {
         int p = mPlayer.active_local_player;
         // set holdoff 200 frames in future so client won't try to drop while syncing
         mPlayer.loc[p].client_last_stdf_rx_frame_num = frame_num + 200;
         state[0] = 11;
      }
   }

   //---------------------------------------
   // 20 - server new game
   //---------------------------------------
   if (state[1] == 20)
   {
      mLog.add(LOG_OTH_program_state, 0, "[State 20 - Server New Game]\n");
      if (!mNetgame.ServerInitNetwork())
      {
         mNetgame.ServerExitNetwork();
         state[0] = 19;
         return;
      }

      mLevel.set_start_level();

      // reset players
      for (int p=0; p<NUM_PLAYERS; p++)
      {
         mPlayer.init_player(p, 1);           // full reset
         mPlayer.set_player_start_pos(p, 0);  // set starting position for all players, active or not
      }

      mPlayer.syn[0].active = 1;
      mPlayer.syn[0].control_method = PM_PLAYER_CONTROL_METHOD_SERVER_LOCAL;
      strncpy(mPlayer.loc[0].hostname, local_hostname, 16);

      mGameMoves.initialize();
      frame_num = 0;
      mNetgame.reset_states();
      mShot.clear_shots();

      mBottomMessage.initialize();
      mInput.initialize();
      mTriggerEvent.initialize();

      initialize_graphs();

      mGameMoves.add_game_move(0, PM_GAMEMOVE_TYPE_LEVEL_START, 0, mLevel.play_level);


      // save colors in game moves array
      for (int p=0; p<NUM_PLAYERS; p++)
         if (mPlayer.syn[p].active) mGameMoves.add_game_move(0, PM_GAMEMOVE_TYPE_PLAYER_ACTIVE, p, mPlayer.syn[p].color); // 1 - player_state and color

      mLog.add_headerf(LOG_NET, 0, 1, "LEVEL %d STARTED", mLevel.play_level);


      mScreen.player_text_overlay_timer = 0;

      mSound.start_music(0); // rewind and start theme
      state[0] = 11;

   }

   //---------------------------------------
   // 19 - server exit
   //---------------------------------------
   if (state[1] == 19) // server exit
   {
      mLog.add(LOG_OTH_program_state, 0, "[State 19 - Server Exit]\n");
      mNetgame.ServerExitNetwork();
      state[0] = 1;
   }



//---------------------------------------
// 10 - single player new game
//---------------------------------------
   if (state[1] == 10)
   {
      mLog.add(LOG_OTH_program_state, 0, "[State 10 - Single Player New Game]\n");
      initialize_graphs();
      if (!mMain.classic_mode) mLevel.start_level = mLevel.play_level = 1;
      if (load_and_setup_level_load(mLevel.play_level)) state[0] = 11;
      else state[0] = 1;
      if (quit_action == 0) mScreen.transition_cutscene(0, 1); // nothing to game
      if (quit_action == 1) mScreen.transition_cutscene(2, 1); // menu to game
   }


//----------------------------------------------------------------------------------------------------------
// 12 - Next Level
//----------------------------------------------------------------------------------------------------------
   if (state[1] == 12)
   {
      mLog.addf(LOG_OTH_program_state, 0, "[State 12 - Next Level]  [play lev:%d]  [next lev:%d]\n", mLevel.play_level, mPlayer.syn[0].level_done_next_level);

// --------------------------------------------------------
// cleanup after level done
// --------------------------------------------------------
      mSound.stop_sound();
      mPlayer.syn[0].level_done_mode = 0;

      mLog.add_headerf(LOG_NET, 0, 1, "NEXT LEVEL:%d", mPlayer.syn[0].level_done_next_level);

      if (mNetgame.ima_client)
      {
         mNetgame.ClientFlush();
         mLog.log_ending_stats_client(LOG_NET_ending_stats, mPlayer.active_local_player);
      }

      if (mNetgame.ima_server)
      {
         mNetgame.ServerFlush();
         mLog.log_ending_stats_server(LOG_NET_ending_stats);
      }


      if (mLog.autosave_log_on_level_done) mLog.save_log_file();
      mGameMoves.blind_save_game_moves(1);


// --------------------------------------------------------
// instant program quit
// started from command line and next level called
// --------------------------------------------------------
      if (done_action == 0) // command line
      {
         mLog.add(LOG_OTH_program_state, 0, "command line exit\n");
         state[0] = 0; // exit to dos
         mScreen.transition_cutscene(1, 0); // game to nothing
         return; // to exit immediately
      }

// --------------------------------------------------------
// single level demo mode completed
// we are in demo mode 1 and next level is called
// --------------------------------------------------------
      if (mDemoMode.mode == 1) // demo level just completed
      {
         mLog.add(LOG_OTH_program_state, 0, "immediate jump to program state 32\n");
         state[0] = 32; // demo mode cleanup and exit
         return;
      }

// ----------------------------------------------------------
// determine what transitions to use
// ----------------------------------------------------------

      // nothing by default
      int pre_load_transistion_initial  = 0;
      int pre_load_transistion_final    = 0;
      int post_load_transistion_initial = 0;
      int post_load_transistion_final   = 0;


// ----------------------------------------------------------
// if destination is overworld
// ----------------------------------------------------------
      if (mPlayer.syn[0].level_done_next_level == 1)
      {
         mLog.add(LOG_OTH_transitions, 0, "Next level to overworld\n");
         pre_load_transistion_initial = 1; // game
         pre_load_transistion_final   = 3; // gate
      }

// -----------------------------------------------------------------------------------------------------
// if going from overworld to any other level, assume gate is used...no transition until after load
// also test that we are not in demo mode
// -----------------------------------------------------------------------------------------------------
      if ((mLevel.play_level == 1) && (mPlayer.syn[0].level_done_next_level != 1) && (!mDemoMode.mode))
      {
         mLog.add(LOG_OTH_transitions, 0, "Next level from overworld\n");
         post_load_transistion_initial = 3; // gate
         post_load_transistion_final   = 1; // game
      }

// -----------------------------------------------------------------------------------------------------
// if going from any level to any level (except overworld)
// also test that previous state == 11  to not trigger when started from menu or settings
// -----------------------------------------------------------------------------------------------------
      if ((mLevel.play_level != 1) && (mPlayer.syn[0].level_done_next_level != 1) && (state[2] == 11))
      {
         mLog.add(LOG_OTH_transitions, 0, "Level to level (no overworld)\n");
         pre_load_transistion_initial  = 1; // game
         pre_load_transistion_final    = 0; // nothing
         post_load_transistion_initial = 0; // nothing
         post_load_transistion_final   = 1; // game
      }

// -----------------------------------------------------------------------------------------------------
// previous state == 1, called here from menu
// the only time this happens is when starting demo mode from menu
// -----------------------------------------------------------------------------------------------------
      if ((state[2] == 1) && (mDemoMode.mode == 2))
      {
         mLog.add(LOG_OTH_transitions, 0, "Demo mode started from menu\n");
         pre_load_transistion_initial  = 2; // menu
         pre_load_transistion_final    = 0; // nothing
      }

// ------------------------------------------------------------------------------------------------------------------------------------
// ---   pre load transition
// ------------------------------------------------------------------------------------------------------------------------------------
      mLog.add(LOG_OTH_transitions, 0, "pre-load ");
      mScreen.transition_cutscene(pre_load_transistion_initial, pre_load_transistion_final);

// ---------------------------------------------------
// load the new level
// ---------------------------------------------------

      // if we get here in demo mode, either initially or after level done
      // call mDemoMode.load_random_demo() to load game moves for next demo level
      // this needs to be after the cutscene transitions
      if (mDemoMode.mode == 2)
      {
         if (mDemoMode.load_random_demo()) state[0] = 31;
         else state[0] = 1;
         return; // to exit immediately
      }

      mLevel.play_level = mPlayer.syn[0].level_done_next_level;
      if (load_and_setup_level_load(mLevel.play_level)) state[0] = 11;
      else state[0] = 1;

      if (mMain.headless_server) printf("Started Level:%d\n", mLevel.play_level);

// ----------------------------------------
// post-load transition
// ----------------------------------------
      mLog.add(LOG_OTH_transitions, 0, "post-load ");
      mScreen.transition_cutscene(post_load_transistion_initial, post_load_transistion_final);
   }




   //---------------------------------------
   // 13 - resume game
   //---------------------------------------
   if (state[1] == 13)
   {
      mLog.add(LOG_OTH_program_state, 0, "[State 13 - Resume Game]\n");
      mSound.start_music(1); // resume theme
      mScreen.transition_cutscene(2, 1); // menu to game
      state[0] = 11;
   }

   //---------------------------------------
   // 31 - setup and play demo level
   //---------------------------------------
   if (state[1] == 31)
   {
      initialize_graphs();

      mLog.addf(LOG_OTH_program_state, 0, "[State 31 - Setup Demo Level]  [lev:%d]  [drm:%d]\n", mLevel.play_level, mDemoMode.restore_mode);

      if (load_and_setup_level_load(mLevel.play_level))
      {
         state[0] = 11;
         if      (mDemoMode.restore_mode == 42) mScreen.transition_cutscene(3, 1); // gate to game
         else if (mDemoMode.restore_mode == 22) mScreen.transition_cutscene(2, 1); // menu to game (single)
         else                                   mScreen.transition_cutscene(0, 1); // all other (nothing to game)
      }
      else state[0] = 1;
   }


   //-------------------------------------------------------
   // 32 - demo level cleanup and exit to where we came from
   //-------------------------------------------------------
   if (state[1] == 32)
   {
      int rm = mDemoMode.restore_mode;
      int m = mDemoMode.mode;

      mSound.stop_sound();

      mLog.addf(LOG_OTH_program_state, 0, "[State 32 - Quit and Cleanup Demo Mode]  [drm:%d]  [m:%d]\n", rm,  m);

      mDemoMode.mode = 0;

      for (int p=0; p<NUM_PLAYERS; p++) mPlayer.syn[p].active = 0;       // set all players inactive
      mPlayer.syn[0].active = 1; // make player 0 active
      mPlayer.syn[0].control_method = PM_PLAYER_CONTROL_METHOD_SINGLE_PLAYER; // reset to local control
      mConfig.load_config(); // restore player color

      if (rm == 10) // started from command line, exit
      {
         mScreen.transition_cutscene(1, 0); // game to nothing
         state[0] = 0;
      }

      if (rm == 21) // rnd demo started from menu
      {
         mLog.addf(LOG_OTH_program_state, 0, "[State 32 - Restore Mode:21 (rnd demo start from menu) Restore Level:%d\n", mDemoMode.restore_level);

         mScreen.transition_cutscene(1, 0);   // game to nothing
         mLevel.load_level(mDemoMode.restore_level, 0, 0);      // restore old level

         for (int p=0; p<NUM_PLAYERS; p++)
         {
            mPlayer.init_player(p, 1);            // full reset
            mPlayer.set_player_start_pos(p, 0);   // get starting position for all players, active or not
         }
         mScreen.transition_cutscene(0, 2); // nothing to menu
         quit_action = 99;  // to prevent transition in state 1
         state[0] = 1;
      }

      if (rm == 22) // single started from menu
      {
         mLog.addf(LOG_OTH_program_state, 0, "[State 32 - Restore Mode:22 (single demo start from menu) Restore Level:%d\n", mDemoMode.restore_level);

         mScreen.transition_cutscene(1, 2);   // game to menu
         mLevel.load_level(mDemoMode.restore_level, 0, 0);      // restore old level

         for (int p=0; p<NUM_PLAYERS; p++)
         {
            mPlayer.init_player(p, 1);            // full reset
            mPlayer.set_player_start_pos(p, 0);   // get starting position for all players, active or not
         }
         quit_action = 99;  // to prevent transition in state 1
         state[0] = 1;
      }

      if (rm == 42) // started from gate, send to overworld with 12 - next level
      {
         mLog.addf(LOG_OTH_program_state, 0, "[State 32 - Restore Mode:42 (demo start from gate) Restore Level:%d -- changed quit_action to 1\n", mDemoMode.restore_level);

         mPlayer.syn[0].level_done_next_level = 1; // set to overworld level
         state[0] = 12; // next level
         quit_action = 1;
      }

      if (rm == 32) // started from settings, restore old level, then back to settings
      {
         mLog.addf(LOG_OTH_program_state, 0, "[State 32 - Restore Mode:32 (started from settings, restore old level, then back to settings) Restore Level:%d\n", mDemoMode.restore_level);

         mScreen.transition_cutscene(1, 0); // settings (game to nothing)
         mLevel.load_level(mDemoMode.restore_level, 0, 0); // restore old level
         state[0] = 3;
         quit_action = 1;    // menu
      }
   }



   //-------------------------------------------------------
   // 40 - server remote control setup
   //-------------------------------------------------------
   if (state[1] == 40)
   {
      mLog.add(LOG_OTH_program_state, 0, "[State 40 - Server Remote Control Setup]\n");
      printf("server remote control setup\n");

      for (int p=0; p<NUM_PLAYERS; p++) mPlayer.init_player(p, 1); // full reset


      // initialize driver with server address
      if (!mNetgame.ClientInitNetwork())
      {
         state[0] = 0; //quit
         return;
      }

      mNetgame.client_send_cjrc_packet();

      while (mNetgame.remote_join_reply)
      {
         mPacketBuffer.rx_and_proc();

         mEventQueue.proc(1);
         al_set_target_backbuffer(mDisplay.display);
         al_clear_to_color(al_map_rgb(0,0,0));
         al_draw_textf(mFont.pr8, mColor.pc[10], 100, 80,  0, "Remote Control - Waiting for server reply");
         al_flip_display();

         if (mInput.key[ALLEGRO_KEY_ESCAPE][0])
         {
            while (mInput.key[ALLEGRO_KEY_ESCAPE][0]) mEventQueue.proc(1);
            state[0] = 0;
            return;
         }
      }
      state[0] = 41;
      printf("server remote control run\n");
      mLog.add(LOG_OTH_program_state, 0, "[State 40 - Server Remote Control Run]\n");
      initialize_graphs();
   }
}


int mwLoop::load_and_setup_level_load(int level)
{
   if (!mLevel.load_level(level, 0, 0)) return 0;
   else setup_level_after_load();
   return 1;
}



void mwLoop::setup_level_after_load(void)
{
   if (state[1] == 10) // new game
   {
      setup_players_after_level_load(1); // type 1 full reset,
      mGameMoves.initialize();
      mGameMoves.add_game_move(0, PM_GAMEMOVE_TYPE_LEVEL_START, 0, mLevel.play_level);
   }
   if (state[1] == 12) // next level
   {
      setup_players_after_level_load(2); // type 2 next level reset
      mGameMoves.initialize();
      mGameMoves.add_game_move(0, PM_GAMEMOVE_TYPE_LEVEL_START, 0, mLevel.play_level);
      mNetgame.reset_states();
   }
   if (state[1] == 31) // demo level setup
   {
      setup_players_after_level_load(1); // type 1 full reset,
      mPlayer.syn[0].control_method = PM_PLAYER_CONTROL_METHOD_DEMO_MODE; // rungame demo mode
   }
   setup_common_after_level_load();
}


void mwLoop::setup_players_after_level_load(int type)
{
   for (int p=0; p<NUM_PLAYERS; p++)       // reset players
   {
      mPlayer.init_player(p, type);        // type 1 full reset, type 2 level done reset
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
   mScreen.player_text_overlay_timer = 0;
   mSound.start_music(0); // rewind and start theme
   state[0] = 11;

   if (mNetgame.ima_server) mPlayer.syn[0].control_method = PM_PLAYER_CONTROL_METHOD_SERVER_LOCAL;

   if (!mDemoMode.mode)
   {
      // save colors in game moves array
      for (int p=0; p<NUM_PLAYERS; p++)
         if (mPlayer.syn[p].active) mGameMoves.add_game_move(0, PM_GAMEMOVE_TYPE_PLAYER_ACTIVE, p, mPlayer.syn[p].color);
      mLog.add_headerf(LOG_NET, 0, 1, "LEVEL %d STARTED", mLevel.play_level);
   }
}


void mwLoop::add_local_cpu_data(double cpu)
{
   // send to rolling average
   mRollingAverage[0].add_data(cpu);

   if (!mMain.headless_server)
   {
      // original style graph
      mQuickGraph[0].add_data(0, mRollingAverage[0].last_input);
      mQuickGraph[0].add_data(1, mRollingAverage[0].mn);
      mQuickGraph[0].add_data(2, mRollingAverage[0].mx);
      mQuickGraph[0].add_data(3, mRollingAverage[0].avg);

      // new style graph
      mQuickGraph2[9].add_data(0, mRollingAverage[0].mx);
      mQuickGraph2[9].new_entry_pos();

   }
}




void mwLoop::initialize_and_resize_remote_graphs(void)
{
   // width and x position are based soley on screen width
   int width = remote_graphs_width;
   int gx = 4;

   // each graph has a suggested height, this will only be used to scale them relative to each other
   mQuickGraph2[0].set_size(width, 100,  0);
   mQuickGraph2[1].set_size(width, 100, 0);
   mQuickGraph2[2].set_size(width, 100, 0);
   mQuickGraph2[3].set_size(width, 40,  0);
   mQuickGraph2[4].set_size(width, 40,  0);
   mQuickGraph2[5].set_size(width, 40,  0);
   mQuickGraph2[6].set_size(width, 100, 0);
   mQuickGraph2[7].set_size(width, 100, 0);


   mQuickGraph2[9].set_size(200, 30, 1);


   // iterate all and tally graph height and space between them
   float gs=0; // graph space
   int gc=0; // graph count

   for (int i=0; i<8; i++)
      if (mQuickGraph2[i].active)
      {
         gs+=mQuickGraph2[i].height;
         gc++;
      }

   int bs = (gc-1) * 10; // buffer space
   float ts = remote_graphs_height - bs; // space available

   if ((gs != 0) && (ts > 0))
   {
      float ra = (float)ts / (float)gs;   // ratio of space available to requested
      // scale all the heights
      for (int i=0; i<8; i++)
         if (mQuickGraph2[i].active) mQuickGraph2[i].set_size(width, mQuickGraph2[i].height * ra, 1);
   }

   // set positions
   int gy = mDisplay.SCREEN_H;

   for (int i=0; i<8; i++)
      if (mQuickGraph2[i].active)
      {
         gy-= mQuickGraph2[i].height + 10;
         mQuickGraph2[i].set_pos(gx, gy);
      }

   al_set_target_backbuffer(mDisplay.display);

}



void mwLoop::initialize_graphs(void)
{
   if (!mMain.headless_server)
   {
      if (mMain.server_remote_control)
      {
         int width = remote_graphs_width;
         mQuickGraph2[0].initialize(width, 100,   0,  100, "CPU",      0, 12, 13, 1);
         mQuickGraph2[1].initialize(width, 100, -60,   20, "SYNC",     0, 10, 15, 1);
         mQuickGraph2[2].initialize(width, 100,   0,  100, "PING",     0, 14, 15, 1);
         mQuickGraph2[3].initialize(width, 40,    0, 9000, "DIF SIZE", 0, 11, 15, 1);
         mQuickGraph2[4].initialize(width, 40,    0,  900, "TX KBPS",  0, 8,  15, 0);
         mQuickGraph2[5].initialize(width, 40,    0,    8, "REWIND",   0, 5,  15, 1);
         mQuickGraph2[6].initialize(width, 100,   0,   40, "LCOR",     0, 6,  15, 1);
         mQuickGraph2[7].initialize(width, 100,   0,   40, "RCOR",     0, 7,  15, 1);
      }

      // for screen overlay common and remote local cpu
      mQuickGraph2[9].initialize(200,    36,    0,   50, "CPU",      9, 12, 13, 1);

      // client ping and sync
      mQuickGraph[1].initialize(2);

      // old style cpu
      mQuickGraph[0].initialize(1);
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
      mEventQueue.proc(0);

      // ----------------------------------------------------------
      // check for packets
      // ----------------------------------------------------------
      mPacketBuffer.check_for_packets();

      // ----------------------------------------------------------
      // temp testing as fast as it can go
      // ----------------------------------------------------------
      if (super_fast_mode) mEventQueue.program_update = 1;

      // ----------------------------------------------------------
      // do things based on the 40 Hz fps_timer event
      // ----------------------------------------------------------
      if (mEventQueue.program_update)
      {
         mEventQueue.program_update = 0;
         mInput.proc_keys_held();



// ----------------------------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------------------
// remote control loop
// ----------------------------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------------------
         if (state[1] == 41) remote_control_loop();

// ----------------------------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------------------
// main game loop
// ----------------------------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------------------

         if (state[1] == 11) // game loop running
         {
            frame_num++;
            frame_start_timestamp = al_get_time();

            // -----------------------------------
            // update animation and process sound
            // -----------------------------------
            if (!mDisplay.no_display)
            {
               mBitmap.update_animation();
               mSound.proc_sound();
            }

            // -----------------------------------
            // process scale factor changes
            // -----------------------------------
            int ldm = mPlayer.syn[0].level_done_mode;
            if (!mDisplay.no_display)
            {
               if ((ldm == 30) || (ldm == 25)) mDisplay.proc_custom_scale_factor_change();
               else                            mDisplay.proc_scale_factor_change();
            }

            mPacketBuffer.check_for_packets();

            // -----------------------------------
            // process server and client control
            // -----------------------------------
            if (mNetgame.ima_server) mNetgame.server_control();
            if (mNetgame.ima_client) mNetgame.client_control();

            // ------------------------------
            // process player input
            // ------------------------------
            mPlayer.proc_player_input();

            // ------------------------------
            // process game games array
            // ------------------------------
            mGameMoves.proc();

            mPacketBuffer.check_for_packets();

            // ------------------------------
            // move
            // ------------------------------
            move_frame();

            mPacketBuffer.check_for_packets();

            // ----------------------------------
            // create and send states to clients
            // ----------------------------------
            if (mNetgame.ima_server)
            {
               double t0 = al_get_time();
               mNetgame.server_create_new_state();
               mLog.add_tmr1(LOG_TMR_sdif, 0, "sdif", al_get_time() - t0);
            }


            mPacketBuffer.check_for_packets();

            // ------------------------------
            // draw
            // ------------------------------
            if ((!mDisplay.no_display) && (ldm != 27) && (!super_fast_mode)) mDrawSequence.draw(0);


            mPacketBuffer.check_for_packets();


            // --------------------------------------------
            // measure time to process loop
            // --------------------------------------------
            double pt = al_get_time() - frame_start_timestamp;
            mLog.add_tmr1(LOG_TMR_cpu, 0, "cpu", pt);

            // convert to 'cpu', a percent of the total frame time (25ms)
            float cpu = (pt / 0.025) * 100;

            // store in player array cpu variable
            mPlayer.loc[mPlayer.active_local_player].cpu = cpu;

            // store in local cpu variables
            add_local_cpu_data(cpu);


            for (int p=0; p<NUM_PLAYERS; p++)
            {
               mPlayer.loc[p].ping = mPacketBuffer.RA[p].last_input;
               mPlayer.loc[p].ping_avg = mPacketBuffer.RA[p].avg;
            }
         }
      }


      // ----------------------------------------------------------
      // do things based on the 1 Hz sec_timer event
      // ----------------------------------------------------------
      if (mEventQueue.program_update_1s)
      {
         mEventQueue.program_update_1s = 0;
         if (state[1] == 11) // game loop running
         {

            mPacketBuffer.process_tally();



            if (mNetgame.ima_client)
            {
               mNetgame.client_send_ping_packet();
               int p = mPlayer.active_local_player;
               mPlayer.loc[p].client_loc_plr_cor_avg = mTally_client_loc_plr_cor_last_sec[p].get_avg(0);
               mPlayer.loc[p].client_rmt_plr_cor_avg = mTally_client_rmt_plr_cor_last_sec[p].get_avg(0);
               mPlayer.loc[p].client_loc_plr_cor_max = mTally_client_loc_plr_cor_last_sec[p].get_max(1);
               mPlayer.loc[p].client_rmt_plr_cor_max = mTally_client_rmt_plr_cor_last_sec[p].get_max(1);
            }
            if (mNetgame.ima_server)
            {
               // tally late cdats and game move dsync
               for (int p=1; p<NUM_PLAYERS; p++)
                  if (mPlayer.syn[p].control_method == PM_PLAYER_CONTROL_METHOD_NETGAME_REMOTE)
                  {
                     mPlayer.syn[p].late_cdats_last_sec = mTally_late_cdats_last_sec[p].get_tally(1);
                     mPlayer.loc[p].game_move_dsync_avg_last_sec = mTally_game_move_dsync_avg_last_sec[p].get_avg(1);

                     mPlayer.loc[p].client_loc_plr_cor_avg = mTally_client_loc_plr_cor_last_sec[p].get_avg(0);
                     mPlayer.loc[p].client_rmt_plr_cor_avg = mTally_client_rmt_plr_cor_last_sec[p].get_avg(0);

                     mPlayer.loc[p].client_loc_plr_cor_max = mTally_client_loc_plr_cor_last_sec[p].get_max(1);
                     mPlayer.loc[p].client_rmt_plr_cor_max = mTally_client_rmt_plr_cor_last_sec[p].get_max(1);
                  }
            }
         }
      } // end of 1 Hz
   }
}
