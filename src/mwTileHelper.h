#ifndef PM_MWTILEHELPER_H
#define PM_MWTILEHELPER_H


class mwTileHelper
{
   public:
   mwTileHelper();

   void init();

   void clearFrameFills(int preset = 0);


   int replace_helper_48_frame(struct tileSet ts, int lv, int type);
   int replace_helper_24_frame(struct tileSet ts, int lv, int type);
   int replace_helper_16_frame(struct tileSet ts, int lv, int type);

   int replace_helper_48(int tile);
   int replace_helper_16(int tile, bool inner);
   int replace_helper_8(int tile);
   int replace_helper_pattern(int x, int y);

   void remove_bound();
   void find_bound(int max_lev);
   void find_adjacent(int x, int y);
   void find_connected(int x, int y, int group);
   bool test4(int v1, int v2, int v3, int v4, int lv, int t1, int t2, int t3, int t4);



   void draw_frame_fill(struct frameFill f, bool preview);
   void draw_frame_fills(bool preview);
   void draw_replace(bool preview);
   void draw_pattern(bool preview);





   void tileSetWidget(int x, int y, int tile, struct tileSet &target, bool &clicked, int d);

   int tileSetSelectWidget(int x1, int y1, int x2, int pad_height, int color, struct tileSet &target, const char * txt, bool &clicked, int d);


   bool get_tileset(int x, int y, struct tileSet &ts, const char *txt);

   bool tileSetWidget(int x, int y, int size, struct tileSet &ts, const char *txt, int d);


   int show_selection_controls(int x1, int x2, int y1, int color, int d);
   int show_replace_controls(  int x1, int x2, int y1, int color, int d);
   int show_pattern_controls(  int x1, int x2, int y1, int color, int d);
   int show_tileset_controls(  int x1, int x2, int y1, int color, int d);

   int show_frame_controls(    int x1, int x2, int y1, int color, int d);
   void show_frame_control_line(int x1, int x2, int y1, int y2, int index, int d);



   int draw_buttons(int x3, int x4, int yfb, int d);

   void process_mouse();


   std::vector<frameFill> frameFills;



   int c, l, r, t, b, tl, tr, bl, br;

   int thl[100][100];

   int add_del;
   int match;
   int group;
   int mark_overlay;

   int pattern_offset_x;
   int pattern_offset_y;
   int pattern_preview;


   int frame_mode_width;
   int frame_mode_preview;
   int frames_detected;
   int fill_sections;

   int frame_common_tileset;



   int replace_preview;
   int replace_mode;


   int test_junk;



};

extern mwTileHelper mTileHelper;

#endif //PM_MWTILEHELPER_H
