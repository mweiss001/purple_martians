// z_player.cpp

#include "pm.h"
#include "z_log.h"
#include "z_player.h"
#include "n_netgame.h"
#include "mwTally.h"
#include "mwFont.h"
#include "mwBitmap.h"
#include "mwLift.h"
#include "mwColor.h"
#include "mwInput.h"
#include "mwDisplay.h"
#include "mwProgramState.h"
#include "mwItems.h"
#include "mwEnemy.h"
#include "mwLevel.h"
#include "z_screen.h"
#include "z_screen_overlay.h"
#include "mwShots.h"
#include "z_solid.h"


struct player players[NUM_PLAYERS];
struct player1 players1[NUM_PLAYERS];
int active_local_player = 0;


void set_player_start_pos(int p, int cont)
{
   //printf("set player:%d start pos\n", p);
   int found = 0;
   if (mLevel.warp_level_location)
   {
      for (int i=0; i<500; i++)
         if (mItem.item[i][0] == 12)
         {
            if (mItem.item[i][8] == mLevel.warp_level_location)
            {
               found = 1;
               //printf("Found warp level location %d\n", warp_level_location);
               players[p].x = mItem.itemf[i][0];
               players[p].y = mItem.itemf[i][1] + 20;
            }
         }
      mLevel.warp_level_location = 0;
   }

   if (found == 0)
   {

      int ns = 0; // count number of starts
      int s[8] = {0};
      for (int i=0; i<500; i++)
         if (mItem.item[i][0] == 5)
         {
            ns++;
            s[mItem.item[i][7]] = i; // save index of this start
         }
      if (ns == 0)
      {
         printf("Error: no start found.\n");
         players[p].x = 20;
         players[p].y = 20;
      }
      if (ns == 1)
      {
         players[p].spawn_point_index = 0;
         int ps = s[players[p].spawn_point_index];
         players[p].x = mItem.itemf[ps][0];
         players[p].y = mItem.itemf[ps][1];
      }


      if (ns > 1)
      {
         int mode = mItem.item[s[0]][6];

         if (mode == 0)
         {
            printf("Error: in start mode:0 there should be only one start.. all other starts are ignored.\n");
            players[p].spawn_point_index = 0;
            players[p].x = mItem.itemf[s[0]][0];
            players[p].y = mItem.itemf[s[0]][1];

         }

         if (mode == 1) // team start
         {
            if (p % 2) // odd
            {
               players[p].spawn_point_index = 1;
               players[p].x = mItem.itemf[s[1]][0];
               players[p].y = mItem.itemf[s[1]][1];
            }
            else
            {
               players[p].spawn_point_index = 0;
               players[p].x = mItem.itemf[s[0]][0];
               players[p].y = mItem.itemf[s[0]][1];
            }
         }

         if ((mode == 2) || (mode == 3)) // check point common and individual
         {
            if (!cont) players[p].spawn_point_index = 0; // initial
            int ps = s[players[p].spawn_point_index];
            players[p].x = mItem.itemf[ps][0];
            players[p].y = mItem.itemf[ps][1];
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
      if ((mItem.item[i][0] == 5) && (mItem.item[i][7] == players[p].spawn_point_index))
      {
         players[p].PX = mItem.itemf[i][0];
         players[p].PY = mItem.itemf[i][1];

      }




*/




/*

   // count number of starts
   int ns = 0;

   int s[8] = {0};

   for (int i=0; i<500; i++)
      if (mItem.item[i][0] == 5)
      {
         ns++;
         s[mItem.item[i][7]] = i; // save index of this start
      }

   if (ns == 0)
   {
      printf("Error: no start found.\n");
      players[p].x = 20;
      players[p].y = 20;
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

      players[p].PX = mItem.itemf[i][0];
      players[p].PY = mItem.itemf[i][1];
   }

   */


}

void proc_player_health(int p)
{
   char msg[1024];
   if ((mwPS.frame_num) && (mwPS.frame_num == players1[p].block_damage_holdoff)) game_event(58, 0, 0, p, 0, 0, 0);

   if (players[p].old_health != players[p].health)
   {
      players1[p].last_health_adjust = (players[p].health - players[p].old_health);
      players[p].old_health = players[p].health;
      players1[p].health_display = 80;
   }
   if (players[p].health > 100)
   {
       players[p].health = 100;     // enforce max
       players[p].old_health = 100; // to prevent display
   }
   if (players[p].health < 1)
   {
      players[p].health = 0;

      sprintf(msg,"PLAYER:%d DIED!", p);
      if (LOG_NET) add_log_entry_header(10, 0, msg, 1);

      game_event(90, 0, 0, p, 0, 0, 0);  // player death

      players[p].stat_respawns++;

      set_player_join_quit_display(p, 3, 60);

      players1[p].health_display = 200;
      players[p].paused = 100;
      players[p].paused_type = 1;

   }
}


void proc_player_xy_move_test(int p)
{
   float m = 0.5;

   if (mI.key[ALLEGRO_KEY_LCTRL][0]) m *= 4;

   if (players[p].up) players[p].y -= m;
   if (players[p].down) players[p].y += m;
   if (players[p].left)
   {
      players[p].left_right = 0;
      players[p].x -= m;
   }
   if (players[p].right)
   {
      players[p].left_right = 1;
      players[p].x += m;
   }
}

void proc_player_xy_move(int p)
{
   float gravity = 0.6;
   float slow_gravity = 0.2; // used when jump is held
   float initial_jump_velocity = -6.6;
   float terminal_velocity = 7.8;
   float x_accel = 0.12;
   float x_de_accel = 0.24;
   float max_x_velocity = 4;
   float initial_x_velocity = 1.15;

   int x = players[p].x;
   int y = players[p].y;


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
   if (players[p].right_xinc < 0) players[p].right_xinc = 0;
   if (players[p].right_xinc > max_x_velocity) players[p].right_xinc = max_x_velocity;

   if (players[p].left_xinc > 0 ) players[p].left_xinc = 0;
   if (players[p].left_xinc < -max_x_velocity) players[p].left_xinc = -max_x_velocity;

   if (is_right_solid(x, y, 0, 1)) players[p].right_xinc = 0;
   if (is_left_solid(x, y, 0, 1))  players[p].left_xinc = 0;

   players[p].xinc = players[p].left_xinc + players[p].right_xinc;  // calc xinc
   players[p].x += players[p].xinc;                                // apply xinc


   x = players[p].x;
   y = players[p].y;
   if (players[p].player_ride) // if player is riding lift
   {
      int d = players[p].player_ride - 32; // lift number
      if ((Lift.cur[d].xinc > 0) && (!is_right_solid(x, y, 1, 1))) players[p].x  += Lift.cur[d].xinc; // moving right
      if ((Lift.cur[d].xinc < 0) && (!is_left_solid (x, y, 1, 1))) players[p].x  += Lift.cur[d].xinc; // moving left
   }

   if (int a = is_left_solid(x, y, 1, 1))
   {
      if (a > 31) // player being pushed right by lift
      {
         if (!is_right_solid(x, y, 0, 1))
         {
            int l = a-32; // lift that is pushing
            players[p].x = Lift.cur[l].x + Lift.cur[l].w + 1; // snap to lift pos + width
            players[p].right_xinc = Lift.cur[l].xinc;     // set players xinc from lift
         }
         else // player is getting squished
         {
            players[p].health -= 1;
            game_event(54, x, y, p, 0, 0, 0);
         }
      }
      else  if (players[p].xinc < 0) // moving left and block collision
      {
         players[p].left_xinc = 0;
         players[p].x = (((x/20)+1) * 20) - 3;  // align with wall
      }
   }
   x = players[p].x;
   y = players[p].y;
   if (int a = is_right_solid(x, y, 1, 1))
   {
      if (a > 31)    // player being pushed left by lift
      {
         if (!is_left_solid(x, y, 0, 1))
         {
            int l = a-32; // lift that is pushing
            players[p].x = Lift.cur[l].x - 20;            // snap to lift pos - 20
            players[p].left_xinc = Lift.cur[a-32].xinc;  // set players xinc from lift
         }
         else // player is getting squished
         {
            players[p].health -= 1;
            game_event(54, x, y, p, 0, 0, 0);
         }
      }
      else if (players[p].xinc > 0) // moving right and block collision
      {
         players[p].right_xinc = 0;
         players[p].x = ((x/20) * 20) + 2; // align with wall
      }
   }


// ---------------   check to see if player riding lift starts or ends --------------------------
   x = players[p].x;
   y = players[p].y;

   int a = is_down_solid(x, y, 1, 1);        // check for lift below player

   if (a > 31)
   {
      if (Lift.cur[a-32].yinc > 0)              // is lift going down
      {
         if (is_down_solid(x, y, 0, 1)) players[p].player_ride = 0;  // check for block below ignoring lifts
         else players[p].player_ride = a;
      }
      if (Lift.cur[a-32].yinc <= 0)             // is lift going up or steady
      {
          int offset = Lift.cur[a-32].y - y;   // to prevent lift from picking up early
          if (offset < 21) players[p].player_ride = a;
      }
   }
   else players[p].player_ride = 0;


// -----------   y move  ---------------------

   if (players[p].player_ride) // if player is riding lift
   {
      players[p].yinc = 0;
      int d = players[p].player_ride - 32; // lift number
      int x = players[p].x;
      int y = players[p].y;

      // if moving up and solid block above
      if ((Lift.cur[d].yinc < 0) && (is_up_solid(x, y, 0, 1) == 1))
      {
         players[p].player_ride = 0;  // player knocked off lift due to collision above
         players[p].health -= 1;      // take some damage
         game_event(54, x, y, p, 0, 0, 0);
      }

      // check for collision with lift above
      if (is_up_solid(x, y+1, 1, 1) > 31)
      {
         players[p].player_ride = 0;   // player knocked off lift due to collision above
         players[p].health -= 1;       // take some damage
         game_event(54, x, y, p, 0, 0, 0);
      }

      if (players[p].player_ride)        // if still riding
         players[p].y = Lift.cur[d].y - 20; // align with fy

      // moving down
      if (Lift.cur[d].yinc > 0)
      {
         if (is_down_solid(x, y, 0, 1))             // no lift check
         {
            players[p].player_ride = 0;             // ride over
            players[p].y = y - (y % 20);            // align with floor
         }
         else players[p].y = Lift.cur[d].y - 20;       // align with fy
      }


      if (players[p].jump)                                    // if jump pressed
      {
         players[p].player_ride = 0;                          // ride over
         x = players[p].x;
         y = players[p].y;
         if (!is_up_solid(x, y, 1, 1))                        // only jump if nothing above
         {
            players[p].yinc = initial_jump_velocity;
            players[p].y += players[p].yinc;                 // apply yinc
            game_event(15, x, y, 0, 0, 0, 0);
         }
      }
   }
   else // not player ride
   {
      if ((players[p].yinc < 0) && players[p].jump)           // if rising and jump pressed
         players[p].yinc += slow_gravity;                     // apply slow gravity
      else players[p].yinc += gravity;                        // apply regular gravity

      if (players[p].yinc < 0)                                // if still rising
      {
         players[p].y += players[p].yinc;                    // apply yinc
         x = players[p].x;
         y = players[p].y;
         if (is_up_solid(x, y+2, 1, 1))                       // look for collision above
         {
            players[p].y -= players[p].yinc;                 // take back move
            players[p].yinc = 0;                              // kill upwards motion
            players[p].y = ((y/20) + 1) * 20;        // align with ceiling
         }
      }
      if (players[p].yinc >= 0)                               // falling or steady
      {
         players[p].yinc += gravity;                          // apply gravity to yinc
         if (players[p].yinc > terminal_velocity)             // check for terminal velocity
            players[p].yinc = terminal_velocity;
         players[p].y += players[p].yinc;                    // apply yinc

         x = players[p].x;
         y = players[p].y;
         if (is_down_solid(x, y, 0, 1))                       // check for floor below (no lift)
         {
            players[p].yinc = 0;                              // kill downwards motion
            players[p].y = y - (y % 20);          // align with floor

            // check for collision with lift above if lift is moving down
            int a = is_up_solid(x, y, 1, 1);
            if ((a > 31) && (Lift.cur[a-32].yinc > 0))
            {
               // take some damage
               players[p].health -= 1;
               game_event(54, x, y, p, 0, 0, 0);
            }

            if (players[p].jump)                              // if jump pressed
            {
               x = players[p].x;
               y = players[p].y;
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
   if (players[p].paused_type == 2) mItem.proc_player_door_move(p);
   if (players[p].paused_type == 1) // frozen after player dies, until the timer runs out
   {
      players[p].carry_item = 0;
      if (players[p].paused == 100) // do only once per death
      {
         // does this player have any bomb remotes?
         for (int i=0; i<500; i++)
            if ((mItem.item[i][0] == 99) && (mItem.item[i][6] == 3)) // lit bomb with remote detonator
            {
               mItem.item[i][0] = 8; // change back to regular bomb
               // set bomb to explode!
               //mItem.item[i][6] = 2; // mode 2; explosion
               //mItem.item[i][8] = mItem.item[i][9] = 20; // explosion timer
            }
      }
      if (--players[p].paused > 0)
      {
         float sa = .025;
         players[p].draw_scale -= sa; // shrink player
         if (players[p].draw_scale < 0) players[p].draw_scale = 0;

         float ra;
         if (players[p].left_right) ra = 5;
         else ra = -5;

         players[p].draw_rot += ra; // rotate player
      }
      else // frozen done !!
      {
         players[p].paused = 0;
         players[p].old_health = 100;
         players[p].health = 100;
         players[p].left_xinc = 0;
         players[p].right_xinc = 0;
         players[p].xinc = 0;
         players[p].yinc = 0;
         set_player_start_pos(p, 1); // get starting position from start block
         draw_level2(NULL, 0, 0, 0, 1, 1, 1, 1, 1); // redraw entire level in case only region has been drawn
      }
   }
}

void reset_player_scale_and_rot(int p)
{
   players[p].draw_scale = 1;
   players[p].draw_rot = 0;
}


int is_player_riding_rocket(int p)
{
   if ((players[p].carry_item) && (mItem.item[players[p].carry_item-1][0] == 98)) return 1;
   return 0;
}

void proc_player_stuck_in_blocks(int p)
{
   int x = players[p].x;
   int y = players[p].y;
   int su =    is_up_solid(x, y, 0, 1);
   int sd =  is_down_solid(x, y, 0, 1);
   int sl =  is_left_solid(x, y, 0, 1);
   int sr = is_right_solid(x, y, 0, 1);
   if ((su) && (sd) && (sl) && (sr))
   {
      players[p].health -= 1;
      game_event(56, x, y, p, 0, 0, 0);
   }
}

void proc_player_riding_rocket(int p)
{
   int c = players[p].carry_item-1;
   float rot_inc = mItem.item[c][6];
   if (players[p].left)  mItem.item[c][10] -= rot_inc;
   if (players[p].right) mItem.item[c][10] += rot_inc;

   players[p].x    = mItem.itemf[c][0];  // set the player's position and incs the same as the rocket
   players[p].y    = mItem.itemf[c][1];
   players[p].xinc = mItem.itemf[c][2];
   players[p].yinc = mItem.itemf[c][3];

   players[p].left_xinc = players[p].right_xinc = 0;
   players[p].draw_rot = (float)mItem.item[c][10] / 1000;
   players[p].draw_scale = 0.5;
}

void proc_player_bounds_check(int p)
{
   // absolute level bounds check
   if (players[p].x < 0)    players[p].x = 0;
   if (players[p].y < 0)    players[p].y = 0;
   if (players[p].x > 1980) players[p].x = 1980;
   if (players[p].y > 1980) players[p].y = 1980;
}


void proc_player_collisions(int p)
{
   float px = players[p].x;
   float py = players[p].y;

   // items
   players1[p].potential_bomb_damage = 0;
   players[p].marked_door = -1; // so player can touch only one door
   for (int x=0; x<500; x++)
   {
      if ((mItem.item[x][0]) && (mItem.item[x][0] != 9) && (mItem.item[x][0] != 16) && (mItem.item[x][0] != 17))
      {
         float ix1 = mItem.itemf[x][0] - 16;
         float ix2 = mItem.itemf[x][0] + 16;
         float iy1 = mItem.itemf[x][1] - 16;
         float iy2 = mItem.itemf[x][1] + 16;
         if ((px > ix1) && (px < ix2) && (py > iy1) && (py < iy2) && (!players[p].paused) ) mItem.proc_item_collision(p, x);
      }
   }
   // enemies
   for (int e=0; e<100; e++)
   {
      if ((mEnemy.Ei[e][0]) && (mEnemy.Ei[e][0] != 99)) // if active and not deathcount
      {
         float b = mEnemy.Ei[e][29]; // collision box size
         float ex1 = mEnemy.Ef[e][0] - b;
         float ex2 = mEnemy.Ef[e][0] + b;
         float ey1 = mEnemy.Ef[e][1] - b;
         float ey2 = mEnemy.Ef[e][1] + b;
         if ((px > ex1) && (px < ex2) && (py > ey1) && (py < ey2)) mEnemy.Ei[e][22] = p+1;
      }
   }

   // eshots
   for (int b=0; b<50; b++)
      if (mwS.e[b].active)  // if active
      {
         // new collision box is based on shot speed and has both x and y component
         float ax = fabs(mwS.e[b].xinc);
         float ay = fabs(mwS.e[b].yinc);

         // enforce minimums
         if (ax < 4) ax = 4;
         if (ay < 4) ay = 4;

         float bx1 = mwS.e[b].x - ax;
         float bx2 = mwS.e[b].x + ax;
         float by1 = mwS.e[b].y - ay;
         float by2 = mwS.e[b].y + ay;

         if ((px > bx1) && (px < bx2) && (py > by1) && (py < by2)) mwS.proc_eshot_collision(p, b);
      }

   // pshots
   for (int b=0; b<50; b++)
      if (mwS.p[b].active)
      {
         float bx1 = mwS.p[b].x - 10;
         float bx2 = mwS.p[b].x + 10;
         float by1 = mwS.p[b].y - 10;
         float by2 = mwS.p[b].y + 10;

         if ((px > bx1) && (px < bx2) && (py > by1) && (py < by2))
         {
            int pb = mwS.p[b].player; // player that fired the shot
            if ((mwS.deathmatch_shots) && (pb != p))
            {
                mwS.proc_pshot_collision(p, b);
                game_event(40, 0, 0, p, pb, 0, mwS.deathmatch_shot_damage);
            }
            if ((mwS.suicide_shots) && (pb == p))
            {
                mwS.proc_pshot_collision(p, b);
                game_event(41, 0, 0, p, pb, 0, mwS.deathmatch_shot_damage);
            }
         }
      }
}


int is_player_within_ladder_reach(int p)
{
   // detect if player is on ladder block
   int AX = players[p].x;
   int AY = players[p].y;

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
         if (mLevel.l[x][y] & PM_BTILE_LADDER_MOVE) return 1;
      }
   return 0;
}

int is_player_within_rope_reach(int p)
{
   // detect if player is on rope block
   int AX = players[p].x;
   int AY = players[p].y;

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

   //al_draw_textf(mF.pr8, mC.pc[15], AX, AY-20, 0, "%d", am );
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
      if (mLevel.l[x][by] & PM_BTILE_ROPE_MOVE) return 1;
   }
   return 0;
}







void proc_player_rope_move(int p)
{
    // reset all regular incs
   players[p].xinc = 0;
   players[p].yinc = 0;
   players[p].left_xinc = 0;
   players[p].right_xinc = 0;

   if (players[p].left)
   {
      players[p].left_right = 0;
      players[p].x -= is_left_solidf(players[p].x, players[p].y, 3, 0);
   }
   if (players[p].right)
   {
      players[p].left_right = 1;
      players[p].x += is_right_solidf(players[p].x, players[p].y, 3, 0);
   }
   if (players[p].down)
   {
      players[p].on_rope = 0;
      players[p].y += 4;
   }
}


void proc_player_ladder_move(int p)
{
   float m = 3;

   int old_px = players[p].x;
   int old_py = players[p].y;

   // reset all regular incs
   players[p].xinc = 0;
   players[p].yinc = 0;
   players[p].left_xinc = 0;
   players[p].right_xinc = 0;

   if (players[p].jump)
   {
      players[p].on_ladder = 0;
      players[p].yinc = -6.6; // initial_jump_velocity;
      players[p].y += players[p].yinc;  // make the first move here
      return;
   }
   else
   {
      if (players[p].up)   players[p].y -=   is_up_solidf(players[p].x, players[p].x, m, 0);
   }

   if (players[p].down) players[p].y += is_down_solidf(players[p].y, players[p].y, m, 0);
   if (players[p].left)
   {
      players[p].left_right = 0;
      players[p].x -= is_left_solidf(players[p].x, players[p].y, m, 0);
   }
   if (players[p].right)
   {
      players[p].left_right = 1;
      players[p].x += is_right_solidf(players[p].x, players[p].y, m, 0);
   }

   // how much did we move this last frame?
   int px = players[p].x;
   int py = players[p].y;
   int xd = px - old_px;
   int yd = py - old_py;

   // did we try to move up past top of ladder?
   if (yd < 0) if (!is_player_within_ladder_reach(p)) players[p].y += m;


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
               float tm = is_up_solidf(tx, players[p].y, m, 0);
               if (tm > 0)
               {
                  //printf("%d px:%d opx:%d tx:%d x+u\n", mwPS.frame_num, px, old_px, tx );
                  players[p].y = tx; // set x to passed by pos
                  players[p].y -= m; // move in y
                  tx = px+1; // break out of loop
               }
            }
            else if (players[p].down)
            {
               float tm = is_down_solidf(tx, players[p].y, m, 0);
               if (tm > 0)
               {
                  //printf("%d px:%d opx:%d tx:%d x+d\n", mwPS.frame_num, px, old_px, tx );
                  players[p].y = tx; // set x to passed by pos
                  players[p].y += m; // move in y
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
               float tm = is_up_solidf(tx, players[p].y, m, 0);
               if (tm > 0)
               {
                  //printf("%d px:%d opx:%d tx:%d x-u\n", mwPS.frame_num, px, old_px, tx );
                  players[p].x = tx; // set x to passed by pos
                  players[p].y -= m; // move in y
                  tx = old_px+1; // break out of loop
               }
            }
            else if (players[p].down)
            {
               float tm = is_down_solidf(tx, players[p].y, m, 0);
               if (tm > 0)
               {
                  //printf("%d px:%d opx:%d tx:%d x-d\n", mwPS.frame_num, px, old_px, tx );
                  players[p].x = tx; // set x to passed by pos
                  players[p].y += m; // move in y
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
               float tm = is_right_solidf(players[p].x, ty1, m, 0);
               if (tm > 0)
               {
                  //printf("%d py:%d opy:%d ty1:%d y+r\n", mwPS.frame_num, py, old_py, ty1 );
                  players[p].y = ty1; // set y to passed by pos
                  players[p].x += m; // move in x
                  ty1 = py+1; // break out of loop
               }
            }
            else if (players[p].left)
            {
               float tm = is_left_solidf(players[p].x, ty1,  m, 0);
               if (tm > 0)
               {
                  //printf("%d py:%d opy:%d ty1:%d y+l\n", mwPS.frame_num, py, old_py, ty1 );
                  players[p].y = ty1; // set y to passed by pos
                  players[p].x -= m; // move in x
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
               float tm = is_right_solidf(players[p].x, ty1, m, 0);
               if (tm > 0)
               {
                  //printf("%d py:%d opy:%d ty1:%d y-r\n", mwPS.frame_num, py, old_py, ty1 );
                  players[p].y = ty1; // set y to passed by pos
                  players[p].x += m; // move in x
                  ty1 = old_py+1; // break out of loop
               }
            }
            else if (players[p].left)
            {
               float tm = is_left_solidf(players[p].x, ty1,  m, 0);
               if (tm > 0)
               {
                  //printf("%d py:%d opy:%d ty1:%d y-l\n", mwPS.frame_num, py, old_py, ty1 );
                  players[p].y = ty1; // set y to passed by pos
                  players[p].x -= m; // move in x
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
         ((players[p].yinc > 0) || (players[p].up)) ) // player is falling or up is pressed
      {
         // plater just got on rope
         players[p].on_rope = 1;
         players[p].on_ladder = 0;

         // snap y to rope
         int AY = players[p].y;
         int by = AY / 20;
         int am = AY % 20;

         if (am <  5) players[p].y = (by+0) * 20; // this block only
         if (am > 15) players[p].y = (by+1) * 20; // next block only

         // check to see if player is embedded in wall to right
         int done = 0;
         while (!done)
         {
            float m = is_right_solidf(players[p].x-1, players[p].y, 1, 0);
            if (m < 1) players[p].x -= 1;
            else done = 1;
         }
         // check to see if player is embedded in wall to left
         done = 0;
         while (!done)
         {
            float m = is_left_solidf(players[p].x+1, players[p].y, 1, 0);
            if (m < 1) players[p].x += 1;
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
         int done = 0;
         while (!done)
         {
            float m = is_right_solidf(players[p].x-1, players[p].y, 1, 0);
            //printf("R %f %f\n", m, players[p].x);
            if (m < 1) players[p].x -= 1;
            else done = 1;
         }

         // check to see if player is embedded in wall to left
         done = 0;
         while (!done)
         {
            float m = is_left_solidf(players[p].x-1, players[p].y, 1, 0);
            //printf("L %f %f\n", m, players[p].x);
            if (m < 1) players[p].x += 1;
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
            mwS.proc_player_shoot(p);
            mItem.proc_player_carry(p);
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
      al_set_target_bitmap(mwB.level_buffer);
      get_players_shape(p);
      int AX = players[p].x;
      int AY = players[p].y;
      float scale = players[p].draw_scale;
      float rot = players[p].draw_rot;
      int flags = ALLEGRO_FLIP_HORIZONTAL;
      if (players[p].left_right) flags = ALLEGRO_FLIP_VERTICAL & ALLEGRO_FLIP_HORIZONTAL;

   //   printf("color:%d shape:%d\n", players[p].color, players[p].shape );

      al_draw_scaled_rotated_bitmap(mwB.player_tile[players[p].color][players[p].shape], 10, 10, AX+10, AY+10, scale, scale, rot, flags);











      /*


      al_draw_textf(mF.pr8, mC.pc[15], AX+10, AY-30, ALLEGRO_ALIGN_CENTER, "X:%d Y:%d", AX, AY);


      if (players[p].on_ladder)
         al_draw_rectangle(0.5+AX, 0.5+AY, 0.5+AX+19, 0.5+AY+19, mC.pc[11], 1);

      if (players[p].on_rope)
         al_draw_rectangle(0.5+AX, 0.5+AY, 0.5+AX+17, 0.5+AY+19, mC.pc[10], 1);


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

      al_draw_textf(mF.pr8, mC.pc[15], AX, AY-20, 0, "%d", am );

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
         if (timer_draw_mode2) al_draw_textf(mF.pr8, mC.pc[col], x0+10, y0+6, ALLEGRO_ALIGN_CENTER, "%d", tts);
         if (timer_draw_mode2) al_draw_textf(mF.pr8, mC.pc[col], x0+10, y0+6, ALLEGRO_ALIGN_CENTER, "%d", tts);
         if (timer_draw_mode2) al_draw_textf(mF.pr8, mC.pc[col], x0+10, y0+6, ALLEGRO_ALIGN_CENTER, "%d", tts);
           if (timer_draw_mode2) al_draw_textf(mF.pr8, mC.pc[col], x0+10, y0+6, ALLEGRO_ALIGN_CENTER, "%d", tts);
    for (int x=bx1; x<=bx2; x++)
      {
         al_draw_rectangle(0.5+x*20, 0.5+by*20, 0.5+(x*20)+19, 0.5+(by*20)+19, mC.pc[8], 1);
         if (l[x][by] == 19) al_draw_rectangle(0.5+x*20, 0.5+by*20, 0.5+(x*20)+19, 0.5+(by*20)+19, mC.pc[11], 1);
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
            al_draw_bitmap(mwB.tile[952+seq], AX, AY, flags);
         }
      }

      if (players1[p].health_display > 0)
      {
         players1[p].health_display--;

         // show current health bar
         int ch = players[p].health; // current health
         draw_percent_bar(AX + 10, AY - 6, 16, 3, ch);

         // show last health adjustment
         int h = players1[p].last_health_adjust; // last health adjust
         if (h > 0) al_draw_textf(mF.pixl, mC.pc[11], AX+10, AY-16, ALLEGRO_ALIGN_CENTER, "%+d", h);
         if (h < 0) al_draw_textf(mF.pixl, mC.pc[10], AX+10, AY-16, ALLEGRO_ALIGN_CENTER, "%+d", h);

         // show potential bomb damage
         int dmg = players1[p].potential_bomb_damage; // potential bomb damage
         if (dmg)
         {
            int nh = ch - dmg; // new health
            if (nh < 0) nh = 0;
            if (mwPS.frame_num % 20 < 10)
            {
               // draw segment from new health to current health
               draw_percent_bar_range(AX+10, AY-6, 16, 3, 10, nh, ch);

               // show damage amount
               al_draw_textf(mF.pixl, mC.pc[10], AX+10, AY-16, ALLEGRO_ALIGN_CENTER, "%+d", -dmg);

               // draw a tiny bomb picture
               al_draw_scaled_rotated_bitmap(mwB.tile[464], 10, 10,  AX+20, AY-12, .5, .5, 0, 0);
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
   int pos = ((int) players[p].x / 6) % 6;  // 6 shapes in sequence

   // if riding rocket use static shape
   if ((players[p].carry_item) && (mItem.item[players[p].carry_item-1][0] == 98)) pos = 0;

   // if player riding lift animate with player's xpos relative to lift
   if (players[p].player_ride)
   {
      int rx = 20 + (int) players[p].x - Lift.cur[players[p].player_ride-32].x;
      pos = (rx / 4) % 5;  // try 5 for now
   }

   // paused can mean dead or it can also mean in door travel
   // if paused use static shape
   if (players[p].paused) players[p].shape = 0;
   else players[p].shape = index + pos;

   // if jump or fall use static shape
   if (players[p].yinc != 0) players[p].shape = 19;


   if (players[p].on_rope)
   {
      // animate with x pos
      int x = players[p].x;

      // 2 shapes in sequence
      // 3 is rope move
      int pos = (x/3) % 2;
      players[p].shape = 20 + pos;
      /// printf("f:%d x:%d pos:%d, shape:%d\n", mwPS.frame_num, x, pos, shape );
   }

   if (players[p].on_ladder)
   {
      // animate with y pos
      int y = players[p].y;

      // 2 shapes in sequence
      // 3 is ladder move
      int pos = (y/3) % 2;
      players[p].shape = 22 + pos;
      //printf("f:%d y:%d pos:%d, shape:%d\n", mwPS.frame_num, y, pos, shape );
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

      players[p].old_health = 100;
      players[p].health = 100;

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

      players[p].xinc = 0;
      players[p].yinc = 0;
      players[p].left_xinc = 0;
      players[p].right_xinc = 0;


      players[p].shot_wait_counter=0;
      players[p].request_shot = 0;
      players[p].shot_wait = 4;
      players[p].shot_speed = 12;

      players[p].draw_rot = 0;
      players[p].draw_scale = 1;
      players[p].shape = 0;

      players[p].stat_respawns = 0;
      players[p].stat_shots_fired = 0;
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
      mwT_late_cdats_last_sec[p].initialize(); // initialize tally

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

      players1[p].game_move_dsync = 0;
      players1[p].game_move_dsync_avg_last_sec = 0;
      mwT_game_move_dsync_avg_last_sec[p].initialize(); // initialize tally
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
      mwB.ptilemap = al_load_bitmap("bitmaps/player_tiles.bmp");
      if (!mwB.ptilemap) mI.m_err((char*)"Can't load tiles from bitmaps/player_tiles.bmp");
      else
      {
         //printf("load good\n");
         al_convert_mask_to_alpha(mwB.ptilemap, al_map_rgb(0, 0, 0)) ;

         al_set_target_bitmap(mwB.M_ptilemap);
         al_draw_bitmap(mwB.ptilemap, 0, 0, 0);

         // create sub bitmaps
         for (a=0; a<16; a++)
            for (b=0; b<24; b++)
               mwB.player_tile[a][b] = al_create_sub_bitmap(mwB.ptilemap, b*20, a*20, 20, 20);
      }
   }


   if (0) // create from shapes
   {
       for (a=0; a<16; a++)
          for (b=0; b<32; b++)
             mwB.player_tile[a][b] = al_create_bitmap(20,20);

   // fill the player_tile array
      for (a=0; a<16; a++) // set all to default shapes
      {
         al_set_target_bitmap(mwB.player_tile[a][0]); al_draw_bitmap(mwB.tile[400], 0, 0, 0);
         al_set_target_bitmap(mwB.player_tile[a][1]); al_draw_bitmap(mwB.tile[401], 0, 0, 0);
         al_set_target_bitmap(mwB.player_tile[a][2]); al_draw_bitmap(mwB.tile[402], 0, 0, 0);
         al_set_target_bitmap(mwB.player_tile[a][3]); al_draw_bitmap(mwB.tile[403], 0, 0, 0);
         al_set_target_bitmap(mwB.player_tile[a][4]); al_draw_bitmap(mwB.tile[404], 0, 0, 0);
         al_set_target_bitmap(mwB.player_tile[a][5]); al_draw_bitmap(mwB.tile[405], 0, 0, 0);

         al_set_target_bitmap(mwB.player_tile[a][6]); al_draw_bitmap(mwB.tile[368], 0, 0, 0);
         al_set_target_bitmap(mwB.player_tile[a][7]); al_draw_bitmap(mwB.tile[369], 0, 0, 0);
         al_set_target_bitmap(mwB.player_tile[a][8]); al_draw_bitmap(mwB.tile[370], 0, 0, 0);
         al_set_target_bitmap(mwB.player_tile[a][9]); al_draw_bitmap(mwB.tile[371], 0, 0, 0);
         al_set_target_bitmap(mwB.player_tile[a][10]); al_draw_bitmap(mwB.tile[372], 0, 0, 0);
         al_set_target_bitmap(mwB.player_tile[a][11]); al_draw_bitmap(mwB.tile[373], 0, 0, 0);

         al_set_target_bitmap(mwB.player_tile[a][12]); al_draw_bitmap(mwB.tile[432], 0, 0, 0);
         al_set_target_bitmap(mwB.player_tile[a][13]); al_draw_bitmap(mwB.tile[433], 0, 0, 0);
         al_set_target_bitmap(mwB.player_tile[a][14]); al_draw_bitmap(mwB.tile[434], 0, 0, 0);
         al_set_target_bitmap(mwB.player_tile[a][15]); al_draw_bitmap(mwB.tile[435], 0, 0, 0);
         al_set_target_bitmap(mwB.player_tile[a][16]); al_draw_bitmap(mwB.tile[436], 0, 0, 0);
         al_set_target_bitmap(mwB.player_tile[a][17]); al_draw_bitmap(mwB.tile[437], 0, 0, 0);

         al_set_target_bitmap(mwB.player_tile[a][18]); al_draw_bitmap(mwB.tile[755], 0, 0, 0);

         al_set_target_bitmap(mwB.player_tile[a][19]); al_draw_bitmap(mwB.tile[438], 0, 0, 0);

         al_set_target_bitmap(mwB.player_tile[a][20]); al_draw_bitmap(mwB.tile[606], 0, 0, 0);
         al_set_target_bitmap(mwB.player_tile[a][21]); al_draw_bitmap(mwB.tile[607], 0, 0, 0);

         al_set_target_bitmap(mwB.player_tile[a][22]); al_draw_bitmap(mwB.tile[638], 0, 0, 0);
         al_set_target_bitmap(mwB.player_tile[a][23]); al_draw_bitmap(mwB.tile[639], 0, 0, 0);

      }


      for (a=1; a<16; a++) //cycle 15 player colors (no zero)
      {
         int cs = -8 + a; // color shift (-8 to get from base to 0, then add player num for color)
         for (b=0; b<24; b++) //cycle 19 bitmaps for one color
         {
            al_set_target_bitmap(mwB.player_tile[a][b]);
            al_lock_bitmap(mwB.player_tile[a][b],al_get_bitmap_format(mwB.player_tile[a][b]),ALLEGRO_LOCK_READWRITE);

            for (x=0; x<20; x++)
               for (y=0; y<20; y++)
               {
                  ALLEGRO_COLOR p = al_get_pixel(mwB.player_tile[a][b], x, y);
                    float D = 0.1;
                    if (  (abs(p.r - mC.pc[8].r) < D) &&
                          (abs(p.g - mC.pc[8].g) < D) &&
                          (abs(p.b - mC.pc[8].b) < D) ) al_put_pixel(x, y, mC.pc[(8+cs)]);

                    if (  (abs(p.r - mC.pc[56].r) < D) &&
                          (abs(p.g - mC.pc[56].g) < D) &&
                          (abs(p.b - mC.pc[56].b) < D) ) al_put_pixel(x, y, mC.pc[(56+cs)]);

                    if (  (abs(p.r - mC.pc[136].r) < D) &&
                          (abs(p.g - mC.pc[136].g) < D) &&
                          (abs(p.b - mC.pc[136].b) < D) ) al_put_pixel(x, y, mC.pc[(136+cs)]);
               }
           al_unlock_bitmap(mwB.player_tile[a][b]);
           al_convert_mask_to_alpha(mwB.player_tile[a][b], al_map_rgb(0, 0, 0)) ;
         }
      }
   }

   if (1)
   {
       // show all new player shapes
       al_set_target_backbuffer(display);
       for (a=0; a<16; a++)
          for (b=0; b<24; b++)
             al_draw_bitmap(mwB.player_tile[a][b], b*20, a*20, 0);
       al_flip_display();
       mI.tsw();

       al_rest(10);


   }

   if (0)
   {
       // save to disk
       al_set_target_bitmap(mwB.ptilemap);
       for (a=0; a<16; a++)
          for (b=0; b<24; b++)
             al_draw_bitmap(mwB.player_tile[a][b], b*20, a*20, 0);

       al_set_target_backbuffer(display);
       al_draw_bitmap(mwB.ptilemap, 0, 0, 0);

       al_flip_display();

       al_save_bitmap("bitmaps/player_tiles.bmp", mwB.ptilemap);

       mI.tsw();
   }
}
