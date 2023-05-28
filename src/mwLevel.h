// mwLevel.h











struct level_data
{
   char level_name[200];
   int par_time;
   int unlocked;
   int completed;
   int best_time;
   int min_respawns;

   int max_purple_coins_collected;
   int tot_purple_coins;

   int min_enemies_left;
   int min_enemies_left_par;



   int max_enemies_killed;



};


struct level_play_data
{
   int level;
   double start_timestamp;
   int timer;

   int completed;

   int number_players;
   int enemies_killed;
   int enemies_left_alive_at_exit;
   int player_respawns;
   int purple_coins_collected;

};



class mwLevel
{
   public:


   int l[100][100];


   int warp_level_location;
   int gate_level;


   int start_level;
   int play_level;

   int overworld_level;

   int valid_level_loaded;
   int last_level_loaded;
   int resume_allowed;

   int number_of_starts;


   void change_block(int x, int y, int block);
   int is_block_empty(int x, int y, int test_block, int test_item, int test_enemy);

   void set_start_level(void);
   void set_start_level(int s);

   void next_level(void);
   void prev_level(void);

   int load_level(int level_to_load, int load_only, int fail_silently);
   void save_level(int level_to_save);

   int load_level_prompt();
   int save_level_prompt();




   int show_level_data(int x_pos, int y_pos, int type);
   void zero_level_data(void);
   void level_check(void);



   struct level_data data[100];

   struct level_play_data play_data[10000];
   int play_data_num;


   void reset_level_data(void);

   void clear_data(void);
   void load_data(void);
   void save_data(void);
   void setup_data(void);

   void level_start_data(void);
   void level_complete_data(void);


   ALLEGRO_BITMAP * level_icon[100];

   void load_level_icons(void);
   void create_level_icons(void);


   int level_data_purple_coins_collected;
   int level_data_player_respawns;
   int level_data_enemies_killed;

   int display_page;

   int skc_trigger_demo;



};
extern mwLevel mLevel;



