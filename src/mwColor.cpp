// mwColor.cpp

#include "pm.h"
#include "mwColor.h"

mwColor mC;

mwColor::mwColor()
{
   initialize();
}

void mwColor::initialize(void)
{
   // printf("make palette\n");

   pc[0]  = al_map_rgb(  0,   0,   0); // black
   pc[1]  = al_map_rgb(191, 108, 232); // alt purple 1
   pc[2]  = al_map_rgb(136,  32, 172); // alt purple 2
   pc[3]  = al_map_rgb( 60, 127, 255); // alt blue
   pc[4]  = al_map_rgb(224,  28,  72); // alt red
   pc[5]  = al_map_rgb(255,   0, 232); // pink
   pc[6]  = al_map_rgb(255, 191, 127); // taan
   pc[7]  = al_map_rgb(255, 127,   0); // orange
   pc[8]  = al_map_rgb(127,   0, 255); // original purple
   pc[9]  = al_map_rgb(  0, 255, 127); // alt green
   pc[10] = al_map_rgb(255,   0,   0); // red
   pc[11] = al_map_rgb(  0, 255,   0); // green
   pc[12] = al_map_rgb(  0,   0, 255); // blue
   pc[13] = al_map_rgb(  0, 255, 255); // lt blue
   pc[14] = al_map_rgb(255, 255,   0); // yellow
   pc[15] = al_map_rgb(255, 255, 255); // white

   // special case reversed white (0)
   for (int x=1; x<16; x++)
   {
      int c = (x+1)*16 - 1;
      pc[x*16]  = al_map_rgb(c, c, c);
   }

   // all the other base colors from 1-15
   for (int a=1; a<16; a++) // iterate base colors
   {
      // extract r, g, b in float format
      float r, g, b;
      al_unmap_rgb_f(pc[a], &r, &g, &b);

      for (int x=1; x<16; x++)
      {
         float nr = r * (1 - ((float)x/15));
         float ng = g * (1 - ((float)x/15));
         float nb = b * (1 - ((float)x/15));
         pc[a+x*16]  = al_map_rgb_f(nr, ng, nb);
      }
   }


   strcpy (color_name[0],  "Zombie");
   strcpy (color_name[1],  "Violet");
   strcpy (color_name[2],  "Mauve");
   strcpy (color_name[3],  "Bluey");
   strcpy (color_name[4],  "Reddy");
   strcpy (color_name[5],  "Pink");
   strcpy (color_name[6],  "Taan");
   strcpy (color_name[7],  "Orange");
   strcpy (color_name[8],  "Purple");
   strcpy (color_name[9],  "Forest");
   strcpy (color_name[10], "Red");
   strcpy (color_name[11], "Green");
   strcpy (color_name[12], "Blue");
   strcpy (color_name[13], "Aqua");
   strcpy (color_name[14], "Yellow");
   strcpy (color_name[15], "White");

   Black  = pc[0];
   Violet = pc[1];
   Mauve  = pc[2];
   Bluey  = pc[3];
   Reddy  = pc[4];
   Pink   = pc[5];
   Taan   = pc[6];
   Orange = pc[7];
   Purple = pc[8];
   Forest = pc[9];
   Red    = pc[10];
   Green  = pc[11];
   Blue   = pc[12];
   Aqua   = pc[13];
   Yellow = pc[14];
   White  = pc[15];

//   for (int x=0; x<16; x++)
//   {
//      unsigned char ur, ug, ub;
//      al_unmap_rgb(mC.pc[a+x*16], &ur, &ug, &ub);
//      printf("%2d %3d r:%3d g:%3d b:%3d \n", x, a+x*16, ur, ug, ub );
//   }
//   for (int x=0; x<16; x++)
//   al_draw_line(10, 10+x*2, 200, 10+x*2, mC.pc[10+x*16], 2);
//   int sz = 16;
//   al_draw_rectangle(0, 0, 17*sz, 17*sz, mC.pc[15], 2);
//
//   for (int a=0; a<16; a++)
//      for (int b=0; b<16; b++)
//         al_draw_filled_rectangle(a*sz, b*sz, a*sz+sz, b*sz+sz, mC.pc[b*16+a]);
//   al_draw_text(mF.pr8, mC.pc[9], 400, 400, ALLEGRO_ALIGN_CENTRE, "Hello World");

}







//void process_flash_color(void)
//{
//   if (++flash_counter > 16) flash_counter = 1;
//   if ((flash_counter > 0)  && (flash_counter < 5))  { flash_color = 10; flash_color2 = 15; }
//   if ((flash_counter > 4)  && (flash_counter < 9))  { flash_color = 14; flash_color2 = 11; }
//   if ((flash_counter > 8)  && (flash_counter < 13)) { flash_color = 15; flash_color2 = 10; }
//   if ((flash_counter > 12) && (flash_counter < 17)) { flash_color = 11; flash_color2 = 14; }
//}


void mwColor::process_flash_color(void)
{
   if (++flash_counter > 32) flash_counter = 1;
   if ((flash_counter > 0)  && (flash_counter < 9))   { flash_color = 10; flash_color2 = 15; }
   if ((flash_counter > 8)  && (flash_counter < 15))  { flash_color = 14; flash_color2 = 11; }
   if ((flash_counter > 16)  && (flash_counter < 23)) { flash_color = 15; flash_color2 = 10; }
   if ((flash_counter > 24) && (flash_counter < 33))  { flash_color = 11; flash_color2 = 14; }

   Flash1 = pc[flash_color];
   Flash2 = pc[flash_color2];



}














