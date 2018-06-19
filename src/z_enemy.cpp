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



         #ifdef SHOW_CLONERLINES
         if (Ei[e][0] == 9) // cloner
         {
            // show counter
            int cx = al_fixtoi(Efi[e][0]) + 10;     // middle of cloner
            int cy = al_fixtoi(Efi[e][1]) - 8;      // above cloner
            //int cy = al_fixtoi(Efi[e][1])+24;     // below cloner
            //int cy = al_fixtoi(Efi[e][1])+8;      // middle of cloner
            al_draw_textf(font, palette_color[10], cx, cy, ALLEGRO_ALIGN_CENTER, "%d" ,Ei[e][7] );

//            int x1 = al_fixtoi(Efi[e][6]) - 2;    // source x
//            int y1 = al_fixtoi(Efi[e][7]) - 2;    // source y
//            int x2 = x1 + x_size;
//            int y2 = y1 + y_size;
//
//            int x3 = al_fixtoi(Efi[e][8]) - 2;    // dest x
//            int y3 = al_fixtoi(Efi[e][9]) - 2;    // dest y
//            int x4 = x3 + x_size;
//            int y4 = y3 + y_size;

            int cw = Ei[e][19]*20;     // width
            int ch = Ei[e][20]*20;     // height

            int x1 = Ei[e][15]*20;    // source
            int y1 = Ei[e][16]*20;
            int x2 = x1 + cw;
            int y2 = y1 + ch;

            int x3 = Ei[e][17]*20;    // destination
            int y3 = Ei[e][18]*20;
            int x4 = x3 + cw;
            int y4 = y3 + ch;


            al_fixed ratio = al_fixdiv(al_itofix(Ei[e][7]), al_itofix(Ei[e][6])) * 10;

            if ((ratio < al_ftofix(2) ) && (ratio > al_ftofix(.5) ))
            {
               int color = 10; // red

            // ratio is a float between 0 and 10
            // to get here ratio must be > 2 and < 4
            // converts to il and li which are the sizes of the inner and outer boxes
            // when stated ratio is 3.999 and ends when ratio is 1.999
            // when started li needs to be ~40-100 and 0 when ends...
            // when started il needs to be ~40-100 and 20 when ends...
               al_fixed tr = ratio - al_itofix(2); // starts at 1 and goes to -1
               int il = al_fixtoi(tr * 32);
               int li = al_fixtoi(tr * 16) - 20;
               al_draw_rectangle(x1-il, y1-il, x2+il, y2+il, palette_color[color], 1);
               al_draw_rectangle(x1-li, y1-li, x2+li, y2+li, palette_color[color], 1);
               al_draw_line(x1-il, y1-il, x1-li, y1-li, palette_color[color], 1);
               al_draw_line(x2+il, y2+il, x2+li, y2+li, palette_color[color], 1);
               al_draw_line(x1-il, y2+il, x1-li, y2+li, palette_color[color], 1);
               al_draw_line(x2+il, y1-il, x2+li, y1-li, palette_color[color], 1);
            }
            if (ratio < al_itofix(1) )
            {
               int color = 11; // green
               al_fixed tr = al_itofix(1) - ratio; // starts at 0 and goes to 1
               int il = al_fixtoi(tr * 8);
               int li = al_fixtoi(tr * 4) - 20;
               al_draw_rectangle(x3-li, y3-li, x4+li, y4+li, palette_color[color], 1);
               al_draw_rectangle(x3-il, y3-il, x4+il, y4+il, palette_color[color], 1);
               al_draw_line(x3-il, y3-il, x3-li, y3-li, palette_color[color], 1);
               al_draw_line(x4+il, y4+il, x4+li, y4+li, palette_color[color], 1);
               al_draw_line(x3-il, y4+il, x3-li, y4+li, palette_color[color], 1);
               al_draw_line(x4+il, y3-il, x4+li, y3-li, palette_color[color], 1);
            }
         }
         #endif


         #ifdef SHOW_FLAPPER_TRIGGER_BOX
         if (Ei[e][0] == 12) // flapper
         {

            // draw trigger box
            int width =  Ei[e][17];
            int height = Ei[e][18];
            int depth =  Ei[e][19];
            al_draw_rectangle(EXint-width+10, EYint-height, EXint+width+10, EYint+depth, palette_color[14], 1);

            // draw height above player
            int hab =  Ei[e][20];
            al_draw_line(EXint-40+10, EYint+hab, EXint+40+10, EYint+hab, palette_color[10], 1);

         }
         #endif


         #ifdef SHOW_TRAKBOT_BULLET_TRIGGER_CIRCLE
         // show trakbot bullet prox circle
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

//void proc_enemy_collision_with_player(int p)
//{
//   for (int e=0; e<100; e++)
//   {
//      if ((Ei[e][0]) && (Ei[e][0] != 99)) // if enemy active and not deathcount
//      {
//         al_fixed px = players[p].PX;
//         al_fixed py = players[p].PY;
//
//         al_fixed b = al_itofix(Ei[e][29]); // collision box size
//         al_fixed ex1 = Efi[e][0] - b;
//         al_fixed ex2 = Efi[e][0] + b;
//         al_fixed ey1 = Efi[e][1] - b;
//         al_fixed ey2 = Efi[e][1] + b;
//
//         if ((px > ex1) && (px < ex2) && (py > ey1) && (py < ey2)) Ei[e][22] = p+1; // player collision
//      }
//   }
//}

void olddumbproc_enemy_collision_with_pbullet(int e)
{
   int EXint = al_fixtoi(Efi[e][0]);
   int EYint = al_fixtoi(Efi[e][1]);
   for (int c=0; c<50; c++)
      if (pbullet[c][0])
      {
         // bullet collision box size adjusted with bullet speed
         int bx = abs(pbullet[c][4]/2) + pm_bullet_collision_box;
         int by = abs(pbullet[c][5]/2) + pm_bullet_collision_box;

         // bullet collision box size adjusted with enemies collison box size
         bx += Ei[e][29] - 10;
         by += Ei[e][29] - 10;

         // check for collision with player's bullets
         if ((abs(pbullet[c][2] - EXint) < bx) && (abs(pbullet[c][3] - EYint) < by))
         {
            int p = pbullet[c][1];   // player number that shot bullet
            Ei[e][31] = 1;           // flag that this enemy got shot with bullet
            players[p].num_hits++;   // add to number of hits the player has
            pbullet[c][0] = 0;       // bullet dies
         }
      }
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
         if (Ei[e][0] != 99) proc_enemy_collision_with_pbullet(e);

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
//   Efi[e][12] = fixmul(Efi[e][11], Efi[e][12]); // scale inc
   Efi[e][12] = mdw_fixmul(Efi[e][11], Efi[e][12], 0.0001); // scale inc

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
   int EXint = al_fixtoi(Efi[e][0]);
   int EYint = al_fixtoi(Efi[e][1]);
   if (--Ei[e][23]<0) // hit player retrigger
   {
      if (Ei[e][22]) // player hit!
      {
         int p = Ei[e][22]-1;
         players[p].LIFE -= Efi[e][4];

         game_event(7, EXint, EYint, al_fixtoi(Efi[e][4]), 0, 0, 0);
         game_event(12, EXint, EYint, e, 0, 0, 0);
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
      case 8:
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
      case 9:
         na = Ei[e][3] = 53;  // new ans
         dl = Ei[e][30] = 40; // death_loop_wait; set delay
         Ei[e][24] = 934+(ht-1)*32; // shape
         Efi[e][11] = al_ftofix(.98); // scale multiplier
         Efi[e][13] = al_ftofix(306/dl); // rot inc

         zz[0][na] = zz[5][na]; // set shape
         zz[1][na] = 0;         // point to zero
         zz[2][na] = frame_num; // set counter
         zz[3][na] = dl / zz[4][na]; // set ans timer
      break;
      case 11:

      break;
      case 12:
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
      int EXint = al_fixtoi(Efi[e][0]);
      int EYint = al_fixtoi(Efi[e][1]);
      game_event(13, EXint, EYint, e, 0, 0, 0); // set type to death loop
      Ei[e][0] = 99;
   }
}




al_fixed mdw_fixmul(al_fixed a, al_fixed b, float f_round)
{
   float flc = al_fixtof(a) * al_fixtof(b);   // convert to float and multiply
   flc = round(flc/f_round) * f_round; // round
   return al_ftofix(flc);

//   al_fixed res = al_ftofix(flc);

//   printf("reg fixmul a:%f b:%f c:%f\n", fla, flb, al_fixtof(fixmul(a, b)));
//   printf("mdw fixmul a:%f b:%f c:%f\n", fla, flb, al_fixtof(res));
//   printf("mdw fixmul a:%f b:%f c:%f\n", fla, flb, flc);

//   return res;
//   return fixmul(a, b);

}

















//--12--flapper-----------------------------------------------------------------------------

//   Ei[e][0] = type 12 "flapper"
//   Ei[e][1] = draw bitmap
//   Ei[e][2] = hflip - right(1) or left(0)
//   Ei[e][3] = base animation sequence

//   Ei[e][4] = animation sequence counter
//   Ei[e][5] = animation sequence index
//   Ei[e][6]


//   Ei[e][15] = bullet retrigger time
//   Ei[e][16] = bullet retrigger counter

//   Ei[e][17] = prox width
//   Ei[e][18] = prox height
//   Ei[e][19] = prox depth
//   Ei[e][20] = height above player



//   Ei[e][30] = collision box
//   Ei[e][31] = health bonus



//   Efi[e][0]  = x pos
//   Efi[e][1]  = y pos
//   Efi[e][2]  = x inc
//   Efi[e][3]  = y inc
//   Efi[e][4]  = health decrement
//   Efi[e][5]  = max x speed
//   Efi[e][6]  = x accel
//   Efi[e][7]  = bullet speed
//   Efi[e][8]  = flap yinc scaler
//   Efi[e][9]  =
//   Efi[e][10] =
//   Efi[e][11] =
//   Efi[e][12] = // scale
//   Efi[e][13] =
//   Efi[e][14] =
//   Efi[e][15] =

void enemy_flapper(int e)
{
   if (Ei[e][31]) // hit
   {
      enemy_killed(e);
      return;
   }
   enemy_player_hit_proc(e);


   int p = -1; // default if none found in range

   if (Ei[e][2])  // move right
   {
      p = find_closest_player_flapper(e, 1);
      Efi[e][2] += Efi[e][6];                             // accel
      if (Efi[e][2] > Efi[e][5]) Efi[e][2] = Efi[e][5]; // max speed
      Efi[e][0] += Efi[e][2];                             // apply xinc

      if (is_right_solid(al_fixtoi(Efi[e][0]), al_fixtoi(Efi[e][1]), 1))
      {
         Efi[e][0] -= Efi[e][2];    // take back last move
         Efi[e][2] = -Efi[e][5]/3;  // set accel to bounce back with 1/3 max accel
         Ei[e][2] = 0;               // change direction
      }
      // try to shoot right
      if (--Ei[e][16] < 0)
      {
         if (p != -1)
         {
            fire_enemy_bulleta(e, 62, p);
            Ei[e][16] = Ei[e][15]; // set new prox wait
         }
      }
   }
   if (!Ei[e][2])  // move left
   {
      p = find_closest_player_flapper(e, 0);
      Efi[e][2] -= Efi[e][6];                               // accel
      if (Efi[e][2] < -Efi[e][5]) Efi[e][2] = -Efi[e][5]; // max speed
      Efi[e][0] += Efi[e][2];                               // apply xinc

      if (is_left_solid(al_fixtoi(Efi[e][0]), al_fixtoi(Efi[e][1]), 1))
      {
         Efi[e][0] -= Efi[e][2];   // take back last move
         Efi[e][2] = Efi[e][5]/3;  // set accel to bounce back with 1/3 max accel
         Ei[e][2] = 1;              // change direction
      }
      // try to shoot left
      if (--Ei[e][16] < 0)
      {
         if (p != -1)
         {
            fire_enemy_bulleta(e, 62, p);
            Ei[e][16] = Ei[e][15]; // set new prox wait
         }

       }
   }
//   printf("dir:%d  xinc :%3.2f  \n", Ei[e][6], al_fixtof(Efi[e][2]) );



   int EXint = al_fixtoi(Efi[e][0]);
   int EYint = al_fixtoi(Efi[e][1]);

   al_fixed yinc = al_itofix(0);  // yinc for this pass


   p = find_closest_player(e); // always seek in y axis (comment out this line to only seek if player in trigger box)
   if (p != -1) // only seek in y axis if valid player in prox
   {
      int height_above_player = Ei[e][20];
      if (EYint < al_fixtoi(players[p].PY)+10 - height_above_player) yinc += Efi[e][3];
      if (EYint > al_fixtoi(players[p].PY)+10 - height_above_player) yinc -= Efi[e][3];
   }




   Ei[e][4]++; // my ans counter
   if (Ei[e][4] > 22) Ei[e][4] = 0;

   int a=0, so, yo;

   if (Ei[e][4] == a++) { so = 0; yo = -3; } // wings at bottom
   if (Ei[e][4] == a++) { so = 0; yo = -4; }
   if (Ei[e][4] == a++) { so = 0; yo = -3; }
   if (Ei[e][4] == a++) { so = 0; yo = -3; }
   if (Ei[e][4] == a++) { so = 0; yo = -2; }
   if (Ei[e][4] == a++) { so = 0; yo = -1; }
   if (Ei[e][4] == a++) { so = 0; yo = +0; }
   if (Ei[e][4] == a++) { so = 0; yo = +1; }

   if (Ei[e][4] == a++) { so = 1; yo = +2; } // wings 1/2 bottom coming up
   if (Ei[e][4] == a++) { so = 1; yo = +2; }
   if (Ei[e][4] == a++) { so = 1; yo = +2; }

   if (Ei[e][4] == a++) { so = 2; yo = +2; } // wings 1/2 up coming up
   if (Ei[e][4] == a++) { so = 2; yo = +2; }
   if (Ei[e][4] == a++) { so = 2; yo = +2; }

   if (Ei[e][4] == a++) { so = 3; yo = +2; } // wings at top
   if (Ei[e][4] == a++) { so = 3; yo = +2; }
   if (Ei[e][4] == a++) { so = 3; yo = +2; }

   if (Ei[e][4] == a++) { so = 4; yo = +2; } // wings 1/2 up coming down
   if (Ei[e][4] == a++) { so = 4; yo = +1; }
   if (Ei[e][4] == a++) { so = 4; yo = -0; }

   if (Ei[e][4] == a++) { so = 5; yo = -1; } // wings 1/2 down coming down
   if (Ei[e][4] == a++) { so = 5; yo = -2; }
   if (Ei[e][4] == a++) { so = 5; yo = -3; }


   // show if empty or has bullet ready
   if (Ei[e][16] < 0) Ei[e][3] = 61;
   else Ei[e][3] = 60;
   Ei[e][1] = zz[5+so][Ei[e][3]];               // set shape from ans and shape offset

   al_fixed fyo = Efi[e][8] * yo;                   // scale flap yinc

   yinc += fyo;                                   // add flap yinc to other yinc
   Efi[e][1] += yinc;                            // apply yinc
   EYint = al_fixtoi(Efi[e][1]);

   if (yinc < al_itofix(0))
      if (is_up_solid(EXint, EYint, 1))
         Efi[e][1] -= yinc; // take back move
   if (yinc > al_itofix(0))
      if (is_down_solid(EXint, EYint, 1))
         Efi[e][1] -= yinc; // take back move

}




/* show whats going on...
   printf("step:%d  so:%d  yo:%3.2f\n",Ei[e][4], so, al_fixtof(fyo));
   if (yo < 0) Ei[e][20] += yo;
   if (yo > 0) Ei[e][21] += yo;

   if (Ei[e][4] == 0)
   {
      printf("up_count%d down_count:%d\n",Ei[e][20], Ei[e][21]);
      Ei[e][20] = 0;
      Ei[e][21] = 0;
   }
*/


/*

   // follow in y axis
   if (EYint < al_fixtoi(players[p].PY) - height_above_player)
   {
      Efi[e][1] += Efi[e][3];
      EYint = al_fixtoi(Efi[e][1]);
      if (is_down_solid(EXint, EYint, 1)) Efi[e][1] -= Efi[e][3]; // take back move
   }
   if (EYint > al_fixtoi(players[p].PY) - height_above_player)
   {
      Efi[e][1] -= Efi[e][3];
      EYint = al_fixtoi(Efi[e][1]);
      if (is_up_solid(EXint, EYint, 1)) Efi[e][1] += Efi[e][3]; // take back move
   }

*/



/*


 // bounce in x axis
   if ((Efi[e][2]) > al_itofix(0))  // move right
   {
      int prox = Ei[e][17];
      Ei[e][2] = 1; // h_flip to face right
      // try to shoot right
      if (--Ei[e][16] < 0)
         if (EYint+Ei[e][18] > al_fixtoi(players[p].PY) && (EYint-Ei[e][19]) < al_fixtoi(players[p].PY))
            if (EXint < al_fixtoi(players[p].PX) && (EXint + prox) > al_fixtoi(players[p].PX) )
            {
               fire_enemy_bulleta(e, 62, p);
               Ei[e][16] = Ei[e][15]; // set new prox wait
            }
     Efi[e][0] += Efi[e][2];
     EXint = al_fixtoi(Efi[e][0]);
     EYint = al_fixtoi(Efi[e][1]);
     if (is_right_solid(EXint, EYint, 1))
     {
        Efi[e][0] -= Efi[e][2]; // take back last move
        Efi[e][2] =- Efi[e][2]; // bounce
        EXint = al_fixtoi(Efi[e][0]);
     }
   }
   else if ((Efi[e][2]) < al_itofix(0))  // move left
   {
      int prox = Ei[e][17];
      Ei[e][2] = 0; // no h_flip
      // try to shoot left
      if (--Ei[e][16] < 0)
         if (EYint+Ei[e][18] > al_fixtoi(players[p].PY) && (EYint-Ei[e][19]) < al_fixtoi(players[p].PY))
            if (EXint-prox < al_fixtoi(players[p].PX) &&  (EXint) > al_fixtoi(players[p].PX) )
            {
               fire_enemy_bulleta(e, 62, p);
               Ei[e][16] = Ei[e][15]; // set new prox wait
            }
      Efi[e][0] += Efi[e][2];
      EXint = al_fixtoi(Efi[e][0]);
      EYint = al_fixtoi(Efi[e][1]);
      if (is_left_solid(EXint, EYint, 1))
      {
         Efi[e][0] -= Efi[e][2]; // take back last move
         Efi[e][2] =- Efi[e][2]; // bounce
         EXint = al_fixtoi(Efi[e][0]);
      }
   }

*/


/*
   int b = Ei[e][3]; // ans
   int c = zz[4][b];  // num of shapes in seq
   int x = (EXint/5) % c;
   Ei[e][1] = zz[x+5][b];
*/


/*
   Ei[e][4]++; // my ans counter
   if (Ei[e][4] > 2)
   {
      Ei[e][4] = 0;
      Ei[e][5]++; // current sequence
      if (Ei[e][5] > 5) E[e][5] = 0;
   }

   int so = Ei[e][5]; // current sequence


   Ei[e][1] = zz[5+so][Ei[e][3]];


   int yo;
   if (so == 0) yo = -15;
   if (so == 1) yo = 2;
   if (so == 2) yo = 10;
   if (so == 3) yo = 15;
   if (so == 4) yo = -2;
   if (so == 5) yo = -10;

   al_fixed fyo = al_itofix(yo)/8;

//   printf("so:%d  yo:%3.2f\n",so, al_fixtof(fyo));

*/

// 6 shapes
// 8 coast
// 3 for all others 3x5 = 15
// 23 total


// coast      = 8 * +1 = +8
// wings up   = 6 * +3 = +18

// wing top   = 3 * +1

// wings down = 6 * -5 = -30





/*
   int so, yo;
   if ((Ei[e][4] >= 0) && (Ei[e][4] < 8)) // coast (0-7)
   {
      so = 0;  // wings at bottom
      yo = +1;
   }

   if ((Ei[e][4] > 7) && (Ei[e][4] < 11)) // wings coming up (8-10)
   {
      so = 1;   // wings 1/2 bottom coming up
      yo = +2;
   }

   if ((Ei[e][4] > 10) && (Ei[e][4] < 14)) // wings coming up (11-13)
   {
      so = 2; // wings 1/2 top coming up
      yo = +2;
   }

   if ((Ei[e][4] > 13) && (Ei[e][4] < 17)) // wings at top (14-16)
   {
      so = 3; // wings at top
      yo = +2;
   }

   if ((Ei[e][4] > 16) && (Ei[e][4] < 20)) // wings coming down (17-19)
   {
      so = 4; // wings 1/2 up coming down
      yo = -4;
   }

   if ((Ei[e][4] > 16) && (Ei[e][4] < 20)) // wings coming down (20-22)
   {
      so = 5; // wings 1/2 down coming down
      yo = -4;
   }
*/












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


      game_event(13, EXint, EYint, e, 0, 0, 0);
      Ei[e][0] = 0;
      return; // break;  to stop rest of execution
   }
   if (!Ei[e][8]) // follow mode
   {
      int p = find_closest_player(e);
      if (EXint < al_fixtoi(players[p].PX)) Efi[e][2] = Efi[e][6];
      if (EXint > al_fixtoi(players[p].PX)) Efi[e][2] = -Efi[e][6];
   }

   int on_solid = 0;
   if (is_down_solid(EXint, EYint, 1)) on_solid = 1;

   if ((Efi[e][2]) > al_itofix(0))  // move right
   {
      Ei[e][2] = 1; // no h_flip
      Efi[e][0] += Efi[e][2];
      EXint= al_fixtoi(Efi[e][0]);
      if (on_solid)
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
      if (on_solid)
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



   if ((!on_solid) && (Ei[e][5] >= 0)) // not solid and falling
   {
      Ei[e][5] +=5; // gravity
      if (Ei[e][5] < -160) Ei[e][5] = -160; // terminal velocity

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


   if (on_solid)
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
   // set the bitmap and drawing mode
   int b = Ei[e][3];      // ans
   int c = zz[4][b];         // num_of_shapes in seq
   int x;
   if (Ei[e][4] == 0) Ei[e][1] = Ei[e][3]; // bmp, not ans
   if (Ei[e][4] == 1) Ei[e][1] = zz[0][b];    // animate with time

   if (Ei[e][4] == 2) // animate with h move
   {
      x = (EXint/3) % c;
      Ei[e][1] = zz[x+5][b];
   }

   if (Ei[e][4] == 3) // animate with v move
   {
      x = (EYint/3) % c;
      Ei[e][1] = zz[x+5][b];
   }
   if (Ei[e][4] == 4) // animate with v and h move
   {
      x = ((EYint/3) % c) + ((EXint/3) % c);
      if (x > c) x-=c;
      Ei[e][1] = zz[x+5][b];
   }
}



//--9--cloner-----------------------------------------------------------------------------
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
   int EXint = al_fixtoi(Efi[e][0]);
   int EYint = al_fixtoi(Efi[e][1]);

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


   al_fixed ratio = al_fixdiv(al_itofix(Ei[e][7]), al_itofix(Ei[e][6])) * 10;

   int b = 10 - al_fixtoi(ratio);
   Ei[e][2] = 1;
   Ei[e][1] = zz[5+b][53];

   if (Ei[e][31]) // hit
   {
      enemy_killed(e);
      return;
   }
   enemy_player_hit_proc(e);

   if (Ei[e][5] == 1) // in trig box
      switch (Ei[e][8])
      {
         case 0: case 1: case 2:
            int in_box = 0;
            for (int p=0; p<NUM_PLAYERS; p++)
            if ((players[p].active) && (!players[p].paused))
               {
                  int px = al_fixtoi(players[p].PX);
                  int py = al_fixtoi(players[p].PY);
                  if ((px > x4) && (px < x5) && (py > y4) &&  (py < y5)) in_box = 1;
               }
             if (in_box)
             {
                if (--Ei[e][7] < 0)  Ei[e][5] = 2; // time for create
             }
             else Ei[e][5] = 0; // mode 0 - out of box
          break;
      }

   if (Ei[e][5] == 0) // not in trig box last time
   {
      int in_box = 0;

      for (int p=0; p<NUM_PLAYERS; p++)
         if ((players[p].active) && (!players[p].paused))
         {
            int px = al_fixtoi(players[p].PX);
            int py = al_fixtoi(players[p].PY);
            if ((px > x4) && (px < x5) && (py > y4) &&  (py < y5)) in_box = 1;
         }
      if (in_box)
      {
         switch (Ei[e][8]) // trigger mode
         {
            case 0: Ei[e][5] = 1; break; // set mode 1 - in box

             // reset when retriggered
            case 1: Ei[e][7] = Ei[e][6]; // reset counter
                     Ei[e][5] = 1; break; // set mode 1 - in box

             // immediate when retriggered
            case 2:  Ei[e][7] = 0; // reset counter
                      Ei[e][5] = 1; break; // set mode 1 - in box
         }
      }
   }

   if (Ei[e][5] == 2)  // mode 2 - create
   {
      Ei[e][5] = 1; // set mode 1 - in trig box
      Ei[e][7] = Ei[e][6]; // reset counter
      Ei[e][1] = zz[15][53]; // full red
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
         game_event(14, EXint, EYint, 0, 0, 0, 0);
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
//     Ei[e][4] = mode
//     Ei[e][5] =
//     Ei[e][6] =
//     Ei[e][7] = drop mode 1 = yes
//     Ei[e][8] =


//     Ei[e][15] = shoot holdoff value
//     Ei[e][16] = shoot holdoff counter
//     Ei[e][17] = prox


//     Ei[e][22] = player hit
//     Ei[e][23] = player hit retrigger
//     Ei[e][24] = bullet bonus
//     Ei[e][25] = health bonus
//     Ei[e][26] = bonus type
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

   Ei[e][1] = zz[ 5 + (Ei[e][6] * zz[4][15] / Ei[e][7]) ][15];     // bitmap = counter * numshapes / limit

   if ((Ei[e][5] == 2) || (Ei[e][5] == 3)) // rotate to face player
   {
      int p = find_closest_player(e);
      Efi[e][14] = get_rot_from_PXY(e, p);
      Ei[e][1] = 335; // green cannon ball
   }
   else Efi[e][14] = get_rot_from_xyinc(e); // rotate to face direction of movement
}

//----------------------------------------------------------------------------------------
//--6--cannon-----------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
void enemy_cannon(int e)
{
   int EXint = al_fixtoi(Efi[e][0]);
   int EYint = al_fixtoi(Efi[e][1]);

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


         Efi[e][2] =  (Efi[e][2]  * 1200) / 1000;  // x speed
         Efi[e][3] =  (Efi[e][3]  * 1200) / 1000;  // y speed
         Efi[e][5] =  (Efi[e][5]  * 1200) / 1000;  // seek speed
         Efi[e][7] =  (Efi[e][7]  * 1200) / 1000;  // bullet speed
         Efi[e][12] = (Efi[e][12] * 1120) / 1000;  // scale

/*         Efi[e][2]  = fmul(Efi[e][2],  al_ftofix(1.2));  // x speed
         Efi[e][5]  = fmul(Efi[e][5],  al_ftofix(1.2));  // seek speed
         Efi[e][5]  = fmul(Efi[e][5],  al_ftofix(1.2));  // seek speed
         Efi[e][7]  = fmul(Efi[e][7],  al_ftofix(1.2));  // bullet speed
         Efi[e][12] = fmul(Efi[e][12], al_ftofix(1.12)); // scale
*/



         Ei[e][29] += 2; // collison box size
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

   if ((Ei[e][8]) && (Ei[e][7] > Ei[e][8])) // seek
   {
      seek_set_xyinc(e);
      Ei[e][7] = 0;
   }


   // set bitmap
   al_fixed ratio = al_fixdiv(al_itofix(Ei[e][16]), al_itofix(Ei[e][15]));
   Ei[e][1] = 412;  // green cannon by default
   if (ratio < al_ftofix(0.3))  Ei[e][1] = 413; // less green cannon
   if (ratio < al_ftofix(0.2))  Ei[e][1] = 414; // orange cannon
   if (ratio < al_ftofix(0.1))  Ei[e][1] = 415; // red cannon

   if ((Efi[e][2]) > al_itofix(0))  // move right
   {
     Efi[e][0] += Efi[e][2];
     EXint = al_fixtoi(Efi[e][0]);
     if (is_right_solid(EXint, EYint, 1)) // bounce
     {
        Ei[e][7]++; // inc bounce count
        Efi[e][2] =- Efi[e][2]; // reverse xinc
        Efi[e][0] += Efi[e][2]; // take back last move
     }
   }
   if ((Efi[e][2]) < al_itofix(0))  // move left
   {
      Efi[e][0] += Efi[e][2];
      EXint = al_fixtoi(Efi[e][0]);
      if (is_left_solid(EXint, EYint, 1)) // bounce
      {
         Ei[e][7]++;
         Efi[e][2] =- Efi[e][2];
         Efi[e][0] += Efi[e][2];
      }
   }
   if (Efi[e][3] > al_itofix(0)) // move down
   {
      Efi[e][1] += Efi[e][3];
      EYint = al_fixtoi(Efi[e][1]);
      if (is_down_solid(EXint, EYint, 1))
      {
         Ei[e][7]++;
         Efi[e][3] =- Efi[e][3];
         Efi[e][1] += Efi[e][3];
      }
   }
   if (Efi[e][3] < al_itofix(0))  // move up
   {
      Efi[e][1] += Efi[e][3];
      EYint = al_fixtoi(Efi[e][1]);
      if (is_up_solid(EXint, EYint, 0) == 1)
      {
         Ei[e][7]++;
         Efi[e][3] =- Efi[e][3];
         Efi[e][1] += Efi[e][3];
      }
   }
   Efi[e][14] = get_rot_from_PXY(e, p);

}

//--4--bouncer-----------------------------------------------------------------------------

void enemy_bouncer(int e)
{
   int EXint = al_fixtoi(Efi[e][0]);
   int EYint = al_fixtoi(Efi[e][1]);
   if (Ei[e][31]) // hit
   {
      enemy_killed(e);
      return; // break;  to stop rest of execution
   }
   enemy_player_hit_proc(e);
   if ((Ei[e][8]) && (Ei[e][7] > Ei[e][8])) // seek
   {
      Ei[e][7] = 0;
      seek_set_xyinc(e);
      Efi[e][14] = get_rot_from_xyinc(e);
      Ei[e][3] = Ei[e][6]; // seek ans
   }
   if (Ei[e][7]) Ei[e][3] = Ei[e][5]; // main ans

   if ((Efi[e][2]) > al_itofix(0))  // move right
   {
      Efi[e][0] += Efi[e][2];
      EXint = al_fixtoi(Efi[e][0]);
      if (is_right_solid(EXint, EYint, 1)) // bounce
      {
         Ei[e][7]++; // inc bounce count
         Efi[e][2] =- Efi[e][2]; // reverse xinc
         Efi[e][0] += Efi[e][2]; // take back last move
         Efi[e][14] = get_rot_from_xyinc(e);
      }
   }
   if ((Efi[e][2]) < al_itofix(0))  // move left
   {
      Efi[e][0] += Efi[e][2];
      EXint = al_fixtoi(Efi[e][0]);
      if (is_left_solid(EXint, EYint, 1)) // bounce
      {
         Ei[e][7]++; // inc bounce count
         Efi[e][2] =- Efi[e][2]; // reverse xinc
         Efi[e][0] += Efi[e][2]; // take back last move
         Efi[e][14] = get_rot_from_xyinc(e);
      }
   }
   if (Efi[e][3] > al_itofix(0)) // move down
   {
      Efi[e][1] += Efi[e][3];
      EYint = al_fixtoi(Efi[e][1]);
      if (is_down_solid(EXint, EYint, 1))
      {
         Ei[e][7]++;
         Efi[e][3] =- Efi[e][3];
         Efi[e][1] += Efi[e][3];
         Efi[e][14] = get_rot_from_xyinc(e);
      }
   }
   if (Efi[e][3] < al_itofix(0))  // move up
   {
      Efi[e][1] += Efi[e][3];
      EYint = al_fixtoi(Efi[e][1]);
      if (is_up_solid(EXint, EYint, 0) == 1)
      {
         Ei[e][7]++;
         Efi[e][3] =- Efi[e][3];
         Efi[e][1] += Efi[e][3];
         Efi[e][14] = get_rot_from_xyinc(e);
      }
   }


   // set the bitmap from animation sequence - always with time
//   Ei[e][1] = zz[0][Ei[e][3]];

// changed this to make chunk netplay work good



// old
//   int old = zz[0][Ei[e][3]];


   // ans number
   int ans = Ei[e][3];
   // how many shapes does this animation seq have?
   int ns = zz[4][ans];

   // how many frames for each shape
   int nf = zz[3][ans]+1;

   // total seq length in frame
   int tsl = nf * ns;

   // get mod
   int pm = frame_num % tsl;

   // get sequences number from mod
   int ss = pm / nf;

   // get shape
   int nw = zz[5+ss][ans];

//   printf("ns:%d nf:%d tsl:%2d pm:%2d ss:%d nw:%d old:%d\n",ns, nf, tsl, pm, ss, nw, old);

   Ei[e][1] = nw;




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

   // set the bitmap and drawing mode
   int x;
   int b = Ei[e][3]; // ans
   int c = zz[4][b]; // num_of_shapes in seq
   if (Ei[e][4] == 0) Ei[e][1] = Ei[e][3]; // bmp, not ans
   if (Ei[e][4] == 1) Ei[e][1] = zz[0][b];    // animate with time
   if (Ei[e][4] == 2) // animate with h move
   {
      if (Ei[e][2] == 1) // right
      {
         x = (EXint/3) % c;
         Ei[e][1] = zz[x+5][b];
      }
      if (Ei[e][2] == 0) // left
      {
         x = (EXint/3) % c;
         Ei[e][1] = zz[5+c-x][b];
      }
   }
   if (Ei[e][4] == 3) // animate with v move
   {
      x = (EYint/3) % c;
      Ei[e][1] = zz[x+5][b];
   }
   if (Ei[e][4] == 4) // animate with v and h move
   {
      x = ((EYint/3) % c) + ((EXint/3) % c);
      if (x > c) x-=c;
      Ei[e][1] = zz[x+5][b];
   }
}




/*

common variables
---------------------

Ei[][0] = enemy type
Ei[][1] = bitmap
Ei[][2] = draw mode
Ei[][4] = mode

Ei[][9]  = extra hits to kill


Ei[][22] = player hit
Ei[][23] = player hit retrigger
Ei[][24] = bonus shape
Ei[][25] = health bonus

Ei[][26] = unused

Ei[][27] = time to live
Ei[][28] = cloner create id
Ei[][29] = collision box size
Ei[][30] = death loop count
Ei[][31] = enemy hit

Efi[][0] =  x
Efi[][1] =  y
Efi[][2] =  xinc
Efi[][3] =  yinc
Efi[][4] =  LIFE decrement
Efi[][7] =  bullet speed
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


----------------------------
-- Variables used in draw --
----------------------------
Ei[e][1] = bitmap
Ei[e][2] = draw mode (v and h flips)
Efi[e][12] = scale;
Efi[e][14] = rot

The animation sequence can be stored in Ei[e][3]
but the enemy move code is responsible for setting the actual bitmap in Ei[e][2]


----------------------------------------------------
-- Variables used after death to create the bonus --
----------------------------------------------------
Ei[e][24]  shape
Ei[e][25]  health bonus
Ei[e][30]  death_loop_wait counter
Ei[e][31]  hit type (1 = bullet, 2 = bomb)

Efi[e][11]  scale multiplier
Efi[e][13]  rot inc


[3] - Archwagon -----------------------------------------------------------------------------
Ei[][3]   ans (2=wagon with arrow, 3=empty wagon)
Ei[][5]   jump/fall -160 max jump, 160 max fall
Ei[][7]   jump when player above
Ei[][8]   follow(0) or bounce(1)
Ei[][11]  jump before hole
Ei[][12]  jump before wall
Ei[][15]  bullet retrigger value
Ei[][16]  bullet retrigger count
Ei[][17]  bullet prox

Efi[][7]  bullet speed


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
Ei[][6]  jump wait (0=none)
Ei[][7]  jump when player above
Ei[][8]  follow(0) or bounce(1)
Ei[][11] jump before hole
Ei[][12] jump before wall
Efi[][8] fall and fallcount
Efi[][9] jump and jumpcount

[12]--flapper-----------------------------------------------------------------------------
Ei[][4]  animation sequence counter
Ei[][5]  animation sequence index
Ei[][15] bullet retrigger time
Ei[][16] bullet retrigger counter
Ei[][17] prox width
Ei[][18] prox height
Ei[][19] prox depth
Ei[][20] height above player
Efi[][5] max x speed
Efi[][6] x accel
Efi[][7] bullet speed
Efi[][8] flap yinc scaler



*/












































