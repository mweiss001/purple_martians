// n_netgame.h

// ------------------------------------------------
// ----------------- netgame ----------------------
// ------------------------------------------------
extern int ima_server;
extern int ima_client;
extern char m_serveraddress[256];
extern int TCP;
extern int zlib_cmp;

#define STATE_SIZE 112128

// server's copies of client states
extern char srv_client_state[8][2][STATE_SIZE];
extern int srv_client_state_frame_num[8][2];

// local client's states
extern char client_state_buffer[STATE_SIZE];  // buffer for building compressed dif from packet pieces
extern int  client_state_buffer_pieces[16];   // to mark packet pieces as received
extern char client_state_base[STATE_SIZE];    // last ack state
extern int  client_state_base_frame_num;      // last ack state mwPS.frame_num
extern char client_state_dif[STATE_SIZE];     // uncompressed dif
extern int  client_state_dif_src;             // uncompressed dif src mwPS.frame_num
extern int  client_state_dif_dst;             // uncompressed dif dst mwPS.frame_num

void game_vars_to_state(char * b);
void state_to_game_vars(char * b);
void get_state_dif(char *a, char *b, char *c, int size);
void apply_state_dif(char *a, char *c, int size);
void reset_states(void);
void reset_client_state(int p); // server only
