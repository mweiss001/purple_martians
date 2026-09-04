// mwWidget.cpp

#include "pm.h"
#include "mwWidget.h"
#include "mwFont.h"
#include "mwColor.h"
#include "mwInput.h"
#include "mwEventQueue.h"
#include "mwMiscFnx.h"

#define SLIDER_BAR_WIDTH 3



// like slider it has a horizontal area that represents a range from ll to ul with var in that range
// clicking will set new range
// mouse over will display new value that would be set if clicked
// returns true if clicked
bool mwWidget::mTrackInt(int xType, int xa, int xb, int yType, int ya, int yb,
                         int r, int backgroundType, int frameType, int textType,
                         int bcol, int fcol, int bar_col, int tcol, int hcol, int highlight,
                         int text_just, int &var, int ul, int ll, const char *txt, int disable_input)
{
   int x1, y1, x2, y2;
   xyHelper(xType, xa, xb, yType, ya, yb, txt, x1, y1, x2, y2);
   mwRect<int> rect = mwRect<int>::fromX1Y1X2Y2(x1, y1, x2, y2);

   // if (backgroundType == 0) ; do nothing
   if (backgroundType == 1) al_draw_filled_rounded_rectangle(x1, y1, x2, y2, r, r, mColor.pc[bcol]); // solid color
   if (backgroundType == 2) draw_widget_area(x1, y1, x2, y2, bcol); // draw button frame

   if (frameType)
   {
      int c = fcol;
      if (highlight     && ((frameType == 2) || (frameType == 4))) c = hcol;
      al_draw_rounded_rectangle(x1, y1, x2, y2, r, r, mColor.pc[c], 1);
   }

   int line_color = bar_col;
   int display_var = var;
   // map var to screen position
   int line_x1 = mMiscFnx.map_range<int>(var, ll, ul, x1, x2);


   if (!disable_input && rect.contains(mInput.mouse_x, mInput.mouse_y))
   {
      line_color = 10;
      line_x1 = mInput.mouse_x;
      // map mouse x position to var range
      display_var = mMiscFnx.map_range<int>( mInput.mouse_x, x1, x2, ll, ul);

      if (mInput.mouse_b[1][0])
      {
         while (mInput.mouse_b[1][0]) mEventQueue.proc(1);
         var = display_var;
         return true;
      }
   }

   // draw text and line
   char msg[80];
   sprintf(msg, "%s%d", txt, display_var);
   draw_widget_text(x1, y1+1, x2, y2, tcol, 0, msg);
   al_draw_line(line_x1, y1, line_x1, y2, mColor.pc[line_color], 1);

   return false;
}



void mwWidget::mStepper(int xType, int xa, int xb, int yType, int ya, int yb,
                          int r, int backgroundType, int frameType, int textType,
                          int bcol, int fcol, int tcol, int hcol, int highlight,
                          int text_just, int &var, int ul, int ll, int inc, const char *txt)
{
   int x1, y1, x2, y2;
   xyHelper(xType, xa, xb, yType, ya, yb, txt, x1, y1, x2, y2);

   int bw = y2-y1; // make buttons square


   // get positions
   int b1x1 = x1;
   int b1x2 = x1+bw;

   int b2x1 = x2-bw;
   int b2x2 = x2;

   int tx1 = b1x2;
   int tx2 = b2x1;


   if (backgroundType == 2) draw_widget_area(tx1, y1, tx2, y2, bcol); // draw frame

   char msg[80];
   sprintf(msg, "%s%d", txt, var);
   draw_widget_text(tx1, y1, tx2, y2, tcol, text_just, msg);

   // add buttons

   if (mButton(0, b1x1, b1x2, 0, y1, y2, r, backgroundType, frameType, textType, bcol, fcol, tcol, hcol, highlight, "-", 0)) var--;
   if (mButton(0, b2x1, b2x2, 0, y1, y2, r, backgroundType, frameType, textType, bcol, fcol, tcol, hcol, highlight, "+", 0)) var++;

   if (var < ll) var = ll;
   if (var > ul) var = ul;
}




// ------------------------------------------------------------------------------------
// ---------------------------sliders--------------------------------------------------
// ------------------------------------------------------------------------------------


void mwWidget::mSliderFloat(int xType, int xa, int xb, int yType, int ya, int yb,
                          int r, int backgroundType, int frameType, int textType,
                          int bcol, int fcol, int bar_col, int tcol, int hcol, int highlight,
                          int text_just, float &var, float sul, float sll, float sinc, const char *txt, int update, bool disable_input)
{
   int x1, y1, x2, y2;
   xyHelper(xType, xa, xb, yType, ya, yb, txt, x1, y1, x2, y2);

   char msg[80];
   sprintf(msg, "%s%0.2f", txt, var);
   float dsx = drawSlider(x1, y1, x2, y2, r, backgroundType, bcol, fcol, bar_col, tcol, text_just, var, sul, sll, 1, msg);

   // is mouse on adjustment bar?
   if (!disable_input && (mInput.mouse_y > y1) && (mInput.mouse_y < y2) && (mInput.mouse_x > dsx-SLIDER_BAR_WIDTH) && (mInput.mouse_x < dsx+SLIDER_BAR_WIDTH))
   {
      drawSliderBar(var, sul, sll, x1+SLIDER_BAR_WIDTH+1, y1, x2-SLIDER_BAR_WIDTH-1, y2, 2, bar_col); // draw highlighted bar
      while (mInput.mouse_b[3][0])
      {
         var = getSliderPositionMouseAlt(var, sul, sll, sinc,  x1, y1, x2, y2);
         updateCall(update);
         sprintf(msg, "%s%0.2f", txt, var);
         drawSlider(x1, y1, x2, y2, r, backgroundType, bcol, fcol,  bar_col, tcol, text_just, var, sul, sll, 2, msg);
      }
      while (mInput.mouse_b[1][0])
      {
         var = getSliderPositionMouse(sul, sll, sinc, x1, y1, x2, y2);
         updateCall(update);
         sprintf(msg, "%s%0.2f", txt, var);
         drawSlider(x1, y1, x2, y2, r, backgroundType, bcol, fcol, bar_col, tcol, text_just, var, sul, sll, 2, msg);
      }
   }
}


void mwWidget::mSliderInt(int xType, int xa, int xb, int yType, int ya, int yb,
                          int r, int backgroundType, int frameType, int textType,
                          int bcol, int fcol, int bar_col, int tcol, int hcol, int highlight,
                          int text_just, int &var, float sul, float sll, float sinc, const char *txt, int update, bool disable_input)
{
   int x1, y1, x2, y2;
   xyHelper(xType, xa, xb, yType, ya, yb, txt, x1, y1, x2, y2);

   char msg[80];
   sprintf(msg, "%s%d", txt, var);
   float dsx = drawSlider(x1, y1, x2, y2, r, backgroundType, bcol, fcol, bar_col, tcol, text_just, var, sul, sll, 1, msg);

   // is mouse on adjustment bar?
   if (!disable_input && (mInput.mouse_y > y1) && (mInput.mouse_y < y2) && (mInput.mouse_x > dsx-SLIDER_BAR_WIDTH) && (mInput.mouse_x < dsx+SLIDER_BAR_WIDTH))
   {
      drawSliderBar(var, sul, sll, x1+SLIDER_BAR_WIDTH+1, y1, x2-SLIDER_BAR_WIDTH-1, y2, 2, bar_col); // draw highlighted bar
      while (mInput.mouse_b[3][0])
      {
         var = getSliderPositionMouseAlt((float) var, sul, sll, sinc,  x1, y1, x2, y2);
         updateCall(update);
         sprintf(msg, "%s%d", txt, var);
         drawSlider(x1, y1, x2, y2, r, backgroundType, bcol, fcol,  bar_col, tcol, text_just, var, sul, sll, 2, msg);
      }
      while (mInput.mouse_b[1][0])
      {
         var = getSliderPositionMouse(sul, sll, sinc, x1, y1, x2, y2);
         updateCall(update);
         sprintf(msg, "%s%d", txt, var);
         drawSlider(x1, y1, x2, y2, r, backgroundType, bcol, fcol, bar_col, tcol, text_just, var, sul, sll, 2, msg);
      }
   }
}


void mwWidget::mSliderInt0(int xType, int xa, int xb, int yType, int ya, int yb,
                          int r, int backgroundType, int frameType, int textType,
                          int bcol, int fcol, int bar_col, int tcol, int hcol, int highlight,
                          int text_just, int &var, float sul, float sll, float sinc, const char *txt, const char *txt0, int update, bool disable_input)
{
   int x1, y1, x2, y2;
   xyHelper(xType, xa, xb, yType, ya, yb, txt, x1, y1, x2, y2);

   char msg[80];
   sprintf(msg, "%s%d", txt, var);
   if (var == 0) sprintf(msg, "%s%s", txt, txt0);

   float dsx = drawSlider(x1, y1, x2, y2, r, backgroundType, bcol, fcol, bar_col, tcol, text_just, var, sul, sll, 1, msg);

   // is mouse on adjustment bar?
   if (!disable_input && (mInput.mouse_y > y1) && (mInput.mouse_y < y2) && (mInput.mouse_x > dsx-SLIDER_BAR_WIDTH) && (mInput.mouse_x < dsx+SLIDER_BAR_WIDTH))
   {
      drawSliderBar(var, sul, sll, x1+SLIDER_BAR_WIDTH+1, y1, x2-SLIDER_BAR_WIDTH-1, y2, 2, bar_col); // draw highlighted bar
      while (mInput.mouse_b[3][0])
      {
         var = getSliderPositionMouseAlt((float) var, sul, sll, sinc,  x1, y1, x2, y2);
         updateCall(update);
         sprintf(msg, "%s%d", txt, var);
         drawSlider(x1, y1, x2, y2, r, backgroundType, bcol, fcol,  bar_col, tcol, text_just, var, sul, sll, 2, msg);
      }
      while (mInput.mouse_b[1][0])
      {
         var = getSliderPositionMouse(sul, sll, sinc, x1, y1, x2, y2);
         updateCall(update);
         sprintf(msg, "%s%d", txt, var);
         drawSlider(x1, y1, x2, y2, r, backgroundType, bcol, fcol, bar_col, tcol, text_just, var, sul, sll, 2, msg);
      }
   }
}




float mwWidget::drawSlider(int x1, int y1, int x2, int y2, int r, int backgroundType,
                           int bcol, int fcol, int bar_col, int tcol, int text_just,
                           float sdx, float sul, float sll, int order, const char *msg)
{
   // if (backgroundType == 0) ; do nothing
   if (backgroundType == 1) al_draw_filled_rounded_rectangle(x1, y1, x2, y2, r, r, mColor.pc[bcol]); // solid color
   if (backgroundType == 2) draw_widget_area(x1, y1, x2, y2, bcol); // draw button frame

   if (fcol > 0) al_draw_rectangle(x1, y1, x2, y2, mColor.pc[fcol], 1);

   float dsx = 0;
   if (order == 1)
   {
      dsx = drawSliderBar(sdx, sul, sll, x1+SLIDER_BAR_WIDTH+1, y1, x2-SLIDER_BAR_WIDTH-1, y2, 1, bar_col);
      draw_widget_text(x1, y1, x2, y2, tcol, text_just, msg);
   }
   if (order == 2)
   {
      draw_widget_text(x1, y1, x2, y2, tcol, text_just, msg);
      dsx = drawSliderBar(sdx, sul, sll, x1+SLIDER_BAR_WIDTH+1, y1, x2-SLIDER_BAR_WIDTH-1, y2, 1, bar_col);
   }
   return dsx;
}


float mwWidget::drawSliderBar(float sdx, float sul, float sll, int x1, int y1, int x2, int y2, int dm, int col)
{
   float f = getSliderPosition(sdx, sul, sll, x1, y1, x2, y2);
   int sx1 = (int)f - SLIDER_BAR_WIDTH;
   int sx2 = (int)f + SLIDER_BAR_WIDTH;
   // draw slider bar
   for (int i=0; i<SLIDER_BAR_WIDTH+1; i++)
      al_draw_rectangle(sx1+i, y1+i, sx2-i, y2-i, mColor.pc[col+192-(i*64)], 1);

   // draw rectangle around slider bar to show highlight
   if (dm == 2) al_draw_rectangle(sx1-1, y1, sx2+1, y2, mColor.pc[15], 1);
   return f;

}


float mwWidget::getSliderPosition(float sdx, float sul, float sll, int x1, int y1, int x2, int y2)
{
   float a, b, c, d, e, f;
   // get slider position
   a = sdx-sll; // relative position
   b = sul-sll; // range
   c = a/b;     // ratio
   d = x2-x1;   // range
   e = d * c;   // range * old ratio
   f = e + x1;  // add offset
   return f;
}

float mwWidget::getSliderPositionMouse(float sul, float sll, float sinc, int x1, int y1, int x2, int y2)
{
   float mx = mInput.mouse_x;
   float a, b, c, d, e, f ;

   // enforce limits
   if (mx<x1) mx = x1;
   if (mx>x2) mx = x2;

   // get slider position
   a = mx-x1;                  // relative position of slider bar in range
   b = x2-x1;                  // range
   c = a / b;                  // ratio = position / range
   d = sul-sll;                // range from buttons
   e = c * d;                  // ratio * range
   f = e + sll;                // add to ll
   f = round(f/sinc) * sinc;   // round to sinc
   return f;
}

float mwWidget::getSliderPositionMouseAlt(float f, float sul, float sll, float sinc, int x1, int y1, int x2, int y2)
{
   if (mInput.key[ALLEGRO_KEY_RIGHT][2]) f += sinc;
   if (mInput.key[ALLEGRO_KEY_LEFT][2])  f -= sinc;
   if (mInput.mouse_dz)
   {
      f += mInput.mouse_dz * sinc;
      mInput.mouse_dz = 0;
   }

   if (f < sll) f = sll;       // limit check
   if (f > sul) f = sul;
   f = round(f/sinc) * sinc;   // round to sinc

   return f;
}














// ------------------------------------------------------------------------------------
// ---------------------------step sliders---------------------------------------------
// ------------------------------------------------------------------------------------


void mwWidget::mStepSliderFloat(int xType, int xa, int xb, int yType, int ya, int yb,
                          int r, int backgroundType, int frameType, int textType,
                          int bcol, int fcol, int bar_col, int tcol, int hcol, int highlight,
                          int text_just, float &var, float ul, float ll, float slinc, float stinc1, float stinc2, const char *txt, int update, bool disable_input)
{
   // running x offset for step buttons
   int rx = 0;

   // full text
   // if (stinc1)
   // {
   //    char msg[100];
   //    sprintf(msg, "-%0.2f", stinc1);
   //    int bsp = strlen(msg)*8 + 6;
   //    if (mButton(1, xa+rx, bsp, yType, ya, yb, r, backgroundType, frameType, textType, bcol, fcol, tcol, hcol, highlight, msg, disable_input)) var-=stinc1;
   //    sprintf(msg, "+%0.2f", stinc1);
   //    if (mButton(2, bsp, xb-rx, yType, ya, yb, r, backgroundType, frameType, textType, bcol, fcol, tcol, hcol, highlight, msg, disable_input)) var+=stinc1;
   //    rx += bsp+2;
   // }
   // if (stinc2)
   // {
   //    char msg[100];
   //    sprintf(msg, "-%0.2f", stinc2);
   //    int bsp = strlen(msg)*8 + 6;
   //    if (mButton(1, xa+rx, bsp, yType, ya, yb, r, backgroundType, frameType, textType, bcol, fcol, tcol, hcol, highlight, msg, disable_input)) var-=stinc2;
   //    sprintf(msg, "-%0.2f", stinc2);
   //    if (mButton(2, bsp, xb-rx, yType, ya, yb, r, backgroundType, frameType, textType, bcol, fcol, tcol, hcol, highlight, msg, disable_input)) var+=stinc2;
   //    rx += bsp+2;
   // }



   // + - ++ --
   if (stinc1)
   {
      int bsp = 1*8 + 6;
      if (mButton(1, xa+rx, bsp, yType, ya, yb, r, backgroundType, frameType, textType, bcol, fcol, tcol, hcol, highlight, "-", disable_input)) var-=stinc1;
      if (mButton(2, bsp, xb-rx, yType, ya, yb, r, backgroundType, frameType, textType, bcol, fcol, tcol, hcol, highlight, "+", disable_input)) var+=stinc1;
      rx += bsp+2;
   }

   if (stinc2)
   {
      int bsp = 2*8 + 6;
      if (mButton(1, xa+rx, bsp, yType, ya, yb, r, backgroundType, frameType, textType, bcol, fcol, tcol, hcol, highlight, "--", disable_input)) var-=stinc2;
      if (mButton(2, bsp, xb-rx, yType, ya, yb, r, backgroundType, frameType, textType, bcol, fcol, tcol, hcol, highlight, "++", disable_input)) var+=stinc2;
      rx += bsp+2;
   }


   // do slider in remaining space
   mSliderFloat(xType, xa+rx, xb-rx, yType, ya, yb,   r, backgroundType, frameType, textType,   bcol, fcol, bar_col, tcol, hcol, highlight,   text_just, var, ul, ll, slinc, txt, update, disable_input);

   // enforce limits
   if (var < ll) var = ll;
   if (var > ul) var = ul;
}








void mwWidget::mStepSliderInt0(int xType, int xa, int xb, int yType, int ya, int yb,
                          int r, int backgroundType, int frameType, int textType,
                          int bcol, int fcol, int bar_col, int tcol, int hcol, int highlight,
                          int text_just, int &var, int ul, int ll, int slinc, int stinc1, int stinc2, const char *txt, const char *txt0, int update, bool disable_input)
{

   // running x offset for step buttons
   int rx = 0;

   // if (stinc1)
   // {
   //    char msg[100];
   //    sprintf(msg, "-%d", stinc1);
   //    int bsp = strlen(msg)*8 + 6;
   //    if (mButton(1, xa+rx, bsp, yType, ya, yb, r, backgroundType, frameType, textType, bcol, fcol, tcol, hcol, highlight, msg, disable_input)) var-=stinc1;
   //    sprintf(msg, "+%d", stinc1);
   //    if (mButton(2, bsp, xb-rx, yType, ya, yb, r, backgroundType, frameType, textType, bcol, fcol, tcol, hcol, highlight, msg, disable_input)) var+=stinc1;
   //    rx += bsp+2;
   // }
   //
   // if (stinc2)
   // {
   //    char msg[100];
   //    sprintf(msg, "-%d", stinc2);
   //    int bsp = strlen(msg)*8 + 6;
   //    if (mButton(1, xa+rx, bsp, yType, ya, yb, r, backgroundType, frameType, textType, bcol, fcol, tcol, hcol, highlight, msg, disable_input)) var-=stinc2;
   //    sprintf(msg, "+%d", stinc2);
   //    if (mButton(2, bsp, xb-rx, yType, ya, yb, r, backgroundType, frameType, textType, bcol, fcol, tcol, hcol, highlight, msg, disable_input)) var+=stinc2;
   //    rx += bsp+2;
   // }


   if (stinc1)
   {
      int bsp = 1*8 + 6;
      if (mButton(1, xa+rx, bsp, yType, ya, yb, r, backgroundType, frameType, textType, bcol, fcol, tcol, hcol, highlight, "-", disable_input)) var-=stinc1;
      if (mButton(2, bsp, xb-rx, yType, ya, yb, r, backgroundType, frameType, textType, bcol, fcol, tcol, hcol, highlight, "+", disable_input)) var+=stinc1;
      rx += bsp+2;
   }

   if (stinc2)
   {
      int bsp = 2*8 + 6;
      if (mButton(1, xa+rx, bsp, yType, ya, yb, r, backgroundType, frameType, textType, bcol, fcol, tcol, hcol, highlight, "--", disable_input)) var-=stinc2;
      if (mButton(2, bsp, xb-rx, yType, ya, yb, r, backgroundType, frameType, textType, bcol, fcol, tcol, hcol, highlight, "++", disable_input)) var+=stinc2;
      rx += bsp+2;
   }

   // do slider in remaining space
   mSliderInt0(xType, xa+rx, xb-rx, yType, ya, yb,   r, backgroundType, frameType, textType,   bcol, fcol, bar_col, tcol, hcol, highlight,   text_just, var, ul, ll, slinc, txt, txt0, update, disable_input);

   // enforce limits
   if (var < ll) var = ll;
   if (var > ul) var = ul;
}




void mwWidget::mStepSliderInt(int xType, int xa, int xb, int yType, int ya, int yb,
                          int r, int backgroundType, int frameType, int textType,
                          int bcol, int fcol, int bar_col, int tcol, int hcol, int highlight,
                          int text_just, int &var, int ul, int ll, int slinc, int stinc1, int stinc2, const char *txt, int update, bool disable_input)
{
   // running x offset for step buttons
   int rx = 0;

   // if (stinc1)
   // {
   //    char msg[100];
   //    sprintf(msg, "-%d", stinc1);
   //    int bsp = strlen(msg)*8 + 6;
   //    if (mButton(1, xa+rx, bsp, yType, ya, yb, r, backgroundType, frameType, textType, bcol, fcol, tcol, hcol, highlight, msg, disable_input)) var-=stinc1;
   //    sprintf(msg, "+%d", stinc1);
   //    if (mButton(2, bsp, xb-rx, yType, ya, yb, r, backgroundType, frameType, textType, bcol, fcol, tcol, hcol, highlight, msg, disable_input)) var+=stinc1;
   //    rx += bsp+2;
   // }
   //
   // if (stinc2)
   // {
   //    char msg[100];
   //    sprintf(msg, "-%d", stinc2);
   //    int bsp = strlen(msg)*8 + 6;
   //    if (mButton(1, xa+rx, bsp, yType, ya, yb, r, backgroundType, frameType, textType, bcol, fcol, tcol, hcol, highlight, msg, disable_input)) var-=stinc2;
   //    sprintf(msg, "+%d", stinc2);
   //    if (mButton(2, bsp, xb-rx, yType, ya, yb, r, backgroundType, frameType, textType, bcol, fcol, tcol, hcol, highlight, msg, disable_input)) var+=stinc2;
   //    rx += bsp+2;
   // }


   if (stinc1)
   {
      int bsp = 1*8 + 6;
      if (mButton(1, xa+rx, bsp, yType, ya, yb, r, backgroundType, frameType, textType, bcol, fcol, tcol, hcol, highlight, "-", disable_input)) var-=stinc1;
      if (mButton(2, bsp, xb-rx, yType, ya, yb, r, backgroundType, frameType, textType, bcol, fcol, tcol, hcol, highlight, "+", disable_input)) var+=stinc1;
      rx += bsp+2;
   }
   if (stinc2)
   {
      int bsp = 2*8 + 6;
      if (mButton(1, xa+rx, bsp, yType, ya, yb, r, backgroundType, frameType, textType, bcol, fcol, tcol, hcol, highlight, "--", disable_input)) var-=stinc2;
      if (mButton(2, bsp, xb-rx, yType, ya, yb, r, backgroundType, frameType, textType, bcol, fcol, tcol, hcol, highlight, "++", disable_input)) var+=stinc2;
      rx += bsp+2;
   }

   // do slider in remaining space
   mSliderInt(xType, xa+rx, xb-rx, yType, ya, yb,   r, backgroundType, frameType, textType,   bcol, fcol, bar_col, tcol, hcol, highlight,   text_just, var, ul, ll, slinc, txt, update, disable_input);

   // enforce limits
   if (var < ll) var = ll;
   if (var > ul) var = ul;
}





