// z_enemy.cpp
#include "pm.h"
#include "z_enemy.h"
#include "z_log.h"
#include "z_player.h"
#include "mwDisplay.h"
#include "mwTimeStamp.h"
#include "mwFont.h"
#include "mwBitmap.h"
#include "z_lift.h"
#include "mwColor.h"
#include "mwProgramState.h"
#include "z_menu.h"
#include "z_item.h"
#include "z_level.h"
#include "z_fnx.h"
#include "z_screen_overlay.h"
#include "mwShots.h"
#include "z_solid.h"



// enemies
int Ei[100][32];        // enemy ints
float Ef[100][16];      // enemy floats

int e_num_of_type[50];
int e_first_num[50];
char enemy_name[100][2][40] = {0};
int num_enemy;
int enemy_tile[20];

int enemy_data(int x_pos, int y_pos)
{
   sort_enemy();
   al_draw_textf(mF.pr8, mC.pc[4], x_pos, y_pos, 0, "%-2d Enemies", num_enemy); y_pos += 8;
   al_draw_text(mF.pr8, mC.pc[4], x_pos, y_pos,  0, "----------"); y_pos += 8;
   for (int c=1; c<16; c++)
   {
      if (e_num_of_type[c]) // not zero
      {
         al_draw_textf(mF.pr8, mC.pc[10], x_pos, y_pos,   0, "%-2d %s", e_num_of_type[c], enemy_name[c][0]);
         y_pos += 8;
      }
   }
   return y_pos;
}


void rectangle_with_diagonal_lines(float x1, float y1, float x2, float y2, int spacing, int frame_color, int line_color, int clip_mode)
{
   int d = mwD.display_transform_double;
   if (!clip_mode) d = 1;

   al_set_clipping_rectangle(x1*d, y1*d, (x2-x1)*d, (y2-y1)*d);

   // find largest dimension
   float xd = x2-x1;
   float yd = y2-y1;
   float ld = xd;
   if (yd > ld) ld = yd;
   for (float k=-ld; k<ld; k+=spacing)
      al_draw_line(x1+k, y1-k, x1+ld+k, y1+ld-k, mC.pc[line_color], 0);
   al_draw_rectangle(x1+0.5, y1+0.5, x2-0.5, y2-0.5, mC.pc[frame_color], 1);
   al_reset_clipping_rectangle();
}


void draw_enemy(int e, int custom, int cx, int cy)
{
   int type = Ei[e][0];
   int EXint = Ef[e][0];
   int EYint = Ef[e][1];
   if (custom)
   {
      EXint = cx;
      EYint = cy;
   }

   if ((type == 13) && (!custom))
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

   int flags = 0;
   if (Ei[e][2] == 0) flags = ALLEGRO_FLIP_HORIZONTAL;
   if (Ei[e][2] == 1) flags = 0;
   if (Ei[e][2] == 2) flags = ALLEGRO_FLIP_VERTICAL;
   if (Ei[e][2] == 3) flags = ALLEGRO_FLIP_VERTICAL & ALLEGRO_FLIP_HORIZONTAL;
   int tn = Ei[e][1];

   float rot = Ef[e][14];
   float sc =  Ef[e][12];

   if ((type == 13) && (Ei[e][15] != 0)) // different rotation point for vinepod
      al_draw_scaled_rotated_bitmap(mwB.tile[tn], 10, 3, EXint+10, EYint+10, sc, sc, rot, flags);
   else al_draw_scaled_rotated_bitmap(mwB.tile[tn], 10, 10, EXint+10, EYint+10, sc, sc, rot, flags);

   // if enemy is expiring show how many seconds it has left
   if ((!mwPS.level_editor_running) && (Ei[e][27])) al_draw_textf(mF.pixl, mC.pc[15], EXint+10, EYint-10, ALLEGRO_ALIGN_CENTER, "%d", 1 + (Ei[e][27] - 10) / 40);

   if ((type == 9) && (!custom)) // cloner
   {
      // trigger box
      float tx1 = (float)Ei[e][11];
      float ty1 = (float)Ei[e][12];
      float tx2 = (float)(Ei[e][11]+Ei[e][13]+20);
      float ty2 = (float)(Ei[e][12]+Ei[e][14]+20);
      int tc1 = 14 + 128; // trigger box color

      // source
      float sx1 = (float)Ei[e][15];
      float sy1 = (float)Ei[e][16];
      float sx2 = sx1 + (float)Ei[e][19];
      float sy2 = sy1 + (float)Ei[e][20];
      int sc1 = 11 + 128; // source box color

      // destination
      float dx1 = (float)Ei[e][17];
      float dy1 = (float)Ei[e][18];
      float dx2 = dx1 + (float)Ei[e][19];
      float dy2 = dy1 + (float)Ei[e][20];
      int dc1 = 10 + 128; // destination box color

      int m = Ei[e][5]; // 2 - 9  total seq (8)

      if ((m > 1) && (m < 6)) // first half (2 - 5) // flash source box green
      {
         int co = 0, d = m-2; // 0 to 3
         if (d == 0) co = 64;
         if (d == 1) co = 0;
         if (d == 2) co = 0;
         if (d == 3) co = 64;
         sc1 = 11 + co; // source box color
      }
      if ((m > 5) && (m < 10)) // second half (6 - 9) // flash destination box red
      {
         int co = 0, d =  m-6; // 0 to 3
         if (d == 0) co = 64;
         if (d == 1) co = 0;
         if (d == 2) co = 0;
         if (d == 3) co = 64;
         dc1 = 10 + co; // destination box color
      }
      if (Ei[e][5] != 0) // in trigger box
      {
         tc1 = 14 + 32; // trigger box color brighter

         // show vertical red green bar animation sequence
        // int b = (Ei[e][7] * 10) / (Ei[e][6]+1);
        // int t = mwB.zz[5+b][53];
        // al_draw_scaled_rotated_bitmap(mwB.tile[t], 10, 10, EXint+10, EYint+10, .5, .5, 0, ALLEGRO_FLIP_VERTICAL);
      }

      // show box mode (0=none) (1=trig only) (2=src/dst only) (3=all)
      int q = Ei[e][4];
      if ((q == 1) || (q == 3))
         rectangle_with_diagonal_lines(tx1, ty1, tx2, ty2, 8, tc1, tc1+64, 0); // trigger box
      if ((q == 2) || (q == 3))
      {
         rectangle_with_diagonal_lines(sx1, sy1, sx2, sy2, 8, sc1, sc1+64, 0); // source
         rectangle_with_diagonal_lines(dx1, dy1, dx2, dy2, 8, dc1, dc1+64, 0); // destination
      }
   }

/*
   if (Ei[e][0] == 5) // jumpworm
   {
      // enemy position (add 10 to get the center)
      int x1 = Ef[e][0];
      int y1 = Ef[e][1];
      int x2 = x1 + 20;
      int y2 = y1 + 20;
      al_draw_rectangle(x1, y1, x2, y2, mC.pc[14], 1);
   }
*/

   #ifdef SHOW_CANNON_COLLISION_BOX
   if (Ei[e][0] == 6) // cannon
   {
      // draw some test rects here
      int cbs = Ei[e][29]; // collision box size

      // enemy position (add 10 to get the center)
      int x1 = Ef[e][0] + 10;
      int y1 = Ef[e][1] + 10;

      int il = cbs;
      int color = 11;
      //  printf("x1:%d  y1:%d il:%d  color:%d\n",x1, y1, il, color);

      // check for collision with player
      int b = Ei[e][29]; // collision box size
      for (int p=0; p<NUM_PLAYERS; p++)
         if ((players[p].active) && (!players[p].paused))
         {
            float px = players[p].PX;
            float py = players[p].PY;

            float ex1 = Ef[e][0] - b;
            float ex2 = Ef[e][0] + b;
            float ey1 = Ef[e][1] - b;
            float ey2 = Ef[e][1] + b;

            // if player in collision box color = red
            if ((px > ex1) && (px < ex2) && (py > ey1) && (py < ey2)) color = 10;
      }
      al_draw_rectangle(x1-il, y1-il, x1+il, y1+il, mC.pc[color], 1);
   }
   #endif



/*
   if (Ei[e][0] == 3) // archwagon
   {
      al_draw_textf(mF.pr8, mC.pc[15], EXint+30, EYint+10, 0, "%d", Ei[e][5]);
   }
*/


   #ifdef SHOW_FLAPPER_DEBUG
   if (Ei[e][0] == 12) // flapper
   {
      int prox = Ei[e][17];
      int color = 14;        // default color
      int ex = EXint+10;
      int ey = EYint+10;
      float pi = ALLEGRO_PI;

      float th1 = -pi/4;   // 45 deg
      float th2 = th1+pi;  // 225 deg
      float thd = pi / 2;  // 90 deg

      // draw flap height
      int base =  Ei[e][14]+10;
      int y1 = base - Ei[e][21];
      int y2 = base + Ei[e][21];

      al_draw_line(EXint-140, base, EXint+140, base, mC.pc[11], 1);
      al_draw_rectangle(EXint-40, y1, EXint+40, y2, mC.pc[11], 1);

      // draw height above player
      int hab =  Ei[e][20];
      al_draw_line(EXint-40, base+hab, EXint+40, base+hab, mC.pc[12], 1);

      int p = find_closest_player_flapper(e);
      if (p != -1)
      {
         color = 10;
         bomb_crosshairs(players[p].x+10, players[p].y+10); // mark targetted player
      }
      if (Ef[e][2] > 0) al_draw_pieslice(ex, ey, prox, th1, thd, mC.pc[color], 1);
      else              al_draw_pieslice(ex, ey, prox, th2, thd, mC.pc[color], 1);
   }
   #endif

   #ifdef SHOW_TRAKBOT_BULLET_TRIGGER_CIRCLE
   if (Ei[e][0] == 8) // trakbot
   {
      int prox = Ei[e][17];
      int color = 14;        // default color
      int ex = EXint+10;
      int ey = EYint+10;
      float pi = ALLEGRO_PI;
      // is any player in range?
      int p = find_closest_player_trakbot(e);
      if (p != -1)
      {
         color = 10;
         bomb_crosshairs(players[p].x+10, players[p].y+10); // mark targetted player
      }


      int quad;
      switch (Ei[e][5]) // mode
      {
         case 0: case 5: quad = 1; break; // floor right, lwall up
         case 1: case 4: quad = 2; break; // rwall up floor left
         case 2: case 7: quad = 3; break; // ceil left, rwall down
         case 3: case 6: quad = 4; break; // lwall down, ceil right
      }
      if (quad == 1) al_draw_pieslice(ex, ey, prox, -pi/2, pi/2, mC.pc[color], 0);
      if (quad == 4) al_draw_pieslice(ex, ey, prox, 0,     pi/2, mC.pc[color], 0);
      if (quad == 3) al_draw_pieslice(ex, ey, prox, pi/2,  pi/2, mC.pc[color], 0);
      if (quad == 2) al_draw_pieslice(ex, ey, prox, pi,    pi/2, mC.pc[color], 0);
   }
   #endif
}



void draw_enemies(void)
{
   al_set_target_bitmap(mwB.level_buffer);
   for (int e=0; e<100; e++)
      if (Ei[e][0])  // if enemy active
         draw_enemy(e, 0, 0, 0);
}

void proc_enemy_collision_with_pshot(int e)
{
   float ex = Ef[e][0];
   float ey = Ef[e][1];

   for (int c=0; c<50; c++)
      if (mwS.p[c].active)
      {
         // shot collision box size adjusted with shot speed
         float cx = fabs(mwS.p[c].xinc/2) + 8;
         float cy = fabs(mwS.p[c].yinc/2) + 8;

         // shot collision box size adjusted with enemies collision box size
         cx += (float) Ei[e][29] - 10;
         cy += (float) Ei[e][29] - 10;

         float bx1 = mwS.p[c].x - cx;
         float bx2 = mwS.p[c].x + cx;
         float by1 = mwS.p[c].y - cy;
         float by2 = mwS.p[c].y + cy;

         // check for collision with player's shots
         if ((ex > bx1) && (ex < bx2) && (ey > by1) && (ey < by2))
         {
            //printf("hit!\n");
            int p = mwS.p[c].player;       // player number of shot
            Ei[e][31] = 1;                 // flag that this enemy got sho
            Ei[e][26] = p;                 // number of player that shot enemy
            players[p].stat_enemy_hits++;  // add to number of hits the player has
            mwS.p[c].active = 0;           // shot dies
         }
      }
}

void move_enemies()
{
   if (LOG_TMR_move_enem) mwTS.init_timestamps();
   num_enemy = 0; // count enemies
   for (int e=0; e<100; e++)
      if (Ei[e][0])
      {
         if (LOG_TMR_move_enem) t0 = al_get_time();
         num_enemy++; // enemy count
         if (Ei[e][0] < 50) proc_enemy_collision_with_pshot(e);
         // check for time to live
         int ttl = Ei[e][27];
         if (ttl)
         {
            if (ttl < 11)
            {
               Ei[e][0] = 66;             // change to different type to prevent use
               Ef[e][4] = 0;  // cant hurt anymore
               Ei[e][29] = 0;             // no collision box
               int sq = 10-ttl;
               Ei[e][1] = mwB.zz[5+sq][74];
            }
            if (ttl == 1) Ei[e][0] = 0; // kill instantly
            Ei[e][27]--;
         }

         // check for out of bounds
         if ((Ef[e][0] < 0) || (Ef[e][0] > 1980)) Ei[e][0]=0;
         if ((Ef[e][1] < 0) || (Ef[e][1] > 1980)) Ei[e][0]=0;

         switch (Ei[e][0])
         {
            case 3:  enemy_archwagon(e);  break;
            case 4:  enemy_bouncer(e);  break;
            case 5:  enemy_jumpworm(e);  break;
            case 6:  enemy_cannon(e);  break;
            case 7:  enemy_podzilla(e);  break;
            case 8:  enemy_trakbot(e);  break;
            case 9:  enemy_cloner(e);  break;
            case 11: enemy_block_walker(e);  break;
            case 12: enemy_flapper(e);  break;
            case 13: enemy_vinepod(e);  break;
            case 99: enemy_deathcount(e); break;
         }
         if (LOG_TMR_move_enem) mwTS.add_timestamp(103, e, Ei[e][0], al_get_time()-t0, 0);
      }
   if (LOG_TMR_move_enem)
   {
      // tally up all the times for each enemy type
      double tmr_tally[100][3] = {0};
      for (int i=0; i<10000; i++)
         if (mwTS.timestamps[i].type == 103)
         {
            tmr_tally[mwTS.timestamps[i].frame2][0] +=1;                 // add to number of this type tally
            tmr_tally[mwTS.timestamps[i].frame2][1] += mwTS.timestamps[i].t1; // add to time tally
         }
      // build log entry
      char t[512] = {0};
      char l[64] = {0};
      for (int i=0; i<100; i++)
         if (tmr_tally[i][1] > 0)
         {
            sprintf(l, "m-%s:[%0.4f] ", enemy_name[i][1], (tmr_tally[i][1]/tmr_tally[i][0])*1000000);
            strcat(t, l);
         }
      sprintf(msg, "tmst %s\n", t);
      // printf("%s", msg);
      add_log_entry2(44, 0, msg);
   }
}



void enemy_deathcount(int e)
{
   int EXint = Ef[e][0];
   int EYint = Ef[e][1];
   Ef[e][14] += Ef[e][13]; // rot inc
   Ef[e][12] *= Ef[e][11]; // scale scaler

   Ef[e][0] += Ef[e][2]; // xinc
   Ef[e][1] += Ef[e][3]; // yinc

   Ei[e][1] = mwB.zz[0][ Ei[e][3] ]; // draw current ans shape
   // dec and check countdown timer
   if (--Ei[e][30] < 0) // create bonus
   {
      Ei[e][0] = 0; // kill enemy
      for (int c=0; c<500; c++)
         if (item[c][0] == 0) // find empty
         {
            for (int y=0; y<16; y++) item[c][y] = 0; // clear item
            item[c][0] = 2;           // type - bonus
            item[c][1] = Ei[e][24];  // flower shape
            item[c][2] = 1; // draw mode normal
            item[c][3] = -1; // carryable
            item[c][4] = EXint;
            item[c][5] = EYint;
            item[c][6] = 1; // bonus type 1 - health
            item[c][7] = Ei[e][25];  //  life
            item[c][8] = 0;

            item[c][14] = 800; // time to live

            itemf[c][0] = item[c][4];
            itemf[c][1] = item[c][5];
            itemf[c][2] = 0;
            itemf[c][3] = 0;
            break; // end loop
         }
   }
}

void enemy_player_hit_proc(int e)
{
   if (--Ei[e][23]<0) // hit player retrigger
   {
      if (Ei[e][22]) // player hit!
      {
         int p = Ei[e][22]-1;
         players[p].health -= Ef[e][4];

         game_event(44, 0, 0, p, e, 0, Ef[e][4]);
         Ei[e][22] = 0;  // clear hit
         Ei[e][23] = 60; // set retrigger amount
      }
   }
  else Ei[e][22] = 0;
}

void enemy_killed(int e)
{
   Ef[e][2] = 0;       // xinc
   Ef[e][3] = 0;       // yinc

   switch (Ei[e][0])
   {
      case 3: // archwagon
         Ei[e][3]  = 34;    // new ans
         Ei[e][30] = 20;    // death_loop_wait; set delay
         Ei[e][24] = 929;   // health bonus tile
         Ef[e][11] = 1.08;  // scale multiplier
         Ef[e][13] = 0;     // rot inc
      break;
      case 4: // bouncer
         if (Ei[e][3] == 29) Ei[e][3] = 46;  // new ans
         if (Ei[e][3] == 14) Ei[e][3] = 13;
         Ei[e][30] = 20;    // death_loop_wait;  set delay
         Ei[e][24] = 928;   // health bonus tile
         Ef[e][11] = 1.03;  // scale multiplier
         Ef[e][13] = 0.05;  // rot inc
      break;
      case 5: //jumpworm
         Ei[e][3]  = 79;    // new ans
         Ei[e][30] = 20;    // death_loop_wait; set delay
         Ei[e][24] = 935;   // health bonus tile
         Ef[e][11] = 0.96;  // scale multiplier
         Ef[e][13] = 0.5;   // rot inc
         Ef[e][3] = -2;     // yinc
      break;
      case 6: // cannon
         Ei[e][3]  = 37;    // new ans
         Ei[e][30] = 20;    // death_loop_wait; set delay
         Ei[e][24] = 930;   // health bonus tile
         Ef[e][11] = 1.08;  // scale multiplier
         Ef[e][13] = 0.024; // rot inc
      break;
      case 7: case 13: // podzilla, vinepod
         Ei[e][3]  = 45;    // new ans
         Ei[e][30] = 40;    // death_loop_wait; set delay
         Ei[e][24] = 932;   // health bonus tile
         Ef[e][12] = 2.8;   // initial scale
         Ef[e][11] = 0.94;  // scale multiplier
         Ef[e][13] = 0;     // rot inc
      break;
      case 8: // trakbot
         Ei[e][3]  = 44;    // new ans
         Ei[e][30] = 12;    // death_loop_wait; set delay
         Ei[e][24] = 931;   // health bonus tile
         Ef[e][11] = 1.00;  // scale multiplier
         Ef[e][13] = 0.4;   // rot inc
      break;
      case 9: // cloner
         Ei[e][3]  = 105;   // new ans
         Ei[e][30] = 40;    // death_loop_wait; set delay
         Ei[e][24] = 934;   // health bonus tile
         Ef[e][11] = 0.98;  // scale multiplier
         Ef[e][13] = 0.2;   // rot inc
      break;
      case 12: // flapper
         Ei[e][3]  = 63;    // new ans
         Ei[e][30] = 20;    // death_loop_wait; set delay
         Ei[e][24] = 933;   // health bonus tile
         Ef[e][11] = 1.04;  // scale multiplier
         Ef[e][13] = 0;     // rot inc
      break;

   }

   int hbm = 1;                 // default health bonus multiplier x1
   if (Ei[e][31] == 2) hbm = 2; // explosion x2
   Ei[e][25] *= hbm;            // health bonus amount
   Ei[e][24] += (hbm-1)*32;     // health bonus tile multiplier for diamonds

   Ei[e][0] = 99;      // set type to death loop
   Ef[e][4] = 0;       // can't hurt player anymore
   int na = Ei[e][3];  // new ans
   int dl = Ei[e][30]; // death_loop_wait; set delay

   mwB.zz[0][na] = mwB.zz[5][na];      // set shape
   mwB.zz[1][na] = 0;                  // point to zero
   mwB.zz[2][na] = 0;                  // set counter
   mwB.zz[3][na] = dl / mwB.zz[4][na]; // set ans timer

   if (hbm == 1) game_event(60, 0, 0, Ei[e][26], e, 0, 0);
   if (hbm == 2) game_event(62, 0, 0, Ei[e][26], e, 0, 0);
}


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



void enemy_flapper(int e)
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

      if (is_right_solid(Ef[e][0], Ef[e][1], 1, 2))
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
            mwS.fire_enemy_shota(e, 62, p);
            Ei[e][16] = Ei[e][15]; // set new prox wait
         }
      }
   }
   else  // move left
   {
      Ef[e][2] -= Ef[e][6];                           // accel
      if (Ef[e][2] < -Ef[e][5]) Ef[e][2] = -Ef[e][5]; // max speed
      Ef[e][0] += Ef[e][2];                           // apply xinc

      if (is_left_solid(Ef[e][0], Ef[e][1], 1, 2))
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
            mwS.fire_enemy_shota(e, 62, p);
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
      float df = Ef[e][1] - (players[p].y - h);
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
      yinc = is_up_solidf(Ef[e][0], Ef[e][1], -yinc, 0);
      Ef[e][1] -= yinc; // apply allowed move
   }
   else if (yinc > 0) // moving down
   {
      yinc = is_down_solidf(Ef[e][0], Ef[e][1], yinc, 0);
      Ef[e][1] += yinc; // apply allowed move
   }
}




int is_player_in_trigger_box(int x1, int y1, int x2, int y2)
{
   for (int p=0; p<NUM_PLAYERS; p++)
      if ((players[p].active) && (!players[p].paused))
      {
         int px = players[p].x;
         int py = players[p].y;
         if ((px > x1) && (px < x2) && (py > y1) && (py < y2)) return 1;
      }
   return 0;
}


void cloner_create(int e)
{
   float x1 = Ei[e][15]-2;    // source
   float y1 = Ei[e][16]-2;
   float x3 = Ei[e][17]-2;    // destination
   float y3 = Ei[e][18]-2;
   float  w = Ei[e][19];     // width
   float  h = Ei[e][20];     // height
   float x2 = x1 + w;
   float y2 = y1 + h;


   int no=0, cl=Ei[e][10]; // limit on number of created objects
   if (cl)
   {
      // count number of active objects tagged by this cloner
      for (int x=0; x<100; x++)
         if ((Ei[x][0]) && (Ei[x][28] == 1000 + e)) no++;
      for (int x=0; x<500; x++)
         if ((item[x][0]) && (item[x][15] == 1000 + e)) no++;
   }
   else cl = 600; // no limit is same as max limit
   if (no < cl) // if number of objects < create limit
   {
      // check for enemies in box
      for (int b=0; b<100; b++)
         if (Ei[b][0])
            if ((Ef[b][0] > x1) && (Ef[b][0] < x2) && (Ef[b][1] > y1) && (Ef[b][1] < y2) && (no < cl))
            {
               // check if new position is empty
               float new_x_pos = Ef[b][0] + x3 - x1;
               float new_y_pos = Ef[b][1] + y3 - y1;
               int nx = new_x_pos / 20;
               int ny = new_y_pos / 20;
               if (is_block_empty(nx, ny, 1, 0, 0)) // block only
               {
                  for (int c=0; c<100; c++) // look for a place to put it
                     if (Ei[c][0] == 0)  // found empty
                     {
                        for (int y=0; y<32; y++) Ei[c][y] = Ei[b][y];
                        for (int y=0; y<16; y++) Ef[c][y] = Ef[b][y];
                        Ef[c][0] = new_x_pos;
                        Ef[c][1] = new_y_pos;
                        Ei[c][27] = Ei[e][9]; // set time to live
                        Ei[c][28] = 1000+e;   // tag with cloner item id
                        c = 100; // end loop
                        no++;    // one more object created
                     }
               }
            }

      // check for items in box
      for (int b=0; b<500; b++)
         if (item[b][0])
         {
            float ix = itemf[b][0];
            float iy = itemf[b][1];
            if ((ix > x1) && (ix < x2) && (iy > y1) && (iy < y2) && (no < cl))
            {
               // check if new position is empty
               float new_x_pos = ix + x3 - x1;
               float new_y_pos = iy + y3 - y1;
               int nx = new_x_pos / 20;
               int ny = new_y_pos / 20;
               if (is_block_empty(nx, ny, 1, 0, 0)) // block only
               {
                  for (int c=0; c<500; c++)
                     if (item[c][0] == 0) // found empty
                     {
                        for (int y=0; y<16; y++) item[c][y] = item[b][y];
                        itemf[c][0]= new_x_pos;
                        itemf[c][1]= new_y_pos;
                        itemf[c][2]= 0;
                        itemf[c][3]= 0;

                        // are we copying something that already has an expiry date?? if so leave it
                        if (item[b][14] == 0) item[c][14] = Ei[e][9]; // otherwise set time to live from cloner
                        if (item[c][0] == 10) strcpy(pmsgtext[c], pmsgtext[b]); // message
                        item[c][15] = 1000+e;   // tag with cloner item id
                        c = 500; // end loop
                        no++;    // one more object created
                     }
               }
            }
         }
   }
}














//--9--cloner-----------------------------------------------------------------------------
//      Ei[e][5] = player in box
//      Ei[e][6] = create wait
//      Ei[e][7] = create wait counter
//      Ei[e][8] = trigger mode (0 = wait, 1=reset, 2=immed)
//      Ei[e][11] = trigger box x1
//      Ei[e][12] = trigger box y1
//      Ei[e][13] = trigger box x2
//      Ei[e][14] = trigger box y2
//      Ei[e][15] = copy box x
//      Ei[e][16] = copy box y
//      Ei[e][17] = dest box x
//      Ei[e][18] = dest box y
//      Ei[e][19] = copy box width
//      Ei[e][20] = copy box height

void enemy_cloner(int e)
{
   int x4 = Ei[e][11] - 10; // trigger box
   int y4 = Ei[e][12] - 10;
   int x5 = Ei[e][11] + Ei[e][13] + 10;
   int y5 = Ei[e][12] + Ei[e][14] + 10;

   Ei[e][1] = mwB.zz[0][105]; // default shape

   if (Ei[e][31] && (Ei[e][30] == 0)) // hit and not invincible
   {
      enemy_killed(e);
      return;
   }
   else Ei[e][31] = 0;
   enemy_player_hit_proc(e);



   // set draw shape
   Ei[e][2] = 0;  // flip mode

//   int b = (Ei[e][7] * 7) / (Ei[e][6]+1);
//   Ei[e][1] = mwB.zz[5+b][106];

   int b = (Ei[e][7] * 9) / (Ei[e][6]+1);
   Ei[e][1] = mwB.zz[5+b][107];

   if (Ei[e][8] == 2) Ei[e][1] = mwB.zz[0][105]; // to make something happen for immed

//   printf("%d %d %d \n", b, Ei[e][7], Ei[e][6]);


   if (Ei[e][7] > Ei[e][6]) Ei[e][7] = Ei[e][6]; // ensure counter is never more than counter reset value


   int create_now = 0;

   int player_in_box = is_player_in_trigger_box(x4, y4, x5, y5);

   int player_just_entered_trigger_box = 0;
   if ((!Ei[e][5]) && (player_in_box)) player_just_entered_trigger_box = 1; // not in trig box last time and in box this time



   // player in box last time (Ei[e][5]
   // 0 = not in trigger box last time
   // 1 = in trigger box last time

   // Ei[][8]  trigger mode (0=wait, 1=reset, 2=immed)//

   // wait mode, player in box, run timer
   if ((Ei[e][8] == 0) && (player_in_box) && (--Ei[e][7] <= 0))
   {
      Ei[e][7] = Ei[e][6]; // reset counter
      create_now = 1;
   }

   // mode immed
   if ((Ei[e][8] == 2) && (player_just_entered_trigger_box)) create_now = 1;


   // mode reset timer
   if (Ei[e][8] == 1)
   {
      if (player_just_entered_trigger_box) Ei[e][7] = Ei[e][6];

      if (player_in_box)
      {
         if (--Ei[e][7] <= 0) // other wise run timer
         {
            Ei[e][7] = Ei[e][6]; // reset counter
            create_now = 1;
         }
      }
   }
   if (player_in_box) Ei[e][5] = 1; // for next time
   else Ei[e][5] = 0;
   if (create_now) cloner_create(e);
}


void set_trakbot_mode(int e, int mode)
{
   Ei[e][5] = mode;
   switch (mode)
   {
      case 0: Ei[e][2] = 1; Ef[e][14] = deg_to_rad(0);   break; // floor right
      case 1: Ei[e][2] = 1; Ef[e][14] = deg_to_rad(270); break; // rwall up
      case 2: Ei[e][2] = 1; Ef[e][14] = deg_to_rad(180); break; // ceil left
      case 3: Ei[e][2] = 1; Ef[e][14] = deg_to_rad(90);  break; // lwall down
      case 4: Ei[e][2] = 0; Ef[e][14] = deg_to_rad(0);   break; // floor left
      case 5: Ei[e][2] = 0; Ef[e][14] = deg_to_rad(90);  break; // lwall up
      case 6: Ei[e][2] = 0; Ef[e][14] = deg_to_rad(180); break; // ceil right
      case 7: Ei[e][2] = 0; Ef[e][14] = deg_to_rad(270); break; // rwall down
   }
}

//--8--trakbot-----------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------


//     Ei[e][0] = type
//     Ei[e][1] = shape
//     Ei[e][2] = draw_type
//     Ei[e][3] = ans
//     Ei[e][4] = fall count
//     Ei[e][5] = mode
//     Ei[e][6] =
//     Ei[e][7] = drop mode 1 = yes
//     Ei[e][8] =


//     Ei[e][15] = shoot holdoff value
//     Ei[e][16] = shoot holdoff counter
//     Ei[e][17] = prox


//     Ei[e][22] = player hit
//     Ei[e][23] = player hit retrigger
//     Ei[e][25] = health bonus
//     Ei[e][27] = bonus shape
//     Ei[e][28] = number of extra hits to kill
//     Ei[e][30] = death loop count
//     Ei[e][31] = enemy hit

//     Ef[e][0] = x
//     Ef[e][1] = y
//     Ef[e][2] = xinc
//     Ef[e][3] = yinc
//     Ef[e][4] = LIFE decrement

//     Ef[e][7] = shot speed

//     Ef[e][11] = scale multiplier
//     Ef[e][12] = scale;
//     Ef[e][13] = rot inc


void enemy_trakbot(int e)
{
   float mv;
   int dp = 0; // debug print
   int shot_request = 0;
   int mode = Ei[e][5];

   if (Ei[e][31]) // hit
   {
      enemy_killed(e);
      return;
   }
   enemy_player_hit_proc(e);

   if (--Ei[e][16] < 0)  // shot retrigger wait
   {
      shot_request = 1; // try to shoot
      Ei[e][3] = 17;     // show trakbot with shot
   }
   else Ei[e][3] = 18;   // show trakbot without shot


   if (Ei[e][4] != 0)   // if falling
   {
      if (dp) printf("mode:fall  x:%1.2f  y:%1.2f\n", Ef[e][0], Ef[e][1]);

      Ei[e][4] += 5;                                             // add acceleration to fsll
      if (Ei[e][4] > 160)  Ei[e][4] = 160;                      // terminal velocity
      float fall_yinc = Ef[e][3] * Ei[e][4] / 100;              // (fix * int) / int
      mv = is_down_solidf(Ef[e][0], Ef[e][1], fall_yinc, 0);   // will we hit floor during this move?
      if (mv < fall_yinc) Ei[e][4] = 0;                          // end fall if allowed move less than requested move
      Ef[e][1] += mv;                                           // move the allowed amount
      if (Ef[e][1] > 1980) Ei[e][0]=0;                 // kill if falls past bottom of level

      // seek player while falling
      int p = find_closest_player(e);
      float ex = Ef[e][0];
      float px = players[p].x;

      if (px > ex) mode = 0;
      else mode = 4;
      set_trakbot_mode(e, mode);
   }
   else // not fall
   {
      if ((Ei[e][7]) && ((mode == 2) || (mode == 6))) // if drop mode on and trakbot on ceiling
      {
         float ex = Ef[e][0];
         float  ey = Ef[e][1];
         float ew = 15;
         for (int p=0; p<NUM_PLAYERS; p++) // if any player passes underneath
            if ((players[p].active) && (!players[p].paused))
            {
               float px = players[p].x;
               float py = players[p].y;
               if ((px < ex + ew) && (px > ex - ew) && (py > ey))  // if directly above player
               {
                  if (dp) printf("just dropped on player:%d\n", p);
                  Ei[e][4] = 5; // start fall mode
                  if (mode == 2) set_trakbot_mode(e, 4);
                  if (mode == 6) set_trakbot_mode(e, 0);
               }
            }
      }
   }

   if (Ei[e][4] == 0)  // if not still not fall
   {
      if (dp) printf("mode:%d  x:%1.2f  y:%1.2f - ", mode, Ef[e][0], Ef[e][1]);
      switch (mode)
      {
         case 0:                                                  // floor right
            Ei[e][2] = 1;                                        // no h flip
            Ef[e][14] = deg_to_rad(0);
            if (dp) { if (is_down_solidf(Ef[e][0], Ef[e][1], 1, 0) < 1) printf(" - 0 with solid floor\n"); else  printf(" - 0 with NOT solid floor\n"); }
            mv = is_down_solidf(Ef[e][0], Ef[e][1], Ef[e][2], 1);
            if (mv < Ef[e][2]) // allowed move less than requested move
            {
               if (dp) printf("will lose floor during this move - mv%1.2f \n", mv);
               Ef[e][0] += mv;                                  // move the allowed amount
               mode = 3;                                          // next mode (lwall down) (cw)
               Ef[e][14] = deg_to_rad(45);
               Ef[e][1] += 1;                           // move down 1 to catch new wall
               break;
            }
            mv = is_right_solidf(Ef[e][0], Ef[e][1], Ef[e][2], 0);
            if (mv < Ef[e][2]) // allowed move less than requested move
            {
               if (dp) printf("will hit wall during this move - mv%1.2f \n", mv);
               Ef[e][0] += mv;                                  // move the allowed amount
               mode = 1;                                          // next mode (rwall up) (ccw)
               Ef[e][14] = deg_to_rad(315);
               break;
            }
            Ef[e][0] += mv;  // if neither of these matched, move the allowed amount
         break;
         case 1:                                                  // rwall up
            Ei[e][2] = 1;                                        // no h flip
            Ef[e][14] = deg_to_rad(270);
            if (dp) { if (is_right_solidf(Ef[e][0], Ef[e][1], 1, 0) < 1) printf(" - 1 with solid r wall\n"); else  printf(" - 1 with NOT solid r wall\n"); }
            mv = is_right_solidf(Ef[e][0], Ef[e][1], Ef[e][3], -1);
            if (mv < Ef[e][3]) // allowed move less than requested move
            {
               if (dp) printf("will lose rwall during this move - mv%1.2f \n", mv);
               Ef[e][1] -= mv;                                  // move the allowed amount
               mode = 0;                                          // next mode (floor right) (cw)
               Ef[e][14] = deg_to_rad(315);
               Ef[e][0] += 1;                           // move right 1 to catch new wall
               break;
            }
            mv = is_up_solidf(Ef[e][0], Ef[e][1], Ef[e][3], 0);
            if (mv < Ef[e][3]) // allowed move less than requested move
            {
               if (dp) printf("will hit ceiling during this move - mv%1.2f \n", mv);
               mode = 2;                                          // next mode (ceil left) (ccw)
               Ef[e][14] = deg_to_rad(225);
               Ef[e][1] -= mv;                                  // move the allowed amount
               break;
            }
            Ef[e][1] -= mv;  // if neither of these matched, move the allowed amount
         break;
         case 2:                                                  // ceil left
            Ei[e][2] = 1;                                        // no h flip
            Ef[e][14] = deg_to_rad(180);
            if (dp) { if (is_up_solidf(Ef[e][0], Ef[e][1], 1, 0) < 1) printf(" - 2 with solid ceil\n"); else  printf(" - 2 with NOT solid ceil\n"); }
            mv = is_up_solidf(Ef[e][0], Ef[e][1], Ef[e][2], -1);
            if (mv < Ef[e][2]) // allowed move less than requested move
            {
               if (dp) printf("will lose ceiling during this move - mv%1.2f \n", mv);
               Ef[e][0] -= mv;                                  // move the allowed amount
               mode = 1;                                          // next mode (rwall up) (cw)
               Ef[e][14] = deg_to_rad(225);
               Ef[e][1] -= 1;                           // move up 1 to catch new wall
               break;
            }
            mv = is_left_solidf(Ef[e][0], Ef[e][1], Ef[e][2], 0);
            if (mv < Ef[e][2]) // allowed move less than requested move
            {
               if (dp) printf("will hit lwall during this move - mv%1.2f \n", mv);
               mode = 3;                                          // next mode (lwall down) (ccw)
               Ef[e][14] = deg_to_rad(135);
               Ef[e][0] -= mv;                                  // move the allowed amount
               break;
            }
            Ef[e][0] -= mv;  // if neither of these matched, move the allowed amount
         break;
         case 3:                                                  // lwall down
            Ei[e][2] = 1;                                        // no h flip
            Ef[e][14] = deg_to_rad(90);
            if (dp) { if (is_left_solidf(Ef[e][0], Ef[e][1], 1, 0) < 1) printf(" - 3 with solid lwall\n"); else  printf(" - 3 with NOT solid lwall\n"); }
            mv = is_left_solidf(Ef[e][0], Ef[e][1], Ef[e][3], 1);
            if (mv < Ef[e][3]) // allowed move less than requested move
            {
               if (dp) printf("will lose lwall during this move - mv%1.2f \n", mv);
               Ef[e][1] += mv;                                  // move the allowed amount
               mode = 2;                                          // next mode  (ceil left) (cw)
               Ef[e][14] = deg_to_rad(135);
               Ef[e][0] -= 1;                           // move left 1 to catch new wall
               break;
            }
            mv = is_down_solidf(Ef[e][0], Ef[e][1], Ef[e][3], 0);
            if (mv < Ef[e][3]) // allowed move less than requested move
            {
               if (dp) printf("will hit floor during this move - mv%1.2f \n", mv);
               mode = 0;                                          // next mode (floor right) (ccw)
               Ef[e][14] = deg_to_rad(45);
               Ef[e][1] += mv;                                  // move the allowed amount
               break;
            }
            Ef[e][1] += mv;  // if neither of these matched, move the allowed amount
         break;
         case 4:                                                  // floor left
            Ei[e][2] = 0;                                        // h flip
            Ef[e][14] = deg_to_rad(0);
            if (dp) { if (is_down_solidf(Ef[e][0], Ef[e][1], 1, 0) < 1) printf(" - 4 with solid floor\n"); else  printf(" - 4 with NOT solid floor\n"); }

            mv = is_down_solidf(Ef[e][0], Ef[e][1], Ef[e][2], -1);
            if (mv < Ef[e][2])                                  // allowed move less than requested move
            {
               Ef[e][0] -= mv;                                  // move the allowed amount
               mode = 7;                                          // next mode (rwall down) (ccw)
               Ef[e][14] = deg_to_rad(315);
               Ef[e][1] += 1;                           // move down 1 to catch new wall
               if (dp) printf("lost floor during this move - mv%1.2f \n", mv);
               break;
            }
            mv = is_left_solidf(Ef[e][0], Ef[e][1], Ef[e][2], 0);
            if (mv < Ef[e][2])                                  // allowed move less than requested move
            {
               Ef[e][0] -= mv;                                  // move the allowed amount
               mode = 5;                                          // next mode (lwall up) (cw)
               Ef[e][14] = deg_to_rad(45);
               if (dp) printf("hit lwall during this move - mv%1.2f \n", mv);
               break;
            }
            Ef[e][0] -= mv;  // if neither of these matched, move the allowed amount
         break;
         case 5:                                                  // lwall up
            Ei[e][2] = 0;                                        // h flip
            Ef[e][14] = deg_to_rad(90);
            if (dp) { if (is_left_solidf(Ef[e][0], Ef[e][1], 1, 0) < 1) printf(" - 5 with solid left\n"); else  printf(" - 5 with NOT solid left\n"); }
            mv = is_left_solidf(Ef[e][0], Ef[e][1], Ef[e][3], -1);
            if (mv < Ef[e][3])                                  // allowed move less than requested move
            {
               Ef[e][1] -= mv;                                  // move the allowed amount
               mode = 4;                                          // next mode (floor left) (ccw)
               Ef[e][14] = deg_to_rad(45);
               Ef[e][0] -= 1;                           // move left 1 to catch new wall
               if (dp) printf("lost lwall during this move - mv%1.2f \n", mv);
               break;
            }
            mv = is_up_solidf(Ef[e][0], Ef[e][1], Ef[e][3], 0);
            if (mv < Ef[e][3])                                  // allowed move less than requested move
            {
               Ef[e][1] -= mv;                                  // move the allowed amount
               mode = 6;                                          // next mode (ceil right) (cw)
               Ef[e][14] = deg_to_rad(135);
               if (dp) printf("hit ceil during this move - mv%1.2f \n", mv);
               break;
            }
            Ef[e][1] -= mv;  // if neither of these matched, move the allowed amount
         break;
         case 6:                                                  // ceil right
            Ei[e][2] = 0;                                        // h flip
            Ef [e][14] = deg_to_rad(180);
            if (dp) { if (is_up_solidf(Ef[e][0], Ef[e][1], 1, 0) < 1) printf(" - 6 with solid ceil\n"); else  printf(" - 6 with NOT solid ceil\n"); }
            mv = is_up_solidf(Ef[e][0], Ef[e][1], Ef[e][2], 1);
            if (mv < Ef[e][2])                                  // allowed move less than requested move
            {
               Ef[e][0] += mv;                                  // move the allowed amount
               mode = 5;                                          // next mode (lwall up ) (ccw)
               Ef[e][14] = deg_to_rad(135);
               Ef[e][1] -= 1;                           // move up 1 to catch new wall
               if (dp) printf("lost ceil during this move - mv%1.2f \n", mv);
               break;
            }
            mv = is_right_solidf(Ef[e][0], Ef[e][1], Ef[e][2], 0);
            if (mv < Ef[e][2])                                  // allowed move less than requested move
            {
               Ef[e][0] += mv;                                  // move the allowed amount
               mode = 7;                                          // next mode (rwall down) (cw)
               Ef[e][14] = deg_to_rad(225);
               if (dp) printf("hit rwall during this move - mv%1.2f \n", mv);
               break;
            }
            Ef[e][0] += mv;  // if neither of these matched, move the allowed amount
         break;
         case 7:                                                  // rwall down
            Ei[e][2] = 0;                                        // h flip
            Ef[e][14] = deg_to_rad(270);
            if (dp) { if (is_right_solidf(Ef[e][0], Ef[e][1], 1, 0) < 1) printf(" - 7 with solid rwall\n"); else  printf(" - 7 with NOT solid rwall\n"); }
            mv = is_right_solidf(Ef[e][0], Ef[e][1], Ef[e][3], 1);
            if (mv < Ef[e][3])                                  // allowed move less than requested move
            {
               Ef[e][1] += mv;                                  // move the allowed amount
               mode = 6;                                          // next mode (ceil right) (ccw)
               Ef[e][14] = deg_to_rad(225);
               Ef[e][0] += 1;                           // move right 1 to catch new wall
               if (dp) printf("lost rwall during this move - mv%1.2f \n", mv);
               break;
            }
            mv = is_down_solidf(Ef[e][0], Ef[e][1], Ef[e][3], 0);
            if (mv < Ef[e][3])                                  // allowed move less than requested move
            {
               Ef[e][1] += mv;                                  // move the allowed amount
               mode = 4;                                          // next mode (floor left) (cw)
               Ef[e][14] = deg_to_rad(315);
               if (dp) printf("hit floor during this move - mv%1.2f \n", mv);
               break;
            }
            Ef[e][1] += mv;  // if neither of these matched, move the allowed amount
         break;
      } // end of switch mode

      if (shot_request)
      {
         int p = find_closest_player_trakbot(e);
         if (p != -1)
         {
            mwS.fire_enemy_shota(e, 20, p);
            Ei[e][16] = Ei[e][15]; // set new prox wait
         }
      }

   }  // end of if !fall


   if (Ei[e][5] == mode) Ei[e][8] = 0;
   else
   {
      Ei[e][5] = mode;
      if (++Ei[e][8] > 3) // twirling
      {
         Ei[e][8] = 0; // reset twirling counter
         if (dp) printf("twirling detected!! - set fall mode\n");
         Ei[e][4] = 5; // start fall mode
         set_trakbot_mode(e, 0);
      }
   }


   // set the bitmap and drawing mode
   int b = Ei[e][3];    // ans
   if ((mode == 0) || (mode == 6)) // x+ floor right and ceil right
   {
      int ex = Ef[e][0];
      int s = ex % 6;
      Ei[e][1] = mwB.zz[10-s][b];
   }
   if ((mode == 1) || (mode == 5)) // y- rwall up and lwall up
   {
      int ey = Ef[e][1];
      int s = ey % 6;
      Ei[e][1] = mwB.zz[5+s][b];
   }
   if ((mode == 2) || (mode == 4)) // x- ceil left or floor left
   {
      int ex = Ef[e][0];
      int s = ex % 6;
      Ei[e][1] = mwB.zz[5+s][b];
   }
   if ((mode == 3) || (mode == 7)) // y+ lwall down or rwall down
   {
      int ey = Ef[e][1];
      int s = ey % 6;
      Ei[e][1] = mwB.zz[10-s][b];
   }
}

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
void enemy_podzilla(int e)
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

      // trigger box offset
      int x1 = Ei[e][11] - 10;
      int y1 = Ei[e][12] - 10;
      int x2 = x1 + Ei[e][13]+20;
      int y2 = y1 + Ei[e][14]+20;

      for (int p=0; p<NUM_PLAYERS; p++)
         if ((players[p].active) && (!players[p].paused))
         {
            int px = players[p].x;
            int py = players[p].y;
            if ((px > x1) && (px < x2) && (py > y1) &&  (py < y2))
            {
               Ei[e][5] = 1;   // set next mode
            }
         }
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
         mwS.fire_enemy_shota(e, 54, p);
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


void enemy_vinepod(int e)
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

      // trigger box offset
      int x1 = Ei[e][11] - 10;
      int y1 = Ei[e][12] - 10;
      int x2 = x1 + Ei[e][13]+20;
      int y2 = y1 + Ei[e][14]+20;

      for (int p=0; p<NUM_PLAYERS; p++)
         if ((players[p].active) && (!players[p].paused))
         {
            int px = players[p].x;
            int py = players[p].y;
            if ((px > x1) && (px < x2) && (py > y1) &&  (py < y2))
            {
               Ei[e][15] = 1;   // set next mode
            }
         }
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
         mwS.fire_enemy_shota(e, 54, p);
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
























void bouncer_cannon_common(int e)
{
   if ((Ei[e][8]) && (Ei[e][7] > Ei[e][8])) // seek
   {
      Ei[e][7] = 0;
      seek_set_xyinc(e);
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
void enemy_cannon(int e)
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
   bouncer_cannon_common(e);
}

//--4--bouncer-----------------------------------------------------------------------------

void enemy_bouncer(int e)
{
   if (Ei[e][31]) // hit
   {
      enemy_killed(e);
      return; // to stop rest of execution
   }
   enemy_player_hit_proc(e);

   bouncer_cannon_common(e);

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







//-------------------------------------------------------------------------------------------
//--3--archwagon-----------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------
/*
// see walker_archwagon_common

Ei[][3]   ans (2=wagon with arrow, 3=empty wagon)
Ei[][15]  shot retrigger value
Ei[][16]  shot retrigger count
Ei[][17]  shot prox

Ef[][7]  shot speed

*/


void enemy_archwagon(int e)
{
   int EXint = Ef[e][0];
   int EYint = Ef[e][1];

   if (Ei[e][31]) // hit
   {
      enemy_killed(e);
      return; // break;  to stop rest of execution
   }
   enemy_player_hit_proc(e);

   if (Ei[e][16] > 0) // shoot arrow holdoff
   {
      Ei[e][3] = 3; // empty wagon ans
      --Ei[e][16];
   }
   else
   {
      int swl = 10; // shot prox window low
      int swh = 6;  // shot prox window high
      Ei[e][3] = 2; // wagon with arrow ans



      if (Ei[e][2]) // attempt shoot right
         for (int p=0; p<NUM_PLAYERS; p++)
            if ((players[p].active) && (!players[p].paused) )
               if ((EXint > players[p].x - Ei[e][17]) && (EXint < players[p].x) )
                  if ((EYint > players[p].y - swh) && (EYint < players[p].y + swl ))
                  {
                     mwS.fire_enemy_x_shot(e, p);
                     Ei[e][16] = Ei[e][15]; // set new prox wait
                  }

      if (!Ei[e][2]) // attempt shoot left
         for (int p=0; p<NUM_PLAYERS; p++)
            if ((players[p].active) && (!players[p].paused) )
               if ((EXint > players[p].x ) && (EXint < players[p].x + Ei[e][17]))
                  if ((EYint > players[p].y  - swh) && (EYint < players[p].y + swl ))
                  {
                     mwS.fire_enemy_x_shot(e, p);
                     Ei[e][16] = Ei[e][15]; // set new prox wait
                  }
   }

   walker_archwagon_common(e);

   // set the bitmap and drawing mode
   int b = Ei[e][3]; // ans
   int c = mwB.zz[4][b]; // num_of_shapes in seq

   // animate with h move
   if (Ei[e][2] == 1) // right
   {
      int x = (EXint/3) % c;
      Ei[e][1] = mwB.zz[x+5][b];
   }
   if (Ei[e][2] == 0) // left
   {
      int x = (EXint/3) % c;
      Ei[e][1] = mwB.zz[5+c-x][b];
   }
}


//-------------------------------------------------------------------------------------------
//--archwagon and block walker common -------------------------------------------------------
//-------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------
/*

Ei[][2]   direction (0=left, 1=right)
Ei[][5]   jump/fall -160 max jump, 160 max fall
Ei[][6]   jump wait (0=none)
Ei[][7]   jump when player above
Ei[][8]   follow(0) or bounce(1)
Ei[][10]  turn before hole
Ei[][11]  jump before hole
Ei[][12]  jump before wall

Ef[][2]  y speed
Ef[][6]  x speed

*/


void walker_archwagon_common(int e)
{
   int EXint = Ef[e][0];
   int EYint = Ef[e][1];

   int on_solid = 0;
   int on_lift = 0;
   int ret = is_down_solid(EXint, EYint, 1, 2);
   if ((ret == 1) || (ret == 2)) on_solid = 1;
   if (ret >= 32) on_lift = 1;



   // check if stuck
   int EXintR = Ef[e][0] + Ef[e][6];
   int EXintL = Ef[e][0] - Ef[e][6];

   if ((is_left_solid(EXintL, EYint, 1, 2)) && (is_right_solid(EXintR, EYint, 1, 2))) // stuck
   {
      int p = find_closest_player(e);
      if (EXint < players[p].x-5) Ei[e][2] = 1;
      if (EXint > players[p].x+5) Ei[e][2] = 0;
   }
   else
   {
      if (Ei[e][2] == 1)  // move right
      {
         int change_dir = 0;
         Ef[e][2] = Ef[e][6];
         Ef[e][0] += Ef[e][2];
         EXint= Ef[e][0];
         if ((on_solid) || (on_lift))
         {
            if (Ei[e][10]) // turn before hole
              if (!is_right_solid(EXint+Ei[e][10]-10, EYint+20, 1, 2)) change_dir = 1;
            if (Ei[e][12]) // jump before wall
              if (is_right_solid(EXint+Ei[e][12], EYint, 1, 2)) Ei[e][5] = -160;
            if (Ei[e][11]) // jump before hole
               if (!is_right_solid(EXint+Ei[e][11]-18, EYint+20, 1, 2)) Ei[e][5] = -160;
         }
         if ((is_right_solid(EXint, EYint, 1, 2)) || (change_dir))
         {
            Ei[e][2] = 0; // change direction;
            Ef[e][0] -= Ef[e][2]; // take back last move
            EXint= Ef[e][0];
         }
      }
      else if (Ei[e][2] == 0)  // move left
      {
         int change_dir = 0;
         Ef[e][2] = -Ef[e][6];
         Ef[e][0] += Ef[e][2];
         EXint = Ef[e][0];
         if ((on_solid) || (on_lift))
         {
            if (Ei[e][10]) // turn before hole
              if (!is_left_solid(EXint-Ei[e][10]+10, EYint+20, 1, 2)) change_dir = 1;
            if (Ei[e][12]) // jump before wall
               if (is_left_solid(EXint-Ei[e][12], EYint, 1, 2)) Ei[e][5] = -160;
            if (Ei[e][11]) // jump before hole
               if (!is_left_solid(EXint-Ei[e][11]+18, EYint+20, 1, 2)) Ei[e][5] = -160;
         }
         if ((is_left_solid(EXint, EYint, 1, 2)) || (change_dir))
         {
            Ef[e][0] -= Ef[e][2]; // take back last move
            Ei[e][2] = 1; // change direction;
            EXint = Ef[e][0];
         }
      }

      if (!Ei[e][8]) // follow mode
      {
         int p = find_closest_player(e);
         if (EXint < players[p].x-5) Ei[e][2] = 1;
         if (EXint > players[p].x+5) Ei[e][2] = 0;
      }
   }


   if ((on_solid) && (Ei[e][5] >= 0)) // solid and not jumping (falling or steady)
   {
      Ef[e][1] -= (int)(Ef[e][1]) % 20;    // align with floor
      Ef[e][1] =  (int)(Ef[e][1]);  // remove decimal
      Ei[e][5] = 0;
   }



   if ((!on_solid) && (!on_lift) && (Ei[e][5] >= 0)) // not solid and falling
   {
      Ei[e][5] +=5; // gravity
      if (Ei[e][5] > 160) Ei[e][5] = 160; // terminal velocity

      // apply y move
      float ym = Ei[e][5] * Ef[e][3];
      float ym1 = ym/100;

      Ef[e][1] += ym1;

      EYint = Ef[e][1];
      if (is_down_solid(EXint, EYint, 1, 2))
      {
         on_solid = 1;
         Ef[e][1] -= (int) (Ef[e][1]) % 20;  // align with floor
         Ef[e][1] =  (int) (Ef[e][1]);  // remove decimal
         Ei[e][5] = 0;
      }
   }

   if ((!on_solid) && (!on_lift) && (Ei[e][5] == 0)) // not solid and falling
      Ei[e][5] +=5; // start falling



   if ((Ei[e][5] < 0) && (!on_lift))  // rising or jumping
   {
      Ei[e][5] +=5; // gravity
      if (Ei[e][5] < -160) Ei[e][5] = -160; // terminal velocity

      // apply y move
      float ym = Ei[e][5] * Ef[e][3];
      float ym1 = ym/100;
      Ef[e][1] += ym1;

      EYint = Ef[e][1];
      if ((is_up_solid(EXint, EYint, 1, 2) == 1) || (is_up_solid(EXint, EYint, 1, 2) > 31) )
         Ei[e][5] = 0;  // stop rising
   }

   if (on_lift)
   {
      Ef[e][1] += lifts[ret-32].yinc;  // move with lift
   }


   if ((on_solid) || (on_lift))
   {
      // frame_num jump
      if ((Ei[e][6] > 0) && ((mwPS.frame_num % Ei[e][6]) == 1)) Ei[e][5] = -160;

      // check for jump if player passes above
      if (Ei[e][7] > 0)
         for (int p=0; p<NUM_PLAYERS; p++)
            if ((players[p].active) && (!players[p].paused) )
               if ((EXint < players[p].x + Ei[e][7]) && (EXint > players[p].x - Ei[e][7]) &&
                   (EYint > players[p].y)) Ei[e][5] = -160;
   }

}


//-------------------------------------------------------------------------------------------
//--11--block walker ------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------
// see walker_archwagon_common


void enemy_block_walker(int e)
{
   int EXint = Ef[e][0];
   int EYint = Ef[e][1];

   enemy_player_hit_proc(e);



   if (Ei[e][31]) // hit
   {
      int ex = EXint/20;
      int ey = EYint/20;

      l[ex][ey] = 168 | PM_BTILE_ALL_SOLID;

      l[ex][ey] |= PM_BTILE_BREAKABLE_PSHOT;

      al_set_target_bitmap(mwB.level_background);
      al_draw_filled_rectangle(ex*20, ey*20, ex*20+20, ey*20+20, mC.pc[0]);
      al_draw_bitmap(mwB.btile[168], ex*20, ey*20, 0);

      game_event(60, 0, 0, Ei[e][26], e, 0, 0);


      Ei[e][0] = 0;
      return; // to stop rest of execution
   }


   walker_archwagon_common(e);

   // set the bitmap and drawing mode
   int b = Ei[e][3];     // ans
   int c = mwB.zz[4][b];     // num_of_shapes in seq

   // animate with h move
   int x = (EXint/2) % c;
   Ei[e][1] = mwB.zz[x+5][b];
}


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



void enemy_jumpworm(int e)
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
      Ef[e][1] += lifts[ret-32].yinc;  // move with lift
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

/*


-------------------------------------------------------------------------
-------------------------------------------------------------------------
common variables (ones that can't be used by specific types)
by function
-------------------------------------------------------------------------
-------------------------------------------------------------------------

-------------------------------------------------------------------------
-- Variables used in draw
-------------------------------------------------------------------------
Ei[][1] // bitmap
Ei[][2] // draw mode (v and h flips)

Ef[][0]  // x
Ef[][1]  // y
Ef[][12] // scale
Ef[][14] // rot

-------------------------------------------------------------------------
-- Variables used in proc_enemy_collision_with_pshot(int e)
-------------------------------------------------------------------------
Ei[][26]  // player number that hit enemy with shot
Ei[][29]  // enemies collison box size
Ei[][31]  // flag that this enemy got shot with shot

Ef[][0] // x
Ef[][1] // y

-------------------------------------------------------------------------
-- Variables used in move_enemies() common
-------------------------------------------------------------------------
Ei[e][27] // time to live
Ei[e][29] // collision box

Ef[e][4] // life dec

-------------------------------------------------------------------------
-- Variables used in enemy_deathcount(int e)
-------------------------------------------------------------------------
Ei[][1]  // draw bitmap
Ei[][3]  // deathcount ans
Ei[][24] // health bonus shape
Ei[][25] // health bonus amount
Ei[][30] // death loop count

Ef[][11]  // scale inc
Ef[][12]  // scale
Ef[][13]  // rot inc
Ef[][14]  // rot


-------------------------------------------------------------------------
-- Variables used in enemy_player_hit_proc(int e)
-------------------------------------------------------------------------
Ei[][22] // player hit flag and player num + 1
Ei[][23] // hit player retrigger

Ef[][4] // life dec

-------------------------------------------------------------------------
-- Variables used in enemy_killed(int e)
-------------------------------------------------------------------------
Ei[][3]  // new ans

Ei[][24] // health bonus shape
Ei[][25] // health bonus amount
Ei[][26] // player num that killed enemy
Ei[][30] // death loop count
Ei[][31] // hit type (shot or bomb)


Ef[][4]  // life dec
Ef[][11] // scale multiplier
Ef[][13] // rot inc

-------------------------------------------------------------------------
-------------------------------------------------------------------------
common variables (ones that can't be used by specific types)
total list
-------------------------------------------------------------------------
-------------------------------------------------------------------------
Ei[][0] = enemy type
Ei[][1] = bitmap
Ei[][2] = draw mode

Ei[][22] = player hit
Ei[][23] = player hit retrigger
Ei[][24] = helath bonus shape
Ei[][25] = health bonus amount
Ei[][26] = used to tell what player killed enemy
Ei[][27] = time to live
Ei[][28] = cloner create id
Ei[][29] = collision box size
Ei[][30] = death loop count
Ei[][31] = flag that this enemy got shot with shot


Ef[][0] =  x
Ef[][1] =  y
Ef[][2] =  xinc
Ef[][3] =  yinc
Ef[][4] =  LIFE decrement


Ef[][11] = scale multiplier
Ef[][12] = scale;
Ef[][13] = rot inc
Ef[][14] = rot



// enemy types
----------------------------------
3  - Archwagon
4  - Bouncer
5  - Jumpworm
6  - Cannon
7  - Podzilla
8  - Trakbot
9  - Cloner
11 - Block Walker
12 - Flapper
13 - Vinepod



similar types
3  - Archwagon
11 - Block Walker

4 - Bouncer
6 - Cannon

7  - Podzilla
13 - Vinepod






[3] - Archwagon -----------------------------------------------------------------------------
Ei[][3]   ans (2=wagon with arrow, 3=empty wagon)
Ei[][15]  shot retrigger value
Ei[][16]  shot retrigger count
Ei[][17]  shot prox

Ef[][2]  y speed speed
Ef[][6]  x speed speed
Ef[][7]  shot speed

walker_archwagon_common(int e)
Ei[][5]   jump/fall -160 max jump, 160 max fall
Ei[][6]   jump wait (0=none)
Ei[][7]   jump when player above
Ei[][8]   follow(0) or bounce(1)
Ei[][11]  jump before hole
Ei[][12]  jump before wall




[4]--bouncer-----------------------------------------------------------------------------
Ei[][5]  main ans
Ei[][6]  seek ans
Ei[][7]  seek counter
Ei[][8]  seek count
Ef[][5] seek speed

[6]--cannon-----------------------------------------------------------------------------
Ei[][7]   seek counter
Ei[][8]   seek count
Ei[][9]   extra hits to kill
Ei[][15]  shot retrigger value
Ei[][16]  shot retrigger counter
Ei[][17]  shot prox

Ef[][5]  seek speed
Ef[][7]  shot speed
Ef[][12] scale
Ef[][14] rot

[7]--podzilla-----------------------------------------------------------------------------
Ei[][5] = mode
Ei[][6] = sequence counter
Ei[][7] = sequence limit
Ei[][8] = wait count
Ei[][9] = wait limit
Ei[][11] = trigger box x1
Ei[][12] = trigger box x1
Ei[][13] = trigger box x2
Ei[][14] = trigger box y2

[8]--trakbot-----------------------------------------------------------------------------
Ei[][3]  base animation seq (17=trakbot with shot, 18=trakbot without shot)
Ei[][4]  fall 160 max fall
Ei[][5]  mode
Ei[][7]  drop mode(0=no, 1=yes)
Ei[][8]  twirling counter
Ei[][15] shoot holdoff value
Ei[][16] shoot holdoff counter
Ei[][17] shot prox
Ei[][20] rot
Ef[][7] shot speed

[9]--cloner-----------------------------------------------------------------------------

Ei[][4]  draw boxes (0 = none) (1 = trigger) (2 = source/dest) (3 = both)
Ei[][5]  mode
Ei[][6]  create wait
Ei[][7]  create wait counter
Ei[][8]  trigger mode (0=wait, 1=reset, 2=immed)
Ei[][9]  time to live for created objects
Ei[][10] max num of created objects
Ei[][11] trigger box x1
Ei[][12] trigger box y1
Ei[][13] trigger box x2
Ei[][14] trigger box y2
Ei[][15] copy box x
Ei[][16] copy box y
Ei[][17] dest box x
Ei[][18] dest box y
Ei[][19] copy box width
Ei[][20] copy box height




[11]--block walker-----------------------------------------------------------------------------
walker_archwagon_common(int e)
Ei[][5]   jump/fall -160 max jump, 160 max fall
Ei[][6]   jump wait (0=none)
Ei[][7]   jump when player above
Ei[][8]   follow(0) or bounce(1)
Ei[][11]  jump before hole
Ei[][12]  jump before wall


[12]--flapper-----------------------------------------------------------------------------
Ei[][14] base yo for debug drawing
Ei[][15] shot retrigger time
Ei[][16] shot retrigger counter
Ei[][17] prox width
Ei[][18] prox height
Ei[][19] prox depth

Ei[][20] height above player
Ei[][21] flap height

Ef[][5] max x speed
Ef[][6] x accel
Ef[][7] shot speed


Ef[][8] flap offset for next loop
Ef[][9] flap speed counter
Ef[][10] flap speed inc

*/
