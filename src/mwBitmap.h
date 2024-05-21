// mwBitmap.h

class mwBitmap
{
   public:

   void create_bitmaps(void);
   void rebuild_bitmaps(void);

   int load_tiles(void);
   void save_sprit(void);
   int load_sprit(void);

   void spin_shape(int tn, int x, int y, int tsx, int tsy, int tsw, int tsh, float scale, float dim, int cycle);

   void spin_shape2(int tn, float x, float y, float scale, float dim, int cycle);



   ALLEGRO_BITMAP *tilemap;
   ALLEGRO_BITMAP *btilemap;
   ALLEGRO_BITMAP *ptilemap;
   ALLEGRO_BITMAP *dtilemap;

   ALLEGRO_BITMAP *M_tilemap;
   ALLEGRO_BITMAP *M_btilemap;
   ALLEGRO_BITMAP *M_ptilemap;
   ALLEGRO_BITMAP *M_dtilemap;

   ALLEGRO_BITMAP *tile[NUM_SPRITES];
   ALLEGRO_BITMAP *btile[NUM_SPRITES];
   ALLEGRO_BITMAP *player_tile[16][32];
   ALLEGRO_BITMAP *door_tile[2][16][8];

   ALLEGRO_BITMAP *level_background;
   ALLEGRO_BITMAP *level_buffer;

   ALLEGRO_BITMAP *text_title = NULL;
   int text_title_bitmaps_create = 1;
   int text_title_draw_color = -1;

   ALLEGRO_BITMAP *large_text_overlay_bitmap;
   int large_text_overlay_state = 0;

   // animation sequence array
   int zz[20][NUM_ANS];

   void reset_animation_sequences(void);
   void update_animation(void);

   int sa[NUM_SPRITES][2]; // shape attributes

   ALLEGRO_BITMAP * create_and_clear_bitmap(int x, int y);


};

extern mwBitmap mBitmap;


