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
#include "mwDemoRecord.h"

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
   for (int i=0; i<8; i++) state[i] = 0;

   top_menu_sel = 0;
   main_loop_exit = 0;

   level_editor_running = 0;
   help_screens_running = 0;
   visual_level_select_running = 0;

   show_debug_overlay = 0;

   // temp testing variables
   pct_x = 0;
   pct_y = 0;

   frame_speed = 40;
   frame_num = 0;
   speed_control_lock = 1;

   reset_frame_speed_at_program_start = 1;

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
   int ret = 1; // yes by default, if any have not then change to no

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
            state[0] = PM_PROGRAM_STATE_QUIT;
            return;
         }

         if (top_menu_sel == 7) // demo
         {
            mDemoMode.run_single_from_menu(); // this blocks until done
         }

         if (top_menu_sel == 0) // visual level select
         {
            int r = mVisualLevel.visual_level_select();
            if (r == 1) top_menu_sel = 1; // start new game
            if (r == 3) top_menu_sel = 6; // start level editor
         }
         if (top_menu_sel == 1) // start new game
         {
            mLevel.play_level = mLevel.start_level;
            state[0] = PM_PROGRAM_STATE_SINGLE_PLAYER_NEW_GAME;
            quit_action = 1;
            done_action = 1;
            top_menu_sel = 2; // resume
            return;
         }
         if ((top_menu_sel == 2) && (mLevel.resume_allowed))  // resume game
         {
            state[0] = PM_PROGRAM_STATE_RESUME;
            quit_action = 1;
            return;
         }
         if (top_menu_sel == 3)  // host network game
         {
            state[0] = PM_PROGRAM_STATE_SERVER_NEW_GAME;
            quit_action = 1;
            done_action = 1;
            return;
         }
         if (top_menu_sel == 4) // join network game
         {
            state[0] = PM_PROGRAM_STATE_CLIENT_NEW_GAME;
            quit_action = 1;
            done_action = 1;
            return;
         }
         if (top_menu_sel == 5) // settings
         {
            state[0] = PM_PROGRAM_STATE_CONFIG;
            mLoop.quit_action = 1; // menu
            return;
         }
         if (top_menu_sel == 6)  // level editor
         {
            mLevel.set_start_level(mWM.loop(mLevel.start_level));
            quit_action = 1; // menu
            state[0] = PM_PROGRAM_STATE_SINGLE_PLAYER_NEW_GAME;
            return;
         }

         if (top_menu_sel == 8) mHelp.help("");

         if (top_menu_sel == 66)
         {
            state[0] = PM_PROGRAM_STATE_DEMO_RECORD;
            return;
         }

         if ((top_menu_sel >= 100) && (top_menu_sel < 200)) // right pressed on menu item
         {
            top_menu_sel -= 100;
            if (top_menu_sel == 0)
            {
               if (mLevel.resume_allowed) mLevel.add_play_data_record(mLevel.play_level, 0);
               mLevel.set_start_level(mLevel.get_next_level(mLevel.start_level, 199, 0));
            }
         }
         if ((top_menu_sel >= 200) && (top_menu_sel < 300)) // left pressed on menu item
         {
            top_menu_sel -= 200;
            if (top_menu_sel == 0)
            {
               if (mLevel.resume_allowed) mLevel.add_play_data_record(mLevel.play_level, 0);
               mLevel.set_start_level(mLevel.get_prev_level(mLevel.start_level, 199, 0));
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
            state[0] = PM_PROGRAM_STATE_QUIT;
            return;
         }
         if (top_menu_sel == 0) // start new game
         {
            state[0] = PM_PROGRAM_STATE_SINGLE_PLAYER_NEW_GAME;
            quit_action = 1; // menu
            done_action = 2; // overworld
            return;
         }
         if (top_menu_sel == 1)  // host network game
         {
            state[0] = PM_PROGRAM_STATE_SERVER_NEW_GAME;
            quit_action = 1;
            done_action = 1;
            return;
         }
         if (top_menu_sel == 2) // join network game
         {
            state[0] = PM_PROGRAM_STATE_CLIENT_NEW_GAME;
            quit_action = 1;
            done_action = 1;
            return;
         }
         if (top_menu_sel == 3) // settings
         {
            state[0] = PM_PROGRAM_STATE_CONFIG;
            mLoop.quit_action = 1; // menu
            return;
         }
         if (top_menu_sel == 4) // demo mode
         {
            mDemoMode.run_continuous_random();
         }

         if (top_menu_sel == 5) mHelp.help(""); // help

         while (top_menu_sel >= 100) top_menu_sel-=100; // to account for left and right which are not used
      }
   }
}


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

      if (state[1] == PM_PROGRAM_STATE_MENU) // game menu or fast exit
      {
         mLog.add(LOG_OTH_program_state, 0, "[State 1 - Game Menu]\n");

         if (mNetgame.ima_server) mNetgame.ServerExitNetwork();
         if (mNetgame.ima_client) mNetgame.ClientExitNetwork();

         if (mLog.autosave_log_on_level_quit) mLog.save_log_file();

         if (mGameMoves.autosave_game_on_level_quit) mGameMoves.save_gm_make_fn("autosave on level quit");

         mSound.stop_sound();

         if (quit_action == 0)
         {
            mScreen.transition_cutscene(1, 0); // game to nothing
            state[0] = PM_PROGRAM_STATE_QUIT; // exit program
            return;
         }

         if (quit_action == 1) // menu (already set)
         {
            // only do this if not returning from settings or demo record
            if ((state[2] != PM_PROGRAM_STATE_CONFIG) && (state[2] != PM_PROGRAM_STATE_DEMO_RECORD)) mScreen.transition_cutscene(1, 2); // game to menu
         }
         if (quit_action == 2)  // overworld
         {
            mLog.add(LOG_OTH_program_state, 0, "instead of menu, go to overworld\n");
            mPlayer.syn[0].level_done_next_level = 1;
            state[0] = PM_PROGRAM_STATE_NEXT_LEVEL;      // next level
            quit_action = 1;    // menu
            done_action = 2;    // overworld (should never trigger level done from overworld, no exits)
            mLevel.add_play_data_record(mLevel.play_level, 0);
            return;
         }

         if (quit_action == 3)  // settings
         {
            mLog.add(LOG_OTH_program_state, 0, "instead of menu, go to settings\n");
            state[0] = PM_PROGRAM_STATE_CONFIG;
            quit_action = 1; // set new quit action to menu
            return;
         }
      }
   }


   if (state[1] == PM_PROGRAM_STATE_QUIT) main_loop_exit = 1; // quit
   if (state[1] == PM_PROGRAM_STATE_MENU) game_menu();  // game menu (this blocks)
   if (state[1] == PM_PROGRAM_STATE_CONFIG) mSettings.settings_pages(-1); // this blocks
   if (state[1] == PM_PROGRAM_STATE_DEMO_RECORD) mDemoRecord.demo_record(); // blocks




//-----------------------------------------------------------------
// PM_PROGRAM_STATE_CLIENT_PREEXIT1
//-----------------------------------------------------------------
   if (state[1] == PM_PROGRAM_STATE_CLIENT_PREEXIT1)
   {
      if (mGameMoves.autosave_game_on_level_quit)
      {
         // when client quits with escape, send a file request to the server to get the gm file
         mNetgame.client_send_crfl();
         state[0] = PM_PROGRAM_STATE_CLIENT_PREEXIT2;
      }
      else state[0] = PM_PROGRAM_STATE_CLIENT_EXIT;
   }

//-----------------------------------------------------------------
// PM_PROGRAM_STATE_CLIENT_PREEXIT2
//-----------------------------------------------------------------
   if (state[1] == PM_PROGRAM_STATE_CLIENT_PREEXIT2)
   {
      mScreen.rtextout_centre(mFont.bltn, NULL, mDisplay.SCREEN_W/2, mDisplay.SCREEN_H/2, 10, -2, 1, "Waiting for file from server");
      al_flip_display();
      mPacketBuffer.rx_and_proc(); // when file is received, state will be set to exit

      mEventQueue.proc(1);
      if (mInput.key[ALLEGRO_KEY_ESCAPE][2]) state[0] = PM_PROGRAM_STATE_CLIENT_EXIT; // give them an escape option
   }

//-----------------------------------------------------------------
// PM_PROGRAM_STATE_CLIENT_EXIT
//-----------------------------------------------------------------
   if (state[1] == PM_PROGRAM_STATE_CLIENT_EXIT)
   {
      mLog.add(LOG_OTH_program_state, 0, "[PM_PROGRAM_STATE_CLIENT_EXIT]\n");
      mNetgame.ClientExitNetwork();

      quit_action = 1; // to prevent quitting clients from automatically going to overworld
      state[0] = PM_PROGRAM_STATE_MENU;
   }

//-----------------------------------------------------------------
// PM_PROGRAM_STATE_CLIENT_NEW_GAME
//-----------------------------------------------------------------
   if (state[1] == PM_PROGRAM_STATE_CLIENT_NEW_GAME)
   {
      mLog.add(LOG_OTH_program_state, 0, "[PM_PROGRAM_STATE_CLIENT_NEW_GAME]\n");

      mLog.log_versions();
      mLog.add_fw (LOG_NET, 0, 76, 10, "+", "-", "");
      mLog.add_fwf(LOG_NET, 0, 76, 10, "|", " ", "Client mode started on localhost:[%s]", mLoop.local_hostname);

      mEventQueue.reset_fps_timer();

      for (int p=0; p<NUM_PLAYERS; p++) mPlayer.init_player(p, 1); // full reset
      mPlayer.syn[0].active = 1;


      if (!mNetgame.ClientInitNetwork())
      {
         state[0] = PM_PROGRAM_STATE_CLIENT_EXIT;
         return;
      }

      if (!mNetgame.ClientJoin())
      {
         state[0] = PM_PROGRAM_STATE_CLIENT_EXIT;
         return;
      }

      mRollingAverage[1].initialize(8); // ping rolling average
      mRollingAverage[2].initialize(8); // dsync rolling average

      state[0] = PM_PROGRAM_STATE_CLIENT_LEVEL_SETUP;
   }

//-----------------------------------------------------------------
// PM_PROGRAM_STATE_CLIENT_LEVEL_SETUP
//-----------------------------------------------------------------
   if (state[1] == PM_PROGRAM_STATE_CLIENT_LEVEL_SETUP)
   {
      mLog.add(LOG_OTH_program_state, 0, "[PM_PROGRAM_STATE_CLIENT_LEVEL_SETUP]\n");

      if (load_and_setup_level(mLevel.play_level, 2)) state[0] = PM_PROGRAM_STATE_CLIENT_WAIT_FOR_INITIAL_STATE;
      else state[0] = PM_PROGRAM_STATE_CLIENT_EXIT;
   }

//-----------------------------------------------------------------
// PM_PROGRAM_STATE_CLIENT_WAIT_FOR_INITIAL_STATE
//-----------------------------------------------------------------
   if (state[1] == PM_PROGRAM_STATE_CLIENT_WAIT_FOR_INITIAL_STATE)
   {
      mLog.add(LOG_OTH_program_state, 0, "[PM_PROGRAM_STATE_CLIENT_WAIT_FOR_INITIAL_STATE]\n");

      mScreen.rtextout_centre(mFont.bltn, NULL, mDisplay.SCREEN_W/2, mDisplay.SCREEN_H/2, 10, -2, 1, "Waiting for game state from server");
      al_flip_display();
      if (mInput.key[ALLEGRO_KEY_ESCAPE][3]) state[0] = PM_PROGRAM_STATE_CLIENT_EXIT;

      mPacketBuffer.rx_and_proc();

      mNetgame.client_apply_dif();

      if (frame_num > 0) state[0] = PM_PROGRAM_STATE_MAIN_GAME_LOOP;
   }

//---------------------------------------
// PM_PROGRAM_STATE_SERVER_NEW_GAME
//---------------------------------------
   if (state[1] == PM_PROGRAM_STATE_SERVER_NEW_GAME)
   {
      mLog.add(LOG_OTH_program_state, 0, "[PM_PROGRAM_STATE_SERVER_NEW_GAME]\n");
      if (!mNetgame.ServerInitNetwork())
      {
         state[0] = PM_PROGRAM_STATE_SERVER_EXIT;
         return;
      }

      mEventQueue.reset_fps_timer();

      if (!load_and_setup_level(mLevel.play_level, 3)) state[0] = PM_PROGRAM_STATE_SERVER_EXIT;

   }

//---------------------------------------
// PM_PROGRAM_STATE_SERVER_EXIT
//---------------------------------------
   if (state[1] == PM_PROGRAM_STATE_SERVER_EXIT)
   {
      mLog.add(LOG_OTH_program_state, 0, "[PM_PROGRAM_STATE_SERVER_EXIT]\n");
      mNetgame.ServerExitNetwork();
      state[0] = PM_PROGRAM_STATE_MENU;
   }

//---------------------------------------
// PM_PROGRAM_STATE_SINGLE_PLAYER_NEW_GAME
//---------------------------------------
   if (state[1] == PM_PROGRAM_STATE_SINGLE_PLAYER_NEW_GAME)
   {
      mLog.add(LOG_OTH_program_state, 0, "[PM_PROGRAM_STATE_SINGLE_PLAYER_NEW_GAME]\n");

      if (!mMain.classic_mode) mLevel.start_level = mLevel.play_level = 1;

      if (!load_and_setup_level(mLevel.play_level, 0)) state[0] = PM_PROGRAM_STATE_MENU;

      if (quit_action == 0) mScreen.transition_cutscene(0, 1); // nothing to game
      if (quit_action == 1) mScreen.transition_cutscene(2, 1); // menu to game
   }

//----------------------------------------------------------------------------------------------------------
// PM_PROGRAM_STATE_NEXT_LEVEL
//----------------------------------------------------------------------------------------------------------
   if (state[1] == PM_PROGRAM_STATE_NEXT_LEVEL)
   {
      mLog.addf(LOG_OTH_program_state, 0, "[PM_PROGRAM_STATE_NEXT_LEVEL]  [play lev:%d]  [next lev:%d]\n", mLevel.play_level, mPlayer.syn[0].level_done_next_level);

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
         if (++mPlayer.syn[0].server_lev_seq_num > 255) mPlayer.syn[0].server_lev_seq_num = 0;
      }
      if (mNetgame.ima_server)
      {
         mNetgame.ServerFlush();
         mLog.log_ending_stats_server(LOG_NET_ending_stats);
         if (++mPlayer.syn[0].server_lev_seq_num > 255) mPlayer.syn[0].server_lev_seq_num = 0;
         if (mLog.log_types[LOG_NET_session].action) mNetgame.session_save_active_at_level_done();
      }
      if (mLog.autosave_log_on_level_done) mLog.save_log_file();
      if (mGameMoves.autosave_game_on_level_done) mGameMoves.save_gm_make_fn("autosave on level done");


// --------------------------------------------------------
// instant program quit
// started from command line and next level called
// --------------------------------------------------------
      if (done_action == 0) // command line
      {
         mLog.add(LOG_OTH_program_state, 0, "command line exit\n");
         state[0] = PM_PROGRAM_STATE_QUIT; // exit
         mScreen.transition_cutscene(1, 0); // game to nothing
         return; // to exit immediately
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
// -----------------------------------------------------------------------------------------------------
      if ((mLevel.play_level == 1) && (mPlayer.syn[0].level_done_next_level != 1))
      {
         mLog.add(LOG_OTH_transitions, 0, "Next level from overworld\n");
         post_load_transistion_initial = 3; // gate
         post_load_transistion_final   = 1; // game
      }

// -----------------------------------------------------------------------------------------------------
// if going from any level to any level (except overworld)
// also test that previous state == 11  to not trigger when started from menu or settings
// -----------------------------------------------------------------------------------------------------
      if ((mLevel.play_level != 1) && (mPlayer.syn[0].level_done_next_level != 1) && (state[2] == PM_PROGRAM_STATE_MAIN_GAME_LOOP))
      {
         mLog.add(LOG_OTH_transitions, 0, "Level to level (no overworld)\n");
         pre_load_transistion_initial  = 1; // game
         pre_load_transistion_final    = 0; // nothing
         post_load_transistion_initial = 0; // nothing
         post_load_transistion_final   = 1; // game
      }

// ------------------------------------------------------------------------------------------------------------------------------------
// ---   pre load transition
// ------------------------------------------------------------------------------------------------------------------------------------
      mLog.add(LOG_OTH_transitions, 0, "pre-load ");
      mScreen.transition_cutscene(pre_load_transistion_initial, pre_load_transistion_final);

// ---------------------------------------------------
// load the new level
// ---------------------------------------------------
      mLevel.play_level = mPlayer.syn[0].level_done_next_level;
      if (!load_and_setup_level(mLevel.play_level, 1)) state[0] = PM_PROGRAM_STATE_MENU;

      //if (mMain.headless_server) printf("Started Level:%d\n", mLevel.play_level);

// ----------------------------------------
// post-load transition
// ----------------------------------------
      mLog.add(LOG_OTH_transitions, 0, "post-load ");
      mScreen.transition_cutscene(post_load_transistion_initial, post_load_transistion_final);
   }


   //---------------------------------------
   // PM_PROGRAM_STATE_RESUME
   //---------------------------------------
   if (state[1] == PM_PROGRAM_STATE_RESUME)
   {
      mLog.add(LOG_OTH_program_state, 0, "[PM_PROGRAM_STATE_RESUME]\n");
      mSound.start_music(1); // resume theme
      mScreen.transition_cutscene(2, 1); // menu to game
      state[0] = PM_PROGRAM_STATE_MAIN_GAME_LOOP;
   }

   //-------------------------------------------------------
   // PM_PROGRAM_STATE_SERVER_REMOTE_CONTROL_SETUP
   //-------------------------------------------------------
   if (state[1] == PM_PROGRAM_STATE_SERVER_REMOTE_CONTROL_SETUP)
   {
      mLog.add(LOG_OTH_program_state, 0, "[PM_PROGRAM_STATE_SERVER_REMOTE_CONTROL_SETUP]\n");
      printf("server remote control setup\n");

      for (int p=0; p<NUM_PLAYERS; p++) mPlayer.init_player(p, 1); // full reset



      if (!mNetgame.ClientInitNetwork())
      {
         state[0] = PM_PROGRAM_STATE_QUIT; // quit
         return;
      }

      if (!mNetgame.RemoteJoin())
      {
         state[0] = PM_PROGRAM_STATE_QUIT; // quit
         return;
      }


      state[0] = PM_PROGRAM_STATE_SERVER_REMOTE_CONTROL_RUN;
      printf("server remote control run\n");
      mLog.add(LOG_OTH_program_state, 0, "[State 40 - Server Remote Control Run]\n");
      initialize_graphs();
   }
}


/*
0 - SINGLE_PLAYER_NEW_GAME
1 - NEXT_LEVEL
2 - CLIENT
3 - SERVER_NEW_GAME
4 - DEMO
5 - LOAD AND RESET ALL BUT DO NOT START LEVEL


*/
int mwLoop::load_and_setup_level(int level, int type)
{
   if (!mLevel.load_level(level, 0, 0)) return 0;
   else
   {

      if (type != 4) mGameMoves.initialize(); // do not reset game moves in demo mode

      if (type == 0) // SINGLE_PLAYER_NEW_GAME
      {
         setup_players_after_level_load(1); // type 1 full reset,
      }

      if (type == 1) // NEXT_LEVEL
      {
         setup_players_after_level_load(2); // type 2 partial reset,
      }

      if (type == 2) // CLIENT_LEVEL_SETUP
      {
         setup_players_after_level_load(2); // type 2 partial reset,
      }

      if (type == 3) // SERVER_NEW_GAME
      {
         setup_players_after_level_load(1); // type 1 full reset,
         strncpy(mPlayer.loc[0].hostname, local_hostname, 16);
         mPlayer.syn[0].control_method = PM_PLAYER_CONTROL_METHOD_SERVER_LOCAL;
         mPlayer.syn[0].server_lev_seq_num = 0;
      }

      if (type == 4) // DEMO
      {
         setup_players_after_level_load(1); // type 1 full reset,
         mPlayer.syn[0].control_method = PM_PLAYER_CONTROL_METHOD_DEMO_MODE;
      }

      if (type == 5) // LOAD AND RESET ALL BUT DO NOT START LEVEL
      {
         setup_players_after_level_load(1); // type 1 full reset,
      }

      initialize_graphs();
      mNetgame.reset_states();
      mInput.initialize();
      mBottomMessage.initialize();
      mTriggerEvent.initialize();
      mShot.clear_shots();
      frame_num = 0;

      if (type == 5) return 1; // LOAD AND RESET ALL BUT DO NOT START LEVEL

      mSound.start_music(0); // rewind and start theme

      if (!mDemoMode.mode)
      {
         state[0] = PM_PROGRAM_STATE_MAIN_GAME_LOOP;
         // add initial special game moves

         mLog.add_headerf(LOG_NET, 0, 1, "LEVEL %d STARTED", mLevel.play_level);

         mGameMoves.add_game_move(0, PM_GAMEMOVE_TYPE_LEVEL_START, 0, mLevel.play_level);

         mGameMoves.add_game_move(1, PM_GAMEMOVE_TYPE_SHOT_CONFIG, 0, 0);

         // save colors in game moves array
         for (int p=0; p<NUM_PLAYERS; p++)
            if (mPlayer.syn[p].active) mGameMoves.add_game_move(1, PM_GAMEMOVE_TYPE_PLAYER_ACTIVE, p, mPlayer.syn[p].color);

         // add game move to hide player 0 in headless server mode
         if (mMain.headless_server) mGameMoves.add_game_move(1, PM_GAMEMOVE_TYPE_PLAYER_HIDDEN, 0, 0);
      }

   }
   return 1;
}

void mwLoop::setup_players_after_level_load(int type)
{
   for (int p=0; p<NUM_PLAYERS; p++)
   {
      mPlayer.init_player(p, type);     // type 1 = full reset, type 2 = level done reset
      mItem.set_player_start_pos(p);    // get starting position for all players, active or not
   }
   mPlayer.syn[0].active = 1;
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
      mQuickGraph2[9].add_data(0, mRollingAverage[0].mx, 0);
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
         if (state[1] == PM_PROGRAM_STATE_SERVER_REMOTE_CONTROL_RUN) remote_control_loop();

// ----------------------------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------------------
// main game loop
// ----------------------------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------------------

         if (state[1] == PM_PROGRAM_STATE_MAIN_GAME_LOOP) // game loop running
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

            // ------------------------------
            // draw
            // ------------------------------
            mDrawSequence.ds_draw(0, 1);


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

            // why is this all the time? only when client?, server, both??
            if ((mNetgame.ima_server) || (mNetgame.ima_client))
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
         if (state[1] == PM_PROGRAM_STATE_MAIN_GAME_LOOP) // game loop running
         {
            if (mNetgame.ima_client)
            {
               mPacketBuffer.process_tally();
               mNetgame.client_send_ping_packet();
               int p = mPlayer.active_local_player;
               mPlayer.loc[p].client_loc_plr_cor_avg = mTally_client_loc_plr_cor_last_sec[p].get_avg(0);
               mPlayer.loc[p].client_rmt_plr_cor_avg = mTally_client_rmt_plr_cor_last_sec[p].get_avg(0);
               mPlayer.loc[p].client_loc_plr_cor_max = mTally_client_loc_plr_cor_last_sec[p].get_max(1);
               mPlayer.loc[p].client_rmt_plr_cor_max = mTally_client_rmt_plr_cor_last_sec[p].get_max(1);
            }
            if (mNetgame.ima_server)
            {
               mPacketBuffer.process_tally();
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
