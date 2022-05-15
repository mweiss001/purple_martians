// e_bitmap.cpp
#include "pm.h"

// used by sliders only for item block manip
// gets block with flags from level
int select_bitmap(int tn)
{
   int quit = 0;
   while (!quit)
   {
      ovw_process_scrolledge();
      ovw_get_block_position_on_map();
      al_flip_display();
      proc_scale_factor_change();
      proc_controllers();
      proc_frame_delay();
      get_new_background(0);
      draw_lifts();
      draw_items();
      draw_enemies();

      // draw rectangle around selected block
      al_draw_rectangle(gx*20, gy*20, gx*20+20, gy*20+20, palette_color[15], 1);

      get_new_screen_buffer(3, 0, 0);

      point_item_type = 1;
      point_item_num = l[gx][gy];

      int swx1 = 200;
      int swy1 = 200;

      int swx2 = swx1 + 144;
      int swy2 = swy1 + 32;  // above flags

      // erase background and frame
      int swy3 = swy2 + 145; // below flags
      al_draw_filled_rectangle(             swx1-2,  swy1-2,  swx2+2, swy3, palette_color[0]);
      al_draw_rectangle(                    swx1-2,  swy1-2,  swx2+2, swy3, palette_color[13], 1);

      // view item area
      al_draw_rectangle(                    swx1,    swy1,    swx2, swy2, palette_color[9], 1);
      al_draw_text(font, palette_color[15], swx1+24, swy1+2, 0, "Choose Block");
      em_draw_item_info(                    swx1+2,  swy1+9, 9, point_item_type, point_item_num);

      // flags section
      int ftx = swx1+11;
      int fty = swy1+35;
      int ys = 10; // y spacing
      draw_flag_text(ftx+4, fty, ys, 15, 0);

      int frw = 6;            // flag rectangle width
      int frh = 6;            // flag rectangle height
      int frx = ftx-frw-2;        // flag rectangle x
      int fry = fty - (frh/2)+4;  // flag rectangle y
      draw_flag_rects(point_item_num, frx, fry, frw, frh, ys, 14);

      if (mouse_b1)
      {
         while (mouse_b1) proc_controllers(); // wait for release
         return point_item_num;
      }
      if (key[ALLEGRO_KEY_ESCAPE])  // trap here 1st
      {
         while (key[ALLEGRO_KEY_ESCAPE]) proc_controllers();
         quit = 1;
      }
   }
   return tn;
}

// this is used by animation_sequence_editor() only to choose new bitmaps for sequences
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

void animation_sequence_editor(void)
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
               case 3: save_sprit(); break;
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




void draw_flag_text(int x, int y, int ys, int col, int last_flag_show)
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
   if (last_flag_show) al_draw_text(font, palette_color[col], x, y, 0, "SELECT_WIN_SHOW");
}


// this is the common one, called by all
// if mouse is on a button return button number
int draw_flag_rects(int tn, int x, int y, int w, int h, int ys, int last_flag_show)
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

   for (int i=0; i<last_flag_show; i++)
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
   int highlight = draw_flag_rects(sa[tn][0], x, y, w, h, ys, 15);
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



// this common one calls text and rect and allows changes if applicable
// used by draw iten, point item and block selection description
void draw_flags(int x1, int y1, int* num, int *mpow, int view_only, int clear_background, int ignore_mpow)
{
   int ys = 10; // y spacing
   int frw = 6; // flag rectangle width
   int frh = 6; // flag rectangle height

   if (clear_background)
   {
      al_draw_filled_rectangle(x1-1, y1-1, x1+142, y1+(13*ys)+8, palette_color[0]);
      al_draw_rectangle(x1-2, y1-2, x1+142, y1+(13*ys)+9, palette_color[15], 1);
   }
   if (!ignore_mpow) if ((mouse_x > x1-2) && (mouse_x < x1+155) && (mouse_y > y1-2) && (mouse_y < y1+(13*ys)+9)) *mpow = 1;
   int highlight = draw_flag_rects(*num, x1, y1, frw, frh, ys, 14);
   if (!view_only)
   {
      if (highlight > -1)
      {
         al_draw_rectangle(x1-1, y1+(ys*highlight)-1, x1+frw+1, y1+frh+(ys*highlight)+1, palette_color[15], 1);
         if (mouse_b1)
         {
            while (mouse_b1) proc_controllers(); // wait for release
            if (highlight ==  0) (*num) ^= PM_BTILE_SOLID_PLAYER;
            if (highlight ==  1) (*num) ^= PM_BTILE_SOLID_ENEMY;
            if (highlight ==  2) (*num) ^= PM_BTILE_SOLID_ITEM;
            if (highlight ==  3) (*num) ^= PM_BTILE_SOLID_PBUL;
            if (highlight ==  4) (*num) ^= PM_BTILE_SOLID_EBUL;
            if (highlight ==  5) (*num) ^= PM_BTILE_SEMISOLID_PLAYER;
            if (highlight ==  6) (*num) ^= PM_BTILE_SEMISOLID_ENEMY;
            if (highlight ==  7) (*num) ^= PM_BTILE_SEMISOLID_ITEM;
            if (highlight ==  8) (*num) ^= PM_BTILE_BOMBABLE;
            if (highlight ==  9) (*num) ^= PM_BTILE_BREAKABLE_PBUL;
            if (highlight == 10) (*num) ^= PM_BTILE_BREAKABLE_EBUL;
            if (highlight == 11) (*num) ^= PM_BTILE_LADDER_MOVE;
            if (highlight == 12) (*num) ^= PM_BTILE_ROPE_MOVE;
            if (highlight == 13) (*num) ^= PM_BTILE_SECRET;
            if (highlight == 14) (*num) ^= PM_BTILE_SHOW_SELECT_WIN;
         }
      }
   }
   draw_flag_text(x1+12, y1, ys, 15, 0);
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


void edit_btile_attributes(void)
{
   int x, y;
   int mode = 0;
   int quit = 0;
   int gridlines = 0;

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

      draw_flag_text(ftx+4, fty, ys, 15, 1);

      int frw = 12;         // flag rectangle width
      int frh = 12;         // flag rectangle height
      int frx = ftx-frw-2;        // flag rectangle x
      int fry = fty - (frh/2)+4;  // flag rectangle y


      // position of 'currently selected' window (relative to flags)
      int csx = ftx-frw-1;
      int csy = fty-ys-1;

      int tx = 40;
      int ty1 = 670;

      if (mode == 0) // show flags for one tile
      {
         al_draw_rounded_rectangle(            tx, ty1, tx+508, ty1+26, 2, 2, palette_color[13], 1);
         al_draw_text(font, palette_color[15], tx+2, ty1+2,  0, "left  mouse button (b1) - select a tile");
         al_draw_text(font, palette_color[15], tx+2, ty1+16, 0, "right mouse button (b2) - paste selected tile flags to new tile");

         draw_and_proc_flag_rects_for_sa(current_selection, frx, fry, frw, frh, ys);
         al_draw_rectangle(                       csx-1,  csy-7, csx+195, csy+15, palette_color[10], 1);
         al_draw_bitmap(btile[current_selection], csx,    csy-6, 0);
         al_draw_textf(font, palette_color[4],    csx+24, csy,   0, "Current Selection %-2d  ", current_selection );
      }

      if (mode == 1) // show the tallied flags multiple
      {
         al_draw_rounded_rectangle(            tx, ty1, tx+302, ty1+26, 2, 2, palette_color[13], 1);
         al_draw_text(font, palette_color[15], tx+2, ty1+2,  0, "- draw selection rectangle");
         al_draw_text(font, palette_color[15], tx+2, ty1+16, 0, "- toggle flags for all selected tiles");

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


      // draw and process save button
      int sb_x = 375;
      int sb_w = 70;
      int sb_c = sb_x+(sb_w/2);
      int sb_y = 644;
      int sb_y2 = sb_y + 22;
      int sb_col = 10;

      al_draw_rounded_rectangle(sb_x, sb_y, sb_x+sb_w, sb_y2, 2, 2, palette_color[sb_col], 1);
      al_draw_textf(font, palette_color[sb_col], sb_c, sb_y+2,  ALLEGRO_ALIGN_CENTER, "Save");
      al_draw_textf(font, palette_color[sb_col], sb_c, sb_y+12, ALLEGRO_ALIGN_CENTER, "Changes");

      if ((mouse_x > sb_x) && (mouse_x < (sb_x+sb_w)) && (mouse_y > sb_y) && (mouse_y < sb_y2))
      {
         al_draw_rounded_rectangle(sb_x, sb_y, sb_x+sb_w, sb_y2, 2, 2, palette_color[sb_col], 2);
         if (mouse_b1)
         {
            while (mouse_b1) proc_controllers();
            save_sprit();
          }
      }

      // draw and process the mode button
      int mb_x = 40;
      int mb_x2 = mb_x + 180;
      int mb_xc = mb_x + (mb_x2-mb_x)/2;
      int mb_y = 644;
      int mb_y2 = mb_y + 22;
      int mb_col = 13;

      al_draw_rounded_rectangle(mb_x, mb_y, mb_x2, mb_y2, 2, 2, palette_color[mb_col], 1);

      al_draw_textf(font, palette_color[mb_col], mb_xc, mb_y+3,  ALLEGRO_ALIGN_CENTER, "Current Mode");
      if (mode == 0) sprintf(msg,"Change or Copy Single");
      if (mode == 1) sprintf(msg,"Change Multiple");
      al_draw_textf(font, palette_color[mb_col], mb_xc, mb_y+12, ALLEGRO_ALIGN_CENTER, msg);

      if ((mouse_x > mb_x) && (mouse_x < (mb_x2)) && (mouse_y > mb_y) && (mouse_y < mb_y2))
      {
         al_draw_rounded_rectangle(mb_x, mb_y, mb_x2, mb_y2, 2, 2, palette_color[mb_col], 2);
         if (mouse_b1)
         {
            while (mouse_b1) proc_controllers(); // wait for release
            if (++mode>1) mode = 0;
         }
      }

      // draw and process gridlines button
      int gb_x = 518;
      int gb_w = 120;
      int gb_c = gb_x + (gb_w/2);
      int gb_y = 644;
      int gb_y2 = gb_y + 12;
      int gb_col = 15;

      al_draw_rounded_rectangle(gb_x, gb_y, gb_x+gb_w, gb_y2, 2, 2, palette_color[gb_col], 1);
      if (gridlines) al_draw_textf(font, palette_color[gb_col], gb_c, gb_y+2, ALLEGRO_ALIGN_CENTER, "Gridlines:ON ");
      else           al_draw_textf(font, palette_color[gb_col], gb_c, gb_y+2, ALLEGRO_ALIGN_CENTER, "Gridlines:OFF");

      if ((mouse_x > gb_x) && (mouse_x < (gb_x+gb_w)) && (mouse_y > gb_y) && (mouse_y < gb_y+20))
      {
         al_draw_rounded_rectangle(gb_x, gb_y, gb_x+gb_w, gb_y2, 2, 2, palette_color[gb_col], 2);
         if (mouse_b1)
         {
            while (mouse_b1) proc_controllers();
            gridlines = !gridlines;
          }
      }
      while (key[ALLEGRO_KEY_ESCAPE])
      {
         proc_controllers();
         quit = 1;
      }
   }
   em_set_swbl();
}




void draw_gridlines_and_frame(int x1, int y1, int x2, int y2, int fd, int fc, int fw, int gd, int gc, int gw)
{
   if (gd) // gridline draw
   {
      for (int x=x1; x<=x2; x+=20) al_draw_line(x, y1, x, y2, palette_color[gc], gw);
      for (int y=y1; y<=y2; y+=20) al_draw_line(x1, y, x2, y, palette_color[gc], gw);
   }
   if (fd) al_draw_rectangle(x1, y1, x2, y2, palette_color[fc], fw);
}






int draw_and_process_button(int x, int y, const char * text, int c1, int c2, int center)
{
   // draw and process button
   int lbx = x;
   int lby = y;
   int lbw = 4 + strlen(text) * 8;
   int lbh = 12;
   int lbx2 = lbx + lbw;
   int lby2 = lby + lbh;

   if (center)
   {
      lbx =  lbx - (strlen(text) * 4) - 2;
      lbx2 = lbx + (strlen(text) * 8) + 4;
   }



   al_draw_textf(font, palette_color[c1], lbx+2,  lby+2, 0, text);
   al_draw_rectangle(lbx, lby, lbx2, lby2, palette_color[c1], 0);

   if ((mouse_x > lbx) && (mouse_x < lbx2) && (mouse_y > lby) && (mouse_y < lby2))
   {
      al_draw_textf(font, palette_color[c2], lbx+2,  lby+2, 0, text);
      al_draw_rectangle(lbx, lby, lbx2, lby2, palette_color[c2], 0);
      if (mouse_b1)
      {
         while (mouse_b1) proc_controllers();
         return 1;
      }
   }
   return 0;
}

// this one has abitrary first and second bmp file
void copy_tiles(void)
{
   int quit = 0;
   int gridlines = 1;
   al_set_target_backbuffer(display);
   al_show_mouse_cursor(display);
   ALLEGRO_BITMAP *qtmp = al_create_bitmap(20, 20);
   ALLEGRO_BITMAP *qtmp2 = al_create_bitmap(20, 20);

   // main bitmap ------------------
   char b1_fn[100];
   char b1_fn2[100];
   sprintf(b1_fn2, "bitmaps\\block_tiles.bmp");

   // convert to 'ALLEGRO_FS_ENTRY' to get platform specific fully qualified path
   ALLEGRO_FS_ENTRY *FS_fname1 = al_create_fs_entry(b1_fn2);
   sprintf(b1_fn, "%s", al_get_fs_entry_name(FS_fname1));

   ALLEGRO_BITMAP *b1 = al_load_bitmap(b1_fn);
   if (!b1)
   {
      sprintf(msg, "Error loading tiles from:%s", b1_fn);
      m_err(msg);
   }

   int b1_x = 10;
   int b1_y = 20;
   int b1_w = al_get_bitmap_width(b1);
   int b1_h = al_get_bitmap_height(b1);
   int b1_x2 = b1_x + b1_w;
   int b1_y2 = b1_y + b1_h;
   int b1_tw = b1_w/20; // tile grid width
   int reload_b1 = 0;

   // second bitmap ------------------
   char b2_fn[100];
   char b2_fn2[100];
   sprintf(b2_fn2, "bitmaps\\tiles.bmp");

   // convert to 'ALLEGRO_FS_ENTRY' to get platform specific fully qualified path
   ALLEGRO_FS_ENTRY *FS_fname2 = al_create_fs_entry(b2_fn2);
   sprintf(b2_fn, "%s", al_get_fs_entry_name(FS_fname2));

   ALLEGRO_BITMAP *b2 = al_load_bitmap(b2_fn);
   if (!b2)
   {
      sprintf(msg, "Error loading tiles from:%s", b2_fn);
      m_err(msg);
   }
   int b2_x = 700;
   int b2_y = 20;
   int b2_w = al_get_bitmap_width(b2);
   int b2_h = al_get_bitmap_height(b2);
   int b2_tw = b2_w/20; // tile grid width
   int b2_x2 = b2_x + b2_w;
   int b2_y2 = b2_y + b2_h;
   int reload_b2 = 0;

   while (!quit)
   {
      if (reload_b1)
      {
         al_destroy_bitmap(b1);
         sprintf(b1_fn, "bitmaps\\");
         if (mw_file_select("Load Bitmap File", b1_fn, ".bmp", 0)) b1 = al_load_bitmap(b1_fn);
         if (!b2) m_err("Load Error");
         else
         {
            b1_w = al_get_bitmap_width(b1);
            b1_h = al_get_bitmap_height(b1);
            b1_x2 = b1_x + b1_w;
            b1_y2 = b1_y + b1_h;
            b1_tw = b1_w/20; // tile grid width
         }
         reload_b1 = 0;
      }

      if (reload_b2 == 1)
      {
         al_destroy_bitmap(b2);
         sprintf(b2_fn, "bitmaps\\");
         if (mw_file_select("Load Bitmap File", b2_fn, ".bmp", 0)) b2 = al_load_bitmap(b2_fn);
         if (!b2) m_err("Load Error");
         else
         {
            b2_w = al_get_bitmap_width(b2);
            b2_h = al_get_bitmap_height(b2);
            b2_x2 = b2_x + b2_w;
            b2_y2 = b2_y + b2_h;
            b2_tw = b2_w/20; // tile grid width
         }
         reload_b2 = 0;
      }

      if (reload_b2 == 2) // load and scale to 1/2
      {
         ALLEGRO_BITMAP *db2 = NULL;
         sprintf(b2_fn, "bitmaps\\");
         if (mw_file_select("Load Bitmap File", b2_fn, ".bmp", 0)) db2 = al_load_bitmap(b2_fn);
         if (!db2) m_err("Load Error");
         else
         {
            int db2_w = al_get_bitmap_width(db2);
            int db2_h = al_get_bitmap_height(db2);
            al_destroy_bitmap(b2);
            b2 = al_create_bitmap(db2_w/2, db2_h/2);
            al_set_target_bitmap(b2);
            al_clear_to_color(al_map_rgb(0, 0, 0));
            al_draw_scaled_bitmap(db2, 0, 0, db2_w, db2_h, 0, 0, db2_w/2, db2_h/2, 0);
            al_set_target_backbuffer(display);
            al_destroy_bitmap(db2);

            b2_w = al_get_bitmap_width(b2);
            b2_h = al_get_bitmap_height(b2);
            b2_tw = b2_w/20; // tile grid width
            b2_x2 = b2_x + b2_w;
            b2_y2 = b2_y + b2_h;
         }
         reload_b2 = 0;
      }


      proc_controllers();
      al_flip_display();
      al_clear_to_color(al_map_rgb(0,0,0));

      // show the draw item
      int dix = b1_x + 1;
      int diy = b1_y + b1_h + 28;
      al_draw_rounded_rectangle(dix, diy, dix+170, diy+22, 2, 2, palette_color[13], 1);
      al_draw_bitmap(qtmp, dix+2, diy+1, 0);
      al_draw_textf(font, palette_color[13], dix+26, diy+3,  0, "Current Draw Item");

      // draw and process gridlines button
      if      ((gridlines == 1) && (draw_and_process_button(b1_x+200, b1_y+b1_h + 2, "Gridlines:ON", 15, 14, 0))) gridlines = 0;
      else if ((gridlines == 0) && (draw_and_process_button(b1_x+200, b1_y+b1_h + 2, "Gridlines:OFF", 15, 14, 0))) gridlines = 1;



      // second bitmap -----------------------------------------------------------------------------------------------
      // draw the second bitmap to copy from only
      al_draw_bitmap(b2, b2_x, b2_y, 0);

      // title with filename only, no path
      ALLEGRO_PATH *ap = al_create_path(b2_fn);
      al_draw_textf(font, palette_color[15], b2_x+(b2_w/2),  b2_y-10, ALLEGRO_ALIGN_CENTER, "%s", al_get_path_filename(ap));
      al_destroy_path(ap);

      if (draw_and_process_button(b2_x,    b2_y-12, "Load",     15, 14, 0)) reload_b2 = 1;
      if (draw_and_process_button(b2_x+40, b2_y-12, "Load 1/2", 15, 14, 0)) reload_b2 = 2;

      // draw a dim rectangle around the entire grid
      draw_gridlines_and_frame(b2_x, b2_y, b2_x2, b2_y2, 1, 15+64, 1, gridlines, 15+128, 0 );


      // is mouse on grid
      if ((mouse_x > b2_x) && (mouse_x < (b2_x + b2_w)) && (mouse_y > b2_y) && (mouse_y < (b2_y+b2_h)))
      {
         // draw a rectangle around the entire grid
         draw_gridlines_and_frame(b2_x, b2_y, b2_x2, b2_y2, 1, 15, 1, gridlines, 15+64, 0 );

         int mx = (mouse_x-b2_x)/20;
         int my = (mouse_y-b2_y)/20;
         int pointer = mx + my*b2_tw;

         // show what tile the mouse is pointing at
         al_draw_rectangle(b2_x+mx*20, b2_y+my*20, b2_x+mx*20+20, b2_y+my*20+20, palette_color[15], 0);

         al_set_target_bitmap(qtmp2);
         al_clear_to_color(al_map_rgb(0, 0, 0));
         al_draw_bitmap_region(b2, mx*20, my*20, 20, 20, 0, 0, 0);
         al_set_target_backbuffer(display);

         // draw b2_pointer button
         int b2p_x1= b2_x+1;
         int b2p_w1= 160;
         int b2p_y1= b2_y+b2_h;

         al_draw_rounded_rectangle(b2p_x1, b2p_y1, b2p_x1+b2p_w1, b2p_y1+22, 2, 2, palette_color[15], 1);
         al_draw_bitmap(qtmp2, b2p_x1+2, b2p_y1+1, 0);
         al_draw_textf(font, palette_color[15], b2p_x1+26, b2p_y1+3,  0, "pointer:%d", pointer);
         al_draw_textf(font, palette_color[15], b2p_x1+26, b2p_y1+12, 0, "mouse b2 to copy");

         if (mouse_b2)
         {
            while (mouse_b2) proc_controllers();
            al_set_target_bitmap(qtmp);
            al_clear_to_color(al_map_rgb(0, 0, 0));
            al_draw_bitmap_region(b2, mx*20, my*20, 20, 20, 0, 0, 0);
            al_set_target_backbuffer(display);
         }
      }

      // main bitmap -----------------------------------------------------------------------------------------------
      // draw the main bitmap
      al_draw_bitmap(b1, b1_x, b1_y, 0);

      // title with filename only, no path
      ALLEGRO_PATH *ap1 = al_create_path(b1_fn);
      al_draw_textf(font, palette_color[15], b1_x+(b1_w/2),  b1_y-10, ALLEGRO_ALIGN_CENTER, "%s", al_get_path_filename(ap1));
      al_destroy_path(ap1);

      if (draw_and_process_button(b1_x,     b1_y-12, "Load", 15, 14, 0)) reload_b1 = 1;
      if (draw_and_process_button(b1_x2-36, b1_y-12, "Save", 15, 10, 0)) al_save_bitmap(b1_fn, b1);

      draw_gridlines_and_frame(b1_x, b1_y, b1_x2, b1_y2, 1, 15+64, 1, gridlines, 15+128, 0 );

      // is mouse on grid
      if ((mouse_x > b1_x) && (mouse_x < (b1_x + b1_w)) && (mouse_y > b1_y) && (mouse_y < (b1_y+b1_h)))
      {
         draw_gridlines_and_frame(b1_x, b1_y, b1_x2, b1_y2, 1, 15, 1, gridlines, 15+64, 0 );

         int mx = (mouse_x-b1_x)/20;
         int my = (mouse_y-b1_y)/20;
         int pointer = mx + my*b1_tw;


         int mx1 = b1_x+mx*20;
         int my1 = b1_y+my*20;
         int mx2 = mx1 + 20;
         int my2 = my1 + 20;

         // show what tile the mouse is pointing at
         al_draw_rectangle(mx1, my1, mx2, my2, palette_color[15], 0);

         al_set_target_bitmap(qtmp2);
         al_clear_to_color(al_map_rgb(0, 0, 0));
         al_draw_bitmap_region(b1, mx*20, my*20, 20, 20, 0, 0, 0);
         al_set_target_backbuffer(display);

         // draw b1_pointer button
         int b1p_x1= b1_x+1;
         int b1p_w1= 160;
         int b1p_y1= b1_y+b1_h;

         al_draw_rounded_rectangle(b1p_x1, b1p_y1, b1p_x1+b1p_w1, b1p_y1+22, 2, 2, palette_color[15], 1);
         al_draw_bitmap(qtmp2, b1p_x1+2, b1p_y1+1, 0);
         al_draw_textf(font, palette_color[15], b1p_x1+26, b1p_y1+3,  0, "pointer:%d", pointer);
         al_draw_textf(font, palette_color[15], b1p_x1+26, b1p_y1+12, 0, "mouse b2 to copy");

         if (mouse_b1)
         {
            while (mouse_b1) proc_controllers();
            mx1 = mx*20;
            my1 = my*20;
            mx2 = mx1+20;
            my2 = my1+20;
            al_set_target_bitmap(b1);                                       // point at bitamp 1
            al_draw_filled_rectangle(mx1, my1, mx2, my2, palette_color[0]); // erase
            al_draw_bitmap(qtmp, mx1, my1, 0);                              // draw new tile
            al_set_target_backbuffer(display);
         }

         if (mouse_b2)
         {
            while (mouse_b2) proc_controllers();
            al_set_target_bitmap(qtmp);
            al_clear_to_color(al_map_rgb(0, 0, 0));
            al_draw_bitmap_region(b1, mx*20, my*20, 20, 20, 0, 0, 0);
            al_set_target_backbuffer(display);
         }

         al_draw_textf(font, palette_color[13], dix+26, diy+12, 0, "mouse b1 to paste"); // only show this line if mouse on main grid
      }
      while (key[ALLEGRO_KEY_ESCAPE])
      {
         proc_controllers();
         quit = 1;
      }
   }
   al_destroy_bitmap(b1);
   al_destroy_bitmap(b2);
   al_destroy_bitmap(qtmp);
   al_destroy_bitmap(qtmp2);
}





