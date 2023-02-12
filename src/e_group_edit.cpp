// e_group_edit.cpp

#include "pm.h"
#include "mwWindow.h"
#include "mwWindowManager.h"
#include "mwFont.h"
#include "mwBitmap.h"


// predefined things that can be group editted
struct ge_datum
{
   char name[40];
   int vartyp;     // 2=item, 3=Ei, 4=Efi, 5=initial direction, 6=initial position
   int varnum;     // index of the variable
   int valid;      // does it apply to items in list
   int collapsed;  // how it is drawn
   int adj_mode;
   float adj_min;
   float adj_max;
   float min_allowed;
   float max_allowed;
};
struct ge_datum ge_data[40];

void ge_init_data(void)
{
   int i;
   for (i=0; i<40; i++)
   {
      ge_data[i].vartyp = 0;
      ge_data[i].varnum = 0;
      ge_data[i].valid = 0;
      ge_data[i].collapsed = 0;
      ge_data[i].adj_mode = 0;
      ge_data[i].adj_min = 0;
      ge_data[i].adj_max = 0;
      ge_data[i].min_allowed = 0;
      ge_data[i].max_allowed = 0;
      sprintf(ge_data[i].name, " ");
   }

   i = 0;
   sprintf(ge_data[i].name, "X speed (arch)");
   ge_data[i].vartyp = 4;
   ge_data[i].varnum = 6;
   ge_data[i].adj_min = 3;
   ge_data[i].adj_max = 5;
   ge_data[i].min_allowed = .5;
   ge_data[i].max_allowed = 30;
   i = 1;
   sprintf(ge_data[i].name, "X speed (trak)");
   ge_data[i].vartyp = 4;
   ge_data[i].varnum = 2;
   ge_data[i].adj_min = 3;
   ge_data[i].adj_max = 5;
   ge_data[i].min_allowed = .5;
   ge_data[i].max_allowed = 30;
   i = 2;
   sprintf(ge_data[i].name, "X Speed");
   ge_data[i].vartyp = 4;
   ge_data[i].varnum = 5;
   ge_data[i].adj_min = 2;
   ge_data[i].adj_max = 4;
   ge_data[i].min_allowed = 1;
   ge_data[i].max_allowed = 20;
   i = 3;
   sprintf(ge_data[i].name, "X Acceleration");
   ge_data[i].vartyp = 4;
   ge_data[i].varnum = 6;
   ge_data[i].adj_min = .1;
   ge_data[i].adj_max = 3;
   ge_data[i].min_allowed = .01;
   ge_data[i].max_allowed = 5;
   i = 4;
   sprintf(ge_data[i].name, "Initial Direction");
   ge_data[i].vartyp = 3;
   ge_data[i].varnum = 2;
   ge_data[i].adj_min = 0;
   ge_data[i].adj_max = 2;
   ge_data[i].min_allowed = 0;
   ge_data[i].max_allowed = 2;
   i = 5;
   sprintf(ge_data[i].name, "Y speed");
   ge_data[i].vartyp = 4;
   ge_data[i].varnum = 3;
   ge_data[i].adj_min = 3;
   ge_data[i].adj_max = 5;
   ge_data[i].min_allowed = .5;
   ge_data[i].max_allowed = 30;
   i = 6;
   sprintf(ge_data[i].name, "Initial Direction");
   ge_data[i].vartyp = 3;
   ge_data[i].varnum = 5;
   ge_data[i].adj_min = 0;
   ge_data[i].adj_max = 8;
   ge_data[i].min_allowed = 0;
   ge_data[i].max_allowed = 8;

   i = 8;
   sprintf(ge_data[i].name, "Jump Before Hole");
   ge_data[i].vartyp = 3;
   ge_data[i].varnum = 11;
   ge_data[i].adj_min = 0;
   ge_data[i].adj_max = 10;
   ge_data[i].min_allowed = 0;
   ge_data[i].max_allowed = 180;
   i = 9;
   sprintf(ge_data[i].name, "Jump Before Wall");
   ge_data[i].vartyp = 3;
   ge_data[i].varnum = 12;
   ge_data[i].adj_min = 30;
   ge_data[i].adj_max = 60;
   ge_data[i].min_allowed = 0;
   ge_data[i].max_allowed = 120;

   i = 11;
   sprintf(ge_data[i].name, "Drop Mode");
   ge_data[i].vartyp = 3;
   ge_data[i].varnum = 7;
   ge_data[i].adj_min = 0;
   ge_data[i].adj_max = 2;
   ge_data[i].min_allowed = 0;
   ge_data[i].max_allowed = 2;
   i = 12;
   sprintf(ge_data[i].name, "Height Above Player");
   ge_data[i].vartyp = 3;
   ge_data[i].varnum = 10;
   ge_data[i].adj_min = 0;
   ge_data[i].adj_max = 80;
   ge_data[i].min_allowed = 0;
   ge_data[i].max_allowed = 200;
   i = 13;
   sprintf(ge_data[i].name, "Flap Height");
   ge_data[i].vartyp = 3;
   ge_data[i].varnum = 21;
   ge_data[i].adj_min = 10;
   ge_data[i].adj_max = 40;
   ge_data[i].min_allowed = 0;
   ge_data[i].max_allowed = 200;
   i = 14;
   sprintf(ge_data[i].name, "Flap Speed");
   ge_data[i].vartyp = 4;
   ge_data[i].varnum = 10;
   ge_data[i].adj_min = 1.5;
   ge_data[i].adj_max = 4;
   ge_data[i].min_allowed = .5;
   ge_data[i].max_allowed = 20;

   i = 19;
   sprintf(ge_data[i].name, "Extend Speed");
   ge_data[i].vartyp = 4;
   ge_data[i].varnum = 9;
   ge_data[i].adj_min = 3;
   ge_data[i].adj_max = 8;
   ge_data[i].min_allowed = .1;
   ge_data[i].max_allowed = 30;
   i = 20;
   sprintf(ge_data[i].name, "Pause Time");
   ge_data[i].vartyp = 3;
   ge_data[i].varnum = 9;
   ge_data[i].adj_min = 5;
   ge_data[i].adj_max = 20;
   ge_data[i].min_allowed = 0;
   ge_data[i].max_allowed = 200;
   i = 21;
   sprintf(ge_data[i].name, "Seek Count");
   ge_data[i].vartyp = 3;
   ge_data[i].varnum = 8;
   ge_data[i].adj_min = 2;
   ge_data[i].adj_max = 5;
   ge_data[i].min_allowed = 0;
   ge_data[i].max_allowed = 100;
   i = 22;
   sprintf(ge_data[i].name, "Seek Speed");
   ge_data[i].vartyp = 4;
   ge_data[i].varnum = 5;
   ge_data[i].adj_min = 2;
   ge_data[i].adj_max = 5;
   ge_data[i].min_allowed = 0;
   ge_data[i].max_allowed = 40;
   i = 23;
   sprintf(ge_data[i].name, "Extra Hits To Kill");
   ge_data[i].vartyp = 3;
   ge_data[i].varnum = 9;
   ge_data[i].adj_min = 0;
   ge_data[i].adj_max = 3;
   ge_data[i].min_allowed = 0;
   ge_data[i].max_allowed = 20;
   i = 24;
   sprintf(ge_data[i].name, "Bullet Speed");
   ge_data[i].vartyp = 4;
   ge_data[i].varnum = 7;
   ge_data[i].adj_min = 2;
   ge_data[i].adj_max = 5;
   ge_data[i].min_allowed = .1;
   ge_data[i].max_allowed = 80;
   i = 25;
   sprintf(ge_data[i].name, "Bullet Retrigger");
   ge_data[i].vartyp = 3;
   ge_data[i].varnum = 15;
   ge_data[i].adj_min = 80;
   ge_data[i].adj_max = 200;
   ge_data[i].min_allowed = 2;
   ge_data[i].max_allowed = 1000;
   i = 26;
   sprintf(ge_data[i].name, "Bullet Proximity");
   ge_data[i].vartyp = 3;
   ge_data[i].varnum = 17;
   ge_data[i].adj_min = 80;
   ge_data[i].adj_max = 200;
   ge_data[i].min_allowed = 2;
   ge_data[i].max_allowed = 2000;
   i = 30;
   sprintf(ge_data[i].name, "Initial Direction");
   ge_data[i].vartyp = 5;
   ge_data[i].varnum = 0;
   ge_data[i].adj_min = 0;
   ge_data[i].adj_max = 0;
   ge_data[i].min_allowed = 0;
   ge_data[i].max_allowed = 0;
   i = 31;
   sprintf(ge_data[i].name, "Initial Position");
   ge_data[i].vartyp = 6;
   ge_data[i].varnum = 0;
   ge_data[i].adj_min = 0;
   ge_data[i].adj_max = 0;
   ge_data[i].min_allowed = 0;
   ge_data[i].max_allowed = 0;
}

// iterate the object list to find out what controls are valid
void ge_set_valid_controls(void)
{
   // set all controls valid by default
   for (int i=0; i<40; i++) if (ge_data[i].vartyp) ge_data[i].valid = 1;

   // remove controls as soon as they don't apply to all objects in list
   for (int i=0; i<NUM_OBJ; i++)
      if (mwWM.obj_list[i][0])
      {
         int typ = mwWM.obj_list[i][0];
         int num = mwWM.obj_list[i][1];

         if (typ == 2) // item
         {
            for (int a=0; a<27; a++) ge_data[a].valid = 0;
            ge_data[30].valid = 0;
         }

         if (typ == 3) // enemy
         {
            int etyp = Ei[num][0];

            if ((etyp != 3) && (etyp != 11))                 ge_data[0].valid = 0; // x speed (archwagon, block walker)
            if (etyp != 8)                                   ge_data[1].valid = 0; // x speed (trakbot)

            if ((etyp != 3) && (etyp != 11) && (etyp != 12)) ge_data[4].valid = 0; // initial direction (archwagon, block walker and flapper)
            if ((etyp != 3) && (etyp != 8) && (etyp != 12))  ge_data[5].valid = 0; // y speed (archwagon, trakbot, flapper)
            if (etyp != 8)                                   ge_data[6].valid = 0; // initial direction (trakbot)

            if ((etyp != 3)  && (etyp != 11))                ge_data[8].valid = 0; // jump before hole (archwagon)
            if ((etyp != 3)  && (etyp != 11))                ge_data[9].valid = 0; // jump before wall (archwagon)
            if (etyp != 8)                                   ge_data[11].valid = 0; // drop mode (trakbot)


            if ((etyp != 4) && (etyp != 6))                                               ge_data[21].valid = 0; // seek count (bouncer and cannon)
            if ((etyp != 4) && (etyp != 6))                                               ge_data[22].valid = 0; // seek speed (bouncer and cannon)
            if (etyp != 6)                                                                ge_data[23].valid = 0; // extra hits to kill (cannon)

            if ((etyp != 3) && (etyp != 6) && (etyp != 7) && (etyp != 8) && (etyp != 12)) ge_data[24].valid = 0; // bullet speed (archwagon, cannon, podzilla, trakbot, flapper)
            if ((etyp != 3) && (etyp != 6) && (etyp != 8) && (etyp != 12))                ge_data[25].valid = 0; // bullet retrigger (archwagon, cannon, trakbot, flapper)
            if ((etyp != 3) && (etyp != 6) && (etyp != 8) && (etyp != 12))                ge_data[26].valid = 0; // bullet proximity (archwagon, cannon, trakbot, flapper)

            if (etyp != 12) // flapper specific
            {
               ge_data[2].valid = 0; // x speed
               ge_data[3].valid = 0; // x accel
               ge_data[12].valid = 0;
               ge_data[13].valid = 0;
               ge_data[14].valid = 0;
            }
            if (etyp != 7) // podzilla specific
            {
               ge_data[19].valid = 0;
               ge_data[20].valid = 0;
            }
            if ((etyp != 4) && (etyp != 6))  ge_data[30].valid = 0; // initial direction (bouncer and cannon)
         }
      }
}

void ge_add_to_obj_list(int t, int i)
{
   // check if already in list
   int duplicate = 0;
   for (int o=0; o<NUM_OBJ; o++)
      if ((mwWM.obj_list[o][0] == t) && (mwWM.obj_list[o][1] == i)) duplicate = 1;
   if (!duplicate)
   {
      for (int o=0; o<NUM_OBJ; o++)
         if (mwWM.obj_list[o][0] == 0)
         {
            mwWM.obj_list[o][0] = t;
            mwWM.obj_list[o][1] = i;
            o = NUM_OBJ;
         }
   }
}

void ge_remove_obj_list_item(int o)
{
   // slide down to close hole
   for (int i=o; i<NUM_OBJ-1; i++)
   {
      mwWM.obj_list[i][0] = mwWM.obj_list[i+1][0];
      mwWM.obj_list[i][1] = mwWM.obj_list[i+1][1];
      mwWM.obj_list[i][2] = mwWM.obj_list[i+1][2];
   }
}

void ge_remove_obj_list_filtered_items(void)
{
   for (int i=0; i<NUM_OBJ; i++)
      if (mwWM.obj_list[i][0])
      {
         int type = mwWM.obj_list[i][0];
         int num = mwWM.obj_list[i][1];
         if ((type == 2) && (mwWM.obj_filter[type][item[num][0]] == 0)) ge_remove_obj_list_item(i);
         if ((type == 3) && (mwWM.obj_filter[type][  Ei[num][0]] == 0)) ge_remove_obj_list_item(i);
      }
}

void ge_swap_obj_list_items(int i1, int i2)
{
   int t0 = mwWM.obj_list[i1][0];
   int t1 = mwWM.obj_list[i1][1];
   int t2 = mwWM.obj_list[i1][2];

   mwWM.obj_list[i1][0] = mwWM.obj_list[i2][0];
   mwWM.obj_list[i1][1] = mwWM.obj_list[i2][1];
   mwWM.obj_list[i1][2] = mwWM.obj_list[i2][2];

   mwWM.obj_list[i2][0] = t0;
   mwWM.obj_list[i2][1] = t1;
   mwWM.obj_list[i2][2] = t2;
}

void ge_enemy_initial_position_random(int e, int csw)
{
   int x, y, empt = 0, lb = 0;
   while ((!empt) && (lb < 10000))
   {
      lb++;
      x = rand() % 100;
      y = rand() % 100;

      empt = 1;
      if (csw) // confine to selection window
      {
         if ((x < mwWM.bx1) || (x >= mwWM.bx2+1)) empt = 0;
         if ((y < mwWM.by1) || (y >= mwWM.by2+1)) empt = 0;
      }
      if (!is_block_empty(x, y, 1, 1, 1)) empt = 0;
   }
   if (empt)
   {
      Efi[e][0] = al_itofix(x)*20;
      Efi[e][1] = al_itofix(y)*20;
   }
   else printf("could not find empty block space for enemy\n");
}

void ge_item_initial_position_random(int i, int csw)
{
   int x, y, empt = 0, lb = 0;
   while ((!empt) && (lb < 10000))
   {
      lb++;
      x = rand() % 100;
      y = rand() % 100;

      empt = 1;
      if (1) // confine to selection window
      {
         if ((x < mwWM.bx1) || (x >= mwWM.bx2+1)) empt = 0;
         if ((y < mwWM.by1) || (y >= mwWM.by2+1)) empt = 0;
      }

      if (!is_block_empty(x, y, 1, 1, 1)) empt = 0;
   }
   if (empt)
   {
      item[i][4] = x*20;
      item[i][5] = y*20;
      itemf[i][0] = al_itofix(item[i][4]);
      itemf[i][1] = al_itofix(item[i][5]);
   }
   else printf("could not find empty block space for item\n");
}

int ge_draw_list_items(int x1, int y1, int flash_color, int ni)
{
   int xt = x1; // to keep track of pos as we expand in the x axis
   if (ni == 0)
   {
      al_draw_text(mF.pr8, palette_color[15], xt, y1, 0, "<empty>");
      xt+=56;
   }
   else
   {
      // column headers
      al_draw_text(mF.pr8, palette_color[15], xt, y1, 0, " #");  xt+=24;
      al_draw_text(mF.pr8, palette_color[15], xt, y1, 0, "S");   xt+=16;
      al_draw_text(mF.pr8, palette_color[15], xt, y1, 0, "num"); xt+=32;
      for (int ge_num=0; ge_num<40; ge_num++)
         if ((ge_data[ge_num].valid) && (!ge_data[ge_num].collapsed) && (ge_data[ge_num].vartyp != 5) && (ge_data[ge_num].vartyp != 6))
         {
            al_draw_textf(mF.pr8, palette_color[15], xt, y1, 0, "%.4s", ge_data[ge_num].name);
            xt+=40;
         }

      // rows
      for (int i=0; i<NUM_OBJ; i++)
         if (mwWM.obj_list[i][0])
         {
            y1+=8;
            int type = mwWM.obj_list[i][0];
            int num = mwWM.obj_list[i][1];
            int col = 15;
            if (mwWM.obj_list[i][2]) col = flash_color;

            xt = x1; // to keep track of pos as we expand in the x axis

            al_draw_textf(mF.pr8, palette_color[col], xt, y1, 0, "%2d",i); xt+=24; // list item number

            int tn = 0;
            if (type == 2) tn = item_tile[item[num][0]];
            if (type == 3) tn = enemy_tile[Ei[num][0]];
            al_draw_scaled_bitmap(mwB.tile[tn], 0, 0, 20, 20, xt, y1, 8, 8, 0); xt+=16; // bitmap

            al_draw_textf(mF.pr8, palette_color[col], xt, y1, 0, "%3d", num); xt+=32; // object number

            // valid variables
            for (int ge_num=0; ge_num<40; ge_num++)
            if ((ge_data[ge_num].valid) && (!ge_data[ge_num].collapsed) && (ge_data[ge_num].vartyp != 5) && (ge_data[ge_num].vartyp != 6))
               {
                  int vt = ge_data[ge_num].vartyp;
                  int vn = ge_data[ge_num].varnum;

                  if (vt == 2) al_draw_textf(mF.pr8, palette_color[col], xt, y1, 0, "%2d", item[num][vn]);
                  if (vt == 3) al_draw_textf(mF.pr8, palette_color[col], xt, y1, 0, "%2d", Ei[num][vn]);
                  if (vt == 4) al_draw_textf(mF.pr8, palette_color[col], xt, y1, 0, "%2.1f", al_fixtof(Efi[num][vn]));
                  if (vt == 5) al_draw_textf(mF.pr8, palette_color[col], xt, y1, 0, "%2d", Ei[num][2]);
                  xt+=40;
               }
         }
   }
   return xt;
}

void ge_show_obj_list(int x, int y, int *ew, int *eh, int d)
{

   // find number of items in list
   int ni = 0;
   for (int i=0; i<NUM_OBJ; i++)
      if (mwWM.obj_list[i][0]) ni++;

   int fs = 12;
   int y1 = y + fs;

   int x1 = x+fs;
   int x2 = x1+100; // minimum size, will be expanded if needed

   int yf1 = y;
   int yf2 = y + (ni+1)*8 + fs*2;

   x2 = ge_draw_list_items(x1, y1, flash_color, ni);

   *ew = (x2+fs) - x ; // actual width
   *eh = yf2 - y;      // actual height

   // draw frame around list
   int ci = 16; //color inc
   for (int q=0; q<fs; q++)
      al_draw_rectangle(x1-fs+q, yf1+q, x2+fs-q, yf2-q, palette_color[12+32+(q*ci)], 1);
   al_draw_textf(mF.pr8, palette_color[15], (x1+x2)/2, yf1+2, ALLEGRO_ALIGN_CENTER, "%d Objects", ni);

   if ((!d) && (mouse_x > x1) && (mouse_x < x2) && (mouse_y > yf1) && (mouse_y < yf2)) // detect if mouse is on list
   {
      for (int o=0; o<NUM_OBJ; o++) mwWM.obj_list[o][2] = 0; // clear all highlights
      int mpl = ((mouse_y - yf1 + fs)/8)-4;             // get raw list item
      if ((mpl < -1) || (mpl > ni-1)) mpl = -1;         // ensure valid list item
      if (mpl != -1)                                    // mouse is on valid list item
      {
         mwWM.obj_list[mpl][2] = 1;                          // mark this item as highlighted
         al_draw_rectangle(x1+1, yf1+fs+(mpl+1)*8, x2-1, yf1+fs+(mpl+2)*8, palette_color[10], 1); // draw rectangle around list item
         if (mouse_b[1][0])
         {
            if (SHFT()) // remove item from list
            {
               while (mouse_b[1][0]) proc_event_queue();
               ge_remove_obj_list_item(mpl);
            }
            else // keep mouse for drag swap
            {
               int old_mpl = mpl;
               while (mouse_b[1][0])
               {
                  mwWM.redraw_level_editor_background();
                  get_new_screen_buffer(3, 0, 0);
                  mwWM.cycle_windows(1); // draw only

                  mpl = ((mouse_y - yf1 + fs)/8)-4;             // get raw list item
                  if ((mpl < -1) || (mpl > ni-1)) mpl = -1;     // ensure valid list item
                  if (mpl != -1) al_draw_rectangle(x1+1, yf1+fs+(mpl+1)*8, x2-1, yf1+fs+(mpl+2)*8, palette_color[10], 1); // if mouse is on valid list item, draw rectangle around list item pointer
               }
               if (mpl != -1) ge_swap_obj_list_items(old_mpl, mpl); // if mouse is on valid list item, do the swap
            } // mouse b1 held
         } // mouse_b[1][0] pressed
      } // mouse on valid list item
   } // mouse on obj list
}

int ge_show_controls(int x, int y, int *ew, int *eh, int hidden, int d)
{
   int nc = 0; // number of valid controls


   int by = y;
   int bts = 16;
   int y_spacing = 6; // spacing between groups

   // find number of items in list
   int ni = 0;
   for (int i=0; i<NUM_OBJ; i++)
      if (mwWM.obj_list[i][0]) ni++;

   if (ni)
   {
      ge_set_valid_controls();

      for (int ge_num=0; ge_num<40; ge_num++)
         if (ge_data[ge_num].valid)
         {
            nc++;
            int gvt = ge_data[ge_num].vartyp;
            int gvn = ge_data[ge_num].varnum;
            int xa = x;

            int x1 = xa;
            int x2 = x1 + 320;
            int x3 = x2 - 60;
            int x4 = x1 + 160;

            sprintf(msg, "Group Edit - %s", ge_data[ge_num].name);
            mdw_toggle(x1, by, x2, bts, 0,0,0,0, 0,0,0,0, 1,1,1,d, ge_data[ge_num].collapsed, msg, msg,  15, 15, 12, 15+64);

            if (!ge_data[ge_num].collapsed)
            {
               if ((gvt == 2) || (gvt == 3) || (gvt == 4))
               {
                  // find min max and average
                  float mins = 9999;
                  float maxs = -9999;
                  float tally = 0;

                  for (int i=0; i<NUM_OBJ; i++)
                     if (mwWM.obj_list[i][0])
                     {
                        int num = mwWM.obj_list[i][1];

                        float val = 0;
                        if (gvt == 2) val = (float)  item[num][gvn];
                        if (gvt == 3) val = (float)    Ei[num][gvn];
                        if (gvt == 4) val = al_fixtof(Efi[num][gvn]);

                        if (val < mins) mins = val;
                        if (val > maxs) maxs = val;
                        tally += val;
                     }
                  float avg = tally / (float) ni;

                  sprintf(msg, "Current - min:%2.1f max:%2.1f avg:%2.1f", mins, maxs, avg);
                  mdw_buttont(x1+8, by, x2, bts, 0,0,0,0,  0,13,15,0,1,0,1,d, msg);

                  float mna = ge_data[ge_num].min_allowed;
                  float mxa = ge_data[ge_num].max_allowed;

                  mdw_sliderd(x1+8, by, x4, bts, 0,0,0,0, 0,3,15,0,1,0,0,d, ge_data[ge_num].adj_min, mxa, mna, .1, "Min:");
                  mdw_sliderd(x4,   by, x2, bts, 0,0,0,0, 0,3,15,0,1,0,1,d, ge_data[ge_num].adj_max, mxa, mna, .1, "Max:");

                  mdw_buttonp(    x1+8, by, x3, bts,   100,0,0,0,  0, 4,15,0,1,1,0,d, ge_data[ge_num].adj_mode); // Action type
                  if (mdw_buttont(x3,  by, x2, bts,      0,0,0,0,  0,10,15,0,1,0,1,d, "Do It!"))
                  {
                     float mn = ge_data[ge_num].adj_min;
                     float mx = ge_data[ge_num].adj_max;
                     if (ge_data[ge_num].adj_mode == 0) // randomize
                     {
                        for (int i=0; i<NUM_OBJ; i++)
                           if (mwWM.obj_list[i][0])
                           {
                              int num = mwWM.obj_list[i][1];
                              float val = mdw_rnd(mn, mx);

                              if (gvt == 2) item[num][gvn] = (float) val;
                              if (gvt == 3) Ei[num][gvn]   = (float) val;
                              if (gvt == 4) Efi[num][gvn]  = al_ftofix(val);
                              if ((gvt == 4) && (gvn == 9)) recalc_pod(num);
                              if ((gvt == 4) && (gvn == 5) && ((Ei[num][0] == 4) || (Ei[num][0] == 6))) scale_bouncer_and_cannon_speed(num); // cannon and bouncer speed only
                           }
                     }
                     if (ge_data[ge_num].adj_mode == 1) // increment
                     {
                        float inc = (mx-mn)/ni;
                        float val = mn;
                        for (int i=0; i<NUM_OBJ; i++)
                           if (mwWM.obj_list[i][0])
                           {
                              int num = mwWM.obj_list[i][1];
                              if (gvt == 2) item[num][gvn] = (float) val;
                              if (gvt == 3) Ei[num][gvn]   = (float) val;
                              if (gvt == 4) Efi[num][gvn]  = al_ftofix(val);
                              if ((gvt == 4) && (gvn == 9)) recalc_pod(num);
                              if ((gvt == 4) && (gvn == 5) && ((Ei[num][0] == 4) || (Ei[num][0] == 6))) scale_bouncer_and_cannon_speed(num); // cannon and bouncer speed only
                              val += inc;
                           }
                     }
                     if (ge_data[ge_num].adj_mode == 2) // set all to min
                     {
                        float val = mn;
                        for (int i=0; i<NUM_OBJ; i++)
                           if (mwWM.obj_list[i][0])
                           {
                              int num = mwWM.obj_list[i][1];
                              if (gvt == 2) item[num][gvn] = (float) val;
                              if (gvt == 3) Ei[num][gvn]   = (float) val;
                              if (gvt == 4) Efi[num][gvn]  = al_ftofix(val);
                              if ((gvt == 4) && (gvn == 9)) recalc_pod(num);
                              if ((gvt == 4) && (gvn == 5) && ((Ei[num][0] == 4) || (Ei[num][0] == 6))) scale_bouncer_and_cannon_speed(num); // cannon and bouncer speed only
                           }
                     }
                  } // end of Do It!
               } // end of types 2, 3, 4
               else if (gvt == 5)
               {
                  if (mdw_buttont(x1+10, by, x4, bts, 0,0,0,0,  0,10,15,0,1,0,1,d, "Random"))
                  {
                     for (int i=0; i<NUM_OBJ; i++) // iterate all items in list
                        if (mwWM.obj_list[i][0])
                        {
                           int num = mwWM.obj_list[i][1];
                           set_xyinc_rot(num, rand() % 2000, rand() % 2000); // random
                        }
                  }
                  if (mdw_buttont(x4, by, x2, bts, 0,0,0,0,  0,10,15,0,1,0,1,d, "Aim at Start Block"))
                  {
                     int sbx = 0, sby = 0; // start block x and y
                     for (int c=0; c<500; c++)
                        if (item[c][0] == 5)
                        {
                           sbx = item[c][4];
                           sby = item[c][5];
                        }
                     for (int i=0; i<NUM_OBJ; i++) // iterate all items in list
                        if (mwWM.obj_list[i][0])
                        {
                           int num = mwWM.obj_list[i][1];
                           set_xyinc_rot(num, sbx, sby); // point at start block
                        }
                  } // end of aim at start block
               } // end of type 5
               else if (gvt == 6)
               {
                  if (mdw_buttont(x1+10, by, x2, bts, 0,0,0,0,  0,10,15,0,1,0,1,d, "Randomize Position Within Selection"))
                  {
                     for (int i=0; i<NUM_OBJ; i++) // iterate all items in list
                        if (mwWM.obj_list[i][0])
                        {
                           int typ = mwWM.obj_list[i][0];
                           int num = mwWM.obj_list[i][1];
                           if (typ == 2) ge_item_initial_position_random(num, 1);
                           if (typ == 3) ge_enemy_initial_position_random(num, 1);
                        }
                  }
               }
            } // end of not collapsed
            by+=y_spacing;
         } // end of valid ge_num
   } // end of more than 0 items in list

   *ew = 322; // fixed width for now
   *eh = by-y-y_spacing;
   return nc;
}

void ge_add_selection_to_list(int set_filters)
{
   // add everything in selection to list...
   int rx1 = mwWM.bx1*20;
   int ry1 = mwWM.by1*20;
   int rx2 = mwWM.bx2*20+20;
   int ry2 = mwWM.by2*20+20;

   al_fixed frx1 = al_itofix(rx1);
   al_fixed fry1 = al_itofix(ry1);
   al_fixed frx2 = al_itofix(rx2);
   al_fixed fry2 = al_itofix(ry2);

   for (int b=0; b<100; b++) // add enemies in selection
      if ((Ei[b][0]) && (Efi[b][0] >= frx1) && (Efi[b][0] < frx2) && (Efi[b][1] >= fry1) && (Efi[b][1] < fry2)) ge_add_to_obj_list(3, b);

   for (int b=0; b<500; b++) // add items in selection
      if ((item[b][0]) && (item[b][4] >= rx1) && (item[b][4] < rx2) && (item[b][5] >= ry1) && (item[b][5] < ry2)) ge_add_to_obj_list(2, b);

   // set filters to what is actually in the list
   if (set_filters)
   {
      for (int i=0; i<NUM_OBJ; i++)
      if (mwWM.obj_list[i][0])
      {
         int typ = mwWM.obj_list[i][0];
         int num = mwWM.obj_list[i][1];
         if (typ == 2) mwWM.obj_filter[typ][item[num][0]] = 1;
         if (typ == 3) mwWM.obj_filter[typ][  Ei[num][0]] = 1;
      }
   }
}

void ge_process_mouse(void)
{
   if (mouse_b[1][0])
   {
      if (mwWM.mW[5].show_sel_frame) // get new selection rectangle
      {
         mwWM.get_new_box();
         if (SHFT()) ge_add_selection_to_list(1); // add everything in selection to list and set filters...
      }
      else
      {
         while (mouse_b[1][0]) proc_event_queue();

         // is mouse on item
         for (int i=0; i<500; i++)
         {
            int type = item[i][0];
            if ((type) && (mwWM.obj_filter[2][type])) // filter for this type of item
            {
               int sox = item[i][4]/20;
               int soy = item[i][5]/20;
               if ((mwWM.gx == sox) && (mwWM.gy == soy)) ge_add_to_obj_list(2, i);
            }
         }
         // is mouse on enemy
         for (int e=0; e<100; e++)
         {
            int type = Ei[e][0];
            if ((type) && (mwWM.obj_filter[3][type])) // filter for this type of enemy
            {
               int sox = al_fixtoi(Efi[e][0]/20);
               int soy = al_fixtoi(Efi[e][1]/20);
               if ((mwWM.gx == sox) && (mwWM.gy == soy)) ge_add_to_obj_list(3, e);
            }
         }
      }
   }
   if (mouse_b[2][0])
   {
      while (mouse_b[2][0]) proc_event_queue();
      mwWM.set_windows(1);
   }
}
