// mwTransitions.h

class mwTransitions
{
   public:


   // level done
   // --------------------------------
   void proc_level_done_mode();

   ALLEGRO_BITMAP * cutscene_background;
   float cutscene_original_zoom;
   float cutscene_accel;
   float cutscene_bg_x;

   int level_done_seek_time = 10;
   int level_done_rotate_time = 10;
   float level_done_num_rotations = 1;
   float level_done_player_final_scale = .1;
   float level_done_transition_time = 40;



   // gate enter
   // --------------------------------

   void proc_gate_enter_mode();

   int gate_enter_seek_time = 10;
   int gate_enter_rotate_time = 10;
   int gate_enter_num_rotations = 1;
   float gate_enter_player_final_scale = .1;
   int gate_enter_transition_time = 10;




   float gate_enter_level_icon_size;

   int gate_enter_next_level_start_x;
   int gate_enter_next_level_start_y;



   // transitions
   // --------------------------------
   void set_transition_mode(int i, int f, int num_steps);
   void proc_transition_mode();

   float transition_initial_x;
   float transition_initial_y;
   float transition_initial_scale;

   float transition_final_x;
   float transition_final_y;
   float transition_final_scale;

   float transition_current_x;
   float transition_current_y;
   float transition_current_scale;

   float transition_x_inc;
   float transition_y_inc;
   float transition_scale_multiplier;

   int transition_steps = 60;


   void transition_cutscene(int i, int f);
   void do_transition(float fmxi, float fmyi, float fmxf, float fmyf, float sci, float scf, float num_steps, float delay);

   float gate_transition_x;
   float gate_transition_y;
   float gate_transition_wx;
   float gate_transition_wy;
   float gate_transition_scale;

   int transition_num_steps;
   int transition_delay;











};

extern mwTransitions mTransitions;

