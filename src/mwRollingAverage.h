// mwRollingAverage.h
#ifndef mwRollingAverage_H
#define mwRollingAverage_H

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
extern mwRollingAverage mRollingAverage[4];

#endif
