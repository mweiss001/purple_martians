// e_pde.cpp (20100220 cleanup)
#include "pm.h"


int load_PDE()
{
   FILE *filepntr;
   extern int PDEi[100][32];
   extern al_fixed PDEfx[100][16];
   extern char PDEt[100][20][40];
   int PDE_load_error;
   int loop, ch, c, x;
   char buff[80];
   PDE_load_error = 0;
   if (!al_filename_exists("bitmaps/pde.pm"))
   {
      sprintf(msg, "Can't find pde.pm");
      PDE_load_error = 2;
   }
   if (!PDE_load_error) // file exists
      if ((filepntr=fopen("bitmaps/pde.pm","r")) == NULL)
      {
         sprintf(msg, "Error opening pde.pm");
         m_err(msg);
         PDE_load_error = 3;
      }
   if (!PDE_load_error) // file exists and is open!
   {
      for (c=0; c<100; c++) // read PDE enemy al_fixed
         for (x=0; x<16; x++)
         {
            loop = 0;
            ch = fgetc(filepntr);
            while((ch != '\n') && (ch != EOF))
            {
               buff[loop] = ch;
               loop++;
               ch = fgetc(filepntr);
            }
            buff[loop] = (char)NULL;
            PDEfx[c][x] = atoi(buff);  // al_fixed

            if (ch == EOF)
            {
               sprintf(msg, "Error reading al_fixed in PDE");
               m_err(msg);
               PDE_load_error = 4;
            }
         }
      for (c=0; c < 100; c++)  // enemy ints
         for (x=0; x<32; x++)
         {
            loop = 0;
            ch = fgetc(filepntr);
            while((ch != '\n') && (ch != EOF))
            {
               buff[loop] = ch;
               loop++;
               ch = fgetc(filepntr);
            }
            buff[loop] = (char)NULL;
            PDEi[c][x] = atoi(buff);
            if (ch == EOF)
            {
               sprintf(msg,  "Error reading ints in PDE" );
               m_err(msg);
               PDE_load_error = 5;
            }
         }
      for (c=0; c < 100; c++)  // enemy text
         for (x=0; x<20; x++)
         {
            loop = 0;
            ch = fgetc(filepntr);
            while((ch != '\n') && (ch != EOF))
            {
               PDEt[c][x][loop] = ch;
               loop++;
               ch = fgetc(filepntr);
            }
            PDEt[c][x][loop] = (char)NULL;
            if (ch == EOF)
            {
               sprintf(msg,"Error reading text at %d %d %d in PDE", loop, c, x);
               m_err(msg);
               PDE_load_error = 6;
            }
         }
      fclose(filepntr);
   }
   if (PDE_load_error)
   {
      sprintf(msg,"PDI load error %d", PDE_load_error);
      m_err(msg);
      return 0;
   }
   return 1;
}

void save_PDE()
{
   FILE *filepntr;
   extern char PDEt[100][20][40];
   extern int PDEi[100][32];
   extern al_fixed PDEfx[100][16];
   int c, x;
   filepntr = fopen("bitmaps/pde.pm","w");
   for (c=0; c < 100; c++)  // enemy al_fixed
      for (x=0; x<16; x++)
         fprintf(filepntr,"%d\n",PDEfx[c][x]);

   for (c=0; c < 100; c++) // enemy int
      for (x=0; x<32; x++)
         fprintf(filepntr,"%d\n",PDEi[c][x]);

   for (c=0; c < 100; c++) // enemy text
      for (x=0; x<20; x++)
         fprintf(filepntr,"%s\n",PDEt[c][x]);
   fclose(filepntr);
}


// this is a non blocking, pass through function and should be called in a loop
int bottom_menu(int menu_num)
{
   int selection=0;
   if (mouse_y > 186) selection = (mouse_x / 40);   // highlight only
   for (int c=0; c<8; c++)
   {
      int d = 9+64;
      if (c == selection) d = 9;
      al_draw_textf(font, palette_color[d], c*40, 184, 0, " F%-1d  ", c+1);
      al_draw_text(font,  palette_color[d], c*40, 192, 0, global_string[menu_num][c]);
   }
   selection = 999; // normal return --  nothing happened
   if ((mouse_b1) && (mouse_y > 186))
   {
      selection = (mouse_x / 40);
      while (mouse_b1) proc_controllers();   // wait for release
   }
   if (selection == 7) selection = -1;
   if (key[ALLEGRO_KEY_F1]) selection = 0;
   if (key[ALLEGRO_KEY_F2]) selection = 1;
   if (key[ALLEGRO_KEY_F3]) selection = 2;
   if (key[ALLEGRO_KEY_F4]) selection = 3;
   if (key[ALLEGRO_KEY_F5]) selection = 4;
   if (key[ALLEGRO_KEY_F6]) selection = 5;
   if (key[ALLEGRO_KEY_F7]) selection = 6;
   if (key[ALLEGRO_KEY_F8]) selection = -1;
   if (key[ALLEGRO_KEY_ESCAPE])selection = -1;
   return selection;
}


int edit_int(int x, int y, int val, int inc, int lv, int uv)
{
   int imx = mouse_x * les;
   int imy = mouse_y * les;
   int old_mouse;
   al_hide_mouse_cursor(display);
   while (mouse_b1)
   {
      al_set_mouse_xy(display, 160, 100);
      proc_controllers();
      old_mouse = mouse_y;
      proc_controllers();

      al_draw_filled_rectangle(x, y, x+32, y+8, palette_color[0]);
      al_draw_textf(font, palette_color[10], x, y, 0, "%+-d ", val);
      al_flip_display();

      val = val - ((mouse_y - old_mouse) * inc);

      if (val > uv) val = uv;
      if (val < lv) val = lv;

   }
   al_set_mouse_xy(display, imx, imy);
   al_show_mouse_cursor(display);
   return val;
}


al_fixed edit_fix(int x, int y, al_fixed val)
{
   al_fixed fxinc = al_ftofix(.01);    // x inc
   al_fixed fyinc = al_ftofix(.1);     // y inc
   al_fixed flv   = al_ftofix(-2000);  // lv
   al_fixed fuv   = al_ftofix(2000);   // uv

   int imx = mouse_x * les; // initial mouse position
   int imy = mouse_y * les;
   int old_mouse_x;
   int old_mouse_y;
   al_hide_mouse_cursor(display);

   while (mouse_b1)
   {
      al_set_mouse_xy(display, 160, 100);
      proc_controllers();
      old_mouse_x = mouse_x;
      old_mouse_y = mouse_y;
      proc_controllers();

      al_draw_filled_rectangle(x, y, x+96, y+8, palette_color[0]);
      al_draw_textf(font, palette_color[10], x, y, 0, "%+-8.6f ",al_fixtof(val));
      al_flip_display();

      if (key[ALLEGRO_KEY_0]) val = al_ftofix(0);
      if (key[ALLEGRO_KEY_1]) val = al_ftofix(1);
      if (key[ALLEGRO_KEY_2]) val = al_ftofix(2);
      if (key[ALLEGRO_KEY_3]) val = al_ftofix(3);
      if (key[ALLEGRO_KEY_4]) val = al_ftofix(4);
      if (key[ALLEGRO_KEY_5]) val = al_ftofix(5);
      if (key[ALLEGRO_KEY_6]) val = al_ftofix(6);
      if (key[ALLEGRO_KEY_7]) val = al_ftofix(7);
      if (key[ALLEGRO_KEY_8]) val = al_ftofix(8);
      if (key[ALLEGRO_KEY_9]) val = al_ftofix(9);

      val += (mouse_x - old_mouse_x) * fxinc;
      val += (mouse_y - old_mouse_y) * fyinc;

      float sfr = 0.01; // round
      float fval = al_fixtof(val);
      fval = round(fval/sfr) * sfr;
      val = al_ftofix(fval);

      // check and enforce limits
      if (val > fuv) val = fuv;
      if (val < flv) val = flv;


   }
   al_set_mouse_xy(display, imx, imy); // set original mouse position
   al_show_mouse_cursor(display);
   return val;
}





extern int PDEi[100][32];
extern al_fixed PDEfx[100][16];
extern char PDEt[100][20][40];

void PDE_swap(int s1, int s2)
{
   for (int y=0; y<32; y++)
   {
      int temp    = PDEi[s1][y];
      PDEi[s1][y] = PDEi[s2][y];
      PDEi[s2][y] = temp;
   }
   for (int y=0; y<16; y++)
   {
      al_fixed temp   = PDEfx[s1][y];
      PDEfx[s1][y] = PDEfx[s2][y];
      PDEfx[s2][y] = temp;
   }
   for (int y=0; y<20; y++)
   {
      char stemp[80];
      strcpy(stemp,       PDEt[s1][y] );
      strcpy(PDEt[s1][y], PDEt[s2][y] );
      strcpy(PDEt[s2][y], stemp);
   }
}


void PDE_sort(void)
{
   // first just sort by type
   int swap_flag = 1;
   while (swap_flag)
   {
      swap_flag = 0;
      for (int x=0; x<99; x++)
      {
         if ( PDEi[x][0] > PDEi[x+1][0]) // sort by type
         {
            PDE_swap(x, x + 1);
            swap_flag++; // if any swaps
         }
      }
   }

   // then sort by text line
   swap_flag = 1;
   int do_swap = 0;
   while (swap_flag)
   {
      do_swap = 0;
      swap_flag = 0;
      for (int x=0; x<99; x++)
      {
         if ((PDEi[x][0] > 199) && ( PDEi[x][0] < 300))  // if both creators
            if ((PDEi[x+1][0] > 199) && ( PDEi[x+1][0] < 300))
               if (strncmp(PDEt[x][1], PDEt[x+1][1], strlen(PDEt[x][1])) > 0)
                  do_swap = 1;

         if ((PDEi[x][0] > 99) && ( PDEi[x][0] < 200))  // if both items
            if ((PDEi[x+1][0] > 99) && ( PDEi[x+1][0] < 200))
               if ( PDEi[x][0] == PDEi[x+1][0] ) // if same type
                  if (strncmp(PDEt[x][1], PDEt[x+1][1], strlen(PDEt[x][1]) ) > 0)  // secondary sort by text line 1
                     do_swap = 1;

         if ((PDEi[x][0] > 0) && ( PDEi[x][0] < 100))  // if both enemies
            if ((PDEi[x+1][0] > 0) && ( PDEi[x+1][0] < 100))
               if ( PDEi[x][0] == PDEi[x+1][0] ) // if same type
                  if (strncmp(PDEt[x][1], PDEt[x+1][1], strlen(PDEt[x][1]) ) > 0)  // secondary sort by text line 1
                     do_swap = 1;

         if (do_swap) // do the swap
         {
            do_swap = 0;
            swap_flag++; // if any swaps
            PDE_swap(x, x + 1);
         } // end of swap
      } // end of for x
   } // end of while swap flag
//   draw_select_window();
}



void predefined_enemies(void)
{
   while (mouse_b2) proc_controllers();
   if (load_PDE())
   {
      int x,y, EN = 0, redraw = 1, menu_sel;

      int quit = 0;
      while(!quit)
      {
         if (redraw)
         {
            int a,b=0;
            int rt = PDEi[EN][0];
            redraw = 0;

            al_flip_display();
            al_clear_to_color(al_map_rgb(0,0,0));

            a = PDEi[EN][1]; // bmp or ans
            if (a < NUM_SPRITES) b = a; // bmp
            if (a > 999) b = zz[5][a-1000]; // ans

            al_draw_bitmap(memory_bitmap[b], 0,0,0);

            for (x=0; x<20; x++)
               al_draw_text(font, palette_color[15], 0, 20+(x*8), 0, PDEt[EN][x]);

            if (rt < 99)
            {
               al_draw_textf(font, palette_color[15], 40, 0, 0, "Predefined Enemy %d", EN);
               for (x=0; x<16; x++) // three columns
               {
                  al_draw_textf(font, palette_color[15], 256, 20+(x*8), 0, "Ef%-2d %+-f", x, al_fixtof(PDEfx[EN][x]));
                  al_draw_textf(font, palette_color[15], 400, 20+(x*8), 0, "Ei%-3d %+-d", x, PDEi[EN][x]);
                  al_draw_textf(font, palette_color[15], 500, 20+(x*8), 0, "Ei%-3d %+-d", x+16, PDEi[EN][x+16]);
               }
            }

            if ((rt > 99) && (rt < 200))
            {
               al_draw_textf(font, palette_color[15], 40, 0, 0, "Predefined Item %d", EN);
               for (x=0; x<16; x++)
                  al_draw_textf(font, palette_color[15], 400, 20+(x*8), 0, "I%-2d %+-d", x, PDEi[EN][x]);
            }
            if (rt > 199)
            {
               al_draw_textf(font, palette_color[15], 40, 0, 0, "Special Creator %d", EN);
               for (x=0; x<16; x++) // three columns
               {
                  al_draw_textf(font, palette_color[15], 256, 20+(x*8), 0, "Ef%-2d %+-f", x, al_fixtof(PDEfx[EN][x]));
                  al_draw_textf(font, palette_color[15], 400, 20+(x*8), 0, "Ei%-2d %+-d", x, PDEi[EN][x]);
                  al_draw_textf(font, palette_color[15], 500, 20+(x*8), 0, "Ei%-2d %+-d", x+16, PDEi[EN][x+16]);
               }
            }
         }
         menu_sel = (bottom_menu(5));   // call the menu handler
         al_flip_display();
         proc_controllers();
         al_rest(0.01);


         if ((key[ALLEGRO_KEY_RCTRL]) && (key[ALLEGRO_KEY_S])) // sort
         {
            while (key[ALLEGRO_KEY_S]) proc_controllers();
            PDE_sort();

            for (int x=0; x<50; x++) PDE_swap(x, x + 50); // move all to + 50

            int insert_pos = 0; // creators to 0
            for (int x=50; x<100; x++)
            {
               int rt = PDEi[x][0];
               if (rt > 199) PDE_swap(x, insert_pos++);
            }

            insert_pos = 16; // items to 16   while (key[KEY_ESC]) proc_controllers();

            for (int x=50; x<100; x++)
            {
               int rt = PDEi[x][0];
               if ((rt > 99) && (rt < 200)) PDE_swap(x, insert_pos++);
            }

            insert_pos = 32; // enemies to 32
            for (int x=50; x<100; x++)
            {
               int rt = PDEi[x][0];
               if ((rt) && (rt < 99)) PDE_swap(x, insert_pos++);
            }
            redraw = 1;
         }


         // --------------  edit text
         if ((mouse_b1) && (mouse_x < 240) && (mouse_y > 20) && (mouse_y < 180))
         {
            int line_length = 30;
            int tx = mouse_x/8;
            int ty = (mouse_y-20)/8;
            int text_edit_quit = 0;
            while (!text_edit_quit)
            {
               // redraw
               // erase background
               al_draw_filled_rectangle(0, 20, 256, 180, palette_color[0]);
               al_draw_rectangle(0, 20, 256, 180, palette_color[14], 1);
               al_draw_text(font, palette_color[14], 128, 12,  ALLEGRO_ALIGN_CENTER, "Text Edit Mode");

               // redraw all the text
               for (x=0; x<20; x++)
               {
                  redraw = 0;
                  al_draw_text(font, palette_color[15], 0, 20+(x*8), 0, PDEt[EN][x]);
               }
               // mark the text entry position
               al_draw_filled_rectangle((tx*8), 20+(ty*8), (tx*8)+8, 20+(ty*8)+8, palette_color[138]);
               msg[0] = PDEt[EN][ty][tx];
               if (msg[0] == (char)NULL) msg[0] = 32;
               msg[1] = (char)NULL;
               al_draw_text(font, palette_color[10], (tx*8), 20+(ty*8), 0, msg);
               al_flip_display();


               proc_controllers();
               int k = Key_pressed_ASCII;
               if ((k>31) && (k<127)) // insert if alphanumeric or return
               {
                  int z = strlen(PDEt[EN][ty]);
                  if (z > line_length) z = line_length;
                  for (x=z; x>tx; x--)
                     PDEt[EN][ty][x] = PDEt[EN][ty][x-1];
                  PDEt[EN][ty][tx] = k;
                  if (++tx > line_length) // end of line?
                  {
                     PDEt[EN][ty][tx] = (char)NULL; // terminate the line
                     ty++;  // LF
                     tx = 0; // CR
                  }
               }

               if (key[ALLEGRO_KEY_BACKSPACE])
               {
                  while (key[ALLEGRO_KEY_BACKSPACE]) proc_controllers();
                  if (--tx<0) tx = 0;
                  int z = strlen(PDEt[EN][ty]);
                  for (x=tx; x<z; x++)
                     PDEt[EN][ty][x] = PDEt[EN][ty][x+1];
               }
               if (key[ALLEGRO_KEY_ENTER])
               {
                  while (key[ALLEGRO_KEY_ENTER]) proc_controllers();
                  for (y=19; y>ty; y--)  // slide all down
                     strcpy(PDEt[EN][y],PDEt[EN][y-1]);
                  if (strlen(PDEt[EN][ty]) == 999) // cursor past end of line
                     PDEt[EN][ty+1][0] = (char)NULL; // next line empty
                  if ((signed int)strlen(PDEt[EN][ty]) >= tx) // cursor not past end of line
                  {
                     for (x=0; x <= 30-tx; x++)         // split line at tx
                         PDEt[EN][ty+1][x] = PDEt[EN][ty+1][tx+x];

                     PDEt[EN][ty][tx] = (char)NULL;  // terminate top line
                     tx = 0;
                     ty++;
                  }
               }
               if (key[ALLEGRO_KEY_DELETE])
               {
                  while (key[ALLEGRO_KEY_DELETE]) proc_controllers();
                  if (PDEt[EN][ty][tx] == (char)NULL)
                  {
                     for (x=0; x<=30-tx; x++) // get portion from line below
                         PDEt[EN][ty][tx+x] = PDEt[EN][ty+1][x];
                     for (y=ty+1; y<19; y++)  // slide all up
                        strcpy(PDEt[EN][y],PDEt[EN][y+1]);
                     PDEt[EN][19][0] = (char)NULL; // last line empty
                  }
                  else
                  {
                     for (x = tx; x < (int)strlen(PDEt[EN][ty]); x++)
                        PDEt[EN][ty][x] = PDEt[EN][ty][x+1];
                  }
               }

               if (key[ALLEGRO_KEY_RIGHT])
               {
                  while (key[ALLEGRO_KEY_RIGHT]) proc_controllers();
                  if (++tx > line_length-1) tx = line_length-1;
               }
               if (key[ALLEGRO_KEY_LEFT])
               {
                  while (key[ALLEGRO_KEY_LEFT]) proc_controllers();
                  if (--tx < 0) tx = 0;
               }
               if (key[ALLEGRO_KEY_UP])
               {
                  while (key[ALLEGRO_KEY_UP]) proc_controllers();
                  if (--ty < 0) ty = 0;
               }
               if (key[ALLEGRO_KEY_DOWN])
               {
                  while (key[ALLEGRO_KEY_DOWN]) proc_controllers();
                  if (++ty > 19) ty = 19;
               }
               if ((mouse_b1) && (mouse_x < 250) && (mouse_y > 20) && (mouse_y < 180))
               {
                  ty = (mouse_y-20)/8;
                  tx = mouse_x/8;
               }
               if (tx > (signed int)strlen(PDEt[EN][ty])) tx = strlen(PDEt[EN][ty]);

               while ((key[ALLEGRO_KEY_ESCAPE]) || (mouse_b2))
               {
                  proc_controllers();
                  text_edit_quit = 1;
               }
            }
            redraw = 1;
         } // end of edit text


         if (key[ALLEGRO_KEY_PGUP])
         {
            while (key[ALLEGRO_KEY_PGUP]) proc_controllers();
            EN +=10;
            if (EN > 99) EN = 99;
            redraw =1;
         }
         if (key[ALLEGRO_KEY_PGDN])
         {
            while (key[ALLEGRO_KEY_PGDN]) proc_controllers();
            EN -=10;
            if (EN < 0) EN = 0;
            redraw =1;
         }
         if ((key[ALLEGRO_KEY_RCTRL]) && (key[ALLEGRO_KEY_DELETE])) // DELETE PD
         {
            while (key[ALLEGRO_KEY_DELETE]) proc_controllers();
            for (int y=0; y<32; y++) PDEi[EN][y] = 0;
            for (int y=0; y<16; y++) PDEfx[EN][y] = al_itofix(0);
            for (int y=0; y<20; y++) strcpy(PDEt[EN][y],"");
            PDEi[EN][0] = 9999; // mark as empty
            redraw =1;
            //PDE_sort();
         }



         if (menu_sel == 1) // prev PDE
         {
            if (--EN < 0) EN = 0;
            redraw =1;
         }
         if (menu_sel == 2) // next PDE
         {
            if (++EN > 99) EN = 99;
            redraw =1;
         }
         if (menu_sel == 5) // save PDE
         {
            save_PDE();
            redraw =1;
         }
         if (menu_sel == 6) // load PDE
         {
            load_PDE();
            redraw =1;
         }

         if ((mouse_y > 20) && (mouse_y < 148) && (mouse_b1))  // edit variables
         {
            y = (mouse_y-20)/8;
            if ((mouse_x > 256) && (mouse_x < 400)) PDEfx[EN][y]   = edit_fix(296, 20+(y*8), PDEfx[EN][y]);
            if ((mouse_x > 400) && (mouse_x < 500)) PDEi[EN][y]    = edit_int(440, 20+(y*8), PDEi[EN][y], 1, -1, 20000);
            if ((mouse_x > 500) && (mouse_x < 600)) PDEi[EN][y+16] = edit_int(540, 20+(y*8), PDEi[EN][y+16], 1, -1, 20000);
            redraw=1;
        }

         while (key[ALLEGRO_KEY_ESCAPE])
         {
            quit = 1;
            proc_controllers();
         }
         if (menu_sel == -1) quit = 1;
      }
   }
}
























