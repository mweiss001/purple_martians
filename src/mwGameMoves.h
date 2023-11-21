// mwGameMoves.h

#define GAME_MOVES_SIZE 1000000

class mwGameMoves
{
   public:

   mwGameMoves(); // default constructor
   void initialize(void);

   int has_player_acknowledged(int p);
   int arr[GAME_MOVES_SIZE][4];
   int entry_pos = 0;
   int current_pos = 0; // for savegame running only
   void proc(void);
   void add_game_move2(int frame, int type, int data1, int data2);
   void add_game_move(int frame, int type, int data1, int data2);


//   void proc_client_join_game_move(int x);
//   void proc_client_quit_game_move(int x);


   void proc_player_active_game_move(int x);
   void proc_player_inactive_game_move(int x);

   void proc_player_hidden_game_move(int x);



   void save_gm_txt(char *sfname);
   void save_gm_gm(char *sfname);
   void save_gm();
   void blind_save_game_moves(int d);

   int load_gm(int lev);
   int load_gm(const char *sfname);


};
extern mwGameMoves mGameMoves;






