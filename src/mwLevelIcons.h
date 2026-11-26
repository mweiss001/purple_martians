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
   void load();
   void reload();
   bool reload_needed = true;
   void draw_level_icon(int level, int x, int y, int size);

   void update_level_icon(int level);

   void create_regular_level_icons();
   void create_webpage_level_icons();


private:
   bool profile = false;
   double t0;

   int regular_level_icon_size = 200;
   int regular_level_icon_last_level = 400;

   int webpage_level_icon_size = 200;
   int webpage_level_icon_last_level = 150;

   void create_level_icon(int level, bool save_web = false);
   int load_icons_to_tilemap();
   int get_level_num_from_filepath(const char* fn);
   bool find_level_icon_index(int &index, int level);


   ALLEGRO_BITMAP * tilemap = nullptr;
   ALLEGRO_BITMAP * M_tilemap = nullptr;
   std::vector<level_icon> level_icons;
};

extern mwLevelIcons mLevelIcons;

#endif //PM_MWLEVELICONS_H


/*
used to draw level icons on gates
used in visual level select

regular level_icons
-------------------
There is a png file for each level in 'bitmaps/level_icons' lev001.png etc...
These are not saved in git or supplied with release, they are dynamically created when needed

int regular_level_icon_size = 200; // determines their size
int regular_level_icon_last_level = 400; // determines the max level

in initial setup, 'load()' is called
it must be after bitmaps are all created and level data has been setup

mLevel.setup_data();
mFont.load_fonts();
mBitmap.load_tiles();
mLevelIcons.load();

'load()'
----------------------------
destroys and recreates tilemaps
clears and rebuilds level_icons vector

iterates all level_icon files in "bitmaps/level_icons"
- draws on tilemap
- adds to vector that relates position on tilemap to level number

if no png files are found they are all recreated with: create_regular_level_icons();
this can also be run manually from settings


then load() iterates all the icon files and puts them in one large tilemap
it also makes a memory bitmap version as well to quickly restore when the video bitmap is changed
it stores the level number and position in the tilemap in a vector

then there is the draw function:
void draw_level_icon(int x, int y, int size, int level);
this is used by everything that needs to draw a level icon


rebuilding level_icons tilemap
------------------------------
if the display is changed all video bitmaps nee to be recreated
I do not do this immediately, I just set a flag that it needs to be done:
mLevelIcons.reload_needed = true;

whenever draw is called, if the tilemap needs reloading it is done

void mwLevelIcons::draw_level_icon(int x, int y, int draw_size, int level)
{
   if (reload_needed) reload();







web level_icons
---------------
Used by the web pages on headless server

These ARE saved in git (headless server has no display and no way to generate them)

There is a png file for each level in web/assets/icons' lev001.png etc...

To recreate, run 'create_webpage_level_icons()' by clicking the button in settings

int webpage_level_icon_size = 200;
int webpage_level_icon_last_level = 150;





*/



