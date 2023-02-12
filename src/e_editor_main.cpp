// e_editor_main.cpp
#include "pm.h"
#include "mwWindow.h"
#include "mwFont.h"
#include "mwBitmap.h"


#ifndef CLASS_MWWINDOWS_DEFINED
#include "mwWindowManager.h"
#endif

void em_set_swbl(void)
{
   mwWM.mW[2].swbn = 0;
   for (int c=0; c<NUM_SPRITES; c++)
   {
      mwWM.swbl[c][0] = mwWM.swbl[c][1] = 0;                    // erase
      if (sa[c][0] & PM_BTILE_SHOW_SELECT_WIN)
      {
         if ((c == 384) || (c == 416) || (c == 448) || (c == 480) || (c == 512) || (c == 576) || (c == 608) || (c == 640)|| (c == 672)|| (c == 704)) // start new line
         {
            int off = (16 - (mwWM.mW[2].swbn % 16));
            if (off < 16) mwWM.mW[2].swbn += off;
         }

         mwWM.swbl[mwWM.mW[2].swbn][0] = c | sa[c][0];                // add to list with default flags
         mwWM.swbl[mwWM.mW[2].swbn][0] &= ~PM_BTILE_SHOW_SELECT_WIN;  // clear flag
         mwWM.mW[2].swbn++;
      }
   }
   mwWM.mW[2].swnbl = (mwWM.mW[2].swbn / 16) + 1;
   if (mwWM.mW[2].swnbl_cur == 0) mwWM.mW[2].swnbl_cur = mwWM.mW[2].swnbl; // initial only
}



void em_set_block_range(void)
{
   int draw_item_flags = mwWM.mW[1].draw_item_num & PM_BTILE_MOST_FLAGS;


   int b = 0, f = 0;


   // -------------------------------------------
   // rectangles (h>1 && w>1)
   // -------------------------------------------
   int fsd[20][20] = {0};

   // white brain with blue outline
   b = 384; f=0;
   fsd[f][0] = b;    // trigger blocks start
   fsd[f][1] = b+18; // trigger block end
   fsd[f][9] = b+16;   // default shape
   fsd[f][10] = b+0; // upper left corner
   fsd[f][11] = b+1; // upper right corner
   fsd[f][12] = b+2; // lower left corner
   fsd[f][13] = b+3; // lower right corner
   fsd[f][14] = b+4; // left vertical through
   fsd[f][15] = b+5; // right vertical through
   fsd[f][16] = b+6; // upper horizontal through
   fsd[f][17] = b+7; // lower horizontal through

   // brown and yellow thatch
   b = 416; f++;
   fsd[f][0] = b;    // trigger blocks start
   fsd[f][1] = b+18; // trigger block end
   fsd[f][9] = b+16;   // default shape
   fsd[f][10] = b+0; // upper left corner
   fsd[f][11] = b+1; // upper right corner
   fsd[f][12] = b+2; // lower left corner
   fsd[f][13] = b+3; // lower right corner
   fsd[f][14] = b+4; // left vertical through
   fsd[f][15] = b+5; // right vertical through
   fsd[f][16] = b+6; // upper horizontal through
   fsd[f][17] = b+7; // lower horizontal through

   // brown brick with corners
   b = 448; f++;
   fsd[f][0] = b;    // trigger blocks start
   fsd[f][1] = b+18; // trigger block end
   fsd[f][9] = b+16;   // default shape
   fsd[f][10] = b+0; // upper left corner
   fsd[f][11] = b+1; // upper right corner
   fsd[f][12] = b+2; // lower left corner
   fsd[f][13] = b+3; // lower right corner
   fsd[f][14] = b+4; // left vertical through
   fsd[f][15] = b+5; // right vertical through
   fsd[f][16] = b+6; // upper horizontal through
   fsd[f][17] = b+7; // lower horizontal through

   // grey brick with corners
   b = 480; f++;
   fsd[f][0] = b;    // trigger blocks start
   fsd[f][1] = b+18; // trigger block end
   fsd[f][9] = b+16;   // default shape
   fsd[f][10] = b+0; // upper left corner
   fsd[f][11] = b+1; // upper right corner
   fsd[f][12] = b+2; // lower left corner
   fsd[f][13] = b+3; // lower right corner
   fsd[f][14] = b+4; // left vertical through
   fsd[f][15] = b+5; // right vertical through
   fsd[f][16] = b+6; // upper horizontal through
   fsd[f][17] = b+7; // lower horizontal through

//   // purple pipe with solid center
//   b = 512; f++;
//   fsd[f][0] = b;    // trigger blocks start
//   fsd[f][1] = b+29; // trigger block end
//   fsd[f][9] = b+16; // default shape
//   fsd[f][10] = b+0; // upper left corner
//   fsd[f][11] = b+1; // upper right corner
//   fsd[f][12] = b+2; // lower left corner
//   fsd[f][13] = b+3; // lower right corner
//   fsd[f][14] = b+4; // left vertical through
//   fsd[f][15] = b+5; // right vertical through
//   fsd[f][16] = b+6; // upper horizontal through
//   fsd[f][17] = b+7; // lower horizontal through

   // purple pipe with solid center
   b = 512; f++;
   fsd[f][0] = b;    // trigger blocks start
   fsd[f][1] = b+7;  // trigger block end
   fsd[f][9] = b+16; // default shape
   fsd[f][10] = b+0; // upper left corner
   fsd[f][11] = b+1; // upper right corner
   fsd[f][12] = b+2; // lower left corner
   fsd[f][13] = b+3; // lower right corner
   fsd[f][14] = b+4; // left vertical through
   fsd[f][15] = b+5; // right vertical through
   fsd[f][16] = b+6; // upper horizontal through
   fsd[f][17] = b+7; // lower horizontal through

   // purple pipe with solid center rev
   b = 520; f++;
   fsd[f][0] = b;    // trigger blocks start
   fsd[f][1] = b+20; // trigger block end
   fsd[f][9] = 0;    // default shape
   fsd[f][10] = b+7; // upper left corner
   fsd[f][11] = b+6; // upper right corner
   fsd[f][12] = b+5; // lower left corner
   fsd[f][13] = b+4; // lower right corner
   fsd[f][14] = b-3; // left vertical through
   fsd[f][15] = b-4; // right vertical through
   fsd[f][16] = b-1; // upper horizontal through
   fsd[f][17] = b-2; // lower horizontal through





   // purple pipe with open center
   b = 576; f++;
   fsd[f][0] = b;    // trigger blocks start
   fsd[f][1] = b+16; // trigger block end
   fsd[f][9] = -1;   // default shape
   fsd[f][10] = b+0; // upper left corner
   fsd[f][11] = b+1; // upper right corner
   fsd[f][12] = b+2; // lower left corner
   fsd[f][13] = b+3; // lower right corner
   fsd[f][14] = b+4; // left vertical through
   fsd[f][15] = b+5; // right vertical through
   fsd[f][16] = b+6; // upper horizontal through
   fsd[f][17] = b+7; // lower horizontal through

   // wires
   b = 608; f++;
   fsd[f][0] = b;    // trigger blocks start
   fsd[f][1] = b+18; // trigger block end
   fsd[f][9] = -1;   // default shape
   fsd[f][10] = b+0; // upper left corner
   fsd[f][11] = b+1; // upper right corner
   fsd[f][12] = b+2; // lower left corner
   fsd[f][13] = b+3; // lower right corner
   fsd[f][14] = b+4; // left vertical through
   fsd[f][15] = b+5; // right vertical through
   fsd[f][16] = b+6; // upper horizontal through
   fsd[f][17] = b+7; // lower horizontal through


   // rainbows
   b = 672; f++;
   fsd[f][0] = b;    // trigger blocks start
   fsd[f][1] = b+7;  // trigger block end
   fsd[f][9] = -1;   // default shape
   fsd[f][10] = b+0; // upper left corner
   fsd[f][11] = b+1; // upper right corner
   fsd[f][12] = b+2; // lower left corner
   fsd[f][13] = b+3; // lower right corner
   fsd[f][14] = b+4; // left vertical through
   fsd[f][15] = b+5; // right vertical through
   fsd[f][16] = b+6; // upper horizontal through
   fsd[f][17] = b+7; // lower horizontal through

   // rainbows 2
   b = 704; f++;
   fsd[f][0] = b;    // trigger blocks start
   fsd[f][1] = b+7;  // trigger block end
   fsd[f][9] = -1;   // default shape
   fsd[f][10] = b+0; // upper left corner
   fsd[f][11] = b+1; // upper right corner
   fsd[f][12] = b+2; // lower left corner
   fsd[f][13] = b+3; // lower right corner
   fsd[f][14] = b+4; // left vertical through
   fsd[f][15] = b+5; // right vertical through
   fsd[f][16] = b+6; // upper horizontal through
   fsd[f][17] = b+7; // lower horizontal through

   for (int i=0; i<20; i++)
      for (int j=0; j<20; j++)
         fsd[i][j] |= draw_item_flags;

   // -------------------------------------------
   // horizontal lines
   // -------------------------------------------
   int fsx[20][5] = {0};

   // semi-solid screen
   b=33; f=0;
   fsx[f][0] = b; // trigger blocks start
   fsx[f][1] = b+6; // trigger block end
   fsx[f][2] = b+4; // middle
   fsx[f][3] = b+3; // left end
   fsx[f][4] = b+5; // right end

   // lined platform
   b=161; f++;
   fsx[f][0] = b;    // trigger blocks start
   fsx[f][1] = b+2;  // trigger block end
   fsx[f][2] = b+1;  // middle
   fsx[f][3] = b+0;  // left end
   fsx[f][4] = b+2;  // right end

   // brown bricks
   b=177; f++;
   fsx[f][0] = b;    // trigger blocks start
   fsx[f][1] = b+2;  // trigger block end
   fsx[f][2] = b+1;  // middle
   fsx[f][3] = b+0;  // left end
   fsx[f][4] = b+2;  // right end

   // white brain with blue outline
   b = 384; f++;
   fsx[f][0] = b;    // trigger blocks start
   fsx[f][1] = b+16; // trigger block end
   fsx[f][2] = b+9 ; // middle
   fsx[f][3] = b+14; // left end
   fsx[f][4] = b+12; // right end

   // brown and yellow thatch
   b = 416; f++;
   fsx[f][0] = b;    // trigger blocks start
   fsx[f][1] = b+16; // trigger block end
   fsx[f][2] = b+9 ; // middle
   fsx[f][3] = b+14; // left end
   fsx[f][4] = b+12; // right end

   // brown brick with corners
   b = 448; f++;
   fsx[f][0] = b;    // trigger blocks start
   fsx[f][1] = b+16; // trigger block end
   fsx[f][2] = b+9 ; // middle
   fsx[f][3] = b+14; // left end
   fsx[f][4] = b+12; // right end

   // grey bricks
   b = 480; f++;
   fsx[f][0] = b;    // trigger blocks start
   fsx[f][1] = b+16; // trigger block end
   fsx[f][2] = b+9 ; // middle
   fsx[f][3] = b+14; // left end
   fsx[f][4] = b+12; // right end

   // purple pipes
   b = 576; f++;
   fsx[f][0] = b;    // trigger blocks start
   fsx[f][1] = b+16; // trigger block end
   fsx[f][2] = b+6;  // middle
   fsx[f][3] = b+14; // left end
   fsx[f][4] = b+12; // right end

   // wires
   b=608; f++;
   fsx[f][0] = b;    // trigger blocks start
   fsx[f][1] = b+16; // trigger block end
   fsx[f][2] = b+6;  // middle
   fsx[f][3] = b+14; // left end
   fsx[f][4] = b+12; // right end

   for (int i=0; i<20; i++)
      for (int j=0; j<5; j++)
         fsx[i][j] |= draw_item_flags;


   // -------------------------------------------
   // vertical lines
   // -------------------------------------------
   int fsy[20][5] = {0};

   // semi-solid screen
   b=33; f=0;
   fsy[f][0] = b;   // trigger blocks start
   fsy[f][1] = b+5; // trigger block end
   fsy[f][2] = b+1; // middle
   fsy[f][3] = b;   // upper end
   fsy[f][4] = b+2; // lower end

   // white brain with blue outline
   b = 384; f++;
   fsy[f][0] = b;    // trigger blocks start
   fsy[f][1] = b+16; // trigger block end
   fsy[f][2] = b+8;  // middle
   fsy[f][3] = b+15; // upper end
   fsy[f][4] = b+13; // lower end

   // brown and yellow thatch
   b = 416; f++;
   fsy[f][0] = b;    // trigger blocks start
   fsy[f][1] = b+16; // trigger block end
   fsy[f][2] = b+8;  // middle
   fsy[f][3] = b+15; // upper end
   fsy[f][4] = b+13; // lower end

   // brown brick with corners
   b = 448; f++;
   fsy[f][0] = b;    // trigger blocks start
   fsy[f][1] = b+16; // trigger block end
   fsy[f][2] = b+8;  // middle
   fsy[f][3] = b+15; // upper end
   fsy[f][4] = b+13; // lower end

   // grey bricks
   b = 480; f++;
   fsy[f][0] = b;    // trigger blocks start
   fsy[f][1] = b+16; // trigger block end
   fsy[f][2] = b+8;  // middle
   fsy[f][3] = b+15; // upper end
   fsy[f][4] = b+13; // lower end

   // purple pipes
   b = 576; f++;
   fsy[f][0] = b;    // trigger blocks start
   fsy[f][1] = b+16; // trigger block end
   fsy[f][2] = b+4;  // middle
   fsy[f][3] = b+15; // upper end
   fsy[f][4] = b+13; // lower end

   // wires
   b=608; f++;
   fsy[f][0] = b;    // trigger blocks start
   fsy[f][1] = b+16; // trigger block end
   fsy[f][2] = b+4;  // middle
   fsy[f][3] = b+15; // upper end
   fsy[f][4] = b+13; // lower end






   for (int i=0; i<20; i++)
      for (int j=0; j<5; j++)
         fsy[i][j] |= draw_item_flags;


   if ((mwWM.bx2==mwWM.bx1) && (mwWM.by2==mwWM.by1)) l[mwWM.bx1][mwWM.by1] = mwWM.mW[1].draw_item_num; // single block 1 x 1

   if ((mwWM.bx2==mwWM.bx1) && (mwWM.by2-mwWM.by1>0)) // vertical line 1 x >1
   {
      int a = mwWM.bx1;
      for (int b=mwWM.by1; b<mwWM.by2+1; b++) // cycle the range
      {
         l[a][b] = mwWM.mW[1].draw_item_num; // set draw item as default
         for (int x=0; x<20; x++)
            if (fsy[x][0]&1023)
            {
               if (((mwWM.mW[1].draw_item_num&1023) >= (fsy[x][0]&1023)) && ((mwWM.mW[1].draw_item_num&1023) <= (fsy[x][1]&1023)))
               {
                  l[a][b] = fsy[x][2]; // default
                  if (b == mwWM.by1) l[a][b] = fsy[x][3]; // left end cap
                  if (b == mwWM.by2) l[a][b] = fsy[x][4]; // right end cap
               }
            }
      }
   }
   if ((mwWM.bx2-mwWM.bx1>0) && (mwWM.by2==mwWM.by1)) // horizontal line >1 x 1
   {
      int b = mwWM.by1;
      for (int a=mwWM.bx1; a<mwWM.bx2+1; a++) // cycle the range
      {
         l[a][b] = mwWM.mW[1].draw_item_num; // set draw item as default
         for (int x=0; x<20; x++)
         {
            if (fsx[x][0]&1023)
            {
               if (((mwWM.mW[1].draw_item_num&1023) >= (fsx[x][0]&1023)) && ((mwWM.mW[1].draw_item_num&1023) <= (fsx[x][1]&1023)))
               {
                  l[a][b] = fsx[x][2]; // default
                  if (a == mwWM.bx1) l[a][b] = fsx[x][3]; // left end cap
                  if (a == mwWM.bx2) l[a][b] = fsx[x][4]; // right end cap
               }
            }
         }
      }
   }
   if ((mwWM.bx2-mwWM.bx1>0) && (mwWM.by2-mwWM.by1>0)) // box shape with corners >1 x >1
   {
      int special_handler = 0;
      for (int a=mwWM.bx1; a<mwWM.bx2+1; a++)       // cycle the range
         for (int b=mwWM.by1; b<mwWM.by2+1; b++)
            for (int x=0; x<20; x++)
               if (fsd[x][0]&1023)
               {
                  if (((mwWM.mW[1].draw_item_num&1023) >= (fsd[x][0]&1023)) && ((mwWM.mW[1].draw_item_num&1023) <= (fsd[x][1]&1023)))
                  {
                     special_handler = 1;

                     if (fsd[x][9] != -1) l[a][b] = fsd[x][9];   // default block
                     if (b == mwWM.by1)        l[a][b] = fsd[x][16];  // upper horizontal through
                     if (b == mwWM.by2)        l[a][b] = fsd[x][17];  // lower horizontal through


                     if (a == mwWM.bx1)
                     {
                        if (b == mwWM.by1)      l[a][b] = fsd[x][10];  // upper-right corner
                        else if (b == mwWM.by2) l[a][b] = fsd[x][12];  // lower-right corner
                        else               l[a][b] = fsd[x][14];  // right vertical through
                     }
                     if (a == mwWM.bx2)
                     {
                        if (b == mwWM.by1)      l[a][b] = fsd[x][11];  // upper-left corner
                        else if (b == mwWM.by2) l[a][b] = fsd[x][13];  // lower-left corner
                        else               l[a][b] = fsd[x][15];  // left vertical through
                     }
                  }
                  if (!special_handler) l[a][b] = mwWM.mW[1].draw_item_num;

             } // end of cycle block range
   } // end of box shape with corners
}

char* em_get_text_description_of_block_based_on_flags(int flags)
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

void em_show_draw_item_cursor(void)
{
   int x = mwWM.gx;
   int y = mwWM.gy;
   if (mwWM.mW[1].point_item_type > -1) // if mouse pointer on window, do not show draw item
   {
      int type = mwWM.mW[1].draw_item_type;
      int num  = mwWM.mW[1].draw_item_num;
      switch (type)
      {
         case 1: // block
            if (mwWM.mW[1].show_non_default_blocks) draw_block_non_default_flags(num, x*20, y*20);
            else al_draw_bitmap(mwB.btile[num&1023], x*20, y*20, 0);
         break;
         case 2: // item
            draw_item(num, 1, x*20, y*20);
         break;
         case 3: // enemy
            draw_enemy(num, 1, x*20, y*20);
         break;
         case 5: // PDE
            int a = PDEi[num][1]; // bmp or ans
            if (a > 999) a = zz[5][a-1000]; // ans
            al_draw_bitmap(mwB.tile[a], x*20, y*20, 0);

            if ((PDEi[num][0] == 108) && (PDEi[num][11])) al_draw_bitmap(mwB.tile[440], x*20, y*20, 0); // bomb sticky spikes



         break;
      }
      al_draw_rectangle(x*20, y*20, x*20+21, y*20+21, palette_color[15], 1);
   }
}

void em_show_item_info(int x, int y, int color, int type, int num)
{
   int a, b;
   switch (type)
   {
      case 1:
         if (mwWM.mW[1].show_non_default_blocks) draw_block_non_default_flags(num, x, y);
         else al_draw_bitmap(mwB.btile[num&1023], x, y, 0);
         al_draw_textf(mF.pr8, palette_color[color], x+22, y+2, 0, "Block #%d",num&1023);
         al_draw_textf(mF.pr8, palette_color[color], x+22, y+12, 0, "%s", em_get_text_description_of_block_based_on_flags(num) );
      break;
      case 2:
         draw_item(num, 1, x, y);
         a = item[num][0]; // type
         al_draw_textf(mF.pr8, palette_color[color], x+22, y+2, 0, "%s", item_name[a]);
         al_draw_textf(mF.pr8, palette_color[color], x+22, y+12, 0, "%d of %d", 1+num - item_first_num[a], item_num_of_type[a]);
      break;
      case 3:
         draw_enemy(num, 1, x, y);
         a = Ei[num][0]; // type
         al_draw_textf(mF.pr8, palette_color[color], x+22, y+2, 0, "%s", enemy_name[a][0]);
         al_draw_textf(mF.pr8, palette_color[color], x+22, y+12, 0, "%d of %d", 1+num - e_first_num[a], e_num_of_type[a]);
      break;
      case 4:
      {
         int col = lifts[num].color;
         int width = lifts[num].width;
         if (width > 140) width = 140;
         for (a=0; a<10; a++)
            al_draw_rectangle(x+a, y+a, x+(width)-1-a, y+19-a, palette_color[col+((9-a)*16)], 1 );
         al_draw_text(mF.pr8, palette_color[col+160], x+(width/2), y+6, ALLEGRO_ALIGN_CENTER, lifts[num].lift_name);
      }
      break;
      case 5:
         a = PDEi[num][1]; // bmp or ans
         if (a < NUM_SPRITES) b = a; // bmp
         if (a > 999) b = zz[5][a-1000]; // ans
         al_draw_bitmap(mwB.tile[b], x, y, 0);

         if ((PDEi[num][0] == 108) && (PDEi[num][11])) al_draw_bitmap(mwB.tile[440], x, y, 0); // bomb sticky spikes

         a = Ei[num][0]; // type
         al_draw_text(mF.pr8, palette_color[color], x+22, y+2, 0, "Special Item");
         al_draw_textf(mF.pr8, palette_color[color], x+22, y+12, 0, "%s", PDEt[num][1]);
      break;
   }
}

void em_find_point_item(void)
{
   // find point item
   mwWM.mW[1].point_item_type = 1; // block by default
   mwWM.mW[1].point_item_num = l[mwWM.gx][mwWM.gy];

   int max_ob = 20;                  // max objects to find
   int ob = 0;                       // objects found
   int mo[max_ob][2];                // array of objects found
   for (int a=0; a<max_ob; a++)          // clear array
   {
       mo[a][0] = 0;
       mo[a][1] = 1;
   }
   for (int i=0; i<500; i++) // check for item
      if ((item[i][0]) && (ob < max_ob))
      {
         int x = item[i][4];
         int y = item[i][5];
         if ( (mwWM.hx >= x) && (mwWM.hx <= x+19) && (mwWM.hy > y) && (mwWM.hy < y+19) && (ob < max_ob))
         {
             mo[ob][0] = 2;
             mo[ob][1] = i;
             ob++;
         }
      }
   for (int e=0; e<100; e++) // check for enemy
      if ((Ei[e][0]) && (ob < max_ob))
      {
         int x = al_fixtoi(Efi[e][0]);
         int y = al_fixtoi(Efi[e][1]);
         if ( (mwWM.hx >= x) && (mwWM.hx <= x+19) && (mwWM.hy > y) && (mwWM.hy < y+19) && (ob < max_ob))
         {
             mo[ob][0] = 3;
             mo[ob][1] = e;
             ob++;
         }
      }
   for (int l=0; l<num_lifts; l++) // check for lifts
   {
      int x = lifts[l].x1;
      int y = lifts[l].y1;
      if ( (mwWM.hx >= x) && (mwWM.hx <= x+19) && (mwWM.hy > y) && (mwWM.hy < y+19) && (ob < max_ob))
      {
          mo[ob][0] = 4;
          mo[ob][1] = l;
          ob++;
      }
   }
   //al_draw_textf(mF.pr8, palette_color[15], 100, 100, 0, "mouse is on: %d objects", ob);
   //for (int a=0; a<ob; a++) al_draw_textf(mF.pr8, palette_color[15], 100, 108+a*8, 0, "%d %d ", mo[a][0], mo[a][1]);

   // which one is the winner?
   if (ob)
   {
      int mm = mouse_x % 20;         // mouse position relative to block boundary
      int ss = 20/ob;                // step space
      int of = mm / ss;              // convert to offset into ob array
      mwWM.mW[1].point_item_type = mo[of][0];
      mwWM.mW[1].point_item_num  = mo[of][1];
      //al_draw_textf(mF.pr8, palette_color[11], 100, 92, 0, "mm:%2d ss:%2d of:%2d  ", mm, ss, of);
   }
}

void em_process_mouse(void)
{
   if (mouse_b[1][0])
   {
      // don't allow drag draw selection unless draw type is block
      if (mwWM.mW[1].draw_item_type != 1) while (mouse_b[1][0]) proc_event_queue();

      int din = mwWM.mW[1].draw_item_num; // shorter variable name
      switch (mwWM.mW[1].draw_item_type)
      {
         case 1:  // block
         {
            mwWM.bx1 = mwWM.gx;
            mwWM.by1 = mwWM.gy;
            mwWM.get_new_box();
            em_set_block_range();
            init_level_background(0);
         }
         break;
         case 2:  // item
         {
            int type = item[din][0];
            int ofx = mwWM.gx*20 - item[din][4]; // get offset of move in 2000 format
            int ofy = mwWM.gy*20 - item[din][5];
            int c = get_empty_item(); // get a place to put it

            printf("din:%d c:%d\n", din, c);

            if (c == -1)  break;
            for (int b=0; b<16; b++) item[c][b] = item[din][b]; // copy from draw item
            item[c][4] += ofx; // adjust with offsets
            item[c][5] += ofy;
            if ((type == 4) || (type == 9) || (type == 10) || (type == 16) || (type == 17)) // move range for key, trig, msg, manip, damage
            {
               item[c][6] += ofx; // adjust with offsets
               item[c][7] += ofy;
            }
            if (type == 10)
            {
               int x=0, y=0;
               get_int_3216(item[c][10], x, y); // get x and y of upper left corner
               set_int_3216(item[c][10], x+ofx, y+ofy);
               strcpy(pmsgtext[c], pmsgtext[din]); // msg
            }
            sort_item(1);
         }
         break;
         case 3:    // enemy
         {
            int type = Ei[din][0];

            int ofx = mwWM.gx*20 - al_fixtoi(Efi[din][0]); // get offset of move in 2000 format
            int ofy = mwWM.gy*20 - al_fixtoi(Efi[din][1]);

            int c = get_empty_enemy(type); // get a place to put it
            if (c == -1)  break;
            for (int x=0; x<32; x++) Ei[c][x]  = Ei[din][x];
            for (int x=0; x<16; x++) Efi[c][x] = Efi[din][x];

            Efi[c][0] += al_itofix(ofx);  // apply offsets
            Efi[c][1] += al_itofix(ofy);

            if (type == 13) // vinepod
            {
               for (int i=3; i<12; i+=2)
               {
                  Ei[c][i+0] = Ei[din][i+0] + ofx;
                  Ei[c][i+1] = Ei[din][i+1] + ofy;
               }
            }


            if (type == 7) // podzilla
            {
               if (SHFT()) // move stuff also
               //if (al_show_native_message_box(display, "Move?", "Move podzilla's extended position too?", NULL, NULL, ALLEGRO_MESSAGEBOX_YES_NO | ALLEGRO_MESSAGEBOX_QUESTION ) == 1)
               {
                   Efi[c][5] = Efi[din][5] + al_itofix(ofx);
                   Efi[c][6] = Efi[din][6] + al_itofix(ofy);
               }
               if (SHFT()) // move stuff also
               //if (al_show_native_message_box(display, "Move?", "Move podzilla's trigger box too?", NULL, NULL, ALLEGRO_MESSAGEBOX_YES_NO | ALLEGRO_MESSAGEBOX_QUESTION ) == 1)
               {
                  Ei[c][11] = Ei[din][11] + ofx;
                  Ei[c][12] = Ei[din][12] + ofy;
               }
               recalc_pod(c);
            }
            if (type == 9) // cloner
            {
               //if (al_show_native_message_box(display, "Move?", "Move cloner's source and destination boxes too?", NULL, NULL, ALLEGRO_MESSAGEBOX_YES_NO | ALLEGRO_MESSAGEBOX_QUESTION ) == 1)
               if (SHFT()) // move stuff also
               {
                  Ei[c][15] = Ei[din][15] + ofx;
                  Ei[c][16] = Ei[din][16] + ofy;
                  Ei[c][17] = Ei[din][17] + ofx;
                  Ei[c][18] = Ei[din][18] + ofy;
               }
               //if (al_show_native_message_box(display, "Move?", "Move cloner's trigger box too?", NULL, NULL, ALLEGRO_MESSAGEBOX_YES_NO | ALLEGRO_MESSAGEBOX_QUESTION ) == 1)
               if (SHFT()) // move stuff also
               {
                  Ei[c][11] = Ei[din][11] + ofx;
                  Ei[c][12] = Ei[din][12] + ofy;
               }
            }
            sort_enemy();
         }
         break;
         case 5: // Special
         if ((PDEi[din][0] > 99) && (PDEi[din][0] < 200)) // PDE item
         {
            int d = get_empty_item(); // get a place to put it
            if (d == -1)  break;
            // copy from pde
            for (int x=0; x<16; x++) // item
               item[d][x] = PDEi[din][x];
            item[d][0] -= 100;
            item[d][4] = mwWM.gx*20;
            item[d][5] = mwWM.gy*20;
            if (item[d][0] == 4)
            {
               itemf[d][0] = al_itofix(item[d][4]);
               itemf[d][1] = al_itofix(item[d][5]);
               get_block_range("Block Range", &item[d][6], &item[d][7], &item[d][8], &item[d][9], 1);
            }
            sort_item(1);
         }
         if (PDEi[din][0] < 99) // PDE enemy
         {
            int d = get_empty_enemy(); // get a place to put it
            if (d == -1)  break;
            for (int x=0; x<32; x++) Ei[d][x]  = PDEi[din][x];
            for (int x=0; x<16; x++) Efi[d][x] = PDEfx[din][x];
            Efi[d][0] = al_itofix(mwWM.gx*20);  // set new x,y
            Efi[d][1] = al_itofix(mwWM.gy*20);
            sort_enemy();
         }
         break;
      } // end of switch case
   } // end of mouse_b[1][0]
   if (mouse_b[2][0])
   {
      switch (mwWM.mW[1].point_item_type)
      {
         case 1:
            sprintf(global_string[2][2], "Copy Block    ");
            sprintf(global_string[2][3], "              ");
            sprintf(global_string[2][4], "                ");
         break;
         case 2:
            sprintf(global_string[2][2], "Copy %s  ",  item_name[item[mwWM.mW[1].point_item_num][0]]);
            sprintf(global_string[2][3], "View %s  ",  item_name[item[mwWM.mW[1].point_item_num][0]]);
            sprintf(global_string[2][4], "Delete %s ", item_name[item[mwWM.mW[1].point_item_num][0]]);
         break;
         case 3:
            sprintf(global_string[2][2], "Copy %s  ",  (const char *)enemy_name[Ei[mwWM.mW[1].point_item_num][0]][0]);
            sprintf(global_string[2][3], "View %s  ",  (const char *)enemy_name[Ei[mwWM.mW[1].point_item_num][0]][0]);
            sprintf(global_string[2][4], "Delete %s ", (const char *)enemy_name[Ei[mwWM.mW[1].point_item_num][0]][0]);
         break;
         case 4:
            sprintf(global_string[2][2], "              ");
            sprintf(global_string[2][3], "View Lift '%s'",   lifts[mwWM.mW[1].point_item_num].lift_name);
            sprintf(global_string[2][4], "Delete Lift '%s'", lifts[mwWM.mW[1].point_item_num].lift_name);
         break;
      }

      switch (pmenu(2, 0))
      {
         case 2:  // copy
            if (mwWM.mW[1].point_item_type < 4)
            {
               mwWM.mW[1].draw_item_type = mwWM.mW[1].point_item_type;
               mwWM.mW[1].draw_item_num  = mwWM.mW[1].point_item_num;
            }
         break;
         case 3:  // view
            if (mwWM.mW[1].point_item_type > 1) object_viewer(mwWM.mW[1].point_item_type, mwWM.mW[1].point_item_num);
         break;
         case 4:  // delete
            switch (mwWM.mW[1].point_item_type)
            {
               case 1: // delete block
                    l[mwWM.gx][mwWM.gy] = 0;
               break;
               case 2: // delete item
                  if ((mwWM.mW[1].draw_item_type == 2) && (mwWM.mW[1].draw_item_num == mwWM.mW[1].point_item_num)) // are you deleting the draw item?
                  {
                     mwWM.mW[1].draw_item_type = 1;
                     mwWM.mW[1].draw_item_num = 0;
                  }
                  erase_item(mwWM.mW[1].point_item_num);
                  sort_item(1);
               break;
               case 3: // delete enemy
                  if ((mwWM.mW[1].draw_item_type == 3) && (mwWM.mW[1].draw_item_num == mwWM.mW[1].point_item_num)) // are you deleting the draw item?
                  {
                     mwWM.mW[1].draw_item_type = 1;
                     mwWM.mW[1].draw_item_num = 0;
                  }
                  erase_enemy(mwWM.mW[1].point_item_num);
                  sort_enemy();
               break;
               case 4: // delete lift
                  erase_lift(mwWM.mW[1].point_item_num);
               break;
            }
         break;
         case 5: break; // menu divider
         case 6: mwWM.set_windows(2); break;   // edit selection
         case 7: mwWM.set_windows(3); break;   // group edit
         case 8: mwWM.set_windows(9); break;   // tile helper
         case 9: mwWM.mW[1].active = 1; break; // status_window
         case 10: mwWM.mW[2].active = 1; break; // select_window
         case 12: // new level
         if (al_show_native_message_box(display, "New Level", "Clicking OK will create a new blank level", NULL, NULL, ALLEGRO_MESSAGEBOX_OK_CANCEL) == 1)
         {
            zero_level_data();
            save_level_prompt();
         }
         load_level(last_level_loaded, 0); // blind load
         break;
         case 13: // load level
            load_level_prompt();
            sort_enemy();
            sort_item(1);
         break;
         case 14: save_level(last_level_loaded); break; // save level
         case 15: mwWM.active = 0; break; // save and exit
         case 16: help("Level Editor Basics"); break;// help
         case 17: mwWM.active = 0; break; // exit
      } // end of switch case
   } // end of mouse_b[2][0]
}


int edit_menu(int edit_level)
{
   mwWM.loop(edit_level);
   return last_level_loaded;
}
