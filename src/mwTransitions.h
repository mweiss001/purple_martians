// mwTransitions.h

class mwTransitions
{
   public:

   void set_transition_speed();

   const char* transition_speed_text[5]  = { "Fastest", "Hasty", "Normal", "Relaxed"};


   // gate enter
   // --------------------------------
   int gate_enter_seek_time = 20;
   int gate_enter_rotate_time = 20;
   int gate_enter_num_rotations = 4;
   float gate_enter_player_final_scale = .1;
   int gate_enter_transition_time = 20;


   // level done
   // --------------------------------
   void proc_level_done_mode();
   int level_done_seek_time = 20;
   int level_done_rotate_time = 20;
   float level_done_num_rotations = 4;
   float level_done_player_final_scale = .4;
   float level_done_transition_time = 20;

   float min_scale = 0.0001;
   bool scale_compound_interest_mode = 0;


   // these are set in level done mode 6 and used in mode 5
   // controls the player moving from level icon to gate
   // -----------------------------------------------------
   float player_exit_transition_time = 20;
   float px_initial;
   float py_initial;
   float px_final;
   float py_final;
   float px_inc;
   float py_inc;
   float px_current;
   float py_current;
   float pscale_initial;
   float pscale_final;
   float pscale_current;
   float pscale_inc;
   float p_num_rotations = 4;
   float prot_inc;



   // rocket move
   // ----------------
   ALLEGRO_BITMAP * cutscene_background;
   float cutscene_original_zoom;
   float cutscene_accel;
   float cutscene_bg_x;




   // transition variables
   // ---------------------
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

   int transition_num_steps;



   // blocking transitions
   // -------------------------------------
   void transition_cutscene(int initial, int final, bool show_player = true);

//   int transition_delay;



};

extern mwTransitions mTransitions;

