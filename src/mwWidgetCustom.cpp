
// mwWidget.cpp

#include "pm.h"
#include "mwWidget.h"
#include "mwBitmapTools.h"
#include "mwFont.h"
#include "mwBitmap.h"
#include "mwLift.h"
#include "mwColor.h"
#include "mwInput.h"
#include "mwEventQueue.h"
#include "mwItem.h"
#include "mwEnemy.h"
#include "mwHelp.h"



bool mwWidget::mButtonCustom(int xType, int xa, int xb, int yType, int ya, int yb, int r, int backgroundType, int frameType, int textType, int bcol, int fcol, int tcol, int hcol, int highlight, int type, int &v1, int v2, int v3, int disable_input)
{
   int x1, y1, x2, y2;
   xyHelper(xType, xa, xb, yType, ya, yb, "", x1, y1, x2, y2);

   // check if mouse is on button
   bool mouseOnButton = false;
   if ((!disable_input) && (mInput.mouse_x > x1) && (mInput.mouse_x < x2) && (mInput.mouse_y > y1) && (mInput.mouse_y < y2)) mouseOnButton = true;

   // if (backgroundType == 0) ; do nothing
   if (backgroundType == 1) al_draw_filled_rounded_rectangle(x1, y1, x2, y2, r, r, mColor.pc[bcol]); // solid color
   if (backgroundType == 2) draw_widget_area(x1, y1, x2, y2, bcol); // draw button frame

   // 0 = no frame
   // 1 = static
   // 2 = highlight with var
   // 3 = highlight with mouse
   // 4 = highlight with both
   if (frameType)
   {
      int c = fcol;
      if (highlight     && ((frameType == 2) || (frameType == 4))) c = hcol;
      if (mouseOnButton && ((frameType == 3) || (frameType == 4))) c = hcol;
      if (c != -1) al_draw_rounded_rectangle(x1, y1, x2, y2, r, r, mColor.pc[c], 1);
   }
   bool pressed = false;
   if (mouseOnButton && (mInput.mouse_b[1][0]))
   {
      while (mInput.mouse_b[1][0]) mEventQueue.proc(1); // wait for release
      pressed = true;
   }

   char msg[256];
   sprintf(msg, "%s", "");

   if (type == 13)
   {
      sprintf(msg, "Main Shape");
      float rot = mEnemy.Ef[v1][14];
      al_draw_rotated_bitmap(mBitmap.sprite[mBitmap.zz[0][mEnemy.Ei[v1][5]]], 10, 10, (x2+x1)/2+60, (y2+y1)/2, rot, 0);

      if (pressed)
      {
         int ans = mEnemy.Ei[v1][5];
         if (ans == 31) ans = 14;
         else
         {
            if (ans == 29) ans = 31;
            if (ans == 14) ans = 29;
         }
         mEnemy.Ei[v1][5] = ans;
         mEnemy.Ei[v1][3] = ans;
         mEnemy.Ei[v1][1] = mBitmap.zz[5][ans];
      }
   }

   if (type == 14)
   {
      sprintf(msg, "Seek Shape");
      float rot = mEnemy.Ef[v1][14];
      al_draw_rotated_bitmap(mBitmap.sprite[mBitmap.zz[0][mEnemy.Ei[v1][6]]], 10, 10, (x2+x1)/2+60, (y2+y1)/2, rot, 0);

      if (pressed)
      {
         int ans = mEnemy.Ei[v1][6];
         if (ans == 31) ans = 14;
         else
         {
            if (ans == 29) ans = 31;
            if (ans == 14) ans = 29;
         }
         mEnemy.Ei[v1][6] = ans;
      }
   }

   if (type == 310)
   {

      int tn = mItem.item[v1][v2] & PM_BTILE_TILENUM_MASK;
      sprintf(msg, "Block %d: %-4d", v3, tn);

      int tx1 = (x2+x1)/2+60;
      int ty1 = (y2+y1)/2-10;

      al_draw_filled_rectangle(tx1-1, ty1-1, tx1+22, ty1+22, mColor.Black);

      al_draw_bitmap(mBitmap.tile[tn], tx1, ty1, 0);

      if (pressed) mBitmapTools.select_bitmap_from_level(mItem.item[v1][v2]);

      if (mouseOnButton)
      {
         int mpow_jnk = 0;
         mBitmapTools.draw_flags(x2+6, y1, mItem.item[v1][v2], mpow_jnk, 0, 1, 1);
         while (mInput.CTRL())
         {
            mEventQueue.proc(1);
            al_flip_display();
            mBitmapTools.draw_flags(x2+6, y1, mItem.item[v1][v2], mpow_jnk, 0, 1, 1);
         }
      }
   }


   if (type == 318)
   {
      int tn = mEnemy.Ei[v1][13] & PM_BTILE_TILENUM_MASK;
      sprintf(msg, "Block:%d", tn);

      int tx1 = (x2+x1)/2+60;
      int ty1 = (y2+y1)/2-10;

      al_draw_filled_rectangle(tx1-1, ty1-1, tx1+22, ty1+22, mColor.Black);

      al_draw_bitmap(mBitmap.tile[tn], tx1, ty1, 0);

      if (pressed) mBitmapTools.select_bitmap_from_level(mEnemy.Ei[v1][13]);

      if (mouseOnButton)
      {
         int mpow_jnk = 0;
         mBitmapTools.draw_flags(x2+6, y1, mEnemy.Ei[v1][13], mpow_jnk, 0, 1, 1);
         while (mInput.CTRL())
         {
            mEventQueue.proc(1);
            al_flip_display();
            mBitmapTools.draw_flags(x2+6, y1, mEnemy.Ei[v1][13], mpow_jnk, 0, 1, 1);
         }
      }
   }


   if (type == 320)
   {
      int tn = mLift.cur[v1].draw_mode_val1 & PM_BTILE_TILENUM_MASK;
      sprintf(msg, "Block:%d", tn);
      int tx1 = (x2+x1)/2+60;
      int ty1 = (y2+y1)/2-10;
      al_draw_filled_rectangle(tx1-1, ty1-1, tx1+22, ty1+22, mColor.Black);
      al_draw_bitmap(mBitmap.tile[tn], tx1, ty1, 0);
      if (pressed) mBitmapTools.select_bitmap_from_level(mLift.cur[v1].draw_mode_val1);
   }

   if (type == 321)
   {
      int tn = mLift.cur[v1].draw_mode_val1 & PM_BTILE_TILENUM_MASK;
      sprintf(msg, "Start Block:%d", tn);
      int tx1 = (x2+x1)/2+80;
      int ty1 = (y2+y1)/2-10;
      al_draw_filled_rectangle(tx1-1, ty1-1, tx1+61, ty1+21, mColor.pc[0]);
      al_draw_bitmap(mBitmap.tile[tn+0], tx1+0,  ty1, 0);
      al_draw_bitmap(mBitmap.tile[tn+1], tx1+20, ty1, 0);
      al_draw_bitmap(mBitmap.tile[tn+2], tx1+40, ty1, 0);
      if (pressed) mBitmapTools.select_bitmap_from_level(mLift.cur[v1].draw_mode_val1);
   }


   if (type == 94) // orb rotation
   {
      int rb = (mItem.item[v1][2] & PM_ITEM_ORB_ROTB) >> 14;
      if (pressed) rb++;
      if ((rb < 0) || (rb > 3)) rb = 0;
      sprintf(msg, "Change Rotation");
      // set rb
      rb = rb << 14; // shift bits into place
      mItem.item[v1][2] &= ~PM_ITEM_ORB_ROTB; // clear bits in target
      mItem.item[v1][2] |= rb; // merge
   }


   if (type == 96) // block damage draw rotation
   {
      // get rb
      int rb = (mItem.item[v1][3] & PM_ITEM_DAMAGE_ROTB) >> 14;

      if (pressed) rb++;
      if ((rb < 0) || (rb > 3)) rb = 0;

      sprintf(msg, "Draw Rotation:%d", rb);

      // set rb
      rb = rb << 14; // shift bits into place
      mItem.item[v1][3] &= ~PM_ITEM_DAMAGE_ROTB; // clear bits in target
      mItem.item[v1][3] |= rb; // merge
   }

   if (type == 92) // orb mode
   {
      if (pressed) mItem.item[v1][6]++;
      if ((mItem.item[v1][6] < 0) || (mItem.item[v1][6] > 4)) mItem.item[v1][6] = 0;
      sprintf(msg, "undef");
      if (mItem.item[v1][6] == 0) sprintf(msg, "Mode:Toggle");
      if (mItem.item[v1][6] == 1)
      {
         sprintf(msg, "Mode:Stick ON");
         mItem.item[v1][2] &= ~PM_ITEM_ORB_STATE;
      }
      if (mItem.item[v1][6] == 2)
      {
         sprintf(msg, "Mode:Stick OFF");
         mItem.item[v1][2] |= PM_ITEM_ORB_STATE;
      }
      if (mItem.item[v1][6] == 3)
      {
         sprintf(msg, "Mode:Timed ON");
         mItem.item[v1][2] &= ~PM_ITEM_ORB_STATE;
      }
      if (mItem.item[v1][6] == 4)
      {
         sprintf(msg, "Mode:Timed OFF");
         mItem.item[v1][2] |= PM_ITEM_ORB_STATE;
      }
   }

   if (type == 90) // orb trigger type
   {
      if (pressed)
      {
         if (mItem.item[v1][2] & PM_ITEM_ORB_TRIG_TOUCH)
         {
            mItem.item[v1][2] &= ~PM_ITEM_ORB_TRIG_TOUCH; // clear flag
            mItem.item[v1][2] |= PM_ITEM_ORB_TRIG_UP; // set flag
         }
         else if (mItem.item[v1][2] & PM_ITEM_ORB_TRIG_UP)
         {
            mItem.item[v1][2] &= ~PM_ITEM_ORB_TRIG_UP; // clear flag
            mItem.item[v1][2] |= PM_ITEM_ORB_TRIG_DOWN; // set flag
         }
         else if (mItem.item[v1][2] & PM_ITEM_ORB_TRIG_DOWN)
         {
            mItem.item[v1][2] &= ~PM_ITEM_ORB_TRIG_DOWN;  // clear flag
            mItem.item[v1][2] |= PM_ITEM_ORB_TRIG_SHOT; // set flag
         }
         else if (mItem.item[v1][2] & PM_ITEM_ORB_TRIG_SHOT)
         {
            mItem.item[v1][2] &= ~PM_ITEM_ORB_TRIG_SHOT; // clear flag
            mItem.item[v1][2] |= PM_ITEM_ORB_TRIG_TOUCH; // set flag
         }
      }

      sprintf(msg, "undef");

      if (mItem.item[v1][2] & PM_ITEM_ORB_TRIG_TOUCH) sprintf(msg, "Trigger:Touch");
      if (mItem.item[v1][2] & PM_ITEM_ORB_TRIG_UP)    sprintf(msg, "Trigger:Up");
      if (mItem.item[v1][2] & PM_ITEM_ORB_TRIG_DOWN)  sprintf(msg, "Trigger:Down");
      if (mItem.item[v1][2] & PM_ITEM_ORB_TRIG_SHOT)  sprintf(msg, "Trigger:Shot");


   }


   if (type == 211) // Trigger Field X Lift Alignment
   {
      int C = mItem.item[v1][3] & PM_ITEM_TRIGGER_LIFT_XC;
      int F = mItem.item[v1][3] & PM_ITEM_TRIGGER_LIFT_XF;
      int L = mItem.item[v1][3] & PM_ITEM_TRIGGER_LIFT_XL;
      if (C) sprintf(msg, "Lift X Align:Center");
      else
      {
         if ((!F) && (!L)) sprintf(msg, "Lift X Align: Field x1 = Lift x1");
         if ((!F) &&  (L)) sprintf(msg, "Lift X Align: Field x1 = Lift x2");
         if ((F)  && (!L)) sprintf(msg, "Lift X Align: Field x2 = Lift x1");
         if ((F)  &&  (L)) sprintf(msg, "Lift X Align: Field x2 = Lift x2");
      }
      if (pressed)
      {
         int C = mItem.item[v1][3] & PM_ITEM_TRIGGER_LIFT_XC;
         int F = mItem.item[v1][3] & PM_ITEM_TRIGGER_LIFT_XF;
         int L = mItem.item[v1][3] & PM_ITEM_TRIGGER_LIFT_XL;
         if (C)    // 1 X X
         {  // set to 0 0 0
            mItem.item[v1][3] &= ~PM_ITEM_TRIGGER_LIFT_XC; // clear C flag
            mItem.item[v1][3] &= ~PM_ITEM_TRIGGER_LIFT_XF; // clear F flag
            mItem.item[v1][3] &= ~PM_ITEM_TRIGGER_LIFT_XL; // clear L flag
         }
         else if ((!F) && (!L)) // 0 0 0
         {               // set to 0 0 1
            mItem.item[v1][3] &= ~PM_ITEM_TRIGGER_LIFT_XC; // clear C flag
            mItem.item[v1][3] &= ~PM_ITEM_TRIGGER_LIFT_XF; // clear F flag
            mItem.item[v1][3] |=  PM_ITEM_TRIGGER_LIFT_XL; // set   L flag
         }
         else if ((!F) && (L)) // 0 0 1
         {              // set to 0 1 0
            mItem.item[v1][3] |=  PM_ITEM_TRIGGER_LIFT_XF; // set   F flag
            mItem.item[v1][3] &= ~PM_ITEM_TRIGGER_LIFT_XL; // clear L flag
         }
         else if ((F) && (!L)) // 0 1 0
         {              // set to 0 1 1
            mItem.item[v1][3] |=  PM_ITEM_TRIGGER_LIFT_XF; // set   F flag
            mItem.item[v1][3] |=  PM_ITEM_TRIGGER_LIFT_XL; // set   L flag
         }
         else if ((F) && (L))  // 0 1 1
         {              // set to 1 0 0
            mItem.item[v1][3] |=  PM_ITEM_TRIGGER_LIFT_XC; // set   C flag
            mItem.item[v1][3] &= ~PM_ITEM_TRIGGER_LIFT_XL; // clear L flag
            mItem.item[v1][3] &= ~PM_ITEM_TRIGGER_LIFT_XF; // clear F flag
         }
      }
   }


   if (type == 212) // Trigger Field Y Lift Alignment
   {
      int C = mItem.item[v1][3] & PM_ITEM_TRIGGER_LIFT_YC;
      int F = mItem.item[v1][3] & PM_ITEM_TRIGGER_LIFT_YF;
      int L = mItem.item[v1][3] & PM_ITEM_TRIGGER_LIFT_YL;
      if (C) sprintf(msg, "Lift Y Align:Center");
      else
      {
         if ((!F) && (!L)) sprintf(msg, "Lift Y Align: Field y1 = Lift y1");
         if ((!F) &&  (L)) sprintf(msg, "Lift Y Align: Field y1 = Lift y2");
         if ((F)  && (!L)) sprintf(msg, "Lift Y Align: Field y2 = Lift y1");
         if ((F)  &&  (L)) sprintf(msg, "Lift Y Align: Field y2 = Lift y2");
      }
      if (pressed)
      {
         int C = mItem.item[v1][3] & PM_ITEM_TRIGGER_LIFT_YC;
         int F = mItem.item[v1][3] & PM_ITEM_TRIGGER_LIFT_YF;
         int L = mItem.item[v1][3] & PM_ITEM_TRIGGER_LIFT_YL;

         if (C)    // 1 X X
         {  // set to 0 0 0
            mItem.item[v1][3] &= ~PM_ITEM_TRIGGER_LIFT_YC; // clear C flag
            mItem.item[v1][3] &= ~PM_ITEM_TRIGGER_LIFT_YF; // clear F flag
            mItem.item[v1][3] &= ~PM_ITEM_TRIGGER_LIFT_YL; // clear L flag
         }
         else if ((!F) && (!L)) // 0 0 0
         {               // set to 0 0 1
            mItem.item[v1][3] &= ~PM_ITEM_TRIGGER_LIFT_YC; // clear C flag
            mItem.item[v1][3] &= ~PM_ITEM_TRIGGER_LIFT_YF; // clear F flag
            mItem.item[v1][3] |=  PM_ITEM_TRIGGER_LIFT_YL; // set   L flag
         }
         else if ((!F) && (L)) // 0 0 1
         {              // set to 0 1 0
            mItem.item[v1][3] |=  PM_ITEM_TRIGGER_LIFT_YF; // set   F flag
            mItem.item[v1][3] &= ~PM_ITEM_TRIGGER_LIFT_YL; // clear L flag
         }
         else if ((F) && (!L)) // 0 1 0
         {              // set to 0 1 1
            mItem.item[v1][3] |=  PM_ITEM_TRIGGER_LIFT_YF; // set   F flag
            mItem.item[v1][3] |=  PM_ITEM_TRIGGER_LIFT_YL; // set   L flag
         }
         else if ((F) && (L))  // 0 1 1
         {              // set to 1 0 0
            mItem.item[v1][3] |=  PM_ITEM_TRIGGER_LIFT_YC; // set   C flag
            mItem.item[v1][3] &= ~PM_ITEM_TRIGGER_LIFT_YL; // clear L flag
            mItem.item[v1][3] &= ~PM_ITEM_TRIGGER_LIFT_YF; // clear F flag
         }
      }
   }

   
   
   


   if (type == 411) // DAMAGE Field X Lift Alignment
   {
      int C = mItem.item[v1][3] & PM_ITEM_DAMAGE_LIFT_XC;
      int F = mItem.item[v1][3] & PM_ITEM_DAMAGE_LIFT_XF;
      int L = mItem.item[v1][3] & PM_ITEM_DAMAGE_LIFT_XL;

      if (C) sprintf(msg, "Lift X Align:Center");
      else
      {
         if ((!F) && (!L)) sprintf(msg, "Lift X Align: Field x1 = Lift x1");
         if ((!F) &&  (L)) sprintf(msg, "Lift X Align: Field x1 = Lift x2");
         if ((F)  && (!L)) sprintf(msg, "Lift X Align: Field x2 = Lift x1");
         if ((F)  &&  (L)) sprintf(msg, "Lift X Align: Field x2 = Lift x2");
      }
      if (pressed)
      {
         int C = mItem.item[v1][3] & PM_ITEM_DAMAGE_LIFT_XC;
         int F = mItem.item[v1][3] & PM_ITEM_DAMAGE_LIFT_XF;
         int L = mItem.item[v1][3] & PM_ITEM_DAMAGE_LIFT_XL;


         if (C)    // 1 X X
         {  // set to 0 0 0
            mItem.item[v1][3] &= ~PM_ITEM_DAMAGE_LIFT_XC; // clear C flag
            mItem.item[v1][3] &= ~PM_ITEM_DAMAGE_LIFT_XF; // clear F flag
            mItem.item[v1][3] &= ~PM_ITEM_DAMAGE_LIFT_XL; // clear L flag
         }
         else if ((!F) && (!L)) // 0 0 0
         {               // set to 0 0 1
            mItem.item[v1][3] &= ~PM_ITEM_DAMAGE_LIFT_XC; // clear C flag
            mItem.item[v1][3] &= ~PM_ITEM_DAMAGE_LIFT_XF; // clear F flag
            mItem.item[v1][3] |=  PM_ITEM_DAMAGE_LIFT_XL; // set   L flag
         }
         else if ((!F) && (L)) // 0 0 1
         {              // set to 0 1 0
            mItem.item[v1][3] |=  PM_ITEM_DAMAGE_LIFT_XF; // set   F flag
            mItem.item[v1][3] &= ~PM_ITEM_DAMAGE_LIFT_XL; // clear L flag
         }
         else if ((F) && (!L)) // 0 1 0
         {              // set to 0 1 1
            mItem.item[v1][3] |=  PM_ITEM_DAMAGE_LIFT_XF; // set   F flag
            mItem.item[v1][3] |=  PM_ITEM_DAMAGE_LIFT_XL; // set   L flag
         }
         else if ((F) && (L))  // 0 1 1
         {              // set to 1 0 0
            mItem.item[v1][3] |=  PM_ITEM_DAMAGE_LIFT_XC; // set   C flag
            mItem.item[v1][3] &= ~PM_ITEM_DAMAGE_LIFT_XL; // clear L flag
            mItem.item[v1][3] &= ~PM_ITEM_DAMAGE_LIFT_XF; // clear F flag
         }
      }
   }
   if (type == 412) // DAMAGE Field Y Lift Alignment
   {
      int C = mItem.item[v1][3] & PM_ITEM_DAMAGE_LIFT_YC;
      int F = mItem.item[v1][3] & PM_ITEM_DAMAGE_LIFT_YF;
      int L = mItem.item[v1][3] & PM_ITEM_DAMAGE_LIFT_YL;

      if (C) sprintf(msg, "Lift Y Align:Center");
      else
      {
         if ((!F) && (!L)) sprintf(msg, "Lift Y Align: Field y1 = Lift y1");
         if ((!F) &&  (L)) sprintf(msg, "Lift Y Align: Field y1 = Lift y2");
         if ((F)  && (!L)) sprintf(msg, "Lift Y Align: Field y2 = Lift y1");
         if ((F)  &&  (L)) sprintf(msg, "Lift Y Align: Field y2 = Lift y2");
      }
      if (pressed)
      {
         int C = mItem.item[v1][3] & PM_ITEM_DAMAGE_LIFT_YC;
         int F = mItem.item[v1][3] & PM_ITEM_DAMAGE_LIFT_YF;
         int L = mItem.item[v1][3] & PM_ITEM_DAMAGE_LIFT_YL;

         if (C)    // 1 X X
         {  // set to 0 0 0
            mItem.item[v1][3] &= ~PM_ITEM_DAMAGE_LIFT_YC; // clear C flag
            mItem.item[v1][3] &= ~PM_ITEM_DAMAGE_LIFT_YF; // clear F flag
            mItem.item[v1][3] &= ~PM_ITEM_DAMAGE_LIFT_YL; // clear L flag
         }
         else if ((!F) && (!L)) // 0 0 0
         {               // set to 0 0 1
            mItem.item[v1][3] &= ~PM_ITEM_DAMAGE_LIFT_YC; // clear C flag
            mItem.item[v1][3] &= ~PM_ITEM_DAMAGE_LIFT_YF; // clear F flag
            mItem.item[v1][3] |=  PM_ITEM_DAMAGE_LIFT_YL; // set   L flag
         }
         else if ((!F) && (L)) // 0 0 1
         {              // set to 0 1 0
            mItem.item[v1][3] |=  PM_ITEM_DAMAGE_LIFT_YF; // set   F flag
            mItem.item[v1][3] &= ~PM_ITEM_DAMAGE_LIFT_YL; // clear L flag
         }
         else if ((F) && (!L)) // 0 1 0
         {              // set to 0 1 1
            mItem.item[v1][3] |=  PM_ITEM_DAMAGE_LIFT_YF; // set   F flag
            mItem.item[v1][3] |=  PM_ITEM_DAMAGE_LIFT_YL; // set   L flag
         }
         else if ((F) && (L))  // 0 1 1
         {              // set to 1 0 0
            mItem.item[v1][3] |=  PM_ITEM_DAMAGE_LIFT_YC; // set   C flag
            mItem.item[v1][3] &= ~PM_ITEM_DAMAGE_LIFT_YL; // clear L flag
            mItem.item[v1][3] &= ~PM_ITEM_DAMAGE_LIFT_YF; // clear F flag
         }
      }
   }


   
   if (type == 57)
   {
      int n = v1;
      int o = v2;

      int t = 0;
      sprintf(msg,"?? Help");

      if (o == 2)
      {
         t = mItem.item[n][0];
         sprintf(msg,"%s Help", mItem.item_name[t]);
      }
      if (o == 3)
      {
         t = mEnemy.Ei[n][0];
         sprintf(msg,"%s Help", (const char *)mEnemy.enemy_name[t][0]);
      }
      if (o == 4) sprintf(msg,"Lift Help");

      if (pressed)
      {
         if (o==2)
         {
            if (t == 1)  mHelp.help("Door Viewer");
            if (t == 2)  mHelp.help("Bonus Viewer");
            if (t == 3)  mHelp.help("Exit Viewer");
            if (t == 4)  mHelp.help("Key Viewer");
            if (t == 5)  mHelp.help("Start Viewer");
            if (t == 6)  mHelp.help("Orb Viewer");
            if (t == 7)  mHelp.help("Mine Viewer");
            if (t == 8)  mHelp.help("Bomb Viewer");
            if (t == 9)  mHelp.help("Trigger Viewer");
            if (t == 10) mHelp.help("Message Viewer");
            if (t == 11) mHelp.help("Rocket Viewer");
            if (t == 12) mHelp.help("Warp Viewer");
            if (t == 13) mHelp.help("Timer Viewer");
            if (t == 14) mHelp.help("Switch Viewer");
            if (t == 15) mHelp.help("Sproingy Viewer");
            if (t == 16) mHelp.help("Block Manip Viewer");
            if (t == 17) mHelp.help("Block Damage Viewer");
         }
         if (o==3)
         {
            if (t == 1) mHelp.help("Bouncer Viewer");
            if (t == 2) mHelp.help("Cannon Viewer");
            if (t == 3) mHelp.help("Archwagon Viewer");
            if (t == 4) mHelp.help("BlokWalk Viewer");
            if (t == 5) mHelp.help("Jumpworm Viewer");
            if (t == 6) mHelp.help("Flapper Viewer");
            if (t == 7) mHelp.help("Vinepod Viewer");
            if (t == 8) mHelp.help("Trakbot Viewer");
            if (t == 9) mHelp.help("Cloner Viewer");
         }
         if (o==4)
         {
            mHelp.help("Lift Viewer");
         }
      }
   }



   if (type == 1010)
   {
      if (pressed) v1++;
      if ((v1 < 0) || (v1 > 3)) v1 = 0;
      if (v1 == 0) sprintf(msg,  "Player Display: Icon Only");
      if (v1 == 1) sprintf(msg,  "Player Display: Text Only");
      if (v1 == 2) sprintf(msg,  "Player Display: Icon + Text");
      if (v1 == 3) sprintf(msg,  "Player Display: Text + Icon");
   }

   if (type == 1011)
   {
      if (pressed) v1++;
      if ((v1 < 0) || (v1 > 3)) v1 = 0;
      if (v1 == 0) sprintf(msg,  "Enemy Display: Icon Only");
      if (v1 == 1) sprintf(msg,  "Enemy Display: Text Only");
      if (v1 == 2) sprintf(msg,  "Enemy Display: Icon + Text");
      if (v1 == 3) sprintf(msg,  "Enemy Display: Text + Icon");
   }

   if (type == 1012)
   {
      if (pressed) v1++;
      if ((v1 < 0) || (v1 > 3)) v1 = 0;
      if (v1 == 0) sprintf(msg,  "Item Display: Icon Only");
      if (v1 == 1) sprintf(msg,  "Item Display: Text Only");
      if (v1 == 2) sprintf(msg,  "Item Display: Icon + Text");
      if (v1 == 3) sprintf(msg,  "Item Display: Text + Icon");
   }

   if (type == 1013)
   {
      if (pressed) v1++;
      if ((v1 < 0) || (v1 > 3)) v1 = 0;
      if (v1 == 0) sprintf(msg,  "Health Display: Long Text");
      if (v1 == 1) sprintf(msg,  "Health Display: Short Text");
      if (v1 == 2) sprintf(msg,  "Health Display: Value Only");
      if (v1 == 3) sprintf(msg,  "Health Display: Percent Bar");
   }

   if (type == 1014)
   {
      if (pressed) v1++;
      if ((v1 < 0) || (v1 > 1)) v1 = 0;
      if (v1 == 0) sprintf(msg,  "Player Display: Short Text");
      if (v1 == 1) sprintf(msg,  "Player Display: Long Text");
   }

   
   if (type == 1017)
   {
      if (pressed) v1++;
      if ((v1 < 0) || (v1 > 2)) v1 = 0;
      if (v1 == 0) sprintf(msg,  "Never");
      if (v1 == 1) sprintf(msg,  "Remote Players Only");
      if (v1 == 2) sprintf(msg,  "Always");
   }


   if (type == 1020)
   {
      if (pressed) v1++;
      if ((v1 < 0) || (v1 > 1)) v1 = 0;
      if (v1 == 0) sprintf(msg,  "Centered Mode");
      if (v1 == 1) sprintf(msg,  "Hysteresis Mode");
   }
   
   
   if (type == 1021)
   {
      if (pressed) v1++;
      if ((v1 < 0) || (v1 > 1)) v1 = 0;
      if (v1 == 0) sprintf(msg,  "Stationary");
      if (v1 == 1) sprintf(msg,  "Fall");
   }
   if (type == 1022)
   {
      if (pressed) v1++;
      if ((v1 < -2) || (v1 > 1)) v1 = -2;
      if (v1 ==  1) sprintf(msg, "Fall");
      if (v1 ==  0) sprintf(msg, "Stationary");
      if (v1 == -1) sprintf(msg, "Carry");
      if (v1 == -2) sprintf(msg, "Carry Through Door");
   }
   if (type == 1023) // rocket only
   {
      if (pressed) v1++;
      if ((v1 < -2) || (v1 > 1)) v1 = -2;
      if (v1 == -1) v1 = 0;
      if (v1 ==  1) sprintf(msg,  "Fall");
      if (v1 ==  0) sprintf(msg,  "Stationary");
      if (v1 == -2) sprintf(msg,  "Ride Through Door");
   }

   if (type == 1600)
   {
      if (pressed) v1++;
      if ((v1 < 1) || (v1 > 3)) v1 = 1;

      if (v1 == 1) sprintf(msg, "Both");
      if (v1 == 2) sprintf(msg, "Tile");
      if (v1 == 3) sprintf(msg, "Flag");

   }

   if (type == 1701)
   {
      if (pressed) v1++;
      if ((v1 < 0) || (v1 > 2)) v1 = 0;
      if (v1 == 0) sprintf(msg,  "X Centered");
      if (v1 == 1) sprintf(msg,  "X Left Justified");
      if (v1 == 2) sprintf(msg,  "X Right Justified");
   }

   if (type == 1702)
   {
      if (pressed) v1++;
      if ((v1 < 0) || (v1 > 2)) v1 = 0;
      if (v1 == 0) sprintf(msg,  "Y Centered");
      if (v1 == 1) sprintf(msg,  "Y Top Justified");
      if (v1 == 2) sprintf(msg,  "Y Bottom Justified");
   }


   if (type == 1081)
   {
      if (pressed) v1++;
      if ((v1 < 0) || (v1 > 3)) v1 = 0;
      if (v1 == 0) sprintf(msg, "Draw Boxes:None");
      if (v1 == 1) sprintf(msg, "Draw Boxes:Source Only");
      if (v1 == 2) sprintf(msg, "Draw Boxes:Destination Only");
      if (v1 == 3) sprintf(msg, "Draw Boxes:Both");
   }

   if (type == 1082)
   {
      if (pressed) v1++;
      if ((v1 < 0) || (v1 > 3)) v1 = 0;
      if (v1 == 0) sprintf(msg, "Draw Mode:Hidden");
      if (v1 == 1) sprintf(msg, "Draw Mode:Static Shape");
      if (v1 == 2) sprintf(msg, "Draw Mode:Static Animation");
      if (v1 == 3) sprintf(msg, "Draw Mode:Follow Event Timer");
   }

   
   if (type == 1083)
   {
      if (pressed) v1++;
      if ((v1 < 0) || (v1 > 1)) v1 = 0;
      if (v1 == 0) sprintf(msg, "Draw Mode:Hidden");
      if (v1 == 1) sprintf(msg, "Draw Mode:Progress Bar");
   }


   if (type == 1085)
   {
      if (pressed) v1++;
      if ((v1 < 0) || (v1 > 1)) v1 = 0;
      if (v1 == 0) sprintf(msg, "Output Mode:One Time");
      if (v1 == 1) sprintf(msg, "Output Mode:Continuous");
   }

   
   if (type == 1101)
   {
      if (pressed) v1++;
      if ((v1 < 1) || (v1 > 3)) v1 = 1;
      if (v1 == 2) v1 = 3; // no free man anymore, skip to the next
      if (v1 == 1) sprintf(msg, "Type: Health Bonus");
      if (v1 == 3) sprintf(msg, "Type: Purple Coin");
   }


   if (type == 1050) // door entry type
   {
      if (pressed) v1++;
      if ((v1 < 0) || (v1 > 2)) v1 = 0;
      if (v1 == 0) sprintf(msg, "Enter Immediate  ");
      if (v1 == 1) sprintf(msg, "Enter with <up>  ");
      if (v1 == 2) sprintf(msg, "Enter with <down>");
   }
   if (type == 1051) // door show dest line type
   {
      if (pressed) v1++;
      if ((v1 < 0) || (v1 > 2)) v1 = 0;
      if (v1 == 0) sprintf(msg, "Exit link:never show  ");
      if (v1 == 1) sprintf(msg, "Exit link:alway show  ");
      if (v1 == 2) sprintf(msg, "Exit link:when touched");
   }

   if (type == 1052)
   {
      if (pressed) v1++;
      if ((v1 < 0) || (v1 > 2)) v1 = 0;
      if (v1 == 0) sprintf(msg, "Draw Type:Hidden");
      if (v1 == 1) sprintf(msg, "Draw Type:Static Door");
      if (v1 == 2) sprintf(msg, "Draw Type:Animated Warp");
   }
   if (type == 1053) // door move type
   {
      if (pressed) v1++;
      if ((v1 < 1) || (v1 > 2)) v1 = 1;
      if (v1 == 1) sprintf(msg, "Move Type:Instant");
      if (v1 == 2) sprintf(msg, "Move Type:Travel ");
   }
   if (type == 1078)
   {
      if (pressed) v1++;
      if ((v1 < 0) || (v1 > 3)) v1 = 0;
      if (v1 == 0) sprintf(msg, "Start Mode:Default");
      if (v1 == 1) sprintf(msg, "Team Start");
      if (v1 == 2) sprintf(msg, "Checkpoint Common");
      if (v1 == 3) sprintf(msg, "Checkpoint Individual");
   }















   // 0 = no text
   // 1 = normal
   // 2 = highlight with var
   // 3 = highlight with mouse
   // 4 = highlight with both

   // text justify
   int tj = 0;
   if (textType > 20)
   {
      tj = 1;
      textType-=20;
   }

   if (textType)
   {
      int c = tcol;
      if (highlight     && ((textType == 2) || (textType == 4))) c = hcol;
      if (mouseOnButton && ((textType == 3) || (textType == 4))) c = hcol;
      draw_widget_text(x1, y1, x2, y2, c, tj, msg);
   }

   return pressed;
}




// wrapper that increments ya by bts
bool mwWidget::mButtonPD(int xType, int xa, int xb, int &ya, int bts,  int r, int backgroundType, int bcol, int fcol, int hcol, int tcol, int text_just,  int type, int &var, int disable_input)
{
   bool ret = mButtonPD(xType, xa, xb, 1, ya, bts-2,  r, backgroundType, bcol, fcol, hcol, tcol, text_just,  type, var, disable_input);
   ya+= bts;
   return ret;
}

// calls mDropDown with predefined list
// returns true if changed
bool mwWidget::mButtonPD(int xType, int xa, int xb, int yType, int ya, int yb,  int r, int backgroundType, int bcol, int fcol, int hcol, int tcol, int text_just,  int type, int &var, int disable_input)
{
   std::vector<struct listItem> listItems;
   bool valid_type = 0;


   if (type == 301) // block manip mode
   {
      valid_type = 1;
      listItems =
      {
         {  0,  "MODE:OFF"  },
         {  1,  "MODE:Set All To Block 1" },
         {  2,  "MODE:Set All Block 2 To Block 1" },
         {  3,  "MODE:Toggle Block 2 To Block 1" },
         {  5,  "MODE:Cycle 3 Blocks" },
         {  6,  "MODE:Cycle 4 Blocks" },
         {  4,  "MODE:Copy Area" }
      };
   }


   if (type == 100)
   {
      valid_type = 1;
      listItems =
      {
         {  0,  "Action:Random from v1 to v2"  },
         {  1,  "Action:Step from v1 to v2" },
         {  2,  "Action:Set all to v1" },
         {  3,  "Action:Set all from 1st obj" }
      };

   }

   if (type == 403) // hider mode
   {
      valid_type = 1;
      listItems =
      {
         {  0,  "MODE:Always Show"              },
         {  1,  "MODE:Always Hide"              },
         {  2,  "MODE:Show Until Triggered"     },
         {  3,  "MODE:Hide Until Triggered"     },
         {  4,  "MODE:Toggle When Triggered"    },
         {  5,  "MODE:Show Only When Triggered" }
      };
   }


   if (type == 160) // timer mode
   {
      valid_type = 1;
      listItems =
      {
         {  0,  "MODE:Free Run"  },
         {  1,  "MODE:Free Run After Trigger" },
         {  2,  "MODE:Run Only When Triggered" },
         {  3,  "MODE:Reset When Not Triggered" },
         {  4,  "MODE:Reset When Triggered" }
      };
   }


   if (type == 102) // key color
   {
      valid_type = 1;
      listItems =
      {
         {  1039,  "Color:Red"    },
         {  1040,  "Color:Green"  },
         {  1041,  "Color:Blue"   },
         {  1042,  "Color:Purple" }
      };

      int col = 0;
      if (var == 1039) col = 10;
      if (var == 1040) col = 11;
      if (var == 1041) col = 13;
      if (var == 1042) col = 8;

      bcol = fcol = hcol = col;

   }

   if (type == 500) // lift mode
   {
      valid_type = 1;
      listItems =
      {
         {  0,  "Mode:Normal"  },
         {  1,  "Mode:Prox Run and Reset" },
         {  2,  "Mode:Prox Reset" }
      };
   }

   if (type == 504) // lift draw mode
   {
      valid_type = 1;
      listItems =
      {
         {  0,   "Draw Mode:Hidden"  },
         {  1,   "Draw Mode:Legacy" },
         {  2,   "Draw Mode:Plain Filled Rect" },
         {  3,   "Draw Mode:Plain Rect" },
         {  10,  "Draw Mode:Single Block" },
         {  11,  "Draw Mode:3 Block Platform" },
         {  12,  "Draw Mode:3 Block Column" }
      };
   }




   if (type == 505) // lift end step mode
   {
      valid_type = 1;
      listItems =
      {
         {  0,   "Loop to Start"  },
         {  1,   "Warp to Start" },
         {  2,   "Freeze Here" }
      };
   }

   if (type == 402) // damage mode
   {
      valid_type = 1;
      listItems =
      {
         {  0,  "MODE:Always ON"  },
         {  1,  "MODE:Toggle" },
         {  2,  "MODE:ON Until Triggered" },
         {  3,  "MODE:OFF Until Triggered" }
      };
   }

   if (type == 404)
   {
      valid_type = 1;
      listItems =
      {
         {  0,  "Draw Type:none"  },
         {  1,  "Draw Type:Red Rectangle" },
         {  2,  "Draw Type:Spikey Floor" },
         {  3,  "Draw Type:Lava" },
         {  4,  "Draw Type:Triangle Spikes" },
         {  5,  "Draw Type:Gold Spikes" },
         {  6,  "Draw Type:Silver White Spikes" },
         {  7,  "Draw Type:Silver Two Height Spikes" },
         {  8,  "Draw Type:Silver Bluish Spikes" },
         {  9,  "Draw Type:Silver Greenish Spikes" },
         {  10, "Draw Type:Mine" },

         {  11, "Draw Type:Wood Pole" },
         {  12, "Draw Type:Silver Cone" },
         {  13, "Draw Type:White Spear" },
         {  14, "Draw Type:Grey Cones" },
         {  15, "Draw Type:Wood Spear" },

         {  16, "Draw Type:Dual Wood Pole" },
         {  17, "Draw Type:Dual Silver Cone" },
         {  18, "Draw Type:Dual White Spear" },
         {  19, "Draw Type:Dual Grey Cones" },
         {  20, "Draw Type:Dual Wood Spear" }

      };
   }

   if (type == 7)
   {
      valid_type = 1;
      listItems =
      {
         {  0,    "Frame Size:0"  },
         {  1,    "Frame Size:1"  },
         {  2,    "Frame Size:2"  },
         {  4,    "Frame Size:4"  },
         {  12,   "Frame Size:12" }
      };
   }

   if (valid_type) return mDropDown(xType, xa, xb, yType, ya, yb,  r, text_just, backgroundType, bcol, fcol, hcol, listItems, var, disable_input);
   return 0;
}















