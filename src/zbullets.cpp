#include "pm.h"

extern int e_bullet_active[50], e_bullet_shape[50];
extern al_fixed e_bullet_fx[50], e_bullet_fy[50], e_bullet_fxinc[50], e_bullet_fyinc[50];

extern int pbullet[50][6];
extern int pm_bullet_collision_box;


void proc_pbullets()
{
   int b,d,p,x,y;
   for (p=0; p<NUM_PLAYERS; p++)
      if ((players[p].active) && (!players[p].paused))
      {
         int x = al_fixtoi(players[p].PX) ;
         int y = al_fixtoi(players[p].PY) ;
         if (players[p].fire)
         {
            if (players[p].fire_held == 0) // fire button pressed, but not held
            {
               players[p].fire_held = 1;
               if (players[p].bullet_wait_counter < 1 )
               {
                  for (b=0; b<50; b++)     // search for empty bullet
                     if (!pbullet[b][0])
                     {
                        pbullet[b][0] = 1;
                        pbullet[b][1] = p;
                        pbullet[b][2] = x;
                        pbullet[b][3] = y + 1;
                        pbullet[b][4] = 0; // xinc
                        pbullet[b][5] = 0; // yinc

                        if (players[p].up)
                        {
                           pbullet[b][5] = -players[p].bullet_speed;
                           if (players[p].left_right) pbullet[b][2] = x+4;
                           else pbullet[b][2] = x-3;
                        }
                        else if (players[p].down)
                        {
                           pbullet[b][5] = players[p].bullet_speed;
                           if (players[p].left_right) pbullet[b][2] = x+4;
                           else pbullet[b][2] = x-3;
                        }
                        else
                        {
                           pbullet[b][4] = (players[p].left_right * (players[p].bullet_speed*2) ) - players[p].bullet_speed;
                        }
                        players[p].bullet_wait_counter = players[p].bullet_wait;
                        players[p].fire_held = 1;

                        // extra data is player number, bullet number
                        event(1, x, y, p, b, 0, 0);

                        break; // to break out of for loop
                     }
               }
            }
         }
         else players[p].fire_held = 0;  // fire is not pressed
         if (players[p].bullet_wait_counter > 0) players[p].bullet_wait_counter--;
      }

   // move and process collisions
   for (b = 0; b < 50; b++)
      if (pbullet[b][0])  // if bullet not active skip to next one
      {
         // move
         pbullet[b][2] += pbullet[b][4];  // xinc
         pbullet[b][3] += pbullet[b][5];  // yinc

         // bounds check
         if ((pbullet[b][2] < 5) || (pbullet[b][2] > 1995) || (pbullet[b][3]<5) || (pbullet[b][3] > 1995) ) pbullet[b][0] = 0;

         // level block collision
         x = ( (pbullet[b][2] + 10) / 20);
         y = ( (pbullet[b][3] + 10) / 20);
         d = l[x][y];
         if ((d > 63) && (d < NUM_SPRITES)) // if hit solid or breakable
         {
            pbullet[b][0] = 0;  // bullet dies
            if ((d > 95) && (d < 128)) // breakable wall
            {
               void remove_block(int, int);
               remove_block(x, y);
               draw_lift_lines();
            }
         }

         // check for player collisions
         if (deathmatch_pbullets)
         {
            for (int p=0; p<NUM_PLAYERS; p++)
               if ((players[p].active) && (!players[p].paused))
               {
                  int px = al_fixtoi(players[p].PX);
                  int py = al_fixtoi(players[p].PY);
                  int bx = pbullet[b][2];
                  int by = pbullet[b][3];
                  if ((bx > px - 10) && (bx < px + 10) && (by > py - 10) && (by < py + 10))
                  {
                     int pb = pbullet[b][1]; // the player that fired this bullet
                     if ( (p != pb) || ( (p == pb) && (suicide_pbullets == 1)) )  // shooting yourself allowed
                     {

                        players[p].LIFE -= al_itofix(deathmatch_pbullets_damage);

                        event(7, px, py, deathmatch_pbullets_damage, 0, 0, 0);

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
                        event(11, px, py, 0, 0, 0, 0);
                        p = NUM_PLAYERS; // break out of loop

                     }
                  }
               }
           }
      }
}

void draw_pbullets()
{
   al_set_target_bitmap(level_buffer);
   for (int b=0; b<50; b++)
      if (pbullet[b][0])
      {
         int p = pbullet[b][1];
         int x = pbullet[b][2];
         int y = pbullet[b][3];
         //draw_sprite(level_buffer, player_bitmap[players[p].bitmap_index][18], x, y);
         al_draw_bitmap(player_bitmap[players[p].bitmap_index][18], x, y, 0);
      }
}

void proc_ebullets()
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
               void remove_block(int, int);
               remove_block(xi, yi);
               draw_lift_lines();
            }
         }

         al_fixed x = e_bullet_fx[b];
         al_fixed y = e_bullet_fy[b];

         // new collision box is based on bullet speed and has both x and z component
         al_fixed ax = abs(e_bullet_fxinc[b]);      // enemy_bullet_collision_window x
         al_fixed ay = abs(e_bullet_fyinc[b]);      // enemy_bullet_collision_window y

         // enforce some minimums
         if (ax < al_itofix(4)) ax = al_itofix(4);
         if (ay < al_itofix(4)) ay = al_itofix(4);


         // check for collision with players
         for (int p=0; p<NUM_PLAYERS; p++)
            if ((players[p].active) && (!players[p].paused))
            {
               al_fixed px = players[p].PX;
               al_fixed py = players[p].PY;
               if ((x > px-ax) && (x < px+ax) && (y > py-ay) && (y < py+ay)) // did player get hit?
               {
                  int damage = 0;
                  switch (e_bullet_shape[b])
                  {
                     case 488:   damage = 5;  break; // arrow
                     case 489:   damage = 5;  break; // arrow
                     case 1055:  damage = 7;  break; // cannon ball
                     case 1020:  damage = 9;  break; // yellow things
                     case 1054:  damage = 10; break; // green ball
                     case 1062:  damage = 8;  break; // flapper thing
                  }
                  players[p].LIFE -= al_itofix(damage);

                  event(7, ex, ey, damage, 0, 0, 0);
                  event(11, ex, ey, 0, 0, 0, 0);


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
                  p = NUM_PLAYERS; // break out of loop
               }
            }
      }
}
void draw_ebullets()
{
   al_set_target_bitmap(level_buffer);
   for (int b = 0; b < 50; b++)
      if (e_bullet_active[b])
      {
         int ex = al_fixtoi(e_bullet_fx[b]);
         int ey = al_fixtoi(e_bullet_fy[b]);
         if (e_bullet_shape[b] > 1000)
         {
            int d = e_bullet_shape[b]-1000;
            d = zz[0][d];
            //draw_sprite(level_buffer, memory_bitmap[d], ex, ey);
            al_draw_bitmap(memory_bitmap[d], ex, ey, 0);
         }
         else
            //draw_sprite(level_buffer, memory_bitmap[e_bullet_shape[b]], ex, ey);
            al_draw_bitmap(memory_bitmap[e_bullet_shape[b]], ex, ey, 0);
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




