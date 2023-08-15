// mwLevel.h

struct level_data
{
   char level_name[200];

   int status;
   int status_color;
   char status_text[16];

   int min_respawns;
   int max_purple_coins_collected;
   int tot_purple_coins;
   int min_enemies_left;
   int min_enemies_left_par;
   int max_enemies_killed;

   int times_played;
   int times_beat;
   int times_quit;

   int time_par;
   int time_best;
   int time_best_all_coins;
   int time_worst;
   int time_average;
   int time_total;

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

   int start_level;
   int play_level;

   int valid_level_loaded;
   int prev_level_loaded;
   int last_level_loaded;
   int resume_allowed;

   int number_of_starts;


   void change_block(int x, int y, int block);
   int is_block_empty(int x, int y, int test_block, int test_item, int test_enemy);

   void set_start_level(void);
   void set_start_level(int s);

   void next_level(void);
   void prev_level(void);

   int get_next_level(int lev);


   int load_level(int level_to_load, int load_only, int fail_silently);
   void save_level(int level_to_save);

   int load_level_prompt();
   int save_level_prompt();



   void show_level_stats_outline_with_thicker_lines(int x1, int x2, int gy, int y, int draw, ALLEGRO_COLOR c);

   void show_level_stats_totals(int x1, int x2, int gy, int ty, int& max_x, int& y, int draw, int vline[], int &vli, int tally[][16], int col, int msg_type);

   void show_level_stats_title_and_header(int x1, int x2, int& gy, int& ty, int& max_x, int& y, int draw, int vline[], int &vli, int tally[][16], int col, const char* title, int msg_type);

   void show_level_stats_row(int i, int x1, int x2, int draw, int &max_x, int &y, int vline[], int &vli, int tally[][16], int msg_type);

   int show_level_data(int x_pos, int y_pos, int type);
   void zero_level_data(void);
   void level_check(void);


   void faa_helper(int x1, int x2, int y1, int y2, int xasc, int &aai, int area);

   void fill_area_array(void);

   struct level_data data[100];
   struct level_play_data play_data[100000];
   int play_data_num;

   int area_locks[16];

   const char* area_names[20] =
   {
      "",
      "Area 1",
      "Area 2",
      "Area 3",
      "Area 4",
      "Area 5",
      "Area 6",
      "Area 7",
      "Area 8",
      "Area 9",
      "Extra",
      "2P",
      "Demo",
      "Training",
      "Training"
   };



   int area_array[100][2];

   void unlock_all_level_in_area(int area);

   void reset_level_data(void);
   void unlock_all_levels(void);

   void draw_level_stats(int x1, int y1, int msg_type);
   void show_level_stats(int &w, int &h, int draw, int msg_type );


   void sob_hline(int x1, int x2, int y, int a);
   void sob_vline(int x, int y1, int y2, int a);
   void sob_create_msg(const char* txt, int col, int x, int y, int w, int h);
   void sob_area_msg(int area, int x, int y);
   void set_overworld_barriers(void);

   int find_msg(const char* str);

   void clear_data(void);
   void load_data(void);
   void save_data(void);
   void setup_data(void);

   void dump_level_data(void);

   void level_start_data(void);

   void add_play_data_record(int lev, int type);
   void calc_level_stats(int lev);
   void check_achievments(void);

   void update_level_status(int lev);


   ALLEGRO_BITMAP * level_icon_100[100];
   ALLEGRO_BITMAP * level_icon_200[100];


   ALLEGRO_BITMAP * level_stats_bitmap = NULL;
   int level_stats_bmp_msg_type = 0;
   int level_stats_bmp_w;
   int level_stats_bmp_h;



   void load_level_icons(void);
   void create_level_icons(void);








   int level_data_purple_coins_collected;
   int level_data_player_respawns;
   int level_data_enemies_killed;

   int display_page;

   int skc_trigger_demo;
   int skc_trigger_demo_cheat;


};
extern mwLevel mLevel;

