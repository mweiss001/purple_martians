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

void set_lift_to_step(int lift, int step)
{
   // searches back from passed step until a move step is found or step 0 is reached
   // sets coordinates in lift from that step
   // then sets the current step in lift to one previous than that (or 0 if no prev)
   // set the type and val to immediately advance to the next step when run

   while ((lift_steps[lift][step].type != 1) && (step > 0) ) step--;

   int c = step-1;
   if (c < 0) c = 0;
   lifts[lift].current_step = c;   // initial step
   lifts[lift].limit_type = 5;     // type wait for time
   lifts[lift].limit_counter = 0;  // 0 = no wait! immediate next mode

   // get pos and size from step
   lifts[lift].x1     = lift_steps[lift][c].x;
   lifts[lift].y1     = lift_steps[lift][c].y;
   lifts[lift].width  = lift_steps[lift][c].w;
   lifts[lift].height = lift_steps[lift][c].h;
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

void draw_lift_lines()
{

   for (int x=0; x<num_lifts; x++)  // cycle lifts
   {
      int col = lifts[x].color+128;
      int sx = lift_steps[x][0].x + lifts[x].width / 2;
      int sy = lift_steps[x][0].y + lifts[x].height / 2;
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
               lifts[x].x1 = lift_steps[x][y].x; // actually do the move
               lifts[x].y1 = lift_steps[x][y].y;

               nx = lifts[x].x1 + lifts[x].width / 2; // find center
               ny = lifts[x].y1 + lifts[x].height / 2;

               al_draw_line( px, py, nx, ny, palette_color[col], 1);
               for (int c=3; c>=0; c--)
                  al_draw_filled_circle(nx, ny, c, palette_color[(col - 96) + c*48]);
               px = nx;
               py = ny;
            }
            if (lift_steps[x][y].type == 6) // look for resize step
            {
               lifts[x].width  = lift_steps[x][y].w; // actually change width
               lifts[x].height = lift_steps[x][y].h;

               nx = lifts[x].x1 + lifts[x].width / 2; // find center
               ny = lifts[x].y1 + lifts[x].height / 2;

               al_draw_line( px, py, nx, ny, palette_color[col], 1);
               for (int c=3; c>=0; c--)
                  al_draw_filled_circle(nx, ny, c, palette_color[(col - 96) + c*48]);
               px = nx;
               py = ny;
               al_draw_line(sx, sy, nx, ny, palette_color[col], 1);
            }
         }
         al_draw_line(sx, sy, nx, ny, palette_color[col], 1); // draw line from last to first
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
        al_draw_rounded_rectangle(x1+a, y1+a, x2-a, y2-a, 4, 4, palette_color[color + ((9 - a)*16)], 2 );

      // solid core
      al_draw_filled_rectangle(x1+a, y1+a, x2-a, y2-a, palette_color[color] );


//      if ((lifts[d].width == 1) && (lifts[d].height > 1)) // rotate lift name for vertical lifts
//         rtextout_centre(level_buffer, lifts[d].lift_name, ((x1+x2)/2), ((y1+y2)/2), color+160, 1, 64, 1);
//      else




      al_draw_text(font, palette_color[color+160], (x1+x2)/2, (y1+y2)/2 - 3, ALLEGRO_ALIGN_CENTRE, lifts[d].lift_name);

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
               al_draw_textf(font, palette_color[color+64], (x1 + x2)/2 + 2, lifts[d].y1 - 8, ALLEGRO_ALIGN_CENTRE, "%d", lifts[d].limit_counter);
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
      al_fixed xlen = al_itofix(xln);                           // x distance
      al_fixed ylen = al_itofix(yln);                           // y distance
      al_fixed hy_dist = al_fixhypot(xlen, ylen);               // hypotenuse distance
      al_fixed speed = al_itofix(lift_steps[d][step].val) / 10; // speed is stored scaled by 10

      al_fixed move_time = al_fixdiv(hy_dist, speed);           // get move time
      lifts[d].limit_counter = al_fixtoi(move_time);
      lifts[d].limit_type = 7;

      al_fixed xinc = al_fixdiv(xlen, move_time);         // calc xinc
      al_fixed yinc = al_fixdiv(ylen, move_time);         // calc yinc
      lifts[d].fxinc = -xinc;                             // do initial move
      lifts[d].fyinc = -yinc;
   }
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


int is_player_riding_lift(int l)
{
   for (int p=0; p<NUM_PLAYERS; p++)
      if ((players[p].active) && (!players[p].paused))
         if ((players[p].player_ride) && (l == players[p].player_ride - 32)) return 1; // player is riding this lift
   return 0;
}


void set_lift_whinc(int l, int step)
{
   //  used when running a resize step
   //  sets winc, hinc and num of frames for limit mode 8 (resize)

   // set fw, fh to current

   lifts[l].fw = al_itofix(lifts[l].width);
   lifts[l].fh = al_itofix(lifts[l].height);


   int time = lift_steps[l][step].val;
   lifts[l].limit_counter = time;
   lifts[l].limit_type = 8;

   // get the difference between the new and old sizes
   al_fixed wdif = al_itofix(lift_steps[l][step].w - lifts[l].width);
   al_fixed hdif = al_itofix(lift_steps[l][step].h - lifts[l].height);

   lifts[l].fwinc = wdif / time; // inc = dif / time
   lifts[l].fhinc = hdif / time; // inc = dif / time

   printf("time:%d wdif:%f hdif:%f fwinc:%f fhinc:%f \n", time, al_fixtof(wdif), al_fixtof(hdif), al_fixtof(lifts[l].fwinc), al_fixtof(lifts[l].fhinc));



}



void move_lifts(int ignore_prox)
{
   for (int l=0; l<num_lifts; l++)
   {
      int next_step = 0;



      // limits ----------------------------------------------------------------------
      switch (lifts[l].limit_type) // limit type
      {
         case 5: // timer wait
            if (--lifts[l].limit_counter < 0) next_step = 1;
         break;
         case 7: // step count for move
            lifts[l].fx += lifts[l].fxinc;           // xinc
            lifts[l].fy += lifts[l].fyinc;           // yinc
            lifts[l].x1 = al_fixtoi(lifts[l].fx);    // put as int in x1
            lifts[l].y1 = al_fixtoi(lifts[l].fy);    // put as int in y1
            lifts[l].x2 = lifts[l].x1 + (lifts[l].width)-1;  // width
            lifts[l].y2 = lifts[l].y1 + (lifts[l].height)-1;  // height

            if (--lifts[l].limit_counter < 0)
            {
               next_step = 1;

               //make sure lift is exactly where it should be at the end of the move...
               int step = lifts[l].current_step;
               lifts[l].x1 = lift_steps[l][step].x;
               lifts[l].y1 = lift_steps[l][step].y;
               lifts[l].x2 = lifts[l].x1 + lifts[l].width;
               lifts[l].y2 = lifts[l].y1 + lifts[l].height;
               lifts[l].fx = al_itofix(lifts[l].x1);
               lifts[l].fy = al_itofix(lifts[l].y1);

               // zero incs when done
               lifts[l].fxinc = al_itofix(0);
               lifts[l].fyinc = al_itofix(0);
            }
         break;
         case 6: // prox wait
            if (ignore_prox) next_step = 1;
            else if (lift_check_prox(l, lifts[l].limit_counter)) next_step = 1;
         break;


         case 8: // step count for resize
            lifts[l].fw += lifts[l].fwinc;           // width inc
            lifts[l].fh += lifts[l].fhinc;           // height inc

            lifts[l].width  = al_fixtoi(lifts[l].fw); // put as int in width
            lifts[l].height = al_fixtoi(lifts[l].fh); // put as int in height

            lifts[l].x2 = lifts[l].x1 + (lifts[l].width)-1;  // get new x2 from width
            lifts[l].y2 = lifts[l].y1 + (lifts[l].height)-1;  // get new y2 from height

            if (--lifts[l].limit_counter < 0)
            {
               next_step = 1;

               // make sure lift is exactly the right size at the end of the move...
               int step = lifts[l].current_step;
               lifts[l].width  = lift_steps[l][step].w;
               lifts[l].height = lift_steps[l][step].h;

               // zero incs when done
               lifts[l].fwinc = al_itofix(0);
               lifts[l].fhinc = al_itofix(0);
            }
         break;

      }

      // modes----------------------------------------------------------------------
      if (lifts[l].mode == 2) // prox reset
      {
         if (is_player_riding_lift(l)) lifts[l].val1 = 80;
         else
         {
            if (--lifts[l].val1 < 0)
            {
               lifts[l].val1 = 80;
               set_lift_to_step(l, 0);
            }
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
            case 6:
               set_lift_whinc(l, step);
            break;

            case 1: // move
               set_lift_xyinc(l, step);
            break;
            case 2: // wait time
               lifts[l].limit_type = 5; // wait time
               lifts[l].limit_counter = lift_steps[l][step].val; // limit
               lifts[l].fxinc= 0; // no xinc
               lifts[l].fyinc= 0; // no yinc
            break;
            case 3: // wait prox
               lifts[l].limit_type = 6; // wait prox
               lifts[l].limit_counter = lift_steps[l][step].val; // limit
               lifts[l].fxinc= 0; // no xinc
               lifts[l].fyinc= 0; // no yinc
            break;
            case 4: // move to step 0

               if (lifts[l].mode == 0)
               {
                  step = lifts[l].current_step = 0; // set step 0
                  set_lift_xyinc(l, step);
               }

               if (lifts[l].mode == 1) // immediately warp to step 0
               {
                  set_lift_to_step(l, 0);
               }

            break;
         }
      } // end of next step
   } // end of lift iterate
}








































