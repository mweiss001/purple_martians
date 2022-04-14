// zlifts.cpp

#include "pm.h"

int construct_lift(int l, char* lift_name, int width, int height, int color, int num_steps)
{
   strcpy(lifts[l].lift_name, lift_name);
   lifts[l].width = width;
   lifts[l].height = height;
   lifts[l].color = color;
   lifts[l].num_steps = num_steps;
   lifts[l].x1 = 0;
   lifts[l].y1 = 0;
   lifts[l].x2 = 0;
   lifts[l].y2 = 0;
   lifts[l].current_step = 0;
   lifts[l].limit_type = 0;
   lifts[l].limit_counter = 0;
   return 1;
}

void clear_lift(int l)
{
   lifts[l].width = 0;
   lifts[l].height = 0;
   lifts[l].color = 0;
   lifts[l].num_steps = 0;
   lifts[l].x1 = 0;
   lifts[l].y1 = 0;
   lifts[l].x2 = 0;
   lifts[l].y2 = 0;
   lifts[l].current_step = 0;
   lifts[l].limit_type = 0;
   lifts[l].limit_counter = 0;
   strcpy(lifts[l].lift_name, "");
}

int construct_lift_step(int lift, int step, int x, int y, int w, int h, int val, int type)
{
   lift_steps[lift][step].x    = x;
   lift_steps[lift][step].y    = y;
   lift_steps[lift][step].w    = w;
   lift_steps[lift][step].h    = h;
   lift_steps[lift][step].val  = val;
   lift_steps[lift][step].type = type;
   return 1;
}

void clear_lift_step(int lift, int step)
{
   lift_steps[lift][step].x    = 0;
   lift_steps[lift][step].y    = 0;
   lift_steps[lift][step].val  = 0;
   lift_steps[lift][step].type = 0;
}

void set_lift_to_step(int lift, int step)
{
   // new behaviour
   // if passed step is a move step, set that
   // if not, then set to previous move step

   if (lift_steps[lift][step].type != 1) step = lift_find_previous_move_step(lift, step);

   lifts[lift].current_step = step; // initial step
   lifts[lift].limit_type = 2;      // type wait for time
   lifts[lift].limit_counter = 0;   // 0 = no wait, immediate next mode

   // get pos and size from step
   lifts[lift].x1     = lift_steps[lift][step].x;
   lifts[lift].y1     = lift_steps[lift][step].y;
   lifts[lift].width  = lift_steps[lift][step].w;
   lifts[lift].height = lift_steps[lift][step].h;
   lifts[lift].x2     = lifts[lift].x1 + lifts[lift].width;
   lifts[lift].y2     = lifts[lift].y1 + lifts[lift].height;

   // set fixed positions too
   lifts[lift].fx = al_itofix(lifts[lift].x1);
   lifts[lift].fy = al_itofix(lifts[lift].y1);
   lifts[lift].fw = al_itofix(lifts[lift].width);
   lifts[lift].fh = al_itofix(lifts[lift].height);

   // set incs to 0
   lifts[lift].fxinc = al_itofix(0);
   lifts[lift].fyinc = al_itofix(0);
   lifts[lift].fwinc = al_itofix(0);
   lifts[lift].fhinc = al_itofix(0);
}


int is_player_riding_lift(int l)
{
   for (int p=0; p<NUM_PLAYERS; p++)
      if ((players[p].active) && (!players[p].paused))
         if ((players[p].player_ride) && (l == players[p].player_ride - 32)) return p+1; // player is riding this lift
   return 0;
}

void draw_lift_lines()
{
   for (int x=0; x<num_lifts; x++)  // cycle lifts
   {
      int col = lifts[x].color+128;
      int sx = lift_steps[x][0].x + lift_steps[x][0].w / 2;  // start pos
      int sy = lift_steps[x][0].y + lift_steps[x][0].h / 2;
      int px = sx; // previous
      int py = sy;
      int nx = 0;  // next
      int ny = 0;
      al_set_target_bitmap(level_background);
      if (lifts[x].num_steps > 1) // only draw lines if more than one step
      {
         for (int y=0; y<lifts[x].num_steps; y++)  // cycle step
         {
            if (lift_steps[x][y].type == 1) // look for move step
            {
               nx = lift_steps[x][y].x + lift_steps[x][y].w / 2; // don't actually do the move
               ny = lift_steps[x][y].y + lift_steps[x][y].h / 2; // don't actually do the move

               al_draw_line( px, py, nx, ny, palette_color[col], 1);
               for (int c=3; c>=0; c--)
                  al_draw_filled_circle(nx, ny, c, palette_color[(col - 96) + c*48]);
               px = nx;
               py = ny;
            }
         }
         al_draw_line(sx, sy, nx, ny, palette_color[col], 1); // draw line from last to first
      }
   }
}

void draw_lifts()
{
   al_set_target_bitmap(level_buffer);
   for (int l=0; l<num_lifts; l++)
   {
      int x1 = lifts[l].x1;
      int x2 = lifts[l].x2;
      int y1 = lifts[l].y1;
      int y2 = lifts[l].y2;
      int color = lifts[l].color;

      int a;

      // faded outer shell
      for (a=0; a<10; a++)
        al_draw_rounded_rectangle(x1+a, y1+a, x2-a, y2-a, 4, 4, palette_color[color + ((9 - a)*16)], 2 );

      // solid core
      al_draw_filled_rectangle(x1+a, y1+a, x2-a, y2-a, palette_color[color] );

      // name
//      al_draw_text(font, palette_color[color+160], (x1+x2)/2, (y1+y2)/2 - 3, ALLEGRO_ALIGN_CENTRE, lifts[l].lift_name);
       al_draw_textf(font, palette_color[color+160], (x1+x2)/2, (y1+y2)/2 - 3, ALLEGRO_ALIGN_CENTRE, "step:%d val:%2d", lifts[l].current_step, lifts[l].val1);

      // show if player is riding this lift
      int p = is_player_riding_lift(l);
      if (p)
      {
         p -=1; // player number
         int pc = players[p].color;
         if (pc == color) pc = 127;
         al_draw_rounded_rectangle(x1, y1, x2, y2, 4, 4, palette_color[pc], 2);
      }


//      if ((lifts[l].width == 1) && (lifts[l].height > 1)) // rotate lift name for vertical lifts
//         rtextout_centre(level_buffer, lifts[l].lift_name, ((x1+x2)/2), ((y1+y2)/2), color+160, 1, 64, 1);
//      else


      if ((lifts[l].mode == 1) && (!is_player_riding_lift(l)) && (lifts[l].current_step > 1))
      {
         int percent = (100 * lifts[l].val1) / 80;
         draw_percent_bar((x1+x2)/2, (y1+y2)/2 -44, 40, 8, percent);
      }

      switch (lifts[l].limit_type) // limit type
      {
         case 2: // timer wait
            if (lifts[l].limit_counter > 0)
               al_draw_textf(font, palette_color[color+64], (x1 + x2)/2 + 2, lifts[l].y1 - 8, ALLEGRO_ALIGN_CENTRE, "%d", lifts[l].limit_counter);
         break;
         case 3: // prox wait
         {
            int pd = lifts[l].limit_counter; // prox dist
            int bx1 = x1 - pd;
            int by1 = y1 - pd;
            int bx2 = x2 + pd;
            int by2 = y2 + pd;
            al_draw_rectangle(bx1+10, by1+10, bx2-10, by2-10, palette_color[color+128], 1);
         }
         break;
      }
   }  // end of iterate lifts
}


void set_lift_xyinc(int d, int step)
{
   //  used when switching to a new move step;
   //  sets xinc, yinc and num of frames for mode 7 (move)

   int val = lift_steps[d][step].val;

   al_fixed move_time = al_itofix(0);

   al_fixed xlen = al_itofix(lift_steps[d][step].x - lifts[d].x1);   // x dif
   al_fixed ylen = al_itofix(lift_steps[d][step].y - lifts[d].y1);   // y dif
   al_fixed hy_dist = al_fixhypot(xlen, ylen);                       // hypotenuse is distance

   if (hy_dist < al_itofix(1))        // no move
   {
      move_time = al_itofix(val);     // use val as straight timer
      lifts[d].fxinc = al_itofix(0);  // no xinc
      lifts[d].fyinc = al_itofix(0);  // no yinc
   }
   else // get time based on speed and distance
   {
      al_fixed speed = al_itofix(val) / 10;         // stored speed scaled by 10
      move_time = al_fixdiv(hy_dist, speed);        // get time (time = distance / speed)
      lifts[d].fxinc = al_fixdiv(xlen, move_time);  // set xinc
      lifts[d].fyinc = al_fixdiv(ylen, move_time);  // set yinc
   }

//   printf("xlen:%4d ylen:%4d hy:%4d mt:%4d fxinc:%4d fyinc:%4d\n",
//           al_fixtoi(xlen), al_fixtoi(ylen), al_fixtoi(hy_dist), al_fixtoi(move_time), al_fixtoi(lifts[d].fxinc), al_fixtoi(lifts[d].fyinc));
//   printf("xlen:%3.2f ylen:%3.2f hy:%3.2f mt:%3.2f fxinc:%3.2f fyinc:%3.2f\n",
//           al_fixtof(xlen), al_fixtof(ylen), al_fixtof(hy_dist), al_fixtof(move_time), al_fixtof(lifts[d].fxinc), al_fixtof(lifts[d].fyinc));

   al_fixed wd = al_itofix(lift_steps[d][step].w - lifts[d].width);  // width dif
   al_fixed hd = al_itofix(lift_steps[d][step].h - lifts[d].height); // height dif
   lifts[d].fwinc = al_fixdiv(wd, move_time);                        // set winc
   lifts[d].fhinc = al_fixdiv(hd, move_time);                        // set hinc

   lifts[d].limit_counter = al_fixtoi(move_time);
   lifts[d].limit_type = 1;
}

int lift_check_prox(int l, int pd)
{
   // check if player is within lift proximity
   int bx1 = lifts[l].x1 - pd - 10;
   int by1 = lifts[l].y1 - pd - 10;
   int bx2 = lifts[l].x2 + pd - 10;
   int by2 = lifts[l].y2 + pd - 10;

   for (int p=0; p<NUM_PLAYERS; p++)
      if (players[p].active)
         if ((players[p].PX > al_itofix(bx1)) && (players[p].PX < al_itofix(bx2)))
            if ((players[p].PY > al_itofix(by1)) && (players[p].PY < al_itofix(by2))) return 1;
   return 0;
}



void move_lifts(int ignore_prox)
{
   for (int l=0; l<num_lifts; l++)
   {
      int next_step = 0;

      int frozen = 0;

//      if ((lifts[d].mode == 1) && (!is_player_riding_lift(d)) && (lifts[d].current_step > 0))

      int mode = lifts[l].mode;
      int cs = lifts[l].current_step;
      int cst = lift_steps[l][cs].type;


      // modes----------------------------------------------------------------------
      if ((mode == 1) && (cs > 0) && (cst == 1)) // prox run and reset mode, not step 0, current step type = move
      {
         if (is_player_riding_lift(l))
         {
            lifts[l].val1 = 80;
         }

         else
         {
            frozen = 1;
            if (--lifts[l].val1 < 0)
            {
               lifts[l].val1 = 80;
               set_lift_to_step(l, 0);
            }
         }
      }




      if (!frozen)
      {
         // limits ----------------------------------------------------------------------
         switch (lifts[l].limit_type) // limit type
         {
            case 1: // step count for move

               lifts[l].fx += lifts[l].fxinc;
               lifts[l].fy += lifts[l].fyinc;
               lifts[l].fw += lifts[l].fwinc;
               lifts[l].fh += lifts[l].fhinc;

               lifts[l].x1     = al_fixtoi(lifts[l].fx);    // put as int in x1
               lifts[l].y1     = al_fixtoi(lifts[l].fy);    // put as int in y1
               lifts[l].width  = al_fixtoi(lifts[l].fw);    // put as int in width
               lifts[l].height = al_fixtoi(lifts[l].fh);    // put as int in height

               lifts[l].x2 = lifts[l].x1 + (lifts[l].width)-1;
               lifts[l].y2 = lifts[l].y1 + (lifts[l].height)-1;

               if (--lifts[l].limit_counter < 0)
               {
                  next_step = 1;

                  //make sure lift is exactly where it should be at the end of the move...
                  int step = lifts[l].current_step;
                  lifts[l].x1     = lift_steps[l][step].x;
                  lifts[l].y1     = lift_steps[l][step].y;
                  lifts[l].width  = lift_steps[l][step].w;
                  lifts[l].height = lift_steps[l][step].h;

                  lifts[l].x2 = lifts[l].x1 + lifts[l].width;
                  lifts[l].y2 = lifts[l].y1 + lifts[l].height;

                  // set fixed too
                  lifts[l].fx = al_itofix(lifts[l].x1);
                  lifts[l].fy = al_itofix(lifts[l].y1);
                  lifts[l].fw = al_itofix(lifts[l].width);
                  lifts[l].fh = al_itofix(lifts[l].height);

                  // zero incs when done
                  lifts[l].fxinc = al_itofix(0);
                  lifts[l].fyinc = al_itofix(0);
                  lifts[l].fwinc = al_itofix(0);
                  lifts[l].fhinc = al_itofix(0);
               }
            break;
            case 2: // timer wait
               if (--lifts[l].limit_counter < 0) next_step = 1;
            break;
            case 3: // prox wait
               if (ignore_prox) next_step = 1;
               else if (lift_check_prox(l, lifts[l].limit_counter)) next_step = 1;
            break;

         }
      }


      // changing steps ----------------------------------------------------------------------
      if (next_step)
      {
         if (++lifts[l].current_step > lifts[l].num_steps - 1)    // increment step
               lifts[l].current_step = lifts[l].num_steps -1;     // bounds check (for lifts with no loop to zero)

         int step = lifts[l].current_step;
         next_step = 0;
         switch (lift_steps[l][step].type)
         {
            case 1: // move
               set_lift_xyinc(l, step);
            break;
            case 2: // wait time
               lifts[l].limit_type = 2; // wait time
               lifts[l].limit_counter = lift_steps[l][step].val; // limit
               lifts[l].fxinc= 0; // no xinc
               lifts[l].fyinc= 0; // no yinc
            break;
            case 3: // wait prox
               lifts[l].limit_type = 3; // wait prox
               lifts[l].limit_counter = lift_steps[l][step].val; // limit
               lifts[l].fxinc= 0; // no xinc
               lifts[l].fyinc= 0; // no yinc
            break;
            case 4: // end step
               if (lift_steps[l][step].val == 0) // move to step 0
               {
                  lifts[l].current_step = 0;
                  set_lift_xyinc(l, 0);
               }
               if (lift_steps[l][step].val == 1) // warp to step 0
               {
                  set_lift_to_step(l, 0);
               }
               if (lift_steps[l][step].val == 2) // freeze here
               {
                  lifts[l].limit_type = 0;
               }




            /*
               if (lifts[l].mode == 0)
               {
                  step = lifts[l].current_step = 0; // set step 0
                  set_lift_xyinc(l, step);
               }

               if (lifts[l].mode == 1) // immediately warp to step 0
               {
                  set_lift_to_step(l, 0);
               }
*/
            break;
         }
      } // end of next step
   } // end of lift iterate
}

