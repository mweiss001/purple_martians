// z_main_loop.cpp

#include "pm.h"


void proc_events(ALLEGRO_EVENT ev)
{
   if (ev.type == ALLEGRO_EVENT_DISPLAY_RESIZE) proc_display_change();
   if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) fast_exit(0);
   if (ev.type == ALLEGRO_EVENT_MOUSE_WARPED)
   {
      mouse_x = ev.mouse.x / display_transform_double;
      mouse_y = ev.mouse.y / display_transform_double;
   }
   if (ev.type == ALLEGRO_EVENT_MOUSE_AXES)
   {
      mouse_x = ev.mouse.x / display_transform_double;
      mouse_y = ev.mouse.y / display_transform_double;
      mouse_z = ev.mouse.z / display_transform_double;
      mouse_dx = ev.mouse.dx;
      mouse_dy = ev.mouse.dy;
      mouse_dz = ev.mouse.dz;
   }
   if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
   {
      if (ev.mouse.button == 1) mouse_b[1][0] = true;
      if (ev.mouse.button == 2) mouse_b[2][0] = true;
      if (ev.mouse.button == 3) mouse_b[3][0] = true;
      if (ev.mouse.button == 4) mouse_b[4][0] = true;
   }
   if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
   {
      if (ev.mouse.button == 1) mouse_b[1][0] = false;
      if (ev.mouse.button == 2) mouse_b[2][0] = false;
      if (ev.mouse.button == 3) mouse_b[3][0] = false;
      if (ev.mouse.button == 4) mouse_b[4][0] = false;
   }
   if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
   {
      int k = ev.keyboard.keycode;
      key[k][0] = true;
   }
   if (ev.type == ALLEGRO_EVENT_KEY_UP)
   {
      int k = ev.keyboard.keycode;
      key[k][0] = false;
      if (k == ALLEGRO_KEY_PRINTSCREEN) key[k][0] = true; // special exception to make PRINTSCREEN work
   }
   if (ev.type == ALLEGRO_EVENT_KEY_CHAR)
   {
      key_pressed_ASCII = ev.keyboard.unichar;
      serial_key_check(key_pressed_ASCII);
      //printf("key_pressed_ASCII:%d\n", key_pressed_ASCII);
   }
   if (ev.type == ALLEGRO_EVENT_JOYSTICK_AXIS)
   {
      int jy = getJoystickNum(ev.joystick.id);
      int jo = 0; // offset
      if (jy == 0) jo = 0;
      if (jy == 1) jo = 20;
      int ax = ev.joystick.axis;
      float pos = ev.joystick.pos;
      if (ax == 0) // x axis
      {
         key[130+jo][0] = false;
         key[131+jo][0] = false;
         if (pos > 0) key[131+jo][0] = true;
         if (pos < 0) key[130+jo][0] = true;
      }
      if (ax == 1) // y axis
      {
         key[128+jo][0] = false;
         key[129+jo][0] = false;
         if (pos > 0) key[129+jo][0] = true;
         if (pos < 0) key[128+jo][0] = true;
      }
   }
   if (ev.type == ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN)
   {
      int jy = getJoystickNum(ev.joystick.id);
      int sc = get_scan_code_from_joystick(jy, 1, ev.joystick.button);
      key[sc][0] = true;
   }
   if (ev.type == ALLEGRO_EVENT_JOYSTICK_BUTTON_UP)
   {
      int jy = getJoystickNum(ev.joystick.id);
      int sc = get_scan_code_from_joystick(jy, 1, ev.joystick.button);
      key[sc][0] = false;
   }
   if (ev.type == ALLEGRO_EVENT_TIMER)
   {
      if ((program_state == 11) && (ev.timer.source == fps_timer)) program_update = 1;
      if ((program_state == 11) && (ev.timer.source == sec_timer)) program_update_1s = 1;
   }
}

void proc_keys_held(void)
{
   for (int k = ALLEGRO_KEY_A; k < ALLEGRO_KEY_MAX; k++)
   {
      if ((key[k][0] == true) && (key[k][1] == false)) key[k][2] = true; // just pressed
      else key[k][2] = false;
      if ((key[k][0] == false) && (key[k][1] == true)) key[k][3] = true; // just released
      else key[k][3] = false;
      key[k][1] = key[k][0]; // previous for next time
   }
   for (int m=1; m<5; m++)
   {
      if ((mouse_b[m][0] == true) && (mouse_b[m][1] == false)) mouse_b[m][2] = true; // just pressed
      else mouse_b[m][2] = false;
      if ((mouse_b[m][0] == false) && (mouse_b[m][1] == true)) mouse_b[m][3] = true; // just released
      else mouse_b[m][3] = false;
      mouse_b[m][1] = mouse_b[m][0]; // previous for next time
   }
}

void proc_event_queue(void)
{
   key[ALLEGRO_KEY_PRINTSCREEN][0] = 0; // hack to make PRINTSCREEN key work properly
   key_pressed_ASCII = 0;

   while (!al_is_event_queue_empty(event_queue))
   {
      ALLEGRO_EVENT ev;
      al_get_next_event(event_queue, &ev);
      proc_events(ev);
   }
   proc_keys_held();
   function_key_check();
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

void move_frame(int t)
{
//   if (L_LOGGING_NETPLAY_move_frame)
//   {
//      //printf("move frame:%d-------------\n", frame_num);
//      sprintf(msg, "move[%d] frame:%d\n", t, frame_num);
//      add_log_entry2(41, 0, msg);
//   }

   move_ebullets();
   move_pbullets();
   move_lifts(0);
   move_players();
   move_enemies();
   move_items();
}

// used for fast forwarding after rewind
void loop_frame(int times)
{
   for (int i=0; i<times; i++)
   {
      proc_game_moves_array();
      if (players[0].level_done_mode) proc_level_done_mode();
      else move_frame(1);
      frame_num++;
   }
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

   if (players[0].level_done_mode == 9)  // set xinc and yinc for player exit home
   {
      show_player_join_quit_timer = 60;
      show_player_join_quit_player = players[0].level_done_player;
      show_player_join_quit_jq = 2;

      for (int p=0; p<NUM_PLAYERS; p++)
         if (players[p].active)
         {
            players[p].paused = 5; // also set all players paused

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
      if (players[0].level_done_mode == 8) players[0].level_done_timer = 60;  // seek
      if (players[0].level_done_mode == 7) players[0].level_done_timer = 20;  // shrink
      if (players[0].level_done_mode == 6) players[0].level_done_timer = 0;
      if (players[0].level_done_mode == 5) players[0].level_done_timer = 600; // skippable 15s delay;
      if (players[0].level_done_mode == 4) players[0].level_done_timer = 0;
      if (players[0].level_done_mode == 3) players[0].level_done_timer = 0;
      if (players[0].level_done_mode == 2) players[0].level_done_timer = 10;  // delay to load next level
      if (players[0].level_done_mode == 1) program_state = 12;
   }
}


void proc_program_state(void)
{
   // ----------------------------------------------------------
   // handle all the changes from one state to another
   // ----------------------------------------------------------
   if (new_program_state)
   {
      if (new_program_state != program_state)
      {
         if (new_program_state == 10) program_state = 10; // start new game
         if (new_program_state == 12) program_state = 12; // level_done
         if (new_program_state == 13) program_state = 13; // resume
         if (new_program_state == 14) program_state = 14; // run demo

         if (new_program_state == 2) program_state = 2;   // demo_mode


         if (new_program_state == 20) program_state = 20; // start server game
         if (new_program_state == 21) program_state = 21; // start client game
         if (new_program_state == 1) // game menu or fast exit
         {
            stop_sound();
            stamp();
            program_state = old_program_state; // go back to the state that called 0,1 or 2
         }
      }
      new_program_state = 0;
   }

//   if (program_state == 0) fast_exit(0); // instantly quit

   if (program_state == 0) main_loop_exit = 1; // quit
   if (program_state == 1) game_menu();  // game menu (this blocks)
   if (program_state == 2) demo_mode();  // demo mode


   //---------------------------------------
   // single player new game
   //---------------------------------------
   if (program_state == 10)
   {

      play_level = start_level;

      if (!load_level(play_level, 0))
      {
         new_program_state = 1;
         return;
      }

      // reset players
      for (int p=0; p<NUM_PLAYERS; p++)
      {
         init_player(p, 1); // full reset (start modes 1, 2, 3, 9)
         set_player_start_pos(p, 0);             // get starting position for all players, active or not
      }
      players[0].active = 1;

      clear_game_moves(); // clear game moves array, except for demo mode

      set_frame_nums(0);
      reset_states();
      clear_bullets();
      clear_bmsg();
      clear_keys();
      clear_pm_events();

      stimp();
      show_player_join_quit_timer = 0;
      start_music(0); // rewind and start theme

      program_state = 11;
   }


   //---------------------------------------
   // level done
   //---------------------------------------
   if (program_state == 12)
   {
      stop_sound();


      if (L_LOGGING_NETPLAY) { sprintf(msg,"NEXT LEVEL:%d", next_level); add_log_entry_header(10, 0, msg, 3); }

      if (players[active_local_player].control_method == 1) // run demo mode saved game file
      {
         new_program_state = 2;
         al_rest(1);
         return; // to exit immediately
      }

      if (ima_client) log_ending_stats(active_local_player);
      if (ima_server) log_ending_stats_server();

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
         init_player(p, 2);            // next level reset
         set_player_start_pos(p, 0);   // get starting position for all players, active or not
      }
      players[0].active = 1;


      clear_game_moves(); // clear game moves array, except for demo mode


      set_frame_nums(0);
      reset_states();
      clear_bullets();
      clear_bmsg();
      clear_keys();
      clear_pm_events();

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

      add_game_move(0, 0, 0, play_level);       // [00] game_start

      // save colors in game moves array
      for (int p=0; p<NUM_PLAYERS; p++)
         if (players[p].active) add_game_move(0, 1, p, players[p].color); // [01] player_state and color

      if (L_LOGGING_NETPLAY)
      {
         sprintf(msg,"LEVEL %d STARTED", play_level);
         add_log_entry_header(10, 0, msg, 3);
      }

      // only do fancy zoom into level if not in netgame
      if ((!ima_client) && (!ima_server)) stimp();

      show_player_join_quit_timer = 0;
      start_music(0); // rewind and start theme
      program_state = 11;
   }


   //---------------------------------------
   // resume game
   //---------------------------------------
   if (program_state == 13)
   {
      set_frame_nums(frame_num); // set fps_timer count to frame_num
      start_music(1); // resume theme
      stimp();
      program_state = 11;
   }


   //---------------------------------------
   // run demo
   //---------------------------------------
   if (program_state == 14)
   {
      if (!load_level(play_level, 0))
      {
         new_program_state = 1;
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

      set_frame_nums(0);
      reset_states();
      clear_bullets();
      clear_bmsg();
      clear_keys();
      clear_pm_events();

      show_player_join_quit_timer = 0;
      start_music(0); // rewind and start theme
      stimp();
      program_state = 11;
   }


   //---------------------------------------
   // server new game
   //---------------------------------------
   if (program_state == 20)
   {
      if (!load_level(play_level, 0))
      {
         new_program_state = 1;
         return;
      }

      // reset players
      for (int p=0; p<NUM_PLAYERS; p++)
      {
         init_player(p, 1);           // full reset (start modes 1, 2, 3, 9)
         set_player_start_pos(p, 0);  // get starting position for all players, active or not
      }
      players[0].active = 1;

      clear_game_moves(); // clear game moves array, except for demo mode


      set_frame_nums(0);
      reset_states();
      clear_bullets();
      clear_bmsg();
      clear_keys();
      clear_pm_events();


      if (!server_init())
      {
         server_exit();
         new_program_state = 1;
         return;
      }

      players[0].control_method = 3;
      game_vars_to_state(srv_stdf_state[1]);
      srv_stdf_state_frame_num[1] = frame_num;
      if (L_LOGGING_NETPLAY_stdf)
      {
         //   printf("saved server state[1]:%d\n\n", frame_num);
         sprintf(msg, "stdf saved server state[1]:%d\n", frame_num);
         add_log_entry2(27, 0, msg);
      }

      add_game_move(0, 0, 0, play_level);       // [00] game_start

      // save colors in game moves array
      for (int p=0; p<NUM_PLAYERS; p++)
         if (players[p].active) add_game_move(0, 1, p, players[p].color); // [01] player_state and color

      if (L_LOGGING_NETPLAY)
      {
         sprintf(msg,"LEVEL %d STARTED", play_level);
         add_log_entry_header(10, 0, msg, 3);
      }

      show_player_join_quit_timer = 0;
      start_music(0); // rewind and start theme
      program_state = 11;
   }

   //---------------------------------------
   // client new game
   //---------------------------------------
   if (program_state == 21)
   {
      if (!client_init())
      {
         client_exit();
         new_program_state = 1;
         return;
      }

      if (!load_level(play_level, 0))
      {
         new_program_state = 1;
         return;
      }

      // reset players
      for (int p=0; p<NUM_PLAYERS; p++)
      {
         init_player(p, 1);              // full reset (start modes 1, 2, 3, 9)
         set_player_start_pos(p, 0);     // get starting position for all players, active or not
      }
      players[0].active = 1;


      clear_game_moves(); // clear game moves array, except for demo mode


      set_frame_nums(0);
      reset_states();
      clear_bullets();
      clear_bmsg();
      clear_keys();
      clear_pm_events();


      if (L_LOGGING_NETPLAY)
      {
         sprintf(msg,"LEVEL %d STARTED", play_level);
         add_log_entry_header(10, 0, msg, 3);
      }


      show_player_join_quit_timer = 0;
      start_music(0); // rewind and start theme
      program_state = 11;
   }
}



// ----------------------------------------------------------
// Is the timer too far ahead of current frame?
// Test this before call move_frame()
// ----------------------------------------------------------
void proc_timer_adjust(void)
{
   if (al_get_timer_count(fps_timer) > frame_num+1)
   {
      players1[active_local_player].timer_adjust++;
      players1[active_local_player].timer_adjust_last_sec_tally++;
      al_set_timer_count(fps_timer, frame_num+1);
      //printf("Adjusted timer - fn:%d timer:%d\n", frame_num, al_get_timer_count(fps_timer));
   }
}

int proc_frame_skip(void)
{
   frame_num++;
   update_animation();

   if (frame_num < al_get_timer_count(fps_timer)) // skip drawing frame
   {
      players1[active_local_player].frames_skipped++;
      players1[active_local_player].frames_skipped_last_sec_tally++;
      //printf("Skipped drawing frame:%d\n", frame_num);
      return 0;
   }
   return 1;
}


void main_loop(void)
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
      proc_event_queue();


      // ----------------------------------------------------------
      // do things based on the 40 Hz fps_timer event
      // ----------------------------------------------------------
      if (program_update)
      {
         program_update = 0;

         if (program_state == 11) // game loop running
         {
            proc_timer_adjust();

            proc_scale_factor_change();

            if (ima_server) server_control();
            if (ima_client) client_control();

            proc_player_input();
            proc_game_moves_array();
            if (players[0].level_done_mode) proc_level_done_mode();
            else move_frame(0);

            if (players1[0].server_send_dif) server_send_stdf();

            if (proc_frame_skip()) draw_frame();

            // speed test... draw every frame
//            proc_frame_skip();
//            draw_frame();


            // speed test...draw no frames...maybe 1 i 1000
//            proc_frame_skip();
//            if (frame_num)
//            {
//               if ((frame_num % 10000)==0) draw_frame();
//            }




         }
      }



      // ----------------------------------------------------------
      // do things based on the 1 Hz sec_timer event
      // ----------------------------------------------------------
      if (program_update_1s)
      {
         program_update_1s = 0;
         if (program_state == 11) // game loop running
         {
            players1[active_local_player].frames_skipped_last_sec = players1[active_local_player].frames_skipped_last_sec_tally;
            players1[active_local_player].frames_skipped_last_sec_tally = 0;
            players1[active_local_player].timer_adjust_last_sec = players1[active_local_player].timer_adjust_last_sec_tally;
            players1[active_local_player].timer_adjust_last_sec_tally = 0;
            actual_fps = frame_num - last_fps_frame_num;
            last_fps_frame_num = frame_num;
         }
      }
   }
}


















