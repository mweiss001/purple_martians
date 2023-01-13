// z_game_menu.cpp
#include "pm.h"
void game_menu(void)
{
   old_program_state = 1;
   if (!splash_screen_done) { splash_screen(); splash_screen_done = 1; }
   if (!resume_allowed) load_level(start_level, 0);
   if (top_menu_sel < 3) top_menu_sel = 3;
   while (top_menu_sel != 1)
   {
      top_menu_sel = zmenu(7, top_menu_sel, 10);
      if  (top_menu_sel == 1)  { program_state = 0;                                           return; } // exit
      if  (top_menu_sel == 2)  { visual_level_select(); top_menu_sel = 3;                             } // visual level select
      if ((top_menu_sel == 4) && (resume_allowed)) { new_program_state = 13;                  return; } // resume game
      if  (top_menu_sel == 3)  { new_program_state = 10;  top_menu_sel = 4;                   return; } // start new game
      if  (top_menu_sel == 5)  { new_program_state = 20;                                      return; } // host network game
      if  (top_menu_sel == 6)  { new_program_state = 24;                                      return; } // join network game
      if  (top_menu_sel == 7)  { new_program_state = 3;                                       return; } // settings
      if  (top_menu_sel == 8)  { play_level = edit_menu(start_level); new_program_state = 10; return; } // level editor
      if  (top_menu_sel == 9)  { new_program_state = 2;  older_program_state = 1;             return; } // demo mode
      if  (top_menu_sel == 10)                                                                help(""); // help
      if (top_menu_sel == 102) if (++start_level > 399) start_level = 399; // start level inc
      if (top_menu_sel == 202) if (--start_level < 1) start_level = 1;     // start level dec
      if (top_menu_sel > 100) { set_start_level(start_level); load_level(start_level, 0); top_menu_sel = 2; }
   }
}
