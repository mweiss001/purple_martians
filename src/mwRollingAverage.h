// mwRollingAverage.h
#ifndef mwRollingAverage_H
#define mwRollingAverage_H

class mwRollingAverage
{
   public:
   mwRollingAverage(); // default constructor
   void initialize(int arr_siz);
   void add_data(double d);
   double arr[4000];
   int arr_size = 8;
   int    num_filled;
   int    index;
   double last_input;
   double avg;
   double mn;
   double mx;
};
extern mwRollingAverage mRollingAverage[4];

#endif
