// mwLoop.h

class mwLoop
{
   public:

   mwLoop(); // default constructor
   void initialize(void);


   int program_state = 0;
   int new_program_state = 0;
   int old_program_state = 0;
   int older_program_state = 0;

   int top_menu_sel = 3;
   int main_loop_exit = 0;

   // temp testing variables
   int pct_x = 0;
   int pct_y = 0;

   int autosave_level_editor_state;
   int level_editor_running;
   int help_screens_running;
   int visual_level_select_running;

   int show_debug_overlay;

   int show_player_join_quit_timer;
   int show_player_join_quit_player;
   int show_player_join_quit_jq;


   // frame_speed, frames per second, frame_num stuff
   int speed_testing = 0;
   int frame_speed = 40;
   int frame_num;
   int speed_control_lock = 1;
   int eco_draw = 0;

   // some global strings

   char local_hostname[80];
   char pm_version_string[80];
   char al_version_string[80];


   int ff_state;


   void draw_frame(void);
   void move_frame(void);
   void loop_frame(int);
   int have_all_players_acknowledged(void);
   void game_menu(void);
   void proc_level_done_mode(void);
   void proc_program_state(void);
   void main_loop(void);

};
extern mwLoop mLoop;


