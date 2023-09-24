// mwQuickGraph2.h

class mwQuickGraph2
{
   public:

   mwQuickGraph2(); // default constructor
   void initialize(int width, int height, float dmin, float dmax, const char* name, int name_disp_type, int bgc);

   int width;
   int height;

   int entry_pos;
   int last_entry_pos;


   ALLEGRO_BITMAP *bmp = NULL;

   int bg_color;

   char name[256];

   int name_display_type;



   float data_min;
   float data_max;
   float data_rng;


   void draw_graph(int x, int y);


   void new_entry_pos();

   void add_data(int series, float d);

   struct data_series
   {
      float last_data;
      int active;
      int color;
   };
   data_series series[10] = {0};

};
extern mwQuickGraph2 mQuickGraph2[10];




