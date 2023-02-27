// mwEnemyTrakbot.cpp

#include "pm.h"
#include "mwEnemy.h"
#include "mwPlayers.h"
#include "mwBitmap.h"
#include "mwShots.h"
#include "mwSolid.h"


//--8--trakbot-------------------------------------------------------------------
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


   if (Ei[e][4] != 0)   // if falling
   {
      if (dp) printf("mode:fall  x:%1.2f  y:%1.2f\n", Ef[e][0], Ef[e][1]);

      Ei[e][4] += 5;                                             // add acceleration to fsll
      if (Ei[e][4] > 160)  Ei[e][4] = 160;                      // terminal velocity
      float fall_yinc = Ef[e][3] * Ei[e][4] / 100;              // (fix * int) / int
      mv = mSolid.is_down_solidf(Ef[e][0], Ef[e][1], fall_yinc, 0);   // will we hit floor during this move?
      if (mv < fall_yinc) Ei[e][4] = 0;                          // end fall if allowed move less than requested move
      Ef[e][1] += mv;                                           // move the allowed amount
      if (Ef[e][1] > 1980) Ei[e][0]=0;                 // kill if falls past bottom of level

      // seek player while falling
      int p = find_closest_player(e);
      float ex = Ef[e][0];
      float px = mPlayer.syn[p].x;

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
            if ((mPlayer.syn[p].active) && (!mPlayer.syn[p].paused))
            {
               float px = mPlayer.syn[p].x;
               float py = mPlayer.syn[p].y;
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
            if (dp) { if (mSolid.is_down_solidf(Ef[e][0], Ef[e][1], 1, 0) < 1) printf(" - 0 with solid floor\n"); else  printf(" - 0 with NOT solid floor\n"); }
            mv = mSolid.is_down_solidf(Ef[e][0], Ef[e][1], Ef[e][2], 1);
            if (mv < Ef[e][2]) // allowed move less than requested move
            {
               if (dp) printf("will lose floor during this move - mv%1.2f \n", mv);
               Ef[e][0] += mv;                                  // move the allowed amount
               mode = 3;                                          // next mode (lwall down) (cw)
               Ef[e][14] = deg_to_rad(45);
               Ef[e][1] += 1;                           // move down 1 to catch new wall
               break;
            }
            mv = mSolid.is_right_solidf(Ef[e][0], Ef[e][1], Ef[e][2], 0);
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
            if (dp) { if (mSolid.is_right_solidf(Ef[e][0], Ef[e][1], 1, 0) < 1) printf(" - 1 with solid r wall\n"); else  printf(" - 1 with NOT solid r wall\n"); }
            mv = mSolid.is_right_solidf(Ef[e][0], Ef[e][1], Ef[e][3], -1);
            if (mv < Ef[e][3]) // allowed move less than requested move
            {
               if (dp) printf("will lose rwall during this move - mv%1.2f \n", mv);
               Ef[e][1] -= mv;                                  // move the allowed amount
               mode = 0;                                          // next mode (floor right) (cw)
               Ef[e][14] = deg_to_rad(315);
               Ef[e][0] += 1;                           // move right 1 to catch new wall
               break;
            }
            mv = mSolid.is_up_solidf(Ef[e][0], Ef[e][1], Ef[e][3], 0);
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
            if (dp) { if (mSolid.is_up_solidf(Ef[e][0], Ef[e][1], 1, 0) < 1) printf(" - 2 with solid ceil\n"); else  printf(" - 2 with NOT solid ceil\n"); }
            mv = mSolid.is_up_solidf(Ef[e][0], Ef[e][1], Ef[e][2], -1);
            if (mv < Ef[e][2]) // allowed move less than requested move
            {
               if (dp) printf("will lose ceiling during this move - mv%1.2f \n", mv);
               Ef[e][0] -= mv;                                  // move the allowed amount
               mode = 1;                                          // next mode (rwall up) (cw)
               Ef[e][14] = deg_to_rad(225);
               Ef[e][1] -= 1;                           // move up 1 to catch new wall
               break;
            }
            mv = mSolid.is_left_solidf(Ef[e][0], Ef[e][1], Ef[e][2], 0);
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
            if (dp) { if (mSolid.is_left_solidf(Ef[e][0], Ef[e][1], 1, 0) < 1) printf(" - 3 with solid lwall\n"); else  printf(" - 3 with NOT solid lwall\n"); }
            mv = mSolid.is_left_solidf(Ef[e][0], Ef[e][1], Ef[e][3], 1);
            if (mv < Ef[e][3]) // allowed move less than requested move
            {
               if (dp) printf("will lose lwall during this move - mv%1.2f \n", mv);
               Ef[e][1] += mv;                                  // move the allowed amount
               mode = 2;                                          // next mode  (ceil left) (cw)
               Ef[e][14] = deg_to_rad(135);
               Ef[e][0] -= 1;                           // move left 1 to catch new wall
               break;
            }
            mv = mSolid.is_down_solidf(Ef[e][0], Ef[e][1], Ef[e][3], 0);
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
            if (dp) { if (mSolid.is_down_solidf(Ef[e][0], Ef[e][1], 1, 0) < 1) printf(" - 4 with solid floor\n"); else  printf(" - 4 with NOT solid floor\n"); }

            mv = mSolid.is_down_solidf(Ef[e][0], Ef[e][1], Ef[e][2], -1);
            if (mv < Ef[e][2])                                  // allowed move less than requested move
            {
               Ef[e][0] -= mv;                                  // move the allowed amount
               mode = 7;                                          // next mode (rwall down) (ccw)
               Ef[e][14] = deg_to_rad(315);
               Ef[e][1] += 1;                           // move down 1 to catch new wall
               if (dp) printf("lost floor during this move - mv%1.2f \n", mv);
               break;
            }
            mv = mSolid.is_left_solidf(Ef[e][0], Ef[e][1], Ef[e][2], 0);
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
            if (dp) { if (mSolid.is_left_solidf(Ef[e][0], Ef[e][1], 1, 0) < 1) printf(" - 5 with solid left\n"); else  printf(" - 5 with NOT solid left\n"); }
            mv = mSolid.is_left_solidf(Ef[e][0], Ef[e][1], Ef[e][3], -1);
            if (mv < Ef[e][3])                                  // allowed move less than requested move
            {
               Ef[e][1] -= mv;                                  // move the allowed amount
               mode = 4;                                          // next mode (floor left) (ccw)
               Ef[e][14] = deg_to_rad(45);
               Ef[e][0] -= 1;                           // move left 1 to catch new wall
               if (dp) printf("lost lwall during this move - mv%1.2f \n", mv);
               break;
            }
            mv = mSolid.is_up_solidf(Ef[e][0], Ef[e][1], Ef[e][3], 0);
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
            if (dp) { if (mSolid.is_up_solidf(Ef[e][0], Ef[e][1], 1, 0) < 1) printf(" - 6 with solid ceil\n"); else  printf(" - 6 with NOT solid ceil\n"); }
            mv = mSolid.is_up_solidf(Ef[e][0], Ef[e][1], Ef[e][2], 1);
            if (mv < Ef[e][2])                                  // allowed move less than requested move
            {
               Ef[e][0] += mv;                                  // move the allowed amount
               mode = 5;                                          // next mode (lwall up ) (ccw)
               Ef[e][14] = deg_to_rad(135);
               Ef[e][1] -= 1;                           // move up 1 to catch new wall
               if (dp) printf("lost ceil during this move - mv%1.2f \n", mv);
               break;
            }
            mv = mSolid.is_right_solidf(Ef[e][0], Ef[e][1], Ef[e][2], 0);
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
            if (dp) { if (mSolid.is_right_solidf(Ef[e][0], Ef[e][1], 1, 0) < 1) printf(" - 7 with solid rwall\n"); else  printf(" - 7 with NOT solid rwall\n"); }
            mv = mSolid.is_right_solidf(Ef[e][0], Ef[e][1], Ef[e][3], 1);
            if (mv < Ef[e][3])                                  // allowed move less than requested move
            {
               Ef[e][1] += mv;                                  // move the allowed amount
               mode = 6;                                          // next mode (ceil right) (ccw)
               Ef[e][14] = deg_to_rad(225);
               Ef[e][0] += 1;                           // move right 1 to catch new wall
               if (dp) printf("lost rwall during this move - mv%1.2f \n", mv);
               break;
            }
            mv = mSolid.is_down_solidf(Ef[e][0], Ef[e][1], Ef[e][3], 0);
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
            mShot.fire_enemy_shota(e, 20, p);
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
      Ei[e][1] = mBitmap.zz[10-s][b];
   }
   if ((mode == 1) || (mode == 5)) // y- rwall up and lwall up
   {
      int ey = Ef[e][1];
      int s = ey % 6;
      Ei[e][1] = mBitmap.zz[5+s][b];
   }
   if ((mode == 2) || (mode == 4)) // x- ceil left or floor left
   {
      int ex = Ef[e][0];
      int s = ex % 6;
      Ei[e][1] = mBitmap.zz[5+s][b];
   }
   if ((mode == 3) || (mode == 7)) // y+ lwall down or rwall down
   {
      int ey = Ef[e][1];
      int s = ey % 6;
      Ei[e][1] = mBitmap.zz[10-s][b];
   }
}


void mwEnemy::set_trakbot_mode(int e, int mode)
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
