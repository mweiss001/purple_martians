// e_editor_main.cpp
#include "pm.h"

#include "mwWindow.h"
#include "mwFont.h"
#include "mwBitmap.h"
#include "mwLift.h"
#include "mwPDE.h"
#include "mwColor.h"
#include "mwInput.h"
#include "mwDisplay.h"
#include "mwEventQueue.h"
#include "mwHelp.h"
#include "mwItems.h"
#include "mwEnemy.h"
#include "mwLevel.h"
#include "mwMiscFnx.h"
#include "mwScreen.h"
#include "mwMenu.h"


#ifndef CLASS_MWWINDOWS_DEFINED
#include "mwWindowManager.h"
#endif

void mwWindow::em_set_swbl(void)
{
   mwWM.mW[2].swbn = 0;
   for (int c=0; c<NUM_SPRITES; c++)
   {
      mwWM.swbl[c][0] = mwWM.swbl[c][1] = 0;                    // erase
      if (mBitmap.sa[c][0] & PM_BTILE_SHOW_SELECT_WIN)
      {
         if ((c == 384) || (c == 416) || (c == 448) || (c == 480) || (c == 512) || (c == 576) || (c == 608) || (c == 640)|| (c == 672)|| (c == 704)) // start new line
         {
            int off = (16 - (mwWM.mW[2].swbn % 16));
            if (off < 16) mwWM.mW[2].swbn += off;
         }

         mwWM.swbl[mwWM.mW[2].swbn][0] = c | mBitmap.sa[c][0];                // add to list with default flags
         mwWM.swbl[mwWM.mW[2].swbn][0] &= ~PM_BTILE_SHOW_SELECT_WIN;  // clear flag
         mwWM.mW[2].swbn++;
      }
   }
   mwWM.mW[2].swnbl = (mwWM.mW[2].swbn / 16) + 1;
   if (mwWM.mW[2].swnbl_cur == 0) mwWM.mW[2].swnbl_cur = mwWM.mW[2].swnbl; // initial only
}



void mwWindow::em_set_block_range(void)
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


   if ((mwWM.bx2==mwWM.bx1) && (mwWM.by2==mwWM.by1)) mLevel.l[mwWM.bx1][mwWM.by1] = mwWM.mW[1].draw_item_num; // single block 1 x 1

   if ((mwWM.bx2==mwWM.bx1) && (mwWM.by2-mwWM.by1>0)) // vertical line 1 x >1
   {
      int a = mwWM.bx1;
      for (int b=mwWM.by1; b<mwWM.by2+1; b++) // cycle the range
      {
         mLevel.l[a][b] = mwWM.mW[1].draw_item_num; // set draw item as default
         for (int x=0; x<20; x++)
            if (fsy[x][0]&1023)
            {
               if (((mwWM.mW[1].draw_item_num&1023) >= (fsy[x][0]&1023)) && ((mwWM.mW[1].draw_item_num&1023) <= (fsy[x][1]&1023)))
               {
                                     mLevel.l[a][b] = fsy[x][2]; // default
                  if (b == mwWM.by1) mLevel.l[a][b] = fsy[x][3]; // left end cap
                  if (b == mwWM.by2) mLevel.l[a][b] = fsy[x][4]; // right end cap
               }
            }
      }
   }
   if ((mwWM.bx2-mwWM.bx1>0) && (mwWM.by2==mwWM.by1)) // horizontal line >1 x 1
   {
      int b = mwWM.by1;
      for (int a=mwWM.bx1; a<mwWM.bx2+1; a++) // cycle the range
      {
         mLevel.l[a][b] = mwWM.mW[1].draw_item_num; // set draw item as default
         for (int x=0; x<20; x++)
         {
            if (fsx[x][0]&1023)
            {
               if (((mwWM.mW[1].draw_item_num&1023) >= (fsx[x][0]&1023)) && ((mwWM.mW[1].draw_item_num&1023) <= (fsx[x][1]&1023)))
               {
                                     mLevel.l[a][b] = fsx[x][2]; // default
                  if (a == mwWM.bx1) mLevel.l[a][b] = fsx[x][3]; // left end cap
                  if (a == mwWM.bx2) mLevel.l[a][b] = fsx[x][4]; // right end cap
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

                     if (fsd[x][9] != -1) mLevel.l[a][b] = fsd[x][9];   // default block
                     if (b == mwWM.by1)   mLevel.l[a][b] = fsd[x][16];  // upper horizontal through
                     if (b == mwWM.by2)   mLevel.l[a][b] = fsd[x][17];  // lower horizontal through


                     if (a == mwWM.bx1)
                     {
                        if (b == mwWM.by1)      mLevel.l[a][b] = fsd[x][10];  // upper-right corner
                        else if (b == mwWM.by2) mLevel.l[a][b] = fsd[x][12];  // lower-right corner
                        else                    mLevel.l[a][b] = fsd[x][14];  // right vertical through
                     }
                     if (a == mwWM.bx2)
                     {
                        if (b == mwWM.by1)      mLevel.l[a][b] = fsd[x][11];  // upper-left corner
                        else if (b == mwWM.by2) mLevel.l[a][b] = fsd[x][13];  // lower-left corner
                        else                    mLevel.l[a][b] = fsd[x][15];  // left vertical through
                     }
                  }
                  if (!special_handler)         mLevel.l[a][b] = mwWM.mW[1].draw_item_num;

             } // end of cycle block range
   } // end of box shape with corners
}


char* mwWindow::em_get_text_description_of_block_based_on_flags(int flags, char * msg)
{
   sprintf(msg, "Empty");  // default

   if (flags & PM_BTILE_SOLID_PLAYER)     sprintf(msg, "Solid");
   if (flags & PM_BTILE_SEMISOLID_PLAYER) sprintf(msg, "Semi-Solid");
   if (flags & PM_BTILE_BREAKABLE_PSHOT)  sprintf(msg, "Breakable");
   if (flags & PM_BTILE_BOMBABLE)         sprintf(msg, "Bombable");
   if (flags & PM_BTILE_LADDER_MOVE)      sprintf(msg, "Ladder");
   if (flags & PM_BTILE_ROPE_MOVE)        sprintf(msg, "Rope");
   return msg;
}

void mwWindow::em_show_draw_item_cursor(void)
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
            if (mwWM.mW[1].show_non_default_blocks) mMiscFnx.draw_block_non_default_flags(num, x*20, y*20);
            else al_draw_bitmap(mBitmap.btile[num&1023], x*20, y*20, 0);
         break;
         case 2: // item
            mItem.draw_item(num, 1, x*20, y*20);
         break;
         case 3: // enemy
            mEnemy.draw_enemy(num, 1, x*20, y*20);
         break;
         case 5: // PDE
            int a = mPDE.PDEi[num][1]; // bmp or ans
            if (a > 999) a = mBitmap.zz[5][a-1000]; // ans
            al_draw_bitmap(mBitmap.tile[a], x*20, y*20, 0);
            if ((mPDE.PDEi[num][0] == 108) && (mPDE.PDEi[num][11])) al_draw_bitmap(mBitmap.tile[440], x*20, y*20, 0); // bomb sticky spikes
         break;
      }
      al_draw_rectangle(x*20, y*20, x*20+21, y*20+21, mColor.pc[15], 1);
   }
}

void mwWindow::em_show_item_info(int x, int y, int color, int type, int num)
{
   char msg[1024];
   int a, b;
   switch (type)
   {
      case 1:
         if (mwWM.mW[1].show_non_default_blocks) mMiscFnx.draw_block_non_default_flags(num, x, y);
         else al_draw_bitmap(mBitmap.btile[num&1023], x, y, 0);
         al_draw_textf(mFont.pr8, mColor.pc[color], x+22, y+2, 0, "Block #%d",num&1023);
         al_draw_textf(mFont.pr8, mColor.pc[color], x+22, y+12, 0, "%s", em_get_text_description_of_block_based_on_flags(num, msg));
      break;
      case 2:
         mItem.draw_item(num, 1, x, y);
         a = mItem.item[num][0]; // type
         al_draw_textf(mFont.pr8, mColor.pc[color], x+22, y+2, 0, "%s", mItem.item_name[a]);
         al_draw_textf(mFont.pr8, mColor.pc[color], x+22, y+12, 0, "%d of %d", 1+num - mItem.item_first_num[a], mItem.item_num_of_type[a]);
      break;
      case 3:
         mEnemy.draw_enemy(num, 1, x, y);
         a = mEnemy.Ei[num][0]; // type
         al_draw_textf(mFont.pr8, mColor.pc[color], x+22, y+2, 0, "%s", mEnemy.enemy_name[a][0]);
         al_draw_textf(mFont.pr8, mColor.pc[color], x+22, y+12, 0, "%d of %d", 1+num - mEnemy.e_first_num[a], mEnemy.e_num_of_type[a]);
      break;
      case 4:
      {
         int col = mLift.cur[num].color;
         int width = mLift.cur[num].w;
         if (width > 140) width = 140;
         for (a=0; a<10; a++)
            al_draw_rectangle(x+a, y+a, x+(width)-1-a, y+19-a, mColor.pc[col+((9-a)*16)], 1 );
         al_draw_text(mFont.pr8, mColor.pc[col+160], x+(width/2), y+6, ALLEGRO_ALIGN_CENTER, mLift.cur[num].lift_name);
      }
      break;
      case 5:
         a = mPDE.PDEi[num][1]; // bmp or ans
         if (a < NUM_SPRITES) b = a; // bmp
         if (a > 999) b = mBitmap.zz[5][a-1000]; // ans
         al_draw_bitmap(mBitmap.tile[b], x, y, 0);

         if ((mPDE.PDEi[num][0] == 108) && (mPDE.PDEi[num][11])) al_draw_bitmap(mBitmap.tile[440], x, y, 0); // bomb sticky spikes

         a = mEnemy.Ei[num][0]; // type
         al_draw_text(mFont.pr8, mColor.pc[color], x+22, y+2, 0, "Special Item");
         al_draw_textf(mFont.pr8, mColor.pc[color], x+22, y+12, 0, "%s", mPDE.PDEt[num][1]);
      break;
   }
}

void mwWindow::em_find_point_item(void)
{
   // find point item
   mwWM.mW[1].point_item_type = 1; // block by default
   mwWM.mW[1].point_item_num = mLevel.l[mwWM.gx][mwWM.gy];

   int max_ob = 20;                  // max objects to find
   int ob = 0;                       // objects found
   int mo[max_ob][2];                // array of objects found
   for (int a=0; a<max_ob; a++)          // clear array
   {
       mo[a][0] = 0;
       mo[a][1] = 1;
   }
   for (int i=0; i<500; i++) // check for item
      if ((mItem.item[i][0]) && (ob < max_ob))
      {
         int x = mItem.item[i][4];
         int y = mItem.item[i][5];
         if ( (mwWM.hx >= x) && (mwWM.hx <= x+19) && (mwWM.hy > y) && (mwWM.hy < y+19) && (ob < max_ob))
         {
             mo[ob][0] = 2;
             mo[ob][1] = i;
             ob++;
         }
      }
   for (int e=0; e<100; e++) // check for enemy
      if ((mEnemy.Ei[e][0]) && (ob < max_ob))
      {
         int x = mEnemy.Ef[e][0];
         int y = mEnemy.Ef[e][1];
         if ( (mwWM.hx >= x) && (mwWM.hx <= x+19) && (mwWM.hy > y) && (mwWM.hy < y+19) && (ob < max_ob))
         {
             mo[ob][0] = 3;
             mo[ob][1] = e;
             ob++;
         }
      }
   for (int l=0; l<NUM_LIFTS; l++) // check for lifts
      if (mLift.cur[l].active)
      {
         int x = mLift.cur[l].x;
         int y = mLift.cur[l].y;
         if ( (mwWM.hx >= x) && (mwWM.hx <= x+19) && (mwWM.hy > y) && (mwWM.hy < y+19) && (ob < max_ob))
         {
             mo[ob][0] = 4;
             mo[ob][1] = l;
             ob++;
         }
      }
   //al_draw_textf(mFont.pr8, mColor.pc[15], 100, 100, 0, "mouse is on: %d objects", ob);
   //for (int a=0; a<ob; a++) al_draw_textf(mFont.pr8, mColor.pc[15], 100, 108+a*8, 0, "%d %d ", mo[a][0], mo[a][1]);

   // which one is the winner?
   if (ob)
   {
      int mm = mInput.mouse_x % 20;         // mouse position relative to block boundary
      int ss = 20/ob;                // step space
      int of = mm / ss;              // convert to offset into ob array
      mwWM.mW[1].point_item_type = mo[of][0];
      mwWM.mW[1].point_item_num  = mo[of][1];
      //al_draw_textf(mFont.pr8, mColor.pc[11], 100, 92, 0, "mm:%2d ss:%2d of:%2d  ", mm, ss, of);
   }
}

void mwWindow::em_process_mouse(void)
{
   if (mInput.mouse_b[1][0])
   {
      // don't allow drag draw selection unless draw type is block
      if (mwWM.mW[1].draw_item_type != 1) while (mInput.mouse_b[1][0]) mEventQueue.proc();

      int din = mwWM.mW[1].draw_item_num; // shorter variable name
      switch (mwWM.mW[1].draw_item_type)
      {
         case 1:  // block
         {
            mwWM.bx1 = mwWM.gx;
            mwWM.by1 = mwWM.gy;
            mwWM.get_new_box();
            em_set_block_range();
            mScreen.init_level_background(0);
         }
         break;
         case 2:  // item
         {
            int type = mItem.item[din][0];
            int ofx = mwWM.gx*20 - mItem.item[din][4]; // get offset of move in 2000 format
            int ofy = mwWM.gy*20 - mItem.item[din][5];
            int c = mItem.get_empty_item(); // get a place to put it
            //printf("din:%d c:%d\n", din, c);
            if (c == -1)  break;
            for (int b=0; b<16; b++) mItem.item[c][b] = mItem.item[din][b]; // copy from draw item

            mItem.item[c][4] += ofx; // adjust with offsets
            mItem.item[c][5] += ofy;

            if (mItem.item_secondary67(type))
            {
               mItem.item[c][6] += ofx; // adjust with offsets
               mItem.item[c][7] += ofy;
            }
            if (type == 10) strcpy(mItem.pmsgtext[c], mItem.pmsgtext[din]); // message
            mItem.sort_item(1);
         }
         break;
         case 3:    // enemy
         {
            int type = mEnemy.Ei[din][0];

            int ofx = mwWM.gx*20 - mEnemy.Ef[din][0]; // get offset of move in 2000 format
            int ofy = mwWM.gy*20 - mEnemy.Ef[din][1];

            int c = mEnemy.get_empty_enemy(type); // get a place to put it
            if (c == -1)  break;
            for (int x=0; x<32; x++) mEnemy.Ei[c][x] = mEnemy.Ei[din][x];
            for (int x=0; x<16; x++) mEnemy.Ef[c][x] = mEnemy.Ef[din][x];

            mEnemy.Ef[c][0] += ofx;  // apply offsets
            mEnemy.Ef[c][1] += ofy;

            if (type == 13) // vinepod
            {
               for (int i=3; i<12; i+=2)
               {
                  mEnemy.Ei[c][i+0] = mEnemy.Ei[din][i+0] + ofx;
                  mEnemy.Ei[c][i+1] = mEnemy.Ei[din][i+1] + ofy;
               }
            }
            if (type == 7) // podzilla
            {
               if (mInput.SHFT()) // move stuff also
               //if (al_show_native_message_box(mDisplay.display, "Move?", "Move podzilla's extended position too?", NULL, NULL, ALLEGRO_MESSAGEBOX_YES_NO | ALLEGRO_MESSAGEBOX_QUESTION ) == 1)
               {
                   mEnemy.Ef[c][5] = mEnemy.Ef[din][5] + ofx;
                   mEnemy.Ef[c][6] = mEnemy.Ef[din][6] + ofy;
               }
               if (mInput.SHFT()) // move stuff also
               //if (al_show_native_message_box(mDisplay.display, "Move?", "Move podzilla's trigger box too?", NULL, NULL, ALLEGRO_MESSAGEBOX_YES_NO | ALLEGRO_MESSAGEBOX_QUESTION ) == 1)
               {
                  mEnemy.Ei[c][11] = mEnemy.Ei[din][11] + ofx;
                  mEnemy.Ei[c][12] = mEnemy.Ei[din][12] + ofy;
               }
               mEnemy.recalc_pod(c);
            }
            if (type == 9) // cloner
            {
               //if (al_show_native_message_box(mDisplay.display, "Move?", "Move cloner's source and destination boxes too?", NULL, NULL, ALLEGRO_MESSAGEBOX_YES_NO | ALLEGRO_MESSAGEBOX_QUESTION ) == 1)
               if (mInput.SHFT()) // move stuff also
               {
                  mEnemy.Ei[c][15] = mEnemy.Ei[din][15] + ofx;
                  mEnemy.Ei[c][16] = mEnemy.Ei[din][16] + ofy;
                  mEnemy.Ei[c][17] = mEnemy.Ei[din][17] + ofx;
                  mEnemy.Ei[c][18] = mEnemy.Ei[din][18] + ofy;
               }
            }
            mEnemy.sort_enemy();
         }
         break;
         case 5: // Special
         if ((mPDE.PDEi[din][0] > 99) && (mPDE.PDEi[din][0] < 200)) // PDE item
         {
            int d = mItem.get_empty_item(); // get a place to put it
            if (d == -1)  break;
            // copy from pde
            for (int x=0; x<16; x++) // item
               mItem.item[d][x] = mPDE.PDEi[din][x];
            mItem.item[d][0] -= 100;
            mItem.item[d][4] = mwWM.gx*20;
            mItem.item[d][5] = mwWM.gy*20;
            if (mItem.item[d][0] == 4)
            {
               mItem.itemf[d][0] = mItem.item[d][4];
               mItem.itemf[d][1] = mItem.item[d][5];
               mMiscFnx.get_block_range("Block Range", &mItem.item[d][6], &mItem.item[d][7], &mItem.item[d][8], &mItem.item[d][9], 1);
            }
            mItem.sort_item(1);
         }
         if (mPDE.PDEi[din][0] < 99) // PDE enemy
         {
            int d = mEnemy.get_empty_enemy(); // get a place to put it
            if (d == -1)  break;
            for (int x=0; x<32; x++) mEnemy.Ei[d][x] = mPDE.PDEi[din][x];
            for (int x=0; x<16; x++) mEnemy.Ef[d][x] = mPDE.PDEf[din][x];
            mEnemy.Ef[d][0] = mwWM.gx*20;  // set new x,y
            mEnemy.Ef[d][1] = mwWM.gy*20;
            mEnemy.sort_enemy();
         }
         break;
      } // end of switch case
   } // end of mInput.mouse_b[1][0]
   if (mInput.mouse_b[2][0])
   {
      switch (mwWM.mW[1].point_item_type)
      {
         case 1:
            sprintf(mMenu.menu_string[2], "Copy Block    ");
            sprintf(mMenu.menu_string[3], "              ");
            sprintf(mMenu.menu_string[4], "                ");
         break;
         case 2:
            sprintf(mMenu.menu_string[2], "Copy %s  ",  mItem.item_name[mItem.item[mwWM.mW[1].point_item_num][0]]);
            sprintf(mMenu.menu_string[3], "View %s  ",  mItem.item_name[mItem.item[mwWM.mW[1].point_item_num][0]]);
            sprintf(mMenu.menu_string[4], "Delete %s ", mItem.item_name[mItem.item[mwWM.mW[1].point_item_num][0]]);
         break;
         case 3:
            sprintf(mMenu.menu_string[2], "Copy %s  ",  (const char *)mEnemy.enemy_name[mEnemy.Ei[mwWM.mW[1].point_item_num][0]][0]);
            sprintf(mMenu.menu_string[3], "View %s  ",  (const char *)mEnemy.enemy_name[mEnemy.Ei[mwWM.mW[1].point_item_num][0]][0]);
            sprintf(mMenu.menu_string[4], "Delete %s ", (const char *)mEnemy.enemy_name[mEnemy.Ei[mwWM.mW[1].point_item_num][0]][0]);
         break;
         case 4:
            sprintf(mMenu.menu_string[2], "              ");
            sprintf(mMenu.menu_string[3], "View Lift '%s'",   mLift.cur[mwWM.mW[1].point_item_num].lift_name);
            sprintf(mMenu.menu_string[4], "Delete Lift '%s'", mLift.cur[mwWM.mW[1].point_item_num].lift_name);
         break;
      }

      switch (mMenu.pmenu(2, 0))
      {
         case 2:  // copy
            if (mwWM.mW[1].point_item_type < 4)
            {
               mwWM.mW[1].draw_item_type = mwWM.mW[1].point_item_type;
               mwWM.mW[1].draw_item_num  = mwWM.mW[1].point_item_num;
            }
         break;
         case 3:  // view
            if (mwWM.mW[1].point_item_type > 1) mwWM.mW[7].object_viewer(mwWM.mW[1].point_item_type, mwWM.mW[1].point_item_num);
         break;
         case 4:  // delete
            switch (mwWM.mW[1].point_item_type)
            {
               case 1: // delete block
                    mLevel.l[mwWM.gx][mwWM.gy] = 0;
               break;
               case 2: // delete item
                  if ((mwWM.mW[1].draw_item_type == 2) && (mwWM.mW[1].draw_item_num == mwWM.mW[1].point_item_num)) // are you deleting the draw item?
                  {
                     mwWM.mW[1].draw_item_type = 1;
                     mwWM.mW[1].draw_item_num = 0;
                  }
                  mItem.erase_item(mwWM.mW[1].point_item_num);
                  mItem.sort_item(1);
               break;
               case 3: // delete enemy
                  if ((mwWM.mW[1].draw_item_type == 3) && (mwWM.mW[1].draw_item_num == mwWM.mW[1].point_item_num)) // are you deleting the draw item?
                  {
                     mwWM.mW[1].draw_item_type = 1;
                     mwWM.mW[1].draw_item_num = 0;
                  }
                  mEnemy.erase_enemy(mwWM.mW[1].point_item_num);
                  mEnemy.sort_enemy();
               break;
               case 4: // delete lift
                  mLift.erase_lift(mwWM.mW[1].point_item_num);
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
         if (al_show_native_message_box(mDisplay.display, "New Level", "Clicking OK will create a new blank level", NULL, NULL, ALLEGRO_MESSAGEBOX_OK_CANCEL) == 1)
         {
            mLevel.zero_level_data();
            mLevel.save_level_prompt();
         }
         mLevel.load_level(mLevel.last_level_loaded, 0, 1); // blind load
         break;
         case 13: // load level
            mLevel.load_level_prompt();
            mEnemy.sort_enemy();
            mItem.sort_item(1);
         break;
         case 14: mLevel.save_level(mLevel.last_level_loaded); break; // save level
         case 15: mwWM.active = 0; break; // save and exit
         case 16: mHelp.help("Level Editor Basics"); break;// help
         case 17: mwWM.active = 0; break; // exit
      } // end of switch case
   } // end of mInput.mouse_b[2][0]
}


int mwWindow::edit_menu(int edit_level)
{
   mwWM.loop(edit_level);
   return mLevel.last_level_loaded;
}