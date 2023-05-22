// mwWindowObjectViewer.cpp

#include "pm.h"
#include "mwWindow.h"
#include "mwWindowManager.h"
#include "mwFont.h"
#include "mwBitmap.h"
#include "mwLift.h"
#include "mwWidget.h"
#include "mwColor.h"
#include "mwTriggerEvent.h"
#include "mwInput.h"
#include "mwDisplay.h"
#include "mwEventQueue.h"
#include "mwHelp.h"
#include "mwItem.h"
#include "mwEnemy.h"
#include "mwMiscFnx.h"
#include "mwScreen.h"

#include "mwBitmapTools.h"

#define MAP_LOCK_KEY ALLEGRO_KEY_Z




int mwWindow::create_obj(int obt, int type, int num)
{
   if (obt == 2) // items
   {
      int ret = mItem.create_item(type);
      if (ret > 500)
      {
         al_show_native_message_box(mDisplay.display,
               "Error", "No creator exists for the current item type",
               "Copy from an existing item of that type, or get one from the selection window",
                NULL, ALLEGRO_MESSAGEBOX_ERROR);
      }
      else num = ret;
   }
   if (obt == 3)
   {
      if (type == 7)
      {
         int e = mEnemy.create_vinepod();
         if ((e>=0) && (e<99) && (mEnemy.Ei[e][0] == 13))
         {
            object_viewer(3, e);
            num = e;
         }
      }
      if (type == 9)
      {
         int e = mEnemy.create_cloner();
         if ((e>=0) && (e<99) && (mEnemy.Ei[e][0] == 9))
         {
            object_viewer(3, e);
            num = e;
         }
      }
   }
   if (obt == 4) mLift.create_lift();
   return num;  // return number of created obj or sent_num if bad create
}
void mwWindow::ov_get_size(void)
{
   int obt = mWM.mW[7].obt;
   int num = mWM.mW[7].num;
   int type=0, w=300;
   if (obt == 2) type = mItem.item[num][0];
   if (obt == 3) type = mEnemy.Ei[num][0];

   if ((obt == 2) && (type == 1 )) w = 210; // door
   if ((obt == 2) && (type == 2 )) w = 200; // bonus
   if ((obt == 2) && (type == 3 )) w = 200; // exit
   if ((obt == 2) && (type == 4 )) w = 200; // key
   if ((obt == 2) && (type == 5 )) w = 200; // start
   if ((obt == 2) && (type == 6 )) w = 280; // orb
   if ((obt == 2) && (type == 7 )) w = 200; // mine
   if ((obt == 2) && (type == 8 )) w = 200; // bomb
   if ((obt == 2) && (type == 9 )) w = 280; // trigger
   if ((obt == 2) && (type == 10)) w = 220; // message
   if ((obt == 2) && (type == 11)) w = 220; // rocket
   if ((obt == 2) && (type == 12)) w = 220; // warp
   if ((obt == 2) && (type == 13)) w = 260; // timer
   if ((obt == 2) && (type == 14)) w = 200; // switch
   if ((obt == 2) && (type == 15)) w = 240; // sproingy
   if ((obt == 2) && (type == 16)) w = 280; // bm
   if ((obt == 2) && (type == 17)) w = 290; // bd

   if ((obt == 3) && (type == 1 )) w = 220; // bouncer
   if ((obt == 3) && (type == 2 )) w = 220; // cannon
   if ((obt == 3) && (type == 3 )) w = 220; // archwagon
   if ((obt == 3) && (type == 4 )) w = 220; // blokwalk
   if ((obt == 3) && (type == 5 )) w = 220; // jumpworm
   if ((obt == 3) && (type == 6 )) w = 220; // flapper
   if ((obt == 3) && (type == 7 )) w = 220; // vinepod
   if ((obt == 3) && (type == 8 )) w = 220; // trakbot
   if ((obt == 3) && (type == 9 )) w = 240; // cloner

   if (obt == 4)                   w = 300; // lift

   mWM.mW[7].set_size(w, mWM.mW[7].h);
}



void mwWindow::set_switch_tile(int i)
{
   int b = 96; // base
   int nc = 0; // number of colors
   int c[4] = {0};

   if (mItem.item[i][10] & 0b0001) { c[nc] = 9;  nc++;} // green
   if (mItem.item[i][10] & 0b0010) { c[nc] = 10; nc++;} // red
   if (mItem.item[i][10] & 0b0100) { c[nc] = 12; nc++;} // blue
   if (mItem.item[i][10] & 0b1000) { c[nc] = 8;  nc++;} // purple

   // no colors selected
   if (nc == 0) mItem.item[i][1] = b;                     // white

   // one color selected
   if ((nc == 1) && (c[0] == 9))  mItem.item[i][1] = b+1; // green
   if ((nc == 1) && (c[0] == 10)) mItem.item[i][1] = b+2; // red
   if ((nc == 1) && (c[0] == 12)) mItem.item[i][1] = b+3; // blue
   if ((nc == 1) && (c[0] == 8))  mItem.item[i][1] = b+4; // purple


   // two colors selected
   if ((nc == 2) && (c[0] == 9)  && (c[1] == 10)) mItem.item[i][1] = b+5;  // green red
   if ((nc == 2) && (c[0] == 9)  && (c[1] == 12)) mItem.item[i][1] = b+6;  // green blue
   if ((nc == 2) && (c[0] == 9)  && (c[1] == 8))  mItem.item[i][1] = b+7;  // green purple
   if ((nc == 2) && (c[0] == 10) && (c[1] == 12)) mItem.item[i][1] = b+8;  // red blue
   if ((nc == 2) && (c[0] == 10) && (c[1] == 8))  mItem.item[i][1] = b+9;  // red purple
   if ((nc == 2) && (c[0] == 12) && (c[1] == 8))  mItem.item[i][1] = b+10; // blue purple

   // three colors selected
   if ((nc == 3) && (c[0] == 9)  && (c[1] == 10) && (c[2] == 12)) mItem.item[i][1] = b+11;  // green red  blue
   if ((nc == 3) && (c[0] == 9)  && (c[1] == 10) && (c[2] == 8))  mItem.item[i][1] = b+12;  // green red  purple
   if ((nc == 3) && (c[0] == 9)  && (c[1] == 12) && (c[2] == 8))  mItem.item[i][1] = b+13;  // green blue purple
   if ((nc == 3) && (c[0] == 10) && (c[1] == 12) && (c[2] == 8))  mItem.item[i][1] = b+14;  // red   blue purple

   // four colors selected
   if (nc == 4) mItem.item[i][1] = b+15;  // green red blue purple

}


void mwWindow::ov_title(int x1, int x2, int y1, int y2, int legend_highlight)
{
   char msg[1024];
   int obt = mWM.mW[7].obt;
   int num = mWM.mW[7].num;
   int type=0;
   if (obt == 2) type = mItem.item[num][0];
   if (obt == 3) type = mEnemy.Ei[num][0];

//   al_draw_rectangle(x1-1, y1-1, x2+1, y2+1, mColor.pc[10], 1);  // outline entire window

   int xc = (x1+x2)/2;
   int yt = y1+14;

   // legend line text
   char lmsg[6][80];
   for (int x=0; x<6; x++) sprintf(lmsg[x],"%s","");

   // legend line colors
   int legend_color[6];

   // default number of legend lines and colors
   mWM.mW[7].num_legend_lines = 3;
   legend_color[0] = 7;   // legend color
   legend_color[1] = 13;  // location color
   legend_color[2] = 14;  // yellow
   legend_color[3] = 10;  // red
   legend_color[4] = 0;   // unused
   legend_color[5] = 0;   // unused

   legend_highlight == 1 ? legend_color[1] = mColor.flash_color : legend_color[1] = 13;


   // title bar
   msg[0] = 0;
   if (obt == 2) sprintf(msg, "Item Viewer [%d]", num);
   if (obt == 3) sprintf(msg, "Enemy Viewer [%d]", num);
   if (obt == 4) sprintf(msg, "Lift Viewer [%d]", num);

   for (int x=0; x<15; x++)
      al_draw_line(x1, y1+x, x2, y1+x, mColor.pc[13+(x*16)], 1);
   al_draw_text(mFont.pr8, mColor.pc[15], xc, y1+2, ALLEGRO_ALIGN_CENTER,  msg);


   if (obt == 4)  // lifts
   {
      mWM.mW[7].num_legend_lines = 0;
      al_draw_rectangle(xc-94, yt, xc+94, yt+22, mColor.pc[15], 1);
      al_draw_textf(mFont.pr8, mColor.pc[13], xc, yt+8, ALLEGRO_ALIGN_CENTER, "Lift %d of %d",num+1, mLift.get_num_lifts());
   }
   if (obt == 3)  // enemies
   {
      al_draw_rectangle(xc-94, yt, xc+94, yt+22, mColor.pc[15], 1);
      mEnemy.draw_enemy(num, 1, xc-92, yt+1);
      sprintf(msg,"%s %d of %d", (const char *)mEnemy.enemy_name[type][0],1+num - mEnemy.e_first_num[type],mEnemy.e_num_of_type[type]);
      al_draw_text(mFont.pr8, mColor.pc[13], xc-69, yt+8, 0, msg);
      switch (type)
      {
         case 1:
            mWM.mW[7].num_legend_lines = 2;
            sprintf(lmsg[1],"Bouncer Location");
         break;
         case 2: // cannon
            sprintf(lmsg[1],"Cannon Location");
            sprintf(lmsg[2],"Bullet Proximity");
            legend_highlight == 2 ? legend_color[2] = mColor.flash_color : legend_color[2] = 14;
         break;
         case 3: // archwagon
            sprintf(lmsg[1],"ArchWagon Location");
            sprintf(lmsg[2],"Bullet Proximity");
            legend_highlight == 2 ? legend_color[2] = mColor.flash_color : legend_color[2] = 14;
         break;
         case 4:
            mWM.mW[7].num_legend_lines = 2;
            sprintf(lmsg[1],"Block Walker Location");
         break;
         case 5:
            mWM.mW[7].num_legend_lines = 2;
            sprintf(lmsg[1],"JumpWorm Location");
         break;
         case 6: // flapper
            mWM.mW[7].num_legend_lines = 4;
            sprintf(lmsg[1],"Flapper Location");
            sprintf(lmsg[2],"Bullet Proximity");
            sprintf(lmsg[3],"Height Above Player");
            legend_highlight == 2 ? legend_color[2] = mColor.flash_color : legend_color[2] = 14;
            legend_highlight == 3 ? legend_color[3] = mColor.flash_color : legend_color[3] = 10;
         break;
         case 7: // vinepod
         {
            sprintf(lmsg[1],"Vinepod Location");
            sprintf(lmsg[2],"Extended Position");
            legend_highlight == 2 ? legend_color[2] = mColor.flash_color : legend_color[2] = 10;
            int legend_count = 3;
            if (mEnemy.Ei[num][20] & PM_ENEMY_VINEPOD_USE_TRIGGER)
            {
               sprintf(lmsg[legend_count],"Trigger Box");
               legend_highlight == legend_count ? legend_color[legend_count] = mColor.flash_color : legend_color[legend_count] = 14;
               legend_count++;
            }
            if (mEnemy.Ei[num][20] & PM_ENEMY_VINEPOD_USE_SPLINE)
            {
               sprintf(lmsg[legend_count],"Control Point 1");
               legend_highlight == legend_count ? legend_color[legend_count] = mColor.flash_color : legend_color[legend_count] = 6;
               legend_count++;
               sprintf(lmsg[legend_count],"Control Point 2");
               legend_highlight == legend_count ? legend_color[legend_count] = mColor.flash_color : legend_color[legend_count] = 7;
               legend_count++;
            }
            mWM.mW[7].num_legend_lines = legend_count;
         }
         break;
         case 8: // trakbot
            sprintf(lmsg[1],"TrakBot Location");
            sprintf(lmsg[2],"Bullet Proximity");
            legend_highlight == 2 ? legend_color[2] = mColor.flash_color : legend_color[2] = 14;
         break;
         case 9: // cloner
            mWM.mW[7].num_legend_lines = 4;
            sprintf(lmsg[1],"Cloner Location");
            sprintf(lmsg[2],"Source Area");
            sprintf(lmsg[3],"Destination Area");
            legend_highlight == 2 ? legend_color[2] = mColor.flash_color : legend_color[2] = 11;
            legend_highlight == 3 ? legend_color[3] = mColor.flash_color : legend_color[3] = 10;
         break;
      }
   }
   if (obt == 2)  // items
   {
      al_draw_rectangle(xc-94, yt, xc+94, yt+22, mColor.pc[15], 1);
      mItem.draw_item(num, 1, xc-94, yt+1);
      sprintf(msg,"%s %d of %d", mItem.item_name[type], 1+num - mItem.item_first_num[type], mItem.item_num_of_type[type]);
      al_draw_text(mFont.pr8, mColor.pc[13], xc-69, yt+8, 0, msg);
      switch (type)
      {
         case 1: // door
         {
            legend_highlight == 2 ? legend_color[2] = mColor.flash_color : legend_color[2] = 10;
            if (mItem.item[num][8] == 0)  // exit only, no destination
            {
               // find src door(s)
               int num_src = 0;
               for (int i=0; i<500; i++)
                  if ((mItem.item[i][0] == 1) && (mItem.item[i][9] == num))
                  {
                     num_src++;
                  }
               sprintf(lmsg[1],"Exit Door Location");
               if (num_src == 0) sprintf(lmsg[2],"No Source Door");
               if (num_src == 1) sprintf(lmsg[2],"Source Door Position");
               if (num_src >  1) sprintf(lmsg[2],"Source Door Positions");
            }

            if (mItem.item[num][8] == 1)  // draw destination
            {
               sprintf(lmsg[1],"Door Location");
               sprintf(lmsg[2],"Destination");
            }
         }
         break;
         case 2:
             mWM.mW[7].num_legend_lines = 2;
             sprintf(lmsg[1],"Bonus Location");
         break;
         case 3:
             mWM.mW[7].num_legend_lines = 2;
             sprintf(lmsg[1],"Exit Location");
         break;
         case 4: // key
            sprintf(lmsg[1],"Key Location");
            sprintf(lmsg[2],"Block Range");
            legend_highlight == 2 ? legend_color[2] = mColor.flash_color : legend_color[2] = 10;
         break;
         case 5:
             mWM.mW[7].num_legend_lines = 2;
             sprintf(lmsg[1],"Start Location");
         break;
         case 6:
             mWM.mW[7].num_legend_lines = 2;
             sprintf(lmsg[1],"Orb Location");
         break;
         case 7:
             mWM.mW[7].num_legend_lines = 2;
             sprintf(lmsg[1],"Mine Location");
         break;
         case 8:
            sprintf(lmsg[1],"Bomb Location");
            sprintf(lmsg[2],"Damage Range");
            legend_highlight == 2 ? legend_color[2] = mColor.flash_color : legend_color[2] = 14;
         break;
         case 9: // trigger
            sprintf(lmsg[1],"Trigger Item Location");
            sprintf(lmsg[2],"Trigger Field");
            legend_highlight == 2 ? legend_color[2] = mColor.flash_color : legend_color[2] = 14;
         break;
         case 10:
            sprintf(lmsg[1],"Scroll Location");
            sprintf(lmsg[2],"Message Area");
            legend_highlight == 2 ? legend_color[2] = mColor.flash_color : legend_color[2] = 10;
         break;
         case 11:
            sprintf(lmsg[1],"Rocket Location");
            sprintf(lmsg[2],"Damage Range");
            legend_highlight == 2 ? legend_color[2] = mColor.flash_color : legend_color[2] = 14;
         break;
         case 12:
             mWM.mW[7].num_legend_lines = 2;
             sprintf(lmsg[1],"Warp Location");
         break;
         case 13:
            sprintf(lmsg[1],"Timer Location");
            sprintf(lmsg[2],"Display Area");
            legend_highlight == 2 ? legend_color[2] = mColor.flash_color : legend_color[2] = 14;
         break;
         case 14: // switch
            sprintf(lmsg[1],"Switch Location");
            sprintf(lmsg[2],"Block Range");
            legend_highlight == 2 ? legend_color[2] = mColor.flash_color : legend_color[2] = 10;
         break;
         case 15:
            sprintf(lmsg[1],"Sproingy Location");
            sprintf(lmsg[2],"Jump Height");
            legend_highlight == 2 ? legend_color[2] = mColor.flash_color : legend_color[2] = 14;
         break;
         case 16: // block manip
            mWM.mW[7].num_legend_lines = 3;
            sprintf(lmsg[1],"Block Manip Location");
            legend_highlight == 2 ? legend_color[2] = mColor.flash_color : legend_color[2] = 12;
            sprintf(lmsg[2],"Manip Field");
            if (mItem.item[num][3] == 4)
            {
               mWM.mW[7].num_legend_lines = 4;
               sprintf(lmsg[3],"Source Field");
               legend_highlight == 3 ? legend_color[3] = mColor.flash_color : legend_color[3] = 11;
            }
         break;
         case 17: // block damage
            sprintf(lmsg[1],"Item Location");
            sprintf(lmsg[2],"Damage Area");
            legend_highlight == 2 ? legend_color[2] = mColor.flash_color : legend_color[2] = 10;
         break;
      } // end of switch case
   }  // end of items

   al_draw_rectangle(x1, y1, x2, y2, mColor.pc[13], 1);  // outline entire window

   if (mWM.mW[7].num_legend_lines > 0)
   {
      al_draw_text(mFont.pr8, mColor.pc[legend_color[0]], xc, y2-36+ (4-mWM.mW[7].num_legend_lines)*8, ALLEGRO_ALIGN_CENTER, "Legend");
      al_draw_rectangle(xc-100, y2-38+ (4-mWM.mW[7].num_legend_lines)*8, xc+100, y2-1, mColor.pc[13], 1); // big frame
      al_draw_rectangle(xc-100, y2-38+ (4-mWM.mW[7].num_legend_lines)*8, xc+100, y2-28+ (4-mWM.mW[7].num_legend_lines)*8, mColor.pc[13], 1); // top frame
   }

   for (int x=1; x<mWM.mW[7].num_legend_lines; x++)// draw text lines
      al_draw_text(mFont.pr8, mColor.pc[legend_color[x]], xc, y2-26+(3-mWM.mW[7].num_legend_lines+x)*8, ALLEGRO_ALIGN_CENTER, lmsg[x]);
}
void mwWindow::ov_draw_buttons(int x1, int y1, int x2, int y2, int d)
{
   int obt = mWM.mW[7].obt;
   int n = mWM.mW[7].num;
   int type=0;
   if (obt == 2) type = mItem.item[n][0];
   if (obt == 3) type = mEnemy.Ei[n][0];



   mWM.mW[7].mb = 0;


   // button x position
   int xa = x1+1;
   int xb = x2-1;
   int ya = y1+38;
   // split into thirds
   int x13 = xa + 1 * (xb-xa) / 3; // 1/3
   int x23 = xa + 2 * (xb-xa) / 3; // 2/3
   // split into quarters
   int x14 = xa + 1 * (xb-xa) / 4; // 1/4
   // split into sevenths
   int x27 = xa + 2 * (xb-xa) / 7; // 2/7
   int x57 = xa + 5 * (xb-xa) / 7; // 5/7

   int bts=16;
   int dim=128;


   // --------------------------------------------------
   // --- Common ---------------------------------------
   // --------------------------------------------------
   int lc = 6; // lock_color;
   if (mWM.mW[7].viewer_lock) lc = 7;

   if (mWidget.buttont(xa,  ya, x27-1, bts,                0,0,0,0,  0, 9,15,0, 1,0,0,d, "Prev")) mWM.mW[7].mb = 22;
       mWidget.toggle( x27, ya, x57-1, bts,                0,0,0,0,  0,lc,15,0, 1,0,0,d, mWM.mW[7].viewer_lock, "Unlocked", "Locked", 15, 15, 6, 7);
   if (mWidget.buttont(x57, ya, xb,    bts,                0,0,0,0,  0, 9,15,0, 1,0,1,d, "Next")) mWM.mW[7].mb = 21;

   if (obt == 4) { if (mWidget.buttont(xa, ya, x13-1, bts, 0,0,0,0,  0,13,15,0, 1,0,0,d, "Run Lifts")) mWM.mW[7].mb = 18; }
   else          { if (mWidget.buttont(xa, ya, x13-1, bts, 0,0,0,0,  0,13,15,0, 1,0,0,d, "Move"))      mWM.mW[7].mb = 18; }

   if (mWidget.buttont(x13, ya, x23-1, bts,                0,0,0,0,  0,14,15,0, 1,0,0,d, "Create")) mWM.mW[7].mb = 19;
   if (mWidget.buttont(x23, ya, xb,    bts,                0,0,0,0,  0,10,15,0, 1,0,1,d, "Delete")) mWM.mW[7].mb = 20;

   if (mWidget.buttont( xa, ya, x14-1, bts,                0,0,0,0,  0,1,15,0,  1,0,0,d, "Help")) mHelp.help("Viewer Basics");
   mWidget.button(     x14, ya, xb,    bts,               57,0,0,0,  0,1,15,0,  1,0,1,d);         // object specific help

   ya+=4; // space after common buttons



   // --------------------------------------------------
   // --- Lifts ----------------------------------------
   // --------------------------------------------------
   if (obt == 4) // lifts
   {
      int lift = n;
      int step = mLift.cur[n].current_step;

      mWidget.buttonp(       xa, ya, xb, bts, 500,0,0,0,    0,13,15,0,  1,0,1,d, mLift.cur[lift].mode); // MODE
      if (mLift.cur[lift].mode)
         mWidget.slideri    (xa, ya, xb, bts, 0,0,0,0,      0,13,15,15, 1,0,1,d, mLift.cur[lift].val2, 2000, 1, 1,  "Reset Timer:");
      if (mWidget.button(    xa, ya, xb, bts, 504,lift,0,0, 0, 4,15,0,  1,0,1,d)) mWM.mW[7].mb = 26; // lift name
      ya+=bts;

      // list of step buttons
      // --------------------------------------------------------------------------------
      int ysb = ya; // y pos of step buttons

      // process mouse on list of step buttons
      int step_pointer = -99;
      if ((mInput.mouse_x > xa + 10) && (mInput.mouse_x < xb - 10)) // is mouse on step buttons?
      {
         int step0_y_offset = ysb + 14 + bts;
         int mouse_step = (mInput.mouse_y - step0_y_offset) / bts;               // calculate step that mouse is on
         if ((mouse_step >= 0) && (mouse_step < mLift.cur[lift].num_steps))   // is this a valid step?
         {
            step_pointer = mouse_step;  // set step pointer to this step
            if (mInput.mouse_b[1][0])
            {
               step = mouse_step; // set current step to this step
               mLift.cur[lift].current_step = step; // set current step in lift
            }
            if (mInput.mouse_b[2][0]) mLift.step_popup_menu(lift, step_pointer); // step pop-up menu for this step
         }
      }

      // draw the list of steps  - this has to go after, because it can eat the mouse clicks needed for previous section
      int ycs = ysb + mLift.draw_steps(xa, xb, ysb, lift, step, step_pointer, d);
      ycs +=bts;

      // draw buttons for the current step button and get y postion for next item (lift)
      int yld = ycs + mLift.draw_current_step_buttons(xa, xb, ycs, lift, step, d);


      mWM.mW[7].h = yld - mWM.mW[7].y1-1; // global variable for height of ovw when variable due to lift
      mWM.mW[7].set_size(mWM.mW[7].w, mWM.mW[7].h);

      yld += bts;

      // draw current lift under step list buttons
      if (bts == 16) // only if max button size
      {
         int x1 = xa+10;
         int y1 = yld; // only see in 2 highest screen modes
         int s = step;

         // if step is not a move step, find prev that is
         if ((mLift.stp[lift][s].type & 31) != 1) s = mLift.find_previous_move_step(lift, s);

         // get w h from step
         int w  = mLift.stp[lift][s].w;
         int h  = mLift.stp[lift][s].h;

         // get x2 and y2 based on x1 y1 and w h
         int x2 = x1 + w;
         int y2 = y1 + h;
         mLift.draw_lift(lift, x1, y1, x2, y2);
      }

      if (mWM.mW[7].mb == 26)
      {
         char fst[80];
         strcpy(fst, mLift.cur[lift].lift_name);
         if (mMiscFnx.edit_lift_name(lift, yld, xa+10, fst)) strcpy(mLift.cur[lift].lift_name, fst);
      }
   }


   // --------------------------------------------------
   // --- Enemies --------------------------------------
   // --------------------------------------------------
   if (obt == 3)    // enemies
   {
      switch (type) // enemy subtypes
      {
         case 1: // bouncer
            mWidget.sliderf(    xa, ya, xb, bts,  22,n,0,0, 0, 9,15,15,  1,0,1,d, mEnemy.Ef[n][5],  12, 0, 0.01, "Speed:");
            ya+=4; // spacer
            if (mWidget.buttont(xa, ya, xb, bts,  0,0,0,0,  0,12,15, 0,  1,0,1,d, "Set Initial Direction"))  mMiscFnx.getxy("Initial Direction", 96, 4, n);
            mWidget.slideri(    xa, ya, xb, bts,  0,0,0,0,  0,12,15,15,  1,0,1,d, mEnemy.Ei[n][8], 100, 0, 1,   "Seek Count:");
            ya+=4; // spacer
            mWidget.button(     xa, ya, xb, bts,  13,n,0,0, 0, 8, 9, 0,  1,0,1,d); // main shape
            ya+=4; // spacer
            mWidget.button(     xa, ya, xb, bts,  14,n,0,0, 0, 8, 9, 0,  1,0,1,d); // seek shape
            ya+=4; // spacer
            mWidget.slideri(    xa, ya, xb, bts,  0,0,0,0,  0, 4,15,15,  1,0,1,d, mEnemy.Ei[n][29], 20, 0, 1,    "Collision Box:");
            mWidget.sliderf(    xa, ya, xb, bts,  0,0,0,0,  0, 4,15,15,  1,0,1,d, mEnemy.Ef[n][4] , 10, 0, 0.1,  "Health Decrement:");
            mWidget.slideri(    xa, ya, xb, bts,  0,0,0,0,  0, 4,15,15,  1,0,1,d, mEnemy.Ei[n][25], 50, 0, 1,    "Health Bonus:");
         break;
         case 2: // cannon
            mWidget.sliderf(    xa, ya, xb, bts,  22,n,0,0, 0, 9,15,15,  1,0,1,d, mEnemy.Ef[n][5],  12, 0, 0.01, "Speed:");
            ya+=4; // spacer
            mWidget.slideri(    xa, ya, xb, bts,  0,0,0,0,  0,12,15,15,  1,0,1,d, mEnemy.Ei[n][8], 100, 0, 1,     "Seek Count:");
            if (mWidget.buttont(xa, ya, xb, bts,  0,0,0,0,  0,12,15, 0,  1,0,1,d, "Set Initial Direction"))  mMiscFnx.getxy("Initial Direction", 96, 4, n);
            ya+=4; // spacer
            mWidget.slideri(    xa, ya, xb, bts,  0,0,0,0,  0,8, 15,15,  1,0,1,d, mEnemy.Ei[n][9], 40, 0, 1,      "Extra Hits To Kill:");
            ya+=4; // spacer
            mWidget.sliderf(    xa, ya, xb, bts,  0,0,0,0,  0,9, 15,15,  1,0,1,d, mEnemy.Ef[n][7],  20, 0.8, 0.1, "Shot Speed:");
            mWidget.slideri(    xa, ya, xb, bts,  0,0,0,0,  0,9, 15,15,  1,0,1,d, mEnemy.Ei[n][17], 2000, 20, 1,  "Shot Proximity:");
            mWidget.slideri(    xa, ya, xb, bts,  0,0,0,0,  0,9, 15,15,  1,0,1,d, mEnemy.Ei[n][15], 200, 1, 1,    "Shot Retrigger Time:");
            ya+=4; // spacer
            mWidget.slideri(    xa, ya, xb, bts,  0,0,0,0,  0,4, 15,15,  1,0,1,d, mEnemy.Ei[n][29], 20, 0, 1,     "Collision Box:");
            mWidget.sliderf(    xa, ya, xb, bts,  0,0,0,0,  0,4, 15,15,  1,0,1,d, mEnemy.Ef[n][4],  10, 0, 0.1,   "Health Decrement:");
            mWidget.slideri(    xa, ya, xb, bts,  0,0,0,0,  0,4, 15,15,  1,0,1,d, mEnemy.Ei[n][25], 50, 0, 1,     "Health Bonus:");
         break;
         case 3: // archwag
            mWidget.sliderf(    xa, ya, xb, bts,  0,0,0,0,  0,12,15,15,  1,0,1,d, mEnemy.Ef[n][6],  9,  0.7, 0.01, "X-Speed:");
            mWidget.sliderf(    xa, ya, xb, bts,  0,0,0,0,  0,12,15,15,  1,0,1,d, mEnemy.Ef[n][3],  16, 0.7, 0.01, "Y-Speed:");
            ya+=4; // spacer
            mWidget.toggle(     xa, ya, xb, bts,  0,0,0,0,  0, 0, 0, 0,  1,0,1,d, mEnemy.Ei[n][2], "Initial Direction:Left ", "Initial Direction:Right", 13, 13, 15, 15);
            mWidget.toggle(     xa, ya, xb, bts,  0,0,0,0,  0, 0, 0, 0,  1,0,1,d, mEnemy.Ei[n][8], "Follow Mode",             "Bounce Mode",             13, 13, 15, 15);
            ya+=4; // spacer
            mWidget.slider0(    xa, ya, xb, bts,  0,0,0,0,  0, 7,15,15,  1,0,1,d, mEnemy.Ei[n][10], 100, 0, 1,    "Turn Before Hole:", "Off");
            mWidget.slider0(    xa, ya, xb, bts,  0,0,0,0,  0, 7,15,15,  1,0,1,d, mEnemy.Ei[n][11], 100, 0, 1,    "Jump Before Hole:", "Off");
            mWidget.slider0(    xa, ya, xb, bts,  0,0,0,0,  0, 7,15,15,  1,0,1,d, mEnemy.Ei[n][12], 100, 0, 1,    "Jump Before Wall:", "Off");
            mWidget.slider0(    xa, ya, xb, bts,  0,0,0,0,  0, 7,15,15,  1,0,1,d, mEnemy.Ei[n][7],  600, 0, 1,    "Jump Under Width:", "Off");
            mWidget.slider0(    xa, ya, xb, bts,  0,0,0,0,  0, 7,15,15,  1,0,1,d, mEnemy.Ei[n][6],  500, 0, 1,    "Jump Timer Wait:",  "Off");
            ya+=4; // spacer
            mWidget.sliderf(    xa, ya, xb, bts,  0,0,0,0,  0, 9,15,15,  1,0,1,d, mEnemy.Ef[n][7],  20, 0.8, 0.1, "Shot Speed:");
            mWidget.slideri(    xa, ya, xb, bts,  0,0,0,0,  0, 9,15,15,  1,0,1,d, mEnemy.Ei[n][17], 2000, 20, 1,  "Shot Proximity:");
            mWidget.slideri(    xa, ya, xb, bts,  0,0,0,0,  0, 9,15,15,  1,0,1,d, mEnemy.Ei[n][15], 200, 1, 1,    "Shot Retrigger Time:");
            ya+=4; // spacer
            mWidget.slideri(    xa, ya, xb, bts,  0,0,0,0,  0, 4,15,15,  1,0,1,d, mEnemy.Ei[n][29], 20, 0, 1,     "Collision Box:");
            mWidget.sliderf(    xa, ya, xb, bts,  0,0,0,0,  0, 4,15,15,  1,0,1,d, mEnemy.Ef[n][4],  10, 0, 0.1,   "Health Decrement:");
            mWidget.slideri(    xa, ya, xb, bts,  0,0,0,0,  0, 4,15,15,  1,0,1,d, mEnemy.Ei[n][25], 50, 0, 1,     "Health Bonus:");
         break;
         case 4: // block walker
            mWidget.sliderf(    xa, ya, xb, bts,  0,0,0,0,  0,12,15,15,  1,0,1,d, mEnemy.Ef[n][6],  9, 0.7, 0.01, "X-Speed:");
            mWidget.sliderf(    xa, ya, xb, bts,  0,0,0,0,  0,12,15,15,  1,0,1,d, mEnemy.Ef[n][3],  9, 0.7, 0.01, "Y-Speed:");
            ya+=4; // spacer
            mWidget.toggle(     xa, ya, xb, bts,  0,0,0,0,  0, 0, 0, 0,  1,0,1,d, mEnemy.Ei[n][2], "Initial Direction:Left ", "Initial Direction:Right", 13, 13, 15, 15);
            mWidget.toggle(     xa, ya, xb, bts,  0,0,0,0,  0, 0, 0, 0,  1,0,1,d, mEnemy.Ei[n][8], "Follow Mode",             "Bounce Mode",             13, 13, 15, 15);
            ya+=4; // spacer
            mWidget.slider0(    xa, ya, xb, bts,  0,0,0,0,  0, 7,15,15,  1,0,1,d, mEnemy.Ei[n][10], 100, 0, 1,    "Turn Before Hole:", "Off");
            mWidget.slider0(    xa, ya, xb, bts,  0,0,0,0,  0, 7,15,15,  1,0,1,d, mEnemy.Ei[n][11], 100, 0, 1,    "Jump Before Hole:", "Off");
            mWidget.slider0(    xa, ya, xb, bts,  0,0,0,0,  0, 7,15,15,  1,0,1,d, mEnemy.Ei[n][12], 100, 0, 1,    "Jump Before Wall:", "Off");
            mWidget.slider0(    xa, ya, xb, bts,  0,0,0,0,  0, 7,15,15,  1,0,1,d, mEnemy.Ei[n][7],  600, 0, 1,    "Jump Under Width:", "Off");
            mWidget.slider0(    xa, ya, xb, bts,  0,0,0,0,  0, 7,15,15,  1,0,1,d, mEnemy.Ei[n][6],  500, 0, 1,    "Jump Timer Wait:",  "Off");

            ya+=4; // spacer

            mWidget.button(     xa, ya, xb, bts, 313,n,0,0,  0, 8,15,0,   1,0,1,d); // block

            ya+=4; // spacer
            mWidget.slideri(    xa, ya, xb, bts,  0,0,0,0,  0, 4,15,15,  1,0,1,d, mEnemy.Ei[n][29], 20, 0, 1,     "Collision Box:");
            mWidget.sliderf(    xa, ya, xb, bts,  0,0,0,0,  0, 4,15,15,  1,0,1,d, mEnemy.Ef[n][4],  10, 0, 0.1,   "Health Decrement:");
            mWidget.slideri(    xa, ya, xb, bts,  0,0,0,0,  0, 4,15,15,  1,0,1,d, mEnemy.Ei[n][25], 50, 0, 1,     "Health Bonus:");
         break;
         case 5: // jumpworm
            mWidget.sliderf(    xa, ya, xb, bts,  0,0,0,0,  0,12,15,15,  1,0,1,d, mEnemy.Ef[n][6],  12, 0.4, 0.01,"X-Speed (Jump):");
            mWidget.slideri(    xa, ya, xb, bts,  0,0,0,0,  0,11,15,15,  1,0,1,d, mEnemy.Ei[n][4],  12, 1, 1,     "X-Speed (Ground):1/");
            mWidget.sliderf(    xa, ya, xb, bts,  0,0,0,0,  0,12,15,15,  1,0,1,d, mEnemy.Ef[n][3],  9, 0.4, 0.01, "Y-Speed:");
            ya+=4; // spacer
            mWidget.toggle(     xa, ya, xb, bts,  0,0,0,0,  0, 0, 0, 0,  1,0,1,d, mEnemy.Ei[n][2], "Initial Direction:Left ", "Initial Direction:Right", 13, 13, 15, 15);
            ya+=4; // spacer
            mWidget.slider0(    xa, ya, xb, bts,  0,0,0,0,  0, 7,15,15,  1,0,1,d, mEnemy.Ei[n][10], 100, 0, 1,    "Turn Before Hole:", "Off");
            mWidget.slider0(    xa, ya, xb, bts,  0,0,0,0,  0, 7,15,15,  1,0,1,d, mEnemy.Ei[n][11], 100, 0, 1,    "Jump Before Hole:", "Off");
            mWidget.slider0(    xa, ya, xb, bts,  0,0,0,0,  0, 7,15,15,  1,0,1,d, mEnemy.Ei[n][12], 100, 0, 1,    "Jump Before Wall:", "Off");
            mWidget.slider0(    xa, ya, xb, bts,  0,0,0,0,  0, 7,15,15,  1,0,1,d, mEnemy.Ei[n][8],  160, 0, 1,    "Wall Jump Boost:", "Off");
            mWidget.slider0(    xa, ya, xb, bts,  0,0,0,0,  0, 7,15,15,  1,0,1,d, mEnemy.Ei[n][7],  600, 0, 1,    "Jump Under Width:", "Off");
            mWidget.slider0(    xa, ya, xb, bts,  0,0,0,0,  0, 7,15,15,  1,0,1,d, mEnemy.Ei[n][6],  500, 0, 1,    "Jump Timer Wait:",  "Off");
            ya+=4; // spacer
            mWidget.slideri(    xa, ya, xb, bts,  0,0,0,0,  0, 4,15,15,  1,0,1,d, mEnemy.Ei[n][29], 20, 0, 1,     "Collision Box:");
            mWidget.sliderf(    xa, ya, xb, bts,  0,0,0,0,  0, 4,15,15,  1,0,1,d, mEnemy.Ef[n][4],  10, 0, 0.1,   "Health Decrement:");
            mWidget.slideri(    xa, ya, xb, bts,  0,0,0,0,  0, 4,15,15,  1,0,1,d, mEnemy.Ei[n][25], 50, 0, 1,     "Health Bonus:");
         break;
         case 6: // flapper
            mWidget.sliderf(    xa, ya, xb, bts,  0,0,0,0,  0,12,15,15,  1,0,1,d, mEnemy.Ef[n][5],  8, .5, 0.1,    "X-Speed:");
            mWidget.sliderf(    xa, ya, xb, bts,  0,0,0,0,  0,12,15,15,  1,0,1,d, mEnemy.Ef[n][6],  1, 0.01, 0.01, "X-Accel:");
            mWidget.toggle(     xa, ya, xb, bts,  0,0,0,0,  0, 0, 0, 0,  1,0,1,d, mEnemy.Ei[n][2], "Initial Direction:Left ", "Initial Direction:Right", 15, 15, 12, 12);
            ya+=4; // spacer
            mWidget.sliderf(    xa, ya, xb, bts,  0,0,0,0,  0,13,15,15,  1,0,1,d, mEnemy.Ef[n][3],  5, 0, 0.01,      "Y-Speed:");
            mWidget.sliderf(    xa, ya, xb, bts,  0,0,0,0,  0,13,15,15,  1,0,1,d, mEnemy.Ef[n][10],  8, .5, 0.1,     "Flap Speed:");
            mWidget.slideri(    xa, ya, xb, bts,  0,0,0,0,  0,13,15,15,  1,0,1,d, mEnemy.Ei[n][21], 400, 0, 5,       "Flap Height:");
            mWidget.slideri(    xa, ya, xb, bts,  0,0,0,0,  0,13,15,15,  1,0,1,d, mEnemy.Ei[n][20], 1000, -1000, 10, "Height Above Player:");
            ya+=4; // spacer
            mWidget.sliderf(    xa, ya, xb, bts,  0,0,0,0,  0, 9,15,15,  1,0,1,d, mEnemy.Ef[n][7],  20, 0.8, 0.1, "Shot Speed:");
            mWidget.slideri(    xa, ya, xb, bts,  0,0,0,0,  0, 9,15,15,  1,0,1,d, mEnemy.Ei[n][17], 2000, 20, 1,  "Shot Proximity:");
            mWidget.slideri(    xa, ya, xb, bts,  0,0,0,0,  0, 9,15,15,  1,0,1,d, mEnemy.Ei[n][15], 200, 1, 1,    "Shot Retrigger Time:");
            ya+=4; // spacer
            mWidget.slideri(    xa, ya, xb, bts,  0,0,0,0,  0, 4,15,15,  1,0,1,d, mEnemy.Ei[n][29], 20, 0, 1,     "Collision Box:");
            mWidget.sliderf(    xa, ya, xb, bts,  0,0,0,0,  0, 4,15,15,  1,0,1,d, mEnemy.Ef[n][4],  10, 0, 0.1,   "Health Decrement:");
            mWidget.slideri(    xa, ya, xb, bts,  0,0,0,0,  0, 4,15,15,  1,0,1,d, mEnemy.Ei[n][25], 50, 0, 1,     "Health Bonus:");
         break;




         case 7: // vinepod
         {



            if (mWidget.buttont(xa, ya, xb, bts,  0,0,0,0,  0,10,15, 0,  1,0,1,d, "Set Extended Position")) mMiscFnx.getxy("Vinepod Extended Position", 90, 13, n);
            mWidget.slideri(    xa, ya, xb, bts,  0,0,0,0,  0, 4,15,15,  1,0,1,d, mEnemy.Ei[n][17], 400, 10, 10,  "Extend Time:");
            mWidget.sliderf(    xa, ya, xb, bts,  0,0,0,0,  0, 4,15,15,  1,0,1,d, mEnemy.Ef[n][9],  400,  0,  1,  "Pause:");

            ya+=4; // spacer

            mWidget.togglf(     xa, ya, xb, bts, 0,0,0,0,   0,0,0,0,     1,0,1,d, mEnemy.Ei[n][20], PM_ENEMY_VINEPOD_FIRE_SHOT, "Fire Shot:OFF", "Fire Shot:ON ", 15+dim, 15, 11+dim, 11);
            if (mEnemy.Ei[n][20] & PM_ENEMY_VINEPOD_FIRE_SHOT)
            mWidget.sliderf(    xa, ya, xb, bts,  0,0,0,0,  0,11,15,15,  1,0,1,d, mEnemy.Ef[n][7],  20, 0.8, 0.1, "Shot Speed:");

            ya+=4; // spacer

            mWidget.slider0(    xa, ya, xb, bts, 0,0,0,0,    0,2,15,15,  1,0,1,d, mEnemy.Ei[n][19], 99, 0, 1, "Output Event:", "OFF");

            ya+=4; // spacer

            mWidget.togglf(     xa, ya, xb, bts, 0,0,0,0,    0,0,0,0,    1,0,1,d, mEnemy.Ei[n][20], PM_ENEMY_VINEPOD_SHOW_PATH, "Hide Path","Show Path", 15+dim, 15, 14+dim, 14);
            mWidget.togglf(     xa, ya, xb, bts, 0,0,0,0,    0,0,0,0,    1,0,1,d, mEnemy.Ei[n][20], PM_ENEMY_VINEPOD_INV_INIT, "Invincible At Rest:OFF", "Invincible At Rest:ON ", 15+dim, 15, 14+dim, 14);
            mWidget.togglf(     xa, ya, xb, bts, 0,0,0,0,    0,0,0,0,    1,0,1,d, mEnemy.Ei[n][20], PM_ENEMY_VINEPOD_INV_EXTN, "Invincible Extended:OFF", "Invincible Extended:ON ", 15+dim, 15, 14+dim, 14);
            mWidget.togglf(     xa, ya, xb, bts, 0,0,0,0,    0,0,0,0,    1,0,1,d, mEnemy.Ei[n][20], PM_ENEMY_VINEPOD_INV_MOVE, "Invincible Moving:OFF", "Invincible Moving:ON ", 15+dim, 15, 14+dim, 14);

            ya+=4; // spacer

            mWidget.slider0(    xa, ya, xb, bts,  0,0,0,0,  0,13,15,15,  1,0,1,d, mEnemy.Ei[n][18], 99, 0, 1,    "Event Trigger:", "OFF");
            if (mWidget.buttont(xa, ya, xb, bts,  0,0,0,0,  0,13,15,0,   1,0,1,d, "Set Trigger")) mTriggerEvent.find_event_sender_for_obj(3, n, 0, 0);

            ya+=4; // spacer

            mWidget.togglf(     xa, ya, xb, bts,  0,0,0,0,  0,0,0,0,     1,0,1,d, mEnemy.Ei[n][20], PM_ENEMY_VINEPOD_USE_TRIGGER, "Use Internal Trigger:OFF", "Use Internal Trigger:ON ", 15+dim, 15, 14+dim, 14);
            if (mEnemy.Ei[n][20] & PM_ENEMY_VINEPOD_USE_TRIGGER)
            {
            if (mWidget.buttont(xa, ya, xb, bts,  0,0,0,0,  0,14,15, 0,  1,0,1,d, "Set Internal Trigger Box")) mMiscFnx.get_block_range("Trigger Box", &mEnemy.Ei[n][11], &mEnemy.Ei[n][12], &mEnemy.Ei[n][13], &mEnemy.Ei[n][14], 2);
            }

            ya+=4; // spacer

            mWidget.togglf(     xa, ya, xb, bts, 0,0,0,0,   0,0,0,0,     1,0,1,d, mEnemy.Ei[n][20], PM_ENEMY_VINEPOD_USE_SPLINE, "Use Spline:OFF", "Use Spline:ON ", 15+dim, 15, 3+dim, 3);
            if (mEnemy.Ei[n][20] & PM_ENEMY_VINEPOD_USE_SPLINE)
            {
            mWidget.sliderf(    xa, ya, xb, bts,  0,0,0,0,  0, 3,15,15,  1,0,1,d, mEnemy.Ef[n][6],  8,  1,  0.1,  "Control Point Scale:");
            if (mWidget.buttont(xa, ya, xb, bts,  0,0,0,0,  0, 6,15, 0,  1,0,1,d, "Set Control Point 1")) mMiscFnx.getxy("Vinepod Control Point 1", 91, 13, n);
            if (mWidget.buttont(xa, ya, xb, bts,  0,0,0,0,  0, 7,15, 0,  1,0,1,d, "Set Control Point 2")) mMiscFnx.getxy("Vinepod Control Point 2", 92, 13, n);
            //if (mWidget.buttont(xa, ya, xb, bts,  0,0,0,0,  0, 3,15, 0,  1,0,1,d, "Set Thirds")) mEnemy.vinepod_set_cp_thirds(n);
            }

            ya+=4; // spacer
            static int xfrm = 1;
            static int rot = 90;
            static float sca = 1.0;



            int c = 6;
            mWidget.toggle(     xa, ya, xb, bts,  0,0,0,0,  0, 0, 0, 0,  1,0,1,d, xfrm, "Transform Tools:Off", "Transform Tools:On", 15, 15, c+64, c);


            if (xfrm)
            {
               int xh = 70;
               if (mWidget.buttont(xa, ya, xa+xh-2, bts,  0,0,0,0,  0, c,15, 0,  1,0,0,d, "Rotate")) mEnemy.vinepod_rotate(n, mEnemy.deg_to_rad(rot));
               mWidget.slideri(    xa+xh+2, ya, xb, bts,  0,0,0,0,  0, c,15,15,  1,0,1,d, rot,  180,  -180,  1,  "Degrees:");

  //             ya+=4; // spacer

               if (mWidget.buttont(xa, ya, xa+xh-2, bts,  0,0,0,0,  0, c,15, 0,  1,0,0,d, "Scale")) mEnemy.vinepod_scale(n, sca);
               mWidget.sliderf(    xa+xh+2, ya, xb, bts,  0,0,0,0,  0, c,15,15,  1,0,1,d, sca,  10,  0.01,  .01,  "");

//               ya+=4; // spacer

               xh = (xb-xa)/2;
               if (mWidget.buttont(xa, ya, xa+xh-2, bts,  0,0,0,0,  0, c,15, 0,  1,0,0,d, "Mirror X")) mEnemy.vinepod_mirror_x(n);
               if (mWidget.buttont(xa+xh+2, ya, xb, bts,  0,0,0,0,  0, c,15, 0,  1,0,1,d, "Mirror Y")) mEnemy.vinepod_mirror_y(n);
            }
            ya+=4; // spacer
            mWidget.slideri(    xa, ya, xb, bts,  0,0,0,0,  0, 4,15,15,  1,0,1,d, mEnemy.Ei[n][29], 20, 0, 1,     "Collision Box:");
            mWidget.sliderf(    xa, ya, xb, bts,  0,0,0,0,  0, 4,15,15,  1,0,1,d, mEnemy.Ef[n][4],  10, 0, 0.1,   "Health Decrement:");
            mWidget.slideri(    xa, ya, xb, bts,  0,0,0,0,  0, 4,15,15,  1,0,1,d, mEnemy.Ei[n][25], 50, 0, 1,     "Health Bonus:");
         }
         break;
         case 8: // trakbot
            mWidget.sliderf(    xa, ya, xb, bts,  0,0,0,0,  0,12,15,15,  1,0,1,d, mEnemy.Ef[n][2],  10, .5, 0.1, "X-Speed:");
            mWidget.sliderf(    xa, ya, xb, bts,  0,0,0,0,  0,12,15,15,  1,0,1,d, mEnemy.Ef[n][3],  10, .5, 0.1, "Y-Speed:");
            ya+=4; // spacer
            mWidget.button(     xa, ya, xb, bts,  11,n,0,0, 0,12,15, 0,  1,0,1,d);  // initial direction
            ya+=4; // spacer
            mWidget.toggle(     xa, ya, xb, bts,  0,0,0,0,  0, 0, 0, 0,  1,0,1,d, mEnemy.Ei[n][7], "Drop Mode:Off", "Drop Mode:On ", 15, 15, 6+64, 6);
            ya+=4; // spacer
            mWidget.sliderf(    xa, ya, xb, bts,  0,0,0,0,  0, 9,15,15,  1,0,1,d, mEnemy.Ef[n][7],  20, 0.8, 0.1, "Shot Speed:");
            mWidget.slideri(    xa, ya, xb, bts,  0,0,0,0,  0, 9,15,15,  1,0,1,d, mEnemy.Ei[n][17], 2000, 20, 1,  "Shot Proximity:");
            mWidget.slideri(    xa, ya, xb, bts,  0,0,0,0,  0, 9,15,15,  1,0,1,d, mEnemy.Ei[n][15], 200, 1, 1,    "Shot Retrigger Time:");
            ya+=4; // spacer
            mWidget.slideri(    xa, ya, xb, bts,  0,0,0,0,  0, 4,15,15,  1,0,1,d, mEnemy.Ei[n][29], 20, 0, 1,     "Collision Box:");
            mWidget.sliderf(    xa, ya, xb, bts,  0,0,0,0,  0, 4,15,15,  1,0,1,d, mEnemy.Ef[n][4],  10, 0, 0.1,   "Health Decrement:");
            mWidget.slideri(    xa, ya, xb, bts,  0,0,0,0,  0, 4,15,15,  1,0,1,d, mEnemy.Ei[n][25], 50, 0, 1,     "Health Bonus:");
         break;
         case 9: // cloner
            mWidget.toggle(     xa, ya, xb, bts,  0,0,0,0,  0, 0, 0, 0,  1,0,1,d, mEnemy.Ei[n][30], "Not Invincible", "Invincible!", 15, 15, 12, 10);
            ya+=4; // spacer
            mWidget.slider0(    xa, ya, xb, bts,  0,0,0,0,  0,13,15,15,  1,0,1,d, mEnemy.Ei[n][8], 99, 0, 1,    "Event Trigger:", "OFF");
            if (mWidget.buttont(xa, ya, xb, bts,  0,0,0,0,  0,13,15,0,   1,0,1,d, "Set Trigger")) mTriggerEvent.find_event_sender_for_obj(3, n, 0, 0);
            ya+=4; // spacer
            mWidget.slider0(    xa, ya, xb, bts,  0,0,0,0,  0,14,15,15,  1,0,1,d, mEnemy.Ei[n][9], 4800, 0, 1,  "Created Time To Live:", "-");
            mWidget.slider0(    xa, ya, xb, bts,  0,0,0,0,  0,14,15,15,  1,0,1,d, mEnemy.Ei[n][10], 600, 0, 1,  "Max Created Objects:", "-");
            ya+=4; // spacer
            if (mWidget.buttont(xa, ya, xb, bts,  0,0,0,0,  0,11,15, 0,  1,0,1,d, "Set Source Area")) mMiscFnx.get_block_range("Cloner Source Area", &mEnemy.Ei[n][15], &mEnemy.Ei[n][16], &mEnemy.Ei[n][19], &mEnemy.Ei[n][20], 1);
            if (mWidget.buttont(xa, ya, xb, bts,  0,0,0,0,  0,10,15, 0,  1,0,1,d, "Set Destination")) mMiscFnx.getxy("Cloner Destination", 98, 9, n);
            ya+=4; // spacer
            mWidget.buttonp(    xa, ya, xb, bts,  81,0,0,0, 0,13,15, 0,  1,0,1,d, mEnemy.Ei[n][4]); // show boxes
            mWidget.buttonp(    xa, ya, xb, bts,  82,0,0,0, 0,13,15, 0,  1,0,1,d, mEnemy.Ei[n][5]); // draw mode

            ya+=4; // spacer
            mWidget.slideri(    xa, ya, xb, bts,  0,0,0,0,  0, 4,15,15,  1,0,1,d, mEnemy.Ei[n][29], 20, 0, 1,   "Collision Box:");
            mWidget.sliderf(    xa, ya, xb, bts,  0,0,0,0,  0, 4,15,15,  1,0,1,d, mEnemy.Ef[n][4],  10, 0, 0.1, "Health Decrement:");
            mWidget.slideri(    xa, ya, xb, bts,  0,0,0,0,  0, 4,15,15,  1,0,1,d, mEnemy.Ei[n][25], 50, 0, 1,   "Health Bonus:");
         break;

         case 19: // crew
            mWidget.sliderf(    xa, ya, xb, bts,  0,0,0,0,  0,12,15,15,  1,0,1,d, mEnemy.Ef[n][6],  9, 0.7, 0.01, "X-Speed:");
            mWidget.sliderf(    xa, ya, xb, bts,  0,0,0,0,  0,12,15,15,  1,0,1,d, mEnemy.Ef[n][3],  9, 0.7, 0.01, "Y-Speed:");
            ya+=4; // spacer
            mWidget.toggle(     xa, ya, xb, bts,  0,0,0,0,  0, 0, 0, 0,  1,0,1,d, mEnemy.Ei[n][2], "Initial Direction:Left ", "Initial Direction:Right", 13, 13, 15, 15);
            mWidget.toggle(     xa, ya, xb, bts,  0,0,0,0,  0, 0, 0, 0,  1,0,1,d, mEnemy.Ei[n][8], "Follow Mode",             "Bounce Mode",             13, 13, 15, 15);
            ya+=4; // spacer
            mWidget.slider0(    xa, ya, xb, bts,  0,0,0,0,  0, 7,15,15,  1,0,1,d, mEnemy.Ei[n][10], 100, 0, 1,    "Turn Before Hole:", "Off");
            mWidget.slider0(    xa, ya, xb, bts,  0,0,0,0,  0, 7,15,15,  1,0,1,d, mEnemy.Ei[n][11], 100, 0, 1,    "Jump Before Hole:", "Off");
            mWidget.slider0(    xa, ya, xb, bts,  0,0,0,0,  0, 7,15,15,  1,0,1,d, mEnemy.Ei[n][12], 100, 0, 1,    "Jump Before Wall:", "Off");
            mWidget.slider0(    xa, ya, xb, bts,  0,0,0,0,  0, 7,15,15,  1,0,1,d, mEnemy.Ei[n][7],  600, 0, 1,    "Jump Under Width:", "Off");
            mWidget.slider0(    xa, ya, xb, bts,  0,0,0,0,  0, 7,15,15,  1,0,1,d, mEnemy.Ei[n][6],  500, 0, 1,    "Jump Timer Wait:",  "Off");
            ya+=4; // spacer
            mWidget.colsel(    xa, ya, xb, bts,  9,n,0,0,  0, 0, 0,0, 0,0,1,d); // change color
         break;















      }
   }

   // --------------------------------------------------
   // --- Items ----------------------------------------
   // --------------------------------------------------
   if (obt == 2) // items
   {
      switch (type) // item subtypes
      {
         case 1: // door
            mWidget.buttonp(   xa, ya, xb, bts, 22,0,0,0,  0,13,15,0, 1,0,1,d, mItem.item[n][3]); // stat | fall | carry
            ya+=4; // spacer
            mWidget.button(    xa, ya, xb, bts, 49,n,0,0,  0,12,15,0, 1,0,1,d); // door type
            if (mItem.item[n][8]) // regular door (not exit only)
            {
               ya+=4; // spacer
               mWidget.button( xa, ya, xb, bts,  4,n,0,0,  0,11,15,0, 1,0,1,d); // set linked item
               mWidget.buttonp(xa, ya, xb, bts, 50,n,0,0,  0,11,15,0, 1,0,1,d, mItem.item[n][11]); // door entry type
               mWidget.buttonp(xa, ya, xb, bts, 53,n,0,0,  0,11,15,0, 1,0,1,d, mItem.item[n][7]); // move type
               mWidget.buttonp(xa, ya, xb, bts, 51,n,0,0,  0,11,15,0, 1,0,1,d, mItem.item[n][12]); // exit link show
            }
            ya+=4; // spacer
            mWidget.button(    xa, ya, xb, bts, 52,n,0,0,  0,13,15,0, 1,0,1,d); // get new shape
            mWidget.colsel(    xa, ya, xb, bts,  5,n,0,0,  0, 0, 0,0, 0,0,1,d); // change color
         break;

         case 2: // bonus
            if (mItem.item[n][6] == 1) // health bonus
            {
               mWidget.buttonp(xa, ya, xb, bts, 22,0,0,0,   0,13,15,0,  1,0,1,d, mItem.item[n][3]); // stat | fall | carry
               ya+=4; // spacer
               mWidget.buttonp(xa, ya, xb, bts, 101,0,0,0,  0,11,15,0,  1,0,1,d, mItem.item[n][6]); // bonus type
               mWidget.slideri(xa, ya, xb, bts, 0,0,0,0,    0,11,15,15, 1,0,1,d, mItem.item[n][7], 100, 2, 1, "Health Bonus:");
               mItem.item[n][1] = 1035;
               mItem.item[n][2] = 1;
               if (mItem.item[n][7] == 10) mItem.item[n][1] = 1023;
               if (mItem.item[n][7] == 50) mItem.item[n][1] = 1027;
            }
            else if (mItem.item[n][6] == 2) // free man
            {
               mWidget.buttonp(xa, ya, xb, bts, 21, 0,0,0,  0,13,15,0,  1,0,1,d, mItem.item[n][3]); // stat | fall
               ya+=4; // spacer
               mWidget.buttonp(xa, ya, xb, bts, 101,0,0,0,  0,10,15,0,  1,0,1,d, mItem.item[n][6]); // bonus type
               mItem.item[n][1] = 1038;
               mItem.item[n][2] = 1;
            }
            else if (mItem.item[n][6] == 3) // purple coin
            {
               mWidget.buttonp(xa, ya, xb, bts, 21, 0,0,0,  0,13,15,0, 1,0,1,d, mItem.item[n][3]); // stat | fall
               ya+=4; // spacer
               mWidget.buttonp(xa, ya, xb, bts, 101,0,0,0,  0, 8,15,0, 1,0,1,d, mItem.item[n][6]); // bonus type
               mItem.item[n][1] = 197;
               mItem.item[n][2] = 0;
            }
         break;
         case 3: // exit
            mWidget.buttonp(    xa, ya, xb, bts, 22,0,0,0,  0,13,15, 0,  1,0,1,d, mItem.item[n][3]); // stat | fall | carry
            ya+=4; // spacer
            mWidget.slideri(    xa, ya, xb, bts, 0,0,0,0,   0,12,15,15,  1,0,1,d, mItem.item[n][8], 100, 0, 1,  "Enemy Count Lock:");
         break;
         case 4: // key
            mWidget.buttonp(    xa, ya, xb, bts, 21,0,0,0,  0,13,15, 0,  1,0,1,d, mItem.item[n][3]); // stat | fall
            ya+=4; // spacer
            mWidget.buttonp(    xa, ya, xb, bts, 102,0,0,0, 0, 8,15, 0,  1,0,1,d, mItem.item[n][1]); // color
            ya+=4; // spacer
            if (mWidget.buttont(xa, ya, xb, bts, 0,0,0,0,   0,10,15, 0,  1,0,1,d, "Get New Block Range")) mMiscFnx.get_block_range("Block Range", &mItem.item[n][6], &mItem.item[n][7], &mItem.item[n][8], &mItem.item[n][9], 1);
            ya+=4; // spacer
            mWidget.toggle(     xa, ya, xb, bts, 0,0,0,0,   0, 0, 0, 0,  1,0,1,d, mItem.item[n][12], "Remove All Blocks", "Remove Only Matching", 15, 15, 7, 7);
         break;
         case 5: // start
            mWidget.buttonp(    xa, ya, xb, bts, 22,0,0,0,  0,13,15,0,   1,0,1,d, mItem.item[n][3]); // stat | fall | carry
            ya+=4; // spacer
            mWidget.buttonp(    xa, ya, xb, bts, 78,0,0,0,  0,11,15,0,   1,0,1,d, mItem.item[n][6]); // start mode
            mWidget.buttonp(    xa, ya, xb, bts, 79,0,0,0,  0,11,15,0,   1,0,1,d, mItem.item[n][7]); // start index
         break;
         case 6: // orb
            mWidget.buttonp(    xa, ya, xb, bts, 22,0,0,0,  0,13,15,0,   1,0,1,d, mItem.item[n][3]); // stat | fall | carry
            ya+=4; // spacer
            mWidget.button(     xa, ya, xb, bts, 94,n,0,0,  0, 7,15, 0,  1,0,1,d);             // rotation
            ya+=4; // spacer
            mWidget.button(     xa, ya, xb, bts, 90,n,0,0,  0,12,15, 0,  1,0,1,d);             // orb trigger type
            ya+=4; // spacer
            mWidget.button(     xa, ya, xb, bts, 92,n,0,0,  0, 8,15, 0,  1,0,1,d);             // mode
            if (mItem.item[n][6] == 0)                                                           // only show initial state for toggle mode
               mWidget.togglf(  xa, ya, xb, bts, 0,0,0,0,   0,0,0,0,    1,0,1,d, mItem.item[n][2], PM_ITEM_ORB_STATE, "Initial State:OFF","Initial State:ON ", 15, 15, 8, 8);
            if ((mItem.item[n][6] == 3) || (mItem.item[n][6] == 4))                                    // only show timer for mode 3 and 4
               mWidget.slideri(xa, ya, xb, bts,     0,0,0,0,   0,8,15,15,  1,0,1,d, mItem.item[n][7], 400, 1, 1,  "Time:");
            ya+=4; // spacer
            mWidget.slider0(    xa, ya, xb, bts, 0,0,0,0,    0,13,15,15,  1,0,1,d, mItem.item[n][10], 99, 0, 1,    "Set Event Always While On   :", "OFF");
            mWidget.slider0(    xa, ya, xb, bts, 0,0,0,0,    0,13,15,15,  1,0,1,d, mItem.item[n][11], 99, 0, 1,    "Set Event Always While Off  :", "OFF");
            mWidget.slider0(    xa, ya, xb, bts, 0,0,0,0,    0,13,15,15,  1,0,1,d, mItem.item[n][12], 99, 0, 1,    "Set Event When Switching On :", "OFF");
            mWidget.slider0(    xa, ya, xb, bts, 0,0,0,0,    0,13,15,15,  1,0,1,d, mItem.item[n][13], 99, 0, 1,    "Set Event When Switching Off:", "OFF");
         break;
         case 7: // mine
            mWidget.buttonp(    xa, ya, xb, bts, 22,0,0,0,  0,13,15, 0,  1,0,1,d, mItem.item[n][3]); // stat | fall | carry
            ya+=4; // spacer
            mWidget.slideri(    xa, ya, xb, bts, 0,0,0,0,   0,12,15,15,  1,0,1,d, mItem.item[n][8], 200, 1, 1,  "Mine Damage:");
         break;
         case 8: // bomb
            mWidget.buttonp(    xa, ya, xb, bts, 22,0,0,0,  0,13,15, 0,  1,0,1,d, mItem.item[n][3]); // stat | fall | carry
            ya+=4; // spacer
            mWidget.button(     xa, ya, xb, bts, 77,n,0,0,  0,12,15, 0,  1,0,1,d);          // fuse timer / remote detonator
            if (!mItem.item[n][12])
               mWidget.slideri( xa, ya, xb, bts, 0,0,0,0,   0,12,15,15,  1,0,1,d, mItem.item[n][9], 2000, 1,  1,  "Fuse Length:");
            ya+=4; // spacer
            mWidget.toggle(     xa, ya, xb, bts, 0,0,0,0,   0, 0, 0, 0,  1,0,1,d, mItem.item[n][11], "Sticky:Off", "Sticky:On", 15, 15, 4, 4);
            ya+=4; // spacer
            mWidget.slideri(    xa, ya, xb, bts, 0,0,0,0,   0,14,15,15,  1,0,1,d, mItem.item[n][7], 1200, 20, 1,  "Damage Range:");
         break;
         case 10: // message
            mWidget.togglf(     xa, ya, xb, bts, 0,0,0,0,   0,0,0,0,    1,0,1,d, mItem.item[n][2], PM_ITEM_PMSG_SHOW_ALWAYS , "Show Always:OFF", "Show Always:ON",  15+dim, 15, 12+dim, 12);
            ya+=4; // spacer
            if (!(mItem.item[n][2] & PM_ITEM_PMSG_SHOW_ALWAYS))
            {
               mWidget.slideri(xa, ya, xb, bts,     0,0,0,0,   0,8,15,15,  1,0,1,d, mItem.item[n][12], 400, 0, 1,  "Message display time:");
               ya+=4; // spacer
               mWidget.togglf(     xa, ya, xb, bts, 0,0,0,0,   0,0,0,0,    1,0,1,d, mItem.item[n][2], PM_ITEM_PMSG_SHOW_SCROLL , "Show Scroll:OFF", "Show Scroll:ON",  15+dim, 15, 9+dim, 9);
               ya+=4; // spacer
               mWidget.slider0(    xa, ya, xb, bts, 0,0,0,0,   0,13,15,15, 1,0,1,d, mItem.item[n][1], 99, 0, 1, "Event Trigger:", "OFF");
               if (mWidget.buttont(xa, ya, xb, bts, 0,0,0,0,   0,13,15,0,  1,0,1,d, "Set Trigger")) mTriggerEvent.find_event_sender_for_obj(2, n, 0, 0);
               ya+=4; // spacer
            }
            if (mWidget.buttont(xa, ya, xb, bts, 0,0,0,0,   0,10,15,0,  1,0,1,d, "Get New Message Area")) mMiscFnx.get_block_range("Block Range", &mItem.item[n][6], &mItem.item[n][7], &mItem.item[n][8], &mItem.item[n][9], 1);
            ya+=4; // spacer
            mWidget.button(xa, ya, xb, bts, 7,n,0,0,   0,3,15,0,   1,0,1,d); // Set Message Frame
            ya+=4; // spacer
            mWidget.colsel(     xa, ya, xb, bts, 2,n,0,0,   0, 0, 0, 0,  0,0,1,d);  // frame color select
            mWidget.colsel(     xa, ya, xb, bts, 3,n,0,0,   0, 0, 0, 0,  0,0,1,d);  // text color select
            if (mWidget.buttont(xa, ya, xb, bts, 0,0,0,0,   0,15,13, 0,  1,0,1,d, "Edit Message")) mItem.edit_pmsg_text(n, 0);
            ya+=bts*2; // leave space for OK and Cancel buttons
            // draw the current one last to ensure it is on top
            mWM.mW[7].pop_msg_viewer_pos = ya+bts/2+2;
            mItem.draw_pop_message(n, 1, (xa+xb)/2, mWM.mW[7].pop_msg_viewer_pos, 0, 0, mItem.pmsgtext[n]); // show the message
            ya+=bts*8;
         break;
         case 11: // rocket
            mWidget.buttonp(    xa, ya, xb, bts, 23,0,0,0,  0,13,15,0,  1,0,1,d, mItem.item[n][3]); // stat | fall | | ride through door
            ya+=4; // spacer
            if (mWidget.buttont(xa, ya, xb, bts, 0,0,0,0,   0,10,15,0,  1,0,1,d, "Set Initial Direction")) mMiscFnx.getxy("Initial Direction", 97, 11, n);
            ya+=4; // spacer
            mWidget.slideri(    xa, ya, xb, bts, 0,0,0,0,   0,12,15,15, 1,0,1,d, mItem.item[n][8], 20,  1, 1,  "Maximum Speed:");
            mWidget.slideri(    xa, ya, xb, bts, 0,0,0,0,   0,12,15,15, 1,0,1,d, mItem.item[n][9], 200, 1, 1,  "Acceleration:");
            mWidget.slideri(    xa, ya, xb, bts, 0,0,0,0,   0,12,15,15, 1,0,1,d, mItem.item[n][6], 100, 0, 1,  "Steerability:");
            ya+=4; // spacer
            mWidget.slideri(    xa, ya, xb, bts, 0,0,0,0,   0,14,15,15, 1,0,1,d, mItem.item[n][7], 1200, 20, 1,  "Damage Range:");

         break;
         case 14: // switch
            mWidget.buttonp(    xa, ya, xb, bts, 22,0,0,0,  0,13,15,14, 1,0,1,d, mItem.item[n][3]); // stat | fall | carry
            ya+=4; // spacer
            if (mWidget.buttont(xa, ya, xb, bts, 0,0,0,0,   0,10,15,0,  1,0,1,d, "Get New Block Range")) mMiscFnx.get_block_range("Block Range", &mItem.item[n][6], &mItem.item[n][7], &mItem.item[n][8], &mItem.item[n][9], 1);
            ya+=4; // spacer
            mWidget.togglf(     xa, ya, xb, bts, 0,0,0,0,   0,0,0,0,    1,0,1,d, mItem.item[n][10], 0b0001 , "Green:OFF", "Green:ON",  15+dim, 15, 9+dim, 9);
            mWidget.togglf(     xa, ya, xb, bts, 0,0,0,0,   0,0,0,0,    1,0,1,d, mItem.item[n][10], 0b0010 , "Red:OFF",   "Red:ON",    15+dim, 15, 10+dim, 10);
            mWidget.togglf(     xa, ya, xb, bts, 0,0,0,0,   0,0,0,0,    1,0,1,d, mItem.item[n][10], 0b0100 , "Blue:OFF",  "Blue:ON",   15+dim, 15, 12+dim, 12);
            mWidget.togglf(     xa, ya, xb, bts, 0,0,0,0,   0,0,0,0,    1,0,1,d, mItem.item[n][10], 0b1000 , "Purple:OFF","Purple:ON", 15+dim, 15, 8+dim, 8);
            set_switch_tile(n);
         break;
         case 15: // sproingy
            mWidget.buttonp(    xa, ya, xb, bts, 22,0,0,0,  0,13,15, 0, 1,0,1,d, mItem.item[n][3]); // stat | fall | carry
            ya+=4; // spacer
            mWidget.slideri(    xa, ya, xb, bts, 0,0,0,0,   0,12,15,15, 1,0,1,d, mItem.item[n][7], 200, 40, 1, "Sproinginess:" );
         break;
         case 12: // warp
            mWidget.buttonp(    xa, ya, xb, bts, 21,0,0,0,  0,13,15, 0,  1,0,1,d, mItem.item[n][3]); // stat | fall
            ya+=4; // spacer
            mWidget.slideri(    xa, ya, xb, bts, 0,0,0,0,   0,12,15,15,  1,0,1,d, mItem.item[n][8], 300, 1, 1, "Warp Level:" );
         break;


         case 13: // timer
         {
            int state, t1_mode, t2_mode, t1_op_mode, t2_op_mode, time;
            mItem.get_timer_flags(mItem.item[n][3], state, t1_mode, t2_mode, t1_op_mode, t2_op_mode, time);

            mWidget.buttonp(       xa, ya, xb, bts, 83,0,0,0,   0,11,15,0,  1,0,1,d, mItem.item[n][2]); // draw mode
            if (mWidget.buttont(   xa, ya, xb, bts, 0,0,0,0,    0,11,15,0,  1,0,1,d, "Get New Display Area")) mMiscFnx.get_block_range("Display Area", &mItem.item[n][6], &mItem.item[n][7], &mItem.item[n][8], &mItem.item[n][9], 1);

            ya+=4; int p = 13; // spacer
            mWidget.slider0(       xa, ya, xb, bts, 0,0,0,0,    0,p,15,15,  1,0,1,d, mItem.item[n][10], 1000, 0, 1, "Timer 1:" , "OFF");
            if (mItem.item[n][10])
            {
               mWidget.buttonp(    xa, ya, xb, bts, 160,0,0,0,  0,p,15, 0,  1,0,1,d, t1_mode); // mode
               mWidget.slider0(    xa, ya, xb, bts, 0,0,0,0,    0,p,15,15,  1,0,1,d, mItem.item[n][12], 99, 0, 1, "Input Event:", "OFF");
               if (mWidget.buttont(xa, ya, xb, bts, 0,0,0,0,    0,p,15,0,   1,0,1,d, "Set Input Event")) mTriggerEvent.find_event_sender_for_obj(2, n, 1, t1_mode);
               mWidget.slider0(    xa, ya, xb, bts, 0,0,0,0,    0,p,15,15,  1,0,1,d, mItem.item[n][13], 99, 0, 1, "Output Event:", "OFF");
               mWidget.buttonp(    xa, ya, xb, bts, 85,0,0,0,   0,p,15,0,   1,0,1,d, t1_op_mode); // output mode
            }
            ya+=4; p = 14; // spacer
            mWidget.slider0(       xa, ya, xb, bts, 0,0,0,0,    0,p,15,15,  1,0,1,d, mItem.item[n][11], 1000, 0, 1, "Timer 2:", "OFF");
            if (mItem.item[n][11])
            {
               mWidget.buttonp(    xa, ya, xb, bts, 160,0,0,0,  0,p,15, 0,  1,0,1,d, t2_mode); // mode
               mWidget.slider0(    xa, ya, xb, bts, 0,0,0,0,    0,p,15,15,  1,0,1,d, mItem.item[n][14], 99, 0, 1, "Input Event:", "OFF");
               if (mWidget.buttont(xa, ya, xb, bts, 0,0,0,0,    0,p,15,0,   1,0,1,d, "Set Input Event")) mTriggerEvent.find_event_sender_for_obj(2, n, 2, t2_mode);
               mWidget.slider0(    xa, ya, xb, bts, 0,0,0,0,    0,p,15,15,  1,0,1,d, mItem.item[n][15], 99, 0, 1, "Output Event :", "OFF");
               mWidget.buttonp(    xa, ya, xb, bts, 85,0,0,0,   0,p,15,0,   1,0,1,d, t2_op_mode); // output mode
            }
            state = 1;
            time = mItem.item[n][10]; // always start with full t1_timer
            mItem.set_timer_flags(mItem.item[n][3], state, t1_mode, t2_mode, t1_op_mode, t2_op_mode, time);
         }
         break;
         case 9: // trigger
         {
            if (mWidget.buttont(xa, ya, xb, bts, 0,0,0,0,    0,14,15,0,   1,0,1,d, "Get New Trigger Field")) mMiscFnx.get_block_range("Trigger Rectangle", &mItem.item[n][6], &mItem.item[n][7], &mItem.item[n][8], &mItem.item[n][9], 1);
            ya+=4; // spacer
            // draw trigger field on/off with optional color select if on
            if (mWidget.togglf( xa, ya, xb, bts, 0,0,0,0,    0,0,0,0,     1,0,1,d, mItem.item[n][3], PM_ITEM_TRIGGER_DRAW_ON, "Draw Trigger Field:OFF","Draw Trigger Field:ON", 15+dim, 15, 15+dim, mItem.item[n][2]))
               mWidget.colsel(  xa, ya, xb, bts, 6,n,0,0,    0,0,0,0,     0,0,1,d); // color select
            ya+=4; // spacer
            mWidget.togglf(     xa, ya, xb, bts, 0,0,0,0,    0,0,0,0,     1,0,1,d, mItem.item[n][3], PM_ITEM_TRIGGER_PLAYER, "Triggered by Players:OFF       ", "Triggered by Players:ON        ", 15+dim, 15, 9+dim, 9);
            mWidget.togglf(     xa, ya, xb, bts, 0,0,0,0,    0,0,0,0,     1,0,1,d, mItem.item[n][3], PM_ITEM_TRIGGER_ENEMY,  "Triggered by Enemies:OFF       ", "Triggered by Enemies:ON        ", 15+dim, 15, 9+dim, 9);
            mWidget.togglf(     xa, ya, xb, bts, 0,0,0,0,    0,0,0,0,     1,0,1,d, mItem.item[n][3], PM_ITEM_TRIGGER_ITEM,   "Triggered by Items:OFF         ", "Triggered by Items:ON          ", 15+dim, 15, 9+dim, 9);
            mWidget.togglf(     xa, ya, xb, bts, 0,0,0,0,    0,0,0,0,     1,0,1,d, mItem.item[n][3], PM_ITEM_TRIGGER_PSHOT,  "Triggered by Player's Shots:OFF", "Triggered by Player's Shots:ON ", 15+dim, 15, 9+dim, 9);
            mWidget.togglf(     xa, ya, xb, bts, 0,0,0,0,    0,0,0,0,     1,0,1,d, mItem.item[n][3], PM_ITEM_TRIGGER_ESHOT,  "Triggered by Enemy's Shots:OFF ", "Triggered by Enemy's Shots:ON  ", 15+dim, 15, 9+dim, 9);
            ya+=4; // spacer
            mWidget.slider0(    xa, ya, xb, bts, 0,0,0,0,    0,13,15,15,  1,0,1,d, mItem.item[n][11], 99, 0, 1,    "Set Event Always While On   :", "OFF");
            mWidget.slider0(    xa, ya, xb, bts, 0,0,0,0,    0,13,15,15,  1,0,1,d, mItem.item[n][12], 99, 0, 1,    "Set Event Always While Off  :", "OFF");
            mWidget.slider0(    xa, ya, xb, bts, 0,0,0,0,    0,13,15,15,  1,0,1,d, mItem.item[n][13], 99, 0, 1,    "Set Event When Switching On :", "OFF");
            mWidget.slider0(    xa, ya, xb, bts, 0,0,0,0,    0,13,15,15,  1,0,1,d, mItem.item[n][14], 99, 0, 1,    "Set Event When Switching Off:", "OFF");
            ya+=4; // spacer
            if (mWidget.togglf( xa, ya, xb, bts, 0,0,0,0,    0,0,0,0,     1,0,1,d, mItem.item[n][3], PM_ITEM_TRIGGER_LIFT_ON, "Follows Lift:OFF","Follows Lift:ON ", 15+dim, 15, 6+dim, 6))
            {
               mWidget.slideri( xa, ya, xb, bts, 0,0,0,0,    0,6,15,15,   1,0,1,d,mItem.item[n][10], 39, 0, 1, "Lift Number:" );  // Lift Number
               mWidget.button(  xa, ya, xb, bts, 211,n,0,0,  0,6,15,0,    1,0,1,d);  // X Alignment
               mWidget.button(  xa, ya, xb, bts, 212,n,0,0,  0,6,15,0,    1,0,1,d);  // Y Alignment
            }
         }
         break;





         case 16: // block manip
         {

            if (mWidget.buttont(xa, ya, xb, bts, 0,0,0,0,    0,14,15,14,  1,0,1,d, "Get New Block Manip Field")) mMiscFnx.get_block_range("Block Manip Rectangle", &mItem.item[n][6], &mItem.item[n][7], &mItem.item[n][8], &mItem.item[n][9], 1);
            ya+=4; // spacer
            if (mWidget.toggle( xa, ya, xb, bts, 0,0,0,0,    0,0,0,0,     1,0,1,d, mItem.item[n][2], "Draw Block Manip Field:OFF", "Draw Block Manip Field:ON ", 15, 15, 15+96, mItem.item[n][12]))
               mWidget.colsel(  xa, ya, xb, bts, 7,n,0,0,    0,0,0,0,     0,0,1,d);               // color select
            ya+=4; // spacer
            mWidget.buttonp(    xa, ya, xb, bts, 301,0,0,0,  0,14,15,0,   1,0,1,d, mItem.item[n][3]);   // mode

            if (mItem.item[n][3]) // all modes except 0
            {
               ya+=2; // spacer

               if (mItem.item[n][3] == 4)
               {
                  if (mWidget.buttont(xa, ya, xb, bts,  0,0,0,0,  0,11,15, 0,  1,0,1,d, "Set Copy Source")) mMiscFnx.getxy("Copy Source", 95, 0, n);
                  ya+=2; // spacer
               }
               else
               {
                  ya+=2; // spacer
                  mWidget.button(     xa, ya, xb, bts, 310,n,0,0,  0, 8,15,0,   1,0,0,d); // block 1
                  ya+=22; // spacer

                  if (mItem.item[n][3] > 1)
                  {
                     mWidget.button(     xa, ya, xb, bts, 311,n,0,0,  0, 8,15,0,   1,0,0,d); // block 2
                     ya+=22; // spacer
                  }
               }


               mWidget.slider0(    xa, ya, xb, bts, 0,0,0,0,    0,13,15,15,  1,0,1,d, mItem.item[n][1], 99, 0, 1,    "Event Trigger:", "OFF");
               if (mWidget.buttont(xa, ya, xb, bts, 0,0,0,0,    0,13,15,0,   1,0,1,d, "Set Trigger")) mTriggerEvent.find_event_sender_for_obj(2, n, 0, 0);



            }
         }
         break;
         case 17: // block damage
         {
            if (mWidget.buttont(   xa, ya, xb, bts, 0,0,0,0,   0,10,15,0,  1,0,1,d, "Get New Block Damage Field")) mMiscFnx.get_block_range("Block Damage Rectangle", &mItem.item[n][6], &mItem.item[n][7], &mItem.item[n][8], &mItem.item[n][9], 1);
            ya+=4; // spacer
            mWidget.buttonp(       xa, ya, xb, bts, 404,0,0,0, 0, 8,15,0,  1,0,1,d, mItem.item[n][2]); // damage draw mode
            ya+=4; // spacer
            int p=7; // mode color
            mWidget.buttonp(       xa, ya, xb, bts, 402,n,0,0, 0,p,15,0,   1,0,1,d, mItem.item[n][11]); // mode
            int MODE = mItem.item[n][11];

            ya+=4; // spacer
            if (MODE == 1) // toggle
               mWidget.togglf(     xa, ya, xb, bts, 0,0,0,0,   0,0,0,0,    1,0,1,d, mItem.item[n][3], PM_ITEM_DAMAGE_CURR, "Initial State:OFF","Initial State:ON ", 15+dim, 15, p+dim, p);
            if ((MODE == 1) || (MODE == 2) || (MODE == 3)) // Mode 1, 2, and 3
            {
               ya+=4; // spacer
               mWidget.slider0(    xa, ya, xb, bts, 0,0,0,0,   0,13,15,15, 1,0,1,d, mItem.item[n][1], 99, 0, 1, "Event Trigger:", "OFF");
               if (mWidget.buttont(xa, ya, xb, bts, 0,0,0,0,   0,13,15,0,  1,0,1,d, "Set Trigger")) mTriggerEvent.find_event_sender_for_obj(2, n, 0, 0);
            }
            ya+=4; // spacer
            if (mWidget.togglf(    xa, ya, xb, bts, 0,0,0,0,   0,0,0,0,    1,0,1,d, mItem.item[n][3], PM_ITEM_DAMAGE_PLAYER,  "Affects Players:OFF",        "Affects Players:ON",        15+dim, 15, 10+dim, 10))
               if (!mWidget.togglf(xa, ya, xb, bts, 0,0,0,0,   0,0,0,0,    1,0,1,d, mItem.item[n][3], PM_ITEM_DAMAGE_INSTGIB, "Player Instant Death:OFF",   "Player Instant Death:ON",   15+dim, 15, 10+dim, 10))
                  mWidget.slideri( xa, ya, xb, bts, 0,0,0,0,   0,10,15,15, 1,0,1,d, mItem.item[n][15], 2000, -2000, 1,        "Player Damage:" );
            mWidget.togglf(        xa, ya, xb, bts, 0,0,0,0,   0,0,0,0,    1,0,1,d, mItem.item[n][3], PM_ITEM_DAMAGE_ENEMY,   "Affects Enemies:OFF",        "Affects Enemies:ON",        15+dim, 15, 10+dim, 10);
            mWidget.togglf(        xa, ya, xb, bts, 0,0,0,0,   0,0,0,0,    1,0,1,d, mItem.item[n][3], PM_ITEM_DAMAGE_ITEM,    "Affects Items:OFF",          "Affects Items:ON",          15+dim, 15, 10+dim, 10);
            mWidget.togglf(        xa, ya, xb, bts, 0,0,0,0,   0,0,0,0,    1,0,1,d, mItem.item[n][3], PM_ITEM_DAMAGE_PSHOT,   "Affects Player's Shots:OFF", "Affects Player's Shots:ON", 15+dim, 15, 10+dim, 10);
            mWidget.togglf(        xa, ya, xb, bts, 0,0,0,0,   0,0,0,0,    1,0,1,d, mItem.item[n][3], PM_ITEM_DAMAGE_ESHOT,   "Affects Enemy's Shots:OFF",  "Affects Enemy's Shots:ON",  15+dim, 15, 10+dim, 10);
            ya+=4; // spacer
            if (mWidget.togglf(    xa, ya, xb, bts, 0,0,0,0,   0,0,0,0,    1,0,1,d, mItem.item[n][3], PM_ITEM_DAMAGE_LIFT_ON, "Follows Lift:OFF",           "Follows Lift:ON ",          15+dim, 15, 6+dim, 6))
            {
               mWidget.slideri(    xa, ya, xb, bts, 0,0,0,0,   0,6,15,15,  1,0,1,d,mItem.item[n][10], 39, 0, 1, "Lift Number:" );
               mWidget.button(     xa, ya, xb, bts, 411,n,0,0, 0,6,15,0,   1,0,1,d);  // X Alignment
               mWidget.button(     xa, ya, xb, bts, 412,n,0,0, 0,6,15,0,   1,0,1,d);  // Y Alignment
            }
         }
         break;
      }
   }
   // set height
   if (obt != 4)
   {
      int llo = (3 - mWM.mW[7].num_legend_lines)*8; // legend line offset
      mWM.mW[7].h = ya-y1+32-llo;
      mWM.mW[7].set_size(mWM.mW[7].w, mWM.mW[7].h);
   }
}


void mwWindow::ov_draw_overlay_rectangle_and_crosshairs(int x1, int y1, int w, int h, int color, int crosshairs)
{
   float thickness = 2.0;
   int x2 = x1+w;
   int y2 = y1+h;
   al_draw_rectangle(x1+1, y1+1, x2-1, y2-1, mColor.pc[color], thickness);
   if (crosshairs)
   {
      int xc = (x1+x2)/2;
      int yc = (y1+y2)/2;
      al_draw_line(0, yc, 1999, yc, mColor.pc[color], thickness);
      al_draw_line(xc, 0, xc, 1999, mColor.pc[color], thickness);
   }
}

void mwWindow::ov_draw_overlays(int legend_highlight)
{
   int obt = mWM.mW[7].obt;
   int num = mWM.mW[7].num;

   al_set_target_bitmap(mBitmap.level_buffer);

   if (obt == 4)  // lifts
   {
      int lift = num;
      int step = mLift.cur[lift].current_step;

      if ((mLift.stp[lift][step].type & 31) != 1) step = mLift.find_previous_move_step(lift, step);

      int color = (mLift.stp[lift][step].type >> 28) & 15;

      int x1 = mLift.stp[lift][step].x-1;
      int y1 = mLift.stp[lift][step].y-1;

      int w = mLift.stp[lift][step].w;
      int h = mLift.stp[lift][step].h;

      int x2 = x1 + w + 2;
      int y2 = y1 + h + 2;
      int xc = (x1 + x2) / 2;
      int yc = (y1 + y2) / 2;

      al_draw_rectangle(x1, y1, x2, y2, mColor.pc[color], 1);

      al_draw_line(xc, 0, xc, y1, mColor.pc[color], 1);
      al_draw_line(xc, y2, xc, 2000, mColor.pc[color], 1);
      al_draw_line(0, yc, x1, yc, mColor.pc[color], 1);
      al_draw_line(x2, yc, 2000, yc, mColor.pc[color], 1);
   }
   if (obt == 3)  // enemies
   {
      int type = mEnemy.Ei[num][0];
      int obj_x = mEnemy.Ef[num][0]+10;
      int obj_y = mEnemy.Ef[num][1]+10;

      int color = 13;
      if (legend_highlight == 1) color = mColor.flash_color;
      mMiscFnx.crosshairs_full(obj_x, obj_y, color, 1);

      if ((type == 2) || (type == 3) || (type == 6) || (type == 8)) // // draw yellow shot prox circle for cannon, archwagon, flapper, trakbot
      {
         (legend_highlight == 2) ? color = mColor.flash_color : color = 14;
         al_draw_circle(obj_x, obj_y, mEnemy.Ei[num][17], mColor.pc[color], 1);
      }
      if (type == 6) // flapper
      {
         (legend_highlight == 3) ? color = mColor.flash_color : color = 10;

         // draw red height above player line
         int hab = mEnemy.Ei[num][20];
         al_draw_line(obj_x-40, obj_y+hab, obj_x+40, obj_y+hab, mColor.pc[color], 3);

         // draw flap height
         int fh = mEnemy.Ei[num][21];
         al_draw_line(obj_x-60, obj_y+fh, obj_x+60, obj_y+fh, mColor.pc[12], 1);
         al_draw_line(obj_x-60, obj_y-fh, obj_x+60, obj_y-fh, mColor.pc[12], 1);
      }
      if (type == 7)
      {
         mEnemy.draw_vinepod_controls(num, legend_highlight);
         mTriggerEvent.find_and_show_event_links(obt, num, 0);
      }
      if (type == 9) // cloner
      {
         int color2 = 11;
         if (legend_highlight == 2) color2 = mColor.flash_color;
         ov_draw_overlay_rectangle_and_crosshairs(mEnemy.Ei[num][15], mEnemy.Ei[num][16], mEnemy.Ei[num][19], mEnemy.Ei[num][20], color2, 1);

         int color3 = 10;
         if (legend_highlight == 3) color3 = mColor.flash_color;
         ov_draw_overlay_rectangle_and_crosshairs(mEnemy.Ei[num][17], mEnemy.Ei[num][18], mEnemy.Ei[num][19], mEnemy.Ei[num][20], color3, 1);
         mTriggerEvent.find_and_show_event_links(obt, num, 0);
      }
   }
   if (obt == 2)  // items
   {
      int type = mItem.item[num][0];
      int obj_x = mItem.item[num][4]+10;
      int obj_y = mItem.item[num][5]+10;
      int color = 13;
      if (legend_highlight == 1) color = mColor.flash_color;
      mMiscFnx.crosshairs_full(obj_x, obj_y, color, 1);
      switch (type)
      {
         case 1: // door
         {
            (legend_highlight == 2) ? color = mColor.flash_color : color = 10;
            if (mItem.item[num][8] == 0)  // exit only, no destination
            {
               // find src door(s)
               int num_src = 0;
               for (int i=0; i<500; i++)
                  if ((mItem.item[i][0] == 1) && (mItem.item[i][9] == num))
                  {
                     num_src++;
                     int x2 = mItem.item[i][4]+10;
                     int y2 = mItem.item[i][5]+10;
                     mMiscFnx.crosshairs_full(x2, y2, color, 1);
                     al_draw_line(obj_x, obj_y, x2, y2, mColor.pc[color], 1);
                  }
            }
            if (mItem.item[num][8] == 1)  // draw destination
            {
               // dest item
               int di = mItem.item[num][9];
               int x2 = mItem.item[di][4]+10;
               int y2 = mItem.item[di][5]+10;
               mMiscFnx.crosshairs_full(x2, y2, color, 1);
               al_draw_line(obj_x, obj_y, x2, y2, mColor.pc[color], 1);
            }
         }
         break;
         case 4: // key
            (legend_highlight == 2) ? color = mColor.flash_color : color = 10;
            ov_draw_overlay_rectangle_and_crosshairs(mItem.item[num][6], mItem.item[num][7], mItem.item[num][8], mItem.item[num][9], color, 1);
            mItem.proc_key_block_range(num, 2); // show blocks that will be affected
         break;
         case 14: // switch
            (legend_highlight == 2) ? color = mColor.flash_color : color = 10;
            ov_draw_overlay_rectangle_and_crosshairs(mItem.item[num][6], mItem.item[num][7], mItem.item[num][8], mItem.item[num][9], color, 1);
            mItem.proc_switch_block_range(num, 2); // show blocks that will be affected
         break;
         case 8: // bomb
            (legend_highlight == 2) ? color = mColor.flash_color : color = 14;
            al_draw_circle(obj_x, obj_y, mItem.item[num][7], mColor.pc[color], 1); // draw yellow bomb damage
         break;
         case 9: // trigger
            (legend_highlight == 2) ? color = mColor.flash_color : color = 14;
            ov_draw_overlay_rectangle_and_crosshairs(mItem.item[num][6], mItem.item[num][7], mItem.item[num][8], mItem.item[num][9], color, 1);
            mTriggerEvent.find_and_show_event_links(obt, num, 0);
         break;
         case 6: // orb
            mTriggerEvent.find_and_show_event_links(obt, num, 0);
         break;
         case 10: // pmsg
         {
            (legend_highlight == 2) ? color = mColor.flash_color : color = 10;
            int mx = mItem.item[num][6];
            int my = mItem.item[num][7];
            int mw = mItem.item[num][8];
            int mh = mItem.item[num][9];
            ov_draw_overlay_rectangle_and_crosshairs(mx, my, mw, mh, color, 1);
            al_draw_textf(mFont.pr8, mColor.pc[15], mx+mw/2, my-10,   ALLEGRO_ALIGN_CENTRE, "x:%d y:%d", mx, my);
            al_draw_textf(mFont.pr8, mColor.pc[15], mx+mw/2, my+mh+2, ALLEGRO_ALIGN_CENTRE, "w:%d h:%d", mw, mh);
            mTriggerEvent.find_and_show_event_links(obt, num, 0);
         }
         break;
         case 11: // rocket
            (legend_highlight == 2) ? color = mColor.flash_color : color = 14;
            al_draw_circle(obj_x, obj_y, mItem.item[num][7], mColor.pc[color], 1); // draw yellow bomb damage
         break;
         case 13: // timer
            (legend_highlight == 2) ? color = mColor.flash_color : color = 11;
            ov_draw_overlay_rectangle_and_crosshairs(mItem.item[num][6], mItem.item[num][7], mItem.item[num][8], mItem.item[num][9], color, 1);
            mTriggerEvent.find_and_show_event_links(obt, num, 0);
         break;
         case 15: // sproingy
         {
            (legend_highlight == 2) ? color = mColor.flash_color : color = 14;
            int y = mMiscFnx.get_sproingy_jump_height(num);
            mMiscFnx.crosshairs_full(obj_x, obj_y-y, color, 1);
         }
         break;
         case 16: // block manip
            (legend_highlight == 2) ? color = mColor.flash_color : color = 12;
            ov_draw_overlay_rectangle_and_crosshairs(mItem.item[num][6], mItem.item[num][7], mItem.item[num][8], mItem.item[num][9], color, 1);

            if (mItem.item[num][3] == 4)
            {
               (legend_highlight == 3) ? color = mColor.flash_color : color = 11;
               ov_draw_overlay_rectangle_and_crosshairs(mItem.item[num][13], mItem.item[num][14], mItem.item[num][8], mItem.item[num][9], color, 1);
            }



            mTriggerEvent.find_and_show_event_links(obt, num, 0);
         break;
         case 17: // block damage
            (legend_highlight == 2) ? color = mColor.flash_color : color = 10;
            ov_draw_overlay_rectangle_and_crosshairs(mItem.item[num][6], mItem.item[num][7], mItem.item[num][8], mItem.item[num][9], color, 1);
            mTriggerEvent.find_and_show_event_links(obt, num, 0);
         break;

      } // end of switch case
   }
}
void mwWindow::ov_process_mouse(void)
{

   int lift=0, step=0;
   if (mWM.mW[7].obt == 4)
   {
      lift = num;
      step = mLift.cur[lift].current_step;
   }

   int mouse_on_obj = 0;

   // vinepod only
   int mouse_on_vpodx = 0;
   int mouse_on_vpod1 = 0;
   int mouse_on_vpod2 = 0;
   int mouse_on_tb_ul = 0;
   int mouse_on_tb_lr = 0;

   // cloner only
   int mouse_on_csb_ul = 0;
   int mouse_on_csb_lr = 0;
   int mouse_on_cdb_ul = 0;

   // enemy shot prox
   int mouse_on_esp = 0;

   int mouse_on_sp = 0;
   int mouse_on_bmb = 0;
   int mouse_on_bms = 0;


   int mouse_on_item_sec_ul = 0;
   int mouse_on_item_sec_lr = 0;

   int mouse_on_lift = 0;

   int mouse_move = 0;
   int mouse_adj = 0;

   int mst = 6;      // mouse select threshold
   int msn = 10-mst; // mouse select negative threshold
   int msp = 10+mst; // mouse select positive threshold


   // -----------------------------------------------------------
   // --  detect if mouse pointer is on enemy
   // -----------------------------------------------------------
   for (int e=0; e<100; e++)
      if (mEnemy.Ei[e][0])
      {
         int ex = mEnemy.Ef[e][0];
         int ey = mEnemy.Ef[e][1];
         int type = mEnemy.Ei[e][0];

         // check to see if we can set this object to be the current object
         if ((mWM.hx>ex+msn) && (mWM.hx<ex+msp) && (mWM.hy>ey+msn) && (mWM.hy<ey+msp) && (!mWM.mW[7].viewer_lock) && (!mInput.key[MAP_LOCK_KEY][0]) && (mWM.obj_filter[3][type]))
         {
            // set this enemy to current object
            mWM.mW[7].obt = 3;
            mWM.mW[7].num = e;
            mouse_move = 1;
            mouse_on_obj = 1;
         }

         // if this object is already current object
         if ((mWM.hx>ex+msn) && (mWM.hx<ex+msp) && (mWM.hy>ey+msn) && (mWM.hy<ey+msp) && (mWM.mW[7].obt == 3) && (mWM.mW[7].num == e))
         {
            mouse_move = 1;
            mouse_on_obj = 1;
         }
      }

   // if current object is enemy, check for secondaries
   if (mWM.mW[7].obt == 3)
   {
      int e = mWM.mW[7].num;
      int type = mEnemy.Ei[e][0];
      if ((type == 2) || (type == 3) || (type == 6) || (type == 8)) // cannon, archwagon, cannon, flapper and trakbot shot prox
      {
         float x0 = mEnemy.Ef[e][0]+10;                       // center of enemy
         float y0 = mEnemy.Ef[e][1]+10;
         float fx = mWM.hx;                                   // hi-res mouse pos
         float fy = mWM.hy;
         float dst = sqrt(pow((x0-fx), 2) + pow((y0-fy), 2)); // distance from mouse
         float bdr = mEnemy.Ei[e][17];                        // prox radius
         float dif = dst-bdr;                                 // difference
         if ((dif < 3 ) && (dif > -3))                        // mouse is on prox circle
         {
            mouse_on_esp = 1;
            mouse_move = 1;
         }
      }

      if (type == 7) // vinepod
      {
         // vinepod trigger box
         int x1 = mEnemy.Ei[e][11];
         int y1 = mEnemy.Ei[e][12];
         int x2 = mEnemy.Ei[e][11]+mEnemy.Ei[e][13]+20;
         int y2 = mEnemy.Ei[e][12]+mEnemy.Ei[e][14]+20;
         if ((mWM.hx>x1-mst) && (mWM.hx<x1+mst) && (mWM.hy>y1-mst) && (mWM.hy<y1+mst)) // upper left corner (move)
         {
            mouse_on_tb_ul = 1;
            mouse_move = 1;
         }
         if ((mWM.hx>x2-mst) && (mWM.hx<x2+mst) && (mWM.hy>y2-mst) && (mWM.hy<y2+mst)) // lower right corner (resize)
         {
            mouse_on_tb_lr = 1;
            mouse_adj = 1;
         }

         // vinepod extended position
         int px = mEnemy.Ei[e][9];
         int py = mEnemy.Ei[e][10];
         if ((mWM.hx>px+msn) && (mWM.hx<px+msp) && (mWM.hy>py+msn) && (mWM.hy<py+msp))
         {
            mouse_move = 1;
            mouse_on_vpodx = 1;
         }

         if (mEnemy.Ei[e][20] & PM_ENEMY_VINEPOD_USE_SPLINE)
         {
            // vinepod control point 1
            px = mEnemy.Ei[e][5];
            py = mEnemy.Ei[e][6];
            if ((mWM.hx>px+msn) && (mWM.hx<px+msp) && (mWM.hy>py+msn) && (mWM.hy<py+msp))
            {
               mouse_move = 1;
               mouse_on_vpod1 = 1;
            }

            // vinepod control point 2
            px = mEnemy.Ei[e][7];
            py = mEnemy.Ei[e][8];
            if ((mWM.hx>px+msn) && (mWM.hx<px+msp) && (mWM.hy>py+msn) && (mWM.hy<py+msp))
            {
               mouse_move = 1;
               mouse_on_vpod2 = 1;
            }
         }
      }

      if (type == 9) // cloner source and destination boxes
      {
         int w  = mEnemy.Ei[e][19];  // width
         int h  = mEnemy.Ei[e][20];  // height
         int x3 = mEnemy.Ei[e][17];  // dest box
         int y3 = mEnemy.Ei[e][18];
         int x1 = mEnemy.Ei[e][15];  // source box
         int y1 = mEnemy.Ei[e][16];
         int x2 = x1 + w - 1;
         int y2 = y1 + h - 1;
         if ((mWM.hx>x1-mst) && (mWM.hx<x1+mst) && (mWM.hy>y1-mst) && (mWM.hy<y1+mst)) // source upper left corner (move)
         {
            mouse_on_csb_ul = 1;
            mouse_move = 1;
         }
         if ((mWM.hx>x2-mst) && (mWM.hx<x2+mst) && (mWM.hy>y2-mst) && (mWM.hy<y2+mst)) // source lower right corner (re-size)
         {
            mouse_on_csb_lr = 1;
            mouse_adj = 1;
         }
         if ((mWM.hx>x3-mst) && (mWM.hx<x3+mst) && (mWM.hy>y3-mst) && (mWM.hy<y3+mst)) // destination upper left corner (move)
         {
            mouse_on_cdb_ul = 1;
            mouse_move = 1;
         }
      }
   }

   // -----------------------------------------------------------
   // --  detect if mouse pointer is on item
   // -----------------------------------------------------------
   for (int b=0; b<500; b++)
      if (mItem.item[b][0])
      {
         int ix = mItem.item[b][4];
         int iy = mItem.item[b][5];
         int type = mItem.item[b][0];
         // check to see if we can set this object to be the current object
         if ((mWM.hx>ix+msn) && (mWM.hx<ix+msp) && (mWM.hy>iy+msn) && (mWM.hy<iy+msp) && (!mWM.mW[7].viewer_lock) && (!mInput.key[MAP_LOCK_KEY][0]) && (mWM.obj_filter[2][type]))
         {
            // set this item to current object
            mWM.mW[7].obt = 2;
            mWM.mW[7].num = b;
            mouse_move = 1;
            mouse_on_obj = 1;
         }
         // if this object is already current object
         if ((mWM.hx>ix+msn) && (mWM.hx<ix+msp) && (mWM.hy>iy+msn) && (mWM.hy<iy+msp) && (mWM.mW[7].obt == 2) && (mWM.mW[7].num == b))
         {
            mouse_move = 1;
            mouse_on_obj = 1;
         }
      }

   // if current object is item, check for secondaries
   if (mWM.mW[7].obt == 2)
   {
      int type = mItem.item[num][0];

      if ((type == 8) || (type == 11)) // bomb or rocket
      {
         float x0 = (float) mItem.item[num][4]+10; // get center of item location
         float y0 = (float) mItem.item[num][5]+10;
         float fx = (float) mWM.hx;
         float fy = (float) mWM.hy;
         float dst = sqrt(pow((x0-fx), 2) + pow((y0-fy), 2)); // distance from mouse to item
         float bdr = (float) mItem.item[num][7]; // bomb damage radius
         float dif = dst-bdr; // difference
         if ((dif < 3 ) && (dif > -3))  // mouse is on blast radius circle
         {
            mouse_on_bmb = 1;
            mouse_move = 1;
         }
      }
      if (type == 15) // sproingy
      {
         int x1 = mItem.item[num][4];
         int y1 = mItem.item[num][5];
         int y2 = y1 - mMiscFnx.get_sproingy_jump_height(num);
         if ((mWM.hx>x1+msn) && (mWM.hx<x1+msp) && (mWM.hy>y2+msn) && (mWM.hy<y2+msp))
         {
            mouse_on_sp = 1;
            mouse_move = 1;
         }
      }
      if (type == 16) // block manip source
      {
         int x1 = mItem.item[num][13];
         int y1 = mItem.item[num][14];
         if ((mWM.hx>x1-mst) && (mWM.hx<x1+msp) && (mWM.hy>y1-mst) && (mWM.hy<y1+mst))
         {
            mouse_on_bms = 1;
            mouse_move = 1;
         }
      }
      if (mItem.item_secondary67(type))
      {
         int x1 =      mItem.item[num][6];
         int y1 =      mItem.item[num][7];
         int x2 = x1 + mItem.item[num][8];
         int y2 = y1 + mItem.item[num][9];
         if ((mWM.hx>x1-mst) && (mWM.hx<x1+mst) && (mWM.hy>y1-mst) && (mWM.hy<y1+mst)) // upper left corner (move)
         {
            mouse_on_item_sec_ul = 1;
            mouse_move = 1;
         }
         if ((mWM.hx>x2-mst) && (mWM.hx<x2+mst) && (mWM.hy>y2-mst) && (mWM.hy<y2+mst)) // lower right corner (resize)
         {
            mouse_on_item_sec_lr = 1;
            mouse_adj = 1;
         }
      }
   }

   // -----------------------------------------------------------
   // --  detect if mouse pointer is on lift
   // -----------------------------------------------------------
   mouse_on_lift = 0;
   if (mWM.obj_filter[4][1])
   {
      if ((!mInput.key[MAP_LOCK_KEY][0]) && (!mWM.mW[7].viewer_lock)) // no lock...check all lifts and steps
      {
         for (int x=0; x<NUM_LIFTS; x++)  // cycle lifts
            if (mLift.cur[x].active)
               for (int y=0; y<mLift.cur[x].num_steps; y++)  // cycle steps
                  if ((mLift.stp[x][y].type & 31) == 1) // look for move step
                  {
                     int w =  mLift.stp[x][y].w / 2;
                     int h =  mLift.stp[x][y].h / 2;
                     int nx = mLift.stp[x][y].x + w;
                     int ny = mLift.stp[x][y].y + h;
                     if ((mWM.hx > nx - w)  && (mWM.hx < nx + w) && (mWM.hy > ny - h)  && (mWM.hy < ny + h)) // is mouse on this step ?
                     {
                        mouse_on_lift = 1;
                        mWM.mW[7].obt = 4;
                        mWM.mW[7].num = x;
                        lift = x;
                        step = y;
                        mLift.set_lift_to_step(lift, step);   // set current step in current lift
                     }
                  }
      }

      if (((mInput.key[MAP_LOCK_KEY][0]) || (mWM.mW[7].viewer_lock)) && (mWM.mW[7].obt == 4)) // locked, but locked to current lift
      {
         int x = lift; // check only current lift
         for (int y=0; y<mLift.cur[x].num_steps; y++)  // cycle steps
            if ((mLift.stp[x][y].type & 31) == 1) // look for move step
            {
               int w =  mLift.stp[x][y].w / 2;
               int h =  mLift.stp[x][y].h / 2;
               int nx = mLift.stp[x][y].x + w;
               int ny = mLift.stp[x][y].y + h;
               if ((mWM.hx > nx - w)  && (mWM.hx < nx + w) && (mWM.hy > ny - h)  && (mWM.hy < ny + h)) // is mouse on this step ?
               {
                  mouse_on_lift = 1;
                  step = y;
                  mLift.set_lift_to_step(lift, step);   // set current step in current lift
               }
            }
      }
   }
   if (mouse_on_lift)
   {
      mouse_on_obj = 1;
      if (mInput.mouse_b[2][0]) mLift.step_popup_menu(lift, step);

      // is mouse on lower right adjustable corner
      int x2 = mLift.stp[lift][step].x + mLift.stp[lift][step].w;
      int y2 = mLift.stp[lift][step].y + mLift.stp[lift][step].h;
      if ((mWM.hx > x2-8) && (mWM.hy > y2-8)) mouse_adj = 1;
      else mouse_move = 1;
   }


   // -----------------------------------------------------------
   // --  set mouse cursor
   // -----------------------------------------------------------
   if (mouse_move)     al_set_system_mouse_cursor(mDisplay.display, ALLEGRO_SYSTEM_MOUSE_CURSOR_MOVE);
   else if (mouse_adj) al_set_system_mouse_cursor(mDisplay.display, ALLEGRO_SYSTEM_MOUSE_CURSOR_RESIZE_SE);
   else                al_set_system_mouse_cursor(mDisplay.display, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);



   if (mInput.mouse_b[3][0])
   {
      while (mInput.mouse_b[3][0])
      {
         if (mouse_on_obj)
         {
            if (obt == 3) // move enemy
            {
               if (mInput.key[ALLEGRO_KEY_UP][3])    ov_move_enemy(num,  0, -1);
               if (mInput.key[ALLEGRO_KEY_DOWN][3])  ov_move_enemy(num,  0,  1);
               if (mInput.key[ALLEGRO_KEY_RIGHT][3]) ov_move_enemy(num,  1,  0);
               if (mInput.key[ALLEGRO_KEY_LEFT][3])  ov_move_enemy(num, -1,  0);
            }
         }
         if (mItem.item_secondary67_hires(mItem.item[num][0])) // hi-res adjust
         {
            if (mouse_on_item_sec_ul) ov_b3_arrow_nudge(mItem.item[num][6], mItem.item[num][7]); // move item secondary
            if (mouse_on_item_sec_lr) ov_b3_arrow_nudge(mItem.item[num][8], mItem.item[num][9]); // resize item secondary
         }

         if (mouse_on_vpod1)  ov_b3_arrow_nudge(mEnemy.Ei[num][5],  mEnemy.Ei[num][6]);
         if (mouse_on_vpod2)  ov_b3_arrow_nudge(mEnemy.Ei[num][7],  mEnemy.Ei[num][8]);
         if (mouse_on_vpodx)  ov_b3_arrow_nudge(mEnemy.Ei[num][9],  mEnemy.Ei[num][10]);
         if (mouse_on_tb_ul)  ov_b3_arrow_nudge(mEnemy.Ei[num][11], mEnemy.Ei[num][12]);
         if (mouse_on_tb_lr)  ov_b3_arrow_nudge(mEnemy.Ei[num][13], mEnemy.Ei[num][14]);

         if (mouse_on_csb_ul) ov_b3_arrow_nudge(mEnemy.Ei[num][15], mEnemy.Ei[num][16]);
         if (mouse_on_cdb_ul) ov_b3_arrow_nudge(mEnemy.Ei[num][17], mEnemy.Ei[num][18]);
         if (mouse_on_csb_lr) ov_b3_arrow_nudge(mEnemy.Ei[num][19], mEnemy.Ei[num][20]);

         if (mouse_on_esp) ov_b3_arrow_nudge(mEnemy.Ei[num][17], mEnemy.Ei[num][17]);
         if (mouse_on_bmb) ov_b3_arrow_nudge(mItem.item[num][7], mItem.item[num][7]);
         if (mouse_on_sp) ov_b3_arrow_nudge(mItem.item[num][7], mItem.item[num][7]);
         mWM.redraw_level_editor_background();
      }
   }





   if (mInput.mouse_b[1][0])
   {
      // hi-res mouse pos relative to level
      int mx = mWM.hx-10;
      int my = mWM.hy-10;

      // snap to pos
      mMiscFnx.mw_round(mx, mWM.mW[7].snap);
      mMiscFnx.mw_round(my, mWM.mW[7].snap);


      int lsox=0, lsoy=0;
      int lspx=0, lspy=0;
      if (mouse_on_lift)
      {
         if (mouse_move)
         {
            // snap pos
            mMiscFnx.mw_round(mLift.stp[lift][step].x, mWM.mW[7].snap);
            mMiscFnx.mw_round(mLift.stp[lift][step].y, mWM.mW[7].snap);

            // get offset from where mouse was initially clicked to lift step upper left
            lsox = mx - mLift.stp[lift][step].x;
            lsoy = my - mLift.stp[lift][step].y;
         }
         if (mouse_adj)
         {
            // snap size
            mMiscFnx.mw_round(mLift.stp[lift][step].w, mWM.mW[7].snap);
            mMiscFnx.mw_round(mLift.stp[lift][step].h, mWM.mW[7].snap);

            // enforce minimum size
            if (mLift.stp[lift][step].w < 4) mLift.stp[lift][step].w = 4;
            if (mLift.stp[lift][step].h < 4) mLift.stp[lift][step].h = 4;

            // get offset from where mouse was initially clicked to lift step lower right
            lspx = mx - (mLift.stp[lift][step].x + mLift.stp[lift][step].w);
            lspy = my - (mLift.stp[lift][step].y + mLift.stp[lift][step].h);
         }
      }


      while (mInput.mouse_b[1][0])
      {
         // hi-res mouse pos relative to level
         mx = mWM.hx-10;
         my = mWM.hy-10;

         // snap to pos
         mMiscFnx.mw_round(mx, mWM.mW[7].snap);
         mMiscFnx.mw_round(my, mWM.mW[7].snap);


         if (mouse_on_obj)
         {
            if (obt == 2) ov_move_item( num, mx - mItem.item[num][4], my - mItem.item[num][5]);
            if (obt == 3) ov_move_enemy(num, mx - mEnemy.Ef[num][0],  my - mEnemy.Ef[num][1]);
            if (obt == 4) // lift
            {
               if (mouse_move)
               {
                  // set new position
                  mLift.stp[lift][step].x = mx-lsox;
                  mLift.stp[lift][step].y = my-lsoy;
                  mLift.set_lift_to_step(lift, step);   // set current step in current lift
               }
               if (mouse_adj)
               {
                  // set new size
                  mLift.stp[lift][step].w = (mx-lspx) - mLift.stp[lift][step].x;
                  mLift.stp[lift][step].h = (my-lspy) - mLift.stp[lift][step].y;

                  // enforce minimum size
                  if (mLift.stp[lift][step].w < 4) mLift.stp[lift][step].w = 4;
                  if (mLift.stp[lift][step].h < 4) mLift.stp[lift][step].h = 4;

                  mLift.set_lift_to_step(lift, step);   // set current step in current lift
               }
            }
         }
         if (mouse_on_vpod1)
         {
            mEnemy.Ei[num][5] = mx;
            mEnemy.Ei[num][6] = my;
         }
         if (mouse_on_vpod2)
         {
            mEnemy.Ei[num][7] = mx;
            mEnemy.Ei[num][8] = my;
         }
         if (mouse_on_vpodx)
         {
            mEnemy.Ei[num][9]  = mx;
            mEnemy.Ei[num][10] = my;
         }
         if (mouse_on_tb_ul) // move vinepod trigger box from ul
         {
            mEnemy.Ei[num][11] = mx;
            mEnemy.Ei[num][12] = my;
         }
         if (mouse_on_tb_lr)  // resize vinepod trigger box from lr
         {
            // prevent lr corner from being less than ul corner
            if (mx < mEnemy.Ei[num][11]) mx = mEnemy.Ei[num][11];
            if (my < mEnemy.Ei[num][12]) my = mEnemy.Ei[num][12];

            // set new size
            mEnemy.Ei[num][13] = mx - mEnemy.Ei[num][11];
            mEnemy.Ei[num][14] = my - mEnemy.Ei[num][12];
         }

         if (mouse_on_csb_ul) // move cloner source box from ul
         {
            mEnemy.Ei[num][15] = mx;
            mEnemy.Ei[num][16] = my;
         }
         if (mouse_on_cdb_ul) // move cloner destination box from ul
         {
            mEnemy.Ei[num][17] = mx;
            mEnemy.Ei[num][18] = my;
         }
         if (mouse_on_csb_lr) // resize cloner boxes from source box lr
         {
            // prevent lr corner from being less than ul corner
            if (mx < mEnemy.Ei[num][15]+20) mx = mEnemy.Ei[num][15]+20;
            if (my < mEnemy.Ei[num][16]+20) my = mEnemy.Ei[num][16]+20;

            // set new size
            mEnemy.Ei[num][19] = mx - mEnemy.Ei[num][15];
            mEnemy.Ei[num][20] = my - mEnemy.Ei[num][16];
         }

         if (mouse_on_item_sec_ul) // move item secondary
         {
            if (!mItem.item_secondary67_hires(mItem.item[num][0])) // force snap 20 for these item types
            {
               mItem.item[num][6] = mWM.gx*20;
               mItem.item[num][7] = mWM.gy*20;
            }
            else
            {
               mItem.item[num][6] = mx;
               mItem.item[num][7] = my;
            }
         }
         if (mouse_on_item_sec_lr) // resize item secondary
         {
            int min_size = mWM.mW[7].snap;
            if (min_size < 4) min_size = 4;
            if (!mItem.item_secondary67_hires(mItem.item[num][0])) // force snap 20 for these item types
            {
               mItem.item[num][8] = (mWM.gx+1)*20 - mItem.item[num][6];
               mItem.item[num][9] = (mWM.gy+1)*20 - mItem.item[num][7];
               min_size = 20;
            }
            else
            {
               mItem.item[num][8] = mx - mItem.item[num][6];
               mItem.item[num][9] = my - mItem.item[num][7];
            }
            // don't allow sizes to be less than min_size
            if (mItem.item[num][8] < min_size) mItem.item[num][8] = min_size;
            if (mItem.item[num][9] < min_size) mItem.item[num][9] = min_size;
         }
         if (mouse_on_sp) // adjust sproingy jump height
         {
            float y0 = mItem.item[num][5]+10;
            float fy = mWM.hy;
            mItem.item[num][7] = mMiscFnx.get_sp(y0-fy);

            // bounds check
            if (mItem.item[num][7] < 40) mItem.item[num][7] = 40;
            if (mItem.item[num][7] > 200) mItem.item[num][7] = 200;
         }
         if (mouse_on_bmb) // adjust bomb blast radius
         {
            float x0 = mItem.item[num][4]+10; // get center of item location
            float y0 = mItem.item[num][5]+10;
            float fx = mWM.hx;
            float fy = mWM.hy;
            float dist = sqrt(pow((x0-fx), 2) + pow((y0-fy), 2)); // distance from mouse to item
            mItem.item[num][7] = dist;
         }
         if (mouse_on_bms) // move block manip source
         {
            mItem.item[num][13] = mWM.gx*20;
            mItem.item[num][14] = mWM.gy*20;
         }
         if (mouse_on_esp) // adjust enemy shot prox
         {
            float x0 = mEnemy.Ef[num][0]+10; // get center of object location
            float y0 = mEnemy.Ef[num][1]+10;
            float fx = mWM.hx;
            float fy = mWM.hy;
            float dst = sqrt(pow((x0-fx), 2) + pow((y0-fy), 2)); // distance from mouse
            mEnemy.Ei[num][17] = dst;
         }
         mWM.redraw_level_editor_background();
      } // end of while mInput.mouse_b[1][0] pressed

      // snap these item secondaries to 20
      if ((mouse_on_obj) && (obt == 2) && (!mItem.item_secondary67_hires(mItem.item[num][0])))
      {
         mMiscFnx.mw_round(mItem.item[num][6], 20);
         mMiscFnx.mw_round(mItem.item[num][7], 20);
      }

   } // end of if mInput.mouse_b[1][0] pressed

   if (mInput.mouse_b[2][0])
   {
      while (mInput.mouse_b[2][0]) mEventQueue.proc();
      mWM.set_windows(1);
   }
}



void mwWindow::ov_b3_arrow_nudge(int& x, int& y)
{
   if (mInput.key[ALLEGRO_KEY_LEFT][3])  x--;
   if (mInput.key[ALLEGRO_KEY_RIGHT][3]) x++;
   if (mInput.key[ALLEGRO_KEY_UP][3])    y--;
   if (mInput.key[ALLEGRO_KEY_DOWN][3])  y++;
}



void mwWindow::ov_move_item(int num, int x_offset, int y_offset)
{
   mItem.itemf[num][0] = mItem.item[num][4] += x_offset;
   mItem.itemf[num][1] = mItem.item[num][5] += y_offset;

   if ((mInput.SHFT()) && (mItem.item_secondary67(mItem.item[num][0]))) // move secondaries also
   {
      mItem.item[num][6] += x_offset;
      mItem.item[num][7] += y_offset;
   }
}


void mwWindow::ov_move_enemy(int num, int x_offset, int y_offset)
{
   mEnemy.Ef[num][0] += x_offset;
   mEnemy.Ef[num][1] += y_offset;
   if (mEnemy.Ei[num][0] == 7) // vinepod
   {
      // vinepod also stores this as initial position
      mEnemy.Ei[num][3] = mEnemy.Ef[num][0];
      mEnemy.Ei[num][4] = mEnemy.Ef[num][1];

      if (mInput.SHFT()) // vinepod secondaries
      {
         mEnemy.Ei[num][5]  += x_offset;
         mEnemy.Ei[num][6]  += y_offset;
         mEnemy.Ei[num][7]  += x_offset;
         mEnemy.Ei[num][8]  += y_offset;
         mEnemy.Ei[num][9 ] += x_offset;
         mEnemy.Ei[num][10] += y_offset;
         mEnemy.Ei[num][11] += x_offset;
         mEnemy.Ei[num][12] += y_offset;
      }
   }
   if ((mEnemy.Ei[num][0] == 9) && (mInput.SHFT())) // cloner secondaries
   {
      mEnemy.Ei[num][15] += x_offset; // source
      mEnemy.Ei[num][16] += y_offset;
      mEnemy.Ei[num][17] += x_offset; // dest
      mEnemy.Ei[num][18] += y_offset;
   }
}


void mwWindow::ov_check_if_valid(void)
{
   // check if the current object is valid
   int obt = mWM.mW[7].obt;
   int num = mWM.mW[7].num;
   if (obt==0)                               mWM.set_windows(1);
   if ((obt==2) && (!mItem.item[num][0]))    mWM.set_windows(1);
   if ((obt==3) && (!mEnemy.Ei[num][0]))     mWM.set_windows(1);
   if ((obt==4) && (!mLift.cur[num].active)) mWM.set_windows(1);
}

void mwWindow::ov_process_keypress(void)
{
   char msg[1024];
   int mb = mWM.mW[7].mb;
   int type=0, lift=0, step=0;
   if (mWM.mW[7].obt == 2) type = mItem.item[mWM.mW[7].num][0];
   if (mWM.mW[7].obt == 3) type = mEnemy.Ei[mWM.mW[7].num][0];
   if (mWM.mW[7].obt == 4)
   {
      lift = mWM.mW[7].num;
      step = mLift.cur[mWM.mW[7].num].current_step;
      mLift.set_lift_to_step(lift, step);   // set current step in current lift
      mLift.cur[mWM.mW[7].num].current_step = step;
   }

   while (mInput.key[ALLEGRO_KEY_DELETE][0]) { mEventQueue.proc(); mb = 20; }
   while (mInput.key[ALLEGRO_KEY_RIGHT][0])  { mEventQueue.proc(); mb = 21; }
   while (mInput.key[ALLEGRO_KEY_LEFT][0])   { mEventQueue.proc(); mb = 22; }

   switch(mb)
   {
      case 18: // move
      {
         if (mWM.mW[7].obt==2)
         {
            sprintf(msg,"%s", mItem.item_name[type]);
            mMiscFnx.getxy(msg, mWM.mW[7].obt, type, mWM.mW[7].num);
         }
         if (mWM.mW[7].obt==3)
         {
            sprintf(msg,"%s", mEnemy.enemy_name[type][0]);
            mMiscFnx.getxy(msg, mWM.mW[7].obt, type, mWM.mW[7].num);
         }
         if (mWM.mW[7].obt==4)
         {
            while (!mInput.key[ALLEGRO_KEY_ESCAPE][0])
            {
               if (mWM.redraw_level_editor_background())
               {
                  for (int t=0; t<2; t++) mLift.move_lifts(1);  // move lifts for 2 frames
                  ov_draw_overlays(0);
                  mScreen.get_new_screen_buffer(3, 0, 0);
                  mWM.cycle_windows(1); // draw only
               }
            }
            while (mInput.key[ALLEGRO_KEY_ESCAPE][0]) mEventQueue.proc(); // wait for release
            mLift.lift_setup(); // reset all lifts to step 0
         }
      }
      break;
      case 19:
         mWM.mW[7].num = create_obj(mWM.mW[7].obt, type, mWM.mW[7].num);
      break;
      case 20: // delete
         if (mWM.mW[7].obt == 2)
         {
            mItem.erase_item(mWM.mW[7].num);
            mItem.sort_item(1);
            if (mWM.mW[7].num >= mItem.item_first_num[type]+ mItem.item_num_of_type[type]) mWM.mW[7].num--;
            ov_check_if_valid();
         }
         if (mWM.mW[7].obt == 3)
         {
            mEnemy.Ei[mWM.mW[7].num][0] = 0;
            mEnemy.sort_enemy();
            if (mWM.mW[7].num >= mEnemy.e_first_num[type]+mEnemy.e_num_of_type[type]) mWM.mW[7].num--;
            ov_check_if_valid();
         }
         if (mWM.mW[7].obt == 4)
         {
            mLift.erase_lift(mWM.mW[7].num);
            if (--mWM.mW[7].num < 0) mWM.mW[7].num = 0;      // set to prev lift or zero
            ov_check_if_valid();
         }
      break;
      case 21: // next
         if ((mWM.mW[7].obt==2) && (++mWM.mW[7].num >= mItem.item_first_num[type] + mItem.item_num_of_type[type])) mWM.mW[7].num--;
         if ((mWM.mW[7].obt==3) && (++mWM.mW[7].num >= mEnemy.e_first_num[type] + mEnemy.e_num_of_type[type])) mWM.mW[7].num--;
         if (mWM.mW[7].obt==4) mWM.mW[7].num = mLift.get_next_lift(mWM.mW[7].num);
      break;
      case 22: // previous
         if ((mWM.mW[7].obt==3) && (--mWM.mW[7].num < mEnemy.e_first_num[type])) mWM.mW[7].num++;
         if ((mWM.mW[7].obt==2) && (--mWM.mW[7].num < mItem.item_first_num[type])) mWM.mW[7].num++;
         if (mWM.mW[7].obt==4) mWM.mW[7].num = mLift.get_prev_lift(mWM.mW[7].num);
      break;
   } // end of switch (mb)
}

void mwWindow::object_viewer(int obt, int num)
{
   mWM.mW[7].obt = obt;
   mWM.mW[7].num = num;
   mWM.set_windows(4); // object viewer
//   al_set_system_mouse_cursor(mDisplay.display, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);
}
