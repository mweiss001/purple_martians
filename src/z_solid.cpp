// z_solid.cpp
#include "pm.h"
#include "mwLift.h"
#include "mwLevel.h"


// helper function for is_right_solid, is_left_solid, is_down_solid
int is_solid(int b, int c, int type)
{
   if (b == -1) // do c only
   {
      if ((type == 1) && (c & PM_BTILE_SOLID_PLAYER)) return 1;
      if ((type == 2) && (c & PM_BTILE_SOLID_ENEMY))  return 1;
      if ((type == 3) && (c & PM_BTILE_SOLID_ITEM))   return 1;
      if ((type == 4) && (c & PM_BTILE_SOLID_PBUL))   return 1;
      if ((type == 5) && (c & PM_BTILE_SOLID_EBUL))   return 1;
   }
   else
   {
      if ((type == 1) && ((b & PM_BTILE_SOLID_PLAYER) || (c & PM_BTILE_SOLID_PLAYER)))  return 1;
      if ((type == 2) && ((b & PM_BTILE_SOLID_ENEMY)  || (c & PM_BTILE_SOLID_ENEMY)))   return 1;
      if ((type == 3) && ((b & PM_BTILE_SOLID_ITEM)   || (c & PM_BTILE_SOLID_ITEM)))    return 1;
      if ((type == 4) && ((b & PM_BTILE_SOLID_PBUL)   || (c & PM_BTILE_SOLID_PBUL)))    return 1;
      if ((type == 5) && ((b & PM_BTILE_SOLID_EBUL)   || (c & PM_BTILE_SOLID_EBUL)))    return 1;
   }
   return 0;
}

// custom type for is_up_solid, to take into account semisolid exceptions
int is_solidu(int b, int c, int type)
{
   if (b == -1) // do c only
   {
      if ((type == 1) && (c & PM_BTILE_SOLID_PLAYER) && (!(c & PM_BTILE_SEMISOLID_PLAYER))) return 1;
      if ((type == 2) && (c & PM_BTILE_SOLID_ENEMY)  && (!(c & PM_BTILE_SEMISOLID_ENEMY)))  return 1;
      if ((type == 3) && (c & PM_BTILE_SOLID_ITEM)   && (!(c & PM_BTILE_SEMISOLID_ITEM)))   return 1;
      if ((type == 4) && (c & PM_BTILE_SOLID_PBUL)) return 1;
      if ((type == 5) && (c & PM_BTILE_SOLID_EBUL)) return 1;
   }
   else
   {
      if ( (type == 1) &&
           ( ((b & PM_BTILE_SOLID_PLAYER) && (!(b & PM_BTILE_SEMISOLID_PLAYER))) ||
             ((c & PM_BTILE_SOLID_PLAYER) && (!(c & PM_BTILE_SEMISOLID_PLAYER))) )
         ) return 1;
     if ( (type == 2) &&
           ( ((b & PM_BTILE_SOLID_ENEMY)  && (!(b & PM_BTILE_SEMISOLID_ENEMY))) ||
             ((c & PM_BTILE_SOLID_ENEMY)  && (!(c & PM_BTILE_SEMISOLID_ENEMY))) )
         ) return 1;
      if ( (type == 3) &&
           ( ((b & PM_BTILE_SOLID_ITEM)   && (!(b & PM_BTILE_SEMISOLID_ITEM))) ||
             ((c & PM_BTILE_SOLID_ITEM)   && (!(c & PM_BTILE_SEMISOLID_ITEM))) )
         ) return 1;
      if ((type == 4) && ((b & PM_BTILE_SOLID_PBUL)   || (c & PM_BTILE_SOLID_PBUL)))    return 1;
      if ((type == 5) && ((b & PM_BTILE_SOLID_EBUL)   || (c & PM_BTILE_SOLID_EBUL)))    return 1;
   }
   return 0;
}

int lift_check_helper(int solid_x, int solid_y, int lift_check, int type, int dir)
{
   if (lift_check)
      for (int d=0; d<NUM_LIFTS; d++)
         if (Lift.cur[d].active)
         {
            int x1 = Lift.cur[d].x;
            int y1 = Lift.cur[d].y;
            int x2 = Lift.cur[d].x + Lift.cur[d].w;
            int y2 = Lift.cur[d].y + Lift.cur[d].h;

            if ( ((type == 1) && (Lift.cur[d].flags & PM_LIFT_SOLID_PLAYER)) ||
                 ((type == 2) && (Lift.cur[d].flags & PM_LIFT_SOLID_ENEMY )) ||
                 ((type == 3) && (Lift.cur[d].flags & PM_LIFT_SOLID_ITEM  )) )
            {
               if ((dir == 0) && (solid_y > y1 - 18) && (solid_y < y2 - 2) && (solid_x < x1 - 8) && (solid_x > x1 - 18)) return 32+d; // right
               if ((dir == 1) && (solid_y > y1 - 18) && (solid_y < y2 - 2) && (solid_x < x2 + 2) && (solid_x > x2 - 8 )) return 32+d; // left
               if ((dir == 2) && (solid_x > x1 - 18) && (solid_x < x2 - 2) && (solid_y < y1 -10) && (solid_y > y1 - 25)) return 32+d; // down
               if ((dir == 3) && (solid_x > x1 - 18) && (solid_x < x2 - 2) && (solid_y < y2 + 2) && (solid_y > y2 - 10)) return 32+d; // up
            }
         }
   return 0;
}

// returns 1 for solid block, 32+lift_num for lift
int is_right_solid(int solid_x, int solid_y, int lift_check, int type)
{
   int lch = lift_check_helper(solid_x, solid_y, lift_check, type, 0);
   if (lch) return lch;


//   if (lift_check)
//      for (int d=0; d<NUM_LIFTS; d++)
//         if (lifts[d].active)
//         {
//            int x1 = lifts[d].x;
//            int y1 = lifts[d].y;
//            int x2 = lifts[d].x + lifts[d].w;
//            int y2 = lifts[d].y + lifts[d].h;
//
//            if ( ((type == 1) && (lifts[d].flags & PM_LIFT_SOLID_PLAYER)) ||
//                 ((type == 2) && (lifts[d].flags & PM_LIFT_SOLID_ENEMY )) ||
//                 ((type == 3) && (lifts[d].flags & PM_LIFT_SOLID_ITEM  )) )
//            if ((solid_y > - 18) && (solid_y < y2 - 2) && (solid_x < x1 - 8) && (solid_x > lifts[d].x1 - 18)) return 32+d;
//         }

   int xx = solid_x / 20 + 1;
   int yy = solid_y / 20;
   int cc = solid_y / 20 + 1;
   int a = solid_y % 20;

   if (a > 16)  // next block only
      if (is_solid( -1, mLevel.l[xx][cc], type)) return 1;

   if (a < 3)    // this block only
      if (is_solid( -1, mLevel.l[xx][yy], type)) return 1;

   if ((a > 2) && (a <17)) // dual compare with ||
      if (is_solid( mLevel.l[xx][yy], mLevel.l[xx][cc], type)) return 1;

   return 0;
}

// returns 1 for solid block, 32+lift_num for lift
int is_left_solid(int solid_x, int solid_y, int lift_check, int type)
{
   int lch = lift_check_helper(solid_x, solid_y, lift_check, type, 1);
   if (lch) return lch;

   int xx = solid_x / 20;
   int yy = solid_y / 20;
   int cc = solid_y / 20 + 1;
   int a = solid_y % 20;

//   if (lift_check)
//      for (int d=0; d<NUM_LIFTS; d++)
//         if (lifts[d].active)
//            if ( ((type == 1) && (lifts[d].flags & PM_LIFT_SOLID_PLAYER)) ||
//                 ((type == 2) && (lifts[d].flags & PM_LIFT_SOLID_ENEMY )) ||
//                 ((type == 3) && (lifts[d].flags & PM_LIFT_SOLID_ITEM  )) )
//            if (solid_y > lifts[d].y1 - 18)
//               if (solid_y < lifts[d].y2 - 2)
//                  if (solid_x < lifts[d].x2 + 2)
//                     if (solid_x > lifts[d].x2 - 8)
//                        return 32+d;

   if (a > 16)  // next block only
      if (is_solid( -1, mLevel.l[xx][cc], type)) return 1;

   if (a < 3)    // this block only
      if (is_solid( -1, mLevel.l[xx][yy], type)) return 1;

   if ((a > 2) && (a <17)) // dual compare with ||
      if (is_solid( mLevel.l[xx][yy], mLevel.l[xx][cc], type)) return 1;

   return 0;
}

// returns 1 for solid block, 32+lift_num for lift
int is_down_solid(int solid_x, int solid_y, int lift_check, int type)
{
   int lch = lift_check_helper(solid_x, solid_y, lift_check, type, 2);
   if (lch) return lch;
//   if (lift_check)
//      for (int d=0; d<NUM_LIFTS; d++)
//         if (lifts[d].active)
//            if ( ((type == 1) && (lifts[d].flags & PM_LIFT_SOLID_PLAYER)) ||
//                 ((type == 2) && (lifts[d].flags & PM_LIFT_SOLID_ENEMY )) ||
//                 ((type == 3) && (lifts[d].flags & PM_LIFT_SOLID_ITEM  )) )
//               if (solid_x > lifts[d].x1-18)
//                  if (solid_x < lifts[d].x2-2)
//                     if (solid_y > lifts[d].y1 - 25)
//                        if (solid_y < lifts[d].y1 - 10)
//                            return d+32;
//

   int yy = solid_y / 20 + 1;
   int cc = solid_x / 20;
   int xx = solid_x / 20 + 1;
   int a = solid_x % 20;
   if (a > 16)  // next block only
      if (is_solid( -1, mLevel.l[xx][yy], type)) return 1;

   if (a < 3)    // this block only
      if (is_solid( -1, mLevel.l[cc][yy], type)) return 1;

   if ((a > 2) && (a <17)) // dual compare with ||
      if (is_solid( mLevel.l[xx][yy], mLevel.l[cc][yy], type)) return 1;
   return 0;
}

// returns 1 for solid block, 32+lift_num for lift
int is_up_solid(int solid_x, int solid_y, int lift_check, int type)
{
   int lch = lift_check_helper(solid_x, solid_y, lift_check, type, 3);
   if (lch) return lch;

//   if (lift_check)
//      for (int d=0; d<NUM_LIFTS; d++)
//         if (lifts[d].active)
//            if ( ((type == 1) && (lifts[d].flags & PM_LIFT_SOLID_PLAYER)) ||
//                 ((type == 2) && (lifts[d].flags & PM_LIFT_SOLID_ENEMY )) ||
//                 ((type == 3) && (lifts[d].flags & PM_LIFT_SOLID_ITEM  )) )
//            if (solid_x > lifts[d].x1 - 18)
//               if (solid_x < lifts[d].x2 - 2)
//                  if (solid_y < lifts[d].y2 + 2)
//                     if (solid_y > lifts[d].y2 - 10)
//                       return d+32;


   int yy = (solid_y - 2) / 20;
   int cc = solid_x / 20;
   int xx = solid_x / 20 + 1;
   int a = solid_x % 20;

   if (a > 16)  // next block only
      if (is_solidu( -1, mLevel.l[xx][yy], type)) return 1;

   if (a < 3)    // this block only
      if (is_solidu( -1, mLevel.l[cc][yy], type)) return 1;

   if ((a > 2) && (a <17)) // dual compare with ||
      if (is_solidu( mLevel.l[xx][yy], mLevel.l[cc][yy], type)) return 1;

   return 0;
}























/*

These 4 functions:

float is_up_solidf(float fx, float fy, float fmove, int dir)
float is_down_solidf(float fx, float fy, float fmove, int dir)
float is_left_solidf(float fx, float fy, float fmove, int dir)
float is_right_solidf(float fx, float fy, float fmove, int dir)

are used with trakbot only (for now).
also flapper and player rope and ladder move

They are more accurate than the other ones, they work to the exact pixel.
This was needed when trakbot clings to walls, ceilings and floors,
and need to navigate single block openings.

It is possible to move exactly adjacent to a solid, and not trigger a solid.

These checks have some new parameters:
'float fmove' - is the distance of the requested move
we want to know if we can move that far
the functions returns 'float allowed_move' - how far before the condition is met.

it is assumed that whatever calls the functions will add fmove to position (fx or fy) on return
by comparing requested move to allowed move you can tell if a collision occured:

mv = is_down_solidf(Ef[e][0], Ef[e][1], Ef[e][2], 1);
if (mv < Ef[e][2]) // allowed move less than requested move

'int dir' - the direction to look and the type of thing we are checking for
if dir == 0, we are looking in the direction of the named function for the next solid

for example:
is_down_solidf(fx, fy, fmove, 0)
looks down from fy, fmove amount for a solid
if none is found, the full fmove is returned
if solid is found, fmove is adjusted to make the move line up with solid

if (dir == 1 or -1) we are looking at a right angle from the named function
for the next empty

for example:
is_down_solidfm(fx, fy, fmove, 1)
looks right (+1 in x dir) from fx for the next empty below
if none is found, the full fmove is returned
if empty is found, fmove is adjusted to make the move line up to the next empty

is_down_solidfm(fx, fy, fmove, -1)
looks left (-1 in x dir) from fx for the next empty below
if none is found, the full fmove is returned
if empty is found, fmove is adjusted to make the move line up to the next empty

if you just want to know if the floor below you is solid:
if (is_down_solidfm(fx, fy, 1, 0) < 1)

these functions completely ignore lifts

*/


float is_up_solidf(float fx, float fy, float fmove, int dir)
{
   int ix = fx;
   int iy = fy;
   int im = fmove;
   int mov = 0;
   for (int t=0; t<=im; t++)
   {
      int j = 0, a, b=0, c=0, ret = 0, xx, yy, cc;
      if (dir == 0) yy = (iy-t-1) / 20;
      else
      {
         yy = (iy-1) / 20;
         j = dir * t;
      }
      cc = (ix+j) / 20;
      xx = (ix+j) / 20 + 1;
      a = (ix+j) % 20;

      if (a == 0)    // this block only
      {
         c = mLevel.l[cc][yy];
         ret = is_solid(-1, c, 2);
      }
      else // dual compare with ||
      {
         b = mLevel.l[xx][yy];
         c = mLevel.l[cc][yy];
         ret = is_solid(b, c, 2);
      }
      if (dir == 0) // solid mode - look up for next solid
      {
         if (ret) // as soon as solid is found, return
         {
             int fp = iy - mov;              // next solid pos
             float dist = fy - fp;           // distance from initial pos
             if (dist > fmove) dist = fmove; // limit to requested if over
             return dist;
         }
         else mov++;
      }
      else // empty mode - look left or right for next empty
      {
         if (ret == 0) // as soon as an empty is found, return
         {
            if (dir == 1) // look right
            {
                int fp = ix + mov;              // next empty pos
                float dist = fp - fx;           // distance from initial pos
                if (dist > fmove) dist = fmove; // limit to requested if over
                return dist;
             }
            if (dir == -1) // look left
            {
                int fp = ix - mov;             // next empty pos
                float dist = fx - fp;   // distance from initial pos
                if (dist > fmove) dist = fmove; // limit to requested if over
                return dist;
             }
         }
         else mov++;
      }
   }
   // finished the loop with finding either case;
   return fmove; // max move allowed
}




float is_down_solidf(float fx, float fy, float fmove, int dir)
{
   int ix = fx;
   int iy = fy;
   int im = fmove;
   int mov = 0;
   for (int t=0; t<=im; t++)
   {
      int j = 0, a, b=0, c=0, ret = 0, xx, yy, cc;
      if (dir == 0) yy = (iy + t) / 20 + 1;
      else
      {
         yy = iy / 20 + 1;
         j = dir * t;
      }
      cc = (ix+j) / 20;
      xx = (ix+j) / 20 + 1;
      a = (ix+j) % 20;
      if (a == 0)    // this block only
      {
         c = mLevel.l[cc][yy];
         ret = is_solid(-1, c, 2);
      }
      else // dual compare with ||
      {
         b = mLevel.l[xx][yy];
         c = mLevel.l[cc][yy];
         ret = is_solid(b, c, 2);
      }
      if (dir == 0) // solid mode - looks down for next solid
      {
         if (ret) // as soon as solid is found, return
         {
             int fp = iy + mov;              // next solid pos
             float dist = fp - fy;           // distance from initial pos
             if (dist > fmove) dist = fmove; // limit to requested if over
             return dist;
         }
         else mov++;
      }
      else // empty mode - look left or right for next empty
      {
         if (ret == 0) // as soon as an empty is found, return
         {
            if (dir == 1) // look right
            {
                int fp = ix + mov;             // next empty pos
                float dist = fp - fx;   // distance from initial pos
                if (dist > fmove) dist = fmove; // limit to requested if over
                return dist;
             }
            if (dir == -1) // look left
            {
                int fp = ix - mov;              // next empty pos
                float dist = fx - fp;           // distance from initial pos
                if (dist > fmove) dist = fmove; // limit to requested if over
                return dist;
             }
         }
         else mov++;
      }
   }
   // finished the loop with finding either case;
   return fmove; // max move allowed
}

float is_left_solidf(float fx, float fy, float fmove, int dir)
{
   int ix = fx;
   int iy = fy;
   int im = fmove;
   int mov = 0;
   for (int t=0; t<=im; t++)
   {
      int j = 0, a, b=0, c=0, ret = 0, xx, yy, cc;
      if (dir == 0) xx = (ix-t-1) / 20;
      else
      {
         xx = (ix-1) / 20;
         j = dir * t;
      }
      yy = (iy+j) / 20;
      cc = (iy+j) / 20 + 1;
      a = (iy+j) % 20;
      if (a == 0)    // this block only
      {
         c = mLevel.l[xx][yy];
         ret = is_solid(-1, c, 2);
      }
      else // dual compare with ||
      {
         b = mLevel.l[xx][yy];
         c = mLevel.l[xx][cc];
         ret = is_solid(b, c, 2);
      }

      if (dir == 0) // solid mode - looks left for next solid
      {
         if (ret) // as soon as solid is found, return
         {
             int fp = ix - mov;             // next solid pos
             float dist = fx - fp;   // distance from initial pos
             if (dist > fmove) dist = fmove; // limit to requested if over
             return dist;
         }
         else mov++;
      }
      else // empty mode - look up or down for next empty
      {
         if (ret == 0) // as soon as an empty is found, return
         {
            if (dir == 1) // look down
            {
                int fp = iy + mov;             // next empty pos
                float dist = fp - fy;   // distance from initial pos
                if (dist > fmove) dist = fmove; // limit to requested if over
                return dist;
             }
            if (dir == -1) // look up
            {
                int fp = iy - mov;             // next empty pos
                float dist = fy - fp;   // distance from initial pos
                if (dist > fmove) dist = fmove; // limit to requested if over
                return dist;
             }
         }
         else mov++;
      }
   }
   // finished the loop with finding either case;
   return fmove; // max move allowed
}

float is_right_solidf(float fx, float fy, float fmove, int dir)
{
   int ix = fx;
   int iy = fy;
   int im = fmove;
   int mov = 0;
   for (int t=0; t<=im; t++)
   {
      int j = 0, a, b=0, c=0, ret = 0, xx, yy, cc;
      if (dir == 0) xx = (ix + t) / 20 + 1;
      else
      {
         xx = (ix) / 20 + 1;
         j = dir * t;
      }
      yy = (iy+j) / 20;
      cc = (iy+j) / 20 + 1;

      a = (iy+j) % 20;

      if (a == 0)    // this block only
      {
         c = mLevel.l[xx][yy];
         ret = is_solid(-1, c, 2);
      }
      else // dual compare with ||
      {
         b = mLevel.l[xx][yy];
         c = mLevel.l[xx][cc];
         ret = is_solid(b, c, 2);
      }
      if (dir == 0) // solid mode - looks right for next solid
      {
         if (ret) // as soon as solid is found, return
         {
             int fp = ix + mov;             // next solid pos
             float dist = fp - fx;   // distance from initial pos
             if (dist > fmove) dist = fmove; // limit to requested if over
             return dist;
         }
         else mov++;
      }
      else // empty mode - look up or down for next empty
      {
         if (ret == 0) // as soon as an empty is found, return
         {
            if (dir == 1) // look down
            {
                int fp = iy + mov;             // next empty pos
                float dist = fp - fy;   // distance from initial pos
                if (dist > fmove) dist = fmove; // limit to requested if over
                return dist;
             }
            if (dir == -1) // look up
            {
                int fp = iy - mov;             // next empty pos
                float dist = fy - fp;   // distance from initial pos
                if (dist > fmove) dist = fmove; // limit to requested if over
                return dist;
             }
         }
         else mov++;
      }
   }
   // finished the loop with finding either case;
   return fmove; // max move allowed
}





