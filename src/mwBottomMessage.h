// mwBottomMessage.h
#include "mwPlayers.h"
#include "mwLog.h"

#define BMSG_MAX_LINES 30

class mwBottomMessage
{
   public:


   int num_lines;
   float io; // initial opacity
   float fo; // final opacity
   float ivs; // initial v size
   float fvs; // final v size
   float ihs; // initial h size
   float fhs; // final h size

   int disp_player;
   int disp_enemy;
   int disp_item;
   int disp_health;

   int disp_player_text_long;

   int filter_event[100];

   int bottom_msg_on;
   int bottom_msg_timer;

   int test_mode_list_created;

   void create_test_mode_list(void);

   void create_bitmaps(void);
   void initialize(void);

   void game_event(int ev, int x, int y, int z1, int z2, int z3, int z4);

   void add(int ev, int x, int y, int z1, int z2, int z3, int z4);
   void draw(int outline);


   void draw_health(int p, int h, int &xpos, int ypos, int xo, int yo);
   void draw_text(const char *txt, int col, int &xpos, int ypos);
   void draw_bmp(ALLEGRO_BITMAP *tmp, int &xpos, int ypos, int xo, int yo);
   void draw_player(int p, int &xpos, int ypos);
   void draw_enemy(int e_type, int &xpos, int ypos);
   void draw_item(ALLEGRO_BITMAP *tmp, const char *txt, int col, int &xpos, int ypos, int xo, int yo);


   private:

   ALLEGRO_BITMAP *bmsg_bmp[BMSG_MAX_LINES];
   ALLEGRO_BITMAP *bmsg_bmp2[BMSG_MAX_LINES];
   ALLEGRO_BITMAP *bmsg_temp;

   int bmsg_index;
   int game_event_retrigger_holdoff[10];
   //float game_event_retrigger_holdoff_tally[10];



};
extern mwBottomMessage mBottomMessage;





