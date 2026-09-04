// mwWidget.cpp

#include "pm.h"
#include "mwWidget.h"
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
#include "mwItem.h"
#include "mwEnemy.h"
#include "mwMiscFnx.h"
#include "mwHelp.h"
#include "mwBottomMessage.h"
#include "mwConfig.h"
#include "mwDemoMode.h"
#include "mwLevelEditor.h"
#include "mwLoop.h"
#include "mwObjectViewer.h"
#include "mwScreen.h"
#include "mwSettings.h"


mwWidget mWidget;

// #define SLIDER_BAR_WIDTH 3


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


20260829

   6  x mSliderFloat
   10 x mSliderInt
   1  x mSliderInt0

   6  x mStepSliderFloat
   53 x mStepSliderInt
   20 x mStepSliderInt0

   34 x mwObjectViewer::odbi
   26 x mwObjectViewer::odb0
   34 x mwObjectViewer::odbf
   10 x mwObjectViewer::odbt












*/













// ------------------------------------------------------------------------------------
// ---------------------- widget common -----------------------------------------------
// ------------------------------------------------------------------------------------

void mwWidget::draw_widget_area(int x1, int y1, int x2, int y2, int base_color)
{
   if (base_color == -1000) // hue slider only
   {
      float width = x2-x1;
      float hue_inc = 360 / width;
      for (int x=0; x<width; x++)
         al_draw_line(x1+x, y1, x1+x, y2, al_color_hsl(x*hue_inc, 1, 0.5), 1);

      for (int c=0; c<1; c++)
         al_draw_rounded_rectangle(x1+c, y1+c, x2-c, y2-c, 1, 1, mColor.White, 1);

      return;
   }

   if (base_color != -1)
   {
      for (int c=0; c<((y2-y1)/2+1); c++)
      {
         int a = (c*32); // color increment
         if (a>224) a = 224;

         // frame fades from solid outer to black inner
         //int a = 224 - (c*32); // color increment
         //if (a<0) a = 0;

         int col = base_color + a;
         while (col > 255) col -=16;
         al_draw_rounded_rectangle(x1+c, y1+c, x2-c, y2-c, 1, 1, mColor.pc[col], 1);
      }
   }
}


void mwWidget::draw_widget_text(int x1, int y1, int x2, int y2, int color, int left_justified, const char* msg)
{
   int xt = (x2+x1)/2;
   int yt = y1 + (y2-y1-8)/2;
   if (left_justified) al_draw_text(mFont.pr8, mColor.pc[color], x1+4, yt, 0, msg);
   else                al_draw_text(mFont.pr8, mColor.pc[color], xt, yt, ALLEGRO_ALIGN_CENTER, msg);
}


// ------------------------------------------------------------------------------------
// --------------------------buttons---------------------------------------------------
// ------------------------------------------------------------------------------------



// wrapper that increments ya by bts
int mwWidget::mColorSelect(int xType, int xa, int xb, int &ya, int yb, int type, int &v1, int v2, const char* txt, int disable_input)
{
   int ret = mColorSelect(xType, xa, xb, 1, ya, yb-2, type, v1, v2, txt, disable_input);
   ya+= yb;
   return ret;
}


int mwWidget::mColorSelect(int xType, int xa, int xb, int yType, int ya, int yb, int type, int &v1, int v2, const char* txt, int disable_input)
{
   int x1, y1, x2, y2;
   xyHelper(xType, xa, xb, yType, ya, yb, "", x1, y1, x2, y2);

   al_draw_filled_rectangle(x1, y1, x2, y2, mColor.pc[0]); // erase

   // draw colors (1-15)
   float a = x2-x1; // range
   float b = a/15;  // color swatch width
   if (b<2) b = 2;  // min width
   for (int c=0; c<15; c++)
      al_draw_filled_rectangle((int)(x1+c*b), y1, (int)(b+x1+c*b), y2, mColor.pc[(int)c+1]);


   draw_widget_text(x1, y1,  x2, y2, 0, 0, txt);

   // draw outline
   al_draw_rectangle(x1, y1, x2, y2, mColor.pc[15], 1);


   // check if mouse is on button
   bool mouseOnButton = false;
   if ((!disable_input) && (mInput.mouse_x > x1) && (mInput.mouse_x < x2) && (mInput.mouse_y > y1) && (mInput.mouse_y < y2)) mouseOnButton = true;


   if (mouseOnButton && mInput.mouse_b[1][0])
   {
      while (mInput.mouse_b[1][0]) mEventQueue.proc(1); // wait for release
      int color = (int)(1+(mInput.mouse_x-x1)/b);

      if (type == 1) v1 = color;

      return color;

   }

   return -1;

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
      while (mInput.mouse_b[1][0]) mEventQueue.proc(1); // wait for release
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
   draw_widget_area(x1, y1, x2, y2, q1); // draw button frame
   draw_widget_text(x1, y1,  x2, y2, q2, q5, msg);

   if (q6 == 1) y1+=bts;
   return ret;
}








// toggles the int and displays text, text color, and frame color based on value  -- check box style
int mwWidget::togglec(int x1, int &y1, int x2, int bts, int bn, int num, int type, int obt, int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7,
               int &var, const char* t, int text_col, int frame_col)
{
   int y2 = y1+bts-2;
   int press = 0;

   /*
   // debug show mouse detection area
   if ((mInput.mouse_x > x1) && (mInput.mouse_x < x2) && (mInput.mouse_y > y1) && (mInput.mouse_y < y2) && (!q7))
      al_draw_rectangle(x1, y1, x2, y2, mColor.pc[10], 1);
*/

   // is mouse pressed on this button?
   if ((mInput.mouse_b[1][0]) && (mInput.mouse_x > x1) && (mInput.mouse_x < x2) && (mInput.mouse_y > y1) && (mInput.mouse_y < y2) && (!q7))
   {
      while (mInput.mouse_b[1][0]) mEventQueue.proc(1); // wait for release
      var = ! var;
      press = 1;
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

   if (q6 == 1) y1+=bts;
   return press;
}




















// double toggle check box flag -- custom for log types only
void mwWidget::togglec_log(int x1, int &y1, int x2, int bts, int q6, int ltn, int text_col, int frame_col)
{
   // check box size
   int cbs = 6;

   // check box y positions
   int cb_y1 = y1 + (bts - cbs - 2) /2;
   int cb_y2 = cb_y1 + cbs;

   // checkbox 1
   int cb1_x1 = x1;
   int cb1_x2 = cb1_x1 + cbs;

   // checkbox 2
   int cb2_x1 = cb1_x2 + cbs + 2;
   int cb2_x2 = cb2_x1 + cbs;


   // draw checkboxes
   if (mLog.log_types[ltn].action & LOG_ACTION_LOG)   al_draw_filled_rectangle(cb1_x1, cb_y1, cb1_x2, cb_y2, mColor.pc[frame_col]);
   else                                               al_draw_rectangle(       cb1_x1, cb_y1, cb1_x2, cb_y2, mColor.pc[frame_col], 1);
   if (mLog.log_types[ltn].action & LOG_ACTION_PRINT) al_draw_filled_rectangle(cb2_x1, cb_y1, cb2_x2, cb_y2, mColor.pc[frame_col]);
   else                                               al_draw_rectangle(       cb2_x1, cb_y1, cb2_x2, cb_y2, mColor.pc[frame_col], 1);


   // process mouse on checkboxes
   if ((mInput.mouse_y > cb_y1) && (mInput.mouse_y < cb_y2))
   {
      if ((mInput.mouse_x > cb1_x1) && (mInput.mouse_x < cb1_x2))
      {
         al_draw_rectangle(cb1_x1, cb_y1, cb1_x2, cb_y2, mColor.pc[14], 1);
         if (mInput.mouse_b[1][0])
         {
             while (mInput.mouse_b[1][0]) mEventQueue.proc(1); // wait for release
             mLog.log_types[ltn].action ^= LOG_ACTION_LOG;
         }
      }
      if ((mInput.mouse_x > cb2_x1) && (mInput.mouse_x < cb2_x2))
      {
         al_draw_rectangle(cb2_x1, cb_y1, cb2_x2, cb_y2, mColor.pc[14], 1);
         if (mInput.mouse_b[1][0])
         {
             while (mInput.mouse_b[1][0]) mEventQueue.proc(1); // wait for release
             mLog.log_types[ltn].action ^= LOG_ACTION_PRINT;
         }
      }
   }

   // draw text
   float mtx = cb2_x2 + cbs + 2;
   float mty = y1 + (bts-10)/2;
   al_draw_text(mFont.pr8, mColor.pc[text_col], mtx, mty, 0, mLog.log_types[ltn].name);

   if (q6 == 1) y1+=bts;
}













// toggles the flag and displays text, text color, and frame color based on value  -- check box style
int mwWidget::togglfc(int x1, int &y1, int x2, int bts, int bn, int num, int type, int obt, int q0, int q1, int q2, int q3, int q4, int q5, int q6, int q7,
               int &var, int flag, const char* t, int text_col, int frame_col)
{
   int y2 = y1+bts-2;
   int press = 0;

   // debug show mouse detection area
//   if ((mInput.mouse_x > x1) && (mInput.mouse_x < x2) && (mInput.mouse_y > y1) && (mInput.mouse_y < y2) && (!q7))
//      al_draw_rectangle(x1, y1, x2, y2, mColor.pc[10], 1);


   // is mouse pressed on this button?
   if ((mInput.mouse_b[1][0]) && (mInput.mouse_x > x1) && (mInput.mouse_x < x2) && (mInput.mouse_y > y1) && (mInput.mouse_y < y2) && (!q7))
   {
      while (mInput.mouse_b[1][0]) mEventQueue.proc(1); // wait for release
      var ^= flag;
      press = 1;
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


   if (var & flag) al_draw_filled_rectangle(rx1, ry1, rx2, ry2, mColor.pc[frame_col]);
   else            al_draw_rectangle(       rx1, ry1, rx2, ry2, mColor.pc[frame_col], 1);

   al_draw_text(mFont.pr8, mColor.pc[text_col], mtx, mty, 0, t);

   if (q6 == 1) y1+=bts;
   return press;
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
      while (mInput.mouse_b[1][0]) mEventQueue.proc(1); // wait for release
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
   draw_widget_area(x1, y1, x2, y2, q1); // draw button frame
   draw_widget_text(x1, y1,  x2, y2, q2, q5, msg);
   if (q6 == 1) y1+=bts;
   return ret;
}



















// ---------------------------------------------------------------------------------------------------------------------------
// ------------------new widgets from 2026 -----------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------------


// xType
// 0  abs xa, abs xb
// 1  abs xa, xb is width
// 2  abs xb, xa is width
// 3  abs xa,         width from text length, xb used to pad text length
// 4  abs xb,         width from text length, xa used to pad text length
// 5  centered on xa, width from text length, xb used to pad text length



// yType
// 0  abs ya, abs yb
// 1  abs ya, yb is height
// 2  abs yb, ya is height
// 5  abs center ya, yb height (y1, y2 are set from yb height)

// backgroundType
// 0 = do nothing
// 1 = solid color
// 2 = legacy faded button frame

// frameType
// 0 = no frame
// 1 = static
// 2 = highlight with var
// 3 = highlight with mouse
// 4 = highlight with both


// textType
// 0  = no text
// 1  = centered
// 2  = centered highlight with var
// 3  = centered highlight with mouse
// 4  = centered highlight with both
// 21 = left justified
// 22 = left justified highlight with var
// 23 = left justified highlight with mouse
// 24 = left justified highlight with both








void mwWidget::xyHelper(int xType, int xa, int xb, int yType, int ya, int yb, const char* txt, int &x1, int &y1, int &x2, int &y2)
{
   // process x

   // default when xType == 0
   x1 = xa;
   x2 = xb;

   if (xType == 1) x2 = xa + xb; // abs xa, xb is width
   if (xType == 2) x1 = xb - xa; // abs xb, xa is width

   int tl = (strlen(txt)+1)*8; // text length

   if (xType == 3) x2 = xa + (tl + xb); // abs xa, auto width from text with xb padding text length
   if (xType == 4) x1 = xb - (tl + xa); // abs xb, auto width from text with xa padding text length
   if (xType == 5) // abs center xa,  auto width from text with xb padding text length
   {
      tl+=xb;
      x1 = xa - tl/2;
      x2 = xa + tl/2;
   }



   if (xType == 6) // abs center xa,  auto text width with xb padding added to text length
   {
      printf("xyHelper MODE 6  --  I thought this was unused!!!!!!\n");

      tl += xb; // text length

      x1 = xa - tl/2;
      x2 = xa + tl/2;
   }

   // process y
   // default when yType == 0
   y1 = ya;
   y2 = yb;

   if (yType == 1) y2 = ya + yb; // abs ya, yb is height
   if (yType == 2) y1 = yb - ya; // abs yb, ya is height
   if (yType == 5) // abs center ya,  y1, y2 is auto from yb height
   {
      y1 = ya - yb/2;
      y2 = ya + yb/2;
   }
}

mwRect<int> mwWidget::xyHelper(int xType, int xa, int xb, int yType, int ya, int yb, const char* txt)
{
   int x1, y1, x2, y2;
   xyHelper(xType, xa, xb, yType, ya, yb, txt, x1, y1, x2, y2);
   mwRect<int> r = mwRect<int>::fromX1Y1X2Y2(x1, y1, x2, y2);
   return r;
}



bool mwWidget::mCheckBoxWithToolTip(int xType, int xa, int xb, int yType, int ya, int yb, int frame_col, int &var, const char* t, int text_col, int box_col, bool disable_input, const char* tt)
{
   bool ret = mCheckBox(xType, xa, xb, yType, ya, yb, frame_col, var, t, text_col, box_col, disable_input);
   if (!disable_input) mToolTip(xType, xa+12, (strlen(tt) + 1)*8+6, yType, ya, yb,  0,   1,0,1,   0,15,15, tt, xa+2, ya, xa+10, ya+9);
   return ret;
}


void mwWidget::mToolTip(int xType, int xa, int xb, int yType, int ya, int yb, int r, int backgroundType, int frameType, int textType, int bcol, int fcol, int tcol, const char* txt, int tx1, int ty1, int tx2, int ty2)
{
   //al_draw_rectangle(tx1, ty1, tx2, ty2, mColor.Red, 1); // debug show trigger area

   // check if mouse is on button
   if ((mInput.mouse_x > tx1) && (mInput.mouse_x < tx2) && (mInput.mouse_y > ty1) && (mInput.mouse_y < ty2))
   {
      int x1, y1, x2, y2;
      xyHelper(xType, xa, xb, yType, ya, yb, txt, x1, y1, x2, y2);

      // if (backgroundType == 0) ; do nothing
      if (backgroundType == 1) al_draw_filled_rounded_rectangle(x1, y1, x2, y2, r, r, mColor.pc[bcol]); // solid color
      if (backgroundType == 2) draw_widget_area(x1, y1, x2, y2, bcol); // draw button frame

      if (frameType) al_draw_rounded_rectangle(x1, y1, x2, y2, r, r, mColor.pc[fcol], 1);

      draw_widget_text(x1, y1+1, x2, y2, tcol, textType, txt);
   }
}



// toggles the int and displays text, text color, and frame color based on value
// return true if changed
bool mwWidget::mButtonToggle(int xType, int xa, int xb, int yType, int ya, int yb, int r, int backgroundType, int frameType, int textType, int bcol, int fcol, int tcol, int hcol, int highlight,
                             int &var, const char* t0, const char* t1 , int text_col0, int text_col1, int frame_col0, int frame_col1, int disable_input)
{
   bool ret = 0;

   // get current text
   char txt[500];
   sprintf(txt, "%s", t0);
   int text_col = text_col0;
   int frame_col = frame_col0;

   if (var)
   {
      sprintf(txt, "%s", t1);
      text_col = text_col1;
      frame_col = frame_col1;
   }



   int x1, y1, x2, y2;
   xyHelper(xType, xa, xb, yType, ya, yb, txt, x1, y1, x2, y2);

   // check if mouse is on button
   bool mouseOnButton = false;
   if ((!disable_input) && (mInput.mouse_x > x1) && (mInput.mouse_x < x2) && (mInput.mouse_y > y1) && (mInput.mouse_y < y2)) mouseOnButton = true;

   if (mouseOnButton && mInput.mouse_b[1][0])
   {
      while (mInput.mouse_b[1][0]) mEventQueue.proc(1); // wait for release
      var = ! var;
      ret = 1;
   }


   if (var)
   {
      sprintf(txt, "%s", t1);
      text_col = text_col1;
      frame_col = frame_col1;
   }
   else
   {
      sprintf(txt, "%s", t0);
      text_col = text_col0;
      frame_col = frame_col0;
   }

   xyHelper(xType, xa, xb, yType, ya, yb, txt, x1, y1, x2, y2);

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
      int c = frame_col;
      if (highlight     && ((frameType == 2) || (frameType == 4))) c = hcol;
      if (mouseOnButton && ((frameType == 3) || (frameType == 4))) c = hcol;
      al_draw_rounded_rectangle(x1, y1, x2, y2, r, r, mColor.pc[c], 1);
   }


   draw_widget_text(x1, y1,  x2, y2, text_col, textType, txt);

   return ret;
}







// wrapper for button that takes rect
bool mwWidget::mButton(mwRect<int> rect, int r, int backgroundType, int frameType, int textType, int bcol, int fcol, int tcol, int hcol, int highlight, const char* txt, int disable_input)
{
   return mButton(0, rect.x1, rect.x2, 0, rect.y1, rect.y2, r, backgroundType, frameType, textType, bcol, fcol, tcol, hcol, highlight, txt, disable_input);
}


// wrapper to increment ya by bts
bool mwWidget::mButton(int xType, int xa, int xb, int &ya, int bts, int r, int backgroundType, int frameType, int textType, int bcol, int fcol, int tcol, int hcol, int highlight, const char* txt, int disable_input)
{
   bool res = mButton(xType, xa, xb, 1, ya, bts-2, r, backgroundType, frameType, textType, bcol, fcol, tcol, hcol, highlight, txt, disable_input);
   ya += bts;
   return res;
}



bool mwWidget::mButtonNB(int xType, int xa, int xb, int yType, int ya, int yb, int r, int backgroundType, int frameType, int textType, int bcol, int fcol, int tcol, int hcol, int highlight, const char* txt, int disable_input)
{
   int x1, y1, x2, y2;
   xyHelper(xType, xa, xb, yType, ya, yb, txt, x1, y1, x2, y2);

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
      draw_widget_text(x1, y1, x2, y2, c, tj, txt);
   }

   if (mouseOnButton && (mInput.mouse_b[1][2])) return true;


   return false;
}








bool mwWidget::mButton(int xType, int xa, int xb, int yType, int ya, int yb, int r, int backgroundType, int frameType, int textType, int bcol, int fcol, int tcol, int hcol, int highlight, const char* txt, int disable_input)
{
   int x1, y1, x2, y2;
   xyHelper(xType, xa, xb, yType, ya, yb, txt, x1, y1, x2, y2);

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
      draw_widget_text(x1, y1, x2, y2, c, tj, txt);
   }

   if (mouseOnButton && (mInput.mouse_b[1][0]))
   {
      while (mInput.mouse_b[1][0]) mEventQueue.proc(1); // wait for release
      return true;
   }


   return false;
}






































































































// toggles the int and displays text, text color, and frame color based on value  -- check box style
bool mwWidget::mCheckBox(int xType, int xa, int xb, int yType, int ya, int yb, int frame_col, int &var, const char* t, int text_col, int box_col, bool disable_input)
{
   bool changed = 0;

   int x1, y1, x2, y2;
   xyHelper(xType, xa, xb, yType, ya, yb, t, x1, y1, x2, y2);

   // check if mouse is on button
   if ((!disable_input) && (mInput.mouse_x > x1) && (mInput.mouse_x < x2) && (mInput.mouse_y > y1) && (mInput.mouse_y < y2))
   {
      // debug show mouse detection area
      //al_draw_rectangle(x1, y1, x2, y2, mColor.pc[10], 1);

      // is mouse pressed on this button?
      if (mInput.mouse_b[1][0])
      {
         while (mInput.mouse_b[1][0]) mEventQueue.proc(1); // wait for release
         var = !var;
         changed = 1;
      }
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

   if (frame_col>0) al_draw_rectangle(x1, y1, x2, y2, mColor.pc[frame_col], 1);

   if (var) al_draw_filled_rectangle(rx1, ry1, rx2, ry2, mColor.pc[box_col]);
   else     al_draw_rectangle(       rx1, ry1, rx2, ry2, mColor.pc[box_col], 1);

   al_draw_text(mFont.pr8, mColor.pc[text_col], mtx, mty, 0, t);

   return changed;

}






// toggles the int and displays text, text color, and frame color based on value  -- check box style
void mwWidget::mCheckBoxSmallText(int xType, int xa, int xb, int yType, int ya, int yb, int frame_col, int &var, const char* t, int text_col, int box_col)
{
   int x1, y1, x2, y2;
   xyHelper(xType, xa, xb, yType, ya, yb, t, x1, y1, x2, y2);

   // check if mouse is on button
   if ((mInput.mouse_x > x1) && (mInput.mouse_x < x2) && (mInput.mouse_y > y1) && (mInput.mouse_y < y2))
   {
      // debug show mouse detection area
      //al_draw_rectangle(x1, y1, x2, y2, mColor.pc[10], 1);

      // is mouse pressed on this button?
      if (mInput.mouse_b[1][0])
      {
         while (mInput.mouse_b[1][0]) mEventQueue.proc(1); // wait for release
         var = !var;
      }
   }

   float my1 = y1;
   float my2 = y2;

   // get y center of button
   float myc = my1 + (my2-my1)/2;

   float rs = 4; // check box size
   float ry1 = myc - rs/2;
   float ry2 = ry1 + rs;
   float rx1 = x1 + 4;
   float rx2 = rx1 + rs;

   // text pos
   float mtx = rx2+6;
   float mty = myc-6.5;

   if (frame_col>0) al_draw_rectangle(x1, y1, x2, y2, mColor.pc[frame_col], 1);

   if (var) al_draw_filled_rectangle(rx1, ry1, rx2, ry2, mColor.pc[box_col]);
   else     al_draw_rectangle(       rx1, ry1, rx2, ry2, mColor.pc[box_col], 1);

   al_draw_text(mFont.pixl, mColor.pc[text_col], mtx, mty, 0, t);
}



// displays a framed, scaled tile and toggle int if pressed
bool mwWidget::mButtonTile(int x1, int y1, int size, int tn, bool &var, bool disable_input)
{
   int x2 = x1 + size;
   int y2 = y1 + size;

   bool mouse_on_tile = false;
   if ((!disable_input) && (mInput.mouse_x > x1) && (mInput.mouse_x < x2) && (mInput.mouse_y > y1) && (mInput.mouse_y < y2)) mouse_on_tile = true;

   // determine frame color
   int c = 15;
   if (!var) c += 128;
   if (mouse_on_tile) c = 12;

   // draw frame
   al_draw_rectangle(x1, y1, x2, y2, mColor.pc[c], 1);

   // draw tile
   al_draw_scaled_bitmap(mBitmap.tile[tn], 0, 0, 20, 20, x1+1, y1+1, size-2, size-2, 0);

   if (mouse_on_tile && mInput.mouse_b[1][0])
   {
      while (mInput.mouse_b[1][0]) mEventQueue.proc(1); // wait for release
      var = !var;
      return true;
   }
   return false;
}

// displays a scaled tile and returns true if clicked, displays tooltip if mouse over and not disabled
bool mwWidget::mButtonTile2(int x1, int y1, int size, int tn, const char* t, bool disable_input)
{
   int x2 = x1 + size;
   int y2 = y1 + size;

   al_draw_filled_rectangle(x1, y1, x2, y2, mColor.Black); // erase tile and frame

   // draw tile
   al_draw_scaled_bitmap(mBitmap.tile[tn], 0, 0, 20, 20, x1+1, y1+1, size-2, size-2, 0);

   bool mouse_on_tile = false;
   if ((!disable_input) && (mInput.mouse_x > x1) && (mInput.mouse_x < x2) && (mInput.mouse_y > y1) && (mInput.mouse_y < y2)) mouse_on_tile = true;

   if (mouse_on_tile)
   {
      if (mInput.mouse_b[1][0])
      {
         while (mInput.mouse_b[1][0]) mEventQueue.proc(1); // wait for release
         return true;
      }

      al_draw_rectangle(x1, y1, x2, y2, mColor.pc[15], 1); // draw frame

      if (strlen(t)) mToolTip(5, x1+10, 0,     1, y1-14, 12,       1,     1, 1, 1,    0,  15, 15,    t, x1, y1, x2, y2);

   }
   return false;
}

// just like button but with added tile
bool mwWidget::mButtonTile3(int xType, int xa, int xb, int yType, int ya, int yb, int r, int backgroundType, int frameType, int textType, int bcol, int fcol, int tcol, int hcol, int highlight, int x1, int y1, int size, int tn, const char* txt, int disable_input)
{
   bool ret = mButton(xType, xa, xb, yType, ya, yb, r, backgroundType, frameType, textType, bcol, fcol, tcol, hcol, highlight, txt, disable_input);

   int x2 = x1 + size;
   int y2 = y1 + size;
   al_draw_filled_rectangle(x1-1, y1-1, x2+1, y2+1, mColor.Black); // erase tile and frame

   // draw tile
   al_draw_scaled_bitmap(mBitmap.tile[tn], 0, 0, 20, 20, x1, y1, size, size, 0);

   return ret;

}





// displays a player tile with highlight frame, and returns 1 if pressed --- tile is tn
bool mwWidget::mButtonPlayerTile(int x1, int y1, int size, int tn, int fc, int hc, int highlight)
{
   int x2 = x1+ size;
   int y2 = y1+ size;
   int scale = size - 2;

   // erase background
//   al_draw_filled_rectangle(x1, y1, x2, y2, mColor.pc[0]);

   // draw tile
   al_draw_scaled_bitmap(mBitmap.player_tile[tn][1], 1, 0, 18, 20, x1+1, y1+1, scale, scale, 0);

   int c = fc;
   if (highlight) c = hc;

   // draw button frame
   if (c!=0) al_draw_rounded_rectangle(x1, y1, x2, y2, 0, 0, mColor.pc[c], 0);

   if ((mInput.mouse_b[1][0]) && (mInput.mouse_x > x1) && (mInput.mouse_x < x2) && (mInput.mouse_y > y1) && (mInput.mouse_y < y2))
   {
      while (mInput.mouse_b[1][0]) mEventQueue.proc(1); // wait for release
      return true;
   }
   return false;
}

















void mwWidget::drawWidgetSmallText(int x1, int y1, int x2, int y2, int color, int left_justified, const char* msg)
{
   int xt = (x2+x1)/2;
   int yt = y1 - 7 + (y2-y1)/2; // why 7 pixels off??

   if (left_justified) al_draw_text(mFont.pixl, mColor.pc[color], x1+4, yt, 0, msg);
   else                al_draw_text(mFont.pixl, mColor.pc[color], xt, yt, ALLEGRO_ALIGN_CENTER, msg);
}




bool mwWidget::mButtonSmallText(int xType, int xa, int xb, int yType, int ya, int yb, int r, int backgroundType, int frameType, int textType, int bcol, int fcol, int tcol, int hcol, int highlight, const char* txt)
{
   int x1, y1, x2, y2;
   xyHelper(xType, xa, xb, yType, ya, yb, txt, x1, y1, x2, y2);

   // check if mouse is on button
   bool mouseOnButton = false;
   if ((mInput.mouse_x > x1) && (mInput.mouse_x < x2) && (mInput.mouse_y > y1) && (mInput.mouse_y < y2)) mouseOnButton = true;

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
      al_draw_rounded_rectangle(x1, y1, x2, y2, r, r, mColor.pc[c], 1);
   }


   // 2 = highlight with var
   // 3 = highlight with mouse
   // 4 = highlight with both
   if (textType)
   {
      int c = tcol;
      if (highlight     && ((textType == 2) || (textType == 4))) c = hcol;
      if (mouseOnButton && ((textType == 3) || (textType == 4))) c = hcol;

      // centering
      int ta = 0; // always
      // if (xType > 2) ta = 0;

      drawWidgetSmallText(x1, y1+1, x2, y2, c, ta, txt);
   }

   if (mouseOnButton && (mInput.mouse_b[1][0]))
   {
      while (mInput.mouse_b[1][0]) mEventQueue.proc(1); // wait for release
      return true;
   }
   return false;
}



bool mwWidget::colorClickSlider(int type, float x1, float &y1, float x2, float bts, float &val, bool &hover, float &hover_val, ALLEGRO_COLOR c, bool &changed_flag, bool display_only)
{
   int df = ALLEGRO_ALIGN_CENTER | ALLEGRO_ALIGN_INTEGER;

   bool changed = false;
   float min = 0;
   float max = 1;
   float step = 0.002;
   int round_decimals = 3;

   if ((type == 1) || (type == 2) || (type == 3)) step = 1/256; // r g b

   if (type == 4) // hue
   {
      step = 1.0;
      max = 360;
      round_decimals = 0;
   }

   if ((type == 5) || (type == 6)) // sat and light
   {
      step = 0.01;
      round_decimals = 2;
   }


   if (type == 7) // steps
   {
      step = 1.0;
      max = 20;
      round_decimals = 0;
   }
   if (type == 8)
   {
      max = 20; // round
      step = 0.2;
      round_decimals = 1;
   }

   float y2 = y1 + bts - 1;

   // erase background
   al_draw_filled_rectangle(x1, y1, x2, y2, mColor.Black);

   // slider position
   float sx1 = x1+bts;
   float sx2 = x2-bts;
   float sw = sx2-sx1;
   float sxc = sx1+sw/2;
   float tyc = y1+(y2-y1-8)/2;

   // draw slider background
   if (type >= 1 && type <= 3) // r g b
   {
      float ri = 0;
      float gi = 0;
      float bi = 0;
      if (type == 1) ri = 1 / sw;
      if (type == 2) gi = 1 / sw;
      if (type == 3) bi = 1 / sw;
      for (float i=0; i<sw; i++)
         al_draw_line(sx1+i, y1, sx1+i, y2, al_map_rgb_f(i*ri, i*gi, i*bi), 1);
   }
   if (type == 4) // hue
   {
      for (float i=0; i<sw; i++)
         al_draw_line(sx1+i, y1, sx1+i, y2, al_color_hsl(i * (max/sw), 1.0, 0.5), 1);
   }
   if (type == 5) // sat
   {
      float h, s, l;
      mColor.map_rgb_to_hsl(c, h, s, l);
      for (float i=0; i<sw; i++)
         al_draw_line(sx1+i, y1, sx1+i, y2, al_color_hsl(h, i * (max/sw), l), 1);
   }
   if (type == 6) // light
   {
      float h, s, l;
      mColor.map_rgb_to_hsl(c, h, s, l);
      for (float i=0; i<sw; i++)
         al_draw_line(sx1+i, y1, sx1+i, y2, al_color_hsl(h, s, i * (max/sw)), 1);
   }

   // frame
   al_draw_rectangle(x1, y1, x2, y2, mColor.White, 1);

   // draw + and minus lines and text
   al_draw_rectangle(sx1, y1, sx1, y2, mColor.White, 1);
   al_draw_rectangle(sx2, y1, sx2, y2, mColor.White, 1);
   al_draw_text(mFont.pr8, mColor.White, x1 + (sx1-x1)/2, tyc, df, "-");
   al_draw_text(mFont.pr8, mColor.White, x2 + (sx2-x2)/2, tyc, df, "+");


   if (!display_only)
   {
      // mouse on - button
      if (mInput.mouse_x > x1 && mInput.mouse_x < sx1 && mInput.mouse_y > y1 && mInput.mouse_y < y2)
      {
         if (mInput.mouse_b[1][0])
         {
            while (mInput.mouse_b[1][0]) mEventQueue.proc(1);
            changed = changed_flag = true;
            val -= step;
            if (val < min) val = min;
         }
      }

      // mouse on + button
      if (mInput.mouse_x > sx2 && mInput.mouse_x < x2 && mInput.mouse_y > y1 && mInput.mouse_y < y2)
      {
         if (mInput.mouse_b[1][0])
         {
            while (mInput.mouse_b[1][0]) mEventQueue.proc(1);
            changed = changed_flag = true;
            val += step;
            if (val > max) val = max;
         }
      }

      if (mInput.mouse_x > sx1 && mInput.mouse_x < sx2 && mInput.mouse_y > y1 && mInput.mouse_y < y2)
      {
         hover = true;
         // get hover value from mouse position
         hover_val = mMiscFnx.map_range<float>((float)mInput.mouse_x, sx1, sx2, min, max);
         if (mInput.mouse_b[1][0])
         {
            changed = changed_flag = true;
            val = hover_val;
         }
      }
   }

   // round here
   if (type == 8) val = mMiscFnx.roundToDecimalPlaces(val, 1);
   if (type == 8) hover_val = mMiscFnx.roundToDecimalPlaces(hover_val, 1);

   val = mMiscFnx.roundToDecimalPlaces(val, round_decimals);
   hover_val = mMiscFnx.roundToDecimalPlaces(hover_val, round_decimals);

   // set display value from either actual value or hover value
   float display_val = val;
   if (hover) display_val = hover_val;

   // get x position from display val and draw line
   float vx = mMiscFnx.map_range<float>(display_val, min, max,  sx1, sx2);
   al_draw_line(vx, y1, vx, y2, mColor.White, 2);

   if (type == 1) al_draw_textf(mFont.pr8, mColor.White, sxc, tyc, df, "red:%d",       (int) (display_val * 255));
   if (type == 2) al_draw_textf(mFont.pr8, mColor.White, sxc, tyc, df, "green:%d",     (int) (display_val * 255));
   if (type == 3) al_draw_textf(mFont.pr8, mColor.White, sxc, tyc, df, "blue:%d",      (int) (display_val * 255));
   if (type == 4) al_draw_textf(mFont.pr8, mColor.Black, sxc, tyc, df, "hue:%d",       (int) display_val);
   if (type == 5) al_draw_textf(mFont.pr8, mColor.White, sxc, tyc, df, "sat:%3.2f",          display_val);
   if (type == 6) al_draw_textf(mFont.pr8, mColor.White, sxc, tyc, df, "light:%3.2f",        display_val);
   if (type == 7) al_draw_textf(mFont.pr8, mColor.White, sxc, tyc, df, "steps:%d",     (int) display_val);
   if (type == 8) al_draw_textf(mFont.pr8, mColor.White, sxc, tyc, df, "round:%3.1f",        display_val);

   y1 += bts;

  return changed;
}





void mwWidget::updateCall(int update)
{
   if (update == 0)
   {
      mEventQueue.proc(1);
      al_flip_display();
   }
   if (update == 1)
   {
      mLevelEditor.redraw_background();
      mLevelEditor.mWM.cycle_windows(1);
   }
   if (update == 2)
   {
      mSettings.settings_pages_redraw(1);
   }
}




