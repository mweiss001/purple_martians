#ifndef PM_MWTILESETS_H
#define PM_MWTILESETS_H

#include <string>
#include <vector>


struct tileSet
{
   std::string name;
   int startIndex;
   int endIndex;

   int hlineMode;
   int vlineMode;
   int solidMode;
   int frameMode;
   int extendedMode;


   // single tiles
   int FrameCross;  // 4 way Tee
   int Single; // standalone single tile
   int SolidFill;   // center tile with no borders

   // single lines
   int HLineL;
   int HLineM;
   int HLineR;

   int VLineT;
   int VLineM;
   int VLineB;

   // frame corners
   int FrameCornerTL;
   int FrameCornerTR;
   int FrameCornerBL;
   int FrameCornerBR;

   // frame edges (these are the same as hline and vline middle)
   int FrameEdgeL;
   int FrameEdgeR;
   int FrameEdgeT;
   int FrameEdgeB;

   // frame edges tees
   int FrameEdgeLTee;
   int FrameEdgeRTee;
   int FrameEdgeTTee;
   int FrameEdgeBTee;

   // solid outer corners
   int OuterCornerTL;
   int OuterCornerTR;
   int OuterCornerBL;
   int OuterCornerBR;

   // solid outer edges
   int OuterEdgeL;
   int OuterEdgeR;
   int OuterEdgeT;
   int OuterEdgeB;

   // solid outer edge tees
   int OuterEdgeLTee;
   int OuterEdgeRTee;
   int OuterEdgeTTee;
   int OuterEdgeBTee;

   // solid outer corner tees
   int OuterCornerTLTeeT;
   int OuterCornerTLTeeL;
   int OuterCornerTLTeeTL;
   int OuterCornerTRTeeT;
   int OuterCornerTRTeeR;
   int OuterCornerTRTeeTR;
   int OuterCornerBLTeeB;
   int OuterCornerBLTeeL;
   int OuterCornerBLTeeBL;
   int OuterCornerBRTeeB;
   int OuterCornerBRTeeR;
   int OuterCornerBRTeeBR;

   // solid inner corners
   int InnerCornerTL;
   int InnerCornerTR;
   int InnerCornerBL;
   int InnerCornerBR;

   // solid inner edges (these are the same as OuterEdge* only reversed)
   int InnerEdgeL;
   int InnerEdgeR;
   int InnerEdgeT;
   int InnerEdgeB;

   // diagonal notches
   int OuterCornerTRDiag;
   int OuterCornerTLDiag;


   int HLineMKeyR;
   int HLineMKeyG;
   int HLineMKeyB;
   int HLineMKeyP;

   int VLineMKeyR;
   int VLineMKeyG;
   int VLineMKeyB;
   int VLineMKeyP;



};


class mwTileSets
{
   private:

   void init();

      void constructExtendedSet(std::string name, int i);

      tileSet constructSolidRectangleSet(std::string name, int i);
      tileSet constructFrameRectangleSet(std::string name, int i);
      tileSet constructHlineSet(std::string name, int i);

      void constructEmptySet();
      void setBasicRect(int i);

      void setHline(int i, int left, int middle, int right);
      void setVline(int i, int top, int middle, int bottom);


   public:

      mwTileSets();

      struct tileSet ts;

      bool findTileSetContainingIndex(struct tileSet &t, int tileIndex, int type = 0);

      bool isTileKeyedBlock(int tileIndex, int keyIndex);

      std::vector<tileSet> tileSets;


      void draw(int bx1, int bx2, int by1, int by2, int drawItemNum, int drawItemFlags, int level[][100]);
      bool compareTile(int rb, int cb, int set);
};

extern mwTileSets mTileSets;

#endif //PM_MWTILESETS_H