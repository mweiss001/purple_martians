// mwEnemyCannon.cpp

#include "pm.h"
#include "mwEnemy.h"
#include "mwBitmap.h"
#include "mwShots.h"
#include "z_solid.h"
#include "mwProgramState.h"




void mwEnemy::move_bouncer_cannon_common(int e)
{
   if ((Ei[e][8]) && (Ei[e][7] > Ei[e][8])) // seek
   {
      Ei[e][7] = 0;
      set_enemy_xyinc_from_player(e, find_closest_player(e));
   }
   if ((Ef[e][2]) > 0)  // move right
   {
     Ef[e][0] += Ef[e][2];
     if (is_right_solid(Ef[e][0], Ef[e][1], 1, 2)) // bounce
     {
        Ei[e][7]++; // inc bounce count
        Ef[e][2] =- Ef[e][2]; // reverse xinc
        Ef[e][0] += Ef[e][2]; // take back last move
     }
   }
   if ((Ef[e][2]) < 0)  // move left
   {
      Ef[e][0] += Ef[e][2];
      if (is_left_solid(Ef[e][0], Ef[e][1], 1, 2)) // bounce
      {
         Ei[e][7]++;
         Ef[e][2] =- Ef[e][2]; // reverse xinc
         Ef[e][0] += Ef[e][2]; // take back last move
      }
   }
   if (Ef[e][3] > 0) // move down
   {
      Ef[e][1] += Ef[e][3];
      if (is_down_solid(Ef[e][0], Ef[e][1], 1, 2))
      {
         Ei[e][7]++;
         Ef[e][3] =- Ef[e][3]; // reverse yinc
         Ef[e][1] += Ef[e][3]; // take back last move
      }
   }
   if (Ef[e][3] < 0)  // move up
   {
      Ef[e][1] += Ef[e][3];
      if (is_up_solid(Ef[e][0], Ef[e][1], 0, 2) == 1)
      {
         Ei[e][7]++;
         Ef[e][3] =- Ef[e][3]; // reverse yinc
         Ef[e][1] += Ef[e][3]; // take back last move
      }
   }
}


//----------------------------------------------------------------------------------------
//--6--cannon-----------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
void mwEnemy::move_cannon(int e)
{
   enemy_player_hit_proc(e);
   if (Ei[e][31]) // enemy hit
   {
      if (Ei[e][31] == 2) // hit type == bomb
      {
         //Ei[e][9] -= 3;      // three hits
         //if (Ei[e][9] < 0) Ei[e][9] = 0;
         enemy_killed(e); // kill instantly no matter how many hits left
         return;
      }
      if (Ei[e][9] == 0) // dead
      {
         enemy_killed(e);
         return;
      }
      else
      {
         Ei[e][9]--;        // one less hit
         Ei[e][31] = 0;     // clear hit

         float mul = 1.2;
         if (Ef[e][5] > 10) mul = 1.1;
         if (Ef[e][5] > 20) mul = 1.05;

         Ef[e][2]  *= mul; // x speed
         Ef[e][3]  *= mul; // y speed
         Ef[e][5]  *= mul; // seek speed
         Ef[e][7]  *= mul; // shot speed
         Ef[e][12] *= 1.1; // scale

         Ei[e][29] += 2; // collision box size
      }
   }

   int p = find_closest_player_cannon(e);
   if (p != -1) set_enemy_rot_from_player(e, p); // rotate to face player if player in proximity
   else         set_enemy_rot_from_incs(e);      // set rotation based on direction of travel

   // set bitmap
   float rtio = Ei[e][16] / Ei[e][15];
   Ei[e][1] = 412;                  // green cannon by default
   if (rtio < 0.3)  Ei[e][1] = 413; // less green cannon
   if (rtio < 0.2)  Ei[e][1] = 414; // orange cannon
   if (rtio < 0.1)  Ei[e][1] = 415; // red cannon

   // time to shoot cannonball ?
   if (Ei[e][15])
      if (--Ei[e][16] < 0) // cannon shot wait
      {
        Ei[e][16] = 0; // don't let it go below zero, because it is used for ratio, but also don't reset it unless fired
        if (p != -1)
         {
            Ei[e][16] = Ei[e][15]; // reset cannon shot wait
            mwS.fire_enemy_shota(e, 55, p);
         }
      }
   move_bouncer_cannon_common(e);
}

//--4--bouncer-----------------------------------------------------------------------------

void mwEnemy::move_bouncer(int e)
{
   if (Ei[e][31]) // hit
   {
      enemy_killed(e);
      return; // to stop rest of execution
   }
   enemy_player_hit_proc(e);

   move_bouncer_cannon_common(e);

   set_enemy_rot_from_incs(e);  // set rotation based on direction of travel

   // set animation sequence
   if (Ei[e][7]) Ei[e][3] = Ei[e][5]; // main ans
   else Ei[e][3] = Ei[e][6]; // seek ans


   // set the bitmap from frame_num mod by animation sequence
   int ans = Ei[e][3]; // animation sequence number

   // number of shapes in animation sequence
   int ns = mwB.zz[4][ans];

   // number of frames for each shape
   int nf = mwB.zz[3][ans]+1;

   // total sequence length in frames
   int tsl = nf * ns;

   // get mod of mwPS.frame_num
   int pm = mwPS.frame_num % tsl;

   // get shape number from mod
   int ss = pm / nf;

   // set shape in enemy array
   Ei[e][1] = mwB.zz[5+ss][ans];
}




