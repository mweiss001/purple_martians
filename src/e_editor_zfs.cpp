// e_editor_zfs.cpp

#include "pm.h"

int copy_blocks=1;
int copy_enemies=1;
int copy_items=1;
int copy_lifts=1;
int copy_flags=1;
int copy_mode = 0;
int brf_mode =0;

int ft_level_header[20];
int ft_l[100][100];
int ft_item[500][16];
char ft_pmsgtext[500][500] = {0};


int ft_Ei[100][32];
al_fixed ft_Efi[100][16];

char ft_ln[NUM_LIFTS][80];
int ft_lift[NUM_LIFTS][6];
int ft_ls[NUM_LIFTS][40][6];


void zfs_pointer_text(int mx, int my, int x1, int x2, int y, int mouse_on_window)
{
   int xc = (x1+x2)/2;
   if ((mx<99) && (my < 99))
   {
      al_draw_text( font, palette_color[15], xc, y+2,  ALLEGRO_ALIGN_CENTER, "Pointer");

      if (!mouse_on_window) al_draw_textf(font, palette_color[15], xc, y+11, ALLEGRO_ALIGN_CENTER, "  x:%d    y:%d ", mx, my);
      else                  al_draw_text( font, palette_color[15], xc, y+11, ALLEGRO_ALIGN_CENTER, "  x:--    y:-- ");

      al_draw_rectangle(x1, y+0, x2, y+20, palette_color[15], 1);
   }
   int rx1 = stx *20;    // source x
   int ry1 = sty *20;    // source y
   int rx2 = sux *20;    // sizes
   int ry2 = suy *20;
   int eib=0;
   int iib=0;
   int lib=0;
   al_fixed frx1 = al_itofix(rx1); // source position and size
   al_fixed fry1 = al_itofix(ry1);
   al_fixed frx2 = al_itofix(rx2);
   al_fixed fry2 = al_itofix(ry2);

   // count enemies in box
   for (int b=0; b<100; b++)
      if (Ei[b][0])
         if (Efi[b][0] >= frx1)
            if (Efi[b][0] < frx2)
               if (Efi[b][1] >= fry1)
                  if (Efi[b][1] < fry2)
                     eib++;

   // count items in box
   for (int b=0; b<500; b++)
      if (item[b][0])
         if (item[b][4] >= rx1)
            if (item[b][4] < rx2)
               if (item[b][5] >= ry1)
                  if (item[b][5] < ry2)
                     iib++;

   // count lifts in box
   for (int d=0; d<num_lifts; d++)
      if (lifts[d].x1 >= rx1)
         if (lifts[d].x1 < rx2)
            if (lifts[d].y1 >= ry1)
               if (lifts[d].y1 < ry2)
                  lib++;


   y+=24;

   al_draw_rectangle(x1, y, x2, y+10, palette_color[14], 1);
   al_draw_rectangle(x1, y, x2, y+36, palette_color[14], 1);
   al_draw_rectangle(x1, y, x2, y+62, palette_color[14], 1);


   al_draw_text( font, palette_color[6], xc, y+1,  ALLEGRO_ALIGN_CENTER, "Selection");
   al_draw_textf(font, palette_color[6], xc, y+11, ALLEGRO_ALIGN_CENTER, " x:%2d  y:%2d ", stx, sty);
   al_draw_textf(font, palette_color[6], xc, y+19, ALLEGRO_ALIGN_CENTER, " width:%d ",  sux-stx);
   al_draw_textf(font, palette_color[6], xc, y+27, ALLEGRO_ALIGN_CENTER, " height:%d ", suy-sty);

   al_draw_textf(font, palette_color[7], xc, y+37, ALLEGRO_ALIGN_CENTER, " %d Enemies ", eib);
   al_draw_textf(font, palette_color[7], xc, y+45, ALLEGRO_ALIGN_CENTER, " %d Items ", iib);
   al_draw_textf(font, palette_color[7], xc, y+53, ALLEGRO_ALIGN_CENTER, " %d Lifts ", lib);
}

void zfs_do_brf(int x, int y, int flood_block)
{
   int show_progress = 1;
   int f[100][100] = {0};   // array of blocks to mark
   int rb = l[x][y];        // block num to replace
   f[x][y] = 1;             // mark initial block pos in array

   int times=0, found=0;
   do
   {
      times++;
      found = 0;
      for (int a=0; a<100; a++)
         for (int b=0; b<100; b++)
            if (f[a][b]) // iterate already marked
            {
               if ((a >  0) && (l[a-1][b]) == rb) // look left
               {
                  if (f[a-1][b] == 0) found++; // found unmarked
                  f[a-1][b] = 1; // mark it
               }
               if ((b >  0) && (l[a][b-1]) == rb) // look up
               {
                  if (f[a][b-1] == 0) found++; // found unmarked
                  f[a][b-1] = 1; // mark it
               }
               if ((a < 99) && (l[a+1][b]) == rb) // look right
               {
                  if (f[a+1][b] == 0) found++; // found unmarked
                  f[a+1][b] = 1; // mark it
               }
               if ((b < 99) && (l[a][b+1]) == rb) // look down
               {
                  if (f[a][b+1] == 0) found++; // found unmarked
                  f[a][b+1] = 1; // mark it
               }
            }

      if (show_progress)
      {
         // just because I can and it looks cool
         //printf("times:%d found:%d\n", times, found);
         for (int a1=0; a1<100; a1++)
            for (int b1=0; b1<100; b1++)
               if (f[a1][b1]) l[a1][b1] = flood_block;

         init_level_background();
         zfs_redraw_background(0, 0);
         al_rest(.04);

      }
   } while (found);
   if (!show_progress)
   {
      // or we could just do it instantly at the end
      for (int a=0; a<100; a++)
         for (int b=0; b<100; b++)
            if (f[a][b]) l[a][b] = flood_block;
      init_level_background();
   }
}

void zfs_clear_ft(void)
{
   for (int x=0; x<20; x++) // level_header
      ft_level_header[x] = 0;
   for (int x=0; x<100; x++) // blocks
      for (int y=0; y<100; y++) ft_l[x][y] = 0;
   for (int c=0; c<500; c++) // items
   {
      ft_pmsgtext[c][0] = 0;
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
     for (int x=0; x<6; x++) // lift var
        ft_lift[l][x] = 0;
     for (int s=0; s<40; s++) // step
        for (int x=0; x<4; x++) // step var
           ft_ls[l][s][x] = 0;
   }
}

int zfs_load_selection(void)
{
   FILE *filepntr;
   int loop, ch, c, x, y;
   char buff[2000];
   sprintf(sel_filename,"sel\\");
   if (mw_file_select("Load Selection", sel_filename, ".sel", 0))
   {
      zfs_clear_ft();
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
         if (ft_item[c][0] == 10) // get pop msg
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
            strcpy(ft_pmsgtext[c], buff);
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

         // get lift data
         for (y=0; y<6; y++)
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
            for (y=0; y<6; y++)
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

void zfs_save_selection(int save)
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

   zfs_clear_ft();

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

            if ((item[b][0] == 4) || (item[b][0] == 9) || (item[b][0] == 16) || (item[b][0] == 17)) // key, trigger, manip, damage
            {   // set new destination
               ft_item[c][6] = item[b][6] - stx*20;
               ft_item[c][7] = item[b][7] - sty*20;
            }

            if (item[b][0] == 10) // message
            {
               ft_item[c][10] = item[b][10] - stx*20;
               ft_item[c][11] = item[b][11] - sty*20;
               strcpy(ft_pmsgtext[c], pmsgtext[b]);
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
               ft_Ei[c][11]-= x1;
               ft_Ei[c][12]-= y1;
            }
            if (ft_Ei[c][0] == 9 ) // cloner
            {
               ft_Ei[c][11]-= x1;
               ft_Ei[c][12]-= y1;
               ft_Ei[c][15]-= x1;
               ft_Ei[c][16]-= y1;
               ft_Ei[c][17]-= x1;
               ft_Ei[c][18]-= y1;
            }
            if (ft_Ei[c][0] == 10 ) // field
            {
               ft_Ei[c][11]-= x1;
               ft_Ei[c][12]-= y1;
               ft_Ei[c][15]-= x1;
               ft_Ei[c][16]-= y1;
            }
         }
   if (copy_lifts)
      for (b=0; b<num_lifts; b++) // source, if in selection
         if ((lifts[b].x1 >= x1) && (lifts[b].x1 < x2) && (lifts[b].y1 >= y1) && (lifts[b].y1 < y2))
         {
            c = lib++; // destination

            strcpy(ft_ln[c], lifts[b].lift_name);

            ft_lift[c][0] = lifts[b].mode;
            ft_lift[c][1] = lifts[b].flags;
            ft_lift[c][2] = lifts[b].color;
            ft_lift[c][3] = lifts[b].num_steps;
            ft_lift[c][4] = lifts[b].val1;
            ft_lift[c][5] = lifts[b].val2;


            for (y = 0; y < lifts[b].num_steps; y++) // copy steps
            {
               int vx = lift_steps[b][y].x;
               int vy = lift_steps[b][y].y;
               if ((lift_steps[b][y].type & 31) == 1) // shift move steps
               {
                  vx -= x1;
                  vy -= y1;
               }
               ft_ls[c][y][0] = vx;
               ft_ls[c][y][1] = vy;
               ft_ls[c][y][2] = lift_steps[b][y].w;
               ft_ls[c][y][3] = lift_steps[b][y].h;
               ft_ls[c][y][4] = lift_steps[b][y].val;
               ft_ls[c][y][5] = lift_steps[b][y].type;
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
      sprintf(sel_filename, "sel\\");
      if (mw_file_select("Save Selection", sel_filename, ".sel", 1))
      {
         //printf("fn:%s\n", sel_filename);

         FILE * filepntr = fopen(sel_filename,"w");
         for (x=0; x<20; x++)
            fprintf(filepntr,"%d\n",ft_level_header[x]);
         for (c=0; c<(sux-stx); c++)  // selection of blocks
            for (x=0; x<(suy-sty); x++)
               fprintf(filepntr,"%d\n",ft_l[c][x]);

         for (c=0; c < ft_level_header[3]; c++) // items
         {
            for (x=0; x<16; x++)
               fprintf(filepntr,"%d\n",ft_item[c][x]);

            if (ft_item[c][0] == 10) // pop msg
            {
               y = 0;
               while (ft_pmsgtext[c][y] != (char)NULL)
               {
                  if (ft_pmsgtext[c][y] == 13) fprintf(filepntr,"%c", 126);
                  else fprintf(filepntr,"%c",ft_pmsgtext[c][y]);
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
            for (x=0; x<6; x++) fprintf(filepntr,"%d\n",ft_lift[c][x]);
            for (x=0; x<ft_lift[c][3]; x++)
               for (y=0; y<6; y++) fprintf(filepntr,"%d\n",ft_ls[c][x][y]);
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

void zfs_do_fcopy(int qx1, int qy1)
{
   int b, c, x, y;
   int x3 = qx1*20;         // dest x
   int y3 = qy1*20;         // dest y
   int x5 = ft_level_header[8];
   int y5 = ft_level_header[9];


   int erase_out_of_bounds_main = 0;      // if 0 we will adjust
   //int erase_out_of_bounds_secondary = 0; // if 0 we will adjust

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
            int lim = 0;
            c = num_lifts++; // dest lift
            //copied = 1000+c;
            construct_lift(c, ft_ln[b]);
            lifts[c].mode      = ft_lift[b][0];
            lifts[c].flags     = ft_lift[b][1];
            lifts[c].num_steps = ft_lift[b][3];
            lifts[c].val1      = ft_lift[b][4];
            lifts[c].val2      = ft_lift[b][5];


            for (y=0; y<ft_lift[b][3]; y++) // copy steps
            {
               int vx   = ft_ls[b][y][0];
               int vy   = ft_ls[b][y][1];
               int vw   = ft_ls[b][y][2];
               int vh   = ft_ls[b][y][3];
               int val  = ft_ls[b][y][4];
               int type = ft_ls[b][y][5];

               if ((type & 31) == 1) // shift  move steps
               {
                  // apply offsets
                  vx += x3;
                  vy += y3;

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

               //printf("contructing step:%d\n", y);
               construct_lift_step(c, y, type, vx, vy, vw, vh, val);
            }
            set_lift_to_step(c, 0);
            if (lim)
            {
               erase_lift(c);
               //copied = -1;
            }
         }
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
                  if (Ei[c][0] == 7) // podzilla trigger box
                  {
                     Ei[c][11]+= x3;
                     Ei[c][12]+= y3;
                  }
                  if (Ei[c][0] == 9) // cloner
                  {
                     Ei[c][11]+= x3;
                     Ei[c][12]+= y3;
                     Ei[c][15]+= x3;
                     Ei[c][16]+= y3;
                     Ei[c][17]+= x3;
                     Ei[c][18]+= y3;
                  }
                  if (Ei[c][0] == 10) // field
                  {
                     Ei[c][11]+= x3;
                     Ei[c][12]+= y3;
                     Ei[c][15]+= x3;
                     Ei[c][16]+= y3;
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
         } // end of attempt copy
      } // end of iterate enemies in ft
   } // end of if copy enemies


   if (copy_items)
   {

      // this section is to make any copied pm_event links have new unique pm_events and still linked properly
      int clt[500][4] = { 0 };
      int clt_last = 0; // index

      for (b=0; b<500; b++)       // iterate items in selection
      {
         if ((ft_item[b][0] == 16) || (ft_item[b][0] == 17)) // manip or block
         {
            clt_last += add_item_link_translation(b, 1, ft_item[b][1], clt, clt_last);
         }

         if (ft_item[b][0] == 9) // trigger
         {
            clt_last += add_item_link_translation(b, 11, ft_item[b][11], clt, clt_last);
            clt_last += add_item_link_translation(b, 12, ft_item[b][12], clt, clt_last);
            clt_last += add_item_link_translation(b, 13, ft_item[b][13], clt, clt_last);
            clt_last += add_item_link_translation(b, 14, ft_item[b][14], clt, clt_last);
         }
      }

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

                  // does this copy item have an entry in the clt table?
                  for (int i=0; i<clt_last; i++)
                     if (clt[i][0] == b) // found index of source item table
                     {
                        int var_index = clt[i][1]; // var #
                        int ev2 = clt[i][3];       // new ev

                        item[c][var_index] = ev2;
                     }

                  if ((item[c][0] == 4) || (item[c][0] == 9) || (item[c][0] == 16) || (item[c][0] == 17)) // key, trigger, manip, damage
                  {
                     item[c][6] += qx1*20;
                     item[c][7] += qy1*20;
                  }
                  if (item[c][0] == 5) // start
                  {
                     // do something here to prevent exact duplicates
                  }
                  if (item[c][0] == 10) // message
                  {
                     item[c][10] += qx1*20; // adjust msg location
                     item[c][11] += qy1*20;
                     strcpy(pmsgtext[c], ft_pmsgtext[b]);
                  }
                  // limits exceeded; erase
                  if (lim)
                  {
                     erase_item(c);
                     //copied = -1;
                  }
                  c = 500; // end loop
               } // end of found empty
            }  // end if iterate real item array
         } // end of attempt copy
      } // end of iterate items ft
   } // end of if copy items
   sort_enemy();
   sort_item();
}

void zfs_do_clear(void)
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

}

void zfs_draw_selection_filters(int x1, int y1, int x2)
{
   int a=0, bnh=16, fs=12;
   int xc = (x1+x2)/2;

   // draw frame around filter buttons
   int y2 = y1+5*bnh+fs*2;
   int ci = 16; //color inc
   for (int q=0; q<fs; q++)
      al_draw_rectangle(x1+q, y1+q, x2-q, y2-q, palette_color[12+32+(q*ci)], 1);
   al_draw_text(font, palette_color[15], xc, y1+2, ALLEGRO_ALIGN_CENTER, "Selection Filters");

   int yfb = y1+fs;
   x1+=fs;
   x2-=fs;

   if (mdw_toggle(x1, yfb+a*bnh, x2, yfb+(a+1)*bnh-2, 1000,0,0,0,0,0,0,0,1,0,0,0, copy_blocks, "Copy Blocks", "Copy Blocks", 15, 15, 15+64, 9)) // block filter
      if (copy_mode) zfs_draw_fsel();
   a++;
   if (mdw_toggle(x1, yfb+a*bnh, x2, yfb+(a+1)*bnh-2, 1000,0,0,0,0,0,0,0,1,0,0,0, copy_flags, "Copy Flags", "Copy Flags", 15, 15, 15+64, 9)) // flags filter
      if (copy_mode) zfs_draw_fsel();
   a++;
   if (mdw_toggle(x1, yfb+a*bnh, x2, yfb+(a+1)*bnh-2, 1000,0,0,0,0,0,0,0,1,0,0,0, copy_items, "Copy Items", "Copy Items", 15, 15, 15+64, 9)) // item filter
      if (copy_mode) zfs_draw_fsel();
   a++;
   if (mdw_toggle(x1, yfb+a*bnh, x2, yfb+(a+1)*bnh-2, 1000,0,0,0,0,0,0,0,1,0,0,0, copy_enemies, "Copy Enemies", "Copy Enemies", 15, 15, 15+64, 9)) // enemy filter
      if (copy_mode) zfs_draw_fsel();
   a++;
   if (mdw_toggle(x1, yfb+a*bnh, x2, yfb+(a+1)*bnh-2, 1000,0,0,0,0,0,0,0,1,0,0,0, copy_lifts, "Copy Lifts", "Copy Lifts", 15, 15, 15+64, 9)) // lifts filter
      if (copy_mode) zfs_draw_fsel();
   a++;
}

void zfs_draw_buttons(int x3, int x4, int yfb)
{
   int bnh = 16;
   int a=0, col=0;
   if (mdw_button(x3, yfb+a*bnh, x4, yfb+(a+1)*bnh-2, 620, 0,  0, 0, 0,  9, 15, 0, 1,0,0,0)) // move selection
   {
      if (copy_mode) copy_mode = 0;
      else
      {
         copy_mode = 1;
         zfs_save_selection(0); // just puts in ft_
         zfs_draw_fsel();
         zfs_do_clear();
      }
   }
   a++;
   if (mdw_button(x3, yfb+a*bnh, x4, yfb+(a+1)*bnh-2, 621, 0,  0, 0, 0,  9, 15, 0, 1,0,0,0)) zfs_do_clear(); // clear selection
   a++;
   col = 9;
   if (copy_mode) col = 10;
   if (mdw_button(x3, yfb+a*bnh, x4, yfb+(a+1)*bnh-2, 622, 0,  0, 0, 0,  col, 15, 0, 1,0,0,0)) // paste selection
   {
      if (copy_mode) copy_mode = 0;
      else
      {
         copy_mode = 1;
         zfs_save_selection(0); // just puts in ft_
         zfs_draw_fsel();
      }
   }
   a+=2;
   if (mdw_button(x3, yfb+a*bnh, x4, yfb+(a+1)*bnh-2, 623, 0,  0, 0, 0,  9, 15, 0, 1,0,0,0)) zfs_save_selection(1); // puts in ft_ and saves to disk
   a++;
   if (mdw_button(x3, yfb+a*bnh, x4, yfb+(a+1)*bnh-2, 624, 0,  0, 0, 0,  9, 15, 0, 1,0,0,0)) // load from disk
   {
      if (zfs_load_selection())
      {
         copy_mode = 1;
         zfs_draw_fsel();
      }
   }
   a+=2;
   if (draw_item_type == 1) // don't even show these 3 buttons unless draw item type is block
   {
      if (mdw_button(x3, yfb+a*bnh, x4, yfb+(a+1)*bnh-2, 610, 0,  0, 0, 0,  9, 15, 0, 1,0,0,0)) // block fill
      {
         for (int x=stx; x<sux; x++)
            for (int y=sty; y<suy; y++)
            {
               if ((copy_flags) && (copy_blocks))  l[x][y] = draw_item_num;
               if ((copy_flags) && (!copy_blocks))
               {
                  int flags = draw_item_num & PM_BTILE_MOST_FLAGS; // get only flags from draw item
                  l[x][y] &= ~PM_BTILE_MOST_FLAGS;                 // clear flags in destination
                  l[x][y] |= flags;                                // merge
               }
            }
         init_level_background();
      }
      a++;
      if (mdw_button(x3, yfb+a*bnh, x4, yfb+(a+1)*bnh-2, 611, 0,  0, 0, 0,  9, 15, 0, 1,0,0,0)) // block frame
      {
         for (int x=stx; x<sux; x++)
         {
            l[x][sty] = draw_item_num;
            l[x][suy-1] = draw_item_num;
         }
         for (int y=sty; y<suy; y++)
         {
            l[stx][y] = draw_item_num;
            l[sux-1][y] = draw_item_num;
         }
         init_level_background();
      }
      a++;

      col = 9;
      if (brf_mode) col = 10;
      if (mdw_button(x3, yfb+a*bnh, x4, yfb+(a+1)*bnh-2, 612, 0,  0, 0, 0,  col, 15, 0, 1,0,0,0)) // block floodfill
         brf_mode = !brf_mode;
      a++;
   }
}

void zfs_proc_window_move(int *x1, int *y1, int *x2, int *y2, int w, int h)
{  // move window by dragging title bar
   if ((mouse_x > *x1) && (mouse_x < *x2) && (mouse_y > *y1) && (mouse_y < *y1+15))
   {
      if (mouse_b1)
      {
         int mxo = mouse_x - *x1; // get offset from mouse position to window x, y
         int myo = mouse_y - *y1;

         while (mouse_b1)
         {
            *x1 = mouse_x - mxo;
            *y1 = mouse_y - myo;
            *x2 = *x1 + w;
            *y2 = *y1 + h;

            zfs_redraw_background(0, 0);
            zfs_redraw_window(*x1, *y1, *x2, *y2, 0, 0);
         }
      }
   }
}

void zfs_draw_fsel(void)
{
   if (copy_blocks)
   {
      // do this to remove any other drawing on the level buffer
      get_new_background(0);
   }
   else
   {
      // do this to make the blocks transparent by making the level_buffer transparent
      al_set_target_bitmap(level_buffer);
      al_clear_to_color(al_map_rgba(0,0,0,0)); // transparent zero pixels
   }

   if (copy_items) draw_items();
   if (copy_enemies) draw_enemies();
   if (copy_lifts) draw_lifts();

   int sw = sux-stx;
   int sh = suy-sty;
   al_destroy_bitmap(ft_bmp);
   ft_bmp = al_create_bitmap(sw*20, sh*20);
   al_set_target_bitmap(ft_bmp);
   al_clear_to_color(al_map_rgba(0,0,0,0));
   al_draw_bitmap_region(level_buffer, stx*20, sty*20, sw*20, sh*20, 0, 0, 0);
}

void zfs_show_level_buffer_block_rect(int x1, int y1, int x2, int y2, int color, const char * text)
{
   int dstx = x1*20;
   if (dstx == 0) dstx = 1;
   int dsty = y1*20;
   if (dsty == 0) dsty = 1;
   al_draw_rectangle(dstx, dsty, (x2*20)-1, (y2*20)-1, palette_color[color], 1);
   al_draw_text(font, palette_color[color], x1*20+2, y1*20-11,  0, text);
}

void zfs_redraw_background(int gx, int gy)
{
   al_flip_display();
   proc_scale_factor_change();
   proc_controllers();
   proc_frame_delay();
   get_new_background(0);
   draw_lifts();
   draw_items();
   draw_enemies();

   if (brf_mode) crosshairs_full(gx*20+10, gy*20+10, 15, 1);
   else zfs_show_level_buffer_block_rect(stx, sty, sux, suy, 14, "selection");

   if (copy_mode)
   {
      int sw = sux-stx;
      int sh = suy-sty;
      al_draw_bitmap(ft_bmp, gx*20, gy*20, 0);
      zfs_show_level_buffer_block_rect(gx, gy, gx+sw, gy+sh, 10, "paste");
   }
   get_new_screen_buffer(3, 0, 0);
}

int zfs_redraw_window(int zfs_window_x1, int zfs_window_y1, int zfs_window_x2, int zfs_window_y2, int gx, int gy)
{
   int mouse_on_window = ((mouse_x > zfs_window_x1) && (mouse_x < zfs_window_x2) && (mouse_y > zfs_window_y1) && (mouse_y < zfs_window_y2));

   // erase background
   al_draw_filled_rectangle(zfs_window_x1-1, zfs_window_y1-1, zfs_window_x2+1, zfs_window_y2+1, palette_color[0]);

   // draw title
   titlex("Zoom Full Screen", 15, 13, zfs_window_x1-1, zfs_window_x2+1, zfs_window_y1);

   // draw filter buttons
   zfs_draw_selection_filters(zfs_window_x1, zfs_window_y1+110, zfs_window_x2);

   // draw action buttons
   zfs_draw_buttons(zfs_window_x1, zfs_window_x2, zfs_window_y1+220);

   // show information about selection and pointer
   zfs_pointer_text(gx, gy, zfs_window_x1, zfs_window_x2, zfs_window_y1+20, mouse_on_window);

   // frame
   al_draw_rectangle(zfs_window_x1-1, zfs_window_y1-1, zfs_window_x2+1, zfs_window_y2+1, palette_color[13], 1);

   return mouse_on_window;
}

void zoom_full_screen(int draw_item)
{
   init_level_background();
   int zfs_window_x1 = 400;
   int zfs_window_w = 160;
   int zfs_window_x2 = zfs_window_x1 + zfs_window_w;
   int zfs_window_y1 = 40;
   int zfs_window_h = 378;
   int zfs_window_y2 = zfs_window_y1 + zfs_window_h;

   int gx=0, gy=0, hx=0, hy=0;
   int mouse_on_window = 0;

   int exit =0;
   copy_mode = 0;
   brf_mode = 0;
   while (mouse_b2) proc_controllers();
   while (!exit)
   {
      ovw_process_scrolledge();

      // get mouse position on scaled level background
      ovw_get_block_position_on_map(&gx, &gy, &hx, &hy);

      // redraw level background
      zfs_redraw_background(gx, gy);

      // redraw window
      mouse_on_window = zfs_redraw_window(zfs_window_x1, zfs_window_y1, zfs_window_x2, zfs_window_y2, gx, gy);

      // process window move by dragging title bar
      if (mouse_on_window) zfs_proc_window_move(&zfs_window_x1, &zfs_window_y1, &zfs_window_x2, &zfs_window_y2, zfs_window_w, zfs_window_h);
      else // mouse is not on window
      {
         if (mouse_b1)
         {
            if (copy_mode)
            {
               while (mouse_b1) proc_controllers();
               zfs_do_fcopy(gx, gy);
               init_level_background();
            }
            if (brf_mode)
            {
               while (mouse_b1) proc_controllers();
               zfs_do_brf(gx, gy, draw_item_num);
               init_level_background();
            }
            if ((!copy_mode) && (!brf_mode)) // get new selection
            {
               // initial values
               stx = gx;
               sty = gy;
               sux = gx+1;
               suy = gy+1;
               while (mouse_b1)
               {
                  sux = gx+1;
                  suy = gy+1;
                  ovw_get_block_position_on_map(&gx, &gy, &hx, &hy);
                  zfs_redraw_background(gx, gy);
               }
               if (sux < stx) // swap if wrong order
               {
                  int temp = sux;
                  sux = stx;
                  stx = temp;
               }
               if (suy < sty)
               {
                  int temp = suy;
                  suy = sty;
                  sty = temp;
               }
               if (stx - sux == 0) sux++;  // don't allow zero size
               if (sty - suy == 0) suy++;  // don't allow zero size
            }
         }
      }
      while ((mouse_b2) || (key[ALLEGRO_KEY_ESCAPE]))
      {
         proc_controllers();
         exit = 1;
      }
   }
}

