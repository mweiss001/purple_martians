// mwItemEditorFnx.cpp


#include "pm.h"
#include "mwBitmap.h"
#include "mwWindowManager.h"
#include "mwWindow.h"
#include "mwItem.h"
#include "mwDisplay.h"
#include "mwFont.h"
#include "mwColor.h"
#include "mwLevel.h"
#include "mwMiscFnx.h"
#include "mwInput.h"

int mwItem::item_data(int x, int y)
{
   char msg[1024];
   int inum = sort_item(0);
                               al_draw_textf(mFont.pr8, mColor.pc[12], x, y, 0, "%d Items",      inum);                 y+=8;
                               al_draw_textf(mFont.pr8, mColor.pc[12], x, y, 0, "--------");                            y+=8;
                               al_draw_textf(mFont.pr8, mColor.pc[13], x, y, 0, "%d Starts",     item_num_of_type[5]);  y+=8;
                               al_draw_textf(mFont.pr8, mColor.pc[13], x, y, 0, "%d Exits",      item_num_of_type[3]);  y+=8;
   if (item_num_of_type[1])  { al_draw_textf(mFont.pr8, mColor.pc[13], x, y, 0, "%d Doors",      item_num_of_type[1]);  y+=8; }
   if (item_num_of_type[4])  { al_draw_textf(mFont.pr8, mColor.pc[13], x, y, 0, "%d Keys",       item_num_of_type[4]);  y+=8; }
   if (item_num_of_type[14]) { al_draw_textf(mFont.pr8, mColor.pc[13], x, y, 0, "%d Switches",   item_num_of_type[14]); y+=8; }
   if (item_num_of_type[15]) { al_draw_textf(mFont.pr8, mColor.pc[13], x, y, 0, "%d Sproingies", item_num_of_type[15]); y+=8; }
   if (item_num_of_type[8])  { al_draw_textf(mFont.pr8, mColor.pc[14], x, y, 0, "%d Bombs",      item_num_of_type[8]);  y+=8; }
   if (item_num_of_type[11]) { al_draw_textf(mFont.pr8, mColor.pc[14], x, y, 0, "%d Rockets",    item_num_of_type[11]); y+=8; }
   if (item_num_of_type[7])  { al_draw_textf(mFont.pr8, mColor.pc[14], x, y, 0, "%d Mines",      item_num_of_type[7]);  y+=8; }
   if (item_num_of_type[6])  { al_draw_textf(mFont.pr8, mColor.pc[14], x, y, 0, "%d Orbs",       item_num_of_type[6]);  y+=8; }
   if (item_num_of_type[18]) { al_draw_textf(mFont.pr8, mColor.pc[14], x, y, 0, "%d Gates",      item_num_of_type[18]); y+=8; }
   if (item_num_of_type[19]) { al_draw_textf(mFont.pr8, mColor.pc[14], x, y, 0, "%d Hiders",     item_num_of_type[19]); y+=8; }



   for (int c=1; c<20; c++)
      if ((c!= 1) && (c !=3) && (c!= 4) && (c != 5) && (c != 6) && (c!= 7) && (c!= 8) && (c!= 9) && (c!= 11) && (c!= 13) && (c!= 14) && (c!= 15) && (c!= 16) && (c!= 17) && (c!= 18) && (c!= 19))
         if (item_num_of_type[c]) // not zero
         {
                         sprintf(msg, "%d type %d???  ", item_num_of_type[c], c); // default unknown
            if (c ==  0) sprintf(msg, "%d type 0      ", item_num_of_type[c]);
            if (c ==  2) sprintf(msg, "%d Bonus       ", item_num_of_type[c]);
            if (c == 10) sprintf(msg, "%d Messages    ", item_num_of_type[c]);
            al_draw_text(mFont.pr8, mColor.pc[3], x, y, 0, msg);
            y+=8;
         }
   return y;
}

void mwItem::show_all_items(void)
{
   char msg[1024];
   ALLEGRO_BITMAP *tmp;
   tmp = al_create_bitmap(20, 20);

   sort_item(1);
   al_set_target_backbuffer(mDisplay.display);
   al_clear_to_color(al_map_rgb(0,0,0));

   test_items();
   int text_pos = 0;

   int num_items = sort_item(1);
   text_pos = item_data(10, text_pos);

   int rh = 10; // row height

   for (int i=0; i<num_items; i++)
   {
      al_draw_scaled_bitmap(mBitmap.sprite[item_tile[item[i][0]]], 0, 0, 20, 20, 0, text_pos, rh, rh, 0);

      sprintf(msg,"item:[%2d] ",i );
      for (int j=0; j<16; j++)
      {
         char msg2[80];
         sprintf(msg2,"[%d] ", item[i][j]);
         if (j == 0) sprintf(msg2,"[%-2d] ", item[i][j]);
         if (j == 1) sprintf(msg2,"[%-4d] ", item[i][j]);
         if (j == 3) sprintf(msg2,"[%-2d] ", item[i][j]);
         if (j == 4) sprintf(msg2,"[%-4d] ", item[i][j]);
         if (j == 5) sprintf(msg2,"[%-4d] ", item[i][j]);
         strcat(msg, msg2);
      }
      al_draw_text(mFont.pr8, mColor.pc[13], rh+2, text_pos+(rh-8)/2, 0, msg);
      text_pos +=rh;
      if (text_pos > mDisplay.SCREEN_H - 10)
      {
         al_flip_display();
         mInput.tsw(); // wait for keypress
         al_clear_to_color(al_map_rgb(0,0,0));
         text_pos = 0;
      }
   }
   al_flip_display();
   mInput.tsw(); // wait for keypress
   al_destroy_bitmap(tmp);
}

int mwItem::sort_item(int set_pos)
{
   char msg[1024];
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
            itemf[c][0] = item[c][4];
            itemf[c][1] = item[c][5];
         }

         if (item[c][0] < item[c+1][0]) iswap = 1; // sort by first value 'type'

         else if (item[c][0] == item[c+1][0]) // if type is the same, do secondary sort within same item type
         {
            if (item[c][0] == 5) // start
            {
               if (item[c][7] > item[c+1][7]) iswap = 1; // sort by 2nd value 'start_index'
            }
         }


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
               float ftemp = itemf[c][d];
               itemf[c][d] = itemf[c+1][d];
               itemf[c+1][d] = ftemp;
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
         int link = item[c][9];                                   // look for doors with that link in 15
         for (int d=0; d < 500; d++)
            if ((item[d][0] == 1) && (item[d][15] == link)) // found a door with link in 15
               item[c][9] = d;                                    // update to new link number
      }

   // erase what we put in 15
   for (int c=0; c < 500; c++)
      if (item[c][0] == 1)     // door
         item[c][15] = 0;

   // set number of starts...
   mLevel.number_of_starts = item_num_of_type[5];


   return inum;
}

int mwItem::get_empty_item(void) // just find me an empty
{
   int mt = -1;
   for (int i=0; i<500; i++)
      if (item[i][0] == 0)
      {
         mt = i;
         break;
      }
   if (mt == -1) al_show_native_message_box(mDisplay.display, "Error", "Error creating item", "Item list full!", NULL, ALLEGRO_MESSAGEBOX_ERROR);
   return mt;
}

int mwItem::get_empty_item(int type) // finds, sets type, sorts, refinds
{
   int mt = get_empty_item();
   if (mt == -1) return 500;
   else
   {
      clear_item(mt);
      item[mt][0] = type; // set type
      item[mt][9] = 9999; // mark to find after sort !!
      sort_item(1);
      mt = 0;
      while ((mt < 500) && (item[mt][9] != 9999)) mt++;
      item[mt][9] = 0; // remove mark
   }
   return mt;
}


void mwItem::test_items()
{
   for (int c=0; c<500; c++)
      if (item[c][0])
      {
         int type = item[c][0];
         // first check for valid type
         int good = 1;
         if ((type < 1) || (type > 19)) good = 0; // out of range
         if (type == 12) good = 0; // no item 12

         if (!good)  printf("Item:%d - bad type:%d\n", c, type);
         else
         {
            if ( ((item[c][1] < 95) || (item[c][1] > 1084)) && (type != 9) && (type != 16) && (type != 17) && (type != 18) )
               printf("Item:%d - type:%d - bad shape:%d\n", c, type, item[c][1]);
            int xpos = item[c][4];
            if ((xpos < 20) || (xpos > 1960)) printf("Item:%d - bad xpos%d\n", c, xpos);
            int ypos = item[c][5];
            if ((ypos < 20) || (ypos > 1960)) printf("Item:%d - bad ypos%d\n", c, ypos);
         }
      }
}

void mwItem::clear_item(int num)
{
   if (item[num][0] == 10) pmsgtext[num][0] = 0;
   for (int x=0; x<16; x++) item[num][x] = 0;
}

int mwItem::create_trigger(int i)
{
   int bad = 0;
   // set the item location
   if (mMiscFnx.getxy("Trigger", 2, 9, i) == 1)
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
      if (!mMiscFnx.get_block_range("Trigger Rectangle", item[i][6], item[i][7], item[i][8], item[i][9], 1)) bad = 1;
   }
   if (bad) return 0;
   else mWM.mW[7].object_viewer(2, i);
   return 1;
}


int mwItem::create_block_manip(int i)
{
   int bad = 0;

   // set the item location
   if (mMiscFnx.getxy("Block Manip Object", 2, 16, i) == 1)
   {
      item[i][0] = 16; // type 16 - block manip
      item[i][2] = 1; // draw mode on
      item[i][12] = 12; // default color
   }
   else bad = 1;
   // then set the block range
   if (!bad)
   {
      if (!mMiscFnx.get_block_range("Block Manip Rectangle", item[i][6], item[i][7], item[i][8], item[i][9], 1)) bad = 1;
   }
   if (bad) return 0;
   else mWM.mW[7].object_viewer(2, i);
   return 1;
}

int mwItem::create_block_damage(int i)
{
   int bad = 0;

   // set the item location
   if (mMiscFnx.getxy("Block Damage Object", 2, 17, i) == 1)
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
      if (!mMiscFnx.get_block_range("Block Damage Rectangle", item[i][6], item[i][7], item[i][8], item[i][9], 1)) bad = 1;
   }
   if (bad) return 0;
   else mWM.mW[7].object_viewer(2, i);
   return 1;
}


int mwItem::create_start_block(int c)
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

   if (mMiscFnx.getxy("Start", 2, 5, c) == 1) return 1;
   else return 0;
}

int mwItem::create_exit(int c)
{
   item[c][0] = 3 ;           // type 3 - exit
   item[c][1] = 1022;         // default animation seq
   item[c][2] = 1;            // draw mode
   item[c][3] = 0;            // stationary


   item[c][8] = 100;          // num enemies left alive to activate this exit
   if (mMiscFnx.getxy("Exit", 2, 3, c) == 1) // xorg, yorg
   {
      mLevel.l[item[c][4]/20][item[c][5]/20] = 0; // make sure empty block in that pos
      return 1;
   }
   else return 0;
}

void mwItem::show_all_pmsg(void)
{
   int text_pos = 0;
   al_set_target_backbuffer(mDisplay.display);
   al_clear_to_color(al_map_rgb(0,0,0));

   al_draw_text(mFont.pr8, mColor.pc[15], 20, text_pos, 0, "List of pop messages:");
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

         al_draw_textf(mFont.pr8, mColor.pc[14], 20, text_pos+=8, 0, "Item:%-2d len:%-3d lines:%-2d max length:%-2d", i, len, lines, mll);

         for (int j=0; j<len; j++)
         {
            int col = 15;
            if ((pmsgtext[i][j] < 32) || (pmsgtext[i][j] > 126)) col = 10; // bad char
            if (pmsgtext[i][j] == 10) col = 9; // LF
            al_draw_textf(mFont.pr8, mColor.pc[col], 20, text_pos+=8, 0, "[%2d][%3d] - %c", j, pmsgtext[i][j], pmsgtext[i][j] );

            if (text_pos > mDisplay.SCREEN_H - 20)
            {
               al_flip_display();
               mInput.tsw(); // wait for keypress
               al_clear_to_color(al_map_rgb(0,0,0));
               text_pos = 0;
            }
         }
         text_pos +=8;
      }
   }
   al_flip_display();
   mInput.tsw(); // wait for keypress
}

int mwItem::create_pmsg(int c)
{
   item[c][0] = 10 ;  // type 10 - msg
   item[c][1] = 0;    // trigger event
   item[c][3] = 0;    // stationary
   item[c][2] = 0;    // flags
   item[c][2] |= PM_ITEM_PMSG_SHOW_SCROLL;
   item[c][2] |= PM_ITEM_PMSG_FRAME12;

   item[c][12] = 120;  // default message time

   mMiscFnx.set_int_3216(item[c][13], 15, 13); // default text color (white) and frame color (blue)

   int bad=0;

   if (mMiscFnx.getxy("Message Object", 2, 10, c) != 1) bad = 1;

   if (!bad) mMiscFnx.get_block_range("Message Area", item[c][6], item[c][7], item[c][8], item[c][9], 1);

   if (!bad) if (!edit_pmsg_text(c, 1)) bad = 1; // get text of message

   if (bad) return 0;
   else mWM.mW[7].object_viewer(2, c);
   return 1;
}

int mwItem::create_door(int type)
{
   switch (type)
   {

      case 1: // single door item
      {
         // first get an empty item
         int c = get_empty_item(); // get 1st item
         if (c != -1)
         {
            if (mMiscFnx.getxy("Door", 2, 1, c) == 1)
            {
               item[c][0] = 1;    // type 1
               item[c][1] = 2;    // draw type
               item[c][3] = 0;    // stationary
               item[c][6] = 13;   // default color = lt blue
               item[c][7] = 2;    // move type: move
               item[c][8] = 0;    // type: exit only
               item[c][11] = 0;   // enter immed
               item[c][12] = 1;   // always draw line
            }
         }
      }
      break;
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
            if (mMiscFnx.getxy("Door", 2, 1, c) == 1)
            {

               item[c][0] = 1;    // type 1
               item[c][1] = 2;    // draw type
               item[c][3] = 0;    // stationary
               item[c][6] = 13;   // default color = lt blue
               item[c][7] = 2;    // move type: move
               item[c][8] = 1;    // type: linked exit
               item[c][11] = 0;   // enter immed
               item[c][12] = 1;   // always draw line

               if (mMiscFnx.getxy("Destination Door", 2, 1, d) == 1)
               {
                  item[c][9] = d;  // linked exit
                  item[d][0] = 1;  // type 1
                  item[d][1] = 2;  // draw type
                  item[d][3] = 0;  // stationary
                  item[d][6] = 13; // default color = lt blue
                  item[d][7] = 2;  // move type: move
                  item[d][8] = 0;  // type: exit only

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
            if (mMiscFnx.getxy("1st Door", 2, 1, c) == 1)
            {
               item[c][0] = 1;    // type 1
               item[c][1] = 2;    // draw type
               item[c][3] = 0;    // stationary
               item[c][6] = 13;   // default color = lt blue
               item[c][7] = 2;    // move type: move
               item[c][8] = 1;    // type: linked exit
               item[c][11] = 1;   // trigger with up
               item[c][12] = 1;   // always draw line

               if (mMiscFnx.getxy("2nd Door", 2, 1, d) == 1)
               {
                  item[c][9] = d;    // linked exit
                  item[d][9] = c;    // linked exit

                  item[d][0] = 1;    // type 1
                  item[d][1] = 2;    // draw type
                  item[d][3] = 0;    // stationary
                  item[d][6] = 13;   // default color = lt blue
                  item[d][7] = 2;    // move type: move
                  item[d][8] = 1;    // type: linked exit
                  item[d][11] = 1;   // trigger with up
                  item[d][12] = 1;   // always draw line


               } // end of get destination without cancel
               else clear_item(c);
            } // end of get location without cancel
            else clear_item(c);
         } // end of found empty items
      }
      break;
   }
   return sort_item(1);
}

int mwItem::create_item(int type)
{
   // check for no creator
   if ((type != 1) && (type != 3) && (type != 5) && (type != 9) && (type != 10) && (type != 13) && (type != 16) && (type != 17) && (type != 19)) return 9999;
   int i = get_empty_item(type); // get a place to put it
   if (i > 499) return i; // no items
   switch (type)
   {
      case 3:  if (!create_exit(i))         clear_item(i); break;
      case 5:  if (!create_start_block(i))  clear_item(i); break;
      case 9:  if (!create_trigger(i))      clear_item(i); break;
      case 10: if (!create_pmsg(i))         clear_item(i); break;
      case 13: if (!create_timer(i))        clear_item(i); break;
      case 16: if (!create_block_manip(i))  clear_item(i); break;
      case 17: if (!create_block_damage(i)) clear_item(i); break;
      case 19: if (!create_hider(i))        clear_item(i); break;

   }
   sort_item(1);
   i = item_first_num[type]+item_num_of_type[type]-1;
   return i;
}





int mwItem::create_timer(int i)
{
   int bad = 0;
   // set the item location
   if (mMiscFnx.getxy("Timer", 2, 13, i) == 1)
   {
      item[i][0] = 13;  // type 13 - timer
      //mItem.item[i][2] = 0; // draw color
   }
   else bad = 1;
   if (!bad)
   {
      if (!mMiscFnx.get_block_range("Display Area", item[i][6], item[i][7], item[i][8], item[i][9], 1)) bad = 1;
   }
   if (bad) return 0;
   else mWM.mW[7].object_viewer(2, i);
   return 1;
}

int mwItem::create_hider(int i)
{
   int bad = 0;
   // set the item location
   if (mMiscFnx.getxy("Hider", 2, 19, i) == 1)
   {
      item[i][0] = 19; // type 13 - timer
      item[i][1] = 0;  // tile not needed, use custom draw, use 1 for trigger
      item[i][2] = 1;  // initial action: hide
      item[i][3] = 1;  // mode:1 (always hide)
   }
   else bad = 1;
   if (!bad)
   {
      if (!mMiscFnx.get_block_range("Hidden Area", item[i][6], item[i][7], item[i][8], item[i][9], 1)) bad = 1;
   }
   if (bad) return 0;
   else mWM.mW[7].object_viewer(2, i);
   return 1;
}


