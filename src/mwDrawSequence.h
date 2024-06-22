// mwDrawSequence.h

#include "mwRollingAverage.h"

class mwDrawSequence
{
   public:
   mwDrawSequence();

   void ds_draw(int setup_only, int flip);
   void ds_show_text(int x, int y);

   private:
   int ns;
   char name[2][20][40];
   mwRollingAverage RA[20];
   double ts[20];


   void ds_add_names(int& i, const char* n0, const char* n1);

   void ds_add(int& i);
   void ds_add_total(int &i);

};
extern mwDrawSequence mDrawSequence;
