// mwPDE.cpp

#include "pm.h"
#include "mwPDE.h"
#include "mwFont.h"
#include "mwBitmap.h"
#include "mwWidgets.h"
#include "mwColor.h"
#include "mwInput.h"
#include "mwEventQueue.h"



mwPDE mPDE;

int mwPDE::load()
{
   FILE *fp =fopen("bitmaps/pde.pm","rb");
   if (fp)
   {
      fread(PDEf,  sizeof(PDEf), 1, fp);
      fread(PDEi,  sizeof(PDEi),  1, fp);
      fread(PDEt,  sizeof(PDEt),  1, fp);
      fclose(fp);
      return 1;
   }
   mI.m_err("Error loading pde.pm");
   return 0;
}

void mwPDE::save()
{
   FILE *fp =fopen("bitmaps/pde.pm","wb");
   if (fp)
   {
      fwrite(PDEf, sizeof(PDEf), 1, fp);
      fwrite(PDEi, sizeof(PDEi), 1, fp);
      fwrite(PDEt, sizeof(PDEt), 1, fp);
      fclose(fp);
   }
   else mI.m_err("Error saving pde.pm");
}


void mwPDE::pde_swap(int s1, int s2)
{
   for (int y=0; y<32; y++)
   {
      int temp    = PDEi[s1][y];
      PDEi[s1][y] = PDEi[s2][y];
      PDEi[s2][y] = temp;
   }
   for (int y=0; y<16; y++)
   {
      float temp   = PDEf[s1][y];
      PDEf[s1][y] = PDEf[s2][y];
      PDEf[s2][y] = temp;
   }
   for (int y=0; y<20; y++)
   {
      char stemp[80];
      strcpy(stemp,       PDEt[s1][y] );
      strcpy(PDEt[s1][y], PDEt[s2][y] );
      strcpy(PDEt[s2][y], stemp);
   }
}


void mwPDE::pde_sort(void)
{
   // first just sort by type
   int swap_flag = 1;
   while (swap_flag)
   {
      swap_flag = 0;
      for (int x=0; x<99; x++)
      {
         if (PDEi[x][0] > PDEi[x+1][0]) // sort by type
         {
            pde_swap(x, x + 1);
            swap_flag++; // if any swaps
         }
      }
   }

   // then sort by text line
   swap_flag = 1;
   int do_swap = 0;
   while (swap_flag)
   {
      do_swap = 0;
      swap_flag = 0;
      for (int x=0; x<99; x++)
      {
         if ((PDEi[x][0] > 199) && ( PDEi[x][0] < 300))  // if both creators
            if ((PDEi[x+1][0] > 199) && ( PDEi[x+1][0] < 300))
               if (strncmp(PDEt[x][1], PDEt[x+1][1], strlen(PDEt[x][1])) > 0)
                  do_swap = 1;

         if ((PDEi[x][0] > 99) && ( PDEi[x][0] < 200))  // if both items
            if ((PDEi[x+1][0] > 99) && ( PDEi[x+1][0] < 200))
               if ( PDEi[x][0] == PDEi[x+1][0] ) // if same type
                  if (strncmp(PDEt[x][1], PDEt[x+1][1], strlen(PDEt[x][1]) ) > 0)  // secondary sort by text line 1
                     do_swap = 1;

         if ((PDEi[x][0] > 0) && ( PDEi[x][0] < 100))  // if both enemies
            if ((PDEi[x+1][0] > 0) && ( PDEi[x+1][0] < 100))
               if ( PDEi[x][0] == PDEi[x+1][0] ) // if same type
                  if (strncmp(PDEt[x][1], PDEt[x+1][1], strlen(PDEt[x][1]) ) > 0)  // secondary sort by text line 1
                     do_swap = 1;

         if (do_swap) // do the swap
         {
            do_swap = 0;
            swap_flag++; // if any swaps
            pde_swap(x, x + 1);
         } // end of swap
      } // end of for x
   } // end of while swap flag



   for (int x=0; x<50; x++) pde_swap(x, x + 50); // move all to + 50

   int insert_pos = 0; // creators to 0
   for (int x=50; x<100; x++)
   {
      int rt = PDEi[x][0];
      if (rt > 199) pde_swap(x, insert_pos++);
   }

   insert_pos = 32; // keys to 32
   for (int x=50; x<100; x++)
   {
      int rt = PDEi[x][0];
      if (rt == 104) pde_swap(x, insert_pos++);
   }


   insert_pos = 36; // switches to 36
   for (int x=50; x<100; x++)
   {
      int rt = PDEi[x][0];
      if (rt == 114) pde_swap(x, insert_pos++);
   }


   insert_pos = 16; // items to 16
   for (int x=50; x<100; x++)
   {
      int rt = PDEi[x][0];
      if ((rt > 99) && (rt < 200)) pde_swap(x, insert_pos++);
   }

   insert_pos = 48; // enemies to 48
   for (int x=50; x<100; x++)
   {
      int rt = PDEi[x][0];
      if ((rt) && (rt < 99)) pde_swap(x, insert_pos++);
   }






}




void mwPDE::edit_text(int e)
{
   char msg[1024];
   int line_length = 30;
   int tx = mI.mouse_x/8;
   int ty1 = (mI.mouse_y-20)/8;
   int quit = 0;
   while (!quit)
   {
      // redraw
      // erase background
      al_draw_filled_rectangle(0, 20, 256, 180, mC.pc[0]);
      al_draw_rectangle(0, 20, 256, 180, mC.pc[14], 1);
      al_draw_text(mF.pr8, mC.pc[14], 128, 12,  ALLEGRO_ALIGN_CENTER, "Text Edit Mode");

      // redraw all the text
      for (int x=0; x<20; x++)
         al_draw_text(mF.pr8, mC.pc[15], 0, 20+(x*8), 0, PDEt[e][x]);

      // mark the text entry position
      al_draw_filled_rectangle((tx*8), 20+(ty1*8), (tx*8)+8, 20+(ty1*8)+8, mC.pc[138]);
      msg[0] = PDEt[e][ty1][tx];
      if (msg[0] == (char)NULL) msg[0] = 32;
      msg[1] = (char)NULL;
      al_draw_text(mF.pr8, mC.pc[10], (tx*8), 20+(ty1*8), 0, msg);
      al_flip_display();


      mwEQ.proc_event_queue();
      int k = mI.key_pressed_ASCII;
      if ((k>31) && (k<127)) // insert if alphanumeric or return
      {
         int z = strlen(PDEt[e][ty1]);
         if (z > line_length) z = line_length;
         for (int x=z; x>tx; x--)
            PDEt[e][ty1][x] = PDEt[e][ty1][x-1];
         PDEt[e][ty1][tx] = k;
         if (++tx > line_length) // end of line?
         {
            PDEt[e][ty1][tx] = (char)NULL; // terminate the line
            ty1++;  // LF
            tx = 0; // CR
         }
      }

      if (mI.key[ALLEGRO_KEY_BACKSPACE][0])
      {
         while (mI.key[ALLEGRO_KEY_BACKSPACE][0]) mwEQ.proc_event_queue();
         if (--tx<0) tx = 0;
         int z = strlen(PDEt[e][ty1]);
         for (int x=tx; x<z; x++)
            PDEt[e][ty1][x] = PDEt[e][ty1][x+1];
      }
      if (mI.key[ALLEGRO_KEY_ENTER][3])
      {
         while (mI.key[ALLEGRO_KEY_ENTER][0]) mwEQ.proc_event_queue();
         for (int y=19; y>ty1; y--)  // slide all down
            strcpy(PDEt[e][y],PDEt[e][y-1]);
         if (strlen(PDEt[e][ty1]) == 999) // cursor past end of line
            PDEt[e][ty1+1][0] = (char)NULL; // next line empty
         if ((signed int)strlen(PDEt[e][ty1]) >= tx) // cursor not past end of line
         {
            for (int x=0; x <= 30-tx; x++)         // split line at tx
                PDEt[e][ty1+1][x] = PDEt[e][ty1+1][tx+x];

            PDEt[e][ty1][tx] = (char)NULL;  // terminate top line
            tx = 0;
            ty1++;
         }
      }
      if (mI.key[ALLEGRO_KEY_DELETE][0])
      {
         while (mI.key[ALLEGRO_KEY_DELETE][0]) mwEQ.proc_event_queue();
         if (PDEt[e][ty1][tx] == (char)NULL)
         {
            for (int x=0; x<=30-tx; x++) // get portion from line below
                PDEt[e][ty1][tx+x] = PDEt[e][ty1+1][x];
            for (int y=ty1+1; y<19; y++)  // slide all up
               strcpy(PDEt[e][y],PDEt[e][y+1]);
            PDEt[e][19][0] = (char)NULL; // last line empty
         }
         else
         {
            for (int x = tx; x < (int)strlen(PDEt[e][ty1]); x++)
               PDEt[e][ty1][x] = PDEt[e][ty1][x+1];
         }
      }
      if (mI.key[ALLEGRO_KEY_RIGHT][0])
      {
         while (mI.key[ALLEGRO_KEY_RIGHT][0]) mwEQ.proc_event_queue();
         if (++tx > line_length-1) tx = line_length-1;
      }
      if (mI.key[ALLEGRO_KEY_LEFT][0])
      {
         while (mI.key[ALLEGRO_KEY_LEFT][0]) mwEQ.proc_event_queue();
         if (--tx < 0) tx = 0;
      }
      if (mI.key[ALLEGRO_KEY_UP][0])
      {
         while (mI.key[ALLEGRO_KEY_UP][0]) mwEQ.proc_event_queue();
            if (--ty1 < 0) ty1 = 0;
      }
      if (mI.key[ALLEGRO_KEY_DOWN][0])
      {
         while (mI.key[ALLEGRO_KEY_DOWN][0]) mwEQ.proc_event_queue();
         if (++ty1 > 19) ty1 = 19;
      }


      if ((mI.mouse_b[1][0]) && (mI.mouse_x < 250) && (mI.mouse_y > 20) && (mI.mouse_y < 180))
      {
         ty1 = (mI.mouse_y-20)/8;
         tx = mI.mouse_x/8;
      }
      if (tx > (signed int)strlen(PDEt[e][ty1])) tx = strlen(PDEt[e][ty1]);

      while ((mI.key[ALLEGRO_KEY_ESCAPE][0]) || (mI.mouse_b[2][0]))
      {
         mwEQ.proc_event_queue();
         quit = 1;
      }
   }
}



void mwPDE::run(void)
{
   char msg[1024];
   while (mI.mouse_b[2][0]) mwEQ.proc_event_queue();
   if (load())
   {
      int e = 0, redraw = 1;

      int y5 = 10;
      int xa = 280;
      int xb = 380;
      int xc = 480;
      int xd = 580;

      int quit = 0;
      while(!quit)
      {
         if (redraw)
         {
            int a,b=0;
            int rt = PDEi[e][0];
            redraw = 0;

            y5 = 0;

            al_flip_display();
            al_clear_to_color(al_map_rgb(0,0,0));

            a = PDEi[e][1]; // bmp or ans
            if (a < NUM_SPRITES) b = a; // bmp
            if (a > 999) b = mwB.zz[5][a-1000]; // ans

            al_draw_bitmap(mwB.tile[b], 0,0,0);

            for (int x=0; x<20; x++)
               al_draw_text(mF.pr8, mC.pc[15], 0, 20+(x*8), 0, PDEt[e][x]);

            if (rt < 99)
            {
               al_draw_textf(mF.pr8, mC.pc[15], 40, 0, 0, "Predefined Enemy %d", e);
               for (int x=0; x<16; x++)
               {
                  sprintf(msg,"F[%d]:", x);
                  mdw_sliderf(xa, y5, xb-4, 16, 0,0,0,0,  0,12,15,15,  0,0,0,0, PDEf[e][x], 1000, 0, 1, msg);
                  sprintf(msg,"I[%d]:", x);
                  mdw_slideri(xb, y5, xc-4, 16, 0,0,0,0,  0,11,15,15,  0,0,0,0, PDEi[e][x], 1100, -2, 1, msg);
                  sprintf(msg,"I[%d]:", x+16);
                  mdw_slideri(xc, y5, xd-4, 16, 0,0,0,0,  0,11,15,15,  0,0,1,0, PDEi[e][x+16], 1000, 0, 1, msg);
               }
            }
            if ((rt > 99) && (rt < 200))
            {
               al_draw_textf(mF.pr8, mC.pc[15], 40, 0, 0, "Predefined Item %d", e);
               for (int x=0; x<16; x++)
               {
                  sprintf(msg,"I[%d]:", x);
                  mdw_slideri(xa, y5, xb, 16, 0,0,0,0,  0,11,15,15,  0,0,1,0, PDEi[e][x], 1100, -2, 1, msg);
               }
            }
            if (rt > 199)
            {
               al_draw_textf(mF.pr8, mC.pc[15], 40, 0, 0, "Special Creator %d", e);


               for (int x=0; x<16; x++)
               {
                  sprintf(msg,"F[%d]:", x);
                  mdw_sliderf(xa, y5, xb-4, 16, 0,0,0,0,  0,12,15,15,  0,0,0,0, PDEf[e][x], 1000, 0, 1, msg);
                  sprintf(msg,"I[%d]:", x);
                  mdw_slideri(xb, y5, xc-4, 16, 0,0,0,0,  0,11,15,15,  0,0,0,0, PDEi[e][x], 1100, -2, 1, msg);
                  sprintf(msg,"I[%d]:", x+16);
                  mdw_slideri(xc, y5, xd-4, 16, 0,0,0,0,  0,11,15,15,  0,0,1,0, PDEi[e][x+16], 1000, 0, 1, msg);
               }
            }
         }

         y5 = 200;
         if (mdw_buttont(100, y5, 160, 16, 0,0,0,0,    0,10,15,0, 1,0,1,0, "Save")) save();
         if (mdw_buttont(100, y5, 160, 16, 0,0,0,0,    0, 9,15,0, 1,0,1,0, "Load")) load();

         redraw=1;

         mwEQ.proc_event_queue();

         if (mI.CTRL() && mI.key[ALLEGRO_KEY_S][0]) // sort
         {
            while (mI.key[ALLEGRO_KEY_S][0]) mwEQ.proc_event_queue();
            pde_sort();
            redraw = 1;
         }

         if ((mI.mouse_b[1][0]) && (mI.mouse_x < 240) && (mI.mouse_y > 20) && (mI.mouse_y < 180)) edit_text(e);

         if (mI.key[ALLEGRO_KEY_RIGHT][0])
         {
            while (mI.key[ALLEGRO_KEY_RIGHT][0]) mwEQ.proc_event_queue();
            e +=1;
            if (e > 99) e = 99;
            redraw =1;
         }

         if (mI.key[ALLEGRO_KEY_LEFT][0])
         {
            while (mI.key[ALLEGRO_KEY_LEFT][0]) mwEQ.proc_event_queue();
            e -=1;
            if (e < 0) e = 0;
            redraw =1;
         }
         if (mI.key[ALLEGRO_KEY_PGUP][0])
         {
            while (mI.key[ALLEGRO_KEY_PGUP][0]) mwEQ.proc_event_queue();
            e +=10;
            if (e > 99) e = 99;
            redraw =1;
         }
         if (mI.key[ALLEGRO_KEY_PGDN][0])
         {
            while (mI.key[ALLEGRO_KEY_PGDN][0]) mwEQ.proc_event_queue();
            e -=10;
            if (e < 0) e = 0;
            redraw =1;
         }
         if (mI.CTRL() && mI.key[ALLEGRO_KEY_DELETE][0]) // DELETE PD
         {
            while (mI.key[ALLEGRO_KEY_DELETE][0]) mwEQ.proc_event_queue();

            for (int y=0; y<32; y++) PDEi[e][y] = 0;
            for (int y=0; y<16; y++) PDEf[e][y] = 0;
            for (int y=0; y<20; y++) strcpy(PDEt[e][y],"");
            PDEi[e][0] = 9999; // mark as empty
            redraw =1;
            //PDE_sort();
         }
         if (mI.key[ALLEGRO_KEY_ESCAPE][0])
         {
            while (mI.key[ALLEGRO_KEY_ESCAPE][0]) mwEQ.proc_event_queue();
            quit = 1;
         }
      }
   }
}

