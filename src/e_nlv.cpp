// e_nlv.cpp (20100220 cleanup)

#include "pm.h"

char fst[80];
void edit_server_name(void)
{
   strcpy(fst, m_serveraddress);
   int char_count = strlen(fst);
   int cursor_pos=0;
   int old_cp=0;
   int blink_count = 3;
   int blink_counter = 0;
   while (key[ALLEGRO_KEY_ENTER]) proc_controllers();
   int quit = 0;
   while (!quit)
   {
      int tx = SCREEN_W/2;
      int ty = SCREEN_H/2;
      int w = (char_count+1)*4;

      al_flip_display();
      // clear text background
      al_draw_filled_rectangle(tx-w-8, ty-4-2, tx+w+18, ty+4+3, palette_color[0]);

      al_draw_text(font, palette_color[15], tx, ty-14, ALLEGRO_ALIGN_CENTER, "Set Server IP or Hostname");
      // frame text
      al_draw_rectangle       (tx-w-1, ty-4-1, tx+w+6, ty+6, palette_color[15], 1);

      rtextout_centre(NULL, fst, tx, ty+1, 15, 1, 0, 1);

      if (blink_counter++ < blink_count) show_cursor(fst, cursor_pos, tx, ty-3, 15, 0, 0);
      else show_cursor(fst, cursor_pos, tx, ty-3, 15, 1, 0);
      if (blink_counter> blink_count*2) blink_counter = 0;

      if (cursor_pos != old_cp)
      {
         show_cursor(fst, old_cp, tx, ty-3, 15, 1, 0); // erase old blinking cursor if moved
         old_cp = cursor_pos;
         blink_counter = 0;
      }

      al_rest(.08);
      int k = proc_controllers();
      if (key[ALLEGRO_KEY_RIGHT])
      {
         if (++cursor_pos > char_count) cursor_pos = char_count;
      }
      if (key[ALLEGRO_KEY_LEFT])
      {
         if (--cursor_pos < 0) cursor_pos = 0;
      }
      if ((key[ALLEGRO_KEY_DELETE]) && (cursor_pos < char_count))
      {
         for (int a = cursor_pos; a < char_count; a++)
           fst[a]=fst[a+1];
         --char_count;
         fst[char_count] = (char)NULL; // set last to NULL
      }
      if ((key[ALLEGRO_KEY_BACKSPACE]) && (cursor_pos > 0))
      {
         cursor_pos--;
         for (int a = cursor_pos; a < char_count; a++)
           fst[a]=fst[a+1];
         char_count--;
         fst[char_count] = (char)NULL; // set last to NULL
      }

      k = Key_pressed_ASCII;
      if ((k>31) && (k<127)) // insert if alphanumeric or return
      {
         // move over to make room
         for (int a = char_count; a>=cursor_pos; a--)
            fst[a+1]=fst[a];

         // set char
         fst[cursor_pos] = k;

         // inc both
         cursor_pos++;
         char_count++;

         fst[char_count] = (char)NULL; // set last to NULL
      }
      if (key[ALLEGRO_KEY_ENTER])
      {
         while (key[ALLEGRO_KEY_ENTER]) proc_controllers();
         strcpy(m_serveraddress, fst);
         quit = 1;
      }

      if (key[ALLEGRO_KEY_ESCAPE])
      {
         while (key[ALLEGRO_KEY_ESCAPE]) proc_controllers();
         quit = 1;
      }
   }
}


int edit_lift_name(int lift, int step_ty, int bts)
{
   int cursor_pos=0;
   int old_cp=0;
   int blink_count = 3;
   int blink_counter = 0;
   int a, k=0;
   int char_count = strlen(fst);
   while (1)
   {
      al_flip_display();
      //al_clear_to_color(al_map_rgb(0,0,0));
      al_rest(.1);

      int x1 = (SCREEN_H/100)*100+22;
      int x2 = x1 + (lifts[lift].width * 20) -1;
      int y1 = step_ty + (lifts[lift].num_steps+3) * bts; // only see in 2 highest screen modes
      int y2 = y1 + (lifts[lift].height * 20) -1;

      int tx = ((x1+x2)/2);
      int ty = ((y1+y2)/2);
      int w = (char_count+1) *4;

      int rot = 0;
      if ((lifts[lift].width == 1) && (lifts[lift].height > 1)) rot = 64;
      int color = lifts[lift].color;

      // clear text background
      if (rot == 64) al_draw_filled_rectangle(tx-4, ty-w, tx+4, ty+w, palette_color[0]);
      else           al_draw_filled_rectangle(tx-w, ty-4, tx+w, ty+4, palette_color[0]);

      for (a=0; a<10; a++)
         al_draw_rectangle(x1+a, y1+a, x2-a, y2-a, palette_color[color + ((9 - a)*16)], 1 );
      al_draw_filled_rectangle(x1+a, y1+a, x2-a, y2-a, palette_color[color] );
      rtextout_centre(NULL, fst, tx, ty+1, color+160, 1, rot, 1);

      if (blink_counter++ < blink_count) show_cursor(fst, cursor_pos, tx, ty-3, 15, 0, rot);
      else show_cursor(fst, cursor_pos, tx, ty-3, 15, 1, rot);
      if (blink_counter> blink_count*2) blink_counter = 0;

      if (cursor_pos != old_cp)
      {
         show_cursor(fst, old_cp, tx, ty-3, 15, 1, rot); // erase old blinking cursor if moved
         old_cp = cursor_pos;
         blink_counter = 0;
      }

      k = proc_controllers();
      if (key[ALLEGRO_KEY_RIGHT])
      {
         if (++cursor_pos >= char_count) cursor_pos = char_count-1;
      }
      if (key[ALLEGRO_KEY_LEFT])
      {
         if (--cursor_pos < 0) cursor_pos = 0;
      }
      if ((key[ALLEGRO_KEY_DELETE]) && (cursor_pos < char_count))
      {
         for (a = cursor_pos; a < char_count; a++)
           fst[a]=fst[a+1];
         char_count--;
         // set last to NULL
         fst[char_count] = (char)NULL;
      }
      if ((key[ALLEGRO_KEY_BACKSPACE]) && (cursor_pos > 0))
      {
         cursor_pos--;
         for (a = cursor_pos; a < char_count; a++)
           fst[a]=fst[a+1];
         char_count--;
         // set last to NULL
         fst[char_count] = (char)NULL;
      }

      k = Key_pressed_ASCII;
      if ((k>31) && (k<127)) // insert if alphanumeric or return
      {
         // move over to make room
         for (a = char_count; a>=cursor_pos; a--)
            fst[a+1]=fst[a];

         // set char
         fst[cursor_pos] = k;

         // inc both
         cursor_pos++;
         char_count++;

         // set last to NULL
         fst[char_count] = (char)NULL;
      }
      if (key[ALLEGRO_KEY_ENTER]) return 1;
      if (key[ALLEGRO_KEY_ESCAPE]) return 0;
   }
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
      int w    = lifts[l].width;
      int h    = lifts[l].height;
      int col  = lifts[l].color;
      sprintf(msg,"lift:%-2d w:%-2d h:%-2d col:%-2d ns:%-2d  name:%s  ", l, w, h, col, lifts[l].num_steps, lifts[l].lift_name);
      al_draw_text(font, palette_color[10], 10, text_pos*8, 0, msg);
      text_pos++;

      int x1 = lifts[l].x1;
      int y1 = lifts[l].y1;
      int x2 = lifts[l].x2;
      int y2 = lifts[l].y2;
      sprintf(msg,"x1:%d y1:%d x2:%d y2:%d", x1, y1, x2, y2);
      al_draw_text(font, palette_color[10], 10, text_pos*8, 0, msg);
      text_pos++;

      for (int s=0; s<lifts[l].num_steps; s++) // iterate steps
      {
         int x    = lift_steps[l][s].x;
         int y    = lift_steps[l][s].y;
         int val  = lift_steps[l][s].val;
         int type = lift_steps[l][s].type;
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

         sprintf(msg," step:%-2d x:%-4d y:%-4d val:%-4d type:%d (%s)", s, x, y, val, type, typemsg );
         al_draw_text(font, palette_color[color], 10, text_pos*8, 0, msg);
         text_pos++;
      }
   }
   al_flip_display();
   tsw(); // wait for keypress
}

void erase_lift(int lift)
{
   for (int a=lift; a<num_lifts; a++) // slide down to close hole in array
   {
      lifts[a] = lifts[a+1];
      for (int b=0; b<40; b++)
         lift_steps[a][b] = lift_steps[a+1][b];
   }
   clear_lift(num_lifts);                                  // clear last lift
   for (int a=0; a<40; a++) clear_lift_step(num_lifts, a); // clear last lift's steps
   num_lifts--;                                            // one less lift
}

void delete_lift_step(int lift, int step)
{
   for (int x=step; x<lifts[lift].num_steps-1; x++)   // slide all down
   {
      lift_steps[lift][x].x    = lift_steps[lift][x+1].x ;
      lift_steps[lift][x].y    = lift_steps[lift][x+1].y ;
      lift_steps[lift][x].val  = lift_steps[lift][x+1].val ;
      lift_steps[lift][x].type = lift_steps[lift][x+1].type ;
   }
   lifts[lift].num_steps--;
}

void lift_setup(void)
{
   // set all lifts to step 0
   for (int d=0; d<num_lifts; d++) set_lift(d, 0);
}

void draw_step_button(int xa, int xb, int ty, int ty2, int lift, int step, int rc)
{
   switch (lift_steps[lift][step].type)
   {
      case 1: mdw_slider(xa, ty, xb, ty2, 71, step, lift, 0, 0, rc, 15, 15, 1,0,0,0); break;
      case 2: mdw_slider(xa, ty, xb, ty2, 72, step, lift, 0, 0, rc, 15, 15, 1,0,0,0); break;
      case 3: mdw_slider(xa, ty, xb, ty2, 73, step, lift, 0, 0, rc, 15, 15, 1,0,0,0); break;
      case 4: mdw_button(xa, ty, xb, ty2, 74, step, lift, 0, 0, rc, 15, 0,  1,0,0,0); break;
   }
}

void draw_steps(int step_ty, int lift, int current_step, int highlight_step)
{
   int ty  = step_ty;
   int jh = 0;

//   int xa = txc-98;
//   int xb = txc+96;
   int xa = SCREEN_W-(SCREEN_W-(db*100))+1+10;
   int xb = SCREEN_W-3-10;

   // faded title bar
   title((char *)"List of Steps", ty + jh*bts,  15,  12 );

   // title button
   mdw_button(xa, ty+(jh+1)*bts, xb, ty+(jh+2)*bts-1, 70, 0, 0, 0, 0, 8, 15, 0, 1,0,0,0);

   // draw steps
   for (int step=0; step<lifts[lift].num_steps; step++)
      if (lift_steps[lift][step].type) // step is valid
      {
         int color = 3;
         if (step == current_step) color = 13;
         if (step == highlight_step) color = 12;
         draw_step_button(xa, xb, ty+(jh+2+step)*bts, ty+(jh+3+step)*bts-1, lift, step, color);
      }
}

void highlight_current_lift(int lift)
{
   int color = lifts[lift].color;
   int x3 = lifts[lift].width * 20;
   int y3 = lifts[lift].height * 20;
   int x1 = lifts[lift].x1 + 4;
   int y1 = lifts[lift].y1 + 4;
   int x2 = x1 + x3-4;
   int y2 = y1 + y3-4;

   al_set_clipping_rectangle(1, 1, les*db*100-2, les*db*100-2);

   // mark current lift with crosshairs and rect
   al_draw_line(((x1+x2)/2)*db/20, 1, ((x1+x2)/2)*db/20, db*100-2, palette_color[10], 1);
   al_draw_line(1, ((y1+y2)/2)*db/20, db*100-2, ((y1+y2)/2)*db/20,  palette_color[10], 1);
   al_draw_rectangle((x1*db/20)-1, (y1*db/20)-1, (x2*db/20)+1, (y2*db/20)+1, palette_color[15], 1);

   // draw lift
   int a;
   for (a=0; a<10; a++)
      al_draw_rectangle((x1+a)*db/20, (y1+a)*db/20, (x2-a)*db/20, (y2-a)*db/20, palette_color[color+ (9-a)*16], 1 );
   al_draw_filled_rectangle((x1+a)*db/20, (y1+a)*db/20, (x2-a)*db/20, (y2-a)*db/20, palette_color[color] );

   if ((lifts[lift].width == 1) && (lifts[lift].height > 1)) // rotate lift name for vertical lifts
      rtextout_centre(NULL, lifts[lift].lift_name, ((x1+x2)/2)*db/20, ((y1+y2)/2)*db/20, color+160, (float)db/20, 64, 1);
   else rtextout_centre(NULL, lifts[lift].lift_name, ((x1+x2)/2)*db/20, ((y1+y2)/2)*db/20, color+160, (float)db/20, 0, 1);

   al_reset_clipping_rectangle();

}

void draw_lift_mp(int lift) // draws the current lift on mp
{
   int a, d = lift;
   int color = lifts[d].color;
   int szx   = lifts[d].width;
   int szy   = lifts[d].height;

   ALLEGRO_BITMAP *mp_big = NULL;
   mp_big = al_create_bitmap(szx*20, szy*20);
   al_set_target_bitmap(mp_big);
   al_clear_to_color(al_map_rgb(0,0,0));

   for (a=0; a<10; a++)
      al_draw_rectangle(a, a, (lifts[d].width*20)-1-a, (lifts[d].height*20)-1-a, palette_color[color+((9-a)*16)], 1 );
   al_draw_filled_rectangle(a, a, (lifts[d].width*20)-1-a, (lifts[d].height*20)-1-a, palette_color[color]);

   if ((lifts[d].width == 1) && (lifts[d].height > 1)) // rotate lift name for vertical lifts
      rtextout_centre(mp_big, lifts[d].lift_name, (lifts[d].width*10)-2, (lifts[d].height*10)-2, color+160, 1, 64, 1);
   else al_draw_text(font, palette_color[color+160], (lifts[d].width*10)-2, (lifts[d].height*10)-2, 0, lifts[d].lift_name);

   al_set_target_bitmap(mp);
   al_clear_to_color(al_map_rgb(0,0,0));
   al_draw_scaled_bitmap(mp_big, 0, 0, szx*20, szy*20, 0, 0, szx*db, szy*db, 0);
   al_destroy_bitmap(mp_big);
}



int create_lift(void)
{
   int step = 0;
   if (num_lifts < NUM_LIFTS)
   {
      num_lifts++;
      int lift = num_lifts-1;

      sprintf(msg, "new lift %d", lift);
      construct_lift(lift, msg, 6, 1, 10, 1);
      construct_lift_step(lift, step, 0, 0, 20, 1);

      if (getxy((char *)"Set initial position", 4, lift, step) == 1)
      {
         // set step 0 and main x y
         lifts[lift].x1 = lift_steps[lift][step].x = get100_x * 20;
         lifts[lift].y1 = lift_steps[lift][step].y = get100_y * 20;

         // set size
         lifts[lift].x2 = lifts[lift].x1 + (lifts[lift].width*20)-1;
         lifts[lift].y2 = lifts[lift].y1 + (lifts[lift].height*20)-1;

         step++;
         construct_lift_step(lift, step, 0, 0, 20, 4); // type 4 - loop to step zero
         lifts[lift].num_steps++; // add one to steps

         set_lift(lift, 0); // set step 0 for lift
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
   if (lift_steps[lift][step].type == 1) // only if type = move
   {
      int nx = ((lift_steps[lift][step].x + lifts[lift].width  * 10) *db)/20;
      int ny = ((lift_steps[lift][step].y + lifts[lift].height * 10) *db)/20;
      al_set_mouse_xy(display, nx*les, ny*les);
      getxy((char *)"Set new location", 4,  lift, step);
   }
}


void redraw_get_new_lift_step_menu(int sty, int step, int highlight)
{
   int fc = 14; // frame color
   int tc = 15; // text color
   int bc = 6;  // button color
   int hc = 10; // highlight frame color

   // erase to background color
   al_draw_filled_rectangle(txc-98, sty-8, txc+96, sty+(7*12)-22, palette_color[fc+192]);

   al_draw_rectangle(txc-98, sty-8, txc+96, sty+3, palette_color[fc], 1); // frame
   al_draw_textf(font, palette_color[fc], txc, sty-6, ALLEGRO_ALIGN_CENTER, "Insert New Step %d", step);

   al_draw_rectangle(txc-98, sty-8, txc+96, sty+(7*12)-22, palette_color[fc], 1); // frame
   al_draw_text(font, palette_color[tc], txc, sty+5, ALLEGRO_ALIGN_CENTER, "Select Step Type");

   int jh = 1;
   al_draw_filled_rectangle(txc-21, sty+(jh*12)+2, txc+18, sty+(jh*12)+13, palette_color[bc+128]);
   al_draw_text(font, palette_color[tc], txc, sty+(jh*12)+4, ALLEGRO_ALIGN_CENTER, "Move");
   al_draw_rectangle(txc-21, sty+(jh*12)+2, txc+18, sty+(jh*12)+13, palette_color[bc], 1);
   if (highlight == 1)
   al_draw_rectangle(txc-21, sty+(jh*12)+2, txc+18, sty+(jh*12)+13, palette_color[hc], 1);

   jh = 2;
   al_draw_filled_rectangle(txc-57, sty+(jh*12)+2, txc+54, sty+(jh*12)+13, palette_color[bc+128]);
   al_draw_text(font, palette_color[tc], txc, sty+(jh*12)+4, ALLEGRO_ALIGN_CENTER, "Wait For Time");
   al_draw_rectangle       (txc-57, sty+(jh*12)+2, txc+54, sty+(jh*12)+13, palette_color[bc], 1);
   if (highlight == 2)
   al_draw_rectangle       (txc-57, sty+(jh*12)+2, txc+54, sty+(jh*12)+13, palette_color[hc], 1);

   jh = 3;
   al_draw_filled_rectangle(txc-57, sty+(jh*12)+2, txc+54, sty+(jh*12)+13, palette_color[bc+128]);
   al_draw_text(font, palette_color[tc], txc, sty+(jh*12)+4, ALLEGRO_ALIGN_CENTER,"Wait For Prox" );
   al_draw_rectangle(txc-57, sty+(jh*12)+2, txc+54, sty+(jh*12)+13, palette_color[bc], 1);
   if (highlight == 3)
   al_draw_rectangle(txc-57, sty+(jh*12)+2, txc+54, sty+(jh*12)+13, palette_color[hc], 1);

   jh = 4;
   al_draw_filled_rectangle(txc-21, sty+(jh*12)+2, txc+18, sty+(jh*12)+13, palette_color[bc+128]);
   al_draw_text(font, palette_color[tc], txc, sty+(jh*12)+4, ALLEGRO_ALIGN_CENTER, "Done");
   al_draw_rectangle(txc-21, sty+(jh*12)+2, txc+18, sty+(jh*12)+13, palette_color[bc], 1);
   if (highlight == 4)
   al_draw_rectangle(txc-21, sty+(jh*12)+2, txc+18, sty+(jh*12)+13, palette_color[hc], 1);

}


int get_new_lift_step(int lift, int step)
{
   // step is a new blank step already created for us
   // if we return 99 the step will be erased

   // position the menu on top of the step we are inserting before
   int sty = 53 + (step + 9) * bts;
   if (sty > SCREEN_H-60) sty = SCREEN_H-60;

   // position the mouse
   al_set_mouse_xy(display, txc * les, (sty+24) * les);

   int quit = 0;
   while (!quit)
   {
      int selection = 0;
      al_rest(0.02);
      al_flip_display();
      proc_controllers();
      if ((mouse_b2) || (key[ALLEGRO_KEY_ESCAPE])) quit = 99;
      if ((mouse_x > txc-95) && (mouse_x < txc+95))
      {
         selection = ((mouse_y-sty-3)/12-1)+1;     // get selection based on mouse y
         if ((selection < 0 ) || (selection > 4)) selection = 0; // selection valid
         if (mouse_b1)
         {
            while(mouse_b1) proc_controllers(); // wait for release
            quit = 1;
            switch (selection)
            {
               case 1:
                  construct_lift_step(lift, step, 0, 0, 20, 1);              // set move step
                  if (getxy((char*)"Set lift position", 4, lift, step) == 1)        // set location
                  {
                     lift_steps[lift][step].x = get100_x * 20;
                     lift_steps[lift][step].y = get100_y * 20;
                  }
                  else quit = 99;
               break;
               case 2: construct_lift_step(lift, step, 0, 0, 100, 2); break; // set wait step
               case 3: construct_lift_step(lift, step, 0, 0, 80,  3); break; // set prox step
               case 4: quit=99; break; // cancel
            }
         }  // end of if mouse_b 1
      }  // end of mouse on menu
      redraw_get_new_lift_step_menu(sty, step, selection);
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

   al_set_mouse_xy(display, smx * les, smy * les);
   proc_controllers(); // to deal with mouse warp

   sprintf(global_string[6][0],"Lift:%d Step:%d", lift+1, step);
   sprintf(global_string[6][1],"---------------");
   sprintf(global_string[6][2],"Cancel");
   sprintf(global_string[6][3],"Move Step %d", step);
   sprintf(global_string[6][4],"Delete Step %d", step);
   sprintf(global_string[6][5],"Insert Steps");
   sprintf(global_string[6][6],"end");

   // special case for first step (don't allow delete or insert, only move )
   if (step == 0)
   {
      // set menu text
      sprintf(global_string[6][3],"Move Step %d", step);
      sprintf(global_string[6][4],"end");

      // blank and frame for menu
      al_draw_filled_rectangle(smx-70, smy-24, smx+70, smy+14, palette_color[0]);
             al_draw_rectangle(smx-70, smy-24, smx+70, smy+14, palette_color[13], 1);

      // call the menu
      if (pmenu(6) == 3) move_lift_step(lift, step);
   }

   // special case for last step (don't allow move or delete, only insert)
   else if (step == lifts[lift].num_steps-1)
   {
      // set menu text
      strcpy (global_string[6][3],"Insert Steps");
      sprintf(global_string[6][4],"end");

      // blank and frame for menu
      al_draw_filled_rectangle(smx-70, smy-24, smx+70, smy+14, palette_color[0]);
             al_draw_rectangle(smx-70, smy-24, smx+70, smy+14, palette_color[13], 1);

      // call the menu
      if (pmenu(6) == 3) insert_steps_until_quit(lift, step);
   }

   // regular step (not first or last)
   else
   {
      if (lift_steps[lift][step].type == 1) // only allow move for step type 1
      {
         // set menu text
         sprintf(global_string[6][3],"Move Step %d", step);
         sprintf(global_string[6][4],"Delete Step %d", step);
         sprintf(global_string[6][5],"Insert Steps");
         sprintf(global_string[6][6],"end");

         // blank and frame for menu
         al_draw_filled_rectangle(smx-70, smy-24, smx+70, smy+30, palette_color[0]);
                al_draw_rectangle(smx-70, smy-24, smx+70, smy+30, palette_color[13], 1);

         // call the menu
         int msel = pmenu(6);

         // do the action
         switch (msel)
         {
            case 3: move_lift_step(lift, step); break;
            case 4: delete_lift_step(lift, step); break;
            case 5: insert_steps_until_quit(lift, step); break;
         }
      }
      else
      {
         // set menu text
         sprintf(global_string[6][3],"Delete Step %d", step);
         sprintf(global_string[6][4],"Insert Steps");
         sprintf(global_string[6][5],"end");

         // blank and frame for menu
         al_draw_filled_rectangle(smx-70, smy-24, smx+70, smy+14, palette_color[0]);
                al_draw_rectangle(smx-70, smy-24, smx+70, smy+22, palette_color[13], 1);

         // call the menu
         int msel = pmenu(6);

         // do the action
         switch (msel)
         {
            case 3: delete_lift_step(lift, step); break;
            case 4: insert_steps_until_quit(lift, step); break;
         }
      }
   }
}





void set_bts(int lift)
{                                      // get button y size
   int ns = lifts[lift].num_steps + 9; // number of steps for this lift
   int sp = SCREEN_H - 46;             // how much vertical screen space
   bts = sp/ns;                        // adjust button size so they all will fit
   if (bts > 16) bts = 16;             // max button size
   if (bts < 8) bts = 8;               // min button size
}

void redraw_lift_viewer(int lift, int step)
{
   set_lift(lift, step);               // set current step in current lift
   draw_big(1);                        // update the map bitmap
   title_obj(4, lift, 0, 0, 15);       // show title and map on screen
   highlight_current_lift(lift);       // crosshairs and rect on current lift
   set_bts(lift);
   int step_ty = 46 + 7 * bts;
   draw_steps(step_ty, lift, step, step); // show lift steps
}



int lift_editor(int lift)
{
   int current_step = 0;
   int step_pointer=0;
   extern int ty;
   int ret = 0;
   int quit = 0;
   while (!quit)
   {
      // button x position
      int xa = SCREEN_W-(SCREEN_W-(db*100))+1;
      int xb = SCREEN_W-3;
      set_bts(lift);
      int step_ty = 46 + 7 * bts;
      redraw_lift_viewer(lift, current_step);

      // draw current lift on menu
      if (bts == 16) // only if max button size
      {
         int a;
         int x1 = (SCREEN_H/100)*100+22;
         int x2 = x1 + (lifts[lift].width * 20) -1;
         int y1 = step_ty + (lifts[lift].num_steps+3) * bts; // only see in 2 highest screen modes
         int y2 = y1 + (lifts[lift].height * 20) -1;
         int color = lifts[lift].color;
         for (a=0; a<10; a++)
            al_draw_rectangle(x1+a, y1+a, x2-a, y2-a, palette_color[color + ((9 - a)*16)], 1 );
         al_draw_filled_rectangle(x1+a, y1+a, x2-a, y2-a, palette_color[color] );

         int rot = 0;
         if ((lifts[lift].width == 1) && (lifts[lift].height > 1)) rot = 64;
         rtextout_centre(NULL, lifts[lift].lift_name, ((x1+x2)/2), ((y1+y2)/2)+1, color+160, 1, rot, 1);
      }
      while (key[ALLEGRO_KEY_ESCAPE])
      {
         quit = 1;
         proc_controllers();
      }
      int mb = 0; // return value from buttons

      while (key[ALLEGRO_KEY_DELETE])
      {
         mb = 20;
         proc_controllers();
      }
      while (key[ALLEGRO_KEY_RIGHT])
      {
         mb = 21;
         proc_controllers();
      }
      while (key[ALLEGRO_KEY_LEFT])
      {
         mb = 22;
         proc_controllers();
      }

      int a = 0;  // keep track of button y spacing
      int x12 = xa + 1 * (xb-xa) / 2; // 1/2          // split into half
      int x13 = xa + 1 * (xb-xa) / 3; // 1/3          // split into thirds
      int x23 = xa + 2 * (xb-xa) / 3; // 2/3
      int x27 = xa + 2 * (xb-xa) / 7; // 2/7          // split into sevenths
      int x57 = xa + 5 * (xb-xa) / 7; // 5/7

      int lc = 6; // lock_color;
      if (Viewer_lock) lc = 7;

      if (mdw_button(xa,    ty+a*bts, x27-1, ty+(a+1)*bts-2, 23, lift, 0, 0, 0,  9, 15, 0, 1,0,0,0)) mb = 22;  // prev
          mdw_button(x27,   ty+a*bts, x57-1, ty+(a+1)*bts-2, 56, lift, 0, 0, 0, lc, 15, 0, 1,0,0,0);           // lock
      if (mdw_button(x57,   ty+a*bts, xb,    ty+(a+1)*bts-2, 22, lift, 0, 0, 0,  9, 15, 0, 1,0,0,0)) mb = 21;  // next
      a++;
      if (mdw_button(xa,    ty+a*bts, x13-1, ty+(a+1)*bts-2, 28, lift, 0, 0, 0, 13, 15, 0, 1,0,0,0)) mb = 18;  // move
      if (mdw_button(x13,   ty+a*bts, x23-1, ty+(a+1)*bts-2, 20, lift, 0, 0, 0, 14, 15, 0, 1,0,0,0)) mb = 19;  // create
      if (mdw_button(x23,   ty+a*bts, xb,    ty+(a+1)*bts-2, 21, lift, 0, 0, 0, 10, 15, 0, 1,0,0,0)) mb = 20;  // delete
      a++;
      if (mdw_button(xa,    ty+a*bts, x12-1, ty+(a+1)*bts-2, 25, lift, 0, 0, 0, 1,  15, 0, 1,0,0,0)) mb = 24;  // viewer help
      if (mdw_button(x12,   ty+a*bts, xb,    ty+(a+1)*bts-2, 57, lift, 0, 0, 0, 1,  15, 0, 1,0,0,0)) mb = 25;  // lift help
      a++;
          mdw_slider(xa,    ty+a*bts, xb,    ty+(a+1)*bts-2, 43, lift, 0, 0, 0, 12, 15, 15, 1,0,0,0);          // lift width
      a++;
          mdw_slider(xa,    ty+a*bts, xb,    ty+(a+1)*bts-2, 44, lift, 0, 0, 0, 12, 15, 15, 1,0,0,0);          // lift height
      a++;
          mdw_colsel(xa,    ty+a*bts, xb,    ty+(a+1)*bts-2,  4, lift, 0, 0, 0, 15, 13, 14, 0,0,0,0);          // lift color
      a++;
      if (mdw_button(xa,    ty+a*bts, xb,    ty+(a+1)*bts-2, 29, lift, 0, 0, 0,  4, 15,  0, 1,0,0,0)) mb = 26; // lift name
      a++;

      draw_steps(step_ty, lift, current_step, step_pointer);

      // this is the awesome section that lets you move steps on the map just by clicking and dragging
      if ((mouse_x < db*100)  && (mouse_x < db*100) )        // is mouse x on map ?
      {
         int mouse_on_lift = 0;
         int mouse_on_moveable_step = 0;
         for (int x=0; x<num_lifts; x++)  // cycle lifts
            for (int y=0; y<lifts[x].num_steps; y++)  // cycle steps
               if (lift_steps[x][y].type == 1) // look for move step
               {
                  int nx = ((lift_steps[x][y].x + lifts[x].width  * 10) *db)/20;
                  int ny = ((lift_steps[x][y].y + lifts[x].height * 10) *db)/20;
                  int w = lifts[x].width  * 10 * db / 20 + 1;
                  int h = lifts[x].height * 10 * db / 20 + 1;
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
                        if (mouse_b1) al_set_mouse_xy(display, (nx+w/2) * les, (ny+h/2) * les);
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

                           set_lift(lift, current_step);   // set current step in current lift

                           al_set_clipping_rectangle(1, 1, les*db*100-2, les*db*100-2);
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
      // mouse x on step buttons
      if ((mouse_x > xa + 10) && (mouse_x < xb -10) && (quit != 1) )
      {
         // calculate step
         int step = (mouse_y - step_ty) / bts -2;
         if (mouse_b1)
         {
            current_step = step;
         }

         // is step valid
         if ((step >= 0) && (step < lifts[lift].num_steps))
         {
            step_pointer = step;
            if (mouse_b2) // button pop up menu
            {
              step_popup_menu(lift, step);
            }
         } // end of valid step
         else step_pointer = -1;// step not valid
      }
      else step_pointer = -1;// // not on buttons at all

      if (mb)
      {
         while (mouse_b1) proc_controllers(); // wait for release
         switch (mb)
         {
            case 19: create_lift(); break;                                // create new
            case 21: if (++lift > num_lifts-1) lift = num_lifts-1; break; // next
            case 22: if (--lift < 0) lift++; break;                       // previous
            case 24: help((char *)"Viewer Basics"); break;                        // help viewer
            case 25: help((char *)"Lift Viewer"); break;                          // help lifts
            case 20:  // delete
               erase_lift(lift);
               if (--lift < 0) lift = 0;    // set to prev lift or zero
               if (num_lifts < 1) quit = 1; // if erased last lift; exit lift viewer
            break;
            case 26:  // new name
            {
               strcpy(fst, lifts[lift].lift_name);
               if (edit_lift_name(lift, step_ty, bts)) strcpy(lifts[lift].lift_name, fst);
            }
            break;
            case 18:  // run lifts
            {
               while (!key[ALLEGRO_KEY_ESCAPE])
               {
                  proc_controllers();
                  for (int t=0; t<8; t++) move_lifts(1);                   // move lifts for 8 frames
                  draw_big(1);                                             // update the map bitmap
                  show_big();                                              // show the map
                  highlight_current_lift(lift);                            // highlight current lift
                  draw_steps(step_ty, lift, lifts[lift].current_step, -1); // show lift steps
                  al_flip_display();
               }
               while (key[ALLEGRO_KEY_ESCAPE]) proc_controllers(); // wait for release
               lift_setup(); // reset all lifts to step 0
            }
            break;
         } // end if switch (mb)
      } // end of if (mb)

      proc_controllers();
      al_flip_display();
      al_clear_to_color(al_map_rgb(0,0,0));

   } // end of while !quit
   lift_setup();
   draw_big(1);
   return ret;
}






