// e_fnx.cpp

#include "pm.h"
#include "e_fnx.h"
#include "mwWindowManager.h"
#include "mwDisplay.h"
#include "mwFont.h"
#include "mwBitmap.h"
#include "mwLift.h"
#include "mwWidgets.h"
#include "mwColor.h"
#include "mwPMEvent.h"
#include "mwInput.h"
#include "mwEventQueue.h"
#include "mwItems.h"
#include "mwEnemy.h"
#include "z_screen.h"
#include "z_player.h"
#include "n_netgame.h"



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


void chop_first_x_char(char *str, int n)
{
   char tmp[200];
   for(int x=n; x < (signed int)strlen(str)+1; x++)
      tmp[x-n] = str[x]; // chop first n
   strcpy(str, tmp);
}

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
   char msg[1024];
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

float get_sproingy_jump_height(int num)
{
   float t1 = mItem.item[num][7] / 7.1;
   float t2 = -15; // jump starts not at zero, but at almost one block height off ground

   while (t1 > 0)
   {
      t2 += t1;  // distance moved this time period
      t1 -= 0.2; // minus slow gravity
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
void set_xyinc_rot(int e, int x2, int y2)
{
   float xlen = x2 - mEnemy.Ef[e][0];      // get the x distance between enemy and x2
   float ylen = y2 - mEnemy.Ef[e][1];      // get the y distance between enemy and y2
   float hy_dist = sqrt(pow(xlen, 2) + pow(ylen, 2)); // hypotenuse distance
   float speed = mEnemy.Ef[e][5];                  // speed
   float scaler = hy_dist / speed;     // get scaler
   mEnemy.Ef[e][2] = xlen / scaler;         // calc xinc
   mEnemy.Ef[e][3] = ylen / scaler;         // calc yinc
   mEnemy.Ef[e][14] = atan2(ylen, xlen) - ALLEGRO_PI / 2;
}

// used only in sliders for button set new direction (rocket)
void set_rocket_rot(int num, int x2, int y2)
{
   float xlen = x2 - mItem.item[num][4];      // get the x distance between item and x2
   float ylen = y2 - mItem.item[num][5];      // get the y distance between item and y2
   float a = atan2(ylen, xlen) + ALLEGRO_PI/2;
   mItem.item[num][10] = a * 1000;
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
   mEnemy.Ei[num][3] = mEnemy.Ef[num][0];
   mEnemy.Ei[num][4] = mEnemy.Ef[num][1];

   // put variables in spline array
   float pnts[8];
   for (int i=0; i<8; i++) pnts[i] = mEnemy.Ei[num][i+3]+10;

   al_draw_spline(pnts, mC.pc[10], 0);

   // fill array of points from the spline
   float dest[200];
   al_calculate_spline(dest, 8, pnts, 0, 100);

   // initial position
   int ipx = mEnemy.Ei[num][3];
   int ipy = mEnemy.Ei[num][4];

   // set initial rotation
   float xlen = dest[4] - dest[0];            // get the x distance
   float ylen = dest[5] - dest[1];            // get the y distance
   mEnemy.Ef[num][14] = atan2(ylen, xlen) - ALLEGRO_PI/2;  // rotation

   // extended position
   int color1 = 10;
   if (legend_highlight == 2) color1 = mC.flash_color;
   int epx = mEnemy.Ei[num][9];
   int epy = mEnemy.Ei[num][10];
   crosshairs_full(epx+10, epy+10, color1, 1);

   al_draw_textf(mF.pixl, mC.White, epx+22, epy+0, 0, "x:%d", epx-ipx);
   al_draw_textf(mF.pixl, mC.White, epx+22, epy+8, 0, "y:%d", epy-ipy);

   // set extended rotation
   xlen = dest[198] - dest[194];            // get the x distance
   ylen = dest[199] - dest[193];            // get the y distance
   float ext_rot = atan2(ylen, xlen) - ALLEGRO_PI/2;  // rotation

   // draw mwB.tile at extended pos
   al_draw_scaled_rotated_bitmap(mwB.tile[mEnemy.Ei[num][1]], 10, 10, epx+10, epy+10, 1, 1, ext_rot, ALLEGRO_FLIP_HORIZONTAL);

   // control point 1
   color1 = 6;
   if (legend_highlight == 3) color1 = mC.flash_color;
   int px = mEnemy.Ei[num][5];
   int py = mEnemy.Ei[num][6];
   al_draw_line(ipx+10, ipy+10, px+10, py+10, mC.pc[color1], 0);
   al_draw_line(epx+10, epy+10, px+10, py+10, mC.pc[color1], 0);
   al_draw_filled_circle(px+10, py+10, 3, mC.pc[color1]);
   al_draw_circle(px+10, py+10, 6, mC.pc[color1], 1);
   al_draw_textf(mF.pixl, mC.White, px+20, py+0, 0, "x:%d", px-ipx);
   al_draw_textf(mF.pixl, mC.White, px+20, py+8, 0, "y:%d", py-ipy);



   // control point 2
   color1 = 7;
   if (legend_highlight == 4) color1 = mC.flash_color;
   px = mEnemy.Ei[num][7];
   py = mEnemy.Ei[num][8];
   al_draw_line(ipx+10, ipy+10, px+10, py+10, mC.pc[color1], 0);
   al_draw_line(epx+10, epy+10, px+10, py+10, mC.pc[color1], 0);
   al_draw_filled_circle(px+10, py+10, 3, mC.pc[color1]);
   al_draw_circle(px+10, py+10, 6, mC.pc[color1], 1);
   al_draw_textf(mF.pixl, mC.pc[15], px+20, py+0, 0, "x:%d", px-ipx);
   al_draw_textf(mF.pixl, mC.pc[15], px+20, py+8, 0, "y:%d", py-ipy);

   // trigger box
   int color = 14;
   if (legend_highlight == 5) color = mC.flash_color;
   int tx1 = mEnemy.Ei[num][11];
   int ty1 = mEnemy.Ei[num][12];
   int tx2 = mEnemy.Ei[num][11]+mEnemy.Ei[num][13] + 20;
   int ty2 = mEnemy.Ei[num][12]+mEnemy.Ei[num][14] + 20;
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
   int original_dx=0;
   int original_dy=0;
   float original_rot  = 0; // for direction
   float original_xinc = 0;
   float original_yinc = 0;


   if (obj_type == 2) // items
   {
      original_dx = mItem.item[num][4];
      original_dy = mItem.item[num][5];
   }
   if (obj_type == 3) // enemies
   {
      original_dx = mEnemy.Ef[num][0];
      original_dy = mEnemy.Ef[num][1];
   }
   if (obj_type == 99) // pod extended
   {
      original_dx = mEnemy.Ef[num][5];
      original_dy = mEnemy.Ef[num][6];
   }
   if (obj_type == 98) // cloner destination
   {
      original_dx = mEnemy.Ei[num][17];
      original_dy = mEnemy.Ei[num][18];
   }
   if (obj_type == 97) // rocket initial direction
   {
      original_rot = mItem.item[num][10];
   }
   if (obj_type == 96) // cannon or bouncer initial direction
   {
      original_rot  = mEnemy.Ef[num][14];
      original_xinc = mEnemy.Ef[num][2];
      original_yinc = mEnemy.Ef[num][3];
   }
   if (obj_type == 90) // vinepod extended
   {
      original_dx = mEnemy.Ei[num][9];
      original_dy = mEnemy.Ei[num][10];
   }
   if (obj_type == 91) // vinepod cp1
   {
      original_dx = mEnemy.Ei[num][5];
      original_dy = mEnemy.Ei[num][6];
   }
   if (obj_type == 92) // vinepod cp2
   {
      original_dx = mEnemy.Ei[num][7];
      original_dy = mEnemy.Ei[num][8];
   }

   if (obj_type == 4) // lift
   {
      original_dx = Lift.stp[lift][step].x;
      original_dy = Lift.stp[lift][step].y;
   }

   while(!quit)
   {
      mwWM.redraw_level_editor_background(0);

      crosshairs_full(mwWM.gx*20+10, mwWM.gy*20+10, 15, 1);

      if (obj_type == 99) // move pod extended
      {
         int ex = mEnemy.Ef[num][0]+10;
         int ey = mEnemy.Ef[num][1]+10;
         int px = mEnemy.Ef[num][5]+10;
         int py = mEnemy.Ef[num][6]+10;
         float rot = mEnemy.Ef[num][14];
         al_draw_scaled_rotated_bitmap(mwB.tile[mEnemy.Ei[num][1]], 10, 10, px, py, 1, 1, rot, ALLEGRO_FLIP_HORIZONTAL); // draw tile at extended pos
         al_draw_line(ex, ey, px, py, mC.pc[10], 1); // connect with line
      }

      if ((obj_type == 90) || (obj_type == 91) || (obj_type == 92)) draw_vinepod_controls(num, -1); // move vinepod extended, cp1, cp2


      if (obj_type == 98) // move cloner destination
      {
         float dx1 = (float)mEnemy.Ei[num][17];
         float dy1 = (float)mEnemy.Ei[num][18];
         float dx2 = dx1 + (float)mEnemy.Ei[num][19];
         float dy2 = dy1 + (float)mEnemy.Ei[num][20];
         int dc1 = 10 + 128; // destination box color
         rectangle_with_diagonal_lines(dx1, dy1, dx2, dy2, 8, dc1, dc1+64, 0); // destination
      }
      if (obj_type == 97) // set new rocket direction
      {
         int ix = mItem.item[num][4]+10;
         int iy = mItem.item[num][5]+10;
         int dx = mwWM.gx*20+10;
         int dy = mwWM.gy*20+10;

         float rot = (float)mItem.item[num][10] / 1000;
         al_draw_rotated_bitmap(mwB.tile[mItem.item[num][1]], 10, 10, dx, dy, rot, 0);

         al_draw_line(ix, iy, dx, dy, mC.pc[10], 1);     // connect with line
      }
      if (obj_type == 96) // set cannon or bouncer direction
      {
         int ex = mEnemy.Ef[num][0]+10;
         int ey = mEnemy.Ef[num][1]+10;
         int dx = mwWM.gx*20+10;
         int dy = mwWM.gy*20+10;
         float rot = mEnemy.Ef[num][14];
         al_draw_scaled_rotated_bitmap(mwB.tile[mEnemy.Ei[num][1]], 10, 10, dx, dy, 1, 1, rot, ALLEGRO_FLIP_HORIZONTAL); // draw tile
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
            mItem.itemf[num][0] = mItem.item[num][4] = mwWM.gx*20;
            mItem.itemf[num][1] = mItem.item[num][5] = mwWM.gy*20;
         break;
         case 3: // show enem
            mEnemy.Ef[num][0] = mwWM.gx*20;
            mEnemy.Ef[num][1] = mwWM.gy*20;
         break;
         case 4: // show lift
         {
            Lift.stp[lift][step].x = mwWM.gx*20;
            Lift.stp[lift][step].y = mwWM.gy*20;
            Lift.set_lift_to_step(lift, step);   // set current step in current lift
         }
         break;
         case 99: // move pod extended
         {
            mEnemy.Ef[num][5] = mwWM.gx*20;
            mEnemy.Ef[num][6] = mwWM.gy*20;
            mEnemy.recalc_pod(num);
         }
         break;
         case 98: // cloner destination
         {
            mEnemy.Ei[num][17] = mwWM.gx*20;
            mEnemy.Ei[num][18] = mwWM.gy*20;
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
            mEnemy.Ei[num][9] = mwWM.gx*20;
            mEnemy.Ei[num][10] = mwWM.gy*20;
         }
         break;
         case 91: // vinepod cp1
         {
            mEnemy.Ei[num][5] = mwWM.gx*20;
            mEnemy.Ei[num][6] = mwWM.gy*20;
         }
         break;
         case 92: // vinepod cp2
         {
            mEnemy.Ei[num][7] = mwWM.gx*20;
            mEnemy.Ei[num][8] = mwWM.gy*20;
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
         mItem.item[num][4] = original_dx;
         mItem.item[num][5] = original_dy;
         mItem.itemf[num][0] = original_dx;
         mItem.itemf[num][1] = original_dy;
      }
      if (obj_type == 3)
      {
          mEnemy.Ef[num][0] = original_dx;
          mEnemy.Ef[num][1] = original_dy;
      }
      if (obj_type == 90)
      {
          mEnemy.Ei[num][9] = original_dx;
          mEnemy.Ei[num][10] = original_dy;
      }
      if (obj_type == 91)
      {
          mEnemy.Ei[num][5] = original_dx;
          mEnemy.Ei[num][6] = original_dy;
      }
      if (obj_type == 92)
      {
          mEnemy.Ei[num][7] = original_dx;
          mEnemy.Ei[num][8] = original_dy;
      }
      if (obj_type == 99)
      {
          mEnemy.Ef[num][5] = original_dx;
          mEnemy.Ef[num][6] = original_dy;
      }
      if (obj_type == 98)
      {
          mEnemy.Ef[num][5] = original_dx;
          mEnemy.Ef[num][6] = original_dy;
      }
      if (obj_type == 97) // restore rocket direction
      {
         mItem.item[num][10] = original_rot;
      }
      if (obj_type == 96) // cannon or bouncer initial direction
      {
         mEnemy.Ef[num][14] = original_rot;
         mEnemy.Ef[num][2]  = original_xinc;
         mEnemy.Ef[num][3]  = original_yinc;
      }
      if (obj_type == 4)
      {
         Lift.stp[lift][step].x = original_dx;
         Lift.stp[lift][step].y = original_dy;
      }
   }
   return retval;
}



int round20(int val) // pass it an int and it will round it to the nearest 20
{
   int m = val%20;
   if (m<10) return (val - m);
   else return val + (20-m);
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
      x2 = mItem.item[num][4]+10; // get the object position
      y2 = mItem.item[num][5]+10;
   }
   if (obj_type == 4)
   {
      int pms = Lift.lift_find_previous_move_step(lift, step);
      x2 = Lift.stp[lift][pms].x + Lift.stp[lift][pms].w / 2;
      y2 = Lift.stp[lift][pms].y + Lift.stp[lift][pms].h / 2;
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
         if ((mItem.item[x][0] == 9) || (mItem.item[x][0] == 6))    // trigger or orb only
         {
            itx = mItem.item[x][4]/20;
            ity = mItem.item[x][5]/20;
            if ((mwWM.gx == itx) && (mwWM.gy == ity))
            {
               mouse_on_item = 1;
               ret_item = x;
            }
         }

      itx = mItem.item[ret_item][4];
      ity = mItem.item[ret_item][5];

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

   int x2 = mItem.item[num][4]+10; // get the originating door's location
   int y2 = mItem.item[num][5]+10;

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
         if (mItem.item[x][0] == 1)    // door only
         {
            itx = mItem.item[x][4]/20;
            ity = mItem.item[x][5]/20;
            if ((mwWM.gx == itx) && (mwWM.gy == ity))
            {
               mouse_on_item = 1;
               ret_item = x;
            }
         }

      itx = mItem.item[ret_item][4];
      ity = mItem.item[ret_item][5];

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



void rectangle_with_diagonal_lines(float x1, float y1, float x2, float y2, int spacing, int frame_color, int line_color, int clip_mode)
{
   int d = mwD.display_transform_double;
   if (!clip_mode) d = 1;

   al_set_clipping_rectangle(x1*d, y1*d, (x2-x1)*d, (y2-y1)*d);

   // find largest dimension
   float xd = x2-x1;
   float yd = y2-y1;
   float ld = xd;
   if (yd > ld) ld = yd;
   for (float k=-ld; k<ld; k+=spacing)
      al_draw_line(x1+k, y1-k, x1+ld+k, y1+ld-k, mC.pc[line_color], 0);
   al_draw_rectangle(x1+0.5, y1+0.5, x2-0.5, y2-0.5, mC.pc[frame_color], 1);
   al_reset_clipping_rectangle();
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

// when speed is changed in level editor (mEnemy.Ef[][5]) scale the xinc, yinc to match
void scale_bouncer_and_cannon_speed(int e)
{
   // new v
   float nv =  mEnemy.Ef[e][5];

   // get the original x and y velocities
   float oxv = mEnemy.Ef[e][2];
   float oyv = mEnemy.Ef[e][3];

   // get the combined original velocity
   float ov = sqrt( pow(oxv, 2) + pow(oyv, 2) );

   // if this was previously stationary, set direction to 100% up
   if (ov == 0)
   {
      mEnemy.Ef[e][3] = -mEnemy.Ef[e][5];
      mEnemy.set_enemy_rot_from_incs(e); // set rotation
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

         mEnemy.Ef[e][2] = oxv;
         mEnemy.Ef[e][3] = oyv;
      }
      else // if new speed not > 0, zero both x and y
      {
         mEnemy.Ef[e][2] = 0;
         mEnemy.Ef[e][3] = 0;
      }

   }
}













































int mw_file_select(const char * title, char * fn, const char * ext, int save)
{
   // no matter what I do I cannot start the native file chooser with the file I pass to it selected
   // i have tried everything I can think of
   // the filename and path is absolute, but still shows up blank

   int mode = 0; // default
   if (save) mode = ALLEGRO_FILECHOOSER_SAVE;

   char wext[100];
   sprintf(wext, "*%s", ext);


   //printf("fn:%s\n", fn);
   // convert to 'ALLEGRO_FS_ENTRY' (also makes fully qualified path)
   ALLEGRO_FS_ENTRY *FS_fname = al_create_fs_entry(fn);
   sprintf(fn, "%s", al_get_fs_entry_name(FS_fname));
   //printf("FS_fn:'%s'  title:'%s' ext:'%s'  mode:%d\n", fn, title, wext, mode);
   ALLEGRO_FILECHOOSER *afc = al_create_native_file_dialog(fn, title, wext, mode);
   if (afc==NULL) printf("failed to create native filechooser dialog\n");

   if (al_show_native_file_dialog(display, afc))
   {
      if (al_get_native_file_dialog_count(afc) == 1)
      {
         const char * r = al_get_native_file_dialog_path(afc, 0);
         sprintf(fn, "%s", r);

         //enforce extension
         if (save)
         {
            printf("save file name and path is:%s\n", fn);
            ALLEGRO_PATH *tp = al_create_path(fn);
            const char * pfn = al_get_path_filename(tp);
            if (pfn == NULL)
            {
               printf("filename is NULL??\n");
               al_destroy_native_file_dialog(afc);
               return 0;
            }
            printf("save file name is:%s\n", pfn);


            const char * pfe = al_get_path_extension(tp);
            if (pfe == NULL)
            {
               printf("extension is NULL\n");
               al_set_path_extension(tp, ext);
            }

            if (strcmp(ext, pfe) != 0)
            {
               printf("extensions do not match [%s] [%s] \n", pfe, ext);
               al_set_path_extension(tp, ext);
            }

            printf("extension is:%s\n", pfe);
            sprintf(fn, "%s", al_path_cstr(tp, ALLEGRO_NATIVE_PATH_SEP));
            // printf("final path and filename is:%s\n", fn);

         }

         al_destroy_native_file_dialog(afc);
         return 1;
      }
   }
   //else printf("file select cancelled\n" );
   al_destroy_native_file_dialog(afc);
   return 0;
}













































































// edit text stuff
void show_cursor(char *f, int cursor_pos, int xpos_c, int ypos, int cursor_color, int restore)
{
   char msg[1024];
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
   x = cursor_col*8+xpos_c - strlen(dt[cursor_row])*4;
   y = ypos+cursor_row*8;

   if (restore) // black background, text color text
   {
      al_draw_filled_rectangle(x, y, x+8, y+8, mC.pc[0]);
      al_draw_text(mF.pr8, mC.pc[cursor_color], x, y, 0, msg);
   }
   else // red background, black text
   {
      al_draw_filled_rectangle(x, y, x+8, y+8, mC.pc[10]);
      al_draw_text(mF.pr8, mC.pc[0], x, y, 0, msg);
   }
}



void edit_server_name(int type, int x, int y)
{
   char fst[80];
   strcpy(fst, m_serveraddress);
   int char_count = strlen(fst);
   int cursor_pos=0;
   int old_cp=0;
   int blink_count = 3;
   int blink_counter = 0;
   while (mI.key[ALLEGRO_KEY_ENTER][0]) mwEQ.proc_event_queue();
   int quit = 0;
   while (!quit)
   {
      int tx = mwD.SCREEN_W/2;
      int ty1 = mwD.SCREEN_H/2;
      int w = (char_count+1)*4;

      if (type == 1)
      {
         tx = x;
         ty1 = y+20;

      }

      al_flip_display();
      // clear text background
      al_draw_filled_rectangle(tx-w-8, ty1-4-2, tx+w+18, ty1+4+3, mC.pc[0]);

      al_draw_text(mF.pr8, mC.pc[15], tx, ty1-14, ALLEGRO_ALIGN_CENTER, "Set Server IP or Hostname");
      // frame text
      al_draw_rectangle       (tx-w-1, ty1-4-1, tx+w+6, ty1+6, mC.pc[15], 1);

      rtextout_centre(mF.pr8, NULL, fst, tx, ty1+1, 15, 1, 1);

      if (blink_counter++ < blink_count) show_cursor(fst, cursor_pos, tx, ty1-3, 15, 0);
      else show_cursor(fst, cursor_pos, tx, ty1-3, 15, 1);
      if (blink_counter> blink_count*2) blink_counter = 0;

      if (cursor_pos != old_cp)
      {
         show_cursor(fst, old_cp, tx, ty1-3, 15, 1); // erase old blinking cursor if moved
         old_cp = cursor_pos;
         blink_counter = 0;
      }

      al_rest(.08);
      mwEQ.proc_event_queue();
      if (mI.key[ALLEGRO_KEY_RIGHT][0])
      {
         if (++cursor_pos > char_count) cursor_pos = char_count;
      }
      if (mI.key[ALLEGRO_KEY_LEFT][0])
      {
         if (--cursor_pos < 0) cursor_pos = 0;
      }
      if ((mI.key[ALLEGRO_KEY_DELETE][0]) && (cursor_pos < char_count))
      {
         for (int a = cursor_pos; a < char_count; a++)
           fst[a]=fst[a+1];
         --char_count;
         fst[char_count] = (char)NULL; // set last to NULL
      }
      if ((mI.key[ALLEGRO_KEY_BACKSPACE][0]) && (cursor_pos > 0))
      {
         cursor_pos--;
         for (int a = cursor_pos; a < char_count; a++)
           fst[a]=fst[a+1];
         char_count--;
         fst[char_count] = (char)NULL; // set last to NULL
      }

      int k = mI.key_pressed_ASCII;
      if ((k>31) && (k<127)) // insert if alphanumeric or return
      {
         // move over to make room
         for (int a = char_count; a>=cursor_pos; a--)
            fst[a+1]=fst[a];

         // set char
         fst[cursor_pos] = k;

         // inc both
         cursor_pos++;
         char_count++;

         fst[char_count] = (char)NULL; // set last to NULL
      }
      if (mI.key[ALLEGRO_KEY_ENTER][3])
      {
         strcpy(m_serveraddress, fst);
         quit = 1;
      }

      if (mI.key[ALLEGRO_KEY_ESCAPE][3]) quit = 1;

   }
}


int edit_lift_name(int lift, int y1, int x1, char *fst)
{
   int cursor_pos=0;
   int old_cp=0;
   int blink_count = 3;
   int blink_counter = 0;
   int a=0;
   int char_count = strlen(fst);
   while (1)
   {
      al_flip_display();
      al_rest(0.05);


      int x2 = x1 + Lift.cur[lift].w -1;
      int y2 = y1 + Lift.cur[lift].h -1;
      int tx = ((x1+x2)/2);
      int ty1 = ((y1+y2)/2) - 3;

      //int color = lifts[lift].color;

      int color = (Lift.stp[lift][0].type >> 28) & 15;


      // draw updated lift
      for (a=0; a<10; a++)
        al_draw_rounded_rectangle(x1+a, y1+a, x2-a, y2-a, 4, 4, mC.pc[color + ((9 - a)*16)], 2 );
      al_draw_filled_rectangle(x1+a, y1+a, x2-a, y2-a, mC.pc[color] );
      al_draw_text(mF.pr8, mC.pc[color+160], tx, ty1, ALLEGRO_ALIGN_CENTRE, fst);


      if (blink_counter++ < blink_count) show_cursor(fst, cursor_pos, tx, ty1, 15, 0);
      else show_cursor(fst, cursor_pos, tx, ty1, 15, 1);
      if (blink_counter> blink_count*2) blink_counter = 0;

      if (cursor_pos != old_cp)
      {
         show_cursor(fst, old_cp, tx, ty1, 15, 1); // erase old blinking cursor if moved
         old_cp = cursor_pos;
         blink_counter = 0;
      }

      mwEQ.proc_event_queue();
      if (mI.key[ALLEGRO_KEY_RIGHT][0])
      {
         if (++cursor_pos >= char_count) cursor_pos = char_count-1;
      }
      if (mI.key[ALLEGRO_KEY_LEFT][0])
      {
         if (--cursor_pos < 0) cursor_pos = 0;
      }
      if ((mI.key[ALLEGRO_KEY_DELETE][0]) && (cursor_pos < char_count))
      {
         for (a = cursor_pos; a < char_count; a++)
           fst[a]=fst[a+1];
         char_count--;
         // set last to NULL
         fst[char_count] = (char)NULL;
      }
      if ((mI.key[ALLEGRO_KEY_BACKSPACE][0]) && (cursor_pos > 0))
      {
         cursor_pos--;
         for (a = cursor_pos; a < char_count; a++)
           fst[a]=fst[a+1];
         char_count--;
         // set last to NULL
         fst[char_count] = (char)NULL;
      }

      int k = mI.key_pressed_ASCII;
      if ((k>31) && (k<127)) // insert if alphanumeric or return
      {
         // move over to make room
         for (a = char_count; a>=cursor_pos; a--)
            fst[a+1]=fst[a];

         // set char
         fst[cursor_pos] = k;

         // inc both
         cursor_pos++;
         char_count++;

         // set last to NULL
         fst[char_count] = (char)NULL;
      }
      if (mI.key[ALLEGRO_KEY_ENTER][3]) return 1;
      if (mI.key[ALLEGRO_KEY_ESCAPE][3]) return 0;
   }
}






