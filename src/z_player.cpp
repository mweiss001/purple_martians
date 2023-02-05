// z_player.cpp

#include "pm.h"
#include "z_log.h"
#include "z_player.h"
#include "n_netgame.h"
#include "mwTally.h"

struct player players[NUM_PLAYERS];
struct player1 players1[NUM_PLAYERS];
int active_local_player = 0;


void set_player_start_pos(int p, int cont)
{
   int found = 0;
   if (warp_level_location)
   {
      for (int i=0; i<500; i++)
         if (item[i][0] == 12)
         {
            if (item[i][8] == warp_level_location)
            {
               found = 1;
               //printf("Found warp level location %d\n", warp_level_location);
               players[p].PX = itemf[i][0];
               players[p].PY = itemf[i][1] + al_itofix(20);
            }
         }
      warp_level_location = 0;
   }

   if (found == 0)
   {

      int ns = 0; // count number of starts
      int s[8] = {0};
      for (int i=0; i<500; i++)
         if (item[i][0] == 5)
         {
            ns++;
            s[item[i][7]] = i; // save index of this start
         }
      if (ns == 0)
      {
         printf("Error: no start found.\n");
         players[p].PX = al_itofix(20);
         players[p].PY = al_itofix(20);;
      }
      if (ns == 1)
      {
         players[p].spawn_point_index = 0;
         int ps = s[players[p].spawn_point_index];
         players[p].PX = itemf[ps][0];
         players[p].PY = itemf[ps][1];
      }



      if (ns > 1)
      {
         int mode = item[s[0]][6];

         if (mode == 0)
         {
            printf("Error: in start mode:0 there should be only one start.. all other starts are ignored.\n");
            players[p].spawn_point_index = 0;
            players[p].PX = itemf[s[0]][0];
            players[p].PY = itemf[s[0]][1];

         }

         if (mode == 1) // team start
         {
            if (p % 2) // odd
            {
               players[p].spawn_point_index = 1;
               players[p].PX = itemf[s[1]][0];
               players[p].PY = itemf[s[1]][1];
            }
            else
            {
               players[p].spawn_point_index = 0;
               players[p].PX = itemf[s[0]][0];
               players[p].PY = itemf[s[0]][1];
            }
         }

         if ((mode == 2) || (mode == 3)) // check point common and individual
         {
            if (!cont) players[p].spawn_point_index = 0; // initial
            int ps = s[players[p].spawn_point_index];
            players[p].PX = itemf[ps][0];
            players[p].PY = itemf[ps][1];
         }

      }

   }

   /*




   int mode =

   if (cont == 0) // players initial entry
   {
      players[p].spawn_point_index = 0;




   }


   if (cont == 1) // players re spawn
   {
   }

   for (int i=0; i<500; i++)
      if ((item[i][0] == 5) && (item[i][7] == players[p].spawn_point_index))
      {
         players[p].PX = itemf[i][0];
         players[p].PY = itemf[i][1];

      }




*/




/*

   // count number of starts
   int ns = 0;

   int s[8] = {0};

   for (int i=0; i<500; i++)
      if (item[i][0] == 5)
      {
         ns++;
         s[item[i][7]] = i; // save index of this start
      }

   if (ns == 0)
   {
      printf("Error: no start found.\n");
      players[p].PX = al_itofix(20);
      players[p].PY = al_itofix(20);;
   }
   else if (ns == 1)
   {



   }
   else
   {
      // start to use for this player
      int stu = p % ns;

      // item index of start to use
      int i = s[stu];

      players[p].PX = itemf[i][0];
      players[p].PY = itemf[i][1];
   }

   */


}

void proc_player_health(int p)
{
   if ((frame_num) && (frame_num == players1[p].block_damage_holdoff)) game_event(58, 0, 0, p, 0, 0, 0);

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
      if (LOG_NET) add_log_entry_header(10, 0, msg, 1);

      game_event(90, 0, 0, p, 0, 0, 0);  // player death

      players[p].stat_respawns++;

      set_player_joint_quit_display(p, 3, 60);

      players1[p].health_display = 200;
      players[p].paused = 100;
      players[p].paused_type = 1;

   }
}


void proc_player_xy_move_test(int p)
{
   al_fixed m = al_ftofix(0.5);

   if (key[ALLEGRO_KEY_LCTRL][0]) m *= 4;

   if (players[p].up) players[p].PY -= m;
   if (players[p].down) players[p].PY += m;
   if (players[p].left)
   {
      players[p].left_right = 0;
      players[p].PX -= m;
   }
   if (players[p].right)
   {
      players[p].left_right = 1;
      players[p].PX += m;
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

   if (is_right_solid(x, y, 0, 1)) players[p].right_xinc = z;
   if (is_left_solid(x, y, 0, 1))  players[p].left_xinc = z;

   players[p].xinc = players[p].left_xinc + players[p].right_xinc;  // calc xinc
   players[p].PX += players[p].xinc;                                // apply xinc


   x = al_fixtoi(players[p].PX);
   y = al_fixtoi(players[p].PY);
   if (players[p].player_ride) // if player is riding lift
   {
      int d = players[p].player_ride - 32; // lift number
      // moving right

      if ((lifts[d].fxinc > z) && (!is_right_solid(x, y, 1, 1))) players[p].PX  += lifts[d].fxinc;

      // moving left
      if ((lifts[d].fxinc < z) && (!is_left_solid(x, y, 1, 1))) players[p].PX  += lifts[d].fxinc;
   }




   if (int a = is_left_solid(x, y, 1, 1))
   {
      if (a > 31) // player being pushed right by lift
      {
         if (!is_right_solid(x, y, 0, 1))
         {
            int l = a-32; // lift that is pushing
            int width = lifts[l].width + 1;
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
   if (int a = is_right_solid(x, y, 1, 1))
   {
      if (a > 31)    // player being pushed left by lift
      {
         if (!is_left_solid(x, y, 0, 1))
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




         int a = is_down_solid(x, y, 1, 1);                                // check for lift below player




         if (a > 31)
         {
            if (lifts[a-32].fyinc > z)                                  // is lift going down
            {
               if (is_down_solid(x, y, 0, 1)) players[p].player_ride = 0;  // check for block below ignoring lifts
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
      if ((lifts[d].fyinc < z) && (is_up_solid(x, y, 0, 1) == 1))
      {
         players[p].player_ride = 0;        // player knocked off lift due to collision above
         players[p].LIFE -= al_itofix(1);   // take some damage
         game_event(54, x, y, p, 0, 0, 0);
      }

      // check for collision with lift above
      if (is_up_solid(x, y+1, 1, 1) > 31)
      {
         players[p].player_ride = 0;        // player knocked off lift due to collision above
         players[p].LIFE -= al_itofix(1);   // take some damage
         game_event(54, x, y, p, 0, 0, 0);
      }

      if (players[p].player_ride)                       // if still riding
         players[p].PY  = lifts[d].fy - al_itofix(20);  // align with fy

      // moving down
      if (lifts[d].fyinc > z)
      {
         if (is_down_solid(x, y, 0, 1))                          // no lift check
         {
            players[p].player_ride = 0;                          // ride over
            players[p].PY = al_itofix(y - (y % 20));             // align with floor
         }
         else players[p].PY = lifts[d].fy - al_itofix(20);       // align with fy
      }


      if (players[p].jump)                                    // if jump pressed
      {
         players[p].player_ride = 0;                          // ride over
         x = al_fixtoi(players[p].PX);
         y = al_fixtoi(players[p].PY);
         if (!is_up_solid(x, y, 1, 1))                        // only jump if nothing above
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
         if (is_up_solid(x, y+2, 1, 1))                       // look for collision above
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
         if (is_down_solid(x, y, 0, 1))                       // check for floor below (no lift)
         {
            players[p].yinc = z;                              // kill downwards motion
            players[p].PY = al_itofix(y - (y % 20));          // align with floor

            // check for collision with lift above if lift is moving down
            int a = is_up_solid(x, y, 1, 1);
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
               int a = is_up_solid(x, y, 1, 1);
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
      if (players[p].paused == 100) // do only once per death
      {
         // does this player have any bomb remotes?
         for (int i=0; i<500; i++)
            if ((item[i][0] == 99) && (item[i][6] == 3)) // lit bomb with remote detonator
            {
               // change back to regular bomb
               item[i][0] = 8;

               // set bomb to explode!
               //item[i][6] = 2; // mode 2; explosion
               //item[i][8] = item[i][9] = 20; // explosion timer
            }
      }

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
         set_player_start_pos(p, 1); // get starting position from start block
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
            if (item[x][13] == 448) item[x][1] = 448; // restore door shape
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


int is_player_riding_rocket(int p)
{
   if ((players[p].carry_item) && (item[players[p].carry_item-1][0] == 98)) return 1;
   return 0;
}

void proc_player_stuck_in_blocks(int p)
{
   int x = al_fixtoi(players[p].PX);
   int y = al_fixtoi(players[p].PY);
   int su =    is_up_solid(x, y, 0, 1);
   int sd =  is_down_solid(x, y, 0, 1);
   int sl =  is_left_solid(x, y, 0, 1);
   int sr = is_right_solid(x, y, 0, 1);
   if ((su) && (sd) && (sl) && (sr))
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

   players[p].left_xinc = players[p].right_xinc = 0;

   players[p].PX = itemf[c][0];
   players[p].PY = itemf[c][1];

   // players[p].xinc = players[p].yinc = 0;
   // would it hurt here to set players xinx, yinc the same as the item?
   // i want to be able to use xinx, yinc for bullet targetting
   players[p].xinc = itemf[c][2];
   players[p].yinc = itemf[c][3];

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
   {
      if ((item[x][0]) && (item[x][0] != 9) && (item[x][0] != 16) && (item[x][0] != 17))
      {
         al_fixed ix1 = itemf[x][0] - f16;
         al_fixed ix2 = itemf[x][0] + f16;
         al_fixed iy1 = itemf[x][1] - f16;
         al_fixed iy2 = itemf[x][1] + f16;
         if ((players[p].PX > ix1) && (players[p].PX < ix2)
          && (players[p].PY > iy1) && (players[p].PY < iy2)
          && (!players[p].paused) ) proc_item_collision(p, x);
      }
   }
   // enemies
   for (int e=0; e<100; e++)
   {
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
   }
   // ebullets
   for (int b=0; b<50; b++)
      if (ebullets[b].active)  // if active
      {
         // new collision box is based on bullet speed and has both x and y component
         al_fixed ax = abs(ebullets[b].fxinc);
         al_fixed ay = abs(ebullets[b].fyinc);

         // enforce minimums
         if (ax < al_itofix(4)) ax = al_itofix(4);
         if (ay < al_itofix(4)) ay = al_itofix(4);

         al_fixed bx1 = ebullets[b].fx - ax;
         al_fixed bx2 = ebullets[b].fx + ax;
         al_fixed by1 = ebullets[b].fy - ay;
         al_fixed by2 = ebullets[b].fy + ay;
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


int is_player_within_ladder_reach(int p)
{
   // detect if player is on ladder block
   int AX = al_fixtoi(players[p].PX);
   int AY = al_fixtoi(players[p].PY);

   int bx2 = 0, bx1 = AX / 20;
   int am = AX % 20;
   if (am < 9) // this block only
   {
      bx2 = bx1;
   }
   if (am > 10) // next block only
   {
      bx1++;
      bx2 = bx1;
   }
   if ((am > 8) && (am < 11)) // both blocks
   {
      bx2 = bx1 + 1;
   }

   int by2 = 0, by1 = AY / 20;
   am = AY % 20;
   if (am < 9) // this block only
   {
      by2 = by1;
   }
   if (am > 10) // next block only
   {
      by1++;
      by2 = by1;
   }
   if ((am > 8) && (am < 11)) // both blocks
   {
      by2 = by1 + 1;
   }
   for (int x=bx1; x<=bx2; x++)
      for (int y=by1; y<=by2; y++)
      {
         if (l[x][y] & PM_BTILE_LADDER_MOVE) return 1;
      }
   return 0;
}

int is_player_within_rope_reach(int p)
{
   // detect if player is on rope block
   int AX = al_fixtoi(players[p].PX);
   int AY = al_fixtoi(players[p].PY);

   int bx2 = 0, bx1 = AX / 20;
   int am = AX % 20;
   if (am < 9) // this block only
   {
      bx2 = bx1;
   }
   if (am > 10) // next block only
   {
      bx1++;
      bx2 = bx1;
   }
   if ((am > 8) && (am < 11)) // both blocks
   {
      bx2 = bx1 + 1;
   }


   int by = AY / 20;
   am = AY % 20;

   //al_draw_textf(font, palette_color[15], AX, AY-20, 0, "%d", am );
   int good_height = 0;

   if (am < 5) // this block only
   {
      good_height = 1;
   }
   if (am > 15) // next block only
   {
      good_height = 1;
      by++;
   }

   if (good_height)
   for (int x=bx1; x<=bx2; x++)
   {
      if (l[x][by] & PM_BTILE_ROPE_MOVE) return 1;
   }
   return 0;
}


void proc_player_rope_move(int p)
{
    // reset all regular incs
   al_fixed f0 = al_itofix(0);
   players[p].xinc = f0;
   players[p].yinc = f0;
   players[p].left_xinc = f0;
   players[p].right_xinc = f0;

   al_fixed m = al_ftofix(3);

   if (players[p].left)
   {
      players[p].left_right = 0;
      players[p].PX -= is_left_solidfm(players[p].PX, players[p].PY, m, 0);
   }
   if (players[p].right)
   {
      players[p].left_right = 1;
      players[p].PX += is_right_solidfm(players[p].PX, players[p].PY, m, 0);
   }
   if (players[p].down)
   {
      players[p].on_rope = 0;
      players[p].PY += al_itofix(4);
   }
}


void proc_player_ladder_move(int p)
{
   int old_px = al_fixtoi(players[p].PX);
   int old_py = al_fixtoi(players[p].PY);

   al_fixed m = al_ftofix(3);

   // reset all regular incs
   al_fixed f0 = al_itofix(0);
   players[p].xinc = f0;
   players[p].yinc = f0;
   players[p].left_xinc = f0;
   players[p].right_xinc = f0;

   if (players[p].jump)
   {
      players[p].on_ladder = 0;
      players[p].yinc = al_ftofix(-6.6); // initial_jump_velocity;
      players[p].PY += players[p].yinc;  // make the first move here
      return;
   }
   else
   {
      if (players[p].up)   players[p].PY -=   is_up_solidfm(players[p].PX, players[p].PY, m, 0);
   }

   if (players[p].down) players[p].PY += is_down_solidfm(players[p].PX, players[p].PY, m, 0);
   if (players[p].left)
   {
      players[p].left_right = 0;
      players[p].PX -= is_left_solidfm(players[p].PX, players[p].PY, m, 0);
   }
   if (players[p].right)
   {
      players[p].left_right = 1;
      players[p].PX += is_right_solidfm(players[p].PX, players[p].PY, m, 0);
   }


   // how much did we move this last frame?
   int px = al_fixtoi(players[p].PX);
   int py = al_fixtoi(players[p].PY);
   int xd = px - old_px;
   int yd = py - old_py;

   // did we try to move up past top of ladder?
   if (yd < 0) if (!is_player_within_ladder_reach(p)) players[p].PY +=m;


   // did we pass by any single block openings??

   // if only x move
   if ((xd != 0) && (yd == 0))
   {
      if (xd > 0) // x+
      {
         for (int tx = old_px; tx <= px; tx++)
         {
            if (players[p].up)
            {
               al_fixed tm = is_up_solidfm(al_itofix(tx), players[p].PY, m, 0);
               if (tm > al_itofix(0))
               {
                  //printf("%d px:%d opx:%d tx:%d x+u\n", frame_num, px, old_px, tx );
                  players[p].PX = al_itofix(tx); // set x to passed by pos
                  players[p].PY -= m; // move in y
                  tx = px+1; // break out of loop
               }
            }
            else if (players[p].down)
            {
               al_fixed tm = is_down_solidfm(al_itofix(tx), players[p].PY, m, 0);
               if (tm > al_itofix(0))
               {
                  //printf("%d px:%d opx:%d tx:%d x+d\n", frame_num, px, old_px, tx );
                  players[p].PX = al_itofix(tx); // set x to passed by pos
                  players[p].PY += m; // move in y
                  tx = px+1; // break out of loop
               }
            }
         }
      }
      if (xd < 0) // x-
      {
         for (int tx = px; tx <= old_px; tx++)
         {
            if (players[p].up)
            {
               al_fixed tm = is_up_solidfm(al_itofix(tx), players[p].PY, m, 0);
               if (tm > al_itofix(0))
               {
                  //printf("%d px:%d opx:%d tx:%d x-u\n", frame_num, px, old_px, tx );
                  players[p].PX = al_itofix(tx); // set x to passed by pos
                  players[p].PY -= m; // move in y
                  tx = old_px+1; // break out of loop
               }
            }
            else if (players[p].down)
            {
               al_fixed tm = is_down_solidfm(al_itofix(tx), players[p].PY, m, 0);
               if (tm > al_itofix(0))
               {
                  //printf("%d px:%d opx:%d tx:%d x-d\n", frame_num, px, old_px, tx );
                  players[p].PX = al_itofix(tx); // set x to passed by pos
                  players[p].PY += m; // move in y
                  tx = old_px+1; // break out of loop
               }
            }
         }
      }
   }


   // if only y move
   if ((yd != 0) && (xd == 0))
   {
      if (yd > 0) // y+
      {
         for (int ty1 = old_py; ty1 <= py; ty1++)
         {
            if (players[p].right)
            {
               al_fixed tm = is_right_solidfm(players[p].PX, al_itofix(ty1), m, 0);
               if (tm > al_itofix(0))
               {
                  //printf("%d py:%d opy:%d ty1:%d y+r\n", frame_num, py, old_py, ty1 );
                  players[p].PY = al_itofix(ty1); // set y to passed by pos
                  players[p].PX += m; // move in x
                  ty1 = py+1; // break out of loop
               }
            }
            else if (players[p].left)
            {
               al_fixed tm = is_left_solidfm(players[p].PX, al_itofix(ty1),  m, 0);
               if (tm > al_itofix(0))
               {
                  //printf("%d py:%d opy:%d ty1:%d y+l\n", frame_num, py, old_py, ty1 );
                  players[p].PY = al_itofix(ty1); // set y to passed by pos
                  players[p].PX -= m; // move in x
                  ty1 = py+1; // break out of loop
               }
            }
         }
      }
      if (yd < 0) // y-
      {
         for (int ty1 = py; ty1 <= old_py; ty1++)
         {
            if (players[p].right)
            {
               al_fixed tm = is_right_solidfm(players[p].PX, al_itofix(ty1), m, 0);
               if (tm > al_itofix(0))
               {
                  //printf("%d py:%d opy:%d ty1:%d y-r\n", frame_num, py, old_py, ty1 );
                  players[p].PY = al_itofix(ty1); // set y to passed by pos
                  players[p].PX += m; // move in x
                  ty1 = old_py+1; // break out of loop
               }
            }
            else if (players[p].left)
            {
               al_fixed tm = is_left_solidfm(players[p].PX, al_itofix(ty1),  m, 0);
               if (tm > al_itofix(0))
               {
                  //printf("%d py:%d opy:%d ty1:%d y-l\n", frame_num, py, old_py, ty1 );
                  players[p].PY = al_itofix(ty1); // set y to passed by pos
                  players[p].PX -= m; // move in x
                  ty1 = old_py+1; // break out of loop
               }
            }
         }
      }
   }
}

void proc_player_rope(int p)
{
   if (players[p].on_rope)                 // if player is currently on rope
   {
      if (!is_player_within_rope_reach(p)) // check to see if player is still on rope
      {
         players[p].on_rope = 0;
         players[p].on_ladder = is_player_within_ladder_reach(p); // automatically grab ladder if with reach and just leaving rope
      }
   }
   else
   {
      if ( (is_player_within_rope_reach(p)) &&                   // not already on rope and within rope reach
         ((players[p].yinc > al_itofix(0)) || (players[p].up)) ) // player is falling or up is pressed
      {
         // plater just got on rope
         players[p].on_rope = 1;
         players[p].on_ladder = 0;

         // snap y to rope
         int AY = al_fixtoi(players[p].PY);
         int by = AY / 20;
         int am = AY % 20;

         if (am <  5) players[p].PY = al_itofix((by+0) * 20); // this block only
         if (am > 15) players[p].PY = al_itofix((by+1) * 20); // next block only

         // check to see if player is embedded in wall to right
         al_fixed f1 = al_itofix(1);
         int done = 0;
         while (!done)
         {
            al_fixed m = is_right_solidfm(players[p].PX-f1, players[p].PY, f1, 0);
            if (m < f1) players[p].PX -= f1;
            else done = 1;
         }
         // check to see if player is embedded in wall to left
         done = 0;
         while (!done)
         {
            al_fixed m = is_left_solidfm(players[p].PX+f1, players[p].PY, f1, 0);
            if (m < f1) players[p].PX += f1;
            else done = 1;
         }
      }
   }
   if (players[p].on_rope) proc_player_rope_move(p);
}

void proc_player_ladder(int p)
{
   if (players[p].on_ladder) // already on ladder (grabbing ladder)
   {
      // check to see if player is still with ladder reach
      players[p].on_ladder = is_player_within_ladder_reach(p);
   }
   else // not initially grabbing ladder when function started
   {
      if (is_player_within_ladder_reach(p) && (players[p].up) && (!players[p].jump)) // within ladder reach and up pressed AND jump not pressed
      {
         // just got on ladder
         players[p].on_ladder = 1;
         players[p].on_rope = 0;

         // check to see if player is embedded in wall to right
         al_fixed f1 = al_itofix(1); // the number 1 in fixed format
         int done = 0;
         while (!done)
         {
            al_fixed m = is_right_solidfm(players[p].PX-f1, players[p].PY, f1, 0);
            //printf("R %f %f\n", al_fixtof(m), al_fixtof(players[p].PX));
            if (m < f1) players[p].PX -= f1;
            else done = 1;
         }

         // check to see if player is embedded in wall to left
         done = 0;
         while (!done)
         {
            al_fixed m = is_left_solidfm(players[p].PX+f1, players[p].PY, f1, 0);
            //printf("L %f %f\n", al_fixtof(m), al_fixtof(players[p].PX));
            if (m < f1) players[p].PX += f1;
            else done = 1;
         }
      }
   }
   if (players[p].on_ladder) proc_player_ladder_move(p);
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

            if (is_player_riding_rocket(p)) proc_player_riding_rocket(p);
            else // not riding rocket
            {
               proc_player_rope(p);
               proc_player_ladder(p);
               if (!players[p].on_ladder && !players[p].on_rope) // not on ladder or rope
                //  proc_player_xy_move_test(p);
                  proc_player_xy_move(p);
            }

            // common to all not paused modes
            proc_pbullet_shoot(p);
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


   //   printf("color:%d shape:%d\n", players[p].color, players[p].shape );


      al_draw_scaled_rotated_bitmap(player_tile[players[p].color][players[p].shape], 10, 10, AX+10, AY+10, scale, scale, rot, flags);











      /*


      al_draw_textf(font, palette_color[15], AX+10, AY-30, ALLEGRO_ALIGN_CENTER, "X:%d Y:%d", AX, AY);


      if (players[p].on_ladder)
         al_draw_rectangle(0.5+AX, 0.5+AY, 0.5+AX+19, 0.5+AY+19, palette_color[11], 1);

      if (players[p].on_rope)
         al_draw_rectangle(0.5+AX, 0.5+AY, 0.5+AX+17, 0.5+AY+19, palette_color[10], 1);


      // detect block that player is on...

      int bx2, bx1 = AX / 20;
      int am = AX % 20;
      if (am < 9) // this block only
      {
         bx2 = bx1;
      }
      if (am > 10) // next block only
      {
         bx1++;
         bx2 = bx1;
      }
      if ((am > 8) && (am < 11)) // both blocks
      {
         bx2 = bx1 + 1;
      }

      int by = AY / 20;
      am = AY % 20;

      al_draw_textf(font, palette_color[15], AX, AY-20, 0, "%d", am );

      int good_height = 0;

      if (am < 5) // this block only
      {
         good_height = 1;
      }
      if (am > 15) // next block only
      {
         good_height = 1;
         by++;
      }

      if (good_height)
         if (timer_draw_mode2) al_draw_textf(font, palette_color[col], x0+10, y0+6, ALLEGRO_ALIGN_CENTER, "%d", tts);
         if (timer_draw_mode2) al_draw_textf(font, palette_color[col], x0+10, y0+6, ALLEGRO_ALIGN_CENTER, "%d", tts);
         if (timer_draw_mode2) al_draw_textf(font, palette_color[col], x0+10, y0+6, ALLEGRO_ALIGN_CENTER, "%d", tts);
           if (timer_draw_mode2) al_draw_textf(font, palette_color[col], x0+10, y0+6, ALLEGRO_ALIGN_CENTER, "%d", tts);
    for (int x=bx1; x<=bx2; x++)
      {
         al_draw_rectangle(0.5+x*20, 0.5+by*20, 0.5+(x*20)+19, 0.5+(by*20)+19, palette_color[8], 1);
         if (l[x][by] == 19) al_draw_rectangle(0.5+x*20, 0.5+by*20, 0.5+(x*20)+19, 0.5+(by*20)+19, palette_color[11], 1);
      }

*/

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
   int pos = (al_fixtoi(players[p].PX)/6) % 6;  // 6 shapes in sequence

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

   // if jump or fall use static shape
   if (players[p].yinc != al_itofix(0)) players[p].shape = 19;


   if (players[p].on_rope)
   {
      // animate with x pos
      int x = al_fixtoi(players[p].PX);

      // 2 shapes in sequence
      // 3 is rope move
      int pos = (x/3) % 2;
      players[p].shape = 20 + pos;
      /// printf("f:%d x:%d pos:%d, shape:%d\n", frame_num, x, pos, shape );
   }

   if (players[p].on_ladder)
   {
      // animate with y pos
      int y = al_fixtoi(players[p].PY);

      // 2 shapes in sequence
      // 3 is ladder move
      int pos = (y/3) % 2;
      players[p].shape = 22 + pos;
      //printf("f:%d y:%d pos:%d, shape:%d\n", frame_num, y, pos, shape );
   }
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
      players[p].paused = 0;
      players[p].carry_item = 0;
      players[p].player_ride = 0;
      players[p].on_ladder = 0;
      players[p].on_rope = 0;
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
      players[p].shape = 0;

      players[p].stat_respawns = 0;
      players[p].stat_bullets_fired = 0;
      players[p].stat_enemy_hits = 0;
      players[p].stat_player_hits = 0;
      players[p].stat_self_hits = 0;
      players[p].stat_purple_coins = 0;

      players1[p].comp_move = 0;
      players1[p].old_comp_move = 0;

      players1[p].health_display = 0;
      players1[p].last_health_adjust = 0;
      players1[p].potential_bomb_damage = 0;

      players[p].level_done_mode = 0;
      players[p].level_done_timer = 0;
      players[p].level_done_ack = 0;
      players[p].level_done_x = 0;
      players[p].level_done_y = 0;
      players[p].level_done_player = 0;



   }

   if (t == 21) // netgame counters, etc
   {
      players1[p].client_cdat_packets_tx = 0;

      players1[p].late_cdats = 0;
      mwT[0].initialize(); // initialize tally

      players1[p].client_chase_fps = 0;
      players1[p].server_game_move_sync = 0;

      players1[p].quit_frame = 0;
      players1[p].quit_reason = 0;

      players1[p].join_frame = 0;
      players1[p].client_base_resets = 0;

      players1[p].num_dif_packets =0 ;
      players1[p].server_last_stak_rx_frame_num = 0;
      players1[p].client_last_stdf_rx_frame_num = 0;

      players1[p].server_state_freq = 2;

      players1[p].cmp_dif_size = 0;
      players1[p].made_active_holdoff = 0;

      players1[p].client_chase_offset = 0.0;
      players1[p].client_chase_offset_auto_offset = -0.01;
      players1[p].client_chase_offset_mode = 1;

      players1[p].xcor_reset_frame = 0;
      players1[p].xcor_max = 0;

      players1[p].ping = 0;
      players1[p].ping_avg = 0;
      mwT[4].initialize(); // initialize max tally

      mwT[3].initialize(); // initialize tally
      players1[p].game_move_dsync = 0;
      players1[p].game_move_dsync_avg_last_sec = 0;
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

void fill_player_tile(void)
{
   //printf("fill player bitmap\n");
   int a, b, x, y;

   if (0) // load from disk
   {
      ptilemap = al_load_bitmap("bitmaps/player_tiles.bmp");
      if (!ptilemap) m_err((char*)"Can't load tiles from bitmaps/player_tiles.bmp");
      else
      {
         //printf("load good\n");
         al_convert_mask_to_alpha(ptilemap, al_map_rgb(0, 0, 0)) ;

         al_set_target_bitmap(M_ptilemap);
         al_draw_bitmap(ptilemap, 0, 0, 0);

         // create sub bitmaps
         for (a=0; a<16; a++)
            for (b=0; b<24; b++)
               player_tile[a][b] = al_create_sub_bitmap(ptilemap, b*20, a*20, 20, 20);
      }
   }


   if (0) // create from shapes
   {
       for (a=0; a<16; a++)
          for (b=0; b<32; b++)
             player_tile[a][b] = al_create_bitmap(20,20);

   // fill the player_tile array
      for (a=0; a<16; a++) // set all to default shapes
      {
         al_set_target_bitmap(player_tile[a][0]); al_draw_bitmap(tile[400], 0, 0, 0);
         al_set_target_bitmap(player_tile[a][1]); al_draw_bitmap(tile[401], 0, 0, 0);
         al_set_target_bitmap(player_tile[a][2]); al_draw_bitmap(tile[402], 0, 0, 0);
         al_set_target_bitmap(player_tile[a][3]); al_draw_bitmap(tile[403], 0, 0, 0);
         al_set_target_bitmap(player_tile[a][4]); al_draw_bitmap(tile[404], 0, 0, 0);
         al_set_target_bitmap(player_tile[a][5]); al_draw_bitmap(tile[405], 0, 0, 0);

         al_set_target_bitmap(player_tile[a][6]); al_draw_bitmap(tile[368], 0, 0, 0);
         al_set_target_bitmap(player_tile[a][7]); al_draw_bitmap(tile[369], 0, 0, 0);
         al_set_target_bitmap(player_tile[a][8]); al_draw_bitmap(tile[370], 0, 0, 0);
         al_set_target_bitmap(player_tile[a][9]); al_draw_bitmap(tile[371], 0, 0, 0);
         al_set_target_bitmap(player_tile[a][10]); al_draw_bitmap(tile[372], 0, 0, 0);
         al_set_target_bitmap(player_tile[a][11]); al_draw_bitmap(tile[373], 0, 0, 0);

         al_set_target_bitmap(player_tile[a][12]); al_draw_bitmap(tile[432], 0, 0, 0);
         al_set_target_bitmap(player_tile[a][13]); al_draw_bitmap(tile[433], 0, 0, 0);
         al_set_target_bitmap(player_tile[a][14]); al_draw_bitmap(tile[434], 0, 0, 0);
         al_set_target_bitmap(player_tile[a][15]); al_draw_bitmap(tile[435], 0, 0, 0);
         al_set_target_bitmap(player_tile[a][16]); al_draw_bitmap(tile[436], 0, 0, 0);
         al_set_target_bitmap(player_tile[a][17]); al_draw_bitmap(tile[437], 0, 0, 0);

         al_set_target_bitmap(player_tile[a][18]); al_draw_bitmap(tile[755], 0, 0, 0);

         al_set_target_bitmap(player_tile[a][19]); al_draw_bitmap(tile[438], 0, 0, 0);

         al_set_target_bitmap(player_tile[a][20]); al_draw_bitmap(tile[606], 0, 0, 0);
         al_set_target_bitmap(player_tile[a][21]); al_draw_bitmap(tile[607], 0, 0, 0);

         al_set_target_bitmap(player_tile[a][22]); al_draw_bitmap(tile[638], 0, 0, 0);
         al_set_target_bitmap(player_tile[a][23]); al_draw_bitmap(tile[639], 0, 0, 0);

      }


      for (a=1; a<16; a++) //cycle 15 player colors (no zero)
      {
         int cs = -8 + a; // color shift (-8 to get from base to 0, then add player num for color)
         for (b=0; b<24; b++) //cycle 19 bitmaps for one color
         {
            al_set_target_bitmap(player_tile[a][b]);
            al_lock_bitmap(player_tile[a][b],al_get_bitmap_format(player_tile[a][b]),ALLEGRO_LOCK_READWRITE);

            for (x=0; x<20; x++)
               for (y=0; y<20; y++)
               {
                  ALLEGRO_COLOR p = al_get_pixel(player_tile[a][b], x, y);
                    float D = 0.1;
                    if (  (abs(p.r - palette_color[8].r) < D) &&
                          (abs(p.g - palette_color[8].g) < D) &&
                          (abs(p.b - palette_color[8].b) < D) ) al_put_pixel(x, y, palette_color[(8+cs)]);

                    if (  (abs(p.r - palette_color[56].r) < D) &&
                          (abs(p.g - palette_color[56].g) < D) &&
                          (abs(p.b - palette_color[56].b) < D) ) al_put_pixel(x, y, palette_color[(56+cs)]);

                    if (  (abs(p.r - palette_color[136].r) < D) &&
                          (abs(p.g - palette_color[136].g) < D) &&
                          (abs(p.b - palette_color[136].b) < D) ) al_put_pixel(x, y, palette_color[(136+cs)]);
               }
           al_unlock_bitmap(player_tile[a][b]);
           al_convert_mask_to_alpha(player_tile[a][b], al_map_rgb(0, 0, 0)) ;
         }
      }
   }

   if (1)
   {
       // show all new player shapes
       al_set_target_backbuffer(display);
       for (a=0; a<16; a++)
          for (b=0; b<24; b++)
             al_draw_bitmap(player_tile[a][b], b*20, a*20, 0);
       al_flip_display();
       tsw();

       al_rest(10);


   }

   if (0)
   {
       // save to disk
       al_set_target_bitmap(ptilemap);
       for (a=0; a<16; a++)
          for (b=0; b<24; b++)
             al_draw_bitmap(player_tile[a][b], b*20, a*20, 0);

       al_set_target_backbuffer(display);
       al_draw_bitmap(ptilemap, 0, 0, 0);

       al_flip_display();

       al_save_bitmap("bitmaps/player_tiles.bmp", ptilemap);

       tsw();
   }
}
