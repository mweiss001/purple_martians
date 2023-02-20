// z_lift.cpp

/*

#include "pm.h"
#include "z_lift.h"
#include "z_player.h"
#include "mwFont.h"
#include "mwBitmap.h"
#include "mwWindow.h"
#include "mwWindowManager.h"
#include "mwDisplay.h"
#include "mwWidgets.h"
#include "mwColor.h"
#include "mwPMEvent.h"
#include "mwInput.h"
#include "mwEventQueue.h"
#include "z_menu.h"
#include "mwProgramState.h"
#include "e_fnx.h"
#include "e_object_viewer.h"
#include "z_fnx.h"
#include "z_screen.h"

struct lift lifts[NUM_LIFTS];
struct lift_step lift_steps[NUM_LIFTS][40];

char lift_step_type_name[10][10];



//struct lift2 lifts2[NUM_LIFTS];


int construct_lift(int l, char* lift_name)
{
   clear_lift(l);
   strcpy(lifts[l].lift_name, lift_name);
   lifts[l].active = 1;
   return 1;
}

void clear_lift(int l)
{
   lifts[l].active = 0;
   lifts[l].x = 0;
   lifts[l].y = 0;
   lifts[l].xinc = 0;
   lifts[l].yinc = 0;
   lifts[l].w = 0;
   lifts[l].h = 0;
   lifts[l].winc = 0;
   lifts[l].hinc = 0;
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

int get_num_lifts(void)
{
   int nl = 0;
   for (int l=0; l<NUM_LIFTS; l++)
      if (lifts[l].active) nl++;
   return nl;
}

int get_next_lift(int l)
{
   if (++l >= NUM_LIFTS) l = 0;
   while (!lifts[l].active)
   {
      if (++l >= NUM_LIFTS) l = 0;
   }
   return l;
}

int get_prev_lift(int l)
{
   if (--l < 0) l = NUM_LIFTS-1;
   while (!lifts[l].active)
   {
      if (--l < 0) l = NUM_LIFTS-1;
   }
   return l;
}

void show_all_lifts(void)
{
   int text_pos = 0;
   al_set_target_backbuffer(display);
   al_clear_to_color(al_map_rgb(0,0,0));
   al_draw_textf(mF.pr8, mC.pc[13], 10, text_pos*8, 0, "number of lifts:%d", get_num_lifts());
   text_pos++;
   for (int l=0; l<NUM_LIFTS; l++) // iterate lifts
      if (lifts[l].active)
      {
         int x1 = lifts[l].x;
         int y1 = lifts[l].y;
         int x2 = lifts[l].x + lifts[l].w;
         int y2 = lifts[l].y + lifts[l].h;
         int w  = lifts[l].w;
         int h  = lifts[l].h;
         int mode = lifts[l].mode;

         int v1 = lifts[l].val1;
         int v2 = lifts[l].val2;

         int col  = lifts[l].color;
         sprintf(msg,"lift:%-2d  x:%-4d y:%-4d w:%-4d h:%-4d x2:%-4d y2:%-4d mode:%d v1:%d v2:%d col:%-2d ns:%-2d  name:%s  ", l, x1, y1, w, h, x2, y2, mode, v1, v2, col, lifts[l].num_steps, lifts[l].lift_name);
         al_draw_text(mF.pr8, mC.pc[10], 10, text_pos*8, 0, msg);
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
            printBits(4, &lift_steps[l][s].type);
            al_draw_textf(mF.pr8, mC.pc[color], 10, text_pos*8, 0, " step:%-2d x:%-4d y:%-4d w:%-4d h:%-4d val:%-4d type:%d (%s) col:%2d b:%s", s, x, y, w, h, val, type, typemsg, step_color, msg);
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
   if (step == 0) step = 40; // if started at step 0, start searching back from end to get last move step
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
   for (int l=lift; l<NUM_LIFTS-1; l++) // slide down to close hole in array
   {
      lifts[l] = lifts[l+1];
      for (int s=0; s<40; s++)
         lift_steps[l][s] = lift_steps[l+1][s];
   }

   // no need to erase last lift because a blank one past it will be copied over it
   // except in the case that there is no blank one past it...
   if (get_num_lifts() == 40)
   {
      clear_lift(39);  // clear last lift
      for (int s=0; s<40; s++) clear_lift_step(39, s); // clear last lift's steps
   }
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
   for (int l=0; l<NUM_LIFTS; l++)
      if (lifts[l].active) set_lift_to_step(l, 0);
}

int get_empty_lift(void) // find an inactive lift
{
   for (int l=0; l<NUM_LIFTS; l++)
      if (!lifts[l].active) return l;
   return -1;
}

int create_lift(void)
{
   int step = 0;

   int l = get_empty_lift(); // find an inactive lift
   if (l > -1)
   {

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

      sprintf(msg, "lift %d", l);

      construct_lift(l, msg);
      construct_lift_step(l, step, initial_type, 0, 0, initial_width, initial_height, initial_val);
      lifts[l].num_steps++; // add one to steps

      if (getxy("Lift Initial Position", 4, l, step) == 1)
      {
         step++;
         initial_type = (4 | cf);  // make type 4 step type with same flags as initial
         construct_lift_step(l, step, initial_type, 0, 0, 0, 0, 0); // type 4 - loop to step zero
         lifts[l].num_steps++; // add one to steps

         set_lift_to_step(l, 0); // set step 0 for lift

         mwWM.redraw_level_editor_background();  // do this twice to get proper window height
         mwWM.redraw_level_editor_background();


         insert_steps_until_quit(l, step);

         object_viewer(4, l);
         return 1;
      }
      else
      {
         erase_lift(l);
         return 0;
      }
   }
   else
   {
      al_show_native_message_box(display, "Error", "Error creating lift", "Lift list full!", NULL, ALLEGRO_MESSAGEBOX_ERROR);
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

   int sty = mwWM.mW[7].y1 + 44 + (step + 10) * bts;

   if (sty > mwD.SCREEN_H-60) sty = mwD.SCREEN_H-60;

   int num_of_step_types = 5;
   int sth = (num_of_step_types * 12) + 26;
   int sty2 = sty + sth;

   int fc = 14; // frame color
   int tc = 15; // text color

   int xc = (mwWM.mW[7].x1 + mwWM.mW[7].x2)/2;

   al_draw_filled_rectangle(xc-98, sty-8, xc+96, sty2, mC.pc[fc+192]); // erase to background color
   al_draw_rectangle       (xc-98, sty-8, xc+96, sty2, mC.pc[fc], 1); // frame

   al_set_mouse_xy(display, xc * mwD.display_transform_double, (sty+24) * mwD.display_transform_double); // position the mouse

   int quit = 0;
   while (!quit)
   {
      al_rest(0.02);
      al_flip_display();
      mwEQ.proc_event_queue();
      if ((mI.mouse_b[2][0]) || (mI.key[ALLEGRO_KEY_ESCAPE][0])) quit = 99;
      al_draw_textf(mF.pr8, mC.pc[fc], xc, sty-6, ALLEGRO_ALIGN_CENTER, "Insert New Step %d", step);
      al_draw_text(mF.pr8, mC.pc[tc], xc, sty+5, ALLEGRO_ALIGN_CENTER, "Select Step Type");
      al_draw_rectangle(xc-98, sty+3, xc+96, sty+14, mC.pc[fc], 1); // frame
      int c1 = 14+64; // button color
      int c2 = 15; // text color
      int c3 = 15; // highlight color
      int bts = 14;
      int ya = sty+2+bts;
      if (mdw_buttontca(xc, ya, 0, bts,  0,0,0,0,  0,c1,c2,c3,  1,0,1,0, "Move"))
      {
         quit = construct_lift_step(lift, step, 1, 0, 0, 0, 0, 20);
         lift_step_set_size_from_previous_move_step(lift, step);
         if (getxy("Step Position", 4, lift, step) != 1) quit = 99;
      }
      if (mdw_buttontca(xc, ya, 0, bts,  0,0,0,0,  0,c1,c2,c3,  1,0,1,0, "Wait For Time")) quit = construct_lift_step(lift, step, 2, 0, 0, 0, 0, 100);
      if (mdw_buttontca(xc, ya, 0, bts,  0,0,0,0,  0,c1,c2,c3,  1,0,1,0, "Wait For Prox")) quit = construct_lift_step(lift, step, 3, 0, 0, 0, 0, 80);
      if (mdw_buttontca(xc, ya, 0, bts,  0,0,0,0,  0,c1,c2,c3,  1,0,1,0, "Wait For Trig")) quit = construct_lift_step(lift, step, 5, 0, 0, 0, 0, 0);
      if (mdw_buttontca(xc, ya, 0, bts,  0,0,0,0,  0,c1,c2,c3,  1,0,1,0, "Done"))          quit = 99;
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
      int step_ty = mwWM.mW[7].y1+ 38 + 7 * bts;
      draw_steps(mwWM.mW[7].x1+1, mwWM.mW[7].x2-1, step_ty, lift, step, step, 1);     // show lift steps
      if (get_new_lift_step(lift, step) == 99) // cancelled
      {
         delete_lift_step(lift, step);
         ret = 0;
      }
      else ret = 1;
      mwWM.redraw_level_editor_background();  // do this twice to get proper window height
      mwWM.redraw_level_editor_background();

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
   int smx = mI.mouse_x;
   int smy = mI.mouse_y;

   if (smx > mwD.SCREEN_W-100) smx = mwD.SCREEN_W-100;
   if (smy > mwD.SCREEN_H-100) smy = mwD.SCREEN_H-100;
   if (smx < 100) smx = 100;
   if (smy < 30) smy = 30;

   al_set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);
   al_set_mouse_xy(display, smx * mwD.display_transform_double, smy * mwD.display_transform_double);

   mwEQ.proc_event_queue(); // to deal with mouse warp

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

int draw_current_step_buttons(int x1, int x2, int y, int l, int s, int d)
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
         mdw_buttont(xa, ya, xb, bts,  0,0,0,0, 0,sd,15,0, 1,0,1,d, msg);
      break;
      case 2: // wait time
         sprintf(msg, "Step:%d - Wait For Timer", s);
         mdw_buttont(xa, ya, xb, bts,  0,0,0,0, 0,sd,15,0, 1,0,1,d, msg);
      break;
      case 3: // wait prox
         sprintf(msg, "Step:%d - Wait For Player Proximity", s);
         mdw_buttont(xa, ya, xb, bts,  0,0,0,0, 0,sd,15,0, 1,0,1,d, msg);
      break;
      case 4: // end step
         sprintf(msg, "Step:%d - Ending Step", s);
         mdw_buttont(xa, ya, xb, bts,  0,0,0,0, 0,sd,15,0, 1,0,1,d, msg);
      break;
      case 5: // wait trigger
         sprintf(msg, "Step:%d - Wait For Trigger", s);
         mdw_buttont(xa, ya, xb, bts,  0,0,0,0, 0,sd,15,0, 1,0,1,d, msg);
      break;
   }

   // default buttons
   int col = (lift_steps[l][s].type >> 28) & 15;
   sprintf(msg, "Step Color:%d", col);
   mdw_buttont(xa, ya, xb, bts, 0,0,0,0,  0, col, 15, 0,   1,0,1,d, msg);

   mdw_colsel(xa, ya, xb, bts, 8,l,s,0,  0,15,13,14,   0,0,1,d); // lift step color

   int dim = 32;
   int c2 = 14;
   int c3 = 15;

   mdw_togglf(xa, ya, xb, bts, 0,0,0,0, 0,0,0,0, 1,0,1,d, lift_steps[l][s].type, PM_LIFT_NO_DRAW,      "Draw Lift",            "Hide Lift",            c3, c3+dim, c2, c2+dim);
   mdw_togglf(xa, ya, xb, bts, 0,0,0,0, 0,0,0,0, 1,0,1,d, lift_steps[l][s].type, PM_LIFT_SOLID_PLAYER, "Solid for Player:OFF", "Solid for Player:ON ", c3+dim, c3, c2+dim, c2);
   mdw_togglf(xa, ya, xb, bts, 0,0,0,0, 0,0,0,0, 1,0,1,d, lift_steps[l][s].type, PM_LIFT_SOLID_ENEMY,  "Solid for Enemy:OFF ", "Solid for Enemy:ON  ", c3+dim, c3, c2+dim, c2);
   mdw_togglf(xa, ya, xb, bts, 0,0,0,0, 0,0,0,0, 1,0,1,d, lift_steps[l][s].type, PM_LIFT_SOLID_ITEM,   "Solid for Item:OFF  ", "Solid for Item:ON   ", c3+dim, c3, c2+dim, c2);
   mdw_togglf(xa, ya, xb, bts, 0,0,0,0, 0,0,0,0, 1,0,1,d, lift_steps[l][s].type, PM_LIFT_HIDE_LINES,   "Draw Lift Lines",      "Hide Lift Lines",      c3, c3+dim, c2, c2+dim);

   // specific buttons
   switch (lift_steps[l][s].type & 31)
   {
      case 1: // move and resize
         mdw_slideri(xa, ya, xb, bts,  0,0,0,0,   0,c1,15,15, 1,0,1,d, lift_steps[l][s].val, 1000, 1, 1, "Speed:");
         mdw_slideri(xa, ya, xb, bts,  0,0,0,0,   0,c1,15,15, 1,0,1,d, lift_steps[l][s].w,   1600, 20, 1, "Width:");
         mdw_slideri(xa, ya, xb, bts,  0,0,0,0,   0,c1,15,15, 1,0,1,d, lift_steps[l][s].h,   1600, 20, 1, "Height:");
      break;
      case 2: // wait time
         mdw_slideri(xa, ya, xb, bts,  0,0,0,0,   0,c1,15,15, 1,0,1,d, lift_steps[l][s].val, 2000, 1, 1, "Timer:");
      break;
      case 3: // wait prox
         mdw_slideri(xa, ya, xb, bts,  0,0,0,0,   0,c1,15,15, 1,0,1,d, lift_steps[l][s].val, 200, 20, 10, "Distance:");
      break;
      case 4: // end step
         mdw_button( xa, ya, xb, bts,  505,l,s,0, 0,c1,15,0,  1,0,1,d); // lift step end step mode
      break;
      case 5: // wait trigger
         mdw_slideri(xa, ya, xb, bts,  0,0,0,0,   0,c1,15,15, 1,0,1,d, lift_steps[l][s].val, 99, 0, 1, "Trigger:");
         mdw_button( xa, ya, xb, bts,  520,l,s,4, 0,c1,15,0,  1,0,1,d); ya+=bts; // lift step wait trigger get event
      break;
   }

   // frame and title
   int y1 = y;
   int y2 = ya-bts+(fs*2);
   for (int q=0; q<fs; q++)
      al_draw_rectangle(x1+q, y1+q, x2-q, y2-q, mC.pc[10+(q*16)], 1);
   al_draw_text(mF.pr8, mC.pc[15], (x1+x2)/2, y1+2, ALLEGRO_ALIGN_CENTER, "Current Step Details");
   return y2-y1+2; // return how much y space was used
}

void draw_step_button(int xa, int xb, int ty1, int ty2, int l, int s, int rc, int d)
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
      mdw_button(x1, ty1, x2, ty2-ty1, 501, -1, 0, 0,  0,rc,15,0,  1,0,0,d); // num
      mdw_button(x2, ty1, x3, ty2-ty1, 506, -1, 0, 0,  0,rc,15,0,  1,0,0,d); // 'C'
      mdw_button(x3, ty1, x4, ty2-ty1, 502, -1, 0, 0,  0,rc,15,0,  1,0,0,d); // type
      mdw_button(x4, ty1, xb, ty2-ty1, 503, -1, 0, 0,  0,rc,15,0,  1,0,0,d); // description
   }
   else
   {
      mdw_button(x1, ty1, x2, ty2-ty1, 501, s, 0, 0,   0,rc,15,0,  1,0,0,d); // num
      mdw_button(x2, ty1, x3, ty2-ty1, 506, c, 0, 0,   0, c,15,0,  1,0,0,d); // color
      mdw_button(x3, ty1, x4, ty2-ty1, 502, t, 0, 0,   0,rc,15,0,  1,0,0,d); // type
      mdw_button(x4, ty1, xb, ty2-ty1, 503, t, l, s,   0,rc,15,0,  1,1,0,d); // description
   }
}

int draw_steps(int x1, int x2, int y, int lift, int current_step, int highlight_step, int d)
{
   int bts = 16;
   int fs = 14; // frame_size
   int a = 0;
   int xa = x1+fs;
   int xb = x2-fs;
   int ya = y+fs;

   // draw title step
   draw_step_button(xa, xb, ya+(a)*bts, ya+(a+1)*bts-1, lift, -1, 13, d); a++;

   // draw steps
   for (int step=0; step<lifts[lift].num_steps; step++)
   {
      int color = 13;
      if (step == current_step) color = 10;
      draw_step_button(xa, xb, ya+(a)*bts, ya+(a+1)*bts-1, lift, step, color, d); a++;
   }

   // show outline around highlighted step
   int hs = highlight_step+1;
   if (hs > -1) al_draw_rectangle(xa, ya+hs*bts, xb, ya+(hs+1)*bts, mC.pc[14], 2);

   int y1 = y;
   int y2 = y1+a*bts+fs*2;
   int ci = 16; //color inc
   for (int q=0; q<fs; q++)
      al_draw_rectangle(x1+q, y1+q, x2-q, y2-q, mC.pc[12+32+(q*ci)], 1);
   al_draw_text(mF.pr8, mC.pc[15], (x1+x2)/2, y1+2, ALLEGRO_ALIGN_CENTER, "List of Steps");

   return fs*2 + (lifts[lift].num_steps + 1) * bts; // return how much y space was used
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
   lifts[l].x     = lift_steps[l][s].x;
   lifts[l].y     = lift_steps[l][s].y;
   lifts[l].w     = lift_steps[l][s].w;
   lifts[l].h     = lift_steps[l][s].h;
   lifts[l].flags = lift_steps[l][s].type; // get all the flags from the step type

 //  lifts[l].color  = (lift_steps[l][s].type >> 28) & 15; // not needed

   // set incs to 0
   lifts[l].xinc = 0;
   lifts[l].yinc = 0;
   lifts[l].winc = 0;
   lifts[l].hinc = 0;
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
   if ((!(lifts[l].flags & PM_LIFT_NO_DRAW)) || (mwPS.level_editor_running))
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
                  al_draw_line( px, py, nx, ny, mC.pc[col], 1);
                  if (mwPS.eco_draw)
                  {
                     // al_draw_filled_circle(nx, ny, 2, mC.pc[col]);
                  }
                  else
                  {
                     for (int c=3; c>=0; c--)
                        al_draw_filled_circle(nx, ny, c, mC.pc[(col - 96) + c*48]);
                  }
               }

               px = nx;
               py = ny;
            }
         }
         if (!(lift_steps[l][0].type & PM_LIFT_HIDE_LINES))
         {
            col = (lift_steps[l][0].type >> 28) & 15;
            al_draw_line(sx, sy, nx, ny, mC.pc[col], 1); // draw line from last to first
         }
      }
   }
}

















// this is it....the one base function that draws a lift
void draw_lift(int l, int x1, int y1, int x2, int y2)
{
   int col = (lifts[l].flags >> 28) & 15;

   if ((lifts[l].flags & PM_LIFT_NO_DRAW) && (mwPS.level_editor_running)) col = 0;

   if (mwPS.eco_draw)
   {
      al_draw_filled_rectangle(x1, y1, x2, y2, mC.pc[col]);
      //al_draw_filled_rounded_rectangle(x1, y1, x2, y2, 4, 4, mC.pc[col] );
      al_draw_text(mF.bltn, mC.pc[col+160], (x1+x2)/2, (y1+y2)/2 - 3, ALLEGRO_ALIGN_CENTRE, lifts[l].lift_name); // name
   }
   else
   {
      int a;
      for (a=0; a<10; a++)
        al_draw_rounded_rectangle(x1+a, y1+a, x2-a, y2-a, 4, 4, mC.pc[col + ((9 - a)*16)], 2 ); // faded outer shell

      al_draw_filled_rectangle(x1+a, y1+a, x2-a, y2-a, mC.pc[col] );                            // solid core
      al_draw_text(mF.bltn, mC.pc[col+160], (x1+x2)/2, (y1+y2)/2 - 3, ALLEGRO_ALIGN_CENTRE, lifts[l].lift_name); // name
   }


  // printf("x1:%d y1:%d x2:%d y2:%d\n", x1, y1, x2, y2);

//   al_draw_textf(mF.pr8, mC.pc[col+160], (x1+x2)/2, (y1+y2)/2 - 3, ALLEGRO_ALIGN_CENTRE, "s:%d v:%d", lifts[l].current_step, lifts[l].val1);    // debug name

//   al_draw_textf(mF.pr8, mC.pc[15], (x1+x2)/2, (y1+y2)/2 - 16, ALLEGRO_ALIGN_CENTRE, "s:%d v:%d", lifts[l].current_step, lifts[l].val1);    // debug name


}


void draw_lifts()
{
   al_set_target_bitmap(mwB.level_buffer);

   for (int l=0; l<NUM_LIFTS; l++)
      if (lifts[l].active)
      {
         draw_lift_line(l);

         if ((!(lifts[l].flags & PM_LIFT_NO_DRAW)) || (mwPS.level_editor_running))
         {
            int color = (lifts[l].flags >> 28) & 15;
            int x1 = lifts[l].x;
            int x2 = lifts[l].x + lifts[l].w;
            int y1 = lifts[l].y;
            int y2 = lifts[l].y + lifts[l].h;
            draw_lift(l, x1, y1, x2, y2);


            // show if player is riding this lift
            int p = is_player_riding_lift(l);
            if (p)
            {
               p -=1; // player number
               int pc = players[p].color;
               if (pc == color) pc = 127;
               al_draw_rounded_rectangle(x1, y1, x2, y2, 4, 4, mC.pc[pc], 2);
            }

            if ((lifts[l].mode) && (!is_player_riding_lift(l)))
            {
               // if not in initial position
               if (!((lifts[l].x == lift_steps[l][0].x) && (lifts[l].y == lift_steps[l][0].y)))
               {
                  int lw = lifts[l].w-10;
                  int lh = lifts[l].h-10;
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
                        al_draw_textf(mF.pr8, mC.pc[color+64], (x1 + x2)/2 + 2, lifts[l].y - 8, ALLEGRO_ALIGN_CENTRE, "%d", lifts[l].limit_counter);
                  break;
                  case 3: // prox wait
                  {
                     int pd = lifts[l].limit_counter; // prox dist
                     int bx1 = x1 - pd;
                     int by1 = y1 - pd;
                     int bx2 = x2 + pd;
                     int by2 = y2 + pd;
                     al_draw_rectangle(bx1+10, by1+10, bx2-10, by2-10, mC.pc[color+128], 1);
                  }
                  break;
               }
            }
         }
      }  // end of iterate lifts
}


void set_lift_xyinc(int l, int step)
{
   //  used when switching to a new move step;
   //  sets xinc, yinc and num of frames for move

   int val = lift_steps[l][step].val;

   float move_time = 0;
   float xlen = lift_steps[l][step].x - lifts[l].x;    // x length
   float ylen = lift_steps[l][step].y - lifts[l].y;    // y length
   float hy_dist = sqrt(pow(xlen, 2) + pow(ylen, 2));  // hypotenuse distance

   if (hy_dist < 1)        // no move
   {
      move_time = val;     // use val as straight timer
      lifts[l].xinc = 0;   // no xinc
      lifts[l].yinc = 0;   // no yinc
   }
   else // get time based on speed and distance
   {
      float speed = (float) val / 10;    // stored speed scaled by 10
      move_time = hy_dist / speed;       // get time (time = distance / speed)
      lifts[l].xinc = xlen / move_time;  // set xinc
      lifts[l].yinc = ylen / move_time;  // set yinc
   }

   float wd = lift_steps[l][step].w - lifts[l].w;   // width dif
   float hd = lift_steps[l][step].h - lifts[l].h;   // height dif
   lifts[l].winc = wd / move_time;                  // set winc
   lifts[l].hinc = hd / move_time;                  // set hinc

   lifts[l].limit_counter = move_time;
   lifts[l].limit_type = 1;
}

int lift_check_prox(int l, int pd)
{
   // check if player is within lift proximity
   float bx1 = lifts[l].x - pd - 10;
   float by1 = lifts[l].y - pd - 10;
   float bx2 = lifts[l].x + lifts[l].w + pd - 10;
   float by2 = lifts[l].y + lifts[l].h + pd - 10;

   for (int p=0; p<NUM_PLAYERS; p++)
      if ((players[p].active) && (!players[p].paused))
         if ((players[p].x > bx1) && (players[p].x < bx2) && (players[p].y > by1) && (players[p].y < by2)) return 1;
   return 0;
}



void move_lifts(int ignore_prox)
{
   for (int l=0; l<NUM_LIFTS; l++)
      if (lifts[l].active)
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

                  lifts[l].x += lifts[l].xinc;
                  lifts[l].y += lifts[l].yinc;
                  lifts[l].w += lifts[l].winc;
                  lifts[l].h += lifts[l].hinc;


                  if (--lifts[l].limit_counter < 0) // run down the counter
                  {
                     next_step = 1;

                     //make sure lift is exactly where it should be at the end of the move...
                     int step = lifts[l].current_step;
                     lifts[l].x = lift_steps[l][step].x;
                     lifts[l].y = lift_steps[l][step].y;
                     lifts[l].w = lift_steps[l][step].w;
                     lifts[l].h = lift_steps[l][step].h;

                     // zero incs when done
                     lifts[l].xinc = 0;
                     lifts[l].yinc = 0;
                     lifts[l].winc = 0;
                     lifts[l].hinc = 0;
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
                  if (mwPME.event[lifts[l].limit_counter]) next_step = 1;
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
            mwPME.event[lift_steps[l][s].val] = 0; // clear trigger event

      } // end of lift iterate
}
*/
