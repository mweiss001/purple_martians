// mwEnemy.cpp

#include "pm.h"
#include "mwEnemy.h"
#include "mwLog.h"
#include "mwPlayers.h"
#include "mwDisplay.h"
#include "mwTimeStamp.h"
#include "mwFont.h"
#include "mwBitmap.h"
#include "mwColor.h"
#include "mwLoop.h"
#include "mwItems.h"
#include "mwShots.h"
#include "mwGameEvent.h"


mwEnemy mEnemy;


void mwEnemy::draw_enemy(int e, int custom, int cx, int cy)
{
   int type = Ei[e][0];
   int x = Ef[e][0];
   int y = Ef[e][1];
   if (custom)
   {
      x = cx;
      y = cy;
   }


   if (type == 9)  draw_cloner(e, x, y, custom);
   if (type == 13) draw_vinepod(e, x, y, custom);

   int flags = 0;
   if (Ei[e][2] == 0) flags = ALLEGRO_FLIP_HORIZONTAL;
   if (Ei[e][2] == 1) flags = 0;
   if (Ei[e][2] == 2) flags = ALLEGRO_FLIP_VERTICAL;
   if (Ei[e][2] == 3) flags = ALLEGRO_FLIP_VERTICAL & ALLEGRO_FLIP_HORIZONTAL;
   int tn = Ei[e][1];

   float rot = Ef[e][14];
   float sc =  Ef[e][12];

   if ((type == 13) && (Ei[e][15] != 0)) // different rotation point for vinepod
      al_draw_scaled_rotated_bitmap(mBitmap.tile[tn], 10, 3, x+10, y+10, sc, sc, rot, flags);
   else al_draw_scaled_rotated_bitmap(mBitmap.tile[tn], 10, 10, x+10, y+10, sc, sc, rot, flags);


   // if enemy is expiring show how many seconds it has left
   if ((!mLoop.level_editor_running) && (Ei[e][27])) al_draw_textf(mFont.pixl, mColor.pc[15], x+10, y-10, ALLEGRO_ALIGN_CENTER, "%d", 1 + (Ei[e][27] - 10) / 40);



/*
   if (Ei[e][0] == 5) // jumpworm
   {
      // enemy position (add 10 to get the center)
      int x1 = Ef[e][0];
      int y1 = Ef[e][1];
      int x2 = x1 + 20;
      int y2 = y1 + 20;
      al_draw_rectangle(x1, y1, x2, y2, mColor.pc[14], 1);
   }
*/


   #ifdef SHOW_CANNON_COLLISION_BOX
   if (Ei[e][0] == 6) // cannon
   {
      // draw some test rects here
      int cbs = Ei[e][29]; // collision box size

      // enemy position (add 10 to get the center)
      int x1 = Ef[e][0] + 10;
      int y1 = Ef[e][1] + 10;

      int il = cbs;
      int color = 11;
      //  printf("x1:%d  y1:%d il:%d  color:%d\n",x1, y1, il, color);

      // check for collision with player
      int b = Ei[e][29]; // collision box size
      for (int p=0; p<NUM_PLAYERS; p++)
         if ((mPlayer.syn[p].active) && (!mPlayer.syn[p].paused))
         {
            float px = mPlayer.syn[p].PX;
            float py = mPlayer.syn[p].PY;

            float ex1 = Ef[e][0] - b;
            float ex2 = Ef[e][0] + b;
            float ey1 = Ef[e][1] - b;
            float ey2 = Ef[e][1] + b;

            // if player in collision box color = red
            if ((px > ex1) && (px < ex2) && (py > ey1) && (py < ey2)) color = 10;
      }
      al_draw_rectangle(x1-il, y1-il, x1+il, y1+il, mColor.pc[color], 1);
   }
   #endif



/*
   if (Ei[e][0] == 3) // archwagon
   {
      al_draw_textf(mFont.pr8, mColor.pc[15], EXint+30, EYint+10, 0, "%d", Ei[e][5]);
   }
*/


   #ifdef SHOW_FLAPPER_DEBUG
   if (Ei[e][0] == 12) // flapper
   {
      int prox = Ei[e][17];
      int color = 14;        // default color
      int ex = EXint+10;
      int ey = EYint+10;
      float pi = ALLEGRO_PI;

      float th1 = -pi/4;   // 45 deg
      float th2 = th1+pi;  // 225 deg
      float thd = pi / 2;  // 90 deg

      // draw flap height
      int base =  Ei[e][14]+10;
      int y1 = base - Ei[e][21];
      int y2 = base + Ei[e][21];

      al_draw_line(EXint-140, base, EXint+140, base, mColor.pc[11], 1);
      al_draw_rectangle(EXint-40, y1, EXint+40, y2, mColor.pc[11], 1);

      // draw height above player
      int hab =  Ei[e][20];
      al_draw_line(EXint-40, base+hab, EXint+40, base+hab, mColor.pc[12], 1);

      int p = find_closest_player_flapper(e);
      if (p != -1)
      {
         color = 10;
         bomb_crosshairs(mPlayer.syn[p].x+10, mPlayer.syn[p].y+10); // mark targetted player
      }
      if (Ef[e][2] > 0) al_draw_pieslice(ex, ey, prox, th1, thd, mColor.pc[color], 1);
      else              al_draw_pieslice(ex, ey, prox, th2, thd, mColor.pc[color], 1);
   }
   #endif

   #ifdef SHOW_TRAKBOT_BULLET_TRIGGER_CIRCLE
   if (Ei[e][0] == 8) // trakbot
   {
      int prox = Ei[e][17];
      int color = 14;        // default color
      int ex = EXint+10;
      int ey = EYint+10;
      float pi = ALLEGRO_PI;
      // is any player in range?
      int p = find_closest_player_trakbot(e);
      if (p != -1)
      {
         color = 10;
         bomb_crosshairs(mPlayer.syn[p].x+10, mPlayer.syn[p].y+10); // mark targetted player
      }


      int quad;
      switch (Ei[e][5]) // mode
      {
         case 0: case 5: quad = 1; break; // floor right, lwall up
         case 1: case 4: quad = 2; break; // rwall up floor left
         case 2: case 7: quad = 3; break; // ceil left, rwall down
         case 3: case 6: quad = 4; break; // lwall down, ceil right
      }
      if (quad == 1) al_draw_pieslice(ex, ey, prox, -pi/2, pi/2, mColor.pc[color], 0);
      if (quad == 4) al_draw_pieslice(ex, ey, prox, 0,     pi/2, mColor.pc[color], 0);
      if (quad == 3) al_draw_pieslice(ex, ey, prox, pi/2,  pi/2, mColor.pc[color], 0);
      if (quad == 2) al_draw_pieslice(ex, ey, prox, pi,    pi/2, mColor.pc[color], 0);
   }
   #endif
}



void mwEnemy::draw_enemies(void)
{
   al_set_target_bitmap(mBitmap.level_buffer);
   for (int e=0; e<100; e++)
      if (Ei[e][0])  // if enemy active
         draw_enemy(e, 0, 0, 0);
}

void mwEnemy::proc_enemy_collision_with_pshot(int e)
{
   // enemy collison box
   float cb = (float) Ei[e][29]; // collision box size
   float x1 = Ef[e][0] - cb;
   float y1 = Ef[e][1] - cb;
   float x2 = Ef[e][0] + cb;
   float y2 = Ef[e][1] + cb;

   for (int c=0; c<50; c++)
      if (mShot.p[c].active)
      {
         // shot position
         float x = mShot.p[c].x;
         float y = mShot.p[c].y;

         // adjust enemy collision box with shot speed
         float cx = fabs(mShot.p[c].xinc/2);
         float cy = fabs(mShot.p[c].yinc/2);

         if ((x > x1-cx) && (x < x2+cx) && (y > y1-cy) && (y < y2+cy))
         {
            //printf("hit!\n");
            int p = mShot.p[c].player;       // player number of shot
            Ei[e][31] = 1;                 // flag that this enemy got shot
            Ei[e][26] = p;                 // number of player that shot enemy
            mPlayer.syn[p].stat_enemy_hits++;  // add to number of hits the player has
            mShot.p[c].active = 0;           // shot dies
         }
      }
}

void mwEnemy::move_enemies()
{
   char msg[1024];
   if (mLog.LOG_TMR_move_enem) mTimeStamp.init_timestamps();
   num_enemy = 0; // count enemies
   for (int e=0; e<100; e++)
      if (Ei[e][0])
      {
         double t0 = al_get_time();
         num_enemy++; // enemy count
         if (Ei[e][0] < 50) proc_enemy_collision_with_pshot(e);
         // check for time to live
         int ttl = Ei[e][27];
         if (ttl)
         {
            if (ttl < 11)
            {
               Ei[e][0] = 66;             // change to different type to prevent use
               Ef[e][4] = 0;  // cant hurt anymore
               Ei[e][29] = 0;             // no collision box
               int sq = 10-ttl;
               Ei[e][1] = mBitmap.zz[5+sq][74];
            }
            if (ttl == 1) Ei[e][0] = 0; // kill instantly
            Ei[e][27]--;
         }

         // check for out of bounds
         if ((Ef[e][0] < 0) || (Ef[e][0] > 1980)) Ei[e][0]=0;
         if ((Ef[e][1] < 0) || (Ef[e][1] > 1980)) Ei[e][0]=0;

         switch (Ei[e][0])
         {
            case 3:  move_archwagon(e);  break;
            case 4:  move_bouncer(e);  break;
            case 5:  move_jumpworm(e);  break;
            case 6:  move_cannon(e);  break;
            case 7:  move_podzilla(e);  break;
            case 8:  move_trakbot(e);  break;
            case 9:  move_cloner(e);  break;
            case 11: move_block_walker(e);  break;
            case 12: move_flapper(e);  break;
            case 13: move_vinepod(e);  break;
            case 99: enemy_deathcount(e); break;
         }
         if (mLog.LOG_TMR_move_enem) mTimeStamp.add_timestamp(103, e, Ei[e][0], al_get_time()-t0, 0);
      }
   if (mLog.LOG_TMR_move_enem)
   {
      // tally up all the times for each enemy type
      double tmr_tally[100][3] = {0};
      for (int i=0; i<10000; i++)
         if (mTimeStamp.timestamps[i].type == 103)
         {
            tmr_tally[mTimeStamp.timestamps[i].frame2][0] +=1;                 // add to number of this type tally
            tmr_tally[mTimeStamp.timestamps[i].frame2][1] += mTimeStamp.timestamps[i].t1; // add to time tally
         }
      // build log entry
      char t[512] = {0};
      char l[64] = {0};
      for (int i=0; i<100; i++)
         if (tmr_tally[i][1] > 0)
         {
            sprintf(l, "m-%s:[%0.4f] ", enemy_name[i][1], (tmr_tally[i][1]/tmr_tally[i][0])*1000000);
            strcat(t, l);
         }
      sprintf(msg, "tmst %s\n", t);
      // printf("%s", msg);
      mLog.add_log_entry2(44, 0, msg);
   }
}



void mwEnemy::enemy_deathcount(int e)
{
   int EXint = Ef[e][0];
   int EYint = Ef[e][1];
   Ef[e][14] += Ef[e][13]; // rot inc
   Ef[e][12] *= Ef[e][11]; // scale scaler

   Ef[e][0] += Ef[e][2]; // xinc
   Ef[e][1] += Ef[e][3]; // yinc

   Ei[e][1] = mBitmap.zz[0][ Ei[e][3] ]; // draw current ans shape
   // dec and check countdown timer
   if (--Ei[e][30] < 0) // create bonus
   {
      Ei[e][0] = 0; // kill enemy
      for (int c=0; c<500; c++)
         if (mItem.item[c][0] == 0) // find empty
         {
            for (int y=0; y<16; y++) mItem.item[c][y] = 0; // clear item
            mItem.item[c][0] = 2;           // type - bonus
            mItem.item[c][1] = Ei[e][24];  // flower shape
            mItem.item[c][2] = 1; // draw mode normal
            mItem.item[c][3] = -1; // carryable
            mItem.item[c][4] = EXint;
            mItem.item[c][5] = EYint;
            mItem.item[c][6] = 1; // bonus type 1 - health
            mItem.item[c][7] = Ei[e][25];  //  life
            mItem.item[c][8] = 0;

            mItem.item[c][14] = 800; // time to live

            mItem.itemf[c][0] = mItem.item[c][4];
            mItem.itemf[c][1] = mItem.item[c][5];
            mItem.itemf[c][2] = 0;
            mItem.itemf[c][3] = 0;
            break; // end loop
         }
   }
}

void mwEnemy::enemy_player_hit_proc(int e)
{
   if (--Ei[e][23] < 0) // hit player retrigger
   {
      if (Ei[e][22]) // player hit!
      {
         int p = Ei[e][22]-1;
         mPlayer.syn[p].health -= Ef[e][4];

         mGameEvent.add(41, 0, 0, p, Ei[e][0], 1, Ef[e][4]);
         Ei[e][22] = 0;  // clear hit
         Ei[e][23] = 60; // set retrigger amount
      }
   }
   else Ei[e][22] = 0;
}

void mwEnemy::enemy_killed(int e)
{
   Ef[e][2] = 0;       // xinc
   Ef[e][3] = 0;       // yinc

   int type = Ei[e][0];
   switch (type)
   {
      case 3: // archwagon
         Ei[e][3]  = 34;    // new ans
         Ei[e][30] = 20;    // death_loop_wait; set delay
         Ei[e][24] = 929;   // health bonus tile
         Ef[e][11] = 1.08;  // scale multiplier
         Ef[e][13] = 0;     // rot inc
      break;
      case 4: // bouncer
         if (Ei[e][3] == 29) Ei[e][3] = 46;  // new ans
         if (Ei[e][3] == 14) Ei[e][3] = 13;
         Ei[e][30] = 20;    // death_loop_wait;  set delay
         Ei[e][24] = 928;   // health bonus tile
         Ef[e][11] = 1.03;  // scale multiplier
         Ef[e][13] = 0.05;  // rot inc
      break;
      case 5: //jumpworm
         Ei[e][3]  = 79;    // new ans
         Ei[e][30] = 20;    // death_loop_wait; set delay
         Ei[e][24] = 935;   // health bonus tile
         Ef[e][11] = 0.96;  // scale multiplier
         Ef[e][13] = 0.5;   // rot inc
         Ef[e][3] = -2;     // yinc
      break;
      case 6: // cannon
         Ei[e][3]  = 37;    // new ans
         Ei[e][30] = 20;    // death_loop_wait; set delay
         Ei[e][24] = 930;   // health bonus tile
         Ef[e][11] = 1.08;  // scale multiplier
         Ef[e][13] = 0.024; // rot inc
      break;
      case 7: case 13: // podzilla, vinepod
         Ei[e][3]  = 45;    // new ans
         Ei[e][30] = 40;    // death_loop_wait; set delay
         Ei[e][24] = 932;   // health bonus tile
         Ef[e][12] = 2.8;   // initial scale
         Ef[e][11] = 0.94;  // scale multiplier
         Ef[e][13] = 0;     // rot inc
      break;
      case 8: // trakbot
         Ei[e][3]  = 44;    // new ans
         Ei[e][30] = 12;    // death_loop_wait; set delay
         Ei[e][24] = 931;   // health bonus tile
         Ef[e][11] = 1.00;  // scale multiplier
         Ef[e][13] = 0.4;   // rot inc
      break;
      case 9: // cloner
         Ei[e][3]  = 105;   // new ans
         Ei[e][30] = 40;    // death_loop_wait; set delay
         Ei[e][24] = 934;   // health bonus tile
         Ef[e][11] = 0.98;  // scale multiplier
         Ef[e][13] = 0.2;   // rot inc
      break;
      case 12: // flapper
         Ei[e][3]  = 63;    // new ans
         Ei[e][30] = 20;    // death_loop_wait; set delay
         Ei[e][24] = 933;   // health bonus tile
         Ef[e][11] = 1.04;  // scale multiplier
         Ef[e][13] = 0;     // rot inc
      break;

   }

   int hbm = 1;                 // default health bonus multiplier x1
   if (Ei[e][31] == 2) hbm = 2; // explosion x2
   Ei[e][25] *= hbm;            // health bonus amount
   Ei[e][24] += (hbm-1)*32;     // health bonus tile multiplier for diamonds

   Ei[e][0] = 99;      // set type to death loop
   Ef[e][4] = 0;       // can't hurt player anymore
   int na = Ei[e][3];  // new ans
   int dl = Ei[e][30]; // death_loop_wait; set delay

   mBitmap.zz[0][na] = mBitmap.zz[5][na];      // set shape
   mBitmap.zz[1][na] = 0;                  // point to zero
   mBitmap.zz[2][na] = 0;                  // set counter
   mBitmap.zz[3][na] = dl / mBitmap.zz[4][na]; // set ans timer

   mGameEvent.add(42, 0, 0, Ei[e][26], e, type, hbm);

//   if (hbm == 1) mGameEvent.add(46, 0, 0, Ei[e][26], e, type, 0);
//   if (hbm == 2) mGameEvent.add(47, 0, 0, Ei[e][26], e, type, 0);


}


/*

-------------------------------------------------------------------------
-------------------------------------------------------------------------
common variables (ones that can't be used by specific types)
by function
-------------------------------------------------------------------------
-------------------------------------------------------------------------

-------------------------------------------------------------------------
-- Variables used in draw
-------------------------------------------------------------------------
Ei[][1] // bitmap
Ei[][2] // draw mode (v and h flips)

Ef[][0]  // x
Ef[][1]  // y
Ef[][12] // scale
Ef[][14] // rot

-------------------------------------------------------------------------
-- Variables used in proc_enemy_collision_with_pshot(int e)
-------------------------------------------------------------------------
Ei[][26]  // player number that hit enemy with shot
Ei[][29]  // enemies collison box size
Ei[][31]  // flag that this enemy got shot with shot

Ef[][0] // x
Ef[][1] // y

-------------------------------------------------------------------------
-- Variables used in move_enemies() common
-------------------------------------------------------------------------
Ei[e][27] // time to live
Ei[e][29] // collision box

Ef[e][4] // life dec

-------------------------------------------------------------------------
-- Variables used in enemy_deathcount(int e)
-------------------------------------------------------------------------
Ei[][1]  // draw bitmap
Ei[][3]  // deathcount ans
Ei[][24] // health bonus shape
Ei[][25] // health bonus amount
Ei[][30] // death loop count

Ef[][11]  // scale inc
Ef[][12]  // scale
Ef[][13]  // rot inc
Ef[][14]  // rot


-------------------------------------------------------------------------
-- Variables used in enemy_player_hit_proc(int e)
-------------------------------------------------------------------------
Ei[][22] // player hit flag and player num + 1
Ei[][23] // hit player retrigger

Ef[][4] // life dec

-------------------------------------------------------------------------
-- Variables used in enemy_killed(int e)
-------------------------------------------------------------------------
Ei[][3]  // new ans

Ei[][24] // health bonus shape
Ei[][25] // health bonus amount
Ei[][26] // player num that killed enemy
Ei[][30] // death loop count
Ei[][31] // hit type (shot or bomb)


Ef[][4]  // life dec
Ef[][11] // scale multiplier
Ef[][13] // rot inc

-------------------------------------------------------------------------
-------------------------------------------------------------------------
common variables (ones that can't be used by specific types)
total list
-------------------------------------------------------------------------
-------------------------------------------------------------------------
Ei[][0] = enemy type
Ei[][1] = bitmap
Ei[][2] = draw mode

Ei[][22] = player hit
Ei[][23] = player hit retrigger
Ei[][24] = helath bonus shape
Ei[][25] = health bonus amount
Ei[][26] = used to tell what player killed enemy
Ei[][27] = time to live
Ei[][28] = cloner create id
Ei[][29] = collision box size
Ei[][30] = death loop count
Ei[][31] = flag that this enemy got shot with shot


Ef[][0] =  x
Ef[][1] =  y
Ef[][2] =  xinc
Ef[][3] =  yinc
Ef[][4] =  LIFE decrement


Ef[][11] = scale multiplier
Ef[][12] = scale;
Ef[][13] = rot inc
Ef[][14] = rot



// enemy types
----------------------------------
3  - Archwagon
4  - Bouncer
5  - Jumpworm
6  - Cannon
7  - Podzilla
8  - Trakbot
9  - Cloner
11 - Block Walker
12 - Flapper
13 - Vinepod



similar types
3  - Archwagon
11 - Block Walker

4 - Bouncer
6 - Cannon

7  - Podzilla
13 - Vinepod






[3] - Archwagon -----------------------------------------------------------------------------
Ei[][3]   ans (2=wagon with arrow, 3=empty wagon)
Ei[][15]  shot retrigger value
Ei[][16]  shot retrigger count
Ei[][17]  shot prox

Ef[][2]  y speed speed
Ef[][6]  x speed speed
Ef[][7]  shot speed

walker_archwagon_common(int e)
Ei[][5]   jump/fall -160 max jump, 160 max fall
Ei[][6]   jump wait (0=none)
Ei[][7]   jump when player above
Ei[][8]   follow(0) or bounce(1)
Ei[][11]  jump before hole
Ei[][12]  jump before wall




[4]--bouncer-----------------------------------------------------------------------------
Ei[][5]  main ans
Ei[][6]  seek ans
Ei[][7]  seek counter
Ei[][8]  seek count
Ef[][5] seek speed

[6]--cannon-----------------------------------------------------------------------------
Ei[][7]   seek counter
Ei[][8]   seek count
Ei[][9]   extra hits to kill
Ei[][15]  shot retrigger value
Ei[][16]  shot retrigger counter
Ei[][17]  shot prox

Ef[][5]  seek speed
Ef[][7]  shot speed
Ef[][12] scale
Ef[][14] rot

[7]--podzilla-----------------------------------------------------------------------------
Ei[][5] = mode
Ei[][6] = sequence counter
Ei[][7] = sequence limit
Ei[][8] = wait count
Ei[][9] = wait limit
Ei[][11] = trigger box x1
Ei[][12] = trigger box x1
Ei[][13] = trigger box x2
Ei[][14] = trigger box y2

[8]--trakbot-----------------------------------------------------------------------------
Ei[][3]  base animation seq (17=trakbot with shot, 18=trakbot without shot)
Ei[][4]  fall 160 max fall
Ei[][5]  mode
Ei[][7]  drop mode(0=no, 1=yes)
Ei[][8]  twirling counter
Ei[][15] shoot holdoff value
Ei[][16] shoot holdoff counter
Ei[][17] shot prox
Ei[][20] rot
Ef[][7] shot speed

[9]--cloner-----------------------------------------------------------------------------

Ei[][4]  draw boxes (0 = none) (1 = trigger) (2 = source/dest) (3 = both)
Ei[][5]  mode
Ei[][6]  create wait
Ei[][7]  create wait counter
Ei[][8]  trigger mode (0=wait, 1=reset, 2=immed)
Ei[][9]  time to live for created objects
Ei[][10] max num of created objects
Ei[][11] trigger box x1
Ei[][12] trigger box y1
Ei[][13] trigger box x2
Ei[][14] trigger box y2
Ei[][15] copy box x
Ei[][16] copy box y
Ei[][17] dest box x
Ei[][18] dest box y
Ei[][19] copy box width
Ei[][20] copy box height




[11]--block walker-----------------------------------------------------------------------------
walker_archwagon_common(int e)
Ei[][5]   jump/fall -160 max jump, 160 max fall
Ei[][6]   jump wait (0=none)
Ei[][7]   jump when player above
Ei[][8]   follow(0) or bounce(1)
Ei[][11]  jump before hole
Ei[][12]  jump before wall


[12]--flapper-----------------------------------------------------------------------------
Ei[][14] base yo for debug drawing
Ei[][15] shot retrigger time
Ei[][16] shot retrigger counter
Ei[][17] prox width
Ei[][18] prox height
Ei[][19] prox depth

Ei[][20] height above player
Ei[][21] flap height

Ef[][5] max x speed
Ef[][6] x accel
Ef[][7] shot speed


Ef[][8] flap offset for next loop
Ef[][9] flap speed counter
Ef[][10] flap speed inc

*/