// mwEnemyFnx.cpp

#include "pm.h"
#include "mwEnemy.h"
#include "mwPlayer.h"



// used by vinepod
int mwEnemy::is_player_in_enemy_trigger_box(int e)
{
   int x1 = Ei[e][11] - 10; // trigger box
   int y1 = Ei[e][12] - 10;
   int x2 = Ei[e][11] + Ei[e][13] + 10;
   int y2 = Ei[e][12] + Ei[e][14] + 10;
   for (int p=0; p<NUM_PLAYERS; p++)
      if ((mPlayer.syn[p].active) && (!mPlayer.syn[p].paused))
      {
         int px = mPlayer.syn[p].x;
         int py = mPlayer.syn[p].y;
         if ((px > x1) && (px < x2) && (py > y1) && (py < y2)) return 1;
      }
   return 0;
}



// used in vinepod when moving
// used in bouncer always
// used in cannon when player not in prox range
// used in level editor scale_bouncer_and_cannon_speed(int e)
void mwEnemy::set_enemy_rot_from_incs(int e)
{
   Ef[e][14] = atan2(Ef[e][3], Ef[e][2]) - ALLEGRO_PI/2;
}

// used in cannon when player in prox range
// used in vinepod
void mwEnemy::set_enemy_rot_from_player(int e, int p)
{
   float xlen = mPlayer.syn[p].x - Ef[e][0];
   float ylen = mPlayer.syn[p].y - Ef[e][1];
   Ef[e][14] = atan2(ylen, xlen) - ALLEGRO_PI/2;
}


// used once in bouncer cannon common
void mwEnemy::set_enemy_xyinc_from_player(int e, int p)
{
   float xlen = mPlayer.syn[p].x - Ef[e][0];
   float ylen = mPlayer.syn[p].y - Ef[e][1];
   float hy_dist = sqrt(pow(xlen, 2) + pow(ylen, 2));  // hypotenuse distance
   float speed = Ef[e][5];                              // speed
   float scaler = hy_dist / speed;     // get scaler
   Ef[e][2] = xlen / scaler;         // calc xinc
   Ef[e][3] = ylen / scaler;         // calc 7inc
}

// used by flapper y seek
// used by trakbot falling seek
// used by vinepod shoot
// used by vinepod to set rot when extended
// used by walker_archwagon_common in follow mode
// used by walker_archwagon_common to face left or right when can't move
// used by bouncer cannon common set_seek
int mwEnemy::find_closest_player(int e)
{
   int closest_player = 0; // defaults to zero (will always return a valid player)
   float hd = 99999;
   for (int p=0; p<NUM_PLAYERS; p++)
      if ((mPlayer.syn[p].active) && (!mPlayer.syn[p].paused))
      {
         float xlen = mPlayer.syn[p].x - Ef[e][0];
         float ylen = mPlayer.syn[p].y - Ef[e][1];
         float h = sqrt(pow(xlen, 2) + pow(ylen, 2));  // hypotenuse distance
         if (h < hd)
         {
             hd = h;
             closest_player = p;
         }
      }
   return closest_player;
}


// used only by flapper
int mwEnemy::find_closest_player_flapper(int e)
{
   int dir = Ei[e][2];

   float prox = Ei[e][17];
   float d[NUM_PLAYERS]; // array of distances for each player
   for (int p=0; p<NUM_PLAYERS; p++)
   {
      d[p] = -1;
      if ((mPlayer.syn[p].active) && (!mPlayer.syn[p].paused))
      {
         float xlen = mPlayer.syn[p].x - Ef[e][0];            // get x distance
         float ylen = mPlayer.syn[p].y - Ef[e][1];            // get y distance
         float dist = sqrt(pow(xlen, 2) + pow(ylen, 2));  // hypotenuse distance
         float angle = atan2(ylen, xlen) + ALLEGRO_PI/2;  // get raw angle and add 90 deg in radians
         float da = (angle / (ALLEGRO_PI*2)) * 360;       // convert from radians to degrees
         if (da < 0) da += 360;                           // add 360 if negative

         if ((dir == 1) && (da >  45) && (da < 135) && (dist < prox)) d[p] = dist; // right
         if ((dir == 0) && (da > 225) && (da < 315) && (dist < prox)) d[p] = dist; // left

         // printf("angle:%d\n", ngl);
/*
                       -64
                        |
             -127       |
           128/-128 ----+---- 0
              127       |
                        |
                       +64
        only shoots at players in a 90 degree pie slice
        45  to 135 for +xinc (-32 to +32)
        225 to 315 for -xinc (96 to -96)
*/

      }
   }

   float closest_val = 9999;
   int closest_p = -1;
   for (int p=0; p<NUM_PLAYERS; p++)
   {
      if ((d[p] != -1) && (d[p] < closest_val))
      {
         closest_val = d[p];
         closest_p = p;
      }
   }
   if (closest_val == 9999) return -1;    // no player in range
   else return closest_p;

}


//           normal cartesian
//              I    x+  Y+        II | I
//             II    x-  Y+       ----+----
//             III   x-  Y-       III | IV
//             IV    x+  Y-
//
//             screen
//              I    x+  Y-        II | I
//             II    x-  Y-       ----+----
//             III   x-  Y+       III | IV
//             IV    x+  Y+
// used by trakbot only




//int find_closest_player_quad(int e, int quad, int prox)
int mwEnemy::find_closest_player_trakbot(int e)
{
   int quad = 0;
   float prox = Ei[e][17];
   switch (Ei[e][5])
   {
      case 0: case 5: quad = 1; break; // floor right, lwall up
      case 1: case 4: quad = 2; break; // rwall up floor left
      case 2: case 7: quad = 3; break; // ceil left, rwall down
      case 3: case 6: quad = 4; break; // lwall down, ceil right
   }

   int closest_p = -1;    // return -1 if no player in range
   float closest_val = 3000;
   float d[NUM_PLAYERS];
   for (int p=0; p<NUM_PLAYERS; p++)
   {
      d[p] = -1;
      if ((mPlayer.syn[p].active) && (!mPlayer.syn[p].paused))
      {
         float xlen = mPlayer.syn[p].x - Ef[e][0];            // get x distance
         float ylen = mPlayer.syn[p].y - Ef[e][1];            // get y distance
         float dist = sqrt(pow(xlen, 2) + pow(ylen, 2));  // hypotenuse distance
         float angle = atan2(ylen, xlen) + ALLEGRO_PI/2;  // get raw angle and add 90 deg in radians
         float da = (angle / (ALLEGRO_PI*2)) * 360;       // convert from radians to degrees
         if (da < 0) da += 360;                           // add 360 if negative

         // printf("angle:%f   da:%f\n", angle, da);

         if ((quad == 1) && (da>  0) && (da< 90)) d[p] = dist;
         if ((quad == 4) && (da> 90) && (da<180)) d[p] = dist;
         if ((quad == 3) && (da>180) && (da<270)) d[p] = dist;
         if ((quad == 2) && (da>270) && (da<359)) d[p] = dist;
      }
      if (d[p] > prox) d[p] = -1; // add, if distance is within range
   }
   for (int p=0; p<NUM_PLAYERS; p++)
      if ((d[p] != -1) && (d[p] < closest_val))
      {
         closest_val = d[p];
         closest_p = p;
      }
   if (closest_val == 3000) return -1; // no player in range
   else return closest_p;
}


// finds closest player...if closest player is not within dist, returns -1
// used only by cannon
int mwEnemy::find_closest_player_cannon(int e)
{
   float prox = Ei[e][17];
   int closest_player = -1; // default if no player within distance
   float hd = 99999;
   for (int p=0; p<NUM_PLAYERS; p++)
      if ((mPlayer.syn[p].active) && (!mPlayer.syn[p].paused))
      {
         float xlen = mPlayer.syn[p].x - Ef[e][0];
         float ylen = mPlayer.syn[p].y - Ef[e][1];
         float h = sqrt(pow(xlen, 2) + pow(ylen, 2));  // hypotenuse distance
         if (h < hd)
         {
             hd = h;
             closest_player = p;
         }
      }
   if (hd < prox) return closest_player;
   else return -1;
}

float mwEnemy::deg_to_rad(float deg)
{
   return (deg/360) * ALLEGRO_PI*2;
}


