// mwEnemyVinePod.cpp

#include "pm.h"
#include "mwEnemy.h"
#include "mwPlayer.h"
#include "mwBitmap.h"
#include "mwShot.h"
#include "mwSolid.h"
#include "mwLift.h"
#include "mwLoop.h"
#include "mwColor.h"
#include "mwFont.h"
#include "mwMiscFnx.h"
#include "mwTriggerEvent.h"
#include "mwScreen.h"
#include "mwDisplay.h"



/*
-----------------------------------------------
---  7 - Vinepod  -----------------------------
-----------------------------------------------

Ei[e][3]  = x initial position
Ei[e][4]  = y initial position
Ei[e][5]  = x control point 1
Ei[e][6]  = y control point 1
Ei[e][7]  = x control point 2
Ei[e][8]  = y control point 2
Ei[e][9]  = x extended position
Ei[e][10] = y extended position

Ei[e][11] = trigger box x
Ei[e][12] = trigger box y
Ei[e][13] = trigger box w
Ei[e][14] = trigger box h

Ei[e][15] = mode
Ei[e][16] = sequence counter
Ei[e][17] = sequence limit

Ei[e][18] = input event
Ei[e][19] = output event

Ei[e][20] = flags
Ei[e][21] = unused

Ef[e][6] = control point multiplier
Ef[e][7] = bullet speed
Ef[e][8] = wait count
Ef[e][9] = wait limit

#define PM_ENEMY_VINEPOD_SHOW_PATH   0b00000000000000001
#define PM_ENEMY_VINEPOD_INV_INIT    0b00000000000000010
#define PM_ENEMY_VINEPOD_INV_EXTN    0b00000000000000100
#define PM_ENEMY_VINEPOD_INV_MOVE    0b00000000000001000
#define PM_ENEMY_VINEPOD_USE_SPLINE  0b00000000000010000
#define PM_ENEMY_VINEPOD_USE_TRIGGER 0b00000000000100000
#define PM_ENEMY_VINEPOD_FIRE_SHOT   0b00000000001000000


*/





void mwEnemy::move_vinepod(int e)
{
   if (Ei[e][31]) // hit
   {
      int ek = 0;
      if ((Ei[e][15] == 0)                       && (!(Ei[e][20] & PM_ENEMY_VINEPOD_INV_INIT))) ek = 1;
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

   if (Ei[e][19]) mTriggerEvent.event[Ei[e][19]] = 0;  // clear output event


   if (Ei[e][15] == 0) // mode 0 - wait for trigger
   {
      Ei[e][16] = 0; // reset extend counter
      if ((Ei[e][20] & PM_ENEMY_VINEPOD_USE_TRIGGER) && (is_player_in_enemy_trigger_box(e))) Ei[e][15] = 1; // set next mode
      if ((Ei[e][18]) && (mTriggerEvent.event[Ei[e][18]])) Ei[e][15] = 1;                                   // set next mode
   }
   if (Ei[e][15] == 1) // mode 1 - extend
   {
      if (++Ei[e][16] >= Ei[e][17]) // extend done
      {
         if (Ei[e][19]) mTriggerEvent.event[Ei[e][19]] = 1;  // set output event
         Ei[e][15] = 2;       // set next mode
         Ef[e][8] = Ef[e][9]; // init timer
      }
   }
   if (Ei[e][15] == 2)  // mode 2 - pre-shot pause
   {
      Ef[e][8] -=1;
      if (Ef[e][8] <= 0)
      {
         //if (Ei[e][19]) mTriggerEvent.event[Ei[e][19]] = 1;  // set output event
         if (Ei[e][20] & PM_ENEMY_VINEPOD_FIRE_SHOT) mShot.fire_enemy_shota(e, 54, find_closest_player(e)); // shoot
         Ei[e][15] = 3;       // set next mode
         Ef[e][8] = Ef[e][9]; // init timer
      }
   }
   if (Ei[e][15] == 3)  // mode 3 - post-shot pause
   {
      Ef[e][8] -=1;
      if (Ef[e][8] <= 0) Ei[e][15] = 4; // next mode
   }

   if (Ei[e][15] == 4) // mode 4 - retract
   {
      if (--Ei[e][16] <= 0)  // retract done
      Ei[e][15] = 0; // set next mode (back to wait for trigger)
   }

   // set shape based on how far extended
   int ns = mBitmap.zz[4][15]; // number of shapes
   float v = (float)Ei[e][16] / (float)Ei[e][17]; // ratio
   int s = v*ns;
   //printf("v:%f, 6:%f  7:%f  s:%d\n", v, (float)Ei[e][6], (float)Ei[e][7], s);
   Ei[e][1] = mBitmap.zz[5+s][15];


   if (Ei[e][20] & PM_ENEMY_VINEPOD_USE_SPLINE)
   {
      // create and fill the spline array
      int np = Ei[e][17]+1; // number of points
      float dest[np*2] = {0};
      float pnts[8] = {0};
      vinepod_fill_points_array(e, np, pnts, dest);

      // set position
      int npi = Ei[e][16]*2; // current index into array
      Ef[e][0] = dest[npi]   -10;
      Ef[e][1] = dest[npi+1] -10;

      // set rotation
      float xlen = dest[npi+2] - dest[npi+0];
      float ylen = dest[npi+3] - dest[npi+1];
      Ef[e][14] = atan2(ylen, xlen) - ALLEGRO_PI/2;
   }
   else
   {
      calc_simple_vinepod(e);
      Ef[e][0] = Ei[e][3] + Ef[e][2] * Ei[e][16];
      Ef[e][1] = Ei[e][4] + Ef[e][3] * Ei[e][16];
   }

   if ((Ei[e][15] == 2) || (Ei[e][15] == 3)) set_enemy_rot_from_player(e, find_closest_player(e)); // rotate to face player in wait modes
}


void mwEnemy::vinepod_fill_points_array(int e, int np, float pnts[], float dest[])
{
   // put variables in spline array
   for (int i=0; i<8; i++) pnts[i] = mEnemy.Ei[e][i+3]+10;

   // scaling of control points
   float d = Ef[e][6];
   pnts[2] = pnts[0] + (pnts[2] - pnts[0]) * d;
   pnts[3] = pnts[1] + (pnts[3] - pnts[1]) * d;
   pnts[4] = pnts[0] + (pnts[4] - pnts[0]) * d;
   pnts[5] = pnts[1] + (pnts[5] - pnts[1]) * d;

   al_calculate_spline(dest, 8, pnts, 0, np);
}





void mwEnemy::calc_simple_vinepod(int e)
{
   float xlen = Ei[e][9]  - Ei[e][3];   // get the x distance
   float ylen = Ei[e][10] - Ei[e][4];   // get the y distance
   Ef[e][2] = xlen / Ei[e][17];         // xinc dist/time  (v = d/t)
   Ef[e][3] = ylen / Ei[e][17];         // yinc dist/time  (v = d/t)
   Ef[e][14] = atan2(ylen, xlen) - ALLEGRO_PI/2;  // rotation
}

void mwEnemy::vinepod_set_cp_thirds(int e)
{
   float xd = (float)(Ei[e][9]  - Ei[e][3]) / 3;
   float yd = (float)(Ei[e][10] - Ei[e][4]) / 3;
   Ei[e][5] = Ei[e][3] + xd*1;
   Ei[e][6] = Ei[e][4] + yd*1;
   Ei[e][7] = Ei[e][3] + xd*2;
   Ei[e][8] = Ei[e][4] + yd*2;
}

void mwEnemy::vinepod_rotate_cw_90(int e)
{
   int xa = Ei[e][3];
   int ya = Ei[e][4];

   for (int i=5; i<10; i+=2)
   {
      int xd = Ei[e][i+0] - xa;
      int yd = Ei[e][i+1] - ya;
      Ei[e][i+0] = xa - yd;
      Ei[e][i+1] = ya + xd;
   }
}

void mwEnemy::vinepod_rotate_ccw_90(int e)
{
   int xa = Ei[e][3];
   int ya = Ei[e][4];

   for (int i=5; i<10; i+=2)
   {
      int xd = Ei[e][i+0] - xa;
      int yd = Ei[e][i+1] - ya;
      Ei[e][i+0] = xa + yd;
      Ei[e][i+1] = ya - xd;
   }
}

void mwEnemy::vinepod_rotate(int e, float a)
{
   int xa = Ei[e][3]; // origin
   int ya = Ei[e][4];

   for (int i=5; i<10; i+=2)
   {
      float xd = (float)(Ei[e][i+0] - xa);       // x distance
      float yd = (float)(Ei[e][i+1] - ya);       // y distance
      float oa = atan2(yd, xd);                  // original angle
      float hd = sqrt(pow(xd, 2) + pow(yd, 2));  // hypotenuse distance
      oa+=a;                                     // change angle
      Ei[e][i+0] = xa + cos(oa) * hd;            // get new x
      Ei[e][i+1] = ya + sin(oa) * hd;            // get new y
   }
}


void mwEnemy::vinepod_scale(int e, float s)
{
   int xa = Ei[e][3]; // origin
   int ya = Ei[e][4];

   for (int i=5; i<10; i+=2)
   {
      float xd = (float)(Ei[e][i+0] - xa);       // x distance
      float yd = (float)(Ei[e][i+1] - ya);       // y distance
      float oa = atan2(yd, xd);                  // original angle
      float hd = sqrt(pow(xd, 2) + pow(yd, 2));  // hypotenuse distance

      hd *= s;                                     // change h dis

      Ei[e][i+0] = xa + cos(oa) * hd;            // get new x
      Ei[e][i+1] = ya + sin(oa) * hd;            // get new y
   }
}






void mwEnemy::vinepod_mirror_x(int e)
{
   int xa = Ei[e][3];
   for (int i=5; i<10; i+=2)
      Ei[e][i] = xa - (Ei[e][i] - xa);
}

void mwEnemy::vinepod_mirror_y(int e)
{
   int ya = Ei[e][4];
   for (int i=6; i<11; i+=2)
      Ei[e][i] = ya - (Ei[e][i] - ya);
}



void mwEnemy::draw_vinepod_screen_direct(int e)
{
   float sc = mDisplay.scale_factor_current;

   // screen scale all the points
   float sx0=0, sy0=0, sx1=0, sy1=0, sx2=0, sy2=0, sx3=0, sy3=0;
   mScreen.calc_actual_screen_position(Ei[e][3]+10, Ei[e][ 4]+10, sx0, sy0);
   mScreen.calc_actual_screen_position(Ei[e][5]+10, Ei[e][ 6]+10, sx1, sy1);
   mScreen.calc_actual_screen_position(Ei[e][7]+10, Ei[e][ 8]+10, sx2, sy2);
   mScreen.calc_actual_screen_position(Ei[e][9]+10, Ei[e][10]+10, sx3, sy3);

   float ld = 17;      // distance between leaves
   float ls = 0.7*sc;  // leaf size

   ALLEGRO_COLOR c2 = mColor.pc[9+128];

   if (Ei[e][20] & PM_ENEMY_VINEPOD_USE_SPLINE)
   {
      // create and fill the spline array
      int np = Ei[e][17]+1; // number of points
      float dest[np*2] = {0};
      float pnts[8] = {0};

      // put variables in spline array
      pnts[0] = sx0;
      pnts[1] = sy0;
      pnts[2] = sx1;
      pnts[3] = sy1;
      pnts[4] = sx2;
      pnts[5] = sy2;
      pnts[6] = sx3;
      pnts[7] = sy3;

      // scaling of control points
      float d = Ef[e][6];
      pnts[2] = pnts[0] + (pnts[2] - pnts[0]) * d;
      pnts[3] = pnts[1] + (pnts[3] - pnts[1]) * d;
      pnts[4] = pnts[0] + (pnts[4] - pnts[0]) * d;
      pnts[5] = pnts[1] + (pnts[5] - pnts[1]) * d;

      al_calculate_spline(dest, 8, pnts, 0, np);


      // draw spline path outline
      if (Ei[e][20] & PM_ENEMY_VINEPOD_SHOW_PATH) al_draw_spline(pnts, c2, 0.7*sc);

      // if anything other than mode 0
      if (Ei[e][15])
      {
         // drawing leaves
         float dist = 0;  // extended distance tally
         float tl = ld;   // next leaf distance tally
         int lsa = 0;     // leaf side alternate

         // cycle points as far as vine has extended
         for (int i=2; i<=(Ei[e][16]-0)*2; i+=2)
         {
            int x1 = dest[i-2];
            int y1 = dest[i-1];
            int x2 = dest[i+0];
            int y2 = dest[i+1];

            // draw vine up to extended point
            al_draw_line(x1, y1, x2, y2, c2, 1.5*sc);

            // get distance of this step
            float xlen = x1-x2;
            float ylen = y1-y2;
            float ds = sqrt(pow(xlen, 2) + pow(ylen, 2)) / sc;

            dist += ds; // add to total distance

            if (dist > tl)   // time for a leaf
            {
               tl += ld;     // next leaf point
               lsa = !lsa;   // toggle leaf side
               float ang = atan2(ylen, xlen); // get the angle of a tangent line at this point
               if (lsa) ang+= ALLEGRO_PI;
               al_draw_scaled_rotated_bitmap(mBitmap.tile[311], 10, 20, x1, y1, ls, ls, ang, 0);
            }
         }
      }
   }
   else // non spline
   {
      if (Ei[e][20] & PM_ENEMY_VINEPOD_SHOW_PATH) al_draw_line(sx0, sy0, sx3, sy3, c2, 0.7*sc);

      if (Ei[e][15])
      {
         float x1 = Ei[e][3];
         float y1 = Ei[e][4];
         float sx01, sy01, sx02, sy02;

         // drawing leaves
         float dist = 0;  // extended distance tally
         float tl = ld;   // next leaf distance tally
         int lsa = 0;     // leaf side alternate

         for (int i=0; i<Ei[e][16]; i++)
         {
            float x2 = x1 + Ef[e][2];
            float y2 = y1 + Ef[e][3];

            mScreen.calc_actual_screen_position(x1+10, y1+10, sx01, sy01);
            mScreen.calc_actual_screen_position(x2+10, y2+10, sx02, sy02);

            al_draw_line(sx01, sy01, sx02, sy02, c2, 1.5*sc);

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
               al_draw_scaled_rotated_bitmap(mBitmap.tile[311], 10, 20, sx01, sy01, ls, ls, ang, 0);
            }
            x1 = x2;
            y1 = y2;
         }
      }
   }

    // draw circle at initial position in every mode except at rest
   if (Ei[e][15]) al_draw_filled_circle(sx0, sy0, 2*sc, c2);

   // draw circle at end if fully extended
   if ((Ei[e][15] == 2) || (Ei[e][15] == 3)) al_draw_filled_circle(sx3, sy3, 2*sc, c2);

}



void mwEnemy::draw_vinepod(int e)
{
   float ld = 15;   // distance between leaves

   ALLEGRO_COLOR c2 = mColor.pc[9+128];

   if (Ei[e][20] & PM_ENEMY_VINEPOD_USE_SPLINE)
   {
      // create and fill the spline array
      int np = Ei[e][17]+1; // number of points
      float dest[np*2] = {0};
      float pnts[8] = {0};
      vinepod_fill_points_array(e, np, pnts, dest);

      // draw spline path outline
      if (Ei[e][20] & PM_ENEMY_VINEPOD_SHOW_PATH) al_draw_spline(pnts, c2, 0);

      // if anything other than mode 0
      if (Ei[e][15])
      {
         // drawing leaves
         float dist = 0;  // extended distance tally
         float tl = ld;   // next leaf distance tally
         int lsa = 0;     // leaf side alternate
         float ls = 0.7;  // leaf size

         // cycle points as far as vine has extended
         for (int i=2; i<=(Ei[e][16]-0)*2; i+=2)
         {
            int x1 = dest[i-2];
            int y1 = dest[i-1];
            int x2 = dest[i+0];
            int y2 = dest[i+1];

            // draw vine up to extended point
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
               al_draw_scaled_rotated_bitmap(mBitmap.tile[311], 10, 20, x1, y1, ls, ls, ang, 0);
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
   else
   {
      if (Ei[e][20] & PM_ENEMY_VINEPOD_SHOW_PATH)
         al_draw_line(Ei[e][3]+10, Ei[e][4]+10, Ei[e][9]+10, Ei[e][10]+10, c2, 0);

      if (Ei[e][15])
      {
         float x1 = Ei[e][3];
         float y1 = Ei[e][4];

         // drawing leaves
         float dist = 0;  // extended distance tally
         float tl = ld;   // next leaf distance tally
         int lsa = 0;     // leaf side alternate
         float ls = 0.7;  // leaf size

         for (int i=0; i<Ei[e][16]; i++)
         {
            float x2 = x1 + Ef[e][2];
            float y2 = y1 + Ef[e][3];

            al_draw_line(x1+10, y1+10, x2+10, y2+10, c2, 1.5);

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
               al_draw_scaled_rotated_bitmap(mBitmap.tile[311], 10, 20, x1+10, y1+10, ls, ls, ang, 0);
            }
            x1 = x2;
            y1 = y2;
         }
         // draw circle at initial position
         al_draw_filled_circle(Ei[e][3]+10, Ei[e][4]+10, 2, c2);

         // draw circle at end if fully extended
         if ((Ei[e][15] == 2) || (Ei[e][15] == 3))
         {
            int x1 = Ei[e][9];
            int y1 = Ei[e][10];
            al_draw_filled_circle(x1+10, y1+10, 2, c2);
         }
      }
   }
}


void mwEnemy::draw_vinepod_controls(int e, int legend_highlight)
{
   // enforce that 34 is 01
   Ei[e][3] = Ef[e][0];
   Ei[e][4] = Ef[e][1];

   // initial position
   int ipx = Ei[e][3]+10;
   int ipy = Ei[e][4]+10;

   // extended position
   int epx = Ei[e][9] +10;
   int epy = Ei[e][10]+10;

   // actual legend lines
   // extended is 2
   // trigger is 3, if on
   // trigger off: cp1 = 3 cp2 = 4
   // trigger on:  cp1 = 4 cp2 = 5
   int cp1l = 4;
   int cp2l = 5;
   if (!(Ei[e][20] & PM_ENEMY_VINEPOD_USE_TRIGGER))
   {
      cp1l = 3;
      cp2l = 4;
   }

   if (Ei[e][20] & PM_ENEMY_VINEPOD_USE_SPLINE)
   {
      // create and fill the spline array
      int np = 100; // number of points
      float dest[np*2] = {0};
      float pnts[8] = {0};
      vinepod_fill_points_array(e, np, pnts, dest);

      al_draw_spline(pnts, mColor.pc[10], 0);

      // set initial rotation
      float xlen = dest[4] - dest[0];            // get the x distance
      float ylen = dest[5] - dest[1];            // get the y distance
      Ef[e][14] = atan2(ylen, xlen) - ALLEGRO_PI/2;  // rotation

      // extended position
      int color1 = 10;
      if (legend_highlight == 2) color1 = mColor.flash_color;
      mMiscFnx.crosshairs_full(epx, epy, color1, 1);

      al_draw_textf(mFont.pixl, mColor.White, epx+12, epy-10, 0, "x:%d", epx-ipx);
      al_draw_textf(mFont.pixl, mColor.White, epx+12, epy-2,  0, "y:%d", epy-ipy);

      // set extended rotation
      xlen = dest[198] - dest[194];            // get the x distance
      ylen = dest[199] - dest[193];            // get the y distance
      float ext_rot = atan2(ylen, xlen) - ALLEGRO_PI/2;  // rotation

      // draw mBitmap.tile at extended pos
      al_draw_scaled_rotated_bitmap(mBitmap.tile[Ei[e][1]], 10, 10, epx, epy, 1, 1, ext_rot, ALLEGRO_FLIP_HORIZONTAL);

      // control point 1
      color1 = 6;
      if (legend_highlight == cp1l) color1 = mColor.flash_color;
      int px = Ei[e][5]+10;
      int py = Ei[e][6]+10;
      al_draw_line(ipx, ipy, px, py, mColor.pc[color1], 0);
      al_draw_line(epx, epy, px, py, mColor.pc[color1], 0);
      al_draw_filled_circle(px, py, 3, mColor.pc[color1]);
      al_draw_circle(       px, py, 6, mColor.pc[color1], 1);
      al_draw_textf(mFont.pixl, mColor.White, px+10, py-10, 0, "x:%d", px-ipx);
      al_draw_textf(mFont.pixl, mColor.White, px+10, py-2,  0, "y:%d", py-ipy);

      // control point 2
      color1 = 7;
      if (legend_highlight == cp2l) color1 = mColor.flash_color;
      px = Ei[e][7]+10;
      py = Ei[e][8]+10;
      al_draw_line(ipx, ipy, px, py, mColor.pc[color1], 0);
      al_draw_line(epx, epy, px, py, mColor.pc[color1], 0);
      al_draw_filled_circle(px, py, 3, mColor.pc[color1]);
      al_draw_circle(       px, py, 6, mColor.pc[color1], 1);
      al_draw_textf(mFont.pixl, mColor.pc[15], px+10, py-10, 0, "x:%d", px-ipx);
      al_draw_textf(mFont.pixl, mColor.pc[15], px+10, py-2,  0, "y:%d", py-ipy);
   }

   else
   {
      calc_simple_vinepod(e);
      al_draw_line(epx, epy, ipx, ipy, mColor.pc[10], 0); // draw line from initial pos to extended pos

      int color1 = 10;
      if (legend_highlight == 2) color1 = mColor.flash_color;
      mMiscFnx.crosshairs_full(epx, epy, color1, 1);
      al_draw_scaled_rotated_bitmap(mBitmap.tile[Ei[e][1]], 10, 10, epx, epy, 1, 1, Ef[e][14], ALLEGRO_FLIP_HORIZONTAL); // draw tile at extended pos
   }


   if (Ei[e][20] & PM_ENEMY_VINEPOD_USE_TRIGGER) // draw trigger box
   {
      int color = 14;
      if (legend_highlight == 3) color = mColor.flash_color;
      al_draw_rectangle(Ei[e][11], Ei[e][12], Ei[e][11]+Ei[e][13]+20, Ei[e][12]+Ei[e][14]+20, mColor.pc[color], 1);
   }
}
