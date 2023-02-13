// mwGameMovesArray.h

#define GAME_MOVES_SIZE 1000000

class mwGameMovesArray
{
   public:


   mwGameMovesArray(); // default constructor
   void initialize(void);

   int has_player_acknowledged(int p);


   int game_moves[GAME_MOVES_SIZE][4];
   int game_move_entry_pos = 0;
   int game_move_current_pos = 0; // for savegame running only

   void proc_game_moves_array(void);
   void add_game_move2(int frame, int type, int data1, int data2);
   void add_game_move(int frame, int type, int data1, int data2);
   void proc_player_client_join_game_move(int x);
   void proc_player_client_quit_game_move(int x);
   void proc_player_active_game_move(int x);
   void proc_player_inactive_game_move(int x);

};
extern mwGameMovesArray mwGMA;






