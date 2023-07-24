// mwGlobalLevelTool.cpp

#include "pm.h"
#include "mwGlobalLevelTool.h"
#include "mwDisplay.h"
#include "mwFont.h"
#include "mwBitmap.h"
#include "mwColor.h"
#include "mwItem.h"
#include "mwLevel.h"
#include "mwScreen.h"
#include "mwEnemy.h"
#include "mwInput.h"
#include "mwMiscFnx.h"
#include "mwTriggerEvent.h"
#include "mwLift.h"




mwGlobalLevelTool mGlobalLevelTool;

void mwGlobalLevelTool::show_block_list(int blt[])
{
   int y = 0;
   int count_unique = 0;

   al_draw_filled_rectangle(200, 0, mDisplay.SCREEN_W-1, mDisplay.SCREEN_H-1, mColor.pc[0]);
   for (int z=0; z<NUM_SPRITES; z++)
      if (blt[z])
      {
         count_unique++;
         al_draw_bitmap(mBitmap.btile[z], 180, y, 0);
         //al_draw_textf(mFont.pr8, mColor.pc[11], 200, y+6, 0, "sa%d   block# %d   count %d ",mBitmap.sa[z][0],  z, blt[z] );
         al_draw_textf(mFont.pr8, mColor.pc[11], 200, y+6, 0, "block:%d count:%d ", z, blt[z] );
         printf("block:%d count:%d\n", z, blt[z] );
         y+=20;
         if (y > mDisplay.SCREEN_H-20)
         {
            al_flip_display();
            al_clear_to_color(al_map_rgb(0,0,0));
            mInput.tsw();
            y = 0;
         }
      }

   printf("\nunique blocks:%d\n", count_unique);

   al_flip_display();
   mInput.tsw();

}


void mwGlobalLevelTool::remove_unused_tiles(int blt[])
{
   for (int z=0; z<NUM_SPRITES; z++)
   {
      if (blt[z] == 0) // block is not used
      {
         al_set_target_bitmap(mBitmap.btile[z]);
         al_clear_to_color(al_map_rgb(0,0,0));
      }
   }

   ALLEGRO_BITMAP* temp = al_create_bitmap(640, 640);
   al_set_target_bitmap(temp);
   for (int y = 0; y < 32; y++)
      for (int x = 0; x < 32; x++)
         al_draw_bitmap(mBitmap.btile[y*32 + x], (x*20), (y*20), 0);

   al_save_bitmap("bitmaps/tempb_tiles.bmp", temp);
   al_destroy_bitmap(temp);

}


void mwGlobalLevelTool::execute(void)
{
   //int blt[NUM_SPRITES] = {0};

   int old_start_level = mLevel.start_level;

   int le[200] = {0}; // level exists array

   int num_levs = 0;
   char fn[20] = "levels/level000.PML";

   // level range to look for
   for (int x=0; x<400; x++)
   {
      int h, d, rem = x;

      h = rem/100;
      fn[12] = 48+h;
      rem -=h*100;

      d = rem/10;
      fn[13] = 48 + d;
      rem -=d*10;

      fn[14] = 48 + rem;

      if (al_filename_exists(fn)) le[num_levs++] = x; // put in array
   }

   int count0 = 0;
   int count1 = 0;
   int count2 = 0;
   int count3 = 0;
//   int count4 = 0;
//   int count5 = 0;

   int max = 0;
   int min = 9990;

   al_set_target_backbuffer(mDisplay.display);
   al_flip_display();
   al_clear_to_color(al_map_rgb(0,0,0));

   printf("\nRunning Global Level Test\n");


   // iterate array of found levels
   for (int x=0; x<num_levs; x++)
   {
      al_set_target_backbuffer(mDisplay.display);
      al_flip_display();

      // progress bar
      mScreen.draw_percent_bar(mDisplay.SCREEN_W/2, mDisplay.SCREEN_H/2, mDisplay.SCREEN_W-200, 20, (x+1)*100 / num_levs);
      al_draw_text(mFont.pr8, mColor.pc[15], mDisplay.SCREEN_W/2, mDisplay.SCREEN_H/2+7 , ALLEGRO_ALIGN_CENTER, "Doing glt...");
      al_draw_textf(mFont.pr8, mColor.pc[11], 10, 10+x*8, 0, "lev:%d", le[x]);
      mLevel.load_level(le[x], 1, 1);


//      for (int y=0; y<100; y++)
//      {
//         int type = mEnemy.Ei[y][0];
//         if (type == 4)
//         {
//            printf("Level:%3d - blkwlk\n", le[x]);
//         }
//
//      }

//            mEnemy.Ei[y][20] = 16;


//            for (int i=3; i<10; i++)
               //mEnemy.Ei[y][i+10] = mEnemy.Ei[y][i];

//               mEnemy.Ei[y][i] = 0;

//            mEnemy.Ei[y][3] = mEnemy.Ef[y][0];
//            mEnemy.Ei[y][4] = mEnemy.Ef[y][1];

//            mEnemy.get_pod_extended_position(y, mEnemy.Ei[y][9], mEnemy.Ei[y][10]);

//            printf("Level:%3d - vinepod:%d\n", le[x], y);



//         if (type == 13)
//         {
//            count1++;
//            printf("Level:%3d - vinepod:%d\n", le[x], y);
//         }
//      }

//            {
//               int ex = mEnemy.Ef[y][0];
//               int ey = mEnemy.Ef[y][1];
//
//               int yinc = -20;
//               if (ey < 80) yinc = 20;
//
//
//               printf("Level:%3d - cloner:%d trigger type:%d \n", le[x], y, mEnemy.Ei[y][8]);
//
//               int i = mItem.get_empty_item(9);
//               if (i < 500)
//               {
//                  int ix = ex;
//                  int iy = ey + yinc;
//
//                  mItem.item[i][4] = ix;




////      if (le[x] == 6)
//      {
//
//
//         for (int y=0; y<100; y++)
//         {
//            int type = mEnemy.Ei[y][0];
//            if (type == 9)
//            {
//               int ex = mEnemy.Ef[y][0];
//               int ey = mEnemy.Ef[y][1];
//
//               int yinc = -20;
//               if (ey < 80) yinc = 20;
//
//
//               printf("Level:%3d - cloner:%d trigger type:%d \n", le[x], y, mEnemy.Ei[y][8]);
//
//               int i = mItem.get_empty_item(9);
//               if (i < 500)
//               {
//                  int ix = ex;
//                  int iy = ey + yinc;
//
//                  mItem.item[i][4] = ix;
//                  mItem.item[i][5] = iy;
//
//
//                  mItem.item[i][0] = 9;  // type 9 - trigger
//                  mItem.item[i][2] = 14; // draw color
//                  mItem.item[i][3] |= PM_ITEM_TRIGGER_PLAYER;
//                  mItem.item[i][3] |= PM_ITEM_TRIGGER_DRAW_ON;
//
//
//                  mItem.item[i][6] = mEnemy.Ei[y][11];
//                  mItem.item[i][7] = mEnemy.Ei[y][12];
//                  mItem.item[i][8] = mEnemy.Ei[y][13]+20;
//                  mItem.item[i][9] = mEnemy.Ei[y][14]+20;
//               }
//
//               i = mItem.get_empty_item(13);
//               if (i < 500)
//               {
//                  int ix = ex;
//                  int iy = ey + yinc*2;
//
//                  mItem.item[i][4] = ix;
//                  mItem.item[i][5] = iy;
//
//                  mItem.item[i][0] = 13;  // type 13 - timer
//
//                  mItem.item[i][10] = mEnemy.Ei[y][6];
//
//               }
//
//            }
//         }
//      }
//
//
//
//      for (int y=0; y<500; y++)
//      {
//         int it = mItem.item[y][0];
//
//         if ((it == 4) || (it == 9) || (it == 14) || (it == 16) || (it == 17))
//         {
//            if (mItem.item[y][6] < 0)    { mItem.item[y][6] = 0;     printf("l:%d it:%d x<0 \n", le[x], it );  }
//            if (mItem.item[y][7] < 0)    { mItem.item[y][7] = 0;     printf("l:%d it:%d y<0 \n", le[x], it );  }
//            if (mItem.item[y][8] > 2000) { mItem.item[y][8] = 2000;  printf("l:%d it:%d w<2000 \n", le[x], it );  }
//            if (mItem.item[y][9] > 2000) { mItem.item[y][9] = 2000;  printf("l:%d it:%d h<2000 \n", le[x], it );  }
//
//         }
//      }

//4 key
//9 trigger
//14 switch
//16 bm
//17 bd
//
//
////      if (le[x] < 100)
//         for (int y=0; y<500; y++)
//            if (mItem.item[y][0] == 12) // warp
//            {
//                printf("Level:%3d warp lev:%d  3:%d\n", le[x], mItem.item[y][8], mItem.item[y][3]);
//
//                mItem.item[y][0] = 0; // change to exit
////                mItem.item[y][1] = 1022; // change tile
////                mItem.item[y][2] = 1;
////                mItem.item[y][3] = 0;
////                mItem.item[y][8] = 100;
//
//
////
////                mItem.item[y][5] += 40; // move down 2 blocks
////
////                mItem.item[y][6] = mItem.item[y][8]; // change where lev is
////
//
//            }
//


//      for (int y=0; y<500; y++)
//         if (mItem.item[y][0] == 1) // door
//         {
////            if (mItem.item[y][13] == 448)  mItem.item[y][1] = 1;
////            if (mItem.item[y][13] == 1083) mItem.item[y][1] = 2;
////            if (mItem.item[y][13] == 448)  mItem.item[y][13] = 1;
////            if (mItem.item[y][13] == 1083) mItem.item[y][13] = 2;
//
//            mItem.item[y][13] = 0;
//
//         }

//            mItem.item[y][1] = 0;


//      for (int y=0; y<500; y++)
//         if (mItem.item[y][0] == 1) // door
//         {
//            count0++;
//            if (mItem.item[y][13] == 448) count1++;
//            if (mItem.item[y][13] == 1083) count2++;
//            if (mItem.item[y][13] == 0) count3++;
//
//
//
//            if ( (mItem.item[y][13] != 448) && (mItem.item[y][13] != 1083))// && (mItem.item[y][13] != 0) )
//                printf("Level:%3d 13:%d\n", le[x], mItem.item[y][13]);
//
//           //  mItem.item[y][6] = (float)mItem.item[y][6] * 2.5;
//         }

//
//      // block counter
//      for (int y=0; y<100; y++)
//         for (int z=0; z<100; z++)
//            blt[l[y][z] & 1023]++; // inc block counter

//      for (int l=0; l<NUM_LIFTS; l++)
//         if (lifts[l].active)
//         {
//         }
//


//      if (le[x] < 100)
//      for (int y=0; y<500; y++)
//         if (mItem.item[y][0] == 10)
//         {
//            count0++; // all
//            if ((mItem.item[y][2] & PM_ITEM_PMSG_SHOW_ALWAYS)) count1++;
//            else if (mItem.item[y][2] & PM_ITEM_PMSG_TRIGGER_BOX) count2++;
//            else count3++;
//         }

//      for (int y=0; y<500; y++)
//         if (mItem.item[y][0] == 10)
//         {
//            count0++; // all
//            if ((mItem.item[y][1] == 1036)) count1++;
//         }


//      for (int y=0; y<500; y++)
//         if (mItem.item[y][0] == 10)
//         {
//            if (mItem.item[y][2] & PM_ITEM_PMSG_TRIGGER_BOX)
//               printf("level:%3d y:%3d\n", le[x], y);
//         }



//      if (le[x] == 104)
//      for (int y=0; y<500; y++)
//         if (mItem.item[y][0] == 10)
//         {
//            if (!(mItem.item[y][2] & PM_ITEM_PMSG_SHOW_ALWAYS))
//               if (mItem.item[y][2] & PM_ITEM_PMSG_TRIGGER_BOX)
//               {
//                  printf("level:%3d y:%3d\n", le[x], y);
//
//                  // create a trigger item based on the trigger stuff in pmsg
//                  int i = mItem.get_empty_item(9);
//                  if (i < 500)
//                  {
//                     mItem.item[i][4] = mItem.item[y][4];
//                     mItem.item[i][5] = mItem.item[y][5]-20;
//                     if (mItem.item[i][5] < 0) mItem.item[1][5] = 20;
//
//                     mItem.item[i][0] = 9;  // type 9 - trigger
//                     mItem.item[i][3] |= PM_ITEM_TRIGGER_PLAYER;
//
//                     mItem.item[i][6] = mItem.item[y][6];
//                     mItem.item[i][7] = mItem.item[y][7];
//                     mItem.item[i][8] = mItem.item[y][8];
//                     mItem.item[i][9] = mItem.item[y][9];
//
//
//                     int ev = mTriggerEvent.get_unused_pm_event();
//
//                     mItem.item[i][11] = ev; // send always when on
//
//                     mItem.item[y][1] = ev;
//
//                     mItem.item[y][2] &= ~PM_ITEM_PMSG_TRIGGER_BOX; // clear flag
//
//                  }
//
//            }
//         }
//


//
//      for (int y=0; y<500; y++)
//         if (mItem.item[y][0] == 10)
//         {
//            int x1=0, y1=0, w=0, h=0;
//            mMiscFnx.get_int_3216(mItem.item[y][10], x1, y1);       // get x and y
//            mMiscFnx.get_int_3216(mItem.item[y][11], w, h);         // get width and height
//
//            mItem.item[y][6] = x1;
//            mItem.item[y][7] = y1;
//            mItem.item[y][8] = w;
//            mItem.item[y][9] = h;
//         }


//      for (int y=0; y<500; y++)
//         if (mItem.item[y][0] == 13)
//         {
//            int state, t1_mode, t2_mode, cnt;
//            mItem.get_timer_flags(mItem.item[y][3], state, t1_mode, t2_mode, cnt);
//            printf("l:%3d y:%3d  t1_mode:%d t2_mode:%d\n", le[x], y, t1_mode, t2_mode );
//
////            mItem.set_timer_flags(mItem.item[y][3], state, t1_mode, t2_mode, cnt);
//
//         }



//
//      for (int y=0; y<500; y++)
//         if (mItem.item[y][0] == 1) // door
//         {
//            int mt = mItem.item[y][7];  // move type
//            int dt = mItem.item[y][8];  // door type
//            int li = mItem.item[y][9];  // linked door item num
//
////            if (dt) count0++;
////            else count1++;
//
////            if ((dt) && (!mt)) count0++;
////
////
////            printf("l:%3d y:%3d  dt:%d mt:%d\n", le[x], y, dt, mt);
//
//            if ((dt) && (!mt)) // door type normal and auto move
//            {
//               printf("l:%3d y:%3d  li:%d\n", le[x], y, li);
//
//               if ((li<0) || (li > 499)) printf("error! - linked item (%d) out of range\n", li);
//               else if (mItem.item[li][0] != 1) printf("error! - linked item is not door (actual type:%d\n", mItem.item[li][0]);
//               else
//               {
//                  // is linked item stationary?
//
//                  if (mItem.item[li][3] == 0) mItem.item[y][7] = 2;  // force move
//                  else                        mItem.item[y][7] = 1;  // force instant
//
//               }
//            }
//         }
//
//
//







//
//
//
//      for (int y=0; y<100; y++)
//      {
//         int type = mEnemy.Ei[y][0];
//
//         if (type == 7)
//         {
//
//
//
//            if (mEnemy.Ei[y][20] & PM_ENEMY_VINEPOD_USE_SPLINE)
//            {
//
//               count1++;
//               printf("Level:%3d - Spline Vinepod:%d\n", le[x], y);
//
//
//            }
//            else
//            {
//               count0++;
//               printf("Level:%3d - Normal Vinepod:%d\n", le[x], y);
//
//
//
//            }
//
//
//
//
//
//
//         }
//
//      }
//

//
//            mEnemy.Ei[y][20] = 16;
//
//
//            for (int i=3; i<10; i++)
//               mEnemy.Ei[y][i+10] = mEnemy.Ei[y][i];
//
//               mEnemy.Ei[y][i] = 0;
//
//            mEnemy.Ei[y][3] = mEnemy.Ef[y][0];
//            mEnemy.Ei[y][4] = mEnemy.Ef[y][1];
//
//            mEnemy.get_pod_extended_position(y, mEnemy.Ei[y][9], mEnemy.Ei[y][10]);
//
//            printf("Level:%3d - vinepod:%d\n", le[x], y);
//
//
//
//         if (type == 13)
//         {
//            count1++;
//            printf("Level:%3d - vinepod:%d\n", le[x], y);
//         }
//      }
//
//
//



//      for (int y=0; y<500; y++)
//         if (mItem.item[y][0] == 19)
//            printf("l:%3d y:%3d  hider\n", le[x], y);







//
//      for (int l=0; l<NUM_LIFTS; l++) // iterate lifts
//         if (mLift.cur[l].active)
//            for (int s=0; s<mLift.cur[l].num_steps; s++) // iterate steps
//            {
//
//               int step_type = mLift.stp[l][s].type & 31; // step type
//               int step_val  = mLift.stp[l][s].val;       // step val
//
//               if ((step_type == 1) && (step_val > 20))
//                  printf("l:%3d s:%3d  \n", le[x], step_val);
//
//
//
//            }
//




      if (0)
      {
         mLevel.save_level(le[x]);
         al_set_target_backbuffer(mDisplay.display);
         al_draw_textf(mFont.pr8, mColor.pc[10], 110, 10+x*8, 0, "lev:%d", le[x]);
      }
   } // end of level iterate

   al_flip_display();

   printf("Total count0:%d \n",count0 );
   printf("Total count1:%d \n",count1 );
   printf("Total count2:%d \n",count2 );
   printf("Total count3:%d \n",count3 );
   printf("min:%d max:%d\n", min, max);

   //show_block_list(blt);
   mInput.tsw();
   mLevel.set_start_level(old_start_level);
}



























      /*

      //------------------------------------------------------------------------------------------------------------------
      //--------------check for bad enemy data ---------------------------------------------------------------------------
      //------------------------------------------------------------------------------------------------------------------
      for (int y=0; y<100; y++)
      {
         int good = 0;
         int type = mEnemy.Ei[y][0];
         if (type == 0)  good = 1; // this is a legal value
         if (type == 3)  good = 1;
         if (type == 4)  good = 1;
         if (type == 5)  good = 1;
         if (type == 6)  good = 1;
         if (type == 7)  good = 1;
         if (type == 8)  good = 1;
         if (type == 9)  good = 1;
         if (type == 10) good = 1;
         if (type == 11) good = 1;
         if (type == 12) good = 1;

         if (!good)
         {
            mEnemy.Ei[y][0] = 0; // erase
            printf("Level:%3d - Enemy:%d - bad type:%d   <----- will be erased!!!!!\n", le[x], y, type);
         }
            else if (type) // only check the rest if type valid
         {
            int shape = mEnemy.Ei[y][1];
            // min is flapper with 155
            // max is block walker 864

            if (type != 10) // don't check for 10, has custon draw
            {

               if ((shape < 147) || (shape > 1020))
                  printf("Level:%3d - Enemy:%2d - type%2d - bad shape%d\n", le[x], y, type, shape);

               float scale = mEnemy.Ef[y][12];
               if ((scale < 0.2) || (scale > 10))
                  printf("Level:%3d - Enemy:%2d - type%2d - bad scale%f\n", le[x], y, type, scale);

               float rot = mEnemy.Ef[y][14];
               if ((rot < -256) || (rot > 256))
                  printf("Level:%3d - Enemy:%2d - type%2d - bad rot%f\n", le[x], y, type, rot);
            }

            float xpos = mEnemy.Ef[y][0];
            if ((xpos < 20) || (xpos > 1980))
               printf("Level:%3d - Enemy:%2d - type%2d - bad xpos%f\n", le[x], y, type, xpos);

            float ypos = mEnemy.Ef[y][1];
            if ((ypos < 20) || (ypos > 1980))
               printf("Level:%3d - Enemy:%2d - type%2d - bad ypos%f\n", le[x], y, type, ypos);

         }
      }




      //------------------------------------------------------------------------------------------------------------------
      //--------------check for bad item data ---------------------------------------------------------------------------
      //------------------------------------------------------------------------------------------------------------------
      for (int c=0; c<500; c++)
      {
         int type = mItem.item[c][0];
         int good = 0;

         if (type)
         {
            if (type == 1)  good = 1;
            if (type == 2)  good = 1;
            if (type == 3)  good = 1;
            if (type == 4)  good = 1;
            if (type == 5)  good = 1;
            if (type == 6)  good = 1;
            if (type == 7)  good = 1;
            if (type == 8)  good = 1;
            if (type == 9)  good = 1;
            if (type == 10) good = 1;
            if (type == 11) good = 1;
            if (type == 12) good = 1;
            if (type == 14) good = 1;
            if (type == 15) good = 1;
            if (type == 16) good = 1;
            if (type == 17) good = 1;

            if (!good)
            {
               printf("Level:%3d - Item: %2d - bad type:%d   <----- will be erased!!!!!\n", le[x], c, type);
               mItem.item[c][0] = 0;
            }




            if (good)
            {
               if ((type != 9) && (type != 16) && (type != 17)) // don't check shape for these items...custom draw
               {
                  if ((mItem.item[c][1] < 169) || (mItem.item[c][1] > 1084))
                     printf("Level:%3d - Item: %2d - type:%2d - bad shape:%d\n", le[x], c, type, mItem.item[c][1]);
               }



               if ((type == 9) || (type == 16) || (type == 17)) // check field range values
               {
                  int x1 = mItem.item[c][6];
                  int y1 = mItem.item[c][7];
                  int x2 = mItem.item[c][6] + mItem.item[c][8];
                  int y2 = mItem.item[c][7] + mItem.item[c][9];

                  if ((x1 < 0) || (x1 > 1980)) printf("Level:%3d - Item: %2d - type:%2d - bad field x1 pos %d\n", le[x], c, type, x1);
                  if ((x2 < 0) || (x2 > 1980)) printf("Level:%3d - Item: %2d - type:%2d - bad field x2 pos %d\n", le[x], c, type, x2);
                  if ((y1 < 0) || (y1 > 1980)) printf("Level:%3d - Item: %2d - type:%2d - bad field y1 pos %d\n", le[x], c, type, x1);
                  if ((y2 < 0) || (y2 > 1980)) printf("Level:%3d - Item: %2d - type:%2d - bad field y2 pos %d\n", le[x], c, type, x2);



               }








               int xpos = mItem.item[c][4];
               if ((xpos < 0) || (xpos > 1980))
                   printf("Level:%3d - Item: %2d - type:%2d - bad xpos%d\n", le[x], c, type, xpos);

               int ypos = mItem.item[c][5];
               if ((ypos < 0) || (ypos > 1980))
                   printf("Level:%3d - Item: %2d - type:%2d - bad ypos%d\n", le[x], c, type, ypos);
            }
         }
      }


*/




/*
     if (le[x] == 61)
      for (int y=0; y<500; y++)
         if (mItem.item[y][0] == 1) // door only
            if (mItem.item[y][6] == 8) // color 8 only
               if (mItem.item[y][8] == 1) // linked only
               {
                    printf("lev%d door:%d move_type:%d\n", le[x], y, mItem.item[y][7] );

                    mItem.item[y][7] = 1;
               }

*/

/*

      for (int y=0; y<500; y++)
         if (mItem.item[y][0] == 14) // switch only
         {
             if (mItem.item[y][1] == 746) count0++;

             if (mItem.item[y][1] == 746) mItem.item[y][1] = 745;
         }


*/



/*

      for (int y=0; y<500; y++)
         if (mItem.item[y][0] == 2) // free man
         {
            //mItem.item[y][6] = 1;

            count0++;
            if (mItem.item[y][6] == 2) count1++;

//            mItem.item[y][0] = 2;
  //          mItem.item[y][6] = 2;

*/




/*            if (mItem.item[y][7] == 10) count1++;
            if (mItem.item[y][7] == 50) count2++;

            if ((mItem.item[y][7] != 10) && (mItem.item[y][7] != 50))
            {

               printf("Level:%3d - Item: %2d - type:%2d - bonus health:%d\n", le[x], y, 2, mItem.item[y][7]);

*/







/*
             if (mItem.item[y][7] != 0) count0++;



             if (mItem.item[y][0] == 11) count1++;
             if ((mItem.item[y][0] == 11) && (mItem.item[y][10] = 0)) count2++;

             if ((mItem.item[y][10] != 0) && (mItem.item[y][0] != 11))  count3++;

*/

//            mItem.item[y][10] = mItem.item[y][4]/20;
 //           mItem.item[y][11] = mItem.item[y][5]/20;













/*





      for (int y=0; y<500; y++)
         if (mItem.item[y][0] == 4) // keys only
         {
             count0++;


             int x1 = mItem.item[y][6]*20;

             int y1 = mItem.item[y][7]*20;

             int x2 = (mItem.item[y][8] - mItem.item[y][6]+1) * 20;
             int y2 = (mItem.item[y][9] - mItem.item[y][7]+1) * 20;



             mItem.item[y][6] = x1;
             mItem.item[y][7] = y1;
             mItem.item[y][8] = x2;
             mItem.item[y][9] = y2;

         }


*/









/*

         if (mItem.item[y][0] == 10) // msg only
         {

            count0++;


         }




*/

             /*

             if (mItem.item[y][0] == 11) count1++;
             if ((mItem.item[y][0] == 11) && (mItem.item[y][10] = 0)) count2++;

             if ((mItem.item[y][10] != 0) && (mItem.item[y][0] != 11))  count3++;



//            mItem.item[y][10] = mItem.item[y][4]/20;
 //           mItem.item[y][11] = mItem.item[y][5]/20;


 */








/*
      for (int y=0; y<500; y++)
         if (mItem.item[y][0] == 1) // door only
         {
            mItem.item[y][6] = mItem.item[y][14];
            mItem.item[y][14] = 0;
            mItem.item[y][15] = 0;
        }

*/

/*

            if (mItem.item[y][8] == 0) count1++; // old door
            if (mItem.item[y][8] == 1) count2++; // new door
            if (mItem.item[y][8] == 2) count3++; // exit only

            if (mItem.item[y][8] == 2) mItem.item[y][8] = 0;

           }

*/

/*
      for (int y=0; y<500; y++)
         if (mItem.item[y][0] == 1) // door only
         {
            count0++;
            if (mItem.item[y][13] == 448) count1++;
            if (mItem.item[y][13] == 1083) count2++;

            if ((mItem.item[y][13] != 1083) && (mItem.item[y][13] != 448))
               printf("lev%d\n", le[x]);

         }

*/






/*

            if (mItem.item[y][8] == 0)
            {
               // create a new item and link to it
               int found_empty_items = 0;
               int c = get_empty_item(); // get 1st item
               if (c != -1) found_empty_items = 1;

               if (found_empty_items)
               {
                  // new item
                  mItem.item[c][0] = 1;
                  mItem.item[c][1] = 1084; // ans shape
                  mItem.item[c][2] = 1; // draw mode
                  mItem.item[c][3] = 0; // stationary

                  mItem.item[c][4] = mItem.item[y][6]*20;
                  mItem.item[c][5] = mItem.item[y][7]*20;

                  mItem.item[c][8] = 2; // exit only
                  mItem.item[c][9] = 0; // link
                  mItem.item[c][11] = 0; // immediate
                  mItem.item[c][12] = 1; // draw lines always
                  mItem.item[c][13] = 1084; // ans shape


                  // this item
                  mItem.item[y][1] = 1083; // ans shape
                  mItem.item[y][2] = 1;    // draw mode
                  mItem.item[y][3] = 0;    // stat

                  mItem.item[y][6] = 0;
                  mItem.item[y][7] = 0;
                  mItem.item[y][8] = 1; // linked exit
                  mItem.item[y][9] = c; // link
                  mItem.item[y][11] = 0; // immediate
                  mItem.item[y][12] = 1; // draw lines always
                  mItem.item[y][13] = 1083; // base ans shape


//                  l[mItem.item[c][4]/20][mItem.item[c][5]/20] = 0;   // zero block


                 }
              }


    }


*/



/*


      // items


      for (y=0; y<500; y++)
         if (mItem.item[y][0] == 0) mItem.item[y][0] = 0;

*/


  /*

      // blocks
      for (int y=0; y<100; y++)
         for (int z=0; z<100; z++)
         {
            if (l[y][z] == 167)
            {
               count0++;
               printf("Lev:%3d\n",le[x]);
            }
         }
*/


/*

0-31   empty
32-63  semisold
64-95  bombable
96-127 breakable

>127 solid

18 ladder
19 rope


next most common are solid and will get:
#define PM_BTILE_SOLID_PLAYER      0b00000000000000010000000000000000
#define PM_BTILE_SOLID_ENEMY       0b00000000000000100000000000000000
#define PM_BTILE_SOLID_ITEM        0b00000000000001000000000000000000
#define PM_BTILE_SOLID_PBUL        0b00000000000010000000000000000000
#define PM_BTILE_SOLID_EBUL        0b00000000000100000000000000000000

then semisolid...add to solid
#define PM_BTILE_SEMISOLID_PLAYER  0b00000000001000000000000000000000
#define PM_BTILE_SEMISOLID_ENEMY   0b00000000010000000000000000000000
#define PM_BTILE_SEMISOLID_ITEM    0b00000000100000000000000000000000
#define PM_BTILE_SOLID_PLAYER      0b00000000000000010000000000000000
#define PM_BTILE_SOLID_ENEMY       0b00000000000000100000000000000000
#define PM_BTILE_SOLID_ITEM        0b00000000000001000000000000000000
#define PM_BTILE_SOLID_PBUL        0b00000000000010000000000000000000
#define PM_BTILE_SOLID_EBUL        0b00000000000100000000000000000000

#define PM_BTILE_BOMBABLE          0b00000001000000000000000000000000
#define PM_BTILE_SOLID_PLAYER      0b00000000000000010000000000000000
#define PM_BTILE_SOLID_ENEMY       0b00000000000000100000000000000000
#define PM_BTILE_SOLID_ITEM        0b00000000000001000000000000000000
#define PM_BTILE_SOLID_PBUL        0b00000000000010000000000000000000
#define PM_BTILE_SOLID_EBUL        0b00000000000100000000000000000000

#define PM_BTILE_BREAKABLE_PBUL    0b00000010000000000000000000000000
#define PM_BTILE_BREAKABLE_EBUL    0b00000100000000000000000000000000
#define PM_BTILE_SOLID_PLAYER      0b00000000000000010000000000000000
#define PM_BTILE_SOLID_ENEMY       0b00000000000000100000000000000000
#define PM_BTILE_SOLID_ITEM        0b00000000000001000000000000000000
#define PM_BTILE_SOLID_PBUL        0b00000000000010000000000000000000
#define PM_BTILE_SOLID_EBUL        0b00000000000100000000000000000000


#define PM_BTILE_LADDER_MOVE       0b00001000000000000000000000000000

#define PM_BTILE_ROPE_MOVE         0b00010000000000000000000000000000

  */

/*

      // blocks
      for (int y=0; y<100; y++)
         for (int z=0; z<100; z++)
         {
            int t = l[y][z] & 1023; // get tile only

            blt[t]++; // inc block counter


            if ((t > 639) && (t < 660))
            {
               printf("Lev:%3d\n" ,le[x]);

            }






         }


*/


            /*


            if (l[y][z] == 18) l[y][z] |= PM_BTILE_LADDER_MOVE;
            if (l[y][z] == 19) l[y][z] |= PM_BTILE_ROPE_MOVE;

            if ((l[y][z] > 31) && (l[y][z] < 64)) // semi solid
            {
               l[y][z] |= PM_BTILE_SOLID_PLAYER;
               l[y][z] |= PM_BTILE_SOLID_ENEMY;
               l[y][z] |= PM_BTILE_SOLID_ITEM;
               l[y][z] |= PM_BTILE_SEMISOLID_PLAYER;
               l[y][z] |= PM_BTILE_SEMISOLID_ENEMY;
               l[y][z] |= PM_BTILE_SEMISOLID_ITEM;
            }

            if ((l[y][z] > 63) && (l[y][z] < 96)) // bombable
            {
               l[y][z] |= PM_BTILE_SOLID_PLAYER;
               l[y][z] |= PM_BTILE_SOLID_ENEMY;
               l[y][z] |= PM_BTILE_SOLID_ITEM;
               l[y][z] |= PM_BTILE_SOLID_PBUL;
               l[y][z] |= PM_BTILE_SOLID_EBUL;
               l[y][z] |= PM_BTILE_BOMBABLE;
            }

            if ((l[y][z] > 95) && (l[y][z] < 128)) // breakable
            {
               l[y][z] |= PM_BTILE_SOLID_PLAYER;
               l[y][z] |= PM_BTILE_SOLID_ENEMY;
               l[y][z] |= PM_BTILE_SOLID_ITEM;
               l[y][z] |= PM_BTILE_SOLID_PBUL;
               l[y][z] |= PM_BTILE_SOLID_EBUL;
               l[y][z] |= PM_BTILE_BREAKABLE_PBUL;
               l[y][z] |= PM_BTILE_BREAKABLE_EBUL;
            }



            if ((l[y][z] > 63) && (l[y][z] < 1024)) // solid block
            {
               l[y][z] |= PM_BTILE_SOLID_PLAYER;
               l[y][z] |= PM_BTILE_SOLID_ENEMY;
               l[y][z] |= PM_BTILE_SOLID_ITEM;
               l[y][z] |= PM_BTILE_SOLID_PBUL;
               l[y][z] |= PM_BTILE_SOLID_EBUL;
            }






         }


         */



















            /*

            if (l[y][z] == 169) l[y][z] = 0;

            // replace blocks here
            if (l[y][z] == 109) l[y][z] = 990;
            if (l[y][z] == 76) l[y][z] = 657;
            if (l[y][z] == 119) l[y][z] = 625;
            if (l[y][z] == 135) l[y][z] = 592;
            if (l[y][z] == 149) l[y][z] = 624;
            if (l[y][z] == 74) l[y][z] = 656;
            if (l[y][z] == 118) l[y][z] = 620;
            if (l[y][z] == 147) l[y][z] = 621;
            if (l[y][z] == 114) l[y][z] = 622;
            if (l[y][z] == 146) l[y][z] = 623;

            if (l[y][z] == 115) l[y][z] = 616;
            if (l[y][z] == 140) l[y][z] = 584;
            if (l[y][z] == 116) l[y][z] = 617;
            if (l[y][z] == 137) l[y][z] = 585;
            if (l[y][z] == 117) l[y][z] = 618;
            if (l[y][z] == 141) l[y][z] = 586;
            if (l[y][z] == 180) l[y][z] = 619;
            if (l[y][z] == 138) l[y][z] = 587;
            if (l[y][z] == 170) l[y][z] = 588;
            if (l[y][z] == 203) l[y][z] = 589;
            if (l[y][z] == 142) l[y][z] = 590;
            if (l[y][z] == 202) l[y][z] = 591;


            if (l[y][z] == 148) l[y][z] = 612;
            if (l[y][z] == 181) l[y][z] = 614;
            if (l[y][z] == 139) l[y][z] = 580;

            if (l[y][z] == 136) l[y][z] = 582;
            if (l[y][z] == 201) l[y][z] = 582;

            if (l[y][z] == 73) l[y][z] = 644;
            if (l[y][z] == 75) l[y][z] = 645;

            if (l[y][z] == 171) l[y][z] = 576;
            if (l[y][z] == 173) l[y][z] = 577;
            if (l[y][z] == 172) l[y][z] = 578;
            if (l[y][z] == 174) l[y][z] = 579;

            if (l[y][z] == 150) l[y][z] = 608;
            if (l[y][z] == 151) l[y][z] = 609;
            if (l[y][z] == 182) l[y][z] = 610;
            if (l[y][z] == 183) l[y][z] = 611;

            if (l[y][z] == 41)  l[y][z] = 640;
            if (l[y][z] == 43)  l[y][z] = 641;
            if (l[y][z] == 105) l[y][z] = 642;
            if (l[y][z] == 107) l[y][z] = 643;


            if (l[y][z] == 32) l[y][z] = 128;
            if (l[y][z] == 33) l[y][z] = 129;
            if (l[y][z] == 34) l[y][z] = 130;
            if (l[y][z] == 35) l[y][z] = 135;
            if (l[y][z] == 36) l[y][z] = 136;
            if (l[y][z] == 37) l[y][z] = 137;
            if (l[y][z] == 38) l[y][z] = 160;
            if (l[y][z] == 39) l[y][z] = 167;

            if (l[y][z] == 48) l[y][z] = 138;
            if (l[y][z] == 49) l[y][z] = 139;
            if (l[y][z] == 50) l[y][z] = 140;
            if (l[y][z] == 51) l[y][z] = 141;
            if (l[y][z] == 52) l[y][z] = 142;
            if (l[y][z] == 53) l[y][z] = 143;
            if (l[y][z] == 54) l[y][z] = 160;

            if (l[y][z] == 14) l[y][z] = 164;

            if (l[y][z] == 8) l[y][z] = 32;
            if (l[y][z] == 9) l[y][z] = 33;
            if (l[y][z] == 10) l[y][z] = 34;
            if (l[y][z] == 11) l[y][z] = 35;

            if (l[y][z] == 16) l[y][z] = 64;

            if (l[y][z] == 24) l[y][z] = 96;
            if (l[y][z] == 25) l[y][z] = 97;
            if (l[y][z] == 26) l[y][z] = 98;
            if (l[y][z] == 30) l[y][z] = 99;
            if (l[y][z] == 31) l[y][z] = 100;

        if (l[y][z] == 42) l[y][z] = 646;
        if (l[y][z] == 106) l[y][z] = 647;


*/



/*


      for (int y=0; y<500; y++)
         if (mItem.item[y][0] == 16) // block manip
         {

            printf("Lev:%3d\n" ,le[x]);

            if (le[x] == 314)
            {
               int tn = mItem.item[y][10] & 1023; // get tile only
               tn |= sa[tn][0]; // apply default flags
               tn &= ~PM_BTILE_SHOW_SELECT_WIN; // remove select win flags
               mItem.item[y][10] = tn;

               tn = mItem.item[y][11] & 1023; // get tile only
               tn |= sa[tn][0]; // apply default flags
               tn &= ~PM_BTILE_SHOW_SELECT_WIN; // remove select win flags
               mItem.item[y][11] = tn;

            }
         }

*/







/*



int construct_lift(int l, char* lift_name, int width, int height, int color, int num_steps)
{
   strcpy(lifts[l].lift_name, lift_name);
   lifts[l].width = width;
   lifts[l].height = height;
   lifts[l].color = color;
   lifts[l].num_steps = num_steps;
   lifts[l].x1 = 0;
   lifts[l].y1 = 0;
   lifts[l].x2 = 0;
   lifts[l].y2 = 0;
   lifts[l].current_step = 0;
   lifts[l].limit_type = 0;
   lifts[l].limit_counter = 0;
   return 1;
}



*/








/*

      printf("-----------------------------------\n");
      printf("-----LEVEL %3d ---- %2d lifts-----\n", le[x], num_lifts);
      printf("-----------------------------------\n");

      for (int l=0; l<num_lifts; l++)
      {
         count0++;


         printf("lift:%-2d name:(%s) w:%-2d h:%-2d col:%-2d number of steps:%-2d    \n",
                      l, lifts[l].lift_name, lifts[l].width, lifts[l].height, lifts[l].color, lifts[l].num_steps);
         printf("-----------------------------------------------------------\n");

         for (int s=0; s<lifts[l].num_steps; s++)
         {
            count1++;

            int type = lift_steps[l][s].type & 31;
            int val = lift_steps[l][s].val;
            char typemsg[10] = {0};
            if ((type >0) && (type < 6)) sprintf(typemsg,"%s", lift_step_type_name[type] );


            if ((type < 1) || (type > 4))
            {
                count2++;
                printf("Lev:%3d lift:%-2d step%d type:%d\n",le[x], l, s, type);

             }

            int zx = lift_steps[l][s].x;
            if ((zx < 0) || (zx > 1999))
            {
                count2++;
                printf("Lev:%3d lift:%-2d step%d x:%d\n",le[x], l, s, zx);
             }

            int zy = lift_steps[l][s].y;
            if ((zy < 0) || (zy > 1999))
            {
                count2++;
                printf("Lev:%3d lift:%-2d step%d y:%d\n",le[x], l, s, zy);
             }


            int zv = val;
            if ((zv < 0) || (zv > 1999))
            {
                count2++;
                printf("Lev:%3d lift:%-2d step%d val:%d\n",le[x], l, s, zv);
             }
            printf("step:%-2d x:%-4d y:%-4d val:%-4d type:%d(%s)\n",
                         s, lift_steps[l][s].x, lift_steps[l][s].y, val, type, typemsg);
         }
         printf("------------------------------------------------------------\n");
      }

*/

/*


      for (int l=0; l<num_lifts; l++)
      {
         for (int s=0; s<lifts[l].num_steps; s++)
         {
            int type = lift_steps[l][s].type & 31;
            int val = lift_steps[l][s].val;

            if (type == 4)
            {
                count0++;
                if (val == 0) count1++;
                if (val == 20) lift_steps[l][s].val = 0; //count2++;

                printf("Lev:%3d lift:%-2d step%d val:%d\n",le[x], l, s, val);
             }
         }
      }

*/









/*
      // set flags for all lifts
      for (int l=0; l<num_lifts; l++)
      {
         lifts[l].flags |= PM_LIFT_SOLID_PLAYER;
         lifts[l].flags |= PM_LIFT_SOLID_ENEMY;
         lifts[l].flags |= PM_LIFT_SOLID_ITEM;
      }

*/


/*

      // put color and flags in all steps
      for (int l=0; l<num_lifts; l++)
      {
         int color = lifts[l].color;
         int cf = color << 28;
         for (int s=0; s<lifts[l].num_steps; s++)
         {
            lift_steps[l][s].type |= cf;

            lift_steps[l][s].type |= PM_LIFT_SOLID_PLAYER;
            lift_steps[l][s].type |= PM_LIFT_SOLID_ENEMY;
            lift_steps[l][s].type |= PM_LIFT_SOLID_ITEM;
         }
        // lifts[l].color = 0;
     }


      // remove color from lift...we will rely on steps only
      for (int l=0; l<num_lifts; l++)
      {
         lifts[l].color = 0;
      }



      // converting pmsg position from 100 to 2000 format
      for (int y=0; y<500; y++)
         if (mItem.item[y][0] == 10) // p msg
         {
            mItem.item[y][10] *= 20;
            mItem.item[y][11] *= 20;
         }

      // converting pod and cloner trigger from 100 x1 y1 x2 y2 to 2000 xywh format

      for (int y=0; y<100; y++)
         if ((mEnemy.Ei[y][0] == 7) || (mEnemy.Ei[y][0] == 9))
         {
            int w = mEnemy.Ei[y][13] - mEnemy.Ei[y][11];
            mEnemy.Ei[y][11] *= 20;
            mEnemy.Ei[y][13] = w*20;

            int h = mEnemy.Ei[y][14] - mEnemy.Ei[y][12];
            mEnemy.Ei[y][12] *= 20;
            mEnemy.Ei[y][14] = h*20;

         }



      // converting cloner source and dest from 100 xywh to 2000 xywh format
      for (int y=0; y<100; y++)
         if (mEnemy.Ei[y][0] == 9)
         {
            mEnemy.Ei[y][15] *= 20;
            mEnemy.Ei[y][16] *= 20;
            mEnemy.Ei[y][17] *= 20;
            mEnemy.Ei[y][18] *= 20;
            mEnemy.Ei[y][19] *= 20;
            mEnemy.Ei[y][20] *= 20;
         }

*/

//      printf("Lev:%3d \n",le[x]);


//      for (int y=0; y<100; y++)
//         if (mEnemy.Ei[y][0] == 10)
//            printf("Lev:%3d b:%d\n",le[x], y);

/*

      for (int y=0; y<100; y++)
         if (mEnemy.Ei[y][0] == 14) mEnemy.Ei[y][0] = 0;
*/






/*
      for (int y=0; y<500; y++)
         if (mItem.item[y][0] == 14) // switch
         {
            int t = mItem.item[y][1];

            for (int i=6; i<16; i++) mItem.item[y][i] = 0; // clear all ints from 6 up

            mItem.item[y][2] = 0;
            mItem.item[y][8] = 2000;
            mItem.item[y][9] = 2000;


//            printf("Lev:%3d t:%d\n" ,le[x], t);
//            if (t == 745) count0++;
//            if (t == 777) count1++;
//            if (t == 809) count2++;
//            if (t == 841) count3++;
//            if ((t != 745) && (t != 777) && (t != 809) && (t != 841)) count3++;


            if (t == 745) // green
            {
               mItem.item[y][1] = 97;
               mItem.item[y][10] = 1;
            }

            if (t == 777) // red
            {
               mItem.item[y][1] = 98;
               mItem.item[y][10] = 2;
            }

            if (t == 809) // blue
            {
               mItem.item[y][1] = 99;
               mItem.item[y][10] = 4;
            }

            if (t == 841) // purple
            {
               mItem.item[y][1] = 100;
               mItem.item[y][10] = 8;
            }
         }
*/
