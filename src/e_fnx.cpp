// e_fnx.cpp

#include "pm.h"
#include "e_fnx.h"
#include "mwWindowManager.h"
#include "mwDisplay.h"
#include "mwFont.h"
#include "mwBitmap.h"
#include "z_lift.h"
#include "mwWidgets.h"
#include "mwColor.h"
#include "mwPMEvent.h"
#include "mwInput.h"
#include "mwEventQueue.h"
#include "z_menu.h"
#include "z_item.h"
#include "z_enemy.h"
#include "z_fnx.h"
#include "z_screen.h"





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


int exit_level_editor_dialog(void)
{
   while (mI.key[ALLEGRO_KEY_ESCAPE][0]) mwEQ.proc_event_queue();

   al_set_target_backbuffer(display);
   al_show_mouse_cursor(display);

   int quit = 0;
   int ret = 0;

   int w = 200, h = 100;
   int x = (mwD.SCREEN_W - w) /2;
   int y = (mwD.SCREEN_H - h) /2;

   int xc = x+w/2;
   int xa = xc-84;
   int xb = xc+84;
   int bts = 16;

   while (!quit)
   {
      al_draw_filled_rectangle(x, y, x+w, y+h, mC.pc[10+128+64]);

      for (int a=0; a<10; a++)
         al_draw_rounded_rectangle(x+a, y+a, x+w-a, y+h-a, 1, 1, mC.pc[10+a*16], 1);
      al_draw_text(mF.pr8, mC.pc[15], xc, y+14, ALLEGRO_ALIGN_CENTER, "Exit Level Editor?");

      int bc = 15;

      int by1 = y+30;

      if (ret == 0) bc = 10;
      else bc = 15;

      if (mdw_buttont(xa, by1, xb, bts, 0,0,0,0,  0,bc,15,0, 1,0,1,0, "Save and Exit"))         { quit = 1; ret = 0; }
      if (ret == 0) al_draw_rounded_rectangle(xa-1, by1-bts-1, xb+1, by1-1, 2, 2, mC.pc[10], 2);

      by1+=4;

      if (ret == 1) bc = 10;
      else bc = 15;
      if (mdw_buttont(xa, by1, xb, bts, 0,0,0,0,  0,bc,15,0, 1,0,1,0, "Exit Without Saving"))   { quit = 1; ret = 1; }
      if (ret == 1) al_draw_rounded_rectangle(xa-1, by1-bts-1, xb+1, by1-1, 2, 2, mC.pc[10], 2);

      by1+=4;

      if (ret == 2) bc = 10;
      else bc = 15;
      if (mdw_buttont(xa, by1, xb, bts, 0,0,0,0,  0,bc,15,0, 1,0,1,0, "Cancel"))                { quit = 1; ret = 2; }
      if (ret == 2) al_draw_rounded_rectangle(xa-1, by1-bts-1, xb+1, by1-1, 2, 2, mC.pc[10], 2);

      mwEQ.proc_event_queue();
      al_flip_display();

      if (mI.key[ALLEGRO_KEY_DOWN][0])
      {
         while (mI.key[ALLEGRO_KEY_DOWN][0]) mwEQ.proc_event_queue();
         if (++ret > 2) ret = 2;
      }

      if (mI.key[ALLEGRO_KEY_UP][0])
      {
         while (mI.key[ALLEGRO_KEY_UP][0]) mwEQ.proc_event_queue();
         if (--ret < 0) ret = 0;
      }
      if (mI.key[ALLEGRO_KEY_ENTER][0])
      {
         while (mI.key[ALLEGRO_KEY_ENTER][0]) mwEQ.proc_event_queue();
         quit = 1;
      }
      if (mI.key[ALLEGRO_KEY_ESCAPE][0])
      {
         while (mI.key[ALLEGRO_KEY_ESCAPE][0]) mwEQ.proc_event_queue();
         ret = 2; // cancel
         quit = 1;
      }
   }
   //printf("ret:%d\n", ret);
   return ret;
}


void draw_block_non_default_flags(int tn, int x, int y)
{
   int c = tn & 1023;
   al_draw_bitmap(mwB.btile[c], x, y, 0);
   if ((mwB.sa[c][0] & PM_BTILE_MOST_FLAGS) != (tn & PM_BTILE_MOST_FLAGS))
   {
      al_draw_line(x, y, x+20, y+20, mC.pc[10], 1);
      al_draw_line(x+20, y, x, y+20, mC.pc[10], 1);
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
   init_level_background(0);
   int tx = mwD.SCREEN_W/2;
   int quit = 0;
   int ret = 0;
   while (!quit)
   {
      mwWM.redraw_level_editor_background(0);
      crosshairs_full(mwWM.gx*20+10, mwWM.gy*20+10, 15, 1);
      get_new_screen_buffer(3, 0, 0);

      al_draw_filled_rectangle(tx-90, 70, tx+90, 170, mC.pc[0]);
      al_draw_rectangle(       tx-90, 70, tx+90, 170, mC.pc[15], 1);

      al_draw_text(mF.pr8, mC.pc[9],   tx, 72,  ALLEGRO_ALIGN_CENTER, "Draw a new");
      al_draw_text(mF.pr8, mC.pc[10],  tx, 80,  ALLEGRO_ALIGN_CENTER, txt);
      al_draw_text(mF.pr8, mC.pc[9],   tx, 88,  ALLEGRO_ALIGN_CENTER, "by clicking and");
      al_draw_text(mF.pr8, mC.pc[9],   tx, 96,  ALLEGRO_ALIGN_CENTER, "dragging with the");
      al_draw_text(mF.pr8, mC.pc[9],   tx, 104, ALLEGRO_ALIGN_CENTER, "left mouse button");
      al_draw_text(mF.pr8, mC.pc[14],  tx, 130, ALLEGRO_ALIGN_CENTER, "Cancel with <ESC>");
      al_draw_text(mF.pr8, mC.pc[14],  tx, 138, ALLEGRO_ALIGN_CENTER, "or right mouse button");
      al_draw_textf(mF.pr8, mC.pc[15], tx, 150, ALLEGRO_ALIGN_CENTER, "x:%2d y:%2d", mwWM.gx, mwWM.gy);

      if (mI.mouse_b[1][0])
      {
         mwWM.get_new_box();
         *x1 = mwWM.bx1*20;
         *y1 = mwWM.by1*20;
         *x2 = (mwWM.bx2-mwWM.bx1)*20+20;
         *y2 = (mwWM.by2-mwWM.by1)*20+20;

         if (type == 2)
         {
            *x2 = (mwWM.bx2-mwWM.bx1)*20;
            *y2 = (mwWM.by2-mwWM.by1)*20;
         }

         quit = 1;
         ret = 1;
      }
      while ((mI.mouse_b[2][0]) || (mI.key[ALLEGRO_KEY_ESCAPE][0]))
      {
         mwEQ.proc_event_queue(); // wait for release
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

   al_draw_spline(pnts, mC.pc[10], 0);

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
   if (legend_highlight == 2) color1 = mC.flash_color;
   int epx = Ei[num][9];
   int epy = Ei[num][10];
   crosshairs_full(epx+10, epy+10, color1, 1);

   al_draw_textf(mF.pixl, mC.White, epx+22, epy+0, 0, "x:%d", epx-ipx);
   al_draw_textf(mF.pixl, mC.White, epx+22, epy+8, 0, "y:%d", epy-ipy);

   // set extended rotation
   xlen = al_ftofix(dest[198] - dest[194]);                     // get the x distance
   ylen = al_ftofix(dest[199] - dest[193]);                     // get the y distance
   al_fixed ext_rot = al_fixatan2(ylen, xlen) - al_itofix(64);  // rotation

   // draw mwB.tile at extended pos
   float rot = al_fixtof(al_fixmul(ext_rot, al_fixtorad_r));
   al_draw_scaled_rotated_bitmap(mwB.tile[Ei[num][1]], 10, 10, epx+10, epy+10, 1, 1, rot, ALLEGRO_FLIP_HORIZONTAL);

   // control point 1
   color1 = 6;
   if (legend_highlight == 3) color1 = mC.flash_color;
   int px = Ei[num][5];
   int py = Ei[num][6];
   al_draw_line(ipx+10, ipy+10, px+10, py+10, mC.pc[color1], 0);
   al_draw_line(epx+10, epy+10, px+10, py+10, mC.pc[color1], 0);
   al_draw_filled_circle(px+10, py+10, 3, mC.pc[color1]);
   al_draw_circle(px+10, py+10, 6, mC.pc[color1], 1);
   al_draw_textf(mF.pixl, mC.White, px+20, py+0, 0, "x:%d", px-ipx);
   al_draw_textf(mF.pixl, mC.White, px+20, py+8, 0, "y:%d", py-ipy);



   // control point 2
   color1 = 7;
   if (legend_highlight == 4) color1 = mC.flash_color;
   px = Ei[num][7];
   py = Ei[num][8];
   al_draw_line(ipx+10, ipy+10, px+10, py+10, mC.pc[color1], 0);
   al_draw_line(epx+10, epy+10, px+10, py+10, mC.pc[color1], 0);
   al_draw_filled_circle(px+10, py+10, 3, mC.pc[color1]);
   al_draw_circle(px+10, py+10, 6, mC.pc[color1], 1);
   al_draw_textf(mF.pixl, mC.pc[15], px+20, py+0, 0, "x:%d", px-ipx);
   al_draw_textf(mF.pixl, mC.pc[15], px+20, py+8, 0, "y:%d", py-ipy);

   // trigger box
   int color = 14;
   if (legend_highlight == 5) color = mC.flash_color;
   int tx1 = Ei[num][11];
   int ty1 = Ei[num][12];
   int tx2 = Ei[num][11]+Ei[num][13] + 20;
   int ty2 = Ei[num][12]+Ei[num][14] + 20;
   al_draw_rectangle(tx1, ty1, tx2, ty2, mC.pc[color], 1);


}









int getxy(const char *txt, int obj_type, int sub_type, int num)
{
   int tx = mwD.SCREEN_W/2;

   // in case these are needed for lifts
   int lift = sub_type;
   int step = num;

   int retval=0;
   int quit=0;

   while (mI.mouse_b[1][0]) mwEQ.proc_event_queue(); // wait for release


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
      mwWM.redraw_level_editor_background(0);

      crosshairs_full(mwWM.gx*20+10, mwWM.gy*20+10, 15, 1);

      if (obj_type == 99) // move pod extended
      {
         int ex = al_fixtoi(Efi[num][0])+10;
         int ey = al_fixtoi(Efi[num][1])+10;
         int px = al_fixtoi(Efi[num][5])+10;
         int py = al_fixtoi(Efi[num][6])+10;
         float rot = al_fixtof(al_fixmul(Efi[num][14], al_fixtorad_r));
         al_draw_scaled_rotated_bitmap(mwB.tile[Ei[num][1]], 10, 10, px, py, 1, 1, rot, ALLEGRO_FLIP_HORIZONTAL); // draw tile at extended pos
         al_draw_line(ex, ey, px, py, mC.pc[10], 1); // connect with line
      }

      if ((obj_type == 90) || (obj_type == 91) || (obj_type == 92)) draw_vinepod_controls(num, -1); // move vinepod extended, cp1, cp2


      if (obj_type == 98) // move cloner destination
      {
         float dx1 = (float)Ei[num][17];
         float dy1 = (float)Ei[num][18];
         float dx2 = dx1 + (float)Ei[num][19];
         float dy2 = dy1 + (float)Ei[num][20];
         int dc1 = 10 + 128; // destination box color
         rectangle_with_diagonal_lines(dx1, dy1, dx2, dy2, 8, dc1, dc1+64, 0); // destination
      }
      if (obj_type == 97) // set new rocket direction
      {
         int ix = item[num][4]+10;
         int iy = item[num][5]+10;
         int dx = mwWM.gx*20+10;
         int dy = mwWM.gy*20+10;
         float rot = al_fixtof(al_fixmul(al_itofix(item[num][10]/10), al_fixtorad_r));
         al_draw_rotated_bitmap(mwB.tile[item[num][1]], 10, 10, dx, dy, rot, 0);
         al_draw_line(ix, iy, dx, dy, mC.pc[10], 1);     // connect with line
      }
      if (obj_type == 96) // set cannon or bouncer direction
      {
         int ex = al_fixtoi(Efi[num][0])+10;
         int ey = al_fixtoi(Efi[num][1])+10;
         int dx = mwWM.gx*20+10;
         int dy = mwWM.gy*20+10;
         float rot = al_fixtof(al_fixmul(Efi[num][14], al_fixtorad_r));
         al_draw_scaled_rotated_bitmap(mwB.tile[Ei[num][1]], 10, 10, dx, dy, 1, 1, rot, ALLEGRO_FLIP_HORIZONTAL); // draw tile
         al_draw_line(ex, ey, dx, dy, mC.pc[10], 1);   // connect with line
      }

      get_new_screen_buffer(3, 0, 0);

      al_draw_filled_rectangle(tx-100, 70, tx+100, 128, mC.pc[0]);
      al_draw_rectangle(       tx-100, 70, tx+100, 128, mC.pc[15], 1);

      al_draw_text(mF.pr8,  mC.pc[9],   tx, 72,  ALLEGRO_ALIGN_CENTER, "Set new Location for:");
      al_draw_text(mF.pr8,  mC.pc[10],  tx, 80,  ALLEGRO_ALIGN_CENTER, txt);
      al_draw_text(mF.pr8,  mC.pc[9],   tx, 88,  ALLEGRO_ALIGN_CENTER, "with left mouse button");
      al_draw_text(mF.pr8,  mC.pc[14],  tx, 110, ALLEGRO_ALIGN_CENTER, "Cancel with <ESC>");
      al_draw_text(mF.pr8,  mC.pc[14],  tx, 118, ALLEGRO_ALIGN_CENTER, "or right mouse button");

      al_flip_display();


      switch (obj_type)
      {
         case 2: // items
            item[num][4] = mwWM.gx*20;
            item[num][5] = mwWM.gy*20;
            itemf[num][0] = al_itofix(mwWM.gx*20);
            itemf[num][1] = al_itofix(mwWM.gy*20);
         break;
         case 3: // show enem
            Efi[num][0] = al_itofix(mwWM.gx*20);
            Efi[num][1] = al_itofix(mwWM.gy*20);
         break;
         case 4: // show lift
         {
            lift_steps[lift][step].x = mwWM.gx*20;
            lift_steps[lift][step].y = mwWM.gy*20;
            set_lift_to_step(lift, step);   // set current step in current lift
         }
         break;
         case 99: // move pod extended
         {
            Efi[num][5] = al_itofix(mwWM.gx*20);
            Efi[num][6] = al_itofix(mwWM.gy*20);
            recalc_pod(num);
         }
         break;
         case 98: // cloner destination
         {
            Ei[num][17] = mwWM.gx*20;
            Ei[num][18] = mwWM.gy*20;
         }
         break;
         case 97: // set new rocket direction
         {
            set_rocket_rot(num, mwWM.gx*20, mwWM.gy*20);
         }
         break;
         case 96: // set cannon or bouncer direction
         {
            set_xyinc_rot(num, mwWM.gx*20, mwWM.gy*20);
         }
         break;
         case 90: // vinepod extended
         {
            Ei[num][9] = mwWM.gx*20;
            Ei[num][10] = mwWM.gy*20;
         }
         break;
         case 91: // vinepod cp1
         {
            Ei[num][5] = mwWM.gx*20;
            Ei[num][6] = mwWM.gy*20;
         }
         break;
         case 92: // vinepod cp2
         {
            Ei[num][7] = mwWM.gx*20;
            Ei[num][8] = mwWM.gy*20;
         }
         break;
      }


      while (mI.mouse_b[1][0])
      {
         mwEQ.proc_event_queue();
         quit = 1;
         retval = 1;  // b1 xy
      }
      while (mI.mouse_b[2][0])
      {
         mwEQ.proc_event_queue();
         quit = 1;
         retval = 2;  // b2 xy
      }
      while (mI.key[ALLEGRO_KEY_ESCAPE][0])
      {
         mwEQ.proc_event_queue();
         quit = 1;
         retval = 0;  // ignore xy
      }
   } // end of while(!quit);


   if (mwWM.gx > 99) mwWM.gx = 99;
   if (mwWM.gy > 99) mwWM.gy = 99;

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
      if (obj_type == 4)
      {
         lift_steps[lift][step].x = original_dx;
         lift_steps[lift][step].y = original_dy;
      }
   }
   return retval;
}







int get_trigger_item(int obj_type, int sub_type, int num )
{
   init_level_background(0);
   int tx = mwD.SCREEN_W/2;

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

   while (mI.mouse_b[1][0]) mwEQ.proc_event_queue(); // wait for release

   while(!quit)
   {
      mwWM.redraw_level_editor_background(0);
      crosshairs_full(mwWM.gx*20+10, mwWM.gy*20+10, 15, 1);

      if (mouse_on_item) al_draw_line(x2, y2, itx+10, ity+10, mC.pc[10], 2);
      else
      {
         if (obj_type == 2) mwPME.find_and_show_event_links(2, num, 0);
         if (obj_type == 4) mwPME.find_and_show_event_links(3, lift, step);
      }

      get_new_screen_buffer(3, 0, 0);
      al_draw_filled_rectangle(tx-110, 78, tx+110, 146, mC.pc[0]);
      al_draw_rectangle(       tx-110, 78, tx+110, 146, mC.pc[15], 1);

      al_draw_text(mF.pr8,  mC.pc[9],  tx, 80,  ALLEGRO_ALIGN_CENTER, "Select a Trigger Object");
      al_draw_text(mF.pr8,  mC.pc[9],  tx, 88,  ALLEGRO_ALIGN_CENTER, "with left mouse button");
      al_draw_text(mF.pr8,  mC.pc[14], tx, 110, ALLEGRO_ALIGN_CENTER, "Cancel with <ESC>");
      al_draw_text(mF.pr8,  mC.pc[14], tx, 118, ALLEGRO_ALIGN_CENTER, "or right mouse button");

      if (mouse_on_item) al_draw_textf(mF.pr8, mC.pc[15], tx, 136, ALLEGRO_ALIGN_CENTER, "Trigger Object:%d", ret_item);
      else               al_draw_textf(mF.pr8, mC.pc[15], tx, 136, ALLEGRO_ALIGN_CENTER, "No Trigger Object Selected");

      mouse_on_item = 0;
      for (int x=0; x<500; x++)
         if ((item[x][0] == 9) || (item[x][0] == 6))    // trigger or orb only
         {
            itx = item[x][4]/20;
            ity = item[x][5]/20;
            if ((mwWM.gx == itx) && (mwWM.gy == ity))
            {
               mouse_on_item = 1;
               ret_item = x;
            }
         }

      itx = item[ret_item][4];
      ity = item[ret_item][5];

      while (mI.mouse_b[1][0])
      {
         mwEQ.proc_event_queue();
         quit = 1;
      }
      while ((mI.mouse_b[2][0]) || (mI.key[ALLEGRO_KEY_ESCAPE][0]))
      {
         mwEQ.proc_event_queue();
         quit = 1;
         ret_item = - 1;
      }
   } // end of while(!quit);
   if (!mouse_on_item) ret_item = -1;
   return ret_item;
}



int get_item(int obj_type, int sub_type, int num )
{
   init_level_background(0);
   int tx = mwD.SCREEN_W/2;

   int ret_item=0;
   int quit=0;
   int mouse_on_item = 0;

   int itx=0, ity=0;

   int x2 = item[num][4]+10; // get the originating door's location
   int y2 = item[num][5]+10;

   while (mI.mouse_b[1][0]) mwEQ.proc_event_queue(); // wait for release

   while(!quit)
   {
      mwWM.redraw_level_editor_background(0);
      crosshairs_full(mwWM.gx*20+10, mwWM.gy*20+10, 15, 1);

      if (mouse_on_item) al_draw_line(x2, y2, itx+10, ity+10, mC.pc[10], 2);

      get_new_screen_buffer(3, 0, 0);

      al_draw_filled_rectangle(tx-110, 78, tx+110, 146, mC.pc[0]);
      al_draw_rectangle(       tx-110, 78, tx+110, 146, mC.pc[15], 1);

      al_draw_text(mF.pr8,  mC.pc[9],  tx, 80,  ALLEGRO_ALIGN_CENTER, "Select a Door Object");
      al_draw_text(mF.pr8,  mC.pc[9],  tx, 88,  ALLEGRO_ALIGN_CENTER, "with left mouse button");
      al_draw_text(mF.pr8,  mC.pc[14], tx, 110, ALLEGRO_ALIGN_CENTER, "Cancel with <ESC>");
      al_draw_text(mF.pr8,  mC.pc[14], tx, 118, ALLEGRO_ALIGN_CENTER, "or right mouse button");

      if (mouse_on_item) al_draw_textf(mF.pr8, mC.pc[15], tx, 136, ALLEGRO_ALIGN_CENTER, "Door Object:%d", ret_item);
      else               al_draw_textf(mF.pr8, mC.pc[15], tx, 136, ALLEGRO_ALIGN_CENTER, "No Door Object Selected");


      mouse_on_item = 0;
      for (int x=0; x<500; x++)
         if (item[x][0] == 1)    // door only
         {
            itx = item[x][4]/20;
            ity = item[x][5]/20;
            if ((mwWM.gx == itx) && (mwWM.gy == ity))
            {
               mouse_on_item = 1;
               ret_item = x;
            }
         }

      itx = item[ret_item][4];
      ity = item[ret_item][5];

      while (mI.mouse_b[1][0])
      {
         mwEQ.proc_event_queue();
         quit = 1;
      }
      while ((mI.mouse_b[2][0]) || (mI.key[ALLEGRO_KEY_ESCAPE][0]))
      {
         mwEQ.proc_event_queue();
         quit = 1;
         ret_item = - 1;
      }
   } // end of while(!quit);
   if (!mouse_on_item) ret_item = -1;
   return ret_item;
}


void crosshairs_full(int x, int y, int color, int line_width) // draws a square and crosshairs around a full 20x20 block on level buffer
{
   al_draw_rectangle(x-10, y-10, x+11, y+11, mC.pc[color], line_width);
   al_draw_line(0,    y, x-10, y, mC.pc[color], line_width);
   al_draw_line(x+11, y, 1999, y, mC.pc[color], line_width);
   al_draw_line(x,    0, x, y-10, mC.pc[color], line_width);
   al_draw_line(x, y+11, x, 1999, mC.pc[color], line_width);
}
void titlex(const char *txt, int tc, int fc, int x1, int x2, int y)
{
   for (int x=0; x<15; x++)
      al_draw_line(x1, y+x, x2, y+x, mC.pc[fc+(x*16)], 1);
   al_draw_text(mF.pr8, mC.pc[tc], (x1+x2)/2, y+2, ALLEGRO_ALIGN_CENTER,  txt);
}

// when speed is changed in level editor (Efi[][5]) scale the xinc, yinc to match
void scale_bouncer_and_cannon_speed(int e)
{
   // new v
   float nv =  al_fixtof(Efi[e][5]);

   // get the original x and y velocities
   float oxv = al_fixtof(Efi[e][2]);
   float oyv = al_fixtof(Efi[e][3]);

   // get the combined original velocity
   float ov = sqrt( pow(oxv, 2) + pow(oyv, 2) );

   // if this was previously stationary, set direction to 100% up
   if (al_ftofix(ov) == al_ftofix(0))
   {
      Efi[e][3] = -Efi[e][5];
      Efi[e][14] = get_rot_from_xyinc(e); // set rotation
   }
   else
   {
      if (nv>0)
      {
         // get the scaler
         float sc = nv/ov;

         // apply that to the old
         oxv *= sc;
         oyv *= sc;

         Efi[e][2] = al_ftofix(oxv);
         Efi[e][3] = al_ftofix(oyv);
      }
      else // if new speed not > 0, zero both x and y
      {
         Efi[e][2] = al_ftofix(0);
         Efi[e][3] = al_ftofix(0);
      }

   }
}
