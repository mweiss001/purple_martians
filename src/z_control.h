// z_control.h

void clear_controls(int p);
void set_controls_from_comp_move(int p, int comp_move);
void set_comp_move_from_player_key_check(int p);
void set_controls_from_player_key_check(int p);
void rungame_key_check(int p);
void proc_events(ALLEGRO_EVENT ev);
void proc_player_input(void);

