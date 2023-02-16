// mwShots.cpp

#include "pm.h"
#include "mwShots.h"
#include "n_netgame.h"
#include "mwBitmap.h"
#include "z_level.h"
#include "z_fnx.h"
#include "z_screen_overlay.h"
#include "z_enemy.h"
#include "z_player.h"
#include "z_lift.h"

mwShots mwS;

void mwShots::proc_pshot_collision(int p, int b)
{
   players[p].LIFE -= al_itofix(deathmatch_shot_damage);

   al_fixed bxinc = al_ftofix(mwS.p[b].xinc / 3);
   al_fixed z = al_itofix(0);
   if (bxinc > z) players[p].right_xinc += bxinc;
   if (bxinc < z) players[p].left_xinc += bxinc;

   if (players[p].right_xinc > al_itofix(4)) players[p].right_xinc = al_itofix(4);
   if (players[p].left_xinc < al_itofix(-4)) players[p].left_xinc = al_itofix(-4);

   players[p].yinc += al_ftofix(mwS.p[b].yinc/2);
   if (players[p].yinc > al_itofix( 5)) players[p].yinc = al_itofix( 5);
   if (players[p].yinc < al_itofix(-5)) players[p].yinc = al_itofix(-5);

   if (mwS.p[b].yinc < 0)  // player got shot from below
   {
      players[p].player_ride = 0;
      players[p].PY += players[p].yinc;
   }
   mwS.p[b].active = 0;  // shot dies
}

void mwShots::proc_player_shoot(int p)
{
   float x = al_fixtof(players[p].PX) ;
   float y = al_fixtof(players[p].PY) ;
   float bs = (float) players[p].shot_speed;

   if (players[p].fire)
   {
      if (players[p].fire_held == 0) // fire button pressed, but not held
      {
         players[p].fire_held = 1;
         if (players[p].shot_wait_counter < 1 )
         {
            players[p].stat_shots_fired++;

            for (int b=0; b<50; b++)     // search for empty shot
               if (!mwS.p[b].active)
               {
                  mwS.p[b].active = 1;
                  mwS.p[b].player = p;
                  mwS.p[b].x = x;
                  mwS.p[b].y = y + 1;
                  mwS.p[b].xinc = 0;
                  mwS.p[b].yinc = 0;

                  if (players[p].left_right) mwS.p[b].x = x+4;
                  else mwS.p[b].x = x-3;

                  if      (players[p].up)    mwS.p[b].yinc = -bs;
                  else if (players[p].down)  mwS.p[b].yinc =  bs;
                  else                       mwS.p[b].xinc = (players[p].left_right*bs*2) - bs;


                  // if this line is not here player cannot shoot breakable blocks
                  // when directly in front of them..
                  if ((!players[p].up) && (!players[p].down) && (players[p].left_right))
                      mwS.p[b].x -=1;

//                  // temp testing
//                  mwS.p[b][2] = x + (players[p].left_right * (40*2) ) - 40;
//                  mwS.p[b][3] = y + 1;

                     // move
                     mwS.p[b].x += mwS.p[b].xinc;  // xinc
                     mwS.p[b].y += mwS.p[b].yinc;  // yinc

//                  // temp testing
//                  mwS.p[b][4] = 0; // xinc
//                  mwS.p[b][5] = 0; // yinc

                  players[p].shot_wait_counter = players[p].shot_wait;
                  players[p].fire_held = 1;

                  // extra data is player number, shot number
                  game_event(1, x, y, p, b, 0, 0);

                  break; // to break out of for loop
               }
         }
      }
   }
   else players[p].fire_held = 0;  // fire is not pressed
   if (players[p].shot_wait_counter > 0) players[p].shot_wait_counter--;
}


void mwShots::move_pshots()
{
   // move and process wall collisions
   for (int b=0; b<50; b++)
      if (mwS.p[b].active)
      {
         // move
         mwS.p[b].x += mwS.p[b].xinc;
         mwS.p[b].y += mwS.p[b].yinc;

         // bounds check
         if ((mwS.p[b].x < 5) || (mwS.p[b].x > 1995) || (mwS.p[b].y<5) || (mwS.p[b].y > 1995) ) mwS.p[b].active = 0;

         // level block collision
         int x = ((mwS.p[b].x+10) / 20);
         int y = ((mwS.p[b].y+10) / 20);
         int d = l[x][y];
         if ((d & PM_BTILE_SOLID_PBUL) || (d & PM_BTILE_BREAKABLE_PSHOT)) // shot hit solid or breakable wall
         {
            mwS.p[b].active = 0;  // shot is done
            if (d & PM_BTILE_BREAKABLE_PSHOT) change_block(x, y, 0);
         }
      }
}

void mwShots::draw_pshots()
{
   for (int b=0; b<50; b++)
      if (mwS.p[b].active) al_draw_bitmap(mwB.player_tile[players[mwS.p[b].player].color][18], mwS.p[b].x, mwS.p[b].y, 0);
}

void mwShots::proc_eshot_collision(int p, int b)
{
   int damage = 0;
   int e_type = 0;
   switch (mwS.e[b].shape)
   {
      case 488:   e_type = 3;  damage = 5;  break; // arrow
      case 489:   e_type = 3;  damage = 5;  break; // arrow
      case 1055:  e_type = 6;  damage = 7;  break; // cannon ball
      case 1020:  e_type = 8;  damage = 9;  break; // yellow things
      case 1054:  e_type = 7;  damage = 10; break; // green ball
      case 1062:  e_type = 12; damage = 8;  break; // flapper thing
   }
   players[p].LIFE -= al_itofix(damage);

   game_event(43, 0, 0, p, e_type, 0, damage);

   // recoil !!
   if (mwS.e[b].xinc > 0)
   {
      players[p].right_xinc += al_ftofix(mwS.e[b].xinc/2);
      if (players[p].right_xinc > al_itofix(4)) players[p].right_xinc = al_itofix(4);
   }
   if (mwS.e[b].xinc < 0)
   {
      players[p].left_xinc += al_ftofix(mwS.e[b].xinc/2);
      if (players[p].left_xinc < al_itofix(-4)) players[p].left_xinc = al_itofix(-4);
   }

   players[p].yinc += al_ftofix(mwS.e[b].yinc/2);

   if (players[p].yinc > al_itofix( 5)) players[p].yinc = al_itofix( 5);
   if (players[p].yinc < al_itofix(-8)) players[p].yinc = al_itofix(-8);

   mwS.e[b].active = 0; // shot dies
}

void mwShots::move_eshots()
{
   for (int b=0; b<50; b++)
      if (mwS.e[b].active)
      {
         mwS.e[b].x += mwS.e[b].xinc;
         mwS.e[b].y += mwS.e[b].yinc;

         // check if out of bounds
         if ((mwS.e[b].x<0) || (mwS.e[b].x>2000) || (mwS.e[b].y<0) || (mwS.e[b].y>2000)) mwS.e[b].active = 0;

         // check if hit wall (or more accurately if co-located with a block)
         int xi = (mwS.e[b].x+10)/20;
         int yi = (mwS.e[b].y+10)/20;
         int d = l[xi][yi];
         if (d & PM_BTILE_SOLID_EBUL)  // shot hit solid or breakable wall
         {
            mwS.e[b].active = 0;                                   // shot dies
            if (d & PM_BTILE_BREAKABLE_ESHOT) change_block(xi, yi, 0); // breakable wall
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
      if (mwS.e[b].active)
      {
         int t = mwS.e[b].shape;
         if (t > 1000) t = mwB.zz[0][mwS.e[b].shape-1000];
         al_draw_bitmap(mwB.tile[t], mwS.e[b].x, mwS.e[b].y, 0);
      }
}

void mwShots::clear_shots(void)
{
   for (int b=0; b<50; b++)
   {
      mwS.e[b].active = 0;
      mwS.e[b].shape  = 0;
      mwS.e[b].xinc   = 0;
      mwS.e[b].yinc   = 0;
      mwS.e[b].y      = 0;
      mwS.e[b].x      = 0;

      mwS.p[b].active = 0;
      mwS.p[b].player = 0;
      mwS.p[b].xinc   = 0;
      mwS.p[b].yinc   = 0;
      mwS.p[b].y      = 0;
      mwS.p[b].x      = 0;

   }
}


void mwShots::fire_enemy_shotz(int EN, int shot_ans, al_fixed px, al_fixed py)
{
   al_fixed xlen = px - Efi[EN][0];   // get the x distance between enemy and player
   al_fixed ylen = py - Efi[EN][1];   // get the y distance between enemy and player
   al_fixed hy_dist =  al_fixhypot(xlen, ylen);     // hypotenuse distance
   al_fixed speed = Efi[EN][7];                  // speed
   al_fixed scaler = al_fixdiv(hy_dist, speed);     // get scaler
   al_fixed xinc = al_fixdiv(xlen, scaler);        // calc xinc
   al_fixed yinc = al_fixdiv(ylen, scaler);        // calc yinc

   for (int b=0; b<50; b++)  // find empty e_shot
      if (!mwS.e[b].active)
      {
         mwS.e[b].active = 1;
         mwS.e[b].shape = 1000 + shot_ans;
         mwS.e[b].x = al_fixtof(Efi[EN][0]);
         mwS.e[b].y = al_fixtof(Efi[EN][1]);
         mwS.e[b].xinc = al_fixtof(xinc);
         mwS.e[b].yinc = al_fixtof(yinc);
         b=50;
      }
}
void mwShots::fire_enemy_shota(int EN, int shot_ans, int p)
{
   float bx = Efi[EN][0];
   float by = Efi[EN][1];
   float bv = Efi[EN][7];

   float px  = players[p].PX;
   float py  = players[p].PY;
   float pvx = players[p].xinc;
   float pvy = players[p].yinc;


   // this is here so that motion due to riding lifts is also used for shot tracking
   if (players[p].player_ride) // if player is riding lift
   {
      int d = players[p].player_ride - 32; // lift number
      pvx += lifts[d].fxinc;
      pvy += lifts[d].fyinc;
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

//   if (A == 0)     printf("A == 0 in function 'fire_enemy_shota'\n");
//   else if (D < 0) printf("negative discriminant in function 'fire_enemy_shota'\n");
//   else
//   {
//      float t = ( -B - sqrt(pow(B,2) - 4*(A*C)) ) / (2*A);
//      al_fixed px1 = px + al_fixmul(pvx, al_ftofix(t)); // get player target position based on t
//      al_fixed py1 = py + al_fixmul(pvy, al_ftofix(t));
//      fire_enemy_shotz(EN, shot_ans, px1, py1);
//   }
//

   //if ((A != 0) && (D >= 0)) // this complains about comparing a float to zero
   if ( ((A > 0) || (A < 0)) && (D >= 0))
   {
      float t = ( -B - sqrt(pow(B,2) - 4*(A*C)) ) / (2*A);
      al_fixed px1 = px + al_fixmul(pvx, al_ftofix(t)); // get player target position based on t
      al_fixed py1 = py + al_fixmul(pvy, al_ftofix(t));
      fire_enemy_shotz(EN, shot_ans, px1, py1);
   }
   else fire_enemy_shotz(EN, shot_ans, px, py);

}

void mwShots::fire_enemy_x_shot(int EN, int p)
{
   float x_shot_speed = al_fixtof(Efi[EN][7]);
   for (int b=0; b<50; b++)  // find empty e_shot
      if (!mwS.e[b].active)
      {
         mwS.e[b].active = 1;
         mwS.e[b].yinc = al_itofix(0);
         mwS.e[b].x = al_fixtof(Efi[EN][0]);
         mwS.e[b].y = al_fixtof(Efi[EN][1]);
         if (Efi[EN][0] < players[p].PX)
         {
            mwS.e[b].xinc = x_shot_speed;
            mwS.e[b].shape = 488;
         }
         if (Efi[EN][0] >= players[p].PX)
         {
            mwS.e[b].xinc = -x_shot_speed;
            mwS.e[b].shape = 489;
         }
         b=50; // end loop
      }
}









