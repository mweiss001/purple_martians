// mwDrawSequence.h

#include "mwRollingAverage.h"

class mwDrawSequence
{
   public:

   mwDrawSequence(); // default constructor
   void initialize(void);
   void ds_draw(int setup_only, int flip);
   void add(int& i);
   void show_text(int x, int y);

   void add_names(int& i, const char* n0, const char* n1);

   private:

   int ns;

   char name[2][20][40];
   mwRollingAverage RA[20];
   double ts[20];
   char* get_line(int s, char*);
};
extern mwDrawSequence mDrawSequence;




