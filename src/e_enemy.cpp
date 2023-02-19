// e_enemy.cpp

#include "pm.h"
#include "mwDisplay.h"
#include "mwFont.h"
#include "z_lift.h"
#include "mwColor.h"
#include "z_menu.h"
#include "z_item.h"
#include "z_enemy.h"
#include "z_level.h"
#include "e_fnx.h"
#include "z_fnx.h"

void erase_enemy(int e)
{
   for (int a=0; a<32; a++) Ei[e][a] = 0;
   for (int a=0; a<16; a++) Ef[e][a] = 0;

}


// fix this here..............returns even if full!!!!
int get_empty_enemy(void)
{
   int en = -1;
   for (int d=0; d<100; d++)
      if (Ei[d][0] == 0)
      {
         en = d;
         break;
      }
   if (en == -1) al_show_native_message_box(display, "Error", "Error creating enemy", "Enemy list full!", NULL, ALLEGRO_MESSAGEBOX_ERROR);
   else erase_enemy(en);
   return en;
}

int get_empty_enemy(int type)
{
   int en = get_empty_enemy();
//   printf("get enemy:%d\n", en);
//   show_all_enemies();

   if (en == -1) return 500;
   else
   {
      Ei[en][0] = type;
      sort_enemy();
      en = e_first_num[type]+e_num_of_type[type]-1;

//      printf("post sort:%d\n", en);
//      show_all_enemies();
       return en;
   }
}

void recalc_pod(int e)
{
   float xlen = Ef[e][5] - Ef[e][0];           // get the x distance
   float ylen = Ef[e][6] - Ef[e][1];           // get the y distance
   float hy_dist = sqrt(pow(xlen, 2) + pow(ylen, 2));    // hypotenuse distance
   float speed = Ef[e][9];                      // speed
   float scaler = al_fixdiv(hy_dist, speed);     // get scaler
   Ef[e][2] = xlen / scaler;         // calc xinc
   Ef[e][3] = ylen / scaler;         // calc yinc
   Ef[e][14] = atan2(ylen, xlen) - ALLEGRO_PI/2;  // rotation
   Ei[e][7] = scaler; // num steps
}


void get_pod_extended_position(int e, int *x, int *y)
{
   float ex = Ef[e][0];
   float ey = Ef[e][1];
   for (int j=0; j<Ei[e][7]; j++)
   {
      ex += Ef[e][2];
      ey += Ef[e][3];
   }
   *x = ex;
   *y = ey;
}


void show_level_data(void)
{
   al_set_target_backbuffer(display);
   al_clear_to_color(al_map_rgb(0,0,0));
   int y_pos = 40;
   int x_pos = 40;
   if (valid_level_loaded)
   {
      y_pos = enemy_data(x_pos, y_pos) + 8;
      y_pos = item_data(x_pos, y_pos) + 8;
      al_draw_textf(mF.pr8, mC.pc[15], x_pos, y_pos,   0, "%d Lifts  ", get_num_lifts());
      al_draw_text(mF.pr8, mC.pc[15], x_pos, y_pos+8,  0, "-------");
   }
   al_flip_display();
   tsw(); // wait for keypress
}



void show_all_enemies(void)
{
   ALLEGRO_BITMAP *tmp;
   tmp = al_create_bitmap(20, 20);

   int text_pos = 0;
   al_set_target_backbuffer(display);
   al_clear_to_color(al_map_rgb(0,0,0));

   int rh = 16; // row height

   sort_enemy();
   text_pos = enemy_data(10, text_pos);
   for (int e=0; e<num_enemy; e++)
   {
      al_set_target_bitmap(tmp);
      al_clear_to_color(al_map_rgb(0,0,0));
      draw_enemy(e, 1, 0, 0);
      al_set_target_backbuffer(display);
      al_draw_scaled_bitmap(tmp, 0, 0, 20, 20, 0, text_pos, rh, rh, 0);


      al_draw_line(0, text_pos, mwD.SCREEN_W, text_pos, mC.pc[15+128], 0);


      int tp1 = text_pos+(rh-16)/2;
      int tp2 = tp1+8;
      int tpc = (tp1+tp2)/2;

      al_draw_textf(mF.pr8, mC.pc[14], rh+2, tpc, 0, "[%2d]",e);
      strcpy(msg,"");
      for (int j=0; j<32; j++)
      {
         char msg2[80];
         sprintf(msg2,"[%d] ", Ei[e][j]);
         strcat(msg, msg2);
      }
      al_draw_text(mF.pr8, mC.pc[13], 54, tp1, 0, msg);
      strcpy(msg,"");
      for (int j=0; j<16; j++)
      {
         char msg2[80];
         sprintf(msg2,"[%3.2f] ", Ef[e][j]);
         strcat(msg, msg2);
      }
      al_draw_text(mF.pr8, mC.pc[9], 54, tp2, 0, msg);
      text_pos +=rh;
      if (text_pos > mwD.SCREEN_H - 10)
      {
         al_flip_display();
         tsw(); // wait for keypress
         al_clear_to_color(al_map_rgb(0,0,0));
         text_pos = 0;
      }
   }
   al_flip_display();
   tsw(); // wait for keypress
   al_destroy_bitmap(tmp);
}


void sort_enemy(void)
{
   int swap_flag = 1;
   while (swap_flag)
   {
      swap_flag = 0;
      for (int e=0; e<99; e++)
         if (Ei[e][0] < Ei[e+1][0])
         {
            swap_flag = 1;
            for (int x=0; x<32; x++)
            {
               int temp = Ei[e][x];
               Ei[e][x] = Ei[e+1][x];
               Ei[e+1][x] = temp;
            }
            for (int x=0; x<16; x++)
            {
               float ftemp = Ef[e][x];
               Ef[e][x] = Ef[e+1][x];
               Ef[e+1][x] = ftemp;
            }
         }
   }

   // get data about first 50 enem types and make sub lists of enem types using these variables
   for (int x=0; x<50; x++) // zero the type counters
   {
      e_num_of_type[x] = 0;
      e_first_num[x] = 0;
   }
   num_enemy = 0; // zero total enemy count
   for (int e=0; e<100; e++)  // get type counts
   {
      e_num_of_type[Ei[e][0]]++;   // inc number of this type
      if (Ei[e][0]) num_enemy++;   // inc total num
   }

   for (int x=0; x<50; x++)  // get first nums
      if (e_num_of_type[x] > 0)  // are there any of this type?
         for (int y=0; y<100; y++)
            if (Ei[y][0] == x)
            {
               e_first_num[x] = y;
               y=100;   // exit loop
            }
//   // show counts and first nums
//   for (int x=0; x<50; x++)  // get first nums1
//      if (e_num_of_type[x] > 0)  // are there any of this type?
//          printf("%2d %2d %2d\n", x, e_num_of_type[x], e_first_num[x]);

}

int create_cloner(void)
{
   int aborted_create = 0;
   int e = get_empty_enemy(9); // type 9 cloner

   Ef[e][12] = 1;  // scale
   Ef[e][14] = 0;  // rotation

   Ei[e][1] = 550;   // shape
   Ei[e][2] = 0;     // draw type

   Ei[e][4] = 3;     // draw all boxes
   Ei[e][5] = 0;     // mode
   Ei[e][8] = 2;     // trigger type

   Ei[e][6] = 400;  // default delay
   Ei[e][7] = 0;  // default delay

   Ei[e][25] = 25;  // health bonus
   Ei[e][29] = 10;  // default collision box


   if (getxy("Cloner Location", 3, 9, e) == 1)
   {

      if (get_block_range("Cloner Source Area", &Ei[e][15], &Ei[e][16], &Ei[e][19], &Ei[e][20], 1))
      {
         if (getxy("Cloner Destination Area", 98, 9, e ) == 1)
         {
            if (!get_block_range("Trigger Box", &Ei[e][11], &Ei[e][12], &Ei[e][13], &Ei[e][14], 2)) aborted_create = 1;
         }
         else aborted_create = 1;
      }  // end of get source area
      else aborted_create = 1;
   }  // end of set cloner location
   else aborted_create = 1;

   if (aborted_create)
   {
      Ei[e][0] = 0;
      sort_enemy();
      return -1;
   }
   else return e;
}

int create_pod(void)
{
   int aborted_create = 0;
   int e = get_empty_enemy(7); // type 7 - pod

   Ei[e][1] = 374;   // shape
   Ei[e][2] = 0;     // draw type
   Ef[e][12] = 1;  // scale
   Ef[e][14] = 0;  // rotation
   Ef[e][7] = 6;     // shot speed
   Ef[e][9] = 10;    // default speed
   Ei[e][7] = 20;    // default seq delay
   Ei[e][9] = 20;    // default delay
   Ei[e][25] = 12;  // health bonus
   Ei[e][29] = 10;  // default collision box
   Ef[e][4] = 2;   // damage

   if (getxy("Podzilla", 3, 7, e) == 1)
   {
      if (getxy("Pod Extended Position", 99, 7, e) == 1)
      {
         if (!get_block_range("Trigger Box", &Ei[e][11], &Ei[e][12], &Ei[e][13], &Ei[e][14], 2)) aborted_create = 1;
      }
      else aborted_create = 1;
   }
   else aborted_create = 1;

   if (aborted_create)
   {
      Ei[e][0] = 0;
      sort_enemy();
      return -1;
   }
   else return e;
}

int create_vinepod(void)
{
   int aborted_create = 0;
   int e = get_empty_enemy(13);

   Ei[e][1] = 374;             // shape
   Ei[e][2] = 0;               // draw type
   Ef[e][12] = 1;  // scale
   Ef[e][14] = 0;  // rotation
   Ef[e][7] = 6;   // shot speed
   Ei[e][17] = 120;            // extend time
   Ei[e][19] = 20;             // pause
   Ei[e][25] = 13;             // health bonus
   Ei[e][29] = 10;             // default collision box
   Ef[e][4] = 2;   // damage

   if (getxy("VinePod Initial Position", 3, 17, e) == 1)
   {
      // enforce that 34 is 01
      Ei[e][3] = Ef[e][0];
      Ei[e][4] = Ef[e][1];

      // temporarily set control points to the same as initial position
      Ei[e][5] = Ei[e][3];
      Ei[e][6] = Ei[e][4];
      Ei[e][7] = Ei[e][3];
      Ei[e][8] = Ei[e][4];

      if (getxy("VinePod Extended Position", 90, 17, e) == 1)
      {
         if (!get_block_range("Trigger Box", &Ei[e][11], &Ei[e][12], &Ei[e][13], &Ei[e][14], 2)) aborted_create = 1;

         // set control points away from initial position to make them easier to adjust l
         Ei[e][5] = Ei[e][3]+20;
         Ei[e][6] = Ei[e][4]-20;
         Ei[e][7] = Ei[e][3]-20;
         Ei[e][8] = Ei[e][4]-20;

      }
      else aborted_create = 1;
   }
   else aborted_create = 1;

   if (aborted_create)
   {
      Ei[e][0] = 0;
      sort_enemy();
      return -1;
   }
   else return e;
}
