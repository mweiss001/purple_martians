// e_lift.cpp

#include "pm.h"

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
      int mode = lifts[l].mode;

      int v1 = lifts[l].val1;
      int v2 = lifts[l].val2;

      int col  = lifts[l].color;
      sprintf(msg,"lift:%-2d  1:%-4d y:%-4d w:%-4d h:%-4d x2:%-4d y2:%-4d mode:%d v1:%d v2:%d col:%-2d ns:%-2d  name:%s  ", l, x1, y1, w, h, x2, y2, mode, v1, v2, col, lifts[l].num_steps, lifts[l].lift_name);
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
         int step_color = (lift_steps[l][s].type >> 28) & 15;

         int color = 9;
         char typemsg[10];
         if ((type >0) && (type < 6))
         {
            sprintf(typemsg,"%s", lift_step_type_name[type] );
            if (type == 1) color = 9; // green for move
            if (type == 2) color = 6; // tan for wait
            if (type == 3) color = 6; // tan for prox
            if (type == 4) color = 5; // green for loop
            if (type == 5) color = 4; // dk red for event trigger
         }
         else
         {
            sprintf(typemsg,"bad step");
            color = 14;
         }
//         sprintf(msg," step:%-2d x:%-4d y:%-4d val:%-4d type:%d (%s)", s, x, y, val, type, typemsg );
//         int ls = lift_find_previous_move_step(l, s);
//         al_fixed lsd = lift_get_distance_to_previous_move_step(l, s);
//         sprintf(msg," step:%-2d x:%-4d y:%-4d w:%-4d h:%-4d val:%-4d type:%d (%s), ls:%d lsd:%d step_color:%d", s, x, y, w, h, val, type, typemsg, ls, al_fixtoi(lsd), step_color);
//         sprintf(msg," step:%-2d x:%-4d y:%-4d w:%-4d h:%-4d val:%-4d type:%d (%s) step_color:%d", s, x, y, w, h, val, type, typemsg, step_color);
         printBits(4, &lift_steps[l][s].type);
         al_draw_textf(font, palette_color[color], 10, text_pos*8, 0, " step:%-2d x:%-4d y:%-4d w:%-4d h:%-4d val:%-4d type:%d (%s) col:%2d b:%s", s, x, y, w, h, val, type, typemsg, step_color, msg);
         text_pos++;
      }
   }
   al_flip_display();
   tsw(); // wait for keypress
}












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

void lift_setup(void) // set all lifts to step 0
{
   for (int l=0; l<num_lifts; l++) set_lift_to_step(l, 0);
}

int create_lift(void)
{
   int step = 0;
   if (num_lifts < NUM_LIFTS)
   {
      num_lifts++;
      int lift = num_lifts-1;

      int initial_width = 80;
      int initial_height = 20;
      int initial_color = 12;
      int initial_type = 1;
      int initial_val = 20;

      int cf = initial_color << 28; // shift 4 bits of color into place
      cf |= PM_LIFT_SOLID_PLAYER;
      cf |= PM_LIFT_SOLID_ENEMY;
      cf |= PM_LIFT_SOLID_ITEM;
      initial_type |= cf; ;     // merge with type

      sprintf(msg, "lift %d", lift);

      construct_lift(lift, msg);
      construct_lift_step(lift, step, initial_type, 0, 0, initial_width, initial_height, initial_val);
      lifts[lift].num_steps++; // add one to steps

      if (getxy("Lift Initial Position", 4, lift, step) == 1)
      {
         step++;
         initial_type = (4 | cf);  // make type 4 step type with same flags as initial
         construct_lift_step(lift, step, initial_type, 0, 0, 0, 0, 0); // type 4 - loop to step zero
         lifts[lift].num_steps++; // add one to steps

         set_lift_to_step(lift, 0); // set step 0 for lift

         cm_redraw_level_editor_background();  // do this twice to get proper window height
         cm_redraw_level_editor_background();


         insert_steps_until_quit(lift, step);

         object_viewerw(4, num_lifts-1);

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
   if ((lift_steps[lift][step].type & 31) == 1) getxy("Step Position", 4,  lift, step); // only if type = move
}

int get_new_lift_step(int lift, int step)
{
   int bts = 16;
   // step is a new blank step already created for us
   // if we return 99 the step will be erased

   // position the menu on top of the step we are inserting before
//   int sty = 53 + (step + 9) * bts;

   int sty = mW[7].y1 + 44 + (step + 10) * bts;

   if (sty > SCREEN_H-60) sty = SCREEN_H-60;

   int num_of_step_types = 5;
   int sth = (num_of_step_types * 12) + 17;
   int sty2 = sty + sth;

   int fc = 14; // frame color
   int tc = 15; // text color

   int xc = (mW[7].x1 + mW[7].x2)/2;

   al_draw_filled_rectangle(xc-98, sty-8, xc+96, sty2, palette_color[fc+192]); // erase to background color
   al_draw_rectangle       (xc-98, sty-8, xc+96, sty2, palette_color[fc], 1); // frame

   al_set_mouse_xy(display, xc * display_transform_double, (sty+24) * display_transform_double); // position the mouse

   int quit = 0;
   while (!quit)
   {
      al_rest(0.02);
      al_flip_display();
      proc_controllers();
      if ((mouse_b2) || (key[ALLEGRO_KEY_ESCAPE])) quit = 99;

      al_draw_textf(font, palette_color[fc], xc, sty-6, ALLEGRO_ALIGN_CENTER, "Insert New Step %d", step);

      al_draw_text(font, palette_color[tc], xc, sty+5, ALLEGRO_ALIGN_CENTER, "Select Step Type");
      al_draw_rectangle(xc-98, sty+3, xc+96, sty+14, palette_color[fc], 1); // frame


      int c1 = 15+96; // regular button color
      int c2 = 15; // highlighted button color
      int stys = sty+3;

      int jh = 1;
      if (draw_and_process_button(xc, stys+(jh*12), "Move", c1, c2, 1))
      {
         quit = construct_lift_step(lift, step, 1, 0, 0, 0, 0, 20);
         lift_step_set_size_from_previous_move_step(lift, step);
         if (getxy("Step Position", 4, lift, step) != 1) quit = 99;
      }

      jh++;
      if (draw_and_process_button(xc, stys+(jh*12), "Wait For Time", c1, c2, 1)) quit = construct_lift_step(lift, step, 2, 0, 0, 0, 0, 100);
      jh++;
      if (draw_and_process_button(xc, stys+(jh*12), "Wait For Prox", c1, c2, 1)) quit = construct_lift_step(lift, step, 3, 0, 0, 0, 0, 80);
      jh++;
      if (draw_and_process_button(xc, stys+(jh*12), "Wait For Trig", c1, c2, 1)) quit = construct_lift_step(lift, step, 5, 0, 0, 0,  0, 0);
      jh++;
      if (draw_and_process_button(xc, stys+(jh*12), "Done", c1, c2, 1)) quit = 99;

   } // end of while (!quit)
   return quit;
}


int insert_lift_step(int lift, int step) // inserts a step in 'lift' before 'step'
{
   int bts = 16;
   int ret = 0;

   // increment the number of steps
   if (++lifts[lift].num_steps > 40)
   {
      lifts[lift].num_steps--;
      ret = 0;
      al_show_native_message_box(display, "Error", "Error creating lift step", "40 steps is the maximum", NULL, ALLEGRO_MESSAGEBOX_ERROR);
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
      clear_lift_step(lift, step);
      int step_ty = mW[7].y1+ 38 + 7 * bts;
      draw_steps(mW[7].x1+1, mW[7].x2-1, step_ty, lift, step, step);     // show lift steps
      if (get_new_lift_step(lift, step) == 99) // cancelled
      {
         delete_lift_step(lift, step);
         ret = 0;
      }
      else ret = 1;
      cm_redraw_level_editor_background();  // do this twice to get proper window height
      cm_redraw_level_editor_background();

   }
   return ret;
}

void insert_steps_until_quit(int lift, int step)
{
   while (insert_lift_step(lift, step)) step++;
}


void set_all_steps(int l, int s, int what)
{
   if (what == 1) // size
   {
      int w = lift_steps[l][s].w;
      int h = lift_steps[l][s].h;

      for (int si=0; si<lifts[l].num_steps; si++) // iterate steps
      {
         if ((lift_steps[l][si].type & 31) == 1)
         {
            lift_steps[l][si].w = w;
            lift_steps[l][si].h = h;
         }
      }
   }
   if (what == 2) // color
   {
      int col = lift_steps[l][s].type & PM_LIFT_COLOR_BITS; // mask off all except color
      for (int si=0; si<lifts[l].num_steps; si++)           // iterate steps
      {
         lift_steps[l][si].type &= ~PM_LIFT_COLOR_BITS;     // clear color bits
         lift_steps[l][si].type |= col;                     // merge
      }
   }
   if (what == 3) // flags
   {
      int flags = lift_steps[l][s].type & PM_LIFT_FLAG_BITS; // mask off all except flags
      for (int si=0; si<lifts[l].num_steps; si++)            // iterate steps
      {
         lift_steps[l][si].type &= ~PM_LIFT_FLAG_BITS;       // clear flag bits
         lift_steps[l][si].type |= flags;                    // merge
      }
   }
}


void step_popup_menu(int lift, int step)
{
   int smx = mouse_x;
   int smy = mouse_y;

   if (smx > SCREEN_W-100) smx = SCREEN_W-100;
   if (smy > SCREEN_H-100) smy = SCREEN_H-100;
   if (smx < 100) smx = 100;
   if (smy < 30) smy = 30;

   al_set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);
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
      sprintf(global_string[6][4],"Apply Size to All");
      sprintf(global_string[6][5],"Apply Color to All");
      sprintf(global_string[6][6],"Apply Flags to All");
      sprintf(global_string[6][7],"end");
         switch (pmenu(6, 13))
         {
            case 3: move_lift_step(lift, step); break;
            case 4: set_all_steps(lift, step, 1); break;
            case 5: set_all_steps(lift, step, 2); break;
            case 6: set_all_steps(lift, step, 3); break;
         }
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
         sprintf(global_string[6][6],"Get Size From Prev");

         sprintf(global_string[6][7],"Apply Size to All");
         sprintf(global_string[6][8],"Apply Color to All");
         sprintf(global_string[6][9],"Apply Flags to All");




         sprintf(global_string[6][10],"end");
         switch (pmenu(6, 13))
         {
            case 3: move_lift_step(lift, step); break;
            case 4: delete_lift_step(lift, step); break;
            case 5: insert_steps_until_quit(lift, step); break;
            case 6: lift_step_set_size_from_previous_move_step(lift, step); break;
            case 7: set_all_steps(lift, step, 1); break;
            case 8: set_all_steps(lift, step, 2); break;
            case 9: set_all_steps(lift, step, 3); break;
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

int draw_current_step_buttons(int x1, int x2, int y, int l, int s)
{
   int bts = 16; // button height
   int fs = 14;  // frame_size
   int xa = x1+fs;
   int xb = x2-fs;
   int ya = y+fs;
   int c1 = 9;

   // step details text header button
   int sd = 10; // color
   switch (lift_steps[l][s].type & 31)
   {
      case 1: // move and resize
         sprintf(msg, "Step:%d - Move and Resize", s);
         mdw_buttont(xa, ya, xb, bts,  1,0,0,0, 0,sd,15,0, 1,0,0,0, msg);
      break;
      case 2: // wait time
         sprintf(msg, "Step:%d - Wait For Timer", s);
         mdw_buttont(xa, ya, xb, bts,  1,0,0,0, 0,sd,15,0, 1,0,0,0, msg);
      break;
      case 3: // wait prox
         sprintf(msg, "Step:%d - Wait For Player Proximity", s);
         mdw_buttont(xa, ya, xb, bts,  1,0,0,0, 0,sd,15,0, 1,0,0,0, msg);
      break;
      case 4: // end step
         sprintf(msg, "Step:%d - Ending Step", s);
         mdw_buttont(xa, ya, xb, bts,  1,0,0,0, 0,sd,15,0, 1,0,0,0, msg);
      break;
      case 5: // wait trigger
         sprintf(msg, "Step:%d - Wait For Trigger", s);
         mdw_buttont(xa, ya, xb, bts,  1,0,0,0, 0,sd,15,0, 1,0,0,0, msg);
      break;
   }

   // default buttons
   int col = (lift_steps[l][s].type >> 28) & 15;
   sprintf(msg, "Step Color:%d", col);
   mdw_buttont(xa, ya, xb, bts, 1,0,0,0,  0, col, 15, 0,   0,0,0,0, msg);

   mdw_colsel(xa, ya, xb, bts, 8,l,s,0,  0,15,13,14,   0,0,1,0); // lift step color

   int dim = 32;
   int c2 = 14;
   int c3 = 15;

   mdw_togglf(xa, ya, xb, bts, 1,0,0,0, 0,0,0,0, 1,0,0,0, lift_steps[l][s].type, PM_LIFT_NO_DRAW,      "Draw Lift",            "Hide Lift",            c3, c3+dim, c2, c2+dim);
   mdw_togglf(xa, ya, xb, bts, 1,0,0,0, 0,0,0,0, 1,0,0,0, lift_steps[l][s].type, PM_LIFT_SOLID_PLAYER, "Solid for Player:OFF", "Solid for Player:ON ", c3+dim, c3, c2+dim, c2);
   mdw_togglf(xa, ya, xb, bts, 1,0,0,0, 0,0,0,0, 1,0,0,0, lift_steps[l][s].type, PM_LIFT_SOLID_ENEMY,  "Solid for Enemy:OFF ", "Solid for Enemy:ON  ", c3+dim, c3, c2+dim, c2);
   mdw_togglf(xa, ya, xb, bts, 1,0,0,0, 0,0,0,0, 1,0,0,0, lift_steps[l][s].type, PM_LIFT_SOLID_ITEM,   "Solid for Item:OFF  ", "Solid for Item:ON   ", c3+dim, c3, c2+dim, c2);
   mdw_togglf(xa, ya, xb, bts, 1,0,0,0, 0,0,0,0, 1,0,0,0, lift_steps[l][s].type, PM_LIFT_HIDE_LINES,   "Draw Lift Lines",      "Hide Lift Lines",      c3, c3+dim, c2, c2+dim);
















   /*



   if (bn == 550) sprintf(smsg, "Speed:%d",         lift_steps[num][type].val);  // lift step move and resize time
   if (bn == 551) sprintf(smsg, "Width:%d",         lift_steps[num][type].w);    // lift step width
   if (bn == 552) sprintf(smsg, "Height:%d",        lift_steps[num][type].h);    // lift step height
   if (bn == 553) sprintf(smsg, "Timer:%-3d",       lift_steps[num][type].val);  // lift step wait timer
   if (bn == 554) sprintf(smsg, "Distance:%-3d",    lift_steps[num][type].val);  // lift step wait player prox distance
   if (bn == 555) sprintf(smsg, "Reset Timer:%-3d", lifts[num].val2);            // lift mode 1 player ride timer
   if (bn == 556) sprintf(smsg, "Trigger:%-2d",     lift_steps[num][type].val);  // lift step wait trigger

      // lifts --------------------------------------
      case 550: sul=1000; sll=1;    sinc=1;   sdx=lift_steps[num][type].val;   break;  // lift step resize speed
      case 551: sul=1600; sll=20;   sinc=1;   sdx=lift_steps[num][type].w;     break;  // lift step width
      case 552: sul=1600; sll=20;   sinc=1;   sdx=lift_steps[num][type].h;     break;  // lift step height
      case 553: sul=2000; sll=1;    sinc=1;   sdx=lift_steps[num][type].val;   break;  // lift step wait time
      case 554: sul=200;  sll=20;   sinc=10;  sdx=lift_steps[num][type].val;   break;  // lift step wait player prox distance
      case 555: sul=2000; sll=1;    sinc=1;   sdx=lifts[num].val2;             break;  // lift mode 1 player ride timer
      case 556: sul=99;   sll=0;    sinc=1;   sdx=lift_steps[num][type].val;   break;  // lift step wait trigger

   */


   // specific buttons
   switch (lift_steps[l][s].type & 31)
   {
      case 1: // move and resize
         mdw_slider2_int(xa, ya, xb, bts,  0,0,0,0,   0,c1,15,15, 1,0,1,0, lift_steps[l][s].val, 1000, 1, 1, "Speed:");
         mdw_slider2_int(xa, ya, xb, bts,  0,0,0,0,   0,c1,15,15, 1,0,1,0, lift_steps[l][s].w,   1600, 20, 1, "Width:");
         mdw_slider2_int(xa, ya, xb, bts,  0,0,0,0,   0,c1,15,15, 1,0,1,0, lift_steps[l][s].h,   1600, 20, 1, "Height:");
      break;
      case 2: // wait time
         mdw_slider2_int(xa, ya, xb, bts,  0,0,0,0,   0,c1,15,15, 1,0,1,0, lift_steps[l][s].val, 2000, 1, 1, "Timer:");
      break;
      case 3: // wait prox
         mdw_slider2_int(xa, ya, xb, bts,  0,0,0,0,   0,c1,15,15, 1,0,1,0, lift_steps[l][s].val, 200, 20, 10, "Distance:");
      break;
      case 4: // end step
         mdw_button(     xa, ya, xb, bts,  505,l,s,0, 0,c1,15,0,  1,0,1,0); // lift step end step mode
      break;
      case 5: // wait trigger
         mdw_slider2_int(xa, ya, xb, bts,  0,0,0,0,   0,c1,15,15, 1,0,1,0, lift_steps[l][s].val, 99, 0, 1, "Trigger:");
         mdw_button(     xa, ya, xb, bts,  520,l,s,4, 0,c1,15,0,  1,0,1,0); ya+=bts; // lift step wait trigger get event
      break;
   }

   // frame and title
   int y1 = y;
   int y2 = ya-bts+(fs*2);
   for (int q=0; q<fs; q++)
      al_draw_rectangle(x1+q, y1+q, x2-q, y2-q, palette_color[10+(q*16)], 1);
   al_draw_text(font, palette_color[15], (x1+x2)/2, y1+2, ALLEGRO_ALIGN_CENTER, "Current Step Details");
   return y2-y1+2; // return how much y space was used
}

void draw_step_button(int xa, int xb, int ty1, int ty2, int l, int s, int rc)
{
   int t = lift_steps[l][s].type & 31;
   int c = (lift_steps[l][s].type >> 28) & 15;
   if (lift_steps[l][s].type & PM_LIFT_NO_DRAW) c = 0;
   int x1 = xa;
   int x2 = x1 + 26; // first column (step number) is fixed size
   int x3 = x2 + 26; // second column (color)   is fixed size
   int x4 = x3 + 42; // third  column (type)   is fixed size
   if (s == -1) // show row headers
   {
      mdw_button(x1, ty1, x2, ty2-ty1, 501, -1, 0, 0, 0, rc, 15, 0,1,0,0,0); // num
      mdw_button(x2, ty1, x3, ty2-ty1, 506, -1, 0, 0, 0, rc, 15, 0,1,0,0,0); // 'C'
      mdw_button(x3, ty1, x4, ty2-ty1, 502, -1, 0, 0, 0, rc, 15, 0,1,0,0,0); // type
      mdw_button(x4, ty1, xb, ty2-ty1, 503, -1, 0, 0, 0, rc, 15, 0,1,0,0,0); // description
   }
   else
   {
      mdw_button(x1, ty1, x2, ty2-ty1, 501, s, 0, 0, 0, rc, 15, 0,1,0,0,0); // num
      mdw_button(x2, ty1, x3, ty2-ty1, 506, c, 0, 0, 0, c,  15, 0,0,0,0,0); // color
      mdw_button(x3, ty1, x4, ty2-ty1, 502, t, 0, 0, 0, rc, 15, 0,1,0,0,0); // type
      mdw_button(x4, ty1, xb, ty2-ty1, 503, t, l, s, 0, rc, 15, 0,1,1,0,0); // description
   }
}

int draw_steps(int x1, int x2, int y, int lift, int current_step, int highlight_step)
{
   int bts = 16;
   int fs = 14; // frame_size
   int a = 0;
   int xa = x1+fs;
   int xb = x2-fs;
   int ya = y+fs;

   // draw title step
   draw_step_button(xa, xb, ya+(a)*bts, ya+(a+1)*bts-1, lift, -1, 13); a++;

   // draw steps
   for (int step=0; step<lifts[lift].num_steps; step++)
   {
      int color = 13;
      if (step == current_step) color = 10;
      draw_step_button (xa, xb, ya+(a)*bts, ya+(a+1)*bts-1, lift, step, color); a++;
   }

   // show outline around highlighted step
   int hs = highlight_step+1;
   if (hs > -1) al_draw_rectangle(xa, ya+hs*bts, xb, ya+(hs+1)*bts, palette_color[14], 2);

   int y1 = y;
   int y2 = y1+a*bts+fs*2;
   int ci = 16; //color inc
   for (int q=0; q<fs; q++)
      al_draw_rectangle(x1+q, y1+q, x2-q, y2-q, palette_color[12+32+(q*ci)], 1);
   al_draw_text(font, palette_color[15], (x1+x2)/2, y1+2, ALLEGRO_ALIGN_CENTER, "List of Steps");

   return fs*2 + (lifts[lift].num_steps + 1) * bts; // return how much y space was used
}


