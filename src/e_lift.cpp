// e_lift.cpp

#include "pm.h"



void lift_step_set_size_from_previous_move_step(int lift, int step)
{
   int pms = lift_find_previous_move_step(lift, step); // searches back from passed step until a move step is found
   if (pms != -1)
   {
      lift_steps[lift][step].w = lift_steps[lift][pms].w;
      lift_steps[lift][step].h = lift_steps[lift][pms].h;
   }
}

int lift_find_previous_move_step(int lift, int step) // searches back from passed step until a move step is found
{
   if (step == 0) step = 19; // if started at step 0, start searching back from end to get last move step
   while (step)
   {
      step--;
      if ((lift_steps[lift][step].type & 31) == 1) return step;
   }
   printf("could not find move step!!\n");
   return -1; // bad !!!
}

al_fixed lift_get_distance_to_previous_move_step(int lift, int step)
{
   al_fixed m1 = al_itofix(-1);
   if ((lift_steps[lift][step].type & 31) == 1)
   {
      int ls = lift_find_previous_move_step(lift, step);
      if (ls == -1) return m1;
      else
      {
         al_fixed x1 = al_itofix(lift_steps[lift][step].x);
         al_fixed y1 = al_itofix(lift_steps[lift][step].y);
         al_fixed x2 = al_itofix(lift_steps[lift][ls].x);
         al_fixed y2 = al_itofix(lift_steps[lift][ls].y);
         return al_fixhypot(x2-x1, y2-y1); // hypotenuse distance
      }
   }
   return m1; // error! not a move step
}


void show_all_lifts(void)
{
   int text_pos = 0;
   al_set_target_backbuffer(display);
   al_clear_to_color(al_map_rgb(0,0,0));
   al_draw_textf(font, palette_color[13], 10, text_pos*8, 0, "number of lifts:%d", num_lifts);
   text_pos++;
   for (int l=0; l<num_lifts; l++) // iterate lifts
   {
      int x1 = lifts[l].x1;
      int y1 = lifts[l].y1;
      int x2 = lifts[l].x2;
      int y2 = lifts[l].y2;
      int w  = lifts[l].width;
      int h  = lifts[l].height;

      int v1 = lifts[l].val1;
      int v2 = lifts[l].val2;


      int col  = lifts[l].color;
      sprintf(msg,"lift:%-2d  1:%-4d y:%-4d w:%-4d h:%-4d x2:%-4d y2:%-4d v1:%d v2:%d col:%-2d ns:%-2d  name:%s  ", l, x1, y1, w, h, x2, y2, v1, v2, col, lifts[l].num_steps, lifts[l].lift_name);
      al_draw_text(font, palette_color[10], 10, text_pos*8, 0, msg);
      text_pos++;

      for (int s=0; s<lifts[l].num_steps; s++) // iterate steps
      {
         int x    = lift_steps[l][s].x;
         int y    = lift_steps[l][s].y;
         int w    = lift_steps[l][s].w;
         int h    = lift_steps[l][s].h;
         int val  = lift_steps[l][s].val;
         int type = lift_steps[l][s].type & 31;
         int step_color = -1;
         if (type == 1)
         {
             step_color = lift_steps[l][s].type >> 28;
             step_color &=15 ;
         }


         int color = 9;
         char typemsg[10];
         if ((type >0) && (type < 5))
         {
            sprintf(typemsg,"%s", lift_step_type_name[type] );
            if (type == 1) color = 9; // green for move
            if (type == 2) color = 6; // tan for wait
            if (type == 3) color = 6; // tan for prox
            if (type == 4) color = 5; // green for loop
         }
         else
         {
            sprintf(typemsg,"bad step");
            color = 14;
         }

//         sprintf(msg," step:%-2d x:%-4d y:%-4d val:%-4d type:%d (%s)", s, x, y, val, type, typemsg );

         int ls = lift_find_previous_move_step(l, s);

         al_fixed lsd = lift_get_distance_to_previous_move_step(l, s);


//         sprintf(msg," step:%-2d x:%-4d y:%-4d w:%-4d h:%-4d val:%-4d type:%d (%s), ls:%d lsd:%d step_color:%d", s, x, y, w, h, val, type, typemsg, ls, al_fixtoi(lsd), step_color);
         sprintf(msg," step:%-2d x:%-4d y:%-4d w:%-4d h:%-4d val:%-4d type:%d (%s) step_color:%d", s, x, y, w, h, val, type, typemsg, step_color);
         al_draw_text(font, palette_color[color], 10, text_pos*8, 0, msg);
         text_pos++;
      }
   }
   al_flip_display();
   tsw(); // wait for keypress
}

void erase_lift(int lift)
{
   for (int l=lift; l<num_lifts; l++) // slide down to close hole in array
   {
      lifts[l] = lifts[l+1];
      for (int s=0; s<40; s++)
         lift_steps[l][s] = lift_steps[l+1][s];
   }
   clear_lift(num_lifts);                                  // clear last lift
   for (int s=0; s<40; s++) clear_lift_step(num_lifts, s); // clear last lift's steps
   num_lifts--;                                            // one less lift
}

void delete_lift_step(int l, int step)
{
   for (int x=step; x<lifts[l].num_steps-1; x++)   // slide all down
   {
      lift_steps[l][x].x    = lift_steps[l][x+1].x ;
      lift_steps[l][x].y    = lift_steps[l][x+1].y ;
      lift_steps[l][x].w    = lift_steps[l][x+1].w ;
      lift_steps[l][x].h    = lift_steps[l][x+1].h ;
      lift_steps[l][x].val  = lift_steps[l][x+1].val ;
      lift_steps[l][x].type = lift_steps[l][x+1].type ;
   }
   lifts[l].num_steps--;
}

void lift_setup(void)
{
   // set all lifts to step 0
   for (int d=0; d<num_lifts; d++) set_lift_to_step(d, 0);
}



void highlight_current_lift(int l) // mark current lift with crosshairs and rect on map
{
   int x3 = lifts[l].width;
   int y3 = lifts[l].height;
   int x1 = lifts[l].x1 + 4;
   int y1 = lifts[l].y1 + 4;
   int x2 = x1 + x3-4;
   int y2 = y1 + y3-4;

   al_set_clipping_rectangle(1, 1, display_transform_double*db*100-2, display_transform_double*db*100-2);
   al_draw_line(((x1+x2)/2)*db/20, 1, ((x1+x2)/2)*db/20, db*100-2, palette_color[10], 1);
   al_draw_line(1, ((y1+y2)/2)*db/20, db*100-2, ((y1+y2)/2)*db/20, palette_color[10], 1);
   al_draw_rectangle((x1*db/20)-1, (y1*db/20)-1, (x2*db/20)+1, (y2*db/20)+1, palette_color[15], 1);
   al_reset_clipping_rectangle();
}

int create_lift(void)
{
   int step = 0;
   if (num_lifts < NUM_LIFTS)
   {
      num_lifts++;
      int lift = num_lifts-1;

      sprintf(msg, "new lift %d", lift);
      construct_lift(lift, msg, 120, 20, 10, 1);
      construct_lift_step(lift, step, 0, 0, 80, 20, 20, 1);

      if (getxy("Set initial position", 4, lift, step) == 1)
      {
         // set step 0 and main x y
         lifts[lift].x1 = lift_steps[lift][step].x = get100_x * 20;
         lifts[lift].y1 = lift_steps[lift][step].y = get100_y * 20;

         // set size
         lifts[lift].x2 = lifts[lift].x1 + (lifts[lift].width)-1;
         lifts[lift].y2 = lifts[lift].y1 + (lifts[lift].height)-1;

         step++;
         construct_lift_step(lift, step, 0, 0, 0, 0, 0, 4); // type 4 - loop to step zero
         lifts[lift].num_steps++; // add one to steps

         set_lift_to_step(lift, 0); // set step 0 for lift
         insert_steps_until_quit(lift, step);
         return 1;
      }
      else
      {
         erase_lift(lift);
         return 0;
      }
   }
   else
   {
      al_show_native_message_box(display, "Error", "Error creating lift", "40 lifts is the maximum", NULL, ALLEGRO_MESSAGEBOX_ERROR);
      return 0;
   }
}

void move_lift_step(int lift, int step)
{
   if ((lift_steps[lift][step].type & 31) == 1) // only if type = move
   {
      int nx = ((lift_steps[lift][step].x + lifts[lift].width  / 2) *db)/20;
      int ny = ((lift_steps[lift][step].y + lifts[lift].height / 2) *db)/20;
      al_set_mouse_xy(display, nx*display_transform_double, ny*display_transform_double);
      getxy("Set new location", 4,  lift, step);
   }
}

int get_new_lift_step(int lift, int step)
{
   // step is a new blank step already created for us
   // if we return 99 the step will be erased

   // position the menu on top of the step we are inserting before
   int sty = 53 + (step + 9) * bts;
   if (sty > SCREEN_H-60) sty = SCREEN_H-60;

   int num_of_step_types = 5;
   int sth = (num_of_step_types * 12) + 17;
   int sty2 = sty + sth;

   int fc = 14; // frame color
   int tc = 15; // text color

   al_draw_filled_rectangle(txc-98, sty-8, txc+96, sty2, palette_color[fc+192]); // erase to background color
   al_draw_rectangle       (txc-98, sty-8, txc+96, sty2, palette_color[fc], 1); // frame

   al_set_mouse_xy(display, txc * display_transform_double, (sty+24) * display_transform_double); // position the mouse

   int quit = 0;
   while (!quit)
   {
      al_rest(0.02);
      al_flip_display();
      proc_controllers();
      if ((mouse_b2) || (key[ALLEGRO_KEY_ESCAPE])) quit = 99;

      al_draw_textf(font, palette_color[fc], txc, sty-6, ALLEGRO_ALIGN_CENTER, "Insert New Step %d", step);

      al_draw_text(font, palette_color[tc], txc, sty+5, ALLEGRO_ALIGN_CENTER, "Select Step Type");
      al_draw_rectangle(txc-98, sty+3, txc+96, sty+14, palette_color[fc], 1); // frame


      int c1 = 15+96; // regular button color
      int c2 = 15; // highlighted button color
      int stys = sty+3;

      int jh = 1;
      if (draw_and_process_button(txc, stys+(jh*12), "Move", c1, c2, 1))
      {
         quit = construct_lift_step(lift, step, 0, 0, 80, 20, 20, 1);
         lift_step_set_size_from_previous_move_step(lift, step);
         if (getxy("Set lift position", 4, lift, step) == 1)
         {
            lift_steps[lift][step].x = get100_x * 20;
            lift_steps[lift][step].y = get100_y * 20;
         }
         else quit = 99;
      }

      jh++;
      if (draw_and_process_button(txc, stys+(jh*12), "Wait For Time", c1, c2, 1)) quit = construct_lift_step(lift, step, 0, 0, 0, 0, 100, 2);
      jh++;
      if (draw_and_process_button(txc, stys+(jh*12), "Wait For Prox", c1, c2, 1)) quit = construct_lift_step(lift, step, 0, 0, 0, 0, 80, 3);
      jh++;
      if (draw_and_process_button(txc, stys+(jh*12), "Wait For Trig", c1, c2, 1)) quit = construct_lift_step(lift, step, 0, 0, 0, 0,  0, 5);
      jh++;
      if (draw_and_process_button(txc, stys+(jh*12), "Done", c1, c2, 1)) quit = 99;

   } // end of while (!quit)
   return quit;
}


int insert_lift_step(int lift, int step) // inserts a step in 'lift' before 'step'
{
   int ret = 0;
   redraw_lift_viewer(lift, step);

   // increment the number of steps
   if (++lifts[lift].num_steps > 40)
   {
      lifts[lift].num_steps--;
      ret = 0;
      al_show_native_message_box(display, "Error", "Error creating lift step",
         "40 steps is the maximum", NULL, ALLEGRO_MESSAGEBOX_ERROR);
   }
   else
   {
      // slide steps down to make room for new step
      for (int x=lifts[lift].num_steps-2; x>=step; x--)
      {
         lift_steps[lift][x+1].x    = lift_steps[lift][x].x;
         lift_steps[lift][x+1].y    = lift_steps[lift][x].y;
         lift_steps[lift][x+1].w    = lift_steps[lift][x].w;
         lift_steps[lift][x+1].h    = lift_steps[lift][x].h;
         lift_steps[lift][x+1].val  = lift_steps[lift][x].val;
         lift_steps[lift][x+1].type = lift_steps[lift][x].type;
      }

      // do this after creating a new lift step so stuff lines up...
      set_bts(lift);
      int step_ty = 46 + 7 * bts;
      draw_steps(step_ty, lift, step, step);     // show lift steps

      if (get_new_lift_step(lift, step) == 99) // cancelled
      {
         delete_lift_step(lift, step);
         ret = 0;
      }
      else ret = 1;
      redraw_lift_viewer(lift, step);
   }
   return ret;
}

void insert_steps_until_quit(int lift, int step)
{
   while (insert_lift_step(lift, step)) step++;
}

void step_popup_menu(int lift, int step)
{
   int smx, smy;
   if ((mouse_x < db*100) && (mouse_x < db*100)) // called from map
   {
      smx = mouse_x;
      int lift_ypos = lifts[lift].y2;
      smy = lift_ypos*db/20 + 27;
   }
   else // called from step buttons
   {
      smx = txc;
      smy = 86 + (step + 9) * bts;
   }

   if (smx > SCREEN_W-100) smx = SCREEN_W-100;
   if (smy > SCREEN_H-100) smy = SCREEN_H-100;
   if (smx < 100) smx = 100;
   if (smy < 30) smy = 30;

   al_set_mouse_xy(display, smx * display_transform_double, smy * display_transform_double);
   proc_controllers(); // to deal with mouse warp

   // full menu
   sprintf(global_string[6][0],"Lift:%d Step:%d", lift+1, step);
   sprintf(global_string[6][1],"---------------");
   sprintf(global_string[6][2],"Cancel");
   sprintf(global_string[6][3],"Move Step %d", step);
   sprintf(global_string[6][4],"Delete Step %d", step);
   sprintf(global_string[6][5],"Insert Steps");
   sprintf(global_string[6][6],"Size=Prev Step");
   sprintf(global_string[6][7],"end");


   // special case for first step (don't allow delete or insert, only move )
   if (step == 0)
   {
      sprintf(global_string[6][3],"Move Step %d", step);
      sprintf(global_string[6][4],"end");
      if (pmenu(6, 13) == 3) move_lift_step(lift, step);
   }
   // special case for last step (don't allow move or delete, only insert)
   else if (step == lifts[lift].num_steps-1)
   {
      strcpy (global_string[6][3],"Insert Steps");
      sprintf(global_string[6][4],"end");
      if (pmenu(6, 13) == 3) insert_steps_until_quit(lift, step);
   }
   else // regular step (not first or last)
   {
      if ((lift_steps[lift][step].type & 31) == 1) // only allow move for step type 1
      {
         sprintf(global_string[6][3],"Move Step %d", step);
         sprintf(global_string[6][4],"Delete Step %d", step);
         sprintf(global_string[6][5],"Insert Steps");
         sprintf(global_string[6][6],"Size=Prev Step");
         sprintf(global_string[6][7],"end");
         switch (pmenu(6, 13))
         {
            case 3: move_lift_step(lift, step); break;
            case 4: delete_lift_step(lift, step); break;
            case 5: insert_steps_until_quit(lift, step); break;
            case 6: lift_step_set_size_from_previous_move_step(lift, step); break;
         }
      }
      else
      {
         sprintf(global_string[6][3],"Delete Step %d", step);
         sprintf(global_string[6][4],"Insert Steps");
         sprintf(global_string[6][5],"end");
         switch (pmenu(6, 13))
         {
            case 3: delete_lift_step(lift, step); break;
            case 4: insert_steps_until_quit(lift, step); break;
         }
      }
   }
}



void set_bts(int lift)
{                                       // get button y size
   int ns = lifts[lift].num_steps + 10; // number of steps for this lift
   int sp = SCREEN_H - 46;              // how much vertical screen space
   bts = sp/ns;                         // adjust button size so they all will fit
   if (bts > 16) bts = 16;              // max button size
   if (bts < 8) bts = 8;                // min button size
}



void redraw_lift_viewer(int lift, int step)
{
   set_lift_to_step(lift, step);       // set current step in current lift
   draw_big(1);                        // update the map bitmap
   title_obj(4, lift, 0, 0, 15);       // show title and map on screen
   highlight_current_lift(lift);       // crosshairs and rect on current lift
   set_bts(lift);
   int step_ty = 46 + 8 * bts;
   draw_steps(step_ty, lift, step, step); // show lift steps
}

int draw_current_step_buttons(int y, int l, int s)
{
   int fs = 14; // frame_size
   int a = 0;
   int x1 = SCREEN_W-(SCREEN_W-(db*100))+1;
   int x2 = SCREEN_W-1;
   int xa = x1+fs;
   int xb = x2-fs;
   int ya = y+fs;
   int c1 = 9;

   sprintf(msg, "Current Step [%d] - undefined", s);
   switch (lift_steps[l][s].type & 31)
   {
      case 1: // move and resize
         mdw_slider(xa, ya+a*bts, xb, ya+(a+1)*bts-2, 550, s, l, 0, 0, c1, 15, 15, 1,0,0,0); a++; // lift step resize speed
         mdw_slider(xa, ya+a*bts, xb, ya+(a+1)*bts-2, 551, s, l, 0, 0, c1, 15, 15, 1,0,0,0); a++; // lift step width
         mdw_slider(xa, ya+a*bts, xb, ya+(a+1)*bts-2, 552, s, l, 0, 0, c1, 15, 15, 1,0,0,0); a++; // lift step height

         mdw_colsel(xa, ya+a*bts, xb, ya+(a+1)*bts-2,  8,  l, s, 0, 0, 15, 13, 14, 0,0,0,0); a++; // lift step color




         sprintf(msg, "Current Step [%d] - Move and Resize", s);
      break;
      case 2: // wait time
         mdw_slider(xa, ya+a*bts, xb, ya+(a+1)*bts-2, 553, s, l, 0, 0, c1, 15, 15, 1,0,0,0); a++; // lift step wait time
         sprintf(msg, "Current Step [%d] - Wait For Timer", s);
      break;
      case 3: // wait prox
          mdw_slider(xa, ya+a*bts, xb, ya+(a+1)*bts-2, 554, s, l, 0, 0, c1, 15, 15, 1,0,0,0); a++; // lift step wait player prox distance
          sprintf(msg, "Current Step [%d] - Wait For Player Proximity", s);
      break;
      case 4: // end step
         mdw_button(xa, ya+a*bts, xb, ya+(a+1)*bts-2, 505, s, l, 0, 0, c1, 15, 0,  1,0,0,0); a++; // lift step end step mode
         sprintf(msg, "Current Step [%d] - Ending Step", s);
      break;
      case 5: // wait trigger
         mdw_slider(xa, ya+a*bts, xb, ya+(a+1)*bts-2, 556, s, l, 0, 0, c1, 15, 15, 1,0,0,0); a++; // lift step wait trigger slider
         mdw_button(xa, ya+a*bts, xb, ya+(a+1)*bts-2, 520, s, l, 4, 0, c1, 15, 0,  1,0,0,0); a++; // lift step wait trigger get event
         sprintf(msg, "Current Step [%d] - Wait For Trigger", s);
      break;
   }

   int y1 = y;
   int y2 = y1+a*bts+fs*2-2;
   int ci = 16; //color inc
   for (int q=0; q<fs; q++)
      al_draw_rectangle(x1+q, y1+q, x2-q, y2-q, palette_color[10+(q*ci)], 1);
   al_draw_text(font, palette_color[15], (x1+x2)/2, y1+2, ALLEGRO_ALIGN_CENTER, msg);
   return fs*2+a*bts; // return how much y space was used
}


void draw_step_button(int xa, int xb, int ty, int ty2, int lift, int step, int rc)
{
   int x1 = xa;
   int x2 = xa + 24;  // first column (step number) is fixed size
   int x3 = xa + 160; // second colum (step type)   is fixed size

   int type = lift_steps[lift][step].type & 31;

   if (step == -1) // show row headers
   {
      mdw_button(x1, ty, x2, ty2, 501, -1, 0, 0, 0, rc, 15, 0,  1,0,0,0); // num
      mdw_button(x2, ty, x3, ty2, 502, -1, 0, 0, 0, rc, 15, 0,  1,0,0,0); // type
      mdw_button(x3, ty, xb, ty2, 503,  0, 0, 0, 0, rc, 15, 0,  1,0,0,0); // parameters
   }
   else
   {
      mdw_button(x1, ty, x2, ty2, 501, step, 0, 0, 0, rc, 15, 0,  1,0,0,0); // num
      mdw_button(x2, ty, x3, ty2, 502, type, 0, 0, 0, rc, 15, 0,  1,0,0,0); // type
   }

   switch (type)
   {
      case 1: mdw_slider(x3, ty, xb, ty2, 550, step, lift, 0, 0, rc, 15, 15, 1,0,0,0); break; // speed for move and resize
      case 2: mdw_slider(x3, ty, xb, ty2, 553, step, lift, 0, 0, rc, 15, 15, 1,0,0,0); break; // wait time
      case 3: mdw_slider(x3, ty, xb, ty2, 554, step, lift, 0, 0, rc, 15, 15, 1,0,0,0); break; // wait prox
      case 4: mdw_button(x3, ty, xb, ty2, 505, step, lift, 0, 0, rc, 15, 0,  1,0,0,0); break; // end step
      case 5: mdw_slider(x3, ty, xb, ty2, 556, step, lift, 0, 0, rc, 15, 15, 1,0,0,0); break; // wait trigger
   }
}








int draw_steps(int y, int lift, int current_step, int highlight_step)
{
   int fs = 14; // frame_size
   int a = 0;
   int x1 = SCREEN_W-(SCREEN_W-(db*100))+1;
   int x2 = SCREEN_W-1;
   int xa = x1+fs;
   int xb = x2-fs;
   int ya = y+fs;

   // draw title step
   draw_step_button(xa, xb, ya+(a)*bts, ya+(a+1)*bts-1, lift, -1, 13);
   a++;

   // draw steps
   for (int step=0; step<lifts[lift].num_steps; step++)
      if (lift_steps[lift][step].type & 31) // step is valid
      {
         int color = 13;
         if (step == current_step) color = 10;
         draw_step_button(xa, xb, ya+(a)*bts, ya+(a+1)*bts-1, lift, step, color);
         a++;
      }

   // show outline around highlighted step
   int hs = highlight_step+1;
//   if (hs > -1) al_draw_rectangle(xa-1, ya+hs*bts-1, xb+1, ya+(hs+1)*bts, palette_color[14], 2);
   if (hs > -1) al_draw_rectangle(xa, ya+hs*bts, xb, ya+(hs+1)*bts, palette_color[14], 2);


   int y1 = y;
   int y2 = y1+a*bts+fs*2;
   int ci = 16; //color inc
   for (int q=0; q<fs; q++)
      al_draw_rectangle(x1+q, y1+q, x2-q, y2-q, palette_color[12+32+(q*ci)], 1);
   al_draw_text(font, palette_color[15], (x1+x2)/2, y1+2, ALLEGRO_ALIGN_CENTER, "List of Steps");

   return fs*2 + (lifts[lift].num_steps + 1) * bts; // return how much y space was used
}











int lift_viewer(int lift)
{
   int current_step = 0;
   int step_pointer = 0;
   int ret = 0;
   int quit = 0;



   while (!quit)
   {
      proc_controllers();
      al_flip_display();
      al_clear_to_color(al_map_rgb(0,0,0));

      set_lift_to_step(lift, current_step); // set current step in current lift
      draw_big(1);                          // update the map bitmap
      title_obj(4, lift, 0, 0, 15);         // show title and map on screen
      highlight_current_lift(lift);         // crosshairs and rect on current lift


      // button x positions
      int xa = SCREEN_W-(SCREEN_W-(db*100))+1;
      int xb = SCREEN_W-3;

      // y positions
      // ty is a global...that's where the buttons start

      int ymb = ty; // where the main buttons start
      int ysb = 0; // where the step buttons start
      int ycs = 0; // where the current step buttons are drawn
      int yld = 0; // where the lift is drawn

      set_bts(lift); // bts is used to vary the buttons y size so they will all fit


      int mb = 0; // return value from buttons
      int a = 0;  // keep track of button y spacing
      int x12 = xa + 1 * (xb-xa) / 2; // 1/2          // split into half
      int x13 = xa + 1 * (xb-xa) / 3; // 1/3          // split into thirds
      int x23 = xa + 2 * (xb-xa) / 3; // 2/3
      int x27 = xa + 2 * (xb-xa) / 7; // 2/7          // split into sevenths
      int x57 = xa + 5 * (xb-xa) / 7; // 5/7

      int lc = 6; // lock_color;
      if (Viewer_lock) lc = 7;

      // mdw_slider(xb-30,     ty+a*bts, xb,    ty+a*bts+5, 26,  0, 0, 0, 0, 12, 15, 15, 1,0,0,0); a++;     // bts


      if (mdw_button(xa,    ymb+a*bts, x27-1, ymb+(a+1)*bts-2, 23,  lift, 0, 0, 0,  9, 15, 0, 1,0,0,0)) mb = 22;  // prev
          mdw_button(x27,   ymb+a*bts, x57-1, ymb+(a+1)*bts-2, 56,  lift, 0, 0, 0, lc, 15, 0, 1,0,0,0);           // lock
      if (mdw_button(x57,   ymb+a*bts, xb,    ymb+(a+1)*bts-2, 22,  lift, 0, 0, 0,  9, 15, 0, 1,0,0,0)) mb = 21;  // next
      a++;
      if (mdw_button(xa,    ymb+a*bts, x13-1, ymb+(a+1)*bts-2, 28,  lift, 0, 0, 0, 13, 15, 0, 1,0,0,0)) mb = 18;  // move
      if (mdw_button(x13,   ymb+a*bts, x23-1, ymb+(a+1)*bts-2, 20,  lift, 0, 0, 0, 14, 15, 0, 1,0,0,0)) mb = 19;  // create
      if (mdw_button(x23,   ymb+a*bts, xb,    ymb+(a+1)*bts-2, 21,  lift, 0, 0, 0, 10, 15, 0, 1,0,0,0)) mb = 20;  // delete
      a++;
      if (mdw_button(xa,    ymb+a*bts, x12-1, ymb+(a+1)*bts-2, 25,  lift, 0, 0, 0, 1,  15, 0, 1,0,0,0)) mb = 24;  // viewer help
      if (mdw_button(x12,   ymb+a*bts, xb,    ymb+(a+1)*bts-2, 57,  lift, 0, 4, 0, 1,  15, 0, 1,0,0,0)) mb = 25;  // lift help
      a++;

          mdw_button(xa,    ymb+a*bts, xb,    ymb+(a+1)*bts-2, 500, lift, 0, 0, 0, 13, 15,  0, 1,0,0,0); a++;     // lift mode
          if (lifts[lift].mode == 1)
          {
             mdw_slider(xa,    ymb+a*bts, xb,    ymb+(a+1)*bts-2, 555, lift, 0, 0, 0, 13, 15, 15, 1,0,0,0); a++; // lift mode 1 player ride timer
          }


          mdw_button(xa,    ymb+a*bts, xb,    ymb+(a+1)*bts-2, 510, lift, 0, 0, 0, 13, 15,  0, 1,0,0,0); a++;     // lift draw on/off
          mdw_button(xa,    ymb+a*bts, xb,    ymb+(a+1)*bts-2, 511, lift, 0, 0, 0, 13, 15,  0, 1,0,0,0); a++;     // lift solid player on/off
          mdw_button(xa,    ymb+a*bts, xb,    ymb+(a+1)*bts-2, 512, lift, 0, 0, 0, 13, 15,  0, 1,0,0,0); a++;     // lift solid enemy on/off
          mdw_button(xa,    ymb+a*bts, xb,    ymb+(a+1)*bts-2, 513, lift, 0, 0, 0, 13, 15,  0, 1,0,0,0); a++;     // lift solid item on/off





          mdw_colsel(xa,    ymb+a*bts, xb,    ymb+(a+1)*bts-2,  4,  lift, 0, 0, 0, 15, 13, 14, 0,0,0,0); a++;     // lift color
      if (mdw_button(xa,    ymb+a*bts, xb,    ymb+(a+1)*bts-2, 504, lift, 0, 0, 0,  4, 15,  0, 1,0,0,0)) mb = 26; // lift name
      a+=2;

      while (key[ALLEGRO_KEY_DELETE]) {proc_controllers(); mb=20;}
      while (key[ALLEGRO_KEY_RIGHT])  {proc_controllers(); mb=21;}
      while (key[ALLEGRO_KEY_LEFT])   {proc_controllers(); mb=22;}
      while (key[ALLEGRO_KEY_ESCAPE]) {proc_controllers(); quit = 1;}


      // list of step buttons
      // --------------------------------------------------------------------------------
      ysb = ty + (a*bts); // y pos of step buttons
      step_pointer = -99;
      if ((mouse_x > xa + 10) && (mouse_x < xb - 10))         // mouse on step buttons
      {
         int step0_y_offset = ysb + 14 + bts;
         int step = (mouse_y - step0_y_offset) / bts;           // calculate step that mouse is on
         if ((step >= 0) && (step < lifts[lift].num_steps))   // is step valid
         {
            step_pointer = step;
            if (mouse_b1) current_step = step;
            if (mouse_b2) step_popup_menu(lift, step);        // button pop up menu
         }
      }
      ycs = ysb + draw_steps(ysb, lift, current_step, step_pointer);  // draw has to go after, because it can eat the mouse clicks
      ycs +=bts;



      // draw current step button and get y postion for next item (lift)
      yld = ycs + draw_current_step_buttons(ycs, lift, current_step);
      yld += bts;


      // draw current lift under step list buttons
      if (bts == 16) // only if max button size
      {
         int x1 = xa+10;
         int y1 = yld; // only see in 2 highest screen modes
         int s = current_step;

         // if step is not a move step, find prev that is
         if ((lift_steps[lift][s].type & 31) != 1) s = lift_find_previous_move_step(lift, s);

         // get w h from step
         int w  = lift_steps[lift][s].w;
         int h  = lift_steps[lift][s].h;

         // get x2 and y2 based on x1 y1 and w h
         int x2 = x1 + w;
         int y2 = y1 + h;
         draw_lift(lift, x1, y1, x2, y2);
      }

//      // debug text
//      int dty = SCREEN_H-100;
//      al_draw_textf(font, palette_color[15], xa, dty, 0, "c_step:%d h_step:%d n_step:%d", current_step, step_pointer, lifts[lift].num_steps);




      // button clicked return values
      // ------------------------------------------------------------------------------
      if (mb)
      {
         while (mouse_b1) proc_controllers(); // wait for release
         switch (mb)
         {
            case 19: create_lift(); break;                                // create new
            case 21: if (++lift > num_lifts-1) lift = num_lifts-1; break; // next
            case 22: if (--lift < 0) lift++; break;                       // previous
            case 24: help("Viewer Basics"); break;                        // help viewer
            case 25: help("Lift Viewer"); break;                          // help lifts
            case 20:                                                      // delete
               erase_lift(lift);
               if (--lift < 0) lift = 0;    // set to prev lift or zero
               if (num_lifts < 1) quit = 1; // if erased last lift; exit lift viewer
            break;
            case 26: // new name
            {
               char fst[80];
               strcpy(fst, lifts[lift].lift_name);
               if (edit_lift_name(lift, yld, xa+10, fst)) strcpy(lifts[lift].lift_name, fst);
            }
            break;
            case 18: // run lifts
            {
               while (!key[ALLEGRO_KEY_ESCAPE])
               {
                  proc_controllers();
                  for (int t=0; t<8; t++) move_lifts(1);               // move lifts for 8 frames
                  draw_big(1);                                             // update the map bitmap
                  show_big();                                              // show the map
                  highlight_current_lift(lift);                            // highlight current lift
                  draw_steps(ysb, lift, lifts[lift].current_step, -1); // show lift steps
                  al_flip_display();
               }
               while (key[ALLEGRO_KEY_ESCAPE]) proc_controllers(); // wait for release
               lift_setup(); // reset all lifts to step 0
            }
            break;
         } // end if switch (mb)
      } // end of if (mb)



      // this is the awesome section that lets you move steps on the map just by clicking and dragging
      // ----------------------------------------------------------------------------------------------------
      if ((mouse_x < db*100)  && (mouse_x < db*100) )        // is mouse x on map ?
      {
         int mouse_on_lift = 0;
         int mouse_on_moveable_step = 0;
         for (int x=0; x<num_lifts; x++)  // cycle lifts
            for (int y=0; y<lifts[x].num_steps; y++)  // cycle steps
               if ((lift_steps[x][y].type & 31) == 1) // filter for move step
               {
                  int nx = ((lift_steps[x][y].x + lifts[x].width/2) *db)/20;
                  int ny = ((lift_steps[x][y].y + lifts[x].height/2) *db)/20;
                  int w = lifts[x].width  /2 * db / 20 + 1;
                  int h = lifts[x].height /2 * db / 20 + 1;
                  // is mouse on this step ?
                  if ((mouse_x > nx - w)  && (mouse_x < nx + w) && (mouse_y > ny - h)  && (mouse_y < ny + h))
                  {
                     mouse_on_lift = 1;
                     if ((key[MAP_LOCK_KEY]) || (Viewer_lock)) // locked to current lift
                     {
                        if (lift == x) // are we on current lift?
                        {
                           mouse_on_moveable_step = 1;
                           if (current_step != y) // same lift, different step
                           {
                              current_step = y;
                           }
                        }
                     }
                     else // no lock
                     {
                        mouse_on_moveable_step = 1;
                        if ((lift != x) || (current_step != y)) // no lock, different lift or step
                        {
                           lift = x;
                           current_step = y;
                        }
                     }
                     if (mouse_on_moveable_step)
                     {
                        if (mouse_b2)
                        {
                           step_popup_menu(x, y);
                        }
                        // first time only to snap to middle of lift step we're moving
                        if (mouse_b1) al_set_mouse_xy(display, (nx+w/2) * display_transform_double, (ny+h/2) * display_transform_double);
                        while (mouse_b1)
                        {
                           al_rest(0.01);
                           proc_controllers();
                           nx = mouse_x;
                           ny = mouse_y;

                           int jnx = (nx - w) / db * 20;
                           int jny = (ny - h) / db * 20;

                           if (jnx > 1979) jnx = 1979;
                           if (jny > 1979) jny = 1979;
                           if (jnx < 0) jnx = 0;
                           if (jny < 0) jny = 0;

                           lift_steps[x][y].x = jnx;
                           lift_steps[x][y].y = jny;

                           set_lift_to_step(lift, current_step);   // set current step in current lift

                           al_set_clipping_rectangle(1, 1, display_transform_double*db*100-2, display_transform_double*db*100-2);
                           draw_big(1);
                           show_big();
                           highlight_current_lift(lift);   // crosshairs and rect on current lift
                           al_flip_display();


                        } // end of while mouse b1 pressed
                        al_reset_clipping_rectangle();
                     } // end of mouse on moveable step
                  } // end of mouse on this step
               }  // end of cycle move steps



         // if mouse was not on any lift steps, see if we can switch to another object
         if ((!mouse_on_lift) && (!key[MAP_LOCK_KEY]) && (!Viewer_lock))
         {
            int mx = mouse_x / db;
            int my = mouse_y / db;

            // is mouse on any enemy?
            for (int e=0; e<100; e++)
            {
               int x0 = al_fixtoi(Efi[e][0]);
               int y0 = al_fixtoi(Efi[e][1]);
               if ((Ei[e][0]) && (x0 == mx*20) && (y0 == my*20))
               {
                   quit = 1;
                   ret = e + 1000;
               }
            }
            // is mouse on any item?
            if (!quit) for (int x=0; x<500; x++)
            {
               int x0 = item[x][4];
               int y0 = item[x][5];
               if ((item[x][0]) && (x0 == mx*20) && (y0 == my*20))
               {
                   quit = 1;
                   ret = x + 2000;
               }
            }
         }
      } // end of mouse on map


      if (key[ALLEGRO_KEY_B])
      {
         while (key[ALLEGRO_KEY_B]) proc_controllers();
         if ((key[ALLEGRO_KEY_LSHIFT]) || (key[ALLEGRO_KEY_RSHIFT]))
         {
            printf("save bookmark\n");
            bookmark_level = last_level_loaded;
            bookmark_obj = 4;
            bookmark_num = lift;
            save_config();
         }
         else
         {
            printf("load bookmark\n");
            if (bookmark_level == last_level_loaded)
            {
               if ((bookmark_obj == 2) && (item[bookmark_num]))
               {
                  ret = 2000 + bookmark_num;
                  quit = 1;
               }
               if ((bookmark_obj == 3) && (Ei[bookmark_num]))
               {
                  ret = 1000 + bookmark_num;
                  quit = 1;
               }
               if ((bookmark_obj == 4) && (bookmark_num < num_lifts)) lift = bookmark_num;
            }
         }
      }











   } // end of while !quit
   lift_setup();
   draw_big(1);
   return ret;
}






