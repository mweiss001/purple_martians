// mwGameMoves.h

#define GAME_MOVES_SIZE 1000000

class mwGameMoves
{
   public:

   mwGameMoves(); // default constructor

private:

   void initialize(void);
public:


   void clear_single(int i);


   void gm_remove(int i);
   void gm_swap(int i, int j);
   void gm_sort(void);

   void remove_doubled_moves(void);


   int has_player_acknowledged(int p);



   void add_game_move2(int frame, int type, int data1, int data2);
   void add_game_move(int frame, int type, int data1, int data2);

   void proc(void);
   void proc_game_move_player_active(int p, int color);
   void proc_game_move_player_inactive(int p, int reason);
   void proc_game_move_player_hidden(int p);
   void proc_game_move_shot_config(int shot, int damg);


   char* get_gm_text2(int gm, int f, int t, int p, int v, char* tmp);
   char* get_gm_text(int gm, char* tmp);

   char* get_save_txt(int num, char *txt);





   void save_gm_file_select(void);
   void save_gm_make_fn(const char* desc, int sendto);
   void save_gm_txt(const char *sfname);




   int  save_gm(const char *sfname, int sendto);
   bool save_gm(const char *fname);





   void find_player_info();

   void check_gma(int x);


   void print_header();

   bool parse_header_line(const char* buf);

   int load_demo_level(int lev);
   int load_gm_file_select(void);
   int load_gm(const char *sfname);


   void add_gm_to_db(void);


   void new_level();















   public:


   int status;
   // 0 - clear
   // 1 - level started
   // 2 - demo loaded
   // 9 - client game (partial data)

   int arr[GAME_MOVES_SIZE][4];

   int entry_pos = 0;
   // marks the insert position of the array
   // also the count of used rows in the array
   // when status is demo loaded, marks the last row


   int current_pos = 0;
   // when status is demo loaded, marks the current playback pos



   // header variables
   string   HEADER_create_timestamp;
   string   HEADER_modify_timestamp;
   string   HEADER_muid;

//   uint64_t HEADER_muid;


   int      HEADER_version;
   int      HEADER_level;
   int      HEADER_last_frame;
   int      HEADER_num_entries;




   char last_loaded_gm_filename[256];


   int autosave_game_on_level_done = 0;
   int autosave_game_on_level_quit = 0;
   int server_send_files_to_clients = 0;




   // tmp variable for functions in this class to have something to return
   char tmp[256];


};
extern mwGameMoves mGameMoves;






