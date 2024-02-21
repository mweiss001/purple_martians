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

#include "mwDemoMode.h"
#include "mwDemoRecord.h"

mwItem mItem;

mwItem::mwItem()
{
   initialize();
}

void mwItem::initialize(void)
{

   strcpy(item_name[0],  "item_empty");

   strcpy(item_name[PM_ITEM_TYPE_DOOR],   "Door");
   strcpy(item_name[PM_ITEM_TYPE_BONUS],  "Bonus");
   strcpy(item_name[PM_ITEM_TYPE_EXIT],   "Exit");
   strcpy(item_name[PM_ITEM_TYPE_KEY],    "Key");
   strcpy(item_name[PM_ITEM_TYPE_START],  "Start");
   strcpy(item_name[PM_ITEM_TYPE_ORB],    "Orb");
   strcpy(item_name[PM_ITEM_TYPE_MINE],   "Mine");
   strcpy(item_name[PM_ITEM_TYPE_BOMB],   "Bomb");
   strcpy(item_name[PM_ITEM_TYPE_TRIGGR], "Trigger");
   strcpy(item_name[PM_ITEM_TYPE_MSG],    "Message");
   strcpy(item_name[PM_ITEM_TYPE_ROCKET], "Rocket");

   strcpy(item_name[12], "undef");

   strcpy(item_name[PM_ITEM_TYPE_TIMER],  "Timer");
   strcpy(item_name[PM_ITEM_TYPE_SWITCH], "Switch");
   strcpy(item_name[PM_ITEM_TYPE_SPRING], "Sproingy");
   strcpy(item_name[PM_ITEM_TYPE_BLKMNP], "Block Manip");
   strcpy(item_name[PM_ITEM_TYPE_BLKDMG], "Block Damage");
   strcpy(item_name[PM_ITEM_TYPE_GATE],   "Gate");
   strcpy(item_name[PM_ITEM_TYPE_HIDER],  "Hider");

   item_tile[0]  = 0;

   item_tile[PM_ITEM_TYPE_DOOR]   = 448;
   item_tile[PM_ITEM_TYPE_BONUS]  = 304;
   item_tile[PM_ITEM_TYPE_EXIT]   = 398;
   item_tile[PM_ITEM_TYPE_KEY]    = 272;
   item_tile[PM_ITEM_TYPE_START]  = 383;
   item_tile[PM_ITEM_TYPE_ORB]    = 418;
   item_tile[PM_ITEM_TYPE_MINE]   = 456;
   item_tile[PM_ITEM_TYPE_BOMB]   = 538;
   item_tile[PM_ITEM_TYPE_TRIGGR] = 991;
   item_tile[PM_ITEM_TYPE_MSG]    = 256;
   item_tile[PM_ITEM_TYPE_ROCKET] = 249;

   item_tile[12] = 825;

   item_tile[PM_ITEM_TYPE_TIMER] = 987;
   item_tile[PM_ITEM_TYPE_SWITCH] = 745;
   item_tile[PM_ITEM_TYPE_SPRING] = 237;
   item_tile[PM_ITEM_TYPE_BLKMNP] = 989;
   item_tile[PM_ITEM_TYPE_BLKDMG] = 988;
   item_tile[PM_ITEM_TYPE_HIDER]  = 940;
   item_tile[PM_ITEM_TYPE_GATE]   = 476;

}

// does the item type have a secondary position at 6,7?
int mwItem::item_secondary67(int type)
{
   if (type == PM_ITEM_TYPE_KEY)    return 1; // key
   if (type == PM_ITEM_TYPE_TRIGGR) return 1; // trigger
   if (type == PM_ITEM_TYPE_MSG)    return 1; // message
   if (type == PM_ITEM_TYPE_TIMER)  return 1; // timer display
   if (type == PM_ITEM_TYPE_SWITCH) return 1; // switch
   if (type == PM_ITEM_TYPE_BLKMNP) return 1; // block manip
   if (type == PM_ITEM_TYPE_BLKDMG) return 1; // block damage
   if (type == PM_ITEM_TYPE_HIDER)  return 1; // hider
   return 0;
}


int mwItem::item_secondary67_hires(int type)
{
   if (type == PM_ITEM_TYPE_TRIGGR) return 1; // trigger
   if (type == PM_ITEM_TYPE_MSG)    return 1; // message
   if (type == PM_ITEM_TYPE_TIMER)  return 1; // timer display
   if (type == PM_ITEM_TYPE_BLKDMG) return 1; // block damage
   if (type == PM_ITEM_TYPE_HIDER)  return 1; // hider
   return 0;
}



void mwItem::draw_items(void)
{
   al_set_target_bitmap(mBitmap.level_buffer);
   for (int i=0; i<500; i++)
      if ((item[i][0]) && (!((mItem.item[i][0] == 10) && (!strncmp(mItem.pmsgtext[i], "Level Statistics", 16))))) // skip drawing level stats message here, do it later
         draw_item(i, 0, 0, 0);

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
   if (type == PM_ITEM_TYPE_DOOR)   drawn = draw_door         (i, x, y, custom);
   if (type == PM_ITEM_TYPE_BONUS)  drawn = draw_bonus        (i, x, y, shape);
   if (type == PM_ITEM_TYPE_EXIT)   drawn = draw_exit         (i, x, y, shape);
   if (type == PM_ITEM_TYPE_KEY)    drawn = draw_key          (i, x, y, shape);
   if (type == PM_ITEM_TYPE_START)  drawn = draw_start        (i, x, y, shape);
   if (type == PM_ITEM_TYPE_ORB)    drawn = draw_orb          (i, x, y);
   if (type == PM_ITEM_TYPE_BOMB)   drawn = draw_bomb         (i, x, y, shape);
   if (type == PM_ITEM_TYPE_TRIGGR) drawn = draw_trigger      (i, x, y, custom);
   if (type == PM_ITEM_TYPE_MSG)    drawn = draw_message      (i, x, y, custom);
   if (type == PM_ITEM_TYPE_ROCKET) drawn = draw_rocket       (i, x, y, shape);
   if (type == PM_ITEM_TYPE_TIMER)  drawn = draw_timer        (i, x, y, custom);
   if (type == PM_ITEM_TYPE_BLKMNP) drawn = draw_block_manip  (i, x, y);
   if (type == PM_ITEM_TYPE_BLKDMG) drawn = draw_block_damage (i, x, y, custom);

//   if (type == PM_ITEM_TYPE_GATE) drawn = draw_gate         (i, x, y, custom);

   if (type == PM_ITEM_TYPE_GATE) drawn = 1;

   if (type == PM_ITEM_TYPE_HIDER) drawn = draw_hider        (i, x, y, shape);

   if (type == PM_ITEM_TYPE_LIT_RCKT) drawn = draw_rocket       (i, x, y, shape);
   if (type == PM_ITEM_TYPE_LIT_BOMB) drawn = draw_lit_bomb     (i);

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
   if ((item[i][6] == 3) && (mLoop.frame_num > 0)) // purple coin
   {
      if (mLoop.state[0] == PM_PROGRAM_STATE_MAIN_GAME_LOOP) return 1; // purple coin custom draw when game running
      if ((mLoop.state[0] == PM_PROGRAM_STATE_DEMO_RECORD) && (mDemoRecord.play)) return 1;
      if (mDemoMode.mode) return 1; // demo mode is playing
   }
   return 0;
}


void mwItem::draw_purple_coin_screen_direct(int i)
{
   float ex1=0, ey1=0;
   mScreen.calc_actual_screen_position(itemf[i][0]+10, itemf[i][1]+10, ex1, ey1);
   mBitmap.spin_shape2(item[i][1], ex1, ey1, 0.8, 0.5, 40);
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
         if ((type == PM_ITEM_TYPE_KEY) && (item[i][11] > 0)) proc_moving_key(i);
         if (type == PM_ITEM_TYPE_ORB)      proc_orb(i);
         if (type == PM_ITEM_TYPE_TRIGGR)   proc_trigger(i);
         if (type == PM_ITEM_TYPE_MSG)      proc_pmsg(i);
         if (type == PM_ITEM_TYPE_TIMER)    proc_timer(i);
         if (type == PM_ITEM_TYPE_BLKMNP)   proc_block_manip(i);
         if (type == PM_ITEM_TYPE_BLKDMG)   proc_block_damage(i);
         if (type == PM_ITEM_TYPE_HIDER)    proc_hider(i);
         if (type == PM_ITEM_TYPE_LIT_BOMB) proc_lit_bomb(i);
         if (type == PM_ITEM_TYPE_LIT_RCKT) proc_lit_rocket(i);


         // check for time to live
         if ((type != PM_ITEM_TYPE_TRIGGR) &&
             (type != PM_ITEM_TYPE_TIMER) &&
             (type != PM_ITEM_TYPE_BLKMNP) &&
             (type != PM_ITEM_TYPE_LIT_BOMB) &&
             (type != PM_ITEM_TYPE_BLKDMG))

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
         if ((item[i][3]) &&  // not stationary
             (type != PM_ITEM_TYPE_LIT_RCKT) &&
             (type != PM_ITEM_TYPE_TRIGGR) &&
             (type != PM_ITEM_TYPE_TIMER) &&
             (type != PM_ITEM_TYPE_BLKMNP) &&
             (type != PM_ITEM_TYPE_BLKDMG) &&
             (type != PM_ITEM_TYPE_HIDER) &&
             (! ((type == PM_ITEM_TYPE_KEY) && (item[i][11] > 0)))    ) // not moving key
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
               if ((type == PM_ITEM_TYPE_LIT_BOMB) && (item[i][11])) sticky = 1;
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

         if ((item[i][0] == PM_ITEM_TYPE_LIT_RCKT) || (item[i][0] == PM_ITEM_TYPE_LIT_BOMB)) item[i][13] = p; // mark player carrying lit bomb or rocket

         if (item[i][0] != PM_ITEM_TYPE_LIT_RCKT)            // not lit rocket
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
               if (item[i][0] != PM_ITEM_TYPE_LIT_RCKT)            // not lit rocket
               {
                  itemf[i][2] = mPlayer.syn[p].xinc;  // inherit the players momentum
                  itemf[i][3] = mPlayer.syn[p].yinc;
                  if (mPlayer.syn[p].up)    itemf[i][3] -= 6; // throw item upwards
                  if (mPlayer.syn[p].down)  itemf[i][3] =  3; // throw item downwards
                  if (mPlayer.syn[p].left)  itemf[i][2] -= 2; // throw item left
                  if (mPlayer.syn[p].right) itemf[i][2] += 2; // throw item right
               }

               // prevent sticky bombs from sticking to the ground when throwing upwards
               if ((item[i][0] == PM_ITEM_TYPE_LIT_BOMB) && (item[i][11]) && (mPlayer.syn[p].up)) itemf[i][1] -= 2;

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
       if ((other_player_carrying == 0) ||             // if no other player is carrying
          (item[i][0] == PM_ITEM_TYPE_LIT_RCKT))       // allow multiple player carry for rocket
          mPlayer.syn[p].carry_item = i+1;
   }


   if (mPlayer.syn[p].carry_item) mDemoRecord.mark_player_carry(p);



   int t = item[i][0];
   if (t == PM_ITEM_TYPE_DOOR)    proc_door_collision(p, i);
   if (t == PM_ITEM_TYPE_BONUS)   proc_bonus_collision(p, i);
   if (t == PM_ITEM_TYPE_EXIT)    proc_exit_collision(p, i);
   if (t == PM_ITEM_TYPE_KEY)     proc_key_collision(p, i);
   if (t == PM_ITEM_TYPE_START)   proc_start_collision(p, i);
   if (t == PM_ITEM_TYPE_ORB)     proc_orb_collision(p, i);
   if (t == PM_ITEM_TYPE_MINE)    proc_mine_collision(p, i);
   if (t == PM_ITEM_TYPE_BOMB)    proc_bomb_collision(p, i);
   if (t == PM_ITEM_TYPE_MSG)     proc_pmsg_collision(i);
   if (t == PM_ITEM_TYPE_ROCKET)  proc_rocket_collision(p, i);
   if (t == PM_ITEM_TYPE_SWITCH)  proc_switch_collision(p, i);
   if (t == PM_ITEM_TYPE_SPRING)  proc_sproingy_collision(p, i);
   if (t == PM_ITEM_TYPE_GATE)    proc_gate_collision(p, i);


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
      if (!mLoop.ff_state) mLevel.level_data_purple_coins_collected++;
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
item[][8] no backsies



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

[10] - msg
item[][1] event trigger
item[][2] draw flags
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

[18] - gate
item[][6] level num
item[][7] player touching
item[][8] display page
item[][9]
item[][10]
item[][11]
item[][12]



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

