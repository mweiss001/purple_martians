#include "pm.h"
#include "mwTileSets.h"
#include "mwInput.h"
#include "mwBitmap.h"
#include "mwBitmapTools.h"
#include "mwColor.h"
#include "mwDisplay.h"
#include "mwFont.h"
#include "mwEventQueue.h"
#include "mwWidget.h"

mwTileSets mTileSets;


mwTileSets::mwTileSets() { init(); }


void mwTileSets::constructEmptySet()
{
   ts.name = "";
   ts.startIndex        = 0;
   ts.endIndex          = 0;
   ts.solidMode         = 0;
   ts.frameMode         = 0;
   ts.hlineMode         = 0;
   ts.vlineMode         = 0;
   ts.extendedMode      = 0;
   ts.FrameCross        = 0;
   ts.Single            = 0;
   ts.SolidFill         = 0;

   ts.SingleReverse     = 0;

   ts.HLineL            = 0;
   ts.HLineM            = 0;
   ts.HLineR            = 0;
   ts.VLineT            = 0;
   ts.VLineM            = 0;
   ts.VLineB            = 0;
   ts.FrameCornerTL     = 0;
   ts.FrameCornerTR     = 0;
   ts.FrameCornerBL     = 0;
   ts.FrameCornerBR     = 0;
   ts.FrameEdgeL        = 0;
   ts.FrameEdgeR        = 0;
   ts.FrameEdgeT        = 0;
   ts.FrameEdgeB        = 0;
   ts.FrameEdgeLTee     = 0;
   ts.FrameEdgeRTee     = 0;
   ts.FrameEdgeTTee     = 0;
   ts.FrameEdgeBTee     = 0;
   ts.OuterCornerTL     = 0;
   ts.OuterCornerTR     = 0;
   ts.OuterCornerBL     = 0;
   ts.OuterCornerBR     = 0;
   ts.OuterEdgeL        = 0;
   ts.OuterEdgeR        = 0;
   ts.OuterEdgeT        = 0;
   ts.OuterEdgeB        = 0;
   ts.OuterEdgeLTee     = 0;
   ts.OuterEdgeRTee     = 0;
   ts.OuterEdgeTTee     = 0;
   ts.OuterEdgeBTee     = 0;
   ts.OuterCornerTLTeeT = 0;
   ts.OuterCornerTLTeeL = 0;
   ts.OuterCornerTLTeeTL= 0;
   ts.OuterCornerTRTeeT = 0;
   ts.OuterCornerTRTeeR = 0;
   ts.OuterCornerTRTeeTR= 0;
   ts.OuterCornerBLTeeB = 0;
   ts.OuterCornerBLTeeL = 0;
   ts.OuterCornerBLTeeBL= 0;
   ts.OuterCornerBRTeeB = 0;
   ts.OuterCornerBRTeeR = 0;
   ts.OuterCornerBRTeeBR= 0;
   ts.InnerCornerTL     = 0;
   ts.InnerCornerTR     = 0;
   ts.InnerCornerBL     = 0;
   ts.InnerCornerBR     = 0;
   ts.InnerEdgeL        = 0;
   ts.InnerEdgeR        = 0;
   ts.InnerEdgeT        = 0;
   ts.InnerEdgeB        = 0;

   ts.OuterCornerTRDiag = 0;
   ts.OuterCornerTLDiag = 0;

   ts.HLineMBreakable     = 0;
   ts.VLineMBreakable     = 0;
   ts.SingleReverseBreakable  = 0;

   ts.HLineMBomb        = 0;
   ts.VLineMBomb        = 0;
   ts.SingleReverseBomb     = 0;

   ts.SemiSolid         = 0;

}



// extended set
void mwTileSets::constructExtendedSet(std::string name, int i)
{
   constructEmptySet();

   ts.name              = name;
   ts.extendedMode      = 1;
   ts.startIndex        = i;
   ts.endIndex          = i + 63;

   ts.Single            = i;

   ts.HLineL            = i + 1;
   ts.HLineM            = i + 2;
   ts.HLineR            = i + 3;
   ts.VLineT            = i + 4;
   ts.VLineM            = i + 5;
   ts.VLineB            = i + 6;

   ts.FrameCornerTL     = i + 7;
   ts.FrameCornerTR     = i + 8;
   ts.FrameCornerBR     = i + 9;
   ts.FrameCornerBL     = i + 10;

   ts.FrameEdgeL        = i + 5;
   ts.FrameEdgeR        = i + 5;
   ts.FrameEdgeT        = i + 2;
   ts.FrameEdgeB        = i + 2;

   ts.FrameEdgeLTee     = i + 11;
   ts.FrameEdgeBTee     = i + 12;
   ts.FrameEdgeRTee     = i + 13;
   ts.FrameEdgeTTee     = i + 14;

   ts.FrameCross        = i + 15;


   ts.InnerCornerTL     = i + 16;
   ts.InnerCornerTR     = i + 17;
   ts.InnerCornerBR     = i + 18;
   ts.InnerCornerBL     = i + 19;

   ts.InnerEdgeL        = i + 26;
   ts.InnerEdgeT        = i + 27;
   ts.InnerEdgeR        = i + 24;
   ts.InnerEdgeB        = i + 25;


   ts.OuterCornerTL     = i + 20;
   ts.OuterCornerTR     = i + 21;
   ts.OuterCornerBR     = i + 22;
   ts.OuterCornerBL     = i + 23;

   ts.OuterEdgeL        = i + 24;
   ts.OuterEdgeT        = i + 25;
   ts.OuterEdgeR        = i + 26;
   ts.OuterEdgeB        = i + 27;

   ts.OuterEdgeLTee     = i + 28;
   ts.OuterEdgeTTee     = i + 29;
   ts.OuterEdgeRTee     = i + 30;
   ts.OuterEdgeBTee     = i + 31;


   ts.OuterCornerTLTeeT = i + 32;
   ts.OuterCornerTLTeeL = i + 36;
   ts.OuterCornerTLTeeTL= i + 40;

   ts.OuterCornerTRTeeR = i + 33;
   ts.OuterCornerTRTeeT = i + 37;
   ts.OuterCornerTRTeeTR= i + 41;

   ts.OuterCornerBRTeeB = i + 34;
   ts.OuterCornerBRTeeR = i + 38;
   ts.OuterCornerBRTeeBR= i + 42;

   ts.OuterCornerBLTeeL = i + 35;
   ts.OuterCornerBLTeeB = i + 39;
   ts.OuterCornerBLTeeBL= i + 43;

   ts.OuterCornerTRDiag = i + 44;
   ts.OuterCornerTLDiag = i + 45;

   ts.SolidFill         = i + 46;
   ts.SingleReverse     = i + 47;

   ts.HLineMBreakable     = i + 48;
   ts.VLineMBreakable     = i + 49;
   ts.SingleReverseBreakable  = i + 50;

   ts.HLineMBomb        = i + 51;
   ts.VLineMBomb        = i + 52;
   ts.SingleReverseBomb = i + 53;
   ts.SemiSolid         = i + 54;


   tileSets.push_back(ts);



   // frame
   constructEmptySet();
   ts.name              = name;
   ts.frameMode         = 1;
   ts.hlineMode         = 1;
   ts.vlineMode         = 1;
   ts.startIndex        = i;
   ts.endIndex          = i+15;
   ts.HLineL            = i + 1;
   ts.HLineM            = i + 2;
   ts.HLineR            = i + 3;
   ts.VLineT            = i + 4;
   ts.VLineM            = i + 5;
   ts.VLineB            = i + 6;
   ts.FrameCornerTL     = i + 7;
   ts.FrameCornerTR     = i + 8;
   ts.FrameCornerBR     = i + 9;
   ts.FrameCornerBL     = i + 10;
   ts.FrameEdgeL        = i + 5;
   ts.FrameEdgeR        = i + 5;
   ts.FrameEdgeT        = i + 2;
   ts.FrameEdgeB        = i + 2;
   tileSets.push_back(ts);

   // inner
   constructEmptySet();
   ts.name              = name;
   ts.frameMode         = 1;
   ts.hlineMode         = 1;
   ts.vlineMode         = 1;
   ts.startIndex        = i+16;
   ts.endIndex          = i+19;
   ts.HLineL            = i + 1;
   ts.HLineM            = i + 2;
   ts.HLineR            = i + 3;
   ts.VLineT            = i + 4;
   ts.VLineM            = i + 5;
   ts.VLineB            = i + 6;
   ts.FrameCornerTL     = i + 16;
   ts.FrameCornerTR     = i + 17;
   ts.FrameCornerBR     = i + 18;
   ts.FrameCornerBL     = i + 19;
   ts.FrameEdgeL        = i + 26;
   ts.FrameEdgeT        = i + 27;
   ts.FrameEdgeR        = i + 24;
   ts.FrameEdgeB        = i + 25;
   tileSets.push_back(ts);


   // solid
   constructEmptySet();
   ts.name              = name;
   ts.solidMode         = 1;
   ts.hlineMode         = 1;
   ts.vlineMode         = 1;
   ts.startIndex        = i+20;
   ts.endIndex          = i+63;
   ts.HLineL            = i + 1;
   ts.HLineM            = i + 2;
   ts.HLineR            = i + 3;
   ts.VLineT            = i + 4;
   ts.VLineM            = i + 5;
   ts.VLineB            = i + 6;
   ts.OuterCornerTL     = i + 20;
   ts.OuterCornerTR     = i + 21;
   ts.OuterCornerBR     = i + 22;
   ts.OuterCornerBL     = i + 23;
   ts.OuterEdgeL        = i + 24;
   ts.OuterEdgeT        = i + 25;
   ts.OuterEdgeR        = i + 26;
   ts.OuterEdgeB        = i + 27;
   ts.SolidFill         = i + 46;
   tileSets.push_back(ts);

}


















// solid rectangle set
tileSet mwTileSets::constructSolidRectangleSet(std::string name, int i)
{
   constructEmptySet();

   ts.name            = name;
   ts.solidMode       = 1;
   ts.startIndex      = i;
   ts.endIndex        = i+15;
   ts.Single     = i+1;
   ts.SolidFill       = i;
   ts.OuterCornerTL   = i + 8;
   ts.OuterCornerTR   = i + 9;
   ts.OuterCornerBL   = i + 10;
   ts.OuterCornerBR   = i + 11;
   ts.OuterEdgeL      = i + 12;
   ts.OuterEdgeR      = i + 13;
   ts.OuterEdgeT      = i + 14;
   ts.OuterEdgeB      = i + 15;
   setHline(i, 2, 3, 4);
   setVline(i, 5, 6, 7);
   return ts;
}


// frame rectangle set
tileSet mwTileSets::constructFrameRectangleSet(std::string name, int i)
{
   constructEmptySet();

   ts.name            = name;
   ts.frameMode       = 1;
   ts.startIndex      = i;
   ts.endIndex        = i+15;
   ts.Single     = i;
   ts.FrameCornerTL   = i + 8;
   ts.FrameCornerTR   = i + 9;
   ts.FrameCornerBL   = i + 10;
   ts.FrameCornerBR   = i + 11;
   ts.FrameEdgeL      = i + 6;
   ts.FrameEdgeR      = i + 6;
   ts.FrameEdgeT      = i + 3;
   ts.FrameEdgeB      = i + 3;
   setHline(i, 2, 3, 4);
   setVline(i, 5, 6, 7);
   return ts;
}

void mwTileSets::setBasicRect(int i)
{
   ts.FrameCornerTL   = i + 0;
   ts.FrameCornerTR   = i + 1;
   ts.FrameCornerBL   = i + 2;
   ts.FrameCornerBR   = i + 3;
   ts.FrameEdgeL      = i + 4;
   ts.FrameEdgeR      = i + 5;
   ts.FrameEdgeT      = i + 6;
   ts.FrameEdgeB      = i + 7;
}

void mwTileSets::setHline(int i, int left, int middle, int right)
{
   ts.hlineMode = 1;
   ts.HLineL    = i + left;
   ts.HLineM    = i + middle;
   ts.HLineR    = i + right;
}

void mwTileSets::setVline(int i, int top, int middle, int bottom)
{
   ts.vlineMode = 1;
   ts.VLineT    = i + top;
   ts.VLineM    = i + middle;
   ts.VLineB    = i + bottom;
}


// hline with addition of multiple hlines when drawing rect
tileSet mwTileSets::constructHlineSet(std::string name, int i)
{
   constructEmptySet();
   setHline(i, 0, 1, 2);

   ts.name            = name;
   ts.startIndex      = i;
   ts.endIndex        = i + 2;
   ts.solidMode       = 1;
   ts.SolidFill       = i + 1;
   ts.OuterCornerTL   = i + 0;
   ts.OuterCornerTR   = i + 2;
   ts.OuterCornerBL   = i + 0;
   ts.OuterCornerBR   = i + 2;
   ts.OuterEdgeL      = i + 0;
   ts.OuterEdgeR      = i + 2;
   ts.OuterEdgeT      = i + 1;
   ts.OuterEdgeB      = i + 1;


   return ts;
}

void mwTileSets::init()
{

   constructExtendedSet("pipes", 256);
   constructExtendedSet("pipes2", 320);



   tileSets.push_back(constructFrameRectangleSet("wires", 832));


   tileSets.push_back(constructSolidRectangleSet("brown and yellow thatch", 864));
   tileSets.push_back(constructSolidRectangleSet("brain", 896));
   tileSets.push_back(constructSolidRectangleSet("grey bricks", 928));
   tileSets.push_back(constructSolidRectangleSet("brown bricks", 960));
   tileSets.push_back(constructSolidRectangleSet("blue solid", 992));


   // single hline
   tileSets.push_back(constructHlineSet("lined platform", 161));
   tileSets.push_back(constructHlineSet("brown bricks", 177));


   // single vline
   constructEmptySet();
   ts.name = "white chain";
   ts.startIndex = 54;
   ts.endIndex   = 56;
   setVline(54, 0, 1, 2);
   tileSets.push_back(ts);


   int i = 33;
   constructEmptySet();
   ts.name = "semi-solid screen";
   ts.startIndex      = 33;
   ts.endIndex        = 40;
   ts.frameMode       = 1;
   ts.FrameCornerTL   = i + 6;
   ts.FrameCornerTR   = i + 7;
   ts.FrameCornerBL   = i + 3;
   ts.FrameCornerBR   = i + 5;
   ts.FrameEdgeL      = i + 1;
   ts.FrameEdgeR      = i + 1;
   ts.FrameEdgeT      = i + 4;
   ts.FrameEdgeB      = i + 4;
   setHline(i, 3, 4, 5);
   setVline(i, 0, 1, 2);
   tileSets.push_back(ts);


   i = 672;
   constructEmptySet();
   ts.name = "rainbows";
   ts.startIndex = i;
   ts.endIndex   = i + 7;
   ts.frameMode   = 1;
   setBasicRect(i);
   tileSets.push_back(ts);

   i = 704;
   constructEmptySet();
   ts.name = "rainbows2";
   ts.startIndex = i;
   ts.endIndex   = i + 7;
   ts.frameMode   = 1;
   setBasicRect(i);
   tileSets.push_back(ts);
}





bool mwTileSets::findTileSetContainingIndex(struct tileSet &t, int tileIndex, int type)
{
   for (auto ts : tileSets)
      if (tileIndex >= ts.startIndex && tileIndex <= ts.endIndex)
      {
         if (type == 0)                    { t = ts; return true; }
         if (type == 1 && ts.frameMode)    { t = ts; return true; }
         if (type == 2 && ts.solidMode)    { t = ts; return true; }
         if (type == 3 && ts.hlineMode)    { t = ts; return true; }
         if (type == 4 && ts.vlineMode)    { t = ts; return true; }
         if (type == 5 && ts.extendedMode) { t = ts; return true; }
      }
   return false;
}




// passed a rect to draw from level editor
// bx and by are tile positions in the level array
// rect is guaranteed to have bx1 < bx2 and by1 < by2
// if drawItem is part of a tile set, use appropriate tiles from that set
void mwTileSets::draw(int bx1, int bx2, int by1, int by2, int drawItemNum, int drawItemFlags, int level[][100])
{

   int bw = bx2-bx1; // width
   int bh = by2-by1; // height

   int drawTile = drawItemNum; // default

   // single tile
   if (bw==0 && bh==0) level[bx1][by1] = drawTile | drawItemFlags; // single tile 1 x 1


   // vertical line
   if (bw==0 && bh>0)
   {
      int drawMode = 0;
      struct tileSet s;
      if (findTileSetContainingIndex(s, drawItemNum, 4)) drawMode = 1;
      for (int y=by1; y<by2+1; y++) // cycle the range
      {
         if (drawMode)
         {
            drawTile = s.VLineM;
            if (y == by1) drawTile = s.VLineT;
            if (y == by2) drawTile = s.VLineB;
         }
         level[bx1][y] = drawTile |= drawItemFlags;
      }
   }


   // horizontal line
   if (bw>0 && bh==0)
   {
      int drawMode = 0;
      struct tileSet s;
      if (findTileSetContainingIndex(s, drawItemNum, 3)) drawMode = 1;
      for (int x=bx1; x<bx2+1; x++) // cycle the range
      {
         if (drawMode)
         {
            drawTile = s.HLineM;
            if (x == bx1) drawTile = s.HLineL;
            if (x == bx2) drawTile = s.HLineR;
         }
         level[x][by1] = drawTile |= drawItemFlags;
      }
   }

   // rectangle (where w and h are both > 0)
   if (bw>0 && bh>0)
   {
      struct tileSet s;
      // get draw mode
      int drawMode = 0; // default
      if (findTileSetContainingIndex(s, drawItemNum, 1)) drawMode = 1; // frame
      if (findTileSetContainingIndex(s, drawItemNum, 2)) drawMode = 2; // solid

      for (int x=bx1; x<bx2+1; x++)
         for (int y=by1; y<by2+1; y++)
         {
            if (drawMode == 1)
            {
                                     drawTile = -1;
               if (y == by1)         drawTile = s.FrameEdgeT;
               if (y == by2)         drawTile = s.FrameEdgeB;
               if (x == bx1)
               {
                  if      (y == by1) drawTile = s.FrameCornerTL;
                  else if (y == by2) drawTile = s.FrameCornerBL;
                  else               drawTile = s.FrameEdgeL;
               }
               if (x == bx2)
               {
                  if      (y == by1) drawTile = s.FrameCornerTR;
                  else if (y == by2) drawTile = s.FrameCornerBR;
                  else               drawTile = s.FrameEdgeR;
               }
            }
            if (drawMode == 2)
            {
                                     drawTile = s.SolidFill;
               if (y == by1)         drawTile = s.OuterEdgeT;
               if (y == by2)         drawTile = s.OuterEdgeB;
               if (x == bx1)
               {
                  if      (y == by1) drawTile = s.OuterCornerTL;
                  else if (y == by2) drawTile = s.OuterCornerBL;
                  else               drawTile = s.OuterEdgeL;
               }
               if (x == bx2)
               {
                  if      (y == by1) drawTile = s.OuterCornerTR;
                  else if (y == by2) drawTile = s.OuterCornerBR;
                  else               drawTile = s.OuterEdgeR;
               }
            }
            if (drawTile != -1) level[x][y] = drawTile |= drawItemFlags;
         } 
   }
}


// from tile helper
// do tiles match exactly, or are they part of the same set?
bool mwTileSets::compareTile(int rb, int cb, int set)
{

   // remove flags for comparison
   int r = rb & 1023;
   int c = cb & 1023;

   // exact match
   if (r == c) return true;

   if (set) // check if both belong to same set
   {
      for (const auto& s : tileSets)
         if (r >= s.startIndex && r <= s.endIndex && c >= s.startIndex && c <= s.endIndex) return true;
   }
   return false;
}










































































































































































































































































































































































// ----------------------------------------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------------------------------
// -------- create tiles for tileSets -----------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------------------------------


void mwTileSets::erase_pixels_outside_rounded_corners(ALLEGRO_BITMAP *b, ALLEGRO_COLOR c)
{
   al_set_target_bitmap(b);

   int w = al_get_bitmap_width(b);
   int h = al_get_bitmap_height(b);

   float D = 0.005;
   for (int y=0; y<h; y++)
   {
      int x = 0;
      int done = 0;
      while (!done)
      {
         ALLEGRO_COLOR p = al_get_pixel(b, x, y);
         if ( (abs(p.r - c.r) < D) && (abs(p.g - c.g) < D) && (abs(p.b - c.b) < D) ) done = 1;
         else al_put_pixel(x, y, al_map_rgb(0,0,0));
         if (++x > w-1) done = 1;
      }
      x = w-1;
      done = 0;
      while (!done)
      {
         ALLEGRO_COLOR p = al_get_pixel(b, x, y);
         if ( (abs(p.r - c.r) < D) && (abs(p.g - c.g) < D) && (abs(p.b - c.b) < D) ) done = 1;
         else al_put_pixel(x, y, al_map_rgb(0,0,0));
         if (--x < 0) done = 1;
      }
   }
}


void mwTileSets::make_hline_vline_and_single(ALLEGRO_BITMAP *b, ALLEGRO_BITMAP *b60, int indexS, int indexH, int indexV)
{
   int s;

   // make 3x1 hline
   ALLEGRO_BITMAP *tmp2 = al_create_bitmap(60, 20);
   al_set_target_bitmap(tmp2);
   al_draw_bitmap_region(b60, 0,   0, 60, 10, 0,  0, 0); // upper half
   al_draw_bitmap_region(b60, 0,  50, 60, 10, 0, 10, 0); // lower half
   al_set_target_bitmap(b);
   s = indexH + 0; al_draw_bitmap_region(tmp2,  0, 0, 20, 20, (s % 32)*22+1, (s / 32)*22+1, 0); // hline l
   s = indexH + 1; al_draw_bitmap_region(tmp2, 20, 0, 20, 20, (s % 32)*22+1, (s / 32)*22+1, 0); // hline m
   s = indexH + 2; al_draw_bitmap_region(tmp2, 40, 0, 20, 20, (s % 32)*22+1, (s / 32)*22+1, 0); // hline r

   // make 1x3 vline
   ALLEGRO_BITMAP *tmp3 = al_create_bitmap(20, 60);
   al_set_target_bitmap(tmp3);
   al_draw_bitmap_region(b60, 0,  0, 10, 60, 0,  0, 0); // left half
   al_draw_bitmap_region(b60, 50, 0, 10, 60, 10, 0, 0); // right half
   al_set_target_bitmap(b);
   s = indexV + 0; al_draw_bitmap_region(tmp3, 0,  0, 20, 20, (s % 32)*22+1, (s / 32)*22+1, 0); // vline u
   s = indexV + 1; al_draw_bitmap_region(tmp3, 0, 20, 20, 20, (s % 32)*22+1, (s / 32)*22+1, 0); // vline m
   s = indexV + 2; al_draw_bitmap_region(tmp3, 0, 40, 20, 20, (s % 32)*22+1, (s / 32)*22+1, 0); // vline l

   // single standalone block with frame on all sides
   al_set_target_bitmap(tmp3);
   al_draw_bitmap_region(tmp3, 0, 50, 20, 10, 0, 10, 0); // copy bottom to line up with top

   al_set_target_bitmap(b);
   s = indexS;  al_draw_bitmap_region(tmp3, 0, 0, 20, 20, (s % 32)*22+1, (s / 32)*22+1, 0);

   al_destroy_bitmap(tmp2);
   al_destroy_bitmap(tmp3);
}


void mwTileSets::draw_bomb_overlay()
{
   al_draw_filled_rectangle(6.5,5.5,14.5,15.5, al_map_rgb(0,0,0));
   al_draw_filled_rectangle(5.5,6.5,15.5,14.5, al_map_rgb(0,0,0));

   al_put_pixel( 9, 13, al_map_rgb(130,130,130));
   al_put_pixel(10, 13, al_map_rgb(130,130,130));
   al_put_pixel( 9, 10, al_map_rgb(130,130,130));
   al_put_pixel(10, 10, al_map_rgb(130,130,130));
   al_put_pixel( 8, 11, al_map_rgb(130,130,130));
   al_put_pixel( 8, 12, al_map_rgb(130,130,130));
   al_put_pixel(11, 11, al_map_rgb(130,130,130));
   al_put_pixel(11, 12, al_map_rgb(130,130,130));

   al_put_pixel( 9, 11, al_map_rgb(65,65,65));
   al_put_pixel( 9, 12, al_map_rgb(65,65,65));
   al_put_pixel(10, 11, al_map_rgb(65,65,65));
   al_put_pixel(10, 12, al_map_rgb(65,65,65));

   al_put_pixel(10, 9,  al_map_rgb(185,185,185));
   al_put_pixel(11, 8,  al_map_rgb(185,185,185));

   al_put_pixel(12, 7,  al_map_rgb(202,202,0));

}

void mwTileSets::draw_B_overlay()
{
   ALLEGRO_COLOR c = al_map_rgb(0,219,105);
   al_draw_filled_rectangle(4.5 , 3.5, 14.5, 5.5, c);
   al_draw_filled_rectangle(4.5,  9.5, 15.5, 11.5, c);
   al_draw_filled_rectangle(4.5, 15.5, 14.5, 17.5, c);
   al_draw_filled_rectangle(4.5 , 3.5, 6.5, 17.5, c);
   al_draw_line(14.5,  4.5, 14.5, 16.5, c, 1); // 14,4   14,15
   al_draw_line(15.5,  5.5, 15.5,  8.5, c, 1); // 15,5   15,7
   al_draw_line(15.5, 11.5, 15.5, 15.5, c, 1); // 15,11  15,14
   al_put_pixel(13, 5, c);
   al_put_pixel(13, 8, c);
   al_put_pixel(13, 11, c);
   al_put_pixel(13, 14, c);
}



void mwTileSets::draw_lightning_crack_overlay()
{
   ALLEGRO_COLOR c = al_map_rgb(0,0,0);
   al_draw_line(5.5, 10.5, 16.5,    0, c, 0); // 5,10 15,0
   al_draw_line(6.5, 10.5, 17.5,    0, c, 0);
   al_draw_line(7.5, 10.5, 18.5,    0, c, 0);
   al_draw_line(2.5, 19.5, 13.5,    9, c, 0);
   al_draw_line(3.5, 19.5, 14.5,    9, c, 0);
   al_draw_line(4.5, 19.5, 15.5,    9, c, 0);
   al_draw_line(5.5, 10.5, 13.5, 10.5, c, 0);
   al_draw_line(6.5,  9.5, 14.5,  9.5, c, 0);
}


void mwTileSets::draw_lock_overlay_bitmap(ALLEGRO_BITMAP *b, ALLEGRO_COLOR lc)
{
   al_set_target_bitmap(b);
   al_clear_to_color(al_map_rgb(0,0,0));
   ALLEGRO_COLOR ac = al_map_rgb(1, 1, 1); // alpha mask color
   ALLEGRO_COLOR gc = mColor.pc[15+96];    // grey color

   al_draw_line(3.5, 9.5, 9.5, 9.5, lc, 1);
   al_draw_line(3.5, 8.5, 9.5, 8.5, gc, 1);
   al_draw_line(3.5, 7.5, 9.5, 7.5, lc, 1);
   al_draw_line(3.5, 6.5, 9.5, 6.5, gc, 1);

   al_draw_line(4.5, 3.5, 4.5, 6.5, lc, 1);
   al_draw_line(7.5, 3.5, 7.5, 6.5, lc, 1);
   al_draw_line(5.5, 2.5, 7.5, 2.5, lc, 1);

   al_draw_pixel(0.5,   0.5, ac);
   al_draw_pixel(0.5,  11.5, ac);
   al_draw_pixel(11.5,  0.5, ac);
   al_draw_pixel(11.5, 11.5, ac);
   al_convert_mask_to_alpha(b, ac);
}

void mwTileSets::fill_step_color_array(float h1, float h2, float s1, float s2, float l1, float l2, int steps)
{
   if (steps > 20) steps = 20;
   int div = steps - 1;
   if (div < 1) div = 1;
   float linc = (l2 - l1) / div;
   float sinc = (s2 - s1) / div;
   float hinc = (h2 - h1) / div;
   for (int i=0; i<steps; i++)
      step_color_array[i] = al_color_hsl(h1+i*hinc, s1+i*sinc, l1+i*linc);

   step_color_array_size = steps;
}

void mwTileSets::draw_and_frame_3x3_bitmap(int type, float round, ALLEGRO_BITMAP *bmp, int tile)
{
   al_set_target_bitmap(bmp);
   // background solid color or tiles
   if (!tile) al_clear_to_color(step_color_array[step_color_array_size-1]);
   else
   {
      for (int i=0; i<3; i++)
         for (int j=0; j<3; j++)
            al_draw_bitmap(mBitmap.btile[tile], j*20, i*20, 0);
   }

   float x1 = 0.5;
   float y1 = 0.5;
   float x2 = 59.5;
   float y2 = 59.5;

   float x1a = 19.5;
   float y1a = 19.5;
   float x2a = 40.5;
   float y2a = 40.5;

   if ((type == 0) || (type == 2)) al_draw_filled_rectangle(x1a, y1a, x2a, y2a, al_map_rgb(0,0,0));

   for (float i=0; i<step_color_array_size-.6; i+=.01)
   {
      // this is common for all types
      al_draw_rounded_rectangle(x1+i, y1+i, x2-i, y2-i, round, round, step_color_array[(int)i], 1);

      if ((type == 0) || (type == 2))
      {
         al_draw_rounded_rectangle(x1a-i, y1a-i, x2a+i, y2a+i, 0, 0, step_color_array[(int)i], 1);
         //al_draw_rounded_rectangle(x1a-i, y1a-i, x2a+i, y2a+i, round, round, step_color_array[(int)i], 1);
      }
   }
   erase_pixels_outside_rounded_corners(bmp, step_color_array[0]);
}



void mwTileSets::create_tileset_frame(int bs, float h1, float h2, float s1, float s2, float l1, float l2, int steps, float round)
{
   fill_step_color_array(h1, h2, s1, s2, l1, l2, steps);
   ALLEGRO_COLOR bc = step_color_array[step_color_array_size-1]; // background color

   // make 3x3 grid, frame and extract tiles
   ALLEGRO_BITMAP *tmp1 = al_create_bitmap(60, 60);
   draw_and_frame_3x3_bitmap(0, round, tmp1, 0);

   ALLEGRO_BITMAP *b1 = mBitmapTools.load_block_tiles_to_bitmap();
   al_set_target_bitmap(b1);
   int s=bs;
   s = bs +  8; al_draw_bitmap_region(tmp1, 0,   0, 20, 20, (s % 32)*22+1, (s / 32)*22+1, 0); // ul corner
   s = bs +  9; al_draw_bitmap_region(tmp1, 40,  0, 20, 20, (s % 32)*22+1, (s / 32)*22+1, 0); // ur corner
   s = bs + 10; al_draw_bitmap_region(tmp1, 0,  40, 20, 20, (s % 32)*22+1, (s / 32)*22+1, 0); // ll corner
   s = bs + 11; al_draw_bitmap_region(tmp1, 40, 40, 20, 20, (s % 32)*22+1, (s / 32)*22+1, 0); // lr corner

   make_hline_vline_and_single(b1, tmp1, bs+1, bs+2, bs+5);

   // get some pieces to construct more complex blocks
   ALLEGRO_BITMAP *cn = al_create_bitmap(10, 10);
   al_set_target_bitmap(cn);
   al_clear_to_color(al_map_rgb(0,0,0));
   al_draw_bitmap_region(tmp1, 10, 10, 10, 10, 0, 0, 0); // inside corner

   ALLEGRO_BITMAP *hf = al_create_bitmap(10, 20);
   al_set_target_bitmap(hf);
   al_clear_to_color(al_map_rgb(0,0,0));
   al_draw_bitmap_region(tmp1, 0, 20, 10, 20, 0, 0, 0); // half

   // single block
   al_destroy_bitmap(tmp1);
   tmp1 = al_create_bitmap(20, 20);

   // cross
   al_set_target_bitmap(tmp1);
   al_clear_to_color(bc);
   al_draw_bitmap_region(cn, 0, 0, 10, 10, 10, 10, 0);
   al_draw_bitmap_region(cn, 0, 0, 10, 10, 0,  10, ALLEGRO_FLIP_HORIZONTAL);
   al_draw_bitmap_region(cn, 0, 0, 10, 10, 10,  0, ALLEGRO_FLIP_VERTICAL);
   al_draw_bitmap_region(cn, 0, 0, 10, 10, 0,   0, ALLEGRO_FLIP_HORIZONTAL | ALLEGRO_FLIP_VERTICAL);
   al_set_target_bitmap(b1);
   s = bs; al_draw_bitmap(tmp1, (s % 32)*22+1, (s / 32)*22+1, 0);

   // tees
   al_set_target_bitmap(tmp1);
   al_clear_to_color(bc);
   al_draw_bitmap_region(hf, 0, 0, 10, 20, 0,  0, 0); // left half
   al_draw_bitmap_region(cn, 0, 0, 10, 10, 10, 10, 0); // inside corner for lr
   al_draw_bitmap_region(cn, 0, 0, 10, 10, 10,  0, ALLEGRO_FLIP_VERTICAL); // inside corner for ur
   al_set_target_bitmap(b1);
   s = bs+12; al_draw_bitmap(        tmp1,         (s % 32)*22+1,  (s / 32)*22+1,  0);
   s = bs+13; al_draw_bitmap(        tmp1,         (s % 32)*22+1,  (s / 32)*22+1,  ALLEGRO_FLIP_HORIZONTAL);
   s = bs+14; al_draw_rotated_bitmap(tmp1, 10, 10, (s % 32)*22+11, (s / 32)*22+11, ALLEGRO_PI / 2, 0);
   s = bs+15; al_draw_rotated_bitmap(tmp1, 10, 10, (s % 32)*22+11, (s / 32)*22+11, ALLEGRO_PI / 2, ALLEGRO_FLIP_HORIZONTAL);

   mBitmapTools.save_bitmap_to_block_tiles_file(b1);

   al_destroy_bitmap(b1);
   al_destroy_bitmap(tmp1);
   al_destroy_bitmap(cn);
   al_destroy_bitmap(hf);
}

void mwTileSets::create_tileset_solid(int bs, int tile, float h1, float h2, float s1, float s2, float l1, float l2, int steps, float round)
{
   printf("mTileSets.create_tileset_solid(%d, %d, %3.2f, %3.2f, %3.2f, %3.2f, %3.2f, %3.2f, %d, %3.2f);\n", bs, tile, h1, h2, s1, s2, l1, l2, steps, round);
   printf("base_tile_index=%d;\nsolid_source_tile_index=%d;\nh1=%3.0f;\nh2=%3.0f;\ns1=%3.2f;\ns2=%3.2f;\nl1=%3.2f;\nl2=%3.2f;\nsteps=%d;\nround=%3.1f;\n",  bs, tile, h1, h2, s1, s2, l1, l2, steps, round);
   printf("create_tileset_solid(base_tile_index, solid_source_tile_index, h1, h2, s1, s2, l1, l2, steps, round);\n");

   fill_step_color_array(h1, h2, s1, s2, l1, l2, steps);

   ALLEGRO_BITMAP *tmp1 = al_create_bitmap(60, 60);
   draw_and_frame_3x3_bitmap(1, round, tmp1, tile);

   ALLEGRO_BITMAP *b1 = mBitmapTools.load_block_tiles_to_bitmap();
   al_set_target_bitmap(b1);

   int s=bs;  al_draw_bitmap_region(tmp1, 20, 20, 20, 20, (s % 32)*22+1, (s / 32)*22+1, 0); // middle
   s = bs+8;  al_draw_bitmap_region(tmp1, 0,   0, 20, 20, (s % 32)*22+1, (s / 32)*22+1, 0); // ul corner
   s = bs+9;  al_draw_bitmap_region(tmp1, 40,  0, 20, 20, (s % 32)*22+1, (s / 32)*22+1, 0); // ur corner
   s = bs+10; al_draw_bitmap_region(tmp1, 0,  40, 20, 20, (s % 32)*22+1, (s / 32)*22+1, 0); // ll corner
   s = bs+11; al_draw_bitmap_region(tmp1, 40, 40, 20, 20, (s % 32)*22+1, (s / 32)*22+1, 0); // lr corner
   s = bs+12; al_draw_bitmap_region(tmp1, 0,  20, 20, 20, (s % 32)*22+1, (s / 32)*22+1, 0); // l edge
   s = bs+13; al_draw_bitmap_region(tmp1, 40, 20, 20, 20, (s % 32)*22+1, (s / 32)*22+1, 0); // r edge
   s = bs+14; al_draw_bitmap_region(tmp1, 20,  0, 20, 20, (s % 32)*22+1, (s / 32)*22+1, 0); // u edge
   s = bs+15; al_draw_bitmap_region(tmp1, 20, 40, 20, 20, (s % 32)*22+1, (s / 32)*22+1, 0); // d edge

   make_hline_vline_and_single(b1, tmp1, bs+1, bs+2, bs+5);

   // save modified block tiles bitmap to file
   mBitmapTools.save_bitmap_to_block_tiles_file(b1);

   al_destroy_bitmap(b1);
   al_destroy_bitmap(tmp1);
}










void mwTileSets::create_tileset_extended(int bs, float h1, float h2, float s1, float s2, float l1, float l2, int steps, float round)
{
   printf("\n\nmTileSets.create_tileset_extended(%d, %3.2f, %3.2f, %3.2f, %3.2f, %3.2f, %3.2f, %d, %3.2f);\n", bs, h1, h2, s1, s2, l1, l2, steps, round);
   printf("base_tile_index=%d;\nh1=%3.0f;\nh2=%3.0f;\ns1=%3.2f;\ns2=%3.2f;\nl1=%3.2f;\nl2=%3.2f;\nsteps=%d;\nround=%3.1f;\n",  bs, h1, h2, s1, s2, l1, l2, steps, round);
   printf("create_tileset_extended(base_tile_index, h1, h2, s1, s2, l1, l2, steps, round);\n");

   fill_step_color_array(h1, h2, s1, s2, l1, l2, steps);

   // background color
   ALLEGRO_COLOR bc = step_color_array[step_color_array_size-1];


   // load block tiles from file to bitmap, so we can modify it
   ALLEGRO_BITMAP *b1 = mBitmapTools.load_block_tiles_to_bitmap();
   al_set_target_bitmap(b1);
   int s=bs;

   // draw 3x3 with frame fade color and round
   ALLEGRO_BITMAP *tmp1 = al_create_bitmap(60, 60);
   draw_and_frame_3x3_bitmap(2, round, tmp1, 0);

   // get single, hline and vline
   make_hline_vline_and_single(b1, tmp1, bs, bs+1, bs+4);

   // get corners
   s = bs +  7;  al_draw_bitmap_region(tmp1, 0,   0, 20, 20, (s % 32)*22+1, (s / 32)*22+1, 0); // tl corner
   s = bs +  8;  al_draw_bitmap_region(tmp1, 40,  0, 20, 20, (s % 32)*22+1, (s / 32)*22+1, 0); // tr corner
   s = bs +  9;  al_draw_bitmap_region(tmp1, 40, 40, 20, 20, (s % 32)*22+1, (s / 32)*22+1, 0); // br corner
   s = bs + 10;  al_draw_bitmap_region(tmp1, 0,  40, 20, 20, (s % 32)*22+1, (s / 32)*22+1, 0); // bl corner

   // get some pieces to construct more complex blocks
   ALLEGRO_BITMAP *cf = al_create_bitmap(10, 10);
   al_set_target_bitmap(cf);
   al_clear_to_color(al_map_rgb(0,0,0));
   al_draw_bitmap_region(tmp1, 10, 10, 10, 10, 0, 0, 0); // faded br corner

   ALLEGRO_BITMAP *hf = al_create_bitmap(10, 20);
   al_set_target_bitmap(hf);
   al_clear_to_color(al_map_rgb(0,0,0));
   al_draw_bitmap_region(tmp1, 0, 20, 10, 20, 0, 0, 0); // faded half

   // single block
   ALLEGRO_BITMAP *sb = al_create_bitmap(20, 20);

   // tees
   al_set_target_bitmap(sb);
   al_clear_to_color(bc);
   al_draw_bitmap(hf, 0,  0,  0);                             // faded left half
   al_draw_bitmap(cf, 10,  0, ALLEGRO_FLIP_VERTICAL);         // faded tr corner
   al_draw_bitmap(cf, 10, 10, 0);                             // faded br corner
   al_set_target_bitmap(b1);
   s = bs+11; al_draw_rotated_bitmap(sb, 10, 10, (s % 32)*22+11, (s / 32)*22+11, ALLEGRO_PI * 0.0, 0);
   s = bs+12; al_draw_rotated_bitmap(sb, 10, 10, (s % 32)*22+11, (s / 32)*22+11, ALLEGRO_PI * 0.5, 0);
   s = bs+13; al_draw_rotated_bitmap(sb, 10, 10, (s % 32)*22+11, (s / 32)*22+11, ALLEGRO_PI * 1.0, 0);
   s = bs+14; al_draw_rotated_bitmap(sb, 10, 10, (s % 32)*22+11, (s / 32)*22+11, ALLEGRO_PI * 1.5, 0);

   // cross
   al_set_target_bitmap(sb);
   al_clear_to_color(bc);
   al_draw_bitmap(cf, 10,  0, ALLEGRO_FLIP_VERTICAL);                           // faded tr corner
   al_draw_bitmap(cf, 10, 10, 0);                                               // faded br corner
   al_draw_bitmap(cf, 0,  10, ALLEGRO_FLIP_HORIZONTAL);                         // faded bl corner
   al_draw_bitmap(cf, 0,   0, ALLEGRO_FLIP_HORIZONTAL | ALLEGRO_FLIP_VERTICAL); // faded tl corner
   al_set_target_bitmap(b1);
   s = bs + 15; al_draw_bitmap(sb, (s % 32)*22+1, (s / 32)*22+1, 0);


   // the first 16 tiles should be done.......they are all part of the 'frame' tileset


   // inner corners
   al_set_target_bitmap(sb);
   al_clear_to_color(bc);
   al_draw_bitmap(cf, 10, 10, 0);          // faded br corner
   al_set_target_bitmap(b1);
   s = bs+16; al_draw_rotated_bitmap(sb, 10, 10, (s % 32)*22+11, (s / 32)*22+11, ALLEGRO_PI * 0.0, 0);
   s = bs+17; al_draw_rotated_bitmap(sb, 10, 10, (s % 32)*22+11, (s / 32)*22+11, ALLEGRO_PI * 0.5, 0);
   s = bs+18; al_draw_rotated_bitmap(sb, 10, 10, (s % 32)*22+11, (s / 32)*22+11, ALLEGRO_PI * 1.0, 0);
   s = bs+19; al_draw_rotated_bitmap(sb, 10, 10, (s % 32)*22+11, (s / 32)*22+11, ALLEGRO_PI * 1.5, 0);


   // solid
   // draw 3x3 grid and extract tiles
   draw_and_frame_3x3_bitmap(1, round, tmp1, 0);

   al_set_target_bitmap(b1);
   s = bs+20; al_draw_bitmap_region(tmp1, 0,   0, 20, 20, (s % 32)*22+1, (s / 32)*22+1, 0); // tl corner
   s = bs+21; al_draw_bitmap_region(tmp1, 40,  0, 20, 20, (s % 32)*22+1, (s / 32)*22+1, 0); // tr corner
   s = bs+22; al_draw_bitmap_region(tmp1, 40, 40, 20, 20, (s % 32)*22+1, (s / 32)*22+1, 0); // br corner
   s = bs+23; al_draw_bitmap_region(tmp1, 0,  40, 20, 20, (s % 32)*22+1, (s / 32)*22+1, 0); // bl corner
   s = bs+46; al_draw_bitmap_region(tmp1, 20, 20, 20, 20, (s % 32)*22+1, (s / 32)*22+1, 0); // center
   s = bs+24; al_draw_bitmap_region(tmp1, 0,  20, 20, 20, (s % 32)*22+1, (s / 32)*22+1, 0); // l edge
   s = bs+25; al_draw_bitmap_region(tmp1, 20,  0, 20, 20, (s % 32)*22+1, (s / 32)*22+1, 0); // u edge
   s = bs+26; al_draw_bitmap_region(tmp1, 40, 20, 20, 20, (s % 32)*22+1, (s / 32)*22+1, 0); // r edge
   s = bs+27; al_draw_bitmap_region(tmp1, 20, 40, 20, 20, (s % 32)*22+1, (s / 32)*22+1, 0); // d edge


   // edge tees
   al_set_target_bitmap(sb);
   al_clear_to_color(bc);
   al_draw_bitmap(cf, 10,  0, ALLEGRO_FLIP_VERTICAL);         // faded tr corner
   al_draw_bitmap(cf, 10, 10, 0);                             // faded br corner
   al_set_target_bitmap(b1);
   s = bs+28; al_draw_rotated_bitmap(sb, 10, 10, (s % 32)*22+11, (s / 32)*22+11, ALLEGRO_PI * 1.0, 0);
   s = bs+29; al_draw_rotated_bitmap(sb, 10, 10, (s % 32)*22+11, (s / 32)*22+11, ALLEGRO_PI * 1.5, 0);
   s = bs+30; al_draw_rotated_bitmap(sb, 10, 10, (s % 32)*22+11, (s / 32)*22+11, ALLEGRO_PI * 0.0, 0);
   s = bs+31; al_draw_rotated_bitmap(sb, 10, 10, (s % 32)*22+11, (s / 32)*22+11, ALLEGRO_PI * 0.5, 0);

   // solid corner 1 way tees
   al_set_target_bitmap(sb);
   al_clear_to_color(bc);
   al_draw_bitmap(hf, 0, 0, 0); // left half faded
   al_draw_bitmap(cf,  10, 0, ALLEGRO_FLIP_VERTICAL); // faded tr corner
   al_set_target_bitmap(b1);
   s = bs+32; al_draw_rotated_bitmap(sb, 10, 10, (s % 32)*22+11, (s / 32)*22+11, ALLEGRO_PI * 0.0, 0);
   s = bs+33; al_draw_rotated_bitmap(sb, 10, 10, (s % 32)*22+11, (s / 32)*22+11, ALLEGRO_PI * 0.5, 0);
   s = bs+34; al_draw_rotated_bitmap(sb, 10, 10, (s % 32)*22+11, (s / 32)*22+11, ALLEGRO_PI * 1.0, 0);
   s = bs+35; al_draw_rotated_bitmap(sb, 10, 10, (s % 32)*22+11, (s / 32)*22+11, ALLEGRO_PI * 1.5, 0);

   // flip and rotate
   al_set_target_bitmap(sb);
   al_draw_rotated_bitmap(sb, 10, 10, 10, 10, ALLEGRO_PI * 1.5, ALLEGRO_FLIP_HORIZONTAL);
   al_set_target_bitmap(b1);
   s = bs+36; al_draw_rotated_bitmap(sb, 10, 10, (s % 32)*22+11, (s / 32)*22+11, ALLEGRO_PI * 0.0, 0);
   s = bs+37; al_draw_rotated_bitmap(sb, 10, 10, (s % 32)*22+11, (s / 32)*22+11, ALLEGRO_PI * 0.5, 0);
   s = bs+38; al_draw_rotated_bitmap(sb, 10, 10, (s % 32)*22+11, (s / 32)*22+11, ALLEGRO_PI * 1.0, 0);
   s = bs+39; al_draw_rotated_bitmap(sb, 10, 10, (s % 32)*22+11, (s / 32)*22+11, ALLEGRO_PI * 1.5, 0);

   // solid corner 2 way tees
   // make cross with one solid corner
   al_set_target_bitmap(sb);
   al_clear_to_color(bc);
   al_draw_bitmap(cf,  0,  0, ALLEGRO_FLIP_HORIZONTAL | ALLEGRO_FLIP_VERTICAL);  // faded corner tl
   al_draw_bitmap(cf, 10,  0, ALLEGRO_FLIP_VERTICAL);                            // faded corner tr

   al_draw_bitmap(cf,  0, 10, ALLEGRO_FLIP_HORIZONTAL);                          // faded corner bl
   al_set_target_bitmap(b1);
   s = bs+40; al_draw_rotated_bitmap(sb, 10, 10, (s % 32)*22+11, (s / 32)*22+11, ALLEGRO_PI * 0.0, 0);
   s = bs+41; al_draw_rotated_bitmap(sb, 10, 10, (s % 32)*22+11, (s / 32)*22+11, ALLEGRO_PI * 0.5, 0);
   s = bs+42; al_draw_rotated_bitmap(sb, 10, 10, (s % 32)*22+11, (s / 32)*22+11, ALLEGRO_PI * 1.0, 0);
   s = bs+43; al_draw_rotated_bitmap(sb, 10, 10, (s % 32)*22+11, (s / 32)*22+11, ALLEGRO_PI * 1.5, 0);

   // diagonal corners
   al_set_target_bitmap(sb);
   al_clear_to_color(bc);
   al_draw_bitmap(cf, 10,  0, ALLEGRO_FLIP_VERTICAL);                            // faded corner tr
   al_draw_bitmap(cf,  0, 10, ALLEGRO_FLIP_HORIZONTAL);                          // faded corner bl
   al_set_target_bitmap(b1);
   s = bs+44; al_draw_rotated_bitmap(sb, 10, 10, (s % 32)*22+11, (s / 32)*22+11, ALLEGRO_PI * 0.0, 0);
   s = bs+45; al_draw_rotated_bitmap(sb, 10, 10, (s % 32)*22+11, (s / 32)*22+11, ALLEGRO_PI * 0.5, 0);


   // single reverse
   al_set_target_bitmap(sb);
   al_clear_to_color(bc);
   al_draw_bitmap(cf, 0,   0, 0);                                                // reverse faded corner tl
   al_draw_bitmap(cf, 0,  10, ALLEGRO_FLIP_VERTICAL);                            // reverse faded corner bl
   al_draw_bitmap(cf, 10,  0, ALLEGRO_FLIP_HORIZONTAL);                          // reverse faded corner tr
   al_draw_bitmap(cf, 10, 10, ALLEGRO_FLIP_HORIZONTAL | ALLEGRO_FLIP_VERTICAL);  // reverse faded corner br
   al_set_target_bitmap(b1);
   s = bs+47; al_draw_rotated_bitmap(sb, 10, 10, (s % 32)*22+11, (s / 32)*22+11, ALLEGRO_PI * 0.0, 0);



/*


   mBitmapTools.get_tile_from_tilemap(b1, sb, bs+2); // hline through
   al_draw_bitmap(mBitmap.btile[188], 0, 0, 0);
   mBitmapTools.put_tile_to_tilemap(b1, sb, bs+56);

   mBitmapTools.get_tile_from_tilemap(b1, sb, bs+2); // hline through
   al_draw_bitmap(mBitmap.btile[189], 0, 0, 0);
   mBitmapTools.put_tile_to_tilemap(b1, sb, bs+57);

   mBitmapTools.get_tile_from_tilemap(b1, sb, bs+2); // hline through
   al_draw_bitmap(mBitmap.btile[190], 0, 0, 0);
   mBitmapTools.put_tile_to_tilemap(b1, sb, bs+58);

   mBitmapTools.get_tile_from_tilemap(b1, sb, bs+2); // hline through
   al_draw_bitmap(mBitmap.btile[191], 0, 0, 0);
   mBitmapTools.put_tile_to_tilemap(b1, sb, bs+59);

   mBitmapTools.get_tile_from_tilemap(b1, sb, bs+5); // vline through
   al_draw_bitmap(mBitmap.btile[188], 0, 0, 0);
   mBitmapTools.put_tile_to_tilemap(b1, sb, bs+60);

   mBitmapTools.get_tile_from_tilemap(b1, sb, bs+5); // vline through
   al_draw_bitmap(mBitmap.btile[189], 0, 0, 0);
   mBitmapTools.put_tile_to_tilemap(b1, sb, bs+61);

   mBitmapTools.get_tile_from_tilemap(b1, sb, bs+5); // vline through
   al_draw_bitmap(mBitmap.btile[190], 0, 0, 0);
   mBitmapTools.put_tile_to_tilemap(b1, sb, bs+62);

   mBitmapTools.get_tile_from_tilemap(b1, sb, bs+5); // vline through
   al_draw_bitmap(mBitmap.btile[191], 0, 0, 0);
   mBitmapTools.put_tile_to_tilemap(b1, sb, bs+63);

   // create lock overlays
      ALLEGRO_BITMAP *lkr = al_create_bitmap(12, 12);
      draw_lock_overlay_bitmap(lkr, mColor.pc[10]);

      ALLEGRO_BITMAP *lkg = al_create_bitmap(12, 12);
      draw_lock_overlay_bitmap(lkg, mColor.pc[11]);

      ALLEGRO_BITMAP *lkb = al_create_bitmap(12, 12);
      draw_lock_overlay_bitmap(lkb, mColor.pc[13]);

      ALLEGRO_BITMAP *lkp = al_create_bitmap(12, 12);
      draw_lock_overlay_bitmap(lkp, mColor.pc[8]);



   mBitmapTools.get_tile_from_tilemap(b1, sb, bs+2); // hline through
   al_draw_bitmap(lkr, 4, 4, 0);
   mBitmapTools.put_tile_to_tilemap(b1, sb, bs+56);

   mBitmapTools.get_tile_from_tilemap(b1, sb, bs+2); // hline through
   al_draw_bitmap(lkg, 4, 4, 0);
   mBitmapTools.put_tile_to_tilemap(b1, sb, bs+57);

   mBitmapTools.get_tile_from_tilemap(b1, sb, bs+2); // hline through
   al_draw_bitmap(lkb, 4, 4, 0);
   mBitmapTools.put_tile_to_tilemap(b1, sb, bs+58);

   mBitmapTools.get_tile_from_tilemap(b1, sb, bs+2); // hline through
   al_draw_bitmap(lkp, 4, 4, 0);
   mBitmapTools.put_tile_to_tilemap(b1, sb, bs+59);


   mBitmapTools.get_tile_from_tilemap(b1, sb, bs+5); // vline through
   al_draw_bitmap(lkr, 4, 4, 0);
   mBitmapTools.put_tile_to_tilemap(b1, sb, bs+60);

   mBitmapTools.get_tile_from_tilemap(b1, sb, bs+5); // vline through
   al_draw_bitmap(lkg, 4, 4, 0);
   mBitmapTools.put_tile_to_tilemap(b1, sb, bs+61);

   mBitmapTools.get_tile_from_tilemap(b1, sb, bs+5); // vline through
   al_draw_bitmap(lkb, 4, 4, 0);
   mBitmapTools.put_tile_to_tilemap(b1, sb, bs+62);

   mBitmapTools.get_tile_from_tilemap(b1, sb, bs+5); // vline through
   al_draw_bitmap(lkp, 4, 4, 0);
   mBitmapTools.put_tile_to_tilemap(b1, sb, bs+63);

*/


   // create B blocks
   mBitmapTools.get_tile_from_tilemap(b1, sb, bs+2); //  hline through
   draw_B_overlay();
   mBitmapTools.put_tile_to_tilemap(b1, sb, bs+48);

   mBitmapTools.get_tile_from_tilemap(b1, sb, bs+5); //  vline through
   draw_B_overlay();
   mBitmapTools.put_tile_to_tilemap(b1, sb, bs+49);

   mBitmapTools.get_tile_from_tilemap(b1, sb, bs+47); //  single reverse
   draw_B_overlay();
   mBitmapTools.put_tile_to_tilemap(b1, sb, bs+50);

/*
   // create lightning cracks
   mBitmapTools.get_tile_from_tilemap(b1, sb, bs+2); //  hline through
   draw_lightning_crack_overlay();
   mBitmapTools.put_tile_to_tilemap(b1, sb, bs+48);

   mBitmapTools.get_tile_from_tilemap(b1, sb, bs+5); //  vline through
   draw_lightning_crack_overlay();
   mBitmapTools.put_tile_to_tilemap(b1, sb, bs+49);

   mBitmapTools.get_tile_from_tilemap(b1, sb, bs+47); //  single reverse
   draw_lightning_crack_overlay();
   mBitmapTools.put_tile_to_tilemap(b1, sb, bs+50);
*/

   // create bombable
   mBitmapTools.get_tile_from_tilemap(b1, sb, bs+2); //  hline through
   draw_bomb_overlay();
   mBitmapTools.put_tile_to_tilemap(b1, sb, bs+51);

   mBitmapTools.get_tile_from_tilemap(b1, sb, bs+5); //  vline through
   draw_bomb_overlay();
   mBitmapTools.put_tile_to_tilemap(b1, sb, bs+52);

   mBitmapTools.get_tile_from_tilemap(b1, sb, bs+47); //  single reverse
   draw_bomb_overlay();
   mBitmapTools.put_tile_to_tilemap(b1, sb, bs+53);



   // create semisolid from base tile
   mBitmapTools.get_tile_from_tilemap(b1, sb, bs);
   mBitmapTools.put_tile_to_tilemap(b1, sb, bs+54);


   // save modified block tiles bitmap to file
   mBitmapTools.save_bitmap_to_block_tiles_file(b1);

   al_destroy_bitmap(b1);
   al_destroy_bitmap(tmp1);
   al_destroy_bitmap(sb);
   al_destroy_bitmap(hf);
   al_destroy_bitmap(cf);
/*
   al_destroy_bitmap(lkr);
   al_destroy_bitmap(lkg);
   al_destroy_bitmap(lkb);
   al_destroy_bitmap(lkp);
*/

   // set default tile flags

   // set all to solid
   int flag = PM_BTILE_SOLID_PLAYER | PM_BTILE_SOLID_ENEMY | PM_BTILE_SOLID_ITEM | PM_BTILE_SOLID_PBUL | PM_BTILE_SOLID_EBUL | PM_BTILE_SHOW_SELECT_WIN;
   for (int i=bs; i<bs+64; i++)
      mBitmap.tileFlags[i] = flag;

   // breakable
   int br_flag = flag;
   br_flag |= PM_BTILE_BREAKABLE_ESHOT;
   br_flag |= PM_BTILE_BREAKABLE_PSHOT;
   for (int i=bs+48; i<bs+51; i++)
      mBitmap.tileFlags[i] = br_flag;

   // bombable
   int bm_flag = flag | PM_BTILE_BOMBABLE;
   for (int i=bs+51; i<bs+54; i++)
      mBitmap.tileFlags[i] = bm_flag;

   // semisolid
   int ss_flag = PM_BTILE_SOLID_PLAYER | PM_BTILE_SOLID_ENEMY | PM_BTILE_SOLID_ITEM | PM_BTILE_SEMISOLID_PLAYER | PM_BTILE_SEMISOLID_ENEMY | PM_BTILE_SEMISOLID_ITEM | PM_BTILE_SHOW_SELECT_WIN;
   mBitmap.tileFlags[bs+54] = ss_flag;

   // unused
   for (int i=bs+55; i<bs+64; i++)
      mBitmap.tileFlags[i] = 0;


   mBitmap.save_sprit();

}



void mwTileSets::showLiveTile(int x, int y, int type, int tile, float h1, float h2, float s1, float s2, float l1, float l2, int steps, float round)
{
   fill_step_color_array(h1, h2, s1, s2, l1, l2, steps);

   ALLEGRO_BITMAP *tmp1 = al_create_bitmap(60, 60);

   draw_and_frame_3x3_bitmap(type, round, tmp1, tile);
   al_set_target_backbuffer(mDisplay.display);
   al_draw_scaled_bitmap(tmp1, 0, 0, 60, 60, x, y, 180, 180, 0);

   if (type == 2) // draw second one for extended
   {
      draw_and_frame_3x3_bitmap(1, round, tmp1, tile);
      al_set_target_backbuffer(mDisplay.display);
      al_draw_scaled_bitmap(tmp1, 0, 0, 60, 60, x+200, y, 180, 180, 0);
   }
   al_destroy_bitmap(tmp1);
}






void mwTileSets::showTileSet(int x, int y, int type, int bs)
{
   if (type == 2)
   {
      struct tileSet s;
      if (mTileSets.findTileSetContainingIndex(s, bs, 5))
      {
         ALLEGRO_BITMAP *tmp1 = al_create_bitmap(320, 80); // 16 x 4
         al_set_target_bitmap(tmp1);
         al_clear_to_color(al_map_rgb(0,0,0));

         al_draw_bitmap(mBitmap.btile[s.FrameCornerTL],      0*20,  0*20, 0);
         al_draw_bitmap(mBitmap.btile[s.FrameEdgeBTee],      1*20,  0*20, 0);
         al_draw_bitmap(mBitmap.btile[s.FrameCornerTR],      2*20,  0*20, 0);
         al_draw_bitmap(mBitmap.btile[s.VLineT],             3*20,  0*20, 0);

         al_draw_bitmap(mBitmap.btile[s.FrameEdgeLTee],      0*20,  1*20, 0);
         al_draw_bitmap(mBitmap.btile[s.FrameCross],         1*20,  1*20, 0);
         al_draw_bitmap(mBitmap.btile[s.FrameEdgeRTee],      2*20,  1*20, 0);
         al_draw_bitmap(mBitmap.btile[s.VLineM],             3*20,  1*20, 0);

         al_draw_bitmap(mBitmap.btile[s.FrameCornerBL],      0*20,  2*20, 0);
         al_draw_bitmap(mBitmap.btile[s.FrameEdgeTTee],      1*20,  2*20, 0);
         al_draw_bitmap(mBitmap.btile[s.FrameCornerBR],      2*20,  2*20, 0);
         al_draw_bitmap(mBitmap.btile[s.VLineB],             3*20,  2*20, 0);

         al_draw_bitmap(mBitmap.btile[s.HLineL],             0*20,  3*20, 0);
         al_draw_bitmap(mBitmap.btile[s.HLineM],             1*20,  3*20, 0);
         al_draw_bitmap(mBitmap.btile[s.HLineR],             2*20,  3*20, 0);
         al_draw_bitmap(mBitmap.btile[s.Single],             3*20,  3*20, 0);


         al_draw_bitmap(mBitmap.btile[s.OuterCornerTL],      4*20,  0*20, 0);
         al_draw_bitmap(mBitmap.btile[s.OuterCornerTR],      5*20,  0*20, 0);
         al_draw_bitmap(mBitmap.btile[s.OuterCornerBL],      4*20,  1*20, 0);
         al_draw_bitmap(mBitmap.btile[s.OuterCornerBR],      5*20,  1*20, 0);

         al_draw_bitmap(mBitmap.btile[s.InnerCornerTL],      4*20,  2*20, 0);
         al_draw_bitmap(mBitmap.btile[s.InnerCornerTR],      5*20,  2*20, 0);
         al_draw_bitmap(mBitmap.btile[s.InnerCornerBL],      4*20,  3*20, 0);
         al_draw_bitmap(mBitmap.btile[s.InnerCornerBR],      5*20,  3*20, 0);

         al_draw_bitmap(mBitmap.btile[s.OuterEdgeL],         6*20,  0*20, 0);
         al_draw_bitmap(mBitmap.btile[s.OuterEdgeT],         7*20,  0*20, 0);
         al_draw_bitmap(mBitmap.btile[s.OuterEdgeB],         6*20,  1*20, 0);
         al_draw_bitmap(mBitmap.btile[s.OuterEdgeR],         7*20,  1*20, 0);

         al_draw_bitmap(mBitmap.btile[s.OuterEdgeLTee],      6*20,  2*20, 0);
         al_draw_bitmap(mBitmap.btile[s.OuterEdgeTTee],      7*20,  2*20, 0);
         al_draw_bitmap(mBitmap.btile[s.OuterEdgeBTee],      6*20,  3*20, 0);
         al_draw_bitmap(mBitmap.btile[s.OuterEdgeRTee],      7*20,  3*20, 0);

         al_draw_bitmap(mBitmap.btile[s.OuterCornerTLTeeT],  8*20,  0*20, 0);
         al_draw_bitmap(mBitmap.btile[s.OuterCornerTRTeeR],  9*20,  0*20, 0);
         al_draw_bitmap(mBitmap.btile[s.OuterCornerBLTeeL],  8*20,  1*20, 0);
         al_draw_bitmap(mBitmap.btile[s.OuterCornerBRTeeB],  9*20,  1*20, 0);

         al_draw_bitmap(mBitmap.btile[s.OuterCornerTLTeeL],  8*20,  2*20, 0);
         al_draw_bitmap(mBitmap.btile[s.OuterCornerTRTeeT],  9*20,  2*20, 0);
         al_draw_bitmap(mBitmap.btile[s.OuterCornerBLTeeB],  8*20,  3*20, 0);
         al_draw_bitmap(mBitmap.btile[s.OuterCornerBRTeeR],  9*20,  3*20, 0);

         al_draw_bitmap(mBitmap.btile[s.OuterCornerTLTeeTL], 10*20, 0*20, 0);
         al_draw_bitmap(mBitmap.btile[s.OuterCornerTRTeeTR], 11*20, 0*20, 0);
         al_draw_bitmap(mBitmap.btile[s.OuterCornerBLTeeBL], 10*20, 1*20, 0);
         al_draw_bitmap(mBitmap.btile[s.OuterCornerBRTeeBR], 11*20, 1*20, 0);

         al_draw_bitmap(mBitmap.btile[s.OuterCornerTRDiag],  10*20, 2*20, 0);
         al_draw_bitmap(mBitmap.btile[s.OuterCornerTLDiag],  10*20, 3*20, 0);

         al_draw_bitmap(mBitmap.btile[s.SolidFill],          11*20, 2*20, 0);
         al_draw_bitmap(mBitmap.btile[s.SingleReverse],      11*20, 3*20, 0);

         al_draw_bitmap(mBitmap.btile[s.HLineMBreakable],    12*20, 0*20, 0);
         al_draw_bitmap(mBitmap.btile[s.VLineMBreakable],    12*20, 1*20, 0);
         al_draw_bitmap(mBitmap.btile[s.SingleReverseBreakable], 12*20, 2*20, 0);

         al_draw_bitmap(mBitmap.btile[s.SemiSolid],          12*20, 3*20, 0);

         al_draw_bitmap(mBitmap.btile[s.HLineMBomb],         13*20, 0*20, 0);
         al_draw_bitmap(mBitmap.btile[s.VLineMBomb],         13*20, 1*20, 0);
         al_draw_bitmap(mBitmap.btile[s.SingleReverseBomb],  13*20, 2*20, 0);


         al_set_target_backbuffer(mDisplay.display);
         al_draw_scaled_bitmap(tmp1, 0, 0, 320, 80, x, y, 640, 160, 0);

         al_destroy_bitmap(tmp1);
      }
      else printf("tileset not found\n");


   }


   if (type < 2)
   {
      ALLEGRO_BITMAP *tmp1 = al_create_bitmap(90, 90);

      al_set_target_bitmap(tmp1);
      al_clear_to_color(al_map_rgb(0,0,0));


      // corners
      al_draw_bitmap(mBitmap.btile[bs+8],   0,  0, 0);
      al_draw_bitmap(mBitmap.btile[bs+9],  40,  0, 0);
      al_draw_bitmap(mBitmap.btile[bs+10],  0, 40, 0);
      al_draw_bitmap(mBitmap.btile[bs+11], 40, 40, 0);

      // hline
      al_draw_bitmap(mBitmap.btile[bs+2],  0, 70, 0); // left
      al_draw_bitmap(mBitmap.btile[bs+3], 20, 70, 0); // center
      al_draw_bitmap(mBitmap.btile[bs+4], 40, 70, 0); // right

      // vline
      al_draw_bitmap(mBitmap.btile[bs+5], 70, 0,  0); // top
      al_draw_bitmap(mBitmap.btile[bs+6], 70, 20, 0); // center
      al_draw_bitmap(mBitmap.btile[bs+7], 70, 40, 0); // bottom

      // single
      al_draw_bitmap(mBitmap.btile[bs+1], 70, 70, 0);



      if (type == 1)
      {
         // solid sides
         al_draw_bitmap(mBitmap.btile[bs+14], 20,  0, 0); // top
         al_draw_bitmap(mBitmap.btile[bs+15], 20, 40, 0); // bottom
         al_draw_bitmap(mBitmap.btile[bs+12],  0, 20, 0); // left
         al_draw_bitmap(mBitmap.btile[bs+13], 40, 20, 0); // right

         // solid center
         al_draw_bitmap(mBitmap.btile[bs+0], 20, 20, 0); // center
      }

      // through pipes for frame
      if (type == 0)
      {
         al_draw_bitmap(mBitmap.btile[bs+3],  20,  0, 0); // top    - hline through
         al_draw_bitmap(mBitmap.btile[bs+3],  20, 40, 0); // bottom - hline through
         al_draw_bitmap(mBitmap.btile[bs+6],   0, 20, 0); // left   - vline through
         al_draw_bitmap(mBitmap.btile[bs+6],  40, 20, 0); // right  - vline through
      }

      al_set_target_backbuffer(mDisplay.display);
      al_draw_scaled_bitmap(tmp1, 0, 0, 90, 90, x, y, 360, 360, 0);


      // tees and cross for frame type
      if (type == 0)
      {
         al_destroy_bitmap(tmp1);
         tmp1 = al_create_bitmap(40, 40);
         al_set_target_bitmap(tmp1);
         al_clear_to_color(al_map_rgb(0,0,0));

         // corners
         al_draw_bitmap(mBitmap.btile[bs+12],  0,  0, 0);
         al_draw_bitmap(mBitmap.btile[bs+14], 20,  0, 0);
         al_draw_bitmap(mBitmap.btile[bs+15],  0, 20, 0);
         al_draw_bitmap(mBitmap.btile[bs+13], 20, 20, 0);

         al_set_target_backbuffer(mDisplay.display);
         al_draw_scaled_bitmap(tmp1, 0, 0, 40, 40, x+400, y, 160, 160, 0);

         // cross
         al_draw_scaled_bitmap(mBitmap.btile[bs], 0, 0, 20, 20, x+400, y+280, 80, 80, 0);
      }
      al_destroy_bitmap(tmp1);
   }
}


void mwTileSets::modify_tile_set()
{
   int type = 1; // 0-frame 1-solid 2-extended
   int base_tile_index = 864;
   int solid_source_tile_index = 192;


   // // frame in 992
   // type = 0; // 0-frame 1-solid 2-extended
   // base_tile_index = 992;
   // solid_source_tile_index = 0;

   // // yellow thatch solid
   // type = 1; // 0-frame 1-solid 2-extended
   // base_tile_index = 864;
   // solid_source_tile_index = 192;

   // purple pipes extended
   type = 2; // 0-frame 1-solid 2-extended
   base_tile_index = 256;
   solid_source_tile_index = 0;

/*
   // red pipes extended
   type = 2; // 0-frame 1-solid 2-extended
   base_tile_index = 320;
   solid_source_tile_index = 0;
*/



   int quit = 0;
   float h1 = 270;
   float h2 = 270;
   float s1 = 0.63;
   float s2 = 0.80;
   float l1 = 0.10;
   float l2 = 0.35;
   float steps = 9;
   float round = 7.5;



   if (type == 1)
   {
      steps = 2;
      round = 2.0;
   }

   // main extended purple pipe set
   if (base_tile_index == 256)
   {
      h1=266;
      h2=270;
      s1=0.88;
      s2=0.95;
      l1=0.10;
      l2=0.35;
      steps=10;
      round=0.0;
      create_tileset_extended(base_tile_index, h1, h2, s1, s2, l1, l2, steps, round);
   }


   // 2nd extended red pipe set
   if (base_tile_index == 320)
   {
      h1=346;
      h2=350;
      s1=0.88;
      s2=0.95;
      l1=0.10;
      l2=0.35;
      steps=10;
      round=0.0;
      create_tileset_extended(base_tile_index, h1, h2, s1, s2, l1, l2, steps, round);
   }






   // yellow thatch
   if (solid_source_tile_index == 192)
   {
      h1=141;
      h2= 83;
      s1=0.71;
      s2=0.37;
      l1=0.36;
      l2=0.30;
      steps=2;
      round=2.0;
      create_tileset_solid(base_tile_index, solid_source_tile_index, h1, h2, s1, s2, l1, l2, steps, round);
   }

   // brain
   if (solid_source_tile_index == 193)
   {
      h1=237;
      h2=237;
      s1=0.15;
      s2=0.17;
      l1=0.27;
      l2=0.13;
      steps=2;
      round=2.0;
      create_tileset_solid(base_tile_index, solid_source_tile_index, h1, h2, s1, s2, l1, l2, steps, round);
   }

   // grey bricks
   if (solid_source_tile_index == 194)
   {
      h1=145;
      h2=145;
      s1=0.07;
      s2=0.10;
      l1=0.55;
      l2=0.35;
      steps=2;
      round=2.0;
      create_tileset_solid(base_tile_index, solid_source_tile_index, h1, h2, s1, s2, l1, l2, steps, round);
   }

   // brown bricks
   if (solid_source_tile_index == 195)
   {
      h1=12;
      h2=12;
      s1=0.76;
      s2=0.93;
      l1=0.12;
      l2=0.37;
      steps=1;
      round=0.0;
      create_tileset_solid(base_tile_index, solid_source_tile_index, h1, h2, s1, s2, l1, l2, steps, round);
   }



   int h_lock = true;
   int s_lock = true;
   int l_lock = true;

   float h_offset = h2-h1;
   float s_offset = s2-s1;
   float l_offset = l2-l1;


   al_show_mouse_cursor(mDisplay.display);
   while (!quit)
   {
      mEventQueue.proc(1);
      al_flip_display();

      al_set_target_backbuffer(mDisplay.display);
      al_clear_to_color(al_map_rgb(0,0,0));
      showTileSet(20, 210, type, base_tile_index);

      float h1_hover_val = h1;
      float s1_hover_val = s1;
      float l1_hover_val = l1;
      float h2_hover_val = h2;
      float s2_hover_val = s2;
      float l2_hover_val = l2;
      float steps_hover_val = steps;
      float round_hover_val = round;

      bool h1_hover = false;
      bool h2_hover = false;
      bool s1_hover = false;
      bool s2_hover = false;
      bool l1_hover = false;
      bool l2_hover = false;

      bool na = false;

      int xa = 20;
      int xb = 260;
      float ya = 8;
      int bts = 16;

      bool changed = false;

      ALLEGRO_COLOR c1 = al_color_hsl(h1, s1, l1);
      ALLEGRO_COLOR c2 = al_color_hsl(h2, s2, l2);


      // hue
      int old_hlock = h_lock;
      mWidget.mCheckBox(1, xa+246, xb,  1,ya,bts, -1, h_lock, "lock", 15, 15);
      if ((!h_lock) || (h_lock && !old_hlock)) h_offset = h2-h1; // if not locked, or just became locked
      al_draw_textf(mFont.pr8, mColor.pc[15], xa+246, ya+bts+(20-bts)/2, 0, "offset:%3.2f", h_offset);
      if (mWidget.colorClickSlider(4, xa, ya, xb, bts, h1, h1_hover, h1_hover_val,   c1, changed) && h_lock) h2 = h1 + h_offset;
      if (h_lock) // second control is display only and value follows first one if hovered
      {
         h2_hover_val = h1_hover_val + h_offset;
         mWidget.colorClickSlider(4, xa, ya, xb, bts, h2_hover_val, h2_hover, h2_hover_val,   c2, changed, true);
      }
      else mWidget.colorClickSlider(4, xa, ya, xb, bts, h2, h2_hover, h2_hover_val,   c2, changed);
      ya += 8;


      // sat
      int old_slock = s_lock;
      mWidget.mCheckBox(1, xa+246, xb,  1,ya,bts, -1, s_lock, "lock", 15, 15);
      if ((!s_lock) || (s_lock && !old_slock)) s_offset = s2-s1; // if not locked, or just became locked
      al_draw_textf(mFont.pr8, mColor.pc[15], xa+246, ya+bts+(20-bts)/2, 0, "offset:%3.2f", s_offset);
      if (mWidget.colorClickSlider(5, xa, ya, xb, bts, s1, s1_hover, s1_hover_val,   c1, changed) && s_lock) s2 = s1 + s_offset;
      if (s_lock) // second control is display only and value follows first one if hovered
      {
         s2_hover_val = s1_hover_val + s_offset;
         mWidget.colorClickSlider(5, xa, ya, xb, bts, s2_hover_val, s2_hover, s2_hover_val,   c2, changed, true);
      }
      else mWidget.colorClickSlider(5, xa, ya, xb, bts, s2, s2_hover, s2_hover_val,   c2, changed);
      ya += 8;

      // light
      int old_llock = l_lock;
      mWidget.mCheckBox(1, xa+246, xb,  1,ya,bts, -1, l_lock, "lock", 15, 15);
      if ((!l_lock) || (l_lock && !old_llock)) l_offset = l2-l1; // if not locked, or just became locked
      al_draw_textf(mFont.pr8, mColor.pc[15], xa+246, ya+bts+(20-bts)/2, 0, "offset:%3.2f", l_offset);
      if (mWidget.colorClickSlider(6, xa, ya, xb, bts, l1, l1_hover, l1_hover_val,   c1, changed) && l_lock) l2 = l1 + l_offset;
      if (l_lock) // second control is display only and value follows first one if hovered
      {
         l2_hover_val = l1_hover_val + l_offset;
         mWidget.colorClickSlider(6, xa, ya, xb, bts, l2_hover_val, l2_hover, l2_hover_val,   c2, changed, true);
      }
      else mWidget.colorClickSlider(6, xa, ya, xb, bts, l2, l2_hover, l2_hover_val,   c2, changed);
      ya += 8;


      // steps
      mWidget.colorClickSlider(7, xa, ya, xb, bts, steps, na, steps_hover_val, c2, changed);
      ya += 8;

      // round
      mWidget.colorClickSlider(8, xa, ya, xb, bts, round, na, round_hover_val, c2, changed);

      showLiveTile(400, 8, type, solid_source_tile_index, h1_hover_val, h2_hover_val, s1_hover_val, s2_hover_val, l1_hover_val, l2_hover_val, steps_hover_val, round_hover_val);


      if (changed)
      {
         if (type == 0) create_tileset_frame(base_tile_index, h1, h2, s1, s2, l1, l2, steps, round);
         if (type == 1) create_tileset_solid(base_tile_index, solid_source_tile_index, h1, h2, s1, s2, l1, l2, steps, round);
         if (type == 2) create_tileset_extended(base_tile_index, h1, h2, s1, s2, l1, l2, steps, round);
         mBitmap.load_tiles();
      }

      if (mInput.key[ALLEGRO_KEY_ESCAPE][0])
      {
         while (mInput.key[ALLEGRO_KEY_ESCAPE][0]) mEventQueue.proc(1);
         quit = 1;
      }
   }
}




