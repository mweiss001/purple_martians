// mwItemMessage.cpp

#include "pm.h"
#include "mwItem.h"
#include "mwPlayer.h"
#include "mwFont.h"
#include "mwBitmap.h"
#include "mwWidget.h"
#include "mwColor.h"
#include "mwDisplay.h"
#include "mwLoop.h"
#include "mwMiscFnx.h"
#include "mwWindowManager.h"
#include "mwEventQueue.h"
#include "mwInput.h"
#include "mwTriggerEvent.h"
#include "mwLevel.h"

void mwItem::proc_pmsg_collision(int i)
{
   if (item[i][2] & PM_ITEM_PMSG_SHOW_SCROLL) proc_pmsg_reset_timer(i);
}





void mwItem::proc_pmsg_reset_timer(int i)
{
   int timer_count=0, timer_val=0;
   mMiscFnx.get_int_3216(mItem.item[i][12], timer_count, timer_val);
   timer_count = timer_val;
   mMiscFnx.set_int_3216(mItem.item[i][12], timer_count, timer_val);
}

void mwItem::proc_pmsg(int i)
{
   if (!(mItem.item[i][2] & PM_ITEM_PMSG_SHOW_ALWAYS)) // only if not show always
   {
      // count down the timer
      int timer_count=0, timer_val=0;
      mMiscFnx.get_int_3216(mItem.item[i][12], timer_count, timer_val);
      if (timer_count > 0) timer_count--;
      mMiscFnx.set_int_3216(mItem.item[i][12], timer_count, timer_val);

      // check for event trigger
      if ((item[i][1]) && (mTriggerEvent.event[item[i][1]])) proc_pmsg_reset_timer(i);
   }
}

typedef struct DRAW_CUSTOM_LINE_EXTRA
{
   int num_lines;
} DRAW_CUSTOM_LINE_EXTRA;

static bool draw_multiline_cb(int line_num, const char *line, int size, void *extra)
{
   DRAW_CUSTOM_LINE_EXTRA *s = (DRAW_CUSTOM_LINE_EXTRA *) extra;
   s->num_lines++;
   return 1;
}

int mwItem::draw_message(int i, int x, int y, int custom)
{
   char msg[1024];
   if (!custom)
   {
      int timer_count=0, timer_val=0;
      mMiscFnx.get_int_3216(item[i][12], timer_count, timer_val);
      // if timer running or always show, draw the message
      if ((timer_count) || (item[i][2] & PM_ITEM_PMSG_SHOW_ALWAYS)) draw_pop_message(i, 0, 0, 0, 0, 0, msg);
   }


   // if show scroll or level editor running draw scroll
   int show_scroll = 0;

   if (item[i][2] & PM_ITEM_PMSG_SHOW_SCROLL) show_scroll = 1;
   if (item[i][2] & PM_ITEM_PMSG_SHOW_ALWAYS) show_scroll = 0;
   if (mLoop.level_editor_running) show_scroll = 1;
   if (show_scroll) al_draw_bitmap(mBitmap.tile[mBitmap.zz[0][36]], x, y, 0);

   return 1;
}

void mwItem::draw_pop_message(int i, int custom, int xpos_c, int ypos, int cursor_pos, int cursor_blink, char *f)
{
   char msg[1024];

   // set where we will draw
   if (custom) al_set_target_backbuffer(mDisplay.display);
   else al_set_target_bitmap(mBitmap.level_buffer);

   // make a copy of the string
   char pt[500];

   if (custom) // get text from f
   {
      if (strlen(f) > 0) strcpy(pt, f);
      else sprintf(pt, "<empty>");
   }
   else strcpy(pt, pmsgtext[i]); // get text from pmsg

   int tc=0, fc=0;
   mMiscFnx.get_int_3216(item[i][13], tc, fc); // get text and frame colors


   int frame_width = get_frame_size(i);

   // hijack for special stats messages
   int show_stats_msg = 0;
   if ((!strncmp(mItem.pmsgtext[i], "Level Statistics Full",            21)) && (!custom)) show_stats_msg = 1;
   if ((!strncmp(mItem.pmsgtext[i], "Level Statistics Game",            21)) && (!custom)) show_stats_msg = 2;
   if ((!strncmp(mItem.pmsgtext[i], "Level Statistics Overworld All",   30)) && (!custom)) show_stats_msg = 3;
   if ((!strncmp(mItem.pmsgtext[i], "Level Statistics Overworld Train", 30)) && (!custom)) show_stats_msg = 4;
   if ((!strncmp(mItem.pmsgtext[i], "Level Statistics Overworld Main",  30)) && (!custom)) show_stats_msg = 5;
   if (show_stats_msg) mLevel.show_level_stats(0, 0, 0, frame_width, item[i][8], item[i][9], 0, show_stats_msg); // only set w and h


   int x1 = item[i][6];
   int y1 = item[i][7];
   int w  = item[i][8];
   int h  = item[i][9];




   if (custom) // get custom x and y
   {
      x1 = xpos_c - w/2 - 8;
      y1 = ypos - 10;
   }

   // other x and y positions
   int x2 = x1 + w;
   int xc = x1 + w / 2;
   int y2 = y1 + h;



   if (frame_width == 0)
   {
      if (mLoop.level_editor_running) al_draw_rectangle(x1, y1, x2, y2, mColor.pc[15], 1);

      if (show_stats_msg) al_draw_filled_rectangle(x1, y1, x2, y2, mColor.pc[fc+13*16]);  // background


   }
   else al_draw_filled_rectangle(x1, y1, x2, y2, mColor.pc[fc+13*16]);  // background


   if ((mLoop.eco_draw) && (frame_width > 1)) frame_width = 1;

   if (frame_width == 1)
   {
      al_draw_rectangle(x1, y1, x2, y2, mColor.pc[fc], 1);
   }


   if (frame_width == 2)
   {
      for (int a=0; a<frame_width; a++)
         al_draw_rounded_rectangle(x1+a, y1+a, x2-a, y2-a, 4, 4, mColor.pc[fc+a*128], 1.5);
   }
   if (frame_width == 4)
   {
      for (int a=0; a<frame_width; a++)
         al_draw_rounded_rectangle(x1+a, y1+a, x2-a, y2-a, 4, 4, mColor.pc[fc+a*48], 1.5);
   }
   if (frame_width == 12)
   {
      for (int a=0; a<frame_width; a++)
         al_draw_rounded_rectangle(x1+a, y1+a, x2-a, y2-a, 6, 6, mColor.pc[fc+a*16], 1.5);
   }


   if (show_stats_msg) mLevel.show_level_stats(x1, y1, x2, frame_width, item[i][8], item[i][9], 1, show_stats_msg);
   else
   {

      //al_draw_rounded_rectangle(x1+frame_width, y1+frame_width, x2-frame_width, y2-frame_width, 4, 4, mColor.pc[15], 1); // debug show inner frame

      if (custom)
      {

         // show cursor char
         sprintf(msg, "%c", pt[cursor_pos]);
         if (pt[cursor_pos] == 0)  sprintf(msg, "NULL");
         if (pt[cursor_pos] == 10) sprintf(msg, "LF");
         if (pt[cursor_pos] == 32) sprintf(msg, "SPACE");

         al_draw_textf(mFont.pr8, mColor.pc[15], xc+4, y2+2, ALLEGRO_ALIGN_CENTRE, "%d/%d/500 [%s] ", cursor_pos, (int) strlen(pt), msg);
   //      al_draw_textf(mFont.pr8, mColor.pc[15], xc+4, y1+2, ALLEGRO_ALIGN_CENTRE, "[%s] %d/%d/500", msg, cursor_pos, (int) strlen(pt));
   //      al_draw_textf(mFont.pr8, mColor.pc[15], x2-60, y2-9, 0, "[%s]", msg);
         //al_draw_textf(mFont.pr8, mColor.pc[15], xc+4, y1-20, ALLEGRO_ALIGN_CENTRE, "x:%d y:%d w:%d h:%d", x1, y1, w, h);
      }


      // figure out what line height to use so that text is justified vertically
      float line_height = 7.9; // initial line height

      int max_text_width = w - 16;

      // do_multiline callback to figure out number of lines that will be used
      DRAW_CUSTOM_LINE_EXTRA extra;
      extra.num_lines = 0;
      al_do_multiline_text(mFont.pr8, max_text_width, pt, draw_multiline_cb, (void *)&extra);

      float sp=0; // space between text_height and frame_height
      float text_height = 0;

      do
      {
         line_height+=0.1;
         text_height = extra.num_lines * line_height;
         sp = h - frame_width*2 - text_height;

      } while ((sp > 2) && (line_height < 100));

   //   al_draw_textf(mFont.pr8, mColor.pc[15], xc+4, y2+20, ALLEGRO_ALIGN_CENTRE, "lh:%2.1f nl:%d th:%2.1f", line_height, extra.num_lines, text_height);
   //   al_draw_textf(mFont.pr8, mColor.pc[15], xc+4, y2+28, ALLEGRO_ALIGN_CENTRE, "fh:%d fh-fw:%d sp:%2.1f", h, h - frame_width*2, sp);

      float y3 = y1+frame_width+line_height/2-3.5;

      if (sp < 1)
      {
         sp = h - text_height;
         y3 = y1+sp/2;

      }

      al_draw_multiline_text(mFont.pr8, mColor.pc[tc], xc, y3, max_text_width, line_height, ALLEGRO_ALIGN_CENTRE, pt);

      if (cursor_blink)
      {
         int cursor_char = 127;

         // convert all printable char to blank non-printable spaces (can't use space because multiline text will break in new places)
         for (int a=0; a<(int)strlen(pt); a++)
            if ((pt[a] > 32) && (pt[a] < 126)) pt[a] = 96; // alternate space

         // check if cursor is on special char
         int cursor_on_special = 0;
         if ((pt[cursor_pos] == 10) || (pt[cursor_pos] == 32))
         {
            int dcp = cursor_pos;
            cursor_on_special = 1;
            while ( (dcp > 0) &&  ((pt[dcp] == 10) || (pt[dcp] == 32)) ) dcp--; // find previous non special char
            pt[dcp] = cursor_char;
         }
         else pt[cursor_pos] = cursor_char;

         al_draw_multiline_text(mFont.pr8, mColor.pc[10], xc+cursor_on_special*8, y3, max_text_width, line_height,  ALLEGRO_ALIGN_CENTRE, pt);
      }

      // crosshairs for alignment
//      al_draw_line(x1, y1, x2, y2, mColor.pc[fc], 1);
//      al_draw_line(x1, y2, x2, y1, mColor.pc[fc], 1);
//      al_draw_line(x1+w/2, y1,     x1+w/2, y2,     mColor.pc[fc], 1);
//      al_draw_line(x1,     y1+h/2, x2,       y1+h/2, mColor.pc[fc], 1);
   }
}

















int mwItem::edit_pmsg_text(int c, int new_msg)
{
   int bts = 16;
   int char_count;
   int cursor_pos=0;
   int blink_count = 6;
   int blink_counter = 0;
   char f[1800];
   int quit = 0;

   int xa = mWM.mW[7].x1;
   int xb = mWM.mW[7].x2;

   int smx = (xa+xb)/2;  // x center
   int smy = mWM.mW[7].pop_msg_viewer_pos;

   int bad=0;

   if (new_msg)
   {
      smy = 200;
      f[0] = (char)NULL;
      f[1] = (char)NULL;
      char_count = 1;
   }
   else
   {
      strcpy(f, pmsgtext[c]);
      char_count = strlen(f);
   }

   while (!quit)
   {
      al_set_target_backbuffer(mDisplay.display);

   //   printf("bc:%d\n", blink_counter);

      if (++blink_counter > blink_count) blink_counter = 0;
      if (blink_counter > (blink_count/2)) draw_pop_message(c, 1, smx, smy, cursor_pos, 1, f); // show the message with cursor_pos
      else                                 draw_pop_message(c, 1, smx, smy, cursor_pos, 0, f); // show the message without

      int by = smy-bts/2-2;
      int ey = by+-3*bts; // erase y1
      int by1 = ey;

      mWidget.buttont(    xa, by1, xb, bts, 0,0,0,0,  0,15,13,0, 1,0,1,1, "Edit Message"); // display text only
      if (mWidget.buttont(xa, by1, xb, bts, 0,0,0,0,  0,11,15,0, 1,0,1,0, "OK"))     { quit = 1; bad = 0; }
      if (mWidget.buttont(xa, by1, xb, bts, 0,0,0,0,  0,10,15,0, 1,0,1,0, "Cancel")) { quit = 1; bad = 1; }


//      al_draw_filled_rectangle(xa, by1-30, xb, by1-10, mColor.pc[0]);
//      al_draw_rectangle(xa, by1-30, xb, by1-10, mColor.pc[15], 1);
//      al_draw_textf(mFont.pr8, mColor.pc[15], xa, by1-20, 0, "%d/%d %d", cursor_pos, char_count, strlen(f));

      mEventQueue.proc();

      if (mInput.key[ALLEGRO_KEY_HOME][0])  cursor_pos = 0;
      if (mInput.key[ALLEGRO_KEY_END][0])   cursor_pos = char_count;
      if (mInput.key[ALLEGRO_KEY_RIGHT][3])
      {
         if (mInput.SHFT() && mInput.CTRL())
         {
            while ((++cursor_pos < char_count) && (f[cursor_pos] != 10));  // find next LF
         }
         else if (mInput.SHFT())
         {
            while ((++cursor_pos < char_count) && (f[cursor_pos] != 32) && (f[cursor_pos] != 10));  // find next space or LF
         }
         else if (mInput.CTRL()) cursor_pos+=16;

         else cursor_pos++;
         if (cursor_pos > char_count) cursor_pos = char_count;          // make sure we are not past the end
      }

      if (mInput.key[ALLEGRO_KEY_LEFT][3])
      {
         while (mInput.key[ALLEGRO_KEY_LEFT][3]) mEventQueue.proc();    // wait for release

         if ((mInput.SHFT()) && (mInput.CTRL()))
         {
            while ((--cursor_pos > 0) && (f[cursor_pos] != 10));        // find next LF
         }
         else if (mInput.SHFT())
         {
            while ((--cursor_pos > 0) && (f[cursor_pos] != 32) && (f[cursor_pos] != 10)); // find next space or LF
         }
         else if (mInput.CTRL()) cursor_pos-=16;

         else cursor_pos--;
         if (cursor_pos < 0) cursor_pos = 0;                            // make sure we are not before the start
      }


      if (mInput.key[ALLEGRO_KEY_UP][3])                                // move up one line
      {
         int ocp = cursor_pos;                                          // get original position
         while ((--cursor_pos > 0) && (f[cursor_pos] != 10));           // find previous LF
         int mv = cursor_pos - ocp;                                     // how far did we move?
         while ((--cursor_pos > 0) && (f[cursor_pos] != 10));           // find previous LF
         cursor_pos -= mv;                                              // subtract move
         if (cursor_pos < 0) cursor_pos = 0;                            // make sure we are not before the start
      }
      if (mInput.key[ALLEGRO_KEY_DOWN][3])                              // move down one line
      {
         int ocp = cursor_pos;                                          // get original position
         while ((++cursor_pos < char_count) && (f[cursor_pos] != 10));  // find next LF
         int mv = cursor_pos - ocp;                                     // how far did we move?
         while ((++cursor_pos < char_count) && (f[cursor_pos] != 10));  // find next LF
         cursor_pos -= mv;                                              // subtract move
         if (cursor_pos > char_count) cursor_pos = char_count;          // make sure we are not past the end
      }
      if ((mInput.key[ALLEGRO_KEY_DELETE][0]) && (cursor_pos < char_count))
      {
         al_rest(0.02);
         for (int aa = cursor_pos; aa < char_count; aa++) f[aa]=f[aa+1];
         char_count--;
         f[char_count] = (char)NULL; // set last to NULL
      }
      if ((mInput.key[ALLEGRO_KEY_BACKSPACE][0]) && (cursor_pos > 0))
      {
         al_rest(0.02);
         cursor_pos--;
         for (int aa = cursor_pos; aa < char_count; aa++) f[aa]=f[aa+1];
         char_count--;
         f[char_count] = (char)NULL; // set last to NULL
      }
      if (mInput.key_pressed_ASCII)
      {
         al_rest(0.07);
         int k = mInput.key_pressed_ASCII;
         if (k == 13) k = 10; // replace CR with LF

         if ( (k == 10) || ((k>31) && (k<127))) // if alphanumeric
         {
            for (int aa = char_count; aa>=cursor_pos; aa--) f[aa+1]=f[aa]; // move over to make room
            f[cursor_pos] = k; // set char
            cursor_pos++;      // inc both
            char_count++;
            f[char_count] = (char)NULL; // set last to NULL
         }
      }
      if (mInput.key[ALLEGRO_KEY_ESCAPE][3])
      {
         quit = 1;
         bad = 1;
      }
      al_flip_display();
      al_draw_filled_rectangle(xa, ey, xb, ey+10*bts, mColor.pc[0]);
      al_rest(0.07);

   } // end of while (!quit)

   if (bad) return 0;
   else
   {
      strcpy(pmsgtext[c], f);
      return 1;
   }
}





int mwItem::get_frame_size(int num)
{
   int f = item[num][2];
   if (f & PM_ITEM_PMSG_FRAME0)  return 0;
   if (f & PM_ITEM_PMSG_FRAME1)  return 1;
   if (f & PM_ITEM_PMSG_FRAME2)  return 2;
   if (f & PM_ITEM_PMSG_FRAME4)  return 4;
   if (f & PM_ITEM_PMSG_FRAME12) return 12;
   return 0;
}


void mwItem::set_frame_size(int num, int frame_size)
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




