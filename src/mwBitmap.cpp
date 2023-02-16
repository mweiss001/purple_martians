// mwBitmap.cpp

#include "pm.h"
#include "mwBitmap.h"
#include "mwFont.h"
#include "mwBottomMessage.h"
#include "mwDisplay.h"
#include "e_visual_level.h"
#include "mwLogo.h"
#include "z_screen.h"

mwBitmap mwB;

ALLEGRO_BITMAP * mwBitmap::create_and_clear_bitmap(int x, int y)
{
   ALLEGRO_BITMAP * bmp = al_create_bitmap(x, y);
   al_set_target_bitmap(bmp);
   al_clear_to_color(al_map_rgba(0,0,0,0));
   return bmp;
}


// done at start only now
void mwBitmap::create_bitmaps(void)
{
   // this bitmap format will be used for all bitmaps, it is never changed
   al_set_new_bitmap_format(ALLEGRO_PIXEL_FORMAT_ANY_32_WITH_ALPHA);

   // these bitmap flags are only for the M_XXX memory bitmaps
   al_set_new_bitmap_flags(ALLEGRO_MEMORY_BITMAP);

   // create memory bitmaps as temp storage for restoring tilemaps after screen change
   M_tilemap  = create_and_clear_bitmap(640, 640);
   M_btilemap = create_and_clear_bitmap(640, 640);
   M_ptilemap = create_and_clear_bitmap(480, 320);
   M_dtilemap = create_and_clear_bitmap(160, 160);

   // this bitmap format is used for every other bitmap created, it is never changed
   al_set_new_bitmap_flags(ALLEGRO_NO_PRESERVE_TEXTURE | ALLEGRO_VIDEO_BITMAP);

   // create tilemap bitmaps
   tilemap  = create_and_clear_bitmap(640, 640);
   btilemap = create_and_clear_bitmap(640, 640);
   ptilemap = create_and_clear_bitmap(480, 320);
   dtilemap = create_and_clear_bitmap(160, 160);

   // create level_background and mwB.level_buffer bitmaps
   level_background  = create_and_clear_bitmap(2000, 2000);
   level_buffer      = create_and_clear_bitmap(2000, 2000);

   // create bottom message bitmaps
   mwBM.create_bitmaps();
}



void mwBitmap::rebuild_bitmaps(void)
{
   //printf("rebuild bitmaps\n");

   // rebuild main tiles
   al_set_target_bitmap(mwB.tilemap);
   al_clear_to_color(al_map_rgba(0,0,0,0));
   al_draw_bitmap(mwB.M_tilemap, 0, 0, 0);

   // rebuild block tiles
   al_set_target_bitmap(mwB.btilemap);
   al_clear_to_color(al_map_rgba(0,0,0,0));
   al_draw_bitmap(mwB.M_btilemap, 0, 0, 0);

   // rebuild player tiles
   al_set_target_bitmap(mwB.ptilemap);
   al_clear_to_color(al_map_rgba(0,0,0,0));
   al_draw_bitmap(mwB.M_ptilemap, 0, 0, 0);

   // rebuild door tiles
   al_set_target_bitmap(mwB.dtilemap);
   al_clear_to_color(al_map_rgba(0,0,0,0));
   al_draw_bitmap(mwB.M_dtilemap, 0, 0, 0);

   mF.load_fonts();


   init_level_background(0);
   mwD.set_display_transform();
   mwL.logo_text_bitmaps_create = 1;
   large_text_overlay_state = 0;
   text_title_bitmaps_create = 1;
   set_map_var();
   load_visual_level_select_done = 0;
}




void mwBitmap::reset_animation_sequences(void)
{
   for (int c=0; c<NUM_ANS; c++)
   {
     mwB.zz[2][c] = 0;        // reset the tally
     mwB.zz[1][c] = 0;        // set the bitmap index to 0
     mwB.zz[0][c] = mwB.zz[5][c]; // put the first shape in 0
   }
}

void mwBitmap::update_animation(void)
{
   // 0 = current shape
   // 1 = current shape index
   // 2 = count tally
   // 3 = seq change delay count
   // 4 = num of shapes in seq (15 shapes max)
   // 5 = shape 0
   // 19 = shape 14

   // printf("update_animation :%d\n", mwPS.frame_num);

   for (int y=0; y<NUM_ANS; y++)
      if (mwB.zz[4][y] != 0)
      {
         if ((++mwB.zz[2][y]) > mwB.zz[3][y])
         {
            mwB.zz[2][y] = 0;                             // reset tally
            mwB.zz[1][y]++;                               // next bitmap
            if (mwB.zz[1][y] > mwB.zz[4][y]) mwB.zz[1][y] = 0;    // is bitmap past end?
            mwB.zz[0][y] = mwB.zz[ 5 + mwB.zz[1][y] ] [y];        // put shape in 0
         }
      }
}












