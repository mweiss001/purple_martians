// mwEnemyCloner.cpp

#include "pm.h"
#include "mwEnemy.h"
#include "z_player.h"
#include "mwBitmap.h"
#include "mwShots.h"
#include "z_solid.h"
#include "mwLift.h"
#include "mwProgramState.h"
#include "mwItems.h"
#include "mwLevel.h"







//--9--cloner-----------------------------------------------------------------------------
//      Ei[e][5] = player in box
//      Ei[e][6] = create wait
//      Ei[e][7] = create wait counter
//      Ei[e][8] = trigger mode (0 = wait, 1=reset, 2=immed)
//      Ei[e][11] = trigger box x1
//      Ei[e][12] = trigger box y1
//      Ei[e][13] = trigger box x2
//      Ei[e][14] = trigger box y2
//      Ei[e][15] = copy box x
//      Ei[e][16] = copy box y
//      Ei[e][17] = dest box x
//      Ei[e][18] = dest box y
//      Ei[e][19] = copy box width
//      Ei[e][20] = copy box height

void mwEnemy::enemy_cloner(int e)
{
   int x4 = Ei[e][11] - 10; // trigger box
   int y4 = Ei[e][12] - 10;
   int x5 = Ei[e][11] + Ei[e][13] + 10;
   int y5 = Ei[e][12] + Ei[e][14] + 10;

   Ei[e][1] = mwB.zz[0][105]; // default shape

   if (Ei[e][31] && (Ei[e][30] == 0)) // hit and not invincible
   {
      enemy_killed(e);
      return;
   }
   else Ei[e][31] = 0;
   enemy_player_hit_proc(e);



   // set draw shape
   Ei[e][2] = 0;  // flip mode

//   int b = (Ei[e][7] * 7) / (Ei[e][6]+1);
//   Ei[e][1] = mwB.zz[5+b][106];

   int b = (Ei[e][7] * 9) / (Ei[e][6]+1);
   Ei[e][1] = mwB.zz[5+b][107];

   if (Ei[e][8] == 2) Ei[e][1] = mwB.zz[0][105]; // to make something happen for immed

//   printf("%d %d %d \n", b, Ei[e][7], Ei[e][6]);


   if (Ei[e][7] > Ei[e][6]) Ei[e][7] = Ei[e][6]; // ensure counter is never more than counter reset value


   int create_now = 0;

   int player_in_box = is_player_in_trigger_box(x4, y4, x5, y5);

   int player_just_entered_trigger_box = 0;
   if ((!Ei[e][5]) && (player_in_box)) player_just_entered_trigger_box = 1; // not in trig box last time and in box this time



   // player in box last time (Ei[e][5]
   // 0 = not in trigger box last time
   // 1 = in trigger box last time

   // Ei[][8]  trigger mode (0=wait, 1=reset, 2=immed)//

   // wait mode, player in box, run timer
   if ((Ei[e][8] == 0) && (player_in_box) && (--Ei[e][7] <= 0))
   {
      Ei[e][7] = Ei[e][6]; // reset counter
      create_now = 1;
   }

   // mode immed
   if ((Ei[e][8] == 2) && (player_just_entered_trigger_box)) create_now = 1;


   // mode reset timer
   if (Ei[e][8] == 1)
   {
      if (player_just_entered_trigger_box) Ei[e][7] = Ei[e][6];

      if (player_in_box)
      {
         if (--Ei[e][7] <= 0) // other wise run timer
         {
            Ei[e][7] = Ei[e][6]; // reset counter
            create_now = 1;
         }
      }
   }
   if (player_in_box) Ei[e][5] = 1; // for next time
   else Ei[e][5] = 0;
   if (create_now) cloner_create(e);
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
                        Ei[c][28] = 1000+e;   // tag with cloner item id
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
                        mItem.item[c][15] = 1000+e;   // tag with cloner item id
                        c = 500; // end loop
                        no++;    // one more object created
                     }
               }
            }
         }
   }
}












