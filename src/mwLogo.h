// mwLogo.h

class mwLogo
{
   public:

   mwLogo(); // default constructor
   void initialize(void);

   ALLEGRO_BITMAP *logo_ichael;
   ALLEGRO_BITMAP *logo_avid;
   ALLEGRO_BITMAP *logo_eiss;
   int logo_text_bitmaps_create;

   int mdw_an_seq;      // animation sequence number
   float points[9][8];  // array of points for splines

   void seed_logo(void);
   void fill_logo(void);

   void draw_logo(float x, float y, float x_scale, float y_scale);

   void mw_text(ALLEGRO_FONT *tf, int col, float x_pc, const char * txt);
   void idw(int txt, float x, float y, float x_scale, float y_scale);

   void mspline(float *par, ALLEGRO_COLOR col, int thickness);
   void mfspline(float *par, int col, int thickness);

   void mdw_an(float x, float y, float sc);
   int mdw_an2(void);
   void splash_screen(void);

   void spline_test(void);
   void redraw_spline(int s);
   void spline_adjust(void);


   float mdw_map_logo_x;
   float mdw_map_logo_y;
   float mdw_map_logo_scale;

   float mdw_splash_logo_x;
   float mdw_splash_logo_y;
   float mdw_splash_logo_scale;

   float mdw_logo_scale_dec;
   float mdw_logo_x_dec;
   float mdw_logo_y_dec;



   // might as well store these here too
   int menu_map_size;
   int menu_map_x;
   int menu_map_y;




   int show_splash_screen = 1;
   int splash_screen_done = 0;

};

extern mwLogo mwL;




