// glt.cpp (20100220 cleanup)
#include "pm.h"


int blt[NUM_SPRITES];
void show_block_list(void)
{
   int y = 0;
   al_draw_filled_rectangle(200, 0, SCREEN_W-1, SCREEN_H-1, palette_color[0]);
   for (int z=0; z<NUM_SPRITES; z++)
      if (blt[z])
      {
         al_draw_bitmap(memory_bitmap[z], 180, y, 0);
         //al_draw_textf(font, palette_color[11], 200, 10+y, 0, "sa%d   block# %d   count %d ",sa[z][0],  z, blt[z] );
         al_draw_textf(font, palette_color[11], 200, y, 0, "block:%d count:%d ", z, blt[z] );
         y+=20;
         if (y > SCREEN_H-20)
         {
            al_flip_display();
            al_clear_to_color(al_map_rgb(0,0,0));
            tsw();
            y = 0;
         }
      }
   al_flip_display();
   tsw();
}

void global_level()
{
   int old_start_level = start_level;

   int le[200]; // level exists array

   int num_levs = 0;
   char fn[20] = "levels/level000.PML";
   int x, z;

   // clear filename array
   for (x=0; x<200; x++) le[x] = 0;

   // clear block list
   for (z=0; z<1024; z++) blt[z] = 0;

   // level range to look for
   for (x=0; x<400; x++)
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

   al_set_target_backbuffer(display);
   al_flip_display();
   al_clear_to_color(al_map_rgb(0,0,0));



   // iterate array of found levels
   for (x=0; x<num_levs; x++)
   {
      al_set_target_backbuffer(display);
      al_flip_display();

      // progress bar
      draw_percent_bar(SCREEN_W/2, SCREEN_H/2, SCREEN_W-200, 20, (x+1)*100 / num_levs);
      al_draw_text(font, palette_color[15], SCREEN_W/2, SCREEN_H/2+7 , ALLEGRO_ALIGN_CENTER, "Doing glt...");
      al_draw_textf(font, palette_color[11], 10, 10+x*8, 0, "lev:%d", le[x]);
      load_level(le[x], 0);

  //------------------------------------------------------------------------------------------------------------------
  //--------------check for bad enemy data ---------------------------------------------------------------------------
  //------------------------------------------------------------------------------------------------------------------
         for (int y=0; y<100; y++)
         {
             int good = 0;
             int type = Ei[y][0];
             if (type == 0) good = 1; // this is a legal value
             if (type == 3) good = 1;
             if (type == 4) good = 1;
             if (type == 6) good = 1;
             if (type == 7) good = 1;
             if (type == 8) good = 1;
             if (type == 9) good = 1;
             if (type == 11) good = 1;
             if (type == 12) good = 1;

             if (!good)
             {
                Ei[y][0] = 0; // erase
                printf("lev%d -- enemy:%d - bad type%d\n", le[x], y, type);
             }


             else if (type) // only check the rest if type valid
             {
                 int shape = Ei[y][1];
                 // min is flapper with 155
                 // max is block walker 864
                 if ((shape < 147) || (shape > 1020))
                    printf("lev%d -- enemy:%d - bad shape%d\n", le[x], y, shape);

                  al_fixed scale = Efi[y][12];
                  if ((scale < al_ftofix(.2)) || (scale > al_ftofix(10)))
                      printf("lev%d -- enemy:%d - bad scale%f\n", le[x], y, al_fixtof(scale));

                  al_fixed rot = Efi[y][14];
                  if ((rot < al_itofix(-256)) || (rot > al_itofix(256)))
                      printf("lev%d -- enemy:%d - bad rot%f\n", le[x], y, al_fixtof(rot));

                  al_fixed xpos = Efi[y][0];
                  if ((xpos < al_itofix(20)) || (xpos > al_itofix(1960)))
                      printf("lev%d -- enemy:%d - bad xpos%f\n", le[x], y, al_fixtof(xpos));

                  al_fixed ypos = Efi[y][1];
                  if ((ypos < al_itofix(20)) || (ypos > al_itofix(1960)))
                      printf("lev%d -- enemy:%d - bad ypos%f\n", le[x], y, al_fixtof(ypos));

             }
          }


/*

  //------------------------------------------------------------------------------------------------------------------
  //--------------check for bad itemdata ---------------------------------------------------------------------------
  //------------------------------------------------------------------------------------------------------------------

   for (int c=0; c<500; c++)
      if (item[c][0])
      {
         // first check for valid type
         int good = 0;
         if (item[c][0] == 1) good = 1;
         if (item[c][0] == 2) good = 1;
         if (item[c][0] == 3) good = 1;
         if (item[c][0] == 4) good = 1;
         if (item[c][0] == 5) good = 1;
         if (item[c][0] == 6) good = 1;
         if (item[c][0] == 7) good = 1;
         if (item[c][0] == 8) good = 1;
         if (item[c][0] == 10) good = 1;
         if (item[c][0] == 11) good = 1;
         if (item[c][0] == 12) good = 1;
         if (item[c][0] == 14) good = 1;
         if (item[c][0] == 15) good = 1;

         if (!good)  printf("Level:%d - Item:%d - bad type:%d\n", le[x], c, item[c][0]);

         if (good)
         {
            if ((item[c][1] < 169) || (item[c][1] > 1084))
               printf("Level:%d - Item:%d - bad shape:%d\n", le[x], c, item[c][1]);


            int xpos = item[c][4];
            if ((xpos < 20) || (xpos > 1960))
                printf("Level:%d - Item:%d - bad xpos%d\n", le[x], c, xpos);

            int ypos = item[c][5];
            if ((ypos < 20) || (ypos > 1960))
                printf("Level:%d - Item:%d - bad ypos%d\n", le[x], c, ypos);
         }
      }

*/


/*

// get min max item shapes
      for (int y=0; y<500; y++)
         if (item[y][0])
         {
            count0++;
            if (item[y][1] > max ) max = item[y][1];

            if (item[y][1] > 0)
               if (item[y][1] < min ) min = item[y][1];

            if (item[y][1] == 0) count1++;
            if (item[y][1] > 0) count2++;
         }




         for (int y=0; y<100; y++)
            if (Ei[y][0] == 12) // flapper
               if (Ei[y][19] == 0)
               {
//                  Ei[y][17] = 160;
  //                Ei[y][18] = 40;
                  Ei[y][19] = 100;
               }


         for (int y=0; y<100; y++)
            if (Ei[y][0])
            {
               Ei[y][28] = 0;
            }


         for (int y=0; y<100; y++)
            if (Ei[y][0] == 9) // cloner only
               if ((Ei[y][19] ==0) && (Ei[y][20] ==0))
               {


                  printf("lev%d\n", le[x]);

                }


*/

/*


Ei[y][15] = al_fixtoi(Efi[y][6])/20;    // source x
Ei[y][16] = al_fixtoi(Efi[y][7])/20;    // source y

Ei[y][17] = al_fixtoi(Efi[y][8])/20;    // dest x
Ei[y][18] = al_fixtoi(Efi[y][9])/20;    // dest y

Ei[y][19] = al_fixtoi(Efi[y][2])/20;    // w
Ei[y][20] = al_fixtoi(Efi[y][3])/20;    // h


Ef[y][2] = 0;
Ef[y][3] = 0;
Ef[y][6] = 0;
Ef[y][7] = 0;
Ef[y][8] = 0;
Ef[y][9] = 0;


*/

/*

      for (int y=0; y<500; y++)
         if (item[y][0] == 14) // switch only
         {
             if (item[y][1] == 746) count0++;

             if (item[y][1] == 746) item[y][1] = 745;
         }


*/



/*


      for (int y=0; y<500; y++)
//         if (item[y][0] == 4) // keys only
         {
             if (item[y][10] != 0) count0++;

             if (item[y][0] == 11) count1++;
             if ((item[y][0] == 11) && (item[y][10] = 0)) count2++;

             if ((item[y][10] != 0) && (item[y][0] != 11))  count3++;



//            item[y][10] = item[y][4]/20;
 //           item[y][11] = item[y][5]/20;
         }


*/


/*
      for (int y=0; y<500; y++)
         if (item[y][0] == 1) // door only
         {
            item[y][6] = item[y][14];
            item[y][14] = 0;
            item[y][15] = 0;
        }

*/

/*

            if (item[y][8] == 0) count1++; // old door
            if (item[y][8] == 1) count2++; // new door
            if (item[y][8] == 2) count3++; // exit only

            if (item[y][8] == 2) item[y][8] = 0;

           }

*/

/*
      for (int y=0; y<500; y++)
         if (item[y][0] == 1) // door only
         {
            count0++;
            if (item[y][13] == 448) count1++;
            if (item[y][13] == 1083) count2++;

            if ((item[y][13] != 1083) && (item[y][13] != 448))
               printf("lev%d\n", le[x]);

         }

*/






/*

            if (item[y][8] == 0)
            {
               // create a new item and link to it
               int found_empty_items = 0;
               int c = get_empty_item(); // get 1st item
               if (c != -1) found_empty_items = 1;

               if (found_empty_items)
               {
                  // new item
                  item[c][0] = 1;
                  item[c][1] = 1084; // ans shape
                  item[c][2] = 1; // draw mode
                  item[c][3] = 0; // stationary

                  item[c][4] = item[y][6]*20;
                  item[c][5] = item[y][7]*20;

                  item[c][8] = 2; // exit only
                  item[c][9] = 0; // link
                  item[c][11] = 0; // immediate
                  item[c][12] = 1; // draw lines always
                  item[c][13] = 1084; // ans shape


                  // this item
                  item[y][1] = 1083; // ans shape
                  item[y][2] = 1;    // draw mode
                  item[y][3] = 0;    // stat

                  item[y][6] = 0;
                  item[y][7] = 0;
                  item[y][8] = 1; // linked exit
                  item[y][9] = c; // link
                  item[y][11] = 0; // immediate
                  item[y][12] = 1; // draw lines always
                  item[y][13] = 1083; // base ans shape


//                  l[item[c][4]/20][item[c][5]/20] = 0;   // zero block


                 }
              }


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
                      l, lifts[l]->lift_name, lifts[l]->width, lifts[l]->height, lifts[l]->color, lifts[l]->num_steps);
         printf("-----------------------------------------------------------\n");

         for (int s=0; s<lifts[l].num_steps; s++)
         {
            count1++;

            int type = lift_steps[l][s].type;

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


            int zv = lift_steps[l][s].val;
            if ((zv < 0) || (zv > 1999))
            {
                count2++;
                printf("Lev:%3d lift:%-2d step%d val:%d\n",le[x], l, s, zv);
             }


            printf("step:%-2d ",s);
            printf("x:%-4d y:%-4d ", lift_steps[l][s].x, lift_steps[l][s].y);
            printf("val:%-4d type:%d ",  lift_steps[l][s].val, lift_steps[l][s].type );

//            printf("(%s)\n", lift_step_type_name[lift_steps[l][s].type] );

            printf("\n");




            printf("step:%-2d x:%-4d y:%-4d val:%-4d type:%d(%s)\n",
                         s, lift_steps[l][s].x,
                         lift_steps[l][s].y, lift_steps[l][s].val, lift_steps[l][s].type,
                         lift_step_type_name[lift_steps[l][s].type] );


         }
//         printf("------------------------------------------------------------\n");
      }





      // items


      for (y=0; y<500; y++)
         if (item[y][0] == 0) item[y][0] = 0;

*/
/*
      for (y=0; y<500; y++)
         if (item[y][0] == 2)
         {
            count0++;

            if (item[y][7]) count1++;
            if (item[y][8]) count2++;
            if (item[y][9]) count3++;


            if (item[y][8]) item[y][0] = 0;
            if (item[y][9]) item[y][0] = 0;


            printf("bonus: %d %d %d\n", item[y][7], item[y][8], item[y][9]);
         }

*/





      // enemies
/*

         for (int y=0; y<100; y++)
            if (Ei[y][0] == 3) // archwagon
            {
               count0++;

//               if (Efi[y][6] == al_itofix(0)) count1++;

 //              if (Efi[y][6] == al_ftofix(2.85)) count2++;

   //            if (Efi[y][2] == al_itofix(0)) count3++;




               if (Efi[y][2] == Efi[y][6]) count1++;
               if (Efi[y][2] == -Efi[y][6]) count2++;


               if (Efi[y][2] > al_itofix(0)) Efi[y][2] = Efi[y][6];
               if (Efi[y][2] < al_itofix(0)) Efi[y][2] = -Efi[y][6];




//               al_fixed xlen = Efi[y][2];
  //             al_fixed ylen = Efi[y][3];
    //           printf("e:%d  %d [%f] [%f] \n", y, Ei[y][20], al_fixtof(xlen), al_fixtof(ylen));
            }



*/





/*

      for (y=0; y<500; y++)
         if (item[y][0] == 11) // rocket
         {

             item[y][2] = 2; // rotate draw mode

         }


*/


/*



      // enemies

//      if (le[x] == 1) // level 1 only


         for (y=0; y<100; y++)
            if (Ei[y][0] == 6) // cannon only
            {
               count0++;

//               al_fixed xlen = Efi[y][2];
  //             al_fixed ylen = Efi[y][3];
    //           printf("e:%d  %d [%f] [%f] \n", y, Ei[y][20], al_fixtof(xlen), al_fixtof(ylen));
            }

  */

/*
            int r1 = get_rot_from_xyinc(y);
            int r2 = get_rot_from_xyinc(y);

            al_fixed xlen = Efi[y][2];
            al_fixed ylen = Efi[y][3];


            printf("Lev:%3d e:%d  %d %d %d [%f] [%f] \n" ,le[x], y, Ei[y][20], r1, r2, al_fixtof(xlen), al_fixtof(ylen));

            Ei[y][20] = r2;
*/





/*
         for (int y=0; y<100; y++)
            if (Ei[y][0])
            {
               count0++;
               if (Ei[y][29] != 10)  // how many cbs are non standard
               {
                  count1++;
   //               Ei[y][29] = 10;
                  printf("l:%3d E:%2d T:%2d C:%2d \n",le[x], y, Ei[y][0], Ei[y][29] );

               }



               }


*/
   /*



      for (y=0; y<100; y++)
      {
         if (Ei[y][0])
         {


            printf("Lev:%3d b:%d\n",le[x], y);





            if (Ei[y][29] != 10)  // how many cbs are non standard
            {
               Ei[y][29] = 10;
               printf("l:%3d E:%2d T:%2d C:%2d \n",le[x], y, Ei[y][0], Ei[y][29] );
               count++;
            }
         }

         if (Ei[y][0] == 9) // cloner
         {
            printf("l:%3d E:%2d [cloner  ] C:%2d \n",le[x], y, Ei[y][29] );
            Ei[y][29] = 10;  // set collision box
            count++;
         }
         if (Ei[y][0] == 7)  // podzilla only
         {
            printf("l:%3d E:%2d [podzilla] C:%2d \n",le[x], y, Ei[y][29] );
            Ei[y][29] = 10;  // set collision box
            count++;

         }
      }




      for (y=0; y<100; y++)
         if (Ei[y][0])
         {
             Ef[y][14] = (float)Ei[y][20];
             Ei[y][20] = 0;
         }




      for (y=0; y<100; y++)
         if (Ei[y][0])
         {
            if (Ei[y][0] == 9999)  // set bonus
            {
               Ei[y][24] = 3;
               Ei[y][25] = 3;
            }

            if (Ei[y][0] == 9999)  // erase enemy types here
            {
               for (z=0; z<32; z++)
                  Ei[y][z] = 0;
               for (z=0; z<16; z++)
                  Ef[y][z] = 0;
            }
         }

*/

/*

    for (int y=0; y<500; y++)
         if (item[y][0] == 14) //switch
         {
            if (item[y][1] == 422) count2++;
            if (item[y][1] == 423) count2++;
            if (item[y][1] == 364) count2++;
            if (item[y][1] == 365) count2++;


            }

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

   // blocks
   for (int y=0; y<100; y++)
      for (int z=0; z<100; z++)
         blt[l[y][z]]++; // inc block counter


/*

      // blocks
      for (y=0; y<100; y++)
         for (z=0; z<100; z++)
         {
            blt[l[y][z]]++; // inc block counter


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

      if (0)
      {
         save_level(le[x]);
         al_set_target_backbuffer(display);
         al_draw_textf(font, palette_color[10], 110, 10+x*8, 0, "lev:%d", le[x]);
      }
   } // end of level iterate
   al_flip_display();


   printf("Total count0:%d \n",count0 );
   printf("Total count1:%d \n",count1 );
   printf("Total count2:%d \n",count2 );
   printf("Total count3:%d \n",count3 );
   printf("min:%d max:%d\n", min, max);



   //show_block_list();
   tsw();


   start_level = old_start_level;
   load_level(start_level, 0);
}




