// mwWidgets.cpp
#include "pm.h"
#include "mwWidgets.h"
#include "mwWindow.h"
#include "mwWindowManager.h"
#include "mwDisplay.h"
#include "e_bitmap_tools.h"
#include "mwFont.h"
#include "mwBitmap.h"
#include "mwLift.h"
#include "mwColor.h"
#include "mwPMEvent.h"
#include "mwInput.h"
#include "mwEventQueue.h"
#include "z_menu.h"
#include "z_item.h"
#include "z_enemy.h"
#include "e_fnx.h"
#include "z_item_door.h"
#include "mwHelp.h"

char smsg[80];
int bw = 3; // slider adjustment bar width




/*

q0 =
q1 = frame color   (if -1 do not draw frame)
q2 = text color    (use white 99% of time)
q3 = slider color  (use white 99% of time)
q4 = redraw mode   (1 for most, 0 for simple proc_control and flip only) // only for mouse button hold on slider bar
q5 = text justify  (0-center 1-left...buttons only)
q6 - increment y1 with bts
q7 - (0-normal) (1-don't process mouse b1 press)



20220601

slideri 68
slider0 27
sliderf 31
sliderd 3
button 29
toggle 42
togglef 25
buttont 65
buttonp 31
colsel 6
-------------
327

*/



// ------------------------------------------------------------------------------------
// ---------------------------sliders--------------------------------------------------
// ------------------------------------------------------------------------------------


void draw_slider_frame(int x1, int y1, int x2, int y2, int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7 )
{
   if (q1 != -1)
   {
      for (int c=0; c<((y2-y1)/2+1); c++)
      {
         int a = (c*32); // color increment
         if (a>224) a = 224;
         // frame fades from solid outer to black inner
         //int a = 224 - (c*32); // color increment
         //if (a<0) a = 0;

         int col = q1+a;
         while (col > 255) col -=16;
         al_draw_rounded_rectangle(x1+c, y1+c, x2-c, y2-c, 1, 1, mC.pc[col], 1);
      }
   }
}


void draw_slider_text(int x1, int y1, int x2, int y2, int q2, int q5)
{
   int xt = (x2+x1)/2;
   int yt = y1 + (y2-y1-8)/2;
   if (q5) al_draw_text(mF.pr8, mC.pc[q2], x1+4, yt, 0, smsg);
   else    al_draw_text(mF.pr8, mC.pc[q2], xt, yt, ALLEGRO_ALIGN_CENTER, smsg);
}

float get_slider_position(float sdx, float sul, float sll, int x1, int y1, int x2, int y2)
{
   float a, b, c, d, e, f;
   // get slider position
   a = sdx-sll; // relative postion
   b = sul-sll; // range
   c = a/b;     // ratio
   d = x2-x1;   // range
   e = d * c;   // range * old ratio
   f = e + x1;  // add offset
   return f;
}


float get_slider_position2(float sul, float sll, float sinc, int q4 ,int x1, int y1, int x2, int y2)
{
   if (q4)
   {
      mwWM.redraw_level_editor_background();
      mwWM.cycle_windows(1);
   }
   else
   {
      mwEQ.proc_event_queue();
      al_flip_display();
   }


   float my = mI.mouse_y;
   float mx = mI.mouse_x;
   float a, b, c, d, e, f ;

   // enforce limits
   if (my<y1) my = y1;
   if (mx<x1) mx = x1;
   if (my>y2) my = y2;
   if (mx>x2) mx = x2;

   // get slider position
   a = mx-x1;                  // relative postion of slider bar in range
   b = x2-x1;                  // range
   c = a / b;                  // ratio = position / range
   d = sul-sll;                // range from buttons
   e = c * d;                  // ratio * range
   f = e + sll;                // add to ll
   f = round(f/sinc) * sinc;   // round to sinc
   return f;
}

float get_slider_position3(float f, float sul, float sll, float sinc, int q4, int x1, int y1, int x2, int y2)
{
   if (q4)
   {
      mwWM.redraw_level_editor_background();
      mwWM.cycle_windows(1);
   }
   else
   {
      mwEQ.proc_event_queue();
      al_flip_display();
   }
   if (mI.mouse_dz)
   {
      int dif = mI.mouse_dz;
      mI.mouse_dz = 0;

      f += dif * sinc;                  // only allow increments of sinc
      if (f < sll) f = sll;             // limit check
      if (f > sul) f = sul;
      f = round(f/sinc) * sinc;         // round to sinc
   }
   return f;
}






float draw_slider_bar(float sdx, float sul, float sll, int x1, int y1, int x2, int y2, int dm, int col)
{
   float f = get_slider_position(sdx, sul, sll, x1, y1, x2, y2);
   int sx1 = (int)f - bw;
   int sx2 = (int)f + bw;
   // draw slider bar
   for (int i=0; i<bw+1; i++)
      al_draw_rectangle(sx1+i, y1+i, sx2-i, y2-i, mC.pc[col+192-(i*64)], 1);

   // draw rectangle around slider bar to show highlight
   if (dm == 2) al_draw_rectangle(sx1-1, y1, sx2+1, y2, mC.pc[15], 1);
   return f;
}


float draw_slider(int x1, int y1, int x2, int y2, int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7, float sdx, float sul, float sll, int order)
{
   float dsx = 0;
   draw_slider_frame(x1, y1, x2, y2, q0, q1, q2, q3, q4, q5, q6, q7);
   if (order == 1)
   {
      dsx = draw_slider_bar(sdx, sul, sll, x1+bw+1, y1, x2-bw-1, y2, 1, q3);
      draw_slider_text(x1, y1, x2, y2, q2, q5);
   }
   if (order == 2)
   {
      draw_slider_text(x1, y1, x2, y2, q2, q5);
      dsx = draw_slider_bar(sdx, sul, sll, x1+bw+1, y1, x2-bw-1, y2, 1, q3);
   }
   return dsx;
}


void mdw_slider0(int x1, int &y1, int x2, int bts, int bn, int num, int type, int obt, int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7,
                 int &var, float sul, float sll, float sinc, const char *txt, const char *txt2)
{
   int y2 = y1+bts-2;
   float sdx = (float) var;
   if (var == 0) sprintf(smsg, "%s%s", txt, txt2);
   else          sprintf(smsg, "%s%d", txt, var);
   float dsx = draw_slider(x1, y1, x2, y2, q0, q1, q2, q3, q4, q5, q6, q7, sdx, sul, sll, 1);


   if (!q7)
   {
      // is mouse on adjustment bar?
      if ((mI.mouse_x > dsx-bw) && (mI.mouse_x < dsx+bw) && (mI.mouse_y > y1) && (mI.mouse_y < y2))
      {
         draw_slider_bar(sdx, sul, sll, x1+bw+1, y1, x2-bw-1, y2, 2, q3); // draw highlighted bar
         al_draw_text(mF.pr8, mC.pc[q2], (x2+x1)/2, (y2+y1)/2-4, ALLEGRO_ALIGN_CENTER, smsg);

         if (mI.mouse_b[3][0]) // only when initially clicked
         {
            while (mI.mouse_b[3][0])
            {
               var = (int)get_slider_position3((float) var, sul, sll, sinc, q4, x1, y1, x2, y2);
               if (var == 0) sprintf(smsg, "%s%s", txt, txt2);
               else          sprintf(smsg, "%s%d", txt, var);
               draw_slider(x1, y1, x2, y2, q0, q1, q2, q3, q4, q5, q6, q7, (float)var, sul, sll, 2);
            }
         }
         if (mI.mouse_b[1][0])
         {
            while (mI.mouse_b[1][0])
            {
               var = (int) get_slider_position2(sul, sll, sinc, q4, x1, y1, x2, y2);
               if (var == 0) sprintf(smsg, "%s%s", txt, txt2);
               else          sprintf(smsg, "%s%d", txt, var);
               draw_slider(x1, y1, x2, y2, q0, q1, q2, q3, q4, q5, q6, q7, (float) var, sul, sll, 1);
            }
         }
      }
      else // if not on adjustment bar, is mouse pressed anywhere else on this button?
      {
         if ((mI.mouse_b[1][0]) && (mI.mouse_x > x1) && (mI.mouse_x < x2) && (mI.mouse_y > y1) && (mI.mouse_y < y2))
         {
            while (mI.mouse_b[1][0])
            {
               var = (int) get_slider_position2(sul, sll, sinc, q4, x1, y1, x2, y2);
               if (var == 0) sprintf(smsg, "%s%s", txt, txt2);
               else          sprintf(smsg, "%s%d", txt, var);
               draw_slider(x1, y1, x2, y2, q0, q1, q2, q3, q4, q5, q6, q7, (float) var, sul, sll, 1);
            }
         }
      }
   }

   if (q6 == 1) y1+=bts;
}


void mdw_slideri(int x1, int &y1, int x2, int bts, int bn, int num, int type, int obt, int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7,
                 int &var, float sul, float sll, float sinc, const char *txt)
{
   int y2 = y1+bts-2;
   float sdx = (float) var;

   sprintf(smsg, "%s%d", txt, var);
   float dsx = draw_slider(x1, y1, x2, y2, q0, q1, q2, q3, q4, q5, q6, q7, sdx, sul, sll, 1);

   // is mouse on adjustment bar?
   if ((!q7) && (mI.mouse_x > dsx-bw) && (mI.mouse_x < dsx+bw) && (mI.mouse_y > y1) && (mI.mouse_y < y2))
   {
      draw_slider_bar(sdx, sul, sll, x1+bw+1, y1, x2-bw-1, y2, 2, q3); // draw highlighted bar
      draw_slider_text(x1, y1, x2, y2, q2, q5);
      if (mI.mouse_b[3][0])
      {
         while (mI.mouse_b[3][0])
         {
            var = (int)get_slider_position3((float) var, sul, sll, sinc, q4, x1, y1, x2, y2);
            sprintf(smsg, "%s%d", txt, var);
            draw_slider(x1, y1, x2, y2, q0, q1, q2, q3, q4, q5, q6, q7, (float)var, sul, sll, 2);
         }
      }
      if (mI.mouse_b[1][0])
      {
         while (mI.mouse_b[1][0])
         {
            var = (int) get_slider_position2(sul, sll, sinc, q4, x1, y1, x2, y2);
            sprintf(smsg, "%s%d", txt, var);
            draw_slider(x1, y1, x2, y2, q0, q1, q2, q3, q4, q5, q6, q7, (float) var, sul, sll, 2);
         }
      }
   }
   if (q6 == 1) y1+=bts;
}

// float version
void mdw_sliderf(int x1, int &y1, int x2, int bts, int bn, int num, int type, int obt, int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7,
                 float &var, float sul, float sll, float sinc, const char *txt)
{
   int y2 = y1+bts-2;
   float sdx = var;

   sprintf(smsg, "%s%3.2f", txt, var);
   float dsx = draw_slider(x1, y1, x2, y2, q0, q1, q2, q3, q4, q5, q6, q7, sdx, sul, sll, 1);

   // is mouse on adjustment bar?
   if ((!q7) && (mI.mouse_x > dsx-bw) && (mI.mouse_x < dsx+bw) && (mI.mouse_y > y1) && (mI.mouse_y < y2))
   {
      draw_slider_bar(sdx, sul, sll, x1+bw+1, y1, x2-bw-1, y2, 2, q3); // draw highlighted bar
      al_draw_text(mF.pr8, mC.pc[q2], (x2+x1)/2, (y2+y1)/2-4, ALLEGRO_ALIGN_CENTER, smsg);

      if (mI.mouse_b[3][0])
      {
         while (mI.mouse_b[3][0])
         {
            var = get_slider_position3(var, sul, sll, sinc, q4, x1, y1, x2, y2);
            sprintf(smsg, "%s%3.2f", txt, var);
            draw_slider(x1, y1, x2, y2, q0, q1, q2, q3, q4, q5, q6, q7, var, sul, sll, 2);
            if (bn == 1) mwD.scale_factor_current = mwD.scale_factor;
         }
      }
      if (mI.mouse_b[1][0])
      {
         while (mI.mouse_b[1][0])
         {
            var = get_slider_position2(sul, sll, sinc, q4, x1, y1, x2, y2);
            sprintf(smsg, "%s%3.2f", txt, var);
            draw_slider(x1, y1, x2, y2, q0, q1, q2, q3, q4, q5, q6, q7, var, sul, sll, 1);
            if (bn == 1) mwD.scale_factor_current = mwD.scale_factor;
         }
      }
   }
   if (q6 == 1) y1+=bts;
}



























































































































































void set_trigger_event(int i, int ev0, int ev1, int ev2, int ev3)
{
   if (item[i][0] == 6) // orb
   {
      item[i][10] = ev0;
      item[i][11] = ev1;
      item[i][12] = ev2;
      item[i][13] = ev3;
   }
   if (item[i][0] == 9) // trigger
   {
      item[i][11] = ev0;
      item[i][12] = ev1;
      item[i][13] = ev2;
      item[i][14] = ev3;
   }
}




int get_frame_size(int num)
{
   if (item[num][2] & PM_ITEM_PMSG_FRAME0) return 0;
   if (item[num][2] & PM_ITEM_PMSG_FRAME1) return 1;
   if (item[num][2] & PM_ITEM_PMSG_FRAME2) return 2;
   if (item[num][2] & PM_ITEM_PMSG_FRAME4) return 4;
   if (item[num][2] & PM_ITEM_PMSG_FRAME12) return 12;
   return 0;
}


void set_frame_size(int num, int frame_size)
{
   // clear all flags
   item[num][2] &= ~PM_ITEM_PMSG_FRAME0;
   item[num][2] &= ~PM_ITEM_PMSG_FRAME1;
   item[num][2] &= ~PM_ITEM_PMSG_FRAME2;
   item[num][2] &= ~PM_ITEM_PMSG_FRAME4;
   item[num][2] &= ~PM_ITEM_PMSG_FRAME12;

   if (frame_size == 0)  item[num][2] |= PM_ITEM_PMSG_FRAME0;
   if (frame_size == 1)  item[num][2] |= PM_ITEM_PMSG_FRAME1;
   if (frame_size == 2)  item[num][2] |= PM_ITEM_PMSG_FRAME2;
   if (frame_size == 4)  item[num][2] |= PM_ITEM_PMSG_FRAME4;
   if (frame_size == 12) item[num][2] |= PM_ITEM_PMSG_FRAME12;
}








// ------------------------------------------------------------------------------------
// --------------------------buttons---------------------------------------------------
// ------------------------------------------------------------------------------------

int mdw_button(int x1, int &y1, int x2, int bts,
                int bn, int num, int type, int obt,
                 int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7 )
{
   int y2 = y1+bts-2;

   // is mouse pressed on this button?
   int press = 0;
   int retval = 0;

   if ((!q7) && (mI.mouse_b[1][0]) && (mI.mouse_x > x1) && (mI.mouse_x < x2) && (mI.mouse_y > y1) && (mI.mouse_y < y2))
   {
      while (mI.mouse_b[1][0]) mwEQ.proc_event_queue(); // wait for release
      press = 1;
   }



   if (bn == 4)
   {
      if (item[num][8] == 0) sprintf(smsg, "disabled");
      if (item[num][8] == 1) sprintf(smsg, "Set Desination Item (%d)", item[num][9]);
      if (press)
      {
         if (item[num][8] == 1) // Set Linked Item
         {
             int i = get_item(2, 1, num );
             if (i > -1) item[num][9] = i;
         }
      }
   }

   if (bn == 6)
   {
      sprintf(smsg, "Set Message Area");
      if (press)
      {
         int x=0, y=0, w=0, h=0;
         get_block_range("Message Area", &x, &y, &w, &h, 1);
         set_int_3216(item[num][10], x, y);
         set_int_3216(item[num][11], w, h);

      }
   }

   if (bn == 7)
   {
      int frame_size = get_frame_size(num);

      sprintf(smsg, "Frame Size:%d", frame_size);
      if (press)
      {
              if (frame_size == 0)  frame_size = 1;
         else if (frame_size == 1)  frame_size = 2;
         else if (frame_size == 2)  frame_size = 4;
         else if (frame_size == 4)  frame_size = 12;
         else if (frame_size == 12) frame_size = 0;

         set_frame_size(num, frame_size);

      }
   }



   if (bn == 11)
   {
      sprintf(smsg, "Set Initial Direction");// trakbot direction
      if (press)
      {
         if (++Ei[num][5] > 7) Ei[num][5] = 0;
         set_trakbot_mode(num, Ei[num][5]);
      }
      if ((Ei[num][5] < 0) || (Ei[num][5] > 7)) Ei[num][5] = 0; // enforce limits
   }

   if (bn == 13)
   {
      sprintf(smsg, "Main Shape");
      if (press)
      {
         int main_ans = Ei[num][5];
         if (main_ans == 31) main_ans = 14;
         else
         {
            if (main_ans == 29) main_ans = 31;
            if (main_ans == 14) main_ans = 29;
         }
         Ei[num][5] = main_ans;
         Ei[num][3] = main_ans;
         Ei[num][1] = mwB.zz[5][main_ans];
      }
   }
   if (bn == 14)
   {
      sprintf(smsg, "Seek Shape");
      if (press)
      {
         int seek_ans = Ei[num][6];
         if (seek_ans == 31) seek_ans = 14;
         else
         {
            if (seek_ans == 29) seek_ans = 31;
            if (seek_ans == 14) seek_ans = 29;
         }
         Ei[num][6] = seek_ans;
      }
   }

   if (bn == 49) // door type
   {
      if (press)
      {
         item[num][8] = !item[num][8];
         // check for bad link
         int link = item[num][9];
         if (item[link][0] != 1) // link is not door
         {
            item[num][9] = num;  // link to self
            item[num][11] = 1;   // trigger with up
         }
      }
      if (item[num][8] == 0) sprintf(smsg, "Door Type:Exit Only");
      if (item[num][8] == 1) sprintf(smsg, "Door Type:Normal   ");
   }


   if (bn == 52)
   {
      sprintf(smsg, "Change Door Shape");
      if (press)
      {
         int shape = item[num][13];
         if (shape == 448) shape = 1083;
         else if (shape == 1083) shape = 0;
         else if (shape == 0) shape = 448;

         if ((shape != 448)  && (shape != 1083) && (shape != 0)) shape = 1083;
//         if ((shape != 448) && (shape != 1083)) shape = 1083;

         item[num][13] = shape;
         item[num][1] = shape;

//         if (shape != -1) change_linked_door_color_and_shape(num);
      }
   }


   if (bn == 57)
   {
      int o = mwWM.mW[7].obt;
      int n = mwWM.mW[7].num;
      int t = 0;
      sprintf(smsg,"?? Help");

      if (o == 2)
      {
         t = item[n][0];
         sprintf(smsg,"%s Help", item_name[t]);
      }
      if (o == 3)
      {
         t = Ei[n][0];
         sprintf(smsg,"%s Help", (const char *)enemy_name[t][0]);
      }
      if (o == 4) sprintf(smsg,"Lift Help");

      if (press)
      {
         if (o==3)
         {
            if (t == 3)  mHelp.help("Archwagon Viewer");
            if (t == 4)  mHelp.help("Bouncer Viewer");
            if (t == 5)  mHelp.help("Jumpworm Viewer");
            if (t == 6)  mHelp.help("Cannon Viewer");
            if (t == 7)  mHelp.help("Podzilla Viewer");
            if (t == 8)  mHelp.help("Trakbot Viewer");
            if (t == 9)  mHelp.help("Cloner Viewer");
            if (t == 10) mHelp.help("Field Viewer");
            if (t == 11) mHelp.help("Block Walker Viewer");
            if (t == 12) mHelp.help("Flapper Viewer");
         }
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
            if (t == 14) mHelp.help("Switch Viewer");
            if (t == 15) mHelp.help("Sproingy Viewer");
            if (t == 16) mHelp.help("Block Manip Viewer");
            if (t == 17) mHelp.help("Block Damage Viewer");
         }
         if (o==4)
         {
            mHelp.help("Lift Viewer");
         }
      }
   }

   if (bn == 77)
   {
      if (item[num][12] == 0) sprintf(smsg, "Type:Fuse Timer");
      if (item[num][12] == 1) sprintf(smsg, "Type:Remote Detonator");
      if (press)
      {
         item[num][12] = !item[num][12];
         if (item[num][12]) item[num][1] = 537;
         else item[num][1] = 464;
      }
   }






   if (bn == 90) // orb trigger type
   {
      if (press)
      {
         if (item[num][2] & PM_ITEM_ORB_TRIG_TOUCH)
         {
            item[num][2] &= ~PM_ITEM_ORB_TRIG_TOUCH; // clear flag
            item[num][2] |= PM_ITEM_ORB_TRIG_UP; // set flag
         }
         else if (item[num][2] & PM_ITEM_ORB_TRIG_UP)
         {
            item[num][2] &= ~PM_ITEM_ORB_TRIG_UP; // clear flag
            item[num][2] |= PM_ITEM_ORB_TRIG_DOWN; // set flag
         }
         else if (item[num][2] & PM_ITEM_ORB_TRIG_DOWN)
         {
            item[num][2] &= ~PM_ITEM_ORB_TRIG_DOWN;  // clear flag
            item[num][2] |= PM_ITEM_ORB_TRIG_SHOT; // set flag
         }
         else if (item[num][2] & PM_ITEM_ORB_TRIG_SHOT)
         {
            item[num][2] &= ~PM_ITEM_ORB_TRIG_SHOT; // clear flag
            item[num][2] |= PM_ITEM_ORB_TRIG_TOUCH; // set flag
         }
      }

      sprintf(smsg, "undef");

      if (item[num][2] & PM_ITEM_ORB_TRIG_TOUCH)  sprintf(smsg, "Trigger:Touch");
      if (item[num][2] & PM_ITEM_ORB_TRIG_UP)     sprintf(smsg, "Trigger:Up");
      if (item[num][2] & PM_ITEM_ORB_TRIG_DOWN)   sprintf(smsg, "Trigger:Down");
      if (item[num][2] & PM_ITEM_ORB_TRIG_SHOT) sprintf(smsg, "Trigger:Bullet");
   }


   if (bn == 92) // orb mode
   {
      if (press) item[num][6]++;
      if ((item[num][6] < 0) || (item[num][6] > 4)) item[num][6] = 0;
      sprintf(smsg, "undef");
      if (item[num][6] == 0) sprintf(smsg, "Mode:Toggle");
      if (item[num][6] == 1)
      {
         sprintf(smsg, "Mode:Stick ON");
         item[num][2] &= ~PM_ITEM_ORB_STATE;
      }
      if (item[num][6] == 2)
      {
         sprintf(smsg, "Mode:Stick OFF");
         item[num][2] |= PM_ITEM_ORB_STATE;
      }
      if (item[num][6] == 3)
      {
         sprintf(smsg, "Mode:Timed ON");
         item[num][2] &= ~PM_ITEM_ORB_STATE;
      }
      if (item[num][6] == 4)
      {
         sprintf(smsg, "Mode:Timed OFF");
         item[num][2] |= PM_ITEM_ORB_STATE;
      }
   }

   if (bn == 94) // orb rotation
   {
      // get rb
      int rb = (item[num][2] & PM_ITEM_ORB_ROTB) >> 14;

      if (press) rb++;
      if ((rb < 0) || (rb > 3)) rb = 0;

      sprintf(smsg, "Change Rotation");

      // set rb
      rb = rb << 14; // shift bits into place
      item[num][2] &= ~PM_ITEM_ORB_ROTB; // clear bits in target
      item[num][2] |= rb; // merge


   }


   if (bn == 211) // Trigger Field X Lift Alignment
   {
      int C = item[num][3] & PM_ITEM_TRIGGER_LIFT_XC;
      int F = item[num][3] & PM_ITEM_TRIGGER_LIFT_XF;
      int L = item[num][3] & PM_ITEM_TRIGGER_LIFT_XL;
      if (C) sprintf(smsg, "Lift X Align:Center");
      else
      {
         if ((!F) && (!L)) sprintf(smsg, "Lift X Align: Field x1 = Lift x1");
         if ((!F) &&  (L)) sprintf(smsg, "Lift X Align: Field x1 = Lift x2");
         if ((F)  && (!L)) sprintf(smsg, "Lift X Align: Field x2 = Lift x1");
         if ((F)  &&  (L)) sprintf(smsg, "Lift X Align: Field x2 = Lift x2");
      }
      if (press)
      {
         int C = item[num][3] & PM_ITEM_TRIGGER_LIFT_XC;
         int F = item[num][3] & PM_ITEM_TRIGGER_LIFT_XF;
         int L = item[num][3] & PM_ITEM_TRIGGER_LIFT_XL;
         if (C)    // 1 X X
         {  // set to 0 0 0
            item[num][3] &= ~PM_ITEM_TRIGGER_LIFT_XC; // clear C flag
            item[num][3] &= ~PM_ITEM_TRIGGER_LIFT_XF; // clear F flag
            item[num][3] &= ~PM_ITEM_TRIGGER_LIFT_XL; // clear L flag
         }
         else if ((!F) && (!L)) // 0 0 0
         {               // set to 0 0 1
            item[num][3] &= ~PM_ITEM_TRIGGER_LIFT_XC; // clear C flag
            item[num][3] &= ~PM_ITEM_TRIGGER_LIFT_XF; // clear F flag
            item[num][3] |=  PM_ITEM_TRIGGER_LIFT_XL; // set   L flag
         }
         else if ((!F) && (L)) // 0 0 1
         {              // set to 0 1 0
            item[num][3] |=  PM_ITEM_TRIGGER_LIFT_XF; // set   F flag
            item[num][3] &= ~PM_ITEM_TRIGGER_LIFT_XL; // clear L flag
         }
         else if ((F) && (!L)) // 0 1 0
         {              // set to 0 1 1
            item[num][3] |=  PM_ITEM_TRIGGER_LIFT_XF; // set   F flag
            item[num][3] |=  PM_ITEM_TRIGGER_LIFT_XL; // set   L flag
         }
         else if ((F) && (L))  // 0 1 1
         {              // set to 1 0 0
            item[num][3] |=  PM_ITEM_TRIGGER_LIFT_XC; // set   C flag
            item[num][3] &= ~PM_ITEM_TRIGGER_LIFT_XL; // clear L flag
            item[num][3] &= ~PM_ITEM_TRIGGER_LIFT_XF; // clear F flag
         }
      }
   }
   if (bn == 212) // Trigger Field Y Lift Alignment
   {
      int C = item[num][3] & PM_ITEM_TRIGGER_LIFT_YC;
      int F = item[num][3] & PM_ITEM_TRIGGER_LIFT_YF;
      int L = item[num][3] & PM_ITEM_TRIGGER_LIFT_YL;
      if (C) sprintf(smsg, "Lift Y Align:Center");
      else
      {
         if ((!F) && (!L)) sprintf(smsg, "Lift Y Align: Field y1 = Lift y1");
         if ((!F) &&  (L)) sprintf(smsg, "Lift Y Align: Field y1 = Lift y2");
         if ((F)  && (!L)) sprintf(smsg, "Lift Y Align: Field y2 = Lift y1");
         if ((F)  &&  (L)) sprintf(smsg, "Lift Y Align: Field y2 = Lift y2");
      }
      if (press)
      {
         int C = item[num][3] & PM_ITEM_TRIGGER_LIFT_YC;
         int F = item[num][3] & PM_ITEM_TRIGGER_LIFT_YF;
         int L = item[num][3] & PM_ITEM_TRIGGER_LIFT_YL;

         if (C)    // 1 X X
         {  // set to 0 0 0
            item[num][3] &= ~PM_ITEM_TRIGGER_LIFT_YC; // clear C flag
            item[num][3] &= ~PM_ITEM_TRIGGER_LIFT_YF; // clear F flag
            item[num][3] &= ~PM_ITEM_TRIGGER_LIFT_YL; // clear L flag
         }
         else if ((!F) && (!L)) // 0 0 0
         {               // set to 0 0 1
            item[num][3] &= ~PM_ITEM_TRIGGER_LIFT_YC; // clear C flag
            item[num][3] &= ~PM_ITEM_TRIGGER_LIFT_YF; // clear F flag
            item[num][3] |=  PM_ITEM_TRIGGER_LIFT_YL; // set   L flag
         }
         else if ((!F) && (L)) // 0 0 1
         {              // set to 0 1 0
            item[num][3] |=  PM_ITEM_TRIGGER_LIFT_YF; // set   F flag
            item[num][3] &= ~PM_ITEM_TRIGGER_LIFT_YL; // clear L flag
         }
         else if ((F) && (!L)) // 0 1 0
         {              // set to 0 1 1
            item[num][3] |=  PM_ITEM_TRIGGER_LIFT_YF; // set   F flag
            item[num][3] |=  PM_ITEM_TRIGGER_LIFT_YL; // set   L flag
         }
         else if ((F) && (L))  // 0 1 1
         {              // set to 1 0 0
            item[num][3] |=  PM_ITEM_TRIGGER_LIFT_YC; // set   C flag
            item[num][3] &= ~PM_ITEM_TRIGGER_LIFT_YL; // clear L flag
            item[num][3] &= ~PM_ITEM_TRIGGER_LIFT_YF; // clear F flag
         }
      }
   }



   if (bn == 310) // block 1 select...
   {
      int tn = item[num][10]&1023; // block 1
      sprintf(smsg, "Block 1: %d", tn);
      if (press) item[num][10] = select_bitmap(item[num][10]);
   }
   if (bn == 311) // block 2 select...
   {
      int tn = item[num][11]&1023; // block 2
      sprintf(smsg, "Block 2: %d", tn);
      if (press) item[num][11] = select_bitmap(item[num][11]);
   }
   if (bn == 320)
   {
      sprintf(smsg, "Set Event Trigger (%d)", item[num][1]);
      if (press)
      {
         int i = get_trigger_item(2, item[num][0], num );
         if (i > -1)
         {
            int ev = mwPME.get_unused_pm_event();
            item[num][1] = ev;

            if (item[num][0] == 16) // block manip
            {
               if (item[num][3] == 3) set_trigger_event(i, 0, 0, ev, 0); // mode 3 - toggle blocks - needs a toggle ON trigger
               else                   set_trigger_event(i, ev, 0, 0, 0); // needs a regular ON trigger
            }
            if (item[num][0] == 17) // block damage
            {
               if (item[num][11] == 1) set_trigger_event(i, 0, 0, ev, 0); // mode 1 - toggle damage - needs a toggle ON trigger
               else                    set_trigger_event(i, ev, 0, 0, 0); // needs a regular ON trigger
            }
         }
      }
   }

   if (bn == 401) // timer draw mode
   {
                                                 sprintf(smsg, "Timer Display: OFF          ");
      if (item[num][3] & PM_ITEM_DAMAGE_TIMR_SN) sprintf(smsg, "Timer Display: Small Number ");
      if (item[num][3] & PM_ITEM_DAMAGE_TIMR_BN) sprintf(smsg, "Timer Display: Large Number ");
      if (item[num][3] & PM_ITEM_DAMAGE_TIMR_SP) sprintf(smsg, "Timer Display: Small Percent");
      if (item[num][3] & PM_ITEM_DAMAGE_TIMR_BP) sprintf(smsg, "Timer Display: Large Percent");
      if (press)
      {
         if (item[num][3] & PM_ITEM_DAMAGE_TIMR_SN)
         {
            item[num][3] &= ~PM_ITEM_DAMAGE_TIMR_SN; // clear
            item[num][3] |=  PM_ITEM_DAMAGE_TIMR_BN; // set
         }
         else if (item[num][3] & PM_ITEM_DAMAGE_TIMR_BN)
         {
            item[num][3] &= ~PM_ITEM_DAMAGE_TIMR_BN; // clear
            item[num][3] |=  PM_ITEM_DAMAGE_TIMR_SP; // set
         }
         else if (item[num][3] & PM_ITEM_DAMAGE_TIMR_SP)
         {
            item[num][3] &= ~PM_ITEM_DAMAGE_TIMR_SP; // clear
            item[num][3] |=  PM_ITEM_DAMAGE_TIMR_BP; // set
         }
         else if (item[num][3] & PM_ITEM_DAMAGE_TIMR_BP)
         {
            item[num][3] &= ~PM_ITEM_DAMAGE_TIMR_SN; // clear all
            item[num][3] &= ~PM_ITEM_DAMAGE_TIMR_BN; // clear all
            item[num][3] &= ~PM_ITEM_DAMAGE_TIMR_SP; // clear all
            item[num][3] &= ~PM_ITEM_DAMAGE_TIMR_BP; // clear all
         }
         else if ( (!(item[num][3] & PM_ITEM_DAMAGE_TIMR_SN))
                && (!(item[num][3] & PM_ITEM_DAMAGE_TIMR_BN))
                && (!(item[num][3] & PM_ITEM_DAMAGE_TIMR_SP))
                && (!(item[num][3] & PM_ITEM_DAMAGE_TIMR_BP)) ) // all clear
         {
            item[num][3] |=  PM_ITEM_DAMAGE_TIMR_SN; // set
         }
      }
   }





   if (bn == 411) // DAMAGE Field X Lift Alignment
   {
      int C = item[num][3] & PM_ITEM_DAMAGE_LIFT_XC;
      int F = item[num][3] & PM_ITEM_DAMAGE_LIFT_XF;
      int L = item[num][3] & PM_ITEM_DAMAGE_LIFT_XL;

      if (C) sprintf(smsg, "Lift X Align:Center");
      else
      {
         if ((!F) && (!L)) sprintf(smsg, "Lift X Align: Field x1 = Lift x1");
         if ((!F) &&  (L)) sprintf(smsg, "Lift X Align: Field x1 = Lift x2");
         if ((F)  && (!L)) sprintf(smsg, "Lift X Align: Field x2 = Lift x1");
         if ((F)  &&  (L)) sprintf(smsg, "Lift X Align: Field x2 = Lift x2");
      }
      if (press)
      {
         int C = item[num][3] & PM_ITEM_DAMAGE_LIFT_XC;
         int F = item[num][3] & PM_ITEM_DAMAGE_LIFT_XF;
         int L = item[num][3] & PM_ITEM_DAMAGE_LIFT_XL;


         if (C)    // 1 X X
         {  // set to 0 0 0
            item[num][3] &= ~PM_ITEM_DAMAGE_LIFT_XC; // clear C flag
            item[num][3] &= ~PM_ITEM_DAMAGE_LIFT_XF; // clear F flag
            item[num][3] &= ~PM_ITEM_DAMAGE_LIFT_XL; // clear L flag
         }
         else if ((!F) && (!L)) // 0 0 0
         {               // set to 0 0 1
            item[num][3] &= ~PM_ITEM_DAMAGE_LIFT_XC; // clear C flag
            item[num][3] &= ~PM_ITEM_DAMAGE_LIFT_XF; // clear F flag
            item[num][3] |=  PM_ITEM_DAMAGE_LIFT_XL; // set   L flag
         }
         else if ((!F) && (L)) // 0 0 1
         {              // set to 0 1 0
            item[num][3] |=  PM_ITEM_DAMAGE_LIFT_XF; // set   F flag
            item[num][3] &= ~PM_ITEM_DAMAGE_LIFT_XL; // clear L flag
         }
         else if ((F) && (!L)) // 0 1 0
         {              // set to 0 1 1
            item[num][3] |=  PM_ITEM_DAMAGE_LIFT_XF; // set   F flag
            item[num][3] |=  PM_ITEM_DAMAGE_LIFT_XL; // set   L flag
         }
         else if ((F) && (L))  // 0 1 1
         {              // set to 1 0 0
            item[num][3] |=  PM_ITEM_DAMAGE_LIFT_XC; // set   C flag
            item[num][3] &= ~PM_ITEM_DAMAGE_LIFT_XL; // clear L flag
            item[num][3] &= ~PM_ITEM_DAMAGE_LIFT_XF; // clear F flag
         }
      }
   }
   if (bn == 412) // DAMAGE Field Y Lift Alignment
   {
      int C = item[num][3] & PM_ITEM_DAMAGE_LIFT_YC;
      int F = item[num][3] & PM_ITEM_DAMAGE_LIFT_YF;
      int L = item[num][3] & PM_ITEM_DAMAGE_LIFT_YL;

      if (C) sprintf(smsg, "Lift Y Align:Center");
      else
      {
         if ((!F) && (!L)) sprintf(smsg, "Lift Y Align: Field y1 = Lift y1");
         if ((!F) &&  (L)) sprintf(smsg, "Lift Y Align: Field y1 = Lift y2");
         if ((F)  && (!L)) sprintf(smsg, "Lift Y Align: Field y2 = Lift y1");
         if ((F)  &&  (L)) sprintf(smsg, "Lift Y Align: Field y2 = Lift y2");
      }
      if (press)
      {
         int C = item[num][3] & PM_ITEM_DAMAGE_LIFT_YC;
         int F = item[num][3] & PM_ITEM_DAMAGE_LIFT_YF;
         int L = item[num][3] & PM_ITEM_DAMAGE_LIFT_YL;

         if (C)    // 1 X X
         {  // set to 0 0 0
            item[num][3] &= ~PM_ITEM_DAMAGE_LIFT_YC; // clear C flag
            item[num][3] &= ~PM_ITEM_DAMAGE_LIFT_YF; // clear F flag
            item[num][3] &= ~PM_ITEM_DAMAGE_LIFT_YL; // clear L flag
         }
         else if ((!F) && (!L)) // 0 0 0
         {               // set to 0 0 1
            item[num][3] &= ~PM_ITEM_DAMAGE_LIFT_YC; // clear C flag
            item[num][3] &= ~PM_ITEM_DAMAGE_LIFT_YF; // clear F flag
            item[num][3] |=  PM_ITEM_DAMAGE_LIFT_YL; // set   L flag
         }
         else if ((!F) && (L)) // 0 0 1
         {              // set to 0 1 0
            item[num][3] |=  PM_ITEM_DAMAGE_LIFT_YF; // set   F flag
            item[num][3] &= ~PM_ITEM_DAMAGE_LIFT_YL; // clear L flag
         }
         else if ((F) && (!L)) // 0 1 0
         {              // set to 0 1 1
            item[num][3] |=  PM_ITEM_DAMAGE_LIFT_YF; // set   F flag
            item[num][3] |=  PM_ITEM_DAMAGE_LIFT_YL; // set   L flag
         }
         else if ((F) && (L))  // 0 1 1
         {              // set to 1 0 0
            item[num][3] |=  PM_ITEM_DAMAGE_LIFT_YC; // set   C flag
            item[num][3] &= ~PM_ITEM_DAMAGE_LIFT_YL; // clear L flag
            item[num][3] &= ~PM_ITEM_DAMAGE_LIFT_YF; // clear F flag
         }
      }
   }



   if (bn == 501)
   {
      if (num == -1) sprintf(smsg, "#");       // show row header
      else           sprintf(smsg, "%d", num); // show step num
   }
   if (bn == 502)
   {
      if (num == -1) sprintf(smsg, "Type");  // show row header
      if (num == 0)  sprintf(smsg, "----");
      if (num == 1)  sprintf(smsg, "Move");
      if (num == 2)  sprintf(smsg, "Wait");
      if (num == 3)  sprintf(smsg, "Wait");
      if (num == 4)  sprintf(smsg, "End ");
      if (num == 5)  sprintf(smsg, "Wait");
   }
   if (bn == 503)
   {
      int l = type;
      int s = obt;
      int v = Lift.stp[l][s].val;

      if (num == -1) sprintf(smsg, "Details");  // show row header
      if (num == 0)  sprintf(smsg, "blank");
      if (num == 1)  sprintf(smsg, "and Resize [speed:%d]", v);
      if (num == 2)  sprintf(smsg, "for Timer:%d", v);
      if (num == 3)  sprintf(smsg, "for Player prox:%d", v);
      if (num == 5)  sprintf(smsg, "for Trigger Event:%d", v);
      if (num == 4)
      {
         sprintf(smsg, "Step - undefined val");
         if (v == 0) sprintf(smsg, "Step - Loop to Start");
         if (v == 1) sprintf(smsg, "Step - Warp to Start");
         if (v == 2) sprintf(smsg, "Step - Freeze Here");
      }
   }
   if (bn == 504)
   {
      sprintf(smsg, "Name:%s", Lift.cur[num].lift_name); // edit lift name
      if (press) return 1;
   }
   if (bn == 505) // lift step end step mode
   {
      sprintf(smsg, "Undefined value");
      if (Lift.stp[num][type].val == 0) sprintf(smsg, "Loop to Start");
      if (Lift.stp[num][type].val == 1) sprintf(smsg, "Warp to Start");
      if (Lift.stp[num][type].val == 2) sprintf(smsg, "Freeze Here  ");
      if (press) if (++Lift.stp[num][type].val > 2) Lift.stp[num][type].val = 0; // lift step end step mode
   }
   if (bn == 506)
   {
      if (num == -1) sprintf(smsg, "C");       // show row header
      else           sprintf(smsg, "%d", num); // show num
   }

   if (bn == 520)
   {
      sprintf(smsg, "Set Event Trigger (%d)", Lift.stp[num][type].val);
      if (press)
      {
         //printf("520 type:%d num:%d \n", type, num);
         int i = get_trigger_item(4, type, num );
         if (i > -1)
         {
            int ev = mwPME.get_unused_pm_event();
            Lift.stp[num][type].val = ev;
            set_trigger_event(i, 0, 0, ev, 0); // toggle ON trigger
         }
      }
   }

   draw_slider_frame(x1, y1, x2, y2, q0, q1, q2, q3, q4, q5, q6, q7); // draw button frame
   draw_slider_text(x1, y1,  x2, y2, q2, q5);

   // special cases that need bitmaps draw on them
   if (bn == 13)
   {
      float rot = Ef[num][14];
      al_draw_rotated_bitmap(mwB.tile[mwB.zz[0][Ei[num][5]]], 10, 10, (x2+x1)/2+60, (y2+y1)/2, rot, 0);
   }
   if (bn == 14)
   {
      float rot = Ef[num][14];
      al_draw_rotated_bitmap(mwB.tile[mwB.zz[0][Ei[num][6]]], 10, 10, (x2+x1)/2+60, (y2+y1)/2, rot, 0);
   }

   if (bn == 310)
   {
      int tn = item[num][10];
      int x = (x2+x1)/2+60;
      int y = (y2+y1)/2-10;

      al_draw_filled_rectangle(x-1, y-1, x+21, y+21, mC.pc[0]);
      al_draw_bitmap(mwB.btile[tn&1023], x, y, 0);
   }

   if (bn == 311)
   {
      int tn = item[num][11];
      int x = (x2+x1)/2+60;
      int y = (y2+y1)/2-10;

      al_draw_filled_rectangle(x-1, y-1, x+21, y+21, mC.pc[0]);
      al_draw_bitmap(mwB.btile[tn&1023], x, y, 0);
   }
   if (q6) y1+=bts;
   return retval;
}




// displays a text string, and returns 1 if pressed
int mdw_buttont(int x1, int &y1, int x2, int bts, int bn, int num, int type, int obt, int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7, const char* txt)
{
   int y2 = y1+bts-2;
   int ret = 0;
   sprintf(smsg, "%s", txt);

   draw_slider_frame(x1, y1, x2, y2, q0, q1, q2, q3, q4, q5, q6, q7); // draw button frame
   draw_slider_text(x1, y1,  x2, y2, q2, q5);

   if ((!q7) && (mI.mouse_b[1][0]) && (mI.mouse_x > x1) && (mI.mouse_x < x2) && (mI.mouse_y > y1) && (mI.mouse_y < y2))
   {
      while (mI.mouse_b[1][0]) mwEQ.proc_event_queue(); // wait for release
      ret = 1;
   }
   if (q6) y1+=bts;
   return ret;
}




// displays a text string, and returns 1 if pressed
// auto width based on text length x1 is the center
// highlight outline when moused over


int mdw_buttontca(int xc, int &y1, int xd, int bts, int bn, int num, int type, int obt, int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7, const char* txt)
{
   int y2 = y1+bts-2;
   int ret = 0;
   sprintf(smsg, "%s", txt);

   int tl = (strlen(txt)+1)*4;

   int x1 = xc-tl;
   int x2 = xc+tl;


   draw_slider_frame(x1, y1, x2, y2, q0, q1, q2, q3, q4, q5, q6, q7); // draw button frame
   draw_slider_text(x1, y1,  x2, y2, q2, q5);

   if ((mI.mouse_x > x1) && (mI.mouse_x < x2) && (mI.mouse_y > y1) && (mI.mouse_y < y2)) // mouse on button
   {
      // show highlight
      al_draw_rounded_rectangle(x1, y1, x2, y2, 1, 1, mC.pc[q3], 1);

      if ((!q7) && (mI.mouse_b[1][0]))
      {
         while (mI.mouse_b[1][0]) mwEQ.proc_event_queue(); // wait for release
         ret = 1;
      }



   }



   if ((!q7) && (mI.mouse_b[1][0]) && (mI.mouse_x > x1) && (mI.mouse_x < x2) && (mI.mouse_y > y1) && (mI.mouse_y < y2))
   {
      while (mI.mouse_b[1][0]) mwEQ.proc_event_queue(); // wait for release
      ret = 1;
   }
   if (q6) y1+=bts;
   return ret;
}
































// displays a text string, and returns 1 if pressed but doesnt block
int mdw_buttont_nb(int x1, int &y1, int x2, int bts, int bn, int num, int type, int obt, int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7, const char* txt)
{
   int y2 = y1+bts-2;
   int ret = 0;
   sprintf(smsg, "%s", txt);

   draw_slider_frame(x1, y1, x2, y2, q0, q1, q2, q3, q4, q5, q6, q7); // draw button frame
   draw_slider_text(x1, y1,  x2, y2, q2, q5);

   if ((!q7) && (mI.mouse_b[1][0]) && (mI.mouse_x > x1) && (mI.mouse_x < x2) && (mI.mouse_y > y1) && (mI.mouse_y < y2))
   {
//      while (mI.mouse_b[1][0]) mwEQ.proc_event_queue(); // wait for release
      ret = 1;
   }
   if (q6) y1+=bts;
   return ret;
}

















// displays a text string and tile, and returns 1 if pressed --- tile is bn
int mdw_buttontt(int x1, int &y1, int x2, int bts, int tn, int num, int type, int obt, int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7, const char* txt)
{
   int y2 = y1+bts-2;
   int ret = 0;
   sprintf(smsg, "%s", txt);

   draw_slider_frame(x1, y1, x2, y2, q0, q1, q2, q3, q4, q5, q6, q7); // draw button frame
   draw_slider_text(x1, y1,  x2, y2, q2, q5);

   // draw tile
   int x = x1+14;
   al_draw_filled_rectangle(x-1, y1+0, x+21, y1+22, mC.pc[0]);
   al_draw_bitmap(mwB.btile[tn], x, y1+1, 0);

   if ((!q7) && (mI.mouse_b[1][0]) && (mI.mouse_x > x1) && (mI.mouse_x < x2) && (mI.mouse_y > y1) && (mI.mouse_y < y2))
   {
      while (mI.mouse_b[1][0]) mwEQ.proc_event_queue(); // wait for release
      ret = 1;
   }
   if (q6) y1+=bts;
   return ret;
}







// increment passed pointer (int &var) and display different text for each value
void mdw_buttonp(int x1, int &y1, int x2, int bts, int bn, int num, int type, int obt, int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7, int &var)
{
   int y2 = y1+bts-2;
   int press = 0;
   if ((!q7) && (mI.mouse_b[1][0]) && (mI.mouse_x > x1) && (mI.mouse_x < x2) && (mI.mouse_y > y1) && (mI.mouse_y < y2))
   {
      while (mI.mouse_b[1][0]) mwEQ.proc_event_queue(); // wait for release
      press = 1;
   }


   if (bn == 20)
   {
      if (press) var++;
      if ((var < 0) || (var > 2)) var = 0;
      if (var == 0) sprintf(smsg,  "Hysteresis Mode:Static");
      if (var == 1) sprintf(smsg,  "Hysteresis Mode:Instant");
      if (var == 2) sprintf(smsg,  "Hysteresis Mode:Gradual");
   }







   if (bn == 21)
   {
      if (press) var++;
      if ((var < 0) || (var > 1)) var = 0;
      if (var == 0) sprintf(smsg,  "Stationary");
      if (var == 1) sprintf(smsg,  "Fall");
   }
   if (bn == 22)
   {
      if (press) var++;
      if ((var < -2) || (var > 1)) var = -2;
      if (var ==  1) sprintf(smsg, "Fall");
      if (var ==  0) sprintf(smsg, "Stationary");
      if (var == -1) sprintf(smsg, "Carry");
      if (var == -2) sprintf(smsg, "Carry Through Door");
   }
   if (bn == 23) // rocket only
   {
      if (press) var++;
      if ((var < -2) || (var > 1)) var = -2;
      if (var == -1) var = 0;
      if (var ==  1) sprintf(smsg,  "Fall");
      if (var ==  0) sprintf(smsg,  "Stationary");
      if (var == -2) sprintf(smsg,  "Ride Through Door");
   }
   if (bn == 27) // cloner trigger type
   {
      if (press) var++;
      if ((var < 0) || (var > 2)) var = 0;
      if (var == 0)
      {
         sprintf(smsg, "Trigger Type:Timer Runs  ");
         Ei[num][7] = Ei[num][6]; // set counter
      }

      if (var == 1)
      {
         sprintf(smsg, "Trigger Type:Timer Resets");
         Ei[num][7] = Ei[num][6]; // set counter
      }

      if (var == 2) sprintf(smsg, "Trigger Type:Immediate   ");
   }
   if (bn == 50) // door entry type
   {
      if (press) var++;
      if ((var < 0) || (var > 2)) var = 0;
      if (var == 0) sprintf(smsg, "Enter Immediate  ");
      if (var == 1) sprintf(smsg, "Enter with <up>  ");
      if (var == 2) sprintf(smsg, "Enter with <down>");
   }
   if (bn == 51) // door show dest line type
   {
      if (press) var++;
      if ((var < 0) || (var > 2)) var = 0;
      if (var == 0) sprintf(smsg, "Exit link:never show  ");
      if (var == 1) sprintf(smsg, "Exit link:alway show  ");
      if (var == 2) sprintf(smsg, "Exit link:when touched");
   }
   if (bn == 53) // door move type
   {
      if (press) var++;
      if ((var < 0) || (var > 2)) var = 0;
      if (var == 0) sprintf(smsg, "Move Type:Automatic    ");
      if (var == 1) sprintf(smsg, "Move Type:Force Instant");
      if (var == 2) sprintf(smsg, "Move Type:Force Move   ");
   }
   if (bn == 78)
   {
      if (press) var++;
      if ((var < 0) || (var > 2)) var = 0;
      if (var == 0) sprintf(smsg, "Start Mode:Default");
      if (var == 1) sprintf(smsg, "Team Start");
      if (var == 2) sprintf(smsg, "Checkpoint Common");
      if (var == 3) sprintf(smsg, "Checkpoint Individual");
   }
   if (bn == 79)
   {
      if (press) var++;
      if ((var < 0) || (var > 7)) var = 0;
      sprintf(smsg, "Start Index:%d", var);
   }
   if (bn == 81)
   {
      if (press) var++;
      if ((var < 0) || (var > 3)) var = 0;
      if (var == 0) sprintf(smsg, "Draw Boxes:off");
      if (var == 1) sprintf(smsg, "Draw Boxes:trigger only");
      if (var == 2) sprintf(smsg, "Draw Boxes:src/dst only");
      if (var == 3) sprintf(smsg, "Draw Boxes:all");
   }
   if (bn == 100)
   {
      if (press) var++;
      if ((var < 0) || (var > 2)) var = 0;
      if (var == 0) sprintf(smsg, "Action:Randomize");
      if (var == 1) sprintf(smsg, "Action:Step from Min to Max");
      if (var == 2) sprintf(smsg, "Action:Set all to Min");
   }
   if (bn == 101)
   {
      if (press) var++;
      if ((var < 1) || (var > 3)) var = 1;
      if (var == 1) sprintf(smsg, "Type: Health Bonus");
      if (var == 2) sprintf(smsg, "Type: Free Man");
      if (var == 3) sprintf(smsg, "Type: Purple Coin");
   }
   if (bn == 102)
   {
      if (press) var++;
      if ((var < 1039) || (var > 1042)) var = 1039;
      if (var == 1039) { sprintf(smsg, "Color:Red");    q1 = 10; }
      if (var == 1040) { sprintf(smsg, "Color:Green");  q1 = 11; }
      if (var == 1041) { sprintf(smsg, "Color:Blue");   q1 = 13; }
      if (var == 1042) { sprintf(smsg, "Color:Purple"); q1 = 8;  }
   }
   if (bn == 301) // block manip mode
   {
      if (press) var++;
      if ((var < 0) || (var > 3)) var = 0;
      if (var == 0) sprintf(smsg, "MODE:OFF");
      if (var == 1) sprintf(smsg, "MODE:Set All To Block 1");
      if (var == 2) sprintf(smsg, "MODE:Set All Block 2 To Block 1");
      if (var == 3) sprintf(smsg, "MODE:Toggle Block 2 To Block 1");
   }
   if (bn == 402) // damage mode
   {
      if (press) var++;
      if ((var < 0) || (var > 4)) var = 0;
      if (var == 0)
      {
         sprintf(smsg, "MODE:Always ON");
         item[num][3] |=  PM_ITEM_DAMAGE_CURR; // set damage on

      }

      if (var == 1) sprintf(smsg, "MODE:Toggle");
      if (var == 2)
      {
         sprintf(smsg, "MODE:ON Until Triggered");
         item[num][3] |=  PM_ITEM_DAMAGE_CURR; // set damage on
      }

      if (var == 3)
      {
         sprintf(smsg, "MODE:OFF Until Triggered");
         item[num][3] &=  ~PM_ITEM_DAMAGE_CURR; // set damage off
      }

      if (var == 4) sprintf(smsg, "MODE:Timed ON And OFF");
   }


   if (bn == 404) // Block Damage draw mode
   {
      if (press) var++;
      if ((var < 0) || (var > 2)) var = 0;
      if (var == 0) sprintf(smsg, "Draw Type:none         ");
      if (var == 1) sprintf(smsg, "Draw Type:Red Rectangle");
      if (var == 2) sprintf(smsg, "Draw Type:Spikey Floor ");
   }
   if (bn == 500) // lift mode
   {
      if (press) var++;
      if ((var < 0) || (var > 2)) var = 0;
      if (var == 0) sprintf(smsg, "Mode 0 - Normal");
      if (var == 1) sprintf(smsg, "Mode 1 - Prox Run and Reset");
      if (var == 2) sprintf(smsg, "Mode 2 - Prox Reset");
   }
   draw_slider_frame(x1, y1, x2, y2, q0, q1, q2, q3, q4, q5, q6, q7); // draw button frame
   draw_slider_text(x1, y1,  x2, y2, q2, q5);
   if (q6) y1+=bts;
}























































void mdw_colsel(int x1, int &y1, int x2, int bts, int bn, int num, int type, int obt, int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7 )
{
   int y2 = y1+bts-2;
   al_draw_filled_rectangle(x1, y1, x2, y2, mC.pc[0]); // erase

   // draw colors (1-15)
   float a = x2-x1; // range
   float b = a/15;  // color swatch width
   if (b<2) b = 2;  // min width
   for (int c=0; c<15; c++)
      al_draw_filled_rectangle((int)(x1+c*b), y1, (int)(b+x1+c*b), y2, mC.pc[(int)c+1]);

   if (bn == 2) sprintf(smsg, "Select Text Color");
   if (bn == 3) sprintf(smsg, "Select Frame Color");
   if (bn == 4) sprintf(smsg, "Select Lift Color");
   if (bn == 5) sprintf(smsg, "Select Door Color");
   if (bn == 6) sprintf(smsg, "Select Trigger Field Color");
   if (bn == 7) sprintf(smsg, "Select Block Manip Field Color");
   if (bn == 8) sprintf(smsg, "Select Lift Step Color");


   draw_slider_text(x1, y1,  x2, y2, q2, q5);

   // draw outline
   al_draw_rectangle(x1, y1, x2, y2, mC.pc[15], 1);

   // is mouse pressed on button?
   if ((!q7) && (mI.mouse_x > x1) && (mI.mouse_x < x2) && (mI.mouse_y > y1) && (mI.mouse_y < y2) && (mI.mouse_b[1][0]))
   {
      while (mI.mouse_b[1][0]) mwEQ.proc_event_queue();
      int color = (int)(1+(mI.mouse_x-x1)/b);

      if (bn == 2) // text color
      {
         int tc=0, fc = 0;
         get_int_3216(item[num][13], tc, fc);
         tc = color;
         set_int_3216(item[num][13], tc, fc);
      }

      if (bn == 3) // frame color
      {
         int tc=0, fc = 0;                  // text and frame colors
         get_int_3216(item[num][13], tc, fc);
         fc = color;
         set_int_3216(item[num][13], tc, fc);
      }
      if (bn == 4) Lift.cur[num].color = color; // lift color
      if (bn == 5)
      {
         item[num][6] = color;     // door color
         change_linked_door_color_and_shape(num);
      }
      if (bn == 6) item[num][2] = color;     // trigger color
      if (bn == 7) item[num][12] = color;    // block manip color


      if (bn == 8) // lift step color
      {
        // printf("n:%d t:%d c:%d\n",num, type, color);
         int cf = color << 28; // shift 4 bits of color into place
         Lift.stp[num][type].type &= 0b00001111111111111111111111111111; // clear old color
         Lift.stp[num][type].type |= cf; // merge color with type
      }
   }
   if (q6 == 1) y1+=bts;
}


// toggles the int and displays text, text color, and frame color based on value
int mdw_toggle(int x1, int &y1, int x2, int bts, int bn, int num, int type, int obt, int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7,
               int &var, const char* t0, const char* t1 , int text_col0, int text_col1, int frame_col0, int frame_col1)
{
   int y2 = y1+bts-2;
   int ret = 0;
   // is mouse pressed on this button?
   if ((mI.mouse_b[1][0]) && (mI.mouse_x > x1) && (mI.mouse_x < x2) && (mI.mouse_y > y1) && (mI.mouse_y < y2) && (!q7))
   {
      while (mI.mouse_b[1][0]) mwEQ.proc_event_queue(); // wait for release
      var = ! var;
   }
   if (var)
   {
       q1 = frame_col1;
       q2 = text_col1;
       sprintf(smsg, "%s", t1);
       ret = 1;
   }
   else
   {
      q1 = frame_col0;
      q2 = text_col0;
      sprintf(smsg, "%s", t0);
      ret = 0;
   }
   draw_slider_frame(x1, y1, x2, y2, q0, q1, q2, q3, q4, q5, q6, q7); // draw button frame
   draw_slider_text(x1, y1,  x2, y2, q2, q5);

   if (q6 == 1) y1+=bts;
   return ret;
}








// toggles the int and displays text, text color, and frame color based on value  -- check box style
int mdw_togglec(int x1, int &y1, int x2, int bts, int bn, int num, int type, int obt, int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7,
               int &var, const char* t, int text_col, int frame_col)
{
   int y2 = y1+bts-2;
   int ret = 0;

   // debug show mouse detection area
//   if ((mI.mouse_x > x1) && (mI.mouse_x < x2) && (mI.mouse_y > y1) && (mI.mouse_y < y2) && (!q7))
//      al_draw_rectangle(x1, y1, x2, y2, mC.pc[10], 1);


   // is mouse pressed on this button?
   if ((mI.mouse_b[1][0]) && (mI.mouse_x > x1) && (mI.mouse_x < x2) && (mI.mouse_y > y1) && (mI.mouse_y < y2) && (!q7))
   {
      while (mI.mouse_b[1][0]) mwEQ.proc_event_queue(); // wait for release
      var = ! var;
   }


   float my1 = y1;
   float my2 = y2;

   // get y center of button
   float myc = my1 + (my2-my1)/2;

   float rs = 6; // check box size
   float ry1 = myc - rs/2;
   float ry2 = ry1 + rs;
   float rx1 = x1 + 4;
   float rx2 = rx1 + rs;

   // text pos
   float mtx = rx2+6;
   float mty = myc-4;

   if (q1>0) al_draw_rectangle(x1, y1, x2, y2, mC.pc[q1], 1);


   if (var) al_draw_filled_rectangle(rx1, ry1, rx2, ry2, mC.pc[frame_col]);
   else     al_draw_rectangle(       rx1, ry1, rx2, ry2, mC.pc[frame_col], 1);

   al_draw_text(mF.pr8, mC.pc[text_col], mtx, mty, 0, t);

   if (var) ret = 1;
   else ret = 0;

   if (q6 == 1) y1+=bts;
   return ret;
}

































// toggle the flag and displays the corresponding string
// returns the value of the flag
int mdw_togglf(int x1, int &y1, int x2, int bts, int bn, int num, int type, int obt, int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7,
               int &var, int flag, const char* t0, const char* t1 , int text_col0, int text_col1, int frame_col0, int frame_col1)
{
   int ret = 0;
   int y2 = y1+bts-2;

   // is mouse pressed on this button?
   if ((!q7) && (mI.mouse_b[1][0]) && (mI.mouse_x > x1) && (mI.mouse_x < x2) && (mI.mouse_y > y1) && (mI.mouse_y < y2))
   {
      while (mI.mouse_b[1][0]) mwEQ.proc_event_queue(); // wait for release
      var ^= flag;
   }
   if (var & flag)
   {
       q1 = frame_col1;
       q2 = text_col1;
       sprintf(smsg, "%s", t1);
       ret = 1;
   }
   else
   {
      q1 = frame_col0;
      q2 = text_col0;
      sprintf(smsg, "%s", t0);
      ret = 0;
   }
   draw_slider_frame(x1, y1, x2, y2, q0, q1, q2, q3, q4, q5, q6, q7); // draw button frame
   draw_slider_text(x1, y1,  x2, y2, q2, q5);
   if (q6 == 1) y1+=bts;
   return ret;
}
