// z_item.cpp
#include "pm.h"
#include "z_item.h"
#include "z_sound.h"
#include "z_player.h"
#include "mwFont.h"
#include "mwBitmap.h"
#include "z_lift.h"
#include "mwWidgets.h"
#include "mwColor.h"
#include "mwPMEvent.h"
#include "mwDisplay.h"
#include "mwProgramState.h"
#include "z_menu.h"
#include "z_enemy.h"
#include "z_level.h"
#include "e_fnx.h"
#include "z_fnx.h"
#include "z_screen.h"
#include "z_screen_overlay.h"
#include "mwShots.h"

// items
int item[500][16];      // item ints
al_fixed itemf[500][4]; // item fixeds
int item_num_of_type[30];
int item_first_num[30];
char item_name[30][40];
char pmsgtext[500][500] = {0};
int item_tile[20];


int item_data(int x, int y)
{
   int inum = sort_item(0);
                               al_draw_textf(mF.pr8, mC.pc[12], x, y, 0, "%d Items",      inum);                 y+=8;
                               al_draw_textf(mF.pr8, mC.pc[12], x, y, 0, "--------");                            y+=8;
                               al_draw_textf(mF.pr8, mC.pc[13], x, y, 0, "%d Starts",     item_num_of_type[5]);  y+=8;
                               al_draw_textf(mF.pr8, mC.pc[13], x, y, 0, "%d Exits",      item_num_of_type[3]);  y+=8;
   if (item_num_of_type[12]) { al_draw_textf(mF.pr8, mC.pc[10], x, y, 0, "%d Warps",      item_num_of_type[12]); y+=8; }
   if (item_num_of_type[1])  { al_draw_textf(mF.pr8, mC.pc[13], x, y, 0, "%d Doors",      item_num_of_type[1]);  y+=8; }
   if (item_num_of_type[4])  { al_draw_textf(mF.pr8, mC.pc[13], x, y, 0, "%d Keys",       item_num_of_type[4]);  y+=8; }
   if (item_num_of_type[14]) { al_draw_textf(mF.pr8, mC.pc[13], x, y, 0, "%d Switches",   item_num_of_type[14]); y+=8; }
   if (item_num_of_type[15]) { al_draw_textf(mF.pr8, mC.pc[13], x, y, 0, "%d Sproingies", item_num_of_type[15]); y+=8; }
   if (item_num_of_type[8])  { al_draw_textf(mF.pr8, mC.pc[14], x, y, 0, "%d Bombs",      item_num_of_type[8]);  y+=8; }
   if (item_num_of_type[11]) { al_draw_textf(mF.pr8, mC.pc[14], x, y, 0, "%d Rockets",    item_num_of_type[11]); y+=8; }
   if (item_num_of_type[7])  { al_draw_textf(mF.pr8, mC.pc[14], x, y, 0, "%d Mines",      item_num_of_type[7]);  y+=8; }
   if (item_num_of_type[6])  { al_draw_textf(mF.pr8, mC.pc[14], x, y, 0, "%d Orbs",       item_num_of_type[6]);  y+=8; }

   for (int c=1; c<20; c++)
      if ((c!= 1) && (c !=3) && (c!= 4) && (c != 5) && (c != 6) && (c!= 7) && (c!= 8) && (c!= 9) && (c!= 11) && (c!= 12) && (c!= 14) && (c!= 15) && (c!= 16) && (c!= 17) )
         if (item_num_of_type[c]) // not zero
         {
                         sprintf(msg, "%d type %d???  ", item_num_of_type[c], c); // default unknown
            if (c ==  0) sprintf(msg, "%d type 0      ", item_num_of_type[c]);
            if (c ==  2) sprintf(msg, "%d Bonus       ", item_num_of_type[c]);
            if (c == 10) sprintf(msg, "%d Messages    ", item_num_of_type[c]);
            al_draw_text(mF.pr8, mC.pc[3], x, y, 0, msg);
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




typedef struct DRAW_CUSTOM_LINE_EXTRA
{
   int num_lines;
} DRAW_CUSTOM_LINE_EXTRA;

static bool draw_multiline_cb(int line_num, const char *line, int size, void *extra)
{
   DRAW_CUSTOM_LINE_EXTRA *s = (DRAW_CUSTOM_LINE_EXTRA *) extra;
   s->num_lines++;
   return 1;
}



void draw_pop_message(int i, int custom, int xpos_c, int ypos, int cursor_pos, int cursor_blink, char *f)
{
   // set where we will draw
   if (custom) al_set_target_backbuffer(display);
   else al_set_target_bitmap(mwB.level_buffer);

   // make a copy of the string
   char pt[500];

   if (custom) // get text from f
   {
      if (strlen(f) > 0) strcpy(pt, f);
      else sprintf(pt, "<empty>");
   }
   else strcpy(pt, pmsgtext[i]); // get text from pmsg


   int x1=0, y1=0, w=0, h=0, tc=0, fc = 0;
   get_int_3216(item[i][10], x1, y1);       // get x and y
   get_int_3216(item[i][11], w, h);         // get width and height
   get_int_3216(item[i][13], tc, fc);       // get text and frame colors

   if (custom) // get custom x and y
   {
      x1 = xpos_c - w/2 - 8;
      y1 = ypos - 10;
   }

   // other x and y positions
   int x2 = x1 + w;
   int xc = x1 + w / 2;
   int y2 = y1 + h;



   int frame_width = get_frame_size(i);




   if (frame_width == 0)
   {
      if (mwPS.level_editor_running) al_draw_rectangle(x1, y1, x2, y2, mC.pc[15], 1);
   }
   else al_draw_filled_rectangle(x1, y1, x2, y2, mC.pc[fc+13*16]);  // background


   if ((mwPS.eco_draw) && (frame_width > 1)) frame_width = 1;

   if (frame_width == 1)
   {
      al_draw_rectangle(x1, y1, x2, y2, mC.pc[fc], 1);
   }




   if (frame_width == 2)
   {
      for (int a=0; a<frame_width; a++)
         al_draw_rounded_rectangle(x1+a, y1+a, x2-a, y2-a, 4, 4, mC.pc[fc+a*128], 1.5);
   }
   if (frame_width == 4)
   {
      for (int a=0; a<frame_width; a++)
         al_draw_rounded_rectangle(x1+a, y1+a, x2-a, y2-a, 4, 4, mC.pc[fc+a*48], 1.5);
   }
   if (frame_width == 12)
   {
      for (int a=0; a<frame_width; a++)
         al_draw_rounded_rectangle(x1+a, y1+a, x2-a, y2-a, 6, 6, mC.pc[fc+a*16], 1.5);
   }


   // debug show inner frame
//   al_draw_rounded_rectangle(x1+frame_width, y1+frame_width, x2-frame_width, y2-frame_width, 4, 4, mC.pc[15], 1);



   if (custom)
   {

      // show cursor char
      sprintf(msg, "%c", pt[cursor_pos]);
      if (pt[cursor_pos] == 0)  sprintf(msg, "NULL");
      if (pt[cursor_pos] == 10) sprintf(msg, "LF");
      if (pt[cursor_pos] == 32) sprintf(msg, "SPACE");

      al_draw_textf(mF.pr8, mC.pc[15], xc+4, y2+2, ALLEGRO_ALIGN_CENTRE, "%d/%d/500 [%s] ", cursor_pos, (int) strlen(pt), msg);

//      al_draw_textf(mF.pr8, mC.pc[15], xc+4, y1+2, ALLEGRO_ALIGN_CENTRE, "[%s] %d/%d/500", msg, cursor_pos, (int) strlen(pt));

//      al_draw_textf(mF.pr8, mC.pc[15], x2-60, y2-9, 0, "[%s]", msg);
      //al_draw_textf(mF.pr8, mC.pc[15], xc+4, y1-20, ALLEGRO_ALIGN_CENTRE, "x:%d y:%d w:%d h:%d", x1, y1, w, h);
   }








   // figure out what line height to use so that text is justified vertically
   float line_height = 7.9; // initial line height

   int max_text_width = w - 16;

   // do_multiline callback to figure out number of lines that will be used
   DRAW_CUSTOM_LINE_EXTRA extra;
   extra.num_lines = 0;
   al_do_multiline_text(mF.pr8, max_text_width, pt, draw_multiline_cb, (void *)&extra);

   float sp=0; // space between text_height and frame_height
   float text_height = 0;

   do
   {
      line_height+=0.1;
      text_height = extra.num_lines * line_height;
      sp = h - frame_width*2 - text_height;

   } while ((sp > 2) && (line_height < 100));


//   al_draw_textf(mF.pr8, mC.pc[15], xc+4, y2+20, ALLEGRO_ALIGN_CENTRE, "lh:%2.1f nl:%d th:%2.1f", line_height, extra.num_lines, text_height);
//   al_draw_textf(mF.pr8, mC.pc[15], xc+4, y2+28, ALLEGRO_ALIGN_CENTRE, "fh:%d fh-fw:%d sp:%2.1f", h, h - frame_width*2, sp);


   float y3 = y1+frame_width+line_height/2-3.5;


   if (sp < 1)
   {
      sp = h - text_height;
      y3 = y1+sp/2;

   }

   al_draw_multiline_text(mF.pr8, mC.pc[tc], xc, y3, max_text_width, line_height, ALLEGRO_ALIGN_CENTRE, pt);

   if (cursor_blink)
   {

      // convert all printable char to blank non-printable spaces (can't use space because multiline text will break in new places)
      for (int a=0; a<(int)strlen(pt); a++)
         if ((pt[a] > 32) && (pt[a] < 126)) pt[a] = 30; // alternate space

      // check if cursor is on special char
      int cursor_on_special = 0;
      if ((pt[cursor_pos] == 10) || (pt[cursor_pos] == 32))
      {
         int dcp = cursor_pos;
         cursor_on_special = 1;
         while ( (dcp > 0) &&  ((pt[dcp] == 10) || (pt[dcp] == 32)) ) dcp--; // find previous non special char
         pt[dcp] = 95;
      }
      else pt[cursor_pos] = 95;

      al_draw_multiline_text(mF.pr8, mC.pc[10], xc+cursor_on_special*8, y3, max_text_width, line_height,  ALLEGRO_ALIGN_CENTRE, pt);
   }

//      // crosshairs for alignment
//      al_draw_line(x1, y1, x2, y2, mC.pc[fc], 1);
//      al_draw_line(x1, y2, x2, y1, mC.pc[fc], 1);
//      al_draw_line(x1+w/2, y1,     x1+w/2, y2,     mC.pc[fc], 1);
//      al_draw_line(x1,     y1+h/2, x2,       y1+h/2, mC.pc[fc], 1);

}



void draw_door(int i, int x, int y, int custom)
{
   ALLEGRO_BITMAP *tmp = NULL;
   int col = item[i][6];
   int drawn = 0;
   if (item[i][13] == 448) // old style door shape
   {
      int shape = item[i][1];       // get shape
      int si = shape-448;           // convert to index to bitmap sequence
      tmp = mwB.door_tile[1][col][si];
   }
   else if (item[i][13] == 1083) // new style doors
   {
      int an = mwB.zz[1][83];             // cheat and use shape index from base door animation sequence
      if (item[i][8] == 0) an = 7-an; // exit only, run the sequence backwards
      tmp = mwB.door_tile[0][col][an];
   }
   else if (item[i][13] == 0) drawn = 1;

   if ((item[i][8] == 1) && (!custom)) // linked item destination
   {
      // linked destination item position
      int dx = al_fixtoi(itemf[item[i][9]][0]);
      int dy = al_fixtoi(itemf[item[i][9]][1]);
      int line_color = item[item[i][9]][6];

      if (item[i][12] == 1) // if always draw lines (1)
         al_draw_line(x+10, y+10, dx+10, dy+10, mC.pc[line_color], 1);  // draw a line connecting them

      if (!drawn)
      {
         for (int p=0; p<NUM_PLAYERS; p++) // is player touching door
            if ((players[p].active) && (players[p].marked_door == i))
            {
               if (item[i][12] > 0)  // always draw or only draw when touching ( 1 or 2)
                  al_draw_line(x+10, y+10, dx+10, dy+10, mC.pc[line_color], 1);  // draw a line connecting them

               // bigger door when player touching it
               al_draw_scaled_bitmap(tmp, 0, 0, 20, 20, x-5, y-6, 30, 26, 0 );

               if (item[i][8] == 0) al_draw_scaled_bitmap(mwB.tile[1015], 0, 0, 20, 20, x-5, y-6, 30, 26, 0); // OUT
               else al_draw_scaled_bitmap(mwB.tile[1014], 0, 0, 20, 20, x-5, y-6, 30, 26, 0); // IN

               if (item[i][11] == 1) // enter with <up>
                  al_draw_text(mF.pr8, mC.pc[15],  x+3, y-14, 0, "up");

               if (item[i][11] == 2) // enter with <down>
                  al_draw_text(mF.pr8, mC.pc[15],  x-5, y-14, 0, "down");
               drawn = 1;
            }
      }
   }
   if (!drawn)
   {
      al_draw_bitmap(tmp, x, y, 0); // if not drawn yet

      if (item[i][8] == 0) al_draw_bitmap(mwB.tile[1015], x, y, 0); // OUT
      else al_draw_bitmap(mwB.tile[1014], x, y, 0); // IN
   }

}

int seq_color(int mod, int c1, int c2)
{
   int col = c1; // initial color
   if ( (mwPS.frame_num % mod) < mod/2) col = c2; // other color
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




   int mod = mwPS.frame_num % 40;
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

   int mod = mwPS.frame_num % 20;
   return ca[mod];
}

void bomb_block_crosshairs(int e, int f)
{
   //int col = seq_color(16, 14, 10);
   int col = seq_color2();
   if (col)
   {
      al_draw_rectangle(e*20, f*20, e*20+20, f*20+20, mC.pc[col], 1);
      al_draw_line(e*20, f*20, e*20+19, f*20+19, mC.pc[col], 1);
      al_draw_line(e*20, f*20+19, e*20+19, f*20, mC.pc[col], 1);
   }
}

void bomb_blocks(int i, int t, int dr, al_fixed fx, al_fixed fy)
{


   // center of bomb
//   float x = al_fixtoi(itemf[i][0]) + 10;
//   float y = al_fixtoi(itemf[i][1]) + 10;
   float x = al_fixtoi(fx) + 10;
   float y = al_fixtoi(fy) + 10;

//   printf("bomb_blocks %d %d   %1.3f %1.3f \n", t, dr, x, y);


   float d = (float)dr;

   // convert to 0-100 range to cycle block rect
   int bx = (int)(x / 20);
   int by = (int)(y / 20);
   int bd = (int)(dr / 20);  // damage range

   for (int e = (bx-bd); e < (bx+bd)+1; e++)        // cycle blast range of blocks
      for (int f = (by-bd); f < (by+bd)+1; f++)
      {
         if ((e) && (e < 100) && (f) && (f < 100))
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
               if (t == 2) change_block(e, f, 0);
            }
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
//   int sq = mwPS.frame_num % ns;
//   if (sq < ns/2) rad = ms+sq;
//   else rad = ms+ns-sq;

   if (col)
   {
      // circle with a slash
      al_draw_circle(x, y, rad, mC.pc[col], 1);
      float h = sqrt ((rad * rad) / 2) + 0;
      al_draw_line(x-h, y-h, x+h, y+h, mC.pc[col], 1);
//      // circle with crosshairs
//      al_draw_circle(x, y, rad, mC.pc[col], 1);
//      float h = sqrt ((rad * rad) / 2) + 6;
//      al_draw_line(x-h, y-h, x+h, y+h, mC.pc[col], 1);
//      al_draw_line(x-h, y+h, x+h, y-h, mC.pc[col], 1);
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

      if (item[i][8] < 1) item[i][0] = 0; // explosion done, erase item
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
      al_draw_bitmap(mwB.tile[538], x, y, 0);

      // get detonator position relative to player
      int p = item[i][13];
      int xo = players[p].left_right*28-14;
      int px = al_fixtoi(players[p].PX) + xo;
      int py = al_fixtoi(players[p].PY) + -1;

      // draw detonator
      al_draw_bitmap(mwB.tile[539], px, py, 0);

      // draw a line connecting detonator and bomb
      al_draw_line(x+10, y+10, px+10, py+8, mC.pc[207], 0);

      // show damage range circle
      int col = seq_color2();
      if (col) al_draw_circle(x+10, y+10, item[i][7], mC.pc[col], 2);
   }


   if (item[i][6] == 1) // fuse burning
   {
      int num_seq_shapes = mwB.zz[4][fuse_seq] + 1;   // number of shapes in seq
      int si = (int)(r * (float)num_seq_shapes);  // ratio * number of shapes
      int shape = mwB.zz[5+si][fuse_seq];             // get shape to draw
      //printf("ratio:%f shape_index:%d\n", r, si);
      al_draw_bitmap(mwB.tile[shape], x, y, 0);

      // show countdown clock
      float cr = 6; // clock radius
      float pi = ALLEGRO_PI;
      float rd = pi*2*r - pi/2; // countdown position in radians
      al_draw_circle(x+10, y+13, cr, mC.pc[15], 0);
      al_draw_pieslice(x+10, y+13, cr, rd, 0, mC.pc[15], 0);
      al_draw_arc(x+10, y+13, cr, -pi/2, pi*2*r, mC.pc[10], 0);

      // show damage range circle
      int col = seq_color2();
      if (col) al_draw_circle(x+10, y+10, item[i][7], mC.pc[col], 2);

   }

   if (item[i][6] == 2) // explosion mode
   {
      int num_seq_shapes = mwB.zz[4][expl_seq] + 1;   // number of shapes in seq
      int si = (int)(r * (float)num_seq_shapes);  // ratio * number of shapes
      int shape = mwB.zz[5+si][expl_seq];             // get shape to draw

      // set size of explosion
      float max_scale = item[i][7] / 10;         // max scale = damage / 10
      float sc = .5 + (r) * max_scale;
      //printf("ratio:%f shape_index:%d scale:%f \n", r, si, sc);
      al_draw_scaled_rotated_bitmap(mwB.tile[shape], 10, 10, x+10, y+10, sc, sc, 0, 0);

      // show damage range circle
      al_draw_circle(x+10, y+10, (float)item[i][7]*r-8, mC.pc[10], 1);
      al_draw_circle(x+10, y+10, (float)item[i][7]*r, mC.pc[14], 2);
      al_draw_circle(x+10, y+10, (float)item[i][7]*r+8, mC.pc[10], 1);
   }

   bomb_blocks( i, 1, item[i][7], itemf[i][0], itemf[i][1]); // mark blocks that will be destroyed
   bomb_enemies(i, 1, item[i][7], itemf[i][0], itemf[i][1]); // mark enemies
   bomb_players(i, 1, item[i][7], itemf[i][0], itemf[i][1]); // mark players in damage range

   // debug show sequence numbers
   //al_draw_textf(mF.pr8, mC.pc[15], x, y-20, 0, "%d / %d  %f ", item[i][8], item[i][9], (float)item[i][8]/(float)item[i][9]);
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
   //al_draw_textf(mF.pr8, mC.pc[10], al_fixtof(fxi)+30, al_fixtof(fyi)+10, ALLEGRO_ALIGN_CENTER, "%d" ,speed );


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
         if (col) al_draw_circle(fxf, fyf, item[i][7], mC.pc[col], 2); // show damage range circle
         al_draw_filled_circle(fxf, fyf, 5, mC.pc[col]); // show center
         al_draw_line(fxif, fyif, fxf, fyf, mC.pc[10+80], 0); // draw connecting line

         bomb_blocks( i, 1, item[i][7], fx, fy); // mark blocks that will be destroyed
         bomb_enemies(i, 1, item[i][7], fx, fy); // mark enemies that will be destroyed
         bomb_players(i, 1, item[i][7], fx, fy); // mark players in damage range

         j = 1000; // to break out of loop
      }
   }
}


















void draw_item(int i, int custom, int cx, int cy)
{
   int type = item[i][0];
   int x = al_fixtoi(itemf[i][0]);
   int y = al_fixtoi(itemf[i][1]);

   if (custom)
   {
      x = cx;
      y = cy;
   }

   int shape = item[i][1];                       // get shape
   if (shape > 999) shape = mwB.zz[0][shape-1000];   // ans
   int drawn = 0;


   if ((type == 10) && (!custom)) // pop up message
   {
      int timer_count=0, timer_val=0;
      get_int_3216(item[i][12], timer_count, timer_val);

      // if timer running or always show, draw the message
      if ((timer_count) || (item[i][2] & PM_ITEM_PMSG_SHOW_ALWAYS)) draw_pop_message(i, 0, 0, 0, 0, 0, msg);

      // if hide scroll and not running level editor flag scroll as being drawn already
      if ((!(item[i][2] & PM_ITEM_PMSG_SHOW_SCROLL)) && (!mwPS.level_editor_running)) drawn = 1;

   }


   if (type == 1)  { draw_door(i, x, y, custom);         drawn = 1; }
   if (type == 6)  { draw_orb(i, x, y);                  drawn = 1; }
   if (type == 9)  { draw_trigger(i, x, y);              drawn = 1; }
   if (type == 16) { draw_block_manip(i, x, y);          drawn = 1; }
   if (type == 17) { draw_block_damage(i, x, y, custom); drawn = 1; }

   if ((type == 8) && (item[i][11]) ) al_draw_bitmap(mwB.tile[440], x, y, 0); // bomb sticky spikes

   if (type == 99)
   {
      draw_lit_bomb(i);
      if ((item[i][11]) && (item[i][6] != 2)) al_draw_bitmap(mwB.tile[440], x, y, 0);  // bomb sticky spikes
      drawn = 1;
   }

   // moving key in final sequence
   if ((type == 4) && (item[i][11] > 0) && (item[i][11] < 10))
   {
      // moving key in final stage gets static shape not ans
      shape = item[i][1];                           // get shape
      if (shape > 999) shape = mwB.zz[5][shape-1000];   // get first shape only

      // stretch the key
      float sc = 1 + 4*((10 - (float)item[i][11]) / 10);
      float rot = al_fixtof(al_fixmul(al_itofix(item[i][10]/10), al_fixtorad_r));
      al_draw_scaled_rotated_bitmap(mwB.tile[shape],10, 10, x+10, y+10, sc, sc, rot, 0);
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
      al_draw_rectangle(x1+xo, y1+yo, x2-xo, y2-yo, mC.pc[15], 1);
   }


    // these types need rotation
   if ((type == 11) || (type == 98) ||  // rockets
      ((type == 4) && (item[i][11] > 0))) // moving key
   {
      float rot = al_fixtof(al_fixmul(al_itofix(item[i][10]/10), al_fixtorad_r));
      al_draw_rotated_bitmap(mwB.tile[shape], 10, 10, x+10, y+10, rot, 0);
      drawn = 1;
   }

   if (type == 98) draw_rocket_lines(i); // for lit rockets

   if (type == 5) // start
   {
      if (number_of_starts > 1)
      {
         al_draw_bitmap(mwB.tile[shape], x, y, 0);
         al_draw_textf(mF.pixl, mC.pc[12], x+10, y-4, ALLEGRO_ALIGN_CENTER, "%d", item[i][7]);
         drawn = 1;
      }
   }

   if (type == 3) // exit
   {
      al_draw_bitmap(mwB.tile[399], x, y, 0); // 'exit' text not shown
      if (mwPS.frame_num % 60 > 30)
         al_draw_text(mF.pixl, mC.pc[10], x+11, y-2, ALLEGRO_ALIGN_CENTER, "EXIT");

      int exit_enemys_left = num_enemy - item[i][8];
      if (exit_enemys_left > 0) // locked
      {
         al_draw_bitmap(mwB.tile[366], x, y, 0); // show lock
         if (mwPS.frame_num % 60 < 30)
            al_draw_textf(mF.pixl, mC.pc[14], x+11, y-2, ALLEGRO_ALIGN_CENTER, "%d", exit_enemys_left);

      }
      drawn = 1;
   }

   if ((type == 2) && (item[i][6] == 3)) // purple coin custom draw
   {
      if (!mwPS.level_editor_running)
      {
         spin_shape(shape, x, y, 0, 0, 19, 19, 0.8, 0.5, 40);
         drawn = 1;
      }
   }

   // default draw if nothing else has drawn it up to now
   if (!drawn) al_draw_bitmap(mwB.tile[shape], x, y, 0);



   if (!mwPS.level_editor_running)
   {
      // if item is expiring show how many seconds left it has
      if ((item[i][14]>10) && (type != 9) && (type != 16) && (type != 17))
         al_draw_textf(mF.pixl, mC.pc[15], x+10, y-10, ALLEGRO_ALIGN_CENTER, "%d", 1 + (item[i][14] - 10) / 40);
   }
}


void draw_items(void)
{
   al_set_target_bitmap(mwB.level_buffer);

   for (int i=0; i<500; i++)
      if ((item[i][0]) && (item[i][0] != 10)) draw_item(i, 0, 0, 0);

   for (int i=0; i<500; i++) // draw msg last so they are on top...
      if (item[i][0] == 10) draw_item(i, 0, 0, 0);
}


void proc_pmsg_reset_timer(int i)
{
   int timer_count=0, timer_val=0;
   get_int_3216(item[i][12], timer_count, timer_val);
   timer_count = timer_val;
   set_int_3216(item[i][12], timer_count, timer_val);
}

void proc_pmsg(int i)
{
   // count down the timer
   if (!(item[i][2] & PM_ITEM_PMSG_SHOW_ALWAYS))
   {
      int timer_count=0, timer_val=0;
      get_int_3216(item[i][12], timer_count, timer_val);

      if (timer_count > 0) timer_count--;
      set_int_3216(item[i][12], timer_count, timer_val);
   }


   // check for player in trigger box
   if (item[i][2] & PM_ITEM_PMSG_TRIGGER_BOX)
   {
      al_fixed tfx1 = al_itofix(item[i][6]-10);
      al_fixed tfy1 = al_itofix(item[i][7]-10);
      al_fixed tfx2 = tfx1 + al_itofix(item[i][8]);
      al_fixed tfy2 = tfy1 + al_itofix(item[i][9]);

      for (int p=0; p<NUM_PLAYERS; p++)
         if ((players[p].active) && (!players[p].paused))
         {
            al_fixed x = players[p].PX;
            al_fixed y = players[p].PY;
            if ((x > tfx1) && (x < tfx2) && (y > tfy1) && (y < tfy2)) proc_pmsg_reset_timer(i);
         }
   }


}

int is_item_stuck_to_wall(int i)
{
   int x = al_fixtoi(itemf[i][0]);
   int y = al_fixtoi(itemf[i][1]);
   if ( (is_left_solid(x,y, 0, 3)) || (is_right_solid(x,y, 0, 3)) || (is_down_solid(x,y, 0, 3)) || (is_up_solid(x,y, 0, 3)) ) return 1;
   return 0;
}


void proc_switch_block_range(int i, int action)
{
   int x1 = item[i][6] / 20;
   int y1 = item[i][7] / 20;
   int x2 = (item[i][6] + item[i][8]) / 20;
   int y2 = (item[i][7] + item[i][9]) / 20;


   if (action == 1)
   {
      al_set_target_bitmap(mwB.level_background);
      for (int x=x1; x<x2; x++)
         for (int y=y1; y<y2; y++)
         {
            int tn = l[x][y]&1023;
            if ((item[i][10] & 0b0001) && (tn == 172)) change_block(x, y, 7);  // green  solid to empty
            if ((item[i][10] & 0b0010) && (tn == 173)) change_block(x, y, 8);  // red    solid to empty
            if ((item[i][10] & 0b0100) && (tn == 174)) change_block(x, y, 9);  // blue   solid to empty
            if ((item[i][10] & 0b1000) && (tn == 175)) change_block(x, y, 10); // purple solid to empty
            if ((item[i][10] & 0b0001) && (tn ==  7))  change_block(x, y, (172 | PM_BTILE_ALL_SOLID)); // green  empty to solid
            if ((item[i][10] & 0b0010) && (tn ==  8))  change_block(x, y, (173 | PM_BTILE_ALL_SOLID)); // red    empty to solid
            if ((item[i][10] & 0b0100) && (tn ==  9))  change_block(x, y, (174 | PM_BTILE_ALL_SOLID)); // blue   empty to solid
            if ((item[i][10] & 0b1000) && (tn == 10))  change_block(x, y, (175 | PM_BTILE_ALL_SOLID)); // purple empty to solid
         }
      al_set_target_backbuffer(display);
   }

   // this method sets block then calls init_level..
//   if (action == 1)
//   {
//      for (int x=x1; x<x2; x++)
//         for (int y=y1; y<y2; y++)
//         {
//            int tn = l[x][y]&1023;
//
//            if ((item[i][10] & 0b0001) && (tn == 172)) l[x][y] = 7;  // green  solid to empty
//            if ((item[i][10] & 0b0010) && (tn == 173)) l[x][y] = 8;  // red    solid to empty
//            if ((item[i][10] & 0b0100) && (tn == 174)) l[x][y] = 9;  // blue   solid to empty
//            if ((item[i][10] & 0b1000) && (tn == 175)) l[x][y] = 10; // purple solid to empty
//            if ((item[i][10] & 0b0001) && (tn ==  7))  l[x][y] = (172 | PM_BTILE_ALL_SOLID); // green  empty to solid
//            if ((item[i][10] & 0b0010) && (tn ==  8))  l[x][y] = (173 | PM_BTILE_ALL_SOLID); // red    empty to solid
//            if ((item[i][10] & 0b0100) && (tn ==  9))  l[x][y] = (174 | PM_BTILE_ALL_SOLID); // blue   empty to solid
//            if ((item[i][10] & 0b1000) && (tn == 10))  l[x][y] = (175 | PM_BTILE_ALL_SOLID); // purple empty to solid
//         }
//      if (action == 1) init_level_background(0);
//   }


   if (action == 2)
   {
      for (int x=x1; x<x2; x++)
         for (int y=y1; y<y2; y++)
         {
            int tn = l[x][y]&1023;
            if ((item[i][10] & 0b0001) && ( (tn == 7)  || (tn == 172)) ) bomb_block_crosshairs(x, y);
            if ((item[i][10] & 0b0010) && ( (tn == 8)  || (tn == 173)) ) bomb_block_crosshairs(x, y);
            if ((item[i][10] & 0b0100) && ( (tn == 9)  || (tn == 174)) ) bomb_block_crosshairs(x, y);
            if ((item[i][10] & 0b1000) && ( (tn == 10) || (tn == 175)) ) bomb_block_crosshairs(x, y);
         }
   }
}

void proc_key_block_range(int i, int action)
{
   int x1 = item[i][6] / 20;
   int y1 = item[i][7] / 20;
   int x2 = (item[i][6] + item[i][8]) / 20;
   int y2 = (item[i][7] + item[i][9]) / 20;

   for (int x=x1; x<x2; x++)
      for (int y=y1; y<y2; y++)
      {
         if (item[i][12]) // matching keyed blocks only
         {
            int key = item[i][1] - 1039;
            if (((l[x][y]&1023) == 188 + key) || ((l[x][y]&1023) == 204 + key) || ((l[x][y]&1023) == 220 + key))
            {
               if (action == 1) change_block(x, y, 0);
               if (action == 2) bomb_block_crosshairs(x, y);
            }
         }
         else // all blocks in range
         {
            if (action == 1) change_block(x, y, 0);
            if (action == 2) bomb_block_crosshairs(x, y);
         }
      }
}



void proc_moving_key(int i)
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
      item[i][0] = 0; // remove the key
      proc_key_block_range(i, 1);
    }
}

void move_items()
{
   for (int i=0; i<500; i++)
      if (item[i][0])
      {
         int x = al_fixtoi(itemf[i][0]);
         int y = al_fixtoi(itemf[i][1]);
         if ((x<0) || (x>1980) || (y<0) || (y>1980)) item[i][0] = 0; // remove if out of bounds

         int type = item[i][0];
         if ((type == 4) && (item[i][11] > 0)) proc_moving_key(i);
         if (type == 6) proc_orb(i);
         if (type == 9) proc_trigger(i);
         if (type == 10) proc_pmsg(i);
         if (type == 16) proc_block_manip(i);
         if (type == 17) proc_block_damage(i);
         if (type == 99) proc_lit_bomb(i);
         if (type == 98) proc_lit_rocket(i);


         // check for time to live
         if ((type != 9) && (type != 16) && (type != 17))
         {
            int ttl = item[i][14];
            if (ttl)
            {
               if (ttl < 11)       // start ans seq
               {
                  item[i][0] = 66; // change to different type to prevent use
                  int sq = 10-ttl;
                  item[i][1] = mwB.zz[5+sq][74];
               }
               if (ttl == 1) item[i][0] = 0; // kill instantly
               item[i][14]--;
            }
         }

         // not stationary and not lit rocket, trigger, bm, bd or moving key
         if ((item[i][3]) && (type != 98) && (type != 9) && (type != 16) && (type != 17) && (! ((type == 4) && (item[i][11] > 0)))    )
         {
            // check if being carried
            int pc = 0;
            for (int p=0; p<NUM_PLAYERS; p++)
               if (players[p].active)
                  if ((!players[p].paused) || (players[p].paused && players[p].paused_type == 2))
                     if (i == (players[p].carry_item-1)) pc = 1;

            if (!pc) // not being carried
            {
               // check for sticky bomb stuck to wall
               int sticky = 0;
               if ((type == 99) && (item[i][11])) sticky = 1;
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

                  // slow down xinc (friction)
                  if (itemf[i][2] > al_itofix(0))
                  {
                      itemf[i][2] -= al_ftofix(.01);     // slow down +xinc
                         if (itemf[i][2] < al_itofix(0)) // set to zero if crosses zero
                            itemf[i][2] = al_itofix(0);
                  }

                  if (itemf[i][2] < al_itofix(0))
                  {
                      itemf[i][2] += al_ftofix(.01);     // slow down - xinc
                         if (itemf[i][2] > al_itofix(0)) // set to zero if crosses zero
                            itemf[i][2] = al_itofix(0);
                  }

                  x = al_fixtoi(itemf[i][0]);
                  y = al_fixtoi(itemf[i][1]);


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

                        if (! ((type == 99) && (item[i][6] == 2))) // no gravity for exploding bomb
                           itemf[i][3] += al_ftofix(.1);           // apply gravity to yinc

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


         if (!item[i][0]) players[p].carry_item = 0; // player was carrying item that had become inactive


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


/*

item[][0] = 6 - orb
item[][1] = bitmap
item[][2] = flags
item[][3] = stat | fall | carry
item[][4] = x pos (2000)
item[][5] = y pos (2000)
item[][6]  = MODE
item[][7]  = timer val
item[][8]  = time count
item[][9]  =
item[][10] = STATE ON  pm_event
item[][11] = SATEE OFF pm_event
item[][12] = TGON pm_event
item[][13] = TGOF pm_event

#define PM_ITEM_ORB_STATE       0b0000000000000001
#define PM_ITEM_ORB_PREV_STATE  0b0000000000000010
#define PM_ITEM_ORB_TGON        0b0000000000000100
#define PM_ITEM_ORB_TGOF        0b0000000000001000

#define PM_ITEM_ORB_TRIG_TOUCH  0b0000000000010000
#define PM_ITEM_ORB_TRIG_UP     0b0000000000100000
#define PM_ITEM_ORB_TRIG_DOWN   0b0000000001000000
#define PM_ITEM_ORB_TRIG_CURR   0b0000000010000000
#define PM_ITEM_ORB_TRIG_PREV   0b0000000100000000

*/

int proc_orb_shot_collision(int i)
{
   int s = 8; // collison box size
   float x = al_fixtof(itemf[i][0]);
   float y = al_fixtof(itemf[i][1]);
   for (int b=0; b<50; b++)
      if (mwS.p[b].active)
      {
         float bx = mwS.p[b].x;
         float by = mwS.p[b].y;
         if ((x > bx-s) && (x < bx+s) && (y > by-s) && (y < by+s)) return 1;
      }
   return 0;
}


void proc_orb(int i)
{
   int MODE = item[i][6];

   // timer mode stuff
   if (MODE == 3) // timed ON
   {
      if (--item[i][8] <= 0)
      {
         item[i][8] = 0;
         item[i][2] &= ~PM_ITEM_ORB_STATE;  // OFF
      }
      else item[i][2] |= PM_ITEM_ORB_STATE; // ON
   }
   if (MODE == 4) // timed OFF
   {
      if (--item[i][8] <= 0)
      {
         item[i][8] = 0;
         item[i][2] |= PM_ITEM_ORB_STATE;    // ON
      }
      else item[i][2] &= ~PM_ITEM_ORB_STATE; // OFF
   }


   if ((item[i][2] & PM_ITEM_ORB_TRIG_SHOT) && (proc_orb_shot_collision(i))) item[i][2] |= PM_ITEM_ORB_TRIG_CURR; // set CURR flag



   // trigger stuff
   if (item[i][2] & PM_ITEM_ORB_TRIG_CURR)                     // currently triggered
   {
      if ((MODE == 3) || (MODE == 4)) item[i][8] = item[i][7]+1; // reset counter
      if (!(item[i][2] & PM_ITEM_ORB_TRIG_PREV))                 // not triggered last time
      {
         item[i][2] |= PM_ITEM_ORB_TRIG_PREV;                  // set PREV flag
         if (MODE == 0) item[i][2] ^= PM_ITEM_ORB_STATE;       // toggle state
         if (MODE == 1) item[i][2] |= PM_ITEM_ORB_STATE;       // stick ON
         if (MODE == 2) item[i][2] &= ~PM_ITEM_ORB_STATE;      // stick OFF
      }
   }
   else item[i][2] &= ~PM_ITEM_ORB_TRIG_PREV;                  // clear PREV flag
   item[i][2] &= ~PM_ITEM_ORB_TRIG_CURR;                       // clear CURR flag



   // STATE stuff
   item[i][2] &= ~PM_ITEM_ORB_TGON;               // clear TGON flag
   item[i][2] &= ~PM_ITEM_ORB_TGOF;               // clear TGOF flag
   if (item[i][2] & PM_ITEM_ORB_STATE)            // orb state ON
   {
      item[i][1] = 419;                           // red orb
      if (!(item[i][2] & PM_ITEM_ORB_PREV_STATE)) // prev OFF
      {
         item[i][2] |= PM_ITEM_ORB_TGON;          // set TGON flag
         item[i][2] |= PM_ITEM_ORB_PREV_STATE;    // set PREV flag
      }
   }
   else                                           // orb state OFF
   {
      item[i][1] = 418;                           // green orb
      if ((item[i][2] & PM_ITEM_ORB_PREV_STATE))  // prev ON
      {
         item[i][2] |=  PM_ITEM_ORB_TGOF;         // set TGOF flag
         item[i][2] &= ~PM_ITEM_ORB_PREV_STATE;   // clear PREV flag
      }
   }

   // clear all events
   mwPME.event[item[i][10]] = 0;
   mwPME.event[item[i][11]] = 0;
   mwPME.event[item[i][12]] = 0;
   mwPME.event[item[i][13]] = 0;
   int FLAGS = item[i][2];
   if   (FLAGS & PM_ITEM_ORB_STATE)  mwPME.event[item[i][10]] = 1;
   if (!(FLAGS & PM_ITEM_ORB_STATE)) mwPME.event[item[i][11]] = 1;
   if   (FLAGS & PM_ITEM_ORB_TGON)   mwPME.event[item[i][12]] = 1;
   if   (FLAGS & PM_ITEM_ORB_TGOF)   mwPME.event[item[i][13]] = 1;
}

void draw_orb(int i, int x, int y)
{
   item[i][1] = 418;                                          // green orb
   if (item[i][2] & PM_ITEM_ORB_STATE) item[i][1] = 419;      // red orb

   // rotation
   int rb = (item[i][2] & PM_ITEM_ORB_ROTB) >> 14;
   float a=0, xo=0, yo=0; // angle, x and y offsets
   if (rb == 0) { a = 0;             xo = 10, yo = 9; } // floor
   if (rb == 1) { a = ALLEGRO_PI/2;  xo = 7,  yo = 7; } // wall left
   if (rb == 2) { a = ALLEGRO_PI;    xo = 9,  yo = 4; } // ceiling
   if (rb == 3) { a = -ALLEGRO_PI/2; xo = 12, yo = 6; } // wall right

   int MODE = item[i][6];
   int drawn = 0;
   if ((MODE == 3) || (MODE == 4))
   {
      int c1=11, c2=10;
      if (MODE == 4) {c1=10; c2=11;}
      int percent =  ((item[i][8]-1) * 100) / item[i][7];
      if (percent > 0)
      {
         draw_percent_barc(x+xo, y+yo, 7, 7,  percent, c1, c2, -1);
         al_draw_rotated_bitmap(mwB.tile[417], 10, 10, x+10, y+10, a, 0);
         drawn = 1;
      }
   }
   if (!drawn) al_draw_rotated_bitmap(mwB.tile[item[i][1]], 10, 10, x+10, y+10, a, 0);
}
void proc_orb_collision(int p, int i)
{
   if (  (item[i][2] & PM_ITEM_ORB_TRIG_TOUCH) ||
        ((item[i][2] & PM_ITEM_ORB_TRIG_UP)   && (players[p].up)) ||
        ((item[i][2] & PM_ITEM_ORB_TRIG_DOWN) && (players[p].down)) )
           item[i][2] |= PM_ITEM_ORB_TRIG_CURR;
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
               if (item[i][10] && item[i][10] < mwPS.frame_num-1) item[i][10] = 0;

               // up pressed and !pressed last frame
               if ((players[p].up) && (!item[i][10])) do_entry = 1;

               if (players[p].up) item[i][10] = mwPS.frame_num;
               else item[i][10] = 0;

            }

            if (item[i][11] == 2) // enter with <down>
            {
               // to prevent immediate triggering when destination door, wait for release and re-press

               // if key held is old, ignore
               if (item[i][10] && item[i][10] < mwPS.frame_num-1) item[i][10] = 0;

               // down pressed and !pressed last frame
               if ((players[p].down) && (!item[i][10])) do_entry = 1;

               if (players[p].down) item[i][10] = mwPS.frame_num;
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
               if (item[i][13]) game_event(5, 0, 0, p, i, 0, 0); // no event if door is inivisible

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

                  item[li][10] = mwPS.frame_num;
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
   int ns = 0; // count number of starts
   int s[8] = {0};
   for (int i=0; i<500; i++)
      if (item[i][0] == 5)
      {
         ns++;
         s[item[i][7]] = i; // save index of this start
      }
/*
   if (ns == 0)
   {
      printf("Error: no start found.\n");
      players[p].PX = al_itofix(20);
      players[p].PY = al_itofix(20);;
   }

   if (ns == 1)
   {
      players[p].spawn_point_index = 0;
      int ps = s[players[p].spawn_point_index];
      players[p].PX = itemf[ps][0];
      players[p].PY = itemf[ps][1];
   }
*/


   if (ns > 1)
   {
      int mode = item[s[0]][6];

      if (mode == 2) // check point common
      {
         for (p=0; p<8; p++)
            players[p].spawn_point_index = item[i][7]; // set new spawn point for all players

         // mark this one as active and all others as not
         for (int ii=0; ii<500; ii++)
            if (item[ii][0] == 5)
            {
               if (item[ii][7] == item[i][7]) item[ii][1] = 1021;
               else item[ii][1] = 1011;
            }
      }

      if (mode == 3) // check point individual
      {
         players[p].spawn_point_index = item[i][7]; // set new spawn point for this player
      }

   }

   /*
   players[p].spawn_point_index = item[i][7]; // set new spawn point

   // mark this one as active and all others as not
   for (int ii=0; ii<500; ii++)
      if (item[ii][0] == 5)
      {
         if (item[ii][7] == item[i][7]) item[ii][1] = 1021;
         else item[ii][1] = 1011;
      }

*/


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
      game_event(70, 0, 0, p, i, 0, 0);
   }

   if (bonus_type == 3) // purple coin!!!
   {
      item[i][0] = 0;
      players[p].stat_purple_coins++;
      game_event(71, 0, 0, p, i, 0, 0);
   }
}

void proc_exit_collision(int p, int i)
{
   int exit_enemys_left = num_enemy - item[i][8];
   if (exit_enemys_left <= 0)
   {
      if (players[0].level_done_mode == 0)
      {
         players[0].level_done_mode = 9;
         players[0].level_done_timer = 0;
         players[0].level_done_x = al_fixtoi(itemf[i][0]);
         players[0].level_done_y = al_fixtoi(itemf[i][1]);
         players[0].level_done_player = p;
         players[0].level_done_next_level = play_level + 1;
         game_event(4, 0, 0, 0, 0, 0, 0);
      }
   }
   else game_event(3, 0, 0, p, i, exit_enemys_left, 0); // not enough dead yet
}

void proc_key_collision(int p, int i)
{
   if (item[i][11] == 0) // only collide if not already moving
   {
      int x2 = (item[i][6] + item[i][8] / 2);         // get the center of the block range
      int y2 = (item[i][7] + item[i][9] / 2);
      al_fixed xlen = al_itofix(x2) - itemf[i][0];     // distance between block range and key
      al_fixed ylen = al_itofix(y2) - itemf[i][1];
      al_fixed hy_dist =  al_fixhypot(xlen, ylen);     // hypotenuse distance

   //   printf("hy_dist:%d\n", al_fixtoi(hy_dist));

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
   if (players[0].level_done_mode == 0)
   {
      if (play_level > 1) warp_level_location = play_level;
//      players[0].level_done_mode = 3;
//      players[0].level_done_timer = 0;

      players[0].level_done_mode = 2;
      players[0].level_done_timer = 0;


      players[0].level_done_x = al_fixtoi(itemf[i][0]);
      players[0].level_done_y = al_fixtoi(itemf[i][1]);
      players[0].level_done_player = p;
      players[0].level_done_next_level = item[i][8];
      game_event(4, 0, 0, p, i, 0, 0);
   }
}

void proc_switch_collision(int p, int i)
{
   if (item[i][11] < mwPS.frame_num) // if not lockout
   {
      // if falling and landing on it
      if ( (players[p].PX  > itemf[i][0] - al_itofix(12)) && (players[p].PX  < itemf[i][0] + al_itofix(12)) &&
           (players[p].PY  > itemf[i][1] - al_itofix(16)) && (players[p].PY  < itemf[i][1] - al_itofix(8)) &&
           (players[p].yinc > al_itofix(0)) )  // falling
      {
         item[i][11] = mwPS.frame_num + 4; // switch lockout for next 4 frames
         game_event(30, 0, 0, p, i, 0, 0);

         if (item[i][1] > 111) item[i][1] -= 16;
         else item[i][1] += 16;

         al_set_target_bitmap(mwB.level_background);
         proc_switch_block_range(i, 1);
      }
   }
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
      if ((item[players[p].carry_item][0] == 2) && (item[i][0] != 2)) // carried item is bonus and new item is not bonus
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
      case 6:  proc_orb_collision(p, i);      break;
      case 7:  proc_mine_collision(p, i);     break;
      case 8:  proc_bomb_collision(p, i);     break;
      case 10: proc_pmsg_reset_timer(i);      break;
      case 11: proc_rocket_collision(p, i);   break;
      case 12: proc_warp_collision(p, i);     break;
      case 14: proc_switch_collision(p, i);   break;
      case 15: proc_sproingy_collision(p, i); break;
   }
}



void proc_lit_rocket(int i)
{
   lit_item = 1;

   int max_speed = item[i][8]*1000;
   int accel = item[i][9];
   if (item[i][11] < max_speed) item[i][11]+=accel;   // add accel to speed

   al_fixed angle = al_itofix((item[i][10]-640) / 10);
   itemf[i][2] = (al_fixcos(angle) * item[i][11]) / 1000;
   itemf[i][3] = (al_fixsin(angle) * item[i][11]) / 1000;

   int x = al_fixtoi(itemf[i][0] += itemf[i][2]); // apply the increments
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

      item[i][3] = 1;   // not carryable (fall)

      item[i][8] = 20;   // explosion timer count
      item[i][9] = 20;   // explosion timer limit


      // if any players are riding this rocket, make them drop it
      for (int p=0; p<NUM_PLAYERS; p++)
         if ( (players[p].active) && (!players[p].paused) && (players[p].carry_item) && (players[p].carry_item == i+1 )) players[p].carry_item = 0;

   }
   else
   {
      // if any players are riding this rocket, bind them to rocket's position
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
#define PM_ITEM_TRIGGER_PSHOT     0b0000000000001000
#define PM_ITEM_TRIGGER_ESHOT     0b0000000000010000
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

void proc_trigger(int i)
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

/*   if (FLAGS & PM_ITEM_TRIGGER_CURR) printf("%d - CURR\n", mwPS.frame_num);
   if (FLAGS & PM_ITEM_TRIGGER_PREV) printf("%d - PREV\n", mwPS.frame_num);
   if (FLAGS & PM_ITEM_TRIGGER_TGON) printf("%d - TGON\n", mwPS.frame_num);
   if (FLAGS & PM_ITEM_TRIGGER_TGOF) printf("%d - TGOF\n", mwPS.frame_num); */

   // clear them all
   mwPME.event[item[i][11]] = 0;
   mwPME.event[item[i][12]] = 0;
   mwPME.event[item[i][13]] = 0;
   mwPME.event[item[i][14]] = 0;

   if   (FLAGS & PM_ITEM_TRIGGER_CURR)  mwPME.event[item[i][11]] = 1;
   if (!(FLAGS & PM_ITEM_TRIGGER_CURR)) mwPME.event[item[i][12]] = 1;
   if   (FLAGS & PM_ITEM_TRIGGER_TGON)  mwPME.event[item[i][13]] = 1;
   if   (FLAGS & PM_ITEM_TRIGGER_TGOF)  mwPME.event[item[i][14]] = 1;
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
   if (FLAGS & PM_ITEM_TRIGGER_PSHOT) // check player shots
      for (int b=0; b<50; b++)
         if (mwS.p[b].active)
         {
            al_fixed x = al_ftofix(mwS.p[b].x);
            al_fixed y = al_ftofix(mwS.p[b].y);
            if ((x > tfx1) && (x < tfx2) && (y > tfy1) && (y < tfy2)) item[i][3] |= PM_ITEM_TRIGGER_CURR;
         }
   if (FLAGS & PM_ITEM_TRIGGER_ESHOT) // check enemy shots
      for (int b=0; b<50; b++)
         if (mwS.e[b].active)
         {
            al_fixed x = al_ftofix(mwS.e[b].x);
            al_fixed y = al_ftofix(mwS.e[b].y);
            if ((x > tfx1) && (x < tfx2) && (y > tfy1) && (y < tfy2)) item[i][3] |= PM_ITEM_TRIGGER_CURR;
         }
}





void draw_trigger(int i, int x, int y)
{
   if (mwPS.level_editor_running)
   {
      al_draw_bitmap(mwB.tile[991], x, y, 0); // draw item shape in level editor, invisible when game running
      if (item[i][3] & PM_ITEM_TRIGGER_LIFT_ON) set_item_trigger_location_from_lift(i, 1); // snap to lift here because main function wont be called while in level editor
   }

   if (item[i][3] & PM_ITEM_TRIGGER_DRAW_ON)
   {
      int col = item[i][2];
      float x1 = item[i][6];
      float y1 = item[i][7];
      float x2 = x1 + item[i][8];
      float y2 = y1 + item[i][9];
      rectangle_with_diagonal_lines(x1, y1, x2, y2, 10, col, col+96, 0);
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
void proc_block_manip(int i)
{
   int et = item[i][1]; // pm_event trigger we are looking for
   if (mwPME.event[et])
   {
      al_set_target_bitmap(mwB.level_background);
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
               al_draw_filled_rectangle(x*20, y*20, x*20+20, y*20+20, mC.pc[0]);
               al_draw_bitmap(mwB.btile[block1&1023], x*20, y*20, 0 );
            }

            if (mode == 2) // set all block2 to block 1
            {
               if (l[x][y] == block2)
               {
                  l[x][y] = block1;
                  al_draw_filled_rectangle(x*20, y*20, x*20+20, y*20+20, mC.pc[0]);
                  al_draw_bitmap(mwB.btile[block1&1023], x*20, y*20, 0 );
               }
            }

            if (mode == 3) // toggle block1 and block 2
            {
               if (l[x][y] == block1)
               {
                  l[x][y] = block2;
                  al_draw_filled_rectangle(x*20, y*20, x*20+20, y*20+20, mC.pc[0]);
                  al_draw_bitmap(mwB.btile[block2&1023], x*20, y*20, 0 );
               }
               else if (l[x][y] == block2)
               {
                  l[x][y] = block1;
                  al_draw_filled_rectangle(x*20, y*20, x*20+20, y*20+20, mC.pc[0]);
                  al_draw_bitmap(mwB.btile[block1&1023], x*20, y*20, 0 );
               }
            }
         }
   }
}



void draw_block_manip(int i, int x, int y)
{
   if (mwPS.level_editor_running)
   {
      al_draw_bitmap(mwB.tile[989], x, y, 0); // draw item shape in level editor, invisible when game running
   }
   if (item[i][2]) // draw mode on
   {
      int col = item[i][12];
      float x1 = item[i][6];
      float y1 = item[i][7];
      float x2 = x1 + item[i][8];
      float y2 = y1 + item[i][9];
      rectangle_with_diagonal_lines(x1, y1, x2, y2, 10, col, col+96, 0);
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
#define PM_ITEM_DAMAGE_PSHOT     0b00000000000001000
#define PM_ITEM_DAMAGE_ESHOT     0b00000000000010000
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
   int cdpb = ((cd) && (FLAGS & PM_ITEM_DAMAGE_PSHOT));   // damage active and player shot flag
   int cdeb = ((cd) && (FLAGS & PM_ITEM_DAMAGE_ESHOT));   // damage active and enemy shot flag

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
                  //game_event(59, 0, 0, p, i, 0, 0);
               }
               else
               {
                  if (item[i][15] > 0) // lose health
                  {
                     players[p].LIFE -= al_fixdiv(al_itofix(item[i][15]), al_itofix(100));
                     game_event(59, 0, 0, p, i, 0, 0); // only do damage noise when taking health..??
                  }
                  else // gain health
                  {
                     al_fixed f100 = al_itofix(100);
                     if (players[p].LIFE < f100)
                     {
                        players[p].LIFE -= al_fixdiv(al_itofix(item[i][15]), al_itofix(100));
                        if (players[p].LIFE > f100) players[p].LIFE = f100;
                     }
                  }
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
               Ei[e2][31] = 3;           // flag that this enemy got shot
               //Ei[e2][26] = x;           // number of player's shot that hit enemy
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
               // orb, trig, bm, bd -- kill these immed
               if ((item[i][0] == 6) || (item[i][0] == 9) || (item[i][0] == 16) || (item[i][0] == 17)) item[i][0] = 0;

               if ((item[i][0] != 66) && (item[i][0] != 5)) // never kill start

               {
                  //item[i][0] = 66;
                  item[i][14] = 10;
               }
            }
         }
   if (cdpb) // check player shots
      for (int b=0; b<50; b++)
         if (mwS.p[b].active)
         {
            al_fixed x = al_ftofix(mwS.p[b].x);
            al_fixed y = al_ftofix(mwS.p[b].y);
            if ((x > tfx1) && (x < tfx2) && (y > tfy1) && (y < tfy2)) mwS.p[b].active = 0; // kill the shot
         }
   if (cdeb) // check enemy shots
      for (int b=0; b<50; b++)
         if (mwS.e[b].active)
         {
            al_fixed x = al_ftofix(mwS.e[b].x);
            al_fixed y = al_ftofix(mwS.e[b].y);
            if ((x > tfx1) && (x < tfx2) && (y > tfy1) && (y < tfy2)) mwS.e[b].active = 0; // kill the shot
         }
}





void draw_block_damage(int i, int x, int y, int custom)
{
   int draw_mode = item[i][2];
   int mode = item[i][11];
   int FLAGS = item[i][3];
   float x0 = x;
   float y0 = y;
   float x1 = item[i][6];
   float y1 = item[i][7];
   float x2 = x1 + item[i][8];
   float y2 = y1 + item[i][9];

   if (mwPS.level_editor_running)
   {
      al_draw_bitmap(mwB.tile[988], x0, y0, 0); // draw item shape in level editor, invisible when game running
      if (FLAGS & PM_ITEM_DAMAGE_LIFT_ON) set_item_damage_location_from_lift(i, 1); // set this here only when level editor is running
   }

   // damage field drawing
   if (!custom)
   {

      if (draw_mode == 1) // basic
      {
         int col = 11;
         if (FLAGS & PM_ITEM_DAMAGE_CURR) col = 10;
         rectangle_with_diagonal_lines(x1, y1, x2, y2, 10, col, col+96, 0);
      }

      if (draw_mode == 2) // spikey floor
      {
         int tn = 808;
         if (FLAGS & PM_ITEM_DAMAGE_CURR) tn = 807;
         for (int hx = x1; hx<x2; hx+=20)
            al_draw_bitmap(mwB.tile[tn], hx, y2-20, 0); // draw spikes only on bottom row
      }
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
         if (timer_draw_mode1) al_draw_textf(mF.pixl,   mC.pc[col], x0+10, y0+4, ALLEGRO_ALIGN_CENTER, "%d", tts);
         if (timer_draw_mode2) al_draw_textf(mF.pr8, mC.pc[col], x0+10, y0+6, ALLEGRO_ALIGN_CENTER, "%d", tts);

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


      if (timer_draw_mode1) al_draw_textf(mF.pixl,   mC.pc[col], x0+10, y0+4, ALLEGRO_ALIGN_CENTER, "%d", tts);
      if (timer_draw_mode2) al_draw_textf(mF.pr8, mC.pc[col], x0+10, y0+6, ALLEGRO_ALIGN_CENTER, "%d", tts);
      if (timer_draw_mode3) draw_percent_bar(x0+9, y0+5, 32, 8,  percent);
      if (timer_draw_mode4) draw_percent_bar(x0+9, y0+1, 64, 16, percent);
   }
}




void proc_block_damage(int i)
{
   int et = item[i][1];      // number of pm_event trigger we are looking for
   int FLAGS = item[i][3];
   int mode = item[i][11];


   if (FLAGS & PM_ITEM_DAMAGE_LIFT_ON) set_item_damage_location_from_lift(i, 0); // follow lift location

   proc_item_damage_collisions(i);

   if (mode == 0) item[i][3] |= PM_ITEM_DAMAGE_CURR; // in mode 0, always set damage flag

   if (mode == 1)
   {
      if (mwPME.event[et]) item[i][3] ^= PM_ITEM_DAMAGE_CURR; // toggle current damage flag
   }
   if (mode == 2) // damage unless timer running  (no damage when triggered)
   {
      if (mwPME.event[et]) item[i][13] = item[i][12];              // reset timer
      if (item[i][13] == 0) item[i][3] |=  PM_ITEM_DAMAGE_CURR; // set damage on
      else                  item[i][3] &= ~PM_ITEM_DAMAGE_CURR; // set damage off
   }
   if (mode == 3) // damage when timer is running (no damage until triggered)
   {
      if (mwPME.event[et]) item[i][13] = item[i][12];             // reset timer
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
[6]  - orb
[7]  - mine
[8]  - bomb
[9]  - trigger
[10] - pop-up msg
[11] - rocket
[12] - warp
[14] - switch
[15] - sproingy
[16] - block manip
[17] - block damage

[98] - lit rocket
[99] - lit bomb

// common
item[][0] = active and type
item[][1] = bitmap or ans
item[][2] = draw type (not used)
item[][3] = (0=stat, 1=fall, -1=carry, -2=carry through door)
item[][4] = x pos
item[][5] = y pos

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

[3] - exit
item[][8] exit with x enemies left

[4] - key
item[][6]  block range x
item[][7]  block range y
item[][8]  block range w
item[][9]  block range h
item[][10] rotation for key move
item[][11] counter for key move
item[][12] matching keyed blocks only

[5] - start
item[][6] mode
item[][7] start index

[7] - mine
item[][8] mine damage

[8] - bomb
item[][6]  mode (2=explosion)
item[][7]  blast size
item[][8]  fuse length counter
item[][9]  fuse length value
item[][11]  sticky
item[][12]  timer / remote



[9]  - Trigger
item[][2] = draw_type (color)
item[][3] = flags
item[][6]  = trigger field x (2000)
item[][7]  = trigger field y (2000)
item[][8]  = trigger field w (2000)
item[][9]  = trigger field x (2000)
item[][10] = trigger field lift number
item[][11] = CURR ON  pm_event
item[][12] = CURR OFF pm_event
item[][13] = TGON pm_event #
item[][14] = TGOF pm_event #



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
item[][6]  block range x
item[][7]  block range y
item[][8]  block range w
item[][9]  block range h
item[][10] color flags
item[][11]  lockout

[15] - sproingy
item[][6] jump length (only used in level editor)
item[][7] sproinginess


[16] - Block Manip
item[][1] = pm_event_trigger
item[][2] = draw on
item[][3] = mode
item[][6]  = trigger field x (2000)
item[][7]  = trigger field y (2000)
item[][8]  = trigger field w (2000)
item[][9]  = trigger field x (2000)
item[][10] block 1
item[][11] block 2
item[][12] = draw color

[17] - Block Damage
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
