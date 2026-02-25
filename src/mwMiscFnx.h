// mwMiscFnx.h

class mwMiscFnx
{
   public:


   // use this so I do not have to send a char when I want to return one
   char tmp_return_40[40];


   int exit_level_editor_dialog();
   int mw_file_select(const char * title, char * fn, const char * ext, int save);

   void chop_first_x_char(char *str, int n);

   void get_tag_text(char *str, char *res, int show);
   int get_tag_text2(char *str, char *res, char *res1, int show);

   std::string timestamp(const char* format);

//   std::string timestamp2(constexpr std::string_view fmt);


   std::string timestamp_UTC_ISO8601();
   int find_duration(const char* d1, const char* d2);

   void mw_strncpy(char* d, const char* s, int n);


   char* chrms(int time, char* ft);
//   char* chrsi(int num, char* ft);

   char * chrmhd(int time, char* ft);



   int round20(int val);

   void mw_round(int& val, int rnd);

   float roundToDecimalPlaces(float value, int decimalPlaces);



   int enforce_limit(int val, int ll, int ul);
   int check_limit(int val, int ll, int ul);
   void swap_int(int *i1, int* i2);

   float mdw_rnd(float rmin, float rmax);

   std::string generate_muid();


   void set_int_3216(int &I32, int H16, int L16);
   void get_int_3216(int I32, int &H16, int &L16);
   void printBits(size_t const size, void const * const ptr);




   void printBits2(size_t const size, void const * const ptr);


   void gma_to_val(int type, int d1, int d2, int &p, int &c, char * name);
   void val_to_gma(int &type, int &d1, int &d2, int p, int c, char * name);









   float get_sproingy_jump_height(int num);
   int get_sp(float jh);

   int get_block_range(const char *txt, int *x1, int *y1, int *x2, int *y2, int type);
   int getxy(const char *txt, int obj_type, int sub_type, int num);
   int get_item(int obj_type, int sub_type, int num);


   void draw_block_non_default_flags(int tn, int x, int y);
   void crosshairs_full(int x, int y, int color, int line_width);
   void titlex(const char *txt, int tc, int fc, int x1, int x2, int y);
   void rectangle_with_diagonal_lines(float x1, float y1, float x2, float y2, int spacing, int frame_color, int line_color, int clip_mode);

   void show_cursor(char *f, int cursor_pos, int xpos_c, int ypos, int cursor_color, int restore);

   void edit_server_name(int x, int y);
   void edit_player_name(int x, int y, int p);


   int edit_lift_name(int lift, int step_ty, int bts, char *fst);

};
extern mwMiscFnx mMiscFnx;


