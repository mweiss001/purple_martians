#ifndef PM_MWTILESETS_H
#define PM_MWTILESETS_H

#include <string>
#include <vector>


struct point
{
   int x;
   int y;
};


struct uniqueColor
{
   ALLEGRO_COLOR color;
   float hue;
   int selected;
   std::vector<point> points;
};

struct uniqueHue
{
   float hue;
   int selected;
   std::vector<point> points;
};


struct tileSet
{
   std::string name;

   int tileSetType;

   int startIndex;
   int endIndex;
   int displayIndex;



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


};





class mwTileSets
{
   public:
   mwTileSets();

   private:

   void init();

   void constructEmptySet();

   void construct1x3(int i, std::string name);
   void construct3x1(int i, std::string name);

   void construct6(int i, std::string name);
   void construct8(int i, std::string name);
   void construct9(int i, std::string name);

   void construct16(int i, int d, std::string name);
   void construct24(int i, int d, std::string name);
   void construct48(int i, int d, std::string name);






   void create_tileset_solid(int bs, int tile, float h1, float h2, float s1, float s2, float l1, float l2, int steps, float round);
   void create_tileset_frame(int bs, float h1, float h2, float s1, float s2, float l1, float l2, int steps, float round);
   void create_tileset_extended(int bs, float h1, float h2, float s1, float s2, float l1, float l2, int steps, float round);

   public:




   void create_tileset_extended2(int bs);
   void create_tileset_from_16_mega(int bs, int mbx, int mby);
   void create_tileset_from_24_atomic(int bs);

   void create_tileset_single_faded_rect(int bs, ALLEGRO_COLOR bc);

   private:

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


   char type_name[50][40];


   struct tileSet currentTileSet;

   struct tileSet ts;
   std::vector<tileSet> tileSets;


   int altDrawRectMode = 0;
   int altDrawRectModePatternWidth = 2;
   int altDrawRectModePatternHeight = 2;
   int altDrawRectModePatternFill = 0;
   int altDrawRectModeForceSingle = 0;



   std::string altTextLine1;
   std::string altTextLine2;
   std::string altTextLine3;

   bool findTileSetContainingName(struct tileSet &t, std::string n);
   bool findTileSetContainingIndex(struct tileSet &t, int tileIndex);
   void drawTile(int x, int y, int tileNum, int drawItemFlags, int drawTileMode, bool preview);
   void drawRectHelper(int s, int t, int b, int l, int r, int tl, int tr, int bl, int br, bool preview);
   void drawRectHelperPattern(struct tileSet ts, bool preview);
   void drawRectHelperHline(struct tileSet ts, bool preview);
   void drawRectHelperVline(struct tileSet ts, bool preview);
   void drawRect(bool preview);


   bool compareTile(int rb, int cb, int set);

   void modify_tile_set();

   void adjust_tile_set_color();

   void load_region_for_edit(ALLEGRO_BITMAP *bmp, int x, int y, int w, int h);
   void save_region_from_edit(ALLEGRO_BITMAP *bmp, int x, int y);

   void color_controls(const char* text, ALLEGRO_COLOR &c, int x, int y, int width, int bts);


   std::vector<uniqueColor> uniqueColors;
   std::vector<uniqueHue> uniqueHues;
   std::vector<point> selectedPixels;

   void find_unique_colors_and_hues(ALLEGRO_BITMAP *b);

   void is_pixel_unique(ALLEGRO_BITMAP *b, int x, int y);

   void set_pixel_selection();

   void show_unique_colors(int x, int y);

   void modify_selected(ALLEGRO_BITMAP *b, int mod, float val);

};

extern mwTileSets mTileSets;

#endif //PM_MWTILESETS_H