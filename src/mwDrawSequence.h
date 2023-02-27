// mwDrawSequence.h

#include "mwRollingAverage.h"

class mwDrawSequence
{
   public:

   mwDrawSequence(); // default constructor
   void initialize(void);

   void draw(void);
   void add(int s, double v);
   void show_text(int x, int y);

   private:

   void calc(void);
   int seq[20][2];
   char name_long[20][40];
   char name_short[20][10];
   double time_cur[20];
   double time_avg[20];
   double time_min[20];
   double time_max[20];
   mwRollingAverage RA[20];
   char* get_line(int s, char*);
};
extern mwDrawSequence mDrawSequence;




