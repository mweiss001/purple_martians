// mwWindowTileHelper.cpp


#include "pm.h"
#include "mwBitmap.h"
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
#include "mwLoop.h"


int mwWindow::th_replace_helper_48(int tile, int l, int r, int t, int b, int tl, int tr, int bl, int br)
{
   int fb = -1;

   struct tileSet s;
   if (mTileSets.findTileSetContainingIndex(s, tile))
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


int mwWindow::th_replace_helper_16(int tile, int l, int r, int t, int b, int tl, int tr, int bl, int br, bool inner)
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

   if (inner) // for 24 sets only (not much difference, gets a few blocks)
   {
      if ((l == 1) && (r == 1) && (t == 1) && (b == 1)) // blocks on all u d l r
      {
         // single corner open
         if ((tr == 1) && (tl == 1) && (bl == 1) && (br == 0)) fb = tile + 16; // only br open
         if ((tr == 1) && (tl == 1) && (bl == 0) && (br == 1)) fb = tile + 17; // only bl open
         if ((tr == 0) && (tl == 1) && (bl == 1) && (br == 1)) fb = tile + 18; // only tr open
         if ((tr == 1) && (tl == 0) && (bl == 1) && (br == 1)) fb = tile + 19; // only tl open

         // 2 adjacent corners open
         if ((tr == 1) && (tl == 0) && (bl == 0) && (br == 1)) fb = tile + 20; // tl and bl open
         if ((tr == 0) && (tl == 0) && (bl == 1) && (br == 1)) fb = tile + 21; // tr and tl open
         if ((tr == 0) && (tl == 1) && (bl == 1) && (br == 0)) fb = tile + 22; // tr and br open
         if ((tr == 1) && (tl == 1) && (bl == 0) && (br == 0)) fb = tile + 23; // br and bl open

      }
   }

   return fb;
}



int mwWindow::th_replace_helper_pattern(int x, int y)
{
   int bx1 = mWM.bx1;
   int by1 = mWM.by1;
   int bx2 = mWM.bx2;
   int by2 = mWM.by2;
   mMiscFnx.ensure_xy1_less_than_xy2(bx1, by1, bx2, by2); // swap if wrong order

   // pattern sizes
   int xs = (bx2 - bx1) + 1;
   int ys = (by2 - by1) + 1;

   // pattern offsets
   int xo = th_pattern_offset_x;
   int yo = th_pattern_offset_y;

   // offset into pattern
   int xi = (x+xo) % xs;
   int yi = (y+yo) % ys;

   return mLevel.l[bx1+xi][by1+yi];
}






// for rainbows only
int mwWindow::th_replace_helper_8(int tile, int l, int r, int t, int b, int tl, int tr, int bl, int br)
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
   if (type == 40) tile = mBitmapTools.select_bitmap();

   for (int x=0; x<100; x++)
      for (int y=0; y<100; y++)
         if (mWM.thl[x][y])
         {
//            int ot = mLevel.l[x][y]; // original tile

            th_find_adj(x, y, l, r, t, b, tl, tr, bl, br);
            int fb = -1; // default shape
            if (type == 1)  fb = th_replace_helper_48(256, l, r, t, b, tl, tr, bl, br); // purple pipe set
            if (type == 2)  fb = th_replace_helper_48(320, l, r, t, b, tl, tr, bl, br); // red pipe set
            if (type == 3)  fb = th_replace_helper_48(384, l, r, t, b, tl, tr, bl, br); // green pipe set
            if (type == 4)  fb = th_replace_helper_48(448, l, r, t, b, tl, tr, bl, br); // blue pipe set
            if (type == 5)  fb = th_replace_helper_48(192, l, r, t, b, tl, tr, bl, br); // purple bricks

            if (type == 7)   fb = th_replace_helper_16(896, l, r, t, b, tl, tr, bl, br, 0); // semi solid screen
            if (type == 8)   fb = th_replace_helper_16(928, l, r, t, b, tl, tr, bl, br, 0); // wrought iron
            if (type == 9)   fb = th_replace_helper_16(832, l, r, t, b, tl, tr, bl, br, 0); // wires
            if (type == 10)  fb = th_replace_helper_16(880, l, r, t, b, tl, tr, bl, br, 0); // white bricks

            if (type == 13)  fb = th_replace_helper_16(960, l, r, t, b, tl, tr, bl, br, 0); // brown bricks
            if (type == 14)  fb = th_replace_helper_16(992, l, r, t, b, tl, tr, bl, br, 0); // fat fuzzy pipe

            if (type == 15)  fb = th_replace_helper_16(912, l, r, t, b, tl, tr, bl, br, 0); // brown dirt and grass
            if (type == 16)  fb = th_replace_helper_16(864, l, r, t, b, tl, tr, bl, br, 0); // red dirt and grass
            if (type == 17)  fb = th_replace_helper_16(944, l, r, t, b, tl, tr, bl, br, 0); // red dirt
            if (type == 18)  fb = th_replace_helper_16(976, l, r, t, b, tl, tr, bl, br, 0); // white rock
            if (type == 19)  fb = th_replace_helper_16(1008, l, r, t, b, tl, tr, bl, br, 0); // purple rivet blocks


            if (type == 22)  fb = th_replace_helper_16(672, l, r, t, b, tl, tr, bl, br, 1); // Template
            if (type == 23)  fb = th_replace_helper_16(704, l, r, t, b, tl, tr, bl, br, 1); // Ice Machine
            if (type == 24)  fb = th_replace_helper_16(736, l, r, t, b, tl, tr, bl, br, 1); // Tan Platform
            if (type == 25)  fb = th_replace_helper_16(768, l, r, t, b, tl, tr, bl, br, 1); // Alien Machine
            if (type == 26)  fb = th_replace_helper_16(800, l, r, t, b, tl, tr, bl, br, 1); // Alien Dark


            if (type == 30) fb = th_replace_helper_8(504, l, r, t, b, tl, tr, bl, br); // rainbow 1
            if (type == 31) fb = th_replace_helper_8(496, l, r, t, b, tl, tr, bl, br); // rainbow 2


            if (type == 50) fb = th_replace_helper_pattern(x, y); // pattern

            if (type == 40) fb = tile; // single block


            // finally, replace the tile in the level array
            if (tile != -1) // if -1 do nothing
            {
               // draw both tile and all solid flags
               if (mWM.mW[1].em_draw_tile_mode == 1) mLevel.l[x][y] = fb | PM_BTILE_ALL_SOLID;

               // draw tile only, preserve existing flags
               if (mWM.mW[1].em_draw_tile_mode == 2)
               {
                  int tileNum = fb & 1023; // just make sure the flags are cleared
                  mLevel.l[x][y] &= 0b11111111111111111111110000000000; // clear lower 10 bits
                  mLevel.l[x][y] |= tileNum; // merge tileNum
               }
            }
         }
   mScreen.init_level_background();
   al_set_target_backbuffer(mDisplay.display);
}



int mwWindow::th_draw_buttons(int x3, int x4, int yfb, int d)
{
   int bts = 18;

   // margins
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
   if (mWidget.buttont(tx2, yfb, tx2+bf, bts, 0,0,0,0, 0,9,15,0, 1,0,0,d, msg)) mWM.mW[9].th_add_del = !mWM.mW[9].th_add_del;

   sprintf(msg, "Hide Marks");
   if (mWM.mW[9].th_hide_marks) sprintf(msg, "Show Marks");
   bf = (strlen(msg)+2)*8; // size button based on text length
   if (mWidget.buttont(x4-bf-2, yfb, x4-2, bts, 0,0,0,0, 0,9,15,0, 1,0,1,d, msg)) mWM.mW[9].th_hide_marks = !mWM.mW[9].th_hide_marks;



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

   y2f = yfb + 80;
   // y2f += mLoop.pct_y;



   al_draw_filled_rectangle(x3-1, yfb-2, x4+1, y2f-1, mColor.pc[c1+224]); // background color
   mMiscFnx.titlex("Change Marked Tiles To Tileset", 15, c1, x3-1, x4+1, yfb); // title
   al_draw_rectangle(x3-1, yfb, x4+1, y2f-1, mColor.pc[c1], 1); // frame

   yfb+=16;

   // next margin
   int x5 = x3+8;



   int c = 10 + 64;
   int choice = 0;


   yfb+=bsp;

   int xt = 0;
   int xts=23;


   if (mWidget.mButtonTile2(x5+xt++*xts, yfb, 22, 256,  "Purple Pipes"         , d))  choice = 1;
   if (mWidget.mButtonTile2(x5+xt++*xts, yfb, 22, 320,  "Red Pipes"            , d))  choice = 2;
   if (mWidget.mButtonTile2(x5+xt++*xts, yfb, 22, 384,  "Green Pipes"          , d))  choice = 3;
   if (mWidget.mButtonTile2(x5+xt++*xts, yfb, 22, 448,  "Blue Pipes"           , d))  choice = 4;
   if (mWidget.mButtonTile2(x5+xt++*xts, yfb, 22, 192,  "Purple Bricks"        , d))  choice = 5;
   xt++;

   if (mWidget.mButtonTile2(x5+xt++*xts, yfb, 22, 673,  "Template"             , d))  choice = 22;
   if (mWidget.mButtonTile2(x5+xt++*xts, yfb, 22, 713,  "Ice Machine"          , d))  choice = 23;
   if (mWidget.mButtonTile2(x5+xt++*xts, yfb, 22, 737,  "Tan Platform"         , d))  choice = 24;
   if (mWidget.mButtonTile2(x5+xt++*xts, yfb, 22, 776,  "Alien Machine"        , d))  choice = 25;
   if (mWidget.mButtonTile2(x5+xt++*xts, yfb, 22, 809,  "Alien Dark"           , d))  choice = 26;


   xt++;
   if (mWidget.mButtonTile2(x3+xt++*xts, yfb, 22, 504,  "Rainbow 1"            , d))  choice = 30;



   yfb+=30;

   xt = 0;
   if (mWidget.mButtonTile2(x5+xt++*xts, yfb, 22, 897,  "Semi Solid Screen"    , d))  choice = 7;
   if (mWidget.mButtonTile2(x5+xt++*xts, yfb, 22, 928,  "Wrought Iron"         , d))  choice = 8;
   if (mWidget.mButtonTile2(x5+xt++*xts, yfb, 22, 832,  "Wires"                , d))  choice = 9;
   if (mWidget.mButtonTile2(x5+xt++*xts, yfb, 22, 881,  "Slate Bricks"         , d))  choice = 10;

   if (mWidget.mButtonTile2(x5+xt++*xts, yfb, 22, 961,  "Brown Bricks"         , d))  choice = 13;
   if (mWidget.mButtonTile2(x5+xt++*xts, yfb, 22, 993,  "Fat Fuzzy Pipes"      , d))  choice = 14;

   if (mWidget.mButtonTile2(x5+xt++*xts, yfb, 22, 913,  "Brown Dirt and Grass" , d))  choice = 15;
   if (mWidget.mButtonTile2(x5+xt++*xts, yfb, 22, 865,  "Red Dirt and Grass"   , d))  choice = 16;
   if (mWidget.mButtonTile2(x5+xt++*xts, yfb, 22, 945,  "Red Dirt"             , d))  choice = 17;
   if (mWidget.mButtonTile2(x5+xt++*xts, yfb, 22, 977,  "White Rock"           , d))  choice = 18;
   if (mWidget.mButtonTile2(x5+xt++*xts, yfb, 22, 1009, "Rivet Blocks"         , d))  choice = 19;

   xt++;
   if (mWidget.mButtonTile2(x3+xt++*xts, yfb, 22, 496,  "Rainbow 2"            , d))  choice = 31;


   yfb+=30;

   bts = 18;

   yfb+=bsp;
   if (mWidget.buttont(x3, yfb, x4, bts,   0,14,0,0, 0,c,15,0, 1,0,1,d, "Change Marked Tiles To Single Tile")) choice = 40;

   yfb+=bsp;
   //yfb += mLoop.pct_y;
   y2f = yfb + 58;
   //y2f += mLoop.pct_y;


   c1 = 13; // color
   al_draw_filled_rectangle(x3-1, yfb-2, x4+1, y2f-1, mColor.pc[c1+224]); // background color
   mMiscFnx.titlex("Apply Tile Pattern To Marked Tiles", 15, c1, x3-1, x4+1, yfb); // title
   al_draw_rectangle(x3-1, yfb, x4+1, y2f-1, mColor.pc[c1], 1); // frame

   yfb+=12;
   //yfb += mLoop.pct_y;

   int x6 = x3+6;
   int x7 = x4-6;
   int x6a = (x6+x7)/2;


   c = c1 + 64;
   yfb+=bsp;
   if (mWidget.buttont(x6, yfb, x6a-6, bts,   0,14,0,0, 0,c,15,0, 1,0,0,d, "Select Pattern"))
   {
      int j1, j2, j3, j4;
      mMiscFnx.get_block_range("Select Pattern", &j1, &j2, &j3, &j4, 1);
   }
   if (mWidget.buttont(x6a+6, yfb, x7, bts,   0,14,0,0, 0,c,15,0, 1,0,1,d, "Apply Pattern")) choice = 50;

   yfb+=bsp;
   mWidget.slideri( x6, yfb, x6a-6, bts, 0,0,0,0,   0,c,15,15, 1,0,0,d, th_pattern_offset_x, 8, 0, 1, "Offset X:" );
   mWidget.slideri( x6a+6, yfb, x7, bts, 0,0,0,0,   0,c,15,15, 1,0,1,d, th_pattern_offset_y, 8, 0, 1, "Offset Y:" );



   // show pattern

   int bx1 = mWM.bx1;
   int by1 = mWM.by1;
   int bx2 = mWM.bx2;
   int by2 = mWM.by2;
   mMiscFnx.ensure_xy1_less_than_xy2(bx1, by1, bx2, by2); // swap if wrong order
   int bw = bx2 - bx1;
   int bh = by2 - by1;

   int sx1 = x3;
   int sy1 = yfb+5;
   //sy1 += mLoop.pct_y;

   int sw = bw*20+20;
   int sh = bh*20+20;

   int sx2 = sx1 + sw;
   int sy2 = sy1 + sh;

   al_draw_filled_rectangle(sx1, sy1, sx2, sy2, mColor.Black);
   al_draw_rectangle(sx1-1, sy1-1, sx2+1, sy2+1, mColor.White, 1);
   al_draw_bitmap_region(mBitmap.level_background, bx1*20, by1*20, bw*20+20, bh*20+20, sx1, sy1, 0);


   yfb+=sh;
   yfb+=5;
   //yfb += mLoop.pct_y;

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
