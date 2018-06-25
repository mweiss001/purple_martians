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


void draw_pop_message(int i)
{
   al_set_target_bitmap(level_buffer);
   // process the text string into lines and rows and put in temp array
   char dt[40][120];
   int row = 0, col = 0, num_lines = 0;;
   int longest_line_len = 1;
   for (int a=0; a < (int)strlen(pmsg[i]) + 1; a++)
   {
       if (pmsg[i][a] == 126) // line break
      {
         dt[row][col] = 0; // in case len == 0  on first line
         row++;
         col=0;
         dt[row][col] = 0; // in case len == 0 on next line
      }
      else  // regular char
      {
         dt[row][col] = pmsg[i][a];
         if (col > longest_line_len) longest_line_len = col;
         col++;
         dt[row][col] = 0;
      }
   }
   num_lines = row;

   // x positions
   int pxw = (longest_line_len+2)*8;  // width is set from longest text line
   int px1 = item[i][10]*20;          // item sets left edge with specified corner block
   int px2 = px1 + pxw + 8;           // right edge depends on text width
   int pxc = px1 + pxw / 2;           // text center position

   // y positions
   int py1 = item[i][11]*20;          // item sets top edge with specified corner block
   int py2 = py1 + (num_lines+3) * 8; // bottom edge is set from number of lines of text

   int fc = item[i][9];               // frame color
   for (int a=0; a<12; a++)           // frame
      al_draw_filled_rounded_rectangle(px1+a, py1+a, px2-a, py2-a, 4, 4, palette_color[fc+a*16]);

   int tc = item[i][8];               // text color
   for (row=0; row<=num_lines; row++) // text
      al_draw_text(font, palette_color[tc], pxc+4, py1+row*8+9, ALLEGRO_ALIGN_CENTRE, dt[row]);
}

void draw_door(int i, int x, int y)
{
   ALLEGRO_BITMAP *tmp = NULL;
   int col = item[i][6];
   if (item[i][13] == 448) // old style door shape
   {
      int shape = item[i][1];       // get shape
      int si = shape-448;           // convert to index to bitmap sequence
      tmp = door_tile[1][col][si];
   }
   else // new style doors
   {
      int an = zz[1][83];             // cheat and use shape index from base door animation sequence
      if (item[i][8] == 0) an = 7-an; // exit only, run the sequence backwards
      tmp = door_tile[0][col][an];
   }

   int drawn = 0;
   if (item[i][8] == 1)  // linked item destination
   {
      // linked destination item position
      int dx = al_fixtoi(itemf[item[i][9]][0]);
      int dy = al_fixtoi(itemf[item[i][9]][1]);
      int line_color = item[item[i][9]][6];

      if (item[i][12] == 1) // if always draw lines (1)
         al_draw_line(x+10, y+10, dx+10, dy+10, palette_color[line_color], 1);  // draw a line connecting them

      for (int p=0; p<NUM_PLAYERS; p++) // is player touching door
         if ((players[p].active) && (players[p].marked_door == i))
         {
            if (item[i][12] > 0)  // always draw or only draw when touching ( 1 or 2)
               al_draw_line(x+10, y+10, dx+10, dy+10, palette_color[line_color], 1);  // draw a line connecting them

            // bigger door when player touching it
            al_draw_scaled_bitmap(tmp, 0, 0, 20, 20, x-5, y-6, 30, 26, 0 );

            if (item[i][8] == 0) al_draw_scaled_bitmap(tile[1015], 0, 0, 20, 20, x-5, y-6, 30, 26, 0); // OUT
            else al_draw_scaled_bitmap(tile[1014], 0, 0, 20, 20, x-5, y-6, 30, 26, 0); // IN

            if (item[i][11] == 1) // enter with <up>
               al_draw_text(font, palette_color[15],  x+3, y-14, 0, "up");

            if (item[i][11] == 2) // enter with <down>
               al_draw_text(font, palette_color[15],  x-5, y-14, 0, "down");
            drawn = 1;
         }
   }
   if (!drawn)
   {
      al_draw_bitmap(tmp, x, y, 0); // if not drawn yet

      if (item[i][8] == 0) al_draw_bitmap(tile[1015], x, y, 0); // OUT
      else al_draw_bitmap(tile[1014], x, y, 0); // IN
   }
}




int seq_color(int mod, int c1, int c2)
{
   int col = c1; // initial color
   if ( (frame_num % mod) < mod/2) col = c2; // other color
   return col;
}

int seq_color2(void)
{
   int ca[40];
   int ci = 0;

   for (ci = 0; ci<40; ci++) ca[ci] = 0;

   ci = 0;
   int b = 10;
   ca[ci] = b+192; ci++;
   ca[ci] = b+160; ci++;
   ca[ci] = b+128; ci++;
   ca[ci] = b+96; ci++;
   ca[ci] = b+64; ci++;
   ca[ci] = b+32; ci++;
   ca[ci] = b; ci++;
   ca[ci] = b; ci++;
   ca[ci] = b; ci++;
   ca[ci] = b; ci++;
   ca[ci] = b+32; ci++;
   ca[ci] = b+64; ci++;
   ca[ci] = b+96; ci++;
   ca[ci] = b+128; ci++;
   ca[ci] = b+160; ci++;
   ca[ci] = b+192; ci++;


   ci = 20;
   b = 14;
   ca[ci] = b+192; ci++;
   ca[ci] = b+160; ci++;
   ca[ci] = b+128; ci++;
   ca[ci] = b+96; ci++;
   ca[ci] = b+64; ci++;
   ca[ci] = b+32; ci++;
   ca[ci] = b; ci++;
   ca[ci] = b; ci++;
   ca[ci] = b; ci++;
   ca[ci] = b; ci++;
   ca[ci] = b+32; ci++;
   ca[ci] = b+64; ci++;
   ca[ci] = b+96; ci++;
   ca[ci] = b+128; ci++;
   ca[ci] = b+160; ci++;
   ca[ci] = b+192; ci++;




   int mod = frame_num % 40;
   return ca[mod];
}



int seq_color3(void)
{
   int ca[20];
   int ci = 0;

   for (ci = 0; ci<20; ci++) ca[ci] = 0;

   ci = 0;
   int b = 10;
   ca[ci] = b+192; ci++;
   ca[ci] = b+160; ci++;
   ca[ci] = b+128; ci++;
   ca[ci] = b+96; ci++;
   ca[ci] = b+64; ci++;
   ca[ci] = b+32; ci++;
   ca[ci] = b; ci++;
   ca[ci] = b; ci++;
   ca[ci] = b; ci++;
   ca[ci] = b; ci++;
   ca[ci] = b+32; ci++;
   ca[ci] = b+64; ci++;
   ca[ci] = b+96; ci++;
   ca[ci] = b+128; ci++;
   ca[ci] = b+160; ci++;
   ca[ci] = b+192; ci++;

   int mod = frame_num % 20;
   return ca[mod];
}






















void bomb_block_crosshairs(int e, int f)
{
   //int col = seq_color(16, 14, 10);
   int col = seq_color2();
   if (col)
   {
      al_draw_rectangle(e*20, f*20, e*20+20, f*20+20, palette_color[col], 1);
      al_draw_line(e*20, f*20, e*20+19, f*20+19, palette_color[col], 1);
      al_draw_line(e*20, f*20+19, e*20+19, f*20, palette_color[col], 1);
   }
}



void bomb_blocks(int i, int t, int dr)
{
   // center of bomb
   int x = al_fixtoi(itemf[i][0]) + 10;
   int y = al_fixtoi(itemf[i][1]) + 10;


   // convert to 0-100 range
   x /= 20;
   y /= 20;
   dr /= 20;        // damage range
   int dr2 = dr*dr; // damage range squared

   for (int e = (x-dr); e < (x+dr)+1; e++)        // cycle blast range of blocks
      for (int f = (y-dr); f < (y+dr)+1; f++)
      {
         // check radius from center
         int xd = abs(x-e);
         int yd = abs(y-f);
         int br = xd*xd+yd*yd;
         if ((br < dr2) && (l[e][f] > 63) && (l[e][f] < 96))
         {
            if (t == 1) bomb_block_crosshairs(e, f);
            if (t == 2) remove_block(e, f);
         }
      }
}


void bomb_crosshairs(float x, float y)
{
   int col = seq_color3();
   float rad = 12;   // circle radius


//   // radius seq
//   int ms = 8;  // min size
//   int ns = 16; // seq length
//   int sq = frame_num % ns;
//   if (sq < ns/2) rad = ms+sq;
//   else rad = ms+ns-sq;

   if (col)
   {
      // circle with a slash
      al_draw_circle(x, y, rad, palette_color[col], 1);
      float h = sqrt ((rad * rad) / 2) + 0;
      al_draw_line(x-h, y-h, x+h, y+h, palette_color[col], 1);
//      // circle with crosshairs
//      al_draw_circle(x, y, rad, palette_color[col], 1);
//      float h = sqrt ((rad * rad) / 2) + 6;
//      al_draw_line(x-h, y-h, x+h, y+h, palette_color[col], 1);
//      al_draw_line(x-h, y+h, x+h, y-h, palette_color[col], 1);
   }
}

void bomb_enemies(int i, int t, int dr)
{
   for (int e=0; e<100 ; e++) // enemies in damage window?
      if (Ei[e][0])
      {
         al_fixed dist = al_fixhypot( (Efi[e][0] - itemf[i][0]), (Efi[e][1] - itemf[i][1]) );
         if (dist < al_itofix(dr))
         {
            if (t == 1) bomb_crosshairs(10 + al_fixtof(Efi[e][0]), 10 + al_fixtof(Efi[e][1]));
            if (t == 2)
            {
               Ei[e][31] = 2; // set bomb hit
               Ei[e][26] = item[i][13];  // player that did bomb
            }
         }
      }
}





void bomb_players(int i, int t, int dr)
{
   for (int p=0; p<NUM_PLAYERS; p++)
   {
      if ((players[p].active) && (!players[p].paused))
      {
         al_fixed dist = al_fixhypot( (players[p].PX - itemf[i][0]), (players[p].PY - itemf[i][1]) );
         if (dist < al_itofix(dr))
         {
            // calculate bomb damage based on  blast size and player's distance
            al_fixed damage = al_fixdiv(al_itofix(dr), dist); // damage window size / distance (scale of 1 - 4)
            damage *= 20; // multiply by 20 (scale of 20-80)
            if (damage > al_itofix(80)) damage = al_itofix(80);
            int dmg = al_fixtoi(damage);

            if (t == 1) // add potential bomb damage for display
            {
               players1[p].potential_bomb_damage += dmg;
               players1[p].health_display = 40;
               players1[p].last_health_adjust = 0;
               bomb_crosshairs(10 + al_fixtof(players[p].PX), 10 + al_fixtof(players[p].PY));
            }
            if ((t == 2) && (item[i][8] == 0)) // only do damage once at end of explosion seq
            {
               players[p].LIFE -= damage;
               int p2 = item[i][13]; // player that last touched bomb
               if (p == p2) game_event(53, 0, 0, p, 0, 0, dmg);
               else game_event(52, 0, 0, p, p2, 0, dmg);
            }
         }
      }
   }
}


void proc_lit_bomb(int i)
{
   item[i][8]--; // timer dec

   if (item[i][6] == 3) // remote detonator
   {
      if (players[item[i][13]].down)
      {
         item[i][6] = 2; // mode 2; explosion
         item[i][8] = item[i][9] = 20; // explosion timer
      }
   }

   if (item[i][6] == 1) // fuse burning
   {
      lit_item = 1;
      if (item[i][8] < 1) // fuse done
      {
         item[i][6] = 2; // mode 2; explosion
         item[i][8] = item[i][9] = 20; // explosion timer

         // force player to drop item
         for (int p=0; p<NUM_PLAYERS; p++)
            if ((players[p].active) && (players[p].carry_item-1 == i)) player_drop_item(p);

         // check for other co-located bombs
         for (int ii=0; ii<500; ii++)
            if (item[ii][0] == 99) // lit bomb
               if (ii != i) // not this one
                  if (itemf[ii][0] == itemf[i][0]) // compare x
                     if (itemf[ii][1] == itemf[i][1]) // compare y
                        if (item[ii][15] == item[i][15]) // check if created by same cloner
                           item[ii][0] = 0; // delete item
      }
   }
   if (item[i][6] == 2) // explosion
   {
      float r = 1 - (float)item[i][8] / (float)item[i][9]; // ratio
      int dr = (int) (  (float)item[i][7] * r); // damage range scaled by ratio

      // do damage
      bomb_blocks(i, 2, dr);
      bomb_enemies(i, 2, dr);
      bomb_players(i, 2, dr);

      if (item[i][8] == 16) game_event(22,0,0,0,0,0,0); // explosion sound

      if (item[i][8] < 1)
      {
         item[i][0] = 0; // explosion done, erase item
         draw_lift_lines();
      }
   }
}
void draw_lit_bomb(int i)
{
   int x = al_fixtoi(itemf[i][0]);
   int y = al_fixtoi(itemf[i][1]);
   int fuse_seq = 96;
   int expl_seq = 97;
   float r = 1 - (float)item[i][8] / (float)item[i][9]; // countdown timer ratio (0 - 1)

   if (item[i][6] == 3) // remote detonator
   {
      // draw bomb
      al_draw_bitmap(tile[538], x, y, 0);

      // get detonator position relative to player
      int p = item[i][13];
      int xo = players[p].left_right*28-14;
      int px = al_fixtoi(players[p].PX) + xo;
      int py = al_fixtoi(players[p].PY) + -1;

      // draw detonator
      al_draw_bitmap(tile[539], px, py, 0);

      // draw a line connecting detonator and bomb
      al_draw_line(x+10, y+10, px+10, py+8, palette_color[207], 0);

      // show damage range circle
      int col = seq_color2();
      if (col) al_draw_circle(x+10, y+10, item[i][7], palette_color[col], 2);
   }


   if (item[i][6] == 1) // fuse burning
   {
      int num_seq_shapes = zz[4][fuse_seq] + 1;   // number of shapes in seq
      int si = (int)(r * (float)num_seq_shapes);  // ratio * number of shapes
      int shape = zz[5+si][fuse_seq];             // get shape to draw
      //printf("ratio:%f shape_index:%d\n", r, si);
      al_draw_bitmap(tile[shape], x, y, 0);

      // show countdown clock
      float cr = 6; // clock radius
      float pi = ALLEGRO_PI;
      float rd = pi*2*r - pi/2; // countdown position in radians
      al_draw_circle(x+10, y+13, cr, palette_color[15], 0);
      al_draw_pieslice(x+10, y+13, cr, rd, 0, palette_color[15], 0);
      al_draw_arc(x+10, y+13, cr, -pi/2, pi*2*r, palette_color[10], 0);

      // show damage range circle
      int col = seq_color2();
      if (col) al_draw_circle(x+10, y+10, item[i][7], palette_color[col], 2);

   }

   if (item[i][6] == 2) // explosion mode
   {
      int num_seq_shapes = zz[4][expl_seq] + 1;   // number of shapes in seq
      int si = (int)(r * (float)num_seq_shapes);  // ratio * number of shapes
      int shape = zz[5+si][expl_seq];             // get shape to draw

      // set size of explosion
      float max_scale = item[i][7] / 10;         // max scale = damage / 10
      float sc = .5 + (r) * max_scale;
      //printf("ratio:%f shape_index:%d scale:%f \n", r, si, sc);
      al_draw_scaled_rotated_bitmap(tile[shape], 10, 10, x+10, y+10, sc, sc, 0, 0);

      // show damage range circle
      al_draw_circle(x+10, y+10, (float)item[i][7]*r-8, palette_color[10], 1);
      al_draw_circle(x+10, y+10, (float)item[i][7]*r, palette_color[14], 2);
      al_draw_circle(x+10, y+10, (float)item[i][7]*r+8, palette_color[10], 1);
   }

   bomb_blocks(i, 1, item[i][7]);  // mark blocks that will be destroyed
   bomb_enemies(i, 1, item[i][7]); // mark enemies in damage range
   bomb_players(i, 1, item[i][7]); // mark players in damage range

   // debug show sequence numbers
   //al_draw_textf(font, palette_color[15], x, y-20, 0, "%d / %d  %f ", item[i][8], item[i][9], (float)item[i][8]/(float)item[i][9]);
}

void draw_items(void)
{
   al_set_target_bitmap(level_buffer);
   for (int i=0; i<500; i++)
      if (item[i][0])
      {
         int x = al_fixtoi(itemf[i][0]);
         int y = al_fixtoi(itemf[i][1]);
         int shape = item[i][1];                       // get shape
         if (shape > 999) shape = zz[0][shape-1000];   // ans
         int drawn = 0;

         if ((item[i][0] == 10) && (item[i][6] > 0)) // pop up message
         {
            item[i][6]--;
            draw_pop_message(i);
         }

         if (item[i][0] == 1)
         {
             draw_door(i, x, y);
             drawn = 1;
         }

         if (item[i][0] == 99)
         {
            draw_lit_bomb(i);
            if (item[i][11]) al_draw_bitmap(tile[440], x, y, 0);  // bomb sticky spikes
            drawn = 1;
         }
         if ((item[i][0] == 8) && (item[i][11])) al_draw_bitmap(tile[440], x, y, 0); // bomb sticky spikes

         // moving key in final sequence
         if ((item[i][0] == 4) && (item[i][11] > 0) && (item[i][11] < 10))
         {
            // moving key in final stage gets static shape not ans
            shape = item[i][1];                           // get shape
            if (shape > 999) shape = zz[5][shape-1000];   // get first shape only

            // stretch the key
            float sc = 1 + 4*((10 - (float)item[i][11]) / 10);
            float rot = al_fixtof(al_fixmul(al_itofix(item[i][10]/10), al_fixtorad_r));
            al_draw_scaled_rotated_bitmap(tile[shape],10, 10, x+10, y+10, sc, sc, rot, 0);
            drawn = 1;

            // draw a collapsing rectangle
            int x1 = item[i][6] * 20;
            int y1 = item[i][7] * 20;
            int x2 = (item[i][8]+1) * 20;
            int y2 = (item[i][9]+1) * 20;
            int xw = x2-x1;
            int yh = y2-y1;
            float xinc = xw/8;
            float yinc = yh/8;

            float seq = 9 - item[i][11]; // starts at 0, goes to 8
            int xo = (int)(seq * xinc / 2);
            int yo = (int)(seq * yinc / 2);
            al_draw_rectangle(x1+xo, y1+yo, x2-xo, y2-yo, palette_color[15], 1);
         }


          // these types need rotation
         if ((item[i][0] == 11) || (item[i][0] == 98) ||  // rockets
            ((item[i][0] == 4) && (item[i][11] > 0))) // moving key
         {
            float rot = al_fixtof(al_fixmul(al_itofix(item[i][10]/10), al_fixtorad_r));
            al_draw_rotated_bitmap(tile[shape], 10, 10, x+10, y+10, rot, 0);
            drawn = 1;
         }

         if (item[i][0] == 3) // exit
         {
            al_draw_bitmap(tile[399], x, y, 0); // 'exit' text not shown
            if (frame_num % 60 > 30)
               al_draw_text(f3, palette_color[10], x+11, y-2, ALLEGRO_ALIGN_CENTER, "EXIT");

            int exit_enemys_left = num_enemy - item[i][8];
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
      } // end of active item iterate
}



int is_item_stuck_to_wall(int i)
{
   int x = al_fixtoi(itemf[i][0]);
   int y = al_fixtoi(itemf[i][1]);
   if ( (is_left_solid(x,y, 0)) || (is_right_solid(x,y, 0)) || (is_down_solid(x,y, 0)) || (is_up_solid(x,y, 0)) ) return 1;
   return 0;
}



void move_items()
{
   for (int i=0; i<500; i++)
      if (item[i][0])
      {
         // check for time to live
         int ttl = item[i][14];
         if (ttl)
         {
            if (ttl < 11)       // start ans seq
            {
               item[i][0] = 66; // change to different type to prevent use
               int sq = 10-ttl;
               item[i][1] = zz[5+sq][74];
            }
            if (ttl == 1) item[i][0] = 0; // kill instantly
            item[i][14]--;
         }

         if (item[i][0] == 99) proc_lit_bomb(i);
         if (item[i][0] == 98) proc_lit_rocket(i);

         if ((item[i][0] == 4) && (item[i][11] > 0)) // moving key
         {
            // do the incs until the last 10 frames, which are for final sequence
            if (item[i][11] > 10)
            {
               itemf[i][0] += itemf[i][2];  // xinc
               itemf[i][1] += itemf[i][3];  // yinc
            }
            item[i][11]--;
            if (item[i][11] == 0)
            {
               // remove the key
               item[i][0] = 0;
               if (item[i][12]) // matching keyed blocks only
               {
                  int key = item[i][1] - 1039;
                  for (int x = item[i][6]; x <= item[i][8]; x++)
                     for (int y = item[i][7]; y <= item[i][9]; y++)
                        if ((l[x][y] == 188 + key) || (l[x][y] == 204 + key) || (l[x][y] == 220 + key))
                           remove_block(x, y);
               }
               else // remove all blocks in range
               {
                  for (int x = item[i][6]; x <= item[i][8]; x++)
                     for (int y = item[i][7]; y <= item[i][9]; y++)
                        remove_block(x, y);
               }
               draw_lift_lines(); // in case removing the key blocks erases lift lines
             }
         }  // end of moving key
         else if ((item[i][3]) && (item[i][0] != 98)) // and not stationary and not lit rocket
         {
            int pc = 0;
            for (int p=0; p<NUM_PLAYERS; p++)
               if (players[p].active)
                  if ((!players[p].paused) || (players[p].paused && players[p].paused_type == 2))
                     if (i == (players[p].carry_item-1)) pc = 1;



            if (!pc) // not being carried
            {
               int sticky = 0;
               if ((item[i][0] == 99) && (item[i][11])) sticky = 1;
               if ((sticky) && (is_item_stuck_to_wall(i)) )
               {
                  itemf[i][2] = al_itofix(0);  // xinc
                  itemf[i][3] = al_itofix(0);  // yinc
               }
               else
               {

                  // apply incs
                  itemf[i][0] += itemf[i][2];  // xinc
                  itemf[i][1] += itemf[i][3];  // yinc

                  // always slow down xinc (kinda like friction)
                  if (itemf[i][2] > al_itofix(0))
                  {
                      itemf[i][2] -= al_ftofix(.01); // slow down + x move
                         if (itemf[i][2] < al_itofix(0)) // set to zero if crosses zero
                            itemf[i][2] = al_itofix(0);
                  }

                  if (itemf[i][2] < al_itofix(0))
                  {
                      itemf[i][2] += al_ftofix(.01); // slow down + x move
                         if (itemf[i][2] > al_itofix(0)) // set to zero if crosses zero
                            itemf[i][2] = al_itofix(0);
                  }

                  int x = al_fixtoi(itemf[i][0]);
                  int y = al_fixtoi(itemf[i][1]);

                  // moving right
                  if ((itemf[i][2] > al_itofix(0)) && (is_right_solid(x,y, 1)))
                  {
                     if (!sticky) itemf[i][0] -= itemf[i][2];  // take back xinc
                     itemf[i][2] = al_itofix(0);     // stop
                  }

                  // moving left
                  if ((itemf[i][2] < al_itofix(0)) && (is_left_solid(x,y, 1)))
                  {
                     if (!sticky) itemf[i][0] -= itemf[i][2];  // take back xinc
                     itemf[i][2] = al_itofix(0);     // stop
                  }

                  x = al_fixtoi(itemf[i][0]);
                  y = al_fixtoi(itemf[i][1]);

                  // moving up
                  if (itemf[i][3] < al_itofix(0))
                  {
                     if (is_up_solid(x, y, 0) == 1)    // only check for solid blocks
                        itemf[i][3] = al_itofix(0);        // if collision kill upwards yinc
                     else itemf[i][3] += al_ftofix(.1);    // else de-accel
                  }
                  else // not moving up
                  {
                     int a = is_down_solid(x, y, 1);             // check for block below
                     if (a==0)
                     {
                        itemf[i][3] += al_ftofix(.1);                             // apply gravity to yinc
                        if (itemf[i][3] > al_itofix(3)) itemf[i][3] = al_itofix(3);  // max gravity
                     }
                     if (a) // slow down xinc if block or lift below
                     {
                        if (itemf[i][2] > al_itofix(0)) itemf[i][2] -= al_ftofix(.12);
                        if (itemf[i][2] < al_itofix(0)) itemf[i][2] += al_ftofix(.12);
                     }

                     if ((a==1) || (a==2)) // align with ground if block below
                     {
                        itemf[i][1] = al_itofix((y/20)*20); // align with ground
                        itemf[i][3] = al_itofix(0);  // zero yinc
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

                           itemf[i][0] += lxi;                             // move x with lift's xinc
                           itemf[i][1]  = lifts[a-32].fy - al_itofix(20);  // align with lift's y

                           x = al_fixtoi(itemf[i][0]);
                           y = al_fixtoi(itemf[i][1]);

                           if (lyi > al_itofix(0)) // down
                              if (is_down_solid(x, y, 0))                      // no lift check
                                 itemf[i][1] = al_itofix(y - (y % 20));        // item not on lift anymore, align with block

                           if (lyi < al_itofix(0)) // up
                              if (is_up_solid(x, y, 0) == 1)
                                 itemf[i][1] += al_itofix(10);

                           if (lxi > al_itofix(0)) // right
                              if (is_right_solid(x, y, 1))
                                 itemf[i][0] -= lxi;

                           if (lxi < al_itofix(0)) // left
                              if (is_left_solid(x, y, 1))
                                 itemf[i][0] -= lxi;
                        }
                     } // end of riding lift
                  } // end of not moving up
               } // end of not stuck to wall
            } // end of not being carried
         } // end of if not stationary and not lit rocket
      } // end of iterate all active items
}


int player_drop_item(int p)
{
   int i = players[p].carry_item-1; // number of item
   // printf("drop item:%d\n", pc);

   int wall_stuck = 0;
   players[p].carry_item = 0;
   if (item[i][0] != 99) // not lit bomb
   {
      // check to see if the item is embedded in the wall
      int x = al_fixtoi(itemf[i][0] );
      int y = al_fixtoi(itemf[i][1] );

      if (players[p].left_right) // right
      {
         // is item embedded in a wall to the right?
         while (is_right_solid(x, y, 1))
         {
            x--;
            wall_stuck++; // just how stuck was it?
         }
         itemf[i][0] = al_itofix(x);
      }

      if (!players[p].left_right) // left
      {
         // is item embedded in a wall to the leftright?
         while (is_left_solid(x, y, 1))
         {
            x++;
            wall_stuck++; // just how stuck was it?
         }
         itemf[i][0] = al_itofix(x);
       }
   }
   else itemf[i][1] += al_itofix(2); // when putting bomb in wall, this stops it from snapping to block above
   return wall_stuck;
}

void proc_player_carry(int p)
{
   if ((players[p].active) && (players[p].carry_item))
      if (!players[p].paused || (players[p].paused && players[p].paused_type == 2))// player is carrying item
      {
         int i = players[p].carry_item-1; // number of item
         if ((item[i][0] == 98) || (item[i][0] == 99)) item[i][13] = p; // mark player carrying lit bomb or rocket
         if (item[i][0] != 98)            // not lit rocket
         {
            // set item position relative to player that's carrying it
            itemf[i][1] = players[p].PY - al_itofix(2);
            if (!players[p].left_right) itemf[i][0] = players[p].PX - al_itofix(15);
            if (players[p].left_right) itemf[i][0] = players[p].PX + al_itofix(15);
         }
         if (!players[p].fire) // drop
         {
            if (player_drop_item(p) < 6)
            {
               if (item[i][0] != 98)            // not lit rocket
               {
                  itemf[i][2] = players[p].xinc;  // inherit the players momentum
                  itemf[i][3] = players[p].yinc;
                  if (players[p].up)    itemf[i][3] -= al_itofix(6); // throw item upwards
                  if (players[p].left)  itemf[i][2] -= al_itofix(2); // throw item left
                  if (players[p].right) itemf[i][2] += al_itofix(2); // throw item right
               }
            }
         }
      }
}


void proc_door_collision(int p, int i)
{
   if ((players[p].marked_door == -1)  // player has no marked door yet
     && (players[p].carry_item != i+1)) // player is not carrying this door
   {
      players[p].marked_door = i;

      // item[x][6]  color
      // item[x][7]  move type (0 = auto, 1 = force instant, 2 = force move
      // item[x][8]  type (0 = no dest (exit only), 1 = linked dest
      // item[x][9]  linked destination item
      // item[x][10] key held flag
      // item[x][11] door entry type (0 0-immed, 1 = up 2 = down)
      // item[x][12] draw lines always, never
      // item[x][13] base animation shape


      if (item[i][8]) // do nothing if exit only
      {
         int do_entry = 0;
         if (item[i][11] == 0) do_entry = 1; // enter immed
         if (players[p].carry_item-1 != i) // cant trigger entry if carrying this door
         {
            if (item[i][11] == 1) // enter with <up>
            {
               // to prevent immediate triggering when destination door, wait for release and re-press

               // if key held is old, ignore
               if (item[i][10] && item[i][10] < frame_num-1) item[i][10] = 0;

               // up pressed and !pressed last frame
               if ((players[p].up) && (!item[i][10])) do_entry = 1;

               if (players[p].up) item[i][10] = frame_num;
               else item[i][10] = 0;

            }

            if (item[i][11] == 2) // enter with <down>
            {
               // to prevent immediate triggering when destination door, wait for release and re-press

               // if key held is old, ignore
               if (item[i][10] && item[i][10] < frame_num-1) item[i][10] = 0;

               // down pressed and !pressed last frame
               if ((players[p].down) && (!item[i][10])) do_entry = 1;

               if (players[p].down) item[i][10] = frame_num;
               else item[i][10] = 0;
            }
         }

         if (do_entry)
         {
            int bad_exit = 0;
            item[i][10] = 0; // clear the key hold for the door you just left

           // check if dest item is same as source item
            if (item[i][9] == i) bad_exit = 1;

            // is player carrying an item ?
            if (players[p].carry_item)
            {
                int ci = players[p].carry_item - 1;
                //printf("do entry, player is carrying item:%d\n", ci);

               // check to see if player is carrying this door
                if (ci == i) player_drop_item(p);

               // check to see if player is carrying an item without the carry through door flag set
               if (item[ci][3] != -2)  player_drop_item(p);
            }

            // get the destination
            al_fixed dx = al_itofix(0), dy = al_itofix(0);
            int li = item[i][9]; // linked item number

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
               game_event(5, 0, 0, p, i, 0, 0);

               int instant_move = 0;
               if (item[i][7] == 0) // 0 = auto
                  if (item[li][3]) // if dest is not stat
                     instant_move = 1;

               if (item[i][7] == 1) instant_move = 1; // 1 = force instant
               if (item[i][7] == 2) instant_move = 0; // 2 = force move

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
                  players[p].PX = itemf[i][0];
                  players[p].PY = itemf[i][1];

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
                     players[p].door_item = i;
                     players[p].door_draw_rot_inc = al_fixdiv(players[p].door_draw_rot, al_itofix(ddrns));
                  }
                  // printf("ns:%d xinc:%3.2f yinc:%3.2f \n", num_steps, al_fixtof(players[p].xinc), al_fixtof(players[p].yinc));
               } // end of if not instant move
            }  // end of if not bad exit
         } // end of do entry
      } // end of if not exit only
   } // end of if not first door touched
}

void proc_bonus_collision(int p, int i)
{
   if (item[i][7])
   {
      al_fixed f100 = al_itofix(100);
      if (players[p].LIFE < f100)
      {
         item[i][0] = 0;
         players[p].LIFE += al_itofix(item[i][7]);
         if (players[p].LIFE > f100) players[p].LIFE = f100;
      }
      else game_event(26, 0, 0, p, i, 0, 0); // already have 100 health
   }
}

void proc_exit_collision(int p, int i)
{
   int exit_enemys_left = num_enemy - item[i][8];
   if (exit_enemys_left <= 0)
   {
      level_done_trig = 1;
      next_level = play_level + 1;
      game_event(4, 0, 0, 0, 0, 0, 0);
   }
   else game_event(3, 0, 0, p, i, exit_enemys_left, 0); // not enough dead yet
}

void proc_key_collision(int p, int i)
{
   int x2 = (item[i][6] + item[i][8]) * 10;   // get the center of the block range
   int y2 = (item[i][7] + item[i][9]) * 10;
   al_fixed xlen = al_itofix(x2) - itemf[i][0];     // distance between block range and key
   al_fixed ylen = al_itofix(y2) - itemf[i][1];
   al_fixed hy_dist =  al_fixhypot(xlen, ylen);     // hypotenuse distance
   al_fixed speed = al_itofix(12);                  // speed
   al_fixed scaler = al_fixdiv(hy_dist, speed);     // get scaler
   al_fixed xinc = al_fixdiv(xlen, scaler);         // calc xinc
   al_fixed yinc = al_fixdiv(ylen, scaler);         // calc yinc
   itemf[i][2] = xinc;
   itemf[i][3] = yinc;
   al_fixed angle = al_fixatan2(ylen, xlen);        // get the angle for item rotation
   item[i][10] = al_fixtoi(angle) * 10;
   al_fixed ns;                                  // get the number of steps
   if (abs(xlen) > abs(ylen)) ns = al_fixdiv(xlen, xinc);
   else  ns = al_fixdiv(ylen, yinc);
   int num_steps = al_fixtoi(ns);
   item[i][11] = num_steps + 10;              // add 10 for final sequence
   game_event(2, 0, 0, p, i, 0, 0);
}

void proc_freeman_collision(int p, int i)
{
   item[i][0] = 0;
   players[p].LIVES++;
   game_event(70, 0, 0, p, i, 0, 0);
}

void proc_mine_collision(int p, int i)
{
   players[p].LIFE -= al_itofix(item[i][8]) / 10;
   game_event(50, 0, 0, p, i, 0, item[i][8]);
}

void proc_bomb_collision(int p, int i)
{
   item[i][0] = 99; // change to lit bomb
   item[i][13] = p; // mark player that lit bomb
   if (item[i][12] == 0) // fuse timer
   {
      item[i][6] = 1;  // mode == lit
      item[i][8] = item[i][9]; // fuse wait count
      game_event(24, 0, 0, p, item[i][7]/20, item[i][9]/40, 0);
   }
   if (item[i][12] == 1) // remote detonator
   {
      item[i][6] = 3;
      game_event(25, 0, 0, p, item[i][7]/20, 0, 0);
   }
}


void proc_rocket_collision(int p, int i)
{
   item[i][0] = 98;   // new type - lit rocket
   item[i][13] = p;   // mark player that lit rocket
   item[i][1] = 1026; // new ans
   if ((item[i][3] == 0) || (item[i][3] == 1)) item[i][3] = -1;  // if stat or fall set to carryable
   itemf[i][3] = 0;   // stop if falling
   game_event(26, 0, 0, p, i, 0, 0);
}

void proc_warp_collision(int p, int i)
{
   next_level = item[i][8];
   level_done_trig = 1;
   game_event(4, 0, 0, p, i, 0, 0);
}

void proc_switch_collision(int p, int i)
{
   if (item[i][7] < frame_num) // if not lockout
   {
      // if falling and landing on it
      if ( (players[p].PX  > itemf[i][0] - al_itofix(12)) &&
           (players[p].PX  < itemf[i][0] + al_itofix(12)) &&
           (players[p].PY  > itemf[i][1] - al_itofix(16)) &&
           (players[p].PY  < itemf[i][1] - al_itofix(8)) &&
           (players[p].yinc > al_itofix(0)) )  // falling
      {
         item[i][7] = frame_num + 4; // switch lockout for next 4 frames
         item[i][6] = !item[i][6];
         if (item[i][6]) item[i][1] = item[i][8]; // on bmp
         else            item[i][1] = item[i][9]; // off bmp
         al_set_target_bitmap(level_background);
         // toggle blocks
         for (int c=0; c<100; c++)
            for (int y=0; y<100; y++)
            {
               if (l[c][y] == item[i][11]) // empty switch block
               {
                  l[c][y] = item[i][10]; // replace with solid switch block
                  al_draw_filled_rectangle(c*20, y*20, c*20+19, y*20+19, palette_color[0]);
                  al_draw_bitmap(tile[l[c][y]], c*20, y*20, 0 );
               }
               else if (l[c][y] == item[i][10]) // solid switch block
               {
                  l[c][y] = item[i][11]; // replace with empty switch block
                  al_draw_filled_rectangle(c*20, y*20, c*20+19, y*20+19, palette_color[0]);
                  al_draw_bitmap(tile[l[c][y]], c*20, y*20, 0 );
               }

            } // end of toggle blocks
         draw_lift_lines();
         game_event(30, 0, 0, p, i, 0, 0);
      }  // end of falling and landing on
   } // end of if not lockout
}

void proc_sproingy_collision(int p, int i)
{
   if ( (players[p].PX  > itemf[i][0] - al_itofix(10)) &&
        (players[p].PX  < itemf[i][0] + al_itofix(10)) &&
        (players[p].PY  > itemf[i][1] - al_itofix(16)) &&
        (players[p].PY  < itemf[i][1] - al_itofix(8)) &&
        (players[p].yinc > al_itofix(0)) && // falling
        (players[p].jump) )   //  jump pressed
   {
      game_event(31, 0, 0, p, i, 0, 0);
      players[p].yinc = al_itofix(0) - al_fixdiv(al_itofix(item[i][7]), al_ftofix(7.1));
   }
}



void proc_item_collision(int p, int i)
{
   // check if player can carry item
   if ( (!players[p].carry_item)  // not carrying
     && (item[i][3] < 0)          // item is carryable
     && (players[p].fire) )       // fire pressed
   {
      // check to see if another player is already carrying this item
      int other_player_carrying = 0;
      for (int op=0; op<NUM_PLAYERS; op++)
         if ((players[op].active) && (!players[op].paused))
            if (players[op].carry_item == i+1)
               other_player_carrying = 1;

       // allow carry if no other player is carrying
       if (other_player_carrying == 0)
       {
          players[p].carry_item = i+1;
          //printf("player picked up item:%d\n", x);
       }
       // allow mutiple player carry for rocket
       if (item[i][0] == 98) players[p].carry_item = i+1;
   }

   switch (item[i][0]) // item type
   {
      case 1:  proc_door_collision(p, i);     break;
      case 2:  proc_bonus_collision(p, i);    break;
      case 3:  proc_exit_collision(p, i);     break;
      case 4:  proc_key_collision(p, i);      break;
      case 6:  proc_freeman_collision(p, i);  break;
      case 7:  proc_mine_collision(p, i);     break;
      case 8:  proc_bomb_collision(p, i);     break;
      case 10: item[i][6] = item[i][7];       break; // set pop-up message timer
      case 11: proc_rocket_collision(p, i);   break;
      case 12: proc_warp_collision(p, i);     break;
      case 14: proc_switch_collision(p, i);   break;
      case 15: proc_sproingy_collision(p, i); break;
   }
}

void proc_lit_rocket(int i)
{
   int max_speed = item[i][8]*1000;
   int accel = item[i][9];
   lit_item = 1;

   if (item[i][11] < max_speed) item[i][11]+=accel;   // speed and accel

   al_fixed angle = al_itofix((item[i][10]-640) / 10);
   itemf[i][2] = (al_fixcos(angle) * item[i][11]) / 1000;       // hypotenuse is the speed!
   itemf[i][3] = (al_fixsin(angle) * item[i][11]) / 1000;

   int x = al_fixtoi(itemf[i][0] += itemf[i][2]); // temp apply the increments
   int y = al_fixtoi(itemf[i][1] += itemf[i][3]);

   // check for wall collisions
   if ( ((itemf[i][3]<al_itofix(0)) && (is_up_solid(     x, y, 0) == 1)) ||
        ((itemf[i][3]>al_itofix(0)) && (is_down_solid(   x, y, 0) == 1)) ||
        ((itemf[i][3]>al_itofix(0)) && (is_down_solid(   x, y, 0) == 2)) ||
        ((itemf[i][2]<al_ftofix(-1.1)) && (is_left_solid(x, y, 0) == 1)) ||
        ((itemf[i][2]>al_ftofix(1.1)) && (is_right_solid(x, y, 0) == 1)) )
   {

      // stop movement
      itemf[i][2] = al_itofix(0);
      itemf[i][3] = al_itofix(0);

      item[i][0] = 99;   // change to lit bomb
      item[i][6] = 2;    // mode 2; explosion
      item[i][8] = 20;   // explosion timer count
      item[i][9] = 20;   // explosion timer limit
   }
   else
   {
      // if player riding, bind player to rocket
      for (int p=0; p<NUM_PLAYERS; p++)
         if ( (players[p].active) && (!players[p].paused) && (riding_rocket(p)) )
         {
            players[p].PX = itemf[i][0];
            players[p].PY = itemf[i][1];
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
item[][3] = (0=stat, 1=fall, -1=carry, -2=carry through door -3=sticky)
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

item[][11]  sticky
item[][12]  timer / remote




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
item[][6]  mode (1=lit, 2=explosion, 3=remote detonator)
item[][7]  blast size
item[][8]  fuse length counter
item[][9]  fuse length value

item[][11] sticky
item[][12] timer | remote
item[][13] last player to touch





*/



























