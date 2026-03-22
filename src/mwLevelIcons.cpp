#include "pm.h"
#include "mwLevelIcons.h"

#include "mwBitmap.h"
#include "mwColor.h"
#include "mwDisplay.h"
#include "mwFileIterator.h"
#include "mwFont.h"
#include "mwItem.h"
#include "mwLevel.h"
#include "mwLoop.h"
#include "mwScreen.h"

mwLevelIcons mLevelIcons;


// create one level_icon file per level
void mwLevelIcons::create()
{
   bool save_web = true;

   //double t0 = al_get_time();

   char fname[256];

   // create directory if it does not already exist
   al_make_directory("bitmaps/level_icons");

   // get old values to restore later
   int old_level_editor_running = mLoop.level_editor_running;
   int old_start_level          = mLevel.start_level;

   // this needs to be off to draw level properly
   mLoop.level_editor_running = 0;

   // temp bitmap to draw level on
   ALLEGRO_BITMAP *tmp = al_create_bitmap(size, size);

   // maximum level number to search
   int max_level = 400;


   for (int level=0; level<max_level; level++)
      if (mLevel.load_level(level, 0, 1))
      {
         // draw level
         al_set_target_bitmap(tmp);
         mScreen.draw_level2(tmp,  0, 0, size, 1, 1, 1, 1, 0);

         // save level
         sprintf(fname,"bitmaps/level_icons/lev%03d.png", level);
         al_save_bitmap(fname, tmp);

         if (save_web && level < 100)
         {
            sprintf(fname,"web/assets/icons/lev%03d.png", level);
            al_save_bitmap(fname, tmp);
         }


         // show progress bar
         int pc = level*100 / max_level;
         al_set_target_backbuffer(mDisplay.display);
         mScreen.draw_percent_bar(mDisplay.SCREEN_W/2, mDisplay.SCREEN_H/2, mDisplay.SCREEN_W-200, 20, pc );
         al_draw_text(mFont.pr8, mColor.pc[15], mDisplay.SCREEN_W/2, mDisplay.SCREEN_H/2+6, ALLEGRO_ALIGN_CENTER, "Creating Level Icons");
         al_flip_display();
      }

   // load the icons into the tilemaps
   load();

   // do level 1
   // this has to be last, because gates drawn on this level require level_icons created above
   int level = 1;
   if (mLevel.load_level(level, 0, 1))
   {
      al_set_target_bitmap(tmp);
      mScreen.draw_level2(tmp,  0, 0, size, 1, 1, 1, 1, 0);
      sprintf(fname,"bitmaps/level_icons/lev%03d.png", level);
      al_save_bitmap(fname, tmp);
      if (save_web)
      {
         sprintf(fname,"web/assets/icons/lev%03d.png", level);
         al_save_bitmap(fname, tmp);
      }
   }

   al_destroy_bitmap(tmp);

   // restore
   mLoop.level_editor_running = old_level_editor_running;
   if (old_start_level)  mLevel.set_start_level(old_start_level);

   //printf("void mwLevelIcons::create() - %3.2fms\n", (al_get_time()- t0)*1000);
}


int mwLevelIcons::get_level_num_from_filepath(const char* fn)
{
   // filename only
   char nm[256];
   ALLEGRO_PATH * path = al_create_path(fn);
   sprintf(nm, "%s", al_get_path_filename(path));
   al_destroy_path(path);

   // level number
   char ln[4];
   ln[0] = nm[3];
   ln[1] = nm[4];
   ln[2] = nm[5];
   ln[3] = 0;
   return atoi(ln);
}




void mwLevelIcons::add(const char* fn, int &index)
{
   int lev = get_level_num_from_filepath(fn);

   // get bitmap from file
   ALLEGRO_BITMAP *tmp = al_load_bitmap(fn);

   // draw in place on tilemap
   int x = (index % 10) * size;
   int y = (index / 10) * size;
   al_set_target_bitmap(M_tilemap);
   al_draw_bitmap(tmp, x, y, 0);

   al_destroy_bitmap(tmp);

   // push index and level num to vector
   struct level_icon li;
   li.index = index;
   li.level_num = lev;
   level_icons.push_back(li);

   // next index
   index++;
}


// load all level icon files and draw level icons on tilemaps
void mwLevelIcons::load(int rebuild_all)
{
   //double t0 = al_get_time();

   if (rebuild_all)
   {
      // erase all files
      int num_files = mFileIterator.iterate("bitmaps/level_icons");
      for (int i=0; i<num_files; i++)
         al_remove_fs_entry(mFileIterator.filenames[i]);
   }

   // count number of level files
   int num_lev_files = mFileIterator.iterate("levels");
   printf("num_lev_files%d\n", num_lev_files);

   // get size of tilemaps
   int w = size * 10;
   int h = size * ((num_lev_files/10) + 1);

   // create tilemaps
   al_set_new_bitmap_flags(ALLEGRO_MEMORY_BITMAP);
   M_tilemap = al_create_bitmap(w, h);
   if (M_tilemap == nullptr) printf("failed creating memory bitmap %d x %d\n", w, h);

   al_set_new_bitmap_flags(ALLEGRO_NO_PRESERVE_TEXTURE | ALLEGRO_VIDEO_BITMAP);
   tilemap = al_create_bitmap(w, h);
   if (tilemap == nullptr) printf("failed creating video bitmap %d x %d\n", w, h);

   // count number of icon files
   int num_files = mFileIterator.iterate("bitmaps/level_icons");
   if (num_files == 0)
   {
      printf("No files found.\n");
      create();
      num_files = mFileIterator.iterate("bitmaps/level_icons");
   }

   level_icons.clear();
   int index = 0;

   for (int i=0; i<num_files; i++)
      add(al_get_fs_entry_name(mFileIterator.filenames[i]), index);

   reload();

   //printf("void mwLevelIcons::load() - %3.1fms\n", (al_get_time()- t0)*1000);


}

void mwLevelIcons::reload()
{
   // double t0 = al_get_time();
   al_set_target_bitmap(tilemap);
   al_draw_bitmap(M_tilemap, 0, 0, 0);
   reload_needed = false;
   //printf("void mwLevelIcons::reload() - %3.1fms\n", (al_get_time()- t0)*1000);

}



bool mwLevelIcons::find_level_icon(struct level_icon &li, int lev)
{
   for (auto l : level_icons)
      if (l.level_num == lev) { li = l; return true; }
   return false;
}

void mwLevelIcons::draw_level_icon(int x, int y, int draw_size, int level)
{
   if (reload_needed) reload();
   struct level_icon li;
   if (find_level_icon(li, level))
   {
      // get sub bitmap of region to draw
      int sx = (li.index % 10) * size;
      int sy = (li.index / 10) * size;
      ALLEGRO_BITMAP *tmp = al_create_sub_bitmap(tilemap, sx, sy, size, size);

      al_draw_scaled_bitmap(tmp, 0, 0, size, size, x, y, draw_size, draw_size, 0);

      al_destroy_bitmap(tmp);
   }
   else printf("level icon %d not found\n", level);
}


