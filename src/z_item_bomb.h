// z_item_bomb.h



void proc_bomb_collision(int p, int i);
void proc_rocket_collision(int p, int i);
void proc_lit_rocket(int i);


int seq_color(int mod, int c1, int c2);
int seq_color2(void);
int seq_color3(void);
void bomb_block_crosshairs(int e, int f);
//void bomb_blocks(int i, int t, int dr, float x, float y);
//void bomb_crosshairs(float x, float y);
//void bomb_enemies(int i, int t, int dr, float x, float y);
//void bomb_players(int i, int t, int dr, float x, float y);
void proc_lit_bomb(int i);
void draw_lit_bomb(int i);
void draw_rocket_lines(int i);
