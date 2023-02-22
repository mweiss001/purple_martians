// mwEnemyJumpworm.cpp

#include "pm.h"
#include "mwEnemy.h"
#include "z_player.h"
#include "mwBitmap.h"
#include "mwShots.h"
#include "z_solid.h"
#include "mwLift.h"
#include "mwProgramState.h"





//-------------------------------------------------------------------------------------------
//--5--jumpworm-----------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------
/*

Ei[][1]   tile to draw
Ei[][2]   direction (0=left, 1=right)
Ei[][4]   ground speed divider

Ei[][5]   jump/fall -160 max jump, 160 max fall
Ei[][6]   jump wait (0=none)
Ei[][7]   jump when player above
Ei[][8]   wall jump boost (0=none, 160=max)

Ei[][10]  turn before hole
Ei[][11]  jump before hole
Ei[][12]  jump before wall

Ei[][13]  change dir on next cycle 0
Ei[][14]  cycle offset

Ef[][2]  y speed
Ef[][6]  x speed when jumping

 */



void mwEnemy::enemy_jumpworm(int e)
{
   int EXint = Ef[e][0];
   int EYint = Ef[e][1];

   if (Ei[e][31]) // hit
   {
      enemy_killed(e);
      return; // break;  to stop rest of execution
   }
   enemy_player_hit_proc(e);

   int sd = Ei[e][4]; // ground speed divider

   int c = mwPS.frame_num % (10*sd);
   int mf = 0; // move frame
   if (c % sd == 0) mf = 1;

   int cycle = c/sd;

   cycle += Ei[e][14]; // add cycle_offset
   while (cycle > 9) cycle -=10;



   // if change direction on next cycle 0
   if ((Ei[e][13]) && (cycle == 0))
   {
      Ei[e][13] = 0;
      if (Ei[e][2])
      {
          Ei[e][2] = 0;
          Ef[e][0] += 10;
      }
      else
      {
          Ei[e][2] = 1;
          Ef[e][0] -= 10;
      }
   }


   int attempt_jump = 0;
   int on_solid = 0;
   int on_lift = 0;

   int ret = is_down_solid(EXint, EYint, 1, 2);
   if ((ret == 1) || (ret == 2)) on_solid = 1;

   if (ret >= 32) // on lift
   {
      on_lift = 1;
      Ef[e][1] += Lift.cur[ret-32].yinc;  // move with lift
   }

   // x move when on ground (0-4 move) (5-9 retract)
   if ((cycle < 5) && (mf) && (!Ei[e][5]))
   {
      if (Ei[e][2] == 1)  // move right
      {
         int change_dir = 0;
         Ef[e][0] += 2;
         EXint = Ef[e][0];
         if ((on_solid) || (on_lift))
         {
            if (Ei[e][10]) // turn before hole
              if (!is_right_solid(EXint+Ei[e][10]-6, EYint+20, 1, 2)) change_dir = 1;
            if (Ei[e][12]) // jump before wall
              if (is_right_solid(EXint+Ei[e][12], EYint, 1, 2)) attempt_jump = 1;
            if (Ei[e][11]) // jump before hole
               if (!is_right_solid(EXint+Ei[e][11]-18, EYint+20, 1, 2)) attempt_jump = 1;
         }
         if ((is_right_solid(EXint, EYint, 1, 2)) || (change_dir))
         {
            Ei[e][13] = 1; // change direction on next cycle 0;
            if (cycle == 0) Ei[e][14] +=8;
            if (cycle == 1) Ei[e][14] +=6;
            if (cycle == 2) Ei[e][14] +=4;
            if (cycle == 3) Ei[e][14] +=2;
         }
      }
      if (Ei[e][2] == 0)  // move left
      {
         int change_dir = 0;
         Ef[e][0] -= 2;
         EXint = Ef[e][0];
         if ((on_solid) || (on_lift))
         {
            if (Ei[e][10]) // turn before hole
              if (!is_left_solid(EXint-Ei[e][10]+6, EYint+20, 1, 2)) change_dir = 1;
            if (Ei[e][12]) // jump before wall
               if (is_left_solid(EXint-Ei[e][12], EYint, 1, 2)) attempt_jump = 1;
            if (Ei[e][11]) // jump before hole
               if (!is_left_solid(EXint-Ei[e][11]+18, EYint+20, 1, 2)) attempt_jump = 1;
         }
         if ((is_left_solid(EXint-2, EYint, 1, 2)) || (change_dir))
         {
            Ei[e][13] = 1; // change direction on next cycle 0;
            if (cycle == 0) Ei[e][14] +=8;
            if (cycle == 1) Ei[e][14] +=6;
            if (cycle == 2) Ei[e][14] +=4;
            if (cycle == 3) Ei[e][14] +=2;
         }
      }
   }


   if (cycle == 0) Ei[e][1] = 708;
   if (cycle == 1) Ei[e][1] = 707;
   if (cycle == 2) Ei[e][1] = 706;
   if (cycle == 3) Ei[e][1] = 705;
   if (cycle == 4) Ei[e][1] = 704;
   if (cycle == 5) Ei[e][1] = 705;
   if (cycle == 6) Ei[e][1] = 706;
   if (cycle == 7) Ei[e][1] = 707;
   if (cycle == 8) Ei[e][1] = 708;
   if (cycle == 9) Ei[e][1] = 709;


   if ((on_solid) && (Ei[e][5] >= 0)) // solid and not jumping (falling or steady)
   {
      Ef[e][1] -= (int) (Ef[e][1]) % 20;  // align with floor
      Ef[e][1] =  (int) Ef[e][1];  // remove decimal
      Ei[e][5] = 0;
   }

   if ((!on_solid) && (!on_lift) && (Ei[e][5] >= 0)) // not solid and falling
   {
      Ei[e][5] +=5; // gravity
      if (Ei[e][5] > 160) Ei[e][5] = 160; // terminal velocity

      // apply y move
      float ym = Ei[e][5] * Ef[e][3];
      float ym1 = ym / 100;
      Ef[e][1] += ym1;

      EYint = Ef[e][1];
      if (is_down_solid(EXint, EYint, 1, 2))
      {
         on_solid = 1;
         Ef[e][1] -= (int) (Ef[e][1]) % 20;  // align with floor
         Ef[e][1] =  (int) Ef[e][1];  // remove decimal
         Ei[e][5] = 0;
      }
   }

   if (Ei[e][5] < 0) // rising or jumping
   {
      Ei[e][5] +=5; // gravity
      if (Ei[e][5] < -160) Ei[e][5] = -160; // max velocity

      // apply y move
      float ym = Ei[e][5] * Ef[e][3];
      float ym1 = ym/100;
      Ef[e][1] += ym1;
      EYint = Ef[e][1];
      if ((is_up_solid(EXint, EYint, 1, 2) == 1) || (is_up_solid(EXint, EYint, 1, 2) > 31) )
      {
         Ei[e][5] = 1;     // stop rising
         Ef[e][1] -= ym1; // take back move
      }
   }


   // timed jump and jump when player passes above
   if ((on_solid) || (on_lift))
   {
      // mwPS.frame_num jump
      if ((Ei[e][6] > 0) && ((mwPS.frame_num % Ei[e][6]) == 1)) attempt_jump = 1;

      // check for jump if player passes above
      if (Ei[e][7] > 0)
         for (int p=0; p<NUM_PLAYERS; p++)
            if ((players[p].active) && (!players[p].paused) )
               if ((EXint < players[p].x + Ei[e][7]) && (EXint > players[p].x - Ei[e][7]) &&
                   (EYint > players[p].y)) attempt_jump = 1;
   }

   if (attempt_jump)
   {
      EXint = Ef[e][0];
      EYint = Ef[e][1];
      if ((is_up_solid(EXint, EYint, 1, 2) != 1) && (is_up_solid(EXint, EYint, 1, 2) < 32) ) Ei[e][5] = -160;
   }

   Ef[e][14] = 0; // default is no rotation


   // x move differently if jumping or falling
   if ((Ei[e][5]) || (!on_solid))
   {
      float js = Ef[e][6]; // x speed for jump
      if (Ei[e][2] == 1)                              // move right
      {
         if (is_right_solid(EXint+js, EYint, 1, 2))
         {
            Ei[e][2] = 0;                             // change direction
            Ei[e][5] -= Ei[e][8];                     // wall jump boost
            if (Ei[e][5] < -160) Ei[e][5] = -160;     // max jump
         }
         else Ef[e][0] += js;                        // make the move
      }
      else if (Ei[e][2] == 0)                         // move left
      {
         if (is_left_solid(EXint-js, EYint, 1, 2))
         {
            Ei[e][2] = 1;                             // change direction
            Ei[e][5] -= Ei[e][8];                     // wall jump boost
            if (Ei[e][5] < -160) Ei[e][5] = -160;     // max jump
        }
         else Ef[e][0] -= js;                        // make the move
      }


      // set shape
      Ei[e][1] = 704;              // only one shape for falling or jumping
      float ym = Ei[e][5] * Ef[e][3] / 100;
      float angle = atan2(ym, js);
      if (!Ei[e][2]) angle = -angle;   // reverse angle depending on x direction
      Ef[e][14] = angle;
   }
}
