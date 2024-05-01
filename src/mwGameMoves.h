// mwGameMoves.h

#define GAME_MOVES_SIZE 1000000

class mwGameMoves
{
   public:

   mwGameMoves(); // default constructor
   void initialize(void);

   void clear_single(int i);

   void gm_remove(int i);
   void gm_swap(int i, int j);
   void gm_sort(void);

   void remove_doubled_moves(void);

   int has_player_acknowledged(int p);
   int arr[GAME_MOVES_SIZE][4];
   int entry_pos = 0;
   int current_pos = 0; // for savegame running only
   void proc(void);

   void add_game_move2(int frame, int type, int data1, int data2);
   void add_game_move(int frame, int type, int data1, int data2);

   void proc_game_move_player_active(int x);
   void proc_game_move_player_inactive(int x);
   void proc_game_move_player_hidden(int x);
   void proc_game_move_shot_config(int i);

   int autosave_game_on_level_done = 0;
   int autosave_game_on_level_quit = 0;
   int server_send_files_to_clients = 0;

   char* get_gm_text2(int gm, int f, int t, int p, int v, char* tmp);
   char* get_gm_text(int gm, char* tmp);

   char * get_save_txt(int num, char *txt);



   void save_gm_file_select(void);
   void save_gm_make_fn(const char* desc, int sendto);
   int save_gm(const char *sfname, int sendto);
   void save_gm_txt(const char *sfname);

   int load_demo_level(int lev);
   int load_gm(const char *sfname);
   int load_gm_file_select(void);
};
extern mwGameMoves mGameMoves;






