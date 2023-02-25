// mwEnemyVinePod.cpp

#include "pm.h"
#include "mwEnemy.h"
#include "mwPlayers.h"
#include "mwBitmap.h"
#include "mwShots.h"
#include "mwSolid.h"
#include "mwLift.h"
#include "mwLoop.h"
#include "mwColor.h"
#include "mwFont.h"
#include "mwMiscFnx.h"










//--7--podzilla-----------------------------------------------------------------------------
//     Ei[e][5] = mode
//     Ei[e][6] = sequence counter
//     Ei[e][7] = sequence limit
//     Ei[e][8] = wait count
//     Ei[e][9] = wait limit
//     Ei[e][11] = trigger box x
//     Ei[e][12] = trigger box y
//     Ei[e][13] = trigger box w
//     Ei[e][14] = trigger box h
void mwEnemy::move_podzilla(int e)
{
   if (Ei[e][31]) // podzilla hit
   {
      //if (Ei[e][5] == 2)  // mode 2; wait then shoot
      {
         enemy_killed(e);
         return;
      }
      //else Ei[e][31] = 0;

   }
   enemy_player_hit_proc(e);

   if (Ei[e][5] == 0) // mode 0 - wait for trigger
   {
      Ei[e][6] = 0; // extend counter
      if (is_player_in_enemy_trigger_box(e)) Ei[e][5] = 1;   // set next mode
   }
   if (Ei[e][5] == 1) // mode 1; extend
   {
      Ef[e][0] += Ef[e][2];
      Ef[e][1] += Ef[e][3];
      if (++Ei[e][6] >= Ei[e][7]) // extend done
      {
         Ei[e][5] = 2; // set next mode
         Ei[e][8] = Ei[e][9];
      }
   }
   if (Ei[e][5] == 2)  // mode 2; wait then shoot
   {

      if (--Ei[e][8] <= 0)
      {
         int p = find_closest_player(e);
         mShot.fire_enemy_shota(e, 54, p);
         Ei[e][5] = 3; // set next mode
         Ei[e][8] = Ei[e][9];
      }
   }
   if (Ei[e][5] == 3)  // mode 3; post shoot pause
   {
      if (--Ei[e][8] <= 0) Ei[e][5] = 4; // next mode
   }

   if (Ei[e][5] == 4) // mode 4; retract
   {
      Ef[e][0] -= Ef[e][2];
      Ef[e][1] -= Ef[e][3];
      if (--Ei[e][6] <= 0)  // retract done
      Ei[e][5] = 0; // set next mode (back to trigger)
   }

   // set shape based on how far it has retracted
   int ns = mwB.zz[4][15]; // number of shapes
   float v = (float)Ei[e][6] / (float)Ei[e][7]; // ratio
   int s = v*ns;
   //printf("v:%f, 6:%f  7:%f  s:%d\n", v, (float)Ei[e][6], (float)Ei[e][7], s);
   Ei[e][1] = mwB.zz[5+s][15];

   // set rotation
   if (!Ei[e][5]) set_enemy_rot_from_incs(e);                            // rotate to face direction of movement
   else           set_enemy_rot_from_player(e, find_closest_player(e));  // rotate to face player in any mode except wait
}


//--13--vinepod-----------------------------------------------------------------------------

//     Ei[e][3]  = x initial position
//     Ei[e][4]  = y initial position
//     Ei[e][5]  = x control point 1
//     Ei[e][6]  = y control point 1
//     Ei[e][7]  = x control point 2
//     Ei[e][8]  = y control point 2
//     Ei[e][9]  = x extended position
//     Ei[e][10] = y extended position

//     Ei[e][11] = trigger box x
//     Ei[e][12] = trigger box y
//     Ei[e][13] = trigger box w
//     Ei[e][14] = trigger box h
//     Ei[e][15] = mode
//     Ei[e][16] = sequence counter
//     Ei[e][17] = sequence limit
//     Ei[e][18] = wait count
//     Ei[e][19] = wait limit

//     Ei[e][20] = flags
//     Ei[e][21] = unused


void mwEnemy::move_vinepod(int e)
{
   if (Ei[e][31]) // hit
   {
      int ek = 0;
      if ((Ei[e][15] == 0) && (!(Ei[e][20] & PM_ENEMY_VINEPOD_INV_INIT))) ek = 1;
      if (((Ei[e][15] == 2) || (Ei[e][15] == 3)) && (!(Ei[e][20] & PM_ENEMY_VINEPOD_INV_EXTN))) ek = 1;
      if (((Ei[e][15] == 1) || (Ei[e][15] == 4)) && (!(Ei[e][20] & PM_ENEMY_VINEPOD_INV_MOVE))) ek = 1;

      if (ek)
      {
         enemy_killed(e);
         return; // break;  to stop rest of execution
      }
      else Ei[e][31] = 0; // clear hit
   }
   enemy_player_hit_proc(e);

   if (Ei[e][15] == 0) // mode 0 - wait for trigger
   {
      Ei[e][16] = 0; // extend counter
      if (is_player_in_enemy_trigger_box(e)) Ei[e][15] = 1;   // set next mode
   }
   if (Ei[e][15] == 1) // mode 1; extend
   {
      if (++Ei[e][16] >= Ei[e][17]-1) // extend done
      {
         Ei[e][15] = 2; // set next mode
         Ei[e][18] = Ei[e][19];
      }
   }
   if (Ei[e][15] == 2)  // mode 2; wait then shoot
   {
      if (--Ei[e][18] <= 0)
      {
         int p = find_closest_player(e);
         mShot.fire_enemy_shota(e, 54, p);
         Ei[e][15] = 3; // set next mode
         Ei[e][18] = Ei[e][19];
      }
   }
   if (Ei[e][15] == 3)  // mode 3; post shoot pause
   {
      if (--Ei[e][18] <= 0) Ei[e][15] = 4; // next mode
   }

   if (Ei[e][15] == 4) // mode 4; retract
   {
      if (--Ei[e][16] <= 0)  // retract done
      Ei[e][15] = 0; // set next mode (back to trigger)
   }

   // set shape based on how far it has extended
   int ns = mwB.zz[4][15]; // number of shapes
   float v = (float)Ei[e][16] / (float)Ei[e][17]; // ratio
   int s = v*ns;
   //printf("v:%f, 6:%f  7:%f  s:%d\n", v, (float)Ei[e][6], (float)Ei[e][7], s);
   Ei[e][1] = mwB.zz[5+s][15];


   // put variables in spline array
   float pnts[8];
   for (int i=0; i<8; i++) pnts[i] = Ei[e][i+3];

   // fill array of points from the spline
   int np = Ei[e][17]; // number of points
   float dest[np*2];
   al_calculate_spline(dest, 8, pnts, 0, np);

   // set position
   int npi = Ei[e][16]*2; // index into array
   Ef[e][0] = dest[npi];
   Ef[e][1] = dest[npi+1];

   // set rotation
   float xlen = dest[npi+2] - dest[npi+0];  // get the x distance
   float ylen = dest[npi+3] - dest[npi+1];  // get the y distance
   Ef[e][14] = atan2(ylen, xlen) - ALLEGRO_PI/2;  // rotation

   if ((Ei[e][15] == 2) || (Ei[e][15] == 3)) set_enemy_rot_from_player(e, find_closest_player(e)); // rotate to face player in wait modes

}





void mwEnemy::draw_vinepod(int e, int x, int c, int custom)
{
   if (!custom)
   {
      ALLEGRO_COLOR c2 = mC.pc[9+128];

      // put variables in spline array
      float pnts[8];
      for (int i=0; i<8; i++) pnts[i] = Ei[e][i+3]+10;

      // draw spline path outline
      if (Ei[e][20] & PM_ENEMY_VINEPOD_SHOW_PATH) al_draw_spline(pnts, c2, 0);

      // if anything other than mode 0
      if (Ei[e][15])
      {
         // fill array of points from the spline
         int np = Ei[e][17]; // number of points
         float dest[np*2];
         al_calculate_spline(dest, 8, pnts, 0, np);


         // drawing leaves
         float dist = 0;  // extended distance tally
         float ld = 15;   // distance between leaves
         float tl = ld;   // next leaf distance tally
         int lsa = 0;     // leaf side alternate
         float ls = 0.7;  // leaf size

         // cycle points as far as vine has extended
         for (int i=2; i<=Ei[e][16]*2; i+=2)
         {
            int x1 = dest[i-2];
            int y1 = dest[i-1];
            int x2 = dest[i+0];
            int y2 = dest[i+1];

            // draw vine up to extened point
            al_draw_line(x1, y1, x2, y2, c2, 1.5);

            // get distance of this step
            float xlen = x1-x2;
            float ylen = y1-y2;
            float ds = sqrt(pow(xlen, 2) + pow(ylen, 2));

            dist += ds; // add to total distance

            if (dist > tl)   // time for a leaf
            {
               tl += ld;     // next leaf point
               lsa = !lsa;   // toggle leaf side
               float ang = atan2(ylen, xlen); // get the angle of a tangent line at this point
               if (lsa) ang+= ALLEGRO_PI;
               al_draw_scaled_rotated_bitmap(mwB.tile[311], 10, 20, x1, y1, ls, ls, ang, 0);
            }
         }

         // draw circle at initial position
         al_draw_filled_circle(dest[0], dest[1], 2, c2);

         // draw circle at end if fully extended
         if ((Ei[e][15] == 2) || (Ei[e][15] == 3))
         {
            int x1 = dest[Ei[e][17]*2-2];
            int y1 = dest[Ei[e][17]*2-1];
            al_draw_filled_circle(x1, y1, 2, c2);
         }
      }
   }
}


void mwEnemy::draw_vinepod_controls(int num, int legend_highlight)
{
   // enforce that 34 is 01
   mEnemy.Ei[num][3] = mEnemy.Ef[num][0];
   mEnemy.Ei[num][4] = mEnemy.Ef[num][1];

   // put variables in spline array
   float pnts[8];
   for (int i=0; i<8; i++) pnts[i] = mEnemy.Ei[num][i+3]+10;

   al_draw_spline(pnts, mC.pc[10], 0);

   // fill array of points from the spline
   float dest[200];
   al_calculate_spline(dest, 8, pnts, 0, 100);

   // initial position
   int ipx = mEnemy.Ei[num][3];
   int ipy = mEnemy.Ei[num][4];

   // set initial rotation
   float xlen = dest[4] - dest[0];            // get the x distance
   float ylen = dest[5] - dest[1];            // get the y distance
   mEnemy.Ef[num][14] = atan2(ylen, xlen) - ALLEGRO_PI/2;  // rotation

   // extended position
   int color1 = 10;
   if (legend_highlight == 2) color1 = mC.flash_color;
   int epx = mEnemy.Ei[num][9];
   int epy = mEnemy.Ei[num][10];
   mMiscFnx.crosshairs_full(epx+10, epy+10, color1, 1);

   al_draw_textf(mF.pixl, mC.White, epx+22, epy+0, 0, "x:%d", epx-ipx);
   al_draw_textf(mF.pixl, mC.White, epx+22, epy+8, 0, "y:%d", epy-ipy);

   // set extended rotation
   xlen = dest[198] - dest[194];            // get the x distance
   ylen = dest[199] - dest[193];            // get the y distance
   float ext_rot = atan2(ylen, xlen) - ALLEGRO_PI/2;  // rotation

   // draw mwB.tile at extended pos
   al_draw_scaled_rotated_bitmap(mwB.tile[mEnemy.Ei[num][1]], 10, 10, epx+10, epy+10, 1, 1, ext_rot, ALLEGRO_FLIP_HORIZONTAL);

   // control point 1
   color1 = 6;
   if (legend_highlight == 3) color1 = mC.flash_color;
   int px = mEnemy.Ei[num][5];
   int py = mEnemy.Ei[num][6];
   al_draw_line(ipx+10, ipy+10, px+10, py+10, mC.pc[color1], 0);
   al_draw_line(epx+10, epy+10, px+10, py+10, mC.pc[color1], 0);
   al_draw_filled_circle(px+10, py+10, 3, mC.pc[color1]);
   al_draw_circle(px+10, py+10, 6, mC.pc[color1], 1);
   al_draw_textf(mF.pixl, mC.White, px+20, py+0, 0, "x:%d", px-ipx);
   al_draw_textf(mF.pixl, mC.White, px+20, py+8, 0, "y:%d", py-ipy);



   // control point 2
   color1 = 7;
   if (legend_highlight == 4) color1 = mC.flash_color;
   px = mEnemy.Ei[num][7];
   py = mEnemy.Ei[num][8];
   al_draw_line(ipx+10, ipy+10, px+10, py+10, mC.pc[color1], 0);
   al_draw_line(epx+10, epy+10, px+10, py+10, mC.pc[color1], 0);
   al_draw_filled_circle(px+10, py+10, 3, mC.pc[color1]);
   al_draw_circle(px+10, py+10, 6, mC.pc[color1], 1);
   al_draw_textf(mF.pixl, mC.pc[15], px+20, py+0, 0, "x:%d", px-ipx);
   al_draw_textf(mF.pixl, mC.pc[15], px+20, py+8, 0, "y:%d", py-ipy);

   // trigger box
   int color = 14;
   if (legend_highlight == 5) color = mC.flash_color;
   int tx1 = mEnemy.Ei[num][11];
   int ty1 = mEnemy.Ei[num][12];
   int tx2 = mEnemy.Ei[num][11]+mEnemy.Ei[num][13] + 20;
   int ty2 = mEnemy.Ei[num][12]+mEnemy.Ei[num][14] + 20;
   al_draw_rectangle(tx1, ty1, tx2, ty2, mC.pc[color], 1);


}




















