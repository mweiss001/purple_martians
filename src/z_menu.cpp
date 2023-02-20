// z_menu.cpp
#include "pm.h"
#include "n_netgame.h"
#include "mwDisplay.h"
#include "mwFont.h"
#include "mwLift.h"
#include "mwColor.h"
#include "mwInput.h"
#include "mwEventQueue.h"
#include "z_screen.h"

char msg[1024];



// edit text stuff
void show_cursor(char *f, int cursor_pos, int xpos_c, int ypos, int cursor_color, int restore)
{
   int len = strlen(f);
   char dt[40][120];
   int row=0, col=0, cursor_row=0, cursor_col=0;
   // get cursor row and column and fill dt
   for (int a=0; a<len+1; a++)
   {
      if (a == cursor_pos)
      {
         cursor_row = row;
         cursor_col = col;
      }
      if (f[a] == 126) // line break
      {
         row++;
         col=0;
         dt[row][col] = (char)NULL; // in case len = 0
      }
      else  // regular char
      {
         dt[row][col] = f[a];
         col++;
         dt[row][col] = (char)NULL;
      }
   }

   // make a string from the cursor text char
   msg[0] = f[cursor_pos];
   msg[1] = 0;

   int x, y;
   x = cursor_col*8+xpos_c - strlen(dt[cursor_row])*4;
   y = ypos+cursor_row*8;

   if (restore) // black background, text color text
   {
      al_draw_filled_rectangle(x, y, x+8, y+8, mC.pc[0]);
      al_draw_text(mF.pr8, mC.pc[cursor_color], x, y, 0, msg);
   }
   else // red background, black text
   {
      al_draw_filled_rectangle(x, y, x+8, y+8, mC.pc[10]);
      al_draw_text(mF.pr8, mC.pc[0], x, y, 0, msg);
   }
}



void edit_server_name(int type, int x, int y)
{
   char fst[80];
   strcpy(fst, m_serveraddress);
   int char_count = strlen(fst);
   int cursor_pos=0;
   int old_cp=0;
   int blink_count = 3;
   int blink_counter = 0;
   while (mI.key[ALLEGRO_KEY_ENTER][0]) mwEQ.proc_event_queue();
   int quit = 0;
   while (!quit)
   {
      int tx = mwD.SCREEN_W/2;
      int ty1 = mwD.SCREEN_H/2;
      int w = (char_count+1)*4;

      if (type == 1)
      {
         tx = x;
         ty1 = y+20;

      }

      al_flip_display();
      // clear text background
      al_draw_filled_rectangle(tx-w-8, ty1-4-2, tx+w+18, ty1+4+3, mC.pc[0]);

      al_draw_text(mF.pr8, mC.pc[15], tx, ty1-14, ALLEGRO_ALIGN_CENTER, "Set Server IP or Hostname");
      // frame text
      al_draw_rectangle       (tx-w-1, ty1-4-1, tx+w+6, ty1+6, mC.pc[15], 1);

      rtextout_centre(mF.pr8, NULL, fst, tx, ty1+1, 15, 1, 1);

      if (blink_counter++ < blink_count) show_cursor(fst, cursor_pos, tx, ty1-3, 15, 0);
      else show_cursor(fst, cursor_pos, tx, ty1-3, 15, 1);
      if (blink_counter> blink_count*2) blink_counter = 0;

      if (cursor_pos != old_cp)
      {
         show_cursor(fst, old_cp, tx, ty1-3, 15, 1); // erase old blinking cursor if moved
         old_cp = cursor_pos;
         blink_counter = 0;
      }

      al_rest(.08);
      mwEQ.proc_event_queue();
      if (mI.key[ALLEGRO_KEY_RIGHT][0])
      {
         if (++cursor_pos > char_count) cursor_pos = char_count;
      }
      if (mI.key[ALLEGRO_KEY_LEFT][0])
      {
         if (--cursor_pos < 0) cursor_pos = 0;
      }
      if ((mI.key[ALLEGRO_KEY_DELETE][0]) && (cursor_pos < char_count))
      {
         for (int a = cursor_pos; a < char_count; a++)
           fst[a]=fst[a+1];
         --char_count;
         fst[char_count] = (char)NULL; // set last to NULL
      }
      if ((mI.key[ALLEGRO_KEY_BACKSPACE][0]) && (cursor_pos > 0))
      {
         cursor_pos--;
         for (int a = cursor_pos; a < char_count; a++)
           fst[a]=fst[a+1];
         char_count--;
         fst[char_count] = (char)NULL; // set last to NULL
      }

      int k = mI.key_pressed_ASCII;
      if ((k>31) && (k<127)) // insert if alphanumeric or return
      {
         // move over to make room
         for (int a = char_count; a>=cursor_pos; a--)
            fst[a+1]=fst[a];

         // set char
         fst[cursor_pos] = k;

         // inc both
         cursor_pos++;
         char_count++;

         fst[char_count] = (char)NULL; // set last to NULL
      }
      if (mI.key[ALLEGRO_KEY_ENTER][3])
      {
         strcpy(m_serveraddress, fst);
         quit = 1;
      }

      if (mI.key[ALLEGRO_KEY_ESCAPE][3]) quit = 1;

   }
}


int edit_lift_name(int lift, int y1, int x1, char *fst)
{
   int cursor_pos=0;
   int old_cp=0;
   int blink_count = 3;
   int blink_counter = 0;
   int a=0;
   int char_count = strlen(fst);
   while (1)
   {
      al_flip_display();
      al_rest(0.05);


      int x2 = x1 + Lift.cur[lift].w -1;
      int y2 = y1 + Lift.cur[lift].h -1;
      int tx = ((x1+x2)/2);
      int ty1 = ((y1+y2)/2) - 3;

      //int color = lifts[lift].color;

      int color = (Lift.stp[lift][0].type >> 28) & 15;


      // draw updated lift
      for (a=0; a<10; a++)
        al_draw_rounded_rectangle(x1+a, y1+a, x2-a, y2-a, 4, 4, mC.pc[color + ((9 - a)*16)], 2 );
      al_draw_filled_rectangle(x1+a, y1+a, x2-a, y2-a, mC.pc[color] );
      al_draw_text(mF.pr8, mC.pc[color+160], tx, ty1, ALLEGRO_ALIGN_CENTRE, fst);


      if (blink_counter++ < blink_count) show_cursor(fst, cursor_pos, tx, ty1, 15, 0);
      else show_cursor(fst, cursor_pos, tx, ty1, 15, 1);
      if (blink_counter> blink_count*2) blink_counter = 0;

      if (cursor_pos != old_cp)
      {
         show_cursor(fst, old_cp, tx, ty1, 15, 1); // erase old blinking cursor if moved
         old_cp = cursor_pos;
         blink_counter = 0;
      }

      mwEQ.proc_event_queue();
      if (mI.key[ALLEGRO_KEY_RIGHT][0])
      {
         if (++cursor_pos >= char_count) cursor_pos = char_count-1;
      }
      if (mI.key[ALLEGRO_KEY_LEFT][0])
      {
         if (--cursor_pos < 0) cursor_pos = 0;
      }
      if ((mI.key[ALLEGRO_KEY_DELETE][0]) && (cursor_pos < char_count))
      {
         for (a = cursor_pos; a < char_count; a++)
           fst[a]=fst[a+1];
         char_count--;
         // set last to NULL
         fst[char_count] = (char)NULL;
      }
      if ((mI.key[ALLEGRO_KEY_BACKSPACE][0]) && (cursor_pos > 0))
      {
         cursor_pos--;
         for (a = cursor_pos; a < char_count; a++)
           fst[a]=fst[a+1];
         char_count--;
         // set last to NULL
         fst[char_count] = (char)NULL;
      }

      int k = mI.key_pressed_ASCII;
      if ((k>31) && (k<127)) // insert if alphanumeric or return
      {
         // move over to make room
         for (a = char_count; a>=cursor_pos; a--)
            fst[a+1]=fst[a];

         // set char
         fst[cursor_pos] = k;

         // inc both
         cursor_pos++;
         char_count++;

         // set last to NULL
         fst[char_count] = (char)NULL;
      }
      if (mI.key[ALLEGRO_KEY_ENTER][3]) return 1;
      if (mI.key[ALLEGRO_KEY_ESCAPE][3]) return 0;
   }
}


