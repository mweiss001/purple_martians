// mwColor.cpp

#include "pm.h"
#include "mwColor.h"

mwColor mColor;

mwColor::mwColor()
{
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

   // fade base colors 1-15
   for (int a=1; a<16; a++)
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

   // reverse fade black (color 0)
   for (int x=1; x<16; x++)
   {
      int c = (x+1)*16 - 1;
      pc[x*16]  = al_map_rgb(c, c, c);
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
}

void mwColor::show_palette(void)
{
   int x = 100;
   int y = 100;
   int sz = 10;

   for (int a=0; a<16; a++)
      for (int b=0; b<16; b++)
         al_draw_filled_rectangle(x+a*sz, y+b*sz, x+a*sz+sz-1, y+b*sz+sz-1, pc[a*16+b]);
   al_draw_rectangle(x, y, x+16*sz-1, y+16*sz-1, White, 0);

   for (int x=0; x<16; x++)
   {
      unsigned char ur, ug, ub;
      al_unmap_rgb(pc[x], &ur, &ug, &ub);
      printf("%2d r:%3d g:%3d b:%3d \n", x, ur, ug, ub);
   }
}

void mwColor::process_flash_color(void)
{
   if (++flash_counter > 32) flash_counter = 1;
   if ((flash_counter  > 0)  && (flash_counter < 9))   { flash_color = 10;  flash_color2 = 15; }
   if ((flash_counter  > 8)  && (flash_counter < 15))  { flash_color = 14;  flash_color2 = 11; }
   if ((flash_counter  > 16) && (flash_counter < 23))  { flash_color = 15;  flash_color2 = 10; }
   if ((flash_counter  > 24) && (flash_counter < 33))  { flash_color = 11;  flash_color2 = 14; }
   Flash1 = pc[flash_color];
   Flash2 = pc[flash_color2];
}

int mwColor::get_contrasting_color(int color)
{
   if (color == 15) return 10;
   if (color == 14) return 2;
   if (color == 13) return 8;
//   if (color == 12) return 15;
//   if (color == 11) return 15;
//   if (color == 10) return 15;
//   if (color == 9) return 15;
//   if (color == 8) return 15;
//   if (color == 7) return 15;
//   if (color == 6) return 15;
//   if (color == 5) return 15;
//   if (color == 4) return 15;
//   if (color == 3) return 15;
//   if (color == 2) return 15;
//   if (color == 1) return 15;
//   if (color == 0) return 15;
   return 15;
}
