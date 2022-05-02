// e_object_viewer_windows.cpp

#include "pm.h"


int create_obj(int obt, int sub_type, int sent_num)
{
   int num = sent_num; // default
   int ret;

   if (obt == 2) // items
   {
      ret = create_item(sub_type);
      if (ret > 500)
      {
         num = sent_num;
         al_show_native_message_box(display,
               "Error", "No creator exists for the current item type",
               "Copy from an existing item of that type, or get one from the selection window",
                NULL, ALLEGRO_MESSAGEBOX_ERROR);
      }
      else num = ret;
   }
   if (obt == 3)
   {
      if (sub_type == 7)
      {
         int e = create_pod();
         if ((e>=0) && (e<99) && (Ei[e][0] == 7))
         {
            object_viewerw(3, e);
            num = e;
         }
      }
      if (sub_type == 9)
      {
         int e = create_cloner();
         if ((e>=0) && (e<99) && (Ei[e][0] == 9))
         {
            object_viewerw(3, e);
            num = e;
         }
      }
      if (sub_type == 10)
      {
         int e = create_field();
         if ((e>=0) && (e<99) && (Ei[e][0] == 10))
         {
            object_viewerw(3, e);
            num = e;
         }
      }
   }
   return num;  // return number of created obj or sent_num if bad create
}




int obj_buttons(int xa, int xb, int ty, int a, int bts, int obt, int num)
{
   int type=0;
   if (obt == 2) type = item[num][0];
   if (obt == 3) type = Ei[num][0];



 //  printf("xa:%d xb:%d ty:%d, a:%d bts:%d obt:%d type:%d num:%d\n",xa, xb, ty, a, bts, obt, type, num);


   int d = 96; // dim
   if (obt == 3)    // enemies
   {
      switch (type) // enemy subtypes
      {
         case 3:     // archwag
            mdw_slider2_fix(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 0,0,0,0,0, 12, 15, 15, 1,0,0,0, Efi[num][6], 9, 0.7, 0.01, "X-Speed:"); a++;
            mdw_slider2_fix(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 0,0,0,0,0, 12, 15, 15, 1,0,0,0, Efi[num][3], 9, 0.7, 0.01, "Y-Speed:"); a+=2;
            mdw_toggle(     xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 1000, 0,0,0,0,0,0,0,   1,0,0,0, Ei[num][2], "Initial Direction:Left ", "Initial Direction:Right", 13, 13, 15, 15); a++;
            mdw_toggle(     xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 1000, 0,0,0,0,0,0,0,   1,0,0,0, Ei[num][8], "Follow Mode",             "Bounce Mode",             13, 13, 15, 15); a+=2;
            mdw_slider0_int(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 0,0,0,0,0, 7, 15, 15,  1,0,0,0, Ei[num][11], 100, 0, 1,    "Jump Before Hole:", "Off"); a++;
            mdw_slider0_int(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 0,0,0,0,0, 7, 15, 15,  1,0,0,0, Ei[num][12], 100, 0, 1,    "Jump Before Wall:", "Off"); a++;
            mdw_slider0_int(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 0,0,0,0,0, 7, 15, 15,  1,0,0,0, Ei[num][7],  600, 0, 1,    "Jump Under Width:", "Off"); a++;
            mdw_slider0_int(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 0,0,0,0,0, 7, 15, 15,  1,0,0,0, Ei[num][6],  500, 0, 1,    "Jump Timer Wait:",  "Off"); a+=2;
            mdw_slider2_fix(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 0,0,0,0,0, 9, 15, 15,  1,0,0,0, Efi[num][7], 20, 0.8, 0.1, "Bullet Speed:"); a++;
            mdw_slider2_int(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 0,0,0,0,0, 9, 15, 15,  1,0,0,0, Ei[num][17], 2000, 20, 1,  "Bullet Proximity:"); a++;
            mdw_slider2_int(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 0,0,0,0,0, 9, 15, 15,  1,0,0,0, Ei[num][15], 200, 1, 1,    "Bullet Retrigger Time:"); a+=2;
            mdw_slider2_int(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 0,0,0,0,0, 4, 15, 15,  1,0,0,0, Ei[num][29], 20, 0, 1,     "Collision Box:"); a++;
            mdw_slider2_fix(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 0,0,0,0,0, 4, 15, 15,  1,0,0,0, Efi[num][4], 10, 0, 0.1,   "Health Decrement:"); a++;
            mdw_slider2_int(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 0,0,0,0,0, 4, 15, 15,  1,0,0,0, Ei[num][25], 50, 0, 1,     "Health Bonus:"); a++;
         break;
         case 4: // bouncer
            mdw_button(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 10, num, type, obt, 0, 12, 15,  0, 1,0,0,0); a++;  // initial direction
            mdw_slider(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 22, num, type, obt, 0, 12, 15, 15, 1,0,0,0); a++;  // speed
            mdw_slider2_int(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 0,0,0,0,0, 12, 15, 15,  1,0,0,0, Ei[num][8], 100, 0, 1,   "Seek Count:"); a+=2;
            mdw_button(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 13, num, type, obt, 0,  8,  9,  0, 1,0,0,0); a+=2; // main shape
            mdw_button(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 14, num, type, obt, 0,  8,  9,  0, 1,0,0,0); a+=2; // seek shape
            mdw_slider2_int(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 0,0,0,0,0, 4, 15, 15,  1,0,0,0, Ei[num][29], 20, 0, 1,    "Collision Box:"); a++;
            mdw_slider2_fix(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 0,0,0,0,0, 4, 15, 15,  1,0,0,0, Efi[num][4], 10, 0, 0.1,  "Health Decrement:"); a++;
            mdw_slider2_int(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 0,0,0,0,0, 4, 15, 15,  1,0,0,0, Ei[num][25], 50, 0, 1,    "Health Bonus:"); a++;
         break;
         case 6: // cannon
            mdw_button(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 10, num, type, obt, 0, 12, 15,  0, 1,0,0,0); a++;  // initial direction
            mdw_slider(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 22, num, type, obt, 0, 12, 15, 15, 1,0,0,0); a++;  // speed
            mdw_slider2_int(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 0,0,0,0,0, 12, 15, 15,  1,0,0,0, Ei[num][8], 100, 0, 1,   "Seek Count:"); a+=2;
            mdw_slider2_int(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 0,0,0,0,0, 8, 15, 15,  1,0,0,0, Ei[num][9], 40, 0, 1,     "Extra Hits To Kill:"); a+=2;
            mdw_slider2_fix(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 0,0,0,0,0, 9, 15, 15, 1,0,0,0, Efi[num][7], 20, 0.8, 0.1, "Bullet Speed:"); a++;
            mdw_slider2_int(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 0,0,0,0,0, 9, 15, 15, 1,0,0,0, Ei[num][15], 200, 1, 1,    "Bullet Retrigger Time:"); a+=2;
            mdw_slider2_int(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 0,0,0,0,0, 4, 15, 15, 1,0,0,0, Ei[num][29], 20, 0, 1,     "Collision Box:"); a++;
            mdw_slider2_fix(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 0,0,0,0,0, 4, 15, 15, 1,0,0,0, Efi[num][4], 10, 0, 0.1,   "Health Decrement:"); a++;
            mdw_slider2_int(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 0,0,0,0,0, 4, 15, 15, 1,0,0,0, Ei[num][25], 50, 0, 1,     "Health Bonus:"); a++;
         break;
         case 7: // podzilla
            mdw_slider(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 29, num, type, obt, 0, 12, 15, 15, 1,0,0,0); a++;  // speed
            mdw_slider(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 30, num, type, obt, 0, 12, 15, 15, 1,0,0,0); a+=2;  // pause
            mdw_slider2_fix(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 0,0,0,0,0,  9, 15, 15, 1,0,0,0, Efi[num][7], 20, 0.8, 0.1, "Bullet Speed:"); a++;
            mdw_button(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 15, num, type, obt, 0, 10, 10,  0, 1,0,0,0); a++;  // move extended position
            mdw_button(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 16, num, type, obt, 0, 14, 14,  0, 1,0,0,0); a+=2;  // set trigger box
            mdw_slider2_int(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 0,0,0,0,0, 4, 15, 15, 1,0,0,0, Ei[num][29], 20, 0, 1,     "Collision Box:"); a++;
            mdw_slider2_fix(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 0,0,0,0,0, 4, 15, 15, 1,0,0,0, Efi[num][4], 10, 0, 0.1,   "Health Decrement:"); a++;
            mdw_slider2_int(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 0,0,0,0,0, 4, 15, 15, 1,0,0,0, Ei[num][25], 50, 0, 1,     "Health Bonus:"); a++;
         break;
         case 8: // trakbot
            mdw_button(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 11, num, type, obt, 0, 12, 15,  0, 1,0,0,0); a++;  // initial direction
            mdw_slider2_fix(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 0,0,0,0,0, 12, 15, 15, 1,0,0,0, Efi[num][2], 10, .5, 0.1, "X-Speed:"); a++;
            mdw_slider2_fix(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 0,0,0,0,0, 12, 15, 15, 1,0,0,0, Efi[num][3], 10, .5, 0.1, "Y-Speed:"); a+=2;
            mdw_toggle(     xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 1000, 0,0,0,0,0,0,0,   1,0,0,0, Ei[num][7], "Drop Mode:Off", "Drop Mode:On ", 15, 15, 11, 11); a+=2;
            mdw_slider2_fix(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 0,0,0,0,0, 9, 15, 15,  1,0,0,0, Efi[num][7], 20, 0.8, 0.1, "Bullet Speed:"); a++;
            mdw_slider2_int(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 0,0,0,0,0, 9, 15, 15,  1,0,0,0, Ei[num][17], 2000, 20, 1,  "Bullet Proximity:"); a++;
            mdw_slider2_int(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 0,0,0,0,0, 9, 15, 15,  1,0,0,0, Ei[num][15], 200, 1, 1,    "Bullet Retrigger Time:"); a+=2;
            mdw_slider2_int(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 0,0,0,0,0, 4, 15, 15,  1,0,0,0, Ei[num][29], 20, 0, 1,     "Collision Box:"); a++;
            mdw_slider2_fix(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 0,0,0,0,0, 4, 15, 15,  1,0,0,0, Efi[num][4], 10, 0, 0.1,   "Health Decrement:"); a++;
            mdw_slider2_int(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 0,0,0,0,0, 4, 15, 15,  1,0,0,0, Ei[num][25], 50, 0, 1,     "Health Bonus:"); a++;
         break;
         case 9: // cloner
            mdw_toggle(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 1000, 0,0,0,0,0,0,0,   1,0,0,0, Ei[num][30], "Not Invincible", "Invincible", 15, 15, 12, 12); a++;
            mdw_button(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 27, num, type, obt, 0, 12, 15,  0, 1,0,0,0); a++; // trigger type
            mdw_slider2_int(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 0,0,0,0,0, 12, 15, 15,  1,0,0,0, Ei[num][6], 1000, 20, 1, "Delay Timer:"); a++;
            a++;
            mdw_slider0_int(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 0,0,0,0,0, 14, 15, 15,  1,0,0,0, Ei[num][9], 4800, 0, 1, "Created Time To Live:", "-"); a++;
            mdw_slider0_int(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 0,0,0,0,0, 14, 15, 15,  1,0,0,0, Ei[num][10], 600, 0, 1, "Max Created Objects:", "-"); a++;
            a++;
            mdw_button(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 17, num, type, obt, 0, 11, 11,  0, 1,0,0,0); a++; // get source
            mdw_button(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 18, num, type, obt, 0, 10, 10,  0, 1,0,0,0); a++; // get destination
            mdw_button(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 16, num, type, obt, 0, 14, 14,  0, 1,0,0,0); a++; // get trigger box
            mdw_button(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 81, num, type, obt, 0, 15, 15,  0, 1,0,0,0); a++; // show boxes
            a++;
            mdw_slider2_int(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 0,0,0,0,0, 4, 15, 15,  1,0,0,0, Ei[num][29], 20, 0, 1,     "Collision Box:"); a++;
            mdw_slider2_fix(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 0,0,0,0,0, 4, 15, 15,  1,0,0,0, Efi[num][4], 10, 0, 0.1,   "Health Decrement:"); a++;
            mdw_slider2_int(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 0,0,0,0,0, 4, 15, 15,  1,0,0,0, Ei[num][25], 50, 0, 1,     "Health Bonus:"); a++;
         break;
         case 11: // block walker
            mdw_slider2_fix(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 0,0,0,0,0, 12, 15, 15, 1,0,0,0, Efi[num][6], 9, 0.7, 0.01, "X-Speed:"); a++;
            mdw_slider2_fix(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 0,0,0,0,0, 12, 15, 15, 1,0,0,0, Efi[num][3], 9, 0.7, 0.01, "Y-Speed:"); a++;
            a++;
            mdw_toggle(     xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 1000, 0,0,0,0,0,0,0,   1,0,0,0, Ei[num][2], "Initial Direction:Left ", "Initial Direction:Right", 13, 13, 15, 15); a++;
            mdw_toggle(     xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 1000, 0,0,0,0,0,0,0,   1,0,0,0, Ei[num][8], "Follow Mode",             "Bounce Mode",             13, 13, 15, 15); a++;
            a++;
            mdw_slider0_int(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 0,0,0,0,0, 7, 15, 15,  1,0,0,0, Ei[num][11], 100, 0, 1,    "Jump Before Hole:", "Off"); a++;
            mdw_slider0_int(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 0,0,0,0,0, 7, 15, 15,  1,0,0,0, Ei[num][12], 100, 0, 1,    "Jump Before Wall:", "Off"); a++;
            mdw_slider0_int(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 0,0,0,0,0, 7, 15, 15,  1,0,0,0, Ei[num][7],  600, 0, 1,    "Jump Under Width:", "Off"); a++;
            mdw_slider0_int(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 0,0,0,0,0, 7, 15, 15,  1,0,0,0, Ei[num][6],  500, 0, 1,    "Jump Timer Wait:",  "Off"); a++;
            a++;
            mdw_slider2_int(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 0,0,0,0,0, 4, 15, 15,  1,0,0,0, Ei[num][29], 20, 0, 1,     "Collision Box:"); a++;
            mdw_slider2_fix(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 0,0,0,0,0, 4, 15, 15,  1,0,0,0, Efi[num][4], 10, 0, 0.1,   "Health Decrement:"); a++;
            mdw_slider2_int(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 0,0,0,0,0, 4, 15, 15,  1,0,0,0, Ei[num][25], 50, 0, 1,     "Health Bonus:"); a++;
         break;
         case 12: // flapper
            mdw_slider2_fix(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 0,0,0,0,0, 12, 15, 15, 1,0,0,0, Efi[num][5], 8, .5, 0.1,  "X-Speed:"); a++;
            mdw_slider2_fix(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 0,0,0,0,0, 12, 15, 15, 1,0,0,0, Efi[num][6], 2, 0.1, 0.1, "X-Accel:"); a++;
            mdw_toggle(     xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 1000, 0,0,0,0,0,0,0,   1,0,0,0, Ei[num][2], "Initial Direction:Left ", "Initial Direction:Right", 15, 15, 12, 12); a++;
            a++;

            mdw_slider2_fix(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 0,0,0,0,0, 13, 15, 15, 1,0,0,0, Efi[num][3], 5, 0, 0.01,  "Y-Speed:"); a++;
            mdw_slider2_fix(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 0,0,0,0,0, 13, 15, 15, 1,0,0,0, Efi[num][10], 8, .5, 0.1, "Flap Speed:"); a++;
            mdw_slider2_int(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 0,0,0,0,0, 13, 15, 15, 1,0,0,0, Ei[num][21], 400, 0, 10,  "Flap Height:"); a++;
            mdw_slider2_int(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 0,0,0,0,0, 13, 15, 15, 1,0,0,0, Ei[num][20], 1000, -1000, 10,  "Height Above Player:"); a++;
            a++;

            mdw_slider2_int(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 0,0,0,0,0, 14, 15, 15, 1,0,0,0, Ei[num][17], 500, 20, 1,  "Trigger Width:"); a++;
            mdw_slider2_int(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 0,0,0,0,0, 14, 15, 15, 1,0,0,0, Ei[num][18], 600, 1, 10,  "Trigger Height:"); a++;
            mdw_slider2_int(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 0,0,0,0,0, 14, 15, 15, 1,0,0,0, Ei[num][19], 600, 1, 11,  "Trigger Depth:"); a++;
            a++;

            mdw_slider2_fix(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 0,0,0,0,0,  9, 15, 15, 1,0,0,0, Efi[num][7], 20, 0.8, 0.1, "Bullet Speed:"); a++;
            mdw_slider2_int(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 0,0,0,0,0,  9, 15, 15, 1,0,0,0, Ei[num][15], 200, 1, 1,    "Bullet Retrigger Time:"); a+=2;
            mdw_slider2_int(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 0,0,0,0,0, 4, 15, 15,  1,0,0,0, Ei[num][29], 20, 0, 1,     "Collision Box:"); a++;
            mdw_slider2_fix(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 0,0,0,0,0, 4, 15, 15,  1,0,0,0, Efi[num][4], 10, 0, 0.1,   "Health Decrement:"); a++;
            mdw_slider2_int(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 0,0,0,0,0, 4, 15, 15,  1,0,0,0, Ei[num][25], 50, 0, 1,     "Health Bonus:"); a++;
         break;



         case 10: // field
         {
            int FLAGS = Ei[num][3];
            int MODE = Ei[num][5];

            mdw_button(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 98,  num, type, obt, 0, 15, 15,  0, 1,0,0,0); a+=2; // Change Tile

            mdw_button(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 85,  num, type, obt, 0, 10, 10,  0, 1,0,0,0); a++;  // Get New Damage Field
            mdw_button(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 102, num, type, obt, 0, 10, 15,  0, 1,0,0,0); a++;  // Damage Field draw type


            if (FLAGS & PM_ENEMY_FIELD_LIFT_DMG_ON)
            {
                   mdw_button(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 134, num, type, obt, 0,  13,   15,  0, 1,0,0,0); a++;  // Damage Field follows lift:ON
                   mdw_slider(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 84,  num, type, obt, 0,  13,   15, 15, 1,0,0,0); a++;  // Lift Number
                   mdw_button(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 136, num, type, obt, 0,  13,   15,  0, 1,0,0,0); a++;  // X Alignment
                   mdw_button(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 137, num, type, obt, 0,  13,   15,  0, 1,0,0,0); a++;  // Y Alignment



            }
            else { mdw_button(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 134, num, type, obt, 0, 10+d, 15+d, 0, 1,0,0,0); a++; } // Damage Field follows lift:OFF

            if (FLAGS & PM_ENEMY_FIELD_DAMAGE_PLAYER)
            {
                   mdw_button(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 120, num, type, obt, 0, 10,    15,  0, 1,0,0,0); a++;   // Damage Field Affects Player ON
                   mdw_slider(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 83,  num, type, obt, 0, 10,    15,  0, 1,0,0,0); a++;   // Player damage
            }
            else { mdw_button(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 120, num, type, obt, 0, 10+d, 15+d, 0, 1,0,0,0); a++; } // Damage Field Affects Player OFF


            if (FLAGS & PM_ENEMY_FIELD_DAMAGE_ENEMY) { mdw_button(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 121, num, type, obt, 0, 10,     15,  0,1,0,0,0); a++; }
            else                                      { mdw_button(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 121, num, type, obt, 0, 10+d, 15+d,  0,1,0,0,0); a++; }
            if (FLAGS & PM_ENEMY_FIELD_DAMAGE_ITEM)  { mdw_button(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 122, num, type, obt, 0, 10,     15,  0,1,0,0,0); a++; }
            else                                      { mdw_button(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 122, num, type, obt, 0, 10+d, 15+d,  0,1,0,0,0); a++; }
            if (FLAGS & PM_ENEMY_FIELD_DAMAGE_PBUL)  { mdw_button(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 123, num, type, obt, 0, 10,     15,  0,1,0,0,0); a++; }
            else                                      { mdw_button(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 123, num, type, obt, 0, 10+d, 15+d,  0,1,0,0,0); a++; }
            if (FLAGS & PM_ENEMY_FIELD_DAMAGE_EBUL)  { mdw_button(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 124, num, type, obt, 0, 10,     15,  0,1,0,0,0); a++; }
            else                                      { mdw_button(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 124, num, type, obt, 0, 10+d, 15+d,  0,1,0,0,0); a++; }


            a++; // leave a blank line


            if (MODE == 0) { mdw_button(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 87, num, type, obt, 0, 8, 15,  0, 1,0,0,0); a++; } // MODE (0) // always on


            if ((MODE == 1) || (MODE == 2) || (MODE == 3)) // Mode 1, 2 and 3
            {
               mdw_button(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 87,  num, type, obt, 0,  8, 15,  0, 1,0,0,0); a++;  // MODE


               if (MODE == 1) // toggle
               {
                  mdw_button(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 133, num, type, obt, 0, 11, 15,  0, 1,0,0,0); a++;  // Set Damage Field Initially ON or OFF
               }

               if (MODE == 2) // Damage Field Off Time
               {
                  mdw_slider(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 90,  num, type, obt, 0, 11, 15, 15, 1,0,0,0); a++;
               }

               if (MODE == 3) // Damage Field On Time
               {
                  mdw_slider(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 89,  num, type, obt, 0, 11, 15, 15, 1,0,0,0); a++;
               }

               a++; // leave a blank line


               mdw_button(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 86,  num, type, obt, 0, 14, 14,  0, 1,0,0,0); a++;   // Get New Trigger Field
               mdw_button(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 105, num, type, obt, 0, 14, 15,  0, 1,0,0,0); a++;   // Trigger Field draw type

               if (FLAGS & PM_ENEMY_FIELD_LIFT_TRG_ON) // Trigger Field follows lift:ON
               {
                      mdw_button(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 135, num, type, obt, 0, 13, 15,  0, 1,0,0,0); a++;  // Trigger Field follows lift:ON
                      mdw_slider(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 85,  num, type, obt, 0, 13, 15, 15, 1,0,0,0); a++;  // Lift Number
                      mdw_button(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 138, num, type, obt, 0, 13, 15,  0, 1,0,0,0); a++;  // X Alignment
                      mdw_button(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 139, num, type, obt, 0, 13, 15,  0, 1,0,0,0); a++;  // Y Alignment



               }
               else { mdw_button(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 135, num, type, obt, 0, 15, 15,  0, 1,0,0,0); a++; } // Trigger Field follows lift:OFF

               if (FLAGS & PM_ENEMY_FIELD_TRIGGER_PLAYER) { mdw_button(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 125, num, type, obt, 0, 14,     15,  0, 1,0,0,0); a++; }
               else                                       { mdw_button(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 125, num, type, obt, 0, 14+d, 15+d,  0, 1,0,0,0); a++; }
               if (FLAGS & PM_ENEMY_FIELD_TRIGGER_ENEMY)  { mdw_button(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 126, num, type, obt, 0, 14,     15,  0, 1,0,0,0); a++; }
               else                                       { mdw_button(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 126, num, type, obt, 0, 14+d, 15+d,  0, 1,0,0,0); a++; }
               if (FLAGS & PM_ENEMY_FIELD_TRIGGER_ITEM)   { mdw_button(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 127, num, type, obt, 0, 14,     15,  0, 1,0,0,0); a++; }
               else                                       { mdw_button(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 127, num, type, obt, 0, 14+d, 15+d,  0, 1,0,0,0); a++; }
               if (FLAGS & PM_ENEMY_FIELD_TRIGGER_PBUL)   { mdw_button(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 128, num, type, obt, 0, 14,     15,  0, 1,0,0,0); a++; }
               else                                       { mdw_button(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 128, num, type, obt, 0, 14+d, 15+d,  0, 1,0,0,0); a++; }
               if (FLAGS & PM_ENEMY_FIELD_TRIGGER_EBUL)   { mdw_button(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 129, num, type, obt, 0, 14,     15,  0, 1,0,0,0); a++; }
               else                                       { mdw_button(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 129, num, type, obt, 0, 14+d, 15+d,  0, 1,0,0,0); a++; }

            }

            if (MODE == 4) // Timed on and off
            {
               mdw_button(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 87, num, type, obt, 0,  8, 15,  0, 1,0,0,0); a+=2; // mode
               mdw_slider(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 86, num, type, obt, 0, 12, 15, 15, 1,0,0,0); a++;  // total timer
               mdw_slider(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 88, num, type, obt, 0, 12, 15, 15, 1,0,0,0); a++;  // damage time
               mdw_slider(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 87, num, type, obt, 0, 12, 15, 15, 1,0,0,0); a++;  // initial time
            }
         }
         break;
      }
   }
   if (obt == 2) // items
   {
      switch (type) // item subtypes
      {
         case 1: // door
          {
            int abc = 11; // regular button frame color
            int atc = 15; // regular button text color

            if (item[num][8] == 0) // if exit only change color of inactive buttons
            {
                abc = 15;
                atc = 127;
            }
            mdw_button(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 26, num, type, obt, 0,  15,  13, 14, 1,0,0,0); a++; // stat | fall | carry
            mdw_button(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 49, num, type, obt, 0, 12 ,  15,  0, 1,0,0,0); a++; // door type
            mdw_button(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 4,  num, type, obt, 0, abc, atc,  0, 1,0,0,0); a++; // set linked item
            mdw_button(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 50, num, type, obt, 0, abc, atc,  0, 1,0,0,0); a++; // enter mode (up | down)
            mdw_button(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 53, num, type, obt, 0, abc, atc,  0, 1,0,0,0); a++; // move type
            mdw_button(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 51, num, type, obt, 0, abc, atc,  0, 1,0,0,0); a++; // exit link show
            mdw_button(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 52, num, type, obt, 0,  13,  15,  0, 1,0,0,0); a++; // get new shape
            mdw_colsel(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 5,  num, type, obt, 0,   0,   0,  0, 0,0,0,0); a++; // change color
         }
         break;
         case 2: // bonus
            mdw_button(xa, ty+a*bts, xb, ty+(a+1)*bts-2, 26, num, type, obt, 0, 15, 15, 15, 1,0,0,0); a++; // stat | fall | carry
            a++;
            if (item[num][6] == 1)
            {
               mdw_buttonp(xa, ty+a*bts, xb, ty+(a+1)*bts-2, 101, num, type, obt, 0, 11, 15, 15, 1,0,0,0, item[num][6]); a++; // bonus type
               mdw_slider2_int(xa, ty+a*bts, xb, ty+(a+1)*bts-2, 0,0,0,0,  0, 11, 15, 15, 1,0,0,0, item[num][7], 100, 2, 1, "Health Bonus:"); a++;
               item[num][1] = 1035;
               item[num][2] = 1;
               item[num][3] = 1;
               if (item[num][7] == 10) item[num][1] = 1023;
               if (item[num][7] == 50) item[num][1] = 1027;
            }
            else if (item[num][6] == 2)
            {
               mdw_buttonp(xa, ty+a*bts, xb, ty+(a+1)*bts-2, 101, num, type, obt, 0, 10, 15, 15, 1,0,0,0, item[num][6]); a++; // bonus type
               a++;
               item[num][1] = 1038;
               item[num][2] = 1;
               item[num][3] = 1;
            }
            else if (item[num][6] == 3)
            {
               mdw_buttonp(xa, ty+a*bts, xb, ty+(a+1)*bts-2, 101, num, type, obt, 0, 8, 15, 15, 1,0,0,0, item[num][6]); a++; // bonus type
               a++;
               item[num][1] = 197;
               item[num][2] = 0;
               item[num][3] = 1;
            }
         break;
         case 3: // exit
            mdw_button(xa, ty+a*bts, xb, ty+(a+1)*bts-2, 26, num, type, obt, 0, 15, 15, 15, 1,0,0,0); a++; // stat | fall | carry
            mdw_slider2_int(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 0,0,0,0,0, 12, 15, 15,  1,0,0,0, item[num][8], 100, 0, 1,  "Enemy Count Lock:"); a++;
         break;
         case 4: // key
            mdw_button(xa, ty+a*bts, xb, ty+(a+1)*bts-2, 2,  num, type, obt, 0, 15, 15, 15, 1,0,0,0); a++; // stat | fall
            mdw_button(xa, ty+a*bts, xb, ty+(a+1)*bts-2, 5,  num, type, obt, 0, 10, 10,  0, 1,0,0,0); a++; // set new block range
            mdw_toggle(xa, ty+a*bts, xb, ty+(a+1)*bts-2, 1000, 0,0,0,0,0,0,0,1,0,0,0, item[num][12], "Remove All Blocks", "Remove Only Matching", 15, 15, 4, 4); a++; // range type
         break;
         case 5: // start
            mdw_button(xa, ty+a*bts, xb, ty+(a+1)*bts-2, 26, num, type, obt, 0, 15, 15, 15, 1,0,0,0); a++; // stat | fall | carry
            mdw_button(xa, ty+a*bts, xb, ty+(a+1)*bts-2, 78, num, type, obt, 0, 11, 15, 15, 1,0,0,0); a++; // start mode
            mdw_button(xa, ty+a*bts, xb, ty+(a+1)*bts-2, 79, num, type, obt, 0, 11, 15, 15, 1,0,0,0); a++; // start index
         break;
         case 7: // mine
            mdw_button(xa, ty+a*bts, xb, ty+(a+1)*bts-2, 26, num, type, obt, 0, 15, 15, 15, 1,0,0,0); a++; // stat | fall | carry
            mdw_slider(xa, ty+a*bts, xb, ty+(a+1)*bts-2, 11, num, type, obt, 0, 10, 15, 15, 1,0,0,0); a++; // mine damage
         break;
         case 8: // bomb
            mdw_button(xa, ty+a*bts, xb, ty+(a+1)*bts-2, 26, num, type, obt, 0, 15, 15, 15, 1,0,0,0); a++; // stat | fall | carry
            mdw_button(xa, ty+a*bts, xb, ty+(a+1)*bts-2, 76, num, type, obt, 0, 11, 15, 15, 1,0,0,0); a++; // sticky
            mdw_button(xa, ty+a*bts, xb, ty+(a+1)*bts-2, 77, num, type, obt, 0, 12, 15, 15, 1,0,0,0); a++; // fuse timer / remote detonator
            mdw_slider(xa, ty+a*bts, xb, ty+(a+1)*bts-2,  4, num, type, obt, 0, 14, 15, 15, 1,0,0,0); a++; // damage range
            mdw_slider(xa, ty+a*bts, xb, ty+(a+1)*bts-2,  5, num, type, obt, 0, 10, 15, 15, 1,0,0,0); a++; // fuse length
         break;
         case 9: // trigger
         {
            int FLAGS = item[num][3];

            mdw_button(xa, ty+a*bts, xb, ty+(a+1)*bts-2, 200,  num, type, obt, 0, 14, 14, 14, 1,0,0,0); a+=2; // Get New Trigger Field




            // draw trigger field on/off with optional color select if on
            mdw_togglf(xa, ty+a*bts, xb, ty+(a+1)*bts-2, 1000,0,0,0, 0,0,0,0,1,0,0,0, item[num][3], PM_ITEM_TRIGGER_DRAW_ON, "Draw Trigger Field:OFF           ","Draw Trigger Field:ON            ", 15+96, 15, 15+96, item[num][2]); a++; // Draw on/off
            if (FLAGS & PM_ITEM_TRIGGER_DRAW_ON)
            {
               mdw_colsel(xa, ty+a*bts, xb, ty+(a+1)*bts-2,  6,   num, type, obt, 0,  0,   0,  0, 0,0,0,0); a++; // color select
            }
            a++;

            mdw_togglf(xa, ty+a*bts, xb, ty+(a+1)*bts-2, 1000,0,0,0, 0,0,0,0,1,0,0,0, item[num][3], PM_ITEM_TRIGGER_PLAYER, "Triggered by Players:OFF         ","Triggered by Players:ON          ", 15+d, 15, 14+d, 14); a++;
            mdw_togglf(xa, ty+a*bts, xb, ty+(a+1)*bts-2, 1000,0,0,0, 0,0,0,0,1,0,0,0, item[num][3], PM_ITEM_TRIGGER_ENEMY,  "Triggered by Enemies:OFF         ","Triggered by Enemies:ON          ", 15+d, 15, 14+d, 14); a++;
            mdw_togglf(xa, ty+a*bts, xb, ty+(a+1)*bts-2, 1000,0,0,0, 0,0,0,0,1,0,0,0, item[num][3], PM_ITEM_TRIGGER_ITEM,   "Triggered by Items:OFF           ","Triggered by Items:ON            ", 15+d, 15, 14+d, 14); a++;
            mdw_togglf(xa, ty+a*bts, xb, ty+(a+1)*bts-2, 1000,0,0,0, 0,0,0,0,1,0,0,0, item[num][3], PM_ITEM_TRIGGER_PBUL,   "Triggered by Player's Bullets:OFF","Triggered by Player's Bullets:ON ", 15+d, 15, 14+d, 14); a++;
            mdw_togglf(xa, ty+a*bts, xb, ty+(a+1)*bts-2, 1000,0,0,0, 0,0,0,0,1,0,0,0, item[num][3], PM_ITEM_TRIGGER_EBUL,   "Triggered by Enemy's Bullets:OFF ","Triggered by Enemy's Bullets:ON  ", 15+d, 15, 14+d, 14); a++;
            a++;

            if (FLAGS & PM_ITEM_TRIGGER_LIFT_ON) // Trigger Field follows lift:ON
            {
               mdw_button(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 210, num, type, obt, 0, 13, 15,  0, 1,0,0,0); a++;  // Trigger Field follows lift:ON
               mdw_slider(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 91,  num, type, obt, 0, 13, 15, 15, 1,0,0,0); a++;  // Lift Number
               mdw_button(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 211, num, type, obt, 0, 13, 15,  0, 1,0,0,0); a++;  // X Alignment
               mdw_button(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 212, num, type, obt, 0, 13, 15,  0, 1,0,0,0); a++;  // Y Alignment
            }
            else { mdw_button(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 210, num, type, obt, 0, 15, 15,  0, 1,0,0,0); a++; } // Trigger Field follows lift:OFF

            a++;


            mdw_slider(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 93,  num, type, obt, 0, 13, 15, 15, 1,0,0,0); a++;  // trigger output CURR ON
            mdw_slider(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 94,  num, type, obt, 0, 13, 15, 15, 1,0,0,0); a++;  // trigger output CURR OFF
            mdw_slider(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 95,  num, type, obt, 0, 13, 15, 15, 1,0,0,0); a++;  // trigger output TGON
            mdw_slider(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 96,  num, type, obt, 0, 13, 15, 15, 1,0,0,0); a++;  // trigger output TGOF


         }
         break;


         case 16: // block manip

            mdw_button(xa, ty+a*bts, xb, ty+(a+1)*bts-2, 300,  num, type, obt, 0, 14, 14, 14, 1,0,0,0); a+=2; // Get New Block Range



            if (item[num][2])
            {
               int col = item[num][12];
               mdw_button(xa, ty+a*bts, xb, ty+(a+1)*bts-2, 304,  num, type, obt, 0, col, 15, 15, 1,0,0,0); a++; // Draw on/off
               mdw_colsel(xa, ty+a*bts, xb, ty+(a+1)*bts-2,  7,   num, type, obt, 0,  0,   0,  0, 0,0,0,0); a++; // color select

            }
            else
            {
               mdw_button(xa, ty+a*bts, xb, ty+(a+1)*bts-2, 304,  num, type, obt, 0, 15+96, 15+96, 15, 1,0,0,0); a++; // Draw on/off
            }
            a++;

            mdw_button(xa, ty+a*bts, xb, ty+(a+1)*bts-2, 301,  num, type, obt, 0, 14, 14, 14, 1,0,0,0); a+=2; // mode

            mdw_button(xa, ty+a*bts, xb, ty+(a+1)*bts-2, 310,  num, type, obt, 0, 14, 14, 14, 1,0,0,0); a++;  // block 1
            mdw_button(xa, ty+a*bts, xb, ty+(a+1)*bts-2, 311,  num, type, obt, 0, 14, 14, 14, 1,0,0,0); a+=2; // block 2

            mdw_slider(xa, ty+a*bts, xb, ty+(a+1)*bts-2, 92,   num, type, obt, 0, 13, 15, 15, 1,0,0,0); a+=2; // pm_event trigger

            a++;

            mdw_button(xa, ty+a*bts, xb, ty+(a+1)*bts-2, 320,  num, type, obt, 0, 13, 15, 15, 1,0,0,0); a+=2; // set trigger

         break;


         case 17: // block damage
         {
            int FLAGS = item[num][3];
            int MODE = item[num][11];

            mdw_button(xa, ty+a*bts, xb, ty+(a+1)*bts-2, 400,  num, type, obt, 0, 10, 10, 10, 1,0,0,0); a+=2; // Get New Damage Field

            mdw_button(xa, ty+a*bts, xb, ty+(a+1)*bts-2, 404,  num, type, obt, 0, 8, 15, 15, 1,0,0,0); a+=2; // damage draw mode


            if (MODE == 0) { mdw_button(xa, ty+a*bts, xb, ty+(a+1)*bts-2, 402,  num, type, obt, 0, 10, 15, 14, 1,0,0,0); a++; } //  MODE 0


            if (MODE == 1)
            {
               mdw_button(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 402, num, type, obt, 0, 8, 15,  0, 1,0,0,0); a++;  // MODE 1
               mdw_button(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 433, num, type, obt, 0, 8, 15,  0, 1,0,0,0); a++;  // Set Damage Field Initially ON or OFF
            }



            if ((MODE == 2) || (MODE == 3) || (MODE == 4)) // Mode 2, 3 and 4
            {
               mdw_button(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 402,  num, type, obt, 0,  11, 15,  0, 1,0,0,0); a++;  // MODE

               if (MODE == 2) { mdw_slider(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 104,  num, type, obt, 0, 11, 15, 15, 1,0,0,0); a++;} // Damage Field Off Time
               if (MODE == 3) { mdw_slider(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 103,  num, type, obt, 0, 11, 15, 15, 1,0,0,0); a++;} // Damage Field On Time
               if (MODE == 4) // Timed on and off
               {
                  mdw_slider(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 100, num, type, obt, 0, 11, 15, 15, 1,0,0,0); a++;  // total timer
                  mdw_slider(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 102, num, type, obt, 0, 11, 15, 15, 1,0,0,0); a++;  // damage time
                  mdw_slider(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 101, num, type, obt, 0, 11, 15, 15, 1,0,0,0); a++;  // initial time
               }
               mdw_button(   xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 401, num, type, obt, 0, 11, 15, 15, 1,0,0,0); a++; // timer draw mode
            }

            a++;

            if (FLAGS & PM_ITEM_DAMAGE_LIFT_ON) // DAMAGE Field follows lift:ON
            {
               mdw_button(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 410, num, type, obt, 0, 13, 15,  0, 1,0,0,0); a++;  // DAMAGE Field follows lift:ON
               mdw_slider(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 97,  num, type, obt, 0, 13, 15, 15, 1,0,0,0); a++;  // Lift Number
               mdw_button(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 411, num, type, obt, 0, 13, 15,  0, 1,0,0,0); a++;  // X Alignment
               mdw_button(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 412, num, type, obt, 0, 13, 15,  0, 1,0,0,0); a++;  // Y Alignment
            }
            else { mdw_button(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 410, num, type, obt, 0, 15, 15,  0, 1,0,0,0); a++; } // DAMAGE Field follows lift:OFF

            a++;

            if (FLAGS & PM_ITEM_DAMAGE_PLAYER)
            {
               mdw_button(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 420, num, type, obt, 0, 10,    15,  0, 1,0,0,0); a++;   // Item Damage Affects Player ON

               if (item[num][3] & PM_ITEM_DAMAGE_INSTGIB)
               {
                  mdw_button(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 403, num, type, obt, 0, 10,    15,  0, 1,0,0,0); a++; // Instant death for player:ON
               }
               else
               {
                  mdw_button(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 403, num, type, obt, 0, 10+d,  15+d,0, 1,0,0,0); a++;   // Instant death for player:OFF
                  mdw_slider(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 98,  num, type, obt, 0, 10,    15,  0, 1,0,0,0); a++;   // Player damage
               }
            }
            else { mdw_button(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 420, num, type, obt, 0, 10+d, 15+d, 0, 1,0,0,0); a++; } // Item Damage Affects Player OFF

            if (FLAGS & PM_ITEM_DAMAGE_ENEMY) { mdw_button(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 421, num, type, obt, 0, 10,     15,  0,1,0,0,0); a++; }
            else                              { mdw_button(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 421, num, type, obt, 0, 10+d, 15+d,  0,1,0,0,0); a++; }
            if (FLAGS & PM_ITEM_DAMAGE_ITEM)  { mdw_button(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 422, num, type, obt, 0, 10,     15,  0,1,0,0,0); a++; }
            else                              { mdw_button(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 422, num, type, obt, 0, 10+d, 15+d,  0,1,0,0,0); a++; }
            if (FLAGS & PM_ITEM_DAMAGE_PBUL)  { mdw_button(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 423, num, type, obt, 0, 10,     15,  0,1,0,0,0); a++; }
            else                              { mdw_button(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 423, num, type, obt, 0, 10+d, 15+d,  0,1,0,0,0); a++; }
            if (FLAGS & PM_ITEM_DAMAGE_EBUL)  { mdw_button(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 424, num, type, obt, 0, 10,     15,  0,1,0,0,0); a++; }
            else                              { mdw_button(xa, ty+(a*bts), xb, ty+(a+1)*bts-2, 424, num, type, obt, 0, 10+d, 15+d,  0,1,0,0,0); a++; }
            a++;
            if ((MODE == 1) || (MODE == 2) || (MODE == 3)) // Mode 1, 2, and 3
            {
               mdw_slider(xa, ty+a*bts, xb, ty+(a+1)*bts-2, 92,   num, type, obt, 0, 13, 15, 15, 1,0,0,0); a++; // pm_event trigger
               a++;
               mdw_button(xa, ty+a*bts, xb, ty+(a+1)*bts-2, 320,  num, type, obt, 0, 13, 15, 15, 1,0,0,0); a+=2; // set trigger
            }
         }
         break;
         case 10: // message
            mdw_button(xa, ty+a*bts, xb, ty+(a+1)*bts-2, 26, num, type, obt, 0, 15, 15, 15, 1,0,0,0); a++; // stat | fall | carry
            mdw_button(xa, ty+a*bts, xb, ty+(a+1)*bts-2, 55, num, type, obt, 0, 14, 15, 14, 1,0,0,0); a++; // set msg position
            mdw_slider(xa, ty+a*bts, xb, ty+(a+1)*bts-2, 54, num, type, obt, 0, 11, 15, 15, 1,0,0,0); a++; // display time
            mdw_colsel(xa, ty+a*bts, xb, ty+(a+1)*bts-2,  2, num, type, obt, 0,  0,  0,  0, 0,0,0,0); a++; // frame color select
            mdw_colsel(xa, ty+a*bts, xb, ty+(a+1)*bts-2,  3, num, type, obt, 0,  0,  0,  0, 0,0,0,0); a++; // text color select
            mdw_button(xa, ty+a*bts, xb, ty+(a+1)*bts-2,  7, num, type, obt, 0, 15, 13, 14, 1,0,0,0); a++; // edit text

            a+=2; // leave space for OK and Cancel buttons

            // draw the current one last to ensure it is on top
            pop_msg_viewer_pos = ty+a*bts+bts/2+2;
            display_pop_message(num, pmsgtext[num], (xa+xb)/2, pop_msg_viewer_pos, 0, 0); // show the message


            a+=8;



         break;
         case 11: // rocket
            mdw_button(xa, ty+a*bts, xb, ty+(a+1)*bts-2, 82, num, type, obt, 0, 15, 15, 15, 1,0,0,0); a++; // stat | fall | ride through door
            mdw_button(xa, ty+a*bts, xb, ty+(a+1)*bts-2,  6, num, type, obt, 0, 15, 13,  0, 1,0,0,0); a++; // set initial direction
            mdw_slider(xa, ty+a*bts, xb, ty+(a+1)*bts-2,  4, num, type, obt, 0, 14, 15, 15, 1,0,0,0); a++; // damage range
            mdw_slider(xa, ty+a*bts, xb, ty+(a+1)*bts-2,  6, num, type, obt, 0, 12, 15, 15, 1,0,0,0); a++; // acceleration
            mdw_slider(xa, ty+a*bts, xb, ty+(a+1)*bts-2,  7, num, type, obt, 0, 12, 15, 15, 1,0,0,0); a++; // max speed
            mdw_slider(xa, ty+a*bts, xb, ty+(a+1)*bts-2,  8, num, type, obt, 0, 12, 15, 15, 1,0,0,0); a++; // steerability
         break;
         case 14: // switch
            mdw_button(xa, ty+a*bts, xb, ty+(a+1)*bts-2, 26, num, type, obt, 0, 15, 15, 15, 1,0,0,0); a++; // stat | fall | carry
         break;
         case 15: // sproingy
            mdw_button(xa, ty+a*bts, xb, ty+(a+1)*bts-2, 26, num, type, obt, 0, 15, 15, 15, 1,0,0,0); a++; // stat | fall | carry
            mdw_slider(xa, ty+a*bts, xb, ty+(a+1)*bts-2, 10, num, type, obt, 0, 12, 15, 15, 1,0,0,0); a++; // jump height
         break;
         case 12: // warp
            mdw_button(xa, ty+a*bts, xb, ty+(a+1)*bts-2, 26, num, type, obt, 0, 15, 13, 0,  1,0,0,0); a++; // stat | fall | carry
            mdw_slider(xa, ty+a*bts, xb, ty+(a+1)*bts-2, 28, num, type, obt, 0, 12, 15, 15, 1,0,0,0); a++; // warp level
         break;

      } // end of switch type
   } // end of if obt = item
   return a;
}





void ovw_draw_overlays(int obj_type, int num, int legend_highlight, int highlight_color)
{
   al_set_target_bitmap(level_buffer);
   int obj_x=0, obj_y=0, sub_type=0;
   if (obj_type == 2)
   {
      sub_type = item[num][0];
      obj_x = item[num][4]+10;
      obj_y = item[num][5]+10;
   }
   if (obj_type == 3)
   {
      sub_type = Ei[num][0];
      obj_x = al_fixtoi(Efi[num][0])+10;
      obj_y = al_fixtoi(Efi[num][1])+10;
   }

   // draw object location crosshairs
   int color = 13;
   if (legend_highlight == 1) color = highlight_color;
   crosshairs_full(obj_x, obj_y, color, 1);

   if (obj_type == 3)  // enemies
   {
      switch (sub_type)
      {
         case 3: // archwagon
         {
            // yellow bullet prox
            int color = 14;
            if (legend_highlight == 2) color = highlight_color;
            int bs = Ei[num][17];
            al_draw_rectangle(obj_x-bs, obj_y-bs, obj_x+bs, obj_y+bs, palette_color[color], 1);
         }
         break;
         case 7: // podzilla
         {
            // extended position
            int color1 = 10;
            if (legend_highlight == 2) color1 = highlight_color;


            int px=0, py=0;
            get_pod_extended_position(num, &px, &py);
            crosshairs_full(px+10, py+10, color1, 1);

            // draw tile at extended pos
            float rot = al_fixtof(al_fixmul(Efi[num][14], al_fixtorad_r));
            al_draw_scaled_rotated_bitmap(tile[Ei[num][1]], 10, 10, px+10, py+10, 1, 1, rot, ALLEGRO_FLIP_HORIZONTAL);

            // draw connecting line
            al_draw_line(obj_x, obj_y, px+10, py+10, palette_color[10], 1);


            // trigger box
            int color = 14;
            if (legend_highlight == 3) color = highlight_color;
            int tx1 = Ei[num][11]*20;
            int ty1 = Ei[num][12]*20;
            int tx2 = Ei[num][13]*20 + 20;
            int ty2 = Ei[num][14]*20 + 20;
            al_draw_rectangle(tx1, ty1, tx2, ty2, palette_color[color], 1);
         }
         break;
         case 8: // trakbot
         {
            // draw yellow bullet prox circle
            int color = 14;
            if (legend_highlight == 2) color = highlight_color;
            al_draw_circle(obj_x, obj_y, Ei[num][17], palette_color[color], 1);
         }
         break;
         case 9: // cloner
         {
            int color2 = 11;
            if (legend_highlight == 2) color2 = highlight_color;

            int color3 = 10;
            if (legend_highlight == 3) color3 = highlight_color;

            int color4 = 14;
            if (legend_highlight == 4) color4 = highlight_color;

            int cw = Ei[num][19]*20;     // width
            int ch = Ei[num][20]*20;     // height

            int cx1 = Ei[num][15]*20;    // source
            int cy1 = Ei[num][16]*20;
            int cx2 = cx1 + cw;
            int cy2 = cy1 + ch;

            //rectangle_with_diagonal_lines(cx1, cy1, cx2, cy2, db/3, color2, color2+64);
            al_draw_rectangle(cx1, cy1, cx2, cy2, palette_color[color2], 1);

            int cx3 = Ei[num][17]*20;    // destination
            int cy3 = Ei[num][18]*20;
            int cx4 = cx3 + cw;
            int cy4 = cy3 + ch;
            //rectangle_with_diagonal_lines(cx3, cy3, cx4, cy4, db/3, color3, color3+64);
            al_draw_rectangle(cx3, cy3, cx4, cy4, palette_color[color3], 1);

            // draw trigger box
            int tx1 = Ei[num][11]*20;
            int ty1 = Ei[num][12]*20;
            int tx2 = Ei[num][13]*20 + 20;
            int ty2 = Ei[num][14]*20 + 20;
            //rectangle_with_diagonal_lines(tx1, ty1, tx2, ty2, db/3, color4, color4+64);
            al_draw_rectangle(tx1, ty1, tx2, ty2, palette_color[color4], 1);

         }
         break;
         case 10: // field
         {

            int color2 = 10;
            if (legend_highlight == 2) color2 = highlight_color;

            int color3 = 14;
            if (legend_highlight == 3) color3 = highlight_color;

            int cw = Ei[num][17];     // width
            int ch = Ei[num][18];     // height
            int cx1 = Ei[num][15];    // source
            int cy1 = Ei[num][16];
            int cx2 = cx1 + cw;
            int cy2 = cy1 + ch;
            //rectangle_with_diagonal_lines(cx1, cy1, cx2, cy2, db/3, color2, color2+64);
            al_draw_rectangle(cx1, cy1, cx2, cy2, palette_color[color2], 1);

            // draw trigger box
            int tw = Ei[num][13];     // width
            int th = Ei[num][14];     // height
            int tx1 = Ei[num][11];
            int ty1 = Ei[num][12];
            int tx2 = tx1 + tw;
            int ty2 = ty1 + th;
            //rectangle_with_diagonal_lines(tx1, ty1, tx2, ty2, db/3, color4, color4+64);
            al_draw_rectangle(tx1, ty1, tx2, ty2, palette_color[color3], 1);
         }
         break;
         case 12: // flapper
         {
            int color2 = 14;
            if (legend_highlight == 2) color2 = highlight_color;

            int color3 = 10;
            if (legend_highlight == 3) color3 = highlight_color;

            // draw yellow bullet prox
            int bw  = Ei[num][17];
            int by1 = Ei[num][18];
            int by2 = Ei[num][19];
            al_draw_rectangle(obj_x-bw, obj_y-by1, obj_x+bw, obj_y+by2, palette_color[color2], 1);

            // draw red height above player line
            int hab = Ei[num][20];
            al_draw_line(obj_x-40, obj_y+hab, obj_x+40, obj_y+hab, palette_color[color3], 3);

            // draw flap height
            int fh = Ei[num][21];
            al_draw_line(obj_x-60, obj_y+fh, obj_x+60, obj_y+fh, palette_color[12], 1);
            al_draw_line(obj_x-60, obj_y-fh, obj_x+60, obj_y-fh, palette_color[12], 1);

         }
         break;
      }
      al_reset_clipping_rectangle();

   }
   if (obj_type == 2)  // items
   {
      switch (sub_type)
      {
         case 1: // door
         {
            int color = 10;
            if (legend_highlight == 2) color = highlight_color;
            if (item[num][8] == 0)  // exit only, no destination
            {
               // find src door(s)
               int num_src = 0;
               for (int i=0; i<500; i++)
                  if ((item[i][0] == 1) && (item[i][9] == num))
                  {
                     num_src++;
                     int x2 = item[i][4]+10;
                     int y2 = item[i][5]+10;
                     crosshairs_full(x2, y2, color, 1);
                     al_draw_line(obj_x, obj_y, x2, y2, palette_color[color], 1);
                  }
            }

            if (item[num][8] == 1)  // draw destination
            {
               // dest item
               int di = item[num][9];
               int x2 = item[di][4]+10;
               int y2 = item[di][5]+10;
               crosshairs_full(x2, y2, color, 1);
               al_draw_line(obj_x, obj_y, x2, y2, palette_color[color], 1);
            }
         }
         break;
         case 4: // key
         {
            int color = 10;
            if (legend_highlight == 2) color = highlight_color;
            int x2 = item[num][6];
            int y2 = item[num][7];
            int x3 = x2 + item[num][8] - 1;
            int y3 = y2 + item[num][9] - 1;;
            int x4 = (x2+x3)/2;
            int y4 = (y2+y3)/2;

            if (x2 == 0) x2 = 1; // to keep it visible
            if (y2 == 0) y2 = 1;

            // draw range
            al_draw_line(0, y4, 1999, y4, palette_color[color], 1);
            al_draw_line(x4, 0, x4, 1999, palette_color[color], 1);
            al_draw_rectangle(x2, y2, x3, y3, palette_color[color], 1);
         }
         break;
         case 8: // bomb
         {
            int color = 14;
            if (legend_highlight == 2) color = highlight_color;
            al_draw_circle(obj_x, obj_y, item[num][7], palette_color[color], 1); // draw yellow bomb damage
         }
         break;
         case 9: // trigger
         {
            int color = 14;
            if (legend_highlight == 2) color = highlight_color;

            int x2 = item[num][6];
            int y2 = item[num][7];
            int x3 = x2 + item[num][8] - 1;
            int y3 = y2 + item[num][9] - 1;;
            int x4 = (x2+x3)/2;
            int y4 = (y2+y3)/2;

            // draw range
            al_draw_line(0, y4, 1999, y4, palette_color[color], 1);
            al_draw_line(x4, 0, x4, 1999, palette_color[color], 1);
            al_draw_rectangle(x2, y2, x3, y3, palette_color[color], 1);

            find_and_show_event_links(1, num, 0);
         }
         break;
         case 11: // rocket
         {
            int color = 14;
            if (legend_highlight == 2) color = highlight_color;
            al_draw_circle(obj_x, obj_y, item[num][7], palette_color[color], 1); // draw yellow bomb damage
         }
         break;
         case 15: // sproingy
         {
            int color = 14;
            if (legend_highlight == 2) color = highlight_color;
            int y = al_fixtoi(get_sproingy_jump_height(num));
            crosshairs_full(obj_x, obj_y-y, color, 1);
         }
         break;
         case 16: // block manip
         {
            int color = 12;
            if (legend_highlight == 2) color = highlight_color;
            int x2 = item[num][6];
            int y2 = item[num][7];
            int x3 = x2 + item[num][8]-1;
            int y3 = y2 + item[num][9]-1;;
            int x4 = (x2+x3)/2;
            int y4 = (y2+y3)/2;

            // draw range
            al_draw_line(0, y4, 1999, y4, palette_color[color], 1);
            al_draw_line(x4, 0, x4, 1999, palette_color[color], 1);
            al_draw_rectangle(x2, y2, x3, y3, palette_color[color], 1);

            find_and_show_event_links(2, num, 0);

         }
         break;
         case 17: // block damage
         {
            int color = 10;
            if (legend_highlight == 2) color = highlight_color;
            int x2 = item[num][6];
            int y2 = item[num][7];
            int x3 = x2 + item[num][8]-1;
            int y3 = y2 + item[num][9]-1;;
            int x4 = (x2+x3)/2;
            int y4 = (y2+y3)/2;

            // draw range
            al_draw_line(0, y4, 1999, y4, palette_color[color], 1);
            al_draw_line(x4, 0, x4, 1999, palette_color[color], 1);
            al_draw_rectangle(x2, y2, x3, y3, palette_color[color], 1);

            find_and_show_event_links(2, num, 0);
         }
         break;

      } // end of switch case
   }
}



void ovw_title(int obj_type, int num, int legend_highlight, int highlight_color)
{
   int sub_type=0;
   if (obj_type == 2) sub_type = item[num][0];
   if (obj_type == 3) sub_type = Ei[num][0];

   int ov_x2 = ov_window_x + ov_window_w;
   int ov_xc = ov_window_x + ov_window_w/2;

   // legend line text
   char lmsg[5][80];
   for (int x=0; x<5; x++) sprintf(lmsg[x],"%s","");

   // legend line colors
   int legend_color[5];

   // default number of legend lines
   num_legend_lines = 2;

   legend_color[0] = 7;   // legend color
   legend_color[1] = 13;  // location color
   legend_color[2] = 14;  // yellow
   legend_color[3] = 10;  // red
   legend_color[4] = 0;   // unused


   if (legend_highlight == 1) legend_color[1] = highlight_color;

   if (!legend_highlight)
   {
      // title bar
      msg[0] = 0;
      if (obj_type == 2) sprintf(msg, "Item Viewer [%d]", num);
      if (obj_type == 3) sprintf(msg, "Enemy Viewer [%d]", num);
      if (obj_type == 4) sprintf(msg, "Lift Viewer");
      for (int x=0; x<15; x++)
         al_draw_line(ov_window_x, ov_window_y+x, ov_x2, ov_window_y+x, palette_color[13+(x*16)], 1);
      al_draw_text(font, palette_color[15], ov_xc, ov_window_y+2, ALLEGRO_ALIGN_CENTER,  msg);
   }



   if (obj_type == 3)  // enemies
   {
      if (!legend_highlight)
      {
         int yt = ov_window_y+14;
         al_draw_rectangle(ov_xc-94, yt, ov_xc+94, yt+22, palette_color[15], 1);
         draw_enemy_shape(num, ov_xc-92, yt+1);
         sprintf(msg,"%s %d of %d", (const char *)enemy_name[sub_type],1+num - e_first_num[sub_type],e_num_of_type[sub_type]);
         al_draw_text(font, palette_color[13], ov_xc-69, yt+8, 0, msg);
      }
      switch (sub_type)
      {
         case 3: // archwagon
         {
            num_legend_lines = 3;
            sprintf(lmsg[1],"ArchWagon Location");
            sprintf(lmsg[2],"Bullet Proximity");
            legend_color[2] = 14;
            if (legend_highlight == 2) legend_color[2] = highlight_color;
         }
         break;
         case 4: sprintf(lmsg[1],"Bouncer Location"); break;
         case 6: sprintf(lmsg[1],"Cannon Location"); break;
         case 7: // podzilla
         {
            sprintf(lmsg[1],"Podzilla Location");
            sprintf(lmsg[2],"Extended Postion");
            sprintf(lmsg[3],"Trigger Box");
            num_legend_lines = 4;

            legend_color[2] = 10;
            if (legend_highlight == 2) legend_color[2] = highlight_color;

            legend_color[3] = 14;
            if (legend_highlight == 3) legend_color[3] = highlight_color;
         }
         break;
         case 8: // trakbot
         {
            num_legend_lines = 3;
            sprintf(lmsg[1],"TrakBot Location");
            sprintf(lmsg[2],"Bullet Proximity");
            legend_color[2] = 14;
            if (legend_highlight == 2) legend_color[2] = highlight_color;
         }
         break;
         case 9: // cloner
         {
            sprintf(lmsg[1],"Cloner Location");
            sprintf(lmsg[2],"Source Area");
            sprintf(lmsg[3],"Destination Area");
            sprintf(lmsg[4],"Trigger Box");
            num_legend_lines = 5;

            legend_color[2] = 11;
            if (legend_highlight == 2) legend_color[2] = highlight_color;

            legend_color[3] = 10;
            if (legend_highlight == 3) legend_color[3] = highlight_color;

            legend_color[4] = 14;
            if (legend_highlight == 4) legend_color[4] = highlight_color;
         }
         break;
         case 10: // field
         {
            sprintf(lmsg[1],"Field Location");
            sprintf(lmsg[2],"Field Area");
            sprintf(lmsg[3],"Trigger Box");
            num_legend_lines = 4;

            legend_color[2] = 10;
            if (legend_highlight == 2) legend_color[2] = highlight_color;

            legend_color[3] = 14;
            if (legend_highlight == 3) legend_color[3] = highlight_color;
         }
         break;
         case 11: sprintf(lmsg[1],"Block Walker Location"); break;
         case 12: // flapper
         {
            sprintf(lmsg[1],"Flapper Location");
            sprintf(lmsg[2],"Bullet Trigger Box");
            sprintf(lmsg[3],"Height Above Player");
            num_legend_lines = 4;

            legend_color[2] = 14;
            if (legend_highlight == 2) legend_color[2] = highlight_color;

            legend_color[3] = 10;
            if (legend_highlight == 3) legend_color[3] = highlight_color;
         }
         break;
      }
   }
   if (obj_type == 2)  // items
   {
      if (!legend_highlight)
      {
         int yt = ov_window_y+14;
         al_draw_rectangle(ov_xc-94, yt, ov_xc+94, yt+22, palette_color[15], 1);
         draw_item_shape(num, ov_xc-94, yt+1);
         sprintf(msg,"%s %d of %d", item_name[sub_type], 1+num - item_first_num[sub_type],item_num_of_type[sub_type]);
         al_draw_text(font, palette_color[13], ov_xc-69, yt+8, 0, msg);
      }
      switch (sub_type)
      {
         case 1: // door
         {
            num_legend_lines = 3;
            legend_color[2] = 10;
            if (legend_highlight == 2) legend_color[2] = highlight_color;

            if (item[num][8] == 0)  // exit only, no destination
            {
               // find src door(s)
               int num_src = 0;
               for (int i=0; i<500; i++)
                  if ((item[i][0] == 1) && (item[i][9] == num))
                  {
                     num_src++;
                  }
               sprintf(lmsg[1],"Exit Door Location");
               if (num_src == 0) sprintf(lmsg[2],"No Source Door");
               if (num_src == 1) sprintf(lmsg[2],"Source Door Position");
               if (num_src >  1) sprintf(lmsg[2],"Source Door Positions");
            }

            if (item[num][8] == 1)  // draw destination
            {
               sprintf(lmsg[1],"Door Location");
               sprintf(lmsg[2],"Destination");
            }
         }
         break;
         case 2: sprintf(lmsg[1],"Bonus Location"); break;
         case 3: sprintf(lmsg[1],"Exit Location"); break;
         case 4: // key
         {
            num_legend_lines = 3;
            sprintf(lmsg[1],"Key Location");
            sprintf(lmsg[2],"Block Range");

            legend_color[2] = 10;
            if (legend_highlight == 2) legend_color[2] = highlight_color;
         }
         break;
         case 5: sprintf(lmsg[1],"Start Location"); break;
         case 7: sprintf(lmsg[1],"Mine Location"); break;
         case 8:
         {
            num_legend_lines = 3;
            sprintf(lmsg[1],"Bomb Location");
            sprintf(lmsg[2],"Damage Range");
            legend_color[2] = 14;
            if (legend_highlight == 2) legend_color[2] = highlight_color;
         }
         break;
         case 9: // trigger
         {
            num_legend_lines = 3;
            sprintf(lmsg[1],"Trigger Item Location");
            sprintf(lmsg[2],"Trigger Field");
            legend_color[2] = 10;
            if (legend_highlight == 2) legend_color[2] = highlight_color;
         }
         break;
         case 10:
         {
            sprintf(lmsg[1],"Message Location");
            sprintf(lmsg[2],"Display Position");
            num_legend_lines = 3;
         }
         break;
         case 11:
         {
            num_legend_lines = 3;
            sprintf(lmsg[1],"Rocket Location");
            sprintf(lmsg[2],"Damage Range");
            legend_color[2] = 14;
            if (legend_highlight == 2) legend_color[2] = highlight_color;
         }
         break;
         case 12: sprintf(lmsg[1],"Warp Location"); break;
         case 14: sprintf(lmsg[1],"Switch Location"); break;
         case 15: sprintf(lmsg[1],"Sproingy Location");
         {
            num_legend_lines = 3;
            legend_color[2] = 14;
            if (legend_highlight == 2) legend_color[2] = highlight_color;
         }
         break;
         case 16: // block manip
         {
            num_legend_lines = 3;
            sprintf(lmsg[1],"Block Manip Item Location");
            sprintf(lmsg[2],"Manip Field");
            legend_color[2] = 12;
            if (legend_highlight == 2) legend_color[2] = highlight_color;
         }
         break;
         case 17: // block damage
         {
            num_legend_lines = 3;
            sprintf(lmsg[1],"Item Location");
            sprintf(lmsg[2],"Damage Area");
            legend_color[2] = 10;
            if (legend_highlight == 2) legend_color[2] = highlight_color;
         }
         break;

      } // end of switch case

   }  // end of items

   if (!legend_highlight)
   {
      ov_window_h += num_legend_lines*8 + 8;
      int ov_y2 = ov_window_y + ov_window_h;

//      printf("w:%d h:%d\n", ov_window_w, ov_window_h);


      al_draw_text(font, palette_color[legend_color[0]], ov_xc, ov_y2-36+ (4-num_legend_lines)*8, ALLEGRO_ALIGN_CENTER, "Legend");
      al_draw_rectangle(ov_xc-100, ov_y2-38+ (4-num_legend_lines)*8, ov_xc+100, ov_y2-1, palette_color[13], 1); // big frame
      al_draw_rectangle(ov_xc-100, ov_y2-38+ (4-num_legend_lines)*8, ov_xc+100, ov_y2-28+ (4-num_legend_lines)*8, palette_color[13], 1); // top frame
      al_draw_rectangle(ov_window_x, ov_window_y, ov_x2, ov_y2, palette_color[13], 1);  // outline entire window

   }

   int ov_y2 = ov_window_y + ov_window_h;
   for (int x=1; x<num_legend_lines; x++)// draw text lines
      al_draw_text(font, palette_color[legend_color[x]], ov_xc, ov_y2-26+(3-num_legend_lines+x)*8, ALLEGRO_ALIGN_CENTER, lmsg[x]);

}







int ovw_get_size(int obt, int type, int*w, int*h)
{
   int ret = 0;

   if ((obt == 2) && (type == 1 )) { *w = 210; *h = 262; ret = 1;} // door
   if ((obt == 2) && (type == 2 )) { *w = 200; *h = 190; ret = 1;} // bonus
   if ((obt == 2) && (type == 3 )) { *w = 200; *h = 158; ret = 1;} // exit
   if ((obt == 2) && (type == 4 )) { *w = 200; *h = 182; ret = 1;} // key
   if ((obt == 2) && (type == 5 )) { *w = 200; *h = 174; ret = 1;} // start
   if ((obt == 2) && (type == 7 )) { *w = 200; *h = 158; ret = 1;} // mine
   if ((obt == 2) && (type == 8 )) { *w = 200; *h = 214; ret = 1;} // bomb
   if ((obt == 2) && (type == 9 )) { *w = 280; *h = 406; ret = 1;} // trigger
   if ((obt == 2) && (type == 10)) { *w = 220; *h = 390; ret = 1;} // message
   if ((obt == 2) && (type == 11)) { *w = 220; *h = 230; ret = 1;} // rocket
   if ((obt == 2) && (type == 12)) { *w = 220; *h = 214; ret = 1;} // warp
   if ((obt == 2) && (type == 14)) { *w = 200; *h = 142; ret = 1;} // switch
   if ((obt == 2) && (type == 15)) { *w = 240; *h = 166; ret = 1;} // sproingy
   if ((obt == 2) && (type == 16)) { *w = 280; *h = 358; ret = 1;} // bm
   if ((obt == 2) && (type == 17)) { *w = 290; *h = 358; ret = 1;} // bd


   if ((obt == 3) && (type == 3 )) { *w = 220; *h = 422; ret = 1;} // archwagon
   if ((obt == 3) && (type == 4 )) { *w = 220; *h = 302; ret = 1;} // bouncer
   if ((obt == 3) && (type == 6 )) { *w = 220; *h = 318; ret = 1;} // cannon
   if ((obt == 3) && (type == 7 )) { *w = 220; *h = 302; ret = 1;} // podzilla
   if ((obt == 3) && (type == 8 )) { *w = 220; *h = 342; ret = 1;} // trakbot
   if ((obt == 3) && (type == 9 )) { *w = 220; *h = 390; ret = 1;} // cloner
   if ((obt == 3) && (type == 11)) { *w = 220; *h = 350; ret = 1;} // block walker
   if ((obt == 3) && (type == 12)) { *w = 220; *h = 446; ret = 1;} // flapper

   if ((obt == 3) && (type == 10)) { *w = 310; *h = 350; ret = 1;} // field

   return ret;
}


























int ovw_draw_buttons(int num, int type, int obt)
{
      // erase background
      int w=0, h = 0;
      if (ovw_get_size(obt, type, &w, &h))
      {
         ov_window_w = w;
         int x2 = ov_window_x + w;
         int y2 = ov_window_y + h;
         al_draw_filled_rectangle(ov_window_x, ov_window_y, x2, y2, palette_color[0]);
         // printf("%d %d %d %d\n", ov_window_x, ov_window_y, x2, y2);
      }
      else ov_window_w = 300;

   // button x position
   int xa = ov_window_x+1;
   int xb = ov_window_x+ov_window_w-1;
   int ty = ov_window_y+38;


   // split into half
//   int x12 = xa + 1 * (xb-xa) / 2; // 1/2

   // split into thirds
   int x13 = xa + 1 * (xb-xa) / 3; // 1/3
   int x23 = xa + 2 * (xb-xa) / 3; // 2/3

   // split into quarters
   int x14 = xa + 1 * (xb-xa) / 4; // 1/4
   //int x25 = xa + 2 * (xb-xa) / 5; // 2/5
   //int x35 = xa + 3 * (xb-xa) / 5; // 3/5
   //int x45 = xa + 4 * (xb-xa) / 5; // 4/5




   // split into fifths
//   int x15 = xa + 1 * (xb-xa) / 5; // 1/5
   //int x25 = xa + 2 * (xb-xa) / 5; // 2/5
   //int x35 = xa + 3 * (xb-xa) / 5; // 3/5
   //int x45 = xa + 4 * (xb-xa) / 5; // 4/5

   // split into sevenths
   //int x17 = xa + 1 * (xb-xa) / 7; // 1/7
   int x27 = xa + 2 * (xb-xa) / 7; // 2/7
   //int x37 = xa + 3 * (xb-xa) / 7; // 3/7
   //int x47 = xa + 4 * (xb-xa) / 7; // 4/7
   int x57 = xa + 5 * (xb-xa) / 7; // 5/7
   //int x67 = xa + 6 * (xb-xa) / 7; // 6/7

   //mdw_slider(xb-40,  ty-33,    xb,    ty-27,          26, num, type, obt, 0, 15, 0,  10, 0,0,0,0); // button height
   //mdw_slider(xa,  ty-35,    xb,    ty-27,          26, num, type, obt, 0, 15, 0,  10, 0,0,0,0); // button height

   int lc = 6; // lock_color;
   if (Viewer_lock) lc = 7;

   int a=0;
   int mb = 0;   // button selection
   if (mdw_button(xa,  ty+a*bts, x27-1, ty+(a+1)*bts-2, 23, num, type, obt, 0,  9, 15, 0, 1,0,0,0)) mb = 22; // prev
       mdw_button(x27, ty+a*bts, x57-1, ty+(a+1)*bts-2, 56, num, type, obt, 0, lc, 15, 0, 1,0,0,0);          // lock
   if (mdw_button(x57, ty+a*bts, xb,    ty+(a+1)*bts-2, 22, num, type, obt, 0,  9, 15, 0, 1,0,0,0)) mb = 21; // next
   a++;
   if (mdw_button(xa,  ty+a*bts, x13-1, ty+(a+1)*bts-2, 19, num, type, obt, 0, 13, 15, 0, 1,0,0,0)) mb = 18; // move
   if (mdw_button(x13, ty+a*bts, x23-1, ty+(a+1)*bts-2, 20, num, type, obt, 0, 14, 15, 0, 1,0,0,0)) mb = 19; // create
   if (mdw_button(x23, ty+a*bts, xb,    ty+(a+1)*bts-2, 21, num, type, obt, 0, 10, 15, 0, 1,0,0,0)) mb = 20; // delete
   a++;
   if (mdw_button(xa,  ty+a*bts, x14-1, ty+(a+1)*bts-2, 25, num, type, obt, 0, 1,  15, 0, 1,0,0,0)) mb = 24; // viewer help
   if (mdw_button(x14, ty+a*bts, xb,    ty+(a+1)*bts-2, 57, num, type, obt, 0, 1,  15, 0, 1,0,0,0)) mb = 25; // specific object help
   a+=2;

   a = obj_buttons(xa, xb, ty, a, bts, obt, num);

   ov_window_h = 38+a*bts;





   return mb;

}



void ovw_process_scrolledge(void)
{
   int bw = BORDER_WIDTH;
   int scrolledge = 10;
   int scroll_amount = 20;

   if (mouse_x < scrolledge) WX-=scroll_amount;           // scroll left
   if (mouse_x > SCREEN_W-scrolledge) WX+=scroll_amount;  // scroll right
   if (mouse_y < scrolledge) WY-=scroll_amount;           // scroll up
   if (mouse_y > SCREEN_H-scrolledge) WY+=scroll_amount;  // scroll down

      // find the size of the source screen from actual screen size and scaler
   int SW = (int)( (float)(SCREEN_W - bw *2) / scale_factor_current);
   int SH = (int)( (float)(SCREEN_H - bw *2) / scale_factor_current);
   if (SW > 2000) SW = 2000;
   if (SH > 2000) SH = 2000;

   // correct for edges
   if (WX < 0) WX = 0;
   if (WY < 0) WY = 0;
   if (WX > (2000 - SW)) WX = 2000 - SW;
   if (WY > (2000 - SH)) WY = 2000 - SH;

   // used by get_new_background to only get what is needed
   level_display_region_x = WX;
   level_display_region_y = WY;
   level_display_region_w = SW;
   level_display_region_h = SH;

}



void ovw_get_block_position_on_map(int*x, int*y, int *hx, int *hy)
{
   // x, y in 0-99 scale
   // the mouse position past the border width is how far we are into the scaled map
   float mx1 = mouse_x-BORDER_WIDTH;
   float my1 = mouse_y-BORDER_WIDTH;

   // divide that by bs to get how many blocks we are into the map
   float mx2 = mx1 / (scale_factor_current * 20);
   float my2 = my1 / (scale_factor_current * 20);
   // get block position of WX
   float mx3 = (float)WX / 20;
   float my3 = (float)WY / 20;

   // add
   float mx4 = mx3 + mx2;
   float my4 = my3 + my2;

   *x = (int) mx4;
   *y = (int) my4;

   if (*x < 0)  *x = 0;
   if (*y < 0)  *y = 0;
   if (*x > 99) *x = 99;
   if (*y > 99) *y = 99;

   // hx, hy in 0-1999 scale
   // the mouse position past the border width is how far we are into the scaled map
   mx1 = mouse_x-BORDER_WIDTH;
   my1 = mouse_y-BORDER_WIDTH;

   // scale
   mx2 = mx1 / scale_factor_current;
   my2 = my1 / scale_factor_current;

   // get position of WX
   mx3 = (float)WX;
   my3 = (float)WY;

   // add
   mx4 = mx3 + mx2;
   my4 = my3 + my2;

   *hx = (int) mx4;
   *hy = (int) my4;

   if (*hx < 0)    *hx = 0;
   if (*hy < 0)    *hy = 0;
   if (*hx > 1999) *hx = 1999;
   if (*hy > 1999) *hy = 1999;
}


void object_viewerw(int obt, int num)
{
   init_level_background();
   bts = 16; // button y size
   int legend_line = -1;
   int highlight_counter=0;

//   int block_highlight_show = 0;
//   int block_highlight_x = 0;
//   int block_highlight_y = 0;
//   int block_highlight_color = 15+64;

   int mst = 6;      // mouse select threshold
   int msn = 10-mst; // mouse select negative threshold
   int msp = 10+mst; // mouse select positive threshold

   int mouse_on_obj = 0;
   int mouse_on_podx = 0;
   int mouse_on_podx_num = 0;
   int mouse_on_csb_ul = 0;
   int mouse_on_csb_lr = 0;
   int mouse_on_cdb_ul = 0;
   int mouse_on_trk = 0;

   int mouse_on_sp = 0;
   int mouse_on_bmb = 0;
   int mouse_on_msg_ul = 0;
   int mouse_on_kbr_ul = 0;
   int mouse_on_kbr_lr = 0;
   int mouse_on_tb_ul = 0;
   int mouse_on_tb_lr = 0;
   int mouse_move = 0;
   int mouse_adj = 0;

   int quit = 0;
   int ret = 0;
   int new_obj = 1;
   int type=0, obj_x=0, obj_y=0;

   int gx=0, gy=0, hx=0, hy=0;

   while (!quit)
   {
      int ov_window_x2 = ov_window_x + ov_window_w;
      int ov_window_y2 = ov_window_y + ov_window_h;





      if (obt == 4) // lift
      {
         ret = lift_viewer(num); // run lift viewer and stay there until quit or object changes
         if ((ret >= 2000) && (ret < 2500)) // obj changed to item
         {
            num = ret-2000;
            obt = 2;
         }
         if ((ret >= 1000) && (ret < 1100)) // obj changed to enemy
         {
            num = ret-1000;
            obt = 3;
         }
         if (ret == 0) quit = 1;
      }


      if (new_obj)
      {
         if (obt == 2)
         {
            type = item[num][0];
            obj_x = item[num][4]+10;
            obj_y = item[num][5]+10;
         }
         if (obt == 3)
         {
            type = Ei[num][0];
            obj_x = al_fixtoi(Efi[num][0])+10;
            obj_y = al_fixtoi(Efi[num][1])+10;
         }
         if (new_obj == 1) get_new_screen_buffer(1, obj_x, obj_y);
         new_obj = 0;
      }

      al_flip_display();
      proc_scale_factor_change();
      proc_controllers();
      proc_frame_delay();
      get_new_background(0);
      draw_lifts();
      draw_items();

    //  if (block_highlight_show) crosshairs_full(block_highlight_x, block_highlight_y, block_highlight_color, 1);

      // if current object is messages, show all messages
      if ((obt == 2) && (type == 10))
      {
         for (int i=0; i<500; i++)
            if (item[i][0] == 10) draw_pop_message(i);
      }

      draw_enemies();
      ovw_draw_overlays(obt, num, legend_line, highlight_counter);
      get_new_screen_buffer(3, obj_x, obj_y);

      //al_draw_textf(font, palette_color[15],100, 100, 0, "gx:%d   gy:%d", gx, gy);
      //al_draw_textf(font, palette_color[15],100, 110, 0, "hx:%d   hy:%d", hx, hy);

      int mb = ovw_draw_buttons(num, type, obt);

      // draw button title, frame and legend lines
      ovw_title(obt, num, 0, 15);

      // is mouse on title bar
      if ((mouse_x > ov_window_x) && (mouse_x < ov_window_x2) && (mouse_y > ov_window_y) && (mouse_y < ov_window_y+14))
      {
         // draw rectangle around title bar to show it can be dragged
         // al_draw_rectangle(ov_window_x, ov_window_y, ov_window_x2, ov_window_y+14, palette_color[14], 1);

         // color text in title bar to show it can be moved
//         msg[0] = 0;
//         if (obt == 2) sprintf(msg, "Item Viewer [%d]", num);
//         if (obt == 3) sprintf(msg, "Enemy Viewer [%d]", num);
//         int ov_xc = (ov_window_x + ov_window_x2) / 2;
//         al_draw_text(font, palette_color[10], ov_xc, ov_window_y+2, ALLEGRO_ALIGN_CENTER,  msg);



         if (mouse_b1)
         {
            int mxo = mouse_x - ov_window_x; // get offset from mouse position to window x, y
            int myo = mouse_y - ov_window_y;

            while (mouse_b1)
            {
               ov_window_x = mouse_x - mxo;
               ov_window_y = mouse_y - myo;
               ov_window_x2 = ov_window_x + ov_window_w;
               ov_window_y2 = ov_window_y + ov_window_h;

               al_flip_display();
               proc_scale_factor_change();

               proc_controllers();
               proc_frame_delay();
               get_new_background(0);
               draw_lifts();
               draw_items();
               draw_enemies();
               ovw_draw_overlays(obt, num, legend_line, highlight_counter);
               get_new_screen_buffer(3, obj_x, obj_y);

               ovw_draw_buttons(num, type, obt);
               ovw_title(obt, num, 0, 15);

            } // mouse b1 held
         } // mouse b1 pressed
      } // mouse on title bar


      // is mouse on legend ?
      legend_line = 0;
      int y1_legend = ov_window_y2 - 34 + (5-num_legend_lines)*8; // legend pos
      int y2_legend = y1_legend + (num_legend_lines-1)*8;
      if ((mouse_x > ov_window_x) && (mouse_x < ov_window_x2) && (mouse_y > y1_legend) && (mouse_y < y2_legend))
      {
         legend_line = ((mouse_y - y1_legend) / 8) + 1; // which legend line are we on?
         if (++highlight_counter > 16) highlight_counter = 1;
         if ((highlight_counter > 0)  && (highlight_counter < 5))  ovw_title(obt, num, legend_line, 10);
         if ((highlight_counter > 4)  && (highlight_counter < 9))  ovw_title(obt, num, legend_line, 14);
         if ((highlight_counter > 8)  && (highlight_counter < 13)) ovw_title(obt, num, legend_line, 15);
         if ((highlight_counter > 12) && (highlight_counter < 17)) ovw_title(obt, num, legend_line, 11);
      }
      else highlight_counter = 0; // mouse not on legend


      // if mouse is on button window
      if ((mouse_x > ov_window_x) && (mouse_x < ov_window_x2) && (mouse_y > ov_window_y) && (mouse_y < ov_window_y2))
      {
         //block_highlight_show = 0;
         // al_draw_rectangle(ov_window_x, ov_window_y, ov_window_x2, ov_window_y2, palette_color[14], 1);
      }
      else // if mouse NOT on button window
      {
         ovw_process_scrolledge();

         //ovw_map_move();

         ovw_get_block_position_on_map(&gx, &gy, &hx, &hy);

//         block_highlight_show = 1;
//         block_highlight_x = gx*20+10;
//         block_highlight_y = gy*20+10;

         mouse_on_obj = 0;

         mouse_on_podx = 0;
         mouse_on_podx_num = 0;
         mouse_on_csb_ul = 0;
         mouse_on_csb_lr = 0;
         mouse_on_cdb_ul = 0;

         mouse_on_sp = 0;
         mouse_on_bmb = 0;

         mouse_on_trk = 0;


         mouse_on_msg_ul = 0;
         mouse_on_kbr_ul = 0;
         mouse_on_kbr_lr = 0;
         mouse_on_tb_ul = 0;
         mouse_on_tb_lr = 0;
         mouse_move = 0;
         mouse_adj = 0;

         // detect if we are on an object...
         for (int b=0; b<100; b++)
            if (Ei[b][0]) // iterate valid enemies
            {
               int ex = al_fixtoi(Efi[b][0]);
               int ey = al_fixtoi(Efi[b][1]);
               if ((hx>ex+msn) && (hx<ex+msp) && (hy>ey+msn) && (hy<ey+msp)) // set this enemy to current object
               {
                  obt = 3;
                  num = b;
                  new_obj = 2;
                  mouse_move = 1;
                  mouse_on_obj = 1;
               }

               if (Ei[b][0] == 8) // trakbot bullet prox
               {
                  float x0 = al_fixtof(Efi[b][0])+10; // get center of item location
                  float y0 = al_fixtof(Efi[b][1])+10;
                  float fx = (float) hx;
                  float fy = (float) hy;
                  float dst = sqrt(pow((x0-fx), 2) + pow((y0-fy), 2)); // distance from mouse
                  float bdr = (float) Ei[b][17]; // prox radius
                  float dif = dst-bdr; // difference
                  if ((dif < 3 ) && (dif > -3))  // mouse is on prox circle
                  {
                     mouse_on_trk = 1;
                     mouse_move = 1;
                  }
               }

               if (Ei[b][0] == 7) // podzilla extended position
               {
                  int px=0, py=0;
                  get_pod_extended_position(b, &px, &py);
                  if ((hx>px+msn) && (hx<px+msp) && (hy>py+msn) && (hy<py+msp))
                  {
                     mouse_move = 1;
                     mouse_on_podx = 1;
                     mouse_on_podx_num = b;
                  }
               }
               if ((Ei[b][0] == 7) || (Ei[b][0] == 9)) // podzilla and cloner trigger box
               {
                  int x1 = Ei[b][11]*20;
                  int y1 = Ei[b][12]*20;
                  int x2 = Ei[b][13]*20+20;
                  int y2 = Ei[b][14]*20+20;
                  if ((hx>x1-mst) && (hx<x1+mst) && (hy>y1-mst) && (hy<y1+mst)) // upper left corner (move)
                  {
                     mouse_on_tb_ul = 1;
                     mouse_move = 1;
                  }
                  if ((hx>x2-mst) && (hx<x2+mst) && (hy>y2-mst) && (hy<y2+mst)) // lower right corner (resize)
                  {
                     mouse_on_tb_lr = 1;
                     mouse_adj = 1;
                  }
               }
               if (Ei[b][0] == 9) // cloner source and destination boxes
               {
                  int w = Ei[num][19]*20;     // width
                  int h = Ei[num][20]*20;     // height
                  int x1 = Ei[num][15]*20;    // source box
                  int y1 = Ei[num][16]*20;
                  int x2 = x1 + w - 1;
                  int y2 = y1 + h - 1;
                  int x3 = Ei[num][17]*20;    // dest box
                  int y3 = Ei[num][18]*20;
                  if ((hx>x1-mst) && (hx<x1+mst) && (hy>y1-mst) && (hy<y1+mst)) // source upper left corner (move)
                  {
                     mouse_on_csb_ul = 1;
                     mouse_move = 1;
                  }
                  if ((hx>x2-mst) && (hx<x2+mst) && (hy>y2-mst) && (hy<y2+mst)) // source lower right corner (re-size)
                  {
                     mouse_on_csb_lr = 1;
                     mouse_adj = 1;
                  }
                  if ((hx>x3-mst) && (hx<x3+mst) && (hy>y3-mst) && (hy<y3+mst)) // destination upper left corner (move)
                  {
                     mouse_on_cdb_ul = 1;
                     mouse_move = 1;
                  }
               }
            }
         for (int b=0; b<500; b++)
            if (item[b][0]) // iterate valid items
            {
               int ix = item[b][4];
               int iy = item[b][5];
               if ((hx>ix+msn) && (hx<ix+msp) && (hy>iy+msn) && (hy<iy+msp)) // set this item to current object
               {
                  obt = 2;
                  num = b;
                  new_obj = 2;
                  mouse_move = 1;
                  mouse_on_obj = 1;
               }
               if (item[b][0] == 10) // pop message
               {
                  int x1 = item[num][10]*20;
                  int y1 = item[num][11]*20;
                  if ((hx>x1-mst) && (hx<x1+mst) && (hy>y1-mst) && (hy<y1+mst)) // upper left corner (move)
                  {
                     mouse_on_msg_ul = 1;
                     mouse_move = 1;
                  }
               }
               if ((item[b][0] == 8) || (item[b][0] == 11)) // bomb or rocket
               {
                  float x0 = (float) item[num][4]+10; // get center of item location
                  float y0 = (float) item[num][5]+10;
                  float fx = (float) hx;
                  float fy = (float) hy;
                  float dst = sqrt(pow((x0-fx), 2) + pow((y0-fy), 2)); // distance from mouse to item
                  float bdr = (float) item[num][7]; // bomb damage radius
                  float dif = dst-bdr; // difference
                  if ((dif < 3 ) && (dif > -3))  // mouse is on blast radius circle
                  {
                     mouse_on_bmb = 1;
                     mouse_move = 1;
                  }
               }
               if (item[b][0] == 15) // sproingy
               {
                  int x1 = item[num][4];
                  int y1 = item[num][5];
                  int y2 = y1 - al_fixtoi(get_sproingy_jump_height(num));

                  if ((hx>x1+msn) && (hx<x1+msp) && (hy>y2+msn) && (hy<y2+msp))
                  {
                     mouse_on_sp = 1;
                     mouse_move = 1;
                  }
               }
               if ((item[b][0] == 4) || (item[b][0] == 9) || (item[b][0] == 16) || (item[b][0]== 17)) // key, trigger, manip, damage
               {
                  int x1 = item[num][6];
                  int y1 = item[num][7];
                  int x2 = x1 + item[num][8];
                  int y2 = y1 + item[num][9];

                  if ((hx>x1-mst) && (hx<x1+mst) && (hy>y1-mst) && (hy<y1+mst)) // upper left corner (move)
                  {
                     mouse_on_kbr_ul = 1;
                     mouse_move = 1;
                  }
                  if ((hx>x2-mst) && (hx<x2+mst) && (hy>y2-mst) && (hy<y2+mst)) // lower right corner (resize)
                  {
                     mouse_on_kbr_lr = 1;
                     mouse_adj = 1;
                  }
               }
            }




             // is mouse on any lift?
             for (int x=0; x<num_lifts; x++)  // cycle lifts
                for (int y=0; y<lifts[x].num_steps; y++)  // cycle steps
                   if ((lift_steps[x][y].type & 31) == 1) // look for move step
                    {
                        int w =  lift_steps[x][y].w / 2;
                        int h =  lift_steps[x][y].h / 2;
                        int nx = lift_steps[x][y].x + w;
                        int ny = lift_steps[x][y].y + h;
                        if ((hx > nx - w)  && (hx < nx + w) && (hy > ny - h)  && (hy < ny + h)) // is mouse on this step ?
                           if ((!key[MAP_LOCK_KEY]) && (!Viewer_lock))
                           {
                              obt = 4;  // lift
                              type = x; // lift #
                              num = y;  //step #
                           }
                     }







         if (mouse_move) al_set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_MOVE);
         else if (mouse_adj) al_set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_RESIZE_SE);
         else al_set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);

         if (mouse_b1)
         {
            while (mouse_b1)
            {
               if (mouse_on_obj)
               {
                  //printf("mouse pressed on obj\n");
                  if (obt == 2) // move item
                  {
                     // get offset of move
                     int x_off = gx - item[num][4] / 20;
                     int y_off = gy - item[num][5] / 20;

                     item[num][4] = gx*20;
                     item[num][5] = gy*20;
                     itemf[num][0] = al_itofix(gx*20);
                     itemf[num][1] = al_itofix(gy*20);

                     if ((key[ALLEGRO_KEY_LSHIFT]) || (key[ALLEGRO_KEY_RSHIFT])) // move stuff also
                     {
                         if ((item[num][0] == 4) || (item[num][0] == 9) || (item[num][0] == 16) || (item[num][0]== 17)) // key, trigger, manip, damage
                         {
                            item[num][6] += x_off*20;
                            item[num][7] += y_off*20;
                         }
                         if (item[num][0] == 10) // msg
                         {
                            item[num][10] += x_off;
                            item[num][11] += y_off;
                         }
                     }
                  }
                  if (obt == 3) // move enemy
                  {
                     // get offset of move
                     int x_off = gx - al_fixtoi(Efi[num][0]) / 20;
                     int y_off = gy - al_fixtoi(Efi[num][1]) / 20;

                     // set new position
                     Efi[num][0] = al_itofix(gx*20);
                     Efi[num][1] = al_itofix(gy*20);

                     if ((key[ALLEGRO_KEY_LSHIFT]) || (key[ALLEGRO_KEY_RSHIFT])) // move stuff also
                     {
                        // move podzilla's trigger box too
                        if (Ei[num][0] == 7)
                        {
                           Ei[num][11] += x_off;
                           Ei[num][12] += y_off;
                           Ei[num][13] += x_off;
                           Ei[num][14] += y_off;
                        }

                        // move cloner's stuff too
                        if (Ei[num][0] == 9)
                        {
                           Ei[num][11] += x_off; // trigger box
                           Ei[num][12] += y_off;
                           Ei[num][13] += x_off;
                           Ei[num][14] += y_off;
                           Ei[num][15] += x_off; // source
                           Ei[num][16] += y_off;
                           Ei[num][17] += x_off; // dest
                           Ei[num][18] += y_off;
                        }
                     }
                  }
               } // end of mouse on obj


               if (mouse_on_podx)
               {
                  //printf("mouse pressed on podx\n");

                  int num = mouse_on_podx_num;
                  Efi[num][5] = al_itofix(gx * 20);  // set dest x,y
                  Efi[num][6] = al_itofix(gy * 20);
                  recalc_pod(num);

               } // end of mouse on podx



               if (mouse_on_tb_ul) // move trigger box from ul
               {
                  //printf("mouse pressed on tb_ul\n");
                  // get the width and height
                  int w = Ei[num][13] - Ei[num][11];
                  int h = Ei[num][14] - Ei[num][12];
                  // set new position
                  Ei[num][11] = gx;
                  Ei[num][12] = gy;
                  Ei[num][13] = gx+w;
                  Ei[num][14] = gy+h;
               }


               if (mouse_on_tb_lr)  // resize trigger box from lr
               {
                  // prevent lr corner from being less than ul corner
                  if (gx < Ei[num][11]) gx = Ei[num][11];
                  if (gy < Ei[num][12]) gy = Ei[num][12];
                  // set new postion
                  Ei[num][13] = gx;
                  Ei[num][14] = gy;
               }
               if (mouse_on_msg_ul) // move msg
               {
                  // set new position
                  item[num][10] = gx;
                  item[num][11] = gy;
               }
               if (mouse_on_sp) // adjust sproingy jump height
               {
                  item[num][7] -= mouse_dy/2;
                  // bounds check
                  if (item[num][7] < 40) item[num][7] = 40;
                  if (item[num][7] > 200) item[num][7] = 200;
               }
               if (mouse_on_bmb) // adjust bomb blast radius
               {
                  float x0 = (float) item[num][4]+10; // get center of item location
                  float y0 = (float) item[num][5]+10;
                  float fx = (float) hx;
                  float fy = (float) hy;
                  float dist = sqrt(pow((x0-fx), 2) + pow((y0-fy), 2)); // distance from mouse to item
                  item[num][7] = (int) dist;
               }


               if (mouse_on_trk) // adjust trakbot bulet prox
               {
                  float x0 = al_fixtof(Efi[num][0])+10; // get center of item location
                  float y0 = al_fixtof(Efi[num][1])+10;
                  float fx = (float) hx;
                  float fy = (float) hy;
                  float dst = sqrt(pow((x0-fx), 2) + pow((y0-fy), 2)); // distance from mouse
                  Ei[num][17] = (int) dst;
               }

               // ranges for key, trigger, manip and damage
               if (mouse_on_kbr_ul) // move block range from ul
               {
                  // set new position
                  item[num][6] = gx*20;
                  item[num][7] = gy*20;
               }
               if (mouse_on_kbr_lr) // adjust block range from lr
               {
                  // don't allow lr to be less than ul
                  if (gx < item[num][6]/20) gx = item[num][6]/20;
                  if (gy < item[num][7]/20) gy = item[num][7]/20;

                  // set new position
                  item[num][8] = gx*20 - item[num][6];
                  item[num][9] = gy*20 - item[num][7];
               }


               if (mouse_on_csb_ul) // move cloner source box from ul
               {
                  Ei[num][15] = gx; // set new postion
                  Ei[num][16] = gy;
               } // end of mouse csb_ul

               if (mouse_on_csb_lr) // resize box from lr
               {
                  // get ul corner
                  int x1 = Ei[num][15];
                  int y1 = Ei[num][16];

                  // prevent lr corner from being less than ul corner
                  if (gx < x1+1) gx = x1+1;
                  if (gy < y1+1) gy = y1+1;

                  // set new sizes
                  Ei[num][19] = gx - x1;
                  Ei[num][20] = gy - y1;
               } // end of mouse csb_lr

               if (mouse_on_cdb_ul) // cloner destination ul
               {
                  Ei[num][17] = gx; // set new postion
                  Ei[num][18] = gy;
               } // end of mouse_on_cdb_ul










               ovw_get_block_position_on_map(&gx, &gy, &hx, &hy);


               al_flip_display();
               proc_scale_factor_change();
               proc_controllers();
               proc_frame_delay();
               get_new_background(0);
               draw_lifts();
               draw_items();

               // if current object is messages, show all messages
               if ((obt == 2) && (type == 10))
               {
                  for (int i=0; i<500; i++)
                     if (item[i][0] == 10) draw_pop_message(i);
               }




               draw_enemies();
               ovw_draw_overlays(obt, num, legend_line, highlight_counter);
               get_new_screen_buffer(3, obj_x, obj_y);




            } // end of while mouse pressed
         } // end of if mouse pressed
      } // end of mouse NOT on button window





      while ((key[ALLEGRO_KEY_ESCAPE]) || (mouse_b2))
      {
         proc_controllers();
         quit = 1;  // wait for release
      }
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
      while (key[ALLEGRO_KEY_UP])
      {
         mb = 26;
         proc_controllers();
      }
      while (key[ALLEGRO_KEY_DOWN])
      {
         mb = 27;
         proc_controllers();
      }


      switch(mb)
      {
         case 18: // move
         {
            sprintf(msg, "Object");
            if (obt==2) sprintf(msg,"%s", item_name[type]);
            if (obt==3) sprintf(msg,"%s", enemy_name[type]);
            getxy(msg,obt, type, num);
         }
         break;
         case 19:
            num = create_obj(obt, type, num);
         break;
         case 20: // delete
            if (obt ==3)
            {
               Ei[num][0] = 0;
               sort_enemy();
               if (num >= e_first_num[type]+e_num_of_type[type]) num--;
               if (e_num_of_type[type] < 1) quit = 1;
            }
            if (obt==2)
            {
               erase_item(num);
               sort_item();
               if (num >= item_first_num[type]+item_num_of_type[type]) num--;
               if (item_num_of_type[type] < 1) quit = 1;
            }
         break;
         case 21: // next
            if ((obt==3) && (++num >= e_first_num[type] + e_num_of_type[type])) num--;
            if ((obt==2) && (++num >= item_first_num[type] + item_num_of_type[type])) num--;
            new_obj = 1;

         break;
         case 22: // previous
            if ((obt==3) && (--num < e_first_num[type])) num++;
            if ((obt==2) && (--num < item_first_num[type])) num++;
            new_obj = 1;
         break;
         case 23: // copy to draw item
            draw_item_num = num;
            draw_item_type = obt;
         break;
         case 24:  // viewer help
            help("Viewer Basics");
         break;
         case 25:  // object help
         if (obt==3)
         {
            if (type == 3)  help("Archwagon Viewer");
            if (type == 4)  help("Bouncer Viewer");
            if (type == 6)  help("Cannon Viewer");
            if (type == 7)  help("Podzilla Viewer");
            if (type == 8)  help("Trakbot Viewer");
            if (type == 9)  help("Cloner Viewer");
            if (type == 10) help("Field Viewer");
            if (type == 11) help("Block Walker Viewer");
            if (type == 12) help("Flapper Viewer");
         }
         if (obt==2)
         {
            if (type == 1)  help("Door Viewer");
            if (type == 2)  help("Health Bonus Viewer");
            if (type == 3)  help("Exit Viewer");
            if (type == 4)  help("Key Viewer");
            if (type == 5)  help("Start Viewer");
            if (type == 6)  help("Free Man Viewer");
            if (type == 7)  help("Mine Viewer");
            if (type == 8)  help("Bomb Viewer");
            if (type == 10) help("Message Viewer");
            if (type == 11) help("Rocket Viewer");
            if (type == 12) help("Warp Viewer");
            if (type == 14) help("Switch Viewer");
            if (type == 15) help("Sproingy Viewer");
         }
         break;
         case 26: // up prev obj type
         {
            new_obj = 1;
            if (obt == 2)
            {
               do
               {
                  type--;
                  if (type < 1) type = 20;
               } while (item_num_of_type[type] == 0);
               num = item_first_num[type];
            }

            if (obt == 3)
            {
               while (e_num_of_type[--type] == 0)
                  if (type < 3) type = 20;
               num = e_first_num[type];
            }
         }
         break;
         case 27: // down - next obj type
         {
            new_obj = 1;
            if (obt == 2)
            {
               do
               {
                  type++;
                  if (type > 20) type = 1;
               } while (item_num_of_type[type] == 0);
               num = item_first_num[type];
            }

            if (obt == 3)
            {
               while (e_num_of_type[++type] == 0)
                  if (type > 20) type = 1;
               num = e_first_num[type];
            }
         }
         break;



      } // end of switch (mb)
   } // end of while (!quit)
   al_set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);
}
