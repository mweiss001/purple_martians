#include "pm.h"
#include "mwLevelIcons.h"

#include "mwBitmap.h"
#include "mwColor.h"
#include "mwDisplay.h"
#include "mwFileIterator.h"
#include "mwFont.h"
#include "mwItem.h"
#include "mwLevel.h"
#include "mwLift.h"
#include "mwLoop.h"
#include "mwScreen.h"

mwLevelIcons mLevelIcons;



void mwLevelIcons::update_one(int level, int save_web)
{

   if (mLevel.load_level_by_number(level, 1, 1)) // load only, and do my own custom setup
   {
      mLevel.valid_level_loaded = 1;
      mLift.lift_setup();
      for (int x=0; x<500; x++)
         if (mItem.item[x][0]) // only if active set x y
         {
            mItem.itemf[x][0] = mItem.item[x][4];
            mItem.itemf[x][1] = mItem.item[x][5];
         }

      mScreen.init_level_background(); // draw blocks on level_background

      char fname[256];

      // temp bitmap to draw level on
      ALLEGRO_BITMAP *tmp = al_create_bitmap(size, size);

      // draw level
      al_set_target_bitmap(tmp);
      mScreen.draw_level2(tmp,  0, 0, size, 1, 1, 1, 1, 0);

      // save level icon
      sprintf(fname,"bitmaps/level_icons/lev%03d.png", level);
      if (!al_save_bitmap(fname, tmp)) printf("error saving %s\n",fname);

      if (save_web && level < 100)
      {
         sprintf(fname,"web/assets/icons/lev%03d.png", level);
         if (!al_save_bitmap(fname, tmp)) printf("error saving %s\n",fname);
      }

      int index = 0;

      if (find_level_icon_index(index, level))
      {

         // draw in place on tilemap
         int x = (index % 10) * size;
         int y = (index / 10) * size;
         al_set_target_bitmap(M_tilemap);
         al_draw_bitmap(tmp, x, y, 0);

         // copy M_tilemap to tilemap
         reload();



      }






      al_destroy_bitmap(tmp);

   }









}


/*
  loads the level (if it exists)
  does custom load then manually sets up and draws on supplied bitmap
  then saves the bitmap as a level icon file

*/
void mwLevelIcons::create_one(int level, int save_web)
{
   printf("mwLevelIcons::create_one(level:%d\n", level);

   if (mLevel.load_level_by_number(level, 1, 1)) // load only, and do my own custom setup
   {

      mLevel.valid_level_loaded = 1;
      mLift.lift_setup();
      for (int x=0; x<500; x++)
         if (mItem.item[x][0]) // only if active set x y
         {
            mItem.itemf[x][0] = mItem.item[x][4];
            mItem.itemf[x][1] = mItem.item[x][5];
         }

      mScreen.init_level_background(); // draw blocks on level_background

      char fname[256];

      // temp bitmap to draw level on
      ALLEGRO_BITMAP *tmp = al_create_bitmap(size, size);

      // draw level
      al_set_target_bitmap(tmp);
      mScreen.draw_level2(tmp,  0, 0, size, 1, 1, 1, 1, 0);

      // save level icon
      sprintf(fname,"bitmaps/level_icons/lev%03d.png", level);
      if (!al_save_bitmap(fname, tmp)) printf("error saving %s\n",fname);

      if (save_web && level < 100)
      {
         sprintf(fname,"web/assets/icons/lev%03d.png", level);
         if (!al_save_bitmap(fname, tmp)) printf("error saving %s\n",fname);
      }

      al_destroy_bitmap(tmp);



   }
}



// if no level icons are found, this will create them
void mwLevelIcons::create()
{
   printf("mwLevelIcons::create()\n");

   bool save_web = true;

   if (profile) t0 = al_get_time();



   // count number of level files
   int num_lev_files = mFileIterator.iterate("levels");
   //printf("num_lev_files:%d\n", num_lev_files);

   if (num_lev_files > 500)
   {
      printf("Error creating level icons. Number of level files (%d) is more than 500\n", num_lev_files);
      return;
   }



   // create directory if it does not already exist
   al_make_directory("bitmaps/level_icons");

   // get old values to restore later
   int old_start_level          = mLevel.start_level;
   int old_level_editor_running = mLoop.level_editor_running;

   // this needs to be off to draw level properly
   mLoop.level_editor_running = 0;


   // maximum level number to search
   int max_level = 400;

   for (int level=2; level<max_level; level++)
      if (level != 70)
      {
         create_one(level, save_web);

         // show progress bar
         int pc = level*100 / max_level;
         al_set_target_backbuffer(mDisplay.display);
         mScreen.draw_percent_bar(mDisplay.SCREEN_W/2, mDisplay.SCREEN_H/2, mDisplay.SCREEN_W-200, 20, pc );
         al_draw_text(mFont.pr8, mColor.pc[15], mDisplay.SCREEN_W/2, mDisplay.SCREEN_H/2+6, ALLEGRO_ALIGN_CENTER, "Creating Level Icons");
         al_flip_display();
      }

   load_icons_to_tilemap();

   // do level 1 and 70
   // this has to be last, because gates drawn on this level require level_icons created above

   create_one(70, save_web);
   load_icons_to_tilemap();

//   mLevel.set_overworld_barriers();
   create_one(1, save_web);
   load_icons_to_tilemap();



   update_one(1, save_web);



   // restore
   mLoop.level_editor_running = old_level_editor_running;
   if (old_start_level)  mLevel.set_start_level(old_start_level);

   if (profile) printf("void mwLevelIcons::create() - %3.2fms\n", (al_get_time()- t0)*1000);
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



/*

iterates all level_icon files
returns number of files found

destroys and recreates tilemaps

clears level_icons vector

iterates all level_icon files and:
- draws on tilemap
- adds to vector that relates position on tilemap to level number

calls reload to copy M_tilemap to tilemap


*/
int mwLevelIcons::load_icons_to_tilemap()
{
   // count number of icon files
   int num_files = mFileIterator.iterate("bitmaps/level_icons");
   if (num_files == 0)
   {
      printf("mwLevelIcons::load_icons_to_tilemap  --  No level icon files found.\n");
   }
   else
   {
      printf("mwLevelIcons::load_icons_to_tilemap --  %d level icon files found\n", num_files);

      // get size of tilemaps (tilemap will be 10 icons wide and as tall as it needs to be to hold all )
      int w = size * 10;
      int h = size * ((num_files/10) + 1);

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
         add(al_get_fs_entry_name(mFileIterator.filenames[i]), i);


      // copy M_tilemap to tilemap
      reload();

   }
   return num_files;
}

void mwLevelIcons::add(const char* fn, int index)
{
   // load bitmap from file
   ALLEGRO_BITMAP *tmp = al_load_bitmap(fn);

   // draw in place on tilemap
   int x = (index % 10) * size;
   int y = (index / 10) * size;
   al_set_target_bitmap(M_tilemap);
   al_draw_bitmap(tmp, x, y, 0);

   al_destroy_bitmap(tmp);

   // get level number from filename
   int lev = get_level_num_from_filepath(fn);

   // push index and level num to vector
   struct level_icon li;
   li.index = index;
   li.level_num = lev;
   level_icons.push_back(li);
}


// load all level icon files and draw level icons on tilemaps
void mwLevelIcons::load(int rebuild_all)
{
   printf("mwLevelIcons::load()\n");

   if (profile) t0 = al_get_time();

   if (rebuild_all)
   {
      // erase all files
      int num_files = mFileIterator.iterate("bitmaps/level_icons");
      for (int i=0; i<num_files; i++)
         al_remove_fs_entry(mFileIterator.filenames[i]);
   }

   // load icons from files (if no files found, run create)
   if (!load_icons_to_tilemap()) create();

   if (profile)  printf("void mwLevelIcons::load() - %3.1fms\n", (al_get_time()- t0)*1000);
}

void mwLevelIcons::reload()
{
   // printf("mwLevelIcons::reload()\n");
   if (profile) t0 = al_get_time();
   al_set_target_bitmap(tilemap);
   al_draw_bitmap(M_tilemap, 0, 0, 0);
   reload_needed = false;
   if (profile) printf("void mwLevelIcons::reload() - %3.1fms\n", (al_get_time()- t0)*1000);
}

bool mwLevelIcons::find_level_icon_index(int &index, int level)
{
   for (auto l : level_icons)
      if (l.level_num == level) { index = l.index; return true; }
   return false;
}

void mwLevelIcons::draw_level_icon(int x, int y, int draw_size, int level)
{
   if (reload_needed) reload();
   int index = 0;
   if (find_level_icon_index(index, level))
   {
      // printf("level %d icon drawn\n", level);

      // get sub bitmap of region to draw
      int sx = (index % 10) * size;
      int sy = (index / 10) * size;

      ALLEGRO_BITMAP *tmp = al_create_sub_bitmap(tilemap, sx, sy, size, size);
      al_draw_scaled_bitmap(tmp, 0, 0, size, size, x, y, draw_size, draw_size, 0);
      al_destroy_bitmap(tmp);

      // or I can do this in one line (not sure if this leaks memory)
      //al_draw_scaled_bitmap(al_create_sub_bitmap(tilemap, sx, sy, size, size), 0, 0, size, size, x, y, draw_size, draw_size, 0);

   }
   else printf("level %d icon not found\n", level);
}

