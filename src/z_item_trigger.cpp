// z_item_trigger.cpp

#include "pm.h"
#include "z_item.h"
#include "z_item_trigger.h"
//#include "z_sound.h"
#include "z_player.h"
#include "mwFont.h"
#include "mwBitmap.h"
#include "mwLift.h"
//#include "mwWidgets.h"
#include "mwColor.h"
#include "mwPMEvent.h"
//#include "mwDisplay.h"
#include "mwProgramState.h"
//#include "z_menu.h"
#include "z_enemy.h"
#include "mwLevel.h"
#include "e_fnx.h"
#include "z_screen.h"
#include "z_screen_overlay.h"
#include "mwShots.h"
//#include "z_solid.h"
//#include "z_item_key.h"
//#include "z_item_bomb.h"
//#include "z_item_pmsg.h"
//#include "z_item_door.h"



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

int proc_orb_shot_collision(int i)
{
   int s = 8; // collison box size
   float x = itemf[i][0];
   float y = itemf[i][1];
   for (int b=0; b<50; b++)
      if (mwS.p[b].active)
      {
         float bx = mwS.p[b].x;
         float by = mwS.p[b].y;
         if ((x > bx-s) && (x < bx+s) && (y > by-s) && (y < by+s)) return 1;
      }
   return 0;
}


void proc_orb(int i)
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
   mwPME.event[item[i][10]] = 0;
   mwPME.event[item[i][11]] = 0;
   mwPME.event[item[i][12]] = 0;
   mwPME.event[item[i][13]] = 0;
   int FLAGS = item[i][2];
   if   (FLAGS & PM_ITEM_ORB_STATE)  mwPME.event[item[i][10]] = 1;
   if (!(FLAGS & PM_ITEM_ORB_STATE)) mwPME.event[item[i][11]] = 1;
   if   (FLAGS & PM_ITEM_ORB_TGON)   mwPME.event[item[i][12]] = 1;
   if   (FLAGS & PM_ITEM_ORB_TGOF)   mwPME.event[item[i][13]] = 1;
}

void draw_orb(int i, int x, int y)
{
   item[i][1] = 418;                                          // green orb
   if (item[i][2] & PM_ITEM_ORB_STATE) item[i][1] = 419;      // red orb

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
         draw_percent_barc(x+xo, y+yo, 7, 7,  percent, c1, c2, -1);
         al_draw_rotated_bitmap(mwB.tile[417], 10, 10, x+10, y+10, a, 0);
         drawn = 1;
      }
   }
   if (!drawn) al_draw_rotated_bitmap(mwB.tile[item[i][1]], 10, 10, x+10, y+10, a, 0);
}
void proc_orb_collision(int p, int i)
{
   if (  (item[i][2] & PM_ITEM_ORB_TRIG_TOUCH) ||
        ((item[i][2] & PM_ITEM_ORB_TRIG_UP)   && (players[p].up)) ||
        ((item[i][2] & PM_ITEM_ORB_TRIG_DOWN) && (players[p].down)) )
           item[i][2] |= PM_ITEM_ORB_TRIG_CURR;
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

void proc_trigger(int i)
{
   int FLAGS = item[i][3];
   if (FLAGS & PM_ITEM_TRIGGER_LIFT_ON) set_item_trigger_location_from_lift(i, 0);

   item[i][3] &= ~PM_ITEM_TRIGGER_TGON;  // clear Toggle ON  trigger flag
   item[i][3] &= ~PM_ITEM_TRIGGER_TGOF;  // clear Toggle OFF trigger flag
   item[i][3] &= ~PM_ITEM_TRIGGER_CURR;  // clear current    trigger flag

   detect_trigger_collisions(i);

   if ( (item[i][3] &  PM_ITEM_TRIGGER_CURR)    // is current trigger flag set?
   && (!(item[i][3] &  PM_ITEM_TRIGGER_PREV)))  // and previous trigger flag not set?
         item[i][3] |= PM_ITEM_TRIGGER_TGON;    // set trigger ON toggle


   if (!(item[i][3] &  PM_ITEM_TRIGGER_CURR)    // is current trigger flag not set?
   && ( (item[i][3] &  PM_ITEM_TRIGGER_PREV)))  // and previous trigger flag set?
         item[i][3] |= PM_ITEM_TRIGGER_TGOF;    // set trigger OFF toggle


   if   (item[i][3] &   PM_ITEM_TRIGGER_CURR)    // is current trigger flag set?
         item[i][3] |=  PM_ITEM_TRIGGER_PREV;    // set previous trigger flag

   if (!(item[i][3] &   PM_ITEM_TRIGGER_CURR))   // is current trigger flag not set?
         item[i][3] &= ~PM_ITEM_TRIGGER_PREV;    // clear previous trigger flag

   FLAGS = item[i][3]; // update FLAGS

/*   if (FLAGS & PM_ITEM_TRIGGER_CURR) printf("%d - CURR\n", mwPS.frame_num);
   if (FLAGS & PM_ITEM_TRIGGER_PREV) printf("%d - PREV\n", mwPS.frame_num);
   if (FLAGS & PM_ITEM_TRIGGER_TGON) printf("%d - TGON\n", mwPS.frame_num);
   if (FLAGS & PM_ITEM_TRIGGER_TGOF) printf("%d - TGOF\n", mwPS.frame_num); */

   // clear them all
   mwPME.event[item[i][11]] = 0;
   mwPME.event[item[i][12]] = 0;
   mwPME.event[item[i][13]] = 0;
   mwPME.event[item[i][14]] = 0;

   if   (FLAGS & PM_ITEM_TRIGGER_CURR)  mwPME.event[item[i][11]] = 1;
   if (!(FLAGS & PM_ITEM_TRIGGER_CURR)) mwPME.event[item[i][12]] = 1;
   if   (FLAGS & PM_ITEM_TRIGGER_TGON)  mwPME.event[item[i][13]] = 1;
   if   (FLAGS & PM_ITEM_TRIGGER_TGOF)  mwPME.event[item[i][14]] = 1;
}

void set_item_trigger_location_from_lift(int i, int a20)
{
   int d = item[i][10]; // lift number
   if (Lift.cur[d].active) // only proceed if lift number is valid
   {
      // x axis
      int lx1 = Lift.cur[d].x;
      int lx2 = Lift.cur[d].x + Lift.cur[d].w;
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
      int ly1 = Lift.cur[d].y;
      int ly2 = Lift.cur[d].y + Lift.cur[d].h;
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
         item[i][6] = round20(item[i][6]);
         item[i][7] = round20(item[i][7]);
      }
   }
}


void detect_trigger_collisions(int i)
{
   int FLAGS = item[i][3];

   // trigger field
   int tfx1 = item[i][6]-10;
   int tfy1 = item[i][7]-10;
   int tfx2 = tfx1 + item[i][8];
   int tfy2 = tfy1 + item[i][9];

   if (FLAGS & PM_ITEM_TRIGGER_PLAYER)
      for (int p=0; p<NUM_PLAYERS; p++)
         if ((players[p].active) && (!players[p].paused))
         {
            int x = players[p].x;
            int y = players[p].y;
            if ((x > tfx1) && (x < tfx2) && (y > tfy1) && (y < tfy2)) item[i][3] |= PM_ITEM_TRIGGER_CURR;
         }
   if (FLAGS & PM_ITEM_TRIGGER_ENEMY)
      for (int e2=0; e2<100; e2++)
         if (Ei[e2][0])
         {
            int x = Ef[e2][0];
            int y = Ef[e2][1];
            if ((x > tfx1) && (x < tfx2) && (y > tfy1) && (y < tfy2)) item[i][3] |= PM_ITEM_TRIGGER_CURR;
         }
   if (FLAGS & PM_ITEM_TRIGGER_ITEM)
      for (int c=0; c<500; c++)
         if (item[c][0])
         {
            int x = itemf[c][0];
            int y = itemf[c][1];
            if ((x > tfx1) && (x < tfx2) && (y > tfy1) && (y < tfy2)) item[i][3] |= PM_ITEM_TRIGGER_CURR;
         }
   if (FLAGS & PM_ITEM_TRIGGER_PSHOT) // check player shots
      for (int b=0; b<50; b++)
         if (mwS.p[b].active)
         {
            int x = mwS.p[b].x;
            int y = mwS.p[b].y;
            if ((x > tfx1) && (x < tfx2) && (y > tfy1) && (y < tfy2)) item[i][3] |= PM_ITEM_TRIGGER_CURR;
         }
   if (FLAGS & PM_ITEM_TRIGGER_ESHOT) // check enemy shots
      for (int b=0; b<50; b++)
         if (mwS.e[b].active)
         {
            int x = mwS.e[b].x;
            int y = mwS.e[b].y;
            if ((x > tfx1) && (x < tfx2) && (y > tfy1) && (y < tfy2)) item[i][3] |= PM_ITEM_TRIGGER_CURR;
         }
}





void draw_trigger(int i, int x, int y)
{
   if (mwPS.level_editor_running)
   {
      al_draw_bitmap(mwB.tile[991], x, y, 0); // draw item shape in level editor, invisible when game running
      if (item[i][3] & PM_ITEM_TRIGGER_LIFT_ON) set_item_trigger_location_from_lift(i, 1); // snap to lift here because main function wont be called while in level editor
   }

   if (item[i][3] & PM_ITEM_TRIGGER_DRAW_ON)
   {
      int col = item[i][2];
      float x1 = item[i][6];
      float y1 = item[i][7];
      float x2 = x1 + item[i][8];
      float y2 = y1 + item[i][9];
      rectangle_with_diagonal_lines(x1, y1, x2, y2, 10, col, col+96, 0);
   }
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
void proc_block_manip(int i)
{
   int et = item[i][1]; // pm_event trigger we are looking for
   if (mwPME.event[et])
   {
      al_set_target_bitmap(mwB.level_background);
      int x1 = item[i][6]/20;
      int y1 = item[i][7]/20;
      int x2 = x1 + item[i][8]/20;
      int y2 = y1 + item[i][9]/20;

      for (int x=x1; x<x2; x++)
         for (int y=y1; y<y2; y++)
         {
            int mode = item[i][3];
            int block1 = item[i][10];
            int block2 = item[i][11];

            if (mode == 1) // set all blocks to block 1
            {
               mLevel.l[x][y] = block1;
               al_draw_filled_rectangle(x*20, y*20, x*20+20, y*20+20, mC.pc[0]);
               al_draw_bitmap(mwB.btile[block1&1023], x*20, y*20, 0 );
            }

            if (mode == 2) // set all block2 to block 1
            {
               if (mLevel.l[x][y] == block2)
               {
                  mLevel.l[x][y] = block1;
                  al_draw_filled_rectangle(x*20, y*20, x*20+20, y*20+20, mC.pc[0]);
                  al_draw_bitmap(mwB.btile[block1&1023], x*20, y*20, 0 );
               }
            }

            if (mode == 3) // toggle block1 and block 2
            {
               if (mLevel.l[x][y] == block1)
               {
                  mLevel.l[x][y] = block2;
                  al_draw_filled_rectangle(x*20, y*20, x*20+20, y*20+20, mC.pc[0]);
                  al_draw_bitmap(mwB.btile[block2&1023], x*20, y*20, 0 );
               }
               else if (mLevel.l[x][y] == block2)
               {
                  mLevel.l[x][y] = block1;
                  al_draw_filled_rectangle(x*20, y*20, x*20+20, y*20+20, mC.pc[0]);
                  al_draw_bitmap(mwB.btile[block1&1023], x*20, y*20, 0 );
               }
            }
         }
   }
}



void draw_block_manip(int i, int x, int y)
{
   if (mwPS.level_editor_running)
   {
      al_draw_bitmap(mwB.tile[989], x, y, 0); // draw item shape in level editor, invisible when game running
   }
   if (item[i][2]) // draw mode on
   {
      int col = item[i][12];
      float x1 = item[i][6];
      float y1 = item[i][7];
      float x2 = x1 + item[i][8];
      float y2 = y1 + item[i][9];
      rectangle_with_diagonal_lines(x1, y1, x2, y2, 10, col, col+96, 0);
   }
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

void set_item_damage_location_from_lift(int i, int a20)
{
   int d = item[i][10]; // lift number
   if (Lift.cur[d].active) // only proceed if lift number is valid
   {
      // x axis
      int lx1 = Lift.cur[d].x;
      int lx2 = Lift.cur[d].x + Lift.cur[d].w;
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
      int ly1 = Lift.cur[d].y;
      int ly2 = Lift.cur[d].y + Lift.cur[d].h;
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
         item[i][6] = round20(item[i][6]);
         item[i][7] = round20(item[i][7]);
      }
   }
}

void proc_item_damage_collisions(int i)
{
   int FLAGS = item[i][3];
   int cd = FLAGS & PM_ITEM_DAMAGE_CURR;                 // damage active
   int cdp =  ((cd) && (FLAGS & PM_ITEM_DAMAGE_PLAYER)); // damage active and player flag
   int cde =  ((cd) && (FLAGS & PM_ITEM_DAMAGE_ENEMY));  // damage active and enemy flag
   int cdi =  ((cd) && (FLAGS & PM_ITEM_DAMAGE_ITEM));   // damage active and item flag
   int cdpb = ((cd) && (FLAGS & PM_ITEM_DAMAGE_PSHOT));   // damage active and player shot flag
   int cdeb = ((cd) && (FLAGS & PM_ITEM_DAMAGE_ESHOT));   // damage active and enemy shot flag

   // damage field
   int tfx1 = item[i][6]-10;
   int tfy1 = item[i][7]-10;
   int tfx2 = tfx1 + item[i][8];
   int tfy2 = tfy1 + item[i][9];

   if (cdp)
      for (int p=0; p<NUM_PLAYERS; p++)
         if ((players[p].active) && (!players[p].paused))
         {
            int x = players[p].x;
            int y = players[p].y;
            if ((x > tfx1) && (x < tfx2) && (y > tfy1) && (y < tfy2))
            {
               if (FLAGS & PM_ITEM_DAMAGE_INSTGIB)
               {
                  players[p].health = 0;
                  //game_event(59, 0, 0, p, i, 0, 0);
               }
               else
               {
                  if (item[i][15] > 0) // lose health
                  {
                     players[p].health -= item[i][15]/100;
                     game_event(59, 0, 0, p, i, 0, 0); // only do damage noise when taking health..??
                  }
                  else // gain health
                  {
                     if (players[p].health < 100)
                     {
                        players[p].health -= item[i][15]/100;
                        if (players[p].health > 100) players[p].health = 100;
                     }
                  }
               }
            }
         }
   if (cde)
      for (int e2=0; e2<100; e2++)
         if (Ei[e2][0])
         {
            int x = Ef[e2][0];
            int y = Ef[e2][1];
            if ((x > tfx1) && (x < tfx2) && (y > tfy1) && (y < tfy2))
            {
               Ei[e2][31] = 3;           // flag that this enemy got shot
               //Ei[e2][26] = x;           // number of player's shot that hit enemy
            }
         }
   if (cdi)
      for (int i=0; i<500; i++)
         if (item[i][0])
         {
            int x = itemf[i][0];
            int y = itemf[i][0];
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
         if (mwS.p[b].active)
         {
            int x = mwS.p[b].x;
            int y = mwS.p[b].y;
            if ((x > tfx1) && (x < tfx2) && (y > tfy1) && (y < tfy2)) mwS.p[b].active = 0; // kill the shot
         }
   if (cdeb) // check enemy shots
      for (int b=0; b<50; b++)
         if (mwS.e[b].active)
         {
            int x = mwS.e[b].x;
            int y = mwS.e[b].y;
            if ((x > tfx1) && (x < tfx2) && (y > tfy1) && (y < tfy2)) mwS.e[b].active = 0; // kill the shot
         }
}





void draw_block_damage(int i, int x, int y, int custom)
{
   int draw_mode = item[i][2];
   int mode = item[i][11];
   int FLAGS = item[i][3];
   float x0 = x;
   float y0 = y;
   float x1 = item[i][6];
   float y1 = item[i][7];
   float x2 = x1 + item[i][8];
   float y2 = y1 + item[i][9];

   if (mwPS.level_editor_running)
   {
      al_draw_bitmap(mwB.tile[988], x0, y0, 0); // draw item shape in level editor, invisible when game running
      if (FLAGS & PM_ITEM_DAMAGE_LIFT_ON) set_item_damage_location_from_lift(i, 1); // set this here only when level editor is running
   }

   // damage field drawing
   if (!custom)
   {

      if (draw_mode == 1) // basic
      {
         int col = 11;
         if (FLAGS & PM_ITEM_DAMAGE_CURR) col = 10;
         rectangle_with_diagonal_lines(x1, y1, x2, y2, 10, col, col+96, 0);
      }

      if (draw_mode == 2) // spikey floor
      {
         int tn = 808;
         if (FLAGS & PM_ITEM_DAMAGE_CURR) tn = 807;
         for (int hx = x1; hx<x2; hx+=20)
            al_draw_bitmap(mwB.tile[tn], hx, y2-20, 0); // draw spikes only on bottom row
      }
   }

   // timer drawing
   int timer_draw_mode1 = item[i][3] & PM_ITEM_DAMAGE_TIMR_SN;
   int timer_draw_mode2 = item[i][3] & PM_ITEM_DAMAGE_TIMR_BN;
   int timer_draw_mode3 = item[i][3] & PM_ITEM_DAMAGE_TIMR_SP;
   int timer_draw_mode4 = item[i][3] & PM_ITEM_DAMAGE_TIMR_BP;


   int col = 15;

   if ((mode == 2) || (mode == 3))
   {
      if (((timer_draw_mode1) || (timer_draw_mode2)) && (item[i][13] > 0)) // small or big number and timer is running
      {
         // time to show
         int tts = (item[i][13]); // raw (40ths of a second)
         //int tts = (item[i][13] / 40); // seconds
         //int tts = (item[i][13] / 4); // tenths of a second
         //int tts = (item[i][13] / 8); // fifths of a second

         if (mode == 2) col = 11;
         if (mode == 3) col = 10;
         if (timer_draw_mode1) al_draw_textf(mF.pixl,   mC.pc[col], x0+10, y0+4, ALLEGRO_ALIGN_CENTER, "%d", tts);
         if (timer_draw_mode2) al_draw_textf(mF.pr8, mC.pc[col], x0+10, y0+6, ALLEGRO_ALIGN_CENTER, "%d", tts);

      }
      if ((timer_draw_mode3) || (timer_draw_mode4)) // percent bar
      {
         int percent = 0;
         if (item[i][12] > 0) // prevent divide by zero
         {
            if (mode == 2) percent =       ((item[i][13]) * 100) / item[i][12];
            if (mode == 3) percent = 100 - ((item[i][13]) * 100) / item[i][12];
         }
         if (timer_draw_mode3) draw_percent_bar(x0+9, y0+5, 32, 8,  percent);
         if (timer_draw_mode4) draw_percent_bar(x0+9, y0+1, 64, 16, percent);

      }
   }
   if (mode == 4)
   {
      int tt = item[i][12]; // total time
      int ct = item[i][13]; // current time
      int st = item[i][14]; // switch time

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


      if (timer_draw_mode1) al_draw_textf(mF.pixl,   mC.pc[col], x0+10, y0+4, ALLEGRO_ALIGN_CENTER, "%d", tts);
      if (timer_draw_mode2) al_draw_textf(mF.pr8, mC.pc[col], x0+10, y0+6, ALLEGRO_ALIGN_CENTER, "%d", tts);
      if (timer_draw_mode3) draw_percent_bar(x0+9, y0+5, 32, 8,  percent);
      if (timer_draw_mode4) draw_percent_bar(x0+9, y0+1, 64, 16, percent);
   }
}


void proc_block_damage(int i)
{
   int et = item[i][1];      // number of pm_event trigger we are looking for
   int FLAGS = item[i][3];
   int mode = item[i][11];


   if (FLAGS & PM_ITEM_DAMAGE_LIFT_ON) set_item_damage_location_from_lift(i, 0); // follow lift location

   proc_item_damage_collisions(i);

   if (mode == 0) item[i][3] |= PM_ITEM_DAMAGE_CURR; // in mode 0, always set damage flag

   if (mode == 1)
   {
      if (mwPME.event[et]) item[i][3] ^= PM_ITEM_DAMAGE_CURR; // toggle current damage flag
   }
   if (mode == 2) // damage unless timer running  (no damage when triggered)
   {
      if (mwPME.event[et]) item[i][13] = item[i][12];              // reset timer
      if (item[i][13] == 0) item[i][3] |=  PM_ITEM_DAMAGE_CURR; // set damage on
      else                  item[i][3] &= ~PM_ITEM_DAMAGE_CURR; // set damage off
   }
   if (mode == 3) // damage when timer is running (no damage until triggered)
   {
      if (mwPME.event[et]) item[i][13] = item[i][12];             // reset timer
      if (item[i][13] > 0) item[i][3] |=  PM_ITEM_DAMAGE_CURR; // set damage on
      else                 item[i][3] &= ~PM_ITEM_DAMAGE_CURR; // set damage off
   }
   if (mode == 4) // timed on and off
   {
      // timer will run outside this function always, but in this mode, when it gets to zero, we will reset it
      if (item[i][13] == 0) item[i][13] = item[i][12];  // reset timer
      if (item[i][13] < item[i][14]) item[i][3] |=  PM_ITEM_DAMAGE_CURR; // set damage on
      else                           item[i][3] &= ~PM_ITEM_DAMAGE_CURR; // set damage off
   }
   if (--item[i][13] < 0) item[i][13] = 0; // always run timer
}










