// mwEnemy.h

class mwEnemy
{
   public:
   mwEnemy() { fill_strings(); }


   int Ei[100][32];        // enemy ints
   float Ef[100][16];      // enemy floats
   int e_num_of_type[50];
   int e_first_num[50];
   char enemy_name[100][3][40] = {0};
   int num_enemy;
   int enemy_tile[50];


   void draw_enemy(int e, int custom, int cx, int cy);
   void draw_enemies();

   void move_enemies();

   void enemy_killed(int e);
   void enemy_deathcount(int e);

   void enemy_player_hit_proc(int e);
   void proc_enemy_collision_with_pshot(int e);

   // member functions in other files

   // mwEnemyArchwagon.cpp
   void move_archwagon(int e);
   void move_arch_block_common(int e);
   void move_blokwalk(int e);

   void move_crew(int e);
   void draw_crew(int e, int x, int y, int custom);
   void draw_crew_screen_direct(int e);



   // mwEnemyCannon.cpp
   void move_bouncer_cannon_common(int e);
   void move_cannon(int e);
   void move_bouncer(int e);
   void scale_bouncer_and_cannon_speed(int e);
   void set_new_initial_direction(int e, int x2, int y2);

   // mwEnemyCloner.cpp
   void draw_cloner(int e, int x, int c, int custom);
   void cloner_create(int e);
   void move_cloner(int e);



   // mwEnemyFlapper.cpp
   void move_flapper(int e);

   // mwEnemyJumpworm.cpp
   void move_jumpworm(int e);

   // mwEnemyTrakbot.cpp
   void set_trakbot_mode(int e, int mode);
   void move_trakbot(int e);

   // mwEnemyVinepod.cpp
   void move_podzilla(int e);
   void move_vinepod(int e);

//   void draw_vinepod_screen_direct(int e);

   void draw_vinepod(int e, int x, int c, int custom);
   void draw_vinepod_controls(int num, int legend_highlight);
   void vinepod_fill_points_array(int e, int np, float pnts[], float dest[]);
   void vinepod_set_cp_thirds(int e);
   void calc_simple_vinepod(int e);

   void vinepod_rotate(int e, float a);

   void vinepod_scale(int e, float s);


   void vinepod_rotate_cw_90(int e);
   void vinepod_rotate_ccw_90(int e);
   void vinepod_mirror_x(int e);
   void vinepod_mirror_y(int e);



   // mwEnemyFnx.cpp
   int is_player_in_enemy_trigger_box(int e);
   void set_enemy_rot_from_incs(int e);


   void set_enemy_rot_from_players_position(int e, int p, int extrapolate);

   void set_enemy_xyinc_from_player(int e, int p);
   int find_closest_player(int e);
   int find_closest_player_flapper(int e);
   int find_closest_player_trakbot(int e);
   int find_closest_player_cannon(int e);
   float deg_to_rad(float deg);

   // mwEnemyEditorFnx.cpp
   void fill_strings();
   int show_enemy_data(int x_pos, int y_pos);
   void show_all_enemies();
   void sort_enemy();
   void erase_enemy(int e);
   int get_empty_enemy();
   int get_empty_enemy(int type);
   int create_cloner();
   int create_vinepod();

};
extern mwEnemy mEnemy;

