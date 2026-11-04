// mwShot.cpp

#include "pm.h"
#include "mwShot.h"
#include "mwBitmap.h"
#include "mwLevel.h"
#include "mwGameEvent.h"
#include "mwLoop.h"
#include "mwDemoRecord.h"
#include "mwFont.h"
#include "mwColor.h"
#include "mwGameState.h"


void mwShot::proc_pshot_collision(int pl, int b)
{
   m_parent.mPlayer.syn[pl].health -= mGameState.player_vs_player_shot_damage;

   float bxinc = p[b].xinc/3;

   if (bxinc > 0) m_parent.mPlayer.syn[pl].right_xinc += bxinc;
   if (bxinc < 0) m_parent.mPlayer.syn[pl].left_xinc += bxinc;

   if (m_parent.mPlayer.syn[pl].right_xinc > 4) m_parent.mPlayer.syn[pl].right_xinc = 4;
   if (m_parent.mPlayer.syn[pl].left_xinc < -4) m_parent.mPlayer.syn[pl].left_xinc = -4;

   m_parent.mPlayer.syn[pl].yinc += p[b].yinc/2;
   if (m_parent.mPlayer.syn[pl].yinc >  5) m_parent.mPlayer.syn[pl].yinc =  5;
   if (m_parent.mPlayer.syn[pl].yinc < -5) m_parent.mPlayer.syn[pl].yinc = -5;

   if (p[b].yinc < 0)  // player got shot from below
   {
      m_parent.mPlayer.syn[pl].player_ride = 0;
      m_parent.mPlayer.syn[pl].y += m_parent.mPlayer.syn[pl].yinc;
   }

   mDemoRecord.mark_player_shot_used(p[b].player, p[b].active, 2);

   p[b].active = 0;  // shot dies
}



int mwShot::find_empty_pshot()
{
   int index = -1;

   for (int b=0; b<50; b++)
      if (!p[b].active) index = b;

   if (index == -1) // array is full !
   {
      // find the oldest entry and remove it
      int lowest_fn = 9999999;
      for (int b=0; b<50; b++)
      {
         if (p[b].active < lowest_fn)
         {
            lowest_fn = p[b].active;
            index = b;
         }
      }
   }
   return index;
}



void mwShot::proc_player_shoot(int pl)
{
   float x = m_parent.mPlayer.syn[pl].x;
   float y = m_parent.mPlayer.syn[pl].y;
   float bs = (float) m_parent.mPlayer.syn[pl].shot_speed;

   if (m_parent.mPlayer.if_players_ctrl_just_pressed(pl, PM_COMPMOVE_FIRE)) // fire pressed this frame, but not last frame
   {
      if (m_parent.mPlayer.syn[pl].shot_wait_counter < 1)
      {
         m_parent.mPlayer.syn[pl].stat_shots_fired++;

         int b = find_empty_pshot();
         if (b != -1)
         {
            p[b].active = mLoop.frame_num;
            p[b].player = pl;
            p[b].x = x;
            p[b].y = y + 1;
            p[b].xinc = 0;
            p[b].yinc = 0;

            if (m_parent.mPlayer.syn[pl].left_right) p[b].x = x+4;
            else p[b].x = x-3;

            if      (m_parent.mPlayer.syn[pl].up)    p[b].yinc = -bs;
            else if (m_parent.mPlayer.syn[pl].down)  p[b].yinc =  bs;
            else                                  p[b].xinc = (m_parent.mPlayer.syn[pl].left_right*bs*2) - bs;

            // if this line is not here player cannot shoot breakable blocks when directly in front of them (when facing right/left??
            if ((!m_parent.mPlayer.syn[pl].up) && (!m_parent.mPlayer.syn[pl].down) && (m_parent.mPlayer.syn[pl].left_right)) p[b].x -=1;

            // initial move
            p[b].x += p[b].xinc;  // xinc
            p[b].y += p[b].yinc;  // yinc

            m_parent.mPlayer.syn[pl].shot_wait_counter = m_parent.mPlayer.syn[pl].shot_wait;

            mGameEvent.add(1, x, y, pl, b, 0, 0);
         }
         else printf("player shot array full!\n");
      }
   }
   if (m_parent.mPlayer.syn[pl].shot_wait_counter > 0) m_parent.mPlayer.syn[pl].shot_wait_counter--;
}


void mwShot::move_pshots()
{
   num_pshots = 0;

   // move and process wall collisions
   for (int b=0; b<50; b++)
      if (p[b].active)
      {
         num_pshots++;

         // move
         p[b].x += p[b].xinc;
         p[b].y += p[b].yinc;

         // bounds check
         if ((p[b].x < 5) || (p[b].x > 1995) || (p[b].y<5) || (p[b].y > 1995) ) p[b].active = 0;

         // level block collision
         int x = ((p[b].x+10) / 20);
         int y = ((p[b].y+10) / 20);
         int d = m_parent.l[x][y];

         if (d & PM_BTILE_BREAKABLE_PSHOT)
         {
            m_parent.change_block(x, y, 0);
            mDemoRecord.mark_player_shot_used(p[b].player, p[b].active, 6);
            p[b].active = 0;  // shot is done
         }
         if (d & PM_BTILE_SOLID_PBUL) p[b].active = 0;  // shot hit solid wall, done
      }
}

void mwShot::draw_pshots()
{
   for (int b=0; b<50; b++)
      if (p[b].active)
      {
         al_draw_bitmap(mBitmap.player_tile[m_parent.mPlayer.syn[p[b].player].color][18], p[b].x, p[b].y, 0);

         // overlay frame number of when bullet was shot (useful for doing multiplayer recording)
         if (mLoop.state[1] == PM_PROGRAM_STATE_DEMO_RECORD) al_draw_textf(mFont.pixl, mColor.pc[15], p[b].x+10, p[b].y+3, ALLEGRO_ALIGN_CENTER, "%d", p[b].active);
      }
}

void mwShot::proc_eshot_collision(int pl, int b)
{
   int damage = 0;
   int e_type = 0;
   switch (e[b].shape)
   {
      case 488:   e_type = 3;  damage = 5;  break; // arrow
      case 489:   e_type = 3;  damage = 5;  break; // arrow
      case 1055:  e_type = 6;  damage = 7;  break; // cannon ball
      case 1020:  e_type = 8;  damage = 9;  break; // yellow things
      case 1054:  e_type = 7;  damage = 10; break; // green ball
      case 1062:  e_type = 12; damage = 8;  break; // flapper thing
   }
   m_parent.mPlayer.syn[pl].health -= damage;
   mGameEvent.add(41, 0, 0, pl, e_type, 2, damage);

   // recoil !!
   if (e[b].xinc > 0)
   {
      m_parent.mPlayer.syn[pl].right_xinc += e[b].xinc/2;
      if (m_parent.mPlayer.syn[pl].right_xinc > 4) m_parent.mPlayer.syn[pl].right_xinc = 4;
   }
   if (e[b].xinc < 0)
   {
      m_parent.mPlayer.syn[pl].left_xinc += e[b].xinc/2;
      if (m_parent.mPlayer.syn[pl].left_xinc < -4) m_parent.mPlayer.syn[pl].left_xinc = -4;
   }

   m_parent.mPlayer.syn[pl].yinc += e[b].yinc/2;

   if (m_parent.mPlayer.syn[pl].yinc >  5) m_parent.mPlayer.syn[pl].yinc =  5;
   if (m_parent.mPlayer.syn[pl].yinc < -8) m_parent.mPlayer.syn[pl].yinc = -8;

   e[b].active = 0; // shot dies
}

void mwShot::move_eshots()
{
   num_eshots = 0;
   for (int b=0; b<50; b++)
      if (e[b].active)
      {
         num_eshots++;
         e[b].x += e[b].xinc;
         e[b].y += e[b].yinc;

         // check if out of bounds
         if ((e[b].x<0) || (e[b].x>2000) || (e[b].y<0) || (e[b].y>2000)) e[b].active = 0;

         // check if hit wall (or more accurately if co-located with a block)
         int xi = (e[b].x+10)/20;
         int yi = (e[b].y+10)/20;
         int d = m_parent.l[xi][yi];
         if (d & PM_BTILE_SOLID_EBUL)  // shot hit solid or breakable wall
         {
            e[b].active = 0;                                   // shot dies
            if (d & PM_BTILE_BREAKABLE_ESHOT) m_parent.change_block(xi, yi, 0); // breakable wall
         }
      }
}


void mwShot::draw_eshots()
{

/*
   // temp testing
   al_draw_circle(tx1+10, ty1+10, 10, mColor.pc[ttc1], 2 );
   al_draw_textf(mFont.pr8, mColor.pc[ttc1], tx1, ty1-10, 0, "%.3f",ttfloat1);


   al_draw_circle(tx2+10, ty2+10, 10, mColor.pc[ttc2], 2 );
   al_draw_textf(mFont.pr8, mColor.pc[ttc2], tx2, ty2+30, 0, "%.3f",ttfloat2);

   // temp testing
   al_draw_rectangle(tx1, ty1, tx2, ty2, mColor.pc[ttc1], 2 );
*/

   for (int b=0; b<50; b++)
      if (e[b].active)
      {
         int t = e[b].shape;
         if (t > 1000) t = mBitmap.zz[0][e[b].shape-1000];
         al_draw_bitmap(mBitmap.sprite[t], (int)e[b].x, (int)e[b].y, 0);
      }
}

void mwShot::clear_shots()
{
   for (int b=0; b<50; b++)
   {
      e[b].active = 0;
      e[b].shape  = 0;
      e[b].xinc   = 0;
      e[b].yinc   = 0;
      e[b].y      = 0;
      e[b].x      = 0;

      p[b].active = 0;
      p[b].player = 0;
      p[b].xinc   = 0;
      p[b].yinc   = 0;
      p[b].y      = 0;
      p[b].x      = 0;

   }
}




int mwShot::find_empty_eshot()
{
   int index = -1;

   for (int b=0; b<50; b++)
      if (!e[b].active) index = b;

   if (index == -1) // array is full !
   {
//      return -1; // old behaviour


      // find the oldest entry and remove it
      int lowest_fn = 9999999;
      for (int b=0; b<50; b++)
      {
         if (e[b].active < lowest_fn)
         {
            lowest_fn = e[b].active;
            index = b;
         }
      }
   }
   return index;
}




void mwShot::fire_enemy_shotz(int en, int shot_ans, float px, float py)
{
   float xlen = px - m_parent.mEnemy.Ef[en][0];   // get the x distance between enemy and player
   float ylen = py - m_parent.mEnemy.Ef[en][1];   // get the y distance between enemy and player
   float hy_dist = sqrt(pow(xlen, 2) + pow(ylen, 2));    // hypotenuse distance
   float speed = m_parent.mEnemy.Ef[en][7];            // speed
   float scaler = hy_dist / speed;    // get scaler
   float xinc = xlen / scaler;        // calc xinc
   float yinc = ylen / scaler;        // calc yinc


   int b = find_empty_eshot();
      if (b != -1)
      {
         e[b].active = mLoop.frame_num;
         e[b].shape = 1000 + shot_ans;
         e[b].x = m_parent.mEnemy.Ef[en][0];
         e[b].y = m_parent.mEnemy.Ef[en][1];
         e[b].xinc = xinc;
         e[b].yinc = yinc;
      }
}
void mwShot::fire_enemy_shota(int en, int shot_ans, int p)
{
   float rx, ry;
   calc_where_player_will_be(en, p, rx, ry);
   fire_enemy_shotz(en, shot_ans, rx, ry);
}

void mwShot::calc_where_player_will_be(int en, int pl, float& rx, float& ry)
{
   // origin of the shot and velocity with no direction
   float bx = m_parent.mEnemy.Ef[en][0];
   float by = m_parent.mEnemy.Ef[en][1];
   float bv = m_parent.mEnemy.Ef[en][7];

   // targetted player x and y and velocity xinc and yinc
   float px  = m_parent.mPlayer.syn[pl].x;
   float py  = m_parent.mPlayer.syn[pl].y;
   float pvx = m_parent.mPlayer.syn[pl].xinc;
   float pvy = m_parent.mPlayer.syn[pl].yinc;


   // Edgar's method
   //float A = pow(pvx,2) + pow(pvy,2) - pow(bv,2);
   //float B = 2*(px*pvx) + 2*(py*pvy) -2*(bx*pvx) -2*(by*pvy);
   //float C = pow(px,2) + pow(bx,2) + pow(py,2) + pow(by,2) - 2*(bx*px) -2*(by*py);

   // Peter's method
   float x = px-bx;
   float y = py-by;
   float A = pow(pvx,2) + pow(pvy,2) - pow(bv,2);
   float B = 2*(x*pvx) + 2*(y*pvy);
   float C = pow(x,2) + pow(y,2);

// Egdar: You will have to check your code for division by A=0 and for a negative B^2 - 4AC discriminant.

// Quadratic Formula: The roots of a quadratic equation ax2 + bx + c = 0 are given by x = [-b +/- sqrt(b^2 - 4ac)] / 2a.
// The discriminant of the quadratic equation is D = b^2 - 4ac
// For D > 0 the roots are real and distinct.
// For D = 0 the roots are real and equal.
// For D < 0 the roots do not exist, or the roots are imaginary.

   float D = pow(B,2) - 4*(A*C); // discriminant

   if ((A != 0) && (D >= 0)) // this complains about comparing a float to zero (not anymore?? why)
   //if ( ((A > 0) || (A < 0)) && (D >= 0))
   {
      float t = ( -B - sqrt(pow(B,2) - 4*(A*C)) ) / (2*A);
      float px1 = px + pvx * t; // get player target position based on t
      float py1 = py + pvy * t;

      rx = px1;
      ry = py1;
   }
   else // if the discriminant test fails, return the current player's position
   {
      rx = px;
      ry = py;
   }
}

void mwShot::fire_enemy_x_shot(int en, int pl)
{
   float x_shot_speed = m_parent.mEnemy.Ef[en][7];
   for (int b=0; b<50; b++)  // find empty e_shot
      if (!e[b].active)
      {
         e[b].active = 1;
         e[b].yinc = 0;
         e[b].x = m_parent.mEnemy.Ef[en][0];
         e[b].y = m_parent.mEnemy.Ef[en][1];

         if (m_parent.mEnemy.Ef[en][0] < m_parent.mPlayer.syn[pl].x)
         {
            e[b].xinc = x_shot_speed;
            e[b].shape = 488;
         }
         if (m_parent.mEnemy.Ef[en][0] >= m_parent.mPlayer.syn[pl].x)
         {
            e[b].xinc = -x_shot_speed;
            e[b].shape = 489;
         }
         b=50; // end loop
      }
}









