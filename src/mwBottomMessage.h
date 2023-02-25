// mwBottomMessage.h
#include "mwPlayers.h"
#include "mwLog.h"

class mwBottomMessage
{
   public:

   ALLEGRO_BITMAP *bmsg_bmp[20];
   ALLEGRO_BITMAP *bmsg_bmp2[20];
   ALLEGRO_BITMAP *bmsg_temp;

   int bottom_msg_on;
   int bottom_msg_timer;
   int bmsg_index;

   int game_event_retrigger_holdoff[10];
   float game_event_retrigger_holdoff_tally[10];

   void initialize(void);
   void create_bitmaps(void);

   void add(int ev, int x, int y, int z1, int z2, int z3, int z4);

   int draw_text(const char *txt, int col, int bmsg_length);
   int draw_tile(int tn, int bmsg_length);
   int draw_tile2(int tn, int bmsg_length, int xo, int yo);
   int draw_player(int p, int bmsg_length);
   int draw_health(int h, int bmsg_length);
   int draw_enemy(int e_type, int bmsg_length);

   void draw();

};
extern mwBottomMessage mwBM;





