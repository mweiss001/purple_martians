// mwItems.cpp

#include "pm.h"
#include "mwItems.h"
#include "z_sound.h"
#include "z_player.h"
#include "mwFont.h"
#include "mwBitmap.h"
#include "mwLift.h"
#include "mwColor.h"
#include "mwDisplay.h"
#include "mwProgramState.h"

#include "z_enemy.h"
#include "mwLevel.h"
#include "e_fnx.h"
#include "z_screen_overlay.h"
#include "z_solid.h"

#include "mwPMEvent.h"
#include "mwWindowManager.h"
#include "mwEventQueue.h"
#include "mwInput.h"
#include "mwWidgets.h"
#include "mwShots.h"
#include "z_screen.h"
#include "e_object_viewer.h"

mwItems mItem;

mwItems::mwItems()
{
   initialize();
}

void mwItems::initialize(void)
{

}



void mwItems::draw_item(int i, int custom, int cx, int cy)
{
   char msg[1024];
   int type = mItem.item[i][0];
   int x = itemf[i][0];
   int y = itemf[i][1];

   if (custom)
   {
      x = cx;
      y = cy;
   }

   int shape = mItem.item[i][1];                       // get shape
   if (shape > 999) shape = mwB.zz[0][shape-1000];   // ans
   int drawn = 0;




   if ((type == 10) && (!custom)) // pop up message
   {
      int timer_count=0, timer_val=0;
      get_int_3216(mItem.item[i][12], timer_count, timer_val);

      // if timer running or always show, draw the message
      if ((timer_count) || (mItem.item[i][2] & PM_ITEM_PMSG_SHOW_ALWAYS)) draw_pop_message(i, 0, 0, 0, 0, 0, msg);

      // if hide scroll and not running level editor flag scroll as being drawn already
      if ((!(mItem.item[i][2] & PM_ITEM_PMSG_SHOW_SCROLL)) && (!mwPS.level_editor_running)) drawn = 1;
   }




   if (type == 1)  { draw_door(i, x, y, custom);         drawn = 1; }
   if (type == 6)  { draw_orb(i, x, y);                  drawn = 1; }
   if (type == 9)  { draw_trigger(i, x, y);              drawn = 1; }
   if (type == 16) { draw_block_manip(i, x, y);          drawn = 1; }
   if (type == 17) { draw_block_damage(i, x, y, custom); drawn = 1; }

   if (type == 4) drawn = draw_key(i); // only draws here in last stage

   if ((type == 8) && (mItem.item[i][11]) ) al_draw_bitmap(mwB.tile[440], x, y, 0); // bomb sticky spikes

   if (type == 99)
   {
      draw_lit_bomb(i);
      if ((mItem.item[i][11]) && (mItem.item[i][6] != 2)) al_draw_bitmap(mwB.tile[440], x, y, 0);  // bomb sticky spikes
      drawn = 1;
   }


    // these types need rotation
   if ((type == 11) || (type == 98) ||  // rockets
      ((type == 4) && (mItem.item[i][11] > 0) && (!drawn)) ) // moving key
   {
      float rot = (float) mItem.item[i][10] / 1000;
      al_draw_rotated_bitmap(mwB.tile[shape], 10, 10, x+10, y+10, rot, 0);
      drawn = 1;
   }

   if (type == 98) draw_rocket_lines(i); // for lit rockets

   if (type == 5) // start
   {
      if (mLevel.number_of_starts > 1)
      {
         al_draw_bitmap(mwB.tile[shape], x, y, 0);
         al_draw_textf(mF.pixl, mC.pc[12], x+10, y-4, ALLEGRO_ALIGN_CENTER, "%d", mItem.item[i][7]);
         drawn = 1;
      }
   }

   if (type == 3) // exit
   {
      al_draw_bitmap(mwB.tile[399], x, y, 0); // 'exit' text not shown
      if (mwPS.frame_num % 60 > 30)
         al_draw_text(mF.pixl, mC.pc[10], x+11, y-2, ALLEGRO_ALIGN_CENTER, "EXIT");

      int exit_enemys_left = num_enemy - mItem.item[i][8];
      if (exit_enemys_left > 0) // locked
      {
         al_draw_bitmap(mwB.tile[366], x, y, 0); // show lock
         if (mwPS.frame_num % 60 < 30)
            al_draw_textf(mF.pixl, mC.pc[14], x+11, y-2, ALLEGRO_ALIGN_CENTER, "%d", exit_enemys_left);

      }
      drawn = 1;
   }

   if ((type == 2) && (mItem.item[i][6] == 3)) // purple coin custom draw
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
      if ((mItem.item[i][14]>10) && (type != 9) && (type != 16) && (type != 17))
         al_draw_textf(mF.pixl, mC.pc[15], x+10, y-10, ALLEGRO_ALIGN_CENTER, "%d", 1 + (mItem.item[i][14] - 10) / 40);
   }
}


void mwItems::draw_items(void)
{
   al_set_target_bitmap(mwB.level_buffer);

   for (int i=0; i<500; i++)
      if ((mItem.item[i][0]) && (mItem.item[i][0] != 10)) draw_item(i, 0, 0, 0);

   for (int i=0; i<500; i++) // draw msg last so they are on top...
      if (mItem.item[i][0] == 10) draw_item(i, 0, 0, 0);
}



int mwItems::is_item_stuck_to_wall(int i)
{
   int x = itemf[i][0];
   int y = itemf[i][1];
   if ( (is_left_solid(x,y, 0, 3)) || (is_right_solid(x,y, 0, 3)) || (is_down_solid(x,y, 0, 3)) || (is_up_solid(x,y, 0, 3)) ) return 1;
   return 0;
}

void mwItems::move_items()
{
   for (int i=0; i<500; i++)
      if (mItem.item[i][0])
      {
         int x = itemf[i][0];
         int y = itemf[i][1];
         if ((x<0) || (x>1980) || (y<0) || (y>1980)) mItem.item[i][0] = 0; // remove if out of bounds

         int type = mItem.item[i][0];
         if ((type == 4) && (mItem.item[i][11] > 0)) proc_moving_key(i);
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
            int ttl = mItem.item[i][14];
            if (ttl)
            {
               if (ttl < 11)       // start ans seq
               {
                  mItem.item[i][0] = 66; // change to different type to prevent use
                  int sq = 10-ttl;
                  mItem.item[i][1] = mwB.zz[5+sq][74];
               }
               if (ttl == 1) mItem.item[i][0] = 0; // kill instantly
               mItem.item[i][14]--;
            }
         }

         // not stationary and not lit rocket, trigger, bm, bd or moving key
         if ((mItem.item[i][3]) && (type != 98) && (type != 9) && (type != 16) && (type != 17) && (! ((type == 4) && (mItem.item[i][11] > 0)))    )
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
               if ((type == 99) && (mItem.item[i][11])) sticky = 1;
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

                        if (! ((type == 99) && (mItem.item[i][6] == 2))) // no gravity for exploding bomb
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

int mwItems::player_drop_item(int p)
{
   int i = players[p].carry_item-1; // number of item
   // printf("drop item:%d\n", i);

   int wall_stuck = 0;
   players[p].carry_item = 0;
   if (mItem.item[i][0] != 99) // not lit bomb
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

void mwItems::proc_player_carry(int p)
{
   if ((players[p].active) && (players[p].carry_item))
      if (!players[p].paused || (players[p].paused && players[p].paused_type == 2))// player is carrying item
      {
         int i = players[p].carry_item-1;  // item number

         if (!mItem.item[i][0]) players[p].carry_item = 0;                    // player is carrying item that is now inactive

         if ((mItem.item[i][0] == 98) || (mItem.item[i][0] == 99)) mItem.item[i][13] = p; // mark player carrying lit bomb or rocket

         if (mItem.item[i][0] != 98)            // not lit rocket
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
               if (mItem.item[i][0] != 98)            // not lit rocket
               {
                  itemf[i][2] = players[p].xinc;  // inherit the players momentum
                  itemf[i][3] = players[p].yinc;
                  if (players[p].up)    itemf[i][3] -= 6; // throw item upwards
                  if (players[p].down)  itemf[i][3] =  3; // throw item downwards
                  if (players[p].left)  itemf[i][2] -= 2; // throw item left
                  if (players[p].right) itemf[i][2] += 2; // throw item right
               }

               // prevent sticky bombs from sticking to the ground when throwing upwards
               if ((mItem.item[i][0] == 99) && (mItem.item[i][11]) && (players[p].up)) itemf[i][1] -= 2;

            }
         }
      }
}


void mwItems::proc_start_collision(int p, int i)
{
   int ns = 0; // count number of starts
   int s[8] = {0};
   for (int i=0; i<500; i++)
      if (mItem.item[i][0] == 5)
      {
         ns++;
         s[mItem.item[i][7]] = i; // save index of this start
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
      int mode = mItem.item[s[0]][6];

      if (mode == 2) // check point common
      {
         for (p=0; p<8; p++)
            players[p].spawn_point_index = mItem.item[i][7]; // set new spawn point for all players

         // mark this one as active and all others as not
         for (int ii=0; ii<500; ii++)
            if (mItem.item[ii][0] == 5)
            {
               if (mItem.item[ii][7] == mItem.item[i][7]) mItem.item[ii][1] = 1021;
               else mItem.item[ii][1] = 1011;
            }
      }

      if (mode == 3) // check point individual
      {
         players[p].spawn_point_index = mItem.item[i][7]; // set new spawn point for this player
      }

   }

   /*
   players[p].spawn_point_index = mItem.item[i][7]; // set new spawn point

   // mark this one as active and all others as not
   for (int ii=0; ii<500; ii++)
      if (mItem.item[ii][0] == 5)
      {
         if (mItem.item[ii][7] == mItem.item[i][7]) mItem.item[ii][1] = 1021;
         else mItem.item[ii][1] = 1011;
      }

*/


}

void mwItems::proc_bonus_collision(int p, int i)
{
   int bonus_type = mItem.item[i][6];

   if (bonus_type == 1) // health bonus
   {
      if (players[p].health < 100)
      {
         mItem.item[i][0] = 0;
         players[p].health += mItem.item[i][7];
         if (players[p].health > 100) players[p].health = 100;
         game_event(72, 0, 0, p, i, mItem.item[i][1], mItem.item[i][7]);
      }
   }
   if (bonus_type == 3) // purple coin!!!
   {
      mItem.item[i][0] = 0;
      players[p].stat_purple_coins++;
      game_event(71, 0, 0, p, i, 0, 0);
   }
}

void mwItems::proc_exit_collision(int p, int i)
{
   int exit_enemys_left = num_enemy - mItem.item[i][8];
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


void mwItems::proc_mine_collision(int p, int i)
{
   players[p].health -= mItem.item[i][8] / 10;
   game_event(50, 0, 0, p, i, 0, mItem.item[i][8]);
}


void mwItems::proc_warp_collision(int p, int i)
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
      players[0].level_done_next_level = mItem.item[i][8];
      game_event(4, 0, 0, p, i, 0, 0);
   }
}


void mwItems::proc_sproingy_collision(int p, int i)
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
      players[p].yinc = 0 - (float) mItem.item[i][7] / 7.1;
   }
}

void mwItems::proc_item_collision(int p, int i)
{
   // make it so any item other than bonus has higher priority
   // if carrying bonus, it will be dropped and new item will be carried
   int already_carrying = 0;
   if (players[p].carry_item) // already carrying item
   {
      already_carrying = 1;
      if ((mItem.item[players[p].carry_item][0] == 2) && (mItem.item[i][0] != 2)) // carried item is bonus and new item is not bonus
         already_carrying = 0;
   }

   // check if player can carry item
   if ( (!already_carrying)  // not carrying
     && (mItem.item[i][3] < 0)     // item is carryable
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
       if (mItem.item[i][0] == 98) players[p].carry_item = i+1;
   }

   switch (mItem.item[i][0]) // item type
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
mItem.item[][0] = active and type
mItem.item[][1] = bitmap or ans
mItem.item[][2] = draw type (not used)
mItem.item[][3] = (0=stat, 1=fall, -1=carry, -2=carry through door)
mItem.item[][4] = x pos
mItem.item[][5] = y pos
mItem.item[][14] = time to live
mItem.item[][15] = tag with cloner item id

// type specific

[1] - door
mItem.item[][6]  color
mItem.item[][7]  move type (0=auto, 1=force instant, 2=force move)
mItem.item[][8]  type (0=exit only, 1=linked dest)
mItem.item[][9]  linked destination item
mItem.item[][10] key held flag
mItem.item[][11] door entry type (0=immed, 1=up, 2=down)
mItem.item[][12] draw lines always, never, only when touched
mItem.item[][13] base animation shape

[2] - bonus
mItem.item[][6] bonus type
1 - Health
2 - Free Man
3 - Purple Coin
mItem.item[][7] health bonus

[3] - exit
mItem.item[][8] exit with x enemies left

[4] - key
mItem.item[][6]  block range x
mItem.item[][7]  block range y
mItem.item[][8]  block range w
mItem.item[][9]  block range h
mItem.item[][10] rotation for key move
mItem.item[][11] counter for key move
mItem.item[][12] matching keyed blocks only

[5] - start
mItem.item[][6] mode
mItem.item[][7] start index

[6] - orb


[7] - mine
mItem.item[][8] mine damage

[8] - bomb
mItem.item[][6]  mode (2=explosion)
mItem.item[][7]  blast size
mItem.item[][8]  fuse length counter
mItem.item[][9]  fuse length value
mItem.item[][11]  sticky
mItem.item[][12]  timer / remote

[9]  - Trigger
mItem.item[][2] = draw_type (color)
mItem.item[][3] = flags
mItem.item[][6]  = trigger field x (2000)
mItem.item[][7]  = trigger field y (2000)
mItem.item[][8]  = trigger field w (2000)
mItem.item[][9]  = trigger field x (2000)
mItem.item[][10] = trigger field lift number
mItem.item[][11] = CURR ON  pm_event
mItem.item[][12] = CURR OFF pm_event
mItem.item[][13] = TGON pm_event #
mItem.item[][14] = TGOF pm_event #

[10] - pop-up msg
mItem.item[][6]  timer counter
mItem.item[][7]  timer value
mItem.item[][8]  text color
mItem.item[][9]  frame color
mItem.item[][10] msg x pos (100)
mItem.item[][11] msg y pos (100)

[11] - rocket
mItem.item[][6]  steerabaility
mItem.item[][7]  blast size
mItem.item[][8]  max speed
mItem.item[][9]  accel
mItem.item[][10] rocket rotation (scaled by 10)

[12]  - warp
mItem.item[][8] warp level

[14]  - switch
mItem.item[][6]  block range x
mItem.item[][7]  block range y
mItem.item[][8]  block range w
mItem.item[][9]  block range h
mItem.item[][10] color flags
mItem.item[][11]  lockout

[15] - sproingy
mItem.item[][6] jump length (only used in level editor)
mItem.item[][7] sproinginess

[16] - Block Manip
mItem.item[][1] = pm_event_trigger
mItem.item[][2] = draw on
mItem.item[][3] = mode
mItem.item[][6]  = trigger field x (2000)
mItem.item[][7]  = trigger field y (2000)
mItem.item[][8]  = trigger field w (2000)
mItem.item[][9]  = trigger field x (2000)
mItem.item[][10] block 1
mItem.item[][11] block 2
mItem.item[][12] = draw color

[17] - Block Damage
mItem.item[][6]  = field x (2000)
mItem.item[][7]  = field y (2000)
mItem.item[][8]  = field w (2000)
mItem.item[][9]  = field h (2000)
mItem.item[][10] = lift number
mItem.item[][11] = mode
mItem.item[][12] = t1 val
mItem.item[][13] = count
mItem.item[][14] = t2 val
mItem.item[][15] = damage


[98] - lit rocket

[99] - lit bomb
mItem.item[][6]  mode (1=lit, 2=explosion, 3=remote detonator)
mItem.item[][7]  blast size
mItem.item[][8]  fuse length counter
mItem.item[][9]  fuse length value

mItem.item[][11] sticky
mItem.item[][12] timer | remote
mItem.item[][13] last player to touch

*/


/*
// e_item.cpp

#include "pm.h"
#include "mwDisplay.h"
#include "mwFont.h"
#include "mwColor.h"
#include "z_item.h"
#include "mwLevel.h"
#include "e_fnx.h"
#include "e_object_viewer.h"
#include "z_item_pmsg.h"
#include "mwInput.h"

*/

void mwItems::set_item_text(void)
{
   strcpy(item_name[0],  "item_empty");
   strcpy(item_name[1],  "Door");
   strcpy(item_name[2],  "Bonus");
   strcpy(item_name[3],  "Exit");
   strcpy(item_name[4],  "Key");
   strcpy(item_name[5],  "Start");
   strcpy(item_name[6],  "Orb");
   strcpy(item_name[7],  "Mine");
   strcpy(item_name[8],  "Bomb");
   strcpy(item_name[9],  "Trigger");
   strcpy(item_name[10], "Message");
   strcpy(item_name[11], "Rocket");
   strcpy(item_name[12], "Warp");
   strcpy(item_name[13], "undef");
   strcpy(item_name[14], "Switch");
   strcpy(item_name[15], "Sproingy");
   strcpy(item_name[16], "Block Manip");
   strcpy(item_name[17], "Block Damage");

   item_tile[0]  = 0;
   item_tile[1]  = 448;
   item_tile[2]  = 304;
   item_tile[3]  = 398;
   item_tile[4]  = 272;
   item_tile[5]  = 383;
   item_tile[6]  = 0;
   item_tile[7]  = 456;
   item_tile[8]  = 538;
   item_tile[9]  = 991;
   item_tile[10] = 256;
   item_tile[11] = 249;
   item_tile[12] = 825;
   item_tile[13] = 0;
   item_tile[14] = 745;
   item_tile[15] = 237;
   item_tile[16] = 989;
   item_tile[17] = 988;
   item_tile[18] = 0;
   item_tile[19] = 0;
}


int mwItems::item_data(int x, int y)
{
   char msg[1024];
   int inum = sort_item(0);
                               al_draw_textf(mF.pr8, mC.pc[12], x, y, 0, "%d Items",      inum);                 y+=8;
                               al_draw_textf(mF.pr8, mC.pc[12], x, y, 0, "--------");                            y+=8;
                               al_draw_textf(mF.pr8, mC.pc[13], x, y, 0, "%d Starts",     item_num_of_type[5]);  y+=8;
                               al_draw_textf(mF.pr8, mC.pc[13], x, y, 0, "%d Exits",      item_num_of_type[3]);  y+=8;
   if (item_num_of_type[12]) { al_draw_textf(mF.pr8, mC.pc[10], x, y, 0, "%d Warps",      item_num_of_type[12]); y+=8; }
   if (item_num_of_type[1])  { al_draw_textf(mF.pr8, mC.pc[13], x, y, 0, "%d Doors",      item_num_of_type[1]);  y+=8; }
   if (item_num_of_type[4])  { al_draw_textf(mF.pr8, mC.pc[13], x, y, 0, "%d Keys",       item_num_of_type[4]);  y+=8; }
   if (item_num_of_type[14]) { al_draw_textf(mF.pr8, mC.pc[13], x, y, 0, "%d Switches",   item_num_of_type[14]); y+=8; }
   if (item_num_of_type[15]) { al_draw_textf(mF.pr8, mC.pc[13], x, y, 0, "%d Sproingies", item_num_of_type[15]); y+=8; }
   if (item_num_of_type[8])  { al_draw_textf(mF.pr8, mC.pc[14], x, y, 0, "%d Bombs",      item_num_of_type[8]);  y+=8; }
   if (item_num_of_type[11]) { al_draw_textf(mF.pr8, mC.pc[14], x, y, 0, "%d Rockets",    item_num_of_type[11]); y+=8; }
   if (item_num_of_type[7])  { al_draw_textf(mF.pr8, mC.pc[14], x, y, 0, "%d Mines",      item_num_of_type[7]);  y+=8; }
   if (item_num_of_type[6])  { al_draw_textf(mF.pr8, mC.pc[14], x, y, 0, "%d Orbs",       item_num_of_type[6]);  y+=8; }

   for (int c=1; c<20; c++)
      if ((c!= 1) && (c !=3) && (c!= 4) && (c != 5) && (c != 6) && (c!= 7) && (c!= 8) && (c!= 9) && (c!= 11) && (c!= 12) && (c!= 14) && (c!= 15) && (c!= 16) && (c!= 17) )
         if (item_num_of_type[c]) // not zero
         {
                         sprintf(msg, "%d type %d???  ", item_num_of_type[c], c); // default unknown
            if (c ==  0) sprintf(msg, "%d type 0      ", item_num_of_type[c]);
            if (c ==  2) sprintf(msg, "%d Bonus       ", item_num_of_type[c]);
            if (c == 10) sprintf(msg, "%d Messages    ", item_num_of_type[c]);
            al_draw_text(mF.pr8, mC.pc[3], x, y, 0, msg);
            y+=8;
         }
   return y;
}

void mwItems::show_all_items(void)
{
   char msg[1024];
   ALLEGRO_BITMAP *tmp;
   tmp = al_create_bitmap(20, 20);

   sort_item(1);
   al_set_target_backbuffer(display);
   al_clear_to_color(al_map_rgb(0,0,0));

   test_items();
   int text_pos = 0;

   int num_items = sort_item(1);
   text_pos = item_data(10, text_pos);

   int rh = 10; // row height

   for (int i=0; i<num_items; i++)
   {
      al_set_target_bitmap(tmp);
      al_clear_to_color(al_map_rgb(0,0,0));
      draw_item(i, 1, 0, 0);
      al_set_target_backbuffer(display);
      al_draw_scaled_bitmap(tmp, 0, 0, 20, 20, 0, text_pos, rh, rh, 0);

      sprintf(msg,"item:[%2d] ",i );
      for (int j=0; j<16; j++)
      {
         char msg2[80];
         //sprintf(msg2,"%d[%d] ", j, mItem.item[i][j]);
         sprintf(msg2,"[%d] ", mItem.item[i][j]);
         if (j == 0) sprintf(msg2,"[%-2d] ", mItem.item[i][j]);
         if (j == 1) sprintf(msg2,"[%-4d] ", mItem.item[i][j]);
         if (j == 3) sprintf(msg2,"[%-2d] ", mItem.item[i][j]);
         if (j == 4) sprintf(msg2,"[%-4d] ", mItem.item[i][j]);
         if (j == 5) sprintf(msg2,"[%-4d] ", mItem.item[i][j]);
         strcat(msg, msg2);
      }
      al_draw_text(mF.pr8, mC.pc[13], rh+2, text_pos+(rh-8)/2, 0, msg);
      text_pos +=rh;
      if (text_pos > mwD.SCREEN_H - 10)
      {
         al_flip_display();
         mI.tsw(); // wait for keypress
         al_clear_to_color(al_map_rgb(0,0,0));
         text_pos = 0;
      }
   }
   al_flip_display();
   mI.tsw(); // wait for keypress
   al_destroy_bitmap(tmp);
}

int mwItems::sort_item(int set_pos)
{
   char msg[1024];
   // to not break linked items
   for (int c=0; c < 500; c++)
      if (mItem.item[c][0] == 1) //door
         mItem.item[c][15] = c; // tag this door with its original item number
   int inum, c, d, quit, temp, iswap;
   quit=0;
   while (!quit) // sort item list
   {
      quit=1; // quit if no iswap
      for (c=0; c < 499; c++)
      {
         if (set_pos)
         {
            itemf[c][0] = mItem.item[c][4];
            itemf[c][1] = mItem.item[c][5];
         }


         if (mItem.item[c][0] < mItem.item[c+1][0]) // sort by first value 'type'
            iswap = 1;
//         else if (mItem.item[c][0] == mItem.item[c+1][0]) // if type is the same
//            if (mItem.item[c][1] < mItem.item[c+1][1]) // sort by 2nd value 'ans'
//               iswap =1;

         if (iswap)
         {
            quit=0;      // as long as a iswap has been made
            iswap = 0;
            if ((mItem.item[c][0] == 10) && (mItem.item[c+1][0] == 10)) // both messages
            {
               strcpy(msg, pmsgtext[c]);
               strcpy(pmsgtext[c], pmsgtext[c+1]);
               strcpy(pmsgtext[c+1], msg);
            }
            if ((mItem.item[c][0] == 10) && (mItem.item[c+1][0] != 10)) // first only
            {
               strcpy(pmsgtext[c+1], pmsgtext[c]);
               pmsgtext[c][0] = 0;
            }
            if ((mItem.item[c][0] != 10) && (mItem.item[c+1][0] == 10)) // second only
            {
               strcpy(pmsgtext[c], pmsgtext[c+1]);
               pmsgtext[c+1][0] = 0;
            }
            for (d=0; d<16; d++)
            {
               temp = mItem.item[c][d];
               mItem.item[c][d] = mItem.item[c+1][d];
               mItem.item[c+1][d] = temp;
            }
            for (d=0; d<4; d++)
            {
               float ftemp = itemf[c][d];
               itemf[c][d] = itemf[c+1][d];
               itemf[c+1][d] = ftemp;
            }
         }
      }
   }

   // get data about first 30 item types
   // and make sub lists of item types using these variables
   inum = 0;    // zero the counters
   for (c=0; c<30; c++)
   {
      item_num_of_type[c] = 0;
      item_first_num[c] = 0;
   }

   for (c=0; c<500; c++) // get the type counts
   {
      item_num_of_type[mItem.item[c][0]]++; // inc number of this type
      if (mItem.item[c][0]) inum++;
   }
   for (c=0; c<30; c++)             // get first nums
      if (item_num_of_type[c] > 0)  // are there any of this type?
         for (d=0; d<500; d++)
            if (mItem.item[d][0] == c)
            {
               item_first_num[c] = d;
               d=500;   // exit loop
            }
   // to not break linked items
   for (int c=0; c < 500; c++)
      if ((mItem.item[c][0] == 1) && (mItem.item[c][8] == 1))           // if door && exit mode == link
      {
         int link = mItem.item[c][9];                             // look for doors with that link in 15
         for (int d=0; d < 500; d++)
            if ((mItem.item[d][0] == 1) && (mItem.item[d][15] == link)) // found a door with link in 15
               mItem.item[c][9] = d;                              // update to new link number
      }

   // erase what we put in 15
   for (int c=0; c < 500; c++)
      if (mItem.item[c][0] == 1)     // door
         mItem.item[c][15] = 0;

   // set number of starts...
   mLevel.number_of_starts = item_num_of_type[5];


   return inum;
}

int mwItems::get_empty_item(void) // just find me an empty
{
   int mt = -1;
   for (int i=0; i<500; i++)
      if (mItem.item[i][0] == 0)
      {
         mt = i;
         break;
      }
   if (mt == -1) al_show_native_message_box(display, "Error", "Error creating item", "Item list full!", NULL, ALLEGRO_MESSAGEBOX_ERROR);
   return mt;
}

int mwItems::get_empty_item(int type) // finds, sets type, sorts, refinds
{
   int mt = get_empty_item();
   if (mt == -1) return 500;
   else
   {
      erase_item(mt);
      mItem.item[mt][0] = type; // set type
      mItem.item[mt][9] = 9999; // mark to find after sort !!
      sort_item(1);
      mt = 0;
      while ((mt < 500) && (mItem.item[mt][9] != 9999)) mt++;
      mItem.item[mt][9] = 0; // remove mark
   }
   return mt;
}

// not used!!!!! too much hassle....
void mwItems::check_item(int i, int ct)
{

   // range check for key, trig, manip, damage
   // probably wont use for trig, manip, damage, because they can be slaved to a lift and go off screen
   if (ct == 1)
   {
      if (mItem.item[i][6] > 1980)
      {
         mItem.item[i][6] = 1980;
         mItem.item[i][8] = 2000;
      }
      if (mItem.item[i][7] > 1980)
      {
         mItem.item[i][7] = 1980;
         mItem.item[i][9] = 2000;
      }
      if (mItem.item[i][6] < 0)
      {
         mItem.item[i][8] += mItem.item[i][6]+20; // first adjust width
         mItem.item[i][6] = 0;              // then adjust x
      }
      if (mItem.item[i][7] < 0)
      {
         mItem.item[i][9] += mItem.item[i][7]+20; // first adjust height
         mItem.item[i][7] = 0;              // then adjust y
      }

      // w and h
      if (mItem.item[i][6] + mItem.item[i][8] > 2000) mItem.item[i][8] = 2000-mItem.item[i][6];
      if (mItem.item[i][7] + mItem.item[i][9] > 2000) mItem.item[i][9] = 2000-mItem.item[i][7];
   }
}






void mwItems::test_items(void)
{
   for (int c=0; c<500; c++)
      if (mItem.item[c][0])
      {
         int type = mItem.item[c][0];
         // first check for valid type
         int good = 0;
         if (type == 1) good = 1;
         if (type == 2) good = 1;
         if (type == 3) good = 1;
         if (type == 4) good = 1;
         if (type == 5) good = 1;
         if (type == 7) good = 1;
         if (type == 8) good = 1;
         if (type == 9) good = 1;
         if (type == 10) good = 1;
         if (type == 11) good = 1;
         if (type == 12) good = 1;
         if (type == 14) good = 1;
         if (type == 15) good = 1;
         if (type == 16) good = 1;
         if (type == 17) good = 1;


         if (!good)  printf("Item:%d - bad type:%d\n", c, mItem.item[c][0]);
         // if (!good)  mItem.item[c][0] = 0; //erase bad type
         if (good)
         {
            if ( ((mItem.item[c][1] < 95) || (mItem.item[c][1] > 1084)) && (type != 9) && (type != 16) && (type != 17) )
               printf("Item:%d - bad shape:%d\n", c, mItem.item[c][1]);
            int xpos = mItem.item[c][4];
            if ((xpos < 20) || (xpos > 1960))
                printf("Item:%d - bad xpos%d\n", c, xpos);
            int ypos = mItem.item[c][5];
            if ((ypos < 20) || (ypos > 1960))
                printf("Item:%d - bad ypos%d\n", c, ypos);
         }
      }
}

void mwItems::erase_item(int num)
{
   if (mItem.item[num][0] == 10) pmsgtext[num][0] = 0;
   for (int x=0; x<16; x++) mItem.item[num][x] = 0;
}

int mwItems::create_trigger(int i)
{
   int bad = 0;
   // set the item location
   if (getxy("Trigger", 2, 9, i) == 1)
   {
      mItem.item[i][0] = 9;  // type 9 - trigger
      mItem.item[i][2] = 14; // draw color
      mItem.item[i][3] |= PM_ITEM_TRIGGER_PLAYER;
      mItem.item[i][3] |= PM_ITEM_TRIGGER_DRAW_ON;
   }
   else bad = 1;
   // then set the block range
   if (!bad)
   {
      if (!get_block_range("Trigger Rectangle", &mItem.item[i][6], &mItem.item[i][7], &mItem.item[i][8], &mItem.item[i][9], 1)) bad = 1;
   }
   if (bad) return 0;
   else object_viewer(2, i);
   return 1;
}


int mwItems::create_block_manip(int i)
{
   int bad = 0;

   // set the item location
   if (getxy("Block Manip Object", 2, 16, i) == 1)
   {
      mItem.item[i][0] = 16; // type 16 - block manip
      mItem.item[i][2] = 1; // draw mode on
      mItem.item[i][12] = 12; // default color
   }
   else bad = 1;
   // then set the block range
   if (!bad)
   {
      if (!get_block_range("Block Manip Rectangle", &mItem.item[i][6], &mItem.item[i][7], &mItem.item[i][8], &mItem.item[i][9], 1)) bad = 1;
   }
   if (bad) return 0;
   else object_viewer(2, i);
   return 1;
}

int mwItems::create_block_damage(int i)
{
   int bad = 0;

   // set the item location
   if (getxy("Block Damage Object", 2, 17, i) == 1)
   {
      mItem.item[i][0] = 17; // type 16 - block damage
      mItem.item[i][2] = 1;  // draw type
      mItem.item[i][3] |= PM_ITEM_DAMAGE_CURR;
      mItem.item[i][12] = 100;
      mItem.item[i][13] = 0;
      mItem.item[i][14] = 100;
   }
   else bad = 1;
   // then set the block range
   if (!bad)
   {
      if (!get_block_range("Block Damage Rectangle", &mItem.item[i][6], &mItem.item[i][7], &mItem.item[i][8], &mItem.item[i][9], 1)) bad = 1;
   }
   if (bad) return 0;
   else object_viewer(2, i);
   return 1;
}


int mwItems::create_start_block(int c)
{
   mItem.item[c][0] = 5 ;           // type 5 - start
   mItem.item[c][1] = 1021;         // default animation seq
   mItem.item[c][2] = 1;            // draw mode
   mItem.item[c][3] = 0;            // stationary
   mItem.item[c][6] = 2;            // mode = checkpoint common

   // find highest existing start index
   int hesi = -1;
   for (int x=0; x<500; x++)
      if ((mItem.item[x][0] == 5) && (x != c))
         if (mItem.item[x][7] > hesi) hesi = mItem.item[x][7];
   mItem.item[c][7] = hesi + 1;     // start index

   if (getxy("Start", 2, 5, c) == 1) return 1;
   else return 0;
}

int mwItems::create_exit(int c)
{
   mItem.item[c][0] = 3 ;           // type 3 - exit
   mItem.item[c][1] = 1022;         // default animation seq
   mItem.item[c][2] = 1;            // draw mode
   mItem.item[c][3] = 0;            // stationary



   mItem.item[c][8] = 100;          // num enemies left alive to activate this exit
   if (getxy("Exit", 2, 3, c) == 1) // xorg, yorg
   {
      mLevel.l[mItem.item[c][4]/20][mItem.item[c][5]/20] = 0; // make sure empty block in that pos
      return 1;
   }
   else return 0;
}

void mwItems::show_all_pmsg(void)
{
   int text_pos = 0;
   al_set_target_backbuffer(display);
   al_clear_to_color(al_map_rgb(0,0,0));

   al_draw_text(mF.pr8, mC.pc[15], 20, text_pos, 0, "List of pop messages:");
   text_pos +=8;

   for (int i=0; i<500; i++)
   {
      int len = strlen(pmsgtext[i]);
      if (len > 0)
      {
         // count lines and max line length
         int lines = 1;
         int mll = 0; // max line length
         int tlc = 0; // temp line counter
         for (int j=0; j<len; j++)
         {
            if (pmsgtext[i][j] == 10)
            {
               lines++;
               if (tlc > mll) mll = tlc;
               tlc = 0;
            }
            else tlc++;
         }
         if (lines == 1) mll = tlc;

         al_draw_textf(mF.pr8, mC.pc[14], 20, text_pos+=8, 0, "Item:%-2d len:%-3d lines:%-2d max length:%-2d", i, len, lines, mll);

         for (int j=0; j<len; j++)
         {
            int col = 15;
            if ((pmsgtext[i][j] < 32) || (pmsgtext[i][j] > 126)) col = 10; // bad char
            if (pmsgtext[i][j] == 10) col = 9; // LF
            al_draw_textf(mF.pr8, mC.pc[col], 20, text_pos+=8, 0, "[%2d][%3d] - %c", j, pmsgtext[i][j], pmsgtext[i][j] );

            if (text_pos > mwD.SCREEN_H - 20)
            {
               al_flip_display();
               mI.tsw(); // wait for keypress
               al_clear_to_color(al_map_rgb(0,0,0));
               text_pos = 0;
            }
         }
         text_pos +=8;
      }
   }
   al_flip_display();
   mI.tsw(); // wait for keypress
}

int mwItems::create_pmsg(int c)
{
   mItem.item[c][0] = 10 ;  // type 10 - msg
   mItem.item[c][1] = 1036; // animation seq
   mItem.item[c][3] = 0;    // stationary
   mItem.item[c][2] = 0;    // flags
   mItem.item[c][2] |= PM_ITEM_PMSG_SHOW_SCROLL;
   mItem.item[c][2] |= PM_ITEM_PMSG_FRAME12;

   mItem.item[c][12] = 120;  // default message time
   set_int_3216(mItem.item[c][13], 15, 13); // default text color (white) and frame color (blue)

   int bad=0;

   if (getxy("Message Object", 2, 10, c) != 1) bad = 1;

   int x=0, y=0, w=0, h=0;
   if (!bad) get_block_range("Message Area", &x, &y, &w, &h, 1);
   set_int_3216(mItem.item[c][10], x, y);
   set_int_3216(mItem.item[c][11], w, h);

   if (!bad) if (!edit_pmsg_text(c, 1)) bad = 1; // get text of message

   if (bad) return 0;
   else object_viewer(2, c);
   return 1;
}

int mwItems::create_door(int type)
{
   switch (type)
   {

    case 2: // one way door with linked item exit
      {
         // first check to see if we can find 2 empty items
         int found_empty_items = 0;
         int c, d;
         c = get_empty_item(); // get 1st item
         if (c != -1)
         {
            mItem.item[c][0] = 1; // mark it so we don't find it as empty again
            d = get_empty_item(); // get 2nd item
            if (d != -1) found_empty_items = 1;
            else mItem.item[c][0] = 0;
         }

         if (found_empty_items)
         {
            if (getxy("Door", 2, 1, c) == 1)
            {

               mItem.item[c][0] = 1;    // type 1
               mItem.item[c][13] = 1083;    // base draw shape or ans
               mItem.item[c][6] = 13;      // default color = lt blue
               mItem.item[c][1] = 1083;     // current shape

               mItem.item[c][2] = 1;    // draw mode normal
               mItem.item[c][3] = 0;    // stationary
               mItem.item[c][8] = 1;    // type: linked exit

               mItem.item[c][11] = 1;    // trigger with up
               mItem.item[c][12] = 1;    // always draw line
               if (getxy("Destination Door", 2, 1, d) == 1)
               {
                  mItem.item[c][9] = d;    // linked exit

                  mItem.item[d][0] = 1;    // type 1

                  mItem.item[d][13] = 1083;    // base draw shape or ans
                  mItem.item[d][1] = 1083;     // current shape
                  mItem.item[d][6] = 6;      // default color = tan

                  mItem.item[d][2] = 1;    // draw mode normal
                  mItem.item[d][3] = 0;    // stationary
                  mItem.item[d][8] = 0;    // type: exit only
               } // end of get destination without cancel
               else mItem.item[c][0] = 0;

             } // end of get location without cancel
             else mItem.item[c][0] = 0;
          } // end of found empty items
      }
      break;
      case 3: // two way door set
      {
         int found_empty_items = 0;
         int c, d;

         // first check to see if we can find 2 empty items
         c = get_empty_item(); // get 1st item
         if (c != -1)
         {
            mItem.item[c][0] = 1; // mark it so we don't find it as empty again
            d = get_empty_item(); // get 2nd item
            if (d != -1) found_empty_items = 1;
            else mItem.item[c][0] = 0;
         }


         if (found_empty_items)
         {
            if (getxy("1st Door", 2, 1, c) == 1)
            {

               mItem.item[c][0] = 1;    // type 1

               mItem.item[c][13] = 1083;    // base draw shape or ans
               mItem.item[c][1] = 1083;     // current shape
               mItem.item[c][6] = 13;      // default color = lt blue

               mItem.item[c][2] = 1;    // draw mode normal
               mItem.item[c][3] = 0;    // stationary
               mItem.item[c][8] = 1;    // type: linked exit

               mItem.item[c][11] = 1;    // trigger with up
               mItem.item[c][12] = 1;    // always draw line

               if (getxy("2nd Door", 2, 1, d) == 1)
               {
                  mItem.item[c][9] = d;    // linked exit

                  mItem.item[d][0] = 1;    // type 1
                  mItem.item[d][2] = 1;    // draw mode normal
                  mItem.item[d][3] = 0;    // stationary
                  mItem.item[d][8] = 1;    // type: linked exit
                  mItem.item[d][9] = c;    // linked exit

                  mItem.item[d][11] = 1;    // trigger with up
                  mItem.item[d][12] = 1;    // always draw line

                  mItem.item[d][13] = 1083;    // base draw shape or ans
                  mItem.item[d][1] = 1083;     // current shape
                  mItem.item[d][6] = 13;      // default color = lt blue

               } // end of get destination without cancel
               else erase_item(c);
            } // end of get location without cancel
            else erase_item(c);
         } // end of found empty items
      }
      break;
   }
   return sort_item(1);
}

int mwItems::create_item(int type)
{
   // check for no creator
   if ((type != 1) && (type != 3) /*&& (type != 4)*/ && (type != 5) && (type != 9) && (type != 10) && (type != 16) && (type != 17)) return 9999;
   int i = get_empty_item(type); // get a place to put it
   if (i > 499) return i; // no items
   switch (type)
   {
      case 3:  if (!create_exit(i))         erase_item(i); break;
      case 5:  if (!create_start_block(i))  erase_item(i); break;
      case 9:  if (!create_trigger(i))      erase_item(i); break;
      case 10: if (!create_pmsg(i))         erase_item(i); break;
      case 16: if (!create_block_manip(i))  erase_item(i); break;
      case 17: if (!create_block_damage(i)) erase_item(i); break;
   }
   sort_item(1);
   i = item_first_num[type]+item_num_of_type[type]-1;
   return i;
}


