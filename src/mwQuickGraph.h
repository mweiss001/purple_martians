// mwQuickGraph.h

class mwQuickGraph
{
   public:

   mwQuickGraph(); // default constructor
   void initialize(int t);

   int width;
   int height;
   int type;

   double y_axis_min;
   double y_axis_max;
   double y_axis_rng;

   void draw_graph(int x, int y);
   void add_data(int s, double d);

   struct data_series
   {
      double data[1000];
      int active;
      int color;
      char name[256];
   };
   data_series series[4] = {0};

};
extern mwQuickGraph mwQG[10];




