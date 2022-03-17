// zbullets.cpp

#include "pm.h"

void proc_pbullet_collision(int p, int b)
{
   players[p].LIFE -= al_itofix(deathmatch_pbullets_damage);

   al_fixed bxinc = al_ftofix(pbullet[b][4]/3);
   al_fixed z = al_itofix(0);
   if (bxinc > z) players[p].right_xinc += bxinc;
   if (bxinc < z) players[p].left_xinc += bxinc;

   if (players[p].right_xinc > al_itofix(4)) players[p].right_xinc = al_itofix(4);
   if (players[p].left_xinc < al_itofix(-4)) players[p].left_xinc = al_itofix(-4);

   players[p].yinc += al_ftofix(pbullet[b][5]/2);
   if (players[p].yinc > al_itofix( 5)) players[p].yinc = al_itofix( 5);
   if (players[p].yinc < al_itofix(-5)) players[p].yinc = al_itofix(-5);

   if (al_ftofix(pbullet[b][5]) < 0)  // player hit from below by bullet
   {
      players[p].player_ride = 0;
      players[p].PY += players[p].yinc;
   }
   pbullet[b][0] = 0;  // bullet dies
}

void proc_pbullet_shoot(int p)
{
   int x = al_fixtoi(players[p].PX) ;
   int y = al_fixtoi(players[p].PY) ;
   int bs = players[p].bullet_speed;

   if (players[p].fire)
   {
      if (players[p].fire_held == 0) // fire button pressed, but not held
      {
         players[p].fire_held = 1;
         if (players[p].bullet_wait_counter < 1 )
         {
            for (int b=0; b<50; b++)     // search for empty bullet
               if (!pbullet[b][0])
               {
                  pbullet[b][0] = 1;
                  pbullet[b][1] = p;
                  pbullet[b][2] = x;
                  pbullet[b][3] = y + 1;
                  pbullet[b][4] = 0; // xinc
                  pbullet[b][5] = 0; // yinc

                  if (players[p].left_right) pbullet[b][2] = x+4;
                  else pbullet[b][2] = x-3;

                  if      (players[p].up)    pbullet[b][5] = -bs;
                  else if (players[p].down)  pbullet[b][5] =  bs;
                  else                       pbullet[b][4] = (players[p].left_right*bs*2) - bs;


                  // if this line is not here player cannot shoot breakable blocks
                  // when directly in front of them..
                  if ((!players[p].up) && (!players[p].down) && (players[p].left_right))
                      pbullet[b][2]--;

//                  // temp testing
//                  pbullet[b][2] = x + (players[p].left_right * (40*2) ) - 40;
//                  pbullet[b][3] = y + 1;

                     // move
                     pbullet[b][2] += pbullet[b][4];  // xinc
                     pbullet[b][3] += pbullet[b][5];  // yinc

//                  // temp testing
//                  pbullet[b][4] = 0; // xinc
//                  pbullet[b][5] = 0; // yinc

                  players[p].bullet_wait_counter = players[p].bullet_wait;
                  players[p].fire_held = 1;

                  // extra data is player number, bullet number
                  game_event(1, x, y, p, b, 0, 0);

                  break; // to break out of for loop
               }
         }
      }
   }
   else players[p].fire_held = 0;  // fire is not pressed
   if (players[p].bullet_wait_counter > 0) players[p].bullet_wait_counter--;
}


void move_pbullets()
{
   // move and process wall collisions
   for (int b=0; b<50; b++)
      if (pbullet[b][0])
      {
         // move
         pbullet[b][2] += pbullet[b][4];  // xinc
         pbullet[b][3] += pbullet[b][5];  // yinc

         // bounds check
         if ((pbullet[b][2] < 5) || (pbullet[b][2] > 1995) || (pbullet[b][3]<5) || (pbullet[b][3] > 1995) ) pbullet[b][0] = 0;

         // level block collision
         int x = ( (pbullet[b][2] + 10) / 20);
         int y = ( (pbullet[b][3] + 10) / 20);
         int d = l[x][y];
         if ((d > 63) && (d < NUM_SPRITES)) // if hit solid or breakable
         {
            pbullet[b][0] = 0;  // bullet dies
            if ((d > 95) && (d < 128)) // breakable wall
            {
               remove_block(x, y);
               draw_lift_lines();
            }
         }
      }
}

void draw_pbullets()
{
   for (int b=0; b<50; b++)
      if (pbullet[b][0]) al_draw_bitmap(player_tile[players[pbullet[b][1]].color][18], pbullet[b][2], pbullet[b][3], 0);
}

void proc_ebullet_collision(int p, int b)
{
   int damage = 0;
   int e_type = 0;
   switch (e_bullet_shape[b])
   {
      case 488:   e_type = 3;  damage = 5;  break; // arrow
      case 489:   e_type = 3;  damage = 5;  break; // arrow
      case 1055:  e_type = 6;  damage = 7;  break; // cannon ball
      case 1020:  e_type = 8;  damage = 9;  break; // yellow things
      case 1054:  e_type = 7;  damage = 10; break; // green ball
      case 1062:  e_type = 12; damage = 8;  break; // flapper thing
   }
   players[p].LIFE -= al_itofix(damage);

   game_event(43, 0, 0, p, e_type, 0, damage);

   // recoil !!
   if (e_bullet_fxinc[b] > al_itofix(0))
   {
      players[p].right_xinc += (e_bullet_fxinc[b]/2);
      if (players[p].right_xinc > al_itofix( 5)) players[p].right_xinc = al_itofix( 5);
   }
   if (e_bullet_fxinc[b] < al_itofix(0))
   {
      players[p].left_xinc += (e_bullet_fxinc[b]/2);
      if (players[p].left_xinc < al_itofix( -5)) players[p].left_xinc = al_itofix(-5);
   }

   players[p].yinc += (e_bullet_fyinc[b]/2);
   if (players[p].yinc > al_itofix( 5)) players[p].yinc = al_itofix( 5);
   if (players[p].yinc < al_itofix(-5)) players[p].yinc = al_itofix(-5);

   e_bullet_active[b] = 0; // bullet dies
}

void move_ebullets()
{
   for (int b = 0; b < 50; b++)
      if (e_bullet_active[b])  // if bullet not active skip to next one
      {
         e_bullet_fx[b] += e_bullet_fxinc[b]; // inc x
         e_bullet_fy[b] += e_bullet_fyinc[b]; // inc y

         int ex = al_fixtoi(e_bullet_fx[b]);
         int ey = al_fixtoi(e_bullet_fy[b]);

         // check if out of bounds
         if ((ex<0) || (ex>2000) || (ey<0) || (ey>2000)) e_bullet_active[b] = 0;

         // check if hit wall (or more accurately if co-located with a block)
         int xi = (ex+10)/20;
         int yi = (ey+10)/20;
         int d = l[xi][yi];
         if ((d > 63) && (d<NUM_SPRITES))  // bullet hit solid or breakable wall
         {
            e_bullet_active[b] = 0;        // bullet dies
            if ((d > 95) && (d < 128))     // remove breakable wall
            {
               remove_block(xi, yi);
               draw_lift_lines();
            }
         }
      }
}


void draw_ebullets()
{

/*
   // temp testing
   al_draw_circle(tx1+10, ty1+10, 10, palette_color[ttc1], 2 );
   al_draw_textf(font, palette_color[ttc1], tx1, ty1-10, 0, "%.3f",ttfloat1);


   al_draw_circle(tx2+10, ty2+10, 10, palette_color[ttc2], 2 );
   al_draw_textf(font, palette_color[ttc2], tx2, ty2+30, 0, "%.3f",ttfloat2);
*/


   for (int b = 0; b < 50; b++)
      if (e_bullet_active[b])
      {
         int t = e_bullet_shape[b];
         if (t > 1000) t = zz[0][e_bullet_shape[b]-1000];
         al_draw_bitmap(tile[t], al_fixtof(e_bullet_fx[b]), al_fixtof(e_bullet_fy[b]), 0);
      }
}

void clear_bullets(void)
{
   for (int c = 0; c < 50; c++)
   {
      e_bullet_active[c] = 0;
      e_bullet_shape[c]  = 0;
      e_bullet_fxinc[c]  = al_itofix(0);
      e_bullet_fyinc[c]  = al_itofix(0);
      e_bullet_fy[c]     = al_itofix(0);
      e_bullet_fx[c]     = al_itofix(0);
      for (int y=0; y<6; y++)
         pbullet[c][y] = 0;
   }
}




