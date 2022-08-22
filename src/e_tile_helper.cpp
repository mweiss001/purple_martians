// e_tile_helper.cpp
#include "pm.h"













void th_replace(int type)
{
   for (int a=0; a<100; a++)
      for (int b=0; b<100; b++)
         if (thl[a][b])
         {
            // choose which block to use

            // find block to left
            int bl_l = 0;
            int la = a-1;
            if (la > -1) bl_l = thl[la][b];

            // find block to right
            int bl_r = 0;
            int ra = a+1;
            if (ra < 100) bl_r = thl[ra][b];

            // find block above
            int bl_u = 0;
            int ab = b-1;
            if (ab > -1) bl_u = thl[a][ab];

            // find block below
            int bl_d = 0;
            int bb = b+1;
            if (bb < 100) bl_d = thl[a][bb];


            int t=0;
            int fb = -1; // default shape


            if (type == 1) // purple pipes with cross center
            {
               t = 576; // base shape

               fb = t+16; // cross center

               if ((bl_l == 0) && (bl_r == 1) && (bl_u == 0) && (bl_d == 1)) fb = t + 0;  // upper left corner
               if ((bl_l == 1) && (bl_r == 0) && (bl_u == 0) && (bl_d == 1)) fb = t + 1;  // upper right corner
               if ((bl_l == 0) && (bl_r == 1) && (bl_u == 1) && (bl_d == 0)) fb = t + 2;  // lower left corner
               if ((bl_l == 1) && (bl_r == 0) && (bl_u == 1) && (bl_d == 0)) fb = t + 3;  // lower right corner

               if ((bl_l == 0) && (bl_r == 1) && (bl_u == 1) && (bl_d == 1)) fb = t + 8;  // left vertical tee
               if ((bl_l == 1) && (bl_r == 0) && (bl_u == 1) && (bl_d == 1)) fb = t + 10; // right vertical tee
               if ((bl_l == 1) && (bl_r == 1) && (bl_u == 0) && (bl_d == 1)) fb = t + 9;  // upper horizontal tee
               if ((bl_l == 1) && (bl_r == 1) && (bl_u == 1) && (bl_d == 0)) fb = t + 11; // lower horizontal tee

               if ((bl_l == 0) && (bl_r == 0) && (bl_u == 1) && (bl_d == 1)) fb = t + 4;  // vertical through line
               if ((bl_l == 1) && (bl_r == 1) && (bl_u == 0) && (bl_d == 0)) fb = t + 6;  // horizontal through line

               if ((bl_l == 1) && (bl_r == 0) && (bl_u == 0) && (bl_d == 0)) fb = t + 12; // right end line
               if ((bl_l == 0) && (bl_r == 0) && (bl_u == 1) && (bl_d == 0)) fb = t + 13; // lower end line
               if ((bl_l == 0) && (bl_r == 1) && (bl_u == 0) && (bl_d == 0)) fb = t + 14; // left end line
               if ((bl_l == 0) && (bl_r == 0) && (bl_u == 0) && (bl_d == 1)) fb = t + 15; // upper end line
            }

            if (type == 2) // wires with filled cross solid center - works good
            {
               t = 608; // base shape
               fb = t + 16; // cross center


               if ((bl_l == 0) && (bl_r == 1) && (bl_u == 0) && (bl_d == 1)) fb = t + 0;  // upper left corner
               if ((bl_l == 1) && (bl_r == 0) && (bl_u == 0) && (bl_d == 1)) fb = t + 1;  // upper right corner
               if ((bl_l == 0) && (bl_r == 1) && (bl_u == 1) && (bl_d == 0)) fb = t + 2;  // lower left corner
               if ((bl_l == 1) && (bl_r == 0) && (bl_u == 1) && (bl_d == 0)) fb = t + 3;  // lower right corner

               if ((bl_l == 0) && (bl_r == 1) && (bl_u == 1) && (bl_d == 1)) fb = t + 8;  // left vertical tee
               if ((bl_l == 1) && (bl_r == 0) && (bl_u == 1) && (bl_d == 1)) fb = t + 10; // right vertical tee
               if ((bl_l == 1) && (bl_r == 1) && (bl_u == 0) && (bl_d == 1)) fb = t + 9;  // upper horizontal tee
               if ((bl_l == 1) && (bl_r == 1) && (bl_u == 1) && (bl_d == 0)) fb = t + 11; // lower horizontal tee

               if ((bl_l == 0) && (bl_r == 0) && (bl_u == 1) && (bl_d == 1)) fb = t + 4;  // vertical through line
               if ((bl_l == 1) && (bl_r == 1) && (bl_u == 0) && (bl_d == 0)) fb = t + 6;  // horizontal through line

               if ((bl_l == 1) && (bl_r == 0) && (bl_u == 0) && (bl_d == 0)) fb = t + 12; // right end line
               if ((bl_l == 0) && (bl_r == 0) && (bl_u == 1) && (bl_d == 0)) fb = t + 13; // lower end line
               if ((bl_l == 0) && (bl_r == 1) && (bl_u == 0) && (bl_d == 0)) fb = t + 14; // left end line
               if ((bl_l == 0) && (bl_r == 0) && (bl_u == 0) && (bl_d == 1)) fb = t + 15; // upper end line

            }

            if (type == 3) // grey bricks
            {
               t = 480; // base shape
               fb = t + 16; // cross center
               if ((bl_l == 0) && (bl_r == 1) && (bl_u == 0) && (bl_d == 1)) fb = t + 0;  // upper left corner
               if ((bl_l == 1) && (bl_r == 0) && (bl_u == 0) && (bl_d == 1)) fb = t + 1;  // upper right corner
               if ((bl_l == 0) && (bl_r == 1) && (bl_u == 1) && (bl_d == 0)) fb = t + 2;  // lower left corner
               if ((bl_l == 1) && (bl_r == 0) && (bl_u == 1) && (bl_d == 0)) fb = t + 3;  // lower right corner

               if ((bl_l == 0) && (bl_r == 1) && (bl_u == 1) && (bl_d == 1)) fb = t + 4;  // left vertical tee
               if ((bl_l == 1) && (bl_r == 0) && (bl_u == 1) && (bl_d == 1)) fb = t + 5;  // right vertical tee
               if ((bl_l == 1) && (bl_r == 1) && (bl_u == 0) && (bl_d == 1)) fb = t + 6;  // upper horizontal tee
               if ((bl_l == 1) && (bl_r == 1) && (bl_u == 1) && (bl_d == 0)) fb = t + 7;  // lower horizontal tee

               if ((bl_l == 0) && (bl_r == 0) && (bl_u == 1) && (bl_d == 1)) fb = t + 8;  // vertical through line
               if ((bl_l == 1) && (bl_r == 1) && (bl_u == 0) && (bl_d == 0)) fb = t + 9;  // horizontal through line

               if ((bl_l == 1) && (bl_r == 0) && (bl_u == 0) && (bl_d == 0)) fb = t + 12; // right end line
               if ((bl_l == 0) && (bl_r == 0) && (bl_u == 1) && (bl_d == 0)) fb = t + 13; // lower end line
               if ((bl_l == 0) && (bl_r == 1) && (bl_u == 0) && (bl_d == 0)) fb = t + 14; // left end line
               if ((bl_l == 0) && (bl_r == 0) && (bl_u == 0) && (bl_d == 1)) fb = t + 15; // upper end line
            }




















            if (fb == -1)
            {
               // do nothing
            }
            if (fb == 0)
            {
               set_block_with_flag_filters(a, b, fb);
            }

            if (fb > 0)
            {
               set_block_with_flag_filters(a, b, fb | sa[fb][0]);
            }
         }
   init_level_background();

}


//               // purple pipes with solid color center - really bad....
//
//               int fb = 528;
//
//               if ((bl_l == 0) && (bl_r == 1) && (bl_u == 0) && (bl_d == 1)) fb = 512; // upper left corner
//               if ((bl_l == 1) && (bl_r == 0) && (bl_u == 0) && (bl_d == 1)) fb = 513; // upper right corner
//               if ((bl_l == 0) && (bl_r == 1) && (bl_u == 1) && (bl_d == 0)) fb = 514; // lower left corner
//               if ((bl_l == 1) && (bl_r == 0) && (bl_u == 1) && (bl_d == 0)) fb = 515; // lower right corner
//
//               if ((bl_l == 0) && (bl_r == 1) && (bl_u == 1) && (bl_d == 1)) fb = 520; // left vertical tee
//               if ((bl_l == 1) && (bl_r == 0) && (bl_u == 1) && (bl_d == 1)) fb = 522; // right vertical tee
//               if ((bl_l == 1) && (bl_r == 1) && (bl_u == 0) && (bl_d == 1)) fb = 521; // upper horizontal tee
//               if ((bl_l == 1) && (bl_r == 1) && (bl_u == 1) && (bl_d == 0)) fb = 523; // lower horizontal tee
//
//               if ((bl_l == 0) && (bl_r == 0) && (bl_u == 1) && (bl_d == 1)) fb = 580; // vertical through line
//               if ((bl_l == 1) && (bl_r == 1) && (bl_u == 0) && (bl_d == 0)) fb = 582; // horizontal through line
//
//               if ((bl_l == 1) && (bl_r == 0) && (bl_u == 0) && (bl_d == 0)) fb = 588; // right end line
//               if ((bl_l == 0) && (bl_r == 0) && (bl_u == 1) && (bl_d == 0)) fb = 589; // lower end line
//               if ((bl_l == 0) && (bl_r == 1) && (bl_u == 0) && (bl_d == 0)) fb = 590; // left end line
//               if ((bl_l == 0) && (bl_r == 0) && (bl_u == 0) && (bl_d == 1)) fb = 591; // upper end line
//

/*

   // purple pipe with solid center
   fsd[1][0] = 512; // trigger blocks start
   fsd[1][1] = 528; // trigger block end
   fsd[1][9] = 528;  // default shape
   fsd[1][10] = 512; // upper left corner
   fsd[1][11] = 513; // upper right corner
   fsd[1][12] = 514; // lower left corner
   fsd[1][13] = 515; // lower right corner
   fsd[1][14] = 516; // left vertical through
   fsd[1][15] = 517; // right vertical through
   fsd[1][16] = 518; // upper horizontal through
   fsd[1][17] = 519; // lower horizontal through

*/








int th_draw_buttons(int x3, int x4, int yfb, int have_focus, int moving)
{
   int d = 1;
   if (have_focus) d = 0;
   if (moving) d = 1;
   int bts = 16;

   if ((mW[9].th_add_del == 1) && (mdw_buttont(x3, yfb, x4, bts, 0,0,0,0, 0,9,15,0, 1,0,1,d, "Add"))) mW[9].th_add_del = 0;
   if ((mW[9].th_add_del == 0) && (mdw_buttont(x3, yfb, x4, bts, 0,0,0,0, 0,9,15,0, 1,0,1,d, "Del"))) mW[9].th_add_del = 1;

   yfb+=bts/2; // spacing between groups

   if      ((mW[9].th_match == 0) && (mdw_buttont(x3, yfb, x4, bts, 0,0,0,0, 0,9,15,0, 1,0,1,d, "Single"))) mW[9].th_match = 1;
   else if ((mW[9].th_match == 1) && (mdw_buttont(x3, yfb, x4, bts, 0,0,0,0, 0,9,15,0, 1,0,1,d, "All"))) mW[9].th_match = 2;
   else if ((mW[9].th_match == 2) && (mdw_buttont(x3, yfb, x4, bts, 0,0,0,0, 0,9,15,0, 1,0,1,d, "Connecting"))) mW[9].th_match = 0;

   yfb+=bts/2; // spacing between groups

   if      ((mW[9].th_group == 0) && (mdw_buttont(x3, yfb, x4, bts, 0,0,0,0, 0,9,15,0, 1,0,1,d, "Specific Tile"))) mW[9].th_group = 1;
   else if ((mW[9].th_group == 1) && (mdw_buttont(x3, yfb, x4, bts, 0,0,0,0, 0,9,15,0, 1,0,1,d, "Tile Group"))) mW[9].th_group = 0;


   yfb+=bts/2; // spacing between groups


   if (mdw_buttont(x3, yfb, x4, bts, 0,0,0,0, 0,9,15,0, 1,0,1,d, "Purple Pipes with Cross Center")) th_replace(1);
   if (mdw_buttont(x3, yfb, x4, bts, 0,0,0,0, 0,9,15,0, 1,0,1,d, "Wires with Cross Center")) th_replace(2);
   if (mdw_buttont(x3, yfb, x4, bts, 0,0,0,0, 0,9,15,0, 1,0,1,d, "Grey Bricks")) th_replace(3);






   return yfb;
}







int th_compare_tile(int rb, int cb, int group)
{
   if (group == 0) if (rb == cb) return 1;

   if (group == 1)
   {
      // remove flags for comparison
      int r = rb & 1023;
      int c = cb & 1023;

      // even though group is selected, if we have an exact match just do it
      if (r == c) return 1;


      // grey brick with corners
      int sb = 480; // start block
      int eb = 496; // end block
      if ((r >= sb) && (r <= eb) && (c >= sb) && (c <= eb)) return 1;

      // purple pipe with open center
      sb = 576; // start block
      eb = 592; // end block
      if ((r >= sb) && (r <= eb) && (c >= sb) && (c <= eb)) return 1;

      // purple pipe with solid center
      sb = 512; // start block
      eb = 528; // end block
      if ((r >= sb) && (r <= eb) && (c >= sb) && (c <= eb)) return 1;

      // wires
      sb = 605; // start block
      eb = 624; // end block
      if ((r >= sb) && (r <= eb) && (c >= sb) && (c <= eb)) return 1;

      // brown brick with corners
      sb = 640; // start block
      eb = 656; // end block
      if ((r >= sb) && (r <= eb) && (c >= sb) && (c <= eb)) return 1;






   }


   return 0;
}





void th_find_connected(int x, int y, int group)
{
   int rb = l[x][y];  // block num to match
   thl[x][y] = -1;    // mark initial block pos in array

   int times=0, found=0;
   do
   {
      times++;
      found = 0;
      for (int a=0; a<100; a++)
         for (int b=0; b<100; b++)
            if (thl[a][b] == -1) // iterate already marked
            {
               if ((a > 0) && (th_compare_tile(rb, l[a-1][b], group))) // look left
               {
                  if (thl[a-1][b] != -1) found++; // found unmarked
                  thl[a-1][b] = -1; // mark it
               }

               if ((b > 0) && (th_compare_tile(rb, l[a][b-1], group))) // look up
               {
                  if (thl[a][b-1] != -1) found++; // found unmarked
                  thl[a][b-1] = -1; // mark it
               }
               if ((a < 99) && (th_compare_tile(rb, l[a+1][b], group))) // look right
               {
                  if (thl[a+1][b] != -1) found++; // found unmarked
                  thl[a+1][b] = -1; // mark it
               }
               if ((b < 99) && (th_compare_tile(rb, l[a][b+1], group))) // look down
               {
                  if (thl[a][b+1] != -1) found++; // found unmarked
                  thl[a][b+1] = -1; // mark it
               }
            }


   } while (found);


   // change all marked from -1 to add_del
   for (int a=0; a<100; a++)
      for (int b=0; b<100; b++)
         if (thl[a][b] == -1) thl[a][b] = mW[9].th_add_del;



}


void th_process_mouse(void)
{
   if (mouse_b1)
   {
      while (mouse_b1) proc_controllers();

      // add or del single tile at the specific location
      if (mW[9].th_match == 0) thl[gx][gy] = mW[9].th_add_del;

      // add or del only connected tiles that match
      if (mW[9].th_match == 2) th_find_connected(gx, gy, mW[9].th_group);

      // add or del all tiles that match
      if (mW[9].th_match == 1)
      {
         for (int x=0; x<100; x++)
            for (int y=0; y<100; y++)
               if (th_compare_tile(l[gx][gy], l[x][y], mW[9].th_group)) thl[x][y] = mW[9].th_add_del;
      }
   }

   if (mouse_b2)
   {
      while (mouse_b2) proc_controllers();
      set_windows(1);
   }
}

