// e_nev.cpp

#include "pm.h"


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
   else
   {   // zero the enemy before use
      for (int i=0; i<32; i++) Ei[en][i] = 0;
      for (int i=0; i<16; i++) Efi[en][i] = al_itofix(0);
   }
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

int move_trigger_box(int num, int type)
{
   if (getbox("Trigger Box", 3, type, num) == 1)
      {
          Ei[num][11] = bx1;
          Ei[num][12] = by1;
          Ei[num][13] = bx2-1;
          Ei[num][14] = by2-1;
          return 1;
      }
   else return 0;
}

void recalc_pod(int EN)
{
   al_fixed xlen = Efi[EN][5] - Efi[EN][0];      // get the x distance
   al_fixed ylen = Efi[EN][6] - Efi[EN][1];      // get the y distance
   al_fixed hy_dist =  al_fixhypot(xlen, ylen);     // hypotenuse distance
   al_fixed speed = Efi[EN][9];                  // speed
   al_fixed scaler = al_fixdiv(hy_dist, speed);     // get scaler
   al_fixed xinc = al_fixdiv(xlen, scaler);         // calc xinc
   al_fixed yinc = al_fixdiv(ylen, scaler);         // calc yinc
   Efi[EN][2] = xinc;
   Efi[EN][3] = yinc;
   Efi[EN][14] = al_fixatan2(ylen, xlen) - al_itofix(64);  // rotation

   // set number of steps
   al_fixed ns;
   if (abs(xlen) > abs(ylen)) ns = al_fixdiv(xlen, xinc);
   else  ns = al_fixdiv(ylen, yinc);
   int num_steps = al_fixtoi(ns);
   if ((num_steps > 0) && (num_steps < 2000))
      Ei[EN][7] = num_steps;


}

int move_pod_extended(int num)
{
   if (getxy("Set Extended Position", 99, 7, num) == 1)
   {
      Efi[num][5] = al_itofix(get100_x * 20);  // set dest x,y
      Efi[num][6] = al_itofix(get100_y * 20);
      recalc_pod(num);
      Redraw = 1;
      return 1;
   }
   else
 return 0;
}

void show_all_enemies(void)
{
   int text_pos = 0;
   al_set_target_backbuffer(display);
   al_clear_to_color(al_map_rgb(0,0,0));

   sort_enemy();
   text_pos = enemy_data(10, text_pos);
   for (int e=0; e<num_enemy; e++)
   {
      draw_enemy_shape(e, 0, text_pos);
      al_draw_textf(font, palette_color[14], 20, text_pos+6, 0, "[%2d]",e);
      strcpy(msg,"");
      for (int j=0; j<32; j++)
      {
         char msg2[80];
         sprintf(msg2,"[%d] ", Ei[e][j]);
         strcat(msg, msg2);
      }
      al_draw_text(font, palette_color[13], 54, text_pos+2, 0, msg);
      strcpy(msg,"");
      for (int j=0; j<16; j++)
      {
         char msg2[80];
         sprintf(msg2,"[%3.2f] ", al_fixtof(Efi[e][j]));
         strcat(msg, msg2);
      }
      al_draw_text(font, palette_color[9], 54, text_pos+10, 0, msg);
      text_pos +=20;
      if (text_pos > SCREEN_H - 10)
      {
         al_flip_display();
         tsw(); // wait for keypress
         al_clear_to_color(al_map_rgb(0,0,0));
         text_pos = 0;
      }
   }
   al_flip_display();
   tsw(); // wait for keypress
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
               al_fixed ftemp = Efi[e][x];
               Efi[e][x] = Efi[e+1][x];
               Efi[e+1][x] = ftemp;
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
   if (getxy( "Set Cloner Location", 3, 9, e) == 1)
   {
      Efi[e][0] = al_itofix(get100_x * 20); // position
      Efi[e][1] = al_itofix(get100_y * 20);
      Efi[e][12] = al_itofix(1);  // scale
      Efi[e][14] = al_itofix(0);  // rotation


      Ei[e][1] = 550;   // shape
      Ei[e][2] = 0;     // draw type

      Ei[e][4] = 3;     // draw all boxes
      Ei[e][5] = 0;     // mode
      Ei[e][8] = 2;     // trigger type

      Ei[e][6] = 400;  // default delay
      Ei[e][7] = 400;  // default delay

      Ei[e][25] = 25;  // health bonus
      Ei[e][29] = 10;  // default collision box

      draw_big(1);
      show_big();

      if (getbox("Cloner Source Area", 3, 9, e ))
      {
         Ei[e][15] = bx1;
         Ei[e][16] = by1;
         Ei[e][19] = bx2-bx1;
         Ei[e][20] = by2-by1;

         draw_big(1);

         if (getxy("Set Cloner Destination Area", 98, 9, e ) == 1)
         {
            Ei[e][17] = get100_x;
            Ei[e][18] = get100_y;

            if (!move_trigger_box(e, 9)) aborted_create = 1;

            draw_big(1);
            show_big();
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



int create_field(void)
{
   int aborted_create = 0;
   int e = get_empty_enemy(10); // type 10 field
   if (getxy( "Set Field Location", 3, 10, e) == 1)
   {
      Efi[e][0] = al_itofix(get100_x * 20); // position
      Efi[e][1] = al_itofix(get100_y * 20);
      Efi[e][4] = al_itofix(2); // damage

      Ei[e][2] = 1;     // draw type (small text)
      Ei[e][5] = 0;     // mode
      Ei[e][6] = 100;   // timer
      Ei[e][8] = 40;    // timer flip val

      Ei[e][3] |= PM_ENEMY_FIELD_DAMAGE_PLAYER;  // set flag
      Ei[e][3] |= PM_ENEMY_FIELD_TRIGGER_PLAYER; // set flag
      Ei[e][3] |= PM_ENEMY_FIELD_DAMAGE_CURR;    // set flag

      draw_big(1);
      show_big();

      if (getbox("Set Damage Field Location", 3, 10, e ))
      {
         Ei[e][15] = bx1*20;
         Ei[e][16] = by1*20;
         Ei[e][17] = (bx2-bx1)*20;
         Ei[e][18] = (by2-by1)*20;
         draw_big(1);
         show_big();

         if (getbox("Set Trigger Field Location", 3, 10, e ))
         {
            Ei[e][11] = bx1*20;
            Ei[e][12] = by1*20;
            Ei[e][13] = (bx2-bx1)*20;
            Ei[e][14] = (by2-by1)*20;
            draw_big(1);
            show_big();
         } // end of set trigger field
         else aborted_create = 1;
      }  // end of set damage field
      else aborted_create = 1;;
   }  // end of set field location
   else aborted_create = 1;
   //sort_enemy(); this is done before we start...

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
   if (getxy("Set Podzilla Location", 3, 7, e) == 1)
   {
      Efi[e][0] = al_itofix(get100_x * 20); // position
      Efi[e][1] = al_itofix(get100_y * 20);

      Efi[e][7] = al_itofix(6);     // bullet speed
      Efi[e][9] = al_itofix(10);    // default speed
      Efi[e][12] = al_itofix(1);    // default scale

      Ei[e][1] = 374;   // shape
      Ei[e][2] = 0;     // draw type

      Ei[e][7] = 20;    // default seq delay
      Ei[e][9] = 20;    // default delay

      Ei[e][25] = 12;     // health bonus
      Ei[e][29] = 10;  // default collision box

      draw_big(1);
      show_big();

      if (!move_pod_extended(e)) aborted_create = 1;

      if (!move_trigger_box(e, 7)) aborted_create = 1;
   }

   if (aborted_create)
   {
      Ei[e][0] = 0;
      sort_enemy();
      return -1;
   }
   else return e;
}


