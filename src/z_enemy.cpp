// zemove.cpp

#include "pm.h"

int enemy_data(int x_pos, int y_pos)
{
   sort_enemy();
   al_draw_textf(font, palette_color[4], x_pos, y_pos, 0, "%-2d Enemies", num_enemy); y_pos += 8;
   al_draw_text(font, palette_color[4], x_pos, y_pos,  0, "----------"); y_pos += 8;
   for (int c=1; c<16; c++)
   {
      if (e_num_of_type[c]) // not zero
      {
         al_draw_textf(font, palette_color[10], x_pos, y_pos,   0, "%-2d %s", e_num_of_type[c], enemy_name[c]);
         y_pos += 8;
      }
   }
   return y_pos;
}

void get_enemy_draw_shape(int e)
{
   int flags = 0;
   if (Ei[e][2] == 0) flags = ALLEGRO_FLIP_HORIZONTAL;
   if (Ei[e][2] == 1) flags = 0;
   if (Ei[e][2] == 2) flags = ALLEGRO_FLIP_VERTICAL;
   if (Ei[e][2] == 3) flags = ALLEGRO_FLIP_VERTICAL | ALLEGRO_FLIP_HORIZONTAL;

   al_set_target_bitmap(dtemp);
   al_clear_to_color(palette_color[0]);
   float rot = al_fixtof(al_fixmul(Efi[e][14], al_fixtorad_r));
   al_draw_rotated_bitmap(tile[Ei[e][1]], 10, 10, 10, 10, rot, flags);
//   if (Ei[e][0] == 8) // show trakbot mode
//      al_draw_textf(font, palette_color[10], 10, 6, ALLEGRO_ALIGN_CENTER, "%d" ,Ei[e][5] );
}


void rectangle_with_diagonal_lines(float x1, float y1, float x2, float y2, int spacing, int frame_color, int line_color)
{
   al_set_clipping_rectangle(x1-1, y1-1, x2-x1+1, y2-y1+1);

   // find largest dimension
   float xd = x2-x1;
   float yd = y2-y1;
   float ld = xd;
   if (yd > ld) ld = yd;

   for (float k=-ld; k<ld; k+=spacing)
      al_draw_line(x1+k, y1-k, x1+ld+k, y1+ld-k, palette_color[line_color], 0);

   al_draw_rectangle(x1, y1, x2, y2, palette_color[frame_color], 1);

   al_reset_clipping_rectangle();
}



void draw_enemies(void)
{
   al_set_target_bitmap(level_buffer);
   for (int e=0; e<100; e++)
      if (Ei[e][0])  // if enemy active
      {
         int EXint = al_fixtoi(Efi[e][0]);
         int EYint = al_fixtoi(Efi[e][1]);

         int flags = 0;
         if (Ei[e][2] == 0) flags = ALLEGRO_FLIP_HORIZONTAL;
         if (Ei[e][2] == 1) flags = 0;
         if (Ei[e][2] == 2) flags = ALLEGRO_FLIP_VERTICAL;
         if (Ei[e][2] == 3) flags = ALLEGRO_FLIP_VERTICAL & ALLEGRO_FLIP_HORIZONTAL;

         float rot = al_fixtof(al_fixmul(Efi[e][14], al_fixtorad_r));
         float sc = al_fixtof(Efi[e][12]);
         al_draw_scaled_rotated_bitmap(tile[Ei[e][1]], 10, 10, EXint+10, EYint+10, sc, sc, rot, flags);



         #ifdef SHOW_POD_CLONER_TRIGGER_BOX
         if ((Ei[e][0] == 7) || (Ei[e][0] == 9))// show podzilla or cloner trigger box
         {
            int x1 = Ei[e][11] * 20;
            int y1 = Ei[e][12] * 20;
            int x2 = Ei[e][13] * 20 + 20;
            int y2 = Ei[e][14] * 20 + 20;
            al_draw_rectangle(x1, y1, x2, y2, palette_color[14], 1);
         }
         #endif

         #ifdef SHOW_CANNON_COLLISION_BOX
         if (Ei[e][0] == 6) // cannon
         {
            // draw some test rects here
            int cbs = Ei[e][29]; // collision box size

            // enemy position (add 10 to get the center)
            int x1 = al_fixtoi(Efi[e][0]) + 10;
            int y1 = al_fixtoi(Efi[e][1]) + 10;

            int il = cbs;
            int color = 11;
            //  printf("x1:%d  y1:%d il:%d  color:%d\n",x1, y1, il, color);

            // check for collision with player
            al_fixed b = al_itofix(Ei[e][29]); // collision box size
            for (int p=0; p<NUM_PLAYERS; p++)
               if ((players[p].active) && (!players[p].paused))
               {
                  al_fixed px = players[p].PX;
                  al_fixed py = players[p].PY;

                  al_fixed ex1 = Efi[e][0] - b;
                  al_fixed ex2 = Efi[e][0] + b;
                  al_fixed ey1 = Efi[e][1] - b;
                  al_fixed ey2 = Efi[e][1] + b;

                  // if player in collision box color = red
                  if ((px > ex1) && (px < ex2) && (py > ey1) && (py < ey2)) color = 10;
            }
            al_draw_rectangle(x1-il, y1-il, x1+il, y1+il, palette_color[color], 1);
         }
         #endif

         if (Ei[e][0] == 9) // cloner
         {
            // trigger box
            float tx1 = (float)Ei[e][11]*20;
            float ty1 = (float)Ei[e][12]*20;
            float tx2 = (float)Ei[e][13]*20+20;
            float ty2 = (float)Ei[e][14]*20+20;
            int tc1 = 14 + 128; // trigger box color

            // source
            float sx1 = (float)Ei[e][15]*20;
            float sy1 = (float)Ei[e][16]*20;
            float sx2 = sx1 + (float)Ei[e][19]*20;
            float sy2 = sy1 + (float)Ei[e][20]*20;
            int sc1 = 11 + 128; // source box color

            // destination
            float dx1 = (float)Ei[e][17]*20;
            float dy1 = (float)Ei[e][18]*20;
            float dx2 = dx1 + (float)Ei[e][19]*20;
            float dy2 = dy1 + (float)Ei[e][20]*20;
            int dc1 = 10 + 128; // destination box color

            int m = Ei[e][5]; // 2 - 9  total seq (8)

            if ((m > 1) && (m < 6)) // first half (2 - 5) // flash source box green
            {
               int co, d = m-2; // 0 to 3
               if (d == 0) co = 64;
               if (d == 1) co = 0;
               if (d == 2) co = 0;
               if (d == 3) co = 64;
               sc1 = 11 + co; // source box color
            }
            if ((m > 5) && (m < 10)) // second half (6 - 9) // flash destination box red
            {
               int co, d =  m-6; // 0 to 3
               if (d == 0) co = 64;
               if (d == 1) co = 0;
               if (d == 2) co = 0;
               if (d == 3) co = 64;
               dc1 = 10 + co; // destination box color
            }
            if (Ei[e][5] != 0) // in trigger box
            {
               tc1 = 14 + 32; // trigger box color

               // show vertical red green bar animation sequence
               int b = (Ei[e][7] * 10) / (Ei[e][6]+1);
               int t = zz[5+b][53];
               al_draw_scaled_rotated_bitmap(tile[t], 10, 10, EXint+10, EYint+10, .5, .5, 0, ALLEGRO_FLIP_VERTICAL);
            }

            // show box mode (0=none) (1=trig only) (2=src/dst only) (3=all)
            int q = Ei[e][4];
            if ((q == 1) || (q == 3))
               rectangle_with_diagonal_lines(tx1, ty1, tx2, ty2, 8, tc1, tc1+64); // trigger box
            if ((q == 2) || (q == 3))
            {
               rectangle_with_diagonal_lines(sx1, sy1, sx2, sy2, 8, sc1, sc1+64); // source
               rectangle_with_diagonal_lines(dx1, dy1, dx2, dy2, 8, dc1, dc1+64); // destination
            }
         }





         #ifdef SHOW_FLAPPER_DEBUG
         if (Ei[e][0] == 12) // flapper
         {

           // draw flap height
            int base =  Ei[e][14]+10;
            int y1 = base - Ei[e][21];
            int y2 = base + Ei[e][21];

            al_draw_line(EXint-140, base, EXint+140, base, palette_color[11], 1);
            al_draw_rectangle(EXint-40, y1, EXint+40, y2, palette_color[11], 1);

            // draw height above player
            int hab =  Ei[e][20];
            al_draw_line(EXint-40, base+hab, EXint+40, base+hab, palette_color[12], 1);

//            // draw trigger box
//            int width =  Ei[e][17];
//            int height = Ei[e][18];
//            int depth =  Ei[e][19];
//            al_draw_rectangle(EXint-width+10, EYint-height, EXint+width+10, EYint+depth, palette_color[14], 1);

         }
         #endif


         #ifdef SHOW_TRAKBOT_BULLET_TRIGGER_CIRCLE
         if (Ei[e][0] == 8) // trakbot
         {
            int prox = Ei[e][17];
            int color = 14; // default circle color

            // is any player in range?
            int mode = Ei[e][5];
            int quad;
            switch (mode)
            {
               case 0: case 5: quad = 1; break; // floor right, lwall up
               case 1: case 4: quad = 2; break; // rwall up floor left
               case 2: case 7: quad = 3; break; // ceil left, rwall down
               case 3: case 6: quad = 4; break; // lwall down, ceil right
            }
            int p = find_closest_player_quad(e, quad, prox);
            if (p != -1)
            {
               color = 10; // change circle color to red
               // mark player with blue crosshairs
               int px = al_fixtoi(players[p].PX)+10;
               int py = al_fixtoi(players[p].PY)+10;
               al_draw_line(px-20, py, px+20, py, palette_color[13], 1);
               al_draw_line(px, py-20, px, py+20, palette_color[13], 1);
            }
            al_draw_circle(EXint+10, EYint+10, prox, palette_color[color], 1);
         }
         #endif



      } // end of if enemy active
}

void proc_enemy_collision_with_pbullet(int e)
{
   int ex = al_fixtoi(Efi[e][0]);
   int ey = al_fixtoi(Efi[e][1]);

   for (int c=0; c<50; c++)
      if (pbullet[c][0])
      {
         // bullet collision box size adjusted with bullet speed
         int cx = pm_bullet_collision_box + abs(pbullet[c][4]/2);
         int cy = pm_bullet_collision_box + abs(pbullet[c][5]/2);

         // bullet collision box size adjusted with enemies collison box size
         cx += Ei[e][29] - 10;
         cy += Ei[e][29] - 10;

         int bx1 = pbullet[c][2] - cx;
         int bx2 = pbullet[c][2] + cx;
         int by1 = pbullet[c][3] - cy;
         int by2 = pbullet[c][3] + cy;

         // check for collision with player's bullets

         if ((ex > bx1) && (ex < bx2) && (ey > by1) && (ey < by2))
         {
            int p = pbullet[c][1];   // player number that shot bullet
            Ei[e][31] = 1;           // flag that this enemy got shot with bullet
            Ei[e][26] = p;           // number of player's bullet that hit enemy
            players[p].num_hits++;   // add to number of hits the player has
            pbullet[c][0] = 0;       // bullet dies
         }
      }
}

void move_enemies()
{
   num_enemy = 0; // count enemies
   for (int e=0; e<100; e++)
      if (Ei[e][0])
      {
         num_enemy++; // count enemies
         if (Ei[e][0] < 50) proc_enemy_collision_with_pbullet(e);

         // check for time to live
         int ttl = Ei[e][27];
         if (ttl)
         {
            if (ttl < 11)
            {
               Ei[e][0] = 66;             // change to different type to prevent use
               Efi[e][4] = al_itofix(0);  // cant hurt anymore
               Ei[e][29] = 0;             // no collision box
               int sq = 10-ttl;
               Ei[e][1] = zz[5+sq][74];
            }
            if (ttl == 1) Ei[e][0] = 0; // kill instantly
            Ei[e][27]--;
         }

         // check for out of bounds
         if ((Efi[e][0] < al_itofix(0)) || (Efi[e][0] > al_itofix(1999))) Ei[e][0]=0;
         if ((Efi[e][1] < al_itofix(0)) || (Efi[e][1] > al_itofix(1999))) Ei[e][0]=0;

         switch (Ei[e][0])
         {
            case 3:   enemy_archwagon(e);  break;
            case 4:   enemy_bouncer(e);  break;
            case 6:   enemy_cannon(e);  break;
            case 7:   enemy_podzilla(e);  break;
            case 8:   enemy_trakbot(e);  break;
            case 9:   enemy_cloner(e);  break;
            case 11:  enemy_block_walker(e);  break;
            case 12:  enemy_flapper(e);  break;
            case 99:  enemy_deathcount(e); break;
         }
      }
}

void enemy_deathcount(int e)
{
   int EXint = al_fixtoi(Efi[e][0]);
   int EYint = al_fixtoi(Efi[e][1]);
   Efi[e][14] += Efi[e][13]; // rot inc
   Efi[e][12] = al_fixmul(Efi[e][11], Efi[e][12]); // scale inc

   Ei[e][1] = zz[0][ Ei[e][3] ]; // draw current ans shape
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
            item[c][7] = Ei[e][25];  //  life
            item[c][8] = 0;

            item[c][14] = 800; // time to live

            itemf[c][0] = al_itofix(item[c][4]);
            itemf[c][1] = al_itofix(item[c][5]);
            itemf[c][2]= al_itofix(0);
            itemf[c][3]= al_itofix(0);
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
         players[p].LIFE -= Efi[e][4];

         game_event(44, 0, 0, p, e, 0, al_fixtoi(Efi[e][4]));
         Ei[e][22] = 0;  // clear hit
         Ei[e][23] = 60; // set retrigger amount
      }
   }
  else Ei[e][22] = 0;
}

void enemy_killed(int e)
{
   int na, dl;
   int ht = Ei[e][31]; // hit type

   // almost all do this but not enough to do by default
   int a = Ei[e][0];
   if (a==3 || a==4 || a==6 || a==7 || a==8 || a==9 || a==12 )
   {
      Efi[e][4] = al_itofix(0);  // cant hurt anymore
      Ei[e][25]*=ht; // health bonus
   }

   switch (Ei[e][0])
   {
      case 3: // archwagon
         na = Ei[e][3] = 34;  // new ans
         dl = Ei[e][30] = 20; // death_loop_wait; set delay
         Ei[e][24] = 929+(ht-1)*32; // shape
         Efi[e][11] = al_ftofix(1.08); // scale multiplier
         Efi[e][13] = al_itofix(0);// 255/dl/2; rot inc

         zz[0][na] = zz[5][na]; // set shape
         zz[1][na] = 0;         // point to zero
         zz[2][na] = frame_num; // set counter
         zz[3][na] = dl / (zz[4][na]+1); // set ans timer
      break;
      case 4:
         na = Ei[e][3]; //new ans
         dl = Ei[e][30] = 20; // death_loop_wait;  set delay
         Ei[e][24] = 928+(ht-1)*32; // shape
         Efi[e][11] = al_ftofix(1.03); // scale multiplier
         Efi[e][13] = al_itofix(2); // 255/dl/2;  rot inc

         if (na == 29) na = 46;
         if (na == 14) na = 13;
         Ei[e][3] = na; // new ans

         zz[0][na] = zz[5][na]; // set shape
         zz[1][na] = 0;         // point to zero
         zz[2][na] = frame_num; // set counter
         zz[3][na] = dl / (zz[4][na]+1); // set ans timer
      break;
      case 6:
         na = Ei[e][3] = 37;  // new ans
         dl = Ei[e][30] = 20; // death_loop_wait; set delay
         Ei[e][24] = 930+(ht-1)*32; // shape
         Efi[e][11] = al_ftofix(1.08); // scale multiplier
         Efi[e][13] = al_itofix(1); // 255/dl/2;  rot inc

         zz[0][na] = zz[5][na]; // set shape
         zz[1][na] = 0;         // point to zero
         zz[2][na] = frame_num; // set counter
         zz[3][na] = dl / (zz[4][na]+1); // set ans timer
      break;
      case 7:
         na = Ei[e][3] = 45;  // new ans
         dl = Ei[e][30] = 40; // death_loop_wait; set delay
         Ei[e][24] = 932+(ht-1)*32; // shape
         Efi[e][12] = al_ftofix(2.8); // initial scale
         Efi[e][11] = al_ftofix(.94); // scale multiplier
         Efi[e][13] = al_itofix(0);  // 255/dl/2;  rot inc

         zz[0][na] = zz[5][na]; // set shape
         zz[1][na] = 0;         // point to zero
         zz[2][na] = frame_num; // set counter
         zz[3][na] = dl / (zz[4][na]+1); // set ans timer
      break;
      case 8: // trakbot
         na = Ei[e][3] = 44;  // new ans
         dl = Ei[e][30] = 12; // death_loop_wait; set delay
         Ei[e][24] = 931+(ht-1)*32; // shape
         Efi[e][11] = al_ftofix(1.00); // scale multiplier
         Efi[e][13] = al_ftofix(255/dl*3/4); // rot inc

         zz[0][na] = zz[5][na]; // set shape
         zz[1][na] = 0;         // point to zero
         zz[2][na] = frame_num; // set counter
         zz[3][na] = dl / (zz[4][na]+1); // set ans timer
      break;
      case 9: // cloner
         na = Ei[e][3] = 105;  // new ans
         dl = Ei[e][30] = 40; // death_loop_wait; set delay
         Ei[e][24] = 934+(ht-1)*32; // shape
         Efi[e][11] = al_ftofix(.98); // scale multiplier
         Efi[e][13] = al_ftofix(306/dl); // rot inc

         zz[0][na] = zz[5][na]; // set shape
         zz[1][na] = 0;         // point to zero
         zz[2][na] = frame_num; // set counter
         zz[3][na] = dl / zz[4][na]; // set ans timer
      break;
      case 11: // none for block walker
      break;
      case 12: // flapper
         na = Ei[e][3] = 63;  // new ans
         dl = Ei[e][30] = 20; // death_loop_wait; set delay
         Ei[e][24] = 933+(ht-1)*32; // shape
         Efi[e][11] = al_ftofix(1.04); // scale multiplier
         Efi[e][13] = al_itofix(0); // rot inc

         zz[0][na] = zz[5][na]; // set shape
         zz[1][na] = 0;         // point to zero
         zz[2][na] = frame_num; // set counter
         zz[3][na] = dl / zz[4][na]; // set ans timer
      break;

   } // end of switch

   // almost all do this but not enough to do by default
   if (a==3 || a==4 || a==6 || a==7 || a==8 || a==9 || a==12 )
   {

      if (ht == 1) game_event(60, 0, 0, Ei[e][26], e, 0, 0);
      if (ht == 2) game_event(62, 0, 0, Ei[e][26], e, 0, 0);
      Ei[e][0] = 99; // set type to death loop
   }
}


//--12--flapper-----------------------------------------------------------------------------

//   Ei[][0] = type 12 "flapper"
//   Ei[][1] = draw bitmap
//   Ei[][2] = hflip - right(1) or left(0)
//   Ei[][3] = base animation sequence

//   Ei[][14] = base ypo for debug drawing

//   Ei[][15] = bullet retrigger time
//   Ei[][16] = bullet retrigger counter

//   Ei[][17] = prox width
//   Ei[][18] = prox height
//   Ei[][19] = prox depth

//   Ei[][20] = height above player
//   Ei[][21] = flap height

//   Ei[][30] = collision box
//   Ei[][31] = health bonus

//   Efi[][0]  = x pos
//   Efi[][1]  = y pos
//   Efi[][2]  = x inc
//   Efi[][3]  = y inc
//   Efi[][4]  = health decrement
//   Efi[][5]  = max x speed
//   Efi[][6]  = x accel
//   Efi[][7]  = bullet speed

//   Efi[][8]  = flap offset for next loop
//   Efi[][9]  = flap speed counter
//   Efi[][10] = flap speed inc


void enemy_flapper(int e)
{
   al_fixed f0 = al_itofix(0);
   al_fixed f100 = al_itofix(100);

   if (Ei[e][31]) // hit
   {
      enemy_killed(e);
      return;
   }
   enemy_player_hit_proc(e);

   int p = -1; // default if no player found in trigger box

   // show if empty or has bullet ready
   if (Ei[e][16] < 0) Ei[e][3] = 61;
   else Ei[e][3] = 60;


   // ------------ x move ---------------
   if (Ei[e][2])  // move right
   {
      Efi[e][2] += Efi[e][6];                           // accel
      if (Efi[e][2] > Efi[e][5]) Efi[e][2] = Efi[e][5]; // max speed
      Efi[e][0] += Efi[e][2];                           // apply xinc

      if (is_right_solid(al_fixtoi(Efi[e][0]), al_fixtoi(Efi[e][1]), 1))
      {
         Efi[e][0] -= Efi[e][2];    // take back last move
         Efi[e][2] = -Efi[e][5]/3;  // set accel to bounce back with 1/3 max accel
         Ei[e][2] = 0;              // change direction
      }
      // try to shoot right
      p = find_closest_player_flapper(e, 1);
      if (--Ei[e][16] < 0)
      {
         if (p != -1)
         {
            fire_enemy_bulleta(e, 62, p);
            Ei[e][16] = Ei[e][15]; // set new prox wait
         }
      }
   }
   else  // move left
   {
      Efi[e][2] -= Efi[e][6];                             // accel
      if (Efi[e][2] < -Efi[e][5]) Efi[e][2] = -Efi[e][5]; // max speed
      Efi[e][0] += Efi[e][2];                             // apply xinc

      if (is_left_solid(al_fixtoi(Efi[e][0]), al_fixtoi(Efi[e][1]), 1))
      {
         Efi[e][0] -= Efi[e][2];   // take back last move
         Efi[e][2] = Efi[e][5]/3;  // set accel to bounce back with 1/3 max accel
         Ei[e][2] = 1;             // change direction
      }
      // try to shoot left
      p = find_closest_player_flapper(e, 0);
      if (--Ei[e][16] < 0)
      {
         if (p != -1)
         {
            fire_enemy_bulleta(e, 62, p);
            Ei[e][16] = Ei[e][15]; // set new prox wait
         }
       }
   }

   // ------------ y seek  ---------------
   al_fixed seek_yinc = f0;  // yinc for this pass

   // (comment out this line to only seek if player in trigger box)
   p = find_closest_player(e); // always seek closest player in y axis

   if (p != -1) // only seek in y axis if valid player in prox
   {
      al_fixed rat = al_itofix(1); // default scaling ratio for seek_yinc
      al_fixed h = al_itofix(Ei[e][20]); // height_above_player
      // difference between actual y and desired y
      al_fixed df = Efi[e][1] - (players[p].PY - h);
      if (df < f0)
      {
         if (df > -f100) rat = al_fixdiv(-df, f100);
         seek_yinc += al_fixmul(Efi[e][3], rat);
      }
      if (df > f0)
      {
         if (df < f100) rat = al_fixdiv(df, f100);
         seek_yinc -= al_fixmul(Efi[e][3], rat);
      }
   }

   // ------------ y flap  ---------------
   /* flap sequence
      Efi[][9]  - counter   goes from 0 to 100
      Efi[][10] - counter increment (flap speed)
      Ei[][21]  - flap height     */

   float f = al_fixtof(Efi[e][9]);

   // shape offset
   int so = (int)(f/16.66); // 100/6 = 16.66
   Ei[e][1] = zz[8+so][Ei[e][3]];

   //convert 0-100 to 0-256 to get fixed binary angle
   al_fixed th = al_ftofix(f * 2.56);

   // get sin ratio and scale with flap height
   al_fixed yo = al_fixsin(th) * Ei[e][21];

   // find difference from last flap offset
   al_fixed flap_yinc = yo - Efi[e][8];
   Efi[e][8] = yo; // save this flap offset for next time

   // set base ypos for debug drawing
   Ei[e][14] = al_fixtoi(Efi[e][1] - yo);

   // inc flap counter and roll over
   Efi[e][9] += Efi[e][10];
   if (Efi[e][9] >= f100) Efi[e][9] = f0;

//   printf("f:%f y:%f sy:%f fy:%f\n", f, al_fixtof(Efi[e][1]), al_fixtof(seek_yinc), al_fixtof(flap_yinc));

   // combine the 2 yincs
   al_fixed yinc = flap_yinc + seek_yinc;

   // check for floor or ceiling collisions
   if (yinc < f0) // moving up
   {
      yinc = is_up_solidfm(Efi[e][0], Efi[e][1], -yinc, 0);
      Efi[e][1] -= yinc; // apply allowed move
   }
   else if (yinc > f0) // moving down
   {
      yinc = is_down_solidfm(Efi[e][0], Efi[e][1], yinc, 0);
      Efi[e][1] += yinc; // apply allowed move
   }
}


void walker_archwagon_common(int e)
{
//Ei[][5]   jump/fall -160 max jump, 160 max fall
//Ei[][6]   jump wait (0=none)
//Ei[][7]   jump when player above
//Ei[][8]   follow(0) or bounce(1)
//Ei[][11]  jump before hole
//Ei[][12]  jump before wall

   int EXint = al_fixtoi(Efi[e][0]);
   int EYint = al_fixtoi(Efi[e][1]);

   if (!Ei[e][8]) // follow mode
   {
      int p = find_closest_player(e);
      if (EXint < al_fixtoi(players[p].PX)) Efi[e][2] = Efi[e][6];
      if (EXint > al_fixtoi(players[p].PX)) Efi[e][2] = -Efi[e][6];
   }

   int on_solid = 0;
   int on_lift = 0;
   int ret = is_down_solid(EXint, EYint, 1);
   if ((ret == 1) || (ret == 2)) on_solid = 1;

   if (ret >= 32) //on lift
   {
      on_lift = 1;
      Efi[e][1] += lifts[ret-32].fyinc ;  // move with lift
   }

   if ((Efi[e][2]) > al_itofix(0))  // move right
   {
      Ei[e][2] = 1; // no h_flip
      Efi[e][0] += Efi[e][2];
      EXint= al_fixtoi(Efi[e][0]);
      if ((on_solid) || (on_lift))
      {
         if (Ei[e][12]) // jump before wall
           if (is_right_solid(EXint+Ei[e][12], EYint, 1)) Ei[e][5] = -160;
         if (Ei[e][11]) // jump before hole
            if (!is_right_solid(EXint+Ei[e][11]-18, EYint+20, 1)) Ei[e][5] = -160;
      }
      if (is_right_solid(EXint, EYint, 1))
      {
         Efi[e][0] -= Efi[e][2]; // take back last move
         if (Ei[e][8]) Efi[e][2] =- Efi[e][6]; // if bounce mode set negative xinc for left
         EXint= al_fixtoi(Efi[e][0]);
      }
   }

   if ((Efi[e][2]) < al_itofix(0))  // move left
   {
      Ei[e][2] = 0; // h_flip to face left
      Efi[e][0] += Efi[e][2];
      EXint= al_fixtoi(Efi[e][0]);
      if ((on_solid) || (on_lift))
      {
         if (Ei[e][12]) // jump before wall
            if (is_left_solid(EXint-Ei[e][12], EYint, 1)) Ei[e][5] = -160;
         if (Ei[e][11]) // jump before hole
            if (!is_left_solid(EXint-Ei[e][11]+18, EYint+20, 1)) Ei[e][5] = -160;
      }
      if (is_left_solid(EXint, EYint, 1))
      {
         Efi[e][0] -= Efi[e][2]; // take back last move
         if (Ei[e][8]) Efi[e][2] = Efi[e][6]; // if bounce mode set positive xinc for right
         EXint= al_fixtoi(Efi[e][0]);
      }
   }

   if ((on_solid) && (Ei[e][5] >= 0)) // solid and not jumping (falling or steady)
   {
      Efi[e][1] -= al_itofix ((al_fixtoi(Efi[e][1]) % 20));  // align with floor
      Efi[e][1] = al_itofix (al_fixtoi(Efi[e][1]));  // remove decimal
      Ei[e][5] = 0;
   }

   if ((!on_solid) && (!on_lift) && (Ei[e][5] >= 0)) // not solid and falling
   {
      Ei[e][5] +=5; // gravity
      if (Ei[e][5] > 160) Ei[e][5] = 160; // terminal velocity

      // apply y move
      al_fixed ym = Ei[e][5] * Efi[e][3];
      al_fixed ym1 = ym/100;

      Efi[e][1] += ym1;

      EYint = al_fixtoi(Efi[e][1]);
      if (is_down_solid(EXint, EYint, 1))
      {
         on_solid = 1;
         Efi[e][1] -= al_itofix ((al_fixtoi(Efi[e][1]) % 20));  // align with floor
         Efi[e][1] = al_itofix (al_fixtoi(Efi[e][1]));  // remove decimal
         Ei[e][5] = 0;
      }
   }

   if (Ei[e][5] < 0) // rising or jumping
   {
      Ei[e][5] +=5; // gravity
      if (Ei[e][5] < -160) Ei[e][5] = -160; // terminal velocity

      // apply y move
      al_fixed ym = Ei[e][5] * Efi[e][3];
      al_fixed ym1 = ym/100;
      Efi[e][1] += ym1;

      EYint = al_fixtoi(Efi[e][1]);
      if ((is_up_solid(EXint, EYint, 1) == 1) || (is_up_solid(EXint, EYint, 1) > 31) )
         Ei[e][5] = 0;  // stop rising
   }

   if ((on_solid) || (on_lift))
   {
      // frame_num jump
      if ((Ei[e][6] > 0) && ((frame_num % Ei[e][6]) == 1)) Ei[e][5] = -160;

      // check for jump if player passes above
      if (Ei[e][7] > 0)
         for (int p=0; p<NUM_PLAYERS; p++)
            if ((players[p].active) && (!players[p].paused) )
               if ((EXint < (al_fixtoi(players[p].PX) + Ei[e][7])) &&
                   (EXint > (al_fixtoi(players[p].PX) - Ei[e][7])) &&
                   (EYint > al_fixtoi(players[p].PY)))
                      Ei[e][5] = -160;
   }

}



//--11--block walker-----------------------------------------------------------------------------
//      Ei[e][6] = jump wait (0=none)
//      Ei[e][7] = jump when player above
//      Ei[e][8] = follow(0) or bounce(1)
//      Ei[e][11] = jump before hole
//      Ei[e][12] = jump before wall
//      Efi[e][8] = fall and fallcount
//      Efi[e][9] = jump and jumpcount */
//      Ei[e][18] = bullet trigger box y1
//      Ei[e][19] = bullet trigger box y2

void enemy_block_walker(int e)
{
   int EXint = al_fixtoi(Efi[e][0]);
   int EYint = al_fixtoi(Efi[e][1]);

   enemy_player_hit_proc(e);
   if (Ei[e][31]) // hit
   {
      int tx = EXint/20;
      int ty = EYint/20;

      l[tx][ty] = 168;
      al_set_target_bitmap(level_background);
      al_draw_filled_rectangle(tx*20, ty*20, tx*20+20, ty*20+20, palette_color[0]);
      al_draw_bitmap(tile[168], tx*20, ty*20, 0);

      game_event(60, 0, 0, e, Ei[e][26], 0, 0);
      Ei[e][0] = 0;
      return; // to stop rest of execution
   }

   walker_archwagon_common(e);

   // set the bitmap and drawing mode
   int b = Ei[e][3];     // ans
   int c = zz[4][b];     // num_of_shapes in seq

   // animate with h move
   int x = (EXint/2) % c;
   Ei[e][1] = zz[x+5][b];
}



int is_player_in_trigger_box(int x1, int y1, int x2, int y2)
{
   for (int p=0; p<NUM_PLAYERS; p++)
      if ((players[p].active) && (!players[p].paused))
      {
         int px = al_fixtoi(players[p].PX);
         int py = al_fixtoi(players[p].PY);
         if ((px > x1) && (px < x2) && (py > y1) && (py < y2)) return 1;
      }
   return 0;
}


//--9--cloner-----------------------------------------------------------------------------
//      Ei[e][5] = mode
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
   al_fixed x1 = al_itofix(Ei[e][15]*20-2);    // source
   al_fixed y1 = al_itofix(Ei[e][16]*20-2);
   al_fixed x3 = al_itofix(Ei[e][17]*20-2);    // destination
   al_fixed y3 = al_itofix(Ei[e][18]*20-2);
   al_fixed w =  al_itofix(Ei[e][19]*20);     // width
   al_fixed h =  al_itofix(Ei[e][20]*20);     // height
   al_fixed x2 = x1 + w;
   al_fixed y2 = y1 + h;

   // trigger box
   int x4 = Ei[e][11]*20 - 10;
   int y4 = Ei[e][12]*20 - 10;
   int x5 = Ei[e][13]*20 + 10;
   int y5 = Ei[e][14]*20 + 10;


   Ei[e][1] = zz[0][105];


   if (Ei[e][31]) // hit
   {
      enemy_killed(e);
      return;
   }
   enemy_player_hit_proc(e);

   // set draw shape
   Ei[e][2] = 0;  // flip mode
   int b = (Ei[e][7] * 7) / (Ei[e][6]+1);
   Ei[e][1] = zz[5+b][106];



   // modes
   // 0 = not in trigger box
   // 1 = in trigger box
   // > 1 in clone mode

   if (Ei[e][5] > 1) Ei[e][5]++; // move the count along if in clone mode

   if (Ei[e][5] > 10) // clone mode done; reset mode
   {
      Ei[e][7] = Ei[e][6]; // reset counter
      if (is_player_in_trigger_box(x4, y4, x5, y5)) Ei[e][5] = 1;
      else Ei[e][5] = 0;
   }

   int mode = Ei[e][5];

   if (mode == 1) // in trig box last time
   {
      if (is_player_in_trigger_box(x4, y4, x5, y5))
      {
         if (--Ei[e][7] == 0)  Ei[e][5] = 2; // time for create
      }
      else Ei[e][5] = 0; // mode 0 - not in trigger box
   }

   if (mode == 0) // not in trig box last time
   {
      if (is_player_in_trigger_box(x4, y4, x5, y5))
      {
         Ei[e][5] = 1; // set mode 1 - in box
         if (Ei[e][8] == 1) Ei[e][7] = Ei[e][6]; // reset timer when triggered
         if (Ei[e][8] == 2)                      // immediate clone when triggered
         {
            Ei[e][7] = 0; // set timer to 0 (for display)
            Ei[e][5] = 2; // set mode to clone
         }
      }
   }

   if (mode == 5)  // create
   {
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
         for (int b=0; b<100; b++)
            if (Ei[b][0])     // check for enemies in box
               if ((Efi[b][0] > x1) && (Efi[b][0] < x2) && (Efi[b][1] > y1) && (Efi[b][1] < y2) && (no < cl))
               {


                  // check if new position is empty
                  al_fixed new_x_pos = Efi[b][0] + x3 - x1;
                  al_fixed new_y_pos = Efi[b][1] + y3 - y1;
                  int nx = al_fixtoi(new_x_pos) / 20;
                  int ny = al_fixtoi(new_y_pos) / 20;
                  if (is_block_empty(nx, ny, 1, 0, 0)) // block only
                  {
                     for (int c=0; c<100; c++) // look for a place to put it
                        if (Ei[c][0] == 0)  // found empty
                        {
                           for (int y=0; y<32; y++)
                              Ei[c][y] = Ei[b][y];
                           for (int y=0; y<16; y++)
                              Efi[c][y] = Efi[b][y];
                           Efi[c][0] = new_x_pos;
                           Efi[c][1] = new_y_pos;
                           Ei[c][27] = Ei[e][9]; // set time to live
                           Ei[c][28] = 1000+e;   // tag with cloner item id


                           c = 100; // end loop
                           no++;    // one more object created
                        }
                  }
               }
         for (int b=0; b<500; b++)
            if (item[b][0])    //  check for items in box
               if ((itemf[b][0] > x1) && (itemf[b][0] < x2) && (itemf[b][1] > y1) && (itemf[b][1] < y2) && (no < cl))
               {
                  // check if new position is empty
                  al_fixed new_x_pos = itemf[b][0] + x3 - x1;
                  al_fixed new_y_pos = itemf[b][1] + y3 - y1;
                  int nx = al_fixtoi(new_x_pos) / 20;
                  int ny = al_fixtoi(new_y_pos) / 20;
                  if (is_block_empty(nx, ny, 1, 1, 0)) // block only
                  {
                     for (int c=0; c<500; c++)
                        if (item[c][0] == 0) // found empty
                        {
                           for (int y=0; y<16; y++)
                              item[c][y] = item[b][y];
                           itemf[c][0]= new_x_pos;
                           itemf[c][1]= new_y_pos;
                           itemf[c][2]= al_itofix(0);
                           itemf[c][3]= al_itofix(0);

                           // are we copying something that already has an expiry date?? if so leave it
                           if (item[b][14] == 0) item[c][14] = Ei[e][9]; // otherwise set time to live from cloner

                           item[c][15] = 1000+e;   // tag with cloner item id
                           c = 500; // end loop
                           no++;    // one more object created
                        }
                  }
               }
      }
   } // end of create
}


void set_trakbot_mode(int e, int mode)
{
   Ei[e][5] = mode;
   switch (mode)
   {
      case 0:                                                  // floor right
         Ei[e][2] = 1;                                        // no h flip
         Efi[e][14] = al_itofix(0);                              // rotation = 0 deg
      break;
      case 1:                                                  // rwall up
         Ei[e][2] = 1;                                        // no h flip
         Efi[e][14] = al_itofix(192);                            // rotation = 270 deg
      break;
      case 2:                                                  // ceil left
         Ei[e][2] = 1;                                        // no h flip
         Efi[e][14] = al_itofix(128);                            // 180 deg
      break;
      case 3:                                                  // lwall down
         Ei[e][2] = 1;                                        // no h flip
         Efi[e][14] = al_itofix(64);                             // 90 deg
      break;
      case 4:                                                  // floor left
         Ei[e][2] = 0;                                        // h flip
         Efi[e][14] = al_itofix(0);                              // rotation = 0 deg
      break;
      case 5:                                                  // lwall up
         Ei[e][2] = 0;                                        // h flip
         Efi[e][14] = al_itofix(64);                             // 90 deg
      break;
      case 6:                                                  // ceil right
         Ei[e][2] = 0;                                        // h flip
         Efi[e][14] = al_itofix(128);                            // 180 deg
      break;
      case 7:                                                  // rwall down
         Ei[e][2] = 0;                                        // h flip
         Efi[e][14] = al_itofix(192);                            // 270 deg
      break;
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

//     Efi[e][0] = x
//     Efi[e][1] = y
//     Efi[e][2] = xinc
//     Efi[e][3] = yinc
//     Efi[e][4] = LIFE decrement

//     Efi[e][7] = bullet speed

//     Efi[e][11] = scale multiplier
//     Efi[e][12] = scale;
//     Efi[e][13] = rot inc


void enemy_trakbot(int e)
{
   al_fixed mv;
   int dp = 0; // debug print
   int bullet_request = 0;
   int mode = Ei[e][5];
   int prox = Ei[e][17];

   if (Ei[e][31]) // hit
   {
      enemy_killed(e);
      return;
   }
   enemy_player_hit_proc(e);

   if (--Ei[e][16] < 0)  // bullet retrigger wait
   {
      bullet_request = 1; // try to shoot
      Ei[e][3] = 17;     // show trakbot with bullet
   }
   else Ei[e][3] = 18;   // show trakbot without bullet


   if (Ei[e][4] != 0)   // if falling
   {
      if (dp) printf("mode:fall  x:%1.2f  y:%1.2f\n", al_fixtof(Efi[e][0]), al_fixtof(Efi[e][1])); //   printf(" [%.2f] ", al_fixtof(speed));

      Ei[e][4] += 5;                                             // add acceleration to fsll
      if (Ei[e][4] > 160)  Ei[e][4] = 160;                      // terminal velocity
      al_fixed fall_yinc = (Efi[e][3] * Ei[e][4]) / 100;           // (fix * int) / int
      mv = is_down_solidfm(Efi[e][0], Efi[e][1], fall_yinc, 0); // will we hit floor during this move?
      if (mv < fall_yinc) Ei[e][4] = 0;                          // end fall if allowed move less than requested move
      Efi[e][1] += mv;                                           // move the allowed amount
      if (Efi[e][1] > al_itofix(1999)) Ei[e][0]=0;                 // kill if falls past bottom of level

      // seek player while falling
      int p = find_closest_player(e);
      al_fixed ex = Efi[e][0];
      al_fixed px = players[p].PX;

      if (px > ex) mode = 0;
      else mode = 4;
      set_trakbot_mode(e, mode);
   }
   else // not fall
   {
      if ((Ei[e][7]) && ((mode == 2) || (mode == 6))) // if drop mode on and trakbot on ceiling
      {
         al_fixed ex = Efi[e][0];
         al_fixed ey = Efi[e][1];
         al_fixed ew = al_itofix(15);
         for (int p=0; p<NUM_PLAYERS; p++) // if any player passes underneath
            if ((players[p].active) && (!players[p].paused))
            {
               al_fixed px = players[p].PX;
               al_fixed py = players[p].PY;
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
      if (dp) printf("mode:%d  x:%1.2f  y:%1.2f - ", mode, al_fixtof(Efi[e][0]), al_fixtof(Efi[e][1])); //   printf(" [%.2f] ", al_fixtof(speed));
      switch (mode)
      {
         case 0:                                                  // floor right
            Ei[e][2] = 1;                                        // no h flip
            Efi[e][14] = al_itofix(0);                              // rotation = 0 deg
            if (dp) { if (is_down_solidfm(Efi[e][0], Efi[e][1], al_itofix(1), 0) < al_itofix(1)) printf(" - 0 with solid floor\n"); else  printf(" - 0 with NOT solid floor\n"); }
            mv = is_down_solidfm(Efi[e][0], Efi[e][1], Efi[e][2], 1);
            if (mv < Efi[e][2]) // allowed move less than requested move
            {
               if (dp) printf("will lose floor during this move - mv%1.2f \n", al_fixtof(mv));
               Efi[e][0] += mv;                                  // move the allowed amount
               mode = 3;                                          // next mode (lwall down) (cw)
               Efi[e][14] = al_itofix(32);                          // 45 deg
               Efi[e][1] += al_itofix(1);                           // move down 1 to catch new wall
               break;
            }
            mv = is_right_solidfm(Efi[e][0], Efi[e][1], Efi[e][2], 0);
            if (mv < Efi[e][2]) // allowed move less than requested move
            {
               if (dp) printf("will hit wall during this move - mv%1.2f \n", al_fixtof(mv));
               Efi[e][0] += mv;                                  // move the allowed amount
               mode = 1;                                          // next mode (rwall up) (ccw)
               Efi[e][14] = al_itofix(224);                         // 315 deg
               break;
            }
            Efi[e][0] += mv;  // if neither of these matched, move the allowed amount
         break;
         case 1:                                                  // rwall up
            Ei[e][2] = 1;                                        // no h flip
            Efi[e][14] = al_itofix(192);                            // rotation = 270 deg
            if (dp) { if (is_right_solidfm(Efi[e][0], Efi[e][1], al_itofix(1), 0) < al_itofix(1)) printf(" - 1 with solid r wall\n"); else  printf(" - 1 with NOT solid r wall\n"); }
            mv = is_right_solidfm(Efi[e][0], Efi[e][1], Efi[e][3], -1);
            if (mv < Efi[e][3]) // allowed move less than requested move
            {
               if (dp) printf("will lose rwall during this move - mv%1.2f \n", al_fixtof(mv));
               Efi[e][1] -= mv;                                  // move the allowed amount
               mode = 0;                                          // next mode (floor right) (cw)
               Efi[e][14] = al_itofix(224);                         // 315 deg
               Efi[e][0] += al_itofix(1);                           // move right 1 to catch new wall
               break;
            }
            mv = is_up_solidfm(Efi[e][0], Efi[e][1], Efi[e][3], 0);
            if (mv < Efi[e][3]) // allowed move less than requested move
            {
               if (dp) printf("will hit ceiling during this move - mv%1.2f \n", al_fixtof(mv));
               mode = 2;                                          // next mode (ceil left) (ccw)
               Efi[e][14] = al_itofix(160);                         // 225 deg
               Efi[e][1] -= mv;                                  // move the allowed amount
               break;
            }
            Efi[e][1] -= mv;  // if neither of these matched, move the allowed amount
         break;
         case 2:                                                  // ceil left
            Ei[e][2] = 1;                                        // no h flip
            Efi[e][14] = al_itofix(128);                            // 180 deg
            if (dp) { if (is_up_solidfm(Efi[e][0], Efi[e][1], al_itofix(1), 0) < al_itofix(1)) printf(" - 2 with solid ceil\n"); else  printf(" - 2 with NOT solid ceil\n"); }
            mv = is_up_solidfm(Efi[e][0], Efi[e][1], Efi[e][2], -1);
            if (mv < Efi[e][2]) // allowed move less than requested move
            {
               if (dp) printf("will lose ceiling during this move - mv%1.2f \n", al_fixtof(mv));
               Efi[e][0] -= mv;                                  // move the allowed amount
               mode = 1;                                          // next mode (rwall up) (cw)
               Efi[e][14] = al_itofix(160);                         // 225 deg
               Efi[e][1] -= al_itofix(1);                           // move up 1 to catch new wall
               break;
            }
            mv = is_left_solidfm(Efi[e][0], Efi[e][1], Efi[e][2], 0);
            if (mv < Efi[e][2]) // allowed move less than requested move
            {
               if (dp) printf("will hit lwall during this move - mv%1.2f \n", al_fixtof(mv));
               mode = 3;                                          // next mode (lwall down) (ccw)
               Efi[e][14] = al_itofix(96);                          // 135 deg
               Efi[e][0] -= mv;                                  // move the allowed amount
               break;
            }
            Efi[e][0] -= mv;  // if neither of these matched, move the allowed amount
         break;
         case 3:                                                  // lwall down
            Ei[e][2] = 1;                                        // no h flip
            Efi[e][14] = al_itofix(64);                             // 90 deg
            if (dp) { if (is_left_solidfm(Efi[e][0], Efi[e][1], al_itofix(1), 0) < al_itofix(1)) printf(" - 3 with solid lwall\n"); else  printf(" - 3 with NOT solid lwall\n"); }
            mv = is_left_solidfm(Efi[e][0], Efi[e][1], Efi[e][3], 1);
            if (mv < Efi[e][3]) // allowed move less than requested move
            {
               if (dp) printf("will lose lwall during this move - mv%1.2f \n", al_fixtof(mv));
               Efi[e][1] += mv;                                  // move the allowed amount
               mode = 2;                                          // next mode  (ceil left) (cw)
               Efi[e][14] = al_itofix(96);                          // 135 deg
               Efi[e][0] -= al_itofix(1);                           // move left 1 to catch new wall
               break;
            }
            mv = is_down_solidfm(Efi[e][0], Efi[e][1], Efi[e][3], 0);
            if (mv < Efi[e][3]) // allowed move less than requested move
            {
               if (dp) printf("will hit floor during this move - mv%1.2f \n", al_fixtof(mv));
               mode = 0;                                          // next mode (floor right) (ccw)
               Efi[e][14] = al_itofix(32);                          // 45 deg
               Efi[e][1] += mv;                                  // move the allowed amount
               break;
            }
            Efi[e][1] += mv;  // if neither of these matched, move the allowed amount
         break;
         case 4:                                                  // floor left
            Ei[e][2] = 0;                                        // h flip
            Efi[e][14] = al_itofix(0);                              // rotation = 0 deg
            if (dp) { if (is_down_solidfm(Efi[e][0], Efi[e][1], al_itofix(1), 0) < al_itofix(1)) printf(" - 4 with solid floor\n"); else  printf(" - 4 with NOT solid floor\n"); }

            mv = is_down_solidfm(Efi[e][0], Efi[e][1], Efi[e][2], -1);
            if (mv < Efi[e][2])                                  // allowed move less than requested move
            {
               Efi[e][0] -= mv;                                  // move the allowed amount
               mode = 7;                                          // next mode (rwall down) (ccw)
               Efi[e][14] = al_itofix(224);                         // 315 deg
               Efi[e][1] += al_itofix(1);                           // move down 1 to catch new wall
               if (dp) printf("lost floor during this move - mv%1.2f \n", al_fixtof(mv));
               break;
            }
            mv = is_left_solidfm(Efi[e][0], Efi[e][1], Efi[e][2], 0);
            if (mv < Efi[e][2])                                  // allowed move less than requested move
            {
               Efi[e][0] -= mv;                                  // move the allowed amount
               mode = 5;                                          // next mode (lwall up) (cw)
               Efi[e][14] = al_itofix(32);                          // 45 deg
               if (dp) printf("hit lwall during this move - mv%1.2f \n", al_fixtof(mv));
               break;
            }
            Efi[e][0] -= mv;  // if neither of these matched, move the allowed amount
         break;
         case 5:                                                  // lwall up
            Ei[e][2] = 0;                                        // h flip
            Efi[e][14] = al_itofix(64);                             // 90 deg
            if (dp) { if (is_left_solidfm(Efi[e][0], Efi[e][1], al_itofix(1), 0) < al_itofix(1)) printf(" - 5 with solid left\n"); else  printf(" - 5 with NOT solid left\n"); }
            mv = is_left_solidfm(Efi[e][0], Efi[e][1], Efi[e][3], -1);
            if (mv < Efi[e][3])                                  // allowed move less than requested move
            {
               Efi[e][1] -= mv;                                  // move the allowed amount
               mode = 4;                                          // next mode (floor left) (ccw)
               Efi[e][14] = al_itofix(32);                          // 45 deg
               Efi[e][0] -= al_itofix(1);                           // move left 1 to catch new wall
               if (dp) printf("lost lwall during this move - mv%1.2f \n", al_fixtof(mv));
               break;
            }
            mv = is_up_solidfm(Efi[e][0], Efi[e][1], Efi[e][3], 0);
            if (mv < Efi[e][3])                                  // allowed move less than requested move
            {
               Efi[e][1] -= mv;                                  // move the allowed amount
               mode = 6;                                          // next mode (ceil right) (cw)
               Efi[e][14] = al_itofix(96);                          // 135 deg
               if (dp) printf("hit ceil during this move - mv%1.2f \n", al_fixtof(mv));
               break;
            }
            Efi[e][1] -= mv;  // if neither of these matched, move the allowed amount
         break;
         case 6:                                                  // ceil right
            Ei[e][2] = 0;                                        // h flip
            Efi[e][14] = al_itofix(128);                            // 180 deg
            if (dp) { if (is_up_solidfm(Efi[e][0], Efi[e][1], al_itofix(1), 0) < al_itofix(1)) printf(" - 6 with solid ceil\n"); else  printf(" - 6 with NOT solid ceil\n"); }
            mv = is_up_solidfm(Efi[e][0], Efi[e][1], Efi[e][2], 1);
            if (mv < Efi[e][2])                                  // allowed move less than requested move
            {
               Efi[e][0] += mv;                                  // move the allowed amount
               mode = 5;                                          // next mode (lwall up ) (ccw)
               Efi[e][14] = al_itofix(96);                          // 135 deg
               Efi[e][1] -= al_itofix(1);                           // move up 1 to catch new wall
               if (dp) printf("lost ceil during this move - mv%1.2f \n", al_fixtof(mv));
               break;
            }
            mv = is_right_solidfm(Efi[e][0], Efi[e][1], Efi[e][2], 0);
            if (mv < Efi[e][2])                                  // allowed move less than requested move
            {
               Efi[e][0] += mv;                                  // move the allowed amount
               mode = 7;                                          // next mode (rwall down) (cw)
               Efi[e][14] = al_itofix(160);                         // 225 deg
               if (dp) printf("hit rwall during this move - mv%1.2f \n", al_fixtof(mv));
               break;
            }
            Efi[e][0] += mv;  // if neither of these matched, move the allowed amount
         break;
         case 7:                                                  // rwall down
            Ei[e][2] = 0;                                        // h flip
            Efi[e][14] = al_itofix(192);                            // 270 deg
            if (dp) { if (is_right_solidfm(Efi[e][0], Efi[e][1], al_itofix(1), 0) < al_itofix(1)) printf(" - 7 with solid rwall\n"); else  printf(" - 7 with NOT solid rwall\n"); }
            mv = is_right_solidfm(Efi[e][0], Efi[e][1], Efi[e][3], 1);
            if (mv < Efi[e][3])                                  // allowed move less than requested move
            {
               Efi[e][1] += mv;                                  // move the allowed amount
               mode = 6;                                          // next mode (ceil right) (ccw)
               Efi[e][14] = al_itofix(160);                         // 225 deg
               Efi[e][0] += al_itofix(1);                           // move right 1 to catch new wall
               if (dp) printf("lost rwall during this move - mv%1.2f \n", al_fixtof(mv));
               break;
            }
            mv = is_down_solidfm(Efi[e][0], Efi[e][1], Efi[e][3], 0);
            if (mv < Efi[e][3])                                  // allowed move less than requested move
            {
               Efi[e][1] += mv;                                  // move the allowed amount
               mode = 4;                                          // next mode (floor left) (cw)
               Efi[e][14] = al_itofix(224);                         // 315 deg
               if (dp) printf("hit floor during this move - mv%1.2f \n", al_fixtof(mv));
               break;
            }
            Efi[e][1] += mv;  // if neither of these matched, move the allowed amount
         break;
      } // end of switch mode

      if (bullet_request)
      {
         int quad = 0;
         switch (mode)
         {
            case 0: case 5: quad = 1; break; // floor right, lwall up
            case 1: case 4: quad = 2; break; // rwall up floor left
            case 2: case 7: quad = 3; break; // ceil left, rwall down
            case 3: case 6: quad = 4; break; // lwall down, ceil right
         }
         int p = find_closest_player_quad(e, quad, prox);
         if (p != -1)
         {
            fire_enemy_bulleta(e, 20, p);
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
      int ex = al_fixtoi(Efi[e][0]);
      int s = ex % 6;
      Ei[e][1] = zz[10-s][b];
   }
   if ((mode == 1) || (mode == 5)) // y- rwall up and lwall up
   {
      int ey = al_fixtoi(Efi[e][1]);
      int s = ey % 6;
      Ei[e][1] = zz[5+s][b];
   }
   if ((mode == 2) || (mode == 4)) // x- ceil left or floor left
   {
      int ex = al_fixtoi(Efi[e][0]);
      int s = ex % 6;
      Ei[e][1] = zz[5+s][b];
   }
   if ((mode == 3) || (mode == 7)) // y+ lwall down or rwall down
   {
      int ey = al_fixtoi(Efi[e][1]);
      int s = ey % 6;
      Ei[e][1] = zz[10-s][b];
   }
}

//--7--podzilla-----------------------------------------------------------------------------
//     Ei[e][5] = mode
//     Ei[e][6] = sequence counter
//     Ei[e][7] = sequence limit
//     Ei[e][8] = wait count
//     Ei[e][9] = wait limit
//     Ei[e][11] = trigger box x1
//     Ei[e][12] = trigger box x1
//     Ei[e][13] = trigger box x2
//     Ei[e][14] = trigger box y2
void enemy_podzilla(int e)
{
   if (Ei[e][31]) // podzilla hit
   {
      enemy_killed(e);
      return;
   }
   enemy_player_hit_proc(e);
   if (Ei[e][5] == 0) // mode 0 - wait for trigger
   {
      // trigger box offset
      int x1 = Ei[e][11]*20 - 10;
      int y1 = Ei[e][12]*20 - 10;
      int x2 = Ei[e][13]*20 + 10;
      int y2 = Ei[e][14]*20 + 10;

      for (int p=0; p<NUM_PLAYERS; p++)
         if ((players[p].active) && (!players[p].paused))
         {
            int px = al_fixtoi(players[p].PX);
            int py = al_fixtoi(players[p].PY);
            if ((px > x1) && (px < x2) && (py > y1) &&  (py < y2))
            {
               Ei[e][5] = 1;          // set next mode
               Ei[e][6] = Ei[e][7];   // set extend counter
            }
         }
   }
   if (Ei[e][5] == 1) // mode 1; extend
   {
      Efi[e][0] += Efi[e][2];
      Efi[e][1] += Efi[e][3];
      if (--Ei[e][6] == 0)  // counter done?
      {
         Ei[e][5] = 2; // set next mode
         Ei[e][8] = frame_num;
      }
   }
   if (Ei[e][5] == 2)  // mode 2; wait then shoot
   {
      if (Ei[e][8] < (frame_num-Ei[e][9]))
      {
         int p = find_closest_player(e);
         fire_enemy_bulleta(e, 54, p);
         Ei[e][5] = 3; // set next mode
         Ei[e][8] = frame_num;
      }
   }
   if (Ei[e][5] == 3)  // mode 3; post shoot pause
   {
      if (Ei[e][8] < (frame_num-Ei[e][9]) )
      {
         Ei[e][5] = 4;        // next mode
         Ei[e][6] = Ei[e][7]; // set retract counter
      }
   }
   if (Ei[e][5] == 4) // mode 4; retract
   {
      Efi[e][0] -= Efi[e][2];
      Efi[e][1] -= Efi[e][3];
      if (--Ei[e][6] == 0)  // counter done?
      Ei[e][5] = 0; // set next mode (back to trigger)
   }

   Ei[e][1] = zz[ 5 + (Ei[e][6] * zz[4][15] / Ei[e][7]) ][15];  // bitmap = counter * numshapes / limit

   if ((Ei[e][5] == 2) || (Ei[e][5] == 3)) // rotate to face player
   {
      int p = find_closest_player(e);
      Efi[e][14] = get_rot_from_PXY(e, p);
      Ei[e][1] = 335; // green cannon ball
   }
   else Efi[e][14] = get_rot_from_xyinc(e); // rotate to face direction of movement
}



void bouncer_cannon_common(int e)
{
   if ((Ei[e][8]) && (Ei[e][7] > Ei[e][8])) // seek
   {
      Ei[e][7] = 0;
      seek_set_xyinc(e);
   }

   al_fixed f0 = al_itofix(0);

   if ((Efi[e][2]) > f0)  // move right
   {
     Efi[e][0] += Efi[e][2];
     if (is_right_solid(al_fixtoi(Efi[e][0]), al_fixtoi(Efi[e][1]), 1)) // bounce
     {
        Ei[e][7]++; // inc bounce count
        Efi[e][2] =- Efi[e][2]; // reverse xinc
        Efi[e][0] += Efi[e][2]; // take back last move
     }
   }
   if ((Efi[e][2]) < f0)  // move left
   {
      Efi[e][0] += Efi[e][2];
      if (is_left_solid(al_fixtoi(Efi[e][0]), al_fixtoi(Efi[e][1]), 1)) // bounce
      {
         Ei[e][7]++;
         Efi[e][2] =- Efi[e][2]; // reverse xinc
         Efi[e][0] += Efi[e][2]; // take back last move
      }
   }
   if (Efi[e][3] > f0) // move down
   {
      Efi[e][1] += Efi[e][3];
      if (is_down_solid(al_fixtoi(Efi[e][0]), al_fixtoi(Efi[e][1]), 1))
      {
         Ei[e][7]++;
         Efi[e][3] =- Efi[e][3]; // reverse yinc
         Efi[e][1] += Efi[e][3]; // take back last move
      }
   }
   if (Efi[e][3] < f0)  // move up
   {
      Efi[e][1] += Efi[e][3];
      if (is_up_solid(al_fixtoi(Efi[e][0]), al_fixtoi(Efi[e][1]), 0) == 1)
      {
         Ei[e][7]++;
         Efi[e][3] =- Efi[e][3]; // reverse yinc
         Efi[e][1] += Efi[e][3]; // take back last move
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

         al_fixed mul = al_ftofix(1.2);

         if (Efi[e][5] > al_itofix(10)) mul = al_ftofix(1.1);
         if (Efi[e][5] > al_itofix(20)) mul = al_ftofix(1.05);


         Efi[e][2]  = al_fixmul(Efi[e][2],  mul);  // x speed
         Efi[e][3]  = al_fixmul(Efi[e][3],  mul);  // y speed
         Efi[e][5]  = al_fixmul(Efi[e][5],  mul);  // seek speed
         Efi[e][7]  = al_fixmul(Efi[e][7],  mul);  // bullet speed
         Efi[e][12] = al_fixmul(Efi[e][12], al_ftofix(1.1)); // scale

         Ei[e][29] += 2; // collison box size
//
//printf("%d %f %f %f %f %f \n",Ei[e][9],
//                            al_fixtof(Efi[e][2]),
//                            al_fixtof(Efi[e][3]),
//                            al_fixtof(Efi[e][5]),
//                            al_fixtof(Efi[e][7]),
//                            al_fixtof(Efi[e][12]) );
//
//
//

      }
   }

   int p = find_closest_player(e);

   // time to shoot cannonball ?
   if (Ei[e][15])
      if (--Ei[e][16] < 0) // cannon shot wait
      {
         fire_enemy_bulleta(e, 55, p);
         Ei[e][16] = Ei[e][15]; // reset cannon shot wait
      }

   bouncer_cannon_common(e);

   // set rotation to always face player
   Efi[e][14] = get_rot_from_PXY(e, p);

   // set bitmap
   al_fixed ratio = al_fixdiv(al_itofix(Ei[e][16]), al_itofix(Ei[e][15]));
   Ei[e][1] = 412;  // green cannon by default
   if (ratio < al_ftofix(0.3))  Ei[e][1] = 413; // less green cannon
   if (ratio < al_ftofix(0.2))  Ei[e][1] = 414; // orange cannon
   if (ratio < al_ftofix(0.1))  Ei[e][1] = 415; // red cannon
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

   // set animation sequence
   if (Ei[e][7]) Ei[e][3] = Ei[e][5]; // main ans
   else Ei[e][3] = Ei[e][6]; // seek ans

   // set rotation based on direction of travel
   Efi[e][14] = get_rot_from_xyinc(e);


   // set the bitmap from frame_num mod by animation sequence

   // animation sequence number
   int ans = Ei[e][3];

   // number of shapes in animation sequence
   int ns = zz[4][ans];

   // number of frames for each shape
   int nf = zz[3][ans]+1;

   // total sequence length in frames
   int tsl = nf * ns;

   // get mod of frame_num
   int pm = frame_num % tsl;

   // get shape number from mod
   int ss = pm / nf;

   // set shape in enemy array
   Ei[e][1] = zz[5+ss][ans];
}


//-------------------------------------------------------------------------------------------
//--3--archwagon-----------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------
void enemy_archwagon(int e)
{
   int EXint = al_fixtoi(Efi[e][0]);
   int EYint = al_fixtoi(Efi[e][1]);

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
      int swl = 10; // bullet prox window low
      int swh = 6;  // bullet prox window high
      Ei[e][3] = 2; // wagon with arrow ans

      if (Ei[e][2]) // attempt shoot right
         for (int p=0; p<NUM_PLAYERS; p++)
            if ((players[p].active) && (!players[p].paused) )
               if ((EXint > al_fixtoi(players[p].PX) - Ei[e][17]) && (EXint < al_fixtoi(players[p].PX) ))
                  if ((EYint > al_fixtoi(players[p].PY) - swh) && (EYint < al_fixtoi(players[p].PY) + swl ))
                  {
                     fire_enemy_x_bullet(e, p);
                     Ei[e][16] = Ei[e][15]; // set new prox wait
                  }

      if (!Ei[e][2]) // attempt shoot left
         for (int p=0; p<NUM_PLAYERS; p++)
            if ((players[p].active) && (!players[p].paused) )
               if ((EXint > al_fixtoi(players[p].PX) ) && (EXint < al_fixtoi(players[p].PX)  + Ei[e][17]))
                  if ((EYint > al_fixtoi(players[p].PY)  - swh) && (EYint < al_fixtoi(players[p].PY) + swl ))
                  {
                     fire_enemy_x_bullet(e, p);
                     Ei[e][16] = Ei[e][15]; // set new prox wait
                  }
   }

   walker_archwagon_common(e);

   // set the bitmap and drawing mode
   int b = Ei[e][3]; // ans
   int c = zz[4][b]; // num_of_shapes in seq

   // animate with h move
   if (Ei[e][2] == 1) // right
   {
      int x = (EXint/3) % c;
      Ei[e][1] = zz[x+5][b];
   }
   if (Ei[e][2] == 0) // left
   {
      int x = (EXint/3) % c;
      Ei[e][1] = zz[5+c-x][b];
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

Efi[][0]  // x
Efi[][1]  // y
Efi[][12] // scale
Efi[][14] // rot

-------------------------------------------------------------------------
-- Variables used in proc_enemy_collision_with_pbullet(int e)
-------------------------------------------------------------------------
Ei[][26]  // player number that hit enemy with bullet
Ei[][29]  // enemies collison box size
Ei[][31]  // flag that this enemy got shot with bullet

Efi[][0] // x
Efi[][1] // y

-------------------------------------------------------------------------
-- Variables used in move_enemies() common
-------------------------------------------------------------------------
Ei[e][27] // time to live
Ei[e][29] // collision box

Efi[e][4] // life dec

-------------------------------------------------------------------------
-- Variables used in enemy_deathcount(int e)
-------------------------------------------------------------------------
Ei[][1]  // draw bitmap
Ei[][3]  // deathcount ans
Ei[][24] // health bonus shape
Ei[][25] // health bonus amount
Ei[][30] // death loop count

Efi[][11]  // scale inc
Efi[][12]  // scale
Efi[][13]  // rot inc
Efi[][14]  // rot


-------------------------------------------------------------------------
-- Variables used in enemy_player_hit_proc(int e)
-------------------------------------------------------------------------
Ei[][22] // player hit flag and player num + 1
Ei[][23] // hit player retrigger

Efi[][4] // life dec

-------------------------------------------------------------------------
-- Variables used in enemy_killed(int e)
-------------------------------------------------------------------------
Ei[][3]  // new ans

Ei[][24] // health bonus shape
Ei[][25] // health bonus amount
Ei[][26] // player num that killed enemy
Ei[][30] // death loop count
Ei[][31] // hit type (bullet or bomb)


Efi[][4]  // life dec
Efi[][11] // scale multiplier
Efi[][13] // rot inc

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
Ei[][24] = bonus shape
Ei[][25] = health bonus
Ei[][26] = used to tell what player killed enemy
Ei[][27] = time to live
Ei[][28] = cloner create id
Ei[][29] = collision box size
Ei[][30] = death loop count
Ei[][31] = enemy hit

Efi[][0] =  x
Efi[][1] =  y
Efi[][4] =  LIFE decrement
Efi[][11] = scale multiplier
Efi[][12] = scale;
Efi[][13] = rot inc
Efi[][14] = rot



// enemy types
----------------------------------
3  - Archwagon
4  - Bouncer
6  - Cannon
7  - Podzilla
8  - Trakbot
9  - Cloner
11 - Block Walker
12 - Flapper

similar types
3 - Archwagon
11 - Block Walker

4 - Bouncer
6 - Cannon


[3] - Archwagon -----------------------------------------------------------------------------
Ei[][3]   ans (2=wagon with arrow, 3=empty wagon)
Ei[][15]  bullet retrigger value
Ei[][16]  bullet retrigger count
Ei[][17]  bullet prox

Efi[][2]  y speed speed
Efi[][6]  x speed speed
Efi[][7]  bullet speed

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
Efi[][5] seek speed

[6]--cannon-----------------------------------------------------------------------------
Ei[][7]   seek counter
Ei[][8]   seek count
Ei[][9]   extra hits to kill
Ei[][15]  bullet retrigger value
Ei[][16]  bullet retrigger counter
Efi[][5]  seek speed
Efi[][7]  bullet speed
Efi[][12] scale
Efi[][14] rot

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
Ei[][3]  base animation seq (17=trakbot with bullet, 18=trakbot without bullet)
Ei[][4]  fall 160 max fall
Ei[][5]  mode
Ei[][7]  drop mode(0=no, 1=yes)
Ei[][8]  twirling counter
Ei[][15] shoot holdoff value
Ei[][16] shoot holdoff counter
Ei[][17] bullet prox
Ei[][20] rot
Efi[][7] bullet speed

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
Ei[][15] bullet retrigger time
Ei[][16] bullet retrigger counter
Ei[][17] prox width
Ei[][18] prox height
Ei[][19] prox depth

Ei[][20] height above player
Ei[][21] flap height

Efi[][5] max x speed
Efi[][6] x accel
Efi[][7] bullet speed


Efi[][8] flap offset for next loop
Efi[][9] flap speed counter
Efi[][10] flap speed inc

*/












































