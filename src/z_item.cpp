// zitem.cpp

#include "pm.h"

int item_data(int x, int y)
{
   int inum = sort_item();
                               al_draw_textf(font, palette_color[12], x, y, 0, "%d Items",      inum);                 y+=8;
                               al_draw_textf(font, palette_color[12], x, y, 0, "--------");                            y+=8;
                               al_draw_textf(font, palette_color[13], x, y, 0, "%d Starts",     item_num_of_type[5]);  y+=8;
                               al_draw_textf(font, palette_color[13], x, y, 0, "%d Exits",      item_num_of_type[3]);  y+=8;
   if (item_num_of_type[12]) { al_draw_textf(font, palette_color[10], x, y, 0, "%d Warps",      item_num_of_type[12]); y+=8; }
   if (item_num_of_type[1])  { al_draw_textf(font, palette_color[13], x, y, 0, "%d Doors",      item_num_of_type[1]);  y+=8; }
   if (item_num_of_type[4])  { al_draw_textf(font, palette_color[13], x, y, 0, "%d Keys",       item_num_of_type[4]);  y+=8; }
   if (item_num_of_type[14]) { al_draw_textf(font, palette_color[13], x, y, 0, "%d Switches",   item_num_of_type[14]); y+=8; }
   if (item_num_of_type[15]) { al_draw_textf(font, palette_color[13], x, y, 0, "%d Sproingies", item_num_of_type[15]); y+=8; }
   if (item_num_of_type[8])  { al_draw_textf(font, palette_color[14], x, y, 0, "%d Bombs",      item_num_of_type[8]);  y+=8; }
   if (item_num_of_type[11]) { al_draw_textf(font, palette_color[14], x, y, 0, "%d Rockets",    item_num_of_type[11]); y+=8; }
   if (item_num_of_type[7])  { al_draw_textf(font, palette_color[14], x, y, 0, "%d Mines",      item_num_of_type[7]);  y+=8; }

   for (int c=1; c<16; c++)
      if ((c != 5) && (c !=3) && (c!= 12) && (c!= 1) && (c!= 4) && (c!= 14) && (c!= 15) && (c!= 8) && (c!= 11) && (c!= 7))
         if (item_num_of_type[c]) // not zero
         {
                         sprintf(msg, "%d type %d???  ", item_num_of_type[c], c); // default unknown
            if (c ==  0) sprintf(msg, "%d type 0      ", item_num_of_type[c]);
            if (c ==  2) sprintf(msg, "%d Bonus       ", item_num_of_type[c]);
            if (c ==  6) sprintf(msg, "%d Free Men    ", item_num_of_type[c]);
            if (c == 10) sprintf(msg, "%d Messages    ", item_num_of_type[c]);
            al_draw_text(font, palette_color[3], x, y, 0, msg);
            y+=8;
         }
   return y;
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
   al_set_target_bitmap(level_background);
   al_draw_filled_rectangle(x*20, y*20, x*20+20, y*20+20, palette_color[0]);
   al_draw_bitmap(tile[0], x*20, y*20, 0);
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
   for (int a=0; a<12; a++)           // frame
      al_draw_filled_rounded_rectangle(px1+a, py1+a, px2-a, py2-a, 4, 4, palette_color[fc+a*16]);

   int tc = item[c][8];               // text color
   for (row=0; row<=num_lines; row++) // text
      al_draw_text(font, palette_color[tc], pxc+4, py1+row*8+9, ALLEGRO_ALIGN_CENTRE, dt[row]);
}

void draw_door(int c, int x, int y)
{
   ALLEGRO_BITMAP *tmp = NULL;
   int col = item[c][6];
   if (item[c][13] == 448) // old style door shape
   {
      int shape = item[c][1];       // get shape
      int si = shape-448;           // convert to index to bitmap sequence
      tmp = door_tile[1][col][si];
   }
   else // new style doors
   {
      int an = zz[1][83];             // cheat and use shape index from base door animation sequence
      if (item[c][8] == 0) an = 7-an; // exit only, run the sequence backwards
      tmp = door_tile[0][col][an];
   }

   int drawn = 0;
   if (item[c][8] == 1)  // linked item destination
   {
      // linked destination item position
      int dx = al_fixtoi(itemf[item[c][9]][0]);
      int dy = al_fixtoi(itemf[item[c][9]][1]);
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

            if (item[c][8] == 0) al_draw_scaled_bitmap(tile[1015], 0, 0, 20, 20, x-5, y-6, 30, 26, 0); // OUT
            else al_draw_scaled_bitmap(tile[1014], 0, 0, 20, 20, x-5, y-6, 30, 26, 0); // IN

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

      if (item[c][8] == 0) al_draw_bitmap(tile[1015], x, y, 0); // OUT
      else al_draw_bitmap(tile[1014], x, y, 0); // IN
   }
}

void bomb_blocks(int b, int t)
{
   // cycle blocks in blast radius of bomb

   int c = item[b][7];                // raw radius


   // are we in exposion mode?
   // item[c][6] == 1 // fuse burning
   // item[c][6] == 2 // explosion


   if (item[b][6] == 2) // explosion
   {
      float r = 1 - (float) item[b][8] / (float)item[b][9]; // ratio from 0 to 1
      float sr = (float) item[b][7] * r; // scaled radius
      c = (int) sr;
      //printf("8:%d r:%f \n", item[b][8], r);
   }

   int xi = al_fixtoi(itemf[b][0]);   // position
   int yi = al_fixtoi(itemf[b][1]);


   if (t == 1) // mark blocks
   {
      //temp mark blast circle
      al_draw_circle(xi+10, yi+10, c, palette_color[10], 1);
   }


   // convert to 0-100 range
   c /= 20;
   int cc = c*c;
   xi += 10; xi /= 20;
   yi += 10; yi /= 20;


//printf("\nc:%d cc:%d xi:%d yi:%d\n", c, cc, xi, yi);




   for (int e = (xi-c); e < (xi+c)+1; e++)        // cycle blast range of blocks
      for (int f = (yi-c); f < (yi+c)+1; f++)
      {
         // check radius from center
         int xd = abs(xi-e);
         int yd = abs(yi-f);
         int br = xd*xd+yd*yd;

//         if (t ==1) al_draw_rectangle(e*20, f*20, e*20+19, f*20+19, palette_color[11], 1);

  //       printf("\ne:%d f:%d xd:%d yd:%d br:%d", e, f, xd, yd, br);

         if (br < cc)
         {
            if (t == 1) // mark blocks
            {
               if ((l[e][f] > 63) && (l[e][f] < 96))
               {
                  al_draw_rectangle(e*20, f*20, e*20+20, f*20+20, palette_color[10+96], 1);
                  al_draw_line(e*20, f*20, e*20+19, f*20+19, palette_color[10], 1);
                  al_draw_line(e*20, f*20+19, e*20+19, f*20, palette_color[10], 1);
               }
            }
            if (t == 2) // remove blocks
            {
               if ((l[e][f] > 63) && (l[e][f] < 96)) remove_block(e, f);
            }
         }
      }
}


void bomb_enemies(int b, int t)
{
   float r = 1; // default ratio
   if (item[b][6] == 2) r = 1 - (float)item[b][8] / (float)item[b][9]; // explosion happening

   float max_damage = (float) item[b][7];    // max damage range
   al_fixed sr =  al_ftofix(max_damage * r); // damage range scaled by ratio

   for (int e=0; e<100 ; e++) // enemies in damage window?
      if (Ei[e][0])
      {
         al_fixed dist = al_fixhypot( (Efi[e][0] - itemf[b][0]), (Efi[e][1] - itemf[b][1]) );
         if (dist < sr)
         {
            if (t == 1)
            {
               float ex = al_fixtof(Efi[e][0]);
               float ey = al_fixtof(Efi[e][1]);

               al_draw_rectangle(ex, ey, ex+19, ey+19, palette_color[11], 1);
               al_draw_line(ex, ey, ex+19, ey+19, palette_color[11], 1);
               al_draw_line(ex, ey+19, ex+19, ey, palette_color[11], 1);
            }
            if (t == 2)
            {
               Ei[e][31] = 2; // set bomb hit
               game_event(23, 0, 0, 0, 0, 0, 0);
            }
         }
      }
}


void bomb_players(int b, int t)
{
   float r = 1; // default ratio
   if (item[b][6] == 2) r = 1 - (float)item[b][8] / (float)item[b][9]; // explosion happening

   float max_damage = (float) item[b][7];    // max damage range
   al_fixed sr =  al_ftofix(max_damage * r); // damage range scaled by ratio



   for (int p=0; p<NUM_PLAYERS; p++)
      if ((players[p].active) && (!players[p].paused) )
      {
         al_fixed dist = al_fixhypot( (players[p].PX - itemf[b][0]), (players[p].PY - itemf[b][1]) );
         if (dist < sr)
         {
            if (t == 1)
            {

               float px = al_fixtof(players[p].PX);
               float py = al_fixtof(players[p].PY);

               al_draw_rectangle(px, py, px+19, py+19, palette_color[11], 1);
               al_draw_line(px, py, px+19, py+19, palette_color[11], 1);
               al_draw_line(px, py+19, px+19, py, palette_color[11], 1);



            }
            if ((t == 2) && (item[b][8] == 1)) // only once do damage
            {
               al_fixed damage = al_fixdiv(sr, dist); // damage window size / distance (scale of 1 - 4)
               damage *= 20; // multiple by 20 (scale of 20-80)
               if (damage > al_itofix(80)) damage = al_itofix(80);
               players[p].LIFE -= damage;
               game_event(7, al_fixtoi(players[p].PX), al_fixtoi(players[p].PY), al_fixtoi(damage), 0, 0, 0);
               game_event(8, al_fixtoi(players[p].PX), al_fixtoi(players[p].PY), 0, 0, 0, 0);
            }
         }
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
            al_draw_scaled_rotated_bitmap(tile[shape],10, 10, x+10, y+10, sc, sc, rot, 0);
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

            float seq = 9 - item[c][11]; // starts at 0, goes to 8
            int xo = (int)(seq * xinc / 2);
            int yo = (int)(seq * yinc / 2);
            al_draw_rectangle(x1+xo, y1+yo, x2-xo, y2-yo, palette_color[15], 1);
         }

        if (item[c][0] == 99) // stretch draw for lit bombs only
        {
            int expl_seq = 97;
            int fuse_seq = 73;
            float sc = 1;
            float r = 1 - (float)item[c][8] / (float)item[c][9]; // ratio done


            if (item[c][6] == 1) // fuse burning
            {
               int num_seq_shapes = zz[4][fuse_seq];       // number of shapes in seq
               int si = (int)(r * (float)num_seq_shapes);  // ratio * number of shapes
               shape = zz[5+si][fuse_seq];
               printf("ratio:%f shape_index:%d\n", r, si);
            }

            if (item[c][6] == 2) // explosion mode
            {
               int num_seq_shapes = zz[4][expl_seq] + 1;   // number of shapes in seq
               int si = (int)(r * (float)num_seq_shapes);  // ratio * number of shapes
               shape = zz[5+si][expl_seq];

               // set size of explosion
               float max_scale = item[c][7] / 10;         // max scale = damage / 10
               sc = .5 + (r) * max_scale;
               printf("ratio:%f shape_index:%d scale:%f \n", r, si, sc);
            }

            al_draw_scaled_rotated_bitmap(tile[shape], 10, 10, x+10, y+10, sc, sc, 0, 0);
            drawn = 1;

            bomb_blocks(c, 1); // mark blocks that will be destroyed
            bomb_enemies(c, 1); // mark enemies that will be destroyed
            bomb_players(c, 1);



            // draw sequence numbers
            sprintf(msg, "%d / %d  %f ", item[c][8], item[c][9], (float)item[c][8]/(float)item[c][9]);
            al_draw_text(font, palette_color[15], x, y-20, 0, msg);

         }

          // these types need rotation
         if ((item[c][0] == 11) || (item[c][0] == 98) ||  // rockets
            ((item[c][0] == 4) && (item[c][11] > 0))) // moving key
         {
            float rot = al_fixtof(al_fixmul(al_itofix(item[c][10]/10), al_fixtorad_r));
            al_draw_rotated_bitmap(tile[shape], 10, 10, x+10, y+10, rot, 0);
            drawn = 1;
         }

         if (item[c][0] == 3) // exit
         {
            al_draw_bitmap(tile[399], x, y, 0); // 'exit' text not shown
            if (frame_num % 60 > 30)
               al_draw_text(f3, palette_color[10], x+11, y-2, ALLEGRO_ALIGN_CENTER, "EXIT");

            int exit_enemys_left = num_enemy - item[c][8];
            if (exit_enemys_left > 0) // locked
            {
               al_draw_bitmap(tile[366], x, y, 0); // show lock
               if (frame_num % 60 < 30)
                  al_draw_textf(f3, palette_color[14], x+11, y-2, ALLEGRO_ALIGN_CENTER, "%d", exit_enemys_left);

            }
            drawn = 1;
         }

         // default draw if nothing else has drawn it up to now
         if (!drawn) al_draw_bitmap(tile[shape], x, y, 0);


         #ifdef SHOW_BOMB_DAMAGE_BOX
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
                     al_fixed distance = al_fixhypot(x_dist, y_dist);
                     al_fixed damage = al_fixdiv(b, distance); // damage window size / distance (scale of 1 - 4)
                     damage *= 20; // multiple by 20 (scale of 20-80)
                     if (damage > al_itofix(80)) damage = al_itofix(80);

                     sprintf(msg, "b:%d d:%d  damage:%d ", al_fixtoi(b), al_fixtoi(distance), al_fixtoi(damage) );
                     al_draw_text(font, palette_color[color], al_fixtof(x1)+10, al_fixtof(y1), 0, msg);
                   }

            for (int e=0; e<100 ; e++) // enemies in damage window?
               if (Ei[e][0])
                  if ((Efi[e][0] > x1) && (Efi[e][0] < x2) && (Efi[e][1] > y1) && (Efi[e][1] < y2))
                     color = 13;
            al_draw_rectangle(al_fixtof(dx1), al_fixtof(dy1), al_fixtof(dx2), al_fixtof(dy2), palette_color[color], 2);
         }
         #endif
      } // end of active item iterate
}


void move_items()
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

         if (item[c][0] == 99) proc_lit_bomb(c);
         if (item[c][0] == 98) proc_lit_rocket(c);

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
   // printf("drop item:%d\n", pc);

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

void proc_player_carry(int p)
{
   if ((players[p].active) && (players[p].carry_item))
      if (!players[p].paused || (players[p].paused && players[p].paused_type == 2))// player is carrying item
      {
         int pc = players[p].carry_item-1; // number of item
         if (item[pc][0] != 98)            // not lit rocket
         {
            // set item position relative to player that's carrying it
            itemf[pc][1] = players[p].PY - al_itofix(2);
            if (!players[p].left_right) itemf[pc][0] = players[p].PX - al_itofix(15);
            if (players[p].left_right) itemf[pc][0] = players[p].PX + al_itofix(15);
         }
         if (!players[p].fire) // drop
         {
            if (player_drop_item(p) < 6)
            {
               if (item[pc][0] != 98)            // not lit rocket
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
}


void proc_door_collision(int p, int x)
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
               if (item[x][10] && item[x][10] < frame_num-1) item[x][10] = 0;

               // up pressed and !pressed last frame
               if ((players[p].up) && (!item[x][10])) do_entry = 1;

               if (players[p].up) item[x][10] = frame_num;
               else item[x][10] = 0;

            }

            if (item[x][11] == 2) // enter with <down>
            {
               // to prevent immediate triggering when destination door, wait for release and re-press

               // if key held is old, ignore
               if (item[x][10] && item[x][10] < frame_num-1) item[x][10] = 0;

               // down pressed and !pressed last frame
               if ((players[p].down) && (!item[x][10])) do_entry = 1;

               if (players[p].down) item[x][10] = frame_num;
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
                //printf("do entry, player is carrying item:%d\n", ci);

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
               game_event(32, al_fixtoi(itemf[x][0]), al_fixtoi(itemf[x][1]), 0, 0, 0, 0);

               int instant_move = 0;
               if (item[x][7] == 0) // 0 = auto
                  if (item[li][3]) // if dest is not stat
                     instant_move = 1;

               if (item[x][7] == 1) instant_move = 1; // 1 = force instant
               if (item[x][7] == 2) instant_move = 0; // 2 = force move

               if (riding_rocket(p)) instant_move = 1;


               if (instant_move)
               {
                  players[p].PX = itemf[li][0];
                  players[p].PY = itemf[li][1];

                  if (riding_rocket(p))
                  {
                     int c = players[p].carry_item-1;
                     itemf[c][0] = players[p].PX;
                     itemf[c][1] = players[p].PY;
                  }

                  item[li][10] = frame_num;
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

void proc_bonus_collision(int p, int x)
{
   if (item[x][7])
   {
      al_fixed f100 = al_itofix(100);
      if (players[p].LIFE < f100)
      {
         item[x][0] = 0;
         players[p].LIFE += al_itofix(item[x][7]);
         if (players[p].LIFE > f100) players[p].LIFE = f100;
         game_event(6, al_fixtoi(itemf[x][0]), al_fixtoi(itemf[x][1]), item[x][7],0, 0, 0);
      }
      else game_event(26, al_fixtoi(itemf[x][0]), al_fixtoi(itemf[x][1]), 0, 0, 0, 0); // already have 100 health
   }
}

void proc_exit_collision(int p, int x)
{
   int exit_enemys_left = num_enemy - item[x][8];
   if (exit_enemys_left <= 0)
   {
      level_done_trig = 1;
      next_level = play_level + 1;
      game_event(4, al_fixtoi(itemf[x][0]), al_fixtoi(itemf[x][1]), 0, 0, 0, 0);
   }
   else game_event(3, al_fixtoi(itemf[x][0]), al_fixtoi(itemf[x][1]), exit_enemys_left, 0, 0, 0); // not enough dead yet
}

void proc_key_collision(int p, int x)
{
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
   game_event(2, al_fixtoi(itemf[x][0]), al_fixtoi(itemf[x][1]), p, item[x][1], 0, 0); // send player and item shape
}

void proc_freeman_collision(int p, int x)
{
   item[x][0] = 0;
   players[p].LIVES++;
   game_event(9, al_fixtoi(itemf[x][0]), al_fixtoi(itemf[x][1]), 0, 0, 0, 0);
}

void proc_mine_collision(int p, int x)
{
   players[p].LIFE -= al_itofix(item[x][8]) / 10;
   game_event(10, al_fixtoi(itemf[x][0]), al_fixtoi(itemf[x][1]), 0, 0, 0, 0);
   game_event(7, al_fixtoi(itemf[x][0]), al_fixtoi(itemf[x][1]), item[x][8], 0, 0, 0);
}

void proc_bomb_collision(int p, int x)
{
   item[x][0] = 99; // change to lit bomb
   item[x][6] = 1;  // mode == lit
   item[x][8] = item[x][9]; // fuse wait count
   item[x][10] = 100; // default scale = 1.00
}

void proc_rocket_collision(int p, int x)
{
   item[x][0] = 98;   // new type - lit rocket
   item[x][1] = 1026; // new ans
   if ((item[x][3] == 0) || (item[x][3] == 1)) item[x][3] = -1;  // if stat or fall set to carryable
   itemf[x][3] = 0;   // stop if falling
   game_event(25, al_fixtoi(itemf[x][0]), al_fixtoi(itemf[x][1]), 0, 0, 0, 0);
}

void proc_warp_collision(int p, int x)
{
   next_level = item[x][8];
   level_done_trig = 1;
   game_event(4, al_fixtoi(itemf[x][0]), al_fixtoi(itemf[x][1]), 0, 0, 0, 0);
}

void proc_switch_collision(int p, int x)
{
   if (item[x][7] < frame_num) // if not lockout
   {
      // if falling and landing on it
      if ( (players[p].PX  > itemf[x][0] - al_itofix(12)) &&
           (players[p].PX  < itemf[x][0] + al_itofix(12)) &&
           (players[p].PY  > itemf[x][1] - al_itofix(16)) &&
           (players[p].PY  < itemf[x][1] - al_itofix(8)) &&
           (players[p].yinc > al_itofix(0)) )  // falling
      {
         item[x][7] = frame_num + 4; // switch lockout for next 4 frames
         item[x][6] = !item[x][6];
         if (item[x][6]) item[x][1] = item[x][8]; // on bmp
         else            item[x][1] = item[x][9]; // off bmp
         al_set_target_bitmap(level_background);
         // toggle blocks
         for (int c=0; c<100; c++)
            for (int y=0; y<100; y++)
            {
               if (l[c][y] == item[x][11]) // empty switch block
               {
                  l[c][y] = item[x][10]; // replace with solid switch block
                  al_draw_filled_rectangle(c*20, y*20, c*20+19, y*20+19, palette_color[0]);
                  al_draw_bitmap(tile[l[c][y]], c*20, y*20, 0 );
               }
               else if (l[c][y] == item[x][10]) // solid switch block
               {
                  l[c][y] = item[x][11]; // replace with empty switch block
                  al_draw_filled_rectangle(c*20, y*20, c*20+19, y*20+19, palette_color[0]);
                  al_draw_bitmap(tile[l[c][y]], c*20, y*20, 0 );
               }

            } // end of toggle blocks
         draw_lift_lines();
         game_event(30, al_fixtoi(itemf[x][0]), al_fixtoi(itemf[x][1]), 0, 0, 0, 0);
      }  // end of falling and landing on
   } // end of if not lockout
}

void proc_sproingy_collision(int p, int x)
{
   if ( (players[p].PX  > itemf[x][0] - al_itofix(10)) &&
        (players[p].PX  < itemf[x][0] + al_itofix(10)) &&
        (players[p].PY  > itemf[x][1] - al_itofix(16)) &&
        (players[p].PY  < itemf[x][1] - al_itofix(8)) &&
        (players[p].yinc > al_itofix(0)) && // falling
        (players[p].jump) )   //  jump pressed
   {
      game_event(31, al_fixtoi(itemf[x][0]), al_fixtoi(itemf[x][1]), 0, 0, 0, 0);
      players[p].yinc = al_itofix(0) - al_fixdiv(al_itofix(item[x][7]), al_ftofix(7.1));
   }
}



void proc_item_collision(int p, int x)
{
   // check if player can carry item
   if ( (!players[p].carry_item)  // not carrying
     && (item[x][3] < 0)          // item is carryable
     && (players[p].fire) )       // fire pressed
   {
      // check to see if another player is already carrying this item
      int other_player_carrying = 0;
      for (int op=0; op<NUM_PLAYERS; op++)
         if ((players[op].active) && (!players[op].paused))
            if (players[op].carry_item == x+1)
               other_player_carrying = 1;

       // allow carry if no other player is carrying
       if (other_player_carrying == 0)
       {
          players[p].carry_item = x+1;
          //printf("player picked up item:%d\n", x);
       }
       // allow mutiple player carry for rocket
       if (item[x][0] == 98) players[p].carry_item = x+1;
   }

   switch (item[x][0]) // item type
   {
      case 1:  proc_door_collision(p, x);     break;
      case 2:  proc_bonus_collision(p, x);    break;
      case 3:  proc_exit_collision(p, x);     break;
      case 4:  proc_key_collision(p, x);      break;
      case 6:  proc_freeman_collision(p, x);  break;
      case 7:  proc_mine_collision(p, x);     break;
      case 8:  proc_bomb_collision(p, x);     break;
      case 10: item[x][6] = item[x][7];       break; // set pop-up message timer
      case 11: proc_rocket_collision(p, x);   break;
      case 12: proc_warp_collision(p, x);     break;
      case 14: proc_switch_collision(p, x);   break;
      case 15: proc_sproingy_collision(p, x); break;
   }
}

void proc_lit_bomb(int c)
{
   // timer is item[c][8]
   // it has 2 modes set by item[c][6]
   // item[c][6] == 1 // fuse burning
   // item[c][6] == 2 // explosion

   // in both modes every frame it is decremented by 1

   // in mode 1 fuse
   // when timer gets to 0
   // mode is set to 2 and timer reset to 20

   // in mode 2 explosion
   // sound is played at 16
   // damage is done every frame

   lit_item = 1;
   item[c][8]--; // timer dec
   if (item[c][6] == 1) // fuse burning
   {
      if (item[c][8] < 1) // fuse done
      {
         item[c][6] = 2; // mode 2; explosion
         item[c][8] = item[c][9] = 20; // explosion timer

         // force player to drop item
         for (int p=0; p<NUM_PLAYERS; p++)
            if ((players[p].active) && (players[p].carry_item-1 == c)) player_drop_item(p);

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
      bomb_blocks(c, 2);
      bomb_enemies(c, 2);
      bomb_players(c, 2);

      if (item[c][8] == 16) game_event(22,0,0,0,0,0,0); // explosion sound

      if (item[c][8] <   1)
      {
         item[c][0] = 0; // explosion timer done, erase item
         draw_lift_lines();
      }
   }
}

void proc_lit_rocket(int c)
{
   int max_speed = item[c][8]*1000;
   int accel = item[c][9];
   lit_item = 1;

   if (item[c][11] < max_speed) item[c][11]+=accel;   // speed and accel

   al_fixed angle = al_itofix((item[c][10]-640) / 10);
   itemf[c][2] = (al_fixcos(angle) * item[c][11]) / 1000;       // hypotenuse is the speed!
   itemf[c][3] = (al_fixsin(angle) * item[c][11]) / 1000;

   int x = al_fixtoi(itemf[c][0] += itemf[c][2]); // do the increments
   int y = al_fixtoi(itemf[c][1] += itemf[c][3]);

   // check for wall collisions
   if ( ((itemf[c][3]<al_itofix(0)) && (is_up_solid(     x, y, 0) == 1)) ||
        ((itemf[c][3]>al_itofix(0)) && (is_down_solid(   x, y, 0) == 1)) ||
        ((itemf[c][3]>al_itofix(0)) && (is_down_solid(   x, y, 0) == 2)) ||
        ((itemf[c][2]<al_ftofix(-1.1)) && (is_left_solid(x, y, 0) == 1)) ||
        ((itemf[c][2]>al_ftofix(1.1)) && (is_right_solid(x, y, 0) == 1)) )
   {
      item[c][0] = 99;   // change to lit bomb
      item[c][2] = 3;    // draw mode
      item[c][6] = 2;    // mode 2; explosion
      item[c][8] = 20;   // explosion timer count
      item[c][9] = 20;   // explosion timer limit
      item[c][10] = 100; // start_size
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



























