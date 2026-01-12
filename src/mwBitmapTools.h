// mwBitmapTools.h

class mwBitmapTools
{
   public:
   void select_bitmap(int& tn);
   void animation_sequence_editor();
   void draw_flags(int x1, int y1, int& num, int& mpow, int view_only, int clear_background, int ignore_mpow);
   void edit_btile_attributes();
   void copy_tiles();

//   private:

   void fill_player_tile();

   void color_shiftc(ALLEGRO_BITMAP *b, int sc, int cs, int x, int y);
   void color_shift4(ALLEGRO_BITMAP *b, int sc, int cs1, int cs2, int cs3, int cs4);
   void color_shift3(ALLEGRO_BITMAP *b, int sc, int cs1, int cs2, int cs3);
   void color_shift2(ALLEGRO_BITMAP *b, int sc, int cs1, int cs2);
   void color_shift(ALLEGRO_BITMAP *b, int sc, int cs);
   void colorize_tile();
   void combine_tile();
   int select_bitmap_ans(int zzindx, int& bmp_index);
   void redraw_grid(int x, int y, int current_selection);
   void draw_gridlines_and_frame(int x1, int y1, int x2, int y2, int fd, int fc, int fw, int gd, int gc, int gw);

   void draw_flag_rects_multiple(int bx1, int by1, int bx2, int by2, int x, int y, int w, int h, int ys, int con, int cof, int highlight);
   void draw_flag_text(int x, int y, int ys, int col, int last_flag_show);
   int draw_flag_rects(int tn, int x, int y, int w, int h, int ys, int last_flag_show);
   void draw_and_proc_flag_rects_for_sa(int tn, int x, int y, int w, int h, int ys);
};
extern mwBitmapTools mBitmapTools;


