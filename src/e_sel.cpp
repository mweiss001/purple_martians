//  e_sel.cpp (20100220 cleanup)

#include "pm.h"

void clear_ft(void)
{
   for (int x=0; x<20; x++) // level_header
      ft_level_header[x] = 0;

   for (int x=0; x<100; x++) // blocks
      for (int y=0; y<100; y++) ft_l[x][y] = 0;

   for (int c=0; c<500; c++) // items
   {
      free (ft_pmsg[c]);
      for (int x=0; x<16; x++) ft_item[c][x] = 0;
   }
   for (int c=0; c<100; c++) // enemies
   {
      for (int x=0; x<32; x++) ft_Ei[c][x] = 0;
      for (int x=0; x<16; x++) ft_Efi[c][x] = al_itofix(0);
   }

   for (int l=0; l<NUM_LIFTS; l++)  // lifts
   {
     ft_ln[l][0] = 0; // erase lift name
     for (int x=0; x<4; x++) // lift var
        ft_lift[l][x] = 0;
     for (int s=0; s<40; s++) // step
        for (int x=0; x<4; x++) // step var
           ft_ls[l][s][x] = 0;
   }
}



int load_selection(void)
{
   FILE *filepntr;
   int loop, ch, c, x, y;
   char buff[2000];
   sprintf(sel_filename,"sel\\");
   if (mw_file_select("Load Selection", sel_filename, ".sel", 0))
   {
      clear_ft();
      if (!al_filename_exists(sel_filename))
      {
         sprintf(msg, "Can't Find %s ", sel_filename);
         m_err(msg);
         return 0;
      }
      if ((filepntr=fopen(sel_filename,"r")) == NULL)
      {
         sprintf(msg, "Error opening %s ", sel_filename);
         m_err(msg);
         return 0;
      }

      for (c=0; c<20; c++) // level header
      {
         loop = 0;
         ch = fgetc(filepntr);
         while((ch != '\n') && (ch != EOF))
         {
            buff[loop] = ch;
            loop++;
            ch = fgetc(filepntr);
         }
         buff[loop] = (char)NULL;
         ft_level_header[c] = atoi(buff);
      }

      for (c=0; c<ft_level_header[8]; c++)  // l[100][100]
         for (y=0; y<ft_level_header[9]; y++)
         {
            loop = 0;
            ch = fgetc(filepntr);
            while((ch != '\n') && (ch != EOF))
            {
               buff[loop] = ch;
               loop++;
               ch = fgetc(filepntr);
            }
            buff[loop] = (char)NULL;
            ft_l[c][y] = atoi(buff);
         }
      for (c=0; c<ft_level_header[3]; c++)  // read item
      {
         for (x=0; x<16; x++)
         {
            loop = 0;
            ch = fgetc(filepntr);
            while((ch != '\n') && (ch != EOF))
            {
                buff[loop] = ch;
                loop++;
                ch = fgetc(filepntr);
            }
            buff[loop] = (char)NULL;
            ft_item[c][x] = atoi(buff);
         }
         if (ft_item[c][0] == 10) // get pmsg
         {
            loop = 0;
            ch = fgetc(filepntr);
            while((ch != '\n') && (ch != EOF))
            {
               //if (ch == 126) ch = 13;
               buff[loop] = ch;
               loop++;
               ch = fgetc(filepntr);
            }
            buff[loop] = (char)NULL;
            ft_pmsg[c] = (char*) malloc (strlen(buff)+1);
            strcpy(ft_pmsg[c], buff);
         }
      }
      for (c=0; c < ft_level_header[4]; c++)  // enemy ints and fixeds
      {
         for (x=0; x<32; x++)  // first 32 ints
         {
            loop = 0;
            ch = fgetc(filepntr);
            while((ch != '\n') && (ch != EOF))
            {
                buff[loop] = ch;
                loop++;
                ch = fgetc(filepntr);
            }
            buff[loop] = (char)NULL;
            ft_Ei[c][x] = atoi(buff);
         }
         for (x=0; x<16; x++)  // then 16 fixeds
         {
            loop = 0;
            ch = fgetc(filepntr);
            while((ch != '\n') && (ch != EOF))
            {
               buff[loop] = ch;
               loop++;
               ch = fgetc(filepntr);
            }
            buff[loop] = (char)NULL;
            ft_Efi[c][x] = atoi(buff);
         }
      }
      for (c=0; c<ft_level_header[5]; c++) // lifts
      {
         // get lift name
         loop = 0;
         ch = fgetc(filepntr);
         while((ch != '\n') && (ch != EOF))
         {
             buff[loop] = ch;
             loop++;
             ch = fgetc(filepntr);
         }
         buff[loop] = (char)NULL;
         strcpy(ft_ln[c], buff);

         // get lift data   four ints
         for (y=0; y<4; y++)
         {
            loop = 0;
            ch = fgetc(filepntr);
            while((ch != '\n') && (ch != EOF))
            {
                buff[loop] = ch;
                loop++;
                ch = fgetc(filepntr);
            }
            buff[loop] = (char)NULL;
            ft_lift[c][y] = atoi(buff);
         }
         for (x=0; x<ft_lift[c][3]; x++) // get step data
            for (y=0; y<4; y++)
            {
               loop = 0;
               ch = fgetc(filepntr);
               while((ch != '\n') && (ch != EOF))
               {
                   buff[loop] = ch;
                   loop++;
                   ch = fgetc(filepntr);
               }
               buff[loop] = (char)NULL;
               ft_ls[c][x][y] = atoi(buff);
            }
      } // next lift
      fclose(filepntr);
   } // end of file select
   else return 0; // cancel pressed
   return 1;

}


void save_selection(int save)
{
   int b, c, x, y;
   int eib=0;
   int iib=0;
   int lib=0;
   int x1 = stx * 20;    // source x
   int y1 = sty * 20;    // source y
   int x2 = sux * 20;    // sizes
   int y2 = suy * 20;
   al_fixed fx1 = al_itofix(x1);
   al_fixed fy1 = al_itofix(y1);
   al_fixed fx2 = al_itofix(x2);
   al_fixed fy2 = al_itofix(y2);

   clear_ft();

   if (copy_blocks)
      for (x=0; x<(sux-stx); x++)
         for (y=0; y<(suy-sty); y++)
            if ( (x >= 0) && (x < 100) && (y >= 0) && (y < 100) && (stx+x >= 0) && (stx+x < 100) && (sty+y >= 0) && (sty+y < 100) )
               ft_l[x][y] = l[stx+x][sty+y];

   if (copy_items)
      for (b=0; b<500; b++) // check for items in box
         if ((item[b][0]) && (item[b][4] >= x1) && (item[b][4] < x2) && (item[b][5] >= y1) && (item[b][5] < y2))
         {
            c = iib++;
            // copy all 16 variables
            for (y=0; y<16; y++)
                ft_item[c][y] = item[b][y];

            // set new x, y (now relative to the selection window ul corner)
            ft_item[c][4] = item[b][4] - x1;
            ft_item[c][5] = item[b][5] - y1;

            if (item[b][0] == 4) // key
            {   // set new destination
               ft_item[c][6] = item[b][6] - stx;
               ft_item[c][7] = item[b][7] - sty;
               ft_item[c][8] = item[b][8] - stx;
               ft_item[c][9] = item[b][9] - sty;
            }
            if (item[b][0] == 10) // message
            {
               ft_item[c][10] = item[b][10] - stx;
               ft_item[c][11] = item[b][11] - sty;
               free (ft_pmsg[c]);
               ft_pmsg[c] = (char*) malloc (strlen(pmsg[b])+1);
               strcpy(ft_pmsg[c], pmsg[b]);
            }
         }

   if (copy_enemies)
      for (b=0; b<100; b++) // check for enemies in box
         if ((Ei[b][0]) && (Efi[b][0] >= fx1) && (Efi[b][0] < fx2) && (Efi[b][1] >= fy1) && (Efi[b][1] < fy2))
         {
            //printf("copying enemy:%d to ft\n", b);
            c = eib++;
            for (y=0; y<32; y++)
               ft_Ei[c][y] = Ei[b][y];
            for (y=0; y<16; y++)
               ft_Efi[c][y] = Efi[b][y];

            ft_Efi[c][0]-= fx1;
            ft_Efi[c][1]-= fy1;

            if (ft_Ei[c][0] == 7 ) // podzilla
            {
               ft_Ei[c][11]-= x1/20;
               ft_Ei[c][12]-= y1/20;
               ft_Ei[c][13]-= x1/20;
               ft_Ei[c][14]-= y1/20;
            }
            if (ft_Ei[c][0] == 9 ) // cloner
            {
               ft_Ei[c][11]-= x1/20;
               ft_Ei[c][12]-= y1/20;
               ft_Ei[c][13]-= x1/20;
               ft_Ei[c][14]-= y1/20;
               ft_Ei[c][15]-= x1/20;
               ft_Ei[c][16]-= y1/20;
               ft_Ei[c][17]-= x1/20;
               ft_Ei[c][18]-= y1/20;
            }
         }


   if (copy_lifts)
      for (b=0; b<num_lifts; b++) // source, if in selection
         if ((lifts[b].x1 >= x1) && (lifts[b].x1 < x2) && (lifts[b].y1 >= y1) && (lifts[b].y1 < y2))
         {
            c = lib++; // destination

            strcpy(ft_ln[c], lifts[b].lift_name);

            ft_lift[c][0] = lifts[b].width;
            ft_lift[c][1] = lifts[b].height;
            ft_lift[c][2] = lifts[b].color;
            ft_lift[c][3] = lifts[b].num_steps;

            for (y = 0; y < lifts[b].num_steps; y++) // copy steps
            {
               int vx = 0;
               int vy = 0;

               if (lift_steps[b][y].type == 1) // shift move steps
               {
                  vx = lift_steps[b][y].x - x1;
                  vy = lift_steps[b][y].y - y1;
               }

               ft_ls[c][y][0] = vx;
               ft_ls[c][y][1] = vy;
               ft_ls[c][y][2] = lift_steps[b][y].val;
               ft_ls[c][y][3] = lift_steps[b][y].type;
            }
         }


   ft_level_header[3] = iib; // num_of_items
   ft_level_header[4] = eib; // num_of_enemies
   ft_level_header[5] = lib; // num_of_lifts

   ft_level_header[8] =  sux-stx; // width
   ft_level_header[9] =  suy-sty; // height

   // printf("finished copying to ft - i:%d e:%d l:%d\n", iib, eib, lib);


   if (save)
   {
      al_make_directory("sel"); // create if not already created
      FILE *filepntr;
      sprintf(sel_filename, "sel\\");
      if (mw_file_select("Save Selection", sel_filename, ".sel", 1))
      {
         //printf("fn:%s\n", sel_filename);
         filepntr = fopen(sel_filename,"w");
         for (x=0; x<20; x++)
            fprintf(filepntr,"%d\n",ft_level_header[x]);
         for (c=0; c<(sux-stx); c++)  // selection of blocks
            for (x=0; x<(suy-sty); x++)
               fprintf(filepntr,"%d\n",ft_l[c][x]);

         for (c=0; c < ft_level_header[3]; c++) // items
         {
            for (x=0; x<16; x++)
               fprintf(filepntr,"%d\n",ft_item[c][x]);

            if (ft_item[c][0] == 10) // pmsg
            {
               y = 0;
               while (ft_pmsg[c][y] != (char)NULL)
               {
                  if (ft_pmsg[c][y] == 13) fprintf(filepntr,"%c",126);
                  else fprintf(filepntr,"%c",ft_pmsg[c][y]);
                  y++ ;
               }
               fprintf(filepntr,"\n");
            }
         }
         for (c=0; c < ft_level_header[4]; c++) // enemy int and float
         {
            for (x=0; x<32; x++) fprintf(filepntr,"%d\n",ft_Ei[c][x]);
            for (x=0; x<16; x++) fprintf(filepntr,"%d\n",ft_Efi[c][x]);
         }
         for (c=0; c < ft_level_header[5]; c++) // lifts
         {
            fprintf(filepntr,"%s\n",ft_ln[c]);
            for (x=0; x<4; x++) fprintf(filepntr,"%d\n",ft_lift[c][x]);
            for (x=0; x<ft_lift[c][3]; x++)
               for (y=0; y<4; y++) fprintf(filepntr,"%d\n",ft_ls[c][x][y]);
         }
         fclose(filepntr);
      }
   }
}

int enforce_limit(int val, int ll, int ul)
{
   if (val < ll) val = ll;
   if (val > ul) val = ul;
   return val;
}

int check_limit(int val, int ll, int ul)
{
   if (val < ll) return 1;
   if (val > ul) return 1;
   return 0;
}


void do_fcopy(int qx1, int qy1)
{
   int b, c, x, y;
   int x3 = qx1*20;         // dest x
   int y3 = qy1*20;         // dest y
   int x5 = ft_level_header[8];
   int y5 = ft_level_header[9];

   int erase_out_of_bounds_main = 0;      // if 0 we will adjust
   int erase_out_of_bounds_secondary = 0; // if 0 we will adjust

   if (copy_blocks)
      for (x=0; x<x5; x++)
         for (y=0; y<y5; y++)
            if ( (qx1+x >= 0) && (qx1+x < 100) )
               if ( (qy1+y >= 0) && (qy1+y < 100) )
                  l[qx1+x][qy1+y] = ft_l[x][y];

   if (copy_lifts)
   {
      for (b=0; b<ft_level_header[5]; b++)
      {
         //int copied = 0;
         if (num_lifts < NUM_LIFTS)
         {
            int vx = 0, vy = 0, lim = 0;
            c = num_lifts++; // dest lift
            //copied = 1000+c;
            construct_lift(c, ft_ln[b], ft_lift[b][0], ft_lift[b][1], ft_lift[b][2], ft_lift[b][3]);
            for (y=0; y<ft_lift[b][3]; y++) // copy steps
            {
               if (ft_ls[b][y][3] == 1) // shift  move steps
               {
                  // apply offsets
                  vx = ft_ls[b][y][0] + x3;
                  vy = ft_ls[b][y][1] + y3;

                  if (erase_out_of_bounds_main)
                  {
                     if (check_limit(vx, 0, 1980)) lim = 1;
                     if (check_limit(vy, 0, 1980)) lim = 1;
                  }
                  else // adjust if out of bounds
                  {
                     vx = enforce_limit(vx, 0, 1980);
                     vy = enforce_limit(vy, 0, 1980);
                  }
               }
               construct_lift_step(c, y, vx, vy, ft_ls[b][y][2], ft_ls[b][y][3]);
            }
            set_lift(c, 0);
            if (lim)
            {
               erase_lift(c);
               //copied = -1;
            }
         }

//         #ifdef LOGGING_ZFS
//         sprintf(msg,"dfc attempt to copy lift:%d - ", b);
//         add_log_entry(msg);
//         if (copied >= 1000)  sprintf(msg,"found empty lift:%d\n", copied-1000);
//         if (copied == 0)  sprintf(msg,"copy aborted - no empty lifts found\n");
//         if (copied == -1) sprintf(msg,"copy aborted - limits exceeded\n");
//         add_log_entry(msg);
//         #endif

      } // end of iterate enemies in ft
   } // end of if copy lifts
   if (copy_enemies)
   {
      for (b=0; b<100; b++) // iterate enemies in ft
      {
         if (ft_Ei[b][0]) // if active attempt to copy this enemy
         {
            //int copied = 0;
            for (c=0; c<100; c++)
            {
               if (Ei[c][0] == 0) // found empty
               {
                  //copied = 1000+c;
                  int lim = 0;
                  for (y=0; y<32; y++)        // copy 32 ints
                     Ei[c][y] = ft_Ei[b][y];
                  for (y=0; y<16; y++)        // copy 16 al_fixed
                     Efi[c][y] = ft_Efi[b][y];

                  // apply offsets
                  Efi[c][0] += al_itofix(x3);
                  Efi[c][1] += al_itofix(y3);

                  if (erase_out_of_bounds_main)
                  {
                     if (check_limit(al_fixtoi(Efi[c][0]), 0, 1980)) lim = 1;
                     if (check_limit(al_fixtoi(Efi[c][1]), 0, 1980)) lim = 1;
                  }
                  else // adjust if out of bounds
                  {
                     Efi[c][0] = al_itofix(enforce_limit(al_fixtoi(Efi[c][0]), 0, 1980));
                     Efi[c][1] = al_itofix(enforce_limit(al_fixtoi(Efi[c][1]), 0, 1980));
                  }

                  if (Ei[c][0] == 7 ) // podzilla trigger box
                  {
                     Ei[c][11]+= x3/20;
                     Ei[c][12]+= y3/20;
                     Ei[c][13]+= x3/20;
                     Ei[c][14]+= y3/20;
                  }
                  if (Ei[c][0] == 9 ) // cloner
                  {
                     Ei[c][11]+= x3/20;
                     Ei[c][12]+= y3/20;
                     Ei[c][13]+= x3/20;
                     Ei[c][14]+= y3/20;

                     Ei[c][15]+= x3/20;
                     Ei[c][16]+= y3/20;
                     Ei[c][17]+= x3/20;
                     Ei[c][18]+= y3/20;
                  }
                  if (lim)
                  {
                     //copied = -1;
                     for (y=0; y<32; y++) Ei[c][y] = 0;
                     for (y=0; y<16; y++) Efi[c][y] = al_itofix(0);
                  }
                  c = 100; // end loop
               } // end of found empty
            }  // end if iterate real enemy array

//            #ifdef LOGGING_ZFS
//            sprintf(msg,"dfc attempt to copy enemy:%d - ", b);
//            add_log_entry(msg);
//            if (copied >= 1000)  sprintf(msg,"found empty enemy:%d\n", copied-1000);
//            if (copied == 0)  sprintf(msg,"copy aborted - no empty enemies found\n");
//            if (copied == -1) sprintf(msg,"copy aborted - limits exceeded\n");
//            add_log_entry(msg);
//            #endif

         } // end of attempt copy
      } // end of iterate enemies in ft
   } // end of if copy enemies


   if (copy_items)
   {
      for (b=0; b<500; b++)       // iterate items in selection
      {
         if (ft_item[b][0])       // if active attempt copy
         {
            //int copied = 0;
            for (c=0; c<500; c++) // search for empty place to copy to
            {
               if (item[c][0] == 0) // found empty
               {
                  //copied = 1000+c;
                  int lim = 0;
                  // copy all 16 variables
                  for (y=0; y<16; y++)
                        item[c][y] = ft_item[b][y];

                  // apply offsets
                  item[c][4] += x3;
                  item[c][5] += y3;

                  if (erase_out_of_bounds_main)
                  {
                     if (check_limit(item[c][4], 0, 1980)) lim = 1;
                     if (check_limit(item[c][5], 0, 1980)) lim = 1;
                  }
                  else // adjust if out of bounds
                  {
                     item[c][4] = enforce_limit(item[c][4], 0, 1980);
                     item[c][5] = enforce_limit(item[c][5], 0, 1980);
                  }

                  if (item[c][0] == 4) // // set new key block range
                  {
                     // apply offsets
                     item[c][6] += qx1;
                     item[c][7] += qy1;
                     item[c][8] += qx1;
                     item[c][9] += qy1;

                     if (erase_out_of_bounds_secondary)
                     {
                        if (check_limit(item[c][6], 0, 99)) lim = 1;
                        if (check_limit(item[c][7], 0, 99)) lim = 1;
                        if (check_limit(item[c][8], 0, 99)) lim = 1;
                        if (check_limit(item[c][9], 0, 99)) lim = 1;
                     }
                     else // adjust if out of bounds
                     {
                        item[c][6] = enforce_limit(item[c][6], 0, 99);
                        item[c][7] = enforce_limit(item[c][7], 0, 99);
                        item[c][8] = enforce_limit(item[c][8], 0, 99);
                        item[c][9] = enforce_limit(item[c][9], 0, 99);
                     }

                  }
                  if (item[c][0] == 10) // message
                  {
                     // apply offsets
                     item[c][10] += qx1;
                     item[c][11] += qy1;

                     if (erase_out_of_bounds_secondary)
                     {
                        if (check_limit(item[c][10], 0, 99)) lim = 1;
                        if (check_limit(item[c][11], 0, 99)) lim = 1;
                     }
                     else // adjust if out of bounds
                     {
                        item[c][10] = enforce_limit(item[c][10], 0, 99);
                        item[c][11] = enforce_limit(item[c][11], 0, 99);
                     }


                     free (pmsg[c]);
                     pmsg[c] = (char*) malloc (strlen(ft_pmsg[b])+1);
                     strcpy(pmsg[c], ft_pmsg[b]);
                  }
                  // limits exceeded; erase
                  if (lim)
                  {
                     //copied = -1;
                     for (y=0; y<16; y++) item[c][y] = 0;
                  }
                  c = 500; // end loop
               } // end of found empty
            }  // end if iterate real item array

//            #ifdef LOGGING_ZFS
//            sprintf(msg,"dfc attempt to copy item:%d - ", b);
//            add_log_entry(msg);
//            if (copied >= 1000) sprintf(msg,"found empty item:%d\n", copied-1000);
//            if (copied == 0)    sprintf(msg,"copy aborted - no empty items found\n");
//            if (copied == -1)   sprintf(msg,"copy aborted - limits exceeded\n");
//            add_log_entry(msg);
//            #endif

         } // end of attempt copy
      } // end of iterate items ft
   } // end of if copy items
   sort_enemy();
   sort_item();
   draw_big(1);
}




void do_rnd(void)
{
   extern int stx, sty, sux, suy;
   int x1 = stx *20;  // source box
   int y1 = sty *20;
   int x2 = sux *20;
   int y2 = suy *20;

   al_fixed fx1 = al_itofix(x1);
   al_fixed fy1 = al_itofix(y1);
   al_fixed fx2 = al_itofix(x2);
   al_fixed fy2 = al_itofix(y2);

   int x, y;


/*

   for (int b=0; b<500; b++) // check for items in box
      if (item[b][0])     // if active
         if (item[b][4] >= x1)
            if (item[b][4] < x2)
               if (item[b][5] >= y1)
                  if (item[b][5] < y2)
                  {

                        if (1) // initial position random
                        {
                           int empty = 0;
                           int lb = 0;
                           while ((!empty) && (lb < 10000))
                           {
                              lb++;
                              x = rand() % 100;
                              y = rand() % 100;

                              empty = 1;
                              if (1) // confine to selection window
                              {
                                 if ((x < stx) || (x >= sux)) empty = 0;
                                 if ((y < sty) || (y >= suy)) empty = 0;
                              }

                              if (!is_block_empty(x, y, 1, 1, 1)) empty = 0;

                           }
                           if (empty)
                           {
                              item[b][4] = x*20;
                              item[b][5] = y*20;
                           }
                           else printf("could not find empty\n");

                        }
                  }

*/

   for (int b=0; b<100; b++) // check for enemies in box
      if (Ei[b][0])     // if active
         if (Efi[b][0] >= fx1)
            if (Efi[b][0] < fx2)
               if (Efi[b][1] >= fy1)
                  if (Efi[b][1] < fy2)
                  {

/*

                        if (1) // initial position random
                        {
                           int empty = 0;
                           int lb = 0;
                           while ((!empty) && (lb < 10000))
                           {
                              lb++;
                              x = rand() % 100;
                              y = rand() % 100;

                              empty = 1;
                              if (1) // confine to selection window
                              {
                                 if ((x < stx) || (x >= sux)) empty = 0;
                                 if ((y < sty) || (y >= suy)) empty = 0;
                              }

                              if (!is_block_empty(x, y, 1, 1, 1)) empty = 0;

                           }
                           if (empty)
                           {
                              Efi[b][0] = al_itofix(x)*20;
                              Efi[b][1] = al_itofix(y)*20;
                           }
                           else printf("could not find empty\n");


                        }













                     if (Ei[b][0] == 3) // if archwag
                     {
                        printf("randomizing archwag:%d\n", b);

                        //  x speed
                        int s = 50 + rand() % 1000;
                        Efi[b][2] = al_itofix(s)/100;

                        //  y speed
                        s = 200 + rand() % 900;
                        Efi[b][3] = al_itofix(s)/100;

                        //  bullet speed
                        s = 100 + rand() % 900;
                        Efi[b][7] = al_itofix(s)/100;

                        Ei[b][15] = 10;  // bullet retrigger value
                        Ei[b][17] = 500; // bullet prox

                     }


                     if (Ei[b][0] == 4) // if bouncer
                     {
                        printf("randomizing bouncer:%d\n", b);

                        //  seek speed
                        int s = 150 + rand() % 700;
                        Efi[b][5] = al_itofix(s)/100;

                        // initial direction
                        int x = rand() % 2000;
                        int y = rand() % 2000;
                        seek_set_xyinc(b, x, y);
                     }

*/
                     if (Ei[b][0] == 6) // if cannon
                     {
                        printf("randomizing cannon:%d\n", b);

                        if (1) // initial position random
                        {
                           int empty = 0;
                           while (!empty)
                           {
                              x = rand() % 100;
                              y = rand() % 100;
                              if (l[x][y] < 32) empty = 1;
                              if (1) // confine to selection window
                              {
                                 if ((x < stx) || (x >= sux)) empty = 0;
                                 if ((y < sty) || (y >= suy)) empty = 0;
                              }
                           }
                           Efi[b][0] = al_itofix(x)*20;
                           Efi[b][1] = al_itofix(y)*20;
                        }
/*
                        if (0) // seek speed random
                        {
                           int s = 150 + rand() % 700;
                           Efi[b][5] = al_itofix(s)/100;
                        }


                        if (0) // initial direction random
                        {
                           x = rand() % 2000;
                           y = rand() % 2000;
                           seek_set_xyinc(b, x, y);
                        }

                        if (0) // initial direction start block
                        {
                           for (int c=0; c<500; c++)
                              if (item[c][0] == 5)
                              {
                                 x = item[c][4];
                                 y = item[c][5];
                              }
                           seek_set_xyinc(b, x, y);
                        }

                        if (0) // bullet speed random
                        {
                           int s = 200 + rand() % 900;
                           Efi[b][7] = al_itofix(s)/100;
                        }


                        if (0) // bullet retrigger
                        {
                           int s = 5000 + rand() % 6000;
                           Ei[b][15] = s/100;
                        }

*/

                     }





/*



                     if (Ei[b][0] == 8)
                     {
                        printf("randomizing trakbot:%d\n", b);

                        //  x speed
                        int s = 50 + rand() % 1000;
                        Efi[b][2] = al_itofix(s)/100;

                        //  y speed
                        s = 200 + rand() % 900;
                        Efi[b][3] = al_itofix(s)/100;

                        //  bullet speed
                        s = 100 + rand() % 900;
                        Efi[b][7] = al_itofix(s)/100;

                        Ei[b][15] = 80;  // bullet retrigger value




                     }


                     if (Ei[b][0] == 12)
                     {
                        printf("randomizing flapper:%d\n", b);

                        //  y speed
                        int s = 200 + rand() % 900;
                        Efi[b][3] = al_itofix(s)/100;


                        //  max x speed
                        s = 50 + rand() % 1000;
                        Efi[b][5] = al_itofix(s)/100;

                        //  x accel .1 to 2
                        s = 10 + rand() % 200;
                        Efi[b][6] = al_itofix(s)/100;


                        //  flap yinc scaler .1 to 3
                        s = 10 + rand() % 300;
                        Efi[b][8] = al_itofix(s)/100;



                        //  bullet speed
                        s = 100 + rand() % 900;
                        Efi[b][7] = al_itofix(s)/100;


                        Ei[b][15] = 80;  // bullet retrigger value


                     }
*/

                  }
   sort_enemy();
   sort_item();

}

void do_clear(void)
{
   int b, y;

   int x1 = stx *20;  // source box
   int y1 = sty *20;
   int x2 = sux *20;
   int y2 = suy *20;

   al_fixed fx1 = al_itofix(x1);
   al_fixed fy1 = al_itofix(y1);
   al_fixed fx2 = al_itofix(x2);
   al_fixed fy2 = al_itofix(y2);

   if (copy_blocks) // erase blocks
      for (b=stx; b<sux; b++)
         for (y=sty; y<suy; y++) l[b][y]=0;

   if (copy_items)  // erase items
      for (b=0; b<500; b++)
         if (item[b][0])
            if ((item[b][4] >= x1) && (item[b][4] < x2))
               if ((item[b][5] >= y1) && (item[b][5] < y2)) erase_item(b);

   if (copy_enemies)  // erase enemies
      for (b=0; b<100; b++)
         if (Ei[b][0])
            if ((Efi[b][0] >= fx1) && (Efi[b][0] < fx2))
               if ((Efi[b][1] >= fy1) && (Efi[b][1] < fy2))
               {
                  for (y=0; y<32; y++) Ei[b][y] = 0;
                  for (y=0; y<16; y++) Efi[b][y] = al_itofix(0);
               }

   if (copy_lifts) // erase lifts
      for (b=num_lifts-1; b>=0; b--) // have to iterate backwards because erase_lift() does a resort after every erase
         if ((lifts[b].x1 >= x1) && (lifts[b].x1 < x2))
            if ((lifts[b].y1 >= y1) && (lifts[b].y1 < y2)) erase_lift(b);

   sort_item();
   sort_enemy();
   draw_big(1);
}

void draw_fsel(void)
{
   int ft_w = ft_level_header[8]*db;
   int ft_h = ft_level_header[9]*db;
   int t_w = ft_level_header[8]*20;
   int t_h = ft_level_header[9]*20;
   int a,d,x,y;

   ALLEGRO_BITMAP *temp = NULL;
   temp = al_create_bitmap(t_w, t_h);
   al_set_target_bitmap(temp);
   al_clear_to_color(al_map_rgb(0,0,0));

   if (copy_blocks)
      for (x=0; x<ft_level_header[8]; x++)
         for (y=0; y<ft_level_header[9]; y++)
            if ((ft_l[x][y] < NUM_SPRITES) && (ft_l[x][y] > 0))
               al_draw_bitmap(memory_bitmap[ft_l[x][y]], x*20, y*20, 0);

   if (copy_enemies)
      for (x=0; x<100; x++)
         if (ft_Ei[x][0])
         {
            int a, b;
            int ex = al_fixtoi(ft_Efi[x][0]);
            int ey = al_fixtoi(ft_Efi[x][1]);

            a = ft_Ei[x][1]; // bmp or ans
            if (a < NUM_SPRITES) b = a; // bmp
            if (a > 999) b = zz[5][a-1000]; // ans
            al_draw_bitmap(memory_bitmap[b], ex, ey, 0);
         }
   if (copy_items)
      for (x=0; x<500; x++)
         if (ft_item[x][0])
         {
            int b;
            int ex = ft_item[x][4];
            int ey = ft_item[x][5];
            b = ft_item[x][1]; // bmp or ans
            if (b > 1000) b = zz[0][b-1000]; // ans
            al_draw_bitmap(memory_bitmap[b], ex, ey, 0);
         }
   if (copy_lifts)
      for (d=0; d<ft_level_header[5]; d++)
      {
         int color = ft_lift[d][2];
         int x1 = ft_ls[d][0][0];
         int y1 = ft_ls[d][0][1];
         int x2 = ft_ls[d][0][0] + (ft_lift[d][0] * 20)-1;
         int y2 = ft_ls[d][0][1] + (ft_lift[d][1] * 20)-1;
         int tx = ((x1+x2)/2) ;
         int ty = ((y1+y2)/2) -2;
         for (a=0; a<10; a++)
           al_draw_rectangle(x1+a, y1+a, x2-a, y2-a, palette_color[color+((9-a)*16)], 1 );
         al_draw_filled_rectangle(x1+a, y1+a, x2-a, y2-a, palette_color[color]);
         al_draw_text(font, palette_color[color+160], tx, ty, ALLEGRO_ALIGN_CENTER, ft_ln[d]);
      }
   al_destroy_bitmap(ft_bmp);
   ft_bmp = al_create_bitmap(ft_w, ft_h);
   al_set_target_bitmap(ft_bmp);
   al_clear_to_color(al_map_rgb(0,0,0));
   al_draw_scaled_bitmap(temp, 0, 0, t_w, t_h, 0, 0, ft_w, ft_h, 0);
   al_destroy_bitmap(temp);
}


















