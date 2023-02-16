// mwRollingAverage.cpp

#include "pm.h"
#include "mwRollingAverage.h"
#include <limits>

mwRollingAverage mwRA[4];

mwRollingAverage::mwRollingAverage()
{
   initialize();
}

void mwRollingAverage::initialize(void)
{
   for (int i=0; i<8; i++) arr[i] = 0;
   num_filled = 0;
   index = 0;
   avg = 0;
   last_input = 0;
   mn = 0;
   mx = 0;
}

void mwRollingAverage::add_data(double d)
{
   last_input = d;

   arr[index] = d;  // put new entries where index points
   if (++index > 7) index = 0;
   num_filled++;

   int ul = 8; // find number of valid entries, 8 by default unless not filled yet
   if (num_filled < 8) ul = num_filled;

   mn = std::numeric_limits<double>::max();
   mx = std::numeric_limits<double>::lowest();

   double tally = 0;
   for (int i=0; i<ul; i++) // cycle all the valid entries
   {
      tally += arr[i];
      if (arr[i] < mn) mn = arr[i]; // min
      if (arr[i] > mx) mx = arr[i]; // max
   }
   avg = tally / ul; // average
}


