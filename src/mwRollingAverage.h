// mwRollingAverage.h
#ifndef mwRollingAverage_H
#define mwRollingAverage_H
#include <deque>
#include <limits>

class mwRollingAverage
{
   public:
   mwRollingAverage(int size = 8);
   void initialize(int size);
   void add_data(double d);

   double last_input;
   double avg;
   double min;
   double max;

   private:
   std::deque<double> dq;
   int size = 8;

};
extern mwRollingAverage mRollingAverage[4];

#endif
