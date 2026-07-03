#include "pm.h"
#include "mwTileSets.h"
#include "mwInput.h"
#include "mwBitmap.h"
#include "mwBitmapTools.h"
#include "mwColor.h"
#include "mwDisplay.h"
#include "mwFont.h"
#include "mwEventQueue.h"
#include "mwLevel.h"
#include "mwMiscFnx.h"
#include "mwWidget.h"
#include "mwWindowManager.h"

mwTileSets mTileSets;

mwTileSets::mwTileSets() { init(); }

void mwTileSets::init()
{

   strcpy(type_name[0],  "none");
   strcpy(type_name[3],  "1x3 Platform");
   strcpy(type_name[4],  "3x1 Column");
   strcpy(type_name[6],  "3x2 Column");
   strcpy(type_name[8],  "Rainbow");
   strcpy(type_name[9],  "Girder Frame");
   strcpy(type_name[16], "16 Tileset");
   strcpy(type_name[24], "24 Tileset");
   strcpy(type_name[48], "48 Tileset");



   constructEmptySet();
   ts.name              = "none";
   tileSets.push_back(ts);


   construct48( 192, "purple bricks");
   construct48( 256, "purple pipes");
   construct48( 320, "red pipes");
   construct48( 384, "green pipes");
   construct48( 448, "blue pipes");

   construct24( 160, "new test");
   construct24( 736, "template");
   construct24( 768, "alien machine");
   construct24( 800, "alien dark");

   construct16( 688 ,"screen");
   construct16( 720 ,"wrought iron");
   construct16( 832 ,"wires");
   construct16( 880, "white bricks");

   construct16( 896, "brain");
   construct16( 928, "grey bricks");
   construct16( 960, "brown bricks");
   construct16( 992, "fat fuzzy pipe");

   construct16( 912, "dirt grass");
   construct16( 864, "red dirt grass");
   construct16( 944, "red dirt");
   construct16( 976, "white rock");
   construct16( 1008,"purple blocks");

   construct8(  704, "rainbows");
   construct8(  712, "rainbows2");

   // 3x1 single hline (platforms)
   construct3x1(585, "brown bricks");
   construct3x1(579, "curved floater");
   construct3x1(582, "grey floater");
   construct3x1(564, "gold pipe");
   construct3x1(567, "gold beam");
   construct3x1(570, "gold wood");
   construct3x1(573, "green platform");

   // 1x3 single vlines (columns)
   construct1x3(544, "gold column");
   construct1x3(548, "grey pipe");
   construct1x3(552, "orange");
   construct1x3(556, "grey dogbone");
   construct1x3(560, "grey hex pipe");

   construct1x3(588, "brown rough");
   construct1x3(592, "gold column");
   construct1x3(596, "orange pipe");
   construct1x3(600, "blue");
   construct1x3(604, "grey holes");
   construct1x3(635, "white chain");

   // 2x3 double vlines (columns)
   construct6(  512, "gold brick");
   construct6(  518, "orange vent");
   construct6(  524, "orange rivet");
   construct6(  530, "grey blue pipe");
   construct6(  536, "green pipe");

   // combo hline vline frames
   construct9(  608, "grey hole");
   construct9(  617, "grey cross");
   construct9(  626, "grey rivet");
   construct9(  672, "red cross");



   // printf("num of tilesets:%d\n", (int) tileSets.size());
   // for (auto ts : tileSets) printf("%s %d - %d\n", ts.name.c_str(), ts.startIndex, ts.endIndex);

}


void mwTileSets::constructEmptySet()
{
   ts.name = "";
   ts.startIndex        = 0;
   ts.endIndex          = 0;
   ts.tileSetType       = 0;

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

}




// new 48
void mwTileSets::construct48(int i, std::string name)
{
   constructEmptySet();

   ts.name              = name;
   ts.startIndex        = i;
   ts.endIndex          = i + 47;
   ts.tileSetType       = 48;

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

   tileSets.push_back(ts);
}







// new 24 tileset
void mwTileSets::construct24(int i, std::string name)
{
   constructEmptySet();

   ts.name            = name;
   ts.startIndex      = i;
   ts.endIndex        = i+23;
   ts.tileSetType     = 24;
   ts.Single          = i+1;
   ts.SolidFill       = i;

   ts.HLineL          = i + 2;
   ts.HLineM          = i + 3;
   ts.HLineR          = i + 4;
   ts.VLineT          = i + 5;
   ts.VLineM          = i + 6;
   ts.VLineB          = i + 7;

   ts.OuterCornerTL   = i + 8;
   ts.OuterCornerTR   = i + 9;
   ts.OuterCornerBL   = i + 10;
   ts.OuterCornerBR   = i + 11;
   ts.OuterEdgeL      = i + 12;
   ts.OuterEdgeR      = i + 13;
   ts.OuterEdgeT      = i + 14;
   ts.OuterEdgeB      = i + 15;

   ts.FrameCornerTL   = i + 16;
   ts.FrameCornerTR   = i + 17;
   ts.FrameCornerBL   = i + 18;
   ts.FrameCornerBR   = i + 19;

   ts.FrameEdgeL      = i + 13;
   ts.FrameEdgeT      = i + 15;
   ts.FrameEdgeR      = i + 12;
   ts.FrameEdgeB      = i + 14;

   ts.InnerCornerTL   = i + 16;
   ts.InnerCornerTR   = i + 17;
   ts.InnerCornerBL   = i + 18;
   ts.InnerCornerBR   = i + 19;

   ts.InnerEdgeR      = i + 20;
   ts.InnerEdgeB      = i + 21;
   ts.InnerEdgeL      = i + 22;
   ts.InnerEdgeT      = i + 23;

   tileSets.push_back(ts);
}



// new 16 tileset
void mwTileSets::construct16(int i, std::string name)
{
   constructEmptySet();
   ts.name            = name;
   ts.startIndex      = i;
   ts.endIndex        = i+15;

   ts.tileSetType     = 16;

   ts.Single          = i+1;
   ts.SolidFill       = i;

   ts.HLineL          = i + 2;
   ts.HLineM          = i + 3;
   ts.HLineR          = i + 4;
   ts.VLineT          = i + 5;
   ts.VLineM          = i + 6;
   ts.VLineB          = i + 7;

   ts.OuterCornerTL   = i + 8;
   ts.OuterCornerTR   = i + 9;
   ts.OuterCornerBL   = i + 10;
   ts.OuterCornerBR   = i + 11;

   ts.OuterEdgeL      = i + 12;
   ts.OuterEdgeR      = i + 13;
   ts.OuterEdgeT      = i + 14;
   ts.OuterEdgeB      = i + 15;

   ts.FrameCornerTL   = i + 8;
   ts.FrameCornerTR   = i + 9;
   ts.FrameCornerBL   = i + 10;
   ts.FrameCornerBR   = i + 11;

   ts.FrameEdgeL      = i + 6;
   ts.FrameEdgeR      = i + 6;
   ts.FrameEdgeT      = i + 3;
   ts.FrameEdgeB      = i + 3;

   tileSets.push_back(ts);
}



// 8 tileset for rainbows only
void mwTileSets::construct8(int i, std::string name)
{
   constructEmptySet();
   ts.name        = name;
   ts.startIndex  = i;
   ts.endIndex    = i + 7;
   ts.tileSetType = 8;

   ts.FrameCornerTL   = i + 0;
   ts.FrameCornerTR   = i + 1;
   ts.FrameCornerBL   = i + 2;
   ts.FrameCornerBR   = i + 3;
   ts.FrameEdgeL      = i + 4;
   ts.FrameEdgeR      = i + 5;
   ts.FrameEdgeT      = i + 6;
   ts.FrameEdgeB      = i + 7;

   tileSets.push_back(ts);
}

// 6 tileset for 2x3 columns
void mwTileSets::construct6(int i, std::string name)
{
   constructEmptySet();
   ts.name        = name;
   ts.startIndex  = i;
   ts.endIndex    = i + 5;
   ts.tileSetType = 6;
   ts.OuterCornerTL   = i + 0;
   ts.OuterCornerTR   = i + 1;
   ts.OuterEdgeL      = i + 2;
   ts.OuterEdgeR      = i + 3;
   ts.OuterCornerBL   = i + 4;
   ts.OuterCornerBR   = i + 5;
   tileSets.push_back(ts);
}

// combo hline and vline set to draw frame
void mwTileSets::construct9(int i, std::string name)
{
   constructEmptySet();
   ts.name        = name;
   ts.startIndex  = i;
   ts.endIndex    = i + 8;
   ts.tileSetType = 9;

   ts.HLineL          = i + 0;
   ts.HLineM          = i + 1;
   ts.HLineR          = i + 2;
   ts.VLineT          = i + 3;
   ts.VLineM          = i + 4;
   ts.VLineB          = i + 5;

   ts.FrameCornerTL   = i + 0;
   ts.FrameCornerTR   = i + 2;
   ts.FrameCornerBL   = i + 0;
   ts.FrameCornerBR   = i + 2;
   ts.FrameEdgeL      = i + 4;
   ts.FrameEdgeR      = i + 4;
   ts.FrameEdgeT      = i + 1;
   ts.FrameEdgeB      = i + 1;

   tileSets.push_back(ts);
}

// 1x3 vline
void mwTileSets::construct1x3(int i, std::string name)
{
   constructEmptySet();
   ts.name            = name;
   ts.startIndex      = i;
   ts.endIndex        = i + 2;
   ts.tileSetType     = 4;
   ts.VLineT          = i + 0;
   ts.VLineM          = i + 1;
   ts.VLineB          = i + 2;
   tileSets.push_back(ts);
}

// 3x1 hline
void mwTileSets::construct3x1(int i, std::string name)
{
   constructEmptySet();
   ts.name            = name;
   ts.startIndex      = i;
   ts.endIndex        = i + 2;
   ts.tileSetType     = 3;
   ts.HLineL          = i + 0;
   ts.HLineM          = i + 1;
   ts.HLineR          = i + 2;
   tileSets.push_back(ts);
}





bool mwTileSets::findTileSetContainingName(struct tileSet &t, std::string n)
{
   for (auto tmp: tileSets)
      if (tmp.name == n)
      {
         t = tmp;
         return true;
      }
   return false;
}



bool mwTileSets::findTileSetContainingIndex(struct tileSet &t, int tileIndex)
{
   for (auto tmp : tileSets)
      if (tileIndex >= tmp.startIndex && tileIndex <= tmp.endIndex)
      {
         t = tmp;
         return true;
      }
   return false;
}




void mwTileSets::drawTile(int x, int y, int tileNum, int drawItemFlags, int drawTileMode, bool preview)
{
   if (preview) // draw tile on level_buffer
   {
      al_draw_filled_rectangle(x*20, y*20, x*20+20, y*20+20, mColor.Black);
      al_draw_bitmap(mBitmap.btile[tileNum], x*20, y*20, 0);
   }
   else // change tile in level array
   {
      // change both tile and flags
      if (drawTileMode == 1) mLevel.l[x][y] = tileNum | drawItemFlags;

      // change only tile portion (lower 10 bits)
      if (drawTileMode == 2)
      {
         mLevel.l[x][y] &= 0b11111111111111111111110000000000; // clear lower bits
         mLevel.l[x][y] |= tileNum; // merge tileNum
      }

      // change only flags portion (upper 22 bits)
      if (drawTileMode == 3)
      {
         mLevel.l[x][y] &= 0b00000000000000000000001111111111; // clear upper bits
         mLevel.l[x][y] |= drawItemFlags; // merge drawItemFlags
      }
   }
}




void mwTileSets::drawRectHelper(int s, int t, int b, int l, int r, int tl, int tr, int bl, int br, bool preview)
{
   int bx1 = mWM.bx1;
   int by1 = mWM.by1;
   int bx2 = mWM.bx2;
   int by2 = mWM.by2;
   mMiscFnx.ensure_xy1_less_than_xy2(bx1, by1, bx2, by2); // swap if wrong order
   int drawItem = mWM.mW[1].draw_item_num;
   int drawTileMode = mWM.mW[1].em_draw_tile_mode;
   int drawItemFlags = drawItem & PM_BTILE_ALL_FLAGS;

   for (int x=bx1; x<=bx2; x++)
      for (int y=by1; y<=by2; y++)
      {
         int drawTileNum = s;
         if (y == by1)         drawTileNum = t;
         if (y == by2)         drawTileNum = b;
         if (x == bx1)
         {
            if      (y == by1) drawTileNum = tl;
            else if (y == by2) drawTileNum = bl;
            else               drawTileNum = l;
         }
         if (x == bx2)
         {
            if      (y == by1) drawTileNum = tr;
            else if (y == by2) drawTileNum = br;
            else               drawTileNum = r;
         }
         if (drawTileNum != -1) drawTile(x, y, drawTileNum, drawItemFlags, drawTileMode, preview);
      }
}

void mwTileSets::drawRectHelperHline(struct tileSet ts, bool preview)
{
   int bx1 = mWM.bx1;
   int by1 = mWM.by1;
   int bx2 = mWM.bx2;
   int by2 = mWM.by2;
   mMiscFnx.ensure_xy1_less_than_xy2(bx1, by1, bx2, by2); // swap if wrong order
   int drawItem = mWM.mW[1].draw_item_num;
   int drawTileMode = mWM.mW[1].em_draw_tile_mode;
   int drawItemFlags = drawItem & PM_BTILE_ALL_FLAGS;

   for (int y=by1; y<=by2; y++)
      for (int x=bx1; x<=bx2; x++)
      {
         int drawTileNum = ts.HLineM;
         if (x == bx1) drawTileNum = ts.HLineL;
         if (x == bx2) drawTileNum = ts.HLineR;
         drawTile(x, y, drawTileNum, drawItemFlags, drawTileMode, preview);
      }

}

void mwTileSets::drawRectHelperVline(struct tileSet ts, bool preview)
{
   int bx1 = mWM.bx1;
   int by1 = mWM.by1;
   int bx2 = mWM.bx2;
   int by2 = mWM.by2;
   mMiscFnx.ensure_xy1_less_than_xy2(bx1, by1, bx2, by2); // swap if wrong order
   int drawItem = mWM.mW[1].draw_item_num;
   int drawTileMode = mWM.mW[1].em_draw_tile_mode;
   int drawItemFlags = drawItem & PM_BTILE_ALL_FLAGS;

   for (int x=bx1; x<=bx2; x++)
      for (int y=by1; y<=by2; y++)
      {
         int drawTileNum = ts.VLineM;
         if (y == by1) drawTileNum = ts.VLineT;
         if (y == by2) drawTileNum = ts.VLineB;
         drawTile(x, y, drawTileNum, drawItemFlags, drawTileMode, preview);
      }
}


void mwTileSets::drawRectHelperPattern(struct tileSet ts, bool preview)
{
   int bx1 = mWM.bx1;
   int by1 = mWM.by1;
   int bx2 = mWM.bx2;
   int by2 = mWM.by2;
   int drawItem = mWM.mW[1].draw_item_num;
   int drawTileMode = mWM.mW[1].em_draw_tile_mode;
   int drawItemFlags = drawItem & PM_BTILE_ALL_FLAGS;

   mMiscFnx.ensure_xy1_less_than_xy2(bx1, by1, bx2, by2); // swap if wrong order


   // set up 3x3 pattern array
   int p[3][3];
   p[0][0] = ts.InnerCornerTL;
   p[1][0] = ts.InnerEdgeT;
   p[2][0] = ts.InnerCornerTR;
   p[0][1] = ts.InnerEdgeL;
   p[1][1] = 0; // middle tile
   p[2][1] = ts.InnerEdgeR;
   p[0][2] = ts.InnerCornerBL;
   p[1][2] = ts.InnerEdgeB;
   p[2][2] = ts.InnerCornerBR;


   // set middle tile
   mMiscFnx.enforce_limits_with_rollover(altDrawRectModePatternFill, 0, 7);
   if (altDrawRectModePatternFill == 0) p[1][1] = 0;
   if (altDrawRectModePatternFill == 1) p[1][1] = ts.SolidFill;
   if (altDrawRectModePatternFill == 2) p[1][1] = ts.InnerEdgeT;
   if (altDrawRectModePatternFill == 3) p[1][1] = ts.InnerEdgeB;
   if (altDrawRectModePatternFill == 4) p[1][1] = ts.HLineM;
   if (altDrawRectModePatternFill == 5) p[1][1] = ts.InnerEdgeL;
   if (altDrawRectModePatternFill == 6) p[1][1] = ts.InnerEdgeR;
   if (altDrawRectModePatternFill == 7) p[1][1] = ts.VLineM;

   // set pattern size
   mMiscFnx.enforce_limits(altDrawRectModePatternWidth, 1, 6);
   mMiscFnx.enforce_limits(altDrawRectModePatternHeight, 1, 6);
   int xs = altDrawRectModePatternWidth;
   int ys = altDrawRectModePatternHeight;


   // iterate the draw area
   for (int x=bx1; x<=bx2; x++)
      for (int y=by1; y<=by2; y++)
      {
         int drawTileNum = 0; // middle

         // offset into pattern
         int xi = ((x-bx1) % xs);
         int yi = ((y-by1) % ys);

         if (xi == 0) // first column
         {
                            drawTileNum = p[0][1]; // middle
            if (yi == 0)    drawTileNum = p[0][0]; // top
            if (yi == ys-1) drawTileNum = p[0][2]; // bottom
         }

         if (xi == xs-1) // last column
         {
                            drawTileNum = p[2][1]; // middle
            if (yi == 0)    drawTileNum = p[2][0]; // top
            if (yi == ys-1) drawTileNum = p[2][2]; // bottom
         }

         if (xi > 0 && xi < xs-1) // middle columns
         {
                            drawTileNum = p[1][1]; // middle
            if (yi == 0)    drawTileNum = p[1][0]; // top
            if (yi == ys-1) drawTileNum = p[1][2]; // bottom
         }

         drawTile(x, y, drawTileNum, drawItemFlags, drawTileMode, preview);
      }
}





// uses bx1, by1, bx2, bx2 from level editor
// replaces blocks in level, or for preview draws on level buffer
// if drawItem is part of a tile set, use appropriate tiles from that set
// calls drawTile to actually do the draw (or preview)

void mwTileSets::drawRect(bool preview)
{
   // get local copies
   int bx1 = mWM.bx1;
   int by1 = mWM.by1;
   int bx2 = mWM.bx2;
   int by2 = mWM.by2;
   mMiscFnx.ensure_xy1_less_than_xy2(bx1, by1, bx2, by2); // swap if wrong order

   int bw = bx2 - bx1;
   int bh = by2 - by1;



   int drawItem      = mWM.mW[1].draw_item_num;
   int drawTileMode  = mWM.mW[1].em_draw_tile_mode;
   int drawItemNum   = drawItem & 1023;
   int drawItemFlags = drawItem & PM_BTILE_ALL_FLAGS;



   // clear variables
   findTileSetContainingName(ts, "none");
   altTextLine1 = "";
   altTextLine2 = "";
   altTextLine3 = "";



   // search for tileset
   findTileSetContainingIndex(ts, drawItemNum);


   // selection is a single block
   if (bw == 0 && bh == 0)
   {
      altTextLine1 = "Single Tile";
      drawTile(bx1, by1, drawItemNum, drawItemFlags, drawTileMode, preview);
      return;
   }



   if (ts.tileSetType == 3)
   {
      altTextLine1 = "Rectangle - Horizontal Lines";
      drawRectHelperHline(ts, preview);
   }
   if (ts.tileSetType == 4)
   {
      altTextLine1 = "Rectangle - Vertical Lines";
      drawRectHelperVline(ts, preview);
   }


   if (ts.tileSetType == 6)
   {
      altTextLine1 = "Rectangle - 2x3 Column";
      drawRectHelper(-1, 0, 0, ts.OuterEdgeL, ts.OuterEdgeR, ts.OuterCornerTL, ts.OuterCornerTR, ts.OuterCornerBL, ts.OuterCornerBR, preview);
      return;
   }
   if (ts.tileSetType == 8)
   {
      altTextLine1 = "Frame - Rainbows";
      drawRectHelper(-1, ts.FrameEdgeT, ts.FrameEdgeB, ts.FrameEdgeL, ts.FrameEdgeR, ts.FrameCornerTL, ts.FrameCornerTR, ts.FrameCornerBL, ts.FrameCornerBR, preview); // rainbows
      return;
   }




   // selection is a vertical line
   if (bw == 0 && bh > 0)
   {
      if (ts.tileSetType == 3) ts.tileSetType = 0; // trying to draw vline from hline set...clear tileset to draw only specific tile at end
      else
      {
         altTextLine1 = "Vertical Line";
         drawRectHelperVline(ts, preview);
      }
      if (ts.tileSetType != 0) return; // only return if we drew something here, otherwise let it pass through to specific tile at end
   }

   // selection is a horizontal line
   if (bh == 0 && bw > 0)
   {
      if (ts.tileSetType == 4) ts.tileSetType = 0; // trying to draw hline from vline set...clear tileset to draw only specific tile at end
      else
      {
         altTextLine1 = "Horizontal Line";
         drawRectHelperHline(ts, preview);
      }
      if (ts.tileSetType != 0) return;  // only return if we drew something here, otherwise let it pass through to specific tile at end
   }





   if (ts.tileSetType == 9) // hline vline frame
   {
      mMiscFnx.enforce_limits_with_rollover(altDrawRectMode, 0, 3);
      if (altDrawRectMode == 0)
      {
         altTextLine1 = "Frame - Full Horizontal";
         drawRectHelper(-1, ts.FrameEdgeT, ts.FrameEdgeB, ts.FrameEdgeL, ts.FrameEdgeR, ts.HLineL, ts.HLineR, ts.HLineL, ts.HLineR, preview);

         if (bh == 2) // make columns single block
         {
            drawTile(bx1, by1+1, ts.HLineL+7, drawItemFlags, drawTileMode, preview);
            drawTile(bx2, by1+1, ts.HLineL+7, drawItemFlags, drawTileMode, preview);
         }
         if (bh > 2) // fix column top and bottom
         {
            drawTile(bx1, by1+1, ts.VLineT, drawItemFlags, drawTileMode, preview);
            drawTile(bx1, by2-1, ts.VLineB, drawItemFlags, drawTileMode, preview);
            drawTile(bx2, by1+1, ts.VLineT, drawItemFlags, drawTileMode, preview);
            drawTile(bx2, by2-1, ts.VLineB, drawItemFlags, drawTileMode, preview);
         }

      }
      if (altDrawRectMode == 1)
      {
         altTextLine1 = "Frame - Full Vertical";
         drawRectHelper(-1, ts.FrameEdgeT, ts.FrameEdgeB, ts.FrameEdgeL, ts.FrameEdgeR, ts.VLineT, ts.VLineT, ts.VLineB, ts.VLineB, preview);
         if (bw == 2) // make platforms single block
         {
            drawTile(bx1+1, by1, ts.HLineL+6, drawItemFlags, drawTileMode, preview);
            drawTile(bx1+1, by2, ts.HLineL+6, drawItemFlags, drawTileMode, preview);
         }
         if (bw > 2) // fix platform right and left
         {
            drawTile(bx1+1, by1, ts.HLineL, drawItemFlags, drawTileMode, preview);
            drawTile(bx2-1, by1, ts.HLineR, drawItemFlags, drawTileMode, preview);
            drawTile(bx1+1, by2, ts.HLineL, drawItemFlags, drawTileMode, preview);
            drawTile(bx2-1, by2, ts.HLineR, drawItemFlags, drawTileMode, preview);
         }
      }
      if (altDrawRectMode == 2)
      {
         altTextLine1 = "Rectangle - Horizontal Lines";
         drawRectHelperHline(ts, preview);
      }
      if (altDrawRectMode == 3)
      {
         altTextLine1 = "Rectangle - Vertical Lines";
         drawRectHelperVline(ts, preview);
      }
   }





   if (ts.tileSetType == 16) // 16 tilesets
   {
      mMiscFnx.enforce_limits_with_rollover(altDrawRectMode, 0, 4);

      if (altDrawRectMode == 0)
      {
         altTextLine1 = "Rectangle - Solid";
         drawRectHelper(ts.SolidFill, ts.OuterEdgeT, ts.OuterEdgeB, ts.OuterEdgeL, ts.OuterEdgeR, ts.OuterCornerTL, ts.OuterCornerTR, ts.OuterCornerBL, ts.OuterCornerBR, preview); // 16 solid
      }
      if (altDrawRectMode == 1)
      {
         altTextLine1 = "Rectangle - Frame";
         drawRectHelper(-1,           ts.FrameEdgeT, ts.FrameEdgeB, ts.FrameEdgeL, ts.FrameEdgeR, ts.FrameCornerTL, ts.FrameCornerTR, ts.FrameCornerBL, ts.FrameCornerBR, preview); // 16 frame
      }
      if (altDrawRectMode == 2)
      {
         altTextLine1 = "Rectangle - Frame Alt";
         drawRectHelper(-1,           ts.OuterEdgeT, ts.OuterEdgeB, ts.OuterEdgeL, ts.OuterEdgeR, ts.OuterCornerTL, ts.OuterCornerTR, ts.OuterCornerBL, ts.OuterCornerBR, preview); // 16 frame alt
      }
      if (altDrawRectMode == 3)
      {
         altTextLine1 = "Rectangle - Horizontal Lines";
         drawRectHelperHline(ts, preview);
      }
      if (altDrawRectMode == 4)
      {
         altTextLine1 = "Rectangle - Vertical Lines";
         drawRectHelperVline(ts, preview);
      }
   }


   if (ts.tileSetType == 24) // 24 tilesets
   {
      mMiscFnx.enforce_limits_with_rollover(altDrawRectMode, 0, 6);

      if (altDrawRectMode == 0)
      {
         altTextLine1 = "Frame - Outer";
         drawRectHelper(-1, ts.OuterEdgeT, ts.OuterEdgeB, ts.OuterEdgeL, ts.OuterEdgeR, ts.OuterCornerTL, ts.OuterCornerTR, ts.OuterCornerBL, ts.OuterCornerBR, preview);
      }
      if (altDrawRectMode == 1)
      {
         altTextLine1 = "Frame - Outer Alt";
         drawRectHelper(-1, ts.InnerEdgeB, ts.InnerEdgeT, ts.InnerEdgeR, ts.InnerEdgeL, ts.OuterCornerTL, ts.OuterCornerTR, ts.OuterCornerBL, ts.OuterCornerBR, preview);
      }
      if (altDrawRectMode == 2)
      {
         altTextLine1 = "Frame - Inner";
         drawRectHelper(-1, ts.FrameEdgeT, ts.FrameEdgeB, ts.FrameEdgeL, ts.FrameEdgeR, ts.FrameCornerTL, ts.FrameCornerTR, ts.FrameCornerBL, ts.FrameCornerBR, preview);
      }
      if (altDrawRectMode == 3)
      {
         altTextLine1 = "Frame - Inner Alt";
         drawRectHelper(-1, ts.InnerEdgeT, ts.InnerEdgeB, ts.InnerEdgeL, ts.InnerEdgeR, ts.FrameCornerTL, ts.FrameCornerTR, ts.FrameCornerBL, ts.FrameCornerBR, preview);
      }

      if (altDrawRectMode == 4)
      {
         altTextLine1 = "Rectangle - Pattern";
         altTextLine3 = "arrows PGUP PGDN to change pattern";

         char msg[80];
         sprintf(msg, "Pattern %dx%d Fill:%d", altDrawRectModePatternWidth, altDrawRectModePatternHeight, altDrawRectModePatternFill );
         altTextLine2 = msg;

         drawRectHelperPattern(ts, preview); // pattern
      }

      if (altDrawRectMode == 5)
      {
         altTextLine1 = "Rectangle - Horizontal Lines";
         drawRectHelperHline(ts, preview);
      }
      if (altDrawRectMode == 6)
      {
         altTextLine1 = "Rectangle - Vertical Lines";
         drawRectHelperVline(ts, preview);
      }
   }




   if (ts.tileSetType == 48) // 48 tilesets
   {
      mMiscFnx.enforce_limits_with_rollover(altDrawRectMode, 0, 6);
      if (altDrawRectMode == 0)
      {
         altTextLine1 = "Frame - Outer";
         drawRectHelper(-1, ts.OuterEdgeT, ts.OuterEdgeB, ts.OuterEdgeL, ts.OuterEdgeR, ts.OuterCornerTL, ts.OuterCornerTR, ts.OuterCornerBL, ts.OuterCornerBR, preview);
      }
      if (altDrawRectMode == 1)
      {
         altTextLine1 = "Frame - Single";
         drawRectHelper(-1, ts.FrameEdgeT, ts.FrameEdgeB, ts.FrameEdgeL, ts.FrameEdgeR, ts.FrameCornerTL, ts.FrameCornerTR, ts.FrameCornerBL, ts.FrameCornerBR, preview);
      }
      if (altDrawRectMode == 2)
      {
         altTextLine1 = "Frame - Inner";
         drawRectHelper(-1, ts.InnerEdgeT, ts.InnerEdgeB, ts.InnerEdgeL, ts.InnerEdgeR, ts.InnerCornerTL, ts.InnerCornerTR, ts.InnerCornerBL, ts.InnerCornerBR, preview);
      }
      if (altDrawRectMode == 3)
      {
         altTextLine1 = "Rectangle - Solid";
         drawRectHelper(ts.SolidFill, ts.OuterEdgeT, ts.OuterEdgeB, ts.OuterEdgeL, ts.OuterEdgeR, ts.OuterCornerTL, ts.OuterCornerTR, ts.OuterCornerBL, ts.OuterCornerBR, preview);
      }
      if (altDrawRectMode == 4)
      {
         altTextLine1 = "Rectangle - Solid Alt";
         drawRectHelper(ts.FrameCross, ts.FrameEdgeBTee, ts.FrameEdgeTTee, ts.FrameEdgeLTee,  ts.FrameEdgeRTee, ts.FrameCornerTL, ts.FrameCornerTR, ts.FrameCornerBL, ts.FrameCornerBR, preview);
      }

      if (altDrawRectMode == 5)
      {
         altTextLine1 = "Rectangle - Horizontal Lines";
         drawRectHelperHline(ts, preview);
      }
      if (altDrawRectMode == 6)
      {
         altTextLine1 = "Rectangle - Vertical Lines";
         drawRectHelperVline(ts, preview);
      }

   }


   if (ts.tileSetType == 0) // none
   {
      altTextLine1 = "Rectangle - Single Tile Fill";

      // if nothing else matched up to this point, fill with single tile
      for (int x=bx1; x<=bx2; x++)
         for (int y=by1; y<=by2; y++)
            drawTile(x, y, drawItemNum, drawItemFlags, drawTileMode, preview);

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



void mwTileSets::create_tileset_single_faded_rect(int bs, ALLEGRO_COLOR bc)
{
   printf("mwTileSets::create_tileset_single_faded_rect(%d);\n", bs);

   // start by finding what lch wants for color

   // extract rgb
   float r, g, b;
   al_unmap_rgb_f(bc, &r, &g, &b);

   // convert rgb to lch
   float l, c, h;
   al_color_rgb_to_lch(r, g, b, &l, &c, &h);
   printf("l:%f  c:%f  h:%f \n", l, c, h);

   // fill color array
   float l1 = l;
   float l2 = 0;

   float c1 = c;
   float c2 = 0;

   int steps = 10;
   int div = steps - 1;
   if (div < 1) div = 1;
   float linc = (l2 - l1) / div;
   float cinc = (c2 - c1) / div;

   for (int i=0; i<steps; i++)
      step_color_array[i] = al_color_lch(l1+i*linc, c1+i*cinc, h);
   step_color_array_size = steps;


   // create and clear bitmap to draw on
   ALLEGRO_BITMAP *tmp1 = al_create_bitmap(20, 20);
   al_set_target_bitmap(tmp1);
   al_clear_to_color(al_map_rgb(0,0,0));

   float x1 = 0.5;
   float y1 = 0.5;
   float x2 = 19.5;
   float y2 = 19.5;

   // draw rectangles
   for (float i=0; i<step_color_array_size; i+=1)
      al_draw_rectangle(x1+i, y1+i, x2-i, y2-i, step_color_array[(int)i], 1);


   // load tileset
   ALLEGRO_BITMAP *b1 = mBitmapTools.load_block_tiles_to_bitmap();
   al_set_target_bitmap(b1);

   // draw tile
   al_draw_bitmap_region(tmp1, 0,   0, 20, 20, (bs % 32)*22+1, (bs / 32)*22+1, 0);

   // save tileset
   mBitmapTools.save_bitmap_to_block_tiles_file(b1);



   al_destroy_bitmap(b1);
   al_destroy_bitmap(tmp1);

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


void mwTileSets::create_tileset_from_24_atomic(int bs)
{
   // load mega 8 bit tileset to copy from
   char b2_fn[100];
   char b2_fn2[100];
   //sprintf(b2_fn2, "bitmaps/Template_Tileset.png");      // pink
   //sprintf(b2_fn2, "bitmaps/Industrial_Tileset_1A.png"); // dark grey
   //sprintf(b2_fn2, "bitmaps/Industrial_Tileset_1B.png"); // blue orange
   sprintf(b2_fn2, "bitmaps/Industrial_Tileset_1C.png"); // brown and purple
   //sprintf(b2_fn2, "bitmaps/Industrial_Tileset_1D.png"); // light blue and green
   //sprintf(b2_fn2, "bitmaps/Industrial_Tileset_1E.png"); // light blue and orange
   //sprintf(b2_fn2, "bitmaps/Industrial_Tileset_1F.png"); // green blue pink





   // convert to 'ALLEGRO_FS_ENTRY' to get platform specific fully qualified path
   ALLEGRO_FS_ENTRY *FS_fname2 = al_create_fs_entry(b2_fn2);
   sprintf(b2_fn, "%s", al_get_fs_entry_name(FS_fname2));
   ALLEGRO_BITMAP *b2 = nullptr;

   b2 = al_load_bitmap(b2_fn);

   if (!b2)
   {
      mInput.m_err("Load Error");
      return;
   }

   // load block tiles from file to bitmap, so we can modify it
   ALLEGRO_BITMAP *b1 = mBitmapTools.load_block_tiles_to_bitmap();
   al_set_target_bitmap(b1);
   int s=bs;

   s = bs +  0;  al_draw_bitmap_region(b2, 20,  40, 20, 20, (s % 32)*22+1, (s / 32)*22+1, 0); // middle
   s = bs +  1;  al_draw_bitmap_region(b2, 60,   0, 20, 20, (s % 32)*22+1, (s / 32)*22+1, 0); // single

   s = bs +  2;  al_draw_bitmap_region(b2,  0,   0, 20, 20, (s % 32)*22+1, (s / 32)*22+1, 0); // hline l
   s = bs +  3;  al_draw_bitmap_region(b2, 20,   0, 20, 20, (s % 32)*22+1, (s / 32)*22+1, 0); // hline m
   s = bs +  4;  al_draw_bitmap_region(b2, 40,   0, 20, 20, (s % 32)*22+1, (s / 32)*22+1, 0); // hline r

   s = bs +  5;  al_draw_bitmap_region(b2, 60,  20, 20, 20, (s % 32)*22+1, (s / 32)*22+1, 0); // vline t
   s = bs +  6;  al_draw_bitmap_region(b2, 60,  40, 20, 20, (s % 32)*22+1, (s / 32)*22+1, 0); // vline m
   s = bs +  7;  al_draw_bitmap_region(b2, 60,  60, 20, 20, (s % 32)*22+1, (s / 32)*22+1, 0); // vline b

   s = bs +  8;  al_draw_bitmap_region(b2,  0,  20, 20, 20, (s % 32)*22+1, (s / 32)*22+1, 0); // tl
   s = bs +  9;  al_draw_bitmap_region(b2, 40,  20, 20, 20, (s % 32)*22+1, (s / 32)*22+1, 0); // tr
   s = bs + 10;  al_draw_bitmap_region(b2,  0,  60, 20, 20, (s % 32)*22+1, (s / 32)*22+1, 0); // bl
   s = bs + 11;  al_draw_bitmap_region(b2, 40,  60, 20, 20, (s % 32)*22+1, (s / 32)*22+1, 0); // br

   s = bs + 12;  al_draw_bitmap_region(b2,  0,  40, 20, 20, (s % 32)*22+1, (s / 32)*22+1, 0); // l
   s = bs + 13;  al_draw_bitmap_region(b2, 40,  40, 20, 20, (s % 32)*22+1, (s / 32)*22+1, 0); // r
   s = bs + 14;  al_draw_bitmap_region(b2, 20,  20, 20, 20, (s % 32)*22+1, (s / 32)*22+1, 0); // t
   s = bs + 15;  al_draw_bitmap_region(b2, 20,  60, 20, 20, (s % 32)*22+1, (s / 32)*22+1, 0); // b

   s = bs + 16;  al_draw_bitmap_region(b2, 80,   0, 20, 20, (s % 32)*22+1, (s / 32)*22+1, 0); // inner br
   s = bs + 17;  al_draw_bitmap_region(b2, 100,  0, 20, 20, (s % 32)*22+1, (s / 32)*22+1, 0); // inner bl
   s = bs + 18;  al_draw_bitmap_region(b2, 80,  20, 20, 20, (s % 32)*22+1, (s / 32)*22+1, 0); // inner tr
   s = bs + 19;  al_draw_bitmap_region(b2, 100, 20, 20, 20, (s % 32)*22+1, (s / 32)*22+1, 0); // inner tl

   s = bs + 20;  al_draw_bitmap_region(b2, 80,  40, 20, 20, (s % 32)*22+1, (s / 32)*22+1, 0); // inner br
   s = bs + 21;  al_draw_bitmap_region(b2, 100, 40, 20, 20, (s % 32)*22+1, (s / 32)*22+1, 0); // inner bl
   s = bs + 22;  al_draw_bitmap_region(b2, 80,  60, 20, 20, (s % 32)*22+1, (s / 32)*22+1, 0); // inner tr
   s = bs + 23;  al_draw_bitmap_region(b2, 100, 60, 20, 20, (s % 32)*22+1, (s / 32)*22+1, 0); // inner tl




   // save modified block tiles bitmap to file
   mBitmapTools.save_bitmap_to_block_tiles_file(b1);

   al_destroy_bitmap(b1);
   al_destroy_bitmap(b2);








}


void mwTileSets::create_tileset_from_16_mega(int bs, int b2_x, int b2_y)
{


   // load mega 8 bit tileset to copy from
   char b2_fn[100];
   char b2_fn2[100];
   sprintf(b2_fn2, "bitmaps/MegaPixellPack8bit_20scale.png");

   // convert to 'ALLEGRO_FS_ENTRY' to get platform specific fully qualified path
   ALLEGRO_FS_ENTRY *FS_fname2 = al_create_fs_entry(b2_fn2);
   sprintf(b2_fn, "%s", al_get_fs_entry_name(FS_fname2));
   ALLEGRO_BITMAP *b2 = nullptr;

   b2 = al_load_bitmap(b2_fn);

   if (!b2)
   {
      mInput.m_err("Load Error");
      return;
   }

   // load block tiles from file to bitmap, so we can modify it
   ALLEGRO_BITMAP *b1 = mBitmapTools.load_block_tiles_to_bitmap();
   al_set_target_bitmap(b1);
   int s=bs;

   // orange brown dirt
   //int b2_x = 12;
   //int b2_y = 17;

   // grey bricks
//   int b2_x = 16;
  // int b2_y = 21;

   // outlined grey
//   int b2_x = 28;
//   int b2_y = 21;




   s = bs +  0;  al_draw_bitmap_region(b2, (b2_x+2)*20, (b2_y+2)*20, 20, 20, (s % 32)*22+1, (s / 32)*22+1, 0); // middle
   s = bs +  1;  al_draw_bitmap_region(b2, (b2_x+0)*20, (b2_y+0)*20, 20, 20, (s % 32)*22+1, (s / 32)*22+1, 0); // single

   s = bs +  2;  al_draw_bitmap_region(b2, (b2_x+1)*20, (b2_y+0)*20, 20, 20, (s % 32)*22+1, (s / 32)*22+1, 0); // hline l
   s = bs +  3;  al_draw_bitmap_region(b2, (b2_x+2)*20, (b2_y+0)*20, 20, 20, (s % 32)*22+1, (s / 32)*22+1, 0); // hline m
   s = bs +  4;  al_draw_bitmap_region(b2, (b2_x+3)*20, (b2_y+0)*20, 20, 20, (s % 32)*22+1, (s / 32)*22+1, 0); // hline r

   s = bs +  5;  al_draw_bitmap_region(b2, (b2_x+0)*20, (b2_y+1)*20, 20, 20, (s % 32)*22+1, (s / 32)*22+1, 0); // vline t
   s = bs +  6;  al_draw_bitmap_region(b2, (b2_x+0)*20, (b2_y+2)*20, 20, 20, (s % 32)*22+1, (s / 32)*22+1, 0); // vline m
   s = bs +  7;  al_draw_bitmap_region(b2, (b2_x+0)*20, (b2_y+3)*20, 20, 20, (s % 32)*22+1, (s / 32)*22+1, 0); // vline b

   s = bs +  8;  al_draw_bitmap_region(b2, (b2_x+1)*20, (b2_y+1)*20, 20, 20, (s % 32)*22+1, (s / 32)*22+1, 0); // tl
   s = bs +  9;  al_draw_bitmap_region(b2, (b2_x+3)*20, (b2_y+1)*20, 20, 20, (s % 32)*22+1, (s / 32)*22+1, 0); // tr

   s = bs + 10;  al_draw_bitmap_region(b2, (b2_x+1)*20, (b2_y+3)*20, 20, 20, (s % 32)*22+1, (s / 32)*22+1, 0); // bl
   s = bs + 11;  al_draw_bitmap_region(b2, (b2_x+3)*20, (b2_y+3)*20, 20, 20, (s % 32)*22+1, (s / 32)*22+1, 0); // br

   s = bs + 12;  al_draw_bitmap_region(b2, (b2_x+1)*20, (b2_y+2)*20, 20, 20, (s % 32)*22+1, (s / 32)*22+1, 0); // l
   s = bs + 13;  al_draw_bitmap_region(b2, (b2_x+3)*20, (b2_y+2)*20, 20, 20, (s % 32)*22+1, (s / 32)*22+1, 0); // r

   s = bs + 14;  al_draw_bitmap_region(b2, (b2_x+2)*20, (b2_y+1)*20, 20, 20, (s % 32)*22+1, (s / 32)*22+1, 0); // t
   s = bs + 15;  al_draw_bitmap_region(b2, (b2_x+2)*20, (b2_y+3)*20, 20, 20, (s % 32)*22+1, (s / 32)*22+1, 0); // b


   // save modified block tiles bitmap to file
   mBitmapTools.save_bitmap_to_block_tiles_file(b1);

   al_destroy_bitmap(b1);
   al_destroy_bitmap(b2);







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


   // save modified block tiles bitmap to file
   mBitmapTools.save_bitmap_to_block_tiles_file(b1);

   al_destroy_bitmap(b1);
   al_destroy_bitmap(tmp1);
   al_destroy_bitmap(sb);
   al_destroy_bitmap(hf);
   al_destroy_bitmap(cf);

   // set default tile flags

   // set all to solid
   int flag = PM_BTILE_SOLID_PLAYER | PM_BTILE_SOLID_ENEMY | PM_BTILE_SOLID_ITEM | PM_BTILE_SOLID_PBUL | PM_BTILE_SOLID_EBUL;
   for (int i=bs; i<bs+48; i++)
      mBitmap.tileFlags[i] = flag;

   // unused
   for (int i=bs+48; i<bs+64; i++)
      mBitmap.tileFlags[i] = 0;

   mBitmap.save_sprit();
}



ALLEGRO_BITMAP * create_bitmap10_and_fill(ALLEGRO_BITMAP *tilemap, int sx, int sy)
{
   ALLEGRO_BITMAP *b = al_create_bitmap(10, 10);
   al_set_target_bitmap(b);
   al_clear_to_color(al_map_rgb(0,0,0));
   al_draw_bitmap_region(tilemap, sx, sy, 10, 10, 0, 0, 0);
   return b;
}


void prepare_tile_for_drawing(ALLEGRO_BITMAP *tile)
{
   al_set_target_bitmap(tile);
   al_clear_to_color(al_map_rgb(0,0,0));
}


void draw_tile_from_quarters(int index, ALLEGRO_BITMAP *tilemap, ALLEGRO_BITMAP *tl, ALLEGRO_BITMAP *tr, ALLEGRO_BITMAP *br, ALLEGRO_BITMAP *bl)
{
   ALLEGRO_BITMAP *b = al_create_bitmap(20, 20);
   al_set_target_bitmap(b);
   al_clear_to_color(al_map_rgb(0,0,0));
   al_draw_bitmap(tl,    0,   0,  0);
   al_draw_bitmap(tr,   10,   0,  0);
   al_draw_bitmap(br,   10,  10,  0);
   al_draw_bitmap(bl,    0,  10,  0);

   al_set_target_bitmap(tilemap);
   al_draw_bitmap(b, (index % 32)*22+1, (index / 32)*22+1, 0);

   al_destroy_bitmap(b);

}


void draw_tile_on_tilemap(ALLEGRO_BITMAP *tile, ALLEGRO_BITMAP *tilemap, int index)
{
   al_set_target_bitmap(tilemap);
   al_draw_bitmap(tile, (index % 32)*22+1, (index / 32)*22+1, 0);
}


void mwTileSets::create_tileset_extended2(int bs)
{

   // load tilemap from file to bitmap, so it can be modified
   ALLEGRO_BITMAP *b1 = mBitmapTools.load_block_tiles_to_bitmap();

   // load deep dungeon tileset to copy from
   char dd_ts_fn[100];

   // convert to 'ALLEGRO_FS_ENTRY' to get platform specific fully qualified path
   ALLEGRO_FS_ENTRY *FS_fname2 = al_create_fs_entry("bitmaps/DeepDungeonsTileset_20scale.png");
   sprintf(dd_ts_fn, "%s", al_get_fs_entry_name(FS_fname2));
   ALLEGRO_BITMAP *dd_ts_bmp = nullptr;
   dd_ts_bmp = al_load_bitmap(dd_ts_fn);
   if (!dd_ts_bmp)
   {
      mInput.m_err("Error loading tileset");
      return;
   }

   // single block tile bitmap for temp drawing
   ALLEGRO_BITMAP *sb = al_create_bitmap(20, 20);

   // solid color center
   prepare_tile_for_drawing(sb);
   al_clear_to_color(al_map_rgb(45, 33, 46));
   draw_tile_on_tilemap(sb, b1, bs + 46);

   // copy single line tiles
   int xo = 0;
   int yo = 80;

   prepare_tile_for_drawing(sb);
   al_draw_bitmap_region(dd_ts_bmp, xo + 60, yo + 60, 20, 20, 0, 0, 0);
   draw_tile_on_tilemap(sb, b1, bs +  0);  // single stand alone

   prepare_tile_for_drawing(sb);
   al_draw_bitmap_region(dd_ts_bmp, xo +  0, yo + 60, 20, 20, 0, 0, 0);
   draw_tile_on_tilemap(sb, b1, bs +  1);  // hline l

   prepare_tile_for_drawing(sb);
   al_draw_bitmap_region(dd_ts_bmp, xo + 20, yo + 60, 20, 20, 0, 0, 0);
   draw_tile_on_tilemap(sb, b1, bs +  2);  // hline m

   prepare_tile_for_drawing(sb);
   al_draw_bitmap_region(dd_ts_bmp, xo + 40, yo + 60, 20, 20, 0, 0, 0);
   draw_tile_on_tilemap(sb, b1, bs +  3);  // hline r

   prepare_tile_for_drawing(sb);
   al_draw_bitmap_region(dd_ts_bmp, xo + 60, yo +  0, 20, 20, 0, 0, 0);
   draw_tile_on_tilemap(sb, b1, bs +  4);  // vline t

   prepare_tile_for_drawing(sb);
   al_draw_bitmap_region(dd_ts_bmp, xo + 60, yo + 20, 20, 20, 0, 0, 0);
   draw_tile_on_tilemap(sb, b1, bs +  5);  // vline m

   prepare_tile_for_drawing(sb);
   al_draw_bitmap_region(dd_ts_bmp, xo + 60, yo + 40, 20, 20, 0, 0, 0);
   draw_tile_on_tilemap(sb, b1, bs +  6);  // vline b

   prepare_tile_for_drawing(sb);
   al_draw_bitmap_region(dd_ts_bmp, xo +  0, yo +  0, 20, 20, 0, 0, 0);
   draw_tile_on_tilemap(sb, b1, bs +  7);  // tr

   prepare_tile_for_drawing(sb);
   al_draw_bitmap_region(dd_ts_bmp, xo + 40, yo +  0, 20, 20, 0, 0, 0);
   draw_tile_on_tilemap(sb, b1, bs +  8);  // tl

   prepare_tile_for_drawing(sb);
   al_draw_bitmap_region(dd_ts_bmp, xo + 40, yo + 40, 20, 20, 0, 0, 0);
   draw_tile_on_tilemap(sb, b1, bs +  9);  // bl

   prepare_tile_for_drawing(sb);
   al_draw_bitmap_region(dd_ts_bmp, xo +  0, yo + 40, 20, 20, 0, 0, 0);
   draw_tile_on_tilemap(sb, b1, bs + 10);  // br

   // copy block edge tiles
   xo = 0;
   yo = 20;

   prepare_tile_for_drawing(sb);
   al_draw_bitmap_region(dd_ts_bmp, xo +  0, yo +  0, 20, 20, 0, 0, 0);
   draw_tile_on_tilemap(sb, b1, bs + 20);  // tr

   prepare_tile_for_drawing(sb);
   al_draw_bitmap_region(dd_ts_bmp, xo + 40, yo +  0, 20, 20, 0, 0, 0);
   draw_tile_on_tilemap(sb, b1, bs + 21);  // tl

   prepare_tile_for_drawing(sb);
   al_draw_bitmap_region(dd_ts_bmp, xo + 40, yo + 40, 20, 20, 0, 0, 0);
   draw_tile_on_tilemap(sb, b1, bs + 22);  // bl

   prepare_tile_for_drawing(sb);
   al_draw_bitmap_region(dd_ts_bmp, xo +  0, yo + 40, 20, 20, 0, 0, 0);
   draw_tile_on_tilemap(sb, b1, bs + 23);  // br

   prepare_tile_for_drawing(sb);
   al_draw_bitmap_region(dd_ts_bmp, xo +  0, yo + 20, 20, 20, 0, 0, 0);
   draw_tile_on_tilemap(sb, b1, bs + 24);  // edge l

   prepare_tile_for_drawing(sb);
   al_draw_bitmap_region(dd_ts_bmp, xo + 20, yo +  0, 20, 20, 0, 0, 0);
   draw_tile_on_tilemap(sb, b1, bs + 25);  // edge t

   prepare_tile_for_drawing(sb);
   al_draw_bitmap_region(dd_ts_bmp, xo + 40, yo + 20, 20, 20, 0, 0, 0);
   draw_tile_on_tilemap(sb, b1, bs + 26);  // edge r

   prepare_tile_for_drawing(sb);
   al_draw_bitmap_region(dd_ts_bmp, xo + 20, yo + 40, 20, 20, 0, 0, 0);
   draw_tile_on_tilemap(sb, b1, bs + 27);  // edge b


   // copy inner fill tiles
   xo = 60;
   yo = 0;

   prepare_tile_for_drawing(sb);
   al_draw_bitmap_region(dd_ts_bmp, xo +  0, yo +  0, 20, 20, 0, 0, 0);
   draw_tile_on_tilemap(sb, b1, bs + 16);  // tr

   prepare_tile_for_drawing(sb);
   al_draw_bitmap_region(dd_ts_bmp, xo + 20, yo +  0, 20, 20, 0, 0, 0);
   draw_tile_on_tilemap(sb, b1, bs + 17);  // tl

   prepare_tile_for_drawing(sb);
   al_draw_bitmap_region(dd_ts_bmp, xo + 20, yo + 20, 20, 20, 0, 0, 0);
   draw_tile_on_tilemap(sb, b1, bs + 18);  // bl

   prepare_tile_for_drawing(sb);
   al_draw_bitmap_region(dd_ts_bmp, xo +  0, yo + 20, 20, 20, 0, 0, 0);
   draw_tile_on_tilemap(sb, b1, bs + 19);  // br


   // this is the end of the simple ones that could be copied as 20x20 from source

   // now I need to make some smaller 10x10 pieces to construct other things


   xo = 0;
   yo = 80;

   // outer edge pieces
   ALLEGRO_BITMAP *outer_edge_t = create_bitmap10_and_fill(dd_ts_bmp, xo + 20, yo +  0);
   ALLEGRO_BITMAP *outer_edge_b = create_bitmap10_and_fill(dd_ts_bmp, xo + 20, yo + 50);
   ALLEGRO_BITMAP *outer_edge_l = create_bitmap10_and_fill(dd_ts_bmp, xo +  0, yo + 20);
   ALLEGRO_BITMAP *outer_edge_r = create_bitmap10_and_fill(dd_ts_bmp, xo + 50, yo + 20);

   // inner corner pieces
   ALLEGRO_BITMAP *inner_corner_tl= create_bitmap10_and_fill(dd_ts_bmp, xo + 10, yo + 10);
   ALLEGRO_BITMAP *inner_corner_tr= create_bitmap10_and_fill(dd_ts_bmp, xo + 40, yo + 10);
   ALLEGRO_BITMAP *inner_corner_br= create_bitmap10_and_fill(dd_ts_bmp, xo + 40, yo + 40);
   ALLEGRO_BITMAP *inner_corner_bl= create_bitmap10_and_fill(dd_ts_bmp, xo + 10, yo + 40);

   // solid pieces
   xo = 60;
   yo = 0;

   // outer corner pieces
   ALLEGRO_BITMAP *solid_outer_corner_tl = create_bitmap10_and_fill(dd_ts_bmp, xo +  0, yo +  0);
   ALLEGRO_BITMAP *solid_outer_corner_tr = create_bitmap10_and_fill(dd_ts_bmp, xo + 30, yo +  0);
   ALLEGRO_BITMAP *solid_outer_corner_br = create_bitmap10_and_fill(dd_ts_bmp, xo + 30, yo + 30);
   ALLEGRO_BITMAP *solid_outer_corner_bl = create_bitmap10_and_fill(dd_ts_bmp, xo +  0, yo + 30);

   // outer edge pieces
   ALLEGRO_BITMAP *solid_outer_edge_t = create_bitmap10_and_fill(dd_ts_bmp, xo + 10, yo +  0);
   ALLEGRO_BITMAP *solid_outer_edge_r = create_bitmap10_and_fill(dd_ts_bmp, xo + 30, yo + 10);
   ALLEGRO_BITMAP *solid_outer_edge_b = create_bitmap10_and_fill(dd_ts_bmp, xo + 10, yo + 30);
   ALLEGRO_BITMAP *solid_outer_edge_l = create_bitmap10_and_fill(dd_ts_bmp, xo +  0, yo + 10);



   // make tiles from pieces

   draw_tile_from_quarters(bs+15, b1, inner_corner_br,       inner_corner_bl,      inner_corner_tl,        inner_corner_tr);       // single width cross

   draw_tile_from_quarters(bs+11, b1, outer_edge_l,          inner_corner_bl,      inner_corner_tl,        outer_edge_l);          // single width tee l
   draw_tile_from_quarters(bs+13, b1, inner_corner_br,       outer_edge_r,         outer_edge_r,           inner_corner_tr);       // single width tee l
   draw_tile_from_quarters(bs+12, b1, outer_edge_t,          outer_edge_t,         inner_corner_tl,        inner_corner_tr);       // single width tee l
   draw_tile_from_quarters(bs+14, b1, inner_corner_br,       inner_corner_bl,      outer_edge_b,           outer_edge_b);          // single width tee l

   // tiles with single connection on one side and solid on opposite
   draw_tile_from_quarters(bs+28, b1, inner_corner_br,       solid_outer_edge_r,   solid_outer_edge_r,     inner_corner_tr);       // single width tee l - solid r
   draw_tile_from_quarters(bs+30, b1, solid_outer_edge_l,    inner_corner_bl,      inner_corner_tl,        solid_outer_edge_l);    // single width tee r - solid l
   draw_tile_from_quarters(bs+29, b1, inner_corner_br,       inner_corner_bl,      solid_outer_edge_b,     solid_outer_edge_b);    // single width tee t - solid b
   draw_tile_from_quarters(bs+31, b1, solid_outer_edge_t,    solid_outer_edge_t,   inner_corner_tl,        inner_corner_tr);       // single width tee b - solid t

   // tiles with outer frame on one side, other side with notch and inner (CW)
   draw_tile_from_quarters(bs+32, b1, outer_edge_l,          inner_corner_bl,       solid_outer_corner_br, outer_edge_l);          // outer frame side l
   draw_tile_from_quarters(bs+33, b1, outer_edge_t,          outer_edge_t,          inner_corner_tl,       solid_outer_corner_bl); // outer frame side t
   draw_tile_from_quarters(bs+34, b1, solid_outer_corner_tl, outer_edge_r,          outer_edge_r,          inner_corner_tr);       // outer frame side r
   draw_tile_from_quarters(bs+35, b1, inner_corner_br,       solid_outer_corner_tr, outer_edge_b,          outer_edge_b);          // outer frame side b

   // tiles with outer frame on one side, other side with inner and notch (CW)
   draw_tile_from_quarters(bs+36, b1, outer_edge_t,          outer_edge_t,          solid_outer_corner_br, inner_corner_tr);       // outer frame side t
   draw_tile_from_quarters(bs+37, b1, inner_corner_br,       outer_edge_r,          outer_edge_r,          solid_outer_corner_bl); // outer frame side r
   draw_tile_from_quarters(bs+38, b1, solid_outer_corner_tl, inner_corner_bl,       outer_edge_b,          outer_edge_b);          // outer frame side b
   draw_tile_from_quarters(bs+39, b1, outer_edge_l,          solid_outer_corner_tr, inner_corner_tl,       outer_edge_l);          // outer frame side l

   // tiles with 3 notches and one inner
   draw_tile_from_quarters(bs+40, b1, inner_corner_br,       inner_corner_bl,       solid_outer_corner_br, inner_corner_tr);       // notch br
   draw_tile_from_quarters(bs+41, b1, inner_corner_br,       inner_corner_bl,       inner_corner_tl,       solid_outer_corner_bl); // notch bl
   draw_tile_from_quarters(bs+42, b1, solid_outer_corner_tl, inner_corner_bl,       inner_corner_tl,       inner_corner_tr);       // notch tl
   draw_tile_from_quarters(bs+43, b1, inner_corner_br,       solid_outer_corner_tr, inner_corner_tl,       inner_corner_tr);       // notch tr

   // tiles with 2 opposite notches
   draw_tile_from_quarters(bs+44, b1, solid_outer_corner_tl, inner_corner_bl,       solid_outer_corner_br, inner_corner_tr);       // notch tl and br
   draw_tile_from_quarters(bs+45, b1, inner_corner_br,       solid_outer_corner_tr, inner_corner_tl,       solid_outer_corner_bl); // notch tr and bl

   draw_tile_from_quarters(bs+47, b1, inner_corner_tl,       inner_corner_tr,      inner_corner_br,        inner_corner_bl);       // single reverse


   // save modified block tiles bitmap to file
   mBitmapTools.save_bitmap_to_block_tiles_file(b1);

   al_destroy_bitmap(b1);
   al_destroy_bitmap(dd_ts_bmp);
   al_destroy_bitmap(sb);


   // set default tile flags

   // set all to solid
   int flag = PM_BTILE_SOLID_PLAYER | PM_BTILE_SOLID_ENEMY | PM_BTILE_SOLID_ITEM | PM_BTILE_SOLID_PBUL | PM_BTILE_SOLID_EBUL;
   for (int i=bs; i<bs+48; i++)
      mBitmap.tileFlags[i] = flag;


   // unused
   for (int i=bs+48; i<bs+64; i++)
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
      if (mTileSets.findTileSetContainingIndex(s, bs))
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
   // type = 2; // 0-frame 1-solid 2-extended
   // base_tile_index = 256;
   // solid_source_tile_index = 0;

   // // red pipes extended
   // type = 2; // 0-frame 1-solid 2-extended
   // base_tile_index = 320;
   // solid_source_tile_index = 0;

   // green pipes extended
   // type = 2; // 0-frame 1-solid 2-extended
   // base_tile_index = 384;
   // solid_source_tile_index = 0;

   // // blue pipes extended
   type = 2; // 0-frame 1-solid 2-extended
   base_tile_index = 448;
   solid_source_tile_index = 0;






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
      l2=0.30;
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

   // 3rd extended green pipe set
   if (base_tile_index == 384)
   {
      h1=144;
      h2=158;
      s1=0.88;
      s2=0.95;
      l1=0.10;
      l2=0.35;
      steps=10;
      round=0.0;
      create_tileset_extended(base_tile_index, h1, h2, s1, s2, l1, l2, steps, round);
   }


   // 4th extended blue pipe set
   if (base_tile_index == 448)
   {
      h1=222;
      h2=236;
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







































































































bool colors_equal(ALLEGRO_COLOR c1, ALLEGRO_COLOR c2)
{
   float r1, g1, b1, a1;
   float r2, g2, b2, a2;
   al_unmap_rgba_f(c1, &r1, &g1, &b1, &a1);
   al_unmap_rgba_f(c2, &r2, &g2, &b2, &a2);
   const float EPSILON = 0.001f; // Tolerance for floating-point math
   return (fabs(r1 - r2) < EPSILON && fabs(g1 - g2) < EPSILON && fabs(b1 - b2) < EPSILON && fabs(a1 - a2) < EPSILON);
}

void mwTileSets::is_pixel_unique(ALLEGRO_BITMAP *bmp, int x, int y)
{
   // get pixel color
   ALLEGRO_COLOR c = al_get_pixel(bmp, x, y);

   // extract r, b, g from color
   float r, g, b;
   al_unmap_rgb_f(c, &r, &g, &b);

   // get h s l from r g b
   float h, s, l;
   al_color_rgb_to_hsl(r, g, b, &h, &s, &l);

   bool isBlack = ((r==0 && g==0 && b==0));



   // check unique colors vector for existing match
   bool match_color=0;
   for (auto& uc : uniqueColors)
   {
      if (colors_equal(c, uc.color)) // found existing match
      {
         match_color = 1;
         uc.points.push_back(point(x, y)); // add point to vector of points for this unique color
      }
   }
   if (match_color == 0) // no match found
   {
      // create a new unique color
      uniqueColor uc;
      uc.color = c;
      uc.hue = h;
      uc.points.push_back(point(x, y));
      uc.selected = 0;
      uniqueColors.push_back(uc);
   }

   // check for unique hue
   if (!isBlack) // only continue if not black
   {
      // check unique hues vector for existing match
      bool match_hue=0;
      for (auto& uh : uniqueHues)
      {
         if (fabs(uh.hue - h) < 4) // found existing match (or close enough)
         {
            match_hue = 1;
            uh.points.push_back(point(x, y)); // add point to vector of points for this unique hue
         }
      }
      if (match_hue == 0) // no match found
      {
         // create a new unique hue
         uniqueHue uh;
         uh.hue = h;
         uh.points.push_back(point(x, y));
         uh.selected = 0;
         uniqueHues.push_back(uh);
      }
   }
}

void mwTileSets::find_unique_colors_and_hues(ALLEGRO_BITMAP *b)
{
   uniqueColors.clear();
   uniqueHues.clear();
   selectedPixels.clear();

   // iterate all pixels
   int w = al_get_bitmap_width(b);
   int h = al_get_bitmap_height(b);

   for (int x=0; x<w; x++)
      for (int y=0; y<h; y++)
         is_pixel_unique(b, x, y);

   // sort vectors by hue
   std::sort(uniqueColors.begin(), uniqueColors.end(), [](const uniqueColor& a, const uniqueColor& b) { return a.hue < b.hue; });
   std::sort(uniqueHues.begin(),   uniqueHues.end(),   [](const uniqueHue& a,   const uniqueHue& b)   { return a.hue < b.hue; });
}

void modify_hsl(ALLEGRO_BITMAP *bmp, int x, int y, float h_adj, float s_adj, float l_adj)
{
   // extract r, b, g, h, s, l from color
   float r, g, b;
   al_unmap_rgb_f(al_get_pixel(bmp, x, y), &r, &g, &b);
   if (!(r==0 && g==0 && b==0)) // only if not black
   {
      float h, s, l;
      al_color_rgb_to_hsl(r, g, b, &h, &s, &l);

      h += h_adj;
      if (h<0) h+=360;
      if (h>360) h-=360;

      s += s_adj;
      // wrap
      if (s<0) s+=1;
      if (s>1) s-=1;
//      // clamp
//      if (s<0) s = 0;
//      if (s>1) s = 1;

      l += l_adj;
      // wrap
      if (l<0) l+=1;
      if (l>1) l-=1;
//      // clamp zaq
//      if (l<0) l = 0;
//      if (l>1) l = 1;


      al_set_target_bitmap(bmp);
      al_put_pixel(x, y, al_color_hsl(h, s, l));
   }
}

void mwTileSets::modify_selected(ALLEGRO_BITMAP *b, int mod, float val)
{
   for (auto& p : selectedPixels)
   {
      if (mod == 1) modify_hsl(b, p.x, p.y, val, 0, 0);
      if (mod == 2) modify_hsl(b, p.x, p.y, 0, val, 0);
      if (mod == 3) modify_hsl(b, p.x, p.y, 0, 0, val);
   }
}

void mwTileSets::set_pixel_selection()
{
   selectedPixels.clear();
   for (auto& uc : uniqueColors)
      if (uc.selected)
      {
         for (auto& p : uc.points) selectedPixels.push_back(p);
      }
   for (auto& uh : uniqueHues)
      if (uh.selected)
      {
         for (auto& p : uh.points) selectedPixels.push_back(p);
      }
}


void mwTileSets::load_region_for_edit(ALLEGRO_BITMAP *bmp, int x, int y, int w, int h)
{
   // load tilemap
   ALLEGRO_BITMAP *b1 = mBitmapTools.load_block_tiles_to_bitmap();

   // draw region
   al_set_target_bitmap(bmp);
   al_draw_bitmap_region(b1, x, y, w, h, 0, 0, 0);

   al_destroy_bitmap(b1);

   al_set_target_backbuffer(mDisplay.display);
}

void mwTileSets::save_region_from_edit(ALLEGRO_BITMAP *bmp, int x, int y)
{
   // load tilemap
   ALLEGRO_BITMAP *b1 = mBitmapTools.load_block_tiles_to_bitmap();

   // draw edited bitmap back to tilemap and save to disk
   al_set_target_bitmap(b1);
   al_draw_bitmap(bmp, x, y, 0);

   // save to disk
   mBitmapTools.save_bitmap_to_block_tiles_file(b1);

   al_destroy_bitmap(b1);
   al_set_target_backbuffer(mDisplay.display);
}




void mwTileSets::adjust_tile_set_color()
{

   // the tileset to be modified will always be consecutive blocks on the tilemap
   int bs = 1008; // start block
   int bl = 16; // length (in blocks)





   // position of source region in tilemap
   int b2s_x = (bs % 32)*22;
   int b2s_y = (bs / 32)*22;
   int b2s_w = bl*22;
   int b2s_h = 22;

   // create and clear a bitmap to hold a copy of the region to modify
   ALLEGRO_BITMAP *b2 = al_create_bitmap(b2s_w, b2s_h);
   al_set_target_bitmap(b2);
   al_clear_to_color(al_map_rgb(0,0,0));

   load_region_for_edit(b2, b2s_x, b2s_y, b2s_w, b2s_h);

   // find unique colors and hues
   find_unique_colors_and_hues(b2);

   ALLEGRO_COLOR tint = al_map_rgba_f(0.5, 0.5, 0.5, 0.5);
   tint = al_map_rgba_f(1, 1, 1, 1);

   int quit = 0;
   al_show_mouse_cursor(mDisplay.display);
   while (!quit)
   {
      mEventQueue.proc(1);
      al_flip_display();
      al_set_target_backbuffer(mDisplay.display);
      al_clear_to_color(al_map_rgb(0,0,0));

      int x=10;
      int y=10;
      int w = bl*22;
      int h = 22;

      if (mWidget.buttontcb(x,    y, 0, 14, 0,0,0,0, 0,11,15,14, 1,0,0,0, "Load")) load_region_for_edit(b2, b2s_x, b2s_y, b2s_w, b2s_h);
      if (mWidget.buttontcb(x+60, y, 0, 14, 0,0,0,0, 0,10,15,14, 1,0,0,0, "Save")) save_region_from_edit(b2, b2s_x, b2s_y);

      y+=24;

      int d = 1; // draw 1x
      al_draw_rectangle(x-1, y-1, x+w*d+1, y+h*d+1, mColor.White, 1);
      al_draw_scaled_bitmap(b2, 0, 0, w, h, x, y, w*d, h*d, 0);

      y+=32;

      d=2; // draw 2x
      al_draw_rectangle(x-1, y-1, x+w*d+1, y+h*d+1, mColor.White, 1);
      al_draw_scaled_bitmap(b2, 0, 0, w, h, x, y, w*d, h*d, 0);

      y+=64;

      if (mWidget.buttontcb(x+200, y, 0, 14, 0,0,0,0, 0,12,15,14, 1,0,0,0, "Detect Unique")) find_unique_colors_and_hues(b2);

      y+=32;

      show_unique_colors(x+200, y);


/*
      if (mInput.key[ALLEGRO_KEY_0][0]) tint = al_map_rgba_f(1, 1, 1, 1);
      if (mInput.key[ALLEGRO_KEY_1][0])
      {
         while (mInput.key[ALLEGRO_KEY_1][0]) mEventQueue.proc(1);
      }

      color_controls("tint", tint, x, y+200, 600, 16);

      al_set_target_bitmap(b2);
      al_draw_tinted_bitmap_region(b1, tint, (bs % 32)*22, (bs / 32)*22, bl*22, 22, 0, 0, 0);

      */


      int ya = y+120;

      ya+=20;
      if (mWidget.buttontcb(x,     ya, 0, 14, 0,0,0,0, 0,15,15,14, 1,0,0,0, "HUE+")) modify_selected(b2, 1, 10);
      if (mWidget.buttontcb(x+80,  ya, 0, 14, 0,0,0,0, 0,15,15,14, 1,0,0,0, "HUE-")) modify_selected(b2, 1, -10);

      ya+=16;
      if (mWidget.buttontcb(x,     ya, 0, 14, 0,0,0,0, 0,15,15,14, 1,0,0,0, "SAT+")) modify_selected(b2, 2, .1);
      if (mWidget.buttontcb(x+80,  ya, 0, 14, 0,0,0,0, 0,15,15,14, 1,0,0,0, "SAT-")) modify_selected(b2, 2, -.1);

      ya+=16;
      if (mWidget.buttontcb(x,     ya, 0, 14, 0,0,0,0, 0,15,15,14, 1,0,0,0, "LIG+")) modify_selected(b2, 3, .1);
      if (mWidget.buttontcb(x+80,  ya, 0, 14, 0,0,0,0, 0,15,15,14, 1,0,0,0, "LIG-")) modify_selected(b2, 3, -.1);

      ya+=20;


      if (mInput.key[ALLEGRO_KEY_ESCAPE][0])
      {
         while (mInput.key[ALLEGRO_KEY_ESCAPE][0]) mEventQueue.proc(1);
         quit = 1;
      }
   }

   al_destroy_bitmap(b2);
}



void mwTileSets::show_unique_colors(int x, int y)
{
   al_draw_textf(mFont.pr8, mColor.pc[15], x, y, 0, "%d unique colors", (int)uniqueColors.size());
   y+=14;

   int cc = 0;

   for (auto& uc : uniqueColors)
   {
      float r, g, b, a;
      al_unmap_rgba_f(uc.color, &r, &g, &b, &a);

      float h, s, l;
      al_color_rgb_to_hsl(r, g, b, &h, &s, &l);

      al_draw_textf(mFont.pr8, mColor.pc[15], x+24, y, 0, "r:%1.2f g:%1.2f b:%1.2f h:%6.2f s:%1.2f l:%1.2f count:%d", r, g, b, h, s, l, (int)uc.points.size());
      al_draw_filled_rectangle(x, y, x+20, y+8, uc.color);

      // check box
      int ya = y;
      if (mWidget.togglec(x-20, ya, x, 10,  0,0,0,0,  0, 0, 0, 0,  1,0,1,0, uc.selected, "", 15, 15)) set_pixel_selection();

      y+=10;
      cc+=(int)uc.points.size();
   }
   al_draw_textf(mFont.pr8, mColor.pc[15], x+24, y, 0, "total count:%d", cc);


   y+=20;
   al_draw_textf(mFont.pr8, mColor.pc[15], x, y, 0, "%d unique hues", (int)uniqueHues.size());
   y+=14;

   cc = 0;

   for (auto& uh : uniqueHues)
   {
      al_draw_filled_rectangle(x, y, x+20, y+8, al_color_hsl(uh.hue, 1, 0.5));
      al_draw_textf(mFont.pr8, mColor.pc[15], x+24, y, 0, "h:%6.2f count:%d", uh.hue, (int) uh.points.size());

      // check box
      int ya = y;
      if (mWidget.togglec(x-20, ya, x, 10,  0,0,0,0,  0, 0, 0, 0,  1,0,1,0, uh.selected, "", 15, 15)) set_pixel_selection();

      y+=10;
      cc+=(int)uh.points.size();
   }
   al_draw_textf(mFont.pr8, mColor.pc[15], x+24, y, 0, "total count:%d", cc);
}


void mwTileSets::color_controls(const char* text, ALLEGRO_COLOR &c, int x, int y, int width, int bts)
{
   float o = 0.5;

   ALLEGRO_COLOR show_color = c;

   float fpw = 44; // first panel width

   float xa = x+o+fpw+2;
   float xb = x+o+width-2;

   int height = 4 + bts*6;

   float r, g, b;
   al_unmap_rgb_f(c, &r, &g, &b);

   float h, s, l;
   mColor.map_rgb_to_hsl(c, h, s, l);

   bool r_hover = false;
   bool g_hover = false;
   bool b_hover = false;
   bool h_hover = false;
   bool s_hover = false;
   bool l_hover = false;

   bool na = false;

   float r_hover_val = 0;
   float g_hover_val = 0;
   float b_hover_val = 0;
   float h_hover_val = 0;
   float s_hover_val = 0;
   float l_hover_val = 0;

   float ya = y+2;

   if (mWidget.colorClickSlider(1, xa, ya, xb, bts, r, r_hover, r_hover_val, c, na)) c = al_map_rgb_f(r,g,b);
   if (mWidget.colorClickSlider(2, xa, ya, xb, bts, g, g_hover, g_hover_val, c, na)) c = al_map_rgb_f(r,g,b);
   if (mWidget.colorClickSlider(3, xa, ya, xb, bts, b, b_hover, b_hover_val, c, na)) c = al_map_rgb_f(r,g,b);
   if (r_hover) show_color = al_map_rgb_f(r_hover_val, g,           b );
   if (g_hover) show_color = al_map_rgb_f(r,           g_hover_val, b );
   if (b_hover) show_color = al_map_rgb_f(r,           g,           b_hover_val );

   ya +=2;
   al_draw_line(x+o+fpw, ya+o-2, x+o+width, ya+o+-2, mColor.White, 1); // line between rgb and hsl

   if (mWidget.colorClickSlider(4, xa, ya, xb, bts, h, h_hover, h_hover_val, c, na)) c = al_color_hsl(h, s, l);
   if (mWidget.colorClickSlider(5, xa, ya, xb, bts, s, s_hover, s_hover_val, c, na)) c = al_color_hsl(h, s, l);
   if (mWidget.colorClickSlider(6, xa, ya, xb, bts, l, l_hover, l_hover_val, c, na)) c = al_color_hsl(h, s, l);
   if (h_hover) show_color = al_color_hsl(h_hover_val, s,           l );
   if (s_hover) show_color = al_color_hsl(h,           s_hover_val, l );
   if (l_hover) show_color = al_color_hsl(h,           s,           l_hover_val );

   // show color rect
   al_draw_filled_rectangle(x, y+20, x+o+fpw, y+o+height, show_color);

   // show text
   al_draw_text(mFont.pr8, mColor.White, x+o+fpw/2, y+o+2,  ALLEGRO_ALIGN_CENTER | ALLEGRO_ALIGN_INTEGER, text);
   al_draw_text(mFont.pr8, mColor.White, x+o+fpw/2, y+o+10, ALLEGRO_ALIGN_CENTER | ALLEGRO_ALIGN_INTEGER, "color");

   // frame all
   al_draw_rectangle(x+o, y+o, x+o+width, y+o+height, mColor.White, 1);
   al_draw_line(x+o, y+o+20, x+o+fpw, y+o+20, mColor.White, 1);
   al_draw_line(x+o+fpw, y+o, x+o+fpw, y+o+height, mColor.White, 1);

}



