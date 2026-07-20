// mwWindow.cpp
#include "pm.h"
#include "mwWindow.h"
#include "mwWindowManager.h"
#include "mwColor.h"
#include "mwDisplay.h"
#include "mwFont.h"
#include "mwInput.h"
#include "mwLevelEditor.h"
#include "mwScreen.h"


mwWindow::mwWindow()
{
   rect.setXYWH(0,0,0,0);
   color = 15;
   title[0] = 0;
   active = 0;
   have_focus = 0;
   disable_input = 0;
   layer = 0;
   hidden = 0;
   moveable = 1;
   moving = 0;
   resizable = 0;
}

void mwWindow::set_title(const char* st) {  sprintf(title, "%s", st);  }
void mwWindow::set_pos(int x, int y)     {  rect.setX1Y1(x, y);   }
void mwWindow::set_size(int w, int h)    {  rect.setWH(w, h);  }

bool mwWindow::detect_mouse()
{
   if (!detectMouse) return rect.contains(mInput.mouse_x, mInput.mouse_y);
   else return detectMouse();
}


void mwWindow::check_offscreen()
{
   int change = 0;
   if (rect.x1 < 1) { rect.x1=1; change=1; }
   if (rect.y1 < 1) { rect.y1=1; change=1; }
   if (rect.x1 > mDisplay.SCREEN_W-100) { rect.x1=mDisplay.SCREEN_W-100; change=1; }
   if (rect.y1 > mDisplay.SCREEN_H-100) { rect.y1=mDisplay.SCREEN_H-100; change=1; }
   if (change) set_pos(rect.x1, rect.y1 );
}



void mwWindow::process_mouse(void)
{
   if (!hidden)
   {
      if (mInput.mouse_b[1][0])
      {
         // moveable and mouse on title bar
         if ((moveable) && (mInput.mouse_x > rect.x1) && (mInput.mouse_x < rect.x2) && (mInput.mouse_y > rect.y1) && (mInput.mouse_y < rect.y1+8))
         {
            int mxo = mInput.mouse_x - rect.x1; // get offset from mouse position to window x, y
            int myo = mInput.mouse_y - rect.y1;
            moving = 1;
            while (mInput.mouse_b[1][0])
            {
               set_pos(mInput.mouse_x - mxo, mInput.mouse_y - myo);
               mLevelEditor.redraw_background(0);
               mScreen.draw_scaled_level_region_to_display();
               mLevelEditor.mWM.cycle_windows(1); // draw only
            }
            moving = 0;
         }

         // resizable and mouse on lower right corner
         if ((resizable) && (mInput.mouse_x > rect.x2-10) && (mInput.mouse_x < rect.x2) && (mInput.mouse_y > rect.y2-10) && (mInput.mouse_y < rect.y2))
         {
            int mxo = mInput.mouse_x - rect.x2; // get offset from mouse position to window x, y
            int myo = mInput.mouse_y - rect.y2;
            moving = 1;
            while (mInput.mouse_b[1][0])
            {
               rect.x2 = mInput.mouse_x-mxo;
               rect.y2 = mInput.mouse_y-myo;
               rect.w = rect.x2 - rect.x1;
               rect.h = rect.y2 - rect.y1;
               mLevelEditor.redraw_background(0);
               mScreen.draw_scaled_level_region_to_display();
               mLevelEditor.mWM.cycle_windows(1); // draw only
            }
         }
         moving = 0;
      }
   }
}

void mwWindow::draw(int draw_only)
{
   // erase background
   if ((!hidden) && (index != 8)) al_draw_filled_rectangle(rect.x1, rect.y1, rect.x2, rect.y2, mColor.pc[0]);

   // by default disable input (display only)
   disable_input = 1;

   // this is the only case where input is enabled
   if (have_focus) disable_input = 0;

   if (moving) disable_input = 1;
   if (draw_only) disable_input = 1;

   if (drawFunction) drawFunction();
   else
   {
      // frame window
      al_draw_rectangle(rect.x1, rect.y1, rect.x2, rect.y2, mColor.pc[color], 1);

      // frame title bar
      al_draw_rectangle(rect.x1, rect.y1, rect.x2, rect.y1+11, mColor.pc[color], 1);

      // title
      al_draw_textf(mFont.pr8, mColor.pc[color], rect.x1+2, rect.y1+2, 0, "title:%s", title);
      al_draw_textf(mFont.pr8, mColor.pc[color], rect.x1+2, rect.y1+12, 0, "index:%d", index);
      al_draw_textf(mFont.pr8, mColor.pc[color], rect.x1+2, rect.y1+22, 0, "layer:%d", layer);
      al_draw_textf(mFont.pr8, mColor.pc[color], rect.x1+2, rect.y1+30, 0, "focus:%d", have_focus);
      al_draw_textf(mFont.pr8, mColor.pc[color], rect.x1+2, rect.y1+38, 0, "x1:%d x2:%d w:%d", rect.x1, rect.x2, rect.w);
      al_draw_textf(mFont.pr8, mColor.pc[color], rect.x1+2, rect.y1+46, 0, "y1:%d y2:%d h:%d", rect.y1, rect.y2, rect.h);
      al_draw_rectangle(rect.x1, rect.y1, rect.x2, rect.y2, mColor.pc[10], 1);

      if (have_focus)
      {
         // frame entire window
         al_draw_rectangle(rect.x1, rect.y1, rect.x2, rect.y2, mColor.pc[10], 1);

         // frame title bar
         if (moveable)
         {
            if ((mInput.mouse_x > rect.x1) && (mInput.mouse_x < rect.x2) && (mInput.mouse_y > rect.y1) && (mInput.mouse_y < rect.y1+8))
               al_draw_rectangle(rect.x1, rect.y1, rect.x2, rect.y1+11, mColor.pc[10], 1);
         }
         // lower right corner
         if (resizable)
         {
            if ((mInput.mouse_x > rect.x2-10) && (mInput.mouse_x < rect.x2) && (mInput.mouse_y > rect.y2-10) && (mInput.mouse_y < rect.y2)) // lower right corner for resize
               al_draw_rectangle(rect.x2-10, rect.y2-10, rect.x2, rect.y2, mColor.pc[10], 1);
         }
      }
   }
}


