// e_item.cpp

#include "pm.h"

void show_all_items(void)
{
   al_set_target_backbuffer(display);
   al_clear_to_color(al_map_rgb(0,0,0));

   test_items();
   int text_pos = 0;

   int num_items = sort_item();
   text_pos = item_data(10, text_pos);

   for (int i=0; i<num_items; i++)
   {
      get_item_draw_shape(i);
      al_set_target_backbuffer(display);
      al_draw_bitmap(dtemp, 0, text_pos, 0);
      sprintf(msg,"item:[%2d] ",i );

      for (int j=0; j<16; j++)
      {
         char msg2[80];
         sprintf(msg2,"%d[%d] ", j, item[i][j]);
         if (j == 0) sprintf(msg2,"[%-2d] ", item[i][j]);
         if (j == 1) sprintf(msg2,"[%-4d] ", item[i][j]);
         if (j == 3) sprintf(msg2,"[%-2d] ", item[i][j]);
         if (j == 4) sprintf(msg2,"[%-4d] ", item[i][j]);
         if (j == 5) sprintf(msg2,"[%-4d] ", item[i][j]);
         strcat(msg, msg2);
      }
      al_draw_text(font, palette_color[13], 20, text_pos+6, 0, msg);
      text_pos +=20;
      if (text_pos > SCREEN_H - 10)
      {
         al_flip_display();
         tsw(); // wait for keypress
         al_clear_to_color(al_map_rgb(0,0,0));
         text_pos = 0;
      }
   }
   al_flip_display();
   tsw(); // wait for keypress
}

int sort_item(void)
{
   // to not break linked items
   for (int c=0; c < 500; c++)
      if (item[c][0] == 1) //door
         item[c][15] = c; // tag this door with its original item number
   int inum, c, d, quit, temp, swap;
   quit=0;
   while (!quit) // sort item list
   {
      quit=1; // quit if no swap
      for (c=0; c < 499; c++)
      {
         itemf[c][0] = al_itofix(item[c][4]);
         itemf[c][1] = al_itofix(item[c][5]);

         if (item[c][0] < item[c+1][0]) // sort by first value 'type'
            swap = 1;
//         else if (item[c][0] == item[c+1][0]) // if type is the same
//            if (item[c][1] < item[c+1][1]) // sort by 2nd value 'ans'
//               swap =1;

         if (swap)
         {
            quit=0;      // as long as a swap has been made
            swap = 0;
            if ((item[c][0] == 10) && (item[c+1][0] == 10)) // both messages
            {
               strcpy(msg, pmsgtext[c]);
               strcpy(pmsgtext[c], pmsgtext[c+1]);
               strcpy(pmsgtext[c+1], msg);
            }
            if ((item[c][0] == 10) && (item[c+1][0] != 10)) // first only
            {
               strcpy(pmsgtext[c+1], pmsgtext[c]);
               pmsgtext[c][0] = 0;
            }
            if ((item[c][0] != 10) && (item[c+1][0] == 10)) // second only
            {
               strcpy(pmsgtext[c], pmsgtext[c+1]);
               pmsgtext[c+1][0] = 0;
            }
            for (d=0; d<16; d++)
            {
               temp = item[c][d];
               item[c][d] = item[c+1][d];
               item[c+1][d] = temp;
            }
            for (d=0; d<4; d++)
            {
               temp = itemf[c][d];
               itemf[c][d] = itemf[c+1][d];
               itemf[c+1][d] = temp;
            }
         }
      }
   }

   // get data about first 30 item types
   // and make sub lists of item types using these variables
   inum = 0;    // zero the counters
   for (c=0; c<30; c++)
   {
      item_num_of_type[c] = 0;
      item_first_num[c] = 0;
   }

   for (c=0; c<500; c++) // get the type counts
   {
      item_num_of_type[item[c][0]]++; // inc number of this type
      if (item[c][0]) inum++;
   }
   for (c=0; c<30; c++)             // get first nums
      if (item_num_of_type[c] > 0)  // are there any of this type?
         for (d=0; d<500; d++)
            if (item[d][0] == c)
            {
               item_first_num[c] = d;
               d=500;   // exit loop
            }
   // to not break linked items
   for (int c=0; c < 500; c++)
      if ((item[c][0] == 1) && (item[c][8] == 1))           // if door && exit mode == link
      {
         int link = item[c][9];                             // look for doors with that link in 15
         for (int d=0; d < 500; d++)
            if ((item[d][0] == 1) && (item[d][15] == link)) // found a door with link in 15
               item[c][9] = d;                              // update to new link number
      }

   // erase what we put in 15
   for (int c=0; c < 500; c++)
      if (item[c][0] == 1)     // door
         item[c][15] = 0;

   // set number of starts...
   number_of_starts = item_num_of_type[5];
   return inum;
}

int get_empty_item(void) // just find me an empty
{
   int mt = -1;
   for (int i=0; i<500; i++)
      if (item[i][0] == 0)
      {
         mt = i;
         break;
      }
   if (mt == -1) al_show_native_message_box(display, "Error", "Error creating item", "Item list full!", NULL, ALLEGRO_MESSAGEBOX_ERROR);
   return mt;
}

int get_empty_item(int type) // finds, sets type, sorts, refinds
{
   int mt = get_empty_item();
   if (mt == -1) return 500;
   else
   {
      erase_item(mt);
      item[mt][0] = type; // set type
      item[mt][9] = 9999; // mark to find after sort !!
      sort_item();
      mt = 0;
      while ((mt < 500) && (item[mt][9] != 9999)) mt++;
      item[mt][9] = 0; // remove mark
   }
   return mt;
}

void test_items(void)
{
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
         if (item[c][0] == 7) good = 1;
         if (item[c][0] == 8) good = 1;
         if (item[c][0] == 10) good = 1;
         if (item[c][0] == 11) good = 1;
         if (item[c][0] == 12) good = 1;
         if (item[c][0] == 14) good = 1;
         if (item[c][0] == 15) good = 1;

         if (item[c][0] == 20) good = 1;

         if (!good)  printf("Item:%d - bad type:%d\n", c, item[c][0]);
         // if (!good)  item[c][0] = 0; //erase bad type
         if (good)
         {
            if ((item[c][1] < 169) || (item[c][1] > 1084))
               printf("Item:%d - bad shape:%d\n", c, item[c][1]);
            int xpos = item[c][4];
            if ((xpos < 20) || (xpos > 1960))
                printf("Item:%d - bad xpos%d\n", c, xpos);
            int ypos = item[c][5];
            if ((ypos < 20) || (ypos > 1960))
                printf("Item:%d - bad ypos%d\n", c, ypos);
         }
      }
}

void erase_item(int num)
{
   if (item[num][0] == 10) pmsgtext[num][0] = 0;
   for (int x=0; x<16; x++) item[num][x] = 0;
}

int create_key(int c)
{
   int key_color = 0, exit=0, bad=0;
   al_flip_display();
   al_clear_to_color(al_map_rgb(0,0,0));

   // first get a key color
   al_draw_text(font, palette_color[15], SCREEN_W/2, 10,  ALLEGRO_ALIGN_CENTER, "Key and Locked Block Creator");
   al_draw_text(font, palette_color[15], SCREEN_W/2, 18,  ALLEGRO_ALIGN_CENTER, "----------------------------");
   al_draw_text(font, palette_color[15], SCREEN_W/2, 180, ALLEGRO_ALIGN_CENTER, "b1 to choose a color");
   al_draw_text(font, palette_color[15], SCREEN_W/2, 188, ALLEGRO_ALIGN_CENTER, "b2 or <esc> to quit");

   for (int x=0; x<4; x++)
      al_draw_bitmap(btile[220+x], SCREEN_W/2, (x*20)+60, 0);

   al_flip_display();

   while (!exit)
   {
      proc_controllers();
      while ((mouse_b2) || (key[ALLEGRO_KEY_ESCAPE]))
      {
         proc_controllers();
         exit = 1;
         bad = 1;
      }

      if ((mouse_b1) && (mouse_x > (SCREEN_W/2)) && (mouse_x < ((SCREEN_W/2)+20)))
      {
         exit = 1;
         key_color = (mouse_y - 60)/20;
         if ((key_color < 0) || (key_color > 3)) key_color = 0;
         while (mouse_b1) proc_controllers(); // wait for release
      }
   }

   // then set the key location
   if (!bad)
   {
      if (getxy("Set Key Location", 2, 4, c) == 1)
      {
         item[c][0] = 4;     // type 4 - key
         item[c][1] = 1039 + key_color; // animation seq
         item[c][2] = 1; // draw mode
         item[c][3] = 1; // fall
         item[c][4] = get100_x*20;
         item[c][5] = get100_y*20;
         l[get100_x][get100_y] = 0; // make sure empty block in that pos
      }
      else bad = 1;
   }

   // then set the block range
   if (!bad)
   {
      if (getbox("Block Range", 2, 4, c))
      {
         bx2 -= 1;
         by2 -= 1;
         if (bx2 < bx1) bx2 = bx1;
         if (by2 < by1) by2 = by1;
         item[c][6] = bx1*20;
         item[c][7] = by1*20;
         item[c][8] = (bx2-bx1+1)*20;
         item[c][9] = (by2-by1+1)*20;
      }
      else bad = 1;
   }
   if (bad) return 0;
   else return 1;
}

int create_trigger(int i)
{
   int bad = 0;

   // set the item location
   if (getxy("Set Trigger Location", 2, 9, i) == 1)
   {
      item[i][0] = 9;  // type 9 - trigger
      item[i][2] = 14; // draw color
      item[i][3] |= PM_ITEM_TRIGGER_PLAYER;
      item[i][3] |= PM_ITEM_TRIGGER_DRAW_ON;
      item[i][4] = get100_x*20;
      item[i][5] = get100_y*20;
   }
   else bad = 1;
   // then set the block range
   if (!bad)
   {
      if (getbox("Draw Trigger Rectangle", 2, 9, i))
      {
         if (bx2 < bx1) bx2 = bx1;
         if (by2 < by1) by2 = by1;
         item[i][6] = bx1*20;
         item[i][7] = by1*20;
         item[i][8] = (bx2-bx1)*20;
         item[i][9] = (by2-by1)*20;
      }
      else bad = 1;
   }
   if (bad) return 0;
   else return 1;
}


int create_block_manip(int i)
{
   int bad = 0;

   // set the item location
   if (getxy("Set Block Manip Location", 2, 16, i) == 1)
   {
      item[i][0] = 16; // type 16 - block manip
      item[i][2] = 1; // draw mode on
      item[i][4] = get100_x*20;
      item[i][5] = get100_y*20;
      item[i][12] = 12; // default color
   }
   else bad = 1;
   // then set the block range
   if (!bad)
   {
      if (getbox("Draw Block Manip Rectangle", 2, 16, i))
      {
         if (bx2 < bx1) bx2 = bx1;
         if (by2 < by1) by2 = by1;
         item[i][6] = bx1*20;
         item[i][7] = by1*20;
         item[i][8] = (bx2-bx1)*20;
         item[i][9] = (by2-by1)*20;
      }
      else bad = 1;
   }
   if (bad) return 0;
   else return 1;
}

int create_block_damage(int i)
{
   int bad = 0;

   // set the item location
   if (getxy("Set Block Damage Location", 2, 17, i) == 1)
   {
      item[i][0] = 17; // type 16 - block damage
      item[i][2] = 1;  // draw type

      item[i][3] |= PM_ITEM_DAMAGE_CURR;

      item[i][4] = get100_x*20;
      item[i][5] = get100_y*20;


      item[i][12] = 100;
      item[i][13] = 0;
      item[i][14] = 100;

   }
   else bad = 1;
   // then set the block range
   if (!bad)
   {
      if (getbox("Draw Block Damage Rectangle", 2, 17, i))
      {
         if (bx2 < bx1) bx2 = bx1;
         if (by2 < by1) by2 = by1;
         item[i][6] = bx1*20;
         item[i][7] = by1*20;
         item[i][8] = (bx2-bx1)*20;
         item[i][9] = (by2-by1)*20;
      }
      else bad = 1;
   }
   if (bad) return 0;
   else return 1;
}




































int create_start_block(int c)
{
   if (getxy("Put Start Location", 2, 5, c) == 1)
   {
      // erase all other starts
      for (int x=0; x<500; x++)
         if (item[x][0] == 5)
            for (int y=0; y<16; y++)
               item[x][y] = 0;

      item[c][0] = 5 ;           // type 5 - start
      item[c][1] = 1021;         // default animation seq
      item[c][2] = 1;            // draw mode
      item[c][3] = 0;            // stationary
      item[c][4] = get100_x*20;  // x pos
      item[c][5] = get100_y*20;  // y pos
      l[get100_x][get100_y] = 0; // make sure empty block in that pos
      return 1;
   }
   else return 0;
}

int create_exit(int c)
{
   if (getxy("Put Exit Location", 2, 3, c) == 1) // xorg, yorg
   {
      item[c][0] = 3 ;           // type 3 - exit
      item[c][1] = 1022;         // default animation seq
      item[c][2] = 1;            // draw mode
      item[c][3] = 0;            // stationary
      item[c][4] = get100_x*20;  // x pos
      item[c][5] = get100_y*20;  // x pos
      item[c][8] = 100;          // num enemies left alive to activate this exit
      l[get100_x][get100_y] = 0; // make sure empty block in that pos
      return 1;
   }
   else return 0;
}

void show_all_pmsg(void)
{
   int text_pos = 0;
   al_set_target_backbuffer(display);
   al_clear_to_color(al_map_rgb(0,0,0));

   al_draw_text(font, palette_color[15], 20, text_pos, 0, "List of pop messages:");
   text_pos +=8;

   for (int i=0; i<500; i++)
   {
      int len = strlen(pmsgtext[i]);
      if (len > 0)
      {
         // count lines and max line length
         int lines = 1;
         int mll = 0; // max line length
         int tlc = 0; // temp line counter

//            for (int j=0; j<len; j++)
//               al_draw_textf(font, palette_color[14], 20, text_pos+=8, 0, "[%d][%d]", j, pmsgtext[i][j]);
//
//            for (int j=0; j<len; j++)
//               al_draw_textf(font, palette_color[14], 20, text_pos+=8, 0, "[%d][%c]", j, pmsgtext[i][j]);

         al_draw_textf(font, palette_color[14], 20, text_pos+=8, 0, "Item:%2d len:%3d lines:%2d max length:%2d", i, len, lines, mll);
         for (int j=0; j<len; j++)
         {
            int col = 15;

            if ((pmsgtext[i][j] < 32) || (pmsgtext[i][j] > 126)) col = 10; // bad char


            if (pmsgtext[i][j] == 126)
            {
               col = 14;
               lines++;
               if (tlc > mll) mll = tlc;
               tlc = 0;
            }
            else tlc++;


            al_draw_textf(font, palette_color[col], 20, text_pos+=8, 0, "[%2d][%3d] - %c", j, pmsgtext[i][j], pmsgtext[i][j] );

            if (pmsgtext[i][j] == 126)
            {
               lines++;
               if (tlc > mll) mll = tlc;
               tlc = 0;
            }
            else tlc++;
         }
         text_pos +=8;
         if (tlc > mll) mll = tlc;
      }
      if (text_pos > SCREEN_H - 10)
      {
         al_flip_display();
         tsw(); // wait for keypress
         al_clear_to_color(al_map_rgb(0,0,0));
         text_pos = 0;
      }
   }
   al_flip_display();
   tsw(); // wait for keypress
}


void display_pop_message(int c, char *f, int xpos_c, int ypos, int redraw_map, int show_line_breaks)
{
   if (f != NULL)
   {
      int len = strlen(f);
      char dt[40][120];
      int row = 0, col = 0;
      int longest_line_len = 1; // default
      int num_lines = 0;
      int tc = item[c][8]; // text color
      int fc = item[c][9]; // frame color

      for (int a=0; a<len+1; a++)
      {
         if (f[a] == 126) // line break
         {
            if (show_line_breaks)
            {
               dt[row][col] = 64; // mark line break with '@'
               dt[row][col+1] = (char)NULL;
            }
            else dt[row][col] = (char)NULL; // in case len = 0 on 1st line
            row++;
            col=0;
            dt[row][col] = (char)NULL; // in case len = 0 on next line
         }
         else  // regular char
         {
            dt[row][col] = f[a];
            if (col > longest_line_len) longest_line_len = col;
            col++;
            dt[row][col] = (char)NULL;
         }
      }
      num_lines = row;

      // new positions and sizes if text changed
      int xw = (longest_line_len+1)*4;
      int px  = xpos_c-xw-2-8;
      int py  = ypos-2-8;
      int px2 = xpos_c+xw+8;
      int py2 = ypos+(num_lines+1)*8+8;
      int pxw = px2 - px;
      int pyh = py2 - py;

      // temp bitmap to draw on
      ALLEGRO_BITMAP * tmp = NULL;
      tmp = al_create_bitmap(pxw, pyh);
      al_set_target_bitmap(tmp);
      al_clear_to_color(al_map_rgb(0,0,0));

      // draw the message frame
      for (int a=0; a<12; a++)
         al_draw_filled_rounded_rectangle(a, a, pxw-a, pyh-a, 4, 4, palette_color[fc+a*16]);

      // draw the message text
      for (row=0; row<=num_lines; row++)
         al_draw_text(font, palette_color[tc], pxw/2+1, 10+row*8, ALLEGRO_ALIGN_CENTER, dt[row]);

     // draw on screen
     al_set_target_backbuffer(display);
     al_draw_bitmap(tmp, px, py, 0);

      // redraw map to erase old
      if (redraw_map) show_big();

      // get the position on the map to copy it to
      int mmx = item[c][10] * db;
      int mmy = item[c][11] * db;

      al_set_clipping_rectangle(1, 1, display_transform_double*db*100-2, display_transform_double*db*100-2);
      al_draw_scaled_bitmap(tmp, 0, 0, pxw, pyh, mmx, mmy, pxw*db/20, pyh*db/20, 0);
      al_reset_clipping_rectangle();

      // also put it in here in case we need it for the mouse pointer
      al_destroy_bitmap(mp);
      mp = al_create_bitmap(pxw*db/20, pyh*db/20);
      al_set_target_bitmap(mp);
      al_clear_to_color(al_map_rgb(0,0,0));

      al_draw_scaled_bitmap(tmp, 0, 0, pxw, pyh, 0, 0, pxw*db/20, pyh*db/20, 0);
      al_destroy_bitmap(tmp);
      al_set_target_backbuffer(display);
   }
}

int create_pmsg(int c)
{
   item[c][0] = 10 ;  // type 10 - msg
   item[c][1] = 1036; // animation seq
   item[c][2] = 1;    // draw mode
   item[c][3] = 0;    // stationary

   item[c][6] = 0;    // clear counter
   item[c][7] = 120;  // default message time
   item[c][8] = 15;   // default text color (white)
   item[c][9] = 12;   // default frame color (blue)

   int bad=0;





   // get text of message
   if (!edit_pmsg_text(c, 1)) bad = 1;

   if (!bad)
   {
      if (getxy("Set Location", 2, 10, c) == 1)
      {
         item[c][4] = get100_x*20;
         item[c][5] = get100_y*20;
         l[get100_x][get100_y] = 0; // zero block
      }
      else bad = 1;
   }

   if (!bad)
   {
      if (getxy("Set Display Position", 2, 1010, c) == 1)
      {
         item[c][10] = get100_x;
         item[c][11] = get100_y;
      }
      else bad = 1;
   }
   if (bad) return 0;
   else
   {
      object_viewer(2, c);

   }


    return 1;
}

int create_door(int type)
{
   switch (type)
   {

    case 2: // one way door with linked item exit
      {
         // first check to see if we can find 2 empty items
         int found_empty_items = 0;
         int c, d;
         c = get_empty_item(); // get 1st item
         if (c != -1)
         {
            item[c][0] = 1; // mark it so we don't find it as empty again
            d = get_empty_item(); // get 2nd item
            if (d != -1) found_empty_items = 1;
            else item[c][0] = 0;
         }

         if (found_empty_items)
         {
            if (getxy("Put door location", 2, 1, c) == 1)
            {
               item[c][4] = get100_x*20;
               item[c][5] = get100_y*20;

               l[item[c][4]/20][item[c][5]/20] = 0;   // zero block

               item[c][0] = 1;    // type 1
               item[c][13] = 1083;    // base draw shape or ans
               item[c][6] = 13;      // default color = lt blue
               item[c][1] = 1083;     // current shape

//               item[c][1] = 1083; // ans


               item[c][2] = 1;    // draw mode normal
               item[c][3] = 0;    // stationary
               item[c][8] = 1;    // type: linked exit

               item[c][11] = 1;    // trigger with up
               item[c][12] = 1;    // always draw line

               draw_big(1);
               show_big();


               if (getxy("Put door destination", 2, 1, d) == 1)
               {
                  item[c][9] = d;    // linked exit

                  item[d][4] = get100_x*20;
                  item[d][5] = get100_y*20;

                  l[item[d][4]/20][item[d][5]/20] = 0;   // zero block

                  item[d][0] = 1;    // type 1

               item[d][13] = 1083;    // base draw shape or ans
               item[d][1] = 1083;     // current shape
               item[d][6] = 6;      // default color = tan


                  item[d][2] = 1;    // draw mode normal
                  item[d][3] = 0;    // stationary
                  item[d][8] = 0;    // type: exit only

                  draw_big(1);
                  show_big();
               } // end of get destination without cancel
               else // cancelled while choosing destination
               {
                  item[c][0] = 0;
               }
             } // end of get location without cancel
             else  // cancelled while choosing location
             {
                item[c][0] = 0;
             }
          } // end of found empty items

      }
      break;

      case 3: // two way door set
      {
         int found_empty_items = 0;
         int c, d;

         // first check to see if we can find 2 empty items
         c = get_empty_item(); // get 1st item
         if (c != -1)
         {
            item[c][0] = 1; // mark it so we don't find it as empty again
            d = get_empty_item(); // get 2nd item
            if (d != -1) found_empty_items = 1;
            else item[c][0] = 0;
         }


         if (found_empty_items)
         {
            if (getxy("Put first door", 2, 1, c) == 1)
            {
               item[c][4] = get100_x*20;
               item[c][5] = get100_y*20;

               l[item[c][4]/20][item[c][5]/20] = 0;   // zero block

               item[c][0] = 1;    // type 1

               item[c][13] = 1083;    // base draw shape or ans
               item[c][1] = 1083;     // current shape
               item[c][6] = 13;      // default color = lt blue

               item[c][2] = 1;    // draw mode normal
               item[c][3] = 0;    // stationary
               item[c][8] = 1;    // type: linked exit

               item[c][11] = 1;    // trigger with up
               item[c][12] = 1;    // always draw line

               draw_big(1);
               show_big();


               if (getxy("Put second door", 2, 1, d) == 1)
               {
                  item[c][9] = d;    // linked exit

                  item[d][4] = get100_x*20;
                  item[d][5] = get100_y*20;

                  l[item[d][4]/20][item[d][5]/20] = 0;   // zero block

                  item[d][0] = 1;    // type 1
                  item[d][2] = 1;    // draw mode normal
                  item[d][3] = 0;    // stationary
                  item[d][8] = 1;    // type: linked exit
                  item[d][9] = c;    // linked exit

                  item[d][11] = 1;    // trigger with up
                  item[d][12] = 1;    // always draw line

                  item[d][13] = 1083;    // base draw shape or ans
                  item[d][1] = 1083;     // current shape
                  item[d][6] = 13;      // default color = lt blue


                  draw_big(1);
                  show_big();
               } // end of get destination without cancel
               else // cancelled while choosing destination
               {
                  item[c][0] = 0;
               }
             } // end of get location without cancel
             else  // cancelled while choosing location
             {
                item[c][0] = 0;
             }
          } // end of found empty items

      }

      break;
   }
   return sort_item();
}

int create_item(int type)
{
   // check for no creator
   if ((type != 1) && (type != 3) && (type != 4) && (type != 5) && (type != 9) && (type != 10) && (type != 16) && (type != 17)) return 9999;



   int i = get_empty_item(type); // get a place to put it


   if (i > 499) return i; // no items
   switch (type)
   {
      case 3:  if (!create_exit(i))         erase_item(i); break;
      case 4:  if (!create_key(i))          erase_item(i); break;
      case 5:  if (!create_start_block(i))  erase_item(i); break;
      case 9:  if (!create_trigger(i))      erase_item(i); break;
      case 10: if (!create_pmsg(i))         erase_item(i); break;
      case 16: if (!create_block_manip(i))  erase_item(i); break;
      case 17: if (!create_block_damage(i)) erase_item(i); break;
   }

   sort_item();
   i = item_first_num[type]+item_num_of_type[type]-1;
   draw_big(1);
   show_big();
   set_wx(item[i][4]/20+4, item[i][5]/20);
   return i;
}




























