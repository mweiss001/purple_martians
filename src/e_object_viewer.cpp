// e_object_viewer.cpp
#include "pm.h"
#include "e_object_viewer.h"
#include "mwWindow.h"
#include "mwWindowManager.h"
#include "mwFont.h"
#include "mwBitmap.h"
#include "z_lift.h"
#include "mwWidgets.h"
#include "mwColor.h"
#include "mwPMEvent.h"
#include "mwInput.h"
#include "mwDisplay.h"
#include "mwEventQueue.h"
#include "z_menu.h"
#include "z_item.h"
#include "z_enemy.h"
#include "e_fnx.h"
#include "z_screen.h"


#define MAP_LOCK_KEY ALLEGRO_KEY_Z




int create_obj(int obt, int type, int num)
{
   if (obt == 2) // items
   {
      int ret = create_item(type);
      if (ret > 500)
      {
         al_show_native_message_box(display,
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
         int e = create_pod();
         if ((e>=0) && (e<99) && (Ei[e][0] == 7))
         {
            object_viewer(3, e);
            num = e;
         }
      }
      if (type == 9)
      {
         int e = create_cloner();
         if ((e>=0) && (e<99) && (Ei[e][0] == 9))
         {
            object_viewer(3, e);
            num = e;
         }
      }
      if (type == 13)
      {
         int e = create_vinepod();
         if ((e>=0) && (e<99) && (Ei[e][0] == 13))
         {
            object_viewer(3, e);
            num = e;
         }
      }

   }
   if (obt == 4) create_lift();
   return num;  // return number of created obj or sent_num if bad create
}
void ov_get_size(void)
{
   int obt = mwWM.mW[7].obt;
   int num = mwWM.mW[7].num;
   int type=0, w=300;
   if (obt == 2) type = item[num][0];
   if (obt == 3) type = Ei[num][0];

   ov_check_if_valid(type);



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
   if ((obt == 2) && (type == 14)) w = 200; // switch
   if ((obt == 2) && (type == 15)) w = 240; // sproingy
   if ((obt == 2) && (type == 16)) w = 280; // bm
   if ((obt == 2) && (type == 17)) w = 290; // bd

   if ((obt == 3) && (type == 3 )) w = 220; // archwagon
   if ((obt == 3) && (type == 4 )) w = 220; // bouncer
   if ((obt == 3) && (type == 5 )) w = 220; // jumpworm
   if ((obt == 3) && (type == 6 )) w = 220; // cannon
   if ((obt == 3) && (type == 7 )) w = 220; // podzilla
   if ((obt == 3) && (type == 8 )) w = 220; // trakbot
   if ((obt == 3) && (type == 9 )) w = 220; // cloner
   if ((obt == 3) && (type == 11)) w = 220; // block walker
   if ((obt == 3) && (type == 12)) w = 220; // flapper
   if ((obt == 3) && (type == 13)) w = 220; // vinepod

   if (obt == 4)                   w = 300; // lift

   mwWM.mW[7].set_size(w, mwWM.mW[7].h);
}



void set_switch_tile(int i)
{
   int b = 96; // base
   int nc = 0; // number of colors
   int c[4] = {0};

   if (item[i][10] & 0b0001) { c[nc] = 9;  nc++;} // green
   if (item[i][10] & 0b0010) { c[nc] = 10; nc++;} // red
   if (item[i][10] & 0b0100) { c[nc] = 12; nc++;} // blue
   if (item[i][10] & 0b1000) { c[nc] = 8;  nc++;} // purple

   // no colors selected
   if (nc == 0) item[i][1] = b;                     // white

   // one color selected
   if ((nc == 1) && (c[0] == 9))  item[i][1] = b+1; // green
   if ((nc == 1) && (c[0] == 10)) item[i][1] = b+2; // red
   if ((nc == 1) && (c[0] == 12)) item[i][1] = b+3; // blue
   if ((nc == 1) && (c[0] == 8))  item[i][1] = b+4; // purple


   // two colors selected
   if ((nc == 2) && (c[0] == 9)  && (c[1] == 10)) item[i][1] = b+5;  // green red
   if ((nc == 2) && (c[0] == 9)  && (c[1] == 12)) item[i][1] = b+6;  // green blue
   if ((nc == 2) && (c[0] == 9)  && (c[1] == 8))  item[i][1] = b+7;  // green purple
   if ((nc == 2) && (c[0] == 10) && (c[1] == 12)) item[i][1] = b+8;  // red blue
   if ((nc == 2) && (c[0] == 10) && (c[1] == 8))  item[i][1] = b+9;  // red purple
   if ((nc == 2) && (c[0] == 12) && (c[1] == 8))  item[i][1] = b+10; // blue purple

   // three colors selected
   if ((nc == 3) && (c[0] == 9)  && (c[1] == 10) && (c[2] == 12)) item[i][1] = b+11;  // green red  blue
   if ((nc == 3) && (c[0] == 9)  && (c[1] == 10) && (c[2] == 8))  item[i][1] = b+12;  // green red  purple
   if ((nc == 3) && (c[0] == 9)  && (c[1] == 12) && (c[2] == 8))  item[i][1] = b+13;  // green blue purple
   if ((nc == 3) && (c[0] == 10) && (c[1] == 12) && (c[2] == 8))  item[i][1] = b+14;  // red   blue purple

   // four colors selected
   if (nc == 4) item[i][1] = b+15;  // green red blue purple

}


void ov_title(int x1, int x2, int y1, int y2, int legend_highlight)
{
   int obt = mwWM.mW[7].obt;
   int num = mwWM.mW[7].num;
   int type=0;
   if (obt == 2) type = item[num][0];
   if (obt == 3) type = Ei[num][0];

//   al_draw_rectangle(x1-1, y1-1, x2+1, y2+1, mC.pc[10], 1);  // outline entire window

   int xc = (x1+x2)/2;
   int yt = y1+14;

   // legend line text
   char lmsg[6][80];
   for (int x=0; x<6; x++) sprintf(lmsg[x],"%s","");

   // legend line colors
   int legend_color[6];

   // default number of legend lines and colors
   mwWM.mW[7].num_legend_lines = 3;
   legend_color[0] = 7;   // legend color
   legend_color[1] = 13;  // location color
   legend_color[2] = 14;  // yellow
   legend_color[3] = 10;  // red
   legend_color[4] = 0;   // unused
   legend_color[5] = 0;   // unused

   legend_highlight == 1 ? legend_color[1] = mC.flash_color : legend_color[1] = 13;


   // title bar
   msg[0] = 0;
   if (obt == 2) sprintf(msg, "Item Viewer [%d]", num);
   if (obt == 3) sprintf(msg, "Enemy Viewer [%d]", num);
   if (obt == 4) sprintf(msg, "Lift Viewer [%d]", num);

   for (int x=0; x<15; x++)
      al_draw_line(x1, y1+x, x2, y1+x, mC.pc[13+(x*16)], 1);
   al_draw_text(mF.pr8, mC.pc[15], xc, y1+2, ALLEGRO_ALIGN_CENTER,  msg);


   if (obt == 4)  // lifts
   {
      mwWM.mW[7].num_legend_lines = 0;
      al_draw_rectangle(xc-94, yt, xc+94, yt+22, mC.pc[15], 1);
      al_draw_textf(mF.pr8, mC.pc[13], xc, yt+8, ALLEGRO_ALIGN_CENTER, "Lift %d of %d",num+1, num_lifts);
   }
   if (obt == 3)  // enemies
   {
      al_draw_rectangle(xc-94, yt, xc+94, yt+22, mC.pc[15], 1);
      draw_enemy(num, 1, xc-92, yt+1);
      sprintf(msg,"%s %d of %d", (const char *)enemy_name[type][0],1+num - e_first_num[type],e_num_of_type[type]);
      al_draw_text(mF.pr8, mC.pc[13], xc-69, yt+8, 0, msg);
      switch (type)
      {
         case 3: // archwagon
            sprintf(lmsg[1],"ArchWagon Location");
            sprintf(lmsg[2],"Bullet Proximity");
            legend_highlight == 2 ? legend_color[2] = mC.flash_color : legend_color[2] = 14;
         break;
         case 4:
            mwWM.mW[7].num_legend_lines = 2;
            sprintf(lmsg[1],"Bouncer Location");
         break;
         case 5:
            mwWM.mW[7].num_legend_lines = 2;
            sprintf(lmsg[1],"JumpWorm Location");
         break;
         case 6: // cannon
            sprintf(lmsg[1],"Cannon Location");
            sprintf(lmsg[2],"Bullet Proximity");
            legend_highlight == 2 ? legend_color[2] = mC.flash_color : legend_color[2] = 14;
         break;
         case 7: // podzilla
            mwWM.mW[7].num_legend_lines = 4;
            sprintf(lmsg[1],"Podzilla Location");
            sprintf(lmsg[2],"Extended Postion");
            sprintf(lmsg[3],"Trigger Box");
            legend_highlight == 2 ? legend_color[2] = mC.flash_color : legend_color[2] = 10;
            legend_highlight == 3 ? legend_color[3] = mC.flash_color : legend_color[3] = 14;
         break;
         case 8: // trakbot
            sprintf(lmsg[1],"TrakBot Location");
            sprintf(lmsg[2],"Bullet Proximity");
            legend_highlight == 2 ? legend_color[2] = mC.flash_color : legend_color[2] = 14;
         break;
         case 9: // cloner
            mwWM.mW[7].num_legend_lines = 5;
            sprintf(lmsg[1],"Cloner Location");
            sprintf(lmsg[2],"Source Area");
            sprintf(lmsg[3],"Destination Area");
            sprintf(lmsg[4],"Trigger Box");
            legend_highlight == 2 ? legend_color[2] = mC.flash_color : legend_color[2] = 11;
            legend_highlight == 3 ? legend_color[3] = mC.flash_color : legend_color[3] = 10;
            legend_highlight == 4 ? legend_color[4] = mC.flash_color : legend_color[4] = 14;
         break;
         case 11:
            mwWM.mW[7].num_legend_lines = 2;
            sprintf(lmsg[1],"Block Walker Location");
         break;
         case 12: // flapper
            mwWM.mW[7].num_legend_lines = 4;
            sprintf(lmsg[1],"Flapper Location");
            sprintf(lmsg[2],"Bullet Proximity");
            sprintf(lmsg[3],"Height Above Player");
            legend_highlight == 2 ? legend_color[2] = mC.flash_color : legend_color[2] = 14;
            legend_highlight == 3 ? legend_color[3] = mC.flash_color : legend_color[3] = 10;
         break;

         case 13: // vinepod
            mwWM.mW[7].num_legend_lines = 6;
            sprintf(lmsg[1],"Vinepod Location");
            sprintf(lmsg[2],"Extended Position");
            sprintf(lmsg[3],"Control Point 1");
            sprintf(lmsg[4],"Control Point 2");
            sprintf(lmsg[5],"Trigger Box");
            legend_highlight == 2 ? legend_color[2] = mC.flash_color : legend_color[2] = 10;
            legend_highlight == 3 ? legend_color[3] = mC.flash_color : legend_color[3] = 6;
            legend_highlight == 4 ? legend_color[4] = mC.flash_color : legend_color[4] = 7;
            legend_highlight == 5 ? legend_color[5] = mC.flash_color : legend_color[5] = 14;
         break;



      }
   }
   if (obt == 2)  // items
   {
      al_draw_rectangle(xc-94, yt, xc+94, yt+22, mC.pc[15], 1);
      draw_item(num, 1, xc-94, yt+1);
      sprintf(msg,"%s %d of %d", item_name[type], 1+num - item_first_num[type],item_num_of_type[type]);
      al_draw_text(mF.pr8, mC.pc[13], xc-69, yt+8, 0, msg);
      switch (type)
      {
         case 1: // door
         {
            legend_highlight == 2 ? legend_color[2] = mC.flash_color : legend_color[2] = 10;
            if (item[num][8] == 0)  // exit only, no destination
            {
               // find src door(s)
               int num_src = 0;
               for (int i=0; i<500; i++)
                  if ((item[i][0] == 1) && (item[i][9] == num))
                  {
                     num_src++;
                  }
               sprintf(lmsg[1],"Exit Door Location");
               if (num_src == 0) sprintf(lmsg[2],"No Source Door");
               if (num_src == 1) sprintf(lmsg[2],"Source Door Position");
               if (num_src >  1) sprintf(lmsg[2],"Source Door Positions");
            }

            if (item[num][8] == 1)  // draw destination
            {
               sprintf(lmsg[1],"Door Location");
               sprintf(lmsg[2],"Destination");
            }
         }
         break;
         case 2:
             mwWM.mW[7].num_legend_lines = 2;
             sprintf(lmsg[1],"Bonus Location");
         break;
         case 3:
             mwWM.mW[7].num_legend_lines = 2;
             sprintf(lmsg[1],"Exit Location");
         break;
         case 4: // key
            sprintf(lmsg[1],"Key Location");
            sprintf(lmsg[2],"Block Range");
            legend_highlight == 2 ? legend_color[2] = mC.flash_color : legend_color[2] = 10;
         break;
         case 5:
             mwWM.mW[7].num_legend_lines = 2;
             sprintf(lmsg[1],"Start Location");
         break;
         case 6:
             mwWM.mW[7].num_legend_lines = 2;
             sprintf(lmsg[1],"Orb Location");
         break;
         case 7:
             mwWM.mW[7].num_legend_lines = 2;
             sprintf(lmsg[1],"Mine Location");
         break;
         case 8:
            sprintf(lmsg[1],"Bomb Location");
            sprintf(lmsg[2],"Damage Range");
            legend_highlight == 2 ? legend_color[2] = mC.flash_color : legend_color[2] = 14;
         break;
         case 9: // trigger
            sprintf(lmsg[1],"Trigger Item Location");
            sprintf(lmsg[2],"Trigger Field");
            legend_highlight == 2 ? legend_color[2] = mC.flash_color : legend_color[2] = 14;
         break;
         case 10:
            mwWM.mW[7].num_legend_lines = 3;
            sprintf(lmsg[1],"Scroll Location");
            sprintf(lmsg[2],"Message Area");
            legend_highlight == 2 ? legend_color[2] = mC.flash_color : legend_color[2] = 10;
            if (item[num][2] & PM_ITEM_PMSG_TRIGGER_BOX)
            {
               mwWM.mW[7].num_legend_lines = 4;
               sprintf(lmsg[3],"Trigger Area");
               legend_highlight == 3 ? legend_color[3] = mC.flash_color : legend_color[3] = 14;
            }
         break;
         case 11:
            sprintf(lmsg[1],"Rocket Location");
            sprintf(lmsg[2],"Damage Range");
            legend_highlight == 2 ? legend_color[2] = mC.flash_color : legend_color[2] = 14;
         break;
         case 12:
             mwWM.mW[7].num_legend_lines = 2;
             sprintf(lmsg[1],"Warp Location");
         break;
         case 14: // switch
            sprintf(lmsg[1],"Switch Location");
            sprintf(lmsg[2],"Block Range");
            legend_highlight == 2 ? legend_color[2] = mC.flash_color : legend_color[2] = 10;
         break;
         case 15:
            sprintf(lmsg[1],"Sproingy Location");
            sprintf(lmsg[2],"Jump Height");
            legend_highlight == 2 ? legend_color[2] = mC.flash_color : legend_color[2] = 14;
         break;
         case 16: // block manip
            sprintf(lmsg[1],"Block Manip Item Location");
            sprintf(lmsg[2],"Manip Field");
            legend_highlight == 2 ? legend_color[2] = mC.flash_color : legend_color[2] = 12;
         break;
         case 17: // block damage
            sprintf(lmsg[1],"Item Location");
            sprintf(lmsg[2],"Damage Area");
            legend_highlight == 2 ? legend_color[2] = mC.flash_color : legend_color[2] = 10;
         break;
      } // end of switch case
   }  // end of items

   al_draw_rectangle(x1, y1, x2, y2, mC.pc[13], 1);  // outline entire window

   if (mwWM.mW[7].num_legend_lines > 0)
   {
      al_draw_text(mF.pr8, mC.pc[legend_color[0]], xc, y2-36+ (4-mwWM.mW[7].num_legend_lines)*8, ALLEGRO_ALIGN_CENTER, "Legend");
      al_draw_rectangle(xc-100, y2-38+ (4-mwWM.mW[7].num_legend_lines)*8, xc+100, y2-1, mC.pc[13], 1); // big frame
      al_draw_rectangle(xc-100, y2-38+ (4-mwWM.mW[7].num_legend_lines)*8, xc+100, y2-28+ (4-mwWM.mW[7].num_legend_lines)*8, mC.pc[13], 1); // top frame
   }

   for (int x=1; x<mwWM.mW[7].num_legend_lines; x++)// draw text lines
      al_draw_text(mF.pr8, mC.pc[legend_color[x]], xc, y2-26+(3-mwWM.mW[7].num_legend_lines+x)*8, ALLEGRO_ALIGN_CENTER, lmsg[x]);
}
void ov_draw_buttons(int x1, int y1, int x2, int y2, int d)
{
   int obt = mwWM.mW[7].obt;
   int n = mwWM.mW[7].num;
   int type=0;
   if (obt == 2) type = item[n][0];
   if (obt == 3) type = Ei[n][0];



   mwWM.mW[7].mb = 0;


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
   if (mwWM.mW[7].viewer_lock) lc = 7;

   if (mdw_buttont(xa,  ya, x27-1, bts,                0,0,0,0,  0, 9,15,0, 1,0,0,d, "Prev")) mwWM.mW[7].mb = 22;
       mdw_toggle( x27, ya, x57-1, bts,                0,0,0,0,  0,lc,15,0, 1,0,0,d, mwWM.mW[7].viewer_lock, "Unlocked", "Locked", 15, 15, 6, 7);
   if (mdw_buttont(x57, ya, xb,    bts,                0,0,0,0,  0, 9,15,0, 1,0,1,d, "Next")) mwWM.mW[7].mb = 21;

   if (obt == 4) { if (mdw_buttont(xa, ya, x13-1, bts, 0,0,0,0,  0,13,15,0, 1,0,0,d, "Run Lifts")) mwWM.mW[7].mb = 18; }
   else          { if (mdw_buttont(xa, ya, x13-1, bts, 0,0,0,0,  0,13,15,0, 1,0,0,d, "Move"))      mwWM.mW[7].mb = 18; }

   if (mdw_buttont(x13, ya, x23-1, bts,                0,0,0,0,  0,14,15,0, 1,0,0,d, "Create")) mwWM.mW[7].mb = 19;
   if (mdw_buttont(x23, ya, xb,    bts,                0,0,0,0,  0,10,15,0, 1,0,1,d, "Delete")) mwWM.mW[7].mb = 20;

   if (mdw_buttont( xa, ya, x14-1, bts,                0,0,0,0,  0,1,15,0,  1,0,0,d, "Help")) help("Viewer Basics");
   mdw_button(     x14, ya, xb,    bts,               57,0,0,0,  0,1,15,0,  1,0,1,d);         // object specific help

   ya+=4; // space after common buttons





   // --------------------------------------------------
   // --- Lifts ----------------------------------------
   // --------------------------------------------------
   if (obt == 4) // lifts
   {
      int lift = n;
      int step = lifts[n].current_step;

      mdw_buttonp(       xa, ya, xb, bts, 500,0,0,0,    0,13,15,0,  1,0,1,d, lifts[lift].mode); // MODE
      if (lifts[lift].mode)
         mdw_slideri(xa, ya, xb, bts, 0,0,0,0,      0,13,15,15, 1,0,1,d, lifts[lift].val2, 2000, 1, 1,  "Reset Timer:");
      if (mdw_button(    xa, ya, xb, bts, 504,lift,0,0, 0, 4,15,0,  1,0,1,d)) mwWM.mW[7].mb = 26; // lift name
      ya+=bts;

      // list of step buttons
      // --------------------------------------------------------------------------------
      int ysb = ya; // y pos of step buttons

      // process mouse on list of step buttons
      int step_pointer = -99;
      if ((mI.mouse_x > xa + 10) && (mI.mouse_x < xb - 10)) // is mouse on step buttons?
      {
         int step0_y_offset = ysb + 14 + bts;
         int mouse_step = (mI.mouse_y - step0_y_offset) / bts;               // calculate step that mouse is on
         if ((mouse_step >= 0) && (mouse_step < lifts[lift].num_steps))   // is this a valid step?
         {
            step_pointer = mouse_step;  // set step pointer to this step
            if (mI.mouse_b[1][0])
            {
               step = mouse_step; // set current step to this step
               lifts[lift].current_step = step; // set current step in lift
            }
            if (mI.mouse_b[2][0]) step_popup_menu(lift, step_pointer); // step pop-up menu for this step
         }
      }

      // draw the list of steps  - this has to go after, because it can eat the mouse clicks needed for previous section
      int ycs = ysb + draw_steps(xa, xb, ysb, lift, step, step_pointer, d);
      ycs +=bts;

      // draw buttons for the current step button and get y postion for next item (lift)
      int yld = ycs + draw_current_step_buttons(xa, xb, ycs, lift, step, d);


      mwWM.mW[7].h = yld - mwWM.mW[7].y1-1; // global variable for height of ovw when variable due to lift
      mwWM.mW[7].set_size(mwWM.mW[7].w, mwWM.mW[7].h);

      yld += bts;

      // draw current lift under step list buttons
      if (bts == 16) // only if max button size
      {
         int x1 = xa+10;
         int y1 = yld; // only see in 2 highest screen modes
         int s = step;

         // if step is not a move step, find prev that is
         if ((lift_steps[lift][s].type & 31) != 1) s = lift_find_previous_move_step(lift, s);

         // get w h from step
         int w  = lift_steps[lift][s].w;
         int h  = lift_steps[lift][s].h;

         // get x2 and y2 based on x1 y1 and w h
         int x2 = x1 + w;
         int y2 = y1 + h;
         draw_lift(lift, x1, y1, x2, y2);
      }

      if (mwWM.mW[7].mb == 26)
      {
         char fst[80];
         strcpy(fst, lifts[lift].lift_name);
         if (edit_lift_name(lift, yld, xa+10, fst)) strcpy(lifts[lift].lift_name, fst);
      }
   }


   // --------------------------------------------------
   // --- Enemies --------------------------------------
   // --------------------------------------------------
   if (obt == 3)    // enemies
   {
      switch (type) // enemy subtypes
      {
         case 3:     // archwag
            mdw_sliderf(    xa, ya, xb, bts,  0,0,0,0,  0,12,15,15,  1,0,1,d, Efi[n][6], 9,  0.7, 0.01, "X-Speed:");
            mdw_sliderf(    xa, ya, xb, bts,  0,0,0,0,  0,12,15,15,  1,0,1,d, Efi[n][3], 16, 0.7, 0.01, "Y-Speed:");
            ya+=4; // spacer
            mdw_toggle(     xa, ya, xb, bts,  0,0,0,0,  0, 0, 0, 0,  1,0,1,d, Ei[n][2], "Initial Direction:Left ", "Initial Direction:Right", 13, 13, 15, 15);
            mdw_toggle(     xa, ya, xb, bts,  0,0,0,0,  0, 0, 0, 0,  1,0,1,d, Ei[n][8], "Follow Mode",             "Bounce Mode",             13, 13, 15, 15);
            ya+=4; // spacer
            mdw_slider0(    xa, ya, xb, bts,  0,0,0,0,  0, 7,15,15,  1,0,1,d, Ei[n][10], 100, 0, 1,    "Turn Before Hole:", "Off");
            mdw_slider0(    xa, ya, xb, bts,  0,0,0,0,  0, 7,15,15,  1,0,1,d, Ei[n][11], 100, 0, 1,    "Jump Before Hole:", "Off");
            mdw_slider0(    xa, ya, xb, bts,  0,0,0,0,  0, 7,15,15,  1,0,1,d, Ei[n][12], 100, 0, 1,    "Jump Before Wall:", "Off");
            mdw_slider0(    xa, ya, xb, bts,  0,0,0,0,  0, 7,15,15,  1,0,1,d, Ei[n][7],  600, 0, 1,    "Jump Under Width:", "Off");
            mdw_slider0(    xa, ya, xb, bts,  0,0,0,0,  0, 7,15,15,  1,0,1,d, Ei[n][6],  500, 0, 1,    "Jump Timer Wait:",  "Off");
            ya+=4; // spacer
            mdw_sliderf(    xa, ya, xb, bts,  0,0,0,0,  0, 9,15,15,  1,0,1,d, Efi[n][7], 20, 0.8, 0.1, "Bullet Speed:");
            mdw_slideri(    xa, ya, xb, bts,  0,0,0,0,  0, 9,15,15,  1,0,1,d, Ei[n][17], 2000, 20, 1,  "Bullet Proximity:");
            mdw_slideri(    xa, ya, xb, bts,  0,0,0,0,  0, 9,15,15,  1,0,1,d, Ei[n][15], 200, 1, 1,    "Bullet Retrigger Time:");
            ya+=4; // spacer
            mdw_slideri(    xa, ya, xb, bts,  0,0,0,0,  0, 4,15,15,  1,0,1,d, Ei[n][29], 20, 0, 1,     "Collision Box:");
            mdw_sliderf(    xa, ya, xb, bts,  0,0,0,0,  0, 4,15,15,  1,0,1,d, Efi[n][4], 10, 0, 0.1,   "Health Decrement:");
            mdw_slideri(    xa, ya, xb, bts,  0,0,0,0,  0, 4,15,15,  1,0,1,d, Ei[n][25], 50, 0, 1,     "Health Bonus:");
         break;
         case 4: // bouncer
            mdw_sliderf(    xa, ya, xb, bts,  22,n,0,0, 0, 9,15,15,  1,0,1,d, Efi[n][5], 12, 0, 0.01, "Speed:");
            ya+=4; // spacer
            if (mdw_buttont(xa, ya, xb, bts,  0,0,0,0,  0,12,15, 0,  1,0,1,d, "Set Initial Direction"))  getxy("Initial Direction", 96, 4, n);
            mdw_slideri(    xa, ya, xb, bts,  0,0,0,0,  0,12,15,15,  1,0,1,d, Ei[n][8], 100, 0, 1,   "Seek Count:");
            ya+=4; // spacer
            mdw_button(     xa, ya, xb, bts,  13,n,0,0, 0, 8, 9, 0,  1,0,1,d); // main shape
            ya+=4; // spacer
            mdw_button(     xa, ya, xb, bts,  14,n,0,0, 0, 8, 9, 0,  1,0,1,d); // seek shape
            ya+=4; // spacer
            mdw_slideri(    xa, ya, xb, bts,  0,0,0,0,  0, 4,15,15,  1,0,1,d, Ei[n][29], 20, 0, 1,    "Collision Box:");
            mdw_sliderf(    xa, ya, xb, bts,  0,0,0,0,  0, 4,15,15,  1,0,1,d, Efi[n][4], 10, 0, 0.1,  "Health Decrement:");
            mdw_slideri(    xa, ya, xb, bts,  0,0,0,0,  0, 4,15,15,  1,0,1,d, Ei[n][25], 50, 0, 1,    "Health Bonus:");
         break;
         case 5:     // jumpworm
            mdw_sliderf(    xa, ya, xb, bts,  0,0,0,0,  0,12,15,15,  1,0,1,d, Efi[n][6], 12, 0.4, 0.01,"X-Speed (Jump):");
            mdw_slideri(    xa, ya, xb, bts,  0,0,0,0,  0,11,15,15,  1,0,1,d, Ei[n][4],  12, 1, 1,     "X-Speed (Ground):1/");
            mdw_sliderf(    xa, ya, xb, bts,  0,0,0,0,  0,12,15,15,  1,0,1,d, Efi[n][3], 9, 0.4, 0.01, "Y-Speed:");
            ya+=4; // spacer
            mdw_toggle(     xa, ya, xb, bts,  0,0,0,0,  0, 0, 0, 0,  1,0,1,d, Ei[n][2], "Initial Direction:Left ", "Initial Direction:Right", 13, 13, 15, 15);
            ya+=4; // spacer
            mdw_slider0(    xa, ya, xb, bts,  0,0,0,0,  0, 7,15,15,  1,0,1,d, Ei[n][10], 100, 0, 1,    "Turn Before Hole:", "Off");
            mdw_slider0(    xa, ya, xb, bts,  0,0,0,0,  0, 7,15,15,  1,0,1,d, Ei[n][11], 100, 0, 1,    "Jump Before Hole:", "Off");
            mdw_slider0(    xa, ya, xb, bts,  0,0,0,0,  0, 7,15,15,  1,0,1,d, Ei[n][12], 100, 0, 1,    "Jump Before Wall:", "Off");
            mdw_slider0(    xa, ya, xb, bts,  0,0,0,0,  0, 7,15,15,  1,0,1,d, Ei[n][8],  160, 0, 1,    "Wall Jump Boost:", "Off");
            mdw_slider0(    xa, ya, xb, bts,  0,0,0,0,  0, 7,15,15,  1,0,1,d, Ei[n][7],  600, 0, 1,    "Jump Under Width:", "Off");
            mdw_slider0(    xa, ya, xb, bts,  0,0,0,0,  0, 7,15,15,  1,0,1,d, Ei[n][6],  500, 0, 1,    "Jump Timer Wait:",  "Off");
            ya+=4; // spacer
            mdw_slideri(    xa, ya, xb, bts,  0,0,0,0,  0, 4,15,15,  1,0,1,d, Ei[n][29], 20, 0, 1,     "Collision Box:");
            mdw_sliderf(    xa, ya, xb, bts,  0,0,0,0,  0, 4,15,15,  1,0,1,d, Efi[n][4], 10, 0, 0.1,   "Health Decrement:");
            mdw_slideri(    xa, ya, xb, bts,  0,0,0,0,  0, 4,15,15,  1,0,1,d, Ei[n][25], 50, 0, 1,     "Health Bonus:");
         break;
         case 6: // cannon
            mdw_sliderf(    xa, ya, xb, bts,  22,n,0,0, 0, 9,15,15,  1,0,1,d, Efi[n][5], 12, 0, 0.01, "Speed:");
            ya+=4; // spacer
            mdw_slideri(    xa, ya, xb, bts,  0,0,0,0,  0,12,15,15,  1,0,1,d, Ei[n][8], 100, 0, 1,     "Seek Count:");
            if (mdw_buttont(xa, ya, xb, bts,  0,0,0,0,  0,12,15, 0,  1,0,1,d, "Set Initial Direction"))  getxy("Initial Direction", 96, 4, n);
            ya+=4; // spacer
            mdw_slideri(    xa, ya, xb, bts,  0,0,0,0,  0,8, 15,15,  1,0,1,d, Ei[n][9], 40, 0, 1,      "Extra Hits To Kill:");
            ya+=4; // spacer
            mdw_sliderf(    xa, ya, xb, bts,  0,0,0,0,  0,9, 15,15,  1,0,1,d, Efi[n][7], 20, 0.8, 0.1, "Bullet Speed:");
            mdw_slideri(    xa, ya, xb, bts,  0,0,0,0,  0,9, 15,15,  1,0,1,d, Ei[n][17], 2000, 20, 1,  "Bullet Proximity:");
            mdw_slideri(    xa, ya, xb, bts,  0,0,0,0,  0,9, 15,15,  1,0,1,d, Ei[n][15], 200, 1, 1,    "Bullet Retrigger Time:");
            ya+=4; // spacer
            mdw_slideri(    xa, ya, xb, bts,  0,0,0,0,  0,4, 15,15,  1,0,1,d, Ei[n][29], 20, 0, 1,     "Collision Box:");
            mdw_sliderf(    xa, ya, xb, bts,  0,0,0,0,  0,4, 15,15,  1,0,1,d, Efi[n][4], 10, 0, 0.1,   "Health Decrement:");
            mdw_slideri(    xa, ya, xb, bts,  0,0,0,0,  0,4, 15,15,  1,0,1,d, Ei[n][25], 50, 0, 1,     "Health Bonus:");
         break;
         case 7: // podzilla
            mdw_sliderf(    xa, ya, xb, bts,  29,n,0,0, 0, 9,15,15,  1,0,1,d, Efi[n][9], 30, 0.5, 0.5, "Speed:");
            mdw_slideri(    xa, ya, xb, bts,  0,0,0,0,  0, 9,15,15,  1,0,1,d, Ei[n][9], 240, 0, 1,      "Pause:");
            mdw_sliderf(    xa, ya, xb, bts,  0,0,0,0,  0, 9,15,15,  1,0,1,d, Efi[n][7], 20, 0.8, 0.1, "Bullet Speed:");
            ya+=4; // spacer
            if (mdw_buttont(xa, ya, xb, bts,  0,0,0,0,  0,10,15, 0,  1,0,1,d, "Move Extended Position")) getxy("Pod Extended Position", 99, 7, n);
            if (mdw_buttont(xa, ya, xb, bts,  0,0,0,0,  0,14,15, 0,  1,0,1,d, "Set Trigger Box")) get_block_range("Trigger Box", &Ei[n][11], &Ei[n][12], &Ei[n][13], &Ei[n][14], 2);
            ya+=4; // spacer
            mdw_slideri(    xa, ya, xb, bts,  0,0,0,0,  0, 4,15,15,  1,0,1,d, Ei[n][29], 20, 0, 1,     "Collision Box:");
            mdw_sliderf(    xa, ya, xb, bts,  0,0,0,0,  0, 4,15,15,  1,0,1,d, Efi[n][4], 10, 0, 0.1,   "Health Decrement:");
            mdw_slideri(    xa, ya, xb, bts,  0,0,0,0,  0, 4,15,15,  1,0,1,d, Ei[n][25], 50, 0, 1,     "Health Bonus:");
         break;
         case 8: // trakbot
            mdw_sliderf(    xa, ya, xb, bts,  0,0,0,0,  0,12,15,15,  1,0,1,d, Efi[n][2], 10, .5, 0.1, "X-Speed:");
            mdw_sliderf(    xa, ya, xb, bts,  0,0,0,0,  0,12,15,15,  1,0,1,d, Efi[n][3], 10, .5, 0.1, "Y-Speed:");
            ya+=4; // spacer
            mdw_button(     xa, ya, xb, bts,  11,n,0,0, 0,12,15, 0,  1,0,1,d);  // initial direction
            ya+=4; // spacer
            mdw_toggle(     xa, ya, xb, bts,  0,0,0,0,  0, 0, 0, 0,  1,0,1,d, Ei[n][7], "Drop Mode:Off", "Drop Mode:On ", 15, 15, 6+64, 6);
            ya+=4; // spacer
            mdw_sliderf(    xa, ya, xb, bts,  0,0,0,0,  0, 9,15,15,  1,0,1,d, Efi[n][7], 20, 0.8, 0.1, "Bullet Speed:");
            mdw_slideri(    xa, ya, xb, bts,  0,0,0,0,  0, 9,15,15,  1,0,1,d, Ei[n][17], 2000, 20, 1,  "Bullet Proximity:");
            mdw_slideri(    xa, ya, xb, bts,  0,0,0,0,  0, 9,15,15,  1,0,1,d, Ei[n][15], 200, 1, 1,    "Bullet Retrigger Time:");
            ya+=4; // spacer
            mdw_slideri(    xa, ya, xb, bts,  0,0,0,0,  0, 4,15,15,  1,0,1,d, Ei[n][29], 20, 0, 1,     "Collision Box:");
            mdw_sliderf(    xa, ya, xb, bts,  0,0,0,0,  0, 4,15,15,  1,0,1,d, Efi[n][4], 10, 0, 0.1,   "Health Decrement:");
            mdw_slideri(    xa, ya, xb, bts,  0,0,0,0,  0, 4,15,15,  1,0,1,d, Ei[n][25], 50, 0, 1,     "Health Bonus:");
         break;
         case 9: // cloner
            mdw_toggle(     xa, ya, xb, bts,  0,0,0,0,  0, 0, 0, 0,  1,0,1,d, Ei[n][30], "Not Invincible", "Invincible!", 15, 15, 12, 10);
            ya+=4; // spacer
            mdw_buttonp(    xa, ya, xb, bts,  27,n,0,0, 0,12,15, 0,  1,0,1,d, Ei[n][8]); // trigger type
            mdw_slideri(    xa, ya, xb, bts,  0,0,0,0,  0,12,15,15,  1,0,1,d, Ei[n][6], 1000, 20, 1, "Delay Timer:");
            ya+=4; // spacer
            mdw_slider0(    xa, ya, xb, bts,  0,0,0,0,  0,14,15,15,  1,0,1,d, Ei[n][9], 4800, 0, 1,  "Created Time To Live:", "-");
            mdw_slider0(    xa, ya, xb, bts,  0,0,0,0,  0,14,15,15,  1,0,1,d, Ei[n][10], 600, 0, 1,  "Max Created Objects:", "-");
            ya+=4; // spacer
            if (mdw_buttont(xa, ya, xb, bts,  0,0,0,0,  0,11,15, 0,  1,0,1,d, "Set Source Area")) get_block_range("Cloner Source Area", &Ei[n][15], &Ei[n][16], &Ei[n][19], &Ei[n][20], 1);
            if (mdw_buttont(xa, ya, xb, bts,  0,0,0,0,  0,10,15, 0,  1,0,1,d, "Set Destination")) getxy("Cloner Destination", 98, 9, n);
            if (mdw_buttont(xa, ya, xb, bts,  0,0,0,0,  0,14,15, 0,  1,0,1,d, "Set Trigger Box")) get_block_range("Trigger Box", &Ei[n][11], &Ei[n][12], &Ei[n][13], &Ei[n][14], 2);
            ya+=4; // spacer
            mdw_buttonp(    xa, ya, xb, bts,  81,0,0,0, 0,13,15, 0,  1,0,1,d, Ei[n][4]); // show boxes
            ya+=4; // spacer
            mdw_slideri(    xa, ya, xb, bts,  0,0,0,0,  0, 4,15,15,  1,0,1,d, Ei[n][29], 20, 0, 1,   "Collision Box:");
            mdw_sliderf(    xa, ya, xb, bts,  0,0,0,0,  0, 4,15,15,  1,0,1,d, Efi[n][4], 10, 0, 0.1, "Health Decrement:");
            mdw_slideri(    xa, ya, xb, bts,  0,0,0,0,  0, 4,15,15,  1,0,1,d, Ei[n][25], 50, 0, 1,   "Health Bonus:");
         break;
         case 11: // block walker
            mdw_sliderf(    xa, ya, xb, bts,  0,0,0,0,  0,12,15,15,  1,0,1,d, Efi[n][6], 9, 0.7, 0.01, "X-Speed:");
            mdw_sliderf(    xa, ya, xb, bts,  0,0,0,0,  0,12,15,15,  1,0,1,d, Efi[n][3], 9, 0.7, 0.01, "Y-Speed:");
            ya+=4; // spacer
            mdw_toggle(     xa, ya, xb, bts,  0,0,0,0,  0, 0, 0, 0,  1,0,1,d, Ei[n][2], "Initial Direction:Left ", "Initial Direction:Right", 13, 13, 15, 15);
            mdw_toggle(     xa, ya, xb, bts,  0,0,0,0,  0, 0, 0, 0,  1,0,1,d, Ei[n][8], "Follow Mode",             "Bounce Mode",             13, 13, 15, 15);
            ya+=4; // spacer
            mdw_slider0(    xa, ya, xb, bts,  0,0,0,0,  0, 7,15,15,  1,0,1,d, Ei[n][10], 100, 0, 1,    "Turn Before Hole:", "Off");
            mdw_slider0(    xa, ya, xb, bts,  0,0,0,0,  0, 7,15,15,  1,0,1,d, Ei[n][11], 100, 0, 1,    "Jump Before Hole:", "Off");
            mdw_slider0(    xa, ya, xb, bts,  0,0,0,0,  0, 7,15,15,  1,0,1,d, Ei[n][12], 100, 0, 1,    "Jump Before Wall:", "Off");
            mdw_slider0(    xa, ya, xb, bts,  0,0,0,0,  0, 7,15,15,  1,0,1,d, Ei[n][7],  600, 0, 1,    "Jump Under Width:", "Off");
            mdw_slider0(    xa, ya, xb, bts,  0,0,0,0,  0, 7,15,15,  1,0,1,d, Ei[n][6],  500, 0, 1,    "Jump Timer Wait:",  "Off");
            ya+=4; // spacer
            mdw_slideri(    xa, ya, xb, bts,  0,0,0,0,  0, 4,15,15,  1,0,1,d, Ei[n][29], 20, 0, 1,     "Collision Box:");
            mdw_sliderf(    xa, ya, xb, bts,  0,0,0,0,  0, 4,15,15,  1,0,1,d, Efi[n][4], 10, 0, 0.1,   "Health Decrement:");
            mdw_slideri(    xa, ya, xb, bts,  0,0,0,0,  0, 4,15,15,  1,0,1,d, Ei[n][25], 50, 0, 1,     "Health Bonus:");
         break;
         case 12: // flapper
            mdw_sliderf(    xa, ya, xb, bts,  0,0,0,0,  0,12,15,15,  1,0,1,d, Efi[n][5], 8, .5, 0.1,  "X-Speed:");
            mdw_sliderf(    xa, ya, xb, bts,  0,0,0,0,  0,12,15,15,  1,0,1,d, Efi[n][6], 1, 0.01, 0.01, "X-Accel:");
            mdw_toggle(     xa, ya, xb, bts,  0,0,0,0,  0, 0, 0, 0,  1,0,1,d, Ei[n][2], "Initial Direction:Left ", "Initial Direction:Right", 15, 15, 12, 12);
            ya+=4; // spacer
            mdw_sliderf(    xa, ya, xb, bts,  0,0,0,0,  0,13,15,15,  1,0,1,d, Efi[n][3], 5, 0, 0.01,      "Y-Speed:");
            mdw_sliderf(    xa, ya, xb, bts,  0,0,0,0,  0,13,15,15,  1,0,1,d, Efi[n][10], 8, .5, 0.1,     "Flap Speed:");
            mdw_slideri(    xa, ya, xb, bts,  0,0,0,0,  0,13,15,15,  1,0,1,d, Ei[n][21], 400, 0, 5,       "Flap Height:");
            mdw_slideri(    xa, ya, xb, bts,  0,0,0,0,  0,13,15,15,  1,0,1,d, Ei[n][20], 1000, -1000, 10, "Height Above Player:");
            ya+=4; // spacer
            mdw_sliderf(    xa, ya, xb, bts,  0,0,0,0,  0, 9,15,15,  1,0,1,d, Efi[n][7], 20, 0.8, 0.1, "Bullet Speed:");
            mdw_slideri(    xa, ya, xb, bts,  0,0,0,0,  0, 9,15,15,  1,0,1,d, Ei[n][17], 2000, 20, 1,  "Bullet Proximity:");
            mdw_slideri(    xa, ya, xb, bts,  0,0,0,0,  0, 9,15,15,  1,0,1,d, Ei[n][15], 200, 1, 1,    "Bullet Retrigger Time:");
            ya+=4; // spacer
            mdw_slideri(    xa, ya, xb, bts,  0,0,0,0,  0, 4,15,15,  1,0,1,d, Ei[n][29], 20, 0, 1,     "Collision Box:");
            mdw_sliderf(    xa, ya, xb, bts,  0,0,0,0,  0, 4,15,15,  1,0,1,d, Efi[n][4], 10, 0, 0.1,   "Health Decrement:");
            mdw_slideri(    xa, ya, xb, bts,  0,0,0,0,  0, 4,15,15,  1,0,1,d, Ei[n][25], 50, 0, 1,     "Health Bonus:");
         break;

         case 13: // vinepod
            mdw_slideri(    xa, ya, xb, bts,  0,0,0,0,  0, 9,15,15,  1,0,1,d, Ei[n][17], 400, 10, 10,  "Extend Time:");
            mdw_slideri(    xa, ya, xb, bts,  0,0,0,0,  0, 9,15,15,  1,0,1,d, Ei[n][19], 400,  0,  1,  "Pause:");
            mdw_sliderf(    xa, ya, xb, bts,  0,0,0,0,  0, 9,15,15,  1,0,1,d, Efi[n][7], 20, 0.8, 0.1, "Bullet Speed:");
            ya+=4; // spacer

            mdw_togglf(     xa, ya, xb, bts, 0,0,0,0,   0,0,0,0,    1,0,1,d, Ei[n][20], PM_ENEMY_VINEPOD_SHOW_PATH, "Hide Path","Show Path", 15+dim, 15, 14+dim, 14);
            mdw_togglf(     xa, ya, xb, bts, 0,0,0,0,   0,0,0,0,    1,0,1,d, Ei[n][20], PM_ENEMY_VINEPOD_INV_INIT, "Invincible At Rest:OFF", "Invincible At Rest:ON ", 15+dim, 15, 14+dim, 14);
            mdw_togglf(     xa, ya, xb, bts, 0,0,0,0,   0,0,0,0,    1,0,1,d, Ei[n][20], PM_ENEMY_VINEPOD_INV_EXTN, "Invincible Extended:OFF", "Invincible Extended:ON ", 15+dim, 15, 14+dim, 14);
            mdw_togglf(     xa, ya, xb, bts, 0,0,0,0,   0,0,0,0,    1,0,1,d, Ei[n][20], PM_ENEMY_VINEPOD_INV_MOVE, "Invincible Moving:OFF", "Invincible Moving:ON ", 15+dim, 15, 14+dim, 14);

            ya+=4; // spacer

            if (mdw_buttont(xa, ya, xb, bts,  0,0,0,0,  0,10,15, 0,  1,0,1,d, "Set Extended Position")) getxy("Vinepod Extended Position", 90, 13, n);
            if (mdw_buttont(xa, ya, xb, bts,  0,0,0,0,  0, 6,15, 0,  1,0,1,d, "Set Control Point 1")) getxy("Vinepod Control Point 1", 91, 13, n);
            if (mdw_buttont(xa, ya, xb, bts,  0,0,0,0,  0, 7,15, 0,  1,0,1,d, "Set Control Point 2")) getxy("Vinepod Control Point 2", 92, 13, n);
            if (mdw_buttont(xa, ya, xb, bts,  0,0,0,0,  0,14,15, 0,  1,0,1,d, "Set Trigger Box")) get_block_range("Trigger Box", &Ei[n][11], &Ei[n][12], &Ei[n][13], &Ei[n][14], 2);
            ya+=4; // spacer
            mdw_slideri(    xa, ya, xb, bts,  0,0,0,0,  0, 4,15,15,  1,0,1,d, Ei[n][29], 20, 0, 1,     "Collision Box:");
            mdw_sliderf(    xa, ya, xb, bts,  0,0,0,0,  0, 4,15,15,  1,0,1,d, Efi[n][4], 10, 0, 0.1,   "Health Decrement:");
            mdw_slideri(    xa, ya, xb, bts,  0,0,0,0,  0, 4,15,15,  1,0,1,d, Ei[n][25], 50, 0, 1,     "Health Bonus:");
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
            mdw_buttonp(   xa, ya, xb, bts, 22,0,0,0,  0,13,15,0, 1,0,1,d, item[n][3]); // stat | fall | carry
            ya+=4; // spacer
            mdw_button(    xa, ya, xb, bts, 49,n,0,0,  0,12,15,0, 1,0,1,d); // door type
            if (item[n][8]) // regular door (not exit only)
            {
               ya+=4; // spacer
               mdw_button( xa, ya, xb, bts,  4,n,0,0,  0,11,15,0, 1,0,1,d); // set linked item
               mdw_buttonp(xa, ya, xb, bts, 50,n,0,0,  0,11,15,0, 1,0,1,d, item[n][11]); // door entry type
               mdw_buttonp(xa, ya, xb, bts, 53,n,0,0,  0,11,15,0, 1,0,1,d, item[n][7]); // move type
               mdw_buttonp(xa, ya, xb, bts, 51,n,0,0,  0,11,15,0, 1,0,1,d, item[n][12]); // exit link show
            }
            ya+=4; // spacer
            mdw_button(    xa, ya, xb, bts, 52,n,0,0,  0,13,15,0, 1,0,1,d); // get new shape
            mdw_colsel(    xa, ya, xb, bts,  5,n,0,0,  0, 0, 0,0, 0,0,1,d); // change color
         break;

         case 2: // bonus
            if (item[n][6] == 1) // health bonus
            {
               mdw_buttonp(xa, ya, xb, bts, 22,0,0,0,   0,13,15,0,  1,0,1,d, item[n][3]); // stat | fall | carry
               ya+=4; // spacer
               mdw_buttonp(xa, ya, xb, bts, 101,0,0,0,  0,11,15,0,  1,0,1,d, item[n][6]); // bonus type
               mdw_slideri(xa, ya, xb, bts, 0,0,0,0,    0,11,15,15, 1,0,1,d, item[n][7], 100, 2, 1, "Health Bonus:");
               item[n][1] = 1035;
               item[n][2] = 1;
               if (item[n][7] == 10) item[n][1] = 1023;
               if (item[n][7] == 50) item[n][1] = 1027;
            }
            else if (item[n][6] == 2) // free man
            {
               mdw_buttonp(xa, ya, xb, bts, 21, 0,0,0,  0,13,15,0,  1,0,1,d, item[n][3]); // stat | fall
               ya+=4; // spacer
               mdw_buttonp(xa, ya, xb, bts, 101,0,0,0,  0,10,15,0,  1,0,1,d, item[n][6]); // bonus type
               item[n][1] = 1038;
               item[n][2] = 1;
            }
            else if (item[n][6] == 3) // purple coin
            {
               mdw_buttonp(xa, ya, xb, bts, 21, 0,0,0,  0,13,15,0, 1,0,1,d, item[n][3]); // stat | fall
               ya+=4; // spacer
               mdw_buttonp(xa, ya, xb, bts, 101,0,0,0,  0, 8,15,0, 1,0,1,d, item[n][6]); // bonus type
               item[n][1] = 197;
               item[n][2] = 0;
            }
         break;
         case 3: // exit
            mdw_buttonp(    xa, ya, xb, bts, 22,0,0,0,  0,13,15, 0,  1,0,1,d, item[n][3]); // stat | fall | carry
            ya+=4; // spacer
            mdw_slideri(    xa, ya, xb, bts, 0,0,0,0,   0,12,15,15,  1,0,1,d, item[n][8], 100, 0, 1,  "Enemy Count Lock:");
         break;
         case 4: // key
            mdw_buttonp(    xa, ya, xb, bts, 21,0,0,0,  0,13,15, 0,  1,0,1,d, item[n][3]); // stat | fall
            ya+=4; // spacer
            mdw_buttonp(    xa, ya, xb, bts, 102,0,0,0, 0, 8,15, 0,  1,0,1,d, item[n][1]); // color
            ya+=4; // spacer
            if (mdw_buttont(xa, ya, xb, bts, 0,0,0,0,   0,10,15, 0,  1,0,1,d, "Get New Block Range")) get_block_range("Block Range", &item[n][6], &item[n][7], &item[n][8], &item[n][9], 1);
            ya+=4; // spacer
            mdw_toggle(     xa, ya, xb, bts, 0,0,0,0,   0, 0, 0, 0,  1,0,1,d, item[n][12], "Remove All Blocks", "Remove Only Matching", 15, 15, 7, 7);
         break;
         case 5: // start
            mdw_buttonp(    xa, ya, xb, bts, 22,0,0,0,  0,13,15,0,   1,0,1,d, item[n][3]); // stat | fall | carry
            ya+=4; // spacer
            mdw_buttonp(    xa, ya, xb, bts, 78,0,0,0,  0,11,15,0,   1,0,1,d, item[n][6]); // start mode
            mdw_buttonp(    xa, ya, xb, bts, 79,0,0,0,  0,11,15,0,   1,0,1,d, item[n][7]); // start index
         break;
         case 6: // orb
            mdw_buttonp(    xa, ya, xb, bts, 22,0,0,0,  0,13,15,0,   1,0,1,d, item[n][3]); // stat | fall | carry
            ya+=4; // spacer
            mdw_button(     xa, ya, xb, bts, 94,n,0,0,  0, 7,15, 0,  1,0,1,d);             // rotation
            ya+=4; // spacer
            mdw_button(     xa, ya, xb, bts, 90,n,0,0,  0,12,15, 0,  1,0,1,d);             // orb trigger type
            ya+=4; // spacer
            mdw_button(     xa, ya, xb, bts, 92,n,0,0,  0, 8,15, 0,  1,0,1,d);             // mode
            if (item[n][6] == 0)                                                           // only show initial state for toggle mode
               mdw_togglf(  xa, ya, xb, bts, 0,0,0,0,   0,0,0,0,    1,0,1,d, item[n][2], PM_ITEM_ORB_STATE, "Initial State:OFF","Initial State:ON ", 15, 15, 8, 8);
            if ((item[n][6] == 3) || (item[n][6] == 4))                                    // only show timer for mode 3 and 4
               mdw_slideri(xa, ya, xb, bts,     0,0,0,0,   0,8,15,15,  1,0,1,d, item[n][7], 400, 1, 1,  "Time:");
            ya+=4; // spacer
            mdw_slider0(    xa, ya, xb, bts, 0,0,0,0,    0,13,15,15,  1,0,1,d, item[n][10], 99, 0, 1,    "Set Event Always While On   :", "OFF");
            mdw_slider0(    xa, ya, xb, bts, 0,0,0,0,    0,13,15,15,  1,0,1,d, item[n][11], 99, 0, 1,    "Set Event Always While Off  :", "OFF");
            mdw_slider0(    xa, ya, xb, bts, 0,0,0,0,    0,13,15,15,  1,0,1,d, item[n][12], 99, 0, 1,    "Set Event When Switching On :", "OFF");
            mdw_slider0(    xa, ya, xb, bts, 0,0,0,0,    0,13,15,15,  1,0,1,d, item[n][13], 99, 0, 1,    "Set Event When Switching Off:", "OFF");
         break;
         case 7: // mine
            mdw_buttonp(    xa, ya, xb, bts, 22,0,0,0,  0,13,15, 0,  1,0,1,d, item[n][3]); // stat | fall | carry
            ya+=4; // spacer
            mdw_slideri(    xa, ya, xb, bts, 0,0,0,0,   0,12,15,15,  1,0,1,d, item[n][8], 20, 1, 1,  "Mine Damage:");
         break;
         case 8: // bomb
            mdw_buttonp(    xa, ya, xb, bts, 22,0,0,0,  0,13,15, 0,  1,0,1,d, item[n][3]); // stat | fall | carry
            ya+=4; // spacer
            mdw_button(     xa, ya, xb, bts, 77,n,0,0,  0,12,15, 0,  1,0,1,d);          // fuse timer / remote detonator
            if (!item[n][12])
               mdw_slideri( xa, ya, xb, bts, 0,0,0,0,   0,12,15,15,  1,0,1,d, item[n][9], 2000, 1,  1,  "Fuse Length:");
            ya+=4; // spacer
            mdw_toggle(     xa, ya, xb, bts, 0,0,0,0,   0, 0, 0, 0,  1,0,1,d, item[n][11], "Sticky:Off", "Sticky:On", 15, 15, 4, 4);
            ya+=4; // spacer
            mdw_slideri(    xa, ya, xb, bts, 0,0,0,0,   0,14,15,15,  1,0,1,d, item[n][7], 1200, 20, 1,  "Damage Range:");
         break;
         case 10: // message

            mdw_togglf(     xa, ya, xb, bts, 0,0,0,0,   0,0,0,0,    1,0,1,d, item[n][2], PM_ITEM_PMSG_SHOW_SCROLL , "Show Scroll:OFF", "Show Scroll:ON",  15+dim, 15, 9+dim, 9);
            ya+=4; // spacer
            mdw_button(xa, ya, xb, bts, 6,n,0,0,   0,10,15,0,   1,0,1,d); // Set Message Area
            mdw_button(xa, ya, xb, bts, 7,n,0,0,   0,10,15,0,   1,0,1,d); // Set Message Frame
            ya+=4; // spacer
            mdw_togglf(     xa, ya, xb, bts, 0,0,0,0,   0,0,0,0,    1,0,1,d, item[n][2], PM_ITEM_PMSG_TRIGGER_BOX , "Trigger Box:OFF", "Trigger Box:ON",  15+dim, 15, 14+dim, 14);
            if (item[n][2] & PM_ITEM_PMSG_TRIGGER_BOX)
            if (mdw_buttont(xa, ya, xb, bts, 0,0,0,0,   0,14,15,0,  1,0,1,d, "Set Trigger Area")) get_block_range("Trigger Area", &item[n][6], &item[n][7], &item[n][8], &item[n][9], 1);
            ya+=4; // spacer
            mdw_togglf(     xa, ya, xb, bts, 0,0,0,0,   0,0,0,0,    1,0,1,d, item[n][2], PM_ITEM_PMSG_SHOW_ALWAYS , "Show Always:OFF", "Show Always:ON",  15+dim, 15, 12+dim, 12);
            if (!(item[n][2] & PM_ITEM_PMSG_SHOW_ALWAYS))
            mdw_slideri(xa, ya, xb, bts,     0,0,0,0,   0,12,15,15,  1,0,1,d, item[n][12], 400, 0, 1,  "Message display time:");
            ya+=4; // spacer
            mdw_colsel(     xa, ya, xb, bts, 2,n,0,0,   0, 0, 0, 0,  0,0,1,d);  // frame color select
            mdw_colsel(     xa, ya, xb, bts, 3,n,0,0,   0, 0, 0, 0,  0,0,1,d);  // text color select
            if (mdw_buttont(xa, ya, xb, bts, 0,0,0,0,   0,15,13, 0,  1,0,1,d, "Edit Message")) edit_pmsg_text(n, 0);
            ya+=bts*2; // leave space for OK and Cancel buttons
            // draw the current one last to ensure it is on top
            mwWM.mW[7].pop_msg_viewer_pos = ya+bts/2+2;

            draw_pop_message(n, 1, (xa+xb)/2, mwWM.mW[7].pop_msg_viewer_pos, 0, 0, pmsgtext[n]); // show the message

            ya+=bts*8;
         break;
         case 11: // rocket
            mdw_buttonp(    xa, ya, xb, bts, 23,0,0,0,  0,13,15,0,  1,0,1,d, item[n][3]); // stat | fall | | ride through door
            ya+=4; // spacer
            if (mdw_buttont(xa, ya, xb, bts, 0,0,0,0,   0,10,15,0,  1,0,1,d, "Set Initial Direction")) getxy("Initial Direction", 97, 11, n);
            ya+=4; // spacer
            mdw_slideri(    xa, ya, xb, bts, 0,0,0,0,   0,12,15,15, 1,0,1,d, item[n][8], 20,  1, 1,  "Maximum Speed:");
            mdw_slideri(    xa, ya, xb, bts, 0,0,0,0,   0,12,15,15, 1,0,1,d, item[n][9], 200, 1, 1,  "Acceleration:");
            mdw_slideri(    xa, ya, xb, bts, 0,0,0,0,   0,12,15,15, 1,0,1,d, item[n][6], 50,  1, 1,  "Steerability:");
            ya+=4; // spacer
            mdw_slideri(    xa, ya, xb, bts, 0,0,0,0,   0,14,15,15, 1,0,1,d, item[n][7], 1200, 20, 1,  "Damage Range:");

         break;
         case 14: // switch
            mdw_buttonp(    xa, ya, xb, bts, 22,0,0,0,  0,13,15,14, 1,0,1,d, item[n][3]); // stat | fall | carry
            ya+=4; // spacer
            if (mdw_buttont(xa, ya, xb, bts, 0,0,0,0,   0,10,15,0,  1,0,1,d, "Get New Block Range")) get_block_range("Block Range", &item[n][6], &item[n][7], &item[n][8], &item[n][9], 1);
            ya+=4; // spacer
            mdw_togglf(     xa, ya, xb, bts, 0,0,0,0,   0,0,0,0,    1,0,1,d, item[n][10], 0b0001 , "Green:OFF", "Green:ON",  15+dim, 15, 9+dim, 9);
            mdw_togglf(     xa, ya, xb, bts, 0,0,0,0,   0,0,0,0,    1,0,1,d, item[n][10], 0b0010 , "Red:OFF",   "Red:ON",    15+dim, 15, 10+dim, 10);
            mdw_togglf(     xa, ya, xb, bts, 0,0,0,0,   0,0,0,0,    1,0,1,d, item[n][10], 0b0100 , "Blue:OFF",  "Blue:ON",   15+dim, 15, 12+dim, 12);
            mdw_togglf(     xa, ya, xb, bts, 0,0,0,0,   0,0,0,0,    1,0,1,d, item[n][10], 0b1000 , "Purple:OFF","Purple:ON", 15+dim, 15, 8+dim, 8);
            set_switch_tile(n);
         break;
         case 15: // sproingy
            mdw_buttonp(    xa, ya, xb, bts, 22,0,0,0,  0,13,15, 0, 1,0,1,d, item[n][3]); // stat | fall | carry
            ya+=4; // spacer
            mdw_slideri(    xa, ya, xb, bts, 0,0,0,0,   0,12,15,15, 1,0,1,d, item[n][7], 200, 40, 1, "Sproinginess:" );
         break;
         case 12: // warp
            mdw_buttonp(    xa, ya, xb, bts, 21,0,0,0,  0,13,15, 0,  1,0,1,d, item[n][3]); // stat | fall
            ya+=4; // spacer
            mdw_slideri(    xa, ya, xb, bts, 0,0,0,0,   0,12,15,15,  1,0,1,d, item[n][8], 300, 1, 1, "Warp Level:" );
         break;
         case 9: // trigger
         {
            if (mdw_buttont(xa, ya, xb, bts, 0,0,0,0,    0,14,15,0,   1,0,1,d, "Get New Trigger Field")) get_block_range("Trigger Rectangle", &item[n][6], &item[n][7], &item[n][8], &item[n][9], 1);
            ya+=4; // spacer
            // draw trigger field on/off with optional color select if on
            if (mdw_togglf( xa, ya, xb, bts, 0,0,0,0,    0,0,0,0,     1,0,1,d, item[n][3], PM_ITEM_TRIGGER_DRAW_ON, "Draw Trigger Field:OFF","Draw Trigger Field:ON", 15+dim, 15, 15+dim, item[n][2]))
               mdw_colsel(  xa, ya, xb, bts, 6,n,0,0,    0,0,0,0,     0,0,1,d); // color select
            ya+=4; // spacer
            mdw_togglf(     xa, ya, xb, bts, 0,0,0,0,    0,0,0,0,     1,0,1,d, item[n][3], PM_ITEM_TRIGGER_PLAYER, "Triggered by Players:OFF         ","Triggered by Players:ON          ", 15+dim, 15, 9+dim, 9);
            mdw_togglf(     xa, ya, xb, bts, 0,0,0,0,    0,0,0,0,     1,0,1,d, item[n][3], PM_ITEM_TRIGGER_ENEMY,  "Triggered by Enemies:OFF         ","Triggered by Enemies:ON          ", 15+dim, 15, 9+dim, 9);
            mdw_togglf(     xa, ya, xb, bts, 0,0,0,0,    0,0,0,0,     1,0,1,d, item[n][3], PM_ITEM_TRIGGER_ITEM,   "Triggered by Items:OFF           ","Triggered by Items:ON            ", 15+dim, 15, 9+dim, 9);
            mdw_togglf(     xa, ya, xb, bts, 0,0,0,0,    0,0,0,0,     1,0,1,d, item[n][3], PM_ITEM_TRIGGER_PSHOT,   "Triggered by Player's Bullets:OFF","Triggered by Player's Bullets:ON ", 15+dim, 15, 9+dim, 9);
            mdw_togglf(     xa, ya, xb, bts, 0,0,0,0,    0,0,0,0,     1,0,1,d, item[n][3], PM_ITEM_TRIGGER_ESHOT,   "Triggered by Enemy's Bullets:OFF ","Triggered by Enemy's Bullets:ON  ", 15+dim, 15, 9+dim, 9);
            ya+=4; // spacer
            mdw_slider0(    xa, ya, xb, bts, 0,0,0,0,    0,13,15,15,  1,0,1,d, item[n][11], 99, 0, 1,    "Set Event Always While On   :", "OFF");
            mdw_slider0(    xa, ya, xb, bts, 0,0,0,0,    0,13,15,15,  1,0,1,d, item[n][12], 99, 0, 1,    "Set Event Always While Off  :", "OFF");
            mdw_slider0(    xa, ya, xb, bts, 0,0,0,0,    0,13,15,15,  1,0,1,d, item[n][13], 99, 0, 1,    "Set Event When Switching On :", "OFF");
            mdw_slider0(    xa, ya, xb, bts, 0,0,0,0,    0,13,15,15,  1,0,1,d, item[n][14], 99, 0, 1,    "Set Event When Switching Off:", "OFF");
            ya+=4; // spacer
            if (mdw_togglf( xa, ya, xb, bts, 0,0,0,0,    0,0,0,0,     1,0,1,d, item[n][3], PM_ITEM_TRIGGER_LIFT_ON, "Follows Lift:OFF","Follows Lift:ON ", 15+dim, 15, 6+dim, 6))
            {
               mdw_slideri( xa, ya, xb, bts, 0,0,0,0,    0,6,15,15,   1,0,1,d,item[n][10], 39, 0, 1, "Lift Number:" );  // Lift Number
               mdw_button(  xa, ya, xb, bts, 211,n,0,0,  0,6,15,0,    1,0,1,d);  // X Alignment
               mdw_button(  xa, ya, xb, bts, 212,n,0,0,  0,6,15,0,    1,0,1,d);  // Y Alignment
            }
         }
         break;
         case 16: // block manip
            if (mdw_buttont(xa, ya, xb, bts, 0,0,0,0,    0,14,15,14,  1,0,1,d, "Get New Block Manip Field")) get_block_range("Block Manip Rectangle", &item[n][6], &item[n][7], &item[n][8], &item[n][9], 1);
            ya+=4; // spacer
            if (mdw_toggle( xa, ya, xb, bts, 0,0,0,0,    0,0,0,0,     1,0,1,d, item[n][2], "Draw Block Manip Field:OFF", "Draw Block Manip Field:ON ", 15, 15, 15+96, item[n][12]))
               mdw_colsel(  xa, ya, xb, bts, 7,n,0,0,    0,0,0,0,     0,0,1,d);               // color select
            ya+=4; // spacer
            mdw_buttonp(    xa, ya, xb, bts, 301,0,0,0,  0,14,15,0,   1,0,0,d, item[n][3]);   // mode
            ya+=22; // spacer
            mdw_button(     xa, ya, xb, bts, 310,n,0,0,  0, 8,15,0,   1,0,0,d);               // block 1
            ya+=22; // spacer
            mdw_button(     xa, ya, xb, bts, 311,n,0,0,  0, 8,15,0,   1,0,0,d);               // block 2
            ya+=22; // spacer
            mdw_slider0(    xa, ya, xb, bts, 0,0,0,0,    0,13,15,15,  1,0,1,d, item[n][1], 99, 0, 1,    "Event Trigger:", "OFF");
            mdw_button(     xa, ya, xb, bts, 320,n,0,0,  0,13,15,0,   1,0,1,d);               // set trigger
         break;
         case 17: // block damage
         {
            if (mdw_buttont(   xa, ya, xb, bts, 0,0,0,0,   0,10,15,0,  1,0,1,d, "Get New Block Damage Field")) get_block_range("Block Damage Rectangle", &item[n][6], &item[n][7], &item[n][8], &item[n][9], 1);
            ya+=4; // spacer
            mdw_buttonp(       xa, ya, xb, bts, 404,0,0,0, 0, 8,15,0,  1,0,1,d, item[n][2]); // damage draw mode
            ya+=4; // spacer
            int p=7; // mode color
            mdw_buttonp(       xa, ya, xb, bts, 402,n,0,0, 0,p,15,0,   1,0,1,d, item[n][11]); // MODE
            int MODE = item[n][11];

            ya+=4; // spacer
            if (MODE == 1) // toggle
               mdw_togglf(     xa, ya, xb, bts, 0,0,0,0,   0,0,0,0,    1,0,1,d, item[n][3], PM_ITEM_DAMAGE_CURR, "Initial State:OFF","Initial State:ON ", 15+dim, 15, p+dim, p);
            if (MODE == 2) // ON until triggered
               mdw_slideri(    xa, ya, xb, bts, 0,0,0,0,   0,p,15,15,  1,0,1,d,item[n][12], 1000, 0, 1, "OFF Time:" );
            if (MODE == 3) // OFF until triggered
               mdw_slideri(    xa, ya, xb, bts, 0,0,0,0,   0,p,15,15,  1,0,1,d,item[n][12], 1000, 0, 1, "ON Time:" );
            if (MODE == 4) // Timed on and off
            {
               mdw_slideri(    xa, ya, xb, bts, 0,0,0,0,   0,p,15,15,  1,0,1,d,item[n][12], 1000, 0, 1, "Total Time:" );
               mdw_slideri(    xa, ya, xb, bts, 0,0,0,0,   0,p,15,15,  1,0,1,d,item[n][14], 1000, 0, 1, "Damage Time:" );
               mdw_slideri(    xa, ya, xb, bts, 0,0,0,0,   0,p,15,15,  1,0,1,d,item[n][13], 1000, 0, 1, "Initial Time:" );
            }
            if ((MODE == 2) || (MODE == 3) || (MODE == 4))
            {
               ya+=4; // spacer
               mdw_button(     xa, ya, xb, bts, 401,n,0,0, 0,12,15,15, 1,0,1,d); // timer draw mode
            }
            if ((MODE == 1) || (MODE == 2) || (MODE == 3)) // Mode 1, 2, and 3
            {
               ya+=4; // spacer
               mdw_slider0(    xa, ya, xb, bts, 0,0,0,0,   0,13,15,15, 1,0,1,d, item[n][1], 99, 0, 1, "Event Trigger:", "OFF");
               mdw_button(     xa, ya, xb, bts, 320,n,0,0, 0,13,15, 0, 1,0,1,d); // set trigger
            }
            ya+=4; // spacer
            if (mdw_togglf(    xa, ya, xb, bts, 0,0,0,0,   0,0,0,0,    1,0,1,d, item[n][3], PM_ITEM_DAMAGE_PLAYER,  "Affects Players:OFF",          "Affects Players:ON",          15+dim, 15, 10+dim, 10))
               if (!mdw_togglf(xa, ya, xb, bts, 0,0,0,0,   0,0,0,0,    1,0,1,d, item[n][3], PM_ITEM_DAMAGE_INSTGIB, "Player Instant Death:OFF",     "Player Instant Death:ON",     15+dim, 15, 10+dim, 10))
                  mdw_slideri( xa, ya, xb, bts, 0,0,0,0,   0,10,15,15, 1,0,1,d, item[n][15], 2000, -2000, 1,        "Player Damage:" );
            mdw_togglf(        xa, ya, xb, bts, 0,0,0,0,   0,0,0,0,    1,0,1,d, item[n][3], PM_ITEM_DAMAGE_ENEMY,   "Affects Enemies:OFF",          "Affects Enemies:ON",          15+dim, 15, 10+dim, 10);
            mdw_togglf(        xa, ya, xb, bts, 0,0,0,0,   0,0,0,0,    1,0,1,d, item[n][3], PM_ITEM_DAMAGE_ITEM,    "Affects Items:OFF",            "Affects Items:ON",            15+dim, 15, 10+dim, 10);
            mdw_togglf(        xa, ya, xb, bts, 0,0,0,0,   0,0,0,0,    1,0,1,d, item[n][3], PM_ITEM_DAMAGE_PSHOT,    "Affects Player's Bullets:OFF", "Affects Player's Bullets:ON", 15+dim, 15, 10+dim, 10);
            mdw_togglf(        xa, ya, xb, bts, 0,0,0,0,   0,0,0,0,    1,0,1,d, item[n][3], PM_ITEM_DAMAGE_ESHOT,    "Affects Enemy's Bullets:OFF",  "Affects Enemy's Bullets:ON",  15+dim, 15, 10+dim, 10);
            ya+=4; // spacer
            if (mdw_togglf(    xa, ya, xb, bts, 0,0,0,0,   0,0,0,0,    1,0,1,d, item[n][3], PM_ITEM_DAMAGE_LIFT_ON, "Follows Lift:OFF",             "Follows Lift:ON ",            15+dim, 15, 6+dim, 6))
            {
               mdw_slideri(    xa, ya, xb, bts, 0,0,0,0,   0,6,15,15,  1,0,1,d,item[n][10], 39, 0, 1, "Lift Number:" );
               mdw_button(     xa, ya, xb, bts, 411,n,0,0, 0,6,15,0,   1,0,1,d);  // X Alignment
               mdw_button(     xa, ya, xb, bts, 412,n,0,0, 0,6,15,0,   1,0,1,d);  // Y Alignment
            }
         }
         break;
      }
   }
   // set height
   if (obt != 4)
   {
      int llo = (3 - mwWM.mW[7].num_legend_lines)*8; // legend line offset
      mwWM.mW[7].h = ya-y1+32-llo;
      mwWM.mW[7].set_size(mwWM.mW[7].w, mwWM.mW[7].h);
   }
}
void ov_draw_overlays(int legend_highlight)
{
   int obt = mwWM.mW[7].obt;
   int num = mwWM.mW[7].num;

   al_set_target_bitmap(mwB.level_buffer);

   if (obt == 4)  // lifts
   {
      int lift = num;
      int step = lifts[lift].current_step;
      int color = (lift_steps[lift][step].type >> 28) & 15;

      int x1 = lift_steps[lift][step].x-1;
      int y1 = lift_steps[lift][step].y-1;
      int x2 = x1 + lift_steps[lift][step].w+2;
      int y2 = y1 + lift_steps[lift][step].h+2;
      int xc = (x1 + x2) / 2;
      int yc = (y1 + y2) / 2;

      al_draw_rectangle(x1, y1, x2, y2, mC.pc[color], 1);

      al_draw_line(xc, 0, xc, y1, mC.pc[color], 1);
      al_draw_line(xc, y2, xc, 2000, mC.pc[color], 1);
      al_draw_line(0, yc, x1, yc, mC.pc[color], 1);
      al_draw_line(x2, yc, 2000, yc, mC.pc[color], 1);
   }
   if (obt == 3)  // enemies
   {
      int type = Ei[num][0];
      int obj_x = al_fixtoi(Efi[num][0])+10;
      int obj_y = al_fixtoi(Efi[num][1])+10;

      int color = 13;
      if (legend_highlight == 1) color = mC.flash_color;
      crosshairs_full(obj_x, obj_y, color, 1);

      if ((type == 3) || (type == 6) || (type == 8) || (type == 12)) // archwagon, cannon, trakbot, flapper
      {
         // draw yellow shot prox circle
         int color = 14;
         if (legend_highlight == 2) color = mC.flash_color;
         al_draw_circle(obj_x, obj_y, Ei[num][17], mC.pc[color], 1);
      }
      if (type == 7) // podzilla
      {
         // extended position
         int color1 = 10;
         if (legend_highlight == 2) color1 = mC.flash_color;

         int px=0, py=0;
         get_pod_extended_position(num, &px, &py);
         crosshairs_full(px+10, py+10, color1, 1);

         // draw tile at extended pos
         float rot = al_fixtof(al_fixmul(Efi[num][14], al_fixtorad_r));
         al_draw_scaled_rotated_bitmap(mwB.tile[Ei[num][1]], 10, 10, px+10, py+10, 1, 1, rot, ALLEGRO_FLIP_HORIZONTAL);

         // draw connecting line
         al_draw_line(obj_x, obj_y, px+10, py+10, mC.pc[10], 1);

         // trigger box
         int color = 14;
         if (legend_highlight == 3) color = mC.flash_color;
         int tx1 = Ei[num][11];
         int ty1 = Ei[num][12];
         int tx2 = Ei[num][11]+Ei[num][13] + 20;
         int ty2 = Ei[num][12]+Ei[num][14] + 20;
         al_draw_rectangle(tx1, ty1, tx2, ty2, mC.pc[color], 1);
      }

      if (type == 13) // vinepod
      {
         draw_vinepod_controls(num, legend_highlight);

      }


      if (type == 9) // cloner
      {
         int color2 = 11;
         if (legend_highlight == 2) color2 = mC.flash_color;

         int color3 = 10;
         if (legend_highlight == 3) color3 = mC.flash_color;

         int color4 = 14;
         if (legend_highlight == 4) color4 = mC.flash_color;

         int cw = Ei[num][19];     // width
         int ch = Ei[num][20];     // height

         int cx1 = Ei[num][15];    // source
         int cy1 = Ei[num][16];
         int cx2 = cx1 + cw;
         int cy2 = cy1 + ch;
         al_draw_rectangle(cx1, cy1, cx2, cy2, mC.pc[color2], 1);

         int cx3 = Ei[num][17];    // destination
         int cy3 = Ei[num][18];
         int cx4 = cx3 + cw;
         int cy4 = cy3 + ch;
         al_draw_rectangle(cx3, cy3, cx4, cy4, mC.pc[color3], 1);

         // draw trigger box
         int tx1 = Ei[num][11];
         int ty1 = Ei[num][12];
         int tx2 = Ei[num][11]+Ei[num][13] + 20;
         int ty2 = Ei[num][12]+Ei[num][14] + 20;
         al_draw_rectangle(tx1, ty1, tx2, ty2, mC.pc[color4], 1);
      }
      if (type == 12) // flapper
      {
         int color = 10;
         if (legend_highlight == 3) color = mC.flash_color;

         // draw red height above player line
         int hab = Ei[num][20];
         al_draw_line(obj_x-40, obj_y+hab, obj_x+40, obj_y+hab, mC.pc[color], 3);

         // draw flap height
         int fh = Ei[num][21];
         al_draw_line(obj_x-60, obj_y+fh, obj_x+60, obj_y+fh, mC.pc[12], 1);
         al_draw_line(obj_x-60, obj_y-fh, obj_x+60, obj_y-fh, mC.pc[12], 1);
      }
   }
   if (obt == 2)  // items
   {
      int type = item[num][0];
      int obj_x = item[num][4]+10;
      int obj_y = item[num][5]+10;
      int color = 13;
      if (legend_highlight == 1) color = mC.flash_color;
      crosshairs_full(obj_x, obj_y, color, 1);
      switch (type)
      {
         case 1: // door
         {
            int color = 10;
            if (legend_highlight == 2) color = mC.flash_color;
            if (item[num][8] == 0)  // exit only, no destination
            {
               // find src door(s)
               int num_src = 0;
               for (int i=0; i<500; i++)
                  if ((item[i][0] == 1) && (item[i][9] == num))
                  {
                     num_src++;
                     int x2 = item[i][4]+10;
                     int y2 = item[i][5]+10;
                     crosshairs_full(x2, y2, color, 1);
                     al_draw_line(obj_x, obj_y, x2, y2, mC.pc[color], 1);
                  }
            }

            if (item[num][8] == 1)  // draw destination
            {
               // dest item
               int di = item[num][9];
               int x2 = item[di][4]+10;
               int y2 = item[di][5]+10;
               crosshairs_full(x2, y2, color, 1);
               al_draw_line(obj_x, obj_y, x2, y2, mC.pc[color], 1);
            }
         }
         break;
         case 4: // key
         {
            int color = 10;
            if (legend_highlight == 2) color = mC.flash_color;
            int x2 = item[num][6];
            int y2 = item[num][7];
            int x3 = x2 + item[num][8] - 1;
            int y3 = y2 + item[num][9] - 1;;
            int x4 = (x2+x3)/2;
            int y4 = (y2+y3)/2;

            if (x2 == 0) x2 = 1; // to keep it visible
            if (y2 == 0) y2 = 1;

            // draw range
            al_draw_line(0, y4, 1999, y4, mC.pc[color], 1);
            al_draw_line(x4, 0, x4, 1999, mC.pc[color], 1);
            al_draw_rectangle(x2, y2, x3, y3, mC.pc[color], 1);

            // show blocks that will be affected
            proc_key_block_range(num, 2);
         }
         break;
         case 14: // switch
         {
            int color = 10;
            if (legend_highlight == 2) color = mC.flash_color;
            int x2 = item[num][6];
            int y2 = item[num][7];
            int x3 = x2 + item[num][8] - 1;
            int y3 = y2 + item[num][9] - 1;;
            int x4 = (x2+x3)/2;
            int y4 = (y2+y3)/2;

            if (x2 == 0) x2 = 1; // to keep it visible
            if (y2 == 0) y2 = 1;

            // draw range
            al_draw_line(0, y4, 1999, y4, mC.pc[color], 1);
            al_draw_line(x4, 0, x4, 1999, mC.pc[color], 1);
            al_draw_rectangle(x2, y2, x3, y3, mC.pc[color], 1);

            // show blocks that will be affected
            proc_switch_block_range(num, 2);
         }
         break;
         case 8: // bomb
         {
            int color = 14;
            if (legend_highlight == 2) color = mC.flash_color;
            al_draw_circle(obj_x, obj_y, item[num][7], mC.pc[color], 1); // draw yellow bomb damage
         }
         break;
         case 9: // trigger
         {
            int color = 14;
            if (legend_highlight == 2) color = mC.flash_color;

            int x2 = item[num][6];
            int y2 = item[num][7];
            int x3 = x2 + item[num][8] - 1;
            int y3 = y2 + item[num][9] - 1;;
            int x4 = (x2+x3)/2;
            int y4 = (y2+y3)/2;

            // draw range
            al_draw_line(0, y4, 1999, y4, mC.pc[color], 1);
            al_draw_line(x4, 0, x4, 1999, mC.pc[color], 1);
            al_draw_rectangle(x2, y2, x3, y3, mC.pc[color], 1);

            mwPME.find_and_show_event_links(1, num, 0);
         }
         break;
         case 10: // pmsg
         {
            int color = 10;
            if (legend_highlight == 2) color = mC.flash_color;

            int mx=0, my=0, mw=0, mh=0;
            get_int_3216(item[num][10], mx, my);
            get_int_3216(item[num][11], mw, mh);
            int x2 = mx;
            int y2 = my;
            int x3 = mx + mw;
            int y3 = my + mh;
            int x4 = (x2+x3)/2;
            int y4 = (y2+y3)/2;

            al_draw_textf(mF.pr8, mC.pc[15], x4, y2-10, ALLEGRO_ALIGN_CENTRE, "x:%d y:%d", mx, my);
            al_draw_textf(mF.pr8, mC.pc[15], x4, y3+2,  ALLEGRO_ALIGN_CENTRE, "w:%d h:%d", mw, mh);

            al_draw_line(0, y4, 1999, y4, mC.pc[color], 1);
            al_draw_line(x4, 0, x4, 1999, mC.pc[color], 1);
            al_draw_rectangle(x2, y2, x3, y3, mC.pc[color], 1);

            if (item[num][2] & PM_ITEM_PMSG_TRIGGER_BOX)
            {
               int color = 14;
               if (legend_highlight == 3) color = mC.flash_color;

               int x2 = item[num][6];
               int y2 = item[num][7];
               int x3 = x2 + item[num][8] - 1;
               int y3 = y2 + item[num][9] - 1;;
               int x4 = (x2+x3)/2;
               int y4 = (y2+y3)/2;

               al_draw_line(0, y4, 1999, y4, mC.pc[color], 1);
               al_draw_line(x4, 0, x4, 1999, mC.pc[color], 1);
               al_draw_rectangle(x2, y2, x3, y3, mC.pc[color], 1);
            }
         }
         break;
         case 11: // rocket
         {
            int color = 14;
            if (legend_highlight == 2) color = mC.flash_color;
            al_draw_circle(obj_x, obj_y, item[num][7], mC.pc[color], 1); // draw yellow bomb damage
         }
         break;
         case 15: // sproingy
         {
            int color = 14;
            if (legend_highlight == 2) color = mC.flash_color;
            int y = al_fixtoi(get_sproingy_jump_height(num));
            crosshairs_full(obj_x, obj_y-y, color, 1);
         }
         break;
         case 16: // block manip
         {
            int color = 12;
            if (legend_highlight == 2) color = mC.flash_color;
            int x2 = item[num][6];
            int y2 = item[num][7];
            int x3 = x2 + item[num][8]-1;
            int y3 = y2 + item[num][9]-1;;
            int x4 = (x2+x3)/2;
            int y4 = (y2+y3)/2;

            // draw range
            al_draw_line(0, y4, 1999, y4, mC.pc[color], 1);
            al_draw_line(x4, 0, x4, 1999, mC.pc[color], 1);
            al_draw_rectangle(x2, y2, x3, y3, mC.pc[color], 1);

            mwPME.find_and_show_event_links(2, num, 0);

         }
         break;
         case 17: // block damage
         {
            int color = 10;
            if (legend_highlight == 2) color = mC.flash_color;
            int x2 = item[num][6];
            int y2 = item[num][7];
            int x3 = x2 + item[num][8]-1;
            int y3 = y2 + item[num][9]-1;;
            int x4 = (x2+x3)/2;
            int y4 = (y2+y3)/2;

            // draw range
            al_draw_line(0, y4, 1999, y4, mC.pc[color], 1);
            al_draw_line(x4, 0, x4, 1999, mC.pc[color], 1);
            al_draw_rectangle(x2, y2, x3, y3, mC.pc[color], 1);

            mwPME.find_and_show_event_links(2, num, 0);
         }
         break;

      } // end of switch case
   }
}
void ov_process_mouse(void)
{

   int lift=0, step=0;
   if (mwWM.mW[7].obt == 4)
   {
      lift = mwWM.mW[7].num;
      step = lifts[lift].current_step;
   }

   int mouse_on_obj = 0;
   int mouse_on_podx = 0;

   int mouse_on_vpodx = 0;
   int mouse_on_vpod1 = 0;
   int mouse_on_vpod2 = 0;

   int mouse_on_csb_ul = 0;
   int mouse_on_csb_lr = 0;
   int mouse_on_cdb_ul = 0;
   int mouse_on_trk = 0;





   int mouse_on_sp = 0;
   int mouse_on_bmb = 0;
   int mouse_on_msg_ul = 0;
   int mouse_on_msg_lr = 0;

   int mouse_on_kbr_ul = 0;
   int mouse_on_kbr_lr = 0;
   int mouse_on_tb_ul = 0;
   int mouse_on_tb_lr = 0;

   int mouse_on_lift = 0;

   int mouse_move = 0;
   int mouse_adj = 0;

   int mst = 6;      // mouse select threshold
   int msn = 10-mst; // mouse select negative threshold
   int msp = 10+mst; // mouse select positive threshold


   // -----------------------------------------------------------
   // --  detect if mouse pointer is on enemy
   // -----------------------------------------------------------
   for (int b=0; b<100; b++)
      if (Ei[b][0])
      {
         int ex = al_fixtoi(Efi[b][0]);
         int ey = al_fixtoi(Efi[b][1]);
         int type = Ei[b][0];

         // check to see if we can set this object to be the current object
         if ((mwWM.hx>ex+msn) && (mwWM.hx<ex+msp) && (mwWM.hy>ey+msn) && (mwWM.hy<ey+msp) && (!mwWM.mW[7].viewer_lock) && (!mI.key[MAP_LOCK_KEY][0]) && (mwWM.obj_filter[3][type]))
         {
            // set this enemy to current object
            mwWM.mW[7].obt = 3;
            mwWM.mW[7].num = b;
            mouse_move = 1;
            mouse_on_obj = 1;
         }

         // if this object is already current object
         if ((mwWM.hx>ex+msn) && (mwWM.hx<ex+msp) && (mwWM.hy>ey+msn) && (mwWM.hy<ey+msp) && (mwWM.mW[7].obt == 3) && (mwWM.mW[7].num == b))
         {
            mouse_move = 1;
            mouse_on_obj = 1;
         }
      }

   // if current object is enemy, check for secondaries
   if (mwWM.mW[7].obt == 3)
   {
      int b = mwWM.mW[7].num;
      int type = Ei[b][0];
      if ((type == 3) || (type == 6) || (type == 8) || (type == 12)) // archwagon, cannon, trakbot and flapper shot prox
      {
         float x0 = al_fixtof(Efi[b][0])+10; // get center of item location
         float y0 = al_fixtof(Efi[b][1])+10;
         float fx = (float) mwWM.hx;
         float fy = (float) mwWM.hy;
         float dst = sqrt(pow((x0-fx), 2) + pow((y0-fy), 2)); // distance from mouse
         float bdr = (float) Ei[b][17]; // prox radius
         float dif = dst-bdr; // difference
         if ((dif < 3 ) && (dif > -3))  // mouse is on prox circle
         {
            mouse_on_trk = 1;
            mouse_move = 1;
         }
      }
      if (type == 7) // podzilla extended position
      {
         int px=0, py=0;
         get_pod_extended_position(b, &px, &py);
         if ((mwWM.hx>px+msn) && (mwWM.hx<px+msp) && (mwWM.hy>py+msn) && (mwWM.hy<py+msp))
         {
            mouse_move = 1;
            mouse_on_podx = 1;
         }
      }

      if (type == 13) // vinepod extended position
      {
         int px = Ei[b][9];
         int py = Ei[b][10];
         if ((mwWM.hx>px+msn) && (mwWM.hx<px+msp) && (mwWM.hy>py+msn) && (mwWM.hy<py+msp))
         {
            mouse_move = 1;
            mouse_on_vpodx = 1;
         }

         px = Ei[b][5];
         py = Ei[b][6];
         if ((mwWM.hx>px+msn) && (mwWM.hx<px+msp) && (mwWM.hy>py+msn) && (mwWM.hy<py+msp))
         {
            mouse_move = 1;
            mouse_on_vpod1 = 1;
         }

         px = Ei[b][7];
         py = Ei[b][8];
         if ((mwWM.hx>px+msn) && (mwWM.hx<px+msp) && (mwWM.hy>py+msn) && (mwWM.hy<py+msp))
         {
            mouse_move = 1;
            mouse_on_vpod2 = 1;
         }

      }



      if ((type == 7) || (type == 13) || (type == 9)) // podzilla, vinepod and cloner trigger box
      {
         int x1 = Ei[b][11];
         int y1 = Ei[b][12];
         int x2 = Ei[b][11]+Ei[b][13]+20;
         int y2 = Ei[b][12]+Ei[b][14]+20;
         if ((mwWM.hx>x1-mst) && (mwWM.hx<x1+mst) && (mwWM.hy>y1-mst) && (mwWM.hy<y1+mst)) // upper left corner (move)
         {
            mouse_on_tb_ul = 1;
            mouse_move = 1;
         }
         if ((mwWM.hx>x2-mst) && (mwWM.hx<x2+mst) && (mwWM.hy>y2-mst) && (mwWM.hy<y2+mst)) // lower right corner (resize)
         {
            mouse_on_tb_lr = 1;
            mouse_adj = 1;
         }
      }
      if (type == 9) // cloner source and destination boxes
      {
         int w = Ei[mwWM.mW[7].num][19];     // width
         int h = Ei[mwWM.mW[7].num][20];     // height
         int x1 = Ei[mwWM.mW[7].num][15];    // source box
         int y1 = Ei[mwWM.mW[7].num][16];
         int x2 = x1 + w - 1;
         int y2 = y1 + h - 1;
         int x3 = Ei[mwWM.mW[7].num][17];    // dest box
         int y3 = Ei[mwWM.mW[7].num][18];
         if ((mwWM.hx>x1-mst) && (mwWM.hx<x1+mst) && (mwWM.hy>y1-mst) && (mwWM.hy<y1+mst)) // source upper left corner (move)
         {
            mouse_on_csb_ul = 1;
            mouse_move = 1;
         }
         if ((mwWM.hx>x2-mst) && (mwWM.hx<x2+mst) && (mwWM.hy>y2-mst) && (mwWM.hy<y2+mst)) // source lower right corner (re-size)
         {
            mouse_on_csb_lr = 1;
            mouse_adj = 1;
         }
         if ((mwWM.hx>x3-mst) && (mwWM.hx<x3+mst) && (mwWM.hy>y3-mst) && (mwWM.hy<y3+mst)) // destination upper left corner (move)
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
      if (item[b][0])
      {
         int ix = item[b][4];
         int iy = item[b][5];
         int type = item[b][0];
         // check to see if we can set this object to be the current object
         if ((mwWM.hx>ix+msn) && (mwWM.hx<ix+msp) && (mwWM.hy>iy+msn) && (mwWM.hy<iy+msp) && (!mwWM.mW[7].viewer_lock) && (!mI.key[MAP_LOCK_KEY][0]) && (mwWM.obj_filter[2][type]))
         {
            // set this item to current object
            mwWM.mW[7].obt = 2;
            mwWM.mW[7].num = b;
            mouse_move = 1;
            mouse_on_obj = 1;
         }
         // if this object is already current object
         if ((mwWM.hx>ix+msn) && (mwWM.hx<ix+msp) && (mwWM.hy>iy+msn) && (mwWM.hy<iy+msp) && (mwWM.mW[7].obt == 2) && (mwWM.mW[7].num == b))
         {
            mouse_move = 1;
            mouse_on_obj = 1;
         }
      }

   // if current object is item, check for secondaries
   if (mwWM.mW[7].obt == 2)
   {
      int b = mwWM.mW[7].num;
      int type = item[b][0];



      if (type == 10) // pop message
      {

         int mx=0, my=0, mw=0, mh=0;
         get_int_3216(item[b][10], mx, my);
         get_int_3216(item[b][11], mw, mh);


         int x1=mx, y1=my;
         if ((mwWM.hx>x1-mst) && (mwWM.hx<x1+mst) && (mwWM.hy>y1-mst) && (mwWM.hy<y1+mst)) // upper left corner (move)
         {
            mouse_on_msg_ul = 1;
            mouse_move = 1;
         }

         int x2=mx+mw, y2=my+mh;
         if ((mwWM.hx>x2-mst) && (mwWM.hx<x2+mst) && (mwWM.hy>y2-mst) && (mwWM.hy<y2+mst)) // lower right corner (move)
         {
            mouse_on_msg_lr = 1;
            mouse_adj = 1;
         }



      }
      if ((type == 8) || (type == 11)) // bomb or rocket
      {
         float x0 = (float) item[mwWM.mW[7].num][4]+10; // get center of item location
         float y0 = (float) item[mwWM.mW[7].num][5]+10;
         float fx = (float) mwWM.hx;
         float fy = (float) mwWM.hy;
         float dst = sqrt(pow((x0-fx), 2) + pow((y0-fy), 2)); // distance from mouse to item
         float bdr = (float) item[mwWM.mW[7].num][7]; // bomb damage radius
         float dif = dst-bdr; // difference
         if ((dif < 3 ) && (dif > -3))  // mouse is on blast radius circle
         {
            mouse_on_bmb = 1;
            mouse_move = 1;
         }
      }
      if (type == 15) // sproingy
      {
         int x1 = item[mwWM.mW[7].num][4];
         int y1 = item[mwWM.mW[7].num][5];
         int y2 = y1 - al_fixtoi(get_sproingy_jump_height(mwWM.mW[7].num));

         if ((mwWM.hx>x1+msn) && (mwWM.hx<x1+msp) && (mwWM.hy>y2+msn) && (mwWM.hy<y2+msp))
         {
            mouse_on_sp = 1;
            mouse_move = 1;
         }
      }
      if ((type == 4) || (type == 9) || (type == 10) || (type == 14) || (type == 16) || (type == 17)) // key, switch, trigger, manip, damage, msg trigger
      {
         int x1 = item[mwWM.mW[7].num][6];
         int y1 = item[mwWM.mW[7].num][7];
         int x2 = x1 + item[mwWM.mW[7].num][8];
         int y2 = y1 + item[mwWM.mW[7].num][9];

         if ((mwWM.hx>x1-mst) && (mwWM.hx<x1+mst) && (mwWM.hy>y1-mst) && (mwWM.hy<y1+mst)) // upper left corner (move)
         {
            mouse_on_kbr_ul = 1;
            mouse_move = 1;
         }
         if ((mwWM.hx>x2-mst) && (mwWM.hx<x2+mst) && (mwWM.hy>y2-mst) && (mwWM.hy<y2+mst)) // lower right corner (resize)
         {
            mouse_on_kbr_lr = 1;
            mouse_adj = 1;
         }
      }
   }

   // -----------------------------------------------------------
   // --  detect if mouse pointer is on lift
   // -----------------------------------------------------------
   mouse_on_lift = 0;
   if (mwWM.obj_filter[4][1])
   {
      if ((!mI.key[MAP_LOCK_KEY][0]) && (!mwWM.mW[7].viewer_lock)) // no lock...check all lifts and steps
      {
         for (int x=0; x<num_lifts; x++)  // cycle lifts
            for (int y=0; y<lifts[x].num_steps; y++)  // cycle steps
               if ((lift_steps[x][y].type & 31) == 1) // look for move step
               {
                  int w =  lift_steps[x][y].w / 2;
                  int h =  lift_steps[x][y].h / 2;
                  int nx = lift_steps[x][y].x + w;
                  int ny = lift_steps[x][y].y + h;
                  if ((mwWM.hx > nx - w)  && (mwWM.hx < nx + w) && (mwWM.hy > ny - h)  && (mwWM.hy < ny + h)) // is mouse on this step ?
                  {
                     mouse_on_lift = 1;
                     mwWM.mW[7].obt = 4;
                     mwWM.mW[7].num = x;
                     lift = x;
                     step = y;
                     set_lift_to_step(lift, step);   // set current step in current lift
                  }
               }
      }

      if (((mI.key[MAP_LOCK_KEY][0]) || (mwWM.mW[7].viewer_lock)) && (mwWM.mW[7].obt == 4)) // locked, but locked to current lift
      {
         int x = lift; // check only current lift
         for (int y=0; y<lifts[x].num_steps; y++)  // cycle steps
            if ((lift_steps[x][y].type & 31) == 1) // look for move step
            {
               int w =  lift_steps[x][y].w / 2;
               int h =  lift_steps[x][y].h / 2;
               int nx = lift_steps[x][y].x + w;
               int ny = lift_steps[x][y].y + h;
               if ((mwWM.hx > nx - w)  && (mwWM.hx < nx + w) && (mwWM.hy > ny - h)  && (mwWM.hy < ny + h)) // is mouse on this step ?
               {
                  mouse_on_lift = 1;
                  step = y;
                  set_lift_to_step(lift, step);   // set current step in current lift
               }
            }
      }
   }
   if (mouse_on_lift)
   {
      mouse_on_obj = 1;
      if (mI.mouse_b[2][0]) step_popup_menu(lift, step);

      // is mouse on lower right adjustable corner
      int x2 = lift_steps[lift][step].x + lift_steps[lift][step].w;
      int y2 = lift_steps[lift][step].y + lift_steps[lift][step].h;
      if ((mwWM.hx > x2-8) && (mwWM.hy > y2-8)) mouse_adj = 1;
      else mouse_move = 1;
   }



   // -----------------------------------------------------------
   // --  set mouse cursor
   // -----------------------------------------------------------
   if (mouse_move) al_set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_MOVE);
   else if (mouse_adj) al_set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_RESIZE_SE);
   else al_set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);

   if (mI.mouse_b[1][0])
   {
      // get offset from where mouse was clicked to lift step upper left origin
      int lsox=0, lsoy=0;
      if (mouse_on_lift)
      {
         int l = mwWM.mW[7].num;
         int s = lifts[l].current_step;
         lsox = (mwWM.gx - lift_steps[l][s].x/20);
         lsoy = (mwWM.gy - lift_steps[l][s].y/20);
      }


      while (mI.mouse_b[1][0])
      {
         if (mouse_on_obj)
         {
            //printf("mouse pressed on obj\n");
            if (mwWM.mW[7].obt == 2) // move item
            {
               int n = mwWM.mW[7].num;
               int it = item[n][0]; // item_type

               // get offset of move
               int x_off = mwWM.gx - item[n][4] / 20;
               int y_off = mwWM.gy - item[n][5] / 20;

               item[n][4] = mwWM.gx*20;
               item[n][5] = mwWM.gy*20;
               itemf[n][0] = al_itofix(mwWM.gx*20);
               itemf[n][1] = al_itofix(mwWM.gy*20);

               if (mI.SHFT()) // move stuff also
               {
                   if ((it == 4) || (it == 9) || (it == 10) || (it == 14) || (it == 16) || (it == 17)) // key, trigger, msg, switch, manip, damage
                   {
                      item[n][6] += x_off*20;
                      item[n][7] += y_off*20;
                   }
                   if (it == 10) // msg
                   {
                      int x1=0, y1=0;
                      get_int_3216(item[n][10], x1, y1);
                      x1 += x_off*20;
                      y1 += y_off*20;
                      set_int_3216(item[n][10], x1, y1);
                   }
               }
            }
            if (mwWM.mW[7].obt == 3) // move enemy
            {
               // get offset of move
               int x_off = mwWM.gx - al_fixtoi(Efi[mwWM.mW[7].num][0]) / 20;
               int y_off = mwWM.gy - al_fixtoi(Efi[mwWM.mW[7].num][1]) / 20;

               // set new position
               Efi[mwWM.mW[7].num][0] = al_itofix(mwWM.gx*20);
               Efi[mwWM.mW[7].num][1] = al_itofix(mwWM.gy*20);

               if (mI.SHFT()) // move stuff also
               {
                  // move podzilla's trigger box too
                  if (Ei[mwWM.mW[7].num][0] == 7)
                  {
                     Ei[mwWM.mW[7].num][11] += x_off*20;
                     Ei[mwWM.mW[7].num][12] += y_off*20;
                  }

                  // move vinepod's stuff also
                  if (Ei[mwWM.mW[7].num][0] == 13)
                  {
                     // control point and extended pos
                     Ei[mwWM.mW[7].num][5] += x_off*20;
                     Ei[mwWM.mW[7].num][6] += y_off*20;
                     Ei[mwWM.mW[7].num][7] += x_off*20;
                     Ei[mwWM.mW[7].num][8] += y_off*20;
                     Ei[mwWM.mW[7].num][9] += x_off*20;
                     Ei[mwWM.mW[7].num][10] += y_off*20;

                     // trigger box
                     Ei[mwWM.mW[7].num][11] += x_off*20;
                     Ei[mwWM.mW[7].num][12] += y_off*20;
                  }

                  // move cloner's stuff too
                  if (Ei[mwWM.mW[7].num][0] == 9)
                  {
                     Ei[mwWM.mW[7].num][11] += x_off*20; // trigger box
                     Ei[mwWM.mW[7].num][12] += y_off*20;
                     Ei[mwWM.mW[7].num][15] += x_off*20; // source
                     Ei[mwWM.mW[7].num][16] += y_off*20;
                     Ei[mwWM.mW[7].num][17] += x_off*20; // dest
                     Ei[mwWM.mW[7].num][18] += y_off*20;
                  }
               }
            }
            if (mwWM.mW[7].obt == 4) // lift
            {
               if (mouse_move)
               {
                  lift_steps[lift][step].x = (mwWM.gx-lsox)*20;
                  lift_steps[lift][step].y = (mwWM.gy-lsoy)*20;
                  set_lift_to_step(lift, step);   // set current step in current lift
               }
               if (mouse_adj)
               {
                  // don't allow lr to be less than ul
                  if (mwWM.gx < lift_steps[lift][step].x/20+1) mwWM.gx = lift_steps[lift][step].x/20+1;
                  if (mwWM.gy < lift_steps[lift][step].y/20+1) mwWM.gy = lift_steps[lift][step].y/20+1;

                  // set new position
                  lift_steps[lift][step].w = mwWM.gx*20 - lift_steps[lift][step].x;
                  lift_steps[lift][step].h = mwWM.gy*20 - lift_steps[lift][step].y;
                  set_lift_to_step(lift, step);   // set current step in current lift
               }
            }
         }
         if (mouse_on_podx)
         {
            Efi[mwWM.mW[7].num][5] = al_itofix(mwWM.gx * 20);
            Efi[mwWM.mW[7].num][6] = al_itofix(mwWM.gy * 20);
            recalc_pod(mwWM.mW[7].num);
         }
         if (mouse_on_vpodx)
         {
            Ei[mwWM.mW[7].num][9]  = mwWM.gx * 20;
            Ei[mwWM.mW[7].num][10] = mwWM.gy * 20;
         }
         if (mouse_on_vpod1)
         {
            Ei[mwWM.mW[7].num][5] = mwWM.gx * 20;
            Ei[mwWM.mW[7].num][6] = mwWM.gy * 20;
         }
         if (mouse_on_vpod2)
         {
            Ei[mwWM.mW[7].num][7] = mwWM.gx * 20;
            Ei[mwWM.mW[7].num][8] = mwWM.gy * 20;
         }


         if (mouse_on_tb_ul) // move trigger box from ul
         {
            //printf("mouse pressed on tb_ul\n");
            Ei[mwWM.mW[7].num][11] = mwWM.gx*20;
            Ei[mwWM.mW[7].num][12] = mwWM.gy*20;
         }
         if (mouse_on_tb_lr)  // resize trigger box from lr
         {
            // prevent lr corner from being less than ul corner
            if (mwWM.gx < Ei[mwWM.mW[7].num][11]/20) mwWM.gx = Ei[mwWM.mW[7].num][11]/20;
            if (mwWM.gy < Ei[mwWM.mW[7].num][12]/20) mwWM.gy = Ei[mwWM.mW[7].num][12]/20;
            // set new postion
            Ei[mwWM.mW[7].num][13] = mwWM.gx*20 - Ei[mwWM.mW[7].num][11];
            Ei[mwWM.mW[7].num][14] = mwWM.gy*20 - Ei[mwWM.mW[7].num][12];
         }
         if (mouse_on_msg_ul) // move msg
         {
            set_int_3216(item[mwWM.mW[7].num][10], mwWM.hx, mwWM.hy);
         }

         if (mouse_on_msg_lr) // move msg
         {
            int mx=0, my=0, mw=0, mh=0;
            get_int_3216(item[mwWM.mW[7].num][10], mx, my);
            get_int_3216(item[mwWM.mW[7].num][11], mw, mh);

            // don't allow lr to be less than ul
            if (mwWM.hx < mx+8) mwWM.hx = mx+8;
            if (mwWM.hy < my+8) mwWM.hy = my+8;

            // set new size
            mw = mwWM.hx - mx;
            mh = mwWM.hy - my;

            set_int_3216(item[mwWM.mW[7].num][11], mw, mh);




         }

         if (mouse_on_sp) // adjust sproingy jump height
         {
            //al_hide_mouse_cursor(display);
            //item[mW[7].num][7] -= mouse_dy/2;
            int get_sp(float jh);
            float y0 = (float) item[mwWM.mW[7].num][5]+10;
            float fy = (float) mwWM.hy;
            item[mwWM.mW[7].num][7] = get_sp(y0-fy);

            // bounds check
            if (item[mwWM.mW[7].num][7] < 40) item[mwWM.mW[7].num][7] = 40;
            if (item[mwWM.mW[7].num][7] > 200) item[mwWM.mW[7].num][7] = 200;
         }
         if (mouse_on_bmb) // adjust bomb blast radius
         {
            float x0 = (float) item[mwWM.mW[7].num][4]+10; // get center of item location
            float y0 = (float) item[mwWM.mW[7].num][5]+10;
            float fx = (float) mwWM.hx;
            float fy = (float) mwWM.hy;
            float dist = sqrt(pow((x0-fx), 2) + pow((y0-fy), 2)); // distance from mouse to item
            item[mwWM.mW[7].num][7] = (int) dist;
         }
         if (mouse_on_trk) // adjust trakbot shot prox
         {
            float x0 = al_fixtof(Efi[mwWM.mW[7].num][0])+10; // get center of item location
            float y0 = al_fixtof(Efi[mwWM.mW[7].num][1])+10;
            float fx = (float) mwWM.hx;
            float fy = (float) mwWM.hy;
            float dst = sqrt(pow((x0-fx), 2) + pow((y0-fy), 2)); // distance from mouse
            Ei[mwWM.mW[7].num][17] = (int) dst;
         }
         // ranges for key, trigger, manip and damage
         if (mouse_on_kbr_ul) // move block range from ul
         {
            // set new position
            item[mwWM.mW[7].num][6] = mwWM.gx*20;
            item[mwWM.mW[7].num][7] = mwWM.gy*20;
         }
         if (mouse_on_kbr_lr) // adjust block range from lr
         {
            // don't allow lr to be less than ul
            if (mwWM.gx < item[mwWM.mW[7].num][6]/20) mwWM.gx = item[mwWM.mW[7].num][6]/20;
            if (mwWM.gy < item[mwWM.mW[7].num][7]/20) mwWM.gy = item[mwWM.mW[7].num][7]/20;

            // set new position
            item[mwWM.mW[7].num][8] = mwWM.gx*20 - item[mwWM.mW[7].num][6];
            item[mwWM.mW[7].num][9] = mwWM.gy*20 - item[mwWM.mW[7].num][7];
         }
         if (mouse_on_csb_ul) // move cloner source box from ul
         {
            Ei[mwWM.mW[7].num][15] = mwWM.gx*20; // set new postion
            Ei[mwWM.mW[7].num][16] = mwWM.gy*20;
         } // end of mouse csb_ul
         if (mouse_on_csb_lr) // resize box from lr
         {
            // get ul corner
            int x1 = Ei[mwWM.mW[7].num][15]/20;
            int y1 = Ei[mwWM.mW[7].num][16]/20;

            // prevent lr corner from being less than ul corner
            if (mwWM.gx < x1+1) mwWM.gx = x1+1;
            if (mwWM.gy < y1+1) mwWM.gy = y1+1;

            // set new sizes
            Ei[mwWM.mW[7].num][19] = (mwWM.gx-x1)*20;
            Ei[mwWM.mW[7].num][20] = (mwWM.gy-y1)*20;
         } // end of mouse csb_lr

         if (mouse_on_cdb_ul) // cloner destination ul
         {
            Ei[mwWM.mW[7].num][17] = mwWM.gx*20; // set new postion
            Ei[mwWM.mW[7].num][18] = mwWM.gy*20;
         }
         mwWM.redraw_level_editor_background();
      } // end of while mI.mouse_b[1][0] pressed
   } // end of if mI.mouse_b[1][0] pressed

   if (mI.mouse_b[2][0])
   {
      while (mI.mouse_b[2][0]) mwEQ.proc_event_queue();
      mwWM.set_windows(1);
   }
}


void ov_set_to_0(void)
{
   mwWM.mW[7].obt = 2;
   mwWM.mW[7].num = 0;
   if (item[mwWM.mW[7].num][0] == 0) mwWM.set_windows(1);
}

void ov_check_if_valid(int type)
{
   //printf("valid? obt:%d num:%d\n", mW[7].obt, mW[7].num);

   if (mwWM.mW[7].obt==0) ov_set_to_0();

   if (mwWM.mW[7].obt==2)
   {
      if (item_num_of_type[type] < 1) ov_set_to_0();
      if (item[mwWM.mW[7].num][0] != type) ov_set_to_0();
      if (item[mwWM.mW[7].num][0] == 0) ov_set_to_0();
   }
   if (mwWM.mW[7].obt==3)
   {
      if (e_num_of_type[type] < 1) ov_set_to_0();
      if (Ei[mwWM.mW[7].num][0] != type) ov_set_to_0();
   }

   if (mwWM.mW[7].obt==4)
   {
      //printf("num:%d num_lifts:%d\n", mW[7].num, num_lifts);
      if (mwWM.mW[7].num >= num_lifts) ov_set_to_0();
      if (num_lifts < 1) ov_set_to_0();
   }
}

void ov_process_keypress(void)
{
   int mb = mwWM.mW[7].mb;
   int type=0, lift=0, step=0;
   if (mwWM.mW[7].obt == 2) type = item[mwWM.mW[7].num][0];
   if (mwWM.mW[7].obt == 3) type = Ei[mwWM.mW[7].num][0];
   if (mwWM.mW[7].obt == 4)
   {
      lift = mwWM.mW[7].num;
      step = lifts[mwWM.mW[7].num].current_step;
      set_lift_to_step(lift, step);   // set current step in current lift
      lifts[mwWM.mW[7].num].current_step = step;
   }

   while (mI.key[ALLEGRO_KEY_DELETE][0]) { mwEQ.proc_event_queue(); mb = 20; }
   while (mI.key[ALLEGRO_KEY_RIGHT][0])  { mwEQ.proc_event_queue(); mb = 21; }
   while (mI.key[ALLEGRO_KEY_LEFT][0])   { mwEQ.proc_event_queue(); mb = 22; }

   switch(mb)
   {
      case 18: // move
      {
         if (mwWM.mW[7].obt==2)
         {
            sprintf(msg,"%s", item_name[type]);
            getxy(msg, mwWM.mW[7].obt, type, mwWM.mW[7].num);
         }
         if (mwWM.mW[7].obt==3)
         {
            sprintf(msg,"%s", enemy_name[type][0]);
            getxy(msg, mwWM.mW[7].obt, type, mwWM.mW[7].num);
         }
         if (mwWM.mW[7].obt==4)
         {
            while (!mI.key[ALLEGRO_KEY_ESCAPE][0])
            {
               if (mwWM.redraw_level_editor_background())
               {
                  for (int t=0; t<2; t++) move_lifts(1);  // move lifts for 2 frames
                  ov_draw_overlays(0);
                  get_new_screen_buffer(3, 0, 0);
                  mwWM.cycle_windows(1); // draw only
               }
            }
            while (mI.key[ALLEGRO_KEY_ESCAPE][0]) mwEQ.proc_event_queue(); // wait for release
            lift_setup(); // reset all lifts to step 0
         }
      }
      break;
      case 19:
         mwWM.mW[7].num = create_obj(mwWM.mW[7].obt, type, mwWM.mW[7].num);
      break;
      case 20: // delete
         if (mwWM.mW[7].obt== 2)
         {
            erase_item(mwWM.mW[7].num);
            sort_item(1);
            if (mwWM.mW[7].num >= item_first_num[type]+item_num_of_type[type]) mwWM.mW[7].num--;
            ov_check_if_valid(type);
         }
         if (mwWM.mW[7].obt == 3)
         {
            Ei[mwWM.mW[7].num][0] = 0;
            sort_enemy();
            if (mwWM.mW[7].num >= e_first_num[type]+e_num_of_type[type]) mwWM.mW[7].num--;
            ov_check_if_valid(type);
         }
         if (mwWM.mW[7].obt == 4)
         {
            erase_lift(mwWM.mW[7].num);
            if (--mwWM.mW[7].num < 0) mwWM.mW[7].num = 0;      // set to prev lift or zero
            ov_check_if_valid(0);
         }
      break;
      case 21: // next
         if ((mwWM.mW[7].obt==2) && (++mwWM.mW[7].num >= item_first_num[type] + item_num_of_type[type])) mwWM.mW[7].num--;
         if ((mwWM.mW[7].obt==3) && (++mwWM.mW[7].num >= e_first_num[type] + e_num_of_type[type])) mwWM.mW[7].num--;
         if ((mwWM.mW[7].obt==4) && (++mwWM.mW[7].num > num_lifts-1)) mwWM.mW[7].num--;
      break;
      case 22: // previous
         if ((mwWM.mW[7].obt==3) && (--mwWM.mW[7].num < e_first_num[type])) mwWM.mW[7].num++;
         if ((mwWM.mW[7].obt==2) && (--mwWM.mW[7].num < item_first_num[type])) mwWM.mW[7].num++;
         if ((mwWM.mW[7].obt==4) && (--mwWM.mW[7].num < 0)) mwWM.mW[7].num++;
      break;
   } // end of switch (mb)
}

void object_viewer(int obt, int num)
{
   mwWM.mW[7].obt = obt;
   mwWM.mW[7].num = num;
   mwWM.set_windows(4); // object viewer
//   al_set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);
}
