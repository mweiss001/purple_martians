// mwWindowTileHelper.cpp

#include "mwBlockSets.h"
#include "pm.h"
#include "mwWindow.h"
#include "mwWindowManager.h"

#include "mwFont.h"
#include "mwWidget.h"
#include "mwColor.h"
#include "mwInput.h"
#include "mwEventQueue.h"
#include "mwLevel.h"
#include "mwMiscFnx.h"
#include "mwScreen.h"


// for rainbows only
int mwWindow::th_replace_helper3(int t, int bl_l, int bl_r, int bl_u, int bl_d, int bl_ul, int bl_ur, int bl_dl, int bl_dr)
{
   int fb = -1;

   if ((bl_l == 0) && (bl_r == 1) && (bl_u == 0) && (bl_d == 1)) fb = t + 0;  // upper left corner
   if ((bl_l == 1) && (bl_r == 0) && (bl_u == 0) && (bl_d == 1)) fb = t + 1;  // upper right corner
   if ((bl_l == 0) && (bl_r == 1) && (bl_u == 1) && (bl_d == 0)) fb = t + 2;  // lower left corner
   if ((bl_l == 1) && (bl_r == 0) && (bl_u == 1) && (bl_d == 0)) fb = t + 3;  // lower right corner

   if ((bl_l == 0) && (bl_r == 1) && (bl_u == 1) && (bl_d == 1)) fb = t + 4;  // left vertical tee
   if ((bl_l == 1) && (bl_r == 0) && (bl_u == 1) && (bl_d == 1)) fb = t + 5;  // right vertical tee
   if ((bl_l == 1) && (bl_r == 1) && (bl_u == 0) && (bl_d == 1)) fb = t + 6;  // upper horizontal tee
   if ((bl_l == 1) && (bl_r == 1) && (bl_u == 1) && (bl_d == 0)) fb = t + 7;  // lower horizontal tee

   if ((bl_l == 0) && (bl_r == 0) && (bl_u == 1) && (bl_d == 1)) fb = t + 4;  // vertical through line
   if ((bl_l == 1) && (bl_r == 1) && (bl_u == 0) && (bl_d == 0)) fb = t + 6;  // horizontal through line

   if ((bl_l == 1) && (bl_r == 0) && (bl_u == 0) && (bl_d == 0)) fb = t + 6; // right end line
   if ((bl_l == 0) && (bl_r == 0) && (bl_u == 1) && (bl_d == 0)) fb = t + 4; // lower end line
   if ((bl_l == 0) && (bl_r == 1) && (bl_u == 0) && (bl_d == 0)) fb = t + 6; // left end line
   if ((bl_l == 0) && (bl_r == 0) && (bl_u == 0) && (bl_d == 1)) fb = t + 4; // upper end line

   return fb;
}


// for all solid rect types and purple pipes cross center and wires cross center
int mwWindow::th_replace_helper(int t, int bl_l, int bl_r, int bl_u, int bl_d, int bl_ul, int bl_ur, int bl_dl, int bl_dr)
{
   int fb = t; //  default
   if ((bl_l == 0) && (bl_r == 1) && (bl_u == 0) && (bl_d == 1)) fb = t + 8;   // upper left corner
   if ((bl_l == 1) && (bl_r == 0) && (bl_u == 0) && (bl_d == 1)) fb = t + 9;   // upper right corner
   if ((bl_l == 0) && (bl_r == 1) && (bl_u == 1) && (bl_d == 0)) fb = t + 10;  // lower left corner
   if ((bl_l == 1) && (bl_r == 0) && (bl_u == 1) && (bl_d == 0)) fb = t + 11;  // lower right corner

   if ((bl_l == 0) && (bl_r == 1) && (bl_u == 1) && (bl_d == 1)) fb = t + 12;  // left vertical tee
   if ((bl_l == 1) && (bl_r == 0) && (bl_u == 1) && (bl_d == 1)) fb = t + 13;  // right vertical tee
   if ((bl_l == 1) && (bl_r == 1) && (bl_u == 0) && (bl_d == 1)) fb = t + 14;  // upper horizontal tee
   if ((bl_l == 1) && (bl_r == 1) && (bl_u == 1) && (bl_d == 0)) fb = t + 15;  // lower horizontal tee

   if ((bl_l == 0) && (bl_r == 0) && (bl_u == 1) && (bl_d == 1)) fb = t + 6;  // vertical through line
   if ((bl_l == 1) && (bl_r == 1) && (bl_u == 0) && (bl_d == 0)) fb = t + 3;  // horizontal through line

   if ((bl_l == 1) && (bl_r == 0) && (bl_u == 0) && (bl_d == 0)) fb = t + 4; // right end line
   if ((bl_l == 0) && (bl_r == 0) && (bl_u == 1) && (bl_d == 0)) fb = t + 7; // lower end line
   if ((bl_l == 0) && (bl_r == 1) && (bl_u == 0) && (bl_d == 0)) fb = t + 2; // left end line
   if ((bl_l == 0) && (bl_r == 0) && (bl_u == 0) && (bl_d == 1)) fb = t + 5; // upper end line


   if ((bl_ur == 0) && (bl_ul == 0) && (bl_dl == 0) && (bl_dr == 0)) // no diagonals
      if ((bl_l == 0) && (bl_r == 0) && (bl_u == 0) && (bl_d == 0)) // no blocks on all u d l r
         fb = t+1;  // orphan single block

   return fb;
}


void mwWindow::th_remove_bound()
{
   int l, r, u, d, ul, ur, dl, dr;

   for (int a=0; a<100; a++)
      for (int b=0; b<100; b++)
         if (mWM.thl[a][b])
         {
            th_find_adj(a, b, l, r, u, d, ul, ur, dl, dr);
            // if block is bound on all 8 sides in selection, remove from level
            if (ur && ul && dl && dr && l && r && u && d) mLevel.l[a][b] = 0;
         }

   // de-select if removed
   for (int a=0; a<100; a++)
      for (int b=0; b<100; b++)
         if (mWM.thl[a][b] && !mLevel.l[a][b]) mWM.thl[a][b] = 0;

   mScreen.init_level_background();
}

void mwWindow::th_find_adj(int a, int b, int &l, int &r, int &u, int &d, int &ul, int &ur, int &dl, int &dr )
{
   // find block to left
   l = 0;
   int la = a-1;
   if (la > -1) l = mWM.thl[la][b];

   // find block to right
   r = 0;
   int ra = a+1;
   if (ra < 100) r = mWM.thl[ra][b];

   // find block above
   u = 0;
   int ub = b-1;
   if (ub > -1) u = mWM.thl[a][ub];

   // find block below
   d = 0;
   int lb = b+1;
   if (lb < 100) d = mWM.thl[a][lb];

   // find block to upper left
   ul = 0;
   if ((la > -1) && (ub > -1)) ul = mWM.thl[la][ub];

   // find block to upper right
   ur = 0;
   if ((ra < 100) && (ub > -1)) ur = mWM.thl[ra][ub];

   // find block to lower left
   dl = 0;
   if ((la > -1) && (lb < 100)) dl = mWM.thl[la][lb];

   // find block to lower right
   dr = 0;
   if ((ra < 100) && (lb < 100)) dr = mWM.thl[ra][lb];
}



void mwWindow::th_replace(int type)
{
   int l, r, u, d, ul, ur, dl, dr;
   
   for (int a=0; a<100; a++)
      for (int b=0; b<100; b++)
         if (mWM.thl[a][b])
         {
            th_find_adj(a, b, l, r, u, d, ul, ur, dl, dr);

            int fb = -1; // default shape

            if (type == 1) fb = th_replace_helper(800, l, r, u, d, ul, ur, dl, dr); // purple pipes with cross center
            if (type == 2) fb = th_replace_helper(832, l, r, u, d, ul, ur, dl, dr); // wires with filled cross solid center - works good
            if (type == 3) fb = th_replace_helper(928, l, r, u, d, ul, ur, dl, dr); // grey bricks
            if (type == 5) fb = th_replace_helper(960, l, r, u, d, ul, ur, dl, dr); // brown bricks
            if (type == 7) fb = th_replace_helper(864, l, r, u, d, ul, ur, dl, dr); // brown and yellow thatch
            if (type == 8) fb = th_replace_helper(896, l, r, u, d, ul, ur, dl, dr); // white brain
            if (type == 9) fb = th_replace_helper(992, l, r, u, d, ul, ur, dl, dr); // blue solid

            if (type == 10) fb = th_replace_helper3(672, l, r, u, d, ul, ur, dl, dr); // rainbow
            if (type == 11) fb = th_replace_helper3(704, l, r, u, d, ul, ur, dl, dr); // rainbow 2


            int t=0;

            if (type == 4) // purple pipes with solid color center
            {
               t = 512;     // base shape
               fb = t + 16; // solid center
               if ((l == 0) && (r == 1) && (u == 0) && (d == 1)) fb = t + 0;  // upper left corner
               if ((l == 1) && (r == 0) && (u == 0) && (d == 1)) fb = t + 1;  // upper right corner
               if ((l == 0) && (r == 1) && (u == 1) && (d == 0)) fb = t + 2;  // lower left corner
               if ((l == 1) && (r == 0) && (u == 1) && (d == 0)) fb = t + 3;  // lower right corner

               // solid interior
               if ((l == 1) && (r == 1) && (u == 1) && (d == 1)) // blocks on all u d l r
               {
                  // single corner notch
                  if (ul == 0) fb = 524; // empty to ul
                  if (ur == 0) fb = 525; // empty to ur
                  if (dl == 0) fb = 526; // empty to dr
                  if (dr == 0) fb = 527; // empty to dl

                  // double corner notches
                  if ((ur == 0) && (dr == 0)) fb = 520; // empty to ur and dr
                  if ((dr == 0) && (dl == 0)) fb = 521; // empty to dr and dl
                  if ((dl == 0) && (ul == 0)) fb = 522; // empty to dl and ul
                  if ((ul == 0) && (ur == 0)) fb = 523; // empty to ul and ur

                  // triple corner notches
                  if ((ur == 0) && (ul == 1) && (dl == 0) && (dr == 0)) fb = 537; // only ul
                  if ((ur == 1) && (ul == 0) && (dl == 0) && (dr == 0)) fb = 538; // only ur
                  if ((ur == 0) && (ul == 0) && (dl == 0) && (dr == 1)) fb = 539; // only dr
                  if ((ur == 0) && (ul == 0) && (dl == 1) && (dr == 0)) fb = 540; // only dl

                  // all corner notches (full cross)
                  if ((ur == 0) && (ul == 0) && (dl == 0) && (dr == 0)) fb = 800; // no diagonals

               }

               if ((l == 0) && (r == 1) && (u == 1) && (d == 1)) // solid except for left
               {
                  if ((ur == 1) && (dr == 1)) fb = 516; // solid to ur and dr
                  if ((ur == 0) && (dr == 1)) fb = 529; // open to ur and solid to dr
                  if ((ur == 1) && (dr == 0)) fb = 530; // solid to ur and open to dr
                  if ((ur == 0) && (dr == 0)) fb = 812; // open to ur and dr
               }

               if ((l == 1) && (r == 0) && (u == 1) && (d == 1)) // solid except for right
               {
                  if ((ul == 1) && (dl == 1)) fb = 517; // solid to ul and dl
                  if ((ul == 0) && (dl == 1)) fb = 531; // open to ul and solid to dl
                  if ((ul == 1) && (dl == 0)) fb = 532; // solid to ul and open to dl
                  if ((ul == 0) && (dl == 0)) fb = 814; // open to ul and dl
               }

               if ((l == 1) && (r == 1) && (u == 0) && (d == 1)) // solid except for up
               {
                  if ((dl == 1) && (dr == 1)) fb = 518; // solid to dl and dr
                  if ((dl == 0) && (dr == 1)) fb = 533; // open to dl and solid to dr
                  if ((dl == 1) && (dr == 0)) fb = 534; // solid to dl and open to dr
                  if ((dl == 0) && (dr == 0)) fb = 813; // open to dl and dr
               }

               if ((l == 1) && (r == 1) && (u == 1) && (d == 0)) // solid except for down
               {
                  if ((ul == 1) && (ur == 1)) fb = 519; // solid to ul and ur
                  if ((ul == 0) && (ur == 1)) fb = 535; // open to ul and solid to ur
                  if ((ul == 1) && (ur == 0)) fb = 536; // solid to ul and open to ur
                  if ((ul == 0) && (ur == 0)) fb = 815; // open to ul and ur
               }

               if ((ur == 0) && (ul == 0) && (dl == 0) && (dr == 0)) // no diagonals
                  if ((l == 0) && (r == 0) && (u == 0) && (d == 0)) // no blocks on all u d l r
                     fb = 541;  // orphan single block

               // single block line corners
               if ((l == 0) && (r == 1) && (u == 0) && (d == 1) && (dr == 0)) fb = 808;  // upper left corner
               if ((l == 1) && (r == 0) && (u == 0) && (d == 1) && (dl == 0)) fb = 809;  // upper right corner
               if ((l == 0) && (r == 1) && (u == 1) && (d == 0) && (ur == 0)) fb = 810;  // lower left corner
               if ((l == 1) && (r == 0) && (u == 1) && (d == 0) && (ul == 0)) fb = 811;  // lower right corner

               if ((l == 0) && (r == 0) && (u == 1) && (d == 1)) fb = 806; // vertical through line
               if ((l == 1) && (r == 1) && (u == 0) && (d == 0)) fb = 803; // horizontal through line

               if ((l == 1) && (r == 0) && (u == 0) && (d == 0)) fb = 804; // right end line
               if ((l == 0) && (r == 0) && (u == 1) && (d == 0)) fb = 807; // lower end line
               if ((l == 0) && (r == 1) && (u == 0) && (d == 0)) fb = 802; // left end line
               if ((l == 0) && (r == 0) && (u == 0) && (d == 1)) fb = 805; // upper end line
            }




            if (fb == -1)
            {
               // do nothing
            }
            if (fb == 0)
            {
               mLevel.l[a][b] = 0;
            }

            if (fb > 0)
            {
               // change only tile portion (lower 10 bits)
               mLevel.l[a][b] &= 0b11111111111111110000000000000000; // clear lower bits
               mLevel.l[a][b] |= fb; // merge tile number
            }
         }
   mScreen.init_level_background();

}

int mwWindow::th_draw_buttons(int x3, int x4, int yfb, int d)
{
   int bts = 18;
   x3+=4;
   x4-=4;

   // background, title and frame for this group of buttons
   int c1 = 12; // frame color
   int y2f = yfb + bts*6 + 16; // get y2
   if (mWM.mW[9].th_match == 0) y2f -= (bts+4);
   al_draw_filled_rectangle(x3-1, yfb, x4+1, y2f-1, mColor.pc[c1+208]); // background
   mMiscFnx.titlex("Modify Which Tiles Are Marked", 15, c1, x3-1, x4+1, yfb); // title
   al_draw_rectangle(x3-1, yfb, x4+1, y2f-1, mColor.pc[c1], 1); // frame
   yfb +=16;

   int marked_count = 0;
   for (int a=0; a<100; a++)
      for (int b=0; b<100; b++)
        if (mWM.thl[a][b]) marked_count++;

   int xc = x3 + (x4-x3)/2 + 40; // split between buttons
   char msg[80];
   sprintf(msg, "Number of Marks:%d", marked_count);
   mWidget.buttont(x3+2, yfb, xc-2, bts, 0,0,0,0, 0,8,15,0, 1,0,0,d, msg);

   if (mWidget.buttont(xc+2, yfb, x4-2, bts, 0,0,0,0, 0,14,15,0, 1,0,1,d, "Clear All"))
   {
      for (int a=0; a<100; a++)
         for (int b=0; b<100; b++) mWM.thl[a][b] = 0;
   }

   yfb+=4;

   int tx1 = x3+6;
   sprintf(msg, "Action: ");
   al_draw_text(mFont.pr8, mColor.pc[color], tx1, yfb+5, 0, msg);
   int tx2 = tx1 + strlen(msg)*8; // x pos past text
   sprintf(msg, "Remove Marks");
   if (mWM.mW[9].th_add_del) sprintf(msg, "Add Marks");
   int bf = (strlen(msg)+2)*8; // size button based on text length
   if (mWidget.buttont(tx2, yfb, tx2+bf, bts, 0,0,0,0, 0,9,15,0, 1,0,1,d, msg)) mWM.mW[9].th_add_del = !mWM.mW[9].th_add_del;

   yfb+=4;

   sprintf(msg, "Filter: ");
   al_draw_text(mFont.pr8, mColor.pc[color], tx1, yfb+5, 0, msg);
   tx2 = tx1 + strlen(msg)*8; // x pos past text
   if (mWM.mW[9].th_match == 0) sprintf(msg, "Single Tile");
   if (mWM.mW[9].th_match == 1) sprintf(msg, "All Matching Tiles");
   if (mWM.mW[9].th_match == 2) sprintf(msg, "Connected Matching Tiles");
   bf = (strlen(msg)+2)*8; // size button based on text length
   if (mWidget.buttont(tx2, yfb, tx2+bf, bts, 0,0,0,0, 0,7,15,0, 1,0,1,d, msg)) mWM.mW[9].th_match++;
   if (mWM.mW[9].th_match > 2) mWM.mW[9].th_match = 0;


   if (mWM.mW[9].th_match)
   {
      yfb+=4;
      sprintf(msg, "Match: ");
      al_draw_text(mFont.pr8, mColor.pc[color], tx1, yfb+5, 0, msg);
      tx2 = tx1 + strlen(msg)*8; // x pos past text

      sprintf(msg, "Specific Tile Only");
      if (mWM.mW[9].th_group) sprintf(msg, "All Tiles In Set");
      bf = (strlen(msg)+2)*8; // size button based on text length
      if (mWidget.buttont(tx2, yfb, tx2+bf, bts, 0,0,0,0, 0,7,15,0, 1,0,1,d, msg)) mWM.mW[9].th_group = !mWM.mW[9].th_group;
   }

   yfb+=4;
   if (mWidget.buttont(x3+2, yfb, x4-2, bts, 0,0,0,0, 0,12,15,0, 1,0,1,d, "Delete Bound Tiles in Selection")) th_remove_bound();


   yfb+=bts/2; // spacing between groups

   // background, title and frame for this group of buttons
   c1 = 10; // color
   bts = 24;
   int bsp = 4; // spacing in between buttons
   y2f = yfb + bts*7 + bsp*7 + 18;

   al_draw_filled_rectangle(x3-1, yfb, x4+1, y2f-1, mColor.pc[c1+192]); // background color
   mMiscFnx.titlex("Change Marked Tiles To Tileset", 15, c1, x3-1, x4+1, yfb); // title
   al_draw_rectangle(x3-1, yfb, x4+1, y2f-1, mColor.pc[c1], 1); // frame

   yfb+=16;
   x3+=2;
   x4-=2;

   int c = 10 + 64;
   int choice = 0;
   yfb+=bsp;
   if (mWidget.buttontt(x3, yfb, x4, bts, 800,14,0,0, 0,c,15,0, 1,1,1,d, "     Purple Pipes with Cross Center")) choice = 1;
   yfb+=bsp;
   if (mWidget.buttontt(x3, yfb, x4, bts, 512,14,0,0, 0,c,15,0, 1,1,1,d, "     Purple Pipes with Solid Center")) choice = 4;
   yfb+=bsp;
   if (mWidget.buttontt(x3, yfb, x4, bts, 832,14,0,0, 0,c,15,0, 1,1,1,d, "     Wires with Cross Center")) choice = 2;
   yfb+=bsp;
   if (mWidget.buttontt(x3, yfb, x4, bts, 928,14,0,0, 0,c,15,0, 1,1,1,d, "     Grey Bricks")) choice = 3;
   yfb+=bsp;
   if (mWidget.buttontt(x3, yfb, x4, bts, 960,14,0,0, 0,c,15,0, 1,1,1,d, "     Brown Bricks")) choice = 5;
   yfb+=bsp;
   if (mWidget.buttontt(x3, yfb, x4, bts, 864,14,0,0, 0,c,15,0, 1,1,1,d, "     Brown and Yellow Thatch")) choice = 7;
   yfb+=bsp;
   if (mWidget.buttontt(x3, yfb, x4, bts, 896,14,0,0, 0,c,15,0, 1,1,1,d, "     Brain")) choice = 8;
   yfb+=bsp;
   if (mWidget.buttontt(x3, yfb, x4, bts, 992,14,0,0, 0,c,15,0, 1,1,1,d, "     Blue")) choice = 9;
   yfb+=bsp;
   if (mWidget.buttontt(x3, yfb, x4, bts, 672,14,0,0, 0,c,15,0, 1,1,1,d, "     Rainbow")) choice = 10;
   yfb+=bsp;
   if (mWidget.buttontt(x3, yfb, x4, bts, 704,14,0,0, 0,c,15,0, 1,1,1,d, "     Rainbow 2")) choice = 11;


   if (choice) th_replace(choice);
   return yfb;
}

void mwWindow::th_find_connected(int x, int y, int group)
{
   int rb = mLevel.l[x][y];  // block num to match
   mWM.thl[x][y] = -1;       // mark initial block pos in array

   int times=0, found=0;
   do
   {
      times++;
      found = 0;
      for (int a=0; a<100; a++)
         for (int b=0; b<100; b++)
            if (mWM.thl[a][b] == -1) // iterate already marked
            {
               if ((a > 0) && (mBlockSets.compareBlock(rb, mLevel.l[a-1][b], group))) // look left
               {
                  if (mWM.thl[a-1][b] != -1) found++; // found unmarked
                  mWM.thl[a-1][b] = -1; // mark it
               }
               if ((b > 0) && (mBlockSets.compareBlock(rb, mLevel.l[a][b-1], group))) // look up
               {
                  if (mWM.thl[a][b-1] != -1) found++; // found unmarked
                  mWM.thl[a][b-1] = -1; // mark it
               }
               if ((a < 99) && (mBlockSets.compareBlock(rb, mLevel.l[a+1][b], group))) // look right
               {
                  if (mWM.thl[a+1][b] != -1) found++; // found unmarked
                  mWM.thl[a+1][b] = -1; // mark it
               }
               if ((b < 99) && (mBlockSets.compareBlock(rb, mLevel.l[a][b+1], group))) // look down
               {
                  if (mWM.thl[a][b+1] != -1) found++; // found unmarked
                  mWM.thl[a][b+1] = -1; // mark it
               }
            }
   } while (found);

   // change all marked from -1 to add_del
   for (int a=0; a<100; a++)
      for (int b=0; b<100; b++)
         if (mWM.thl[a][b] == -1) mWM.thl[a][b] = mWM.mW[9].th_add_del;
}


void mwWindow::th_process_mouse(void)
{
   if (mInput.mouse_b[1][0])
   {
      while (mInput.mouse_b[1][0]) mEventQueue.proc(1);

      // add or del single tile at the specific location
      if (mWM.mW[9].th_match == 0) mWM.thl[mWM.gx][mWM.gy] = mWM.mW[9].th_add_del;

      // add or del only connected tiles that match
      if (mWM.mW[9].th_match == 2) th_find_connected(mWM.gx, mWM.gy, mWM.mW[9].th_group);

      // add or del all tiles that match
      if (mWM.mW[9].th_match == 1)
      {
         for (int x=0; x<100; x++)
            for (int y=0; y<100; y++)
               if (mBlockSets.compareBlock(mLevel.l[mWM.gx][mWM.gy], mLevel.l[x][y], mWM.mW[9].th_group)) mWM.thl[x][y] = mWM.mW[9].th_add_del;
      }
   }

   if (mInput.mouse_b[2][0])
   {
      while (mInput.mouse_b[2][0]) mEventQueue.proc(1);
      mWM.set_windows(1);
   }
}
