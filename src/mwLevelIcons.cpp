#include "pm.h"
#include "mwLevelIcons.h"
#include "mwLevel.h"
#include "mwFileIterator.h"
#include "mwScreen.h"
#include "mwDisplay.h"
#include "mwFont.h"
#include "mwColor.h"


mwLevelIcons mLevelIcons;



// only called from set overworld barriers, and only if local player or server (not headless)
// updates a single level icon (overworld)
void mwLevelIcons::update_level_icon(int level)
{
   // printf("mwLevelIcons::update_level_icon(%d)\n", level);

   // create a new level icon
   create_level_icon(level);

   // update the level_icon tilemap
   int index = 0;
   if (find_level_icon_index(index, level))
   {
      // load bitmap from file
      char fn[256];
      sprintf(fn,"bitmaps/level_icons/lev%03d.png", level);
      ALLEGRO_BITMAP *tmp = al_load_bitmap(fn);

      // draw in place on tilemap
      int x = (index % 10) * regular_level_icon_size;
      int y = (index / 10) * regular_level_icon_size;
      al_set_target_bitmap(M_tilemap);
      al_draw_bitmap(tmp, x, y, 0);

      al_destroy_bitmap(tmp);

      // copy M_tilemap to tilemap
      reload();
   }
}


// creates regular level icons up to regular_level_icon_last_level
// loads levels, draws on temp bitmap, and saves
// erases all files first
//
// this gets called when loading level icons and none are found
// or manually from a button in settings


void mwLevelIcons::create_regular_level_icons()
{
   //printf("mwLevelIcons::create_regular_level_icons()\n");

   if (profile) t0 = al_get_time();

   // create directory if it does not already exist
   al_make_directory("bitmaps/level_icons");

   // erase all files if there are any
   int num_files = mFileIterator.iterate("bitmaps/level_icons");
   for (int i=0; i<num_files; i++)
      al_remove_fs_entry(mFileIterator.filenames[i]);


   // create level icons from start_level to end_level inclusive
   for (int level=1; level<=regular_level_icon_last_level; level++)
      if ((level != PM_OVERWORLD_LEVEL) && (level != PM_OVERWORLD_BASE_LEVEL))
      {
         create_level_icon(level);

         // show progress bar
         int range = regular_level_icon_last_level - 2;
         int progress = 100 * (level-2) / range;
         al_set_target_backbuffer(mDisplay.display);
         mScreen.draw_percent_bar(mDisplay.SCREEN_W/2, mDisplay.SCREEN_H/2, mDisplay.SCREEN_W-200, 20, progress );
         al_draw_text(mFont.pr8, mColor.pc[15], mDisplay.SCREEN_W/2, mDisplay.SCREEN_H/2+6, ALLEGRO_ALIGN_CENTER, "Creating Level Icons");
         al_flip_display();
      }

   // iterate level icon files, draw on tilemap, save index to vector
   load_icons_to_tilemap();

   // these have to be last, because gates drawn on these levels require level_icons created above
   create_level_icon(PM_OVERWORLD_BASE_LEVEL);
   create_level_icon(PM_OVERWORLD_LEVEL);

   // iterate level icon files, draw on tilemap, save index to vector
   load_icons_to_tilemap();

   if (profile) printf("mwLevelIcons::create_regular_level_icons() - %3.2fms\n", (al_get_time()- t0)*1000);
}


void mwLevelIcons::create_webpage_level_icons()
{
   // printf("mwLevelIcons::create_webpage_level_icons()\n");

   mLevel.make_level_name_web_includes();

   if (profile) t0 = al_get_time();

   // erase all level icon files
   int num_files = mFileIterator.iterate("web/assets/icons");
   for (int i=0; i<num_files; i++)
   {
      const char *filename = al_get_path_filename(al_create_path(al_get_fs_entry_name(mFileIterator.filenames[i])));
      if (strncmp(filename, "lev", 3 ) == 0) al_remove_fs_entry(mFileIterator.filenames[i]);
   }

   // create level icons from start_level to end_level inclusive
   for (int level=1; level<=webpage_level_icon_last_level; level++)
   {
      create_level_icon(level, 1);

      // show progress bar
      int range = webpage_level_icon_last_level - 2;
      int progress = 100 * (level-2) / range;
      al_set_target_backbuffer(mDisplay.display);
      mScreen.draw_percent_bar(mDisplay.SCREEN_W/2, mDisplay.SCREEN_H/2, mDisplay.SCREEN_W-200, 20, progress );
      al_draw_text(mFont.pr8, mColor.pc[15], mDisplay.SCREEN_W/2, mDisplay.SCREEN_H/2+6, ALLEGRO_ALIGN_CENTER, "Creating Level Icons");
      al_flip_display();
   }

   if (profile) printf("mwLevelIcons::create_webpage_level_icons() - %3.2fms\n", (al_get_time()- t0)*1000);
}




/*
   creates a single icon for either regular or webpage icons
   loads and draws level on temp bitmap
   saves the bitmap to png file in appropriate folder
*/
void mwLevelIcons::create_level_icon(int level, bool save_web)
{
   //printf("mwLevelIcons::create_level_icon(level:%d, save_web:%d)\n", level, save_web);

   if (!l2.load_level_by_number(level, 1, 1)) return;

   if (!save_web)
   {
      // temp bitmap to draw level on
      ALLEGRO_BITMAP *tmp = al_create_bitmap(regular_level_icon_size, regular_level_icon_size);
      al_set_target_bitmap(tmp);

      // draw level
      l2.draw_level3(tmp, level,  0, 0, regular_level_icon_size, 1, 1, 1, 1, 0, 0);

      // save
      char fname[256];
      sprintf(fname,"bitmaps/level_icons/lev%03d.png", level);
      if (!al_save_bitmap(fname, tmp)) printf("error saving %s\n",fname);

      al_destroy_bitmap(tmp);

   }
   if (save_web)
   {
      // temp bitmap to draw level on
      ALLEGRO_BITMAP *tmp = al_create_bitmap(webpage_level_icon_size, webpage_level_icon_size);
      al_set_target_bitmap(tmp);

      // draw level
      l2.draw_level3(tmp, level,  0, 0, webpage_level_icon_size, 1, 1, 1, 1, 0, 0);

      // save
      char fname[256];
      sprintf(fname,"web/assets/icons/lev%03d.png", level);
      if (!al_save_bitmap(fname, tmp)) printf("error saving %s\n",fname);

      al_destroy_bitmap(tmp);
   }
}



int mwLevelIcons::get_level_num_from_filepath(const char* fn)
{
   // get filename only
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



/*
destroys and recreates tilemaps
clears and rebuilds level_icons vector

iterates all level_icon files in "bitmaps/level_icons"
- draws on tilemap
- adds to vector that relates position on tilemap to level number

returns number of files found

calls reload to copy M_tilemap to tilemap

*/
int mwLevelIcons::load_icons_to_tilemap()
{
   // count number of icon files
   int num_files = mFileIterator.iterate("bitmaps/level_icons", ".png");
   if (num_files == 0)
   {
      printf("mwLevelIcons::load_icons_to_tilemap  --  No level icon files found.\n");
   }
   else
   {
      // printf("mwLevelIcons::load_icons_to_tilemap --  %d level icon files found\n", num_files);

      // get size of tilemaps (tilemap will be 10 icons wide and as tall as it needs to be to hold all )
      int w = regular_level_icon_size * 10;
      int h = regular_level_icon_size * ((num_files/10) + 1);

      // destroy and create tilemaps
      al_set_new_bitmap_flags(ALLEGRO_MEMORY_BITMAP);
      if (M_tilemap != nullptr) al_destroy_bitmap(M_tilemap);
      M_tilemap = al_create_bitmap(w, h);
      if (M_tilemap == nullptr) printf("Error loading level icons - failed creating memory bitmap %d x %d\n", w, h);

      al_set_new_bitmap_flags(ALLEGRO_NO_PRESERVE_TEXTURE | ALLEGRO_VIDEO_BITMAP);
      if (tilemap != nullptr) al_destroy_bitmap(tilemap);
      tilemap = al_create_bitmap(w, h);
      if (tilemap == nullptr) printf("Error loading level icons - failed creating video bitmap %d x %d\n", w, h);

      // clear level_icons vector
      level_icons.clear();


      // iterate all found level icon tiles and:
      // - draw on tilemap
      // - add to vector that relates position on tilemap to level number
      for (int i=0; i<num_files; i++)
      {
         // get filename and level number
         const char *fn = al_get_fs_entry_name(mFileIterator.filenames[i]);
         int lev_num = get_level_num_from_filepath(fn);

         // load bitmap from file
         ALLEGRO_BITMAP *tmp = al_load_bitmap(fn);

         // draw in place on M_tilemap
         int x = (i % 10) * regular_level_icon_size;
         int y = (i / 10) * regular_level_icon_size;
         al_set_target_bitmap(M_tilemap);
         al_draw_bitmap(tmp, x, y, 0);

         // done with temp bitmap
         al_destroy_bitmap(tmp);

         // push index and level num to vector
         level_icons.push_back({i, lev_num });

      }

      reload(); // copy M_tilemap to tilemap

   }
   return num_files;
}


// load all level icon files and draw level icons on tilemaps
void mwLevelIcons::load()
{
   //printf("mwLevelIcons::load()\n");

   if (profile) t0 = al_get_time();

   // load icons from files (if no files found, run create)
   if (!load_icons_to_tilemap()) create_regular_level_icons();

   if (profile)  printf("void mwLevelIcons::load() - %3.1fms\n", (al_get_time()- t0)*1000);
}


// draws M_tilemap on tilemap (done when screen changes and tilemap contents are lost)
void mwLevelIcons::reload()
{
   // printf("mwLevelIcons::reload()\n");
   if (profile) t0 = al_get_time();
   al_set_target_bitmap(tilemap);
   al_draw_bitmap(M_tilemap, 0, 0, 0);
   reload_needed = false;
   if (profile) printf("void mwLevelIcons::reload() %dx%d - %3.1fms\n", al_get_bitmap_width(tilemap), al_get_bitmap_height(tilemap), (al_get_time()- t0)*1000);
}

bool mwLevelIcons::find_level_icon_index(int &index, int level)
{
   for (auto l : level_icons)
      if (l.level_num == level) { index = l.index; return true; }
   return false;
}

void mwLevelIcons::draw_level_icon(int level, int dx, int dy, int draw_size)
{
   if (reload_needed) reload();
   int index = 0;
   if (find_level_icon_index(index, level))
   {
      // get location of tilemap region to grab from
      int sx = (index % 10) * regular_level_icon_size;
      int sy = (index / 10) * regular_level_icon_size;

      // draw
      al_draw_scaled_bitmap(tilemap, sx, sy, regular_level_icon_size, regular_level_icon_size, dx, dy, draw_size, draw_size, 0);
   }
   else printf("level %d icon not found\n", level);
}

