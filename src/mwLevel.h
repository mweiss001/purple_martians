// mwLevel.h











struct level_data
{
   char level_name[200];
   int par_time;
   int unlocked;
   int completed;
   int best_time;
   int min_deaths;
};




class mwLevel
{
   public:

   struct level_data data[100];

   void clear_data(void);
   void load_data(void);
   void save_data(void);


   ALLEGRO_BITMAP * level_icon[100];
//   int level_icon_loaded_num = 0;

   void load_level_icons();

   int l[100][100];
   int warp_level_location;
   int start_level;
   int play_level;
   int valid_level_loaded;
   int last_level_loaded;
   int resume_allowed;



   int number_of_starts;
   int number_of_purple_coins;



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
};
extern mwLevel mLevel;



