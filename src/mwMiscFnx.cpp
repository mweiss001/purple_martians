// mwMiscFnx.cpp

#include "pm.h"
#include "mwMiscFnx.h"
#include "mwWindowManager.h"
#include "mwDisplay.h"
#include "mwFont.h"
#include "mwBitmap.h"
#include "mwLift.h"
#include "mwWidgets.h"
#include "mwColor.h"
#include "mwTriggerEvent.h"
#include "mwInput.h"
#include "mwEventQueue.h"
#include "mwItems.h"
#include "mwEnemy.h"
#include "mwScreen.h"
#include "mwNetgame.h"

mwMiscFnx mMiscFnx;


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


void mwMiscFnx::chop_first_x_char(char *str, int n)
{
   char tmp[200];
   for(int x=n; x < (signed int)strlen(str)+1; x++)
      tmp[x-n] = str[x]; // chop first n
   strcpy(str, tmp);
}



void mwMiscFnx::get_tag_text(char *str, char *res, int show)
{
   if (show) printf("\nget tag initial %s\n", str);

   // get first tag
   char * pch1 = strchr(str, '[');
   char * pch2 = strchr(str, ']');
   int p1 = pch1-str;
   int p2 = pch2-str;
   int plen = p2-p1;

   if (show) printf("p1:%d  p2:%d  plen:%d\n", p1, p2, plen);

   if (plen < 10) // ???
   {
       for(int j=0; j<plen; j++)
          res[j] = str[j+p1+1];
       res[plen-1] = 0;
       if (show) printf("tag text [%s]\n", res);
       mMiscFnx.chop_first_x_char(str, p2+1);
   }
   if (show) printf("get tag final %s\n", str);
}




int mwMiscFnx::get_tag_text2(char *str, char *res, char *res1, int show)
{
   if (show) printf("Initial string:%s\n", str);

   // get first tag - from space to :
   char * pch1 = strchr(str, ' ');
   char * pch2 = strchr(str, ':');
   if ((pch1 == NULL) || (pch2 == NULL))
   {
      printf("Did not find 1st tag\n");
      return 0;
   }
   else
   {
      int p1 = pch1-str;
      int p2 = pch2-str;
      int plen = p2-p1;

      //if (show) printf("p1:%d  p2:%d  plen:%d\n", p1, p2, plen);

      if (plen < 20) // ???
      {
          for(int j=0; j<plen; j++)
             res[j] = str[j+p1+1];
          res[plen-1] = 0;
          if (show) printf("First tag:'%s'\n", res);
          mMiscFnx.chop_first_x_char(str, p2+1);
      }
      if (show) printf("Final string after chop:'%s'\n", str);
   }

   // get 2nd tag - from [ to ]
   pch1 = strchr(str, '[');
   pch2 = strchr(str, ']');
   if ((pch1 == NULL) || (pch2 == NULL))
   {
      printf("Did not find 2nd tag\n");
      return 0;
   }
   else
   {
      int p1 = pch1-str;
      int p2 = pch2-str;
      int plen = p2-p1;

      // if (show) printf("p1:%d  p2:%d  plen:%d\n", p1, p2, plen);
      if (plen < 20) // ???
      {
          for(int j=0; j<plen; j++)
             res1[j] = str[j+p1+1];
          res1[plen-1] = 0;
          if (show) printf("2nd tag:'%s'\n", res1);
          mMiscFnx.chop_first_x_char(str, p2+1);
      }
      if (show) printf("Final string after chop:'%s'\n", str);
   }
   return 1;
}















































int mwMiscFnx::exit_level_editor_dialog(void)
{
   while (mInput.key[ALLEGRO_KEY_ESCAPE][0]) mEventQueue.proc();

   al_set_target_backbuffer(mDisplay.display);
   al_show_mouse_cursor(mDisplay.display);

   int quit = 0;
   int ret = 0;

   int w = 200, h = 100;
   int x = (mDisplay.SCREEN_W - w) /2;
   int y = (mDisplay.SCREEN_H - h) /2;

   int xc = x+w/2;
   int xa = xc-84;
   int xb = xc+84;
   int bts = 16;

   while (!quit)
   {
      al_draw_filled_rectangle(x, y, x+w, y+h, mColor.pc[10+128+64]);

      for (int a=0; a<10; a++)
         al_draw_rounded_rectangle(x+a, y+a, x+w-a, y+h-a, 1, 1, mColor.pc[10+a*16], 1);
      al_draw_text(mFont.pr8, mColor.pc[15], xc, y+14, ALLEGRO_ALIGN_CENTER, "Exit Level Editor?");

      int bc = 15;

      int by1 = y+30;

      if (ret == 0) bc = 10;
      else bc = 15;

      if (mWidget.buttont(xa, by1, xb, bts, 0,0,0,0,  0,bc,15,0, 1,0,1,0, "Save and Exit"))         { quit = 1; ret = 0; }
      if (ret == 0) al_draw_rounded_rectangle(xa-1, by1-bts-1, xb+1, by1-1, 2, 2, mColor.pc[10], 2);

      by1+=4;

      if (ret == 1) bc = 10;
      else bc = 15;
      if (mWidget.buttont(xa, by1, xb, bts, 0,0,0,0,  0,bc,15,0, 1,0,1,0, "Exit Without Saving"))   { quit = 1; ret = 1; }
      if (ret == 1) al_draw_rounded_rectangle(xa-1, by1-bts-1, xb+1, by1-1, 2, 2, mColor.pc[10], 2);

      by1+=4;

      if (ret == 2) bc = 10;
      else bc = 15;
      if (mWidget.buttont(xa, by1, xb, bts, 0,0,0,0,  0,bc,15,0, 1,0,1,0, "Cancel"))                { quit = 1; ret = 2; }
      if (ret == 2) al_draw_rounded_rectangle(xa-1, by1-bts-1, xb+1, by1-1, 2, 2, mColor.pc[10], 2);

      mEventQueue.proc();
      al_flip_display();

      if (mInput.key[ALLEGRO_KEY_DOWN][0])
      {
         while (mInput.key[ALLEGRO_KEY_DOWN][0]) mEventQueue.proc();
         if (++ret > 2) ret = 2;
      }

      if (mInput.key[ALLEGRO_KEY_UP][0])
      {
         while (mInput.key[ALLEGRO_KEY_UP][0]) mEventQueue.proc();
         if (--ret < 0) ret = 0;
      }
      if (mInput.key[ALLEGRO_KEY_ENTER][0])
      {
         while (mInput.key[ALLEGRO_KEY_ENTER][0]) mEventQueue.proc();
         quit = 1;
      }
      if (mInput.key[ALLEGRO_KEY_ESCAPE][0])
      {
         while (mInput.key[ALLEGRO_KEY_ESCAPE][0]) mEventQueue.proc();
         ret = 2; // cancel
         quit = 1;
      }
   }
   //printf("ret:%d\n", ret);
   return ret;
}


void mwMiscFnx::draw_block_non_default_flags(int tn, int x, int y)
{
   int c = tn & 1023;
   al_draw_bitmap(mBitmap.btile[c], x, y, 0);
   if ((mBitmap.sa[c][0] & PM_BTILE_MOST_FLAGS) != (tn & PM_BTILE_MOST_FLAGS))
   {
      al_draw_line(x, y, x+20, y+20, mColor.pc[10], 1);
      al_draw_line(x+20, y, x, y+20, mColor.pc[10], 1);
   }
}


int mwMiscFnx::enforce_limit(int val, int ll, int ul)
{
   if (val < ll) val = ll;
   if (val > ul) val = ul;
   return val;
}

int mwMiscFnx::check_limit(int val, int ll, int ul)
{
   if (val < ll) return 1;
   if (val > ul) return 1;
   return 0;
}






void mwMiscFnx::swap_int(int *i1, int* i2)
{
   int it = *i1;
   *i1 = *i2;
   *i2 = it;
}


float mwMiscFnx::mdw_rnd(float rmin, float rmax)
{
   int imin = (int) (rmin * 100.0);
   int irng = (int) ((rmax-rmin) * 100.0);
   int s = imin + rand() % irng;
   float res = (float)s / 100.0;
   return res;
}









// packs two 16 bit ints into one 32 bit int
void mwMiscFnx::set_int_3216(int &I32, int H16, int L16)
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
void mwMiscFnx::get_int_3216(int I32, int &H16, int &L16)
{
   H16 = I32 >> 16; // shift high into low
   H16 = H16 & 0b00000000000000001111111111111111; // clear high

   L16 = I32 & 0b00000000000000001111111111111111; // clear high
}









void mwMiscFnx::printBits(size_t const size, void const * const ptr)
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

float mwMiscFnx::get_sproingy_jump_height(int num)
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
int mwMiscFnx::get_sp(float jh)
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







int mwMiscFnx::get_block_range(const char *txt, int *x1, int *y1, int *x2, int *y2, int type)
{
   mScreen.init_level_background(0);
   int tx = mDisplay.SCREEN_W/2;
   int quit = 0;
   int ret = 0;
   while (!quit)
   {
      mwWM.redraw_level_editor_background(0);
      crosshairs_full(mwWM.gx*20+10, mwWM.gy*20+10, 15, 1);
      mScreen.get_new_screen_buffer(3, 0, 0);

      al_draw_filled_rectangle(tx-90, 70, tx+90, 170, mColor.pc[0]);
      al_draw_rectangle(       tx-90, 70, tx+90, 170, mColor.pc[15], 1);

      al_draw_text(mFont.pr8, mColor.pc[9],   tx, 72,  ALLEGRO_ALIGN_CENTER, "Draw a new");
      al_draw_text(mFont.pr8, mColor.pc[10],  tx, 80,  ALLEGRO_ALIGN_CENTER, txt);
      al_draw_text(mFont.pr8, mColor.pc[9],   tx, 88,  ALLEGRO_ALIGN_CENTER, "by clicking and");
      al_draw_text(mFont.pr8, mColor.pc[9],   tx, 96,  ALLEGRO_ALIGN_CENTER, "dragging with the");
      al_draw_text(mFont.pr8, mColor.pc[9],   tx, 104, ALLEGRO_ALIGN_CENTER, "left mouse button");
      al_draw_text(mFont.pr8, mColor.pc[14],  tx, 130, ALLEGRO_ALIGN_CENTER, "Cancel with <ESC>");
      al_draw_text(mFont.pr8, mColor.pc[14],  tx, 138, ALLEGRO_ALIGN_CENTER, "or right mouse button");
      al_draw_textf(mFont.pr8, mColor.pc[15], tx, 150, ALLEGRO_ALIGN_CENTER, "x:%2d y:%2d", mwWM.gx, mwWM.gy);

      if (mInput.mouse_b[1][0])
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
      while ((mInput.mouse_b[2][0]) || (mInput.key[ALLEGRO_KEY_ESCAPE][0]))
      {
         mEventQueue.proc(); // wait for release
         quit = 1;
         ret = 0;
      }
   }
   return ret;
}

















































int mwMiscFnx::getxy(const char *txt, int obj_type, int sub_type, int num)
{
   int tx = mDisplay.SCREEN_W/2;

   // in case these are needed for lifts
   int lift = sub_type;
   int step = num;

   int retval=0;
   int quit=0;

   while (mInput.mouse_b[1][0]) mEventQueue.proc(); // wait for release

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
      original_dx = mLift.stp[lift][step].x;
      original_dy = mLift.stp[lift][step].y;
   }

   while(!quit)
   {
      mwWM.redraw_level_editor_background(0);

      crosshairs_full(mwWM.gx*20+10, mwWM.gy*20+10, 15, 1);

      if ((obj_type == 90) || (obj_type == 91) || (obj_type == 92)) mEnemy.draw_vinepod_controls(num, -1); // move vinepod extended, cp1, cp2

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
         al_draw_rotated_bitmap(mBitmap.tile[mItem.item[num][1]], 10, 10, dx, dy, rot, 0);

         al_draw_line(ix, iy, dx, dy, mColor.pc[10], 1);     // connect with line
      }
      if (obj_type == 96) // set cannon or bouncer direction
      {
         int ex = mEnemy.Ef[num][0]+10;
         int ey = mEnemy.Ef[num][1]+10;
         int dx = mwWM.gx*20+10;
         int dy = mwWM.gy*20+10;
         float rot = mEnemy.Ef[num][14];
         al_draw_scaled_rotated_bitmap(mBitmap.tile[mEnemy.Ei[num][1]], 10, 10, dx, dy, 1, 1, rot, ALLEGRO_FLIP_HORIZONTAL); // draw tile
         al_draw_line(ex, ey, dx, dy, mColor.pc[10], 1);   // connect with line
      }

      mScreen.get_new_screen_buffer(3, 0, 0);

      al_draw_filled_rectangle(tx-100, 70, tx+100, 128, mColor.pc[0]);
      al_draw_rectangle(       tx-100, 70, tx+100, 128, mColor.pc[15], 1);

      al_draw_text(mFont.pr8,  mColor.pc[9],   tx, 72,  ALLEGRO_ALIGN_CENTER, "Set new Location for:");
      al_draw_text(mFont.pr8,  mColor.pc[10],  tx, 80,  ALLEGRO_ALIGN_CENTER, txt);
      al_draw_text(mFont.pr8,  mColor.pc[9],   tx, 88,  ALLEGRO_ALIGN_CENTER, "with left mouse button");
      al_draw_text(mFont.pr8,  mColor.pc[14],  tx, 110, ALLEGRO_ALIGN_CENTER, "Cancel with <ESC>");
      al_draw_text(mFont.pr8,  mColor.pc[14],  tx, 118, ALLEGRO_ALIGN_CENTER, "or right mouse button");

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
            mLift.stp[lift][step].x = mwWM.gx*20;
            mLift.stp[lift][step].y = mwWM.gy*20;
            mLift.set_lift_to_step(lift, step);   // set current step in current lift
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
            float xlen = mwWM.gx*20 - mItem.item[num][4];      // get the x distance between item and x2
            float ylen = mwWM.gy*20 - mItem.item[num][5];      // get the y distance between item and y2
            float a = atan2(ylen, xlen) + ALLEGRO_PI/2;
            mItem.item[num][10] = a * 1000;
         }
         break;
         case 96: // set cannon or bouncer direction
         {
            mEnemy.set_new_initial_direction(num, mwWM.gx*20, mwWM.gy*20);
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


      while (mInput.mouse_b[1][0])
      {
         mEventQueue.proc();
         quit = 1;
         retval = 1;  // b1 xy
      }
      while (mInput.mouse_b[2][0])
      {
         mEventQueue.proc();
         quit = 1;
         retval = 2;  // b2 xy
      }
      while (mInput.key[ALLEGRO_KEY_ESCAPE][0])
      {
         mEventQueue.proc();
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
      if (obj_type == 98) // cloner destination
      {
          mEnemy.Ei[num][17] = original_dx;
          mEnemy.Ei[num][18] = original_dy;
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
         mLift.stp[lift][step].x = original_dx;
         mLift.stp[lift][step].y = original_dy;
      }
   }
   return retval;
}



int mwMiscFnx::round20(int val) // pass it an int and it will round it to the nearest 20
{
   int m = val%20;
   if (m<10) return (val - m);
   else return val + (20-m);
}







int mwMiscFnx::get_item(int obj_type, int sub_type, int num )
{
   mScreen.init_level_background(0);
   int tx = mDisplay.SCREEN_W/2;

   int ret_item=0;
   int quit=0;
   int mouse_on_item = 0;

   int itx=0, ity=0;

   int x2 = mItem.item[num][4]+10; // get the originating door's location
   int y2 = mItem.item[num][5]+10;

   while (mInput.mouse_b[1][0]) mEventQueue.proc(); // wait for release

   while(!quit)
   {
      mwWM.redraw_level_editor_background(0);
      crosshairs_full(mwWM.gx*20+10, mwWM.gy*20+10, 15, 1);

      if (mouse_on_item) al_draw_line(x2, y2, itx+10, ity+10, mColor.pc[10], 2);

      mScreen.get_new_screen_buffer(3, 0, 0);

      al_draw_filled_rectangle(tx-110, 78, tx+110, 146, mColor.pc[0]);
      al_draw_rectangle(       tx-110, 78, tx+110, 146, mColor.pc[15], 1);

      al_draw_text(mFont.pr8,  mColor.pc[9],  tx, 80,  ALLEGRO_ALIGN_CENTER, "Select a Door Object");
      al_draw_text(mFont.pr8,  mColor.pc[9],  tx, 88,  ALLEGRO_ALIGN_CENTER, "with left mouse button");
      al_draw_text(mFont.pr8,  mColor.pc[14], tx, 110, ALLEGRO_ALIGN_CENTER, "Cancel with <ESC>");
      al_draw_text(mFont.pr8,  mColor.pc[14], tx, 118, ALLEGRO_ALIGN_CENTER, "or right mouse button");

      if (mouse_on_item) al_draw_textf(mFont.pr8, mColor.pc[15], tx, 136, ALLEGRO_ALIGN_CENTER, "Door Object:%d", ret_item);
      else               al_draw_textf(mFont.pr8, mColor.pc[15], tx, 136, ALLEGRO_ALIGN_CENTER, "No Door Object Selected");


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

      while (mInput.mouse_b[1][0])
      {
         mEventQueue.proc();
         quit = 1;
      }
      while ((mInput.mouse_b[2][0]) || (mInput.key[ALLEGRO_KEY_ESCAPE][0]))
      {
         mEventQueue.proc();
         quit = 1;
         ret_item = - 1;
      }
   } // end of while(!quit);
   if (!mouse_on_item) ret_item = -1;
   return ret_item;
}



void mwMiscFnx::rectangle_with_diagonal_lines(float x1, float y1, float x2, float y2, int spacing, int frame_color, int line_color, int clip_mode)
{
   int d = mDisplay.display_transform_double;
   if (!clip_mode) d = 1;

   al_set_clipping_rectangle(x1*d, y1*d, (x2-x1)*d, (y2-y1)*d);

   // find largest dimension
   float xd = x2-x1;
   float yd = y2-y1;
   float ld = xd;
   if (yd > ld) ld = yd;
   for (float k=-ld; k<ld; k+=spacing)
      al_draw_line(x1+k, y1-k, x1+ld+k, y1+ld-k, mColor.pc[line_color], 0);
   al_draw_rectangle(x1+0.5, y1+0.5, x2-0.5, y2-0.5, mColor.pc[frame_color], 1);
   al_reset_clipping_rectangle();
}











void mwMiscFnx::crosshairs_full(int x, int y, int color, int line_width) // draws a square and crosshairs around a full 20x20 block on level buffer
{
   al_draw_rectangle(x-10, y-10, x+11, y+11, mColor.pc[color], line_width);
   al_draw_line(0,    y, x-10, y, mColor.pc[color], line_width);
   al_draw_line(x+11, y, 1999, y, mColor.pc[color], line_width);
   al_draw_line(x,    0, x, y-10, mColor.pc[color], line_width);
   al_draw_line(x, y+11, x, 1999, mColor.pc[color], line_width);
}
void mwMiscFnx::titlex(const char *txt, int tc, int fc, int x1, int x2, int y)
{
   for (int x=0; x<15; x++)
      al_draw_line(x1, y+x, x2, y+x, mColor.pc[fc+(x*16)], 1);
   al_draw_text(mFont.pr8, mColor.pc[tc], (x1+x2)/2, y+2, ALLEGRO_ALIGN_CENTER,  txt);
}


int mwMiscFnx::mw_file_select(const char * title, char * fn, const char * ext, int save)
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

   if (al_show_native_file_dialog(mDisplay.display, afc))
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
void mwMiscFnx::show_cursor(char *f, int cursor_pos, int xpos_c, int ypos, int cursor_color, int restore)
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
      al_draw_filled_rectangle(x, y, x+8, y+8, mColor.pc[0]);
      al_draw_text(mFont.pr8, mColor.pc[cursor_color], x, y, 0, msg);
   }
   else // red background, black text
   {
      al_draw_filled_rectangle(x, y, x+8, y+8, mColor.pc[10]);
      al_draw_text(mFont.pr8, mColor.pc[0], x, y, 0, msg);
   }
}



void mwMiscFnx::edit_server_name(int x, int y)
{
   int tc = 10;

   char fst[80];
   strcpy(fst, mNetgame.m_serveraddress);
   int char_count = strlen(fst);
   int cursor_pos=0;
   int old_cp=0;
   int blink_count = 3;
   int blink_counter = 0;
   while (mInput.key[ALLEGRO_KEY_ENTER][0]) mEventQueue.proc();
   int quit = 0;
   while (!quit)
   {
      int tx = mDisplay.SCREEN_W/2;
      int ty1 = mDisplay.SCREEN_H/2;
      int w = (char_count+1)*4;

      tx = x;
      ty1 = y+20;



      al_flip_display();
      // clear text background
      al_draw_filled_rectangle(tx-w-8, ty1-4-2, tx+w+18, ty1+4+3, mColor.pc[0]);

//      al_draw_text(mFont.pr8, mColor.pc[15], tx, ty1-14, ALLEGRO_ALIGN_CENTER, "Set Server IP or Hostname");
      // frame text
      al_draw_rectangle       (tx-w-1, ty1-4-1, tx+w+6, ty1+6, mColor.pc[tc], 1);

      mScreen.rtextout_centre(mFont.pr8, NULL, fst, tx, ty1+1, tc, 1, 1);

      if (blink_counter++ < blink_count) show_cursor(fst, cursor_pos, tx, ty1-3, tc, 0);
      else show_cursor(fst, cursor_pos, tx, ty1-3, tc, 1);
      if (blink_counter> blink_count*2) blink_counter = 0;

      if (cursor_pos != old_cp)
      {
         show_cursor(fst, old_cp, tx, ty1-3, tc, 1); // erase old blinking cursor if moved
         old_cp = cursor_pos;
         blink_counter = 0;
      }

      al_rest(.08);
      mEventQueue.proc();
      if (mInput.key[ALLEGRO_KEY_RIGHT][0])
      {
         if (++cursor_pos > char_count) cursor_pos = char_count;
      }
      if (mInput.key[ALLEGRO_KEY_LEFT][0])
      {
         if (--cursor_pos < 0) cursor_pos = 0;
      }
      if ((mInput.key[ALLEGRO_KEY_DELETE][0]) && (cursor_pos < char_count))
      {
         for (int a = cursor_pos; a < char_count; a++)
           fst[a]=fst[a+1];
         --char_count;
         fst[char_count] = (char)NULL; // set last to NULL
      }
      if ((mInput.key[ALLEGRO_KEY_BACKSPACE][0]) && (cursor_pos > 0))
      {
         cursor_pos--;
         for (int a = cursor_pos; a < char_count; a++)
           fst[a]=fst[a+1];
         char_count--;
         fst[char_count] = (char)NULL; // set last to NULL
      }

      int k = mInput.key_pressed_ASCII;
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
      if (mInput.key[ALLEGRO_KEY_ENTER][3])
      {
         strcpy(mNetgame.m_serveraddress, fst);
         quit = 1;
      }

      if (mInput.key[ALLEGRO_KEY_ESCAPE][3]) quit = 1;

   }
}


int mwMiscFnx::edit_lift_name(int lift, int y1, int x1, char *fst)
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


      int x2 = x1 + mLift.cur[lift].w -1;
      int y2 = y1 + mLift.cur[lift].h -1;
      int tx = ((x1+x2)/2);
      int ty1 = ((y1+y2)/2) - 3;

      //int color = lifts[lift].color;

      int color = (mLift.stp[lift][0].type >> 28) & 15;


      // draw updated lift
      for (a=0; a<10; a++)
        al_draw_rounded_rectangle(x1+a, y1+a, x2-a, y2-a, 4, 4, mColor.pc[color + ((9 - a)*16)], 2 );
      al_draw_filled_rectangle(x1+a, y1+a, x2-a, y2-a, mColor.pc[color] );
      al_draw_text(mFont.pr8, mColor.pc[color+160], tx, ty1, ALLEGRO_ALIGN_CENTRE, fst);


      if (blink_counter++ < blink_count) show_cursor(fst, cursor_pos, tx, ty1, 15, 0);
      else show_cursor(fst, cursor_pos, tx, ty1, 15, 1);
      if (blink_counter> blink_count*2) blink_counter = 0;

      if (cursor_pos != old_cp)
      {
         show_cursor(fst, old_cp, tx, ty1, 15, 1); // erase old blinking cursor if moved
         old_cp = cursor_pos;
         blink_counter = 0;
      }

      mEventQueue.proc();
      if (mInput.key[ALLEGRO_KEY_RIGHT][0])
      {
         if (++cursor_pos >= char_count) cursor_pos = char_count-1;
      }
      if (mInput.key[ALLEGRO_KEY_LEFT][0])
      {
         if (--cursor_pos < 0) cursor_pos = 0;
      }
      if ((mInput.key[ALLEGRO_KEY_DELETE][0]) && (cursor_pos < char_count))
      {
         for (a = cursor_pos; a < char_count; a++)
           fst[a]=fst[a+1];
         char_count--;
         // set last to NULL
         fst[char_count] = (char)NULL;
      }
      if ((mInput.key[ALLEGRO_KEY_BACKSPACE][0]) && (cursor_pos > 0))
      {
         cursor_pos--;
         for (a = cursor_pos; a < char_count; a++)
           fst[a]=fst[a+1];
         char_count--;
         // set last to NULL
         fst[char_count] = (char)NULL;
      }

      int k = mInput.key_pressed_ASCII;
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
      if (mInput.key[ALLEGRO_KEY_ENTER][3]) return 1;
      if (mInput.key[ALLEGRO_KEY_ESCAPE][3]) return 0;
   }
}








