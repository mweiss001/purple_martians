// e_item.cpp

#include "pm.h"

void show_all_items(void)
{
   ALLEGRO_BITMAP *tmp;
   tmp = al_create_bitmap(20, 20);

   sort_item(1);
   al_set_target_backbuffer(display);
   al_clear_to_color(al_map_rgb(0,0,0));

   test_items();
   int text_pos = 0;

   int num_items = sort_item(1);
   text_pos = item_data(10, text_pos);

   int rh = 10; // row height

   for (int i=0; i<num_items; i++)
   {
      al_set_target_bitmap(tmp);
      al_clear_to_color(al_map_rgb(0,0,0));
      draw_item(i, 1, 0, 0);
      al_set_target_backbuffer(display);
      al_draw_scaled_bitmap(tmp, 0, 0, 20, 20, 0, text_pos, rh, rh, 0);

      sprintf(msg,"item:[%2d] ",i );
      for (int j=0; j<16; j++)
      {
         char msg2[80];
         //sprintf(msg2,"%d[%d] ", j, item[i][j]);
         sprintf(msg2,"[%d] ", item[i][j]);
         if (j == 0) sprintf(msg2,"[%-2d] ", item[i][j]);
         if (j == 1) sprintf(msg2,"[%-4d] ", item[i][j]);
         if (j == 3) sprintf(msg2,"[%-2d] ", item[i][j]);
         if (j == 4) sprintf(msg2,"[%-4d] ", item[i][j]);
         if (j == 5) sprintf(msg2,"[%-4d] ", item[i][j]);
         strcat(msg, msg2);
      }
      al_draw_text(font, palette_color[13], rh+2, text_pos+(rh-8)/2, 0, msg);
      text_pos +=rh;
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
   al_destroy_bitmap(tmp);
}

int sort_item(int set_pos)
{
   // to not break linked items
   for (int c=0; c < 500; c++)
      if (item[c][0] == 1) //door
         item[c][15] = c; // tag this door with its original item number
   int inum, c, d, quit, temp, iswap;
   quit=0;
   while (!quit) // sort item list
   {
      quit=1; // quit if no iswap
      for (c=0; c < 499; c++)
      {
         if (set_pos)
         {
            itemf[c][0] = al_itofix(item[c][4]);
            itemf[c][1] = al_itofix(item[c][5]);
         }


         if (item[c][0] < item[c+1][0]) // sort by first value 'type'
            iswap = 1;
//         else if (item[c][0] == item[c+1][0]) // if type is the same
//            if (item[c][1] < item[c+1][1]) // sort by 2nd value 'ans'
//               iswap =1;

         if (iswap)
         {
            quit=0;      // as long as a iswap has been made
            iswap = 0;
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
      sort_item(1);
      mt = 0;
      while ((mt < 500) && (item[mt][9] != 9999)) mt++;
      item[mt][9] = 0; // remove mark
   }
   return mt;
}

// not used!!!!! too much hassle....
void check_item(int i, int ct)
{

   // range check for key, trig, manip, damage
   // probably wont use for trig, manip, damage, because they can be slaved to a lift and go off screen
   if (ct == 1)
   {
      if (item[i][6] > 1980)
      {
         item[i][6] = 1980;
         item[i][8] = 2000;
      }
      if (item[i][7] > 1980)
      {
         item[i][7] = 1980;
         item[i][9] = 2000;
      }
      if (item[i][6] < 0)
      {
         item[i][8] += item[i][6]+20; // first adjust width
         item[i][6] = 0;              // then adjust x
      }
      if (item[i][7] < 0)
      {
         item[i][9] += item[i][7]+20; // first adjust height
         item[i][7] = 0;              // then adjust y
      }

      // w and h
      if (item[i][6] + item[i][8] > 2000) item[i][8] = 2000-item[i][6];
      if (item[i][7] + item[i][9] > 2000) item[i][9] = 2000-item[i][7];
   }
}






void test_items(void)
{
   for (int c=0; c<500; c++)
      if (item[c][0])
      {
         int type = item[c][0];
         // first check for valid type
         int good = 0;
         if (type == 1) good = 1;
         if (type == 2) good = 1;
         if (type == 3) good = 1;
         if (type == 4) good = 1;
         if (type == 5) good = 1;
         if (type == 7) good = 1;
         if (type == 8) good = 1;
         if (type == 9) good = 1;
         if (type == 10) good = 1;
         if (type == 11) good = 1;
         if (type == 12) good = 1;
         if (type == 14) good = 1;
         if (type == 15) good = 1;
         if (type == 16) good = 1;
         if (type == 17) good = 1;


         if (!good)  printf("Item:%d - bad type:%d\n", c, item[c][0]);
         // if (!good)  item[c][0] = 0; //erase bad type
         if (good)
         {
            if ( ((item[c][1] < 95) || (item[c][1] > 1084)) && (type != 9) && (type != 16) && (type != 17) )
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

int create_trigger(int i)
{
   int bad = 0;
   // set the item location
   if (getxy("Trigger", 2, 9, i) == 1)
   {
      item[i][0] = 9;  // type 9 - trigger
      item[i][2] = 14; // draw color
      item[i][3] |= PM_ITEM_TRIGGER_PLAYER;
      item[i][3] |= PM_ITEM_TRIGGER_DRAW_ON;
   }
   else bad = 1;
   // then set the block range
   if (!bad)
   {
      if (!get_block_range("Trigger Rectangle", &item[i][6], &item[i][7], &item[i][8], &item[i][9], 1)) bad = 1;
   }
   if (bad) return 0;
   else object_viewerw(2, i);
   return 1;
}


int create_block_manip(int i)
{
   int bad = 0;

   // set the item location
   if (getxy("Block Manip Object", 2, 16, i) == 1)
   {
      item[i][0] = 16; // type 16 - block manip
      item[i][2] = 1; // draw mode on
      item[i][12] = 12; // default color
   }
   else bad = 1;
   // then set the block range
   if (!bad)
   {
      if (!get_block_range("Block Manip Rectangle", &item[i][6], &item[i][7], &item[i][8], &item[i][9], 1)) bad = 1;
   }
   if (bad) return 0;
   else object_viewerw(2, i);
   return 1;
}

int create_block_damage(int i)
{
   int bad = 0;

   // set the item location
   if (getxy("Block Damage Object", 2, 17, i) == 1)
   {
      item[i][0] = 17; // type 16 - block damage
      item[i][2] = 1;  // draw type
      item[i][3] |= PM_ITEM_DAMAGE_CURR;
      item[i][12] = 100;
      item[i][13] = 0;
      item[i][14] = 100;
   }
   else bad = 1;
   // then set the block range
   if (!bad)
   {
      if (!get_block_range("Block Damage Rectangle", &item[i][6], &item[i][7], &item[i][8], &item[i][9], 1)) bad = 1;
   }
   if (bad) return 0;
   else object_viewerw(2, i);
   return 1;
}


int create_start_block(int c)
{
   item[c][0] = 5 ;           // type 5 - start
   item[c][1] = 1021;         // default animation seq
   item[c][2] = 1;            // draw mode
   item[c][3] = 0;            // stationary
   item[c][6] = 2;            // mode = checkpoint common

   // find highest existing start index
   int hesi = -1;
   for (int x=0; x<500; x++)
      if ((item[x][0] == 5) && (x != c))
         if (item[x][7] > hesi) hesi = item[x][7];
   item[c][7] = hesi + 1;     // start index

   if (getxy("Start", 2, 5, c) == 1) return 1;
   else return 0;
}

int create_exit(int c)
{
   item[c][0] = 3 ;           // type 3 - exit
   item[c][1] = 1022;         // default animation seq
   item[c][2] = 1;            // draw mode
   item[c][3] = 0;            // stationary



   item[c][8] = 100;          // num enemies left alive to activate this exit
   if (getxy("Exit", 2, 3, c) == 1) // xorg, yorg
   {
      l[item[c][4]/20][item[c][5]/20] = 0; // make sure empty block in that pos
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
         for (int j=0; j<len; j++)
         {
            if (pmsgtext[i][j] == 10)
            {
               lines++;
               if (tlc > mll) mll = tlc;
               tlc = 0;
            }
            else tlc++;
         }
         if (lines == 1) mll = tlc;

         al_draw_textf(font, palette_color[14], 20, text_pos+=8, 0, "Item:%-2d len:%-3d lines:%-2d max length:%-2d", i, len, lines, mll);

         for (int j=0; j<len; j++)
         {
            int col = 15;
            if ((pmsgtext[i][j] < 32) || (pmsgtext[i][j] > 126)) col = 10; // bad char
            if (pmsgtext[i][j] == 10) col = 9; // LF
            al_draw_textf(font, palette_color[col], 20, text_pos+=8, 0, "[%2d][%3d] - %c", j, pmsgtext[i][j], pmsgtext[i][j] );

            if (text_pos > SCREEN_H - 20)
            {
               al_flip_display();
               tsw(); // wait for keypress
               al_clear_to_color(al_map_rgb(0,0,0));
               text_pos = 0;
            }
         }
         text_pos +=8;
      }
   }
   al_flip_display();
   tsw(); // wait for keypress
}

int create_pmsg(int c)
{
   item[c][0] = 10 ;  // type 10 - msg
   item[c][1] = 1036; // animation seq
   item[c][3] = 0;    // stationary
   item[c][2] = 0;    // flags
   item[c][2] |= PM_ITEM_PMSG_SHOW_SCROLL;
   item[c][2] |= PM_ITEM_PMSG_FRAME12;

   item[c][12] = 120;  // default message time
   set_int_3216(item[c][13], 15, 13); // default text color (white) and frame color (blue)

   int bad=0;

   int x=0, y=0, w=0, h=0;
   get_block_range("Message Area", &x, &y, &w, &h, 1);
   set_int_3216(item[c][10], x, y);
   set_int_3216(item[c][11], w, h);

   if (!bad) if (!edit_pmsg_text(c, 1)) bad = 1; // get text of message
   if (!bad) if (getxy("Message Object", 2, 10, c) != 1) bad = 1;

   if (bad) return 0;
   else object_viewerw(2, c);
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
            if (getxy("Door", 2, 1, c) == 1)
            {
               l[item[c][4]/20][item[c][5]/20] = 0; // make sure empty block in that pos

               item[c][0] = 1;    // type 1
               item[c][13] = 1083;    // base draw shape or ans
               item[c][6] = 13;      // default color = lt blue
               item[c][1] = 1083;     // current shape

               item[c][2] = 1;    // draw mode normal
               item[c][3] = 0;    // stationary
               item[c][8] = 1;    // type: linked exit

               item[c][11] = 1;    // trigger with up
               item[c][12] = 1;    // always draw line
               if (getxy("Destination Door", 2, 1, d) == 1)
               {
                  item[c][9] = d;    // linked exit

                  l[item[c][4]/20][item[c][5]/20] = 0; // make sure empty block in that pos

                  item[d][0] = 1;    // type 1

                  item[d][13] = 1083;    // base draw shape or ans
                  item[d][1] = 1083;     // current shape
                  item[d][6] = 6;      // default color = tan

                  item[d][2] = 1;    // draw mode normal
                  item[d][3] = 0;    // stationary
                  item[d][8] = 0;    // type: exit only
               } // end of get destination without cancel
               else item[c][0] = 0;

             } // end of get location without cancel
             else item[c][0] = 0;
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
            if (getxy("1st Door", 2, 1, c) == 1)
            {
               l[item[c][4]/20][item[c][5]/20] = 0; // make sure empty block in that pos

               item[c][0] = 1;    // type 1

               item[c][13] = 1083;    // base draw shape or ans
               item[c][1] = 1083;     // current shape
               item[c][6] = 13;      // default color = lt blue

               item[c][2] = 1;    // draw mode normal
               item[c][3] = 0;    // stationary
               item[c][8] = 1;    // type: linked exit

               item[c][11] = 1;    // trigger with up
               item[c][12] = 1;    // always draw line

               if (getxy("2nd Door", 2, 1, d) == 1)
               {
                  item[c][9] = d;    // linked exit
                  l[item[c][4]/20][item[c][5]/20] = 0; // make sure empty block in that pos

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

               } // end of get destination without cancel
               else erase_item(c);
            } // end of get location without cancel
            else erase_item(c);
         } // end of found empty items
      }
      break;
   }
   return sort_item(1);
}

int create_item(int type)
{
   // check for no creator
   if ((type != 1) && (type != 3) /*&& (type != 4)*/ && (type != 5) && (type != 9) && (type != 10) && (type != 16) && (type != 17)) return 9999;
   int i = get_empty_item(type); // get a place to put it
   if (i > 499) return i; // no items
   switch (type)
   {
      case 3:  if (!create_exit(i))         erase_item(i); break;
      case 5:  if (!create_start_block(i))  erase_item(i); break;
      case 9:  if (!create_trigger(i))      erase_item(i); break;
      case 10: if (!create_pmsg(i))         erase_item(i); break;
      case 16: if (!create_block_manip(i))  erase_item(i); break;
      case 17: if (!create_block_damage(i)) erase_item(i); break;
   }
   sort_item(1);
   i = item_first_num[type]+item_num_of_type[type]-1;
   return i;
}




























