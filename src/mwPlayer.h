// mwPlayer.h
#ifndef mwPlayers_H
#define mwPlayers_H

struct psyn // synced between server and client
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
   int level_done_frame;
   int level_done_next_level;

   int overworld_last_touched_gate;


   float x, y;        // players position
   float xinc, yinc;  // players speed
   float right_xinc, left_xinc;

   float health;
   float old_health;

   int on_ladder;
   int on_rope;
   int player_ride;

   int carry_item;

   int shape; // index to player_tile
   int color; // used to draw frames and stuff in players color

   float draw_rot;
   float draw_scale;

   int door_draw_rot_num_steps;
   float door_draw_rot;
   float door_draw_rot_inc;

   float door_xinc;
   float door_yinc;

   int marked_door;
   int door_item;
   int door_num_steps;

   int marked_gate;
   int spawn_point_index;
   int left_right; // determines the direction the player is facing


   // flags that indicate a control has been activated or held
   int up, down, left, right, jump, fire, fire_held, menu;

   int shot_wait_counter, request_shot, shot_wait, shot_speed;

   int stat_respawns;
   int stat_shots_fired;
   int stat_enemy_hits;
   int stat_player_hits;
   int stat_self_hits;
   int stat_purple_coins;

   int stat_enemy_exploded;
   int stat_player_exploded;
   int stat_self_exploded;

   int late_cdats;
   int late_cdats_last_sec;

   float client_chase_offset = 0.01;

   int spare_int1;
   int spare_int2;
   int spare_int3;
   int spare_int4;
   int spare_int5;
   int spare_int6;


};

struct ploc // not synced between server and client
{
   int health_display;
   int last_health_adjust;
   int potential_bomb_damage;

   float damage_tally;
   int damage_holdoff;
   int damage_type;

   int up_key, down_key, left_key, right_key, jump_key, fire_key, menu_key;
   int comp_move;

   int fake_keypress_mode;

   int who; // for network id of clients
   char hostname[16];

   int server_last_stak_rx_frame_num; // used by server to see if client is still responding
   int client_last_stdf_rx_frame_num; // used by client to see if server is still responding
   int server_send_dif;               // used by server to determine which frame to send dif

   int rewind; // for client and server to keep track of how many frames rewound and played back


   float old_x, old_y;        // old players position, used to calc client pos_correction
   float client_loc_plr_cor;
   float client_rmt_plr_cor;
   float client_loc_plr_cor_max;
   float client_rmt_plr_cor_max;
   float client_loc_plr_cor_avg;
   float client_rmt_plr_cor_avg;

   int client_base_resets;
   int client_last_dif_applied;

   double ping;
   double ping_avg;

   double dsync;
   double dsync_avg;

   double client_chase_fps;
   double stak_dsync;


   int client_cdat_packets_tx;

   int server_game_move_sync;
   double game_move_dsync;
   double game_move_dsync_avg_last_sec;


   int made_active_holdoff;
   int sync_stabilization_holdoff;

   int join_frame;
   int quit_frame;
   int quit_reason;

   // used only to display in server debug grid
   int num_dif_packets;
   int cmp_dif_size;


   // used only by server remote_control
   int srv_frame_num;
   int srv_stdf_freq;
   int srv_total_game_moves;
   int srv_level;
   float srv_cpu;

   int srv_zlib_cmp;
   int srv_extra_packets_num;
   int srv_extra_packets_size;




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


class mwPlayer
{
   public:

   struct psyn syn[NUM_PLAYERS];
   struct ploc loc[NUM_PLAYERS];
   int active_local_player = 0;

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

   void draw_player_direct_to_screen(int p);

   void draw_player(int p);
   void draw_players(void);
   void set_players_shape(int p);
   int is_player_color_used(int color);
   void init_player(int p, int t);
   void fill_player_tile(void);

   void clear_controls(int p);

   int comp_move_from_players_current_controls(int p);


   void set_controls_from_comp_move(int p, int comp_move);
   void set_comp_move_from_player_key_check(int p);
   void set_controls_from_player_key_check(int p);
   void proc_player_input(void);

};
extern mwPlayer mPlayer;

#endif

