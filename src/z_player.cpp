// zplayer.cpp

#include "pm.h"



void set_player_start_pos(int p)
{
   // count number of starts
   int ns = 0;

   int s[8] = {0};

   for (int i=0; i<500; i++)
      if (item[i][0] == 5)
      {
         ns++;
         s[item[i][7]] = i; // save index of this start
      }

   // start to use for this player
   int stu = p % ns;

   // item index of start to use
   int i = s[stu];

   players[p].PX = itemf[i][0];
   players[p].PY = itemf[i][1];




//   for (int c=0; c<500; c++)  // get start and time
//      if (item[c][0] == 5)
//      {
//         players[p].PX = itemf[c][0];
//         players[p].PY = itemf[c][1];
//      }
}

void proc_player_health(int p)
{
   if (players[p].old_LIFE != players[p].LIFE)
   {
      players1[p].last_health_adjust = al_fixtoi(players[p].LIFE - players[p].old_LIFE);
      players[p].old_LIFE = players[p].LIFE;
      players1[p].health_display = 80;
   }
   if (players[p].LIFE>al_itofix(100))
   {
       players[p].LIFE = al_itofix(100);     // enforce max
       players[p].old_LIFE = al_itofix(100); // to prevent display
   }
   if (players[p].LIFE < al_itofix(1)) // is LIFE < 1
   {
      players[p].LIFE = al_itofix(0);

      sprintf(msg,"PLAYER:%d DIED!", p);
      if (L_LOGGING_NETPLAY) add_log_entry_header(10, 0, msg, 1);

      game_event(90, 0, 0, p, 0, 0, 0);  // player death

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


void proc_player_xy_move_test(int p)
{
   if (players[p].up) players[p].PY -= al_ftofix(1);
   if (players[p].down) players[p].PY += al_ftofix(1);
   if (players[p].left)
   {
      players[p].left_right = 0;
      players[p].PX -= al_ftofix(1);
   }
   if (players[p].right)
   {
      players[p].left_right = 1;
      players[p].PX += al_ftofix(1);
   }
}

void proc_player_xy_move(int p)
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

   int x = al_fixtoi(players[p].PX);
   int y = al_fixtoi(players[p].PY);


// -----------   x move  ---------------------

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
            game_event(54, x, y, p, 0, 0, 0);
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
            game_event(54, x, y, p, 0, 0, 0);
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
         game_event(54, x, y, p, 0, 0, 0);
      }

      // check for collision with lift above
      if (is_up_solid(x, y+1, 1) > 31)
      {
         players[p].player_ride = 0;     // player knocked off lift due to collision above
         players[p].LIFE -= al_itofix(1);   // take some damage
         game_event(54, x, y, p, 0, 0, 0);
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
               game_event(54, x, y, p, 0, 0, 0);
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

}




void proc_player_paused(int p)
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
         players[p].paused = 0;
         players[p].old_LIFE = al_itofix(100);
         players[p].LIFE = al_itofix(100);
         players[p].left_xinc = al_itofix(0);
         players[p].right_xinc = al_itofix(0);
         players[p].xinc = al_itofix(0);
         players[p].yinc = al_itofix(0);
         set_player_start_pos(p); // get starting position from start block
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


            // snap to dest...
            if (item[x][8] == 0) // regular dest
            {
               players[p].PX  = al_itofix(item[x][6] * 20);
               players[p].PY  = al_itofix(item[x][7] * 20);
            }
            if (item[x][8] == 1) // linked item dest
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
      proc_player_carry(p);
   } // end of door move
}  // end of if player paused


void reset_player_scale_and_rot(int p)
{
   players[p].draw_scale = al_itofix(1);
   players[p].draw_rot = al_itofix(0);
}


int riding_rocket(int p)
{
   if ((players[p].carry_item) && (item[players[p].carry_item-1][0] == 98)) return 1;
   return 0;
}

void proc_player_stuck_in_blocks(int p)
{
   int x = al_fixtoi(players[p].PX);
   int y = al_fixtoi(players[p].PY);
   int su =    is_up_solid(x, y, 0);
   int sd =  is_down_solid(x, y, 0);
   int sl =  is_left_solid(x, y, 0);
   int sr = is_right_solid(x, y, 0);
   if ((su == 1) && (sd) && (sl) && (sr))
   {
      players[p].LIFE -= al_itofix(1);
      game_event(56, x, y, p, 0, 0, 0);
   }
}

void proc_player_riding_rocket(int p)
{
   int c = players[p].carry_item-1;
   int rot_inc = item[c][6];
   if (players[p].left)  item[c][10]-=rot_inc;
   if (players[p].right) item[c][10]+=rot_inc;
   players[p].xinc = players[p].yinc = 0;
   players[p].left_xinc = players[p].right_xinc = 0;
   players[p].xinc = players[p].yinc = 0;
   players[p].PX = itemf[c][0];
   players[p].PY = itemf[c][1];

   players[p].draw_rot = al_itofix(item[c][10]/10);
   players[p].draw_scale = al_ftofix(.5);
}

void proc_player_bounds_check(int p)
{
   // absolute level bounds check
   if (players[p].PX < al_itofix(0))    players[p].PX = al_itofix(0);
   if (players[p].PY < al_itofix(0))    players[p].PY = al_itofix(0);
   if (players[p].PX > al_itofix(1980)) players[p].PX = al_itofix(1980);
   if (players[p].PY > al_itofix(1980)) players[p].PY = al_itofix(1980);
}

void proc_player_collisions(int p)
{
   al_fixed f10 = al_itofix(10);
   al_fixed f16 = al_itofix(16);

   // items
   players1[p].potential_bomb_damage = 0;
   players[p].marked_door = -1; // so player can touch only one door
   for (int x=0; x<500; x++)
      if (item[x][0])
      {
         al_fixed ix1 = itemf[x][0] - f16;
         al_fixed ix2 = itemf[x][0] + f16;
         al_fixed iy1 = itemf[x][1] - f16;
         al_fixed iy2 = itemf[x][1] + f16;
         if ((players[p].PX > ix1) && (players[p].PX < ix2)
          && (players[p].PY > iy1) && (players[p].PY < iy2)
          && (!players[p].paused) ) proc_item_collision(p, x);
      }

   // enemies
   for (int e=0; e<100; e++)
      if ((Ei[e][0]) && (Ei[e][0] != 99)) // if active and not deathcount
      {
         al_fixed b = al_itofix(Ei[e][29]); // collision box size
         al_fixed ex1 = Efi[e][0] - b;
         al_fixed ex2 = Efi[e][0] + b;
         al_fixed ey1 = Efi[e][1] - b;
         al_fixed ey2 = Efi[e][1] + b;
         if ((players[p].PX > ex1) && (players[p].PX < ex2)
          && (players[p].PY > ey1) && (players[p].PY < ey2)) Ei[e][22] = p+1;
      }

   // ebullets
   for (int b=0; b<50; b++)
      if (e_bullet_active[b])  // if active
      {
         // new collision box is based on bullet speed and has both x and y component
         al_fixed ax = abs(e_bullet_fxinc[b]);
         al_fixed ay = abs(e_bullet_fyinc[b]);

         // enforce minimums
         if (ax < al_itofix(4)) ax = al_itofix(4);
         if (ay < al_itofix(4)) ay = al_itofix(4);

         al_fixed bx1 = e_bullet_fx[b] - ax;
         al_fixed bx2 = e_bullet_fx[b] + ax;
         al_fixed by1 = e_bullet_fy[b] - ay;
         al_fixed by2 = e_bullet_fy[b] + ay;
         if ((players[p].PX > bx1) && (players[p].PX < bx2)
          && (players[p].PY > by1) && (players[p].PY < by2)) proc_ebullet_collision(p, b);
      }

   // pbullets
   for (int b=0; b<50; b++)
      if (pbullet[b][0])  // if active
      {
         al_fixed bx1 = al_itofix(pbullet[b][2]) - f10;
         al_fixed bx2 = al_itofix(pbullet[b][2]) + f10;
         al_fixed by1 = al_itofix(pbullet[b][3]) - f10;
         al_fixed by2 = al_itofix(pbullet[b][3]) + f10;

         if ((players[p].PX > bx1) && (players[p].PX < bx2)
          && (players[p].PY > by1) && (players[p].PY < by2))
         {
            int pb = pbullet[b][1]; // the player that fired this bullet
            if ((deathmatch_pbullets) && (pb != p))
            {
                proc_pbullet_collision(p, b);
                game_event(40, 0, 0, p, pb, 0, deathmatch_pbullets_damage);
            }
            if ((suicide_pbullets) && (pb == p))
            {
                proc_pbullet_collision(p, b);
                game_event(41, 0, 0, p, pb, 0, deathmatch_pbullets_damage);
            }
         }
      }
}

void move_players(void)
{
   for (int p=0; p<NUM_PLAYERS; p++)
      if (players[p].active)
      {
         if (players[p].paused) proc_player_paused(p);
         else // not paused
         {
            reset_player_scale_and_rot(p);

            if (riding_rocket(p)) proc_player_riding_rocket(p);
            else // not riding rocket
            {
               //proc_player_xy_move_test(p);
               proc_player_xy_move(p);
               proc_pbullet_shoot(p);
            }
            // common to both riding rocket and not
            proc_player_carry(p);
            proc_player_collisions(p);
            proc_player_health(p);
            proc_player_bounds_check(p);
            proc_player_stuck_in_blocks(p);
         }
      }
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

      al_draw_scaled_rotated_bitmap(player_tile[players[p].color][players[p].shape], 10, 10, AX+10, AY+10, scale, scale, rot, flags);

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

         // show current health bar
         int ch = al_fixtoi(players[p].LIFE); // current health
         draw_percent_bar(AX + 10, AY - 6, 16, 3, ch);

         // show last health adjustment
         int h = players1[p].last_health_adjust; // last health adjust
         if (h > 0) al_draw_textf(f3, palette_color[11], AX+10, AY-16, ALLEGRO_ALIGN_CENTER, "%+d", h);
         if (h < 0) al_draw_textf(f3, palette_color[10], AX+10, AY-16, ALLEGRO_ALIGN_CENTER, "%+d", h);

         // show potential bomb damage
         int dmg = players1[p].potential_bomb_damage; // potential bomb damage
         if (dmg)
         {
            int nh = ch - dmg; // new health
            if (nh < 0) nh = 0;
            if (frame_num % 20 < 10)
            {
               // draw segment from new health to current health
               draw_percent_bar_range(AX+10, AY-6, 16, 3, 10, nh, ch);

               // show damage amount
               al_draw_textf(f3, palette_color[10], AX+10, AY-16, ALLEGRO_ALIGN_CENTER, "%+d", -dmg);

               // draw a tiny bomb picture
               al_draw_scaled_rotated_bitmap(tile[464], 10, 10,  AX+20, AY-12, .5, .5, 0, 0);
            }
         }
      }
   }
}

void draw_players(void)
{
   for (int p=0; p<NUM_PLAYERS; p++)
      draw_player(p);

   // do this so that that local player is always drawn on top
   draw_player(active_local_player);
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

   // paused can mean dead or it can also mean in door travel
   // if paused use static shape
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
   if (t == 1) // new game
   {
      players[p].active = 0;
      players[p].paused = 0;
      players[p].control_method = 0;

      players[p].old_LIFE = al_itofix(100);
      players[p].LIFE = al_itofix(100);

      players[p].LIVES = 5;
      players[p].num_hits = 0;

      players1[p].hostname[0] = 0;
      players1[p].who = 99;
      players1[p].fake_keypress_mode = 0;

      players1[p].up_key =    players1[0].up_key;
      players1[p].down_key =  players1[0].down_key;
      players1[p].left_key =  players1[0].left_key;
      players1[p].right_key = players1[0].right_key;
      players1[p].jump_key =  players1[0].jump_key;
      players1[p].fire_key =  players1[0].fire_key;
      players1[p].menu_key =  players1[0].menu_key;

      init_player(p, 17); // clear player common
      init_player(p, 21); // clear netgame counters, etc
      init_player(p, 23); // clear bandwidth counters
   }

   if (t == 2) // new level
   {
      init_player(p, 17); // clear player common
      init_player(p, 21); // clear netgame counters, etc
      init_player(p, 23); // clear bandwidth counters
   }

   if (t == 17) // player common
   {
      players[p].carry_item = 0;
      players[p].player_ride = 0;
      players[p].left_right = 0;
      players[p].left = 0;
      players[p].right = 0;
      players[p].up = 0;
      players[p].down = 0;
      players[p].jump = 0;
      players[p].fire = 0;

      players[p].xinc = al_itofix(0);
      players[p].yinc = al_itofix(0);
      players[p].left_xinc = al_itofix(0);
      players[p].right_xinc = al_itofix(0);


      players[p].bullet_wait_counter=0;
      players[p].request_bullet = 0;
      players[p].bullet_wait = 4;
      players[p].bullet_speed = 12;

      players[p].draw_rot = al_itofix(0);
      players[p].draw_scale = al_itofix(1);
      players[p].shape = 0;;

      players1[p].comp_move = 0;
      players1[p].old_comp_move = 0;

      players1[p].health_display = 0;
      players1[p].last_health_adjust = 0;
      players1[p].potential_bomb_damage = 0;

      players1[p].frames_skipped = 0;
   }

   if (t == 21) // netgame counters, etc
   {
      players1[p].client_cdat_packets_tx = 0;
      players1[p].client_sdat_packets_rx = 0;
      players1[p].client_sdat_packets_skipped = 0;
      players1[p].moves_entered = 0;
      players1[p].moves_skipped = 0;

      players1[p].moves_skipped_tally = 0;
      players1[p].moves_skipped_last_tally = 0;

      players1[p].server_sdat_sync_freq = 0;

      players1[p].game_move_entry_pos = 0;
      players1[p].client_sync = 0;
      players1[p].server_sync = 0;
      players1[p].serr_c_sync_err = 0;
      players1[p].serr_display_timer = 0;
      players1[p].join_frame = 0;
      players1[p].server_last_sdat_sent_frame_num = 0;
      players1[p].server_last_sdat_sent_start = 0;
      players1[p].server_last_sdat_sent_num = 0;

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

      players1[p].num_dif_packets =0 ;
      players1[p].server_last_sdak_rx_frame_num = 0;
      players1[p].client_last_sdat_rx_frame_num = 0;

      players1[p].stdf_rx = 0;
      players1[p].stdf_late = 0;
      players1[p].stdf_on_time = 0;
      players1[p].dif_corr = 0;

      players1[p].cmp_dif_size = 0;
      players1[p].n_stdf = 0;
      players1[p].made_active_holdoff = 0;
   }

   if (t == 23) // clear bandwidth counters
   {
      // used to add up until frame end
      players1[p].tx_current_bytes_for_this_frame = 0;
      players1[p].tx_current_packets_for_this_frame = 0;
      players1[p].tx_max_bytes_per_frame = 0;
      players1[p].tx_max_packets_per_frame = 0;
      players1[p].rx_current_bytes_for_this_frame = 0;
      players1[p].rx_current_packets_for_this_frame = 0;
      players1[p].rx_max_bytes_per_frame = 0;
      players1[p].rx_max_packets_per_frame = 0;

      // used to add up until tally end
      players1[p].tx_packets_tally = 0;
      players1[p].tx_bytes_tally = 0;
      players1[p].tx_max_packets_per_tally = 0;
      players1[p].tx_max_bytes_per_tally = 0;
      players1[p].rx_packets_tally = 0;
      players1[p].rx_bytes_tally = 0;
      players1[p].rx_max_packets_per_tally = 0;
      players1[p].rx_max_bytes_per_tally = 0;

      // used for display
      players1[p].tx_packets_per_tally = 0;
      players1[p].tx_bytes_per_tally = 0;
      players1[p].rx_packets_per_tally = 0;
      players1[p].rx_bytes_per_tally = 0;

      // grand totals
      players1[p].tx_total_bytes = 0;
      players1[p].tx_total_packets = 0;
      players1[p].rx_total_bytes = 0;
      players1[p].rx_total_packets = 0;
   }
}









































