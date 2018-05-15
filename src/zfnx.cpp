#include "pm.h"
#include <zlib.h>



int is_block_empty(int x, int y, int test_block, int test_item, int test_enemy)
{
   int empty = 1; // default is empty

   if (test_block) if (l[x][y] > 31) empty = 0;

   if (test_enemy)
      for (int c=0; c<100; c++)
         if (Ei[c][0])
            if ((Efi[c][0] == al_itofix(x*20)) && (Efi[c][1] == al_itofix(y*20))) empty = 0;

   if (test_item)
      for (int c=0; c<500; c++)
         if (item[c][0])
            if ((item[c][4] == (x*20)) && (item[c][5] == (y*20))) empty = 0;

   return empty;
}



void erase_last_bmsg(void)
{
    extern char b_msg[40][80];
    int c;
    if (strcmp(b_msg[0],b_msg[1])==0) /* if last two are the same */
       for (c=0; c<39; c++)
          sprintf(b_msg[c], "%s", b_msg[c+1]);
}
void slide_bmsg(void)
{
    extern char b_msg[40][80];
    int c;
    for (c=39; c>0; c--)
       sprintf(b_msg[c], "%s", b_msg[c-1]);
}

void tsw(void)
{
   al_flush_event_queue(event_queue);
   int quit = 0;
   while(!quit)
   {
      while (!al_is_event_queue_empty(event_queue))
      {
         ALLEGRO_EVENT ev;
         if (al_get_next_event(event_queue, &ev))
         {
             if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) fast_exit(0);
             if (ev.type == ALLEGRO_EVENT_KEY_DOWN) quit = 1;
         }
      }
   }
   while (key[ALLEGRO_KEY_ESCAPE]) proc_controllers();
}

void reset_animation_sequence_passcounts(int pc)
{
   int c;
   for (c=0; c<NUM_ANS; c++)
   {
     zz[2][c] = pc;       // reset the passcounts
     zz[1][c] = 0;        // set the bitmap indexes to 0
     zz[0][c] = zz[5][c]; // put the first shapes in 0
   }
   passcount = pc;
}

void update_animation(void)
{
   // 0 = current shape
   // 1 = current shape index
   // 2 = passcount of last seq change
   // 3 = seq change delay count
   // 4 = num of shapes in seq
   // 5 = shape 0
   // 6 = shape 1
   // 19 = shape 14
   // 15 shapes is tha max

   passcount++;
   for (int y=0; y<NUM_ANS; y++)
      if (zz[4][y] != 0)
         if ((passcount - zz[2][y]) > zz[3][y])
         {
            zz[2][y] = passcount;                     // set counter
            zz[1][y]++;                               // next bitmap
            if (zz[1][y] > zz[4][y]) zz[1][y] = 0;    // is bitmap past end?
            zz[0][y] = zz[ 5 + zz[1][y] ] [y];        // put shape in 0
         }
}

al_fixed get_rot_from_xyinc(int EN)
{
   al_fixed xlen = Efi[EN][2];
   al_fixed ylen = Efi[EN][3];
   al_fixed angle = al_fixatan2(ylen, xlen);
   return angle - al_itofix(64);
}

al_fixed get_rot_from_PXY(int EN, int p)
{
   al_fixed xlen = players[p].PX - Efi[EN][0];
   al_fixed ylen = players[p].PY - Efi[EN][1];
   al_fixed angle = al_fixatan2(ylen, xlen);
   return angle - al_itofix(64);
}

void seek_set_xyinc(int EN)
{
   int p = find_closest_player(EN);
   al_fixed xlen = players[p].PX - Efi[EN][0];   // get the x distance between enemy and player
   al_fixed ylen = players[p].PY - Efi[EN][1];   // get the y distance between enemy and player
   al_fixed hy_dist =  al_fixhypot(xlen, ylen);     // hypotenuse distance
   al_fixed speed = Efi[EN][5];                  // speed
   al_fixed scaler = al_fixdiv(hy_dist, speed);     // get scaler
   al_fixed xinc = al_fixdiv(xlen, scaler);         // calc xinc
   al_fixed yinc = al_fixdiv(ylen, scaler);         // calc yinc
   Efi[EN][2] = xinc;
   Efi[EN][3] = yinc;
}

void seek_set_xyinc(int EN, int x, int y)
{
   al_fixed xlen = al_itofix(x) - Efi[EN][0];       // get the x distance between enemy and x
   al_fixed ylen = al_itofix(y) - Efi[EN][1];       // get the y distance between enemy and y
   al_fixed hy_dist =  al_fixhypot(xlen, ylen);     // hypotenuse distance
   al_fixed speed = Efi[EN][5];                  // speed
   al_fixed scaler = al_fixdiv(hy_dist, speed);     // get scaler
   al_fixed xinc = al_fixdiv(xlen, scaler);         // calc xinc
   al_fixed yinc = al_fixdiv(ylen, scaler);         // calc yinc
   Efi[EN][2] = xinc;
   Efi[EN][3] = yinc;
}





int find_closest_player_flapper(int EN, int dir)
{
   al_fixed width = al_itofix(Ei[EN][17]); // al_fixed version of width
   int height  = Ei[EN][18];
   int depth   = Ei[EN][19];

   // get the enemy's trigger box y values
   int ey = al_fixtoi(Efi[EN][1]);
   int ey1 = ey - height;
   int ey2 = ey + depth;

   al_fixed d[NUM_PLAYERS]; // array of distances for each player
   for (int p=0; p<NUM_PLAYERS; p++)
   {
      d[p] = al_itofix(-1); // default result (player not in range or not active)
      if ((players[p].active) && (!players[p].paused))
      {
         // is player in the y range?
         int py = al_fixtoi(players[p].PY)+10;
         if ((py > ey1) && (py < ey2))
         {
            al_fixed xlen = players[p].PX - Efi[EN][0]; // get x distance
            if ((!dir) && (xlen < al_itofix(0))) d[p] = -xlen; // left
            if ( (dir) && (xlen > al_itofix(0))) d[p] =  xlen; // right
         }
      }
      // check if distance is within range (width); invalidate if not
      if (d[p] > width) d[p] = al_itofix(-1);
   }

   al_fixed closest_val = al_itofix(9999);
   int closest_p = -1;
   for (int p=0; p<NUM_PLAYERS; p++)
   {
      if ((d[p] != al_itofix(-1)) && (d[p] < closest_val))
      {
         closest_val = d[p];
         closest_p = p;
      }
   }
   if (closest_val == al_itofix(9999)) return -1;    // no player in range
   else return closest_p;
}


int find_closest_player_quad(int EN, int quad, int prox)
{
   int closest_p = -1;    // return -1 if no player in range
   al_fixed closest_val = al_itofix(3000);

   al_fixed fz = al_itofix(0);
   al_fixed fm1 = al_itofix(-1);
   al_fixed fprox = al_itofix(prox);

   al_fixed d[NUM_PLAYERS];
   for (int p=0; p<NUM_PLAYERS; p++)
   {
      d[p] = fm1; // default result (players not in quad or not active)
      if ((players[p].active) && (!players[p].paused))
      {
         al_fixed xlen = players[p].PX - Efi[EN][0];
         al_fixed ylen = players[p].PY - Efi[EN][1];

/*           normal cartesian
              I    x+  Y+        II | I
             II    x-  Y+       ----+----
             III   x-  Y-       III | IV
             IV    x+  Y-

             screen
              I    x+  Y-        II | I
             II    x-  Y-       ----+----
             III   x-  Y+       III | IV
             IV    x+  Y+ */

            if ((quad == 1) && (xlen >= fz) && (ylen <= fz)) d[p] = al_fixhypot(xlen,ylen);
            if ((quad == 2) && (xlen <= fz) && (ylen <= fz)) d[p] = al_fixhypot(xlen,ylen);
            if ((quad == 3) && (xlen <= fz) && (ylen >= fz)) d[p] = al_fixhypot(xlen,ylen);
            if ((quad == 4) && (xlen >= fz) && (ylen >= fz)) d[p] = al_fixhypot(xlen,ylen);
         }
         // check if distances are within range
         if (d[p] > fprox) d[p] = fm1;
   }
   for (int p=0; p<NUM_PLAYERS; p++)
      if ((d[p] != fm1) && (d[p] < closest_val))
      {
         closest_val = d[p];
         closest_p = p;
      }
   if (closest_val == al_itofix(3000)) return -1;    // no player in range
   else return closest_p;
}

int find_closest_player(int EN)
{
   int closest_player = 0; // defaults to zero (will always return a valid player)
   al_fixed hd = al_itofix(10000);
   for (int p=0; p<NUM_PLAYERS; p++)
      if ((players[p].active) && (!players[p].paused))
      {
         al_fixed h = al_fixhypot((players[p].PX - Efi[EN][0]), (players[p].PY - Efi[EN][1]));
         if (h < hd)
         {
             hd = h;
             closest_player = p;
         }
      }
   return closest_player;
}

al_fixed fixmod(al_fixed in, int mod)
{
   int t = al_fixtoi(in);
   int u = t / 20;
   int s = u * 20;
   al_fixed r = al_itofix(s);
//   printf("%f %d %d %d %f\n", al_fixtof(in), t, u, s, al_fixtof(s));
   return r;
}

void fire_enemy_bulleta(int EN, int bullet_ans, int p)
{
   extern int e_bullet_active[50], e_bullet_shape[50];
   extern al_fixed e_bullet_fx[50], e_bullet_fy[50], e_bullet_fxinc[50], e_bullet_fyinc[50];
   al_fixed xlen = players[p].PX - Efi[EN][0];   // get the x distance between enemy and player
   al_fixed ylen = players[p].PY - Efi[EN][1];   // get the y distance between enemy and player
   al_fixed hy_dist =  al_fixhypot(xlen, ylen);     // hypotenuse distance
   al_fixed speed = Efi[EN][7];                  // speed
   al_fixed scaler = al_fixdiv(hy_dist, speed);     // get scaler
   al_fixed xinc = al_fixdiv(xlen, scaler);        // calc xinc
   al_fixed yinc = al_fixdiv(ylen, scaler);        // calc yinc

   for (int z=0; z<50; z++)  // find empty e_bullet
      if (!e_bullet_active[z])
      {
         e_bullet_active[z] = 1;
         e_bullet_shape[z] = 1000 + bullet_ans;
         e_bullet_fx[z] = Efi[EN][0];
         e_bullet_fy[z] = Efi[EN][1];
         e_bullet_fxinc[z] = xinc;
         e_bullet_fyinc[z] = yinc;

         int EXint = al_fixtoi(Efi[EN][0]);
         int EYint = al_fixtoi(Efi[EN][1]);

         switch (bullet_ans)
         {
            case 54: event(17, EXint, EYint, 0, 0, 0, 0); break; // green
            case 55: event(18, EXint, EYint, 0, 0, 0, 0); break; // cannonball
            case 20: event(19, EXint, EYint, 0, 0, 0, 0); break; // twirly
         }
         z=50;
      }
}

void fire_enemy_x_bullet(int EN, int p)
{
   extern int e_bullet_active[50], e_bullet_shape[50];
   extern al_fixed e_bullet_fx[50], e_bullet_fy[50], e_bullet_fxinc[50], e_bullet_fyinc[50];
   al_fixed x_bullet_speed = Efi[EN][7];
   for (int z=0; z<50; z++)  // find empty e_bullet
      if (!e_bullet_active[z])
      {
         e_bullet_active[z] = 1;
         e_bullet_fyinc[z] = al_itofix(0);
         e_bullet_fx[z] = Efi[EN][0];
         e_bullet_fy[z] = Efi[EN][1];
         if (Efi[EN][0] < players[p].PX)
         {
            e_bullet_fxinc[z] = x_bullet_speed;
            e_bullet_shape[z] = 488;
         }
         if (Efi[EN][0] >= players[p].PX)
         {
            e_bullet_fxinc[z] = -x_bullet_speed;
            e_bullet_shape[z] = 489;
         }
         event(16, al_fixtoi(Efi[EN][0]), al_fixtoi(Efi[EN][1]), 0, 0, 0, 0);
         z=50; // end loop
      }
}

// returns 1 for solid block, 2 for semi-solid, or 32+lift_num for lift
int is_right_solid(int solid_x, int solid_y, int lift_check)
{
   int a, b, c, d;
   int xx = solid_x / 20 + 1;
   int yy = solid_y / 20;
   int cc = solid_y / 20 + 1;
   a = solid_y % 20;

   if (lift_check)
      for (d = 0; d<num_lifts; d++)

         if (solid_y > lifts[d].y1 - 18)
            if (solid_y < lifts[d].y2 - 2)
               if (solid_x < lifts[d].x1 - 8)
                  if (solid_x > lifts[d].x1 - 18)
                     return 32+d;

   if (a > 16)  // next block only
   {
      c = l[xx][cc];
      if ((c > 63) && (c < NUM_SPRITES)) return 1;
      if ((c > 31) && (c < NUM_SPRITES)) return 2;
   }
   if (a < 3)    // this block only
   {
      c = l[xx][yy];
      if ((c > 63) && (c < NUM_SPRITES)) return 1;
      if ((c > 31) && (c < NUM_SPRITES)) return 2;
   }
   if ((a > 2) && (a < 17)) // dual compare with ||
   {
      b = l[xx][yy];
      c = l[xx][cc];
      if ( ((b > 63) && (b < NUM_SPRITES)) || ((c > 63) && (c < NUM_SPRITES)) ) return 1;
      if ( ((b > 31) && (b < NUM_SPRITES)) || ((c > 31) && (c < NUM_SPRITES)) ) return 2;
   }
   return 0;
}




// returns 1 for solid block, 2 for semi-solid, or 32+lift_num for lift
int is_left_solid(int solid_x, int solid_y, int lift_check)
{
   int a, b, c, d;
   int xx = solid_x / 20;
   int yy = solid_y / 20;
   int cc = solid_y / 20 + 1;
   a = solid_y % 20;
   if (lift_check)
      for (d = 0; d<num_lifts; d++)
         if (solid_y > lifts[d].y1 - 18)
            if (solid_y < lifts[d].y2 - 2)
               if (solid_x < lifts[d].x2 + 2)
                  if (solid_x > lifts[d].x2 - 8)
                     return 32+d;

   if (a > 16)  // next block only
   {
      c = l[xx][cc];
      if ((c > 63) && (c < NUM_SPRITES)) return 1;
      if ((c > 31) && (c < NUM_SPRITES)) return 2;
   }
   if (a < 3)    // this block only
   {
      c = l[xx][yy];
      if ((c > 63) && (c < NUM_SPRITES)) return 1;
      if ((c > 31) && (c < NUM_SPRITES)) return 2;
   }
   if ((a > 2) && (a < 17)) // dual compare with ||
   {
      b = l[xx][yy];
      c = l[xx][cc];
      if ( ((b > 63) && (b < NUM_SPRITES)) || ((c > 63) && (c < NUM_SPRITES)) ) return 1;
      if ( ((b > 31) && (b < NUM_SPRITES)) || ((c > 31) && (c < NUM_SPRITES)) ) return 2;
   }
   return 0;
}


// returns 1 for solid block, 2 for semi-solid, or 32+lift_num for lift
int is_down_solid(int solid_x, int solid_y, int lift_check)
{
   int a, b, c, d;
   int yy = solid_y / 20 + 1;
   int cc = solid_x / 20;
   int xx = solid_x / 20 + 1;
   a = solid_x % 20;
   if (lift_check)
      for (d = 0; d<num_lifts; d++)
            if (solid_x > lifts[d].x1-18)
               if (solid_x < lifts[d].x2-2)
                  if (solid_y > lifts[d].y1 - 25)
                     if (solid_y < lifts[d].y1 - 10)
                         return d+32;

   if (a > 16)  // next block only
   {
      c = l[xx][yy];
      if ((c > 63) && (c < NUM_SPRITES)) return 1;
      if ((c > 31) && (c < NUM_SPRITES)) return 2;
   }
   if (a < 3)    // this block only
   {
      c = l[cc][yy];
      if ((c > 63) && (c < NUM_SPRITES)) return 1;
      if ((c > 31) && (c < NUM_SPRITES)) return 2;
   }
   if ((a > 2) && (a <17)) // dual compare with ||
   {
      b = l[xx][yy];
      c = l[cc][yy];
      if ( ((b > 63) && (b < NUM_SPRITES)) || ((c > 63) && (c < NUM_SPRITES)) ) return 1;
      if ( ((b > 31) && (b < NUM_SPRITES)) || ((c > 31) && (c < NUM_SPRITES)) ) return 2;
   }

   return 0;
}

// returns 1 for solid block, 2 for semi-solid, or 32+lift_num for lift
int is_up_solid(int solid_x, int solid_y, int lift_check)
{
   int yy = (solid_y - 2) / 20;
   int cc = solid_x / 20;
   int xx = solid_x / 20 + 1;
   int a = solid_x % 20;

   if (lift_check)
      for (int d = 0; d<num_lifts; d++)
         if (solid_x > lifts[d].x1 - 18)
            if (solid_x < lifts[d].x2 - 2)
               if (solid_y < lifts[d].y2 + 2)
                  if (solid_y > lifts[d].y2 - 10)
                     return d+32;

   if (a > 16)  // next block only
   {
      int c = l[xx][yy];
      if ((c > 63) && (c < NUM_SPRITES)) return 1;
      if ((c > 31) && (c < NUM_SPRITES)) return 2;
   }
   if (a < 3)    // this block only
   {
      int c = l[cc][yy];
      if ((c > 63) && (c < NUM_SPRITES)) return 1;
      if ((c > 31) && (c < NUM_SPRITES)) return 2;
   }
   if ((a > 2) && (a < 17))// dual compare with ||
   {
      int b = l[cc][yy];
      int c = l[xx][yy];
      if ( ((b > 63) && (b < NUM_SPRITES)) || ((c > 63) && (c < NUM_SPRITES)) ) return 1;
      if ( ((b > 31) && (b < NUM_SPRITES)) || ((c > 31) && (c < NUM_SPRITES)) ) return 2;
   }
   return 0;
}





/*

These 4 functions:

al_fixed is_up_solidfm(al_fixed fx, al_fixed fy, al_fixed fmove, int dir)
al_fixed is_down_solidfm(al_fixed fx, al_fixed fy, al_fixed fmove, int dir)
al_fixed is_left_solidfm(al_fixed fx, al_fixed fy, al_fixed fmove, int dir)
al_fixed is_right_solidfm(al_fixed fx, al_fixed fy, al_fixed fmove, int dir)

are used with trakbot only (for now).

They are more accurate than the other ones, they work to the exact pixel.
This was needed when trakbot clings to walls, ceilings and floors,
and need to navigate single block openings.

It is possible to exactly adjacent to a solid, and not trigger a solid.

These checks have some new parameters:
'al_fixed fmove' - is the distance of the requested move
we want to know if we can move that far
the functions returns 'al_fixed allowed_move' - how far before the condition is met.

it is assumed that what ever calls the functions will add fmove to position (fx or fy) on return
by comparing requested move to allowed move you can tell if a collision occured:

mv = is_down_solidfm(Efi[EN][0], Efi[EN][1], Efi[EN][2], 1);
if (mv < Efi[EN][2]) // allowed move less than requested move

'int dir' - the direction to look and the type of thing we are checking for

if dir == 0, we are looking in the direction of the named function for the next solid

for example:
is_down_solidfm(fx, fy, fmove, 0)
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
is_down_solidfm(fx, fy, al_itofix(1), 0)

also they completely ignore lifts


*/

al_fixed is_up_solidfm(al_fixed fx, al_fixed fy, al_fixed fmove, int dir)
{
   int ix = al_fixtoi(fx);
   int iy = al_fixtoi(fy);
   int im = al_fixtoi(fmove);
   int move = 0;
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
         c = l[cc][yy];
         if      ((c > 63) && (c < NUM_SPRITES)) ret = 1;
         else if ((c > 31) && (c < NUM_SPRITES)) ret = 2;
      }
      else // dual compare with ||
      {
         b = l[xx][yy];
         c = l[cc][yy];
         if ( ((b > 63) && (b < NUM_SPRITES)) || ((c > 63) && (c < NUM_SPRITES)) ) ret = 1;
         else if ( ((b > 31) && (b < NUM_SPRITES)) || ((c > 31) && (c < NUM_SPRITES)) ) ret = 2;
      }
      if (dir == 0) // solid mode - look up for next solid
      {
         if (ret) // as soon as solid is found, return
         {
             int fp = iy - move;             // next solid pos
             al_fixed dist = fy - al_itofix(fp);   // distance from initial pos
             if (dist > fmove) dist = fmove; // limit to requested if over
             return dist;
         }
         else move++;
      }
      else // empty mode - look left or right for next empty
      {
         if (ret == 0) // as soon as an empty is found, return
         {
            if (dir == 1) // look right
            {
                int fp = ix + move;             // next empty pos
                al_fixed dist = al_itofix(fp) - fx;   // distance from initial pos
                if (dist > fmove) dist = fmove; // limit to requested if over
                return dist;
             }
            if (dir == -1) // look left
            {
                int fp = ix - move;             // next empty pos
                al_fixed dist = fx - al_itofix(fp);   // distance from initial pos
                if (dist > fmove) dist = fmove; // limit to requested if over
                return dist;
             }
         }
         else move++;
      }
   }
   // finished the loop with finding either case;
   return fmove; // max move allowed
}

al_fixed is_down_solidfm(al_fixed fx, al_fixed fy, al_fixed fmove, int dir)
{
   int ix = al_fixtoi(fx);
   int iy = al_fixtoi(fy);
   int im = al_fixtoi(fmove);
   int move = 0;
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
         c = l[cc][yy];
         if      ((c > 63) && (c < NUM_SPRITES)) ret = 1;
         else if ((c > 31) && (c < NUM_SPRITES)) ret = 2;
      }
      else // dual compare with ||
      {
         b = l[xx][yy];
         c = l[cc][yy];
         if ( ((b > 63) && (b < NUM_SPRITES)) || ((c > 63) && (c < NUM_SPRITES)) ) ret = 1;
         else if ( ((b > 31) && (b < NUM_SPRITES)) || ((c > 31) && (c < NUM_SPRITES)) ) ret = 2;
      }
      if (dir == 0) // solid mode - looks down for next solid
      {
         if (ret) // as soon as solid is found, return
         {
             int fp = iy + move;             // next solid pos
             al_fixed dist = al_itofix(fp) - fy;   // distance from initial pos
             if (dist > fmove) dist = fmove; // limit to requested if over
             return dist;
         }
         else move++;
      }
      else // empty mode - look left or right for next empty
      {
         if (ret == 0) // as soon as an empty is found, return
         {
            if (dir == 1) // look right
            {
                int fp = ix + move;             // next empty pos
                al_fixed dist = al_itofix(fp) - fx;   // distance from initial pos
                if (dist > fmove) dist = fmove; // limit to requested if over
                return dist;
             }
            if (dir == -1) // look left
            {
                int fp = ix - move;             // next empty pos
                al_fixed dist = fx - al_itofix(fp);   // distance from initial pos
                if (dist > fmove) dist = fmove; // limit to requested if over
                return dist;
             }
         }
         else move++;
      }
   }
   // finished the loop with finding either case;
   return fmove; // max move allowed
}


al_fixed is_left_solidfm(al_fixed fx, al_fixed fy, al_fixed fmove, int dir)
{
   int ix = al_fixtoi(fx);
   int iy = al_fixtoi(fy);
   int im = al_fixtoi(fmove);
   int move = 0;
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
         c = l[xx][yy];
         if      ((c > 63) && (c < NUM_SPRITES)) ret = 1;
         else if ((c > 31) && (c < NUM_SPRITES)) ret = 2;
      }
      else // dual compare with ||
      {
         b = l[xx][yy];
         c = l[xx][cc];
         if ( ((b > 63) && (b < NUM_SPRITES)) || ((c > 63) && (c < NUM_SPRITES)) ) ret = 1;
         else if ( ((b > 31) && (b < NUM_SPRITES)) || ((c > 31) && (c < NUM_SPRITES)) ) ret = 2;
      }

      if (dir == 0) // solid mode - looks left for next solid
      {
         if (ret) // as soon as solid is found, return
         {
             int fp = ix - move;             // next solid pos
             al_fixed dist = fx - al_itofix(fp);   // distance from initial pos
             if (dist > fmove) dist = fmove; // limit to requested if over
             return dist;
         }
         else move++;
      }
      else // empty mode - look up or down for next empty
      {
         if (ret == 0) // as soon as an empty is found, return
         {
            if (dir == 1) // look down
            {
                int fp = iy + move;             // next empty pos
                al_fixed dist = al_itofix(fp) - fy;   // distance from initial pos
                if (dist > fmove) dist = fmove; // limit to requested if over
                return dist;
             }
            if (dir == -1) // look up
            {
                int fp = iy - move;             // next empty pos
                al_fixed dist = fy - al_itofix(fp);   // distance from initial pos
                if (dist > fmove) dist = fmove; // limit to requested if over
                return dist;
             }
         }
         else move++;
      }
   }
   // finished the loop with finding either case;
   return fmove; // max move allowed
}


al_fixed is_right_solidfm(al_fixed fx, al_fixed fy, al_fixed fmove, int dir)
{
   int ix = al_fixtoi(fx);
   int iy = al_fixtoi(fy);
   int im = al_fixtoi(fmove);
   int move = 0;
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
         c = l[xx][yy];
         if      ((c > 63) && (c < NUM_SPRITES)) ret = 1;
         else if ((c > 31) && (c < NUM_SPRITES)) ret = 2;
      }
      else // dual compare with ||
      {
         b = l[xx][yy];
         c = l[xx][cc];
         if ( ((b > 63) && (b < NUM_SPRITES)) || ((c > 63) && (c < NUM_SPRITES)) ) ret = 1;
         else if ( ((b > 31) && (b < NUM_SPRITES)) || ((c > 31) && (c < NUM_SPRITES)) ) ret = 2;
      }
      if (dir == 0) // solid mode - looks right for next solid
      {
         if (ret) // as soon as solid is found, return
         {
             int fp = ix + move;             // next solid pos
             al_fixed dist = al_itofix(fp) - fx;   // distance from initial pos
             if (dist > fmove) dist = fmove; // limit to requested if over
             return dist;
         }
         else move++;
      }
      else // empty mode - look up or down for next empty
      {
         if (ret == 0) // as soon as an empty is found, return
         {
            if (dir == 1) // look down
            {
                int fp = iy + move;             // next empty pos
                al_fixed dist = al_itofix(fp) - fy;   // distance from initial pos
                if (dist > fmove) dist = fmove; // limit to requested if over
                return dist;
             }
            if (dir == -1) // look up
            {
                int fp = iy - move;             // next empty pos
                al_fixed dist = fy - al_itofix(fp);   // distance from initial pos
                if (dist > fmove) dist = fmove; // limit to requested if over
                return dist;
             }
         }
         else move++;
      }
   }
   // finished the loop with finding either case;
   return fmove; // max move allowed
}



void state_to_chunk(char * b)
{
   int offset = 0;
   int size;

   size = sizeof(players);
   memmove(b+offset, players, size);
//      printf("players \tsize %d \toffset %d \n", size, offset);
   offset += size;
   size = sizeof(Ei);
   memmove(b+offset, Ei, size);
//      printf("int Ei[100][32] \tsize %d \toffset %d \n", size, offset);
   offset += size;
   size = sizeof(Efi);
   memmove(b+offset, Efi, size);
//    printf("al_fixed Efi[100][16] \tsize %d \toffset %d \n", size, offset);
   offset += size;
   size = sizeof(item);
   memmove(b+offset, item, size);
//    printf("int item[500][16] \tsize %d \toffset %d \n", size, offset);
   offset += size;
   size = sizeof(itemf);
   memmove(b+offset, itemf, size);
//    printf("al_fixed item[500][4] \tsize %d \toffset %d \n", size, offset);
   offset += size;
   size = sizeof(lifts);
   memmove(b+offset, lifts, size);
//      printf("lifts \t\t\tsize %d \toffset %d \n", size, offset);
   offset += size;
   size = sizeof(l);
   memmove(b+offset, l, size);
//    printf("l[100][100] \t\tsize %d \toffset %d \n", size, offset);
   offset += size;
//      printf("----------------total chunk size %d \n", offset);

}


void chnk_to_state(char * b)
{
   int offset = 0;
   int size;

   size = sizeof(players);
   memcpy(players, b+offset, size);
   offset += size;

   size = sizeof(Ei);
   memcpy(Ei, b+offset, size);
   offset += size;

   size = sizeof(Efi);
   memcpy(Efi, b+offset, size);
   offset += size;

   size = sizeof(item);
   memcpy(item, b+offset, size);
   offset += size;

   size = sizeof(itemf);
   memcpy(itemf, b+offset, size);
   offset += size;

   size = sizeof(lifts);
   memcpy(lifts, b+offset, size);
   offset += size;

   size = sizeof(l);
   memcpy(l, b+offset, size);
}


// test a=20 b=10

// get  c = a - b (c=10)

// apply a-= c (a=10)


void get_chunk_dif(char *a, char *b, char *c, int size)
{
   for (int i=0; i<size; i++)
      c[i] = a[i] - b[i];
}

void apply_chunk_dif(char *a, char *c, int size)
{
   for (int i=0; i<size; i++)
      a[i] -= c[i];
}


void reset_states(void)
{
   // reset base state on client
   memset(clientl_chdf, 0, CHUNK_SIZE);
   clientl_chdf_id = 0;  // passcount id


   // reset dif buffer and passcount
   memset(dif, 0, CHUNK_SIZE);
   dif_id[0] = -1; // -1 will never match a passcount
   dif_id[1] = -1;

   // reset buffer and passcounts used to build compressed chdf from packets
   memset(chdf, 0, CHUNK_SIZE);
   for (int i=0; i<16; i++)
      chdf_pieces[i] = -1;

   // reset server chdf's
   for (int i=0; i<8; i++)
   {
      memset(client_chdf[i][0], 0, CHUNK_SIZE);
      memset(client_chdf[i][1], 0, CHUNK_SIZE);
      client_chdf_id[i][0] = 0; // src passcount id
      client_chdf_id[i][1] = -3; // dst passcount id
   }
}



void show_chunk_dif(char *a, char *b)
{
   // copy chunks to temp vars

   struct player c1_players[NUM_PLAYERS];
   struct player c2_players[NUM_PLAYERS];

   int c1_Ei[100][32];    // enemy ints
   int c2_Ei[100][32];    // enemy ints

   al_fixed c1_Efi[100][16]; // enemy fixeds
   al_fixed c2_Efi[100][16]; // enemy fixeds


   int c1_item[500][16];  // item ints
   int c2_item[500][16];  // item ints

   al_fixed c1_itemf[500][4]; // item al_fixed points
   al_fixed c2_itemf[500][4]; // item al_fixed points

   struct lift c1_lifts[NUM_LIFTS];
   struct lift c2_lifts[NUM_LIFTS];

   int c1_l[100][100];    // level
   int c2_l[100][100];    // level


   int offset = 0;
   int size;

   size = sizeof(players);
   memcpy(c1_players, a+offset, size);
   memcpy(c2_players, b+offset, size);
   offset += size;

   size = sizeof(Ei);
   memcpy(c1_Ei, a+offset, size);
   memcpy(c2_Ei, b+offset, size);
   offset += size;

   size = sizeof(Efi);
   memcpy(c1_Efi, a+offset, size);
   memcpy(c2_Efi, b+offset, size);
   offset += size;

   size = sizeof(item);
   memcpy(c1_item, a+offset, size);
   memcpy(c2_item, b+offset, size);
   offset += size;

   size = sizeof(itemf);
   memcpy(c1_itemf, a+offset, size);
   memcpy(c2_itemf, b+offset, size);
   offset += size;

   size = sizeof(lifts);
   memcpy(c1_lifts, a+offset, size);
   memcpy(c2_lifts, b+offset, size);
   offset += size;

   size = sizeof(l);
   memcpy(c1_l, a+offset, size);
   memcpy(c2_l, b+offset, size);


   int sop = sizeof(players);

   if (memcmp(c1_players, c2_players, sop))
   {
      if (L_LOGGING_NETPLAY_show_dif1)
      {
         #ifdef LOGGING_NETPLAY_show_dif1
         sprintf(msg, "player errors detected\n");
         add_log_entry2(31, active_local_player, msg); printf("%s", msg);
         #endif
      }
      if (L_LOGGING_NETPLAY_show_dif2)
      {
         #ifdef LOGGING_NETPLAY_show_dif2

         // show where
         for (int i=0; i<sop; i++)
         {
            if (a[i] != b[i])
            {
               int ps = sop /8;  // size of one player
               int p = i/ps ;    // get player
               int o = i % ps;   // get offset
   //            printf("player[%d] error detected at offset%d byte_pos:%d\n", p, o, o/4);
   //            sprintf(msg, "[%d][%d] \n", a[i], b[i]);

               sprintf(msg, "player[%d] byte:[%d] var:[%d] server:[%d] local:[%d] \n", p, o, o/4, a[i], b[i] );
               add_log_entry2(32, active_local_player, msg);
            }
         }


         for (int p=0; p<8; p++)
         {
            if (c1_players[p].active != c2_players[p].active)
            {
               sprintf(msg, "player[%d].active  server:[%d] local:[%d]\n", p, c1_players[p].active, c2_players[p].active );
               add_log_entry2(32, active_local_player, msg);
            }
            if (c1_players[p].paused != c2_players[p].paused)
            {
               sprintf(msg, "player[%d].paused  server:[%d] local:[%d]\n", p, c1_players[p].paused, c2_players[p].paused );
               add_log_entry2(32, active_local_player, msg);
            }

            if (c1_players[p].paused_type  != c2_players[p].paused_type)
            {
               sprintf(msg, "player[%d].paused_type   server:[%d] local:[%d]\n", p, c1_players[p].paused_type,  c2_players[p].paused_type);
               add_log_entry2(32, active_local_player, msg);
            }

            if (c1_players[p].paused_mode  != c2_players[p].paused_mode)
            {
               sprintf(msg, "player[%d].paused_mode   server:[%d] local:[%d]\n", p, c1_players[p].paused_mode,  c2_players[p].paused_mode);
               add_log_entry2(32, active_local_player, msg);
            }

            if (c1_players[p].paused_mode_count  != c2_players[p].paused_mode_count)
            {
               sprintf(msg, "player[%d].paused_mode_count   server:[%d] local:[%d]\n", p, c1_players[p].paused_mode_count,  c2_players[p].paused_mode_count);
               add_log_entry2(32, active_local_player, msg);
            }

           if (c1_players[p].PX != c2_players[p].PX)
            {
               sprintf(msg, "player[%d].PX   server:[%f] local:[%f]\n", p, al_fixtof(c1_players[p].PX), al_fixtof(c2_players[p].PX));
               add_log_entry2(32, active_local_player, msg);
            }

            if (c1_players[p].PY  != c2_players[p].PY)
            {
               sprintf(msg, "player[%d].PY   server:[%f] local:[%f]\n", p, al_fixtof(c1_players[p].PY), al_fixtof(c2_players[p].PY)  );
               add_log_entry2(32, active_local_player, msg);
            }

            if (c1_players[p].xinc != c2_players[p].xinc)
            {
               sprintf(msg, "player[%d].xinc server:[%f] local:[%f]\n", p, al_fixtof(c1_players[p].xinc), al_fixtof(c2_players[p].xinc)  );
               add_log_entry2(32, active_local_player, msg);
            }

            if (c1_players[p].yinc != c2_players[p].yinc)
            {
               sprintf(msg, "player[%d].yinc server:[%f] local:[%f]\n", p, al_fixtof(c1_players[p].yinc), al_fixtof(c2_players[p].yinc)  );
               add_log_entry2(32, active_local_player, msg);
            }

            if (c1_players[p].right_xinc != c2_players[p].right_xinc)
            {
               sprintf(msg, "player[%d].right_xinc server:[%f] local:[%f]\n", p, al_fixtof(c1_players[p].right_xinc), al_fixtof(c2_players[p].right_xinc)  );
               add_log_entry2(32, active_local_player, msg);
            }

            if (c1_players[p].left_xinc != c2_players[p].left_xinc)
            {
               sprintf(msg, "player[%d].left_xinc server:[%f] local:[%f]\n", p, al_fixtof(c1_players[p].left_xinc), al_fixtof(c2_players[p].left_xinc)  );
               add_log_entry2(32, active_local_player, msg);
            }

            if (c1_players[p].LIFE != c2_players[p].LIFE)
            {
               sprintf(msg, "player[%d].LIFE   server:[%f] local:[%f]\n", p, al_fixtof(c1_players[p].LIFE), al_fixtof(c2_players[p].LIFE));
               add_log_entry2(32, active_local_player, msg);
            }
            if (c1_players[p].old_LIFE != c2_players[p].old_LIFE)
            {
               sprintf(msg, "player[%d].old_LIFE   server:[%f] local:[%f]\n", p, al_fixtof(c1_players[p].old_LIFE), al_fixtof(c2_players[p].old_LIFE));
               add_log_entry2(32, active_local_player, msg);
            }
            if (c1_players[p].LIVES  != c2_players[p].LIVES)
            {
               sprintf(msg, "player[%d].LIVES   server:[%d] local:[%d]\n", p, c1_players[p].LIVES,  c2_players[p].LIVES);
               add_log_entry2(32, active_local_player, msg);
            }


            if (c1_players[p].bitmap_index  != c2_players[p].bitmap_index)
            {
               sprintf(msg, "player[%d].bitmap_index   server:[%d] local:[%d]\n", p, c1_players[p].bitmap_index,  c2_players[p].bitmap_index);
               add_log_entry2(32, active_local_player, msg);
            }


            if (c1_players[p].shape  != c2_players[p].shape)
            {
               sprintf(msg, "player[%d].shape   server:[%d] local:[%d]\n", p, c1_players[p].shape,  c2_players[p].shape);
               add_log_entry2(32, active_local_player, msg);
            }



            if (c1_players[p].color  != c2_players[p].color)
            {
               sprintf(msg, "player[%d].color   server:[%d] local:[%d]\n", p, c1_players[p].color,  c2_players[p].color );
               add_log_entry2(32, active_local_player, msg);
            }




           if (c1_players[p].door_draw_rot_num_steps  != c2_players[p].door_draw_rot_num_steps)
            {
               sprintf(msg, "player[%d].door_draw_rot_num_steps   server:[%d] local:[%d]\n", p, c1_players[p].door_draw_rot_num_steps,  c2_players[p].door_draw_rot_num_steps);
               add_log_entry2(32, active_local_player, msg);
            }

            if (c1_players[p].door_draw_rot != c2_players[p].door_draw_rot)
            {
               sprintf(msg, "player[%d].door_draw_rot   server:[%f] local:[%f]\n", p, al_fixtof(c1_players[p].door_draw_rot), al_fixtof(c2_players[p].door_draw_rot));
               add_log_entry2(32, active_local_player, msg);
            }
            if (c1_players[p].door_draw_rot_inc != c2_players[p].door_draw_rot_inc)
            {
               sprintf(msg, "player[%d].door_draw_rot_inc   server:[%f] local:[%f]\n", p, al_fixtof(c1_players[p].door_draw_rot_inc), al_fixtof(c2_players[p].door_draw_rot_inc));
               add_log_entry2(32, active_local_player, msg);
            }
            if (c1_players[p].draw_rot != c2_players[p].draw_rot)
            {
               sprintf(msg, "player[%d].draw_rot   server:[%f] local:[%f]\n", p, al_fixtof(c1_players[p].draw_rot), al_fixtof(c2_players[p].draw_rot));
               add_log_entry2(32, active_local_player, msg);
            }
            if (c1_players[p].draw_scale != c2_players[p].draw_scale)
            {
               sprintf(msg, "player[%d].draw_scale   server:[%f] local:[%f]\n", p, al_fixtof(c1_players[p].draw_scale), al_fixtof(c2_players[p].draw_scale));
               add_log_entry2(32, active_local_player, msg);
            }



            if (c1_players[p].marked_door  != c2_players[p].marked_door)
            {
               sprintf(msg, "player[%d].marked_door   server:[%d] local:[%d]\n", p, c1_players[p].marked_door,  c2_players[p].marked_door);
               add_log_entry2(32, active_local_player, msg);
            }
            if (c1_players[p].door_item  != c2_players[p].door_item)
            {
               sprintf(msg, "player[%d].door_item   server:[%d] local:[%d]\n", p, c1_players[p].door_item,  c2_players[p].door_item);
               add_log_entry2(32, active_local_player, msg);
            }
            if (c1_players[p].door_xinc  != c2_players[p].door_xinc)
            {
               sprintf(msg, "player[%d].door_xinc   server:[%d] local:[%d]\n", p, c1_players[p].door_xinc,  c2_players[p].door_xinc);
               add_log_entry2(32, active_local_player, msg);
            }
            if (c1_players[p].door_yinc  != c2_players[p].door_yinc)
            {
               sprintf(msg, "player[%d].door_yinc   server:[%d] local:[%d]\n", p, c1_players[p].door_yinc,  c2_players[p].door_yinc);
               add_log_entry2(32, active_local_player, msg);
            }
            if (c1_players[p].door_num_steps  != c2_players[p].door_num_steps)
            {
               sprintf(msg, "player[%d].door_num_steps   server:[%d] local:[%d]\n", p, c1_players[p].door_num_steps,  c2_players[p].door_num_steps);
               add_log_entry2(32, active_local_player, msg);
            }

            if (c1_players[p].left_right  != c2_players[p].left_right)
            {
               sprintf(msg, "player[%d].left_right   server:[%d] local:[%d]\n", p, c1_players[p].left_right,  c2_players[p].left_right);
               add_log_entry2(32, active_local_player, msg);
            }

            if (c1_players[p].player_ride  != c2_players[p].player_ride)
            {
               sprintf(msg, "player[%d].player_ride   server:[%d] local:[%d]\n", p, c1_players[p].player_ride,  c2_players[p].player_ride);
               add_log_entry2(32, active_local_player, msg);
            }

            if (c1_players[p].carry_item  != c2_players[p].carry_item)
            {
               sprintf(msg, "player[%d].carry_item   server:[%d] local:[%d]\n", p, c1_players[p].carry_item,  c2_players[p].carry_item);
               add_log_entry2(32, active_local_player, msg);
            }

            if (c1_players[p].up  != c2_players[p].up)
            {
               sprintf(msg, "player[%d].up   server:[%d] local:[%d]\n", p, c1_players[p].up,  c2_players[p].up);
               add_log_entry2(32, active_local_player, msg);
            }

            if (c1_players[p].down  != c2_players[p].down)
            {
               sprintf(msg, "player[%d].down   server:[%d] local:[%d]\n", p, c1_players[p].down,  c2_players[p].down);
               add_log_entry2(32, active_local_player, msg);
            }

            if (c1_players[p].left  != c2_players[p].left)
            {
               sprintf(msg, "player[%d].left   server:[%d] local:[%d]\n", p, c1_players[p].left,  c2_players[p].left);
               add_log_entry2(32, active_local_player, msg);
            }

            if (c1_players[p].right  != c2_players[p].right)
            {
               sprintf(msg, "player[%d].right   server:[%d] local:[%d]\n", p, c1_players[p].right,  c2_players[p].right);
               add_log_entry2(32, active_local_player, msg);
            }

            if (c1_players[p].jump  != c2_players[p].jump)
            {
               sprintf(msg, "player[%d].jump   server:[%d] local:[%d]\n", p, c1_players[p].jump,  c2_players[p].jump);
               add_log_entry2(32, active_local_player, msg);
            }

            if (c1_players[p].fire  != c2_players[p].fire)
            {
               sprintf(msg, "player[%d].fire   server:[%d] local:[%d]\n", p, c1_players[p].fire,  c2_players[p].fire);
               add_log_entry2(32, active_local_player, msg);
            }

            if (c1_players[p].fire_held  != c2_players[p].fire_held)
            {
               sprintf(msg, "player[%d].fire_held   server:[%d] local:[%d]\n", p, c1_players[p].fire_held,  c2_players[p].fire_held);
               add_log_entry2(32, active_local_player, msg);
            }

            if (c1_players[p].menu  != c2_players[p].menu)
            {
               sprintf(msg, "player[%d].menu   server:[%d] local:[%d]\n", p, c1_players[p].menu,  c2_players[p].menu);
               add_log_entry2(32, active_local_player, msg);
            }


            if (c1_players[p].bullet_wait_counter  != c2_players[p].bullet_wait_counter)
            {
               sprintf(msg, "player[%d].bullet_wait_counter   server:[%d] local:[%d]\n", p, c1_players[p].bullet_wait_counter,  c2_players[p].bullet_wait_counter);
               add_log_entry2(32, active_local_player, msg);
            }

            if (c1_players[p].request_bullet  != c2_players[p].request_bullet)
            {
               sprintf(msg, "player[%d].request_bullet   server:[%d] local:[%d]\n", p, c1_players[p].request_bullet,  c2_players[p].request_bullet);
               add_log_entry2(32, active_local_player, msg);
            }

            if (c1_players[p].bullet_wait  != c2_players[p].bullet_wait)
            {
               sprintf(msg, "player[%d].bullet_wait   server:[%d] local:[%d]\n", p, c1_players[p].bullet_wait,  c2_players[p].bullet_wait);
               add_log_entry2(32, active_local_player, msg);
            }

            if (c1_players[p].bullet_speed  != c2_players[p].bullet_speed)
            {
               sprintf(msg, "player[%d].bullet_speed   server:[%d] local:[%d]\n", p, c1_players[p].bullet_speed,  c2_players[p].bullet_speed );
               add_log_entry2(32, active_local_player, msg);
            }

            if (c1_players[p].num_bullets  != c2_players[p].num_bullets)
            {
               sprintf(msg, "player[%d].num_bullets   server:[%d] local:[%d]\n", p, c1_players[p].num_bullets,  c2_players[p].num_bullets );
               add_log_entry2(32, active_local_player, msg);
            }

            if (c1_players[p].num_hits  != c2_players[p].num_hits)
            {
               sprintf(msg, "player[%d].num_hits   server:[%d] local:[%d]\n", p, c1_players[p].num_hits,  c2_players[p].num_hits );
               add_log_entry2(32, active_local_player, msg);
            }

            if (c1_players[p].control_method  != c2_players[p].control_method)
            {
               sprintf(msg, "player[%d].control_method   server:[%d] local:[%d]\n", p, c1_players[p].control_method,  c2_players[p].control_method );
               add_log_entry2(32, active_local_player, msg);
            }



   /*
            if (c1_players[p].health_display  != c2_players[p].health_display)
            {
               sprintf(msg, "player[%d].health_display   server:[%d] local:[%d]\n", p, c1_players[p].health_display,  c2_players[p].health_display );
               add_log_entry2(32, active_local_player, msg);

            }
   */

         }
         #endif
      }
   }
   if (memcmp(c1_Ei, c2_Ei, sizeof(Ei)))
   {
      if (L_LOGGING_NETPLAY_show_dif1)
      {
         #ifdef LOGGING_NETPLAY_show_dif1
         sprintf(msg, "Ei errors detected\n");
         add_log_entry2(31, active_local_player, msg); printf("%s", msg);
         #endif
      }
      if (L_LOGGING_NETPLAY_show_dif2)
      {
         #ifdef LOGGING_NETPLAY_show_dif2
         for (int e=0; e<100; e++)
            for (int f=0; f<32; f++)
               if (c1_Ei[e][f] != c2_Ei[e][f])
               {
                  sprintf(msg, "Ei[%2d][%2d][t:%2d] server:%d local:%d\n", e, f, c1_Ei[e][0], c1_Ei[e][f], c2_Ei[e][f]);
                  add_log_entry2(32, active_local_player, msg);
               }
         #endif
      }
   }
   if (memcmp(c1_Efi, c2_Efi, sizeof(Efi)))
   {
      if (L_LOGGING_NETPLAY_show_dif1)
      {
         #ifdef LOGGING_NETPLAY_show_dif1
         sprintf(msg, "Efi errors detected\n");
         add_log_entry2(31, active_local_player, msg); printf("%s", msg);
         #endif
      }
      if (L_LOGGING_NETPLAY_show_dif2)
      {
         #ifdef LOGGING_NETPLAY_show_dif2
         for (int e=0; e<100; e++)
            for (int f=0; f<16; f++)
               if (c1_Efi[e][f] != c2_Efi[e][f])
               {
                  sprintf(msg, "Efi[%2d][%2d][t:%2d] server:%f local:%f\n", e, f, c1_Ei[e][0], al_fixtof(c1_Efi[e][f]), al_fixtof(c2_Efi[e][f]));
                  add_log_entry2(32, active_local_player, msg);
               }
         #endif
      }
   }
   if (memcmp(c1_item, c2_item, sizeof(item)))
   {
      if (L_LOGGING_NETPLAY_show_dif1)
      {
         #ifdef LOGGING_NETPLAY_show_dif1
         sprintf(msg, "item errors detected\n");
         add_log_entry2(31, active_local_player, msg); printf("%s", msg);
         #endif
      }
      if (L_LOGGING_NETPLAY_show_dif2)
      {
         #ifdef LOGGING_NETPLAY_show_dif2
         for (int e=0; e<500; e++)
            for (int f=0; f<16; f++)
               if (c1_item[e][f] != c2_item[e][f])
               {
                  sprintf(msg, "item[%d][%d][t:%2d] server:%d local:%d\n", e, f, c1_item[e][0], c1_item[e][f], c2_item[e][f]);
                  add_log_entry2(32, active_local_player, msg);
               }
         #endif
      }
   }
   if (memcmp(c1_itemf, c2_itemf, sizeof(itemf)))
   {
      if (L_LOGGING_NETPLAY_show_dif1)
      {
         #ifdef LOGGING_NETPLAY_show_dif1
         sprintf(msg, "itemf errors detected\n");
         add_log_entry2(31, active_local_player, msg); printf("%s", msg);
         #endif
      }
      if (L_LOGGING_NETPLAY_show_dif2)
      {
         #ifdef LOGGING_NETPLAY_show_dif2
         for (int e=0; e<500; e++)
            for (int f=0; f<4; f++)
               if (c1_itemf[e][f] != c2_itemf[e][f])
               {
                  sprintf(msg, "itemf[%d][%d][t:%2d] server:%f local:%f\n", e, f, c1_item[e][0], al_fixtof(c1_itemf[e][f]), al_fixtof(c2_itemf[e][f]));
                  add_log_entry2(32, active_local_player, msg);
               }
         #endif
      }
   }
   if (memcmp(c1_lifts, c2_lifts, sizeof(lifts)))
   {
      if (L_LOGGING_NETPLAY_show_dif1)
      {
         #ifdef LOGGING_NETPLAY_show_dif1
         sprintf(msg, "lift errors detected\n");
         add_log_entry2(31, active_local_player, msg); printf("%s", msg);
         #endif
      }
      if (L_LOGGING_NETPLAY_show_dif2)
      {
         #ifdef LOGGING_NETPLAY_show_dif2
         for (int e=0; e<40; e++)
         {
            if (c1_lifts[e].fx    != c2_lifts[e].fx )
            {
               sprintf(msg, "lifts[%d].fx server:%f local:%f\n",    e, al_fixtof(c1_lifts[e].fx),    al_fixtof(c2_lifts[e].fx));
               add_log_entry2(32, active_local_player, msg);
            }
            if (c1_lifts[e].fy    != c2_lifts[e].fy )
            {
               sprintf(msg, "lifts[%d].fy server:%f local:%f\n",    e, al_fixtof(c1_lifts[e].fy),    al_fixtof(c2_lifts[e].fy));
               add_log_entry2(32, active_local_player, msg);
            }
            if (c1_lifts[e].fxinc != c2_lifts[e].fxinc )
            {
               sprintf(msg, "lifts[%d].fxinc server:%f local:%f\n", e, al_fixtof(c1_lifts[e].fxinc), al_fixtof(c2_lifts[e].fxinc));
               add_log_entry2(32, active_local_player, msg);
            }
            if (c1_lifts[e].fyinc != c2_lifts[e].fyinc )
            {
               sprintf(msg, "lifts[%d].fyinc server:%f local:%f\n", e, al_fixtof(c1_lifts[e].fyinc), al_fixtof(c2_lifts[e].fyinc));
               add_log_entry2(32, active_local_player, msg);
            }
         }
         #endif
      }
   }
   if (memcmp(c1_l, c2_l, sizeof(l)))
   {
      if (L_LOGGING_NETPLAY_show_dif1)
      {
         #ifdef LOGGING_NETPLAY_show_dif1
         sprintf(msg, "block errors detected\n");
         add_log_entry2(31, active_local_player, msg); printf("%s", msg);
         #endif
      }
      if (L_LOGGING_NETPLAY_show_dif2)
      {
         #ifdef LOGGING_NETPLAY_show_dif2
         for (int e=0; e<100; e++)
            for (int f=0; f<100; f++)
               if (c1_l[e][f] != c2_l[e][f])
               {
                  sprintf(msg, "block[%d][%d] server:%d local:%d\n", e, f, c1_l[e][f], c2_l[e][f] );
                  add_log_entry2(32, active_local_player, msg);
               }
         #endif
      }
   }
}








/*
int st_sr(int sx, int sy, int cd, int gfx_card)
{
   set_color_depth(cd);
   if (set_gfx_mode(gfx_card, sx, sy, 0, 0) == 0)
   {
      if (gfx_card == 1) sprintf(msg, "[%4d x %4d] %2d bit fullscreen..", sx, sy, cd);
      if (gfx_card == 2) sprintf(msg, "[%4d x %4d] %2d bit windowed....", sx, sy, cd);

      strcat(log_msg, msg);
      printf("%s", msg);
      return 1;
   }
   else  // failed
   {
      sprintf(msg, "Error setting: [%d x %d] gfx_card:%d color_depth:%d\n", sx, sy, gfx_card, cd);
      strcat(log_msg, msg);
      printf("%s", msg);
      return 0;
   }
}


void st_run(void)
{
   load_gm("test1.gm");
   players[0].control_method = 1;
   start_mode = 2; // load level and start, but skip game array erasing
   game_exit = 0;

   int start_timer = clock();
   pm_main();
   int finish_time = clock();
   int time = finish_time - start_timer;

   int cps = CLOCKS_PER_SEC;

   sprintf(msg, "[%3d] Frames per second -- [%dms] per frame\n",(passcount*cps) / time, time/passcount);
   printf("%s", msg);
   strcat(log_msg, msg);

}

void speed_test(void)
{
   sprintf(msg, "\n---------------------------------\n- Speed test for %s\n", local_hostname);
   printf("%s", msg);
   strcpy(log_msg, msg);

   get_desktop_resolution(&desktop_sx, &desktop_sy);
   desktop_colordepth = desktop_color_depth();

   sprintf(msg, "Desktop resolution: [%d x %d] %d bit\n", desktop_sx, desktop_sy, desktop_colordepth);
   printf("%s", msg);
   strcat(log_msg, msg);

   // get operating system type
   show_os_detected();
   strcat(log_msg, msg);
   strcat(log_msg, "\n");

   int cps = CLOCKS_PER_SEC;
   sprintf(msg, "CLOCKS_PER_SEC [%d]\n", cps);
   printf("%s", msg);
   strcat(log_msg, msg);

   // get allegro id
   sprintf(msg, "Allegro version:    [%s]\n\n", allegro_id);
   printf("%s", msg);
   strcat(log_msg, msg);

   speed_testing = 1;








//   if (st_sr(desktop_sx, desktop_sy, 32, 1)) st_run();
//   if (st_sr(desktop_sx, desktop_sy, 16, 1)) st_run();
//   if (st_sr(desktop_sx, desktop_sy, 8, 1)) st_run();
//
//   if (st_sr(1024, 768, 32, 2)) st_run();
//   if (st_sr(1024, 768, 16, 2)) st_run();
//   if (st_sr(1024, 768, 8, 2)) st_run();
//
//
//
//   if (st_sr(1024, 768, 32, 1)) st_run();
//   if (st_sr(1024, 768, 16, 1)) st_run();
//   if (st_sr(1024, 768, 8, 1)) st_run();



//   if (st_sr(800, 600, 32, 2)) st_run();



//   if (st_sr(800, 600, 16, 2)) st_run();
//   if (st_sr(800, 600, 8, 2)) st_run();
//
//   if (st_sr(800, 600, 32, 1)) st_run();
//   if (st_sr(800, 600, 16, 1)) st_run();
//   if (st_sr(800, 600, 8, 1)) st_run();


   FILE *filepntr;
   char filename[140];
   sprintf(filename, "logs/speedtest[%s].txt", local_hostname);
   filepntr = fopen(filename,"w");
   fprintf(filepntr, "%s", log_msg);
   fclose(filepntr);
}

*/




int fill_demo_array(ALLEGRO_FS_ENTRY *fs, void * extra)
{
   if (num_demo_filenames > 99) return 0; // only get 100 max
   demo_FS_filenames[num_demo_filenames] = al_create_fs_entry(al_get_fs_entry_name(fs));
   demo_played[num_demo_filenames] = 0;
   num_demo_filenames++;
   return ALLEGRO_FOR_EACH_FS_ENTRY_OK;
}

void demo_mode(void)
{
   demo_mode_on = 1;
   num_demo_filenames = 0;

   char fname[1024];
   sprintf(fname, "savegame/demo");

   //printf("fname:%s\n", fname);

   // convert to 'ALLEGRO_FS_ENTRY' (also makes fully qualified path)
   ALLEGRO_FS_ENTRY *FS_fname = al_create_fs_entry(fname);
   sprintf(fname, "%s", al_get_fs_entry_name(FS_fname));

   //printf("FS_fname:%s\n", fname);

   // iterate levels in demo folder and put in filename array
   al_for_each_fs_entry(FS_fname, fill_demo_array, NULL);

//   for (int i=0; i< num_demo_filenames; i++)
//     printf("%s\n", al_get_fs_entry_name(demo_FS_filenames[i]));

   if (num_demo_filenames == 0)
   {
      printf("No demo files found.\n");
      demo_mode_on = 0;
   }

   int prev_lev = 0, lev, pass = 1;
   while (demo_mode_on)
   {
      if (num_demo_filenames > 1)
      {
         // have all levels this pass been played?
         int all_played = 1;
         for (int i=0; i< num_demo_filenames; i++)
            if (demo_played[i] < pass) all_played = 0;
         if (all_played == 1) pass++; // next pass

         lev = -1;
         while (lev < 0)
         {
            lev = rand() % num_demo_filenames;      // get random index
            if (demo_played[lev] >= pass) lev = -1; // already been played this pass
            if (prev_lev == lev) lev = -1;          // just previously played
         }
         demo_played[lev] = pass;
         prev_lev = lev;
      }
      else lev = 0;

//     printf("demo level index [%d]\n", lev);
//     for (int i=0; i< num_demo_filenames; i++)
//        printf("%d demo_played[%d] - \n", i, demo_played[i]);

      if (load_gm((char *)al_get_fs_entry_name(demo_FS_filenames[lev])))
      {
         players[0].control_method = 1;
         start_mode = 2; // load level and start, but skip game array erasing
         game_exit = 0;
         pm_main();

         // reset player data
         for (int p=0; p<NUM_PLAYERS; p++)
         {
             players[p].active = 0;
             players[p].control_method = 0;
         }
         players[0].active = 1;
         active_local_player = 0;
      }
      else demo_mode_on = 0;
   }

   load_level(start_level, 0);
   resume_allowed = 0;

}





void temp_test(void)
{




  //select_bitmap_proc();
  //copy_bitmap_proc();
  // animation_proc();




               void spline_adjust(void);
   //            void mdw_an(void);
               spline_test();
//               spline_adjust();












/*
   load_sprit();
   for (int z = 0; z < 32; z++)
   {
      al_set_target_bitmap(memory_bitmap[z]);
      al_lock_bitmap(memory_bitmap[z], al_get_bitmap_format(memory_bitmap[z]), ALLEGRO_LOCK_READWRITE);
      for (int x = 0; x < 20; x++)
         for (int y = 0; y < 20; y++)
         {
            ALLEGRO_COLOR c = al_get_pixel(memory_bitmap[z], x, y);
            float r, g, b;
            al_unmap_rgb_f(c, &r, &g, &b);

            if ( (r < 0.2) && (g < 0.2) && (b < 0.2) )
               al_put_pixel(x, y, al_map_rgb(0,0,0));

         }
      al_unlock_bitmap(memory_bitmap[z]);
   }
    save_sprit();
*/




//   draw_large_2lines(gv, "Purple", "Martians", 8, 1);
//   al_flip_display();
//   tsw();

//   ALLEGRO_COLOR c[5];
//   c[0] = al_map_rgb(255, 0, 0);
//   c[1] = al_map_rgb(0, 255, 0);
//   c[2] = al_map_rgb(0, 0, 255);
//   c[3] = al_map_rgb(255, 255, 255);
//   c[4] = al_map_rgb(0, 0, 0);
//
//   int quit = 0;
//   while (!quit)
//   {
//      proc_controllers();
//      if (key[ALLEGRO_KEY_ESCAPE]) quit =1;
//
//      char m1[256];
//      char m2[256];
//      int bbx1, bby1, bbw1, bbh1;
//      int bbx2, bby2, bbw2, bbh2;
//
//      sprintf(m1, "Michael") ;
//      sprintf(m2, "Weiss") ;
//
//      sprintf(m1, "Purple") ;
//      sprintf(m2, "Martians") ;
//      al_get_text_dimensions(gv, m1, &bbx1, &bby1, &bbw1, &bbh1);
//      al_get_text_dimensions(gv, m2, &bbx2, &bby2, &bbw2, &bbh2);
//
//      // get max w and h
//      float bbw3 = bbw1; if (bbw2 > bbw1) bbw3 = bbw2;
//      float bbh3 = bbh1; if (bbh2 > bbh1) bbh3 = bbh2;
//
//      printf("bbx1:%d bby1:%d bbw1:%d bbh1:%d\n",bbx1, bby1, bbw1, bbh1);
//      printf("bbx2:%d bby2:%d bbw2:%d bbh2:%d\n",bbx2, bby2, bbw2, bbh2);
//
//      ALLEGRO_BITMAP *t1 = al_create_bitmap(bbw3, bbh3);
//      al_set_target_bitmap(t1);
//      al_clear_to_color(al_map_rgb(0,0,0));
//      al_draw_text(gv, palette_color[8], 0-bbx1 + (bbw3-bbw1)/2, 0-bby1, 0, m1);
//      al_convert_mask_to_alpha(t1, al_map_rgb(0, 0, 0));
//
//      ALLEGRO_BITMAP *t2 = al_create_bitmap(bbw3, bbh3);
//      al_set_target_bitmap(t2);
//      al_clear_to_color(al_map_rgb(0,0,0));
//      al_draw_text(gv, palette_color[8], 0-bbx2 + (bbw3-bbw2)/2, 0-bby2, 0, m2);
//      al_convert_mask_to_alpha(t2, al_map_rgb(0, 0, 0));
//
//
//      int xs = SCREEN_W*7/8; // x size
//      int ys = SCREEN_H*3/8; // y size
//
//
//      int x1 = (SCREEN_W - xs)/2;
//      int x2 = xs;
//
//      int yu1 = SCREEN_H*1/4 - ys/2;
//      int yu2 = ys;
//
//      int yl1 = SCREEN_H*3/4 - ys/2;
//      int yl2 = ys;
//
//
//
//
//      al_set_target_backbuffer(display);
//
////      al_draw_line(0, SCREEN_H*1/4, SCREEN_W, SCREEN_H*1/4, c[2], 1);
////      al_draw_line(0, SCREEN_H*2/4, SCREEN_W, SCREEN_H*2/4, c[2], 1);
////      al_draw_line(0, SCREEN_H*3/4, SCREEN_W, SCREEN_H*3/4, c[2], 1);
//
//
//
//      al_draw_scaled_bitmap(t1, 0, 0, bbw3, bbh3, x1, yu1, x2, yu2, 0);
//      al_draw_scaled_bitmap(t2, 0, 0, bbw3, bbh3, x1, yl1, x2, yl2, 0);
//
//      al_flip_display();
//      al_clear_to_color(al_map_rgb(0,0,0));
//
//      al_destroy_bitmap(t1);
//      al_destroy_bitmap(t2);
//   }
//


//   char fn[1000];
//   sprintf(fn, "help/");
//   int ret = mw_file_select("Test get filename", fn, ".txt", 0);
//   printf("ret:%d  filename:[%s]", ret, fn);


   //options_menu();

   /*

   int c, r, g, b;

   c = palette_color[3];
   r = getr(c);
   g = getg(c);
   b = getb(c);
   printf("3 c:%4d   r:%3d g:%3d b:%3d \n", c, r, g, b);

   c = palette_color[9];
   r = getr(c);
   g = getg(c);
   b = getb(c);
   printf("9 c:%4d   r:%3d g:%3d b:%3d \n", c, r, g, b);

   c = palette_color[10];
   r = getr(c);
   g = getg(c);
   b = getb(c);
   printf("10 c:%4d   r:%3d g:%3d b:%3d \n", c, r, g, b);

   c = palette_color[13];
   r = getr(c);
   g = getg(c);
   b = getb(c);
   printf("13 c:%4d   r:%3d g:%3d b:%3d \n", c, r, g, b);

   c = palette_color[15];
   r = getr(c);
   g = getg(c);
   b = getb(c);
   printf("15 c:%4d   r:%3d g:%3d b:%3d \n", c, r, g, b);


   c = palette_color[173];
   r = getr(c);
   g = getg(c);
   b = getb(c);
   printf("173 c:%4d   r:%3d g:%3d b:%3d \n", c, r, g, b);

   c = palette_color[141];
   r = getr(c);
   g = getg(c);
   b = getb(c);
   printf("141 c:%4d   r:%3d g:%3d b:%3d \n", c, r, g, b);

   //            tsw();

   int c, r, g, b;

   c = getpixel(memory_bitmap[400], 6, 3);
   r = getr(c);
   g = getg(c);
   b = getb(c);

   int c32 = makecol32(r, g, b);

   int ccc = 56;
   //            int ccc = palette_color[c];

   int cc8 = palette_color[8];

   //            int cc8 = 23;


   printf("c:%4d   r:%3d g:%3d b:%3d   32:%d  ccc:%d  cc8:%d\n", c, r, g, b, c32, ccc, cc8);


   c = getpixel(memory_bitmap[400], 7, 3);
   c = getpixel(memory_bitmap[400], 8, 3);

   */




   /*

   int size = sizeof(players);
   int lim = size / 1;

   memmove(c_players, players, size); // copy to temp player struct for comparison

   //            players[0].active++;

   //            players[0].paused+=256;

   //            players[0].shape = 256;



   for (i=0; i<lim; i++)
      if (memcmp(&players+i, &c_players+i, 1) != 0)
         printf("difference found in player struct at %d bytes\n", i);
   */

   /*
   int size = sizeof(players);
   char b1[size];
   char b2[size];

   memcpy(b1, players, size);

   players[0].active++;

   players[0].active = 256;

   players[1].active = 256;

   memcpy(b2, players, size);


   for (int i=0; i<size; i++)
      if (b1[i] != b2[i])
         printf("difference found in player struct at %d bytes\n", i);


   tsw();

   */

   /*

     load_level(39, 1);
     void chunk(void);
     chunk();

   */


   /*

   ALLEGRO_PATH * exe  = al_get_standard_path(ALLEGRO_EXENAME_PATH);

   // remove the filename part
   al_set_path_filename(exe, NULL);

   // add savegame as last dir in path
   al_append_path_component(exe, "savegame");

   // convert to string
   const char * cpathx  = al_path_cstr(exe, ALLEGRO_NATIVE_PATH_SEP);
   const char * ret;

   ALLEGRO_FILECHOOSER *afc = al_create_native_file_dialog(cpathx, "Run Game Filename", "*.gm", 0);

   if (al_show_native_file_dialog(display, afc))
   {

   if (al_get_native_file_dialog_count(afc) == 1)
   {
   ret =  al_get_native_file_dialog_path(afc, 0);
   printf("file selected:%s\n", ret);

   }

   }
   else printf("file select cancelled\n" );

   al_destroy_native_file_dialog(afc);



   */

   //     load_gm(NULL);


   //            void spline_test(void);
   //            spline_test();


   //            void spline_test(void);
   //            void spline_adjust(void);
   //            void mdw_an(void);
   //            spline_test();
   //            spline_adjust();
   //            mdw_an();


   // get some random numbers

   // suppose i want from 2-5 with .01

   // scale for int = *100;
   // 200-500

   // range = 300
   // 300

   /*

   for (int y = 0; y<100; y++)
   {
      int r = 200 + rand() % 300;
      al_fixed s = al_itofix(r) / 100;
      printf("[%3.2f]",al_fixtof(s));
   }
   tsw();



   //void solid_test(void);

   //   solid_test();

   //void run_dialog_netgame_conf(void);
   //run_dialog_netgame_conf();


   //void speed_test(void);
   //speed_test();




   // test nested if else


    int a = 5;
    int b = 18;
    int c = 10;

    if (a == 5)
       if (b == 8)
          if (c == 10) printf("test1\n");
             else printf("test2\n");

   tsw();




   int d =4;
   while (d > 0)
   {
      rect(screen, SCREEN_W/2-164, SCREEN_H/2-42, SCREEN_W/2+160, SCREEN_H/2-24, 11);
      sprintf(msg, "Chasing to passcount");
      rtextout_centre(screen, msg, SCREEN_W/2, SCREEN_H/2-32, 10, 2, 0);
      rectfill(screen, SCREEN_W/2-180, SCREEN_H/2-20, SCREEN_W/2+180, SCREEN_H/2+20, 0);
      rect(screen, SCREEN_W/2-180, SCREEN_H/2-20, SCREEN_W/2+180, SCREEN_H/2+16, 10);
      sprintf(msg, "[%d]", d);
      rtextout_centre(screen, msg, SCREEN_W/2, SCREEN_H/2, 10, 4, 0);
       d--;
    }

   tsw();*/

}









