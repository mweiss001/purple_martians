// e_fnx.cpp (20100220 cleanup)
#include "pm.h"

int old_mouse_x;
int old_mouse_y;
int old_mouse_b1;
int old_mouse_b2;

int mouse_changed(void)
{
   int change = 0;
   if (old_mouse_x != mouse_x)
   {
      old_mouse_x = mouse_x;
      change = 1;
//      printf("x\n");
   }
   if (old_mouse_y != mouse_y)
   {
      old_mouse_y = mouse_y;
      change = 1;
//      printf("y\n");
   }
   if (old_mouse_b1 != mouse_b1)
   {
      old_mouse_b1 = mouse_b1;
      change = 1;
//      printf("b\n");
   }
   if (old_mouse_b2 != mouse_b2)
   {
      old_mouse_b2 = mouse_b2;
      change = 1;
//      printf("b\n");
   }
   return change;
}


al_fixed get_sproingy_jump_height(int num)
{
   al_fixed t1 = al_fixdiv(al_itofix(item[num][7]), al_ftofix(7.1));
   al_fixed t2 = al_itofix(-15); // jump starts not at zero, but at almost one block height off ground
   while (t1 > al_itofix(0))
   {
      t2 += t1; // distance moved this time period
      t1 -= al_ftofix(.2); // minus slow gravity
   }
   return t2;
}

// used only in sliders for button set new direction (cannon and podzilla)
void set_xyinc_rot(int EN, int x2, int y2)
{
   al_fixed xlen = al_itofix(x2) - Efi[EN][0];      // get the x distance between enemy and x2
   al_fixed ylen = al_itofix(y2) - Efi[EN][1];      // get the y distance between enemy and y2
   al_fixed hy_dist =  al_fixhypot(xlen, ylen);     // hypotenuse distance
   al_fixed speed = Efi[EN][5];                  // speed
   al_fixed scaler = al_fixdiv(hy_dist, speed);     // get scaler
   al_fixed xinc = al_fixdiv(xlen, scaler);         // calc xinc
   al_fixed yinc = al_fixdiv(ylen, scaler);         // calc yinc
   Efi[EN][2] = xinc;
   Efi[EN][3] = yinc;
   Efi[EN][14] = al_fixatan2(ylen, xlen) - al_itofix(64);
}

// used only in sliders for button set new direction (rocket)
void set_rocket_rot(int num, int x2, int y2)
{
   al_fixed xlen = al_itofix(x2) - al_itofix(item[num][4]);      // get the x distance between item and x2
   al_fixed ylen = al_itofix(y2) - al_itofix(item[num][5]);      // get the y distance between item and y2
   al_fixed rot = al_fixatan2(ylen, xlen) + al_itofix(64);
   item[num][10] = al_fixtoi(rot) * 10;
}

void set_wx(int x, int y)
{
     extern int wx, wy;
     int d;
     wx = x - SCREEN_W/40;
     wy = y - SCREEN_H/40;

    // check limits
    d = 100 - (SCREEN_W/20);
    if (wx>d) wx = d;
    if (wx<0) wx = 0;

    d = 100 - (SCREEN_H/20);
    if (wy>d) wy = d;
    if (wy<0) wy = 0;
}
void set_wx_from_start_block(void)
{
   int x = 0, y = 0;
   for (int c=0; c<500; c++)  // get initial wx, wy from start block
   if (item[c][0] == 5)
   {
      x = item[c][4]/20;
      y = item[c][5]/20;
      break;
   }
   set_wx(x+4, y);
}

void show_big(void)
{
   al_draw_bitmap(lefsm, 0, 0, 0);
}

void draw_big(int draw_lifts)
{
   init_l2000(); // fill l2000 with blocks and lift lines
   draw_level2(l2000, 0, 0, 2000, 1, 1, 1, 1, 0);
   draw_level2(lefsm, 0, 0, db*100, 1, 1, 1, draw_lifts, 0);
   al_set_target_backbuffer(display);
}

void draw_cloner_boxes(int num)
{
   int cw = Ei[num][19];     // width
   int ch = Ei[num][20];     // height

   int cx1 = Ei[num][15];    // source
   int cy1 = Ei[num][16];
   int cx2 = cx1 + cw;
   int cy2 = cy1 + ch;

   int cx3 = Ei[num][17];    // destination
   int cy3 = Ei[num][18];
   int cx4 = cx3 + cw;
   int cy4 = cy3 + ch;

   int tx1 = Ei[num][11] * db; // trigger box
   int ty1 = Ei[num][12] * db;
   int tx2 = Ei[num][13] * db + db;
   int ty2 = Ei[num][14] * db + db;

   al_set_clipping_rectangle(1, 1, les*db*100-2, les*db*100-2);
   al_draw_rectangle(cx1*db, cy1*db, cx2*db, cy2*db, palette_color[10], 1); // src
   al_draw_rectangle(cx3*db, cy3*db, cx4*db, cy4*db, palette_color[11], 1); // dest
   al_draw_rectangle(tx1, ty1, tx2, ty2, palette_color[14], 1);             // trig
   al_reset_clipping_rectangle();
}


void draw_bs(int cc)
{
   ALLEGRO_BITMAP *jtemp = NULL;

   // get mouse pos
   int dx = mouse_x/db;
   int dy = mouse_y/db;

   // set bullseye map size
   int ssz = 180;
   if (db == 4) ssz = 220;
   if (db == 6) ssz = 180;
   if (db > 7) ssz = 300;
   if (db > 9) ssz = 300;


   int ccz = (((ssz/20)-1)/2); // 7 = 3, 5 = 2, 3 = 1

   int mx = -ssz/2 + (db*100) + (SCREEN_W-(db*100)) / 2;
   int my = (db*100)-ssz-2;

   int ex = dx*20 - (ssz/2-10);
   int ey = dy*20 - (ssz/2-10);

   if ((dx<100) && (dy < 100))
   {
      // get background from l2000 for bullseye map sized ssz x ssz
      jtemp = al_create_bitmap(ssz, ssz);
      al_set_target_bitmap(jtemp);
      al_draw_bitmap_region(l2000, ex, ey, ssz, ssz, 0, 0, 0);

      // clear edges if necessary
      if (dx < ccz)    al_draw_filled_rectangle(0,   0,  ((ccz-dx)*20)-1,    ssz-1,   palette_color[0]);
      if (dy < ccz)    al_draw_filled_rectangle(0,   0,   ssz-1,   ((ccz-dy)*20)-1,   palette_color[0]);
      if (dx > 99-ccz) al_draw_filled_rectangle(ssz-(ccz-(99-dx))*20,  0,  ssz-1,  ssz-1,   palette_color[0]);
      if (dy > 99-ccz) al_draw_filled_rectangle(0,  ssz-(ccz-(99-dy))*20,  ssz-1,  ssz-1,   palette_color[0]);

      // draw red bullseye
      al_draw_line(       0,   ssz/2-10,     ssz-1,  ssz/2-10,  palette_color[10],1);
      al_draw_line(       0,   ssz/2+11,     ssz-1,  ssz/2+11,  palette_color[10],1);
      al_draw_line(ssz/2-10,          0,  ssz/2-10,     ssz-1,  palette_color[10],1);
      al_draw_line(ssz/2+11,          0,  ssz/2+11,     ssz-1,  palette_color[10],1);

      al_set_clipping_rectangle(les*db*100+1, 0, SCREEN_W-2, SCREEN_H-1);


      al_set_target_backbuffer(display);
      al_draw_bitmap(jtemp, mx+1, my+1, 0);
      al_destroy_bitmap(jtemp);
      // frame bullseye
      al_draw_rectangle(mx, my, mx+ssz+1, my+ssz+1, palette_color[10], 1);
      al_reset_clipping_rectangle();
      al_draw_textf(font, palette_color[15], txc, my-9, ALLEGRO_ALIGN_CENTER, " x=%-2d     y=%-2d ", dx, dy );
   }
   else
   {
      al_draw_text(font, palette_color[14], txc, my-9, ALLEGRO_ALIGN_CENTER, "  mouse off map  ");
      // erase bullseye map
      al_draw_filled_rectangle(mx, my, mx+ssz, my+ssz, palette_color[0]);
      // frame bullseye
      al_draw_rectangle(mx, my, mx+ssz+1, my+ssz+1, palette_color[10], 1);
   }
}

int getbox(char *txt, int obj_type, int sub_type, int num )
{
   int bs_on = 1;
   int quit = 0;
   while (mouse_b1) proc_controllers();    // wait for release
   while (!quit)
   {
      proc_controllers();
      int dx = mouse_x/db;
      int dy = mouse_y/db;

      title_obj(obj_type, sub_type, num, 0, 15);

      // show text line
      al_draw_text(font, palette_color[9],  txc, 72,  ALLEGRO_ALIGN_CENTER, "Draw a new");
      al_draw_text(font, palette_color[10], txc, 80,  ALLEGRO_ALIGN_CENTER, txt);
      al_draw_text(font, palette_color[9],  txc, 88,  ALLEGRO_ALIGN_CENTER, "by clicking and");
      al_draw_text(font, palette_color[9],  txc, 96,  ALLEGRO_ALIGN_CENTER, "dragging with the");
      al_draw_text(font, palette_color[9],  txc, 104, ALLEGRO_ALIGN_CENTER, "left mouse button");
      al_draw_text(font, palette_color[14], txc, 130, ALLEGRO_ALIGN_CENTER, "Cancel with <ESC> or");
      al_draw_text(font, palette_color[9],  txc, 138, ALLEGRO_ALIGN_CENTER, "the right mouse button");

      if (bs_on) draw_bs(15);

      if ((obj_type == 3) && (sub_type == 9)) // cloner
         draw_cloner_boxes(num);

      // show block cursor
      if ((dx<100) && (dy < 100)) al_draw_rectangle(dx*db, dy*db, (dx+1)*db-1, (dy+1)*db-1, palette_color[127-32], 1); // show which block is selected

      al_flip_display();
      al_clear_to_color(al_map_rgb(0,0,0));

      if (mouse_b1)
      {
         quit = 1;
         bx1 = mouse_x/db;
         by1 = mouse_y/db;
         while (mouse_b1) // trap while b1 is held
         {
            proc_controllers();
            title_obj(obj_type, sub_type, num, 0, 15);

            // show text line
            al_draw_text(font, palette_color[9],  txc, 72,  ALLEGRO_ALIGN_CENTER, "Draw a new");
            al_draw_text(font, palette_color[10], txc, 80,  ALLEGRO_ALIGN_CENTER, txt);
            al_draw_text(font, palette_color[9],  txc, 88,  ALLEGRO_ALIGN_CENTER, "by clicking and");
            al_draw_text(font, palette_color[9],  txc, 96,  ALLEGRO_ALIGN_CENTER, "dragging with the");
            al_draw_text(font, palette_color[9],  txc, 104, ALLEGRO_ALIGN_CENTER, "left mouse button");
            al_draw_text(font, palette_color[14], txc, 130, ALLEGRO_ALIGN_CENTER, "Cancel with <ESC> or");
            al_draw_text(font, palette_color[9],  txc, 138, ALLEGRO_ALIGN_CENTER, "the right mouse button");

            if (bs_on) draw_bs(15);

            if ((obj_type == 3) && (sub_type == 9)) // cloner
              draw_cloner_boxes(num);


            // show selection rectangle
            bx2 = (mouse_x/db)+1;
            by2 = (mouse_y/db)+1;
            al_set_clipping_rectangle(0, 0, les*db*100-1, les*db*100-1);
            al_draw_rectangle((bx1)*db, (by1)*db, (bx2)*db, (by2)*db, palette_color[15], 1);
            al_reset_clipping_rectangle();

            al_flip_display();
            al_clear_to_color(al_map_rgb(0,0,0));
         }

         // limits
         if (bx1<0) bx1 = 0;
         if (bx2<0) bx2 = 0;
         if (by1<0) by1 = 0;
         if (by2<0) by2 = 0;

         if (bx1>99) bx1 = 99;
         if (bx2>99) bx2 = 99;
         if (by1>99) by1 = 99;
         if (by2>99) by2 = 99;

         // ensure top-right, bottom left format
         if (bx1 > bx2)
         {
            int btemp = bx2;
            bx2 = bx1;
            bx1= btemp;
         }
         if (by1 > by2)
         {
            int btemp = by2;
            by2 = by1;
            by1= btemp;
         }
      }
      if  (mouse_b2)
      {
         while (mouse_b2) proc_controllers(); // wait for release
         return 0;
      }
      if (key[ALLEGRO_KEY_ESCAPE])
      {
         return 0;
      }
   } // end of while not quit
   return 1;
}




int getxy(char *txt, int obj_type, int sub_type, int num )
{
   int dx=0, dy=0;

   // in case these are needed for lifts
   int lift = sub_type;
   int step = num;

   int retval=0;
   int quit=0;

   while (mouse_b1) proc_controllers(); // wait for release
   al_flip_display();
   al_clear_to_color(al_map_rgb(0,0,0));


// get original positions in case we are cancelled
   int original_dx=0, original_dy=0;
   if (obj_type == 2)
   {
      original_dx = item[num][4];
      original_dy = item[num][5];
   }
   if (obj_type == 3)
   {
      original_dx = al_fixtoi(Efi[num][0]) / 20;
      original_dy = al_fixtoi(Efi[num][1]) / 20;
   }

   if (obj_type == 99) // pod extended
   {
      original_dx = al_fixtoi(Efi[num][5]) / 20;
      original_dy = al_fixtoi(Efi[num][6]) / 20;
   }

   if (obj_type == 98) // cloner destination
   {
      original_dx = Ei[num][17];
      original_dy = Ei[num][18];
   }

   if (obj_type == 4)
   {

      original_dx = lift_steps[lift][step].x / 20;
      original_dy = lift_steps[lift][step].y / 20;
   }

   while(!quit)
   {
      dx = mouse_x/db;
      dy = mouse_y/db;

      if ((dx < 100) && (dy < 100)) // if mouse on map
      {
         switch (obj_type) // show draw_item
         {
            case 2: // show item
               if (sub_type == 1010) // message display only
               {
                  show_big();
                  al_set_clipping_rectangle(1, 1, les*db*100-2, les*db*100-2);
                  al_draw_bitmap(mp, dx*db, dy*db, 0);
                  al_reset_clipping_rectangle();
               }
               else // all other items
               {
                  item[num][4] = dx*20;
                  item[num][5] = dy*20;
                  itemf[num][0] = al_itofix(dx*20);
                  itemf[num][1] = al_itofix(dy*20);
                  draw_big(1);
                  show_big();
                  draw_bs(14);            // show bullseye map
                  al_set_clipping_rectangle(1, 1, les*db*100-2, les*db*100-2);
                  al_draw_rectangle(dx*db, dy*db, (dx+1)*db-1, (dy+1)*db-1, palette_color[127-32], 1); // draw box to show cursor
                  al_reset_clipping_rectangle();
               }
            break;
            case 3: // show enem
               Efi[num][0] = al_itofix(dx * 20);
               Efi[num][1] = al_itofix(dy * 20);
               draw_big(1);
               show_big();
               draw_bs(14);            // show bullseye map
               al_set_clipping_rectangle(1, 1, les*db*100-2, les*db*100-2);
               al_draw_rectangle(dx*db, dy*db, (dx+1)*db-1, (dy+1)*db-1, palette_color[127-32], 1); // draw box to show cursor
               al_reset_clipping_rectangle();
            break;
            case 99: // move pod extended
            {
               Efi[num][5] = al_itofix(dx * 20);
               Efi[num][6] = al_itofix(dy * 20);
               draw_big(1);
               show_big();
               draw_bs(14);
               int ex = al_fixtoi(Efi[num][0])*db/20;
               int ey = al_fixtoi(Efi[num][1])*db/20;
               int px = al_fixtoi(Efi[num][5])*db/20;
               int py = al_fixtoi(Efi[num][6])*db/20;
               al_set_clipping_rectangle(1, 1, les*db*100-2, les*db*100-2);
               al_draw_rectangle(ex, ey, ex+db-1, ey+db-1, palette_color[13], 1);           // draw box to show pod location
               al_draw_filled_rectangle(px, py, px+db-1, py+db-1, palette_color[10]);       // draw box to show pod extended
               al_draw_line(ex+db/2, ey+db/2, px+db/2, py+db/2, palette_color[10], 1); // connect with line
               al_reset_clipping_rectangle();
            }
            break;
            case 98: // cloner destination
            {
               Ei[num][17] = dx;
               Ei[num][18] = dy;
               draw_big(1);
               show_big();
               draw_bs(14);
               draw_cloner_boxes(num);
            }
            break;
            case 97: // set new rocket direction
            {
               draw_big(1);
               show_big();
               draw_bs(14);
               int ix = item[num][4]*db/20;
               int iy = item[num][5]*db/20;
               al_set_clipping_rectangle(1, 1, les*db*100-2, les*db*100-2);
               al_draw_rectangle(ix, iy, ix+db-1, iy+db-1, palette_color[13], 1);                     // show rocket location
               al_draw_filled_rectangle(dx*db, dy*db, (dx+1)*db-1, (dy+1)*db-1, palette_color[10]);   // show cursor
               al_draw_line(ix+db/2, iy+db/2, dx*db+db/2, dy*db+db/2, palette_color[10], 1);     // connect with line
               al_reset_clipping_rectangle();
            }
            break;
            case 96: // set cannon or bouncer direction
            {
               draw_big(1);
               show_big();
               draw_bs(14);
               int ex = al_fixtoi(Efi[num][0])*db/20;
               int ey = al_fixtoi(Efi[num][1])*db/20;
               al_set_clipping_rectangle(1, 1, les*db*100-2, les*db*100-2);
               al_draw_rectangle(ex, ey, ex+db-1, ey+db-1, palette_color[13], 1);                   // draw box to show enemy location
               al_draw_filled_rectangle(dx*db, dy*db, (dx+1)*db-1, (dy+1)*db-1, palette_color[10]); // draw box to show cursor
               al_draw_line(ex+db/2, ey+db/2, dx*db+db/2, dy*db+db/2, palette_color[10], 1);   // connect with line
               al_reset_clipping_rectangle();

            }
            break;
            case 4: // show lift
            {
               lift_steps[lift][step].x = dx*20;
               lift_steps[lift][step].y = dy*20;         proc_controllers();

               //  redraw
               set_lift(lift, step);   // set current step in current lift
               draw_big(1);
               show_big();
               draw_bs(14);            // show bullseye map

               al_set_clipping_rectangle(1, 1, les*db*100-2, les*db*100-2);
               void highlight_current_lift(int lift);
               highlight_current_lift(lift);   // crosshairs and rect on current lift
               al_reset_clipping_rectangle();
            }
            break;
         }
      } // end of if mouse on map

      al_set_target_backbuffer(display);

      // show text line
      al_draw_text(font, palette_color[10], txc, 80,  ALLEGRO_ALIGN_CENTER, txt);
      al_draw_text(font, palette_color[9],  txc, 88,  ALLEGRO_ALIGN_CENTER, "with left mouse button");
      al_draw_text(font, palette_color[14], txc, 120, ALLEGRO_ALIGN_CENTER, "Cancel");
      al_draw_text(font, palette_color[9],  txc, 128, ALLEGRO_ALIGN_CENTER, "with right mouse button");

      al_flip_display();
      al_clear_to_color(al_map_rgb(0,0,0));
      proc_controllers();

      title_obj(obj_type, sub_type, num, 0, 15);

      while (mouse_b1)
      {
         proc_controllers();
         quit = 1;
         retval = 1;  // b1 xy
      }
      while (mouse_b2)
      {
         proc_controllers();
         quit = 1;
         retval = 2;  // b2 xy
      }
      while (key[ALLEGRO_KEY_ESCAPE])
      {
         proc_controllers();
         quit = 1;
         retval = 0;  // ignore xy
      }
   } // end of while(!quit);


   if (dx > 99) dx = 99;
   if (dy > 99) dy = 99;
   get100_x = dx;
   get100_y = dy;

   if (retval != 1) // restore old positions if cancelled
   {
      if (obj_type == 2)
      {
         item[num][4] = original_dx;
         item[num][5] = original_dy;
         itemf[num][0] = al_itofix(original_dx);
         itemf[num][1] = al_itofix(original_dy);
      }
      if (obj_type == 3)
      {
          Efi[num][0] = al_itofix(original_dx * 20);
          Efi[num][1] = al_itofix(original_dy * 20);
      }

      if (obj_type == 99)
      {
          Efi[num][5] = al_itofix(original_dx * 20);
          Efi[num][6] = al_itofix(original_dy * 20);
      }

      if (obj_type == 98)
      {
          Efi[num][5] = original_dx;
          Efi[num][6] = original_dy;
      }

      if (obj_type == 4)
      {
         lift_steps[lift][step].x = original_dx * 20;
         lift_steps[lift][step].y = original_dy * 20;
         set_lift(lift, step);   // set current step in current lift
      }
   }
   return retval;
}


int get_item(char *txt, int obj_type, int sub_type, int num )
{
   int itx, ity, dx, dy;
   int mouse_on_item = 0;
   int quit = 0;
   int ret_item = -1;

   while (mouse_b1) proc_controllers();      // wait for release

   int x2 = item[num][4]/20; // get the door entry position
   int y2 = item[num][5]/20;

   int li = item[num][9];    // get original linked item position
   int x3 = item[li][4]/20;
   int y3 = item[li][5]/20;

   while(!quit)
   {
      // show text line
      al_draw_text(font, palette_color[10], txc, 80,  ALLEGRO_ALIGN_CENTER, txt);
      al_draw_text(font, palette_color[9],  txc, 88,  ALLEGRO_ALIGN_CENTER, "with left mouse button");
      al_draw_text(font, palette_color[14], txc, 120, ALLEGRO_ALIGN_CENTER, "Cancel");
      al_draw_text(font, palette_color[9],  txc, 128, ALLEGRO_ALIGN_CENTER, "with right mouse button");

      al_flip_display();
      al_clear_to_color(al_map_rgb(0,0,0));
      title_obj(obj_type, sub_type, num, 0, 15);

      proc_controllers();

      dx = mouse_x/db;
      dy = mouse_y/db;

      draw_bs(14);      // show bullseye map

      mouse_on_item = 0;

      if ((dx<100) && (dy<100))      // if mouse on map
         for (int x=0; x<500; x++)   // see if we are pointing at a door item
            if (item[x][0] == 1)
            {
               itx = item[x][4]/20;
               ity = item[x][5]/20;
               if ((dx == itx) && (dy == ity))
               {
                  mouse_on_item = 1;
                  ret_item = x;
               }
            }

      itx = item[ret_item][4]/20;
      ity = item[ret_item][5]/20;

      crosshairs(0, 0, x2, y2, 13); // draw the door entry position
      if (mouse_on_item)
      {
         crosshairs(0, 0, itx, ity, 14); // draw the selected item position
         int o = db/2;
         al_draw_line(x2*db+o, y2*db+o, itx*db+o, ity*db+o, palette_color[14], 1);
         al_draw_textf(font, palette_color[15], txc, 180, ALLEGRO_ALIGN_CENTER, " Item:%d ", ret_item);

      }
      else
      {
         crosshairs(0, 0, x3, y3, 10);  // show orignal linked item
         int o = db/2;
         al_draw_line(x2*db+o, y2*db+o, x3*db+o, y3*db+o, palette_color[10], 1);
      }

      while (mouse_b1)
      {
         proc_controllers();
         quit = 1;
      }
      while ((mouse_b2) || (key[ALLEGRO_KEY_ESCAPE]))
      {
         proc_controllers();
         quit =1;
         ret_item = -1;
      }

   } // end of while(!quit);
   if (!mouse_on_item) ret_item = -1;
   return ret_item;
}


void initialize_zz(void)
{
   int c;
   for (c=0; c<NUM_ANS; c++)
      zz[2][c]=0;  // reset the passcount
   passcount = 0;

   for (c=0; c<NUM_ANS; c++)
      zz[0][c]=zz[5][c];  // set shape one
}

