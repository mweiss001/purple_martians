// mwPlayer.cpp

#include "pm.h"
#include "mwPlayer.h"
#include "mwLog.h"
#include "mwTally.h"
#include "mwFont.h"
#include "mwBitmap.h"
#include "mwLift.h"
#include "mwColor.h"
#include "mwInput.h"
#include "mwDisplay.h"
#include "mwLoop.h"
#include "mwItem.h"
#include "mwEnemy.h"
#include "mwLevel.h"
#include "mwScreen.h"
#include "mwGameEvent.h"
#include "mwShot.h"
#include "mwSolid.h"
#include "mwNetgame.h"
#include "mwDemoRecord.h"
#include "mwGameMoves.h"
#include "mwEventQueue.h"
#include "mwConfig.h"
#include "mwMain.h"

mwPlayer mPlayer;



void mwPlayer::proc_player_health(int p)
{
   if ((mLoop.frame_num) && (mLoop.frame_num == loc[p].damage_holdoff)) mGameEvent.add(12, 0, 0, p, loc[p].damage_type, 0, loc[p].damage_tally);
   if (syn[p].old_health != syn[p].health)
   {
      loc[p].last_health_adjust = (syn[p].health - syn[p].old_health);
      syn[p].old_health = syn[p].health;
      loc[p].health_display = 80;
   }
   if (syn[p].health > 100)
   {
       syn[p].health = 100;     // enforce max
       syn[p].old_health = 100; // to prevent display
   }
   if (syn[p].health < 1)
   {
      syn[p].health = 0;
      loc[p].health_display = 200;
      syn[p].paused = 100;
      syn[p].paused_type = 1;
      if (!mLoop.ff_state)
      {
         mGameEvent.add(8, 0, 0, p, 0, 0, 0);  // player death
         mLog.add_headerf(LOG_NET, p, 0, "PLAYER:%d DIED!", p);
         mScreen.add_player_text_overlay(p, 3);
         mLevel.level_data_player_respawns++;
         syn[p].stat_respawns++;
      }
   }
}


void mwPlayer::proc_player_xy_move_test(int p)
{
   float m = 0.5;

   syn[p].xinc = 0;
   syn[p].yinc = 0;

   if (mInput.key[ALLEGRO_KEY_LCTRL][0]) m *= 4;

   if (syn[p].up)
   {
      syn[p].y -= m;
      syn[p].yinc = -m;
   }
   if (syn[p].down)
   {
      syn[p].y += m;
      syn[p].yinc = m;
   }
   if (syn[p].left)
   {
      syn[p].left_right = 0;
      syn[p].x -= m;
      syn[p].xinc = -m;
   }
   if (syn[p].right)
   {
      syn[p].left_right = 1;
      syn[p].x += m;
      syn[p].xinc = m;
   }
}

void mwPlayer::proc_player_xy_move(int p)
{
   int debug_print = 0;
   if (debug_print) printf("\nPlayer xy move\n");

   float gravity = 0.6;
   float slow_gravity = 0.2; // used when jump is held
   float initial_jump_velocity = -6.6;
   float terminal_velocity = 7.8;
   float x_accel = 0.12;
   float x_de_accel = 0.24;
   float max_x_velocity = 4;
   float initial_x_velocity = 1.15;





// -----------   x move  ---------------------
   if ((syn[p].left) && (!syn[p].right)) // left only
   {
      syn[p].left_right = 0;
      if (syn[p].left_xinc > -initial_x_velocity) syn[p].left_xinc = -initial_x_velocity;
      syn[p].left_xinc -= x_accel;     // accel left
      syn[p].right_xinc -= x_de_accel; // de accel right
   }

   if ((syn[p].right) && (!syn[p].left) ) // right only
   {
      syn[p].left_right = 1;
      if (syn[p].right_xinc < initial_x_velocity) syn[p].right_xinc = initial_x_velocity;
      syn[p].right_xinc += x_accel;   // accel right
      syn[p].left_xinc += x_de_accel; // de-accel left
   }

   if ((!syn[p].right) && (!syn[p].left) ) // neither left nor right pressed
   {
      syn[p].left_xinc  += x_de_accel; // de-accel left
      syn[p].right_xinc -= x_de_accel; // de accel right
   }

   if ((syn[p].right) && (syn[p].left) ) // both left and right pressed
   {
      syn[p].left_xinc  += x_de_accel; // de-accel left
      syn[p].right_xinc -= x_de_accel; // de accel right
   }

      // why don't I just do all the bounds checks here, just once.
   if (syn[p].right_xinc < 0) syn[p].right_xinc = 0;
   if (syn[p].right_xinc > max_x_velocity) syn[p].right_xinc = max_x_velocity;

   if (syn[p].left_xinc > 0 ) syn[p].left_xinc = 0;
   if (syn[p].left_xinc < -max_x_velocity) syn[p].left_xinc = -max_x_velocity;

   if (mSolid.is_right_solid(syn[p].x, syn[p].y, 0, 1)) syn[p].right_xinc = 0;
   if (mSolid.is_left_solid( syn[p].x, syn[p].y, 0, 1)) syn[p].left_xinc = 0;

   syn[p].xinc = syn[p].left_xinc + syn[p].right_xinc;  // calc xinc

   if (syn[p].player_ride) syn[p].xinc += mLift.cur[syn[p].player_ride-32].xinc; // if player is riding lift add lift xinc

   syn[p].x += syn[p].xinc; // apply xinc to players x


   if (int a = mSolid.is_left_solid(syn[p].x, syn[p].y, 1, 1))
   {
      if (a > 31) // player being pushed right by lift
      {
         if (!mSolid.is_right_solid(syn[p].x, syn[p].y, 0, 1))
         {
            int l = a-32; // lift that is pushing
            syn[p].x = mLift.cur[l].x + mLift.cur[l].w + 1; // snap to lift pos + width
            syn[p].right_xinc = mLift.cur[l].xinc;          // set players xinc from lift
         }
         else // player is getting squished
         {
            syn[p].health -= 1;
            mGameEvent.add(11, 0, 0, p, 9, 0, 100);
         }
      }
      else  if (syn[p].xinc < 0) // moving left and block collision
      {
         syn[p].left_xinc = 0;
         int x = syn[p].x;
         syn[p].x = (((x/20)+1) * 20) - 3;  // align with wall
      }
   }
   if (int a = mSolid.is_right_solid(syn[p].x, syn[p].y, 1, 1))
   {
      if (a > 31)    // player being pushed left by lift
      {
         if (!mSolid.is_left_solid(syn[p].x, syn[p].y, 0, 1))
         {
            int l = a-32; // lift that is pushing
            syn[p].x = mLift.cur[l].x - 20;            // snap to lift pos - 20
            syn[p].left_xinc = mLift.cur[a-32].xinc;  // set players xinc from lift
         }
         else // player is getting squished
         {
            syn[p].health -= 1;
            mGameEvent.add(11, 0, 0, p, 9, 0, 100);
         }
      }
      else if (syn[p].xinc > 0) // moving right and block collision
      {
         syn[p].right_xinc = 0;
         int x = syn[p].x;
         syn[p].x = ((x/20) * 20) + 2; // align with wall
      }
   }


// ---------------   check to see if player riding lift starts or ends --------------------------


   // check for lift below player
   // this is not ideal, there could be more than one lift and only the first one will detect
   int a = mSolid.is_down_solid(syn[p].x, syn[p].y, 1, 1);

   if (debug_print)
   {
      if (a == 0) printf("detect nothing solid below\n");
      if (a == 1) printf("detect solid block below\n");
      if ((a > 31) && (a == syn[p].player_ride)) printf("detect same lift below\n");
   }
   if (a < 32) syn[p].player_ride = 0; // no lift detected below player
   else if (syn[p].player_ride != a)   // lift detected below player, and player is not currently riding that lift
   {
      if (debug_print) printf("detect new lift below:%d\n", a );
      if (mLift.cur[a-32].yinc > 0)          // is lift going down
      {
         syn[p].player_ride = a;             // now we are riding this lift
         syn[p].y = mLift.cur[a-32].y - 20;  // align with lift
         syn[p].yinc = mLift.cur[a-32].yinc; // same speed as lift
      }
      if (mLift.cur[a-32].yinc <= 0)         // is lift going up or steady
      {
         int y = syn[p].y;
         int offset = mLift.cur[a-32].y - y;   // to prevent lift from picking up early
         if (debug_print) printf("offset from rising lift:%d\n", offset );
         if (offset < 21)
         {
            syn[p].player_ride = a;             // now we are riding this lift
            syn[p].y = mLift.cur[a-32].y - 20;  // align with lift
            syn[p].yinc = mLift.cur[a-32].yinc; // same speed as lift
         }
      }
   }


// -----------   y move  ---------------------
   if (syn[p].player_ride) // if player is riding lift
   {

      if (debug_print) printf("player riding lift\n");
      int d = syn[p].player_ride - 32; // lift number

      syn[p].yinc = mLift.cur[d].yinc;


      // check for collision with lift above
      if (mSolid.is_up_solid(syn[p].x, syn[p].y+1, 1, 1) > 31)
      {
         if (debug_print) printf("collison with lift above\n");
         syn[p].player_ride = 0;   // player knocked off lift due to collision above
         syn[p].health -= 1;       // take some damage
         mGameEvent.add(11, 0, 0, p, 9, 0, 100);
      }

      // if moving up and solid block above
      if ((mLift.cur[d].yinc < 0) && (mSolid.is_up_solid(syn[p].x, syn[p].y, 0, 1) == 1))
      {
         if (debug_print) printf("moving up and solid block above\n");
         syn[p].player_ride = 0;  // player knocked off lift
         syn[p].health -= 1;      // take some damage

         syn[p].y += 10;  // drop to make sure player falls off

         mGameEvent.add(11, 0, 0, p, 9, 0, 100);
      }

      // if moving down and solid block below
      if ((mLift.cur[d].yinc > 0) && (mSolid.is_down_solid(syn[p].x, syn[p].y, 0, 1))) // no lift check
      {
         if (debug_print) printf("moving down and solid block below\n");
         syn[p].player_ride = 0;
         int y = syn[p].y;
         syn[p].y = y - (y % 20);            // align with floor
      }

      if (syn[p].jump)
      {
         if (debug_print) printf("jump while riding lift\n");

         syn[p].player_ride = 0;
         syn[p].yinc = initial_jump_velocity + mLift.cur[d].yinc;

         syn[p].y += syn[p].yinc; // apply yinc
         mGameEvent.add(5, 0, 0, 0, 0, 0, 0);
      }

      if (syn[p].player_ride)
      {
         if (debug_print) printf("still riding\n");
         syn[p].y = mLift.cur[d].y - 20;  // if still riding, align with fy
      }

   }


   if (!syn[p].player_ride) // if player is not riding lift
   {
      if (debug_print) printf("player not riding lift\n");
      if ((syn[p].yinc < 0) && (syn[p].jump)) syn[p].yinc += slow_gravity; // if rising and jump pressed use slow gravity
      else syn[p].yinc += gravity;                                         // use regular gravity

      if (syn[p].yinc > terminal_velocity) syn[p].yinc = terminal_velocity; // check for terminal velocity
      syn[p].y += syn[p].yinc;             // apply yinc


      if (syn[p].yinc < 0) // if moving upwards
      {
         if (mSolid.is_up_solid(syn[p].x, syn[p].y+3, 1, 1))  // look for collision above
         {
            if (debug_print) printf("player moving up and collision\n");
            syn[p].y -= syn[p].yinc;        // take back move
            syn[p].yinc = 0;                // kill upwards motion
         }
      }

      if (syn[p].yinc >= 0) // if moving downwards or steady
      {
         if (mSolid.is_down_solid(syn[p].x, syn[p].y, 0, 1))  // check for floor below (no lift)
         {
            if (debug_print) printf("player moving down and collison\n");

            int y = syn[p].y;
            int ym20 = y % 20;

            if (ym20 < 19) // prevent warping one block up
            {
               syn[p].yinc = 0;              // kill downwards motion
               syn[p].y = y - ym20;          // align with floor
            }

            // check for collision with lift above if lift is moving down
            int a = mSolid.is_up_solid(syn[p].x, syn[p].y, 1, 1);
            if ((a > 31) && (mLift.cur[a-32].yinc > 0))
            {
               if (debug_print) printf("player moving down or steady and collison with lift moving down\n");
               syn[p].health -= 1; // take some damage
               mGameEvent.add(11, 0, 0, p, 9, 0, 100);
            }

            if (syn[p].jump)       // jump pressed
            {
               if (debug_print) printf("jump pressed\n");
               int a = mSolid.is_up_solid(syn[p].x, syn[p].y, 1, 1);
               if ((a == 0) || (a == 2))  // only jump if nothing above
               {
                  syn[p].yinc = initial_jump_velocity;
                  mGameEvent.add(5, 0, 0, 0, 0, 0, 0);
               }
            }
         }  // end of if floor below
      } // end of if falling or steady
   } // end of not player ride
}



void mwPlayer::proc_player_paused(int p)
{
   syn[p].player_ride = 0;

   if (syn[p].paused_type == 2) mItem.proc_player_door_move(p);

   if (syn[p].paused_type == 3) syn[p].paused = 1; // headless server player


   if (syn[p].paused_type == 1) // frozen after player dies, until the timer runs out
   {
      syn[p].carry_item = 0;
      if (syn[p].paused == 100) // do only once per death
      {
         // does this player have any bomb remotes?
         for (int i=0; i<500; i++)
            if ((mItem.item[i][0] == 99) && (mItem.item[i][6] == 3) && (mItem.item[i][13] == p))  // lit bomb with remote detonator that was lit by this player
            {
               mItem.item[i][0] = 8; // change back to unlit bomb or...

               // set bomb to explode!
               //mItem.item[i][6] = 2; // mode 2; explosion
               //mItem.item[i][8] = mItem.item[i][9] = 20; // explosion timer
            }
      }
      if (--syn[p].paused > 0)
      {
         // shrink player
         syn[p].draw_scale -= 0.25;
         if (syn[p].draw_scale < 0) syn[p].draw_scale = 0;

         // rotate player
         if (syn[p].left_right) syn[p].draw_rot += 5;
         else                   syn[p].draw_rot -= 5;
      }
      else // frozen done
      {
         syn[p].paused = 0;
         syn[p].old_health = 100;
         syn[p].health = 100;
         syn[p].left_xinc = 0;
         syn[p].right_xinc = 0;
         syn[p].xinc = 0;
         syn[p].yinc = 0;
         mItem.set_player_start_pos(p);
      }
   }
}

void mwPlayer::reset_player_scale_and_rot(int p)
{
   syn[p].draw_scale = 1;
   syn[p].draw_rot = 0;
}


int mwPlayer::is_player_riding_rocket(int p)
{
   if ((syn[p].carry_item) && (mItem.item[syn[p].carry_item-1][0] == 98)) return 1;
   return 0;
}

void mwPlayer::proc_player_stuck_in_blocks(int p)
{
   int x = syn[p].x;
   int y = syn[p].y;
   int su =    mSolid.is_up_solid(x, y, 0, 1);
   int sd =  mSolid.is_down_solid(x, y, 0, 1);
   int sl =  mSolid.is_left_solid(x, y, 0, 1);
   int sr = mSolid.is_right_solid(x, y, 0, 1);
   if ((su) && (sd) && (sl) && (sr))
   {
      syn[p].health -= 1;
      mGameEvent.add(11, x, y, p, 7, 0, 100);
   }
}

void mwPlayer::proc_player_riding_rocket(int p)
{
   int c = syn[p].carry_item-1;
   float rot_inc = mItem.item[c][6];
   if (syn[p].left)  mItem.item[c][10] -= rot_inc;
   if (syn[p].right) mItem.item[c][10] += rot_inc;

   syn[p].x    = mItem.itemf[c][0];  // set the player's position and incs the same as the rocket
   syn[p].y    = mItem.itemf[c][1];
   syn[p].xinc = mItem.itemf[c][2];
   syn[p].yinc = mItem.itemf[c][3];

   syn[p].left_xinc = syn[p].right_xinc = 0;
   syn[p].draw_rot = (float)mItem.item[c][10] / 1000;
   syn[p].draw_scale = 0.5;
}

void mwPlayer::proc_player_bounds_check(int p)
{
   // absolute level bounds check
   if (syn[p].x < 0)    syn[p].x = 0;
   if (syn[p].y < 0)    syn[p].y = 0;
   if (syn[p].x > 1980) syn[p].x = 1980;
   if (syn[p].y > 1980) syn[p].y = 1980;
}


void mwPlayer::proc_player_collisions(int p)
{
   float px = syn[p].x;
   float py = syn[p].y;

   // items
   loc[p].potential_bomb_damage = 0;
   syn[p].marked_door = -1; // so player can touch only one door
   syn[p].marked_gate = -1; // so player can touch only one gate

   for (int x=0; x<500; x++)
   {
      if ((mItem.item[x][0]) && (mItem.item[x][0] != 9) && (mItem.item[x][0] != 16) && (mItem.item[x][0] != 17))
      {
         float ix1 = mItem.itemf[x][0] - 16;
         float ix2 = mItem.itemf[x][0] + 16;
         float iy1 = mItem.itemf[x][1] - 16;
         float iy2 = mItem.itemf[x][1] + 16;
         if ((px > ix1) && (px < ix2) && (py > iy1) && (py < iy2) && (!syn[p].paused) ) mItem.proc_item_collision(p, x);
      }
   }

   // enemies
   for (int e=0; e<100; e++)
   {
      if ((mEnemy.Ei[e][0]) && (mEnemy.Ei[e][0] != 99)) // if active and not deathcount
      {
         float b = mEnemy.Ei[e][29]; // collision box size
         float ex1 = mEnemy.Ef[e][0] - b;
         float ex2 = mEnemy.Ef[e][0] + b;
         float ey1 = mEnemy.Ef[e][1] - b;
         float ey2 = mEnemy.Ef[e][1] + b;
         if ((px > ex1) && (px < ex2) && (py > ey1) && (py < ey2)) mEnemy.Ei[e][22] = p+1;
      }
   }

   // eshots
   for (int b=0; b<50; b++)
      if (mShot.e[b].active)  // if active
      {
         // new collision box is based on shot speed and has both x and y component
         float ax = fabs(mShot.e[b].xinc);
         float ay = fabs(mShot.e[b].yinc);

         // enforce minimums
         if (ax < 4) ax = 4;
         if (ay < 4) ay = 4;

         float bx1 = mShot.e[b].x - ax;
         float bx2 = mShot.e[b].x + ax;
         float by1 = mShot.e[b].y - ay;
         float by2 = mShot.e[b].y + ay;

         if ((px > bx1) && (px < bx2) && (py > by1) && (py < by2)) mShot.proc_eshot_collision(p, b);
      }

   // pshots
   for (int b=0; b<50; b++)
      if (mShot.p[b].active)
      {
         float bx1 = mShot.p[b].x - 10;
         float bx2 = mShot.p[b].x + 10;
         float by1 = mShot.p[b].y - 10;
         float by2 = mShot.p[b].y + 10;

         if ((px > bx1) && (px < bx2) && (py > by1) && (py < by2))
         {
            int pb = mShot.p[b].player; // player that fired the shot

            if ((syn[0].player_vs_player_shots) && (pb != p))
            {
               mShot.proc_pshot_collision(p, b);
               mGameEvent.add(40, 0, 0, pb, p, 1, syn[0].player_vs_player_shot_damage);
               mPlayer.syn[pb].stat_player_hits++;
            }
            if ((syn[0].player_vs_self_shots) && (pb == p))
            {
               mShot.proc_pshot_collision(p, b);
               mGameEvent.add(40, 0, 0, pb, p, 1, syn[0].player_vs_player_shot_damage);
               mPlayer.syn[pb].stat_self_hits++;
            }
         }
      }
}


int mwPlayer::is_player_within_ladder_reach(int p)
{
   // detect if player is on ladder block
   int AX = syn[p].x;
   int AY = syn[p].y;

   int bx2 = 0, bx1 = AX / 20;
   int am = AX % 20;
   if (am < 9) // this block only
   {
      bx2 = bx1;
   }
   if (am > 10) // next block only
   {
      bx1++;
      bx2 = bx1;
   }
   if ((am > 8) && (am < 11)) // both blocks
   {
      bx2 = bx1 + 1;
   }

   int by2 = 0, by1 = AY / 20;
   am = AY % 20;
   if (am < 9) // this block only
   {
      by2 = by1;
   }
   if (am > 10) // next block only
   {
      by1++;
      by2 = by1;
   }
   if ((am > 8) && (am < 11)) // both blocks
   {
      by2 = by1 + 1;
   }
   for (int x=bx1; x<=bx2; x++)
      for (int y=by1; y<=by2; y++)
      {
         if (mLevel.l[x][y] & PM_BTILE_LADDER_MOVE) return 1;
      }
   return 0;
}

int mwPlayer::is_player_within_rope_reach(int p)
{
   // detect if player is on rope block
   int AX = syn[p].x;
   int AY = syn[p].y;

   int bx2 = 0, bx1 = AX / 20;
   int am = AX % 20;
   if (am < 9) // this block only
   {
      bx2 = bx1;
   }
   if (am > 10) // next block only
   {
      bx1++;
      bx2 = bx1;
   }
   if ((am > 8) && (am < 11)) // both blocks
   {
      bx2 = bx1 + 1;
   }


   int by = AY / 20;
   am = AY % 20;

   //al_draw_textf(mFont.pr8, mColor.pc[15], AX, AY-20, 0, "%d", am );
   int good_height = 0;

   if (am < 5) // this block only
   {
      good_height = 1;
   }
   if (am > 15) // next block only
   {
      good_height = 1;
      by++;
   }

   if (good_height)
   for (int x=bx1; x<=bx2; x++)
   {
      if (mLevel.l[x][by] & PM_BTILE_ROPE_MOVE) return 1;
   }
   return 0;
}


void mwPlayer::proc_player_rope_move(int p)
{
    // reset all regular incs
   syn[p].xinc = 0;
   syn[p].yinc = 0;
   syn[p].left_xinc = 0;
   syn[p].right_xinc = 0;

   if (syn[p].left)
   {
      syn[p].left_right = 0;
      syn[p].x -= mSolid.is_left_solidf(syn[p].x, syn[p].y, 3, 0);
   }
   if (syn[p].right)
   {
      syn[p].left_right = 1;
      syn[p].x += mSolid.is_right_solidf(syn[p].x, syn[p].y, 3, 0);
   }
   if (syn[p].down)
   {
      syn[p].on_rope = 0;
      syn[p].y += 5;
   }
}



void mwPlayer::proc_player_ladder_move(int p)
{
   float m = 3;

   int old_px = syn[p].x;
   int old_py = syn[p].y;

   // reset all regular incs
   syn[p].xinc = 0;
   syn[p].yinc = 0;
   syn[p].left_xinc = 0;
   syn[p].right_xinc = 0;

   if (syn[p].jump)
   {
      syn[p].on_ladder = 0;
      syn[p].yinc = -6.6;       // initial_jump_velocity;
      syn[p].y += syn[p].yinc;  // make the first move here
      return;
   }
   else
   {
      if (syn[p].up) syn[p].y -= mSolid.is_up_solidf(syn[p].x, syn[p].y, m, 0);
   }
   if (syn[p].down) syn[p].y += mSolid.is_down_solidf(syn[p].x, syn[p].y, m, 0);
   if (syn[p].left)
   {
      syn[p].left_right = 0;
      syn[p].x -= mSolid.is_left_solidf(syn[p].x, syn[p].y, m, 0);
   }
   if (syn[p].right)
   {
      syn[p].left_right = 1;
      syn[p].x += mSolid.is_right_solidf(syn[p].x, syn[p].y, m, 0);
   }


   // how much did we move this last frame?
   int px = syn[p].x;
   int py = syn[p].y;
   int xd = px - old_px;
   int yd = py - old_py;

   syn[p].xinc = xd;
   syn[p].yinc = yd;

//   printf("1 - %d yd:%d\n", mLoop.frame_num, yd);

   // did we try to move up past top of ladder?
   if (yd < 0) if (!is_player_within_ladder_reach(p)) syn[p].y += m;


   // did we pass by any single block openings??
   // if only x move
   if ((xd != 0) && (yd == 0))
   {
      if (xd > 0) // x+
      {
         for (int tx = old_px; tx <= px; tx++)
         {
            if (syn[p].up)
            {
               float tm = mSolid.is_up_solidf(tx, syn[p].y, m, 0);
               if (tm > 0)
               {
                  //printf("%d px:%d opx:%d tx:%d x+u\n", mLoop.frame_num, px, old_px, tx );
                  syn[p].x = tx; // set x to passed by pos
                  syn[p].y -= m; // move in y
                  tx = px+1; // break out of loop
               }
            }
            else if (syn[p].down)
            {
               float tm = mSolid.is_down_solidf(tx, syn[p].y, m, 0);
               if (tm > 0)
               {
                  //printf("%d px:%d opx:%d tx:%d x+d\n", mLoop.frame_num, px, old_px, tx );
                  syn[p].x = tx; // set x to passed by pos
                  syn[p].y += m; // move in y
                  tx = px+1; // break out of loop
               }
            }
         }
      }
      if (xd < 0) // x-
      {
         for (int tx = px; tx <= old_px; tx++)
         {
            if (syn[p].up)
            {
               float tm = mSolid.is_up_solidf(tx, syn[p].y, m, 0);
               if (tm > 0)
               {
                  //printf("%d px:%d opx:%d tx:%d x-u\n", mLoop.frame_num, px, old_px, tx );
                  syn[p].x = tx; // set x to passed by pos
                  syn[p].y -= m; // move in y
                  tx = old_px+1; // break out of loop
               }
            }
            else if (syn[p].down)
            {
               float tm = mSolid.is_down_solidf(tx, syn[p].y, m, 0);
               if (tm > 0)
               {
                  //printf("%d px:%d opx:%d tx:%d x-d\n", mLoop.frame_num, px, old_px, tx );
                  syn[p].x = tx; // set x to passed by pos
                  syn[p].y += m; // move in y
                  tx = old_px+1; // break out of loop
               }
            }
         }
      }
   }

   // if only y move
   if ((yd != 0) && (xd == 0))
   {
      if (yd > 0) // y+
      {
         for (int ty1 = old_py; ty1 <= py; ty1++)
         {
            if (syn[p].right)
            {
               float tm = mSolid.is_right_solidf(syn[p].x, ty1, m, 0);
               if (tm > 0)
               {
                  //printf("%d py:%d opy:%d ty1:%d y+r\n", mLoop.frame_num, py, old_py, ty1 );
                  syn[p].y = ty1; // set y to passed by pos
                  syn[p].x += m; // move in x
                  ty1 = py+1; // break out of loop
               }
            }
            else if (syn[p].left)
            {
               float tm = mSolid.is_left_solidf(syn[p].x, ty1,  m, 0);
               if (tm > 0)
               {
                  //printf("%d py:%d opy:%d ty1:%d y+l\n", mLoop.frame_num, py, old_py, ty1 );
                  syn[p].y = ty1; // set y to passed by pos
                  syn[p].x -= m; // move in x
                  ty1 = py+1; // break out of loop
               }
            }
         }
      }
      if (yd < 0) // y-
      {
         for (int ty1 = py; ty1 <= old_py; ty1++)
         {
            if (syn[p].right)
            {
               float tm = mSolid.is_right_solidf(syn[p].x, ty1, m, 0);
               if (tm > 0)
               {
                  //printf("%d py:%d opy:%d ty1:%d y-r\n", mLoop.frame_num, py, old_py, ty1 );
                  syn[p].y = ty1; // set y to passed by pos
                  syn[p].x += m; // move in x
                  ty1 = old_py+1; // break out of loop
               }
            }
            else if (syn[p].left)
            {
               float tm = mSolid.is_left_solidf(syn[p].x, ty1,  m, 0);
               if (tm > 0)
               {
                  //printf("%d py:%d opy:%d ty1:%d y-l\n", mLoop.frame_num, py, old_py, ty1 );
                  syn[p].y = ty1; // set y to passed by pos
                  syn[p].x -= m; // move in x
                  ty1 = old_py+1; // break out of loop
               }
            }
         }
      }
   }


//   yd = syn[p].y - old_py;
//   printf("2 - %d yd:%d\n", mLoop.frame_num, yd);




}

void mwPlayer::proc_player_rope(int p)
{
   if (syn[p].on_rope)                 // if player is currently on rope
   {
      if (!is_player_within_rope_reach(p)) // check to see if player is still on rope
      {
         syn[p].on_rope = 0;
         syn[p].on_ladder = is_player_within_ladder_reach(p); // automatically grab ladder if with reach and just leaving rope
      }
   }
   else
   {
      if ( (is_player_within_rope_reach(p)) &&                   // not already on rope and within rope reach
         ((syn[p].yinc > 0) || (syn[p].up)) ) // player is falling or up is pressed
      {
         // plater just got on rope
         syn[p].on_rope = 1;
         syn[p].on_ladder = 0;

         // snap y to rope
         int AY = syn[p].y;
         int by = AY / 20;
         int am = AY % 20;

         if (am <  5) syn[p].y = (by+0) * 20; // this block only
         if (am > 15) syn[p].y = (by+1) * 20; // next block only

         // check to see if player is embedded in wall to right
         int done = 0;
         while (!done)
         {
            float m = mSolid.is_right_solidf(syn[p].x-1, syn[p].y, 1, 0);
            if (m < 1) syn[p].x -= 1;
            else done = 1;
         }
         // check to see if player is embedded in wall to left
         done = 0;
         while (!done)
         {
            float m = mSolid.is_left_solidf(syn[p].x+1, syn[p].y, 1, 0);
            if (m < 1) syn[p].x += 1;
            else done = 1;
         }
      }
   }
   if (syn[p].on_rope) proc_player_rope_move(p);
}

void mwPlayer::proc_player_ladder(int p)
{
   if (syn[p].on_ladder) // already on ladder (grabbing ladder)
   {
      // check to see if player is still with ladder reach
      syn[p].on_ladder = is_player_within_ladder_reach(p);
   }
   else // not initially grabbing ladder when function started
   {
      if (is_player_within_ladder_reach(p) && (syn[p].up) && (!syn[p].jump)) // within ladder reach and up pressed AND jump not pressed
      {
         // just got on ladder
         syn[p].on_ladder = 1;
         syn[p].on_rope = 0;

         // check to see if player is embedded in wall to right
         int done = 0;
         while (!done)
         {
            float m = mSolid.is_right_solidf(syn[p].x-1, syn[p].y, 1, 0);
            //printf("R %f %f\n", m, syn[p].x);
            if (m < 1) syn[p].x -= 1;
            else done = 1;
         }

         // check to see if player is embedded in wall to left
         done = 0;
         while (!done)
         {
            float m = mSolid.is_left_solidf(syn[p].x-1, syn[p].y, 1, 0);
            //printf("L %f %f\n", m, syn[p].x);
            if (m < 1) syn[p].x += 1;
            else done = 1;
         }
      }
   }
   if (syn[p].on_ladder) proc_player_ladder_move(p);
}

void mwPlayer::move_players(void)
{
   if (mMain.headless_server)
   {
      syn[0].paused = 1;
      syn[0].paused_type = 3;
   }
   for (int p=0; p<NUM_PLAYERS; p++)
   {
      mScreen.proc_player_text_overlay_timer(p);
      if (syn[p].active)
      {

         if (syn[p].paused) proc_player_paused(p);
         else // not paused
         {
            reset_player_scale_and_rot(p);
            if (is_player_riding_rocket(p)) proc_player_riding_rocket(p);
            else // not riding rocket
            {
               proc_player_rope(p);
               proc_player_ladder(p);
               if (!syn[p].on_ladder && !syn[p].on_rope) // not on ladder or rope
                  //proc_player_xy_move_test(p);
                  proc_player_xy_move(p);
            }
            // common to all not paused modes
            mShot.proc_player_shoot(p);
            mItem.proc_player_carry(p);
            proc_player_collisions(p);
            proc_player_health(p);
            proc_player_bounds_check(p);
            proc_player_stuck_in_blocks(p);
         }
      }
   }
}
















void mwPlayer::draw_player(int p)
{
   if (syn[p].paused_type != 3)
   {
      al_set_target_bitmap(mBitmap.level_buffer);
      int px = syn[p].x;
      int py = syn[p].y;
      set_players_shape(p);



//      int old_draw_method = 1;
//      if (mLoop.state[0] == PM_PROGRAM_STATE_MAIN_GAME_LOOP) old_draw_method = 0;
//      if ((mLoop.state[0] == PM_PROGRAM_STATE_DEMO_RECORD) && (mDemoRecord.play)) old_draw_method = 0;
//      if (syn[0].level_done_mode == 27) old_draw_method = 1;
//      int old_draw_method = 0;
//      if (syn[0].level_done_mode == 27) old_draw_method = 1;
//      if (old_draw_method)

      float scale = syn[p].draw_scale;
      float rot = syn[p].draw_rot;
      if ((scale == 1) && (rot == 0))
      {
         int flags = ALLEGRO_FLIP_HORIZONTAL;
         if (syn[p].left_right) flags = ALLEGRO_FLIP_VERTICAL & ALLEGRO_FLIP_HORIZONTAL;
         al_draw_scaled_rotated_bitmap(mBitmap.player_tile[syn[p].color][syn[p].shape], 10, 10, px+10, py+10, scale, scale, rot, flags);


   //      printf("color:%d shape:%d\n", syn[p].color, syn[p].shape );

         /*

         if (syn[p].on_ladder)
            al_draw_rectangle(0.5+AX, 0.5+AY, 0.5+AX+19, 0.5+AY+19, mColor.pc[11], 1);

         if (syn[p].on_rope)
            al_draw_rectangle(0.5+AX, 0.5+AY, 0.5+AX+17, 0.5+AY+19, mColor.pc[10], 1);


          detect block that player is on...

         int bx2, bx1 = AX / 20;
         int am = AX % 20;
         if (am < 9) // this block only
         {
            bx2 = bx1;
         }
         if (am > 10) // next block only
         {
            bx1++;
            bx2 = bx1;
         }
         if ((am > 8) && (am < 11)) // both blocks
         {
            bx2 = bx1 + 1;
         }

         int by = AY / 20;
         am = AY % 20;

         al_draw_textf(mFont.pr8, mColor.pc[15], AX, AY-20, 0, "%d", am );

         int good_height = 0;

         if (am < 5) // this block only
         {
            good_height = 1;
         }
         if (am > 15) // next block only
         {
            good_height = 1;
            by++;
         }

         if (good_height)
            if (timer_draw_mode2) al_draw_textf(mFont.pr8, mColor.pc[col], x0+10, y0+6, ALLEGRO_ALIGN_CENTER, "%d", tts);
            if (timer_draw_mode2) al_draw_textf(mFont.pr8, mColor.pc[col], x0+10, y0+6, ALLEGRO_ALIGN_CENTER, "%d", tts);
            if (timer_draw_mode2) al_draw_textf(mFont.pr8, mColor.pc[col], x0+10, y0+6, ALLEGRO_ALIGN_CENTER, "%d", tts);
              if (timer_draw_mode2) al_draw_textf(mFont.pr8, mColor.pc[col], x0+10, y0+6, ALLEGRO_ALIGN_CENTER, "%d", tts);
       for (int x=bx1; x<=bx2; x++)
         {
            al_draw_rectangle(0.5+x*20, 0.5+by*20, 0.5+(x*20)+19, 0.5+(by*20)+19, mColor.pc[8], 1);
            if (l[x][by] == 19) al_draw_rectangle(0.5+x*20, 0.5+by*20, 0.5+(x*20)+19, 0.5+(by*20)+19, mColor.pc[11], 1);
         }
   */

      }


      if (mLoop.state[1] == PM_PROGRAM_STATE_DEMO_RECORD)
      {
         if (p == mDemoRecord.record_player_number)
         {
            al_draw_textf(mFont.pr8, mColor.pc[15], px+10, py-38, ALLEGRO_ALIGN_CENTER, "x:%2.1f y:%2.1f", syn[p].x, syn[p].y);
            al_draw_textf(mFont.pr8, mColor.pc[15], px+10, py-30, ALLEGRO_ALIGN_CENTER, "xi:%2.1f yi:%2.1f", syn[p].xinc, syn[p].yinc);
         }
      }




      // death sequence star overlay
      if ((syn[p].paused) && (syn[p].paused_type == 1))
      {
         int pp = syn[p].paused;
         int st = 96; // frame to start at
         int sp = 3;  // speed of sequence
         if ((pp < st) && (pp > st-sp*8))
         {
            int seq = (st-syn[p].paused) / sp;
            al_draw_bitmap(mBitmap.tile[952+seq], px, py, 0);
         }
      }


      // health bar overlay
      if (loc[p].health_display > 0)
      {
         loc[p].health_display--;

         // show current health bar
         int ch = syn[p].health; // current health
         mScreen.draw_percent_bar(px+10, py-6, 16, 3, ch);

         // show last health adjustment
         int h = loc[p].last_health_adjust; // last health adjust
         if (h > 0) al_draw_textf(mFont.pixl, mColor.pc[11], px+10, py-16, ALLEGRO_ALIGN_CENTER, "%+d", h);
         if (h < 0) al_draw_textf(mFont.pixl, mColor.pc[10], px+10, py-16, ALLEGRO_ALIGN_CENTER, "%+d", h);

         // show potential bomb damage
         int dmg = loc[p].potential_bomb_damage; // potential bomb damage
         if (dmg)
         {
            int nh = ch - dmg; // new health
            if (nh < 0) nh = 0;
            if (mLoop.frame_num % 20 < 10)
            {
               // draw segment from new health to current health
               mScreen.draw_percent_bar_range(px+10, py-6, 16, 3, 10, nh, ch);

               // show damage amount
               al_draw_textf(mFont.pixl, mColor.pc[10], px+10, py-16, ALLEGRO_ALIGN_CENTER, "%+d", -dmg);

               // draw a tiny bomb picture
               al_draw_scaled_rotated_bitmap(mBitmap.tile[464], 10, 10,  px+20, py-12, .5, .5, 0, 0);
            }
         }
      }
   }
}

void mwPlayer::draw_players(void)
{
   for (int p=0; p<NUM_PLAYERS; p++)
      if (syn[p].active) draw_player(p);
   draw_player(active_local_player); // do this so that that local player is always drawn on top

   // in some weird cases (demo record) active local player might not be active
   if (mLoop.state[1] == PM_PROGRAM_STATE_DEMO_RECORD)
      if (syn[active_local_player].active) draw_player(active_local_player);
}

void mwPlayer::draw_player_direct_to_screen(int p)
{
   if (syn[p].paused_type != 3)
   {
      float scale = syn[p].draw_scale;
      float rot = syn[p].draw_rot;
      if ((scale != 1) || (rot != 0))
      {
         float px, py;
         mScreen.calc_actual_screen_position(syn[p].x+10, syn[p].y+10, px, py);
         float scale = mPlayer.syn[p].draw_scale * mDisplay.scale_factor_current;
         int flags = ALLEGRO_FLIP_HORIZONTAL;
         if (mPlayer.syn[p].left_right) flags = ALLEGRO_FLIP_VERTICAL & ALLEGRO_FLIP_HORIZONTAL;
         al_draw_scaled_rotated_bitmap(mBitmap.player_tile[mPlayer.syn[p].color][mPlayer.syn[p].shape], 10, 10, px, py, scale, scale, mPlayer.syn[p].draw_rot, flags);
         // printf("%d - draw direct\n", mLoop.frame_num);
      }
   }
}


void mwPlayer::draw_players_direct_to_screen(void)
{
   for (int p=0; p<NUM_PLAYERS; p++)
      if (syn[p].active) draw_player_direct_to_screen(p);

   // do this so that that local player is always drawn on top
   mPlayer.draw_player_direct_to_screen(mPlayer.active_local_player);

   // in some weird cases (demo record) active local player might not be active
   if (mLoop.state[1] == PM_PROGRAM_STATE_DEMO_RECORD)
      if (syn[active_local_player].active) draw_player_direct_to_screen(active_local_player);
}



void mwPlayer::set_players_shape(int p)
{
   int index = 0;
   if (syn[p].up) index = 6;
   if (syn[p].down) index = 12;

   // animate with x pos
   int pos = ((int) syn[p].x / 6) % 6;  // 6 shapes in sequence


   // if player riding lift animate with player's xpos relative to lift
   if (syn[p].player_ride)
   {
      int rx = 20 + (int) (syn[p].x - mLift.cur[syn[p].player_ride-32].x);
//      printf("rx:%d\n", rx);


      pos = (rx / 4) % 5;  // try 5 for now
   }

   // paused can mean dead or it can also mean in door travel
   // if paused use static shape
   if (syn[p].paused) syn[p].shape = 0;
   else syn[p].shape = index + pos;

   // if jump or fall and not riding lift use static shape
   if ((syn[p].yinc != 0) && (!syn[p].player_ride)) syn[p].shape = 19;


   // if riding rocket use static shape
   if ((syn[p].carry_item) && (mItem.item[syn[p].carry_item-1][0] == 98)) syn[p].shape = 1;


   if (syn[p].on_rope)
   {
      // animate with x pos
      int x = syn[p].x;

      // 2 shapes in sequence
      // 3 is rope move
      int pos = (x/3) % 2;
      syn[p].shape = 20 + pos;
      // printf("f:%d x:%d pos:%d, shape:%d\n", mLoop.frame_num, x, pos, shape );
   }

   if (syn[p].on_ladder)
   {
      // animate with y pos
      int y = syn[p].y;

      // 2 shapes in sequence
      // 3 is ladder move
      int pos = (y/3) % 2;
      syn[p].shape = 22 + pos;
      //printf("f:%d y:%d pos:%d, shape:%d\n", mLoop.frame_num, y, pos, shape );
   }
}


int mwPlayer::find_inactive_player(void)
{
   for (int p=1; p<NUM_PLAYERS; p++)
      if (!syn[p].active) return p;
   return 99;
}

int mwPlayer::is_player_color_used(int color)
{
   for (int p=0; p<NUM_PLAYERS; p++)
      if ((syn[p].active) && (syn[p].paused_type != 3)) // active and not hidden player in server headless
         if (color == syn[p].color) return 1;
   return 0;
}




int mwPlayer::get_new_client_color(int color)
{
   // try to use requested color, unless already used by another player
   if (!is_player_color_used(color)) return color;

   // custom list of what colors to try next
   if (!is_player_color_used(8))  return 8;  // purple
   if (!is_player_color_used(10)) return 10; // red
   if (!is_player_color_used(12)) return 12; // dark blue
   if (!is_player_color_used(9))  return 9;  // lt green
   if (!is_player_color_used(13)) return 13; // lt blue
   if (!is_player_color_used(14)) return 14; // yellow
   if (!is_player_color_used(15)) return 15; // white
   if (!is_player_color_used(6))  return 6;  // taan

   // if all these fail, just search for next unused
   while (mPlayer.is_player_color_used(color)) if (++color > 15) color = 1;

   return color;
}






void mwPlayer::init_player(int p, int t)
{
   if (t == 1) // new game
   {
      syn[p].active = 0;
      syn[p].paused = 0;
      syn[p].control_method = PM_PLAYER_CONTROL_METHOD_SINGLE_PLAYER;

      syn[p].old_health = 100;
      syn[p].health = 100;

      loc[p].hostname[0] = 0;
      loc[p].fake_keypress_mode = 0;
      syn[p].server_force_fakekey = 0;

      init_player(p, 17); // clear player common
      init_player(p, 21); // clear netgame counters, etc
      init_player(p, 23); // clear bandwidth counters
   }

   if (t == 2) // new level
   {
      init_player(p, 17); // clear player common
      init_player(p, 21); // clear netgame counters, etc
      init_player(p, 23); // clear bandwidth counters
   }

   if (t == 17) // player common
   {
      syn[p].paused = 0;
      syn[p].paused_type = 0;
      syn[p].paused_mode = 0;
      syn[p].paused_mode_count = 0;


      syn[p].carry_item = 0;
      syn[p].player_ride = 0;
      syn[p].on_ladder = 0;
      syn[p].on_rope = 0;
      syn[p].left_right = 0;
      syn[p].left = 0;
      syn[p].right = 0;
      syn[p].up = 0;
      syn[p].down = 0;
      syn[p].jump = 0;
      syn[p].fire = 0;

      syn[p].xinc = 0;
      syn[p].yinc = 0;
      syn[p].left_xinc = 0;
      syn[p].right_xinc = 0;

      syn[p].shot_wait_counter = 0;
      syn[p].shot_wait = 4;
      syn[p].shot_speed = 12;

      syn[p].draw_rot = 0;
      syn[p].draw_scale = 1;
      syn[p].shape = 0;

      syn[p].stat_respawns = 0;
      syn[p].stat_shots_fired = 0;
      syn[p].stat_enemy_hits = 0;
      syn[p].stat_player_hits = 0;
      syn[p].stat_self_hits = 0;
      syn[p].stat_purple_coins = 0;

      loc[p].comp_move = 0;

      loc[p].health_display = 0;
      loc[p].last_health_adjust = 0;
      loc[p].potential_bomb_damage = 0;

      syn[p].level_done_mode = 0;
      syn[p].level_done_timer = 0;
      syn[p].level_done_ack = 0;
      syn[p].level_done_x = 0;
      syn[p].level_done_y = 0;
      syn[p].level_done_player = 0;

      syn[p].marked_gate = -1;

      syn[p].spawn_point_index = 0;

   }

   if (t == 21) // netgame counters, etc
   {
      loc[p].client_cdat_packets_tx = 0;

      syn[p].late_cdats = 0;
      syn[p].late_cdats_last_sec = 0;

      syn[p].player_text_overlay_timer = 0;
      syn[p].player_text_overlay_type = 0;


      mTally_late_cdats_last_sec[p].initialize(); // initialize tally

      loc[p].client_chase_fps = 0;
      loc[p].server_game_move_sync = 0;





      loc[p].quit_frame = 0;
      loc[p].quit_reason = 0;

      loc[p].join_frame = 0;
      loc[p].client_base_resets = 0;

      loc[p].num_dif_packets = 0;
      loc[p].server_last_stak_rx_frame_num = 0;
      loc[p].client_last_dif_applied_frame_num = 0;

      loc[p].cmp_dif_size = 0;
      loc[p].made_active_holdoff = 0;

      loc[p].ping = 0;
      loc[p].ping_avg = 0;

      loc[p].game_move_dsync = 0;
      loc[p].game_move_dsync_avg_last_sec = 0;
      mTally_game_move_dsync_avg_last_sec[p].initialize(); // initialize tally


      loc[p].old_x = 0;
      loc[p].old_y = 0;

      loc[p].client_loc_plr_cor = 0;
      loc[p].client_rmt_plr_cor = 0;
      loc[p].client_loc_plr_cor_max = 0;
      loc[p].client_rmt_plr_cor_max = 0;
      loc[p].client_loc_plr_cor_avg = 0;
      loc[p].client_rmt_plr_cor_avg = 0;

      mTally_client_loc_plr_cor_last_sec[p].initialize();
      mTally_client_rmt_plr_cor_last_sec[p].initialize();


   }

   if (t == 23) // clear bandwidth counters
   {
      // used to add up until frame end
      loc[p].tx_current_bytes_for_this_frame = 0;
      loc[p].tx_current_packets_for_this_frame = 0;
      loc[p].tx_max_bytes_per_frame = 0;
      loc[p].tx_max_packets_per_frame = 0;
      loc[p].rx_current_bytes_for_this_frame = 0;
      loc[p].rx_current_packets_for_this_frame = 0;
      loc[p].rx_max_bytes_per_frame = 0;
      loc[p].rx_max_packets_per_frame = 0;

      // used to add up until tally end
      loc[p].tx_packets_tally = 0;
      loc[p].tx_bytes_tally = 0;
      loc[p].tx_max_packets_per_tally = 0;
      loc[p].tx_max_bytes_per_tally = 0;
      loc[p].rx_packets_tally = 0;
      loc[p].rx_bytes_tally = 0;
      loc[p].rx_max_packets_per_tally = 0;
      loc[p].rx_max_bytes_per_tally = 0;

      // used for display
      loc[p].tx_packets_per_tally = 0;
      loc[p].tx_bytes_per_tally = 0;
      loc[p].rx_packets_per_tally = 0;
      loc[p].rx_bytes_per_tally = 0;

      // grand totals
      loc[p].tx_total_bytes = 0;
      loc[p].tx_total_packets = 0;
      loc[p].rx_total_bytes = 0;
      loc[p].rx_total_packets = 0;
   }
}

void mwPlayer::set_default_player_colors(void)
{
   mPlayer.syn[1].color = 10;
   mPlayer.syn[2].color = 11;
   mPlayer.syn[3].color = 12;
   mPlayer.syn[4].color = 13;
   mPlayer.syn[5].color = 14;
   mPlayer.syn[6].color = 15;
   mPlayer.syn[7].color = 9;
}

bool mwPlayer::if_players_ctrl_just_pressed(int p, int ctrl)
{
   if ((syn[p].ctrl_bitf & ctrl) && (!(syn[p].old_ctrl_bitf & ctrl))) return true;
   else return false;
}

void mwPlayer::clear_controls(int p)
{
   syn[p].left  = 0;
   syn[p].right = 0;
   syn[p].up    = 0;
   syn[p].down  = 0;
   syn[p].jump  = 0;
   syn[p].fire  = 0;
   syn[p].menu  = 0;
}

void mwPlayer::set_controls_from_comp_move(int p, int comp_move)
{
   clear_controls(p);
   if (comp_move & PM_COMPMOVE_LEFT)  syn[p].left  = 1;
   if (comp_move & PM_COMPMOVE_RIGHT) syn[p].right = 1;
   if (comp_move & PM_COMPMOVE_UP)    syn[p].up    = 1;
   if (comp_move & PM_COMPMOVE_DOWN)  syn[p].down  = 1;
   if (comp_move & PM_COMPMOVE_JUMP)  syn[p].jump  = 1;
   if (comp_move & PM_COMPMOVE_FIRE)  syn[p].fire  = 1;
   if (comp_move & PM_COMPMOVE_MENU)  syn[p].menu  = 1;
}

int mwPlayer::comp_move_from_players_current_controls(int p) // new method to replace old_comp_move
{
   int cm = 0;
   if (syn[p].left)    cm |= PM_COMPMOVE_LEFT;
   if (syn[p].right)   cm |= PM_COMPMOVE_RIGHT;
   if (syn[p].up)      cm |= PM_COMPMOVE_UP;
   if (syn[p].down)    cm |= PM_COMPMOVE_DOWN;
   if (syn[p].jump)    cm |= PM_COMPMOVE_JUMP;
   if (syn[p].fire)    cm |= PM_COMPMOVE_FIRE;
   if (syn[p].menu)    cm |= PM_COMPMOVE_MENU;
   return cm;
}

void mwPlayer::set_comp_move_from_player_key_check(int p) // doesn't set controls
{
   int cm = 0;
   if (mInput.key[loc[0].left_key][0])       cm |= PM_COMPMOVE_LEFT;
   if (mInput.key[loc[0].right_key][0])      cm |= PM_COMPMOVE_RIGHT;
   if (mInput.key[loc[0].up_key][0])         cm |= PM_COMPMOVE_UP;
   if (mInput.key[loc[0].down_key][0])       cm |= PM_COMPMOVE_DOWN;
   if (mInput.key[loc[0].jump_key][0])       cm |= PM_COMPMOVE_JUMP;
   if (mInput.key[loc[0].fire_key][0])       cm |= PM_COMPMOVE_FIRE;
   if (mInput.key[loc[0].menu_key][0])       cm |= PM_COMPMOVE_MENU;
   if (mInput.key[ALLEGRO_KEY_ESCAPE][0])    cm |= PM_COMPMOVE_MENU;
   loc[p].comp_move = cm;
}

void mwPlayer::set_controls_from_player_key_check(int p) // used only in menu
{
   if (mInput.key[loc[0].left_key][0])       syn[p].left  = 1;
   if (mInput.key[loc[0].right_key][0])      syn[p].right = 1;
   if (mInput.key[loc[0].up_key][0])         syn[p].up    = 1;
   if (mInput.key[loc[0].down_key][0])       syn[p].down  = 1;
   if (mInput.key[loc[0].jump_key][0])       syn[p].jump  = 1;
   if (mInput.key[loc[0].fire_key][0])       syn[p].fire  = 1;
   if (mInput.key[loc[0].menu_key][0])       syn[p].menu  = 1;
   if (mInput.key[ALLEGRO_KEY_ESCAPE][0])    syn[p].menu  = 1;
}

void mwPlayer::proc_player_input(void)
{
   if ((syn[0].level_done_mode == 0) || (syn[0].level_done_mode == 5)) // only allow player input in these modes
      for (int p=0; p<NUM_PLAYERS; p++)
         if (syn[p].active) // cycle all active players
         {
            int cm = syn[p].control_method;
            if ((cm == PM_PLAYER_CONTROL_METHOD_SINGLE_PLAYER) || (cm == PM_PLAYER_CONTROL_METHOD_SERVER_LOCAL) || (cm == PM_PLAYER_CONTROL_METHOD_CLIENT_LOCAL)) // these are the only local control methods
            {
               set_comp_move_from_player_key_check(p);

               // fakekey mode
               if ((loc[p].fake_keypress_mode) || (syn[0].server_force_fakekey))
               {
                  // even in fakekey mode allow ESC or menu
                  if ((!mInput.key[loc[p].menu_key][0]) && (!mInput.key[ALLEGRO_KEY_ESCAPE][0])) loc[p].comp_move = rand() % 64;
               }

               if (loc[p].comp_move != comp_move_from_players_current_controls(p))   // player's controls have changed
               {
                  mGameMoves.add_game_move(mLoop.frame_num, PM_GAMEMOVE_TYPE_PLAYER_MOVE, p, loc[p].comp_move); // add to game moves array

                  // in client mode, also send cdat packet, and apply move directly to controls
                  if (cm == PM_PLAYER_CONTROL_METHOD_CLIENT_LOCAL)
                  {
                     mNetgame.client_send_cdat_packet(p);
                     set_controls_from_comp_move(p, loc[p].comp_move);
                     if (syn[p].menu) mLoop.state[0] = PM_PROGRAM_STATE_CLIENT_PREEXIT1; // menu key pressed
                  }
               }
            }
         }
}
