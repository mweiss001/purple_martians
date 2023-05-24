// mwItem.cpp

#include "pm.h"
#include "mwItem.h"
#include "mwSound.h"
#include "mwPlayer.h"
#include "mwFont.h"
#include "mwBitmap.h"
#include "mwLift.h"
#include "mwColor.h"
#include "mwDisplay.h"
#include "mwLoop.h"

#include "mwEnemy.h"
#include "mwLevel.h"
#include "mwMiscFnx.h"
#include "mwGameEvent.h"

#include "mwSolid.h"

#include "mwTriggerEvent.h"
#include "mwWindowManager.h"
#include "mwEventQueue.h"
#include "mwInput.h"
#include "mwWidget.h"
#include "mwShot.h"
#include "mwScreen.h"

mwItem mItem;

mwItem::mwItem()
{
   initialize();
}

void mwItem::initialize(void)
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
   strcpy(item_name[13], "Timer");
   strcpy(item_name[14], "Switch");
   strcpy(item_name[15], "Sproingy");
   strcpy(item_name[16], "Block Manip");
   strcpy(item_name[17], "Block Damage");
   strcpy(item_name[18], "Gate");

   item_tile[0]  = 0;
   item_tile[1]  = 448;
   item_tile[2]  = 304;
   item_tile[3]  = 398;
   item_tile[4]  = 272;
   item_tile[5]  = 383;
   item_tile[6]  = 418;
   item_tile[7]  = 456;
   item_tile[8]  = 538;
   item_tile[9]  = 991;
   item_tile[10] = 256;
   item_tile[11] = 249;
   item_tile[12] = 825;
   item_tile[13] = 987;
   item_tile[14] = 745;
   item_tile[15] = 237;
   item_tile[16] = 989;
   item_tile[17] = 988;
   item_tile[18] = 940;
   item_tile[19] = 0;

}

// does the item type have a secondary position at 6,7?
int mwItem::item_secondary67(int type)
{
   if (type == 4)  return 1; // key
   if (type == 9)  return 1; // trigger
   if (type == 10) return 1; // message
   if (type == 13) return 1; // timer display
   if (type == 14) return 1; // switch
   if (type == 16) return 1; // block manip
   if (type == 17) return 1; // block damage
   return 0;
}


int mwItem::item_secondary67_hires(int type)
{
   if (type == 9)  return 1; // trigger
   if (type == 10) return 1; // message
   if (type == 13) return 1; // timer display
   if (type == 17) return 1; // block damage
   return 0;
}




void mwItem::draw_items(void)
{
   al_set_target_bitmap(mBitmap.level_buffer);
   for (int i=0; i<500; i++)
      if ((item[i][0]) && (item[i][0] != 10)) draw_item(i, 0, 0, 0);
   for (int i=0; i<500; i++) // draw msg last so they are on top
      if (item[i][0] == 10) draw_item(i, 0, 0, 0);
}

void mwItem::draw_item(int i, int custom, int cx, int cy)
{
   int type = item[i][0];
   int shape = item[i][1];                               // get shape
   if (shape > 999) shape = mBitmap.zz[0][shape-1000];   // ans
   int x = itemf[i][0];
   int y = itemf[i][1];
   if (custom)
   {
      x = cx;
      y = cy;
   }
   int drawn = 0;
   if (type == 1)  drawn = draw_door         (i, x, y, custom);
   if (type == 2)  drawn = draw_bonus        (i, x, y, shape);
   if (type == 3)  drawn = draw_exit         (i, x, y, shape);
   if (type == 4)  drawn = draw_key          (i, x, y, shape);
   if (type == 5)  drawn = draw_start        (i, x, y, shape);
   if (type == 6)  drawn = draw_orb          (i, x, y);
   if (type == 8)  drawn = draw_bomb         (i, x, y, shape);
   if (type == 9)  drawn = draw_trigger      (i, x, y, custom);
   if (type == 10) drawn = draw_message      (i, x, y, custom);
   if (type == 11) drawn = draw_rocket       (i, x, y, shape);
   if (type == 13) drawn = draw_timer        (i, x, y, custom);
   if (type == 16) drawn = draw_block_manip  (i, x, y);
   if (type == 17) drawn = draw_block_damage (i, x, y, custom);
   if (type == 18) drawn = draw_gate         (i, x, y, shape);

   if (type == 98) drawn = draw_rocket       (i, x, y, shape);
   if (type == 99) drawn = draw_lit_bomb     (i);

   // default draw if nothing else has drawn it up to now
   if (!drawn) al_draw_bitmap(mBitmap.tile[shape], x, y, 0);

   // if item is expiring show how many seconds left it has
   if (!mLoop.level_editor_running)
   {
      if ((item[i][14]>10) && (type != 9) && (type != 13) && (type != 16) && (type != 17))
         al_draw_textf(mFont.pixl, mColor.pc[15], x+10, y-10, ALLEGRO_ALIGN_CENTER, "%d", 1 + (item[i][14] - 10) / 40);
   }
}

int mwItem::draw_bonus(int i, int x, int y, int shape)
{
   if ((item[i][6] == 3) && (!mLoop.level_editor_running)) // purple coin custom draw
   {
      mBitmap.spin_shape(shape, x, y, 0, 0, 19, 19, 0.8, 0.5, 40);
      return 1;
   }
   return 0;
}


void mwItem::move_items()
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
         if (type == 13) proc_timer(i);
         if (type == 16) proc_block_manip(i);
         if (type == 17) proc_block_damage(i);
         if (type == 99) proc_lit_bomb(i);
         if (type == 98) proc_lit_rocket(i);


         // check for time to live
         if ((type != 9) && (type != 13) && (type != 16) && (type != 17) && (type != 20))
         {
            int ttl = item[i][14];
            if (ttl)
            {
               if (ttl < 11)       // start ans seq
               {
                  item[i][0] = 66; // change to different type to prevent use
                  int sq = 10-ttl;
                  item[i][1] = mBitmap.zz[5+sq][74];
               }
               if (ttl == 1) item[i][0] = 0; // kill instantly
               item[i][14]--;
            }
         }

         // not stationary and not lit rocket, trigger, bm, bd or moving key
         if ((item[i][3]) && (type != 98) && (type != 9) && (type != 13) && (type != 16) && (type != 17) && (type != 20) && (! ((type == 4) && (item[i][11] > 0)))    )
         {
            // check if being carried
            int pc = 0;
            for (int p=0; p<NUM_PLAYERS; p++)
               if (mPlayer.syn[p].active)
                  if ((!mPlayer.syn[p].paused) || (mPlayer.syn[p].paused && mPlayer.syn[p].paused_type == 2))
                     if (i == (mPlayer.syn[p].carry_item-1)) pc = 1;

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
                  if ((itemf[i][2] > 0) && (mSolid.is_right_solid(x,y, 1, 3)))
                  {
                     if (!sticky) itemf[i][0] -= itemf[i][2];  // take back xinc
                     itemf[i][2] = 0;                           // stop
                  }

                  // moving left and hit wall
                  if ((itemf[i][2] < 0) && (mSolid.is_left_solid(x,y, 1, 3)))
                  {
                     if (!sticky) itemf[i][0] -= itemf[i][2];  // take back xinc
                     itemf[i][2] = 0;                           // stop
                  }

                  x = itemf[i][0];
                  y = itemf[i][1];

                  // moving up
                  if (itemf[i][3] < 0)
                  {
                     if (mSolid.is_up_solid(x, y, 0, 3) == 1)    // only check for solid blocks
                        itemf[i][3] = 0;                 // if collision kill upwards yinc
                     else itemf[i][3] += .1;             // else de-accel
                  }
                  else // not moving up
                  {
                     int a = mSolid.is_down_solid(x, y, 1, 3);             // check for block below
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

                        if (mLift.cur[a-32].yinc < 0) // lift is moving up
                        {
                           int offset = mLift.cur[a-32].y - y;   // to prevent lift from picking up early when lift going up
                           if (offset < 21) capture = 1;
                        }
                        if (mLift.cur[a-32].yinc >= 0) // lift is moving down or steady
                        {
                           if (mSolid.is_down_solid(x, y, 0, 3)) capture = 0; // to prevent lift attempting to take item down through solid block
                           else capture = 1;
                           int offset = mLift.cur[a-32].y - y;   // to prevent lift from picking up early when item going down
                           if (offset > 21) capture = 0;
                        }
                        if (capture)
                        {
                           float lxi = mLift.cur[a-32].xinc;
                           float lyi = mLift.cur[a-32].yinc;

                           itemf[i][0] += lxi;                 // move x with lift's xinc
                           itemf[i][1]  = mLift.cur[a-32].y - 20;  // align with lift's y

                           x = itemf[i][0];
                           y = itemf[i][1];

                           if (lyi > 0) // down
                              if (mSolid.is_down_solid(x, y, 0, 3))       // no lift check
                                 itemf[i][1] = (y - (y % 20));    // item not on lift anymore, align with block

                           if (lyi < 0) // up
                              if (mSolid.is_up_solid(x, y, 0, 3) == 1)
                                 itemf[i][1] += 10;

                           if (lxi > 0) // right
                              if (mSolid.is_right_solid(x, y, 1, 3))
                                 itemf[i][0] -= lxi;

                           if (lxi < 0) // left
                              if (mSolid.is_left_solid(x, y, 1, 3))
                                 itemf[i][0] -= lxi;
                        }
                     } // end of riding lift
                  } // end of not moving up
               } // end of not stuck to wall
            } // end of not being carried
         } // end of if not stationary and not lit rocket
      } // end of iterate all active items
}


int mwItem::is_item_stuck_to_wall(int i)
{
   int x = itemf[i][0];
   int y = itemf[i][1];
   if ( (mSolid.is_left_solid(x,y,0,3)) || (mSolid.is_right_solid(x,y,0,3)) || (mSolid.is_down_solid(x,y,0,3)) || (mSolid.is_up_solid(x,y,0,3)) ) return 1;
   return 0;
}

int mwItem::player_drop_item(int p, int i)
{
   int wall_stuck = 0;
   mPlayer.syn[p].carry_item = 0;
   if (item[i][0] != 99) // not lit bomb
   {
      // check to see if the item is embedded in the wall
      int x = itemf[i][0];
      int y = itemf[i][1];

      if (mPlayer.syn[p].left_right) // right
      {
         // is item embedded in a wall to the right?
         while (mSolid.is_right_solid(x, y, 1, 1))
         {
            x--;
            wall_stuck++; // just how stuck was it?
         }
         itemf[i][0] = x;
      }

      if (!mPlayer.syn[p].left_right) // left
      {
         // is item embedded in a wall to the left?
         while (mSolid.is_left_solid(x, y, 1, 1))
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

void mwItem::proc_player_carry(int p)
{
   if ((mPlayer.syn[p].active) && (mPlayer.syn[p].carry_item))
      if (!mPlayer.syn[p].paused || (mPlayer.syn[p].paused && mPlayer.syn[p].paused_type == 2))// player is carrying item
      {
         int i = mPlayer.syn[p].carry_item-1;  // item number

         if (!item[i][0]) mPlayer.syn[p].carry_item = 0; // if player is carrying inactive item, drop item

         if ((item[i][0] == 98) || (item[i][0] == 99)) item[i][13] = p; // mark player carrying lit bomb or rocket

         if (item[i][0] != 98)            // not lit rocket
         {
            // set item position relative to player that's carrying it
            itemf[i][1] = mPlayer.syn[p].y - 2;
            if (!mPlayer.syn[p].left_right) itemf[i][0] = mPlayer.syn[p].x - 15;
            if (mPlayer.syn[p].left_right)  itemf[i][0] = mPlayer.syn[p].x + 15;
         }
         if (!mPlayer.syn[p].fire) // drop
         {
            int wall_stuck = player_drop_item(p, i);

            if (mPlayer.syn[p].paused && mPlayer.syn[p].paused_type == 2) // dropped item while in door travel
            {
               itemf[i][2] = 0;
               itemf[i][3] = 0;
            }
            else if (wall_stuck < 6)
            {
               if (item[i][0] != 98)            // not lit rocket
               {
                  itemf[i][2] = mPlayer.syn[p].xinc;  // inherit the players momentum
                  itemf[i][3] = mPlayer.syn[p].yinc;
                  if (mPlayer.syn[p].up)    itemf[i][3] -= 6; // throw item upwards
                  if (mPlayer.syn[p].down)  itemf[i][3] =  3; // throw item downwards
                  if (mPlayer.syn[p].left)  itemf[i][2] -= 2; // throw item left
                  if (mPlayer.syn[p].right) itemf[i][2] += 2; // throw item right
               }

               // prevent sticky bombs from sticking to the ground when throwing upwards
               if ((item[i][0] == 99) && (item[i][11]) && (mPlayer.syn[p].up)) itemf[i][1] -= 2;

            }
         }
      }
}



void mwItem::proc_item_collision(int p, int i)
{
   // make it so any item other than bonus has higher priority
   // if carrying bonus, it will be dropped and new item will be carried
   int already_carrying = 0;
   if (mPlayer.syn[p].carry_item) // already carrying item
   {
      already_carrying = 1;
      if ((item[mPlayer.syn[p].carry_item][0] == 2) && (item[i][0] != 2)) // carried item is bonus and new item is not bonus
         already_carrying = 0;
   }

   // check if player can carry item
   if ( (!already_carrying) &&    // not carrying item already
         (item[i][3]<0) &&  // item is carryable
         (mPlayer.syn[p].fire) )      // fire pressed
   {
      // check to see if another player is already carrying this item
      int other_player_carrying = 0;
      for (int op=0; op<NUM_PLAYERS; op++)
         if ((mPlayer.syn[op].active) && (!mPlayer.syn[op].paused) && (mPlayer.syn[op].carry_item == i+1)) other_player_carrying = 1;

       // allow carry
       if ((other_player_carrying == 0) ||   // if no other player is carrying
          (item[i][0] == 98))          // allow multiple player carry for rocket
          mPlayer.syn[p].carry_item = i+1;
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
      case 10: proc_pmsg_collision(i);        break;
      case 11: proc_rocket_collision(p, i);   break;
      case 12: proc_warp_collision(p, i);     break;
      case 14: proc_switch_collision(p, i);   break;
      case 15: proc_sproingy_collision(p, i); break;
      case 18: proc_gate_collision(p, i);     break;
   }
}



void mwItem::proc_bonus_collision(int p, int i)
{
   int bonus_type = item[i][6];
   if (bonus_type == 1) // health bonus
   {
      if (mPlayer.syn[p].health < 100)
      {
         item[i][0] = 0;
         mPlayer.syn[p].health += item[i][7];
         if (mPlayer.syn[p].health > 100) mPlayer.syn[p].health = 100;
         mGameEvent.add(28, 0, 0, p, 0, item[i][1], item[i][7]);
      }
   }
   if (bonus_type == 3) // purple coin!!!
   {
      item[i][0] = 0;
      mPlayer.syn[p].stat_purple_coins++;
      mGameEvent.add(27, 0, 0, p, 0, 0, 0);
   }
}

void mwItem::proc_mine_collision(int p, int i)
{
   mPlayer.syn[p].health -= (float)item[i][8] / 100;
   mGameEvent.add(11, 0, 0, p, 5, 0, item[i][8]);
}



void mwItem::proc_sproingy_collision(int p, int i)
{
   float px = mPlayer.syn[p].x;
   float py = mPlayer.syn[p].y;
   float x1 = itemf[i][0] - 10;
   float x2 = itemf[i][0] + 10;
   float y1 = itemf[i][1] - 16;
   float y2 = itemf[i][1] - 8;

   if ( (px > x1) && (px < x2) && (py > y1) && (py < y2) &&
        (mPlayer.syn[p].yinc > 0) && (mPlayer.syn[p].jump) )  // falling and jump held
   {
      mGameEvent.add(24, 0, 0, p, i, 0, 0);
      mPlayer.syn[p].yinc = 0 - (float) item[i][7] / 7.1;
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
[13] - timer
[14] - switch
[15] - sproingy
[16] - block manip
[17] - block damage
[18] - gate

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
item[][1]  draw type (0=hidden, 1=static door, 2=animated warp)
item[][6]  color
item[][7]  move type (0=auto, 1=force instant, 2=force move)
item[][8]  type (0=exit only, 1=linked dest)
item[][9]  linked destination item
item[][10] key held flag
item[][11] door entry type (0=immed, 1=up, 2=down)
item[][12] draw lines always, never, only when touched

[18] - gate
item[][6] level num
item[][7]
item[][8]
item[][9]
item[][10]
item[][11]
item[][12]





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
item[][1] event trigger
item[][6]  msg x
item[][7]  msg y
item[][8]  msg w
item[][9]  msg h
item[][10]
item[][11]
item[][12] timer count and val  (packed)
item[][13] text and frame color (packed)

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

