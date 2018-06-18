// zmap.cpp

#include "pm.h"

void set_map_position()
{

//  1   9   2
//      |
// -----+-----
//      |
//  4   |   3

   int alp = active_local_player;

   int px1 = players1[alp].sbx1;
   int py1 = players1[alp].sby1;
   int px2 = players1[alp].sbx2;
   int py2 = players1[alp].sby2;

//   printf("gmo:%d mx:%d my:%d px1:%d py1:%d px2:%d py2:%d\n",game_map_on, map_x, map_y, px1, py1, px2, py2);
   switch (game_map_on)
   {
      case 0:
         map_x = BORDER_WIDTH;
         map_y = BORDER_WIDTH;
      break;
      case 1:
         map_x = BORDER_WIDTH;
         map_y = BORDER_WIDTH;
         // check to see if player is covered by the map
         if ((px1 < map_x + map_size)
          && (py1 < map_y + map_size))  game_map_on = 2;
      break;
      case 2:
         map_x = SCREEN_W - map_size - BORDER_WIDTH;
         map_y = BORDER_WIDTH;
         if ((px2 > map_x)
          && (py1 < map_y + map_size))  game_map_on = 1;
      break;
      case 3:
         map_x = SCREEN_W - map_size - BORDER_WIDTH;
         map_y = SCREEN_H - map_size - BORDER_WIDTH;
         if ((px2 > map_x)
          && (py2 > map_y))  game_map_on = 4;
      break;
      case 4:
         map_x = BORDER_WIDTH;
         map_y = SCREEN_H - map_size - BORDER_WIDTH;
         if ((px1 < map_x + map_size)
          && (py2 > map_y))  game_map_on = 3;
      break;
      case 9:
         map_x = SCREEN_W/2 - map_size/2;
         map_y = BORDER_WIDTH;
         if ((px2 > map_x)
          && (px1 < map_x + map_size)
          && (py1 < map_y + map_size))  game_map_on = 4;
      break;
   }
}

void next_map_mode()
{
   int old_game_map_on = game_map_on;
   switch (game_map_on)
   {
      case 0: game_map_on = 1; break;
      case 1: game_map_on = 2; break;
      case 2: game_map_on = 3; break;
      case 3: game_map_on = 4; break;
      case 4: game_map_on = 9; break;
      case 9: game_map_on = 0; break;
   }
   set_map_position();
   if (old_game_map_on == game_map_on) game_map_on = 0;
}

void next_map_size()
{
   int smin = 0;
   if (SCREEN_H < SCREEN_W) smin = SCREEN_H;
   else smin = SCREEN_W;

   if (new_size == smin /4) new_size = smin/3; // 1/3
   else if (new_size == smin /3) new_size = smin/2; // 1/2
   else if (new_size == smin /2) new_size = smin/4; // 1/4
   else  new_size = smin /3; // if for some reason none of these match
}

void draw_map()
{
   if (game_map_on)
   {
      // process the size change gradually
      if (map_size < new_size)
      {
         map_size+=30;
         if (map_size > new_size) map_size = new_size;
      }
      if (map_size > new_size)
      {
         map_size-=60;
         if (map_size < new_size) map_size = new_size;
      }
      set_map_position();
      al_draw_scaled_bitmap(level_buffer, 0,0, 2000, 2000, map_x, map_y, map_size, map_size, 0);
   }
}
