// mwWindowTileHelper.cpp

#include "pm.h"
#include "mwBitmapTools.h"
#include "mwTileSets.h"
#include "mwWindow.h"
#include "mwWindowManager.h"

#include "mwScreen.h"
#include "mwDisplay.h"
#include "mwWidget.h"
#include "mwColor.h"
#include "mwLevel.h"
#include "mwMiscFnx.h"
#include "mwInput.h"
#include "mwEventQueue.h"
#include "mwFont.h"


int mwWindow::th_replace_helper_extended(int tile, int ot, int l, int r, int t, int b, int tl, int tr, int bl, int br)
{
   int fb = -1;

   // find the tileSet
   for (const auto& s : mTileSets.tileSets)
      if (s.startIndex == tile && s.extendedMode)
      {
         fb = s.SolidFill;

         if ((l == 0) && (r == 1) && (t == 0) && (b == 1)) fb = s.OuterCornerTL;
         if ((l == 1) && (r == 0) && (t == 0) && (b == 1)) fb = s.OuterCornerTR;
         if ((l == 0) && (r == 1) && (t == 1) && (b == 0)) fb = s.OuterCornerBL;
         if ((l == 1) && (r == 0) && (t == 1) && (b == 0)) fb = s.OuterCornerBR;

         // solid interior
         if ((l == 1) && (r == 1) && (t == 1) && (b == 1)) // blocks on all l r t b
         {

            // single corner notch
            if (tl == 0) fb = s.InnerCornerBR; // empty to tl
            if (tr == 0) fb = s.InnerCornerBL; // empty to tr
            if (bl == 0) fb = s.InnerCornerTR; // empty to bl
            if (br == 0) fb = s.InnerCornerTL; // empty to br

            // double corner notches
            if ((tr == 0) && (br == 0)) fb = s.OuterEdgeRTee; // empty to tr and br
            if ((br == 0) && (bl == 0)) fb = s.OuterEdgeBTee; // empty to br and bl
            if ((bl == 0) && (tl == 0)) fb = s.OuterEdgeLTee; // empty to bl and tl
            if ((tl == 0) && (tr == 0)) fb = s.OuterEdgeTTee; // empty to tl and tr

            // triple corner notches
            if ((tr == 0) && (tl == 1) && (bl == 0) && (br == 0)) fb = s.OuterCornerBRTeeBR; // only tl
            if ((tr == 1) && (tl == 0) && (bl == 0) && (br == 0)) fb = s.OuterCornerBLTeeBL; // only tr
            if ((tr == 0) && (tl == 0) && (bl == 0) && (br == 1)) fb = s.OuterCornerTLTeeTL; // only br
            if ((tr == 0) && (tl == 0) && (bl == 1) && (br == 0)) fb = s.OuterCornerTRTeeTR; // only bl

            // opposite corner notches
            if ((tr == 0) && (tl == 1) && (bl == 0) && (br == 1)) fb = s.OuterCornerTRDiag;
            if ((tr == 1) && (tl == 0) && (bl == 1) && (br == 0)) fb = s.OuterCornerTLDiag;

            // all corner notches (full cross)
            if ((tr == 0) && (tl == 0) && (bl == 0) && (br == 0)) fb = s.FrameCross; // no diagonals
         }

         if ((l == 0) && (r == 1) && (t == 1) && (b == 1)) // solid except for left
         {
            if ((tr == 1) && (br == 1)) fb = s.OuterEdgeL;        // solid to tr and br
            if ((tr == 0) && (br == 1)) fb = s.OuterCornerTLTeeT; // open to tr and solid to br
            if ((tr == 1) && (br == 0)) fb = s.OuterCornerBLTeeB; // solid to tr and open to br
            if ((tr == 0) && (br == 0)) fb = s.FrameEdgeLTee;     // open to tr and br
         }

         if ((l == 1) && (r == 0) && (t == 1) && (b == 1)) // solid except for right
         {
            if ((tl == 1) && (bl == 1)) fb = s.OuterEdgeR;        // solid to tl and bl
            if ((tl == 0) && (bl == 1)) fb = s.OuterCornerTRTeeT; // open to tl and solid to bl
            if ((tl == 1) && (bl == 0)) fb = s.OuterCornerBRTeeB; // solid to tl and open to bl
            if ((tl == 0) && (bl == 0)) fb = s.FrameEdgeRTee;     // open to tl and bl
         }

         if ((l == 1) && (r == 1) && (t == 0) && (b == 1)) // solid except for top
         {
            if ((bl == 1) && (br == 1)) fb = s.OuterEdgeT;        // solid to bl and br
            if ((bl == 0) && (br == 1)) fb = s.OuterCornerTLTeeL; // open to bl and solid to br
            if ((bl == 1) && (br == 0)) fb = s.OuterCornerTRTeeR; // solid to bl and open to br
            if ((bl == 0) && (br == 0)) fb = s.FrameEdgeBTee;     // open to bl and br
         }

         if ((l == 1) && (r == 1) && (t == 1) && (b == 0)) // solid except for bottom
         {
            if ((tl == 1) && (tr == 1)) fb = s.OuterEdgeB;         // solid to tl and tr
            if ((tl == 0) && (tr == 1)) fb = s.OuterCornerBLTeeL;  // open to tl and solid to tr
            if ((tl == 1) && (tr == 0)) fb = s.OuterCornerBRTeeR;  // solid to tl and open to tr
            if ((tl == 0) && (tr == 0)) fb = s.FrameEdgeTTee;      // open to tl and tr
         }

         if ((l == 0) && (r == 0) && (t == 0) && (b == 0)) // no blocks on all u d l r
             fb = s.Single;  // orphan single block

         // single block line corners
         if ((l == 0) && (r == 1) && (t == 0) && (b == 1) && (br == 0)) fb = s.FrameCornerTL; // top left corner
         if ((l == 1) && (r == 0) && (t == 0) && (b == 1) && (bl == 0)) fb = s.FrameCornerTR; // top right corner
         if ((l == 0) && (r == 1) && (t == 1) && (b == 0) && (tr == 0)) fb = s.FrameCornerBL; // bottom left corner
         if ((l == 1) && (r == 0) && (t == 1) && (b == 0) && (tl == 0)) fb = s.FrameCornerBR; // bottom right corner

         if ((l == 0) && (r == 0) && (t == 1) && (b == 1)) fb = s.VLineM; // vertical through line
         if ((l == 1) && (r == 1) && (t == 0) && (b == 0)) fb = s.HLineM; // horizontal through line

         if ((l == 1) && (r == 0) && (t == 0) && (b == 0)) fb = s.HLineR; // right end line
         if ((l == 0) && (r == 0) && (t == 1) && (b == 0)) fb = s.VLineB; // bottom end line
         if ((l == 0) && (r == 1) && (t == 0) && (b == 0)) fb = s.HLineL; // left end line
         if ((l == 0) && (r == 0) && (t == 0) && (b == 1)) fb = s.VLineT; // top end line
      }

   return fb;
}


// for rainbows only
int mwWindow::th_replace_helper3(int tile, int l, int r, int t, int b, int tl, int tr, int bl, int br)
{
   int fb = -1;

   if ((l == 0) && (r == 1) && (t == 0) && (b == 1)) fb = tile + 0;  // upper left corner
   if ((l == 1) && (r == 0) && (t == 0) && (b == 1)) fb = tile + 1;  // upper right corner
   if ((l == 0) && (r == 1) && (t == 1) && (b == 0)) fb = tile + 2;  // lower left corner
   if ((l == 1) && (r == 0) && (t == 1) && (b == 0)) fb = tile + 3;  // lower right corner

   if ((l == 0) && (r == 1) && (t == 1) && (b == 1)) fb = tile + 4;  // left vertical tee
   if ((l == 1) && (r == 0) && (t == 1) && (b == 1)) fb = tile + 5;  // right vertical tee
   if ((l == 1) && (r == 1) && (t == 0) && (b == 1)) fb = tile + 6;  // upper horizontal tee
   if ((l == 1) && (r == 1) && (t == 1) && (b == 0)) fb = tile + 7;  // lower horizontal tee

   if ((l == 0) && (r == 0) && (t == 1) && (b == 1)) fb = tile + 4;  // vertical through line
   if ((l == 1) && (r == 1) && (t == 0) && (b == 0)) fb = tile + 6;  // horizontal through line

   if ((l == 1) && (r == 0) && (t == 0) && (b == 0)) fb = tile + 6;  // right end line
   if ((l == 0) && (r == 0) && (t == 1) && (b == 0)) fb = tile + 4;  // lower end line
   if ((l == 0) && (r == 1) && (t == 0) && (b == 0)) fb = tile + 6;  // left end line
   if ((l == 0) && (r == 0) && (t == 0) && (b == 1)) fb = tile + 4;  // upper end line

   return fb;
}


// for all solid rect types and purple pipes cross center and wires cross center
int mwWindow::th_replace_helper(int tile, int l, int r, int t, int b, int tl, int tr, int bl, int br)
{
   int fb = tile; //  default
   if ((l == 0) && (r == 1) && (t == 0) && (b == 1)) fb = tile + 8;  // upper left corner
   if ((l == 1) && (r == 0) && (t == 0) && (b == 1)) fb = tile + 9;  // upper right corner
   if ((l == 0) && (r == 1) && (t == 1) && (b == 0)) fb = tile + 10; // lower left corner
   if ((l == 1) && (r == 0) && (t == 1) && (b == 0)) fb = tile + 11; // lower right corner

   if ((l == 0) && (r == 1) && (t == 1) && (b == 1)) fb = tile + 12; // left vertical tee
   if ((l == 1) && (r == 0) && (t == 1) && (b == 1)) fb = tile + 13; // right vertical tee
   if ((l == 1) && (r == 1) && (t == 0) && (b == 1)) fb = tile + 14; // upper horizontal tee
   if ((l == 1) && (r == 1) && (t == 1) && (b == 0)) fb = tile + 15; // lower horizontal tee

   if ((l == 0) && (r == 0) && (t == 1) && (b == 1)) fb = tile + 6;  // vertical through line
   if ((l == 1) && (r == 1) && (t == 0) && (b == 0)) fb = tile + 3;  // horizontal through line

   if ((l == 1) && (r == 0) && (t == 0) && (b == 0)) fb = tile + 4;  // right end line
   if ((l == 0) && (r == 0) && (t == 1) && (b == 0)) fb = tile + 7;  // lower end line
   if ((l == 0) && (r == 1) && (t == 0) && (b == 0)) fb = tile + 2;  // left end line
   if ((l == 0) && (r == 0) && (t == 0) && (b == 1)) fb = tile + 5;  // upper end line


   if ((tr == 0) && (tl == 0) && (bl == 0) && (br == 0)) // no diagonals
      if ((l == 0) && (r == 0) && (t == 0) && (b == 0)) // no blocks on all u d l r
         fb = tile+1;  // orphan single block

   return fb;
}


void mwWindow::th_remove_bound()
{
   int l, r, u, d, ul, ur, dl, dr;

   for (int a=0; a<100; a++)
      for (int b=0; b<100; b++)
         if (mWM.thl[a][b])
         {
            th_find_adj(a, b, l, r, u, d, ul, ur, dl, dr);
            // if block is bound on all 8 sides in selection, remove from level
            if (ur && ul && dl && dr && l && r && u && d) mLevel.l[a][b] = 0;
         }

   // de-select if removed
   for (int a=0; a<100; a++)
      for (int b=0; b<100; b++)
         if (mWM.thl[a][b] && !mLevel.l[a][b]) mWM.thl[a][b] = 0;

   mScreen.init_level_background();
}

void mwWindow::th_find_adj(int x, int y, int &l, int &r, int &t, int &b, int &tl, int &tr, int &bl, int &br )
{
   // find block to left
   l = 0;
   int la = x-1;
   if (la > -1) l = mWM.thl[la][y];

   // find block to right
   r = 0;
   int ra = x+1;
   if (ra < 100) r = mWM.thl[ra][y];

   // find block above
   t = 0;
   int ub = y-1;
   if (ub > -1) t = mWM.thl[x][ub];

   // find block below
   b = 0;
   int lb = y+1;
   if (lb < 100) b = mWM.thl[x][lb];

   // find block to top left
   tl = 0;
   if ((la > -1) && (ub > -1)) tl = mWM.thl[la][ub];

   // find block to top right
   tr = 0;
   if ((ra < 100) && (ub > -1)) tr = mWM.thl[ra][ub];

   // find block to bottom left
   bl = 0;
   if ((la > -1) && (lb < 100)) bl = mWM.thl[la][lb];

   // find block to bottom right
   br = 0;
   if ((ra < 100) && (lb < 100)) br = mWM.thl[ra][lb];
}



void mwWindow::th_replace(int type)
{
   int l, r, t, b, tl, tr, bl, br;

   int tile = 0;
   if (type == 20) tile = mBitmapTools.select_bitmap();

   for (int x=0; x<100; x++)
      for (int y=0; y<100; y++)
         if (mWM.thl[x][y])
         {
            int ot = mLevel.l[x][y]; // original tile

            th_find_adj(x, y, l, r, t, b, tl, tr, bl, br);
            int fb = -1; // default shape
            if (type == 1)  fb = th_replace_helper_extended(256, ot, l, r, t, b, tl, tr, bl, br); // purple pipe set
            if (type == 2)  fb = th_replace_helper_extended(320, ot, l, r, t, b, tl, tr, bl, br); // red pipe set
            if (type == 3)  fb = th_replace_helper_extended(384, ot, l, r, t, b, tl, tr, bl, br); // green pipe set
            if (type == 4)  fb = th_replace_helper_extended(448, ot, l, r, t, b, tl, tr, bl, br); // blue pipe set


            if (type == 8)   fb = th_replace_helper(832, l, r, t, b, tl, tr, bl, br); // wires
            if (type == 9)   fb = th_replace_helper(928, l, r, t, b, tl, tr, bl, br); // grey bricks
            if (type == 10)  fb = th_replace_helper(960, l, r, t, b, tl, tr, bl, br); // brown bricks
            if (type == 11)  fb = th_replace_helper(864, l, r, t, b, tl, tr, bl, br); // brown and yellow thatch
            if (type == 12)  fb = th_replace_helper(896, l, r, t, b, tl, tr, bl, br); // white brain


            if (type == 15)  fb = th_replace_helper(992, l, r, t, b, tl, tr, bl, br); // blue solid

            if (type == 16)  fb = th_replace_helper(544, l, r, t, b, tl, tr, bl, br); // new bricks



            if (type == 17) fb = th_replace_helper3(672, l, r, t, b, tl, tr, bl, br); // rainbow
            if (type == 18) fb = th_replace_helper3(704, l, r, t, b, tl, tr, bl, br); // rainbow 2

            if (type == 20) fb = tile; // single block

            if (fb == -1)
            {
               // do nothing
            }
            if (fb == 0)
            {
               mLevel.l[x][y] = 0;
            }

            if (fb > 0)
            {
               // change only tile portion (lower 10 bits)
               mLevel.l[x][y] &= 0b11111111111111111111110000000000; // clear lower bits
               mLevel.l[x][y] |= fb; // merge tile number
            }
         }
   mScreen.init_level_background();
   al_set_target_backbuffer(mDisplay.display);
}

int mwWindow::th_draw_buttons(int x3, int x4, int yfb, int d)
{
   int bts = 18;
   x3+=4;
   x4-=4;


   // background, title and frame for this group of buttons
   int c1 = 12; // frame color
   int y2f = yfb + bts*6 + 16; // get y2
   if (mWM.mW[9].th_match == 0) y2f -= (bts+4);
   al_draw_filled_rectangle(x3-1, yfb, x4+1, y2f-1, mColor.pc[c1+208]); // background
   mMiscFnx.titlex("Modify Which Tiles Are Marked", 15, c1, x3-1, x4+1, yfb); // title
   al_draw_rectangle(x3-1, yfb, x4+1, y2f-1, mColor.pc[c1], 1); // frame
   yfb +=16;

   int marked_count = 0;
   for (int a=0; a<100; a++)
      for (int b=0; b<100; b++)
        if (mWM.thl[a][b]) marked_count++;

   int xc = x3 + (x4-x3)/2 + 40; // split between buttons
   char msg[80];
   sprintf(msg, "Number of Marks:%d", marked_count);
   mWidget.buttont(x3+2, yfb, xc-2, bts, 0,0,0,0, 0,8,15,0, 1,0,0,d, msg);

   if (mWidget.buttont(xc+2, yfb, x4-2, bts, 0,0,0,0, 0,14,15,0, 1,0,1,d, "Clear All"))
   {
      for (int a=0; a<100; a++)
         for (int b=0; b<100; b++) mWM.thl[a][b] = 0;
   }

   yfb+=4;

   int tx1 = x3+6;
   sprintf(msg, "Action: ");
   al_draw_text(mFont.pr8, mColor.pc[color], tx1, yfb+5, 0, msg);
   int tx2 = tx1 + strlen(msg)*8; // x pos past text
   sprintf(msg, "Remove Marks");
   if (mWM.mW[9].th_add_del) sprintf(msg, "Add Marks");
   int bf = (strlen(msg)+2)*8; // size button based on text length
   if (mWidget.buttont(tx2, yfb, tx2+bf, bts, 0,0,0,0, 0,9,15,0, 1,0,1,d, msg)) mWM.mW[9].th_add_del = !mWM.mW[9].th_add_del;

   yfb+=4;

   sprintf(msg, "Filter: ");
   al_draw_text(mFont.pr8, mColor.pc[color], tx1, yfb+5, 0, msg);
   tx2 = tx1 + strlen(msg)*8; // x pos past text
   if (mWM.mW[9].th_match == 0) sprintf(msg, "Single Tile");
   if (mWM.mW[9].th_match == 1) sprintf(msg, "All Matching Tiles");
   if (mWM.mW[9].th_match == 2) sprintf(msg, "Connected Matching Tiles");
   bf = (strlen(msg)+2)*8; // size button based on text length
   if (mWidget.buttont(tx2, yfb, tx2+bf, bts, 0,0,0,0, 0,7,15,0, 1,0,1,d, msg)) mWM.mW[9].th_match++;
   if (mWM.mW[9].th_match > 2) mWM.mW[9].th_match = 0;


   if (mWM.mW[9].th_match)
   {
      yfb+=4;
      sprintf(msg, "Match: ");
      al_draw_text(mFont.pr8, mColor.pc[color], tx1, yfb+5, 0, msg);
      tx2 = tx1 + strlen(msg)*8; // x pos past text

      sprintf(msg, "Specific Tile Only");
      if (mWM.mW[9].th_group) sprintf(msg, "All Tiles In Set");
      bf = (strlen(msg)+2)*8; // size button based on text length
      if (mWidget.buttont(tx2, yfb, tx2+bf, bts, 0,0,0,0, 0,7,15,0, 1,0,1,d, msg)) mWM.mW[9].th_group = !mWM.mW[9].th_group;
   }

   yfb+=4;
   if (mWidget.buttont(x3+2, yfb, x4-2, bts, 0,0,0,0, 0,12,15,0, 1,0,1,d, "Delete Bound Tiles in Selection")) th_remove_bound();


   yfb+=bts/2; // spacing between groups

   // background, title and frame for this group of buttons
   c1 = 10; // color
   bts = 24;
   int bsp = 4; // spacing in between buttons

   int num_buttons = 10;

   y2f = yfb + num_buttons * (bts + bsp) + 18;


   al_draw_filled_rectangle(x3-1, yfb, x4+1, y2f-1, mColor.pc[c1+192]); // background color
   mMiscFnx.titlex("Change Marked Tiles To Tileset", 15, c1, x3-1, x4+1, yfb); // title
   al_draw_rectangle(x3-1, yfb, x4+1, y2f-1, mColor.pc[c1], 1); // frame

   yfb+=16;
   x3+=2;
   x4-=2;

   int c = 10 + 64;
   int choice = 0;
   yfb+=bsp;
   if (mWidget.buttontt(x3, yfb, x4, bts, 256,14,0,0, 0,c,15,0, 1,1,1,d, "     Purple Pipes")) choice = 1;
   yfb+=bsp;
   if (mWidget.buttontt(x3, yfb, x4, bts, 320,14,0,0, 0,c,15,0, 1,1,1,d, "     Red Pipes")) choice = 2;
   yfb+=bsp;
   if (mWidget.buttontt(x3, yfb, x4, bts, 384,14,0,0, 0,c,15,0, 1,1,1,d, "     Green Pipes")) choice = 3;
   yfb+=bsp;
   if (mWidget.buttontt(x3, yfb, x4, bts, 448,14,0,0, 0,c,15,0, 1,1,1,d, "     Blue Pipes")) choice = 4;
   yfb+=bsp;
   if (mWidget.buttontt(x3, yfb, x4, bts, 832,14,0,0, 0,c,15,0, 1,1,1,d, "     Wires with Cross Center")) choice = 8;
   yfb+=bsp;
   if (mWidget.buttontt(x3, yfb, x4, bts, 928,14,0,0, 0,c,15,0, 1,1,1,d, "     Grey Bricks")) choice = 9;
   yfb+=bsp;
   if (mWidget.buttontt(x3, yfb, x4, bts, 960,14,0,0, 0,c,15,0, 1,1,1,d, "     Brown Bricks")) choice = 10;
   yfb+=bsp;
   if (mWidget.buttontt(x3, yfb, x4, bts, 864,14,0,0, 0,c,15,0, 1,1,1,d, "     Brown and Yellow Thatch")) choice = 11;
   yfb+=bsp;
   if (mWidget.buttontt(x3, yfb, x4, bts, 896,14,0,0, 0,c,15,0, 1,1,1,d, "     Brain")) choice = 12;
   yfb+=bsp;

   if (mWidget.buttontt(x3, yfb, x4, bts, 992,14,0,0, 0,c,15,0, 1,1,1,d, "     Blue")) choice = 15;
   yfb+=bsp;

   if (mWidget.buttontt(x3, yfb, x4, bts, 544,14,0,0, 0,c,15,0, 1,1,1,d, "     New Bricks")) choice = 16;
   yfb+=bsp;



   if (mWidget.buttontt(x3, yfb, x4, bts, 672,14,0,0, 0,c,15,0, 1,1,1,d, "     Rainbow")) choice = 17;
   yfb+=bsp;
   if (mWidget.buttontt(x3, yfb, x4, bts, 704,14,0,0, 0,c,15,0, 1,1,1,d, "     Rainbow 2")) choice = 18;
   yfb+=bsp;
   if (mWidget.buttontt(x3, yfb, x4, bts,   0,14,0,0, 0,c,15,0, 1,1,1,d, "     Single Tile")) choice = 20;




   if (choice) th_replace(choice);
   return yfb;
}

void mwWindow::th_find_connected(int x, int y, int group)
{
   int rb = mLevel.l[x][y];  // block num to match
   mWM.thl[x][y] = -1;       // mark initial block pos in array

   int times=0, found=0;
   do
   {
      times++;
      found = 0;
      for (int a=0; a<100; a++)
         for (int b=0; b<100; b++)
            if (mWM.thl[a][b] == -1) // iterate already marked
            {
               if ((a > 0) && (mTileSets.compareTile(rb, mLevel.l[a-1][b], group))) // look left
               {
                  if (mWM.thl[a-1][b] != -1) found++; // found unmarked
                  mWM.thl[a-1][b] = -1; // mark it
               }
               if ((b > 0) && (mTileSets.compareTile(rb, mLevel.l[a][b-1], group))) // look up
               {
                  if (mWM.thl[a][b-1] != -1) found++; // found unmarked
                  mWM.thl[a][b-1] = -1; // mark it
               }
               if ((a < 99) && (mTileSets.compareTile(rb, mLevel.l[a+1][b], group))) // look right
               {
                  if (mWM.thl[a+1][b] != -1) found++; // found unmarked
                  mWM.thl[a+1][b] = -1; // mark it
               }
               if ((b < 99) && (mTileSets.compareTile(rb, mLevel.l[a][b+1], group))) // look down
               {
                  if (mWM.thl[a][b+1] != -1) found++; // found unmarked
                  mWM.thl[a][b+1] = -1; // mark it
               }
            }
   } while (found);

   // change all marked from -1 to add_del
   for (int a=0; a<100; a++)
      for (int b=0; b<100; b++)
         if (mWM.thl[a][b] == -1) mWM.thl[a][b] = mWM.mW[9].th_add_del;
}


void mwWindow::th_process_mouse(void)
{
   if (mInput.mouse_b[1][0])
   {
      while (mInput.mouse_b[1][0]) mEventQueue.proc(1);

      // add or del single tile at the specific location
      if (mWM.mW[9].th_match == 0) mWM.thl[mWM.gx][mWM.gy] = mWM.mW[9].th_add_del;

      // add or del only connected tiles that match
      if (mWM.mW[9].th_match == 2) th_find_connected(mWM.gx, mWM.gy, mWM.mW[9].th_group);

      // add or del all tiles that match
      if (mWM.mW[9].th_match == 1)
      {
         for (int x=0; x<100; x++)
            for (int y=0; y<100; y++)
               if (mTileSets.compareTile(mLevel.l[mWM.gx][mWM.gy], mLevel.l[x][y], mWM.mW[9].th_group)) mWM.thl[x][y] = mWM.mW[9].th_add_del;
      }
   }

   if (mInput.mouse_b[2][0])
   {
      while (mInput.mouse_b[2][0]) mEventQueue.proc(1);
      mWM.set_windows(1);
   }
}
