// mwRollingAverage.cpp

#include "mwRollingAverage.h"

mwRollingAverage mRollingAverage[4];

mwRollingAverage::mwRollingAverage(int size)
{
   initialize(size);
}

void mwRollingAverage::initialize(int size)
{
   dq.clear();
   last_input = avg = min = max = 0;
}

void mwRollingAverage::add_data(double d)
{
   last_input = d;

   // add to back of queue
   dq.push_back(d);

   // remove from front of queue
   while ((int) dq.size() > size) dq.pop_front();

   min = std::numeric_limits<double>::max();
   max = std::numeric_limits<double>::lowest();
   double tally = 0;
   for (double v : dq)
   {
      tally += v;
      if (v < min) min = v;
      if (v > max) max = v;
   }
   avg = tally / (double) dq.size();
}
