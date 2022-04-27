// e_editor_main.cpp

#include "pm.h"

// draws item on dtemp, with rotation if required
void get_item_draw_shape(int i)
{
   al_set_target_bitmap(dtemp);
   al_clear_to_color(al_map_rgb(0,0,0));
   int drawn = 0;
   int shape = item[i][1];                       // get shape
   if (shape > 999) shape = zz[0][shape-1000];   // ans

   // doors needs to get special colorized shapes from door_tile[][]
   if (item[i][0] == 1)
   {
      int col = item[i][6];
      if (item[i][13] == 448) // old style door shape
      {
         int shape = item[i][1];       // get shape
         int si = shape-448;           // convert to index to bitmap sequence
         al_draw_bitmap(door_tile[1][col][si], 0, 0, 0);
      }
      else // new style doors
      {
         int an = zz[1][83];             // cheat and use shape index from base door animation sequence
         if (item[i][8] == 0) an = 7-an; // exit only, run the sequence backwards
         al_draw_bitmap(door_tile[0][col][an], 0, 0, 0);
      }
      drawn = 1;
   }


   if (item[i][0] == 9)  shape = 991; // these don't normally show on the level when the game is running
   if (item[i][0] == 16) shape = 989;
   if (item[i][0] == 17) shape = 988;




   // these types need rotation
   if (item[i][0] == 11) // rocket
   {
      //rotate_sprite(dtemp, tile[shape], 0, 0, itofix(item[i][10]/10));
      al_draw_bitmap(tile[shape], 0, 0, 0);
      drawn = 1;
   }

   // every other item draws like this
   if (!drawn) al_draw_bitmap(tile[shape], 0, 0, 0);
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
   int a = PDEi[pde][1]; // bmp or ans
   if (a > 999) a = zz[5][a-1000]; // ans
   al_set_target_backbuffer(display);
   al_draw_bitmap(tile[a], x, y, 0);
}

void show_draw_item_cursor(void)
{
   int x100 = mouse_x/20 + wx;
   int y100 = mouse_y/20 + wy;
   if (x100>99) x100 = 99;
   if (y100>99) y100 = 99;
   int x = (x100-wx)*20;
   int y = (y100-wy)*20;
   // determine if shapes are the same
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
   if (same) al_draw_bitmap(tile[255], x, y, 0);
   else switch (draw_item_type)
   {
      case 1: al_draw_bitmap(btile[draw_item_num & 1023], x, y, 0); break;
      case 2: draw_item_shape(draw_item_num, x, y); break;
      case 3: draw_enemy_shape(draw_item_num, x, y); break;
      case 5: draw_PDE_shape(draw_item_num, x, y);  break;
   }
}

void set_block_range(void)
{
   int draw_item_flags = draw_item_num & PM_BTILE_MOST_FLAGS;

   int fsd[20][20] = {0};

   // purple pipe with open center
   fsd[0][0] = 576; // trigger blocks start
   fsd[0][1] = 592; // trigger block end
   fsd[0][9] = -1;  // default shape
   fsd[0][10] = 576; // upper left corner
   fsd[0][11] = 577; // upper right corner
   fsd[0][12] = 578; // lower left corner
   fsd[0][13] = 579; // lower right corner
   fsd[0][14] = 580; // left vertical through
   fsd[0][15] = 581; // right vertical through
   fsd[0][16] = 582; // upper horizontal through
   fsd[0][17] = 583; // lower horizontal through

   // purple pipe with solid center
   fsd[1][0] = 512; // trigger blocks start
   fsd[1][1] = 528; // trigger block end
   fsd[1][9] = 528;  // default shape
   fsd[1][10] = 512; // upper left corner
   fsd[1][11] = 513; // upper right corner
   fsd[1][12] = 514; // lower left corner
   fsd[1][13] = 515; // lower right corner
   fsd[1][14] = 516; // left vertical through
   fsd[1][15] = 517; // right vertical through
   fsd[1][16] = 518; // upper horizontal through
   fsd[1][17] = 519; // lower horizontal through

   // wires
   fsd[2][0] = 605; // trigger blocks start
   fsd[2][1] = 624; // trigger block end
   fsd[2][9] = -1;  // default shape
   fsd[2][10] = 608; // upper left corner
   fsd[2][11] = 609; // upper right corner
   fsd[2][12] = 610; // lower left corner
   fsd[2][13] = 611; // lower right corner
   fsd[2][14] = 612; // left vertical through
   fsd[2][15] = 612; // right vertical through
   fsd[2][16] = 614; // upper horizontal through
   fsd[2][17] = 614; // lower horizontal through

   // brick with corners
   fsd[3][0] = 640; // trigger blocks start
   fsd[3][1] = 656; // trigger block end
   fsd[3][9] = 656; // default shape
   fsd[3][10] = 640; // upper left corner
   fsd[3][11] = 641; // upper right corner
   fsd[3][12] = 642; // lower left corner
   fsd[3][13] = 643; // lower right corner
   fsd[3][14] = 644; // left vertical through
   fsd[3][15] = 645; // right vertical through
   fsd[3][16] = 646; // upper horizontal through
   fsd[3][17] = 647; // lower horizontal through

   // rainbows
   fsd[4][0] = 672; // trigger blocks start
   fsd[4][1] = 679; // trigger block end
   fsd[4][9] = -1;  // default shape
   fsd[4][10] = 672; // upper left corner
   fsd[4][11] = 673; // upper right corner
   fsd[4][12] = 674; // lower left corner
   fsd[4][13] = 675; // lower right corner
   fsd[4][14] = 676; // left vertical through
   fsd[4][15] = 677; // right vertical through
   fsd[4][16] = 678; // upper horizontal through
   fsd[4][17] = 679; // lower horizontal through

   // rainbows 2
   fsd[5][0] = 704; // trigger blocks start
   fsd[5][1] = 711; // trigger block end
   fsd[5][9] = -1;  // default shape
   fsd[5][10] = 704; // upper left corner
   fsd[5][11] = 705; // upper right corner
   fsd[5][12] = 706; // lower left corner
   fsd[5][13] = 707; // lower right corner
   fsd[5][14] = 708; // left vertical through
   fsd[5][15] = 709; // right vertical through
   fsd[5][16] = 710; // upper horizontal through
   fsd[5][17] = 711; // lower horizontal through

   for (int i=0; i<20; i++)
      for (int j=0; j<20; j++)
         fsd[i][j] |= draw_item_flags;


   int fsx[20][5] = {0};

   // purple pipes
   fsx[0][0] = 576; // trigger blocks start
   fsx[0][1] = 592; // trigger block end
   fsx[0][2] = 582; // middle
   fsx[0][3] = 590; // left end
   fsx[0][4] = 588; // right end

   // wires
   fsx[1][0] = 608; // trigger blocks start
   fsx[1][1] = 625; // trigger block end
   fsx[1][2] = 614; // middle
   fsx[1][3] = 622; // left end
   fsx[1][4] = 620; // right end

   // lined platform
   fsx[2][0] = 161; // trigger blocks start
   fsx[2][1] = 163; // trigger block end
   fsx[2][2] = 162; // middle
   fsx[2][3] = 161; // left end
   fsx[2][4] = 163; // right end

   // brown bricks
   fsx[3][0] = 177; // trigger blocks start
   fsx[3][1] = 179; // trigger block end
   fsx[3][2] = 178; // middle
   fsx[3][3] = 177; // left end
   fsx[3][4] = 179; // right end

   // semi-solid screen
   fsx[4][0] = 33; // trigger blocks start
   fsx[4][1] = 39; // trigger block end
   fsx[4][2] = 37; // middle
   fsx[4][3] = 36; // left end
   fsx[4][4] = 38; // right end

   for (int i=0; i<20; i++)
      for (int j=0; j<5; j++)
         fsx[i][j] |= draw_item_flags;


   int fsy[20][5] = {0};

   // purple pipes
   fsy[0][0] = 576; // trigger blocks start
   fsy[0][1] = 592; // trigger block end
   fsy[0][2] = 580; // middle
   fsy[0][3] = 591; // upper end
   fsy[0][4] = 589; // lower end

   // wires
   fsy[1][0] = 608; // trigger blocks start
   fsy[1][1] = 625; // trigger block end
   fsy[1][2] = 612; // middle
   fsy[1][3] = 623; // upper end
   fsy[1][4] = 621; // lower end

   // semi-solid screen
   fsy[4][0] = 33; // trigger blocks start
   fsy[4][1] = 39; // trigger block end
   fsy[4][2] = 34; // middle
   fsy[4][3] = 33; // upper end
   fsy[4][4] = 35; // lower end

   for (int i=0; i<20; i++)
      for (int j=0; j<5; j++)
         fsy[i][j] |= draw_item_flags;


   if ((bx2-bx1==1) && (by2-by1==1)) l[bx1][by1] = draw_item_num; // single block 1 x 1

   if ((bx2-bx1==1) && (by2-by1>1)) // vertical line 1 x >1
   {
      int a = bx1;
      for (int b=by1; b<by2; b++) // cycle the range
      {
         l[a][b] = draw_item_num; // set draw item as default
         for (int x=0; x<20; x++)
            if (fsy[x][0]&1023)
            {
               if (((draw_item_num&1023) >= (fsy[x][0]&1023)) && ((draw_item_num&1023) <= (fsy[x][1]&1023)))
               {
                  l[a][b] = fsy[x][2]; // default
                  if (b == by1)   l[a][b] = fsy[x][3]; // left end cap
                  if (b == by2-1) l[a][b] = fsy[x][4]; // right end cap
               }
            }
      }
   }
   if ((bx2-bx1>1) && (by2-by1==1)) // horizontal line >1 x 1
   {
      int b = by1;
      for (int a=bx1; a<bx2; a++) // cycle the range
      {
         l[a][b] = draw_item_num; // set draw item as default
         for (int x=0; x<20; x++)
         {
            if (fsx[x][0]&1023)
            {
               if (((draw_item_num&1023) >= (fsx[x][0]&1023)) && ((draw_item_num&1023) <= (fsx[x][1]&1023)))
               {
                  l[a][b] = fsx[x][2]; // default
                  if (a == bx1)   l[a][b] = fsx[x][3]; // left end cap
                  if (a == bx2-1) l[a][b] = fsx[x][4]; // right end cap
               }
            }
         }
      }
   }
   if ((bx2-bx1>1) && (by2-by1>1)) // box shape with corners >1 x >1
   {
      int special_handler = 0;
      for (int a=bx1; a<bx2; a++)       // cycle the range
         for (int b=by1; b<by2; b++)
            for (int x=0; x<20; x++)
               if (fsd[x][0]&1023)
               {
                  if (((draw_item_num&1023) >= (fsd[x][0]&1023)) && ((draw_item_num&1023) <= (fsd[x][1]&1023)))
                  {
                     special_handler = 1;

                     if (fsd[x][9] != -1) l[a][b] = fsd[x][9]; // default block

                     if (b == by1  ) l[a][b] = fsd[x][16];         // upper horizontal through
                     if (b == by2-1) l[a][b] = fsd[x][17];         // lower horizontal through


                     if (a == bx1)
                     {
                        if (b == by1) l[a][b] = fsd[x][10];        // upper-right corner
                        else if (b == by2-1) l[a][b] = fsd[x][12]; // lower-right corner
                        else l[a][b] = fsd[x][14];                 // right vertical through
                     }
                     if (a == bx2-1)
                     {
                        if (b == by1) l[a][b] = fsd[x][11];        // upper-left corner
                        else if (b == by2-1) l[a][b] = fsd[x][13]; // lower-left corner
                        else l[a][b] = fsd[x][15];                 // left vertical through
                     }
                  }
                  if (!special_handler) l[a][b] = draw_item_num;

             } // end of cycle block range
   } // end of box shape with corners*/
}


void get_new_box(void) // keep the mouse !!
{
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
   al_set_target_backbuffer(display);
   al_draw_bitmap_region(level_background, wx*20, wy*20, (SCREEN_W/20)*20, (SCREEN_H/20)*20, 0, 0, 0);

   int lesw = (SCREEN_W/20)*20-1;
   int lesh = (SCREEN_H/20)*20-1;
   if (lesw > 1999) lesw = 1999;
   if (lesh > 1999) lesh = 1999;
   al_draw_rectangle(0.5, 0.5, lesw, lesh, palette_color[9], 1);
}

int process_scrolledge(void)
{
   int scrolledge=10;
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


char* get_text_description_of_block_based_on_flags(int flags)
{
   sprintf(msg, "Empty");  // default

   if (flags & PM_BTILE_SOLID_PLAYER)     sprintf(msg, "Solid");
   if (flags & PM_BTILE_SEMISOLID_PLAYER) sprintf(msg, "Semi-Solid");
   if (flags & PM_BTILE_BREAKABLE_PBUL)   sprintf(msg, "Breakable");
   if (flags & PM_BTILE_BOMBABLE)         sprintf(msg, "Bombable");
   if (flags & PM_BTILE_LADDER_MOVE)      sprintf(msg, "Ladder");
   if (flags & PM_BTILE_ROPE_MOVE)        sprintf(msg, "Rope");
   return msg;
}

void draw_item_info(int x, int y, int color, int type, int num)
{
   int a, b;
   switch (type)
   {
      case 1:
         al_draw_bitmap(btile[num&1023], x, y, 0);
         al_draw_textf(font, palette_color[color], x+22, y+2, 0, "Block #%d",num&1023);
         al_draw_textf(font, palette_color[color], x+22, y+12, 0, "%s", get_text_description_of_block_based_on_flags(num) );
      break;
      case 2:
         draw_item_shape(num, x, y);
         a = item[num][0]; // type
         al_draw_textf(font, palette_color[color], x+22, y+2, 0, "%s", item_name[a]);
         al_draw_textf(font, palette_color[color], x+22, y+12, 0, "%d of %d", 1+num - item_first_num[a],item_num_of_type[a]);
      break;
      case 3:
         draw_enemy_shape(num, x, y);
         a = Ei[num][0]; // type
         al_draw_textf(font, palette_color[color], x+22, y+2, 0, "%s", enemy_name[a]);
         al_draw_textf(font, palette_color[color], x+22, y+12, 0, "%d of %d", 1+num - e_first_num[a],e_num_of_type[a]);
      break;
      case 4:
      {

         int col = lifts[num].color;
         int width = lifts[num].width;
         if (width > 140) width = 140;
         for (a=0; a<10; a++)
            al_draw_rectangle(x+a, y+a, x+(width)-1-a, y+19-a, palette_color[col+((9-a)*16)], 1 );

//         if ((lifts[num].width == 1) && (lifts[num].height > 1)) // rotate lift name for vertical lifts
  //          rtextout_centre(NULL, lifts[num].lift_name, x+(width*10), y, color+160, 1, 64, 1 );
    //     else
            al_draw_text(font, palette_color[col+160], x+(width/2), y+6, ALLEGRO_ALIGN_CENTER, lifts[num].lift_name);
      }
      break;
      case 5:
         a = PDEi[num][1]; // bmp or ans
         if (a < NUM_SPRITES) b = a; // bmp
         if (a > 999) b = zz[5][a-1000]; // ans
         al_draw_bitmap(tile[b], x, y, 0);
         a = Ei[num][0]; // type
         al_draw_text(font, palette_color[color], x+22, y+2, 0, "Special Item");
         al_draw_textf(font, palette_color[color], x+22, y+12, 0, "%s", PDEt[num][1]);
      break;
   }
}

int edit_menu(int el)
{
   level_editor_running = 1;
   int original_display_transform_double = display_transform_double;

//   int target_display_transform_double = 1; // this is the orig

   int target_display_transform_double = original_display_transform_double; // this is the hacked line

   if (disp_w_curr > 3800) target_display_transform_double = 2;
   if (display_transform_double != target_display_transform_double)
   {
      display_transform_double = target_display_transform_double;
      set_display_transform();
      set_map_var();
   }

   al_show_mouse_cursor(display);
   resume_allowed = 0;
   int mpow;
   int em_quit = 0;
   int x100, y100, x2000, y2000;
   int a, b, c, d, e, x, y;

   // clamp screen size to 2000, 2000 if bigger
   int lesw = (SCREEN_W/20)*20-1;
   int lesh = (SCREEN_H/20)*20-1;
   if (lesw > 1999) lesw = 1999;
   if (lesh > 1999) lesh = 1999;

   check_s_window_pos(0);
   load_PDE();

   if (!el) load_level_prompt(); // load prompt
   else load_level(el, 0); // blind load

   draw_big(1);

   sort_enemy();
   sort_item();

   set_swbl();

   draw_item_type = 1;
   draw_item_num  = 0;


   set_frame_nums(0);

   set_wx_from_start_block();

   al_set_mouse_xy(display, SCREEN_W/2, SCREEN_H/2);
   proc_controllers();

   // clear_key array
   for (int k = ALLEGRO_KEY_A; k < ALLEGRO_KEY_MAX; k++) key[k] = 0;

   int draw_item_blink = 0;

   if (autoload_bookmark)
   {
      printf("load bookmark\n");
      if (bookmark_level == last_level_loaded)
      {
         if ((bookmark_obj == 2) && (item[bookmark_num]))       object_viewer(2, bookmark_num);
         if ((bookmark_obj == 3) && (Ei[bookmark_num]))         object_viewer(3, bookmark_num);
         if ((bookmark_obj == 4) && (bookmark_num < num_lifts)) object_viewer(4, bookmark_num);
      }
   }



   do
   {
      set_display_transform();

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


      // detect if mouse is on draw item flags
      if ((draw_item_type == 1) && (status_window_active))
      {
         int ftx = status_window_x+11;
         int fty = status_window_y+47;
         int ys = 10; // y spacing

         int frw = 6;         // flag rectangle width
         int frh = 6;         // flag rectangle height
         int frx = ftx-frw-2;        // flag rectangle x
         int fry = fty - (frh/2)+4;  // flag rectangle y

         int frx2 = frx+frw;
         int fry2 = fty+frh+(ys*14);

         frx  -=2;
         frx2 +=2;
         fry  -=2;
         fry2 +=4;

         if ((mouse_x > frx) && (mouse_x < frx2))
            if ((mouse_y > fry) && (fry2))
            {
               mpow = 1;
               al_draw_rectangle(frx, fry, frx2, fry2, palette_color[14], 1);
            }
      }






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
            {
               c = get_empty_item(); // get a place to put it
               int din = draw_item_num;
               int di = item[din][0];
               if (c == -1)  break;
               // copy
               for (b=0; b<16; b++) // copy from draw item
                  item[c][b] = item[din][b];
               item[c][4]= x100*20; // get x
               item[c][5]= y100*20; // get y

               if (di == 4) // key
                  if (al_show_native_message_box(display, "Move?", "Move the key's block range also?", NULL, NULL, ALLEGRO_MESSAGEBOX_YES_NO | ALLEGRO_MESSAGEBOX_QUESTION ) == 1)
                  {
                     item[c][6] = item[din][6] + x100 - item[din][4]/20; // move x
                     item[c][7] = item[din][7] + y100 - item[din][5]/20;
                     item[c][8] = item[din][8] + x100 - item[din][4]/20;
                     item[c][9] = item[din][9] + y100 - item[din][5]/20;
                  }

               if ((di == 9) || (di == 16) || (di == 17)) // trigger, manip, damage
               {
                  item[c][6] = item[din][6] + (x100*20) - item[din][4]; // move field relative to item move
                  item[c][7] = item[din][7] + (y100*20) - item[din][5];
               }
               if (item[din][0] == 10) strcpy(pmsgtext[c], pmsgtext[din]); // msg

               sort_item();
               draw_big(1);
            }
            break;
            case 3:    // enemy
            {
               int c = get_empty_enemy(); // get a place to put it
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

               if (Ei[draw_item_num][0] == 10) // field
               {
                  // (al_show_native_message_box(display, "Move?", "Move field's damage and trigger fields too?", NULL, NULL, ALLEGRO_MESSAGEBOX_YES_NO | ALLEGRO_MESSAGEBOX_QUESTION ) == 1)
                  {
                     Ei[c][11] = Ei[draw_item_num][11] + x_offset*20;
                     Ei[c][12] = Ei[draw_item_num][12] + y_offset*20;
                     Ei[c][15] = Ei[draw_item_num][15] + x_offset*20;
                     Ei[c][16] = Ei[draw_item_num][16] + y_offset*20;
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
               sort_item();
               draw_big(1);
            }
            if (PDEi[draw_item_num][0] < 99) // put enemy
            {
               int d = get_empty_enemy(); // get a place to put it
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




      if (key[ALLEGRO_KEY_B])
      {
         while (key[ALLEGRO_KEY_B]) proc_controllers();
         printf("load bookmark\n");
         if (bookmark_level == last_level_loaded)
         {
            if ((bookmark_obj == 2) && (item[bookmark_num]))       object_viewer(2, bookmark_num);
            if ((bookmark_obj == 3) && (Ei[bookmark_num]))         object_viewer(3, bookmark_num);
            if ((bookmark_obj == 4) && (bookmark_num < num_lifts)) object_viewer(4, bookmark_num);
         }
      }




      if (key[ALLEGRO_KEY_L])
      {
         while (key[ALLEGRO_KEY_L]) proc_controllers();
         show_all_lifts();
      }
      if (key[ALLEGRO_KEY_E])
      {
         while (key[ALLEGRO_KEY_E]) proc_controllers();
         show_all_enemies();
      }
      if (key[ALLEGRO_KEY_S])
      {
         while (key[ALLEGRO_KEY_S]) proc_controllers();
         level_check();
      }
      if (key[ALLEGRO_KEY_I])
      {
         while (key[ALLEGRO_KEY_I]) proc_controllers();
         show_all_items();
      }
      if (key[ALLEGRO_KEY_P])
      {
         while (key[ALLEGRO_KEY_P]) proc_controllers();
         show_all_pmsg();
      }

      if ((mouse_b2) && (!mpow))  // pop up menu
      {
         int pop_menu_selection = 999;
         int temp_mouse_x, temp_mouse_y;
         switch (point_item_type)
         {
            case 1:
               sprintf(global_string[2][2], "Copy Block    ");
               sprintf(global_string[2][3], "              ");
               sprintf(global_string[2][4], "                ");
            break;
            case 2:
               sprintf(global_string[2][2], "Copy %s  ",  item_name[item[point_item_num][0]]);
               sprintf(global_string[2][3], "View %s  ",  item_name[item[point_item_num][0]]);
               sprintf(global_string[2][4], "Delete %s ", item_name[item[point_item_num][0]]);
            break;
            case 3:
               sprintf(global_string[2][2], "Copy %s  ",  (const char *)enemy_name[Ei[point_item_num][0]]);
               sprintf(global_string[2][3], "View %s  ",  (const char *)enemy_name[Ei[point_item_num][0]]);
               sprintf(global_string[2][4], "Delete %s ", (const char *)enemy_name[Ei[point_item_num][0]]);
            break;
            case 4:
               sprintf(global_string[2][2], "              ");
               sprintf(global_string[2][3], "View Lift '%s'",   lifts[point_item_num].lift_name);
               sprintf(global_string[2][4], "Delete Lift '%s'", lifts[point_item_num].lift_name);
            break;
         }

         temp_mouse_x = mouse_x*display_transform_double;
         temp_mouse_y = mouse_y*display_transform_double;

         pop_menu_selection = pmenu(2, 0);
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
        //       if (point_item_type > 1) object_viewer(point_item_type, point_item_num);
               if (point_item_type > 1) object_viewerw(0, 0, point_item_type, point_item_num);


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
                     sort_item();
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
               group_edit();
            break;
            case 8:
               status_window_active = 1;
               check_s_window_pos(0);
            break;
            case 9:
               select_window_active = 1;
               check_s_window_pos(0);
            break;

            case 11: // new level
            if (al_show_native_message_box(display, "New Level", "Clicking OK will create a new blank level", NULL, NULL, ALLEGRO_MESSAGEBOX_OK_CANCEL) == 1)
            {
               zero_level_data();
               draw_big(1);
               update_editor_background();
               al_flip_display();
               save_level_prompt();
            }
            load_level(last_level_loaded, 0); // blind load
            break;

            case 12: // load level
               load_level_prompt();
               set_wx_from_start_block();
               sort_enemy();
               sort_item();
               draw_big(1);
            break;
            case 13: // save level
               save_level_prompt();
            break;
            case 14: // save and exit
               if (save_level_prompt()) em_quit=1;
            break;
            case 15: // help
               help("Level Editor Basics");
            break;
            case 16: // exit
               em_quit=1;
            break;
            case 18: predefined_enemies(); break;
            case 19: global_level(); break;
            case 20: level_viewer(); break;
            case 21: animation_sequence_editor(); break;
            case 22: copy_tiles(); break;
            case 23: edit_btile_attributes(); break;

         } // end of switch case
         al_set_mouse_xy(display, temp_mouse_x, temp_mouse_y);

      } // end of pop up menu processing



      if (status_window_active) process_status_window(0);


      if (select_window_active)
      {
         d = process_select_window(0);
         if (d >= 0) // block
         {
            draw_item_num = d;
            draw_item_type = 1;
         }
         if (d < -1000) // pde copy type
         {
            //printf("pde %d  %d\n", d, d+2000);
            draw_item_num = d+2000;
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
         if (al_show_native_message_box(display, "Save?", "Save before exit?", NULL, NULL, ALLEGRO_MESSAGEBOX_OK_CANCEL) == 1) save_level(last_level_loaded);
         em_quit = 1;
      }
   } while (!em_quit);
   level_editor_running = 0;
   if (original_display_transform_double != display_transform_double)
   {
      display_transform_double = original_display_transform_double;
      set_display_transform();
      set_map_var();
   }
   al_hide_mouse_cursor(display);
   return last_level_loaded;
}   // end of editor menu




























































