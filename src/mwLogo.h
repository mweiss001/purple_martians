// mwLogo.h

class mwLogo
{
   public:

   mwLogo(); // default constructor
   void initialize();

   ALLEGRO_BITMAP *logo_ichael;
   ALLEGRO_BITMAP *logo_avid;
   ALLEGRO_BITMAP *logo_eiss;
   int logo_text_bitmaps_create;

   int mdw_an_seq;      // animation sequence number
   float points[9][8];  // array of points for splines

   void seed_logo();
   void fill_logo();

   void draw_logo(float x, float y, float x_scale, float y_scale);

   void mw_text(ALLEGRO_FONT *tf, int col, float x_pc, const char * txt);
   void idw(int txt, float x, float y, float x_scale, float y_scale);

   void mspline(float *par, ALLEGRO_COLOR col, int thickness);
   void mfspline(float *par, int col, int thickness);

   void mdw_an(float x, float y, float sc);
   int mdw_an2();
   void splash_screen();

   int show_splash_screen = 1;
   int splash_screen_done = 0;

   void spline_test();
   void redraw_spline(int s);
   void spline_adjust();

};

extern mwLogo mLogo;




