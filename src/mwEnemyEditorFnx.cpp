// mwEnemyEditorFnx.cpp

#include "pm.h"
#include "mwEnemy.h"
#include "mwDisplay.h"
#include "mwFont.h"
#include "mwColor.h"
#include "mwMiscFnx.h"
#include "mwInput.h"
#include "mwBitmap.h"



void mwEnemy::fill_strings(void)
{
   strcpy (enemy_name[0][0],  "empty");
   strcpy (enemy_name[1][0],  "undef");
   strcpy (enemy_name[2][0],  "undef");
   strcpy (enemy_name[3][0],  "ArchWagon");
   strcpy (enemy_name[4][0],  "Bouncer");
   strcpy (enemy_name[5][0],  "JumpWorm");
   strcpy (enemy_name[6][0],  "Cannon");
   strcpy (enemy_name[7][0],  "PodZilla");
   strcpy (enemy_name[8][0],  "TrakBot");
   strcpy (enemy_name[9][0],  "Cloner");
   strcpy (enemy_name[10][0], "undef");
   strcpy (enemy_name[11][0], "Block Walker");
   strcpy (enemy_name[12][0], "Flapper");
   strcpy (enemy_name[13][0], "VinePod");

   strcpy (enemy_name[0][1],  "mpty");
   strcpy (enemy_name[1][1],  "undf");
   strcpy (enemy_name[2][1],  "undf");
   strcpy (enemy_name[3][1],  "arch");
   strcpy (enemy_name[4][1],  "boun");
   strcpy (enemy_name[5][1],  "jump");
   strcpy (enemy_name[6][1],  "cann");
   strcpy (enemy_name[7][1],  "podz");
   strcpy (enemy_name[8][1],  "trak");
   strcpy (enemy_name[9][1],  "clon");
   strcpy (enemy_name[10][1], "undf");
   strcpy (enemy_name[11][1], "blkw");
   strcpy (enemy_name[12][1], "flap");
   strcpy (enemy_name[13][1], "vine");
   strcpy (enemy_name[66][1], "dth2");
   strcpy (enemy_name[99][1], "dth1");



   strcpy (enemy_name[0][2],  "empty");
   strcpy (enemy_name[1][2],  "undef");
   strcpy (enemy_name[2][2],  "undef");
   strcpy (enemy_name[3][2],  "archwagon");
   strcpy (enemy_name[4][2],  "bouncer");
   strcpy (enemy_name[5][2],  "jumpworm");
   strcpy (enemy_name[6][2],  "cannon");
   strcpy (enemy_name[7][2],  "podzilla");
   strcpy (enemy_name[8][2],  "trakbot");
   strcpy (enemy_name[9][2],  "cloner");
   strcpy (enemy_name[10][2], "undef");
   strcpy (enemy_name[11][2], "block walker");
   strcpy (enemy_name[12][2], "flapper");
   strcpy (enemy_name[13][2], "vinepod");

   enemy_tile[3]  = 496;
   enemy_tile[4]  = 508;
   enemy_tile[5]  = 706;
   enemy_tile[6]  = 415;
   enemy_tile[7]  = 374;
   enemy_tile[8]  = 384;
   enemy_tile[9]  = 550;
   enemy_tile[10] = 550;
   enemy_tile[11] = 866;
   enemy_tile[12] = 159;
   enemy_tile[13] = 374;
}


void mwEnemy::erase_enemy(int e)
{
   for (int a=0; a<32; a++) Ei[e][a] = 0;
   for (int a=0; a<16; a++) Ef[e][a] = 0;

}


int mwEnemy::show_enemy_data(int x_pos, int y_pos)
{
   sort_enemy();
   al_draw_textf(mFont.pr8, mColor.pc[4], x_pos, y_pos, 0, "%-2d Enemies", num_enemy); y_pos += 8;
   al_draw_text(mFont.pr8, mColor.pc[4], x_pos, y_pos,  0, "----------"); y_pos += 8;
   for (int c=1; c<16; c++)
   {
      if (e_num_of_type[c]) // not zero
      {
         al_draw_textf(mFont.pr8, mColor.pc[10], x_pos, y_pos,   0, "%-2d %s", e_num_of_type[c], enemy_name[c][0]);
         y_pos += 8;
      }
   }
   return y_pos;
}



// fix this here..............returns even if full!!!!
int mwEnemy::get_empty_enemy(void)
{
   int en = -1;
   for (int d=0; d<100; d++)
      if (Ei[d][0] == 0)
      {
         en = d;
         break;
      }
   if (en == -1) al_show_native_message_box(mDisplay.display, "Error", "Error creating enemy", "Enemy list full!", NULL, ALLEGRO_MESSAGEBOX_ERROR);
   else erase_enemy(en);
   return en;
}

int mwEnemy::get_empty_enemy(int type)
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

void mwEnemy::recalc_pod(int e)
{
   float xlen = Ef[e][5] - Ef[e][0];           // get the x distance
   float ylen = Ef[e][6] - Ef[e][1];           // get the y distance
   float hy_dist = sqrt(pow(xlen, 2) + pow(ylen, 2));    // hypotenuse distance
   float speed = Ef[e][9];                      // speed
   float scaler = hy_dist / speed;     // get scaler
   Ef[e][2] = xlen / scaler;         // calc xinc
   Ef[e][3] = ylen / scaler;         // calc yinc
   Ef[e][14] = atan2(ylen, xlen) - ALLEGRO_PI/2;  // rotation
   Ei[e][7] = scaler; // num steps
}

void mwEnemy::get_pod_extended_position(int e, int *x, int *y)
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

void mwEnemy::show_all_enemies(void)
{
   char msg[1024];
   ALLEGRO_BITMAP *tmp;
   tmp = al_create_bitmap(20, 20);

   int text_pos = 0;
   al_set_target_backbuffer(mDisplay.display);
   al_clear_to_color(al_map_rgb(0,0,0));

   int rh = 16; // row height

   sort_enemy();
   text_pos = show_enemy_data(10, text_pos);
   for (int e=0; e<num_enemy; e++)
   {
      al_set_target_bitmap(tmp);
      al_clear_to_color(al_map_rgb(0,0,0));
      draw_enemy(e, 1, 0, 0);
      al_set_target_backbuffer(mDisplay.display);
      al_draw_scaled_bitmap(tmp, 0, 0, 20, 20, 0, text_pos, rh, rh, 0);

      al_draw_line(0, text_pos, mDisplay.SCREEN_W, text_pos, mColor.pc[15+128], 0);

      int tp1 = text_pos+(rh-16)/2;
      int tp2 = tp1+8;
      int tpc = (tp1+tp2)/2;

      al_draw_textf(mFont.pr8, mColor.pc[14], rh+2, tpc, 0, "[%2d]",e);
      strcpy(msg,"");
      for (int j=0; j<32; j++)
      {
         char msg2[80];
         sprintf(msg2,"[%d] ", Ei[e][j]);
         strcat(msg, msg2);
      }
      al_draw_text(mFont.pr8, mColor.pc[13], 54, tp1, 0, msg);
      strcpy(msg,"");
      for (int j=0; j<16; j++)
      {
         char msg2[80];
         sprintf(msg2,"[%3.2f] ", Ef[e][j]);
         strcat(msg, msg2);
      }
      al_draw_text(mFont.pr8, mColor.pc[9], 54, tp2, 0, msg);
      text_pos +=rh;
      if (text_pos > mDisplay.SCREEN_H - 10)
      {
         al_flip_display();
         mInput.tsw(); // wait for keypress
         al_clear_to_color(al_map_rgb(0,0,0));
         text_pos = 0;
      }
   }
   al_flip_display();
   mInput.tsw(); // wait for keypress
   al_destroy_bitmap(tmp);
}


void mwEnemy::sort_enemy(void)
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

int mwEnemy::create_cloner(void)
{
   int aborted_create = 0;
   int e = get_empty_enemy(9); // type 9 cloner

   Ef[e][12] = 1;  // scale
   Ef[e][14] = 0;  // rotation

   Ei[e][1] = mBitmap.zz[7][105]; // static shape
   Ei[e][5] = 1;                  // draw mode 1 - static shape

   Ei[e][4] = 0; // draw box mode - none

   Ei[e][25] = 25;  // health bonus
   Ei[e][29] = 10;  // default collision box


   if (mMiscFnx.getxy("Cloner Location", 3, 9, e) == 1)
   {
      if (mMiscFnx.get_block_range("Cloner Source Area", &Ei[e][15], &Ei[e][16], &Ei[e][19], &Ei[e][20], 1))
      {
         if (!mMiscFnx.getxy("Cloner Destination Area", 98, 9, e)) aborted_create = 1;
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

int mwEnemy::create_pod(void)
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

   if (mMiscFnx.getxy("Podzilla", 3, 7, e) == 1)
   {
      if (mMiscFnx.getxy("Pod Extended Position", 99, 7, e) == 1)
      {
         if (!mMiscFnx.get_block_range("Trigger Box", &Ei[e][11], &Ei[e][12], &Ei[e][13], &Ei[e][14], 2)) aborted_create = 1;
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

int mwEnemy::create_vinepod(void)
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

   if (mMiscFnx.getxy("VinePod Initial Position", 3, 17, e) == 1)
   {
      // enforce that 34 is 01
      Ei[e][3] = Ef[e][0];
      Ei[e][4] = Ef[e][1];

      // temporarily set control points to the same as initial position
      Ei[e][5] = Ei[e][3];
      Ei[e][6] = Ei[e][4];
      Ei[e][7] = Ei[e][3];
      Ei[e][8] = Ei[e][4];

      if (mMiscFnx.getxy("VinePod Extended Position", 90, 17, e) == 1)
      {
         if (!mMiscFnx.get_block_range("Trigger Box", &Ei[e][11], &Ei[e][12], &Ei[e][13], &Ei[e][14], 2)) aborted_create = 1;

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


