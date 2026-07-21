#include "pm.h"
#include "mwTileHelper.h"

#include "mwBitmap.h"
#include "mwBitmapTools.h"
#include "mwColor.h"
#include "mwDisplay.h"
#include "mwEditorMain.h"
#include "mwEventQueue.h"
#include "mwFont.h"
#include "mwHelp.h"
#include "mwInput.h"
#include "mwLevel.h"
#include "mwLevelEditor.h"
#include "mwLoop.h"
#include "mwMenu.h"
#include "mwMiscFnx.h"
#include "mwScreen.h"
#include "mwTileSets.h"
#include "mwWidget.h"
#include "mwWindowManager.h"


struct frameFill
{
   int frameIndex;
   int mode;
   tileSet ts;
};


mwTileHelper mTileHelper;

mwTileHelper::mwTileHelper() { init(); }

void mwTileHelper::init()
{
   add_del = 1;
   match = 1;
   group = 1;
   mark_overlay = 1;

   replace_mode = 0;
   replace_preview = 0;

   pattern_offset_x = 0;
   pattern_offset_y = 0;
   pattern_preview = 0;

   frames_detected = 0;
   frame_sections = 0;
   frame_common_tileset = 1;
   frame_mode_preview = 0;

   // clear all marks
   for (int x=0; x<100; x++)
      for (int y=0; y<100; y++) thl[x][y] = 0;

   clearFrameFills();
}




std::vector<struct listItem> listItemPresets =
{
  {  0, "presets"                     },
  {  1, "24 2 Outer Inner"            },
  {  2, "24 3 Outer Fill Inner"       },

  {  40, "48 2 Outer Inner"           },
  {  41, "48 3 Outer Fill Inner"      },
  {  42, "48 2 Outer Inner Alt"       },
  {  43, "48 3 Outer Fill Inner Alt"  },
  {  44, "48 3 Outer Fill Fill"       },

  {  3, "Purple Bricks"               }

};

void mwTileHelper::clearFrameFills(int preset)
{
   frameFills.clear();


   // create 32
   for (int i=0; i<32; i++)
   {
      struct frameFill f;
      f.frameIndex = i+1;
      f.mode = 0;
      mTileSets.findTileSetContainingIndex(f.ts, 0);
      frameFills.push_back(f);
   }


   if (preset == 40)
   {
      frame_sections = 2;
      frameFills[0].mode = 30;
      frameFills[1].mode = 40;
   }

   if (preset == 41)
   {
      frame_sections = 3;
      frameFills[0].mode = 30;
      frameFills[1].mode = 20;
      frameFills[2].mode = 40;
   }

   if (preset == 44)
   {
      frame_sections = 3;
      frameFills[0].mode = 30;
      frameFills[1].mode = 20;
      frameFills[2].mode = 20;
   }


   if (preset == 42)
   {
      frame_sections = 2;
      frameFills[0].mode = 31;
      frameFills[1].mode = 41;
   }

   if (preset == 43)
   {
      frame_sections = 3;
      frameFills[0].mode = 31;
      frameFills[1].mode = 21;
      frameFills[2].mode = 41;
   }

   if (preset == 0)
   {
      frame_sections = 3;
      frameFills[0].mode = 30;
      frameFills[1].mode = 20;
      frameFills[2].mode = 40;
   }

   if (preset == 1)
   {
      frame_sections = 2;
      frameFills[0].mode = 30;
      frameFills[1].mode = 40;
   }

   if (preset == 2)
   {
      frame_sections = 3;
      frameFills[0].mode = 30;
      frameFills[1].mode = 20;
      frameFills[2].mode = 40;
   }


   if (preset == 3)
   {
      frame_common_tileset = 1;
      mTileSets.findTileSetContainingName(mTileSets.currentTileSet, "Purple Blocks");

      frame_sections = 3;
      frameFills[0].mode = 31;
      frameFills[1].mode = 21;
      frameFills[2].mode = 43;
   }


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





std::vector<struct listItem> listItems16Frame =
{
   {  0, "Do Nothing"  },
   {  1, "Erase (blank)" },
   { 10, "Single Width" },
   { 31, "Outer" },
   { 21, "Fill"  },
   { 41, "Inner" },
   { 42, "Inner Reversed Corners" },
   { 43, "Inner Single Corners" }
};


int mwTileHelper::replace_helper_16_frame(struct tileSet ts, int lv, int type)
{
   if (c != lv) return -1;               // not the correct frame level
   if (type == 0) return -1;             // 'do nothing'
   if (type == 1) return 0;              // 'erase'
   if (type == 21) return ts.startIndex; // 'fill alt'

   int octl = 0, octr = 0, ocbl = 0, ocbr = 0;
   int oel = 0, oer = 0, oet = 0, oeb = 0;


   bool type_match = false;


   if (type == 10) // 'frame'    edges are single through,        corners normal
   {
      type_match = true;
      oel =  6;
      oer =  6;
      oet =  3;
      oeb =  3;
      octl = 8;
      octr = 9;
      ocbl = 10;
      ocbr = 11;
   }

   if (type == 31) // 'outer'    edges with tee facing inwards,   corners normal
   {
      type_match = true;
      oel =  12;
      oer =  13;
      oet =  14;
      oeb =  15;
      octl = 8;
      octr = 9;
      ocbl = 10;
      ocbr = 11;
   }
   if (type == 41) // 'inner'    edges with tee facing inwards,   corners fill (through)
   {
      type_match = true;
      oel =  13;
      oer =  12;
      oet =  15;
      oeb =  14;
      octl = 0;
      octr = 0;
      ocbl = 0;
      ocbr = 0;
   }
   if (type == 42) // 'inner 1'  edges with tee facing inwards,   corners reversed
   {
      type_match = true;
      oel =  13;
      oer =  12;
      oet =  15;
      oeb =  14;
      octl = 11;
      octr = 10;
      ocbl = 9;
      ocbr = 8;
   }

   if (type == 43) // 'inner 2'  edges with tee facing inwards,   corners single block
   {
      type_match = true;
      oel =  13;
      oer =  12;
      oet =  15;
      oeb =  14;
      octl = 1;
      octr = 1;
      ocbl = 1;
      ocbr = 1;
   }

   if (type_match == false) return -1;


   int fb = -1; //  default
   int tile = ts.startIndex;

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



































std::vector<struct listItem> listItems24Frame =
{
   {  0, "Do Nothing"  },
   {  1, "Erase (blank)" },
   { 30, "Outer" },
   { 31, "Outer Alt 1" },
   { 32, "Outer Alt 2" },



   { 20, "Fill"  },
   { 40, "Inner" },
   { 41, "Inner Alt" }
};

int mwTileHelper::replace_helper_24_frame(struct tileSet ts, int lv, int type)
{
   if (c != lv)    return -1;            // not the correct frame level
   if (type == 0)  return -1;            // 'do nothing'
   if (type == 1)  return 0;             // 'erase'
   if (type == 20) return ts.SolidFill;  // 'fill alt'

   int octl = 0, octr = 0, ocbl = 0, ocbr = 0;
   int ictl = 0, ictr = 0, icbl = 0, icbr = 0;
   int oel = 0, oer = 0, oet = 0, oeb = 0;

   bool type_match = false;

   if ((type >= 30) || (type <= 39)) // outer edge
   {
      type_match = true;
      octl = 8;
      octr = 9;
      ocbl = 10;
      ocbr = 11;
      oel =  12;
      oer =  13;
      oet =  14;
      oeb =  15;
      icbr = 16;
      icbl = 17;
      ictr = 18;
      ictl = 19;
      if (type > 30) // outer edge alt 1
      {
         oel =  20;
         oer =  22;
         oet =  21;
         oeb =  23;

         if (type == 32) // outer edge alt 2
         {
            octl = 18;
            octr = 19;
            ocbl = 16;
            ocbr = 17;
            icbr = 16;
            icbl = 17;
            ictr = 18;
            ictl = 19;
         }

      }





   }

   if ((type == 40) || (type == 41)) // inner edge
   {
      type_match = true;
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
      if (type == 41) // inner edge alt tiles
      {
         oel =  22;
         oer =  20;
         oet =  23;
         oeb =  21;
      }
   }


   if (type_match == false) return -1;

   int fb = -1; //  default
   int tile = ts.startIndex;


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


std::vector<struct listItem> listItems48Frame =
{
   {  0, "Do Nothing"  },
   {  1, "Erase (blank)" },
   { 10, "Single"  },
   { 30, "Outer" },
   { 20, "Fill"  },
   { 40, "Inner" },
   { 31, "Outer Alt" },
   { 21, "Fill Alt"  },
   { 41, "Inner Alt" }
};



int mwTileHelper::replace_helper_48_frame(struct tileSet ts, int lv, int type)
{
   if (c != lv)    return -1;            // not the correct frame level
   if (type == 0)  return -1;            // 'do nothing'
   if (type == 1)  return 0;             // 'erase'
   if (type == 20) return ts.SolidFill;
   if (type == 21) return ts.FrameCross;





   int octl = 0, octr = 0, ocbl = 0, ocbr = 0;
   int ictl = 0, ictr = 0, icbl = 0, icbr = 0;
   int oel = 0, oer = 0, oet = 0, oeb = 0;



   bool type_match = false;




   if (type == 10) // single width (pipes)
   {
      type_match = true;
      octl = ts.FrameCornerTL;
      octr = ts.FrameCornerTR;
      ocbr = ts.FrameCornerBR;
      ocbl = ts.FrameCornerBL;

      ictl = ts.FrameCornerBR;
      ictr = ts.FrameCornerBL;
      icbr = ts.FrameCornerTL;
      icbl = ts.FrameCornerTR;

      oel =  ts.FrameEdgeL;
      oer =  ts.FrameEdgeR;
      oet =  ts.FrameEdgeT;
      oeb =  ts.FrameEdgeB;
   }

   if (type == 30) // outer frame, solid inner
   {
      type_match = true;
      octl = ts.OuterCornerTL;
      octr = ts.OuterCornerTR;
      ocbl = ts.OuterCornerBL;
      ocbr = ts.OuterCornerBR;

      ictl = ts.InnerCornerBR;
      ictr = ts.InnerCornerBL;
      icbr = ts.InnerCornerTL;
      icbl = ts.InnerCornerTR;

      oel =  ts.OuterEdgeL;
      oer =  ts.OuterEdgeR;
      oet =  ts.OuterEdgeT;
      oeb =  ts.OuterEdgeB;
   }

   if (type == 31) // outer frame, inner cross
   {
      type_match = true;
      octl = ts.FrameCornerTL;
      octr = ts.FrameCornerTR;
      ocbr = ts.FrameCornerBR;
      ocbl = ts.FrameCornerBL;

      ictl = ts.FrameCross;
      ictr = ts.FrameCross;
      icbr = ts.FrameCross;
      icbl = ts.FrameCross;

      oel =  ts.FrameEdgeLTee;
      oer =  ts.FrameEdgeRTee;
      oet =  ts.FrameEdgeBTee;
      oeb =  ts.FrameEdgeTTee;
   }


   if (type == 40) // inner frame
   {
      type_match = true;
      octl = ts.InnerCornerTL;
      octr = ts.InnerCornerTR;
      ocbr = ts.InnerCornerBR;
      ocbl = ts.InnerCornerBL;

      ictl = ts.FrameCross;
      ictr = ts.FrameCross;
      icbr = ts.FrameCross;
      icbl = ts.FrameCross;

      oel =  ts.OuterEdgeR;
      oer =  ts.OuterEdgeL;
      oet =  ts.OuterEdgeB;
      oeb =  ts.OuterEdgeT;
   }




   if (type == 41) // inner frame, outer cross
   {
      type_match = true;
      octl = ts.FrameCross;
      octr = ts.FrameCross;
      ocbr = ts.FrameCross;
      ocbl = ts.FrameCross;

      ictl = ts.FrameCornerBR;
      ictr = ts.FrameCornerBL;
      icbl = ts.FrameCornerTR;
      icbr = ts.FrameCornerTL;

      oel =  ts.FrameEdgeRTee;
      oer =  ts.FrameEdgeLTee;
      oet =  ts.FrameEdgeTTee;
      oeb =  ts.FrameEdgeBTee;
   }
   if (type_match == false) return -1;

   int fb = -1; //  default

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












void mwTileHelper::draw_frame_fill(struct frameFill f, bool preview)
{
   for (int x=0; x<100; x++)
      for (int y=0; y<100; y++)
         if (thl[x][y] == f.frameIndex)
         {
            mTileHelper.find_adjacent(x, y);
            int fb = -1;
            if (f.ts.tileSetType == 16) fb = mTileHelper.replace_helper_16_frame(f.ts, f.frameIndex, f.mode);
            if (f.ts.tileSetType == 24) fb = mTileHelper.replace_helper_24_frame(f.ts, f.frameIndex, f.mode);
            if (f.ts.tileSetType == 48) fb = mTileHelper.replace_helper_48_frame(f.ts, f.frameIndex, f.mode);
            if (fb != -1)  mTileSets.drawTile(x, y, fb & PM_BTILE_TILENUM_MASK, PM_BTILE_ALL_SOLID, mEditorMain.draw_tile_mode, preview);
         }
}


void mwTileHelper::draw_frame_fills(bool preview)
{
   // choose the lesser of the 2 sizes
   int size = frame_sections;
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
            if (fb != -1) mTileSets.drawTile(x, y, fb & PM_BTILE_TILENUM_MASK, PM_BTILE_ALL_SOLID, mEditorMain.draw_tile_mode, preview);
         }

   if (!preview)
   {
      mScreen.init_level_background();
      al_set_target_backbuffer(mDisplay.display);
   }
}





void mwTileHelper::draw_replace(bool preview)
{
   if (replace_mode == 1) // single tile
   {
      for (int x=0; x<100; x++)
         for (int y=0; y<100; y++)
            if (thl[x][y])
            {
               int tile = mEditorMain.draw_item_num;
               int tileNum = tile & PM_BTILE_TILENUM_MASK;
               int tileFlags = tile & PM_BTILE_ALL_FLAGS;

               mTileSets.drawTile(x, y, tileNum, tileFlags, mEditorMain.draw_tile_mode, preview);
            }
   }
   if (replace_mode == 0) // tile set
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
               if (fb != -1) mTileSets.drawTile(x, y, fb & PM_BTILE_TILENUM_MASK, PM_BTILE_ALL_SOLID, mEditorMain.draw_tile_mode, preview);

            }
   }

   if (replace_mode == 3) // exact tile set translate
   {
      if (mTileSets.currentTileSet.startIndex == 0) return;

      for (int x=0; x<100; x++)
         for (int y=0; y<100; y++)
            if (thl[x][y])
            {
               int tile = mLevel.l[x][y] & PM_BTILE_TILENUM_MASK;

               // find tileset that tile belongs to
               struct tileSet ts;
               if (mTileSets.findTileSetContainingIndex(ts, tile))
               {
                  // does tileSetType match current tileSetType?
                  if (ts.tileSetType == mTileSets.currentTileSet.tileSetType)
                  {
                     // get offset between tilesets
                     int offset = mTileSets.currentTileSet.startIndex - ts.startIndex;

                     mTileSets.drawTile(x, y, tile+offset, 0, 2, preview); // force draw tile only
                  }
               }
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
   int xs = (mLevelEditor.selection.x2 - mLevelEditor.selection.x1) + 1;
   int ys = (mLevelEditor.selection.y2 - mLevelEditor.selection.y1) + 1;

   // pattern offsets
   int xo = pattern_offset_x;
   int yo = pattern_offset_y;

   // offset into pattern
   int xi = (x+xo) % xs;
   int yi = (y+yo) % ys;

   return mLevel.l[mLevelEditor.selection.x1+xi][mLevelEditor.selection.y1+yi];
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






















































void mwTileHelper::invert_marks()
{
   // add 100 to all valid marks
   for (int a=0; a<100; a++)
      for (int b=0; b<100; b++) if (thl[a][b]) thl[a][b] += 100;

   // find max mark index
   int max = 0;
   for (int a=0; a<100; a++)
      for (int b=0; b<100; b++) if (thl[a][b]>max) max = thl[a][b];

   int nv = 1; // new index start

   // loop from max to 101
   for (int i=max; i>100; i--)
   {
      for (int a=0; a<100; a++)
         for (int b=0; b<100; b++) if (thl[a][b] == i) thl[a][b] = nv;

      nv++;
   }
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




void mwTileHelper::tileSetWidget(int x, int y, int tile, struct tileSet &target, bool &clicked, int d)
{
   struct tileSet ts;
   mTileSets.findTileSetContainingIndex(ts, tile);
   if (mWidget.mButtonTile2(x, y, 22, ts.displayIndex, ts.name.c_str(), d))
   {
      target = ts;
      clicked = true;
   }
}



int mwTileHelper::tileSetSelectWidget(int x1, int y1, int x2, int pad_height, int color, struct tileSet &target, const char * txt, bool &clicked, int d)
{
   int bts = 16;
   int height = 46 + bts + pad_height;
//   height += mLoop.pct_y;
   int y2 = y1 + height;
   al_draw_filled_rectangle(x1, y1, x2, y2, mColor.pc[color+208]); // background color
   mMiscFnx.titlex(txt, 15, color, x1, x2, y1); // title
   al_draw_rectangle(x1, y1, x2, y2, mColor.pc[color], 1); // frame

   int yfb = y1 + 15;
   x1+=6;
   int xt = 0;
   int xts=23;


   tileSetWidget(x1+xt++*xts, yfb, 256, target, clicked, d); // purple pipes
   tileSetWidget(x1+xt++*xts, yfb, 320, target, clicked, d); // red pipes
   tileSetWidget(x1+xt++*xts, yfb, 384, target, clicked, d); // green pipes
   tileSetWidget(x1+xt++*xts, yfb, 448, target, clicked, d); // blue pipes
   tileSetWidget(x1+xt++*xts, yfb, 192, target, clicked, d); // purple bricks
   xt++;
   tileSetWidget(x1+xt++*xts, yfb, 672, target, clicked, d); // template
   tileSetWidget(x1+xt++*xts, yfb, 704, target, clicked, d); // ice machine
   tileSetWidget(x1+xt++*xts, yfb, 736, target, clicked, d); // tan platform
   tileSetWidget(x1+xt++*xts, yfb, 768, target, clicked, d); // alien machine
   tileSetWidget(x1+xt++*xts, yfb, 800, target, clicked, d); // alien dark
   xt++;
   tileSetWidget(x1+xt++*xts, yfb, 504, target, clicked, d); // rainbow 1
   yfb+=23;
   xt=0;

   tileSetWidget(x1+xt++*xts, yfb, 896, target, clicked, d); // screen
   tileSetWidget(x1+xt++*xts, yfb, 928, target, clicked, d); // wrought
   tileSetWidget(x1+xt++*xts, yfb, 832, target, clicked, d); // wires
   tileSetWidget(x1+xt++*xts, yfb, 880, target, clicked, d); // white bricks
   tileSetWidget(x1+xt++*xts, yfb, 960, target, clicked, d); // brown brick
   tileSetWidget(x1+xt++*xts, yfb, 992, target, clicked, d); // fuzzy pipe
   tileSetWidget(x1+xt++*xts, yfb, 912, target, clicked, d); // dirt
   tileSetWidget(x1+xt++*xts, yfb, 864, target, clicked, d); // red dirt grass
   tileSetWidget(x1+xt++*xts, yfb, 944, target, clicked, d); // red dirt
   tileSetWidget(x1+xt++*xts, yfb, 976, target, clicked, d); // white rock
   tileSetWidget(x1+xt++*xts, yfb, 1008, target, clicked, d); // purple blocks
   xt++;
   tileSetWidget(x1+xt++*xts, yfb, 496, target, clicked, d); // rainbow 2

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
   mMiscFnx.titlex("Replace Tools", 15, color, x1, x2, y1); // title
   al_draw_rectangle(x1, y1, x2, y2, mColor.pc[color], 1); // frame
   int yfb = y1 + 15;

   // inner control margins
   int x3 = x1+3;
   int x4 = x2-3;

   yfb+=bsp;

   char msg[80];
   sprintf(msg, "invalid");
   if (replace_mode == 0) sprintf(msg, "Use Current Tileset");
   if (replace_mode == 1) sprintf(msg, "Use Specific Tile");
   if (replace_mode == 2) sprintf(msg, "Change Flags Only");
   if (replace_mode == 3) sprintf(msg, "Translate Tileset");
   if (mWidget.mButton(1, x3, 170,    1, yfb+2, bts,  2,2,1,1,  color, color,  15,   0,0, msg)) replace_mode++;
   if (replace_mode > 3) replace_mode = 0;

   int tx = x3+176;

   if ((replace_mode == 0) || (replace_mode == 3)) mWidget.mButtonTile2(tx, yfb, 22, mTileSets.currentTileSet.displayIndex, mTileSets.currentTileSet.name.c_str(), d);
   if (replace_mode == 1) // single tile
   {
      int tile = mEditorMain.draw_item_num;
      mEditorMain.get_text_description_of_block_based_on_flags(tile, msg);
      if (mWidget.mButtonTile2(tx, yfb, 22, tile & PM_BTILE_TILENUM_MASK, msg, d)) mBitmapTools.select_bitmap_from_level(mEditorMain.draw_item_num);
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
   int bx1 = mLevelEditor.selection.x1;
   int by1 = mLevelEditor.selection.y1;
   int bx2 = mLevelEditor.selection.x2;
   int by2 = mLevelEditor.selection.y2;
   int bw = bx2 - bx1;
   int bh = by2 - by1;

   int height = 83 + bh*20;
   //height += mLoop.pct_y;

   int y2 = y1 + height;

   al_draw_filled_rectangle(x1, y1, x2, y2, mColor.pc[color+208]); // background color
   mMiscFnx.titlex("Pattern Tools", 15, color, x1, x2, y1); // title
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
      mMiscFnx.get_block_range("Select Pattern", j1, j2, j3, j4, 1);
   }

   if (mWidget.mButton(4, -1, x4,    1, yfb, bts,  2,2,1,1,  10, 10,   15,  0,0, "Commit")) draw_pattern(0);
   yfb+=(bts+bsp);

   mWidget.mCheckBox(4, -1, x4-8,  1, yfb,bts, -1, pattern_preview, "preview", 15, 15);

   mWidget.mStepper(  1, x3,     80,    1, yfb, bts,   2, 2,  1, 1,  c1,  c1,  15,  15, 15,   0, pattern_offset_x, 8, 0, 1, "+X:" );
   mWidget.mStepper(  1, x3+90,  80,    1, yfb, bts,   2, 2,  1, 1,  c1,  c1,  15,  15, 15,   0, pattern_offset_y, 8, 0, 1, "+Y:" );

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














int mwTileHelper::show_tileset_controls(int x1, int x2, int y1, int color, int d)
{
   int bts = 16;
   int height = 85 + bts;
   int pad_height = 39;

   bool clicked = false;
   int yfb = y1 - pad_height + tileSetSelectWidget(x1, y1, x2, pad_height, color, mTileSets.currentTileSet, "Tile Sets", clicked, d);

   char msg[256];
   sprintf(msg, " Current Tile Set: %s ", mTileSets.currentTileSet.name.c_str());
   mWidget.mButton(5, (x1+x2)/2-3, -1,    1, yfb, bts,    2,2,1,1,   color,color,15,0,  0,msg);

   yfb+=(bts+2);

   int nt = 14; // max num tiles
   int t1 = mTileSets.currentTileSet.startIndex;
   int t2 = mTileSets.currentTileSet.endIndex;
   if ((t2-t1) < nt) nt = t2-t1; // if actual number of tiles is less, use that
   for (int i=0; i<nt; i++) al_draw_bitmap(mBitmap.tile[t1+i], x1+i*21+2, yfb, 0);

   return height;
}



// blocking function to select a tileset
// returns true if tileset was clicked
bool mwTileHelper::get_tileset(int x1, int y1, struct tileSet &ts, const char *txt)
{
   int x2 = x1 + 408;
   while (1)
   {
      mEventQueue.proc(1);
      al_flip_display();

      bool clicked = false;
      tileSetSelectWidget(x1, y1, x2, 0, 10, ts, txt, clicked, 0);
      if (clicked) return true;
      if ((mInput.key[ALLEGRO_KEY_ESCAPE][3]) || (mInput.mouse_b[2][3])) return false;
   }
   return false;
}


// display tileset and allow changing if clicked
// shows displayIndex tile from passed tileset
// also shows tooltip if hovered
// if clicked, calls 'get_tileset' to prompt for a new tileset
// returns true if changed
// used by frame controls
// - show and change current tileset
// - show and change tileset for each frame fill
bool mwTileHelper::tileSetWidget(int x, int y, int size, struct tileSet &ts, const char *txt, int d)
{
   if (strlen(txt))
   {
      int txo = size + 4;
      int tyo = (size-8) / 2;
      al_draw_text(mFont.pr8, mColor.pc[15], x+txo, y+tyo, 0, txt);
   }

   if (mWidget.mButtonTile2(x, y, size, ts.displayIndex, ts.name.c_str(), d))
   {
      if (get_tileset(x, y, ts, "Choose a Tile Set")) return true;
   }
   return false;
}









int mwTileHelper::show_frame_controls(int x1, int x2, int y1, int color, int d)
{
   int bts = 16; // button height
   int bsp = 4; // button spacing
   //   bts += mLoop.pct_y;
   //      bsp += mLoop.pct_x;

   // choose the lesser of the 2 sizes
   int list_size = frame_sections;
   if (list_size > (int)frameFills.size()) list_size = (int)frameFills.size();
   int list_item_height = 16;
   int list_item_y_spacing = 2;


   int height = 77 + list_size * (list_item_height + list_item_y_spacing);
   //height += mLoop.pct_y;

   int y2 = y1 + height;





   al_draw_filled_rectangle(x1, y1, x2, y2, mColor.pc[color+208]); // background color
   mMiscFnx.titlex("Frame Tools", 15, color, x1, x2, y1); // title
   al_draw_rectangle(x1, y1, x2, y2, mColor.pc[color], 1); // frame
   int yfb = y1 + 15;

   // inner control margins
   int x3 = x1+3;
   int x4 = x2-3;
   int xc = (x1+x2)/2;

   int c1 = color + 64;

   char msg[80];

   sprintf(msg, "Frames Detected:%d", frames_detected);
       mWidget.mButton(5, xc, -1, 1, yfb, bts, 2, 2, 1,  1,   c1, c1, 15, 0, 0, msg);
   if (mWidget.mButton(3, x3, -1, 1, yfb, bts, 2, 2, 1,  1,   c1, c1, 15, 0, 0, "Detect")) find_bound(20);

   if (mWidget.mButton(3, x3+61+mLoop.pct_x, -1, 1, yfb, bts, 2, 2, 1,  1,   c1, c1, 15, 0, 0, "I")) invert_marks();

   if (mWidget.mButton(4, -1, x4, 1, yfb, bts, 2, 2, 1,  1,   10, 10, 15, 0, 0, "Commit")) draw_frame_fills(0);


   yfb+=(bts+bsp);


   if (frame_common_tileset)
   {
      // set all to current tileset
      for (int i=0; i<16; i++) frameFills[i].ts = mTileSets.currentTileSet;

      tileSetWidget(x3+142, yfb, 14, mTileSets.currentTileSet, "", d);
      sprintf(msg, "Common Tileset:");
   }
   else sprintf(msg, "Common Tileset");
   mWidget.mCheckBox(3, x3+4, -1,  1, yfb,bts, -1, frame_common_tileset, msg, 15, 15);


   mWidget.mCheckBox(4, -1, x4-8,  1, yfb,bts, -1, frame_mode_preview, "preview", 15, 15);


   yfb+=(bts+bsp);





   // presets
   int col = 9+64;
   int preset = 0;
   mWidget.mDropDown(1, x3, 84,   1, yfb, bts, 1, 0, 2, col, col, 14, listItemPresets, preset, d);
   if (preset) clearFrameFills(preset);


   //   bts += mLoop.pct_y;
   mWidget.mStepper(2, 140, x4-4,    1, yfb, bts, 2, 2,  1, 1, c1,  c1,  15,  15, 0,      0, frame_sections, 8, 0, 1, "Sections:");
   //   bts -= mLoop.pct_y;


   yfb+=bts;
   yfb+=6;



   // iterate fill sections
   for (int i = 0; i<list_size; i++)
   {
      int y2 = yfb + list_item_height;
      show_frame_control_line(x3, yfb, x4, y2, i, d);
      yfb+= (list_item_height + list_item_y_spacing);

   }


   return height;
}

void mwTileHelper::show_frame_control_line(int x1, int y1, int x2, int y2, int index, int d)
{
   int height = y2-y1;

   // text y offset
   int tyo = (height - 8) / 2;

   // mode size
   int msz = height -2;

   // mode offset
   int myo = (height - msz) / 2;


   // tileset icon size
   int isz = height -2;
   //isz+= mLoop.pct_y;

   // icon offsets
   int iyo = (height - isz) / 2;

   // erase background
   al_draw_filled_rectangle(x1, y1, x2, y2, mColor.pc[0]);

   // frame
   al_draw_rectangle       (x1, y1, x2, y2, mColor.pc[15], 1);



   // text
   al_draw_textf(mFont.pr8, mColor.pc[15], x1+4, y1+tyo, 0, "%d", frameFills[index].frameIndex);

   int txs = 17;
   //txs+= mLoop.pct_x;

   al_draw_rectangle(x1, y1, x1+txs-1, y2, mColor.pc[15], 1);

   x1 += txs;

   // tileset icon
   if (!frame_common_tileset)
   {
      tileSetWidget(x1, y1+iyo, isz, frameFills[index].ts, "", d);
      x1 += (isz + 2);
   }

   // check for mouse on line and mouse b2 for context menu

   if ((mInput.mouse_b[2][0]) && (mInput.mouse_x > x1) && (mInput.mouse_x < x2) && (mInput.mouse_y > y1) && (mInput.mouse_y < y2))
   {
      sprintf(mMenu.menu_string[0],"Fill Items Menu");
      sprintf(mMenu.menu_string[1],"--------------");
      sprintf(mMenu.menu_string[2],"Clone Current Item");
      sprintf(mMenu.menu_string[3],"Remove Current Item");
      sprintf(mMenu.menu_string[4],"Copy last 2 to next");
      sprintf(mMenu.menu_string[5],"Copy last 3 to next");
      sprintf(mMenu.menu_string[6],"end");
      switch (mMenu.pmenu(5, 13, -20, 1))
      {
         case 2:
            printf("Clone Current Item\n");

            // insert copy of current index
            frameFills.insert(frameFills.begin() + index, frameFills[index]);

            // iterate fill sections to fix indexes
            for (int i = 0; i<(int)frameFills.size(); i++) frameFills[i].frameIndex = i+1;

            // add one section
            frame_sections++;
         break;
         case 3:
            printf("Remove Current Item\n");

            // insert copy of current index
            frameFills.erase(frameFills.begin() + index);

            // iterate fill sections to fix indexes
            for (int i = 0; i<(int)frameFills.size(); i++) frameFills[i].frameIndex = i+1;

            // subtract one section
            frame_sections--;
         break;


         case 4:
            //printf("Copy 2\n");

            // set next(+1) to current(0)
            frameFills.insert(frameFills.begin() + index+1, frameFills[index]);

            // set next(+1) to previous(-1)
            frameFills.insert(frameFills.begin() + index+1, frameFills[index-1]);

            // iterate fill sections to fix indexes
            for (int i = 0; i<(int)frameFills.size(); i++) frameFills[i].frameIndex = i+1;

            // add two sections
            frame_sections+=2;
            break;

         case 5:
            //printf("Copy 3\n");

            // set next(+1) to current(0)
            frameFills.insert(frameFills.begin() + index+1, frameFills[index]);

            // set next(+1) to previous(-1)
            frameFills.insert(frameFills.begin() + index+1, frameFills[index-1]);

            // set next(+1) to previous(-2)
            frameFills.insert(frameFills.begin() + index+1, frameFills[index-2]);

            // iterate fill sections to fix indexes
            for (int i = 0; i<(int)frameFills.size(); i++) frameFills[i].frameIndex = i+1;

            // add three sections
            frame_sections+=3;
            break;

      }
   }


   // draw mode dropDown widget
   if (frameFills[index].ts.tileSetType == 16) mWidget.mDropDown(0, x1, x2,    1, y1+myo,msz,  0, 0, 1, 0, 15, 14, listItems16Frame,  frameFills[index].mode, d);
   if (frameFills[index].ts.tileSetType == 24) mWidget.mDropDown(0, x1, x2,    1, y1+myo,msz,  0, 0, 1, 0, 15, 14, listItems24Frame,  frameFills[index].mode, d);
   if (frameFills[index].ts.tileSetType == 48) mWidget.mDropDown(0, x1, x2,    1, y1+myo,msz,  0, 0, 1, 0, 15, 14, listItems48Frame,  frameFills[index].mode, d);





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
   y += ss + show_replace_controls(  x1, x2, y,  9, d);
   y += ss + show_tileset_controls(  x1, x2, y,  8, d);
   y += ss + show_pattern_controls(  x1, x2, y,  6, d);
   y += ss + show_frame_controls(    x1, x2, y,  9, d);

   y -= ss; // remove last section spacing

   y += ms; // add margin spacing
   y-=3;    // height offset
   return y;
}


void mwTileHelper::draw(mwRect<int> &rect, int d, int have_focus)
{
   // erase background
   rect.draw_filled_rectangle(mColor.pc[0]);

   mMiscFnx.titlex("Tile Helper", 15, 13, rect.x1, rect.x2, rect.y1+1);
   int by1 = rect.y1+3;
   if (mWidget.buttont(rect.x2-12, by1, rect.x2-4, 9, 0,0,0,0, 0,-1,15,0, 0,0,0,d,"?")) mHelp.help("Tile Helper");

   int ys = mTileHelper.draw_buttons(rect.x1+1, rect.x2-1, rect.y1+20, d);

   rect.setHeight(ys-rect.y1+4);

   int fc = 13+64;
   if (have_focus) fc = 13;

   al_draw_rectangle(rect.x1, rect.y1, rect.x2, rect.y2, mColor.pc[fc], 1); // frame entire window
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


void mwTileHelper::process_mouse_on_background(void)
{
   if (mInput.mouse_b[1][0])
   {
      while (mInput.mouse_b[1][0]) mEventQueue.proc(1);

      // add or del single tile at the specific location
      if (match == 0) thl[mLevelEditor.gx][mLevelEditor.gy] = add_del;

      // add or del only connected tiles that match
      if (match == 2) find_connected(mLevelEditor.gx, mLevelEditor.gy, group);

      // add or del all tiles that match
      if (match == 1)
      {
         for (int x=0; x<100; x++)
            for (int y=0; y<100; y++)
               if (mTileSets.compareTile(mLevel.l[mLevelEditor.gx][mLevelEditor.gy], mLevel.l[x][y], group)) thl[x][y] = add_del;
      }
   }

   if (mInput.mouse_b[2][0])
   {
      while (mInput.mouse_b[2][0]) mEventQueue.proc(1);
      mLevelEditor.set_mode(1);
   }
}

void mwTileHelper::draw_level_editor_background_overlays(int mouse_on_window)
{
   if (!mouse_on_window) mEditorMain.show_draw_item_cursor();
   if (frame_mode_preview) draw_frame_fills(1);
   if (replace_preview) draw_replace(1);
   if (pattern_preview) draw_pattern(1);
   if (mark_overlay)
   {
      // show marked blocks
      for (int x=0; x<100; x++)
         for (int y=0; y<100; y++)
         {
            if (thl[x][y])
            {
               if (mark_overlay == 1)
               {
                  //int col = 10;
                  int c = mColor.flash_color+64;
                  int c2 = mColor.flash_color2+64;
                  al_draw_rectangle(x*20+0.5, y*20+0.5, x*20+20, y*20+20,   mColor.pc[c2], 0);
                  al_draw_line(x*20, y*20, x*20+20, y*20+20,   mColor.pc[c], 0);
                  al_draw_line(x*20+20, y*20, x*20, y*20+20,   mColor.pc[c], 0);
               }
               if (mark_overlay == 2)
               {
                  int c = mColor.flash_color+64;
                  al_draw_textf(mFont.pr8, mColor.pc[c], x*20+6, y*20+6, 0, "%d", thl[x][y]);
               }
            }
         }
   }
}





