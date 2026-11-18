// mwEnemyTrakbot.cpp

#include "pm.h"
#include "mwEnemy.h"
#include "mwBitmap.h"
#include "mwLevel.h"
#include "mwSolid.h"


//--8--trakbot-------------------------------------------------------------------
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------

//     Ei[e][0] = type
//     Ei[e][1] = shape
//     Ei[e][3] = ans

//     Ei[e][4] = draw_type (hflip)
//     Ei[e][5] = mode
//     Ei[e][6] = fall count
//     Ei[e][7] = drop mode 1 = yes
//     Ei[e][8] = twirling counter


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


void mwEnemy::move_trakbot(int e)
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


   if (Ei[e][6] != 0)   // if falling
   {
      if (dp) printf("mode:fall  x:%1.2f  y:%1.2f\n", Ef[e][0], Ef[e][1]);

      Ei[e][6] += 5;                                                  // add acceleration to fall
      if (Ei[e][6] > 160)  Ei[e][6] = 160;                            // terminal velocity
      float fall_yinc = Ef[e][3] * Ei[e][6] / 100;                    // (float * int) / int
      mv = mSolid.is_down_solidf(Ef[e][0], Ef[e][1], fall_yinc, 0);   // will we hit floor during this move?
      if (mv < fall_yinc) Ei[e][6] = 0;                               // end fall if allowed move less than requested move
      Ef[e][1] += mv;                                                 // move the allowed amount
      if (Ef[e][1] > 1980) Ei[e][0]=0;                                // kill if falls past bottom of level

      // seek player while falling
      int p = find_closest_player(e);
      float ex = Ef[e][0];
      float px = m_parent.mPlayer.syn[p].x;

      if (px > ex) mode = PM_ENEMY_TRAKBOT_MODE_FLOOR_RIGHT;
      else mode = PM_ENEMY_TRAKBOT_MODE_FLOOR_LEFT;
      set_trakbot_mode(e, mode);
   }
   else // not fall
   {
      if ((Ei[e][7]) && ((mode == PM_ENEMY_TRAKBOT_MODE_CEIL_LEFT) || (mode == PM_ENEMY_TRAKBOT_MODE_CEIL_RIGHT))) // if drop mode on and trakbot on ceiling
      {
         float ex = Ef[e][0];
         float ey = Ef[e][1];
         float ew = 15;
         for (int p=0; p<NUM_PLAYERS; p++) // if any player passes underneath
            if (m_parent.mPlayer.is_player_active_and_not_paused(p))
            {
               float px = m_parent.mPlayer.syn[p].x;
               float py = m_parent.mPlayer.syn[p].y;
               if ((px < ex + ew) && (px > ex - ew) && (py > ey))  // if directly above player
               {
                  if (dp) printf("just dropped on player:%d\n", p);
                  Ei[e][6] = 5; // start fall mode
                  if (mode == PM_ENEMY_TRAKBOT_MODE_CEIL_LEFT)  set_trakbot_mode(e, PM_ENEMY_TRAKBOT_MODE_FLOOR_LEFT);
                  if (mode == PM_ENEMY_TRAKBOT_MODE_CEIL_RIGHT) set_trakbot_mode(e, PM_ENEMY_TRAKBOT_MODE_FLOOR_RIGHT);
               }
            }
      }
   }


   if (Ei[e][6] == 0)  // if not still not fall
   {
      if (dp) printf("mode:%d  x:%1.2f  y:%1.2f - ", mode, Ef[e][0], Ef[e][1]);
      switch (mode)
      {
         case PM_ENEMY_TRAKBOT_MODE_FLOOR_RIGHT:
            set_mode_draw_variables(e, PM_ENEMY_TRAKBOT_MODE_FLOOR_RIGHT);
            if (dp) { if (mSolid.is_down_solidf(Ef[e][0], Ef[e][1], 1, 0) < 1) printf(" - 0 with solid floor\n"); else  printf(" - 0 with NOT solid floor\n"); }
            mv = mSolid.is_down_solidf(Ef[e][0], Ef[e][1], Ef[e][2], 1);
            if (mv < Ef[e][2]) // allowed move less than requested move
            {
               if (dp) printf("will lose floor during this move - mv%1.2f \n", mv);
               Ef[e][0] += mv;                           // move the allowed amount
               mode = PM_ENEMY_TRAKBOT_MODE_LWALL_DOWN;  // next mode (cw)
               Ef[e][14] += deg_to_rad(45);              // cw transition rotation
               Ef[e][1] += 1;                            // move down 1 to catch new wall
               break;
            }
            mv = mSolid.is_right_solidf(Ef[e][0], Ef[e][1], Ef[e][2], 0);
            if (mv < Ef[e][2]) // allowed move less than requested move
            {
               if (dp) printf("will hit wall during this move - mv%1.2f \n", mv);
               Ef[e][0] += mv;                           // move the allowed amount
               mode = PM_ENEMY_TRAKBOT_MODE_RWALL_UP;    // next mode (ccw)
               Ef[e][14] -= deg_to_rad(45);              // ccw transition rotation
               break;
            }
            Ef[e][0] += mv;  // if neither of these matched, move the allowed amount
         break;



         case PM_ENEMY_TRAKBOT_MODE_RWALL_UP:
            set_mode_draw_variables(e, PM_ENEMY_TRAKBOT_MODE_RWALL_UP);
            if (dp) { if (mSolid.is_right_solidf(Ef[e][0], Ef[e][1], 1, 0) < 1) printf(" - 1 with solid r wall\n"); else  printf(" - 1 with NOT solid r wall\n"); }
            mv = mSolid.is_right_solidf(Ef[e][0], Ef[e][1], Ef[e][3], -1);
            if (mv < Ef[e][3]) // allowed move less than requested move
            {
               if (dp) printf("will lose rwall during this move - mv%1.2f \n", mv);
               Ef[e][1] -= mv;                           // move the allowed amount
               mode = PM_ENEMY_TRAKBOT_MODE_FLOOR_RIGHT; // next mode (cw)
               Ef[e][14] += deg_to_rad(45);              // cw transition rotation
               Ef[e][0] += 1;                            // move right 1 to catch new wall
               break;
            }
            mv = mSolid.is_up_solidf(Ef[e][0], Ef[e][1], Ef[e][3], 0);
            if (mv < Ef[e][3]) // allowed move less than requested move
            {
               if (dp) printf("will hit ceiling during this move - mv%1.2f \n", mv);
               mode = PM_ENEMY_TRAKBOT_MODE_CEIL_LEFT;   // next mode (ccw)
               Ef[e][14] -= deg_to_rad(45);              // ccw transition rotation
               Ef[e][1] -= mv;                           // move the allowed amount
               break;
            }
            Ef[e][1] -= mv;  // if neither of these matched, move the allowed amount
         break;
         case PM_ENEMY_TRAKBOT_MODE_CEIL_LEFT:
            set_mode_draw_variables(e, PM_ENEMY_TRAKBOT_MODE_CEIL_LEFT);
            if (dp) { if (mSolid.is_up_solidf(Ef[e][0], Ef[e][1], 1, 0) < 1) printf(" - 2 with solid ceil\n"); else  printf(" - 2 with NOT solid ceil\n"); }
            mv = mSolid.is_up_solidf(Ef[e][0], Ef[e][1], Ef[e][2], -1);
            if (mv < Ef[e][2]) // allowed move less than requested move
            {
               if (dp) printf("will lose ceiling during this move - mv%1.2f \n", mv);
               Ef[e][0] -= mv;                           // move the allowed amount
               mode = PM_ENEMY_TRAKBOT_MODE_RWALL_UP;    // next mode (cw)
               Ef[e][14] += deg_to_rad(45);              // cw transition rotation
               Ef[e][1] -= 1;                            // move up 1 to catch new wall
               break;
            }
            mv = mSolid.is_left_solidf(Ef[e][0], Ef[e][1], Ef[e][2], 0);
            if (mv < Ef[e][2]) // allowed move less than requested move
            {
               if (dp) printf("will hit lwall during this move - mv%1.2f \n", mv);
               mode = PM_ENEMY_TRAKBOT_MODE_LWALL_DOWN;  // next mode (ccw)
               Ef[e][14] -= deg_to_rad(45);              // ccw transition rotation
               Ef[e][0] -= mv;                           // move the allowed amount
               break;
            }
            Ef[e][0] -= mv;  // if neither of these matched, move the allowed amount
         break;
         case PM_ENEMY_TRAKBOT_MODE_LWALL_DOWN:
            set_mode_draw_variables(e, PM_ENEMY_TRAKBOT_MODE_LWALL_DOWN);
            if (dp) { if (mSolid.is_left_solidf(Ef[e][0], Ef[e][1], 1, 0) < 1) printf(" - 3 with solid lwall\n"); else  printf(" - 3 with NOT solid lwall\n"); }
            mv = mSolid.is_left_solidf(Ef[e][0], Ef[e][1], Ef[e][3], 1);
            if (mv < Ef[e][3]) // allowed move less than requested move
            {
               if (dp) printf("will lose lwall during this move - mv%1.2f \n", mv);
               Ef[e][1] += mv;                           // move the allowed amount
               mode = PM_ENEMY_TRAKBOT_MODE_CEIL_LEFT;   // next mode (cw)
               Ef[e][14] += deg_to_rad(45);              // cw transition rotation
               Ef[e][0] -= 1;                            // move left 1 to catch new wall
               break;
            }
            mv = mSolid.is_down_solidf(Ef[e][0], Ef[e][1], Ef[e][3], 0);
            if (mv < Ef[e][3]) // allowed move less than requested move
            {
               if (dp) printf("will hit floor during this move - mv%1.2f \n", mv);
               mode = PM_ENEMY_TRAKBOT_MODE_FLOOR_RIGHT; // next mode (ccw)
               Ef[e][14] -= deg_to_rad(45);              // ccw transition rotation
               Ef[e][1] += mv;                           // move the allowed amount
               break;
            }
            Ef[e][1] += mv;  // if neither of these matched, move the allowed amount
         break;
         case PM_ENEMY_TRAKBOT_MODE_FLOOR_LEFT:
            set_mode_draw_variables(e, PM_ENEMY_TRAKBOT_MODE_FLOOR_LEFT);
            if (dp) { if (mSolid.is_down_solidf(Ef[e][0], Ef[e][1], 1, 0) < 1) printf(" - 4 with solid floor\n"); else  printf(" - 4 with NOT solid floor\n"); }
            mv = mSolid.is_down_solidf(Ef[e][0], Ef[e][1], Ef[e][2], -1);
            if (mv < Ef[e][2])                           // allowed move less than requested move
            {
               Ef[e][0] -= mv;                           // move the allowed amount
               mode = PM_ENEMY_TRAKBOT_MODE_RWALL_DOWN;  // next mode (ccw)
               Ef[e][14] -= deg_to_rad(45);              // ccw transition rotation
               Ef[e][1] += 1;                            // move down 1 to catch new wall
               if (dp) printf("lost floor during this move - mv%1.2f \n", mv);
               break;
            }
            mv = mSolid.is_left_solidf(Ef[e][0], Ef[e][1], Ef[e][2], 0);
            if (mv < Ef[e][2])                           // allowed move less than requested move
            {
               Ef[e][0] -= mv;                           // move the allowed amount
               mode = PM_ENEMY_TRAKBOT_MODE_LWALL_UP;    // next mode (cw)
               Ef[e][14] += deg_to_rad(45);              // cw transition rotation
               if (dp) printf("hit lwall during this move - mv%1.2f \n", mv);
               break;
            }
            Ef[e][0] -= mv;  // if neither of these matched, move the allowed amount
         break;
         case PM_ENEMY_TRAKBOT_MODE_LWALL_UP:
            set_mode_draw_variables(e, PM_ENEMY_TRAKBOT_MODE_LWALL_UP);
            if (dp) { if (mSolid.is_left_solidf(Ef[e][0], Ef[e][1], 1, 0) < 1) printf(" - 5 with solid left\n"); else  printf(" - 5 with NOT solid left\n"); }
            mv = mSolid.is_left_solidf(Ef[e][0], Ef[e][1], Ef[e][3], -1);
            if (mv < Ef[e][3])                           // allowed move less than requested move
            {
               Ef[e][1] -= mv;                           // move the allowed amount
               mode = PM_ENEMY_TRAKBOT_MODE_FLOOR_LEFT;  // next mode (ccw)
               Ef[e][14] -= deg_to_rad(45);              // ccw transition rotation
               Ef[e][0] -= 1;                            // move left 1 to catch new wall
               if (dp) printf("lost lwall during this move - mv%1.2f \n", mv);
               break;
            }
            mv = mSolid.is_up_solidf(Ef[e][0], Ef[e][1], Ef[e][3], 0);
            if (mv < Ef[e][3])                           // allowed move less than requested move
            {
               Ef[e][1] -= mv;                           // move the allowed amount
               mode = PM_ENEMY_TRAKBOT_MODE_CEIL_RIGHT;  // next mode (cw)
               Ef[e][14] += deg_to_rad(45);              // cw transition rotation
               if (dp) printf("hit ceil during this move - mv%1.2f \n", mv);
               break;
            }
            Ef[e][1] -= mv;  // if neither of these matched, move the allowed amount
         break;


         case PM_ENEMY_TRAKBOT_MODE_CEIL_RIGHT:
            set_mode_draw_variables(e, PM_ENEMY_TRAKBOT_MODE_CEIL_RIGHT);
            if (dp) { if (mSolid.is_up_solidf(Ef[e][0], Ef[e][1], 1, 0) < 1) printf(" - 6 with solid ceil\n"); else  printf(" - 6 with NOT solid ceil\n"); }
            mv = mSolid.is_up_solidf(Ef[e][0], Ef[e][1], Ef[e][2], 1);
            if (mv < Ef[e][2])                           // allowed move less than requested move
            {
               Ef[e][0] += mv;                           // move the allowed amount
               mode = PM_ENEMY_TRAKBOT_MODE_LWALL_UP;    // next mode (ccw)
               Ef[e][14] -= deg_to_rad(45);              // ccw transition rotation
               Ef[e][1] -= 1;                            // move up 1 to catch new wall
               if (dp) printf("lost ceil during this move - mv%1.2f \n", mv);
               break;
            }
            mv = mSolid.is_right_solidf(Ef[e][0], Ef[e][1], Ef[e][2], 0);
            if (mv < Ef[e][2])                           // allowed move less than requested move
            {
               Ef[e][0] += mv;                           // move the allowed amount
               mode = PM_ENEMY_TRAKBOT_MODE_RWALL_DOWN;  // next mode (cw)
               Ef[e][14] += deg_to_rad(45);              // cw transition rotation
               if (dp) printf("hit rwall during this move - mv%1.2f \n", mv);
               break;
            }
            Ef[e][0] += mv;  // if neither of these matched, move the allowed amount
         break;
         case PM_ENEMY_TRAKBOT_MODE_RWALL_DOWN:
            set_mode_draw_variables(e, PM_ENEMY_TRAKBOT_MODE_RWALL_DOWN);
            if (dp) { if (mSolid.is_right_solidf(Ef[e][0], Ef[e][1], 1, 0) < 1) printf(" - 7 with solid rwall\n"); else  printf(" - 7 with NOT solid rwall\n"); }
            mv = mSolid.is_right_solidf(Ef[e][0], Ef[e][1], Ef[e][3], 1);
            if (mv < Ef[e][3])                           // allowed move less than requested move
            {
               Ef[e][1] += mv;                           // move the allowed amount
               mode = PM_ENEMY_TRAKBOT_MODE_CEIL_RIGHT;  // next mode (ccw)
               Ef[e][14] -= deg_to_rad(45);              // ccw transition rotation
               Ef[e][0] += 1;                            // move right 1 to catch new wall
               if (dp) printf("lost rwall during this move - mv%1.2f \n", mv);
               break;
            }
            mv = mSolid.is_down_solidf(Ef[e][0], Ef[e][1], Ef[e][3], 0);
            if (mv < Ef[e][3])                           // allowed move less than requested move
            {
               Ef[e][1] += mv;                           // move the allowed amount
               mode = PM_ENEMY_TRAKBOT_MODE_FLOOR_LEFT;  // next mode (cw)
               Ef[e][14] += deg_to_rad(45);              // cw transition rotation
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
            m_parent.mShot.fire_enemy_shota(e, 20, p);
            Ei[e][16] = Ei[e][15]; // set new prox wait
         }
      }

   }  // end of if !fall


   // detect twirling (when mode changes every pass for more than 3 consecutive passes)


   if (Ei[e][5] == mode) // same mode as last pass
   {
      Ei[e][8] = 0; // reset twirling counter

   }
   else // different mode from last time
   {
      Ei[e][8]++; // increment the twirling counter
      Ei[e][5] = mode; // set the new mode
      if (Ei[e][8] > 3) // if twirling (3 mode changes in a row)
      {
         if (dp) printf("twirling detected!! - set fall mode\n");
         Ei[e][6] = 5; // start fall mode
         set_trakbot_mode(e, PM_ENEMY_TRAKBOT_MODE_FLOOR_RIGHT);
         Ei[e][8] = 0; // reset twirling counter
      }
   }




   // set the bitmap and drawing mode
   int b = Ei[e][3];    // ans
   if ((mode == PM_ENEMY_TRAKBOT_MODE_FLOOR_RIGHT) || (mode == PM_ENEMY_TRAKBOT_MODE_CEIL_RIGHT)) // x+
   {
      int ex = Ef[e][0];
      int s = ex % 6;
      Ei[e][1] = mBitmap.zz[10-s][b];
   }
   if ((mode == PM_ENEMY_TRAKBOT_MODE_RWALL_UP) || (mode == PM_ENEMY_TRAKBOT_MODE_LWALL_UP)) // y-
   {
      int ey = Ef[e][1];
      int s = ey % 6;
      Ei[e][1] = mBitmap.zz[5+s][b];
   }
   if ((mode == PM_ENEMY_TRAKBOT_MODE_CEIL_LEFT) || (mode == PM_ENEMY_TRAKBOT_MODE_FLOOR_LEFT)) // x-
   {
      int ex = Ef[e][0];
      int s = ex % 6;
      Ei[e][1] = mBitmap.zz[5+s][b];
   }
   if ((mode == PM_ENEMY_TRAKBOT_MODE_LWALL_DOWN) || (mode == PM_ENEMY_TRAKBOT_MODE_RWALL_DOWN)) // y+
   {
      int ey = Ef[e][1];
      int s = ey % 6;
      Ei[e][1] = mBitmap.zz[10-s][b];
   }
}


void mwEnemy::set_trakbot_mode(int e, int mode)
{
   Ei[e][5] = mode;
   set_mode_draw_variables(e, mode);
}

void mwEnemy::set_mode_draw_variables(int e, int mode)
{
   switch (mode)
   {
      case PM_ENEMY_TRAKBOT_MODE_FLOOR_RIGHT: Ei[e][4] = 1; Ef[e][14] = deg_to_rad(0);   break;
      case PM_ENEMY_TRAKBOT_MODE_RWALL_UP:    Ei[e][4] = 1; Ef[e][14] = deg_to_rad(270); break;
      case PM_ENEMY_TRAKBOT_MODE_CEIL_LEFT:   Ei[e][4] = 1; Ef[e][14] = deg_to_rad(180); break;
      case PM_ENEMY_TRAKBOT_MODE_LWALL_DOWN:  Ei[e][4] = 1; Ef[e][14] = deg_to_rad(90);  break;
      case PM_ENEMY_TRAKBOT_MODE_FLOOR_LEFT:  Ei[e][4] = 0; Ef[e][14] = deg_to_rad(0);   break;
      case PM_ENEMY_TRAKBOT_MODE_LWALL_UP:    Ei[e][4] = 0; Ef[e][14] = deg_to_rad(90);  break;
      case PM_ENEMY_TRAKBOT_MODE_CEIL_RIGHT:  Ei[e][4] = 0; Ef[e][14] = deg_to_rad(180); break;
      case PM_ENEMY_TRAKBOT_MODE_RWALL_DOWN:  Ei[e][4] = 0; Ef[e][14] = deg_to_rad(270); break;
   }
}






