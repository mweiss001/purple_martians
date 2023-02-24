// mwShots.cpp

#include "pm.h"
#include "mwShots.h"
#include "n_netgame.h"
#include "mwBitmap.h"
#include "mwLevel.h"
#include "z_screen_overlay.h"
#include "mwEnemy.h"
#include "mwPlayers.h"
#include "mwLift.h"


mwShots mShot;

void mwShots::proc_pshot_collision(int p, int b)
{
   mPlayer.syn[p].health -= deathmatch_shot_damage;

   float bxinc = mShot.p[b].xinc/3;

   if (bxinc > 0) mPlayer.syn[p].right_xinc += bxinc;
   if (bxinc < 0) mPlayer.syn[p].left_xinc += bxinc;

   if (mPlayer.syn[p].right_xinc > 4) mPlayer.syn[p].right_xinc = 4;
   if (mPlayer.syn[p].left_xinc < -4) mPlayer.syn[p].left_xinc = -4;

   mPlayer.syn[p].yinc += mShot.p[b].yinc/2;
   if (mPlayer.syn[p].yinc >  5) mPlayer.syn[p].yinc =  5;
   if (mPlayer.syn[p].yinc < -5) mPlayer.syn[p].yinc = -5;

   if (mShot.p[b].yinc < 0)  // player got shot from below
   {
      mPlayer.syn[p].player_ride = 0;
      mPlayer.syn[p].y += mPlayer.syn[p].yinc;
   }
   mShot.p[b].active = 0;  // shot dies
}

void mwShots::proc_player_shoot(int p)
{
   float x = mPlayer.syn[p].x;
   float y = mPlayer.syn[p].y;
   float bs = (float) mPlayer.syn[p].shot_speed;

   if (mPlayer.syn[p].fire)
   {
      if (mPlayer.syn[p].fire_held == 0) // fire button pressed, but not held
      {
         mPlayer.syn[p].fire_held = 1;
         if (mPlayer.syn[p].shot_wait_counter < 1 )
         {
            mPlayer.syn[p].stat_shots_fired++;

            for (int b=0; b<50; b++)     // search for empty shot
               if (!mShot.p[b].active)
               {
                  mShot.p[b].active = 1;
                  mShot.p[b].player = p;
                  mShot.p[b].x = x;
                  mShot.p[b].y = y + 1;
                  mShot.p[b].xinc = 0;
                  mShot.p[b].yinc = 0;

                  if (mPlayer.syn[p].left_right) mShot.p[b].x = x+4;
                  else mShot.p[b].x = x-3;

                  if      (mPlayer.syn[p].up)    mShot.p[b].yinc = -bs;
                  else if (mPlayer.syn[p].down)  mShot.p[b].yinc =  bs;
                  else                       mShot.p[b].xinc = (mPlayer.syn[p].left_right*bs*2) - bs;


                  // if this line is not here player cannot shoot breakable blocks
                  // when directly in front of them..
                  if ((!mPlayer.syn[p].up) && (!mPlayer.syn[p].down) && (mPlayer.syn[p].left_right))
                      mShot.p[b].x -=1;

//                  // temp testing
//                  mShot.p[b][2] = x + (mPlayer.syn[p].left_right * (40*2) ) - 40;
//                  mShot.p[b][3] = y + 1;

                     // move
                     mShot.p[b].x += mShot.p[b].xinc;  // xinc
                     mShot.p[b].y += mShot.p[b].yinc;  // yinc

//                  // temp testing
//                  mShot.p[b][4] = 0; // xinc
//                  mShot.p[b][5] = 0; // yinc

                  mPlayer.syn[p].shot_wait_counter = mPlayer.syn[p].shot_wait;
                  mPlayer.syn[p].fire_held = 1;

                  // extra data is player number, shot number
                  game_event(1, x, y, p, b, 0, 0);

                  break; // to break out of for loop
               }
         }
      }
   }
   else mPlayer.syn[p].fire_held = 0;  // fire is not pressed
   if (mPlayer.syn[p].shot_wait_counter > 0) mPlayer.syn[p].shot_wait_counter--;
}


void mwShots::move_pshots()
{
   // move and process wall collisions
   for (int b=0; b<50; b++)
      if (mShot.p[b].active)
      {
         // move
         mShot.p[b].x += mShot.p[b].xinc;
         mShot.p[b].y += mShot.p[b].yinc;

         // bounds check
         if ((mShot.p[b].x < 5) || (mShot.p[b].x > 1995) || (mShot.p[b].y<5) || (mShot.p[b].y > 1995) ) mShot.p[b].active = 0;

         // level block collision
         int x = ((mShot.p[b].x+10) / 20);
         int y = ((mShot.p[b].y+10) / 20);
         int d = mLevel.l[x][y];
         if ((d & PM_BTILE_SOLID_PBUL) || (d & PM_BTILE_BREAKABLE_PSHOT)) // shot hit solid or breakable wall
         {
            mShot.p[b].active = 0;  // shot is done
            if (d & PM_BTILE_BREAKABLE_PSHOT) mLevel.change_block(x, y, 0);
         }
      }
}

void mwShots::draw_pshots()
{
   for (int b=0; b<50; b++)
      if (mShot.p[b].active) al_draw_bitmap(mwB.player_tile[mPlayer.syn[mShot.p[b].player].color][18], mShot.p[b].x, mShot.p[b].y, 0);
}

void mwShots::proc_eshot_collision(int p, int b)
{
   int damage = 0;
   int e_type = 0;
   switch (mShot.e[b].shape)
   {
      case 488:   e_type = 3;  damage = 5;  break; // arrow
      case 489:   e_type = 3;  damage = 5;  break; // arrow
      case 1055:  e_type = 6;  damage = 7;  break; // cannon ball
      case 1020:  e_type = 8;  damage = 9;  break; // yellow things
      case 1054:  e_type = 7;  damage = 10; break; // green ball
      case 1062:  e_type = 12; damage = 8;  break; // flapper thing
   }
   mPlayer.syn[p].health -= damage;

   game_event(43, 0, 0, p, e_type, 0, damage);

   // recoil !!
   if (mShot.e[b].xinc > 0)
   {
      mPlayer.syn[p].right_xinc += mShot.e[b].xinc/2;
      if (mPlayer.syn[p].right_xinc > 4) mPlayer.syn[p].right_xinc = 4;
   }
   if (mShot.e[b].xinc < 0)
   {
      mPlayer.syn[p].left_xinc += mShot.e[b].xinc/2;
      if (mPlayer.syn[p].left_xinc < -4) mPlayer.syn[p].left_xinc = -4;
   }

   mPlayer.syn[p].yinc += mShot.e[b].yinc/2;

   if (mPlayer.syn[p].yinc >  5) mPlayer.syn[p].yinc =  5;
   if (mPlayer.syn[p].yinc < -8) mPlayer.syn[p].yinc = -8;

   mShot.e[b].active = 0; // shot dies
}

void mwShots::move_eshots()
{
   for (int b=0; b<50; b++)
      if (mShot.e[b].active)
      {
         mShot.e[b].x += mShot.e[b].xinc;
         mShot.e[b].y += mShot.e[b].yinc;

         // check if out of bounds
         if ((mShot.e[b].x<0) || (mShot.e[b].x>2000) || (mShot.e[b].y<0) || (mShot.e[b].y>2000)) mShot.e[b].active = 0;

         // check if hit wall (or more accurately if co-located with a block)
         int xi = (mShot.e[b].x+10)/20;
         int yi = (mShot.e[b].y+10)/20;
         int d = mLevel.l[xi][yi];
         if (d & PM_BTILE_SOLID_EBUL)  // shot hit solid or breakable wall
         {
            mShot.e[b].active = 0;                                   // shot dies
            if (d & PM_BTILE_BREAKABLE_ESHOT) mLevel.change_block(xi, yi, 0); // breakable wall
         }
      }
}


void mwShots::draw_eshots()
{

/*
   // temp testing
   al_draw_circle(tx1+10, ty1+10, 10, mC.pc[ttc1], 2 );
   al_draw_textf(mF.pr8, mC.pc[ttc1], tx1, ty1-10, 0, "%.3f",ttfloat1);


   al_draw_circle(tx2+10, ty2+10, 10, mC.pc[ttc2], 2 );
   al_draw_textf(mF.pr8, mC.pc[ttc2], tx2, ty2+30, 0, "%.3f",ttfloat2);

   // temp testing
   al_draw_rectangle(tx1, ty1, tx2, ty2, mC.pc[ttc1], 2 );
*/

   for (int b=0; b<50; b++)
      if (mShot.e[b].active)
      {
         int t = mShot.e[b].shape;
         if (t > 1000) t = mwB.zz[0][mShot.e[b].shape-1000];
         al_draw_bitmap(mwB.tile[t], mShot.e[b].x, mShot.e[b].y, 0);
      }
}

void mwShots::clear_shots(void)
{
   for (int b=0; b<50; b++)
   {
      mShot.e[b].active = 0;
      mShot.e[b].shape  = 0;
      mShot.e[b].xinc   = 0;
      mShot.e[b].yinc   = 0;
      mShot.e[b].y      = 0;
      mShot.e[b].x      = 0;

      mShot.p[b].active = 0;
      mShot.p[b].player = 0;
      mShot.p[b].xinc   = 0;
      mShot.p[b].yinc   = 0;
      mShot.p[b].y      = 0;
      mShot.p[b].x      = 0;

   }
}


void mwShots::fire_enemy_shotz(int e, int shot_ans, float px, float py)
{
   float xlen = px - mEnemy.Ef[e][0];   // get the x distance between enemy and player
   float ylen = py - mEnemy.Ef[e][1];   // get the y distance between enemy and player
   float hy_dist = sqrt(pow(xlen, 2) + pow(ylen, 2));    // hypotenuse distance
   float speed = mEnemy.Ef[e][7];            // speed
   float scaler = hy_dist / speed;    // get scaler
   float xinc = xlen / scaler;        // calc xinc
   float yinc = ylen / scaler;        // calc yinc

   for (int b=0; b<50; b++)  // find empty e_shot
      if (!mShot.e[b].active)
      {
         mShot.e[b].active = 1;
         mShot.e[b].shape = 1000 + shot_ans;
         mShot.e[b].x = mEnemy.Ef[e][0];
         mShot.e[b].y = mEnemy.Ef[e][1];
         mShot.e[b].xinc = xinc;
         mShot.e[b].yinc = yinc;
         b=50;
      }
}
void mwShots::fire_enemy_shota(int e, int shot_ans, int p)
{
   float bx = mEnemy.Ef[e][0];
   float by = mEnemy.Ef[e][1];
   float bv = mEnemy.Ef[e][7];

   float px  = mPlayer.syn[p].x;
   float py  = mPlayer.syn[p].y;
   float pvx = mPlayer.syn[p].xinc;
   float pvy = mPlayer.syn[p].yinc;


   // this is here so that motion due to riding lifts is also used for shot tracking
   if (mPlayer.syn[p].player_ride) // if player is riding lift
   {
      int d = mPlayer.syn[p].player_ride - 32; // lift number
      pvx += mLift.cur[d].xinc;
      pvy += mLift.cur[d].yinc;
   }
   // Edgar's method
   //float A = pow(pvx,2) + pow(pvy,2) - pow(bv,2);
   //float B = 2*(px*pvx) + 2*(py*pvy) -2*(bx*pvx) -2*(by*pvy);
   //float C = pow(px,2) + pow(bx,2) + pow(py,2) + pow(by,2) - 2*(bx*px) -2*(by*py);

   // Peter's method
   float x = px-bx;
   float y = py-by;
   float A = pow(pvx,2) + pow(pvy,2) - pow(bv,2);
   float B = 2*(x*pvx) + 2*(y*pvy);
   float C = pow(x,2) + pow(y,2);

// Egdar: You will have to check your code for division by A=0 and for a negative B^2 - 4AC discriminant.

// Quadratic Formula: The roots of a quadratic equation ax2 + bx + c = 0 are given by x = [-b +/- sqrt(b^2 - 4ac)] / 2a.
// The discriminant of the quadratic equation is D = b2 - 4ac
// For D > 0 the roots are real and distinct.
// For D = 0 the roots are real and equal.
// For D < 0 the roots do not exist, or the roots are imaginary.

   float D = pow(B,2) - 4*(A*C); // discriminant

   //if ((A != 0) && (D >= 0)) // this complains about comparing a float to zero
   if ( ((A > 0) || (A < 0)) && (D >= 0))
   {
      float t = ( -B - sqrt(pow(B,2) - 4*(A*C)) ) / (2*A);
      float px1 = px + pvx * t; // get player target position based on t
      float py1 = py + pvy * t;
      fire_enemy_shotz(e, shot_ans, px1, py1);
   }
   else fire_enemy_shotz(e, shot_ans, px, py);

}

void mwShots::fire_enemy_x_shot(int e, int p)
{
   float x_shot_speed = mEnemy.Ef[e][7];
   for (int b=0; b<50; b++)  // find empty e_shot
      if (!mShot.e[b].active)
      {
         mShot.e[b].active = 1;
         mShot.e[b].yinc = 0;
         mShot.e[b].x = mEnemy.Ef[e][0];
         mShot.e[b].y = mEnemy.Ef[e][1];

         if (mEnemy.Ef[e][0] < mPlayer.syn[p].x)
         {
            mShot.e[b].xinc = x_shot_speed;
            mShot.e[b].shape = 488;
         }
         if (mEnemy.Ef[e][0] >= mPlayer.syn[p].x)
         {
            mShot.e[b].xinc = -x_shot_speed;
            mShot.e[b].shape = 489;
         }
         b=50; // end loop
      }
}









