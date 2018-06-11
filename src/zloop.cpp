// zloop.cpp

#include "pm.h"


void proc_frame_delay(void)
{
   frame_num++;
   if (al_get_timer_count(sec_timer) > 0)
   {
      al_set_timer_count(sec_timer, 0); // reset_second_timer
      actual_fps = frame_num - last_fps_frame_num;
      last_fps_frame_num = frame_num;
      frames_skipped_last_second = (players1[active_local_player].frames_skipped - last_frames_skipped);
      last_frames_skipped = players1[active_local_player].frames_skipped;
   }
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
}


void proc_level_done(void)
{
   stop_sound();
   int p = active_local_player;

   if (players[p].control_method == 0) show_level_done(1); // wait for keypress in single player mode
   else
   {
      show_level_done(0);
      al_rest(1);
   }

   #ifdef NETPLAY
   if (ima_server) server_flush();
   if (ima_client) client_flush();
   if ((ima_server) || (ima_client))
      for (int p=0; p<NUM_PLAYERS; p++)
      {
         // free all the used clients, so they can be re-assigned on the next level
         if (players[p].control_method == 9) players[p].control_method = 0;
         // set all clients inactive on server and client, to force them to re-chase and lock on the new level
         if ((players[p].control_method == 2) || (players[p].control_method == 4)) players[p].active = 0;
   }
   #endif

   if (players[p].control_method == 1) // run game file play
   {
      game_exit = 1;
   }
   else // all modes except run game file play
   {
      blind_save_game_moves(1);
      save_log_file();
      play_level = next_level;
      start_mode = 1;
      stamp();
   }
}


// start modes:
// 0 = ignore and run game
// 1 = load level and run
// 2 = load level and run (demo from file)

void proc_start_mode(void)
{
	if (!load_level(play_level,0))
	{
		game_exit = 1;
		resume_allowed = 0;
	}


   if (start_mode == 1) // skip this for run demo game
   {
      // reset game_move array
      for (int x=0; x<1000000; x++)
         for (int y=0; y<4; y++)
            game_moves[x][y] = 0;
      game_move_entry_pos = 0;

      // add initial level info to game_move array (unless client)
      if (!ima_client) // server or local
      {
         add_game_move(0, 0, play_level, 0);       // [00] game_start
         add_game_move(0, 1, 0, players[0].color); // [01] player_state and color
      }
      if (L_LOGGING)
      {
         sprintf(msg,"LEVEL %d STARTED", play_level);
         add_log_entry_header(10, 0, msg, 3);
         log_player_array();
      }

      // reset player data
      for (int p=0; p<NUM_PLAYERS; p++) init_player(p, 2);

   }


   clear_bullets();
   clear_keys();

   show_player_join_quit_timer = 0;
   level_done = 0;
   bottom_msg = 0;
   start_mode = 0;

   // reset frame_num and fps_timer count
	frame_num = 0;
   al_set_timer_count(fps_timer, frame_num);

   // reset sound counters
   for (int c=0; c<8; c++) sample_delay[c] = frame_num;

   start_music(0); // rewind and start theme
   stimp();
}

void pm_main(void) // the famous game loop!
{
   if (!start_mode) // resume
   {
      stimp();
      al_set_timer_count(fps_timer, frame_num); // set fps_timer count to frame_num
      start_music(1); // resume theme
   }
   while (!game_exit) // game loop
   {
      if (start_mode) proc_start_mode();

      proc_scale_factor_change();
      move_lifts(0);
      proc_item_collision();
      proc_item_move();
      proc_lit_bomb();
      proc_lit_rocket();

      #ifdef NETPLAY
      if (ima_server) server_control();
      if (ima_client) client_control();
      #endif

      proc_controllers();
      player_move();
      proc_player_carry();
      update_animation();
      enemy_move();
      enemy_collision();
      proc_ebullets();
      proc_pbullets();
      proc_player_health();
      proc_sound();

      proc_frame_delay();

      if (draw_frame)
      {
         // these all draw on level_buffer
         get_new_background(0);
         draw_lifts();
         draw_items();
         draw_enemy();
         draw_ebullets();
         draw_pbullets();
         draw_players();

         get_new_screen_buffer();
         draw_screen_overlay();
         al_flip_display();
      }
   }
   stop_sound();
   stamp();
}
