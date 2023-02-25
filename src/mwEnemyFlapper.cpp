// mwEnemyFlapper.cpp

#include "pm.h"
#include "mwEnemy.h"
#include "mwPlayers.h"
#include "mwBitmap.h"
#include "mwShots.h"
#include "mwSolid.h"


//--12--flapper-----------------------------------------------------------------------------

//   Ei[][0] = type 12 "flapper"
//   Ei[][1] = draw bitmap
//   Ei[][2] = hflip - right(1) or left(0)
//   Ei[][3] = base animation sequence

//   Ei[][14] = base ypo for debug drawing

//   Ei[][15] = shot retrigger time
//   Ei[][16] = shot retrigger counter

//   Ei[][17] = prox width
//   Ei[][18] = prox height
//   Ei[][19] = prox depth

//   Ei[][20] = height above player
//   Ei[][21] = flap height

//   Ei[][30] = collision box
//   Ei[][31] = health bonus

//   Ef[][0]  = x pos
//   Ef[][1]  = y pos
//   Ef[][2]  = x inc
//   Ef[][3]  = y inc
//   Ef[][4]  = health decrement
//   Ef[][5]  = max x speed
//   Ef[][6]  = x accel
//   Ef[][7]  = shot speed

//   Ef[][8]  = flap offset for next loop
//   Ef[][9]  = flap speed counter
//   Ef[][10] = flap speed inc



void mwEnemy::move_flapper(int e)
{

   if (Ei[e][31]) // hit
   {
      enemy_killed(e);
      return;
   }
   enemy_player_hit_proc(e);

   int p = -1; // default if no player found in trigger box

   // show if empty or has shot ready
   if (Ei[e][16] < 0) Ei[e][3] = 61;
   else Ei[e][3] = 60;


   // ------------ x move ---------------
   if (Ei[e][2])  // move right
   {
      Ef[e][2] += Ef[e][6];                         // accel
      if (Ef[e][2] > Ef[e][5]) Ef[e][2] = Ef[e][5]; // max speed
      Ef[e][0] += Ef[e][2];                         // apply xinc

      if (mSolid.is_right_solid(Ef[e][0], Ef[e][1], 1, 2))
      {
         Ef[e][0] -= Ef[e][2];    // take back last move
         Ef[e][2] = -Ef[e][5]/3;  // set accel to bounce back with 1/3 max accel
         Ei[e][2] = 0;            // change direction
      }
      // try to shoot right

      if (--Ei[e][16] < 0)
      {
         p = find_closest_player_flapper(e);
         if (p != -1)
         {
            mShot.fire_enemy_shota(e, 62, p);
            Ei[e][16] = Ei[e][15]; // set new prox wait
         }
      }
   }
   else  // move left
   {
      Ef[e][2] -= Ef[e][6];                           // accel
      if (Ef[e][2] < -Ef[e][5]) Ef[e][2] = -Ef[e][5]; // max speed
      Ef[e][0] += Ef[e][2];                           // apply xinc

      if (mSolid.is_left_solid(Ef[e][0], Ef[e][1], 1, 2))
      {
         Ef[e][0] -= Ef[e][2];   // take back last move
         Ef[e][2] = Ef[e][5]/3;  // set accel to bounce back with 1/3 max accel
         Ei[e][2] = 1;           // change direction
      }
      // try to shoot left
      if (--Ei[e][16] < 0)
      {
         p = find_closest_player_flapper(e);
         if (p != -1)
         {
            mShot.fire_enemy_shota(e, 62, p);
            Ei[e][16] = Ei[e][15]; // set new prox wait
         }
       }
   }

   // ------------ y seek  ---------------
   float seek_yinc = 0;  // yinc for this pass

   // (comment out this line to only seek if player in trigger box)
   p = find_closest_player(e); // always seek closest player in y axis

   if (p != -1) // only seek in y axis if valid player in prox
   {


      float rat = 1; // default scaling ratio for seek_yinc
      float h = Ei[e][20]; // height_above_player

      // difference between actual y and desired y
      float df = Ef[e][1] - (mPlayer.syn[p].y - h);
      if (df < 0)
      {
         if (df > -100) rat = -df / 100;
         seek_yinc += Ef[e][3] * rat;
      }
      if (df > 0)
      {
         if (df < 100) rat = df / 100;
         seek_yinc -= Ef[e][3] * rat;
      }
   }

   // ------------ y flap  ---------------
   /* flap sequence
      Efi[][9]  - counter   goes from 0 to 100
      Efi[][10] - counter increment (flap speed)
      Ei[][21]  - max flap height     */

   float f = Ef[e][9];

   // shape offset
   int so = (int)(f/16.66); // 100/6 = 16.66
   Ei[e][1] = mwB.zz[8+so][Ei[e][3]];

   //convert 0-100 to (0-2*PI) to get angle in rad
   float th = (f / 100) * ALLEGRO_PI*2;

   // get sin ratio and scale with flap height
   float yo = sin(th) * Ei[e][21];

   // find difference from last flap offset
   float flap_yinc = yo - Ef[e][8];

   Ef[e][8] = yo; // save this flap offset for next time

   // set base ypos for debug drawing
   Ei[e][14] = Ef[e][1] - yo;

   // inc flap counter and roll over
   Ef[e][9] += Ef[e][10];
   if (Ef[e][9] >= 100) Ef[e][9] = 0;

//   printf("f:%f y:%f sy:%f fy:%f\n", f, Ef[e][1], seek_yinc, flap_yinc);

   // combine the 2 yincs
   float yinc = flap_yinc + seek_yinc;

   // check for floor or ceiling collisions
   if (yinc < 0) // moving up
   {
      yinc = mSolid.is_up_solidf(Ef[e][0], Ef[e][1], -yinc, 0);
      Ef[e][1] -= yinc; // apply allowed move
   }
   else if (yinc > 0) // moving down
   {
      yinc = mSolid.is_down_solidf(Ef[e][0], Ef[e][1], yinc, 0);
      Ef[e][1] += yinc; // apply allowed move
   }
}

