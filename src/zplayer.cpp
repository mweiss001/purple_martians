// zplayer.cpp

#include "pm.h"



void get_player_start_pos(int p)
{
   for (int c=0; c<500; c++)  // get start and time
   if (item[c][0] == 5)
   {
      players[p].PX = itemf[c][0];
      players[p].PY = itemf[c][1];
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

            sprintf(msg,"PLAYER:%d DIED!", p);
            if (L_LOGGING_NETPLAY) add_log_entry_header(10, 0, msg, 1);

            game_event(21, al_fixtoi(players[p].PX), al_fixtoi(players[p].PY), 0, 0, 0, 0);  // player death

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

void player_move(void)
{
   al_fixed z = al_itofix(0);
   al_fixed gravity = al_ftofix(.60);
   al_fixed slow_gravity = al_ftofix(.2); // used when jump is held
   al_fixed initial_jump_velocity = al_ftofix(-6.6);
   al_fixed terminal_velocity = al_ftofix(7.8);
   al_fixed x_accel = al_ftofix(.12);
   al_fixed x_de_accel = al_ftofix(.24);
   al_fixed max_x_velocity = al_ftofix(4);
   al_fixed initial_x_velocity = al_ftofix(1.15);

   for (int p=0; p<NUM_PLAYERS; p++)
   {
      if (((players[p].active) && (!players[p].paused)) &&
          (!((players[p].carry_item) && (item[players[p].carry_item-1][0] == 98))))  // not riding rocket
      {


// ---------------- check to see if player stuck in blocks...then kill him slowly!

         int x = al_fixtoi(players[p].PX);
         int y = al_fixtoi(players[p].PY);
         int su =    is_up_solid(x, y, 0);
         int sd =  is_down_solid(x, y, 0);
         int sl =  is_left_solid(x, y, 0);
         int sr = is_right_solid(x, y, 0);
         if ((su == 1) && (sd) && (sl) && (sr))
         {
            players[p].LIFE -= al_itofix(1);
            game_event(7, x, y, 1, 0, 0, 0);
            game_event(35, x, y, 0, 0, 0, 0);

         }


// ---------------------------   x move  ---------------------
         if ((players[p].left) && (!players[p].right)) // left only
         {
            players[p].left_right = 0;
            if (players[p].left_xinc > -initial_x_velocity) players[p].left_xinc = -initial_x_velocity;
            players[p].left_xinc -= x_accel;     // accel left
            players[p].right_xinc -= x_de_accel; // de accel right
         }

         if ((players[p].right) && (!players[p].left) ) // right only
         {
            players[p].left_right = 1;
            if (players[p].right_xinc < initial_x_velocity) players[p].right_xinc = initial_x_velocity;
            players[p].right_xinc += x_accel;   // accel right
            players[p].left_xinc += x_de_accel; // de-accel left
         }

         if ((!players[p].right) && (!players[p].left) ) // neither left nor right pressed
         {
            players[p].left_xinc  += x_de_accel; // de-accel left
            players[p].right_xinc -= x_de_accel; // de accel right
         }

         if ((players[p].right) && (players[p].left) ) // both left and right pressed
         {
            players[p].left_xinc  += x_de_accel; // de-accel left
            players[p].right_xinc -= x_de_accel; // de accel right
         }

            // why don't I just do all the bounds checks here, just once.
         if (players[p].right_xinc < z ) players[p].right_xinc = z;
         if (players[p].right_xinc > max_x_velocity) players[p].right_xinc = max_x_velocity;

         if (players[p].left_xinc > z ) players[p].left_xinc = z;
         if (players[p].left_xinc < -max_x_velocity) players[p].left_xinc = -max_x_velocity;

         if (is_right_solid(x, y, 0)) players[p].right_xinc = z;
         if (is_left_solid(x, y, 0))  players[p].left_xinc = z;

         players[p].xinc = players[p].left_xinc + players[p].right_xinc;  // calc xinc
         players[p].PX += players[p].xinc;                                // apply xinc


         x = al_fixtoi(players[p].PX);
         y = al_fixtoi(players[p].PY);
         if (players[p].player_ride) // if player is riding lift
         {
            int d = players[p].player_ride - 32; // lift number
            // moving right

            if ((lifts[d].fxinc > z) && (!is_right_solid(x, y, 1))) players[p].PX  += lifts[d].fxinc;

            // moving left
            if ((lifts[d].fxinc < z) && (!is_left_solid(x, y, 1))) players[p].PX  += lifts[d].fxinc;
         }




         if (int a = is_left_solid(x, y, 1))
         {
            if ( a > 31 ) // player being pushed right by lift
            {
               if (!is_right_solid(x, y, 0))
               {
                  int l = a-32; // lift that is pushing
                  int width = lifts[l].width *20 + 1;
                  players[p].PX = lifts[l].fx + al_itofix(width); // snap to lift pos + width
                  players[p].right_xinc = lifts[l].fxinc; // set players xinc from lift
               }
               else // player is getting squished
               {
                  players[p].LIFE -= al_itofix(1);
                  game_event(7, x, y, 1, 0, 0, 0);
                  game_event(34, x, y, 0, 0, 0, 0);

               }
            }
            else  if (players[p].xinc < al_itofix(0)) // moving left and block collision
            {
               players[p].left_xinc = z;
               players[p].PX = al_itofix((((x/20)+1) * 20) - 3);  // align with wall
            }
         }
         x = al_fixtoi(players[p].PX);
         y = al_fixtoi(players[p].PY);
         if (int a = is_right_solid(x, y, 1))
         {
            if (a > 31)    // player being pushed left by lift
            {
               if (!is_left_solid(x, y, 0))
               {
                  int l = a-32; // lift that is pushing
                  players[p].PX = lifts[l].fx - al_itofix(20);       // snap to lift pos - 20
                  players[p].left_xinc = lifts[a-32].fxinc;       // set players xinc from lift
               }
               else // player is getting squished
               {
                  players[p].LIFE -= al_itofix(1);
                  game_event(7, x, y, 1, 0, 0, 0);
                  game_event(34, x, y, 0, 0, 0, 0);
               }
            }
            else if (players[p].xinc > z) // moving right and block collision
            {
               players[p].right_xinc = z;
               players[p].PX = al_itofix(((x/20) * 20) + 2); // align with wall
            }
         }




// ---------------   check to see if player riding lift starts or ends --------------------------
         x = al_fixtoi(players[p].PX);
         y = al_fixtoi(players[p].PY);
         int a = is_down_solid(x, y, 1);                                // check for lift below player
         if (a > 31)
         {
            if (lifts[a-32].fyinc > z)                                  // is lift going down
            {
               if (is_down_solid(x, y, 0)) players[p].player_ride = 0;  // check for block below ignoring lifts
               else players[p].player_ride = a;
            }
            if (lifts[a-32].fyinc <= z)                                 // is lift going up or steady
            {
                int offset = lifts[a-32].y1 - y;                        // to prevent lift from picking up early
                if (offset < 21) players[p].player_ride = a;
            }
         }
         else players[p].player_ride = 0;





// -----------   y move  ---------------------


         if (players[p].player_ride) // if player is riding lift
         {
            players[p].yinc = z;
            int d = players[p].player_ride - 32; // lift number
            int x = al_fixtoi(players[p].PX);
            int y = al_fixtoi(players[p].PY);

            // if moving up and solid block above
            if ((lifts[d].fyinc < z) && (is_up_solid(x, y, 0) == 1))
            {
               players[p].player_ride = 0;     // player knocked off lift due to collision above
               players[p].LIFE -= al_itofix(1);   // take some damage
               game_event(7, x, y, 1, 0, 0, 0);
               game_event(34, x, y, 0, 0, 0, 0);


            }

            // check for collision with lift above
            if (is_up_solid(x, y+1, 1) > 31)
            {
               players[p].player_ride = 0;     // player knocked off lift due to collision above
               players[p].LIFE -= al_itofix(1);   // take some damage
               game_event(7, x, y, 1, 0, 0, 0);
               game_event(34, x, y, 0, 0, 0, 0);
            }

            if (players[p].player_ride)                    // if still riding
               players[p].PY  = lifts[d].fy - al_itofix(20);  // align with fy

            // moving down
            if (lifts[d].fyinc > z)
            {
               if (is_down_solid(x, y, 0))                          // no lift check
               {
                  players[p].player_ride = 0;                       // ride over
                  players[p].PY = al_itofix(y - (y % 20));             // align with floor
               }
               else players[p].PY = lifts[d].fy - al_itofix(20);       // align with fy
            }


            if (players[p].jump)                                    // if jump pressed
            {
               players[p].player_ride = 0;                          // ride over
               x = al_fixtoi(players[p].PX);
               y = al_fixtoi(players[p].PY);
               int a = is_up_solid(x, y, 1);
               if ((a == 0) || (a == 2))                            // only jump if nothing above
               {
                  players[p].yinc = initial_jump_velocity;
                  players[p].PY += players[p].yinc;                 // apply yinc
                  game_event(15, x, y, 0, 0, 0, 0);
               }
            }

         }
         else // not player ride
         {
            if ((players[p].yinc < z) && players[p].jump)           // if rising and jump pressed
               players[p].yinc += slow_gravity;                     // apply slow gravity
            else players[p].yinc += gravity;                        // apply regular gravity

            if (players[p].yinc < z)                                // if still rising
            {
               players[p].PY += players[p].yinc;                    // apply yinc
               x = al_fixtoi(players[p].PX);
               y = al_fixtoi(players[p].PY);
               int a = is_up_solid(x, y+2, 1);                      // look for collision above
               if ((a == 1) || (a > 31))                            // solid or lift only
               {
                  players[p].PY -= players[p].yinc;                 // take back move
                  players[p].yinc = z;                              // kill upwards motion
                  players[p].PY = al_itofix(((y/20) + 1) * 20);        // align with ceiling
               }
            }
            if (players[p].yinc >= z)                               // falling or steady
            {
               players[p].yinc += gravity;                          // apply gravity to yinc
               if (players[p].yinc > terminal_velocity)             // check for terminal velocity
                  players[p].yinc = terminal_velocity;
               players[p].PY += players[p].yinc;                    // apply yinc

               x = al_fixtoi(players[p].PX);
               y = al_fixtoi(players[p].PY);
               if (is_down_solid(x, y, 0))                          // check for floor below (no lift)
               {
                  players[p].yinc = z;                              // kill downwards motion
                  players[p].PY = al_itofix(y - (y % 20));             // align with floor

                  // check for collision with lift above if lift is moving down
                  int a = is_up_solid(x, y, 1);
                  if ((a > 31) && (lifts[a-32].fyinc > z))
                  {
                     // take some damage
                     players[p].LIFE -= al_itofix(1);
                     game_event(7, x, y, 1, 0, 0, 0);
                     game_event(34, x, y, 0, 0, 0, 0);
                  }

                  if (players[p].jump)                              // if jump pressed
                  {
                     x = al_fixtoi(players[p].PX);
                     y = al_fixtoi(players[p].PY);
                     int a = is_up_solid(x, y, 1);
                     if ((a == 0) || (a == 2))                      // only jump if nothing above
                     {
                        players[p].yinc = initial_jump_velocity;
                        game_event(15, x, y, 0, 0, 0, 0);
                     }
                  }
               }  // end of if floor below
            } // end of if falling or steady
         } // end of not player ride

         // absolute level bounds check
         if (players[p].PX < al_itofix(0)) players[p].PX = al_itofix(0);
         if (players[p].PY < al_itofix(0)) players[p].PY = al_itofix(0);
         if (players[p].PX > al_itofix(1980)) players[p].PX = al_itofix(1980);
         if (players[p].PY > al_itofix(1980)) players[p].PY = al_itofix(1980);
      } // end of if active, not paused, not riding rocket


      if (players[p].paused) // player is still on level but is frozen and controls are inactive
      {
         players[p].player_ride = 0;
         if (players[p].paused_type == 1) // frozen after player dies, until the timer runs out
         {
            players[p].carry_item = 0;
            players[p].player_ride = 0;
            if (--players[p].paused > 0)
            {
               al_fixed sa = al_ftofix(.025);
               players[p].draw_scale -= sa; // shrink player
               if (players[p].draw_scale < al_itofix(0)) players[p].draw_scale = al_itofix(0);

               al_fixed ra;
               if (players[p].left_right) ra = al_ftofix(5);
               else ra = al_ftofix(-5);

               players[p].draw_rot += ra; // rotate player
            }

            else // frozen done !!
            {
               players[p].draw_scale = al_ftofix(1);
               players[p].draw_rot += al_ftofix(0);

               players[p].paused = 0;
               players[p].num_bullets = 200;
               players[p].old_LIFE = al_itofix(100);
               players[p].LIFE = al_itofix(100);
               players[p].left_xinc = al_itofix(0);
               players[p].right_xinc = al_itofix(0);
               players[p].xinc = al_itofix(0);
               players[p].yinc = al_itofix(0);
               get_player_start_pos(p); // get starting position from start block
               draw_level2(NULL, 0, 0, 0, 1, 1, 1, 1, 1); // redraw entire level in case only region has been drawn
            }
         }
         if (players[p].paused_type == 2)// paused type 2: door move
         {
            int x = players[p].door_item;
            int ddrns = players[p].door_draw_rot_num_steps;
            al_fixed amount_to_shrink = al_ftofix(.5);
            al_fixed sa = al_fixdiv(amount_to_shrink, al_itofix(ddrns));  // shrink and grow player inc
            int as = 7; // door open/close animation speed

            if (players[p].paused_mode == 1) // mode 1: enter door
            {
               players[p].paused_mode_count--;
               if (players[p].paused_mode_count)
               {
                  players[p].draw_scale -= sa; // shrink player
                  players[p].draw_rot += players[p].door_draw_rot_inc; // rotate player

                  // open door
                  float ratio = 1 - (float) players[p].paused_mode_count / ddrns;
                  float shape_shift = ratio * as;

                  if (item[x][13] == 448)
                     item[x][1] = 448 + (int)shape_shift;
      //            printf("ratio:%f scaled_ratio%f int_sr:%d shape:%d \n", ratio, shape_shift, int(shape_shift), item[x][1] );

               }
               else // next mode
               {
                  if (item[x][13] == 448)
                     item[x][1] = 448; // restore door shape
                  players[p].paused_mode = 2;
                  players[p].paused_mode_count = players[p].door_num_steps;
                  players[p].xinc = players[p].door_xinc;
                  players[p].yinc = players[p].door_yinc;
                  players[p].draw_rot = players[p].door_draw_rot;
               }
            }

            if (players[p].paused_mode == 2) // mode 2: player move
            {
               players[p].paused_mode_count--;
               if (players[p].paused_mode_count)
               {
                  players[p].PX -= players[p].xinc;
                  players[p].PY -= players[p].yinc;
               }
               else // mode 2 done
               {
                  players[p].paused_mode = 3;
                  players[p].paused_mode_count = ddrns;

                  players[p].xinc=al_itofix(0);
                  players[p].yinc=al_itofix(0);

//                  players[p].draw_rot = al_itofix(0);
//                  players[p].draw_scale = al_itofix(1);

                  // snap to dest...
                  if (item[x][8] == 0) // regular dest
                  {
                     players[p].PX  = al_itofix(item[x][6] * 20);
                     players[p].PY  = al_itofix(item[x][7] * 20);
                  }
                  if (item[x][8] == 1) // liked item dest
                  {
                     int li = item[x][9]; // linked item number
                     players[p].PX  = itemf[li][0];
                     players[p].PY  = itemf[li][1];
                     // set destination key held to prevent immediate retriggering
                     item[li][10] = frame_num;
                  }
                }
            } // end of mode 2

            if (players[p].paused_mode == 3) // mode 3: exit
            {
               players[p].paused_mode_count--;
               if (players[p].paused_mode_count)
               {
                  players[p].draw_scale += sa;                         // un-shrink player
                  players[p].draw_rot -= players[p].door_draw_rot_inc; // un-rotate player
                  // close door (only if linked item exit)
                  if (item[x][8] == 1)    // if linked item exit
                  {
                     float ratio = 1 - (float) players[p].paused_mode_count / ddrns;
                     float shape_shift = ratio * 6 ;
                     int li = item[x][9]; // linked item number
                     if (item[li][13] == 448)
                        item[li][1] = 454 - (int)shape_shift;
    //                 printf("ratio:%f scaled_ratio%f int_sr:%d shape:%d \n", ratio, shape_shift, int(shape_shift), item[li][1] );

                  }
               }
               else // next mode (done)
               {
                  // if linked item exit
                  if (item[x][8] == 1) // liked item dest
                  {
                     int li = item[x][9]; // linked item number
                     if (item[li][13] == 448)
                        item[li][1] = 448;   // restore door shape
                     item[li][10] = frame_num;   // key hold off

                  }
                  players[p].paused = 0;  // the entire thing is done
               }
            }
         } // end of door move
      }  // end of if player paused
   } // end if player iterate
}

void draw_player(int p)
{
   if (players[p].active)
   {
      al_set_target_bitmap(level_buffer);
      get_players_shape(p);
      int AX = al_fixtoi(players[p].PX);
      int AY = al_fixtoi(players[p].PY);
      float scale = al_fixtof(players[p].draw_scale);
      float rot = al_fixtof(al_fixmul(players[p].draw_rot, al_fixtorad_r));

      int flags = ALLEGRO_FLIP_HORIZONTAL;
      if (players[p].left_right) flags = ALLEGRO_FLIP_VERTICAL & ALLEGRO_FLIP_HORIZONTAL;

      al_draw_scaled_rotated_bitmap(player_tile[players[p].color][players[p].shape], 10, 10,  AX+10, AY+10, scale, scale, rot, flags);

      // death sequence star overlay
      if ((players[p].paused) && (players[p].paused_type == 1))
      {
         int pp = players[p].paused;
         int st = 96; // frame to start at
         int sp = 3;  // speed of sequence
         if ((pp < st) && (pp > st-sp*8))
         {
            int seq = (st-players[p].paused) / sp;
            al_draw_bitmap(tile[952+seq], AX, AY, flags);
         }
      }

      if (players1[p].health_display > 0)
      {
         players1[p].health_display--;
         draw_percent_bar(AX + 10, AY - 6, 16, 3, al_fixtoi(players[p].LIFE));
         int h = players1[p].last_health_adjust;
         if (h > 0) al_draw_textf(f3, palette_color[11], AX+10, AY-16, ALLEGRO_ALIGN_CENTER, "%+d", h);
         if (h < 0) al_draw_textf(f3, palette_color[10], AX+10, AY-16, ALLEGRO_ALIGN_CENTER, "%+d", h);
         //if (h > 0) al_draw_textf(font, palette_color[11], AX+10, AY-14, ALLEGRO_ALIGN_CENTER, "%+d", h);
         //if (h < 0) al_draw_textf(font, palette_color[10], AX+10, AY-14, ALLEGRO_ALIGN_CENTER, "%+d", h);

      }
   }
}

void draw_players(void)
{
   for (int p=0; p<NUM_PLAYERS; p++)
      draw_player(p);

   // do this so that that local player is always drawn on top
   int p = active_local_player;
   draw_player(p);
}

void get_players_shape(int p)
{
   int index = 0;
   if (players[p].up) index = 6;
   if (players[p].down) index = 12;

   // animate with x pos
   int pos = (al_fixtoi(players[p].PX) /4) % 5;  // try 5 for now

   // if jump or fall use static shape
   if (players[p].yinc != al_itofix(0)) pos = 0;

   // if riding rocket use static shape
   if ((players[p].carry_item) && (item[players[p].carry_item-1][0] == 98)) pos = 0;

   // if player riding lift animate with player's xpos relative to lift
   if (players[p].player_ride)
   {
      int rx = 20 + al_fixtoi( (players[p].PX - lifts[players[p].player_ride-32].fx) );
      pos = (rx / 4) % 5;  // try 5 for now
   }

   // does paused always mean dead??
   // can it also mean in door travel

   // if paused use static shape
   //if (players[p].paused) pos = 0;
   if (players[p].paused) players[p].shape = 0;
   else players[p].shape = index + pos;
}


int is_player_color_used(int color)
{
   for (int p=0; p<NUM_PLAYERS; p++)
      if ((players[p].active) || (players[p].control_method == 9) || (players[p].control_method == 2))
         if (color == players[p].color) return 1;
   return 0;
}

void init_player(int p, int t)
{
   // 1 = full, 2 = level done, 3 = netplay join

   // common stuff first

   if (t == 1) // full
   {
      players[p].active = 0;
      players[p].paused = 0;
      players[p].draw_rot = al_itofix(0);
      players[p].draw_scale = al_itofix(1);
      players[p].control_method = 0;
      players[p].shape = 0;;
      players[p].old_LIFE = al_itofix(100);
      players[p].LIFE = al_itofix(100);
      players[p].LIVES = 5;
      players[p].num_hits = 0;
      players[p].left_right=0;
      players[p].carry_item = 0;
      players[p].bullet_wait_counter=0;
      players[p].request_bullet = 0;
      players[p].bullet_wait = 5;
      players[p].bullet_speed = 12;
      players[p].num_bullets = 0; // not used anymore
      players1[p].health_display = 0;
      players1[p].last_health_adjust = 0;

      players1[p].client_game_move_sync = 0;
      players1[p].client_game_move_sync_err = 0;

      players1[p].server_game_move_sync = 0;
      players1[p].server_game_move_sync_err = 0;


      players1[p].server_sync = 99;
      players1[p].client_sync = 99;
      players1[p].who = 99;

      players1[p].comp_move = 0;
      players1[p].old_comp_move = 0;

      players1[p].fake_keypress_mode = 0;

      players1[p].up_key =    players1[0].up_key;
      players1[p].down_key =  players1[0].down_key;
      players1[p].left_key =  players1[0].left_key;
      players1[p].right_key = players1[0].right_key;
      players1[p].jump_key =  players1[0].jump_key;
      players1[p].fire_key =  players1[0].fire_key;
      players1[p].menu_key =  players1[0].menu_key;

/*
      players[p].color = 15; // white
      players[p].color = 14; // yellow
      players[p].color = 13; // lt blue
      players[p].color = 12; // dk blue
      players[p].color = 11; // green
      players[p].color = 10; // red
      players[p].color = 9; // lt green
      players[p].color = 8; // original purple
      players[p].color = 7; // orange
      players[p].color = 6; // tan
      players[p].color = 5; // pink
      players[p].color = 4; //lt red
      players[p].color = 3; // med blue
      players[p].color = 2; // reddish purple
      players[p].color = 1; // light purple
      players[p].color = 0; // zombie...don't use
*/

      // customize players
      // don't mess with player 0; its loaded from config file
      if (p==1) players[p].color = 9;  // lt green (forest)
      if (p==2) players[p].color = 6;  // tan
      if (p==3) players[p].color = 14; // yellow
      if (p==4) players[p].color = 11; // green
      if (p==5) players[p].color = 15; // white
      if (p==6) players[p].color = 7;  // orange
      if (p==7) players[p].color = 10; // red

   }

   // common to 2 and 3
   if ((t==2) || (t == 3))
   {
      get_player_start_pos(p); // get starting position from start block
      players[p].paused = 0;
      players[p].old_LIFE = al_itofix(100);
      players[p].LIFE = al_itofix(100);
      players1[p].last_health_adjust = 0;
      players[p].carry_item = 0;
      players[p].player_ride = 0;
      players[p].left_right = 0;
      players[p].left = 0;
      players[p].right = 0;
      players[p].up = 0;
      players[p].down = 0;
      players[p].jump = 0;
      players[p].fire = 0;
      players[p].left_xinc = al_itofix(0);
      players[p].right_xinc = al_itofix(0);
      players[p].xinc = al_itofix(0);
      players[p].yinc = al_itofix(0);
      players1[p].comp_move = 0;
      players1[p].old_comp_move = 0;

      players1[p].client_cdat_packets_tx = 0;

      players1[p].client_sdat_packets_rx = 0;
      players1[p].client_sdat_packets_skipped = 0;
      players1[p].moves_entered = 0;
      players1[p].moves_skipped = 0;

      players1[p].client_game_move_sync = 0;
      players1[p].client_game_move_sync_min = 99;
      players1[p].client_game_move_sync_err = 0;

      players1[p].server_game_move_sync = 0;
      players1[p].server_game_move_sync_min = 99;
      players1[p].server_game_move_sync_err = 0;


      players1[p].serr_c_sync_err = 0;
      players1[p].serr_display_timer = 0;
      players1[p].server_sync = 99;
      players1[p].client_sync = 99;
      players1[p].quit_frame = 0;
      players1[p].quit_reason = 0;

      players1[p].join_stdf_sent = 0;


      players1[p].frames_skipped = 0;

      players1[p].tx_current_bytes_for_this_frame = 0;
      players1[p].tx_current_packets_for_this_frame = 0;
      players1[p].tx_total_bytes = 0;
      players1[p].tx_total_packets = 0;
      players1[p].tx_max_bytes_per_frame = 0;
      players1[p].tx_max_packets_per_frame = 0;


      players1[p].tx_packets_per_tally = 0;
      players1[p].tx_bytes_per_tally = 0;
      players1[p].tx_max_packets_per_tally = 0;
      players1[p].tx_max_bytes_per_tally = 0;


      players1[p].rx_current_bytes_for_this_frame = 0;
      players1[p].rx_current_packets_for_this_frame = 0;
      players1[p].rx_total_bytes = 0;
      players1[p].rx_total_packets = 0;
      players1[p].rx_max_bytes_per_frame = 0;
      players1[p].rx_max_packets_per_frame = 0;

      players1[p].rx_packets_per_tally = 0;
      players1[p].rx_bytes_per_tally = 0;
      players1[p].rx_max_packets_per_tally = 0;
      players1[p].rx_max_bytes_per_tally = 0;

      players1[p].num_dif_packets =0 ;
      players1[p].server_last_sdak_rx_frame_num = 0;
      players1[p].client_last_sdat_rx_frame_num = 0;

      players1[p].stdf_rx = 0;
      players1[p].stdf_late = 0;
      players1[p].stdf_on_time = 0;
      players1[p].dif_corr = 0;
      players1[p].made_active_holdoff = 0;


   }
   if (t == 2) // level done
   {
      players1[p].game_move_entry_pos = 0; // server only  ( for client game_move data sync )
      players1[p].server_last_sdat_sent_frame_num = 0; // only server uses it, to keep track of when last sdat was sent to client
   }



   if (t == 3) // netplay join
   {
      players1[p].server_last_sdak_rx_frame_num = frame_num + 200;
   }















}









































