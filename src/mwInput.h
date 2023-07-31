// mwInput.h

class mwInput
{
   public:

   mwInput(); // default constructor
   void initialize(void);

   void proc_input_events(ALLEGRO_EVENT ev);
   void proc_keys_held(void);

   void serial_key_check(int key);

   void function_key_check(void);


   int getJoystickNum(ALLEGRO_JOYSTICK* joy);
   int get_scan_code_from_joystick(int joy, int button, int num);

   int is_key_used(int k);

   int my_readkey(void);
   void my_readkey3(int x, int y, int tc, int bts, int num, int num_lines);
   void my_readkey2(int x, int y, int tc, int bts, int num);


   void get_all_keys(int x, int y, int tc, int bts);
   void test_keys(int x, int sy);
   void set_controls_to_custom_sets(int s);


   int function_key_fullscreen;
   int function_key_text_double;
   int function_key_debug_overlay;
   int function_key_fakekey;
   int function_key_printscreen;
   int function_key_zoom_in;
   int function_key_zoom_out;
   int function_key_force_save;
   int function_key_init_background;
   int function_key_speed_inc;
   int function_key_speed_dec;


   int SHFT(void);
   int CTRL(void);

   void tsw(void);
   void m_err(const char * err_msg);

   ALLEGRO_JOYSTICK *joy0 = NULL;
   ALLEGRO_JOYSTICK *joy1 = NULL;

   // serial key check
   char skc[64];
   int skc_index;

   bool key[ALLEGRO_KEY_MAX][4];
   int key_pressed_ASCII;

   int mouse_x;
   int mouse_y;
   int mouse_z;
   int mouse_dx;
   int mouse_dy;
   int mouse_dz;
   bool mouse_b[5][4];

   const char *key_names[256] =
   {
     (char*) "(none)",     (char*) "A",         (char*) "B",         (char*) "C",
     (char*) "D",          (char*) "E",         (char*) "F",         (char*) "G",
     (char*) "H",          (char*) "I",         (char*) "J",         (char*) "K",
     (char*) "L",          (char*) "M",         (char*) "N",         (char*) "O",
     (char*) "P",          (char*) "Q",         (char*) "R",         (char*) "S",
     (char*) "T",          (char*) "U",         (char*) "V",         (char*) "W",
     (char*) "X",          (char*) "Y",         (char*) "Z",         (char*) "0",
     (char*) "1",          (char*) "2",         (char*) "3",         (char*) "4",
     (char*) "5",          (char*) "6",         (char*) "7",         (char*) "8",
     (char*) "9",          (char*) "PAD 0",     (char*) "PAD 1",     (char*) "PAD 2",
     (char*) "PAD 3",      (char*) "PAD 4",     (char*) "PAD 5",     (char*) "PAD 6",
     (char*) "PAD 7",      (char*) "PAD 8",     (char*) "PAD 9",     (char*) "F1",
     (char*) "F2",         (char*) "F3",        (char*) "F4",        (char*) "F5",
     (char*) "F6",         (char*) "F7",        (char*) "F8",        (char*) "F9",
     (char*) "F10",        (char*) "F11",       (char*) "F12",       (char*) "ESCAPE",
     (char*) "KEY60",      (char*) "KEY61",     (char*) "KEY62",     (char*) "BACKSPACE",
     (char*) "TAB",        (char*) "KEY65",     (char*) "KEY66",     (char*) "ENTER",
     (char*) "KEY68",      (char*) "KEY69",     (char*) "BACKSLASH", (char*) "KEY71",
     (char*) "KEY72",      (char*) "KEY73",     (char*) "KEY74",     (char*) "SPACE",
     (char*) "INSERT",     (char*) "DELETE",    (char*) "HOME",      (char*) "END",
     (char*) "PGUP",       (char*) "PGDN",      (char*) "LEFT",      (char*) "RIGHT",
     (char*) "UP",         (char*) "DOWN",      (char*) "PAD /",     (char*) "PAD *",
     (char*) "PAD -",      (char*) "PAD +",     (char*) "PAD DELETE",(char*) "PAD ENTER",
     (char*) "PRINTSCREEN",(char*) "PAUSE",     (char*) "KEY94",     (char*) "KEY95",
     (char*) "KEY96",      (char*) "KEY97",     (char*) "KEY98",     (char*) "KEY99",
     (char*) "KEY100",     (char*) "KEY101",    (char*) "KEY102",    (char*) "PAD =",
     (char*) "KEY104",     (char*) "KEY105",    (char*) "KEY106",    (char*) "KEY107",
     (char*) "KEY108",     (char*) "KEY109",    (char*) "KEY110",    (char*) "KEY111",
     (char*) "KEY112",     (char*) "KEY113",    (char*) "KEY114",    (char*) "KEY115",
     (char*) "KEY116",     (char*) "KEY117",    (char*) "KEY118",    (char*) "KEY119",
     (char*) "KEY120",     (char*) "KEY121",    (char*) "KEY122",    (char*) "KEY123",
     (char*) "KEY124",     (char*) "KEY125",    (char*) "KEY126",    (char*) "KEY127",

      // joystick equivalents
      // 128-
             (char*) "joy1-up", (char*) "joy1-down",(char*) "joy1-left",(char*) "joy1-right",
             (char*) "joy1-b0", (char*) "joy1-b1",  (char*) "joy1-b2",  (char*) "joy1-b3",
             (char*) "joy1-b4", (char*) "joy1-b5",  (char*) "joy1-b6",  (char*) "joy1-b7",
             (char*) "joy1-b8", (char*) "joy1-b9",  (char*) "joy1-b10", (char*) "joy1-b11",
             (char*) "joy1-b12",(char*) "joy1-b13", (char*) "joy1-b14", (char*) "joy1-b15",
      // 148-
             (char*) "joy2-up", (char*) "joy2-down",(char*) "joy2-left",(char*) "joy2-right",
             (char*) "joy2-b0", (char*) "joy2-b1",  (char*) "joy2-b2",  (char*) "joy2-b3",
             (char*) "joy2-b4", (char*) "joy2-b5",  (char*) "joy2-b6",  (char*) "joy2-b7",
             (char*) "joy2-b8", (char*) "joy2-b9",  (char*) "joy2-b10", (char*) "joy2-b11",
             (char*) "joy2-b12",(char*) "joy2-b13", (char*) "joy2-b14", (char*) "joy2-b15",

   //  (char*) "KEY128",    (char*) "KEY129",    (char*) "KEY130",    (char*) "KEY131",
   //  (char*) "KEY132",    (char*) "KEY133",    (char*) "KEY134",    (char*) "KEY135",
   //  (char*) "KEY136",    (char*) "KEY137",    (char*) "KEY138",    (char*) "KEY139",
   //  (char*) "KEY140",    (char*) "KEY141",    (char*) "KEY142",    (char*) "KEY143",
   //  (char*) "KEY144",    (char*) "KEY145",    (char*) "KEY146",    (char*) "KEY147",
   //
   //  (char*) "KEY148",    (char*) "KEY149",    (char*) "KEY150",    (char*) "KEY151",
   //  (char*) "KEY152",    (char*) "KEY153",    (char*) "KEY154",    (char*) "KEY155",
   //  (char*) "KEY156",    (char*) "KEY157",    (char*) "KEY158",    (char*) "KEY159",
   //  (char*) "KEY160",    (char*) "KEY161",    (char*) "KEY162",    (char*) "KEY163",
   //  (char*) "KEY164",    (char*) "KEY165",    (char*) "KEY166",    (char*) "KEY167",

     (char*) "KEY168",    (char*) "KEY169",    (char*) "KEY170",    (char*) "KEY171",
     (char*) "KEY172",    (char*) "KEY173",    (char*) "KEY174",    (char*) "KEY175",
     (char*) "KEY176",    (char*) "KEY177",    (char*) "KEY178",    (char*) "KEY179",
     (char*) "KEY180",    (char*) "KEY181",    (char*) "KEY182",    (char*) "KEY183",
     (char*) "KEY184",    (char*) "KEY185",    (char*) "KEY186",    (char*) "KEY187",
     (char*) "KEY188",    (char*) "KEY189",    (char*) "KEY190",    (char*) "KEY191",
     (char*) "KEY192",    (char*) "KEY193",    (char*) "KEY194",    (char*) "KEY195",
     (char*) "KEY196",    (char*) "KEY197",    (char*) "KEY198",    (char*) "KEY199",
     (char*) "KEY200",    (char*) "KEY201",    (char*) "KEY202",    (char*) "KEY203",
     (char*) "KEY204",    (char*) "KEY205",    (char*) "KEY206",    (char*) "KEY207",
     (char*) "KEY208",    (char*) "KEY209",    (char*) "KEY210",    (char*) "KEY211",
     (char*) "KEY212",    (char*) "KEY213",    (char*) "KEY214",    (char*) "LSHIFT",
     (char*) "RSHIFT",    (char*) "LCTRL",     (char*) "RCTRL",     (char*) "ALT",
     (char*) "ALTGR",     (char*) "LWIN",      (char*) "RWIN",      (char*) "MENU",
     (char*) "SCROLLLOCK",(char*) "NUMLOCK",   (char*) "CAPSLOCK"
   };


};
extern mwInput mInput;

