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
   int mpty = 1; // default is empty

   if (test_block) if (l[x][y] > 31) mpty = 0;

   if (test_enemy)
      for (int c=0; c<100; c++)
         if (Ei[c][0])
            if ((Ef[c][0] == x*20) && (Ef[c][1] == y*20)) mpty = 0;

   if (test_item)
      for (int c=0; c<500; c++)
         if (item[c][0])
            if ((item[c][4] == (x*20)) && (item[c][5] == (y*20))) mpty = 0;

   return mpty;
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






































// used in pod when moving
// used in bouncer always
// used in cannon when player not in prox range
// used in level editor scale_bouncer_and_cannon_speed(int e)
void set_enemy_rot_from_incs(int e)
{
   Ef[e][14] = atan2(Ef[e][3], Ef[e][2]) - ALLEGRO_PI/2;
}

// used in cannon when player in prox range
// used in podzilla and vinepod
void set_enemy_rot_from_player(int e, int p)
{
   float xlen = players[p].x - Ef[e][0];
   float ylen = players[p].y - Ef[e][1];
   Ef[e][14] = atan2(ylen, xlen) - ALLEGRO_PI/2;
}


// used once in bouncer cannon common
void seek_set_xyinc(int e)
{
   int p = find_closest_player(e);
   float xlen = players[p].x - Ef[e][0];
   float ylen = players[p].y - Ef[e][1];
   float hy_dist = sqrt(pow(xlen, 2) + pow(ylen, 2));  // hypotenuse distance
   float speed = Ef[e][5];                              // speed
   float scaler = hy_dist / speed;     // get scaler
   Ef[e][2] = xlen / scaler;         // calc xinc
   Ef[e][3] = ylen / scaler;         // calc 7inc
}

// used by flapper y seek
// used by trakbot falling seek
// used by podzilla shoot
// used by podzilla to set rot when extended
// used by vinepod shoot
// used by vinepod to set rot when extended
// used by walker_archwagon_common in follow mode
// used by walker_archwagon_common to face left or right when can't move
// used by bouncer cannon common set_seek
int find_closest_player(int e)
{
   int closest_player = 0; // defaults to zero (will always return a valid player)
   float hd = 99999;
   for (int p=0; p<NUM_PLAYERS; p++)
      if ((players[p].active) && (!players[p].paused))
      {
         float xlen = players[p].x - Ef[e][0];
         float ylen = players[p].y - Ef[e][1];
         float h = sqrt(pow(xlen, 2) + pow(ylen, 2));  // hypotenuse distance
         if (h < hd)
         {
             hd = h;
             closest_player = p;
         }
      }
   return closest_player;
}


// used only by flapper
int find_closest_player_flapper(int e)
{
   int dir = Ei[e][2];

   float prox = Ei[e][17];
   float d[NUM_PLAYERS]; // array of distances for each player
   for (int p=0; p<NUM_PLAYERS; p++)
   {
      d[p] = -1;
      if ((players[p].active) && (!players[p].paused))
      {
         float xlen = players[p].x - Ef[e][0];            // get x distance
         float ylen = players[p].y - Ef[e][1];            // get y distance
         float dist = sqrt(pow(xlen, 2) + pow(ylen, 2));  // hypotenuse distance
         float angle = atan2(ylen, xlen) + ALLEGRO_PI/2;  // get raw angle and add 90 deg in radians
         float da = (angle / (ALLEGRO_PI*2)) * 360;       // convert from radians to degrees
         if (da < 0) da += 360;                           // add 360 if negative

         if ((dir == 1) && (da >  45) && (da < 135) && (dist < prox)) d[p] = dist; // right
         if ((dir == 0) && (da > 225) && (da < 315) && (dist < prox)) d[p] = dist; // left

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
*/

      }
   }

   float closest_val = 9999;
   int closest_p = -1;
   for (int p=0; p<NUM_PLAYERS; p++)
   {
      if ((d[p] != -1) && (d[p] < closest_val))
      {
         closest_val = d[p];
         closest_p = p;
      }
   }
   if (closest_val == 9999) return -1;    // no player in range
   else return closest_p;

}


//           normal cartesian
//              I    x+  Y+        II | I
//             II    x-  Y+       ----+----
//             III   x-  Y-       III | IV
//             IV    x+  Y-
//
//             screen
//              I    x+  Y-        II | I
//             II    x-  Y-       ----+----
//             III   x-  Y+       III | IV
//             IV    x+  Y+
// used by trakbot only




//int find_closest_player_quad(int e, int quad, int prox)
int find_closest_player_trakbot(int e)
{
   int quad = 0;
   float prox = Ei[e][17];
   switch (Ei[e][5])
   {
      case 0: case 5: quad = 1; break; // floor right, lwall up
      case 1: case 4: quad = 2; break; // rwall up floor left
      case 2: case 7: quad = 3; break; // ceil left, rwall down
      case 3: case 6: quad = 4; break; // lwall down, ceil right
   }

   int closest_p = -1;    // return -1 if no player in range
   float closest_val = 3000;
   float d[NUM_PLAYERS];
   for (int p=0; p<NUM_PLAYERS; p++)
   {
      d[p] = -1;
      if ((players[p].active) && (!players[p].paused))
      {
         float xlen = players[p].x - Ef[e][0];            // get x distance
         float ylen = players[p].y - Ef[e][1];            // get y distance
         float dist = sqrt(pow(xlen, 2) + pow(ylen, 2));  // hypotenuse distance
         float angle = atan2(ylen, xlen) + ALLEGRO_PI/2;  // get raw angle and add 90 deg in radians
         float da = (angle / (ALLEGRO_PI*2)) * 360;       // convert from radians to degrees
         if (da < 0) da += 360;                           // add 360 if negative

         // printf("angle:%f   da:%f\n", angle, da);

         if ((quad == 1) && (da>  0) && (da< 90)) d[p] = dist;
         if ((quad == 4) && (da> 90) && (da<180)) d[p] = dist;
         if ((quad == 3) && (da>180) && (da<270)) d[p] = dist;
         if ((quad == 2) && (da>270) && (da<359)) d[p] = dist;
      }
      if (d[p] > prox) d[p] = -1; // add, if distance is within range
   }
   for (int p=0; p<NUM_PLAYERS; p++)
      if ((d[p] != -1) && (d[p] < closest_val))
      {
         closest_val = d[p];
         closest_p = p;
      }
   if (closest_val == 3000) return -1; // no player in range
   else return closest_p;
}


// finds closest player...if closest player is not within dist, returns -1
// used only by cannon
int find_closest_player_cannon(int e)
{
   float prox = Ei[e][17];
   int closest_player = -1; // default if no player within distance
   float hd = 99999;
   for (int p=0; p<NUM_PLAYERS; p++)
      if ((players[p].active) && (!players[p].paused))
      {
         float xlen = players[p].x - Ef[e][0];
         float ylen = players[p].y - Ef[e][1];
         float h = sqrt(pow(xlen, 2) + pow(ylen, 2));  // hypotenuse distance
         if (h < hd)
         {
             hd = h;
             closest_player = p;
         }
      }
   if (hd < prox) return closest_player;
   else return -1;
}


float deg_to_rad(float deg)
{
   return (deg/360) * ALLEGRO_PI*2;
}














void show_var_sizes(void)
{
   printf("\nVariables used to save levels in pml format\n\n");
   printf("l           :%6d\n",  (int)sizeof(l)            );
   printf("item        :%6d\n",  (int)sizeof(item)         );
   printf("Ef          :%6d\n",  (int)sizeof(Ef)           );
   printf("Ei          :%6d\n",  (int)sizeof(Ei)           );
   printf("lifts       :%6d\n",  (int)sizeof(lifts)        );
   printf("lift_steps  :%6d\n",  (int)sizeof(lift_steps)   );
   printf("pmsgtext    :%6d\n",  (int)sizeof(pmsgtext)     );

   int sz = 0;
   sz+= sizeof(l)            ;
   sz+= sizeof(item)         ;
   sz+= sizeof(Ef)           ;
   sz+= sizeof(Ei)           ;
   sz+= sizeof(lifts)        ;
   sz+= sizeof(lift_steps)   ;
   sz+= sizeof(pmsgtext)     ;
   printf("------------:------\n");
   printf("total       :%6d\n",  sz );

   printf("\nVariables used for netgame state exchange\n\n");

   printf("players  :%6d\n", (int)sizeof(players)      );
   printf("Ei       :%6d\n", (int)sizeof(Ei)           );
   printf("Ef       :%6d\n", (int)sizeof(Ef)           );
   printf("item     :%6d\n", (int)sizeof(item)         );
   printf("itemf   :%6d\n", (int)sizeof(itemf)       );
   printf("lifts    :%6d\n", (int)sizeof(lifts)        );
   printf("l        :%6d\n", (int)sizeof(l)            );
   printf("mwS.p    :%6d\n", (int)sizeof(mwS.p)        );
   printf("mwS.e    :%6d\n", (int)sizeof(mwS.e)        );
   printf("pm_event :%6d\n", (int)sizeof(mwPME.event)  );

   sz = 0;
   sz+= sizeof(players)      ;
   sz+= sizeof(Ei)           ;
   sz+= sizeof(Ef)           ;
   sz+= sizeof(item)         ;
   sz+= sizeof(itemf)       ;
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
}

