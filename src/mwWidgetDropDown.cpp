// mwWidget.cpp

#include "pm.h"
#include "mwWidget.h"
#include "mwColor.h"
#include "mwInput.h"
#include "mwEventQueue.h"
#include "mwMiscFnx.h"
#include "mwLoop.h"



void mwWidget::mDropDownSetNextItem(std::vector<listItem> listItems, int & var, bool rollOver)
{
   int ls = (int)listItems.size(); // list size
   for (int i=0; i<ls; i++)
      if (listItems[i].value == var)
      {
         int new_index = i+1;
         if (new_index > ls-1)
         {
            if (rollOver) new_index = 0;
            else new_index = ls-1;
         }
         var = listItems[new_index].value;
         break;
      }
}

void mwWidget::mDropDownSetPrevItem(std::vector<listItem> listItems, int & var, bool rollOver)
{
   int ls = (int)listItems.size(); // list size
   for (int i=0; i<ls; i++)
      if (listItems[i].value == var)
      {
         int new_index = i-1;
         if (new_index < 0)
         {
            if (rollOver) new_index = ls-1;
            else new_index = 0;
         }
         var = listItems[new_index].value;
         break;
      }
}



// draws the main control
// if crop = 1, then text will be chopped, rect width will be exact
// if crop = 0, rect width will be adjusted to fit entire text
void mwWidget::mDropDownDrawMain(mRect<int> f, int r, std::string text, int text_just, int btype, int bcol, int fcol, bool crop)
{
   // triangle width, height
   float tr_w = 6;
   float tr_h = tr_w * 0.8660;

   // spacing on either side of triangle
   int tr_xsp1 = 5;
   int tr_xsp2 = 7;

   // triangle full width (triangle width + spacing)
   int tr_fw = tr_w + tr_xsp1 + tr_xsp2;

   // resize box to fit string
   if (crop == false)
   {
      // get width of text
      int text_width = (int)text.size() * 8;

      // get custom pad value based on widget width mod 8
      int xm = (f.w) % 8;
      int pd = xm + 14;
      if (pd > 15) pd-=8;

      int pad = tr_fw + pd;

      // get current box width
      int box_width = (f.w) - pad; // full width - pad

      // check if box needs to be enlarged
      if (text_width > box_width)
      {
         box_width = text_width + pad; // text width + pad
//         x2 = x1 + box_width;
         f.setWidth(box_width);
      }
   }

   // triangle absolute position
   float tr_x1 = f.x2 - tr_w - tr_xsp2;
   float tr_y1 = f.y1 + (f.h-tr_h) / 2;

   // end of the space remaining for the text area (start of the triangle area)
   int x3 = f.x2 - tr_fw;

   // resize string to fit box
   if (crop == true)
   {
      // width of the text area (in char)
      int xw3 = ((x3 - f.x1) / 8) - 1;

      // crop text
      if (((int)text.size()) > xw3) text.resize(xw3);

   }

   // draw background
   if (btype == 0) al_draw_filled_rectangle(f.x1-1, f.y1-1, f.x2+1, f.y2+1, mColor.pc[0]); // draw nothing
   if (btype == 1) al_draw_filled_rounded_rectangle(f.x1, f.y1, f.x2, f.y2, r, r, mColor.pc[bcol]); // solid color
   if (btype == 2) draw_widget_area(f.x1, f.y1, f.x2, f.y2, bcol); // draw button frame




   // draw frame
   al_draw_rounded_rectangle(f.x1, f.y1, f.x2, f.y2, r, r, mColor.pc[fcol], 1);

   // draw triangle
   al_draw_filled_triangle(tr_x1, tr_y1,   tr_x1+tr_w, tr_y1,  tr_x1+tr_w/2, tr_y1+tr_h,    mColor.pc[15]);

   // draw line to mark edge of triangle area
   //al_draw_line(x3, f.y1,    x3, f.y2,    mColor.pc[15], 1);

   // draw text
   int text_x1 = f.x1 + 4;
   if (text_just == 1) text_x1 = (f.x1 + x3) / 2;
   float text_y1 = f.y1 + (f.h-8)/2;
   mMiscFnx.mw_draw_text(15, text_x1, text_y1, text_just, text);

}


/* mDropDown widget
shows current value and allows change by selecting from drop down list
requires 2 actions:
- mouse click to activate dropDown list
- another click to select from list or cancel
blocks when list is active

when blocking:
- hovered list item is highlighted
- mouse b1 on list item sets var to hovered item
- blocking will cancel with:
-- ESC
-- mouse b2
-- mouse b1 not on list

listItems is a vector of listItem structs, each has integer value and string
current value is var
if var is not a valid entry in listItems, 'invalid' will be displayed
returns true if var is changed

current text value is cropped to fix widget area
on hover, widget area is expanded to fix entire text value

on hover, arrow keys UP, DOWN, increment and decrement var


btype = 0 - single color background
btype = 1 - regular faded color frame
bcol - background color

fcol - frame color
hcol - frame color when hovered

tjust = 0 - text left justify
tjust = 1 - text center

*/


bool mwWidget::mDropDown(int xType, int xa, int xb, int yType, int ya, int yb, int r, int tjust, int btype, int bcol, int fcol, int hcol, std::vector<listItem> listItems, int & var, int d)
{
   // get mRect from parameters
   mRect<int> f = xyHelper(xType, xa, xb, yType, ya, yb, "");

   // find the listItem that matches var
   std::string text = "invalid";
   for (const auto& li : listItems)
      if (li.value == var) text = li.text.c_str();

   mDropDownDrawMain(f, r, text, tjust, btype, bcol, fcol, 1);

   if (d) return false; // input disabled

   // check if mouse is on widget
   if (f.contains(mInput.mouse_x, mInput.mouse_y))
   {
      mDropDownDrawMain(f, r, text, tjust, btype, bcol, hcol, 0);

      if (mInput.key[ALLEGRO_KEY_UP][0])
      {
         while (mInput.key[ALLEGRO_KEY_UP][0]) mEventQueue.proc(1);
         mDropDownSetPrevItem(listItems, var, 1);
         return true;
      }

      if (mInput.key[ALLEGRO_KEY_DOWN][0])
      {
         while (mInput.key[ALLEGRO_KEY_DOWN][0]) mEventQueue.proc(1);
         mDropDownSetNextItem(listItems, var, 1);
         return true;
      }


      if (mInput.mouse_b[1][0])
      {
         while (mInput.mouse_b[1][0]) mEventQueue.proc(1); // wait for release

         int ls = (int)listItems.size(); // list size

         // get the length of the longest line in lise
         int max_line_length = 0;
         for (int i=0; i<ls; i++)
         {
            int len = (int) listItems[i].text.size() * 8;
            if (len > max_line_length) max_line_length = len;
         }


         // line x positions

         // line width
         int lw = max_line_length + 8;

         // start of line is x1 by default
         int lx1 = f.x1;

         // offest lx1 if text is centered
         if (tjust) lx1 = f.x1 + f.w/2 - lw / 2;

         // end of line
         int lx2 = lx1 + lw;

         // line y positions
         int lih = f.h; // list item height
         int ly1 = f.y2 + 1;
         int ly2 = ly1 + (ls * lih);

         // make a mRect for list area
//         mRect<int> listRect(lx1, ly1, lx2, ly2, -999);

         mRect<int> listRect = mRect<int>::fromX1Y1X2Y2(lx1, ly1, lx2, ly2);


         // text y position
         int tyo = (lih-8)/2;

         // text x positions
         int txo = f.x1+4;
         if (tjust) txo = (lx1+lx2)/2;


         while (1)
         {
            mEventQueue.proc(1);
            al_flip_display();

            // get background color
            int col = bcol+256; // force color past end
            while (col>239) col-=16; // reduce until less than 240 (this should get second most faded color)
            if (bcol == 0) col = 0;

            // erase area 1 pixel bigger
            listRect.draw_filled_rectangle(mColor.pc[col], 1);

            // frame area
            listRect.draw_rectangle(mColor.pc[15], 1);

            // draw list and process mouse on list item
            bool mouse_on_list = false;
            for (int i=0; i<ls; i++)
            {
               // make a mRect for line area
//               mRect<int> lineRect(lx1, ly1 + (i*lih), lx2, ly1 + (i*lih) + lih, -999);

               mRect<int> lineRect = mRect<int>::fromX1Y1X2Y2(lx1, ly1 + (i*lih), lx2, ly1 + (i*lih) + lih);



               mMiscFnx.mw_draw_text(15, txo, lineRect.y1 + tyo, tjust, listItems[i].text);
               al_draw_line(lineRect.x1, lineRect.y2, lineRect.x2, lineRect.y2, mColor.pc[15+128], 0);

               // is mouse on line
               if (lineRect.contains(mInput.mouse_x, mInput.mouse_y))
               {
                  mouse_on_list = true;

                  // show highlight of item in list
                  lineRect.draw_rectangle(mColor.pc[10], 1);

                  // set as selected
                  int sel = listItems[i].value;
                  if (mInput.mouse_b[1][0])
                  {
                     while (mInput.mouse_b[1][0]) mEventQueue.proc(1); // wait for release
                     var = sel;
                     return true;
                  }
               }
            }

            // mouse b1 click not on list cancels
            if ((mouse_on_list == false) && (mInput.mouse_b[1][0]))
            {
               while (mInput.mouse_b[1][0]) mEventQueue.proc(1);
               return false;
            }
            // mouse b2 click cancels
            if (mInput.mouse_b[2][0])
            {
               while (mInput.mouse_b[2][0]) mEventQueue.proc(1);
               return false;
            }
            // escape cancels
            if (mInput.key[ALLEGRO_KEY_ESCAPE][0])
            {
               while (mInput.key[ALLEGRO_KEY_ESCAPE][0]) mEventQueue.proc(1);
               return false;
            }

         } // end of blocking loop
      } // end of mouse click on widget
   } // end of mouse on widget
   return false;
}
