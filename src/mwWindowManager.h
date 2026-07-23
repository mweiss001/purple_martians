// mwWindowManager.h
#ifndef mwWindowManager_H
#define mwWindowManager_H
#include "mwWindow.h"

class mwWindowManager
{
   public:
   mwWindowManager(int number_of_windows); // default constructor
   std::vector<mwWindow> mW;
   int num_windows = 10;
   int mouse_on_window;
   void cycle_windows(int draw_only);
};
#endif





























