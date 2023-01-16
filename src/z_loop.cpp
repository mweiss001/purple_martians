// z_loop.cpp
#include "pm.h"

#include "z_qGraph.h"


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
      if (ev.timer.source == fps_timer) program_update = 1;
      if (ev.timer.source == sec_timer) program_update_1s = 1;
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
   double t1, t2, t3, t4, t5, t6, t7, t8, t9, t10;
   if ((LOG_TMR_draw_tot) || (LOG_TMR_draw_all)) t0 = al_get_time();
   get_new_background(1);
   if (LOG_TMR_draw_all) t1 = al_get_time();
   draw_lifts();
   if (LOG_TMR_draw_all) t2 = al_get_time();
   draw_items();
   if (LOG_TMR_draw_all) t3 = al_get_time();
   draw_enemies();
   if (LOG_TMR_draw_all) t4 = al_get_time();
   draw_ebullets();
   if (LOG_TMR_draw_all) t5 = al_get_time();
   draw_pbullets();
   if (LOG_TMR_draw_all) t6 = al_get_time();
   draw_players();
   if (LOG_TMR_draw_all) t7 = al_get_time();
   get_new_screen_buffer(0, 0, 0);
   if (LOG_TMR_draw_all) t8 = al_get_time();
   draw_screen_overlay();
   if (LOG_TMR_draw_all) t9 = al_get_time();
   al_flip_display();
   if (LOG_TMR_draw_all)
   {
      t10 = al_get_time();
      sprintf(msg, "tmst bkgr:[%0.4f] lift:[%0.4f] item:[%0.4f] enem:[%0.4f] ebul:[%0.4f] pbul:[%0.4f] plyr:[%0.4f] buff:[%0.4f] ovrl:[%0.4f] flip:[%0.4f] totl:[%0.4f]\n",
      (t1-t0)*1000, (t2-t1)*1000, (t3-t2)*1000, (t4-t3)*1000, (t5-t4)*1000, (t6-t5)*1000, (t7-t6)*1000, (t8-t7)*1000, (t9-t8)*1000, (t10-t9)*1000, (t10-t0)*1000);
      //printf("\n%s\n", msg);
      add_log_entry2(44, 0, msg);
   }
   if (LOG_TMR_draw_tot) add_log_TMR(al_get_time() - t0, "draw", 0);
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
      sprintf(msg, "tmst ebul:[%0.4f] pbul:[%0.4f] lift:[%0.4f] plyr:[%0.4f] enem:[%0.4f] item:[%0.4f] totl:[%0.4f]\n",
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
      proc_game_moves_array();
      if (players[0].level_done_mode) process_level_done_mode();
      else move_frame();
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



void game_menu(void)
{
   old_program_state = 1;
   if (!splash_screen_done) { splash_screen(); splash_screen_done = 1; }
   if (!resume_allowed) load_level(start_level, 0);
   if (top_menu_sel < 3) top_menu_sel = 3;
   while (top_menu_sel != 1)
   {
      top_menu_sel = zmenu(7, top_menu_sel, 10);
      if  (top_menu_sel == 1)  { program_state = 0;                                           return; } // exit
      if  (top_menu_sel == 2)  { visual_level_select(); top_menu_sel = 3;                             } // visual level select
      if ((top_menu_sel == 4) && (resume_allowed)) { new_program_state = 13;                  return; } // resume game
      if  (top_menu_sel == 3)  { new_program_state = 10;  top_menu_sel = 4;                   return; } // start new game
      if  (top_menu_sel == 5)  { new_program_state = 20;                                      return; } // host network game
      if  (top_menu_sel == 6)  { new_program_state = 24;                                      return; } // join network game
      if  (top_menu_sel == 7)  { new_program_state = 3;                                       return; } // settings
      if  (top_menu_sel == 8)  { play_level = edit_menu(start_level); new_program_state = 10; return; } // level editor
      if  (top_menu_sel == 9)  { new_program_state = 2;  older_program_state = 1;             return; } // demo mode
      if  (top_menu_sel == 10)                                                                help(""); // help

      if ((top_menu_sel > 100) && (top_menu_sel < 200)) // left pressed on menu item
      {
         top_menu_sel -= 100;
         if (top_menu_sel == 2)
         {
            set_start_level(--start_level); // start level decrement
            load_level(start_level, 0);
         }
      }
      if ((top_menu_sel > 200) && (top_menu_sel < 300)) // right pressed on menu item
      {
         top_menu_sel -= 200;
         if (top_menu_sel == 2)
         {
            set_start_level(++start_level); // start level increment
            load_level(start_level, 0);
         }
      }
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
         if (new_program_state ==  2) program_state =  2; // demo_mode
         if (new_program_state ==  3) program_state =  3; // config
         if (new_program_state == 10) program_state = 10; // start new game
         if (new_program_state == 12) program_state = 12; // level_done
         if (new_program_state == 13) program_state = 13; // resume
         if (new_program_state == 14) program_state = 14; // run demo
         if (new_program_state == 20) program_state = 20; // start server game
         if (new_program_state == 21) program_state = 21; // client wait for initial state
         if (new_program_state == 22) program_state = 22; // client load level and set up
         if (new_program_state == 23) program_state = 23; // client wait for sjon
         if (new_program_state == 24) program_state = 24; // client init network and send cjon
         if (new_program_state == 25) program_state = 25; // client exit and clean up network

         if (new_program_state == 1) // game menu or fast exit
         {
            if (ima_server) server_exit();
            if (ima_client) client_exit();

            if (autosave_log_on_game_exit) save_log_file();
            if (autosave_game_on_game_exit) blind_save_game_moves(2);

            stop_sound();
            if (program_state != 3) stamp();

            program_state = old_program_state; // go back to the state that called 0,1,2 or 3
         }
      }
      new_program_state = 0;
   }
   if (program_state == 0) main_loop_exit = 1; // quit
   if (program_state == 1) game_menu();  // game menu (this blocks)
   if (program_state == 2) demo_mode();  // demo mode
   if (program_state == 3) settings_pages(-1);  // this blocks

   //---------------------------------------
   // 21 - client exit
   //---------------------------------------
   if (program_state == 25) // client exit
   {
      client_exit();
      new_program_state = 1;
   }


   //---------------------------------------
   // 24 - client new game
   //---------------------------------------
   if (program_state == 24)
   {
      if (!client_init())
      {
         new_program_state = 25;
         return;
      }

      // set up qGraph here
      // set up qGraph here
      qG[0].initialize(1);
      qG[1].initialize(2);
      qG[2].initialize(2);

      new_program_state = 23;
   }

   //---------------------------------------
   // 23 - client wait for join
   //---------------------------------------
   if (program_state == 23)
   {
      client_fast_packet_loop();
      client_read_packet_buffer();
   }

   //---------------------------------------
   // 22 - client level setup
   //---------------------------------------
   if (program_state == 22)
   {
      printf("client set up level\n");

      if (!load_level(play_level, 0))
      {
         new_program_state = 25;
         return;
      }


      for (int p=0; p<NUM_PLAYERS; p++)
      {
         init_player(p, 1);              // full reset
         set_player_start_pos(p, 0);     // get starting position for all players, active or not
      }
      players[0].active = 1;

      clear_game_moves();

      set_frame_nums(0);
      reset_states();
      clear_bullets();
      clear_bmsg();
      clear_keys();
      clear_pm_events();

      if (LOG_NET)
      {
         sprintf(msg,"LEVEL %d STARTED", play_level);
         add_log_entry_header(10, 0, msg, 3);
      }

      show_player_join_quit_timer = 0;
      start_music(0);
      init_timestamps();
      new_program_state = 21;
   }

   //---------------------------------------
   // 21 - client wait for intial state
   //---------------------------------------
   if (program_state == 21)
   {
      client_fast_packet_loop();
      client_read_packet_buffer();
      client_apply_diff();

      if (key[ALLEGRO_KEY_ESCAPE][3]) program_state = 25;

      sprintf(msg, "Waiting for game state from server");
      float stretch = ( (float)SCREEN_W / ((strlen(msg)+2)*8));
      rtextout_centre(NULL, msg, SCREEN_W/2, SCREEN_H/2, 10, stretch, 0, 1);

      al_flip_display();

      if (frame_num > 0)
      {
         printf("got initial state for frame:%d\n", frame_num);
         int p = active_local_player;

         // set holdoff 200 frames in future so client won't try to drop while syncing
         players1[p].client_last_stdf_rx_frame_num = frame_num + 200;

         if (LOG_NET_join) add_log_entry_header(11, p, "Game state updated - starting chase and lock", 1);
         program_state = 11;
      }
   }


   //---------------------------------------
   // 10 - single player new game
   //---------------------------------------
   if (program_state == 10)
   {
      play_level = start_level;
      if (!load_level(play_level, 0))
      {
         new_program_state = 1;
         return;
      }

      for (int p=0; p<NUM_PLAYERS; p++)
      {
         init_player(p, 1);            // full reset
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

      stimp();
      show_player_join_quit_timer = 0;
      start_music(0); // rewind and start theme

      init_timestamps();
      program_state = 11;

      // set up qGraph here
      qG[0].initialize(1);
      qG[1].initialize(2);


   }


   //---------------------------------------
   // 12 - level done
   //---------------------------------------
   if (program_state == 12)
   {
      stop_sound();

      players[0].level_done_mode = 0;

      if (LOG_NET) { sprintf(msg,"NEXT LEVEL:%d", next_level); add_log_entry_header(10, 0, msg, 3); }

      if (players[active_local_player].control_method == 1) // run demo mode saved game file
      {
         new_program_state = 2;
         al_rest(1);
         return; // to exit immediately
      }

   //   if (ima_client) log_ending_stats(active_local_player);
 //     if (ima_server) log_ending_stats_server();

      if (ima_server) server_flush();
      if (ima_client) client_flush();

      blind_save_game_moves(1);

      if (autosave_log_on_level_done) save_log_file();

      play_level = next_level;

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
      if (!load_level(play_level, 0))
      {
         new_program_state = 1;
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
         if (LOG_NET_stdf)
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

      if (LOG_NET)
      {
         sprintf(msg,"LEVEL %d STARTED", play_level);
         add_log_entry_header(10, 0, msg, 3);
      }

      // only do fancy zoom into level if not in netgame
      if ((!ima_client) && (!ima_server)) stimp();

      show_player_join_quit_timer = 0;
      start_music(0); // rewind and start theme
      init_timestamps();
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
      init_timestamps();
      program_state = 11;
   }


   //---------------------------------------
   // server new game
   //---------------------------------------
   if (program_state == 20)
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
      if (LOG_NET_stdf)
      {
         //   printf("saved server state[1]:%d\n\n", frame_num);
         sprintf(msg, "stdf saved server state[1]:%d\n", frame_num);
         add_log_entry2(27, 0, msg);
      }

      add_game_move(0, 0, 0, play_level);       // [00] game_start

      // save colors in game moves array
      for (int p=0; p<NUM_PLAYERS; p++)
         if (players[p].active) add_game_move(0, 1, p, players[p].color); // [01] player_state and color

      if (LOG_NET)
      {
         sprintf(msg,"LEVEL %d STARTED", play_level);
         add_log_entry_header(10, 0, msg, 3);
      }

      show_player_join_quit_timer = 0;
      start_music(0); // rewind and start theme
      init_timestamps();
      program_state = 11;
   }

}





// ----------------------------------------------------------
// Is the timer too far ahead of current frame?
// Must be done before call to move_frame()
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
   if ((frame_num+1) < al_get_timer_count(fps_timer)) // skip drawing frame
   {
      players1[active_local_player].frames_skipped++;
      players1[active_local_player].frames_skipped_last_sec_tally++;
      //printf("Skipped drawing frame:%d\n", frame_num);
      return 0;
   }
   return 1;
}

void process_level_done_mode(void)
{
   if (players[0].level_done_mode == 9) // pause players and set up exit xyincs
   {
      show_player_join_quit_timer = 60;
      show_player_join_quit_player = players[0].level_done_player;
      show_player_join_quit_jq = 2;

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
         for (int p=0; p<NUM_PLAYERS; p++)
            if (players[p].active)
            {
               if (has_player_acknowledged(p)) players[p].level_done_ack = 1;
               else players[p].level_done_ack = 0;
            }
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
      if (players[0].level_done_mode == 1) program_state = 12;                // load new level
   }
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
      // process fast packet loops
      // ----------------------------------------------------------
      if (ima_server) server_fast_packet_loop();
      if (ima_client) client_fast_packet_loop();


      // ----------------------------------------------------------
      // do things based on the 40 Hz fps_timer event
      // ----------------------------------------------------------
      if (program_update)
      {
         program_update = 0;

         if (program_state == 11) // game loop running
         {
            frame_num++;
            update_animation();

            //add_timestamp(1, 0,0,0,0);
            timestamp_frame_start = al_get_time();

            proc_timer_adjust();
            proc_scale_factor_change();

 //           add_timestamp(2, 0,0,0,0);

            if (ima_server) server_control();
            if (ima_client) client_control();

//            add_timestamp(3, 0,0,0,0);

            proc_player_input();
            proc_game_moves_array();

//            add_timestamp(4, 0,0,0,0);

            if (players[0].level_done_mode) process_level_done_mode();
            else move_frame();

//            add_timestamp(5, 0,0,0,0);

            if (players1[0].server_send_dif) server_send_stdf();

//            add_timestamp(6, 0,0,0,0);

            if (proc_frame_skip()) draw_frame();

//
//            add_timestamp(7, 0,0,0,0);


            double pt = al_get_time() - timestamp_frame_start;
            double cpu = (pt/0.025)*100;
            qG[0].add_data(0, cpu);



//            double td=0, ts=0, tm=0, tt=0;
//            if (get_delta(frame_num,   6, frame_num,   7, td)) printf("time in draw: %5.3f us\n", td*1000000);
//
//            if (get_delta(frame_num,   5, frame_num,   6, ts)) printf("time in stdf: %5.3f us\n", ts*1000000);
//
//            if (get_delta(frame_num,   4, frame_num,   5, tm)) printf("time in move: %5.4f us\n", tm*1000000);
//            if (get_delta(frame_num-1, 1, frame_num,   1, tt)) printf("total   time: %5.5f us\n", tt*1000000);
//
//
//            sprintf(msg, "tmst [%0.4f] [%0.4f] [%0.4f]\n", td*1000, tm*1000, tt*1000);
//            add_log_entry2(44, 0, msg);
//


//
//
//            double base = 0, m1 = 0, m2 = 0, d1 = 0, d2 = 0;
//            get_timestamp(frame_num, 0, base);
//            get_timestamp(frame_num, 1, m1);
//            get_timestamp(frame_num, 2, m2);
//
//            get_timestamp(frame_num, 3, d1);
//
//            get_timestamp(frame_num, 4, d2);
//
//            //printf("base:%f m1:%f m2:%f d1:%f d2:%f\n", base, m1, m2, d1, d2);
//
//            printf("base:0 m1:%f m2:%f d1:%f d2:%f\n", m1-base, m2-base, d1-base, d2-base);

//            // speed test... draw every frame
//            proc_frame_skip();
//            draw_frame();

//            // speed test...draw no frames...maybe 1 in 1000
//            proc_frame_skip();
//            if (frame_num)
//            {
//               if ((frame_num % 10000)==0) draw_frame();
//            }


            if ((ima_client) && (frame_num > 100))
            {
               int mod = 80;
               if (ping_num_filled < 8) mod = 10;
               if ((frame_num % mod) == 0)
               {
                  //printf("ping server\n");
                  Packet("ping");
                  PacketPutDouble(al_get_time());
                  ClientSend(packetbuffer, packetsize);
               }
            }
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

            if (ima_server)
               for (int p=1; p<NUM_PLAYERS; p++)
                  if (players[p].control_method == 2)
                  {
                     players1[p].late_cdats_last_sec = players1[p].late_cdats_last_sec_tally;
                     players1[p].late_cdats_last_sec_tally = 0;

                     if (players1[p].game_move_dsync_avg_last_sec_count > 0)
                     {
                        players1[p].game_move_dsync_avg_last_sec = players1[p].game_move_dsync_avg_last_sec_tally / players1[p].game_move_dsync_avg_last_sec_count;
                        players1[p].game_move_dsync_avg_last_sec_tally = 0;
                        players1[p].game_move_dsync_avg_last_sec_count = 0;
                     }
                  }


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



