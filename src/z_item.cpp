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

   for (int c=1; c<20; c++)
      if ((c!= 1) && (c !=3) && (c!= 4) && (c != 5) && (c!= 7) && (c!= 8) && (c!= 9) && (c!= 11) && (c!= 12) && (c!= 14) && (c!= 15) && (c!= 16) && (c!= 17) )
         if (item_num_of_type[c]) // not zero
         {
                         sprintf(msg, "%d type %d???  ", item_num_of_type[c], c); // default unknown
            if (c ==  0) sprintf(msg, "%d type 0      ", item_num_of_type[c]);
            if (c ==  2) sprintf(msg, "%d Bonus       ", item_num_of_type[c]);
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
   al_draw_bitmap(btile[0], x*20, y*20, 0);
}


void draw_pop_message(int i)
{
   al_set_target_bitmap(level_buffer);
   // process the text string into lines and rows and put in temp array
   char dt[40][120];
   int row = 0, col = 0, num_lines = 0;;
   int longest_line_len = 1;

   for (int a=0; a < (int)strlen(pmsgtext[i]) + 1; a++)
   {
      if (pmsgtext[i][a] == 126) // line break
      {
         dt[row][col] = 0; // in case len == 0  on first line
         row++;
         col=0;
         dt[row][col] = 0; // in case len == 0 on next line
      }
      else  // regular char
      {
         dt[row][col] = pmsgtext[i][a];
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

void bomb_blocks(int i, int t, int dr, al_fixed fx, al_fixed fy)
{
   // center of bomb
//   float x = al_fixtoi(itemf[i][0]) + 10;
//   float y = al_fixtoi(itemf[i][1]) + 10;
   float x = al_fixtoi(fx) + 10;
   float y = al_fixtoi(fy) + 10;
   float d = (float)dr;

   // convert to 0-100 range to cycle block rect
   int bx = (int)(x / 20);
   int by = (int)(y / 20);
   int bd = (int)(dr / 20);  // damage range

   for (int e = (bx-bd); e < (bx+bd)+1; e++)        // cycle blast range of blocks
      for (int f = (by-bd); f < (by+bd)+1; f++)
      {
         // get center of block
         float cx = (float)(e*20+10);
         float cy = (float)(f*20+10);

         // check radius from center
         float xd = abs(x-cx);
         float yd = abs(y-cy);
         float br = sqrt(xd*xd+yd*yd);

         if ((br < d) && (l[e][f] & PM_BTILE_BOMBABLE))
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

void bomb_enemies(int i, int t, int dr, al_fixed x, al_fixed y)
{
   for (int e=0; e<100 ; e++) // enemies in damage window?
      if ((Ei[e][0]) && (Ei[e][0] != 10))
      {
//         al_fixed dist = al_fixhypot( (Efi[e][0] - itemf[i][0]), (Efi[e][1] - itemf[i][1]) );
         al_fixed dist = al_fixhypot( (Efi[e][0] - x), (Efi[e][1] - y));
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

void bomb_players(int i, int t, int dr, al_fixed x, al_fixed y)
{
   for (int p=0; p<NUM_PLAYERS; p++)
   {
      if ((players[p].active) && (!players[p].paused))
      {
         al_fixed dist = al_fixhypot( (players[p].PX - x), (players[p].PY - y) );
         if (dist < al_itofix(dr))
         {
            // calculate bomb damage based on blast size and player's distance
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
      bomb_blocks( i, 2, dr, itemf[i][0], itemf[i][1]);
      bomb_enemies(i, 2, dr, itemf[i][0], itemf[i][1]);
      bomb_players(i, 2, dr, itemf[i][0], itemf[i][1]);


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

   bomb_blocks( i, 1, item[i][7], itemf[i][0], itemf[i][1]); // mark blocks that will be destroyed
   bomb_enemies(i, 1, item[i][7], itemf[i][0], itemf[i][1]); // mark enemies
   bomb_players(i, 1, item[i][7], itemf[i][0], itemf[i][1]); // mark players in damage range

   // debug show sequence numbers
   //al_draw_textf(font, palette_color[15], x, y-20, 0, "%d / %d  %f ", item[i][8], item[i][9], (float)item[i][8]/(float)item[i][9]);
}

void draw_rocket_lines(int i)
{
   al_fixed fxi = itemf[i][0]; // initial position
   al_fixed fyi = itemf[i][1];

   al_fixed fx = fxi; // path variables
   al_fixed fy = fyi;


   int speed = item[i][11];
   if (speed < 2000) speed = 2000; // if moving too slow, lines won't be drawn

   // temp show speed
   //al_draw_textf(font, palette_color[10], al_fixtof(fxi)+30, al_fixtof(fyi)+10, ALLEGRO_ALIGN_CENTER, "%d" ,speed );


   al_fixed angle = al_itofix((item[i][10]-640) / 10);  // angle
   al_fixed fxinc = (al_fixcos(angle) * speed) / 1000;  // x and y increments
   al_fixed fyinc = (al_fixsin(angle) * speed) / 1000;

   for (int j=0; j<1000; j++)
   {
      int x = al_fixtoi(fx += fxinc); // apply the increments
      int y = al_fixtoi(fy += fyinc);

      if ((x < 0) || (x > 2000) || (y < 0) || (y > 2000)) j = 1000; // level bounds check

      // check for wall collisions
      if ( ((fyinc < al_itofix(0))    && (is_up_solid(   x, y, 0, 3) == 1)) ||
           ((fyinc > al_itofix(0))    && (is_down_solid( x, y, 0, 3) == 1)) ||
           ((fyinc > al_itofix(0))    && (is_down_solid( x, y, 0, 3) == 2)) ||
           ((fxinc < al_ftofix(-1.1)) && (is_left_solid( x, y, 0, 3) == 1)) ||
           ((fxinc > al_ftofix(1.1))  && (is_right_solid(x, y, 0, 3) == 1)) )
      {

         float fxf =  al_fixtof(fx)+10; // offset floats for display purposes
         float fyf =  al_fixtof(fy)+10;
         float fxif = al_fixtof(fxi)+10;
         float fyif = al_fixtof(fyi)+10;

         int col = seq_color2(); // color sequence
         if (col) al_draw_circle(fxf, fyf, item[i][7], palette_color[col], 2); // show damage range circle
         al_draw_filled_circle(fxf, fyf, 5, palette_color[col]); // show center
         al_draw_line(fxif, fyif, fxf, fyf, palette_color[10+80], 0); // draw connecting line

         bomb_blocks( i, 1, item[i][7], fx, fy); // mark blocks that will be destroyed
         bomb_enemies(i, 1, item[i][7], fx, fy); // mark enemies that will be destroyed
         bomb_players(i, 1, item[i][7], fx, fy); // mark players in damage range

         j = 1000; // to break out of loop
      }
   }
}





void draw_items(void)
{
   al_set_target_bitmap(level_buffer);
   for (int i=0; i<500; i++)
      if (item[i][0])
      {
         int type = item[i][0];
         int x = al_fixtoi(itemf[i][0]);
         int y = al_fixtoi(itemf[i][1]);
         int shape = item[i][1];                       // get shape
         if (shape > 999) shape = zz[0][shape-1000];   // ans
         int drawn = 0;

         if ((type == 10) && (item[i][6] > 0)) // pop up message
         {
            item[i][6]--;
            draw_pop_message(i);
         }

         if (type == 1)
         {
             draw_door(i, x, y);
             drawn = 1;
         }

         if (type == 9)
         {
             draw_trigger(i);
             drawn = 1;
         }

         if (type == 16)
         {
             draw_block_manip(i);
             drawn = 1;
         }

         if (type == 17)
         {
             draw_block_damage(i);
             drawn = 1;
         }


         if (type == 99)
         {
            draw_lit_bomb(i);
            if (item[i][11]) al_draw_bitmap(tile[440], x, y, 0);  // bomb sticky spikes
            drawn = 1;
         }
         if ((type == 8) && (item[i][11])) al_draw_bitmap(tile[440], x, y, 0); // bomb sticky spikes

         // moving key in final sequence
         if ((type == 4) && (item[i][11] > 0) && (item[i][11] < 10))
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
         if ((type == 11) || (type == 98) ||  // rockets
            ((type == 4) && (item[i][11] > 0))) // moving key
         {
            float rot = al_fixtof(al_fixmul(al_itofix(item[i][10]/10), al_fixtorad_r));
            al_draw_rotated_bitmap(tile[shape], 10, 10, x+10, y+10, rot, 0);
            drawn = 1;
         }

         if (type == 98) draw_rocket_lines(i); // for lit rockets




         if (type == 5) // start
         {
            if (number_of_starts > 1)
            {
               al_draw_bitmap(tile[shape], x, y, 0);
               al_draw_textf(f3, palette_color[12], x+10, y-4, ALLEGRO_ALIGN_CENTER, "%d", item[i][7]);
               drawn = 1;
            }
         }

         if (type == 3) // exit
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




         if ((type == 2) && (item[i][6] == 3)) // purple coin custom draw
         {
            if (!level_editor_running)
            {
               spin_shape(shape, x, y, 0, 0, 19, 19, 0.8, 0.5, 40);
               drawn = 1;
            }
         }

         // default draw if nothing else has drawn it up to now
         if (!drawn) al_draw_bitmap(tile[shape], x, y, 0);

         // if item is expiring show how many seconds left it has
         if ((item[i][14]>10) && (type != 9) && (type != 16) && (type != 17))
            al_draw_textf(f3, palette_color[15], x+10, y-10, ALLEGRO_ALIGN_CENTER, "%d", 1 + (item[i][14] - 10) / 40);


      } // end of active item iterate
}











int is_item_stuck_to_wall(int i)
{
   int x = al_fixtoi(itemf[i][0]);
   int y = al_fixtoi(itemf[i][1]);
   if ( (is_left_solid(x,y, 0, 3)) || (is_right_solid(x,y, 0, 3)) || (is_down_solid(x,y, 0, 3)) || (is_up_solid(x,y, 0, 3)) ) return 1;
   return 0;
}

void move_items()
{
   for (int i=0; i<500; i++)
      if (item[i][0])
      {
         if      (item[i][0] == 9) process_trigger(i);
         else if (item[i][0] == 16) process_block_manip(i);
         else if (item[i][0] == 17) process_block_damage(i);
         else
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
                     int x1 = item[i][6] / 20;
                     int y1 = item[i][7] / 20;
                     int x2 = (item[i][6] + item[i][8]) / 20;
                     int y2 = (item[i][7] + item[i][9]) / 20;



                     for (int x = x1; x < x2; x++)
                        for (int y = y1; y < y2; y++)
                           if (((l[x][y]&1023) == 188 + key) || ((l[x][y]&1023) == 204 + key) || ((l[x][y]&1023) == 220 + key))
                              remove_block(x, y);



                  }
                  else // remove all blocks in range
                  {
                     int x1 = item[i][6] / 20;
                     int y1 = item[i][7] / 20;
                     int x2 = (item[i][6] + item[i][8]) / 20;
                     int y2 = (item[i][7] + item[i][9]) / 20;
                     for (int x = x1; x < x2; x++)
                        for (int y = y1; y < y2; y++)
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
                     if ((itemf[i][2] > al_itofix(0)) && (is_right_solid(x,y, 1, 3)))
                     {
                        if (!sticky) itemf[i][0] -= itemf[i][2];  // take back xinc
                        itemf[i][2] = al_itofix(0);     // stop
                     }

                     // moving left
                     if ((itemf[i][2] < al_itofix(0)) && (is_left_solid(x,y, 1, 3)))
                     {
                        if (!sticky) itemf[i][0] -= itemf[i][2];  // take back xinc
                        itemf[i][2] = al_itofix(0);     // stop
                     }

                     x = al_fixtoi(itemf[i][0]);
                     y = al_fixtoi(itemf[i][1]);

                     // moving up
                     if (itemf[i][3] < al_itofix(0))
                     {
                        if (is_up_solid(x, y, 0, 3) == 1)    // only check for solid blocks
                           itemf[i][3] = al_itofix(0);        // if collision kill upwards yinc
                        else itemf[i][3] += al_ftofix(.1);    // else de-accel
                     }
                     else // not moving up
                     {
                        int a = is_down_solid(x, y, 1, 3);             // check for block below
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
                              if (is_down_solid(x, y, 0, 3)) capture = 0; // to prevent lift attempting to take item down through solid block
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
                                 if (is_down_solid(x, y, 0, 3))                      // no lift check
                                    itemf[i][1] = al_itofix(y - (y % 20));        // item not on lift anymore, align with block

                              if (lyi < al_itofix(0)) // up
                                 if (is_up_solid(x, y, 0, 3) == 1)
                                    itemf[i][1] += al_itofix(10);

                              if (lxi > al_itofix(0)) // right
                                 if (is_right_solid(x, y, 1, 3))
                                    itemf[i][0] -= lxi;

                              if (lxi < al_itofix(0)) // left
                                 if (is_left_solid(x, y, 1, 3))
                                    itemf[i][0] -= lxi;
                           }
                        } // end of riding lift
                     } // end of not moving up
                  } // end of not stuck to wall
               } // end of not being carried
            } // end of if not stationary and not lit rocket
         } // end of if not item 9
      } // end of iterate all active items
}

int player_drop_item(int p)
{
   int i = players[p].carry_item-1; // number of item
   // printf("drop item:%d\n", i);

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
         while (is_right_solid(x, y, 1, 1))
         {
            x--;
            wall_stuck++; // just how stuck was it?
         }
         itemf[i][0] = al_itofix(x);
      }

      if (!players[p].left_right) // left
      {
         // is item embedded in a wall to the left?
         while (is_left_solid(x, y, 1, 1))
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
            int wall_stuck = player_drop_item(p);

            if (players[p].paused && players[p].paused_type == 2) // door travel
            {
               itemf[i][2] = al_itofix(0);
               itemf[i][3] = al_itofix(0);
            }
            else if (wall_stuck < 6)
            {
               if (item[i][0] != 98)            // not lit rocket
               {
                  itemf[i][2] = players[p].xinc;  // inherit the players momentum
                  itemf[i][3] = players[p].yinc;
                  if (players[p].up)    itemf[i][3] -= al_itofix(6); // throw item upwards
                  if (players[p].down)  itemf[i][3] = al_itofix(3); // throw item downwards
                  if (players[p].left)  itemf[i][2] -= al_itofix(2); // throw item left
                  if (players[p].right) itemf[i][2] += al_itofix(2); // throw item right
               }

               // prevent sticky bombs from sticking to the ground when throwing upwards
               if ((item[i][0] == 99) && (item[i][11]) && (players[p].up)) itemf[i][1] -= al_itofix(2);

            }
         }
      }
}

void proc_door_collision(int p, int i)
{
   if ((players[p].marked_door == -1)  // player has no marked door yet
     && (players[p].carry_item != i+1)) // player is not carrying this door
   {

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
         players[p].marked_door = i;
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

               if (is_player_riding_rocket(p)) instant_move = 1; // 1 = force instant if riding rocket

               if (instant_move)
               {
                  players[p].PX = itemf[li][0];
                  players[p].PY = itemf[li][1];

                  if (is_player_riding_rocket(p))
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



















void proc_start_collision(int p, int i)
{
   players[p].spawn_point_index = item[i][7]; // set new spawn point

   // mark this one as active and all others as not
   for (int ii=0; ii<500; ii++)
      if (item[ii][0] == 5)
      {
         if (item[ii][7] == item[i][7]) item[ii][1] = 1021;
         else item[ii][1] = 1011;
      }
}


































void proc_bonus_collision(int p, int i)
{
   int bonus_type = item[i][6];

   if (bonus_type == 1) // health bonus
   {
      al_fixed f100 = al_itofix(100);


      if (players[p].LIFE < f100)
      {
         item[i][0] = 0;
         players[p].LIFE += al_itofix(item[i][7]);
         if (players[p].LIFE > f100) players[p].LIFE = f100;

         game_event(72, 0, 0, p, i, item[i][1], item[i][7]);
      }
   }

   if (bonus_type == 2) // free man
   {
      item[i][0] = 0;
      players[p].LIVES++;
      game_event(70, 0, 0, p, i, 0, 0);
   }



   if (bonus_type == 3) // purple coin!!!
   {
      item[i][0] = 0;
      // game_event(70, 0, 0, p, i, 0, 0);

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
   if (item[i][11] == 0) // only collide if not already moving
   {

//      int x2 = (item[i][6] + item[i][8]) * 10;         // get the center of the block range
//      int y2 = (item[i][7] + item[i][9]) * 10;
      int x2 = (item[i][6] + item[i][8] / 2);         // get the center of the block range
      int y2 = (item[i][7] + item[i][9] / 2);



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
      al_fixed ns;                                     // get the number of steps
      if (abs(xlen) > abs(ylen)) ns = al_fixdiv(xlen, xinc);
      else  ns = al_fixdiv(ylen, yinc);
      int num_steps = al_fixtoi(ns);
      item[i][11] = num_steps + 10;                    // add 10 for final sequence
      game_event(2, 0, 0, p, i, 0, 0);
   }
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
               if ((l[c][y]&1023) == item[i][11]) // empty switch block
               {
                  l[c][y] = item[i][10] | PM_BTILE_ALL_SOLID; // replace with solid switch block
                  al_draw_filled_rectangle(c*20, y*20, c*20+19, y*20+19, palette_color[0]);
                  al_draw_bitmap(btile[l[c][y]&1023], c*20, y*20, 0 );
               }
               else if ((l[c][y]&1023) == item[i][10]) // solid switch block
               {
                  l[c][y] = item[i][11]; // replace with empty switch block
                  al_draw_filled_rectangle(c*20, y*20, c*20+19, y*20+19, palette_color[0]);
                  al_draw_bitmap(btile[l[c][y]&1023], c*20, y*20, 0 );
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
   // make it so any item other than bonus has higher priority
   // if carrying bonus, it will be dropped and new item will be carried
   int already_carrying = 0;
   if (players[p].carry_item) // already carrying item
   {
      already_carrying = 1;
      if ((item[players[p].carry_item][0] == 10) && (item[i][0] != 10)) // carried item is bonus and new item is not bonus
         already_carrying = 0;
   }

   // check if player can carry item
   if ( (!already_carrying)  // not carrying
     && (item[i][3] < 0)     // item is carryable
     && (players[p].fire) )  // fire pressed

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
          //printf("player picked up item:%d\n", i);
       }
       // allow multiple player carry for rocket
       if (item[i][0] == 98) players[p].carry_item = i+1;
   }

   switch (item[i][0]) // item type
   {
      case 1:  proc_door_collision(p, i);     break;
      case 2:  proc_bonus_collision(p, i);    break;
      case 3:  proc_exit_collision(p, i);     break;
      case 4:  proc_key_collision(p, i);      break;
      case 5:  proc_start_collision(p, i);    break;
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
   if ( ((itemf[i][3]<al_itofix(0)) && (is_up_solid(     x, y, 0, 3) == 1)) ||
        ((itemf[i][3]>al_itofix(0)) && (is_down_solid(   x, y, 0, 3) == 1)) ||
        ((itemf[i][3]>al_itofix(0)) && (is_down_solid(   x, y, 0, 3) == 2)) ||
        ((itemf[i][2]<al_ftofix(-1.1)) && (is_left_solid(x, y, 0, 3) == 1)) ||
        ((itemf[i][2]>al_ftofix(1.1)) && (is_right_solid(x, y, 0, 3) == 1)) )
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
      // if any players are riding this rocket, bind then to rocket's position
      for (int p=0; p<NUM_PLAYERS; p++)
         if ( (players[p].active) && (!players[p].paused) && (players[p].carry_item) && (players[p].carry_item == i+1 ))
         {
            players[p].PX = itemf[i][0];
            players[p].PY = itemf[i][1];
         }
   }
}





/*


item[][0] = 9  - Trigger
item[][1] =
item[][2] = draw_type (color)
item[][3] = flags
item[][4] = x pos (2000)
item[][5] = y pos (2000)

item[][6]  = trigger field x (2000)
item[][7]  = trigger field y (2000)
item[][8]  = trigger field w (2000)
item[][9]  = trigger field x (2000)

item[][10] = trigger field lift number

item[][11] = CURR ON  pm_event
item[][12] = CURR OFF pm_event
item[][13] = TGON pm_event #
item[][14] = TGOF pm_event #


#define PM_ITEM_TRIGGER_PLAYER   0b0000000000000001
#define PM_ITEM_TRIGGER_ENEMY    0b0000000000000010
#define PM_ITEM_TRIGGER_ITEM     0b0000000000000100
#define PM_ITEM_TRIGGER_PBUL     0b0000000000001000
#define PM_ITEM_TRIGGER_EBUL     0b0000000000010000
#define PM_ITEM_TRIGGER_CURR     0b0000000000100000
#define PM_ITEM_TRIGGER_PREV     0b0000000001000000
#define PM_ITEM_TRIGGER_TGON     0b0000000010000000
#define PM_ITEM_TRIGGER_TGOF     0b0000000100000000
#define PM_ITEM_TRIGGER_LIFT_ON  0b0000001000000000
#define PM_ITEM_TRIGGER_LIFT_XC  0b0000010000000000
#define PM_ITEM_TRIGGER_LIFT_XF  0b0000100000000000
#define PM_ITEM_TRIGGER_LIFT_XL  0b0001000000000000
#define PM_ITEM_TRIGGER_LIFT_YC  0b0010000000000000
#define PM_ITEM_TRIGGER_LIFT_YF  0b0100000000000000
#define PM_ITEM_TRIGGER_LIFT_YL  0b1000000000000000

*/

void process_trigger(int i)
{
   int FLAGS = item[i][3];
   if (FLAGS & PM_ITEM_TRIGGER_LIFT_ON) set_item_trigger_location_from_lift(i, 0);

   item[i][3] &= ~PM_ITEM_TRIGGER_TGON;  // clear Toggle ON  trigger flag
   item[i][3] &= ~PM_ITEM_TRIGGER_TGOF;  // clear Toggle OFF trigger flag
   item[i][3] &= ~PM_ITEM_TRIGGER_CURR;  // clear current    trigger flag

   detect_trigger_collisions(i);

   if ( (item[i][3] &  PM_ITEM_TRIGGER_CURR)    // is current trigger flag set?
   && (!(item[i][3] &  PM_ITEM_TRIGGER_PREV)))  // and previous trigger flag not set?
         item[i][3] |= PM_ITEM_TRIGGER_TGON;    // set trigger ON toggle


   if (!(item[i][3] &  PM_ITEM_TRIGGER_CURR)    // is current trigger flag not set?
   && ( (item[i][3] &  PM_ITEM_TRIGGER_PREV)))  // and previous trigger flag set?
         item[i][3] |= PM_ITEM_TRIGGER_TGOF;    // set trigger OFF toggle


   if   (item[i][3] &   PM_ITEM_TRIGGER_CURR)    // is current trigger flag set?
         item[i][3] |=  PM_ITEM_TRIGGER_PREV;    // set previous trigger flag

   if (!(item[i][3] &   PM_ITEM_TRIGGER_CURR))   // is current trigger flag not set?
         item[i][3] &= ~PM_ITEM_TRIGGER_PREV;    // clear previous trigger flag

   FLAGS = item[i][3]; // update FLAGS
/*   if (FLAGS & PM_ITEM_TRIGGER_CURR) printf("%d - CURR\n", frame_num);
   if (FLAGS & PM_ITEM_TRIGGER_PREV) printf("%d - PREV\n", frame_num);
   if (FLAGS & PM_ITEM_TRIGGER_TGON) printf("%d - TGON\n", frame_num);
   if (FLAGS & PM_ITEM_TRIGGER_TGOF) printf("%d - TGOF\n", frame_num); */
   if   (FLAGS & PM_ITEM_TRIGGER_CURR)  pm_event[item[i][11]] = 1;
   if (!(FLAGS & PM_ITEM_TRIGGER_CURR)) pm_event[item[i][12]] = 1;
   if   (FLAGS & PM_ITEM_TRIGGER_TGON)  pm_event[item[i][13]] = 1;
   if   (FLAGS & PM_ITEM_TRIGGER_TGOF)  pm_event[item[i][14]] = 1;
}

void set_item_trigger_location_from_lift(int i, int a20)
{
   int d = item[i][10]; // lift number
   if (d < num_lifts) // only proceed if lift number is valid
   {
      // x axis
      int lx1 = lifts[d].x1;
      int lx2 = lifts[d].x2;
      int C = item[i][3] & PM_ITEM_TRIGGER_LIFT_XC;
      int F = item[i][3] & PM_ITEM_TRIGGER_LIFT_XF;
      int L = item[i][3] & PM_ITEM_TRIGGER_LIFT_XL;
      if (C)
      {
         int lxc = lx1 + (lx2-lx1)/2; // get center of lift
         item[i][6] = lxc - item[i][8]/2;
      }
      else
      {
         if ((!F) && (!L)) item[i][6] = lx1;             // fx1 = lx1
         if ((!F) && ( L)) item[i][6] = lx2;             // fx1 = lx2
         if (( F) && (!L)) item[i][6] = lx1 - item[i][8]; // fx2 = lx1
         if (( F) && ( L)) item[i][6] = lx2 - item[i][8]; // fx2 = lx2
      }
      // y axis
      int ly1 = lifts[d].y1;
      int ly2 = lifts[d].y2;
      C = item[i][3] & PM_ITEM_TRIGGER_LIFT_YC;
      F = item[i][3] & PM_ITEM_TRIGGER_LIFT_YF;
      L = item[i][3] & PM_ITEM_TRIGGER_LIFT_YL;

      if (C)
      {
         int lyc = ly1 + (ly2-ly1)/2; // get center of lift
         item[i][7] = lyc - item[i][9]/2;
      }
      else
      {
         if ((!F) && (!L)) item[i][7] = ly1;             // fy1 = ly1
         if ((!F) && ( L)) item[i][7] = ly2;             // fy1 = ly2
         if (( F) && (!L)) item[i][7] = ly1 - item[i][9]; // fy2 = ly1
         if (( F) && ( L)) item[i][7] = ly2 - item[i][9]; // fy2 = ly2
      }
      if (a20) // align to 20 grid
      {
         item[i][6] = round20(item[i][6]);
         item[i][7] = round20(item[i][7]);
      }
   }
}


void detect_trigger_collisions(int i)
{
   int FLAGS = item[i][3];

   // trigger field
   al_fixed tfx1 = al_itofix(item[i][6]-10);
   al_fixed tfy1 = al_itofix(item[i][7]-10);
   al_fixed tfx2 = tfx1 + al_itofix(item[i][8]);
   al_fixed tfy2 = tfy1 + al_itofix(item[i][9]);

   if (FLAGS & PM_ITEM_TRIGGER_PLAYER)
      for (int p=0; p<NUM_PLAYERS; p++)
         if ((players[p].active) && (!players[p].paused))
         {
            al_fixed x = players[p].PX;
            al_fixed y = players[p].PY;
            if ((x > tfx1) && (x < tfx2) && (y > tfy1) && (y < tfy2)) item[i][3] |= PM_ITEM_TRIGGER_CURR;
         }
   if (FLAGS & PM_ITEM_TRIGGER_ENEMY)
      for (int e2=0; e2<100; e2++)
         if (Ei[e2][0])
         {
            al_fixed x = Efi[e2][0];
            al_fixed y = Efi[e2][1];
            if ((x > tfx1) && (x < tfx2) && (y > tfy1) && (y < tfy2)) item[i][3] |= PM_ITEM_TRIGGER_CURR;
         }
   if (FLAGS & PM_ITEM_TRIGGER_ITEM)
      for (int c=0; c<500; c++)
         if (item[c][0])
         {
            al_fixed x = itemf[c][0];
            al_fixed y = itemf[c][1];
            if ((x > tfx1) && (x < tfx2) && (y > tfy1) && (y < tfy2)) item[i][3] |= PM_ITEM_TRIGGER_CURR;
         }
   if (FLAGS & PM_ITEM_TRIGGER_PBUL) // check player bullets
      for (int b=0; b<50; b++)
         if (pbullet[b][0])
         {
            al_fixed x = al_itofix(pbullet[b][2]);
            al_fixed y = al_itofix(pbullet[b][3]);
            if ((x > tfx1) && (x < tfx2) && (y > tfy1) && (y < tfy2)) item[i][3] |= PM_ITEM_TRIGGER_CURR;
         }
   if (FLAGS & PM_ITEM_TRIGGER_EBUL) // check enemy bullets
      for (int b=0; b<50; b++)
         if (e_bullet_active[b])
         {
            al_fixed x = e_bullet_fx[b];
            al_fixed y = e_bullet_fy[b];
            if ((x > tfx1) && (x < tfx2) && (y > tfy1) && (y < tfy2)) item[i][3] |= PM_ITEM_TRIGGER_CURR;
         }
}





void draw_trigger(int i)
{
   if (level_editor_running)
   {
      al_draw_bitmap(tile[991], item[i][4], item[i][5], 0); // draw item shape in level editor, invisible when game running
      if (item[i][3] & PM_ITEM_TRIGGER_LIFT_ON) set_item_trigger_location_from_lift(i, 1); // snap to lift here because main function wont be called while in level editor
   }

   if (item[i][3] & PM_ITEM_TRIGGER_DRAW_ON)
   {
      int col = item[i][2];
      float x1 = item[i][6];
      float y1 = item[i][7];
      float x2 = x1 + item[i][8];
      float y2 = y1 + item[i][9];
      rectangle_with_diagonal_lines(x1, y1, x2, y2, 10, col, col+96);
   }
}




/*

item[][0] = 16 - Block Manip
item[][1] = pm_event_trigger
item[][2] = draw on
item[][3] = mode

item[][4] = x pos (2000)
item[][5] = y pos (2000)

item[][6]  = trigger field x (2000)
item[][7]  = trigger field y (2000)
item[][8]  = trigger field w (2000)
item[][9]  = trigger field x (2000)


item[][10] block 1
item[][11] block 2


item[][12] = draw color




*/



void process_block_manip(int i)
{
   int et = item[i][1]; // pm_event trigger we are looking for
   if (pm_event[et])
   {
      al_set_target_bitmap(level_background);
      int x1 = item[i][6]/20;
      int y1 = item[i][7]/20;
      int x2 = x1 + item[i][8]/20;
      int y2 = y1 + item[i][9]/20;

      for (int x=x1; x<x2; x++)
         for (int y=y1; y<y2; y++)
         {
            int mode = item[i][3];
            int block1 = item[i][10];
            int block2 = item[i][11];

            if (mode == 1) // set all blocks to block 1
            {
               l[x][y] = block1;
               al_draw_filled_rectangle(x*20, y*20, x*20+20, y*20+20, palette_color[0]);
               al_draw_bitmap(btile[block1&1023], x*20, y*20, 0 );
            }

            if (mode == 2) // set all block2 to block 1
            {
               if (l[x][y] == block2)
               {
                  l[x][y] = block1;
                  al_draw_filled_rectangle(x*20, y*20, x*20+20, y*20+20, palette_color[0]);
                  al_draw_bitmap(btile[block1&1023], x*20, y*20, 0 );
               }
            }

            if (mode == 3) // toggle block1 and block 2
            {
               if (l[x][y] == block1)
               {
                  l[x][y] = block2;
                  al_draw_filled_rectangle(x*20, y*20, x*20+20, y*20+20, palette_color[0]);
                  al_draw_bitmap(btile[block2&1023], x*20, y*20, 0 );
               }
               else if (l[x][y] == block2)
               {
                  l[x][y] = block1;
                  al_draw_filled_rectangle(x*20, y*20, x*20+20, y*20+20, palette_color[0]);
                  al_draw_bitmap(btile[block1&1023], x*20, y*20, 0 );
               }
            }


/* old way...uses only tile number, not tile and flags


            if (mode == 1) // set all blocks to block 1
            {
               l[x][y] = block1 | sa[block1][0]; // replace block (use default flags)
               al_draw_filled_rectangle(x*20, y*20, x*20+20, y*20+20, palette_color[0]);
               al_draw_bitmap(btile[block1], x*20, y*20, 0 );
            }

            if (mode == 2) // set all block2 to block 1
            {
               if ((l[x][y]&1023) == block2)
               {
                  l[x][y] = block1 | sa[block1][0]; // replace block
                  al_draw_filled_rectangle(x*20, y*20, x*20+20, y*20+20, palette_color[0]);
                  al_draw_bitmap(btile[block1], x*20, y*20, 0 );
               }
            }

            if (mode == 3) // toggle block1 and block 2
            {
               if ((l[x][y]&1023) == block1)
               {
                  l[x][y] = block2 | sa[block2][0];
                  al_draw_filled_rectangle(x*20, y*20, x*20+20, y*20+20, palette_color[0]);
                  al_draw_bitmap(btile[block2], x*20, y*20, 0 );
               }
               else if ((l[x][y]&1023) == block2)
               {
                  l[x][y] = block1 | sa[block1][0];
                  al_draw_filled_rectangle(x*20, y*20, x*20+20, y*20+20, palette_color[0]);
                  al_draw_bitmap(btile[block1], x*20, y*20, 0 );
               }
            }



*/


























         }
      draw_lift_lines();
      pm_event[et] = 0; // clear the trigger when we are done with it
   }
}



void draw_block_manip(int i)
{
   if (level_editor_running)
   {
      al_draw_bitmap(tile[989], item[i][4], item[i][5], 0); // draw item shape in level editor, invisible when game running
   }
   if (item[i][2]) // draw mode on
   {
      int col = item[i][12];
      float x1 = item[i][6];
      float y1 = item[i][7];
      float x2 = x1 + item[i][8];
      float y2 = y1 + item[i][9];
      rectangle_with_diagonal_lines(x1, y1, x2, y2, 10, col, col+96);
   }
}





/*

item[][0]  = 17 - Block Damage
item[][1]  = event trigger number
item[][2]  = damage draw type
item[][3]  = flags
#define PM_ITEM_DAMAGE_PLAYER   0b00000000000000001
#define PM_ITEM_DAMAGE_ENEMY    0b00000000000000010
#define PM_ITEM_DAMAGE_ITEM     0b00000000000000100
#define PM_ITEM_DAMAGE_PBUL     0b00000000000001000
#define PM_ITEM_DAMAGE_EBUL     0b00000000000010000
#define PM_ITEM_DAMAGE_CURR     0b00000000000100000
#define PM_ITEM_DAMAGE_LIFT_ON  0b00000000001000000
#define PM_ITEM_DAMAGE_LIFT_XC  0b00000000010000000
#define PM_ITEM_DAMAGE_LIFT_XF  0b00000000100000000
#define PM_ITEM_DAMAGE_LIFT_XL  0b00000001000000000
#define PM_ITEM_DAMAGE_LIFT_YC  0b00000010000000000
#define PM_ITEM_DAMAGE_LIFT_YF  0b00000100000000000
#define PM_ITEM_DAMAGE_LIFT_YL  0b00001000000000000

item[][4]  = x pos   (2000)
item[][5]  = y pos   (2000)
item[][6]  = field x (2000)
item[][7]  = field y (2000)
item[][8]  = field w (2000)
item[][9]  = field h (2000)
item[][10] = lift number
item[][11] = mode
item[][12] = t1 val
item[][13] = count
item[][14] = t2 val
item[][15] = damage

*/



void set_item_damage_location_from_lift(int i, int a20)
{
   int d = item[i][10]; // lift number
   if (d < num_lifts) // only proceed if lift number is valid
   {
      // x axis
      int lx1 = lifts[d].x1;
      int lx2 = lifts[d].x2;
      int C = item[i][3] & PM_ITEM_DAMAGE_LIFT_XC;
      int F = item[i][3] & PM_ITEM_DAMAGE_LIFT_XF;
      int L = item[i][3] & PM_ITEM_DAMAGE_LIFT_XL;
      if (C)
      {
         int lxc = lx1 + (lx2-lx1)/2; // get center of lift
         item[i][6] = lxc - item[i][8]/2;
      }
      else
      {
         if ((!F) && (!L)) item[i][6] = lx1;              // fx1 = lx1
         if ((!F) && ( L)) item[i][6] = lx2;              // fx1 = lx2
         if (( F) && (!L)) item[i][6] = lx1 - item[i][8]; // fx2 = lx1
         if (( F) && ( L)) item[i][6] = lx2 - item[i][8]; // fx2 = lx2
      }
      // y axis
      int ly1 = lifts[d].y1;
      int ly2 = lifts[d].y2;
      C = item[i][3] & PM_ITEM_DAMAGE_LIFT_YC;
      F = item[i][3] & PM_ITEM_DAMAGE_LIFT_YF;
      L = item[i][3] & PM_ITEM_DAMAGE_LIFT_YL;

      if (C)
      {
         int lyc = ly1 + (ly2-ly1)/2; // get center of lift
         item[i][7] = lyc - item[i][9]/2;
      }
      else
      {
         if ((!F) && (!L)) item[i][7] = ly1;              // fy1 = ly1
         if ((!F) && ( L)) item[i][7] = ly2;              // fy1 = ly2
         if (( F) && (!L)) item[i][7] = ly1 - item[i][9]; // fy2 = ly1
         if (( F) && ( L)) item[i][7] = ly2 - item[i][9]; // fy2 = ly2
      }
      if (a20) // align to 20 grid
      {
         item[i][6] = round20(item[i][6]);
         item[i][7] = round20(item[i][7]);
      }
   }
}





void proc_item_damage_collisions(int i)
{
   int FLAGS = item[i][3];
   int cd = FLAGS & PM_ITEM_DAMAGE_CURR;                 // damage active
   int cdp =  ((cd) && (FLAGS & PM_ITEM_DAMAGE_PLAYER)); // damage active and player flag
   int cde =  ((cd) && (FLAGS & PM_ITEM_DAMAGE_ENEMY));  // damage active and enemy flag
   int cdi =  ((cd) && (FLAGS & PM_ITEM_DAMAGE_ITEM));   // damage active and item flag
   int cdpb = ((cd) && (FLAGS & PM_ITEM_DAMAGE_PBUL));   // damage active and player bullet flag
   int cdeb = ((cd) && (FLAGS & PM_ITEM_DAMAGE_EBUL));   // damage active and enemy bullet flag

   // damage field
   al_fixed tfx1 = al_itofix(item[i][6]-10);
   al_fixed tfy1 = al_itofix(item[i][7]-10);
   al_fixed tfx2 = tfx1 + al_itofix(item[i][8]);
   al_fixed tfy2 = tfy1 + al_itofix(item[i][9]);

   if (cdp)
      for (int p=0; p<NUM_PLAYERS; p++)
         if ((players[p].active) && (!players[p].paused))
         {
            al_fixed x = players[p].PX;
            al_fixed y = players[p].PY;
            if ((x > tfx1) && (x < tfx2) && (y > tfy1) && (y < tfy2))
            {
               if (FLAGS & PM_ITEM_DAMAGE_INSTGIB)
               {
                  players[p].LIFE = al_itofix(0);
                  //game_event(57, 0, 0, p, i, 0, 0);
               }
               else
               {
                  players[p].LIFE -= al_fixdiv(al_itofix(item[i][15]), al_itofix(100));
                  game_event(59, 0, 0, p, i, 0, 0);
               }
            }
         }
   if (cde)
      for (int e2=0; e2<100; e2++)
         if (Ei[e2][0])
         {
            al_fixed x = Efi[e2][0];
            al_fixed y = Efi[e2][1];
            if ((x > tfx1) && (x < tfx2) && (y > tfy1) && (y < tfy2))
            {
               Ei[e2][31] = 3;           // flag that this enemy got shot with bullet
               //Ei[e2][26] = x;           // number of player's bullet that hit enemy
            }
         }
   if (cdi)
      for (int i=0; i<500; i++)
         if (item[i][0])
         {
            al_fixed x = itemf[i][0];
            al_fixed y = itemf[i][1];
            if ((x > tfx1) && (x < tfx2) && (y > tfy1) && (y < tfy2))
            {
               if (item[i][0] != 66)
               {
                  //item[i][0] = 66;
                  item[i][14] = 10;
               }
            }
         }
   if (cdpb) // check player bullets
      for (int b=0; b<50; b++)
         if (pbullet[b][0])
         {
            al_fixed x = al_itofix(pbullet[b][2]);
            al_fixed y = al_itofix(pbullet[b][3]);
            if ((x > tfx1) && (x < tfx2) && (y > tfy1) && (y < tfy2)) pbullet[b][0] = 0; // kill the bullet
         }
   if (cdeb) // check enemy bullets
      for (int b=0; b<50; b++)
         if (e_bullet_active[b])
         {
            al_fixed x = e_bullet_fx[b];
            al_fixed y = e_bullet_fy[b];
            if ((x > tfx1) && (x < tfx2) && (y > tfy1) && (y < tfy2)) e_bullet_active[b] = 0; // kill the bullet
         }
}

void draw_block_damage(int i)
{
   int draw_mode = item[i][2];
   int mode = item[i][11];
   int FLAGS = item[i][3];

   float x0 = item[i][4];
   float y0 = item[i][5];
   float x1 = item[i][6];
   float y1 = item[i][7];
   float x2 = x1 + item[i][8];
   float y2 = y1 + item[i][9];

   if (level_editor_running)
   {
      al_draw_bitmap(tile[988], x0, y0, 0); // draw item shape in level editor, invisible when game running
      if (FLAGS & PM_ITEM_DAMAGE_LIFT_ON) set_item_damage_location_from_lift(i, 1); // set this here only when level editor is running
   }

   // damage field drawing
   if (draw_mode == 1) // basic
   {
      int col = 11;
      if (FLAGS & PM_ITEM_DAMAGE_CURR) col = 10;
      rectangle_with_diagonal_lines(x1, y1, x2, y2, 10, col, col+96);
   }

   if (draw_mode == 2) // spikey floor
   {
      int tn = 808;
      if (FLAGS & PM_ITEM_DAMAGE_CURR) tn = 807;
      for (int hx = x1; hx<x2; hx+=20)
         al_draw_bitmap(tile[tn], hx, y2-20, 0); // draw extended spikes only on bottom row
   }


   // timer drawing
   int timer_draw_mode1 = item[i][3] & PM_ITEM_DAMAGE_TIMR_SN;
   int timer_draw_mode2 = item[i][3] & PM_ITEM_DAMAGE_TIMR_BN;
   int timer_draw_mode3 = item[i][3] & PM_ITEM_DAMAGE_TIMR_SP;
   int timer_draw_mode4 = item[i][3] & PM_ITEM_DAMAGE_TIMR_BP;


   int col = 15;

   if ((mode == 2) || (mode == 3))
   {
      if (((timer_draw_mode1) || (timer_draw_mode2)) && (item[i][13] > 0)) // small or big number and timer is running
      {
         // time to show
         int tts = (item[i][13]); // raw (40ths of a second)
         //int tts = (item[i][13] / 40); // seconds
         //int tts = (item[i][13] / 4); // tenths of a second
         //int tts = (item[i][13] / 8); // fifths of a second

         if (mode == 2) col = 11;
         if (mode == 3) col = 10;
         if (timer_draw_mode1) al_draw_textf(f3,   palette_color[col], x0+10, y0+4, ALLEGRO_ALIGN_CENTER, "%d", tts);
         if (timer_draw_mode2) al_draw_textf(font, palette_color[col], x0+10, y0+6, ALLEGRO_ALIGN_CENTER, "%d", tts);

      }
      if ((timer_draw_mode3) || (timer_draw_mode4)) // percent bar
      {
         int percent = 0;
         if (item[i][12] > 0) // prevent divide by zero
         {
            if (mode == 2) percent =       ((item[i][13]) * 100) / item[i][12];
            if (mode == 3) percent = 100 - ((item[i][13]) * 100) / item[i][12];
         }
         if (timer_draw_mode3) draw_percent_bar(x0+9, y0+5, 32, 8,  percent);
         if (timer_draw_mode4) draw_percent_bar(x0+9, y0+1, 64, 16, percent);

      }
   }
   if (mode == 4)
   {
      int tt = item[i][12]; // total time
      int ct = item[i][13]; // current time
      int st = item[i][14]; // switch time

      int percent = 0;
      int tts = 0;
      //int col = 0;

      if (ct >= st) // upper range, damage off
      {
         int ut = tt-st; // total time in the upper
         int dt = ct-st; // current time relative to that
         if (ut > 0)     // prevent divide by zero
         {
            //percent = 100 - (dt * 100) / ut;
            percent = (dt * 100) / ut;
            tts = (dt / 4) + 1;
            col = 11;
         }
      }
      else // lower range, damage on
      {
         int lt = st; // total time in lower
         int dt = ct; // current time relative to that
         if (lt > 0)  // prevent divide by zero
         {
            tts = (dt / 4) + 1;
            //percent = (dt * 100) / lt;
            percent = 100 - (dt * 100) / lt;
            col = 10;
         }
      }


      if (timer_draw_mode1) al_draw_textf(f3,   palette_color[col], x0+10, y0+4, ALLEGRO_ALIGN_CENTER, "%d", tts);
      if (timer_draw_mode2) al_draw_textf(font, palette_color[col], x0+10, y0+6, ALLEGRO_ALIGN_CENTER, "%d", tts);
      if (timer_draw_mode3) draw_percent_bar(x0+9, y0+5, 32, 8,  percent);
      if (timer_draw_mode4) draw_percent_bar(x0+9, y0+1, 64, 16, percent);
   }
}




void process_block_damage(int i)
{
   int et = item[i][1];      // number of pm_event trigger we are looking for
   int FLAGS = item[i][3];
   int mode = item[i][11];


   if (FLAGS & PM_ITEM_DAMAGE_LIFT_ON) set_item_damage_location_from_lift(i, 0); // follow lift location

   proc_item_damage_collisions(i);

   if (mode == 0) item[i][3] |= PM_ITEM_DAMAGE_CURR; // in mode 0, always set damage flag

   if (mode == 1)
   {
      if (pm_event[et])
      {
          item[i][3] ^= PM_ITEM_DAMAGE_CURR; // toggle current damage flag
          pm_event[et] = 0;
      }
   }
   if (mode == 2) // damage unless timer running  (no damage when triggered)
   {
      if (pm_event[et])
      {
          item[i][13] = item[i][12]; // reset timer
          pm_event[et] = 0;
      }
      if (item[i][13] == 0) item[i][3] |=  PM_ITEM_DAMAGE_CURR; // set damage on
      else                  item[i][3] &= ~PM_ITEM_DAMAGE_CURR; // set damage off
   }
   if (mode == 3) // damage when timer is running (no damage until triggered)
   {
      if (pm_event[et])
      {
          item[i][13] = item[i][12]; // reset timer
          pm_event[et] = 0;
      }
      if (item[i][13] > 0) item[i][3] |=  PM_ITEM_DAMAGE_CURR; // set damage on
      else                 item[i][3] &= ~PM_ITEM_DAMAGE_CURR; // set damage off
   }
   if (mode == 4) // timed on and off
   {
      // timer will run outside this function always, but in this mode, when it gets to zero, we will reset it
      if (item[i][13] == 0) item[i][13] = item[i][12];  // reset timer
      if (item[i][13] < item[i][14]) item[i][3] |=  PM_ITEM_DAMAGE_CURR; // set damage on
      else                           item[i][3] &= ~PM_ITEM_DAMAGE_CURR; // set damage off
   }
   if (--item[i][13] < 0) item[i][13] = 0; // always run timer
}




































/*
item variable descriptions

list of items



[1]  - door
[2]  - bonus
[3]  - exit
[4]  - key
[5]  - start
[6]  - free man
[7]  - mine
[8]  - bomb
[10] - pop-up msg
[11] - rocket
[12] - warp
[14] - switch
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

item[][6] bonus type
1 - Health
2 - Free Man
3 - Purple Coin

item[][7] health bonus
item[][8] bullet bonus (ignored now)
item[][9] timer bonus  (ignored now)

[3] - exit
item[][8] exit with x enemies left

[4] - key
item[][6]  block range x1 (100)
item[][7]  block range y1 (100)
item[][8]  block range x2 (100)
item[][9]  block range y2 (100)
item[][10] rotation for key move
item[][11] counter for key move
item[][12] matching keyed blocks only

[5] - start
item[][7] start index
item[][8] initial time (ignored now)

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



























