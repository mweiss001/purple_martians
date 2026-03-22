#ifndef PM_MWLEVELICONS_H
#define PM_MWLEVELICONS_H

struct level_icon
{
   int index;
   int level_num;
};

class mwLevelIcons
{

public:
   void load(int rebuild_all = 0);
   void reload();
   bool reload_needed = true;
   void draw_level_icon(int x, int y, int size, int level);

private:

   int size = 200;
   void create();
   int get_level_num_from_filepath(const char* fn);
   void add(const char* filepath, int &index);
   bool find_level_icon(struct level_icon &li, int lev);

   ALLEGRO_BITMAP * tilemap = nullptr;
   ALLEGRO_BITMAP * M_tilemap = nullptr;
   std::vector<level_icon> level_icons;
};

extern mwLevelIcons mLevelIcons;

#endif //PM_MWLEVELICONS_H


/*

used by many things

- to draw gate icons in 2 different sizes
- in visual level select
- copied to web folder for web pages

creates bitmap files for each level in 200x200 png format
in dir /bitmaps/level_icons

these are generated, not saved in git

in initial setup load is called
it must be after bitmaps are all created and level data has been setup

mLevel.setup_data();
mFont.load_fonts();
mBitmap.load_tiles();
mLevelIcons.load(0);

if no icons already exist, they are created
/bitmaps/level_icons (from leve1 1 to 399)
web/assets/icons (from leve1 1 to 99)

or if mLevelIcons.load(1); is called with 1, then they are all recreated
this could be used to rebuild them if things change
all it does is erase all files in '/bitmaps/level_icons' and let them get automatically re-created

then load() iterates all the icon files and puts them in a big tilemap
it also makes a memory bitmap version as well to quickly restore when the video bitmap is changed
it stores the level number and position in the tilemap in a vector

then there is the draw function:

void draw_level_icon(int x, int y, int size, int level);

this is used by everything that needs to draw a level icon

when the tilemap needs to be recreated, like all the other video bitmaps:

in 'void mwBitmap::rebuild_bitmaps()'

// rebuild level_icons

   // delay loading, just set flag that it needs to be done
   // this is to prevent the delay in the middle of a game
   mLevelIcons.reload_needed = true;

   // if last loaded level is overworld, or we are in visual level select, reload now
   if (mLevel.last_level_loaded == 1 || mLoop.visual_level_select_running) mLevelIcons.reload();

whenever draw is called, if the tilemap needs reloading it is done

void mwLevelIcons::draw_level_icon(int x, int y, int draw_size, int level)
{
   if (reload_needed) reload();

*/



