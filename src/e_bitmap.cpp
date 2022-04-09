// e_bitmap.cpp

#include "pm.h"

// used by sliders only for item block manip
int select_bitmap(int tn)
{
   int quit = 0;
   while (!quit)
   {
      proc_controllers();
      al_flip_display();
      al_clear_to_color(al_map_rgb(0,0,0));

      int x1=200;
      int x2=x1+322;
      int y1=300;

      int x=0;
      int y=0;

      for (int i=0; i<swbn; i++)
      {
         al_draw_bitmap(btile[swbl[i][0] & 1023], x1+x+1, y1+y+1, 0);
         x+=20;
         if (x>=320)
         {
            x=0;
            y+=20;
         }
      }
      int y2=y1+y+22;

      // frame the whole thing
      al_draw_rectangle(x1, y1, x2, y2, palette_color[13], 1);

      al_draw_text(font, palette_color[15], x1, y2+28, 0, "select a block with b1");
      al_draw_text(font, palette_color[15], x1, y2+40, 0, "  b2 or ESC to exit   ");

      if ((mouse_x > x1) && (mouse_x < x2) && (mouse_y > y1) && (mouse_y < y2))
      {
         int pointer = ((mouse_x-x1)/20) + ((mouse_y-y1)/20) * 16 ;
         int tn = swbl[pointer][0] & 1023;

         int ptx = x1;
         int pty = y2;
         al_draw_rectangle(ptx, pty, ptx+160, pty+22, palette_color[13], 1);
         al_draw_bitmap(btile[tn], ptx+1, pty+1, 0);
         al_draw_textf(font, palette_color[13], ptx+30, pty+7, 0, "tile number:%d", tn );

         if (mouse_b1)
         {
            while (mouse_b1) proc_controllers();
            return tn;
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















void save_btiles(void)
{
   // printf("save block_tiles\n");
   ALLEGRO_BITMAP* temp = al_create_bitmap(640, 640);
   al_set_target_bitmap(temp);
   al_clear_to_color(al_map_rgb(0,0,0));

   for (int y = 0; y < 32; y++)
      for (int x = 0; x < 32; x++)
         al_draw_bitmap(btile[y*32 + x], (x*20), (y*20), 0);

   if (!al_save_bitmap("bitmaps/block_tiles.bmp", temp))
   printf("error saving block_tiles\n");
   al_destroy_bitmap(temp);
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


   // ensure sa[][0] does not have any bits set other than the ones we want
   for (int c=0; c<NUM_SPRITES; c++)
   {
      sa[c][0] &= PM_BTILE_ALL_FLAGS;
      sa[c][1] = 0; // not used
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




// this is the common one, called by all
// if mouse is on a button return button number
int draw_flag_rects(int tn, int x, int y, int w, int h, int ys)
{
   int fa[15] = {0}; // make an array to store the flags

   if (tn & PM_BTILE_SOLID_PLAYER)     fa[0]  += 1; // tally set
   if (tn & PM_BTILE_SOLID_ENEMY)      fa[1]  += 1; // tally set
   if (tn & PM_BTILE_SOLID_ITEM)       fa[2]  += 1; // tally set
   if (tn & PM_BTILE_SOLID_PBUL)       fa[3]  += 1; // tally set
   if (tn & PM_BTILE_SOLID_EBUL)       fa[4]  += 1; // tally set
   if (tn & PM_BTILE_SEMISOLID_PLAYER) fa[5]  += 1; // tally set
   if (tn & PM_BTILE_SEMISOLID_ENEMY)  fa[6]  += 1; // tally set
   if (tn & PM_BTILE_SEMISOLID_ITEM)   fa[7]  += 1; // tally set
   if (tn & PM_BTILE_BOMBABLE)         fa[8]  += 1; // tally set
   if (tn & PM_BTILE_BREAKABLE_PBUL)   fa[9]  += 1; // tally set
   if (tn & PM_BTILE_BREAKABLE_EBUL)   fa[10] += 1; // tally set
   if (tn & PM_BTILE_LADDER_MOVE)      fa[11] += 1; // tally set
   if (tn & PM_BTILE_ROPE_MOVE)        fa[12] += 1; // tally set
   if (tn & PM_BTILE_SECRET)           fa[13] += 1; // tally set
   if (tn & PM_BTILE_SHOW_SELECT_WIN)  fa[14] += 1; // tally set

   for (int i=0; i<15; i++)
   {
      if (fa[i] == 0) al_draw_rectangle       (x, y + (ys*i), x+w, y+h+(ys*i), palette_color[15], 1); // empty
      if (fa[i] == 1) al_draw_filled_rectangle(x, y + (ys*i), x+w, y+h+(ys*i), palette_color[15]);    // filled
   }

   if ((mouse_x > x) && (mouse_x < x+w) && (mouse_y > y) && (mouse_y < y+h+(ys*14))) return (mouse_y-y)/ys;
   return -1;
}


// this is for edit attributes only and only only affects sa[][]
void draw_and_proc_flag_rects_for_sa(int tn, int x, int y, int w, int h, int ys)
{
   int highlight = draw_flag_rects(sa[tn][0], x, y, w, h, ys);
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




// this is only for draw_item
void draw_and_proc_flag_rects_draw_item(int x, int y, int w, int h, int ys, int edit)
{
   if (edit)
   {
      int highlight = draw_flag_rects(draw_item_num, x, y, w, h, ys);
      if (highlight > -1)
      {
         al_draw_rectangle(x-1, y+(ys*highlight)-1, x+w+1, y+h+(ys*highlight)+1, palette_color[15], 0);
         if (mouse_b1)
         {
            while (mouse_b1) proc_controllers(); // wait for release
            if (highlight ==  0) draw_item_num ^= PM_BTILE_SOLID_PLAYER;
            if (highlight ==  1) draw_item_num ^= PM_BTILE_SOLID_ENEMY;
            if (highlight ==  2) draw_item_num ^= PM_BTILE_SOLID_ITEM;
            if (highlight ==  3) draw_item_num ^= PM_BTILE_SOLID_PBUL;
            if (highlight ==  4) draw_item_num ^= PM_BTILE_SOLID_EBUL;
            if (highlight ==  5) draw_item_num ^= PM_BTILE_SEMISOLID_PLAYER;
            if (highlight ==  6) draw_item_num ^= PM_BTILE_SEMISOLID_ENEMY;
            if (highlight ==  7) draw_item_num ^= PM_BTILE_SEMISOLID_ITEM;
            if (highlight ==  8) draw_item_num ^= PM_BTILE_BOMBABLE;
            if (highlight ==  9) draw_item_num ^= PM_BTILE_BREAKABLE_PBUL;
            if (highlight == 10) draw_item_num ^= PM_BTILE_BREAKABLE_EBUL;
            if (highlight == 11) draw_item_num ^= PM_BTILE_LADDER_MOVE;
            if (highlight == 12) draw_item_num ^= PM_BTILE_ROPE_MOVE;
            if (highlight == 13) draw_item_num ^= PM_BTILE_SECRET;
            if (highlight == 14) draw_item_num ^= PM_BTILE_SHOW_SELECT_WIN;
         }
      }
   }
   else
   {
      draw_flag_rects(point_item_num, x, y, w, h, ys);
   }



}































void draw_flag_rects_multiple(int bx1, int by1, int bx2, int by2, int x, int y, int w, int h, int ys, int con, int cof, int highlight)
{
    // make an array of flags to store the tally
   int fa[15][2] = {0};

   // cycle the selection
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
                                    al_draw_rectangle       (x, y + (ys*i), x+w, y+h+(ys*i), palette_color[15], 1); // empty box by default
      if (fa[i][0] == 0)            al_draw_filled_rectangle(x, y + (ys*i), x+w, y+h+(ys*i), palette_color[15]); // no clears tallied, filled
      if ((fa[i][0]) && (fa[i][1])) al_draw_filled_triangle (x, y+h+(ys*i), x+w, y+h+(ys*i), x, y+(ys* i), palette_color[15]); // mixed
   }


   if (highlight > -1)
   {
      al_draw_rectangle(x-1, y+(ys*highlight)-1, x+w+1, y+h+(ys*highlight)+1, palette_color[15], 1);



      if (mouse_b1)
      {
         while (mouse_b1) proc_controllers();

         int action = 0;

         if (fa[highlight][0] == 0) action = 0; // all set, so action is to clear
         if (fa[highlight][1] == 0) action = 1; // all clear, so action is to set

         int set_flag = 0;
         if (highlight ==  0) set_flag = PM_BTILE_SOLID_PLAYER;
         if (highlight ==  1) set_flag = PM_BTILE_SOLID_ENEMY;
         if (highlight ==  2) set_flag = PM_BTILE_SOLID_ITEM;
         if (highlight ==  3) set_flag = PM_BTILE_SOLID_PBUL;
         if (highlight ==  4) set_flag = PM_BTILE_SOLID_EBUL;
         if (highlight ==  5) set_flag = PM_BTILE_SEMISOLID_PLAYER;
         if (highlight ==  6) set_flag = PM_BTILE_SEMISOLID_ENEMY;
         if (highlight ==  7) set_flag = PM_BTILE_SEMISOLID_ITEM;
         if (highlight ==  8) set_flag = PM_BTILE_BOMBABLE;
         if (highlight ==  9) set_flag = PM_BTILE_BREAKABLE_PBUL;
         if (highlight == 10) set_flag = PM_BTILE_BREAKABLE_EBUL;
         if (highlight == 11) set_flag = PM_BTILE_LADDER_MOVE;
         if (highlight == 12) set_flag = PM_BTILE_ROPE_MOVE;
         if (highlight == 13) set_flag = PM_BTILE_SECRET;
         if (highlight == 14) set_flag = PM_BTILE_SHOW_SELECT_WIN;


         for (int cx = bx1; cx < bx2; cx++) // cycle the selection
            for (int cy = by1; cy < by2; cy++)
            {
               int tn = cx + (cy*32);
               if (action == 0) sa[tn][0] &= ~set_flag;
               if (action == 1) sa[tn][0] |= set_flag;
            }
      }
   }
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
   int pointer = 0;

   int bx1 = 0;
   int by1 = 0;
   int bx2 = 1;
   int by2 = 1;





   al_set_target_backbuffer(display);
   al_show_mouse_cursor(display);

   while (!quit)
   {
      proc_controllers();
      al_flip_display();
      al_clear_to_color(al_map_rgb(0,0,0));

      if (mode == 0)
      {
          redraw_grid(0, 0, current_selection);
      }
      if (mode == 1)
      {
         redraw_grid(0, 0, -1);
         al_draw_rectangle((bx1)*20, (by1)*20, (bx2)*20, (by2)*20, palette_color[10], 1);
      }

      if (gridlines)
      {
         for (x=0; x<=32; x++) al_draw_line(x*20, 0, x*20, 640, palette_color[15+128], 0);
         for (y=0; y<=32; y++) al_draw_line(0, y*20, 640, y*20, palette_color[15+128], 0);
      }

      // flags section
      int ftx = 690;
      int fty = 340;
      int ys = 20; // y spacing

      draw_flag_text(ftx+4, fty, ys, 15);

      int frw = 12;         // flag rectangle width
      int frh = 12;         // flag rectangle height
      int frx = ftx-frw-2;        // flag rectangle x
      int fry = fty - (frh/2)+4;  // flag rectangle y


      // position of 'currently selected' window (relative to flags)
      int csx = ftx-frw-1;
      int csy = fty-ys-1;

      int tx = 20; // title x
      int ty = 650;



      if (mode == 0) // show flags for one tile
      {

         al_draw_text(font, palette_color[15], tx, ty,     0, "Change or Copy Single");
         al_draw_text(font, palette_color[15], tx, ty+8,   0, "b1 - Select tile");
         al_draw_text(font, palette_color[15], tx, ty+16,  0, "b2 - Copy flags");





         draw_and_proc_flag_rects_for_sa(current_selection, frx, fry, frw, frh, ys);

         al_draw_rectangle(                       csx-1,  csy-7, csx+195, csy+15, palette_color[10], 1);
         al_draw_bitmap(btile[current_selection], csx,    csy-6, 0);
         al_draw_textf(font, palette_color[4],    csx+24, csy,   0, "Current Selection %-2d  ", current_selection );
      }

      if (mode == 1) // show the tallied flags multiple
      {

         al_draw_text(font, palette_color[15], tx, ty,     0, "Change Multiple");
         al_draw_text(font, palette_color[15], tx, ty+8,   0, "Draw selection rectangle");
         al_draw_text(font, palette_color[15], tx, ty+16,  0, "Change flags for all selected");




         draw_flag_rects_multiple(bx1, by1, bx2, by2, frx, fry, frw, frh, ys, 10, 11, -1);

         al_draw_rectangle(csx-4, csy-7, csx+195, csy+15, palette_color[10], 1);
         al_draw_rectangle(csx-4, csy+15, csx+frw+2, csy+15+(15*ys), palette_color[10], 1); // frame for buttons
         int num_tiles = (bx2-bx1) * (by2-by1);
         al_draw_textf(font, palette_color[4],    csx+frw,     csy, 0, " %d Tile(s) Selected", num_tiles);
      }


      // mouse on flag rectangles -- this is to show highlight what flag is selected
      if ((mouse_x > frx) && (mouse_x < frx+frw) && (mouse_y > fry) && (mouse_y < fry+(15*ys) ))
      {
         int indx = (mouse_y-fry)/ys;
         if (mode == 0) draw_and_proc_flag_rects_for_sa(current_selection, frx, fry, frw, frh, ys);
         if (mode == 1) draw_flag_rects_multiple(bx1, by1, bx2, by2, frx, fry, frw, frh, ys, 10, 11, indx);
      }

      if ((mouse_y < 640) && (mouse_x < 640)) // mouse is on main 32x32 tile grid
      {
         int mx = mouse_x/20;
         int my = mouse_y/20;
         pointer = mx + (my*32); // tile the mouse is currently pointing at

         // show pointer on grid as lt blue rectangle
         al_draw_rectangle(mx*20+1, my*20+1, mx*20+19, my*20+19, palette_color[13], 1);

         csx -= 24;
         csy -= 25;
         al_draw_rectangle(csx-4, csy-7, csx+195,   csy+15, palette_color[13], 1);
         al_draw_rectangle(csx-4, csy+15, csx+frw+2, csy+40+(15*ys), palette_color[13], 1); // frame for buttons

         al_draw_bitmap(btile[pointer],         csx-3,  csy-6, 0);
         al_draw_textf(font, palette_color[15], csx+24, csy,   0, "Mouse Pointer %d", pointer);


         // show the the flags of the pointer
         draw_and_proc_flag_rects_for_sa(pointer, frx-24, fry, frw, frh, ys);

         if (mouse_b2)
         {
            while (mouse_b2) proc_controllers();
            if (mode == 0) sa[pointer][0] = sa[current_selection][0];
         }

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

                  bx2 = bx1 + ((mouse_x - old_mouse_x+20)/20);
                  by2 = by1 + ((mouse_y - old_mouse_y+20)/20);

                  if (bx1>32) bx1 = 32; // limits
                  if (bx2>32) bx2 = 32;
                  if (by1>32) by1 = 32;
                  if (by2>32) by2 = 32;

               }
               // ensure top-right, bottom left format
               if (bx1 > bx2) { int temp = bx2; bx2 = bx1; bx1 = temp; }
               if (by1 > by2) { int temp = by2; by2 = by1; by1 = temp; }

               if (bx2 == bx1) bx2++; // don't let size be zero
               if (by2 == by1) by2++;

               if (bx1>32) bx1 = 32; // limits
               if (bx2>32) bx2 = 32;
               if (by1>32) by1 = 32;
               if (by2>32) by2 = 32;


            }
         } // end of mouse_b1 pressed
      } // end of mouse on 32x32 tile grid

      jh=1;
      al_draw_rectangle(button_x1, button_y+(jh*12), button_x2, button_y+(jh*12)+10, palette_color[15], 1);
      if (mode == 0) sprintf(msg,"Change or Copy Single");
      if (mode == 1) sprintf(msg,"Change Multiple");
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
//      while ((key[ALLEGRO_KEY_ESCAPE]) || (mouse_b2))
      while (key[ALLEGRO_KEY_ESCAPE])
      {
         proc_controllers();
         quit = 1;
      }
   }
}
















void copy_tiles()
{
   int quit = 0;
   int reload = 0;
   al_set_target_backbuffer(display);
   al_show_mouse_cursor(display);

   ALLEGRO_BITMAP *qtmp = al_create_bitmap(20, 20);

   ALLEGRO_BITMAP *qtmp2 = al_create_bitmap(20, 20);

   char b2_fn[100];
   char b2_fn2[100];
   sprintf(b2_fn2, "bitmaps\\tiles.bmp");

   // convert to 'ALLEGRO_FS_ENTRY' to get platform specific fully qualified path
   ALLEGRO_FS_ENTRY *FS_fname = al_create_fs_entry(b2_fn2);
   sprintf(b2_fn, "%s", al_get_fs_entry_name(FS_fname));

   // regular load
   ALLEGRO_BITMAP *b2 = al_load_bitmap(b2_fn);


/*   // scale to half size
   ALLEGRO_BITMAP *db2 = al_load_bitmap(b2_fn);
   int db2_w = al_get_bitmap_width(db2);
   int db2_h = al_get_bitmap_height(db2);
   ALLEGRO_BITMAP *b2 = al_create_bitmap(db2_w/2, db2_h/2);
   al_set_target_bitmap(b2);
   al_clear_to_color(al_map_rgb(0, 0, 0));
   al_draw_scaled_bitmap(db2, 0, 0, db2_w, db2_h, 0, 0, db2_w/2, db2_h/2, 0);
   al_set_target_backbuffer(display);
   al_destroy_bitmap(db2); */


   if (!b2)
   {
      sprintf(msg, "Error loading tiles from:%s", b2_fn);
      m_err(msg);
   }

   int b2_w = al_get_bitmap_width(b2);
   int b2_h = al_get_bitmap_height(b2);
   int b2_tw = b2_w/20; // tile grid width

   while (!quit)
   {
      if (reload)
      {
         al_destroy_bitmap(b2);
         sprintf(b2_fn, "bitmaps\\");
         if (mw_file_select("Load Bitmap File", b2_fn, ".bmp", 0)) b2 = al_load_bitmap(b2_fn);
         if (!b2)
         {
            sprintf(msg, "Error loading tiles from:%s", b2_fn);
            m_err(msg);
         }
         b2_w = al_get_bitmap_width(b2);
         b2_h = al_get_bitmap_height(b2);
         b2_tw = b2_w/20; // tile grid width
         reload = 0;
      }



      proc_controllers();
      al_flip_display();
      al_clear_to_color(al_map_rgb(0,0,0));

      // show the draw item
      int dix = 460;
      int diy = 644;
      al_draw_rounded_rectangle(dix, diy, dix+170, diy+22, 2, 2, palette_color[13], 1);
      al_draw_bitmap(qtmp, dix+2, diy+1, 0);
      al_draw_textf(font, palette_color[13], dix+26, diy+3,  0, "Current Draw Item");
//      al_draw_textf(font, palette_color[13], dix+26, diy+12, 0, "mouse b1 to paste"); // only show this line if mouse on main grid



      // draw and process save button
      int sb_x = 375;
      int sb_w = 70;
      int sb_c = sb_x+(sb_w/2);
      int sb_y = 644;

      al_draw_rounded_rectangle(sb_x, sb_y, sb_x+sb_w, sb_y+22, 2, 2, palette_color[10], 1);
      al_draw_textf(font, palette_color[10], sb_c,  sb_y+2,    ALLEGRO_ALIGN_CENTER, "Save");
      al_draw_textf(font, palette_color[10], sb_c,  sb_y+12, ALLEGRO_ALIGN_CENTER, "Changes");


      if ((mouse_x > sb_x) && (mouse_x < (sb_x+sb_w)) && (mouse_y > sb_y) && (mouse_y < sb_y+20))
      {

         al_draw_rounded_rectangle(sb_x, sb_y, sb_x+sb_w, sb_y+22, 2, 2, palette_color[10], 2);
         if (mouse_b1)
         {
            while (mouse_b1) proc_controllers();
            save_btiles();
         }
      }

      // draw and process edit attributes button
      int et_x = 260;
      int et_w = 100;
      int et_c = et_x+(et_w/2);
      int et_y = 644;

      al_draw_rounded_rectangle(et_x, et_y, et_x+et_w, et_y+22, 2, 2, palette_color[12], 1);
      al_draw_textf(font, palette_color[12], et_c,  et_y+2,  ALLEGRO_ALIGN_CENTER, "Edit Tile");
      al_draw_textf(font, palette_color[12], et_c,  et_y+12, ALLEGRO_ALIGN_CENTER, "Attributes");


      if ((mouse_x > et_x) && (mouse_x < (et_x+et_w)) && (mouse_y > et_y) && (mouse_y < et_y+20))
      {

         al_draw_rounded_rectangle(et_x, et_y, et_x+et_w, et_y+22, 2, 2, palette_color[12], 2);
         if (mouse_b1)
         {
            while (mouse_b1) proc_controllers();
            edit_tile_attributes();
         }
      }



      // draw the second bitmap to copy from only
      int b2_x = 700;
      int b2_y = 20;
      al_draw_bitmap(b2, b2_x, b2_y, 0);

      // title with filename only, no path
      ALLEGRO_PATH *ap = al_create_path(b2_fn);
      al_draw_textf(font, palette_color[15], b2_x+(b2_w/2),  b2_y-10, ALLEGRO_ALIGN_CENTER, "%s", al_get_path_filename(ap));
      al_destroy_path(ap);

      // draw and proccess load button
      al_draw_textf(font, palette_color[15], b2_x+2,  b2_y-10, 0, "Load");
      al_draw_rectangle(b2_x, b2_y-12, b2_x+36, b2_y, palette_color[15], 0);

      if ((mouse_x > b2_x) && (mouse_x < (b2_x+36)) && (mouse_y > (b2_y-12)) && (mouse_y < b2_y))
      {
         al_draw_textf(font, palette_color[14], b2_x+2,  b2_y-10, 0, "Load");
         al_draw_rectangle(b2_x, b2_y-12, b2_x+36, b2_y, palette_color[14], 0);
         if (mouse_b1)
         {
            while (mouse_b1) proc_controllers();
            reload = 1;
         }
      }



      // draw a dim rectangle around the entire grid
      al_draw_rectangle(b2_x, b2_y+1, b2_x+b2_w, b2_y+b2_h, palette_color[15+128], 0);

      // is mouse on grid
      if ((mouse_x > b2_x) && (mouse_x < (b2_x + b2_w)) && (mouse_y > b2_y) && (mouse_y < (b2_y+b2_h)))
      {
         int mx = (mouse_x-b2_x)/20;
         int my = (mouse_y-b2_y)/20;
         int pointer = mx + my*b2_tw;

         // draw a rectangle around the entire grid
         al_draw_rectangle(b2_x, b2_y+1, b2_x+b2_w, b2_y+b2_h, palette_color[15], 0);

         // show what tile the mouse is pointing at
         al_draw_rectangle(b2_x+mx*20, b2_y+my*20, b2_x+mx*20+20, b2_y+my*20+20, palette_color[15], 0);


         /*

         // show text attached to bottom of grid
         al_draw_rectangle(b2_x, b2_y+b2_h, b2_x+100, b2_y+b2_h+13, palette_color[15], 0);
//         al_draw_rounded_rectangle(b2_x, b2_y+b2_h, b2_x+100, b2_y+b2_h+13, 2, 2, palette_color[15], 0);
         al_draw_textf(font, palette_color[15], b2_x+4,   b2_y+b2_h+3, 0, "pointer:%d", pointer);
         al_draw_textf(font, palette_color[15], b2_x+120, b2_y+b2_h+3, 0, "mouse b2 to copy");
         //al_draw_textf(font, palette_color[15], b2_x+4,   b2_y+b2_h+23, 0, "mouse b2 to copy");

         */


         al_set_target_bitmap(qtmp2);
         al_clear_to_color(al_map_rgb(0, 0, 0));
         al_draw_bitmap_region(b2, mx*20, my*20, 20, 20, 0, 0, 0);
         al_set_target_backbuffer(display);



         // draw b2_pointer button
         int b1p_x = b2_x+1;
         int b1p_w = 160;
         int b1p_y = b2_y+b2_h;

         al_draw_rounded_rectangle(b1p_x, b1p_y, b1p_x+b1p_w, b1p_y+22, 2, 2, palette_color[15], 1);
         al_draw_bitmap(qtmp2, b1p_x+2, b1p_y+1, 0);
         al_draw_textf(font, palette_color[15], b1p_x+26, b1p_y+3,  0, "pointer:%d", pointer);
         al_draw_textf(font, palette_color[15], b1p_x+26, b1p_y+12, 0, "mouse b2 to copy");













         if (mouse_b2)
         {
            while (mouse_b2) proc_controllers();
            al_set_target_bitmap(qtmp);
            al_clear_to_color(al_map_rgb(0, 0, 0));
            al_draw_bitmap_region(b2, mx*20, my*20, 20, 20, 0, 0, 0);
            al_set_target_backbuffer(display);
         }
      }

      // draw a rectangle around the entire grid
      al_draw_rectangle(1, 1, 640, 640, palette_color[15+96], 0 );



      // draw main 32x32 bitmap grid
      for (int y = 0; y < 32; y++)
         for (int x = 0; x < 32; x++)
            al_draw_bitmap(btile[x+(y*32)], x*20, y*20, 0);

      if ((mouse_y < 640) && (mouse_x < 640))
      {
         int mx = mouse_x/20;
         int my = mouse_y/20;
         int pointer = mx + my*32;

         // show what tile the mouse is pointing at
         al_draw_rectangle(mx*20, my*20, mx*20+20, my*20+20, palette_color[15], 0 );

         // draw a rectangle around the entire grid
         al_draw_rectangle(1, 1, 640, 640, palette_color[15], 0 );

         // draw b1_pointer button
         int b1p_x = 1;
         int b1p_w = 160;
         int b1p_y = 644;

         al_draw_rounded_rectangle(b1p_x, b1p_y, b1p_x+b1p_w, b1p_y+22, 2, 2, palette_color[15], 1);
         al_draw_bitmap(btile[pointer], b1p_x+2, b1p_y+1, 0);
         al_draw_textf(font, palette_color[15], b1p_x+26, b1p_y+3,  0, "pointer:%d", pointer);
         al_draw_textf(font, palette_color[15], b1p_x+26, b1p_y+12, 0, "mouse b2 to copy");


         // this line is from the current draw item box, only show if mouse on first grid
         al_draw_textf(font, palette_color[13], dix+26, diy+12, 0, "mouse b1 to paste"); // only show this if on first grid




         if (mouse_b2)
         {
            while (mouse_b2) proc_controllers();
            al_set_target_bitmap(qtmp);
            al_clear_to_color(al_map_rgb(0, 0, 0));
            al_draw_bitmap(btile[pointer], 0, 0, 0);
            al_set_target_backbuffer(display);
         }

         if (mouse_b1)
         {
            while (mouse_b1) proc_controllers();
            al_set_target_bitmap(btile[pointer]);
            al_clear_to_color(al_map_rgb(0, 0, 0));
            al_draw_bitmap(qtmp, 0, 0, 0);
            al_set_target_backbuffer(display);
         }
      }
      while (key[ALLEGRO_KEY_ESCAPE])
      {
         proc_controllers();
         quit = 1;
      }
   }
   al_destroy_bitmap(b2);
   al_destroy_bitmap(qtmp);
   al_destroy_bitmap(qtmp2);
}



















