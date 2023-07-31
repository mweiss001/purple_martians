// mwItemTrigger.cpp

#include "pm.h"
#include "mwItem.h"
#include "mwFont.h"
#include "mwBitmap.h"
#include "mwPlayer.h"
#include "mwLift.h"
#include "mwColor.h"
#include "mwTriggerEvent.h"
#include "mwLoop.h"
#include "mwEnemy.h"
#include "mwLevel.h"
#include "mwMiscFnx.h"
#include "mwScreen.h"
#include "mwGameEvent.h"
#include "mwShot.h"

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
item[][11] = STATE OFF pm_event
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

int mwItem::proc_orb_shot_collision(int i)
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


void mwItem::proc_orb(int i)
{
   int MODE = item[i][6];

   // timer mode stuff
   if (MODE == 3) // timed ON
   {
      if (--item[i][8] <= 0)
      {
         item[i][8] = 0;
         item[i][2] &= ~PM_ITEM_ORB_STATE;  // OFF
      }
      else item[i][2] |= PM_ITEM_ORB_STATE; // ON
   }
   if (MODE == 4) // timed OFF
   {
      if (--item[i][8] <= 0)
      {
         item[i][8] = 0;
         item[i][2] |= PM_ITEM_ORB_STATE;    // ON
      }
      else item[i][2] &= ~PM_ITEM_ORB_STATE; // OFF
   }


   if ((item[i][2] & PM_ITEM_ORB_TRIG_SHOT) && (proc_orb_shot_collision(i))) item[i][2] |= PM_ITEM_ORB_TRIG_CURR; // set CURR flag



   // trigger stuff
   if (item[i][2] & PM_ITEM_ORB_TRIG_CURR)                     // currently triggered
   {
      if ((MODE == 3) || (MODE == 4)) item[i][8] = item[i][7]+1; // reset counter
      if (!(item[i][2] & PM_ITEM_ORB_TRIG_PREV))                 // not triggered last time
      {
         item[i][2] |= PM_ITEM_ORB_TRIG_PREV;                  // set PREV flag
         if (MODE == 0) item[i][2] ^= PM_ITEM_ORB_STATE;       // toggle state
         if (MODE == 1) item[i][2] |= PM_ITEM_ORB_STATE;       // stick ON
         if (MODE == 2) item[i][2] &= ~PM_ITEM_ORB_STATE;      // stick OFF
      }
   }
   else item[i][2] &= ~PM_ITEM_ORB_TRIG_PREV;                  // clear PREV flag
   item[i][2] &= ~PM_ITEM_ORB_TRIG_CURR;                       // clear CURR flag



   // STATE stuff
   item[i][2] &= ~PM_ITEM_ORB_TGON;               // clear TGON flag
   item[i][2] &= ~PM_ITEM_ORB_TGOF;               // clear TGOF flag
   if (item[i][2] & PM_ITEM_ORB_STATE)            // orb state ON
   {
      item[i][1] = 419;                           // red orb
      if (!(item[i][2] & PM_ITEM_ORB_PREV_STATE)) // prev OFF
      {
         item[i][2] |= PM_ITEM_ORB_TGON;          // set TGON flag
         item[i][2] |= PM_ITEM_ORB_PREV_STATE;    // set PREV flag
      }
   }
   else                                           // orb state OFF
   {
      item[i][1] = 418;                           // green orb
      if ((item[i][2] & PM_ITEM_ORB_PREV_STATE))  // prev ON
      {
         item[i][2] |=  PM_ITEM_ORB_TGOF;         // set TGOF flag
         item[i][2] &= ~PM_ITEM_ORB_PREV_STATE;   // clear PREV flag
      }
   }

   // clear all events
   mTriggerEvent.event[item[i][10]] = 0;
   mTriggerEvent.event[item[i][11]] = 0;
   mTriggerEvent.event[item[i][12]] = 0;
   mTriggerEvent.event[item[i][13]] = 0;
   int FLAGS = item[i][2];
   if   (FLAGS & PM_ITEM_ORB_STATE)  mTriggerEvent.event[item[i][10]] = 1;
   if (!(FLAGS & PM_ITEM_ORB_STATE)) mTriggerEvent.event[item[i][11]] = 1;
   if   (FLAGS & PM_ITEM_ORB_TGON)   mTriggerEvent.event[item[i][12]] = 1;
   if   (FLAGS & PM_ITEM_ORB_TGOF)   mTriggerEvent.event[item[i][13]] = 1;







}

int mwItem::draw_orb(int i, int x, int y)
{
   item[i][1] = 418;                                      // green orb
   if (item[i][2] & PM_ITEM_ORB_STATE) item[i][1] = 419;  // red orb

   // rotation
   int rb = (item[i][2] & PM_ITEM_ORB_ROTB) >> 14;
   float a=0, xo=0, yo=0; // angle, x and y offsets
   if (rb == 0) { a = 0;             xo = 10, yo = 9; } // floor
   if (rb == 1) { a = ALLEGRO_PI/2;  xo = 7,  yo = 7; } // wall left
   if (rb == 2) { a = ALLEGRO_PI;    xo = 9,  yo = 4; } // ceiling
   if (rb == 3) { a = -ALLEGRO_PI/2; xo = 12, yo = 6; } // wall right

   int MODE = item[i][6];
   int drawn = 0;
   if ((MODE == 3) || (MODE == 4))
   {
      int c1=11, c2=10;
      if (MODE == 4) {c1=10; c2=11;}
      int percent =  ((item[i][8]-1) * 100) / item[i][7];
      if (percent > 0)
      {
         mScreen.draw_percent_barc(x+xo, y+yo, 7, 7,  percent, c1, c2, -1);
         al_draw_rotated_bitmap(mBitmap.tile[417], 10, 10, x+10, y+10, a, 0);
         drawn = 1;
      }
   }
   if (!drawn) al_draw_rotated_bitmap(mBitmap.tile[item[i][1]], 10, 10, x+10, y+10, a, 0);
   return 1;
}
void mwItem::proc_orb_collision(int p, int i)
{
   if (  (item[i][2] & PM_ITEM_ORB_TRIG_TOUCH) ||
        ((item[i][2] & PM_ITEM_ORB_TRIG_UP)   && (mPlayer.syn[p].up)) ||
        ((item[i][2] & PM_ITEM_ORB_TRIG_DOWN) && (mPlayer.syn[p].down)) )
   {
      item[i][2] |= PM_ITEM_ORB_TRIG_CURR;
      if (item[i][12] == 99)
      {
         mPlayer.syn[0].level_done_mode = 30;
         mPlayer.syn[0].level_done_player = p;
         mPlayer.syn[0].level_done_frame = mLoop.frame_num;
         mPlayer.syn[0].level_done_next_level = 1;
      }
   }
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

void mwItem::proc_trigger(int i)
{
   int& flags = item[i][3];

   if (flags & PM_ITEM_TRIGGER_LIFT_ON) set_item_trigger_location_from_lift(i, 0);

   flags &= ~PM_ITEM_TRIGGER_TGON;  // clear toggle ON  trigger flag
   flags &= ~PM_ITEM_TRIGGER_TGOF;  // clear toggle OFF trigger flag
   flags &= ~PM_ITEM_TRIGGER_CURR;  // clear current    trigger flag

   detect_trigger_collisions(i);

   if ( (flags &  PM_ITEM_TRIGGER_CURR)    // is current trigger flag set?
   && (!(flags &  PM_ITEM_TRIGGER_PREV)))  // and previous trigger flag not set?
         flags |= PM_ITEM_TRIGGER_TGON;    // set trigger ON toggle

   if (!(flags &  PM_ITEM_TRIGGER_CURR)    // is current trigger flag not set?
   && ( (flags &  PM_ITEM_TRIGGER_PREV)))  // and previous trigger flag set?
         flags |= PM_ITEM_TRIGGER_TGOF;    // set trigger OFF toggle

   if   (flags &   PM_ITEM_TRIGGER_CURR)    // is current trigger flag set?
         flags |=  PM_ITEM_TRIGGER_PREV;    // set previous trigger flag

   if (!(flags &   PM_ITEM_TRIGGER_CURR))   // is current trigger flag not set?
         flags &= ~PM_ITEM_TRIGGER_PREV;    // clear previous trigger flag

   // clear all events
   mTriggerEvent.event[item[i][11]] = 0;
   mTriggerEvent.event[item[i][12]] = 0;
   mTriggerEvent.event[item[i][13]] = 0;
   mTriggerEvent.event[item[i][14]] = 0;

   if   (flags & PM_ITEM_TRIGGER_CURR)  mTriggerEvent.event[item[i][11]] = 1;
   if (!(flags & PM_ITEM_TRIGGER_CURR)) mTriggerEvent.event[item[i][12]] = 1;
   if   (flags & PM_ITEM_TRIGGER_TGON)  mTriggerEvent.event[item[i][13]] = 1;
   if   (flags & PM_ITEM_TRIGGER_TGOF)  mTriggerEvent.event[item[i][14]] = 1;
}

void mwItem::set_item_trigger_location_from_lift(int i, int a20)
{
   int d = item[i][10]; // lift number
   if (mLift.cur[d].active) // only proceed if lift number is valid
   {
      // x axis
      int lx1 = mLift.cur[d].x;
      int lx2 = mLift.cur[d].x + mLift.cur[d].w;
      int C = item[i][3] & PM_ITEM_TRIGGER_LIFT_XC;
      int F = item[i][3] & PM_ITEM_TRIGGER_LIFT_XF;
      int L = item[i][3] & PM_ITEM_TRIGGER_LIFT_XL;
      if (C)
      {
         int lxc = lx1 + (lx2-lx1)/2; // get center of lift
         item[i][6] = lxc - item[i][8]/2;
      }
      else
      {
         if ((!F) && (!L)) item[i][6] = lx1;             // fx1 = lx1
         if ((!F) && ( L)) item[i][6] = lx2;             // fx1 = lx2
         if (( F) && (!L)) item[i][6] = lx1 - item[i][8]; // fx2 = lx1
         if (( F) && ( L)) item[i][6] = lx2 - item[i][8]; // fx2 = lx2
      }
      // y axis
      int ly1 = mLift.cur[d].y;
      int ly2 = mLift.cur[d].y + mLift.cur[d].h;
      C = item[i][3] & PM_ITEM_TRIGGER_LIFT_YC;
      F = item[i][3] & PM_ITEM_TRIGGER_LIFT_YF;
      L = item[i][3] & PM_ITEM_TRIGGER_LIFT_YL;

      if (C)
      {
         int lyc = ly1 + (ly2-ly1)/2; // get center of lift
         item[i][7] = lyc - item[i][9]/2;
      }
      else
      {
         if ((!F) && (!L)) item[i][7] = ly1;             // fy1 = ly1
         if ((!F) && ( L)) item[i][7] = ly2;             // fy1 = ly2
         if (( F) && (!L)) item[i][7] = ly1 - item[i][9]; // fy2 = ly1
         if (( F) && ( L)) item[i][7] = ly2 - item[i][9]; // fy2 = ly2
      }
      if (a20) // align to 20 grid
      {
         item[i][6] = mMiscFnx.round20(item[i][6]);
         item[i][7] = mMiscFnx.round20(item[i][7]);
      }
   }
}


void mwItem::detect_trigger_collisions(int i)
{
   int& flags = item[i][3];

   // trigger field
   int tfx1 = item[i][6]-10;
   int tfy1 = item[i][7]-10;
   int tfx2 = tfx1 + item[i][8];
   int tfy2 = tfy1 + item[i][9];

   if (flags & PM_ITEM_TRIGGER_PLAYER)
      for (int p=0; p<NUM_PLAYERS; p++)
         if ((mPlayer.syn[p].active) && (!mPlayer.syn[p].paused))
         {
            int x = mPlayer.syn[p].x;
            int y = mPlayer.syn[p].y;
            if ((x > tfx1) && (x < tfx2) && (y > tfy1) && (y < tfy2)) flags |= PM_ITEM_TRIGGER_CURR;
         }
   if (flags & PM_ITEM_TRIGGER_ENEMY)
      for (int e=0; e<100; e++)
         if (mEnemy.Ei[e][0])
         {
            int x = mEnemy.Ef[e][0];
            int y = mEnemy.Ef[e][1];
            if ((x > tfx1) && (x < tfx2) && (y > tfy1) && (y < tfy2)) flags |= PM_ITEM_TRIGGER_CURR;
         }
   if (flags & PM_ITEM_TRIGGER_ITEM)
      for (int c=0; c<500; c++)
         if (item[c][0])
         {
            int x = itemf[c][0];
            int y = itemf[c][1];
            if ((x > tfx1) && (x < tfx2) && (y > tfy1) && (y < tfy2)) flags |= PM_ITEM_TRIGGER_CURR;
         }
   if (flags & PM_ITEM_TRIGGER_PSHOT) // check player shots
      for (int b=0; b<50; b++)
         if (mShot.p[b].active)
         {
            int x = mShot.p[b].x;
            int y = mShot.p[b].y;
            if ((x > tfx1) && (x < tfx2) && (y > tfy1) && (y < tfy2)) flags |= PM_ITEM_TRIGGER_CURR;
         }
   if (flags & PM_ITEM_TRIGGER_ESHOT) // check enemy shots
      for (int b=0; b<50; b++)
         if (mShot.e[b].active)
         {
            int x = mShot.e[b].x;
            int y = mShot.e[b].y;
            if ((x > tfx1) && (x < tfx2) && (y > tfy1) && (y < tfy2)) flags |= PM_ITEM_TRIGGER_CURR;
         }
}


int mwItem::draw_trigger(int i, int x, int y, int custom)
{
   if (mLoop.level_editor_running)
   {
      al_draw_bitmap(mBitmap.tile[991], x, y, 0); // draw item shape in level editor, invisible when game running
      if (item[i][3] & PM_ITEM_TRIGGER_LIFT_ON) set_item_trigger_location_from_lift(i, 1); // snap to lift here because main function wont be called while in level editor
   }

   if ((item[i][3] & PM_ITEM_TRIGGER_DRAW_ON) && (!custom))
   {
      int col = item[i][2];
      float x1 = item[i][6];
      float y1 = item[i][7];
      float x2 = x1 + item[i][8];
      float y2 = y1 + item[i][9];
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
item[][6] = trigger field x (2000)
item[][7] = trigger field y (2000)
item[][8] = trigger field w (2000)
item[][9] = trigger field x (2000)
item[][10] block 1
item[][11] block 2
item[][12] = draw color
item[][13] = copy x
item[][14] = copy y




*/
void mwItem::proc_block_manip(int i)
{
   int et = item[i][1]; // pm_event trigger we are looking for
   if (mTriggerEvent.event[et])
   {
      al_set_target_bitmap(mBitmap.level_background);
      int x1 = item[i][6]/20;
      int y1 = item[i][7]/20;
      int x2 = x1 + item[i][8]/20;
      int y2 = y1 + item[i][9]/20;

      if (x1 < 0) x1 = 0;
      if (x2 > 100) x2 = 100;
      if (y1 < 0) y1 = 0;
      if (y2 > 100) y2 = 100;

      // copt rect pos
      int x3 = item[i][13]/20;
      int y3 = item[i][14]/20;

      // copt rect offset
      int x4 = x3-x1;
      int y4 = y3-y1;

      for (int x=x1; x<x2; x++)
         for (int y=y1; y<y2; y++)
         {
            int mode = item[i][3];
            int block1 = item[i][10];
            int block2 = item[i][11];
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
            if (mode == 4) // copy rect area
            {
               block1 = mLevel.l[x+x4][y+y4];
               mLevel.l[x][y] = block1;
               al_draw_filled_rectangle(x*20, y*20, x*20+20, y*20+20, mColor.pc[0]);
               al_draw_bitmap(mBitmap.btile[block1&1023], x*20, y*20, 0 );

            }
         }
   }
}

int mwItem::draw_block_manip(int i, int x, int y)
{
   if (mLoop.level_editor_running)
   {
      al_draw_bitmap(mBitmap.tile[989], x, y, 0); // draw item shape in level editor, invisible when game running
   }
   if (item[i][2]) // draw mode on
   {
      int col = item[i][12];
      float x1 = item[i][6];
      float y1 = item[i][7];
      float x2 = x1 + item[i][8];
      float y2 = y1 + item[i][9];
      mMiscFnx.rectangle_with_diagonal_lines(x1, y1, x2, y2, 10, col, col+96, 0);

      if (mItem.item[i][3] == 4)
      {
         col = item[i][12];
         x1 = item[i][13];
         y1 = item[i][14];
         x2 = x1 + item[i][8];
         y2 = y1 + item[i][9];
         mMiscFnx.rectangle_with_diagonal_lines(x1, y1, x2, y2, 10, col, col+96, 0);
      }

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
#define PM_ITEM_DAMAGE_PSHOT    0b00000000000001000
#define PM_ITEM_DAMAGE_ESHOT    0b00000000000010000
#define PM_ITEM_DAMAGE_CURR     0b00000000000100000
#define PM_ITEM_DAMAGE_LIFT_ON  0b00000000001000000
#define PM_ITEM_DAMAGE_LIFT_XC  0b00000000010000000
#define PM_ITEM_DAMAGE_LIFT_XF  0b00000000100000000
#define PM_ITEM_DAMAGE_LIFT_XL  0b00000001000000000
#define PM_ITEM_DAMAGE_LIFT_YC  0b00000010000000000
#define PM_ITEM_DAMAGE_LIFT_YF  0b00000100000000000
#define PM_ITEM_DAMAGE_LIFT_YL  0b00001000000000000

item[][4]  = x pos
item[][5]  = y pos
item[][6]  = field x
item[][7]  = field y
item[][8]  = field w
item[][9]  = field h
item[][10] = lift number
item[][11] = mode
item[][15] = damage

*/

void mwItem::set_item_damage_location_from_lift(int i, int a20)
{
   int d = item[i][10]; // lift number
   if (mLift.cur[d].active) // only proceed if lift number is valid
   {
      // x axis
      int lx1 = mLift.cur[d].x;
      int lx2 = mLift.cur[d].x + mLift.cur[d].w;
      int C = item[i][3] & PM_ITEM_DAMAGE_LIFT_XC;
      int F = item[i][3] & PM_ITEM_DAMAGE_LIFT_XF;
      int L = item[i][3] & PM_ITEM_DAMAGE_LIFT_XL;
      if (C)
      {
         int lxc = lx1 + (lx2-lx1)/2; // get center of lift
         item[i][6] = lxc - item[i][8]/2;
      }
      else
      {
         if ((!F) && (!L)) item[i][6] = lx1;              // fx1 = lx1
         if ((!F) && ( L)) item[i][6] = lx2;              // fx1 = lx2
         if (( F) && (!L)) item[i][6] = lx1 - item[i][8]; // fx2 = lx1
         if (( F) && ( L)) item[i][6] = lx2 - item[i][8]; // fx2 = lx2
      }
      // y axis
      int ly1 = mLift.cur[d].y;
      int ly2 = mLift.cur[d].y + mLift.cur[d].h;
      C = item[i][3] & PM_ITEM_DAMAGE_LIFT_YC;
      F = item[i][3] & PM_ITEM_DAMAGE_LIFT_YF;
      L = item[i][3] & PM_ITEM_DAMAGE_LIFT_YL;

      if (C)
      {
         int lyc = ly1 + (ly2-ly1)/2; // get center of lift
         item[i][7] = lyc - item[i][9]/2;
      }
      else
      {
         if ((!F) && (!L)) item[i][7] = ly1;              // fy1 = ly1
         if ((!F) && ( L)) item[i][7] = ly2;              // fy1 = ly2
         if (( F) && (!L)) item[i][7] = ly1 - item[i][9]; // fy2 = ly1
         if (( F) && ( L)) item[i][7] = ly2 - item[i][9]; // fy2 = ly2
      }
      if (a20) // align to 20 grid
      {
         item[i][6] = mMiscFnx.round20(item[i][6]);
         item[i][7] = mMiscFnx.round20(item[i][7]);
      }
   }
}

void mwItem::proc_item_damage_collisions(int i)
{
   int FLAGS = item[i][3];
   int cd  =            FLAGS & PM_ITEM_DAMAGE_CURR;     // damage active
   int cdp =  ((cd) && (FLAGS & PM_ITEM_DAMAGE_PLAYER)); // damage active and player flag
   int cde =  ((cd) && (FLAGS & PM_ITEM_DAMAGE_ENEMY));  // damage active and enemy flag
   int cdi =  ((cd) && (FLAGS & PM_ITEM_DAMAGE_ITEM));   // damage active and item flag
   int cdpb = ((cd) && (FLAGS & PM_ITEM_DAMAGE_PSHOT));  // damage active and player shot flag
   int cdeb = ((cd) && (FLAGS & PM_ITEM_DAMAGE_ESHOT));  // damage active and enemy shot flag

   // damage field
   int tfx1 = item[i][6]-10;
   int tfy1 = item[i][7]-10;
   int tfx2 = tfx1 + item[i][8];
   int tfy2 = tfy1 + item[i][9];

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
               }
               else
               {
                  float ha = (float)item[i][15]/100; // health adjust
                  mPlayer.syn[p].health -= ha;

                  if (ha > 0) // lose health
                  {
                     int damage_type = 1; // default
                     if (item[i][2] == 2) damage_type = 2;
                     mGameEvent.add(11, 0, 0, p, damage_type, 0, item[i][15]); // only do damage event when taking health
                  }
                  else // gain health
                  {
                     if (mPlayer.syn[p].health > 100) mPlayer.syn[p].health = 100;
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
            }
         }
   if (cdi)
      for (int i=0; i<500; i++)
         if (item[i][0])
         {
            int x = itemf[i][0];
            int y = itemf[i][1];
            if ((x > tfx1) && (x < tfx2) && (y > tfy1) && (y < tfy2))
            {
               // orb, trig, bm, bd -- kill these immed
               if ((item[i][0] == 6) || (item[i][0] == 9) || (item[i][0] == 16) || (item[i][0] == 17)) item[i][0] = 0;

               if ((item[i][0] != 66) && (item[i][0] != 5)) // never kill start
               {
                  //item[i][0] = 66;
                  item[i][14] = 10;
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

int mwItem::draw_block_damage(int i, int x, int y, int custom)
{
   int draw_mode = item[i][2];
   int FLAGS = item[i][3];
   float x0 = x;
   float y0 = y;
   float x1 = item[i][6];
   float y1 = item[i][7];
   float x2 = x1 + item[i][8];
   float y2 = y1 + item[i][9];

   if (mLoop.level_editor_running)
   {
      al_draw_bitmap(mBitmap.tile[988], x0, y0, 0);                                 // only draw item shape in level editor, invisible when game running
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
         int tn = 804; // off by default
         if (item[i][11] == 2) // mode on unless triggered
         {
            tn = 806; // 2/3 on by default
            if (mTriggerEvent.event[item[i][1]]) // event currently triggered (damage off)
            {
               float rtio = get_timer_ratio_for_event(item[i][1]);
               if ((rtio >= 0.0) && (rtio < 0.33)) tn = 804;
               if ((rtio > 0.33) && (rtio < 0.66)) tn = 805;
               if ((rtio > 0.66) && (rtio < 1.00)) tn = 806;
            }
         }
         if (item[i][11] == 3) // mode off unless triggered
         {
            tn = 806; // 2/3 on by default
            if (!mTriggerEvent.event[item[i][1]]) // event currently not triggered (damage off)
            {
               float rtio = get_timer_ratio_for_event(item[i][1]);
               if ((rtio >= 0.0) && (rtio < 0.33)) tn = 804;
               if ((rtio > 0.33) && (rtio < 0.66)) tn = 805;
               if ((rtio > 0.66) && (rtio < 1.00)) tn = 806;
            }
         }

         if (FLAGS & PM_ITEM_DAMAGE_CURR) tn = 807;

         //printf("f:%d rtio:%f tn:%d\n", mLoop.frame_num, rtio, tn);

         for (int hx=x1; hx<x2; hx+=20)
            al_draw_bitmap(mBitmap.tile[tn], hx, y2-20, 0); // draw spikes only on bottom row
      }
   }
   return 1;
}

void mwItem::proc_block_damage(int i)
{
   int& flags = item[i][3];

   int et = item[i][1];                 // number of pm_event trigger we are looking for
   int trig = mTriggerEvent.event[et];  // is the trigger event set?
   if (et == 0) trig = 0;               // if event is zero, ignore


   if (flags & PM_ITEM_DAMAGE_LIFT_ON) set_item_damage_location_from_lift(i, 0); // follow lift location

   proc_item_damage_collisions(i);

   int mode = item[i][11];

   if (mode == 0) flags |= PM_ITEM_DAMAGE_CURR;             // current damage always set

   if ((mode == 1) && (trig)) flags ^= PM_ITEM_DAMAGE_CURR; // toggle current damage flag

   if (mode == 2) // damage on unless triggered
   {
      if (trig) flags &= ~PM_ITEM_DAMAGE_CURR; // damage off
      else      flags |=  PM_ITEM_DAMAGE_CURR; // damage on
   }
   if (mode == 3) // damage off unless triggered
   {
      if (trig) flags |=  PM_ITEM_DAMAGE_CURR; // damage on
      else      flags &= ~PM_ITEM_DAMAGE_CURR; // damage off
   }
}


/*
item[][0]  = 13 - Timer
item[][1]  =
item[][2]  = draw_mode 0 = Off 1 = progress bar
item[][3]  = flags
item[][4]  = x
item[][5]  = y

item[][6]  = display x
item[][7]  = display y
item[][8]  = display w
item[][9]  = display h

item[][10] = t1 reset val
item[][11] = t2 reset val
item[][12] = t1 i/p event
item[][13] = t1 o/p event
item[][14] = t2 i/p event
item[][15] = t2 o/p event
                                              00000000        00000000
                                      00000000        00000000
#define PM_ITEM_TIMER_COUNT         0b11111111111111110000000000000000
#define PM_ITEM_TIMER_T1_MODE       0b00000000000000001110000000000000
#define PM_ITEM_TIMER_T2_MODE       0b00000000000000000001110000000000
#define PM_ITEM_TIMER_T1_OP_MODE    0b00000000000000000000001000000000
#define PM_ITEM_TIMER_T2_OP_MODE    0b00000000000000000000000100000000
#define PM_ITEM_TIMER_STATE_CURR    0b00000000000000000000000000000001

*/


int mwItem::draw_timer(int i, int x, int y, int custom)
{
   int state, t1, t2, t1om, t2om, time;
   get_timer_flags(item[i][3], state, t1, t2, t1om, t2om, time);

   float x1 = item[i][6];
   float y1 = item[i][7];
   float w =  item[i][8];
   float h =  item[i][9];
   float x2 = x1 + w;
   float y2 = y1 + h;

   if (mLoop.level_editor_running)
   {
      al_draw_bitmap(mBitmap.tile[987], x, y, 0); // draw item shape in level editor, invisible when game running
   }
   if ((!custom) && (item[i][2]))
   {
      int percent = 0;
      if (state == 1)
      {
         int lt = item[i][10]; // total time
         int dt = time;        // current time
         if (lt > 0)           // prevent divide by zero
         {
            percent = 100 - (dt * 100) / lt;
         }
      }
      if (state == 2)
      {
         int lt = item[i][11]; // total time
         int dt = time;        // current time
         if (lt > 0)           // prevent divide by zero
         {
            percent = (dt * 100) / lt;
         }
      }

      int z = 2;
      mScreen.draw_percent_barf(x1+z, y1+z, x2-z, y2-z, percent);
      //al_draw_textf(mFont.pr8, mColor.pc[15], x+10, y-10, ALLEGRO_ALIGN_CENTER, "st:%d  [%d]", state, time/10);
   }
   return 1;
}

void mwItem::proc_timer(int i)
{
   int state = 0;
   int t1_mode = 0;
   int t2_mode = 0;
   int t1_op_mode = 0;
   int t2_op_mode = 0;
   int cnt = 0;
   get_timer_flags(item[i][3], state, t1_mode, t2_mode, t1_op_mode, t2_op_mode, cnt);

   // clear output events
   mTriggerEvent.event[item[i][13]] = 0;
   mTriggerEvent.event[item[i][15]] = 0;

   if (state == 1)
   {
      if (t1_op_mode) mTriggerEvent.event[item[i][13]] = 1; // emit timer 1 event continuosly
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
         case 4: // reset if triggered, run while not triggered
            if (mTriggerEvent.event[item[i][12]]) cnt = item[i][10];
            else cnt--;
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
      if (t2_op_mode) mTriggerEvent.event[item[i][15]] = 1; // emit timer 2 event continuosly
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
         case 4: // reset if triggered, run while not triggered
            if (mTriggerEvent.event[item[i][14]]) cnt = item[i][11];
            else cnt--;
         break;
      }
      if (cnt <= 0)
      {
         state = 1;                            // set state 1
         cnt = item[i][10];                    // reset with timer 1
         mTriggerEvent.event[item[i][15]] = 1; // emit timer 2 end event
      }
   }
   set_timer_flags(item[i][3], state, t1_mode, t2_mode, t1_op_mode, t2_op_mode, cnt);
}

void mwItem::set_timer_flags(int &pack, int state, int t1_mode, int t2_mode, int t1_op_mode, int t2_op_mode, int cnt)
{
   pack = state -1;
   int H16 = cnt << 16;                       // shift to upper 16 bits
   H16 &= 0b11111111111111110000000000000000; // clear other bits
   pack += H16;
   int t1m = t1_mode << 13;                   // shift to bits 13, 14 and 15
   t1m &= 0b00000000000000001110000000000000; // clear other bits
   pack += t1m;
   int t2m = t2_mode << 10;                   // shift to bits 10, 11 and 12
   t2m &= 0b00000000000000000001110000000000; // clear other bits
   pack += t2m;

   int t1om = t1_op_mode << 9;                 // shift to bit 9
   t1om &= 0b00000000000000000000001000000000; // clear other bits
   pack += t1om;

   int t2om = t2_op_mode << 8;                 // shift to bit 8
   t2om &= 0b00000000000000000000000100000000; // clear other bits
   pack += t2om;

}

void mwItem::get_timer_flags(int pack, int &state, int &t1_mode, int &t2_mode, int &t1_op_mode, int &t2_op_mode, int &cnt)
{
   cnt = pack >> 16;
   t1_mode = pack >> 13;
   t1_mode           &= 0b00000000000000000000000000000111;  // clear other bits

   t2_mode = pack >> 10;
   t2_mode           &= 0b00000000000000000000000000000111;  // clear other bits

   t1_op_mode = pack >> 9;
   t1_op_mode        &= 0b00000000000000000000000000000001;  // clear other bits

   t2_op_mode = pack >> 8;
   t2_op_mode        &= 0b00000000000000000000000000000001;  // clear other bits


   state = 1 + (pack &  0b00000000000000000000000000000001); // clear other bits
}

float mwItem::get_timer_ratio_for_event(int ev)
{
   int ti = -1;
   for (int i=0; i<500; i++)
      if (item[i][0] == 13) // timer
      {
         if (item[i][13] == ev) {ti = i; i=500; }
         if (item[i][15] == ev) {ti = i; i=500; }
      }
   if (ti > -1)
   {
      int state, t1, t2, t1om, t2om, cnt;
      get_timer_flags(item[ti][3], state, t1, t2, t1om, t2om, cnt);
      float tot;
      if (state == 1) tot = item[ti][10];
      if (state == 2) tot = item[ti][11];
      if (tot != 0) return 1 - (float)cnt / tot;
   }
   return 0;
}
