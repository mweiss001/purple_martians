// z_item.h
extern int item[500][16];      // item ints
extern float itemf[500][4]; // item floats

extern int item_num_of_type[30];
extern int item_first_num[30];
extern char item_name[30][40];
extern char pmsgtext[500][500];
extern int item_tile[20];

int item_data(int x, int y);
void change_linked_door_color_and_shape(int door);
void draw_pop_message(int i, int custom, int xpos_c, int ypos, int cursor_pos, int cursor_blink, char *f);
void draw_door(int i, int x, int y, int custom);
int seq_color(int mod, int c1, int c2);
int seq_color2(void);
int seq_color3(void);
void bomb_block_crosshairs(int e, int f);
void bomb_blocks(int i, int t, int dr);
void bomb_crosshairs(float x, float y);
void bomb_enemies(int i, int t, int dr);
void bomb_players(int i, int t, int dr);
void proc_lit_bomb(int i);
void draw_lit_bomb(int i);
void draw_rocket_lines(int i);
void draw_item(int i, int custom, int cx, int cy);
void draw_items(void);
void proc_pmsg_reset_timer(int i);
void proc_pmsg(int i);
int is_item_stuck_to_wall(int i);
void proc_switch_block_range(int i, int action);
void proc_key_block_range(int i, int action);
void proc_moving_key(int i);
void move_items();
int player_drop_item(int p);
void proc_player_carry(int p);
int proc_orb_shot_collision(int i);
void proc_orb(int i);
void draw_orb(int i, int x, int y);
void proc_orb_collision(int p, int i);
void proc_door_collision(int p, int i);
void proc_start_collision(int p, int i);
void proc_bonus_collision(int p, int i);
void proc_exit_collision(int p, int i);
void proc_key_collision(int p, int i);
void proc_mine_collision(int p, int i);
void proc_bomb_collision(int p, int i);
void proc_rocket_collision(int p, int i);
void proc_warp_collision(int p, int i);
void proc_switch_collision(int p, int i);
void proc_sproingy_collision(int p, int i);
void proc_item_collision(int p, int i);
void proc_lit_rocket(int i);
void proc_trigger(int i);
void set_item_trigger_location_from_lift(int i, int a20);
void detect_trigger_collisions(int i);
void draw_trigger(int i, int x, int y);
void proc_block_manip(int i);
void draw_block_manip(int i, int x, int y);
void set_item_damage_location_from_lift(int i, int a20);
void proc_item_damage_collisions(int i);
void draw_block_damage(int i, int x, int y, int custom);
void proc_block_damage(int i);




// e_item.h
void show_all_items(void);
int sort_item(int set_pos);
int get_empty_item(void);
int get_empty_item(int type);
void check_item(int i, int ct);
void test_items(void);
void erase_item(int num);
int create_trigger(int i);
int create_block_manip(int i);
int create_block_damage(int i);
int create_start_block(int c);
int create_exit(int c);
void show_all_pmsg(void);
int create_pmsg(int c);
int create_door(int type);
int create_item(int type);


