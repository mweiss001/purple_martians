// mwEnemyArchwagon.cpp

#include "pm.h"
#include "mwEnemy.h"
#include "mwPlayer.h"
#include "mwBitmap.h"
#include "mwShot.h"
#include "mwSolid.h"
#include "mwLift.h"
#include "mwLoop.h"
#include "mwLevel.h"
#include "mwColor.h"
#include "mwGameEvent.h"
#include "mwDisplay.h"
#include "mwScreen.h"



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


void mwEnemy::move_archwagon(int e)
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
            if ((mPlayer.syn[p].active) && (!mPlayer.syn[p].paused) )
               if ((EXint > mPlayer.syn[p].x - Ei[e][17]) && (EXint < mPlayer.syn[p].x) )
                  if ((EYint > mPlayer.syn[p].y - swh) && (EYint < mPlayer.syn[p].y + swl ))
                  {
                     mShot.fire_enemy_x_shot(e, p);
                     Ei[e][16] = Ei[e][15]; // set new prox wait
                  }

      if (!Ei[e][2]) // attempt shoot left
         for (int p=0; p<NUM_PLAYERS; p++)
            if ((mPlayer.syn[p].active) && (!mPlayer.syn[p].paused) )
               if ((EXint > mPlayer.syn[p].x ) && (EXint < mPlayer.syn[p].x + Ei[e][17]))
                  if ((EYint > mPlayer.syn[p].y  - swh) && (EYint < mPlayer.syn[p].y + swl ))
                  {
                     mShot.fire_enemy_x_shot(e, p);
                     Ei[e][16] = Ei[e][15]; // set new prox wait
                  }
   }

   move_arch_block_common(e);

   // set the bitmap and drawing mode
   int b = Ei[e][3]; // ans
   int c = mBitmap.zz[4][b]; // num_of_shapes in seq

   // animate with h move
   if (Ei[e][2] == 1) // right
   {
      int x = (EXint/3) % c;
      Ei[e][1] = mBitmap.zz[x+5][b];
   }
   if (Ei[e][2] == 0) // left
   {
      int x = (EXint/3) % c;
      Ei[e][1] = mBitmap.zz[5+c-x][b];
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



void mwEnemy::move_arch_block_common(int e)
{
   int EXint = Ef[e][0];
   int EYint = Ef[e][1];

   int on_solid = 0;
   int on_lift = 0;
   int ret = mSolid.is_down_solid(EXint, EYint, 1, 2);
   if ((ret == 1) || (ret == 2)) on_solid = 1;
   if (ret >= 32) on_lift = 1;



   // check if stuck
   int EXintR = Ef[e][0] + Ef[e][6];
   int EXintL = Ef[e][0] - Ef[e][6];

   if ((mSolid.is_left_solid(EXintL, EYint, 1, 2)) && (mSolid.is_right_solid(EXintR, EYint, 1, 2))) // stuck
   {
      int p = find_closest_player(e);
      if (EXint < mPlayer.syn[p].x-5) Ei[e][2] = 1;
      if (EXint > mPlayer.syn[p].x+5) Ei[e][2] = 0;
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
              if (!mSolid.is_right_solid(EXint+Ei[e][10]-10, EYint+20, 1, 2)) change_dir = 1;
            if (Ei[e][12]) // jump before wall
              if (mSolid.is_right_solid(EXint+Ei[e][12], EYint, 1, 2)) Ei[e][5] = -160;
            if (Ei[e][11]) // jump before hole
               if (!mSolid.is_right_solid(EXint+Ei[e][11]-18, EYint+20, 1, 2)) Ei[e][5] = -160;
         }
         if ((mSolid.is_right_solid(EXint, EYint, 1, 2)) || (change_dir))
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
              if (!mSolid.is_left_solid(EXint-Ei[e][10]+10, EYint+20, 1, 2)) change_dir = 1;
            if (Ei[e][12]) // jump before wall
               if (mSolid.is_left_solid(EXint-Ei[e][12], EYint, 1, 2)) Ei[e][5] = -160;
            if (Ei[e][11]) // jump before hole
               if (!mSolid.is_left_solid(EXint-Ei[e][11]+18, EYint+20, 1, 2)) Ei[e][5] = -160;
         }
         if ((mSolid.is_left_solid(EXint, EYint, 1, 2)) || (change_dir))
         {
            Ef[e][0] -= Ef[e][2]; // take back last move
            Ei[e][2] = 1; // change direction;
            EXint = Ef[e][0];
         }
      }

      if (!Ei[e][8]) // follow mode
      {
         int p = find_closest_player(e);
         if (EXint < mPlayer.syn[p].x-5) Ei[e][2] = 1;
         if (EXint > mPlayer.syn[p].x+5) Ei[e][2] = 0;
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
      if (mSolid.is_down_solid(EXint, EYint, 1, 2))
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
      if ((mSolid.is_up_solid(EXint, EYint, 1, 2) == 1) || (mSolid.is_up_solid(EXint, EYint, 1, 2) > 31) )
         Ei[e][5] = 0;  // stop rising
   }

   if (on_lift)
   {
      Ef[e][1] += mLift.cur[ret-32].yinc;  // move with lift
   }


   if ((on_solid) || (on_lift))
   {
      // frame_num jump
      if ((Ei[e][6] > 0) && ((mLoop.frame_num % Ei[e][6]) == 1)) Ei[e][5] = -160;

      // check for jump if player passes above
      if (Ei[e][7] > 0)
         for (int p=0; p<NUM_PLAYERS; p++)
            if ((mPlayer.syn[p].active) && (!mPlayer.syn[p].paused) )
               if ((EXint < mPlayer.syn[p].x + Ei[e][7]) && (EXint > mPlayer.syn[p].x - Ei[e][7]) &&
                   (EYint > mPlayer.syn[p].y)) Ei[e][5] = -160;
   }

}


//-------------------------------------------------------------------------------------------
//--4--blokwalk ------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------
// see walker_archwagon_common

// Ei[][13]  block


void mwEnemy::move_blokwalk(int e)
{

   int EXint = Ef[e][0];
   int EYint = Ef[e][1];

   enemy_player_hit_proc(e);

   if (Ei[e][31]) // hit
   {
      int ex = EXint/20;
      int ey = EYint/20;
//      int block = 168 | PM_BTILE_ALL_SOLID | PM_BTILE_BREAKABLE_PSHOT;


      int block = Ei[e][13];

      mLevel.l[ex][ey] = block;

      if (!mDisplay.no_display)
      {

         al_set_target_bitmap(mBitmap.level_background);
         al_draw_filled_rectangle(ex*20, ey*20, ex*20+20, ey*20+20, mColor.pc[0]);
         al_draw_bitmap(mBitmap.btile[block & 1023], ex*20, ey*20, 0);
      }


      mGameEvent.add(42, 0, 0, Ei[e][26], e, 11, Ei[e][31]);

      Ei[e][0] = 0;
      return; // to stop rest of execution
   }
   move_arch_block_common(e);

   // set the bitmap and drawing mode
   int b = Ei[e][3];          // ans
   int c = mBitmap.zz[4][b];  // num_of_shapes in seq

   // animate with h move
   int x = (EXint/2) % c;
   Ei[e][1] = mBitmap.zz[x+5][b];
}







//-------------------------------------------------------------------------------------------
//--19--crew --------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------
// see walker_archwagon_common
/*
Ei[][3]   color

*/
void mwEnemy::move_crew(int e)
{
   move_arch_block_common(e);
}


void mwEnemy::draw_crew(int e, int cx, int cy, int custom)
{
   if ((mLoop.state[0] != PM_PROGRAM_STATE_MAIN_GAME_LOOP) || (mPlayer.syn[0].level_done_mode == 27))// don't draw when game is running, custom draw direct to screen in screen overlay so scaling looks nice
   {
      int x = Ef[e][0];
      int y = Ef[e][1];
      if (custom)
      {
         x = cx;
         y = cy;
      }

      int flags = 0;
      if (Ei[e][2] == 0) flags = ALLEGRO_FLIP_HORIZONTAL;

      int pos = ((int) Ef[e][0] / 6) % 6;  // 6 shapes in sequence
      int col = Ei[e][3];

      float ps = mEnemy.Ef[e][8];  // player scale
      float yo = 10 * (1-ps);      // adjust y offset

      al_draw_scaled_rotated_bitmap(mBitmap.player_tile[col][pos], 10, 10, x+10, y+10+yo, ps, ps, 0, flags);
   }
}



void mwEnemy::draw_crew_screen_direct(int e)
{
   float sc = mDisplay.scale_factor_current;

   float ex1=0, ey1=0;
   mScreen.calc_actual_screen_position(Ef[e][0]+10, Ef[e][1]+10, ex1, ey1);

   float ps = Ef[e][8];  // player scale
   float scale = sc * ps;       // final draw scale

   float sd = 20 * (1-ps);      // size difference
   float yo = (sd/2)*sc;        // half size difference, scaled

   int flags = 0;
   if (Ei[e][2] == 0) flags = ALLEGRO_FLIP_HORIZONTAL;

   int pos = ((int) Ef[e][0] / 6) % 6;  // 6 shapes in sequence
   int col = Ei[e][3];

   al_draw_scaled_rotated_bitmap(mBitmap.player_tile[col][pos], 10, 10, ex1, ey1+yo, scale, scale, 0, flags);
}







