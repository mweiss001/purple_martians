// mwBitmapTools.h

#include "mwRect.h"

class mwBitmapTools
{
   public:
   void select_bitmap_from_level(int& tn);
   void animation_sequence_editor();
   void draw_flags(int x1, int y1, int& num, int& mpow, int view_only, int clear_background, int ignore_mpow);

   void copy_tiles();

   int select_bitmap();

   void get_tile_from_tilemap(ALLEGRO_BITMAP *b, ALLEGRO_BITMAP *t, int tile);
   void put_tile_to_tilemap(ALLEGRO_BITMAP *b, ALLEGRO_BITMAP *t, int tile);

   int make_backup_on_save_bitmap = 1;


   ALLEGRO_BITMAP* load_block_tiles_to_bitmap();
   void save_bitmap_to_block_tiles_file(ALLEGRO_BITMAP* b1);


   void mw_save_bitmap(const char *filename, ALLEGRO_BITMAP *bitmap);


   char block_tiles_fn[100];



   void redraw_grid(mwRect<int> tile_grid_rect, mwRect<int> selection_rect, int gridlines, int mode, int selected_index);
   void edit_tile_flags();





   void draw_flag_rects_multiple_th(int x, int y);








   private:

   void draw_flag_text(int x, int y, int ys, int col);
   void draw_flag_rects_multiple(int bx1, int by1, int bx2, int by2, int x, int y, int w, int h, int ys, int con, int cof, int highlight);
   int draw_flag_rects(int tn, int x, int y, int w, int h, int ys);
   void draw_and_proc_flag_rects(int tn, int x, int y, int w, int h, int ys);


   void copy_tile_range(int s, int d, int r);
   void move_tiles_and_refs(int s, int d, int r);



   void fill_player_tile();

   void color_shiftc(ALLEGRO_BITMAP *b, int sc, int cs, int x, int y);
   void color_shift4(ALLEGRO_BITMAP *b, int sc, int cs1, int cs2, int cs3, int cs4);
   void color_shift3(ALLEGRO_BITMAP *b, int sc, int cs1, int cs2, int cs3);
   void color_shift2(ALLEGRO_BITMAP *b, int sc, int cs1, int cs2);
   void color_shift(ALLEGRO_BITMAP *b, int sc, int cs);
   void colorize_tile();
   void combine_tile();
   int select_bitmap_ans(int zzindx, int& bmp_index);
   int select_bitmap_ans();


   void draw_gridlines_and_frame(mwRect<int>r, int fd, int fc, int fw, int gd, int gc, int gw, int ts);

   void draw_mouse_pointer_button(int x2, int y2, ALLEGRO_BITMAP *b, int index);



   void draw_tilecount_overlays(int x1, int y1);
   void draw_tilecount_overlay(int x1, int y1, int tile, int extended);


   void draw_flags_overlay(int x1, int y1, int tile, int flags);
   void draw_flags_overlays(int x1, int y1, int flags);





};
extern mwBitmapTools mBitmapTools;


