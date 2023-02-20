// z_item_bomb.cpp

#include "pm.h"
#include "z_item_bomb.h"
#include "z_item.h"
#include "z_sound.h"
#include "z_player.h"
#include "mwBitmap.h"
#include "mwColor.h"
#include "mwProgramState.h"
#include "z_enemy.h"
#include "mwLevel.h"

#include "z_screen_overlay.h"
#include "z_solid.h"
#include "mwFont.h"


void proc_bomb_collision(int p, int i)
{
   item[i][0] = 99; // change to lit bomb
   item[i][13] = p; // mark player that lit bomb
   if (item[i][12] == 0) // fuse timer
   {
      item[i][6] = 1;  // mode == lit
      item[i][8] = item[i][9]; // fuse wait count
      game_event(24, 0, 0, p, item[i][7]/20, item[i][9]/40, 0);
   }
   if (item[i][12] == 1) // remote detonator
   {
      item[i][6] = 3;
      game_event(25, 0, 0, p, item[i][7]/20, 0, 0);
   }
}

void proc_rocket_collision(int p, int i)
{
   item[i][0] = 98;   // new type - lit rocket
   item[i][13] = p;   // mark player that lit rocket
   item[i][1] = 1026; // new ans
   if ((item[i][3] == 0) || (item[i][3] == 1)) item[i][3] = -1;  // if stat or fall set to carryable
   itemf[i][3] = 0;   // stop if falling
   game_event(26, 0, 0, p, i, 0, 0);
}



void proc_lit_rocket(int i)
{
   lit_item = 1;

   int accel = item[i][9];
   int max_speed = item[i][8]*1000;
   item[i][11] += accel;                                  // add accel to speed
   if (item[i][11] > max_speed) item[i][11] = max_speed;  // enforce max speed

   float angle = ((float) item[i][10] / 1000)-ALLEGRO_PI/2;
   itemf[i][2] = (cos(angle) * item[i][11]) / 1000;
   itemf[i][3] = (sin(angle) * item[i][11]) / 1000;

   itemf[i][0] += itemf[i][2]; // apply the increments
   itemf[i][1] += itemf[i][3];


   // check for wall collisions
   int x = itemf[i][0];
   int y = itemf[i][1];
   if ( ((itemf[i][3]<0)    && (is_up_solid(   x, y, 0, 3) == 1)) ||
        ((itemf[i][3]>0)    && (is_down_solid( x, y, 0, 3) == 1)) ||
        ((itemf[i][3]>0)    && (is_down_solid( x, y, 0, 3) == 2)) ||
        ((itemf[i][2]<-1.1) && (is_left_solid( x, y, 0, 3) == 1)) ||
        ((itemf[i][2]>1.1)  && (is_right_solid(x, y, 0, 3) == 1)) )
   {

      // stop movement
      itemf[i][2] = 0;
      itemf[i][3] = 0;

      item[i][0] = 99;   // change to lit bomb
      item[i][6] = 2;    // mode 2; explosion

      item[i][3] = 1;   // not carryable (fall)

      item[i][8] = 20;   // explosion timer count
      item[i][9] = 20;   // explosion timer limit

      // if any players are riding this rocket, make them drop it
      for (int p=0; p<NUM_PLAYERS; p++)
         if ( (players[p].active) && (!players[p].paused) && (players[p].carry_item) && (players[p].carry_item == i+1 )) players[p].carry_item = 0;

   }
   else
   {
      // if any players are riding this rocket, bind them to rocket's position
      for (int p=0; p<NUM_PLAYERS; p++)
         if ( (players[p].active) && (!players[p].paused) && (players[p].carry_item == i+1 ))
         {
            players[p].x = itemf[i][0];
            players[p].y = itemf[i][1];
         }
   }
}

int seq_color(int mod, int c1, int c2)
{
   int col = c1; // initial color
   if ( (mwPS.frame_num % mod) < mod/2) col = c2; // other color
   return col;
}

int seq_color2(void)
{
   int ca[40];
   int ci = 0;

   for (ci = 0; ci<40; ci++) ca[ci] = 0;

   ci = 0;
   int b = 10;
   ca[ci] = b+192; ci++;
   ca[ci] = b+160; ci++;
   ca[ci] = b+128; ci++;
   ca[ci] = b+96; ci++;
   ca[ci] = b+64; ci++;
   ca[ci] = b+32; ci++;
   ca[ci] = b; ci++;
   ca[ci] = b; ci++;
   ca[ci] = b; ci++;
   ca[ci] = b; ci++;
   ca[ci] = b+32; ci++;
   ca[ci] = b+64; ci++;
   ca[ci] = b+96; ci++;
   ca[ci] = b+128; ci++;
   ca[ci] = b+160; ci++;
   ca[ci] = b+192; ci++;


   ci = 20;
   b = 14;
   ca[ci] = b+192; ci++;
   ca[ci] = b+160; ci++;
   ca[ci] = b+128; ci++;
   ca[ci] = b+96; ci++;
   ca[ci] = b+64; ci++;
   ca[ci] = b+32; ci++;
   ca[ci] = b; ci++;
   ca[ci] = b; ci++;
   ca[ci] = b; ci++;
   ca[ci] = b; ci++;
   ca[ci] = b+32; ci++;
   ca[ci] = b+64; ci++;
   ca[ci] = b+96; ci++;
   ca[ci] = b+128; ci++;
   ca[ci] = b+160; ci++;
   ca[ci] = b+192; ci++;




   int mod = mwPS.frame_num % 40;
   return ca[mod];
}

int seq_color3(void)
{
   int ca[20];
   int ci = 0;

   for (ci = 0; ci<20; ci++) ca[ci] = 0;

   ci = 0;
   int b = 10;
   ca[ci] = b+192; ci++;
   ca[ci] = b+160; ci++;
   ca[ci] = b+128; ci++;
   ca[ci] = b+96; ci++;
   ca[ci] = b+64; ci++;
   ca[ci] = b+32; ci++;
   ca[ci] = b; ci++;
   ca[ci] = b; ci++;
   ca[ci] = b; ci++;
   ca[ci] = b; ci++;
   ca[ci] = b+32; ci++;
   ca[ci] = b+64; ci++;
   ca[ci] = b+96; ci++;
   ca[ci] = b+128; ci++;
   ca[ci] = b+160; ci++;
   ca[ci] = b+192; ci++;

   int mod = mwPS.frame_num % 20;
   return ca[mod];
}

void bomb_block_crosshairs(int e, int f)
{
   //int col = seq_color(16, 14, 10);
   int col = seq_color2();
   if (col)
   {
      al_draw_rectangle(e*20, f*20, e*20+20, f*20+20, mC.pc[col], 1);
      al_draw_line(e*20, f*20, e*20+19, f*20+19, mC.pc[col], 1);
      al_draw_line(e*20, f*20+19, e*20+19, f*20, mC.pc[col], 1);
   }
}

void bomb_blocks(int i, int t, int dr, float x, float y)
{
   x+=10;
   y+=10;
   float d = (float)dr;
   // convert to 0-100 range to cycle block rect
   int bx = (int)(x / 20);
   int by = (int)(y / 20);
   int bd = (int)(dr / 20);  // damage range
   for (int e = (bx-bd); e < (bx+bd)+1; e++)        // cycle blast range of blocks
      for (int f = (by-bd); f < (by+bd)+1; f++)
      {
         if ((e) && (e < 100) && (f) && (f < 100))
         {
            // get center of block
            float cx = (float)(e*20+10);
            float cy = (float)(f*20+10);

            // check radius from center
            float xd = abs(x-cx);
            float yd = abs(y-cy);
            float br = sqrt(xd*xd+yd*yd);
            if ((br < d) && (mLevel.l[e][f] & PM_BTILE_BOMBABLE))
            {
               if (t == 1) bomb_block_crosshairs(e, f);
               if (t == 2) mLevel.change_block(e, f, 0);
            }
         }
      }
}

void bomb_crosshairs(float x, float y)
{
   int col = seq_color3();
   float rad = 12;   // circle radius


//   // radius seq
//   int ms = 8;  // min size
//   int ns = 16; // seq length
//   int sq = mwPS.frame_num % ns;
//   if (sq < ns/2) rad = ms+sq;
//   else rad = ms+ns-sq;

   if (col)
   {
      // circle with a slash
      al_draw_circle(x, y, rad, mC.pc[col], 1);
      float h = sqrt ((rad * rad) / 2) + 0;
      al_draw_line(x-h, y-h, x+h, y+h, mC.pc[col], 1);
//      // circle with crosshairs
//      al_draw_circle(x, y, rad, mC.pc[col], 1);
//      float h = sqrt ((rad * rad) / 2) + 6;
//      al_draw_line(x-h, y-h, x+h, y+h, mC.pc[col], 1);
//      al_draw_line(x-h, y+h, x+h, y-h, mC.pc[col], 1);
   }
}

void bomb_enemies(int i, int t, int dr, float x, float y)
{
   for (int e=0; e<100 ; e++) // enemies in damage window?
      if (Ei[e][0])
      {
         float dist = sqrt(pow(Ef[e][0]-x, 2) + pow(Ef[e][1]-y, 2));
         if (dist < dr)
         {
            if (t == 1) bomb_crosshairs(Ef[e][0]+10, Ef[e][1]+10);
            if (t == 2)
            {
               Ei[e][31] = 2; // set bomb hit
               Ei[e][26] = item[i][13];  // player that did bomb
            }
         }
      }
}

void bomb_players(int i, int t, int dr, float x, float y)
{
   for (int p=0; p<NUM_PLAYERS; p++)
   {
      if ((players[p].active) && (!players[p].paused))
      {
         float dist = sqrt( pow( (players[p].x - x), 2) + pow( (players[p].y - y), 2)   );
         if (dist < dr)
         {
            // calculate bomb damage based on blast size and player's distance
            float damage = dr / dist; // damage window size / distance (scale of 1 - 4)
            damage *= 20; // multiply by 20 (scale of 20-80)
            if (damage > 80) damage = 80;
            int dmg = damage;
            if (t == 1) // add potential bomb damage for display
            {
               players1[p].potential_bomb_damage += dmg;
               players1[p].health_display = 40;
               players1[p].last_health_adjust = 0;
               bomb_crosshairs(players[p].x + 10, players[p].y + 10);
            }
            if ((t == 2) && (item[i][8] == 0)) // only do damage once at end of explosion seq
            {
               players[p].health -= damage;
               int p2 = item[i][13]; // player that last touched bomb
               if (p == p2) game_event(53, 0, 0, p, 0, 0, dmg);
               else game_event(52, 0, 0, p, p2, 0, dmg);
            }
         }
      }
   }
}

void proc_lit_bomb(int i)
{
   item[i][8]--; // timer dec

   if (item[i][6] == 3) // remote detonator
   {
      if (players[item[i][13]].down)
      {
         item[i][6] = 2; // mode 2; explosion
         item[i][8] = item[i][9] = 20; // explosion timer
      }
   }

   if (item[i][6] == 1) // fuse burning
   {
      lit_item = 1;
      if (item[i][8] < 1) // fuse done
      {
         item[i][6] = 2; // mode 2; explosion
         item[i][8] = item[i][9] = 20; // explosion timer

         // force player to drop item
         for (int p=0; p<NUM_PLAYERS; p++)
            if ((players[p].active) && (players[p].carry_item-1 == i)) player_drop_item(p);

         // check for other co-located bombs
         for (int ii=0; ii<500; ii++)
            if (item[ii][0] == 99) // lit bomb
               if (ii != i) // not this one
                  if (itemf[ii][0] == itemf[i][0]) // compare x
                     if (itemf[ii][1] == itemf[i][1]) // compare y
                        if (item[ii][15] == item[i][15]) // check if created by same cloner
                           item[ii][0] = 0; // delete item
      }
   }
   if (item[i][6] == 2) // explosion
   {
      float r = 1 - (float)item[i][8] / (float)item[i][9]; // ratio
      int dr = (int) (  (float)item[i][7] * r); // damage range scaled by ratio

      // do damage
      bomb_blocks( i, 2, dr, itemf[i][0], itemf[i][1]);
      bomb_enemies(i, 2, dr, itemf[i][0], itemf[i][1]);
      bomb_players(i, 2, dr, itemf[i][0], itemf[i][1]);

      if (item[i][8] == 16) game_event(22,0,0,0,0,0,0); // explosion sound

      if (item[i][8] < 1) item[i][0] = 0; // explosion done, erase item
   }
}

void draw_lit_bomb(int i)
{
   int x = itemf[i][0];
   int y = itemf[i][1];
   int fuse_seq = 96;
   int expl_seq = 97;
   float r = 1 - (float)item[i][8] / (float)item[i][9]; // countdown timer ratio (0 - 1)

   if (item[i][6] == 3) // remote detonator
   {
      // draw bomb
      al_draw_bitmap(mwB.tile[538], x, y, 0);

      // get detonator position relative to player
      int p = item[i][13];
      int xo = players[p].left_right*28-14;
      int px = players[p].x + xo;
      int py = players[p].y + -1;

      // draw detonator
      al_draw_bitmap(mwB.tile[539], px, py, 0);

      // draw a line connecting detonator and bomb
      al_draw_line(x+10, y+10, px+10, py+8, mC.pc[207], 0);

      // show damage range circle
      int col = seq_color2();
      if (col) al_draw_circle(x+10, y+10, item[i][7], mC.pc[col], 2);
   }


   if (item[i][6] == 1) // fuse burning
   {
      int num_seq_shapes = mwB.zz[4][fuse_seq] + 1;   // number of shapes in seq
      int si = (int)(r * (float)num_seq_shapes);  // ratio * number of shapes
      int shape = mwB.zz[5+si][fuse_seq];             // get shape to draw
      //printf("ratio:%f shape_index:%d\n", r, si);
      al_draw_bitmap(mwB.tile[shape], x, y, 0);

      // show countdown clock
      float cr = 6; // clock radius
      float pi = ALLEGRO_PI;
      float rd = pi*2*r - pi/2; // countdown position in radians
      al_draw_circle(x+10, y+13, cr, mC.pc[15], 0);
      al_draw_pieslice(x+10, y+13, cr, rd, 0, mC.pc[15], 0);
      al_draw_arc(x+10, y+13, cr, -pi/2, pi*2*r, mC.pc[10], 0);

      // show damage range circle
      int col = seq_color2();
      if (col) al_draw_circle(x+10, y+10, item[i][7], mC.pc[col], 2);

   }

   if (item[i][6] == 2) // explosion mode
   {
      int num_seq_shapes = mwB.zz[4][expl_seq] + 1;   // number of shapes in seq
      int si = (int)(r * (float)num_seq_shapes);  // ratio * number of shapes
      int shape = mwB.zz[5+si][expl_seq];             // get shape to draw

      // set size of explosion
      float max_scale = item[i][7] / 10;         // max scale = damage / 10
      float sc = .5 + (r) * max_scale;
      //printf("ratio:%f shape_index:%d scale:%f \n", r, si, sc);
      al_draw_scaled_rotated_bitmap(mwB.tile[shape], 10, 10, x+10, y+10, sc, sc, 0, 0);

      // show damage range circle
      al_draw_circle(x+10, y+10, (float)item[i][7]*r-8, mC.pc[10], 1);
      al_draw_circle(x+10, y+10, (float)item[i][7]*r, mC.pc[14], 2);
      al_draw_circle(x+10, y+10, (float)item[i][7]*r+8, mC.pc[10], 1);
   }

   bomb_blocks( i, 1, item[i][7], itemf[i][0], itemf[i][1]); // mark blocks that will be destroyed
   bomb_enemies(i, 1, item[i][7], itemf[i][0], itemf[i][1]); // mark enemies
   bomb_players(i, 1, item[i][7], itemf[i][0], itemf[i][1]); // mark players in damage range

   // debug show sequence numbers
   //al_draw_textf(mF.pr8, mC.pc[15], x, y-20, 0, "%d / %d  %f ", item[i][8], item[i][9], (float)item[i][8]/(float)item[i][9]);
}

void draw_rocket_lines(int i)
{
   float fxi = itemf[i][0]; // initial position
   float fyi = itemf[i][1];

   float fx = fxi; // path variables
   float fy = fyi;

   float speed = item[i][11];
   if (speed < 2000) speed = 2000; // if moving too slow, lines won't be drawn

   float angle = ((float) item[i][10] / 1000 ) - ALLEGRO_PI/2;
   float fxinc = (cos(angle) * item[i][11]) / 1000;
   float fyinc = (sin(angle) * item[i][11]) / 1000;

   for (int j=0; j<1000; j++)
   {
      float x = fx += fxinc; // apply the increments
      float y = fy += fyinc;

      if ((x < 0) || (x > 2000) || (y < 0) || (y > 2000)) j = 1000; // level bounds check

      // check for wall collisions
      if ( ((fyinc < 0)    && (is_up_solid(   x, y, 0, 3) == 1)) ||
           ((fyinc > 0)    && (is_down_solid( x, y, 0, 3) == 1)) ||
           ((fyinc > 0)    && (is_down_solid( x, y, 0, 3) == 2)) ||
           ((fxinc < -1.1) && (is_left_solid( x, y, 0, 3) == 1)) ||
           ((fxinc > 1.1)  && (is_right_solid(x, y, 0, 3) == 1)) )
      {

         float fxf =  fx+10; // offset floats for display purposes
         float fyf =  fy+10;
         float fxif = fxi+10;
         float fyif = fyi+10;

         int col = seq_color2(); // color sequence
         if (col) al_draw_circle(fxf, fyf, item[i][7], mC.pc[col], 2); // show damage range circle
         al_draw_filled_circle(fxf, fyf, 5, mC.pc[col]); // show center
         al_draw_line(fxif, fyif, fxf, fyf, mC.pc[10+80], 0); // draw connecting line

         bomb_blocks( i, 1, item[i][7], x, y); // mark blocks that will be destroyed
         bomb_enemies(i, 1, item[i][7], x, y); // mark enemies that will be destroyed
         bomb_players(i, 1, item[i][7], x, y); // mark players in damage range

         j = 1000; // to break out of loop
      }
   }
}



