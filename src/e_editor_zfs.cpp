// e_editor_zfs.cpp

#include "pm.h"

void pointer_text(int x, int y, int ty)
{
   if ((x<99) && (y < 99))
   {
      al_draw_text( font, palette_color[15], txc, ty-39, ALLEGRO_ALIGN_CENTER, "Pointer");
      al_draw_textf(font, palette_color[15], txc, ty-31, ALLEGRO_ALIGN_CENTER, "  x:%d    y:%d ", x, y);
   }
   int sey = -20;
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

   al_draw_rectangle(txc-70, ty+sey, txc+70, ty+sey+10,palette_color[14], 1);
   al_draw_rectangle(txc-70, ty+sey, txc+70, ty+sey+36,palette_color[14], 1);
   al_draw_rectangle(txc-70, ty+sey, txc+70, ty+sey+62,palette_color[14], 1);

   al_draw_text( font, palette_color[6], txc, ty+sey+1,  ALLEGRO_ALIGN_CENTER, "Selection");
   al_draw_textf(font, palette_color[6], txc, ty+sey+11, ALLEGRO_ALIGN_CENTER, " x:%2d  y:%2d ", stx, sty);
   al_draw_textf(font, palette_color[6], txc, ty+sey+19, ALLEGRO_ALIGN_CENTER, " width:%d ",  sux-stx);
   al_draw_textf(font, palette_color[6], txc, ty+sey+27, ALLEGRO_ALIGN_CENTER, " height:%d ", suy-sty);

   al_draw_textf(font, palette_color[7], txc, ty+sey+37, ALLEGRO_ALIGN_CENTER, " %d Enemies ", eib);
   al_draw_textf(font, palette_color[7], txc, ty+sey+45, ALLEGRO_ALIGN_CENTER, " %d Items ", iib);
   al_draw_textf(font, palette_color[7], txc, ty+sey+53, ALLEGRO_ALIGN_CENTER, " %d Lifts ", lib);
}



void do_brf(int x, int y, int flood_block)
{
   int f[100][100];                             // array of blocks to mark
   for (int a=0; a<100; a++)
      for (int b=0; b<100; b++) f[a][b] = 0;    // erase array

   int rb = l[x][y]; // block num to replace
   f[x][y] = 1;      // mark initial block pos in array


   //loop start
   int times = 0;
   int found = 0;
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

      // show progress (just because I can and it looks cool
      //printf("times:%d found:%d\n", times, found);
      for (int a1=0; a1<100; a1++)
         for (int b1=0; b1<100; b1++)
            if (f[a1][b1]) l[a1][b1] = flood_block;

      draw_big(1);
      show_big();

      al_rest(.02);
      al_flip_display();

   } while (found);


//      // or we could just do it instantly at a the end
//      // do the swap
//      for (int a=0; a<100; a++)
//         for (int b=0; b<100; b++)
//            if (f[a][b]) l[a][b] = flood_block;


   draw_big(1);

}

int zoom_full_screen(int wx, int wy, int draw_item)
{
   int jh;
   int ty = 60;
   int x1, y1, x2, y2;
   int x, y, exit =0;
   copy_mode = 0;
   brf_mode = 0;

//   // return window size
//   int rw_x1, rw_y1, rw_x2, rw_y2;
//   int rw_w = (SCREEN_W/20);
//   int rw_h = (SCREEN_H/20);

// i don't like this, its usually too big
// and it always reset everytime you come here
// the old way remembers the last selection

   // initial selection
   // set initial selection to screen position of editor
//   stx = wx;
//   sty = wy;
//   sux = stx + (SCREEN_W/20);
//   suy = sty + (SCREEN_H/20);

   while (mouse_b2) proc_controllers();
   while (!exit)
   {
      al_set_target_backbuffer(display);
      al_flip_display();
      al_clear_to_color(al_map_rgb(0,0,0));

      proc_controllers();

      al_rest(mouse_loop_pause);

      draw_big(1);
      show_big();

      title("Zoom Full Screen",  0, 15, 13);
      // menu and buttons
      int bc1 = 10;
      int bc0 = 9;

      jh=4;
      if (copy_blocks) { x = bc1; sprintf(msg,"Blocks On "); }
      else             { x = bc0; sprintf(msg,"Blocks Off"); }
      al_draw_text(font, palette_color[x], txc, ty+(jh*12)+1,  ALLEGRO_ALIGN_CENTER, msg);
      al_draw_rectangle(txc-70, ty+(jh*12), txc+70, ty+(jh*12)+10, palette_color[x], 1);

      jh=5;
      if (copy_enemies){ x = bc1; sprintf(msg,"Enemies On "); }
      else             { x = bc0; sprintf(msg,"Enemies Off"); }
      al_draw_text(font, palette_color[x], txc, ty+(jh*12)+1,  ALLEGRO_ALIGN_CENTER, msg);
      al_draw_rectangle(txc-70, ty+(jh*12), txc+70, ty+(jh*12)+10, palette_color[x], 1);

      jh=6;
      if (copy_items) { x = bc1; sprintf(msg,"Items On "); }
      else            { x = bc0; sprintf(msg,"Items Off"); }
      al_draw_text(font, palette_color[x], txc, ty+(jh*12)+1,  ALLEGRO_ALIGN_CENTER, msg);
      al_draw_rectangle(txc-70, ty+(jh*12), txc+70, ty+(jh*12)+10, palette_color[x], 1);

      jh=7;
      if (copy_lifts) { x = bc1; sprintf(msg,"Lifts On "); }
      else            { x = bc0; sprintf(msg,"Lifts Off"); }
      al_draw_text(font, palette_color[x], txc, ty+(jh*12)+1,  ALLEGRO_ALIGN_CENTER, msg);
      al_draw_rectangle(txc-70, ty+(jh*12), txc+70, ty+(jh*12)+10, palette_color[x], 1);

      jh=9;
      al_draw_rectangle(txc-70, ty+(jh*12), txc+70, ty+(jh*12)+10, palette_color[9], 1);
      al_draw_text(font, palette_color[9], txc, ty+(jh*12)+1,  ALLEGRO_ALIGN_CENTER, "Move Selection");

      jh=10;
      al_draw_rectangle(txc-70, ty+(jh*12), txc+70, ty+(jh*12)+10, palette_color[9], 1);
      al_draw_text(font, palette_color[9], txc, ty+(jh*12)+1,  ALLEGRO_ALIGN_CENTER, "Clear Selection");

      jh=11;
      if (copy_mode) x = 10; else x = 9;
      al_draw_rectangle(txc-70, ty+(jh*12), txc+70, ty+(jh*12)+10, palette_color[x], 1);
      al_draw_text(font, palette_color[x], txc, ty+(jh*12)+1,  ALLEGRO_ALIGN_CENTER, "Paste Selection");

      jh=12;
      al_draw_rectangle(txc-70, ty+(jh*12), txc+70, ty+(jh*12)+10, palette_color[9], 1);
      al_draw_text(font, palette_color[9], txc, ty+(jh*12)+1,  ALLEGRO_ALIGN_CENTER,"Save Selection" );

      jh=13;
      al_draw_rectangle(txc-70, ty+(jh*12), txc+70, ty+(jh*12)+10, palette_color[9], 1);
      al_draw_text(font, palette_color[9], txc, ty+(jh*12)+1,  ALLEGRO_ALIGN_CENTER, "Paste From Disk");

      jh=15;
      al_draw_rectangle(txc-70, ty+(jh*12), txc+70, ty+(jh*12)+10, palette_color[13], 1);
      al_draw_text(font, palette_color[13], txc, ty+(jh*12)+1,  ALLEGRO_ALIGN_CENTER, "Block Fill");

      jh=16;
      al_draw_rectangle(txc-70, ty+(jh*12), txc+70, ty+(jh*12)+10, palette_color[13], 1);
      al_draw_text(font, palette_color[13], txc, ty+(jh*12)+1,  ALLEGRO_ALIGN_CENTER,"Block Frame" );

      jh=17;
      if (brf_mode) x = 10; else x = 13;
      al_draw_rectangle(txc-70, ty+(jh*12), txc+70, ty+(jh*12)+10, palette_color[x], 1);
      al_draw_text(font, palette_color[x], txc, ty+(jh*12)+1,  ALLEGRO_ALIGN_CENTER, "Block Floodfill");

      // mark selection on map
      al_draw_rectangle(stx*db, sty*db, (sux*db)-1, (suy*db)-1, palette_color[14], 1);
      al_draw_text(font, palette_color[14], stx*db+2, sty*db-11,  0, "selection");


#ifndef RELEASE
      if (key[ALLEGRO_KEY_R])
      {
         while (key[ALLEGRO_KEY_R]) proc_controllers();
         do_rnd();
         Redraw = 1;
      }
#endif

      // mouse on buttons
      if ((mouse_b1) && (mouse_x > txc-70) && (mouse_x < txc + 70) && (mouse_y > ty) && (mouse_y < ty+(18*12)))
      {
         while (mouse_b1) proc_controllers(); // wait for release
         int mb = (mouse_y - ty) / 12;
         switch(mb)
         {
            case 4:
               copy_blocks  = !copy_blocks;
               if (copy_mode) draw_fsel();
            break;
            case 5:
               copy_enemies = !copy_enemies;
               if (copy_mode) draw_fsel();
            break;
            case 6:
               copy_items = !copy_items;
               if (copy_mode) draw_fsel();
            break;

            case 7:
               copy_lifts = !copy_lifts;
               if (copy_mode) draw_fsel();
            break;

            case 9: // move selection button clicked
               if (copy_mode) copy_mode = 0;
               else
               {
                  copy_mode = 1;
                  save_selection(0); // just puts in ft_
                  draw_fsel();
                  do_clear();
               }
            break;
            case 10: // clear selection button clicked
               do_clear();
            break;
            case 11: // paste selection button clicked
               if (copy_mode) copy_mode = 0;
               else
               {
                  copy_mode = 1;
                  save_selection(0); // just puts in ft_
                  draw_fsel();
               }
            break;
            case 12: // save selection button clicked
               save_selection(1); // puts in ft_ and saves to disk
            break;
            case 13: // paste from disk button clicked

               if (load_selection())
               {
                  copy_mode = 1;
                  draw_fsel();
               }
            break;
            case 15: // Block Fill
               if (draw_item_type == 1)
               {
                  for (int x=stx; x<sux; x++)
                     for (int y=sty; y<suy; y++)
                        l[x][y] = draw_item_num;
               }
            break;
            case 16: // Block Outline
               if (draw_item_type == 1)
               {
                  for (x=stx; x<sux; x++)
                  {
                     l[x][sty] = draw_item_num;
                     l[x][suy-1] = draw_item_num;
                  }
                  for (y=sty; y<suy; y++)
                  {
                     l[stx][y] = draw_item_num;
                     l[sux-1][y] = draw_item_num;
                  }
               }
            break;
            case 17: // Block Replace/Floodfill
               if (draw_item_type == 1) brf_mode = !brf_mode;
            break;
         } // end of switch button
         Redraw = 1;
      } // end of mouse pressed on menu




      // get the mouse block index on the map
      x1 = mouse_x/db;
      y1 = mouse_y/db;

      // show information about selection and pointer
      pointer_text(x1, y1, ty);

      if ((x1 < 100) && (y1 < 100)) // if mouse pointer on map
      {
//         // show return window outline
//         if ((!copy_mode) && (!brf_mode))
//         {
//            // set ul corner from mouse pos
//            rw_x1 = x1;
//            rw_y1 = y1;
//            // if lr pos is off screen, adjust ul pos
//            if (rw_x1 > 100-rw_w) rw_x1 = 100-rw_w;
//            if (rw_y1 > 100-rw_h) rw_y1 = 100-rw_h;
//
//            // calc lr position
//            rw_x2 = rw_x1 + rw_w;
//            rw_y2 = rw_y1 + rw_h;
//
//            al_draw_text(font, palette_color[15], rw_x1*db+2, rw_y1*db-10, 0, "exit window");
//            al_draw_rectangle(rw_x1*db, rw_y1*db, rw_x2*db - 1, rw_y2*db - 1, palette_color[15], 1);
//         }

         if (copy_mode) // show copy outline window
         {
            x2 = x1 + ft_level_header[8];
            y2 = y1 + ft_level_header[9];
            if (x2 > 100) x2 = 100;
            if (y2 > 100) y2 = 100;

            al_set_clipping_rectangle(0, 0, display_transform_double*db*100-1, display_transform_double*db*100-1);
            al_draw_bitmap(ft_bmp, x1*db, y1*db, 0);
            al_draw_text(font, palette_color[42], x1*db+2, y1*db-11, 0, "paste selection");
            //al_draw_textf(font, palette_color[42], x1*db+2, y1*db-19, 0, "%d %d %d %d", x1, y1, x2, y2);
            al_draw_rectangle(x1*db, y1*db, x2*db-1, y2*db-1, palette_color[10], 1);
            al_reset_clipping_rectangle();

         }

         if (mouse_b1)
         {
            if ((copy_mode) || (brf_mode)) while (mouse_b1) proc_controllers(); // wait for release
            if (copy_mode) do_fcopy(x1, y1);
            if (brf_mode) do_brf(x1, y1, draw_item_num);
            if ((!copy_mode) && (!brf_mode)) // get new selection
            {
               // initial selection
               stx = x1;
               sty = y1;
               sux = x1+1;
               suy = y1+1;

               while (mouse_b1)
               {
                  proc_controllers();
                  sux = (mouse_x/db)+1;
                  if (sux > 100) sux = 100;
                  suy = (mouse_y/db)+1;
                  if (suy > 100) suy = 100;

                  show_big();
                  // show selection rectangle
                  al_draw_rectangle(stx*db, sty*db, (sux*db)-1, (suy*db)-1, palette_color[14], 1);
                  al_draw_text(font, palette_color[14], stx*db+2, sty*db-11,  0, "selection");
                  al_flip_display();
                  al_rest(.02);
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
            } // end of get new selection
         }

      } // end of if mouse pointer on map
      while ((mouse_b2) || (key[ALLEGRO_KEY_ESCAPE]))
      {
         proc_controllers();
         exit = 1;
      }
   } // end of while (!exit)
   return (y1*100)+x1;
}



















void clear_ft(void)
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


            if ((item[b][0] == 9) || (item[b][0] == 16) || (item[b][0] == 17)) // trigger, manip, damage
            {   // set new destination
               ft_item[c][6] = item[b][6] - stx*20;
               ft_item[c][7] = item[b][7] - sty*20;
            }

            if (item[b][0] == 10) // message
            {
               ft_item[c][10] = item[b][10] - stx;
               ft_item[c][11] = item[b][11] - sty;
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
               construct_lift_step(c, y, vx, vy, 0, 0, ft_ls[b][y][2], ft_ls[b][y][3]);  /// fix me <<<<----------00,
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
                     Ei[c][11]+= x3/20;
                     Ei[c][12]+= y3/20;
                     Ei[c][13]+= x3/20;
                     Ei[c][14]+= y3/20;
                  }
                  if (Ei[c][0] == 9) // cloner
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




                  if ((item[c][0] == 9) || (item[c][0] == 16) || (item[c][0] == 17)) // move field for trigger, manip, damage
                  {
                     item[c][6] += qx1*20;
                     item[c][7] += qy1*20;
                     // really should do some bounds checks here
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
                     strcpy(pmsgtext[c], ft_pmsgtext[b]);
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
         } // end of attempt copy
      } // end of iterate items ft
   } // end of if copy items
   sort_enemy();
   sort_item();
   draw_big(1);
}



float mdw_rnd(float min, float max)
{
   int imin = (int) (min * 100.0);
   int irng = (int) ((max-min) * 100.0);
   int s = imin + rand() % irng;
   float res = (float)s / 100.0;
   return res;
}

int enemy_initial_position_random(int e, int csw)
{
   int x, y, empty = 0, lb = 0;
   while ((!empty) && (lb < 10000))
   {
      lb++;
      x = rand() % 100;
      y = rand() % 100;

      empty = 1;
      if (csw) // confine to selection window
      {
         if ((x < stx) || (x >= sux)) empty = 0;
         if ((y < sty) || (y >= suy)) empty = 0;
      }
      if (!is_block_empty(x, y, 1, 1, 1)) empty = 0;
   }
   if (empty)
   {
      Efi[e][0] = al_itofix(x)*20;
      Efi[e][1] = al_itofix(y)*20;
      return 1;
   }
   else
   {
      printf("could not find empty\n");
      return 0;
   }
}

void do_rnd(void)
{
   int x1 = stx *20;  // source box
   int y1 = sty *20;
   int x2 = sux *20;
   int y2 = suy *20;

   al_fixed fx1 = al_itofix(x1);
   al_fixed fy1 = al_itofix(y1);
   al_fixed fx2 = al_itofix(x2);
   al_fixed fy2 = al_itofix(y2);



   int sbx = 0, sby = 0; // start block x and y
   for (int c=0; c<500; c++)
      if (item[c][0] == 5)
      {
         sbx = item[c][4];
         sby = item[c][5];
      }


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
         if ((Efi[b][0] >= fx1) && (Efi[b][0] < fx2) && (Efi[b][1] >= fy1) && (Efi[b][1] < fy2))
         {

            // initial position random for all enemies
            //enemy_initial_position_random(b, 1);

            int archwag = 0;
            int bouncer = 0;
            int cannon = 1;
            int trakbot = 0;
            int flapper = 0;

            if ((Ei[b][0] == 3) && (archwag))
            {
               printf("randomizing archwag:%d\n", b);

               //enemy_initial_position_random(b, 1);

               Efi[b][6] = al_ftofix(mdw_rnd(2.5, 7)); // x speed
               Efi[b][3] = al_ftofix(mdw_rnd(2, 8)); // y speed
               Efi[b][7] = al_ftofix(mdw_rnd(3, 7)); // bullet speed
               Ei[b][15] = (int) mdw_rnd(20, 120);   // bullet retrigger value
               Ei[b][17] = (int) mdw_rnd(60, 200);  // bullet prox
               Ei[b][11] = (int) mdw_rnd(0, 10); // jump before hole
               Ei[b][12] = (int) mdw_rnd(30, 60); //  jump before wall


               Ei[b][2] = (int) mdw_rnd(0, 2);  // initial direction
               if (Ei[b][2]) Efi[b][2] = Efi[b][6];
               else Efi[b][2] = -Efi[b][6];



            }
            if ((Ei[b][0] == 4) && (bouncer))
            {
               printf("randomizing bouncer:%d\n", b);
               enemy_initial_position_random(b, 1);
               Ei[b][8] = (int) mdw_rnd(1, 3); // seek count
               Efi[b][5] = al_ftofix(mdw_rnd(2, 8)); // seek speed
               // set initial direction
               if (0) set_xyinc_rot(b, rand() % 2000, rand() % 2000); // random
               if (1) set_xyinc_rot(b, sbx, sby); // point at start block
            }
            if ((Ei[b][0] == 6) && (cannon))
            {
               printf("randomizing cannon:%d\n", b);
               Ei[b][9] = (int) mdw_rnd(0, 4); // extra hits to kill
               Ei[b][8] = (int) mdw_rnd(1, 4); // seek count
               Efi[b][5] = al_ftofix(mdw_rnd(2, 5)); // seek speed
               Efi[b][7] = al_ftofix(mdw_rnd(3, 8)); // bullet speed
               Ei[b][15] = (int) mdw_rnd(60, 200); // bullet retrigger
               // set initial direction
               if (1) set_xyinc_rot(b, rand() % 2000, rand() % 2000); // random
               if (0) set_xyinc_rot(b, sbx, sby); // point at start block
            }

            if ((Ei[b][0] == 8) && (trakbot))
            {
               printf("randomizing trakbot:%d\n", b);

               Ei[b][7] = (int) mdw_rnd(0, 2);  // drop mode(0=no, 1=yes)

//               Ei[b][5] = (int) mdw_rnd(0, 8); // trakbot direction
//               set_trakbot_mode(b, Ei[b][5]);

               Efi[b][2] = al_ftofix(mdw_rnd(2, 8)); // x speed
               Efi[b][3] = al_ftofix(mdw_rnd(2, 8)); // y speed

               Efi[b][7] = al_ftofix(mdw_rnd(2, 8)); // bullet speed
               Ei[b][15] = (int) mdw_rnd(50, 200);   // bullet retrigger
               Ei[b][17] = (int) mdw_rnd(40, 280);   // bullet prox
            }

            if ((Ei[b][0] == 12) && (flapper))
            {


               printf("randomizing flapper:%d\n", b);

               //enemy_initial_position_random(b, 1);


               Ei[b][2] = (int) mdw_rnd(0, 2);  // initial direction

               if (Ei[b][2]) Efi[b][2] = Efi[b][6];
               else Efi[b][2] = -Efi[b][6];



               Efi[b][5] = al_ftofix(mdw_rnd(2, 4));  // max x speed
               Efi[b][6] = al_ftofix(mdw_rnd(.1, .5)); // x accel

               Efi[b][3] = al_ftofix(mdw_rnd(1, 2.3));  // y seek speed
               Ei[b][20] = (int) mdw_rnd(0, 80);  // height above player
               Efi[b][10] = al_ftofix(mdw_rnd(1.5, 4)); // flap speed
               Ei[b][21] = (int) mdw_rnd(10, 40);     // flap height

               Efi[b][7] = al_ftofix(mdw_rnd(3, 7)); // bullet speed
               Ei[b][15] = (int) mdw_rnd(20, 120);    // bullet retrigger value
//
//               Ei[b][17] = (int) mdw_rnd(20, 120); // prox width
//               Ei[b][18] = (int) mdw_rnd(20, 120); // prox height
//               Ei[b][19] = (int) mdw_rnd(20, 120); // prox depth
            }
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
               al_draw_bitmap(tile[ft_l[x][y]], x*20, y*20, 0);

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
            al_draw_bitmap(tile[b], ex, ey, 0);
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
            al_draw_bitmap(tile[b], ex, ey, 0);
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


















