#include "pm.h"



// global variables

//extern int timer_passcount;
extern int draw_frame;

// timer variables
extern int level_time;

extern int speed;
extern int sound_on;

extern int pbullet[50][6];
extern int pm_bullet_collision_box;

extern int level_done;

// counters and temp string
extern char b_msg[40][80];
extern int bottom_msg;
extern int mx;
extern int my;
void add_screen_msg(char *txt, int x, int y, int delay, int ssn, int z1, int z2, int z3, int z4);
void draw_screen_msg(void);



void add_screen_msg(char *txt, int x, int y, int delay, int ssn, int z1, int z2, int z3, int z4 )
{
   // check to see if identical to any active entry
   int already_exists = 0;
    for (int a=0; a<100; a++)
       if (screen_msgs[a].active)
          if (screen_msgs[a].original_x == x)
             if (screen_msgs[a].original_y == y)
                if (strcmp(screen_msgs[a].text, txt) == 0)
                   if (ssn != 4 ) already_exists = 1;

   if (!already_exists)
   {
      for (int a=0; a<100; a++)
         if (!screen_msgs[a].active) // find first empty
         {
             // default settings
             screen_msgs[a].delay = 40;
             screen_msgs[a].active = 1;
             screen_msgs[a].color = 15;
             screen_msgs[a].color_inc = 0;
             screen_msgs[a].color_inc_holdoff = 0;
             screen_msgs[a].current_holdoff = 0;
             screen_msgs[a].delay = delay;
             screen_msgs[a].ssn = ssn;
             screen_msgs[a].original_x = x;
             screen_msgs[a].original_y = y;
             screen_msgs[a].x = x;
             screen_msgs[a].y = y;
             screen_msgs[a].xinc = 0;
             screen_msgs[a].yinc = 0;
             sprintf(screen_msgs[a].text, "%s", txt);
             screen_msgs[a].size = 1.0;
             screen_msgs[a].size_inc = 0.0;
             screen_msgs[a].rot = 0;
             screen_msgs[a].rot_inc = 0;

             // specific overrides

             if (ssn == 1) // Door
             {
                screen_msgs[a].delay = 1;
                screen_msgs[a].color = 0;

                screen_msgs[a].rot = 0;
                screen_msgs[a].rot_inc = 0;

                screen_msgs[a].size = 1.0;
                screen_msgs[a].size_inc = -.000;

                screen_msgs[a].x = x;
                screen_msgs[a].y = y-12; // raise above door



                // multistep
                screen_msgs[a].current_step = 1;

                // initial step set here
                screen_msgs[a].size = 0;
                screen_msgs[a].size_inc = 0;


             }


             if (ssn == 2) // Health +
             {
                screen_msgs[a].delay = 1;
                screen_msgs[a].color = 11; // green

                // multistep
                screen_msgs[a].current_step = 1;

                // initial step set here
                screen_msgs[a].size = 0;
                screen_msgs[a].size_inc = 0;

             }

             if (ssn == 3) // Health -
             {
                screen_msgs[a].delay = 1;
                screen_msgs[a].color = 10; // red

                // multistep
                screen_msgs[a].current_step = 1;

                // initial step set here
                screen_msgs[a].size = 0;
                screen_msgs[a].size_inc = 0;

             }





             if (ssn == 4) // player shoots, bang!
             {
//                screen_msgs[a].delay = 16;
                screen_msgs[a].delay = 8;

                int p = z1; // player num
                int b = z2; // bullet num

                screen_msgs[a].x = pbullet[b][2];
                screen_msgs[a].y = pbullet[b][3];

                screen_msgs[a].xinc = pbullet[b][4]/3;
                screen_msgs[a].yinc = pbullet[b][5]/3;

                // do one inc to see if lines up better
                screen_msgs[a].x += pbullet[b][4];
                screen_msgs[a].y += pbullet[b][5];


                screen_msgs[a].color = players[p].color;


                screen_msgs[a].size = .2;
                screen_msgs[a].size_inc = .1;


             }


             if (ssn == 5) // key
             {
                screen_msgs[a].delay = 20;
                // z2 == key_shape

                if  (z2 == 1039) screen_msgs[a].color = 10; //red
                if  (z2 == 1040) screen_msgs[a].color = 11; //green
                if  (z2 == 1041) screen_msgs[a].color = 13; //lt blue
                if  (z2 == 1042) screen_msgs[a].color = 8; //purple

             }

             if (ssn == 6) // boing
             {
                screen_msgs[a].delay = 20;
                screen_msgs[a].color = 14; //yellow

                screen_msgs[a].rot = 0;
                screen_msgs[a].rot_inc = 0;

                screen_msgs[a].size = .4;
                screen_msgs[a].size_inc = .015;

                screen_msgs[a].x = x;
                screen_msgs[a].y = y;
                screen_msgs[a].yinc = -4;

             }


             if (ssn == 7) // switch
             {
                screen_msgs[a].delay = 20;
                screen_msgs[a].color = 15; // white

                screen_msgs[a].rot = 0;
                screen_msgs[a].rot_inc = 0;

                screen_msgs[a].size = .8;
                screen_msgs[a].size_inc = .005;

                screen_msgs[a].x = x;
                screen_msgs[a].y = y;
                screen_msgs[a].yinc = -1;

             }


             if ((ssn == 34) || (ssn == 35))  // squished or stuck
             {
                screen_msgs[a].delay = 8;
                screen_msgs[a].color = 10; // red

                screen_msgs[a].rot = 0;
                screen_msgs[a].rot_inc = 0;

                screen_msgs[a].size = .8;
                screen_msgs[a].size_inc = .035;

                screen_msgs[a].x = x;
                screen_msgs[a].y = y-10;
                screen_msgs[a].yinc = -3;

             }

              a =100; // quit loop
         }
   }
}

void draw_screen_msg(void)
{
    for (int a=0; a<100; a++)
       if (screen_msgs[a].active)
       {
          int ssn = screen_msgs[a].ssn;

          int x = screen_msgs[a].x;
          int y = screen_msgs[a].y;
          int c = screen_msgs[a].color;
          int rot = screen_msgs[a].rot;
          float s = screen_msgs[a].size;

          rtextout_centre(level_buffer, screen_msgs[a].text, x + 10, y + 10, c, s, rot, 1 );

           // for all mode that aren't multimode
           if ((ssn == 0) || (ssn == 0) || (ssn == 4) || (ssn == 5) || (ssn == 6) || (ssn == 7) || (ssn == 34)|| (ssn == 35))
           {
             // decrement delay till 0 then set inactive
             --screen_msgs[a].delay;
             if (screen_msgs[a].delay == 0) screen_msgs[a].active = 0;
           }

           if (screen_msgs[a].ssn == 2) // health +
           {
             --screen_msgs[a].delay;

              if (screen_msgs[a].current_step == 1) // immediate next
              {
                 if (screen_msgs[a].delay == 0) // next out
                 {
                    screen_msgs[a].delay = 5;
                    screen_msgs[a].color = 11;
                    screen_msgs[a].yinc = -5;
                    screen_msgs[a].size = .5;
                    screen_msgs[a].size_inc = .11;
                    screen_msgs[a].current_step = 2;
                 }
              }
              if (screen_msgs[a].current_step == 2)
              {
                 if (screen_msgs[a].delay == 0) // next wait
                 {
                    screen_msgs[a].delay = 30;
                    screen_msgs[a].color = 11;

                    screen_msgs[a].yinc = 0;
                    screen_msgs[a].size_inc = 0;
                    screen_msgs[a].current_step = 3;
                 }
              }

              if (screen_msgs[a].current_step == 3)
              {
                 if (screen_msgs[a].delay == 0) // next in
                 {
                    screen_msgs[a].delay = 6;
                    screen_msgs[a].color = 11;
                    screen_msgs[a].yinc = 4;
                    screen_msgs[a].size_inc = -.14;
                    screen_msgs[a].current_step = 8;
                 }
              }
              if (screen_msgs[a].current_step == 8)
              {
                 if (screen_msgs[a].delay == 0)
                 {
                    screen_msgs[a].active = 0;  // done
                 }
              }

          }




           if (screen_msgs[a].ssn == 3) // health -
           {
             --screen_msgs[a].delay;

              if (screen_msgs[a].current_step == 1) // immediate next
              {
                 if (screen_msgs[a].delay == 0) // next out
                 {
                    screen_msgs[a].delay = 5;
                    screen_msgs[a].color = 10;
                    screen_msgs[a].yinc = -8;
                    screen_msgs[a].size = .5;
                    screen_msgs[a].size_inc = .11;
                    screen_msgs[a].rot_inc = 0;
                    screen_msgs[a].current_step = 2;
                 }
              }
              if (screen_msgs[a].current_step == 2)
              {
                 if (screen_msgs[a].delay == 0) // next wait
                 {
                    screen_msgs[a].delay = 30;
                    screen_msgs[a].yinc = 0;
                    screen_msgs[a].size_inc = 0;
                    screen_msgs[a].current_step = 3;
                 }
              }

              if (screen_msgs[a].current_step == 3)
              {


                 if (screen_msgs[a].delay == 0) // next in
                 {
                    screen_msgs[a].delay = 6;
                    screen_msgs[a].yinc = 4;
                    screen_msgs[a].size_inc = -.14;
                    screen_msgs[a].current_step = 8;
                 }
              }
              if (screen_msgs[a].current_step == 8)
              {
                 if (screen_msgs[a].delay == 0)
                 {
                    screen_msgs[a].active = 0;  // done
                 }
              }

          }




           if (screen_msgs[a].ssn == 1) // door (fade in and out)
           {
             --screen_msgs[a].delay;

              if (screen_msgs[a].current_step == 1) // immediate next
              {
                 if (screen_msgs[a].delay == 0) // fade in
                 {
                    screen_msgs[a].delay = 6;

                    screen_msgs[a].color = 12 + (6 * 32);

                    screen_msgs[a].color_inc = - 32;

//                  screen_msgs[a].color_inc_holdoff = screen_msgs[a].delay / 12;
                    screen_msgs[a].color_inc_holdoff = 1;

                    screen_msgs[a].current_holdoff = screen_msgs[a].color_inc_holdoff;

                    screen_msgs[a].size = 1.0;
                    screen_msgs[a].size_inc = 0;
                    screen_msgs[a].current_step = 2;
                 }
              }
              if (screen_msgs[a].current_step == 2)
              {
                 if (screen_msgs[a].delay == 0) // next wait
                 {
                    screen_msgs[a].delay = 30;
                    screen_msgs[a].color = 12;
                    screen_msgs[a].color_inc = 0;
                    screen_msgs[a].current_step = 3;
                 }
              }




              if (screen_msgs[a].current_step == 3)
              {
                 if (screen_msgs[a].delay == 0) // fade out
                 {
                    screen_msgs[a].delay = 15;
                    screen_msgs[a].color = 12;
                    screen_msgs[a].color_inc = 16;
                    screen_msgs[a].color_inc_holdoff = 1;

                    screen_msgs[a].current_holdoff = screen_msgs[a].color_inc_holdoff;


                    screen_msgs[a].current_step = 4;
                 }
              }

              if (screen_msgs[a].current_step == 4)
              {
                 if (screen_msgs[a].delay == 0)
                 {
                    screen_msgs[a].active = 0;  // done
                 }
              }
          }

          // increment the size
          screen_msgs[a].size = screen_msgs[a].size + screen_msgs[a].size_inc;

          // increment the rot
          screen_msgs[a].rot = screen_msgs[a].rot + screen_msgs[a].rot_inc;

          // increment x and y
          screen_msgs[a].x = screen_msgs[a].x + (int) screen_msgs[a].xinc;
          screen_msgs[a].y = screen_msgs[a].y + (int) screen_msgs[a].yinc;

          // increment color
          if (screen_msgs[a].color_inc) // do nothing if no color_inc
          {
             if (--screen_msgs[a].current_holdoff <= 0)
             {
                screen_msgs[a].color += screen_msgs[a].color_inc;

                if (screen_msgs[a].color < 0) screen_msgs[a].color += 256;
                if (screen_msgs[a].color < 255) screen_msgs[a].color -= 256;

                screen_msgs[a].current_holdoff = screen_msgs[a].color_inc_holdoff;
             }
          }

       }
}


void new_bmsg(char *nb)
{
    slide_bmsg();
    sprintf(b_msg[0], "%s", nb);
    bottom_msg = 120;
    erase_last_bmsg();
}

void event(int ev, int x, int y, int z1, int z2, int z3, int z4)
{
   extern int sample_delay[8];
   int text_on = 1;
   int screen_messages_on = 1;
   if (sound_on)
   {
      switch (ev)
      {
         /*  sample numbers
         0 - player shoots
         1 - d'OH
         2 - bonus
         3 - hiss
         4 - la dee dah  door, key, exit
         5 - explosion
         6 - grunt 1 shot
         7 - grunt 2 hit
         8 - enemy killed  */
        case  1: // player shoots
           al_play_sample(snd[0], 0.71, 0, .8, ALLEGRO_PLAYMODE_ONCE, NULL);
        break;
        case  2: case  4: case  5: // la dee dah (key, exit, door)
           if (sample_delay[4]+30 < passcount)
           {
              sample_delay[4] = passcount;
              al_play_sample(snd[4], 0.78, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
           }
        break;
        case  6: case  9: // health bonus and free man
           if (sample_delay[2]+30 < passcount)
           {
              sample_delay[2] = passcount;
              al_play_sample(snd[2], 0.78, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
              //stop_sample(snd[2]); play_sample( snd[2], (200*se_scaler)/9, 127, 1000, 0);
           }
        break;
        case 11: // player got shot
           al_play_sample(snd[6], 0.5, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
        break;
        case 8: case 10: case 12: case 34: case 35: // player got hit (bomb, mine, enemy collision, squished, stuck)
           if (sample_delay[7]+14 < passcount)
           {
              sample_delay[7] = passcount;
              al_play_sample(snd[7], 0.5, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
              //stop_sample(snd[7]); play_sample( snd[7], (127*se_scaler)/9, 127, 1000, 0);
           }
        break;
        case 13: // enemy killed
             al_play_sample(snd[8], 0.5, 0, 1.2, ALLEGRO_PLAYMODE_ONCE, NULL);
        break;
        case 21: // d'Oh (player died)
             al_play_sample(snd[1], 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
        break;
        case 22: // explosion
             al_play_sample(snd[5], .78, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
            //stop_sample(snd[5]); play_sample( snd[5], (200*se_scaler)/9, 127, 1000, 0);
        break;
      }
   }


   if (text_on)
   {
//      erase_last_bmsg();
      switch (ev)
      {
//         case   1:/* player bullet fired */ break;
         case   2: new_bmsg((char*)"You got a key!"); break;
         case   3:
         if (z1 == 1) sprintf(msg, "1 enemy left to kill!!");
         else sprintf(msg, "%d enemies left to kill!", z1);
         new_bmsg(msg); break;

//         case   4: /* level_done  */ break;
         case   5: new_bmsg((char*)"Door!"); break;
         case   6: sprintf(msg, "Health + %d", z1 ); new_bmsg(msg); break;
         case   7: sprintf(msg, "Health - %d", z1 ); new_bmsg(msg); break;

         case   8: sprintf(msg, "Bomb Damage %d", z1 ); new_bmsg(msg); break;

         case   9: new_bmsg((char*)"Wahoo! You got a Free Man!"); break;
         case  10: new_bmsg((char*)"You hit a Mine!"); break;
         case  11: new_bmsg((char*)"You got shot!"); break;
         case  12: sprintf(msg, "You got hit by %s!", enemy_name[Ei[z1][0]]); new_bmsg(msg); break;
         case  13: sprintf(msg, "%s killed!...%d enemies left", enemy_name[Ei[z1][0]],  num_enemy-1); new_bmsg(msg); break;
//         case  14: /* cloner cloned something */ break;
//         case  15: /* jump  */ break;
//         case  16: /* arrow fired */ break;
//         case  17: /* green fired */ break;
//         case  18: /* cannonball  */ break;
//         case  19: /* twirly      */ break;
         case  21: new_bmsg((char*)"You Died!"); break;
//         case  22: /* explosion */ break;
         case  23: new_bmsg((char*)"Enemy killed by bomb!"); break;
         case  24: sprintf(msg, "#%d bomb with %d second fuse",item[x][7]/20, item[x][9]/10 );  new_bmsg(msg); break;
         case  25: new_bmsg((char*)"Rocket!"); break;
         case  26: new_bmsg((char*)"You already have perfect health!"); break;
         case  30: new_bmsg((char*)"You got a Switch"); break;
         case  31: new_bmsg((char*)"Sproingy!"); break;
         case  32: new_bmsg((char*)"Door!"); break;
//         case  33: /* Out Door */ break;
         case  34: new_bmsg((char*)"You got squished!"); break;
         case  35: new_bmsg((char*)"You got stuck!"); break;
      }
   }
   if (screen_messages_on)
      switch (ev)
      {
//         case   1: add_screen_msg("bang!", x, y, 100, 4, z1, z2, z3, z4); break;
//         case   2: add_screen_msg("key", x, y, 100, 5, z1, z2, z3, z4); break;
 //        case   3: sprintf(msg, "%d enemies left to kill!", z1);
 //                  add_screen_msg(msg, x, y, 100, 0, z1, z2, z3, z4); break;
//         case   3: add_screen_msg("------O------", x, y, 100, 0); break;
//         case   5: add_screen_msg("--O--", x, y, 100, 0); break;
//         case   5: add_screen_msg("Door", x, y, 100, 1, z1, z2, z3, z4); break;
         case   6:  sprintf(msg, "H+%d", z1);
                    add_screen_msg(msg, x, y, 100, 2, z1, z2, z3, z4); break;
         case   7:  sprintf(msg, "H-%d", z1);
                    add_screen_msg(msg, x, y, 100, 3, z1, z2, z3, z4); break;
//         case  8: add_screen_msg("Bomb Damage!", x, y, 100, 0, z1, z2, z3, z4); break;
//         case   10: add_screen_msg("Mine", x, y, 100, 0, z1, z2, z3, z4); break;
//         case  13: sprintf(msg, "%s died! (%d left)", enemy_name[Ei[z1][0]],  num_enemy-1);
//                   add_screen_msg(msg, x, y, 100, 0, z1, z2, z3, z4); break;
//         case  21: add_screen_msg("You Died!", x, y, 100, 0, z1, z2, z3, z4); break;
//         case  30: add_screen_msg("switch", x, y, 100, 7, z1, z2, z3, z4); break;
//         case  31: add_screen_msg("boing!", x, y, 100, 6, z1, z2, z3, z4); break;
//         case  32: add_screen_msg("In", x, y, 100, 1, z1, z2, z3, z4); break;
//         case  33: add_screen_msg("Out", x, y, 100, 1, z1, z2, z3, z4); break;
//         case  34: add_screen_msg("Ouch", x, y, 100, 34, z1, z2, z3, z4); break;
//         case  35: add_screen_msg("Ouch", x, y, 100, 35, z1, z2, z3, z4); break;
      }
}


void proc_frame_delay(void)
{
   if (al_get_timer_count(sec_timer) > 0)
   {
      al_set_timer_count(sec_timer, 0); // reset_second_timer
      actual_fps = passcount - last_fps_passcount;
      last_fps_passcount = passcount;
      frames_skipped_last_second = (players1[active_local_player].frames_skipped - last_frames_skipped);
      last_frames_skipped = players1[active_local_player].frames_skipped;
   }
   if ((making_video) || (speed_testing)) // draw every frame no matter how fast or slow it is
   {
      draw_frame = 1;
      al_set_timer_count(fps_timer, passcount);
   }
   else
   {
      if (passcount <=  al_get_timer_count(fps_timer)) // skip drawing frame
      {
         draw_frame = 0;
         players1[active_local_player].frames_skipped++;
      }
      else draw_frame = 1;
      while (passcount > al_get_timer_count(fps_timer)); // delay if too far ahead so timer catches up
   }
}

void proc_player_health(void)
{
   for (int p=0; p<NUM_PLAYERS; p++)
   {
      if ((players[p].active) && (!players[p].paused))
      {
         if (players[p].old_LIFE != players[p].LIFE)
         {
            players1[p].last_health_adjust = al_fixtoi(players[p].LIFE - players[p].old_LIFE);
            players[p].old_LIFE = players[p].LIFE;
            players1[p].health_display = 80;
         }

         if (players[p].LIFE>al_itofix(100))
         {
             players[p].LIFE = al_itofix(100); // enforce max
             players[p].old_LIFE = al_itofix(100); // to prevent display
         }


         if (players[p].LIFE < al_itofix(1)) // is LIFE < 1
         {
            players[p].LIFE = al_itofix(0);
            if (L_LOGGING_NETPLAY)
            {
               #ifdef LOGGING_NETPLAY
               sprintf(msg,"PLAYER:%d DIED!", p);
               add_log_entry_header(10, 0, msg, 1);
               #endif
            }

            event(21, al_fixtoi(players[p].PX), al_fixtoi(players[p].PY), 0, 0, 0, 0);  // player death

            show_player_join_quit_timer = 60;
            show_player_join_quit_player = p;
            show_player_join_quit_jq = 3;

            players1[p].health_display = 200;
            players[p].paused = 100;
            players[p].paused_type = 1;

            if (--players[p].LIVES < 0) // GAME OVER
            {
               players[p].LIVES = 5;
            }
         }
      }
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
   if (ima_server)
   {
      void server_flush(void);
      server_flush();
   }
   if (ima_client)
   {
      void client_flush(void);
      client_flush();
   }

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
      play_level++;
      start_mode = 3; // level_done
   }
}

void pm_main(void) // game loop!
{
   if (!start_mode) // resume
   {
      stimp();
      al_set_timer_count(fps_timer, passcount); // sync timer_passcount to actual
      if (sound_on) al_set_audio_stream_playing(pm_theme_stream, 1);
   }
   while (!game_exit) // game loop
   {
      if (start_mode)
      {
         if (start_mode < 3) // normal or run game
         {
            passcount = 0;
            if (!load_level(play_level,0))
            {
               game_exit = 1;
               resume_allowed = 0;
            }
         }

         if (start_mode == 3) // level done
         {
            stamp();
            passcount = 0;
            if (!load_level(play_level,0))
            {
               game_exit = 1;
               resume_allowed = 0;
            }
         }

         if (start_mode != 2) // skip this when start_mode == 2 ; run game
         {
            // reset game_move array
            for (int x=0; x<1000000; x++)
               for (int y=0; y<4; y++)
                  game_moves[x][y] = 0;
            game_move_entry_pos = 0;

            // add initial level info to game_move array (unless client)
            if (!ima_client) // server or local
            {
               int p = 0; // server and local game always use player 0
               add_game_move(0, 0, play_level, 0);       // [00] game_start
               add_game_move(0, 1, p, players[p].color); // [01] player_state and color
            }
            if (L_LOGGING)
            {
               #ifdef LOGGING
               {
                  sprintf(msg,"LEVEL %d STARTED", play_level);
                  add_log_entry_header(10, 0, msg, 3);
                  log_player_array();
               }
               #endif
            }
         }

         // reset player data
         for (int p=0; p<NUM_PLAYERS; p++) init_player(p, 2);

         for (int k = ALLEGRO_KEY_A; k < ALLEGRO_KEY_MAX; k++) key[k] = 0; // clear_key array

         // reset sound counters
         for (int c=0; c<8; c++) sample_delay[c] = passcount;

         clear_bullets();

         show_player_join_quit_timer = 0;

         level_done = 0;
         bottom_msg=0;

         start_mode = 0;
         stimp();
         al_set_timer_count(fps_timer, passcount);
         if (sound_on) al_rewind_audio_stream(pm_theme_stream);
         if (sound_on) al_set_audio_stream_playing(pm_theme_stream, 1);
      } // end of if (start_mode)

      proc_scale_factor_change();
      move_lifts(0);
      proc_item_collision();
      proc_item_move();
      proc_lit_bomb();
      proc_lit_rocket();
#ifdef NETPLAY
      if (ima_server) server_control(); // runs only once per frame
      if (ima_client) client_control(); // runs only once per frame
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
   } // end of while (!game_exit)
   stop_sound();
   stamp();
}
