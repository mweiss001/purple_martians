// mwWindowManager.cpp
#include "pm.h"
#include "mwWindowManager.h"

mwWindowManager::mwWindowManager(int number_of_windows)
{
   num_windows = number_of_windows;
   mwWindow w; // make blank entries in the vector
   for (int i = 0; i<num_windows; i++) mW.push_back(w);
}

void mwWindowManager::cycle_windows(int draw_only)
{
   // check window positions for offscreen
   for (int a=0; a<num_windows; a++) mW[a].check_offscreen();

   // start with mouse not on any window by default
   mouse_on_window = 0;

   // get the max layer (highest number = bottom layer) top_layer is always 0
   int max_layer = 0;
   for (int a=0; a<num_windows; a++)
      if ((mW[a].active) && (mW[a].layer > max_layer)) max_layer = mW[a].layer;

   // cycle layers from bottom up
   for (int b = max_layer; b>=0; b--)
   {
      // find any active windows at that layer
      for (int a=0; a<num_windows; a++)
         if ((mW[a].active) && (mW[a].layer == b))
         {
            // draw with window
            mW[a].draw(draw_only);

            // detect mouse if not draw only
            if ((!draw_only) && (mW[a].detect_mouse())) mouse_on_window = a;
         }
   }

   if (draw_only) return;

   // set all windows to not have focus
   for (int a=0; a<num_windows; a++) mW[a].have_focus = 0;

   if (mouse_on_window)
   {
      // set the window to be the only one with focus
      mW[mouse_on_window].have_focus = 1;

      // set the window layer to the top layer

      // detect if window is not at the top layer
      if (mW[mouse_on_window].layer !=0)
      {
         // get old layer number of window we are promoting
         int on = mW[mouse_on_window].layer;

         // slide everything down one layer (add 1 to all layers)
         for (int a=0; a<num_windows; a++)
            if (mW[a].active) mW[a].layer++;

         // set new top layer
         mW[mouse_on_window].layer = 0;

         // remove gaps in layers
         for (int b=on+2; b<num_windows+10; b++) // cycle from old promoted layer down
            for (int a=0; a<num_windows; a++)    // cycle all windows
               if ((mW[a].active) && (mW[a].layer == b)) mW[a].layer--; // move layer up
      }

      // process mouse input on the window
      mW[mouse_on_window].process_mouse();
   }
}
