// mwRollingAverage.cpp

#include "pm.h"
#include "mwRollingAverage.h"
#include <limits>

mwRollingAverage mRollingAverage[4];



//mwRollingAverage mRollingAverage[4];




mwRollingAverage::mwRollingAverage()
{
   initialize(8);
}

void mwRollingAverage::initialize(int arr_sz)
{
   for (int i=0; i<400; i++) arr[i] = 0;
   arr_size = arr_sz;
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
   if (++index > arr_size-1) index = 0;
   num_filled++;

   int ul = arr_size; // find number of valid entries, arr_size by default unless not filled yet
   if (num_filled < arr_size) ul = num_filled;

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


