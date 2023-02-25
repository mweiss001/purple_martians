// z_loop.cpp
#include "pm.h"
#include "z_loop.h"
#include "mwQuickGraph.h"
#include "mwSound.h"
#include "mwLog.h"
#include "mwSettings.h"
#include "mwPlayers.h"
#include "n_netgame.h"
#include "mwRollingAverage.h"
#include "mwTally.h"
#include "mwDrawSequence.h"
#include "mwLogo.h"
#include "mwBottomMessage.h"
#include "mwDemoMode.h"
#include "mwDisplay.h"
#include "mwTimeStamp.h"
#include "mwFont.h"
#include "mwLift.h"
#include "n_client.h"
#include "n_server.h"
#include "mwVisualLevel.h"
#include "mwGameMovesArray.h"
#include "mwPMEvent.h"
#include "mwInput.h"
#include "mwEventQueue.h"
#include "mwBitmap.h"
#include "mwMenu.h"
#include "mwHelp.h"
#include "mwProgramState.h"
#include "mwItems.h"
#include "mwEnemy.h"
#include "mwLevel.h"
#include "e_editor_main.h"
#include "z_control.h"
#include "z_screen.h"
#include "z_screen_overlay.h"
#include "mwShots.h"




void draw_frame(void)
{
   mwDS.draw();
//   get_new_background(1);
//   draw_lifts();
//   draw_items();
//   draw_enemies();
//   draw_eshots();
//   draw_pshots();
//   draw_players();
//   get_new_screen_buffer(0, 0, 0);
//   draw_screen_overlay();
//   al_flip_display();
}

void move_frame(void)
{
   char msg[1024];
   double t1, t2, t3, t4, t5, t6;
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


void loop_frame(int times) // used for fast forwarding after rewind
{
   for (int i=0; i<times; i++)
   {
      mwGMA.proc_game_moves_array();
      if (mPlayer.syn[0].level_done_mode) proc_level_done_mode();
      else move_frame();
      mwPS.frame_num++;
   }
}

int have_all_players_acknowledged(void)
{
   int ret = 1; // yes by default
   for (int p=0; p<NUM_PLAYERS; p++)
   {
      if (mPlayer.syn[p].active)
      {
         if (mwGMA.has_player_acknowledged(p))
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

void game_menu(void)
{
   mwPS.old_program_state = 1;
   if (!mwL.splash_screen_done) { mwL.splash_screen(); mwL.splash_screen_done = 1; }
   if (!mLevel.resume_allowed) mLevel.set_start_level();
   if (mwPS.top_menu_sel < 3) mwPS.top_menu_sel = 3;
   while (mwPS.top_menu_sel != 1)
   {
//      mwPS.top_menu_sel = zmenu(7, mwPS.top_menu_sel, 10);

      mwPS.top_menu_sel = mMenu.zmenu(mwPS.top_menu_sel, 10);

      if  (mwPS.top_menu_sel == 1)  { mwPS.program_state = 0;                                           return; } // exit
      if  (mwPS.top_menu_sel == 2)  { if (mVisualLevel.visual_level_select()) mwPS.top_menu_sel = 3;                         } // visual level select
      if ((mwPS.top_menu_sel == 4) && (mLevel.resume_allowed)) { mwPS.new_program_state = 13;           return; } // resume game
      if  (mwPS.top_menu_sel == 3)  { mwPS.new_program_state = 10;  mwPS.top_menu_sel = 4;              return; } // start new game
      if  (mwPS.top_menu_sel == 5)  { mwPS.new_program_state = 20;                                      return; } // host network game
      if  (mwPS.top_menu_sel == 6)  { mwPS.new_program_state = 24;                                      return; } // join network game
      if  (mwPS.top_menu_sel == 7)  { mwPS.new_program_state = 3;                                       return; } // settings
      if  (mwPS.top_menu_sel == 8)  { mLevel.set_start_level(edit_menu(mLevel.start_level)); mwPS.new_program_state = 10; return; } // level editor
      if  (mwPS.top_menu_sel == 9)  { mwPS.new_program_state = 2;  mwPS.older_program_state = 1;        return; } // demo mode
      if  (mwPS.top_menu_sel == 10)                                                                     mHelp.help(""); // help
      if ((mwPS.top_menu_sel > 100) && (mwPS.top_menu_sel < 200)) // right pressed on menu item
      {
         mwPS.top_menu_sel -= 100;
         if (mwPS.top_menu_sel == 2) // next level
         {
            mLevel.next_level();
         }
      }
      if ((mwPS.top_menu_sel > 200) && (mwPS.top_menu_sel < 300)) // left pressed on menu item
      {
         mwPS.top_menu_sel -= 200;
         if (mwPS.top_menu_sel == 2) // prev level
         {
            mLevel.prev_level();
         }
      }
   }
}

void proc_program_state(void)
{
   char msg[1024];
   // ----------------------------------------------------------
   // handle all the changes from one state to another
   // ----------------------------------------------------------
   if (mwPS.new_program_state)
   {
      if (mwPS.new_program_state != mwPS.program_state)
      {
         if (mwPS.new_program_state ==  2) mwPS.program_state =  2; // demo_mode
         if (mwPS.new_program_state ==  3) mwPS.program_state =  3; // config
         if (mwPS.new_program_state == 10) mwPS.program_state = 10; // start new game
         if (mwPS.new_program_state == 12) mwPS.program_state = 12; // level_done
         if (mwPS.new_program_state == 13) mwPS.program_state = 13; // resume
         if (mwPS.new_program_state == 14) mwPS.program_state = 14; // run demo
         if (mwPS.new_program_state == 20) mwPS.program_state = 20; // start server game
         if (mwPS.new_program_state == 21) mwPS.program_state = 21; // client wait for initial state
         if (mwPS.new_program_state == 22) mwPS.program_state = 22; // client load level and set up
         if (mwPS.new_program_state == 23) mwPS.program_state = 23; // client wait for sjon
         if (mwPS.new_program_state == 24) mwPS.program_state = 24; // client init network and send cjon
         if (mwPS.new_program_state == 25) mwPS.program_state = 25; // client exit and clean up network

         if (mwPS.new_program_state == 1) // game menu or fast exit
         {
            if (ima_server) server_exit();
            if (ima_client) client_exit();

            if (mLog.autosave_log_on_game_exit) mLog.save_log_file();
            if (mLog.autosave_game_on_game_exit) mwGMA.blind_save_game_moves(2);

            mSound.stop_sound();
            if (mwPS.program_state != 3) stamp();

            mwPS.program_state = mwPS.old_program_state; // go back to the state that called 0,1,2 or 3
         }
      }
      mwPS.new_program_state = 0;
   }
   if (mwPS.program_state == 0) mwPS.main_loop_exit = 1; // quit
   if (mwPS.program_state == 1) game_menu();  // game menu (this blocks)
   if (mwPS.program_state == 2) mwDM.demo_mode();  // demo mode
   if (mwPS.program_state == 3) mSettings.settings_pages(-1);  // this blocks

   //---------------------------------------
   // 25 - client exit
   //---------------------------------------
   if (mwPS.program_state == 25) // client exit
   {
      client_exit();
      mwPS.new_program_state = 1;
   }


   //---------------------------------------
   // 24 - client new game
   //---------------------------------------
   if (mwPS.program_state == 24)
   {
      if (!client_init())
      {
         mwPS.new_program_state = 25;
         return;
      }

      // set up mwQuickGraph here
      // set up mwQuickGraph here
      mwQG[0].initialize(1);
      mwQG[1].initialize(2);
      mwQG[2].initialize(2);

      for (int p=0; p<NUM_PLAYERS; p++) mPlayer.init_player(p, 1); // full reset
      mPlayer.syn[0].active = 1;

      mwPS.new_program_state = 23;
   }

   //---------------------------------------
   // 23 - client wait for join
   //---------------------------------------
   if (mwPS.program_state == 23)
   {
      client_fast_packet_loop();
      client_read_packet_buffer();
      if (mI.key[ALLEGRO_KEY_ESCAPE][1]) mwPS.new_program_state = 25; // give them an escape option
   }

   //---------------------------------------
   // 22 - client level setup
   //---------------------------------------
   if (mwPS.program_state == 22)
   {
      printf("client set up level\n");


      if (!mLevel.load_level(mLevel.play_level, 0, 0))
      {
         mwPS.new_program_state = 25;
         return;
      }


      for (int p=0; p<NUM_PLAYERS; p++)
         mPlayer.set_player_start_pos(p, 0);     // get starting position for all players, active or not


      mwGMA.initialize();

      mwPS.frame_num = 0;
      reset_states();
      mShot.clear_shots();
      mwBM.initialize();
      mI.initialize();
      mwPME.initialize();

      if (mLog.LOG_NET)
      {
         sprintf(msg,"LEVEL %d STARTED", mLevel.play_level);
         mLog.add_log_entry_header(10, 0, msg, 3);
      }

      mwPS.show_player_join_quit_timer = 0;
      mSound.start_music(0);
      mwTS.init_timestamps();
      mwPS.new_program_state = 21;
   }

   //---------------------------------------
   // 21 - client wait for intial state
   //---------------------------------------
   if (mwPS.program_state == 21)
   {
      client_fast_packet_loop();
      client_read_packet_buffer();
      client_apply_dif();

      if (mI.key[ALLEGRO_KEY_ESCAPE][3]) mwPS.program_state = 25;

      sprintf(msg, "Waiting for game state from server");
      float stretch = ( (float)mwD.SCREEN_W / ((strlen(msg)+2)*8));
      rtextout_centre(mF.bltn, NULL, msg, mwD.SCREEN_W/2, mwD.SCREEN_H/2, 10, stretch, 1);

      al_flip_display();

      if (mwPS.frame_num > 0)
      {
         printf("got initial state for frame:%d\n", mwPS.frame_num);
         int p = mPlayer.active_local_player;

         // set holdoff 200 frames in future so client won't try to drop while syncing
         mPlayer.loc[p].client_last_stdf_rx_frame_num = mwPS.frame_num + 200;

         if (mLog.LOG_NET_join) mLog.add_log_entry_header(11, p, "Game state updated - starting chase and lock", 1);
         mwPS.program_state = 11;
      }
   }




   //---------------------------------------
   // 20 - server new game
   //---------------------------------------
   if (mwPS.program_state == 20)
   {
      if (!server_init())
      {
         server_exit();
         mwPS.new_program_state = 19;
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
      strncpy(mPlayer.loc[0].hostname, mwPS.local_hostname, 16);


      mwGMA.initialize();
      mwPS.frame_num = 0;
      reset_states();
      mShot.clear_shots();
      mwBM.initialize();
      mI.initialize();
      mwPME.initialize();

      game_vars_to_state(srv_client_state[0][1]);
      srv_client_state_frame_num[0][1] = mwPS.frame_num;
      if (mLog.LOG_NET_stdf)
      {
         sprintf(msg, "stdf saved server state[1]:%d\n", mwPS.frame_num);
         mLog.add_log_entry2(27, 0, msg);
      }

      mwGMA.add_game_move(0, 0, 0, mLevel.play_level);       // [00] game_start

      // save colors in game moves array
      for (int p=0; p<NUM_PLAYERS; p++)
         if (mPlayer.syn[p].active) mwGMA.add_game_move(0, 1, p, mPlayer.syn[p].color); // 1 - player_state and color

      if (mLog.LOG_NET)
      {
         sprintf(msg,"LEVEL %d STARTED", mLevel.play_level);
         mLog.add_log_entry_header(10, 0, msg, 3);
      }

      mwPS.show_player_join_quit_timer = 0;
      mSound.start_music(0); // rewind and start theme
      mwTS.init_timestamps();
      mwPS.program_state = 11;
   }

   //---------------------------------------
   // 19 - server exit
   //---------------------------------------
   if (mwPS.program_state == 19) // server exit
   {
      server_exit();
      mwPS.new_program_state = 1;
   }

   //---------------------------------------
   // 10 - single player new game
   //---------------------------------------
   if (mwPS.program_state == 10)
   {

      mLevel.set_start_level();

      for (int p=0; p<NUM_PLAYERS; p++)
      {
         mPlayer.init_player(p, 1);            // full reset
         mPlayer.set_player_start_pos(p, 0);   // get starting position for all players, active or not
      }
      mPlayer.syn[0].active = 1;

      mwGMA.initialize();

      mShot.clear_shots();
      mwBM.initialize();
      mI.initialize();
      mwPME.initialize();

      stimp();
      mwPS.frame_num = 0;
      mwPS.show_player_join_quit_timer = 0;
      mSound.start_music(0); // rewind and start theme

      mwTS.init_timestamps();
      mwPS.program_state = 11;

      // set up mwQuickGraph here
      mwQG[0].initialize(1);
      mwQG[1].initialize(2);


   }


   //---------------------------------------
   // 12 - level done
   //---------------------------------------
   if (mwPS.program_state == 12)
   {
      mSound.stop_sound();

      mPlayer.syn[0].level_done_mode = 0;

      if (mLog.LOG_NET) { sprintf(msg,"NEXT LEVEL:%d", mPlayer.syn[0].level_done_next_level); mLog.add_log_entry_header(10, 0, msg, 3); }

      if (mPlayer.syn[mPlayer.active_local_player].control_method == 1) // run demo mode saved game file
      {
         mwPS.new_program_state = 2;
         al_rest(1);
         return; // to exit immediately
      }

      if ((mLog.LOG_NET) && (ima_client)) mLog.log_ending_stats(mPlayer.active_local_player);
      if ((mLog.LOG_NET) && (ima_server)) mLog.log_ending_stats_server();

      if (ima_server) server_flush();
      if (ima_client) client_flush();

      mwGMA.blind_save_game_moves(1);

      if (mLog.autosave_log_on_level_done) mLog.save_log_file();

      mLevel.play_level = mPlayer.syn[0].level_done_next_level;

      if (0) // reset clients
      {
         if ((ima_client) || (ima_server))
         {
            for (int p=0; p<NUM_PLAYERS; p++)
            {
               // free all the used clients, so they can be re-assigned on the next level
               // if (mPlayer.syn[p].control_method == 9) mPlayer.syn[p].control_method = 0;

               // set all clients inactive on server and client, to force them to re-chase and lock on the new level
               // if ((mPlayer.syn[p].control_method == 2) || (mPlayer.syn[p].control_method == 4)) mPlayer.syn[p].active = 0;

            }
         }
      }



      // every mode after this should require load level so why don't I do it here at the top
      if (!mLevel.load_level(mLevel.play_level, 0, 0))
      {
         mwPS.new_program_state = 1;
         return;
      }

      // reset players
      for (int p=0; p<NUM_PLAYERS; p++)
      {
         mPlayer.init_player(p, 2);            // next level reset
         mPlayer.set_player_start_pos(p, 0);   // get starting position for all players, active or not
      }
      mPlayer.syn[0].active = 1;

      mwGMA.initialize();


      mwPS.frame_num = 0;
      reset_states();
      mShot.clear_shots();
      mwBM.initialize();
      mI.initialize();
      mwPME.initialize();

      if (ima_server) // set server initial state (for both 2-new game and 5-level done when server)
      {
         mPlayer.syn[0].control_method = 3;

         game_vars_to_state(srv_client_state[0][1]);
         srv_client_state_frame_num[0][1] = mwPS.frame_num;

         if (mLog.LOG_NET_stdf)
         {
            //   printf("saved server state[1]:%d\n\n", mwPS.frame_num);
            sprintf(msg, "stdf saved server state[1]:%d\n", mwPS.frame_num);
            mLog.add_log_entry2(27, 0, msg);
         }
      }

      mwGMA.add_game_move(0, 0, 0, mLevel.play_level);       // [00] game_start

      // save colors in game moves array
      for (int p=0; p<NUM_PLAYERS; p++)
         if (mPlayer.syn[p].active) mwGMA.add_game_move(0, 1, p, mPlayer.syn[p].color); // [01] player_state and color

      if (mLog.LOG_NET)
      {
         sprintf(msg,"LEVEL %d STARTED", mLevel.play_level);
         mLog.add_log_entry_header(10, 0, msg, 3);
      }

      // only do fancy zoom into level if not in netgame  .. also warp if I can figure out how to do that here
      if ((!ima_client) && (!ima_server)) stimp();

      mwPS.show_player_join_quit_timer = 0;
      mSound.start_music(0); // rewind and start theme
      mwTS.init_timestamps();
      mwPS.program_state = 11;
   }


   //---------------------------------------
   // resume game
   //---------------------------------------
   if (mwPS.program_state == 13)
   {
      mSound.start_music(1); // resume theme
      stimp();
      mwPS.program_state = 11;
   }


   //---------------------------------------
   // run demo
   //---------------------------------------
   if (mwPS.program_state == 14)
   {
      if (!mLevel.load_level(mLevel.play_level, 0, 0))
      {
         mwPS.new_program_state = 1;
         return;
      }

      // reset players
      for (int p=0; p<NUM_PLAYERS; p++)
      {
         mPlayer.init_player(p, 1);           // full reset (start modes 1, 2, 3, 9)
         mPlayer.set_player_start_pos(p, 0);  // get starting position for all players, active or not
      }

      mPlayer.syn[0].active = 1;
      mPlayer.syn[0].control_method = 1; // rungame demo mode

      mwPS.frame_num = 0;
      mShot.clear_shots();
      mwBM.initialize();
      mI.initialize();
      mwPME.initialize();

      mwPS.show_player_join_quit_timer = 0;
      mSound.start_music(0); // rewind and start theme
      stimp();
      mwTS.init_timestamps();
      mwPS.program_state = 11;
   }
}


void proc_level_done_mode(void)
{
   if (mPlayer.syn[0].level_done_mode == 9) // pause players and set up exit xyincs
   {
      set_player_join_quit_display(mPlayer.syn[0].level_done_player, 2, 60);

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
      if (!ima_client)
      {
         if (have_all_players_acknowledged()) mPlayer.syn[0].level_done_timer = 0; // skip
      }
   }
   if (--mPlayer.syn[0].level_done_timer <= 0) // time to change to next level_done_mode
   {
      mPlayer.syn[0].level_done_mode--;
      if (mPlayer.syn[0].level_done_mode == 8) mPlayer.syn[0].level_done_timer = 60; // players seek exit
      if (mPlayer.syn[0].level_done_mode == 7) mPlayer.syn[0].level_done_timer = 20; // players shrink and rotate into exit
      if (mPlayer.syn[0].level_done_mode == 6) mPlayer.syn[0].level_done_timer = 0;
      if (mPlayer.syn[0].level_done_mode == 5) mPlayer.syn[0].level_done_timer = 600; // skippable 15s delay;
      if (mPlayer.syn[0].level_done_mode == 4) mPlayer.syn[0].level_done_timer = 0;
      if (mPlayer.syn[0].level_done_mode == 3) mPlayer.syn[0].level_done_timer = 0;
      if (mPlayer.syn[0].level_done_mode == 2) mPlayer.syn[0].level_done_timer = 10;  // delay to load next level
      if (mPlayer.syn[0].level_done_mode == 1) mwPS.program_state = 12;                // load new level
   }
}










void main_loop(void)
{
   while (!mwPS.main_loop_exit)
   {
      // ----------------------------------------------------------
      // process state and state changes
      // ----------------------------------------------------------
      proc_program_state();


      // ----------------------------------------------------------
      // process event queue
      // ----------------------------------------------------------
      mwEQ.proc_event_queue();


      // ----------------------------------------------------------
      // process fast packet loops
      // ----------------------------------------------------------
      if (ima_server) server_fast_packet_loop();
      if (ima_client) client_fast_packet_loop();


      // ----------------------------------------------------------
      // do things based on the 40 Hz fps_timer event
      // ----------------------------------------------------------
      if (mwEQ.program_update)
      {
         mwEQ.program_update = 0;

         if (mwPS.program_state == 11) // game loop running
         {
            mwPS.frame_num++;
            mwB.update_animation();

            mwTS.timestamp_frame_start = al_get_time();

            mwD.proc_scale_factor_change();

            if (ima_server) server_control();
            if (ima_client) client_control();

            proc_player_input();
            mwGMA.proc_game_moves_array();

            if (mPlayer.syn[0].level_done_mode) proc_level_done_mode();
            else move_frame();

            if (mLog.LOG_TMR_sdif) t0 = al_get_time();
            server_create_new_state();
            if (mLog.LOG_TMR_sdif) mLog.add_log_TMR(al_get_time() - t0, "sdif", 0);

            draw_frame();

            double pt = al_get_time() - mwTS.timestamp_frame_start;
            if (mLog.LOG_TMR_cpu) mLog.add_log_TMR(pt, "cpu", 0);

            mwRA[0].add_data((pt/0.025)*100);
            mwQG[0].add_data(0, mwRA[0].last_input);
            mwQG[0].add_data(1, mwRA[0].mn);
            mwQG[0].add_data(2, mwRA[0].mx);
            mwQG[0].add_data(3, mwRA[0].avg);

         }
      }


      // send client ping at this time in the loop, if flag is set
      if ((mPlayer.loc[mPlayer.active_local_player].client_ping_flag) && (ima_client))
      {
         mPlayer.loc[mPlayer.active_local_player].client_ping_flag = 0;
         client_send_ping();
      }

      // ----------------------------------------------------------
      // do things based on the 1 Hz sec_timer event
      // ----------------------------------------------------------
      if (mwEQ.program_update_1s)
      {
         mwEQ.program_update_1s = 0;
         if (mwPS.program_state == 11) // game loop running
         {
            if (ima_server)
            {
               if (mPlayer.loc[0].server_state_freq_mode == 1) // 0 = manual, 1 = auto
               {
                  int mcp = mwT[4].get_max()*1000;
                  if (mcp > 100) mcp = 100;
                  mPlayer.loc[0].server_state_freq = 1 + mcp/25; // use max_client_ping to set server_state_freq



               }
               for (int p=1; p<NUM_PLAYERS; p++)
                  if (mPlayer.syn[p].control_method == 2)
                  {
                     mPlayer.loc[p].late_cdats_last_sec = mwT_late_cdats_last_sec[p].get_tally();
                     mPlayer.loc[p].game_move_dsync_avg_last_sec = mwT_game_move_dsync_avg_last_sec[p].get_avg();
                  }
            }
         }
      }
   }
}



