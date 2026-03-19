#ifndef PM_MWTILEEDITOR_H
#define PM_MWTILEEDITOR_H

class mwTileEditor
{

public:
   void edit_tile(int tileIndex);


private:

   // screen position of the magnified tile
   float dx1, dy1, dx2, dy2, dw, dh, d_scale;
   float o = 0.5;

   // selection in pixels relative the 20x20 tile
   int sx1, sy1, sx2, sy2, sw, sh;


   // screen position of draw and point color widgets

   float pcw_x;
   float pcw_y;
   float pcw_w;
   float pcw_bts;

   float dcw_x;
   float dcw_y;
   float dcw_w;
   float dcw_bts;

   float rotate_degrees = 90;



   // mouse index if on main tile
   bool mouse_on_grid;
   int mx, my;


   char title[1024];
   int edit_tile_index = 0;
   int gridlines;

   int paste_mode_alpha;



   int draw_mode;
   float th = 1;

   int tile_palette[3][3] = {0};

   ALLEGRO_COLOR highlight_color = al_map_rgb(255,255,255); // highlight color
   ALLEGRO_COLOR frame_color = al_map_rgb(127,127,127); // frame and gridline color
   ALLEGRO_COLOR point_color = al_map_rgb(0,0,0);
   ALLEGRO_COLOR draw_color = al_map_rgb(0,0,0);

   ALLEGRO_BITMAP *tilemap_bitmap = nullptr;
   ALLEGRO_BITMAP *edit_tile_bitmap = nullptr;
   ALLEGRO_BITMAP *tile_palette_bitmap = nullptr;
   ALLEGRO_BITMAP *edit_tile_preview_bitmap = nullptr;

   void show_color_replace_preview();


   void draw_frame_and_gridlines();
   void edit_tile_redraw(ALLEGRO_BITMAP *t, bool show_selection);
   void process_draw();

   void scroll_selection(ALLEGRO_BITMAP *t, int x, int y);
   void flip_selection(ALLEGRO_BITMAP *t, int x, int y);
   void rotate_selection(ALLEGRO_BITMAP *t, float a);

   void color_controls(const char* text, ALLEGRO_COLOR &c, int x, int y, int width, int bts);


   void selection_controls(int x, int y);
   int draw_mode_controls(int x, int y);
   void tile_palette_select(int x1, int y1, int size);
   void legacy_color_select(int x, int y, int size);

   void fix_sel();
   void set_edit_tile(int tile);

};


#endif //PM_MWTILEEDITOR_H