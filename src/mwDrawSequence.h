// mwDrawSequence.h


class mwDrawSequence
{
   public:

   mwDrawSequence(); // default constructor
   void initialize(void);

   int seq[20][2];

   char name_long[20][40];
   char name_short[20][10];

   double time_cur[20];
   double time_avg[20];
   double time_min[20];
   double time_max[20];


   void add(int s, double v);

   char * get_line(int s);

   void show_text(int x, int y);

   void calc(void);

   void draw(void);


};
extern mwDrawSequence mwDS;



