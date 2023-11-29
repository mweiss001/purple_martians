// mwGameMoves.h

#define GAME_MOVES_SIZE 1000000

class mwGameMoves
{
   public:

   mwGameMoves(); // default constructor
   void initialize(void);

   void clear_single(int i);

   int has_player_acknowledged(int p);
   int arr[GAME_MOVES_SIZE][4];
   int entry_pos = 0;
   int current_pos = 0; // for savegame running only
   void proc(void);
   void add_game_move2(int frame, int type, int data1, int data2);
   void add_game_move(int frame, int type, int data1, int data2);

   void proc_player_active_game_move(int x);
   void proc_player_inactive_game_move(int x);

   void proc_player_hidden_game_move(int x);

   int autosave_game_on_level_done = 0;
   int autosave_game_on_level_quit = 0;
   int autosave_game_on_program_exit = 0;


   char* cmtos(int cm, char* tmp);

   void save_gm_txt(char *sfname);
   void save_gm(char *sfname);
   void save_gm_file_select();
   void autosave_gm(int d);

   int load_gm(int lev);
   int load_gm(const char *sfname);


};
extern mwGameMoves mGameMoves;






