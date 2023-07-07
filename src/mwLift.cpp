// mwLift.cpp

#include "pm.h"
#include "mwLift.h"
#include "mwPlayer.h"
#include "mwFont.h"
#include "mwBitmap.h"
#include "mwWindowManager.h"
#include "mwDisplay.h"
#include "mwWidget.h"
#include "mwColor.h"
#include "mwTriggerEvent.h"
#include "mwInput.h"
#include "mwEventQueue.h"
#include "mwMenu.h"
#include "mwLoop.h"
#include "mwMiscFnx.h"
#include "mwScreen.h"




mwLift mLift;

mwLift::mwLift()
{
   initialize();
}

void mwLift::initialize(void)
{
   strcpy (lift_step_type_name[0], "BAD!");
   strcpy (lift_step_type_name[1], "Move");
   strcpy (lift_step_type_name[2], "Wait");
   strcpy (lift_step_type_name[3], "Prox");
   strcpy (lift_step_type_name[4], "End ");
   strcpy (lift_step_type_name[5], "REvt");
   strcpy (lift_step_type_name[6], "SEvt");
}


int mwLift::construct_lift(int lift, char* lift_name)
{
   clear_lift(lift);
   strcpy(cur[lift].lift_name, lift_name);
   cur[lift].active = 1;
   return 1;
}

void mwLift::clear_lift(int l)
{
   cur[l].active = 0;
   cur[l].x = 0;
   cur[l].y = 0;
   cur[l].xinc = 0;
   cur[l].yinc = 0;
   cur[l].w = 0;
   cur[l].h = 0;
   cur[l].winc = 0;
   cur[l].hinc = 0;
   cur[l].flags = 0;
   cur[l].mode = 0;
   cur[l].val1 = 0;
   cur[l].val2 = 0;
   cur[l].color = 0;
   cur[l].current_step = 0;
   cur[l].num_steps = 0;
   cur[l].limit_type = 0;
   cur[l].limit_counter = 0;
   strcpy(cur[l].lift_name, "");
}

int mwLift::construct_lift_step(int l, int s, int type, int x, int y, int w, int h, int val)
{
   clear_lift_step(l, s);
//   printf("type:%d\n", type);

   if ((type > 0) && (type < 16)) // get flags and color from previous steps
   {
      int ps = 0;
      if (type == 1) // find previous move step
      {
          ps = find_previous_move_step(l, s);
          type = stp[l][ps].type;
          val  = 20;
          w    = stp[l][ps].w;
          h    = stp[l][ps].h;
      }
      else // find any previous step
      {
         if (s == 0) ps = cur[l].num_steps - 1; // if step 0 get last step
         else ps = s - 1;

         // get flags and color by masking off lower 5 bits of type
         int cf = stp[l][ps].type & 0b11111111111111111111111111100000;
         type |= cf; // merge with type
      }
   }

   stp[l][s].type = type;
   stp[l][s].x    = x;
   stp[l][s].y    = y;
   stp[l][s].w    = w;
   stp[l][s].h    = h;
   stp[l][s].val  = val;
   return 1;
}

void mwLift::clear_lift_step(int l, int s)
{
   stp[l][s].type = 0;
   stp[l][s].x    = 0;
   stp[l][s].y    = 0;
   stp[l][s].w    = 0;
   stp[l][s].h    = 0;
   stp[l][s].val  = 0;
}

int mwLift::get_num_lifts(void)
{
   int nl = 0;
   for (int l=0; l<NUM_LIFTS; l++)
      if (cur[l].active) nl++;
   return nl;
}

int mwLift::get_next_lift(int l)
{
   if (++l >= NUM_LIFTS) l = 0;
   while (!cur[l].active)
   {
      if (++l >= NUM_LIFTS) l = 0;
   }
   return l;
}

int mwLift::get_prev_lift(int l)
{
   if (--l < 0) l = NUM_LIFTS-1;
   while (!cur[l].active)
   {
      if (--l < 0) l = NUM_LIFTS-1;
   }
   return l;
}

void mwLift::show_all_lifts(void)
{
   char msg[1024];
   int text_pos = 0;
   al_set_target_backbuffer(mDisplay.display);
   al_clear_to_color(al_map_rgb(0,0,0));
   al_draw_textf(mFont.pr8, mColor.pc[13], 10, text_pos*8, 0, "number of lifts:%d", get_num_lifts());
   text_pos++;
   for (int l=0; l<NUM_LIFTS; l++) // iterate lifts
      if (cur[l].active)
      {
         int x1 = cur[l].x;
         int y1 = cur[l].y;
         int x2 = cur[l].x + cur[l].w;
         int y2 = cur[l].y + cur[l].h;
         int w  = cur[l].w;
         int h  = cur[l].h;
         int mode = cur[l].mode;

         int v1 = cur[l].val1;
         int v2 = cur[l].val2;

         int col  = cur[l].color;
         sprintf(msg,"lift:%-2d  x:%-4d y:%-4d w:%-4d h:%-4d x2:%-4d y2:%-4d mode:%d v1:%d v2:%d col:%-2d ns:%-2d  name:%s  ", l, x1, y1, w, h, x2, y2, mode, v1, v2, col, cur[l].num_steps, cur[l].lift_name);
         al_draw_text(mFont.pr8, mColor.pc[10], 10, text_pos*8, 0, msg);
         text_pos++;

         for (int s=0; s<cur[l].num_steps; s++) // iterate steps
         {
            int x    = stp[l][s].x;
            int y    = stp[l][s].y;
            int w    = stp[l][s].w;
            int h    = stp[l][s].h;
            int val  = stp[l][s].val;
            int type = stp[l][s].type & 31;
            int step_color = (stp[l][s].type >> 28) & 15;

            int color = 9;
            char typemsg[10];
            if ((type >0) && (type < 7))
            {
               sprintf(typemsg,"%s", lift_step_type_name[type] );
               if (type == 1) color = 9; // green for move
               if (type == 2) color = 6; // tan for wait time
               if (type == 3) color = 6; // tan for wait prox
               if (type == 4) color = 5; // green for end step
               if (type == 5) color = 4; // dk red for wait event
               if (type == 6) color = 13; // lt blue for send event
            }
            else
            {
               sprintf(typemsg,"bad step");
               color = 14;
            }
            mMiscFnx.printBits(4, &stp[l][s].type);
            al_draw_textf(mFont.pr8, mColor.pc[color], 10, text_pos*8, 0, " step:%-2d x:%-4d y:%-4d w:%-4d h:%-4d val:%-4d type:%d (%s) col:%2d b:%s", s, x, y, w, h, val, type, typemsg, step_color, msg);
            text_pos++;
         }
      }
   al_flip_display();
   mInput.tsw(); // wait for keypress
}




void mwLift::set_size_from_previous_step(int lift, int step)
{
   int pms = find_previous_move_step(lift, step); // searches back from passed step until a move step is found
   if (pms != -1)
   {
      stp[lift][step].w = stp[lift][pms].w;
      stp[lift][step].h = stp[lift][pms].h;
   }
}

int mwLift::find_previous_move_step(int lift, int step) // searches back from passed step until a move step is found
{
   if (step == 0) step = 40; // if started at step 0, start searching back from end to get last move step
   while (step)
   {
      step--;
      if ((stp[lift][step].type & 31) == 1) return step;
   }
   printf("could not find move step!!\n");
   return -1; // bad !!!
}

void mwLift::erase_lift(int lift)
{
   for (int l=lift; l<NUM_LIFTS-1; l++) // slide down to close hole in array
   {
      cur[l] = cur[l+1];
      for (int s=0; s<40; s++)
         stp[l][s] = stp[l+1][s];
   }

   // no need to erase last lift because a blank one past it will be copied over it
   // except in the case that there is no blank one past it...
   if (get_num_lifts() == 40)
   {
      clear_lift(39);  // clear last lift
      for (int s=0; s<40; s++) clear_lift_step(39, s); // clear last lift's steps
   }
}

void mwLift::delete_lift_step(int l, int step)
{
   for (int x=step; x<cur[l].num_steps-1; x++)   // slide all down
   {
      stp[l][x].x    = stp[l][x+1].x ;
      stp[l][x].y    = stp[l][x+1].y ;
      stp[l][x].w    = stp[l][x+1].w ;
      stp[l][x].h    = stp[l][x+1].h ;
      stp[l][x].val  = stp[l][x+1].val ;
      stp[l][x].type = stp[l][x+1].type ;
   }
   cur[l].num_steps--;
}

void mwLift::lift_setup(void) // set all lifts to step 0
{
   for (int l=0; l<NUM_LIFTS; l++)
      if (cur[l].active) set_lift_to_step(l, 0);
}

int mwLift::get_empty_lift(void) // find an inactive lift
{
   for (int l=0; l<NUM_LIFTS; l++)
      if (!cur[l].active) return l;
   return -1;
}

int mwLift::create_lift(void)
{
   char msg[1024];
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
      cur[l].num_steps++; // add one to steps

      if (mMiscFnx.getxy("Lift Initial Position", 4, l, step) == 1)
      {
         step++;
         initial_type = (4 | cf);  // make type 4 step type with same flags as initial
         construct_lift_step(l, step, initial_type, 0, 0, 0, 0, 0); // type 4 - loop to step zero
         cur[l].num_steps++; // add one to steps

         set_lift_to_step(l, 0); // set step 0 for lift

         mWM.redraw_level_editor_background();  // do this twice to get proper window height
         mWM.redraw_level_editor_background();


         insert_steps_until_quit(l, step);

         mWM.mW[7].object_viewer(4, l);
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
      al_show_native_message_box(mDisplay.display, "Error", "Error creating lift", "Lift list full!", NULL, ALLEGRO_MESSAGEBOX_ERROR);
      return 0;
   }
}

void mwLift::move_lift_step(int lift, int step)
{
   if ((stp[lift][step].type & 31) == 1) mMiscFnx.getxy("Step Position", 4,  lift, step); // only if type = move
}

int mwLift::get_new_lift_step(int lift, int step)
{
   int bts = 16;
   // step is a new blank step already created for us
   // if we return 99 the step will be erased

   // position the menu on top of the step we are inserting before
   // int sty = 53 + (step + 9) * bts;

   int sty = mWM.mW[7].y1 + 44 + (step + 10) * bts;

   if (sty > mDisplay.SCREEN_H-60) sty = mDisplay.SCREEN_H-60;

   int num_of_step_types = 6;
   int sth = (num_of_step_types * 12) + 26;
   int sty2 = sty + sth;

   int fc = 14; // frame color
   int tc = 15; // text color

   int xc = (mWM.mW[7].x1 + mWM.mW[7].x2)/2;

   al_draw_filled_rectangle(xc-98, sty-8, xc+96, sty2, mColor.pc[fc+192]); // erase to background color
   al_draw_rectangle       (xc-98, sty-8, xc+96, sty2, mColor.pc[fc], 1); // frame

   al_set_mouse_xy(mDisplay.display, xc * mDisplay.display_transform_double, (sty+24) * mDisplay.display_transform_double); // position the mouse

   int quit = 0;
   while (!quit)
   {
      al_rest(0.02);
      al_flip_display();
      mEventQueue.proc();
      if ((mInput.mouse_b[2][0]) || (mInput.key[ALLEGRO_KEY_ESCAPE][0])) quit = 99;
      al_draw_textf(mFont.pr8, mColor.pc[fc], xc, sty-6, ALLEGRO_ALIGN_CENTER, "Insert New Step %d", step);
      al_draw_text(mFont.pr8, mColor.pc[tc], xc, sty+5, ALLEGRO_ALIGN_CENTER, "Select Step Type");
      al_draw_rectangle(xc-98, sty+3, xc+96, sty+14, mColor.pc[fc], 1); // frame
      int c1 = 14+64; // button color
      int c2 = 15; // text color
      int c3 = 15; // highlight color
      int bts = 14;
      int ya = sty+2+bts;
      if (mWidget.buttontca(xc, ya, 0, bts,  0,0,0,0,  0,c1,c2,c3,  1,0,1,0, "Move"))
      {
         quit = construct_lift_step(lift, step, 1, 0, 0, 0, 0, 20);
         set_size_from_previous_step(lift, step);
         if (mMiscFnx.getxy("Step Position", 4, lift, step) != 1) quit = 99;
      }
      if (mWidget.buttontca(xc, ya, 0, bts,  0,0,0,0,  0,c1,c2,c3,  1,0,1,0, "Wait For Time")) quit = construct_lift_step(lift, step, 2, 0, 0, 0, 0, 100);
      if (mWidget.buttontca(xc, ya, 0, bts,  0,0,0,0,  0,c1,c2,c3,  1,0,1,0, "Wait For Prox")) quit = construct_lift_step(lift, step, 3, 0, 0, 0, 0, 80);
      if (mWidget.buttontca(xc, ya, 0, bts,  0,0,0,0,  0,c1,c2,c3,  1,0,1,0, "Wait For Event")) quit = construct_lift_step(lift, step, 5, 0, 0, 0, 0, 0);
      if (mWidget.buttontca(xc, ya, 0, bts,  0,0,0,0,  0,c1,c2,c3,  1,0,1,0, "Send Event")) quit = construct_lift_step(lift, step, 6, 0, 0, 0, 0, 0);
      if (mWidget.buttontca(xc, ya, 0, bts,  0,0,0,0,  0,c1,c2,c3,  1,0,1,0, "Done"))          quit = 99;
   } // end of while (!quit)
   return quit;
}


int mwLift::insert_lift_step(int lift, int step) // inserts a step in 'lift' before 'step'
{
   int bts = 16;
   int ret = 0;

   // increment the number of steps
   if (++cur[lift].num_steps > 40)
   {
      cur[lift].num_steps--;
      ret = 0;
      al_show_native_message_box(mDisplay.display, "Error", "Error creating lift step", "40 steps is the maximum", NULL, ALLEGRO_MESSAGEBOX_ERROR);
   }
   else
   {
      // slide steps down to make room for new step
      for (int x=cur[lift].num_steps-2; x>=step; x--)
      {
         stp[lift][x+1].x    = stp[lift][x].x;
         stp[lift][x+1].y    = stp[lift][x].y;
         stp[lift][x+1].w    = stp[lift][x].w;
         stp[lift][x+1].h    = stp[lift][x].h;
         stp[lift][x+1].val  = stp[lift][x].val;
         stp[lift][x+1].type = stp[lift][x].type;
      }
      clear_lift_step(lift, step);
      int step_ty = mWM.mW[7].y1+ 38 + 7 * bts;
      draw_steps(mWM.mW[7].x1+1, mWM.mW[7].x2-1, step_ty, lift, step, step, 1);     // show lift steps
      if (get_new_lift_step(lift, step) == 99) // cancelled
      {
         delete_lift_step(lift, step);
         ret = 0;
      }
      else ret = 1;
      mWM.redraw_level_editor_background();  // do this twice to get proper window height
      mWM.redraw_level_editor_background();

   }
   return ret;
}

void mwLift::insert_steps_until_quit(int lift, int step)
{
   while (insert_lift_step(lift, step)) step++;
}


void mwLift::set_all_steps(int l, int s, int what)
{
   if (what == 1) // size
   {
      int w = stp[l][s].w;
      int h = stp[l][s].h;

      for (int si=0; si<cur[l].num_steps; si++) // iterate steps
      {
         if ((stp[l][si].type & 31) == 1)
         {
            stp[l][si].w = w;
            stp[l][si].h = h;
         }
      }
   }
   if (what == 2) // color
   {
      int col = stp[l][s].type & PM_LIFT_COLOR_BITS; // mask off all except color
      for (int si=0; si<cur[l].num_steps; si++)      // iterate steps
      {
         stp[l][si].type &= ~PM_LIFT_COLOR_BITS;     // clear color bits
         stp[l][si].type |= col;                     // merge
      }
   }
   if (what == 3) // flags
   {
      int flags = stp[l][s].type & PM_LIFT_FLAG_BITS; // mask off all except flags
      for (int si=0; si<cur[l].num_steps; si++)       // iterate steps
      {
         stp[l][si].type &= ~PM_LIFT_FLAG_BITS;       // clear flag bits
         stp[l][si].type |= flags;                    // merge
      }
   }
}


void mwLift::step_popup_menu(int lift, int step)
{
   int smx = mInput.mouse_x;
   int smy = mInput.mouse_y;

   if (smx > mDisplay.SCREEN_W-100) smx = mDisplay.SCREEN_W-100;
   if (smy > mDisplay.SCREEN_H-100) smy = mDisplay.SCREEN_H-100;
   if (smx < 100) smx = 100;
   if (smy < 30) smy = 30;

   al_set_system_mouse_cursor(mDisplay.display, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);
   al_set_mouse_xy(mDisplay.display, smx * mDisplay.display_transform_double, smy * mDisplay.display_transform_double);

   mEventQueue.proc(); // to deal with mouse warp

   // full menu
   sprintf(mMenu.menu_string[0],"Lift:%d Step:%d", lift+1, step);
   sprintf(mMenu.menu_string[1],"---------------");
   sprintf(mMenu.menu_string[2],"Cancel");
   sprintf(mMenu.menu_string[3],"Move Step %d", step);
   sprintf(mMenu.menu_string[4],"Delete Step %d", step);
   sprintf(mMenu.menu_string[5],"Insert Steps");
   sprintf(mMenu.menu_string[6],"Size=Prev Step");
   sprintf(mMenu.menu_string[7],"end");


   // special case for first step (don't allow delete or insert, only move )
   if (step == 0)
   {
      sprintf(mMenu.menu_string[3],"Move Step %d", step);
      sprintf(mMenu.menu_string[4],"Apply Size to All");
      sprintf(mMenu.menu_string[5],"Apply Color to All");
      sprintf(mMenu.menu_string[6],"Apply Flags to All");
      sprintf(mMenu.menu_string[7],"end");
         switch (mMenu.pmenu(6, 13))
         {
            case 3: move_lift_step(lift, step); break;
            case 4: set_all_steps(lift, step, 1); break;
            case 5: set_all_steps(lift, step, 2); break;
            case 6: set_all_steps(lift, step, 3); break;
         }
   }
   // special case for last step (don't allow move or delete, only insert)
   else if (step == cur[lift].num_steps-1)
   {
      strcpy (mMenu.menu_string[3],"Insert Steps");
      sprintf(mMenu.menu_string[4],"end");
      if (mMenu.pmenu(6, 13) == 3) insert_steps_until_quit(lift, step);
   }
   else // regular step (not first or last)
   {
      if ((stp[lift][step].type & 31) == 1) // only allow move for step type 1
      {
         sprintf(mMenu.menu_string[3],"Move Step %d", step);
         sprintf(mMenu.menu_string[4],"Delete Step %d", step);
         sprintf(mMenu.menu_string[5],"Insert Steps");
         sprintf(mMenu.menu_string[6],"Get Size From Prev");

         sprintf(mMenu.menu_string[7],"Apply Size to All");
         sprintf(mMenu.menu_string[8],"Apply Color to All");
         sprintf(mMenu.menu_string[9],"Apply Flags to All");




         sprintf(mMenu.menu_string[10],"end");
         switch (mMenu.pmenu(6, 13))
         {
            case 3: move_lift_step(lift, step); break;
            case 4: delete_lift_step(lift, step); break;
            case 5: insert_steps_until_quit(lift, step); break;
            case 6: set_size_from_previous_step(lift, step); break;
            case 7: set_all_steps(lift, step, 1); break;
            case 8: set_all_steps(lift, step, 2); break;
            case 9: set_all_steps(lift, step, 3); break;
         }
      }
      else
      {
         sprintf(mMenu.menu_string[3],"Delete Step %d", step);
         sprintf(mMenu.menu_string[4],"Insert Steps");
         sprintf(mMenu.menu_string[5],"end");
         switch (mMenu.pmenu(6, 13))
         {
            case 3: delete_lift_step(lift, step); break;
            case 4: insert_steps_until_quit(lift, step); break;
         }
      }
   }
}

int mwLift::draw_current_step_buttons(int x1, int x2, int y, int l, int s, int d)
{
   char msg[1024];
   int bts = 16; // button height
   int fs = 14;  // frame_size
   int xa = x1+fs;
   int xb = x2-fs;
   int ya = y+fs;
   int c1 = 9;




   // step details text header button
   int sd = 10; // color
   switch (stp[l][s].type & 31)
   {
      case 1: // move and resize
         sprintf(msg, "Step:%d - Move and Resize", s);
         mWidget.buttont(xa, ya, xb, bts,  0,0,0,0, 0,sd,15,0, 1,0,1,d, msg);
      break;
      case 2: // wait time
         sprintf(msg, "Step:%d - Wait For Timer", s);
         mWidget.buttont(xa, ya, xb, bts,  0,0,0,0, 0,sd,15,0, 1,0,1,d, msg);
      break;
      case 3: // wait prox
         sprintf(msg, "Step:%d - Wait For Player Proximity", s);
         mWidget.buttont(xa, ya, xb, bts,  0,0,0,0, 0,sd,15,0, 1,0,1,d, msg);
      break;
      case 4: // end step
         sprintf(msg, "Step:%d - Ending Step", s);
         mWidget.buttont(xa, ya, xb, bts,  0,0,0,0, 0,sd,15,0, 1,0,1,d, msg);
      break;
      case 5: // wait event
         sprintf(msg, "Step:%d - Wait For Event", s);
         mWidget.buttont(xa, ya, xb, bts,  0,0,0,0, 0,sd,15,0, 1,0,1,d, msg);
      break;
      case 6: // send event
         sprintf(msg, "Step:%d - Send Event", s);
         mWidget.buttont(xa, ya, xb, bts,  0,0,0,0, 0,sd,15,0, 1,0,1,d, msg);
      break;
   }

   // default buttons
   int col = (stp[l][s].type >> 28) & 15;
   sprintf(msg, "Step Color:%d", col);
   mWidget.buttont(xa, ya, xb, bts, 0,0,0,0,  0, col, 15, 0,   1,0,1,d, msg);

   mWidget.colsel(xa, ya, xb, bts, 8,l,s,0,  0,15,13,14,   0,0,1,d); // lift step color

   int dim = 32;
   int c2 = 14;
   int c3 = 15;

   mWidget.togglf(xa, ya, xb, bts, 0,0,0,0, 0,0,0,0, 1,0,1,d, stp[l][s].type, PM_LIFT_NO_DRAW,      "Draw Lift",            "Hide Lift",            c3, c3+dim, c2, c2+dim);
   mWidget.togglf(xa, ya, xb, bts, 0,0,0,0, 0,0,0,0, 1,0,1,d, stp[l][s].type, PM_LIFT_SOLID_PLAYER, "Solid for Player:OFF", "Solid for Player:ON ", c3+dim, c3, c2+dim, c2);
   mWidget.togglf(xa, ya, xb, bts, 0,0,0,0, 0,0,0,0, 1,0,1,d, stp[l][s].type, PM_LIFT_SOLID_ENEMY,  "Solid for Enemy:OFF ", "Solid for Enemy:ON  ", c3+dim, c3, c2+dim, c2);
   mWidget.togglf(xa, ya, xb, bts, 0,0,0,0, 0,0,0,0, 1,0,1,d, stp[l][s].type, PM_LIFT_SOLID_ITEM,   "Solid for Item:OFF  ", "Solid for Item:ON   ", c3+dim, c3, c2+dim, c2);
   mWidget.togglf(xa, ya, xb, bts, 0,0,0,0, 0,0,0,0, 1,0,1,d, stp[l][s].type, PM_LIFT_HIDE_LINES,   "Draw Lift Lines",      "Hide Lift Lines",      c3, c3+dim, c2, c2+dim);




   // specific buttons
   switch (stp[l][s].type & 31)
   {
      case 1: // move and resize
         mWidget.slideri(xa, ya, xb, bts,  0,0,0,0,   0,c1,15,15, 1,0,1,d, stp[l][s].val, 1000, 1, 1, "Speed:");
         mWidget.slideri(xa, ya, xb, bts,  0,0,0,0,   0,c1,15,15, 1,0,1,d, stp[l][s].w,   1600, 20, 1, "Width:");
         mWidget.slideri(xa, ya, xb, bts,  0,0,0,0,   0,c1,15,15, 1,0,1,d, stp[l][s].h,   1600, 20, 1, "Height:");
      break;
      case 2: // wait time
         mWidget.slideri(xa, ya, xb, bts,  0,0,0,0,   0,c1,15,15, 1,0,1,d, stp[l][s].val, 2000, 1, 1, "Timer:");
      break;
      case 3: // wait prox
         mWidget.slideri(xa, ya, xb, bts,  0,0,0,0,   0,c1,15,15, 1,0,1,d, stp[l][s].val, 200, 20, 10, "Distance:");
      break;
      case 4: // end step
         mWidget.button( xa, ya, xb, bts,  505,l,s,0, 0,c1,15,0,  1,0,1,d); // lift step end step mode
      break;
      case 5: // wait trigger
         mWidget.slideri(xa, ya, xb, bts,  0,0,0,0,   0,c1,15,15, 1,0,1,d, stp[l][s].val, 99, 0, 1, "Event:");
     if (mWidget.buttont(xa, ya, xb, bts,  0,0,0,0,   0,c1,15,0,  1,0,1,d, "Set Trigger")) mTriggerEvent.find_event_sender_for_obj(4, l, s, 0);
      break;
      case 6: // send trigger
         mWidget.slideri(xa, ya, xb, bts,  0,0,0,0,   0,c1,15,15, 1,0,1,d, stp[l][s].val, 99, 0, 1, "Event:");
      break;
   }

   // frame and title
   int y1 = y;
   int y2 = ya-bts+(fs*2);
   for (int q=0; q<fs; q++)
      al_draw_rectangle(x1+q, y1+q, x2-q, y2-q, mColor.pc[10+(q*16)], 1);
   al_draw_text(mFont.pr8, mColor.pc[15], (x1+x2)/2, y1+2, ALLEGRO_ALIGN_CENTER, "Current Step Details");
   return y2-y1+2; // return how much y space was used
}

void mwLift::draw_step_button(int xa, int xb, int ty1, int ty2, int l, int s, int rc, int d)
{
   int t = stp[l][s].type & 31;
   int c = (stp[l][s].type >> 28) & 15;
   if (stp[l][s].type & PM_LIFT_NO_DRAW) c = 0;
   int x1 = xa;
   int x2 = x1 + 26; // first column (step number) is fixed size
   int x3 = x2 + 26; // second column (color)   is fixed size
   int x4 = x3 + 42; // third  column (type)   is fixed size
   if (s == -1) // show row headers
   {
      mWidget.button(x1, ty1, x2, ty2-ty1, 501, -1, 0, 0,  0,rc,15,0,  1,0,0,d); // num
      mWidget.button(x2, ty1, x3, ty2-ty1, 506, -1, 0, 0,  0,rc,15,0,  1,0,0,d); // 'C'
      mWidget.button(x3, ty1, x4, ty2-ty1, 502, -1, 0, 0,  0,rc,15,0,  1,0,0,d); // type
      mWidget.button(x4, ty1, xb, ty2-ty1, 503, -1, 0, 0,  0,rc,15,0,  1,0,0,d); // description
   }
   else
   {
      mWidget.button(x1, ty1, x2, ty2-ty1, 501, s, 0, 0,   0,rc,15,0,  1,0,0,d); // num
      mWidget.button(x2, ty1, x3, ty2-ty1, 506, c, 0, 0,   0, c,15,0,  1,0,0,d); // color
      mWidget.button(x3, ty1, x4, ty2-ty1, 502, t, 0, 0,   0,rc,15,0,  1,0,0,d); // type
      mWidget.button(x4, ty1, xb, ty2-ty1, 503, t, l, s,   0,rc,15,0,  1,1,0,d); // description
   }
}

int mwLift::draw_steps(int x1, int x2, int y, int lift, int current_step, int highlight_step, int d)
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
   for (int step=0; step<cur[lift].num_steps; step++)
   {
      int color = 13;
      if (step == current_step) color = 10;
      draw_step_button(xa, xb, ya+(a)*bts, ya+(a+1)*bts-1, lift, step, color, d); a++;


   }

   // show outline around highlighted step
   int hs = highlight_step+1;
   if (hs > -1) al_draw_rounded_rectangle(xa, ya+hs*bts, xb, ya+(hs+1)*bts-3, 2, 2, mColor.pc[14], 2);

   int y1 = y;
   int y2 = y1+a*bts+fs*2;
   int ci = 16; //color inc
   for (int q=0; q<fs; q++)
      al_draw_rectangle(x1+q, y1+q, x2-q, y2-q, mColor.pc[12+32+(q*ci)], 1);
   al_draw_text(mFont.pr8, mColor.pc[15], (x1+x2)/2, y1+2, ALLEGRO_ALIGN_CENTER, "List of Steps");

   return fs*2 + (cur[lift].num_steps + 1) * bts; // return how much y space was used
}


void mwLift::set_lift_to_step(int l, int s)
{
   // set current step to passed step
   // set current flags from passed step
   // set current incs to zero

   // if passed step is a move step:
   // - use that step to set lift xywh
   // - set limit and limit counter to immediately advance to next step

   // if passed step is not a move step:
   // - find prev move step and use that to set lift xywh
   // - set limit and limit counter from passed step

   cur[l].current_step = s;
   cur[l].flags = stp[l][s].type; // get all the flags from the step type

   // set incs to 0
   cur[l].xinc = 0;
   cur[l].yinc = 0;
   cur[l].winc = 0;
   cur[l].hinc = 0;

   int step_type = stp[l][s].type & 31;
   if ((step_type) == 1)          // if step is a move step
   {
      cur[l].limit_type    = 2;   // type 2 - wait for time
      cur[l].limit_counter = 0;   // no wait, immediate next mode
   }
   else
   {
      cur[l].limit_type    = step_type;
      cur[l].limit_counter = stp[l][s].val;
      s = find_previous_move_step(l, s);
   }
   // get pos and size from step
   cur[l].x = stp[l][s].x;
   cur[l].y = stp[l][s].y;
   cur[l].w = stp[l][s].w;
   cur[l].h = stp[l][s].h;
}


int mwLift::is_player_riding_lift(int l)
{
   for (int p=0; p<NUM_PLAYERS; p++)
      if ((mPlayer.syn[p].active) && (!mPlayer.syn[p].paused))
         if ((mPlayer.syn[p].player_ride) && (l == mPlayer.syn[p].player_ride - 32)) return p+1; // player is riding this lift
   return 0;
}

void mwLift::draw_lift_line(int l)
{
   if ((!(cur[l].flags & PM_LIFT_NO_DRAW)) || (mLoop.level_editor_running))
   {
      int col = 15;
      int sx = stp[l][0].x + stp[l][0].w / 2;  // start pos
      int sy = stp[l][0].y + stp[l][0].h / 2;
      int px = sx; // previous
      int py = sy;
      int nx = 0;  // next
      int ny = 0;
      if (cur[l].num_steps > 1) // only draw lines if more than one step
      {
         for (int s=0; s<cur[l].num_steps; s++) // cycle steps
         {



            if ((stp[l][s].type & 31) == 1) // filter for move steps
            {
               nx = stp[l][s].x + stp[l][s].w / 2;
               ny = stp[l][s].y + stp[l][s].h / 2;


               if (!(stp[l][s].type & PM_LIFT_HIDE_LINES))
               {
                  col = (stp[l][s].type >> 28) & 15;
                  al_draw_line( px, py, nx, ny, mColor.pc[col], 1);
                  if (mLoop.eco_draw)
                  {
                     // al_draw_filled_circle(nx, ny, 2, mColor.pc[col]);
                  }
                  else
                  {
                     for (int c=3; c>=0; c--)
                        al_draw_filled_circle(nx, ny, c, mColor.pc[(col - 96) + c*48]);
                  }
               }

               px = nx;
               py = ny;
            }
         }
         if (!(stp[l][0].type & PM_LIFT_HIDE_LINES))
         {
            col = (stp[l][0].type >> 28) & 15;
            al_draw_line(sx, sy, nx, ny, mColor.pc[col], 1); // draw line from last to first
         }
      }
   }
}

// this is it....the one base function that draws a lift
void mwLift::draw_lift(int l, int x1, int y1, int x2, int y2)
{
   int col = (cur[l].flags >> 28) & 15;
   int tc = col + 160;
   //int tc = mColor.get_contrasting_color(col);
   ALLEGRO_FONT * f = mFont.pr8;


   if ((cur[l].flags & PM_LIFT_NO_DRAW) && (mLoop.level_editor_running)) col = 0;


   if (mLoop.eco_draw)
   {
      al_draw_filled_rectangle(x1, y1, x2, y2, mColor.pc[col]);
      //al_draw_filled_rounded_rectangle(x1, y1, x2, y2, 4, 4, mColor.pc[col] );
      al_draw_text(f, mColor.pc[tc], (x1+x2)/2, (y1+y2)/2 - 3, ALLEGRO_ALIGN_CENTRE, cur[l].lift_name); // name
   }
   else
   {
      int xs = x2-x1; // x size
      int ys = y2-y1; // y size
      int ms = xs;    // min size
      if (ys < xs) ms = ys;
      int fb = 10;    // fade amount
      if (ms < 20) fb = ms/2;

      al_draw_filled_rectangle(x1+fb, y1+fb, x2-fb, y2-fb, mColor.pc[col] );                         // solid core
      for (int a=0; a<fb; a++)
        al_draw_rounded_rectangle(x1+a, y1+a, x2-a, y2-a, 4, 4, mColor.pc[col + ((9 - a)*16)], 2 ); // faded outer shell

      al_draw_text(f, mColor.pc[tc], (x1+x2)/2, (y1+y2)/2 - 4, ALLEGRO_ALIGN_CENTRE, cur[l].lift_name); // name
   }

   if (mLoop.level_editor_running)
   {
      al_draw_textf(mFont.pixl, mColor.pc[15], (x1+x2)/2, y1-20, ALLEGRO_ALIGN_CENTRE, "x:%-4.0f y:%-4.0f", cur[l].x, cur[l].y);
      al_draw_textf(mFont.pixl, mColor.pc[15], (x1+x2)/2, y1-12, ALLEGRO_ALIGN_CENTRE, "w:%-4.0f h:%-4.0f", cur[l].w, cur[l].h);
   }

   //printf("x1:%d y1:%d x2:%d y2:%d\n", x1, y1, x2, y2);
   //al_draw_textf(mFont.pr8, mColor.pc[col+160], (x1+x2)/2, (y1+y2)/2 - 3, ALLEGRO_ALIGN_CENTRE, "s:%d v:%d", cur[l].current_step, cur[l].val1);    // debug name
   //al_draw_textf(mFont.pr8, mColor.pc[15], (x1+x2)/2, (y1+y2)/2 - 16, ALLEGRO_ALIGN_CENTRE, "s:%d v:%d", cur[l].current_step, cur[l].val1);    // debug name
}

void mwLift::draw_lifts()
{
   al_set_target_bitmap(mBitmap.level_buffer);

   for (int l=0; l<NUM_LIFTS; l++)
      if (cur[l].active)
      {
         draw_lift_line(l);

         if ((!(cur[l].flags & PM_LIFT_NO_DRAW)) || (mLoop.level_editor_running))
         {
            int color = (cur[l].flags >> 28) & 15;
            int x1 = cur[l].x;
            int x2 = cur[l].x + cur[l].w;
            int y1 = cur[l].y;
            int y2 = cur[l].y + cur[l].h;
            draw_lift(l, x1, y1, x2, y2);


            // show if player is riding this lift
            int p = is_player_riding_lift(l);
            if (p)
            {
               p -=1; // player number
               int pc = mPlayer.syn[p].color;
               if (pc == color) pc = 127;
               al_draw_rounded_rectangle(x1, y1, x2, y2, 4, 4, mColor.pc[pc], 2);
            }

            if ((cur[l].mode) && (!is_player_riding_lift(l)))
            {
               // if not in initial position
               if (!((cur[l].x == stp[l][0].x) && (cur[l].y == stp[l][0].y)))
               {
                  int lw = cur[l].w-10;
                  int lh = cur[l].h-10;
                  if (cur[l].val2 == 0) cur[l].val2 = 1; // to prevent divide by zero
                  int percent = (100 * cur[l].val1) / cur[l].val2;
                  mScreen.draw_percent_bar((x1+x2)/2, y1+4, lw, lh, percent);
               }
            }

            int cs = cur[l].current_step;
            if (!(stp[l][cs].type & PM_LIFT_HIDE_LINES))
            {
               switch (cur[l].limit_type) // limit type
               {
                  case 2: // timer wait
                     if (cur[l].limit_counter > 0)
                        al_draw_textf(mFont.pr8, mColor.pc[color+64], (x1 + x2)/2, cur[l].y - 9, ALLEGRO_ALIGN_CENTRE, "%d", cur[l].limit_counter);
                  break;
                  case 3: // prox wait
                  {
                     int pd = cur[l].limit_counter; // prox dist
                     int bx1 = x1 - pd;
                     int by1 = y1 - pd;
                     int bx2 = x2 + pd;
                     int by2 = y2 + pd;
                     al_draw_rectangle(bx1+10, by1+10, bx2-10, by2-10, mColor.pc[color+128], 1);
                  }
                  break;
               }
            }
         }
      }  // end of iterate lifts
}


void mwLift::set_lift_xyinc(int l, int step)
{
   //  used when switching to a new move step;
   //  sets xinc, yinc and num of frames for move
   int val = stp[l][step].val;
   float move_time = 0;
   float xlen = stp[l][step].x - cur[l].x;             // x length
   float ylen = stp[l][step].y - cur[l].y;             // y length
   float hy_dist = sqrt(pow(xlen, 2) + pow(ylen, 2));  // hypotenuse distance

   if (hy_dist < 1)      // no move
   {
      move_time = val;   // use val as straight timer
      cur[l].xinc = 0;   // no xinc
      cur[l].yinc = 0;   // no yinc
   }
   else // get time based on speed and distance
   {
      float speed = (float) val / 10;      // stored speed scaled by 10
      move_time = hy_dist / speed;         // get time (time = distance / speed)
      cur[l].xinc = xlen / move_time;      // set xinc
      cur[l].yinc = ylen / move_time;      // set yinc
   }

   float wd = stp[l][step].w - cur[l].w;   // width dif
   float hd = stp[l][step].h - cur[l].h;   // height dif
   cur[l].winc = wd / move_time;           // set winc
   cur[l].hinc = hd / move_time;           // set hinc

   cur[l].current_step = step;

   cur[l].limit_counter = move_time;
   cur[l].limit_type = 1;
}

int mwLift::lift_check_prox(int l, int pd)
{
   // check if player is within lift proximity
   float bx1 = cur[l].x - pd - 10;
   float by1 = cur[l].y - pd - 10;
   float bx2 = cur[l].x + cur[l].w + pd - 10;
   float by2 = cur[l].y + cur[l].h + pd - 10;

   for (int p=0; p<NUM_PLAYERS; p++)
      if ((mPlayer.syn[p].active) && (!mPlayer.syn[p].paused))
         if ((mPlayer.syn[p].x > bx1) && (mPlayer.syn[p].x < bx2) && (mPlayer.syn[p].y > by1) && (mPlayer.syn[p].y < by2)) return 1;
   return 0;
}


void mwLift::next_step(int l)
{
   if (++cur[l].current_step > cur[l].num_steps - 1) cur[l].current_step = cur[l].num_steps -1; // increment step with bounds check
   int s = cur[l].current_step;
   cur[l].flags = stp[l][s].type; // set current lift flags from step type flags

   int step_type = stp[l][s].type & 31; // step type
   int step_val  = stp[l][s].val;       // step val

   switch (step_type)
   {
      case 1: // move
         set_lift_xyinc(l, s);
      break;
      case 2: // wait time
         cur[l].limit_type = 2; // wait time
         cur[l].limit_counter = step_val;
      break;
      case 3: // wait prox
         cur[l].limit_type = 3; // wait prox
         cur[l].limit_counter = step_val;
      break;
      case 5: // wait trig
         cur[l].limit_type = 5; // wait trig
         cur[l].limit_counter = step_val;
      break;
      case 6: // send trig
         mTriggerEvent.event[step_val] = 1; // set event
         next_step(l); // recurse to immediately jump to next step

         // if now in move step, do initial move
         if ((stp[l][cur[l].current_step].type & 31) == 1)
         {
            cur[l].x += cur[l].xinc;
            cur[l].y += cur[l].yinc;
            cur[l].w += cur[l].winc;
            cur[l].h += cur[l].hinc;
            --cur[l].limit_counter;
         }

      break;
      case 4: // end step
         if (step_val == 0) set_lift_xyinc(l, 0);   // move to step 0
         if (step_val == 1) set_lift_to_step(l, 0); // warp to step 0
         if (step_val == 2) cur[l].limit_type = 0;  // freeze here
      break;
   }
}

void mwLift::move_lifts(int ignore_prox)
{
   for (int l=0; l<NUM_LIFTS; l++)
      if (cur[l].active)
      {
         // find and clear all events this lift might send
         for (int s=0; s<cur[l].num_steps; s++)
            if ((stp[l][s].type & 31) == 6)  // only step type 6 - send trigger event
               mTriggerEvent.event[stp[l][s].val] = 0;

         int frozen = 0;
         if (cur[l].mode == 1) // prox run and reset mode
         {
            if (is_player_riding_lift(l)) cur[l].val1 = cur[l].val2; // reset timer
            else
            {
               frozen = 1;
               if (--cur[l].val1 < 0)
               {
                  cur[l].val1 = cur[l].val2; // reset timer
                  set_lift_to_step(l, 0);
               }
            }
         }
         if (cur[l].mode == 2) // prox reset mode
         {
            if (is_player_riding_lift(l)) cur[l].val1 = cur[l].val2; // reset timer
            else
            {
               if (--cur[l].val1 < 0)
               {
                  cur[l].val1 = cur[l].val2; // reset timer
                  set_lift_to_step(l, 0);
               }
            }
         }

         if (!frozen)
         {
            // limits ----------------------------------------------------------------------
            switch (cur[l].limit_type) // limit type
            {
               case 1: // step type 1 - move and resize

                  cur[l].x += cur[l].xinc;
                  cur[l].y += cur[l].yinc;
                  cur[l].w += cur[l].winc;
                  cur[l].h += cur[l].hinc;

                  if (--cur[l].limit_counter < 0) // run down and check counter
                  {
                     // make sure lift is at exact position at the end of the move
                     int s = cur[l].current_step;
                     cur[l].x = stp[l][s].x;
                     cur[l].y = stp[l][s].y;
                     cur[l].w = stp[l][s].w;
                     cur[l].h = stp[l][s].h;

                     // zero incs
                     cur[l].xinc = 0;
                     cur[l].yinc = 0;
                     cur[l].winc = 0;
                     cur[l].hinc = 0;

                     next_step(l);
                  }
               break;
               case 2: // timer wait
                  if (--cur[l].limit_counter < 0) next_step(l);
               break;
               case 3: // prox wait
                  if (ignore_prox) next_step(l);
                  else if (lift_check_prox(l, cur[l].limit_counter)) next_step(l);
               break;
               case 5: // trigger wait
                  if (mTriggerEvent.event[cur[l].limit_counter]) next_step(l);
               break;

            }
         }
      }
}
























