#include "mwServerScreenshot.h"

mwServerScreenshot mServerScreenshot;

#include "pm.h"
#include "mwScreen.h"
#include "mwNetgame.h"
#include <thread>
#include <functional>

void mwServerScreenshot::start()
{
   if (!busy)
   {
      busy = 1;
      // Create and run the long task in a separate thread
      std::thread worker(std::bind_front(&mwServerScreenshot::save, this));
      // Detach the worker thread to let it run in the background
      worker.detach();

   }
   else printf("busy\n");
}


void mwServerScreenshot::save()
{
   int size = mNetgame.server_update_status_img_size;

   double t0 = al_get_time();
   ALLEGRO_BITMAP * tmp = al_create_bitmap(size, size);
   double t1 = al_get_time();

   mScreen.draw_level2(tmp, 0, 0, size, 1, 1, 1, 1, 1);
   double t2 = al_get_time();

   if (!al_save_bitmap("savegame/lev_stat.png", tmp)) printf("error saving!\n");

   al_destroy_bitmap(tmp);

   double t3 = al_get_time();
   printf("Total time:%f Create:%f Draw:%f Save:%f \n", (t3-t0)*1000, (t1-t0)*1000, (t2-t1)*1000, (t3-t2)*1000);

   mNetgame.server_update_status_img_time = (t3-t0)*1000;

   busy = 0;

}


