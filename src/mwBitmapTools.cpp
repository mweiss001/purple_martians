// mwBitmapTools.cpp

#include "pm.h"
#include "mwBitmapTools.h"
#include "mwWindowManager.h"
#include "mwColor.h"
#include "mwFont.h"
#include "mwBitmap.h"
#include "mwWidget.h"
#include "mwInput.h"
#include "mwDisplay.h"
#include "mwEventQueue.h"
#include "mwGlobalLevelTool.h"
#include "mwLevel.h"
#include "mwLoop.h"
#include "mwMiscFnx.h"
#include "mwScreen.h"
#include "mwSelectionWindow.h"
#include "mwTileEditor.h"
#include "mwTileHelper.h"
#include "mwTileSets.h"



mwBitmapTools mBitmapTools;


ALLEGRO_BITMAP* mwBitmapTools::load_block_tiles_to_bitmap()
{
   // load block_tiles to bitmap
   char b1_fn2[100];
   sprintf(b1_fn2, "bitmaps/tiles.png");
   // convert to 'ALLEGRO_FS_ENTRY' to get platform specific fully qualified path
   ALLEGRO_FS_ENTRY *FS_fname1 = al_create_fs_entry(b1_fn2);
   sprintf(block_tiles_fn, "%s", al_get_fs_entry_name(FS_fname1));
   ALLEGRO_BITMAP *b1 = al_load_bitmap(block_tiles_fn);
   if (!b1)
   {
      char msg[1024];
      sprintf(msg, "Error loading tiles from:%s", block_tiles_fn);
      mInput.m_err(msg);
      return nullptr;
   }
   return b1;
}



// this function has the same signature as al_save_bitmap(const char *filename, ALLEGRO_BITMAP *bitmap)
// this adds an optional saving of backup, by renaming the original file first
void mwBitmapTools::mw_save_bitmap(const char *filename, ALLEGRO_BITMAP *bitmap)
{
   bool save_old_versions = 1;
   int log = 1;
   if (save_old_versions)
   {
      // get timestamp
      char timestamp[256];
      time_t now = time(NULL);
      struct tm *timenow = localtime(&now);
      strftime(timestamp, sizeof(timestamp), "%Y%m%d-%H%M%S", timenow);

      char backup_filename[255];
      sprintf(backup_filename, "%s_%s.bak", filename, timestamp);

      if (std::rename(filename, backup_filename) == 0)
      {
         if (log > 1) printf("Success renaming '%s' to '%s'\n", filename, backup_filename);
      }
      else if (log)   printf("Error!! renaming '%s' to '%s'\n", filename, backup_filename);
   }
   al_save_bitmap(filename, bitmap);
}


void mwBitmapTools::save_bitmap_to_block_tiles_file(ALLEGRO_BITMAP* b1)
{
   mw_save_bitmap(block_tiles_fn, b1);
}

void mwBitmapTools::put_tile_to_tilemap(ALLEGRO_BITMAP *b, ALLEGRO_BITMAP *t, int tile)
{
   int x = (tile % 32) * 22 + 1;
   int y = (tile / 32) * 22 + 1;
   al_set_target_bitmap(b);
   al_draw_bitmap(t, x, y, 0);

}

void mwBitmapTools::get_tile_from_tilemap(ALLEGRO_BITMAP *b, ALLEGRO_BITMAP *t, int tile)
{
   int x = (tile % 32) * 22 + 1;
   int y = (tile / 32) * 22 + 1;
   al_set_target_bitmap(t);
   al_draw_bitmap_region(b, x, y, 20, 20, 0, 0, 0);
}










void mwBitmapTools::fill_player_tile(void)
{
   //printf("fill player bitmap\n");
   int a, b, x, y;


   if (0) // load from disk
   {
      mBitmap.ptilemap = al_load_bitmap("bitmaps/player_tiles.png");
      if (!mBitmap.ptilemap) mInput.m_err((char*)"Can't load tiles from bitmaps/player_tiles.png");
      else
      {
         //printf("load good\n");
         al_convert_mask_to_alpha(mBitmap.ptilemap, al_map_rgb(0, 0, 0)) ;

         al_set_target_bitmap(mBitmap.M_ptilemap);
         al_draw_bitmap(mBitmap.ptilemap, 0, 0, 0);

         // create sub bitmaps
         for (a=0; a<16; a++)
            for (b=0; b<24; b++)
               mBitmap.player_tile[a][b] = al_create_sub_bitmap(mBitmap.ptilemap, b*20, a*20, 20, 20);
      }
   }


   if (0) // create from shapes
   {
       for (a=0; a<16; a++)
          for (b=0; b<32; b++)
             mBitmap.player_tile[a][b] = al_create_bitmap(20,20);

   // fill the player_tile array
      for (a=0; a<16; a++) // set all to default shapes
      {
         al_set_target_bitmap(mBitmap.player_tile[a][0]); al_draw_bitmap(mBitmap.sprite[400], 0, 0, 0);
         al_set_target_bitmap(mBitmap.player_tile[a][1]); al_draw_bitmap(mBitmap.sprite[401], 0, 0, 0);
         al_set_target_bitmap(mBitmap.player_tile[a][2]); al_draw_bitmap(mBitmap.sprite[402], 0, 0, 0);
         al_set_target_bitmap(mBitmap.player_tile[a][3]); al_draw_bitmap(mBitmap.sprite[403], 0, 0, 0);
         al_set_target_bitmap(mBitmap.player_tile[a][4]); al_draw_bitmap(mBitmap.sprite[404], 0, 0, 0);
         al_set_target_bitmap(mBitmap.player_tile[a][5]); al_draw_bitmap(mBitmap.sprite[405], 0, 0, 0);

         al_set_target_bitmap(mBitmap.player_tile[a][6]); al_draw_bitmap(mBitmap.sprite[368], 0, 0, 0);
         al_set_target_bitmap(mBitmap.player_tile[a][7]); al_draw_bitmap(mBitmap.sprite[369], 0, 0, 0);
         al_set_target_bitmap(mBitmap.player_tile[a][8]); al_draw_bitmap(mBitmap.sprite[370], 0, 0, 0);
         al_set_target_bitmap(mBitmap.player_tile[a][9]); al_draw_bitmap(mBitmap.sprite[371], 0, 0, 0);
         al_set_target_bitmap(mBitmap.player_tile[a][10]); al_draw_bitmap(mBitmap.sprite[372], 0, 0, 0);
         al_set_target_bitmap(mBitmap.player_tile[a][11]); al_draw_bitmap(mBitmap.sprite[373], 0, 0, 0);

         al_set_target_bitmap(mBitmap.player_tile[a][12]); al_draw_bitmap(mBitmap.sprite[432], 0, 0, 0);
         al_set_target_bitmap(mBitmap.player_tile[a][13]); al_draw_bitmap(mBitmap.sprite[433], 0, 0, 0);
         al_set_target_bitmap(mBitmap.player_tile[a][14]); al_draw_bitmap(mBitmap.sprite[434], 0, 0, 0);
         al_set_target_bitmap(mBitmap.player_tile[a][15]); al_draw_bitmap(mBitmap.sprite[435], 0, 0, 0);
         al_set_target_bitmap(mBitmap.player_tile[a][16]); al_draw_bitmap(mBitmap.sprite[436], 0, 0, 0);
         al_set_target_bitmap(mBitmap.player_tile[a][17]); al_draw_bitmap(mBitmap.sprite[437], 0, 0, 0);

         al_set_target_bitmap(mBitmap.player_tile[a][18]); al_draw_bitmap(mBitmap.sprite[755], 0, 0, 0);

         al_set_target_bitmap(mBitmap.player_tile[a][19]); al_draw_bitmap(mBitmap.sprite[438], 0, 0, 0);

         al_set_target_bitmap(mBitmap.player_tile[a][20]); al_draw_bitmap(mBitmap.sprite[606], 0, 0, 0);
         al_set_target_bitmap(mBitmap.player_tile[a][21]); al_draw_bitmap(mBitmap.sprite[607], 0, 0, 0);

         al_set_target_bitmap(mBitmap.player_tile[a][22]); al_draw_bitmap(mBitmap.sprite[638], 0, 0, 0);
         al_set_target_bitmap(mBitmap.player_tile[a][23]); al_draw_bitmap(mBitmap.sprite[639], 0, 0, 0);

      }


      for (a=1; a<16; a++) //cycle 15 player colors (no zero)
      {
         int cs = -8 + a; // color shift (-8 to get from base to 0, then add player num for color)
         for (b=0; b<24; b++) //cycle 19 bitmaps for one color
         {
            al_set_target_bitmap(mBitmap.player_tile[a][b]);
            al_lock_bitmap(mBitmap.player_tile[a][b],al_get_bitmap_format(mBitmap.player_tile[a][b]),ALLEGRO_LOCK_READWRITE);

            for (x=0; x<20; x++)
               for (y=0; y<20; y++)
               {
                  ALLEGRO_COLOR p = al_get_pixel(mBitmap.player_tile[a][b], x, y);
                    float D = 0.1;
                    if (  (abs(p.r - mColor.pc[8].r) < D) &&
                          (abs(p.g - mColor.pc[8].g) < D) &&
                          (abs(p.b - mColor.pc[8].b) < D) ) al_put_pixel(x, y, mColor.pc[(8+cs)]);

                    if (  (abs(p.r - mColor.pc[56].r) < D) &&
                          (abs(p.g - mColor.pc[56].g) < D) &&
                          (abs(p.b - mColor.pc[56].b) < D) ) al_put_pixel(x, y, mColor.pc[(56+cs)]);

                    if (  (abs(p.r - mColor.pc[136].r) < D) &&
                          (abs(p.g - mColor.pc[136].g) < D) &&
                          (abs(p.b - mColor.pc[136].b) < D) ) al_put_pixel(x, y, mColor.pc[(136+cs)]);
               }
           al_unlock_bitmap(mBitmap.player_tile[a][b]);
           al_convert_mask_to_alpha(mBitmap.player_tile[a][b], al_map_rgb(0, 0, 0)) ;
         }
      }
   }

   if (0)
   {
       // show all new player shapes
       al_set_target_backbuffer(mDisplay.display);
       for (a=0; a<16; a++)
          for (b=0; b<24; b++)
             al_draw_bitmap(mBitmap.player_tile[a][b], b*20, a*20, 0);
       al_flip_display();
       mInput.tsw();

       al_rest(10);


   }

   if (0)
   {
       // save to disk
       al_set_target_bitmap(mBitmap.ptilemap);
       for (a=0; a<16; a++)
          for (b=0; b<24; b++)
             al_draw_bitmap(mBitmap.player_tile[a][b], b*20, a*20, 0);

       al_set_target_backbuffer(mDisplay.display);
       al_draw_bitmap(mBitmap.ptilemap, 0, 0, 0);

       al_flip_display();

       al_save_bitmap("bitmaps/player_tiles.png", mBitmap.ptilemap);

       mInput.tsw();
   }
}








void mwBitmapTools::color_shiftc(ALLEGRO_BITMAP *b, int sc, int cs, int x, int y)
{
   ALLEGRO_COLOR p = al_get_pixel(b, x, y);
   float D = 0.1;
   for (int c=sc; c<255; c+=16)
      if (  (abs(p.r - mColor.pc[c].r) < D) && (abs(p.g - mColor.pc[c].g) < D) && (abs(p.b - mColor.pc[c].b) < D) )
         al_put_pixel(x, y, mColor.pc[c+cs]);
}

void mwBitmapTools::color_shift4(ALLEGRO_BITMAP *b, int sc, int cs1, int cs2, int cs3, int cs4)
{
   al_set_target_bitmap(b);
   al_draw_bitmap(mBitmap.sprite[200], 0, 0, 0);
   al_lock_bitmap(b, al_get_bitmap_format(b), ALLEGRO_LOCK_READWRITE);
   for (int x=2; x<7; x++)
      for (int y=0; y<20; y++)
         color_shiftc(b, sc, cs1, x, y);
   for (int x=7; x<11; x++)
      for (int y=0; y<20; y++)
         color_shiftc(b, sc, cs2, x, y);
   for (int x=11; x<15; x++)
      for (int y=0; y<20; y++)
         color_shiftc(b, sc, cs3, x, y);
   for (int x=15; x<18; x++)
      for (int y=0; y<20; y++)
         color_shiftc(b, sc, cs4, x, y);
   al_unlock_bitmap(b);
   al_convert_mask_to_alpha(b, al_map_rgb(0, 0, 0)) ;
}

void mwBitmapTools::color_shift3(ALLEGRO_BITMAP *b, int sc, int cs1, int cs2, int cs3)
{
   al_set_target_bitmap(b);
   al_draw_bitmap(mBitmap.sprite[200], 0, 0, 0);
   al_lock_bitmap(b, al_get_bitmap_format(b), ALLEGRO_LOCK_READWRITE);
   for (int x=2; x<8; x++)
      for (int y=0; y<20; y++)
         color_shiftc(b, sc, cs1, x, y);
   for (int x=8; x<13; x++)
      for (int y=0; y<20; y++)
         color_shiftc(b, sc, cs2, x, y);
   for (int x=13; x<18; x++)
      for (int y=0; y<20; y++)
         color_shiftc(b, sc, cs3, x, y);
   al_unlock_bitmap(b);
   al_convert_mask_to_alpha(b, al_map_rgb(0, 0, 0)) ;
}

void mwBitmapTools::color_shift2(ALLEGRO_BITMAP *b, int sc, int cs1, int cs2)
{
   al_set_target_bitmap(b);
   al_draw_bitmap(mBitmap.sprite[200], 0, 0, 0);
   al_lock_bitmap(b, al_get_bitmap_format(b), ALLEGRO_LOCK_READWRITE);
   for (int x=0; x<10; x++)
      for (int y=0; y<20; y++)
         color_shiftc(b, sc, cs1, x, y);
   for (int x=10; x<20; x++)
      for (int y=0; y<20; y++)
         color_shiftc(b, sc, cs2, x, y);
   al_unlock_bitmap(b);
   al_convert_mask_to_alpha(b, al_map_rgb(0, 0, 0)) ;
}



void mwBitmapTools::color_shift(ALLEGRO_BITMAP *b, int sc, int cs)
{
   al_set_target_bitmap(b);
//   al_draw_bitmap(mBitmap.tile[200], 0, 0, 0);
   al_lock_bitmap(b, al_get_bitmap_format(b), ALLEGRO_LOCK_READWRITE);
   for (int x=0; x<20; x++)
      for (int y=0; y<20; y++)
         color_shiftc(b, sc, cs, x, y);

   al_unlock_bitmap(b);
   al_convert_mask_to_alpha(b, al_map_rgb(0, 0, 0)) ;
}


void mwBitmapTools::colorize_tile(void)
{
   ALLEGRO_BITMAP *switch_tiles[32];
   for (int d=0; d<32; d++) switch_tiles[d] = al_create_bitmap(20, 20);

   int sc = 9; // source color

   color_shift(switch_tiles[0], sc, 6); // tile 0 - white

   color_shift(switch_tiles[1], sc, 0);  // tile 1 - green
   color_shift(switch_tiles[2], sc, 1);  // tile 2 - red
   color_shift(switch_tiles[3], sc, 3);  // tile 3 - blue
   color_shift(switch_tiles[4], sc, -1); // tile 1 - purple

   color_shift2(switch_tiles[5],  sc, 0, 1);  // gr
   color_shift2(switch_tiles[6],  sc, 0, 3);  // gb
   color_shift2(switch_tiles[7],  sc, 0, -1); // gp
   color_shift2(switch_tiles[8],  sc, 1, 3);  // rb
   color_shift2(switch_tiles[9],  sc, 1, -1); // rp
   color_shift2(switch_tiles[10], sc, 3, -1); // bp


   color_shift3(switch_tiles[11], sc, 0, 1, 3);  // grb
   color_shift3(switch_tiles[12], sc, 0, 1, -1); // grp
   color_shift3(switch_tiles[13], sc, 0, 3, -1); // gbp
   color_shift3(switch_tiles[14], sc, 1, 0, -1); // rbp

   color_shift4(switch_tiles[15], sc, 0, 1, 3, -1); // grbp

   // fill second half and draw yin yang
   for (int d=0; d<16; d++)
   {
      al_set_target_bitmap(switch_tiles[d+16]);
      al_draw_bitmap(switch_tiles[d], 0, 0, 0);
      al_draw_bitmap(mBitmap.sprite[204], 0, 1, 0);
      al_set_target_bitmap(switch_tiles[d]);
      al_draw_bitmap(mBitmap.sprite[205], 0, 1, 0);

      al_convert_mask_to_alpha(switch_tiles[d], al_map_rgb(0, 0, 0)) ;
      al_convert_mask_to_alpha(switch_tiles[d+16], al_map_rgb(0, 0, 0)) ;
   }

   // draw all the bitmaps
   al_set_target_backbuffer(mDisplay.display);
//   al_clear_to_color(al_map_rgb(255, 10, 10));
//   al_clear_to_color(al_map_rgb(100, 100, 100));
   al_clear_to_color(al_map_rgb(0, 0, 0));
   for (int d=0; d<32; d++) al_draw_bitmap(switch_tiles[d], 20+d*20, 200, 0);

   al_flip_display();
   mInput.tsw(); // wait for keypress



   // show tilemap before
   al_set_target_backbuffer(mDisplay.display);
   al_clear_to_color(al_map_rgb(0,0,0));
   al_draw_bitmap(mBitmap.stilemap, 0, 0, 0);
   al_flip_display(); mInput.tsw(); // wait for keypress


   // draw on the tilemap
   al_set_target_bitmap(mBitmap.stilemap);
   // erase
//   for (int d=0; d<32; d++) al_draw_bitmap(mBitmap.tile[0], d*20, 60, 0);

   al_draw_filled_rectangle(0, 60, 640, 80, mColor.pc[0]);

   // show tilemap after erase
   al_set_target_backbuffer(mDisplay.display);
   al_clear_to_color(al_map_rgb(0,0,0));
   al_draw_bitmap(mBitmap.stilemap, 0, 0, 0);
   al_flip_display(); mInput.tsw(); // wait for keypress


   al_set_target_bitmap(mBitmap.stilemap);
   for (int d=0; d<32; d++) al_draw_bitmap(switch_tiles[d], d*20, 60, 0);

   al_convert_mask_to_alpha(mBitmap.stilemap, al_map_rgb(0, 0, 0)) ;

   al_save_bitmap("bitmaps/tiles.png", mBitmap.stilemap);

   for (int d=0; d<32; d++) al_destroy_bitmap(switch_tiles[d]);


   al_set_target_backbuffer(mDisplay.display);
   al_clear_to_color(al_map_rgb(0,0,0));
   al_draw_bitmap(mBitmap.stilemap, 0, 0, 0);
   al_flip_display(); mInput.tsw(); // wait for keypress

}

void mwBitmapTools::combine_tile(void)
{
   // show tilemap before
   al_set_target_backbuffer(mDisplay.display);
   al_clear_to_color(al_map_rgb(0,0,0));
   al_draw_bitmap(mBitmap.stilemap, 0, 0, 0);
   al_flip_display(); mInput.tsw(); // wait for keypress

   al_set_target_bitmap(mBitmap.sprite[534]);
   al_draw_bitmap(mBitmap.sprite[440], 0, 0, 0);

   al_set_target_bitmap(mBitmap.sprite[535]);
   al_draw_bitmap(mBitmap.sprite[440], 0, 0, 0);

   // show tilemap after
   al_set_target_backbuffer(mDisplay.display);
   al_clear_to_color(al_map_rgb(0,0,0));
   al_draw_bitmap(mBitmap.stilemap, 0, 0, 0);
   al_flip_display(); mInput.tsw(); // wait for keypress

  //   al_save_bitmap("bitmaps/tiles.png", mBitmap.tilemap);

}






// used by widget button to get new tile for block manip and blockwalker
// now used in tile helper also
// gets block with flags from level
void mwBitmapTools::select_bitmap_from_level(int &tn)
{
   int quit = 0;
   while (!quit)
   {
      mWM.redraw_level_editor_background(0);
      mMiscFnx.crosshairs_full(mWM.gx*20+10, mWM.gy*20+10, 15, 1);
      mScreen.draw_scaled_level_region_to_display();

      int local_point_item_type = 1;
      int local_point_item_num = mLevel.l[mWM.gx][mWM.gy];

      int swx1 = 200;
      int swy1 = 200;

      int swx2 = swx1 + 144;
      int swy2 = swy1 + 32;  // above flags

      // erase background and frame
      int swy3 = swy2 + 174; // below flags
      // swy3 += mLoop.pct_y;

      al_draw_filled_rectangle(              swx1-2, swy1-2,  swx2+2, swy3, mColor.pc[0]);
      al_draw_rectangle(                     swx1-2, swy1-2,  swx2+2, swy3, mColor.pc[13], 1);

      // view item area
      al_draw_rectangle(                     swx1,    swy1,   swx2,   swy2, mColor.pc[9], 1);
      al_draw_text(mFont.pr8, mColor.pc[15], swx1+24, swy1+2, 0, "Choose Block");
      mWM.mW[1].em_show_item_info(           swx1+2,  swy1+9, 9, local_point_item_type, local_point_item_num);

      // flags section
      int ftx = swx1+11;
      int fty = swy1+35;
      int ys = 10; // y spacing
      draw_flag_text(ftx+4, fty, ys, 15);

      int frw = 6;                // flag rectangle width
      int frh = 6;                // flag rectangle height
      int frx = ftx-frw-2;        // flag rectangle x
      int fry = fty - (frh/2)+4;  // flag rectangle y
      draw_flag_rects(local_point_item_num, frx, fry, frw, frh, ys);

      if (mInput.mouse_b[1][0])
      {
         while (mInput.mouse_b[1][0]) mEventQueue.proc(1); // wait for release
         tn = local_point_item_num;
         quit = 1;
      }
      if (mInput.key[ALLEGRO_KEY_ESCAPE][0])
      {
         while (mInput.key[ALLEGRO_KEY_ESCAPE][0]) mEventQueue.proc(1);
         quit = 1;
      }
   }
}

// used by tile editor to choose new tile
int mwBitmapTools::select_bitmap()
{
   int quit = 0;
   while (!quit)
   {
      mEventQueue.proc(1);

      al_flip_display();
      al_clear_to_color(al_map_rgb(0,0,0));
      al_draw_text(mFont.pr8, mColor.pc[15], 10, 644, 0, "Select a Tile with b1");
      al_draw_text(mFont.pr8, mColor.pc[15], 10, 654, 0, "b2 or ESC to exit      ");

      // draw 32x32 bitmaps
      for (int y = 0; y < 32; y++)
         for (int x = 0; x < 32; x++)
            al_draw_bitmap(mBitmap.tile[x+(y*32)],x*20, y*20, 0);
      al_draw_rectangle(0.5, 0.5, 640.5, 640.5, mColor.pc[13], 1);

      for (int x=0; x<=32; x++) al_draw_line(x*20, 0, x*20, 640, mColor.pc[15+128], 0);
      for (int y=0; y<=32; y++) al_draw_line(0, y*20, 640, y*20, mColor.pc[15+128], 0);

      if ((mInput.mouse_y < 640) && (mInput.mouse_x < 640))
      {
         int pointer = (mInput.mouse_x/20) + (mInput.mouse_y/20) * 32 ;
         al_draw_textf(mFont.pr8, mColor.pc[13], 522, 648, 0, "pointer %-2d", pointer );
         al_draw_bitmap(mBitmap.tile[pointer], 620, 642, 0);
         al_draw_rectangle(518, 640.5, 640.5, 662.5, mColor.pc[13], 1);
         if (mInput.mouse_b[1][3]) return pointer;
      }
      if ((mInput.key[ALLEGRO_KEY_ESCAPE][3]) || (mInput.mouse_b[2][3])) quit = 1;
   }
   return 0;
}




// used by animation_sequence_editor() only to choose a single bitmaps
int mwBitmapTools::select_bitmap_ans()
{
   int quit = 0;
   while (!quit)
   {
      mEventQueue.proc(1);

      al_flip_display();
      al_clear_to_color(al_map_rgb(0,0,0));
      al_draw_text(mFont.pr8, mColor.pc[15], 10, 644, 0, "Select a Tile with b1");
      al_draw_text(mFont.pr8, mColor.pc[15], 10, 654, 0, "b2 or ESC to exit      ");

      // draw 32x32 bitmaps
      for (int y = 0; y < 32; y++)
         for (int x = 0; x < 32; x++)
            al_draw_bitmap(mBitmap.sprite[x+(y*32)],x*20, y*20, 0);
      al_draw_rectangle(0.5, 0.5, 640.5, 640.5, mColor.pc[13], 1);

      for (int x=0; x<=32; x++) al_draw_line(x*20, 0, x*20, 640, mColor.pc[15+128], 0);
      for (int y=0; y<=32; y++) al_draw_line(0, y*20, 640, y*20, mColor.pc[15+128], 0);

      if ((mInput.mouse_y < 640) && (mInput.mouse_x < 640))
      {
         int pointer = (mInput.mouse_x/20) + (mInput.mouse_y/20) * 32 ;
         al_draw_textf(mFont.pr8, mColor.pc[13], 522, 648, 0, "pointer %-2d", pointer );
         al_draw_bitmap(mBitmap.sprite[pointer], 620, 642, 0);
         al_draw_rectangle(518, 640.5, 640.5, 662.5, mColor.pc[13], 1);
         if (mInput.mouse_b[1][3]) return pointer;
      }
      if ((mInput.key[ALLEGRO_KEY_ESCAPE][3]) || (mInput.mouse_b[2][3])) quit = 1;
   }
   return 0;
}





// this is used by animation_sequence_editor() only to choose new bitmaps for sequences
int mwBitmapTools::select_bitmap_ans(int zzindx, int &bmp_index)
{
   char msg[1024];
   int quit = 0;
   while (!quit)
   {
      mEventQueue.proc(1);

      al_flip_display();
      al_clear_to_color(al_map_rgb(0,0,0));
      al_draw_text(mFont.pr8, mColor.pc[9], 0, 642, 0, "Select a Bitmap with b1");
      al_draw_text(mFont.pr8, mColor.pc[9], 0, 650, 0, "b2 or ESC to exit      ");

      // draw 32x32 bitmaps
      for (int y = 0; y < 32; y++)
         for (int x = 0; x < 32; x++)
            al_draw_bitmap(mBitmap.sprite[x+(y*32)],x*20, y*20, 0);
      al_draw_rectangle(0.5, 0.5, 640.5, 640.5, mColor.pc[13], 1);

      // gridlines
      for (int x=0; x<=32; x++) al_draw_line(x*20, 0, x*20, 640, mColor.pc[15+128], 0);
      for (int y=0; y<=32; y++) al_draw_line(0, y*20, 640, y*20, mColor.pc[15+128], 0);

      int xc = 180;
      int yc = 648;

      al_draw_textf(mFont.pr8, mColor.pc[14], 80, 666, 0, "Get Shape %d ", mBitmap.zz[4][zzindx] );

      sprintf(msg, "Current Sequence %d", zzindx);
      int l = 2+strlen(msg)*4;
      al_draw_text(mFont.pr8, mColor.pc[13], xc+150, yc+2, ALLEGRO_ALIGN_CENTER, msg);
      al_draw_rectangle(xc+150-l, yc+1, xc+150+l, yc+11, mColor.pc[13], 1);

      for (int c=0; c < mBitmap.zz[4][zzindx] + 1; c++)   // show current seq shapes
         if (( mBitmap.zz[5+c][zzindx] < NUM_SPRITES) && (mBitmap.zz[5+c][zzindx] > 0))
            al_draw_bitmap(mBitmap.sprite[ mBitmap.zz[5+c][zzindx] ], xc+1+c*20, yc+12, 0);
      al_draw_rectangle(xc+0.5, yc+11.5, xc+302.5, yc+32.5, mColor.pc[13], 1);


      if ((mInput.mouse_y < 640) && (mInput.mouse_x < 640))
      {
         int pointer = (mInput.mouse_x/20) + (mInput.mouse_y/20) * 32 ;
         al_draw_textf(mFont.pr8, mColor.pc[13], 522, 648, 0, "pointer %-2d", pointer );
         al_draw_bitmap(mBitmap.sprite[pointer], 620, 642, 0);

         al_draw_rectangle(518, 640.5, 640.5, 662.5, mColor.pc[13], 1);

         if (mInput.mouse_b[1][3])
         {
            bmp_index = pointer;
            return 1;
         }
      }
      if ((mInput.key[ALLEGRO_KEY_ESCAPE][3]) || (mInput.mouse_b[2][3])) quit = 1;

   }
   return -1;
}

void mwBitmapTools::animation_sequence_editor(void)
{
   char msg[1024];
   int zzindx = 3;
   int pointer = zzindx;
   int quit = 0;
   int xa = 20;
   int xb = 220;

   al_set_target_backbuffer(mDisplay.display);
   al_show_mouse_cursor(mDisplay.display);

   while (!quit)
   {

      mEventQueue.proc(1);

      if (mEventQueue.program_update)
      {
         mBitmap.update_animation();
         mEventQueue.program_update = 0;
      }

      al_flip_display();
      al_clear_to_color(al_map_rgb(0,0,0));

      sprintf(msg, "Animation Sequence Editor");
      int l = 2+strlen(msg)*4;
      al_draw_text(mFont.pr8, mColor.pc[9], 320, 20, ALLEGRO_ALIGN_CENTER, msg);
      al_draw_rectangle(320-l, 19, 320+l, 29, mColor.pc[9], 1);

      if (pointer != -1)
      {
         sprintf(msg, "Pointer %d", pointer);
         l = 2+strlen(msg)*8;
         al_draw_text(mFont.pr8, mColor.pc[9], 643.5 - l, 192, 0, msg);
         al_draw_rectangle(643.5-l-2, 191, 642.5, 201, mColor.pc[9], 1);
      }


      int csx1 = 20;
      int ts = 40; // tile spacing
      int csw = ts*15; // width
      int csm = csx1 + csw/2; // middle


      sprintf(msg, "Current Sequence %d",zzindx);
      l = 2+strlen(msg)*4;
      al_draw_text(mFont.pr8, mColor.pc[13], csm, 202, ALLEGRO_ALIGN_CENTER, msg);
      al_draw_rectangle(csm-l, 201, csm+l, 211, mColor.pc[13], 1);

      for (int c=0; c<15; c++)   // frame
         al_draw_rectangle(csx1+c*ts, 211.5, csx1+c*ts+ts, 232.5, mColor.pc[13], 1);

      for (int c = 0; c < mBitmap.zz[4][zzindx] + 1; c++)   // show current seq shapes
         if (( mBitmap.zz[5+c][zzindx] < NUM_SPRITES) && (mBitmap.zz[5+c][zzindx] > 0))
         {
            int tn = mBitmap.zz[5+c][zzindx];
            int fx1 = csx1+c*ts; // frame x1
            int tilex = fx1 + (ts-20)/2;
            int textx = fx1 + ts/2;
            al_draw_bitmap(mBitmap.sprite[tn], tilex, 212, 0);
            al_draw_textf(mFont.pr8, mColor.pc[13], textx, 236, ALLEGRO_ALIGN_CENTER, "%d", tn);
         }

      // if mouse on current sequence
      if ((mInput.mouse_y > 210) && (mInput.mouse_y < 232))
      {
         int c = (mInput.mouse_x - csx1) / ts;
         if ((c > -1) && (c<15))
         {
            al_draw_rectangle(csx1+c*ts, 211.5, csx1+c*ts+ts, 232.5, mColor.pc[10], 1);
            if (mInput.mouse_b[1][3]) mBitmap.zz[5+c][zzindx] = select_bitmap_ans();
         }
      }

      for (int c=0; c < 32; c++)   // draw 32x8 grid of animation sequences
         for (int x=0; x < 8; x++)
            if (mBitmap.zz[4][c + (x * 32)] != 0)
               if ((mBitmap.zz[0][c + (x * 32)] < NUM_SPRITES) && (mBitmap.zz[0][c + (x * 32)] > 0 ))
                  al_draw_bitmap(mBitmap.sprite[mBitmap.zz[0][c + (x * 32)]], 2+c*20, 30+x*20, 0);

      al_draw_rectangle(0.5, 29.5, 642.5, 190.5, mColor.pc[9], 1);

      // if mouse on grid, get current animation sequence pointer
      if ((mInput.mouse_y > 30) && (mInput.mouse_y < 190) && (mInput.mouse_x < 642))
      {
         pointer = (mInput.mouse_x-2) / 20 + (mInput.mouse_y-30) / 20 * 32;
         if ((pointer < 0) || (pointer > NUM_ANS-1)) pointer = -1;
      }
      else pointer = -1;

      if ((pointer != -1) && (mInput.mouse_b[1][0])) zzindx = pointer;

      int y5 = 260;
      if (mWidget.buttont(xa, y5, xb, 16, 0,0,0,0, 0,11,15,0, 1,0,1,0, "Clear and Get New Shapes"))
      {
         for (int c=0; c<20; c++) mBitmap.zz[c][zzindx] = 0;
         int as_quit = 0;
         while (!as_quit)
         {
            int bmp_index = 0;
            int x = select_bitmap_ans(zzindx, bmp_index);
            if (x == 1) // good return b1
            {
               mBitmap.zz[5 + mBitmap.zz[4][zzindx]][zzindx] = bmp_index;
               mBitmap.zz[4][zzindx]++; // set last shape to point at next
            }
            if (x == -1)  // abort esc
            {
               mBitmap.zz[4][zzindx]--;
               as_quit=1;
            }
            if (mBitmap.zz[4][zzindx] > 14) // last shape
            {
               mBitmap.zz[4][zzindx]--;
               as_quit=1;
            }
         }
      }
      y5+=4;
      mWidget.slideri(xa, y5, xb, 16, 0,0,0,0,  0,12,15,15,  0,0,1,0, mBitmap.zz[3][zzindx], 100, 0, 1, "Animation Delay:");
      y5+=4;
      if (mWidget.buttont(xa, y5, xb, 16, 0,0,0,0,    0,10,15,0, 1,0,1,0, "Save Changes")) mBitmap.save_sprit();

      if (mInput.key[ALLEGRO_KEY_DELETE][0]) // erase current sequence
      {
         while (mInput.key[ALLEGRO_KEY_DELETE][0]) mEventQueue.proc(1);
         for (int c=0; c<20; c++) mBitmap.zz[c][zzindx] = 0;
      }
      while ((mInput.key[ALLEGRO_KEY_ESCAPE][0]) || (mInput.mouse_b[2][0]))
      {
         mEventQueue.proc(1);
         quit = 1;
      }
   }
}



















void mwBitmapTools::draw_flag_text(int x, int y, int ys, int col)
{
   // called by:
   // select_bitmap_from_level()
   // draw_flags
   // edit_tile_flags
   al_draw_text(mFont.pr8, mColor.pc[col], x, y, 0, "SOLID_PLAYER");     y+=ys;
   al_draw_text(mFont.pr8, mColor.pc[col], x, y, 0, "SOLID_ENEMY");      y+=ys;
   al_draw_text(mFont.pr8, mColor.pc[col], x, y, 0, "SOLID_ITEM");       y+=ys;
   al_draw_text(mFont.pr8, mColor.pc[col], x, y, 0, "SOLID_PBUL");       y+=ys;
   al_draw_text(mFont.pr8, mColor.pc[col], x, y, 0, "SOLID_EBUL");       y+=ys;
   al_draw_text(mFont.pr8, mColor.pc[col], x, y, 0, "SEMISOLID_PLAYER"); y+=ys;
   al_draw_text(mFont.pr8, mColor.pc[col], x, y, 0, "SEMISOLID_ENEMY");  y+=ys;
   al_draw_text(mFont.pr8, mColor.pc[col], x, y, 0, "SEMISOLID_ITEM");   y+=ys;
   al_draw_text(mFont.pr8, mColor.pc[col], x, y, 0, "BOMBABLE");         y+=ys;
   al_draw_text(mFont.pr8, mColor.pc[col], x, y, 0, "BREAKABLE_PBUL");   y+=ys;
   al_draw_text(mFont.pr8, mColor.pc[col], x, y, 0, "BREAKABLE_EBUL");   y+=ys;
   al_draw_text(mFont.pr8, mColor.pc[col], x, y, 0, "LADDER_MOVE");      y+=ys;
   al_draw_text(mFont.pr8, mColor.pc[col], x, y, 0, "ROPE_MOVE");        y+=ys;
   al_draw_text(mFont.pr8, mColor.pc[col], x, y, 0, "KEY ACTIVE");       y+=ys;
   al_draw_text(mFont.pr8, mColor.pc[col], x, y, 0, "KEY COLOR1");       y+=ys;
   al_draw_text(mFont.pr8, mColor.pc[col], x, y, 0, "KEY COLOR0");       y+=ys;
   al_draw_text(mFont.pr8, mColor.pc[col], x, y, 0, "SHOW OVERLAY");     y+=ys;
}





int mwBitmapTools::draw_flag_rects(int tn, int x, int y, int w, int h, int ys)
// if mouse is on a checkbox button, return button number
// called by:
// select_bitmap_from_level()
// draw_flags
// draw_and_process_flag_rects
{
   int fa[17] {}; // make an array to store the flag status
   if (tn & PM_BTILE_SOLID_PLAYER)     fa[0]++;
   if (tn & PM_BTILE_SOLID_ENEMY)      fa[1]++;
   if (tn & PM_BTILE_SOLID_ITEM)       fa[2]++;
   if (tn & PM_BTILE_SOLID_PBUL)       fa[3]++;
   if (tn & PM_BTILE_SOLID_EBUL)       fa[4]++;
   if (tn & PM_BTILE_SEMISOLID_PLAYER) fa[5]++;
   if (tn & PM_BTILE_SEMISOLID_ENEMY)  fa[6]++;
   if (tn & PM_BTILE_SEMISOLID_ITEM)   fa[7]++;
   if (tn & PM_BTILE_BOMBABLE)         fa[8]++;
   if (tn & PM_BTILE_BREAKABLE_PSHOT)  fa[9]++;
   if (tn & PM_BTILE_BREAKABLE_ESHOT)  fa[10]++;
   if (tn & PM_BTILE_LADDER_MOVE)      fa[11]++;
   if (tn & PM_BTILE_ROPE_MOVE)        fa[12]++;
   if (tn & PM_BTILE_KEY_ACTIVE)       fa[13]++;
   if (tn & PM_BTILE_KEY_COLOR1)       fa[14]++;
   if (tn & PM_BTILE_KEY_COLOR0)       fa[15]++;
   if (tn & PM_BTILE_SHOW_OVERLAY)     fa[16]++;

   for (int i=0; i<17; i++)
   {
      int ya = y + (i * ys);
      if (fa[i] == 0) al_draw_rectangle       (x, ya, x+w, ya+h, mColor.pc[15], 1); // clear
      if (fa[i] == 1) al_draw_filled_rectangle(x, ya, x+w, ya+h, mColor.pc[15]);    // set
   }

   if ((mInput.mouse_x > x) && (mInput.mouse_x < x+w) && (mInput.mouse_y > y) && (mInput.mouse_y < y+h+(ys*16))) return (mInput.mouse_y-y)/ys;
   return -1;
}



// this common one calls text and rect and allows changes if applicable
// used by draw item, point item and block selection description
// called by:
// 2 x cm_draw_status_window()
// 1 x draw_selection_window()
// 6 x widgets
void mwBitmapTools::draw_flags(int x1, int y1, int& num, int& mpow, int view_only, int clear_background, int ignore_mpow)
{
   int x2 = x1+141;

   int ys = 10; // y spacing

   int y2 = y1 + ys * PM_BTILE_NUM_FLAGS;

   int frw = 6; // flag rectangle width
   int frh = 6; // flag rectangle height
   if (clear_background)
   {
      al_draw_filled_rectangle(x1-1, y1-1, x2+1, y2-2, mColor.pc[0]);
      al_draw_rectangle(       x1-2, y1-2, x2+1, y2-1, mColor.pc[15], 1);
   }

   // check if mouse on window
   mpow = 0;
   if (!ignore_mpow && mInput.mouse_x > x1-2 && mInput.mouse_x < x2+1 && mInput.mouse_y > y1-2 && mInput.mouse_y < y2-1) mpow = 1;

   // draw flags and get mouse highlight
   int highlight = draw_flag_rects(num, x1, y1, frw, frh, ys);

   if (!view_only)
   {
      if (highlight > -1)
      {
         al_draw_rectangle(x1-1, y1+(ys*highlight)-1, x1+frw+1, y1+frh+(ys*highlight)+1, mColor.pc[15], 1);
         if (mInput.mouse_b[1][0])
         {
            while (mInput.mouse_b[1][0]) mEventQueue.proc(1); // wait for release
            if (highlight ==  0) (num) ^= PM_BTILE_SOLID_PLAYER;
            if (highlight ==  1) (num) ^= PM_BTILE_SOLID_ENEMY;
            if (highlight ==  2) (num) ^= PM_BTILE_SOLID_ITEM;
            if (highlight ==  3) (num) ^= PM_BTILE_SOLID_PBUL;
            if (highlight ==  4) (num) ^= PM_BTILE_SOLID_EBUL;
            if (highlight ==  5) (num) ^= PM_BTILE_SEMISOLID_PLAYER;
            if (highlight ==  6) (num) ^= PM_BTILE_SEMISOLID_ENEMY;
            if (highlight ==  7) (num) ^= PM_BTILE_SEMISOLID_ITEM;
            if (highlight ==  8) (num) ^= PM_BTILE_BOMBABLE;
            if (highlight ==  9) (num) ^= PM_BTILE_BREAKABLE_PSHOT;
            if (highlight == 10) (num) ^= PM_BTILE_BREAKABLE_ESHOT;
            if (highlight == 11) (num) ^= PM_BTILE_LADDER_MOVE;
            if (highlight == 12) (num) ^= PM_BTILE_ROPE_MOVE;
            if (highlight == 13) (num) ^= PM_BTILE_KEY_ACTIVE;
            if (highlight == 14) (num) ^= PM_BTILE_KEY_COLOR1;
            if (highlight == 15) (num) ^= PM_BTILE_KEY_COLOR0;
            if (highlight == 16) (num) ^= PM_BTILE_SHOW_OVERLAY;
         }
      }
   }
   draw_flag_text(x1+12, y1, ys, 15);
}






// called 3x from edit_tile_flags()
void mwBitmapTools::draw_and_proc_flag_rects(int tn, int x, int y, int w, int h, int ys)
{
   int highlight = draw_flag_rects(mBitmap.tileFlags[tn], x, y, w, h, ys);
   if (highlight > -1)
   {
      al_draw_rectangle(x-1, y+(ys*highlight), x+w+1, y+h+(ys*highlight), mColor.pc[15], 1);
      if (mInput.mouse_b[1][0])
      {
         while (mInput.mouse_b[1][0]) mEventQueue.proc(1); // wait for release
         if (highlight ==  0) mBitmap.tileFlags[tn] ^= PM_BTILE_SOLID_PLAYER;
         if (highlight ==  1) mBitmap.tileFlags[tn] ^= PM_BTILE_SOLID_ENEMY;
         if (highlight ==  2) mBitmap.tileFlags[tn] ^= PM_BTILE_SOLID_ITEM;
         if (highlight ==  3) mBitmap.tileFlags[tn] ^= PM_BTILE_SOLID_PBUL;
         if (highlight ==  4) mBitmap.tileFlags[tn] ^= PM_BTILE_SOLID_EBUL;
         if (highlight ==  5) mBitmap.tileFlags[tn] ^= PM_BTILE_SEMISOLID_PLAYER;
         if (highlight ==  6) mBitmap.tileFlags[tn] ^= PM_BTILE_SEMISOLID_ENEMY;
         if (highlight ==  7) mBitmap.tileFlags[tn] ^= PM_BTILE_SEMISOLID_ITEM;
         if (highlight ==  8) mBitmap.tileFlags[tn] ^= PM_BTILE_BOMBABLE;
         if (highlight ==  9) mBitmap.tileFlags[tn] ^= PM_BTILE_BREAKABLE_PSHOT;
         if (highlight == 10) mBitmap.tileFlags[tn] ^= PM_BTILE_BREAKABLE_ESHOT;
         if (highlight == 11) mBitmap.tileFlags[tn] ^= PM_BTILE_LADDER_MOVE;
         if (highlight == 12) mBitmap.tileFlags[tn] ^= PM_BTILE_ROPE_MOVE;
         if (highlight == 13) mBitmap.tileFlags[tn] ^= PM_BTILE_KEY_ACTIVE;
         if (highlight == 14) mBitmap.tileFlags[tn] ^= PM_BTILE_KEY_COLOR1;
         if (highlight == 15) mBitmap.tileFlags[tn] ^= PM_BTILE_KEY_COLOR0;
         if (highlight == 16) mBitmap.tileFlags[tn] ^= PM_BTILE_SHOW_OVERLAY;
      }
   }
}




// new version to work with tile helper selection of tiles
void mwBitmapTools::draw_flag_rects_multiple_th(int x, int y)
{
   int w = 7;
   int h = 7;
   int ys = 10;
   int tx = x+ys;
   int ty = y;
//   ty += mLoop.pct_y;
//   tx += mLoop.pct_x;
   draw_flag_text(tx, ty, ys, 15);

   // make an array of flags to store the tallies
   int fa[PM_BTILE_NUM_FLAGS][2] {};

   for (int i=0; i<PM_BTILE_NUM_FLAGS; i++)
   {
      fa[i][0] = 0;
      fa[i][1] = 0;
   }

   // cycle the selection
   for (int cx=0; cx<100; cx++)
      for (int cy=0; cy<100; cy++)
         if (mTileHelper.thl[cx][cy])
         {
            int tn = mLevel.l[cx][cy];

            tn & PM_BTILE_SOLID_PLAYER        ? fa[0][1]++  : fa[0][0]++;
            tn & PM_BTILE_SOLID_ENEMY         ? fa[1][1]++  : fa[1][0]++;
            tn & PM_BTILE_SOLID_ITEM          ? fa[2][1]++  : fa[2][0]++;
            tn & PM_BTILE_SOLID_PBUL          ? fa[3][1]++  : fa[3][0]++;
            tn & PM_BTILE_SOLID_EBUL          ? fa[4][1]++  : fa[4][0]++;
            tn & PM_BTILE_SEMISOLID_PLAYER    ? fa[5][1]++  : fa[5][0]++;
            tn & PM_BTILE_SEMISOLID_ENEMY     ? fa[6][1]++  : fa[6][0]++;
            tn & PM_BTILE_SEMISOLID_ITEM      ? fa[7][1]++  : fa[7][0]++;
            tn & PM_BTILE_BOMBABLE            ? fa[8][1]++  : fa[8][0]++;
            tn & PM_BTILE_BREAKABLE_PSHOT     ? fa[9][1]++  : fa[9][0]++;
            tn & PM_BTILE_BREAKABLE_ESHOT     ? fa[10][1]++ : fa[10][0]++;
            tn & PM_BTILE_LADDER_MOVE         ? fa[11][1]++ : fa[11][0]++;
            tn & PM_BTILE_ROPE_MOVE           ? fa[12][1]++ : fa[12][0]++;
            tn & PM_BTILE_KEY_ACTIVE          ? fa[13][1]++ : fa[13][0]++;
            tn & PM_BTILE_KEY_COLOR1          ? fa[14][1]++ : fa[14][0]++;
            tn & PM_BTILE_KEY_COLOR0          ? fa[15][1]++ : fa[15][0]++;
            tn & PM_BTILE_SHOW_OVERLAY        ? fa[16][1]++ : fa[16][0]++;
         }

   for (int i=0; i<PM_BTILE_NUM_FLAGS; i++)
   {
      int type = 0;                           // default: empty
      if (fa[i][1]) type = 1;                 // if any set: filled
      if ((fa[i][1]) && (fa[i][0])) type = 2; // if both set and clear: mixed

      if (type == 0) al_draw_rectangle       (x, y + (ys*i), x+w, y+h+(ys*i),               mColor.pc[15], 1);
      if (type == 1) al_draw_filled_rectangle(x, y + (ys*i), x+w, y+h+(ys*i),               mColor.pc[15]);
      if (type == 2) al_draw_filled_triangle (x, y+h+(ys*i), x+w, y+h+(ys*i), x, y+(ys* i), mColor.pc[15]);

   }


   if ((mInput.mouse_x > x) && (mInput.mouse_x < x+w) && (mInput.mouse_y > y) && (mInput.mouse_y < y+(PM_BTILE_NUM_FLAGS*ys) ))
   {
      int point = (mInput.mouse_y-y)/ys;
      al_draw_rectangle(x-1, y+(ys*point)-1, x+w+1, y+h+(ys*point)+1, mColor.pc[15], 1);

      if (mInput.mouse_b[1][0])
      {
         while (mInput.mouse_b[1][0]) mEventQueue.proc(1);

         int action = 0;                    // default action is 'clear'
         if (fa[point][1] == 0) action = 1; // if no clears tallied, action is 'set'

         int flag = 0;
         if (point ==  0) flag = PM_BTILE_SOLID_PLAYER;
         if (point ==  1) flag = PM_BTILE_SOLID_ENEMY;
         if (point ==  2) flag = PM_BTILE_SOLID_ITEM;
         if (point ==  3) flag = PM_BTILE_SOLID_PBUL;
         if (point ==  4) flag = PM_BTILE_SOLID_EBUL;
         if (point ==  5) flag = PM_BTILE_SEMISOLID_PLAYER;
         if (point ==  6) flag = PM_BTILE_SEMISOLID_ENEMY;
         if (point ==  7) flag = PM_BTILE_SEMISOLID_ITEM;
         if (point ==  8) flag = PM_BTILE_BOMBABLE;
         if (point ==  9) flag = PM_BTILE_BREAKABLE_PSHOT;
         if (point == 10) flag = PM_BTILE_BREAKABLE_ESHOT;
         if (point == 11) flag = PM_BTILE_LADDER_MOVE;
         if (point == 12) flag = PM_BTILE_ROPE_MOVE;
         if (point == 13) flag = PM_BTILE_KEY_ACTIVE;
         if (point == 14) flag = PM_BTILE_KEY_COLOR1;
         if (point == 15) flag = PM_BTILE_KEY_COLOR0;
         if (point == 16) flag = PM_BTILE_SHOW_OVERLAY;

         // cycle the selection
         for (int cx=0; cx<100; cx++)
            for (int cy=0; cy<100; cy++)
               if (mTileHelper.thl[cx][cy])
               {
                  if (action == 0) mLevel.l[cx][cy] &= ~flag;
                  if (action == 1) mLevel.l[cx][cy] |= flag;
               }
      }
   }
}
























// called 2x from edit_tile_flags()
void mwBitmapTools::draw_flag_rects_multiple(int bx1, int by1, int bx2, int by2, int x, int y, int w, int h, int ys, int con, int cof, int highlight)
{
    // make an array of flags to store the tallies
   int fa[PM_BTILE_NUM_FLAGS][2] {};

   // cycle the selection
   for (int cx = bx1; cx < bx2; cx++)
      for (int cy = by1; cy < by2; cy++)
      {
         int tn = cx + (cy*32);
         mBitmap.tileFlags[tn] & PM_BTILE_SOLID_PLAYER        ? fa[0][1]++  : fa[0][0]++;
         mBitmap.tileFlags[tn] & PM_BTILE_SOLID_ENEMY         ? fa[1][1]++  : fa[1][0]++;
         mBitmap.tileFlags[tn] & PM_BTILE_SOLID_ITEM          ? fa[2][1]++  : fa[2][0]++;
         mBitmap.tileFlags[tn] & PM_BTILE_SOLID_PBUL          ? fa[3][1]++  : fa[3][0]++;
         mBitmap.tileFlags[tn] & PM_BTILE_SOLID_EBUL          ? fa[4][1]++  : fa[4][0]++;
         mBitmap.tileFlags[tn] & PM_BTILE_SEMISOLID_PLAYER    ? fa[5][1]++  : fa[5][0]++;
         mBitmap.tileFlags[tn] & PM_BTILE_SEMISOLID_ENEMY     ? fa[6][1]++  : fa[6][0]++;
         mBitmap.tileFlags[tn] & PM_BTILE_SEMISOLID_ITEM      ? fa[7][1]++  : fa[7][0]++;
         mBitmap.tileFlags[tn] & PM_BTILE_BOMBABLE            ? fa[8][1]++  : fa[8][0]++;
         mBitmap.tileFlags[tn] & PM_BTILE_BREAKABLE_PSHOT     ? fa[9][1]++  : fa[9][0]++;
         mBitmap.tileFlags[tn] & PM_BTILE_BREAKABLE_ESHOT     ? fa[10][1]++ : fa[10][0]++;
         mBitmap.tileFlags[tn] & PM_BTILE_LADDER_MOVE         ? fa[11][1]++ : fa[11][0]++;
         mBitmap.tileFlags[tn] & PM_BTILE_ROPE_MOVE           ? fa[12][1]++ : fa[12][0]++;
         mBitmap.tileFlags[tn] & PM_BTILE_KEY_ACTIVE          ? fa[13][1]++ : fa[13][0]++;
         mBitmap.tileFlags[tn] & PM_BTILE_KEY_COLOR1          ? fa[14][1]++ : fa[14][0]++;
         mBitmap.tileFlags[tn] & PM_BTILE_KEY_COLOR0          ? fa[15][1]++ : fa[15][0]++;
         mBitmap.tileFlags[tn] & PM_BTILE_SHOW_OVERLAY        ? fa[16][1]++ : fa[16][0]++;
      }

   for (int i=0; i<PM_BTILE_NUM_FLAGS; i++)
   {
                                    al_draw_rectangle       (x, y + (ys*i), x+w, y+h+(ys*i), mColor.pc[15], 1);            // empty box by default
      if (fa[i][0] == 0)            al_draw_filled_rectangle(x, y + (ys*i), x+w, y+h+(ys*i), mColor.pc[15]);               // no clears tallied, filled
      if ((fa[i][0]) && (fa[i][1])) al_draw_filled_triangle (x, y+h+(ys*i), x+w, y+h+(ys*i), x, y+(ys* i), mColor.pc[15]); // mixed
   }

   if (highlight > -1)
   {
      al_draw_rectangle(x-1, y+(ys*highlight)-1, x+w+1, y+h+(ys*highlight)+1, mColor.pc[15], 1);

      if (mInput.mouse_b[1][0])
      {
         while (mInput.mouse_b[1][0]) mEventQueue.proc(1);

         int action = 0;                        // default action is 'clear'
         if (fa[highlight][1] == 0) action = 1; // if no clears tallied, action is 'set'

         int action_flag = 0;
         if (highlight ==  0) action_flag = PM_BTILE_SOLID_PLAYER;
         if (highlight ==  1) action_flag = PM_BTILE_SOLID_ENEMY;
         if (highlight ==  2) action_flag = PM_BTILE_SOLID_ITEM;
         if (highlight ==  3) action_flag = PM_BTILE_SOLID_PBUL;
         if (highlight ==  4) action_flag = PM_BTILE_SOLID_EBUL;
         if (highlight ==  5) action_flag = PM_BTILE_SEMISOLID_PLAYER;
         if (highlight ==  6) action_flag = PM_BTILE_SEMISOLID_ENEMY;
         if (highlight ==  7) action_flag = PM_BTILE_SEMISOLID_ITEM;
         if (highlight ==  8) action_flag = PM_BTILE_BOMBABLE;
         if (highlight ==  9) action_flag = PM_BTILE_BREAKABLE_PSHOT;
         if (highlight == 10) action_flag = PM_BTILE_BREAKABLE_ESHOT;
         if (highlight == 11) action_flag = PM_BTILE_LADDER_MOVE;
         if (highlight == 12) action_flag = PM_BTILE_ROPE_MOVE;
         if (highlight == 13) action_flag = PM_BTILE_KEY_ACTIVE;
         if (highlight == 14) action_flag = PM_BTILE_KEY_COLOR1;
         if (highlight == 15) action_flag = PM_BTILE_KEY_COLOR0;

         for (int cx = bx1; cx < bx2; cx++) // cycle the selection
            for (int cy = by1; cy < by2; cy++)
            {
               int tn = cx + (cy*32);
               if (action == 0) mBitmap.tileFlags[tn] &= ~action_flag;
               if (action == 1) mBitmap.tileFlags[tn] |= action_flag;
            }
      }
   }
}

// called 3x from edit_tile_flags()
void mwBitmapTools::redraw_grid(mRect<int> tile_grid_rect, mRect<int> selection_rect, int gridlines, int mode, int selected_index) // draw 32x64 tiles, selection and gridlines
{
   for (int dx=0; dx<32; dx++)
      for (int dy=0; dy<64; dy++)
      {
         int tn = dx + (dy*32); // tile number
         int x = tile_grid_rect.x1 + (dx*20);
         int y = tile_grid_rect.y1 + (dy*20);

         al_draw_bitmap(mBitmap.tile[tn], x, y, 0);
      }

   if (gridlines)
   {
      for (int x=0; x<=32; x++) al_draw_line(tile_grid_rect.x1 + x*20, tile_grid_rect.y1 + 0,     tile_grid_rect.x1 + x*20, tile_grid_rect.y2,        mColor.pc[15+128], 0);
      for (int y=0; y<=64; y++) al_draw_line(tile_grid_rect.x1,        tile_grid_rect.y1 +  y*20, tile_grid_rect.x2,        tile_grid_rect.y1 + y*20, mColor.pc[15+128], 0);
   }

   tile_grid_rect.draw_rectangle(mColor.Forest, 2);


   if (mode == 0)
   {
      int sx = tile_grid_rect.x1 + (selected_index % 32)*20;
      int sy = tile_grid_rect.y1 + (selected_index / 32)*20;
      al_draw_rectangle(sx, sy, sx+20, sy+20, mColor.pc[14], 2);
   }

   if (mode == 1)
   {
      int sx1 = tile_grid_rect.x1 + selection_rect.x1*20;
      int sy1 = tile_grid_rect.y1 + selection_rect.y1*20;
      int sx2 = tile_grid_rect.x1 + selection_rect.x2*20;
      int sy2 = tile_grid_rect.y1 + selection_rect.y2*20;
      al_draw_rectangle(sx1, sy1, sx2, sy2, mColor.pc[14], 2);
   }



}


// blocking full screen dialog to edit default tile flags
// called from cm_process_menu_bar()
void mwBitmapTools::edit_tile_flags()
{

   // rect for position of main tile grid
   mRect<int> tile_grid_rect = mRect<int>::fromX1Y1WH(4, 4, 640, 1280);

   char msg[1024];
   int mode = 0;
   int quit = 0;
   int gridlines = 0;

   int current_selection = 0;


   // rect for selection (blocks of 20)
   mRect<int> selection_rect = mRect<int>::fromX1Y1WH(0, 0, 1, 1);


   al_set_target_backbuffer(mDisplay.display);
   al_show_mouse_cursor(mDisplay.display);


   while (!quit)
   {
      mEventQueue.proc(1);
      al_flip_display();
      al_clear_to_color(al_map_rgb(0,0,0));

      // draw the main background grid
      redraw_grid(tile_grid_rect, selection_rect, gridlines, mode, current_selection);


      // check if mouse is on grid, set variables and highlight grid item
      int pointer = 0;
      bool mouse_on_grid = false;
      if (tile_grid_rect.contains(mInput.mouse_x, mInput.mouse_y))
      {
         mouse_on_grid = true;
         int mx = (mInput.mouse_x - tile_grid_rect.x1)/20;
         int my = (mInput.mouse_y - tile_grid_rect.y1)/20;
         pointer = mx + (my*32); // tile the mouse is currently pointing at
         pointer = mMiscFnx.enforce_limit(pointer, 0, 2047);

         // show pointer on grid as lt blue rectangle
         al_draw_rectangle(tile_grid_rect.x1 + mx*20+1, tile_grid_rect.y1 + my*20+1, tile_grid_rect.x1 + mx*20+19, tile_grid_rect.y1 + my*20+19, mColor.pc[13], 1);
      }



      // all the other controls will be in a column just to the right of the main grid
      int cx = tile_grid_rect.x2 + 4;
      int cy = tile_grid_rect.y1;

      // flags section
      int ftx = cx + 41;
      int fty = cy + 53;

      int ys = 20; // y spacing

      draw_flag_text(ftx+4, fty, ys, 15);

      int frw = 12;               // flag rectangle width
      int frh = 12;               // flag rectangle height
      int frx = ftx-frw-2;        // flag rectangle x
      int fry = fty - (frh/2)+4;  // flag rectangle y


      // position of 'currently selected' flags
      int csx = ftx-frw-1;
      int csy = fty-ys-1;


      if (mode == 0) // show flags for one tile
      {
         // draw flags
         draw_and_proc_flag_rects(current_selection, frx, fry, frw, frh, ys);

         // frame flags
         al_draw_rectangle(csx-4, csy+15, csx+frw+2, csy+15+(PM_BTILE_NUM_FLAGS*ys), mColor.pc[10], 1);



         // draw bitmap
         al_draw_bitmap(mBitmap.tile[current_selection], csx-3,  csy-6, 0);

         // draw text
         al_draw_textf(mFont.pr8, mColor.pc[4],          csx+24, csy,   0, "Current Selection %-2d  ", current_selection );

         // frame bitmap and text
         al_draw_rectangle(                              csx-4,  csy-7, csx+195, csy+15, mColor.pc[10], 1);



      }

      if (mode == 1) // show the tallied flags multiple
      {
         // draw flags
         draw_flag_rects_multiple(selection_rect.x1, selection_rect.y1, selection_rect.x2, selection_rect.y2, frx, fry, frw, frh, ys, 10, 11, -1);

         // frame flags
         al_draw_rectangle(csx-4, csy+15, csx+frw+2, csy+15+(PM_BTILE_NUM_FLAGS*ys), mColor.pc[10], 1);

         // draw text
         al_draw_textf(mFont.pr8, mColor.pc[4],    csx+frw,     csy, 0, " %d Tile(s) Selected", selection_rect.w * selection_rect.h);

         // frame text
         al_draw_rectangle(csx-4, csy-7, csx+195, csy+15, mColor.pc[10], 1);
      }

      // mouse on flag rectangles -- this is to show highlight what flag is selected
      if ((mInput.mouse_x > frx) && (mInput.mouse_x < frx+frw) && (mInput.mouse_y > fry) && (mInput.mouse_y < fry+(PM_BTILE_NUM_FLAGS*ys) ))
      {
         int indx = (mInput.mouse_y-fry)/ys;
         if (mode == 0) draw_and_proc_flag_rects(current_selection, frx, fry, frw, frh, ys);
         if (mode == 1) draw_flag_rects_multiple(selection_rect.x1, selection_rect.y1, selection_rect.x2, selection_rect.y2, frx, fry, frw, frh, ys, 10, 11, indx);
      }

      if (mouse_on_grid)
      {
         // show the flags of the pointer
         csx -= 24;
         csy -= 25;
         draw_and_proc_flag_rects(pointer,      frx-24,  fry, frw, frh, ys);
         al_draw_rectangle(                      csx-4,  csy-7, csx+195,   csy+15, mColor.pc[13], 1); // frame for mouse pointer
         al_draw_bitmap(mBitmap.tile[pointer],   csx-3,  csy-6, 0);
         al_draw_textf(mFont.pr8, mColor.pc[15], csx+24, csy,   0, "Mouse Pointer %d", pointer);
         al_draw_rectangle(csx-4, csy+15, csx+frw+2, csy+40+(PM_BTILE_NUM_FLAGS*ys), mColor.pc[13], 1); // frame for buttons


         if (mInput.mouse_b[2][0])
         {
            while (mInput.mouse_b[2][0]) mEventQueue.proc(1);
            if (mode == 0) mBitmap.tileFlags[pointer] = mBitmap.tileFlags[current_selection];
         }

         if (mInput.mouse_b[1][0])
         {
            if (mode == 0) current_selection = pointer;
            if (mode == 1)
            {
               int bx1 = mInput.mouse_x/20;
               int by1 = mInput.mouse_y/20;
               int bx2 = bx1;
               int by2 = by1;
               int old_mouse_x = mInput.mouse_x;
               int old_mouse_y = mInput.mouse_y;

               while (mInput.mouse_b[1][0]) // trap while b1 is held
               {
                  mEventQueue.proc(1);
                  al_flip_display();
                  al_clear_to_color(al_map_rgb(0,0,0));

                  // get new b2 variables from mouse position
                  bx2 = bx1 + ((mInput.mouse_x - old_mouse_x+20)/20);
                  by2 = by1 + ((mInput.mouse_y - old_mouse_y+20)/20);

                  // set selection rectangle from bx variables
                  selection_rect.setX1Y1X2Y2(bx1, by1, bx2, by2);
                  selection_rect.enforceLimits(0, 0, 31, 63);

                  // redraw grid
                  redraw_grid(tile_grid_rect, selection_rect, gridlines, mode, current_selection);
               }

               // ensure top-right, bottom left format
               if (bx1 > bx2) { int temp = bx2; bx2 = bx1; bx1 = temp; }
               if (by1 > by2) { int temp = by2; by2 = by1; by1 = temp; }

               if (bx2 == bx1) bx2++; // don't let size be zero
               if (by2 == by1) by2++;

               selection_rect.setX1Y1X2Y2(bx1, by1, bx2, by2);
            }
         }
      }


      int ya = cy + (PM_BTILE_NUM_FLAGS*ys);

      // mode button
      ya+=60;
      sprintf(msg, "%s", "Mode:Single");
      if (mode == 1) sprintf(msg, "%s", "Mode:Multiple");
      if (mWidget.buttontcb(cx, ya, 0, 14, 0,0,0,0, 0,13,15,14, 1,0,0,0, msg)) mode = !mode;

      ya += 13;

      // mode text description
      if (mode == 0)
      {
         al_draw_rectangle(                     cx+0, ya, cx+508, ya+26, mColor.pc[13], 1);
         al_draw_text(mFont.pr8, mColor.pc[15], cx+2, ya+2,  0, "left  mouse button (b1) - select a tile");
         al_draw_text(mFont.pr8, mColor.pc[15], cx+2, ya+16, 0, "right mouse button (b2) - paste selected tile flags to new tile");
      }
      if (mode == 1)
      {
         al_draw_rectangle(                     cx+0, ya, cx+302, ya+26, mColor.pc[13], 1);
         al_draw_text(mFont.pr8, mColor.pc[15], cx+2, ya+2,  0, "- draw selection rectangle");
         al_draw_text(mFont.pr8, mColor.pc[15], cx+2, ya+16, 0, "- toggle flags for all selected tiles");
      }


      ya += 50;
      // gridlines check box
      mWidget.togglec(cx, ya, cx+80, 16,  0,0,0,0,  0, 0, 0, 0,  1,0,1,0, gridlines, "Gridlines", 15, 15);

      ya += 20;
      // save button
      if (mWidget.buttontcb(cx, ya, 0, 14, 0,0,0,0, 0,15,15,10, 1,0,0,0, "Save")) mBitmap.save_sprit();








      if (mInput.key[ALLEGRO_KEY_ESCAPE][0])
      {
         while (mInput.key[ALLEGRO_KEY_ESCAPE][0]) mEventQueue.proc(1);
         quit = 1;
      }
   }
}




























void mwBitmapTools::draw_gridlines_and_frame(mRect<int>r, int fd, int fc, int fw, int gd, int gc, int gw, int ts)
{
   if (gd) // gridline draw
   {
      for (int x=r.x1; x<=r.x2; x+=ts) al_draw_line(x, r.y1, x, r.y2, mColor.pc[gc], gw);
      for (int y=r.y1; y<=r.y2; y+=ts) al_draw_line(r.x1, y, r.x2, y, mColor.pc[gc], gw);
   }
   if (fd) r.draw_rectangle(mColor.pc[fc], fw);
}



void mwBitmapTools::draw_tilecount_overlay(int x1, int y1, int tile, int extended)
{
   char msg[80];
   int t = tile & PM_BTILE_TILENUM_MASK;
   int c = mGlobalLevelTool.tileCount[t][0];
   sprintf(msg, "%d", c);

   if (c)
   {
      // get x and y from tile number
      int x = t % 32;
      int y = t / 32;

      // get display position
      int ty = y1+y*22+4;
      int tx = x1+x*22+4;

      if (!extended)
      {
         if (c>99) sprintf(msg, "%s", "!");
         mMiscFnx.drawTextOnClearedBackground(tx, ty, msg, 15);
      }
      else
      {
         mMiscFnx.drawTextOnClearedBackground(tx, ty, msg, 15);
         for (int i=1; i<400; i++)
            if (mGlobalLevelTool.tileCount[t][i])
            {
               ty+=8;
               c = mGlobalLevelTool.tileCount[t][i];
               sprintf(msg, "%d-%d", i, c);
               mMiscFnx.drawTextOnClearedBackground(tx, ty, msg, 15);
            }
      }
   }
}


void mwBitmapTools::draw_tilecount_overlays(int x1, int y1)
{
   for (int x=0; x<32; x++)
      for (int y=0; y<64; y++)
         draw_tilecount_overlay(x1, y1, y*32+x, 0);
}



void mwBitmapTools::draw_flags_overlay(int x1, int y1, int tile, int flags)
{
   char msg[80];
   int t = tile & PM_BTILE_TILENUM_MASK;

   if (mBitmap.tileFlags[t] & flags)
   {
      // get x and y from tile number
      int x = t % 32;
      int y = t / 32;
      // get display position
      int ty = y1+y*22+4;
      int tx = x1+x*22+4;
      sprintf(msg, "%s", "!");
      mMiscFnx.drawTextOnClearedBackground(tx, ty, msg, 15);
   }
}


void mwBitmapTools::draw_flags_overlays(int x1, int y1, int flags)
{
   for (int x=0; x<32; x++)
      for (int y=0; y<64; y++)
         draw_flags_overlay(x1, y1, y*32+x, flags);
}


void mwBitmapTools::draw_mouse_pointer_button(int x2, int y2, ALLEGRO_BITMAP *b, int index)
{
   mRect<int> r = mRect<int>::fromX2Y2WH(x2, y2, 108, 22);
   r.draw_rounded_rectangle(2, 2, mColor.pc[15], 1);
   al_draw_bitmap(b, r.x1+2, r.y1+1, 0);
   al_draw_textf(mFont.pr8, mColor.pc[15], r.x1+26, r.y1+3,  0, "index:%d", index);
   al_draw_textf(mFont.pr8, mColor.pc[15], r.x1+26, r.y1+12, 0, "b2 to copy");
}





// this one has arbitrary first and second bmp file
void mwBitmapTools::copy_tiles()
{
   char msg[1024];

   int gridlines = 1;

   al_set_target_backbuffer(mDisplay.display);
   al_show_mouse_cursor(mDisplay.display);
   ALLEGRO_BITMAP *qtmp = al_create_bitmap(20, 20);
   ALLEGRO_BITMAP *qtmp2 = al_create_bitmap(20, 20);

   // main bitmap ------------------
   char b1_fn[100];
   char b1_fn2[100];
   sprintf(b1_fn2, "bitmaps/tiles.png");
   // convert to 'ALLEGRO_FS_ENTRY' to get platform specific fully qualified path
   ALLEGRO_FS_ENTRY *FS_fname1 = al_create_fs_entry(b1_fn2);
   sprintf(b1_fn, "%s", al_get_fs_entry_name(FS_fname1));
   ALLEGRO_BITMAP *b1 = nullptr;
   int reload_b1 = 2;


   mRect<int> b1r = mRect<int>::fromX1Y1WH(4, 26, 704, 704);
   int b1_tw{};

   int b1_mouse_tile_pointer = -1;


   // second bitmap ------------------
   char b2_fn[100];
   char b2_fn2[100];
   sprintf(b2_fn2, "bitmaps/sprites.png");
   // convert to 'ALLEGRO_FS_ENTRY' to get platform specific fully qualified path
   ALLEGRO_FS_ENTRY *FS_fname2 = al_create_fs_entry(b2_fn2);
   sprintf(b2_fn, "%s", al_get_fs_entry_name(FS_fname2));
   ALLEGRO_BITMAP *b2 = nullptr;
   int reload_b2 = 2;


   mRect<int> b2r = mRect<int>::fromX1Y1WH(740, 26, 704, 704);
   int b2_tw{};

   int b2_pad = 1; // my tilesets have a padding of 1
   int b2_ts = 20 + b2_pad*2;

   int quit = 0;
   while (!quit)
   {


      if (reload_b1)
      {
         if (reload_b1 == 2)
         {
            al_destroy_bitmap(b1);
            b1 = al_load_bitmap(b1_fn);
         }
         else
         {
            sprintf(b1_fn, "bitmaps/");
            if (mMiscFnx.mw_file_select("Load Bitmap File", b1_fn, ".png", 0))
            {
               al_destroy_bitmap(b1);
               b1 = al_load_bitmap(b1_fn);
               if (!b1)
               {
                  sprintf(msg, "Error Loading %s", b1_fn);
                  mInput.m_err(msg);
                  return;
               }
            }
            else
            {
               sprintf(b1_fn2, "bitmaps/tiles.png");
               // convert to 'ALLEGRO_FS_ENTRY' to get platform specific fully qualified path
               ALLEGRO_FS_ENTRY *FS_fname1 = al_create_fs_entry(b1_fn2);
               sprintf(b1_fn, "%s", al_get_fs_entry_name(FS_fname1));
               al_destroy_bitmap(b1);
               b1 = al_load_bitmap(b1_fn);

               if (!b1)
               {
                  sprintf(msg, "Error Loading %s", b1_fn2);
                  mInput.m_err(msg);
                  return;
               }
            }
         }
         if (b1)
         {
            b1r.setWH(al_get_bitmap_width(b1), al_get_bitmap_height(b1));
            b1_tw = b1r.w/22; // tile grid width
         }
         reload_b1 = 0;
      }


      if (reload_b2)
      {
         if (reload_b2 == 2)
         {
            al_destroy_bitmap(b2);
            b2 = al_load_bitmap(b2_fn);
         }
         else
         {
            sprintf(b2_fn, "bitmaps/");
            if (mMiscFnx.mw_file_select("Load Bitmap File", b2_fn, ".png", 0))
            {
               al_destroy_bitmap(b2);
               b2 = al_load_bitmap(b2_fn);
            }
         }
         if (!b2) mInput.m_err("Load Error");
         else
         {
            b2r.setWH(al_get_bitmap_width(b2), al_get_bitmap_height(b2));
            b2_tw = b2r.w/b2_ts; // tile grid width
         }
         reload_b2 = 0;
      }





//      if (reload_b2 == 3) // load and scale to 1/2
//      {
//         ALLEGRO_BITMAP *db2 = NULL;
//         sprintf(b2_fn, "bitmaps\\");
//         if (mMiscFnx.mw_file_select("Load Bitmap File", b2_fn, ".bmp", 0)) db2 = al_load_bitmap(b2_fn);
//         if (!db2) mInput.m_err("Load Error");
//         else
//         {
//            int db2_w = al_get_bitmap_width(db2);
//            int db2_h = al_get_bitmap_height(db2);
//            al_destroy_bitmap(b2);
//            b2 = al_create_bitmap(db2_w/2, db2_h/2);
//            al_set_target_bitmap(b2);
//            al_clear_to_color(al_map_rgb(0, 0, 0));
//            al_draw_scaled_bitmap(db2, 0, 0, db2_w, db2_h, 0, 0, db2_w/2, db2_h/2, 0);
//            al_set_target_backbuffer(mDisplay.display);
//            al_destroy_bitmap(db2);
//
//            b2_w = al_get_bitmap_width(b2);
//            b2_h = al_get_bitmap_height(b2);
//            b2_tw = b2_w/20; // tile grid width
//            b2_x2 = b2_x + b2_w;
//            b2_y2 = b2_y + b2_h;
//         }
//         reload_b2 = 0;
//      }






      mEventQueue.proc(1);
      al_flip_display();
      al_clear_to_color(al_map_rgb(0,0,0));




      // // draw b2_pointer button

//      mRect<int> dir = mRect<int>::fromX2Y2WH(b1r.x2-114, b1r.y1-2, 116, 22);

      mRect<int> dir = mRect<int>::fromX2Y2WH(b1r.x1+704-114, b1r.y1-2, 116, 22);

      al_set_target_backbuffer(mDisplay.display);
      dir.draw_rounded_rectangle(2, 2, mColor.pc[13], 1);
      al_draw_bitmap(qtmp, dir.x1+2, dir.y1+1, 0);
      al_draw_textf(mFont.pr8, mColor.pc[15], dir.x1+26, dir.y1+3,  0, "Draw Item");



      // gridlines check box
      int xa = b1r.x1 + 400;
      int ya = b1r.y1 - 20;

      mWidget.togglec(xa, ya, xa+20, 16,  0,0,0,0,  0, 0, 0, 0,  1,0,0,0, gridlines, "Grid", 15, 15);



      // main bitmap -----------------------------------------------------------------------------------------------
      al_draw_bitmap(b1, b1r.x1, b1r.y1, 0);


      // draw top controls, starting from left
      int b1_cx = b1r.x1;
      ya = b1r.y1 - 12;

      if (mWidget.buttontcb(b1_cx, ya, 0, 14, 0,0,0,0, 0,15,15,14, 1,0,0,0, "Load")) reload_b1 = 1;
      b1_cx+=50;
      if (mWidget.buttontcb(b1_cx, ya, 0, 14, 0,0,0,0, 0,15,15,14, 1,0,0,0, "Reload")) reload_b1 = 2;
      b1_cx+=66;
      if (mWidget.buttontcb(b1_cx, ya, 0, 14, 0,0,0,0, 0,15,15,10, 1,0,0,0, "Save")) mw_save_bitmap(b1_fn, b1);
      b1_cx+=64;



      // title with filename only, no path
      ALLEGRO_PATH *ap1 = al_create_path(b1_fn);
      al_draw_textf(mFont.pr8, mColor.pc[15], b1_cx, b1r.y1-10, 0, "%s", al_get_path_filename(ap1));
      al_destroy_path(ap1);


      draw_gridlines_and_frame(b1r, 1, 15+64, 1, gridlines, 15+128, 0, 22);

      draw_tilecount_overlays(b1r.x1, b1r.y1);

      //draw_flags_overlays(b1_x, b1_y, PM_BTILE_SHOW_SELECT_WIN);
//      draw_flags_overlays(b1_x, b1_y, PM_BTILE_SOLID_PLAYER);

/*
#define PM_BTILE_MOST_FLAGS        0b00111111111111110000000000000000
#define PM_BTILE_ALL_FLAGS         0b01111111111111110000000000000000
#define PM_BTILE_ALL_SOLID         0b00000000000111110000000000000000
#define PM_BTILE_ALL_SEMI          0b00000000111001110000000000000000
#define PM_BTILE_SOLID_PLAYER      0b00000000000000010000000000000000
#define PM_BTILE_SOLID_ENEMY       0b00000000000000100000000000000000
#define PM_BTILE_SOLID_ITEM        0b00000000000001000000000000000000
#define PM_BTILE_SOLID_PBUL        0b00000000000010000000000000000000
#define PM_BTILE_SOLID_EBUL        0b00000000000100000000000000000000
#define PM_BTILE_SEMISOLID_PLAYER  0b00000000001000000000000000000000
#define PM_BTILE_SEMISOLID_ENEMY   0b00000000010000000000000000000000
#define PM_BTILE_SEMISOLID_ITEM    0b00000000100000000000000000000000
#define PM_BTILE_BOMBABLE          0b00000001000000000000000000000000
#define PM_BTILE_BREAKABLE_PSHOT   0b00000010000000000000000000000000
#define PM_BTILE_BREAKABLE_ESHOT   0b00000100000000000000000000000000
#define PM_BTILE_LADDER_MOVE       0b00001000000000000000000000000000
#define PM_BTILE_ROPE_MOVE         0b00010000000000000000000000000000
#define PM_BTILE_SECRET            0b00100000000000000000000000000000
#define PM_BTILE_SHOW_SELECT_WIN   0b01000000000000000000000000000000

  */


      b1_mouse_tile_pointer = -1;

      if (b1r.contains(mInput.mouse_x, mInput.mouse_y))
      {
         draw_gridlines_and_frame(b1r, 1, 15+64, 1, gridlines, 15+64, 0, 22);

         // what tile is mouse pointing at?
         int mx = (mInput.mouse_x-b1r.x1)/22;
         int my = (mInput.mouse_y-b1r.y1)/22;
         b1_mouse_tile_pointer = mx + my * b1_tw;

         // mouse pointer offset relative to bitmap
         int bp_x1 = mx*22+1;
         int bp_y1 = my*22+1;

         // outline tile that mouse is pointing at
         int mx1 = b1r.x1 + bp_x1 - 1;
         int my1 = b1r.y1 + bp_y1 - 1;
         al_draw_rectangle(mx1, my1, mx1+22, my1+22, mColor.pc[10], 0);

         draw_tilecount_overlay(b1r.x1, b1r.y1, b1_mouse_tile_pointer, 1);

         // copy tile to qtmp2
         al_set_target_bitmap(qtmp2);
         al_clear_to_color(al_map_rgb(0, 0, 0));
         al_draw_bitmap_region(b1, bp_x1, bp_y1, 20, 20, 0, 0, 0);
         al_set_target_backbuffer(mDisplay.display);

         // draw b1 pointer button
         draw_mouse_pointer_button(b1r.x1+704, b1r.y1-2, qtmp2, b1_mouse_tile_pointer);


         if (mInput.mouse_b[1][0])
         {
            while (mInput.mouse_b[1][0]) mEventQueue.proc(1);
            al_set_target_bitmap(b1);
            //al_draw_filled_rectangle(bp_x1,   bp_y1,   bp_x1+20, bp_y1+20, mColor.pc[0]);   // erase
            al_draw_filled_rectangle(bp_x1-1, bp_y1-1, bp_x1+21, bp_y1+21, mColor.pc[0]);     // erase
            al_draw_bitmap(qtmp, bp_x1, bp_y1, 0);                                            // draw new tile
            al_set_target_backbuffer(mDisplay.display);
         }
         if (mInput.mouse_b[2][0])
         {
            while (mInput.mouse_b[2][0]) mEventQueue.proc(1);
            al_set_target_bitmap(qtmp);
            al_clear_to_color(al_map_rgb(0, 0, 0));
            al_draw_bitmap_region(b1, bp_x1, bp_y1, 20, 20, 0, 0, 0);
            al_set_target_backbuffer(mDisplay.display);
         }
         al_draw_textf(mFont.pr8, mColor.pc[13], dir.x1+26, dir.y1+12, 0, "b1 to paste"); // only show this line if mouse on main grid
      }












      // second bitmap -----------------------------------------------------------------------------------------------
      al_draw_bitmap(b2, b2r.x1, b2r.y1, 0);

      // title with filename only, no path
      ALLEGRO_PATH *ap = al_create_path(b2_fn);
      al_draw_textf(mFont.pr8, mColor.pc[15], b2r.x1+104,  b2r.y1-10, 0, "%s", al_get_path_filename(ap));
      al_destroy_path(ap);

      ya = b2r.y1-12;
      if (mWidget.buttontcb(b2r.x1,    ya, 0, 14, 0,0,0,0, 0,15,15,14, 1,0,0,0, "Load") )     reload_b2 = 1;


      char msg[80];
      sprintf(msg, "Pad:%d", b2_pad);
      if (mWidget.buttontcb(b2r.x1+48, ya, 0, 14, 0,0,0,0, 0,15,15,14, 1,0,0,0, msg) )
      {
         b2_pad = !b2_pad;
         b2_ts = 20 + b2_pad*2;
      }


      //if (mWidget.buttontcb(b2_x+42, ya, 0, 14, 0,0,0,0, 0,15,15,14, 1,0,0,0, "Load 1/2") ) reload_b2 = 3;



      draw_gridlines_and_frame(b2r, 1, 15+64, 1, gridlines, 15+128, 0, b2_ts); // dim

      // is mouse on grid2
      if (b2r.contains(mInput.mouse_x, mInput.mouse_y))
      {
         draw_gridlines_and_frame(b2r, 1, 15+64, 1, gridlines, 15+64, 0, b2_ts); // not dim

         // what tile is mouse pointing at?
         int mx = (mInput.mouse_x-b2r.x1) / b2_ts;
         int my = (mInput.mouse_y-b2r.y1) / b2_ts;

         int b2_mouse_tile_pointer = mx + my*b2_tw;

         // mouse pointer offset relative to bitmap
         int bp_x1 = mx*b2_ts + b2_pad;
         int bp_y1 = my*b2_ts + b2_pad;

         // outline tile that mouse is pointing at
         int mx1 = b2r.x1 + bp_x1 - b2_pad;
         int my1 = b2r.y1 + bp_y1 - b2_pad;
         al_draw_rectangle(mx1, my1, mx1+b2_ts, my1+b2_ts, mColor.pc[10], 0);

         // copy tile to qtmp2
         al_set_target_bitmap(qtmp2);
         al_clear_to_color(al_map_rgb(0, 0, 0));
         al_draw_bitmap_region(b2, bp_x1, bp_y1, 20, 20, 0, 0, 0);
         al_set_target_backbuffer(mDisplay.display);


         // draw b2_pointer button
         draw_mouse_pointer_button(b2r.x2, b2r.y1-2, qtmp2, b2_mouse_tile_pointer);


         if (mInput.mouse_b[2][0])
         {
            while (mInput.mouse_b[2][0]) mEventQueue.proc(1);
            al_set_target_bitmap(qtmp);
            al_clear_to_color(al_map_rgb(0, 0, 0));
            al_draw_bitmap_region(b2, bp_x1, bp_y1, 20, 20, 0, 0, 0);
            al_set_target_backbuffer(mDisplay.display);
         }
      }



















      if (mInput.key[ALLEGRO_KEY_ESCAPE][0])
      {
         while (mInput.key[ALLEGRO_KEY_ESCAPE][0]) mEventQueue.proc(1);
         quit = 1;
      }

      if (mInput.key[ALLEGRO_KEY_C][0])
      {
         while (mInput.key[ALLEGRO_KEY_C][0]) mEventQueue.proc(1);
         mGlobalLevelTool.countTiles();
         al_set_target_backbuffer(mDisplay.display);

      }


      if (mInput.key[ALLEGRO_KEY_S][0])
      {
         while (mInput.key[ALLEGRO_KEY_S][0]) mEventQueue.proc(1);
















//         mGlobalLevelTool.changeTileNumberRange(672, 640, 3);
//         mGlobalLevelTool.changeTileNumberRange(640, 672, 3);


//         mTileSets.create_tileset_single_faded_rect(28, mColor.Green);
  //       mTileSets.create_tileset_single_faded_rect(60, mColor.Red);
    //     mTileSets.create_tileset_single_faded_rect(92, mColor.Blue);
         //mTileSets.create_tileset_from_16_mega(844, 16, 21); // original hand modify grey bricks (do not run this or they will be overwritten)
//         mTileSets.create_tileset_from_16_mega(864, 20, 17); // red dirt with grass
         //mTileSets.create_tileset_from_24_atomic(704);

//         mTileSets.create_tileset_from_16_mega(912, 12, 21); // brown dirt with grass
//         mTileSets.create_tileset_from_16_mega(944, 12, 17); // orange brown dirt
//         mTileSets.create_tileset_from_16_mega(976, 28, 17); // grey
//         mTileSets.create_tileset_from_16_mega(1008, 28, 21); // outlined grey
//         mTileSets.modify_tile_set(); // for extended pipe tilesets
//         mTileSets.adjust_tile_set_color();
//         mTileSets.create_tileset_extended2(192); this is the very custom purple brick

      }


      if (b1_mouse_tile_pointer != -1 && mInput.key[ALLEGRO_KEY_M][0] && mInput.SHFT() && mInput.CTRL() )
      {
         while (mInput.key[ALLEGRO_KEY_M][0]) mEventQueue.proc(1);
         printf("Change tile number:%d\n", b1_mouse_tile_pointer);

         int quit2 = 0;
         while (!quit2)
         {

            mEventQueue.proc(1);
            al_flip_display();
            al_clear_to_color(al_map_rgb(0,0,0));

            al_draw_textf(mFont.pr8, mColor.pc[10], 10, 6, 0, "Warning!  -  Change all level references to tile:%d", b1_mouse_tile_pointer);

            // draw the main bitmap
            al_draw_bitmap(b1, b1r.x1, b1r.y1, 0);

            draw_gridlines_and_frame(b1r, 1, 15+64, 1, gridlines, 15+64, 0, 22);

            // is mouse on grid
            if (b1r.contains(mInput.mouse_x, mInput.mouse_y))
            {
               // what tile is mouse pointing at?
               int mx = (mInput.mouse_x-b1r.x1)/22;
               int my = (mInput.mouse_y-b1r.y1)/22;
               int pointer = mx + my*b1_tw;

               // mouse pointer offset relative to bitmap
               int bp_x1 = mx*22+1;
               int bp_y1 = my*22+1;

               // outline tile that mouse is pointing at
               int mx1 = b1r.x1 + bp_x1 - 1;
               int my1 = b1r.y1 + bp_y1 - 1;
               al_draw_rectangle(mx1, my1, mx1+22, my1+22, mColor.pc[10], 0);

               // draw b1_pointer button
               int b1p_x1= b1r.x1+1;
               int b1p_w1= 160;
               int b1p_y1= b1r.y2;

               al_set_target_backbuffer(mDisplay.display);
               al_draw_rounded_rectangle(b1p_x1, b1p_y1, b1p_x1+b1p_w1, b1p_y1+22, 2, 2, mColor.pc[15], 1);
               al_draw_bitmap(qtmp2, b1p_x1+2, b1p_y1+1, 0);
               al_draw_textf(mFont.pr8, mColor.pc[15], b1p_x1+26, b1p_y1+3,  0, "pointer:%d", pointer);
               al_draw_textf(mFont.pr8, mColor.pc[15], b1p_x1+26, b1p_y1+12, 0, "mouse b1 to change");

               if (mInput.mouse_b[1][0])
               {
                  while (mInput.mouse_b[1][0]) mEventQueue.proc(1);
                  mGlobalLevelTool.changeTileNumberRange(b1_mouse_tile_pointer, pointer);
                  quit2 = 1;
               }
            }



            if (mInput.key[ALLEGRO_KEY_ESCAPE][0])
            {
               while (mInput.key[ALLEGRO_KEY_ESCAPE][0]) mEventQueue.proc(1);
               quit2 = 1;
            }
         }
      }
   }

   al_destroy_bitmap(b1);
   al_destroy_bitmap(b2);
   al_destroy_bitmap(qtmp);
   al_destroy_bitmap(qtmp2);
}





void mwBitmapTools::move_tiles_and_refs(int s, int d, int r)
{
   copy_tile_range(s, d, r);
   mGlobalLevelTool.changeTileNumberRange(s, d, r);
}




void copy_tile(ALLEGRO_BITMAP *b, int s, int d)
{
   int sx = (s % 32)*22+1;
   int sy = (s / 32)*22+1;

   int dx = (d % 32)*22+1;
   int dy = (d / 32)*22+1;

   al_set_target_bitmap(b);
   al_draw_bitmap_region(b, sx, sy, 20, 20, dx, dy, 0);
}




void mwBitmapTools::copy_tile_range(int s, int d, int r)
{
   ALLEGRO_BITMAP *b1 = load_block_tiles_to_bitmap();
   if (!b1) return;

   for (int i=0; i<r; i++) copy_tile(b1, s+i, d+i);

   save_bitmap_to_block_tiles_file(b1);

   al_destroy_bitmap(b1);

}

