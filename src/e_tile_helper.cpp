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

            // find block to upper left
            int bl_ul = 0;
            if ((la > -1) && (ab > -1)) bl_ul = thl[la][ab];

            // find block to upper right
            int bl_ur = 0;
            if ((ra < 100) && (ab > -1)) bl_ur = thl[ra][ab];

            // find block to lower left
            int bl_dl = 0;
            if ((la > -1) && (bb < 100)) bl_dl = thl[la][bb];

            // find block to lower right
            int bl_dr = 0;
            if ((ra < 100) && (bb < 100)) bl_dr = thl[ra][bb];


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

               if ((bl_ur == 0) && (bl_ul == 0) && (bl_dl == 0) && (bl_dr == 0)) // no diagonals
                  if ((bl_l == 0) && (bl_r == 0) && (bl_u == 0) && (bl_d == 0)) // no blocks on all u d l r
                     fb = 626;  // orphan single block
            }






















            if (type == 3) // grey bricks
            {
               t = 480; // base shape
               fb = t+16; // cross center
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


               if ((bl_ur == 0) && (bl_ul == 0) && (bl_dl == 0) && (bl_dr == 0)) // no diagonals
                  if ((bl_l == 0) && (bl_r == 0) && (bl_u == 0) && (bl_d == 0)) // no blocks on all u d l r
                     fb = t+18;  // orphan single block
            }

            if (type == 5) // brown bricks
            {
               t = 448; // base shape
               fb = t+16; // cross center
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


               if ((bl_ur == 0) && (bl_ul == 0) && (bl_dl == 0) && (bl_dr == 0)) // no diagonals
                  if ((bl_l == 0) && (bl_r == 0) && (bl_u == 0) && (bl_d == 0)) // no blocks on all u d l r
                     fb = t+18;  // orphan single block

            }


            if (type == 7) // brown and yellow thatch
            {
               t = 416; // base shape
               fb = t+16; // cross center
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

               if ((bl_ur == 0) && (bl_ul == 0) && (bl_dl == 0) && (bl_dr == 0)) // no diagonals
                  if ((bl_l == 0) && (bl_r == 0) && (bl_u == 0) && (bl_d == 0)) // no blocks on all u d l r
                     fb = t+18;  // orphan single block
            }


            if (type == 8) // brain
            {
               t = 384; // base shape
               fb = t+16; // cross center
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

               if ((bl_ur == 0) && (bl_ul == 0) && (bl_dl == 0) && (bl_dr == 0)) // no diagonals
                  if ((bl_l == 0) && (bl_r == 0) && (bl_u == 0) && (bl_d == 0)) // no blocks on all u d l r
                     fb = t+18;  // orphan single block
            }

            if (type == 6) // open purple pipes - not working
            {
               t = 448; // base shape
               fb = t+16; // cross center
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


               if ((bl_ur == 0) && (bl_ul == 0) && (bl_dl == 0) && (bl_dr == 0)) // no diagonals
                  if ((bl_l == 0) && (bl_r == 0) && (bl_u == 0) && (bl_d == 0)) // no blocks on all u d l r
                     fb = t+18;  // orphan single block

            }









            if (type == 4) // purple pipes with solid color center
            {
               t = 512;     // base shape
               fb = t + 16; // solid center
               if ((bl_l == 0) && (bl_r == 1) && (bl_u == 0) && (bl_d == 1)) fb = t + 0;  // upper left corner
               if ((bl_l == 1) && (bl_r == 0) && (bl_u == 0) && (bl_d == 1)) fb = t + 1;  // upper right corner
               if ((bl_l == 0) && (bl_r == 1) && (bl_u == 1) && (bl_d == 0)) fb = t + 2;  // lower left corner
               if ((bl_l == 1) && (bl_r == 0) && (bl_u == 1) && (bl_d == 0)) fb = t + 3;  // lower right corner

               // solid interior
               if ((bl_l == 1) && (bl_r == 1) && (bl_u == 1) && (bl_d == 1)) // blocks on all u d l r
               {
                  // single corner notch
                  if (bl_ul == 0) fb = 524; // empty to ul
                  if (bl_ur == 0) fb = 525; // empty to ur
                  if (bl_dl == 0) fb = 526; // empty to dr
                  if (bl_dr == 0) fb = 527; // empty to dl

                  // double corner notches
                  if ((bl_ur == 0) && (bl_dr == 0)) fb = 520; // empty to ur and dr
                  if ((bl_dr == 0) && (bl_dl == 0)) fb = 521; // empty to dr and dl
                  if ((bl_dl == 0) && (bl_ul == 0)) fb = 522; // empty to dl and ul
                  if ((bl_ul == 0) && (bl_ur == 0)) fb = 523; // empty to ul and ur

                  // triple corner notches
                  if ((bl_ur == 0) && (bl_ul == 1) && (bl_dl == 0) && (bl_dr == 0)) fb = 537; // only ul
                  if ((bl_ur == 1) && (bl_ul == 0) && (bl_dl == 0) && (bl_dr == 0)) fb = 538; // only ur
                  if ((bl_ur == 0) && (bl_ul == 0) && (bl_dl == 0) && (bl_dr == 1)) fb = 539; // only dr
                  if ((bl_ur == 0) && (bl_ul == 0) && (bl_dl == 1) && (bl_dr == 0)) fb = 540; // only dl

                  // all corner notches
                  if ((bl_ur == 0) && (bl_ul == 0) && (bl_dl == 0) && (bl_dr == 0)) fb = 592; // no diagonals

               }

               if ((bl_l == 0) && (bl_r == 1) && (bl_u == 1) && (bl_d == 1)) // solid except for left
               {
                  if ((bl_ur == 1) && (bl_dr == 1)) fb = 516; // solid to ur and dr
                  if ((bl_ur == 0) && (bl_dr == 1)) fb = 529; // open to ur and solid to dr
                  if ((bl_ur == 1) && (bl_dr == 0)) fb = 530; // solid to ur and open to dr
                  if ((bl_ur == 0) && (bl_dr == 0)) fb = 584; // open to ur and dr
               }

               if ((bl_l == 1) && (bl_r == 0) && (bl_u == 1) && (bl_d == 1)) // solid except for right
               {
                  if ((bl_ul == 1) && (bl_dl == 1)) fb = 517; // solid to ul and dl
                  if ((bl_ul == 0) && (bl_dl == 1)) fb = 531; // open to ul and solid to dl
                  if ((bl_ul == 1) && (bl_dl == 0)) fb = 532; // solid to ul and open to dl
                  if ((bl_ul == 0) && (bl_dl == 0)) fb = 586; // open to ul and dl
               }

               if ((bl_l == 1) && (bl_r == 1) && (bl_u == 0) && (bl_d == 1)) // solid except for up
               {
                  if ((bl_dl == 1) && (bl_dr == 1)) fb = 518; // solid to dl and dr
                  if ((bl_dl == 0) && (bl_dr == 1)) fb = 533; // open to dl and solid to dr
                  if ((bl_dl == 1) && (bl_dr == 0)) fb = 534; // solid to dl and open to dr
                  if ((bl_dl == 0) && (bl_dr == 0)) fb = 585; // open to dl and dr
               }

               if ((bl_l == 1) && (bl_r == 1) && (bl_u == 1) && (bl_d == 0)) // solid except for down
               {
                  if ((bl_ul == 1) && (bl_ur == 1)) fb = 519; // solid to ul and ur
                  if ((bl_ul == 0) && (bl_ur == 1)) fb = 535; // open to ul and solid to ur
                  if ((bl_ul == 1) && (bl_ur == 0)) fb = 536; // solid to ul and open to ur
                  if ((bl_ul == 0) && (bl_ur == 0)) fb = 587; // open to ul and ur
               }

               if ((bl_ur == 0) && (bl_ul == 0) && (bl_dl == 0) && (bl_dr == 0)) // no diagonals
                  if ((bl_l == 0) && (bl_r == 0) && (bl_u == 0) && (bl_d == 0)) // no blocks on all u d l r
                     fb = 541;  // orphan single block

               // single block line corners
               if ((bl_l == 0) && (bl_r == 1) && (bl_u == 0) && (bl_d == 1) && (bl_dr == 0)) fb = 576;  // upper left corner
               if ((bl_l == 1) && (bl_r == 0) && (bl_u == 0) && (bl_d == 1) && (bl_dl == 0)) fb = 577;  // upper right corner
               if ((bl_l == 0) && (bl_r == 1) && (bl_u == 1) && (bl_d == 0) && (bl_ur == 0)) fb = 578;  // lower left corner
               if ((bl_l == 1) && (bl_r == 0) && (bl_u == 1) && (bl_d == 0) && (bl_ul == 0)) fb = 579;  // lower right corner




               if ((bl_l == 0) && (bl_r == 0) && (bl_u == 1) && (bl_d == 1)) fb = 580; // vertical through line
               if ((bl_l == 1) && (bl_r == 1) && (bl_u == 0) && (bl_d == 0)) fb = 582; // horizontal through line

               if ((bl_l == 1) && (bl_r == 0) && (bl_u == 0) && (bl_d == 0)) fb = 588; // right end line
               if ((bl_l == 0) && (bl_r == 0) && (bl_u == 1) && (bl_d == 0)) fb = 589; // lower end line
               if ((bl_l == 0) && (bl_r == 1) && (bl_u == 0) && (bl_d == 0)) fb = 590; // left end line
               if ((bl_l == 0) && (bl_r == 0) && (bl_u == 0) && (bl_d == 1)) fb = 591; // upper end line
            }

            if (fb == -1)
            {
               // do nothing
            }
            if (fb == 0)
            {
               l[a][b] = 0;
            }

            if (fb > 0)
            {
               // change only tile portion (lower 10 bits)
               l[a][b] &= 0b11111111111111110000000000000000; // clear lower bits
               l[a][b] |= fb; // merge tile number
            }
         }
   init_level_background();

}

int th_draw_buttons(int x3, int x4, int yfb, int have_focus, int moving)
{
   int d = 1;
   if (have_focus) d = 0;
   if (moving) d = 1;
   int bts = 16;
   x3+=4;
   x4-=4;
   int yfb_old = yfb;


   int marked_count = 0;
   for (int a=0; a<100; a++)
      for (int b=0; b<100; b++)
        if (thl[a][b]) marked_count++;
   al_draw_textf(font, palette_color[15], (x3+x4)/2, yfb -34, ALLEGRO_ALIGN_CENTER, "Number of Tiles Marked: %d", marked_count);

   if (mdw_buttont(x3, yfb, x4, bts, 0,0,0,0, 0,9,15,0, 1,0,1,d, "Clear All Marks"))
   {
      for (int a=0; a<100; a++)
         for (int b=0; b<100; b++) thl[a][b] = 0;
   }

   if ((mW[9].th_add_del == 1) && (mdw_buttont(x3, yfb, x4, bts, 0,0,0,0, 0,9,15,0, 1,0,1,d, "Main Action: Add Marks to Tiles"))) mW[9].th_add_del = 0;
   if ((mW[9].th_add_del == 0) && (mdw_buttont(x3, yfb, x4, bts, 0,0,0,0, 0,9,15,0, 1,0,1,d, "Main Action: Remove Marks from Tiles"))) mW[9].th_add_del = 1;

   if (mW[9].th_add_del == 1)
   {
      if      ((mW[9].th_match == 0) && (mdw_buttont(x3, yfb, x4, bts, 0,0,0,0, 0,9,15,0, 1,0,1,d, "Add Single Tile"))) mW[9].th_match = 1;
      else if ((mW[9].th_match == 1) && (mdw_buttont(x3, yfb, x4, bts, 0,0,0,0, 0,9,15,0, 1,0,1,d, "Add All Matching Tiles"))) mW[9].th_match = 2;
      else if ((mW[9].th_match == 2) && (mdw_buttont(x3, yfb, x4, bts, 0,0,0,0, 0,9,15,0, 1,0,1,d, "Add All Connected Matching Tiles"))) mW[9].th_match = 0;
   }
   else
   {
      if      ((mW[9].th_match == 0) && (mdw_buttont(x3, yfb, x4, bts, 0,0,0,0, 0,9,15,0, 1,0,1,d, "Remove Single Tile"))) mW[9].th_match = 1;
      else if ((mW[9].th_match == 1) && (mdw_buttont(x3, yfb, x4, bts, 0,0,0,0, 0,9,15,0, 1,0,1,d, "Remove All Matching Tiles"))) mW[9].th_match = 2;
      else if ((mW[9].th_match == 2) && (mdw_buttont(x3, yfb, x4, bts, 0,0,0,0, 0,9,15,0, 1,0,1,d, "Remove All Connected Matching Tiles"))) mW[9].th_match = 0;
   }

   if (mW[9].th_match)
   {
      if      ((mW[9].th_group == 0) && (mdw_buttont(x3, yfb, x4, bts, 0,0,0,0, 0,9,15,0, 1,0,1,d, "Tile Filter: Specific Tile"))) mW[9].th_group = 1;
      else if ((mW[9].th_group == 1) && (mdw_buttont(x3, yfb, x4, bts, 0,0,0,0, 0,9,15,0, 1,0,1,d, "Tile Filter: Tile Group"))) mW[9].th_group = 0;
   }

   // draw a rectangle and title around this group of buttons
   titlex("Modify Which Tiles Are Marked", 15, 14, x3, x4, yfb_old-16);
   al_draw_rectangle(x3, yfb_old-16, x4, yfb-1, palette_color[14], 1);


   yfb+=bts*2; // spacing between groups
   yfb_old = yfb;
   bts = 24;
   int c = 10 + 64;
   int choice = 0;
   if (mdw_buttontt(x3, yfb, x4, bts, 576,0,0,0, 0,c,15,0, 1,1,1,d, "     Purple Pipes with Cross Center")) choice = 1;
   if (mdw_buttontt(x3, yfb, x4, bts, 512,0,0,0, 0,c,15,0, 1,1,1,d, "     Purple Pipes with Solid Center")) choice = 4;
   if (mdw_buttontt(x3, yfb, x4, bts, 608,0,0,0, 0,c,15,0, 1,1,1,d, "     Wires with Cross Center")) choice = 2;
   if (mdw_buttontt(x3, yfb, x4, bts, 480,0,0,0, 0,c,15,0, 1,1,1,d, "     Grey Bricks")) choice = 3;
   if (mdw_buttontt(x3, yfb, x4, bts, 448,0,0,0, 0,c,15,0, 1,1,1,d, "     Brown Bricks")) choice = 5;
   if (mdw_buttontt(x3, yfb, x4, bts, 416,0,0,0, 0,c,15,0, 1,1,1,d, "     Brown and Yellow Thatch")) choice = 7;
   if (mdw_buttontt(x3, yfb, x4, bts, 384,0,0,0, 0,c,15,0, 1,1,1,d, "     Brain")) choice = 8;

   // draw a rectangle and title around this group of buttons
   titlex("Change Marked Tiles", 15, 10, x3, x4, yfb_old-16);
   al_draw_rectangle(x3, yfb_old-16, x4, yfb-1, palette_color[10], 1);

   if (choice) th_replace(choice);
   return yfb;
}

int th_compare_tile(int rb, int cb, int group)
{
   // remove flags for comparison
   int r = rb & 1023;
   int c = cb & 1023;

   if (group == 0) if (r == c) return 1;

   if (group == 1)
   {

      // even though group is selected, if we have an exact match just do it
      if (r == c) return 1;

      // grey brick with corners
      int sb = 480; // start block
      int eb = 498; // end block
      if ((r >= sb) && (r <= eb) && (c >= sb) && (c <= eb)) return 1;

      // purple pipe with open center
      sb = 576; // start block
      eb = 592; // end block
      if ((r >= sb) && (r <= eb) && (c >= sb) && (c <= eb)) return 1;

      // purple pipe with solid center
      sb = 512; // start block
      eb = 541; // end block
      if ((r >= sb) && (r <= eb) && (c >= sb) && (c <= eb)) return 1;

      // wires
      sb = 608; // start block
      eb = 626; // end block
      if ((r >= sb) && (r <= eb) && (c >= sb) && (c <= eb)) return 1;

      // brown brick with corners
      sb = 448; // start block
      eb = 466; // end block
      if ((r >= sb) && (r <= eb) && (c >= sb) && (c <= eb)) return 1;

      // brown and yellow thatch
      sb = 416; // start block
      eb = 434; // end block
      if ((r >= sb) && (r <= eb) && (c >= sb) && (c <= eb)) return 1;

      // white brain with blue background
      sb = 384; // start block
      eb = 402; // end block
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
