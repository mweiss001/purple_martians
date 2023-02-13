// mwProgramState.cpp

#include "pm.h"
#include "mwProgramState.h"

mwProgramState mwPS;

mwProgramState::mwProgramState()
{
   initialize();
}

void mwProgramState::initialize(void)
{
   program_state = 0;
   new_program_state = 0;
   old_program_state = 0;
   older_program_state = 0;

   top_menu_sel = 3;
   main_loop_exit = 0;

   level_editor_running = 0;
   help_screens_running = 0;
   visual_level_select_running = 0;


   show_debug_overlay = 0;

   // temp testing variables
   pct_x = 0;
   pct_y = 0;

   // frame_speed, frames per second, mwPS.frame_num stuff
   speed_testing = 0;
   frame_speed = 40;
   frame_num = 0;
   speed_control_lock = 1;
   eco_draw = 0;
}

