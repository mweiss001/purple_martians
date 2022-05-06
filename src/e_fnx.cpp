// e_fnx.cpp

#include "pm.h"








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
//         printf("%u", byte);
         st[sc] = byte+48;
         sc++;



      }
      st[sc] = 32;
      sc++;

   }
//   puts("");
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

void set_wx(int x, int y)
{
   int d;
   wx = x - SCREEN_W/40;
   wy = y - SCREEN_H/40;

   // check limits
   d = 100 - (SCREEN_W/20);
   if (wx>d) wx = d;
   if (wx<0) wx = 0;

   d = 100 - (SCREEN_H/20);
   if (wy>d) wy = d;
   if (wy<0) wy = 0;
}
void set_wx_from_start_block(void)
{
   int x = 0, y = 0;
   for (int c=0; c<500; c++)  // get initial wx, wy from start block
   if (item[c][0] == 5)
   {
      x = item[c][4]/20;
      y = item[c][5]/20;
      break;
   }
   set_wx(x+4, y);
}

void show_big(void)
{
   al_draw_bitmap(lefsm, 0, 0, 0);
}

void draw_big(int draw_lifts)
{
   init_level_background(); // fill level_background with blocks and lift lines
   draw_level2(level_background, 0, 0, 2000, 1, 1, 1, 1, 0);
   draw_level2(lefsm, 0, 0, db*100, 1, 1, 1, draw_lifts, 0);
   al_set_target_backbuffer(display);
}

void draw_bs(int cc)
{
   ALLEGRO_BITMAP *jtemp = NULL;

   // get mouse pos
   int dx = mouse_x/db;
   int dy = mouse_y/db;

   // set bullseye map size
   int ssz = 180;
   if (db == 4) ssz = 220;
   if (db == 6) ssz = 180;
   if (db > 7) ssz = 300;
   if (db > 9) ssz = 300;


   int ccz = (((ssz/20)-1)/2); // 7 = 3, 5 = 2, 3 = 1

   int smx = -ssz/2 + (db*100) + (SCREEN_W-(db*100)) / 2;
   int smy = (db*100)-ssz-2;

   int ex = dx*20 - (ssz/2-10);
   int ey = dy*20 - (ssz/2-10);

   if ((dx<100) && (dy < 100))
   {
      // get background from level_background for bullseye map sized ssz x ssz
      jtemp = al_create_bitmap(ssz, ssz);
      al_set_target_bitmap(jtemp);
      al_draw_bitmap_region(level_background, ex, ey, ssz, ssz, 0, 0, 0);

      // clear edges if necessary
      if (dx < ccz)    al_draw_filled_rectangle(0,   0,  ((ccz-dx)*20)-1,    ssz-1,   palette_color[0]);
      if (dy < ccz)    al_draw_filled_rectangle(0,   0,   ssz-1,   ((ccz-dy)*20)-1,   palette_color[0]);
      if (dx > 99-ccz) al_draw_filled_rectangle(ssz-(ccz-(99-dx))*20,  0,  ssz-1,  ssz-1,   palette_color[0]);
      if (dy > 99-ccz) al_draw_filled_rectangle(0,  ssz-(ccz-(99-dy))*20,  ssz-1,  ssz-1,   palette_color[0]);

      // draw red bullseye
      al_draw_line(       0,   ssz/2-10,     ssz-1,  ssz/2-10,  palette_color[10],1);
      al_draw_line(       0,   ssz/2+11,     ssz-1,  ssz/2+11,  palette_color[10],1);
      al_draw_line(ssz/2-10,          0,  ssz/2-10,     ssz-1,  palette_color[10],1);
      al_draw_line(ssz/2+11,          0,  ssz/2+11,     ssz-1,  palette_color[10],1);

      al_set_clipping_rectangle(display_transform_double*db*100+1, 0, SCREEN_W-2, SCREEN_H-1);


      al_set_target_backbuffer(display);
      al_draw_bitmap(jtemp, smx+1, smy+1, 0);
      al_destroy_bitmap(jtemp);
      // frame bullseye
      al_draw_rectangle(smx, smy, smx+ssz+1, smy+ssz+1, palette_color[10], 1);
      al_reset_clipping_rectangle();
      al_draw_textf(font, palette_color[15], txc, smy-9, ALLEGRO_ALIGN_CENTER, " x=%-2d     y=%-2d ", dx, dy );
   }
   else
   {
      al_draw_text(font, palette_color[14], txc, smy-9, ALLEGRO_ALIGN_CENTER, "  mouse off map  ");
      // erase bullseye map
      al_draw_filled_rectangle(smx, smy, smx+ssz, smy+ssz, palette_color[0]);
      // frame bullseye
      al_draw_rectangle(smx, smy, smx+ssz+1, smy+ssz+1, palette_color[10], 1);
   }
}






int get_block_range(const char *txt, int *x1, int *y1, int *x2, int *y2, int type)
{
   init_level_background();
   int gx=0, gy=0, hx=0, hy=0;
   int tx = SCREEN_W/2;
   int quit = 0;
   int ret = 0;
   while (!quit)
   {
      al_flip_display();
      proc_scale_factor_change();
      proc_controllers();
      proc_frame_delay();
      get_new_background(0);
      draw_lifts();
      draw_items();
      draw_enemies();
      crosshairs_full(gx*20+10, gy*20+10, 15, 1);
      get_new_screen_buffer(3, 0, 0);
      ovw_get_block_position_on_map(&gx, &gy, &hx, &hy);
      ovw_process_scrolledge();


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
         if (type == 1) // key block range 0-1999 xywh
         {
            *x1 = gx*20;
            *y1 = gy*20;
         }
         if (type == 2) // pod cloner trigger box 0-99 x1, y1, x2, y2
         {
            *x1 = gx;
            *y1 = gy;
         }
         if (type == 3) // cloner src 0-99 x y w h
         {
            *x1 = gx;
            *y1 = gy;
         }
         while (mouse_b1)
         {
            al_flip_display();
            proc_scale_factor_change();
            proc_controllers();
            proc_frame_delay();
            get_new_background(0);
            draw_lifts();
            draw_items();
            draw_enemies();

            // show the selection rectangle
            if (type == 1) al_draw_rectangle(*x1,    *y1,     *x1+*x2,      *y1+*y2,     palette_color[15], 1);
            if (type == 2) al_draw_rectangle(*x1*20, *y1*20, (*x2+1)*20,   (*y2+1)*20,   palette_color[15], 1);
            if (type == 3) al_draw_rectangle(*x1*20, *y1*20, (*x2+*x1)*20, (*y2+*y1)*20, palette_color[15], 1);

            get_new_screen_buffer(3, 0, 0);
            ovw_process_scrolledge();
            ovw_get_block_position_on_map(&gx, &gy, &hx, &hy);

            if (type == 1)
            {
               *x2 = gx*20-*x1;
               *y2 = gy*20-*y1;
            }

            if (type == 2)
            {
               *x2 = gx;
               *y2 = gy;
            }
            if (type == 3)
            {
               *x2 = gx-*x1;
               *y2 = gy-*y1;
            }

            al_draw_filled_rectangle(tx-90, 70, tx+90, 170, palette_color[0]);
            al_draw_rectangle(       tx-90, 70, tx+90, 170, palette_color[15], 1);

            al_draw_text(font, palette_color[9],   tx, 72,  ALLEGRO_ALIGN_CENTER, "Draw a new");
            al_draw_text(font, palette_color[10],  tx, 80,  ALLEGRO_ALIGN_CENTER, txt);
            al_draw_text(font, palette_color[9],   tx, 88,  ALLEGRO_ALIGN_CENTER, "by clicking and");
            al_draw_text(font, palette_color[9],   tx, 96,  ALLEGRO_ALIGN_CENTER, "dragging with the");
            al_draw_text(font, palette_color[9],   tx, 104, ALLEGRO_ALIGN_CENTER, "left mouse button");
            al_draw_text(font, palette_color[14],  tx, 130, ALLEGRO_ALIGN_CENTER, "Cancel with <ESC>");
            al_draw_text(font, palette_color[14],  tx, 138, ALLEGRO_ALIGN_CENTER, "or right mouse button");

            if (type == 1)
            {
               al_draw_textf(font, palette_color[15], tx, 150, ALLEGRO_ALIGN_CENTER, "x:%2d y:%2d", *x1/20, *y1/20);
               al_draw_textf(font, palette_color[15], tx, 158, ALLEGRO_ALIGN_CENTER, "w:%2d h:%2d", *x2/20, *y2/20);
            }
            if (type == 2)
            {
               al_draw_textf(font, palette_color[15], tx, 150, ALLEGRO_ALIGN_CENTER, "x1:%2d y1:%2d", *x1, *y1);
               al_draw_textf(font, palette_color[15], tx, 158, ALLEGRO_ALIGN_CENTER, "x2:%2d y2:%2d", *x2, *y2);
            }
            if (type == 3)
            {
               al_draw_textf(font, palette_color[15], tx, 150, ALLEGRO_ALIGN_CENTER, "x:%2d y:%2d", *x1, *y1);
               al_draw_textf(font, palette_color[15], tx, 158, ALLEGRO_ALIGN_CENTER, "w:%2d h:%2d", *x2, *y2);
            }

         }
         quit = 1;
         ret = 1;
         //printf("before limit check - x1:%d y1:%d x2:%d y2:%d\n", *x1, *y1, *x2, *y2);
         if (type == 1) // 2000 w h
         {
            // don't allow 0 width or height
            if (*x2 == 0) *x2 = 20;
            if (*y2 == 0) *y2 = 20;

            // UR corner must be at least one block from limit
            if (*x1 > 1980) *x1 = 1980;
            if (*y1 > 1980) *y1 = 1980;

            // correct if lr corner is off level
            if ((*x1 + *x2) > 2000) *x2 = 2000 - *x1;
            if ((*y1 + *y2) > 2000) *y2 = 2000 - *y1;

            // swap if negative width or height
            if (*x2 < 0)
            {
               *x1 = *x1 + *x2;
               *x2 = -*x2;
            }
            if (*y2 < 0)
            {
               *y1 = *y1 + *y2;
               *y2 = -*y2;
            }
         }
         if (type == 2) // 100 x2 y2
         {
            int z; // swap x1 and x2 if neccesary
            if (*x1 > *x2) { z = *x1; *x1 = *x2; *x2 = z;}
            if (*y1 > *y2) { z = *y1; *y1 = *y2; *y2 = z;}

            if (*x1 > 99) *x1 = 99;
            if (*y1 > 99) *y1 = 99;
            if (*x2 > 99) *x2 = 99;
            if (*y2 > 99) *y2 = 99;
         }
         if (type == 3) // 100 w h
         {
            // don't allow 0 width or height
            if (*x2 == 0) *x2 = 1;
            if (*y2 == 0) *y2 = 1;

            // UR corner must be at least one block from limit
            if (*x1 > 98) *x1 = 98;
            if (*y1 > 98) *y1 = 98;

            // correct if lr corner is off level
            if ((*x1 + *x2) > 99) *x2 = 100 - *x1;
            if ((*y1 + *y2) > 99) *y2 = 100 - *y1;

            // swap if negative width or height
            if (*x2 < 0)
            {
               *x1 = *x1 + *x2;
               *x2 = -*x2;
            }
            if (*y2 < 0)
            {
               *y1 = *y1 + *y2;
               *y2 = -*y2;
            }
         }
         //printf("after limit check - x1:%d y1:%d x2:%d y2:%d\n", *x1, *y1, *x2, *y2);
      }
      if (mouse_b2)
      {
         while (mouse_b2) proc_controllers(); // wait for release
         quit = 1;
         ret = 0;
      }
      if (key[ALLEGRO_KEY_ESCAPE])
      {
         quit = 1;
         ret = 0;
      }
   }
   return ret;
}





























































int getxy(const char *txt, int obj_type, int sub_type, int num)
{
   init_level_background();
   int gx=0, gy=0, hx=0, hy=0;
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
      original_dy = item[num][11];
   }
   if (obj_type == 4) // lift
   {
      original_dx = lift_steps[lift][step].x;
      original_dy = lift_steps[lift][step].y;
   }

   while(!quit)
   {

      al_flip_display();
      proc_scale_factor_change();
      proc_controllers();
      proc_frame_delay();
      get_new_background(0);
      draw_lifts();
      draw_items();
      draw_enemies();

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
      if (obj_type == 98) // move cloner destination
      {
         float dx1 = (float)Ei[num][17]*20;
         float dy1 = (float)Ei[num][18]*20;
         float dx2 = dx1 + (float)Ei[num][19]*20;
         float dy2 = dy1 + (float)Ei[num][20]*20;
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
            if (item[i][0] == 10) draw_pop_message(i);
      }

      get_new_screen_buffer(3, 0, 0);
      ovw_process_scrolledge();
      ovw_get_block_position_on_map(&gx, &gy, &hx, &hy);

      al_draw_filled_rectangle(tx-100, 70, tx+100, 128, palette_color[0]);
      al_draw_rectangle(       tx-100, 70, tx+100, 128, palette_color[15], 1);

      al_draw_text(font,  palette_color[9],   tx, 72,  ALLEGRO_ALIGN_CENTER, "Set new Location for:");
      al_draw_text(font,  palette_color[10],  tx, 80,  ALLEGRO_ALIGN_CENTER, txt);
      al_draw_text(font,  palette_color[9],   tx, 88,  ALLEGRO_ALIGN_CENTER, "with left mouse button");
      al_draw_text(font,  palette_color[14],  tx, 110, ALLEGRO_ALIGN_CENTER, "Cancel with <ESC>");
      al_draw_text(font,  palette_color[14],  tx, 118, ALLEGRO_ALIGN_CENTER, "or right mouse button");


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
            init_level_background();        // to redraw lift lines
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
            Ei[num][17] = gx;
            Ei[num][18] = gy;
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
            item[num][10] = gx;
            item[num][11] = gy;
         }
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
          item[num][11] = original_dy;
      }
      if (obj_type == 4)
      {
         lift_steps[lift][step].x = original_dx;
         lift_steps[lift][step].y = original_dy;
      }
   }
   return retval;
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
      if (item[i][0] == 9)
      {
         if (item[i][11] == ev) return 1;
         if (item[i][12] == ev) return 1;
         if (item[i][13] == ev) return 1;
         if (item[i][14] == ev) return 1;
      }
      if ((item[i][0] == 16) && (item[i][1] == ev)) return 1;
      if ((item[i][0] == 17) && (item[i][1] == ev)) return 1;
   }
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
   int gx=0, gy=0, hx=0, hy=0;
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
      al_flip_display();
      proc_scale_factor_change();
      proc_controllers();
      proc_frame_delay();
      get_new_background(0);
      draw_lifts();
      draw_items();
      draw_enemies();

      crosshairs_full(gx*20+10, gy*20+10, 15, 1);

      if (mouse_on_item) al_draw_line(x2, y2, itx+10, ity+10, palette_color[10], 2);
      else
      {
         if (obj_type == 2) find_and_show_event_links(2, num, 0);
         if (obj_type == 4) find_and_show_event_links(3, lift, step);
      }



      get_new_screen_buffer(3, 0, 0);
      ovw_process_scrolledge();
      ovw_get_block_position_on_map(&gx, &gy, &hx, &hy);

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
         if (item[x][0] == 9)    // trigger only
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
   int gx=0, gy=0, hx=0, hy=0;
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
      al_flip_display();
      proc_scale_factor_change();
      proc_controllers();
      proc_frame_delay();
      get_new_background(0);
      draw_lifts();
      draw_items();
      draw_enemies();

      crosshairs_full(gx*20+10, gy*20+10, 15, 1);

      if (mouse_on_item) al_draw_line(x2, y2, itx+10, ity+10, palette_color[10], 2);

      get_new_screen_buffer(3, 0, 0);
      ovw_process_scrolledge();
      ovw_get_block_position_on_map(&gx, &gy, &hx, &hy);

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

void crosshairs(int smx, int smy, int x, int y, int color) // function to draw rectangle and crosshairs
{
   //al_draw_filled_rectangle(smx+(x*db), smy+(y*db), smx+(x*db)+db-1, smy+(y*db)+db-1, palette_color[color]);
   al_draw_filled_rectangle(smx+(x*db)-1, smy+(y*db)-1, smx+(x*db)+db, smy+(y*db)+db, palette_color[color]);
   al_draw_line(smx+1, smy+(y*db)+db/2, smx+(100*db)-2, smy+(y*db)+db/2, palette_color[color], 1);
   al_draw_line(smx+(x*db)+db/2, smy+1, smx+(x*db)+db/2, smy+(100*db)-2, palette_color[color], 1);
}

void crosshairs_nodb(int smx, int smy, int x, int y, int db, int color) // function to draw rectangle and crosshairs
{
   al_draw_filled_rectangle(smx+(x), smy+(y), smx+(x)+db-1, smy+(y)+db-1, palette_color[color]);
   al_draw_line(smx, smy+(y)+db/2, smx+(100*db), smy+(y)+db/2, palette_color[color], 1);
   al_draw_line(smx+(x)+db/2, smy, smx+(x)+db/2, smy+(100*db), palette_color[color], 1);
}

void title(const char *txt, int y, int tc, int fc)
{
   for (int x=0; x<15; x++)
      al_draw_line(db*100, y+x, SCREEN_W-2, y+x, palette_color[fc+(x*16)], 1);
   al_draw_text(font, palette_color[tc], txc, y+2, ALLEGRO_ALIGN_CENTER,  txt);
}

void titlex(const char *txt, int tc, int fc, int x1, int x2, int y)
{
   for (int x=0; x<15; x++)
      al_draw_line(x1, y+x, x2, y+x, palette_color[fc+(x*16)], 1);
   al_draw_text(font, palette_color[tc], (x1+x2)/2, y+2, ALLEGRO_ALIGN_CENTER,  txt);
}






