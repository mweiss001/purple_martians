// z_lift.cpp

#include "pm.h"

int construct_lift(int l, char* lift_name)
{
   clear_lift(l);
   strcpy(lifts[l].lift_name, lift_name);
   lifts[l].x1 = 0;
   lifts[l].y1 = 0;
   lifts[l].x2 = 0;
   lifts[l].y2 = 0;
   lifts[l].current_step = 0;
   lifts[l].limit_type = 0;
   lifts[l].limit_counter = 0;
   lifts[l].flags = 0;
   return 1;
}

void clear_lift(int l)
{
   al_fixed f0 = al_itofix(0);
   lifts[l].fx = f0;
   lifts[l].fy = f0;
   lifts[l].fxinc = f0;
   lifts[l].fyinc = f0;
   lifts[l].fw = f0;
   lifts[l].fh = f0;
   lifts[l].fwinc = f0;
   lifts[l].fhinc = f0;
   lifts[l].x1 = 0;
   lifts[l].y1 = 0;
   lifts[l].x2 = 0;
   lifts[l].y2 = 0;
   lifts[l].width = 0;
   lifts[l].height = 0;
   lifts[l].flags = 0;
   lifts[l].mode = 0;
   lifts[l].val1 = 0;
   lifts[l].val2 = 0;
   lifts[l].color = 0;
   lifts[l].current_step = 0;
   lifts[l].num_steps = 0;
   lifts[l].limit_type = 0;
   lifts[l].limit_counter = 0;
   strcpy(lifts[l].lift_name, "");
}

int construct_lift_step(int l, int s, int type, int x, int y, int w, int h, int val)
{
   clear_lift_step(l, s);
//   printf("type:%d\n", type);

   if ((type > 0) && (type < 16)) // get flags and color from previous steps
   {
      int ps = 0;
      if (type == 1) // find previous move step
      {
          ps = lift_find_previous_move_step(l, s);
          type = lift_steps[l][ps].type;
          val  = 20;
          w    = lift_steps[l][ps].w;
          h    = lift_steps[l][ps].h;
      }
      else // find any previous step
      {
         if (s == 0) ps = lifts[l].num_steps - 1; // if step 0 get last step
         else ps = s - 1;

         // get flags and color by masking off lower 5 bits of type
         int cf = lift_steps[l][ps].type & 0b11111111111111111111111111100000;
         type |= cf; // merge with type
      }
   }

   lift_steps[l][s].type = type;
   lift_steps[l][s].x    = x;
   lift_steps[l][s].y    = y;
   lift_steps[l][s].w    = w;
   lift_steps[l][s].h    = h;
   lift_steps[l][s].val  = val;
   return 1;
}

void clear_lift_step(int l, int s)
{
   lift_steps[l][s].type = 0;
   lift_steps[l][s].x    = 0;
   lift_steps[l][s].y    = 0;
   lift_steps[l][s].w    = 0;
   lift_steps[l][s].h    = 0;
   lift_steps[l][s].val  = 0;
}



void set_lift_to_step(int l, int s)
{
   // new behaviour
   // if passed step is a move step, set that
   // if not, then set to previous move step

   if ((lift_steps[l][s].type & 31) != 1) s = lift_find_previous_move_step(l, s);

   lifts[l].current_step = s; // initial step
   lifts[l].limit_type = 2;      // type wait for time
   lifts[l].limit_counter = 0;   // 0 = no wait, immediate next mode

   // get pos and size from step
   lifts[l].x1     = lift_steps[l][s].x;
   lifts[l].y1     = lift_steps[l][s].y;
   lifts[l].width  = lift_steps[l][s].w;
   lifts[l].height = lift_steps[l][s].h;
   lifts[l].x2     = lifts[l].x1 + lifts[l].width;
   lifts[l].y2     = lifts[l].y1 + lifts[l].height;
   lifts[l].flags = lift_steps[l][s].type; // get all the flags from the step type

 //  lifts[l].color  = (lift_steps[l][s].type >> 28) & 15; // not needed

   // set fixed positions too
   lifts[l].fx = al_itofix(lifts[l].x1);
   lifts[l].fy = al_itofix(lifts[l].y1);
   lifts[l].fw = al_itofix(lifts[l].width);
   lifts[l].fh = al_itofix(lifts[l].height);

   // set incs to 0
   lifts[l].fxinc = al_itofix(0);
   lifts[l].fyinc = al_itofix(0);
   lifts[l].fwinc = al_itofix(0);
   lifts[l].fhinc = al_itofix(0);
}


int is_player_riding_lift(int l)
{
   for (int p=0; p<NUM_PLAYERS; p++)
      if ((players[p].active) && (!players[p].paused))
         if ((players[p].player_ride) && (l == players[p].player_ride - 32)) return p+1; // player is riding this lift
   return 0;
}




void draw_lift_line(int l)
{
   if ((!(lifts[l].flags & PM_LIFT_NO_DRAW)) || (level_editor_running))
   {
      int col = 15;
      int sx = lift_steps[l][0].x + lift_steps[l][0].w / 2;  // start pos
      int sy = lift_steps[l][0].y + lift_steps[l][0].h / 2;
      int px = sx; // previous
      int py = sy;
      int nx = 0;  // next
      int ny = 0;
      if (lifts[l].num_steps > 1) // only draw lines if more than one step
      {
         for (int s=0; s<lifts[l].num_steps; s++) // cycle steps
         {
            if ((lift_steps[l][s].type & 31) == 1) // filter for move steps
            {
               nx = lift_steps[l][s].x + lift_steps[l][s].w / 2;
               ny = lift_steps[l][s].y + lift_steps[l][s].h / 2;


               if (!(lift_steps[l][s].type & PM_LIFT_HIDE_LINES))
               {
                  col = (lift_steps[l][s].type >> 28) & 15;
                  al_draw_line( px, py, nx, ny, palette_color[col], 1);
                  for (int c=3; c>=0; c--)
                     al_draw_filled_circle(nx, ny, c, palette_color[(col - 96) + c*48]);
               }

               px = nx;
               py = ny;
            }
         }
         if (!(lift_steps[l][0].type & PM_LIFT_HIDE_LINES))
         {
            col = (lift_steps[l][0].type >> 28) & 15;
            al_draw_line(sx, sy, nx, ny, palette_color[col], 1); // draw line from last to first
         }
      }
   }
}

















// this is it....the one base function that draws a lift
void draw_lift(int l, int x1, int y1, int x2, int y2)
{
   int col = (lifts[l].flags >> 28) & 15;

   if ((lifts[l].flags & PM_LIFT_NO_DRAW) && (level_editor_running)) col = 0;

   int a;
   for (a=0; a<10; a++)
     al_draw_rounded_rectangle(x1+a, y1+a, x2-a, y2-a, 4, 4, palette_color[col + ((9 - a)*16)], 2 ); // faded outer shell
   al_draw_filled_rectangle(x1+a, y1+a, x2-a, y2-a, palette_color[col] );                            // solid core
   al_draw_text(font0, palette_color[col+160], (x1+x2)/2, (y1+y2)/2 - 3, ALLEGRO_ALIGN_CENTRE, lifts[l].lift_name); // name

  // printf("x1:%d y1:%d x2:%d y2:%d\n", x1, y1, x2, y2);

//   al_draw_textf(font, palette_color[col+160], (x1+x2)/2, (y1+y2)/2 - 3, ALLEGRO_ALIGN_CENTRE, "s:%d v:%d", lifts[l].current_step, lifts[l].val1);    // debug name

//   al_draw_textf(font, palette_color[15], (x1+x2)/2, (y1+y2)/2 - 16, ALLEGRO_ALIGN_CENTRE, "s:%d v:%d", lifts[l].current_step, lifts[l].val1);    // debug name


}


void draw_lifts()
{
   al_set_target_bitmap(level_buffer);

   for (int l=0; l<num_lifts; l++) draw_lift_line(l);

   for (int l=0; l<num_lifts; l++)
   {
      if ((!(lifts[l].flags & PM_LIFT_NO_DRAW)) || (level_editor_running))
      {
         int color = (lifts[l].flags >> 28) & 15;
         int x1 = lifts[l].x1;
         int x2 = lifts[l].x2;
         int y1 = lifts[l].y1;
         int y2 = lifts[l].y2;
         draw_lift(l, x1, y1, x2, y2);

         // show if player is riding this lift
         int p = is_player_riding_lift(l);
         if (p)
         {
            p -=1; // player number
            int pc = players[p].color;
            if (pc == color) pc = 127;
            al_draw_rounded_rectangle(x1, y1, x2, y2, 4, 4, palette_color[pc], 2);
         }

         if ((lifts[l].mode) && (!is_player_riding_lift(l)))
         {
            // if not in initial position
            if (!((lifts[l].x1 == lift_steps[l][0].x) && (lifts[l].y1 == lift_steps[l][0].y)))
            {
               int lw = lifts[l].width-10;
               int lh = lifts[l].height-10;
               if (lifts[l].val2 == 0) lifts[l].val2 = 1; // to prevent divide by zero
               int percent = (100 * lifts[l].val1) / lifts[l].val2;
               draw_percent_bar((x1+x2)/2, y1+4, lw, lh, percent);
            }
         }

         int cs = lifts[l].current_step;
         if (!(lift_steps[l][cs].type & PM_LIFT_HIDE_LINES))
         {
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
         }
      }
   }  // end of iterate lifts
}


void set_lift_xyinc(int d, int step)
{
   //  used when switching to a new move step;
   //  sets xinc, yinc and num of frames for move

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
      if ((players[p].active) && (!players[p].paused))
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

      if (lifts[l].mode == 1) // prox run and reset mode
      {
         if (is_player_riding_lift(l)) lifts[l].val1 = lifts[l].val2; // reset timer
         else
         {
            frozen = 1;
            if (--lifts[l].val1 < 0)
            {
               lifts[l].val1 = lifts[l].val2; // reset timer
               set_lift_to_step(l, 0);
            }
         }
      }

      if (lifts[l].mode == 2) // prox reset mode
      {
         if (is_player_riding_lift(l)) lifts[l].val1 = lifts[l].val2; // reset timer
         else
         {
            if (--lifts[l].val1 < 0)
            {
               lifts[l].val1 = lifts[l].val2; // reset timer
               set_lift_to_step(l, 0);
            }
         }
      }



      if (!frozen)
      {
         // limits ----------------------------------------------------------------------
         switch (lifts[l].limit_type) // limit type
         {
            case 1: // step type 1 - move and resize

               lifts[l].fx += lifts[l].fxinc;
               lifts[l].fy += lifts[l].fyinc;
               lifts[l].fw += lifts[l].fwinc;
               lifts[l].fh += lifts[l].fhinc;

               lifts[l].x1     = al_fixtoi(lifts[l].fx);    // put as int in x1
               lifts[l].y1     = al_fixtoi(lifts[l].fy);    // put as int in y1
               lifts[l].width  = al_fixtoi(lifts[l].fw);    // put as int in width
               lifts[l].height = al_fixtoi(lifts[l].fh);    // put as int in height

               lifts[l].x2 = lifts[l].x1 + lifts[l].width;
               lifts[l].y2 = lifts[l].y1 + lifts[l].height;

               if (--lifts[l].limit_counter < 0) // run down the counter
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

            case 5: // trigger wait
               if (pm_event[lifts[l].limit_counter]) next_step = 1;
            break;
         }
      }


      // changing steps ----------------------------------------------------------------------
      if (next_step)
      {
         next_step = 0;
         if (++lifts[l].current_step > lifts[l].num_steps - 1) lifts[l].current_step = lifts[l].num_steps -1; // increment step with bounds check
         int step = lifts[l].current_step;
         lifts[l].flags = lift_steps[l][step].type; // get flags from the step type
         switch (lift_steps[l][step].type & 31)
         {
            case 1: // move
               set_lift_xyinc(l, step);
            break;
            case 2: // wait time
               lifts[l].limit_type = 2; // wait time
               lifts[l].limit_counter = lift_steps[l][step].val;
            break;
            case 3: // wait prox
               lifts[l].limit_type = 3; // wait prox
               lifts[l].limit_counter = lift_steps[l][step].val;
            break;
            case 5: // wait trig
               lifts[l].limit_type = 5; // wait trig
               lifts[l].limit_counter = lift_steps[l][step].val;
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
            break;
         }
      } // end of next step

   // clear events referenced by this lift
   for (int s=0; s<lifts[l].num_steps; s++)  // iterate steps
      if ((lift_steps[l][s].type & 31) == 5)        // trigger mode
         pm_event[lift_steps[l][s].val] = 0; // clear trigger event

   } // end of lift iterate
}
