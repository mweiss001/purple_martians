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

int construct_lift_step(int lift, int step, int x, int y, int val, int type)
{
   lift_steps[lift][step].x    = x;
   lift_steps[lift][step].y    = y;
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

void set_lift(int lift, int step)
{
   // searches back from passed step until a move step is found or step 0 is reached
   // sets coordinates in lift from that step
   // then sets the current step in lift to one previous than that (or 0 if no prev)
   // set the type and val to immediately advance to the next step when run

   while ((lift_steps[lift][step].type != 1) && (step > 0) ) step--;

   lifts[lift].x1 = lift_steps[lift][step].x;
   lifts[lift].y1 = lift_steps[lift][step].y;
   lifts[lift].x2 = lifts[lift].x1 + lifts[lift].width*20;
   lifts[lift].y2 = lifts[lift].y1 + lifts[lift].height*20;
   lifts[lift].fx = al_itofix(lifts[lift].x1);
   lifts[lift].fy = al_itofix(lifts[lift].y1);
   lifts[lift].fxinc = al_itofix(0);
   lifts[lift].fyinc = al_itofix(0);

   int c = step-1;
   if (c < 0) c = 0;
   lifts[lift].current_step = c;   // initial step
   lifts[lift].limit_type = 5;     // type wait for time
   lifts[lift].limit_counter = 0;  // 0 = no wait! immediate next mode
}

void draw_lift_lines()
{
   for (int x=0; x<num_lifts; x++)  // cycle lifts
   {
      int col = lifts[x].color+128;
      int sx = lift_steps[x][0].x + lifts[x].width * 10;
      int sy = lift_steps[x][0].y + lifts[x].height * 10;
      int px = sx;
      int py = sy;
      int nx = 0;
      int ny = 0;
      al_set_target_bitmap(l2000);
      if (lifts[x].num_steps > 1) // only draw lines if more than one step
      {
         for (int y=0; y<lifts[x].num_steps; y++)  // cycle step
            if (lift_steps[x][y].type == 1) // look for move step
            {
               nx = lift_steps[x][y].x + lifts[x].width * 10;
               ny = lift_steps[x][y].y + lifts[x].height * 10;
               al_draw_line( px, py, nx, ny, palette_color[col], 1);
               for (int c=3; c>=0; c--)
                  al_draw_filled_circle(nx, ny, c, palette_color[(col - 96) + c*48]);
               px = nx;
               py = ny;
            }
         al_draw_line(sx, sy, nx, ny, palette_color[col], 1);
      }
   }
}




void draw_lifts()
{
   al_set_target_bitmap(level_buffer);
   for (int d=0; d<num_lifts; d++)
   {
      int x1 = lifts[d].x1;
      int x2 = lifts[d].x2;
      int y1 = lifts[d].y1;
      int y2 = lifts[d].y2;

      int color = lifts[d].color;
      int a;

      // faded outer shell
      for (a=0; a<10; a++)
        //al_draw_rectangle(x1+a, y1+a, x2-a, y2-a, palette_color[color + ((9 - a)*16)], 1 );
        al_draw_rounded_rectangle(x1+a, y1+a, x2-a, y2-a, 4, 4, palette_color[color + ((9 - a)*16)], 2 );

      // solid core
      al_draw_filled_rectangle(x1+a, y1+a, x2-a, y2-a, palette_color[color] );


      if ((lifts[d].width == 1) && (lifts[d].height > 1)) // rotate lift name for vertical lifts
         rtextout_centre(level_buffer, lifts[d].lift_name, ((x1+x2)/2), ((y1+y2)/2), color+160, 1, 64, 1);
      else
        al_draw_text(font, palette_color[color+160], (x1+x2)/2, (y1+y2)/2 - 3,
                     ALLEGRO_ALIGN_CENTRE, lifts[d].lift_name);

      // show if player is riding this lift
      for (int p=0; p<NUM_PLAYERS; p++)
         if ((players[p].active) && (!players[p].paused))
            if ((players[p].player_ride) && (d == players[p].player_ride - 32)) // if player riding lift
            {
               int pc = players[p].color;
               if (pc == color) pc = 127;
               al_draw_rounded_rectangle(x1, y1, x2, y2, 4, 4, palette_color[pc], 2);
            }

      switch (lifts[d].limit_type) // limit type
      {
         case 5: // timer wait
            if (lifts[d].limit_counter > 0)
               al_draw_textf(font, palette_color[color+64], (x1 + x2)/2 + 2, lifts[d].y1 - 8,
                                        ALLEGRO_ALIGN_CENTRE, "%d", lifts[d].limit_counter);
         break;
         case 6: // prox wait
         {
            int pd = lifts[d].limit_counter; // prox dist
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

   lifts[d].limit_type = 7;   // move step countdown limit type 7

   // get the integer x and y distances between current pos and next step
   int xln = lifts[d].x1 - lift_steps[d][step].x;
   int yln = lifts[d].y1 - lift_steps[d][step].y;
   if ((xln == 0) && (yln == 0)) // no move needed
   {
      lifts[d].limit_counter = 1;
      lifts[d].fxinc = al_itofix(0);
      lifts[d].fyinc = al_itofix(0);
   }
   else
   {
      al_fixed xlen = al_itofix(xln);   // get the x distance
      al_fixed ylen = al_itofix(yln);   // get the y distance

      al_fixed hy_dist = al_fixhypot(xlen, ylen);     // hypotenuse distance
      al_fixed speed = al_itofix(lift_steps[d][step].val) / 10;  // speed is stored scaled by 10

      al_fixed scaler = al_fixdiv(hy_dist, speed);     // get scaler
      al_fixed xinc = al_fixdiv(xlen, scaler);         // calc xinc
      al_fixed yinc = al_fixdiv(ylen, scaler);         // calc yinc

      if (abs(xlen) > abs(ylen))  // xlen is longer so use this to set limit counter
         lifts[d].limit_counter = al_fixtoi( abs( al_fixdiv(xlen, xinc))); // steps = distance / increment

      if (abs(xlen) <= abs(ylen)) // ylen is longer or they are equal
         lifts[d].limit_counter = al_fixtoi( abs( al_fixdiv(ylen, yinc))); // steps = distance / increment

      lifts[d].fxinc = -xinc;
      lifts[d].fyinc = -yinc;
   }
}

void move_lifts(int ignore_prox)
{
   for (int d=0; d<num_lifts; d++)
   {
      lifts[d].fx += lifts[d].fxinc;        // xinc
      lifts[d].fy += lifts[d].fyinc;        // yinc
      lifts[d].x1 = al_fixtoi(lifts[d].fx);    // put as int in x1
      lifts[d].y1 = al_fixtoi(lifts[d].fy);    // put as int in y1
      lifts[d].x2 = lifts[d].x1 + (lifts[d].width *20)-1;  // width
      lifts[d].y2 = lifts[d].y1 + (lifts[d].height*20)-1;  // height

      // limits
      int next_step = 0;
      switch (lifts[d].limit_type) // limit type
      {
         case 5: // timer wait
            if (--lifts[d].limit_counter < 0) next_step = 1;

         break;
         case 7: // step count
            if (--lifts[d].limit_counter < 0)
            {
               next_step = 1;
               //make sure lift is exactly where it should be at the end of the move...


               int step = lifts[d].current_step;
               lifts[d].x1 = lift_steps[d][step].x;
               lifts[d].y1 = lift_steps[d][step].y;
               lifts[d].x2 = lifts[d].x1 + lifts[d].width*20;
               lifts[d].y2 = lifts[d].y1 + lifts[d].height*20;
               lifts[d].fx = al_itofix(lifts[d].x1);
               lifts[d].fy = al_itofix(lifts[d].y1);

            }

         break;
         case 6: // prox wait
            if (ignore_prox) next_step = 1;
            else
            {

               int pd = lifts[d].limit_counter; // prox dist
               int bx1 = lifts[d].x1 - pd - 10;
               int by1 = lifts[d].y1 - pd - 10;
               int bx2 = lifts[d].x2 + pd - 10;
               int by2 = lifts[d].y2 + pd - 10;

               for (int p=0; p<NUM_PLAYERS; p++)
                  if (players[p].active)
                     if ((players[p].PX > al_itofix(bx1)) && (players[p].PX < al_itofix(bx2)))
                        if ((players[p].PY > al_itofix(by1)) && (players[p].PY < al_itofix(by2))) next_step = 1;
            }
         break;

      }
      if (next_step)
      {
         if (++lifts[d].current_step > lifts[d].num_steps - 1)    // increment step
               lifts[d].current_step = lifts[d].num_steps -1;     // bounds check (for lifts with no loop to zero)

         int step = lifts[d].current_step;
         next_step = 0;
         switch (lift_steps[d][step].type)
         {
            case 1: // move
               set_lift_xyinc(d, step);
            break;
            case 2: // wait time
               lifts[d].limit_type = 5; // wait time
               lifts[d].limit_counter = lift_steps[d][step].val; // limit
               lifts[d].fxinc= 0; // no xinc
               lifts[d].fyinc= 0; // no yinc
            break;
            case 3: // wait prox
               lifts[d].limit_type = 6; // wait prox
               lifts[d].limit_counter = lift_steps[d][step].val; // limit
               lifts[d].fxinc= 0; // no xinc
               lifts[d].fyinc= 0; // no yinc
            break;
            case 4: // move to step 0
               step = lifts[d].current_step = 0; // set step 0
               set_lift_xyinc(d, step);
            break;
         }
      } // end of next step
   } // end of lift iterate
}







