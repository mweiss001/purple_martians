#include "pm.h"
#include "mwTileHelper.h"

#include "mwBitmap.h"
#include "mwBitmapTools.h"
#include "mwColor.h"
#include "mwDisplay.h"
#include "mwEventQueue.h"
#include "mwFont.h"
#include "mwInput.h"
#include "mwLevel.h"
#include "mwLoop.h"
#include "mwMiscFnx.h"
#include "mwScreen.h"
#include "mwTileSets.h"
#include "mwWidget.h"
#include "mwWindowManager.h"



struct frameFill
{
   int startFrame;
   int numFrames;
   tileSet ts;
   int drawMode;
};




mwTileHelper mTileHelper;

mwTileHelper::mwTileHelper() { init(); }

void mwTileHelper::init()
{
   add_del = 1;
   match = 1;
   group = 1;
   mark_overlay = 1;
   pattern_offset_x = 0;
   pattern_offset_y = 0;
   frames_detected = 0;
   fill_sections = 0;

   frame_mode_preview = 1;

   replace_preview = 0;
   replace_mode = 0;

   pattern_preview = 0;


   for (int x=0; x<100; x++)
      for (int y=0; y<100; y++) thl[x][y] = 0; // tile helper



   frameFills.clear();


   struct frameFill f;

   /*
   f.startFrame = 1;
   f.numFrames = 1;
   f.drawMode = 1;
   struct tileSet ts;
   mTileSets.findTileSetContainingIndex(ts, 704);
   f.ts = ts;
   mTileSets.frameFills.push_back(f);


   f.startFrame = 2;
   f.numFrames = 1;
   f.drawMode = 3;
   mTileSets.findTileSetContainingIndex(ts, 704);
   f.ts = ts;
   mTileSets.frameFills.push_back(f);

   fill_sections = 2;
*/

   for (int i=0; i<16; i++)
   {
      f.startFrame = i+1;
      f.numFrames = 1;
      f.drawMode = 0;
      struct tileSet ts;
      mTileSets.findTileSetContainingIndex(ts, 704);
      f.ts = ts;
      frameFills.push_back(f);

   }

   fill_sections = 3;
   frameFills[0].drawMode = 5;
   frameFills[1].drawMode = 4;
   frameFills[2].drawMode = 6;





}


int mwTileHelper::replace_helper_48(int tile)
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


int mwTileHelper::replace_helper_16(int tile, bool inner)
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








// tests if all 4 conditions are met
// if any test fails return false
bool mwTileHelper::test4(int v1, int v2, int v3, int v4, int lv, int t1, int t2, int t3, int t4)
{

   if (t1 == 0 && !(v1 <  lv)) return false;
   if (t1 == 1 && !(v1 >= lv)) return false;

   if (t2 == 0 && !(v2 <  lv)) return false;
   if (t2 == 1 && !(v2 >= lv)) return false;

   if (t3 == 0 && !(v3 <  lv)) return false;
   if (t3 == 1 && !(v3 >= lv)) return false;

   if (t4 == 0 && !(v4 <  lv)) return false;
   if (t4 == 1 && !(v4 >= lv)) return false;

   return true;
}












// finds tiles for frame level 'lv'
// types
// 0 - solid inner
// 1 - outer frame with inner or through edges
// 2 - outer frame with through edges
// 3 - inner frame (reversed 1)

int mwTileHelper::replace_helper_16_frame(struct tileSet ts, int lv, int type)
{
   int fb = -1; //  default
   if (c != lv) return fb; // not the correct frame level
   if (type == 0) return ts.SolidFill;
   int tile = ts.startIndex;


//   if (type == 0) return tile; // solid


   // type 1 - default - outer edges, inner sides
   int octl = 8;
   int octr = 9;
   int ocbl = 10;
   int ocbr = 11;
   int oel =  12;
   int oer =  13;
   int oet =  14;
   int oeb =  15;


   if (type == 2) // outer edges, through sides
   {
      oel =  6;
      oer =  6;
      oet =  3;
      oeb =  3;
   }

   if (type == 3) // inner edges, cross corners
   {
      octl = 0;
      octr = 0;
      ocbl = 0;
      ocbr = 0;
      oel =  13;
      oer =  12;
      oet =  15;
      oeb =  14;
   }

   if (type == 4) // inner edges, reversed corners
   {
      octl = 11;
      octr = 10;
      ocbl = 9;
      ocbr = 8;
      oel =  13;
      oer =  12;
      oet =  15;
      oeb =  14;
   }

   if (type == 5) // inner edges, single standalone corners
   {
      octl = 11;
      octr = 10;
      ocbl = 9;
      ocbr = 8;
      oel =  13;
      oer =  12;
      oet =  15;
      oeb =  14;
   }


   if (test4(l, r, t, b, lv, 0, 1, 0, 1)) fb = tile + octl;  // outer corner tl
   if (test4(l, r, t, b, lv, 1, 0, 0, 1)) fb = tile + octr;  // outer corner tr
   if (test4(l, r, t, b, lv, 0, 1, 1, 0)) fb = tile + ocbl;  // outer corner bl
   if (test4(l, r, t, b, lv, 1, 0, 1, 0)) fb = tile + ocbr;  // outer corner br
   if (test4(l, r, t, b, lv, 0, 1, 1, 1)) fb = tile + oel;  // outer edge l
   if (test4(l, r, t, b, lv, 1, 0, 1, 1)) fb = tile + oer;  // outer edge r
   if (test4(l, r, t, b, lv, 1, 1, 0, 1)) fb = tile + oet;  // outer edge t
   if (test4(l, r, t, b, lv, 1, 1, 1, 0)) fb = tile + oeb;  // outer edge b



   // I don't think I will get a nice solution that looks good for 16s
   if (test4(l, r, t, b, lv, 1, 1, 1, 1)) // blocks on all t b l r
   {
      // normal corners
      // if (test4(tr, tl, bl, br, lv, 1, 1, 1, 0)) fb = tile + ocbr; // inner corner br
      // if (test4(tr, tl, bl, br, lv, 1, 1, 0, 1)) fb = tile + ocbl; // inner corner bl
      // if (test4(tr, tl, bl, br, lv, 0, 1, 1, 1)) fb = tile + octr; // inner corner tr
      // if (test4(tr, tl, bl, br, lv, 1, 0, 1, 1)) fb = tile + octl; // inner corner tl

      // reversed corners
      // if (test4(tr, tl, bl, br, lv, 1, 1, 1, 0)) fb = tile + octl; // inner corner br
      // if (test4(tr, tl, bl, br, lv, 1, 1, 0, 1)) fb = tile + octr; // inner corner bl
      // if (test4(tr, tl, bl, br, lv, 0, 1, 1, 1)) fb = tile + ocbl; // inner corner tr
      // if (test4(tr, tl, bl, br, lv, 1, 0, 1, 1)) fb = tile + ocbr; // inner corner tl

      // // middle for all!!!
      // if (test4(tr, tl, bl, br, lv, 1, 1, 1, 0)) fb = tile; // inner corner br
      // if (test4(tr, tl, bl, br, lv, 1, 1, 0, 1)) fb = tile; // inner corner bl
      // if (test4(tr, tl, bl, br, lv, 0, 1, 1, 1)) fb = tile; // inner corner tr
      // if (test4(tr, tl, bl, br, lv, 1, 0, 1, 1)) fb = tile; // inner corner tl

      // single for all!!!
      if (test4(tr, tl, bl, br, lv, 1, 1, 1, 0)) fb = tile + 1; // inner corner br
      if (test4(tr, tl, bl, br, lv, 1, 1, 0, 1)) fb = tile + 1; // inner corner bl
      if (test4(tr, tl, bl, br, lv, 0, 1, 1, 1)) fb = tile + 1; // inner corner tr
      if (test4(tr, tl, bl, br, lv, 1, 0, 1, 1)) fb = tile + 1; // inner corner tl
   }
   return fb;
}











// finds tiles for frame level 'lv'
// types
// 0 - solid fill
// 1 - outer frame
// 2 - inner frame
// 3 - single (pipe)
// 4 - solid cross
// 5 - outer frame, inner cross
// 6 - inner frame, outer cross


int mwTileHelper::replace_helper_48_frame(struct tileSet ts, int lv, int type)
{
   int fb = -1; //  default

   if (c != lv) return fb; // not the correct frame level

   if (type == 0) return ts.SolidFill;
   if (type == 4) return ts.FrameCross;


   // type 1 - default - outer frame
   int octl = ts.OuterCornerTL;
   int octr = ts.OuterCornerTR;
   int ocbl = ts.OuterCornerBL;
   int ocbr = ts.OuterCornerBR;

   int oel =  ts.OuterEdgeL;
   int oer =  ts.OuterEdgeR;
   int oet =  ts.OuterEdgeT;
   int oeb =  ts.OuterEdgeB;

   int ictl = ts.InnerCornerBR;
   int ictr = ts.InnerCornerBL;
   int icbr = ts.InnerCornerTL;
   int icbl = ts.InnerCornerTR;


   if (type == 2) // inner frame
   {
      octl = ts.InnerCornerTL;
      octr = ts.InnerCornerTR;
      ocbr = ts.InnerCornerBR;
      ocbl = ts.InnerCornerBL;

      oel =  ts.OuterEdgeR;
      oer =  ts.OuterEdgeL;
      oet =  ts.OuterEdgeB;
      oeb =  ts.OuterEdgeT;

      ictl = ts.OuterCornerBR;
      ictr = ts.OuterCornerBL;
      icbl = ts.OuterCornerTR;
      icbr = ts.OuterCornerTL;
   }

   if (type == 3) // single width (pipes)
   {

      octl = ts.FrameCornerTL;
      octr = ts.FrameCornerTR;
      ocbr = ts.FrameCornerBR;
      ocbl = ts.FrameCornerBL;

      oel =  ts.FrameEdgeL;
      oer =  ts.FrameEdgeR;
      oet =  ts.FrameEdgeT;
      oeb =  ts.FrameEdgeB;

      ictl = ts.FrameCornerBR;
      ictr = ts.FrameCornerBL;
      icbr = ts.FrameCornerTL;
      icbl = ts.FrameCornerTR;
   }


   if (type == 5) // outer frame, inner cross
   {
      octl = ts.FrameCornerTL;
      octr = ts.FrameCornerTR;
      ocbr = ts.FrameCornerBR;
      ocbl = ts.FrameCornerBL;

      oel =  ts.FrameEdgeLTee;
      oer =  ts.FrameEdgeRTee;
      oet =  ts.FrameEdgeBTee;
      oeb =  ts.FrameEdgeTTee;

      ictl = ts.FrameCross;
      ictr = ts.FrameCross;
      icbr = ts.FrameCross;
      icbl = ts.FrameCross;
   }

   if (type == 6) // inner frame, outer cross
   {
      octl = ts.FrameCross;
      octr = ts.FrameCross;
      ocbr = ts.FrameCross;
      ocbl = ts.FrameCross;

      oel =  ts.FrameEdgeRTee;
      oer =  ts.FrameEdgeLTee;
      oet =  ts.FrameEdgeTTee;
      oeb =  ts.FrameEdgeBTee;

      ictl = ts.FrameCornerBR;
      ictr = ts.FrameCornerBL;
      icbl = ts.FrameCornerTR;
      icbr = ts.FrameCornerTL;
   }

   if (test4(l, r, t, b, lv, 0, 1, 0, 1)) fb = octl;  // outer corner tl
   if (test4(l, r, t, b, lv, 1, 0, 0, 1)) fb = octr;  // outer corner tr
   if (test4(l, r, t, b, lv, 0, 1, 1, 0)) fb = ocbl;  // outer corner bl
   if (test4(l, r, t, b, lv, 1, 0, 1, 0)) fb = ocbr;  // outer corner br

   if (test4(l, r, t, b, lv, 0, 1, 1, 1)) fb = oel;  // outer edge l
   if (test4(l, r, t, b, lv, 1, 0, 1, 1)) fb = oer;  // outer edge r
   if (test4(l, r, t, b, lv, 1, 1, 0, 1)) fb = oet;  // outer edge t
   if (test4(l, r, t, b, lv, 1, 1, 1, 0)) fb = oeb;  // outer edge b


   if (test4(l, r, t, b, lv, 1, 1, 1, 1)) // blocks on all t b l r
   {
      if (test4(tr, tl, bl, br, lv, 1, 1, 1, 0)) fb = icbr; // inner corner br
      if (test4(tr, tl, bl, br, lv, 1, 1, 0, 1)) fb = icbl; // inner corner bl
      if (test4(tr, tl, bl, br, lv, 0, 1, 1, 1)) fb = ictr; // inner corner tr
      if (test4(tr, tl, bl, br, lv, 1, 0, 1, 1)) fb = ictl; // inner corner tl
   }

   return fb;
}
















































// finds tiles for frame level 'lv'
// types
// 0 - solid
// 1 - outer frame
// 2 - outer frame alt
// 3 - inner frame
// 4 - inner frame alt

int mwTileHelper::replace_helper_24_frame(struct tileSet ts, int lv, int type)
{
   int fb = -1; //  default
   if (c != lv) return fb; // not the correct frame level
   if (type == 0) return ts.SolidFill;
   int tile = ts.startIndex;

   // type 1 - default - outer edge tiles
   int octl = 8;
   int octr = 9;
   int ocbl = 10;
   int ocbr = 11;
   int oel =  12;
   int oer =  13;
   int oet =  14;
   int oeb =  15;
   int icbr = 16;
   int icbl = 17;
   int ictr = 18;
   int ictl = 19;

   if (type == 2) // outer edge alt tiles
   {
      oel =  20;
      oer =  22;
      oet =  21;
      oeb =  23;
   }


   if ((type == 3) || (type == 4)) // inner edge tiles
   {
      octl = 16;
      octr = 17;
      ocbl = 18;
      ocbr = 19;
      oel =  13;
      oer =  12;
      oet =  15;
      oeb =  14;
      icbr = 8;
      icbl = 9;
      ictr = 10;
      ictl = 11;
   }

   if (type == 4) // inner edge alt tiles
   {
      oel =  22;
      oer =  20;
      oet =  23;
      oeb =  21;
   }


   if (test4(l, r, t, b, lv, 0, 1, 0, 1)) fb = tile + octl;  // outer corner tl
   if (test4(l, r, t, b, lv, 1, 0, 0, 1)) fb = tile + octr;  // outer corner tr
   if (test4(l, r, t, b, lv, 0, 1, 1, 0)) fb = tile + ocbl;  // outer corner bl
   if (test4(l, r, t, b, lv, 1, 0, 1, 0)) fb = tile + ocbr;  // outer corner br

   if (test4(l, r, t, b, lv, 0, 1, 1, 1)) fb = tile + oel;  // outer edge l
   if (test4(l, r, t, b, lv, 1, 0, 1, 1)) fb = tile + oer;  // outer edge r
   if (test4(l, r, t, b, lv, 1, 1, 0, 1)) fb = tile + oet;  // outer edge t
   if (test4(l, r, t, b, lv, 1, 1, 1, 0)) fb = tile + oeb;  // outer edge b

   if (test4(l, r, t, b, lv, 1, 1, 1, 1)) // blocks on all t b l r
   {
      if (test4(tr, tl, bl, br, lv, 1, 1, 1, 0)) fb = tile + icbr; // inner corner br
      if (test4(tr, tl, bl, br, lv, 1, 1, 0, 1)) fb = tile + icbl; // inner corner bl
      if (test4(tr, tl, bl, br, lv, 0, 1, 1, 1)) fb = tile + ictr; // inner corner tr
      if (test4(tr, tl, bl, br, lv, 1, 0, 1, 1)) fb = tile + ictl; // inner corner tl

   }
   return fb;
}



void mwTileHelper::draw_frame_fill(struct frameFill f, bool preview)
{
   for (int x=0; x<100; x++)
      for (int y=0; y<100; y++)
         if (thl[x][y] == f.startFrame)
         {
            mTileHelper.find_adjacent(x, y);
            int fb = -1;
            if (f.ts.tileSetType == 16) fb = mTileHelper.replace_helper_16_frame(f.ts, f.startFrame, f.drawMode);
            if (f.ts.tileSetType == 24) fb = mTileHelper.replace_helper_24_frame(f.ts, f.startFrame, f.drawMode);
            if (f.ts.tileSetType == 48) fb = mTileHelper.replace_helper_48_frame(f.ts, f.startFrame, f.drawMode);
            if (fb != -1)  mTileSets.drawTile(x, y, fb & 1023, PM_BTILE_ALL_SOLID, mWM.mW[1].em_draw_tile_mode, preview);
         }

}


void mwTileHelper::draw_frame_fills(bool preview)
{
   // choose the lesser of the 2 sizes
   int size = fill_sections;
   if (size > (int)frameFills.size()) size = (int)frameFills.size();

   for (int i=0; i<size; i++) draw_frame_fill(frameFills[i], preview);

   if (!preview)
   {
      mScreen.init_level_background();
      al_set_target_backbuffer(mDisplay.display);
   }
}



void mwTileHelper::draw_pattern(bool preview)
{
   for (int x=0; x<100; x++)
      for (int y=0; y<100; y++)
         if (thl[x][y])
         {
            int fb = replace_helper_pattern(x, y);
            if (fb != -1) mTileSets.drawTile(x, y, fb & 1023, PM_BTILE_ALL_SOLID, mWM.mW[1].em_draw_tile_mode, preview);
         }

   if (!preview)
   {
      mScreen.init_level_background();
      al_set_target_backbuffer(mDisplay.display);
   }
}





void mwTileHelper::draw_replace(bool preview)
{
   if (replace_mode) // single tile
   {
      for (int x=0; x<100; x++)
         for (int y=0; y<100; y++)
            if (thl[x][y])
            {
               int tile = mWM.mW[1].draw_item_num;
               int tileNum = tile & 1023;
               int tileFlags = tile & PM_BTILE_ALL_FLAGS;

               mTileSets.drawTile(x, y, tileNum, tileFlags, mWM.mW[1].em_draw_tile_mode, preview);
            }
   }
   else // tile set
   {
      int index = mTileSets.currentTileSet.startIndex;
      int type = mTileSets.currentTileSet.tileSetType;

      if (index == 0) return;

      for (int x=0; x<100; x++)
         for (int y=0; y<100; y++)
            if (thl[x][y])
            {
               find_adjacent(x, y);
               int fb = -1;
               if (type == 48) fb = replace_helper_48(index);
               if (type == 24) fb = replace_helper_16(index, 1);
               if (type == 16) fb = replace_helper_16(index, 0);
               if (type == 8 ) fb = replace_helper_8(index);
               if (fb != -1) mTileSets.drawTile(x, y, fb & 1023, PM_BTILE_ALL_SOLID, mWM.mW[1].em_draw_tile_mode, preview);

            }
   }

   if (!preview)
   {
      mScreen.init_level_background();
      al_set_target_backbuffer(mDisplay.display);
   }
}




int mwTileHelper::replace_helper_pattern(int x, int y)
{
   // pattern sizes
   int xs = (mWM.bx2 - mWM.bx1) + 1;
   int ys = (mWM.by2 - mWM.by1) + 1;

   // pattern offsets
   int xo = pattern_offset_x;
   int yo = pattern_offset_y;

   // offset into pattern
   int xi = (x+xo) % xs;
   int yi = (y+yo) % ys;

   return mLevel.l[mWM.bx1+xi][mWM.by1+yi];
}


// for rainbows only
int mwTileHelper::replace_helper_8(int tile)
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




void mwTileHelper::remove_bound()
{
   for (int a=0; a<100; a++)
      for (int b=0; b<100; b++)
         if (thl[a][b])
         {
            find_adjacent(a, b);
            // if block is bound on all 8 sides in selection, remove from level
            if (l && r && t && b && tl && tr && bl && br) mLevel.l[a][b] = 0;


         }

   // de-select if removed
   for (int a=0; a<100; a++)
      for (int b=0; b<100; b++)
         if (thl[a][b] && !mLevel.l[a][b]) thl[a][b] = 0;

   mScreen.init_level_background();
}




// increments value if bound
void mwTileHelper::find_bound(int max_lev)
{
   mark_overlay = 2;

   int lev = 1;
   int quit = 0;
   while (!quit)
   {
      int set = 0;

      for (int a=0; a<100; a++)
         for (int b=0; b<100; b++)
            if (thl[a][b] == lev)
            {
               find_adjacent(a, b);

               // if block is bound on all 8 sides by block of same or greater lev, increment
               if (l>=lev && r>=lev && t>=lev && b>=lev && tl>=lev && tr>=lev && bl>=lev && br>=lev)
               {
                  thl[a][b]++;
                  set++;
               }
            }

      if (set == 0) quit = 1;
      if (++lev > max_lev) quit = 1;
   }
   frames_detected = lev;
}





void mwTileHelper::find_adjacent(int x, int y)
{
   // center block
   c = thl[x][y];

   // find block to left
   l = 0;
   int la = x-1;
   if (la > -1) l = thl[la][y];

   // find block to right
   r = 0;
   int ra = x+1;
   if (ra < 100) r = thl[ra][y];

   // find block above
   t = 0;
   int ub = y-1;
   if (ub > -1) t = thl[x][ub];

   // find block below
   b = 0;
   int lb = y+1;
   if (lb < 100) b = thl[x][lb];

   // find block to top left
   tl = 0;
   if ((la > -1) && (ub > -1)) tl = thl[la][ub];

   // find block to top right
   tr = 0;
   if ((ra < 100) && (ub > -1)) tr = thl[ra][ub];

   // find block to bottom left
   bl = 0;
   if ((la > -1) && (lb < 100)) bl = thl[la][lb];

   // find block to bottom right
   br = 0;
   if ((ra < 100) && (lb < 100)) br = thl[ra][lb];
}




void tileSetWidget(int x, int y, int tile, int d)
{
   struct tileSet ts;
   mTileSets.findTileSetContainingIndex(ts, tile);
   if (mWidget.mButtonTile2(x, y, 22, ts.displayIndex, ts.name.c_str(), d)) mTileSets.currentTileSet = ts;
}

int mwTileHelper::show_tileset_controls(int x1, int x2, int y1, int color, int d)
{
   int bts = 16;
   //bts += mLoop.pct_y;

   int height = 88 + bts;

   int y2 = y1 + height;

   al_draw_filled_rectangle(x1, y1, x2, y2, mColor.pc[color+208]); // background color
   mMiscFnx.titlex("Tile Sets", 15, color, x1, x2, y1); // title
   al_draw_rectangle(x1, y1, x2, y2, mColor.pc[color], 1); // frame

   int yfb = y1 + 15;
   x1+=6;
   int xt = 0;
   int xts=23;

   tileSetWidget(x1+xt++*xts, yfb, 256, d); // purple pipes
   tileSetWidget(x1+xt++*xts, yfb, 320, d); // red pipes
   tileSetWidget(x1+xt++*xts, yfb, 384, d); // green pipes
   tileSetWidget(x1+xt++*xts, yfb, 448, d); // blue pipes
   tileSetWidget(x1+xt++*xts, yfb, 192, d); // purple bricks
   xt++;
   tileSetWidget(x1+xt++*xts, yfb, 672, d); // template
   tileSetWidget(x1+xt++*xts, yfb, 704, d); // ice machine
   tileSetWidget(x1+xt++*xts, yfb, 736, d); // tan platform
   tileSetWidget(x1+xt++*xts, yfb, 768, d); // alien machine
   tileSetWidget(x1+xt++*xts, yfb, 800, d); // alien dark
   xt++;
   tileSetWidget(x1+xt++*xts, yfb, 504, d); // rainbow 1
   yfb+=23;
   xt=0;

   tileSetWidget(x1+xt++*xts, yfb, 896, d); // screen
   tileSetWidget(x1+xt++*xts, yfb, 928, d); // wrought
   tileSetWidget(x1+xt++*xts, yfb, 832, d); // wires
   tileSetWidget(x1+xt++*xts, yfb, 880, d); // white bricks
   tileSetWidget(x1+xt++*xts, yfb, 960, d); // brown brick
   tileSetWidget(x1+xt++*xts, yfb, 992, d); // fuzzy pipe
   tileSetWidget(x1+xt++*xts, yfb, 912, d); // dirt
   tileSetWidget(x1+xt++*xts, yfb, 864, d); // red dirt grass
   tileSetWidget(x1+xt++*xts, yfb, 944, d); // red dirt
   tileSetWidget(x1+xt++*xts, yfb, 976, d); // white rock
   tileSetWidget(x1+xt++*xts, yfb, 1008, d); // purple blocks
   xt++;
   tileSetWidget(x1+xt++*xts, yfb, 496, d); // rainbow 2
   yfb+=26;

   char msg[256];
   sprintf(msg, " Current Tile Set: %s ", mTileSets.currentTileSet.name.c_str());
   mWidget.mButton(5, (x1+x2)/2-3, -1,    1, yfb, bts,    2,2,1,1,   color,color,15,0,  0,msg);

   yfb+=(bts+2);

   int nt = 14; // max num tiles
   int t1 = mTileSets.currentTileSet.startIndex;
   int t2 = mTileSets.currentTileSet.endIndex;
   if ((t2-t1) < nt) nt = t2-t1; // if actual number of tiles is less, use that
   for (int i=0; i<nt; i++) al_draw_bitmap(mBitmap.btile[t1+i], x1+i*21+2, yfb, 0);

   return height;

}



int mwTileHelper::show_selection_controls(int x1, int x2, int y1, int color, int d)
{
   int bts = 18; // button height
   int bsp = 1; // button spacing
//   bts += mLoop.pct_y;
//   bsp += mLoop.pct_x;

   int num_lines = 3;
   if (match == 0) num_lines--;


   int height = 35 + (num_lines * bts) + (num_lines-1) * bsp;

   //height += mLoop.pct_y;

   int y2 = y1 + height;


   al_draw_filled_rectangle(x1, y1, x2, y2, mColor.pc[color+208]); // background color
   mMiscFnx.titlex("Marks", 15, color, x1, x2, y1); // title
   al_draw_rectangle(x1, y1, x2, y2, mColor.pc[color], 1); // frame
   int yfb = y1 + 15;


   // inner control margins
   int x3 = x1+3;
   int x4 = x2-3;


   int marked_count = 0;
   for (int a=0; a<100; a++)
      for (int b=0; b<100; b++)
        if (thl[a][b]) marked_count++;

   int xc = x3 + (x4-x3)/2 + 40; // split between buttons
   char msg[80];
   sprintf(msg, "Number of Marks:%d", marked_count);
   mWidget.buttont(x3+2, yfb, xc-2, bts, 0,0,0,0, 0,8,15,0, 1,0,0,d, msg);

   if (mWidget.buttont(xc+2, yfb, x4-2, bts, 0,0,0,0, 0,14,15,0, 1,0,1,d, "Clear All"))
   {
      for (int a=0; a<100; a++)
         for (int b=0; b<100; b++) thl[a][b] = 0;
   }

   yfb+=bsp;

   int tx1 = x3+6;
   sprintf(msg, "Action: ");
   al_draw_text(mFont.pr8, mColor.pc[15], tx1, yfb+5, 0, msg);
   int tx2 = tx1 + strlen(msg)*8; // x pos past text

   sprintf(msg, "Remove Marks");
   if (add_del) sprintf(msg, "Add Marks");
   int bf = (strlen(msg)+2)*8; // size button based on text length
   if (mWidget.buttont(tx2, yfb, tx2+bf, bts, 0,0,0,0, 0,9,15,0, 1,0,0,d, msg)) add_del = !add_del;

   if (mark_overlay == 0) sprintf(msg, "Hide Marks");
   if (mark_overlay == 1) sprintf(msg, "Show Marks");
   if (mark_overlay == 2) sprintf(msg, "Mark Level");

   bf = (strlen(msg)+2)*8; // size button based on text length
   if (mWidget.buttont(x4-bf-2, yfb, x4-2, bts, 0,0,0,0, 0,9,15,0, 1,0,1,d, msg)) mark_overlay++;
   if (mark_overlay > 2) mark_overlay = 0;

   yfb+=bsp;

   sprintf(msg, "Filter: ");
   al_draw_text(mFont.pr8, mColor.pc[15], tx1, yfb+5, 0, msg);
   tx2 = tx1 + strlen(msg)*8; // x pos past text
   if (match == 0) sprintf(msg, "Single Tile");
   if (match == 1) sprintf(msg, "All Matching Tiles");
   if (match == 2) sprintf(msg, "Connected Matching Tiles");
   bf = (strlen(msg)+2)*8; // size button based on text length
   if (mWidget.buttont(tx2, yfb, tx2+bf, bts, 0,0,0,0, 0,7,15,0, 1,0,1,d, msg)) match++;
   if (match > 2) match = 0;

   if (match)
   {
      yfb+=bsp;
      sprintf(msg, "Match: ");
      al_draw_text(mFont.pr8, mColor.pc[15], tx1, yfb+5, 0, msg);
      tx2 = tx1 + strlen(msg)*8; // x pos past text

      sprintf(msg, "Specific Tile Only");
      if (group) sprintf(msg, "All Tiles In Set");
      bf = (strlen(msg)+2)*8; // size button based on text length
      if (mWidget.buttont(tx2, yfb, tx2+bf, bts, 0,0,0,0, 0,7,15,0, 1,0,1,d, msg)) group = !group;
   }
   return height;
}



int mwTileHelper::show_replace_controls(int x1, int x2, int y1, int color, int d)
{
   int bts = 16; // button height
   int bsp = 1; // button spacing

   int height = 48;
   //height += mLoop.pct_y;
   if (replace_mode == 2) height+=162;
   int y2 = y1 + height;

   al_draw_filled_rectangle(x1, y1, x2, y2, mColor.pc[color+208]); // background color
   mMiscFnx.titlex("Replace", 15, color, x1, x2, y1); // title
   al_draw_rectangle(x1, y1, x2, y2, mColor.pc[color], 1); // frame
   int yfb = y1 + 15;

   // inner control margins
   int x3 = x1+3;
   int x4 = x2-3;

   yfb+=bsp;

   char msg[80];
   sprintf(msg, "Use Current Tileset");
   if (replace_mode == 1) sprintf(msg, "Use Specific Tile");
   if (replace_mode == 2) sprintf(msg, "Change Flags Only");
   if (mWidget.mButton(1, x3, 170,    1, yfb+2, bts,  2,2,1,1,  color, color,  15,   0,0, msg)) replace_mode++;
   if (replace_mode > 2) replace_mode = 0;


   int tx = x3+176;

   if (replace_mode == 0) mWidget.mButtonTile2(tx, yfb, 22, mTileSets.currentTileSet.displayIndex, mTileSets.currentTileSet.name.c_str(), d);
   if (replace_mode == 1) // single tile
   {
      int tile = mWM.mW[1].draw_item_num;
      mWM.mW[1].em_get_text_description_of_block_based_on_flags(tile, msg);
      if (mWidget.mButtonTile2(tx, yfb, 22, tile & 1023, msg, d)) mBitmapTools.select_bitmap_from_level(mWM.mW[1].draw_item_num);
   }
   if (replace_mode == 2) mBitmapTools.draw_flag_rects_multiple_th(x3+2, yfb+(bts+bsp) + 6); // flags only

   if (replace_mode != 2) // hide these controls in flag edit mode, they are immediate
   {
      if (mWidget.mButton(4, -1, x4,    1, yfb, bts,  2,2,1,1,  10, 10,   15,  0,0, "Commit")) draw_replace(0);
      yfb+=bts;
      yfb+=bsp;
      mWidget.mCheckBox(4, -1, x4-8,  1, yfb,bts, -1, replace_preview, "preview", 15, 15);
   }

   return height;

}



int mwTileHelper::show_pattern_controls(int x1, int x2, int y1, int color, int d)
{
   int bts = 18; // button height
   int bsp = 3; // button spacing

   // pattern vars
   int bx1 = mWM.bx1;
   int by1 = mWM.by1;
   int bx2 = mWM.bx2;
   int by2 = mWM.by2;
   int bw = bx2 - bx1;
   int bh = by2 - by1;

   int height = 83 + bh*20;
   //height += mLoop.pct_y;

   int y2 = y1 + height;

   al_draw_filled_rectangle(x1, y1, x2, y2, mColor.pc[color+208]); // background color
   mMiscFnx.titlex("Patterns", 15, color, x1, x2, y1); // title
   al_draw_rectangle(x1, y1, x2, y2, mColor.pc[color], 1); // frame
   int yfb = y1 + 15;


   // inner control margins
   int x3 = x1+3;
   int x4 = x2-3;
   int xc = (x1+x2)/2;


   int c1 = color + 64;
   if (mWidget.mButton(0, x3, xc+20,    1, yfb, bts,  2,2,1,1,  c1, c1,  15,   0,0, "Select Pattern"))
   {
      int j1, j2, j3, j4;
      mMiscFnx.get_block_range("Select Pattern", &j1, &j2, &j3, &j4, 1);
   }

   if (mWidget.mButton(4, -1, x4,    1, yfb, bts,  2,2,1,1,  10, 10,   15,  0,0, "Commit")) draw_pattern(0);
   yfb+=(bts+bsp);

   mWidget.mCheckBox(4, -1, x4-8,  1, yfb,bts, -1, pattern_preview, "preview", 15, 15);

   mWidget.mSliderInt(1, x3,     110,    1, yfb, bts,   2, 2,  1, 1,  c1,  c1,  15,  15, 15,   0,  0, pattern_offset_x, 8, 0, 1, "Offset X:" );
   mWidget.mSliderInt(1, x3+120, 110,    1, yfb, bts,   2, 2,  1, 1,  c1,  c1,  15,  15, 15,   0,  0, pattern_offset_y, 8, 0, 1, "Offset Y:" );

   yfb+=(bts+bsp);


   // show pattern
   int sx1 = x3+1;
   int sy1 = yfb+2;

   int sw = bw*20+20;
   int sh = bh*20+20;
   int sx2 = sx1 + sw;
   int sy2 = sy1 + sh;

   al_draw_filled_rectangle(sx1, sy1, sx2, sy2, mColor.Black);
   al_draw_rectangle(sx1-1, sy1-1, sx2+1, sy2+1, mColor.White, 1);
   al_draw_bitmap_region(mBitmap.level_background, bx1*20, by1*20, bw*20+20, bh*20+20, sx1, sy1, 0);

   return height;
}




int mwTileHelper::show_frame_controls(int x1, int x2, int y1, int color, int d)
{
   int bts = 16; // button height
   int bsp = 4; // button spacing
   //   bts += mLoop.pct_y;
   //      bsp += mLoop.pct_x;


   int height = 200;
   //height += mLoop.pct_y;


   int y2 = y1 + height;

   al_draw_filled_rectangle(x1, y1, x2, y2, mColor.pc[color+208]); // background color
   mMiscFnx.titlex("Framesy!", 15, color, x1, x2, y1); // title
   al_draw_rectangle(x1, y1, x2, y2, mColor.pc[color], 1); // frame
   int yfb = y1 + 15;


   // inner control margins
   int x3 = x1+3;
   int x4 = x2-3;
   int xc = (x1+x2)/2;

   int c1 = color + 64;


   // int x6 = x3+6;
   // int x7 = x4-6;
   // int x6a = (x6+x7)/2;

   // yfb+=bsp;
   // c=11+64;

   char msg[80];

   sprintf(msg, "Frames Detected :%d", frames_detected);
   // mWidget.mButton(4, 0, x7, 0,      yfb, bts,   r, backgroundType, frameType, textType, bcol, fcol, tcol, hcol, highlight, txt)
       mWidget.mButton(5, xc, -1,    1, yfb, bts, 2, 2,              1,          1,       c,      c,   15,  0,    0,        msg);


   if (mWidget.mButton(3, x3, -1, 1, yfb, bts, 2, 2,              1,          1,       c,      c,   15,  0,    0,        "Detect")) find_bound(20);


   if (mWidget.mButton(4, -1, x4,    1, yfb, bts, 2, 2,              1,          1,       10,    10,   15,  0,    0,        "Commit")) draw_frame_fills(0);



   yfb+=bts;
   yfb+=bsp;
   c = c1 + 64;

   mWidget.mSliderInt(1, x3, 140,    1, yfb, bts,   2, 2,  1, 1,  c,  c,  15,  15, 15,   0,      1, fill_sections, 8, 0, 1, "Fill Sections:" );
   mWidget.mCheckBox(4, -1, x4-8,  1, yfb,bts, -1, frame_mode_preview, "preview", 15, 15);

//   if (mWidget.mButton(1, x6+150, 30,    1, yfb, bts, 2, 2,              1,          1,       10,    10,   15,  0,    0,        "a ts"))
   {
      for (int i=0; i<16; i++)
         frameFills[i].ts = mTileSets.currentTileSet;


   }

   yfb+=bts;
   yfb+=12;

   // choose the lesser of the 2 sizes
   size_t size = fill_sections;
   if (size > frameFills.size()) size = frameFills.size();
   for (size_t i = 0; i<size; i++)   // draw_frame_fill(mTileSets.frameFills[i], preview);
   {
      int y2f = yfb + 50;

      y2f += mLoop.pct_y;

      al_draw_filled_rectangle(x3, yfb, x4, y2f, mColor.pc[0]);
      al_draw_rectangle       (x3, yfb, x4, y2f, mColor.pc[15], 1);

      al_draw_textf(mFont.pr8, mColor.pc[15], x3, yfb+4, 0, "Frame Fill %d", (int)i+1);

      yfb+=bts;

      mWidget.slideri( x3,   yfb, xc-6, bts, 0,0,0,0,   0,c,15,15, 1,0,0,d, frameFills[i].startFrame, 8, 0, 1, "frame :" );
      mWidget.slideri( xc+6, yfb, x4,   bts, 0,0,0,0,   0,c,15,15, 1,0,1,d, frameFills[i].drawMode  , 8, 0, 1, "mode :" );



      yfb+=bts;

   }




































   return height;
}



















































int mwTileHelper::draw_buttons(int x1, int x2, int y, int d)
{
   y -=6; // base offset
   //y += mLoop.pct_y;

   int ss = 2; // section spacing
   int ms = 1; // margin spacing

   // margins
   y+=  ms;
   x1+= ms;
   x2-= ms;

   y += ss + show_selection_controls(x1, x2, y, 12, d);
//   y += ss + show_replace_controls(  x1, x2, y,  9, d);
   y += ss + show_tileset_controls(  x1, x2, y,  8, d);
//   y += ss + show_pattern_controls(  x1, x2, y,  6, d);
   y += ss + show_frame_controls(    x1, x2, y, 14, d);
   y -= ss; // remove last section spacing

   y += ms; // add margin spacing
   y-=3;    // height offset
   return y;
}

void mwTileHelper::find_connected(int x, int y, int group)
{
   int rb = mLevel.l[x][y];  // block num to match
   thl[x][y] = -1;       // mark initial block pos in array

   int times=0, found=0;
   do
   {
      times++;
      found = 0;
      for (int a=0; a<100; a++)
         for (int b=0; b<100; b++)
            if (thl[a][b] == -1) // iterate already marked
            {
               if ((a > 0) && (mTileSets.compareTile(rb, mLevel.l[a-1][b], group))) // look left
               {
                  if (thl[a-1][b] != -1) found++; // found unmarked
                  thl[a-1][b] = -1; // mark it
               }
               if ((b > 0) && (mTileSets.compareTile(rb, mLevel.l[a][b-1], group))) // look up
               {
                  if (thl[a][b-1] != -1) found++; // found unmarked
                  thl[a][b-1] = -1; // mark it
               }
               if ((a < 99) && (mTileSets.compareTile(rb, mLevel.l[a+1][b], group))) // look right
               {
                  if (thl[a+1][b] != -1) found++; // found unmarked
                  thl[a+1][b] = -1; // mark it
               }
               if ((b < 99) && (mTileSets.compareTile(rb, mLevel.l[a][b+1], group))) // look down
               {
                  if (thl[a][b+1] != -1) found++; // found unmarked
                  thl[a][b+1] = -1; // mark it
               }
            }
   } while (found);

   // change all marked from -1 to add_del
   for (int a=0; a<100; a++)
      for (int b=0; b<100; b++)
         if (thl[a][b] == -1) thl[a][b] = add_del;
}


void mwTileHelper::process_mouse(void)
{
   if (mInput.mouse_b[1][0])
   {
      while (mInput.mouse_b[1][0]) mEventQueue.proc(1);

      // add or del single tile at the specific location
      if (match == 0) thl[mWM.gx][mWM.gy] = add_del;

      // add or del only connected tiles that match
      if (match == 2) find_connected(mWM.gx, mWM.gy, group);

      // add or del all tiles that match
      if (match == 1)
      {
         for (int x=0; x<100; x++)
            for (int y=0; y<100; y++)
               if (mTileSets.compareTile(mLevel.l[mWM.gx][mWM.gy], mLevel.l[x][y], group)) thl[x][y] = add_del;
      }
   }

   if (mInput.mouse_b[2][0])
   {
      while (mInput.mouse_b[2][0]) mEventQueue.proc(1);
      mWM.set_windows(1);
   }
}



