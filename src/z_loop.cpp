// z_loop.cpp
#include "pm.h"

int proc_frame_delay(void)
{
   int draw_frame = 0;
   frame_num++;
   update_animation();
   if (al_get_timer_count(sec_timer) > 0)
   {
      al_set_timer_count(sec_timer, 0); // reset_second_timer
      actual_fps = frame_num - last_fps_frame_num;
      last_fps_frame_num = frame_num;
      frames_skipped_last_second = (players1[active_local_player].frames_skipped - last_frames_skipped);
      last_frames_skipped = players1[active_local_player].frames_skipped;
   }

//   printf("fn:%d timer:%d\n", frame_num, al_get_timer_count(fps_timer));

   if (speed_testing) // draw every frame no matter how fast or slow it is
   {
      draw_frame = 1;
      al_set_timer_count(fps_timer, frame_num);
   }
   else
   {
      if (frame_num <=  al_get_timer_count(fps_timer)) // skip drawing frame
      {
         draw_frame = 0;
         players1[active_local_player].frames_skipped++;
      }
      else draw_frame = 1;
      while (frame_num > al_get_timer_count(fps_timer)); // delay if too far ahead so timer catches up
   }
   return draw_frame;
}





// start modes:
// 1 single player new game
// 2 server new game
// 3 client new game
// 5 level done
// 7 resume single player
// 9 run demo mode
void proc_start_mode(int start_mode)
{
   stop_sound();

   if (start_mode == 7) // resume single player game
   {
      start_music(1); // resume theme
      stimp();
      set_frame_nums(frame_num); // set fps_timer count to frame_num
      return;
   }

   if (start_mode == 5) // level done - do clean up from previous level
   {
      if (L_LOGGING_NETPLAY) { sprintf(msg,"NEXT LEVEL:%d", next_level); add_log_entry_header(10, 0, msg, 3); }

      if (players[active_local_player].control_method == 1) // run demo mode saved game file
      {
         al_rest(1);
         game_exit = 1;// run game file play exits after level done
         return; // to exit immediately
      }

      if (ima_server) server_flush();
      if (ima_client) client_flush();

      blind_save_game_moves(1);
      save_log_file();
      play_level = next_level;
      players[0].level_done_mode = 0;

      if (0) // reset clients
      {
         if ((ima_client) || (ima_server))
         {
            for (int p=0; p<NUM_PLAYERS; p++)
            {
               // free all the used clients, so they can be re-assigned on the next level
               if (players[p].control_method == 9) players[p].control_method = 0;

               // set all clients inactive on server and client, to force them to re-chase and lock on the new level
               // if ((players[p].control_method == 2) || (players[p].control_method == 4)) players[p].active = 0;

            }
            // al_rest(1);
         }
      }

   }


   if (start_mode == 3) // client new game (this has to go before load level)
   {
      if (!client_init())
      {
         client_exit();
         game_exit = 1;
         return;
      }
   }

   // every mode after this should require load level so why don't I do it here at the top
   if (!load_level(play_level, 0))
   {
      game_exit = 1;
      return;
   }

   // reset players
   for (int p=0; p<NUM_PLAYERS; p++)
   {
      if (start_mode == 5) init_player(p, 2); // next level reset
      else                 init_player(p, 1); // full reset (start modes 1, 2, 3, 9)
      set_player_start_pos(p, 0);             // get starting position for all players, active or not
   }
   players[0].active = 1;


   if (start_mode == 9) // load and run demo
   {
      if (L_LOGGING_NETPLAY)
      {
         log_versions();
         sprintf(msg, "Demo mode started on level:%d", play_level);
         add_log_entry_header(10, 0, msg, 0);
      }
      players[0].control_method = 1; // rungame demo mode
   }
   else clear_game_moves(); // clear game moves array, except for demo mode


   set_frame_nums(0);
   reset_states();


   if (start_mode == 2) // server new game
   {
      if (!server_init())
      {
         server_exit();
         game_exit = 1;
         return;
      }
   }

   if (ima_server) // set server initial state (for both 2-new game and 5-level done when server)
   {
      players[0].control_method = 3;
      game_vars_to_state(srv_stdf_state[1]);
      srv_stdf_state_frame_num[1] = frame_num;
      if (L_LOGGING_NETPLAY_stdf)
      {
         //   printf("saved server state[1]:%d\n\n", frame_num);
         sprintf(msg, "stdf saved server state[1]:%d\n", frame_num);
         add_log_entry2(27, 0, msg);
      }
   }

   if ((start_mode == 1) || (start_mode == 2) || (start_mode == 5)) // 1 single player new game, 2 server new game, 5 level done
   {
      add_game_move(0, 0, 0, play_level);       // [00] game_start

      // save colors in game moves array
      for (int p=0; p<NUM_PLAYERS; p++)
         if (players[p].active) add_game_move(0, 1, p, players[p].color); // [01] player_state and color
   }


   // only do fancy zoom into level if not in netgame
   if ((!ima_client) && (!ima_server)) stimp();


   if (L_LOGGING_NETPLAY)
   {
      sprintf(msg,"LEVEL %d STARTED", play_level);
      add_log_entry_header(10, 0, msg, 3);
   }


   clear_bmsg();
   clear_bullets();
   clear_keys();
   clear_pm_events();

   show_player_join_quit_timer = 0;
   start_music(0); // rewind and start theme
}

int ami_server_or_single(void)
{
   int a = active_local_player;
   int cm = players[a].control_method;

   if ((cm == 0) || (cm == 3)) return 1;
   return 0;
}

int has_player_acknowledged(int p)
{
   int start_pos = game_move_entry_pos;
   int end_pos = start_pos - 1000;
   if (end_pos < 0) end_pos = 0;
   for (int x=start_pos; x>end_pos; x--) // look back for ack
      if ((game_moves[x][1] == 8) && (game_moves[x][2] == p)) return 1;
   return 0;
}

int have_all_players_acknowledged(void)
{
   for (int p=0; p<NUM_PLAYERS; p++)
      if ((players[p].active) && (!has_player_acknowledged(p))) return 0;
   return 1;
}

void proc_level_done_mode(void)
{
   stop_sound();

   if (!ima_client)
   {
      if (players[0].level_done_mode == 5)
      {
         if (have_all_players_acknowledged()) players[0].level_done_timer = 0; // skip

         for (int p=0; p<NUM_PLAYERS; p++)
            if (players[p].active)
            {
               if (has_player_acknowledged(p)) players[p].level_done_ack = 1;
               else players[p].level_done_ack = 0;
            }
      }
   }
   for (int p=0; p<NUM_PLAYERS; p++) players[p].paused = 5;


   if (players[0].level_done_mode == 9)  // set xinc and yinc for player exit home
   {
      show_player_join_quit_timer = 60;
      show_player_join_quit_player = players[0].level_done_player;
      show_player_join_quit_jq = 2;

      for (int p=0; p<NUM_PLAYERS; p++)
         if (players[p].active)
         {
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

   if (players[0].level_done_mode == 8)
   {
      for (int p=0; p<NUM_PLAYERS; p++)
         if (players[p].active)
         {
            players[p].PX += players[p].xinc;
            players[p].PY += players[p].yinc;
         }
   }
   if (players[0].level_done_mode == 7)
   {
      for (int p=0; p<NUM_PLAYERS; p++)
         if (players[p].active)
         {
            players[p].draw_scale -= al_ftofix(0.05);
            players[p].draw_rot -= al_ftofix(8);
         }
   }
   if (--players[0].level_done_timer <= 0) // time to change to next level_done_mode
   {
      players[0].level_done_mode--;
      if (players[0].level_done_mode == 8) players[0].level_done_timer = 60;  // unskippable 2s delay - seek
      if (players[0].level_done_mode == 7) players[0].level_done_timer = 20; // shrink
      if (players[0].level_done_mode == 6) players[0].level_done_timer = 0;
      if (players[0].level_done_mode == 5) players[0].level_done_timer = 600; // skippable 15s delay;
      if (players[0].level_done_mode == 4) players[0].level_done_timer = 0;
      if (players[0].level_done_mode == 3) players[0].level_done_timer = 0;
      if (players[0].level_done_mode == 2) players[0].level_done_timer = 10;  // delay to load next level
      if (players[0].level_done_mode == 1) proc_start_mode(5);
   }
}


void draw_frame(void)
{
   get_new_background(1);
   draw_lifts();
   draw_items();
   draw_enemies();
   draw_ebullets();
   draw_pbullets();
   draw_players();
   get_new_screen_buffer(0, 0, 0);
   draw_screen_overlay();
   al_flip_display();
}

void move_frame(void)
{
//   if (L_LOGGING_NETPLAY_move_frame)
   {
      //printf("move frame:%d-------------\n", frame_num);
      sprintf(msg, "move frame:%d\n", frame_num);
      add_log_entry2(41, 0, msg);
   }

   move_ebullets();
   move_pbullets();
   move_lifts(0);
   move_players();
   move_enemies();
   move_items();
}

void game_loop(int start_mode)
{
   game_exit = 0;
   proc_start_mode(start_mode);
   while (!game_exit)
   {
      proc_scale_factor_change();
      proc_sound();

      if (ima_server) server_control();
      if (ima_client) client_control();

      proc_controllers();

      if ((ima_server) && (!server_send_stdf() ))
      {
         if (players[0].level_done_mode) proc_level_done_mode();
         else move_frame();
      }

      if (proc_frame_delay()) draw_frame();
   }
   if (ima_server) server_exit();
   if (ima_client) client_exit();
   stop_sound();
   stamp();
}



// used for fast forwarding after rewind
void loop_frame(int times)
{
   for (int i=0; i<times; i++)
   {
      proc_game_moves_array();
      if (players[0].level_done_mode) proc_level_done_mode();
      else move_frame();
      frame_num++;
   }
}
