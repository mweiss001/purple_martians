// mwLoop.h

class mwLoop
{
   public:

   mwLoop(); // default constructor
   void initialize(void);

   int state[8] = {0};

   int quit_action = 0;
   int done_action = 0;

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


   // frame_speed, frames per second, frame_num stuff
   int frame_speed = 40;
   int frame_num;
   int speed_control_lock = 0;
   int eco_draw = 0;

   int super_fast_mode = 0;

   double frame_start_timestamp = 0;


   // some global strings
   char local_hostname[80];
   char pm_version_string[80];
   char al_version_string[80];

   int ff_state; // if fast forwarding, do not resend game events

   ALLEGRO_BITMAP * cutscene_background;
   float cutscene_original_zoom;
   float cutscene_accel;
   float cutscene_bg_x;



   int load_and_setup_level_load(int level);
   void setup_level_after_load();
   void setup_players_after_level_load(int type);
   void setup_common_after_level_load(void);


   void move_frame(void);
   void loop_frame(int);


   void initialize_and_resize_remote_graphs(void);

   void initialize_graphs();

   int remote_graphs_width = 600;
   int remote_graphs_height = 400;

   int remote_frames_since_last_rctl_sent = 0;

   void add_local_cpu_data(double cpu);

   int have_all_players_acknowledged(void);
   void game_menu(void);
   void proc_level_done_mode(void);
   void proc_program_state(void);

   void remote_control_loop(void);

   void main_loop(void);
};
extern mwLoop mLoop;

