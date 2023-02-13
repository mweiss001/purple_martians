// z_file.h

int load_level_prompt();
int save_level_prompt();
void save_sprit(void);
void load_sprit(void);
void save_mW(void);
void load_mW(void);
int load_tiles(void);
void zero_level_data(void);
void level_check(void);
int load_level(int level_to_load, int load_only, int fail_silently);
void save_level(int level_to_save);
int mw_file_select(const char * title, char * fn, const char * ext, int save);
char* cmtos(int cm);
void save_gm_txt(char *sfname);
void save_gm_gm(char *sfname);
void save_gm();
void blind_save_game_moves(int d);
int load_gm(const char *sfname);
