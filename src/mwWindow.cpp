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
   moveable = 1;
   moving = 0;
   resizable = 0;

   min_width = 100;
   max_width = 100;

   min_height = 100;
   max_height = 100;


   drawFunction = nullptr;
   detectMouse = nullptr;
   redrawCallback = nullptr;

}

void mwWindow::set_title(const char* st) {  sprintf(title, "%s", st);  }
void mwWindow::set_pos(int x, int y)     {  rect.setX1Y1(x, y);   }
void mwWindow::set_size(int w, int h)    {  rect.setWH(w, h);  }

bool mwWindow::detect_mouse()
{
   if (detectMouse) return detectMouse();
   else return rect.contains(mInput.mouse_x, mInput.mouse_y);
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
   if (mInput.mouse_b[1][0])
   {
      // title bar rect
      mwRect<int> title_bar_rect = rect;
      title_bar_rect.setHeight(11);

      // size adjust rect
      mwRect<int> size_adj_rect = rect;
      size_adj_rect.setWH_adj_X1Y1(10, 10);

      int mxo, myo;

      // moveable and mouse on title bar
      if (moveable && title_bar_rect.contains(mInput.mouse_x, mInput.mouse_y, mxo, myo))
      {
         moving = 1;
         while (mInput.mouse_b[1][0])
         {
            set_pos(mInput.mouse_x - mxo, mInput.mouse_y - myo);
            if (redrawCallback) redrawCallback();
         }
      }

      // resizable and mouse on lower right corner
      if (resizable && size_adj_rect.contains(mInput.mouse_x, mInput.mouse_y))
      {
         mxo = mInput.mouse_x - rect.x2; // get offset from mouse position to window x2, y2
         myo = mInput.mouse_y - rect.y2;
         moving = 1;
         while (mInput.mouse_b[1][0])
         {
            rect.x2 = mInput.mouse_x-mxo;
            rect.y2 = mInput.mouse_y-myo;
            rect.w = rect.x2 - rect.x1;
            rect.h = rect.y2 - rect.y1;

            bool lim = false;
            if (rect.w < min_width)  { lim = 1; rect.w = min_width;  }
            if (rect.w > max_width)  { lim = 1; rect.w = max_width;  }
            if (rect.h < min_height) { lim = 1; rect.h = min_height; }
            if (rect.h > max_height) { lim = 1; rect.h = max_height; }
            if (lim)
            {
               rect.x2 = rect.x1 + rect.w;
               rect.y2 = rect.y1 + rect.h;
               al_set_mouse_xy(mDisplay.display, (rect.x2+mxo) * mDisplay.display_transform_double, (rect.y2+myo) * mDisplay.display_transform_double);
            }

            if (redrawCallback) redrawCallback();
         }
      }
      moving = 0;
   }
}


void mwWindow::draw(int draw_only)
{
   // by default disable input (draw only)
   disable_input = 1;

   // this is the only case where input is enabled
   if (have_focus) disable_input = 0;

   if (moving) disable_input = 1;
   if (draw_only) disable_input = 1;

   if (drawFunction) drawFunction();
   else
   {
      // erase background
      rect.draw_filled_rectangle(mColor.pc[0]);

      // title bar rect
      mwRect<int> title_bar_rect = rect;
      title_bar_rect.setHeight(11);

      // size adjust rect
      mwRect<int> size_adj_rect = rect;
      size_adj_rect.setWH_adj_X1Y1(10, 10);

      // frame window
      rect.draw_rectangle(mColor.pc[color], 1);

      // frame title bar
      title_bar_rect.draw_rectangle(mColor.pc[color], 1);

      // title
      al_draw_textf(mFont.pr8, mColor.pc[color], rect.x1+2, rect.y1+ 2, 0, "title:%s", title);
      al_draw_textf(mFont.pr8, mColor.pc[color], rect.x1+2, rect.y1+12, 0, "index:%d", index);
      al_draw_textf(mFont.pr8, mColor.pc[color], rect.x1+2, rect.y1+22, 0, "layer:%d", layer);
      al_draw_textf(mFont.pr8, mColor.pc[color], rect.x1+2, rect.y1+30, 0, "focus:%d", have_focus);
      al_draw_textf(mFont.pr8, mColor.pc[color], rect.x1+2, rect.y1+38, 0, "x1:%d x2:%d w:%d", rect.x1, rect.x2, rect.w);
      al_draw_textf(mFont.pr8, mColor.pc[color], rect.x1+2, rect.y1+46, 0, "y1:%d y2:%d h:%d", rect.y1, rect.y2, rect.h);

      if (have_focus)
      {
         // highlight window frame
         rect.draw_rectangle(mColor.pc[10], 2);

         if (moveable && title_bar_rect.contains(mInput.mouse_x, mInput.mouse_y))
         {
            // highlight title bar frame
            title_bar_rect.draw_rectangle(mColor.pc[10], 2);
         }

         if (resizable && size_adj_rect.contains(mInput.mouse_x, mInput.mouse_y))
         {
            // highlight size adj
            size_adj_rect.draw_rectangle(mColor.pc[10], 2);
         }
      }
   }
}


