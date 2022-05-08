// e_group_edit_windows.cpp

#include "pm.h"


#define NUM_OBJ 600


// list of objects to edit as a group
int obj_list[NUM_OBJ][3] = {0};

// toggles to filter objects in list based on object type and subtype
int obj_filter[4][20] = {0};

// to keep track of window locations and sizes for erasing and redrawing
int ge_window_array[5][5] = {0};

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

void init_ge_data(void)
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
void set_valid_ge_controls(void)
{
   // set all controls valid by default
   for (int i=0; i<40; i++) if (ge_data[i].vartyp) ge_data[i].valid = 1;

   // remove controls as soon as they don't apply to all objects in list
   for (int i=0; i<NUM_OBJ; i++)
      if (obj_list[i][0])
      {
         int typ = obj_list[i][0];
         int num = obj_list[i][1];

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
            if ((etyp != 3) && (etyp != 8))                                               ge_data[26].valid = 0; // bullet proximity (archwagon, trakbot)

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

void add_to_obj_list(int t, int i)
{
   // check if already in list
   int duplicate = 0;
   for (int o=0; o<NUM_OBJ; o++)
      if ((obj_list[o][0] == t) && (obj_list[o][1] == i)) duplicate = 1;
   if (!duplicate)
   {
      for (int o=0; o<NUM_OBJ; o++)
         if (obj_list[o][0] == 0)
         {
            obj_list[o][0] = t;
            obj_list[o][1] = i;
            o = NUM_OBJ;
         }
   }
}

void remove_obj_list_item(int o)
{
   // slide down to close hole
   for (int i=o; i<NUM_OBJ-1; i++)
   {
      obj_list[i][0] = obj_list[i+1][0];
      obj_list[i][1] = obj_list[i+1][1];
      obj_list[i][2] = obj_list[i+1][2];
   }
}

void swap_obj_list_items(int i1, int i2)
{
   int t0 = obj_list[i1][0];
   int t1 = obj_list[i1][1];
   int t2 = obj_list[i1][2];

   obj_list[i1][0] = obj_list[i2][0];
   obj_list[i1][1] = obj_list[i2][1];
   obj_list[i1][2] = obj_list[i2][2];

   obj_list[i2][0] = t0;
   obj_list[i2][1] = t1;
   obj_list[i2][2] = t2;
}

void remove_obj_list_filtered_items(void)
{
   for (int i=0; i<NUM_OBJ; i++)
      if (obj_list[i][0])
      {
         int type = obj_list[i][0];
         int num = obj_list[i][1];
         if ((type == 2) && (obj_filter[type][item[num][0]] == 0)) remove_obj_list_item(i);
         if ((type == 3) && (obj_filter[type][  Ei[num][0]] == 0)) remove_obj_list_item(i);
      }
}

int draw_filter_toggles(int x1, int x2, int y1)
{
   int fs = 12;   // frame size
   int y = y1+fs; // button y position

   int bts = 12;        // button size
   int a = 0;       // keep track of button y spacing

   int tc1 = 15;    // text color 1
   int tc2 = 15;    // text color 2
   int fc1 = 15+64; // frame color 1
   int fc2 = 4;     // frame color 2

   mdw_toggle(x1+fs, y+a*bts, x2-fs, y+(a+1)*bts-2, 1000, 0,0,0,0,0,0,0,1,0,0,0, obj_filter[3][3],  "Arcwgn", "Arcwgn", tc1, tc2, fc1, fc2); a++;
   mdw_toggle(x1+fs, y+a*bts, x2-fs, y+(a+1)*bts-2, 1000, 0,0,0,0,0,0,0,1,0,0,0, obj_filter[3][4],  "Bouncr", "Bouncr", tc1, tc2, fc1, fc2); a++;
   mdw_toggle(x1+fs, y+a*bts, x2-fs, y+(a+1)*bts-2, 1000, 0,0,0,0,0,0,0,1,0,0,0, obj_filter[3][6],  "Cannon", "Cannon", tc1, tc2, fc1, fc2); a++;
   mdw_toggle(x1+fs, y+a*bts, x2-fs, y+(a+1)*bts-2, 1000, 0,0,0,0,0,0,0,1,0,0,0, obj_filter[3][7],  "Podzil", "Podzil", tc1, tc2, fc1, fc2); a++;
   mdw_toggle(x1+fs, y+a*bts, x2-fs, y+(a+1)*bts-2, 1000, 0,0,0,0,0,0,0,1,0,0,0, obj_filter[3][8],  "Trakbt", "Trakbt", tc1, tc2, fc1, fc2); a++;
   mdw_toggle(x1+fs, y+a*bts, x2-fs, y+(a+1)*bts-2, 1000, 0,0,0,0,0,0,0,1,0,0,0, obj_filter[3][9],  "Cloner", "Cloner", tc1, tc2, fc1, fc2); a++;
   mdw_toggle(x1+fs, y+a*bts, x2-fs, y+(a+1)*bts-2, 1000, 0,0,0,0,0,0,0,1,0,0,0, obj_filter[3][10], "Field",  "Field",  tc1, tc2, fc1, fc2); a++;
   mdw_toggle(x1+fs, y+a*bts, x2-fs, y+(a+1)*bts-2, 1000, 0,0,0,0,0,0,0,1,0,0,0, obj_filter[3][11], "Blk Wk", "Blk Wk", tc1, tc2, fc1, fc2); a++;
   mdw_toggle(x1+fs, y+a*bts, x2-fs, y+(a+1)*bts-2, 1000, 0,0,0,0,0,0,0,1,0,0,0, obj_filter[3][12], "Flappr", "Flappr", tc1, tc2, fc1, fc2); a++;
   a++;
   mdw_toggle(x1+fs, y+a*bts, x2-fs, y+(a+1)*bts-2, 1000, 0,0,0,0,0,0,0,1,0,0,0, obj_filter[2][1],  "Door",   "Door",   tc1, tc2, fc1, fc2); a++;
   mdw_toggle(x1+fs, y+a*bts, x2-fs, y+(a+1)*bts-2, 1000, 0,0,0,0,0,0,0,1,0,0,0, obj_filter[2][2],  "Bonus",  "Bonus",  tc1, tc2, fc1, fc2); a++;
   mdw_toggle(x1+fs, y+a*bts, x2-fs, y+(a+1)*bts-2, 1000, 0,0,0,0,0,0,0,1,0,0,0, obj_filter[2][3],  "Exit",   "Exit",   tc1, tc2, fc1, fc2); a++;
   mdw_toggle(x1+fs, y+a*bts, x2-fs, y+(a+1)*bts-2, 1000, 0,0,0,0,0,0,0,1,0,0,0, obj_filter[2][4],  "Key",    "Key",    tc1, tc2, fc1, fc2); a++;
   mdw_toggle(x1+fs, y+a*bts, x2-fs, y+(a+1)*bts-2, 1000, 0,0,0,0,0,0,0,1,0,0,0, obj_filter[2][5],  "Start",  "Start",  tc1, tc2, fc1, fc2); a++;
   mdw_toggle(x1+fs, y+a*bts, x2-fs, y+(a+1)*bts-2, 1000, 0,0,0,0,0,0,0,1,0,0,0, obj_filter[2][7],  "Mine",   "Mine",   tc1, tc2, fc1, fc2); a++;
   mdw_toggle(x1+fs, y+a*bts, x2-fs, y+(a+1)*bts-2, 1000, 0,0,0,0,0,0,0,1,0,0,0, obj_filter[2][8],  "Bomb",   "Bomb",   tc1, tc2, fc1, fc2); a++;
   mdw_toggle(x1+fs, y+a*bts, x2-fs, y+(a+1)*bts-2, 1000, 0,0,0,0,0,0,0,1,0,0,0, obj_filter[2][9],  "Triggr", "Triggr", tc1, tc2, fc1, fc2); a++;
   mdw_toggle(x1+fs, y+a*bts, x2-fs, y+(a+1)*bts-2, 1000, 0,0,0,0,0,0,0,1,0,0,0, obj_filter[2][10], "Messge", "Messge", tc1, tc2, fc1, fc2); a++;
   mdw_toggle(x1+fs, y+a*bts, x2-fs, y+(a+1)*bts-2, 1000, 0,0,0,0,0,0,0,1,0,0,0, obj_filter[2][11], "Rocket", "Rocket", tc1, tc2, fc1, fc2); a++;
   mdw_toggle(x1+fs, y+a*bts, x2-fs, y+(a+1)*bts-2, 1000, 0,0,0,0,0,0,0,1,0,0,0, obj_filter[2][12], "Warp",   "Warp",   tc1, tc2, fc1, fc2); a++;
   mdw_toggle(x1+fs, y+a*bts, x2-fs, y+(a+1)*bts-2, 1000, 0,0,0,0,0,0,0,1,0,0,0, obj_filter[2][14], "Switch", "Switch", tc1, tc2, fc1, fc2); a++;
   mdw_toggle(x1+fs, y+a*bts, x2-fs, y+(a+1)*bts-2, 1000, 0,0,0,0,0,0,0,1,0,0,0, obj_filter[2][15], "Spring", "Spring", tc1, tc2, fc1, fc2); a++;
   mdw_toggle(x1+fs, y+a*bts, x2-fs, y+(a+1)*bts-2, 1000, 0,0,0,0,0,0,0,1,0,0,0, obj_filter[2][16], "Blk Mn", "Blk Mn", tc1, tc2, fc1, fc2); a++;
   mdw_toggle(x1+fs, y+a*bts, x2-fs, y+(a+1)*bts-2, 1000, 0,0,0,0,0,0,0,1,0,0,0, obj_filter[2][17], "Blk Dm", "Blk Dm", tc1, tc2, fc1, fc2); a++;

   // draw frame around filter buttons
   int y2 = y1+a*bts+fs*2;
   int ci = 16; //color inc
   for (int q=0; q<fs; q++)
      al_draw_rectangle(x1+q, y1+q, x2-q, y2-q, palette_color[12+32+(q*ci)], 1);
   al_draw_text(font, palette_color[15], (x1+x2)/2, y1+2, ALLEGRO_ALIGN_CENTER, "Filters");

  // al_draw_rectangle(x1, y1, x2, y2, palette_color[10], 1);
   return y2;
}


float mdw_rnd(float rmin, float rmax)
{
   int imin = (int) (rmin * 100.0);
   int irng = (int) ((rmax-rmin) * 100.0);
   int s = imin + rand() % irng;
   float res = (float)s / 100.0;
   return res;
}

int enemy_initial_position_random(int e, int csw)
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
         if ((x < stx) || (x >= sux)) empt = 0;
         if ((y < sty) || (y >= suy)) empt = 0;
      }
      if (!is_block_empty(x, y, 1, 1, 1)) empt = 0;
   }
   if (empt)
   {
      Efi[e][0] = al_itofix(x)*20;
      Efi[e][1] = al_itofix(y)*20;
      return 1;
   }
   else
   {
      printf("could not find empty\n");
      return 0;
   }
}

void item_initial_position_random(int i, int csw)
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
         if ((x < stx) || (x >= sux)) empt = 0;
         if ((y < sty) || (y >= suy)) empt = 0;
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
   else printf("could not find empty\n");
}

int draw_list_items(int x1, int y1, int flash_color, int ni)
{
   int x2 = x1+100+12;
   int y2 = y1 + (ni+1)*8;
   int xt = x1; // to keep track of pos as we expand in the x axis

   // erase background
   al_draw_filled_rectangle(x1, y1, x2, y2, palette_color[0]);

   // column headers
   al_draw_text(font, palette_color[15], xt, y1, 0, " #");  xt+=24;
   al_draw_text(font, palette_color[15], xt, y1, 0, "S");   xt+=16;
   al_draw_text(font, palette_color[15], xt, y1, 0, "num"); xt+=32;
   for (int ge_num=0; ge_num<40; ge_num++)
      if ((ge_data[ge_num].valid) && (!ge_data[ge_num].collapsed) && (ge_data[ge_num].vartyp != 5) && (ge_data[ge_num].vartyp != 6))
      {
         al_draw_textf(font, palette_color[15], xt, y1, 0, "%.4s", ge_data[ge_num].name);
         xt+=40;
      }

   // rows
   for (int i=0; i<NUM_OBJ; i++)
      if (obj_list[i][0])
      {
         y1+=8;
         int type = obj_list[i][0];
         int num = obj_list[i][1];
         int col = 15;
         if (obj_list[i][2]) col = flash_color;

         xt = x1; // to keep track of pos as we expand in the x axis

         al_draw_textf(font, palette_color[col], xt, y1, 0, "%2d",i); xt+=24; // list item number

         int tn = 0;
         if (type == 2) tn = item_tile[item[num][0]];
         if (type == 3) tn = enemy_tile[Ei[num][0]];
         al_draw_scaled_bitmap(tile[tn], 0, 0, 20, 20, xt, y1, 8, 8, 0); xt+=16; // bitmap

         al_draw_textf(font, palette_color[col], xt, y1, 0, "%3d", num); xt+=32; // object number

         // valid variables
         for (int ge_num=0; ge_num<40; ge_num++)
         if ((ge_data[ge_num].valid) && (!ge_data[ge_num].collapsed) && (ge_data[ge_num].vartyp != 5) && (ge_data[ge_num].vartyp != 6))
            {
               int vt = ge_data[ge_num].vartyp;
               int vn = ge_data[ge_num].varnum;

               if (vt == 2) al_draw_textf(font, palette_color[col], xt, y1, 0, "%2d", item[num][vn]);
               if (vt == 3) al_draw_textf(font, palette_color[col], xt, y1, 0, "%2d", Ei[num][vn]);
               if (vt == 4) al_draw_textf(font, palette_color[col], xt, y1, 0, "%2.1f", al_fixtof(Efi[num][vn]));
               if (vt == 5) al_draw_textf(font, palette_color[col], xt, y1, 0, "%2d", Ei[num][2]);
               xt+=40;
            }
      }
   return xt;
}

int show_obj_list(int x, int y, int *ex2)
{
   // find number of items in list
   int ni = 0;
   for (int i=0; i<NUM_OBJ; i++)
      if (obj_list[i][0]) ni++;

   int fs = 12;
   int y1 = y + fs;

   int x1 = x+fs;
   int x2 = x1+100; // minimum size, will be expanded if needed

   int yf1 = y;
   int yf2 = y + (ni+1)*8 + fs*2;

   if (ni < 1) al_draw_textf(font, palette_color[15], x1, y1, 0, "<empty>");
   else
   {
      x2 = draw_list_items(x1, y1, flash_color, ni);
      if ((mouse_x > x1) && (mouse_x < x2) && (mouse_y > yf1) && (mouse_y < yf2)) // detect if mouse is on list
      {
         for (int o=0; o<NUM_OBJ; o++) obj_list[o][2] = 0; // clear all highlights
         int mpl = ((mouse_y - yf1 + fs)/8)-4;             // get raw list item
         if ((mpl < -1) || (mpl > ni-1)) mpl = -1;         // ensure valid list item
         if (mpl != -1)                                    // mouse is on valid list item
         {
            obj_list[mpl][2] = 1;                          // mark this item as highlighted
            al_draw_rectangle(x1+1, yf1+fs+(mpl+1)*8, x2-1, yf1+fs+(mpl+2)*8, palette_color[10], 1); // draw rectangle around list item
            if (mouse_b1)
            {
               if ((key[ALLEGRO_KEY_LSHIFT]) || (key[ALLEGRO_KEY_RSHIFT])) // remove item from list
               {
                  while (mouse_b1) proc_controllers();
                  remove_obj_list_item(mpl);
               }
               else // keep mouse for drag swap
               {
                  int old_mpl = mpl;
                  while (mouse_b1)
                  {
                     al_flip_display();
                     proc_controllers();
                     mpl = ((mouse_y - yf1 + fs)/8)-4;             // get raw list item
                     if ((mpl < -1) || (mpl > ni-1)) mpl = -1;     // ensure valid list item
                     if (mpl != -1)                                // mouse is on valid list item
                     {
                        draw_list_items(x1, y1, 10, ni);
                        al_draw_rectangle(x1+1, yf1+fs+(mpl+1)*8, x2-1, yf1+fs+(mpl+2)*8, palette_color[10], 1); // draw rectangle around list item pointer
                     }
                  }
                  swap_obj_list_items(old_mpl, mpl); // do the swap
               }
            } // mouse_b1 pressed
         } // mouse on valid list item
      } // mouse on obj list
   } // more than zero items

   // draw frame around list
   int ci = 16; //color inc
   for (int q=0; q<fs; q++)
      al_draw_rectangle(x1-fs+q, yf1+q, x2+fs-q, yf2-q, palette_color[12+32+(q*ci)], 1);
   al_draw_textf(font, palette_color[15], (x1+x2)/2, yf1+2, ALLEGRO_ALIGN_CENTER, "%d Objects", ni);
  *ex2 = x2 + fs; // set the actual width here
  return yf2;
}

int show_ge_controls(int gx, int gy)
{
   // find number of items in list
   int ni = 0;
   for (int i=0; i<NUM_OBJ; i++)
      if (obj_list[i][0]) ni++;

   if (ni)
   {
      set_valid_ge_controls();

      gy-=6;
      for (int ge_num=0; ge_num<40; ge_num++)
         if (ge_data[ge_num].valid)
         {
            int gvt = ge_data[ge_num].vartyp;
            int gvn = ge_data[ge_num].varnum;
            int xa = gx;

            int x1 = xa;
            int x2 = x1 + 320;
            int x3 = x2 - 60;
            int x4 = x1 + 160;

            gy+=6; // spacing between groups
            int y = gy;

            int a = 0;
            int bts = 16;

            sprintf(msg, "Group Edit - %s", ge_data[ge_num].name);
            mdw_toggle(x1, y+a*bts, x2, y+(a+1)*bts-2, 1000, 0,0,0,0,0,0,0,1,1,0,0, ge_data[ge_num].collapsed, msg, msg,  15, 15, 12, 15+64); a++;
            gy+=bts;
            if (!ge_data[ge_num].collapsed)
            {
               if ((gvt == 2) || (gvt == 3) || (gvt == 4))
               {
                  gy+= bts*3;
                  // find min max and average
                  float mins = 9999;
                  float maxs = -9999;
                  float tally = 0;

                  for (int i=0; i<NUM_OBJ; i++)
                     if (obj_list[i][0])
                     {
                        int num = obj_list[i][1];

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
                  mdw_buttont(x1+8, y+a*bts, x2, y+(a+1)*bts-2, 0,0,0,0,  0,13,15,0,1,0,0,0, msg); a++;

                  float mna = ge_data[ge_num].min_allowed;
                  float mxa = ge_data[ge_num].max_allowed;

                  mdw_slider2_flt(x1+8, y+a*bts, x4, y+(a+1)*bts-2, 0,0,0,0, 0,3,15,0,1,0,0,0, ge_data[ge_num].adj_min, mxa, mna, 1, "Min:");
                  mdw_slider2_flt(x4,   y+a*bts, x2, y+(a+1)*bts-2, 0,0,0,0, 0,3,15,0,1,0,0,0, ge_data[ge_num].adj_max, mxa, mna, 1, "Max:"); a++;

                  mdw_buttonp(    x1+8, y+a*bts, x3, y+(a+1)*bts-2, 100,0,0,0,  0, 4,15,0,1,1,0,0, ge_data[ge_num].adj_mode); // Action type
                  if (mdw_buttont(x3,   y+a*bts, x2, y+(a+1)*bts-2, 0,0,0,0,    0,10,15,0,1,0,0,0, "Do It!"))
                  {
                     float mn = ge_data[ge_num].adj_min;
                     float mx = ge_data[ge_num].adj_max;
                     if (ge_data[ge_num].adj_mode == 0) // randomize
                     {
                        for (int i=0; i<NUM_OBJ; i++)
                           if (obj_list[i][0])
                           {
                              int num = obj_list[i][1];
                              float val = mdw_rnd(mn, mx);

                              if (gvt == 2) item[num][gvn] = (float) val;
                              if (gvt == 3) Ei[num][gvn]   = (float) val;
                              if (gvt == 4) Efi[num][gvn]  = al_ftofix(val);
                              if ((gvt == 4) && (gvn == 9)) recalc_pod(num);

                           }
                     }
                     if (ge_data[ge_num].adj_mode == 1) // increment
                     {
                        float inc = (mx-mn)/ni;
                        float val = mn;
                        for (int i=0; i<NUM_OBJ; i++)
                           if (obj_list[i][0])
                           {
                              int num = obj_list[i][1];
                              if (gvt == 2) item[num][gvn] = (float) val;
                              if (gvt == 3) Ei[num][gvn]   = (float) val;
                              if (gvt == 4) Efi[num][gvn]  = al_ftofix(val);
                              if ((gvt == 4) && (gvn == 9)) recalc_pod(num);
                              val += inc;
                           }
                     }
                     if (ge_data[ge_num].adj_mode == 2) // set all to min
                     {
                        float val = mn;
                        for (int i=0; i<NUM_OBJ; i++)
                           if (obj_list[i][0])
                           {
                              int num = obj_list[i][1];
                              if (gvt == 2) item[num][gvn] = (float) val;
                              if (gvt == 3) Ei[num][gvn]   = (float) val;
                              if (gvt == 4) Efi[num][gvn]  = al_ftofix(val);
                              if ((gvt == 4) && (gvn == 9)) recalc_pod(num);
                           }
                     }
                  } // end of Do It!
               } // end of types 2, 3, 4
               else if (gvt == 5)
               {
                  gy+=bts;
                  if (mdw_buttont(x1+10, y+a*bts, x4, y+(a+1)*bts-2, 0,0,0,0,  0,10,15,0,1,0,0,0, "Random"))
                  {
                     for (int i=0; i<NUM_OBJ; i++) // iterate all items in list
                        if (obj_list[i][0])
                        {
                           int num = obj_list[i][1];
                           set_xyinc_rot(num, rand() % 2000, rand() % 2000); // random
                        }
                  }
                  if (mdw_buttont(x4, y+a*bts, x2, y+(a+1)*bts-2, 0,0,0,0,  0,10,15,0,1,0,0,0, "Aim at Start Block"))
                  {
                     int sbx = 0, sby = 0; // start block x and y
                     for (int c=0; c<500; c++)
                        if (item[c][0] == 5)
                        {
                           sbx = item[c][4];
                           sby = item[c][5];
                        }
                     for (int i=0; i<NUM_OBJ; i++) // iterate all items in list
                        if (obj_list[i][0])
                        {
                           int num = obj_list[i][1];
                           set_xyinc_rot(num, sbx, sby); // point at start block
                        }
                  } // end of aim at start block
               } // end of type 5
               else if (gvt == 6)
               {
                  gy+=bts;
                  if (mdw_buttont(x1+10, y+a*bts, x2, y+(a+1)*bts-2, 0,0,0,0,  0,10,15,0,1,0,0,0, "Randomize Position Within Selection"))
                  {
                     for (int i=0; i<NUM_OBJ; i++) // iterate all items in list
                        if (obj_list[i][0])
                        {
                           int typ = obj_list[i][0];
                           int num = obj_list[i][1];
                           if (typ == 2) item_initial_position_random(num, 1);
                           if (typ == 3) enemy_initial_position_random(num, 1);
                        }
                  }
               }
            } // end of not collapsed
         } // end of valid ge_num
   } // end of more than 0 itsems in list
   return gy;
}



void add_selection_to_list(int stx, int sty, int sux, int suy)
{
   // add everything in selection to list...
   int rx1 = stx *20;    // source x
   int ry1 = sty *20;    // source y
   int rx2 = sux *20;    // sizes
   int ry2 = suy *20;

   al_fixed frx1 = al_itofix(rx1); // source position and size
   al_fixed fry1 = al_itofix(ry1);
   al_fixed frx2 = al_itofix(rx2);
   al_fixed fry2 = al_itofix(ry2);

   for (int b=0; b<100; b++) // add enemies in selection
      if ((Ei[b][0]) && (Efi[b][0] >= frx1) && (Efi[b][0] < frx2) && (Efi[b][1] >= fry1) && (Efi[b][1] < fry2)) add_to_obj_list(3, b);

   for (int b=0; b<500; b++) // add items in selection
      if ((item[b][0]) && (item[b][4] >= rx1) && (item[b][4] < rx2) && (item[b][5] >= ry1) && (item[b][5] < ry2)) add_to_obj_list(2, b);


   // set filters to what is actually in the list
   if ((key[ALLEGRO_KEY_LSHIFT]) || (key[ALLEGRO_KEY_RSHIFT]))
   {

      for (int i=0; i<NUM_OBJ; i++)
      if (obj_list[i][0])
      {
         int typ = obj_list[i][0];
         int num = obj_list[i][1];
         if (typ == 2) obj_filter[typ][item[num][0]] = 1;
         if (typ == 3) obj_filter[typ][  Ei[num][0]] = 1;
      }
   }
}



void ge_draw_on_level_buffer(int gx, int gy, int stx, int sty, int sux, int suy, int show_sel_frame, int mouse_on_window)
{
   al_flip_display();
   proc_scale_factor_change();
   proc_controllers();
   proc_frame_delay();
   get_new_background(0);
   draw_lifts();
   draw_items();
   draw_enemies();

   // mark objects on map that are capable of being added to list
   for (int i=0; i<500; i++)
   {
      int type = (item[i][0]);
      if ((type) && (obj_filter[2][type]))
      {
         int sox = item[i][4];
         int soy = item[i][5];
         al_draw_rectangle(sox, soy, sox+20, soy+20, palette_color[13], 1);
      }
   }
   for (int e=0; e<100; e++)
   {
      int type = (Ei[e][0]);
      if ((type) && (obj_filter[3][type]))
      {
         int sox = al_fixtoi(Efi[e][0]);
         int soy = al_fixtoi(Efi[e][1]);
         al_draw_rectangle(sox, soy, sox+20, soy+20, palette_color[13], 1);
      }
   }

   // is mouse on obj already in list?
   for (int i=0; i<NUM_OBJ; i++)
   {
      obj_list[i][2] = 0; // turn off highlight by default
      if (obj_list[i][0])
      {
         int typ = obj_list[i][0];
         int num = obj_list[i][1];
         int hix=0, hiy=0;

         if (typ == 2) // item
         {
            hix = item[num][4]/20;
            hiy = item[num][5]/20;
         }
         if (typ == 3) // enemy
         {
            hix = al_fixtoi(Efi[num][0]/20);
            hiy = al_fixtoi(Efi[num][1]/20);
         }
         if ((gx == hix) && (gy == hiy)) obj_list[i][2] = 1; // turn on highlight for this list item
      }
   }

   // mark objects on map that have already been added to list
   for (int i=0; i<NUM_OBJ; i++)
   {
      if (obj_list[i][0])
      {
         int typ = obj_list[i][0];
         int num = obj_list[i][1];
         int hx=0, hy=0;
         if (typ == 2)
         {
            hx = item[num][4];
            hy = item[num][5];
         }
         if (typ == 3)
         {
            hx = al_fixtoi(Efi[num][0]);
            hy = al_fixtoi(Efi[num][1]);
         }


         if (obj_list[i][2]) al_draw_rectangle(hx-2, hy-2, hx+20+2, hy+20+2, palette_color[flash_color], 1);
         else                al_draw_rectangle(hx, hy, hx+20, hy+20,         palette_color[10], 1);
      }

   }
   if (show_sel_frame)
   {
      int dstx = stx*20;
      if (dstx == 0) dstx = 1;
      int dsty = sty*20;
      if (dsty == 0) dsty = 1;
      al_draw_rectangle(dstx, dsty, (sux*20)-1, (suy*20)-1, palette_color[14], 1);

      al_draw_text(font, palette_color[14], stx*20+2, sty*20+3,  0, "selection");
      //al_draw_textf(font, palette_color[14], stx*20+2, sty*20+3,  0, "selection x1:%d y1:%d x2:%d y2:%d", stx, sty, sux, suy);
   }
   else if (!mouse_on_window) crosshairs_full(gx*20+10, gy*20+10, 15, 1);
}


int ge_draw_on_screen_buffer(int xa, int ya, int &show_sel_frame)
{
   int mouse_on_window = 0;
   int mowt = 4; // mouse on window threshold

   // window positions
   int xb = xa + 300;

   int f = 0; // title bar
   al_draw_filled_rectangle(ge_window_array[f][0], ge_window_array[f][1], ge_window_array[f][2], ge_window_array[f][3], palette_color[0]); // erase
   int tbx1 = xa;
   int tbx2 = xb;
   int tby1 = ya;
   int tby2 = ya+12;

   titlex("Group Edit", 15, 13, tbx1, tbx2, tby1);

   ge_window_array[f][0] = tbx1;
   ge_window_array[f][1] = tby1;
   ge_window_array[f][2] = tbx2;
   ge_window_array[f][3] = tby2;
   if ((mouse_x > ge_window_array[f][0]-mowt) && (mouse_x < ge_window_array[f][2]+mowt) && (mouse_y > ge_window_array[f][1]-mowt) && (mouse_y < ge_window_array[f][3]+mowt))
   {
       mouse_on_window = 1;
       al_draw_rectangle(ge_window_array[f][0], ge_window_array[f][1], ge_window_array[f][2], ge_window_array[f][3], palette_color[14], 1); // frame
   }

   f = 1; // selection and mode button positions
   al_draw_filled_rectangle(ge_window_array[f][0], ge_window_array[f][1], ge_window_array[f][2], ge_window_array[f][3], palette_color[0]); // erase
   int bpx1 = xa;
   int bpx2 = bpx1+300;
   int bpy1 = ya+15;
   int bpy2 = bpy1+12;
   mdw_toggle(bpx1+2, bpy1, bpx1+110, bpy2, 1000, 0,0,0,   0,0,0,0,1,0,0,0, show_sel_frame,  "Selection:OFF", "Selection:ON ", 15, 15, 15+64, 14);
   if (show_sel_frame) if (mdw_buttont(bpx1+120, bpy1, bpx2-2, bpy2, 0,0,0,0,  0,10,15,0,1,0,0,0, "Add Selection to List")) add_selection_to_list(stx, sty, sux, suy);
   ge_window_array[f][0] = bpx1;
   ge_window_array[f][1] = bpy1;
   ge_window_array[f][2] = bpx2;
   ge_window_array[f][3] = bpy2;
   if ((mouse_x > ge_window_array[f][0]-mowt) && (mouse_x < ge_window_array[f][2]+mowt) && (mouse_y > ge_window_array[f][1]-mowt) && (mouse_y < ge_window_array[f][3]+mowt))
   {
       mouse_on_window = 1;
       al_draw_rectangle(ge_window_array[f][0], ge_window_array[f][1], ge_window_array[f][2], ge_window_array[f][3], palette_color[14], 1); // frame
   }


   f = 2; // filter toggle positions
   al_draw_filled_rectangle(ge_window_array[f][0], ge_window_array[f][1], ge_window_array[f][2], ge_window_array[f][3], palette_color[0]); // erase
   int ftx1 = xa;
   int ftx2 = ftx1 + 82;
   int fty1 = bpy2 + 2;
   int fty2 = draw_filter_toggles(ftx1, ftx2, fty1);
   ge_window_array[f][0] = ftx1;
   ge_window_array[f][1] = fty1;
   ge_window_array[f][2] = ftx2;
   ge_window_array[f][3] = fty2;
   if ((mouse_x > ge_window_array[f][0]-mowt) && (mouse_x < ge_window_array[f][2]+mowt) && (mouse_y > ge_window_array[f][1]-mowt) && (mouse_y < ge_window_array[f][3]+mowt))
   {
       mouse_on_window = 1;
       al_draw_rectangle(ge_window_array[f][0], ge_window_array[f][1], ge_window_array[f][2], ge_window_array[f][3], palette_color[14], 1); // frame
   }


   f = 3; // object list positions
   al_draw_filled_rectangle(ge_window_array[f][0], ge_window_array[f][1], ge_window_array[f][2], ge_window_array[f][3], palette_color[0]); // erase
   int olx1 = ftx2+2;
   int olx2 = 0;
   int oly1 = fty1;
   int oly2 = show_obj_list(olx1, oly1, &olx2);
   ge_window_array[f][0] = olx1;
   ge_window_array[f][1] = oly1;
   ge_window_array[f][2] = olx2;
   ge_window_array[f][3] = oly2;
   if ((mouse_x > ge_window_array[f][0]-mowt) && (mouse_x < ge_window_array[f][2]+mowt) && (mouse_y > ge_window_array[f][1]-mowt) && (mouse_y < ge_window_array[f][3]+mowt))
   {
       mouse_on_window = 1;
       al_draw_rectangle(ge_window_array[f][0], ge_window_array[f][1], ge_window_array[f][2], ge_window_array[f][3], palette_color[14], 1); // frame
   }


   f = 4; // ge control positions
   al_draw_filled_rectangle(ge_window_array[f][0]-2, ge_window_array[f][1], ge_window_array[f][2], ge_window_array[f][3], palette_color[0]); // erase
   int gex1 = ftx2+14;
   int gex2 = gex1+322;
   int gey1 = oly2+4;
   int gey2 = gey1+100;
   if (gey1 > SCREEN_H - 200)
   {
      gey1 = ya;
      gex1 = ftx2+200;
   }
   gey2 = show_ge_controls(gex1, gey1);
   ge_window_array[f][0] = gex1;
   ge_window_array[f][1] = gey1;
   ge_window_array[f][2] = gex2;
   ge_window_array[f][3] = gey2;
   if ((mouse_x > ge_window_array[f][0]-mowt) && (mouse_x < ge_window_array[f][2]+mowt) && (mouse_y > ge_window_array[f][1]-mowt) && (mouse_y < ge_window_array[f][3]+mowt))
   {
       mouse_on_window = 1;
       al_draw_rectangle(ge_window_array[f][0], ge_window_array[f][1], ge_window_array[f][2], ge_window_array[f][3], palette_color[14], 1); // frame
   }

   return mouse_on_window;
}




void group_edit(void)
{
   init_level_background();
   int gx=0, gy=0, hx=0, hy=0;

   init_ge_data();

   int show_sel_frame = 1;
   int mouse_on_window = 0;

   int ge_window_x = 400;
   int ge_window_y = 40;

   int exit =0;
   al_show_mouse_cursor(display);
   al_set_target_backbuffer(display);

   while (mouse_b2) proc_controllers();
   while (!exit)
   {
      ge_draw_on_level_buffer(gx, gy, stx, sty, sux, suy, show_sel_frame, mouse_on_window);
      ovw_process_scrolledge();
      ovw_get_block_position_on_map(&gx, &gy, &hx, &hy);
      get_new_screen_buffer(3, 0, 0);

      remove_obj_list_filtered_items();

      process_flash_color();

      mouse_on_window = ge_draw_on_screen_buffer(ge_window_x, ge_window_y, show_sel_frame);

      int f = 0; // title bar
      int mowt = 2;
      if ((mouse_x > ge_window_array[f][0]-mowt) && (mouse_x < ge_window_array[f][2]+mowt) && (mouse_y > ge_window_array[f][1]-mowt) && (mouse_y < ge_window_array[f][3]+mowt))
      {
         if (mouse_b1)
         {
            int mxo = mouse_x - ge_window_x; // get offset from mouse position to window x, y
            int myo = mouse_y - ge_window_y;

            while (mouse_b1)
            {
               ge_window_x = mouse_x - mxo;
               ge_window_y = mouse_y - myo;
               ge_draw_on_level_buffer(gx, gy, stx, sty, sux, suy, show_sel_frame, mouse_on_window);
               get_new_screen_buffer(3, 0, 0);
               mouse_on_window = ge_draw_on_screen_buffer(ge_window_x, ge_window_y, show_sel_frame);

            } // mouse b1 held
         } // mouse b1 pressed
      } // mouse on title bar



      if (mouse_b1)
      {
         if (show_sel_frame) // draw new selection rectangle
         {
            // initial selection
            stx = gx;
            sty = gy;
            sux = gx+1;
            suy = gy+1;

            while (mouse_b1)
            {
               sux = gx+1;
               suy = gy+1;

               ge_draw_on_level_buffer(gx, gy, stx, sty, sux, suy, show_sel_frame, mouse_on_window);
               get_new_screen_buffer(3, 0, 0);
               ovw_process_scrolledge();
               ovw_get_block_position_on_map(&gx, &gy, &hx, &hy);
            }
            if (sux < stx) // swap if wrong order
            {
               int temp = sux;
               sux = stx;
               stx = temp;
            }
            if (suy < sty)
            {
               int temp = suy;
               suy = sty;
               sty = temp;
            }
            if (stx - sux == 0) sux++;  // don't allow zero size
            if (sty - suy == 0) suy++;  // don't allow zero size

            if ((key[ALLEGRO_KEY_LSHIFT]) || (key[ALLEGRO_KEY_RSHIFT]))
               add_selection_to_list(stx, sty, sux, suy); // add everything in selection to list...

         } // end of get new selection
         else
         {

            while (mouse_b1) proc_controllers();

            // is mouse on item
            for (int i=0; i<500; i++)
            {
               int type = item[i][0];
               if ((type) && (obj_filter[2][type])) // filter for this type of item
               {
                  int sox = item[i][4]/20;
                  int soy = item[i][5]/20;
                  if ((gx == sox) && (gy == soy)) add_to_obj_list(2, i);
               }
            }

            // is mouse on enemy
            for (int e=0; e<100; e++)
            {
               int type = Ei[e][0];
               if ((type) && (obj_filter[3][type])) // filter for this type of enemy
               {
                  int sox = al_fixtoi(Efi[e][0]/20);
                  int soy = al_fixtoi(Efi[e][1]/20);
                  if ((gx == sox) && (gy == soy)) add_to_obj_list(3, e);
               }
            }
         }
      }
      while ((mouse_b2) || (key[ALLEGRO_KEY_ESCAPE]))
      {
         proc_controllers();
         exit = 1;
      }
   }
}
