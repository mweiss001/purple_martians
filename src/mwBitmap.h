// mwBitmap.h

class mwBitmap
{
   public:

   void create_bitmaps(void);
   void rebuild_bitmaps(void);

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


















};

extern mwBitmap mwB;


