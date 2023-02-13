// z_loop.cpp
#include "pm.h"
#include "z_loop.h"
#include "mwQuickGraph.h"
#include "z_sound.h"
#include "z_log.h"
#include "z_settings.h"
#include "z_player.h"
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
#include "z_lift.h"
#include "n_client.h"
#include "n_server.h"
#include "z_bullets.h"
#include "e_visual_level.h"
#include "mwGameMovesArray.h"
#include "mwPMEvent.h"
#include "mwInput.h"
#include "mwEventQueue.h"
#include "mwBitmap.h"
#include "z_menu.h"
#include "mwProgramState.h"
#include "z_item.h"
#include "z_enemy.h"
#include "z_level.h"
#include "e_editor_main.h"
#include "z_control.h"
#include "z_file.h"
#include "z_fnx.h"
#include "z_screen.h"
#include "z_screen_overlay.h"


void draw_frame(void)
{
   mwDS.draw();
//   get_new_background(1);
//   draw_lifts();
//   draw_items();
//   draw_enemies();
//   draw_ebullets();
//   draw_pbullets();
//   draw_players();
//   get_new_screen_buffer(0, 0, 0);
//   draw_screen_overlay();
//   al_flip_display();
}

void move_frame(void)
{
   double t1, t2, t3, t4, t5, t6;
   if ((LOG_TMR_move_tot) || (LOG_TMR_move_all)) t0 = al_get_time();
   move_ebullets();
   if (LOG_TMR_move_all) t1 = al_get_time();
   move_pbullets();
   if (LOG_TMR_move_all) t2 = al_get_time();
   move_lifts(0);
   if (LOG_TMR_move_all) t3 = al_get_time();
   move_players();
   if (LOG_TMR_move_all) t4 = al_get_time();
   move_enemies();
   if (LOG_TMR_move_all) t5 = al_get_time();
   move_items();
   if (LOG_TMR_move_all)
   {
      t6 = al_get_time();
      sprintf(msg, "tmst m-ebul:[%0.4f] m-pbul:[%0.4f] m-lift:[%0.4f] m-plyr:[%0.4f] m-enem:[%0.4f] m-item:[%0.4f] m-totl:[%0.4f]\n",
      (t1-t0)*1000, (t2-t1)*1000, (t3-t2)*1000, (t4-t3)*1000, (t5-t4)*1000, (t6-t5)*1000, (t6-t0)*1000);
      //printf("\n%s\n", msg);
      add_log_entry2(44, 0, msg);
   }
   if (LOG_TMR_move_tot) add_log_TMR(al_get_time() - t0, "move", 0);
}


void loop_frame(int times) // used for fast forwarding after rewind
{
   for (int i=0; i<times; i++)
   {
      mwGMA.proc_game_moves_array();
      if (players[0].level_done_mode) proc_level_done_mode();
      else move_frame();
      mwPS.frame_num++;
   }
}

int have_all_players_acknowledged(void)
{
   int ret = 1; // yes by default
   for (int p=0; p<NUM_PLAYERS; p++)
   {
      if (players[p].active)
      {
         if (mwGMA.has_player_acknowledged(p))
         {
            players[p].level_done_ack = 1;
         }
         else
         {
            players[p].level_done_ack = 0;
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
   if (!resume_allowed) load_level(start_level, 0, 0);
   if (mwPS.top_menu_sel < 3) mwPS.top_menu_sel = 3;
   while (mwPS.top_menu_sel != 1)
   {
      mwPS.top_menu_sel = zmenu(7, mwPS.top_menu_sel, 10);
      if  (mwPS.top_menu_sel == 1)  { mwPS.program_state = 0;                                           return; } // exit
      if  (mwPS.top_menu_sel == 2)  { visual_level_select(); mwPS.top_menu_sel = 3;                             } // visual level select
      if ((mwPS.top_menu_sel == 4) && (resume_allowed)) { mwPS.new_program_state = 13;                  return; } // resume game
      if  (mwPS.top_menu_sel == 3)  { mwPS.new_program_state = 10;  mwPS.top_menu_sel = 4;                   return; } // start new game
      if  (mwPS.top_menu_sel == 5)  { mwPS.new_program_state = 20;                                      return; } // host network game
      if  (mwPS.top_menu_sel == 6)  { mwPS.new_program_state = 24;                                      return; } // join network game
      if  (mwPS.top_menu_sel == 7)  { mwPS.new_program_state = 3;                                       return; } // settings
      if  (mwPS.top_menu_sel == 8)  { play_level = edit_menu(start_level); mwPS.new_program_state = 10; return; } // level editor
      if  (mwPS.top_menu_sel == 9)  { mwPS.new_program_state = 2;  mwPS.older_program_state = 1;             return; } // demo mode
      if  (mwPS.top_menu_sel == 10)                                                                help(""); // help
      if ((mwPS.top_menu_sel > 100) && (mwPS.top_menu_sel < 200)) // right pressed on menu item
      {
         mwPS.top_menu_sel -= 100;
         if (mwPS.top_menu_sel == 2)
         {
            set_start_level(++start_level); // start level increment
            load_level(start_level, 0, 0);
         }
      }
      if ((mwPS.top_menu_sel > 200) && (mwPS.top_menu_sel < 300)) // left pressed on menu item
      {
         mwPS.top_menu_sel -= 200;
         if (mwPS.top_menu_sel == 2)
         {
            set_start_level(--start_level); // start level decrement
            load_level(start_level, 0, 0);
         }
      }
   }
}

void proc_program_state(void)
{
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

            if (autosave_log_on_game_exit) save_log_file();
            if (autosave_game_on_game_exit) blind_save_game_moves(2);

            stop_sound();
            if (mwPS.program_state != 3) stamp();

            mwPS.program_state = mwPS.old_program_state; // go back to the state that called 0,1,2 or 3
         }
      }
      mwPS.new_program_state = 0;
   }
   if (mwPS.program_state == 0) mwPS.main_loop_exit = 1; // quit
   if (mwPS.program_state == 1) game_menu();  // game menu (this blocks)
   if (mwPS.program_state == 2) mwDM.demo_mode();  // demo mode
   if (mwPS.program_state == 3) settings_pages(-1);  // this blocks

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

      for (int p=0; p<NUM_PLAYERS; p++) init_player(p, 1); // full reset
      players[0].active = 1;

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

      if (!load_level(play_level, 0, 0))
      {
         mwPS.new_program_state = 25;
         return;
      }


      for (int p=0; p<NUM_PLAYERS; p++)
         set_player_start_pos(p, 0);     // get starting position for all players, active or not


      mwGMA.initialize();

      mwPS.frame_num = 0;
      reset_states();
      clear_bullets();
      mwBM.initialize();
      mI.initialize();
      mwPME.initialize();

      if (LOG_NET)
      {
         sprintf(msg,"LEVEL %d STARTED", play_level);
         add_log_entry_header(10, 0, msg, 3);
      }

      mwPS.show_player_join_quit_timer = 0;
      start_music(0);
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
      rtextout_centre(mF.bltn, NULL, msg, mwD.SCREEN_W/2, mwD.SCREEN_H/2, 10, stretch, 0, 1);

      al_flip_display();

      if (mwPS.frame_num > 0)
      {
         printf("got initial state for frame:%d\n", mwPS.frame_num);
         int p = active_local_player;

         // set holdoff 200 frames in future so client won't try to drop while syncing
         players1[p].client_last_stdf_rx_frame_num = mwPS.frame_num + 200;

         if (LOG_NET_join) add_log_entry_header(11, p, "Game state updated - starting chase and lock", 1);
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

      play_level = start_level;
      if (!load_level(play_level, 0, 0))
      {
         mwPS.new_program_state = 19;
         return;
      }

      // reset players
      for (int p=0; p<NUM_PLAYERS; p++)
      {
         init_player(p, 1);           // full reset (start modes 1, 2, 3, 9)
         set_player_start_pos(p, 0);  // get starting position for all players, active or not
      }

      players[0].active = 1;
      players[0].control_method = 3;
      strncpy(players1[0].hostname, mwPS.local_hostname, 16);


      mwGMA.initialize();
      mwPS.frame_num = 0;
      reset_states();
      clear_bullets();
      mwBM.initialize();
      mI.initialize();
      mwPME.initialize();

      game_vars_to_state(srv_client_state[0][1]);
      srv_client_state_frame_num[0][1] = mwPS.frame_num;
      if (LOG_NET_stdf)
      {
         sprintf(msg, "stdf saved server state[1]:%d\n", mwPS.frame_num);
         add_log_entry2(27, 0, msg);
      }

      mwGMA.add_game_move(0, 0, 0, play_level);       // [00] game_start

      // save colors in game moves array
      for (int p=0; p<NUM_PLAYERS; p++)
         if (players[p].active) mwGMA.add_game_move(0, 1, p, players[p].color); // 1 - player_state and color

      if (LOG_NET)
      {
         sprintf(msg,"LEVEL %d STARTED", play_level);
         add_log_entry_header(10, 0, msg, 3);
      }

      mwPS.show_player_join_quit_timer = 0;
      start_music(0); // rewind and start theme
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
      play_level = start_level;
      if (!load_level(play_level, 0, 0))
      {
         mwPS.new_program_state = 1;
         return;
      }

      for (int p=0; p<NUM_PLAYERS; p++)
      {
         init_player(p, 1);            // full reset
         set_player_start_pos(p, 0);   // get starting position for all players, active or not
      }
      players[0].active = 1;

      mwGMA.initialize();

      clear_bullets();
      mwBM.initialize();
      mI.initialize();
      mwPME.initialize();

      stimp();
      mwPS.frame_num = 0;
      mwPS.show_player_join_quit_timer = 0;
      start_music(0); // rewind and start theme

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
      stop_sound();

      players[0].level_done_mode = 0;

      if (LOG_NET) { sprintf(msg,"NEXT LEVEL:%d", players[0].level_done_next_level); add_log_entry_header(10, 0, msg, 3); }

      if (players[active_local_player].control_method == 1) // run demo mode saved game file
      {
         mwPS.new_program_state = 2;
         al_rest(1);
         return; // to exit immediately
      }

      if ((LOG_NET) && (ima_client)) log_ending_stats(active_local_player);
      if ((LOG_NET) && (ima_server)) log_ending_stats_server();

      if (ima_server) server_flush();
      if (ima_client) client_flush();

      blind_save_game_moves(1);

      if (autosave_log_on_level_done) save_log_file();

      play_level = players[0].level_done_next_level;

      if (0) // reset clients
      {
         if ((ima_client) || (ima_server))
         {
            for (int p=0; p<NUM_PLAYERS; p++)
            {
               // free all the used clients, so they can be re-assigned on the next level
               // if (players[p].control_method == 9) players[p].control_method = 0;

               // set all clients inactive on server and client, to force them to re-chase and lock on the new level
               // if ((players[p].control_method == 2) || (players[p].control_method == 4)) players[p].active = 0;

            }
         }
      }



      // every mode after this should require load level so why don't I do it here at the top
      if (!load_level(play_level, 0, 0))
      {
         mwPS.new_program_state = 1;
         return;
      }

      // reset players
      for (int p=0; p<NUM_PLAYERS; p++)
      {
         init_player(p, 2);            // next level reset
         set_player_start_pos(p, 0);   // get starting position for all players, active or not
      }
      players[0].active = 1;

      mwGMA.initialize();


      mwPS.frame_num = 0;
      reset_states();
      clear_bullets();
      mwBM.initialize();
      mI.initialize();
      mwPME.initialize();

      if (ima_server) // set server initial state (for both 2-new game and 5-level done when server)
      {
         players[0].control_method = 3;

         game_vars_to_state(srv_client_state[0][1]);
         srv_client_state_frame_num[0][1] = mwPS.frame_num;

         if (LOG_NET_stdf)
         {
            //   printf("saved server state[1]:%d\n\n", mwPS.frame_num);
            sprintf(msg, "stdf saved server state[1]:%d\n", mwPS.frame_num);
            add_log_entry2(27, 0, msg);
         }
      }

      mwGMA.add_game_move(0, 0, 0, play_level);       // [00] game_start

      // save colors in game moves array
      for (int p=0; p<NUM_PLAYERS; p++)
         if (players[p].active) mwGMA.add_game_move(0, 1, p, players[p].color); // [01] player_state and color

      if (LOG_NET)
      {
         sprintf(msg,"LEVEL %d STARTED", play_level);
         add_log_entry_header(10, 0, msg, 3);
      }

      // only do fancy zoom into level if not in netgame  .. also warp if I can figure out how to do that here
      if ((!ima_client) && (!ima_server)) stimp();

      mwPS.show_player_join_quit_timer = 0;
      start_music(0); // rewind and start theme
      mwTS.init_timestamps();
      mwPS.program_state = 11;
   }


   //---------------------------------------
   // resume game
   //---------------------------------------
   if (mwPS.program_state == 13)
   {
      start_music(1); // resume theme
      stimp();
      mwPS.program_state = 11;
   }


   //---------------------------------------
   // run demo
   //---------------------------------------
   if (mwPS.program_state == 14)
   {
      if (!load_level(play_level, 0, 0))
      {
         mwPS.new_program_state = 1;
         return;
      }

      // reset players
      for (int p=0; p<NUM_PLAYERS; p++)
      {
         init_player(p, 1);           // full reset (start modes 1, 2, 3, 9)
         set_player_start_pos(p, 0);  // get starting position for all players, active or not
      }

      players[0].active = 1;
      players[0].control_method = 1; // rungame demo mode

      mwPS.frame_num = 0;
      clear_bullets();
      mwBM.initialize();
      mI.initialize();
      mwPME.initialize();

      mwPS.show_player_join_quit_timer = 0;
      start_music(0); // rewind and start theme
      stimp();
      mwTS.init_timestamps();
      mwPS.program_state = 11;
   }
}


void proc_level_done_mode(void)
{
   if (players[0].level_done_mode == 9) // pause players and set up exit xyincs
   {
      set_player_join_quit_display(players[0].level_done_player, 2, 60);

      for (int p=0; p<NUM_PLAYERS; p++)
         if (players[p].active)
         {
            players[p].paused = 5; // set player paused

            // get distance between player and exit
            al_fixed dx = al_itofix(players[0].level_done_x) - players[p].PX;
            al_fixed dy = al_itofix(players[0].level_done_y) - players[p].PY;

            // get move
            players[p].xinc = al_fixdiv(dx, al_itofix(60));
            players[p].yinc = al_fixdiv(dy, al_itofix(60));

            // set left right direction
            if (players[p].xinc > al_itofix(0)) players[p].left_right = 1;
            if (players[p].xinc < al_itofix(0)) players[p].left_right = 0;
         }
   }
   if (players[0].level_done_mode == 8) // players seek exit
   {
      for (int p=0; p<NUM_PLAYERS; p++)
         if (players[p].active)
         {
            players[p].PX += players[p].xinc;
            players[p].PY += players[p].yinc;
         }
   }
   if (players[0].level_done_mode == 7) // shrink and rotate
   {
      for (int p=0; p<NUM_PLAYERS; p++)
         if (players[p].active)
         {
            players[p].draw_scale -= al_ftofix(0.05);
            players[p].draw_rot -= al_ftofix(8);
         }
   }
   if (players[0].level_done_mode == 5) // skippable 15s timeout
   {
      if (!ima_client)
      {
         if (have_all_players_acknowledged()) players[0].level_done_timer = 0; // skip
      }
   }
   if (--players[0].level_done_timer <= 0) // time to change to next level_done_mode
   {
      players[0].level_done_mode--;
      if (players[0].level_done_mode == 8) players[0].level_done_timer = 60; // players seek exit
      if (players[0].level_done_mode == 7) players[0].level_done_timer = 20; // players shrink and rotate into exit
      if (players[0].level_done_mode == 6) players[0].level_done_timer = 0;
      if (players[0].level_done_mode == 5) players[0].level_done_timer = 600; // skippable 15s delay;
      if (players[0].level_done_mode == 4) players[0].level_done_timer = 0;
      if (players[0].level_done_mode == 3) players[0].level_done_timer = 0;
      if (players[0].level_done_mode == 2) players[0].level_done_timer = 10;  // delay to load next level
      if (players[0].level_done_mode == 1) mwPS.program_state = 12;                // load new level
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

            if (players[0].level_done_mode) proc_level_done_mode();
            else move_frame();

            if (LOG_TMR_sdif) t0 = al_get_time();
            server_create_new_state();
            if (LOG_TMR_sdif) add_log_TMR(al_get_time() - t0, "sdif", 0);

            draw_frame();

            double pt = al_get_time() - mwTS.timestamp_frame_start;
            if (LOG_TMR_cpu) add_log_TMR(pt, "cpu", 0);

            mwRA[0].add_data((pt/0.025)*100);
            mwQG[0].add_data(0, mwRA[0].last_input);
            mwQG[0].add_data(1, mwRA[0].mn);
            mwQG[0].add_data(2, mwRA[0].mx);
            mwQG[0].add_data(3, mwRA[0].avg);

         }
      }


      // send client ping at this time in the loop, if flag is set
      if ((players1[active_local_player].client_ping_flag) && (ima_client))
      {
         players1[active_local_player].client_ping_flag = 0;
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
               if (players1[0].server_state_freq_mode == 1) // 0 = manual, 1 = auto
               {
                  int mcp = mwT[4].get_max()*1000;
                  if (mcp > 100) mcp = 100;
                  players1[0].server_state_freq = 1 + mcp/25; // use max_client_ping to set server_state_freq



               }
               for (int p=1; p<NUM_PLAYERS; p++)
                  if (players[p].control_method == 2)
                  {
                     players1[p].late_cdats_last_sec = mwT_late_cdats_last_sec[p].get_tally();
                     players1[p].game_move_dsync_avg_last_sec = mwT_game_move_dsync_avg_last_sec[p].get_avg();
                  }
            }
         }
      }
   }
}



