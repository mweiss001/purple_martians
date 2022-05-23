// e_object_viewer_windows.cpp
#include "pm.h"

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
            object_viewerw(3, e);
            num = e;
         }
      }
      if (type == 9)
      {
         int e = create_cloner();
         if ((e>=0) && (e<99) && (Ei[e][0] == 9))
         {
            object_viewerw(3, e);
            num = e;
         }
      }
   }
   if (obt == 4) create_lift();
   return num;  // return number of created obj or sent_num if bad create
}
void ovw_get_size(void)
{
   int obt = mW[7].obt;
   int num = mW[7].num;
   int type=0, w=300;
   if (obt == 2) type = item[num][0];
   if (obt == 3) type = Ei[num][0];

   if ((obt == 2) && (type == 1 )) w = 210; // door
   if ((obt == 2) && (type == 2 )) w = 200; // bonus
   if ((obt == 2) && (type == 3 )) w = 200; // exit
   if ((obt == 2) && (type == 4 )) w = 200; // key
   if ((obt == 2) && (type == 5 )) w = 200; // start
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
   if ((obt == 3) && (type == 6 )) w = 220; // cannon
   if ((obt == 3) && (type == 7 )) w = 220; // podzilla
   if ((obt == 3) && (type == 8 )) w = 220; // trakbot
   if ((obt == 3) && (type == 9 )) w = 220; // cloner
   if ((obt == 3) && (type == 11)) w = 220; // block walker
   if ((obt == 3) && (type == 12)) w = 220; // flapper

   if (obt == 4)                   w = 300; // lift

   mW[7].set_size(w, mW[7].h);
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


void ovw_title(int x1, int x2, int y1, int y2, int legend_highlight)
{
   int obt = mW[7].obt;
   int num = mW[7].num;
   int type=0;
   if (obt == 2) type = item[num][0];
   if (obt == 3) type = Ei[num][0];

//   al_draw_rectangle(x1-1, y1-1, x2+1, y2+1, palette_color[10], 1);  // outline entire window

   int xc = (x1+x2)/2;
   int yt = y1+14;

   // legend line text
   char lmsg[5][80];
   for (int x=0; x<5; x++) sprintf(lmsg[x],"%s","");

   // legend line colors
   int legend_color[5];

   // default number of legend lines and colors
   mW[7].num_legend_lines = 3;
   legend_color[0] = 7;   // legend color
   legend_color[1] = 13;  // location color
   legend_color[2] = 14;  // yellow
   legend_color[3] = 10;  // red
   legend_color[4] = 0;   // unused

   legend_highlight == 2 ? legend_color[2] = flash_color : legend_color[2] = 13;


   // title bar
   msg[0] = 0;
   if (obt == 2) sprintf(msg, "Item Viewer [%d]", num);
   if (obt == 3) sprintf(msg, "Enemy Viewer [%d]", num);
   if (obt == 4) sprintf(msg, "Lift Viewer [%d]", num);

   for (int x=0; x<15; x++)
      al_draw_line(x1, y1+x, x2, y1+x, palette_color[13+(x*16)], 1);
   al_draw_text(font, palette_color[15], xc, y1+2, ALLEGRO_ALIGN_CENTER,  msg);


   if (obt == 4)  // lifts
   {
      mW[7].num_legend_lines = 0;
      al_draw_rectangle(xc-94, yt, xc+94, yt+22, palette_color[15], 1);
      al_draw_textf(font, palette_color[13], xc, yt+8, ALLEGRO_ALIGN_CENTER, "Lift %d of %d",num+1, num_lifts);
   }
   if (obt == 3)  // enemies
   {
      al_draw_rectangle(xc-94, yt, xc+94, yt+22, palette_color[15], 1);
      draw_enemy(num, 1, xc-92, yt+1);
      sprintf(msg,"%s %d of %d", (const char *)enemy_name[type],1+num - e_first_num[type],e_num_of_type[type]);
      al_draw_text(font, palette_color[13], xc-69, yt+8, 0, msg);
      switch (type)
      {
         case 3: // archwagon
            sprintf(lmsg[1],"ArchWagon Location");
            sprintf(lmsg[2],"Bullet Proximity");
            legend_highlight == 2 ? legend_color[2] = flash_color : legend_color[2] = 14;
         break;
         case 4:
            mW[7].num_legend_lines = 2;
            sprintf(lmsg[1],"Bouncer Location");
         break;
         case 6: // cannon
            sprintf(lmsg[1],"Cannon Location");
            sprintf(lmsg[2],"Bullet Proximity");
            legend_highlight == 2 ? legend_color[2] = flash_color : legend_color[2] = 14;
         break;
         case 7: // podzilla
            mW[7].num_legend_lines = 4;
            sprintf(lmsg[1],"Podzilla Location");
            sprintf(lmsg[2],"Extended Postion");
            sprintf(lmsg[3],"Trigger Box");
            legend_highlight == 2 ? legend_color[2] = flash_color : legend_color[2] = 10;
            legend_highlight == 3 ? legend_color[3] = flash_color : legend_color[3] = 14;
         break;
         case 8: // trakbot
            sprintf(lmsg[1],"TrakBot Location");
            sprintf(lmsg[2],"Bullet Proximity");
            legend_highlight == 2 ? legend_color[2] = flash_color : legend_color[2] = 14;
         break;
         case 9: // cloner
            mW[7].num_legend_lines = 5;
            sprintf(lmsg[1],"Cloner Location");
            sprintf(lmsg[2],"Source Area");
            sprintf(lmsg[3],"Destination Area");
            sprintf(lmsg[4],"Trigger Box");
            legend_highlight == 2 ? legend_color[2] = flash_color : legend_color[2] = 11;
            legend_highlight == 3 ? legend_color[3] = flash_color : legend_color[3] = 10;
            legend_highlight == 4 ? legend_color[4] = flash_color : legend_color[4] = 14;
         break;
         case 11:
            mW[7].num_legend_lines = 2;
            sprintf(lmsg[1],"Block Walker Location");
         break;
         case 12: // flapper
            mW[7].num_legend_lines = 4;
            sprintf(lmsg[1],"Flapper Location");
            sprintf(lmsg[2],"Bullet Proximity");
            sprintf(lmsg[3],"Height Above Player");
            legend_highlight == 2 ? legend_color[2] = flash_color : legend_color[2] = 14;
            legend_highlight == 3 ? legend_color[3] = flash_color : legend_color[3] = 10;
         break;
      }
   }
   if (obt == 2)  // items
   {
      al_draw_rectangle(xc-94, yt, xc+94, yt+22, palette_color[15], 1);
      draw_item(num, 1, xc-94, yt+1);
      sprintf(msg,"%s %d of %d", item_name[type], 1+num - item_first_num[type],item_num_of_type[type]);
      al_draw_text(font, palette_color[13], xc-69, yt+8, 0, msg);
      switch (type)
      {
         case 1: // door
         {
            legend_highlight == 2 ? legend_color[2] = flash_color : legend_color[2] = 10;
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
             mW[7].num_legend_lines = 2;
             sprintf(lmsg[1],"Bonus Location");
         break;
         case 3:
             mW[7].num_legend_lines = 2;
             sprintf(lmsg[1],"Exit Location");
         break;
         case 4: // key
            sprintf(lmsg[1],"Key Location");
            sprintf(lmsg[2],"Block Range");
            legend_highlight == 2 ? legend_color[2] = flash_color : legend_color[2] = 10;
         break;
         case 5:
             mW[7].num_legend_lines = 2;
             sprintf(lmsg[1],"Start Location");
         break;
         case 7:
             mW[7].num_legend_lines = 2;
             sprintf(lmsg[1],"Mine Location");
         break;
         case 8:
            sprintf(lmsg[1],"Bomb Location");
            sprintf(lmsg[2],"Damage Range");
            legend_highlight == 2 ? legend_color[2] = flash_color : legend_color[2] = 14;
         break;
         case 9: // trigger
            sprintf(lmsg[1],"Trigger Item Location");
            sprintf(lmsg[2],"Trigger Field");
            legend_highlight == 2 ? legend_color[2] = flash_color : legend_color[2] = 10;
         break;
         case 10:
            sprintf(lmsg[1],"Message Location");
            sprintf(lmsg[2],"Display Position");
         break;
         case 11:
            sprintf(lmsg[1],"Rocket Location");
            sprintf(lmsg[2],"Damage Range");
            legend_highlight == 2 ? legend_color[2] = flash_color : legend_color[2] = 14;
         break;
         case 12:
             mW[7].num_legend_lines = 2;
             sprintf(lmsg[1],"Warp Location");
         break;
         case 14: // switch
            sprintf(lmsg[1],"Switch Location");
            sprintf(lmsg[2],"Block Range");
            legend_highlight == 2 ? legend_color[2] = flash_color : legend_color[2] = 10;
         break;
         case 15:
            sprintf(lmsg[1],"Sproingy Location");
            sprintf(lmsg[2],"Jump Height");
            legend_highlight == 2 ? legend_color[2] = flash_color : legend_color[2] = 14;
         break;
         case 16: // block manip
            sprintf(lmsg[1],"Block Manip Item Location");
            sprintf(lmsg[2],"Manip Field");
            legend_highlight == 2 ? legend_color[2] = flash_color : legend_color[2] = 12;
         break;
         case 17: // block damage
            sprintf(lmsg[1],"Item Location");
            sprintf(lmsg[2],"Damage Area");
            legend_highlight == 2 ? legend_color[2] = flash_color : legend_color[2] = 10;
         break;
      } // end of switch case
   }  // end of items

   al_draw_rectangle(x1, y1, x2, y2, palette_color[13], 1);  // outline entire window

   if (mW[7].num_legend_lines > 0)
   {
      al_draw_text(font, palette_color[legend_color[0]], xc, y2-36+ (4-mW[7].num_legend_lines)*8, ALLEGRO_ALIGN_CENTER, "Legend");
      al_draw_rectangle(xc-100, y2-38+ (4-mW[7].num_legend_lines)*8, xc+100, y2-1, palette_color[13], 1); // big frame
      al_draw_rectangle(xc-100, y2-38+ (4-mW[7].num_legend_lines)*8, xc+100, y2-28+ (4-mW[7].num_legend_lines)*8, palette_color[13], 1); // top frame
   }

   for (int x=1; x<mW[7].num_legend_lines; x++)// draw text lines
      al_draw_text(font, palette_color[legend_color[x]], xc, y2-26+(3-mW[7].num_legend_lines+x)*8, ALLEGRO_ALIGN_CENTER, lmsg[x]);
}
void ovw_draw_buttons(int x1, int y1, int x2, int y2, int have_focus, int moving)
{

   int obt = mW[7].obt;
   int num = mW[7].num;

   int d = 1;
   if (have_focus) d = 0;
   if (moving) d = 1;

   mW[7].mb = 0;

   int type=0;
   if (obt == 2) type = item[num][0];
   if (obt == 3) type = Ei[num][0];
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

   int bts=16, dim=128;


   // --------------------------------------------------
   // --- Common ---------------------------------------
   // --------------------------------------------------
   int lc = 6; // lock_color;
   if (mW[7].viewer_lock) lc = 7;

   if (mdw_buttont(xa,  ya, x27-1, bts,                0,0,0,0,  0, 9,15,0, 1,0,0,d, "Prev")) mW[7].mb = 22;
       mdw_toggle( x27, ya, x57-1, bts,                0,0,0,0,  0,lc,15,0, 1,0,0,d, mW[7].viewer_lock, "Unlocked", "Locked", 15, 15, 6, 7);
   if (mdw_buttont(x57, ya, xb,    bts,                1,0,0,0,  0, 9,15,0, 1,0,0,d, "Next")) mW[7].mb = 21;

   if (obt == 4) { if (mdw_buttont(xa, ya, x13-1, bts, 0,0,0,0,  0,13,15,0, 1,0,0,d, "Run Lifts")) mW[7].mb = 18; }
   else          { if (mdw_buttont(xa, ya, x13-1, bts, 0,0,0,0,  0,13,15,0, 1,0,0,d, "Move"))      mW[7].mb = 18; }

   if (mdw_buttont(x13, ya, x23-1, bts,                0,0,0,0,  0,14,15,0, 1,0,0,d, "Create")) mW[7].mb = 19;
   if (mdw_buttont(x23, ya, xb,    bts,                1,0,0,0,  0,10,15,0, 1,0,0,d, "Delete")) mW[7].mb = 20;

   if (mdw_buttont(xa,  ya, x14-1, bts,                0,0,0,0,  0,1,15,0,  1,0,0,d, "Help")) mW[7].mb = 24; // viewer help
   if (mdw_button( x14, ya, xb,    bts, 57, num, type, obt,      0,1,15,0,  1,0,1,d)) mW[7].mb = 25; // specific object help

   ya+=4; // space after common buttons


   // --------------------------------------------------
   // --- Lifts ----------------------------------------
   // --------------------------------------------------
   if (obt == 4) // lifts
   {
      int lift = num;
      int step = lifts[num].current_step;

      mdw_buttonp(xa, ya, xb, bts, 500,0,0,0,   0,13,15,0,  1,0,1,d, lifts[num].mode); // MODE


      if (lifts[lift].mode == 1) { mdw_slider(xa, ya, xb, ya+bts-2, 555, lift,0,0, 0,13,15,15,  1,0,0,d); ya+=bts; }     // lift mode 1 player ride timer
      if (mdw_button(                         xa, ya, xb, bts,      504, lift,0,0, 0, 4,15, 0,  1,0,1,d)) mW[7].mb = 26; // lift name
      ya+=bts;

      // list of step buttons
      // --------------------------------------------------------------------------------
      int ysb = ya; // y pos of step buttons

      // process mouse on list of step buttons
      int step_pointer = -99;
      if ((mouse_x > xa + 10) && (mouse_x < xb - 10)) // is mouse on step buttons?
      {
         int step0_y_offset = ysb + 14 + bts;
         int mouse_step = (mouse_y - step0_y_offset) / bts;               // calculate step that mouse is on
         if ((mouse_step >= 0) && (mouse_step < lifts[lift].num_steps))   // is this a valid step?
         {
            step_pointer = mouse_step;  // set step pointer to this step
            if (mouse_b1)
            {
               step = mouse_step; // set current step to this step
               lifts[lift].current_step = step; // set current step in lift
            }
            if (mouse_b2) step_popup_menu(lift, step_pointer); // step pop-up menu for this step
         }
      }

      // draw the list of steps  - this has to go after, because it can eat the mouse clicks needed for previous section
      int ycs = ysb + draw_steps(xa, xb, ysb, lift, step, step_pointer);
      ycs +=bts;

      // draw buttons for the current step button and get y postion for next item (lift)
      int yld = ycs + draw_current_step_buttons(xa, xb, ycs, lift, step);


      mW[7].h = yld - mW[7].y1-1; // global variable for height of ovw when variable due to lift
      mW[7].set_size(mW[7].w, mW[7].h);

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

      if (mW[7].mb == 26)
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
      int type = Ei[num][0];
      switch (type) // enemy subtypes
      {
         case 3:     // archwag
            mdw_slider2_fix(xa, ya, xb, bts,      0,0,0,0, 0,12, 15, 15, 1,0,1,d, Efi[num][6], 9, 0.7, 0.01, "X-Speed:");
            mdw_slider2_fix(xa, ya, xb, bts,      0,0,0,0, 0,12, 15, 15, 1,0,1,d, Efi[num][3], 9, 0.7, 0.01, "Y-Speed:");
            ya+=bts;
            mdw_toggle(     xa, ya, xb, bts,      1,0,0,0, 0, 0,  0,  0, 1,0,0,d, Ei[num][2], "Initial Direction:Left ", "Initial Direction:Right", 13, 13, 15, 15);
            mdw_toggle(     xa, ya, xb, bts,      1,0,0,0, 0, 0,  0,  0, 1,0,0,d, Ei[num][8], "Follow Mode",             "Bounce Mode",             13, 13, 15, 15);
            mdw_slider0_int(xa, ya, xb, bts,      0,0,0,0, 0, 7, 15, 15, 1,0,1,d, Ei[num][11], 100, 0, 1,    "Jump Before Hole:", "Off");
            mdw_slider0_int(xa, ya, xb, bts,      0,0,0,0, 0, 7, 15, 15, 1,0,1,d, Ei[num][12], 100, 0, 1,    "Jump Before Wall:", "Off");
            mdw_slider0_int(xa, ya, xb, bts,      0,0,0,0, 0, 7, 15, 15, 1,0,1,d, Ei[num][7],  600, 0, 1,    "Jump Under Width:", "Off");
            mdw_slider0_int(xa, ya, xb, bts,      0,0,0,0, 0, 7, 15, 15, 1,0,1,d, Ei[num][6],  500, 0, 1,    "Jump Timer Wait:",  "Off");
            ya+=bts;
            mdw_slider2_fix(xa, ya, xb, bts,      0,0,0,0, 0, 9, 15, 15, 1,0,1,d, Efi[num][7], 20, 0.8, 0.1, "Bullet Speed:");
            mdw_slider2_int(xa, ya, xb, bts,      0,0,0,0, 0, 9, 15, 15, 1,0,1,d, Ei[num][17], 2000, 20, 1,  "Bullet Proximity:");
            mdw_slider2_int(xa, ya, xb, bts,      0,0,0,0, 0, 9, 15, 15, 1,0,1,d, Ei[num][15], 200, 1, 1,    "Bullet Retrigger Time:");
            ya+=bts;
            mdw_slider2_int(xa, ya, xb, bts,      0,0,0,0, 0, 4, 15, 15, 1,0,1,d, Ei[num][29], 20, 0, 1,     "Collision Box:");
            mdw_slider2_fix(xa, ya, xb, bts,      0,0,0,0, 0, 4, 15, 15, 1,0,1,d, Efi[num][4], 10, 0, 0.1,   "Health Decrement:");
            mdw_slider2_int(xa, ya, xb, bts,      0,0,0,0, 0, 4, 15, 15, 1,0,1,d, Ei[num][25], 50, 0, 1,     "Health Bonus:");
         break;
         case 4: // bouncer
            if (mdw_buttont(xa, ya, xb, bts,      1,0,0,0,            0, 12, 15,  0, 1,0,0,d, "Set Initial Direction"))  getxy("Initial Direction", 96, 4, num);
            mdw_slider(     xa, ya, xb, ya+bts-2, 22, num, type, obt, 0, 12, 15, 15, 1,0,0,d); ya+=bts;  // speed
            mdw_slider2_int(xa, ya, xb, bts,      0,0,0,0,            0, 12, 15, 15, 1,0,1,d, Ei[num][8], 100, 0, 1,   "Seek Count:"); ya+=bts;
            mdw_button(     xa, ya, xb, bts,      13, num, type, obt, 0,  8,  9,  0, 1,0,1,d); ya+=bts; // main shape
            mdw_button(     xa, ya, xb, bts,      14, num, type, obt, 0,  8,  9,  0, 1,0,1,d); ya+=bts; // seek shape
            mdw_slider2_int(xa, ya, xb, bts,      0,0,0,0,            0,  4, 15, 15, 1,0,1,d, Ei[num][29], 20, 0, 1,    "Collision Box:");
            mdw_slider2_fix(xa, ya, xb, bts,      0,0,0,0,            0,  4, 15, 15, 1,0,1,d, Efi[num][4], 10, 0, 0.1,  "Health Decrement:");
            mdw_slider2_int(xa, ya, xb, bts,      0,0,0,0,            0,  4, 15, 15, 1,0,1,d, Ei[num][25], 50, 0, 1,    "Health Bonus:");
         break;
         case 6: // cannon
            if (mdw_buttont(xa, ya, xb, bts,      1,0,0,0,       0, 12, 15,  0, 1,0,0,d, "Set Initial Direction"))  getxy("Initial Direction", 96, 4, num);
            mdw_slider(xa, ya, xb, ya+bts-2, 22, num, type, obt, 0, 12, 15, 15, 1,0,0,d); ya+=bts;  // speed
            mdw_slider2_int(xa, ya, xb, bts,      0,0,0,0,       0, 12, 15, 15, 1,0,1,d, Ei[num][8], 100, 0, 1,     "Seek Count:");
            ya+=bts;
            mdw_slider2_int(xa, ya, xb, bts,      0,0,0,0,       0, 8,  15, 15, 1,0,1,d, Ei[num][9], 40, 0, 1,      "Extra Hits To Kill:");
            ya+=bts;
            mdw_slider2_fix(xa, ya, xb, bts,      0,0,0,0,       0, 9,  15, 15, 1,0,1,d, Efi[num][7], 20, 0.8, 0.1, "Bullet Speed:");
            mdw_slider2_int(xa, ya, xb, bts,      0,0,0,0,       0, 9,  15, 15, 1,0,1,d, Ei[num][17], 2000, 20, 1,  "Bullet Proximity:");
            mdw_slider2_int(xa, ya, xb, bts,      0,0,0,0,       0, 9,  15, 15, 1,0,1,d, Ei[num][15], 200, 1, 1,    "Bullet Retrigger Time:");
            ya+=bts;
            mdw_slider2_int(xa, ya, xb, bts,      0,0,0,0,       0, 4,  15, 15, 1,0,1,d, Ei[num][29], 20, 0, 1,     "Collision Box:");
            mdw_slider2_fix(xa, ya, xb, bts,      0,0,0,0,       0, 4,  15, 15, 1,0,1,d, Efi[num][4], 10, 0, 0.1,   "Health Decrement:");
            mdw_slider2_int(xa, ya, xb, bts,      0,0,0,0,       0, 4,  15, 15, 1,0,1,d, Ei[num][25], 50, 0, 1,     "Health Bonus:");
         break;
         case 7: // podzilla
            mdw_slider(xa, ya, xb, ya+bts-2, 29, num, type, obt, 0, 12, 15, 15, 1,0,0,d); ya+=bts;  // speed
            mdw_slider2_int(xa, ya, xb, bts,      0,0,0,0,       0, 12,  15, 15, 1,0,1,d, Ei[num][9], 40, 0, 1,     "Pause:");
            mdw_slider2_fix(xa, ya, xb, bts,      0,0,0,0,       0,  9, 15, 15, 1,0,1,d, Efi[num][7], 20, 0.8, 0.1, "Bullet Speed:");
            if (mdw_buttont(xa, ya, xb, bts,      1,0,0,0,       0, 10, 15,  0, 1,0,0,d, "Move Extended Position")) getxy("Pod Extended Position", 99, 7, num);
            if (mdw_buttont(xa, ya, xb, bts,      1,0,0,0,       0, 14, 15,  0, 1,0,0,d, "Set Trigger Box")) get_block_range("Trigger Box", &Ei[num][11], &Ei[num][12], &Ei[num][13], &Ei[num][14], 2);
            mdw_slider2_int(xa, ya, xb, bts,      0,0,0,0,       0,  4, 15, 15, 1,0,1,d, Ei[num][29], 20, 0, 1,     "Collision Box:");
            mdw_slider2_fix(xa, ya, xb, bts,      0,0,0,0,       0,  4, 15, 15, 1,0,1,d, Efi[num][4], 10, 0, 0.1,   "Health Decrement:");
            mdw_slider2_int(xa, ya, xb, bts,      0,0,0,0,       0,  4, 15, 15, 1,0,1,d, Ei[num][25], 50, 0, 1,     "Health Bonus:");
         break;
         case 8: // trakbot
            mdw_button(     xa, ya, xb, bts, 11, num, type, obt, 0,12, 15,  0,  1,0,1,d);  // initial direction
            mdw_slider2_fix(xa, ya, xb, bts,      0,0,0,0,       0,12, 15, 15,  1,0,1,d, Efi[num][2], 10, .5, 0.1, "X-Speed:");
            mdw_slider2_fix(xa, ya, xb, bts,      0,0,0,0,       0,12, 15, 15,  1,0,1,d, Efi[num][3], 10, .5, 0.1, "Y-Speed:");
            ya+=bts;
            mdw_toggle(     xa, ya, xb, bts,      1,0,0,0,       0, 0,  0,  0,  1,0,0,d, Ei[num][7], "Drop Mode:Off", "Drop Mode:On ", 15, 15, 11, 11); ya+=bts;
            mdw_slider2_fix(xa, ya, xb, bts,      0,0,0,0,       0, 9, 15, 15,  1,0,1,d, Efi[num][7], 20, 0.8, 0.1, "Bullet Speed:");
            mdw_slider2_int(xa, ya, xb, bts,      0,0,0,0,       0, 9, 15, 15,  1,0,1,d, Ei[num][17], 2000, 20, 1,  "Bullet Proximity:");
            mdw_slider2_int(xa, ya, xb, bts,      0,0,0,0,       0, 9, 15, 15,  1,0,1,d, Ei[num][15], 200, 1, 1,    "Bullet Retrigger Time:");
            ya+=bts;
            mdw_slider2_int(xa, ya, xb, bts,      0,0,0,0,       0, 4, 15, 15,  1,0,1,d, Ei[num][29], 20, 0, 1,     "Collision Box:");
            mdw_slider2_fix(xa, ya, xb, bts,      0,0,0,0,       0, 4, 15, 15,  1,0,1,d, Efi[num][4], 10, 0, 0.1,   "Health Decrement:");
            mdw_slider2_int(xa, ya, xb, bts,      0,0,0,0,       0, 4, 15, 15,  1,0,1,d, Ei[num][25], 50, 0, 1,     "Health Bonus:");
         break;
         case 9: // cloner
            mdw_toggle(     xa, ya, xb, bts,      1,0,0,0,          0, 0, 0, 0, 1,0,0,d, Ei[num][30], "Not Invincible", "Invincible", 15, 15, 12, 12);
            mdw_buttonp(    xa, ya, xb, bts,      27,0,0,0,         0,12,15, 0, 1,0,1,d, Ei[num][8]); // trigger type
            mdw_slider2_int(xa, ya, xb, bts,      0,0,0,0,          0,12,15,15, 1,0,1,d, Ei[num][6], 1000, 20, 1, "Delay Timer:");
            ya+=bts;
            mdw_slider0_int(xa, ya, xb, bts,      0,0,0,0,          0,14,15,15, 1,0,1,d, Ei[num][9], 4800, 0, 1,  "Created Time To Live:", "-");
            mdw_slider0_int(xa, ya, xb, bts,      0,0,0,0,          0,14,15,15, 1,0,1,d, Ei[num][10], 600, 0, 1,  "Max Created Objects:", "-");
            ya+=bts;
            if (mdw_buttont(xa, ya, xb, bts,      1,0,0,0,          0,11,15,0,  1,0,0,d, "Set Source Area")) get_block_range("Cloner Source Area", &Ei[num][15], &Ei[num][16], &Ei[num][19], &Ei[num][20], 1);
            if (mdw_buttont(xa, ya, xb, bts,      1,0,0,0,          0,10,15,0,  1,0,0,d, "Set Destination")) getxy("Cloner Destination", 98, 9, num);
            if (mdw_buttont(xa, ya, xb, bts,      1,0,0,0,          0,14,15,0,  1,0,0,d, "Set Trigger Box")) get_block_range("Trigger Box", &Ei[num][11], &Ei[num][12], &Ei[num][13], &Ei[num][14], 2);
            mdw_buttonp(    xa, ya, xb, bts,      81,0,0,0,         0,15,15,0,  1,0,1,d, Ei[num][4]); // show boxes
            ya+=bts;
            mdw_slider2_int(xa, ya, xb, bts,      0,0,0,0,          0,4,15,15,  1,0,1,d, Ei[num][29], 20, 0, 1,   "Collision Box:");
            mdw_slider2_fix(xa, ya, xb, bts,      0,0,0,0,          0,4,15,15,  1,0,1,d, Efi[num][4], 10, 0, 0.1, "Health Decrement:");
            mdw_slider2_int(xa, ya, xb, bts,      0,0,0,0,          0,4,15,15,  1,0,1,d, Ei[num][25], 50, 0, 1,   "Health Bonus:");
         break;
         case 11: // block walker
            mdw_slider2_fix(xa, ya, xb, bts,      0,0,0,0,  0, 12, 15, 15,  1,0,1,d, Efi[num][6], 9, 0.7, 0.01, "X-Speed:");
            mdw_slider2_fix(xa, ya, xb, bts,      0,0,0,0,  0, 12, 15, 15,  1,0,1,d, Efi[num][3], 9, 0.7, 0.01, "Y-Speed:");
            ya+=bts;
            mdw_toggle(     xa, ya, xb, bts,      1,0,0,0,  0,  0,  0,  0,  1,0,0,d, Ei[num][2], "Initial Direction:Left ", "Initial Direction:Right", 13, 13, 15, 15);
            mdw_toggle(     xa, ya, xb, bts,      1,0,0,0,  0,  0,  0,  0,  1,0,0,d, Ei[num][8], "Follow Mode",             "Bounce Mode",             13, 13, 15, 15);
            ya+=bts;
            mdw_slider0_int(xa, ya, xb, bts,      0,0,0,0,  0,  7, 15, 15,  1,0,1,d, Ei[num][11], 100, 0, 1,    "Jump Before Hole:", "Off");
            mdw_slider0_int(xa, ya, xb, bts,      0,0,0,0,  0,  7, 15, 15,  1,0,1,d, Ei[num][12], 100, 0, 1,    "Jump Before Wall:", "Off");
            mdw_slider0_int(xa, ya, xb, bts,      0,0,0,0,  0,  7, 15, 15,  1,0,1,d, Ei[num][7],  600, 0, 1,    "Jump Under Width:", "Off");
            mdw_slider0_int(xa, ya, xb, bts,      0,0,0,0,  0,  7, 15, 15,  1,0,1,d, Ei[num][6],  500, 0, 1,    "Jump Timer Wait:",  "Off");
            ya+=bts;
            mdw_slider2_int(xa, ya, xb, bts,      0,0,0,0,  0,  4, 15, 15,  1,0,1,d, Ei[num][29], 20, 0, 1,     "Collision Box:");
            mdw_slider2_fix(xa, ya, xb, bts,      0,0,0,0,  0,  4, 15, 15,  1,0,1,d, Efi[num][4], 10, 0, 0.1,   "Health Decrement:");
            mdw_slider2_int(xa, ya, xb, bts,      0,0,0,0,  0,  4, 15, 15,  1,0,1,d, Ei[num][25], 50, 0, 1,     "Health Bonus:");
         break;
         case 12: // flapper
            mdw_slider2_fix(xa, ya, xb, bts,      0,0,0,0,  0, 12, 15, 15, 1,0,1,d, Efi[num][5], 8, .5, 0.1,  "X-Speed:");
            mdw_slider2_fix(xa, ya, xb, bts,      0,0,0,0,  0, 12, 15, 15, 1,0,1,d, Efi[num][6], 2, 0.1, 0.1, "X-Accel:");
            mdw_toggle(     xa, ya, xb, bts,      1,0,0,0,  0,  0,  0,  0, 1,0,0,d, Ei[num][2], "Initial Direction:Left ", "Initial Direction:Right", 15, 15, 12, 12);
            ya+=bts;
            mdw_slider2_fix(xa, ya, xb, bts,      0,0,0,0,  0, 13, 15, 15, 1,0,1,d, Efi[num][3], 5, 0, 0.01,      "Y-Speed:");
            mdw_slider2_fix(xa, ya, xb, bts,      0,0,0,0,  0, 13, 15, 15, 1,0,1,d, Efi[num][10], 8, .5, 0.1,     "Flap Speed:");
            mdw_slider2_int(xa, ya, xb, bts,      0,0,0,0,  0, 13, 15, 15, 1,0,1,d, Ei[num][21], 400, 0, 10,      "Flap Height:");
            mdw_slider2_int(xa, ya, xb, bts,      0,0,0,0,  0, 13, 15, 15, 1,0,1,d, Ei[num][20], 1000, -1000, 10, "Height Above Player:");
            ya+=bts;
            mdw_slider2_fix(xa, ya, xb, bts,      0,0,0,0,  0,  9, 15, 15, 1,0,1,d, Efi[num][7], 20, 0.8, 0.1, "Bullet Speed:");
            mdw_slider2_int(xa, ya, xb, bts,      0,0,0,0,  0,  9, 15, 15, 1,0,1,d, Ei[num][17], 2000, 20, 1,  "Bullet Proximity:");
            mdw_slider2_int(xa, ya, xb, bts,      0,0,0,0,  0,  9, 15, 15, 1,0,1,d, Ei[num][15], 200, 1, 1,    "Bullet Retrigger Time:");
            ya+=bts;
            mdw_slider2_int(xa, ya, xb, bts,      0,0,0,0,  0,  4, 15, 15, 1,0,1,d, Ei[num][29], 20, 0, 1,     "Collision Box:");
            mdw_slider2_fix(xa, ya, xb, bts,      0,0,0,0,  0,  4, 15, 15, 1,0,1,d, Efi[num][4], 10, 0, 0.1,   "Health Decrement:");
            mdw_slider2_int(xa, ya, xb, bts,      0,0,0,0,  0,  4, 15, 15, 1,0,1,d, Ei[num][25], 50, 0, 1,     "Health Bonus:");
         break;
      }
   }

   // --------------------------------------------------
   // --- Items ----------------------------------------
   // --------------------------------------------------
   if (obt == 2) // items
   {
      int type = item[num][0];
      switch (type) // item subtypes
      {
         case 1: // door
          {
            int abc = 11; // regular button frame color
            int atc = 15; // regular button text color

            if (item[num][8] == 0) // if exit only change color of inactive buttons
            {
                abc = 15;
                atc = 127;
            }
            mdw_buttonp(xa, ya, xb, bts, 22,0,0,0,           0,  15,  13, 0, 1,0,1,d, item[num][3]); // stat | fall | carry
            mdw_button( xa, ya, xb, bts, 49, num, type, obt, 0,  12,  15, 0, 1,0,1,d); // door type
            mdw_button( xa, ya, xb, bts, 4,  num, type, obt, 0, abc, atc, 0, 1,0,1,d); // set linked item
            mdw_button( xa, ya, xb, bts, 50, num, type, obt, 0, abc, atc, 0, 1,0,1,d); // enter mode (up | down)
            mdw_button( xa, ya, xb, bts, 53, num, type, obt, 0, abc, atc, 0, 1,0,1,d); // move type
            mdw_button( xa, ya, xb, bts, 51, num, type, obt, 0, abc, atc, 0, 1,0,1,d); // exit link show
            mdw_button( xa, ya, xb, bts, 52, num, type, obt, 0,  13,  15, 0, 1,0,1,d); // get new shape
            mdw_colsel( xa, ya, xb, bts,  5, num, type, obt, 0,   0,   0, 0, 0,0,1,d); // change color
         }
         break;
         case 2: // bonus
            if (item[num][6] == 1) // health bonus
            {
               mdw_buttonp(    xa, ya, xb, bts, 22,0,0,0,   0,15,13,0,  1,0,1,d, item[num][3]); // stat | fall | carry
               mdw_buttonp(    xa, ya, xb, bts, 101,0,0,0,  0,11,15,0,  1,0,1,d, item[num][6]); // bonus type
               mdw_slider2_int(xa, ya, xb, bts, 0,0,0,0,    0,11,15,15, 1,0,1,d, item[num][7], 100, 2, 1, "Health Bonus:");
               item[num][1] = 1035;
               item[num][2] = 1;
               if (item[num][7] == 10) item[num][1] = 1023;
               if (item[num][7] == 50) item[num][1] = 1027;
            }
            else if (item[num][6] == 2) // free man
            {
               mdw_buttonp(xa, ya, xb, bts, 21, 0,0,0,  0,15,13,0,  1,0,1,d, item[num][3]); // stat | fall
               mdw_buttonp(xa, ya, xb, bts, 101,0,0,0,  0,10,15,0,  1,0,1,d, item[num][6]); // bonus type
               item[num][1] = 1038;
               item[num][2] = 1;
            }
            else if (item[num][6] == 3) // purple coin
            {
               mdw_buttonp(xa, ya, xb, bts, 21, 0,0,0,  0,15,13,0, 1,0,1,d, item[num][3]); // stat | fall
               mdw_buttonp(xa, ya, xb, bts, 101,0,0,0,  0, 8,15,0, 1,0,1,d, item[num][6]); // bonus type
               item[num][1] = 197;
               item[num][2] = 0;
            }
         break;
         case 3: // exit
            mdw_buttonp(    xa, ya, xb, bts, 22,0,0,0,  0, 15, 13, 14, 1,0,1,d, item[num][3]); // stat | fall | carry
            mdw_slider2_int(xa, ya, xb, bts, 0,0,0,0,   0, 12, 15, 15, 1,0,1,d, item[num][8], 100, 0, 1,  "Enemy Count Lock:");
         break;
         case 4: // key
            mdw_buttonp(    xa, ya, xb, bts, 21,0,0,0,  0,15,13,14, 1,0,1,d, item[num][3]); // stat | fall
            mdw_buttonp(    xa, ya, xb, bts, 102,0,0,0, 0, 8,15, 0, 1,0,1,d, item[num][1]); // color
            if (mdw_buttont(xa, ya, xb, bts, 1,0,0,0,   0,10,15, 0, 1,0,0,d, "Get New Block Range")) get_block_range("Block Range", &item[num][6], &item[num][7], &item[num][8], &item[num][9], 1);
            mdw_toggle(     xa, ya, xb, bts, 1,0,0,0,   0, 0, 0, 0, 1,0,0,d, item[num][12], "Remove All Blocks", "Remove Only Matching", 15, 15, 4, 4);
         break;
         case 5: // start
            mdw_buttonp(xa, ya, xb, bts, 22,0,0,0, 0,15,13,0, 1,0,1,d, item[num][3]); // stat | fall | carry
            mdw_buttonp(xa, ya, xb, bts, 78,0,0,0, 0,11,15,0, 1,0,1,d, item[num][6]); // start mode
            mdw_buttonp(xa, ya, xb, bts, 79,0,0,0, 0,11,15,0, 1,0,1,d, item[num][7]); // start index
         break;
         case 7: // mine
            mdw_buttonp(xa, ya, xb, bts, 22,0,0,0,  0, 15,  13, 14, 1,0,1,d, item[num][3]); // stat | fall | carry
            mdw_slider2_int(xa, ya, xb, bts, 0,0,0,0,  0, 12, 15, 15, 1,0,1,d, item[num][8], 20, 1, 1,  "Mine Damage:");
         break;
         case 8: // bomb
            mdw_buttonp(xa, ya, xb, bts, 22,0,0,0,               0,15,13, 0, 1,0,1,d, item[num][3]); // stat | fall | carry
            mdw_toggle( xa, ya, xb, bts, 1,0,0,0,                0, 0, 0, 0, 1,0,0,d, item[num][11], "Sticky:Off", "Sticky:On", 15, 15, 4, 4);
            mdw_button( xa, ya, xb, bts, 77, num, type, obt,     0,12,15, 0, 1,0,1,d);          // fuse timer / remote detonator
            mdw_slider2_int(xa, ya, xb, bts, 0,0,0,0,   0,14,15,15, 1,0,1,d, item[num][7], 1200, 20, 1,  "Damage Range:");
            mdw_slider2_int(xa, ya, xb, bts, 0,0,0,0,   0,10,15,15, 1,0,1,d, item[num][9], 2000, 1,  1,  "Fuse Length:");
         break;
         case 9: // trigger
         {
            if (mdw_buttont(xa, ya, xb, bts, 1,0,0,0,  0, 14, 14, 14, 1,0,0,d, "Get New Trigger Field")) get_block_range("Trigger Rectangle", &item[num][6], &item[num][7], &item[num][8], &item[num][9], 1);

            ya+=bts; // spacing between groups

            // draw trigger field on/off with optional color select if on
            if (mdw_togglf(xa, ya, xb, bts, 1,0,0,0,  0,0,0,0,  1,0,0,d, item[num][3], PM_ITEM_TRIGGER_DRAW_ON, "Draw Trigger Field:OFF","Draw Trigger Field:ON", 15+dim, 15, 15+dim, item[num][2]))
               mdw_colsel(xa, ya, xb, bts, 6, num, type, obt,  0,0,0,0,  0,0,1,d); // color select

            ya+=bts; // spacing

            mdw_togglf(xa, ya, xb, bts, 1,0,0,0,  0,0,0,0,  1,0,0,d, item[num][3], PM_ITEM_TRIGGER_PLAYER, "Triggered by Players:OFF         ","Triggered by Players:ON          ", 15+dim, 15, 14+dim, 14);
            mdw_togglf(xa, ya, xb, bts, 1,0,0,0,  0,0,0,0,  1,0,0,d, item[num][3], PM_ITEM_TRIGGER_ENEMY,  "Triggered by Enemies:OFF         ","Triggered by Enemies:ON          ", 15+dim, 15, 14+dim, 14);
            mdw_togglf(xa, ya, xb, bts, 1,0,0,0,  0,0,0,0,  1,0,0,d, item[num][3], PM_ITEM_TRIGGER_ITEM,   "Triggered by Items:OFF           ","Triggered by Items:ON            ", 15+dim, 15, 14+dim, 14);
            mdw_togglf(xa, ya, xb, bts, 1,0,0,0,  0,0,0,0,  1,0,0,d, item[num][3], PM_ITEM_TRIGGER_PBUL,   "Triggered by Player's Bullets:OFF","Triggered by Player's Bullets:ON ", 15+dim, 15, 14+dim, 14);
            mdw_togglf(xa, ya, xb, bts, 1,0,0,0,  0,0,0,0,  1,0,0,d, item[num][3], PM_ITEM_TRIGGER_EBUL,   "Triggered by Enemy's Bullets:OFF ","Triggered by Enemy's Bullets:ON  ", 15+dim, 15, 14+dim, 14);

            ya+=bts; // spacing

            if (mdw_togglf(xa, ya, xb, bts, 1,0,0,0,  0,0,0,0,  1,0,0,d, item[num][3], PM_ITEM_TRIGGER_LIFT_ON, "Follows Lift:OFF","Follows Lift:ON ", 15+dim, 15, 13+dim, 13))
            {
               mdw_slider(    xa, ya, xb, ya+bts-2, 91,  num, type, obt, 0, 13, 15, 15, 1,0,0,d); ya+=bts;  // Lift Number
               mdw_button(    xa, ya, xb, bts, 211, num, type, obt, 0, 13, 15,  0, 1,0,1,d);  // X Alignment
               mdw_button(    xa, ya, xb, bts, 212, num, type, obt, 0, 13, 15,  0, 1,0,1,d);  // Y Alignment
            }

            ya+=bts;

            mdw_slider(xa, ya, xb, ya+bts-2, 93,  num, type, obt, 0, 13, 15, 15, 1,0,0,d); ya+=bts;  // trigger output CURR ON
            mdw_slider(xa, ya, xb, ya+bts-2, 94,  num, type, obt, 0, 13, 15, 15, 1,0,0,d); ya+=bts;  // trigger output CURR OFF
            mdw_slider(xa, ya, xb, ya+bts-2, 95,  num, type, obt, 0, 13, 15, 15, 1,0,0,d); ya+=bts;  // trigger output TGON
            mdw_slider(xa, ya, xb, ya+bts-2, 96,  num, type, obt, 0, 13, 15, 15, 1,0,0,d); ya+=bts;  // trigger output TGOF
         }
         break;
         case 16: // block manip
            if (mdw_buttont(xa, ya, xb, bts,      1,0,0,0,            0,14,14,14, 1,0,0,d, "Get New Block Manip Field")) get_block_range("Block Manip Rectangle", &item[num][6], &item[num][7], &item[num][8], &item[num][9], 1);
            ya+=bts; // spacing between groups
            if (mdw_toggle(xa, ya, xb, bts,       1,0,0,0,            0,0,0,0,    1,0,0,d, item[num][2], "Draw Block Manip Field:OFF", "Draw Block Manip Field:ON ", 15, 15, 4, 4))
                mdw_colsel(xa, ya, xb, bts,       7,  num, type, obt, 0,0,0,0,    0,0,1,d);          // color select

            mdw_buttonp(   xa, ya, xb, bts,      301,0,0,0,           0,14,15, 0, 1,0,1,d, item[num][3]); // mode
            mdw_button(    xa, ya, xb, bts,      310, num, type, obt, 0,14,15, 0, 1,0,0,d); ya+=bts+6; // block 1
            mdw_button(    xa, ya, xb, bts,      311, num, type, obt, 0,14,15, 0, 1,0,0,d); ya+=bts*2; // block 2
            mdw_slider(    xa, ya, xb, ya+bts-2,  92, num, type, obt, 0,13,15,15, 1,0,0,d); ya+=bts;   // pm_event trigger
            mdw_button(    xa, ya, xb, bts,      320, num, type, obt, 0,13,15, 0, 1,0,1,d); // set trigger
         break;
         case 17: // block damage
         {
            if (mdw_buttont(xa, ya, xb, bts, 1,  0,0,0, 0,10,15,0, 1,0,0,d, "Get New Block Damage Field")) get_block_range("Block Damage Rectangle", &item[num][6], &item[num][7], &item[num][8], &item[num][9], 1);
            mdw_buttonp(    xa, ya, xb, bts, 404,0,0,0, 0, 8,15,0, 1,0,1,d, item[num][2]); // damage draw mode
            mdw_buttonp(    xa, ya, xb, bts, 402,0,0,0, 0,10,15,0, 1,0,1,d, item[num][11]); // MODE
            int MODE = item[num][11];

            if (MODE == 1) mdw_togglf(xa, ya, xb, bts, 1,0,0,0,  0,0,0,0,  1,0,0,d, item[num][3], PM_ITEM_DAMAGE_CURR, "Initial State:OFF","Initial State:ON ", 15+dim, 15, 10+dim, 10);

            if ((MODE == 2) || (MODE == 3) || (MODE == 4)) // Mode 2, 3 and 4
            {
               if (MODE == 2) { mdw_slider(xa, ya, xb, ya+bts-2, 104,  num, type, obt, 0, 11, 15, 15, 1,0,0,d); ya+=bts;} // Damage Field Off Time
               if (MODE == 3) { mdw_slider(xa, ya, xb, ya+bts-2, 103,  num, type, obt, 0, 11, 15, 15, 1,0,0,d); ya+=bts;} // Damage Field On Time
               if (MODE == 4) // Timed on and off
               {
                  mdw_slider(xa, ya, xb, ya+bts-2, 100, num, type, obt, 0, 11, 15, 15, 1,0,0,d); ya+=bts;  // total timer
                  mdw_slider(xa, ya, xb, ya+bts-2, 102, num, type, obt, 0, 11, 15, 15, 1,0,0,d); ya+=bts;  // damage time
                  mdw_slider(xa, ya, xb, ya+bts-2, 101, num, type, obt, 0, 11, 15, 15, 1,0,0,d); ya+=bts;  // initial time
               }
               mdw_button(   xa, ya, xb, bts,      401, num, type, obt, 0, 11, 15, 15, 1,0,1,d); // timer draw mode
            }

            ya+=bts;

            if (mdw_togglf(xa, ya, xb, bts, 1,0,0,0,  0,0,0,0,  1,0,0,d, item[num][3], PM_ITEM_DAMAGE_LIFT_ON, "Follows Lift:OFF","Follows Lift:ON ", 15+dim, 15, 13+dim, 13))
            {
               mdw_slider(xa, ya, xb, ya+bts-2, 97,  num, type, obt, 0, 13, 15, 15, 1,0,0,d); ya+=bts;  // Lift Number
               mdw_button(xa, ya, xb, bts,      411, num, type, obt, 0, 13, 15,  0, 1,0,1,d);  // X Alignment
               mdw_button(xa, ya, xb, bts,      412, num, type, obt, 0, 13, 15,  0, 1,0,1,d);  // Y Alignment
            }

            ya+=bts;

            if (mdw_togglf(xa, ya, xb, bts, 1,0,0,0,  0,0,0,0,  1,0,0,d, item[num][3], PM_ITEM_DAMAGE_PLAYER, "Affects Players:OFF         ","Affects Players:ON          ", 15+dim, 15, 10+dim, 10))
               if (!mdw_togglf(xa, ya, xb, bts, 1,0,0,0,  0,0,0,0,  1,0,0,d, item[num][3], PM_ITEM_DAMAGE_INSTGIB, "Player Instant Death:OFF  ","Player Instant Death:ON   ", 15+dim, 15, 10+dim, 10))
               {
                  mdw_slider(xa, ya, xb, ya+bts-2, 98,  num, type, obt, 0, 10,    15,  0, 1,0,0,d); ya+=bts;   // Player damage
               }

            mdw_togglf(xa, ya, xb, bts, 1,0,0,0,  0,0,0,0,  1,0,0,d, item[num][3], PM_ITEM_DAMAGE_ENEMY, "Affects Enemies:OFF         ","Affects Enemies:ON          ", 15+dim, 15, 10+dim, 10);
            mdw_togglf(xa, ya, xb, bts, 1,0,0,0,  0,0,0,0,  1,0,0,d, item[num][3], PM_ITEM_DAMAGE_ITEM,  "Affects Items:OFF           ","Affects Items:ON            ", 15+dim, 15, 10+dim, 10);
            mdw_togglf(xa, ya, xb, bts, 1,0,0,0,  0,0,0,0,  1,0,0,d, item[num][3], PM_ITEM_DAMAGE_PBUL,  "Affects Player's Bullets:OFF","Affects Player's Bullets:ON ", 15+dim, 15, 10+dim, 10);
            mdw_togglf(xa, ya, xb, bts, 1,0,0,0,  0,0,0,0,  1,0,0,d, item[num][3], PM_ITEM_DAMAGE_EBUL,  "Affects Enemy's Bullets:OFF ","Affects Enemy's Bullets:ON  ", 15+dim, 15, 10+dim, 10);


            if ((MODE == 1) || (MODE == 2) || (MODE == 3)) // Mode 1, 2, and 3
            {
               ya+=bts;
               mdw_slider(xa, ya, xb, ya+bts-2, 92, num,type,obt,  0,13,15,15, 1,0,0,d); ya+=bts; // pm_event trigger
               mdw_button(xa, ya, xb, bts,      320,num,type,obt,  0,13,15, 0, 1,0,1,d); ya+=bts; // set trigger
            }
         }
         break;
         case 10: // message
            mdw_buttonp(    xa, ya, xb, bts,      22,0,0,0,        0,15,13,14, 1,0,1,d, item[num][3]); // stat | fall | carry
            if (mdw_buttont(xa, ya, xb, bts,       1,0,0,0,        0,14,15, 0, 1,0,0,d, "Set Message Position"))  getxy("Message Position", 95, 10, num);
            mdw_slider(     xa, ya, xb, ya+bts-2, 54,num,type,obt, 0,11,15,15, 1,0,0,d); ya+=bts; // display time
            mdw_colsel(     xa, ya, xb, bts,       2,num,type,obt, 0, 0, 0, 0, 0,0,1,d);  // frame color select
            mdw_colsel(     xa, ya, xb, bts,       3,num,type,obt, 0, 0, 0, 0, 0,0,1,d);  // text color select
            if (mdw_buttont(xa, ya, xb, bts,       1,0,0,0,        0,15,13, 0, 1,0,0,d, "Edit Message")) edit_pmsg_text(num, 0);

            ya+=bts*2; // leave space for OK and Cancel buttons

            // draw the current one last to ensure it is on top
            mW[7].pop_msg_viewer_pos = ya+bts/2+2;

            display_pop_message(num, pmsgtext[num], (xa+xb)/2, mW[7].pop_msg_viewer_pos, 0, 0); // show the message
            ya+=bts*8;

         break;
         case 11: // rocket
            mdw_buttonp(xa, ya, xb, bts, 23,0,0,0,  0, 15,  13, 14, 1,0,1,d, item[num][3]); // stat | fall | | ride through door
            if (mdw_buttont(xa, ya, xb, bts, 1,0,0,0,  0,10,15,0,1,0,0,d, "Set Initial Direction")) getxy("Initial Direction", 97, 11, num);
            mdw_slider(xa, ya, xb, ya+bts-2,  4, num, type, obt, 0, 14, 15, 15, 1,0,0,d); ya+=bts; // damage range
            mdw_slider(xa, ya, xb, ya+bts-2,  6, num, type, obt, 0, 12, 15, 15, 1,0,0,d); ya+=bts; // acceleration
            mdw_slider(xa, ya, xb, ya+bts-2,  7, num, type, obt, 0, 12, 15, 15, 1,0,0,d); ya+=bts; // max speed
            mdw_slider(xa, ya, xb, ya+bts-2,  8, num, type, obt, 0, 12, 15, 15, 1,0,0,d); ya+=bts; // steerability
         break;
         case 14: // switch
            mdw_buttonp(xa, ya, xb, bts, 22,0,0,0,  0, 15,  13, 14, 1,0,1,d, item[num][3]); // stat | fall | carry
            if (mdw_buttont(xa, ya, xb, bts, 1,0,0,0,  0,10,15,0,1,0,0,d, "Get New Block Range")) get_block_range("Block Range", &item[num][6], &item[num][7], &item[num][8], &item[num][9], 1);
            mdw_togglf(xa, ya, xb, bts, 1,0,0,0,  0,0,0,0,  1,0,0,d, item[num][10], 0b0001 , "Green:OFF", "Green:ON",  15+dim, 15, 9+dim, 9);
            mdw_togglf(xa, ya, xb, bts, 1,0,0,0,  0,0,0,0,  1,0,0,d, item[num][10], 0b0010 , "Red:OFF",   "Red:ON",    15+dim, 15, 10+dim, 10);
            mdw_togglf(xa, ya, xb, bts, 1,0,0,0,  0,0,0,0,  1,0,0,d, item[num][10], 0b0100 , "Blue:OFF",  "Blue:ON",   15+dim, 15, 12+dim, 12);
            mdw_togglf(xa, ya, xb, bts, 1,0,0,0,  0,0,0,0,  1,0,0,d, item[num][10], 0b1000 , "Purple:OFF","Purple:ON", 15+dim, 15, 8+dim, 8);
            set_switch_tile(num);
         break;
         case 15: // sproingy
            mdw_buttonp(xa, ya, xb, bts,      22,0,0,0,        0,15,13, 0, 1,0,1,d, item[num][3]); // stat | fall | carry
            mdw_slider( xa, ya, xb, ya+bts-2, 10,num,type,obt, 0,12,15,15, 1,0,0,d); ya+=bts;      // jump height
         break;
         case 12: // warp
            mdw_buttonp(xa, ya, xb, bts,      21,0,0,0,        0,15,13, 0, 1,0,1,d, item[num][3]); // stat | fall
            mdw_slider( xa, ya, xb, ya+bts-2, 28,num,type,obt, 0,12,15,15, 1,0,0,d); ya+=bts;      // warp level
         break;
      }
   }

   // set height
   if (obt != 4)
   {
      int llo = (3 - mW[7].num_legend_lines)*8; // legend line offset
      mW[7].h = ya-y1+32-llo;
      mW[7].set_size(mW[7].w, mW[7].h);
   }
}
void ovw_draw_overlays(int legend_highlight)
{
   int obt = mW[7].obt;
   int num = mW[7].num;

   al_set_target_bitmap(level_buffer);

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

      al_draw_rectangle(x1, y1, x2, y2, palette_color[color], 1);

      al_draw_line(xc, 0, xc, y1, palette_color[color], 1);
      al_draw_line(xc, y2, xc, 2000, palette_color[color], 1);
      al_draw_line(0, yc, x1, yc, palette_color[color], 1);
      al_draw_line(x2, yc, 2000, yc, palette_color[color], 1);
   }
   if (obt == 3)  // enemies
   {
      int type = Ei[num][0];
      int obj_x = al_fixtoi(Efi[num][0])+10;
      int obj_y = al_fixtoi(Efi[num][1])+10;

      int color = 13;
      if (legend_highlight == 1) color = flash_color;
      crosshairs_full(obj_x, obj_y, color, 1);

      if ((type == 3) || (type == 6) || (type == 8) || (type == 12)) // archwagon, cannon, trakbot, flapper
      {
         // draw yellow bullet prox circle
         int color = 14;
         if (legend_highlight == 2) color = flash_color;
         al_draw_circle(obj_x, obj_y, Ei[num][17], palette_color[color], 1);
      }
      if (type == 7) // podzilla
      {
         // extended position
         int color1 = 10;
         if (legend_highlight == 2) color1 = flash_color;

         int px=0, py=0;
         get_pod_extended_position(num, &px, &py);
         crosshairs_full(px+10, py+10, color1, 1);

         // draw tile at extended pos
         float rot = al_fixtof(al_fixmul(Efi[num][14], al_fixtorad_r));
         al_draw_scaled_rotated_bitmap(tile[Ei[num][1]], 10, 10, px+10, py+10, 1, 1, rot, ALLEGRO_FLIP_HORIZONTAL);

         // draw connecting line
         al_draw_line(obj_x, obj_y, px+10, py+10, palette_color[10], 1);

         // trigger box
         int color = 14;
         if (legend_highlight == 3) color = flash_color;
         int tx1 = Ei[num][11];
         int ty1 = Ei[num][12];
         int tx2 = Ei[num][11]+Ei[num][13] + 20;
         int ty2 = Ei[num][12]+Ei[num][14] + 20;
         al_draw_rectangle(tx1, ty1, tx2, ty2, palette_color[color], 1);
      }
      if (type == 9) // cloner
      {
         int color2 = 11;
         if (legend_highlight == 2) color2 = flash_color;

         int color3 = 10;
         if (legend_highlight == 3) color3 = flash_color;

         int color4 = 14;
         if (legend_highlight == 4) color4 = flash_color;

         int cw = Ei[num][19];     // width
         int ch = Ei[num][20];     // height

         int cx1 = Ei[num][15];    // source
         int cy1 = Ei[num][16];
         int cx2 = cx1 + cw;
         int cy2 = cy1 + ch;
         al_draw_rectangle(cx1, cy1, cx2, cy2, palette_color[color2], 1);

         int cx3 = Ei[num][17];    // destination
         int cy3 = Ei[num][18];
         int cx4 = cx3 + cw;
         int cy4 = cy3 + ch;
         al_draw_rectangle(cx3, cy3, cx4, cy4, palette_color[color3], 1);

         // draw trigger box
         int tx1 = Ei[num][11];
         int ty1 = Ei[num][12];
         int tx2 = Ei[num][11]+Ei[num][13] + 20;
         int ty2 = Ei[num][12]+Ei[num][14] + 20;
         al_draw_rectangle(tx1, ty1, tx2, ty2, palette_color[color4], 1);
      }
      if (type == 12) // flapper
      {
         int color = 10;
         if (legend_highlight == 3) color = flash_color;

         // draw red height above player line
         int hab = Ei[num][20];
         al_draw_line(obj_x-40, obj_y+hab, obj_x+40, obj_y+hab, palette_color[color], 3);

         // draw flap height
         int fh = Ei[num][21];
         al_draw_line(obj_x-60, obj_y+fh, obj_x+60, obj_y+fh, palette_color[12], 1);
         al_draw_line(obj_x-60, obj_y-fh, obj_x+60, obj_y-fh, palette_color[12], 1);
      }
   }
   if (obt == 2)  // items
   {
      int type = item[num][0];
      int obj_x = item[num][4]+10;
      int obj_y = item[num][5]+10;
      int color = 13;
      if (legend_highlight == 1) color = flash_color;
      crosshairs_full(obj_x, obj_y, color, 1);
      switch (type)
      {
         case 1: // door
         {
            int color = 10;
            if (legend_highlight == 2) color = flash_color;
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
                     al_draw_line(obj_x, obj_y, x2, y2, palette_color[color], 1);
                  }
            }

            if (item[num][8] == 1)  // draw destination
            {
               // dest item
               int di = item[num][9];
               int x2 = item[di][4]+10;
               int y2 = item[di][5]+10;
               crosshairs_full(x2, y2, color, 1);
               al_draw_line(obj_x, obj_y, x2, y2, palette_color[color], 1);
            }
         }
         break;
         case 4: // key
         {
            int color = 10;
            if (legend_highlight == 2) color = flash_color;
            int x2 = item[num][6];
            int y2 = item[num][7];
            int x3 = x2 + item[num][8] - 1;
            int y3 = y2 + item[num][9] - 1;;
            int x4 = (x2+x3)/2;
            int y4 = (y2+y3)/2;

            if (x2 == 0) x2 = 1; // to keep it visible
            if (y2 == 0) y2 = 1;

            // draw range
            al_draw_line(0, y4, 1999, y4, palette_color[color], 1);
            al_draw_line(x4, 0, x4, 1999, palette_color[color], 1);
            al_draw_rectangle(x2, y2, x3, y3, palette_color[color], 1);

            // show blocks that will be affected
            proc_key_block_range(num, 2);
         }
         break;
         case 14: // switch
         {
            int color = 10;
            if (legend_highlight == 2) color = flash_color;
            int x2 = item[num][6];
            int y2 = item[num][7];
            int x3 = x2 + item[num][8] - 1;
            int y3 = y2 + item[num][9] - 1;;
            int x4 = (x2+x3)/2;
            int y4 = (y2+y3)/2;

            if (x2 == 0) x2 = 1; // to keep it visible
            if (y2 == 0) y2 = 1;

            // draw range
            al_draw_line(0, y4, 1999, y4, palette_color[color], 1);
            al_draw_line(x4, 0, x4, 1999, palette_color[color], 1);
            al_draw_rectangle(x2, y2, x3, y3, palette_color[color], 1);

            // show blocks that will be affected
            proc_switch_block_range(num, 2);
         }
         break;
         case 8: // bomb
         {
            int color = 14;
            if (legend_highlight == 2) color = flash_color;
            al_draw_circle(obj_x, obj_y, item[num][7], palette_color[color], 1); // draw yellow bomb damage
         }
         break;
         case 9: // trigger
         {
            int color = 14;
            if (legend_highlight == 2) color = flash_color;

            int x2 = item[num][6];
            int y2 = item[num][7];
            int x3 = x2 + item[num][8] - 1;
            int y3 = y2 + item[num][9] - 1;;
            int x4 = (x2+x3)/2;
            int y4 = (y2+y3)/2;

            // draw range
            al_draw_line(0, y4, 1999, y4, palette_color[color], 1);
            al_draw_line(x4, 0, x4, 1999, palette_color[color], 1);
            al_draw_rectangle(x2, y2, x3, y3, palette_color[color], 1);

            find_and_show_event_links(1, num, 0);
         }
         break;
         case 11: // rocket
         {
            int color = 14;
            if (legend_highlight == 2) color = flash_color;
            al_draw_circle(obj_x, obj_y, item[num][7], palette_color[color], 1); // draw yellow bomb damage
         }
         break;
         case 15: // sproingy
         {
            int color = 14;
            if (legend_highlight == 2) color = flash_color;
            int y = al_fixtoi(get_sproingy_jump_height(num));
            crosshairs_full(obj_x, obj_y-y, color, 1);
         }
         break;
         case 16: // block manip
         {
            int color = 12;
            if (legend_highlight == 2) color = flash_color;
            int x2 = item[num][6];
            int y2 = item[num][7];
            int x3 = x2 + item[num][8]-1;
            int y3 = y2 + item[num][9]-1;;
            int x4 = (x2+x3)/2;
            int y4 = (y2+y3)/2;

            // draw range
            al_draw_line(0, y4, 1999, y4, palette_color[color], 1);
            al_draw_line(x4, 0, x4, 1999, palette_color[color], 1);
            al_draw_rectangle(x2, y2, x3, y3, palette_color[color], 1);

            find_and_show_event_links(2, num, 0);

         }
         break;
         case 17: // block damage
         {
            int color = 10;
            if (legend_highlight == 2) color = flash_color;
            int x2 = item[num][6];
            int y2 = item[num][7];
            int x3 = x2 + item[num][8]-1;
            int y3 = y2 + item[num][9]-1;;
            int x4 = (x2+x3)/2;
            int y4 = (y2+y3)/2;

            // draw range
            al_draw_line(0, y4, 1999, y4, palette_color[color], 1);
            al_draw_line(x4, 0, x4, 1999, palette_color[color], 1);
            al_draw_rectangle(x2, y2, x3, y3, palette_color[color], 1);

            find_and_show_event_links(2, num, 0);
         }
         break;

      } // end of switch case
   }
}
void ovw_process_mouse(void)
{
   int lift=0, step=0;
   if (mW[7].obt == 4)
   {
      lift = mW[7].num;
      step = lifts[lift].current_step;
   }

   int mouse_on_obj = 0;
   int mouse_on_podx = 0;
   int mouse_on_csb_ul = 0;
   int mouse_on_csb_lr = 0;
   int mouse_on_cdb_ul = 0;
   int mouse_on_trk = 0;

   int mouse_on_sp = 0;
   int mouse_on_bmb = 0;
   int mouse_on_msg_ul = 0;
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
         if ((hx>ex+msn) && (hx<ex+msp) && (hy>ey+msn) && (hy<ey+msp) && (!mW[7].viewer_lock) && (!key[MAP_LOCK_KEY]) && (obj_filter[3][type]))
         {
            // set this enemy to current object
            mW[7].obt = 3;
            mW[7].num = b;
            mouse_move = 1;
            mouse_on_obj = 1;
         }

         // if this object is already current object
         if ((hx>ex+msn) && (hx<ex+msp) && (hy>ey+msn) && (hy<ey+msp) && (mW[7].obt == 3) && (mW[7].num == b))
         {
            mouse_move = 1;
            mouse_on_obj = 1;
         }
      }

   // if current object is enemy, check for secondaries
   if (mW[7].obt == 3)
   {
      int b = mW[7].num;
      int type = Ei[b][0];
      if ((type == 3) || (type == 6) || (type == 8) || (type == 12)) // archwagon, cannon, trakbot and flapper bullet prox
      {
         float x0 = al_fixtof(Efi[b][0])+10; // get center of item location
         float y0 = al_fixtof(Efi[b][1])+10;
         float fx = (float) hx;
         float fy = (float) hy;
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
         if ((hx>px+msn) && (hx<px+msp) && (hy>py+msn) && (hy<py+msp))
         {
            mouse_move = 1;
            mouse_on_podx = 1;
         }
      }
      if ((type == 7) || (type == 9)) // podzilla and cloner trigger box
      {
         int x1 = Ei[b][11];
         int y1 = Ei[b][12];
         int x2 = Ei[b][11]+Ei[b][13]+20;
         int y2 = Ei[b][12]+Ei[b][14]+20;
         if ((hx>x1-mst) && (hx<x1+mst) && (hy>y1-mst) && (hy<y1+mst)) // upper left corner (move)
         {
            mouse_on_tb_ul = 1;
            mouse_move = 1;
         }
         if ((hx>x2-mst) && (hx<x2+mst) && (hy>y2-mst) && (hy<y2+mst)) // lower right corner (resize)
         {
            mouse_on_tb_lr = 1;
            mouse_adj = 1;
         }
      }
      if (type == 9) // cloner source and destination boxes
      {
         int w = Ei[mW[7].num][19];     // width
         int h = Ei[mW[7].num][20];     // height
         int x1 = Ei[mW[7].num][15];    // source box
         int y1 = Ei[mW[7].num][16];
         int x2 = x1 + w - 1;
         int y2 = y1 + h - 1;
         int x3 = Ei[mW[7].num][17];    // dest box
         int y3 = Ei[mW[7].num][18];
         if ((hx>x1-mst) && (hx<x1+mst) && (hy>y1-mst) && (hy<y1+mst)) // source upper left corner (move)
         {
            mouse_on_csb_ul = 1;
            mouse_move = 1;
         }
         if ((hx>x2-mst) && (hx<x2+mst) && (hy>y2-mst) && (hy<y2+mst)) // source lower right corner (re-size)
         {
            mouse_on_csb_lr = 1;
            mouse_adj = 1;
         }
         if ((hx>x3-mst) && (hx<x3+mst) && (hy>y3-mst) && (hy<y3+mst)) // destination upper left corner (move)
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
         if ((hx>ix+msn) && (hx<ix+msp) && (hy>iy+msn) && (hy<iy+msp) && (!mW[7].viewer_lock) && (!key[MAP_LOCK_KEY]) && (obj_filter[2][type]))
         {
            // set this item to current object
            mW[7].obt = 2;
            mW[7].num = b;
            mouse_move = 1;
            mouse_on_obj = 1;
         }
         // if this object is already current object
         if ((hx>ix+msn) && (hx<ix+msp) && (hy>iy+msn) && (hy<iy+msp) && (mW[7].obt == 2) && (mW[7].num == b))
         {
            mouse_move = 1;
            mouse_on_obj = 1;
         }
      }

   // if current object is item, check for secondaries
   if (mW[7].obt == 2)
   {
      int b = mW[7].num;
      int type = item[b][0];

      if (type == 10) // pop message
      {
         int x1 = item[mW[7].num][10];
         int y1 = item[mW[7].num][11];
         if ((hx>x1-mst) && (hx<x1+mst) && (hy>y1-mst) && (hy<y1+mst)) // upper left corner (move)
         {
            mouse_on_msg_ul = 1;
            mouse_move = 1;
         }
      }
      if ((type == 8) || (type == 11)) // bomb or rocket
      {
         float x0 = (float) item[mW[7].num][4]+10; // get center of item location
         float y0 = (float) item[mW[7].num][5]+10;
         float fx = (float) hx;
         float fy = (float) hy;
         float dst = sqrt(pow((x0-fx), 2) + pow((y0-fy), 2)); // distance from mouse to item
         float bdr = (float) item[mW[7].num][7]; // bomb damage radius
         float dif = dst-bdr; // difference
         if ((dif < 3 ) && (dif > -3))  // mouse is on blast radius circle
         {
            mouse_on_bmb = 1;
            mouse_move = 1;
         }
      }
      if (type == 15) // sproingy
      {
         int x1 = item[mW[7].num][4];
         int y1 = item[mW[7].num][5];
         int y2 = y1 - al_fixtoi(get_sproingy_jump_height(mW[7].num));

         if ((hx>x1+msn) && (hx<x1+msp) && (hy>y2+msn) && (hy<y2+msp))
         {
            mouse_on_sp = 1;
            mouse_move = 1;
         }
      }
      if ((type == 4) || (type == 9) || (type == 14) || (type == 16) || (type == 17)) // key, switch, trigger, manip, damage
      {
         int x1 = item[mW[7].num][6];
         int y1 = item[mW[7].num][7];
         int x2 = x1 + item[mW[7].num][8];
         int y2 = y1 + item[mW[7].num][9];

         if ((hx>x1-mst) && (hx<x1+mst) && (hy>y1-mst) && (hy<y1+mst)) // upper left corner (move)
         {
            mouse_on_kbr_ul = 1;
            mouse_move = 1;
         }
         if ((hx>x2-mst) && (hx<x2+mst) && (hy>y2-mst) && (hy<y2+mst)) // lower right corner (resize)
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
   if (obj_filter[4][1])
   {
      if ((!key[MAP_LOCK_KEY]) && (!mW[7].viewer_lock)) // no lock...check all lifts and steps
      {
         for (int x=0; x<num_lifts; x++)  // cycle lifts
            for (int y=0; y<lifts[x].num_steps; y++)  // cycle steps
               if ((lift_steps[x][y].type & 31) == 1) // look for move step
               {
                  int w =  lift_steps[x][y].w / 2;
                  int h =  lift_steps[x][y].h / 2;
                  int nx = lift_steps[x][y].x + w;
                  int ny = lift_steps[x][y].y + h;
                  if ((hx > nx - w)  && (hx < nx + w) && (hy > ny - h)  && (hy < ny + h)) // is mouse on this step ?
                  {
                     mouse_on_lift = 1;
                     mW[7].obt = 4;
                     mW[7].num = x;
                     lift = x;
                     step = y;
                     set_lift_to_step(lift, step);   // set current step in current lift
                  }
               }
      }

      if (((key[MAP_LOCK_KEY]) || (mW[7].viewer_lock)) && (mW[7].obt == 4)) // locked, but locked to current lift
      {
         int x = lift; // check only current lift
         for (int y=0; y<lifts[x].num_steps; y++)  // cycle steps
            if ((lift_steps[x][y].type & 31) == 1) // look for move step
            {
               int w =  lift_steps[x][y].w / 2;
               int h =  lift_steps[x][y].h / 2;
               int nx = lift_steps[x][y].x + w;
               int ny = lift_steps[x][y].y + h;
               if ((hx > nx - w)  && (hx < nx + w) && (hy > ny - h)  && (hy < ny + h)) // is mouse on this step ?
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
      if (mouse_b2) step_popup_menu(lift, step);

      // is mouse on lower right adjustable corner
      int x2 = lift_steps[lift][step].x + lift_steps[lift][step].w;
      int y2 = lift_steps[lift][step].y + lift_steps[lift][step].h;
      if ((hx > x2-8) && (hy > y2-8)) mouse_adj = 1;
      else mouse_move = 1;
   }








   // -----------------------------------------------------------
   // --  set mouse cursor
   // -----------------------------------------------------------
   if (mouse_move) al_set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_MOVE);
   else if (mouse_adj) al_set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_RESIZE_SE);
   else al_set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);



   if (mouse_b1)
   {

      // get offset from where mouse was clicked to lift step upper left origin
      int lsox=0, lsoy=0;
      if (mouse_on_lift)
      {
         int l = mW[7].num;
         int s = lifts[l].current_step;
         lsox = (gx - lift_steps[l][s].x/20);
         lsoy = (gy - lift_steps[l][s].y/20);
      }


      while (mouse_b1)
      {
         if (mouse_on_obj)
         {
            //printf("mouse pressed on obj\n");
            if (mW[7].obt == 2) // move item
            {
               // get offset of move
               int x_off = gx - item[mW[7].num][4] / 20;
               int y_off = gy - item[mW[7].num][5] / 20;

               item[mW[7].num][4] = gx*20;
               item[mW[7].num][5] = gy*20;
               itemf[mW[7].num][0] = al_itofix(gx*20);
               itemf[mW[7].num][1] = al_itofix(gy*20);

               if ((key[ALLEGRO_KEY_LSHIFT]) || (key[ALLEGRO_KEY_RSHIFT])) // move stuff also
               {
                   if ((item[mW[7].num][0] == 4) || (item[mW[7].num][0] == 9) || (item[mW[7].num][0] == 16) || (item[mW[7].num][0]== 17)) // key, trigger, manip, damage
                   {
                      item[mW[7].num][6] += x_off*20;
                      item[mW[7].num][7] += y_off*20;
                   }
                   if (item[mW[7].num][0] == 10) // msg
                   {
                      item[mW[7].num][10] += x_off*20;
                      item[mW[7].num][11] += y_off*20;
                   }
               }
            }
            if (mW[7].obt == 3) // move enemy
            {
               // get offset of move
               int x_off = gx - al_fixtoi(Efi[mW[7].num][0]) / 20;
               int y_off = gy - al_fixtoi(Efi[mW[7].num][1]) / 20;

               // set new position
               Efi[mW[7].num][0] = al_itofix(gx*20);
               Efi[mW[7].num][1] = al_itofix(gy*20);

               if ((key[ALLEGRO_KEY_LSHIFT]) || (key[ALLEGRO_KEY_RSHIFT])) // move stuff also
               {
                  // move podzilla's trigger box too
                  if (Ei[mW[7].num][0] == 7)
                  {
                     Ei[mW[7].num][11] += x_off*20;
                     Ei[mW[7].num][12] += y_off*20;
                  }
                  // move cloner's stuff too
                  if (Ei[mW[7].num][0] == 9)
                  {
                     Ei[mW[7].num][11] += x_off*20; // trigger box
                     Ei[mW[7].num][12] += y_off*20;
                     Ei[mW[7].num][15] += x_off*20; // source
                     Ei[mW[7].num][16] += y_off*20;
                     Ei[mW[7].num][17] += x_off*20; // dest
                     Ei[mW[7].num][18] += y_off*20;
                  }
               }
            }
            if (mW[7].obt == 4) // lift
            {
               if (mouse_move)
               {
                  lift_steps[lift][step].x = (gx-lsox)*20;
                  lift_steps[lift][step].y = (gy-lsoy)*20;
//                  lift_steps[lift][step].x = gx*20;
//                  lift_steps[lift][step].y = gy*20;

                  set_lift_to_step(lift, step);   // set current step in current lift
               }
               if (mouse_adj)
               {
                  // don't allow lr to be less than ul
                  if (gx < lift_steps[lift][step].x/20+1) gx = lift_steps[lift][step].x/20+1;
                  if (gy < lift_steps[lift][step].y/20+1) gy = lift_steps[lift][step].y/20+1;

                  // set new position
                  lift_steps[lift][step].w = gx*20 - lift_steps[lift][step].x;
                  lift_steps[lift][step].h = gy*20 - lift_steps[lift][step].y;
                  set_lift_to_step(lift, step);   // set current step in current lift
               }
            }
         }
         if (mouse_on_podx)
         {
            Efi[mW[7].num][5] = al_itofix(gx * 20);
            Efi[mW[7].num][6] = al_itofix(gy * 20);
            recalc_pod(mW[7].num);
         }
         if (mouse_on_tb_ul) // move trigger box from ul
         {
            //printf("mouse pressed on tb_ul\n");
            Ei[mW[7].num][11] = gx*20;
            Ei[mW[7].num][12] = gy*20;
         }
         if (mouse_on_tb_lr)  // resize trigger box from lr
         {
            // prevent lr corner from being less than ul corner
            if (gx < Ei[mW[7].num][11]/20) gx = Ei[mW[7].num][11]/20;
            if (gy < Ei[mW[7].num][12]/20) gy = Ei[mW[7].num][12]/20;
            // set new postion
            Ei[mW[7].num][13] = gx*20 - Ei[mW[7].num][11];
            Ei[mW[7].num][14] = gy*20 - Ei[mW[7].num][12];
         }
         if (mouse_on_msg_ul) // move msg
         {
            item[mW[7].num][10] = gx*20;
            item[mW[7].num][11] = gy*20;
         }
         if (mouse_on_sp) // adjust sproingy jump height
         {
            item[mW[7].num][7] -= mouse_dy/2;
            // bounds check
            if (item[mW[7].num][7] < 40) item[mW[7].num][7] = 40;
            if (item[mW[7].num][7] > 200) item[mW[7].num][7] = 200;
         }
         if (mouse_on_bmb) // adjust bomb blast radius
         {
            float x0 = (float) item[mW[7].num][4]+10; // get center of item location
            float y0 = (float) item[mW[7].num][5]+10;
            float fx = (float) hx;
            float fy = (float) hy;
            float dist = sqrt(pow((x0-fx), 2) + pow((y0-fy), 2)); // distance from mouse to item
            item[mW[7].num][7] = (int) dist;
         }
         if (mouse_on_trk) // adjust trakbot bulet prox
         {
            float x0 = al_fixtof(Efi[mW[7].num][0])+10; // get center of item location
            float y0 = al_fixtof(Efi[mW[7].num][1])+10;
            float fx = (float) hx;
            float fy = (float) hy;
            float dst = sqrt(pow((x0-fx), 2) + pow((y0-fy), 2)); // distance from mouse
            Ei[mW[7].num][17] = (int) dst;
         }
         // ranges for key, trigger, manip and damage
         if (mouse_on_kbr_ul) // move block range from ul
         {
            // set new position
            item[mW[7].num][6] = gx*20;
            item[mW[7].num][7] = gy*20;
         }
         if (mouse_on_kbr_lr) // adjust block range from lr
         {
            // don't allow lr to be less than ul
            if (gx < item[mW[7].num][6]/20) gx = item[mW[7].num][6]/20;
            if (gy < item[mW[7].num][7]/20) gy = item[mW[7].num][7]/20;

            // set new position
            item[mW[7].num][8] = gx*20 - item[mW[7].num][6];
            item[mW[7].num][9] = gy*20 - item[mW[7].num][7];
         }
         if (mouse_on_csb_ul) // move cloner source box from ul
         {
            Ei[mW[7].num][15] = gx*20; // set new postion
            Ei[mW[7].num][16] = gy*20;
         } // end of mouse csb_ul
         if (mouse_on_csb_lr) // resize box from lr
         {
            // get ul corner
            int x1 = Ei[mW[7].num][15]/20;
            int y1 = Ei[mW[7].num][16]/20;

            // prevent lr corner from being less than ul corner
            if (gx < x1+1) gx = x1+1;
            if (gy < y1+1) gy = y1+1;

            // set new sizes
            Ei[mW[7].num][19] = (gx-x1)*20;
            Ei[mW[7].num][20] = (gy-y1)*20;
         } // end of mouse csb_lr

         if (mouse_on_cdb_ul) // cloner destination ul
         {
            Ei[mW[7].num][17] = gx*20; // set new postion
            Ei[mW[7].num][18] = gy*20;
         }
         cm_redraw_level_editor_background();
      } // end of while mouse pressed
   } // end of if mouse pressed
}
int ovw_process_keypress(void)
{



   int quit = 0;
   int mb = mW[7].mb;
   int type=0, lift=0, step=0;
   if (mW[7].obt == 2) type = item[mW[7].num][0];
   if (mW[7].obt == 3) type = Ei[mW[7].num][0];
   if (mW[7].obt == 4)
   {
      lift = mW[7].num;
      step = lifts[mW[7].num].current_step;
      set_lift_to_step(lift, step);   // set current step in current lift
      lifts[mW[7].num].current_step = step;
   }
   if (key[ALLEGRO_KEY_B])
   {
      while (key[ALLEGRO_KEY_B]) proc_controllers();
      if ((key[ALLEGRO_KEY_LSHIFT]) || (key[ALLEGRO_KEY_RSHIFT]))
      {
         printf("save bookmark\n");
         bookmark_level = last_level_loaded;
         bookmark_obj = mW[7].obt;
         bookmark_num = mW[7].num;
         save_config();
      }
      else
      {
         printf("load bookmark\n");
         if (bookmark_level == last_level_loaded)
         {
            if ((bookmark_obj == 2) && (item[bookmark_num]))
            {
               mW[7].obt = bookmark_obj;
               mW[7].num = bookmark_num;
            }
            if ((bookmark_obj == 3) && (Ei[bookmark_num]))
            {
               mW[7].obt = bookmark_obj;
               mW[7].num = bookmark_num;
            }
            if ((bookmark_obj == 4) && (bookmark_num < num_lifts))
            {
               mW[7].obt = bookmark_obj;
               mW[7].num = bookmark_num;
            }
         }
      }
   }


   while ((key[ALLEGRO_KEY_ESCAPE]) || (mouse_b2))
   {
      proc_controllers();
      quit = 1;  // wait for release
   }
   while (key[ALLEGRO_KEY_DELETE])
   {
      mb = 20;
      proc_controllers();
   }
   while (key[ALLEGRO_KEY_RIGHT])
   {
      mb = 21;
      proc_controllers();
   }
   while (key[ALLEGRO_KEY_LEFT])
   {
      mb = 22;
      proc_controllers();
   }
   while (key[ALLEGRO_KEY_UP])
   {
      mb = 26;
      proc_controllers();
   }
   while (key[ALLEGRO_KEY_DOWN])
   {
      mb = 27;
      proc_controllers();
   }

   switch(mb)
   {
      case 18: // move
      {
         if (mW[7].obt==2)
         {
            sprintf(msg,"%s", item_name[type]);
            getxy(msg,mW[7].obt, type, mW[7].num);
         }
         if (mW[7].obt==3)
         {
            sprintf(msg,"%s", enemy_name[type]);
            getxy(msg,mW[7].obt, type, mW[7].num);
         }
         if (mW[7].obt==4)
         {
            while (!key[ALLEGRO_KEY_ESCAPE])
            {
               for (int t=0; t<2; t++) move_lifts(1);  // move lifts for 2 frames
               cm_redraw_level_editor_background(0);
               ovw_draw_overlays(0);
               get_new_screen_buffer(3, 0, 0);
               mW[7].draw();
            }
            while (key[ALLEGRO_KEY_ESCAPE]) proc_controllers(); // wait for release
            lift_setup(); // reset all lifts to step 0
         }
      }
      break;
      case 19:
         mW[7].num = create_obj(mW[7].obt, type, mW[7].num);
      break;
      case 20: // delete
         if (mW[7].obt== 2)
         {
            erase_item(mW[7].num);
            sort_item(1);
            if (mW[7].num >= item_first_num[type]+item_num_of_type[type]) mW[7].num--;
            if (item_num_of_type[type] < 1) quit = 1;
         }
         if (mW[7].obt == 3)
         {
            Ei[mW[7].num][0] = 0;
            sort_enemy();
            if (mW[7].num >= e_first_num[type]+e_num_of_type[type]) mW[7].num--;
            if (e_num_of_type[type] < 1) quit = 1;
         }
         if (mW[7].obt == 4)
         {
            erase_lift(mW[7].num);
            if (--mW[7].num < 0) mW[7].num = 0;      // set to prev lift or zero
            if (num_lifts < 1) quit = 1; // if erased last lift; exit lift viewer
         }
      break;
      case 21: // next
         if ((mW[7].obt==2) && (++mW[7].num >= item_first_num[type] + item_num_of_type[type])) mW[7].num--;
         if ((mW[7].obt==3) && (++mW[7].num >= e_first_num[type] + e_num_of_type[type])) mW[7].num--;
         if ((mW[7].obt==4) && (++mW[7].num > num_lifts-1)) mW[7].num--;
      break;
      case 22: // previous
         if ((mW[7].obt==3) && (--mW[7].num < e_first_num[type])) mW[7].num++;
         if ((mW[7].obt==2) && (--mW[7].num < item_first_num[type])) mW[7].num++;
         if ((mW[7].obt==4) && (--mW[7].num < 0)) mW[7].num++;
      break;
      case 24:  // viewer help
         help("Viewer Basics");
      break;
      case 25:  // object help
         if (mW[7].obt==3)
         {
            if (type == 3)  help("Archwagon Viewer");
            if (type == 4)  help("Bouncer Viewer");
            if (type == 6)  help("Cannon Viewer");
            if (type == 7)  help("Podzilla Viewer");
            if (type == 8)  help("Trakbot Viewer");
            if (type == 9)  help("Cloner Viewer");
            if (type == 10) help("Field Viewer");
            if (type == 11) help("Block Walker Viewer");
            if (type == 12) help("Flapper Viewer");
         }
         if (mW[7].obt==2)
         {
            if (type == 1)  help("Door Viewer");
            if (type == 2)  help("Health Bonus Viewer");
            if (type == 3)  help("Exit Viewer");
            if (type == 4)  help("Key Viewer");
            if (type == 5)  help("Start Viewer");
            if (type == 6)  help("Free Man Viewer");
            if (type == 7)  help("Mine Viewer");
            if (type == 8)  help("Bomb Viewer");
            if (type == 10) help("Message Viewer");
            if (type == 11) help("Rocket Viewer");
            if (type == 12) help("Warp Viewer");
            if (type == 14) help("Switch Viewer");
            if (type == 15) help("Sproingy Viewer");
         }
         if (mW[7].obt==4)
         {
            help("Lift Viewer");
         }
      break;
      case 26: // up
      {
         if (mW[7].obt == 2)  // prev item type
         {
            do
            {
               type--;
               if (type < 1) type = 20;
            } while (item_num_of_type[type] == 0);
            mW[7].num = item_first_num[type];
         }
         if (mW[7].obt == 3) // prev enemy type
         {
            while (e_num_of_type[--type] == 0)
               if (type < 3) type = 20;
            mW[7].num = e_first_num[type];
         }
         if (mW[7].obt == 4) // prev lift step
         {
            if (--step < 0) step = 0;
         }
      }
      break;
      case 27: // down
      {
         if (mW[7].obt == 2) // next obj type
         {
            do
            {
               type++;
               if (type > 20) type = 1;
            } while (item_num_of_type[type] == 0);
            mW[7].num = item_first_num[type];
         }
         if (mW[7].obt == 3) // next obj type
         {
            while (e_num_of_type[++type] == 0)
               if (type > 20) type = 1;
            mW[7].num = e_first_num[type];
         }
         if (mW[7].obt == 4) // next lift step
         {
            if (++step > (lifts[lift].num_steps-1)) step = (lifts[lift].num_steps-1);
         }
      }
      break;
   } // end of switch (mb)
   return quit;
}

void object_viewerw(int obt, int num)
{
   set_windows(4); // object viewer

   init_level_background();

   // from here on I will use the internal mW values
   mW[7].obt = obt;
   mW[7].num = num;

   int quit=0;
   while (!quit)
   {
      cm_redraw_level_editor_background();
      if (!mw_cycle_windows(0)) ovw_process_mouse();
      quit = ovw_process_keypress();
   }
   al_set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);
   set_windows(1); // edit menu
}






































