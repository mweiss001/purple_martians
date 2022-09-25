// e_fnx.cpp

#include "pm.h"


/*

int test = 0;
printBits(4, &test);
printf("b4:[%s]\n",msg);

test |= PM_LIFT_NO_DRAW;
printBits(4, &test);
printf("af:'%s'\n",msg);

b4:[00000000 00000000 00000000 00000000 ]
af:'00001000 00000000 00000000 00000000 '

*/




void draw_block_non_default_flags(int tn, int x, int y)
{
   int c = tn & 1023;
   al_draw_bitmap(btile[c], x, y, 0);
   if ((sa[c][0] & PM_BTILE_MOST_FLAGS) != (tn & PM_BTILE_MOST_FLAGS))
   {
      al_draw_line(x, y, x+20, y+20, palette_color[10], 1);
      al_draw_line(x+20, y, x, y+20, palette_color[10], 1);
   }
}


int enforce_limit(int val, int ll, int ul)
{
   if (val < ll) val = ll;
   if (val > ul) val = ul;
   return val;
}

int check_limit(int val, int ll, int ul)
{
   if (val < ll) return 1;
   if (val > ul) return 1;
   return 0;
}






void swap_int(int *i1, int* i2)
{
   int it = *i1;
   *i1 = *i2;
   *i2 = it;
}


float mdw_rnd(float rmin, float rmax)
{
   int imin = (int) (rmin * 100.0);
   int irng = (int) ((rmax-rmin) * 100.0);
   int s = imin + rand() % irng;
   float res = (float)s / 100.0;
   return res;
}









// packs two 16 bit ints into one 32 bit int
void set_int_3216(int &I32, int H16, int L16)
{
   int err = 0;

   if (H16 > 65535)
   {
      printf("error: H16 > 65535");
      err = 1;
   }

   if (H16 < 0)
   {
      printf("error: H16 < 0");
      err = 1;
   }

   if (L16 > 65535)
   {
      printf("error: L16 > 65535");
      err = 1;
   }

   if (L16 < 0)
   {
      printf("error: L16 < 0");
      err = 1;
   }

   if (!err) I32 = (H16<<16) + L16;
}


// extracts two 16 bit ints from one 32 bit int
void get_int_3216(int I32, int &H16, int &L16)
{
   H16 = I32 >> 16; // shift high into low
   H16 = H16 & 0b00000000000000001111111111111111; // clear high

   L16 = I32 & 0b00000000000000001111111111111111; // clear high
}









void printBits(size_t const size, void const * const ptr)
{
   char st[256] = {0};
   int sc = 0;

   unsigned char *b = (unsigned char*) ptr;
   unsigned char byte;
   int i, j;

   for (i = size-1; i >= 0; i--)
   {
      for (j = 7; j >= 0; j--)
      {
         byte = (b[i] >> j) & 1;
         st[sc] = byte+48;
         sc++;
      }
      st[sc] = 32;
      sc++;
   }
   st[sc] = 0;
   sprintf(msg, "%s", st);
}

al_fixed get_sproingy_jump_height(int num)
{
   al_fixed t1 = al_fixdiv(al_itofix(item[num][7]), al_ftofix(7.1));
   al_fixed t2 = al_itofix(-15); // jump starts not at zero, but at almost one block height off ground
   while (t1 > al_itofix(0))
   {
      t2 += t1; // distance moved this time period
      t1 -= al_ftofix(.2); // minus slow gravity
   }
   return t2;
}
/*
float get_jh(int num)
{
   float t1 = (float) num / 7.1;
   float t2 = -15;

   while (t1 > 0)
   {
      t2 += t1;  // distance moved this time period
      t1 -= 0.2; // minus slow gravity
   }
   return t2;
}
*/
// used for calculating sproinginess from jump height
// used only for map move of sproingy jump height
// misses some numbers
// 1933 = 197
// 1934 = 199
int get_sp(float jh)
{
   float t1 = 0;
   float t2 = 0;
   while (t1 < (jh - 15))
   {
      t1 += t2;
      t2 += 0.2;
   }
   return (int) round(t2*7.1);
}






// used only in sliders for button set new direction (cannon and podzilla)
void set_xyinc_rot(int EN, int x2, int y2)
{
   al_fixed xlen = al_itofix(x2) - Efi[EN][0];      // get the x distance between enemy and x2
   al_fixed ylen = al_itofix(y2) - Efi[EN][1];      // get the y distance between enemy and y2
   al_fixed hy_dist =  al_fixhypot(xlen, ylen);     // hypotenuse distance
   al_fixed speed = Efi[EN][5];                  // speed
   al_fixed scaler = al_fixdiv(hy_dist, speed);     // get scaler
   al_fixed xinc = al_fixdiv(xlen, scaler);         // calc xinc
   al_fixed yinc = al_fixdiv(ylen, scaler);         // calc yinc
   Efi[EN][2] = xinc;
   Efi[EN][3] = yinc;
   Efi[EN][14] = al_fixatan2(ylen, xlen) - al_itofix(64);
}

// used only in sliders for button set new direction (rocket)
void set_rocket_rot(int num, int x2, int y2)
{
   al_fixed xlen = al_itofix(x2) - al_itofix(item[num][4]);      // get the x distance between item and x2
   al_fixed ylen = al_itofix(y2) - al_itofix(item[num][5]);      // get the y distance between item and y2
   al_fixed rot = al_fixatan2(ylen, xlen) + al_itofix(64);
   item[num][10] = al_fixtoi(rot) * 10;
}

int get_block_range(const char *txt, int *x1, int *y1, int *x2, int *y2, int type)
{
   init_level_background();
   int tx = SCREEN_W/2;
   int quit = 0;
   int ret = 0;
   while (!quit)
   {
      cm_redraw_level_editor_background(0);
      crosshairs_full(gx*20+10, gy*20+10, 15, 1);
      get_new_screen_buffer(3, 0, 0);

      al_draw_filled_rectangle(tx-90, 70, tx+90, 170, palette_color[0]);
      al_draw_rectangle(       tx-90, 70, tx+90, 170, palette_color[15], 1);

      al_draw_text(font, palette_color[9],   tx, 72,  ALLEGRO_ALIGN_CENTER, "Draw a new");
      al_draw_text(font, palette_color[10],  tx, 80,  ALLEGRO_ALIGN_CENTER, txt);
      al_draw_text(font, palette_color[9],   tx, 88,  ALLEGRO_ALIGN_CENTER, "by clicking and");
      al_draw_text(font, palette_color[9],   tx, 96,  ALLEGRO_ALIGN_CENTER, "dragging with the");
      al_draw_text(font, palette_color[9],   tx, 104, ALLEGRO_ALIGN_CENTER, "left mouse button");
      al_draw_text(font, palette_color[14],  tx, 130, ALLEGRO_ALIGN_CENTER, "Cancel with <ESC>");
      al_draw_text(font, palette_color[14],  tx, 138, ALLEGRO_ALIGN_CENTER, "or right mouse button");
      al_draw_textf(font, palette_color[15], tx, 150, ALLEGRO_ALIGN_CENTER, "x:%2d y:%2d", gx, gy);

      if (mouse_b1)
      {
         cm_get_new_box();
         *x1 = bx1*20;
         *y1 = by1*20;
         *x2 = (bx2-bx1)*20+20;
         *y2 = (by2-by1)*20+20;

         if (type == 2)
         {
            *x2 = (bx2-bx1)*20;
            *y2 = (by2-by1)*20;
         }

         quit = 1;
         ret = 1;
      }
      while ((mouse_b2) || (key[ALLEGRO_KEY_ESCAPE]))
      {
         proc_controllers(); // wait for release
         quit = 1;
         ret = 0;
      }
   }
   return ret;
}










































void draw_vinepod_controls(int num, int legend_highlight)
{
   // enforce that 34 is 01
   Ei[num][3] = al_fixtoi(Efi[num][0]);
   Ei[num][4] = al_fixtoi(Efi[num][1]);

   // put variables in spline array
   float pnts[8];
   for (int i=0; i<8; i++) pnts[i] = Ei[num][i+3]+10;

   al_draw_spline(pnts, palette_color[10], 0);

   // fill array of points from the spline
   float dest[200];
   al_calculate_spline(dest, 8, pnts, 0, 100);

   // initial position
   int ipx = Ei[num][3];
   int ipy = Ei[num][4];

   // set initial rotation
   al_fixed xlen = al_ftofix(dest[4] - dest[0]);            // get the x distance
   al_fixed ylen = al_ftofix(dest[5] - dest[1]);            // get the y distance
   Efi[num][14] = al_fixatan2(ylen, xlen) - al_itofix(64);  // rotation

   // extended position
   int color1 = 10;
   if (legend_highlight == 2) color1 = flash_color;
   int epx = Ei[num][9];
   int epy = Ei[num][10];
   crosshairs_full(epx+10, epy+10, color1, 1);

   al_draw_textf(f3, palette_color[15], epx+22, epy+0, 0, "x:%d", epx-ipx);
   al_draw_textf(f3, palette_color[15], epx+22, epy+8, 0, "y:%d", epy-ipy);

   // set extended rotation
   xlen = al_ftofix(dest[198] - dest[194]);                     // get the x distance
   ylen = al_ftofix(dest[199] - dest[193]);                     // get the y distance
   al_fixed ext_rot = al_fixatan2(ylen, xlen) - al_itofix(64);  // rotation

   // draw tile at extended pos
   float rot = al_fixtof(al_fixmul(ext_rot, al_fixtorad_r));
   al_draw_scaled_rotated_bitmap(tile[Ei[num][1]], 10, 10, epx+10, epy+10, 1, 1, rot, ALLEGRO_FLIP_HORIZONTAL);

   // control point 1
   color1 = 6;
   if (legend_highlight == 3) color1 = flash_color;
   int px = Ei[num][5];
   int py = Ei[num][6];
   al_draw_line(ipx+10, ipy+10, px+10, py+10, palette_color[color1], 0);
   al_draw_line(epx+10, epy+10, px+10, py+10, palette_color[color1], 0);
   al_draw_filled_circle(px+10, py+10, 3, palette_color[color1]);
   al_draw_circle(px+10, py+10, 6, palette_color[color1], 1);
   al_draw_textf(f3, palette_color[15], px+20, py+0, 0, "x:%d", px-ipx);
   al_draw_textf(f3, palette_color[15], px+20, py+8, 0, "y:%d", py-ipy);



   // control point 2
   color1 = 7;
   if (legend_highlight == 4) color1 = flash_color;
   px = Ei[num][7];
   py = Ei[num][8];
   al_draw_line(ipx+10, ipy+10, px+10, py+10, palette_color[color1], 0);
   al_draw_line(epx+10, epy+10, px+10, py+10, palette_color[color1], 0);
   al_draw_filled_circle(px+10, py+10, 3, palette_color[color1]);
   al_draw_circle(px+10, py+10, 6, palette_color[color1], 1);
   al_draw_textf(f3, palette_color[15], px+20, py+0, 0, "x:%d", px-ipx);
   al_draw_textf(f3, palette_color[15], px+20, py+8, 0, "y:%d", py-ipy);

   // trigger box
   int color = 14;
   if (legend_highlight == 5) color = flash_color;
   int tx1 = Ei[num][11];
   int ty1 = Ei[num][12];
   int tx2 = Ei[num][11]+Ei[num][13] + 20;
   int ty2 = Ei[num][12]+Ei[num][14] + 20;
   al_draw_rectangle(tx1, ty1, tx2, ty2, palette_color[color], 1);


}









int getxy(const char *txt, int obj_type, int sub_type, int num)
{
   int tx = SCREEN_W/2;

   // in case these are needed for lifts
   int lift = sub_type;
   int step = num;

   int retval=0;
   int quit=0;

   while (mouse_b1) proc_controllers(); // wait for release


   // get original values in case we are cancelled and need to restore them
   int original_dx=0, original_dy=0;
   al_fixed original_rot  = al_itofix(0); // for direction
   al_fixed original_xinc = al_itofix(0);
   al_fixed original_yinc = al_itofix(0);


   if (obj_type == 2) // items
   {
      original_dx = item[num][4];
      original_dy = item[num][5];
   }
   if (obj_type == 3) // enemies
   {
      original_dx = al_fixtoi(Efi[num][0]);
      original_dy = al_fixtoi(Efi[num][1]);
   }
   if (obj_type == 99) // pod extended
   {
      original_dx = al_fixtoi(Efi[num][5]);
      original_dy = al_fixtoi(Efi[num][6]);
   }
   if (obj_type == 98) // cloner destination
   {
      original_dx = Ei[num][17];
      original_dy = Ei[num][18];
   }
   if (obj_type == 97) // rocket initial direction
   {
      original_rot = item[num][10];
   }
   if (obj_type == 96) // cannon or bouncer initial direction
   {
      original_rot  = Efi[num][14];
      original_xinc = Efi[num][2];
      original_yinc = Efi[num][3];
   }
   if (obj_type == 95) // message display location
   {


      original_dx = item[num][10];
//      original_dy = item[num][11];



   }

   if (obj_type == 90) // vinepod extended
   {
      original_dx = Ei[num][9];
      original_dy = Ei[num][10];
   }
   if (obj_type == 91) // vinepod cp1
   {
      original_dx = Ei[num][5];
      original_dy = Ei[num][6];
   }
   if (obj_type == 92) // vinepod cp2
   {
      original_dx = Ei[num][7];
      original_dy = Ei[num][8];
   }



   if (obj_type == 4) // lift
   {
      original_dx = lift_steps[lift][step].x;
      original_dy = lift_steps[lift][step].y;
   }

   while(!quit)
   {
      cm_redraw_level_editor_background(0);

      crosshairs_full(gx*20+10, gy*20+10, 15, 1);

      if (obj_type == 99) // move pod extended
      {
         int ex = al_fixtoi(Efi[num][0])+10;
         int ey = al_fixtoi(Efi[num][1])+10;
         int px = al_fixtoi(Efi[num][5])+10;
         int py = al_fixtoi(Efi[num][6])+10;
         float rot = al_fixtof(al_fixmul(Efi[num][14], al_fixtorad_r));
         al_draw_scaled_rotated_bitmap(tile[Ei[num][1]], 10, 10, px, py, 1, 1, rot, ALLEGRO_FLIP_HORIZONTAL); // draw tile at extended pos
         al_draw_line(ex, ey, px, py, palette_color[10], 1); // connect with line
      }

      if ((obj_type == 90) || (obj_type == 91) || (obj_type == 92)) draw_vinepod_controls(num, -1); // move vinepod extended, cp1, cp2


      if (obj_type == 98) // move cloner destination
      {
         float dx1 = (float)Ei[num][17];
         float dy1 = (float)Ei[num][18];
         float dx2 = dx1 + (float)Ei[num][19];
         float dy2 = dy1 + (float)Ei[num][20];
         int dc1 = 10 + 128; // destination box color
         rectangle_with_diagonal_lines(dx1, dy1, dx2, dy2, 8, dc1, dc1+64); // destination
      }
      if (obj_type == 97) // set new rocket direction
      {
         int ix = item[num][4]+10;
         int iy = item[num][5]+10;
         int dx = gx*20+10;
         int dy = gy*20+10;
         float rot = al_fixtof(al_fixmul(al_itofix(item[num][10]/10), al_fixtorad_r));
         al_draw_rotated_bitmap(tile[item[num][1]], 10, 10, dx, dy, rot, 0);
         al_draw_line(ix, iy, dx, dy, palette_color[10], 1);     // connect with line
      }
      if (obj_type == 96) // set cannon or bouncer direction
      {
         int ex = al_fixtoi(Efi[num][0])+10;
         int ey = al_fixtoi(Efi[num][1])+10;
         int dx = gx*20+10;
         int dy = gy*20+10;
         float rot = al_fixtof(al_fixmul(Efi[num][14], al_fixtorad_r));
         al_draw_scaled_rotated_bitmap(tile[Ei[num][1]], 10, 10, dx, dy, 1, 1, rot, ALLEGRO_FLIP_HORIZONTAL); // draw tile
         al_draw_line(ex, ey, dx, dy, palette_color[10], 1);   // connect with line
      }

      if (obj_type == 95) // set message location
      {
         // show all messages
         for (int i=0; i<500; i++)
            if (item[i][0] == 10) draw_pop_message(i, 0, 0, 0, 0, msg);
      }



      get_new_screen_buffer(3, 0, 0);

      al_draw_filled_rectangle(tx-100, 70, tx+100, 128, palette_color[0]);
      al_draw_rectangle(       tx-100, 70, tx+100, 128, palette_color[15], 1);

      al_draw_text(font,  palette_color[9],   tx, 72,  ALLEGRO_ALIGN_CENTER, "Set new Location for:");
      al_draw_text(font,  palette_color[10],  tx, 80,  ALLEGRO_ALIGN_CENTER, txt);
      al_draw_text(font,  palette_color[9],   tx, 88,  ALLEGRO_ALIGN_CENTER, "with left mouse button");
      al_draw_text(font,  palette_color[14],  tx, 110, ALLEGRO_ALIGN_CENTER, "Cancel with <ESC>");
      al_draw_text(font,  palette_color[14],  tx, 118, ALLEGRO_ALIGN_CENTER, "or right mouse button");

      al_flip_display();


      switch (obj_type)
      {
         case 2: // items
            item[num][4] = gx*20;
            item[num][5] = gy*20;
            itemf[num][0] = al_itofix(gx*20);
            itemf[num][1] = al_itofix(gy*20);
         break;
         case 3: // show enem
            Efi[num][0] = al_itofix(gx*20);
            Efi[num][1] = al_itofix(gy*20);
         break;
         case 4: // show lift
         {
            lift_steps[lift][step].x = gx*20;
            lift_steps[lift][step].y = gy*20;
            set_lift_to_step(lift, step);   // set current step in current lift
         }
         break;
         case 99: // move pod extended
         {
            Efi[num][5] = al_itofix(gx*20);
            Efi[num][6] = al_itofix(gy*20);
            recalc_pod(num);
         }
         break;
         case 98: // cloner destination
         {
            Ei[num][17] = gx*20;
            Ei[num][18] = gy*20;
         }
         break;
         case 97: // set new rocket direction
         {
            set_rocket_rot(num, gx*20, gy*20);
         }
         break;
         case 96: // set cannon or bouncer direction
         {
            set_xyinc_rot(num, gx*20, gy*20);
         }
         break;
         case 95: // message position
         {
//            item[num][10] = gx*20;
//            item[num][11] = gy*20;


            set_int_3216(item[num][10], gx*20, gy*20);





         }
         case 90: // vinepod extended
         {
            Ei[num][9] = gx*20;
            Ei[num][10] = gy*20;
         }
         break;
         case 91: // vinepod cp1
         {
            Ei[num][5] = gx*20;
            Ei[num][6] = gy*20;
         }
         break;
         case 92: // vinepod cp2
         {
            Ei[num][7] = gx*20;
            Ei[num][8] = gy*20;
         }
         break;
      }


      while (mouse_b1)
      {
         proc_controllers();
         quit = 1;
         retval = 1;  // b1 xy
      }
      while (mouse_b2)
      {
         proc_controllers();
         quit = 1;
         retval = 2;  // b2 xy
      }
      while (key[ALLEGRO_KEY_ESCAPE])
      {
         proc_controllers();
         quit = 1;
         retval = 0;  // ignore xy
      }
   } // end of while(!quit);


   if (gx > 99) gx = 99;
   if (gy > 99) gy = 99;

   if (retval != 1) // restore old positions if cancelled
   {
      if (obj_type == 2)
      {
         item[num][4] = original_dx;
         item[num][5] = original_dy;
         itemf[num][0] = al_itofix(original_dx);
         itemf[num][1] = al_itofix(original_dy);
      }
      if (obj_type == 3)
      {
          Efi[num][0] = al_itofix(original_dx);
          Efi[num][1] = al_itofix(original_dy);
      }
      if (obj_type == 90)
      {
          Ei[num][9] = original_dx;
          Ei[num][10] = original_dy;
      }
      if (obj_type == 91)
      {
          Ei[num][5] = original_dx;
          Ei[num][6] = original_dy;
      }
      if (obj_type == 92)
      {
          Ei[num][7] = original_dx;
          Ei[num][8] = original_dy;
      }
      if (obj_type == 99)
      {
          Efi[num][5] = al_itofix(original_dx);
          Efi[num][6] = al_itofix(original_dy);
      }
      if (obj_type == 98)
      {
          Efi[num][5] = original_dx;
          Efi[num][6] = original_dy;
      }
      if (obj_type == 97) // restore rocket direction
      {
         item[num][10] = original_rot;
      }
      if (obj_type == 96) // cannon or bouncer initial direction
      {
         Efi[num][14] = original_rot;
         Efi[num][2]  = original_xinc;
         Efi[num][3]  = original_yinc;
      }

      if (obj_type == 95) // message display position
      {
          item[num][10] = original_dx;
       //   item[num][11] = original_dy;
      }
      if (obj_type == 4)
      {
         lift_steps[lift][step].x = original_dx;
         lift_steps[lift][step].y = original_dy;
      }
   }
   return retval;
}










void show_event_line(int x, int &y, int ev, int type, int v1, int v2)
{
   if (type == 1) // item
   {
      if (item[v1][0] == 6)  al_draw_textf(font, palette_color[7],  x, y, 0, "ev:%2d - item:%3d [orb] ", ev, v1);
      if (item[v1][0] == 9)  al_draw_textf(font, palette_color[14], x, y, 0, "ev:%2d - item:%3d [trg] ", ev, v1);
      if (item[v1][0] == 16) al_draw_textf(font, palette_color[13], x, y, 0, "ev:%2d - item:%3d [bm]  ", ev, v1);
      if (item[v1][0] == 17) al_draw_textf(font, palette_color[10], x, y, 0, "ev:%2d - item:%3d [bd]  ", ev, v1);
   }
   if (type == 2) // lift
   {
      al_draw_textf(font, palette_color[15], x, y, 0, "ev:%2d - lift:%3d step:%d", ev, v1, v2);
   }
   y+=8;
}


void show_all_events(void)
{
   int x = 0, y = 20;
   al_set_target_backbuffer(display);
   al_clear_to_color(al_map_rgb(0,0,0));

   al_draw_textf(font, palette_color[15], 0, 0, 0, "All non-zero event references");

   for (int i=0; i<500; i++)
   {
      if (item[i][0] == 6) // orb
      {
         if (item[i][10]) show_event_line(x, y, item[i][10], 1, i, 0);
         if (item[i][11]) show_event_line(x, y, item[i][11], 1, i, 0);
         if (item[i][12]) show_event_line(x, y, item[i][12], 1, i, 0);
         if (item[i][13]) show_event_line(x, y, item[i][13], 1, i, 0);
      }
      if (item[i][0] == 9) // trigger
      {
         if (item[i][11]) show_event_line(x, y, item[i][11], 1, i, 0);
         if (item[i][12]) show_event_line(x, y, item[i][12], 1, i, 0);
         if (item[i][13]) show_event_line(x, y, item[i][13], 1, i, 0);
         if (item[i][14]) show_event_line(x, y, item[i][14], 1, i, 0);
      }


      if ((item[i][0] == 16) && (item[i][1])) show_event_line(x, y, item[i][1], 1, i, 0);
      if ((item[i][0] == 17) && (item[i][1])) show_event_line(x, y, item[i][1], 1, i, 0);
   }

   for (int l=0; l<num_lifts; l++) // iterate lifts
      for (int s=0; s<lifts[l].num_steps; s++) // iterate steps
         if (((lift_steps[l][s].type & 31) == 5) && (lift_steps[l][s].val)) show_event_line(x, y, lift_steps[l][s].val, 2, l, s);




   al_flip_display();
   tsw(); // wait for keypress
}











int add_item_link_translation(int sel_item_num, int sel_item_var, int sel_item_ev, int clt[][4], int clt_last)
{
   if (sel_item_ev)
   {
      // check if this event already has a translation and get it if it does
      int ev2 = check_clt_for_event(sel_item_ev, clt, clt_last);

      if (ev2) // existing translation found
      {
         clt[clt_last][0] = sel_item_num;   // item # in selection
         clt[clt_last][1] = sel_item_var;   // item var #
         clt[clt_last][2] = sel_item_ev;    // original link
         clt[clt_last][3] = ev2; // new link

         printf("et %d %d %d %d\n", clt[clt_last][0], clt[clt_last][1], clt[clt_last][2], clt[clt_last][3]);

      }
      else // no existing translation found
      {
         ev2 = get_unused_pm_event_extended(clt, clt_last);
         clt[clt_last][0] = sel_item_num;   // item # in selection
         clt[clt_last][1] = sel_item_var;   // item var #
         clt[clt_last][2] = sel_item_ev;    // original link
         clt[clt_last][3] = ev2; // new link
         printf("ne %d %d %d %d\n", clt[clt_last][0], clt[clt_last][1], clt[clt_last][2], clt[clt_last][3]);
      }
   }
   else return 0; // nothing added
   return 1; // added
}



void clear_pm_events(void)
{
   for (int x=0; x<1000; x++) pm_event[x] = 0;
}




// check the clt list for an existing pm_event
// return the translation used or 0 if none found
int check_clt_for_event(int ev, int clt[][4], int clt_last)
{
   for (int i=0; i<clt_last; i++)
   {
      if (clt[i][2] == ev) return clt[i][3];
   }
   return 0;
}


int is_pm_event_used(int ev)
{
   for (int i=0; i<500; i++)
   {
      if (item[i][0] == 6) // orb
      {
         if (item[i][10] == ev) return 1;
         if (item[i][11] == ev) return 1;
         if (item[i][12] == ev) return 1;
         if (item[i][13] == ev) return 1;
      }
      if (item[i][0] == 9) // trigger
      {
         if (item[i][11] == ev) return 1;
         if (item[i][12] == ev) return 1;
         if (item[i][13] == ev) return 1;
         if (item[i][14] == ev) return 1;
      }


      if ((item[i][0] == 16) && (item[i][1] == ev)) return 1;
      if ((item[i][0] == 17) && (item[i][1] == ev)) return 1;
   }

   for (int l=0; l<num_lifts; l++) // iterate lifts
      for (int s=0; s<lifts[l].num_steps; s++) // iterate steps
         if (((lift_steps[l][s].type & 31) == 5) && (lift_steps[l][s].val == ev)) return 1;

   return 0;
}

int get_unused_pm_event_extended(int clt[][4], int clt_last)
{
   int ev = 1; // starting event to test (don't ever use event 0)
   int done = 0;

   while (!done)
   {
      int used = 0;

      if (is_pm_event_used(ev)) used = 1; // first check if used in main item item array

      for (int i=0; i<clt_last; i++) if (clt[i][3] == ev) used = 1; // then check the translation table also

      if (used == 0) return ev; // found one!
      else if (++ev > 999) done = 1;
   }
   return 0; // only if no unused can be found
}



int get_unused_pm_event(void)
{
   int ev = 1; // don't ever use event 0
   int done = 0;
   while (!done)
   {
      if (!is_pm_event_used(ev)) return ev;
      else if (++ev > 999) done = 1;
   }
   return 0; // only if no unused can be found
}


void find_and_show_event_links(int type, int i, int num2)
{
   if (type == 1) // trigger
   {
      int x1 = item[i][4]+10;
      int y1 = item[i][5]+10;
      for (int en=11; en<15; en++) // iterate all 4 trigger events
      {
         int ev = item[i][en];
         if (ev > 0) // don't do anything for link if zero
         {
            for (int i2=0; i2<500; i2++)
               if ( ((item[i2][0] == 16) || (item[i2][0] == 17)) && (item[i2][1] == ev))
               {
                  // found a match with manip or damage
                  int x2 = item[i2][4]+10;
                  int y2 = item[i2][5]+10;
                  al_draw_line(x1, y1, x2, y2, palette_color[10], 2);
               }
            for (int l=0; l<num_lifts; l++) // iterate lifts
               for (int s=0; s<lifts[l].num_steps; s++) // iterate steps
                  if (((lift_steps[l][s].type & 31) == 5) && (lift_steps[l][s].val == ev))
                  {
                     //printf("found a match with a lift:%d step:%d ", l, s);
                     int pms = lift_find_previous_move_step(l, s);
                     int x2 = lift_steps[l][pms].x + lift_steps[l][pms].w / 2;
                     int y2 = lift_steps[l][pms].y + lift_steps[l][pms].h / 2;

                     //printf("pms:%d x:%d y:%d\n", pms, x2, y2);

                     al_draw_line(x1, y1, x2, y2, palette_color[10], 2);
                  }
         }
      }
   }

   if (type == 2)  // block manip or block damage
   {
      int x1 = item[i][4]+10;
      int y1 = item[i][5]+10;
      int ev = item[i][1];
      if (ev > 0) // don't do anything for link if zero
      {
         for (int i2=0; i2<500; i2++)
         {
            // this finds matching triggers for bm and bd
            if ((item[i2][0] == 9) && ((item[i2][11] == ev) || (item[i2][12] == ev) || (item[i2][13] == ev) || (item[i2][14] == ev)))
            {
               // found a match with a trigger
               int x2 = item[i2][4]+10;
               int y2 = item[i2][5]+10;
               al_draw_line(x1, y1, x2, y2, palette_color[10], 2);
            }
         }
      }
   }


   if (type == 3)  // lift
   {
      int lift = i;
      int step = num2;
      int stype = lift_steps[lift][step].type & 31;

      // get position from previous move step
      int pms = lift_find_previous_move_step(lift, step);
      int x1 = lift_steps[lift][pms].x + lift_steps[lift][pms].w / 2;
      int y1 = lift_steps[lift][pms].y + lift_steps[lift][pms].h / 2;

//      printf("current lift:%d step:%d stype:%d  ", i, step, stype );
//      printf("pms:%d x:%d y:%d\n", pms, x1, y1);

      if (stype == 5) // only if this step is "wait for trig"
      {
         int ev = lift_steps[lift][step].val; // get event
         if (ev > 0) // don't do anything for link if zero
         {
            for (int i2=0; i2<500; i2++)
            {
               if ((item[i2][0] == 9) && ((item[i2][11] == ev) || (item[i2][12] == ev) || (item[i2][13] == ev) || (item[i2][14] == ev)))
               {
                  // found a match with a trigger
                  int x2 = item[i2][4]+10;
                  int y2 = item[i2][5]+10;

//                  printf("found event:%d match with trigger:%d x:%d y:%d\n", ev, i2, x2, y2);
//                  printf("x1:%d y1:%d x2:%d y2:%d\n", x1, y1, x2, y2);

                  al_draw_line(x1, y1, x2, y2, palette_color[10], 2);
               }

            }
         }
      }
   }
}



int get_trigger_item(int obj_type, int sub_type, int num )
{
   init_level_background();
   int tx = SCREEN_W/2;

   // in case these are needed for lifts
   int lift = num;
   int step = sub_type;

   int ret_item=0;
   int quit=0;
   int mouse_on_item = 0;

   int x2=0, y2=0, itx=0, ity=0;

   if (obj_type == 2)
   {
      x2 = item[num][4]+10; // get the object position
      y2 = item[num][5]+10;
   }
   if (obj_type == 4)
   {
      int pms = lift_find_previous_move_step(lift, step);
      x2 = lift_steps[lift][pms].x + lift_steps[lift][pms].w / 2;
      y2 = lift_steps[lift][pms].y + lift_steps[lift][pms].h / 2;
   }


   while (mouse_b1) proc_controllers(); // wait for release

   while(!quit)
   {
      cm_redraw_level_editor_background(0);
      crosshairs_full(gx*20+10, gy*20+10, 15, 1);

      if (mouse_on_item) al_draw_line(x2, y2, itx+10, ity+10, palette_color[10], 2);
      else
      {
         if (obj_type == 2) find_and_show_event_links(2, num, 0);
         if (obj_type == 4) find_and_show_event_links(3, lift, step);
      }

      get_new_screen_buffer(3, 0, 0);
      al_draw_filled_rectangle(tx-110, 78, tx+110, 146, palette_color[0]);
      al_draw_rectangle(       tx-110, 78, tx+110, 146, palette_color[15], 1);

      al_draw_text(font,  palette_color[9],  tx, 80,  ALLEGRO_ALIGN_CENTER, "Select a Trigger Object");
      al_draw_text(font,  palette_color[9],  tx, 88,  ALLEGRO_ALIGN_CENTER, "with left mouse button");
      al_draw_text(font,  palette_color[14], tx, 110, ALLEGRO_ALIGN_CENTER, "Cancel with <ESC>");
      al_draw_text(font,  palette_color[14], tx, 118, ALLEGRO_ALIGN_CENTER, "or right mouse button");

      if (mouse_on_item) al_draw_textf(font, palette_color[15], tx, 136, ALLEGRO_ALIGN_CENTER, "Trigger Object:%d", ret_item);
      else               al_draw_textf(font, palette_color[15], tx, 136, ALLEGRO_ALIGN_CENTER, "No Trigger Object Selected");

      mouse_on_item = 0;
      for (int x=0; x<500; x++)
         if ((item[x][0] == 9) || (item[x][0] == 6))    // trigger or orb only
         {
            itx = item[x][4]/20;
            ity = item[x][5]/20;
            if ((gx == itx) && (gy == ity))
            {
               mouse_on_item = 1;
               ret_item = x;
            }
         }

      itx = item[ret_item][4];
      ity = item[ret_item][5];

      while (mouse_b1)
      {
         proc_controllers();
         quit = 1;
      }
      while ((mouse_b2) || (key[ALLEGRO_KEY_ESCAPE]))
      {
         proc_controllers();
         quit = 1;
         ret_item = - 1;
      }
   } // end of while(!quit);
   if (!mouse_on_item) ret_item = -1;
   return ret_item;
}



int get_item(int obj_type, int sub_type, int num )
{
   init_level_background();
   int tx = SCREEN_W/2;

   int ret_item=0;
   int quit=0;
   int mouse_on_item = 0;

   int itx=0, ity=0;

   int x2 = item[num][4]+10; // get the originating door's location
   int y2 = item[num][5]+10;

   while (mouse_b1) proc_controllers(); // wait for release

   while(!quit)
   {
      cm_redraw_level_editor_background(0);
      crosshairs_full(gx*20+10, gy*20+10, 15, 1);

      if (mouse_on_item) al_draw_line(x2, y2, itx+10, ity+10, palette_color[10], 2);

      get_new_screen_buffer(3, 0, 0);

      al_draw_filled_rectangle(tx-110, 78, tx+110, 146, palette_color[0]);
      al_draw_rectangle(       tx-110, 78, tx+110, 146, palette_color[15], 1);

      al_draw_text(font,  palette_color[9],  tx, 80,  ALLEGRO_ALIGN_CENTER, "Select a Door Object");
      al_draw_text(font,  palette_color[9],  tx, 88,  ALLEGRO_ALIGN_CENTER, "with left mouse button");
      al_draw_text(font,  palette_color[14], tx, 110, ALLEGRO_ALIGN_CENTER, "Cancel with <ESC>");
      al_draw_text(font,  palette_color[14], tx, 118, ALLEGRO_ALIGN_CENTER, "or right mouse button");

      if (mouse_on_item) al_draw_textf(font, palette_color[15], tx, 136, ALLEGRO_ALIGN_CENTER, "Door Object:%d", ret_item);
      else               al_draw_textf(font, palette_color[15], tx, 136, ALLEGRO_ALIGN_CENTER, "No Door Object Selected");


      mouse_on_item = 0;
      for (int x=0; x<500; x++)
         if (item[x][0] == 1)    // door only
         {
            itx = item[x][4]/20;
            ity = item[x][5]/20;
            if ((gx == itx) && (gy == ity))
            {
               mouse_on_item = 1;
               ret_item = x;
            }
         }

      itx = item[ret_item][4];
      ity = item[ret_item][5];

      while (mouse_b1)
      {
         proc_controllers();
         quit = 1;
      }
      while ((mouse_b2) || (key[ALLEGRO_KEY_ESCAPE]))
      {
         proc_controllers();
         quit = 1;
         ret_item = - 1;
      }
   } // end of while(!quit);
   if (!mouse_on_item) ret_item = -1;
   return ret_item;
}


void crosshairs_full(int x, int y, int color, int line_width) // draws a square and crosshairs around a full 20x20 block on level buffer
{
   al_draw_rectangle(x-10, y-10, x+11, y+11, palette_color[color], line_width);
   al_draw_line(0,    y, x-10, y, palette_color[color], line_width);
   al_draw_line(x+11, y, 1999, y, palette_color[color], line_width);
   al_draw_line(x,    0, x, y-10, palette_color[color], line_width);
   al_draw_line(x, y+11, x, 1999, palette_color[color], line_width);
}
void titlex(const char *txt, int tc, int fc, int x1, int x2, int y)
{
   for (int x=0; x<15; x++)
      al_draw_line(x1, y+x, x2, y+x, palette_color[fc+(x*16)], 1);
   al_draw_text(font, palette_color[tc], (x1+x2)/2, y+2, ALLEGRO_ALIGN_CENTER,  txt);
}

