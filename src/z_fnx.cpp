// zfnx.cpp
#include "pm.h"



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

   int tm = frame_num % cti; // get a number from 0 to cti than increments every frame

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

   if (flags == 0) al_draw_scaled_bitmap(       tile[tn],     tsx, tsy, tsw, tsh, x+xo, y+yo, xs, ys, flags);
   else            al_draw_tinted_scaled_bitmap(tile[tn], c2, tsx, tsy, tsw, tsh, x+xo, y+yo, xs, ys, flags);

}

















void clear_game_moves(void)
{
   for (int x=0; x<GAME_MOVES_SIZE; x++)
      for (int y=0; y<4; y++)
         game_moves[x][y] = 0;
   game_move_entry_pos = 0;
   game_move_current_pos = 0;
}



void get_hostname(void)
{
   sprintf(msg, "hostname");
   FILE *fp;
   fp = popen(msg,"r");

   int loop = 0;
   int ch = fgetc(fp);
   while((ch != '\n') && (ch != EOF))
   {
      local_hostname[loop] = ch;
      loop++;
      ch = fgetc(fp);
   }
   local_hostname[loop] = 0;
   fclose(fp);
}


void make_palette(void)
{
   // printf("make palette\n");

   palette_color[0]  = al_map_rgb(  0,   0,   0); // black
   palette_color[1]  = al_map_rgb(191, 108, 232); // alt purple 1
   palette_color[2]  = al_map_rgb(136,  32, 172); // alt purple 2
   palette_color[3]  = al_map_rgb( 60, 127, 255); // alt blue
   palette_color[4]  = al_map_rgb(224,  28,  72); // alt red
   palette_color[5]  = al_map_rgb(255,   0, 232); // pink
   palette_color[6]  = al_map_rgb(255, 191, 127); // taan
   palette_color[7]  = al_map_rgb(255, 127,   0); // orange
   palette_color[8]  = al_map_rgb(127,   0, 255); // original purple
   palette_color[9]  = al_map_rgb(  0, 255, 127); // alt green
   palette_color[10] = al_map_rgb(255,   0,   0); // red
   palette_color[11] = al_map_rgb(  0, 255,   0); // green
   palette_color[12] = al_map_rgb(  0,   0, 255); // blue
   palette_color[13] = al_map_rgb(  0, 255, 255); // lt blue
   palette_color[14] = al_map_rgb(255, 255,   0); // yellow
   palette_color[15] = al_map_rgb(255, 255, 255); // white

   // special case reversed white (0)
   for (int x=1; x<16; x++)
   {
      int c = (x+1)*16 - 1;
      palette_color[x*16]  = al_map_rgb(c, c, c);
   }

   // all the other base colors from 1-15
   for (int a=1; a<16; a++) // iterate base colors
   {
      // extract r, g, b in float format
      float r, g, b;
      al_unmap_rgb_f(palette_color[a], &r, &g, &b);

      for (int x=1; x<16; x++)
      {
         float nr = r * (1 - ((float)x/15));
         float ng = g * (1 - ((float)x/15));
         float nb = b * (1 - ((float)x/15));
         palette_color[a+x*16]  = al_map_rgb_f(nr, ng, nb);
      }
   }

//   for (int x=0; x<16; x++)
//   {
//      unsigned char ur, ug, ub;
//      al_unmap_rgb(palette_color[a+x*16], &ur, &ug, &ub);
//      printf("%2d %3d r:%3d g:%3d b:%3d \n", x, a+x*16, ur, ug, ub );
//   }
//   for (int x=0; x<16; x++)
//   al_draw_line(10, 10+x*2, 200, 10+x*2, palette_color[10+x*16], 2);
//   int sz = 16;
//   al_draw_rectangle(0, 0, 17*sz, 17*sz, palette_color[15], 2);
//
//   for (int a=0; a<16; a++)
//      for (int b=0; b<16; b++)
//         al_draw_filled_rectangle(a*sz, b*sz, a*sz+sz, b*sz+sz, palette_color[b*16+a]);
//   al_draw_text(font, palette_color[9], 400, 400, ALLEGRO_ALIGN_CENTRE, "Hello World");

}



void m_err(const char * err_msg)
{
   fprintf(stderr, "%s\n", err_msg);
   al_show_native_message_box(display, "Error", "Error:", err_msg, NULL,  ALLEGRO_MESSAGEBOX_ERROR );
}

void window_title(void)
{
   sprintf(msg, "Purple Martians");
//   sprintf(msg, "Purple Martians %s   [%d x %d]", pm_version_string, SCREEN_W, SCREEN_H);
//   sprintf(msg, "Purple Martians %s   S[%d x %d]  A[%d x %d]   [%d]", pm_version_string, SCREEN_W, SCREEN_H,  disp_w_curr, disp_h_curr, display_transform_double);
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
   al_flush_event_queue(event_queue);
   int quit = 0;
   while(!quit)
   {
      while (!al_is_event_queue_empty(event_queue))
      {
         ALLEGRO_EVENT ev;
         if (al_get_next_event(event_queue, &ev))
         {
             if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) fast_exit(0);
             if (ev.type == ALLEGRO_EVENT_KEY_DOWN) quit = 1;
         }
      }
   }
   while (key[ALLEGRO_KEY_ESCAPE]) proc_controllers();
}

void set_frame_nums(int fn)
{
   // reset frame_num and fps_timer count
	frame_num = fn;
   al_set_timer_count(fps_timer, fn);
   reset_animation_sequence_frame_nums(fn);
}

void reset_animation_sequence_frame_nums(int fn)
{
   for (int c=0; c<NUM_ANS; c++)
   {
     zz[2][c] = fn;       // reset the frame_nums
     zz[1][c] = 0;        // set the bitmap index to 0
     zz[0][c] = zz[5][c]; // put the first shape in 0
   }
}

void update_animation(void)
{
   // 0 = current shape
   // 1 = current shape index
   // 2 = frame_num of last seq change
   // 3 = seq change delay count
   // 4 = num of shapes in seq (15 shapes max)
   // 5 = shape 0
   // 19 = shape 14

   for (int y=0; y<NUM_ANS; y++)
      if (zz[4][y] != 0)
         if ((frame_num - zz[2][y]) > zz[3][y])
         {
            zz[2][y] = frame_num;                     // set counter
            zz[1][y]++;                               // next bitmap
            if (zz[1][y] > zz[4][y]) zz[1][y] = 0;    // is bitmap past end?
            zz[0][y] = zz[ 5 + zz[1][y] ] [y];        // put shape in 0
         }
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
   al_fixed speed = Efi[EN][5];                  // speed
   al_fixed scaler = al_fixdiv(hy_dist, speed);     // get scaler
   al_fixed xinc = al_fixdiv(xlen, scaler);         // calc xinc
   al_fixed yinc = al_fixdiv(ylen, scaler);         // calc yinc
   Efi[EN][2] = xinc;
   Efi[EN][3] = yinc;
}





int find_closest_player_flapper(int EN, int dir)
{
   al_fixed width = al_itofix(Ei[EN][17]); // al_fixed version of width
   int height  = Ei[EN][18];
   int depth   = Ei[EN][19];

   // get the enemy's trigger box y values
   int ey = al_fixtoi(Efi[EN][1]);
   int ey1 = ey - height;
   int ey2 = ey + depth;

   al_fixed d[NUM_PLAYERS]; // array of distances for each player
   for (int p=0; p<NUM_PLAYERS; p++)
   {
      d[p] = al_itofix(-1); // default result (player not in range or not active)
      if ((players[p].active) && (!players[p].paused))
      {
         // is player in the y range?
         int py = al_fixtoi(players[p].PY)+10;
         if ((py > ey1) && (py < ey2))
         {
            al_fixed xlen = players[p].PX - Efi[EN][0]; // get x distance
            if ((!dir) && (xlen < al_itofix(0))) d[p] = -xlen; // left
            if ( (dir) && (xlen > al_itofix(0))) d[p] =  xlen; // right
         }
      }
      // check if distance is within range (width); invalidate if not
      if (d[p] > width) d[p] = al_itofix(-1);
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
void fire_enemy_bulletz(int EN, int bullet_ans, al_fixed px, al_fixed py)
{
   al_fixed xlen = px - Efi[EN][0];   // get the x distance between enemy and player
   al_fixed ylen = py - Efi[EN][1];   // get the y distance between enemy and player
   al_fixed hy_dist =  al_fixhypot(xlen, ylen);     // hypotenuse distance
   al_fixed speed = Efi[EN][7];                  // speed
   al_fixed scaler = al_fixdiv(hy_dist, speed);     // get scaler
   al_fixed xinc = al_fixdiv(xlen, scaler);        // calc xinc
   al_fixed yinc = al_fixdiv(ylen, scaler);        // calc yinc

   for (int z=0; z<50; z++)  // find empty e_bullet
      if (!e_bullet_active[z])
      {
         e_bullet_active[z] = 1;
         e_bullet_shape[z] = 1000 + bullet_ans;
         e_bullet_fx[z] = Efi[EN][0];
         e_bullet_fy[z] = Efi[EN][1];
         e_bullet_fxinc[z] = xinc;
         e_bullet_fyinc[z] = yinc;
         z=50;
      }
}
void fire_enemy_bulleta(int EN, int bullet_ans, int p)
{
   float bx = Efi[EN][0];
   float by = Efi[EN][1];
   float bv = Efi[EN][7];

   float px  = players[p].PX;
   float py  = players[p].PY;
   float pvx = players[p].xinc;
   float pvy = players[p].yinc;

   // Edgar's method
   //float A = pow(pvx,2) + pow(pvy,2) - pow(bv,2);
   //float B = 2*(px*pvx) + 2*(py*pvy) -2*(bx*pvx) -2*(by*pvy);
   //float C = pow(px,2) + pow(bx,2) + pow(py,2) + pow(by,2) - 2*(bx*px) -2*(by*py);

   // Peter's method
   float x = px-bx;
   float y = py-by;
   float A = pow(pvx,2) + pow(pvy,2) - pow(bv,2);
   float B = 2*(x*pvx) + 2*(y*pvy);
   float C = pow(x,2) + pow(y,2);

   float t = ( -B - sqrt(pow(B,2) - 4*(A*C)) ) / (2*A);
   al_fixed px1 = px + al_fixmul(pvx, al_ftofix(t)); // get player target position based on t
   al_fixed py1 = py + al_fixmul(pvy, al_ftofix(t));
   fire_enemy_bulletz(EN, bullet_ans, px1, py1);
}

void fire_enemy_x_bullet(int EN, int p)
{
   al_fixed x_bullet_speed = Efi[EN][7];
   for (int z=0; z<50; z++)  // find empty e_bullet
      if (!e_bullet_active[z])
      {
         e_bullet_active[z] = 1;
         e_bullet_fyinc[z] = al_itofix(0);
         e_bullet_fx[z] = Efi[EN][0];
         e_bullet_fy[z] = Efi[EN][1];
         if (Efi[EN][0] < players[p].PX)
         {
            e_bullet_fxinc[z] = x_bullet_speed;
            e_bullet_shape[z] = 488;
         }
         if (Efi[EN][0] >= players[p].PX)
         {
            e_bullet_fxinc[z] = -x_bullet_speed;
            e_bullet_shape[z] = 489;
         }
         z=50; // end loop
      }
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


int is_solidu(int b, int c, int type)
{
   if (b == -1) // do c only
   {
      if ((type == 1) && (c & PM_BTILE_SOLID_PLAYER) && (!(c & PM_BTILE_SEMISOLID_PLAYER))) return 1;


      if ((type == 2) && (c & PM_BTILE_SOLID_ENEMY))  return 1;
      if ((type == 3) && (c & PM_BTILE_SOLID_ITEM))   return 1;
      if ((type == 4) && (c & PM_BTILE_SOLID_PBUL))   return 1;
      if ((type == 5) && (c & PM_BTILE_SOLID_EBUL))   return 1;
   }
   else
   {
      if ( (type == 1) &&
           ( ((b & PM_BTILE_SOLID_PLAYER) && (!(b & PM_BTILE_SEMISOLID_PLAYER))) ||
             ((c & PM_BTILE_SOLID_PLAYER) && (!(c & PM_BTILE_SEMISOLID_PLAYER))) )

             ) return 1;

      if ((type == 2) && ((b & PM_BTILE_SOLID_ENEMY)  || (c & PM_BTILE_SOLID_ENEMY)))   return 1;
      if ((type == 3) && ((b & PM_BTILE_SOLID_ITEM)   || (c & PM_BTILE_SOLID_ITEM)))    return 1;
      if ((type == 4) && ((b & PM_BTILE_SOLID_PBUL)   || (c & PM_BTILE_SOLID_PBUL)))    return 1;
      if ((type == 5) && ((b & PM_BTILE_SOLID_EBUL)   || (c & PM_BTILE_SOLID_EBUL)))    return 1;
   }
   return 0;
}













// returns 1 for solid block, 2 for semi-solid, or 32+lift_num for lift
int is_right_solid(int solid_x, int solid_y, int lift_check, int type)
{
   int xx = solid_x / 20 + 1;
   int yy = solid_y / 20;
   int cc = solid_y / 20 + 1;
   int a = solid_y % 20;

   if (lift_check)
      for (int d = 0; d<num_lifts; d++)

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




// returns 1 for solid block, 2 for semi-solid, or 32+lift_num for lift
int is_left_solid(int solid_x, int solid_y, int lift_check, int type)
{
   int xx = solid_x / 20;
   int yy = solid_y / 20;
   int cc = solid_y / 20 + 1;
   int a = solid_y % 20;
   if (lift_check)
      for (int d = 0; d<num_lifts; d++)
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

// old returns 1 for solid block, 2 for semi-solid, or 32+lift_num for lift
// new returns 1 for solid block, or 32+lift_num for lift
int is_down_solid(int solid_x, int solid_y, int lift_check, int type)
{
   int yy = solid_y / 20 + 1;
   int cc = solid_x / 20;
   int xx = solid_x / 20 + 1;
   int a = solid_x % 20;
   if (lift_check)
      for (int d = 0; d<num_lifts; d++)
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




// returns 1 for solid block, 2 for semi-solid, or 32+lift_num for lift
int is_up_solid(int solid_x, int solid_y, int lift_check, int type)
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

it is assumed that what ever calls the functions will add fmove to position (fx or fy) on return
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
         if      ((c > 63) && (c < NUM_SPRITES)) ret = 1;
         else if ((c > 31) && (c < NUM_SPRITES)) ret = 2;
      }
      else // dual compare with ||
      {
         b = l[xx][yy];
         c = l[cc][yy];
         if ( ((b > 63) && (b < NUM_SPRITES)) || ((c > 63) && (c < NUM_SPRITES)) ) ret = 1;
         else if ( ((b > 31) && (b < NUM_SPRITES)) || ((c > 31) && (c < NUM_SPRITES)) ) ret = 2;
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
         if      ((c > 63) && (c < NUM_SPRITES)) ret = 1;
         else if ((c > 31) && (c < NUM_SPRITES)) ret = 2;
      }
      else // dual compare with ||
      {
         b = l[xx][yy];
         c = l[cc][yy];
         if ( ((b > 63) && (b < NUM_SPRITES)) || ((c > 63) && (c < NUM_SPRITES)) ) ret = 1;
         else if ( ((b > 31) && (b < NUM_SPRITES)) || ((c > 31) && (c < NUM_SPRITES)) ) ret = 2;
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
         if      ((c > 63) && (c < NUM_SPRITES)) ret = 1;
         else if ((c > 31) && (c < NUM_SPRITES)) ret = 2;
      }
      else // dual compare with ||
      {
         b = l[xx][yy];
         c = l[xx][cc];
         if ( ((b > 63) && (b < NUM_SPRITES)) || ((c > 63) && (c < NUM_SPRITES)) ) ret = 1;
         else if ( ((b > 31) && (b < NUM_SPRITES)) || ((c > 31) && (c < NUM_SPRITES)) ) ret = 2;
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
         if      ((c > 63) && (c < NUM_SPRITES)) ret = 1;
         else if ((c > 31) && (c < NUM_SPRITES)) ret = 2;
      }
      else // dual compare with ||
      {
         b = l[xx][yy];
         c = l[xx][cc];
         if ( ((b > 63) && (b < NUM_SPRITES)) || ((c > 63) && (c < NUM_SPRITES)) ) ret = 1;
         else if ( ((b > 31) && (b < NUM_SPRITES)) || ((c > 31) && (c < NUM_SPRITES)) ) ret = 2;
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

void game_vars_to_state(char * b)
{
   int size = 0, offset = 0;
   offset += size; size = sizeof(players); memcpy(b+offset, players, size);
   offset += size; size = sizeof(Ei);      memcpy(b+offset, Ei,      size);
   offset += size; size = sizeof(Efi);     memcpy(b+offset, Efi,     size);
   offset += size; size = sizeof(item);    memcpy(b+offset, item,    size);
   offset += size; size = sizeof(itemf);   memcpy(b+offset, itemf,   size);
   offset += size; size = sizeof(lifts);   memcpy(b+offset, lifts,   size);
   offset += size; size = sizeof(l);       memcpy(b+offset, l,       size);
}

void state_to_game_vars(char * b)
{
   int size = 0, offset = 0;
   size = sizeof(players); memcpy(players, b+offset, size); offset += size;
   size = sizeof(Ei);      memcpy(Ei,      b+offset, size); offset += size;
   size = sizeof(Efi);     memcpy(Efi,     b+offset, size); offset += size;
   size = sizeof(item);    memcpy(item,    b+offset, size); offset += size;
   size = sizeof(itemf);   memcpy(itemf,   b+offset, size); offset += size;
   size = sizeof(lifts);   memcpy(lifts,   b+offset, size); offset += size;
   size = sizeof(l);       memcpy(l,       b+offset, size); offset += size;
}

void get_state_dif(char *a, char *b, char *c, int size)
{
   for (int i=0; i<size; i++)
      c[i] = a[i] - b[i];
}

void apply_state_dif(char *a, char *c, int size)
{
   for (int i=0; i<size; i++)
      a[i] -= c[i];
}

void reset_states(void)
{
   // reset base state on client
   memset(client_state_base, 0, STATE_SIZE);
   client_state_base_frame_num = 0;  // frame_num id

   // reset dif buffer and frame_num
   memset(client_state_dif, 0, STATE_SIZE);
   client_state_dif_src = -1; // -1 will never match a frame_num
   client_state_dif_dst = -1;

   // reset buffer and frame_nums used to build compressed state from packets
   memset(client_state_buffer, 0, STATE_SIZE);
   for (int i=0; i<16; i++)
      client_state_buffer_pieces[i] = -1;

   // reset server's client states
   for (int i=0; i<8; i++)
   {
      memset(srv_client_state[i][0], 0, STATE_SIZE);
      memset(srv_client_state[i][1], 0, STATE_SIZE);
      srv_client_state_frame_num[i][0] = 0;  // src
      srv_client_state_frame_num[i][1] = -3; // dst
   }
}

void show_state_dif(char *a, char *b)
{
   // copy chunks to temp vars

   struct player c1_players[NUM_PLAYERS];
   struct player c2_players[NUM_PLAYERS];

   int c1_Ei[100][32];    // enemy ints
   int c2_Ei[100][32];    // enemy ints

   al_fixed c1_Efi[100][16]; // enemy fixeds
   al_fixed c2_Efi[100][16]; // enemy fixeds

   int c1_item[500][16];  // item ints
   int c2_item[500][16];  // item ints

   al_fixed c1_itemf[500][4]; // item al_fixed points
   al_fixed c2_itemf[500][4]; // item al_fixed points

   struct lift c1_lifts[NUM_LIFTS];
   struct lift c2_lifts[NUM_LIFTS];

   int c1_l[100][100];    // level
   int c2_l[100][100];    // level

   int sz = 0, of = 0;
   sz=sizeof(players); memcpy(c1_players, a+of, sz); memcpy(c2_players, b+of, sz); of+=sz;
   sz=sizeof(Ei);      memcpy(c1_Ei,      a+of, sz); memcpy(c2_Ei,      b+of, sz); of+=sz;
   sz=sizeof(Efi);     memcpy(c1_Efi,     a+of, sz); memcpy(c2_Efi,     b+of, sz); of+=sz;
   sz=sizeof(item);    memcpy(c1_item,    a+of, sz); memcpy(c2_item,    b+of, sz); of+=sz;
   sz=sizeof(itemf);   memcpy(c1_itemf,   a+of, sz); memcpy(c2_itemf,   b+of, sz); of+=sz;
   sz=sizeof(lifts);   memcpy(c1_lifts,   a+of, sz); memcpy(c2_lifts,   b+of, sz); of+=sz;
   sz=sizeof(l);       memcpy(c1_l,       a+of, sz); memcpy(c2_l,       b+of, sz); of+=sz;


   int sop = sizeof(players);

   if (memcmp(c1_players, c2_players, sop))
   {
      if (L_LOGGING_NETPLAY_show_dif1)
      {
         sprintf(msg, "player errors detected\n");
         add_log_entry2(31, active_local_player, msg); printf("%s", msg);
      }
      if (L_LOGGING_NETPLAY_show_dif2)
      {

         // show where
         for (int i=0; i<sop; i++)
         {
            if (a[i] != b[i])
            {
               int ps = sop /8;  // size of one player
               int p = i/ps ;    // get player
               int o = i % ps;   // get offset
   //            printf("player[%d] error detected at offset%d byte_pos:%d\n", p, o, o/4);
   //            sprintf(msg, "[%d][%d] \n", a[i], b[i]);

               sprintf(msg, "player[%d] byte:[%d] var:[%d] server:[%d] local:[%d] \n", p, o, o/4, a[i], b[i] );
               add_log_entry2(32, active_local_player, msg);
            }
         }


         for (int p=0; p<8; p++)
         {
            if (c1_players[p].active != c2_players[p].active)
            {
               sprintf(msg, "player[%d].active  server:[%d] local:[%d]\n", p, c1_players[p].active, c2_players[p].active );
               add_log_entry2(32, active_local_player, msg);
            }
            if (c1_players[p].paused != c2_players[p].paused)
            {
               sprintf(msg, "player[%d].paused  server:[%d] local:[%d]\n", p, c1_players[p].paused, c2_players[p].paused );
               add_log_entry2(32, active_local_player, msg);
            }

            if (c1_players[p].paused_type  != c2_players[p].paused_type)
            {
               sprintf(msg, "player[%d].paused_type   server:[%d] local:[%d]\n", p, c1_players[p].paused_type,  c2_players[p].paused_type);
               add_log_entry2(32, active_local_player, msg);
            }

            if (c1_players[p].paused_mode  != c2_players[p].paused_mode)
            {
               sprintf(msg, "player[%d].paused_mode   server:[%d] local:[%d]\n", p, c1_players[p].paused_mode,  c2_players[p].paused_mode);
               add_log_entry2(32, active_local_player, msg);
            }

            if (c1_players[p].paused_mode_count  != c2_players[p].paused_mode_count)
            {
               sprintf(msg, "player[%d].paused_mode_count   server:[%d] local:[%d]\n", p, c1_players[p].paused_mode_count,  c2_players[p].paused_mode_count);
               add_log_entry2(32, active_local_player, msg);
            }

           if (c1_players[p].PX != c2_players[p].PX)
            {
               sprintf(msg, "player[%d].PX   server:[%f] local:[%f]\n", p, al_fixtof(c1_players[p].PX), al_fixtof(c2_players[p].PX));
               add_log_entry2(32, active_local_player, msg);
            }

            if (c1_players[p].PY  != c2_players[p].PY)
            {
               sprintf(msg, "player[%d].PY   server:[%f] local:[%f]\n", p, al_fixtof(c1_players[p].PY), al_fixtof(c2_players[p].PY)  );
               add_log_entry2(32, active_local_player, msg);
            }

            if (c1_players[p].xinc != c2_players[p].xinc)
            {
               sprintf(msg, "player[%d].xinc server:[%f] local:[%f]\n", p, al_fixtof(c1_players[p].xinc), al_fixtof(c2_players[p].xinc)  );
               add_log_entry2(32, active_local_player, msg);
            }

            if (c1_players[p].yinc != c2_players[p].yinc)
            {
               sprintf(msg, "player[%d].yinc server:[%f] local:[%f]\n", p, al_fixtof(c1_players[p].yinc), al_fixtof(c2_players[p].yinc)  );
               add_log_entry2(32, active_local_player, msg);
            }

            if (c1_players[p].right_xinc != c2_players[p].right_xinc)
            {
               sprintf(msg, "player[%d].right_xinc server:[%f] local:[%f]\n", p, al_fixtof(c1_players[p].right_xinc), al_fixtof(c2_players[p].right_xinc)  );
               add_log_entry2(32, active_local_player, msg);
            }

            if (c1_players[p].left_xinc != c2_players[p].left_xinc)
            {
               sprintf(msg, "player[%d].left_xinc server:[%f] local:[%f]\n", p, al_fixtof(c1_players[p].left_xinc), al_fixtof(c2_players[p].left_xinc)  );
               add_log_entry2(32, active_local_player, msg);
            }

            if (c1_players[p].LIFE != c2_players[p].LIFE)
            {
               sprintf(msg, "player[%d].LIFE   server:[%f] local:[%f]\n", p, al_fixtof(c1_players[p].LIFE), al_fixtof(c2_players[p].LIFE));
               add_log_entry2(32, active_local_player, msg);
            }
            if (c1_players[p].old_LIFE != c2_players[p].old_LIFE)
            {
               sprintf(msg, "player[%d].old_LIFE   server:[%f] local:[%f]\n", p, al_fixtof(c1_players[p].old_LIFE), al_fixtof(c2_players[p].old_LIFE));
               add_log_entry2(32, active_local_player, msg);
            }
            if (c1_players[p].LIVES  != c2_players[p].LIVES)
            {
               sprintf(msg, "player[%d].LIVES   server:[%d] local:[%d]\n", p, c1_players[p].LIVES,  c2_players[p].LIVES);
               add_log_entry2(32, active_local_player, msg);
            }


//            if (c1_players[p].not_used_bitmap_index  != c2_players[p].not_used_bitmap_index)
//            {
//               sprintf(msg, "player[%d].bitmap_index   server:[%d] local:[%d]\n", p, c1_players[p].bitmap_index,  c2_players[p].bitmap_index);
//               add_log_entry2(32, active_local_player, msg);
//            }


            if (c1_players[p].shape  != c2_players[p].shape)
            {
               sprintf(msg, "player[%d].shape   server:[%d] local:[%d]\n", p, c1_players[p].shape,  c2_players[p].shape);
               add_log_entry2(32, active_local_player, msg);
            }



            if (c1_players[p].color  != c2_players[p].color)
            {
               sprintf(msg, "player[%d].color   server:[%d] local:[%d]\n", p, c1_players[p].color,  c2_players[p].color );
               add_log_entry2(32, active_local_player, msg);
            }




           if (c1_players[p].door_draw_rot_num_steps  != c2_players[p].door_draw_rot_num_steps)
            {
               sprintf(msg, "player[%d].door_draw_rot_num_steps   server:[%d] local:[%d]\n", p, c1_players[p].door_draw_rot_num_steps,  c2_players[p].door_draw_rot_num_steps);
               add_log_entry2(32, active_local_player, msg);
            }

            if (c1_players[p].door_draw_rot != c2_players[p].door_draw_rot)
            {
               sprintf(msg, "player[%d].door_draw_rot   server:[%f] local:[%f]\n", p, al_fixtof(c1_players[p].door_draw_rot), al_fixtof(c2_players[p].door_draw_rot));
               add_log_entry2(32, active_local_player, msg);
            }
            if (c1_players[p].door_draw_rot_inc != c2_players[p].door_draw_rot_inc)
            {
               sprintf(msg, "player[%d].door_draw_rot_inc   server:[%f] local:[%f]\n", p, al_fixtof(c1_players[p].door_draw_rot_inc), al_fixtof(c2_players[p].door_draw_rot_inc));
               add_log_entry2(32, active_local_player, msg);
            }
            if (c1_players[p].draw_rot != c2_players[p].draw_rot)
            {
               sprintf(msg, "player[%d].draw_rot   server:[%f] local:[%f]\n", p, al_fixtof(c1_players[p].draw_rot), al_fixtof(c2_players[p].draw_rot));
               add_log_entry2(32, active_local_player, msg);
            }
            if (c1_players[p].draw_scale != c2_players[p].draw_scale)
            {
               sprintf(msg, "player[%d].draw_scale   server:[%f] local:[%f]\n", p, al_fixtof(c1_players[p].draw_scale), al_fixtof(c2_players[p].draw_scale));
               add_log_entry2(32, active_local_player, msg);
            }



            if (c1_players[p].marked_door  != c2_players[p].marked_door)
            {
               sprintf(msg, "player[%d].marked_door   server:[%d] local:[%d]\n", p, c1_players[p].marked_door,  c2_players[p].marked_door);
               add_log_entry2(32, active_local_player, msg);
            }
            if (c1_players[p].door_item  != c2_players[p].door_item)
            {
               sprintf(msg, "player[%d].door_item   server:[%d] local:[%d]\n", p, c1_players[p].door_item,  c2_players[p].door_item);
               add_log_entry2(32, active_local_player, msg);
            }
            if (c1_players[p].door_xinc  != c2_players[p].door_xinc)
            {
               sprintf(msg, "player[%d].door_xinc   server:[%d] local:[%d]\n", p, c1_players[p].door_xinc,  c2_players[p].door_xinc);
               add_log_entry2(32, active_local_player, msg);
            }
            if (c1_players[p].door_yinc  != c2_players[p].door_yinc)
            {
               sprintf(msg, "player[%d].door_yinc   server:[%d] local:[%d]\n", p, c1_players[p].door_yinc,  c2_players[p].door_yinc);
               add_log_entry2(32, active_local_player, msg);
            }
            if (c1_players[p].door_num_steps  != c2_players[p].door_num_steps)
            {
               sprintf(msg, "player[%d].door_num_steps   server:[%d] local:[%d]\n", p, c1_players[p].door_num_steps,  c2_players[p].door_num_steps);
               add_log_entry2(32, active_local_player, msg);
            }

            if (c1_players[p].left_right  != c2_players[p].left_right)
            {
               sprintf(msg, "player[%d].left_right   server:[%d] local:[%d]\n", p, c1_players[p].left_right,  c2_players[p].left_right);
               add_log_entry2(32, active_local_player, msg);
            }

            if (c1_players[p].player_ride  != c2_players[p].player_ride)
            {
               sprintf(msg, "player[%d].player_ride   server:[%d] local:[%d]\n", p, c1_players[p].player_ride,  c2_players[p].player_ride);
               add_log_entry2(32, active_local_player, msg);
            }

            if (c1_players[p].carry_item  != c2_players[p].carry_item)
            {
               sprintf(msg, "player[%d].carry_item   server:[%d] local:[%d]\n", p, c1_players[p].carry_item,  c2_players[p].carry_item);
               add_log_entry2(32, active_local_player, msg);
            }

            if (c1_players[p].up  != c2_players[p].up)
            {
               sprintf(msg, "player[%d].up   server:[%d] local:[%d]\n", p, c1_players[p].up,  c2_players[p].up);
               add_log_entry2(32, active_local_player, msg);
            }

            if (c1_players[p].down  != c2_players[p].down)
            {
               sprintf(msg, "player[%d].down   server:[%d] local:[%d]\n", p, c1_players[p].down,  c2_players[p].down);
               add_log_entry2(32, active_local_player, msg);
            }

            if (c1_players[p].left  != c2_players[p].left)
            {
               sprintf(msg, "player[%d].left   server:[%d] local:[%d]\n", p, c1_players[p].left,  c2_players[p].left);
               add_log_entry2(32, active_local_player, msg);
            }

            if (c1_players[p].right  != c2_players[p].right)
            {
               sprintf(msg, "player[%d].right   server:[%d] local:[%d]\n", p, c1_players[p].right,  c2_players[p].right);
               add_log_entry2(32, active_local_player, msg);
            }

            if (c1_players[p].jump  != c2_players[p].jump)
            {
               sprintf(msg, "player[%d].jump   server:[%d] local:[%d]\n", p, c1_players[p].jump,  c2_players[p].jump);
               add_log_entry2(32, active_local_player, msg);
            }

            if (c1_players[p].fire  != c2_players[p].fire)
            {
               sprintf(msg, "player[%d].fire   server:[%d] local:[%d]\n", p, c1_players[p].fire,  c2_players[p].fire);
               add_log_entry2(32, active_local_player, msg);
            }

            if (c1_players[p].fire_held  != c2_players[p].fire_held)
            {
               sprintf(msg, "player[%d].fire_held   server:[%d] local:[%d]\n", p, c1_players[p].fire_held,  c2_players[p].fire_held);
               add_log_entry2(32, active_local_player, msg);
            }

            if (c1_players[p].menu  != c2_players[p].menu)
            {
               sprintf(msg, "player[%d].menu   server:[%d] local:[%d]\n", p, c1_players[p].menu,  c2_players[p].menu);
               add_log_entry2(32, active_local_player, msg);
            }


            if (c1_players[p].bullet_wait_counter  != c2_players[p].bullet_wait_counter)
            {
               sprintf(msg, "player[%d].bullet_wait_counter   server:[%d] local:[%d]\n", p, c1_players[p].bullet_wait_counter,  c2_players[p].bullet_wait_counter);
               add_log_entry2(32, active_local_player, msg);
            }

            if (c1_players[p].request_bullet  != c2_players[p].request_bullet)
            {
               sprintf(msg, "player[%d].request_bullet   server:[%d] local:[%d]\n", p, c1_players[p].request_bullet,  c2_players[p].request_bullet);
               add_log_entry2(32, active_local_player, msg);
            }

            if (c1_players[p].bullet_wait  != c2_players[p].bullet_wait)
            {
               sprintf(msg, "player[%d].bullet_wait   server:[%d] local:[%d]\n", p, c1_players[p].bullet_wait,  c2_players[p].bullet_wait);
               add_log_entry2(32, active_local_player, msg);
            }

            if (c1_players[p].bullet_speed  != c2_players[p].bullet_speed)
            {
               sprintf(msg, "player[%d].bullet_speed   server:[%d] local:[%d]\n", p, c1_players[p].bullet_speed,  c2_players[p].bullet_speed );
               add_log_entry2(32, active_local_player, msg);
            }

//            if (c1_players[p].not_used_num_bullets  != c2_players[p].not_used_num_bullets)
//            {
//               sprintf(msg, "player[%d].num_bullets   server:[%d] local:[%d]\n", p, c1_players[p].not_used_num_bullets,  c2_players[p].not_used_num_bullets );
//               add_log_entry2(32, active_local_player, msg);
//            }

            if (c1_players[p].num_hits  != c2_players[p].num_hits)
            {
               sprintf(msg, "player[%d].num_hits   server:[%d] local:[%d]\n", p, c1_players[p].num_hits,  c2_players[p].num_hits );
               add_log_entry2(32, active_local_player, msg);
            }

            if (c1_players[p].control_method  != c2_players[p].control_method)
            {
               sprintf(msg, "player[%d].control_method   server:[%d] local:[%d]\n", p, c1_players[p].control_method,  c2_players[p].control_method );
               add_log_entry2(32, active_local_player, msg);
            }



   /*
            if (c1_players[p].health_display  != c2_players[p].health_display)
            {
               sprintf(msg, "player[%d].health_display   server:[%d] local:[%d]\n", p, c1_players[p].health_display,  c2_players[p].health_display );
               add_log_entry2(32, active_local_player, msg);

            }
   */

         }
      }
   }
   if (memcmp(c1_Ei, c2_Ei, sizeof(Ei)))
   {
      if (L_LOGGING_NETPLAY_show_dif1)
      {
         sprintf(msg, "Ei errors detected\n");
         add_log_entry2(31, active_local_player, msg); printf("%s", msg);
      }
      if (L_LOGGING_NETPLAY_show_dif2)
      {
         for (int e=0; e<100; e++)
            for (int f=0; f<32; f++)
               if (c1_Ei[e][f] != c2_Ei[e][f])
               {
                  sprintf(msg, "Ei[%2d][%2d][t:%2d] server:%d local:%d\n", e, f, c1_Ei[e][0], c1_Ei[e][f], c2_Ei[e][f]);
                  add_log_entry2(32, active_local_player, msg);
               }
      }
   }
   if (memcmp(c1_Efi, c2_Efi, sizeof(Efi)))
   {
      if (L_LOGGING_NETPLAY_show_dif1)
      {
         sprintf(msg, "Efi errors detected\n");
         add_log_entry2(31, active_local_player, msg); printf("%s", msg);
      }
      if (L_LOGGING_NETPLAY_show_dif2)
      {
         for (int e=0; e<100; e++)
            for (int f=0; f<16; f++)
               if (c1_Efi[e][f] != c2_Efi[e][f])
               {
                  sprintf(msg, "Efi[%2d][%2d][t:%2d] server:%f local:%f\n", e, f, c1_Ei[e][0], al_fixtof(c1_Efi[e][f]), al_fixtof(c2_Efi[e][f]));
                  add_log_entry2(32, active_local_player, msg);
               }
      }
   }
   if (memcmp(c1_item, c2_item, sizeof(item)))
   {
      if (L_LOGGING_NETPLAY_show_dif1)
      {
         sprintf(msg, "item errors detected\n");
         add_log_entry2(31, active_local_player, msg); printf("%s", msg);
      }
      if (L_LOGGING_NETPLAY_show_dif2)
      {
         for (int e=0; e<500; e++)
            for (int f=0; f<16; f++)
               if (c1_item[e][f] != c2_item[e][f])
               {
                  sprintf(msg, "item[%d][%d][t:%2d] server:%d local:%d\n", e, f, c1_item[e][0], c1_item[e][f], c2_item[e][f]);
                  add_log_entry2(32, active_local_player, msg);
               }
      }
   }
   if (memcmp(c1_itemf, c2_itemf, sizeof(itemf)))
   {
      if (L_LOGGING_NETPLAY_show_dif1)
      {
         sprintf(msg, "itemf errors detected\n");
         add_log_entry2(31, active_local_player, msg); printf("%s", msg);
      }
      if (L_LOGGING_NETPLAY_show_dif2)
      {
         for (int e=0; e<500; e++)
            for (int f=0; f<4; f++)
               if (c1_itemf[e][f] != c2_itemf[e][f])
               {
                  sprintf(msg, "itemf[%d][%d][t:%2d] server:%f local:%f\n", e, f, c1_item[e][0], al_fixtof(c1_itemf[e][f]), al_fixtof(c2_itemf[e][f]));
                  add_log_entry2(32, active_local_player, msg);
               }
      }
   }
   if (memcmp(c1_lifts, c2_lifts, sizeof(lifts)))
   {
      if (L_LOGGING_NETPLAY_show_dif1)
      {
         sprintf(msg, "lift errors detected\n");
         add_log_entry2(31, active_local_player, msg); printf("%s", msg);
      }
      if (L_LOGGING_NETPLAY_show_dif2)
      {
         for (int e=0; e<40; e++)
         {
            if (c1_lifts[e].fx    != c2_lifts[e].fx )
            {
               sprintf(msg, "lifts[%d].fx server:%f local:%f\n",    e, al_fixtof(c1_lifts[e].fx),    al_fixtof(c2_lifts[e].fx));
               add_log_entry2(32, active_local_player, msg);
            }
            if (c1_lifts[e].fy    != c2_lifts[e].fy )
            {
               sprintf(msg, "lifts[%d].fy server:%f local:%f\n",    e, al_fixtof(c1_lifts[e].fy),    al_fixtof(c2_lifts[e].fy));
               add_log_entry2(32, active_local_player, msg);
            }
            if (c1_lifts[e].fxinc != c2_lifts[e].fxinc )
            {
               sprintf(msg, "lifts[%d].fxinc server:%f local:%f\n", e, al_fixtof(c1_lifts[e].fxinc), al_fixtof(c2_lifts[e].fxinc));
               add_log_entry2(32, active_local_player, msg);
            }
            if (c1_lifts[e].fyinc != c2_lifts[e].fyinc )
            {
               sprintf(msg, "lifts[%d].fyinc server:%f local:%f\n", e, al_fixtof(c1_lifts[e].fyinc), al_fixtof(c2_lifts[e].fyinc));
               add_log_entry2(32, active_local_player, msg);
            }
         }
      }
   }
   if (memcmp(c1_l, c2_l, sizeof(l)))
   {
      if (L_LOGGING_NETPLAY_show_dif1)
      {
         sprintf(msg, "block errors detected\n");
         add_log_entry2(31, active_local_player, msg); printf("%s", msg);
      }
      if (L_LOGGING_NETPLAY_show_dif2)
      {
         for (int e=0; e<100; e++)
            for (int f=0; f<100; f++)
               if (c1_l[e][f] != c2_l[e][f])
               {
                  sprintf(msg, "block[%d][%d] server:%d local:%d\n", e, f, c1_l[e][f], c2_l[e][f] );
                  add_log_entry2(32, active_local_player, msg);
               }
      }
   }
}






















































int fill_demo_array(ALLEGRO_FS_ENTRY *fs, void * extra)
{
   if (num_demo_filenames > 99) return 0; // only get 100 max
   demo_FS_filenames[num_demo_filenames] = al_create_fs_entry(al_get_fs_entry_name(fs));
   demo_played[num_demo_filenames] = 0;
   num_demo_filenames++;
   return ALLEGRO_FOR_EACH_FS_ENTRY_OK;
}

void demo_mode(void)
{
   demo_mode_on = 1;
   num_demo_filenames = 0;

   char fname[1024];
   sprintf(fname, "savegame/demo");

   //printf("fname:%s\n", fname);

   // convert to 'ALLEGRO_FS_ENTRY' (also makes fully qualified path)
   ALLEGRO_FS_ENTRY *FS_fname = al_create_fs_entry(fname);
   sprintf(fname, "%s", al_get_fs_entry_name(FS_fname));

   //printf("FS_fname:%s\n", fname);

   // iterate levels in demo folder and put in filename array
   al_for_each_fs_entry(FS_fname, fill_demo_array, NULL);

//   for (int i=0; i< num_demo_filenames; i++)
//     printf("%s\n", al_get_fs_entry_name(demo_FS_filenames[i]));

   if (num_demo_filenames == 0)
   {
      printf("No demo files found.\n");
      demo_mode_on = 0;
   }

   int prev_lev = 0, lev, pass = 1;
   while (demo_mode_on)
   {
      if (num_demo_filenames > 1)
      {
         // have all levels this pass been played?
         int all_played = 1;
         for (int i=0; i< num_demo_filenames; i++)
            if (demo_played[i] < pass) all_played = 0;
         if (all_played == 1) pass++; // next pass

         lev = -1;
         while (lev < 0)
         {
            lev = rand() % num_demo_filenames;      // get random index
            if (demo_played[lev] >= pass) lev = -1; // already been played this pass
            if (prev_lev == lev) lev = -1;          // just previously played
         }
         demo_played[lev] = pass;
         prev_lev = lev;
      }
      else lev = 0;

//     printf("demo level index [%d]\n", lev);
//     for (int i=0; i< num_demo_filenames; i++)
//        printf("%d demo_played[%d] - \n", i, demo_played[i]);

      if (load_gm(al_get_fs_entry_name(demo_FS_filenames[lev])))
      {
         game_loop(9); // demo game
      }
      else demo_mode_on = 0;
   }

   load_level(start_level, 0);

   // reset player data
   for (int p=0; p<NUM_PLAYERS; p++) init_player(p, 1);
   players[0].active = 1;
   active_local_player = 0;
   get_config_values(); // restore player color from config file
   erase_log();
}

void temp_test(void)
{



//   int p[8] = {0};
//   for (int ns=1; ns<9; ns++)
//   {
//      printf("ns = %d\n", ns);
//      for (int q=0; q<8; q++) p[q] = q % ns;
//      for (int q=0; q<8; q++) printf("p:%d s:%d\n", q, p[q]);
//   }







   //spline_test();
   //spline_adjust();
}



