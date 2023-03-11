// mwWidgets.cpp

#include "pm.h"
#include "mwWidgets.h"
#include "mwWindow.h"
#include "mwWindowManager.h"
#include "mwDisplay.h"
#include "mwBitmapTools.h"
#include "mwFont.h"
#include "mwBitmap.h"
#include "mwLift.h"
#include "mwColor.h"
#include "mwTriggerEvent.h"
#include "mwInput.h"
#include "mwEventQueue.h"
#include "mwItems.h"
#include "mwEnemy.h"
#include "mwMiscFnx.h"
#include "mwHelp.h"




mwWidget mWidget;

#define SLIDER_BAR_WIDTH 3


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


20230226
82  slideri
30  slider0
40  sliderf
0   sliderd 0
35  button
144 buttont
9   buttont_nb
7   buttonca
9   buttontt
33  buttonp
45  toggle
34  togglef
44  togglecc
8   colsel
-------------
520


*/



// ------------------------------------------------------------------------------------
// ---------------------------sliders--------------------------------------------------
// ------------------------------------------------------------------------------------


void mwWidget::draw_slider_frame(int x1, int y1, int x2, int y2, int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7 )
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
         al_draw_rounded_rectangle(x1+c, y1+c, x2-c, y2-c, 1, 1, mColor.pc[col], 1);
      }
   }
}


void mwWidget::draw_slider_text(int x1, int y1, int x2, int y2, int q2, int q5, const char* msg)
{
   int xt = (x2+x1)/2;
   int yt = y1 + (y2-y1-8)/2;
   if (q5) al_draw_text(mFont.pr8, mColor.pc[q2], x1+4, yt, 0, msg);
   else    al_draw_text(mFont.pr8, mColor.pc[q2], xt, yt, ALLEGRO_ALIGN_CENTER, msg);
}

float mwWidget::get_slider_position(float sdx, float sul, float sll, int x1, int y1, int x2, int y2)
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


float mwWidget::get_slider_position2(float sul, float sll, float sinc, int q4 ,int x1, int y1, int x2, int y2)
{
   if (q4)
   {
      mwWM.redraw_level_editor_background();
      mwWM.cycle_windows(1);
   }
   else
   {
      mEventQueue.proc();
      al_flip_display();
   }


   float my = mInput.mouse_y;
   float mx = mInput.mouse_x;
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

float mwWidget::get_slider_position3(float f, float sul, float sll, float sinc, int q4, int x1, int y1, int x2, int y2)
{
   if (q4)
   {
      mwWM.redraw_level_editor_background();
      mwWM.cycle_windows(1);
   }
   else
   {
      mEventQueue.proc();
      al_flip_display();
   }
   if (mInput.mouse_dz)
   {
      int dif = mInput.mouse_dz;
      mInput.mouse_dz = 0;

      f += dif * sinc;                  // only allow increments of sinc
      if (f < sll) f = sll;             // limit check
      if (f > sul) f = sul;
      f = round(f/sinc) * sinc;         // round to sinc
   }
   return f;
}






float mwWidget::draw_slider_bar(float sdx, float sul, float sll, int x1, int y1, int x2, int y2, int dm, int col)
{
   float f = get_slider_position(sdx, sul, sll, x1, y1, x2, y2);
   int sx1 = (int)f - SLIDER_BAR_WIDTH;
   int sx2 = (int)f + SLIDER_BAR_WIDTH;
   // draw slider bar
   for (int i=0; i<SLIDER_BAR_WIDTH+1; i++)
      al_draw_rectangle(sx1+i, y1+i, sx2-i, y2-i, mColor.pc[col+192-(i*64)], 1);

   // draw rectangle around slider bar to show highlight
   if (dm == 2) al_draw_rectangle(sx1-1, y1, sx2+1, y2, mColor.pc[15], 1);
   return f;
}


float mwWidget::draw_slider(int x1, int y1, int x2, int y2, int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7, float sdx, float sul, float sll, int order, const char *msg)
{
   float dsx = 0;
   draw_slider_frame(x1, y1, x2, y2, q0, q1, q2, q3, q4, q5, q6, q7);
   if (order == 1)
   {
      dsx = draw_slider_bar(sdx, sul, sll, x1+SLIDER_BAR_WIDTH+1, y1, x2-SLIDER_BAR_WIDTH-1, y2, 1, q3);
      draw_slider_text(x1, y1, x2, y2, q2, q5, msg);
   }
   if (order == 2)
   {
      draw_slider_text(x1, y1, x2, y2, q2, q5, msg);
      dsx = draw_slider_bar(sdx, sul, sll, x1+SLIDER_BAR_WIDTH+1, y1, x2-SLIDER_BAR_WIDTH-1, y2, 1, q3);
   }
   return dsx;
}


void mwWidget::slider0(int x1, int &y1, int x2, int bts, int bn, int num, int type, int obt, int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7,
                 int &var, float sul, float sll, float sinc, const char *txt, const char *txt2)
{
   char msg[80];
   int y2 = y1+bts-2;
   float sdx = (float) var;
   if (var == 0) sprintf(msg, "%s%s", txt, txt2);
   else          sprintf(msg, "%s%d", txt, var);
   float dsx = draw_slider(x1, y1, x2, y2, q0, q1, q2, q3, q4, q5, q6, q7, sdx, sul, sll, 1, msg);


   if (!q7)
   {
      // is mouse on adjustment bar?
      if ((mInput.mouse_x > dsx-SLIDER_BAR_WIDTH) && (mInput.mouse_x < dsx+SLIDER_BAR_WIDTH) && (mInput.mouse_y > y1) && (mInput.mouse_y < y2))
      {
         draw_slider_bar(sdx, sul, sll, x1+SLIDER_BAR_WIDTH+1, y1, x2-SLIDER_BAR_WIDTH-1, y2, 2, q3); // draw highlighted bar
         al_draw_text(mFont.pr8, mColor.pc[q2], (x2+x1)/2, (y2+y1)/2-4, ALLEGRO_ALIGN_CENTER, msg);

         if (mInput.mouse_b[3][0]) // only when initially clicked
         {
            while (mInput.mouse_b[3][0])
            {
               var = (int)get_slider_position3((float) var, sul, sll, sinc, q4, x1, y1, x2, y2);
               if (var == 0) sprintf(msg, "%s%s", txt, txt2);
               else          sprintf(msg, "%s%d", txt, var);
               draw_slider(x1, y1, x2, y2, q0, q1, q2, q3, q4, q5, q6, q7, (float)var, sul, sll, 2, msg);
            }
         }
         if (mInput.mouse_b[1][0])
         {
            while (mInput.mouse_b[1][0])
            {
               var = (int) get_slider_position2(sul, sll, sinc, q4, x1, y1, x2, y2);
               if (var == 0) sprintf(msg, "%s%s", txt, txt2);
               else          sprintf(msg, "%s%d", txt, var);
               draw_slider(x1, y1, x2, y2, q0, q1, q2, q3, q4, q5, q6, q7, (float) var, sul, sll, 1, msg);
            }
         }
      }
      else // if not on adjustment bar, is mouse pressed anywhere else on this button?
      {
         if ((mInput.mouse_b[1][0]) && (mInput.mouse_x > x1) && (mInput.mouse_x < x2) && (mInput.mouse_y > y1) && (mInput.mouse_y < y2))
         {
            while (mInput.mouse_b[1][0])
            {
               var = (int) get_slider_position2(sul, sll, sinc, q4, x1, y1, x2, y2);
               if (var == 0) sprintf(msg, "%s%s", txt, txt2);
               else          sprintf(msg, "%s%d", txt, var);
               draw_slider(x1, y1, x2, y2, q0, q1, q2, q3, q4, q5, q6, q7, (float) var, sul, sll, 1, msg);
            }
         }
      }
   }

   if (q6 == 1) y1+=bts;
}


void mwWidget::slideri(int x1, int &y1, int x2, int bts, int bn, int num, int type, int obt, int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7,
                 int &var, float sul, float sll, float sinc, const char *txt)
{
   char msg[80];
   int y2 = y1+bts-2;
   float sdx = (float) var;

   sprintf(msg, "%s%d", txt, var);
   float dsx = draw_slider(x1, y1, x2, y2, q0, q1, q2, q3, q4, q5, q6, q7, sdx, sul, sll, 1, msg);

   // is mouse on adjustment bar?
   if ((!q7) && (mInput.mouse_x > dsx-SLIDER_BAR_WIDTH) && (mInput.mouse_x < dsx+SLIDER_BAR_WIDTH) && (mInput.mouse_y > y1) && (mInput.mouse_y < y2))
   {
      draw_slider_bar(sdx, sul, sll, x1+SLIDER_BAR_WIDTH+1, y1, x2-SLIDER_BAR_WIDTH-1, y2, 2, q3); // draw highlighted bar
      draw_slider_text(x1, y1, x2, y2, q2, q5, msg);
      if (mInput.mouse_b[3][0])
      {
         while (mInput.mouse_b[3][0])
         {
            var = (int)get_slider_position3((float) var, sul, sll, sinc, q4, x1, y1, x2, y2);
            sprintf(msg, "%s%d", txt, var);
            draw_slider(x1, y1, x2, y2, q0, q1, q2, q3, q4, q5, q6, q7, (float)var, sul, sll, 2, msg);
         }
      }
      if (mInput.mouse_b[1][0])
      {
         while (mInput.mouse_b[1][0])
         {
            var = (int) get_slider_position2(sul, sll, sinc, q4, x1, y1, x2, y2);
            sprintf(msg, "%s%d", txt, var);
            draw_slider(x1, y1, x2, y2, q0, q1, q2, q3, q4, q5, q6, q7, (float) var, sul, sll, 2, msg);
         }
      }
   }
   if (q6 == 1) y1+=bts;
}

// float version
void mwWidget::sliderf(int x1, int &y1, int x2, int bts, int bn, int num, int type, int obt, int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7,
                 float &var, float sul, float sll, float sinc, const char *txt)
{
   char msg[80];
   int y2 = y1+bts-2;
   float sdx = var;

   sprintf(msg, "%s%3.2f", txt, var);
   float dsx = draw_slider(x1, y1, x2, y2, q0, q1, q2, q3, q4, q5, q6, q7, sdx, sul, sll, 1, msg);

   // is mouse on adjustment bar?
   if ((!q7) && (mInput.mouse_x > dsx-SLIDER_BAR_WIDTH) && (mInput.mouse_x < dsx+SLIDER_BAR_WIDTH) && (mInput.mouse_y > y1) && (mInput.mouse_y < y2))
   {
      draw_slider_bar(sdx, sul, sll, x1+SLIDER_BAR_WIDTH+1, y1, x2-SLIDER_BAR_WIDTH-1, y2, 2, q3); // draw highlighted bar
      al_draw_text(mFont.pr8, mColor.pc[q2], (x2+x1)/2, (y2+y1)/2-4, ALLEGRO_ALIGN_CENTER, msg);

      if (mInput.mouse_b[3][0])
      {
         while (mInput.mouse_b[3][0])
         {
            var = get_slider_position3(var, sul, sll, sinc, q4, x1, y1, x2, y2);
            sprintf(msg, "%s%3.2f", txt, var);
            draw_slider(x1, y1, x2, y2, q0, q1, q2, q3, q4, q5, q6, q7, var, sul, sll, 2, msg);
            if (bn == 1) mDisplay.scale_factor_current = mDisplay.scale_factor;
         }
      }
      if (mInput.mouse_b[1][0])
      {
         while (mInput.mouse_b[1][0])
         {
            var = get_slider_position2(sul, sll, sinc, q4, x1, y1, x2, y2);
            sprintf(msg, "%s%3.2f", txt, var);
            draw_slider(x1, y1, x2, y2, q0, q1, q2, q3, q4, q5, q6, q7, var, sul, sll, 1, msg);
            if (bn == 1) mDisplay.scale_factor_current = mDisplay.scale_factor;
         }
      }
   }
   if (q6 == 1) y1+=bts;
}











// non blocking versions...used only on opacity for now

float mwWidget::get_slider_position2nb(float sul, float sll, float sinc, int q4 ,int x1, int y1, int x2, int y2)
{

   float my = mInput.mouse_y;
   float mx = mInput.mouse_x;
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

// float version
void mwWidget::sliderfnb(int x1, int &y1, int x2, int bts, int bn, int num, int type, int obt, int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7,
                 float &var, float sul, float sll, float sinc, const char *txt)
{
   char msg[80];
   int y2 = y1+bts-2;
   float sdx = var;

   sprintf(msg, "%s%3.2f", txt, var);
   draw_slider(x1, y1, x2, y2, q0, q1, q2, q3, q4, q5, q6, q7, sdx, sul, sll, 1, msg);

//   float dsx = draw_slider(x1, y1, x2, y2, q0, q1, q2, q3, q4, q5, q6, q7, sdx, sul, sll, 1, msg);
//   // is mouse on adjustment bar?
//   if ((!q7) && (mInput.mouse_x > dsx-SLIDER_BAR_WIDTH) && (mInput.mouse_x < dsx+SLIDER_BAR_WIDTH) && (mInput.mouse_y > y1) && (mInput.mouse_y < y2))

   if ((!q7) && (mInput.mouse_x > x1) && (mInput.mouse_x < x2) && (mInput.mouse_y > y1) && (mInput.mouse_y < y2))
   {
      draw_slider_bar(sdx, sul, sll, x1+SLIDER_BAR_WIDTH+1, y1, x2-SLIDER_BAR_WIDTH-1, y2, 2, q3); // draw highlighted bar
      al_draw_text(mFont.pr8, mColor.pc[q2], (x2+x1)/2, (y2+y1)/2-4, ALLEGRO_ALIGN_CENTER, msg);

      if (mInput.mouse_b[1][0])
      {
//         while (mInput.mouse_b[1][0])
         {
            var = get_slider_position2nb(sul, sll, sinc, q4, x1, y1, x2, y2);
            sprintf(msg, "%s%3.2f", txt, var);
            draw_slider(x1, y1, x2, y2, q0, q1, q2, q3, q4, q5, q6, q7, var, sul, sll, 1, msg);
            if (bn == 1) mDisplay.scale_factor_current = mDisplay.scale_factor;
         }
      }
   }
   if (q6 == 1) y1+=bts;
}































































































































































// ------------------------------------------------------------------------------------
// --------------------------buttons---------------------------------------------------
// ------------------------------------------------------------------------------------

int mwWidget::button(int x1, int &y1, int x2, int bts,
                int bn, int num, int type, int obt,
                 int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7 )
{
   char msg[80];
   int y2 = y1+bts-2;

   // is mouse pressed on this button?
   int press = 0;
   int retval = 0;

   if ((!q7) && (mInput.mouse_b[1][0]) && (mInput.mouse_x > x1) && (mInput.mouse_x < x2) && (mInput.mouse_y > y1) && (mInput.mouse_y < y2))
   {
      while (mInput.mouse_b[1][0]) mEventQueue.proc(); // wait for release
      press = 1;
   }



   if (bn == 4)
   {
      if (mItem.item[num][8] == 0) sprintf(msg, "disabled");
      if (mItem.item[num][8] == 1) sprintf(msg, "Set Desination Item (%d)", mItem.item[num][9]);
      if (press)
      {
         if (mItem.item[num][8] == 1) // Set Linked Item
         {
             int i = mMiscFnx.get_item(2, 1, num );
             if (i > -1) mItem.item[num][9] = i;
         }
      }
   }

   if (bn == 6)
   {
      sprintf(msg, "Set Message Area");
      if (press)
      {
         int x=0, y=0, w=0, h=0;
         mMiscFnx.get_block_range("Message Area", &x, &y, &w, &h, 1);
         mMiscFnx.set_int_3216(mItem.item[num][10], x, y);
         mMiscFnx.set_int_3216(mItem.item[num][11], w, h);

      }
   }

   if (bn == 7)
   {
      int frame_size = mItem.get_frame_size(num);

      sprintf(msg, "Frame Size:%d", frame_size);
      if (press)
      {
              if (frame_size == 0)  frame_size = 1;
         else if (frame_size == 1)  frame_size = 2;
         else if (frame_size == 2)  frame_size = 4;
         else if (frame_size == 4)  frame_size = 12;
         else if (frame_size == 12) frame_size = 0;

         mItem.set_frame_size(num, frame_size);

      }
   }



   if (bn == 11)
   {
      sprintf(msg, "Set Initial Direction");// trakbot direction
      if (press)
      {
         if (++mEnemy.Ei[num][5] > 7) mEnemy.Ei[num][5] = 0;
         mEnemy.set_trakbot_mode(num, mEnemy.Ei[num][5]);
      }
      if ((mEnemy.Ei[num][5] < 0) || (mEnemy.Ei[num][5] > 7)) mEnemy.Ei[num][5] = 0; // enforce limits
   }

   if (bn == 13)
   {
      sprintf(msg, "Main Shape");
      if (press)
      {
         int main_ans = mEnemy.Ei[num][5];
         if (main_ans == 31) main_ans = 14;
         else
         {
            if (main_ans == 29) main_ans = 31;
            if (main_ans == 14) main_ans = 29;
         }
         mEnemy.Ei[num][5] = main_ans;
         mEnemy.Ei[num][3] = main_ans;
         mEnemy.Ei[num][1] = mBitmap.zz[5][main_ans];
      }
   }
   if (bn == 14)
   {
      sprintf(msg, "Seek Shape");
      if (press)
      {
         int seek_ans = mEnemy.Ei[num][6];
         if (seek_ans == 31) seek_ans = 14;
         else
         {
            if (seek_ans == 29) seek_ans = 31;
            if (seek_ans == 14) seek_ans = 29;
         }
         mEnemy.Ei[num][6] = seek_ans;
      }
   }

   if (bn == 49) // door type
   {
      if (press)
      {
         mItem.item[num][8] = !mItem.item[num][8];
         // check for bad link
         int link = mItem.item[num][9];
         if (mItem.item[link][0] != 1) // link is not door
         {
            mItem.item[num][9] = num;  // link to self
            mItem.item[num][11] = 1;   // trigger with up
         }
      }
      if (mItem.item[num][8] == 0) sprintf(msg, "Door Type:Exit Only");
      if (mItem.item[num][8] == 1) sprintf(msg, "Door Type:Normal   ");
   }


   if (bn == 52)
   {
      sprintf(msg, "Change Door Shape");
      if (press)
      {
         int shape = mItem.item[num][13];
         if (shape == 448) shape = 1083;
         else if (shape == 1083) shape = 0;
         else if (shape == 0) shape = 448;

         if ((shape != 448)  && (shape != 1083) && (shape != 0)) shape = 1083;
//         if ((shape != 448) && (shape != 1083)) shape = 1083;

         mItem.item[num][13] = shape;
         mItem.item[num][1] = shape;

//         if (shape != -1) change_linked_door_color_and_shape(num);
      }
   }


   if (bn == 57)
   {
      int o = mwWM.mW[7].obt;
      int n = mwWM.mW[7].num;
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
      if (mItem.item[num][12] == 0) sprintf(msg, "Type:Fuse Timer");
      if (mItem.item[num][12] == 1) sprintf(msg, "Type:Remote Detonator");
      if (press)
      {
         mItem.item[num][12] = !mItem.item[num][12];
         if (mItem.item[num][12]) mItem.item[num][1] = 537;
         else mItem.item[num][1] = 464;
      }
   }






   if (bn == 90) // orb trigger type
   {
      if (press)
      {
         if (mItem.item[num][2] & PM_ITEM_ORB_TRIG_TOUCH)
         {
            mItem.item[num][2] &= ~PM_ITEM_ORB_TRIG_TOUCH; // clear flag
            mItem.item[num][2] |= PM_ITEM_ORB_TRIG_UP; // set flag
         }
         else if (mItem.item[num][2] & PM_ITEM_ORB_TRIG_UP)
         {
            mItem.item[num][2] &= ~PM_ITEM_ORB_TRIG_UP; // clear flag
            mItem.item[num][2] |= PM_ITEM_ORB_TRIG_DOWN; // set flag
         }
         else if (mItem.item[num][2] & PM_ITEM_ORB_TRIG_DOWN)
         {
            mItem.item[num][2] &= ~PM_ITEM_ORB_TRIG_DOWN;  // clear flag
            mItem.item[num][2] |= PM_ITEM_ORB_TRIG_SHOT; // set flag
         }
         else if (mItem.item[num][2] & PM_ITEM_ORB_TRIG_SHOT)
         {
            mItem.item[num][2] &= ~PM_ITEM_ORB_TRIG_SHOT; // clear flag
            mItem.item[num][2] |= PM_ITEM_ORB_TRIG_TOUCH; // set flag
         }
      }

      sprintf(msg, "undef");

      if (mItem.item[num][2] & PM_ITEM_ORB_TRIG_TOUCH) sprintf(msg, "Trigger:Touch");
      if (mItem.item[num][2] & PM_ITEM_ORB_TRIG_UP)    sprintf(msg, "Trigger:Up");
      if (mItem.item[num][2] & PM_ITEM_ORB_TRIG_DOWN)  sprintf(msg, "Trigger:Down");
      if (mItem.item[num][2] & PM_ITEM_ORB_TRIG_SHOT)  sprintf(msg, "Trigger:Bullet");
   }


   if (bn == 92) // orb mode
   {
      if (press) mItem.item[num][6]++;
      if ((mItem.item[num][6] < 0) || (mItem.item[num][6] > 4)) mItem.item[num][6] = 0;
      sprintf(msg, "undef");
      if (mItem.item[num][6] == 0) sprintf(msg, "Mode:Toggle");
      if (mItem.item[num][6] == 1)
      {
         sprintf(msg, "Mode:Stick ON");
         mItem.item[num][2] &= ~PM_ITEM_ORB_STATE;
      }
      if (mItem.item[num][6] == 2)
      {
         sprintf(msg, "Mode:Stick OFF");
         mItem.item[num][2] |= PM_ITEM_ORB_STATE;
      }
      if (mItem.item[num][6] == 3)
      {
         sprintf(msg, "Mode:Timed ON");
         mItem.item[num][2] &= ~PM_ITEM_ORB_STATE;
      }
      if (mItem.item[num][6] == 4)
      {
         sprintf(msg, "Mode:Timed OFF");
         mItem.item[num][2] |= PM_ITEM_ORB_STATE;
      }
   }

   if (bn == 94) // orb rotation
   {
      // get rb
      int rb = (mItem.item[num][2] & PM_ITEM_ORB_ROTB) >> 14;

      if (press) rb++;
      if ((rb < 0) || (rb > 3)) rb = 0;

      sprintf(msg, "Change Rotation");

      // set rb
      rb = rb << 14; // shift bits into place
      mItem.item[num][2] &= ~PM_ITEM_ORB_ROTB; // clear bits in target
      mItem.item[num][2] |= rb; // merge


   }


   if (bn == 211) // Trigger Field X Lift Alignment
   {
      int C = mItem.item[num][3] & PM_ITEM_TRIGGER_LIFT_XC;
      int F = mItem.item[num][3] & PM_ITEM_TRIGGER_LIFT_XF;
      int L = mItem.item[num][3] & PM_ITEM_TRIGGER_LIFT_XL;
      if (C) sprintf(msg, "Lift X Align:Center");
      else
      {
         if ((!F) && (!L)) sprintf(msg, "Lift X Align: Field x1 = Lift x1");
         if ((!F) &&  (L)) sprintf(msg, "Lift X Align: Field x1 = Lift x2");
         if ((F)  && (!L)) sprintf(msg, "Lift X Align: Field x2 = Lift x1");
         if ((F)  &&  (L)) sprintf(msg, "Lift X Align: Field x2 = Lift x2");
      }
      if (press)
      {
         int C = mItem.item[num][3] & PM_ITEM_TRIGGER_LIFT_XC;
         int F = mItem.item[num][3] & PM_ITEM_TRIGGER_LIFT_XF;
         int L = mItem.item[num][3] & PM_ITEM_TRIGGER_LIFT_XL;
         if (C)    // 1 X X
         {  // set to 0 0 0
            mItem.item[num][3] &= ~PM_ITEM_TRIGGER_LIFT_XC; // clear C flag
            mItem.item[num][3] &= ~PM_ITEM_TRIGGER_LIFT_XF; // clear F flag
            mItem.item[num][3] &= ~PM_ITEM_TRIGGER_LIFT_XL; // clear L flag
         }
         else if ((!F) && (!L)) // 0 0 0
         {               // set to 0 0 1
            mItem.item[num][3] &= ~PM_ITEM_TRIGGER_LIFT_XC; // clear C flag
            mItem.item[num][3] &= ~PM_ITEM_TRIGGER_LIFT_XF; // clear F flag
            mItem.item[num][3] |=  PM_ITEM_TRIGGER_LIFT_XL; // set   L flag
         }
         else if ((!F) && (L)) // 0 0 1
         {              // set to 0 1 0
            mItem.item[num][3] |=  PM_ITEM_TRIGGER_LIFT_XF; // set   F flag
            mItem.item[num][3] &= ~PM_ITEM_TRIGGER_LIFT_XL; // clear L flag
         }
         else if ((F) && (!L)) // 0 1 0
         {              // set to 0 1 1
            mItem.item[num][3] |=  PM_ITEM_TRIGGER_LIFT_XF; // set   F flag
            mItem.item[num][3] |=  PM_ITEM_TRIGGER_LIFT_XL; // set   L flag
         }
         else if ((F) && (L))  // 0 1 1
         {              // set to 1 0 0
            mItem.item[num][3] |=  PM_ITEM_TRIGGER_LIFT_XC; // set   C flag
            mItem.item[num][3] &= ~PM_ITEM_TRIGGER_LIFT_XL; // clear L flag
            mItem.item[num][3] &= ~PM_ITEM_TRIGGER_LIFT_XF; // clear F flag
         }
      }
   }
   if (bn == 212) // Trigger Field Y Lift Alignment
   {
      int C = mItem.item[num][3] & PM_ITEM_TRIGGER_LIFT_YC;
      int F = mItem.item[num][3] & PM_ITEM_TRIGGER_LIFT_YF;
      int L = mItem.item[num][3] & PM_ITEM_TRIGGER_LIFT_YL;
      if (C) sprintf(msg, "Lift Y Align:Center");
      else
      {
         if ((!F) && (!L)) sprintf(msg, "Lift Y Align: Field y1 = Lift y1");
         if ((!F) &&  (L)) sprintf(msg, "Lift Y Align: Field y1 = Lift y2");
         if ((F)  && (!L)) sprintf(msg, "Lift Y Align: Field y2 = Lift y1");
         if ((F)  &&  (L)) sprintf(msg, "Lift Y Align: Field y2 = Lift y2");
      }
      if (press)
      {
         int C = mItem.item[num][3] & PM_ITEM_TRIGGER_LIFT_YC;
         int F = mItem.item[num][3] & PM_ITEM_TRIGGER_LIFT_YF;
         int L = mItem.item[num][3] & PM_ITEM_TRIGGER_LIFT_YL;

         if (C)    // 1 X X
         {  // set to 0 0 0
            mItem.item[num][3] &= ~PM_ITEM_TRIGGER_LIFT_YC; // clear C flag
            mItem.item[num][3] &= ~PM_ITEM_TRIGGER_LIFT_YF; // clear F flag
            mItem.item[num][3] &= ~PM_ITEM_TRIGGER_LIFT_YL; // clear L flag
         }
         else if ((!F) && (!L)) // 0 0 0
         {               // set to 0 0 1
            mItem.item[num][3] &= ~PM_ITEM_TRIGGER_LIFT_YC; // clear C flag
            mItem.item[num][3] &= ~PM_ITEM_TRIGGER_LIFT_YF; // clear F flag
            mItem.item[num][3] |=  PM_ITEM_TRIGGER_LIFT_YL; // set   L flag
         }
         else if ((!F) && (L)) // 0 0 1
         {              // set to 0 1 0
            mItem.item[num][3] |=  PM_ITEM_TRIGGER_LIFT_YF; // set   F flag
            mItem.item[num][3] &= ~PM_ITEM_TRIGGER_LIFT_YL; // clear L flag
         }
         else if ((F) && (!L)) // 0 1 0
         {              // set to 0 1 1
            mItem.item[num][3] |=  PM_ITEM_TRIGGER_LIFT_YF; // set   F flag
            mItem.item[num][3] |=  PM_ITEM_TRIGGER_LIFT_YL; // set   L flag
         }
         else if ((F) && (L))  // 0 1 1
         {              // set to 1 0 0
            mItem.item[num][3] |=  PM_ITEM_TRIGGER_LIFT_YC; // set   C flag
            mItem.item[num][3] &= ~PM_ITEM_TRIGGER_LIFT_YL; // clear L flag
            mItem.item[num][3] &= ~PM_ITEM_TRIGGER_LIFT_YF; // clear F flag
         }
      }
   }



   if (bn == 310) // block 1 select...
   {
      int tn = mItem.item[num][10]&1023; // block 1
      sprintf(msg, "Block 1: %d", tn);
      if (press) mItem.item[num][10] = mBitmapTools.select_bitmap(mItem.item[num][10]);

      if ((!q7) && (mInput.mouse_x > x1) && (mInput.mouse_x < x2) && (mInput.mouse_y > y1) && (mInput.mouse_y < y2))
      {
         int mpow_jnk = 0;
         mBitmapTools.draw_flags(x2+6, y1, mItem.item[num][10], mpow_jnk, 0, 1, 1);
         while (mInput.CTRL())
         {
            mEventQueue.proc();
            al_flip_display();
            mBitmapTools.draw_flags(x2+6, y1, mItem.item[num][10], mpow_jnk, 0, 1, 1);
         }
      }

   }
   if (bn == 311) // block 2 select...
   {
      int tn = mItem.item[num][11]&1023; // block 2
      sprintf(msg, "Block 2: %d", tn);
      if (press) mItem.item[num][11] = mBitmapTools.select_bitmap(mItem.item[num][11]);

      if ((!q7) && (mInput.mouse_x > x1) && (mInput.mouse_x < x2) && (mInput.mouse_y > y1) && (mInput.mouse_y < y2))
      {
         int mpow_jnk = 0;
         mBitmapTools.draw_flags(x2+6, y1, mItem.item[num][11], mpow_jnk, 0, 1, 1);
         while (mInput.CTRL())
         {
            mEventQueue.proc();
            al_flip_display();
            mBitmapTools.draw_flags(x2+6, y1, mItem.item[num][11], mpow_jnk, 0, 1, 1);
         }



      }


   }

   if (bn == 401) // bd timer draw mode
   {
                                                       sprintf(msg, "Timer Display: OFF          ");
      if (mItem.item[num][3] & PM_ITEM_DAMAGE_TIMR_SN) sprintf(msg, "Timer Display: Small Number ");
      if (mItem.item[num][3] & PM_ITEM_DAMAGE_TIMR_BN) sprintf(msg, "Timer Display: Large Number ");
      if (mItem.item[num][3] & PM_ITEM_DAMAGE_TIMR_SP) sprintf(msg, "Timer Display: Small Percent");
      if (mItem.item[num][3] & PM_ITEM_DAMAGE_TIMR_BP) sprintf(msg, "Timer Display: Large Percent");
      if (press)
      {
         if (mItem.item[num][3] & PM_ITEM_DAMAGE_TIMR_SN)
         {
            mItem.item[num][3] &= ~PM_ITEM_DAMAGE_TIMR_SN; // clear
            mItem.item[num][3] |=  PM_ITEM_DAMAGE_TIMR_BN; // set
         }
         else if (mItem.item[num][3] & PM_ITEM_DAMAGE_TIMR_BN)
         {
            mItem.item[num][3] &= ~PM_ITEM_DAMAGE_TIMR_BN; // clear
            mItem.item[num][3] |=  PM_ITEM_DAMAGE_TIMR_SP; // set
         }
         else if (mItem.item[num][3] & PM_ITEM_DAMAGE_TIMR_SP)
         {
            mItem.item[num][3] &= ~PM_ITEM_DAMAGE_TIMR_SP; // clear
            mItem.item[num][3] |=  PM_ITEM_DAMAGE_TIMR_BP; // set
         }
         else if (mItem.item[num][3] & PM_ITEM_DAMAGE_TIMR_BP)
         {
            mItem.item[num][3] &= ~PM_ITEM_DAMAGE_TIMR_SN; // clear all
            mItem.item[num][3] &= ~PM_ITEM_DAMAGE_TIMR_BN; // clear all
            mItem.item[num][3] &= ~PM_ITEM_DAMAGE_TIMR_SP; // clear all
            mItem.item[num][3] &= ~PM_ITEM_DAMAGE_TIMR_BP; // clear all
         }
         else if ( (!(mItem.item[num][3] & PM_ITEM_DAMAGE_TIMR_SN))
                && (!(mItem.item[num][3] & PM_ITEM_DAMAGE_TIMR_BN))
                && (!(mItem.item[num][3] & PM_ITEM_DAMAGE_TIMR_SP))
                && (!(mItem.item[num][3] & PM_ITEM_DAMAGE_TIMR_BP)) ) // all clear
         {
            mItem.item[num][3] |=  PM_ITEM_DAMAGE_TIMR_SN; // set
         }
      }
   }





   if (bn == 411) // DAMAGE Field X Lift Alignment
   {
      int C = mItem.item[num][3] & PM_ITEM_DAMAGE_LIFT_XC;
      int F = mItem.item[num][3] & PM_ITEM_DAMAGE_LIFT_XF;
      int L = mItem.item[num][3] & PM_ITEM_DAMAGE_LIFT_XL;

      if (C) sprintf(msg, "Lift X Align:Center");
      else
      {
         if ((!F) && (!L)) sprintf(msg, "Lift X Align: Field x1 = Lift x1");
         if ((!F) &&  (L)) sprintf(msg, "Lift X Align: Field x1 = Lift x2");
         if ((F)  && (!L)) sprintf(msg, "Lift X Align: Field x2 = Lift x1");
         if ((F)  &&  (L)) sprintf(msg, "Lift X Align: Field x2 = Lift x2");
      }
      if (press)
      {
         int C = mItem.item[num][3] & PM_ITEM_DAMAGE_LIFT_XC;
         int F = mItem.item[num][3] & PM_ITEM_DAMAGE_LIFT_XF;
         int L = mItem.item[num][3] & PM_ITEM_DAMAGE_LIFT_XL;


         if (C)    // 1 X X
         {  // set to 0 0 0
            mItem.item[num][3] &= ~PM_ITEM_DAMAGE_LIFT_XC; // clear C flag
            mItem.item[num][3] &= ~PM_ITEM_DAMAGE_LIFT_XF; // clear F flag
            mItem.item[num][3] &= ~PM_ITEM_DAMAGE_LIFT_XL; // clear L flag
         }
         else if ((!F) && (!L)) // 0 0 0
         {               // set to 0 0 1
            mItem.item[num][3] &= ~PM_ITEM_DAMAGE_LIFT_XC; // clear C flag
            mItem.item[num][3] &= ~PM_ITEM_DAMAGE_LIFT_XF; // clear F flag
            mItem.item[num][3] |=  PM_ITEM_DAMAGE_LIFT_XL; // set   L flag
         }
         else if ((!F) && (L)) // 0 0 1
         {              // set to 0 1 0
            mItem.item[num][3] |=  PM_ITEM_DAMAGE_LIFT_XF; // set   F flag
            mItem.item[num][3] &= ~PM_ITEM_DAMAGE_LIFT_XL; // clear L flag
         }
         else if ((F) && (!L)) // 0 1 0
         {              // set to 0 1 1
            mItem.item[num][3] |=  PM_ITEM_DAMAGE_LIFT_XF; // set   F flag
            mItem.item[num][3] |=  PM_ITEM_DAMAGE_LIFT_XL; // set   L flag
         }
         else if ((F) && (L))  // 0 1 1
         {              // set to 1 0 0
            mItem.item[num][3] |=  PM_ITEM_DAMAGE_LIFT_XC; // set   C flag
            mItem.item[num][3] &= ~PM_ITEM_DAMAGE_LIFT_XL; // clear L flag
            mItem.item[num][3] &= ~PM_ITEM_DAMAGE_LIFT_XF; // clear F flag
         }
      }
   }
   if (bn == 412) // DAMAGE Field Y Lift Alignment
   {
      int C = mItem.item[num][3] & PM_ITEM_DAMAGE_LIFT_YC;
      int F = mItem.item[num][3] & PM_ITEM_DAMAGE_LIFT_YF;
      int L = mItem.item[num][3] & PM_ITEM_DAMAGE_LIFT_YL;

      if (C) sprintf(msg, "Lift Y Align:Center");
      else
      {
         if ((!F) && (!L)) sprintf(msg, "Lift Y Align: Field y1 = Lift y1");
         if ((!F) &&  (L)) sprintf(msg, "Lift Y Align: Field y1 = Lift y2");
         if ((F)  && (!L)) sprintf(msg, "Lift Y Align: Field y2 = Lift y1");
         if ((F)  &&  (L)) sprintf(msg, "Lift Y Align: Field y2 = Lift y2");
      }
      if (press)
      {
         int C = mItem.item[num][3] & PM_ITEM_DAMAGE_LIFT_YC;
         int F = mItem.item[num][3] & PM_ITEM_DAMAGE_LIFT_YF;
         int L = mItem.item[num][3] & PM_ITEM_DAMAGE_LIFT_YL;

         if (C)    // 1 X X
         {  // set to 0 0 0
            mItem.item[num][3] &= ~PM_ITEM_DAMAGE_LIFT_YC; // clear C flag
            mItem.item[num][3] &= ~PM_ITEM_DAMAGE_LIFT_YF; // clear F flag
            mItem.item[num][3] &= ~PM_ITEM_DAMAGE_LIFT_YL; // clear L flag
         }
         else if ((!F) && (!L)) // 0 0 0
         {               // set to 0 0 1
            mItem.item[num][3] &= ~PM_ITEM_DAMAGE_LIFT_YC; // clear C flag
            mItem.item[num][3] &= ~PM_ITEM_DAMAGE_LIFT_YF; // clear F flag
            mItem.item[num][3] |=  PM_ITEM_DAMAGE_LIFT_YL; // set   L flag
         }
         else if ((!F) && (L)) // 0 0 1
         {              // set to 0 1 0
            mItem.item[num][3] |=  PM_ITEM_DAMAGE_LIFT_YF; // set   F flag
            mItem.item[num][3] &= ~PM_ITEM_DAMAGE_LIFT_YL; // clear L flag
         }
         else if ((F) && (!L)) // 0 1 0
         {              // set to 0 1 1
            mItem.item[num][3] |=  PM_ITEM_DAMAGE_LIFT_YF; // set   F flag
            mItem.item[num][3] |=  PM_ITEM_DAMAGE_LIFT_YL; // set   L flag
         }
         else if ((F) && (L))  // 0 1 1
         {              // set to 1 0 0
            mItem.item[num][3] |=  PM_ITEM_DAMAGE_LIFT_YC; // set   C flag
            mItem.item[num][3] &= ~PM_ITEM_DAMAGE_LIFT_YL; // clear L flag
            mItem.item[num][3] &= ~PM_ITEM_DAMAGE_LIFT_YF; // clear F flag
         }
      }
   }



   if (bn == 501)
   {
      if (num == -1) sprintf(msg, "#");       // show row header
      else           sprintf(msg, "%d", num); // show step num
   }
   if (bn == 502)
   {
      if (num == -1) sprintf(msg, "Type");  // show row header
      if (num == 0)  sprintf(msg, "----");
      if (num == 1)  sprintf(msg, "Move");
      if (num == 2)  sprintf(msg, "Wait");
      if (num == 3)  sprintf(msg, "Wait");
      if (num == 4)  sprintf(msg, "End ");
      if (num == 5)  sprintf(msg, "Wait");
      if (num == 6)  sprintf(msg, "Send");
   }
   if (bn == 503)
   {
      int l = type;
      int s = obt;
      int v = mLift.stp[l][s].val;

      if (num == -1) sprintf(msg, "Details");  // show row header
      if (num == 0)  sprintf(msg, "blank");
      if (num == 1)  sprintf(msg, "and Resize [speed:%d]", v);
      if (num == 2)  sprintf(msg, "for Timer:%d", v);
      if (num == 3)  sprintf(msg, "for Player prox:%d", v);
      if (num == 5)  sprintf(msg, "for Event:%d", v);
      if (num == 6)  sprintf(msg, "Event:%d", v);
      if (num == 4)
      {
                     sprintf(msg, "Step - undefined val");
         if (v == 0) sprintf(msg, "Step - Loop to Start");
         if (v == 1) sprintf(msg, "Step - Warp to Start");
         if (v == 2) sprintf(msg, "Step - Freeze Here");
      }
   }
   if (bn == 504)
   {
      sprintf(msg, "Name:%s", mLift.cur[num].lift_name); // edit lift name
      if (press) return 1;
   }
   if (bn == 505) // lift step end step mode
   {
                                         sprintf(msg, "Undefined value");
      if (mLift.stp[num][type].val == 0) sprintf(msg, "Loop to Start");
      if (mLift.stp[num][type].val == 1) sprintf(msg, "Warp to Start");
      if (mLift.stp[num][type].val == 2) sprintf(msg, "Freeze Here  ");
      if (press) if (++mLift.stp[num][type].val > 2) mLift.stp[num][type].val = 0; // lift step end step mode
   }
   if (bn == 506)
   {
      if (num == -1) sprintf(msg, "C");       // show row header
      else           sprintf(msg, "%d", num); // show num
   }

   if (bn == 520)
   {
      sprintf(msg, "Set Event Trigger (%d)", mLift.stp[num][type].val);
      if (press)
      {
         //printf("520 type:%d num:%d \n", type, num);
         int ti_obj_type, ti_sub_type, ti_num;
         if (mTriggerEvent.get_trigger_item(4, num, type, ti_obj_type, ti_num, ti_sub_type))
         {
            int ev = mTriggerEvent.get_unused_pm_event();
            mLift.stp[num][type].val = ev;

//            if (ti_obj_type == 2)



            mItem.set_trigger_event(ti_num, 0, 0, ev, 0); // toggle ON trigger
         }
      }
   }

   draw_slider_frame(x1, y1, x2, y2, q0, q1, q2, q3, q4, q5, q6, q7); // draw button frame
   draw_slider_text(x1, y1,  x2, y2, q2, q5, msg);

   // special cases that need bitmaps draw on them
   if (bn == 13)
   {
      float rot = mEnemy.Ef[num][14];
      al_draw_rotated_bitmap(mBitmap.tile[mBitmap.zz[0][mEnemy.Ei[num][5]]], 10, 10, (x2+x1)/2+60, (y2+y1)/2, rot, 0);
   }
   if (bn == 14)
   {
      float rot = mEnemy.Ef[num][14];
      al_draw_rotated_bitmap(mBitmap.tile[mBitmap.zz[0][mEnemy.Ei[num][6]]], 10, 10, (x2+x1)/2+60, (y2+y1)/2, rot, 0);
   }

   if (bn == 310)
   {
      int tn = mItem.item[num][10];
      int x = (x2+x1)/2+60;
      int y = (y2+y1)/2-10;

      al_draw_filled_rectangle(x-1, y-1, x+21, y+21, mColor.pc[0]);
      al_draw_bitmap(mBitmap.btile[tn&1023], x, y, 0);
   }

   if (bn == 311)
   {
      int tn = mItem.item[num][11];
      int x = (x2+x1)/2+60;
      int y = (y2+y1)/2-10;

      al_draw_filled_rectangle(x-1, y-1, x+21, y+21, mColor.pc[0]);
      al_draw_bitmap(mBitmap.btile[tn&1023], x, y, 0);
   }
   if (q6) y1+=bts;
   return retval;
}




// displays a text string, and returns 1 if pressed
int mwWidget::buttont(int x1, int &y1, int x2, int bts, int bn, int num, int type, int obt, int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7, const char* txt)
{
   int y2 = y1+bts-2;
   int ret = 0;

   draw_slider_frame(x1, y1, x2, y2, q0, q1, q2, q3, q4, q5, q6, q7); // draw button frame
   draw_slider_text(x1, y1,  x2, y2, q2, q5, txt);

   if ((!q7) && (mInput.mouse_b[1][0]) && (mInput.mouse_x > x1) && (mInput.mouse_x < x2) && (mInput.mouse_y > y1) && (mInput.mouse_y < y2))
   {
      while (mInput.mouse_b[1][0]) mEventQueue.proc(); // wait for release
      ret = 1;
   }
   if (q6) y1+=bts;
   return ret;
}




// displays a text string, and returns 1 if pressed
// auto width based on text length x1 is the center
// highlight outline when moused over

int mwWidget::buttontca(int xc, int &y1, int xd, int bts, int bn, int num, int type, int obt, int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7, const char* txt)
{
   int y2 = y1+bts-2;
   int ret = 0;

   int tl = (strlen(txt)+1)*4;

   int x1 = xc-tl;
   int x2 = xc+tl;


   draw_slider_frame(x1, y1, x2, y2, q0, q1, q2, q3, q4, q5, q6, q7); // draw button frame
   draw_slider_text(x1, y1,  x2, y2, q2, q5, txt);

   if ((mInput.mouse_x > x1) && (mInput.mouse_x < x2) && (mInput.mouse_y > y1) && (mInput.mouse_y < y2)) // mouse on button
   {
      // show highlight
      al_draw_rounded_rectangle(x1, y1, x2, y2, 1, 1, mColor.pc[q3], 1);

      if ((!q7) && (mInput.mouse_b[1][0]))
      {
         while (mInput.mouse_b[1][0]) mEventQueue.proc(); // wait for release
         ret = 1;
      }
   }

   if ((!q7) && (mInput.mouse_b[1][0]) && (mInput.mouse_x > x1) && (mInput.mouse_x < x2) && (mInput.mouse_y > y1) && (mInput.mouse_y < y2))
   {
      while (mInput.mouse_b[1][0]) mEventQueue.proc(); // wait for release
      ret = 1;
   }
   if (q6) y1+=bts;
   return ret;
}

// no faded frame, just a simple frame that highlights when moused over
// displays a text string, and returns 1 if pressed
// auto width based on text length
// highlight outline when moused over
int mwWidget::buttontcb(int x1, int &y1, int xd, int bts, int bn, int num, int type, int obt, int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7, const char* txt)
{
   int y2 = y1+bts-2;
   int ret = 0;

   int tl = (strlen(txt)+1)*8;

   int x2 = x1+tl;

   int fc = q1;
   int tc = q2;


   if ((mInput.mouse_x > x1) && (mInput.mouse_x < x2) && (mInput.mouse_y > y1) && (mInput.mouse_y < y2)) // mouse on button
   {
      fc = q3;       // show highlight
      tc = q3;       // show highlight

      if ((!q7) && (mInput.mouse_b[1][0]))
      {
         while (mInput.mouse_b[1][0]) mEventQueue.proc(); // wait for release
         ret = 1;
      }
   }

   al_draw_rectangle(x1, y1, x2, y2, mColor.pc[fc], 1);
   draw_slider_text(x1, y1,  x2, y2, tc, q5, txt);


   if ((!q7) && (mInput.mouse_b[1][0]) && (mInput.mouse_x > x1) && (mInput.mouse_x < x2) && (mInput.mouse_y > y1) && (mInput.mouse_y < y2))
   {
      while (mInput.mouse_b[1][0]) mEventQueue.proc(); // wait for release
      ret = 1;
   }
   if (q6) y1+=bts;
   return ret;
}






























// displays a text string, and returns 1 if pressed but doesn't block
int mwWidget::buttont_nb(int x1, int &y1, int x2, int bts, int bn, int num, int type, int obt, int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7, const char* txt)
{
   int y2 = y1+bts-2;
   int ret = 0;

   draw_slider_frame(x1, y1, x2, y2, q0, q1, q2, q3, q4, q5, q6, q7); // draw button frame
   draw_slider_text(x1, y1,  x2, y2, q2, q5, txt);

   if ((!q7) && (mInput.mouse_b[1][0]) && (mInput.mouse_x > x1) && (mInput.mouse_x < x2) && (mInput.mouse_y > y1) && (mInput.mouse_y < y2))
   {
//      while (mInput.mouse_b[1][0]) mEventQueue.proc(); // wait for release
      ret = 1;
   }
   if (q6) y1+=bts;
   return ret;
}












// displays a text string and tile, and returns 1 if pressed --- tile is bn
int mwWidget::buttontt(int x1, int &y1, int x2, int bts, int tn, int num, int type, int obt, int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7, const char* txt)
{
   int y2 = y1+bts-2;
   int ret = 0;

   draw_slider_frame(x1, y1, x2, y2, q0, q1, q2, q3, q4, q5, q6, q7); // draw button frame
   draw_slider_text(x1, y1,  x2, y2, q2, q5, txt);

   // draw tile
   int x = x1+14;
   al_draw_filled_rectangle(x-1, y1+0, x+21, y1+22, mColor.pc[0]);
   al_draw_bitmap(mBitmap.btile[tn], x, y1+1, 0);

   if ((!q7) && (mInput.mouse_b[1][0]) && (mInput.mouse_x > x1) && (mInput.mouse_x < x2) && (mInput.mouse_y > y1) && (mInput.mouse_y < y2))
   {
      while (mInput.mouse_b[1][0]) mEventQueue.proc(); // wait for release
      ret = 1;
   }
   if (q6) y1+=bts;
   return ret;
}







// increment passed pointer (int &var) and display different text for each value
void mwWidget::buttonp(int x1, int &y1, int x2, int bts, int bn, int num, int type, int obt, int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7, int &var)
{
   char msg[80];
   int y2 = y1+bts-2;
   int press = 0;
   if ((!q7) && (mInput.mouse_b[1][0]) && (mInput.mouse_x > x1) && (mInput.mouse_x < x2) && (mInput.mouse_y > y1) && (mInput.mouse_y < y2))
   {
      while (mInput.mouse_b[1][0]) mEventQueue.proc(); // wait for release
      press = 1;
   }


   if (bn == 20)
   {
      if (press) var++;
      if ((var < 0) || (var > 2)) var = 0;
      if (var == 0) sprintf(msg,  "Hysteresis Mode:Static");
      if (var == 1) sprintf(msg,  "Hysteresis Mode:Instant");
      if (var == 2) sprintf(msg,  "Hysteresis Mode:Gradual");
   }

   if (bn == 21)
   {
      if (press) var++;
      if ((var < 0) || (var > 1)) var = 0;
      if (var == 0) sprintf(msg,  "Stationary");
      if (var == 1) sprintf(msg,  "Fall");
   }
   if (bn == 22)
   {
      if (press) var++;
      if ((var < -2) || (var > 1)) var = -2;
      if (var ==  1) sprintf(msg, "Fall");
      if (var ==  0) sprintf(msg, "Stationary");
      if (var == -1) sprintf(msg, "Carry");
      if (var == -2) sprintf(msg, "Carry Through Door");
   }
   if (bn == 23) // rocket only
   {
      if (press) var++;
      if ((var < -2) || (var > 1)) var = -2;
      if (var == -1) var = 0;
      if (var ==  1) sprintf(msg,  "Fall");
      if (var ==  0) sprintf(msg,  "Stationary");
      if (var == -2) sprintf(msg,  "Ride Through Door");
   }
   if (bn == 27) // cloner trigger type
   {
      if (press) var++;
      if ((var < 0) || (var > 3)) var = 0;
      if (var == 0)
      {
         sprintf(msg, "Trigger Type:Timer Runs  ");
         mEnemy.Ei[num][7] = mEnemy.Ei[num][6]; // set counter
      }

      if (var == 1)
      {
         sprintf(msg, "Trigger Type:Timer Resets");
         mEnemy.Ei[num][7] = mEnemy.Ei[num][6]; // set counter
      }
      if (var == 2) sprintf(msg, "Trigger Type:Immediate   ");
      if (var == 3) sprintf(msg, "Trigger Type:Event");
   }
   if (bn == 50) // door entry type
   {
      if (press) var++;
      if ((var < 0) || (var > 2)) var = 0;
      if (var == 0) sprintf(msg, "Enter Immediate  ");
      if (var == 1) sprintf(msg, "Enter with <up>  ");
      if (var == 2) sprintf(msg, "Enter with <down>");
   }
   if (bn == 51) // door show dest line type
   {
      if (press) var++;
      if ((var < 0) || (var > 2)) var = 0;
      if (var == 0) sprintf(msg, "Exit link:never show  ");
      if (var == 1) sprintf(msg, "Exit link:alway show  ");
      if (var == 2) sprintf(msg, "Exit link:when touched");
   }
   if (bn == 53) // door move type
   {
      if (press) var++;
      if ((var < 0) || (var > 2)) var = 0;
      if (var == 0) sprintf(msg, "Move Type:Automatic    ");
      if (var == 1) sprintf(msg, "Move Type:Force Instant");
      if (var == 2) sprintf(msg, "Move Type:Force Move   ");
   }
   if (bn == 78)
   {
      if (press) var++;
      if ((var < 0) || (var > 2)) var = 0;
      if (var == 0) sprintf(msg, "Start Mode:Default");
      if (var == 1) sprintf(msg, "Team Start");
      if (var == 2) sprintf(msg, "Checkpoint Common");
      if (var == 3) sprintf(msg, "Checkpoint Individual");
   }
   if (bn == 79)
   {
      if (press) var++;
      if ((var < 0) || (var > 7)) var = 0;
      sprintf(msg, "Start Index:%d", var);
   }
   if (bn == 81)
   {
      if (press) var++;
      if ((var < 0) || (var > 3)) var = 0;
      if (var == 0) sprintf(msg, "Draw Boxes:None");
      if (var == 1) sprintf(msg, "Draw Boxes:Source Only");
      if (var == 2) sprintf(msg, "Draw Boxes:Destination Only");
      if (var == 3) sprintf(msg, "Draw Boxes:Both");
   }

   if (bn == 82)
   {
      if (press) var++;
      if ((var < 0) || (var > 3)) var = 0;
      if (var == 0) sprintf(msg, "Draw Mode:Hidden");
      if (var == 1) sprintf(msg, "Draw Mode:Static Shape");
      if (var == 2) sprintf(msg, "Draw Mode:Static Animation");
      if (var == 3) sprintf(msg, "Draw Mode:Follow Event Timer");
   }

   if (bn == 83)
   {
      if (press) var++;
      if ((var < 0) || (var > 1)) var = 0;
      if (var == 0) sprintf(msg, "Draw Mode:Hidden");
      if (var == 1) sprintf(msg, "Draw Mode:Progress Bar");
   }


   if (bn == 85)
   {
      if (press) var++;
      if ((var < 0) || (var > 1)) var = 0;
      if (var == 0) sprintf(msg, "Output Mode:One Time");
      if (var == 1) sprintf(msg, "Output Mode:Continous");
   }




   if (bn == 100)
   {
      if (press) var++;
      if ((var < 0) || (var > 2)) var = 0;
      if (var == 0) sprintf(msg, "Action:Randomize");
      if (var == 1) sprintf(msg, "Action:Step from Min to Max");
      if (var == 2) sprintf(msg, "Action:Set all to Min");
   }
   if (bn == 101)
   {
      if (press) var++;
      if ((var < 1) || (var > 3)) var = 1;
      if (var == 1) sprintf(msg, "Type: Health Bonus");
      if (var == 2) sprintf(msg, "Type: Free Man");
      if (var == 3) sprintf(msg, "Type: Purple Coin");
   }
   if (bn == 102)
   {
      if (press) var++;
      if ((var < 1039) || (var > 1042)) var = 1039;
      if (var == 1039) { sprintf(msg, "Color:Red");    q1 = 10; }
      if (var == 1040) { sprintf(msg, "Color:Green");  q1 = 11; }
      if (var == 1041) { sprintf(msg, "Color:Blue");   q1 = 13; }
      if (var == 1042) { sprintf(msg, "Color:Purple"); q1 = 8;  }
   }



   if (bn == 160) // timer mode
   {
      if (press) var++;
      if ((var < 0) || (var > 4)) var = 0;
      if (var == 0) sprintf(msg, "Mode:Free Run");
      if (var == 1) sprintf(msg, "Mode:Free Run After Trigger");
      if (var == 2) sprintf(msg, "Mode:Run Only When Triggered");
      if (var == 3) sprintf(msg, "Mode:Reset When Not Triggered");
      if (var == 4) sprintf(msg, "Mode:Reset When Triggered");
   }


   if (bn == 301) // block manip mode
   {
      if (press) var++;
      if ((var < 0) || (var > 3)) var = 0;
      if (var == 0) sprintf(msg, "MODE:OFF");
      if (var == 1) sprintf(msg, "MODE:Set All To Block 1");
      if (var == 2) sprintf(msg, "MODE:Set All Block 2 To Block 1");
      if (var == 3) sprintf(msg, "MODE:Toggle Block 2 To Block 1");
   }
   if (bn == 402) // damage mode
   {
      if (press) var++;
      if ((var < 0) || (var > 3)) var = 0;
      if (var == 0)
      {
         sprintf(msg, "MODE:Always ON");
         mItem.item[num][3] |=  PM_ITEM_DAMAGE_CURR; // set damage on

      }

      if (var == 1) sprintf(msg, "MODE:Toggle");
      if (var == 2)
      {
         sprintf(msg, "MODE:ON Until Triggered");
         mItem.item[num][3] |=  PM_ITEM_DAMAGE_CURR; // set damage on
      }

      if (var == 3)
      {
         sprintf(msg, "MODE:OFF Until Triggered");
         mItem.item[num][3] &=  ~PM_ITEM_DAMAGE_CURR; // set damage off
      }

//      if (var == 4) sprintf(msg, "MODE:Timed ON And OFF");
//
//      if (var == 5) sprintf(msg, "MODE:Damage when Triggered");



   }


   if (bn == 404) // Block Damage draw mode
   {
      if (press) var++;
      if ((var < 0) || (var > 2)) var = 0;
      if (var == 0) sprintf(msg, "Draw Type:none         ");
      if (var == 1) sprintf(msg, "Draw Type:Red Rectangle");
      if (var == 2) sprintf(msg, "Draw Type:Spikey Floor ");
   }
   if (bn == 500) // lift mode
   {
      if (press) var++;
      if ((var < 0) || (var > 2)) var = 0;
      if (var == 0) sprintf(msg, "Mode 0 - Normal");
      if (var == 1) sprintf(msg, "Mode 1 - Prox Run and Reset");
      if (var == 2) sprintf(msg, "Mode 2 - Prox Reset");
   }
   draw_slider_frame(x1, y1, x2, y2, q0, q1, q2, q3, q4, q5, q6, q7); // draw button frame
   draw_slider_text(x1, y1,  x2, y2, q2, q5, msg);
   if (q6) y1+=bts;
}























































void mwWidget::colsel(int x1, int &y1, int x2, int bts, int bn, int num, int type, int obt, int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7 )
{
   char msg[80];
   int y2 = y1+bts-2;
   al_draw_filled_rectangle(x1, y1, x2, y2, mColor.pc[0]); // erase

   // draw colors (1-15)
   float a = x2-x1; // range
   float b = a/15;  // color swatch width
   if (b<2) b = 2;  // min width
   for (int c=0; c<15; c++)
      al_draw_filled_rectangle((int)(x1+c*b), y1, (int)(b+x1+c*b), y2, mColor.pc[(int)c+1]);

   if (bn == 2) sprintf(msg, "Select Text Color");
   if (bn == 3) sprintf(msg, "Select Frame Color");
   if (bn == 4) sprintf(msg, "Select Lift Color");
   if (bn == 5) sprintf(msg, "Select Door Color");
   if (bn == 6) sprintf(msg, "Select Trigger Field Color");
   if (bn == 7) sprintf(msg, "Select Block Manip Field Color");
   if (bn == 8) sprintf(msg, "Select Lift Step Color");


   draw_slider_text(x1, y1,  x2, y2, q2, q5, msg);

   // draw outline
   al_draw_rectangle(x1, y1, x2, y2, mColor.pc[15], 1);

   // is mouse pressed on button?
   if ((!q7) && (mInput.mouse_x > x1) && (mInput.mouse_x < x2) && (mInput.mouse_y > y1) && (mInput.mouse_y < y2) && (mInput.mouse_b[1][0]))
   {
      while (mInput.mouse_b[1][0]) mEventQueue.proc();
      int color = (int)(1+(mInput.mouse_x-x1)/b);

      if (bn == 2) // text color
      {
         int tc=0, fc = 0;
         mMiscFnx.get_int_3216(mItem.item[num][13], tc, fc);
         tc = color;
         mMiscFnx.set_int_3216(mItem.item[num][13], tc, fc);
      }

      if (bn == 3) // frame color
      {
         int tc=0, fc = 0;                  // text and frame colors
         mMiscFnx.get_int_3216(mItem.item[num][13], tc, fc);
         fc = color;
         mMiscFnx.set_int_3216(mItem.item[num][13], tc, fc);
      }
      if (bn == 4) mLift.cur[num].color = color; // lift color
      if (bn == 5)
      {
         mItem.item[num][6] = color;     // door color
         mItem.change_linked_door_color_and_shape(num);
      }
      if (bn == 6) mItem.item[num][2] = color;     // trigger color
      if (bn == 7) mItem.item[num][12] = color;    // block manip color


      if (bn == 8) // lift step color
      {
        // printf("n:%d t:%d c:%d\n",num, type, color);
         int cf = color << 28; // shift 4 bits of color into place
         mLift.stp[num][type].type &= 0b00001111111111111111111111111111; // clear old color
         mLift.stp[num][type].type |= cf; // merge color with type
      }
   }
   if (q6 == 1) y1+=bts;
}


// toggles the int and displays text, text color, and frame color based on value
int mwWidget::toggle(int x1, int &y1, int x2, int bts, int bn, int num, int type, int obt, int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7,
               int &var, const char* t0, const char* t1 , int text_col0, int text_col1, int frame_col0, int frame_col1)
{
   char msg[80];
   int y2 = y1+bts-2;
   int ret = 0;
   // is mouse pressed on this button?
   if ((mInput.mouse_b[1][0]) && (mInput.mouse_x > x1) && (mInput.mouse_x < x2) && (mInput.mouse_y > y1) && (mInput.mouse_y < y2) && (!q7))
   {
      while (mInput.mouse_b[1][0]) mEventQueue.proc(); // wait for release
      var = ! var;
   }
   if (var)
   {
       q1 = frame_col1;
       q2 = text_col1;
       sprintf(msg, "%s", t1);
       ret = 1;
   }
   else
   {
      q1 = frame_col0;
      q2 = text_col0;
      sprintf(msg, "%s", t0);
      ret = 0;
   }
   draw_slider_frame(x1, y1, x2, y2, q0, q1, q2, q3, q4, q5, q6, q7); // draw button frame
   draw_slider_text(x1, y1,  x2, y2, q2, q5, msg);

   if (q6 == 1) y1+=bts;
   return ret;
}








// toggles the int and displays text, text color, and frame color based on value  -- check box style
int mwWidget::togglec(int x1, int &y1, int x2, int bts, int bn, int num, int type, int obt, int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7,
               int &var, const char* t, int text_col, int frame_col)
{
   int y2 = y1+bts-2;
   int ret = 0;

   // debug show mouse detection area
//   if ((mInput.mouse_x > x1) && (mInput.mouse_x < x2) && (mInput.mouse_y > y1) && (mInput.mouse_y < y2) && (!q7))
//      al_draw_rectangle(x1, y1, x2, y2, mColor.pc[10], 1);


   // is mouse pressed on this button?
   if ((mInput.mouse_b[1][0]) && (mInput.mouse_x > x1) && (mInput.mouse_x < x2) && (mInput.mouse_y > y1) && (mInput.mouse_y < y2) && (!q7))
   {
      while (mInput.mouse_b[1][0]) mEventQueue.proc(); // wait for release
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

   if (q1>0) al_draw_rectangle(x1, y1, x2, y2, mColor.pc[q1], 1);


   if (var) al_draw_filled_rectangle(rx1, ry1, rx2, ry2, mColor.pc[frame_col]);
   else     al_draw_rectangle(       rx1, ry1, rx2, ry2, mColor.pc[frame_col], 1);

   al_draw_text(mFont.pr8, mColor.pc[text_col], mtx, mty, 0, t);

   if (var) ret = 1;
   else ret = 0;

   if (q6 == 1) y1+=bts;
   return ret;
}

































// toggle the flag and displays the corresponding string
// returns the value of the flag
int mwWidget::togglf(int x1, int &y1, int x2, int bts, int bn, int num, int type, int obt, int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7,
               int &var, int flag, const char* t0, const char* t1 , int text_col0, int text_col1, int frame_col0, int frame_col1)
{
   char msg[80];

   int ret = 0;
   int y2 = y1+bts-2;

   // is mouse pressed on this button?
   if ((!q7) && (mInput.mouse_b[1][0]) && (mInput.mouse_x > x1) && (mInput.mouse_x < x2) && (mInput.mouse_y > y1) && (mInput.mouse_y < y2))
   {
      while (mInput.mouse_b[1][0]) mEventQueue.proc(); // wait for release
      var ^= flag;
   }
   if (var & flag)
   {
       q1 = frame_col1;
       q2 = text_col1;
       sprintf(msg, "%s", t1);
       ret = 1;
   }
   else
   {
      q1 = frame_col0;
      q2 = text_col0;
      sprintf(msg, "%s", t0);
      ret = 0;
   }
   draw_slider_frame(x1, y1, x2, y2, q0, q1, q2, q3, q4, q5, q6, q7); // draw button frame
   draw_slider_text(x1, y1,  x2, y2, q2, q5, msg);
   if (q6 == 1) y1+=bts;
   return ret;
}
