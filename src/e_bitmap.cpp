// e_bitmap.cpp (20100220 cleanup)
#include "pm.h"

int select_bitmap_ans(int zzindx)
{
   extern int bmp_index;
   int quit = 0;
   while (!quit)
   {
      proc_controllers();
      al_flip_display();
      al_clear_to_color(al_map_rgb(0,0,0));
      al_draw_text(font, palette_color[9], 0, 642, 0, "Select a Bitmap with b1");
      al_draw_text(font, palette_color[9], 0, 650, 0, "b2 or ESC to exit      ");

      // draw 32x32 bitmaps
      for (int y = 0; y < 32; y++)
         for (int x = 0; x < 32; x++)
            al_draw_bitmap(memory_bitmap[x+(y*32)],x*20, y*20, 0);
      al_draw_rectangle(0.5, 0.5, 640.5, 640.5, palette_color[13], 1);

      int xc = 180;
      int yc = 648;

      al_draw_textf(font, palette_color[14], 80, 666, 0, "Get Shape %d ", zz[4][zzindx] );

      sprintf(msg, "Current Sequence %d",zzindx);
      int l = 2+strlen(msg)*4;
      al_draw_text(font, palette_color[13], xc+150, yc+2, ALLEGRO_ALIGN_CENTER, msg);
      al_draw_rectangle(xc+150-l, yc+1, xc+150+l, yc+11, palette_color[13], 1);

      for (int c=0; c < zz[4][zzindx] + 1; c++)   // show current seq shapes
         if (( zz[5+c][zzindx] < NUM_SPRITES) && (zz[5+c][zzindx] > 0))
            al_draw_bitmap(memory_bitmap[ zz[5+c][zzindx] ], xc+1+c*20, yc+12, 0);
      al_draw_rectangle(xc+0.5, yc+11.5, xc+302.5, yc+32.5, palette_color[13], 1);


      if ((mouse_y < 640) && (mouse_x < 640))
      {
         int pointer = (mouse_x/20) + (mouse_y/20) * 32 ;
         al_draw_textf(font, palette_color[13], 522, 648, 0, "pointer %-2d", pointer );
         al_draw_bitmap(memory_bitmap[pointer], 620, 642, 0);

         al_draw_rectangle(518, 640.5, 640.5, 662.5, palette_color[13], 1);

         if (mouse_b1)
         {
            while (mouse_b1) proc_controllers();
            bmp_index = pointer;
            return 1;
         }
      }
      while ((key[ALLEGRO_KEY_ESCAPE]) || (mouse_b2))
      {
         proc_controllers();
         quit = 1;
      }
   }
   return -1;
}

int animation_proc()
{
   if (les != 1)
   {
      les = 1;
      rebuild_bitmaps();
   }


   extern int zzindx;
   extern int bmp_index;
   int pointer = zzindx;
   int as_quit = 0;
   int quit = 0;
   int c, x;


   int button_x1 = 40;
   int button_x2 = 260;
   int button_xc = button_x1 + (button_x2-button_x1)/2;
   int button_y = 240;
   int jh;


   al_set_target_backbuffer(display);
   al_show_mouse_cursor(display);

   while (!quit)
   {
      proc_controllers();
      al_flip_display();
      al_clear_to_color(al_map_rgb(0,0,0));

      sprintf(msg, "Animation Sequence Editor");
      int l = 2+strlen(msg)*4;
      al_draw_text(font, palette_color[9], 320, 20, ALLEGRO_ALIGN_CENTER, msg);
      al_draw_rectangle(320-l, 19, 320+l, 29, palette_color[9], 1);

      if (pointer != -1)
      {
         sprintf(msg, "Pointer %d", pointer);
         l = 2+strlen(msg)*8;
         al_draw_text(font, palette_color[9], 643.5 - l, 192, 0, msg);
         al_draw_rectangle(643.5-l-2, 191, 642.5, 201, palette_color[9], 1);
      }

      jh=1;
      al_draw_rectangle(button_x1, button_y+(jh*12), button_x2, button_y+(jh*12)+10, palette_color[15], 1);
      sprintf(msg,"Get New Shapes");
      al_draw_text(font, palette_color[13], button_xc, button_y+(jh*12)+1, ALLEGRO_ALIGN_CENTER, msg);

      jh=2;
      al_draw_rectangle(button_x1, button_y+(jh*12), button_x2, button_y+(jh*12)+10, palette_color[15], 1);
      sprintf(msg,"Passcount Delay %-2d  ",zz[3][zzindx]);
      al_draw_text(font, palette_color[13], button_xc, button_y+(jh*12)+1, ALLEGRO_ALIGN_CENTER, msg);

      jh=3;
      al_draw_rectangle(button_x1, button_y+(jh*12), button_x2, button_y+(jh*12)+10, palette_color[15], 1);
      sprintf(msg,"Edit Bitmap Attributes");
      al_draw_text(font, palette_color[13], button_xc, button_y+(jh*12)+1, ALLEGRO_ALIGN_CENTER, msg);

      jh=4;
      al_draw_rectangle(button_x1, button_y+(jh*12), button_x2, button_y+(jh*12)+10, palette_color[15], 1);
      sprintf(msg,"Copy Bitmaps");
      al_draw_text(font, palette_color[13], button_xc, button_y+(jh*12)+1, ALLEGRO_ALIGN_CENTER, msg);

      jh=5;
      al_draw_rectangle(button_x1, button_y+(jh*12), button_x2, button_y+(jh*12)+10, palette_color[15], 1);
      sprintf(msg,"Save Changes");
      al_draw_text(font, palette_color[13], button_xc, button_y+(jh*12)+1, ALLEGRO_ALIGN_CENTER, msg);


      sprintf(msg, "Current Sequence %d",zzindx);
      l = 2+strlen(msg)*4;
      al_draw_text(font, palette_color[13], 150, 202, ALLEGRO_ALIGN_CENTER, msg);
      al_draw_rectangle(150-l, 201, 150+l, 211, palette_color[13], 1);

      for (c = 0; c < zz[4][zzindx] + 1; c++)   // show current seq shapes
         if (( zz[5+c][zzindx] < NUM_SPRITES) && (zz[5+c][zzindx] > 0))
            al_draw_bitmap(memory_bitmap[ zz[5+c][zzindx] ], 1+c*20, 212, 0);
      al_draw_rectangle(0.5, 211.5, 302.5, 232.5, palette_color[13], 1);

      for (c=0; c < 32; c++)   // draw 32x8 grid of animation sequences
         for (x=0; x < 8; x++)
            if (zz[4][c + (x * 32)] != 0)
               if ((zz[0][c + (x * 32)] < NUM_SPRITES) && (zz[0][c + (x * 32)] > 0 ))
                  al_draw_bitmap(memory_bitmap[zz[0][c + (x * 32)]], 2+c*20, 30+x*20, 0);
      al_draw_rectangle(0.5, 29.5, 642.5, 190.5, palette_color[9], 1);


      // erase current sequence
      if (key[ALLEGRO_KEY_DELETE])
      {
         for (c=0;c<20;c++)
            zz[c][zzindx] = 0;
      }

      // if mouse on grid, get current animation sequence pointer
      if ((mouse_y > 30) && (mouse_y < 190) && (mouse_x < 642))
      {
         pointer = (mouse_x-2) / 20 + (mouse_y-30) / 20 * 32;
         if ((pointer < 0) || (pointer > NUM_ANS-1)) pointer = -1;
      }
      else pointer = -1;


      if (mouse_b1)
      {
         if (pointer != -1) zzindx = pointer; // mouse is on map and pointer is valid

         // edit delay
         if ( (mouse_y > 262) && (mouse_y < 272) && (mouse_x > 200) && (mouse_x < 220) )
            zz[3][zzindx] = edit_int(190, 265, zz[3][zzindx], 1, 0, 100);

         while (mouse_b1) proc_controllers(); // wait for release

         if ((mouse_x > button_x1) && (mouse_x < button_x2) && (mouse_y > button_y) && (mouse_y < button_y+(6*12) ))
         {
            int mb = (mouse_y - button_y) / 12;
            switch(mb)
            {
               case 1:
                  for (c=0;c<20;c++)
                     zz[c][zzindx] = 0;
                  as_quit = 0;
                  while (!as_quit)
                  {
                     x = select_bitmap_ans(zzindx);
                     if (x == 1) // good return b1
                     {
                        zz[5 + zz[4][zzindx]][zzindx] = bmp_index;
                        zz[4][zzindx]++; // set last shape to point at next
                     }
                     if (x == -1)  // abort esc
                     {
                        zz[4][zzindx]--;
                        as_quit=1;
                     }
                     if (zz[4][zzindx] > 14) // last shape
                     {
                        zz[4][zzindx]--;
                        as_quit=1;
                     }
                  }
               break;
               case 3: select_bitmap_proc(); break;
               case 4: copy_bitmap_proc(); break;
               case 5: save_sprit(); break;
            }
         }
      }


      update_animation();

      while ((key[ALLEGRO_KEY_ESCAPE]) || (mouse_b2))
      {
         proc_controllers();
         quit = 1;
      }

   }
   return 0;
}



int select_bitmap_proc()
{
   extern int bmp_index;
   int x, y;
   int quit = 0;
   int view_attrib = 0;
   int button_x1 = 400;
   int button_x2 = 639;
   int button_xc = 520;
   int button_y = 650;
   int jh;
   int b2mode = 0;

   al_set_target_backbuffer(display);
   al_show_mouse_cursor(display);

   while (!quit)
   {
      proc_controllers();
      al_flip_display();
      al_clear_to_color(al_map_rgb(0,0,0));
      al_draw_text(font, palette_color[9], 0, 700, 0, "Select a Bitmap with b1");

      // draw 32x32 bitmaps
      for (x = 0; x < 32; x++)
         for (y = 0; y < 32; y++)
         {
            al_draw_bitmap(memory_bitmap[x+(y*32)],x*20, y*20, 0);
            int color = 240; // normal color = white
            if (view_attrib)
            {
               if (sa[x+(y*32)][1]) color = 10; // locked color = red
               if (sa[x+(y*32)][0] == 0) sprintf(msg, "E");
               if (sa[x+(y*32)][0] == 1) sprintf(msg, "B");
               if (sa[x+(y*32)][0] == 2) sprintf(msg, "S");
               al_draw_text(font, palette_color[color], (x*20)+4, (y*20)+4, 0, msg);
            }
         }

      if ((mouse_y < 640) && (mouse_x < 640))
      {
         int pointer = (mouse_x/20) + (mouse_y/20) * 32 ;
         al_draw_textf(font, palette_color[4], 0, 680, 0, "pointer %-2d  ", pointer );
         al_draw_bitmap(memory_bitmap[pointer], 95, 671, 0);

         if (mouse_b1)
         {
            if (view_attrib)
            {
               int bx1 = mouse_x/20;
               int by1 = mouse_y/20;
               int bx2 = mouse_x/20;
               int by2 = mouse_y/20;
               int old_mouse_x = mouse_x;
               int old_mouse_y = mouse_y;
               int cx;
               int cy;
               while (mouse_b1) // trap while b1 is held
               {
                  bx2 = bx1 + ((mouse_x - old_mouse_x)/20);
                  by2 = by1 + ((mouse_y - old_mouse_y)/20);
                  al_draw_rectangle((bx1)*20, (by1)*20, (bx2)*20, (by2)*20, palette_color[15], 1);
                  al_flip_display();
                  proc_controllers();
               }

               // limits
               if (bx1>32) bx1 = 32;
               if (bx2>32) bx2 = 32;
               if (by1>32) by1 = 32;
               if (by2>32) by2 = 32;
               // ensure top-right, bottom left format
               if (bx1 > bx2)
               {
                  int temp = bx2;
                  bx2 = bx1;
                  bx1= temp;
               }
               if (by1 > by2)
               {
                  int temp = by2;
                  by2 = by1;
                  by1= temp;
               }
               // cycle the selection
               for (cx = bx1; cx < bx2; cx++)
                  for (cy = by1; cy < by2; cy++)
                  {
                     if (b2mode< 3) sa[cx+(cy*32)][0] = b2mode;
                     if ((b2mode> 2) && (b2mode < 5)) sa[cx+(cy*32)][1] = b2mode-3;
                  }
            }
            if (!view_attrib)  // old b1 behaviour
            {
               bmp_index = (mouse_x/20) + (mouse_y/20) * 32;
               quit = 1;
            }
         }
      }

      jh=1;
      al_draw_rectangle(button_x1, button_y+(jh*12), button_x2, button_y+(jh*12)+10, palette_color[15], 1);
      if (view_attrib) sprintf(msg,"View Attributes:ON ");
      else sprintf(msg,"View Attributes:OFF");
      al_draw_text(font, palette_color[13], button_xc, button_y+(jh*12)+1, ALLEGRO_ALIGN_CENTER, msg);

      jh=2;
      al_draw_rectangle(button_x1, button_y+(jh*12), button_x2, button_y+(jh*12)+10, palette_color[15], 1);
      if (b2mode == 0) sprintf(msg,"  Empty  ");
      if (b2mode == 1) sprintf(msg,"  Block  ");
      if (b2mode == 2) sprintf(msg," Special ");
      if (b2mode == 3) sprintf(msg," Unlock ");
      if (b2mode == 4) sprintf(msg,"  Lock  ");
      al_draw_text(font, palette_color[13], button_xc, button_y+(jh*12)+1, ALLEGRO_ALIGN_CENTER, msg);

      if (mouse_b1)
      {
         while (mouse_b1) proc_controllers(); // wait for release
         if ((mouse_x > button_x1) && (mouse_x < button_x2) && (mouse_y > button_y) && (mouse_y < button_y+(4*12) ))
         {
            int mb = (mouse_y - button_y) / 12;
            switch(mb)
            {
               case 1: view_attrib = !view_attrib; break;
               case 2: if (++b2mode>4) b2mode = 0; break;
               break;
            }
         }
      }
      while ((key[ALLEGRO_KEY_ESCAPE]) || (mouse_b2))
      {
         proc_controllers();
         quit = 1;
      }
   }
   return 0;
}


int copy_bitmap_proc()
{
   extern int bmp_index;
   int quit = 0;

   al_set_target_backbuffer(display);
   al_show_mouse_cursor(display);

   while (!quit)
   {

      proc_controllers();
      al_flip_display();
      al_clear_to_color(al_map_rgb(0,0,0));

      al_draw_text(font, palette_color[9], 0, 694, 0, "b1 to put   ");
      al_draw_text(font, palette_color[9], 0, 702, 0, "b2 to get   ");
      al_draw_text(font, palette_color[9], 0, 710, 0, "esc to quit ");

      // draw 32x32 bitmaps
      for (int y = 0; y < 32; y++)
         for (int x = 0; x < 32; x++)
            al_draw_bitmap(memory_bitmap[x+(y*32)], x*20, y*20, 0);


      if ((mouse_y < 640) && (mouse_x < 640))
      {
         int pointer = (mouse_x/20) + (mouse_y/20) * 32 ;
         al_draw_textf(font, palette_color[4], 0, 680, 0, "pointer %-2d  ", pointer );
         al_draw_bitmap(memory_bitmap[pointer], 95, 671, 0);

         if (mouse_b2) bmp_index = pointer;
         if (mouse_b1)
         {
            al_set_target_bitmap(memory_bitmap[pointer]);
             //blit (memory_bitmap[bmp_index], memory_bitmap[pointer], 0, 0, 0, 0, 20, 20);
            al_draw_bitmap(memory_bitmap[bmp_index], 0, 0, 0);
            al_set_target_backbuffer(display);
         }
      }

      while (key[ALLEGRO_KEY_ESCAPE])
      {
         proc_controllers();
         quit = 1;
      }
   }
   return 0;
}
