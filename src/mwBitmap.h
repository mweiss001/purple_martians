// mwBitmap.h

#ifndef PM_MWBITMAP_H
#define PM_MWBITMAP_H


struct portal_tileset
{
   int num_tiles;
   int idle_start;
   int idle_length;
   int main_start;
   int main_length;

   int idle_cx;
   int idle_cy;
   int main_cxi;
   int main_cyi;
   int main_cxf;
   int main_cyf;
};




class mwBitmap
{
   public:

   mwBitmap(); // default constructor


   void initialize();


   void draw_portal(int color, int index, int x, int y, float scale, float rot);

//   void draw_portal_old(int index, int x, int y, float scale, float rot);
   void make_door_tiles();




   void create_bitmaps();
   void rebuild_bitmaps();

   int load_tiles();

   void save_sprit();
   int load_sprit();

   void spin_shape(int tn, int x, int y, int tsx, int tsy, int tsw, int tsh, float scale, float dim, int cycle);
   void spin_shape2(int tn, float x, float y, float scale, float dim, int cycle);


   struct portal_tileset portal_tilesets[6];
   int current_portal_tileset = 5;

   // int portal_tilesets_num_tiles    = 15;
   // int portal_tilesets_idle_start   =  0;
   // int portal_tilesets_idle_length  =  4;
   // int portal_tilesets_main_start   =  4;
   // int portal_tilesets_main_length  = 11;
   // int portal_tilesets_idle_cx      = 28;
   // int portal_tilesets_main_cxi     = 28;
   // int portal_tilesets_main_cxf     = 25;
   // int portal_tilesets_idle_cy      = 33;
   // int portal_tilesets_main_cyi     = 33;
   // int portal_tilesets_main_cyf     = 35;
   //







   ALLEGRO_BITMAP *stilemap;
   ALLEGRO_BITMAP *btilemap;
   ALLEGRO_BITMAP *ptilemap;
   ALLEGRO_BITMAP *dtilemap;
   ALLEGRO_BITMAP *gtilemap;

//   ALLEGRO_BITMAP *wtilemap; // warp portal
   ALLEGRO_BITMAP *ztilemap; // warp portal



   ALLEGRO_BITMAP *M_stilemap;
   ALLEGRO_BITMAP *M_btilemap;
   ALLEGRO_BITMAP *M_ptilemap;
   ALLEGRO_BITMAP *M_dtilemap;
   ALLEGRO_BITMAP *M_gtilemap;

//   ALLEGRO_BITMAP *M_wtilemap;


   ALLEGRO_BITMAP *M_ztilemap;



   ALLEGRO_BITMAP *sprite[NUM_SPRITES];
   ALLEGRO_BITMAP *tile[NUM_TILES];
   ALLEGRO_BITMAP *player_tile[16][32];
//   ALLEGRO_BITMAP *door_tile[2][16][8];
   ALLEGRO_BITMAP *door_tile[16][10];

   ALLEGRO_BITMAP *gate_tile[7][3];




   ALLEGRO_BITMAP *portal_tile[16][15];



   ALLEGRO_BITMAP *text_title = NULL;
   int text_title_bitmaps_create = 1;
   int text_title_draw_color = -1;

   ALLEGRO_BITMAP *large_text_overlay_bitmap;
   int large_text_overlay_state = 0;

   // animation sequence array
   int zz[20][NUM_ANS];

   void reset_animation_sequences();
   void update_animation();

   int tileFlags[NUM_TILES];

   ALLEGRO_BITMAP * create_and_clear_bitmap(int x, int y);
};

extern mwBitmap mBitmap;

#endif // PM_MWBITMAP_H
























