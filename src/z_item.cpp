// z_item.cpp
#include "pm.h"
#include "z_item.h"
#include "z_sound.h"
#include "z_player.h"
#include "mwFont.h"
#include "mwBitmap.h"
#include "mwLift.h"
#include "mwWidgets.h"
#include "mwColor.h"
#include "mwPMEvent.h"
#include "mwDisplay.h"
#include "mwProgramState.h"
#include "z_menu.h"
#include "z_enemy.h"
#include "mwLevel.h"
#include "e_fnx.h"

#include "z_screen.h"
#include "z_screen_overlay.h"
#include "mwShots.h"
#include "z_solid.h"
#include "z_item_key.h"
#include "z_item_bomb.h"
#include "z_item_pmsg.h"
#include "z_item_door.h"
#include "z_item_trigger.h"


// items
int item[500][16];      // item ints
float itemf[500][4]; // item floats

int item_num_of_type[30];
int item_first_num[30];
char item_name[30][40];
char pmsgtext[500][500] = {0};
int item_tile[20];


void draw_item(int i, int custom, int cx, int cy)
{
   int type = item[i][0];
   int x = itemf[i][0];
   int y = itemf[i][1];

   if (custom)
   {
      x = cx;
      y = cy;
   }

   int shape = item[i][1];                       // get shape
   if (shape > 999) shape = mwB.zz[0][shape-1000];   // ans
   int drawn = 0;




   if ((type == 10) && (!custom)) // pop up message
   {
      int timer_count=0, timer_val=0;
      get_int_3216(item[i][12], timer_count, timer_val);

      // if timer running or always show, draw the message
      if ((timer_count) || (item[i][2] & PM_ITEM_PMSG_SHOW_ALWAYS)) draw_pop_message(i, 0, 0, 0, 0, 0, msg);

      // if hide scroll and not running level editor flag scroll as being drawn already
      if ((!(item[i][2] & PM_ITEM_PMSG_SHOW_SCROLL)) && (!mwPS.level_editor_running)) drawn = 1;
   }




   if (type == 1)  { draw_door(i, x, y, custom);         drawn = 1; }
   if (type == 6)  { draw_orb(i, x, y);                  drawn = 1; }
   if (type == 9)  { draw_trigger(i, x, y);              drawn = 1; }
   if (type == 16) { draw_block_manip(i, x, y);          drawn = 1; }
   if (type == 17) { draw_block_damage(i, x, y, custom); drawn = 1; }

   if (type == 4) drawn = draw_key(i); // only draws here in last stage

   if ((type == 8) && (item[i][11]) ) al_draw_bitmap(mwB.tile[440], x, y, 0); // bomb sticky spikes

   if (type == 99)
   {
      draw_lit_bomb(i);
      if ((item[i][11]) && (item[i][6] != 2)) al_draw_bitmap(mwB.tile[440], x, y, 0);  // bomb sticky spikes
      drawn = 1;
   }


    // these types need rotation
   if ((type == 11) || (type == 98) ||  // rockets
      ((type == 4) && (item[i][11] > 0) && (!drawn)) ) // moving key
   {
      float rot = (float) item[i][10] / 1000;
      al_draw_rotated_bitmap(mwB.tile[shape], 10, 10, x+10, y+10, rot, 0);
      drawn = 1;
   }

   if (type == 98) draw_rocket_lines(i); // for lit rockets

   if (type == 5) // start
   {
      if (mLevel.number_of_starts > 1)
      {
         al_draw_bitmap(mwB.tile[shape], x, y, 0);
         al_draw_textf(mF.pixl, mC.pc[12], x+10, y-4, ALLEGRO_ALIGN_CENTER, "%d", item[i][7]);
         drawn = 1;
      }
   }

   if (type == 3) // exit
   {
      al_draw_bitmap(mwB.tile[399], x, y, 0); // 'exit' text not shown
      if (mwPS.frame_num % 60 > 30)
         al_draw_text(mF.pixl, mC.pc[10], x+11, y-2, ALLEGRO_ALIGN_CENTER, "EXIT");

      int exit_enemys_left = num_enemy - item[i][8];
      if (exit_enemys_left > 0) // locked
      {
         al_draw_bitmap(mwB.tile[366], x, y, 0); // show lock
         if (mwPS.frame_num % 60 < 30)
            al_draw_textf(mF.pixl, mC.pc[14], x+11, y-2, ALLEGRO_ALIGN_CENTER, "%d", exit_enemys_left);

      }
      drawn = 1;
   }

   if ((type == 2) && (item[i][6] == 3)) // purple coin custom draw
   {
      if (!mwPS.level_editor_running)
      {
         mwB.spin_shape(shape, x, y, 0, 0, 19, 19, 0.8, 0.5, 40);
         drawn = 1;
      }
   }

   // default draw if nothing else has drawn it up to now
   if (!drawn) al_draw_bitmap(mwB.tile[shape], x, y, 0);

   if (!mwPS.level_editor_running)
   {
      // if item is expiring show how many seconds left it has
      if ((item[i][14]>10) && (type != 9) && (type != 16) && (type != 17))
         al_draw_textf(mF.pixl, mC.pc[15], x+10, y-10, ALLEGRO_ALIGN_CENTER, "%d", 1 + (item[i][14] - 10) / 40);
   }
}


void draw_items(void)
{
   al_set_target_bitmap(mwB.level_buffer);

   for (int i=0; i<500; i++)
      if ((item[i][0]) && (item[i][0] != 10)) draw_item(i, 0, 0, 0);

   for (int i=0; i<500; i++) // draw msg last so they are on top...
      if (item[i][0] == 10) draw_item(i, 0, 0, 0);
}



int is_item_stuck_to_wall(int i)
{
   int x = itemf[i][0];
   int y = itemf[i][1];
   if ( (is_left_solid(x,y, 0, 3)) || (is_right_solid(x,y, 0, 3)) || (is_down_solid(x,y, 0, 3)) || (is_up_solid(x,y, 0, 3)) ) return 1;
   return 0;
}

void move_items()
{
   for (int i=0; i<500; i++)
      if (item[i][0])
      {
         int x = itemf[i][0];
         int y = itemf[i][1];
         if ((x<0) || (x>1980) || (y<0) || (y>1980)) item[i][0] = 0; // remove if out of bounds

         int type = item[i][0];
         if ((type == 4) && (item[i][11] > 0)) proc_moving_key(i);
         if (type == 6)  proc_orb(i);
         if (type == 9)  proc_trigger(i);
         if (type == 10) proc_pmsg(i);
         if (type == 16) proc_block_manip(i);
         if (type == 17) proc_block_damage(i);
         if (type == 99) proc_lit_bomb(i);
         if (type == 98) proc_lit_rocket(i);


         // check for time to live
         if ((type != 9) && (type != 16) && (type != 17))
         {
            int ttl = item[i][14];
            if (ttl)
            {
               if (ttl < 11)       // start ans seq
               {
                  item[i][0] = 66; // change to different type to prevent use
                  int sq = 10-ttl;
                  item[i][1] = mwB.zz[5+sq][74];
               }
               if (ttl == 1) item[i][0] = 0; // kill instantly
               item[i][14]--;
            }
         }

         // not stationary and not lit rocket, trigger, bm, bd or moving key
         if ((item[i][3]) && (type != 98) && (type != 9) && (type != 16) && (type != 17) && (! ((type == 4) && (item[i][11] > 0)))    )
         {
            // check if being carried
            int pc = 0;
            for (int p=0; p<NUM_PLAYERS; p++)
               if (players[p].active)
                  if ((!players[p].paused) || (players[p].paused && players[p].paused_type == 2))
                     if (i == (players[p].carry_item-1)) pc = 1;

            if (!pc) // not being carried
            {
               // check for sticky bomb stuck to wall
               int sticky = 0;
               if ((type == 99) && (item[i][11])) sticky = 1;
               if ((sticky) && (is_item_stuck_to_wall(i)) )
               {
                  itemf[i][2] = 0;  // xinc
                  itemf[i][3] = 0;  // yinc
               }
               else
               {
                  // apply incs
                  itemf[i][0] += itemf[i][2];  // xinc
                  itemf[i][1] += itemf[i][3];  // yinc

                  // slow down xinc (friction)
                  if (itemf[i][2] > 0)
                  {
                     itemf[i][2] -= .01;                    // slow down +xinc
                     if (itemf[i][2] < 0) itemf[i][2] = 0; // set to zero if crosses zero
                  }

                  if (itemf[i][2] < 0)
                  {
                     itemf[i][2] += .01;                    // slow down - xinc
                     if (itemf[i][2] > 0) itemf[i][2] = 0; // set to zero if crosses zero

                  }

                  x = itemf[i][0];
                  y = itemf[i][1];


                  // moving right and hit wall
                  if ((itemf[i][2] > 0) && (is_right_solid(x,y, 1, 3)))
                  {
                     if (!sticky) itemf[i][0] -= itemf[i][2];  // take back xinc
                     itemf[i][2] = 0;                           // stop
                  }

                  // moving left and hit wall
                  if ((itemf[i][2] < 0) && (is_left_solid(x,y, 1, 3)))
                  {
                     if (!sticky) itemf[i][0] -= itemf[i][2];  // take back xinc
                     itemf[i][2] = 0;                           // stop
                  }

                  x = itemf[i][0];
                  y = itemf[i][1];

                  // moving up
                  if (itemf[i][3] < 0)
                  {
                     if (is_up_solid(x, y, 0, 3) == 1)    // only check for solid blocks
                        itemf[i][3] = 0;                 // if collision kill upwards yinc
                     else itemf[i][3] += .1;             // else de-accel
                  }
                  else // not moving up
                  {
                     int a = is_down_solid(x, y, 1, 3);             // check for block below
                     if (a==0)
                     {

                        if (! ((type == 99) && (item[i][6] == 2))) // no gravity for exploding bomb
                           itemf[i][3] += .1;                     // apply gravity to yinc

                        if (itemf[i][3] > 3) itemf[i][3] = 3;  // max gravity
                     }
                     if (a) // slow down xinc if block or lift below
                     {
                        if (itemf[i][2] > 0) itemf[i][2] -= .12;
                        if (itemf[i][2] < 0) itemf[i][2] += .12;
                     }

                     if ((a==1) || (a==2)) // align with ground if block below
                     {
                        itemf[i][1] = (y/20)*20; // align with ground
                        itemf[i][3] = 0;         // zero yinc
                     }

                     if (a > 31) // item riding lift
                     {
                        int capture = 0;

                        if (Lift.cur[a-32].yinc < 0) // lift is moving up
                        {
                           int offset = Lift.cur[a-32].y - y;   // to prevent lift from picking up early when lift going up
                           if (offset < 21) capture = 1;
                        }
                        if (Lift.cur[a-32].yinc >= 0) // lift is moving down or steady
                        {
                           if (is_down_solid(x, y, 0, 3)) capture = 0; // to prevent lift attempting to take item down through solid block
                           else capture = 1;
                           int offset = Lift.cur[a-32].y - y;   // to prevent lift from picking up early when item going down
                           if (offset > 21) capture = 0;
                        }
                        if (capture)
                        {
                           float lxi = Lift.cur[a-32].xinc;
                           float lyi = Lift.cur[a-32].yinc;

                           itemf[i][0] += lxi;                 // move x with lift's xinc
                           itemf[i][1]  = Lift.cur[a-32].y - 20;  // align with lift's y

                           x = itemf[i][0];
                           y = itemf[i][1];

                           if (lyi > 0) // down
                              if (is_down_solid(x, y, 0, 3))       // no lift check
                                 itemf[i][1] = (y - (y % 20));    // item not on lift anymore, align with block

                           if (lyi < 0) // up
                              if (is_up_solid(x, y, 0, 3) == 1)
                                 itemf[i][1] += 10;

                           if (lxi > 0) // right
                              if (is_right_solid(x, y, 1, 3))
                                 itemf[i][0] -= lxi;

                           if (lxi < 0) // left
                              if (is_left_solid(x, y, 1, 3))
                                 itemf[i][0] -= lxi;
                        }
                     } // end of riding lift
                  } // end of not moving up
               } // end of not stuck to wall
            } // end of not being carried
         } // end of if not stationary and not lit rocket
      } // end of iterate all active items
}

int player_drop_item(int p)
{
   int i = players[p].carry_item-1; // number of item
   // printf("drop item:%d\n", i);

   int wall_stuck = 0;
   players[p].carry_item = 0;
   if (item[i][0] != 99) // not lit bomb
   {
      // check to see if the item is embedded in the wall
      int x = itemf[i][0];
      int y = itemf[i][1];

      if (players[p].left_right) // right
      {
         // is item embedded in a wall to the right?
         while (is_right_solid(x, y, 1, 1))
         {
            x--;
            wall_stuck++; // just how stuck was it?
         }
         itemf[i][0] = x;
      }

      if (!players[p].left_right) // left
      {
         // is item embedded in a wall to the left?
         while (is_left_solid(x, y, 1, 1))
         {
            x++;
            wall_stuck++; // just how stuck was it?
         }
         itemf[i][0] = x;
       }
   }
   else itemf[i][1] += 2; // when putting bomb in wall, this stops it from snapping to block above
   return wall_stuck;
}

void proc_player_carry(int p)
{
   if ((players[p].active) && (players[p].carry_item))
      if (!players[p].paused || (players[p].paused && players[p].paused_type == 2))// player is carrying item
      {
         int i = players[p].carry_item-1;  // item number

         if (!item[i][0]) players[p].carry_item = 0;                    // player is carrying item that is now inactive

         if ((item[i][0] == 98) || (item[i][0] == 99)) item[i][13] = p; // mark player carrying lit bomb or rocket

         if (item[i][0] != 98)            // not lit rocket
         {
            // set item position relative to player that's carrying it
            itemf[i][1] = players[p].y - 2;
            if (!players[p].left_right) itemf[i][0] = players[p].x - 15;
            if (players[p].left_right)  itemf[i][0] = players[p].x + 15;
         }
         if (!players[p].fire) // drop
         {
            int wall_stuck = player_drop_item(p);

            if (players[p].paused && players[p].paused_type == 2) // dropped item while in door travel
            {
               itemf[i][2] = 0;
               itemf[i][3] = 0;
            }
            else if (wall_stuck < 6)
            {
               if (item[i][0] != 98)            // not lit rocket
               {
                  itemf[i][2] = players[p].xinc;  // inherit the players momentum
                  itemf[i][3] = players[p].yinc;
                  if (players[p].up)    itemf[i][3] -= 6; // throw item upwards
                  if (players[p].down)  itemf[i][3] =  3; // throw item downwards
                  if (players[p].left)  itemf[i][2] -= 2; // throw item left
                  if (players[p].right) itemf[i][2] += 2; // throw item right
               }

               // prevent sticky bombs from sticking to the ground when throwing upwards
               if ((item[i][0] == 99) && (item[i][11]) && (players[p].up)) itemf[i][1] -= 2;

            }
         }
      }
}


void proc_start_collision(int p, int i)
{
   int ns = 0; // count number of starts
   int s[8] = {0};
   for (int i=0; i<500; i++)
      if (item[i][0] == 5)
      {
         ns++;
         s[item[i][7]] = i; // save index of this start
      }
/*
   if (ns == 0)
   {
      printf("Error: no start found.\n");
      players[p].x = 20;
      players[p].y = 20;;
   }

   if (ns == 1)
   {
      players[p].spawn_point_index = 0;
      int ps = s[players[p].spawn_point_index];
      players[p].x = itemf[ps][0];
      players[p].y = itemf[ps][1];
   }
*/


   if (ns > 1)
   {
      int mode = item[s[0]][6];

      if (mode == 2) // check point common
      {
         for (p=0; p<8; p++)
            players[p].spawn_point_index = item[i][7]; // set new spawn point for all players

         // mark this one as active and all others as not
         for (int ii=0; ii<500; ii++)
            if (item[ii][0] == 5)
            {
               if (item[ii][7] == item[i][7]) item[ii][1] = 1021;
               else item[ii][1] = 1011;
            }
      }

      if (mode == 3) // check point individual
      {
         players[p].spawn_point_index = item[i][7]; // set new spawn point for this player
      }

   }

   /*
   players[p].spawn_point_index = item[i][7]; // set new spawn point

   // mark this one as active and all others as not
   for (int ii=0; ii<500; ii++)
      if (item[ii][0] == 5)
      {
         if (item[ii][7] == item[i][7]) item[ii][1] = 1021;
         else item[ii][1] = 1011;
      }

*/


}

void proc_bonus_collision(int p, int i)
{
   int bonus_type = item[i][6];

   if (bonus_type == 1) // health bonus
   {
      if (players[p].health < 100)
      {
         item[i][0] = 0;
         players[p].health += item[i][7];
         if (players[p].health > 100) players[p].health = 100;
         game_event(72, 0, 0, p, i, item[i][1], item[i][7]);
      }
   }
   if (bonus_type == 3) // purple coin!!!
   {
      item[i][0] = 0;
      players[p].stat_purple_coins++;
      game_event(71, 0, 0, p, i, 0, 0);
   }
}

void proc_exit_collision(int p, int i)
{
   int exit_enemys_left = num_enemy - item[i][8];
   if (exit_enemys_left <= 0)
   {
      if (players[0].level_done_mode == 0)
      {
         players[0].level_done_mode = 9;
         players[0].level_done_timer = 0;
         players[0].level_done_x = itemf[i][0];
         players[0].level_done_y = itemf[i][1];
         players[0].level_done_player = p;
         players[0].level_done_next_level = mLevel.play_level + 1;
         game_event(4, 0, 0, 0, 0, 0, 0);
      }
   }
   else game_event(3, 0, 0, p, i, exit_enemys_left, 0); // not enough dead yet
}


void proc_mine_collision(int p, int i)
{
   players[p].health -= item[i][8] / 10;
   game_event(50, 0, 0, p, i, 0, item[i][8]);
}


void proc_warp_collision(int p, int i)
{
   if (players[0].level_done_mode == 0)
   {
      if (mLevel.play_level > 1) mLevel.warp_level_location = mLevel.play_level;
//      players[0].level_done_mode = 3;
//      players[0].level_done_timer = 0;

      players[0].level_done_mode = 2;
      players[0].level_done_timer = 0;


      players[0].level_done_x = itemf[i][0];
      players[0].level_done_y = itemf[i][1];
      players[0].level_done_player = p;
      players[0].level_done_next_level = item[i][8];
      game_event(4, 0, 0, p, i, 0, 0);
   }
}


void proc_sproingy_collision(int p, int i)
{
   float px = players[p].x;
   float py = players[p].y;
   float ix = itemf[i][0];
   float iy = itemf[i][1];
   if ( (px > ix-10) && (px < ix+10) && (py > iy-16) && (py < iy-8) &&
        (players[p].yinc > 0) && // falling
        (players[p].jump) )   //  jump pressed


   {
      game_event(31, 0, 0, p, i, 0, 0);
      players[p].yinc = 0 - (float) item[i][7] / 7.1;
   }
}

void proc_item_collision(int p, int i)
{
   // make it so any item other than bonus has higher priority
   // if carrying bonus, it will be dropped and new item will be carried
   int already_carrying = 0;
   if (players[p].carry_item) // already carrying item
   {
      already_carrying = 1;
      if ((item[players[p].carry_item][0] == 2) && (item[i][0] != 2)) // carried item is bonus and new item is not bonus
         already_carrying = 0;
   }

   // check if player can carry item
   if ( (!already_carrying)  // not carrying
     && (item[i][3] < 0)     // item is carryable
     && (players[p].fire) )  // fire pressed

   {
      // check to see if another player is already carrying this item
      int other_player_carrying = 0;
      for (int op=0; op<NUM_PLAYERS; op++)
         if ((players[op].active) && (!players[op].paused))
            if (players[op].carry_item == i+1)
               other_player_carrying = 1;

       // allow carry if no other player is carrying
       if (other_player_carrying == 0)
       {
          players[p].carry_item = i+1;
          //printf("player picked up item:%d\n", i);
       }
       // allow multiple player carry for rocket
       if (item[i][0] == 98) players[p].carry_item = i+1;
   }

   switch (item[i][0]) // item type
   {
      case 1:  proc_door_collision(p, i);     break;
      case 2:  proc_bonus_collision(p, i);    break;
      case 3:  proc_exit_collision(p, i);     break;
      case 4:  proc_key_collision(p, i);      break;
      case 5:  proc_start_collision(p, i);    break;
      case 6:  proc_orb_collision(p, i);      break;
      case 7:  proc_mine_collision(p, i);     break;
      case 8:  proc_bomb_collision(p, i);     break;
      case 10: proc_pmsg_reset_timer(i);      break;
      case 11: proc_rocket_collision(p, i);   break;
      case 12: proc_warp_collision(p, i);     break;
      case 14: proc_switch_collision(p, i);   break;
      case 15: proc_sproingy_collision(p, i); break;
   }
}


/*

item variable descriptions

list of items


[1]  - door
[2]  - bonus
[3]  - exit
[4]  - key
[5]  - start
[6]  - orb
[7]  - mine
[8]  - bomb
[9]  - trigger
[10] - pop-up msg
[11] - rocket
[12] - warp
[14] - switch
[15] - sproingy
[16] - block manip
[17] - block damage

[98] - lit rocket
[99] - lit bomb

// common
item[][0] = active and type
item[][1] = bitmap or ans
item[][2] = draw type (not used)
item[][3] = (0=stat, 1=fall, -1=carry, -2=carry through door)
item[][4] = x pos
item[][5] = y pos
item[][14] = time to live
item[][15] = tag with cloner item id

// type specific

[1] - door
item[][6]  color
item[][7]  move type (0=auto, 1=force instant, 2=force move)
item[][8]  type (0=exit only, 1=linked dest)
item[][9]  linked destination item
item[][10] key held flag
item[][11] door entry type (0=immed, 1=up, 2=down)
item[][12] draw lines always, never, only when touched
item[][13] base animation shape

[2] - bonus
item[][6] bonus type
1 - Health
2 - Free Man
3 - Purple Coin
item[][7] health bonus

[3] - exit
item[][8] exit with x enemies left

[4] - key
item[][6]  block range x
item[][7]  block range y
item[][8]  block range w
item[][9]  block range h
item[][10] rotation for key move
item[][11] counter for key move
item[][12] matching keyed blocks only

[5] - start
item[][6] mode
item[][7] start index

[6] - orb


[7] - mine
item[][8] mine damage

[8] - bomb
item[][6]  mode (2=explosion)
item[][7]  blast size
item[][8]  fuse length counter
item[][9]  fuse length value
item[][11]  sticky
item[][12]  timer / remote

[9]  - Trigger
item[][2] = draw_type (color)
item[][3] = flags
item[][6]  = trigger field x (2000)
item[][7]  = trigger field y (2000)
item[][8]  = trigger field w (2000)
item[][9]  = trigger field x (2000)
item[][10] = trigger field lift number
item[][11] = CURR ON  pm_event
item[][12] = CURR OFF pm_event
item[][13] = TGON pm_event #
item[][14] = TGOF pm_event #

[10] - pop-up msg
item[][6]  timer counter
item[][7]  timer value
item[][8]  text color
item[][9]  frame color
item[][10] msg x pos (100)
item[][11] msg y pos (100)

[11] - rocket
item[][6]  steerabaility
item[][7]  blast size
item[][8]  max speed
item[][9]  accel
item[][10] rocket rotation (scaled by 10)

[12]  - warp
item[][8] warp level

[14]  - switch
item[][6]  block range x
item[][7]  block range y
item[][8]  block range w
item[][9]  block range h
item[][10] color flags
item[][11]  lockout

[15] - sproingy
item[][6] jump length (only used in level editor)
item[][7] sproinginess

[16] - Block Manip
item[][1] = pm_event_trigger
item[][2] = draw on
item[][3] = mode
item[][6]  = trigger field x (2000)
item[][7]  = trigger field y (2000)
item[][8]  = trigger field w (2000)
item[][9]  = trigger field x (2000)
item[][10] block 1
item[][11] block 2
item[][12] = draw color

[17] - Block Damage
item[][6]  = field x (2000)
item[][7]  = field y (2000)
item[][8]  = field w (2000)
item[][9]  = field h (2000)
item[][10] = lift number
item[][11] = mode
item[][12] = t1 val
item[][13] = count
item[][14] = t2 val
item[][15] = damage


[98] - lit rocket

[99] - lit bomb
item[][6]  mode (1=lit, 2=explosion, 3=remote detonator)
item[][7]  blast size
item[][8]  fuse length counter
item[][9]  fuse length value

item[][11] sticky
item[][12] timer | remote
item[][13] last player to touch

*/
