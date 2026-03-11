#include "mwTileSets.h"
mwTileSets mTileSets;

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



}



// extended set
void mwTileSets::constructExtendedSet(std::string name, int i)
{
   constructEmptySet();

   ts.name              = name;
   ts.extendedMode      = 1;
   ts.startIndex        = i;
   ts.endIndex          = i+63;

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
   loaded = 1;

   constructExtendedSet("pipes", 256);

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



bool mwTileSets::findTileFrameSet(struct tileSet &t, int tile)
{
   for (auto ts : tileSets)
      if (tile >= ts.startIndex && tile <= ts.endIndex && ts.frameMode)
      {
         t = ts;
         return true;
      }
   return false;
}


bool mwTileSets::findTileSolidSet(struct tileSet &t, int tile)
{
   for (auto ts : tileSets)
      if (tile >= ts.startIndex && tile <= ts.endIndex && ts.solidMode)
      {
         t = ts;
         return true;
      }
   return false;
}











// passed a rect to draw from level editor
// bx and by are tile positions in the level array
// rect is guaranteed to have bx1 < bx2 and by1 < by2
// if drawItem is part of a tile set, use appropriate tiles from that set
void mwTileSets::draw(int bx1, int bx2, int by1, int by2, int drawItemNum, int drawItemFlags, int level[][100])
{
   if (!loaded) init();

   int bw = bx2-bx1; // width
   int bh = by2-by1; // height

   int drawTile = drawItemNum; // default

   // single tile
   if (bw==0 && bh==0) level[bx1][by1] = drawTile | drawItemFlags; // single tile 1 x 1

   // vertical line
   if (bw==0 && bh>0)
   {
      int a = bx1;
      for (int b=by1; b<by2+1; b++) // cycle the range
      {
         drawTile = drawItemNum; // default
         // search for tile set
         for (const auto& s : tileSets)
            if (drawItemNum >= s.startIndex && drawItemNum <= s.endIndex && s.vlineMode)
            {
               drawTile = s.VLineM;
               if (b == by1) drawTile = s.VLineT;
               if (b == by2) drawTile = s.VLineB;
            }
         // draw the tile
         level[a][b] = drawTile |= drawItemFlags;
      }
   }

   // horizontal line
   if (bw>0 && bh==0)
   {
      int b = by1;
      for (int a=bx1; a<bx2+1; a++) // cycle the range
      {
         drawTile = drawItemNum; // default
         // search for tile set
         for (const auto& s : tileSets)
            if (drawItemNum >= s.startIndex && drawItemNum <= s.endIndex && s.hlineMode)
            {
               drawTile = s.HLineM;
               if (a == bx1) drawTile = s.HLineL;
               if (a == bx2) drawTile = s.HLineR;
            }
         // draw the tile
         level[a][b] = drawTile |= drawItemFlags;
      }
   }

   // rectangle where w and h are both > 0
   if (bw>0 && bh>0)
   {
      struct tileSet s;
      // get draw mode
      int drawMode = 0; // default
      if (findTileFrameSet(s, drawItemNum)) drawMode = 1; // frame
      if (findTileSolidSet(s, drawItemNum)) drawMode = 2; // solid

      for (int a=bx1; a<bx2+1; a++)
         for (int b=by1; b<by2+1; b++)
         {
            if (drawMode == 0)       drawTile = drawItemNum; // default
            if (drawMode == 1)
            {
                                     drawTile = -1;
               if (b == by1)         drawTile = s.FrameEdgeT;
               if (b == by2)         drawTile = s.FrameEdgeB;
               if (a == bx1)
               {
                  if      (b == by1) drawTile = s.FrameCornerTL;
                  else if (b == by2) drawTile = s.FrameCornerBL;
                  else               drawTile = s.FrameEdgeL;
               }
               if (a == bx2)
               {
                  if      (b == by1) drawTile = s.FrameCornerTR;
                  else if (b == by2) drawTile = s.FrameCornerBR;
                  else               drawTile = s.FrameEdgeR;
               }
            }
            if (drawMode == 2)
            {
                                     drawTile = s.SolidFill;
               if (b == by1)         drawTile = s.OuterEdgeT;
               if (b == by2)         drawTile = s.OuterEdgeB;
               if (a == bx1)
               {
                  if      (b == by1) drawTile = s.OuterCornerTL;
                  else if (b == by2) drawTile = s.OuterCornerBL;
                  else               drawTile = s.OuterEdgeL;
               }
               if (a == bx2)
               {
                  if      (b == by1) drawTile = s.OuterCornerTR;
                  else if (b == by2) drawTile = s.OuterCornerBR;
                  else               drawTile = s.OuterEdgeR;
               }
            }
            // draw the tile
            if (drawTile != -1) level[a][b] = drawTile |= drawItemFlags;
         } // end of cycle range
   }
}


// from tile helper
// do tiles match exactly, or are they part of the same set?
bool mwTileSets::compareTile(int rb, int cb, int set)
{
   if (!loaded) init();

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


