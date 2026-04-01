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
   int Single;      // standalone single tile
   int SolidFill;   // center tile with no borders

   int SingleReverse;

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

   int SemiSolid;


};


class mwTileSets
{
   private:

   void init();

   void constructExtendedSet(std::string name, int i);

   void construct24tileSet(std::string name, int i);


   tileSet constructSolidRectangleSet(std::string name, int i);
   tileSet constructFrameRectangleSet(std::string name, int i);
   tileSet constructHlineSetMulti(std::string name, int i);

   tileSet constructHlineVlineSet(std::string name, int i);

   tileSet constructVlineSet(std::string name, int i);



   void constructEmptySet();
   void setBasicRect(int i);

   void setHline(int i, int left, int middle, int right);
   void setVline(int i, int top, int middle, int bottom);


   void create_tileset_solid(int bs, int tile, float h1, float h2, float s1, float s2, float l1, float l2, int steps, float round);
   void create_tileset_frame(int bs, float h1, float h2, float s1, float s2, float l1, float l2, int steps, float round);
   void create_tileset_extended(int bs, float h1, float h2, float s1, float s2, float l1, float l2, int steps, float round);

   void fill_step_color_array(float h1, float h2, float s1, float s2, float l1, float l2, int steps);
   ALLEGRO_COLOR step_color_array[20];
   int step_color_array_size;

   void draw_and_frame_3x3_bitmap(int type, float round, ALLEGRO_BITMAP *bmp, int tile = 0);
   void draw_lock_overlay_bitmap(ALLEGRO_BITMAP *b, ALLEGRO_COLOR lc);
   void erase_pixels_outside_rounded_corners(ALLEGRO_BITMAP *b, ALLEGRO_COLOR c);
   void make_hline_vline_and_single(ALLEGRO_BITMAP *b, ALLEGRO_BITMAP *b60, int indexS, int indexH, int indexV);

   void draw_lightning_crack_overlay();
   void draw_bomb_overlay();
   void draw_B_overlay();



   void showTileSet(int x, int y, int type, int bs);
   void showLiveTile(int x, int y, int type, int tile, float h1, float h2, float s1, float s2, float l1, float l2, int steps, float round);


   public:
   mwTileSets();

   struct tileSet ts;
   std::vector<tileSet> tileSets;

   bool findTileSetContainingIndex(struct tileSet &t, int tileIndex, int type = 0);
   bool isTileKeyedBlock(int tileIndex, int keyIndex);


   void drawTile(int x, int y, int tileNum, int drawItemFlags, int drawTileMode);

   void draw(int bx1, int bx2, int by1, int by2, int drawItem, int drawTileMode);


   bool compareTile(int rb, int cb, int set);

   void modify_tile_set();
};

extern mwTileSets mTileSets;

#endif //PM_MWTILESETS_H