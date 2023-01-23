// z_player.h


extern struct player players[NUM_PLAYERS];
extern struct player1 players1[NUM_PLAYERS];
extern int active_local_player;
struct player // synced between server and client
{
   int active;  // all routines that process player will skip inactive
   int control_method; // 0 = local, 1 = file play, 2 = remote view; 3 = server_local; 4 = client_local
   int paused;
   int paused_type; // 1 = death, 2 = door move
   int paused_mode;
   int paused_mode_count;

   int level_done_mode;

   int level_done_timer;
   int level_done_ack;
   int level_done_x;
   int level_done_y;
   int level_done_player;
   int level_done_next_level;

   al_fixed PX, PY;       // players position
   al_fixed xinc, yinc;   // players momentum
   al_fixed right_xinc, left_xinc;

   al_fixed LIFE;
   al_fixed old_LIFE;

   int on_ladder;
   int on_rope;
   int player_ride;
   int carry_item;

   int shape; // index to player_tile
   int color; // used to draw frames and stuff in players color
   al_fixed draw_rot;
   al_fixed draw_scale;
   int door_draw_rot_num_steps;
   al_fixed door_draw_rot;
   al_fixed door_draw_rot_inc;
   int marked_door;
   int door_item;
   int door_xinc;
   int door_yinc;
   int door_num_steps;

   int spawn_point_index;

   int left_right; // determines the direction the player is facing


   // flags that indicate a control has been activated or held
   int up, down, left, right, jump, fire, fire_held, menu;

   int bullet_wait_counter, request_bullet, bullet_wait, bullet_speed;

   int stat_respawns;
   int stat_bullets_fired;
   int stat_enemy_hits;
   int stat_player_hits;
   int stat_self_hits;
   int stat_purple_coins;

   int stat_enemy_exploded;
   int stat_player_exploded;
   int stat_self_exploded;

   al_fixed stat_LIFE_inc;
   al_fixed stat_LIFE_dec;
   al_fixed stat_LIFE_wasted;

   int spare_int1;


};

struct player1 // not synced between server and client
{
   int health_display;
   int last_health_adjust;
   int potential_bomb_damage;

   float field_damage_tally;
   int field_damage_holdoff;
   int field_damage_enemy_number;

   int up_key, down_key, left_key, right_key, jump_key, fire_key, menu_key;
   int comp_move, old_comp_move;

   int fake_keypress_mode;

   int frames_skipped;
   int frames_skipped_last_sec;
   int frames_skipped_last_sec_tally;

   int timer_adjust;
   int timer_adjust_last_sec;
   int timer_adjust_last_sec_tally;


   int who; // for network id of clients
   char hostname[16];

   int server_last_stak_rx_frame_num; // used by server to see if client is still responding

   int client_last_stdf_rx_frame_num; // used by client to see if server is still responding



   int server_send_dif; // used by server to determine which frame to send dif


   int client_base_resets;

   int client_last_dif_applied;

   int client_sync;
   int server_sync;

   int client_ping_flag;

   double ping;
   double ping_avg;
   double dsync;
   double dsync_avg;



   double client_chase_offset;

   int sc_sync; // packet stdf from s to c  sfn-cfn
   int cs_sync; // packet stak from c to s  cfn-sfn
   int rt_sync; // round trip to server back to server through client via stdf and stak

   int last_gm_index;
   int last_gm_frame_num;
   int last_gm_comp_move;

   double client_chase_fps;
   int server_game_move_sync;

   int client_cdat_packets_tx;

   int made_active_holdoff;
   int sync_stabilization_holdoff;

   int join_frame;
   int quit_frame;
   int quit_reason;

   // server rewind and stdf frequencies
   int s1;
   int s2;

   // used only to display on server screen overlay in client grid
   int num_dif_packets;
   int cmp_dif_size;

   int late_cdats;
   int late_cdats_last_sec;
   int late_cdats_last_sec_tally;

   double game_move_dsync; // current value
   double game_move_dsync_max;
   double game_move_dsync_avg_last_sec;
   double game_move_dsync_avg_last_sec_tally;
   double game_move_dsync_avg_last_sec_count;

   // bandwidth counters and tallies

   // used to add up until frame end
   int tx_current_bytes_for_this_frame;
   int tx_current_packets_for_this_frame;
   int tx_max_bytes_per_frame;
   int tx_max_packets_per_frame;
   int rx_current_bytes_for_this_frame;
   int rx_current_packets_for_this_frame;
   int rx_max_bytes_per_frame;
   int rx_max_packets_per_frame;

   // used to add up until tally end
   int tx_packets_tally;
   int tx_bytes_tally;
   int tx_max_packets_per_tally;
   int tx_max_bytes_per_tally;
   int rx_packets_tally;
   int rx_bytes_tally;
   int rx_max_packets_per_tally;
   int rx_max_bytes_per_tally;

   // used for display
   int tx_packets_per_tally;
   int tx_bytes_per_tally;
   int rx_packets_per_tally;
   int rx_bytes_per_tally;

   // grand totals
   int tx_total_bytes;
   int tx_total_packets;
   int rx_total_bytes;
   int rx_total_packets;

};


void set_player_start_pos(int p, int cont);
void proc_player_health(int p);
void proc_player_xy_move_test(int p);
void proc_player_xy_move(int p);
void proc_player_paused(int p);
void reset_player_scale_and_rot(int p);
int is_player_riding_rocket(int p);
void proc_player_stuck_in_blocks(int p);
void proc_player_riding_rocket(int p);
void proc_player_bounds_check(int p);
void proc_player_collisions(int p);
int is_player_within_ladder_reach(int p);
int is_player_within_rope_reach(int p);
void proc_player_rope_move(int p);
void proc_player_ladder_move(int p);
void proc_player_rope(int p);
void proc_player_ladder(int p);
void move_players(void);
void draw_player(int p);
void draw_players(void);
void get_players_shape(int p);
int is_player_color_used(int color);
void init_player(int p, int t);
void fill_player_tile(void);



