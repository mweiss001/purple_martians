// zloop.cpp

#include "pm.h"


void proc_frame_delay(void)
{
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
   if (players[p].control_method == 1)
   {
      show_level_done(0);
      al_rest(1);
      game_exit = 1;// run game file play exits after level done
      return;
   }
   if ((ima_server) || (ima_client))
   {
      show_level_done(0);
      for (int p=0; p<NUM_PLAYERS; p++)
      {
         // free all the used clients, so they can be re-assigned on the next level
         if (players[p].control_method == 9) players[p].control_method = 0;
         // set all clients inactive on server and client, to force them to re-chase and lock on the new level
         if ((players[p].control_method == 2) || (players[p].control_method == 4)) players[p].active = 0;
      }
      if (ima_server) server_flush();
      if (ima_client) client_flush();
      al_rest(1);
   }
   blind_save_game_moves(1);
   save_log_file();
   play_level = next_level;
   level_done_trig = 0;
   level_done_proc = 1;
   if ((!ima_client) && (!ima_server)) stamp();
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
   if (start_mode == 7) // resume single player game
   {
      start_music(1); // resume theme
      stimp();
      set_frame_nums(frame_num); // set fps_timer count to frame_num
      return;
   }

   if (start_mode == 5) // start new level after level done
   {
      for (int p=0; p<NUM_PLAYERS; p++) init_player(p, 2);
   }
   else // 1, 2, 3, 9 - full player data reset
   {
      for (int p=0; p<NUM_PLAYERS; p++) init_player(p, 1);
      players[0].active = 1;
   }

   // clear game moves array, except demo
   if  (start_mode != 9) clear_game_moves();

   if (start_mode == 9) players[0].control_method = 1;

   if (start_mode == 2) // server
   {
      if (!server_init())
      {
         server_exit();
         game_exit = 1;
         return;
      }
   }

   if (start_mode == 3) // client
   {
      if (!client_init())
      {
         client_exit();
         game_exit = 1;
         return;
      }
   }

   if ( (start_mode == 1) || (start_mode == 2) || (start_mode == 5))
   {
      add_game_move(0, 0, play_level, 0);       // [00] game_start
      add_game_move(0, 1, 0, players[0].color); // [01] player_state and color
   }

	if (!load_level(play_level, 0))
	{
		game_exit = 1;
      return;
	}

   if ((ima_client) || (ima_server))
   {
      sprintf(msg,"LEVEL %d STARTED", play_level);
      if (L_LOGGING_NETPLAY) add_log_entry_header(10, 0, msg, 3);
   }
   else stimp();

   set_frame_nums(0);
   clear_bmsg();
   clear_bullets();
   clear_keys();
   show_player_join_quit_timer = 0;
   level_done_trig = 0;
   level_done_proc = 0;
   start_music(0); // rewind and start theme

}

void game_loop(int start_mode)
{
   game_exit = 0;
   proc_start_mode(start_mode);
   while (!game_exit)
   {
      //printf("t1 ld:%d\n", level_done);

      if (level_done_proc) proc_start_mode(5);

      proc_scale_factor_change();

      proc_sound();

      if (ima_server) server_control();
      if (ima_client) client_control();
      proc_controllers();

      move_ebullets();
      move_pbullets();
      move_lifts(0);
      move_players();
      move_enemies();
      move_items();
      proc_frame_delay();
      if (draw_frame)
      {
         get_new_background(0);

         draw_lifts();
         draw_items();
         draw_enemies();
         draw_ebullets();
         draw_pbullets();
         draw_players();

         get_new_screen_buffer();
         draw_screen_overlay();
         al_flip_display();
      }
   }
   if (ima_server) server_exit();
   if (ima_client) client_exit();
   stop_sound();
   stamp();
}
