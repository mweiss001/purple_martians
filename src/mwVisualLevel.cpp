// mwVisualLevel.cpp

#include "pm.h"
#include "mwVisualLevel.h"
#include "mwPlayers.h"
#include "mwDisplay.h"
#include "mwFont.h"
#include "mwLift.h"
#include "mwColor.h"
#include "mwInput.h"
#include "mwEventQueue.h"
#include "mwLoop.h"
#include "mwItems.h"
#include "mwEnemy.h"
#include "mwLevel.h"
#include "mwScreen.h"


mwVisualLevel mVisualLevel;

void mwVisualLevel::mark_rect(int sel, int color)
{
   int y = (sel/20) * 50;
   int x = (sel % 20) * 50;
   al_draw_rectangle(x,     y, x+49, y+49, mColor.pc[color], 1);
   al_draw_rectangle(x+1, y+1, x+48, y+48, mColor.pc[color], 1);
}

void mwVisualLevel::show_cur(void)
{
   int xpos = 1000+ (1280 - 1000)/2;
   al_draw_text(mFont.pr8, mColor.pc[14], xpos, 4, ALLEGRO_ALIGN_CENTER,"Currently selected level");
   al_draw_textf(mFont.pr8, mColor.pc[15], xpos, 15, ALLEGRO_ALIGN_CENTER, "Level:[%d]", cur);
   al_draw_rectangle(1000, 24, 1279, 303, mColor.pc[14], 1 );
   al_draw_rectangle(1001, 25, 1278, 302, mColor.pc[14], 1 );

   int ts = 110;
   al_draw_rectangle(1000, 24, 1279, 303+ts, mColor.pc[14], 1 );
   al_draw_rectangle(1001, 25, 1278, 302+ts, mColor.pc[14], 1 );

   if (mLevel.load_level(cur, 0, 1))
   {
      al_set_target_backbuffer(mDisplay.display);
      mScreen.draw_level2(NULL, 1002, 26, 276, 1, 1, 1, 1, 0);
      al_set_target_backbuffer(mDisplay.display);
      mLevel.show_level_data(1010, 308, 0);
   }
   else al_draw_text(mFont.pr8, mColor.pc[10], xpos, 30, ALLEGRO_ALIGN_CENTER,"not found");

}

void mwVisualLevel::show_msel(void)
{
   int xpos = 1000+ (1280 - 1000)/2;
   int yo = 1024/2 - 96;
   al_draw_text(mFont.pr8, mColor.pc[10], xpos, yo + 4, ALLEGRO_ALIGN_CENTER,"Level under mouse");
   al_draw_textf(mFont.pr8, mColor.pc[10], xpos, yo + 15, ALLEGRO_ALIGN_CENTER, "Level:[%d]", sel);
   al_draw_rectangle(1000, yo + 24, 1279, yo + 303, mColor.pc[10], 1 );
   al_draw_rectangle(1001, yo + 25, 1278, yo + 302, mColor.pc[10], 1 );

   int ts = 110;
   al_draw_rectangle(1000, yo + 24, 1279, yo + 303+ts, mColor.pc[10], 1 );
   al_draw_rectangle(1001, yo + 25, 1278, yo + 302+ts, mColor.pc[10], 1 );

   if (mLevel.load_level(sel, 0, 1))
   {
      al_set_target_backbuffer(mDisplay.display);
      mScreen.draw_level2(NULL, 1002, yo+26, 276, 1, 1, 1, 1, 0);
      al_set_target_backbuffer(mDisplay.display);
      mLevel.show_level_data(1010, yo + 308, 0);
   }
   else al_draw_text(mFont.pr8, mColor.pc[10], xpos, yo + 30, ALLEGRO_ALIGN_CENTER,"not found" );
}


void mwVisualLevel::compare_curr(int sel)
{
   char msg[1024];
   // check if other levels are similar

   // compare l[100][100]
   // count number of non-zero blocks
   // count how many match with other levels...

   int m[100][100];

   // copy current level to m
   for (int a=0; a<100; a++)
      for (int b=0; b<100; b++)
         m[a][b] = mLevel.l[a][b];

   // iterate and load levels
   int le[NUM_LEV]; // level exists array
   int num_levs = 0;

   for (int x=0; x<NUM_LEV; x++) le[x] = 0;

   char fn[20] = "levels/level000.pml";

   // level range to look for
   int lll = 0; // lower level limit
   int ull = 400; // upper level limit

   // look for levels that exist and put them in array
   for (int x=lll; x<ull; x++)
   {
      int h, d, rem = x;

      h = rem/100; // hundreds
      fn[12] = 48+h;
      rem -=h*100;

      d = rem/10;  // tens
      fn[13] = 48 + d;
      rem -=d*10;

      fn[14] = 48 + rem;
      if (al_filename_exists(fn)) le[num_levs++] = x; // put in array
   }

   int num_matches = 0;


   int ypos = 0;

   al_set_target_backbuffer(mDisplay.display);
   al_clear_to_color(al_map_rgb(0,0,0));
   sprintf(msg, "<-------------- Compare Level:%d -------------->", sel);
   al_draw_text(mFont.pr8, mColor.pc[15], 0, ypos, 0, msg); ypos+=8;
   printf("%s\n", msg);
   al_flip_display();


   // load every level
   for (int x=0; x<num_levs; x++)
      if (mLevel.load_level(le[x], 1, 1))
      {
         int sb1 = 0, sb2 = 0, sbm = 0;
         for (int a=0; a<100; a++)
            for (int b=0; b<100; b++)
            {
               if (mLevel.l[a][b]) sb2++;
               if (m[a][b])
               {
                  sb1++;
                  if (m[a][b] == mLevel.l[a][b]) sbm++;
                  }
            }

         int match = sbm*100/sb1;

         // printf("match !!compare to level:[%d] sb1:%d sb2:%d smb:%d [%d]\n", le[x], sb1, sb2, sbm, sbm*100/sb1);

         if (match > 90)
         {
            al_set_target_backbuffer(mDisplay.display);
            sprintf(msg, "----> match [%3d%%] with level:%3d <----", match, le[x]);
            al_draw_text(mFont.pr8, mColor.pc[15], 0, ypos, 0, msg); ypos+=8;
            num_matches++;
         }

      }
   al_set_target_backbuffer(mDisplay.display);
   sprintf(msg, "<--------Done. Press any key to continue------->");
   al_draw_text(mFont.pr8, mColor.pc[15], 0, ypos, 0, msg);
   printf("%s\n", msg);
   al_flip_display();
   mInput.tsw();
}
void mwVisualLevel::compare_all(void)
{
   char msg[1024];
   // iterate and load levels
   int le[NUM_LEV]; // level exists array
   int num_levs = 0;

   for (int x=0; x<NUM_LEV; x++) le[x] = 0;

   char fn[20] = "levels/level000.pml";

   // level range to look for
   int lll = 0; // lower level limit
   int ull = 400; // upper level limit

   // look for levels that exist and put them in array
   for (int x=lll; x<ull; x++)
   {
      int h, d, rem = x;

      h = rem/100; // hundreds
      fn[12] = 48+h;
      rem -=h*100;

      d = rem/10;  // tens
      fn[13] = 48 + d;
      rem -=d*10;

      fn[14] = 48 + rem;
      if (al_filename_exists(fn)) le[num_levs++] = x; // put in array
   }

   int num_matches = 0;
   int ypos = 0;


   al_set_target_backbuffer(mDisplay.display);
   al_clear_to_color(al_map_rgb(0,0,0));

   sprintf(msg, "<----------- Compare all Levels ----------->");
   al_draw_text(mFont.pr8, mColor.pc[15], mDisplay.SCREEN_W/2, ypos, ALLEGRO_ALIGN_CENTER, msg);
   printf("%s\n", msg);

   ypos = 60;

   sprintf(msg, "<-------------List of matches------------------>");
   al_draw_text(mFont.pr8, mColor.pc[15], 10, ypos, 0, msg); ypos+=8;
   printf("%s\n", msg);
   al_flip_display();

   // load every level (outer loop)
   for (int x=0; x<num_levs; x++)
      if (mLevel.load_level(le[x], 1, 1))
      {

         al_set_target_backbuffer(mDisplay.display);
         int pc = (x+1)*100 / num_levs;
         mScreen.draw_percent_bar(mDisplay.SCREEN_W/2, 10, mDisplay.SCREEN_W-200, 14, pc );

         sprintf(msg, "checking level:%d", le[x]);
         al_draw_text(mFont.pr8, mColor.pc[15], mDisplay.SCREEN_W/2, 14, ALLEGRO_ALIGN_CENTER, msg);

         printf("%s\n", msg);

         int sb = 0;
         int m[100][100];
         // copy current level to m
         for (int a=0; a<100; a++)
            for (int b=0; b<100; b++)
            {
               m[a][b] = mLevel.l[a][b];
               if (mLevel.l[a][b]) sb++; // count solid blocksin source
            }

         // load every level (inner loop)
         for (int x1=0; x1<num_levs; x1++)
            if (mLevel.load_level(le[x1], 1, 1))
            {

               al_set_target_backbuffer(mDisplay.display);
               int pc = (x1+1)*100 / num_levs;
               mScreen.draw_percent_bar(mDisplay.SCREEN_W/2, 30, mDisplay.SCREEN_W-200, 14, pc );

               sprintf(msg, "checking level:%d", le[x1]);
               al_draw_text(mFont.pr8, mColor.pc[15], mDisplay.SCREEN_W/2, 34, ALLEGRO_ALIGN_CENTER, msg);

               al_flip_display();

               int sbm = 0;
               for (int a=0; a<100; a++)
                  for (int b=0; b<100; b++)
                     if (m[a][b])
                        if (m[a][b] == mLevel.l[a][b]) sbm++; // count solid block matches

               int match = sbm*100/sb;

               // printf("match !!compare to level:[%d] sb1:%d sb2:%d smb:%d [%d]\n", le[x], sb1, sb2, sbm, sbm*100/sb1);

               if (match > 90)
               {
                  if (le[x] != le[x1])
                  {
                     al_set_target_backbuffer(mDisplay.display);
                     sprintf(msg, "----> level%d - match [%d%%] with level:%d <----", le[x], match, le[x1]);
                     al_draw_text(mFont.pr8, mColor.pc[15], 10, ypos, 0, msg); ypos+=8;
                     printf("%s\n", msg);
                     num_matches++;
                   }
               }
               mEventQueue.proc_menu();
               if (mInput.key[ALLEGRO_KEY_ESCAPE][3]) x1 = num_levs; // break out of inner loop
            } // next level (inner loop)
         mEventQueue.proc_menu();
         if (mInput.key[ALLEGRO_KEY_ESCAPE][3]) x = num_levs;  // break out of outer loop
   } // next level (outer loop)

   al_set_target_backbuffer(mDisplay.display);
   sprintf(msg, "<--------Done. Press any key to continue------->");
   al_draw_text(mFont.pr8, mColor.pc[15], 10, ypos, 0, msg);
   printf("%s\n", msg);
   al_flip_display();
   mInput.tsw();
}







void mwVisualLevel::lev_draw(int full)
{
   int ms = 50; // map size
   if (full) // load all the levels again
   {
      al_set_target_bitmap(le_temp);
      al_clear_to_color(al_map_rgb(0,0,0));
      for (int my=0; my<20; my++)
         for (int mx=0; mx<20; mx++)
         {
            int level = my*20 + mx;
            int col = 11;
            if (!mLevel.load_level(level, 0, 1)) col = 10;
            al_set_target_bitmap(le_temp);
            if (mLevel.valid_level_loaded) mScreen.draw_level2(le_temp, mx*ms, my*ms, ms, 1, 1, 1, 1, 0);
            al_draw_textf(mFont.pr8, mColor.pc[col], mx*ms +ms/2, my*ms+ms/2, ALLEGRO_ALIGN_CENTER, "%d", level);

            // show progress bar
            int pc = level*100 / 400;
            al_set_target_backbuffer(mDisplay.display);
            //al_clear_to_color(al_map_rgb(0,0,0));
            mScreen.draw_percent_bar(mDisplay.SCREEN_W/2, mDisplay.SCREEN_H/2, mDisplay.SCREEN_W-200, 20, pc );
            al_draw_text(mFont.pr8, mColor.pc[15], mDisplay.SCREEN_W/2, mDisplay.SCREEN_H/2+6, ALLEGRO_ALIGN_CENTER, "Loading Levels");
            al_flip_display();
         }

   }

   al_set_target_backbuffer(mDisplay.display);
   al_draw_bitmap(le_temp,0,0,0);

   mark_rect(sel, 10);
   mark_rect(cur, 14);

   show_cur();
   show_msel();

   int xpos = 1000 + (1280 - 1000)/2;
   int ty1 = 840;

   al_draw_text(mFont.pr8, mColor.pc[14], xpos, ty1+=8, ALLEGRO_ALIGN_CENTER, "Change currently selected");
   al_draw_text(mFont.pr8, mColor.pc[14], xpos, ty1+=8, ALLEGRO_ALIGN_CENTER, "level with left mouse button");
   ty1+=20;

   al_draw_text(mFont.pr8, mColor.pc[10], xpos, ty1+=8, ALLEGRO_ALIGN_CENTER, "Move currently selected");
   al_draw_text(mFont.pr8, mColor.pc[10], xpos, ty1+=8, ALLEGRO_ALIGN_CENTER, "level to empty level");
   al_draw_text(mFont.pr8, mColor.pc[10], xpos, ty1+=8, ALLEGRO_ALIGN_CENTER, "with right mouse button");
   ty1+=20;

   al_draw_text(mFont.pr8, mColor.pc[11], xpos, ty1+=8, ALLEGRO_ALIGN_CENTER, "View currently selected" );
   al_draw_text(mFont.pr8, mColor.pc[11], xpos, ty1+=8, ALLEGRO_ALIGN_CENTER, "level full screen");
   al_draw_text(mFont.pr8, mColor.pc[11], xpos, ty1+=8, ALLEGRO_ALIGN_CENTER, "by holding 'S'");
   al_draw_text(mFont.pr8, mColor.pc[11], xpos, ty1+=8, ALLEGRO_ALIGN_CENTER, "or right mouse button");
   ty1+=20;
   al_draw_text(mFont.pr8, mColor.pc[12], xpos, ty1+=8, ALLEGRO_ALIGN_CENTER, "Quit with ESC");
}

void mwVisualLevel::level_viewer(void)
{
   char msg[1024];
   le_temp = al_create_bitmap(1000,1000);

//   show_pixel_format(al_get_bitmap_format(le_temp));
//   show_bitmap_flags(al_get_bitmap_flags(le_temp));

   int redraw = 2;
   int quit = 0;

   while (!quit)
   {
      mEventQueue.proc_menu();

      if (redraw)
      {
         al_set_target_backbuffer(mDisplay.display);
         al_clear_to_color(al_map_rgb(0,0,0));
         if (redraw == 1) lev_draw(0);
         if (redraw == 2) lev_draw(1); // full reload of levels
         redraw = 0;
         al_flip_display();
      }

      if (mInput.key[ALLEGRO_KEY_ESCAPE][3]) quit = 1;
      if (mInput.key[ALLEGRO_KEY_A][3])
      {
         compare_all();
         redraw = 1;
      }
      if (mInput.key[ALLEGRO_KEY_C][0])
      {
         while (mInput.key[ALLEGRO_KEY_C][0]) mEventQueue.proc_menu();
         compare_curr(sel);
         redraw = 1;
      }

      if (mInput.key[ALLEGRO_KEY_S][0])
      {
         while (mInput.key[ALLEGRO_KEY_S][0]) mEventQueue.proc_menu();
         if (mLevel.load_level(cur, 0, 1)) mScreen.draw_level2(NULL, 0, 0, 1000, 1, 1, 1, 1, 0);
         al_flip_display();
         while (mInput.key[ALLEGRO_KEY_S][0]) mEventQueue.proc_menu();
         redraw = 1;
      }


      if ((mInput.mouse_x < 1000) && (mInput.mouse_y < 1000))  // mouse is on the level grid
      {
         sel = (mInput.mouse_y/50) * 20 + (mInput.mouse_x/50); // highlight the level the mouse is over
         if (sel != old_sel)
         {
            old_sel = sel;
            redraw = 1;
         }
         if (mInput.mouse_b[1][0]) // set new current level (and show full screen while mouse b1 pressed)
         {
            cur = sel;
            if (mLevel.load_level(cur, 0, 1)) mScreen.draw_level2(NULL, 0, 0, 1000, 1, 1, 1, 1, 0);
            al_flip_display();
            redraw = 1;
            while (mInput.mouse_b[1][0]) mEventQueue.proc_menu();
         }

         if (mInput.mouse_b[2][0]) // copy current to selected
         {
            while (mInput.mouse_b[2][0]) mEventQueue.proc_menu();
            if ((cur != 0) && (sel !=0) && (cur != sel))
            {
               if (mLevel.load_level(cur, 0, 1) == 1) // cur exists
                  if (mLevel.load_level(sel, 0, 1) == 0) // sel does not exist
                  {
                     sprintf(msg, "Level:%d to Level:%d ?", cur, sel);
                     if (al_show_native_message_box(mDisplay.display,
                                 "Move?", "Are you sure you want to move", msg, NULL,
                                 ALLEGRO_MESSAGEBOX_YES_NO | ALLEGRO_MESSAGEBOX_QUESTION ) == 1)
                     {
                        char lf[256];

                        mLevel.load_level(cur, 0, 1);           // load current level
                        mLevel.save_level(sel);              // save as new level

                        sprintf(lf, "levels/level%03d.pml", cur); // make filename for old
                        ALLEGRO_FS_ENTRY *df = al_create_fs_entry(lf);
                        al_remove_fs_entry(df);
                        redraw = 2;                   // redraw everything (reload all levels)
                     }
                  }
            }
         }  // end of mouse b2
       } // end of mouse on map
   } // end of while !ESC
   al_destroy_bitmap(le_temp);
}


void mwVisualLevel::show_cur_vs(int cur, int x1, int y1, int size, int fc)
{
   int x2 = x1 + size;
   int y2 = y1 + size;
   int xc = x1 + size / 2; // text center
   al_draw_text(mFont.pr8, mColor.pc[15], xc, y1+4-5, ALLEGRO_ALIGN_CENTER,"Currently selected level");
   for (int a=0; a<16; a++)
      al_draw_rectangle(x1+0-a, y1+24-a, x2-0+a, y2+24+a, mColor.pc[fc + (15-a)*16], 1 );
   int tc = 15;
   if (fc == 15) tc = 0;
   al_draw_textf(mFont.pr8, mColor.pc[tc], xc, y1+15-3, ALLEGRO_ALIGN_CENTER, "Level %d", cur);
   if (mLevel.load_level(cur, 0, 1))
   {
      mScreen.draw_level2(NULL, x1+2, y1+26, size-3, 1, 1, 1, 1, 0);
      int ty1 = mLevel.show_level_data(x1+2, y2+32+24, 0);
      for (int a=0; a<16; a++)
         al_draw_rectangle(x1+0-a, y2+55-a, x2-0+a, ty1+a, mColor.pc[fc + (15-a)*16], 1 );
   }
   else al_draw_text(mFont.pr8, mColor.pc[10], xc, y1+30, ALLEGRO_ALIGN_CENTER, "not found" );
}

void mwVisualLevel::load_visual_level_select(void)
{
   num_levs = 0;
   load_visual_level_select_done = 1;
   for (int x=0; x<NUM_LEV; x++) le[x] = 0;

   // level range to look for
   int lll = 1;   // lower level limit
   int ull = VISUAL_LEVEL_SELECT_MAX_LEVEL; // upper level limit

   // look for levels that exist and put them in array
   for (int x=lll; x<ull; x++)
   {
      char fn[256];
      sprintf(fn, "levels/level%03d.pml", x);
      if (al_filename_exists(fn)) le[num_levs++] = x; // put in array
   }

   // now we have the number of levels, we can figure out grid sizes
   // make selection map size 1/3 of mDisplay.SCREEN_W
   sel_size = mDisplay.SCREEN_W / 3;

   // position it at 2/3 of mDisplay.SCREEN_W
   sel_x = mDisplay.SCREEN_W * 2 / 3 - 2 -16;  // -16 for frame
   sel_y = 0;

   // how much x space is left?
   int x_space = sel_x-2 -32 -16 ;

   // what is the area of the space left
   int area = x_space * mDisplay.SCREEN_H;


   // how much space for each icon
   int ia = area / num_levs;
//   printf("ia:%d\n", ia);

   // get the size from this
   grid_size = (int) sqrt((float)ia);
//   printf("gs:%d\n", grid_size);
   grid_size -= grid_size% 20; // mod 20
//   printf("gs:%d\n", grid_size);
   if (grid_size < 20) grid_size = 10;
//   printf("gs:%d\n", grid_size);

   // how many icon will fit vertically?
   grid_cols = x_space / grid_size;
   grid_width = grid_cols * grid_size;


   // how many horizontal rows are needed?
   grid_rows = (num_levs-1) / grid_cols+1;
   grid_height = grid_rows * grid_size;



   // check if the intended grid is off the bottom of the screen
   while (grid_height >= mDisplay.SCREEN_H)
   {
      grid_size -= 20;

      if (grid_size < 20)
      {
         grid_size = 10;
//         printf("gs:%d\n", grid_size);
         break;
      }


      // how many icon will fit vertically
      grid_cols = x_space / grid_size;
      grid_width = grid_cols * grid_size;

      // how many horizontal rows are needed?
      grid_rows = (num_levs-1) / grid_cols+1;
      grid_height = grid_rows * grid_size;
   }




   // how many icon will fit vertically
   grid_cols = x_space / grid_size;
   grid_width = grid_cols * grid_size;

   // how many horizontal rows are needed?
   grid_rows = (num_levs-1) / grid_cols+1;
   grid_height = grid_rows * grid_size;

   // resize the selection map to fit available space
   sel_size = mDisplay.SCREEN_W - 64 - grid_width; // (32 for grid frame, 32 for selmap frame)
   sel_x = mDisplay.SCREEN_W - sel_size - 16;  // -16 for frame



   // create icon bitmaps
   for (int x=0; x<num_levs; x++)
   {
      level_icon_bmp[x] = al_create_bitmap(grid_size, grid_size);

      al_set_target_bitmap(level_icon_bmp[x]);
      al_clear_to_color(al_map_rgb(0,0,0));
   }



//   show_pixel_format(al_get_bitmap_format(level_icon_bmp[0]));
//   show_bitmap_flags(al_get_bitmap_flags(level_icon_bmp[0]));



   // load every level and get icon bitmaps
   for (int x=0; x<num_levs; x++)
      if (mLevel.load_level(le[x], 0, 1))
      {
         al_set_target_bitmap(level_icon_bmp[x]);
         mScreen.draw_level2(level_icon_bmp[x], 0, 0, grid_size, 1, 1, 1, 1, 0);
         int pc = x*100 / num_levs;

         al_set_target_backbuffer(mDisplay.display);
         mScreen.draw_percent_bar(mDisplay.SCREEN_W/2, mDisplay.SCREEN_H/2, mDisplay.SCREEN_W-200, 20, pc );
         al_draw_text(mFont.pr8, mColor.pc[15], mDisplay.SCREEN_W/2, mDisplay.SCREEN_H/2+6, ALLEGRO_ALIGN_CENTER, "Creating level icon grid");
         al_flip_display();
      }

   // create the icon grid bitmap
   al_destroy_bitmap(grid_bmp);
   grid_bmp = al_create_bitmap(grid_width, grid_height);
   al_set_target_bitmap(grid_bmp);
   al_clear_to_color(al_map_rgb(0,0,0));

   // draw the level icon grid on its bitmap
   for (int row=0; row<grid_rows; row++)
      for (int col=0; col<grid_cols; col++)
      {
         int grid_pos = (row * grid_cols) + col;     // the position in the grid
         int lev = le[grid_pos];                     // the level number that corresponds to that grid position
         int bx1 = col * grid_size;                  // the screen position of that grid cell
         int by1 = row * grid_size;
         if (lev)
         {
            al_draw_bitmap(level_icon_bmp[grid_pos], bx1, by1, 0);
            al_draw_textf(mFont.pr8, mColor.pc[15], bx1 + grid_size/2-8, by1 + grid_size/2-4, 0, "%d", lev);
         }
      } // end of grid iterate
   for (int x=0; x<num_levs; x++)  al_destroy_bitmap(level_icon_bmp[x]);
}

int mwVisualLevel::visual_level_select(void)
{
   mLoop.visual_level_select_running = 1;

   int p = mPlayer.active_local_player;
   int fc = mPlayer.syn[p].color; // frame color

   while ( (mInput.key[ALLEGRO_KEY_ENTER][0]) || (mPlayer.syn[0].fire) || (mPlayer.syn[0].jump) ) mEventQueue.proc_menu();;

   if (!load_visual_level_select_done) load_visual_level_select();

   int quit = 0;
   int grid_sel_row, grid_sel_col;

   int up_held = 0;
   int down_held = 0;
   int left_held = 0;
   int right_held = 0;

   // get the initial selection
   int selected_level = mLevel.start_level;

   int ss = 0;
   for (int x=0; x<num_levs; x++)
      if (le[x] == selected_level) ss = x;
   grid_sel_row = ss / grid_cols;
   grid_sel_col = ss - grid_sel_row * grid_cols;

   vl_redraw = 1;
   while (!quit)
   {
      if (!load_visual_level_select_done)
      {
         load_visual_level_select();
         vl_redraw = 1;
      }
      if (vl_redraw)
      {
         al_set_target_backbuffer(mDisplay.display);
         vl_redraw = 0;
         al_clear_to_color(al_map_rgb(0,0,0));

         // draw the grid of icons
         al_draw_bitmap(grid_bmp, 16, 16, 0);

         // frame the grid
         int gx2 = 15 + grid_width;
         int gy2 = 15 + grid_height;
         for (int a=-16; a<0; a++)
            al_draw_rectangle(16+a, 16+a, gx2-a, gy2-a, mColor.pc[256+fc+a*16], 1);

         // frame the selection on grid
         int bx1 = 16 + grid_sel_col * grid_size;
         int by1 = 16 + grid_sel_row * grid_size;
         int bx2 = bx1 + grid_size - 1;
         int by2 = by1 + grid_size - 1;
         for (int a=-16; a<0; a++)
            al_draw_rectangle(bx1+a, by1+a, bx2-a, by2-a, mColor.pc[256+fc+a*16], 1);

         // show the large selected level
         show_cur_vs(selected_level, sel_x, 70, sel_size, fc);

         // text legend position
         int tx = sel_x + (sel_size/2);
         int ty1 = 0;

         // draw the legend frame
         int x1 = sel_x;
         int x2 = sel_x + sel_size;
         int y1 = ty1+15;
         int y2 = ty1+46;
         for (int a=0; a<16; a++)
            al_draw_rectangle(x1-a, y1-a, x2+a, y2+a, mColor.pc[fc + (15-a)*16], 1 );

         // draw the legend
         al_draw_text(mFont.pr8, mColor.pc[14], tx, ty1+=16, ALLEGRO_ALIGN_CENTER, "Choose a level with <ENTER>");
         al_draw_text(mFont.pr8, mColor.pc[11], tx, ty1+=12, ALLEGRO_ALIGN_CENTER, "Arrow keys change selection" );
         al_draw_text(mFont.pr8, mColor.pc[10], tx, ty1+=12, ALLEGRO_ALIGN_CENTER, "<ESC> to abort");

         al_flip_display();
      } // end of redraw



      mEventQueue.proc_menu();

      if (((mInput.key[ALLEGRO_KEY_LEFT][0]) || (mPlayer.syn[0].left)) && (left_held == 0))
      {
         left_held = 1;
         while ((mInput.key[ALLEGRO_KEY_LEFT][0]) || (mPlayer.syn[0].left)) mEventQueue.proc_menu();
         if (--grid_sel_col < 0) grid_sel_col = 0;
         vl_redraw = 1;
      }
      if ( (!(mInput.key[ALLEGRO_KEY_LEFT][0])) &&  (!(mPlayer.syn[0].left)) )  left_held = 0;

      if (((mInput.key[ALLEGRO_KEY_UP][0]) || (mPlayer.syn[0].up)) && (up_held == 0))
      {
         up_held = 1;
         while ((mInput.key[ALLEGRO_KEY_UP][0]) || (mPlayer.syn[0].up)) mEventQueue.proc_menu();
         if (--grid_sel_row < 0) grid_sel_row = 0;
         vl_redraw = 1;
      }
      if ( (!(mInput.key[ALLEGRO_KEY_UP][0])) && (!(mPlayer.syn[0].up)) ) up_held = 0;

      if (((mInput.key[ALLEGRO_KEY_RIGHT][0]) || (mPlayer.syn[0].right)) && (right_held == 0))
      {
         right_held = 1;
         while ((mInput.key[ALLEGRO_KEY_RIGHT][0]) || (mPlayer.syn[0].right)) mEventQueue.proc_menu();
         if (++grid_sel_col > grid_cols-1) grid_sel_col = grid_cols-1;
         // don't allow select of empty level
         if (le[(grid_sel_row * grid_cols) + grid_sel_col] == 0) grid_sel_col--;
         vl_redraw = 1;
      }
      if ( (!(mInput.key[ALLEGRO_KEY_RIGHT][0])) &&  (!(mPlayer.syn[0].right)) )  right_held = 0;

      if (((mInput.key[ALLEGRO_KEY_DOWN][0]) || (mPlayer.syn[0].down)) && (down_held == 0))
      {
         down_held = 1;
         while ((mInput.key[ALLEGRO_KEY_DOWN][0]) || (mPlayer.syn[0].down)) mEventQueue.proc_menu();
         if (++grid_sel_row > grid_rows-1) grid_sel_row = grid_rows-1;
         // don't allow select of empty level
         if (le[(grid_sel_row * grid_cols) + grid_sel_col] == 0) grid_sel_row--;
         vl_redraw = 1;
      }
      if ( (!(mInput.key[ALLEGRO_KEY_DOWN][0])) && (!(mPlayer.syn[0].down)) ) down_held = 0;

      // these checks would normally never be needed, unless screen size changes
      if (grid_sel_col > grid_cols-1) grid_sel_col = 0;
      if (grid_sel_row > grid_rows-1) grid_sel_row = 0;
      if (le[(grid_sel_row * grid_cols) + grid_sel_col] == 0)
      {
         grid_sel_row = 0;
         grid_sel_col = 0;
      }

      // update selected row
      selected_level = le[grid_sel_row * grid_cols + grid_sel_col];

      if ( (mInput.key[ALLEGRO_KEY_ENTER][3]) || (mPlayer.syn[0].fire) || (mPlayer.syn[0].jump) )
      {
         while ((mPlayer.syn[0].jump) || (mPlayer.syn[0].fire)) mEventQueue.proc_menu();
         mLevel.set_start_level(selected_level);
         quit = 1;
      }

      if (mInput.key[ALLEGRO_KEY_END][3]) // start level editor
      {
         mLevel.set_start_level(selected_level);
         quit = 3;
      }



      if (mInput.key[ALLEGRO_KEY_ESCAPE][3])
      {
         mLevel.set_start_level(selected_level);
         quit = 2;
      }



   }
   mLoop.visual_level_select_running = 0;
   if (quit == 1) return 1;
   if (quit == 3) return 3;
   return 0;
}










