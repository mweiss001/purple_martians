// z_enemy.h

extern int Ei[100][32];        // enemy ints
extern al_fixed Efi[100][16];  // enemy fixeds
extern int e_num_of_type[50];
extern int e_first_num[50];
extern char enemy_name[100][2][40];
extern int num_enemy;
extern int enemy_tile[20];


int enemy_data(int x_pos, int y_pos);
void rectangle_with_diagonal_lines(float x1, float y1, float x2, float y2, int spacing, int frame_color, int line_color, int clip_mode);
void draw_enemy(int e, int custom, int cx, int cy);
void draw_enemies(void);
void proc_enemy_collision_with_pshot(int e);
void move_enemies();
void enemy_deathcount(int e);
void enemy_player_hit_proc(int e);
void enemy_killed(int e);
void enemy_flapper(int e);
int is_player_in_trigger_box(int x1, int y1, int x2, int y2);
void cloner_create(int e);
void enemy_cloner(int e);
void set_trakbot_mode(int e, int mode);
void enemy_trakbot(int e);
void enemy_podzilla(int e);
void enemy_vinepod(int e);
void bouncer_cannon_common(int e);
void enemy_cannon(int e);
void enemy_bouncer(int e);
void enemy_archwagon(int e);
void walker_archwagon_common(int e);
void enemy_block_walker(int e);
void enemy_jumpworm(int e);



// e_enemy.h
void erase_enemy(int e);
int get_empty_enemy(void);
int get_empty_enemy(int type);
void recalc_pod(int e);
void get_pod_extended_position(int e, int *x, int *y);
void show_level_data(void);
void show_all_enemies(void);
void sort_enemy(void);
int create_cloner(void);
int create_pod(void);
int create_vinepod(void);

