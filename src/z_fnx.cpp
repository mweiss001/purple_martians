// z_fnx.cpp
#include "pm.h"
#include "z_fnx.h"
#include "z_log.h"
#include "z_player.h"
#include "n_netgame.h"
#include "mwFont.h"
#include "mwBitmap.h"
#include "z_lift.h"
#include "mwGameMovesArray.h"
#include "mwColor.h"
#include "mwPMEvent.h"
#include "mwInput.h"
#include "mwDisplay.h"
#include "mwEventQueue.h"
#include "z_menu.h"
#include "mwProgramState.h"
#include "z_item.h"
#include "z_enemy.h"
#include "z_level.h"
#include "z_config.h"
#include "z_main.h"
#include "mwShots.h"


int check_and_draw(double x1, double y1, double line_length, double line_xinc, double line_yinc, double za, double zb, int col, float thickness, int &segments_drawn, int &lco)
{
   int debug_print = 0;
   char t[1024] = {0};
   sprintf(t, "\n");
   int skip = 0;
   int end_adj = 0;
   // these checks mean that the segment is entirely not on the line
   if (za > line_length) skip = 1; // za after line end
   if (zb < 0)           skip = 1; // zb before line start

   if (skip)
   {
      if (debug_print) sprintf(t, "skip\n");
   }
   else  // at least one point is on the line
   {
      if (za < 0) // za is before line start
      {
         if (debug_print) sprintf(t, "start_adj\n");
         za = 0;
      }
      if (zb > line_length) // zb is after line end
      {
         lco = line_length - zb;
         if (debug_print) sprintf(t, "end_adj - lco:%d\n", lco);
         zb = line_length;
         end_adj = 1;
      }
      double xa = x1 + za * line_xinc; // convert to screen coordinates
      double ya = y1 + za * line_yinc;
      double xb = x1 + zb * line_xinc;
      double yb = y1 + zb * line_yinc;
      if (debug_print) printf("%s   drawn at za:%3.0f zb:%3.0f\n", t, za, zb);
      al_draw_line(xa, ya, xb, yb, mC.pc[col], thickness);
      segments_drawn++;
   }
   return end_adj;
}

int mw_draw_line3(double x1, double y1, double x2, double y2, float thickness, int c0_val, int c0_col, int c1_val, int c1_col, int c2_val, int c2_col, int &line_color_offset)
{
   int debug_print = 0;

   if (debug_print) printf("\nmw_dl x1:%3.1f y1:%3.1f x2:%3.1f y2:%3.1f ", x1, y1, x2, y2);

   int done = 0;
   int lco = 0; // local lco to pass to sub fucntion
   int segments_drawn = 0;

   int color_span = c0_val + c1_val + c2_val;
   if ((color_span < 1) || ((c1_col == 0) && (c2_col == 0))) al_draw_line(x1, y1, x2, y2, mC.pc[c0_col], thickness);
   else
   {
      if (debug_print) printf("lco:%d color_span:%d\n", line_color_offset, color_span); // line_color_offset is in pixels, from 0 to color_span
      double line_xspan = x2-x1;
      double line_yspan = y2-y1;
      double line_length = sqrt(pow(line_xspan, 2) + pow(line_yspan, 2));
      if (line_length != 0)
      {
         double line_xinc = line_xspan / line_length; // one unit of these are one unit of vector move
         double line_yinc = line_yspan / line_length;
         int section = 0;
         while (!done)
         {
            if (debug_print) printf("Section:%d\n", section);
            double section_z_start = ((section-1) * color_span) - line_color_offset;

            double za = section_z_start;   // start position for drawing 1st segment
            double zb = za + c0_val;
            if (debug_print) printf("1st segment za:%3.0f zb:%3.0f ", za, zb);
            if (check_and_draw(x1, y1, line_length, line_xinc, line_yinc, za, zb, c0_col, thickness, segments_drawn, lco))
            {
                done = 1;
                line_color_offset = lco - c2_val - c1_val;
            }
            if (!done)
            {
               za = section_z_start + c0_val;   // start position for drawing 2nd segment
               zb = za + c1_val;
               if (debug_print) printf("2nd segment za:%3.0f zb:%3.0f ", za, zb);
               if (check_and_draw(x1, y1, line_length, line_xinc, line_yinc, za, zb, c1_col, thickness, segments_drawn, lco))
               {
                   done = 1;
                   line_color_offset = lco - c2_val;
               }
               if (!done)
               {
                  za = section_z_start + c0_val + c1_val;   // start position for drawing 3rd segment
                  zb = za + c2_val;
                  if (debug_print) printf("3rd segment za:%3.0f zb:%3.0f ", za, zb);
                  if (check_and_draw(x1, y1, line_length, line_xinc, line_yinc, za, zb, c2_col, thickness, segments_drawn, lco))
                  {
                      done = 1;
                      line_color_offset = lco;
                  }
               }
            }
            section++;
         }
      }
   }
   return segments_drawn;
}

int mw_draw_line2(double x1, double y1, double x2, double y2, float thickness, int c0_val, int c0_col, int c1_val, int c1_col, int &line_color_offset)
{
   int debug_print = 0;

   if (debug_print) printf("\nmw_dl x1:%3.1f y1:%3.1f x2:%3.1f y2:%3.1f ", x1, y1, x2, y2);

   int done = 0;
   int lco = 0; // local lco to pass to sub fucntion
   int segments_drawn = 0;

   int color_span = c0_val + c1_val;
   if ((color_span < 1) || (c1_col == 0)) al_draw_line(x1, y1, x2, y2, mC.pc[c0_col], thickness);
   else
   {
      if (debug_print) printf("lco:%d color_span:%d\n", line_color_offset, color_span); // line_color_offset is in pixels, from 0 to color_span
      double line_xspan = x2-x1;
      double line_yspan = y2-y1;
      double line_length = sqrt(pow(line_xspan, 2) + pow(line_yspan, 2));

      if (line_length != 0)
      {
         //printf("lxs:%f lys:%f  ls:%f\n", line_xspan, line_yspan, line_span);

         double line_xinc = line_xspan / line_length; // one unit of these are one unit of vector move
         double line_yinc = line_yspan / line_length;
         int section = 0;
         while (!done)
         {
            if (debug_print) printf("Section:%d\n", section);
            double section_z_start = ((section-1) * color_span) - line_color_offset;

            double za = section_z_start;   // start position for drawing 1st segment
            double zb = za + c0_val;
            if (debug_print) printf("1st segment za:%3.0f zb:%3.0f ", za, zb);
            if (check_and_draw(x1, y1, line_length, line_xinc, line_yinc, za, zb, c0_col, thickness, segments_drawn, lco))
            {
                done = 1;
                line_color_offset = lco - c1_val;
            }
            if (!done)
            {
               za = section_z_start + c0_val;   // start position for drawing 2nd segment
               zb = za + c1_val;
               if (debug_print) printf("2nd segment za:%3.0f zb:%3.0f ", za, zb);
               if (check_and_draw(x1, y1, line_length, line_xinc, line_yinc, za, zb, c1_col, thickness, segments_drawn, lco))
               {
                   done = 1;
                   line_color_offset = lco;
               }
            }
            section++;
         }
      }
   }
   return segments_drawn;
}














void set_start_level(int s)
{
   start_level = s;
   if (start_level < 1) start_level = 1;
   if (start_level > 399) start_level = 399;
   sprintf(global_string[7][2], "Start Level (%d)", start_level);
   resume_allowed = 0;
   save_config();
}

void set_player_color(int p, int c)
{
   players[p].color = c;
   if (players[p].color < 1) players[p].color = 1;
   if (players[p].color > 15) players[p].color = 15;
}
void set_speed(void)
{
   al_set_timer_speed(mwEQ.fps_timer, 1/(float)mwPS.frame_speed);
}












int round20(int val) // pass it an int and it will round it to the nearest 20
{
   int m = val%20;
   if (m<10) return (val - m);
   else return val + (20-m);
}

void spin_shape(int tn, int x, int y, int tsx, int tsy, int tsw, int tsh, float scale, float dim, int cycle)
{
   int cti = cycle; // how many frame a full spin takes

   float ct = (int)cti; //cycle time
   float ct1 = ct/4;    // 20
   float ct2 = ct/2;    // 40
   float ct3 = ct1*3;   // 60
   float ct4 = ct;      // 80

   int tm = mwPS.frame_num % cti; // get a number from 0 to cti than increments every frame

   float tmr = (int) tm;

   float xs = 0;
   int flags = 0;


   // 80-60 = narrow to wide xs = 0 to 20 regular draw
   // 60-40 = wide to narrow xs = 20 to 0 regular draw

   // 40    = narrowest      xs = 0       flip draw from regular to reverse

   // 40-20 = narrow to wide xs = 0 to 20 reverse draw
   // 20-0  = wide to narrow xs = 20 to 0 reverse draw

   // 0     = narrowest      xs = 0       flip draw from reverse to regular


   if (tmr > ct2)
   {
      if (tmr>ct3) xs = ct4-tmr;  // 80-60 ---> 0-20
      else         xs = tmr-ct2;  // 60-40 ---> 20-0
   }
   else
   {
      if (tmr>ct1) xs = ct2-tmr;    // 40-20 ---> 0-20
      else xs = tmr;                // 20-0  ---> 20-0
      flags |= ALLEGRO_FLIP_HORIZONTAL;
   }

   // scale xs (if ct == 80 scale = 1)
   // or more accurately full width of tile (20) * 4
   float xscale = (tsw*4)/ct;
   xs *= xscale;

   float ys=tsh; // y scale is the same as the source height of the tile


   // optionally scale the entire thing
   xs *= scale;
   ys *= scale;

   // get draw offsets based on scale of final tile
   int xo = 10 - (xs/2); // x offset
   int yo = 10 - (ys/2); // x offset

   ALLEGRO_COLOR c2 = al_map_rgba_f(dim, dim, dim, 1.0); // show dimmer on back side

   if (flags == 0) al_draw_scaled_bitmap(       mwB.tile[tn],     tsx, tsy, tsw, tsh, x+xo, y+yo, xs, ys, flags);
   else            al_draw_tinted_scaled_bitmap(mwB.tile[tn], c2, tsx, tsy, tsw, tsh, x+xo, y+yo, xs, ys, flags);

}



void change_block(int x, int y, int block)
{
   l[x][y] = block;
   al_set_target_bitmap(mwB.level_background);
   al_draw_filled_rectangle(x*20, y*20, x*20+20, y*20+20, mC.pc[0]);
   al_draw_bitmap(mwB.btile[block & 1023], x*20, y*20, 0);
}


void get_hostname(int print)
{
   sprintf(msg, "hostname");
   FILE *fp;
   fp = popen(msg,"r");

   int loop = 0;
   int ch = fgetc(fp);
   while((ch != '\n') && (ch != EOF))
   {
      mwPS.local_hostname[loop] = ch;
      loop++;
      ch = fgetc(fp);
   }
   mwPS.local_hostname[loop] = 0;

   if (print) printf("Local hostname:%s\n", mwPS.local_hostname);

   fclose(fp);
}


void mw_get_text_dimensions(ALLEGRO_FONT *f, const char* txt, int &bx, int &by, int &bw, int &bh)
{
    // first get from the allegro method
   al_get_text_dimensions(f, txt, &bx, &by, &bw, &bh);

   // then override for my nefarious purposes!
   if (f == mF.pixl)    { by = 4; bh = 5; }
   if (f == mF.pr8)  { by = 0; bh = 8; bx = 0; bw = 8*strlen(txt); }
   if (f == mF.bltn) { by = 0; bh = 8; }
}

void make_palette(void)
{
   // printf("make palette\n");

   mC.pc[0]  = al_map_rgb(  0,   0,   0); // black
   mC.pc[1]  = al_map_rgb(191, 108, 232); // alt purple 1
   mC.pc[2]  = al_map_rgb(136,  32, 172); // alt purple 2
   mC.pc[3]  = al_map_rgb( 60, 127, 255); // alt blue
   mC.pc[4]  = al_map_rgb(224,  28,  72); // alt red
   mC.pc[5]  = al_map_rgb(255,   0, 232); // pink
   mC.pc[6]  = al_map_rgb(255, 191, 127); // taan
   mC.pc[7]  = al_map_rgb(255, 127,   0); // orange
   mC.pc[8]  = al_map_rgb(127,   0, 255); // original purple
   mC.pc[9]  = al_map_rgb(  0, 255, 127); // alt green
   mC.pc[10] = al_map_rgb(255,   0,   0); // red
   mC.pc[11] = al_map_rgb(  0, 255,   0); // green
   mC.pc[12] = al_map_rgb(  0,   0, 255); // blue
   mC.pc[13] = al_map_rgb(  0, 255, 255); // lt blue
   mC.pc[14] = al_map_rgb(255, 255,   0); // yellow
   mC.pc[15] = al_map_rgb(255, 255, 255); // white

   // special case reversed white (0)
   for (int x=1; x<16; x++)
   {
      int c = (x+1)*16 - 1;
      mC.pc[x*16]  = al_map_rgb(c, c, c);
   }

   // all the other base colors from 1-15
   for (int a=1; a<16; a++) // iterate base colors
   {
      // extract r, g, b in float format
      float r, g, b;
      al_unmap_rgb_f(mC.pc[a], &r, &g, &b);

      for (int x=1; x<16; x++)
      {
         float nr = r * (1 - ((float)x/15));
         float ng = g * (1 - ((float)x/15));
         float nb = b * (1 - ((float)x/15));
         mC.pc[a+x*16]  = al_map_rgb_f(nr, ng, nb);
      }
   }

//   for (int x=0; x<16; x++)
//   {
//      unsigned char ur, ug, ub;
//      al_unmap_rgb(mC.pc[a+x*16], &ur, &ug, &ub);
//      printf("%2d %3d r:%3d g:%3d b:%3d \n", x, a+x*16, ur, ug, ub );
//   }
//   for (int x=0; x<16; x++)
//   al_draw_line(10, 10+x*2, 200, 10+x*2, mC.pc[10+x*16], 2);
//   int sz = 16;
//   al_draw_rectangle(0, 0, 17*sz, 17*sz, mC.pc[15], 2);
//
//   for (int a=0; a<16; a++)
//      for (int b=0; b<16; b++)
//         al_draw_filled_rectangle(a*sz, b*sz, a*sz+sz, b*sz+sz, mC.pc[b*16+a]);
//   al_draw_text(mF.pr8, mC.pc[9], 400, 400, ALLEGRO_ALIGN_CENTRE, "Hello World");

}



void m_err(const char * err_msg)
{
   fprintf(stderr, "%s\n", err_msg);
   al_show_native_message_box(display, "Error", "Error:", err_msg, NULL,  ALLEGRO_MESSAGEBOX_ERROR );
}

void window_title(void)
{
//   sprintf(msg, "Purple Martians");
   sprintf(msg, "Purple Martians %s", mwPS.pm_version_string);
//   sprintf(msg, "Purple Martians %s   [%d x %d]", mwPS.pm_version_string, mwD.SCREEN_W, mwD.SCREEN_H);
//   sprintf(msg, "%d x %d", mwD.SCREEN_W, mwD.SCREEN_H);
//   sprintf(msg, "Purple Martians %s   S[%d x %d]  A[%d x %d]   [%d]", mwPS.pm_version_string, mwD.SCREEN_W, mwD.SCREEN_H,  disp_w_curr, disp_h_curr, display_transform_double);
   al_set_window_title(display, msg);
}

int is_block_empty(int x, int y, int test_block, int test_item, int test_enemy)
{
   int empty = 1; // default is empty

   if (test_block) if (l[x][y] > 31) empty = 0;

   if (test_enemy)
      for (int c=0; c<100; c++)
         if (Ei[c][0])
            if ((Efi[c][0] == al_itofix(x*20)) && (Efi[c][1] == al_itofix(y*20))) empty = 0;

   if (test_item)
      for (int c=0; c<500; c++)
         if (item[c][0])
            if ((item[c][4] == (x*20)) && (item[c][5] == (y*20))) empty = 0;

   return empty;
}




void tsw(void)
{
   al_flush_event_queue(mwEQ.event_queue);
   int quit = 0;
   while(!quit)
   {
      while (!al_is_event_queue_empty(mwEQ.event_queue))
      {
         ALLEGRO_EVENT ev;
         if (al_get_next_event(mwEQ.event_queue, &ev))
         {
             if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) fast_exit(0);
             if (ev.type == ALLEGRO_EVENT_KEY_DOWN) quit = 1;
         }
      }
   }
   while (mI.key[ALLEGRO_KEY_ESCAPE][0]) mwEQ.proc_event_queue();
}


al_fixed get_rot_from_xyinc(int EN)
{
   al_fixed xlen = Efi[EN][2];
   al_fixed ylen = Efi[EN][3];
   al_fixed angle = al_fixatan2(ylen, xlen);
   return angle - al_itofix(64);
}

al_fixed get_rot_from_PXY(int EN, int p)
{
   al_fixed xlen = players[p].PX - Efi[EN][0];
   al_fixed ylen = players[p].PY - Efi[EN][1];
   al_fixed angle = al_fixatan2(ylen, xlen);
   return angle - al_itofix(64);
}

void seek_set_xyinc(int EN)
{
   int p = find_closest_player(EN);
   al_fixed xlen = players[p].PX - Efi[EN][0];   // get the x distance between enemy and player
   al_fixed ylen = players[p].PY - Efi[EN][1];   // get the y distance between enemy and player
   al_fixed hy_dist =  al_fixhypot(xlen, ylen);     // hypotenuse distance
   al_fixed speed = Efi[EN][5];                  // speed
   al_fixed scaler = al_fixdiv(hy_dist, speed);     // get scaler
   al_fixed xinc = al_fixdiv(xlen, scaler);         // calc xinc
   al_fixed yinc = al_fixdiv(ylen, scaler);         // calc yinc
   Efi[EN][2] = xinc;
   Efi[EN][3] = yinc;
}

void seek_set_xyinc(int EN, int x, int y)
{
   al_fixed xlen = al_itofix(x) - Efi[EN][0];       // get the x distance between enemy and x
   al_fixed ylen = al_itofix(y) - Efi[EN][1];       // get the y distance between enemy and y
   al_fixed hy_dist =  al_fixhypot(xlen, ylen);     // hypotenuse distance
   al_fixed speed = Efi[EN][5];                     // speed
   al_fixed scaler = al_fixdiv(hy_dist, speed);     // get scaler (it's time... distance/speed is time)
   al_fixed xinc = al_fixdiv(xlen, scaler);         // calc xinc  (what is this? v? distance/time = speed
   al_fixed yinc = al_fixdiv(ylen, scaler);         // calc yinc
   Efi[EN][2] = xinc;
   Efi[EN][3] = yinc;
}







int find_closest_player_flapper(int EN, int dir)
{
   al_fixed prox = al_itofix(Ei[EN][17]); // al_fixed version of prox

   al_fixed d[NUM_PLAYERS]; // array of distances for each player
   for (int p=0; p<NUM_PLAYERS; p++)
   {
      d[p] = al_itofix(-1); // default result (player not in range or not active)
      if ((players[p].active) && (!players[p].paused))
      {
         al_fixed xlen = players[p].PX - Efi[EN][0]; // get x distance
         al_fixed ylen = players[p].PY - Efi[EN][1]; // get y distance
         al_fixed dist = al_fixhypot(xlen, ylen);

         al_fixed angle = al_fixatan2(ylen, xlen);

         int ngl = al_fixtoi(angle);

         // printf("angle:%d\n", ngl);

         /*
                       -64
                        |
             -127       |
           128/-128 ----+---- 0
              127       |
                        |
                       +64

        only shoots at players in a 90 degree pie slice
        45  to 135 for +xinc (-32 to +32)
        225 to 315 for -xinc (96 to -96)

        but it is actually from (96 to 128) and (-96 to -128)
        not (96 to -96)


         */


         if ((dir == 1) && (ngl > -32) && (ngl < 32) && (dist < prox)) d[p] = dist; // right
         if ((dir == 0) && ((ngl > 96) || (ngl < -96)) && (dist < prox)) d[p] = dist; // left




      }
   }

   al_fixed closest_val = al_itofix(9999);
   int closest_p = -1;
   for (int p=0; p<NUM_PLAYERS; p++)
   {
      if ((d[p] != al_itofix(-1)) && (d[p] < closest_val))
      {
         closest_val = d[p];
         closest_p = p;
      }
   }
   if (closest_val == al_itofix(9999)) return -1;    // no player in range
   else return closest_p;
}
//
//int find_closest_player_flapper(int EN, int dir)
//{
//   al_fixed width = al_itofix(Ei[EN][17]); // al_fixed version of width
//   int height  = Ei[EN][18];
//   int depth   = Ei[EN][19];
//
//   // get the enemy's trigger box y values
//   int ey = al_fixtoi(Efi[EN][1]);
//   int ey1 = ey - height;
//   int ey2 = ey + depth;
//
//   al_fixed d[NUM_PLAYERS]; // array of distances for each player
//   for (int p=0; p<NUM_PLAYERS; p++)
//   {
//      d[p] = al_itofix(-1); // default result (player not in range or not active)
//      if ((players[p].active) && (!players[p].paused))
//      {
//         // is player in the y range?
//         int py = al_fixtoi(players[p].PY)+10;
//         if ((py > ey1) && (py < ey2))
//         {
//            al_fixed xlen = players[p].PX - Efi[EN][0]; // get x distance
//            if ((!dir) && (xlen < al_itofix(0))) d[p] = -xlen; // left
//            if ( (dir) && (xlen > al_itofix(0))) d[p] =  xlen; // right
//         }
//      }
//      // check if distance is within range (width); invalidate if not
//      if (d[p] > width) d[p] = al_itofix(-1);
//   }
//
//   al_fixed closest_val = al_itofix(9999);
//   int closest_p = -1;
//   for (int p=0; p<NUM_PLAYERS; p++)
//   {
//      if ((d[p] != al_itofix(-1)) && (d[p] < closest_val))
//      {
//         closest_val = d[p];
//         closest_p = p;
//      }
//   }
//   if (closest_val == al_itofix(9999)) return -1;    // no player in range
//   else return closest_p;
//}


int find_closest_player_quad(int EN, int quad, int prox)
{
   int closest_p = -1;    // return -1 if no player in range
   al_fixed closest_val = al_itofix(3000);

   al_fixed fz = al_itofix(0);
   al_fixed fm1 = al_itofix(-1);
   al_fixed fprox = al_itofix(prox);

   al_fixed d[NUM_PLAYERS];
   for (int p=0; p<NUM_PLAYERS; p++)
   {
      d[p] = fm1; // default result (players not in quad or not active)
      if ((players[p].active) && (!players[p].paused))
      {
         al_fixed xlen = players[p].PX - Efi[EN][0];
         al_fixed ylen = players[p].PY - Efi[EN][1];

/*           normal cartesian
              I    x+  Y+        II | I
             II    x-  Y+       ----+----
             III   x-  Y-       III | IV
             IV    x+  Y-

             screen
              I    x+  Y-        II | I
             II    x-  Y-       ----+----
             III   x-  Y+       III | IV
             IV    x+  Y+ */

            if ((quad == 1) && (xlen >= fz) && (ylen <= fz)) d[p] = al_fixhypot(xlen,ylen);
            if ((quad == 2) && (xlen <= fz) && (ylen <= fz)) d[p] = al_fixhypot(xlen,ylen);
            if ((quad == 3) && (xlen <= fz) && (ylen >= fz)) d[p] = al_fixhypot(xlen,ylen);
            if ((quad == 4) && (xlen >= fz) && (ylen >= fz)) d[p] = al_fixhypot(xlen,ylen);
         }
         // check if distances are within range
         if (d[p] > fprox) d[p] = fm1;
   }
   for (int p=0; p<NUM_PLAYERS; p++)
      if ((d[p] != fm1) && (d[p] < closest_val))
      {
         closest_val = d[p];
         closest_p = p;
      }
   if (closest_val == al_itofix(3000)) return -1;    // no player in range
   else return closest_p;
}



// finds closest player
// if closest player is not within dist, returns -1
int find_closest_player_cannon(int e, int dist)
{
   int closest_player = -1; // default if no player within distance
   al_fixed hd = al_itofix(10000);
   for (int p=0; p<NUM_PLAYERS; p++)
      if ((players[p].active) && (!players[p].paused))
      {
         al_fixed h = al_fixhypot((players[p].PX - Efi[e][0]), (players[p].PY - Efi[e][1]));
         if (h < hd)
         {
             hd = h;
             closest_player = p;
         }
      }
   if (al_fixtoi(hd) < dist) return closest_player;
   else return -1;
}



int find_closest_player(int EN)
{
   int closest_player = 0; // defaults to zero (will always return a valid player)
   al_fixed hd = al_itofix(10000);
   for (int p=0; p<NUM_PLAYERS; p++)
      if ((players[p].active) && (!players[p].paused))
      {
         al_fixed h = al_fixhypot((players[p].PX - Efi[EN][0]), (players[p].PY - Efi[EN][1]));
         if (h < hd)
         {
             hd = h;
             closest_player = p;
         }
      }
   return closest_player;
}





/*

backup copies of the old method----------------------------------------------------------------------------------------------------

// returns 1 for solid block, 2 for semi-solid, or 32+lift_num for lift
int is_right_solid(int solid_x, int solid_y, int lift_check)
{
   int a, b, c, d;
   int xx = solid_x / 20 + 1;
   int yy = solid_y / 20;
   int cc = solid_y / 20 + 1;
   a = solid_y % 20;

   if (lift_check)
      for (d = 0; d<num_lifts; d++)

         if (solid_y > lifts[d].y1 - 18)
            if (solid_y < lifts[d].y2 - 2)
               if (solid_x < lifts[d].x1 - 8)
                  if (solid_x > lifts[d].x1 - 18)
                     return 32+d;

   if (a > 16)  // next block only
   {
      c = l[xx][cc];
      if ((c > 63) && (c < NUM_SPRITES)) return 1;
      if ((c > 31) && (c < NUM_SPRITES)) return 2;
   }
   if (a < 3)    // this block only
   {
      c = l[xx][yy];
      if ((c > 63) && (c < NUM_SPRITES)) return 1;
      if ((c > 31) && (c < NUM_SPRITES)) return 2;
   }
   if ((a > 2) && (a < 17)) // dual compare with ||
   {
      b = l[xx][yy];
      c = l[xx][cc];
      if ( ((b > 63) && (b < NUM_SPRITES)) || ((c > 63) && (c < NUM_SPRITES)) ) return 1;
      if ( ((b > 31) && (b < NUM_SPRITES)) || ((c > 31) && (c < NUM_SPRITES)) ) return 2;
   }
   return 0;
}




// returns 1 for solid block, 2 for semi-solid, or 32+lift_num for lift
int is_left_solid(int solid_x, int solid_y, int lift_check)
{
   int a, b, c, d;
   int xx = solid_x / 20;
   int yy = solid_y / 20;
   int cc = solid_y / 20 + 1;
   a = solid_y % 20;
   if (lift_check)
      for (d = 0; d<num_lifts; d++)
         if (solid_y > lifts[d].y1 - 18)
            if (solid_y < lifts[d].y2 - 2)
               if (solid_x < lifts[d].x2 + 2)
                  if (solid_x > lifts[d].x2 - 8)
                     return 32+d;

   if (a > 16)  // next block only
   {
      c = l[xx][cc];
      if ((c > 63) && (c < NUM_SPRITES)) return 1;
      if ((c > 31) && (c < NUM_SPRITES)) return 2;
   }
   if (a < 3)    // this block only
   {
      c = l[xx][yy];
      if ((c > 63) && (c < NUM_SPRITES)) return 1;
      if ((c > 31) && (c < NUM_SPRITES)) return 2;
   }
   if ((a > 2) && (a < 17)) // dual compare with ||
   {
      b = l[xx][yy];
      c = l[xx][cc];
      if ( ((b > 63) && (b < NUM_SPRITES)) || ((c > 63) && (c < NUM_SPRITES)) ) return 1;
      if ( ((b > 31) && (b < NUM_SPRITES)) || ((c > 31) && (c < NUM_SPRITES)) ) return 2;
   }
   return 0;
}



// returns 1 for solid block, 2 for semi-solid, or 32+lift_num for lift
int is_down_solid(int solid_x, int solid_y, int lift_check)
{
   int a, b, c, d;
   int yy = solid_y / 20 + 1;
   int cc = solid_x / 20;
   int xx = solid_x / 20 + 1;
   a = solid_x % 20;
   if (lift_check)
      for (d = 0; d<num_lifts; d++)
            if (solid_x > lifts[d].x1-18)
               if (solid_x < lifts[d].x2-2)
                  if (solid_y > lifts[d].y1 - 25)
                     if (solid_y < lifts[d].y1 - 10)
                         return d+32;

   if (a > 16)  // next block only
   {
      c = l[xx][yy];
      if ((c > 63) && (c < NUM_SPRITES)) return 1;
      if ((c > 31) && (c < NUM_SPRITES)) return 2;
   }
   if (a < 3)    // this block only
   {
      c = l[cc][yy];
      if ((c > 63) && (c < NUM_SPRITES)) return 1;
      if ((c > 31) && (c < NUM_SPRITES)) return 2;
   }
   if ((a > 2) && (a <17)) // dual compare with ||
   {
      b = l[xx][yy];
      c = l[cc][yy];
      if ( ((b > 63) && (b < NUM_SPRITES)) || ((c > 63) && (c < NUM_SPRITES)) ) return 1;
      if ( ((b > 31) && (b < NUM_SPRITES)) || ((c > 31) && (c < NUM_SPRITES)) ) return 2;
   }

   return 0;
}


// returns 1 for solid block, 2 for semi-solid, or 32+lift_num for lift
int is_up_solid(int solid_x, int solid_y, int lift_check)
{
   int yy = (solid_y - 2) / 20;
   int cc = solid_x / 20;
   int xx = solid_x / 20 + 1;
   int a = solid_x % 20;

   if (lift_check)
      for (int d = 0; d<num_lifts; d++)
         if (solid_x > lifts[d].x1 - 18)
            if (solid_x < lifts[d].x2 - 2)
               if (solid_y < lifts[d].y2 + 2)
                  if (solid_y > lifts[d].y2 - 10)
                     return d+32;

   if (a > 16)  // next block only
   {
      int c = l[xx][yy];
      if ((c > 63) && (c < NUM_SPRITES)) return 1;
      if ((c > 31) && (c < NUM_SPRITES)) return 2;
   }
   if (a < 3)    // this block only
   {
      int c = l[cc][yy];
      if ((c > 63) && (c < NUM_SPRITES)) return 1;
      if ((c > 31) && (c < NUM_SPRITES)) return 2;
   }
   if ((a > 2) && (a < 17))// dual compare with ||
   {
      int b = l[cc][yy];
      int c = l[xx][yy];
      if ( ((b > 63) && (b < NUM_SPRITES)) || ((c > 63) && (c < NUM_SPRITES)) ) return 1;
      if ( ((b > 31) && (b < NUM_SPRITES)) || ((c > 31) && (c < NUM_SPRITES)) ) return 2;
   }
   return 0;
}
backup copies of the old method----------------------------------------------------------------------------------------------------
backup copies of the old method----------------------------------------------------------------------------------------------------
backup copies of the old method----------------------------------------------------------------------------------------------------
backup copies of the old method----------------------------------------------------------------------------------------------------


*/

































// helper function for is_right_solid, is_left_solid, is_down_solid
int is_solid(int b, int c, int type)
{
   if (b == -1) // do c only
   {
      if ((type == 1) && (c & PM_BTILE_SOLID_PLAYER)) return 1;
      if ((type == 2) && (c & PM_BTILE_SOLID_ENEMY))  return 1;
      if ((type == 3) && (c & PM_BTILE_SOLID_ITEM))   return 1;
      if ((type == 4) && (c & PM_BTILE_SOLID_PBUL))   return 1;
      if ((type == 5) && (c & PM_BTILE_SOLID_EBUL))   return 1;
   }
   else
   {
      if ((type == 1) && ((b & PM_BTILE_SOLID_PLAYER) || (c & PM_BTILE_SOLID_PLAYER)))  return 1;
      if ((type == 2) && ((b & PM_BTILE_SOLID_ENEMY)  || (c & PM_BTILE_SOLID_ENEMY)))   return 1;
      if ((type == 3) && ((b & PM_BTILE_SOLID_ITEM)   || (c & PM_BTILE_SOLID_ITEM)))    return 1;
      if ((type == 4) && ((b & PM_BTILE_SOLID_PBUL)   || (c & PM_BTILE_SOLID_PBUL)))    return 1;
      if ((type == 5) && ((b & PM_BTILE_SOLID_EBUL)   || (c & PM_BTILE_SOLID_EBUL)))    return 1;
   }
   return 0;
}

// custom type for is_up_solid, to take into account semisolid exceptions
int is_solidu(int b, int c, int type)
{
   if (b == -1) // do c only
   {
      if ((type == 1) && (c & PM_BTILE_SOLID_PLAYER) && (!(c & PM_BTILE_SEMISOLID_PLAYER))) return 1;
      if ((type == 2) && (c & PM_BTILE_SOLID_ENEMY)  && (!(c & PM_BTILE_SEMISOLID_ENEMY)))  return 1;
      if ((type == 3) && (c & PM_BTILE_SOLID_ITEM)   && (!(c & PM_BTILE_SEMISOLID_ITEM)))   return 1;
      if ((type == 4) && (c & PM_BTILE_SOLID_PBUL)) return 1;
      if ((type == 5) && (c & PM_BTILE_SOLID_EBUL)) return 1;
   }
   else
   {
      if ( (type == 1) &&
           ( ((b & PM_BTILE_SOLID_PLAYER) && (!(b & PM_BTILE_SEMISOLID_PLAYER))) ||
             ((c & PM_BTILE_SOLID_PLAYER) && (!(c & PM_BTILE_SEMISOLID_PLAYER))) )
         ) return 1;
     if ( (type == 2) &&
           ( ((b & PM_BTILE_SOLID_ENEMY)  && (!(b & PM_BTILE_SEMISOLID_ENEMY))) ||
             ((c & PM_BTILE_SOLID_ENEMY)  && (!(c & PM_BTILE_SEMISOLID_ENEMY))) )
         ) return 1;
      if ( (type == 3) &&
           ( ((b & PM_BTILE_SOLID_ITEM)   && (!(b & PM_BTILE_SEMISOLID_ITEM))) ||
             ((c & PM_BTILE_SOLID_ITEM)   && (!(c & PM_BTILE_SEMISOLID_ITEM))) )
         ) return 1;
      if ((type == 4) && ((b & PM_BTILE_SOLID_PBUL)   || (c & PM_BTILE_SOLID_PBUL)))    return 1;
      if ((type == 5) && ((b & PM_BTILE_SOLID_EBUL)   || (c & PM_BTILE_SOLID_EBUL)))    return 1;
   }
   return 0;
}

// returns 1 for solid block, 32+lift_num for lift
int is_right_solid(int solid_x, int solid_y, int lift_check, int type)
{
   int xx = solid_x / 20 + 1;
   int yy = solid_y / 20;
   int cc = solid_y / 20 + 1;
   int a = solid_y % 20;

   if (lift_check)
      for (int d=0; d<num_lifts; d++)
         if ( ((type == 1) && (lifts[d].flags & PM_LIFT_SOLID_PLAYER)) ||
              ((type == 2) && (lifts[d].flags & PM_LIFT_SOLID_ENEMY )) ||
              ((type == 3) && (lifts[d].flags & PM_LIFT_SOLID_ITEM  )) )
         if (solid_y > lifts[d].y1 - 18)
            if (solid_y < lifts[d].y2 - 2)
               if (solid_x < lifts[d].x1 - 8)
                  if (solid_x > lifts[d].x1 - 18)
                     return 32+d;
   if (a > 16)  // next block only
      if (is_solid( -1, l[xx][cc], type)) return 1;

   if (a < 3)    // this block only
      if (is_solid( -1, l[xx][yy], type)) return 1;

   if ((a > 2) && (a <17)) // dual compare with ||
      if (is_solid( l[xx][yy], l[xx][cc], type)) return 1;

   return 0;
}

// returns 1 for solid block, 32+lift_num for lift
int is_left_solid(int solid_x, int solid_y, int lift_check, int type)
{
   int xx = solid_x / 20;
   int yy = solid_y / 20;
   int cc = solid_y / 20 + 1;
   int a = solid_y % 20;
   if (lift_check)
      for (int d = 0; d<num_lifts; d++)
         if ( ((type == 1) && (lifts[d].flags & PM_LIFT_SOLID_PLAYER)) ||
              ((type == 2) && (lifts[d].flags & PM_LIFT_SOLID_ENEMY )) ||
              ((type == 3) && (lifts[d].flags & PM_LIFT_SOLID_ITEM  )) )
         if (solid_y > lifts[d].y1 - 18)
            if (solid_y < lifts[d].y2 - 2)
               if (solid_x < lifts[d].x2 + 2)
                  if (solid_x > lifts[d].x2 - 8)
                     return 32+d;

   if (a > 16)  // next block only
      if (is_solid( -1, l[xx][cc], type)) return 1;

   if (a < 3)    // this block only
      if (is_solid( -1, l[xx][yy], type)) return 1;

   if ((a > 2) && (a <17)) // dual compare with ||
      if (is_solid( l[xx][yy], l[xx][cc], type)) return 1;

   return 0;
}

// returns 1 for solid block, 32+lift_num for lift
int is_down_solid(int solid_x, int solid_y, int lift_check, int type)
{
   int yy = solid_y / 20 + 1;
   int cc = solid_x / 20;
   int xx = solid_x / 20 + 1;
   int a = solid_x % 20;
   if (lift_check)
      for (int d=0; d<num_lifts; d++)
         if ( ((type == 1) && (lifts[d].flags & PM_LIFT_SOLID_PLAYER)) ||
              ((type == 2) && (lifts[d].flags & PM_LIFT_SOLID_ENEMY )) ||
              ((type == 3) && (lifts[d].flags & PM_LIFT_SOLID_ITEM  )) )
            if (solid_x > lifts[d].x1-18)
               if (solid_x < lifts[d].x2-2)
                  if (solid_y > lifts[d].y1 - 25)
                     if (solid_y < lifts[d].y1 - 10)
                         return d+32;

   if (a > 16)  // next block only
      if (is_solid( -1, l[xx][yy], type)) return 1;

   if (a < 3)    // this block only
      if (is_solid( -1, l[cc][yy], type)) return 1;

   if ((a > 2) && (a <17)) // dual compare with ||
      if (is_solid( l[xx][yy], l[cc][yy], type)) return 1;
   return 0;
}

// returns 1 for solid block, 32+lift_num for lift
int is_up_solid(int solid_x, int solid_y, int lift_check, int type)
{
   int yy = (solid_y - 2) / 20;
   int cc = solid_x / 20;
   int xx = solid_x / 20 + 1;
   int a = solid_x % 20;

   if (lift_check)
      for (int d = 0; d<num_lifts; d++)
         if ( ((type == 1) && (lifts[d].flags & PM_LIFT_SOLID_PLAYER)) ||
              ((type == 2) && (lifts[d].flags & PM_LIFT_SOLID_ENEMY )) ||
              ((type == 3) && (lifts[d].flags & PM_LIFT_SOLID_ITEM  )) )
         if (solid_x > lifts[d].x1 - 18)
            if (solid_x < lifts[d].x2 - 2)
               if (solid_y < lifts[d].y2 + 2)
                  if (solid_y > lifts[d].y2 - 10)
                    return d+32;

   if (a > 16)  // next block only
      if (is_solidu( -1, l[xx][yy], type)) return 1;

   if (a < 3)    // this block only
      if (is_solidu( -1, l[cc][yy], type)) return 1;

   if ((a > 2) && (a <17)) // dual compare with ||
      if (is_solidu( l[xx][yy], l[cc][yy], type)) return 1;

   return 0;
}

























/*

These 4 functions:

al_fixed is_up_solidfm(al_fixed fx, al_fixed fy, al_fixed fmove, int dir)
al_fixed is_down_solidfm(al_fixed fx, al_fixed fy, al_fixed fmove, int dir)
al_fixed is_left_solidfm(al_fixed fx, al_fixed fy, al_fixed fmove, int dir)
al_fixed is_right_solidfm(al_fixed fx, al_fixed fy, al_fixed fmove, int dir)

are used with trakbot only (for now).

They are more accurate than the other ones, they work to the exact pixel.
This was needed when trakbot clings to walls, ceilings and floors,
and need to navigate single block openings.

It is possible to move exactly adjacent to a solid, and not trigger a solid.

These checks have some new parameters:
'al_fixed fmove' - is the distance of the requested move
we want to know if we can move that far
the functions returns 'al_fixed allowed_move' - how far before the condition is met.

it is assumed that whatever calls the functions will add fmove to position (fx or fy) on return
by comparing requested move to allowed move you can tell if a collision occured:

mv = is_down_solidfm(Efi[EN][0], Efi[EN][1], Efi[EN][2], 1);
if (mv < Efi[EN][2]) // allowed move less than requested move

'int dir' - the direction to look and the type of thing we are checking for

if dir == 0, we are looking in the direction of the named function for the next solid

for example:
is_down_solidfm(fx, fy, fmove, 0)
looks down from fy, fmove amount for a solid
if none is found, the full fmove is returned
if solid is found, fmove is adjusted to make the move line up with solid

if (dir == 1 or -1) we are looking at a right angle from the named function
for the next empty

for example:
is_down_solidfm(fx, fy, fmove, 1)
looks right (+1 in x dir) from fx for the next empty below
if none is found, the full fmove is returned
if empty is found, fmove is adjusted to make the move line up to the next empty

is_down_solidfm(fx, fy, fmove, -1)
looks left (-1 in x dir) from fx for the next empty below
if none is found, the full fmove is returned
if empty is found, fmove is adjusted to make the move line up to the next empty

if you just want to know if the floor below you is solid:
is_down_solidfm(fx, fy, al_itofix(1), 0)

also they completely ignore lifts


*/

al_fixed is_up_solidfm(al_fixed fx, al_fixed fy, al_fixed fmove, int dir)
{
   int ix = al_fixtoi(fx);
   int iy = al_fixtoi(fy);
   int im = al_fixtoi(fmove);
   int move = 0;
   for (int t=0; t<=im; t++)
   {
      int j = 0, a, b=0, c=0, ret = 0, xx, yy, cc;
      if (dir == 0) yy = (iy-t-1) / 20;
      else
      {
         yy = (iy-1) / 20;
         j = dir * t;
      }
      cc = (ix+j) / 20;
      xx = (ix+j) / 20 + 1;
      a = (ix+j) % 20;

      if (a == 0)    // this block only
      {
         c = l[cc][yy];
         ret = is_solid(-1, c, 2);
      }
      else // dual compare with ||
      {
         b = l[xx][yy];
         c = l[cc][yy];
         ret = is_solid(b, c, 2);
      }
      if (dir == 0) // solid mode - look up for next solid
      {
         if (ret) // as soon as solid is found, return
         {
             int fp = iy - move;             // next solid pos
             al_fixed dist = fy - al_itofix(fp);   // distance from initial pos
             if (dist > fmove) dist = fmove; // limit to requested if over
             return dist;
         }
         else move++;
      }
      else // empty mode - look left or right for next empty
      {
         if (ret == 0) // as soon as an empty is found, return
         {
            if (dir == 1) // look right
            {
                int fp = ix + move;             // next empty pos
                al_fixed dist = al_itofix(fp) - fx;   // distance from initial pos
                if (dist > fmove) dist = fmove; // limit to requested if over
                return dist;
             }
            if (dir == -1) // look left
            {
                int fp = ix - move;             // next empty pos
                al_fixed dist = fx - al_itofix(fp);   // distance from initial pos
                if (dist > fmove) dist = fmove; // limit to requested if over
                return dist;
             }
         }
         else move++;
      }
   }
   // finished the loop with finding either case;
   return fmove; // max move allowed
}


al_fixed is_down_solidfm(al_fixed fx, al_fixed fy, al_fixed fmove, int dir)
{
   int ix = al_fixtoi(fx);
   int iy = al_fixtoi(fy);
   int im = al_fixtoi(fmove);
   int move = 0;
   for (int t=0; t<=im; t++)
   {
      int j = 0, a, b=0, c=0, ret = 0, xx, yy, cc;
      if (dir == 0) yy = (iy + t) / 20 + 1;
      else
      {
         yy = iy / 20 + 1;
         j = dir * t;
      }
      cc = (ix+j) / 20;
      xx = (ix+j) / 20 + 1;
      a = (ix+j) % 20;
      if (a == 0)    // this block only
      {
         c = l[cc][yy];
         ret = is_solid(-1, c, 2);
      }
      else // dual compare with ||
      {
         b = l[xx][yy];
         c = l[cc][yy];
         ret = is_solid(b, c, 2);
      }
      if (dir == 0) // solid mode - looks down for next solid
      {
         if (ret) // as soon as solid is found, return
         {
             int fp = iy + move;             // next solid pos
             al_fixed dist = al_itofix(fp) - fy;   // distance from initial pos
             if (dist > fmove) dist = fmove; // limit to requested if over
             return dist;
         }
         else move++;
      }
      else // empty mode - look left or right for next empty
      {
         if (ret == 0) // as soon as an empty is found, return
         {
            if (dir == 1) // look right
            {
                int fp = ix + move;             // next empty pos
                al_fixed dist = al_itofix(fp) - fx;   // distance from initial pos
                if (dist > fmove) dist = fmove; // limit to requested if over
                return dist;
             }
            if (dir == -1) // look left
            {
                int fp = ix - move;             // next empty pos
                al_fixed dist = fx - al_itofix(fp);   // distance from initial pos
                if (dist > fmove) dist = fmove; // limit to requested if over
                return dist;
             }
         }
         else move++;
      }
   }
   // finished the loop with finding either case;
   return fmove; // max move allowed
}

al_fixed is_left_solidfm(al_fixed fx, al_fixed fy, al_fixed fmove, int dir)
{
   int ix = al_fixtoi(fx);
   int iy = al_fixtoi(fy);
   int im = al_fixtoi(fmove);
   int move = 0;
   for (int t=0; t<=im; t++)
   {
      int j = 0, a, b=0, c=0, ret = 0, xx, yy, cc;
      if (dir == 0) xx = (ix-t-1) / 20;
      else
      {
         xx = (ix-1) / 20;
         j = dir * t;
      }
      yy = (iy+j) / 20;
      cc = (iy+j) / 20 + 1;
      a = (iy+j) % 20;
      if (a == 0)    // this block only
      {
         c = l[xx][yy];
         ret = is_solid(-1, c, 2);
      }
      else // dual compare with ||
      {
         b = l[xx][yy];
         c = l[xx][cc];
         ret = is_solid(b, c, 2);
      }

      if (dir == 0) // solid mode - looks left for next solid
      {
         if (ret) // as soon as solid is found, return
         {
             int fp = ix - move;             // next solid pos
             al_fixed dist = fx - al_itofix(fp);   // distance from initial pos
             if (dist > fmove) dist = fmove; // limit to requested if over
             return dist;
         }
         else move++;
      }
      else // empty mode - look up or down for next empty
      {
         if (ret == 0) // as soon as an empty is found, return
         {
            if (dir == 1) // look down
            {
                int fp = iy + move;             // next empty pos
                al_fixed dist = al_itofix(fp) - fy;   // distance from initial pos
                if (dist > fmove) dist = fmove; // limit to requested if over
                return dist;
             }
            if (dir == -1) // look up
            {
                int fp = iy - move;             // next empty pos
                al_fixed dist = fy - al_itofix(fp);   // distance from initial pos
                if (dist > fmove) dist = fmove; // limit to requested if over
                return dist;
             }
         }
         else move++;
      }
   }
   // finished the loop with finding either case;
   return fmove; // max move allowed
}


al_fixed is_right_solidfm(al_fixed fx, al_fixed fy, al_fixed fmove, int dir)
{
   int ix = al_fixtoi(fx);
   int iy = al_fixtoi(fy);
   int im = al_fixtoi(fmove);
   int move = 0;
   for (int t=0; t<=im; t++)
   {
      int j = 0, a, b=0, c=0, ret = 0, xx, yy, cc;
      if (dir == 0) xx = (ix + t) / 20 + 1;
      else
      {
         xx = (ix) / 20 + 1;
         j = dir * t;
      }
      yy = (iy+j) / 20;
      cc = (iy+j) / 20 + 1;

      a = (iy+j) % 20;

      if (a == 0)    // this block only
      {
         c = l[xx][yy];
         ret = is_solid(-1, c, 2);
      }
      else // dual compare with ||
      {
         b = l[xx][yy];
         c = l[xx][cc];
         ret = is_solid(b, c, 2);
      }
      if (dir == 0) // solid mode - looks right for next solid
      {
         if (ret) // as soon as solid is found, return
         {
             int fp = ix + move;             // next solid pos
             al_fixed dist = al_itofix(fp) - fx;   // distance from initial pos
             if (dist > fmove) dist = fmove; // limit to requested if over
             return dist;
         }
         else move++;
      }
      else // empty mode - look up or down for next empty
      {
         if (ret == 0) // as soon as an empty is found, return
         {
            if (dir == 1) // look down
            {
                int fp = iy + move;             // next empty pos
                al_fixed dist = al_itofix(fp) - fy;   // distance from initial pos
                if (dist > fmove) dist = fmove; // limit to requested if over
                return dist;
             }
            if (dir == -1) // look up
            {
                int fp = iy - move;             // next empty pos
                al_fixed dist = fy - al_itofix(fp);   // distance from initial pos
                if (dist > fmove) dist = fmove; // limit to requested if over
                return dist;
             }
         }
         else move++;
      }
   }
   // finished the loop with finding either case;
   return fmove; // max move allowed
}

void show_var_sizes(void)
{
   int level_header[20] = {0};

   printf("\nVariables used to save levels in pml format\n\n");

   printf("level_header:%6d\n",  (int)sizeof(level_header) );
   printf("l           :%6d\n",  (int)sizeof(l)            );
   printf("item        :%6d\n",  (int)sizeof(item)         );
   printf("Efi         :%6d\n",  (int)sizeof(Efi)          );
   printf("Ei          :%6d\n",  (int)sizeof(Ei)           );
   printf("lifts       :%6d\n",  (int)sizeof(lifts)        );
   printf("lift_steps  :%6d\n",  (int)sizeof(lift_steps)   );
   printf("pmsgtext    :%6d\n",  (int)sizeof(pmsgtext)     );

   int sz = 0;
   sz+= sizeof(level_header);
   sz+= sizeof(l)            ;
   sz+= sizeof(item)         ;
   sz+= sizeof(Efi)          ;
   sz+= sizeof(Ei)           ;
   sz+= sizeof(lifts)        ;
   sz+= sizeof(lift_steps)   ;
   sz+= sizeof(pmsgtext)     ;
   printf("------------:------\n");
   printf("total       :%6d\n",  sz );


   printf("\nVariables used for netgame state exchange\n\n");

   printf("players  :%6d\n", (int)sizeof(players)      );
   printf("Ei       :%6d\n", (int)sizeof(Ei)           );
   printf("Efi      :%6d\n", (int)sizeof(Efi)          );
   printf("item     :%6d\n", (int)sizeof(item)         );
   printf("itemf    :%6d\n", (int)sizeof(itemf)        );
   printf("lifts    :%6d\n", (int)sizeof(lifts)        );
   printf("l        :%6d\n", (int)sizeof(l)            );
   printf("mwS.p    :%6d\n", (int)sizeof(mwS.p)        );
   printf("mwS.e    :%6d\n", (int)sizeof(mwS.e)        );
   printf("pm_event :%6d\n", (int)sizeof(mwPME.event)  );

   sz = 0;
   sz+= sizeof(players)      ;
   sz+= sizeof(Ei)           ;
   sz+= sizeof(Efi)          ;
   sz+= sizeof(item)         ;
   sz+= sizeof(itemf)        ;
   sz+= sizeof(lifts)        ;
   sz+= sizeof(l)            ;
   sz+= sizeof(mwS.p)        ;
   sz+= sizeof(mwS.e)        ;
   sz+= sizeof(mwPME.event)  ;
   printf("---------:------\n");
   printf("total    :%6d\n",  sz );


   printf("\nOther Large Variables\n\n");

   sz = (int)sizeof(mwGMA.game_moves);
   printf("game_moves    :%6d  %6dK  %6dM \n", sz, sz/1000, sz/1000000 );

   sz = (int)sizeof(log_msg);
   printf("log_msg       :%6d  %6dK  %6dM \n", sz, sz/1000, sz/1000000 );

   sz = (int)sizeof(log_lines);
   printf("log_lines     :%6d  %6dK  %6dM \n", sz, sz/1000, sz/1000000 );

   sz = (int)sizeof(log_lines_int);
   printf("log_lines_int :%6d  %6dK  %6dM \n", sz, sz/1000, sz/1000000 );

//
//   sz = (int)sizeof(&level_background);
//   printf("level_background :%6d  %6dK  %6dM \n", sz, sz/1000, sz/1000000 );


//   extern ALLEGRO_BITMAP *tilemap;
//extern ALLEGRO_BITMAP *btilemap;
//extern ALLEGRO_BITMAP *ptilemap;
//extern ALLEGRO_BITMAP *dtilemap;
//extern ALLEGRO_BITMAP *M_tilemap;
//extern ALLEGRO_BITMAP *M_btilemap;
//extern ALLEGRO_BITMAP *M_ptilemap;
//extern ALLEGRO_BITMAP *M_dtilemap;
//
//extern ALLEGRO_BITMAP *mwB.tile[NUM_SPRITES];
//extern ALLEGRO_BITMAP *btile[NUM_SPRITES];
//
//extern int mwB.sa[NUM_SPRITES][2];
//
//extern ALLEGRO_BITMAP *player_tile[16][32];
//extern ALLEGRO_BITMAP *door_tile[2][16][8];
//
//extern ALLEGRO_BITMAP *level_background;
//extern ALLEGRO_BITMAP *level_buffer;


}



