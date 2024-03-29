// mwEnemyCloner.cpp

#include "pm.h"
#include "mwEnemy.h"
#include "mwPlayer.h"
#include "mwBitmap.h"
#include "mwShot.h"
#include "mwSolid.h"
#include "mwLift.h"
#include "mwLoop.h"
#include "mwItem.h"
#include "mwLevel.h"
#include "mwMiscFnx.h"
#include "mwTriggerEvent.h"


//--9--cloner-----------------------------------------------------------------------------
//   Ei[][4]  draw boxes (0 = none) (1 = source) (2 = dest) (3 = both)
//   Ei[][5]  draw mode (0 = hidden, 1 = static shape, 2 = static ans, 3 = follow event timer
//   Ei[][8]  trigger event
//   Ei[][9]  time to live for created objects
//   Ei[][10] max num of created objects
//   Ei[e][15] = copy box x
//   Ei[e][16] = copy box y
//   Ei[e][17] = dest box x
//   Ei[e][18] = dest box y
//   Ei[e][19] = copy box width
//   Ei[e][20] = copy box height

void mwEnemy::move_cloner(int e)
{
   if (Ei[e][31] && (Ei[e][30] == 0)) // hit and not invincible
   {
      enemy_killed(e);
      return;
   }
   else Ei[e][31] = 0;
   enemy_player_hit_proc(e);
   if ((Ei[e][8]) && (mTriggerEvent.event[Ei[e][8]])) cloner_create(e);
}

void mwEnemy::cloner_create(int e)
{
   float x1 = Ei[e][15]-2;    // source
   float y1 = Ei[e][16]-2;
   float x3 = Ei[e][17]-2;    // destination
   float y3 = Ei[e][18]-2;
   float  w = Ei[e][19];     // width
   float  h = Ei[e][20];     // height
   float x2 = x1 + w;
   float y2 = y1 + h;

   int no=0, cl=Ei[e][10]; // limit on number of created objects
   if (cl)
   {
      // count number of active objects tagged by this cloner
      for (int x=0; x<100; x++)
         if ((Ei[x][0]) && (Ei[x][28] == 1000 + e)) no++;
      for (int x=0; x<500; x++)
         if ((mItem.item[x][0]) && (mItem.item[x][15] == 1000 + e)) no++;
   }
   else cl = 600; // no limit is same as max limit
   if (no < cl) // if number of objects < create limit
   {
      // check for enemies in box
      for (int b=0; b<100; b++)
         if (Ei[b][0])
            if ((Ef[b][0] > x1) && (Ef[b][0] < x2) && (Ef[b][1] > y1) && (Ef[b][1] < y2) && (no < cl))
            {
               // check if new position is empty
               float new_x_pos = Ef[b][0] + x3 - x1;
               float new_y_pos = Ef[b][1] + y3 - y1;
               int nx = new_x_pos / 20;
               int ny = new_y_pos / 20;
               if (mLevel.is_block_empty(nx, ny, 1, 0, 0)) // block only
               {
                  for (int c=0; c<100; c++) // look for a place to put it
                     if (Ei[c][0] == 0)  // found empty
                     {
                        for (int y=0; y<32; y++) Ei[c][y] = Ei[b][y];
                        for (int y=0; y<16; y++) Ef[c][y] = Ef[b][y];
                        Ef[c][0] = new_x_pos;
                        Ef[c][1] = new_y_pos;
                        Ei[c][27] = Ei[e][9]; // set time to live
                        Ei[c][28] = 1000+e;   // tag with cloner id
                        c = 100; // end loop
                        no++;    // one more object created
                     }
               }
            }

      // check for items in box
      for (int b=0; b<500; b++)
         if (mItem.item[b][0])
         {
            float ix = mItem.itemf[b][0];
            float iy = mItem.itemf[b][1];
            if ((ix > x1) && (ix < x2) && (iy > y1) && (iy < y2) && (no < cl))
            {
               // check if new position is empty
               float new_x_pos = ix + x3 - x1;
               float new_y_pos = iy + y3 - y1;
               int nx = new_x_pos / 20;
               int ny = new_y_pos / 20;
               if (mLevel.is_block_empty(nx, ny, 1, 0, 0)) // block only
               {
                  for (int c=0; c<500; c++)
                     if (mItem.item[c][0] == 0) // found empty
                     {
                        for (int y=0; y<16; y++) mItem.item[c][y] = mItem.item[b][y];
                        mItem.itemf[c][0]= new_x_pos;
                        mItem.itemf[c][1]= new_y_pos;
                        mItem.itemf[c][2]= 0;
                        mItem.itemf[c][3]= 0;

                        // are we copying something that already has an expiry date?? if so leave it
                        if (mItem.item[b][14] == 0) mItem.item[c][14] = Ei[e][9]; // otherwise set time to live from cloner
                        if (mItem.item[c][0] == 10) strcpy(mItem.pmsgtext[c], mItem.pmsgtext[b]); // message
                        mItem.item[c][15] = 1000+e;   // tag with cloner id
                        c = 500; // end loop
                        no++;    // one more object created
                     }
               }
            }
         }
   }
}





void mwEnemy::draw_cloner(int e, int x, int c, int custom)
{
   // draw main shape
   if ((custom) || (mLoop.level_editor_running))
   {
      Ei[e][1] = mBitmap.zz[7][105]; // static shape
   }
   else
   {
      int draw_mode = Ei[e][5];

      if (draw_mode == 0) Ei[e][1] = 0;                  // hidden
      if (draw_mode == 1) Ei[e][1] = mBitmap.zz[7][105]; // static shape
      if (draw_mode == 2) Ei[e][1] = mBitmap.zz[0][105]; // static animation sequence
      if (draw_mode == 3)                                // follow timer event
      {
         if (Ei[e][8])
         {
            float rtio = mItem.get_timer_ratio_for_event(Ei[e][8]);
            //printf("ratio:%f\n", rtio);
            float ns = mBitmap.zz[4][107]; // number of shapes
            float ni = ns * rtio;
            Ei[e][1] = mBitmap.zz[5+(int)ni][107];
         }
         else Ei[e][1] = mBitmap.zz[6][105]; // static shape
      }
   }

   // draw boxes
   if (!custom)
   {
      int q = Ei[e][4]; // show box mode (0=none) (1 = src only) (2 = dst only) (3 = both)
      if ((q == 1) || (q == 3)) mMiscFnx.rectangle_with_diagonal_lines(Ei[e][15], Ei[e][16], Ei[e][15] + Ei[e][19], Ei[e][16] + Ei[e][20], 8, 11+128, 11+192, 0); // source
      if ((q == 2) || (q == 3)) mMiscFnx.rectangle_with_diagonal_lines(Ei[e][17], Ei[e][18], Ei[e][17] + Ei[e][19], Ei[e][18] + Ei[e][20], 8, 10+128, 10+192, 0); // destination
   }
}



