#include "pm.h"


extern int level_done;
extern int LIVES;
extern int lit_item;


int item_data(int x_pos, int y_pos)
{
   int inum = item_sort();
   al_draw_textf(font, palette_color[12], x_pos, y_pos, 0, "%d Items", inum); y_pos += 8;
   al_draw_textf(font, palette_color[12], x_pos, y_pos, 0, "--------"); y_pos += 8;
   al_draw_textf(font, palette_color[13], x_pos, y_pos, 0, "%d Starts",  item_num_of_type[5]); y_pos += 8;
   al_draw_textf(font, palette_color[13], x_pos, y_pos, 0, "%d Exits", item_num_of_type[3]); y_pos += 8;

   if (item_num_of_type[12])
   {
      al_draw_textf(font, palette_color[10], x_pos, y_pos, 0, "%d Warps", item_num_of_type[12]); y_pos += 8;
   }
   if (item_num_of_type[1])
   {
      al_draw_textf(font, palette_color[13], x_pos, y_pos, 0, "%d Doors", item_num_of_type[1]); y_pos += 8;
   }
   if (item_num_of_type[4])
   {
      al_draw_textf(font, palette_color[13], x_pos, y_pos, 0, "%d Keys", item_num_of_type[4]); y_pos += 8;
   }
   if (item_num_of_type[14])
   {
      al_draw_textf(font, palette_color[13], x_pos, y_pos, 0, "%d Switches", item_num_of_type[14]); y_pos += 8;
   }
   if (item_num_of_type[15])
   {
      al_draw_textf(font, palette_color[13], x_pos, y_pos, 0, "%d Sproingies", item_num_of_type[15]); y_pos += 8;
   }
   if (item_num_of_type[8])
   {
      al_draw_textf(font, palette_color[14], x_pos, y_pos, 0, "%d Bombs", item_num_of_type[8]); y_pos += 8;
   }
   if (item_num_of_type[11])
   {
      al_draw_textf(font, palette_color[14], x_pos, y_pos, 0, "%d Rockets", item_num_of_type[11]); y_pos += 8;
   }
   if (item_num_of_type[7])
   {
      al_draw_textf(font, palette_color[14], x_pos, y_pos, 0, "%d Mines", item_num_of_type[7]); y_pos += 8;
   }
   for (int c=1; c<16; c++)
   {
      if ((c != 5) && (c !=3) && (c!= 12) && (c!= 1) && (c!= 4) && (c!= 14) && (c!= 15) && (c!= 8) && (c!= 11) && (c!= 7))
         if (item_num_of_type[c]) // not zero
         {
            sprintf(msg, "%d type %d???", item_num_of_type[c], c); // default
            if (c==0) sprintf(msg, "%d type 0      ", item_num_of_type[c]);
            if (c==2) sprintf(msg, "%d Bonus       ", item_num_of_type[c]);
            if (c==6) sprintf(msg, "%d Free Men    ",  item_num_of_type[c]);
            if (c==9) sprintf(msg, "%d ???WTF      ",  item_num_of_type[c]);
            if (c==10) sprintf(msg, "%d Messages   ",  item_num_of_type[c]);
            al_draw_text(font, palette_color[3], x_pos, y_pos, 0, msg);
            y_pos += 8;
         }

   }
   return y_pos;
}

void change_linked_door_color_and_shape(int door)
{
   // changes all linked doors to match door color and shape
   // first do the linked door if there is one...
   if (item[door][8] == 1)
   {
      int di = item[door][9]; // dest item
      item[di][6] = item[door][6]; // set color
      item[di][13] = item[door][13]; // set shape
      item[di][1]  = item[door][13]; // also set in [1]
   }

   // then look for any doors that link to this door
   for (int i=0; i<500; i++)
      if ((item[i][0] == 1) && (item[i][9] == door))
      {
         item[i][6] = item[door][6]; // set color
         item[i][13] = item[door][13]; // set shape
         item[i][1]  = item[door][13]; // also set in [1]
      }
}

void remove_block(int x, int y)
{
   l[x][y] = 0;
   al_set_target_bitmap(l2000);
   al_draw_filled_rectangle(x*20, y*20, x*20+20, y*20+20, palette_color[0]);
   al_draw_bitmap(memory_bitmap[0], x*20, y*20, 0);
}



void draw_pop_message(int c)
{
   al_set_target_bitmap(level_buffer);
   // process the text string into lines and rows and put in temp array
   char dt[40][120];
   int row = 0, col = 0, num_lines = 0;;
   int longest_line_len = 1;
   for (int a=0; a < (int)strlen(pmsg[c]) + 1; a++)
   {
       if (pmsg[c][a] == 126) // line break
      {
         dt[row][col] = 0; // in case len == 0  on first line
         row++;
         col=0;
         dt[row][col] = 0; // in case len == 0 on next line
      }
      else  // regular char
      {
         dt[row][col] = pmsg[c][a];
         if (col > longest_line_len) longest_line_len = col;
         col++;
         dt[row][col] = 0;
      }
   }
   num_lines = row;

   // x positions
   int pxw = (longest_line_len+2)*8;  // width is set from longest text line
   int px1 = item[c][10]*20;          // item sets left edge with specified corner block
   int px2 = px1 + pxw + 8;           // right edge depends on text width
   int pxc = px1 + pxw / 2;           // text center position

   // y positions
   int py1 = item[c][11]*20;          // item sets top edge with specified corner block
   int py2 = py1 + (num_lines+3) * 8; // bottom edge is set from number of lines of text

   int fc = item[c][9];               // frame color
   for (int a=0; a<8; a++)            // frame
      //al_draw_rectangle(px1+a, py1+a, px2-a, py2-a, palette_color[fc+a*16], 1);
      al_draw_rounded_rectangle(px1+a, py1+a, px2-a, py2-a, 4, 4, palette_color[fc+a*16], 2);

   int tc = item[c][8];               // text color
   for (row=0; row<=num_lines; row++) // text
      al_draw_text(font, palette_color[tc], pxc+4, py1+row*8+9, ALLEGRO_ALIGN_CENTRE, dt[row]);
}


int small_letter(ALLEGRO_BITMAP *bmp, int x, int col, char *t)
{


   int text_width = 0;
   switch(t[0])
   {
/*      case 68: // D
             rect(bmp, x+0, 0, x+2, 3, palette_color[col]);
         putpixel(bmp, x+2, 0,    palette_color[0]);
         putpixel(bmp, x+2, 3,    palette_color[0]);
         text_width = 4;
      break;
*/

      case 73: // I
            al_draw_line(x+0.5f, 0.5f, x+0.5f, 4.5, palette_color[col], 1);
         text_width = 2;
      break;
      case 78: // N
         al_draw_line(x+0.5f, 0.5f, x+0.5f, 4.5, palette_color[col], 1);
         al_draw_line(x+3.5f, 0.5f, x+3.5f, 4.5, palette_color[col], 1);
         al_put_pixel(x+1, 1, palette_color[col]);
         al_put_pixel(x+2, 2, palette_color[col]);
         text_width = 5;
      break;




      case 79: // O
         al_draw_rectangle(x+0.5f, 0+0.5f, x+2+0.5f, 3+0.5f, palette_color[col], 1);
         text_width = 4;
      break;

/*
      case 80: // P
             rect(bmp, x+0, 0, x+2, 2, palette_color[col]);
         putpixel(bmp, x+0, 3,    palette_color[col]);
         text_width = 4;
      break;
  */

      case 84: // T
            //vline(bmp, x+1, 0, 3, palette_color[col]);
         al_draw_line(x+1.5f, 0.5f, x+1.5f, 4.5, palette_color[col], 1);
         al_put_pixel(x+0, 0, palette_color[col]);
         al_put_pixel(x+2, 0, palette_color[col]);
         text_width = 4;
      break;


      case 85: // U
         al_draw_line(x+0.5f, 0.5f, x+0.5f, 4.5, palette_color[col], 1);
         al_draw_line(x+2.5f, 0.5f, x+2.5f, 4.5, palette_color[col], 1);
         al_put_pixel(x+1, 3, palette_color[col]);
         text_width = 4;
      break;


/*

      case 87: // W
            vline(bmp, x+0, 0, 2, palette_color[col]);
            vline(bmp, x+4, 0, 2, palette_color[col]);
         putpixel(bmp, x+1, 3,    palette_color[col]);
         putpixel(bmp, x+3, 3,    palette_color[col]);
         putpixel(bmp, x+2, 2,    palette_color[col]);
         text_width = 6;
      break;
*/

   }


   return text_width;


}

void draw_small_text(ALLEGRO_BITMAP * bmp, int x, int y, int col, int textnum)
{

   int xpos = 0;
   ALLEGRO_BITMAP *temp = NULL;

   if (textnum == 1) // OUT
   {
      temp = al_create_bitmap(11,4);
      al_set_target_bitmap(temp);
      al_clear_to_color(al_map_rgb(0,0,0));
      xpos += small_letter(temp, xpos, col, (char*)"O");
      xpos += small_letter(temp, xpos, col, (char*)"U");
      xpos += small_letter(temp, xpos, col, (char*)"T");
      al_set_target_bitmap(bmp);
      al_draw_bitmap(temp, x, y, 0);
   }
   if (textnum == 2) // IN
   {
      temp = al_create_bitmap(6,4);
      al_set_target_bitmap(temp);
      al_clear_to_color(al_map_rgb(0,0,0));
      xpos += small_letter(temp, xpos, col, (char*)"I");
      xpos += small_letter(temp, xpos, col, (char*)"N");
      al_set_target_bitmap(bmp);
      al_draw_bitmap(temp, x, y, 0);
   }
/*
   if (textnum == 3) // UP
   {
      temp = al_create_bitmap(7,4);
      clear(temp);
      xpos += small_letter(temp, xpos, col, "U");
      xpos += small_letter(temp, xpos, col, "P");
      draw_sprite(bmp, temp, x, y);
   }
   if (textnum == 4) // DOWN
   {
      temp = al_create_bitmap(19,4);
      clear(temp);
      xpos += small_letter(temp, xpos, col, "D");
      xpos += small_letter(temp, xpos, col, "O");
      xpos += small_letter(temp, xpos, col, "W");
      xpos += small_letter(temp, xpos, col, "N");
      draw_sprite(bmp, temp, x, y);
   }

*/
   al_destroy_bitmap(temp);



}






void draw_door(int c, int x, int y)
{
   ALLEGRO_BITMAP *tmp = NULL;

   int col = item[c][6];
   //int text_col = col;
   if (item[c][13] == 448) // old style door shape
   {
      //text_col = 15;                // all old style door use white lettering
      //if (col == 15) text_col = 14;  // except for white!
      int shape = item[c][1];       // get shape
      int si = shape-448;           // convert to index to bitmap sequence
      //al_draw_bitmap(door_bitmap[1][col][si], 0, 0, 0);
      tmp = door_bitmap[1][col][si];

   }
   else // new style doors
   {
      //if (col == 0) text_col = 15;    // all use same color as door, except for door color 0
      int an = zz[1][83];             // cheat and use shape index from base door animation sequence
      if (item[c][8] == 0) an = 7-an; // exit only, run the sequence backwards
      //al_draw_bitmap(door_bitmap[0][col][an], 0, 0, 0);
      tmp = door_bitmap[0][col][an];
   }

   int drawn = 0;
   if (item[c][8] == 1)  // linked item destination
   {
      // linked destination item position
      int dx = al_fixtoi(itemf[item[c][9]][0]);
      int dy = al_fixtoi(itemf[item[c][9]][1]);

      // line_color = players[p].color;
      int line_color = item[item[c][9]][6];

      if (item[c][12] == 1) // if always draw lines (1)
         al_draw_line(x+10, y+10, dx+10, dy+10, palette_color[line_color], 1);  // draw a line connecting them

      for (int p=0; p<NUM_PLAYERS; p++) // is player touching door
         if ((players[p].active) && (players[p].marked_door == c))
         {
            if (item[c][12] > 0)  // always draw or only draw when touching ( 1 or 2)
               al_draw_line(x+10, y+10, dx+10, dy+10, palette_color[line_color], 1);  // draw a line connecting them

            // bigger door when player touching it
            al_draw_scaled_bitmap(tmp, 0, 0, 20, 20, x-5, y-6, 30, 26, 0 );

            if (item[c][8] == 0) al_draw_scaled_bitmap(memory_bitmap[1015], 0, 0, 20, 20, x-5, y-6, 30, 26, 0); // OUT
            else al_draw_scaled_bitmap(memory_bitmap[1014], 0, 0, 20, 20, x-5, y-6, 30, 26, 0); // IN




            if (item[c][11] == 1) // enter with <up>
               al_draw_text(font, palette_color[15],  x+3, y-14, 0, "up");

            if (item[c][11] == 2) // enter with <down>
               al_draw_text(font, palette_color[15],  x-5, y-14, 0, "down");
            drawn = 1;
         }


   }
   if (!drawn)
   {
      al_draw_bitmap(tmp, x, y, 0); // if not drawn yet

      if (item[c][8] == 0) al_draw_bitmap(memory_bitmap[1015], x, y, 0); // OUT
      else al_draw_bitmap(memory_bitmap[1014], x, y, 0); // IN



   }

}










void draw_items(void)
{
   al_set_target_bitmap(level_buffer);
   for (int c=0; c<500; c++)
      if (item[c][0])
      {
         int x = al_fixtoi(itemf[c][0]);
         int y = al_fixtoi(itemf[c][1]);
         int shape = item[c][1];                       // get shape
         if (shape > 999) shape = zz[0][shape-1000];   // ans
         int drawn = 0;

         if ((item[c][0] == 10) && (item[c][6] > 0)) // pop up message
         {
            item[c][6]--;
            draw_pop_message(c);
         }

         if (item[c][0] == 1)
         {
             draw_door(c, x, y);
             drawn = 1;
         }

         // moving key in final sequence
         if ((item[c][0] == 4) && (item[c][11] > 0) && (item[c][11] < 10))
         {
            // moving key in final stage gets static shape not ans
            shape = item[c][1];                           // get shape
            if (shape > 999) shape = zz[5][shape-1000];   // get first shape only

            // stretch the key
            float sc = 1 + 4*((10 - (float)item[c][11]) / 10);
            float rot = al_fixtof(al_fixmul(al_itofix(item[c][10]/10), al_fixtorad_r));
            al_draw_scaled_rotated_bitmap(memory_bitmap[shape],10, 10, x+10, y+10, sc, sc, rot, 0);
            drawn = 1;

            // draw a collapsing rectangle
            int x1 = item[c][6] * 20;
            int y1 = item[c][7] * 20;
            int x2 = (item[c][8]+1) * 20;
            int y2 = (item[c][9]+1) * 20;
            int xw = x2-x1;
            int yh = y2-y1;
            float xinc = xw/8;
            float yinc = yh/8;

            float seq = 9 - item[c][11]; // starts at 0, goes to 8 ;
            int xo = (int)(seq * xinc / 2);
            int yo = (int)(seq * yinc / 2);
            al_draw_rectangle(x1+xo, y1+yo, x2-xo, y2-yo, palette_color[15], 1);

         }

        if (item[c][0] == 99) // stretch draw for lit bombs only
        {
            float sc = (float) item[c][10] / 100;
            al_draw_scaled_rotated_bitmap(memory_bitmap[shape], 10, 10, x+10, y+10, sc, sc, 0, 0);
            drawn = 1;

            // show blast area
            int b = item[c][7]; // blast size
            int d = b + 20;     // b is - inc; d is + inc and needs to be one block (20) bigger

             // snap to blocks to show blocks that will be removed
            b /= 20; b *= 20;
            d /= 20; d *= 20;
            x +=10; x /= 20; x *= 20;
            y +=10; y /= 20; y *= 20;

            // alternate between red outer and yellow inner
            int bmd = passcount % 10;
            if ((bmd > 0) && (bmd < 5)) al_draw_rectangle(x-b, y-b, x+d, y+d, palette_color[10], 1); // red outer
            b -=4; d-=4;
            if ((bmd > 4) && (bmd < 10)) al_draw_rectangle(x-b, y-b, x+d, y+d, palette_color[14], 1); // yellow inner
         }

          // these types need rotation
         if ((item[c][0] == 11) || (item[c][0] == 98) ||  // rockets
            ((item[c][0] == 4) && (item[c][11] > 0))) // moving key
         {
            float rot = al_fixtof(al_fixmul(al_itofix(item[c][10]/10), al_fixtorad_r));
            al_draw_rotated_bitmap(memory_bitmap[shape], 10, 10, x+10, y+10, rot, 0);
            drawn = 1;
         }

         if (item[c][0] == 3) // exit
         {
            al_draw_bitmap(memory_bitmap[399], x, y, 0); // 'exit' text not shown
            if (passcount % 60 > 30)
               al_draw_text(f3, palette_color[10], x+11, y-2, ALLEGRO_ALIGN_CENTER, "EXIT");

            int exit_enemys_left = num_enemy - item[c][8];
            if (exit_enemys_left > 0) // locked
            {
               al_draw_bitmap(memory_bitmap[366], x, y, 0); // show lock
               if (passcount % 60 < 30)
                  al_draw_textf(f3, palette_color[14], x+11, y-2, ALLEGRO_ALIGN_CENTER, "%d", exit_enemys_left);

            }
            drawn = 1;
         }

         // default draw if nothing else has drawn it up to now
         if (!drawn) al_draw_bitmap(memory_bitmap[shape], x, y, 0);




   /*

         // this is to test if the players or enemies are in the damage window
         if ((item[c][0] == 8) || (item[c][0] == 99))  // bomb
         {
            al_fixed b = al_itofix(item[c][7]); // damage window size
            al_fixed x = itemf[c][0];
            al_fixed y = itemf[c][1];

            // these are for the comparison
            al_fixed x1 = x - b;
            al_fixed x2 = x + b;
            al_fixed y1 = y - b;
            al_fixed y2 = y + b;

            // these are to show the rectangle
            al_fixed dx1 = x - b + al_itofix(10);
            al_fixed dx2 = x + b + al_itofix(10);
            al_fixed dy1 = y - b + al_itofix(10);
            al_fixed dy2 = y + b + al_itofix(10);

            int color = 11; // green
            for (int p=0; p<NUM_PLAYERS; p++) // players in damage window
               if ((players[p].active) && (!players[p].paused) )
                  if ((players[p].PX > x1) && (players[p].PX < x2) && (players[p].PY > y1) && (players[p].PY < y2))
                  {
                     color = 12; // blue

                     // calculate the damage the player would take
                     al_fixed x_dist = abs(players[p].PX - x);
                     al_fixed y_dist = abs(players[p].PY - y);
                     al_fixed distance = fixhypot(x_dist, y_dist);
                     al_fixed damage = fixdiv(b, distance); // damage window size / distance (scale of 1 - 4)
                     damage *= 20; // multiple by 20 (scale of 20-80)
                     if (damage > al_itofix(80)) damage = al_itofix(80);

                     sprintf(msg, "b:%d d:%d  damage:%d ", al_fixtoi(b), al_fixtoi(distance), al_fixtoi(damage) );
                     textout_ex(level_buffer, font, msg, al_fixtoi(x1)+10, al_fixtoi(y1), palette_color[color], 0);

                   }

            for (int e=0; e<100 ; e++) // enemies in damage window?
               if (Ei[e][0])
                  if ((Efi[e][0] > x1) && (Efi[e][0] < x2) && (Efi[e][1] > y1) && (Efi[e][1] < y2))
                     color = 13;


            rect(level_buffer, al_fixtoi(dx1), al_fixtoi(dy1), al_fixtoi(dx2), al_fixtoi(dy2), palette_color[color]);
         }

   */


      } // end of active item iterate
}


void proc_item_move()
{
   int a, c, x, y;
   for (c=0; c<500; c++)
      if (item[c][0])
      {

         // check for time to live
         int ttl = item[c][14];
         if (ttl)
         {
            if (ttl < 11)       // start ans seq
            {
               item[c][0] = 66; // change to different type to prevent use
               int sq = 10-ttl;
               item[c][1] = zz[5+sq][74];
            }
            if (ttl == 1) item[c][0] = 0; // kill instantly
            item[c][14]--;
         }


         if ((item[c][0] == 4) && (item[c][11] > 0)) // moving key
         {
            // do the incs until the last 10 frames, which are for final sequence
            if (item[c][11] > 10)
            {
               itemf[c][0] += itemf[c][2];  // xinc
               itemf[c][1] += itemf[c][3];  // yinc
            }
            item[c][11]--;
            if (item[c][11] == 0)
            {
               // remove the key
               item[c][0] = 0;
               if (item[c][12]) // matching keyed blocks only
               {
                  int key = item[c][1] - 1039;
                  for (x = item[c][6]; x <= item[c][8]; x++)
                     for (y = item[c][7]; y <= item[c][9]; y++)
                        if ((l[x][y] == 188 + key) || (l[x][y] == 204 + key) || (l[x][y] == 220 + key))
                           remove_block(x, y);
               }
               else // remove all blocks in range
               {
                  for (x = item[c][6]; x <= item[c][8]; x++)
                     for (y = item[c][7]; y <= item[c][9]; y++)
                        remove_block(x, y);
               }
               draw_lift_lines(); // in case removing the key blocks erases lift lines
             }
         }  // end of moving key
         else if ((item[c][3]) && (item[c][0] != 98)) // and not stationary and not lit rocket
         {
            int pc = 0;
            for (int p=0; p<NUM_PLAYERS; p++)
               if (players[p].active)
                  if ((!players[p].paused) || (players[p].paused && players[p].paused_type == 2))
                     if (c == (players[p].carry_item-1)) pc = 1;



            if (!pc) // not being carried
            {
               // apply incs
               itemf[c][0] += itemf[c][2];  // xinc
               itemf[c][1] += itemf[c][3];  // yinc

               // always slow down xinc (kinda like friction)
               if (itemf[c][2] > al_itofix(0))
               {
                   itemf[c][2] -= al_ftofix(.01); // slow down + x move
                      if (itemf[c][2] < al_itofix(0)) // set to zero if crosses zero
                         itemf[c][2] = al_itofix(0);
               }

               if (itemf[c][2] < al_itofix(0))
               {
                   itemf[c][2] += al_ftofix(.01); // slow down + x move
                      if (itemf[c][2] > al_itofix(0)) // set to zero if crosses zero
                         itemf[c][2] = al_itofix(0);
               }

               x = al_fixtoi(itemf[c][0]);
               y = al_fixtoi(itemf[c][1]);

               // moving right
               if ((itemf[c][2] > al_itofix(0)) && (is_right_solid(x,y, 1)))
               {
                  itemf[c][0] -= itemf[c][2];  // take back xinc
                  itemf[c][2] = al_itofix(0);     // stop
               }

               // moving left
               if ((itemf[c][2] < al_itofix(0)) && (is_left_solid(x,y, 1)))
               {
                  itemf[c][0] -= itemf[c][2];  // take back xinc
                  itemf[c][2] = al_itofix(0);     // stop
               }

               x = al_fixtoi(itemf[c][0]);
               y = al_fixtoi(itemf[c][1]);

               // moving up
               if (itemf[c][3] < al_itofix(0))
               {
                  if (is_up_solid(x, y, 0) == 1)    // only check for solid blocks
                     itemf[c][3] = al_itofix(0);        // if collision kill upwards yinc
                  else itemf[c][3] += al_ftofix(.1);    // else de-accel
               }
               else // not moving up
               {
                  a = is_down_solid(x, y, 1);             // check for block below
                  if (a==0)
                  {
                     itemf[c][3] += al_ftofix(.1);                             // apply gravity to yinc
                     if (itemf[c][3] > al_itofix(3)) itemf[c][3] = al_itofix(3);  // max gravity
                  }
                  if (a) // slow down xinc if block or lift below
                  {
                     if (itemf[c][2] > al_itofix(0)) itemf[c][2] -= al_ftofix(.12);
                     if (itemf[c][2] < al_itofix(0)) itemf[c][2] += al_ftofix(.12);
                  }

                  if ((a==1) || (a==2)) // align with ground if block below
                  {
                     itemf[c][1] = al_itofix((y/20)*20); // align with ground
                     itemf[c][3] = al_itofix(0);  // zero yinc
                  }

                  if (a > 31) // item riding lift
                  {
                     int capture = 0;

                     if (lifts[a-32].fyinc < al_itofix(0)) // lift is moving up
                     {
                        int offset = al_fixtoi(lifts[a-32].fy) - y;   // to prevent lift from picking up early when lift going up
                        if (offset < 21) capture = 1;
                     }
                     if (lifts[a-32].fyinc >= al_itofix(0)) // lift is moving down or steady
                     {
                        if (is_down_solid(x, y, 0)) capture = 0; // to prevent lift attempting to take item down through solid block
                        else capture = 1;
                        int offset = al_fixtoi(lifts[a-32].fy) - y;   // to prevent lift from picking up early when item going down
                        if (offset > 21) capture = 0;
                     }
                     if (capture)
                     {
                        al_fixed lxi = lifts[a-32].fxinc;
                        al_fixed lyi = lifts[a-32].fyinc;

                        itemf[c][0] += lxi;                             // move x with lift's xinc
                        itemf[c][1]  = lifts[a-32].fy - al_itofix(20);  // align with lift's y

                        x = al_fixtoi(itemf[c][0]);
                        y = al_fixtoi(itemf[c][1]);

                        if (lyi > al_itofix(0)) // down
                           if (is_down_solid(x, y, 0))                      // no lift check
                              itemf[c][1] = al_itofix(y - (y % 20));        // item not on lift anymore, align with block

                        if (lyi < al_itofix(0)) // up
                           if (is_up_solid(x, y, 0) == 1)
                              itemf[c][1] += al_itofix(10);

                        if (lxi > al_itofix(0)) // right
                           if (is_right_solid(x, y, 1))
                              itemf[c][0] -= lxi;

                        if (lxi < al_itofix(0)) // left
                           if (is_left_solid(x, y, 1))
                              itemf[c][0] -= lxi;
                     }
                  }  // end of riding lift
               }  // end of not moving up
            } // end of not being carried
         } // end of if not stationary and not lit rocket
      } // end of iterate all active items
}


int player_drop_item(int p)
{
   int pc = players[p].carry_item-1; // number of item
   int wall_stuck = 0;

   players[p].carry_item = 0;


   if (item[pc][0] != 99) // not lit bomb
   {
      // check to see if the item is embedded in the wall
      int x = al_fixtoi(itemf[pc][0] );
      int y = al_fixtoi(itemf[pc][1] );

      if (players[p].left_right) // right
      {
         // is item embedded in a wall to the right?
         while (is_right_solid(x, y, 1))
         {
            x--;
            wall_stuck++; // just how stuck was it?
         }
         itemf[pc][0] = al_itofix(x);
      }

      if (!players[p].left_right) // left
      {
         // is item embedded in a wall to the leftright?
         while (is_left_solid(x, y, 1))
         {
            x++;
            wall_stuck++; // just how stuck was it?
         }
         itemf[pc][0] = al_itofix(x);
       }
   }
   else itemf[pc][1] += al_itofix(2); // when putting bomb in wall, this stops it from snapping to block above
   return wall_stuck;
}

void proc_player_carry(void)
{
   for (int p=0; p<NUM_PLAYERS; p++)
      if (   (players[p].active)
       &&  ((!players[p].paused) || (players[p].paused && players[p].paused_type == 2))
       &&    (players[p].carry_item)   )  // player is carrying item
      {
         int wall_stuck = 0;
         int pc = players[p].carry_item-1; // number of item
         if (item[pc][0] != 98) // not lit rocket
         {
            itemf[pc][1] = players[p].PY - al_itofix(2);
            if (!players[p].left_right) itemf[pc][0] = players[p].PX - al_itofix(15);
            if (players[p].left_right) itemf[pc][0] = players[p].PX + al_itofix(15);
         }
         if (!players[p].fire) // drop
         {
            wall_stuck = player_drop_item(p);

            if (item[pc][0] != 98) // not lit rocket
            {
               if (wall_stuck < 6)
               {
                  itemf[pc][2] = players[p].xinc;  // inherit the players momentum
                  itemf[pc][3] = players[p].yinc;
                  if (players[p].up)    itemf[pc][3] -= al_itofix(6); // throw item upwards
                  if (players[p].left)  itemf[pc][2] -= al_itofix(2); // throw item left
                  if (players[p].right) itemf[pc][2] += al_itofix(2); // throw item right
               }
            }
         }
      }
      else // player is not carrying item
      {
         if (!players[p].paused) // only if not in paused mode
         {
            players[p].draw_rot = al_itofix(0);  // reset rot and scale
            players[p].draw_scale = al_itofix(1);
         }
      }
}

void proc_item_collision()
{
   int c, p, x, y;

   for (p=0; p<NUM_PLAYERS; p++)
      players[p].marked_door = -1;  // so player can touch only one door

   for (x=0; x<500; x++)
      if (item[x][0])
         for (p=0; p<NUM_PLAYERS; p++)
            if ( (players[p].active) && (!players[p].paused) &&
                 (players[p].PX  > itemf[x][0] - al_itofix(16) ) &&
                 (players[p].PX  < itemf[x][0] + al_itofix(16) ) &&
                 (players[p].PY  > itemf[x][1] - al_itofix(16) ) &&
                 (players[p].PY  < itemf[x][1] + al_itofix(16) ) )
            {
               int itx = al_fixtoi(itemf[x][0] );
               int ity = al_fixtoi(itemf[x][1] );

               if ( (!players[p].carry_item)  // not carrying
                 && (item[x][3] < 0)          // item is carryable
                 && (players[p].fire) )       // fire pressed
               {
                  int other_player_carrying = 0;
                  // check to see if another player is already carrying this item
                  for (int op=0; op<NUM_PLAYERS; op++)
                     if ((players[op].active) && (!players[op].paused))
                        if (players[op].carry_item == x+1)
                           other_player_carrying = 1;

                   // allow carry if no other player is carrying
                   if (other_player_carrying == 0) players[p].carry_item = x+1;

                   // allow mutiple player carry for rocket
                   if (item[x][0] == 98) players[p].carry_item = x+1;
               }

               switch (item[x][0]) // item type
               {
                  case 1: // door
                  {
                     if ((players[p].marked_door == -1)  // player has no marked door yet
                       && (players[p].carry_item != x+1)) // player is not carrying this door
                     {
                        players[p].marked_door = x;


                        // item[x][6]  color
                        // item[x][7]  move type (0 = auto, 1 = force instant, 2 = force move
                        // item[x][8]  type (0 = no dest (exit only), 1 = linked dest
                        // item[x][9]  linked destination item
                        // item[x][10] key held flag
                        // item[x][11] door entry type (0 0-immed, 1 = up 2 = down)
                        // item[x][12] draw lines always, never
                        // item[x][13] base animation shape


                        if (item[x][8]) // do nothing if exit only
                        {
                           int do_entry = 0;
                           if (item[x][11] == 0) do_entry = 1; // enter immed
                           if (players[p].carry_item-1 != x) // cant trigger entry if carrying this door
                           {
                              if (item[x][11] == 1) // enter with <up>
                              {
                                 // to prevent immediate triggering when destination door, wait for release and re-press

                                 // if key held is old, ignore
                                 if (item[x][10] && item[x][10] < passcount-1) item[x][10] = 0;

                                 // up pressed and !pressed last frame
                                 if ((players[p].up) && (!item[x][10])) do_entry = 1;

                                 if (players[p].up) item[x][10] = passcount;
                                 else item[x][10] = 0;

                              }

                              if (item[x][11] == 2) // enter with <down>
                              {
                                 // to prevent immediate triggering when destination door, wait for release and re-press

                                 // if key held is old, ignore
                                 if (item[x][10] && item[x][10] < passcount-1) item[x][10] = 0;

                                 // down pressed and !pressed last frame
                                 if ((players[p].down) && (!item[x][10])) do_entry = 1;

                                 if (players[p].down) item[x][10] = passcount;
                                 else item[x][10] = 0;
                              }
                           }

                           if (do_entry)
                           {
                              int bad_exit = 0;
                              item[x][10] = 0; // clear the key hold for the door you just left

                             // check if dest item is same as source item
                              if (item[x][9] == x) bad_exit = 1;


                              // is player carrying an item ?
                              if (players[p].carry_item)
                              {
                                  int ci = players[p].carry_item - 1;

                                 // check to see if player is carrying this door
                                  if (ci == x) player_drop_item(p);

                                 // check to see if player is carrying an item without the carry through door flag set
                                 if (item[ci][3] != -2)  player_drop_item(p);

                              }

                              // get the destination
                              al_fixed dx = al_itofix(0), dy = al_itofix(0);
                              int li = item[x][9]; // linked item number

                              if ((li > -1) && (li < 500))
                              {
                                 dx = itemf[li][0];
                                 dy = itemf[li][1];
                              }
                              else bad_exit = 1;


                              if ( (dx < al_itofix(0)) || (dx > al_itofix(1980)) ) bad_exit = 1;
                              if ( (dy < al_itofix(0)) || (dy > al_itofix(1980)) ) bad_exit = 1;

                              if (!bad_exit)
                              {
                                 event(32, itx, ity, 0, 0, 0, 0 ); // In Door

                                 int instant_move = 0;
                                 if (item[x][7] == 0) // 0 = auto
//                                    if ((item[x][3]) || (item[li][3])) // if source or dest are not stat
                                    if (item[li][3]) // if dest is not stat
                                       instant_move = 1;

                                 if (item[x][7] == 1) instant_move = 1; // 1 = force instant
                                 if (item[x][7] == 2) instant_move = 0; // 2 = force move

                                 if (instant_move)
                                 {
                                    players[p].PX = itemf[li][0];
                                    players[p].PY = itemf[li][1];
                                    item[li][10] = passcount;
                                 }
                                 else
                                 {
                                    // snap player to the source door
                                    players[p].PX = itemf[x][0];
                                    players[p].PY = itemf[x][1];

                                    players[p].right_xinc=al_itofix(0);
                                    players[p].left_xinc=al_itofix(0);

                                    // set player's xinc and yinc
                                    al_fixed xlen = players[p].PX - dx;                // get the x distance between player and exit
                                    al_fixed ylen = players[p].PY - dy;                // get the y distance between player and exit
                                    al_fixed hy_dist =  al_fixhypot(xlen, ylen);          // hypotenuse distance
                                    al_fixed speed = al_itofix(15);                       // speed
                                    al_fixed scaler = al_fixdiv(hy_dist, speed);          // get scaler
                                    players[p].door_xinc = al_fixdiv(xlen, scaler);    // calc xinc
                                    players[p].door_yinc = al_fixdiv(ylen, scaler);    // calc yinc

                                    // get rotation from players xinc, yinc
                                    players[p].door_draw_rot = al_fixatan2(players[p].door_yinc, players[p].door_xinc) - al_itofix(64);

                                    // get the number of steps
                                    al_fixed ns;
                                    if (abs(xlen) > abs(ylen)) ns = al_fixdiv(xlen, players[p].door_xinc);
                                    else  ns = al_fixdiv(ylen, players[p].door_yinc);
                                    int num_steps = al_fixtoi(ns);

                                    if ((num_steps > 0) && (num_steps < 2000))
                                    {
                                       players[p].door_draw_rot_num_steps = 12;
                                       int ddrns = players[p].door_draw_rot_num_steps;
                                       players[p].door_num_steps = num_steps;
                                       players[p].paused = 1;
                                       players[p].paused_type = 2;
                                       players[p].paused_mode = 1;
                                       players[p].paused_mode_count = ddrns;
                                       players[p].door_item = x;
                                       players[p].door_draw_rot_inc = al_fixdiv(players[p].door_draw_rot, al_itofix(ddrns));
                                    }
                                    // printf("ns:%d xinc:%3.2f yinc:%3.2f \n", num_steps, al_fixtof(players[p].xinc), al_fixtof(players[p].yinc));
                                 } // end of if not instant move
                              }  // end of if not bad exit
                           } // end of do entry
                        } // end of if not exit only
                     } // end of if not first door touched
                  }
                  break;
                  case 2: // health bonus
                  {
                     if (item[x][7])
                     {
                        al_fixed f100 = al_itofix(100);
                        if (players[p].LIFE < f100)
                        {
                           item[x][0] = 0;
                           players[p].LIFE += al_itofix(item[x][7]);
                           if (players[p].LIFE > f100) players[p].LIFE = f100;
                           event(6, itx, ity, item[x][7], 0, 0, 0);
                        }
                        else  event(26, itx, ity, 0, 0, 0, 0); // already have 100 health
                     }
                  }
                  break;
                  case 3: // exit
                  {
                     int exit_enemys_left = num_enemy - item[x][8];

                     if (exit_enemys_left <= 0)
                     {
                        level_done = 1;
                        event(4, itx, ity, 0, 0, 0, 0);
                     }
                     else event(3, itx, ity, exit_enemys_left, 0, 0, 0); // not enough dead yet
                  }
                  break;
                  case 4: // key
                  {
                     event(2, itx, ity, p, item[x][1], 0, 0);   // send player and item shape
                     int x2 = (item[x][6] + item[x][8]) * 10;   // get the center of the block range
                     int y2 = (item[x][7] + item[x][9]) * 10;
                     al_fixed xlen = al_itofix(x2) - itemf[x][0];     // distance between block range and key
                     al_fixed ylen = al_itofix(y2) - itemf[x][1];
                     al_fixed hy_dist =  al_fixhypot(xlen, ylen);     // hypotenuse distance
                     al_fixed speed = al_itofix(12);                  // speed
                     al_fixed scaler = al_fixdiv(hy_dist, speed);     // get scaler
                     al_fixed xinc = al_fixdiv(xlen, scaler);         // calc xinc
                     al_fixed yinc = al_fixdiv(ylen, scaler);         // calc yinc
                     itemf[x][2] = xinc;
                     itemf[x][3] = yinc;
                     al_fixed angle = al_fixatan2(ylen, xlen);        // get the angle for item rotation
                     item[x][10] = al_fixtoi(angle) * 10;
                     al_fixed ns;                                  // get the number of steps
                     if (abs(xlen) > abs(ylen)) ns = al_fixdiv(xlen, xinc);
                     else  ns = al_fixdiv(ylen, yinc);
                     int num_steps = al_fixtoi(ns);
                     item[x][11] = num_steps + 10;              // add 10 for final sequence
                  }
                  break;
                  case 6: // free man
                  {
                     item[x][0] = 0;
                     players[p].LIVES++;
                     event(9, itx, ity, 0, 0, 0, 0);
                  }
                  break;
                  case 7: // mine
                  {
                     players[p].LIFE -= al_itofix(item[x][8]) / 10;
                     event(10, itx, ity, 0, 0, 0, 0);
                     event(7, itx, ity, item[x][8], 0, 0, 0);
                  }
                  break;
                  case 8: // un-lit bomb
                  {
                     item[x][0] = 99; // change to lit bomb
                     item[x][6] = 1;  // mode == lit
                     item[x][8] = item[x][9]; // fuse wait count
                     item[x][10] = 100; // default scale = 1.00
                  }
                  break;
                  case 10: // pop-up message
                  {
                     item[x][6] = item[x][7]; // set the message display timer
                  }
                  break;
                  case 11: // rocket
                  {
                     event(25, itx, ity, 0, 0, 0, 0);
                     item[x][0] = 98;   // new type - lit rocket
                     item[x][1] = 1026; // new ans
                     item[x][3] = -1;   // carryable
                     itemf[x][3] = 0;   // stop if falling
                  }
                  break;
                  case 12: // warp
                  {
                     extern int start_mode;
                     play_level = item[x][8];
                     start_mode = 1;
                  }
                  break;
                  case 14: // switch
                  {
                     if (item[x][7] < passcount) // if not lockout
                     {
                        // if falling and landing on it
                        if ( (players[p].PX  > itemf[x][0] - al_itofix(12)) &&
                             (players[p].PX  < itemf[x][0] + al_itofix(12)) &&
                             (players[p].PY  > itemf[x][1] - al_itofix(16)) &&
                             (players[p].PY  < itemf[x][1] - al_itofix(8)) &&
                             (players[p].yinc > al_itofix(0)) )  // falling
                        {
                           event(30, itx, ity, 0, 0, 0, 0);
                           item[x][7] = passcount + 4; // switch lockout for next 4 frames
                           item[x][6] = !item[x][6];
                           if (item[x][6]) item[x][1] = item[x][8]; // on bmp
                           else            item[x][1] = item[x][9]; // off bmp
                           al_set_target_bitmap(l2000);
                           // toggle blocks
                           for (c=0; c<100; c++)
                              for (y=0; y<100; y++)
                              {
                                 if (l[c][y] == item[x][11]) // empty switch block
                                 {
                                    l[c][y] = item[x][10]; // replace with solid switch block
                                    al_draw_filled_rectangle(c*20, y*20, c*20+19, y*20+19, palette_color[0]);
                                    al_draw_bitmap(memory_bitmap[l[c][y]], c*20, y*20, 0 );
                                 }
                                 else if (l[c][y] == item[x][10]) // solid switch block
                                 {
                                    l[c][y] = item[x][11]; // replace with empty switch block
                                    //rectfill(l2000, c*20, y*20, c*20+19, y*20+19, 0);
                                    //draw_sprite(l2000, memory_bitmap[l[c][y]], c*20, y*20);
                                    al_draw_filled_rectangle(c*20, y*20, c*20+19, y*20+19, palette_color[0]);
                                    al_draw_bitmap(memory_bitmap[l[c][y]], c*20, y*20, 0 );
                                 }

                              } // end of toggle blocks
                           draw_lift_lines();
                        }  // end of falling and landing on
                     } // end of if not lockout
                  }
                  break;

                  case 15: // sproingy
                  {
                     if ( (players[p].PX  > itemf[x][0] - al_itofix(10)) &&
                          (players[p].PX  < itemf[x][0] + al_itofix(10)) &&
                          (players[p].PY  > itemf[x][1] - al_itofix(16)) &&
                          (players[p].PY  < itemf[x][1] - al_itofix(8)) &&
                          (players[p].yinc > al_itofix(0)) && // falling
                          (players[p].jump) )   //  jump pressed
                     {
                        event(31, itx, ity, 0, 0, 0, 0);
                        players[p].yinc = al_itofix(0) - al_fixdiv(al_itofix(item[x][7]), al_ftofix(7.1));
                     }
                  }
                  break;
               } // end of switch case
            } // end of player collision with active item
}

void do_bomb_damage(int i)
{
   al_fixed b = al_itofix(item[i][7]);
   al_fixed x = itemf[i][0];
   al_fixed y = itemf[i][1];
   al_fixed x1 = x - b + al_itofix(10);
   al_fixed x2 = x + b + al_itofix(10);
   al_fixed y1 = y - b + al_itofix(10);
   al_fixed y2 = y + b + al_itofix(10);
   for (int e=0; e<100 ; e++) // enemies in damage window?
      if (Ei[e][0])
         if ((Efi[e][0] > x1) && (Efi[e][0] < x2) && (Efi[e][1] > y1) && (Efi[e][1] < y2))
         {
            Ei[e][31] = 2; // set bomb hit
            event(23, x, y, 0, 0, 0, 0);
         }
   for (int p=0; p<NUM_PLAYERS; p++) // players in damage window
      if ((players[p].active) && (!players[p].paused) )
         if ((players[p].PX > x1) && (players[p].PX < x2) && (players[p].PY > y1) && (players[p].PY < y2))
         {
            al_fixed x_dist = abs(players[p].PX - x);
            al_fixed y_dist = abs(players[p].PY - y);
            al_fixed distance = al_fixhypot(x_dist, y_dist);
            al_fixed damage = al_fixdiv(b, distance); // damage window size / distance (scale of 1 - 4)
            damage *= 20; // multiple by 20 (scale of 20-80)
            if (damage > al_itofix(80)) damage = al_itofix(80);
            players[p].LIFE -= damage;
            event(7, al_fixtoi(players[p].PX), al_fixtoi(players[p].PY), al_fixtoi(damage), 0, 0, 0);
            event(8, al_fixtoi(players[p].PX), al_fixtoi(players[p].PY), 0, 0, 0, 0);
         }

   // remove bombable blocks on level
   int c = al_fixtoi(b);
   int xi = al_fixtoi(x);
   int yi = al_fixtoi(y);
   // convert to 0-100 range
   c /= 20;
   xi += 10; xi /= 20;
   yi += 10; yi /= 20;
   for (int e = (xi-c); e < (xi+c)+1; e++)        // cycle blast range of blocks
      for (int f = (yi-c); f < (yi+c)+1; f++)
        if ((e>0) && (e<99) && (f>0) && (f<99))   // if within level boundaries (minus outer block frame)
            if ((l[e][f] > 63) && (l[e][f] < 96)) // if bombable block
               remove_block(e, f);

   draw_lift_lines();
}

void proc_lit_bomb(void)
{
   for (int c=0; c<500; c++)
      if (item[c][0] == 99) // lit bomb
      {
         int fuse_seq = 73;
         int expl_seq = 97;
         item[c][8]--; // proc timer
         lit_item = 1;
         if (item[c][6] == 1) // fuse burning
         {
            // i can ignore these floats because all they do is set the shape from animation sequence
            float r = (float)item[c][8] / (float)item[c][9]; // ratio done
            int num_seq_shapes = zz[4][fuse_seq]; // number of shapes in seq
            int si = (int)(r * (float)num_seq_shapes); // ratio * number of shapes
            int sh = zz[num_seq_shapes+5-si][fuse_seq]; // shape
            item[c][1] = sh; // set in item

            if (item[c][8] < 1) // fuse done
            {
               item[c][6] = 2; // mode 2; explosion
               item[c][8] = item[c][9] = 20; // explosion timer


               // check for other co-located bombs
               for (int cc=0; cc<500; cc++)
                  if (item[cc][0] == 99) // lit bomb
                     if (cc != c) // not this one
                        if (itemf[cc][0] == itemf[c][0]) // compare x
                           if (itemf[cc][1] == itemf[c][1]) // compare y
                              if (item[cc][15] == item[c][15]) // check if created by same cloner
                                 item[cc][0] = 0; // delete item


            }
         }
         if (item[c][6] == 2) // explosion
         {
            // force player to drop item
            for (int p=0; p<NUM_PLAYERS; p++)
               if ((players[p].active) && (!players[p].paused) )
                  if (players[p].carry_item-1 == c) players[p].carry_item = 0;

            // i can ignore these floats because all they do is set the shape from animation sequence
            float r = (float)item[c][8] / (float)item[c][9]; // ratio done
            //r = round(r/0.01) * 0.01; //round
            int num_seq_shapes = zz[4][expl_seq]; // number of shapes in seq
            int si = (int)(r * (float)num_seq_shapes); // ratio * number of shapes
            int sh = zz[num_seq_shapes+5-si][expl_seq];
            item[c][1] = sh;


            int b = item[c][7]; // damage
            // i can ignore these floats because all they do is set the scale the shape is drawn at
            float max_scale = b/10; // max scale
            //max_scale = round(max_scale/0.01) * 0.01; // round
            float scale = 3.5 + (1-r)*max_scale;
            //scale = round(scale/0.01) * 0.01;   //round
            float scale100 = scale * 100;
            //scale100 = round(scale100/10) * 10; //round
            item[c][10] = (int)scale100;

            //printf("[%d]explosion ratio:%f seq_shape:%d scale:%f 10:%d \n", c, r, si, scale, item[c][10]);

            if (item[c][8] == 12) // explosion sound
               event(22, 0, 0, 0, 0, 0, 0);

            if (item[c][8] == 6) // do damage
               do_bomb_damage(c);

            if (item[c][8] < 1) // explosion timer done
               item[c][0] = 0; // erase item
         }

      }
}

void proc_lit_rocket(void)
{
   for (int c=0; c<500; c++)
      if (item[c][0] == 98) // lit rocket!
      {
         int this_rocket_just_expoded = 0;
         int x, y, p;
         int max_speed = item[c][8]*1000;
         int accel = item[c][9];
         int rot_inc = item[c][6];
         lit_item = 1;

         if (item[c][11] < max_speed) item[c][11]+=accel;   // speed and accel

         al_fixed angle = al_itofix((item[c][10]-640) / 10);
         itemf[c][2] = (al_fixcos(angle) * item[c][11]) / 1000;       // hypotenuse is the speed!
         itemf[c][3] = (al_fixsin(angle) * item[c][11]) / 1000;

         x = al_fixtoi(itemf[c][0] += itemf[c][2]); // do the increments
         y = al_fixtoi(itemf[c][1] += itemf[c][3]);

         // check for wall collisions
         if ( ((itemf[c][3]<al_itofix(0)) && (is_up_solid(     x, y, 0)     == 1)) ||
              ((itemf[c][3]>al_itofix(0)) && (is_down_solid(   x, y, 0)     == 1)) ||
              ((itemf[c][3]>al_itofix(0)) && (is_down_solid(   x, y, 0)     == 2)) ||
              ((itemf[c][2]<al_ftofix(-1.1)) && (is_left_solid(x, y, 0)     == 1)) ||
              ((itemf[c][2]>al_ftofix(1.1)) && (is_right_solid(x, y, 0)     == 1)) )
         {
            this_rocket_just_expoded = 1;
            item[c][0] = 99;  // change to lit bomb
            item[c][2] = 3;  // draw mode
            item[c][6] = 2; // mode 2; explosion
            item[c][8] = 20; // explosion timer count
            item[c][9] = 20; // explosion timer limit
            item[c][10] = 100; // start_size
         }

         for (p=0; p<NUM_PLAYERS; p++)
            if ((players[p].active) && (c == (players[p].carry_item-1))) // player is riding this rocket!
            {
               if (this_rocket_just_expoded)
               {
                  players[p].carry_item = 0;
                  players[p].fire_held = 0;
               }
               else
               {
                  if (players[p].left)  item[c][10]-=rot_inc;
                  if (players[p].right) item[c][10]+=rot_inc;
                  players[p].xinc = players[p].yinc = 0;
                  players[p].left_xinc = players[p].right_xinc = 0;
                  players[p].xinc = players[p].yinc = 0;
                  players[p].PX = itemf[c][0];
                  players[p].PY = itemf[c][1];

                  players[p].draw_rot = al_itofix(item[c][10]/10);
                  players[p].draw_scale = al_ftofix(.5);


               }
            }
      }
}











/*
item variable descriptions

list of items



[1] - door
[2] - bonus
[3] - exit
[4] - key
[5] - start
[6] - free man
[7] - mine
[8] - bomb
[10] - pop-up msg
[11] - rocket
[12]  - warp
[14]  - switch
[15] - sproingy
[98] - lit rocket
[99] - lit bomb











// common

item[][0] = active and type
item[][1] = bitmap or ans
item[][2] = draw type (not used)
item[][3] = (0=stat, 1=fall, -1=carry, -2=carry through door)
item[][4] = x pos (int) (2000)
item[][5] = y pos (int) (2000)

item[][14] = time to live
item[][15] = tag with cloner item id





// type specific

[1] - door
item[][6]  color
item[][7]  move type (0=auto, 1=force instant, 2=force move)
item[][8]  type (0=exit only, 1=linked dest)
item[][9]  linked destination item
item[][10] key held flag
item[][11] door entry type (0=immed, 1=up, 2=down)
item[][12] draw lines always, never, only when touched
item[][13] base animation shape


[2] - bonus
item[][7] health bonus
item[][8] bullet bonus (ignored now)
item[][9] timer bonus  (ignored now)

[3] - exit
item[][8] exit with x enemies left

[4] - key
item[][6]  block range x (100)
item[][7]  block range y (100)
item[][8]  block range width (100)
item[][9]  block range height (100)
item[][10] rotation for key move
item[][11] counter for key move
item[][12] matching keyed blocks only

[5] - start
item[][8] initial time (ignored now)

[6] - free man

[7] - mine
item[][8] mine damage

[8] - bomb
item[][6]  mode (2=explosion)
item[][7]  blast size
item[][8]  fuse length counter
item[][9]  fuse length value
item[][10] scale (for explosion)





[10] - pop-up msg
item[][6]  timer counter
item[][7]  timer value
item[][8]  text color
item[][9]  frame color
item[][10] msg x pos (100)
item[][11] msg y pos (100)

[11] - rocket
item[][6]  steerabaility
item[][7]  blast size
item[][8]  max speed
item[][9]  accel
item[][10] rocket rotation (scaled by 10)

[12]  - warp
item[][8] warp level

[14]  - switch
item[][6]  on/off
item[][7]  lockout
item[][8]  on bmp
item[][9]  off bmp
item[][10] solid block shape
item[][11] empty block shape

[15] - sproingy
item[][6] jump length (only used in level editor)
item[][7] sproinginess




[98] - lit rocket


[99] - lit bomb
item[][6]  mode (1=lit, 2=explosion)
item[][7]  blast size
item[][8]  fuse length counter
item[][9]  fuse length value
item[][10] scale (for explosion)







*/



























