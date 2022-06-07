// e_editor_main.cpp
#include "pm.h"

void em_set_swbl(void)
{
   mW[2].swbn = 0;
   for (int c=0; c<NUM_SPRITES; c++)
   {
      swbl[c][0] = swbl[c][1] = 0;                    // erase
      if (sa[c][0] & PM_BTILE_SHOW_SELECT_WIN)
      {
         swbl[mW[2].swbn][0] = c | sa[c][0];                // add to list with default flags
         swbl[mW[2].swbn][0] &= ~PM_BTILE_SHOW_SELECT_WIN;  // clear flag
         mW[2].swbn++;
      }
   }
   mW[2].swnbl = (mW[2].swbn / 16) + 1;
   if (mW[2].swnbl_cur == 0) mW[2].swnbl_cur = mW[2].swnbl; // initial only
}

void em_set_block_range(void)
{
   int draw_item_flags = mW[1].draw_item_num & PM_BTILE_MOST_FLAGS;
   int fsd[20][20] = {0};

   // purple pipe with open center
   fsd[0][0] = 576; // trigger blocks start
   fsd[0][1] = 592; // trigger block end
   fsd[0][9] = -1;  // default shape
   fsd[0][10] = 576; // upper left corner
   fsd[0][11] = 577; // upper right corner
   fsd[0][12] = 578; // lower left corner
   fsd[0][13] = 579; // lower right corner
   fsd[0][14] = 580; // left vertical through
   fsd[0][15] = 581; // right vertical through
   fsd[0][16] = 582; // upper horizontal through
   fsd[0][17] = 583; // lower horizontal through

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

   // wires
   fsd[2][0] = 605; // trigger blocks start
   fsd[2][1] = 624; // trigger block end
   fsd[2][9] = -1;  // default shape
   fsd[2][10] = 608; // upper left corner
   fsd[2][11] = 609; // upper right corner
   fsd[2][12] = 610; // lower left corner
   fsd[2][13] = 611; // lower right corner
   fsd[2][14] = 612; // left vertical through
   fsd[2][15] = 612; // right vertical through
   fsd[2][16] = 614; // upper horizontal through
   fsd[2][17] = 614; // lower horizontal through

   // brick with corners
   fsd[3][0] = 640; // trigger blocks start
   fsd[3][1] = 656; // trigger block end
   fsd[3][9] = 656; // default shape
   fsd[3][10] = 640; // upper left corner
   fsd[3][11] = 641; // upper right corner
   fsd[3][12] = 642; // lower left corner
   fsd[3][13] = 643; // lower right corner
   fsd[3][14] = 644; // left vertical through
   fsd[3][15] = 645; // right vertical through
   fsd[3][16] = 646; // upper horizontal through
   fsd[3][17] = 647; // lower horizontal through

   // rainbows
   fsd[4][0] = 672; // trigger blocks start
   fsd[4][1] = 679; // trigger block end
   fsd[4][9] = -1;  // default shape
   fsd[4][10] = 672; // upper left corner
   fsd[4][11] = 673; // upper right corner
   fsd[4][12] = 674; // lower left corner
   fsd[4][13] = 675; // lower right corner
   fsd[4][14] = 676; // left vertical through
   fsd[4][15] = 677; // right vertical through
   fsd[4][16] = 678; // upper horizontal through
   fsd[4][17] = 679; // lower horizontal through

   // rainbows 2
   fsd[5][0] = 704; // trigger blocks start
   fsd[5][1] = 711; // trigger block end
   fsd[5][9] = -1;  // default shape
   fsd[5][10] = 704; // upper left corner
   fsd[5][11] = 705; // upper right corner
   fsd[5][12] = 706; // lower left corner
   fsd[5][13] = 707; // lower right corner
   fsd[5][14] = 708; // left vertical through
   fsd[5][15] = 709; // right vertical through
   fsd[5][16] = 710; // upper horizontal through
   fsd[5][17] = 711; // lower horizontal through

   for (int i=0; i<20; i++)
      for (int j=0; j<20; j++)
         fsd[i][j] |= draw_item_flags;


   int fsx[20][5] = {0};

   // purple pipes
   fsx[0][0] = 576; // trigger blocks start
   fsx[0][1] = 592; // trigger block end
   fsx[0][2] = 582; // middle
   fsx[0][3] = 590; // left end
   fsx[0][4] = 588; // right end

   // wires
   fsx[1][0] = 608; // trigger blocks start
   fsx[1][1] = 625; // trigger block end
   fsx[1][2] = 614; // middle
   fsx[1][3] = 622; // left end
   fsx[1][4] = 620; // right end

   // lined platform
   fsx[2][0] = 161; // trigger blocks start
   fsx[2][1] = 163; // trigger block end
   fsx[2][2] = 162; // middle
   fsx[2][3] = 161; // left end
   fsx[2][4] = 163; // right end

   // brown bricks
   fsx[3][0] = 177; // trigger blocks start
   fsx[3][1] = 179; // trigger block end
   fsx[3][2] = 178; // middle
   fsx[3][3] = 177; // left end
   fsx[3][4] = 179; // right end

   // semi-solid screen
   fsx[4][0] = 33; // trigger blocks start
   fsx[4][1] = 39; // trigger block end
   fsx[4][2] = 37; // middle
   fsx[4][3] = 36; // left end
   fsx[4][4] = 38; // right end

   for (int i=0; i<20; i++)
      for (int j=0; j<5; j++)
         fsx[i][j] |= draw_item_flags;


   int fsy[20][5] = {0};

   // purple pipes
   fsy[0][0] = 576; // trigger blocks start
   fsy[0][1] = 592; // trigger block end
   fsy[0][2] = 580; // middle
   fsy[0][3] = 591; // upper end
   fsy[0][4] = 589; // lower end

   // wires
   fsy[1][0] = 608; // trigger blocks start
   fsy[1][1] = 625; // trigger block end
   fsy[1][2] = 612; // middle
   fsy[1][3] = 623; // upper end
   fsy[1][4] = 621; // lower end

   // semi-solid screen
   fsy[4][0] = 33; // trigger blocks start
   fsy[4][1] = 39; // trigger block end
   fsy[4][2] = 34; // middle
   fsy[4][3] = 33; // upper end
   fsy[4][4] = 35; // lower end

   for (int i=0; i<20; i++)
      for (int j=0; j<5; j++)
         fsy[i][j] |= draw_item_flags;


   if ((bx2==bx1) && (by2==by1)) l[bx1][by1] = mW[1].draw_item_num; // single block 1 x 1

   if ((bx2==bx1) && (by2-by1>0)) // vertical line 1 x >1
   {
      int a = bx1;
      for (int b=by1; b<by2+1; b++) // cycle the range
      {
         l[a][b] = mW[1].draw_item_num; // set draw item as default
         for (int x=0; x<20; x++)
            if (fsy[x][0]&1023)
            {
               if (((mW[1].draw_item_num&1023) >= (fsy[x][0]&1023)) && ((mW[1].draw_item_num&1023) <= (fsy[x][1]&1023)))
               {
                  l[a][b] = fsy[x][2]; // default
                  if (b == by1) l[a][b] = fsy[x][3]; // left end cap
                  if (b == by2) l[a][b] = fsy[x][4]; // right end cap
               }
            }
      }
   }
   if ((bx2-bx1>0) && (by2==by1)) // horizontal line >1 x 1
   {
      int b = by1;
      for (int a=bx1; a<bx2+1; a++) // cycle the range
      {
         l[a][b] = mW[1].draw_item_num; // set draw item as default
         for (int x=0; x<20; x++)
         {
            if (fsx[x][0]&1023)
            {
               if (((mW[1].draw_item_num&1023) >= (fsx[x][0]&1023)) && ((mW[1].draw_item_num&1023) <= (fsx[x][1]&1023)))
               {
                  l[a][b] = fsx[x][2]; // default
                  if (a == bx1) l[a][b] = fsx[x][3]; // left end cap
                  if (a == bx2) l[a][b] = fsx[x][4]; // right end cap
               }
            }
         }
      }
   }
   if ((bx2-bx1>0) && (by2-by1>0)) // box shape with corners >1 x >1
   {
      int special_handler = 0;
      for (int a=bx1; a<bx2+1; a++)       // cycle the range
         for (int b=by1; b<by2+1; b++)
            for (int x=0; x<20; x++)
               if (fsd[x][0]&1023)
               {
                  if (((mW[1].draw_item_num&1023) >= (fsd[x][0]&1023)) && ((mW[1].draw_item_num&1023) <= (fsd[x][1]&1023)))
                  {
                     special_handler = 1;

                     if (fsd[x][9] != -1) l[a][b] = fsd[x][9];   // default block
                     if (b == by1)        l[a][b] = fsd[x][16];  // upper horizontal through
                     if (b == by2)        l[a][b] = fsd[x][17];  // lower horizontal through


                     if (a == bx1)
                     {
                        if (b == by1)      l[a][b] = fsd[x][10];  // upper-right corner
                        else if (b == by2) l[a][b] = fsd[x][12];  // lower-right corner
                        else               l[a][b] = fsd[x][14];  // right vertical through
                     }
                     if (a == bx2)
                     {
                        if (b == by1)      l[a][b] = fsd[x][11];  // upper-left corner
                        else if (b == by2) l[a][b] = fsd[x][13];  // lower-left corner
                        else               l[a][b] = fsd[x][15];  // left vertical through
                     }
                  }
                  if (!special_handler) l[a][b] = mW[1].draw_item_num;

             } // end of cycle block range
   } // end of box shape with corners
}

char* em_get_text_description_of_block_based_on_flags(int flags)
{
   sprintf(msg, "Empty");  // default

   if (flags & PM_BTILE_SOLID_PLAYER)     sprintf(msg, "Solid");
   if (flags & PM_BTILE_SEMISOLID_PLAYER) sprintf(msg, "Semi-Solid");
   if (flags & PM_BTILE_BREAKABLE_PBUL)   sprintf(msg, "Breakable");
   if (flags & PM_BTILE_BOMBABLE)         sprintf(msg, "Bombable");
   if (flags & PM_BTILE_LADDER_MOVE)      sprintf(msg, "Ladder");
   if (flags & PM_BTILE_ROPE_MOVE)        sprintf(msg, "Rope");
   return msg;
}

void em_show_draw_item_cursor(void)
{
   int x = gx;
   int y = gy;
   if (mW[1].point_item_type > -1) // if mouse pointer on window, do not show draw item
   {
      int type = mW[1].draw_item_type;
      int num = mW[1].draw_item_num;
      switch (type)
      {
         case 1: // block
            if (mW[1].show_non_default_blocks) draw_block_non_default_flags(num, x*20, y*20);
            else al_draw_bitmap(btile[num&1023], x*20, y*20, 0);
         break;
         case 2: // item
            draw_item(num, 1, x*20, y*20);
         break;
         case 3: // enemy
            draw_enemy(num, 1, x*20, y*20);
         break;
         case 5: // PDE
            int a = PDEi[num][1]; // bmp or ans
            if (a > 999) a = zz[5][a-1000]; // ans
            al_draw_bitmap(tile[a], x*20, y*20, 0);

            if ((PDEi[num][0] == 108) && (PDEi[num][11])) al_draw_bitmap(tile[440], x*20, y*20, 0); // bomb sticky spikes



         break;
      }
      al_draw_rectangle(x*20, y*20, x*20+21, y*20+21, palette_color[15], 1);
   }
}

void em_show_item_info(int x, int y, int color, int type, int num)
{
   int a, b;
   switch (type)
   {
      case 1:
         if (mW[1].show_non_default_blocks) draw_block_non_default_flags(num, x, y);
         else al_draw_bitmap(btile[num&1023], x, y, 0);
         al_draw_textf(font, palette_color[color], x+22, y+2, 0, "Block #%d",num&1023);
         al_draw_textf(font, palette_color[color], x+22, y+12, 0, "%s", em_get_text_description_of_block_based_on_flags(num) );
      break;
      case 2:
         draw_item(num, 1, x, y);
         a = item[num][0]; // type
         al_draw_textf(font, palette_color[color], x+22, y+2, 0, "%s", item_name[a]);
         al_draw_textf(font, palette_color[color], x+22, y+12, 0, "%d of %d", 1+num - item_first_num[a], item_num_of_type[a]);
      break;
      case 3:
         draw_enemy(num, 1, x, y);
         a = Ei[num][0]; // type
         al_draw_textf(font, palette_color[color], x+22, y+2, 0, "%s", enemy_name[a]);
         al_draw_textf(font, palette_color[color], x+22, y+12, 0, "%d of %d", 1+num - e_first_num[a], e_num_of_type[a]);
      break;
      case 4:
      {
         int col = lifts[num].color;
         int width = lifts[num].width;
         if (width > 140) width = 140;
         for (a=0; a<10; a++)
            al_draw_rectangle(x+a, y+a, x+(width)-1-a, y+19-a, palette_color[col+((9-a)*16)], 1 );
         al_draw_text(font, palette_color[col+160], x+(width/2), y+6, ALLEGRO_ALIGN_CENTER, lifts[num].lift_name);
      }
      break;
      case 5:
         a = PDEi[num][1]; // bmp or ans
         if (a < NUM_SPRITES) b = a; // bmp
         if (a > 999) b = zz[5][a-1000]; // ans
         al_draw_bitmap(tile[b], x, y, 0);

         if ((PDEi[num][0] == 108) && (PDEi[num][11])) al_draw_bitmap(tile[440], x, y, 0); // bomb sticky spikes

         a = Ei[num][0]; // type
         al_draw_text(font, palette_color[color], x+22, y+2, 0, "Special Item");
         al_draw_textf(font, palette_color[color], x+22, y+12, 0, "%s", PDEt[num][1]);
      break;
   }
}

void em_find_point_item(void)
{
   // find point item
   mW[1].point_item_type = 1; // block by default
   mW[1].point_item_num = l[gx][gy];

   int max_ob = 20;                  // max objects to find
   int ob = 0;                       // objects found
   int mo[max_ob][2];                // array of objects found
   for (int a=0; a<max_ob; a++)          // clear array
   {
       mo[a][0] = 0;
       mo[a][1] = 1;
   }
   for (int i=0; i<500; i++) // check for item
      if ((item[i][0]) && (ob < max_ob))
      {
         int x = item[i][4];
         int y = item[i][5];
         if ( (hx >= x) && (hx <= x+19) && (hy > y) && (hy < y+19) && (ob < max_ob))
         {
             mo[ob][0] = 2;
             mo[ob][1] = i;
             ob++;
         }
      }
   for (int e=0; e<100; e++) // check for enemy
      if ((Ei[e][0]) && (ob < max_ob))
      {
         int x = al_fixtoi(Efi[e][0]);
         int y = al_fixtoi(Efi[e][1]);
         if ( (hx >= x) && (hx <= x+19) && (hy > y) && (hy < y+19) && (ob < max_ob))
         {
             mo[ob][0] = 3;
             mo[ob][1] = e;
             ob++;
         }
      }
   for (int l=0; l<num_lifts; l++) // check for lifts
   {
      int x = lifts[l].x1;
      int y = lifts[l].y1;
      if ( (hx >= x) && (hx <= x+19) && (hy > y) && (hy < y+19) && (ob < max_ob))
      {
          mo[ob][0] = 4;
          mo[ob][1] = l;
          ob++;
      }
   }
   //al_draw_textf(font, palette_color[15], 100, 100, 0, "mouse is on: %d objects", ob);
   //for (int a=0; a<ob; a++) al_draw_textf(font, palette_color[15], 100, 108+a*8, 0, "%d %d ", mo[a][0], mo[a][1]);

   // which one is the winner?
   if (ob)
   {
      int mm = mouse_x % 20;         // mouse position relative to block boundary
      int ss = 20/ob;                // step space
      int of = mm / ss;              // convert to offset into ob array
      mW[1].point_item_type = mo[of][0];
      mW[1].point_item_num  = mo[of][1];
      //al_draw_textf(font, palette_color[11], 100, 92, 0, "mm:%2d ss:%2d of:%2d  ", mm, ss, of);
   }
}

void em_process_mouse(void)
{

   if (mouse_b1)
   {
      // don't allow drag draw selection unless draw type is block
      if (mW[1].draw_item_type != 1) while (mouse_b1) proc_controllers();

      int din = mW[1].draw_item_num; // shorter variable name
      switch (mW[1].draw_item_type)
      {
         case 1:  // block
         {
            bx1 = gx;
            by1 = gy;
            cm_get_new_box();
            em_set_block_range();
         }
         break;
         case 2:  // item
         {
            int type = item[din][0];
            int ofx = gx*20 - item[din][4]; // get offset of move in 2000 format
            int ofy = gy*20 - item[din][5];
            int c = get_empty_item(type); // get a place to put it
            if (c == -1)  break;
            for (int b=0; b<16; b++) item[c][b] = item[din][b]; // copy from draw item
            item[c][4] += ofx; // adjust with offsets
            item[c][5] += ofy;
            if ((type == 4) || (type == 9) || (type == 16) || (type == 17)) // move range for key, trig, manip, damage
            {
               item[c][6] += ofx; // adjust with offsets
               item[c][7] += ofy;
            }
            if (type == 10)
            {
               item[c][10] += ofx; // adjust with offsets
               item[c][11] += ofy;
               strcpy(pmsgtext[c], pmsgtext[din]); // msg
            }
            sort_item(1);
         }
         break;
         case 3:    // enemy
         {
            int type = Ei[din][0];

            int ofx = gx*20 - al_fixtoi(Efi[din][0]); // get offset of move in 2000 format
            int ofy = gy*20 - al_fixtoi(Efi[din][1]);

            int c = get_empty_enemy(type); // get a place to put it
            if (c == -1)  break;
            for (int x=0; x<32; x++) Ei[c][x]  = Ei[din][x];
            for (int x=0; x<16; x++) Efi[c][x] = Efi[din][x];

            Efi[c][0] += al_itofix(ofx);  // apply offsets
            Efi[c][1] += al_itofix(ofy);


            if (type == 7) // podzilla
            {
               if ((key[ALLEGRO_KEY_LSHIFT]) || (key[ALLEGRO_KEY_RSHIFT])) // move stuff also
               //if (al_show_native_message_box(display, "Move?", "Move podzilla's extended position too?", NULL, NULL, ALLEGRO_MESSAGEBOX_YES_NO | ALLEGRO_MESSAGEBOX_QUESTION ) == 1)
               {
                   Efi[c][5] = Efi[din][5] + al_itofix(ofx);
                   Efi[c][6] = Efi[din][6] + al_itofix(ofy);
               }
               if ((key[ALLEGRO_KEY_LSHIFT]) || (key[ALLEGRO_KEY_RSHIFT])) // move stuff also
               //if (al_show_native_message_box(display, "Move?", "Move podzilla's trigger box too?", NULL, NULL, ALLEGRO_MESSAGEBOX_YES_NO | ALLEGRO_MESSAGEBOX_QUESTION ) == 1)
               {
                  Ei[c][11] = Ei[din][11] + ofx;
                  Ei[c][12] = Ei[din][12] + ofy;
               }
               recalc_pod(c);
            }
            if (type == 9) // cloner
            {
               //if (al_show_native_message_box(display, "Move?", "Move cloner's source and destination boxes too?", NULL, NULL, ALLEGRO_MESSAGEBOX_YES_NO | ALLEGRO_MESSAGEBOX_QUESTION ) == 1)
               if ((key[ALLEGRO_KEY_LSHIFT]) || (key[ALLEGRO_KEY_RSHIFT])) // move stuff also
               {
                  Ei[c][15] = Ei[din][15] + ofx;
                  Ei[c][16] = Ei[din][16] + ofy;
                  Ei[c][17] = Ei[din][17] + ofx;
                  Ei[c][18] = Ei[din][18] + ofy;
               }
               //if (al_show_native_message_box(display, "Move?", "Move cloner's trigger box too?", NULL, NULL, ALLEGRO_MESSAGEBOX_YES_NO | ALLEGRO_MESSAGEBOX_QUESTION ) == 1)
               if ((key[ALLEGRO_KEY_LSHIFT]) || (key[ALLEGRO_KEY_RSHIFT])) // move stuff also
               {
                  Ei[c][11] = Ei[din][11] + ofx;
                  Ei[c][12] = Ei[din][12] + ofy;
               }
            }
            sort_enemy();
         }
         break;
         case 5: // Special
         if ((PDEi[din][0] > 99) && (PDEi[din][0] < 200)) // PDE item
         {
            int d = get_empty_item(); // get a place to put it
            if (d == -1)  break;
            // copy from pde
            for (int x=0; x<16; x++) // item
               item[d][x] = PDEi[din][x];
            item[d][0] -= 100;
            item[d][4] = gx*20;
            item[d][5] = gy*20;
            if (item[d][0] == 4)
            {
               itemf[d][0] = al_itofix(item[d][4]);
               itemf[d][1] = al_itofix(item[d][5]);
               get_block_range("Block Range", &item[d][6], &item[d][7], &item[d][8], &item[d][9], 1);
            }
            sort_item(1);
         }
         if (PDEi[din][0] < 99) // PDE enemy
         {
            int d = get_empty_enemy(); // get a place to put it
            if (d == -1)  break;
            for (int x=0; x<32; x++) Ei[d][x]  = PDEi[din][x];
            for (int x=0; x<16; x++) Efi[d][x] = PDEfx[din][x];
            Efi[d][0] = al_itofix(gx*20);  // set new x,y
            Efi[d][1] = al_itofix(gy*20);
            sort_enemy();
         }
         break;
      } // end of switch case
   } // end of mouse_b1
   if (mouse_b2)
   {


/*

      strcpy (global_string[2][0], "Level Editor Pop-Up Menu");
      strcpy (global_string[2][1], "------------------------");


      if (level_editor_mode == 1)
      {

         switch (mW[1].point_item_type)
         {
            case 1:
               sprintf(global_string[2][2], "Copy Block    ");
               sprintf(global_string[2][3], "              ");
               sprintf(global_string[2][4], "                ");
            break;
            case 2:
               sprintf(global_string[2][2], "Copy %s  ",  item_name[item[mW[1].point_item_num][0]]);
               sprintf(global_string[2][3], "View %s  ",  item_name[item[mW[1].point_item_num][0]]);
               sprintf(global_string[2][4], "Delete %s ", item_name[item[mW[1].point_item_num][0]]);
            break;
            case 3:
               sprintf(global_string[2][2], "Copy %s  ",  (const char *)enemy_name[Ei[mW[1].point_item_num][0]]);
               sprintf(global_string[2][3], "View %s  ",  (const char *)enemy_name[Ei[mW[1].point_item_num][0]]);
               sprintf(global_string[2][4], "Delete %s ", (const char *)enemy_name[Ei[mW[1].point_item_num][0]]);
            break;
            case 4:
               sprintf(global_string[2][2], "              ");
               sprintf(global_string[2][3], "View Lift '%s'",   lifts[mW[1].point_item_num].lift_name);
               sprintf(global_string[2][4], "Delete Lift '%s'", lifts[mW[1].point_item_num].lift_name);
            break;
         }



   strcpy (global_string[2][2], "Copy ---");
   strcpy (global_string[2][3], "View ---");
   strcpy (global_string[2][4], "Delete ---");
   strcpy (global_string[2][5], "------------------------");
   strcpy (global_string[2][6], "Zoom Full Screen");
   strcpy (global_string[2][7], "Group Edit");
   strcpy (global_string[2][8], "Show Status Window");
   strcpy (global_string[2][9], "Show Selection Window");
   strcpy (global_string[2][10], "------");
   strcpy (global_string[2][11],"New Level");
   strcpy (global_string[2][12],"Load Level");
   strcpy (global_string[2][13],"Save Level");
   strcpy (global_string[2][14],"Save and Quit");
   strcpy (global_string[2][15],"Help Screens");
   strcpy (global_string[2][16],"Quit Level Editor");
   strcpy (global_string[2][17],"end");


  */

















      switch (mW[1].point_item_type)
      {
         case 1:
            sprintf(global_string[2][2], "Copy Block    ");
            sprintf(global_string[2][3], "              ");
            sprintf(global_string[2][4], "                ");
         break;
         case 2:
            sprintf(global_string[2][2], "Copy %s  ",  item_name[item[mW[1].point_item_num][0]]);
            sprintf(global_string[2][3], "View %s  ",  item_name[item[mW[1].point_item_num][0]]);
            sprintf(global_string[2][4], "Delete %s ", item_name[item[mW[1].point_item_num][0]]);
         break;
         case 3:
            sprintf(global_string[2][2], "Copy %s  ",  (const char *)enemy_name[Ei[mW[1].point_item_num][0]]);
            sprintf(global_string[2][3], "View %s  ",  (const char *)enemy_name[Ei[mW[1].point_item_num][0]]);
            sprintf(global_string[2][4], "Delete %s ", (const char *)enemy_name[Ei[mW[1].point_item_num][0]]);
         break;
         case 4:
            sprintf(global_string[2][2], "              ");
            sprintf(global_string[2][3], "View Lift '%s'",   lifts[mW[1].point_item_num].lift_name);
            sprintf(global_string[2][4], "Delete Lift '%s'", lifts[mW[1].point_item_num].lift_name);
         break;
      }

      switch (pmenu(2, 0))
      {
         case 2:  // copy
            if (mW[1].point_item_type < 4)
            {
               mW[1].draw_item_type = mW[1].point_item_type;
               mW[1].draw_item_num  = mW[1].point_item_num;
            }
         break;
         case 3:  // view
            if (mW[1].point_item_type > 1) object_viewerw(mW[1].point_item_type, mW[1].point_item_num);
         break;
         case 4:  // delete
            switch (mW[1].point_item_type)
            {
               case 1: // delete block
                    l[gx][gy] = 0;
               break;
               case 2: // delete item
                  if ((mW[1].draw_item_type == 2) && (mW[1].draw_item_num == mW[1].point_item_num)) // are you deleting the draw item?
                  {
                     mW[1].draw_item_type = 1;
                     mW[1].draw_item_num = 0;
                  }
                  erase_item(mW[1].point_item_num);
                  sort_item(1);
               break;
               case 3: // delete enemy
                  if ((mW[1].draw_item_type == 3) && (mW[1].draw_item_num == mW[1].point_item_num)) // are you deleting the draw item?
                  {
                     mW[1].draw_item_type = 1;
                     mW[1].draw_item_num = 0;
                  }
                  erase_enemy(mW[1].point_item_num);
                  sort_enemy();
               break;
               case 4: // delete lift
                  erase_lift(mW[1].point_item_num);
               break;
            }
         break;
         case 5: break; // menu divider
         case 6: set_windows(2); break;   // zoom full screen
         case 7: set_windows(3); break;   // group edit
         case 8: mW[1].active = 1; break; // status_window
         case 9: mW[2].active = 1; break; // select_window
         case 11: // new level
         if (al_show_native_message_box(display, "New Level", "Clicking OK will create a new blank level", NULL, NULL, ALLEGRO_MESSAGEBOX_OK_CANCEL) == 1)
         {
            zero_level_data();
            save_level_prompt();
         }
         load_level(last_level_loaded, 0); // blind load
         break;
         case 12: // load level
            load_level_prompt();
            sort_enemy();
            sort_item(1);
         break;
         case 13: save_level_prompt(); break; // save level
         case 14: if (save_level_prompt()) mW[8].active = 0; break; // save and exit
         case 15: help("Level Editor Basics"); break;// help
         case 16: mW[8].active = 0; break; // exit


         case 18: predefined_enemies(); break;
         case 19: global_level(); break;
         case 20: level_viewer(); break;
         case 21: animation_sequence_editor(); break;
         case 22: copy_tiles(); break;
         case 23: edit_btile_attributes(); break;



      } // end of switch case
   } // end of mouse_b2
}


void em_process_keypress(void)
{

   if (key[ALLEGRO_KEY_I])
   {
      while (key[ALLEGRO_KEY_I]) proc_controllers();
      show_all_items();
   }
   if (key[ALLEGRO_KEY_E])
   {
      while (key[ALLEGRO_KEY_E]) proc_controllers();
      show_all_enemies();
   }
   if (key[ALLEGRO_KEY_V])
   {
      while (key[ALLEGRO_KEY_V]) proc_controllers();
      show_all_events();
   }
   if (key[ALLEGRO_KEY_L])
   {
      while (key[ALLEGRO_KEY_L]) proc_controllers();
      show_all_lifts();
   }
   if (key[ALLEGRO_KEY_S])
   {
      while (key[ALLEGRO_KEY_S]) proc_controllers();
      level_check();
   }
   if (key[ALLEGRO_KEY_P])
   {
      while (key[ALLEGRO_KEY_P]) proc_controllers();
      show_all_pmsg();
   }
   while (key[ALLEGRO_KEY_ESCAPE]) { proc_controllers(); mW[8].active = 0; }
}

int edit_menu(int el)
{
   // set all filters on
   for (int i=0; i<5; i++)
      for (int j=0; j<20; j++) obj_filter[i][j] = 1;

   set_windows(0);
   load_mW();
   mW[8].active = 1;      // this is used to quit so it needs to be turned back on
//   mW[8].level_editor_mode = 1; // force start in main edit mode
   if (!el) load_level_prompt(); // load prompt
   else load_level(el, 0);       // blind load
   al_show_mouse_cursor(display);
   level_editor_running = 1;
   resume_allowed = 0;
   mW[1].draw_item_type = 1;
   mW[1].draw_item_num  = 0;
   load_PDE();
   sort_enemy();
   sort_item(1);
   em_set_swbl();
   set_frame_nums(0);
   for (int k = ALLEGRO_KEY_A; k < ALLEGRO_KEY_MAX; k++) key[k] = 0; // clear_key array

   while (mW[8].active)
   {
      cm_redraw_level_editor_background();
      if (!mw_cycle_windows(0)) cm_process_mouse();
      cm_process_keypress();
   }
   level_editor_running = 0;
   al_hide_mouse_cursor(display);
   save_mW();
   return last_level_loaded;
}

