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
#include "mwLevel.h"
#include "mwMiscFnx.h"
#include "mwScreen.h"
#include "mwSelectionWindow.h"


mwBitmapTools mBitmapTools;



ALLEGRO_BITMAP* mwBitmapTools::load_block_tiles_to_bitmap()
{
   // load block_tiles to bitmap
   char b1_fn2[100];
   sprintf(b1_fn2, "bitmaps/block_tiles.bmp");
   // convert to 'ALLEGRO_FS_ENTRY' to get platform specific fully qualified path
   ALLEGRO_FS_ENTRY *FS_fname1 = al_create_fs_entry(b1_fn2);
   sprintf(b1ock_tiles_fn, "%s", al_get_fs_entry_name(FS_fname1));
   ALLEGRO_BITMAP *b1 = al_load_bitmap(b1ock_tiles_fn);
   if (!b1)
   {
      char msg[1024];
      sprintf(msg, "Error loading tiles from:%s", b1ock_tiles_fn);
      mInput.m_err(msg);
      return nullptr;
   }
   return b1;
}

void mwBitmapTools::save_bitmap_to_block_tiles_file(ALLEGRO_BITMAP* b1)
{
   al_save_bitmap(b1ock_tiles_fn, b1);
}






void mwBitmapTools::fill_player_tile(void)
{
   //printf("fill player bitmap\n");
   int a, b, x, y;


   if (0) // load from disk
   {
      mBitmap.ptilemap = al_load_bitmap("bitmaps/player_tiles.bmp");
      if (!mBitmap.ptilemap) mInput.m_err((char*)"Can't load tiles from bitmaps/player_tiles.bmp");
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
         al_set_target_bitmap(mBitmap.player_tile[a][0]); al_draw_bitmap(mBitmap.tile[400], 0, 0, 0);
         al_set_target_bitmap(mBitmap.player_tile[a][1]); al_draw_bitmap(mBitmap.tile[401], 0, 0, 0);
         al_set_target_bitmap(mBitmap.player_tile[a][2]); al_draw_bitmap(mBitmap.tile[402], 0, 0, 0);
         al_set_target_bitmap(mBitmap.player_tile[a][3]); al_draw_bitmap(mBitmap.tile[403], 0, 0, 0);
         al_set_target_bitmap(mBitmap.player_tile[a][4]); al_draw_bitmap(mBitmap.tile[404], 0, 0, 0);
         al_set_target_bitmap(mBitmap.player_tile[a][5]); al_draw_bitmap(mBitmap.tile[405], 0, 0, 0);

         al_set_target_bitmap(mBitmap.player_tile[a][6]); al_draw_bitmap(mBitmap.tile[368], 0, 0, 0);
         al_set_target_bitmap(mBitmap.player_tile[a][7]); al_draw_bitmap(mBitmap.tile[369], 0, 0, 0);
         al_set_target_bitmap(mBitmap.player_tile[a][8]); al_draw_bitmap(mBitmap.tile[370], 0, 0, 0);
         al_set_target_bitmap(mBitmap.player_tile[a][9]); al_draw_bitmap(mBitmap.tile[371], 0, 0, 0);
         al_set_target_bitmap(mBitmap.player_tile[a][10]); al_draw_bitmap(mBitmap.tile[372], 0, 0, 0);
         al_set_target_bitmap(mBitmap.player_tile[a][11]); al_draw_bitmap(mBitmap.tile[373], 0, 0, 0);

         al_set_target_bitmap(mBitmap.player_tile[a][12]); al_draw_bitmap(mBitmap.tile[432], 0, 0, 0);
         al_set_target_bitmap(mBitmap.player_tile[a][13]); al_draw_bitmap(mBitmap.tile[433], 0, 0, 0);
         al_set_target_bitmap(mBitmap.player_tile[a][14]); al_draw_bitmap(mBitmap.tile[434], 0, 0, 0);
         al_set_target_bitmap(mBitmap.player_tile[a][15]); al_draw_bitmap(mBitmap.tile[435], 0, 0, 0);
         al_set_target_bitmap(mBitmap.player_tile[a][16]); al_draw_bitmap(mBitmap.tile[436], 0, 0, 0);
         al_set_target_bitmap(mBitmap.player_tile[a][17]); al_draw_bitmap(mBitmap.tile[437], 0, 0, 0);

         al_set_target_bitmap(mBitmap.player_tile[a][18]); al_draw_bitmap(mBitmap.tile[755], 0, 0, 0);

         al_set_target_bitmap(mBitmap.player_tile[a][19]); al_draw_bitmap(mBitmap.tile[438], 0, 0, 0);

         al_set_target_bitmap(mBitmap.player_tile[a][20]); al_draw_bitmap(mBitmap.tile[606], 0, 0, 0);
         al_set_target_bitmap(mBitmap.player_tile[a][21]); al_draw_bitmap(mBitmap.tile[607], 0, 0, 0);

         al_set_target_bitmap(mBitmap.player_tile[a][22]); al_draw_bitmap(mBitmap.tile[638], 0, 0, 0);
         al_set_target_bitmap(mBitmap.player_tile[a][23]); al_draw_bitmap(mBitmap.tile[639], 0, 0, 0);

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

       al_save_bitmap("bitmaps/player_tiles.bmp", mBitmap.ptilemap);

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
   al_draw_bitmap(mBitmap.tile[200], 0, 0, 0);
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
   al_draw_bitmap(mBitmap.tile[200], 0, 0, 0);
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
   al_draw_bitmap(mBitmap.tile[200], 0, 0, 0);
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
      al_draw_bitmap(mBitmap.tile[204], 0, 1, 0);
      al_set_target_bitmap(switch_tiles[d]);
      al_draw_bitmap(mBitmap.tile[205], 0, 1, 0);

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
   al_draw_bitmap(mBitmap.tilemap, 0, 0, 0);
   al_flip_display(); mInput.tsw(); // wait for keypress


   // draw on the tilemap
   al_set_target_bitmap(mBitmap.tilemap);
   // erase
//   for (int d=0; d<32; d++) al_draw_bitmap(mBitmap.tile[0], d*20, 60, 0);

   al_draw_filled_rectangle(0, 60, 640, 80, mColor.pc[0]);

   // show tilemap after erase
   al_set_target_backbuffer(mDisplay.display);
   al_clear_to_color(al_map_rgb(0,0,0));
   al_draw_bitmap(mBitmap.tilemap, 0, 0, 0);
   al_flip_display(); mInput.tsw(); // wait for keypress


   al_set_target_bitmap(mBitmap.tilemap);
   for (int d=0; d<32; d++) al_draw_bitmap(switch_tiles[d], d*20, 60, 0);

   al_convert_mask_to_alpha(mBitmap.tilemap, al_map_rgb(0, 0, 0)) ;

   al_save_bitmap("bitmaps/tiles.bmp", mBitmap.tilemap);

   for (int d=0; d<32; d++) al_destroy_bitmap(switch_tiles[d]);


   al_set_target_backbuffer(mDisplay.display);
   al_clear_to_color(al_map_rgb(0,0,0));
   al_draw_bitmap(mBitmap.tilemap, 0, 0, 0);
   al_flip_display(); mInput.tsw(); // wait for keypress

}

void mwBitmapTools::combine_tile(void)
{

   // show tilemap before
   al_set_target_backbuffer(mDisplay.display);
   al_clear_to_color(al_map_rgb(0,0,0));
   al_draw_bitmap(mBitmap.tilemap, 0, 0, 0);
   al_flip_display(); mInput.tsw(); // wait for keypress


   al_set_target_bitmap(mBitmap.tile[534]);
   al_draw_bitmap(mBitmap.tile[440], 0, 0, 0);

   al_set_target_bitmap(mBitmap.tile[535]);
   al_draw_bitmap(mBitmap.tile[440], 0, 0, 0);

   // show tilemap after
   al_set_target_backbuffer(mDisplay.display);
   al_clear_to_color(al_map_rgb(0,0,0));
   al_draw_bitmap(mBitmap.tilemap, 0, 0, 0);
   al_flip_display(); mInput.tsw(); // wait for keypress

  //   al_save_bitmap("bitmaps/tiles.bmp", mBitmap.tilemap);

}








// used by sliders only for item block manip and blokwalk block
// gets block with flags from level

void mwBitmapTools::select_bitmap(int &tn)
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
      int swy3 = swy2 + 145; // below flags
      al_draw_filled_rectangle(             swx1-2,  swy1-2,  swx2+2, swy3, mColor.pc[0]);
      al_draw_rectangle(                    swx1-2,  swy1-2,  swx2+2, swy3, mColor.pc[13], 1);

      // view item area
      al_draw_rectangle(                    swx1,    swy1,    swx2, swy2, mColor.pc[9], 1);
      al_draw_text(mFont.pr8, mColor.pc[15], swx1+24, swy1+2, 0, "Choose Block");
      mWM.mW[1].em_show_item_info(                    swx1+2,  swy1+9, 9, local_point_item_type, local_point_item_num);

      // flags section
      int ftx = swx1+11;
      int fty = swy1+35;
      int ys = 10; // y spacing
      draw_flag_text(ftx+4, fty, ys, 15, 0);

      int frw = 6;            // flag rectangle width
      int frh = 6;            // flag rectangle height
      int frx = ftx-frw-2;        // flag rectangle x
      int fry = fty - (frh/2)+4;  // flag rectangle y
      draw_flag_rects(local_point_item_num, frx, fry, frw, frh, ys, 14);

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

// this is used by animation_sequence_editor() only to choose new bitmaps for sequences
//int bmp_index = 0;
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
            al_draw_bitmap(mBitmap.tile[x+(y*32)],x*20, y*20, 0);
      al_draw_rectangle(0.5, 0.5, 640.5, 640.5, mColor.pc[13], 1);

      int xc = 180;
      int yc = 648;

      al_draw_textf(mFont.pr8, mColor.pc[14], 80, 666, 0, "Get Shape %d ", mBitmap.zz[4][zzindx] );

      sprintf(msg, "Current Sequence %d", zzindx);
      int l = 2+strlen(msg)*4;
      al_draw_text(mFont.pr8, mColor.pc[13], xc+150, yc+2, ALLEGRO_ALIGN_CENTER, msg);
      al_draw_rectangle(xc+150-l, yc+1, xc+150+l, yc+11, mColor.pc[13], 1);

      for (int c=0; c < mBitmap.zz[4][zzindx] + 1; c++)   // show current seq shapes
         if (( mBitmap.zz[5+c][zzindx] < NUM_SPRITES) && (mBitmap.zz[5+c][zzindx] > 0))
            al_draw_bitmap(mBitmap.tile[ mBitmap.zz[5+c][zzindx] ], xc+1+c*20, yc+12, 0);
      al_draw_rectangle(xc+0.5, yc+11.5, xc+302.5, yc+32.5, mColor.pc[13], 1);


      if ((mInput.mouse_y < 640) && (mInput.mouse_x < 640))
      {
         int pointer = (mInput.mouse_x/20) + (mInput.mouse_y/20) * 32 ;
         al_draw_textf(mFont.pr8, mColor.pc[13], 522, 648, 0, "pointer %-2d", pointer );
         al_draw_bitmap(mBitmap.tile[pointer], 620, 642, 0);

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
   int xa = 330;
   int xb = 530;

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

      sprintf(msg, "Current Sequence %d",zzindx);
      l = 2+strlen(msg)*4;
      al_draw_text(mFont.pr8, mColor.pc[13], 150, 202, ALLEGRO_ALIGN_CENTER, msg);
      al_draw_rectangle(150-l, 201, 150+l, 211, mColor.pc[13], 1);

      for (int c = 0; c < mBitmap.zz[4][zzindx] + 1; c++)   // show current seq shapes
         if (( mBitmap.zz[5+c][zzindx] < NUM_SPRITES) && (mBitmap.zz[5+c][zzindx] > 0))
            al_draw_bitmap(mBitmap.tile[ mBitmap.zz[5+c][zzindx] ], 1+c*20, 212, 0);
      al_draw_rectangle(0.5, 211.5, 302.5, 232.5, mColor.pc[13], 1);

      for (int c=0; c < 32; c++)   // draw 32x8 grid of animation sequences
         for (int x=0; x < 8; x++)
            if (mBitmap.zz[4][c + (x * 32)] != 0)
               if ((mBitmap.zz[0][c + (x * 32)] < NUM_SPRITES) && (mBitmap.zz[0][c + (x * 32)] > 0 ))
                  al_draw_bitmap(mBitmap.tile[mBitmap.zz[0][c + (x * 32)]], 2+c*20, 30+x*20, 0);

      al_draw_rectangle(0.5, 29.5, 642.5, 190.5, mColor.pc[9], 1);

      // if mouse on grid, get current animation sequence pointer
      if ((mInput.mouse_y > 30) && (mInput.mouse_y < 190) && (mInput.mouse_x < 642))
      {
         pointer = (mInput.mouse_x-2) / 20 + (mInput.mouse_y-30) / 20 * 32;
         if ((pointer < 0) || (pointer > NUM_ANS-1)) pointer = -1;
      }
      else pointer = -1;

      if ((pointer != -1) && (mInput.mouse_b[1][0])) zzindx = pointer;


      int y5 = 200;
      if (mWidget.buttont(xa, y5, xb, 16, 0,0,0,0, 0,11,15,0, 1,0,1,0, "Get New Shapes"))
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
      mWidget.slideri(xa, y5, xb, 16, 0,0,0,0,  0,12,15,15,  0,0,1,0, mBitmap.zz[3][zzindx], 100, 0, 1, "Animation Delay:");
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



void mwBitmapTools::redraw_grid(int x, int y, int current_selection) // draw 32x32 bitmaps
{
   for (int dx=0; dx<32; dx++)
      for (int dy=0; dy<32; dy++)
      {
         int tn = dx + (dy*32); // tile number
         int zx = x + (dx*20);
         int zy = y + (dy*20);

         al_draw_bitmap(mBitmap.btile[tn], zx, zy, 0);
         if (tn == current_selection) al_draw_rectangle(zx, zy, zx+19, zy+19, mColor.pc[10], 2);
      }
}





void mwBitmapTools::draw_flag_text(int x, int y, int ys, int col, int last_flag_show)
{
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
   al_draw_text(mFont.pr8, mColor.pc[col], x, y, 0, "SECRET");           y+=ys;
   if (last_flag_show) al_draw_text(mFont.pr8, mColor.pc[col], x, y, 0, "SELECT_WIN_SHOW");
}


// this is the common one, called by all
// if mouse is on a button return button number
int mwBitmapTools::draw_flag_rects(int tn, int x, int y, int w, int h, int ys, int last_flag_show)
{
   int fa[15] = {0}; // make an array to store the flags

   if (tn & PM_BTILE_SOLID_PLAYER)     fa[0]  += 1; // tally set
   if (tn & PM_BTILE_SOLID_ENEMY)      fa[1]  += 1; // tally set
   if (tn & PM_BTILE_SOLID_ITEM)       fa[2]  += 1; // tally set
   if (tn & PM_BTILE_SOLID_PBUL)       fa[3]  += 1; // tally set
   if (tn & PM_BTILE_SOLID_EBUL)       fa[4]  += 1; // tally set
   if (tn & PM_BTILE_SEMISOLID_PLAYER) fa[5]  += 1; // tally set
   if (tn & PM_BTILE_SEMISOLID_ENEMY)  fa[6]  += 1; // tally set
   if (tn & PM_BTILE_SEMISOLID_ITEM)   fa[7]  += 1; // tally set
   if (tn & PM_BTILE_BOMBABLE)         fa[8]  += 1; // tally set
   if (tn & PM_BTILE_BREAKABLE_PSHOT)  fa[9]  += 1; // tally set
   if (tn & PM_BTILE_BREAKABLE_ESHOT)  fa[10] += 1; // tally set
   if (tn & PM_BTILE_LADDER_MOVE)      fa[11] += 1; // tally set
   if (tn & PM_BTILE_ROPE_MOVE)        fa[12] += 1; // tally set
   if (tn & PM_BTILE_SECRET)           fa[13] += 1; // tally set
   if (tn & PM_BTILE_SHOW_SELECT_WIN)  fa[14] += 1; // tally set

   for (int i=0; i<last_flag_show; i++)
   {
      if (fa[i] == 0) al_draw_rectangle       (x, y + (ys*i), x+w, y+h+(ys*i), mColor.pc[15], 1); // empty
      if (fa[i] == 1) al_draw_filled_rectangle(x, y + (ys*i), x+w, y+h+(ys*i), mColor.pc[15]);    // filled
   }

   if ((mInput.mouse_x > x) && (mInput.mouse_x < x+w) && (mInput.mouse_y > y) && (mInput.mouse_y < y+h+(ys*14))) return (mInput.mouse_y-y)/ys;
   return -1;
}


// this is for edit attributes only and only only affects sa[][]
void mwBitmapTools::draw_and_proc_flag_rects_for_sa(int tn, int x, int y, int w, int h, int ys)
{
   int highlight = draw_flag_rects(mBitmap.sa[tn][0], x, y, w, h, ys, 15);
   if (highlight > -1)
   {
      al_draw_rectangle(x-1, y+(ys*highlight), x+w+1, y+h+(ys*highlight), mColor.pc[15], 1);
      if (mInput.mouse_b[1][0])
      {
         while (mInput.mouse_b[1][0]) mEventQueue.proc(1); // wait for release
         if (highlight ==  0) mBitmap.sa[tn][0] ^= PM_BTILE_SOLID_PLAYER;
         if (highlight ==  1) mBitmap.sa[tn][0] ^= PM_BTILE_SOLID_ENEMY;
         if (highlight ==  2) mBitmap.sa[tn][0] ^= PM_BTILE_SOLID_ITEM;
         if (highlight ==  3) mBitmap.sa[tn][0] ^= PM_BTILE_SOLID_PBUL;
         if (highlight ==  4) mBitmap.sa[tn][0] ^= PM_BTILE_SOLID_EBUL;
         if (highlight ==  5) mBitmap.sa[tn][0] ^= PM_BTILE_SEMISOLID_PLAYER;
         if (highlight ==  6) mBitmap.sa[tn][0] ^= PM_BTILE_SEMISOLID_ENEMY;
         if (highlight ==  7) mBitmap.sa[tn][0] ^= PM_BTILE_SEMISOLID_ITEM;
         if (highlight ==  8) mBitmap.sa[tn][0] ^= PM_BTILE_BOMBABLE;
         if (highlight ==  9) mBitmap.sa[tn][0] ^= PM_BTILE_BREAKABLE_PSHOT;
         if (highlight == 10) mBitmap.sa[tn][0] ^= PM_BTILE_BREAKABLE_ESHOT;
         if (highlight == 11) mBitmap.sa[tn][0] ^= PM_BTILE_LADDER_MOVE;
         if (highlight == 12) mBitmap.sa[tn][0] ^= PM_BTILE_ROPE_MOVE;
         if (highlight == 13) mBitmap.sa[tn][0] ^= PM_BTILE_SECRET;
         if (highlight == 14) mBitmap.sa[tn][0] ^= PM_BTILE_SHOW_SELECT_WIN;
      }
   }
}



// this common one calls text and rect and allows changes if applicable



// used by draw item, point item and block selection description
void mwBitmapTools::draw_flags(int x1, int y1, int& num, int& mpow, int view_only, int clear_background, int ignore_mpow)
{
   int ys = 10; // y spacing
   int frw = 6; // flag rectangle width
   int frh = 6; // flag rectangle height
   if (clear_background)
   {
      al_draw_filled_rectangle(x1-1, y1-1, x1+142, y1+(13*ys)+8, mColor.pc[0]);
      al_draw_rectangle(x1-2, y1-2, x1+142, y1+(13*ys)+9, mColor.pc[15], 1);
   }
   if (!ignore_mpow) if ((mInput.mouse_x > x1-2) && (mInput.mouse_x < x1+155) && (mInput.mouse_y > y1-2) && (mInput.mouse_y < y1+(13*ys)+9)) mpow = 1;
   int highlight = draw_flag_rects(num, x1, y1, frw, frh, ys, 14);
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
            if (highlight == 13) (num) ^= PM_BTILE_SECRET;
            if (highlight == 14) (num) ^= PM_BTILE_SHOW_SELECT_WIN;
         }
      }
   }
   draw_flag_text(x1+12, y1, ys, 15, 0);
}


void mwBitmapTools::draw_flag_rects_multiple(int bx1, int by1, int bx2, int by2, int x, int y, int w, int h, int ys, int con, int cof, int highlight)
{
    // make an array of flags to store the tally
   int fa[15][2] = {0};

   // cycle the selection
   for (int cx = bx1; cx < bx2; cx++)
      for (int cy = by1; cy < by2; cy++)
      {
         int tn = cx + (cy*32);

         if (mBitmap.sa[tn][0] & PM_BTILE_SOLID_PLAYER)     fa[0][1] += 1; // tally set
         else                                               fa[0][0] += 1; // tally clear

         if (mBitmap.sa[tn][0] & PM_BTILE_SOLID_ENEMY)      fa[1][1] += 1; // tally set
         else                                               fa[1][0] += 1; // tally clear

         if (mBitmap.sa[tn][0] & PM_BTILE_SOLID_ITEM)       fa[2][1] += 1; // tally set
         else                                               fa[2][0] += 1; // tally clear

         if (mBitmap.sa[tn][0] & PM_BTILE_SOLID_PBUL)       fa[3][1] += 1; // tally set
         else                                               fa[3][0] += 1; // tally clear

         if (mBitmap.sa[tn][0] & PM_BTILE_SOLID_EBUL)       fa[4][1] += 1; // tally set
         else                                               fa[4][0] += 1; // tally clear

         if (mBitmap.sa[tn][0] & PM_BTILE_SEMISOLID_PLAYER) fa[5][1] += 1; // tally set
         else                                               fa[5][0] += 1; // tally clear

         if (mBitmap.sa[tn][0] & PM_BTILE_SEMISOLID_ENEMY)  fa[6][1] += 1; // tally set
         else                                               fa[6][0] += 1; // tally clear

         if (mBitmap.sa[tn][0] & PM_BTILE_SEMISOLID_ITEM)   fa[7][1] += 1; // tally set
         else                                               fa[7][0] += 1; // tally clear

         if (mBitmap.sa[tn][0] & PM_BTILE_BOMBABLE)         fa[8][1] += 1; // tally set
         else                                               fa[8][0] += 1; // tally clear

         if (mBitmap.sa[tn][0] & PM_BTILE_BREAKABLE_PSHOT)  fa[9][1] += 1; // tally set
         else                                               fa[9][0] += 1; // tally clear

         if (mBitmap.sa[tn][0] & PM_BTILE_BREAKABLE_ESHOT)  fa[10][1] += 1; // tally set
         else                                               fa[10][0] += 1; // tally clear

         if (mBitmap.sa[tn][0] & PM_BTILE_LADDER_MOVE)      fa[11][1] += 1; // tally set
         else                                               fa[11][0] += 1; // tally clear

         if (mBitmap.sa[tn][0] & PM_BTILE_ROPE_MOVE)        fa[12][1] += 1; // tally set
         else                                               fa[12][0] += 1; // tally clear

         if (mBitmap.sa[tn][0] & PM_BTILE_SECRET)           fa[13][1] += 1; // tally set
         else                                               fa[13][0] += 1; // tally clear

         if (mBitmap.sa[tn][0] & PM_BTILE_SHOW_SELECT_WIN)  fa[14][1] += 1; // tally set
         else                                               fa[14][0] += 1; // tally clear
      }

   for (int i=0; i<15; i++)
   {
                                    al_draw_rectangle       (x, y + (ys*i), x+w, y+h+(ys*i), mColor.pc[15], 1); // empty box by default
      if (fa[i][0] == 0)            al_draw_filled_rectangle(x, y + (ys*i), x+w, y+h+(ys*i), mColor.pc[15]); // no clears tallied, filled
      if ((fa[i][0]) && (fa[i][1])) al_draw_filled_triangle (x, y+h+(ys*i), x+w, y+h+(ys*i), x, y+(ys* i), mColor.pc[15]); // mixed
   }


   if (highlight > -1)
   {
      al_draw_rectangle(x-1, y+(ys*highlight)-1, x+w+1, y+h+(ys*highlight)+1, mColor.pc[15], 1);

      if (mInput.mouse_b[1][0])
      {
         while (mInput.mouse_b[1][0]) mEventQueue.proc(1);

         int action = 0;

         if (fa[highlight][0] == 0) action = 0; // all set, so action is to clear
         if (fa[highlight][1] == 0) action = 1; // all clear, so action is to set

         int set_flag = 0;
         if (highlight ==  0) set_flag = PM_BTILE_SOLID_PLAYER;
         if (highlight ==  1) set_flag = PM_BTILE_SOLID_ENEMY;
         if (highlight ==  2) set_flag = PM_BTILE_SOLID_ITEM;
         if (highlight ==  3) set_flag = PM_BTILE_SOLID_PBUL;
         if (highlight ==  4) set_flag = PM_BTILE_SOLID_EBUL;
         if (highlight ==  5) set_flag = PM_BTILE_SEMISOLID_PLAYER;
         if (highlight ==  6) set_flag = PM_BTILE_SEMISOLID_ENEMY;
         if (highlight ==  7) set_flag = PM_BTILE_SEMISOLID_ITEM;
         if (highlight ==  8) set_flag = PM_BTILE_BOMBABLE;
         if (highlight ==  9) set_flag = PM_BTILE_BREAKABLE_PSHOT;
         if (highlight == 10) set_flag = PM_BTILE_BREAKABLE_ESHOT;
         if (highlight == 11) set_flag = PM_BTILE_LADDER_MOVE;
         if (highlight == 12) set_flag = PM_BTILE_ROPE_MOVE;
         if (highlight == 13) set_flag = PM_BTILE_SECRET;
         if (highlight == 14) set_flag = PM_BTILE_SHOW_SELECT_WIN;


         for (int cx = bx1; cx < bx2; cx++) // cycle the selection
            for (int cy = by1; cy < by2; cy++)
            {
               int tn = cx + (cy*32);
               if (action == 0) mBitmap.sa[tn][0] &= ~set_flag;
               if (action == 1) mBitmap.sa[tn][0] |= set_flag;
            }
      }
   }
}


void mwBitmapTools::edit_btile_attributes()
{
   char msg[1024];
   int x, y;
   int mode = 0;
   int quit = 0;
   int gridlines = 0;

   int current_selection = 0;
   int pointer = 0;

   int bx1 = 0;
   int by1 = 0;
   int bx2 = 1;
   int by2 = 1;

   al_set_target_backbuffer(mDisplay.display);
   al_show_mouse_cursor(mDisplay.display);

   while (!quit)
   {
      mEventQueue.proc(1);

      al_flip_display();
      al_clear_to_color(al_map_rgb(0,0,0));

      if (mode == 0)
      {
          redraw_grid(0, 0, current_selection);
      }
      if (mode == 1)
      {
         redraw_grid(0, 0, -1);
         al_draw_rectangle((bx1)*20, (by1)*20, (bx2)*20, (by2)*20, mColor.pc[10], 1);
      }

      if (gridlines)
      {
         for (x=0; x<=32; x++) al_draw_line(x*20, 0, x*20, 640, mColor.pc[15+128], 0);
         for (y=0; y<=32; y++) al_draw_line(0, y*20, 640, y*20, mColor.pc[15+128], 0);
      }

      // flags section
      int ftx = 690;
      int fty = 340;
      int ys = 20; // y spacing

      draw_flag_text(ftx+4, fty, ys, 15, 1);

      int frw = 12;         // flag rectangle width
      int frh = 12;         // flag rectangle height
      int frx = ftx-frw-2;        // flag rectangle x
      int fry = fty - (frh/2)+4;  // flag rectangle y


      // position of 'currently selected' window (relative to flags)
      int csx = ftx-frw-1;
      int csy = fty-ys-1;

      int tx = 40;
      int ty1 = 670;

      if (mode == 0) // show flags for one tile
      {
         al_draw_rounded_rectangle(            tx, ty1, tx+508, ty1+26, 2, 2, mColor.pc[13], 1);
         al_draw_text(mFont.pr8, mColor.pc[15], tx+2, ty1+2,  0, "left  mouse button (b1) - select a tile");
         al_draw_text(mFont.pr8, mColor.pc[15], tx+2, ty1+16, 0, "right mouse button (b2) - paste selected tile flags to new tile");

         draw_and_proc_flag_rects_for_sa(current_selection, frx, fry, frw, frh, ys);
         al_draw_rectangle(                       csx-1,  csy-7, csx+195, csy+15, mColor.pc[10], 1);
         al_draw_bitmap(mBitmap.btile[current_selection], csx,    csy-6, 0);
         al_draw_textf(mFont.pr8, mColor.pc[4],    csx+24, csy,   0, "Current Selection %-2d  ", current_selection );
      }

      if (mode == 1) // show the tallied flags multiple
      {
         al_draw_rounded_rectangle(            tx, ty1, tx+302, ty1+26, 2, 2, mColor.pc[13], 1);
         al_draw_text(mFont.pr8, mColor.pc[15], tx+2, ty1+2,  0, "- draw selection rectangle");
         al_draw_text(mFont.pr8, mColor.pc[15], tx+2, ty1+16, 0, "- toggle flags for all selected tiles");

         draw_flag_rects_multiple(bx1, by1, bx2, by2, frx, fry, frw, frh, ys, 10, 11, -1);
         al_draw_rectangle(csx-4, csy-7, csx+195, csy+15, mColor.pc[10], 1);
         al_draw_rectangle(csx-4, csy+15, csx+frw+2, csy+15+(15*ys), mColor.pc[10], 1); // frame for buttons
         int num_tiles = (bx2-bx1) * (by2-by1);
         al_draw_textf(mFont.pr8, mColor.pc[4],    csx+frw,     csy, 0, " %d Tile(s) Selected", num_tiles);
      }

      // mouse on flag rectangles -- this is to show highlight what flag is selected
      if ((mInput.mouse_x > frx) && (mInput.mouse_x < frx+frw) && (mInput.mouse_y > fry) && (mInput.mouse_y < fry+(15*ys) ))
      {
         int indx = (mInput.mouse_y-fry)/ys;
         if (mode == 0) draw_and_proc_flag_rects_for_sa(current_selection, frx, fry, frw, frh, ys);
         if (mode == 1) draw_flag_rects_multiple(bx1, by1, bx2, by2, frx, fry, frw, frh, ys, 10, 11, indx);
      }

      if ((mInput.mouse_y < 640) && (mInput.mouse_x < 640)) // mouse is on main 32x32 tile grid
      {
         int mx = mInput.mouse_x/20;
         int my = mInput.mouse_y/20;
         pointer = mx + (my*32); // tile the mouse is currently pointing at

         // show pointer on grid as lt blue rectangle
         al_draw_rectangle(mx*20+1, my*20+1, mx*20+19, my*20+19, mColor.pc[13], 1);

         csx -= 24;
         csy -= 25;
         al_draw_rectangle(csx-4, csy-7, csx+195,   csy+15, mColor.pc[13], 1);
         al_draw_rectangle(csx-4, csy+15, csx+frw+2, csy+40+(15*ys), mColor.pc[13], 1); // frame for buttons

         al_draw_bitmap(mBitmap.btile[pointer],         csx-3,  csy-6, 0);
         al_draw_textf(mFont.pr8, mColor.pc[15], csx+24, csy,   0, "Mouse Pointer %d", pointer);


         // show the flags of the pointer
         draw_and_proc_flag_rects_for_sa(pointer, frx-24, fry, frw, frh, ys);

         if (mInput.mouse_b[2][0])
         {
            while (mInput.mouse_b[2][0]) mEventQueue.proc(1);
            if (mode == 0) mBitmap.sa[pointer][0] = mBitmap.sa[current_selection][0];
         }

         if (mInput.mouse_b[1][0])
         {
            if (mode == 0) current_selection = (mInput.mouse_x/20) + (mInput.mouse_y/20) * 32;

            if (mode == 1)
            {
               bx2 = bx1 = mInput.mouse_x/20;
               by2 = by1 = mInput.mouse_y/20;
               int old_mouse_x = mInput.mouse_x;
               int old_mouse_y = mInput.mouse_y;

               while (mInput.mouse_b[1][0]) // trap while b1 is held
               {
                  mEventQueue.proc(1);
                  al_flip_display();
                  al_clear_to_color(al_map_rgb(0,0,0));
                  redraw_grid(0, 0, -1);
                  al_draw_rectangle((bx1)*20, (by1)*20, (bx2)*20, (by2)*20, mColor.pc[15], 1);

                  bx2 = bx1 + ((mInput.mouse_x - old_mouse_x+20)/20);
                  by2 = by1 + ((mInput.mouse_y - old_mouse_y+20)/20);

                  if (bx1>32) bx1 = 32; // limits
                  if (bx2>32) bx2 = 32;
                  if (by1>32) by1 = 32;
                  if (by2>32) by2 = 32;

               }
               // ensure top-right, bottom left format
               if (bx1 > bx2) { int temp = bx2; bx2 = bx1; bx1 = temp; }
               if (by1 > by2) { int temp = by2; by2 = by1; by1 = temp; }

               if (bx2 == bx1) bx2++; // don't let size be zero
               if (by2 == by1) by2++;

               if (bx1>32) bx1 = 32; // limits
               if (bx2>32) bx2 = 32;
               if (by1>32) by1 = 32;
               if (by2>32) by2 = 32;


            }
         } // end of mInput.mouse_b[1][0] pressed
      } // end of mouse on 32x32 tile grid


      // draw and process save button
      int sb_x = 375;
      int sb_w = 70;
      int sb_c = sb_x+(sb_w/2);
      int sb_y = 644;
      int sb_y2 = sb_y + 22;
      int sb_col = 10;

      al_draw_rounded_rectangle(sb_x, sb_y, sb_x+sb_w, sb_y2, 2, 2, mColor.pc[sb_col], 1);
      al_draw_textf(mFont.pr8, mColor.pc[sb_col], sb_c, sb_y+2,  ALLEGRO_ALIGN_CENTER, "Save");
      al_draw_textf(mFont.pr8, mColor.pc[sb_col], sb_c, sb_y+12, ALLEGRO_ALIGN_CENTER, "Changes");

      if ((mInput.mouse_x > sb_x) && (mInput.mouse_x < (sb_x+sb_w)) && (mInput.mouse_y > sb_y) && (mInput.mouse_y < sb_y2))
      {
         al_draw_rounded_rectangle(sb_x, sb_y, sb_x+sb_w, sb_y2, 2, 2, mColor.pc[sb_col], 2);
         if (mInput.mouse_b[1][0])
         {
            while (mInput.mouse_b[1][0]) mEventQueue.proc(1);
            mBitmap.save_sprit();
          }
      }

      // draw and process the mode button
      int mb_x = 40;
      int mb_x2 = mb_x + 180;
      int mb_xc = mb_x + (mb_x2-mb_x)/2;
      int mb_y = 644;
      int mb_y2 = mb_y + 22;
      int mb_col = 13;

      al_draw_rounded_rectangle(mb_x, mb_y, mb_x2, mb_y2, 2, 2, mColor.pc[mb_col], 1);

      al_draw_textf(mFont.pr8, mColor.pc[mb_col], mb_xc, mb_y+3,  ALLEGRO_ALIGN_CENTER, "Current Mode");
      if (mode == 0) sprintf(msg,"Change or Copy Single");
      if (mode == 1) sprintf(msg,"Change Multiple");
      al_draw_text(mFont.pr8, mColor.pc[mb_col], mb_xc, mb_y+12, ALLEGRO_ALIGN_CENTER, msg);

      if ((mInput.mouse_x > mb_x) && (mInput.mouse_x < (mb_x2)) && (mInput.mouse_y > mb_y) && (mInput.mouse_y < mb_y2))
      {
         al_draw_rounded_rectangle(mb_x, mb_y, mb_x2, mb_y2, 2, 2, mColor.pc[mb_col], 2);
         if (mInput.mouse_b[1][0])
         {
            while (mInput.mouse_b[1][0]) mEventQueue.proc(1); // wait for release
            if (++mode>1) mode = 0;
         }
      }

      // draw and process gridlines button
      int gb_x = 518;
      int gb_w = 120;
      int gb_c = gb_x + (gb_w/2);
      int gb_y = 644;
      int gb_y2 = gb_y + 12;
      int gb_col = 15;

      al_draw_rounded_rectangle(gb_x, gb_y, gb_x+gb_w, gb_y2, 2, 2, mColor.pc[gb_col], 1);
      if (gridlines) al_draw_textf(mFont.pr8, mColor.pc[gb_col], gb_c, gb_y+2, ALLEGRO_ALIGN_CENTER, "Gridlines:ON ");
      else           al_draw_textf(mFont.pr8, mColor.pc[gb_col], gb_c, gb_y+2, ALLEGRO_ALIGN_CENTER, "Gridlines:OFF");

      if ((mInput.mouse_x > gb_x) && (mInput.mouse_x < (gb_x+gb_w)) && (mInput.mouse_y > gb_y) && (mInput.mouse_y < gb_y+20))
      {
         al_draw_rounded_rectangle(gb_x, gb_y, gb_x+gb_w, gb_y2, 2, 2, mColor.pc[gb_col], 2);
         if (mInput.mouse_b[1][0])
         {
            while (mInput.mouse_b[1][0]) mEventQueue.proc(1);
            gridlines = !gridlines;
          }
      }
      if (mInput.key[ALLEGRO_KEY_ESCAPE][0])
      {
         while (mInput.key[ALLEGRO_KEY_ESCAPE][0]) mEventQueue.proc(1);
         quit = 1;
      }

   }
   mSelectionWindow.init();


}




void mwBitmapTools::draw_gridlines_and_frame(int x1, int y1, int x2, int y2, int fd, int fc, int fw, int gd, int gc, int gw)
{
   if (gd) // gridline draw
   {
      for (int x=x1; x<=x2; x+=22) al_draw_line(x, y1, x, y2, mColor.pc[gc], gw);
      for (int y=y1; y<=y2; y+=22) al_draw_line(x1, y, x2, y, mColor.pc[gc], gw);
   }
   if (fd) al_draw_rectangle(x1, y1, x2, y2, mColor.pc[fc], fw);
}





// this one has arbitrary first and second bmp file
void mwBitmapTools::copy_tiles()
{
   char msg[1024];
   int quit = 0;
   int gridlines = 1;
   al_set_target_backbuffer(mDisplay.display);
   al_show_mouse_cursor(mDisplay.display);
   ALLEGRO_BITMAP *qtmp = al_create_bitmap(20, 20);
   ALLEGRO_BITMAP *qtmp2 = al_create_bitmap(20, 20);

   // main bitmap ------------------
   char b1_fn[100];
   char b1_fn2[100];
   sprintf(b1_fn2, "bitmaps/block_tiles.bmp");

   // convert to 'ALLEGRO_FS_ENTRY' to get platform specific fully qualified path
   ALLEGRO_FS_ENTRY *FS_fname1 = al_create_fs_entry(b1_fn2);
   sprintf(b1_fn, "%s", al_get_fs_entry_name(FS_fname1));

   ALLEGRO_BITMAP *b1 = al_load_bitmap(b1_fn);
   if (!b1)
   {
      sprintf(msg, "Error loading tiles from:%s", b1_fn);
      mInput.m_err(msg);
   }

   int b1_x = 10;
   int b1_y = 20;
   int b1_w = al_get_bitmap_width(b1);
   int b1_h = al_get_bitmap_height(b1);
   int b1_x2 = b1_x + b1_w;
   int b1_y2 = b1_y + b1_h;
   int b1_tw = b1_w/22; // tile grid width
   int reload_b1 = 0;

   // second bitmap ------------------
   char b2_fn[100];
   char b2_fn2[100];
   sprintf(b2_fn2, "bitmaps/tiles.bmp");

   // convert to 'ALLEGRO_FS_ENTRY' to get platform specific fully qualified path
   ALLEGRO_FS_ENTRY *FS_fname2 = al_create_fs_entry(b2_fn2);
   sprintf(b2_fn, "%s", al_get_fs_entry_name(FS_fname2));

   ALLEGRO_BITMAP *b2 = al_load_bitmap(b2_fn);
   if (!b2)
   {
      sprintf(msg, "Error loading tiles from:%s", b2_fn);
      mInput.m_err(msg);
   }
   int b2_x = 740;
   int b2_y = 20;
   int b2_w = al_get_bitmap_width(b2);
   int b2_h = al_get_bitmap_height(b2);
   int b2_tw = b2_w/22; // tile grid width
   int b2_x2 = b2_x + b2_w;
   int b2_y2 = b2_y + b2_h;
   int reload_b2 = 0;

   while (!quit)
   {
      if (reload_b1)
      {
         al_destroy_bitmap(b1);

         if (reload_b1 == 2) b1 = al_load_bitmap(b1_fn);
         else
         {
            sprintf(b1_fn, "bitmaps/");
            if (mMiscFnx.mw_file_select("Load Bitmap File", b1_fn, ".bmp", 0)) b1 = al_load_bitmap(b1_fn);
         }

         if (!b2) mInput.m_err("Load Error");
         else
         {
            b1_w = al_get_bitmap_width(b1);
            b1_h = al_get_bitmap_height(b1);
            b1_x2 = b1_x + b1_w;
            b1_y2 = b1_y + b1_h;
            b1_tw = b1_w/22; // tile grid width
         }
         reload_b1 = 0;
      }

      if (reload_b2 == 1)
      {
         al_destroy_bitmap(b2);
         sprintf(b2_fn, "bitmaps\\");
         if (mMiscFnx.mw_file_select("Load Bitmap File", b2_fn, ".bmp", 0)) b2 = al_load_bitmap(b2_fn);
         if (!b2) mInput.m_err("Load Error");
         else
         {
            b2_w = al_get_bitmap_width(b2);
            b2_h = al_get_bitmap_height(b2);
            b2_x2 = b2_x + b2_w;
            b2_y2 = b2_y + b2_h;
            b2_tw = b2_w/22; // tile grid width
         }
         reload_b2 = 0;
      }

//      if (reload_b2 == 2) // load and scale to 1/2
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

      // show the draw item
      int dix = b1_x + 1;
      int diy = b1_y + b1_h + 28;
      al_draw_rounded_rectangle(dix, diy, dix+170, diy+22, 2, 2, mColor.pc[13], 1);
      al_draw_bitmap(qtmp, dix+2, diy+1, 0);
      al_draw_textf(mFont.pr8, mColor.pc[13], dix+26, diy+3,  0, "Current Draw Item");

      // gridlines check box
      int ya = b1_y+b1_h + 2;
      mWidget.togglec(b1_x+280, ya, b1_x+320, 16,  0,0,0,0,  0, 0, 0, 0,  1,0,1,0, gridlines, "Gridlines", 15, 15);


      // second bitmap -----------------------------------------------------------------------------------------------
      // draw the second bitmap to copy from only
      al_draw_bitmap(b2, b2_x, b2_y, 0);

      // title with filename only, no path
      ALLEGRO_PATH *ap = al_create_path(b2_fn);
      al_draw_textf(mFont.pr8, mColor.pc[15], b2_x+(b2_w/2),  b2_y-10, ALLEGRO_ALIGN_CENTER, "%s", al_get_path_filename(ap));
      al_destroy_path(ap);

      ya = b2_y-12;
      if (mWidget.buttontcb(b2_x,    ya, 0, 14, 0,0,0,0, 0,15,15,14, 1,0,0,0, "Load") )     reload_b2 = 1;
//      if (mWidget.buttontcb(b2_x+42, ya, 0, 14, 0,0,0,0, 0,15,15,14, 1,0,0,0, "Load 1/2") ) reload_b2 = 2;

      // draw a dim rectangle around the entire grid
      draw_gridlines_and_frame(b2_x, b2_y, b2_x2, b2_y2, 1, 15+64, 1, gridlines, 15+128, 0 );

      // is mouse on grid
      if ((mInput.mouse_x > b2_x) && (mInput.mouse_x < (b2_x + b2_w)) && (mInput.mouse_y > b2_y) && (mInput.mouse_y < (b2_y+b2_h)))
      {
         // draw a rectangle around the entire grid
         draw_gridlines_and_frame(b2_x, b2_y, b2_x2, b2_y2, 1, 15, 1, gridlines, 15+64, 0 );

         // what tile is mouse pointing at?
         int mx = (mInput.mouse_x-b2_x)/22;
         int my = (mInput.mouse_y-b2_y)/22;
         int pointer = mx + my*b2_tw;

         // mouse pointer offset relative to bitmap
         int bp_x1 = mx*22+1;
         int bp_y1 = my*22+1;

         // outline tile that mouse is pointing at
         int mx1 = b2_x + bp_x1 - 1;
         int my1 = b2_y + bp_y1 - 1;
         al_draw_rectangle(mx1, my1, mx1+22, my1+22, mColor.pc[10], 0);

         // copy tile to qtmp2
         al_set_target_bitmap(qtmp2);
         al_clear_to_color(al_map_rgb(0, 0, 0));
         al_draw_bitmap_region(b2, bp_x1, bp_y1, 20, 20, 0, 0, 0);

         // draw b2_pointer button
         int b2p_x1= b2_x+1;
         int b2p_w1= 160;
         int b2p_y1= b2_y+b2_h;

         al_set_target_backbuffer(mDisplay.display);
         al_draw_rounded_rectangle(b2p_x1, b2p_y1, b2p_x1+b2p_w1, b2p_y1+22, 2, 2, mColor.pc[15], 1);
         al_draw_bitmap(qtmp2, b2p_x1+2, b2p_y1+1, 0);
         al_draw_textf(mFont.pr8, mColor.pc[15], b2p_x1+26, b2p_y1+3,  0, "pointer:%d", pointer);
         al_draw_textf(mFont.pr8, mColor.pc[15], b2p_x1+26, b2p_y1+12, 0, "mouse b2 to copy");

         if (mInput.mouse_b[2][0])
         {
            while (mInput.mouse_b[2][0]) mEventQueue.proc(1);
            al_set_target_bitmap(qtmp);
            al_clear_to_color(al_map_rgb(0, 0, 0));
            al_draw_bitmap_region(b2, bp_x1, bp_y1, 20, 20, 0, 0, 0);
            al_set_target_backbuffer(mDisplay.display);
         }
      }

      // main bitmap -----------------------------------------------------------------------------------------------
      // draw the main bitmap
      al_draw_bitmap(b1, b1_x, b1_y, 0);

      // title with filename only, no path
      ALLEGRO_PATH *ap1 = al_create_path(b1_fn);
      al_draw_textf(mFont.pr8, mColor.pc[15], b1_x+(b1_w/2),  b1_y-10, ALLEGRO_ALIGN_CENTER, "%s", al_get_path_filename(ap1));
      al_destroy_path(ap1);

      if (mWidget.buttontcb(b1_x,     ya, 0, 14, 0,0,0,0, 0,15,15,14, 1,0,0,0, "Load")) reload_b1 = 1;
      if (mWidget.buttontcb(b1_x2-40, ya, 0, 14, 0,0,0,0, 0,15,15,10, 1,0,0,0, "Save")) al_save_bitmap(b1_fn, b1);

      draw_gridlines_and_frame(b1_x, b1_y, b1_x2, b1_y2, 1, 15+64, 1, gridlines, 15+128, 0 );

      // is mouse on grid
      if ((mInput.mouse_x > b1_x) && (mInput.mouse_x < (b1_x + b1_w)) && (mInput.mouse_y > b1_y) && (mInput.mouse_y < (b1_y+b1_h)))
      {
         draw_gridlines_and_frame(b1_x, b1_y, b1_x2, b1_y2, 1, 15, 1, gridlines, 15+64, 0 );

         // what tile is mouse pointing at?
         int mx = (mInput.mouse_x-b1_x)/22;
         int my = (mInput.mouse_y-b1_y)/22;
         int pointer = mx + my*b1_tw;

         // mouse pointer offset relative to bitmap
         int bp_x1 = mx*22+1;
         int bp_y1 = my*22+1;

         // outline tile that mouse is pointing at
         int mx1 = b1_x + bp_x1 - 1;
         int my1 = b1_y + bp_y1 - 1;
         al_draw_rectangle(mx1, my1, mx1+22, my1+22, mColor.pc[10], 0);

         // copy tile to qtmp2
         al_set_target_bitmap(qtmp2);
         al_clear_to_color(al_map_rgb(0, 0, 0));
         al_draw_bitmap_region(b1, bp_x1, bp_y1, 20, 20, 0, 0, 0);

         // draw b1_pointer button
         int b1p_x1= b1_x+1;
         int b1p_w1= 160;
         int b1p_y1= b1_y+b1_h;

         al_set_target_backbuffer(mDisplay.display);
         al_draw_rounded_rectangle(b1p_x1, b1p_y1, b1p_x1+b1p_w1, b1p_y1+22, 2, 2, mColor.pc[15], 1);
         al_draw_bitmap(qtmp2, b1p_x1+2, b1p_y1+1, 0);
         al_draw_textf(mFont.pr8, mColor.pc[15], b1p_x1+26, b1p_y1+3,  0, "pointer:%d", pointer);
         al_draw_textf(mFont.pr8, mColor.pc[15], b1p_x1+26, b1p_y1+12, 0, "mouse b2 to copy");


         if (mInput.mouse_b[1][0])
         {
            while (mInput.mouse_b[1][0]) mEventQueue.proc(1);
            al_set_target_bitmap(b1);                                                 // point at bitmap 1
            al_draw_filled_rectangle(bp_x1, bp_y1, bp_x1+20, bp_y1+20, mColor.pc[0]); // erase

            al_draw_filled_rectangle(bp_x1-1, bp_y1-1, bp_x1+21, bp_y1+21, mColor.pc[0]); // erase



            al_draw_bitmap(qtmp, bp_x1, bp_y1, 0);                                     // draw new tile
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

         al_draw_textf(mFont.pr8, mColor.pc[13], dix+26, diy+12, 0, "mouse b1 to paste"); // only show this line if mouse on main grid
      }
      if (mInput.key[ALLEGRO_KEY_ESCAPE][0])
      {
         while (mInput.key[ALLEGRO_KEY_ESCAPE][0]) mEventQueue.proc(1);
         quit = 1;
      }


      if (mInput.key[ALLEGRO_KEY_S][0])
      {
         while (mInput.key[ALLEGRO_KEY_S][0]) mEventQueue.proc(1);
         modify_tile_set();
         //custom_modify();
//         create_tileset_frame(352);
//         create_tileset_solid(384);
//         create_tileset_solid_just_frame(320);
//         mBitmap.load_tiles();
//         reload_b1 = 2;
      }
   }
   al_destroy_bitmap(b1);
   al_destroy_bitmap(b2);
   al_destroy_bitmap(qtmp);
   al_destroy_bitmap(qtmp2);
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



void copy_framegroup(ALLEGRO_BITMAP *b, int s, int d)
{
   copy_tile(b, s+16, d+0);
   copy_tile(b, s+18, d+1);
   copy_tile(b, s+14, d+2);
   copy_tile(b, s+6,  d+3);
   copy_tile(b, s+12, d+4);
   copy_tile(b, s+15, d+5);
   copy_tile(b, s+4,  d+6);
   copy_tile(b, s+13, d+7);
   copy_tile(b, s+0,  d+8);
   copy_tile(b, s+1,  d+9);
   copy_tile(b, s+2,  d+10);
   copy_tile(b, s+3,  d+11);
   copy_tile(b, s+8,  d+12);
   copy_tile(b, s+10, d+13);
   copy_tile(b, s+9,  d+14);
   copy_tile(b, s+11, d+15);
}


void copy_rectgroup(ALLEGRO_BITMAP *b, int s, int d)
{
   copy_tile(b, s+16, d+0);
   copy_tile(b, s+18, d+1);
   copy_tile(b, s+14, d+2);
   copy_tile(b, s+9,  d+3);
   copy_tile(b, s+12, d+4);
   copy_tile(b, s+15, d+5);
   copy_tile(b, s+8,  d+6);
   copy_tile(b, s+13, d+7);
   copy_tile(b, s+0,  d+8);
   copy_tile(b, s+1,  d+9);
   copy_tile(b, s+2,  d+10);
   copy_tile(b, s+3,  d+11);
   copy_tile(b, s+4,  d+12);
   copy_tile(b, s+5,  d+13);
   copy_tile(b, s+6,  d+14);
   copy_tile(b, s+7,  d+15);
}

void mwBitmapTools::custom_modify()
{
   printf("Custom Modify\n");
   ALLEGRO_BITMAP *b1 = load_block_tiles_to_bitmap();
   if (!b1) return;
   //   copy_rectgroup(b1, 384, 896);
   //   copy_rectgroup(b1, 416, 864);
   //   copy_framegroup(b1, 608, 832);
   //   copy_framegroup(b1, 576, 800);
   // save modified block tiles bitmap to file
   //save_bitmap_to_block_tiles_file(b1);
}
























































































// ----------------------------------------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------------------------------


void erase_pixels_outside_rounded_corners(ALLEGRO_BITMAP *b, ALLEGRO_COLOR c)
{
   al_set_target_bitmap(b);

   int w = al_get_bitmap_width(b);
   int h = al_get_bitmap_height(b);

   float D = 0.005;
   for (int y=0; y<h; y++)
   {
      int x = 0;
      int done = 0;
      while (!done)
      {
         ALLEGRO_COLOR p = al_get_pixel(b, x, y);
         if ( (abs(p.r - c.r) < D) && (abs(p.g - c.g) < D) && (abs(p.b - c.b) < D) ) done = 1;
         else al_put_pixel(x, y, al_map_rgb(0,0,0));
         if (++x > w-1) done = 1;
      }
      x = w-1;
      done = 0;
      while (!done)
      {
         ALLEGRO_COLOR p = al_get_pixel(b, x, y);
         if ( (abs(p.r - c.r) < D) && (abs(p.g - c.g) < D) && (abs(p.b - c.b) < D) ) done = 1;
         else al_put_pixel(x, y, al_map_rgb(0,0,0));
         if (--x < 0) done = 1;
      }
   }
}


void make_hline_vline_and_single(ALLEGRO_BITMAP *b, ALLEGRO_BITMAP *b60, int indexS, int indexH, int indexV)
{
   int s;

   // make 3x1 hline
   ALLEGRO_BITMAP *tmp2 = al_create_bitmap(60, 20);
   al_set_target_bitmap(tmp2);
   al_draw_bitmap_region(b60, 0,   0, 60, 10, 0,  0, 0); // upper half
   al_draw_bitmap_region(b60, 0,  50, 60, 10, 0, 10, 0); // lower half
   al_set_target_bitmap(b);
   s = indexH + 0; al_draw_bitmap_region(tmp2,  0, 0, 20, 20, (s % 32)*22+1, (s / 32)*22+1, 0); // hline l
   s = indexH + 1; al_draw_bitmap_region(tmp2, 20, 0, 20, 20, (s % 32)*22+1, (s / 32)*22+1, 0); // hline m
   s = indexH + 2; al_draw_bitmap_region(tmp2, 40, 0, 20, 20, (s % 32)*22+1, (s / 32)*22+1, 0); // hline r

   // make 1x3 vline
   ALLEGRO_BITMAP *tmp3 = al_create_bitmap(20, 60);
   al_set_target_bitmap(tmp3);
   al_draw_bitmap_region(b60, 0,  0, 10, 60, 0,  0, 0); // left half
   al_draw_bitmap_region(b60, 50, 0, 10, 60, 10, 0, 0); // right half
   al_set_target_bitmap(b);
   s = indexV + 0; al_draw_bitmap_region(tmp3, 0,  0, 20, 20, (s % 32)*22+1, (s / 32)*22+1, 0); // vline u
   s = indexV + 1; al_draw_bitmap_region(tmp3, 0, 20, 20, 20, (s % 32)*22+1, (s / 32)*22+1, 0); // vline m
   s = indexV + 2; al_draw_bitmap_region(tmp3, 0, 40, 20, 20, (s % 32)*22+1, (s / 32)*22+1, 0); // vline l

   // single standalone block with frame on all sides
   al_set_target_bitmap(tmp3);
   al_draw_bitmap_region(tmp3, 0, 50, 20, 10, 0, 10, 0); // copy bottom to line up with top


   al_set_target_bitmap(b);
   s = indexS;  al_draw_bitmap_region(tmp3, 0, 0, 20, 20, (s % 32)*22+1, (s / 32)*22+1, 0);

   al_destroy_bitmap(tmp2);
   al_destroy_bitmap(tmp3);

}


































void mwBitmapTools::create_tileset_frame(int bs, float h1, float h2, float s1, float s2, float l1, float l2, int steps, float round)
{
   if (steps > 20) steps = 20;

   // background colors
   ALLEGRO_COLOR bc = al_color_hsl(h2, s2, l2);

   // colors for each step
   ALLEGRO_COLOR c[20];
   float linc = (l2 - l1) / steps;
   float sinc = (s2 - s1) / steps;
   float hinc = (h2 - h1) / steps;
   for (int i=0; i<steps; i++)
      c[i] = al_color_hsl(h1+i*hinc, s1+i*sinc, l1+i*linc);



   // make 3x3 grid, frame and extract tiles
   ALLEGRO_BITMAP *tmp1;
   tmp1 = al_create_bitmap(60, 60);
   al_set_target_bitmap(tmp1);
   al_clear_to_color(bc);

   float x1 = 0.5;
   float y1 = 0.5;
   float x2 = 59.5;
   float y2 = 59.5;

   float x1a = 19.5;
   float y1a = 19.5;
   float x2a = 40.5;
   float y2a = 40.5;

   for (float i=0; i<steps-.6; i+=.01)
   {
      al_draw_rounded_rectangle(x1  + i, y1  + i, x2  - i, y2  - i, round, round, c[(int)i], 1);
      al_draw_rounded_rectangle(x1a - i, y1a - i, x2a + i, y2a + i, 0, 0, c[(int)i], 1);
//      al_draw_rounded_rectangle(x1a - i, y1a - i, x2a + i, y2a + i, round, round, c[(int)i], 1);

   }
   erase_pixels_outside_rounded_corners(tmp1, c[0]);

   ALLEGRO_BITMAP *b1 = load_block_tiles_to_bitmap();
   al_set_target_bitmap(b1);
   int s=bs;
   s = bs +  8; al_draw_bitmap_region(tmp1, 0,   0, 20, 20, (s % 32)*22+1, (s / 32)*22+1, 0); // ul corner
   s = bs +  9; al_draw_bitmap_region(tmp1, 40,  0, 20, 20, (s % 32)*22+1, (s / 32)*22+1, 0); // ur corner
   s = bs + 10; al_draw_bitmap_region(tmp1, 0,  40, 20, 20, (s % 32)*22+1, (s / 32)*22+1, 0); // ll corner
   s = bs + 11; al_draw_bitmap_region(tmp1, 40, 40, 20, 20, (s % 32)*22+1, (s / 32)*22+1, 0); // lr corner

   make_hline_vline_and_single(b1, tmp1, bs+1, bs+2, bs+5);


   // get some pieces to construct more complex blocks
   ALLEGRO_BITMAP *cn = al_create_bitmap(10, 10);
   al_set_target_bitmap(cn);
   al_clear_to_color(al_map_rgb(0,0,0));
   al_draw_bitmap_region(tmp1, 10, 10, 10, 10, 0, 0, 0); // inside corner

   ALLEGRO_BITMAP *hf = al_create_bitmap(10, 20);
   al_set_target_bitmap(hf);
   al_clear_to_color(al_map_rgb(0,0,0));
   al_draw_bitmap_region(tmp1, 0, 20, 10, 20, 0, 0, 0); // half

   // single block
   al_destroy_bitmap(tmp1);
   tmp1 = al_create_bitmap(20, 20);

   // cross
   al_set_target_bitmap(tmp1);
   al_clear_to_color(bc);
   al_draw_bitmap_region(cn, 0, 0, 10, 10, 10, 10, 0);
   al_draw_bitmap_region(cn, 0, 0, 10, 10, 0,  10, ALLEGRO_FLIP_HORIZONTAL);
   al_draw_bitmap_region(cn, 0, 0, 10, 10, 10,  0, ALLEGRO_FLIP_VERTICAL);
   al_draw_bitmap_region(cn, 0, 0, 10, 10, 0,   0, ALLEGRO_FLIP_HORIZONTAL | ALLEGRO_FLIP_VERTICAL);
   al_set_target_bitmap(b1);
   s = bs; al_draw_bitmap(tmp1, (s % 32)*22+1, (s / 32)*22+1, 0);

   // tees
   al_set_target_bitmap(tmp1);
   al_clear_to_color(bc);
   al_draw_bitmap_region(hf, 0, 0, 10, 20, 0,  0, 0); // left half
   al_draw_bitmap_region(cn, 0, 0, 10, 10, 10, 10, 0); // inside corner for lr
   al_draw_bitmap_region(cn, 0, 0, 10, 10, 10,  0, ALLEGRO_FLIP_VERTICAL); // inside corner for ur
   al_set_target_bitmap(b1);
   s = bs+12; al_draw_bitmap(        tmp1,         (s % 32)*22+1,  (s / 32)*22+1,  0);
   s = bs+13; al_draw_bitmap(        tmp1,         (s % 32)*22+1,  (s / 32)*22+1,  ALLEGRO_FLIP_HORIZONTAL);
   s = bs+14; al_draw_rotated_bitmap(tmp1, 10, 10, (s % 32)*22+11, (s / 32)*22+11, ALLEGRO_PI / 2, 0);
   s = bs+15; al_draw_rotated_bitmap(tmp1, 10, 10, (s % 32)*22+11, (s / 32)*22+11, ALLEGRO_PI / 2, ALLEGRO_FLIP_HORIZONTAL);

   save_bitmap_to_block_tiles_file(b1);

   al_destroy_bitmap(b1);
   al_destroy_bitmap(tmp1);
   al_destroy_bitmap(cn);
   al_destroy_bitmap(hf);
}



// ----------------------------------------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------------------------------


void mwBitmapTools::create_tileset_solid(int bs, int tile, float h1, float h2, float s1, float s2, float l1, float l2, int steps, float round)
{
   if (steps > 20) steps = 20;

   // background colors
   ALLEGRO_COLOR bc = al_color_hsl(h2, s2, l2);

   // colors for each step
   ALLEGRO_COLOR c[20];
   float linc = (l2 - l1) / steps;
   float sinc = (s2 - s1) / steps;
   float hinc = (h2 - h1) / steps;
   for (int i=0; i<steps; i++)
      c[i] = al_color_hsl(h1+i*hinc, s1+i*sinc, l1+i*linc);

   // make 3x3 grid, frame and extract tiles
   ALLEGRO_BITMAP *tmp1;
   tmp1 = al_create_bitmap(60, 60);
   al_set_target_bitmap(tmp1);
   if (!tile) al_clear_to_color(bc);
   else
   {
      al_clear_to_color(al_map_rgb(0,0,0));
      for (int i=0; i<3; i++)
         for (int j=0; j<3; j++)
            al_draw_bitmap(mBitmap.btile[tile], j*20, i*20, 0);
   }

   float x1 = 0.5;
   float y1 = 0.5;
   float x2 = 59.5;
   float y2 = 59.5;

   for (float i=0; i<steps-.6; i+=.01)
      al_draw_rounded_rectangle(x1+i, y1+i, x2-i, y2-i, round, round, c[(int)i], 1);

   erase_pixels_outside_rounded_corners(tmp1, c[0]);

   ALLEGRO_BITMAP *b1 = load_block_tiles_to_bitmap();
   al_set_target_bitmap(b1);
   int s=bs;  al_draw_bitmap_region(tmp1, 20, 20, 20, 20, (s % 32)*22+1, (s / 32)*22+1, 0); // middle
   s = bs+8;  al_draw_bitmap_region(tmp1, 0,   0, 20, 20, (s % 32)*22+1, (s / 32)*22+1, 0); // ul corner
   s = bs+9;  al_draw_bitmap_region(tmp1, 40,  0, 20, 20, (s % 32)*22+1, (s / 32)*22+1, 0); // ur corner
   s = bs+10; al_draw_bitmap_region(tmp1, 0,  40, 20, 20, (s % 32)*22+1, (s / 32)*22+1, 0); // ll corner
   s = bs+11; al_draw_bitmap_region(tmp1, 40, 40, 20, 20, (s % 32)*22+1, (s / 32)*22+1, 0); // lr corner
   s = bs+12; al_draw_bitmap_region(tmp1, 0,  20, 20, 20, (s % 32)*22+1, (s / 32)*22+1, 0); // l edge
   s = bs+13; al_draw_bitmap_region(tmp1, 40, 20, 20, 20, (s % 32)*22+1, (s / 32)*22+1, 0); // r edge
   s = bs+14; al_draw_bitmap_region(tmp1, 20,  0, 20, 20, (s % 32)*22+1, (s / 32)*22+1, 0); // u edge
   s = bs+15; al_draw_bitmap_region(tmp1, 20, 40, 20, 20, (s % 32)*22+1, (s / 32)*22+1, 0); // d edge

   make_hline_vline_and_single(b1, tmp1, bs+1, bs+2, bs+5);

   // save modified block tiles bitmap to file
   save_bitmap_to_block_tiles_file(b1);

   al_destroy_bitmap(b1);
   al_destroy_bitmap(tmp1);
}


















/*




















void mwBitmapTools::create_tileset_extended(int bs, float h1, float h2, float s1, float s2, float l1, float l2, int steps, float round)
{
   if (steps > 20) steps = 20;

   // background colors
   ALLEGRO_COLOR bc = al_color_hsl(h2, s2, l2);

   // colors for each step
   ALLEGRO_COLOR c[20];
   float linc = (l2 - l1) / steps;
   float sinc = (s2 - s1) / steps;
   float hinc = (h2 - h1) / steps;
   for (int i=0; i<steps; i++)
      c[i] = al_color_hsl(h1+i*hinc, s1+i*sinc, l1+i*linc);


   // make 3x3 grid, frame and extract tiles
   ALLEGRO_BITMAP *tmp1 = al_create_bitmap(60, 60);
   al_set_target_bitmap(tmp1);
   al_clear_to_color(bc);

   float x1 = 0.5;
   float y1 = 0.5;
   float x2 = 59.5;
   float y2 = 59.5;

   float x1a = 19.5;
   float y1a = 19.5;
   float x2a = 40.5;
   float y2a = 40.5;

   for (float i=0; i<steps-.6; i+=.01)
   {
      al_draw_rounded_rectangle(x1  + i, y1  + i, x2  - i, y2  - i, round, round, c[(int)i], 1);
      al_draw_rounded_rectangle(x1a - i, y1a - i, x2a + i, y2a + i, 0, 0, c[(int)i], 1);
      //      al_draw_rounded_rectangle(x1a - i, y1a - i, x2a + i, y2a + i, round, round, c[(int)i], 1);

   }
   erase_pixels_outside_rounded_corners(tmp1, c[0]);

   ALLEGRO_BITMAP *b1 = load_block_tiles_to_bitmap();
   al_set_target_bitmap(b1);
   int s=bs;
   s = bs +  5; al_draw_bitmap_region(tmp1, 0,   0, 20, 20, (s % 32)*22+1, (s / 32)*22+1, 0); // ul corner
   s = bs +  6; al_draw_bitmap_region(tmp1, 40,  0, 20, 20, (s % 32)*22+1, (s / 32)*22+1, 0); // ur corner
   s = bs + 37; al_draw_bitmap_region(tmp1, 0,  40, 20, 20, (s % 32)*22+1, (s / 32)*22+1, 0); // ll corner
   s = bs + 38; al_draw_bitmap_region(tmp1, 40, 40, 20, 20, (s % 32)*22+1, (s / 32)*22+1, 0); // lr corner

   make_hline_vline_and_single(b1, tmp1, bs+1, bs+2, bs+34);

   // get some pieces to construct more complex blocks
   ALLEGRO_BITMAP *cf = al_create_bitmap(10, 10);
   al_set_target_bitmap(cf);
   al_clear_to_color(al_map_rgb(0,0,0));
   al_draw_bitmap_region(tmp1, 10, 10, 10, 10, 0, 0, 0); // inside corner

   ALLEGRO_BITMAP *hf = al_create_bitmap(10, 20);
   al_set_target_bitmap(hf);
   al_clear_to_color(al_map_rgb(0,0,0));
   al_draw_bitmap_region(tmp1, 0, 20, 10, 20, 0, 0, 0); // half

   // single block
   ALLEGRO_BITMAP *sb = al_create_bitmap(20, 20);

   // cross
   al_set_target_bitmap(sb);
   al_clear_to_color(bc);
   al_draw_bitmap_region(cf, 0, 0, 10, 10, 10, 10, 0);
   al_draw_bitmap_region(cf, 0, 0, 10, 10, 0,  10, ALLEGRO_FLIP_HORIZONTAL);
   al_draw_bitmap_region(cf, 0, 0, 10, 10, 10,  0, ALLEGRO_FLIP_VERTICAL);
   al_draw_bitmap_region(cf, 0, 0, 10, 10, 0,   0, ALLEGRO_FLIP_HORIZONTAL | ALLEGRO_FLIP_VERTICAL);
   al_set_target_bitmap(b1);
   s = bs; al_draw_bitmap(sb, (s % 32)*22+1, (s / 32)*22+1, 0);

   // tees
   al_set_target_bitmap(sb);
   al_clear_to_color(bc);
   al_draw_bitmap_region(hf, 0, 0, 10, 20, 0,  0, 0); // left half
   al_draw_bitmap_region(cf, 0, 0, 10, 10, 10, 10, 0); // inside corner for lr
   al_draw_bitmap_region(cf, 0, 0, 10, 10, 10,  0, ALLEGRO_FLIP_VERTICAL); // inside corner for ur
   al_set_target_bitmap(b1);
   s = bs+7;  al_draw_bitmap(        sb,         (s % 32)*22+1,  (s / 32)*22+1,  0);
   s = bs+40; al_draw_bitmap(        sb,         (s % 32)*22+1,  (s / 32)*22+1,  ALLEGRO_FLIP_HORIZONTAL);
   s = bs+8;  al_draw_rotated_bitmap(sb, 10, 10, (s % 32)*22+11, (s / 32)*22+11, ALLEGRO_PI / 2, 0);
   s = bs+39; al_draw_rotated_bitmap(sb, 10, 10, (s % 32)*22+11, (s / 32)*22+11, ALLEGRO_PI / 2, ALLEGRO_FLIP_HORIZONTAL);


   // solid
   // make 3x3 grid, frame and extract tiles
   al_set_target_bitmap(tmp1);
   al_clear_to_color(bc);
   for (float i=0; i<steps-.6; i+=.01)
      al_draw_rounded_rectangle(x1+i, y1+i, x2-i, y2-i, round, round, c[(int)i], 1);
   erase_pixels_outside_rounded_corners(tmp1, c[0]);

   al_set_target_bitmap(b1);

   s = bs+32; al_draw_bitmap_region(tmp1, 20, 20, 20, 20, (s % 32)*22+1, (s / 32)*22+1, 0); // center

   s = bs+9;  al_draw_bitmap_region(tmp1, 0,   0, 20, 20, (s % 32)*22+1, (s / 32)*22+1, 0); // ul corner
   s = bs+10; al_draw_bitmap_region(tmp1, 40,  0, 20, 20, (s % 32)*22+1, (s / 32)*22+1, 0); // ur corner
   s = bs+41; al_draw_bitmap_region(tmp1, 0,  40, 20, 20, (s % 32)*22+1, (s / 32)*22+1, 0); // ll corner
   s = bs+42; al_draw_bitmap_region(tmp1, 40, 40, 20, 20, (s % 32)*22+1, (s / 32)*22+1, 0); // lr corner

   s = bs+11; al_draw_bitmap_region(tmp1, 0,  20, 20, 20, (s % 32)*22+1, (s / 32)*22+1, 0); // l edge
   s = bs+44; al_draw_bitmap_region(tmp1, 40, 20, 20, 20, (s % 32)*22+1, (s / 32)*22+1, 0); // r edge
   s = bs+12; al_draw_bitmap_region(tmp1, 20,  0, 20, 20, (s % 32)*22+1, (s / 32)*22+1, 0); // u edge
   s = bs+43; al_draw_bitmap_region(tmp1, 20, 40, 20, 20, (s % 32)*22+1, (s / 32)*22+1, 0); // d edge


   // get some pieces to construct more complex blocks
   ALLEGRO_BITMAP *cs = al_create_bitmap(10, 10);
   al_set_target_bitmap(cs);
   al_clear_to_color(al_map_rgb(0,0,0));
   al_draw_bitmap_region(tmp1, 10, 10, 10, 10, 0, 0, 0); // inside corner


   // solid corner tees

   // take existing tee
   al_set_target_bitmap(sb);
   // add solid corner
   al_draw_bitmap(cs, 10, 10, 0);

   al_set_target_bitmap(b1);
   s = bs+13; al_draw_rotated_bitmap(sb, 10, 10, (s % 32)*22+11, (s / 32)*22+11, ALLEGRO_PI * 0.0, 0);
   s = bs+14; al_draw_rotated_bitmap(sb, 10, 10, (s % 32)*22+11, (s / 32)*22+11, ALLEGRO_PI * 0.5, 0);
   s = bs+46; al_draw_rotated_bitmap(sb, 10, 10, (s % 32)*22+11, (s / 32)*22+11, ALLEGRO_PI * 1.0, 0);
   s = bs+45; al_draw_rotated_bitmap(sb, 10, 10, (s % 32)*22+11, (s / 32)*22+11, ALLEGRO_PI * 1.5, 0);

   // flip and rotate
   al_set_target_bitmap(sb);
   al_draw_rotated_bitmap(sb, 10, 10, 10, 10, ALLEGRO_PI * 1.5, ALLEGRO_FLIP_HORIZONTAL);

   al_set_target_bitmap(b1);
   s = bs+15; al_draw_rotated_bitmap(sb, 10, 10, (s % 32)*22+11, (s / 32)*22+11, ALLEGRO_PI * 0.0, 0);
   s = bs+16; al_draw_rotated_bitmap(sb, 10, 10, (s % 32)*22+11, (s / 32)*22+11, ALLEGRO_PI * 0.5, 0);
   s = bs+48; al_draw_rotated_bitmap(sb, 10, 10, (s % 32)*22+11, (s / 32)*22+11, ALLEGRO_PI * 1.0, 0);
   s = bs+47; al_draw_rotated_bitmap(sb, 10, 10, (s % 32)*22+11, (s / 32)*22+11, ALLEGRO_PI * 1.5, 0);

   // solid corner 2 way tees
   // make cross with one solid corner
   al_set_target_bitmap(sb);
   al_clear_to_color(bc);
   al_draw_bitmap(cf,  0,  0, ALLEGRO_FLIP_HORIZONTAL | ALLEGRO_FLIP_VERTICAL);  // faded corner tl
   al_draw_bitmap(cf, 10,  0, ALLEGRO_FLIP_VERTICAL);                            // faded corner tr
   al_draw_bitmap(cf,  0, 10, ALLEGRO_FLIP_HORIZONTAL);                          // faded corner bl
   al_draw_bitmap(cs, 10, 10, 0);                                                // solid corner br

   al_set_target_bitmap(b1);
   s = bs+17; al_draw_rotated_bitmap(sb, 10, 10, (s % 32)*22+11, (s / 32)*22+11, ALLEGRO_PI * 0.0, 0);
   s = bs+18; al_draw_rotated_bitmap(sb, 10, 10, (s % 32)*22+11, (s / 32)*22+11, ALLEGRO_PI * 0.5, 0);
   s = bs+50; al_draw_rotated_bitmap(sb, 10, 10, (s % 32)*22+11, (s / 32)*22+11, ALLEGRO_PI * 1.0, 0);
   s = bs+49; al_draw_rotated_bitmap(sb, 10, 10, (s % 32)*22+11, (s / 32)*22+11, ALLEGRO_PI * 1.5, 0);


   // inner corner
   al_set_target_bitmap(sb);
   al_clear_to_color(bc);
   al_draw_bitmap(cf, 10, 10, 0);                                                // faded corner br

   al_set_target_bitmap(b1);
   s = bs+19; al_draw_rotated_bitmap(sb, 10, 10, (s % 32)*22+11, (s / 32)*22+11, ALLEGRO_PI * 0.0, 0);
   s = bs+20; al_draw_rotated_bitmap(sb, 10, 10, (s % 32)*22+11, (s / 32)*22+11, ALLEGRO_PI * 0.5, 0);
   s = bs+52; al_draw_rotated_bitmap(sb, 10, 10, (s % 32)*22+11, (s / 32)*22+11, ALLEGRO_PI * 1.0, 0);
   s = bs+51; al_draw_rotated_bitmap(sb, 10, 10, (s % 32)*22+11, (s / 32)*22+11, ALLEGRO_PI * 1.5, 0);


   // edge tees (add one more faded corner to existing)
   al_set_target_bitmap(sb);
   al_draw_bitmap(cf, 10,  0, ALLEGRO_FLIP_VERTICAL);                            // faded corner tr


   al_set_target_bitmap(b1);
   s = bs+21; al_draw_rotated_bitmap(sb, 10, 10, (s % 32)*22+11, (s / 32)*22+11, ALLEGRO_PI * 0.0, 0);
   s = bs+22; al_draw_rotated_bitmap(sb, 10, 10, (s % 32)*22+11, (s / 32)*22+11, ALLEGRO_PI * 0.5, 0);
   s = bs+54; al_draw_rotated_bitmap(sb, 10, 10, (s % 32)*22+11, (s / 32)*22+11, ALLEGRO_PI * 1.0, 0);
   s = bs+53; al_draw_rotated_bitmap(sb, 10, 10, (s % 32)*22+11, (s / 32)*22+11, ALLEGRO_PI * 1.5, 0);



   // diagonal corners
   al_set_target_bitmap(sb);
   al_clear_to_color(bc);
   al_draw_bitmap(cs,  0,  0, ALLEGRO_FLIP_HORIZONTAL | ALLEGRO_FLIP_VERTICAL);  // faded corner tl
   al_draw_bitmap(cf, 10,  0, ALLEGRO_FLIP_VERTICAL);                            // faded corner tr
   al_draw_bitmap(cf,  0, 10, ALLEGRO_FLIP_HORIZONTAL);                          // faded corner bl
   al_draw_bitmap(cs, 10, 10, 0);                                                // solid corner br

   al_set_target_bitmap(b1);
   s = bs+23; al_draw_rotated_bitmap(sb, 10, 10, (s % 32)*22+11, (s / 32)*22+11, ALLEGRO_PI * 0.0, 0);
   s = bs+24; al_draw_rotated_bitmap(sb, 10, 10, (s % 32)*22+11, (s / 32)*22+11, ALLEGRO_PI * 0.5, 0);










   // save modified block tiles bitmap to file
   save_bitmap_to_block_tiles_file(b1);

   al_destroy_bitmap(b1);
   al_destroy_bitmap(tmp1);
   al_destroy_bitmap(sb);

   al_destroy_bitmap(hf);
   al_destroy_bitmap(cf);
   al_destroy_bitmap(cs);

}




*/










void mwBitmapTools::create_tileset_extended(int bs, float h1, float h2, float s1, float s2, float l1, float l2, int steps, float round)
{
   if (steps > 20) steps = 20;

   // background colors
   ALLEGRO_COLOR bc = al_color_hsl(h2, s2, l2);

   // colors for each step
   ALLEGRO_COLOR c[20];
   float linc = (l2 - l1) / steps;
   float sinc = (s2 - s1) / steps;
   float hinc = (h2 - h1) / steps;
   for (int i=0; i<steps; i++)
      c[i] = al_color_hsl(h1+i*hinc, s1+i*sinc, l1+i*linc);



   // frame
   // draw 3x3 grid and extract tiles
   ALLEGRO_BITMAP *tmp1 = al_create_bitmap(60, 60);
   al_set_target_bitmap(tmp1);
   al_clear_to_color(bc);

   float x1 = 0.5;
   float y1 = 0.5;
   float x2 = 59.5;
   float y2 = 59.5;

   float x1a = 19.5;
   float y1a = 19.5;
   float x2a = 40.5;
   float y2a = 40.5;

   for (float i=0; i<steps-.6; i+=.01)
   {
      al_draw_rounded_rectangle(x1  + i, y1  + i, x2  - i, y2  - i, round, round, c[(int)i], 1);
      //al_draw_rounded_rectangle(x1a - i, y1a - i, x2a + i, y2a + i, 0,         0, c[(int)i], 1);
      al_draw_rounded_rectangle(x1a - i, y1a - i, x2a + i, y2a + i, round, round, c[(int)i], 1);

   }
   erase_pixels_outside_rounded_corners(tmp1, c[0]);

   // load block tiles from file to bitmap, so we can modify it
   ALLEGRO_BITMAP *b1 = load_block_tiles_to_bitmap();
   al_set_target_bitmap(b1);
   int s=bs;

   // get single, hline and vline
   make_hline_vline_and_single(b1, tmp1, bs, bs+1, bs+4);

   // get corners
   s = bs +  7;  al_draw_bitmap_region(tmp1, 0,   0, 20, 20, (s % 32)*22+1, (s / 32)*22+1, 0); // tl corner
   s = bs +  8;  al_draw_bitmap_region(tmp1, 40,  0, 20, 20, (s % 32)*22+1, (s / 32)*22+1, 0); // tr corner
   s = bs +  9;  al_draw_bitmap_region(tmp1, 40, 40, 20, 20, (s % 32)*22+1, (s / 32)*22+1, 0); // br corner
   s = bs +  10; al_draw_bitmap_region(tmp1, 0,  40, 20, 20, (s % 32)*22+1, (s / 32)*22+1, 0); // bl corner

   // get some pieces to construct more complex blocks
   ALLEGRO_BITMAP *cf = al_create_bitmap(10, 10);
   al_set_target_bitmap(cf);
   al_clear_to_color(al_map_rgb(0,0,0));
   al_draw_bitmap_region(tmp1, 10, 10, 10, 10, 0, 0, 0); // faded br corner

   ALLEGRO_BITMAP *hf = al_create_bitmap(10, 20);
   al_set_target_bitmap(hf);
   al_clear_to_color(al_map_rgb(0,0,0));
   al_draw_bitmap_region(tmp1, 0, 20, 10, 20, 0, 0, 0); // faded half


   // single block
   ALLEGRO_BITMAP *sb = al_create_bitmap(20, 20);

   // tees
   al_set_target_bitmap(sb);
   al_clear_to_color(bc);
   al_draw_bitmap(hf, 0,  0,  0);                             // faded left half
   al_draw_bitmap(cf, 10,  0, ALLEGRO_FLIP_VERTICAL);         // faded tr corner
   al_draw_bitmap(cf, 10, 10, 0);                             // faded br corner
   al_set_target_bitmap(b1);
   s = bs+11; al_draw_rotated_bitmap(sb, 10, 10, (s % 32)*22+11, (s / 32)*22+11, ALLEGRO_PI * 0.0, 0);
   s = bs+12; al_draw_rotated_bitmap(sb, 10, 10, (s % 32)*22+11, (s / 32)*22+11, ALLEGRO_PI * 0.5, 0);
   s = bs+13; al_draw_rotated_bitmap(sb, 10, 10, (s % 32)*22+11, (s / 32)*22+11, ALLEGRO_PI * 1.0, 0);
   s = bs+14; al_draw_rotated_bitmap(sb, 10, 10, (s % 32)*22+11, (s / 32)*22+11, ALLEGRO_PI * 1.5, 0);


   // cross
   al_set_target_bitmap(sb);
   al_clear_to_color(bc);
   al_draw_bitmap(cf, 10,  0, ALLEGRO_FLIP_VERTICAL);                           // faded tr corner
   al_draw_bitmap(cf, 10, 10, 0);                                               // faded br corner
   al_draw_bitmap(cf, 0,  10, ALLEGRO_FLIP_HORIZONTAL);                         // faded bl corner
   al_draw_bitmap(cf, 0,   0, ALLEGRO_FLIP_HORIZONTAL | ALLEGRO_FLIP_VERTICAL); // faded tl corner
   al_set_target_bitmap(b1);
   s = bs + 15; al_draw_bitmap(sb, (s % 32)*22+1, (s / 32)*22+1, 0);



   // the first 16 tiles should be done.......they are all part of the 'frame' tileset



   // inner corners
   al_set_target_bitmap(sb);
   al_clear_to_color(bc);
   al_draw_bitmap(cf, 10, 10, 0);          // faded br corner
   al_set_target_bitmap(b1);
   s = bs+16; al_draw_rotated_bitmap(sb, 10, 10, (s % 32)*22+11, (s / 32)*22+11, ALLEGRO_PI * 0.0, 0);
   s = bs+17; al_draw_rotated_bitmap(sb, 10, 10, (s % 32)*22+11, (s / 32)*22+11, ALLEGRO_PI * 0.5, 0);
   s = bs+18; al_draw_rotated_bitmap(sb, 10, 10, (s % 32)*22+11, (s / 32)*22+11, ALLEGRO_PI * 1.0, 0);
   s = bs+19; al_draw_rotated_bitmap(sb, 10, 10, (s % 32)*22+11, (s / 32)*22+11, ALLEGRO_PI * 1.5, 0);





   // solid
   // draw 3x3 grid and extract tiles
   al_set_target_bitmap(tmp1);
   al_clear_to_color(bc);
   for (float i=0; i<steps-.6; i+=.01)
      al_draw_rounded_rectangle(x1+i, y1+i, x2-i, y2-i, round, round, c[(int)i], 1);
   erase_pixels_outside_rounded_corners(tmp1, c[0]);
   al_set_target_bitmap(b1);
   s = bs+20; al_draw_bitmap_region(tmp1, 0,   0, 20, 20, (s % 32)*22+1, (s / 32)*22+1, 0); // tl corner
   s = bs+21; al_draw_bitmap_region(tmp1, 40,  0, 20, 20, (s % 32)*22+1, (s / 32)*22+1, 0); // tr corner
   s = bs+22; al_draw_bitmap_region(tmp1, 40, 40, 20, 20, (s % 32)*22+1, (s / 32)*22+1, 0); // br corner
   s = bs+23; al_draw_bitmap_region(tmp1, 0,  40, 20, 20, (s % 32)*22+1, (s / 32)*22+1, 0); // bl corner
   s = bs+46; al_draw_bitmap_region(tmp1, 20, 20, 20, 20, (s % 32)*22+1, (s / 32)*22+1, 0); // center
   s = bs+24; al_draw_bitmap_region(tmp1, 0,  20, 20, 20, (s % 32)*22+1, (s / 32)*22+1, 0); // l edge
   s = bs+25; al_draw_bitmap_region(tmp1, 20,  0, 20, 20, (s % 32)*22+1, (s / 32)*22+1, 0); // u edge
   s = bs+26; al_draw_bitmap_region(tmp1, 40, 20, 20, 20, (s % 32)*22+1, (s / 32)*22+1, 0); // r edge
   s = bs+27; al_draw_bitmap_region(tmp1, 20, 40, 20, 20, (s % 32)*22+1, (s / 32)*22+1, 0); // d edge


   // edge tees
   al_set_target_bitmap(sb);
   al_clear_to_color(bc);
   al_draw_bitmap(cf, 10,  0, ALLEGRO_FLIP_VERTICAL);         // faded tr corner
   al_draw_bitmap(cf, 10, 10, 0);                             // faded br corner
   al_set_target_bitmap(b1);
   s = bs+28; al_draw_rotated_bitmap(sb, 10, 10, (s % 32)*22+11, (s / 32)*22+11, ALLEGRO_PI * 1.0, 0);
   s = bs+29; al_draw_rotated_bitmap(sb, 10, 10, (s % 32)*22+11, (s / 32)*22+11, ALLEGRO_PI * 1.5, 0);
   s = bs+30; al_draw_rotated_bitmap(sb, 10, 10, (s % 32)*22+11, (s / 32)*22+11, ALLEGRO_PI * 0.0, 0);
   s = bs+31; al_draw_rotated_bitmap(sb, 10, 10, (s % 32)*22+11, (s / 32)*22+11, ALLEGRO_PI * 0.5, 0);



   // solid corner 1 way tees
   al_set_target_bitmap(sb);
   al_clear_to_color(bc);
   al_draw_bitmap(hf, 0, 0, 0); // left half faded
   al_draw_bitmap(cf,  10, 0, ALLEGRO_FLIP_VERTICAL); // faded tr corner
   al_set_target_bitmap(b1);
   s = bs+32; al_draw_rotated_bitmap(sb, 10, 10, (s % 32)*22+11, (s / 32)*22+11, ALLEGRO_PI * 0.0, 0);
   s = bs+33; al_draw_rotated_bitmap(sb, 10, 10, (s % 32)*22+11, (s / 32)*22+11, ALLEGRO_PI * 0.5, 0);
   s = bs+34; al_draw_rotated_bitmap(sb, 10, 10, (s % 32)*22+11, (s / 32)*22+11, ALLEGRO_PI * 1.0, 0);
   s = bs+35; al_draw_rotated_bitmap(sb, 10, 10, (s % 32)*22+11, (s / 32)*22+11, ALLEGRO_PI * 1.5, 0);

   // flip and rotate
   al_set_target_bitmap(sb);
   al_draw_rotated_bitmap(sb, 10, 10, 10, 10, ALLEGRO_PI * 1.5, ALLEGRO_FLIP_HORIZONTAL);
   al_set_target_bitmap(b1);
   s = bs+36; al_draw_rotated_bitmap(sb, 10, 10, (s % 32)*22+11, (s / 32)*22+11, ALLEGRO_PI * 0.0, 0);
   s = bs+37; al_draw_rotated_bitmap(sb, 10, 10, (s % 32)*22+11, (s / 32)*22+11, ALLEGRO_PI * 0.5, 0);
   s = bs+38; al_draw_rotated_bitmap(sb, 10, 10, (s % 32)*22+11, (s / 32)*22+11, ALLEGRO_PI * 1.0, 0);
   s = bs+39; al_draw_rotated_bitmap(sb, 10, 10, (s % 32)*22+11, (s / 32)*22+11, ALLEGRO_PI * 1.5, 0);

   // solid corner 2 way tees
   // make cross with one solid corner
   al_set_target_bitmap(sb);
   al_clear_to_color(bc);
   al_draw_bitmap(cf,  0,  0, ALLEGRO_FLIP_HORIZONTAL | ALLEGRO_FLIP_VERTICAL);  // faded corner tl
   al_draw_bitmap(cf, 10,  0, ALLEGRO_FLIP_VERTICAL);                            // faded corner tr
   al_draw_bitmap(cf,  0, 10, ALLEGRO_FLIP_HORIZONTAL);                          // faded corner bl
   al_set_target_bitmap(b1);
   s = bs+40; al_draw_rotated_bitmap(sb, 10, 10, (s % 32)*22+11, (s / 32)*22+11, ALLEGRO_PI * 0.0, 0);
   s = bs+41; al_draw_rotated_bitmap(sb, 10, 10, (s % 32)*22+11, (s / 32)*22+11, ALLEGRO_PI * 0.5, 0);
   s = bs+42; al_draw_rotated_bitmap(sb, 10, 10, (s % 32)*22+11, (s / 32)*22+11, ALLEGRO_PI * 1.0, 0);
   s = bs+43; al_draw_rotated_bitmap(sb, 10, 10, (s % 32)*22+11, (s / 32)*22+11, ALLEGRO_PI * 1.5, 0);

   // diagonal corners
   al_set_target_bitmap(sb);
   al_clear_to_color(bc);
   al_draw_bitmap(cf, 10,  0, ALLEGRO_FLIP_VERTICAL);                            // faded corner tr
   al_draw_bitmap(cf,  0, 10, ALLEGRO_FLIP_HORIZONTAL);                          // faded corner bl
   al_set_target_bitmap(b1);
   s = bs+44; al_draw_rotated_bitmap(sb, 10, 10, (s % 32)*22+11, (s / 32)*22+11, ALLEGRO_PI * 0.0, 0);
   s = bs+45; al_draw_rotated_bitmap(sb, 10, 10, (s % 32)*22+11, (s / 32)*22+11, ALLEGRO_PI * 0.5, 0);


   // save modified block tiles bitmap to file
   save_bitmap_to_block_tiles_file(b1);

   al_destroy_bitmap(b1);
   al_destroy_bitmap(tmp1);
   al_destroy_bitmap(sb);
   al_destroy_bitmap(hf);
   al_destroy_bitmap(cf);

}


























void showTileSet(int x, int y, int type, int bs)
{
   if (type == 2)
   {
      ALLEGRO_BITMAP *tmp1 = al_create_bitmap(640, 40); // 32 x 2
      al_set_target_bitmap(tmp1);
      al_clear_to_color(al_map_rgb(0,0,0));
      for (int i=0; i<2; i++)
         for (int j=0; j<32; j++)
            al_draw_bitmap(mBitmap.btile[bs+i*32+j], j*20, i*20, 0);

      al_set_target_backbuffer(mDisplay.display);
      al_draw_scaled_bitmap(tmp1, 0, 0, 640, 40, x, y, 1280, 80, 0);

      al_destroy_bitmap(tmp1);
   }


   if (type < 2)
   {
      ALLEGRO_BITMAP *tmp1 = al_create_bitmap(90, 90);

      al_set_target_bitmap(tmp1);
      al_clear_to_color(al_map_rgb(0,0,0));


      // corners
      al_draw_bitmap(mBitmap.btile[bs+8],   0,  0, 0);
      al_draw_bitmap(mBitmap.btile[bs+9],  40,  0, 0);
      al_draw_bitmap(mBitmap.btile[bs+10],  0, 40, 0);
      al_draw_bitmap(mBitmap.btile[bs+11], 40, 40, 0);

      // hline
      al_draw_bitmap(mBitmap.btile[bs+2],  0, 70, 0); // left
      al_draw_bitmap(mBitmap.btile[bs+3], 20, 70, 0); // center
      al_draw_bitmap(mBitmap.btile[bs+4], 40, 70, 0); // right

      // vline
      al_draw_bitmap(mBitmap.btile[bs+5], 70, 0,  0); // top
      al_draw_bitmap(mBitmap.btile[bs+6], 70, 20, 0); // center
      al_draw_bitmap(mBitmap.btile[bs+7], 70, 40, 0); // bottom

      // single
      al_draw_bitmap(mBitmap.btile[bs+1], 70, 70, 0);



      if (type == 1)
      {
         // solid sides
         al_draw_bitmap(mBitmap.btile[bs+14], 20,  0, 0); // top
         al_draw_bitmap(mBitmap.btile[bs+15], 20, 40, 0); // bottom
         al_draw_bitmap(mBitmap.btile[bs+12],  0, 20, 0); // left
         al_draw_bitmap(mBitmap.btile[bs+13], 40, 20, 0); // right

         // solid center
         al_draw_bitmap(mBitmap.btile[bs+0], 20, 20, 0); // center
      }

      // through pipes for frame
      if (type == 0)
      {
         al_draw_bitmap(mBitmap.btile[bs+3],  20,  0, 0); // top    - hline through
         al_draw_bitmap(mBitmap.btile[bs+3],  20, 40, 0); // bottom - hline through
         al_draw_bitmap(mBitmap.btile[bs+6],   0, 20, 0); // left   - vline through
         al_draw_bitmap(mBitmap.btile[bs+6],  40, 20, 0); // right  - vline through
      }

      al_set_target_backbuffer(mDisplay.display);
      al_draw_scaled_bitmap(tmp1, 0, 0, 90, 90, x, y, 360, 360, 0);


      // tees and cross for frame type
      if (type == 0)
      {
         al_destroy_bitmap(tmp1);
         tmp1 = al_create_bitmap(40, 40);
         al_set_target_bitmap(tmp1);
         al_clear_to_color(al_map_rgb(0,0,0));

         // corners
         al_draw_bitmap(mBitmap.btile[bs+12],  0,  0, 0);
         al_draw_bitmap(mBitmap.btile[bs+14], 20,  0, 0);
         al_draw_bitmap(mBitmap.btile[bs+15],  0, 20, 0);
         al_draw_bitmap(mBitmap.btile[bs+13], 20, 20, 0);

         al_set_target_backbuffer(mDisplay.display);
         al_draw_scaled_bitmap(tmp1, 0, 0, 40, 40, x+400, y, 160, 160, 0);

         // cross
         al_draw_scaled_bitmap(mBitmap.btile[bs], 0, 0, 20, 20, x+400, y+280, 80, 80, 0);
      }
      al_destroy_bitmap(tmp1);
   }
}


void mwBitmapTools::modify_tile_set()
{
   int bs = 256;
   int type = 2; // 0-frame 1-solid 2-extended


   int redraw = 1;

   int quit = 0;
   float h1 = 270;
   float h2 = 270;
   float s1 = 0.63;
   float s2 = 0.80;
   float l1 = 0.10;
   float l2 = 0.35;
   int steps = 9;

   float round = 7.5;

   al_show_mouse_cursor(mDisplay.display);

   while (!quit)
   {
      if (redraw)
      {
         redraw = 0;
         mBitmap.load_tiles();
         al_set_target_backbuffer(mDisplay.display);
         al_clear_to_color(al_map_rgb(0,0,0));

         showTileSet(20, 120, type, bs);

      }

      mEventQueue.proc(1);

      al_flip_display();


      int old_steps = steps;
      float old_h1 = h1;
      float old_h2 = h2;
      float old_s1 = s1;
      float old_s2 = s2;
      float old_l1 = l1;
      float old_l2 = l2;

      float old_round = round;

      int ya = 8;
      int xa = 20;
      int xb = 260;

      mWidget.slideri(xa, ya, xb, 12,  0,0,0,0,  0,12,15,15,  0,0,1,0, steps, 20, 0,    1, "steps:");
      mWidget.sliderf(xa, ya, xb, 12,  0,0,0,0,  0,12,15,15,  0,0,1,0, h1,   360, 0,  0.1, "h1:");
      mWidget.sliderf(xa, ya, xb, 12,  0,0,0,0,  0,12,15,15,  0,0,1,0, h2,   360, 0,  0.1, "h2:");
      mWidget.sliderf(xa, ya, xb, 12,  0,0,0,0,  0,12,15,15,  0,0,1,0, s1,     1, 0, 0.01, "s1:");
      mWidget.sliderf(xa, ya, xb, 12,  0,0,0,0,  0,12,15,15,  0,0,1,0, s2,     1, 0, 0.01, "s2:");
      mWidget.sliderf(xa, ya, xb, 12,  0,0,0,0,  0,12,15,15,  0,0,1,0, l1,     1, 0, 0.01, "l1:");
      mWidget.sliderf(xa, ya, xb, 12,  0,0,0,0,  0,12,15,15,  0,0,1,0, l2,     1, 0, 0.01, "l2:");
      mWidget.sliderf(xa, ya, xb, 12,  0,0,0,0,  0,12,15,15,  0,0,1,0, round,  16, 0, 0.5, "round:");

      if (old_steps != steps || old_h1 != h1 || old_h2 != h2 || old_s1 != s1 || old_s2 != s2 || old_l1 != l1 || old_l2 != l2 || old_round != round)
      {
         //create_tileset_solid(bs, 896, h1, h2, s1, s2, l1, l2, steps, round);
         //create_tileset_solid(bs, 0, h1, h2, s1, s2, l1, l2, steps, round);
         //create_tileset_frame(bs, h1, h2, s1, s2, l1, l2, steps, round);
         create_tileset_extended(bs, h1, h2, s1, s2, l1, l2, steps, round);


         redraw = 1;

      }

      if (mInput.key[ALLEGRO_KEY_ESCAPE][0])
      {
         while (mInput.key[ALLEGRO_KEY_ESCAPE][0]) mEventQueue.proc(1);
         quit = 1;
      }
   }
}

