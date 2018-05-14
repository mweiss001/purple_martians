#include "pm.h"


// global variables

int EXint, EYint, EN;

void enemy_killed(int EN);
void enemy_player_hit_proc(int EN);
int enemy_data(int x_pos, int y_pos)
{
   extern int e_num_of_type[50];
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
   al_draw_rotated_bitmap(memory_bitmap[Ei[e][1]], 10, 10, 10, 10, rot, flags);
//   if (Ei[e][0] == 8) // show trakbot mode
//      al_draw_textf(font, palette_color[10], 10, 6, ALLEGRO_ALIGN_CENTER, "%d" ,Ei[e][5] );
}


void draw_enemy(void)
{
   al_set_target_bitmap(level_buffer);
   for (int e=0; e<100; e++)
      if (Ei[e][0])  // if enemy active
      {
         EXint = al_fixtoi(Efi[e][0]);
         EYint = al_fixtoi(Efi[e][1]);

         int flags = 0;
         if (Ei[e][2] == 0) flags = ALLEGRO_FLIP_HORIZONTAL;
         if (Ei[e][2] == 1) flags = 0;
         if (Ei[e][2] == 2) flags = ALLEGRO_FLIP_VERTICAL;
         if (Ei[e][2] == 3) flags = ALLEGRO_FLIP_VERTICAL & ALLEGRO_FLIP_HORIZONTAL;

         float rot = al_fixtof(al_fixmul(Efi[e][14], al_fixtorad_r));
         float sc = al_fixtof(Efi[e][12]);
         al_draw_scaled_rotated_bitmap(memory_bitmap[Ei[e][1]], 10, 10, EXint+10, EYint+10, sc, sc, rot, flags);


//         if ((Ei[e][0] == 7) || (Ei[e][0] == 9))// show podzilla or cloner trigger box
//         {
//            int x1 = Ei[e][11] * 20;
//            int y1 = Ei[e][12] * 20;
//            int x2 = Ei[e][13] * 20 + 20;
//            int y2 = Ei[e][14] * 20 + 20;
//            al_draw_rectangle(x1, y1, x2, y2, palette_color[10], 1);
//         }





/*
         // test enemy collision boxes
         if (Ei[e][0] == 6) // canon
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
               al_draw_rectangle(x1-il, y1-il, x1+il, y1+il, palette_color[color], 1);
            }
         }

         if (Ei[e][0] == 9) // cloner
         {
            #ifdef CLONERLINES
            // show counter
            int cx = al_fixtoi(Efi[e][0]) + 10;     // middle of cloner
            int cy = al_fixtoi(Efi[e][1]) - 8;      // above cloner
            //int cy = al_fixtoi(Efi[e][1])+24;     // below cloner
            //int cy = al_fixtoi(Efi[e][1])+8;      // middle of cloner
            //sprintf(msg, "%d" ,Ei[e][7] );
            textout_centre_ex(level_buffer, font, msg, cx, cy, palette_color[10], 0); // red
            //al_draw_textf(font, palette_color[10], cx, cy, ALLEGRO_ALIGN_CENTER, "%d" ,Ei[e][7] );



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


            al_fixed ratio = fixdiv(al_itofix(Ei[e][7]), al_itofix(Ei[e][6])) * 10;

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
#endif
          }

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
*/

      } // end of if enemy active
}


void enemy_collision(void)
{
   extern int pbullet[50][6];
   extern int pm_bullet_collision_box;
   num_enemy = 0; // count enemies

   for (int e=0; e<100; e++)
   {

      int EXint = al_fixtoi(Efi[e][0]);
      int EYint = al_fixtoi(Efi[e][1]);
      if ((Ei[e][0]) && (Ei[e][0] != 99)) // if enemy active and not deathcount
      {
         num_enemy++; // count enemies
         // check for collision with player's bullets
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
                  int p = pbullet[c][1];
                  Ei[e][31] = 1;                       // flag that this enemy got shot with bullet
                  players[p].num_hits++;   // add to number of hits the player has
                  pbullet[c][0] = 0;                   // bullet dies
               }
            }

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

               if ((px > ex1) && (px < ex2) && (py > ey1) && (py < ey2))
               {
                  // player got hit
                  Ei[e][22] = p+1;
               }
            }
      }
   }
}


//--12--flapper-----------------------------------------------------------------------------

//   Ei[EN][0] = type 12 "flapper"
//   Ei[EN][1] = draw bitmap
//   Ei[EN][2] = hflip - right(1) or left(0)
//   Ei[EN][3] = base animation sequence

//   Ei[EN][4] = animation sequence counter
//   Ei[EN][5] = animation sequence index
//   Ei[EN][6]


//   Ei[EN][15] = bullet retrigger time
//   Ei[EN][16] = bullet retrigger counter

//   Ei[EN][17] = prox width
//   Ei[EN][18] = prox height
//   Ei[EN][19] = prox depth
//   Ei[EN][20] = height above player



//   Ei[EN][30] = collision box
//   Ei[EN][31] = health bonus



//   Efi[EN][0]  = x pos
//   Efi[EN][1]  = y pos
//   Efi[EN][2]  = x inc
//   Efi[EN][3]  = y inc
//   Efi[EN][4]  = health decrement
//   Efi[EN][5]  = max x speed
//   Efi[EN][6]  = x accel
//   Efi[EN][7]  = bullet speed
//   Efi[EN][8]  = flap yinc scaler
//   Efi[EN][9]  =
//   Efi[EN][10] =
//   Efi[EN][11] =
//   Efi[EN][12] = // scale
//   Efi[EN][13] =
//   Efi[EN][14] =
//   Efi[EN][15] =





int find_closest_player_flapper(int EN, int dir);

void enemy_flapper(void)
{
   if (Ei[EN][31]) // hit
   {
      enemy_killed(EN);
      return;
   }
   enemy_player_hit_proc(EN);


   int p = -1; // default if none found in range

   if (Ei[EN][2])  // move right
   {
      p = find_closest_player_flapper(EN, 1);
      Efi[EN][2] += Efi[EN][6];                             // accel
      if (Efi[EN][2] > Efi[EN][5]) Efi[EN][2] = Efi[EN][5]; // max speed
      Efi[EN][0] += Efi[EN][2];                             // apply xinc

      EXint = al_fixtoi(Efi[EN][0]);
      EYint = al_fixtoi(Efi[EN][1]);
      if (is_right_solid(EXint, EYint, 1))
      {
         Efi[EN][0] -= Efi[EN][2];    // take back last move
         Efi[EN][2] = -Efi[EN][5]/3;  // set accel to bounce back with 1/3 max accel
         Ei[EN][2] = 0;               // change direction
      }
      // try to shoot right
      if (--Ei[EN][16] < 0)
      {
         if (p != -1)
         {
            fire_enemy_bulleta(EN, 62, p);
            Ei[EN][16] = Ei[EN][15]; // set new prox wait
         }
      }
   }
   if (!Ei[EN][2])  // move left
   {
      p = find_closest_player_flapper(EN, 0);
      Efi[EN][2] -= Efi[EN][6];                               // accel
      if (Efi[EN][2] < -Efi[EN][5]) Efi[EN][2] = -Efi[EN][5]; // max speed
      Efi[EN][0] += Efi[EN][2];                               // apply xinc

      EXint = al_fixtoi(Efi[EN][0]);
      EYint = al_fixtoi(Efi[EN][1]);
      if (is_left_solid(EXint, EYint, 1))
      {
         Efi[EN][0] -= Efi[EN][2];   // take back last move
         Efi[EN][2] = Efi[EN][5]/3;  // set accel to bounce back with 1/3 max accel
         Ei[EN][2] = 1;              // change direction
      }
      // try to shoot left
      if (--Ei[EN][16] < 0)
      {
         if (p != -1)
         {
            fire_enemy_bulleta(EN, 62, p);
            Ei[EN][16] = Ei[EN][15]; // set new prox wait
         }

       }
   }
//   printf("dir:%d  xinc :%3.2f  \n", Ei[EN][6], al_fixtof(Efi[EN][2]) );




   al_fixed yinc = al_itofix(0);  // yinc for this pass


   p = find_closest_player(EN); // always seek in y axis (comment out this line to only seek if player in trigger box)
   if (p != -1) // only seek in y axis if valid player in prox
   {
      int height_above_player = Ei[EN][20];
      if (EYint < al_fixtoi(players[p].PY)+10 - height_above_player) yinc += Efi[EN][3];
      if (EYint > al_fixtoi(players[p].PY)+10 - height_above_player) yinc -= Efi[EN][3];
   }




   Ei[EN][4]++; // my ans counter
   if (Ei[EN][4] > 22) Ei[EN][4] = 0;

   int a=0, so, yo;

   if (Ei[EN][4] == a++) { so = 0; yo = -3; } // wings at bottom
   if (Ei[EN][4] == a++) { so = 0; yo = -4; }
   if (Ei[EN][4] == a++) { so = 0; yo = -3; }
   if (Ei[EN][4] == a++) { so = 0; yo = -3; }
   if (Ei[EN][4] == a++) { so = 0; yo = -2; }
   if (Ei[EN][4] == a++) { so = 0; yo = -1; }
   if (Ei[EN][4] == a++) { so = 0; yo = +0; }
   if (Ei[EN][4] == a++) { so = 0; yo = +1; }

   if (Ei[EN][4] == a++) { so = 1; yo = +2; } // wings 1/2 bottom coming up
   if (Ei[EN][4] == a++) { so = 1; yo = +2; }
   if (Ei[EN][4] == a++) { so = 1; yo = +2; }

   if (Ei[EN][4] == a++) { so = 2; yo = +2; } // wings 1/2 up coming up
   if (Ei[EN][4] == a++) { so = 2; yo = +2; }
   if (Ei[EN][4] == a++) { so = 2; yo = +2; }

   if (Ei[EN][4] == a++) { so = 3; yo = +2; } // wings at top
   if (Ei[EN][4] == a++) { so = 3; yo = +2; }
   if (Ei[EN][4] == a++) { so = 3; yo = +2; }

   if (Ei[EN][4] == a++) { so = 4; yo = +2; } // wings 1/2 up coming down
   if (Ei[EN][4] == a++) { so = 4; yo = +1; }
   if (Ei[EN][4] == a++) { so = 4; yo = -0; }

   if (Ei[EN][4] == a++) { so = 5; yo = -1; } // wings 1/2 down coming down
   if (Ei[EN][4] == a++) { so = 5; yo = -2; }
   if (Ei[EN][4] == a++) { so = 5; yo = -3; }


   // show if empty or has bullet ready
   if (Ei[EN][16] < 0) Ei[EN][3] = 61;
   else Ei[EN][3] = 60;
   Ei[EN][1] = zz[5+so][Ei[EN][3]];               // set shape from ans and shape offset

   al_fixed fyo = Efi[EN][8] * yo;                   // scale flap yinc

   yinc += fyo;                                   // add flap yinc to other yinc
   Efi[EN][1] += yinc;                            // apply yinc
   EYint = al_fixtoi(Efi[EN][1]);

   if (yinc < al_itofix(0))
      if (is_up_solid(EXint, EYint, 1))
         Efi[EN][1] -= yinc; // take back move
   if (yinc > al_itofix(0))
      if (is_down_solid(EXint, EYint, 1))
         Efi[EN][1] -= yinc; // take back move

}




/* show whats going on...
   printf("step:%d  so:%d  yo:%3.2f\n",Ei[EN][4], so, al_fixtof(fyo));
   if (yo < 0) Ei[EN][20] += yo;
   if (yo > 0) Ei[EN][21] += yo;

   if (Ei[EN][4] == 0)
   {
      printf("up_count%d down_count:%d\n",Ei[EN][20], Ei[EN][21]);
      Ei[EN][20] = 0;
      Ei[EN][21] = 0;
   }
*/


/*

   // follow in y axis
   if (EYint < al_fixtoi(players[p].PY) - height_above_player)
   {
      Efi[EN][1] += Efi[EN][3];
      EYint = al_fixtoi(Efi[EN][1]);
      if (is_down_solid(EXint, EYint, 1)) Efi[EN][1] -= Efi[EN][3]; // take back move
   }
   if (EYint > al_fixtoi(players[p].PY) - height_above_player)
   {
      Efi[EN][1] -= Efi[EN][3];
      EYint = al_fixtoi(Efi[EN][1]);
      if (is_up_solid(EXint, EYint, 1)) Efi[EN][1] += Efi[EN][3]; // take back move
   }

*/



/*


 // bounce in x axis
   if ((Efi[EN][2]) > al_itofix(0))  // move right
   {
      int prox = Ei[EN][17];
      Ei[EN][2] = 1; // h_flip to face right
      // try to shoot right
      if (--Ei[EN][16] < 0)
         if (EYint+Ei[EN][18] > al_fixtoi(players[p].PY) && (EYint-Ei[EN][19]) < al_fixtoi(players[p].PY))
            if (EXint < al_fixtoi(players[p].PX) && (EXint + prox) > al_fixtoi(players[p].PX) )
            {
               fire_enemy_bulleta(EN, 62, p);
               Ei[EN][16] = Ei[EN][15]; // set new prox wait
            }
     Efi[EN][0] += Efi[EN][2];
     EXint = al_fixtoi(Efi[EN][0]);
     EYint = al_fixtoi(Efi[EN][1]);
     if (is_right_solid(EXint, EYint, 1))
     {
        Efi[EN][0] -= Efi[EN][2]; // take back last move
        Efi[EN][2] =- Efi[EN][2]; // bounce
        EXint = al_fixtoi(Efi[EN][0]);
     }
   }
   else if ((Efi[EN][2]) < al_itofix(0))  // move left
   {
      int prox = Ei[EN][17];
      Ei[EN][2] = 0; // no h_flip
      // try to shoot left
      if (--Ei[EN][16] < 0)
         if (EYint+Ei[EN][18] > al_fixtoi(players[p].PY) && (EYint-Ei[EN][19]) < al_fixtoi(players[p].PY))
            if (EXint-prox < al_fixtoi(players[p].PX) &&  (EXint) > al_fixtoi(players[p].PX) )
            {
               fire_enemy_bulleta(EN, 62, p);
               Ei[EN][16] = Ei[EN][15]; // set new prox wait
            }
      Efi[EN][0] += Efi[EN][2];
      EXint = al_fixtoi(Efi[EN][0]);
      EYint = al_fixtoi(Efi[EN][1]);
      if (is_left_solid(EXint, EYint, 1))
      {
         Efi[EN][0] -= Efi[EN][2]; // take back last move
         Efi[EN][2] =- Efi[EN][2]; // bounce
         EXint = al_fixtoi(Efi[EN][0]);
      }
   }

*/


/*
   int b = Ei[EN][3]; // ans
   int c = zz[4][b];  // num of shapes in seq
   int x = (EXint/5) % c;
   Ei[EN][1] = zz[x+5][b];
*/


/*
   Ei[EN][4]++; // my ans counter
   if (Ei[EN][4] > 2)
   {
      Ei[EN][4] = 0;
      Ei[EN][5]++; // current sequence
      if (Ei[EN][5] > 5) E[EN][5] = 0;
   }

   int so = Ei[EN][5]; // current sequence


   Ei[EN][1] = zz[5+so][Ei[EN][3]];


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
   if ((Ei[EN][4] >= 0) && (Ei[EN][4] < 8)) // coast (0-7)
   {
      so = 0;  // wings at bottom
      yo = +1;
   }

   if ((Ei[EN][4] > 7) && (Ei[EN][4] < 11)) // wings coming up (8-10)
   {
      so = 1;   // wings 1/2 bottom coming up
      yo = +2;
   }

   if ((Ei[EN][4] > 10) && (Ei[EN][4] < 14)) // wings coming up (11-13)
   {
      so = 2; // wings 1/2 top coming up
      yo = +2;
   }

   if ((Ei[EN][4] > 13) && (Ei[EN][4] < 17)) // wings at top (14-16)
   {
      so = 3; // wings at top
      yo = +2;
   }

   if ((Ei[EN][4] > 16) && (Ei[EN][4] < 20)) // wings coming down (17-19)
   {
      so = 4; // wings 1/2 up coming down
      yo = -4;
   }

   if ((Ei[EN][4] > 16) && (Ei[EN][4] < 20)) // wings coming down (20-22)
   {
      so = 5; // wings 1/2 down coming down
      yo = -4;
   }
*/












//--11--block walker-----------------------------------------------------------------------------
//      Ei[EN][6] = jump wait (0=none)
//      Ei[EN][7] = jump when player above
//      Ei[EN][8] = follow(0) or bounce(1)
//      Ei[EN][11] = jump before hole
//      Ei[EN][12] = jump before wall
//      Efi[EN][8] = fall and fallcount
//      Efi[EN][9] = jump and jumpcount */
//      Ei[EN][18] = bullet trigger box y1
//      Ei[EN][19] = bullet trigger box y2

void enemy_block_walker(void)
{
   enemy_player_hit_proc(EN);
   if (Ei[EN][31]) // hit
   {
      int tx = EXint/20;
      int ty = EYint/20;

      l[tx][ty] = 168;
//      blit(memory_bitmap[168], l2000, 0, 0, (tx*20), (ty*20), 20, 20);

   al_set_target_bitmap(l2000);
   al_draw_filled_rectangle(tx*20, ty*20, tx*20+20, ty*20+20, palette_color[0]);
   al_draw_bitmap(memory_bitmap[168], tx*20, ty*20, 0);


      event(13, EXint, EYint, EN, 0, 0, 0);
      Ei[EN][0] = 0;
      return; // break;  to stop rest of execution
   }
   if (!Ei[EN][8]) // follow mode
   {
      int p = find_closest_player(EN);
      if (EXint < al_fixtoi(players[p].PX)) Efi[EN][2] = Efi[EN][6];
      if (EXint > al_fixtoi(players[p].PX)) Efi[EN][2] = -Efi[EN][6];
   }

   int on_solid = 0;
   if (is_down_solid(EXint, EYint, 1)) on_solid = 1;

   if ((Efi[EN][2]) > al_itofix(0))  // move right
   {
      Ei[EN][2] = 1; // no h_flip
      Efi[EN][0] += Efi[EN][2];
      EXint= al_fixtoi(Efi[EN][0]);
      if (on_solid)
      {
         if (Ei[EN][12]) // jump before wall
           if (is_right_solid(EXint+Ei[EN][12], EYint, 1)) Ei[EN][5] = -160;
         if (Ei[EN][11]) // jump before hole
            if (!is_right_solid(EXint+Ei[EN][11]-18, EYint+20, 1)) Ei[EN][5] = -160;
      }
      if (is_right_solid(EXint, EYint, 1))
      {
         Efi[EN][0] -= Efi[EN][2]; // take back last move
         if (Ei[EN][8]) Efi[EN][2] =- Efi[EN][6]; // if bounce mode set negative xinc for left
         EXint= al_fixtoi(Efi[EN][0]);
      }
   }

   if ((Efi[EN][2]) < al_itofix(0))  // move left
   {
      Ei[EN][2] = 0; // h_flip to face left
      Efi[EN][0] += Efi[EN][2];
      EXint= al_fixtoi(Efi[EN][0]);
      if (on_solid)
      {
         if (Ei[EN][12]) // jump before wall
            if (is_left_solid(EXint-Ei[EN][12], EYint, 1)) Ei[EN][5] = -160;
         if (Ei[EN][11]) // jump before hole
            if (!is_left_solid(EXint-Ei[EN][11]+18, EYint+20, 1)) Ei[EN][5] = -160;
      }
      if (is_left_solid(EXint, EYint, 1))
      {
         Efi[EN][0] -= Efi[EN][2]; // take back last move
         if (Ei[EN][8]) Efi[EN][2] = Efi[EN][6]; // if bounce mode set positive xinc for right
         EXint= al_fixtoi(Efi[EN][0]);
      }
   }

   if ((on_solid) && (Ei[EN][5] >= 0)) // solid and not jumping (falling or steady)
   {
      Efi[EN][1] -= al_itofix ((al_fixtoi(Efi[EN][1]) % 20));  // align with floor
      Efi[EN][1] = al_itofix (al_fixtoi(Efi[EN][1]));  // remove decimal
      Ei[EN][5] = 0;
   }



   if ((!on_solid) && (Ei[EN][5] >= 0)) // not solid and falling
   {
      Ei[EN][5] +=5; // gravity
      if (Ei[EN][5] < -160) Ei[EN][5] = -160; // terminal velocity

      // apply y move
      al_fixed ym = Ei[EN][5] * Efi[EN][3];
      al_fixed ym1 = ym/100;
      Efi[EN][1] += ym1;

      EYint = al_fixtoi(Efi[EN][1]);
      if (is_down_solid(EXint, EYint, 1))
      {
         on_solid = 1;
         Efi[EN][1] -= al_itofix ((al_fixtoi(Efi[EN][1]) % 20));  // align with floor
         Efi[EN][1] = al_itofix (al_fixtoi(Efi[EN][1]));  // remove decimal
         Ei[EN][5] = 0;
      }

   }
   if (Ei[EN][5] < 0) // rising or jumping
   {
      Ei[EN][5] +=5; // gravity
      if (Ei[EN][5] < -160) Ei[EN][5] = -160; // terminal velocity

      // apply y move
      al_fixed ym = Ei[EN][5] * Efi[EN][3];
      al_fixed ym1 = ym/100;
      Efi[EN][1] += ym1;

      EYint = al_fixtoi(Efi[EN][1]);
      if ((is_up_solid(EXint, EYint, 1) == 1) || (is_up_solid(EXint, EYint, 1) > 31) )
         Ei[EN][5] = 0;  // stop rising
   }


   if (on_solid)
   {
      // passcount jump
      if ((Ei[EN][6] > 0) && ((passcount % Ei[EN][6]) == 1)) Ei[EN][5] = -160;

      // check for jump if player passes above
      if (Ei[EN][7] > 0)
         for (int p=0; p<NUM_PLAYERS; p++)
            if ((players[p].active) && (!players[p].paused) )
               if ((EXint < (al_fixtoi(players[p].PX) + Ei[EN][7])) &&
                   (EXint > (al_fixtoi(players[p].PX) - Ei[EN][7])) &&
                   (EYint > al_fixtoi(players[p].PY)))
                      Ei[EN][5] = -160;
   }
   // set the bitmap and drawing mode
   int b = Ei[EN][3];      // ans
   int c = zz[4][b];         // num_of_shapes in seq
   int x;
   if (Ei[EN][4] == 0) Ei[EN][1] = Ei[EN][3]; // bmp, not ans
   if (Ei[EN][4] == 1) Ei[EN][1] = zz[0][b];    // animate with time

   if (Ei[EN][4] == 2) // animate with h move
   {
      x = (EXint/3) % c;
      Ei[EN][1] = zz[x+5][b];
   }

   if (Ei[EN][4] == 3) // animate with v move
   {
      x = (EYint/3) % c;
      Ei[EN][1] = zz[x+5][b];
   }
   if (Ei[EN][4] == 4) // animate with v and h move
   {
      x = ((EYint/3) % c) + ((EXint/3) % c);
      if (x > c) x-=c;
      Ei[EN][1] = zz[x+5][b];
   }
}



//--9--cloner-----------------------------------------------------------------------------
//      Ei[EN][6] = create wait
//      Ei[EN][7] = create wait counter
//      Ei[EN][8] = trigger mode (0 = wait, 1=reset, 2=immed)
//      Ei[EN][11] = trigger box x1
//      Ei[EN][12] = trigger box y1
//      Ei[EN][13] = trigger box x2
//      Ei[EN][14] = trigger box y2
//      Ei[EN][15] = copy box x
//      Ei[EN][16] = copy box y
//      Ei[EN][17] = dest box x
//      Ei[EN][18] = dest box y
//      Ei[EN][19] = copy box width
//      Ei[EN][20] = copy box height

void enemy_cloner(void)
{
   al_fixed x1 = al_itofix(Ei[EN][15]*20-2);    // source
   al_fixed y1 = al_itofix(Ei[EN][16]*20-2);
   al_fixed x3 = al_itofix(Ei[EN][17]*20-2);    // destination
   al_fixed y3 = al_itofix(Ei[EN][18]*20-2);
   al_fixed w =  al_itofix(Ei[EN][19]*20);     // width
   al_fixed h =  al_itofix(Ei[EN][20]*20);     // height
   al_fixed x2 = x1 + w;
   al_fixed y2 = y1 + h;

   // trigger box
   int x4 = Ei[EN][11]*20 - 10;
   int y4 = Ei[EN][12]*20 - 10;
   int x5 = Ei[EN][13]*20 + 10;
   int y5 = Ei[EN][14]*20 + 10;


   al_fixed ratio = al_fixdiv(al_itofix(Ei[EN][7]), al_itofix(Ei[EN][6])) * 10;

   int b = 10 - al_fixtoi(ratio);
   Ei[EN][2] = 1;
   Ei[EN][1] = zz[5+b][53];

   if (Ei[EN][31]) // hit
   {
      enemy_killed(EN);
      return;
   }
   enemy_player_hit_proc(EN);

   if (Ei[EN][5] == 1) // in trig box
      switch (Ei[EN][8])
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
                if (--Ei[EN][7] < 0)  Ei[EN][5] = 2; // time for create
             }
             else Ei[EN][5] = 0; // mode 0 - out of box
          break;
      }

   if (Ei[EN][5] == 0) // not in trig box last time
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
         switch (Ei[EN][8]) // trigger mode
         {
            case 0: Ei[EN][5] = 1; break; // set mode 1 - in box

             // reset when retriggered
            case 1: Ei[EN][7] = Ei[EN][6]; // reset counter
                     Ei[EN][5] = 1; break; // set mode 1 - in box

             // immediate when retriggered
            case 2:  Ei[EN][7] = 0; // reset counter
                      Ei[EN][5] = 1; break; // set mode 1 - in box
         }
      }
   }

   if (Ei[EN][5] == 2)  // mode 2 - create
   {

      int is_block_empty(int x, int y, int test_block, int test_item, int test_enemy);

      Ei[EN][5] = 1; // set mode 1 - in trig box
      Ei[EN][7] = Ei[EN][6]; // reset counter
      Ei[EN][1] = zz[15][53]; // full red
      int no=0, cl=Ei[EN][10]; // limit on number of created objects
      if (cl)
      {
         // count number of active objects tagged by this cloner
         for (int x=0; x<100; x++)
            if ((Ei[x][0]) && (Ei[x][28] == 1000 + EN)) no++;
         for (int x=0; x<500; x++)
            if ((item[x][0]) && (item[x][15] == 1000 + EN)) no++;
      }
      else cl = 600; // no limit is same as max limit
      if (no < cl) // if number of objects < create limit
      {
         event(14, EXint, EYint, 0, 0, 0, 0);
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
                           Ei[c][27] = Ei[EN][9]; // set time to live
                           Ei[c][28] = 1000+EN;   // tag with cloner item id


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
                           if (item[b][14] == 0) item[c][14] = Ei[EN][9]; // otherwise set time to live from cloner

                           item[c][15] = 1000+EN;   // tag with cloner item id
                           c = 500; // end loop
                           no++;    // one more object created
                        }
                  }
               }
      }
   } // end of create
}


void set_trakbot_mode(int EN, int mode)
{
   Ei[EN][5] = mode;
   switch (mode)
   {
      case 0:                                                  // floor right
         Ei[EN][2] = 1;                                        // no h flip
         Efi[EN][14] = al_itofix(0);                              // rotation = 0 deg
      break;
      case 1:                                                  // rwall up
         Ei[EN][2] = 1;                                        // no h flip
         Efi[EN][14] = al_itofix(192);                            // rotation = 270 deg
      break;
      case 2:                                                  // ceil left
         Ei[EN][2] = 1;                                        // no h flip
         Efi[EN][14] = al_itofix(128);                            // 180 deg
      break;
      case 3:                                                  // lwall down
         Ei[EN][2] = 1;                                        // no h flip
         Efi[EN][14] = al_itofix(64);                             // 90 deg
      break;
      case 4:                                                  // floor left
         Ei[EN][2] = 0;                                        // h flip
         Efi[EN][14] = al_itofix(0);                              // rotation = 0 deg
      break;
      case 5:                                                  // lwall up
         Ei[EN][2] = 0;                                        // h flip
         Efi[EN][14] = al_itofix(64);                             // 90 deg
      break;
      case 6:                                                  // ceil right
         Ei[EN][2] = 0;                                        // h flip
         Efi[EN][14] = al_itofix(128);                            // 180 deg
      break;
      case 7:                                                  // rwall down
         Ei[EN][2] = 0;                                        // h flip
         Efi[EN][14] = al_itofix(192);                            // 270 deg
      break;
   }
}



//--8--trakbot-----------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------


//     Ei[EN][0] = type
//     Ei[EN][1] = shape
//     Ei[EN][2] = draw_type
//     Ei[EN][3] = ans
//     Ei[EN][4] = mode
//     Ei[EN][5] =
//     Ei[EN][6] =
//     Ei[EN][7] = drop mode 1 = yes
//     Ei[EN][8] =


//     Ei[EN][15] = shoot holdoff value
//     Ei[EN][16] = shoot holdoff counter
//     Ei[EN][17] = prox


//     Ei[EN][22] = player hit
//     Ei[EN][23] = player hit retrigger
//     Ei[EN][24] = bullet bonus
//     Ei[EN][25] = health bonus
//     Ei[EN][26] = bonus type
//     Ei[EN][27] = bonus shape
//     Ei[EN][28] = number of extra hits to kill
//     Ei[EN][30] = death loop count
//     Ei[EN][31] = enemy hit

//     Efi[EN][0] = x
//     Efi[EN][1] = y
//     Efi[EN][2] = xinc
//     Efi[EN][3] = yinc
//     Efi[EN][4] = LIFE decrement

//     Efi[EN][7] = bullet speed

//     Efi[EN][11] = scale multiplier
//     Efi[EN][12] = scale;
//     Efi[EN][13] = rot inc


al_fixed    is_up_solidfm(al_fixed x, al_fixed y, int move, int dir );
al_fixed  is_down_solidfm(al_fixed x, al_fixed y, int move, int dir );
al_fixed  is_left_solidfm(al_fixed x, al_fixed y, int move, int dir );
al_fixed is_right_solidfm(al_fixed x, al_fixed y, int move, int dir );


void enemy_trakbot(void)
{
   al_fixed mv;
   int dp = 0; // debug print
   int bullet_request = 0;
   int mode = Ei[EN][5];
   int prox = Ei[EN][17];

   if (Ei[EN][31]) // hit
   {
      enemy_killed(EN);
      return;
   }
   enemy_player_hit_proc(EN);

   if (--Ei[EN][16] < 0)  // bullet retrigger wait
   {
      bullet_request = 1; // try to shoot
      Ei[EN][3] = 17;     // show trakbot with bullet
   }
   else Ei[EN][3] = 18;   // show trakbot without bullet


   if (Ei[EN][4] != 0)   // if falling
   {
      if (dp) printf("mode:fall  x:%1.2f  y:%1.2f\n", al_fixtof(Efi[EN][0]), al_fixtof(Efi[EN][1])); //   printf(" [%.2f] ", al_fixtof(speed));

      Ei[EN][4] += 5;                                             // add acceleration to fsll
      if (Ei[EN][4] > 160)  Ei[EN][4] = 160;                      // terminal velocity
      al_fixed fall_yinc = (Efi[EN][3] * Ei[EN][4]) / 100;           // (fix * int) / int
      mv = is_down_solidfm(Efi[EN][0], Efi[EN][1], fall_yinc, 0); // will we hit floor during this move?
      if (mv < fall_yinc) Ei[EN][4] = 0;                          // end fall if allowed move less than requested move
      Efi[EN][1] += mv;                                           // move the allowed amount
      if (Efi[EN][1] > al_itofix(1999)) Ei[EN][0]=0;                 // kill if falls past bottom of level

      // seek player while falling
      int p = find_closest_player(EN);
      al_fixed ex = Efi[EN][0];
      al_fixed px = players[p].PX;

      if (px > ex) mode = 0;
      else mode = 4;
      set_trakbot_mode(EN, mode);
   }
   else // not fall
   {
      if ((Ei[EN][7]) && ((mode == 2) || (mode == 6))) // if drop mode on and trakbot on ceiling
      {
         al_fixed ex = Efi[EN][0];
         al_fixed ey = Efi[EN][1];
         al_fixed ew = al_itofix(15);
         for (int p=0; p<NUM_PLAYERS; p++) // if any player passes underneath
            if ((players[p].active) && (!players[p].paused))
            {
               al_fixed px = players[p].PX;
               al_fixed py = players[p].PY;
               if ((px < ex + ew) && (px > ex - ew) && (py > ey))  // if directly above player
               {
                  if (dp) printf("just dropped on player:%d\n", p);
                  Ei[EN][4] = 5; // start fall mode
                  if (mode == 2) set_trakbot_mode(EN, 4);
                  if (mode == 6) set_trakbot_mode(EN, 0);
               }
            }
      }
   }

   if (Ei[EN][4] == 0)  // if not still not fall
   {
      if (dp) printf("mode:%d  x:%1.2f  y:%1.2f - ", mode, al_fixtof(Efi[EN][0]), al_fixtof(Efi[EN][1])); //   printf(" [%.2f] ", al_fixtof(speed));
      switch (mode)
      {
         case 0:                                                  // floor right
            Ei[EN][2] = 1;                                        // no h flip
            Efi[EN][14] = al_itofix(0);                              // rotation = 0 deg
            if (dp) { if (is_down_solidfm(Efi[EN][0], Efi[EN][1], al_itofix(1), 0) < al_itofix(1)) printf(" - 0 with solid floor\n"); else  printf(" - 0 with NOT solid floor\n"); }
            mv = is_down_solidfm(Efi[EN][0], Efi[EN][1], Efi[EN][2], 1);
            if (mv < Efi[EN][2]) // allowed move less than requested move
            {
               if (dp) printf("will lose floor during this move - mv%1.2f \n", al_fixtof(mv));
               Efi[EN][0] += mv;                                  // move the allowed amount
               mode = 3;                                          // next mode (lwall down) (cw)
               Efi[EN][14] = al_itofix(32);                          // 45 deg
               Efi[EN][1] += al_itofix(1);                           // move down 1 to catch new wall
               break;
            }
            mv = is_right_solidfm(Efi[EN][0], Efi[EN][1], Efi[EN][2], 0);
            if (mv < Efi[EN][2]) // allowed move less than requested move
            {
               if (dp) printf("will hit wall during this move - mv%1.2f \n", al_fixtof(mv));
               Efi[EN][0] += mv;                                  // move the allowed amount
               mode = 1;                                          // next mode (rwall up) (ccw)
               Efi[EN][14] = al_itofix(224);                         // 315 deg
               break;
            }
            Efi[EN][0] += mv;  // if neither of these matched, move the allowed amount
         break;
         case 1:                                                  // rwall up
            Ei[EN][2] = 1;                                        // no h flip
            Efi[EN][14] = al_itofix(192);                            // rotation = 270 deg
            if (dp) { if (is_right_solidfm(Efi[EN][0], Efi[EN][1], al_itofix(1), 0) < al_itofix(1)) printf(" - 1 with solid r wall\n"); else  printf(" - 1 with NOT solid r wall\n"); }
            mv = is_right_solidfm(Efi[EN][0], Efi[EN][1], Efi[EN][3], -1);
            if (mv < Efi[EN][3]) // allowed move less than requested move
            {
               if (dp) printf("will lose rwall during this move - mv%1.2f \n", al_fixtof(mv));
               Efi[EN][1] -= mv;                                  // move the allowed amount
               mode = 0;                                          // next mode (floor right) (cw)
               Efi[EN][14] = al_itofix(224);                         // 315 deg
               Efi[EN][0] += al_itofix(1);                           // move right 1 to catch new wall
               break;
            }
            mv = is_up_solidfm(Efi[EN][0], Efi[EN][1], Efi[EN][3], 0);
            if (mv < Efi[EN][3]) // allowed move less than requested move
            {
               if (dp) printf("will hit ceiling during this move - mv%1.2f \n", al_fixtof(mv));
               mode = 2;                                          // next mode (ceil left) (ccw)
               Efi[EN][14] = al_itofix(160);                         // 225 deg
               Efi[EN][1] -= mv;                                  // move the allowed amount
               break;
            }
            Efi[EN][1] -= mv;  // if neither of these matched, move the allowed amount
         break;
         case 2:                                                  // ceil left
            Ei[EN][2] = 1;                                        // no h flip
            Efi[EN][14] = al_itofix(128);                            // 180 deg
            if (dp) { if (is_up_solidfm(Efi[EN][0], Efi[EN][1], al_itofix(1), 0) < al_itofix(1)) printf(" - 2 with solid ceil\n"); else  printf(" - 2 with NOT solid ceil\n"); }
            mv = is_up_solidfm(Efi[EN][0], Efi[EN][1], Efi[EN][2], -1);
            if (mv < Efi[EN][2]) // allowed move less than requested move
            {
               if (dp) printf("will lose ceiling during this move - mv%1.2f \n", al_fixtof(mv));
               Efi[EN][0] -= mv;                                  // move the allowed amount
               mode = 1;                                          // next mode (rwall up) (cw)
               Efi[EN][14] = al_itofix(160);                         // 225 deg
               Efi[EN][1] -= al_itofix(1);                           // move up 1 to catch new wall
               break;
            }
            mv = is_left_solidfm(Efi[EN][0], Efi[EN][1], Efi[EN][2], 0);
            if (mv < Efi[EN][2]) // allowed move less than requested move
            {
               if (dp) printf("will hit lwall during this move - mv%1.2f \n", al_fixtof(mv));
               mode = 3;                                          // next mode (lwall down) (ccw)
               Efi[EN][14] = al_itofix(96);                          // 135 deg
               Efi[EN][0] -= mv;                                  // move the allowed amount
               break;
            }
            Efi[EN][0] -= mv;  // if neither of these matched, move the allowed amount
         break;
         case 3:                                                  // lwall down
            Ei[EN][2] = 1;                                        // no h flip
            Efi[EN][14] = al_itofix(64);                             // 90 deg
            if (dp) { if (is_left_solidfm(Efi[EN][0], Efi[EN][1], al_itofix(1), 0) < al_itofix(1)) printf(" - 3 with solid lwall\n"); else  printf(" - 3 with NOT solid lwall\n"); }
            mv = is_left_solidfm(Efi[EN][0], Efi[EN][1], Efi[EN][3], 1);
            if (mv < Efi[EN][3]) // allowed move less than requested move
            {
               if (dp) printf("will lose lwall during this move - mv%1.2f \n", al_fixtof(mv));
               Efi[EN][1] += mv;                                  // move the allowed amount
               mode = 2;                                          // next mode  (ceil left) (cw)
               Efi[EN][14] = al_itofix(96);                          // 135 deg
               Efi[EN][0] -= al_itofix(1);                           // move left 1 to catch new wall
               break;
            }
            mv = is_down_solidfm(Efi[EN][0], Efi[EN][1], Efi[EN][3], 0);
            if (mv < Efi[EN][3]) // allowed move less than requested move
            {
               if (dp) printf("will hit floor during this move - mv%1.2f \n", al_fixtof(mv));
               mode = 0;                                          // next mode (floor right) (ccw)
               Efi[EN][14] = al_itofix(32);                          // 45 deg
               Efi[EN][1] += mv;                                  // move the allowed amount
               break;
            }
            Efi[EN][1] += mv;  // if neither of these matched, move the allowed amount
         break;
         case 4:                                                  // floor left
            Ei[EN][2] = 0;                                        // h flip
            Efi[EN][14] = al_itofix(0);                              // rotation = 0 deg
            if (dp) { if (is_down_solidfm(Efi[EN][0], Efi[EN][1], al_itofix(1), 0) < al_itofix(1)) printf(" - 4 with solid floor\n"); else  printf(" - 4 with NOT solid floor\n"); }

            mv = is_down_solidfm(Efi[EN][0], Efi[EN][1], Efi[EN][2], -1);
            if (mv < Efi[EN][2])                                  // allowed move less than requested move
            {
               Efi[EN][0] -= mv;                                  // move the allowed amount
               mode = 7;                                          // next mode (rwall down) (ccw)
               Efi[EN][14] = al_itofix(224);                         // 315 deg
               Efi[EN][1] += al_itofix(1);                           // move down 1 to catch new wall
               if (dp) printf("lost floor during this move - mv%1.2f \n", al_fixtof(mv));
               break;
            }
            mv = is_left_solidfm(Efi[EN][0], Efi[EN][1], Efi[EN][2], 0);
            if (mv < Efi[EN][2])                                  // allowed move less than requested move
            {
               Efi[EN][0] -= mv;                                  // move the allowed amount
               mode = 5;                                          // next mode (lwall up) (cw)
               Efi[EN][14] = al_itofix(32);                          // 45 deg
               if (dp) printf("hit lwall during this move - mv%1.2f \n", al_fixtof(mv));
               break;
            }
            Efi[EN][0] -= mv;  // if neither of these matched, move the allowed amount
         break;
         case 5:                                                  // lwall up
            Ei[EN][2] = 0;                                        // h flip
            Efi[EN][14] = al_itofix(64);                             // 90 deg
            if (dp) { if (is_left_solidfm(Efi[EN][0], Efi[EN][1], al_itofix(1), 0) < al_itofix(1)) printf(" - 5 with solid left\n"); else  printf(" - 5 with NOT solid left\n"); }
            mv = is_left_solidfm(Efi[EN][0], Efi[EN][1], Efi[EN][3], -1);
            if (mv < Efi[EN][3])                                  // allowed move less than requested move
            {
               Efi[EN][1] -= mv;                                  // move the allowed amount
               mode = 4;                                          // next mode (floor left) (ccw)
               Efi[EN][14] = al_itofix(32);                          // 45 deg
               Efi[EN][0] -= al_itofix(1);                           // move left 1 to catch new wall
               if (dp) printf("lost lwall during this move - mv%1.2f \n", al_fixtof(mv));
               break;
            }
            mv = is_up_solidfm(Efi[EN][0], Efi[EN][1], Efi[EN][3], 0);
            if (mv < Efi[EN][3])                                  // allowed move less than requested move
            {
               Efi[EN][1] -= mv;                                  // move the allowed amount
               mode = 6;                                          // next mode (ceil right) (cw)
               Efi[EN][14] = al_itofix(96);                          // 135 deg
               if (dp) printf("hit ceil during this move - mv%1.2f \n", al_fixtof(mv));
               break;
            }
            Efi[EN][1] -= mv;  // if neither of these matched, move the allowed amount
         break;
         case 6:                                                  // ceil right
            Ei[EN][2] = 0;                                        // h flip
            Efi[EN][14] = al_itofix(128);                            // 180 deg
            if (dp) { if (is_up_solidfm(Efi[EN][0], Efi[EN][1], al_itofix(1), 0) < al_itofix(1)) printf(" - 6 with solid ceil\n"); else  printf(" - 6 with NOT solid ceil\n"); }
            mv = is_up_solidfm(Efi[EN][0], Efi[EN][1], Efi[EN][2], 1);
            if (mv < Efi[EN][2])                                  // allowed move less than requested move
            {
               Efi[EN][0] += mv;                                  // move the allowed amount
               mode = 5;                                          // next mode (lwall up ) (ccw)
               Efi[EN][14] = al_itofix(96);                          // 135 deg
               Efi[EN][1] -= al_itofix(1);                           // move up 1 to catch new wall
               if (dp) printf("lost ceil during this move - mv%1.2f \n", al_fixtof(mv));
               break;
            }
            mv = is_right_solidfm(Efi[EN][0], Efi[EN][1], Efi[EN][2], 0);
            if (mv < Efi[EN][2])                                  // allowed move less than requested move
            {
               Efi[EN][0] += mv;                                  // move the allowed amount
               mode = 7;                                          // next mode (rwall down) (cw)
               Efi[EN][14] = al_itofix(160);                         // 225 deg
               if (dp) printf("hit rwall during this move - mv%1.2f \n", al_fixtof(mv));
               break;
            }
            Efi[EN][0] += mv;  // if neither of these matched, move the allowed amount
         break;
         case 7:                                                  // rwall down
            Ei[EN][2] = 0;                                        // h flip
            Efi[EN][14] = al_itofix(192);                            // 270 deg
            if (dp) { if (is_right_solidfm(Efi[EN][0], Efi[EN][1], al_itofix(1), 0) < al_itofix(1)) printf(" - 7 with solid rwall\n"); else  printf(" - 7 with NOT solid rwall\n"); }
            mv = is_right_solidfm(Efi[EN][0], Efi[EN][1], Efi[EN][3], 1);
            if (mv < Efi[EN][3])                                  // allowed move less than requested move
            {
               Efi[EN][1] += mv;                                  // move the allowed amount
               mode = 6;                                          // next mode (ceil right) (ccw)
               Efi[EN][14] = al_itofix(160);                         // 225 deg
               Efi[EN][0] += al_itofix(1);                           // move right 1 to catch new wall
               if (dp) printf("lost rwall during this move - mv%1.2f \n", al_fixtof(mv));
               break;
            }
            mv = is_down_solidfm(Efi[EN][0], Efi[EN][1], Efi[EN][3], 0);
            if (mv < Efi[EN][3])                                  // allowed move less than requested move
            {
               Efi[EN][1] += mv;                                  // move the allowed amount
               mode = 4;                                          // next mode (floor left) (cw)
               Efi[EN][14] = al_itofix(224);                         // 315 deg
               if (dp) printf("hit floor during this move - mv%1.2f \n", al_fixtof(mv));
               break;
            }
            Efi[EN][1] += mv;  // if neither of these matched, move the allowed amount
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
         int p = find_closest_player_quad(EN, quad, prox);
         if (p != -1)
         {
            fire_enemy_bulleta(EN, 20, p);
            Ei[EN][16] = Ei[EN][15]; // set new prox wait
         }
      }

   }  // end of if !fall


   if (Ei[EN][5] == mode) Ei[EN][8] = 0;
   else
   {
      Ei[EN][5] = mode;
      if (++Ei[EN][8] > 3) // twirling
      {
         Ei[EN][8] = 0; // reset twirling counter
         if (dp) printf("twirling detected!! - set fall mode\n");
         Ei[EN][4] = 5; // start fall mode
         set_trakbot_mode(EN, 0);
      }
   }


   // set the bitmap and drawing mode
   int b = Ei[EN][3];    // ans
   if ((mode == 0) || (mode == 6)) // x+ floor right and ceil right
   {
      int ex = al_fixtoi(Efi[EN][0]);
      int s = ex % 6;
      Ei[EN][1] = zz[10-s][b];
   }
   if ((mode == 1) || (mode == 5)) // y- rwall up and lwall up
   {
      int ey = al_fixtoi(Efi[EN][1]);
      int s = ey % 6;
      Ei[EN][1] = zz[5+s][b];
   }
   if ((mode == 2) || (mode == 4)) // x- ceil left or floor left
   {
      int ex = al_fixtoi(Efi[EN][0]);
      int s = ex % 6;
      Ei[EN][1] = zz[5+s][b];
   }
   if ((mode == 3) || (mode == 7)) // y+ lwall down or rwall down
   {
      int ey = al_fixtoi(Efi[EN][1]);
      int s = ey % 6;
      Ei[EN][1] = zz[10-s][b];
   }
}

//--7--podzilla-----------------------------------------------------------------------------
//     Ei[EN][5] = mode
//     Ei[EN][6] = sequence counter
//     Ei[EN][7] = sequence limit
//     Ei[EN][8] = wait count
//     Ei[EN][9] = wait limit
//     Ei[EN][11] = trigger box x1
//     Ei[EN][12] = trigger box x1
//     Ei[EN][13] = trigger box x2
//     Ei[EN][14] = trigger box y2
void enemy_podzilla(void)
{
   if (Ei[EN][31]) // podzilla hit
   {
      enemy_killed(EN);
      return;
   }
   enemy_player_hit_proc(EN);
   if (Ei[EN][5] == 0) // mode 0 - wait for trigger
   {
      // trigger box offset
      int x1 = Ei[EN][11]*20 - 10;
      int y1 = Ei[EN][12]*20 - 10;
      int x2 = Ei[EN][13]*20 + 10;
      int y2 = Ei[EN][14]*20 + 10;

      for (int p=0; p<NUM_PLAYERS; p++)
         if ((players[p].active) && (!players[p].paused))
         {
            int px = al_fixtoi(players[p].PX);
            int py = al_fixtoi(players[p].PY);
            if ((px > x1) && (px < x2) && (py > y1) &&  (py < y2))
            {
               Ei[EN][5] = 1;          // set next mode
               Ei[EN][6] = Ei[EN][7];   // set extend counter
            }
         }
   }
   if (Ei[EN][5] == 1) // mode 1; extend
   {
      Efi[EN][0] += Efi[EN][2];
      Efi[EN][1] += Efi[EN][3];
      if (--Ei[EN][6] == 0)  // counter done?
      {
         Ei[EN][5] = 2; // set next mode
         Ei[EN][8] = passcount;
      }
   }
   if (Ei[EN][5] == 2)  // mode 2; wait then shoot
   {
      if (Ei[EN][8] < (passcount-Ei[EN][9]))
      {
         int p = find_closest_player(EN);
         fire_enemy_bulleta(EN, 54, p);
         Ei[EN][5] = 3; // set next mode
         Ei[EN][8] = passcount;
      }
   }
   if (Ei[EN][5] == 3)  // mode 3; post shoot pause
   {
      if (Ei[EN][8] < (passcount-Ei[EN][9]) )
      {
         Ei[EN][5] = 4;        // next mode
         Ei[EN][6] = Ei[EN][7]; // set retract counter
      }
   }
   if (Ei[EN][5] == 4) // mode 4; retract
   {
      Efi[EN][0] -= Efi[EN][2];
      Efi[EN][1] -= Efi[EN][3];
      if (--Ei[EN][6] == 0)  // counter done?
      Ei[EN][5] = 0; // set next mode (back to trigger)
   }

   Ei[EN][1] = zz[ 5 + (Ei[EN][6] * zz[4][15] / Ei[EN][7]) ][15];     // bitmap = counter * numshapes / limit

   if ((Ei[EN][5] == 2) || (Ei[EN][5] == 3)) // rotate to face player
   {
      int p = find_closest_player(EN);
      Efi[EN][14] = get_rot_from_PXY(EN, p);
      Ei[EN][1] = 335; // green cannon ball
   }
   else Efi[EN][14] = get_rot_from_xyinc(EN); // rotate to face direction of movement
}

//----------------------------------------------------------------------------------------
//--6--cannon-----------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
void enemy_cannon()
{
   enemy_player_hit_proc(EN);
   if (Ei[EN][31]) // enemy hit
   {
      if (Ei[EN][31] == 2) // hit type == bomb
      {
         //Ei[EN][9] -= 3;      // three hits
         //if (Ei[EN][9] < 0) Ei[EN][9] = 0;
         enemy_killed(EN); // kill instantly no matter how many hits left
         return;
      }
      if (Ei[EN][9] == 0) // dead
      {
         enemy_killed(EN);
         return;
      }
      else
      {
         Ei[EN][9]--;        // one less hit
         Ei[EN][31] = 0;     // clear hit


         Efi[EN][2] =  (Efi[EN][2]  * 1200) / 1000;  // x speed
         Efi[EN][3] =  (Efi[EN][3]  * 1200) / 1000;  // y speed
         Efi[EN][5] =  (Efi[EN][5]  * 1200) / 1000;  // seek speed
         Efi[EN][7] =  (Efi[EN][7]  * 1200) / 1000;  // bullet speed
         Efi[EN][12] = (Efi[EN][12] * 1120) / 1000;  // scale

/*         Efi[EN][2]  = fmul(Efi[EN][2],  al_ftofix(1.2));  // x speed
         Efi[EN][5]  = fmul(Efi[EN][5],  al_ftofix(1.2));  // seek speed
         Efi[EN][5]  = fmul(Efi[EN][5],  al_ftofix(1.2));  // seek speed
         Efi[EN][7]  = fmul(Efi[EN][7],  al_ftofix(1.2));  // bullet speed
         Efi[EN][12] = fmul(Efi[EN][12], al_ftofix(1.12)); // scale
*/



         Ei[EN][29] += 2; // collison box size
      }
   }

   int p = find_closest_player(EN);

   // time to shoot cannonball ?
   if (Ei[EN][15])
      if (--Ei[EN][16] < 0) // cannon shot wait
      {
         fire_enemy_bulleta(EN, 55, p);
         Ei[EN][16] = Ei[EN][15]; // reset cannon shot wait
      }

   if ((Ei[EN][8]) && (Ei[EN][7] > Ei[EN][8])) // seek
   {
      seek_set_xyinc(EN);
      Ei[EN][7] = 0;
   }


   // set bitmap
   al_fixed ratio = al_fixdiv(al_itofix(Ei[EN][16]), al_itofix(Ei[EN][15]));
   Ei[EN][1] = 412;  // green cannon by default
   if (ratio < al_ftofix(0.3))  Ei[EN][1] = 413; // less green cannon
   if (ratio < al_ftofix(0.2))  Ei[EN][1] = 414; // orange cannon
   if (ratio < al_ftofix(0.1))  Ei[EN][1] = 415; // red cannon

   if ((Efi[EN][2]) > al_itofix(0))  // move right
   {
     Efi[EN][0] += Efi[EN][2];
     EXint = al_fixtoi(Efi[EN][0]);
     if (is_right_solid(EXint, EYint, 1)) // bounce
     {
        Ei[EN][7]++; // inc bounce count
        Efi[EN][2] =- Efi[EN][2]; // reverse xinc
        Efi[EN][0] += Efi[EN][2]; // take back last move
     }
   }
   if ((Efi[EN][2]) < al_itofix(0))  // move left
   {
      Efi[EN][0] += Efi[EN][2];
      EXint = al_fixtoi(Efi[EN][0]);
      if (is_left_solid(EXint, EYint, 1)) // bounce
      {
         Ei[EN][7]++;
         Efi[EN][2] =- Efi[EN][2];
         Efi[EN][0] += Efi[EN][2];
      }
   }
   if (Efi[EN][3] > al_itofix(0)) // move down
   {
      Efi[EN][1] += Efi[EN][3];
      EYint = al_fixtoi(Efi[EN][1]);
      if (is_down_solid(EXint, EYint, 1))
      {
         Ei[EN][7]++;
         Efi[EN][3] =- Efi[EN][3];
         Efi[EN][1] += Efi[EN][3];
      }
   }
   if (Efi[EN][3] < al_itofix(0))  // move up
   {
      Efi[EN][1] += Efi[EN][3];
      EYint = al_fixtoi(Efi[EN][1]);
      if (is_up_solid(EXint, EYint, 0) == 1)
      {
         Ei[EN][7]++;
         Efi[EN][3] =- Efi[EN][3];
         Efi[EN][1] += Efi[EN][3];
      }
   }
   Efi[EN][14] = get_rot_from_PXY(EN, p);

}

//--4--bouncer-----------------------------------------------------------------------------

void enemy_bouncer()
{
   if (Ei[EN][31]) // hit
   {
      enemy_killed(EN);
      return; // break;  to stop rest of execution
   }
   enemy_player_hit_proc(EN);
   if ((Ei[EN][8]) && (Ei[EN][7] > Ei[EN][8])) // seek
   {
      Ei[EN][7] = 0;
      seek_set_xyinc(EN);
      Efi[EN][14] = get_rot_from_xyinc(EN);
      Ei[EN][3] = Ei[EN][6]; // seek ans
   }
   if (Ei[EN][7]) Ei[EN][3] = Ei[EN][5]; // main ans

   if ((Efi[EN][2]) > al_itofix(0))  // move right
   {
      Efi[EN][0] += Efi[EN][2];
      EXint = al_fixtoi(Efi[EN][0]);
      if (is_right_solid(EXint, EYint, 1)) // bounce
      {
         Ei[EN][7]++; // inc bounce count
         Efi[EN][2] =- Efi[EN][2]; // reverse xinc
         Efi[EN][0] += Efi[EN][2]; // take back last move
         Efi[EN][14] = get_rot_from_xyinc(EN);
      }
   }
   if ((Efi[EN][2]) < al_itofix(0))  // move left
   {
      Efi[EN][0] += Efi[EN][2];
      EXint = al_fixtoi(Efi[EN][0]);
      if (is_left_solid(EXint, EYint, 1)) // bounce
      {
         Ei[EN][7]++; // inc bounce count
         Efi[EN][2] =- Efi[EN][2]; // reverse xinc
         Efi[EN][0] += Efi[EN][2]; // take back last move
         Efi[EN][14] = get_rot_from_xyinc(EN);
      }
   }
   if (Efi[EN][3] > al_itofix(0)) // move down
   {
      Efi[EN][1] += Efi[EN][3];
      EYint = al_fixtoi(Efi[EN][1]);
      if (is_down_solid(EXint, EYint, 1))
      {
         Ei[EN][7]++;
         Efi[EN][3] =- Efi[EN][3];
         Efi[EN][1] += Efi[EN][3];
         Efi[EN][14] = get_rot_from_xyinc(EN);
      }
   }
   if (Efi[EN][3] < al_itofix(0))  // move up
   {
      Efi[EN][1] += Efi[EN][3];
      EYint = al_fixtoi(Efi[EN][1]);
      if (is_up_solid(EXint, EYint, 0) == 1)
      {
         Ei[EN][7]++;
         Efi[EN][3] =- Efi[EN][3];
         Efi[EN][1] += Efi[EN][3];
         Efi[EN][14] = get_rot_from_xyinc(EN);
      }
   }


   // set the bitmap from animation sequence - always with time
//   Ei[EN][1] = zz[0][Ei[EN][3]];

// changed this to make chunk netplay work good



// old
//   int old = zz[0][Ei[EN][3]];


   // ans number
   int ans = Ei[EN][3];
   // how many shapes does this animation seq have?
   int ns = zz[4][ans];

   // how many frames for each shape
   int nf = zz[3][ans]+1;

   // total seq length in frame
   int tsl = nf * ns;

   // get mod
   int pm = passcount % tsl;

   // get sequences number from mod
   int ss = pm / nf;

   // get shape
   int nw = zz[5+ss][ans];

//   printf("ns:%d nf:%d tsl:%2d pm:%2d ss:%d nw:%d old:%d\n",ns, nf, tsl, pm, ss, nw, old);

   Ei[EN][1] = nw;




}


//-------------------------------------------------------------------------------------------
//--3--archwagon-----------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------
void enemy_archwagon()
{

   if (Ei[EN][31]) // hit
   {
      enemy_killed(EN);
      return; // break;  to stop rest of execution
   }
   enemy_player_hit_proc(EN);

   if (Ei[EN][16] > 0) // shoot arrow holdoff
   {
      Ei[EN][3] = 3; // empty wagon ans
      --Ei[EN][16];
   }
   else
   {
      int swl = 10; // bullet prox window low
      int swh = 6;  // bullet prox window high
      Ei[EN][3] = 2; // wagon with arrow ans
      if (Ei[EN][2]) // attempt shoot right
         for (int p=0; p<NUM_PLAYERS; p++)
            if ((players[p].active) && (!players[p].paused) )
               if ((EXint > al_fixtoi(players[p].PX) - Ei[EN][17]) && (EXint < al_fixtoi(players[p].PX) ))
                  if ((EYint > al_fixtoi(players[p].PY) - swh) && (EYint < al_fixtoi(players[p].PY) + swl ))
                  {
                     fire_enemy_x_bullet(EN, p);
                     Ei[EN][16] = Ei[EN][15]; // set new prox wait
                  }
      if (!Ei[EN][2]) // attempt shoot left
         for (int p=0; p<NUM_PLAYERS; p++)
            if ((players[p].active) && (!players[p].paused) )
               if ((EXint > al_fixtoi(players[p].PX) ) && (EXint < al_fixtoi(players[p].PX)  + Ei[EN][17]))
                  if ((EYint > al_fixtoi(players[p].PY)  - swh) && (EYint < al_fixtoi(players[p].PY) + swl ))
                  {
                     fire_enemy_x_bullet(EN, p);
                     Ei[EN][16] = Ei[EN][15]; // set new prox wait
                  }
   }

   if (!Ei[EN][8]) // follow mode
   {
      int p = find_closest_player(EN);
      if (EXint < al_fixtoi(players[p].PX)) Efi[EN][2] = Efi[EN][6];
      if (EXint > al_fixtoi(players[p].PX)) Efi[EN][2] = -Efi[EN][6];
   }

   int on_solid = 0;
   int on_lift = 0;
   int ret = is_down_solid(EXint, EYint, 1);
   if ((ret == 1) || (ret == 2)) on_solid = 1;

   if (ret >= 32) //on lift
   {
      on_lift = 1;
      Efi[EN][1] += lifts[ret-32].fyinc ;  // move with lift
   }

   if ((Efi[EN][2]) > al_itofix(0))  // move right
   {
      Ei[EN][2] = 1; // no h_flip
      Efi[EN][0] += Efi[EN][2];
      EXint= al_fixtoi(Efi[EN][0]);
      if ((on_solid) || (on_lift))
      {
         if (Ei[EN][12]) // jump before wall
           if (is_right_solid(EXint+Ei[EN][12], EYint, 1)) Ei[EN][5] = -160;
         if (Ei[EN][11]) // jump before hole
            if (!is_right_solid(EXint+Ei[EN][11]-18, EYint+20, 1)) Ei[EN][5] = -160;
      }
      if (is_right_solid(EXint, EYint, 1))
      {
         Efi[EN][0] -= Efi[EN][2]; // take back last move
         if (Ei[EN][8]) Efi[EN][2] =- Efi[EN][6]; // if bounce mode set negative xinc for left
         EXint= al_fixtoi(Efi[EN][0]);
      }
   }

   if ((Efi[EN][2]) < al_itofix(0))  // move left
   {
      Ei[EN][2] = 0; // h_flip to face left
      Efi[EN][0] += Efi[EN][2];
      EXint= al_fixtoi(Efi[EN][0]);
      if ((on_solid) || (on_lift))
      {
         if (Ei[EN][12]) // jump before wall
            if (is_left_solid(EXint-Ei[EN][12], EYint, 1)) Ei[EN][5] = -160;
         if (Ei[EN][11]) // jump before hole
            if (!is_left_solid(EXint-Ei[EN][11]+18, EYint+20, 1)) Ei[EN][5] = -160;
      }
      if (is_left_solid(EXint, EYint, 1))
      {
         Efi[EN][0] -= Efi[EN][2]; // take back last move
         if (Ei[EN][8]) Efi[EN][2] = Efi[EN][6]; // if bounce mode set positive xinc for right
         EXint= al_fixtoi(Efi[EN][0]);
      }
   }

   if ((on_solid) && (Ei[EN][5] >= 0)) // solid and not jumping (falling or steady)
   {
      Efi[EN][1] -= al_itofix ((al_fixtoi(Efi[EN][1]) % 20));  // align with floor
      Efi[EN][1] = al_itofix (al_fixtoi(Efi[EN][1]));  // remove decimal
      Ei[EN][5] = 0;
   }

   if ((!on_solid) && (!on_lift) && (Ei[EN][5] >= 0)) // not solid and falling
   {
      Ei[EN][5] +=5; // gravity
      if (Ei[EN][5] > 160) Ei[EN][5] = 160; // terminal velocity

      // apply y move
      al_fixed ym = Ei[EN][5] * Efi[EN][3];
      al_fixed ym1 = ym/100;

      Efi[EN][1] += ym1;

      EYint = al_fixtoi(Efi[EN][1]);
      if (is_down_solid(EXint, EYint, 1))
      {
         on_solid = 1;
         Efi[EN][1] -= al_itofix ((al_fixtoi(Efi[EN][1]) % 20));  // align with floor
         Efi[EN][1] = al_itofix (al_fixtoi(Efi[EN][1]));  // remove decimal
         Ei[EN][5] = 0;
      }
   }

   if (Ei[EN][5] < 0) // rising or jumping
   {
      Ei[EN][5] +=5; // gravity
      if (Ei[EN][5] < -160) Ei[EN][5] = -160; // terminal velocity

      // apply y move
      al_fixed ym = Ei[EN][5] * Efi[EN][3];
      al_fixed ym1 = ym/100;
      Efi[EN][1] += ym1;

      EYint = al_fixtoi(Efi[EN][1]);
      if ((is_up_solid(EXint, EYint, 1) == 1) || (is_up_solid(EXint, EYint, 1) > 31) )
         Ei[EN][5] = 0;  // stop rising
   }

   if ((on_solid) || (on_lift))
   {
      // passcount jump
      if ((Ei[EN][6] > 0) && ((passcount % Ei[EN][6]) == 1)) Ei[EN][5] = -160;

      // check for jump if player passes above
      if (Ei[EN][7] > 0)
         for (int p=0; p<NUM_PLAYERS; p++)
            if ((players[p].active) && (!players[p].paused) )
               if ((EXint < (al_fixtoi(players[p].PX) + Ei[EN][7])) &&
                   (EXint > (al_fixtoi(players[p].PX) - Ei[EN][7])) &&
                   (EYint > al_fixtoi(players[p].PY)))
                      Ei[EN][5] = -160;
   }

   // set the bitmap and drawing mode
   int x;
   int b = Ei[EN][3]; // ans
   int c = zz[4][b]; // num_of_shapes in seq
   if (Ei[EN][4] == 0) Ei[EN][1] = Ei[EN][3]; // bmp, not ans
   if (Ei[EN][4] == 1) Ei[EN][1] = zz[0][b];    // animate with time
   if (Ei[EN][4] == 2) // animate with h move
   {
      if (Ei[EN][2] == 1) // right
      {
         x = (EXint/3) % c;
         Ei[EN][1] = zz[x+5][b];
      }
      if (Ei[EN][2] == 0) // left
      {
         x = (EXint/3) % c;
         Ei[EN][1] = zz[5+c-x][b];
      }
   }
   if (Ei[EN][4] == 3) // animate with v move
   {
      x = (EYint/3) % c;
      Ei[EN][1] = zz[x+5][b];
   }
   if (Ei[EN][4] == 4) // animate with v and h move
   {
      x = ((EYint/3) % c) + ((EXint/3) % c);
      if (x > c) x-=c;
      Ei[EN][1] = zz[x+5][b];
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


void enemy_deathcount()
{
   Efi[EN][14] += Efi[EN][13]; // rot inc
//   Efi[EN][12] = fixmul(Efi[EN][11], Efi[EN][12]); // scale inc
   Efi[EN][12] = mdw_fixmul(Efi[EN][11], Efi[EN][12], 0.0001); // scale inc

   Ei[EN][1] = zz[0][ Ei[EN][3] ]; // draw current ans shape
   // dec and check countdown timer
   if (--Ei[EN][30] < 0) // create bonus
   {
      Ei[EN][0] = 0; // kill enemy
      for (int c=0; c<500; c++)
         if (item[c][0] == 0) // find empty
         {
            for (int y=0; y<16; y++) item[c][y] = 0; // clear item
            item[c][0] = 2;           // type - bonus
            item[c][1] = Ei[EN][24];  // flower shape
            item[c][2] = 1; // draw mode normal
            item[c][3] = -1; // carryable
            item[c][4] = EXint;
            item[c][5] = EYint;
            item[c][7] = Ei[EN][25];  //  life
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

void enemy_move()
{
   for (EN=0; EN<100; EN++)
      if (Ei[EN][0])
      {
         EXint = al_fixtoi(Efi[EN][0]);
         EYint = al_fixtoi(Efi[EN][1]);

         // check for time to live
         int ttl = Ei[EN][27];
         if (ttl)
         {
            if (ttl < 11)
            {
               Ei[EN][0] = 66;          // change to different type to prevent use
               Efi[EN][4] = al_itofix(0);  // cant hurt anymore
               Ei[EN][29] = 0;          // no collision box
               int sq = 10-ttl;
               Ei[EN][1] = zz[5+sq][74];
            }

            if (ttl == 1) Ei[EN][0] = 0; // kill instantly
            Ei[EN][27]--;

         }

         // check for out of bounds
         if ((Efi[EN][0] < al_itofix(0)) || (Efi[EN][0] > al_itofix(1999))) Ei[EN][0]=0;
         if ((Efi[EN][1] < al_itofix(0)) || (Efi[EN][1] > al_itofix(1999))) Ei[EN][0]=0;

         switch (Ei[EN][0])
         {
            case 3:   enemy_archwagon();  break;
            case 4:   enemy_bouncer();  break;
            case 6:   enemy_cannon();  break;
            case 7:   enemy_podzilla();  break;
            case 8:   enemy_trakbot();  break;
            case 9:   enemy_cloner();  break;
            case 11:  enemy_block_walker();  break;
            case 12:  enemy_flapper();  break;
            case 99:  enemy_deathcount(); break;
         }
      }
}


void enemy_killed(int EN)
{
   int na, dl;
   int ht = Ei[EN][31]; // hit type

   // almost all do this but not enough to do by default
   int a = Ei[EN][0];
   if (a==3 || a==4 || a==6 || a==7 || a==8 || a==9 || a==12 )
   {
      Efi[EN][4] = al_itofix(0);  // cant hurt anymore
      Ei[EN][25]*=ht; // health bonus
   }

   switch (Ei[EN][0])
   {
      case 3: // archwagon
         na = Ei[EN][3] = 34;  // new ans
         dl = Ei[EN][30] = 20; // death_loop_wait; set delay
         Ei[EN][24] = 929+(ht-1)*32; // shape
         Efi[EN][11] = al_ftofix(1.08); // scale multiplier
         Efi[EN][13] = al_itofix(0);// 255/dl/2; rot inc

         zz[0][na] = zz[5][na]; // set shape
         zz[1][na] = 0;         // point to zero
         zz[2][na] = passcount; // set counter
         zz[3][na] = dl / (zz[4][na]+1); // set ans timer
      break;
      case 4:
         na = Ei[EN][3]; //new ans
         dl = Ei[EN][30] = 20; // death_loop_wait;  set delay
         Ei[EN][24] = 928+(ht-1)*32; // shape
         Efi[EN][11] = al_ftofix(1.03); // scale multiplier
         Efi[EN][13] = al_itofix(2); // 255/dl/2;  rot inc

         if (na == 29) na = 46;
         if (na == 14) na = 13;
         Ei[EN][3] = na; // new ans

         zz[0][na] = zz[5][na]; // set shape
         zz[1][na] = 0;         // point to zero
         zz[2][na] = passcount; // set counter
         zz[3][na] = dl / (zz[4][na]+1); // set ans timer
      break;
      case 6:
         na = Ei[EN][3] = 37;  // new ans
         dl = Ei[EN][30] = 20; // death_loop_wait; set delay
         Ei[EN][24] = 930+(ht-1)*32; // shape
         Efi[EN][11] = al_ftofix(1.08); // scale multiplier
         Efi[EN][13] = al_itofix(1); // 255/dl/2;  rot inc

         zz[0][na] = zz[5][na]; // set shape
         zz[1][na] = 0;         // point to zero
         zz[2][na] = passcount; // set counter
         zz[3][na] = dl / (zz[4][na]+1); // set ans timer
      break;
      case 7:
         na = Ei[EN][3] = 45;  // new ans
         dl = Ei[EN][30] = 40; // death_loop_wait; set delay
         Ei[EN][24] = 932+(ht-1)*32; // shape
         Efi[EN][12] = al_ftofix(2.8); // initial scale
         Efi[EN][11] = al_ftofix(.94); // scale multiplier
         Efi[EN][13] = al_itofix(0);  // 255/dl/2;  rot inc

         zz[0][na] = zz[5][na]; // set shape
         zz[1][na] = 0;         // point to zero
         zz[2][na] = passcount; // set counter
         zz[3][na] = dl / (zz[4][na]+1); // set ans timer
      break;
      case 8:
         na = Ei[EN][3] = 44;  // new ans
         dl = Ei[EN][30] = 12; // death_loop_wait; set delay
         Ei[EN][24] = 931+(ht-1)*32; // shape
         Efi[EN][11] = al_ftofix(1.00); // scale multiplier
         Efi[EN][13] = al_ftofix(255/dl*3/4); // rot inc

         zz[0][na] = zz[5][na]; // set shape
         zz[1][na] = 0;         // point to zero
         zz[2][na] = passcount; // set counter
         zz[3][na] = dl / (zz[4][na]+1); // set ans timer
      break;
      case 9:
         na = Ei[EN][3] = 53;  // new ans
         dl = Ei[EN][30] = 40; // death_loop_wait; set delay
         Ei[EN][24] = 934+(ht-1)*32; // shape
         Efi[EN][11] = al_ftofix(.98); // scale multiplier
         Efi[EN][13] = al_ftofix(306/dl); // rot inc

         zz[0][na] = zz[5][na]; // set shape
         zz[1][na] = 0;         // point to zero
         zz[2][na] = passcount; // set counter
         zz[3][na] = dl / zz[4][na]; // set ans timer
      break;
      case 11:

      break;
      case 12:
         na = Ei[EN][3] = 63;  // new ans
         dl = Ei[EN][30] = 20; // death_loop_wait; set delay
         Ei[EN][24] = 933+(ht-1)*32; // shape
         Efi[EN][11] = al_ftofix(1.04); // scale multiplier
         Efi[EN][13] = al_itofix(0); // rot inc

         zz[0][na] = zz[5][na]; // set shape
         zz[1][na] = 0;         // point to zero
         zz[2][na] = passcount; // set counter
         zz[3][na] = dl / zz[4][na]; // set ans timer
      break;

   } // end of switch

   // almost all do this but not enough to do by default
   if (a==3 || a==4 || a==6 || a==7 || a==8 || a==9 || a==12 )
   {
      event(13, EXint, EYint, EN, 0, 0, 0); // set type to death loop
      Ei[EN][0] = 99;
   }
}

void enemy_player_hit_proc(int EN)
{
   if (--Ei[EN][23]<0) // hit player retrigger
   {
      if (Ei[EN][22]) // player hit!
      {
         int p = Ei[EN][22]-1;
         players[p].LIFE -= Efi[EN][4];

         event(7, EXint, EYint, al_fixtoi(Efi[EN][4]), 0, 0, 0);
         event(12, EXint, EYint, EN, 0, 0, 0);
         Ei[EN][22] = 0;  // clear hit
         Ei[EN][23] = 60; // set retrigger amount
      }
   }
  else Ei[EN][22] = 0;
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
Ei[EN][1] = bitmap
Ei[EN][2] = draw mode (v and h flips)
Efi[EN][12] = scale;
Efi[EN][14] = rot

The animation sequence can be stored in Ei[EN][3]
but the enemy move code is responsible for setting the actual bitmap in Ei[EN][2]


----------------------------------------------------
-- Variables used after death to create the bonus --
----------------------------------------------------
Ei[EN][24]  shape
Ei[EN][25]  health bonus
Ei[EN][30]  death_loop_wait counter
Ei[EN][31]  hit type (1 = bullet, 2 = bomb)

Efi[EN][11]  scale multiplier
Efi[EN][13]  rot inc


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












































