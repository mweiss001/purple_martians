// mwListBox.cpp

#include "pm.h"
#include "mwListBox.h"
#include "mwColor.h"
#include "mwDisplay.h"
#include "mwEventQueue.h"
#include "mwFont.h"
#include "mwInput.h"
#include "mwLoop.h"


void mwListBox::init()
{
   list.clear();
   list_start_index = 0;
   selected_item = -1;
   selection_changed = false;
}


void mwListBox::draw()
{
   line_padding = (line_height - 8) / 2;

   al_set_target_backbuffer(mDisplay.display);


   // clear and frame
   rect.draw_filled_rectangle(mColor.pc[color+224]);
   rect.draw_rectangle(mColor.pc[color], 1);

   // by default take up all the space
   list_rect = rect;

   num_items = list.size();

   // determine number of items shown from rect height and line height
   num_items_shown = (rect.h - line_padding * 2) / line_height;

   // enforce bounds
   set_list_start_index(list_start_index);


   // show scroll bar
   if (num_items > num_items_shown)
   {
      // make the list rect smaller to not include the scrollbar
      list_rect.setX1Y1X2Y2(rect.x1, rect.y1, rect.x2-10, rect.y2);

      // scrollbar frame rect
      mwRect<float> scrollbar_frame = mwRect<float>::fromX1Y1X2Y2(rect.x2-10, rect.y1, rect.x2, rect.y2);

      // draw frame
      scrollbar_frame.draw_rectangle(mColor.White, 1);

      // bar height is frame height * items shown / num items
      float bar_h = scrollbar_frame.h * (float)num_items_shown / (float)num_items;

      // bar y1 is frame height * start index / num items
      float bar_y1 = scrollbar_frame.h * (float)list_start_index / (float)num_items;

      // scrollbar bar rect
      mwRect<float> scrollbar_bar = mwRect<float>::fromX1Y1WH(scrollbar_frame.x1, scrollbar_frame.y1+bar_y1, scrollbar_frame.w, bar_h);

      // draw scrollbar bar
      scrollbar_bar.draw_filled_rectangle(mColor.pc[15+128], -1);


      // is mouse pressed on scrollbar bar
      if (scrollbar_bar.contains(mInput.mouse_x, mInput.mouse_y) && mInput.mouse_b[1][0])
      {
         // capture initial mouse click position
         int my = mInput.mouse_y;


         while (mInput.mouse_b[1][0])
         {
            mEventQueue.proc(1);

            int dy = mInput.mouse_y - my;

            if (dy)
            {
               my = mInput.mouse_y;

               // adjust bar y1 from mouse move
               scrollbar_bar.y1 += dy;

               // clamp if less than scrollbar frame
               if (scrollbar_bar.y1 < scrollbar_frame.y1) scrollbar_bar.y1 = scrollbar_frame.y1;

               // set bar y2 from y1 and height
               scrollbar_bar.y2 = scrollbar_bar.y1 + bar_h;

               // if scrollbar y2 is past bottom of frame, adjust so y2 lines up with bottom of frame
               if (scrollbar_bar.y2 > scrollbar_frame.y2)
               {
                  scrollbar_bar.y1 = scrollbar_frame.y2 - bar_h;
                  scrollbar_bar.y2 = scrollbar_frame.y2;
               }


               // convert bar y1 to index
               float rr = (scrollbar_bar.y1 - scrollbar_frame.y1) / scrollbar_frame.h;

               // set new start index
               set_list_start_index(num_items * rr);

               draw_list();

               scrollbar_frame.draw_filled_rectangle(mColor.Black);
               scrollbar_frame.draw_rectangle(mColor.White, 1);
               scrollbar_bar.draw_filled_rectangle(mColor.pc[15+128], -1);

               al_flip_display();

            }
         }
      }
   }

   draw_list();

   if (mInput.key[ALLEGRO_KEY_DOWN][3]) set_selected_item(selected_item+1);
   if (mInput.key[ALLEGRO_KEY_UP][3]) set_selected_item(selected_item-1);
   if (mInput.key[ALLEGRO_KEY_PGUP][3]) set_selected_item(0);
   if (mInput.key[ALLEGRO_KEY_PGDN][3]) set_selected_item(num_items-1);

   // detect mouse on list
   hover_item = -1;

   if (list_rect.contains(mInput.mouse_x, mInput.mouse_y))
   {
      // get line offset into list
      hover_item = (mInput.mouse_y - list_rect.y1 + line_padding) / line_height;

      // if mouse click, set selected item
      if (mInput.mouse_b[1][3]) set_selected_item(list_start_index + hover_item);
   }

   // mouse scroll wheel
   if (mInput.mouse_dz)
   {
      set_selected_item(selected_item - mInput.mouse_dz);
      mInput.mouse_dz = 0;
   }

}


void mwListBox::set_selected_item(int index)
{
   int old_selected_item = selected_item;
   selected_item = index;

   if (selected_item < 0) selected_item = 0;
   if (selected_item > num_items -1) selected_item = num_items -1;

   if (selected_item < list_start_index) set_list_start_index(list_start_index-1);
   if (selected_item > list_last_index) set_list_start_index(list_start_index+1);

   if (old_selected_item != selected_item) selection_changed = true;
}

void mwListBox::set_list_start_index(int index)
{
   list_start_index = index;

   if (list_start_index < 0) list_start_index = 0;

   // get index of last item shown
   list_last_index = list_start_index + num_items_shown;

   // adjust if past end of list
   if (list_last_index > num_items - 1) list_last_index = num_items - 1;
}


void mwListBox::draw_list()
{
   list_rect.set_clipping_rectangle(mDisplay.display_transform_double);

   int ya = list_rect.y1+line_padding;
   for (int i=list_start_index; i<=list_last_index; i++)
   {
      al_draw_textf(mFont.pr8, mColor.pc[15], list_rect.x1+4, ya, 0, "%s",  list[i].c_str());
      ya+=line_height;
   }

   // frame selected line
   frame_line(selected_item-list_start_index, 11);

   // frame hover item line
   frame_line(hover_item, 14);

//   if (hover_item != -1) frame_line(hover_item, 14);


   al_reset_clipping_rectangle();
}

void mwListBox::frame_line(int line, int color)
{
   if (line>=0 && line < num_items)
   {
      int ya = list_rect.y1 + line_padding + line * line_height - line_padding;
      int yb = ya + 8 + line_padding*2;
      al_draw_rectangle(list_rect.x1+1, ya, list_rect.x2-1, yb, mColor.pc[color], 1);
   }
}
