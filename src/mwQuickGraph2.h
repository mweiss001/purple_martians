// mwQuickGraph2.h

#include "mwRollingAverage.h"

class mwQuickGraph2
{
   public:

   mwQuickGraph2(); // default constructor
   void initialize(float width, float height, float dmin, float dmax, const char* name, int type, int c1, int c2, int act);


   int active;

   float x;
   float y;

   float width;
   float height;

   int entry_pos;
   int last_entry_pos;

   void set_pos(float xp, float yp);
   void set_size(float w, float h, int rebuild_bitmap);

   ALLEGRO_BITMAP *bmp = NULL;

   int col1;
   int col2;

   int bco = 208; // background color offset



   char name[256];

   int type;

   mwRollingAverage RA;

   float data_min;
   float data_max;
   float data_rng;

   void draw_graph();

   void change_range(float new_min, float new_max);

   void new_entry_pos();

   void autorange();


   void add_data(int series, float d, int erase_only);

   struct data_series
   {
      float last_data;
      int active;
      int color;
   };
   data_series series[10] = {};

};
extern mwQuickGraph2 mQuickGraph2[10];




