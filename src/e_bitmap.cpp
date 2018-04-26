// e_bitmap.cpp (20100220 cleanup)
#include "pm.h"


//extern int swbl[NUM_SPRITES][2];
//extern int swnbl;

/*

int show_draw_mode(int msg, DIALOG *d, int c) // show draw mode
{
   extern int line_draw_mode;
   extern int old_line_draw_mode;

   switch (msg)
   {
      case MSG_IDLE:
         if (line_draw_mode != old_line_draw_mode)
         {
            old_line_draw_mode = line_draw_mode;
            show_mouse(NULL);
            SEND_MESSAGE(d, MSG_DRAW, 0);
            show_mouse(screen);
         }
      break;
      case MSG_DRAW:
      {
         char fmsg[80];
         // clear old
         sprintf(fmsg,"                    ");
         textout_ex(screen, font, fmsg, 5, 230, palette_color[11], 0);

         // find mode
         if (line_draw_mode == 0) sprintf(fmsg,"Point Draw");
         if (line_draw_mode == 1) sprintf(fmsg,"Line Draw");
         if (line_draw_mode == 2) sprintf(fmsg,"Framed Rectangle");
         if (line_draw_mode == 3) sprintf(fmsg,"Filled Rectangle");
         if (line_draw_mode == 4) sprintf(fmsg,"Framed Circle");
         if (line_draw_mode == 5) sprintf(fmsg,"Filled Circle");
         if (line_draw_mode == 6) sprintf(fmsg,"Flood Fill");
         if (line_draw_mode == 7) sprintf(fmsg,"Replace Color");
         if (line_draw_mode == 8) sprintf(fmsg,"New Selection");

         // draw it on screen
         int tl = (strlen(fmsg)*8);

         int x1 = 83-tl/2;
         int x2 = 83+tl/2 + 2;

         int y1 = 220;
         int y2 = y1+18;
         int color = 9;

         // erase
         rect(screen, 3, y1, 160, y2, palette_color[0]);

         rect(screen, x1, y1, x2, y2, palette_color[14]);
         textout_centre_ex(screen, font, "Draw Mode", 83+2, y1+2, palette_color[14], 0);
         textout_centre_ex(screen, font, fmsg, 83+2, y1+10, palette_color[color], 0);

      }
      break;
   } // end of switch case
   return D_O_K;
}

int draw_current_bitmap(int msg, DIALOG *d, int c)
{
   extern int old_px;
   extern int old_py;
   extern int bmp_index;
   extern int cbx, cby, cbs;
   extern int slx0, sly0, slx1, sly1;

   extern int b1_color;
   extern int grid_flag;
   char tmsg[80];
   int x, y;
   switch (msg)
   {
      case MSG_IDLE:
         if ((mouse_x > cbx) && (mouse_x < cbx + (20 * cbs)) && (mouse_y > cby) && (mouse_y < cby + (20 * cbs)))
         {
            x = ((mouse_x-cbx)/cbs);
            y = ((mouse_y-cby)/cbs);
            if ((old_px != x) || (old_py != y)) // not same as last
            {
               old_px = x;
               old_py = y;
               show_mouse(NULL);
               SEND_MESSAGE(d, MSG_DRAW, 0);
               show_mouse(screen);
            }
         }
      break;
      case MSG_DRAW:
      {
         // im going to assume cbs is always going to be 20
         ALLEGRO_BITMAP* tmp = al_create_bitmap(402, 423);
         clear(tmp);

         if ((mouse_x > cbx) && (mouse_x < cbx + (20 * cbs)) && (mouse_y > cby) && (mouse_y < cby + (20 * cbs)))
         {
            x = ((mouse_x-cbx)/cbs);
            y = ((mouse_y-cby)/cbs);

            sprintf(tmsg,"x:%-2d ",x);
            textout_ex(tmp, font, tmsg, 1+1, 12+(20*cbs)+2, palette_color[15], 0);
            sprintf(tmsg,"y:%-2d ",y);
            textout_ex(tmp, font, tmsg, 1+40, 12+(20*cbs)+2, palette_color[15], 0);
            c = getpixel(memory_bitmap[bmp_index], x, y);
            sprintf(tmsg,"color:%-2d ",c);
            textout_ex(tmp, font, tmsg, 1+80, 12+(20*cbs)+2, palette_color[15], 0);

            // show color
            rectfill(tmp, 1+156, 12+(20*cbs)+1, 1+195, 12+(20*cbs)+10, palette_color[c]);

            // frame all
            rect(tmp, 1-1, 12+(20*cbs), 1+196, 12+(20*cbs)+10, palette_color[15]);

         }
         else rectfill(tmp, 1-1, 12+(20*cbs)+1, 1+180, 12+(20*cbs)+11, palette_color[0]);

         // show color
         rectfill(tmp, 1+360, 12+(20*cbs)+1, 1+399, 12+(20*cbs)+10, palette_color[c]);

         // frame all
         rect(tmp, 1+250, 12+(20*cbs), 1+400, 12+(20*cbs)+10, palette_color[15]);


         // title and frame
         sprintf(tmsg,"Bitmap # %-3d ",bmp_index);
         textout_ex(tmp, font, tmsg, 1+1,12-9, palette_color[15], 0);
         rect(tmp, 1+100, 12-11, 1-1, 12-1, palette_color[15]);
         rect(tmp, 1-1, 12-1, 1+(20*cbs), 12+(20*cbs), palette_color[15]);

         for (y=0; y<20; y++)  // draw the big image
            for (x=0; x<20; x++)
            {
               rectfill(tmp,((x*cbs)+1),(y*cbs)+12,((x*cbs)+1+cbs-1),((y*cbs)+12+cbs-1),getpixel(memory_bitmap[bmp_index],x,y));
               if (grid_flag) rect(tmp,((x*cbs)+1),(y*cbs)+12,((x*cbs)+1+cbs),((y*cbs)+12+cbs),palette_color[15+128] );
            }
         // show selection
         for (c=0; c<4; c++)
            rect(tmp, 1+(slx0*cbs)+c, 12+(sly0*cbs)+c, 1+(slx1*cbs)-1-c, 12+(sly1*cbs)-1-c, palette_color[14+(c*32)] );

//         show_mouse(NULL);
         blit(tmp, screen, 0, 0, cbx-1, cby-12, 402, 423);
//         show_mouse(screen);
         al_destroy_bitmap(tmp);

      }
      break;
      case MSG_CLICK:
         x = ((mouse_x-cbx)/cbs);
         y = ((mouse_y-cby)/cbs);
         if (mouse_b & 1)
         {
            extern int line_draw_mode;
            extern int zzindx;

            ALLEGRO_BITMAP *temp = NULL;
            extern int cbx;
            extern int cby;
            extern int cbs;
            int omx, omy;
            int x1, y1, x2, y2;


            temp = al_create_bitmap(20,20);
            set_clip_rect(temp, 0,0,20,20);
            clear(temp);
            omx = mouse_x;
            omy = mouse_y;

            x1 = x; x2 = x;
            y1 = y; y2 = y;

            while (mouse_b  & 1)
            {
               int x4, y4;

               x2 = x1 - (omx - mouse_x)/cbs;
               y2 = y1 - (omy - mouse_y)/cbs;
               clear(temp);

               int sel_color = b1_color;

               if (b1_color == 0) sel_color = 15+128;

               switch (line_draw_mode)
               {
                  case 1: line       (temp, x1, y1, x2, y2, sel_color); break;
                  case 2: rect       (temp, x1, y1, x2, y2, sel_color); break;
                  case 3: rectfill   (temp, x1, y1, x2, y2, sel_color); break;
                  case 4: circle     (temp, x2, y1, y2-y1, sel_color); break;
                  case 5: circlefill (temp, x2, y1, y2-y1, sel_color); break;
               }
               show_mouse(NULL);

               // draw original on screen
               for (x4=0; x4<20; x4++)
                  for (y4=0; y4<20; y4++)
                     rectfill(screen,((x4*cbs)+cbx), (y4*cbs)+cby, (x4*cbs)+cbx+cbs-1, (y4*cbs)+cby+cbs-1, getpixel(memory_bitmap[bmp_index],x4,y4) );

               if (line_draw_mode != 8)
               {
                  // draw temp on screen
                  for (x4=0; x4<20; x4++)
                     for (y4=0; y4<20; y4++)
                        if (getpixel(temp,x4,y4)) rectfill(screen,((x4*cbs)+cbx), (y4*cbs)+cby, (x4*cbs)+cbx+cbs-1, (y4*cbs)+cby+cbs-1, getpixel(temp,x4,y4) );
                  show_mouse(screen);
               }

               if (line_draw_mode == 8)
               {
                  if (x1>19) x1 = 19;
                  if (y1>19) y1 = 19;
                  if (x2>19) x2 = 19;
                  if (y2>19) y2 = 19;
                  if (x1<0) x1 = 0;
                  if (y1<0) y1 = 0;
                  if (x2<0) x2 = 0;
                  if (y2<0) y2 = 0;
                  // show selection
                  for (c=0; c<4; c++)
                     rect(screen, cbx+(x1*cbs)+c, cby+(y1*cbs)+c, cbx+((x2+1)*cbs)-1-c, cby+((y2+1)*cbs)-1-c, palette_color[14+(c*32)]);
                  al_rest(0.1);
               }

               if ((mouse_b & 2) || (key[KEY_ESC]))
               {
                  al_destroy_bitmap(temp);
                  return D_REDRAW;
               }

            } // end of while mouse b1
            switch (line_draw_mode)
            {
               case 0: putpixel  (memory_bitmap[bmp_index], x, y, b1_color); break;
               case 1: line      (memory_bitmap[bmp_index], x1, y1, x2, y2, b1_color); break;
               case 2: rect      (memory_bitmap[bmp_index], x1, y1, x2, y2, b1_color); break;
               case 3: rectfill  (memory_bitmap[bmp_index], x1, y1, x2, y2, b1_color); break;
               case 4: circle    (memory_bitmap[bmp_index], x2, y1, y2-y1, b1_color); break;
               case 5: circlefill(memory_bitmap[bmp_index], x2, y1, y2-y1, b1_color); break;
               case 6: floodfill (memory_bitmap[bmp_index], x1, y1, b1_color); break;
               case 8: // new selection
               {
                  if (x1>x2)
                  {
                     int ti = x2;
                     x2 = x1;
                     x1 = ti;
                  }
                  if (y1>y2)
                  {
                     int ti = y2;
                     y2 = y1;
                     y1 = ti;
                  }

                  if (x1>19) x1 = 19;
                  if (y1>19) y1 = 19;
                  if (x2>19) x2 = 19;
                  if (y2>19) y2 = 19;
                  if (x1<0) x1 = 0;
                  if (y1<0) y1 = 0;
                  if (x2<0) x2 = 0;
                  if (y2<0) y2 = 0;

                  slx0 = x1;
                  sly0 = y1;
                  slx1 = x2+1;
                  sly1 = y2+1;
               }
               break;
               case 7: // replace color
               {
                  int rc, xx, yy, azz, bzz;
                  rc = getpixel(memory_bitmap[bmp_index], x, y);
                  line_draw_mode = 0;
                  for (bzz=0; bzz<16; bzz++)
                     if ((zz[4][zzindx]) > (bzz - 1))  // is shape in seq?
                     {
                        azz=zz[ 5+bzz][zzindx];
                        for (yy=0; yy<20; yy++)
                           for (xx=0; xx<20; xx++)
                              if (getpixel(memory_bitmap[azz], xx, yy) == rc)
                                  putpixel(memory_bitmap[azz], xx, yy, b1_color);
                     }
                  for (yy=0; yy<20; yy++) // do the current in case its not in seq
                     for (xx=0; xx<20; xx++)
                        if (getpixel(memory_bitmap[bmp_index], xx, yy) == rc)
                           putpixel(memory_bitmap[bmp_index], xx, yy, b1_color);
                  while (mouse_b  & 1); // wait for release
                  return D_REDRAW;
               }
               break;
            } // end of switch (line_draw_mode)
            al_destroy_bitmap(temp);
            return D_REDRAW;
         }  // end of if (mouse_b & 1)
         if (mouse_b & 2)
         {
            while (mouse_b  & 2); // wait for release
            b1_color = getpixel(memory_bitmap[bmp_index], x, y);
            return D_REDRAW;
         }
      break; // end of case MSG_CLICK
   }
   return D_O_K;
}


int new_draw_csp(int msg, DIALOG *d, int c)
{
   extern int cspx, cspy, csps;
   extern int b1_color;
   char tmsg[80];
   int x,y;
   switch (msg)
   {
      case MSG_DRAW:
      {
         // assume csps is always 10
         ALLEGRO_BITMAP* tmp = al_create_bitmap(162, 184);
         clear(tmp);
         for (y=0; y<16; y++)
            for (x=0; x<16; x++)
               rectfill(tmp, 1+(x*csps), 12+(y*csps), 1+((x+1)*csps), 12+((y+1)*csps), palette_color[(y*16)+x]);

         rect(tmp, 1-1, 12-1, 1+(16*csps), 12+(16*csps),palette_color[11]);
         rect(tmp, 1-1, 12-1, 1+(16*csps), 12+(16*csps)+10,palette_color[15]);
         rect(tmp, 1+60, 12-11, 1-1, 12-1, palette_color[15]);
         textout_ex(tmp, font, "Palette", 1+1,12-9, palette_color[15], 0);

         sprintf(tmsg,"Color=%-3d",b1_color);
         textout_ex(tmp, font, tmsg, 1+1, 12+(16*csps)+2, 15, 0);
         rectfill(tmp, 1+(8*9), 12+(16*csps)+1,1+(16*csps)-1, 12+(16*csps)+9, palette_color[b1_color]);

         blit(tmp, screen, 0, 0, cspx-1, cspy-12, 162, 184);
         al_destroy_bitmap(tmp);
      }
      break;
      case MSG_CLICK:
         b1_color = ((mouse_x-cspx) / csps) + ((mouse_y - cspy) / csps) * 16;
         while (mouse_b & 1); // wait for release
         return D_REDRAW;
      break;
   }
   return D_O_K;
}




*/


/*

int draw_animation(int msg, DIALOG *d, int c)
{
   extern int zzindx;
   extern int bmp_index;
   char tmsg[80];
   int e;
   switch (msg)
   {
      case MSG_DRAW:
         sprintf(tmsg,"Animation Sequence # %-2d ",zzindx);
         textout_ex(screen, font, tmsg, 4,446, palette_color[15], 0);
         rect(screen, 190, 443, 2, 454, palette_color[15]);
         rect(screen, 2, 454, 323, 476, palette_color[15]);
         for (e = 0; e < 16; e++)   // show current seq shapes
            if ((zz[4][zzindx]) > (e - 1))  // is shape in seq?
               blit(memory_bitmap[zz[ 5+e][zzindx]],screen,0,0,3+e*20,456,20,20);
      break;
      case MSG_CLICK:
         bmp_index = zz[5 + ((mouse_x-3)/20)][zzindx];
         return D_REDRAW;
      break;
   }
   return D_O_K;
}
int color_bars(int msg, DIALOG *d, int c)
{
   extern PALLETE pallete;
   extern int b1_color;
   char tmsg[80];
   int e;
   switch (msg)
   {
      case MSG_DRAW:
         pallete[252].r = pallete[b1_color].r; // set temp color to red value
         pallete[252].g = 0;
         pallete[252].b = 0;
         pallete[253].r = 0;
         pallete[253].g = pallete[b1_color].g; // set temp color to green value
         pallete[253].b = 0;
         pallete[254].r = 0;
         pallete[254].g = 0;
         pallete[254].b = pallete[b1_color].b; // set temp color to blue value

         set_pallete(pallete);

         rectfill(screen, 320, 60,  639, 99,  palette_color[252]);
         rect    (screen, 320, 60,  639, 99,  palette_color[15] );

         rectfill(screen, 320, 100, 639, 139, palette_color[253]);
         rect    (screen, 320, 100, 639, 139, palette_color[15] );

         rectfill(screen, 320, 140, 639, 179, palette_color[254]);
         rect    (screen, 320, 140, 639, 179, palette_color[15] );

         sprintf(tmsg,"Red   = %-2d",pallete[b1_color].r);
         textout_ex(screen, font, tmsg, 220, 75, palette_color[15], 0);
         e = (pallete[b1_color].r * 5) + 320;
         rectfill(screen, e, 60, e+3, 99, palette_color[15]);


         sprintf(tmsg,"Green = %-2d",pallete[b1_color].g);
         textout_ex(screen, font, tmsg, 220, 115, palette_color[15], 0);
         e = (pallete[b1_color].g * 5) + 320;
         rectfill(screen, e, 100, e+3, 139, palette_color[15]);


         sprintf(tmsg,"Blue  = %-2d",pallete[b1_color].b);
         textout_ex(screen, font, tmsg, 220, 155, palette_color[15], 0);
         e = (pallete[b1_color].b * 5) + 320;
         rectfill(screen, e, 140, e+3, 179, palette_color[15]);

         rectfill (screen, 320, 200, 639, 380, palette_color[b1_color]);

      break;

      case MSG_CLICK:
         // get rid of bars
         rectfill(screen, 320, 60,  639, 99,  palette_color[252]);
         rect    (screen, 320, 60,  639, 99,  palette_color[15] );

         rectfill(screen, 320, 100, 639, 139, palette_color[253]);
         rect    (screen, 320, 100, 639, 139, palette_color[15] );

         rectfill(screen, 320, 140, 639, 179, palette_color[254]);
         rect    (screen, 320, 140, 639, 179, palette_color[15] );
         while (mouse_b & 1)
         {
            if ((mouse_y >  60) && (mouse_y < 100) ) pallete[b1_color].r = ((mouse_x - 320) / 5 );// red bar
            if ((mouse_y > 100) && (mouse_y < 140) ) pallete[b1_color].g = ((mouse_x - 320) / 5 );// green bar
            if ((mouse_y > 140) && (mouse_y < 180) ) pallete[b1_color].b = ((mouse_x - 320) / 5 );// blue bar

            if (pallete[b1_color].r > 63) pallete[b1_color].r = 63;
            if (pallete[b1_color].g > 63) pallete[b1_color].g = 63;
            if (pallete[b1_color].b > 63) pallete[b1_color].b = 63;
            show_mouse(NULL);

            sprintf(tmsg,"Red   = %-2d",pallete[b1_color].r);
            textout_ex(screen, font, tmsg, 220, 75, palette_color[15], 0);

            xor_mode(TRUE);
            e = (pallete[b1_color].r * 5) + 320;
            rectfill(screen, e, 60, e+3, 99, palette_color[127]);
            al_rest(0.01);
            e = (pallete[b1_color].r * 5) + 320;
            rectfill(screen, e, 60, e+3, 99, palette_color[127]);
            xor_mode(FALSE);


            sprintf(tmsg,"Green = %-2d",pallete[b1_color].g);
            textout_ex(screen, font, tmsg, 220, 115, palette_color[15], 0);

            xor_mode(TRUE);
            e = (pallete[b1_color].g * 5) + 320;
            rectfill(screen, e, 100, e+3, 139, palette_color[127]);
            al_rest(0.01);
            e = (pallete[b1_color].g * 5) + 320;
            rectfill(screen, e, 100, e+3, 139, palette_color[127]);
            xor_mode(FALSE);


            sprintf(tmsg,"Blue  = %-2d",pallete[b1_color].b);
            textout_ex(screen, font, tmsg, 220, 155, palette_color[15], 0);

            xor_mode(TRUE);
            e = (pallete[b1_color].b * 5) + 320;
            rectfill(screen, e, 140, e+3, 179, palette_color[127]);
            al_rest(0.01);
            e = (pallete[b1_color].b * 5) + 320;
            rectfill(screen, e, 140, e+3, 179, palette_color[127]);
            xor_mode(FALSE);

            show_mouse(screen);


            pallete[252].r = pallete[b1_color].r; // set temp color to red value
            pallete[252].g = 0;
            pallete[252].b = 0;
            pallete[253].r = 0;
            pallete[253].g = pallete[b1_color].g; // set temp color to green value
            pallete[253].b = 0;
            pallete[254].r = 0;
            pallete[254].g = 0;
            pallete[254].b = pallete[b1_color].b; // set temp color to blue value


            set_pallete(pallete);
         }
         return D_REDRAW;
      break;
   }
   return D_O_K;
}

int fade_proc(int msg, DIALOG *d, int c)
{
   extern int b1_color;
   extern PALLETE pallete;
   int ret;
   ret =  d_button_proc(msg, d, c);
   if (ret == D_CLOSE)
   {
      if (b1_color < 16)
      {
         int x;
         int cn = b1_color;
         float ns = 15;
         for (x=0; x<ns; x++)
         {
            pallete[cn+(x*16)].r = (unsigned char) ( pallete[cn].r * (1 - (x/ns)) );
            pallete[cn+(x*16)].g = (unsigned char) ( pallete[cn].g * (1 - (x/ns)) );
            pallete[cn+(x*16)].b = (unsigned char) ( pallete[cn].b * (1 - (x/ns)) );
         }
         set_pallete(pallete);
      }
      return D_REDRAW;
   }
   return ret;
}

*/


int select_bitmap_ans(int zzindx)
{
   extern int bmp_index;
   int quit = 0;
   while (!quit)
   {
      proc_controllers();
      al_flip_display();
      al_clear_to_color(al_map_rgb(0,0,0));
      al_draw_text(font, palette_color[9], 0, 642, 0, "Select a Bitmap with b1");
      al_draw_text(font, palette_color[9], 0, 650, 0, "b2 or ESC to exit      ");

      // draw 32x32 bitmaps
      for (int y = 0; y < 32; y++)
         for (int x = 0; x < 32; x++)
            al_draw_bitmap(memory_bitmap[x+(y*32)],x*20, y*20, 0);
      al_draw_rectangle(0.5, 0.5, 640.5, 640.5, palette_color[13], 1);

      int xc = 180;
      int yc = 648;

      al_draw_textf(font, palette_color[14], 80, 666, 0, "Get Shape %d ", zz[4][zzindx] );

      sprintf(msg, "Current Sequence %d",zzindx);
      int l = 2+strlen(msg)*4;
      al_draw_text(font, palette_color[13], xc+150, yc+2, ALLEGRO_ALIGN_CENTER, msg);
      al_draw_rectangle(xc+150-l, yc+1, xc+150+l, yc+11, palette_color[13], 1);

      for (int c=0; c < zz[4][zzindx] + 1; c++)   // show current seq shapes
         if (( zz[5+c][zzindx] < NUM_SPRITES) && (zz[5+c][zzindx] > 0))
            al_draw_bitmap(memory_bitmap[ zz[5+c][zzindx] ], xc+1+c*20, yc+12, 0);
      al_draw_rectangle(xc+0.5, yc+11.5, xc+302.5, yc+32.5, palette_color[13], 1);


      if ((mouse_y < 640) && (mouse_x < 640))
      {
         int pointer = (mouse_x/20) + (mouse_y/20) * 32 ;
         al_draw_textf(font, palette_color[13], 522, 648, 0, "pointer %-2d", pointer );
         al_draw_bitmap(memory_bitmap[pointer], 620, 642, 0);

         al_draw_rectangle(518, 640.5, 640.5, 662.5, palette_color[13], 1);

         if (mouse_b1)
         {
            while (mouse_b1) proc_controllers();
            bmp_index = pointer;
            return 1;
         }
      }
      while ((key[ALLEGRO_KEY_ESCAPE]) || (mouse_b2))
      {
         proc_controllers();
         quit = 1;
      }
   }
   return -1;
}

int edit_int(int x, int y, int val, int inc, int lv, int uv);

int animation_proc()
{
   if (les != 1)
   {
      les = 1;
      rebuild_bitmaps();
   }


   extern int zzindx;
   extern int bmp_index;
   int pointer = zzindx;
   int as_quit = 0;
   int quit = 0;
   int c, x;


   int button_x1 = 40;
   int button_x2 = 260;
   int button_xc = button_x1 + (button_x2-button_x1)/2;
   int button_y = 240;
   int jh;


   al_set_target_backbuffer(display);
   al_show_mouse_cursor(display);

   while (!quit)
   {
      proc_controllers();
      al_flip_display();
      al_clear_to_color(al_map_rgb(0,0,0));

      sprintf(msg, "Animation Sequence Editor");
      int l = 2+strlen(msg)*4;
      al_draw_text(font, palette_color[9], 320, 20, ALLEGRO_ALIGN_CENTER, msg);
      al_draw_rectangle(320-l, 19, 320+l, 29, palette_color[9], 1);

      if (pointer != -1)
      {
         sprintf(msg, "Pointer %d", pointer);
         l = 2+strlen(msg)*8;
         al_draw_text(font, palette_color[9], 643.5 - l, 192, 0, msg);
         al_draw_rectangle(643.5-l-2, 191, 642.5, 201, palette_color[9], 1);
      }

      jh=1;
      al_draw_rectangle(button_x1, button_y+(jh*12), button_x2, button_y+(jh*12)+10, palette_color[15], 1);
      sprintf(msg,"Get New Shapes");
      al_draw_text(font, palette_color[13], button_xc, button_y+(jh*12)+1, ALLEGRO_ALIGN_CENTER, msg);

      jh=2;
      al_draw_rectangle(button_x1, button_y+(jh*12), button_x2, button_y+(jh*12)+10, palette_color[15], 1);
      sprintf(msg,"Passcount Delay %-2d  ",zz[3][zzindx]);
      al_draw_text(font, palette_color[13], button_xc, button_y+(jh*12)+1, ALLEGRO_ALIGN_CENTER, msg);

      jh=3;
      al_draw_rectangle(button_x1, button_y+(jh*12), button_x2, button_y+(jh*12)+10, palette_color[15], 1);
      sprintf(msg,"Edit Bitmap Attributes");
      al_draw_text(font, palette_color[13], button_xc, button_y+(jh*12)+1, ALLEGRO_ALIGN_CENTER, msg);

      jh=4;
      al_draw_rectangle(button_x1, button_y+(jh*12), button_x2, button_y+(jh*12)+10, palette_color[15], 1);
      sprintf(msg,"Copy Bitmaps");
      al_draw_text(font, palette_color[13], button_xc, button_y+(jh*12)+1, ALLEGRO_ALIGN_CENTER, msg);

      jh=5;
      al_draw_rectangle(button_x1, button_y+(jh*12), button_x2, button_y+(jh*12)+10, palette_color[15], 1);
      sprintf(msg,"Save Changes");
      al_draw_text(font, palette_color[13], button_xc, button_y+(jh*12)+1, ALLEGRO_ALIGN_CENTER, msg);


      sprintf(msg, "Current Sequence %d",zzindx);
      l = 2+strlen(msg)*4;
      al_draw_text(font, palette_color[13], 150, 202, ALLEGRO_ALIGN_CENTER, msg);
      al_draw_rectangle(150-l, 201, 150+l, 211, palette_color[13], 1);

      for (c = 0; c < zz[4][zzindx] + 1; c++)   // show current seq shapes
         if (( zz[5+c][zzindx] < NUM_SPRITES) && (zz[5+c][zzindx] > 0))
            al_draw_bitmap(memory_bitmap[ zz[5+c][zzindx] ], 1+c*20, 212, 0);
      al_draw_rectangle(0.5, 211.5, 302.5, 232.5, palette_color[13], 1);

      for (c=0; c < 32; c++)   // draw 32x8 grid of animation sequences
         for (x=0; x < 8; x++)
            if (zz[4][c + (x * 32)] != 0)
               if ((zz[0][c + (x * 32)] < NUM_SPRITES) && (zz[0][c + (x * 32)] > 0 ))
                  al_draw_bitmap(memory_bitmap[zz[0][c + (x * 32)]], 2+c*20, 30+x*20, 0);
      al_draw_rectangle(0.5, 29.5, 642.5, 190.5, palette_color[9], 1);


      // erase current sequence
      if (key[ALLEGRO_KEY_DELETE])
      {
         for (c=0;c<20;c++)
            zz[c][zzindx] = 0;
      }

      // if mouse on grid, get current animation sequence pointer
      if ((mouse_y > 30) && (mouse_y < 190) && (mouse_x < 642))
      {
         pointer = (mouse_x-2) / 20 + (mouse_y-30) / 20 * 32;
         if ((pointer < 0) || (pointer > NUM_ANS-1)) pointer = -1;
      }
      else pointer = -1;


      if (mouse_b1)
      {
         if (pointer != -1) zzindx = pointer; // mouse is on map and pointer is valid

         // edit delay
         if ( (mouse_y > 262) && (mouse_y < 272) && (mouse_x > 200) && (mouse_x < 220) )
            zz[3][zzindx] = edit_int(190, 265, zz[3][zzindx], 1, 0, 100);

         while (mouse_b1) proc_controllers(); // wait for release

         if ((mouse_x > button_x1) && (mouse_x < button_x2) && (mouse_y > button_y) && (mouse_y < button_y+(6*12) ))
         {
            int mb = (mouse_y - button_y) / 12;
            switch(mb)
            {
               case 1:
                  for (c=0;c<20;c++)
                     zz[c][zzindx] = 0;
                  as_quit = 0;
                  while (!as_quit)
                  {
                     x = select_bitmap_ans(zzindx);
                     if (x == 1) // good return b1
                     {
                        zz[5 + zz[4][zzindx]][zzindx] = bmp_index;
                        zz[4][zzindx]++; // set last shape to point at next
                     }
                     if (x == -1)  // abort esc
                     {
                        zz[4][zzindx]--;
                        as_quit=1;
                     }
                     if (zz[4][zzindx] > 14) // last shape
                     {
                        zz[4][zzindx]--;
                        as_quit=1;
                     }
                  }
               break;
               case 3: select_bitmap_proc(); break;
               case 4: copy_bitmap_proc(); break;
               case 5: save_sprit(); break;
            }
         }
      }


      update_animation();

      while ((key[ALLEGRO_KEY_ESCAPE]) || (mouse_b2))
      {
         proc_controllers();
         quit = 1;
      }

   }
   return 0;
}



int select_bitmap_proc()
{
   extern int bmp_index;
   int x, y;
   int quit = 0;
   int view_attrib = 0;
   int button_x1 = 400;
   int button_x2 = 639;
   int button_xc = 520;
   int button_y = 650;
   int jh;
   int b2mode = 0;

   al_set_target_backbuffer(display);
   al_show_mouse_cursor(display);

   while (!quit)
   {
      proc_controllers();
      al_flip_display();
      al_clear_to_color(al_map_rgb(0,0,0));
      al_draw_text(font, palette_color[9], 0, 700, 0, "Select a Bitmap with b1");

      // draw 32x32 bitmaps
      for (x = 0; x < 32; x++)
         for (y = 0; y < 32; y++)
         {
            al_draw_bitmap(memory_bitmap[x+(y*32)],x*20, y*20, 0);
            int color = 240; // normal color = white
            if (view_attrib)
            {
               if (sa[x+(y*32)][1]) color = 10; // locked color = red
               if (sa[x+(y*32)][0] == 0) sprintf(msg, "E");
               if (sa[x+(y*32)][0] == 1) sprintf(msg, "B");
               if (sa[x+(y*32)][0] == 2) sprintf(msg, "S");
               al_draw_text(font, palette_color[color], (x*20)+4, (y*20)+4, 0, msg);
            }
         }

      if ((mouse_y < 640) && (mouse_x < 640))
      {
         int pointer = (mouse_x/20) + (mouse_y/20) * 32 ;
         al_draw_textf(font, palette_color[4], 0, 680, 0, "pointer %-2d  ", pointer );
         al_draw_bitmap(memory_bitmap[pointer], 95, 671, 0);

         if (mouse_b1)
         {
            if (view_attrib)
            {
               int bx1 = mouse_x/20;
               int by1 = mouse_y/20;
               int bx2 = mouse_x/20;
               int by2 = mouse_y/20;
               int old_mouse_x = mouse_x;
               int old_mouse_y = mouse_y;
               int cx;
               int cy;
               while (mouse_b1) // trap while b1 is held
               {
                  bx2 = bx1 + ((mouse_x - old_mouse_x)/20);
                  by2 = by1 + ((mouse_y - old_mouse_y)/20);
                  al_draw_rectangle((bx1)*20, (by1)*20, (bx2)*20, (by2)*20, palette_color[15], 1);
                  al_flip_display();
                  proc_controllers();
               }

               // limits
               if (bx1>32) bx1 = 32;
               if (bx2>32) bx2 = 32;
               if (by1>32) by1 = 32;
               if (by2>32) by2 = 32;
               // ensure top-right, bottom left format
               if (bx1 > bx2)
               {
                  int temp = bx2;
                  bx2 = bx1;
                  bx1= temp;
               }
               if (by1 > by2)
               {
                  int temp = by2;
                  by2 = by1;
                  by1= temp;
               }
               // cycle the selection
               for (cx = bx1; cx < bx2; cx++)
                  for (cy = by1; cy < by2; cy++)
                  {
                     if (b2mode< 3) sa[cx+(cy*32)][0] = b2mode;
                     if ((b2mode> 2) && (b2mode < 5)) sa[cx+(cy*32)][1] = b2mode-3;
                  }
            }
            if (!view_attrib)  // old b1 behaviour
            {
               bmp_index = (mouse_x/20) + (mouse_y/20) * 32;
               quit = 1;
            }
         }
      }

      jh=1;
      al_draw_rectangle(button_x1, button_y+(jh*12), button_x2, button_y+(jh*12)+10, palette_color[15], 1);
      if (view_attrib) sprintf(msg,"View Attributes:ON ");
      else sprintf(msg,"View Attributes:OFF");
      al_draw_text(font, palette_color[13], button_xc, button_y+(jh*12)+1, ALLEGRO_ALIGN_CENTER, msg);

      jh=2;
      al_draw_rectangle(button_x1, button_y+(jh*12), button_x2, button_y+(jh*12)+10, palette_color[15], 1);
      if (b2mode == 0) sprintf(msg,"  Empty  ");
      if (b2mode == 1) sprintf(msg,"  Block  ");
      if (b2mode == 2) sprintf(msg," Special ");
      if (b2mode == 3) sprintf(msg," Unlock ");
      if (b2mode == 4) sprintf(msg,"  Lock  ");
      al_draw_text(font, palette_color[13], button_xc, button_y+(jh*12)+1, ALLEGRO_ALIGN_CENTER, msg);

      if (mouse_b1)
      {
         while (mouse_b1) proc_controllers(); // wait for release
         if ((mouse_x > button_x1) && (mouse_x < button_x2) && (mouse_y > button_y) && (mouse_y < button_y+(4*12) ))
         {
            int mb = (mouse_y - button_y) / 12;
            switch(mb)
            {
               case 1: view_attrib = !view_attrib; break;
               case 2: if (++b2mode>4) b2mode = 0; break;
               break;
            }
         }
      }
      while ((key[ALLEGRO_KEY_ESCAPE]) || (mouse_b2))
      {
         proc_controllers();
         quit = 1;
      }
   }
   return 0;
}


int copy_bitmap_proc()
{
   extern int bmp_index;
   int quit = 0;

   al_set_target_backbuffer(display);
   al_show_mouse_cursor(display);

   while (!quit)
   {

      proc_controllers();
      al_flip_display();
      al_clear_to_color(al_map_rgb(0,0,0));

      al_draw_text(font, palette_color[9], 0, 694, 0, "b1 to put   ");
      al_draw_text(font, palette_color[9], 0, 702, 0, "b2 to get   ");
      al_draw_text(font, palette_color[9], 0, 710, 0, "esc to quit ");

      // draw 32x32 bitmaps
      for (int y = 0; y < 32; y++)
         for (int x = 0; x < 32; x++)
            al_draw_bitmap(memory_bitmap[x+(y*32)], x*20, y*20, 0);


      if ((mouse_y < 640) && (mouse_x < 640))
      {
         int pointer = (mouse_x/20) + (mouse_y/20) * 32 ;
         al_draw_textf(font, palette_color[4], 0, 680, 0, "pointer %-2d  ", pointer );
         al_draw_bitmap(memory_bitmap[pointer], 95, 671, 0);

         if (mouse_b2) bmp_index = pointer;
         if (mouse_b1)
         {
            al_set_target_bitmap(memory_bitmap[pointer]);
             //blit (memory_bitmap[bmp_index], memory_bitmap[pointer], 0, 0, 0, 0, 20, 20);
            al_draw_bitmap(memory_bitmap[bmp_index], 0, 0, 0);
            al_set_target_backbuffer(display);
         }
      }

      while (key[ALLEGRO_KEY_ESCAPE])
      {
         proc_controllers();
         quit = 1;
      }
   }
   return 0;
}
