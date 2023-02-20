// z_item.h
extern int item[500][16];      // item ints
extern float itemf[500][4]; // item floats

extern int item_num_of_type[30];
extern int item_first_num[30];
extern char item_name[30][40];
extern char pmsgtext[500][500];
extern int item_tile[20];


void draw_item(int i, int custom, int cx, int cy);
void draw_items(void);

int is_item_stuck_to_wall(int i);
void move_items();
int player_drop_item(int p);
void proc_player_carry(int p);

void proc_start_collision(int p, int i);
void proc_bonus_collision(int p, int i);
void proc_exit_collision(int p, int i);
void proc_mine_collision(int p, int i);
void proc_warp_collision(int p, int i);
void proc_sproingy_collision(int p, int i);
void proc_item_collision(int p, int i);


// e_item.h
void set_item_text(void);
int item_data(int x, int y);
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


