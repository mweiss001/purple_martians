// mwItemTrigger.cpp

#include "pm.h"
#include "mwItems.h"
#include "mwFont.h"
#include "mwBitmap.h"
#include "mwPlayers.h"
#include "mwLift.h"
#include "mwColor.h"
#include "mwTriggerEvent.h"
#include "mwLoop.h"
#include "mwEnemy.h"
#include "mwLevel.h"
#include "mwMiscFnx.h"
#include "mwScreen.h"
#include "mwGameEvent.h"
#include "mwShots.h"

/*
item[][0] = 6 - orb
item[][1] = bitmap
item[][2] = flags
item[][3] = stat | fall | carry
item[][4] = x pos (2000)
item[][5] = y pos (2000)
item[][6]  = MODE
item[][7]  = timer val
item[][8]  = time count
item[][9]  =
item[][10] = STATE ON  pm_event
item[][11] = SATEE OFF pm_event
item[][12] = TGON pm_event
item[][13] = TGOF pm_event

#define PM_ITEM_ORB_STATE       0b0000000000000001
#define PM_ITEM_ORB_PREV_STATE  0b0000000000000010
#define PM_ITEM_ORB_TGON        0b0000000000000100
#define PM_ITEM_ORB_TGOF        0b0000000000001000

#define PM_ITEM_ORB_TRIG_TOUCH  0b0000000000010000
#define PM_ITEM_ORB_TRIG_UP     0b0000000000100000
#define PM_ITEM_ORB_TRIG_DOWN   0b0000000001000000
#define PM_ITEM_ORB_TRIG_CURR   0b0000000010000000
#define PM_ITEM_ORB_TRIG_PREV   0b0000000100000000

*/

int mwItems::proc_orb_shot_collision(int i)
{
   int s = 8; // collison box size
   float x = itemf[i][0];
   float y = itemf[i][1];
   for (int b=0; b<50; b++)
      if (mShot.p[b].active)
      {
         float bx = mShot.p[b].x;
         float by = mShot.p[b].y;
         if ((x > bx-s) && (x < bx+s) && (y > by-s) && (y < by+s)) return 1;
      }
   return 0;
}


void mwItems::proc_orb(int i)
{
   int MODE = mItem.item[i][6];

   // timer mode stuff
   if (MODE == 3) // timed ON
   {
      if (--mItem.item[i][8] <= 0)
      {
         mItem.item[i][8] = 0;
         mItem.item[i][2] &= ~PM_ITEM_ORB_STATE;  // OFF
      }
      else mItem.item[i][2] |= PM_ITEM_ORB_STATE; // ON
   }
   if (MODE == 4) // timed OFF
   {
      if (--mItem.item[i][8] <= 0)
      {
         mItem.item[i][8] = 0;
         mItem.item[i][2] |= PM_ITEM_ORB_STATE;    // ON
      }
      else mItem.item[i][2] &= ~PM_ITEM_ORB_STATE; // OFF
   }


   if ((mItem.item[i][2] & PM_ITEM_ORB_TRIG_SHOT) && (proc_orb_shot_collision(i))) mItem.item[i][2] |= PM_ITEM_ORB_TRIG_CURR; // set CURR flag



   // trigger stuff
   if (mItem.item[i][2] & PM_ITEM_ORB_TRIG_CURR)                     // currently triggered
   {
      if ((MODE == 3) || (MODE == 4)) mItem.item[i][8] = mItem.item[i][7]+1; // reset counter
      if (!(mItem.item[i][2] & PM_ITEM_ORB_TRIG_PREV))                 // not triggered last time
      {
         mItem.item[i][2] |= PM_ITEM_ORB_TRIG_PREV;                  // set PREV flag
         if (MODE == 0) mItem.item[i][2] ^= PM_ITEM_ORB_STATE;       // toggle state
         if (MODE == 1) mItem.item[i][2] |= PM_ITEM_ORB_STATE;       // stick ON
         if (MODE == 2) mItem.item[i][2] &= ~PM_ITEM_ORB_STATE;      // stick OFF
      }
   }
   else mItem.item[i][2] &= ~PM_ITEM_ORB_TRIG_PREV;                  // clear PREV flag
   mItem.item[i][2] &= ~PM_ITEM_ORB_TRIG_CURR;                       // clear CURR flag



   // STATE stuff
   mItem.item[i][2] &= ~PM_ITEM_ORB_TGON;               // clear TGON flag
   mItem.item[i][2] &= ~PM_ITEM_ORB_TGOF;               // clear TGOF flag
   if (mItem.item[i][2] & PM_ITEM_ORB_STATE)            // orb state ON
   {
      mItem.item[i][1] = 419;                           // red orb
      if (!(mItem.item[i][2] & PM_ITEM_ORB_PREV_STATE)) // prev OFF
      {
         mItem.item[i][2] |= PM_ITEM_ORB_TGON;          // set TGON flag
         mItem.item[i][2] |= PM_ITEM_ORB_PREV_STATE;    // set PREV flag
      }
   }
   else                                           // orb state OFF
   {
      mItem.item[i][1] = 418;                           // green orb
      if ((mItem.item[i][2] & PM_ITEM_ORB_PREV_STATE))  // prev ON
      {
         mItem.item[i][2] |=  PM_ITEM_ORB_TGOF;         // set TGOF flag
         mItem.item[i][2] &= ~PM_ITEM_ORB_PREV_STATE;   // clear PREV flag
      }
   }

   // clear all events
   mTriggerEvent.event[mItem.item[i][10]] = 0;
   mTriggerEvent.event[mItem.item[i][11]] = 0;
   mTriggerEvent.event[mItem.item[i][12]] = 0;
   mTriggerEvent.event[mItem.item[i][13]] = 0;
   int FLAGS = mItem.item[i][2];
   if   (FLAGS & PM_ITEM_ORB_STATE)  mTriggerEvent.event[mItem.item[i][10]] = 1;
   if (!(FLAGS & PM_ITEM_ORB_STATE)) mTriggerEvent.event[mItem.item[i][11]] = 1;
   if   (FLAGS & PM_ITEM_ORB_TGON)   mTriggerEvent.event[mItem.item[i][12]] = 1;
   if   (FLAGS & PM_ITEM_ORB_TGOF)   mTriggerEvent.event[mItem.item[i][13]] = 1;
}

int mwItems::draw_orb(int i, int x, int y)
{
   mItem.item[i][1] = 418;                                          // green orb
   if (mItem.item[i][2] & PM_ITEM_ORB_STATE) mItem.item[i][1] = 419;      // red orb

   // rotation
   int rb = (mItem.item[i][2] & PM_ITEM_ORB_ROTB) >> 14;
   float a=0, xo=0, yo=0; // angle, x and y offsets
   if (rb == 0) { a = 0;             xo = 10, yo = 9; } // floor
   if (rb == 1) { a = ALLEGRO_PI/2;  xo = 7,  yo = 7; } // wall left
   if (rb == 2) { a = ALLEGRO_PI;    xo = 9,  yo = 4; } // ceiling
   if (rb == 3) { a = -ALLEGRO_PI/2; xo = 12, yo = 6; } // wall right

   int MODE = mItem.item[i][6];
   int drawn = 0;
   if ((MODE == 3) || (MODE == 4))
   {
      int c1=11, c2=10;
      if (MODE == 4) {c1=10; c2=11;}
      int percent =  ((mItem.item[i][8]-1) * 100) / mItem.item[i][7];
      if (percent > 0)
      {
         mScreen.draw_percent_barc(x+xo, y+yo, 7, 7,  percent, c1, c2, -1);
         al_draw_rotated_bitmap(mBitmap.tile[417], 10, 10, x+10, y+10, a, 0);
         drawn = 1;
      }
   }
   if (!drawn) al_draw_rotated_bitmap(mBitmap.tile[mItem.item[i][1]], 10, 10, x+10, y+10, a, 0);
   return 1;
}
void mwItems::proc_orb_collision(int p, int i)
{
   if (  (mItem.item[i][2] & PM_ITEM_ORB_TRIG_TOUCH) ||
        ((mItem.item[i][2] & PM_ITEM_ORB_TRIG_UP)   && (mPlayer.syn[p].up)) ||
        ((mItem.item[i][2] & PM_ITEM_ORB_TRIG_DOWN) && (mPlayer.syn[p].down)) )
           mItem.item[i][2] |= PM_ITEM_ORB_TRIG_CURR;
}


/*
item[][0] = 9  - Trigger
item[][1] =
item[][2] = draw_type (color)
item[][3] = flags
item[][4] = x pos (2000)
item[][5] = y pos (2000)

item[][6]  = trigger field x (2000)
item[][7]  = trigger field y (2000)
item[][8]  = trigger field w (2000)
item[][9]  = trigger field x (2000)

item[][10] = trigger field lift number

item[][11] = CURR ON  pm_event
item[][12] = CURR OFF pm_event
item[][13] = TGON pm_event #
item[][14] = TGOF pm_event #


#define PM_ITEM_TRIGGER_PLAYER   0b0000000000000001
#define PM_ITEM_TRIGGER_ENEMY    0b0000000000000010
#define PM_ITEM_TRIGGER_ITEM     0b0000000000000100
#define PM_ITEM_TRIGGER_PSHOT     0b0000000000001000
#define PM_ITEM_TRIGGER_ESHOT     0b0000000000010000
#define PM_ITEM_TRIGGER_CURR     0b0000000000100000
#define PM_ITEM_TRIGGER_PREV     0b0000000001000000
#define PM_ITEM_TRIGGER_TGON     0b0000000010000000
#define PM_ITEM_TRIGGER_TGOF     0b0000000100000000
#define PM_ITEM_TRIGGER_LIFT_ON  0b0000001000000000
#define PM_ITEM_TRIGGER_LIFT_XC  0b0000010000000000
#define PM_ITEM_TRIGGER_LIFT_XF  0b0000100000000000
#define PM_ITEM_TRIGGER_LIFT_XL  0b0001000000000000
#define PM_ITEM_TRIGGER_LIFT_YC  0b0010000000000000
#define PM_ITEM_TRIGGER_LIFT_YF  0b0100000000000000
#define PM_ITEM_TRIGGER_LIFT_YL  0b1000000000000000

*/

void mwItems::proc_trigger(int i)
{
   int FLAGS = mItem.item[i][3];
   if (FLAGS & PM_ITEM_TRIGGER_LIFT_ON) set_item_trigger_location_from_lift(i, 0);

   mItem.item[i][3] &= ~PM_ITEM_TRIGGER_TGON;  // clear Toggle ON  trigger flag
   mItem.item[i][3] &= ~PM_ITEM_TRIGGER_TGOF;  // clear Toggle OFF trigger flag
   mItem.item[i][3] &= ~PM_ITEM_TRIGGER_CURR;  // clear current    trigger flag

   detect_trigger_collisions(i);

   if ( (mItem.item[i][3] &  PM_ITEM_TRIGGER_CURR)    // is current trigger flag set?
   && (!(mItem.item[i][3] &  PM_ITEM_TRIGGER_PREV)))  // and previous trigger flag not set?
         mItem.item[i][3] |= PM_ITEM_TRIGGER_TGON;    // set trigger ON toggle


   if (!(mItem.item[i][3] &  PM_ITEM_TRIGGER_CURR)    // is current trigger flag not set?
   && ( (mItem.item[i][3] &  PM_ITEM_TRIGGER_PREV)))  // and previous trigger flag set?
         mItem.item[i][3] |= PM_ITEM_TRIGGER_TGOF;    // set trigger OFF toggle


   if   (mItem.item[i][3] &   PM_ITEM_TRIGGER_CURR)    // is current trigger flag set?
         mItem.item[i][3] |=  PM_ITEM_TRIGGER_PREV;    // set previous trigger flag

   if (!(mItem.item[i][3] &   PM_ITEM_TRIGGER_CURR))   // is current trigger flag not set?
         mItem.item[i][3] &= ~PM_ITEM_TRIGGER_PREV;    // clear previous trigger flag

   FLAGS = mItem.item[i][3]; // update FLAGS

/*   if (FLAGS & PM_ITEM_TRIGGER_CURR) printf("%d - CURR\n", mLoop.frame_num);
   if (FLAGS & PM_ITEM_TRIGGER_PREV) printf("%d - PREV\n", mLoop.frame_num);
   if (FLAGS & PM_ITEM_TRIGGER_TGON) printf("%d - TGON\n", mLoop.frame_num);
   if (FLAGS & PM_ITEM_TRIGGER_TGOF) printf("%d - TGOF\n", mLoop.frame_num); */

   // clear them all
   mTriggerEvent.event[mItem.item[i][11]] = 0;
   mTriggerEvent.event[mItem.item[i][12]] = 0;
   mTriggerEvent.event[mItem.item[i][13]] = 0;
   mTriggerEvent.event[mItem.item[i][14]] = 0;

   if   (FLAGS & PM_ITEM_TRIGGER_CURR)  mTriggerEvent.event[mItem.item[i][11]] = 1;
   if (!(FLAGS & PM_ITEM_TRIGGER_CURR)) mTriggerEvent.event[mItem.item[i][12]] = 1;
   if   (FLAGS & PM_ITEM_TRIGGER_TGON)  mTriggerEvent.event[mItem.item[i][13]] = 1;
   if   (FLAGS & PM_ITEM_TRIGGER_TGOF)  mTriggerEvent.event[mItem.item[i][14]] = 1;
}

void mwItems::set_item_trigger_location_from_lift(int i, int a20)
{
   int d = mItem.item[i][10]; // lift number
   if (mLift.cur[d].active) // only proceed if lift number is valid
   {
      // x axis
      int lx1 = mLift.cur[d].x;
      int lx2 = mLift.cur[d].x + mLift.cur[d].w;
      int C = mItem.item[i][3] & PM_ITEM_TRIGGER_LIFT_XC;
      int F = mItem.item[i][3] & PM_ITEM_TRIGGER_LIFT_XF;
      int L = mItem.item[i][3] & PM_ITEM_TRIGGER_LIFT_XL;
      if (C)
      {
         int lxc = lx1 + (lx2-lx1)/2; // get center of lift
         mItem.item[i][6] = lxc - mItem.item[i][8]/2;
      }
      else
      {
         if ((!F) && (!L)) mItem.item[i][6] = lx1;             // fx1 = lx1
         if ((!F) && ( L)) mItem.item[i][6] = lx2;             // fx1 = lx2
         if (( F) && (!L)) mItem.item[i][6] = lx1 - mItem.item[i][8]; // fx2 = lx1
         if (( F) && ( L)) mItem.item[i][6] = lx2 - mItem.item[i][8]; // fx2 = lx2
      }
      // y axis
      int ly1 = mLift.cur[d].y;
      int ly2 = mLift.cur[d].y + mLift.cur[d].h;
      C = mItem.item[i][3] & PM_ITEM_TRIGGER_LIFT_YC;
      F = mItem.item[i][3] & PM_ITEM_TRIGGER_LIFT_YF;
      L = mItem.item[i][3] & PM_ITEM_TRIGGER_LIFT_YL;

      if (C)
      {
         int lyc = ly1 + (ly2-ly1)/2; // get center of lift
         mItem.item[i][7] = lyc - mItem.item[i][9]/2;
      }
      else
      {
         if ((!F) && (!L)) mItem.item[i][7] = ly1;             // fy1 = ly1
         if ((!F) && ( L)) mItem.item[i][7] = ly2;             // fy1 = ly2
         if (( F) && (!L)) mItem.item[i][7] = ly1 - mItem.item[i][9]; // fy2 = ly1
         if (( F) && ( L)) mItem.item[i][7] = ly2 - mItem.item[i][9]; // fy2 = ly2
      }
      if (a20) // align to 20 grid
      {
         mItem.item[i][6] = mMiscFnx.round20(mItem.item[i][6]);
         mItem.item[i][7] = mMiscFnx.round20(mItem.item[i][7]);
      }
   }
}


void mwItems::detect_trigger_collisions(int i)
{
   int FLAGS = mItem.item[i][3];

   // trigger field
   int tfx1 = mItem.item[i][6]-10;
   int tfy1 = mItem.item[i][7]-10;
   int tfx2 = tfx1 + mItem.item[i][8];
   int tfy2 = tfy1 + mItem.item[i][9];

   if (FLAGS & PM_ITEM_TRIGGER_PLAYER)
      for (int p=0; p<NUM_PLAYERS; p++)
         if ((mPlayer.syn[p].active) && (!mPlayer.syn[p].paused))
         {
            int x = mPlayer.syn[p].x;
            int y = mPlayer.syn[p].y;
            if ((x > tfx1) && (x < tfx2) && (y > tfy1) && (y < tfy2)) mItem.item[i][3] |= PM_ITEM_TRIGGER_CURR;
         }
   if (FLAGS & PM_ITEM_TRIGGER_ENEMY)
      for (int e2=0; e2<100; e2++)
         if (mEnemy.Ei[e2][0])
         {
            int x = mEnemy.Ef[e2][0];
            int y = mEnemy.Ef[e2][1];
            if ((x > tfx1) && (x < tfx2) && (y > tfy1) && (y < tfy2)) mItem.item[i][3] |= PM_ITEM_TRIGGER_CURR;
         }
   if (FLAGS & PM_ITEM_TRIGGER_ITEM)
      for (int c=0; c<500; c++)
         if (mItem.item[c][0])
         {
            int x = itemf[c][0];
            int y = itemf[c][1];
            if ((x > tfx1) && (x < tfx2) && (y > tfy1) && (y < tfy2)) mItem.item[i][3] |= PM_ITEM_TRIGGER_CURR;
         }
   if (FLAGS & PM_ITEM_TRIGGER_PSHOT) // check player shots
      for (int b=0; b<50; b++)
         if (mShot.p[b].active)
         {
            int x = mShot.p[b].x;
            int y = mShot.p[b].y;
            if ((x > tfx1) && (x < tfx2) && (y > tfy1) && (y < tfy2)) mItem.item[i][3] |= PM_ITEM_TRIGGER_CURR;
         }
   if (FLAGS & PM_ITEM_TRIGGER_ESHOT) // check enemy shots
      for (int b=0; b<50; b++)
         if (mShot.e[b].active)
         {
            int x = mShot.e[b].x;
            int y = mShot.e[b].y;
            if ((x > tfx1) && (x < tfx2) && (y > tfy1) && (y < tfy2)) mItem.item[i][3] |= PM_ITEM_TRIGGER_CURR;
         }
}





int mwItems::draw_trigger(int i, int x, int y)
{
   if (mLoop.level_editor_running)
   {
      al_draw_bitmap(mBitmap.tile[991], x, y, 0); // draw item shape in level editor, invisible when game running
      if (mItem.item[i][3] & PM_ITEM_TRIGGER_LIFT_ON) set_item_trigger_location_from_lift(i, 1); // snap to lift here because main function wont be called while in level editor
   }

   if (mItem.item[i][3] & PM_ITEM_TRIGGER_DRAW_ON)
   {
      int col = mItem.item[i][2];
      float x1 = mItem.item[i][6];
      float y1 = mItem.item[i][7];
      float x2 = x1 + mItem.item[i][8];
      float y2 = y1 + mItem.item[i][9];
      mMiscFnx.rectangle_with_diagonal_lines(x1, y1, x2, y2, 10, col, col+96, 0);
   }
   return 1;
}


/*
item[][0] = 16 - Block Manip
item[][1] = pm_event_trigger
item[][2] = draw on
item[][3] = mode
item[][4] = x pos (2000)
item[][5] = y pos (2000)
item[][6]  = trigger field x (2000)
item[][7]  = trigger field y (2000)
item[][8]  = trigger field w (2000)
item[][9]  = trigger field x (2000)
item[][10] block 1
item[][11] block 2
item[][12] = draw color

*/
void mwItems::proc_block_manip(int i)
{
   int et = mItem.item[i][1]; // pm_event trigger we are looking for
   if (mTriggerEvent.event[et])
   {
      al_set_target_bitmap(mBitmap.level_background);
      int x1 = mItem.item[i][6]/20;
      int y1 = mItem.item[i][7]/20;
      int x2 = x1 + mItem.item[i][8]/20;
      int y2 = y1 + mItem.item[i][9]/20;

      for (int x=x1; x<x2; x++)
         for (int y=y1; y<y2; y++)
         {
            int mode = mItem.item[i][3];
            int block1 = mItem.item[i][10];
            int block2 = mItem.item[i][11];

            if (mode == 1) // set all blocks to block 1
            {
               mLevel.l[x][y] = block1;
               al_draw_filled_rectangle(x*20, y*20, x*20+20, y*20+20, mColor.pc[0]);
               al_draw_bitmap(mBitmap.btile[block1&1023], x*20, y*20, 0 );
            }

            if (mode == 2) // set all block2 to block 1
            {
               if (mLevel.l[x][y] == block2)
               {
                  mLevel.l[x][y] = block1;
                  al_draw_filled_rectangle(x*20, y*20, x*20+20, y*20+20, mColor.pc[0]);
                  al_draw_bitmap(mBitmap.btile[block1&1023], x*20, y*20, 0 );
               }
            }

            if (mode == 3) // toggle block1 and block 2
            {
               if (mLevel.l[x][y] == block1)
               {
                  mLevel.l[x][y] = block2;
                  al_draw_filled_rectangle(x*20, y*20, x*20+20, y*20+20, mColor.pc[0]);
                  al_draw_bitmap(mBitmap.btile[block2&1023], x*20, y*20, 0 );
               }
               else if (mLevel.l[x][y] == block2)
               {
                  mLevel.l[x][y] = block1;
                  al_draw_filled_rectangle(x*20, y*20, x*20+20, y*20+20, mColor.pc[0]);
                  al_draw_bitmap(mBitmap.btile[block1&1023], x*20, y*20, 0 );
               }
            }
         }
   }
}

int mwItems::draw_block_manip(int i, int x, int y)
{
   if (mLoop.level_editor_running)
   {
      al_draw_bitmap(mBitmap.tile[989], x, y, 0); // draw item shape in level editor, invisible when game running
   }
   if (mItem.item[i][2]) // draw mode on
   {
      int col = mItem.item[i][12];
      float x1 = mItem.item[i][6];
      float y1 = mItem.item[i][7];
      float x2 = x1 + mItem.item[i][8];
      float y2 = y1 + mItem.item[i][9];
      mMiscFnx.rectangle_with_diagonal_lines(x1, y1, x2, y2, 10, col, col+96, 0);
   }
   return 1;
}

/*

item[][0]  = 17 - Block Damage
item[][1]  = event trigger number
item[][2]  = damage draw type
item[][3]  = flags
#define PM_ITEM_DAMAGE_PLAYER   0b00000000000000001
#define PM_ITEM_DAMAGE_ENEMY    0b00000000000000010
#define PM_ITEM_DAMAGE_ITEM     0b00000000000000100
#define PM_ITEM_DAMAGE_PSHOT     0b00000000000001000
#define PM_ITEM_DAMAGE_ESHOT     0b00000000000010000
#define PM_ITEM_DAMAGE_CURR     0b00000000000100000
#define PM_ITEM_DAMAGE_LIFT_ON  0b00000000001000000
#define PM_ITEM_DAMAGE_LIFT_XC  0b00000000010000000
#define PM_ITEM_DAMAGE_LIFT_XF  0b00000000100000000
#define PM_ITEM_DAMAGE_LIFT_XL  0b00000001000000000
#define PM_ITEM_DAMAGE_LIFT_YC  0b00000010000000000
#define PM_ITEM_DAMAGE_LIFT_YF  0b00000100000000000
#define PM_ITEM_DAMAGE_LIFT_YL  0b00001000000000000

item[][4]  = x pos   (2000)
item[][5]  = y pos   (2000)
item[][6]  = field x (2000)
item[][7]  = field y (2000)
item[][8]  = field w (2000)
item[][9]  = field h (2000)
item[][10] = lift number
item[][11] = mode
item[][12] = t1 val
item[][13] = count
item[][14] = t2 val
item[][15] = damage

*/

void mwItems::set_item_damage_location_from_lift(int i, int a20)
{
   int d = mItem.item[i][10]; // lift number
   if (mLift.cur[d].active) // only proceed if lift number is valid
   {
      // x axis
      int lx1 = mLift.cur[d].x;
      int lx2 = mLift.cur[d].x + mLift.cur[d].w;
      int C = mItem.item[i][3] & PM_ITEM_DAMAGE_LIFT_XC;
      int F = mItem.item[i][3] & PM_ITEM_DAMAGE_LIFT_XF;
      int L = mItem.item[i][3] & PM_ITEM_DAMAGE_LIFT_XL;
      if (C)
      {
         int lxc = lx1 + (lx2-lx1)/2; // get center of lift
         mItem.item[i][6] = lxc - mItem.item[i][8]/2;
      }
      else
      {
         if ((!F) && (!L)) mItem.item[i][6] = lx1;              // fx1 = lx1
         if ((!F) && ( L)) mItem.item[i][6] = lx2;              // fx1 = lx2
         if (( F) && (!L)) mItem.item[i][6] = lx1 - mItem.item[i][8]; // fx2 = lx1
         if (( F) && ( L)) mItem.item[i][6] = lx2 - mItem.item[i][8]; // fx2 = lx2
      }
      // y axis
      int ly1 = mLift.cur[d].y;
      int ly2 = mLift.cur[d].y + mLift.cur[d].h;
      C = mItem.item[i][3] & PM_ITEM_DAMAGE_LIFT_YC;
      F = mItem.item[i][3] & PM_ITEM_DAMAGE_LIFT_YF;
      L = mItem.item[i][3] & PM_ITEM_DAMAGE_LIFT_YL;

      if (C)
      {
         int lyc = ly1 + (ly2-ly1)/2; // get center of lift
         mItem.item[i][7] = lyc - mItem.item[i][9]/2;
      }
      else
      {
         if ((!F) && (!L)) mItem.item[i][7] = ly1;              // fy1 = ly1
         if ((!F) && ( L)) mItem.item[i][7] = ly2;              // fy1 = ly2
         if (( F) && (!L)) mItem.item[i][7] = ly1 - mItem.item[i][9]; // fy2 = ly1
         if (( F) && ( L)) mItem.item[i][7] = ly2 - mItem.item[i][9]; // fy2 = ly2
      }
      if (a20) // align to 20 grid
      {
         mItem.item[i][6] = mMiscFnx.round20(mItem.item[i][6]);
         mItem.item[i][7] = mMiscFnx.round20(mItem.item[i][7]);
      }
   }
}

void mwItems::proc_item_damage_collisions(int i)
{
   int FLAGS = mItem.item[i][3];
   int cd = FLAGS & PM_ITEM_DAMAGE_CURR;                 // damage active
   int cdp =  ((cd) && (FLAGS & PM_ITEM_DAMAGE_PLAYER)); // damage active and player flag
   int cde =  ((cd) && (FLAGS & PM_ITEM_DAMAGE_ENEMY));  // damage active and enemy flag
   int cdi =  ((cd) && (FLAGS & PM_ITEM_DAMAGE_ITEM));   // damage active and item flag
   int cdpb = ((cd) && (FLAGS & PM_ITEM_DAMAGE_PSHOT));   // damage active and player shot flag
   int cdeb = ((cd) && (FLAGS & PM_ITEM_DAMAGE_ESHOT));   // damage active and enemy shot flag

   // damage field
   int tfx1 = mItem.item[i][6]-10;
   int tfy1 = mItem.item[i][7]-10;
   int tfx2 = tfx1 + mItem.item[i][8];
   int tfy2 = tfy1 + mItem.item[i][9];

   if (cdp)
      for (int p=0; p<NUM_PLAYERS; p++)
         if ((mPlayer.syn[p].active) && (!mPlayer.syn[p].paused))
         {
            int x = mPlayer.syn[p].x;
            int y = mPlayer.syn[p].y;
            if ((x > tfx1) && (x < tfx2) && (y > tfy1) && (y < tfy2))
            {
               if (FLAGS & PM_ITEM_DAMAGE_INSTGIB)
               {
                  mPlayer.syn[p].health = 0;
                  //mGameEvent.add(59, 0, 0, p, i, 0, 0);
               }
               else
               {
                  if (mItem.item[i][15] > 0) // lose health
                  {
                     mPlayer.syn[p].health -= mItem.item[i][15]/100;
                     mGameEvent.add(59, 0, 0, p, i, 0, 0); // only do damage noise when taking health..??
                  }
                  else // gain health
                  {
                     if (mPlayer.syn[p].health < 100)
                     {
                        mPlayer.syn[p].health -= mItem.item[i][15]/100;
                        if (mPlayer.syn[p].health > 100) mPlayer.syn[p].health = 100;
                     }
                  }
               }
            }
         }
   if (cde)
      for (int e2=0; e2<100; e2++)
         if (mEnemy.Ei[e2][0])
         {
            int x = mEnemy.Ef[e2][0];
            int y = mEnemy.Ef[e2][1];
            if ((x > tfx1) && (x < tfx2) && (y > tfy1) && (y < tfy2))
            {
               mEnemy.Ei[e2][31] = 3;           // flag that this enemy got shot
               //mEnemy.Ei[e2][26] = x;           // number of player's shot that hit enemy
            }
         }
   if (cdi)
      for (int i=0; i<500; i++)
         if (mItem.item[i][0])
         {
            int x = itemf[i][0];
            int y = itemf[i][0];
            if ((x > tfx1) && (x < tfx2) && (y > tfy1) && (y < tfy2))
            {
               // orb, trig, bm, bd -- kill these immed
               if ((mItem.item[i][0] == 6) || (mItem.item[i][0] == 9) || (mItem.item[i][0] == 16) || (mItem.item[i][0] == 17)) mItem.item[i][0] = 0;

               if ((mItem.item[i][0] != 66) && (mItem.item[i][0] != 5)) // never kill start

               {
                  //mItem.item[i][0] = 66;
                  mItem.item[i][14] = 10;
               }
            }
         }
   if (cdpb) // check player shots
      for (int b=0; b<50; b++)
         if (mShot.p[b].active)
         {
            int x = mShot.p[b].x;
            int y = mShot.p[b].y;
            if ((x > tfx1) && (x < tfx2) && (y > tfy1) && (y < tfy2)) mShot.p[b].active = 0; // kill the shot
         }
   if (cdeb) // check enemy shots
      for (int b=0; b<50; b++)
         if (mShot.e[b].active)
         {
            int x = mShot.e[b].x;
            int y = mShot.e[b].y;
            if ((x > tfx1) && (x < tfx2) && (y > tfy1) && (y < tfy2)) mShot.e[b].active = 0; // kill the shot
         }
}

int mwItems::draw_block_damage(int i, int x, int y, int custom)
{
   int draw_mode = mItem.item[i][2];
   int mode = mItem.item[i][11];
   int FLAGS = mItem.item[i][3];
   float x0 = x;
   float y0 = y;
   float x1 = mItem.item[i][6];
   float y1 = mItem.item[i][7];
   float x2 = x1 + mItem.item[i][8];
   float y2 = y1 + mItem.item[i][9];

   if (mLoop.level_editor_running)
   {
      al_draw_bitmap(mBitmap.tile[988], x0, y0, 0); // draw item shape in level editor, invisible when game running
      if (FLAGS & PM_ITEM_DAMAGE_LIFT_ON) set_item_damage_location_from_lift(i, 1); // set this here only when level editor is running
   }

   // damage field drawing
   if (!custom)
   {

      if (draw_mode == 1) // basic
      {
         int col = 11;
         if (FLAGS & PM_ITEM_DAMAGE_CURR) col = 10;
         mMiscFnx.rectangle_with_diagonal_lines(x1, y1, x2, y2, 10, col, col+96, 0);
      }

      if (draw_mode == 2) // spikey floor
      {
         int tn = 808;
         if (FLAGS & PM_ITEM_DAMAGE_CURR) tn = 807;
         for (int hx = x1; hx<x2; hx+=20)
            al_draw_bitmap(mBitmap.tile[tn], hx, y2-20, 0); // draw spikes only on bottom row
      }
   }

   // timer drawing
   int timer_draw_mode1 = mItem.item[i][3] & PM_ITEM_DAMAGE_TIMR_SN;
   int timer_draw_mode2 = mItem.item[i][3] & PM_ITEM_DAMAGE_TIMR_BN;
   int timer_draw_mode3 = mItem.item[i][3] & PM_ITEM_DAMAGE_TIMR_SP;
   int timer_draw_mode4 = mItem.item[i][3] & PM_ITEM_DAMAGE_TIMR_BP;


   int col = 15;

   if ((mode == 2) || (mode == 3))
   {
      if (((timer_draw_mode1) || (timer_draw_mode2)) && (mItem.item[i][13] > 0)) // small or big number and timer is running
      {
         // time to show
         int tts = (mItem.item[i][13]); // raw (40ths of a second)
         //int tts = (mItem.item[i][13] / 40); // seconds
         //int tts = (mItem.item[i][13] / 4); // tenths of a second
         //int tts = (mItem.item[i][13] / 8); // fifths of a second

         if (mode == 2) col = 11;
         if (mode == 3) col = 10;
         if (timer_draw_mode1) al_draw_textf(mFont.pixl,   mColor.pc[col], x0+10, y0+4, ALLEGRO_ALIGN_CENTER, "%d", tts);
         if (timer_draw_mode2) al_draw_textf(mFont.pr8, mColor.pc[col], x0+10, y0+6, ALLEGRO_ALIGN_CENTER, "%d", tts);

      }
      if ((timer_draw_mode3) || (timer_draw_mode4)) // percent bar
      {
         int percent = 0;
         if (mItem.item[i][12] > 0) // prevent divide by zero
         {
            if (mode == 2) percent =       ((mItem.item[i][13]) * 100) / mItem.item[i][12];
            if (mode == 3) percent = 100 - ((mItem.item[i][13]) * 100) / mItem.item[i][12];
         }
         if (timer_draw_mode3) mScreen.draw_percent_bar(x0+9, y0+5, 32, 8,  percent);
         if (timer_draw_mode4) mScreen.draw_percent_bar(x0+9, y0+1, 64, 16, percent);

      }
   }
   if (mode == 4)
   {
      int tt = mItem.item[i][12]; // total time
      int ct = mItem.item[i][13]; // current time
      int st = mItem.item[i][14]; // switch time

      int percent = 0;
      int tts = 0;
      //int col = 0;

      if (ct >= st) // upper range, damage off
      {
         int ut = tt-st; // total time in the upper
         int dt = ct-st; // current time relative to that
         if (ut > 0)     // prevent divide by zero
         {
            //percent = 100 - (dt * 100) / ut;
            percent = (dt * 100) / ut;
            tts = (dt / 4) + 1;
            col = 11;
         }
      }
      else // lower range, damage on
      {
         int lt = st; // total time in lower
         int dt = ct; // current time relative to that
         if (lt > 0)  // prevent divide by zero
         {
            tts = (dt / 4) + 1;
            //percent = (dt * 100) / lt;
            percent = 100 - (dt * 100) / lt;
            col = 10;
         }
      }
      if (timer_draw_mode1) al_draw_textf(mFont.pixl,   mColor.pc[col], x0+10, y0+4, ALLEGRO_ALIGN_CENTER, "%d", tts);
      if (timer_draw_mode2) al_draw_textf(mFont.pr8, mColor.pc[col], x0+10, y0+6, ALLEGRO_ALIGN_CENTER, "%d", tts);
      if (timer_draw_mode3) mScreen.draw_percent_bar(x0+9, y0+5, 32, 8,  percent);
      if (timer_draw_mode4) mScreen.draw_percent_bar(x0+9, y0+1, 64, 16, percent);
   }
   return 1;
}

void mwItems::proc_block_damage(int i)
{
   int et = mItem.item[i][1];      // number of pm_event trigger we are looking for
   int FLAGS = mItem.item[i][3];
   int mode = mItem.item[i][11];


   if (FLAGS & PM_ITEM_DAMAGE_LIFT_ON) set_item_damage_location_from_lift(i, 0); // follow lift location

   proc_item_damage_collisions(i);

   if (mode == 0) mItem.item[i][3] |= PM_ITEM_DAMAGE_CURR; // in mode 0, always set damage flag

   if (mode == 1)
   {
      if (mTriggerEvent.event[et]) mItem.item[i][3] ^= PM_ITEM_DAMAGE_CURR; // toggle current damage flag
   }
   if (mode == 2) // damage unless timer running  (no damage when triggered)
   {
      if (mTriggerEvent.event[et]) mItem.item[i][13] = mItem.item[i][12];              // reset timer
      if (mItem.item[i][13] == 0) mItem.item[i][3] |=  PM_ITEM_DAMAGE_CURR; // set damage on
      else                  mItem.item[i][3] &= ~PM_ITEM_DAMAGE_CURR; // set damage off
   }
   if (mode == 3) // damage when timer is running (no damage until triggered)
   {
      if (mTriggerEvent.event[et]) mItem.item[i][13] = mItem.item[i][12];             // reset timer
      if (mItem.item[i][13] > 0) mItem.item[i][3] |=  PM_ITEM_DAMAGE_CURR; // set damage on
      else                 mItem.item[i][3] &= ~PM_ITEM_DAMAGE_CURR; // set damage off
   }
   if (mode == 4) // timed on and off
   {
      // timer will run outside this function always, but in this mode, when it gets to zero, we will reset it
      if (mItem.item[i][13] == 0) mItem.item[i][13] = mItem.item[i][12];  // reset timer
      if (mItem.item[i][13] < mItem.item[i][14]) mItem.item[i][3] |=  PM_ITEM_DAMAGE_CURR; // set damage on
      else                           mItem.item[i][3] &= ~PM_ITEM_DAMAGE_CURR; // set damage off
   }
   if (--mItem.item[i][13] < 0) mItem.item[i][13] = 0; // always run timer
}






void mwItems::set_trigger_event(int i, int ev0, int ev1, int ev2, int ev3)
{
   if (mItem.item[i][0] == 6) // orb
   {
      mItem.item[i][10] = ev0;
      mItem.item[i][11] = ev1;
      mItem.item[i][12] = ev2;
      mItem.item[i][13] = ev3;
   }
   if (mItem.item[i][0] == 9) // trigger
   {
      mItem.item[i][11] = ev0;
      mItem.item[i][12] = ev1;
      mItem.item[i][13] = ev2;
      mItem.item[i][14] = ev3;
   }
}






/*
item[][0]  = 13 - Time Trigger
item[][1]  = count
item[][2]  = state
item[][3]  = flags

item[][4]  = x pos   (2000)
item[][5]  = y pos   (2000)

item[][6]  = t1 reset val
item[][7]  = t1 mode (0,1,2,3)
item[][8]  = t1 i/p event
item[][9]  = t1 o/p event

item[][10]  = t2 reset val
item[][11]  = t2 mode (0,1,2,3)
item[][12]  = t2 i/p event
item[][13]  = t2 o/p event

item[][14] = x2  for display
item[][15] = y2

                                      00000000
                                              00000000
#define PM_ITEM_TIMER_T1_MODE       0b1100000000000000
#define PM_ITEM_TIMER_T2_MODE       0b0011000000000000
#define PM_ITEM_TIMER_STATE_CURR    0b0000000000000000
#define PM_ITEM_TIMER_STATE_PREV    0b0000000000000000


item[][10] = t1 reset val
item[][11] = t2 reset val
item[][12] = t1 i/p event
item[][13] = t1 o/p event
item[][14] = t2 i/p event
item[][15] = t2 o/p event



*/


int mwItems::draw_timer(int i, int x, int y)
{
   int state = 0;
   int t1_mode = 0;
   int t2_mode = 0;
   int time = 0;
   get_timer_flags(item[i][3], state, t1_mode, t2_mode, time);



   float x1 = item[i][6];
   float y1 = item[i][7];
   float w =  item[i][8];
   float h =  item[i][9];
   float x2 = x1 + w;
   float y2 = y1 + h;

   float xc = x1 + w/2;

   if (mLoop.level_editor_running)
   {
      al_draw_bitmap(mBitmap.tile[987], x, y, 0); // draw item shape in level editor, invisible when game running
   }
   al_draw_textf(mFont.pr8, mColor.pc[15], x+10, y-10, ALLEGRO_ALIGN_CENTER, "st:%d  [%d]", state, time/10);

   int percent = 0;

   if (state == 1)
   {
      int lt = item[i][10]; // total time
      int dt = time;        // current time
      if (lt > 0)  // prevent divide by zero
      {
//         tts = (dt / 4) + 1;
         //percent = (dt * 100) / lt;
         percent = 100 - (dt * 100) / lt;
      }
   }


   if (state == 2)
   {
      int lt = item[i][11]; // total time
      int dt = time;        // current time
      if (lt > 0)  // prevent divide by zero
      {
//         tts = (dt / 4) + 1;
         percent = (dt * 100) / lt;
         //percent = 100 - (dt * 100) / lt;
      }
   }

//   mScreen.draw_percent_bar(x+20, y+30, 64, 16, percent);
//   mScreen.draw_percent_bar(xc, y1, w, h, percent);

   mScreen.draw_percent_bar(xc, y1+5, w-20, h-10, percent);

   return 1;
}


void mwItems::proc_timer(int i)
{
   int state = 0;
   int t1_mode = 0;
   int t2_mode = 0;
   int cnt = 0;
   get_timer_flags(item[i][3], state, t1_mode, t2_mode, cnt);

   // clear output events
   mTriggerEvent.event[item[i][13]] = 0;
   mTriggerEvent.event[item[i][15]] = 0;

   if (state == 1)
   {
      // check to see if we should run down the timer
      switch (t1_mode)
      {
         case 0: // free run
            cnt--;
         break;
         case 1: // free run after trigger
            if (cnt < item[i][10]) cnt--;                      // already triggered
            else if (mTriggerEvent.event[item[i][12]]) cnt--;  // look for event to start
         break;
         case 2: // run while trigger
            if (mTriggerEvent.event[item[i][12]]) cnt--;
         break;
         case 3: // run while trigger, reset if not
            if (mTriggerEvent.event[item[i][12]]) cnt--;
            else cnt = item[i][10]; // reset timer
         break;
      }
      if (cnt <= 0)
      {
         state = 2;                            // set state 2
         cnt = item[i][11];                    // reset with timer 2
         mTriggerEvent.event[item[i][13]] = 1; // emit timer 1 end event
      }
   }
   if (state == 2)
   {
      // check to see if we should run down the timer
      switch (t2_mode)
      {
         case 0: // free run
            cnt--;
         break;
         case 1: // free run after trigger
            if (cnt < item[i][11]) cnt--;               // already triggered
            else if (mTriggerEvent.event[item[i][14]]) cnt--;  // look for event to start
         break;
         case 2: // run while trigger
            if (mTriggerEvent.event[item[i][14]]) cnt--;
         break;
         case 3: // run while trigger, reset if not
            if (mTriggerEvent.event[item[i][14]]) cnt--;
            else cnt = item[i][11]; // reset timer
         break;
      }
      if (cnt <= 0)
      {
         state = 1;                            // set state 1
         cnt = item[i][10];                    // reset with timer 1
         mTriggerEvent.event[item[i][15]] = 1; // emit timer 2 end event
      }
   }
   set_timer_flags(item[i][3], state, t1_mode, t2_mode, cnt);
}

void mwItems::set_timer_flags(int &pack, int state, int t1_mode, int t2_mode, int cnt)
{
   pack = state -1;
   int H16 = cnt<<16; // shift to upper 16 bits
   H16 &= 0b11111111111111110000000000000000; // clear other bits
   pack += H16;
   int t1m = t1_mode << 14; // shift to bits 14 and 15
   t1m &= 0b00000000000000001100000000000000; // clear other bits
   pack += t1m;
   int t2m = t2_mode << 12; // shift to bits 12 and 13
   t2m &= 0b00000000000000000011000000000000; // clear other bits
   pack += t2m;
}


void mwItems::get_timer_flags(int pack, int &state, int &t1_mode, int &t2_mode, int &cnt)
{
   cnt = pack>>16;
   t1_mode = pack>>14;
   t1_mode           &= 0b00000000000000000000000000000011;  // clear other bits
   t2_mode = pack>>12;
   t2_mode           &= 0b00000000000000000000000000000011;  // clear other bits
   state = 1 + (pack &  0b00000000000000000000000000000001); // clear other bits
}







