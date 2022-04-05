// e_bitmap.cpp

#include "pm.h"

// used by sliders only
int select_bitmap(int tn)
{
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
            al_draw_bitmap(tile[x+(y*32)],x*20, y*20, 0);
      al_draw_rectangle(0.5, 0.5, 640.5, 640.5, palette_color[13], 1);

      if ((mouse_y < 640) && (mouse_x < 640))
      {
         int pointer = (mouse_x/20) + (mouse_y/20) * 32 ;
         al_draw_textf(font, palette_color[13], 522, 648, 0, "pointer %-2d", pointer );
         al_draw_bitmap(tile[pointer], 620, 642, 0);

         al_draw_rectangle(518, 640.5, 640.5, 662.5, palette_color[13], 1);

         if (mouse_b1)
         {
            while (mouse_b1) proc_controllers();
            return pointer;
         }
      }
      while ((key[ALLEGRO_KEY_ESCAPE]) || (mouse_b2))
      {
         proc_controllers();
         quit = 1;
      }
   }
   return tn; // original if bad exit
}







int bmp_index = 0;
int select_bitmap_ans(int zzindx)
{
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
            al_draw_bitmap(tile[x+(y*32)],x*20, y*20, 0);
      al_draw_rectangle(0.5, 0.5, 640.5, 640.5, palette_color[13], 1);

      int xc = 180;
      int yc = 648;

      al_draw_textf(font, palette_color[14], 80, 666, 0, "Get Shape %d ", zz[4][zzindx] );

      sprintf(msg, "Current Sequence %d", zzindx);
      int l = 2+strlen(msg)*4;
      al_draw_text(font, palette_color[13], xc+150, yc+2, ALLEGRO_ALIGN_CENTER, msg);
      al_draw_rectangle(xc+150-l, yc+1, xc+150+l, yc+11, palette_color[13], 1);

      for (int c=0; c < zz[4][zzindx] + 1; c++)   // show current seq shapes
         if (( zz[5+c][zzindx] < NUM_SPRITES) && (zz[5+c][zzindx] > 0))
            al_draw_bitmap(tile[ zz[5+c][zzindx] ], xc+1+c*20, yc+12, 0);
      al_draw_rectangle(xc+0.5, yc+11.5, xc+302.5, yc+32.5, palette_color[13], 1);


      if ((mouse_y < 640) && (mouse_x < 640))
      {
         int pointer = (mouse_x/20) + (mouse_y/20) * 32 ;
         al_draw_textf(font, palette_color[13], 522, 648, 0, "pointer %-2d", pointer );
         al_draw_bitmap(tile[pointer], 620, 642, 0);

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
   int zzindx = 3;
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
      sprintf(msg,"Animation Delay %-2d  ",zz[3][zzindx]);
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
            al_draw_bitmap(tile[ zz[5+c][zzindx] ], 1+c*20, 212, 0);
      al_draw_rectangle(0.5, 211.5, 302.5, 232.5, palette_color[13], 1);

      for (c=0; c < 32; c++)   // draw 32x8 grid of animation sequences
         for (x=0; x < 8; x++)
            if (zz[4][c + (x * 32)] != 0)
               if ((zz[0][c + (x * 32)] < NUM_SPRITES) && (zz[0][c + (x * 32)] > 0 ))
                  al_draw_bitmap(tile[zz[0][c + (x * 32)]], 2+c*20, 30+x*20, 0);
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
               case 5: save_tiles(); break;
            }
         }
      }

      frame_num++;
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
            al_draw_bitmap(tile[x+(y*32)],x*20, y*20, 0);
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
         al_draw_bitmap(tile[pointer], 95, 671, 0);

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
            al_draw_bitmap(tile[x+(y*32)], x*20, y*20, 0);


      if ((mouse_y < 640) && (mouse_x < 640))
      {
         int pointer = (mouse_x/20) + (mouse_y/20) * 32 ;
         al_draw_textf(font, palette_color[4], 0, 680, 0, "pointer %-2d  ", pointer );
         al_draw_bitmap(tile[pointer], 95, 671, 0);

         if (mouse_b2) bmp_index = pointer;
         if (mouse_b1)
         {
            al_set_target_bitmap(tile[pointer]);
            al_clear_to_color(al_map_rgb(0, 0, 0));
            al_draw_bitmap(tile[bmp_index], 0, 0, 0);
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



void tile_editor(void)
{
   int quit = 0;

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

      sprintf(msg, "Tile Editor");
      int l = 2+strlen(msg)*4;
      al_draw_text(font, palette_color[9], SCREEN_W/2, 20, ALLEGRO_ALIGN_CENTER, msg);
      al_draw_rectangle(SCREEN_W/2-l, 19, SCREEN_W/2+l, 29, palette_color[9], 1);


      jh=1;
      al_draw_rectangle(button_x1, button_y+(jh*12), button_x2, button_y+(jh*12)+10, palette_color[15], 1);
      sprintf(msg,"Get New Shapes");
      al_draw_text(font, palette_color[13], button_xc, button_y+(jh*12)+1, ALLEGRO_ALIGN_CENTER, msg);

      jh=2;
      al_draw_rectangle(button_x1, button_y+(jh*12), button_x2, button_y+(jh*12)+10, palette_color[15], 1);
      sprintf(msg,"Animation Delay   ");
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



      if (key[ALLEGRO_KEY_DELETE])
      {
      }

      if (mouse_b1)
      {

         while (mouse_b1) proc_controllers(); // wait for release

         if ((mouse_x > button_x1) && (mouse_x < button_x2) && (mouse_y > button_y) && (mouse_y < button_y+(6*12) ))
         {
            int mb = (mouse_y - button_y) / 12;
            switch(mb)
            {
               case 1:
               break;
               case 3: select_bitmap_proc(); break;
               case 4: copy_bitmap_proc(); break;
               //case 5: save_tiles(); break;
            }
         }
      }

      while ((key[ALLEGRO_KEY_ESCAPE]) || (mouse_b2))
      {
         proc_controllers();
         quit = 1;
      }
   }
}






















void save_sprit(void)
{
   for (int c=0; c<NUM_ANS; c++)    // set all to initial
      if (zz[4][c])
      {
         zz[0][c]=zz[5][c];
         zz[1][c]=0;
         zz[2][c]=0;
      }

   printf("saving\n");

   FILE *fp = fopen("bitmaps/sprit001.pm", "wb");
   fwrite(zz, sizeof(zz), 1, fp);
   fwrite(sa, sizeof(sa), 1, fp);
   fclose(fp);
}


void redraw_grid(int x, int y, int current_selection) // draw 32x32 bitmaps
{
   for (int dx=0; dx<32; dx++)
      for (int dy=0; dy<32; dy++)
      {
         int tn = dx + (dy*32); // tile number
         int zx = x + (dx*20);
         int zy = y + (dy*20);

         al_draw_bitmap(btile[tn], zx, zy, 0);
         if (tn == current_selection) al_draw_rectangle(zx, zy, zx+19, zy+19, palette_color[10], 2);

      }
}









void draw_flag_text(int x, int y, int ys, int col)
{
   al_draw_text(font, palette_color[col], x, y, 0, "SOLID_PLAYER");     y+=ys;
   al_draw_text(font, palette_color[col], x, y, 0, "SOLID_ENEMY");      y+=ys;
   al_draw_text(font, palette_color[col], x, y, 0, "SOLID_ITEM");       y+=ys;
   al_draw_text(font, palette_color[col], x, y, 0, "SOLID_PBUL");       y+=ys;
   al_draw_text(font, palette_color[col], x, y, 0, "SOLID_EBUL");       y+=ys;
   al_draw_text(font, palette_color[col], x, y, 0, "SEMISOLID_PLAYER"); y+=ys;
   al_draw_text(font, palette_color[col], x, y, 0, "SEMISOLID_ENEMY");  y+=ys;
   al_draw_text(font, palette_color[col], x, y, 0, "SEMISOLID_ITEM");   y+=ys;
   al_draw_text(font, palette_color[col], x, y, 0, "BOMBABLE");         y+=ys;
   al_draw_text(font, palette_color[col], x, y, 0, "BREAKABLE_PBUL");   y+=ys;
   al_draw_text(font, palette_color[col], x, y, 0, "BREAKABLE_EBUL");   y+=ys;
   al_draw_text(font, palette_color[col], x, y, 0, "LADDER_MOVE");      y+=ys;
   al_draw_text(font, palette_color[col], x, y, 0, "ROPE_MOVE");        y+=ys;
   al_draw_text(font, palette_color[col], x, y, 0, "SECRET");           y+=ys;
   al_draw_text(font, palette_color[col], x, y, 0, "SELECT_WIN_SHOW");  y+=ys;
}




void draw_flag_rects(int tn, int x, int y, int w, int h, int ys, int con, int cof, int highlight)
{
   if (sa[tn][0] & PM_BTILE_SOLID_PLAYER)      al_draw_filled_rectangle(x, y+(ys* 0), x+w, y+h+(ys* 0), palette_color[con]);
   else                                        al_draw_filled_rectangle(x, y+(ys* 0), x+w, y+h+(ys* 0), palette_color[cof]);
   if (sa[tn][0] & PM_BTILE_SOLID_ENEMY)       al_draw_filled_rectangle(x, y+(ys* 1), x+w, y+h+(ys* 1), palette_color[con]);
   else                                        al_draw_filled_rectangle(x, y+(ys* 1), x+w, y+h+(ys* 1), palette_color[cof]);
   if (sa[tn][0] & PM_BTILE_SOLID_ITEM)        al_draw_filled_rectangle(x, y+(ys* 2), x+w, y+h+(ys* 2), palette_color[con]);
   else                                        al_draw_filled_rectangle(x, y+(ys* 2), x+w, y+h+(ys* 2), palette_color[cof]);
   if (sa[tn][0] & PM_BTILE_SOLID_PBUL)        al_draw_filled_rectangle(x, y+(ys* 3), x+w, y+h+(ys* 3), palette_color[con]);
   else                                        al_draw_filled_rectangle(x, y+(ys* 3), x+w, y+h+(ys* 3), palette_color[cof]);
   if (sa[tn][0] & PM_BTILE_SOLID_EBUL)        al_draw_filled_rectangle(x, y+(ys* 4), x+w, y+h+(ys* 4), palette_color[con]);
   else                                        al_draw_filled_rectangle(x, y+(ys* 4), x+w, y+h+(ys* 4), palette_color[cof]);
   if (sa[tn][0] & PM_BTILE_SEMISOLID_PLAYER)  al_draw_filled_rectangle(x, y+(ys* 5), x+w, y+h+(ys* 5), palette_color[con]);
   else                                        al_draw_filled_rectangle(x, y+(ys* 5), x+w, y+h+(ys* 5), palette_color[cof]);
   if (sa[tn][0] & PM_BTILE_SEMISOLID_ENEMY)   al_draw_filled_rectangle(x, y+(ys* 6), x+w, y+h+(ys* 6), palette_color[con]);
   else                                        al_draw_filled_rectangle(x, y+(ys* 6), x+w, y+h+(ys* 6), palette_color[cof]);
   if (sa[tn][0] & PM_BTILE_SEMISOLID_ITEM)    al_draw_filled_rectangle(x, y+(ys* 7), x+w, y+h+(ys* 7), palette_color[con]);
   else                                        al_draw_filled_rectangle(x, y+(ys* 7), x+w, y+h+(ys* 7), palette_color[cof]);
   if (sa[tn][0] & PM_BTILE_BOMBABLE)          al_draw_filled_rectangle(x, y+(ys* 8), x+w, y+h+(ys* 8), palette_color[con]);
   else                                        al_draw_filled_rectangle(x, y+(ys* 8), x+w, y+h+(ys* 8), palette_color[cof]);
   if (sa[tn][0] & PM_BTILE_BREAKABLE_PBUL)    al_draw_filled_rectangle(x, y+(ys* 9), x+w, y+h+(ys* 9), palette_color[con]);
   else                                        al_draw_filled_rectangle(x, y+(ys* 9), x+w, y+h+(ys* 9), palette_color[cof]);
   if (sa[tn][0] & PM_BTILE_BREAKABLE_EBUL)    al_draw_filled_rectangle(x, y+(ys*10), x+w, y+h+(ys*10), palette_color[con]);
   else                                        al_draw_filled_rectangle(x, y+(ys*10), x+w, y+h+(ys*10), palette_color[cof]);
   if (sa[tn][0] & PM_BTILE_LADDER_MOVE)       al_draw_filled_rectangle(x, y+(ys*11), x+w, y+h+(ys*11), palette_color[con]);
   else                                        al_draw_filled_rectangle(x, y+(ys*11), x+w, y+h+(ys*11), palette_color[cof]);
   if (sa[tn][0] & PM_BTILE_ROPE_MOVE)         al_draw_filled_rectangle(x, y+(ys*12), x+w, y+h+(ys*12), palette_color[con]);
   else                                        al_draw_filled_rectangle(x, y+(ys*12), x+w, y+h+(ys*12), palette_color[cof]);
   if (sa[tn][0] & PM_BTILE_SECRET)            al_draw_filled_rectangle(x, y+(ys*13), x+w, y+h+(ys*13), palette_color[con]);
   else                                        al_draw_filled_rectangle(x, y+(ys*13), x+w, y+h+(ys*13), palette_color[cof]);
   if (sa[tn][0] & PM_BTILE_SHOW_SELECT_WIN)   al_draw_filled_rectangle(x, y+(ys*14), x+w, y+h+(ys*14), palette_color[con]);
   else                                        al_draw_filled_rectangle(x, y+(ys*14), x+w, y+h+(ys*14), palette_color[cof]);

   if (highlight > -1)
   {
      al_draw_rectangle(x-1, y+(ys*highlight), x+w+1, y+h+(ys*highlight), palette_color[15], 1);
      if (mouse_b1)
      {
         while (mouse_b1) proc_controllers(); // wait for release
         if (highlight ==  0) sa[tn][0] ^= PM_BTILE_SOLID_PLAYER;
         if (highlight ==  1) sa[tn][0] ^= PM_BTILE_SOLID_ENEMY;
         if (highlight ==  2) sa[tn][0] ^= PM_BTILE_SOLID_ITEM;
         if (highlight ==  3) sa[tn][0] ^= PM_BTILE_SOLID_PBUL;
         if (highlight ==  4) sa[tn][0] ^= PM_BTILE_SOLID_EBUL;
         if (highlight ==  5) sa[tn][0] ^= PM_BTILE_SEMISOLID_PLAYER;
         if (highlight ==  6) sa[tn][0] ^= PM_BTILE_SEMISOLID_ENEMY;
         if (highlight ==  7) sa[tn][0] ^= PM_BTILE_SEMISOLID_ITEM;
         if (highlight ==  8) sa[tn][0] ^= PM_BTILE_BOMBABLE;
         if (highlight ==  9) sa[tn][0] ^= PM_BTILE_BREAKABLE_PBUL;
         if (highlight == 10) sa[tn][0] ^= PM_BTILE_BREAKABLE_EBUL;
         if (highlight == 11) sa[tn][0] ^= PM_BTILE_LADDER_MOVE;
         if (highlight == 12) sa[tn][0] ^= PM_BTILE_ROPE_MOVE;
         if (highlight == 13) sa[tn][0] ^= PM_BTILE_SECRET;
         if (highlight == 14) sa[tn][0] ^= PM_BTILE_SHOW_SELECT_WIN;
      }
   }
}




































void draw_flag_rects_multiple(int bx1, int by1, int bx2, int by2, int x, int y, int w, int h, int ys, int con, int cof, int highlight)
{
    // make an array of flags to store the tally
   int fa[15][2] = {0};



   //cycle the selection
   for (int cx = bx1; cx < bx2; cx++)
      for (int cy = by1; cy < by2; cy++)
      {
         int tn = cx + (cy*32);

         if (sa[tn][0] & PM_BTILE_SOLID_PLAYER)     fa[0][1] += 1; // tally set
         else                                       fa[0][0] += 1; // tally clear

         if (sa[tn][0] & PM_BTILE_SOLID_ENEMY)      fa[1][1] += 1; // tally set
         else                                       fa[1][0] += 1; // tally clear

         if (sa[tn][0] & PM_BTILE_SOLID_ITEM)       fa[2][1] += 1; // tally set
         else                                       fa[2][0] += 1; // tally clear

         if (sa[tn][0] & PM_BTILE_SOLID_PBUL)       fa[3][1] += 1; // tally set
         else                                       fa[3][0] += 1; // tally clear

         if (sa[tn][0] & PM_BTILE_SOLID_EBUL)       fa[4][1] += 1; // tally set
         else                                       fa[4][0] += 1; // tally clear

         if (sa[tn][0] & PM_BTILE_SEMISOLID_PLAYER) fa[5][1] += 1; // tally set
         else                                       fa[5][0] += 1; // tally clear

         if (sa[tn][0] & PM_BTILE_SEMISOLID_ENEMY)  fa[6][1] += 1; // tally set
         else                                       fa[6][0] += 1; // tally clear

         if (sa[tn][0] & PM_BTILE_SEMISOLID_ITEM)   fa[7][1] += 1; // tally set
         else                                       fa[7][0] += 1; // tally clear

         if (sa[tn][0] & PM_BTILE_BOMBABLE)         fa[8][1] += 1; // tally set
         else                                       fa[8][0] += 1; // tally clear

         if (sa[tn][0] & PM_BTILE_BREAKABLE_PBUL)   fa[9][1] += 1; // tally set
         else                                       fa[9][0] += 1; // tally clear

         if (sa[tn][0] & PM_BTILE_BREAKABLE_EBUL)   fa[10][1] += 1; // tally set
         else                                       fa[10][0] += 1; // tally clear

         if (sa[tn][0] & PM_BTILE_LADDER_MOVE)      fa[11][1] += 1; // tally set
         else                                       fa[11][0] += 1; // tally clear

         if (sa[tn][0] & PM_BTILE_ROPE_MOVE)        fa[12][1] += 1; // tally set
         else                                       fa[12][0] += 1; // tally clear

         if (sa[tn][0] & PM_BTILE_SECRET)           fa[13][1] += 1; // tally set
         else                                       fa[13][0] += 1; // tally clear

         if (sa[tn][0] & PM_BTILE_SHOW_SELECT_WIN)  fa[14][1] += 1; // tally set
         else                                       fa[14][0] += 1; // tally clear
      }

   for (int i=0; i<15; i++)
   {
      int col = 15;
      if (fa[i][0] == 0) col = 10; // no clears tallied

      if (fa[i][1] == 0) col = 11; // no sets tallied

      if ((fa[i][0]) && (fa[i][1])) col = 14; // both


      al_draw_filled_rectangle(x, y+(ys* i), x+w, y+h+(ys* i), palette_color[col]);

   }


/*

















   if (sa[tn][0] & PM_BTILE_SOLID_PLAYER)      al_draw_filled_rectangle(x, y+(ys* 0), x+w, y+h+(ys* 0), palette_color[con]);
   else                                        al_draw_filled_rectangle(x, y+(ys* 0), x+w, y+h+(ys* 0), palette_color[cof]);
   if (sa[tn][0] & PM_BTILE_SOLID_ENEMY)       al_draw_filled_rectangle(x, y+(ys* 1), x+w, y+h+(ys* 1), palette_color[con]);
   else                                        al_draw_filled_rectangle(x, y+(ys* 1), x+w, y+h+(ys* 1), palette_color[cof]);
   if (sa[tn][0] & PM_BTILE_SOLID_ITEM)        al_draw_filled_rectangle(x, y+(ys* 2), x+w, y+h+(ys* 2), palette_color[con]);
   else                                        al_draw_filled_rectangle(x, y+(ys* 2), x+w, y+h+(ys* 2), palette_color[cof]);
   if (sa[tn][0] & PM_BTILE_SOLID_PBUL)        al_draw_filled_rectangle(x, y+(ys* 3), x+w, y+h+(ys* 3), palette_color[con]);
   else                                        al_draw_filled_rectangle(x, y+(ys* 3), x+w, y+h+(ys* 3), palette_color[cof]);
   if (sa[tn][0] & PM_BTILE_SOLID_EBUL)        al_draw_filled_rectangle(x, y+(ys* 4), x+w, y+h+(ys* 4), palette_color[con]);
   else                                        al_draw_filled_rectangle(x, y+(ys* 4), x+w, y+h+(ys* 4), palette_color[cof]);
   if (sa[tn][0] & PM_BTILE_SEMISOLID_PLAYER)  al_draw_filled_rectangle(x, y+(ys* 5), x+w, y+h+(ys* 5), palette_color[con]);
   else                                        al_draw_filled_rectangle(x, y+(ys* 5), x+w, y+h+(ys* 5), palette_color[cof]);
   if (sa[tn][0] & PM_BTILE_SEMISOLID_ENEMY)   al_draw_filled_rectangle(x, y+(ys* 6), x+w, y+h+(ys* 6), palette_color[con]);
   else                                        al_draw_filled_rectangle(x, y+(ys* 6), x+w, y+h+(ys* 6), palette_color[cof]);
   if (sa[tn][0] & PM_BTILE_SEMISOLID_ITEM)    al_draw_filled_rectangle(x, y+(ys* 7), x+w, y+h+(ys* 7), palette_color[con]);
   else                                        al_draw_filled_rectangle(x, y+(ys* 7), x+w, y+h+(ys* 7), palette_color[cof]);
   if (sa[tn][0] & PM_BTILE_BOMBABLE)          al_draw_filled_rectangle(x, y+(ys* 8), x+w, y+h+(ys* 8), palette_color[con]);
   else                                        al_draw_filled_rectangle(x, y+(ys* 8), x+w, y+h+(ys* 8), palette_color[cof]);
   if (sa[tn][0] & PM_BTILE_BREAKABLE_PBUL)    al_draw_filled_rectangle(x, y+(ys* 9), x+w, y+h+(ys* 9), palette_color[con]);
   else                                        al_draw_filled_rectangle(x, y+(ys* 9), x+w, y+h+(ys* 9), palette_color[cof]);
   if (sa[tn][0] & PM_BTILE_BREAKABLE_EBUL)    al_draw_filled_rectangle(x, y+(ys*10), x+w, y+h+(ys*10), palette_color[con]);
   else                                        al_draw_filled_rectangle(x, y+(ys*10), x+w, y+h+(ys*10), palette_color[cof]);
   if (sa[tn][0] & PM_BTILE_LADDER_MOVE)       al_draw_filled_rectangle(x, y+(ys*11), x+w, y+h+(ys*11), palette_color[con]);
   else                                        al_draw_filled_rectangle(x, y+(ys*11), x+w, y+h+(ys*11), palette_color[cof]);
   if (sa[tn][0] & PM_BTILE_ROPE_MOVE)         al_draw_filled_rectangle(x, y+(ys*12), x+w, y+h+(ys*12), palette_color[con]);
   else                                        al_draw_filled_rectangle(x, y+(ys*12), x+w, y+h+(ys*12), palette_color[cof]);
   if (sa[tn][0] & PM_BTILE_SECRET)            al_draw_filled_rectangle(x, y+(ys*13), x+w, y+h+(ys*13), palette_color[con]);
   else                                        al_draw_filled_rectangle(x, y+(ys*13), x+w, y+h+(ys*13), palette_color[cof]);
   if (sa[tn][0] & PM_BTILE_SHOW_SELECT_WIN)   al_draw_filled_rectangle(x, y+(ys*14), x+w, y+h+(ys*14), palette_color[con]);
   else                                        al_draw_filled_rectangle(x, y+(ys*14), x+w, y+h+(ys*14), palette_color[cof]);

   if (highlight > -1)
   {
      al_draw_rectangle(x-1, y+(ys*highlight), x+w+1, y+h+(ys*highlight), palette_color[15], 1);
      if (mouse_b1)
      {
         while (mouse_b1) proc_controllers(); // wait for release
         if (highlight ==  0) sa[tn][0] ^= PM_BTILE_SOLID_PLAYER;
         if (highlight ==  1) sa[tn][0] ^= PM_BTILE_SOLID_ENEMY;
         if (highlight ==  2) sa[tn][0] ^= PM_BTILE_SOLID_ITEM;
         if (highlight ==  3) sa[tn][0] ^= PM_BTILE_SOLID_PBUL;
         if (highlight ==  4) sa[tn][0] ^= PM_BTILE_SOLID_EBUL;
         if (highlight ==  5) sa[tn][0] ^= PM_BTILE_SEMISOLID_PLAYER;
         if (highlight ==  6) sa[tn][0] ^= PM_BTILE_SEMISOLID_ENEMY;
         if (highlight ==  7) sa[tn][0] ^= PM_BTILE_SEMISOLID_ITEM;
         if (highlight ==  8) sa[tn][0] ^= PM_BTILE_BOMBABLE;
         if (highlight ==  9) sa[tn][0] ^= PM_BTILE_BREAKABLE_PBUL;
         if (highlight == 10) sa[tn][0] ^= PM_BTILE_BREAKABLE_EBUL;
         if (highlight == 11) sa[tn][0] ^= PM_BTILE_LADDER_MOVE;
         if (highlight == 12) sa[tn][0] ^= PM_BTILE_ROPE_MOVE;
         if (highlight == 13) sa[tn][0] ^= PM_BTILE_SECRET;
         if (highlight == 14) sa[tn][0] ^= PM_BTILE_SHOW_SELECT_WIN;
      }
   }


   */

}




























































void edit_tile_attributes(void)
{
   int x, y;
   int mode = 0;
   int quit = 0;
   int gridlines = 0;
   int button_x1 = 400;
   int button_x2 = 639;
   int button_xc = 520;
   int button_y = 650;
   int jh;


   int current_selection = 0;


   int bx1 = 0;
   int by1 = 0;
   int bx2 = 0;
   int by2 = 0;





   al_set_target_backbuffer(display);
   al_show_mouse_cursor(display);

   while (!quit)
   {
      proc_controllers();
      al_flip_display();
      al_clear_to_color(al_map_rgb(0,0,0));

      if (mode == 0) redraw_grid(0, 0, current_selection);
      if (mode == 1)
      {
         redraw_grid(0, 0, -1);
         al_draw_rectangle((bx1)*20, (by1)*20, (bx2)*20, (by2)*20, palette_color[10], 1);
      }

      if (gridlines)
      {
         for (x=0; x<=32; x++) al_draw_line(x*20, 0, x*20, 640, palette_color[15], 1);
         for (y=0; y<=32; y++) al_draw_line(0, y*20, 640, y*20, palette_color[15], 1);
      }


      // flags section
      int fx = 680;
      int fy = 400;
      int ys = 10; // y spacing
      draw_flag_text(fx, fy, ys, 15);


      if (mode == 0) draw_flag_rects(current_selection, fx-10, fy, 8, 8, ys, 10, 11, -1);
      if (mode == 1) draw_flag_rects_multiple(bx1, by1, bx2, by2, fx-10, fy, 8, 8, ys, 10, 11, -1);










      // mouse on current flag rectangle
      if ((mouse_x > fx-11) && (mouse_x < fx-11+8) && (mouse_y > fy) && (mouse_y < fy+(15*10) ))
      {
         int indx = (mouse_y-fy)/ys;

         if (mode == 0) draw_flag_rects(current_selection, fx-10, fy, 8, 8, ys, 10, 11, indx); // this is to show selection outline








      }

      int csx = 670;
      int csy = 380;

      if (mode == 0)
      {
         al_draw_rectangle(                       csx-1,  csy-7, csx+195, csy+15, palette_color[10], 1);
         al_draw_bitmap(btile[current_selection], csx,    csy-6, 0);
         al_draw_textf(font, palette_color[4],    csx+24, csy,   0, "Current Selection %-2d  ", current_selection );
      }

      if (mode == 1)
      {
         al_draw_rectangle(csx-4, csy-7, csx+195, csy+15, palette_color[10], 1);
         int num_tiles = (bx2-bx1) * (by2-by1);
         al_draw_textf(font, palette_color[4],    csx,     csy, 0, " %d Tiles Selected", num_tiles);
      }



      // mouse is on main 32x32 tile grid
      if ((mouse_y < 640) && (mouse_x < 640))
      {
         int pointer = (mouse_x/20) + (mouse_y/20) * 32 ;

         csx = 20;
         csy = 650;
         al_draw_rectangle(                     csx-1,  csy-7, csx+195, csy+15, palette_color[15], 1);
         al_draw_bitmap(btile[pointer],         csx,    csy-6, 0);
         al_draw_textf(font, palette_color[15], csx+24, csy,   0, "Mouse Pointer %d", pointer);



         if (mouse_b1)
         {
            if (mode == 0) current_selection = (mouse_x/20) + (mouse_y/20) * 32;

            if (mode == 1)
            {
               bx2 = bx1 = mouse_x/20;
               by2 = by1 = mouse_y/20;
               int old_mouse_x = mouse_x;
               int old_mouse_y = mouse_y;

               while (mouse_b1) // trap while b1 is held
               {
                  proc_controllers();
                  al_flip_display();
                  al_clear_to_color(al_map_rgb(0,0,0));
                  redraw_grid(0, 0, -1);
                  al_draw_rectangle((bx1)*20, (by1)*20, (bx2)*20, (by2)*20, palette_color[15], 1);

                  bx2 = bx1 + ((mouse_x - old_mouse_x)/20);
                  by2 = by1 + ((mouse_y - old_mouse_y)/20);

                  if (bx1>32) bx1 = 32; // limits
                  if (bx2>32) bx2 = 32;
                  if (by1>32) by1 = 32;
                  if (by2>32) by2 = 32;

               }
               // ensure top-right, bottom left format
               if (bx1 > bx2) { int temp = bx2; bx2 = bx1; bx1 = temp; }
               if (by1 > by2) { int temp = by2; by2 = by1; by1 = temp; }

            }
         } // end of mouse_b1 pressed
      } // end of mouse on 32x32 tile grid

      jh=1;
      al_draw_rectangle(button_x1, button_y+(jh*12), button_x2, button_y+(jh*12)+10, palette_color[15], 1);
      if (mode == 0) sprintf(msg,"Change One Selected Tile");
      if (mode == 1) sprintf(msg,"Change Muliple Selected Tiles");
      al_draw_text(font, palette_color[13], button_xc, button_y+(jh*12)+1, ALLEGRO_ALIGN_CENTER, msg);

      jh=2;
      al_draw_rectangle(button_x1, button_y+(jh*12), button_x2, button_y+(jh*12)+10, palette_color[15], 1);
      sprintf(msg," ");

      al_draw_text(font, palette_color[13], button_xc, button_y+(jh*12)+1, ALLEGRO_ALIGN_CENTER, msg);


      jh=3;
      al_draw_rectangle(button_x1, button_y+(jh*12), button_x2, button_y+(jh*12)+10, palette_color[15], 1);
      if (gridlines) sprintf(msg,"Gridlines:ON ");
      else sprintf(msg,"Gridlines:OFF");
      al_draw_text(font, palette_color[13], button_xc, button_y+(jh*12)+1, ALLEGRO_ALIGN_CENTER, msg);

      jh=4;
      al_draw_rectangle(button_x1, button_y+(jh*12), button_x2, button_y+(jh*12)+10, palette_color[15], 1);
      sprintf(msg,"Save");
      al_draw_text(font, palette_color[13], button_xc, button_y+(jh*12)+1, ALLEGRO_ALIGN_CENTER, msg);

      if (mouse_b1)
      {
         while (mouse_b1) proc_controllers(); // wait for release
         if ((mouse_x > button_x1) && (mouse_x < button_x2) && (mouse_y > button_y) && (mouse_y < button_y+(6*12) ))
         {
            int mb = (mouse_y - button_y) / 12;
            switch(mb)
            {
               case 1: if (++mode>1) mode = 0; break;
               case 2: break;
               case 3: gridlines = !gridlines;     break;
               case 4: save_sprit();               break;
            }
         }
      }
      while ((key[ALLEGRO_KEY_ESCAPE]) || (mouse_b2))
      {
         proc_controllers();
         quit = 1;
      }
   }
}
































