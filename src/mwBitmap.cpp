// mwBitmap.cpp

#include "pm.h"
#include "mwBitmap.h"
#include "mwFont.h"
#include "mwBottomMessage.h"
#include "mwDisplay.h"
#include "mwVisualLevel.h"
#include "mwLogo.h"
#include "mwScreen.h"
#include "mwInput.h"
#include "mwLevel.h"
#include "mwLoop.h"

mwBitmap mBitmap;

ALLEGRO_BITMAP * mwBitmap::create_and_clear_bitmap(int x, int y)
{
   ALLEGRO_BITMAP * bmp = al_create_bitmap(x, y);
   al_set_target_bitmap(bmp);
   al_clear_to_color(al_map_rgba(0,0,0,0));
   return bmp;
}

// done only once at start
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
   M_dtilemap = create_and_clear_bitmap(160, 640);

   // this bitmap format is used for tiles
//   al_set_new_bitmap_flags(ALLEGRO_NO_PRESERVE_TEXTURE | ALLEGRO_VIDEO_BITMAP | ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR | ALLEGRO_MIPMAP);
   al_set_new_bitmap_flags(ALLEGRO_NO_PRESERVE_TEXTURE | ALLEGRO_VIDEO_BITMAP);

   // create tilemap bitmaps
   tilemap  = create_and_clear_bitmap(640, 640);
   btilemap = create_and_clear_bitmap(640, 640);
   ptilemap = create_and_clear_bitmap(480, 320);
   dtilemap = create_and_clear_bitmap(160, 640);

   // create bottom message bitmaps
   mBottomMessage.create_bitmaps();

//   al_set_new_bitmap_flags(ALLEGRO_NO_PRESERVE_TEXTURE | ALLEGRO_VIDEO_BITMAP);

   // create level_background and level_buffer bitmaps
   level_background  = create_and_clear_bitmap(2000, 2000);
   level_buffer      = create_and_clear_bitmap(2000, 2000);
}

void mwBitmap::rebuild_bitmaps(void)
{
   // rebuild main tiles
   al_set_target_bitmap(tilemap);
   al_clear_to_color(al_map_rgba(0,0,0,0));
   al_draw_bitmap(M_tilemap, 0, 0, 0);

   // rebuild block tiles
   al_set_target_bitmap(btilemap);
   al_clear_to_color(al_map_rgba(0,0,0,0));
   al_draw_bitmap(M_btilemap, 0, 0, 0);

   // rebuild player tiles
   al_set_target_bitmap(ptilemap);
   al_clear_to_color(al_map_rgba(0,0,0,0));
   al_draw_bitmap(M_ptilemap, 0, 0, 0);

   // rebuild door tiles
   al_set_target_bitmap(dtilemap);
   al_clear_to_color(al_map_rgba(0,0,0,0));
   al_draw_bitmap(M_dtilemap, 0, 0, 0);

   mFont.load_fonts();

   mLevel.load_level_icons();

   mScreen.init_level_background();
   mDisplay.set_display_transform();
   mLogo.logo_text_bitmaps_create = 1;
   large_text_overlay_state = 0;
   text_title_bitmaps_create = 1;
   mScreen.set_map_var();
   mVisualLevel.load_visual_level_select_done = 0;

   mLevel.level_stats_bmp_msg_type = 0;



}

void mwBitmap::reset_animation_sequences(void)
{
   for (int c=0; c<NUM_ANS; c++)
   {
     zz[2][c] = 0;        // reset the tally
     zz[1][c] = 0;        // set the bitmap index to 0
     zz[0][c] = zz[5][c]; // put the first shape in 0
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

   // printf("update_animation :%d\n", mLoop.frame_num);

   for (int y=0; y<NUM_ANS; y++)
      if (zz[4][y] != 0)
      {
         if ((++zz[2][y]) > zz[3][y])
         {
            zz[2][y] = 0;                             // reset tally
            zz[1][y]++;                               // next bitmap
            if (zz[1][y] > zz[4][y]) zz[1][y] = 0;    // is bitmap past end?
            zz[0][y] = zz[ 5 + zz[1][y] ] [y];        // put shape in 0
         }
      }
}

int mwBitmap::load_tiles(void)
{
   int load_error = 0;

   // get main tiles
   tilemap = al_load_bitmap("bitmaps/tiles.bmp");
   if (!tilemap)
   {
      mInput.m_err("Can't load tiles from bitmaps/tiles.bmp");
      load_error = 1;
   }
   else
   {
      al_convert_mask_to_alpha(tilemap, al_map_rgb(0, 0, 0)) ;
      al_set_target_bitmap(M_tilemap);
      al_draw_bitmap(tilemap, 0, 0, 0);
      for (int y=0; y<32; y++)
         for (int x=0; x<32; x++)
            tile[y*32 + x] = al_create_sub_bitmap(tilemap, x*20, y*20, 20, 20);
   }

   // get block tiles
   btilemap = al_load_bitmap("bitmaps/block_tiles.bmp");
   if (!btilemap)
   {
      mInput.m_err("Can't load tiles from bitmaps/block_tiles.bmp");
      load_error = 1;
   }
   else
   {
      al_convert_mask_to_alpha(btilemap, al_map_rgb(0, 0, 0)) ;
      al_set_target_bitmap(M_btilemap);
      al_draw_bitmap(btilemap, 0, 0, 0);
      for (int y=0; y<32; y++)
         for (int x=0; x<32; x++)
            btile[y*32 + x] = al_create_sub_bitmap(btilemap, x*20, y*20, 20, 20);
   }
   // get player tiles
   ptilemap = al_load_bitmap("bitmaps/player_tiles.bmp");
   if (!ptilemap)
   {
      mInput.m_err("Can't load tiles from bitmaps/player_tiles.bmp");
      load_error = 1;
   }
   else
   {
      al_convert_mask_to_alpha(ptilemap, al_map_rgb(0, 0, 0)) ;
      al_set_target_bitmap(M_ptilemap);
      al_draw_bitmap(ptilemap, 0, 0, 0);
      for (int a=0; a<16; a++)
         for (int b=0; b<24; b++)
            player_tile[a][b] = al_create_sub_bitmap(ptilemap, b*20, a*20, 20, 20);
   }

   // get door tiles
   dtilemap = al_load_bitmap("bitmaps/door_tiles.bmp");
   if (!dtilemap)
   {
      mInput.m_err("Can't load tiles from bitmaps/door_tiles.bmp");
      load_error = 1;
   }
   else
   {
      al_convert_mask_to_alpha(dtilemap, al_map_rgb(0, 0, 0)) ;
      al_set_target_bitmap(M_dtilemap);
      al_draw_bitmap(dtilemap, 0, 0, 0);
      for (int a=0; a<16; a++)
         for (int b=0; b<8; b++)
         {
            door_tile[0][a][b] = al_create_sub_bitmap(dtilemap, b*20,     a*20, 20, 20);
            door_tile[1][a][b] = al_create_sub_bitmap(dtilemap, b*20, 320+a*20, 20, 20);
         }
   }
   load_sprit(); // get animation sequences and shape attributes
   if (load_error) return 0;
   else return 1;
}


void mwBitmap::save_sprit(void)
{
   //printf("saving sprit001.pm\n");
   for (int c=0; c<NUM_ANS; c++) // set all animation initial
      if (zz[4][c])
      {
         zz[0][c]=zz[5][c];
         zz[1][c]=0;
         zz[2][c]=0;
      }

   // ensure sa[][0] does not have any bits set other than the ones we want
   for (int c=0; c<NUM_SPRITES; c++)
   {
      sa[c][0] &= PM_BTILE_ALL_FLAGS;
      sa[c][1] = 0; // not used
   }
   FILE *fp = fopen("bitmaps/sprit001.pm", "wb");
   fwrite(zz, sizeof(zz), 1, fp);
   fwrite(sa, sizeof(sa), 1, fp);
   fclose(fp);
}

void mwBitmap::load_sprit(void)
{
   FILE *fp = fopen("bitmaps/sprit001.pm", "rb");
   fread(zz, sizeof(zz), 1, fp);
   fread(sa, sizeof(sa), 1, fp);
   fclose(fp);
}

void mwBitmap::spin_shape(int tn, int x, int y, int tsx, int tsy, int tsw, int tsh, float scale, float dim, int cycle)
{
   int cti = cycle; // how many frame a full spin takes

   float ct = (int)cti; //cycle time
   float ct1 = ct/4;    // 20
   float ct2 = ct/2;    // 40
   float ct3 = ct1*3;   // 60
   float ct4 = ct;      // 80

   int tm = mLoop.frame_num % cti; // get a number from 0 to cti than increments every frame

   float tmr = (int) tm;

   float xs = 0;
   int flags = 0;


   // 80-60 = narrow to wide xs = 0 to 20 regular draw
   // 60-40 = wide to narrow xs = 20 to 0 regular draw

   // 40    = narrowest      xs = 0       flip draw from regular to reverse

   // 40-20 = narrow to wide xs = 0 to 20 reverse draw
   // 20-0  = wide to narrow xs = 20 to 0 reverse draw

   // 0     = narrowest      xs = 0       flip draw from reverse to regular


   if (tmr > ct2)
   {
      if (tmr>ct3) xs = ct4-tmr;  // 80-60 ---> 0-20
      else         xs = tmr-ct2;  // 60-40 ---> 20-0
   }
   else
   {
      if (tmr>ct1) xs = ct2-tmr;    // 40-20 ---> 0-20
      else xs = tmr;                // 20-0  ---> 20-0
      flags |= ALLEGRO_FLIP_HORIZONTAL;
   }

   // scale xs (if ct == 80 scale = 1)
   // or more accurately full width of tile (20) * 4
   float xscale = (tsw*4)/ct;
   xs *= xscale;

   float ys=tsh; // y scale is the same as the source height of the tile

   // optionally scale the entire thing
   xs *= scale;
   ys *= scale;

   // get draw offsets based on scale of final tile
   float xo = 10 - (xs/2); // x offset
   float yo = 10 - (ys/2); // x offset


   ALLEGRO_COLOR c2 = al_map_rgba_f(dim, dim, dim, 1.0); // show dimmer on back side

   if (flags == 0) al_draw_scaled_bitmap(       tile[tn],     tsx, tsy, tsw, tsh, x+xo, y+yo, xs, ys, flags);
   else            al_draw_tinted_scaled_bitmap(tile[tn], c2, tsx, tsy, tsw, tsh, x+xo, y+yo, xs, ys, flags);
}





// this version draws directly on the screen, but also needs to match withe level background
// x and y are the center
// uses scale factor current to match level background size

void mwBitmap::spin_shape2(int tn, float x, float y, float tile_scale, float dim, int ct)
{
   float ct0 = ct;
   float ct1 = ct0/4;    // 20
   float ct2 = ct1*2;    // 40
   float ct3 = ct1*3;    // 60
   float ct4 = ct1*4;    // 80
   int tmr = mLoop.frame_num % ct; // get a number from 0 to ct that increments every frame

   // 80-60 = narrow to wide xs = 0 to 20 regular draw
   // 60-40 = wide to narrow xs = 20 to 0 regular draw
   // 40    = narrowest      xs = 0       flip draw from regular to reverse
   // 40-20 = narrow to wide xs = 0 to 20 reverse draw
   // 20-0  = wide to narrow xs = 20 to 0 reverse draw
   // 0     = narrowest      xs = 0       flip draw from reverse to regular


   int flags = 0;
   float xs = 0;
   float fx = -0.5;
   if (tmr > ct2)
   {
      if (tmr>ct3) xs = ct4-tmr;  // 80-60 ---> 0-20
      else         xs = tmr-ct2;  // 60-40 ---> 20-0
   }
   else
   {
      if (tmr>ct1) xs = ct2-tmr;    // 40-20 ---> 0-20
      else xs = tmr;                // 20-0  ---> 20-0
      flags |= ALLEGRO_FLIP_HORIZONTAL;
      fx = 0.5;
   }

   xs /= (ct/4);          // convert from 0-20 to 0-1
   xs *= tile_scale;      // scale that with passed coin scale
   float ys = tile_scale; // y scale is always the full height

   // scale the entire thing
   xs *= mDisplay.scale_factor_current;
   ys *= mDisplay.scale_factor_current;


   ALLEGRO_COLOR c2 = al_map_rgba_f(dim, dim, dim, 1.0); // show dimmer on back side

   if (flags == 0) al_draw_scaled_rotated_bitmap(       tile[tn],     10+fx, 9.5, x, y, xs, ys, 0, flags);
   else            al_draw_tinted_scaled_rotated_bitmap(tile[tn], c2, 10+fx, 9.5, x, y, xs, ys, 0, flags);

}















