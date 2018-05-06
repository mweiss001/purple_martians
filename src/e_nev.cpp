// e_nev.cpp (20100220 cleanup)
#include "pm.h"


extern int get100_x, get100_y;


void erase_item(int num)
{
   if (item[num][0] == 10) free (pmsg[num]);
   for (int x=0; x<16; x++) item[num][x] = 0;
}

int get_empty_item(void) // just find me an empty
{
   int mt = -1;
   for (int c=0; c<500; c++)
      if (item[c][0] == 0)
      {
         mt = c;
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
         if (item[mt][0] == 10) pmsg[mt] = (char*) malloc(2); // to prevent empty message crashes
         item_sort();
         mt = 0;
         while ((mt < 500) && (item[mt][9] != 9999)) mt++;
         item[mt][9] = 0; // remove mark
   }
   return mt;
}

int get_empty_enemy(void)
{
   int en = -1;
   for (int d=0; d<100; d++)
      if (Ei[d][0] == 0)
      {
         en = d;
         break;
      }
   if (en == -1) al_show_native_message_box(display, "Error", "Error creating enemy", "Enemy list full!", NULL, ALLEGRO_MESSAGEBOX_ERROR);
   return en;
}



int get_empty_enemy(int type)
{
   extern int e_num_of_type[50];   // sort stuff used by others
   extern int e_first_num[50];
   int en=0, d;

   // cycle backwards to find first empty enemy
   for (d=99; d>-1; d--)
      if (Ei[d][0] == 0) en = d;

   Ei[en][0] = type;
   sort_enemy();
   en = e_first_num[type]+e_num_of_type[type]-1;
   return en;
}


int move_trigger_box(int num, int type)
{
   if (getbox((char *)"Trigger Box", 3, type, num) == 1)
      {
          Ei[num][11] = bx1;
          Ei[num][12] = by1;
          Ei[num][13] = bx2-1;
          Ei[num][14] = by2-1;
          return 1;
      }
   else return 0;
}

void recalc_pod(int EN)
{
   al_fixed xlen = Efi[EN][5] - Efi[EN][0];      // get the x distance
   al_fixed ylen = Efi[EN][6] - Efi[EN][1];      // get the y distance
   al_fixed hy_dist =  al_fixhypot(xlen, ylen);     // hypotenuse distance
   al_fixed speed = Efi[EN][9];                  // speed
   al_fixed scaler = al_fixdiv(hy_dist, speed);     // get scaler
   al_fixed xinc = al_fixdiv(xlen, scaler);         // calc xinc
   al_fixed yinc = al_fixdiv(ylen, scaler);         // calc yinc
   Efi[EN][2] = xinc;
   Efi[EN][3] = yinc;
   Efi[EN][14] = al_fixatan2(ylen, xlen) - al_itofix(64);  // rotation

   // set number of steps
   al_fixed ns;
   if (abs(xlen) > abs(ylen)) ns = al_fixdiv(xlen, xinc);
   else  ns = al_fixdiv(ylen, yinc);
   int num_steps = al_fixtoi(ns);
   if ((num_steps > 0) && (num_steps < 2000))
      Ei[EN][7] = num_steps;


}

int move_pod_extended(int num)
{
   if (getxy((char *)"Set Extended Position", 99, 7, num) == 1)
   {
      Efi[num][5] = al_itofix(get100_x * 20);  // set dest x,y
      Efi[num][6] = al_itofix(get100_y * 20);
      recalc_pod(num);
      Redraw = 1;
      return 1;
   }
   else
 return 0;
}

void show_all_enemies(void)
{
   int text_pos = 0;
   al_set_target_backbuffer(display);
   al_clear_to_color(al_map_rgb(0,0,0));

   sort_enemy();
   text_pos = enemy_data(10, text_pos);
   for (int e=0; e<num_enemy; e++)
   {
      draw_enemy_shape(e, 0, text_pos);
      al_draw_textf(font, palette_color[14], 20, text_pos+6, 0, "[%2d]",e);
      strcpy(msg,"");
      for (int j=0; j<32; j++)
      {
         char msg2[80];
         sprintf(msg2,"[%d] ", Ei[e][j]);
         strcat(msg, msg2);
      }
      al_draw_text(font, palette_color[13], 54, text_pos+2, 0, msg);
      strcpy(msg,"");
      for (int j=0; j<16; j++)
      {
         char msg2[80];
         sprintf(msg2,"[%3.2f] ", al_fixtof(Efi[e][j]));
         strcat(msg, msg2);
      }
      al_draw_text(font, palette_color[9], 54, text_pos+10, 0, msg);
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





void sort_enemy(void)
{
   extern int e_num_of_type[50];   // sort stuff used by others
   extern int e_first_num[50];

   int swap_flag = 1;
   int do_swap = 0;
   int temp;
   al_fixed ftemp;
   //int num_enem;
   int c, d, x, y;

   while (swap_flag)
   {
      do_swap = 0;
      swap_flag = 0;
      for (x=0; x<99; x++)
      {

//         for (y=0; y<16; y++)
//            Efi[x][y] = al_ftofix(Ef[x][y]) ; // set all al_fixed here


         if (Ei[x][0] < Ei[x+1][0]) do_swap = 1;
         if (do_swap) // do the swap
         {
            swap_flag = 1;
            do_swap = 0;
            for (y=0; y<32; y++)
            {
               temp = Ei[x][y];
               Ei[x][y] = Ei[x+1][y];
               Ei[x+1][y] = temp;
            }
            for (y=0; y<16; y++)
            {
               ftemp = Efi[x][y];
               Efi[x][y] = Efi[x+1][y];
               Efi[x+1][y] = ftemp;
            }
         } // end of swap
      } // end of for x
   } // end of while swap flag


   // get data about first 50 enem types and make sub lists of enem types using these variables

   for (c=0; c<50; c++)   // zero the counters
      {
         e_num_of_type[c] = 0;
         e_first_num[c] = 0;
      }
   num_enemy = 0; // count enemies
   for (c=0; c<100; c++)  // counts
      {
         e_num_of_type[Ei[c][0]]++;   // inc number of this type
         if (Ei[c][0]) ++num_enemy;    // inc total num
       }

   for (c=0; c<50; c++)  // get first nums
      if (e_num_of_type[c] > 0)  // are there any of this type?
         for (d=0; d<100; d++)
            if (Ei[d][0] == c)
               {
                  e_first_num[c] = d;
                  d=100;   // exit loop
                }

}



void create_cloner(void)
{
   extern int e_num_of_type[50];   // sort stuff used by others
   extern int e_first_num[50];
   int rx=0, ry=0, en=0;

   // cycle backwards to find first empty
   for (int d=99; d>-1; d--)
      if (Ei[d][0] == 0) en = d;

   Ei[en][0] = 9;
   sort_enemy();
   en = e_first_num[9]+e_num_of_type[9]-1;

   if (getxy((char *) "Set Cloner Location", 3, 9, en) == 1)
   {
      rx = get100_x;
      ry = get100_y;

      Ei[en][0] = 9;     // type 9 - cloner
      Ei[en][1] = 231;   // shape !!
      Ei[en][2] = 1;     // draw type

      Ei[en][5] = 1;     // mode

      Ei[en][25] = 25;     // health bonus

      Ei[en][6] = 400;  // default delay
      Ei[en][7] = 400;  // default delay

      Ei[en][29] = 10;  // default collision box

      Efi[en][12] = al_itofix(1);  // scale
      Efi[en][14] = al_itofix(0);  // rotation

      Efi[en][0] = al_itofix(get100_x * 20); // position
      Efi[en][1] = al_itofix(get100_y * 20);

      draw_big(1);
      show_big();

      if (getbox((char *)"Cloner Source Area", 3, 9, en ))
      {
         Ei[en][15] = bx1;
         Ei[en][16] = by1;
         Ei[en][19] = bx2-bx1;
         Ei[en][20] = by2-by1;

         draw_big(1);

         if (getxy((char *)"Set Cloner Destination Area", 98, 9, en ) == 1)
         {
            Ei[en][17] = get100_x;
            Ei[en][18] = get100_y;
            if (move_trigger_box(en, 9)) sort_enemy();
            else Ei[en][0] = 0;
            draw_big(1);
            show_big();
         }
         else Ei[en][0] = 0;
      }  // end of get source area
   else Ei[en][0] = 0;
   }  // end of set cloner location
   else Ei[en][0] = 0;
   set_wx(rx+4, ry);
   sort_enemy();

}







int create_pod(void)
{
   int create_failed = 0;
   int en = get_empty_enemy(7); // type
   if (getxy((char *)"Set Podzilla Location", 3, 7, en) == 1)
   {
      int rx = get100_x;      // used to set return window
      int ry = get100_y;
      Efi[en][0] = al_itofix(rx * 20);  // set new x,y
      Efi[en][1] = al_itofix(ry * 20);
      Efi[en][7] = al_itofix(6);     // bullet speed
      Ei[en][1] = 374;   // shape !!
      Ei[en][2] = 0;     // draw type

      Ei[en][25] = 12;     // health bonus

      Ei[en][7] = 20;    // default seq delay
      Ei[en][9] = 20;    // default delay

      Ei[en][29] = 10;  // default collision box


      Efi[en][9] = al_itofix(10);    // default speed
      Efi[en][12] = al_itofix(1);    // default scale

      draw_big(1);
      show_big();

      if (!move_pod_extended(en)) create_failed = 1;

      if (!create_failed)
         if (!move_trigger_box(en, 7)) create_failed = 1;

      if (create_failed) Ei[en][0] = 0;

      sort_enemy();
      set_wx(rx+4, ry);
   }
   return Ei[en][0]; // type if succesful, 0 if not
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
      al_draw_bitmap(memory_bitmap[220+x], SCREEN_W/2, (x*20)+60, 0);

   al_flip_display();

   while (!exit)
   {
      proc_controllers();
      if ((mouse_b2) || (key[ALLEGRO_KEY_ESCAPE]))
      {
         proc_controllers();
         exit = 1;
         bad = 1;
      }

      if ((mouse_b1) && (mouse_x > (SCREEN_W/2)) && (mouse_x < ((SCREEN_W/2)+20)))
      {
         exit = 1;
         while (mouse_b1) proc_controllers(); // wait for release
         key_color = (mouse_y - 60)/20;
         if ((key_color < 0) || (key_color > 3)) key_color = 0;
      }
   }

   // then set the key location
   if (!bad)
   {
      if (getxy((char *)"Set Key Location", 2, 4, c) == 1)
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
      if (getbox((char *)"Block Range", 2, 4, c))
      {
         bx2 -= 1;
         by2 -= 1;
         if (bx2 < bx1) bx2 = bx1;
         if (by2 < by1) by2 = by1;
         item[c][6] = bx1;
         item[c][7] = by1;
         item[c][8] = bx2;
         item[c][9] = by2;
      }
      else bad = 1;
   }
   if (bad) return 0;
   else return 1;
}




int create_start_block(int c)
{
   if (getxy((char *)"Put Start Location", 2, 5, c) == 1)
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
   if (getxy((char *)"Put Exit Location", 2, 3, c) == 1) // xorg, yorg
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
   for (int i=0; i<500; i++)
   {
      if (pmsg[i] != NULL)
      {
         int len = strlen(pmsg[i]);
         if (len > 0)
         {
            // count lines and max line length
            int lines = 1;
            int mll = 0; // max line length
            int tlc = 0; // temp line counter

//            for (int j=0; j<len; j++)
//               al_draw_textf(font, palette_color[14], 20, text_pos+=8, 0, "[%d][%d]", j, pmsg[i][j]);
//
//            for (int j=0; j<len; j++)
//               al_draw_textf(font, palette_color[14], 20, text_pos+=8, 0, "[%d][%c]", j, pmsg[i][j]);

            for (int j=0; j<len; j++)
            {
               al_draw_textf(font, palette_color[14], 20, text_pos+=8, 0, "[%d][%d]", j, pmsg[i][j]);

               if (pmsg[i][j] == 13)
               {
                  lines++;
                  if (tlc > mll) mll = tlc;
                  tlc = 0;
               }
               else tlc++;
            }
            al_draw_textf(font, palette_color[14], 20, text_pos+=8, 0, "[%2d] len:%3d lines:%2d max length:%2d", i, len, lines, mll);
            text_pos +=8;

         }
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

void show_cursor(char *f, int cursor_pos, int xpos_c, int ypos, int cursor_color, int restore, int rot)
{
   int len = strlen(f);
   char dt[40][120];
   int row=0, col=0, cursor_row=0, cursor_col=0;
   // get cursor row and column and fill dt
   for (int a=0; a<len+1; a++)
   {
      if (a == cursor_pos)
      {
         cursor_row = row;
         cursor_col = col;
      }
      if (f[a] == 126) // line break
      {
         row++;
         col=0;
         dt[row][col] = (char)NULL; // in case len = 0
      }
      else  // regular char
      {
         dt[row][col] = f[a];
         col++;
         dt[row][col] = (char)NULL;
      }
   }

   // make a string from the cursor text char
   msg[0] = f[cursor_pos];
   msg[1] = 0;


   int x, y;
   if (rot == 0)
   {
      x = cursor_col*8+xpos_c - strlen(dt[cursor_row])*4;
      y = ypos+cursor_row*8;

      if (restore) // black background, text color text
      {
         al_draw_filled_rectangle(x, y, x+8, y+8, palette_color[0]);
         al_draw_text(font, palette_color[cursor_color], x, y, 0, msg);
      }

      else // red background, black text
      {
         al_draw_filled_rectangle(x, y, x+8, y+8, palette_color[10]);
         al_draw_text(font, palette_color[0], x, y, 0, msg);
      }
   }

   if (rot == 64)
   {
      x = xpos_c+cursor_row*8;
      y = 8+ cursor_col*8+ypos - strlen(dt[cursor_row])*4;
      if (restore) // black background, text color text
      {
         al_draw_filled_rectangle(x-4, y-4, x+4, y+4, palette_color[0]);
         rtextout_centre(NULL, msg, x, y, cursor_color, 1, 64, 1);
      }
      else // red background, black text
      {
         al_draw_filled_rectangle(x-4, y-4, x+4, y+4, palette_color[10]);
         rtextout_centre(NULL, msg, x, y, 0, 1, 64, 1);
      }
   }
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
      for (int a=0; a<8; a++)
         al_draw_rectangle(a, a, pxw-a, pyh-a, palette_color[fc+a*16], 1);

      // draw the message text
      for (row=0; row<=num_lines; row++)
         al_draw_text(font, palette_color[tc], pxw/2, 9+row*8, ALLEGRO_ALIGN_CENTER, dt[row]);

     // draw on screen
     al_set_target_backbuffer(display);
     al_draw_bitmap(tmp, px, py, 0);

      // redraw map to erase old
      if (redraw_map) show_big();

      // get the position on the map to copy it to
      int mmx = item[c][10] * db;
      int mmy = item[c][11] * db;

      al_set_clipping_rectangle(1, 1, les*db*100-2, les*db*100-2);
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


int edit_pmsg_text(int c, int new_msg)
{
   int tc = item[c][8];
   int char_count;
   int cursor_pos=0;
   int old_cp=0;
   int blink_count = 3;
   int blink_counter = 0;
   int a, k=0;
   char f[1800];
   //int redraw = 1;
   int quit = 0;

   // button row x values
   int xa = 4+SCREEN_W-(SCREEN_W-(db*100));
   int xb = SCREEN_W-4;

   int mx = txc;  // x center
   int my = pop_msg_viewer_pos;

   int bad=0;
   //clear_keybuf();

   if (new_msg)
   {
      my = 200;
      f[0] = (char)NULL;
      f[1] = (char)NULL;
      char_count = 1;
   }
   else
   {
      strcpy(f, pmsg[c]);
      char_count = strlen(f);
   }

   while (!quit)
   {

      title((char *)"Message Creator", 2, 15, 12);

      al_set_target_backbuffer(display);

      // draw the message
      display_pop_message(c, f, mx, my, 1, 0);


      // mark message we are editing with a yellow rectangle
      int x1 = mx - item[c][12]/2-2;
      int y1 = my - 10;
      int x2 = mx + item[c][12]/2;
      int y2 = my + item[c][13]-9;
      al_draw_rectangle(x1, y1, x2, y2, palette_color[14], 1);

      a = -3; //back up from the message to the buttons;;
      extern int bts;
      int by = my-bts/2-2;

      mdw_button(xa, by+a*bts, xb, by+(a+1)*bts-2, 7, 999, 0, 0, 0, 14, 15,  0, 1,0,0,0);  // edit text placeholder

      a++;
      if (mdw_button(xa, by+a*bts, xb, by+(a+1)*bts-2, 1,   0, 0, 0, 0, 11, 15, 15, 1,0,0,0))  // OK
      {
         quit = 1;
         bad = 0;
      }

      a++;
      if (mdw_button(xa, by+a*bts, xb, by+(a+1)*bts-2, 3,   0, 0, 0, 0, 10, 15, 15, 1,0,0,0))  // Cancel
      {
         quit = 1;
         bad = 1;
      }

      if (blink_counter++ < blink_count)
         show_cursor(f, cursor_pos, mx, my, tc, 0, 0);
      else show_cursor(f, cursor_pos, mx, my, tc, 1, 0);
      if (blink_counter> blink_count*2) blink_counter = 0;

      if (cursor_pos != old_cp)
      {
         show_cursor(f, old_cp, mx, my, tc, 1, 0); // erase old blinking cursor if moved
         old_cp = cursor_pos;
         blink_counter = 0;
      }

      k = proc_controllers();

      if (key[ALLEGRO_KEY_RIGHT])
      {
         if (++cursor_pos >= char_count) cursor_pos = char_count-1;
      }
      if (key[ALLEGRO_KEY_LEFT])
      {
         if (--cursor_pos < 0) cursor_pos = 0;
      }
      if ((key[ALLEGRO_KEY_DELETE]) && (cursor_pos < char_count))
      {
         for (a = cursor_pos; a < char_count; a++)
           f[a]=f[a+1];
         char_count--;
         // set last to NULL
         f[char_count] = (char)NULL;
      }
      if ((key[ALLEGRO_KEY_BACKSPACE]) && (cursor_pos > 0))
      {
         cursor_pos--;
         for (a = cursor_pos; a < char_count; a++)
           f[a]=f[a+1];
         char_count--;
         // set last to NULL
         f[char_count] = (char)NULL;
      }
      if (key[ALLEGRO_KEY_DOWN])
      {
         // find next line break
         while ((++cursor_pos < char_count) && (f[cursor_pos] != 126));
         cursor_pos++;
         // make sure we are not past the end
         if (cursor_pos >= char_count) cursor_pos = char_count-1;
      }
      if (key[ALLEGRO_KEY_UP])
      {
         // find previous line break
         while ((--cursor_pos > 0) && (f[cursor_pos] != 126));
         cursor_pos--;
         // make sure we are not before the start
         if (cursor_pos < 0) cursor_pos = 0;
      }
      if (key[ALLEGRO_KEY_HOME])
      {
         cursor_pos = 0;
      }
      if (key[ALLEGRO_KEY_END])
      {
         cursor_pos = char_count-1;
      }

      if (k)
      {
         k = Key_pressed_ASCII;
         if (k==13) k = 126; // replace enter with 126 ~

         if ((k>31) && (k<127)) // if alphanumeric
         {
            // move over to make room
            for (a = char_count; a>=cursor_pos; a--)
               f[a+1]=f[a];

            // set char
            f[cursor_pos] = k;

            // inc both
            cursor_pos++;
            char_count++;

            // set last to NULL
            f[char_count] = (char)NULL;
         }
      }


      if (key[ALLEGRO_KEY_ESCAPE])
      {
         while (key[ALLEGRO_KEY_ESCAPE]) proc_controllers();
         quit = 1;
         bad = 1;
      }

      al_flip_display();
      al_clear_to_color(al_map_rgb(0,0,0));

      al_rest(0.07);


   } // end of while (!quit)

   if (bad) return 0;
   else
   {
      free(pmsg[c]);
      pmsg[c] = (char*) malloc (strlen(f)+1);
      strcpy(pmsg[c], f);
      return 1;
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
      if (getxy((char *)"Set Location", 2, 10, c) == 1)
      {
         item[c][4] = get100_x*20;
         item[c][5] = get100_y*20;
         l[get100_x][get100_y] = 0; // zero block
      }
      else bad = 1;
   }

   if (!bad)
   {
      if (getxy((char *)"Set Display Position", 2, 1010, c) == 1)
      {
         item[c][10] = get100_x;
         item[c][11] = get100_y;
      }
      else bad = 1;
   }
   if (bad) return 0;
   else return 1;
}




int create_obj(int obt, int sub_type, int sent_num)
{
   extern int e_num_of_type[50];   // sort stuff used by others
   extern int e_first_num[50];

   int num = sent_num; // default
   int ret;

   if (obt == 2) // items
   {
      ret = create_item(sub_type);
      if (ret > 500)
      {
         num = sent_num;
         al_show_native_message_box(display,
               "Error", "No creator exists for the current item type",
               "Copy from an existing item of that type, or get one from the selection window",
                NULL, ALLEGRO_MESSAGEBOX_ERROR);
      }
      else num = ret;
   }
   if (obt == 3)
   {
      if (sub_type == 9)
      {
         create_cloner();
         num = e_first_num[sub_type]+e_num_of_type[sub_type]-1;
      }
      if (sub_type == 7)
      {
         create_pod();
         num = e_first_num[sub_type]+e_num_of_type[sub_type]-1;
      }
   }
   return num;  // to return number of created obj
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
            if (getxy((char *)"Put door location", 2, 1, c) == 1)
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


               if (getxy((char *)"Put door destination", 2, 1, d) == 1)
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
            if (getxy((char *)"Put first door", 2, 1, c) == 1)
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


               if (getxy((char *)"Put second door", 2, 1, d) == 1)
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
   return item_sort();
}




int create_item(int type)
{
   // check for no creator
   if ((type != 1) && (type != 3) && (type != 4) && (type != 5) && (type != 10))
      return 9999;

   extern int item_first_num[20];
   int c = get_empty_item(type); // get a place to put it
   if (c > 499) return c; // no items
   switch (type)
   {
      case 3:  if (!create_exit(c))        erase_item(c); break;
      case 4:  if (!create_key(c))         erase_item(c); break;
      case 5:  if (!create_start_block(c)) erase_item(c); break;
      case 10: if (!create_pmsg(c))        erase_item(c); break;
   }

   item_sort();
   c = item_first_num[type]+item_num_of_type[type]-1;
   draw_big(1);
   show_big();
   set_wx(item[c][4]/20+4, item[c][5]/20);
   return c;
}

