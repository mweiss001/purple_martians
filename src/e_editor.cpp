// e_editor.cpp (20100220 cleanup)
#include "pm.h"

// draws item on dtemp, with rotation if required
void get_item_draw_shape(int i)
{
   al_set_target_bitmap(dtemp);
   al_clear_to_color(al_map_rgb(0,0,0));
   int drawn = 0;
   int shape = item[i][1];                       // get shape
   if (shape > 999) shape = zz[0][shape-1000];   // ans
   // doors needs to get special colorized shapes from door_bitmap[][]
   if (item[i][0] == 1)
   {
      int col = item[i][6];
      //int text_col = col;
      if (item[i][13] == 448) // old style door shape
      {
         //text_col = 15;                // all old style door use white lettering
         //if (col == 15) text_col = 14;  // except for white!
         int shape = item[i][1];       // get shape
         int si = shape-448;           // convert to index to bitmap sequence
         //draw_sprite(dtemp, door_bitmap[1][col][si], 0, 0);
         al_draw_bitmap(door_bitmap[1][col][si], 0, 0, 0);
      }
      else // new style doors
      {
         //if (col == 0) text_col = 15;    // all use same color as door, except for door color 0
         int an = zz[1][83];             // cheat and use shape index from base door animation sequence
         if (item[i][8] == 0) an = 7-an; // exit only, run the sequence backwards
         al_draw_bitmap(door_bitmap[0][col][an], 0, 0, 0);
      }
      drawn = 1;
   }

   // these types need rotation
   if (item[i][0] == 11) // rocket
   {
      //rotate_sprite(dtemp, memory_bitmap[shape], 0, 0, itofix(item[i][10]/10));
      al_draw_bitmap(memory_bitmap[shape], 0, 0, 0);
      drawn = 1;
   }

   // every other item draws like this
   if (!drawn) al_draw_bitmap(memory_bitmap[shape], 0, 0, 0);
}

void draw_item_shape(int i, int x, int y)
{
   get_item_draw_shape(i);
   al_set_target_backbuffer(display);
   al_draw_bitmap(dtemp, x, y, 0);
}

void draw_enemy_shape(int e, int x, int y)
{
   get_enemy_draw_shape(e);
   al_set_target_backbuffer(display);
   al_draw_bitmap(dtemp, x, y, 0);
}

void draw_PDE_shape(int pde, int x, int y)
{
   extern int PDEi[100][32];
   int a = PDEi[pde][1]; // bmp or ans
   if (a > 999) a = zz[5][a-1000]; // ans
   al_set_target_backbuffer(display);
   al_draw_bitmap(memory_bitmap[a], x, y, 0);
}

void show_draw_item_cursor(void)
{
   extern int PDEi[100][32];
   extern int wx, wy;
   extern int draw_item_type;
   extern int draw_item_num;
   extern int point_item_type;
   extern int point_item_num;

   int x100 = mouse_x/20 + wx;
   int y100 = mouse_y/20 + wy;

   if (x100>99) x100 = 99;
   if (y100>99) y100 = 99;

   int x = (x100-wx)*20;
   int y = (y100-wy)*20;

   // determine is shapes are the same
   int same = 0;

   if ((draw_item_type == 1) && (point_item_type == 1)) // both blocks
      if (draw_item_num == point_item_num) same = 1;    // same block


   if ((draw_item_type == 2) && (point_item_type == 2))      // both items
      if (item[draw_item_num][0] == item[point_item_num][0]) // same type
         same = 1;

   if ((draw_item_type == 3) && (point_item_type == 3))  // both enemies
      if (Ei[draw_item_num][0] == Ei[point_item_num][0]) // same type
         same = 1;


   if (draw_item_type == 5) //PDE
   {
      int pn = PDEi[draw_item_num][0];
      //int pde_type;
      int pde_sub_type;

      if ( pn < 99) // enemy
      {
         //pde_type = 3;
         pde_sub_type = pn;
         if (point_item_type == 3)      // both enemies
            if (pde_sub_type == Ei[point_item_num][0]) // same type
               same = 1;
      }

      if ((pn > 99) && (pn < 200)) // item
      {
         //pde_type = 2;
         pde_sub_type = pn-100;
         if (point_item_type == 2)      // both items
            if (pde_sub_type == item[point_item_num][0]) // same type
               same = 1;
      }
   }
   al_draw_filled_rectangle(x, y, x+20, y+20, palette_color[0]);
   if (same) al_draw_bitmap(memory_bitmap[255], x, y, 0);
   else switch (draw_item_type)
   {
      case 1: al_draw_bitmap(memory_bitmap[draw_item_num], x, y, 0); break;
      case 2: draw_item_shape(draw_item_num, x, y); break;
      case 3: draw_enemy_shape(draw_item_num, x, y); break;
      case 5: draw_PDE_shape(draw_item_num, x, y);  break;
   }
}

void set_block_range(void)
{
   extern int draw_item_num;
   int a, b;
   for (a = bx1; a < bx2; a++)       // cycle the range
      for (b = by1; b < by2; b++)
      {
         if ( (bx2-bx1==1) && (by2-by1==1) ) //single
            l[a][b] = draw_item_num;

         if ( (bx2-bx1>1) && (by2-by1>1) ) // box shape with corners
         {
            if ((draw_item_num >= 161) && (draw_item_num <= 163))  // lined platform shape draw special
            {
               l[a][b] = 162; // default
               if (a == bx1)   l[a][b] = 161;
               if (a == bx2-1) l[a][b] = 163;
            }
            else if ( (draw_item_num >= 33) && (draw_item_num <= 35) ) // semi solid screen draw special
            {
               l[a][b] = 34; // default
               if (b == by1)   l[a][b] = 33;
               if (b == by2-1) l[a][b] = 35;
            }
            else if ((draw_item_num >= 177) && (draw_item_num <= 179))  // brown brick shape draw special
            {
               l[a][b] = 178; // default
               if (a == bx1)   l[a][b] = 177;
               if (a == bx2-1) l[a][b] = 179;
            }
            else if ((draw_item_num >= 576) && (draw_item_num <= 592)) // purple pipe box with corners
            {
               if (b == by1  ) l[a][b] = 582; // horizontal through
               if (b == by2-1) l[a][b] = 582; // horizontal through
               if (a == bx1)
               {
                  if (b == by1) l[a][b] = 576; // up-right corner
                  else if (b == by2-1) l[a][b] = 578; // left-up corner
                  else l[a][b] = 580; // vertical through
               }
               if (a == bx2-1)
               {
                  if (b == by1) l[a][b] = 577; // right-down corner
                  else if (b == by2-1) l[a][b] = 579; // down-left corner
                  else l[a][b] = 580; //  vertical through
               }
            }
            else if ((draw_item_num >= 576+32) && (draw_item_num <= 592+32)) // wires box with corners
            {
               if (b == by1  ) l[a][b] = 582+32; // horizontal through
               if (b == by2-1) l[a][b] = 582+32; // horizontal through
               if (a == bx1)
               {
                  if (b == by1) l[a][b] = 576+32; // up-right corner
                  else if (b == by2-1) l[a][b] = 578+32; // left-up corner
                  else l[a][b] = 580+32; // vertical through
               }
               if (a == bx2-1)
               {
                  if (b == by1) l[a][b] = 577+32; // right-down corner
                  else if (b == by2-1) l[a][b] = 579+32; // down-left corner
                  else l[a][b] = 580+32; //  vertical through
               }
            }
            else if ((draw_item_num >= 576+64) && (draw_item_num <= 592+64)) // bricks with corners
            {
               l[a][b] = 592+64; // center
               if (b == by1  ) l[a][b] = 582+64; // horizontal through
               if (b == by2-1) l[a][b] = 583+64; // horizontal through
               if (a == bx1)
               {
                  if (b == by1) l[a][b] = 576+64; // up-right corner
                  else if (b == by2-1) l[a][b] = 578+64; // left-up corner
                  else l[a][b] = 580+64; // vertical through
               }
               if (a == bx2-1)
               {
                  if (b == by1) l[a][b] = 577+64; // right-down corner
                  else if (b == by2-1) l[a][b] = 579+64; // down-left corner
                  else l[a][b] = 581+64; //  vertical through
               }
            }
            else if ((draw_item_num >= 576+96) && (draw_item_num <= 592+128)) // rainbows
            {
               if (b == by1  ) l[a][b] = 582+96; // horizontal through
               if (b == by2-1) l[a][b] = 583+96; // horizontal through
               if (a == bx1)
               {
                  if (b == by1) l[a][b] = 576+96; // up-right corner
                  else if (b == by2-1) l[a][b] = 578+96; // left-up corner
                  else l[a][b] = 580+96; // vertical through
               }
               if (a == bx2-1)
               {
                  if (b == by1) l[a][b] = 577+96; // right-down corner
                  else if (b == by2-1) l[a][b] = 579+96; // down-left corner
                  else l[a][b] = 581+96; //  vertical through
               }
            }
            else l[a][b] = draw_item_num;
         }
         if ( (bx2-bx1>1) && (by2-by1==1) ) // horizontal line with end caps
         {
            l[a][b] = draw_item_num;
            if ((draw_item_num >= 161)
              && (draw_item_num <= 163))  // lined platform shape draw special
            {
               l[a][b] = 162; // default
               if (a == bx1)   l[a][b] = 161;
               if (a == bx2-1) l[a][b] = 163;
            }
            if ((draw_item_num >= 177) && (draw_item_num <= 179))  // brown brick shape draw special
            {
               l[a][b] = 178; // default
               if (a == bx1)   l[a][b] = 177;
               if (a == bx2-1) l[a][b] = 179;
            }
            if ((draw_item_num == 582)
                  || (draw_item_num == 583)
                  || (draw_item_num == 590)
                  || (draw_item_num == 588)) // purple pipes
            {
               l[a][b] = 582; // horizontal through
               if (a == bx1) l[a][b] = 590; // left end cap
               if (a == bx2-1) l[a][b] = 588; // right end cap
            }
            if ((draw_item_num == 582+32)
                  || (draw_item_num == 583+32)
                  || (draw_item_num == 590+32)
                  || (draw_item_num == 588+32)) // wires
            {
               l[a][b] = 582+32; // horizontal through
               if (a == bx1) l[a][b] = 590+32; // left end cap
               if (a == bx2-1) l[a][b] = 588+32; // right end cap
            }
         }
         if ( (bx2-bx1==1) && (by2-by1>1) ) // vertical line with end caps
         {
            l[a][b] = draw_item_num;
            if ( (draw_item_num >= 33)
              && (draw_item_num <= 35) ) // semi solid screen draw special
            {
               l[a][b] = 34; // default
               if (b == by1)   l[a][b] = 33;
               if (b == by2-1) l[a][b] = 35;
            }
            if ((draw_item_num == 580)
                  || (draw_item_num == 581)
                  || (draw_item_num == 589)
                  || (draw_item_num == 590)) // purple pipes
            {
               l[a][b] = 580; // vertical through
               if (b == by1) l[a][b] = 591; // up end cap
               if (b == by2-1) l[a][b] = 589; // down end cap
            }
            if ((draw_item_num == 580+32)
                  || (draw_item_num == 581+32)
                  || (draw_item_num == 589+32)
                  || (draw_item_num == 590+32)) // wires
            {
               l[a][b] = 580+32; // vertical through
               if (b == by1) l[a][b] = 591+32; // up end cap
               if (b == by2-1) l[a][b] = 589+32; // down end cap
            }
         }
      } // end of cycle block range
}

void get_new_box(void) // keep the mouse !!
{
   extern int wx, wy;
   int z; // for swap

   bx2 = bx1; // set all three to intial
   by2 = by1;
   int x1 = bx1;
   int y1 = by1;
   int x2 = bx2;
   int y2 = by2;

   while (mouse_b1)
   {
      al_flip_display();
      proc_controllers();
      show_draw_item_cursor();
      process_scrolledge();
      update_editor_background();

      bx2 = (mouse_x)/20+wx; // set both with mouse pointer
      by2 = (mouse_y)/20+wy;

      x2 = bx2;  // set with mouse
      y2 = by2;

      x1 = bx1; // get inital in case it was swapped
      y1 = by1;

      // swap x1 and x2 if neccesary
      if (x1 > x2) { z = x1; x1 = x2; x2 = z;}
      if (y1 > y2) { z = y1; y1 = y2; y2 = z;}

      if (x1>99) x1 = 99;
      if (y1>99) y1 = 99;
      if (x2>99) x2 = 99;
      if (y2>99) y2 = 99;

      // show the selection rectangle
      al_draw_rectangle((x1-wx)*20, (y1-wy)*20, (x2-wx)*20+19, (y2-wy)*20+19, palette_color[127], 1);
      al_draw_textf(font, palette_color[15], 100, 20, 0, " x1:%d y2:%d ", x1, y1);
      al_draw_textf(font, palette_color[15], 100, 28, 0, " x2:%d y2:%d ", x2, y2);

   }
   // swap bx1 and bx2 if neccesary
   if (bx1 > bx2) { z = bx1; bx1 = bx2; bx2 = z; }
   if (by1 > by2) { z = by1; by1 = by2; by2 = z;}

   // always set second to one more
   bx2++;
   by2++;
}


void update_editor_background(void)
{
   extern int wx, wy;
   al_set_target_backbuffer(display);
   al_draw_bitmap_region(l2000, wx*20, wy*20, (SCREEN_W/20)*20, (SCREEN_H/20)*20, 0, 0, 0);

   int lesw = (SCREEN_W/20)*20-1;
   int lesh = (SCREEN_H/20)*20-1;
   if (lesw > 1999) lesw = 1999;
   if (lesh > 1999) lesh = 1999;
   al_draw_rectangle(0.5, 0.5, lesw, lesh, palette_color[9], 1);
}


int process_scrolledge(void)
{
   int scrolledge=10;
   extern int wx, wy;

   int old_wx = wx;
   int old_wy = wy;

   if (mouse_x < scrolledge) wx--;           // scroll left
   if (mouse_x > SCREEN_W-scrolledge) wx++;  // scroll right
   if (mouse_y < scrolledge) wy--;           // scroll up
   if (mouse_y > SCREEN_H-scrolledge) wy++;  // scroll down

   int rx = 100 - (SCREEN_W/20);
   int ry = 100 - (SCREEN_H/20);

   if (wx > rx) wx = rx;
   if (wy > ry) wy = ry;
   if (wx < 0) wx = 0;
   if (wy < 0) wy = 0;

   if ((old_wx != wx) || (old_wy != wy))
   {
      al_rest(.02);
   }

   return 0;
}
void pointer_text(int x, int y, int ty)
{
   if ((x<99) && (y < 99))
   {
      al_draw_text( font, palette_color[15], txc, ty-39, ALLEGRO_ALIGN_CENTER, "Pointer");
      al_draw_textf(font, palette_color[15], txc, ty-31, ALLEGRO_ALIGN_CENTER, "  x:%d    y:%d ", x, y);
   }
   extern int stx, sty, sux, suy;
   int b, d;
   int sey = -20;

   int rx1 = stx *20;    // source x
   int ry1 = sty *20;    // source y
   int rx2 = sux *20;    // sizes
   int ry2 = suy *20;
   int eib=0;
   int iib=0;
   int lib=0;

   al_fixed frx1 = al_itofix(rx1);    // source x
   al_fixed fry1 = al_itofix(ry1);    // source y
   al_fixed frx2 = al_itofix(rx2);    // sizes
   al_fixed fry2 = al_itofix(ry2);

   // count enemies in box
   for (b=0; b<100; b++) // check for enemies in box
      if (Ei[b][0]) // if active
         if (Efi[b][0] >= frx1)
            if (Efi[b][0] < frx2)
               if (Efi[b][1] >= fry1)
                  if (Efi[b][1] < fry2)
                     eib++;

   // count items in box
   for (b=0; b<500; b++) // check for items in box
      if (item[b][0]) // if active
         if (item[b][4] >= rx1)
            if (item[b][4] < rx2)
               if (item[b][5] >= ry1)
                  if (item[b][5] < ry2)
                     iib++;

   // count lifts in box
   for (d=0; d<num_lifts; d++)

         if (lifts[d].x1 >= rx1)
            if (lifts[d].x1 < rx2)
               if (lifts[d].y1 >= ry1)
                  if (lifts[d].y1 < ry2)
                     lib++;

   al_draw_rectangle(txc-70, ty+sey, txc+70, ty+sey+10,palette_color[14], 1);
   al_draw_rectangle(txc-70, ty+sey, txc+70, ty+sey+36,palette_color[14], 1);
   al_draw_rectangle(txc-70, ty+sey, txc+70, ty+sey+62,palette_color[14], 1);

   al_draw_text( font, palette_color[6], txc, ty+sey+1, ALLEGRO_ALIGN_CENTER, "Selection");
   al_draw_textf(font, palette_color[6], txc, ty+sey+11, ALLEGRO_ALIGN_CENTER, " x:%2d  y:%2d ", stx, sty);
   al_draw_textf(font, palette_color[6], txc, ty+sey+19, ALLEGRO_ALIGN_CENTER, " height:%d ", suy-sty);
   al_draw_textf(font, palette_color[6], txc, ty+sey+27, ALLEGRO_ALIGN_CENTER, " width:%d ", sux-stx);

   al_draw_textf(font, palette_color[7], txc, ty+sey+37, ALLEGRO_ALIGN_CENTER, " %d Enemies ", eib);
   al_draw_textf(font, palette_color[7], txc, ty+sey+45, ALLEGRO_ALIGN_CENTER, " %d Items ", iib);
   al_draw_textf(font, palette_color[7], txc, ty+sey+53, ALLEGRO_ALIGN_CENTER, " %d Lifts ", lib);
}



void do_brf(int x, int y, int flood_block)
{
   int f[100][100];                             // array of blocks to mark
   for (int a=0; a<100; a++)
      for (int b=0; b<100; b++) f[a][b] = 0;    // erase array

   int rb = l[x][y]; // block num to replace
   f[x][y] = 1;      // mark initial block pos in array


   //loop start
   int times = 0;
   int found = 0;
   do
   {
      times++;
      found = 0;
      for (int a=0; a<100; a++)
         for (int b=0; b<100; b++)
            if (f[a][b]) // iterate already marked
            {
               if ((a >  0) && (l[a-1][b]) == rb) // look left
               {
                  if (f[a-1][b] == 0) found++; // found unmarked
                  f[a-1][b] = 1; // mark it
               }
               if ((b >  0) && (l[a][b-1]) == rb) // look up
               {
                  if (f[a][b-1] == 0) found++; // found unmarked
                  f[a][b-1] = 1; // mark it
               }
               if ((a < 99) && (l[a+1][b]) == rb) // look right
               {
                  if (f[a+1][b] == 0) found++; // found unmarked
                  f[a+1][b] = 1; // mark it
               }
               if ((b < 99) && (l[a][b+1]) == rb) // look down
               {
                  if (f[a][b+1] == 0) found++; // found unmarked
                  f[a][b+1] = 1; // mark it
               }
            }

      // show progress (just because I can and it looks cool
      //printf("times:%d found:%d\n", times, found);
      for (int a1=0; a1<100; a1++)
         for (int b1=0; b1<100; b1++)
            if (f[a1][b1]) l[a1][b1] = flood_block;

      draw_big(1);
      show_big();

      al_rest(.02);
      al_flip_display();

   } while (found);


//      // or we could just do it instantly at a the end
//      // do the swap
//      for (int a=0; a<100; a++)
//         for (int b=0; b<100; b++)
//            if (f[a][b]) l[a][b] = flood_block;


   draw_big(1);

}





int zoom_full_screen(int wx, int wy, int draw_item)
{
   extern int draw_item_type;
   extern int draw_item_num;
   extern int ft_level_header[20];
//   extern char sel_filename[500];

   extern int stx, sty, sux, suy;
   int jh;
   extern int copy_mode;
   extern int brf_mode;

   extern int copy_blocks;
   extern int copy_enemies;
   extern int copy_items;
   extern int copy_lifts;

   int ty = 60;
   int x1, y1, x2, y2;

   int x, y, exit =0;

   copy_mode = 0;
   brf_mode = 0;




//   // return window size
//   int rw_x1, rw_y1, rw_x2, rw_y2;
//   int rw_w = (SCREEN_W/20);
//   int rw_h = (SCREEN_H/20);


// i don't like this, its usually too big
// and it always reset everytime you come here
// the old way remembers the last selection

   // initial selection
   // set initial selection to screen position of editor
//   stx = wx;
//   sty = wy;
//   sux = stx + (SCREEN_W/20);
//   suy = sty + (SCREEN_H/20);



   while (mouse_b2) proc_controllers();


   while (!exit)
   {
      al_set_target_backbuffer(display);
      al_flip_display();
      al_clear_to_color(al_map_rgb(0,0,0));

      proc_controllers();

      al_rest(mouse_loop_pause);

      draw_big(1);
      show_big();

      title((char *)"Zoom Full Screen",  0, 15, 13);
      // menu and buttons
      int bc1 = 10;
      int bc0 = 9;

      jh=4;
      if (copy_blocks) { x = bc1; sprintf(msg,"Blocks On "); }
      else             { x = bc0; sprintf(msg,"Blocks Off"); }
      al_draw_text(font, palette_color[x], txc, ty+(jh*12)+1,  ALLEGRO_ALIGN_CENTER, msg);
      al_draw_rectangle(txc-70, ty+(jh*12), txc+70, ty+(jh*12)+10, palette_color[x], 1);

      jh=5;
      if (copy_enemies){ x = bc1; sprintf(msg,"Enemies On "); }
      else             { x = bc0; sprintf(msg,"Enemies Off"); }
      al_draw_text(font, palette_color[x], txc, ty+(jh*12)+1,  ALLEGRO_ALIGN_CENTER, msg);
      al_draw_rectangle(txc-70, ty+(jh*12), txc+70, ty+(jh*12)+10, palette_color[x], 1);

      jh=6;
      if (copy_items) { x = bc1; sprintf(msg,"Items On "); }
      else            { x = bc0; sprintf(msg,"Items Off"); }
      al_draw_text(font, palette_color[x], txc, ty+(jh*12)+1,  ALLEGRO_ALIGN_CENTER, msg);
      al_draw_rectangle(txc-70, ty+(jh*12), txc+70, ty+(jh*12)+10, palette_color[x], 1);

      jh=7;
      if (copy_lifts) { x = bc1; sprintf(msg,"Lifts On "); }
      else            { x = bc0; sprintf(msg,"Lifts Off"); }
      al_draw_text(font, palette_color[x], txc, ty+(jh*12)+1,  ALLEGRO_ALIGN_CENTER, msg);
      al_draw_rectangle(txc-70, ty+(jh*12), txc+70, ty+(jh*12)+10, palette_color[x], 1);

      jh=9;
      al_draw_rectangle(txc-70, ty+(jh*12), txc+70, ty+(jh*12)+10, palette_color[9], 1);
      al_draw_text(font, palette_color[9], txc, ty+(jh*12)+1,  ALLEGRO_ALIGN_CENTER, "Move Selection");

      jh=10;
      al_draw_rectangle(txc-70, ty+(jh*12), txc+70, ty+(jh*12)+10, palette_color[9], 1);
      al_draw_text(font, palette_color[9], txc, ty+(jh*12)+1,  ALLEGRO_ALIGN_CENTER, "Clear Selection");

      jh=11;
      if (copy_mode) x = 10; else x = 9;
      al_draw_rectangle(txc-70, ty+(jh*12), txc+70, ty+(jh*12)+10, palette_color[x], 1);
      al_draw_text(font, palette_color[x], txc, ty+(jh*12)+1,  ALLEGRO_ALIGN_CENTER, "Paste Selection");

      jh=12;
      al_draw_rectangle(txc-70, ty+(jh*12), txc+70, ty+(jh*12)+10, palette_color[9], 1);
      al_draw_text(font, palette_color[9], txc, ty+(jh*12)+1,  ALLEGRO_ALIGN_CENTER,"Save Selection" );

      jh=13;
      al_draw_rectangle(txc-70, ty+(jh*12), txc+70, ty+(jh*12)+10, palette_color[9], 1);
      al_draw_text(font, palette_color[9], txc, ty+(jh*12)+1,  ALLEGRO_ALIGN_CENTER, "Paste From Disk");

      jh=15;
      al_draw_rectangle(txc-70, ty+(jh*12), txc+70, ty+(jh*12)+10, palette_color[13], 1);
      al_draw_text(font, palette_color[13], txc, ty+(jh*12)+1,  ALLEGRO_ALIGN_CENTER, "Block Fill");

      jh=16;
      al_draw_rectangle(txc-70, ty+(jh*12), txc+70, ty+(jh*12)+10, palette_color[13], 1);
      al_draw_text(font, palette_color[13], txc, ty+(jh*12)+1,  ALLEGRO_ALIGN_CENTER,"Block Frame" );

      jh=17;
      if (brf_mode) x = 10; else x = 13;
      al_draw_rectangle(txc-70, ty+(jh*12), txc+70, ty+(jh*12)+10, palette_color[x], 1);
      al_draw_text(font, palette_color[x], txc, ty+(jh*12)+1,  ALLEGRO_ALIGN_CENTER, "Block Floodfill");

      // mark selection on map
      al_draw_rectangle(stx*db, sty*db, (sux*db)-1, (suy*db)-1, palette_color[14], 1);
      al_draw_text(font, palette_color[14], stx*db+2, sty*db-11,  0, "selection");


#ifndef RELEASE
      if (key[ALLEGRO_KEY_R])
      {
         while (key[ALLEGRO_KEY_R]) proc_controllers();
         void do_rnd(void);
         do_rnd();
         Redraw = 1;
      }
#endif

      // mouse on buttons
      if ((mouse_b1) && (mouse_x > txc-70) && (mouse_x < txc + 70) && (mouse_y > ty) && (mouse_y < ty+(18*12)))
      {
         while (mouse_b1) proc_controllers(); // wait for release
         int mb = (mouse_y - ty) / 12;
         switch(mb)
         {
            case 4:
               copy_blocks  = !copy_blocks;
               if (copy_mode) draw_fsel();
            break;
            case 5:
               copy_enemies = !copy_enemies;
               if (copy_mode) draw_fsel();
            break;
            case 6:
               copy_items = !copy_items;
               if (copy_mode) draw_fsel();
            break;

            case 7:
               copy_lifts = !copy_lifts;
               if (copy_mode) draw_fsel();
            break;

            case 9: // move selection button clicked
               if (copy_mode) copy_mode = 0;
               else
               {
                  copy_mode = 1;
                  save_selection(0); // just puts in ft_
                  draw_fsel();
                  do_clear();
               }
            break;
            case 10: // clear selection button clicked
               do_clear();
            break;
            case 11: // paste selection button clicked
               if (copy_mode) copy_mode = 0;
               else
               {
                  copy_mode = 1;
                  save_selection(0); // just puts in ft_
                  draw_fsel();
               }
            break;
            case 12: // save selection button clicked
               save_selection(1); // puts in ft_ and saves to disk
            break;
            case 13: // paste from disk button clicked

               if (load_selection())
               {
                  copy_mode = 1;
                  draw_fsel();
               }
            break;
            case 15: // Block Fill
               if (draw_item_type == 1)
               {
                  for (int x=stx; x<sux; x++)
                     for (int y=sty; y<suy; y++)
                        l[x][y] = draw_item_num;
               }
            break;
            case 16: // Block Outline
               if (draw_item_type == 1)
               {
                  for (x=stx; x<sux; x++)
                  {
                     l[x][sty] = draw_item_num;
                     l[x][suy-1] = draw_item_num;
                  }
                  for (y=sty; y<suy; y++)
                  {
                     l[stx][y] = draw_item_num;
                     l[sux-1][y] = draw_item_num;
                  }
               }
            break;
            case 17: // Block Replace/Floodfill
               if (draw_item_type == 1) brf_mode = !brf_mode;
            break;
         } // end of switch button
         Redraw = 1;
      } // end of mouse pressed on menu




      // get the mouse block index on the map
      x1 = mouse_x/db;
      y1 = mouse_y/db;

      // show information about selection and pointer
      pointer_text(x1, y1, ty);

      if ((x1 < 100) && (y1 < 100)) // if mouse pointer on map
      {
//         // show return window outline
//         if ((!copy_mode) && (!brf_mode))
//         {
//            // set ul corner from mouse pos
//            rw_x1 = x1;
//            rw_y1 = y1;
//            // if lr pos is off screen, adjust ul pos
//            if (rw_x1 > 100-rw_w) rw_x1 = 100-rw_w;
//            if (rw_y1 > 100-rw_h) rw_y1 = 100-rw_h;
//
//            // calc lr position
//            rw_x2 = rw_x1 + rw_w;
//            rw_y2 = rw_y1 + rw_h;
//
//            al_draw_text(font, palette_color[15], rw_x1*db+2, rw_y1*db-10, 0, "exit window");
//            al_draw_rectangle(rw_x1*db, rw_y1*db, rw_x2*db - 1, rw_y2*db - 1, palette_color[15], 1);
//         }

         if (copy_mode) // show copy outline window
         {
            x2 = x1 + ft_level_header[8];
            y2 = y1 + ft_level_header[9];
            if (x2 > 99) x2 = 99;
            if (y2 > 99) y2 = 99;

            al_set_clipping_rectangle(0, 0, les*db*100-1, les*db*100-1);
            al_draw_bitmap(ft_bmp, x1*db, y1*db, 0);
            al_draw_text(font, palette_color[42], x1*db+2, y1*db-11, 0, "paste selection");
            al_draw_rectangle(x1*db, y1*db, (x2+1)*db-1, (y2+1)*db-1, palette_color[10], 1);
            al_reset_clipping_rectangle();

         }

         if (mouse_b1)
         {
            if ((copy_mode) || (brf_mode)) while (mouse_b1) proc_controllers(); // wait for release
            if (copy_mode) do_fcopy(x1, y1);
            if (brf_mode) do_brf(x1, y1, draw_item_num);
            if ((!copy_mode) && (!brf_mode)) // get new selection
            {
               // initial selection
               stx = x1;
               sty = y1;
               sux = x1+1;
               suy = y1+1;

               while (mouse_b1)
               {
                  proc_controllers();
                  sux = (mouse_x/db)+1;
                  if (sux > 100) sux = 100;
                  suy = (mouse_y/db)+1;
                  if (suy > 100) suy = 100;

                  show_big();
                  // show selection rectangle
                  al_draw_rectangle(stx*db, sty*db, (sux*db)-1, (suy*db)-1, palette_color[14], 1);
                  al_draw_text(font, palette_color[14], stx*db+2, sty*db-11,  0, "selection");
                  al_flip_display();
                  al_rest(.02);
               }
               if (sux < stx) // swap if wrong order
               {
                  int temp = sux;
                  sux = stx;
                  stx = temp;
               }
               if (suy < sty)
               {
                  int temp = suy;
                  suy = sty;
                  sty = temp;
               }
               if (stx - sux == 0) sux++;  // don't allow zero size
               if (sty - suy == 0) suy++;  // don't allow zero size
            } // end of get new selection
         }

      } // end of if mouse pointer on map
      while ((mouse_b2) || (key[ALLEGRO_KEY_ESCAPE]))
      {
         proc_controllers();
         exit = 1;
      }
   } // end of while (!exit)
   return (y1*100)+x1;
}


void draw_item_info(int x, int y, int color, int type, int num)
{
   extern int e_first_num[50];
   extern int e_num_of_type[50];
   extern int item_first_num[20];
   extern char PDEt[100][20][40];
   extern int PDEi[100][32];
   extern char item_desc[20][40];
   extern char eitype_desc[50][32][40];
   int a, b;
   switch (type)
   {
      case 1:
         al_draw_bitmap(memory_bitmap[num], x, y, 0);
         al_draw_textf(font, palette_color[color], x+22, y+2, 0, "Block #%d",num);
         sprintf(msg, "Solid");  // default
         if (num < 32) sprintf(msg, "Empty");
         if ((num > 31) && (num < 64)) sprintf(msg, "Semi-Solid ");
         if ((num > 63) && (num < 96)) sprintf(msg, "Bombable");
         if ((num > 95) && (num < 128)) sprintf(msg, "Breakable");
         al_draw_text(font, palette_color[color], x+22, y+12, 0, msg);

      break;
      case 2:
         draw_item_shape(num, x, y);
         a = item[num][0]; // type
         al_draw_textf(font, palette_color[color], x+22, y+2, 0, "%s", item_desc[a]);
         al_draw_textf(font, palette_color[color], x+22, y+12, 0, "%d of %d", 1+num - item_first_num[a],item_num_of_type[a]);
      break;
      case 3:
         draw_enemy_shape(num, x, y);
         a = Ei[num][0]; // type
         al_draw_textf(font, palette_color[color], x+22, y+2, 0, "%s", eitype_desc[a][0]);
         al_draw_textf(font, palette_color[color], x+22, y+12, 0, "%d of %d", 1+num - e_first_num[a],e_num_of_type[a]);
      break;
      case 4:
      {
         int col = lifts[num].color;
         int width = lifts[num].width;
         if (width > 7) width = 7;
         for (a=0; a<10; a++)
            al_draw_rectangle(x+a, y+a, x+(width*20)-1-a, y+19-a, palette_color[col+((9-a)*16)], 1 );

         if ((lifts[num].width == 1) && (lifts[num].height > 1)) // rotate lift name for vertical lifts
            rtextout_centre(NULL, lifts[num].lift_name, x+(width*10), y, color+160, 1, 64, 1 );
         else
            al_draw_text(font, palette_color[col+160], x+(width*10), y+6, ALLEGRO_ALIGN_CENTER, lifts[num].lift_name);

      }
      break;
      case 5:
         a = PDEi[num][1]; // bmp or ans
         if (a < NUM_SPRITES) b = a; // bmp
         if (a > 999) b = zz[5][a-1000]; // ans
         al_draw_bitmap(memory_bitmap[b], x, y, 0);
         a = Ei[num][0]; // type
         al_draw_text(font, palette_color[color], x+22, y+2, 0, "Special Item");
         al_draw_textf(font, palette_color[color], x+22, y+12, 0, "%s", PDEt[num][1]);
      break;
   }
}
int edit_menu(int el)
{
   level_editor_running = 1;
   int original_les = les;
   if (les != 1)
   {
      les = 1;
      rebuild_bitmaps();
      //set_ortho();
   }

   al_show_mouse_cursor(display);
   extern int PDEi[100][32];
   extern al_fixed PDEfx[100][16];
   extern char item_desc[20][40];

   resume_allowed = 0;

   int mpow;

   int em_quit = 0;
   extern int wx;
   extern int wy;

   int x100, y100, x2000, y2000;
   int a, b, c, d, e, x, y;

   // clamp screen size to 2000, 2000 if bigger
   int lesw = (SCREEN_W/20)*20-1;
   int lesh = (SCREEN_H/20)*20-1;
   if (lesw > 1999) lesw = 1999;
   if (lesh > 1999) lesh = 1999;

   check_s_window_pos(1);

   load_PDE();

   if (!el) load(); // load prompt
   else load_level(el, 0); // blind load

   draw_big(1);

   sort_enemy();
   item_sort();

   draw_item_type = 1;
   draw_item_num  = 0;

   initialize_zz();
   set_wx_from_start_block();

   text_setup(); // item, enemy, and  menu

   al_set_mouse_xy(display, SCREEN_W/2, SCREEN_H/2);
   proc_controllers();

   // clear_key array
   for (int k = ALLEGRO_KEY_A; k < ALLEGRO_KEY_MAX; k++) key[k] = 0;

   int draw_item_blink = 0;

   do
   {
      set_ortho();

      x100 = mouse_x/20 + wx;
      y100 = mouse_y/20 + wy;

      x2000 = mouse_x + wx*20;
      y2000 = mouse_y + wy*20;

      if (x100>99) x100 = 99;
      if (y100>99) y100 = 99;

      mpow = 0;  // is mouse pointer on a window?
      if ((mouse_x > status_window_x) && (mouse_x < status_window_x+status_window_w))
         if ((mouse_y > status_window_y-12) && (mouse_y < status_window_y+status_window_h))
            if (status_window_active)  mpow = 1;

      if ((mouse_x > select_window_x) && (mouse_x < select_window_x + select_window_w))
         if ((mouse_y > select_window_y) && (mouse_y < select_window_y + select_window_h))
            if (select_window_active) mpow = 1;

      if (!mpow)  // find point item
      {
         // block by default
         point_item_type = 1;
         point_item_num = l[x100][y100];

         int max_ob = 20;                  // max objects to find
         int ob = 0;                       // objects found
         int mo[max_ob][2];                // array of objects found
         for (a=0; a<max_ob; a++)          // clear array
         {
             mo[a][0] = 0;
             mo[a][1] = 1;
         }
         for (d=0; d<500; d++) // check for item
            if ((item[d][0]) && (ob < max_ob))
            {
               a = item[d][4];
               b = item[d][5];
               if ( (x2000 >= a) && (x2000 <= (a+19)) && (y2000 > b) && (y2000 < (b+19)) )
               {
                   mo[ob][0] = 2;
                   mo[ob][1] = d;
                   ob++;
               }
            }
         for (e=0; e<100; e++) // check for enemy
            if ((Ei[e][0]) && (ob < max_ob))
            {
               a = al_fixtoi(Efi[e][0]);
               b = al_fixtoi(Efi[e][1]);
               if ( (x2000 >= (a)) && (x2000 <= (a+19)) && (y2000 > (b)) && (y2000 < (b+19)) )
               {
                   mo[ob][0] = 3;
                   mo[ob][1] = e;
                   ob++;
               }
            }
         for (d=0; d<num_lifts; d++) // check for lifts
         {
            a = lifts[d].x1;
            b = lifts[d].y1;
            if ( (x2000 >= a) && (x2000 <= (a+19)) && (y2000 > b) && (y2000 < (b+19) ) && (ob < max_ob))
            {
                mo[ob][0] = 4;
                mo[ob][1] = d;
                ob++;
            }
         }
         //al_draw_textf(font, palette_color[15], 100, 100, 0, "mouse is on: %d objects", ob);
         //for (a=0; a<ob; a++) al_draw_textf(font, palette_color[15], 100, 108+a*8, 0, "%d %d ", mo[a][0], mo[a][1]);

         // which one is the winner?
         if (ob)
         {
            int mm = mouse_x % 20;         // mouse position relative to block boundary
            int ss = 20/ob;                // step space
            int of = mm / ss;              // convert to offset into ob array
            point_item_type = mo[of][0];
            point_item_num  = mo[of][1];
            //al_draw_textf(font, palette_color[11], 100, 92, 0, "mm:%2d ss:%2d of:%2d  ", mm, ss, of);
         }
         if (draw_item_blink)
         {
            draw_item_blink = 0;
            show_draw_item_cursor();
         }
         else draw_item_blink = 1;

      } // end of if !mpow find and show draw item

      if ((mouse_b1) && (!mpow))  // put draw item
      {
         // this complicated section is so that you can press the mouse button
         // then move to a spot under the menus and release
         if (draw_item_type != 1)
         {
            while (mouse_b1)  // wait for release
            {
               proc_controllers();
               al_flip_display();
               show_draw_item_cursor();
               process_scrolledge();
               update_editor_background();
           }
           x100 = mouse_x/20 + wx;
           y100 = mouse_y/20 + wy;
         }

         switch (draw_item_type)
         {
            case 1:  // block
            {
               bx1 = x100;
               by1 = y100;
               get_new_box(); // this keeps the mouse
               set_block_range();
               draw_big(1);
            }
            break;
            case 2:  // item
               c = get_empty_item(); // get a place to put it
               if (c == -1)  break;
               // copy
               for (b=0; b<16; b++) // copy from draw item
                  item[c][b] = item[draw_item_num][b];
               item[c][4]= x100*20; // get x
               item[c][5]= y100*20; // get y

               if (item[draw_item_num][0] == 4) // key )
                  if (al_show_native_message_box(display, "Move?", "Move the key's block range also?", NULL, NULL, ALLEGRO_MESSAGEBOX_YES_NO | ALLEGRO_MESSAGEBOX_QUESTION ) == 1)
                  {
                     item[c][6] = item[draw_item_num][6] + x100 - item[draw_item_num][4]/20; // move x
                     item[c][7] = item[draw_item_num][7] + y100 - item[draw_item_num][5]/20;
                     item[c][8] = item[draw_item_num][8] + x100 - item[draw_item_num][4]/20;
                     item[c][9] = item[draw_item_num][9] + y100 - item[draw_item_num][5]/20;
                  }
               if (item[draw_item_num][0] == 10) // msg)
               {
                  free (pmsg[c]);
                  pmsg[c] = (char*) malloc (strlen(pmsg[draw_item_num])+1);
                  strcpy(pmsg[c], pmsg[draw_item_num]);
               }
               item_sort();
               draw_big(1);
            break;
            case 3:    // enemy
            {
               int c = get_empty_item(); // get a place to put it
               if (c == -1)  break;
               for (x=0; x<32; x++) Ei[c][x]  = Ei[draw_item_num][x];
               for (x=0; x<16; x++) Efi[c][x] = Efi[draw_item_num][x];
               Efi[c][0] = al_itofix(x100*20);  // set new x,y
               Efi[c][1] = al_itofix(y100*20);
               al_fixed fx_offset = al_itofix( 20 * (x100 - (al_fixtoi(Efi[draw_item_num][0]) / 20)));
               al_fixed fy_offset = al_itofix( 20 * (y100 - (al_fixtoi(Efi[draw_item_num][1]) / 20)));
               int x_offset = x100 - al_fixtoi(Efi[draw_item_num][0]/20) ;
               int y_offset = y100 - al_fixtoi(Efi[draw_item_num][1]/20) ;

               if (Ei[draw_item_num][0] == 7) // podzilla
               {
                  if (al_show_native_message_box(display, "Move?", "Move podzilla's extended position too?", NULL, NULL, ALLEGRO_MESSAGEBOX_YES_NO | ALLEGRO_MESSAGEBOX_QUESTION ) == 1)
                  {
                      Efi[c][5] = Efi[draw_item_num][5] + fx_offset;
                      Efi[c][6] = Efi[draw_item_num][6] + fy_offset;
                  }
                  if (al_show_native_message_box(display, "Move?", "Move podzilla's trigger box too?", NULL, NULL, ALLEGRO_MESSAGEBOX_YES_NO | ALLEGRO_MESSAGEBOX_QUESTION ) == 1)
                  {
                     Ei[c][11] = Ei[draw_item_num][11] + x_offset;
                     Ei[c][12] = Ei[draw_item_num][12] + y_offset;
                     Ei[c][13] = Ei[draw_item_num][13] + x_offset;
                     Ei[c][14] = Ei[draw_item_num][14] + y_offset;
                  }
                  recalc_pod(c);
               }
               if (Ei[draw_item_num][0] == 9) // cloner
               {
                  if (al_show_native_message_box(display, "Move?", "Move cloner's source and destination boxes too?", NULL, NULL, ALLEGRO_MESSAGEBOX_YES_NO | ALLEGRO_MESSAGEBOX_QUESTION ) == 1)
                  {
                     Ei[c][15] = Ei[draw_item_num][15] + x_offset;
                     Ei[c][16] = Ei[draw_item_num][16] + y_offset;
                     Ei[c][17] = Ei[draw_item_num][17] + x_offset;
                     Ei[c][18] = Ei[draw_item_num][18] + y_offset;
                  }
                  if (al_show_native_message_box(display, "Move?", "Move cloner's trigger box too?", NULL, NULL, ALLEGRO_MESSAGEBOX_YES_NO | ALLEGRO_MESSAGEBOX_QUESTION ) == 1)
                  {
                     Ei[c][11] = Ei[draw_item_num][11] + x_offset;
                     Ei[c][12] = Ei[draw_item_num][12] + y_offset;
                     Ei[c][13] = Ei[draw_item_num][13] + x_offset;
                     Ei[c][14] = Ei[draw_item_num][14] + y_offset;
                  }
               }
               sort_enemy();
               draw_big(1);
            }
            break;
            case 5:    // Special
            if ((PDEi[draw_item_num][0] > 99) && (PDEi[draw_item_num][0] < 200)) // put item
            {
               int d = get_empty_item(); // get a place to put it
               if (d == -1)  break;
               // copy from pde
               for (x=0; x<16; x++) // item
                  item[d][x] = PDEi[draw_item_num][x];
               item[d][0] -= 100;
               item[d][4] = x100*20;
               item[d][5] = y100*20;
               item_sort();
               draw_big(1);
            }
            if (PDEi[draw_item_num][0] < 99) // put enemy
            {
               int d = get_empty_item(); // get a place to put it
               if (d == -1)  break;
               for (x=0; x<32; x++) Ei[d][x]  = PDEi[draw_item_num][x];
               for (x=0; x<16; x++) Efi[d][x] = PDEfx[draw_item_num][x];
               Efi[d][0] = al_itofix(x100*20);  // set new x,y
               Efi[d][1] = al_itofix(y100*20);
               sort_enemy();
               draw_big(1);
            }
            break;
         } // end of switch case
      } // end of put draw item



      if (key[ALLEGRO_KEY_E])
      {
         while (key[ALLEGRO_KEY_E]) proc_controllers();
         show_all_enemies();
      }

      if (key[ALLEGRO_KEY_P])
      {
         while (key[ALLEGRO_KEY_P]) proc_controllers();
         show_all_pmsg();
      }
      if (key[ALLEGRO_KEY_I])
      {
         while (key[ALLEGRO_KEY_I]) proc_controllers();
         show_all_items();
      }

      if (key[ALLEGRO_KEY_S])
      {
         while (key[ALLEGRO_KEY_S]) proc_controllers();
         show_all_lifts();
      }

      if ((mouse_b2) && (!mpow))  // pop up menu
      {
         extern char eitype_desc[50][32][40];
         extern int select_window_active;

         int pop_menu_selection = 999;
         int temp_mouse_x, temp_mouse_y;

         switch (point_item_type)
         {
            case 1:
               sprintf(global_string[9][2], "Copy Block    ");
               sprintf(global_string[9][3], "              ");
               sprintf(global_string[9][4], "                ");
            break;
            case 2:
               sprintf(global_string[9][2], "Copy %s  ", item_desc[ item[point_item_num][0] ]);
               sprintf(global_string[9][3], "View %s  ", item_desc[ item[point_item_num][0] ]);
               sprintf(global_string[9][4], "Delete %s ", item_desc[ item[point_item_num][0] ]);
            break;
            case 3:
               sprintf(global_string[9][2], "Copy %s  ", (const char *)eitype_desc[ Ei[point_item_num][0]]);
               sprintf(global_string[9][3], "View %s  ", (const char *)eitype_desc[ Ei[point_item_num][0]]);
               sprintf(global_string[9][4], "Delete %s ", (const char *)eitype_desc[ Ei[point_item_num][0]]);
            break;
            case 4:
               sprintf(global_string[9][2], "              ");
               sprintf(global_string[9][3], "View Lift '%s'", lifts[point_item_num].lift_name);
               sprintf(global_string[9][4], "Delete Lift '%s'", lifts[point_item_num].lift_name);
            break;
         }

         temp_mouse_x = mouse_x*les;
         temp_mouse_y = mouse_y*les;
         pop_menu_selection = pmenu(9);
         al_set_mouse_xy(display, temp_mouse_x, temp_mouse_y);
         proc_controllers();

         switch (pop_menu_selection)
         {
            case 2:  // copy
               if (point_item_type < 4)
               {
                  draw_item_type = point_item_type;
                  draw_item_num  = point_item_num;
               }
            break;
            case 3:  // view
               if (point_item_type > 1) object_viewer(point_item_type, point_item_num);
            break;
            case 4:  // delete
               x = wx + (mouse_x / 20);
               y = wy + (mouse_y / 20);
               switch (point_item_type)
               {
                  case 1: // delete
                       l[x][y] = 0;
                  break;
                  case 2: // zero item
                     // are you deleting the draw item
                     if ((draw_item_type == 2) && (draw_item_num == point_item_num))
                     {
                        draw_item_type = 1;
                        draw_item_num = 0;
                     }
                     for (a=0; a<16; a++)
                        item [point_item_num][a] = 0;
                     item_sort();
                  break;
                  case 3: // zero enemy
                     // are you deleting the draw item
                     if ((draw_item_type == 3) && (draw_item_num == point_item_num))
                     {
                        draw_item_type = 1;
                        draw_item_num = 0;
                     }
                     for (a=0; a<32; a++) Ei[point_item_num][a] = 0;
                     for (a=0; a<16; a++) Efi[point_item_num][a] = al_itofix(0);
                     sort_enemy();
                  break;
                  case 4: // zero lift
                     erase_lift(point_item_num);
                  break;
               }
               draw_big(1);
            break;
            case 5: break; // menu divider
            case 6:   // zoom full screen
            {
               int draw_item;
               if (draw_item_type == 1) draw_item = draw_item_num;
               else draw_item = 0;
               c = zoom_full_screen(wx, wy, draw_item);
               // for mouse and window pos
               wy = (c / 100);
               c = c - (wy * 100);
               wx = c;
               process_scrolledge(); // for wx and wy bounds check
               draw_big(1);
            }
            break;
            case 7:
               status_window_active = 1;
               check_s_window_pos(1);
            break;
            case 8:
               select_window_active = 1;
               check_s_window_pos(1);
            break;
            case 10: // load
               load();
               set_wx_from_start_block();
               sort_enemy();
               item_sort();
               draw_big(1);
            break;
            case 11: // save
               save();
            break;
            case 12: // save and exit
            {
               extern int exit_link;
               if (save()) em_quit=1;
               exit_link = 1;
            }
            break;
            case 13: // help
               help((char *)"Level Editor Basics");
            break;
            case 14: // exit
               em_quit=1;
            break;
            case 16: predefined_enemies(); break;
            case 17: global_level(); break;
            case 18: level_viewer(); break;
            case 19: animation_proc(); break;

         } // end of switch case
         al_set_mouse_xy(display, temp_mouse_x, temp_mouse_y);

      } // end of pop up menu processing



      if (status_window_active) process_status_window(0);


      if (select_window_active)
      {
         d = process_select_window(0);
         if (d < 999) // block
         {
            draw_item_num = d;
            draw_item_type = 1;
         }
         if (d >= 3000) // pd
         {
            draw_item_num = d-3000;
            draw_item_type = 5;
         }
      }

      al_rest(mouse_loop_pause);

      al_set_target_backbuffer(display);
      al_flip_display();
      al_clear_to_color(al_map_rgb(0,0,0));

      proc_controllers();

      process_scrolledge();
      update_editor_background();

      if (Redraw)
      {
         Redraw = 0;
         draw_big(1);
      }

      if (key[ALLEGRO_KEY_ESCAPE])  // trap here 1st
      {
         while (key[ALLEGRO_KEY_ESCAPE]) proc_controllers();
         if (al_show_native_message_box(display, "Save?", "Save before exit?", NULL, NULL, ALLEGRO_MESSAGEBOX_OK_CANCEL) == 1) save_level(level_num);
         em_quit = 1;
      }
   } while (!em_quit);
   menu_setup(); // restore for game
   level_editor_running = 0;

   if (original_les != les)
   {
      les = original_les;
      //set_ortho();
      rebuild_bitmaps();
   }

   al_hide_mouse_cursor(display);
   extern int level_num;
   return level_num;
}   // end of editor menu




























































