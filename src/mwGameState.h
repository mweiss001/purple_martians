// mwGameState.h

class mwGameState
{
   public:

   int level_done_mode;
   int level_done_timer;
   int level_done_x;
   int level_done_y;
   int level_done_player;
   int level_done_frame;
   int level_done_next_level;

   int gate_enter_mode;
   int gate_enter_timer;
   int gate_enter_item;

   int player_vs_player_shots;
   int player_vs_player_shot_damage;
   int player_vs_self_shots;

   int server_force_fakekey;
   int server_force_client_offset;
   float client_chase_offset;


};
extern mwGameState mGameState;


