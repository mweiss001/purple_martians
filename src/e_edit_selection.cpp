// e_edit_selection.cpp

#include "pm.h"
#include "mwWindow.h"
#include "mwWindowManager.h"
#include "mwFont.h"
#include "mwBitmap.h"


int ft_level_header[20];
int ft_l[100][100];
int ft_item[500][16];
char ft_pmsgtext[500][500] = {0};

int ft_Ei[100][32];
al_fixed ft_Efi[100][16];

char ft_ln[NUM_LIFTS][80];
int ft_lift[NUM_LIFTS][6];
int ft_ls[NUM_LIFTS][40][6];


void es_pointer_text(int x1, int x2, int y, int mouse_on_window)
{
   int xc = (x1+x2)/2;

   al_draw_text( mF.pr8, palette_color[15], xc, y+2,  ALLEGRO_ALIGN_CENTER, "Pointer");

   if (!mouse_on_window) al_draw_textf(mF.pr8, palette_color[15], xc, y+11, ALLEGRO_ALIGN_CENTER, "  x:%d    y:%d ", mwWM.gx, mwWM.gy);
   else                  al_draw_text( mF.pr8, palette_color[15], xc, y+11, ALLEGRO_ALIGN_CENTER, "  x:--    y:-- ");

   al_draw_rectangle(x1, y+0, x2, y+20, palette_color[15], 1);



   int rx1 = mwWM.bx1*20;    // source x
   int ry1 = mwWM.by1*20;    // source y
   int rx2 = mwWM.bx2*20+20;
   int ry2 = mwWM.by2*20+20;
   int eib=0;
   int iib=0;
   int lib=0;
   al_fixed frx1 = al_itofix(rx1); // source
   al_fixed fry1 = al_itofix(ry1);
   al_fixed frx2 = al_itofix(rx2);
   al_fixed fry2 = al_itofix(ry2);

   // count items in box
   for (int b=0; b<500; b++)
      if ((item[b][0]) && (mwWM.obj_filter[2][item[b][0]]))
         if (item[b][4] >= rx1)
            if (item[b][4] < rx2)
               if (item[b][5] >= ry1)
                  if (item[b][5] < ry2)
                     iib++;

   // count enemies in box
   for (int b=0; b<100; b++)
      if ((Ei[b][0]) && (mwWM.obj_filter[3][Ei[b][0]]))
         if (Efi[b][0] >= frx1)
            if (Efi[b][0] < frx2)
               if (Efi[b][1] >= fry1)
                  if (Efi[b][1] < fry2)
                     eib++;

   // count lifts in box
   if (mwWM.obj_filter[4][1])
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


   al_draw_text( mF.pr8, palette_color[6], xc, y+1,  ALLEGRO_ALIGN_CENTER, "Selection");
   al_draw_textf(mF.pr8, palette_color[6], xc, y+11, ALLEGRO_ALIGN_CENTER, " x:%2d  y:%2d ", mwWM.bx1, mwWM.by1);
   al_draw_textf(mF.pr8, palette_color[6], xc, y+19, ALLEGRO_ALIGN_CENTER, " width:%d ",  mwWM.bx2-mwWM.bx1+1);
   al_draw_textf(mF.pr8, palette_color[6], xc, y+27, ALLEGRO_ALIGN_CENTER, " height:%d ", mwWM.by2-mwWM.by1+1);

   al_draw_textf(mF.pr8, palette_color[7], xc, y+37, ALLEGRO_ALIGN_CENTER, " %d Enemies ", eib);
   al_draw_textf(mF.pr8, palette_color[7], xc, y+45, ALLEGRO_ALIGN_CENTER, " %d Items ", iib);
   al_draw_textf(mF.pr8, palette_color[7], xc, y+53, ALLEGRO_ALIGN_CENTER, " %d Lifts ", lib);
}

void es_do_brf(int x, int y, int flood_block)
{
   int show_progress = 0;
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
               if (f[a1][b1]) set_block_with_flag_filters(a1, b1, flood_block);

         init_level_background(0);
         get_new_background(0);
         get_new_screen_buffer(3, 0, 0);
         al_flip_display();
         al_rest(.04);
      }
   } while (found);
   if (!show_progress)
   {
      // or we could just do it instantly at the end
      for (int a=0; a<100; a++)
         for (int b=0; b<100; b++)
            if (f[a][b]) set_block_with_flag_filters(a, b, flood_block);
      init_level_background(0);
   }
}

void es_clear_ft(void)
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

int es_load_selection(void)
{
   FILE *filepntr;
   int loop, ch, c, x, y;
   char sel_filename[500];
   char buff[2000];
   sprintf(sel_filename,"sel\\");
   if (mw_file_select("Load Selection", sel_filename, ".sel", 0))
   {
      es_clear_ft();
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

void es_save_selection(int save)
{
   int b, c, x, y;
   int eib=0;
   int iib=0;
   int lib=0;
   int x1 = mwWM.bx1*20;
   int y1 = mwWM.by1*20;
   int x2 = mwWM.bx2*20+20;
   int y2 = mwWM.by2*20+20;

   al_fixed fx1 = al_itofix(x1);
   al_fixed fy1 = al_itofix(y1);
   al_fixed fx2 = al_itofix(x2);
   al_fixed fy2 = al_itofix(y2);

   es_clear_ft();

   // blocks
   for (x=0; x<(mwWM.bx2-mwWM.bx1+1); x++)
      for (y=0; y<(mwWM.by2-mwWM.by1+1); y++)
         if ( (x >= 0) && (x < 100) && (y >= 0) && (y < 100) && (mwWM.bx1+x >= 0) && (mwWM.bx1+x < 100) && (mwWM.by1+y >= 0) && (mwWM.by1+y < 100) )
         {
            if (mwWM.obj_filter[1][1])                          ft_l[x][y] = l[mwWM.bx1+x][mwWM.by1+y];                       // get block and flags
            if ((!mwWM.obj_filter[1][1]) && (mwWM.obj_filter[1][2])) ft_l[x][y] = l[mwWM.bx1+x][mwWM.by1+y] & PM_BTILE_MOST_FLAGS; // get flags only
         }


   // items
   for (b=0; b<500; b++)
      if ((item[b][0]) && (mwWM.obj_filter[2][item[b][0]]) && (item[b][4] >= x1) && (item[b][4] < x2) && (item[b][5] >= y1) && (item[b][5] < y2))
      {
         c = iib++;
         // copy all 16 variables
         for (y=0; y<16; y++)
             ft_item[c][y] = item[b][y];

         // set new x, y (now relative to the selection window ul corner)
         ft_item[c][4] = item[b][4] - x1;
         ft_item[c][5] = item[b][5] - y1;

         if ((item[b][0] == 4) || (item[b][0] == 9) || (item[b][0] == 10) || (item[b][0] == 16) || (item[b][0] == 17)) // key, trigger, manip, damage
         {   // set new destination
            ft_item[c][6] = item[b][6] - mwWM.bx1*20;
            ft_item[c][7] = item[b][7] - mwWM.by1*20;
         }
         if (item[b][0] == 10) // message
         {
            int x=0, y=0;
            get_int_3216(item[b][10], x, y);                        // get x and y
            set_int_3216(ft_item[c][10], x - mwWM.bx1*20, y - mwWM.by1*20);   // add offset and set x and y
            strcpy(ft_pmsgtext[c], pmsgtext[b]);
         }
      }

   // enemies
   for (b=0; b<100; b++) // check for enemies in box
      if ((Ei[b][0]) && (mwWM.obj_filter[3][Ei[b][0]]) && (Efi[b][0] >= fx1) && (Efi[b][0] < fx2) && (Efi[b][1] >= fy1) && (Efi[b][1] < fy2))
      {
         //printf("copying enemy:%d to ft\n", b);
         c = eib++;
         for (y=0; y<32; y++)
            ft_Ei[c][y] = Ei[b][y];
         for (y=0; y<16; y++)
            ft_Efi[c][y] = Efi[b][y];

         ft_Efi[c][0]-= fx1;
         ft_Efi[c][1]-= fy1;

         if (ft_Ei[c][0] == 13) // vinepod
         {
            for (int i=3; i<12; i+=2)
            {
               ft_Ei[c][i+0]-= x1;
               ft_Ei[c][i+1]-= y1;
            }
         }
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
      }

   // lifts
   if (mwWM.obj_filter[4][1])
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

  // printf("num of items:%d",iib);

   ft_level_header[4] = eib; // num_of_enemies
   ft_level_header[5] = lib; // num_of_lifts

   mwWM.mW[4].sw = ft_level_header[8] =  mwWM.bx2-mwWM.bx1+1; // width
   mwWM.mW[4].sh = ft_level_header[9] =  mwWM.by2-mwWM.by1+1; // height

   //printf("finished copying to ft - i:%d e:%d l:%d\n", iib, eib, lib);


   if (save)
   {
      char sel_filename[500];
      al_make_directory("sel"); // create if not already created
      sprintf(sel_filename, "sel\\");
      if (mw_file_select("Save Selection", sel_filename, ".sel", 1))
      {
         //printf("fn:%s\n", sel_filename);

         FILE * filepntr = fopen(sel_filename,"w");
         for (x=0; x<20; x++)
            fprintf(filepntr,"%d\n",ft_level_header[x]);
         for (c=0; c<(mwWM.bx2-mwWM.bx1+1); c++)  // selection of blocks
            for (x=0; x<(mwWM.by2-mwWM.by1+1); x++)
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

void es_do_fcopy(int qx1, int qy1)
{
   // printf("do fcopy\n");

   int b, c, x, y;
   int x3 = qx1*20;         // dest x
   int y3 = qy1*20;         // dest y

   int erase_out_of_bounds_main = 0;  // 0 = adjust

   // blocks
   for (x=0; x<mwWM.mW[4].sw; x++)
      for (y=0; y<mwWM.mW[4].sh; y++)
         if ((qx1+x >= 0) && (qx1+x < 100) && (qy1+y >= 0) && (qy1+y < 100))
            set_block_with_flag_filters(qx1+x, qy1+y, ft_l[x][y]);

   // lifts
   if (mwWM.obj_filter[4][1])
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
      }
   } // end of if copy lifts

   // enemies
   for (b=0; b<100; b++) // iterate enemies in ft
      if ((ft_Ei[b][0]) && (mwWM.obj_filter[3][ft_Ei[b][0]])) // if active attempt to copy this enemy
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

               if (Ei[c][0] == 13) // vinepod
               {
                  for (int i=3; i<12; i+=2)
                  {
                     Ei[c][i+0]+= x3;
                     Ei[c][i+1]+= y3;
                  }
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
   // items
   for (b=0; b<500; b++)
      if ((ft_item[b][0]) && (mwWM.obj_filter[2][ft_item[b][0]]))
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

               if ((item[c][0] == 4) || (item[c][0] == 9)|| (item[c][0] == 10) || (item[c][0] == 16) || (item[c][0] == 17)) // key, trigger, manip, damage
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
                  int x=0, y=0;
                  get_int_3216(item[c][10], x, y);                     // get x y
                  set_int_3216(item[c][10], x + qx1*20, y + qy1*20);   // add offset and set x y
                  strcpy(pmsgtext[c], ft_pmsgtext[b]);
               }
               // limits exceeded; erase
               if (lim)
               {
                  printf("erase:%d\n",c);
                  erase_item(c);
                  //copied = -1;
               }
               c = 500; // end loop
            } // end of found empty
         }  // end if iterate real item array
      } // end of attempt copy
   sort_enemy();
   sort_item(1);
   init_level_background(0);
}

void es_do_clear(void)
{
   int x1 = mwWM.bx1*20;
   int y1 = mwWM.by1*20;
   int x2 = mwWM.bx2*20+20;
   int y2 = mwWM.by2*20+20;
   al_fixed fx1 = al_itofix(x1);
   al_fixed fy1 = al_itofix(y1);
   al_fixed fx2 = al_itofix(x2);
   al_fixed fy2 = al_itofix(y2);

   // blocks
   if (mwWM.obj_filter[1][1])
      for (int x=mwWM.bx1; x<mwWM.bx2+1; x++)
         for (int y=mwWM.by1; y<mwWM.by2+1; y++) l[x][y]=0;

   // items
   for (int i=0; i<500; i++)
      if ((item[i][0]) && (mwWM.obj_filter[2][item[i][0]]))
         if ((item[i][4] >= x1) && (item[i][4] < x2) && (item[i][5] >= y1) && (item[i][5] < y2)) erase_item(i);

   // enemies
   for (int e=0; e<100; e++)
      if ((Ei[e][0]) && (mwWM.obj_filter[3][Ei[e][0]]))
         if ((Efi[e][0] >= fx1) && (Efi[e][0] < fx2) && (Efi[e][1] >= fy1) && (Efi[e][1] < fy2))
         {
            for (int y=0; y<32; y++) Ei[e][y] = 0;
            for (int y=0; y<16; y++) Efi[e][y] = al_itofix(0);
         }

   // lifts
   if (mwWM.obj_filter[4][1])
      for (int l=num_lifts-1; l>=0; l--) // have to iterate backwards because erase_lift() does a resort after every erase
         if ((lifts[l].x1 >= x1) && (lifts[l].x1 < x2) && (lifts[l].y1 >= y1) && (lifts[l].y1 < y2)) erase_lift(l);

   sort_enemy();
   sort_item(1);
   init_level_background(0);
}

void set_block_with_flag_filters(int x, int y, int tn)
{
   if ((x>=0) && (x<100) && (y>=0) && (y<100))
   {
      // blocks and flags
      if ((mwWM.obj_filter[1][1]) && (mwWM.obj_filter[1][2])) l[x][y] = tn;

      // flags only
      if ((!mwWM.obj_filter[1][1]) && (mwWM.obj_filter[1][2]))
      {
         int flags = tn & PM_BTILE_MOST_FLAGS; // get only flags from draw item
         l[x][y] &= ~PM_BTILE_MOST_FLAGS;                       // clear flags in destination
         l[x][y] |= flags;                                      // merge
      }
      // blocks only (same as block and flags?)
      if ((mwWM.obj_filter[1][1]) && (!mwWM.obj_filter[1][2])) l[x][y] = tn;
   }
}

int es_draw_buttons(int x3, int x4, int yfb, int d)
{
   int bts = 16;
   int col=0;
   if (mdw_buttont(x3, yfb, x4, bts, 0,0,0,0, 0,9,15,0, 1,0,1,d, "Move Selection"))
   {
      mwWM.mW[4].copy_mode = 1;
      es_save_selection(0); // just puts in ft_
      es_draw_fsel();
      es_do_clear();
      al_set_target_backbuffer(display);
   }
   if (mdw_buttont(x3, yfb, x4, bts, 0,0,0,0, 0,9,15,0, 1,0,1,d, "Clear Selection"))
   {
      es_do_clear();
      al_set_target_backbuffer(display);
   }

   mwWM.mW[4].copy_mode ? col=10 : col=9;
   if (mdw_buttont(x3, yfb, x4, bts, 0,0,0,0, 0,col,15,0, 1,0,1,d, "Paste Selection"))
   {
      if (mwWM.mW[4].copy_mode) mwWM.mW[4].copy_mode = 0;
      else
      {
         mwWM.mW[4].copy_mode = 1;
         es_save_selection(0); // puts selection in ft_ only
         es_draw_fsel();
      }
   }

   yfb+=bts/2; // spacing between groups

   if (mdw_buttont(x3, yfb, x4, bts, 0,0,0,0, 0,9,15,0, 1,0,1,d, "Save To Disk")) es_save_selection(1); // puts in ft_ and saves to disk
   if (mdw_buttont(x3, yfb, x4, bts, 0,0,0,0, 0,9,15,0, 1,0,1,d, "Load From Disk"))
   {
      if (es_load_selection())
      {
         mwWM.mW[4].copy_mode = 1;
         mwWM.mW[4].sw = ft_level_header[8];
         mwWM.mW[4].sh = ft_level_header[9];
         es_draw_fsel();
      }
   }
   if (mwWM.mW[1].draw_item_type == 1) // don't even show these 3 buttons unless draw item type is block
   {
      yfb+=bts/2; // spacing between groups
      if (mdw_buttont(x3, yfb, x4, bts, 0,0,0,0, 0,9,15,0, 1,0,1,d, "Block Fill"))
      {
         for (int x=mwWM.bx1; x<mwWM.bx2+1; x++)
            for (int y=mwWM.by1; y<mwWM.by2+1; y++)
               set_block_with_flag_filters(x, y, mwWM.mW[1].draw_item_num);
         init_level_background(0);
         al_set_target_backbuffer(display);
      }
      if (mdw_buttont(x3, yfb, x4, bts, 0,0,0,0, 0,9,15,0, 1,0,1,d, "Block Frame"))
      {
         for (int x=mwWM.bx1; x<mwWM.bx2+1; x++)
         {
            set_block_with_flag_filters(x, mwWM.by1, mwWM.mW[1].draw_item_num);
            set_block_with_flag_filters(x, mwWM.by2, mwWM.mW[1].draw_item_num);
         }
         for (int y=mwWM.by1; y<mwWM.by2+1; y++)
         {
            set_block_with_flag_filters(mwWM.bx1, y, mwWM.mW[1].draw_item_num);
            set_block_with_flag_filters(mwWM.bx2, y, mwWM.mW[1].draw_item_num);
         }
         init_level_background(0);
         al_set_target_backbuffer(display);
      }
      mwWM.mW[4].brf_mode ? col=10 : col=9;
      if (mdw_buttont(x3, yfb, x4, bts, 0,0,0,0, 0,col,15,0, 1,0,1,d, "Block Floodfill")) mwWM.mW[4].brf_mode = !mwWM.mW[4].brf_mode;
   }
   return yfb;
}


void es_draw_item_ft(int i)
{
   int x = ft_item[i][4];
   int y = ft_item[i][5];
   int type = ft_item[i][0];

   int shape = ft_item[i][1];                    // get shape
   if (shape > 999) shape = zz[5][shape-1000];   // ans
   int drawn = 0;

   if ((type == 2) && (ft_item[i][6] == 3)) shape = 197; // purple coin

   if (type == 9)   shape = 991; // trig
   if (type == 16)  shape = 989; // bm
   if (type == 17)  shape = 988; // bd

   if (type == 6)
   {
      int rb = (ft_item[i][2] & PM_ITEM_ORB_ROTB) >> 14;
      float a=rb*(ALLEGRO_PI/2);
      al_draw_rotated_bitmap(mwB.tile[ft_item[i][1]], 10, 10, x+10, y+10, a, 0);
      drawn = 1;
   }
   if ((type == 8) && (ft_item[i][11])) al_draw_bitmap(mwB.tile[440], x, y, 0); // bomb sticky spikes

   if (type == 11) // rockets
   {
      float rot = al_fixtof(al_fixmul(al_itofix(ft_item[i][10]/10), al_fixtorad_r));
      al_draw_rotated_bitmap(mwB.tile[shape], 10, 10, x+10, y+10, rot, 0);
      drawn = 1;
   }

   // default draw if nothing else has drawn it up to now
   if (!drawn) al_draw_bitmap(mwB.tile[shape], x, y, 0);
}


void es_draw_enemy_ft(int e)
{
   int type = ft_Ei[e][0];
   int EXint = al_fixtoi(ft_Efi[e][0]);
   int EYint = al_fixtoi(ft_Efi[e][1]);
   int flags = 0;
   if (ft_Ei[e][2] == 0) flags = ALLEGRO_FLIP_HORIZONTAL;
   if (ft_Ei[e][2] == 1) flags = 0;
   if (ft_Ei[e][2] == 2) flags = ALLEGRO_FLIP_VERTICAL;
   if (ft_Ei[e][2] == 3) flags = ALLEGRO_FLIP_VERTICAL & ALLEGRO_FLIP_HORIZONTAL;
   int tn = ft_Ei[e][1];

   float rot = al_fixtof(al_fixmul(ft_Efi[e][14], al_fixtorad_r));
   float sc = al_fixtof(ft_Efi[e][12]);
   al_draw_scaled_rotated_bitmap(mwB.tile[tn], 10, 10, EXint+10, EYint+10, sc, sc, rot, flags);

   if (type == 9) // cloner
   {
      // trigger box
      float tx1 = (float)ft_Ei[e][11];
      float ty1 = (float)ft_Ei[e][12];
      float tx2 = (float)(ft_Ei[e][11]+ft_Ei[e][13]+20);
      float ty2 = (float)(ft_Ei[e][12]+ft_Ei[e][14]+20);
      int tc1 = 14 + 128; // trigger box color

      // source
      float sx1 = (float)ft_Ei[e][15];
      float sy1 = (float)ft_Ei[e][16];
      float sx2 = sx1 + (float)ft_Ei[e][19];
      float sy2 = sy1 + (float)ft_Ei[e][20];
      int sc1 = 11 + 128; // source box color

      // destination
      float dx1 = (float)ft_Ei[e][17];
      float dy1 = (float)ft_Ei[e][18];
      float dx2 = dx1 + (float)ft_Ei[e][19];
      float dy2 = dy1 + (float)ft_Ei[e][20];
      int dc1 = 10 + 128; // destination box color

      // show box mode (0=none) (1=trig only) (2=src/dst only) (3=all)
      int q = ft_Ei[e][4];
      if ((q == 1) || (q == 3))
         rectangle_with_diagonal_lines(tx1, ty1, tx2, ty2, 8, tc1, tc1+64, 0); // trigger box
      if ((q == 2) || (q == 3))
      {
         rectangle_with_diagonal_lines(sx1, sy1, sx2, sy2, 8, sc1, sc1+64, 0); // source
         rectangle_with_diagonal_lines(dx1, dy1, dx2, dy2, 8, dc1, dc1+64, 0); // destination
      }
   }
}


void es_draw_lifts_ft()
{
   for (int l=0; l<ft_level_header[5]; l++)
   {
      int col = (ft_lift[l][1] >> 28) & 15;

      int x1 = ft_ls[l][0][0];
      int y1 = ft_ls[l][0][1];
      int x2 = x1 + ft_ls[l][0][2];
      int y2 = y1 + ft_ls[l][0][3];


      int a;
      for (a=0; a<10; a++)
        al_draw_rounded_rectangle(x1+a, y1+a, x2-a, y2-a, 4, 4, palette_color[col + ((9 - a)*16)], 2 ); // faded outer shell
      al_draw_filled_rectangle(x1+a, y1+a, x2-a, y2-a, palette_color[col] );                            // solid core
      al_draw_text(mF.pr8, palette_color[col+160], (x1+x2)/2, (y1+y2)/2 - 3, ALLEGRO_ALIGN_CENTRE, ft_ln[l]); // name

   }
}


void es_draw_fsel(void)
{
   al_destroy_bitmap(ft_bmp);
   ft_bmp = al_create_bitmap(mwWM.mW[4].sw*20, mwWM.mW[4].sh*20);
   al_set_target_bitmap(ft_bmp);
   al_clear_to_color(al_map_rgba(0,0,0,0));

   // draw blocks
   if (mwWM.obj_filter[1][1])
   {
      for (int x=0; x<mwWM.mW[4].sw; x++)
         for (int y=0; y<mwWM.mW[4].sh; y++)
            if (mwWM.mW[1].show_non_default_blocks) draw_block_non_default_flags(ft_l[x][y], x*20, y*20);
   }

   // draw items
   for (int i=0; i<500; i++)
      if ((ft_item[i][0]) && (mwWM.obj_filter[2][ft_item[i][0]])) es_draw_item_ft(i);

   // draw enemies
   for (int e=0; e<100; e++)
      if ((ft_Ei[e][0]) && (mwWM.obj_filter[3][ft_Ei[e][0]])) es_draw_enemy_ft(e);

   // draw lifts
   if (mwWM.obj_filter[4][1]) es_draw_lifts_ft();

}

void es_process_mouse(void)
{
   if (mouse_b[1][0])
   {
      if (mwWM.mW[4].copy_mode)
      {
         while (mouse_b[1][0]) proc_event_queue();
         es_do_fcopy(mwWM.gx, mwWM.gy);
      }
      if (mwWM.mW[4].brf_mode)
      {
         while (mouse_b[1][0])proc_event_queue();
         es_do_brf(mwWM.gx, mwWM.gy, mwWM.mW[1].draw_item_num);
      }
      if ((!mwWM.mW[4].copy_mode) && (!mwWM.mW[4].brf_mode)) mwWM.get_new_box(); // get new selection
   }

   if (mouse_b[2][0])
   {
      while (mouse_b[2][0]) proc_event_queue();
      mwWM.set_windows(1);
   }
}
