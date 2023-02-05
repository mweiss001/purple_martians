// mwRollingAverage.h

class mwRollingAverage
{
   public:

   mwRollingAverage(); // default constructor
   void initialize(void);

   void add_data(double d);

   double arr[8];
   int    num_filled;
   int    index;
   double last_input;
   double avg;
   double mn;
   double mx;
};
extern mwRollingAverage mwRA[4];

extern mwRollingAverage RA[20];


