// z_control.cpp
#include "pm.h"
#include "z_log.h"
#include "z_settings.h"
#include "z_player.h"
#include "n_netgame.h"

char *key_names[] =
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

int getJoystickNum(ALLEGRO_JOYSTICK* joy) // Thanks Edgar Reynaldo!
{
   for (int i=0; i<al_get_num_joysticks(); ++i)
   {
      if (joy == al_get_joystick(i)) {return i;}
   }
   return -1;
}

int get_scan_code_from_joystick(int joy, int button, int num)
{
   int ret = 0;
   int base = 128;
   if (joy == 0) base = 128;
   if (joy == 1) base = 148;
   if (button) ret = num + 4;
   return base + ret;
}

int SHFT(void)
{
   if ((key[ALLEGRO_KEY_LSHIFT][0]) || (key[ALLEGRO_KEY_RSHIFT][0])) return 1;
   else return 0;
}

int CTRL(void)
{
   if ((key[ALLEGRO_KEY_LCTRL][0]) || (key[ALLEGRO_KEY_RCTRL][0])) return 1;
   else return 0;
}

int my_readkey(int x, int y, int tc, int bts, int num) // used only to set new game control key or joystick bindings
{
   int quit = 0, ret = 0;

   redraw_all_controls(x, y, bts, tc, 0, num);
   al_flip_display();

   while (!quit)
   {
      while (!al_is_event_queue_empty(event_queue))
      {
         ALLEGRO_EVENT ev;
         if (al_get_next_event(event_queue, &ev))
         {
             if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) fast_exit(0);
             if (ev.type == ALLEGRO_EVENT_KEY_UP)
             {
                ret = ev.keyboard.keycode;
                if (ret != ALLEGRO_KEY_ENTER) quit = 1;
             }
             if (ev.type == ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN)
             {
                int jy = getJoystickNum(ev.joystick.id);
                ret = get_scan_code_from_joystick(jy, 1, ev.joystick.button);
                quit = 1;
             }
             if (ev.type == ALLEGRO_EVENT_JOYSTICK_AXIS)
             {
                int jy = getJoystickNum(ev.joystick.id);
                int jo = 0; // offset
                if (jy == 0) jo = 0;
                if (jy == 1) jo = 20;

                int   ax  = ev.joystick.axis;
                float pos = ev.joystick.pos;
                if (ax == 0) // x axis
                {
                   if (pos > 0) { ret = 131 + jo; quit = 1; }
                   if (pos < 0) { ret = 130 + jo; quit = 1; }
                }
                if (ax == 1) // y axis
                {
                   if (pos > 0) { ret = 129 + jo; quit = 1;  }
                   if (pos < 0) { ret = 128 + jo; quit = 1;  }
                }
             }
         }
      }
   }
   clear_keys();
   return ret;
}





void clear_keys(void)
{
   for (int k = ALLEGRO_KEY_A; k < ALLEGRO_KEY_MAX; k++)
      for (int i=0; i<4; i++)
         key[k][i] = 0; // clear my key array
}






void get_all_keys(int x, int y, int tc, int bts) // prompts for all seven keys
{
   for (int key_sel = 2; key_sel < 9; key_sel++)  // iterate the keys
   {
      switch (key_sel)
      {
         case  2: players1[0].up_key    =  my_readkey(x,y,tc,bts,0); break;
         case  3: players1[0].down_key  =  my_readkey(x,y,tc,bts,1); break;
         case  4: players1[0].left_key  =  my_readkey(x,y,tc,bts,2); break;
         case  5: players1[0].right_key =  my_readkey(x,y,tc,bts,3); break;
         case  6: players1[0].jump_key  =  my_readkey(x,y,tc,bts,4); break;
         case  7: players1[0].fire_key  =  my_readkey(x,y,tc,bts,5); break;
         case  8: players1[0].menu_key  =  my_readkey(x,y,tc,bts,6); break;
      }
   }
   save_config();
}


void test_keys(int x, int sy)
{
   ALLEGRO_COLOR c1 = palette_color[15];
   ALLEGRO_COLOR c2 = palette_color[10];
   ALLEGRO_COLOR c3 = palette_color[14];

   int quit = 0;
   while (!quit)
   {
      int y = sy;

      al_draw_filled_rectangle(x-190, y, x+190, y+170, palette_color[0]); // erase background

      al_draw_text(font, c1, x, y+=8, ALLEGRO_ALIGN_CENTRE, "----------------------------------------" );
      al_draw_text(font, c1, x, y+=8, ALLEGRO_ALIGN_CENTRE, "Test your controller setup here.");
      al_draw_text(font, c3, x, y+=8, ALLEGRO_ALIGN_CENTRE, "(F11 to quit)");
      al_draw_text(font, c1, x, y+=8, ALLEGRO_ALIGN_CENTRE, "----------------------------------------");
      al_draw_text(font, c1, x, y+=8, ALLEGRO_ALIGN_CENTRE, "Test how pressing multiple");
      al_draw_text(font, c1, x, y+=8, ALLEGRO_ALIGN_CENTRE, "controls affects other controls.");
      al_draw_text(font, c1, x, y+=8, ALLEGRO_ALIGN_CENTRE, "----------------------------------------");
      al_draw_text(font, c1, x, y+=8, ALLEGRO_ALIGN_CENTRE, "Keyboards widely differ in the number");
      al_draw_text(font, c1, x, y+=8, ALLEGRO_ALIGN_CENTRE, "keys that can be detected at one time.");
      al_draw_text(font, c1, x, y+=8, ALLEGRO_ALIGN_CENTRE, "----------------------------------------" );
      y +=24;
      if (players[0].up) al_draw_text(font, c2, x, y, ALLEGRO_ALIGN_CENTRE, "UP" );
      y +=8;
      if (players[0].down) al_draw_text(font, c2, x, y, ALLEGRO_ALIGN_CENTRE, "DOWN" );
      y +=8;
      if (players[0].left) al_draw_text(font, c2, x, y, ALLEGRO_ALIGN_CENTRE, "LEFT" );
      y +=8;
      if (players[0].right) al_draw_text(font, c2, x, y, ALLEGRO_ALIGN_CENTRE, "RIGHT");
      y +=8;
      if (players[0].jump) al_draw_text(font, c2, x, y, ALLEGRO_ALIGN_CENTRE, "JUMP" );
      y +=8;
      if (players[0].fire) al_draw_text(font, c2, x, y, ALLEGRO_ALIGN_CENTRE, "FIRE" );
      y +=8;
      if (players[0].menu) al_draw_text(font, c2, x, y, ALLEGRO_ALIGN_CENTRE, "MENU" );

      al_flip_display();
      proc_controllers();
      if (key[ALLEGRO_KEY_F11][0]) quit = 1;
   }
}

void set_start_level(int s)
{
   start_level = s;
   if (start_level < 1) start_level = 1;
   if (start_level > 399) start_level = 399;
   sprintf(global_string[7][2], "Start Level (%d)", start_level);
   resume_allowed = 0;
   save_config();
}

void set_player_color(int p, int c)
{
   players[p].color = c;
   if (players[p].color < 1) players[p].color = 1;
   if (players[p].color > 15) players[p].color = 15;
}
void set_speed(void)
{
   al_set_timer_speed(fps_timer, 1/(float)frame_speed);
}

void set_controls_to_custom_sets(int s)
{
   if (s == 1) // set all to joy1
   {
      players1[0].up_key = 128;
      players1[0].down_key = 129;
      players1[0].right_key = 131;
      players1[0].left_key = 130;
      players1[0].jump_key = 133;
      players1[0].fire_key = 132;
      players1[0].menu_key = 135;
   }
   if (s == 2) // set all to joy2
   {
      players1[0].up_key = 148;
      players1[0].down_key = 149;
      players1[0].right_key = 151;
      players1[0].left_key = 150;
      players1[0].jump_key = 153;
      players1[0].fire_key = 152;
      players1[0].menu_key = 155;
   }
   if (s == 3) // set all to arrows
   {
      players1[0].up_key = 84;
      players1[0].down_key = 85;
      players1[0].right_key = 83;
      players1[0].left_key = 82;
      players1[0].jump_key = 75;
      players1[0].fire_key = 3;
      players1[0].menu_key = 59;
   }
   if (s == 4) // set all to IJKL
   {
      players1[0].up_key = 9;
      players1[0].down_key = 11;
      players1[0].right_key = 12;
      players1[0].left_key = 10;
      players1[0].jump_key = 75;
      players1[0].fire_key = 3;
      players1[0].menu_key = 59;
   }
}


void clear_controls(int p)
{
   players[p].left  = 0;
   players[p].right = 0;
   players[p].up    = 0;
   players[p].down  = 0;
   players[p].jump  = 0;
   players[p].fire  = 0;
   players[p].menu  = 0;
}

void set_controls_from_comp_move(int p, int comp_move)
{
   clear_controls(p);
   if (comp_move & PM_COMPMOVE_LEFT)  players[p].left  = 1;
   if (comp_move & PM_COMPMOVE_RIGHT) players[p].right = 1;
   if (comp_move & PM_COMPMOVE_UP)    players[p].up    = 1;
   if (comp_move & PM_COMPMOVE_DOWN)  players[p].down  = 1;
   if (comp_move & PM_COMPMOVE_JUMP)  players[p].jump  = 1;
   if (comp_move & PM_COMPMOVE_FIRE)  players[p].fire  = 1;
   if (comp_move & PM_COMPMOVE_MENU)  players[p].menu  = 1;
}

void set_comp_move_from_player_key_check(int p) // but don't set controls !!!
{
   int cm = 0;
   if (key[players1[p].left_key][0])  cm |= PM_COMPMOVE_LEFT;
   if (key[players1[p].right_key][0]) cm |= PM_COMPMOVE_RIGHT;
   if (key[players1[p].up_key][0])    cm |= PM_COMPMOVE_UP;
   if (key[players1[p].down_key][0])  cm |= PM_COMPMOVE_DOWN;
   if (key[players1[p].jump_key][0])  cm |= PM_COMPMOVE_JUMP;
   if (key[players1[p].fire_key][0])  cm |= PM_COMPMOVE_FIRE;
   if (key[players1[p].menu_key][0])  cm |= PM_COMPMOVE_MENU;
   if (key[ALLEGRO_KEY_ESCAPE][0])    cm |= PM_COMPMOVE_MENU;
   players1[p].comp_move = cm;
}

void set_controls_from_player_key_check(int p) // used only in menu
{
   if (key[players1[p].left_key][0])  players[p].left  = 1;
   if (key[players1[p].right_key][0]) players[p].right = 1;
   if (key[players1[p].up_key][0])    players[p].up    = 1;
   if (key[players1[p].down_key][0])  players[p].down  = 1;
   if (key[players1[p].jump_key][0])  players[p].jump  = 1;
   if (key[players1[p].fire_key][0])  players[p].fire  = 1;
   if (key[players1[p].menu_key][0])  players[p].menu  = 1;
   if (key[ALLEGRO_KEY_ESCAPE][0])    players[p].menu  = 1;
}

int get_comp_move_from_players_controls(int p) // only used to test
{
   int cm = 0;
   if (players[p].left)  cm |= PM_COMPMOVE_LEFT;
   if (players[p].right) cm |= PM_COMPMOVE_RIGHT;
   if (players[p].up)    cm |= PM_COMPMOVE_UP;
   if (players[p].down)  cm |= PM_COMPMOVE_DOWN;
   if (players[p].jump)  cm |= PM_COMPMOVE_JUMP;
   if (players[p].fire)  cm |= PM_COMPMOVE_FIRE;
   if (players[p].menu)  cm |= PM_COMPMOVE_MENU;
   return cm;
}

void function_key_check(void)
{
   if ((level_editor_running) || (program_state == 11))
   {
      if (key[ALLEGRO_KEY_F1][3])
      {
         players1[active_local_player].fake_keypress_mode =! players1[active_local_player].fake_keypress_mode;
         printf("fake keypress mode:%d\n", players1[active_local_player].fake_keypress_mode);
      }

      if (key[ALLEGRO_KEY_F4][3])
      {
         if (autosave_game_on_game_exit) blind_save_game_moves(3);
         if (autosave_log_on_program_exit) save_log_file();
      }

      if (key[ALLEGRO_KEY_F5][0]) set_scale_factor(scale_factor * .90, 0);
      if (key[ALLEGRO_KEY_F6][0]) set_scale_factor(scale_factor * 1.1, 0);
      if ((key[ALLEGRO_KEY_F5][0]) && (key[ALLEGRO_KEY_F6][0])) set_scale_factor(1, 1);



      if (!speed_control_lock)
      {
         if ((!ima_client) && (!ima_server)) // only adjust speed if not in netgame
         {
            if (key[ALLEGRO_KEY_F7][2])
            {
               if (SHFT() && CTRL()) frame_speed -=1000;
               else if (SHFT())      frame_speed -=100;
               else if (CTRL())      frame_speed -=20;
               else                  frame_speed -=1;
               if (frame_speed < 5)  frame_speed =5;
               set_speed();
            }
            if (key[ALLEGRO_KEY_F8][2])
            {
               if (SHFT() && CTRL()) frame_speed +=1000;
               else if (SHFT())      frame_speed +=100;
               else if (CTRL())      frame_speed +=20;
               else                  frame_speed += 1;
               if (frame_speed > 100000) frame_speed = 100000;
               set_speed();
            }
         }
         if ((key[ALLEGRO_KEY_F7][0]) && (key[ALLEGRO_KEY_F7][1]) && (key[ALLEGRO_KEY_F8][0]) && (key[ALLEGRO_KEY_F8][1]))
         {
            frame_speed = 40;
            set_speed();
         }
      }



      if (key[ALLEGRO_KEY_F9][2]);

      if (key[ALLEGRO_KEY_F10][2])
      {
         if (++show_debug_overlay > 3) show_debug_overlay = 0;

//
//         if (show_debug_overlay)  show_debug_overlay = 0;
//         else
//         {
//            #ifdef RELEASE
//            if (SHFT() && CTRL())
//            #endif
//            show_debug_overlay = 1;
//         }
//


      }
   } // end of if not game exit

   if (key[ALLEGRO_KEY_F11][2] && SHFT()) init_level_background(0);

   if (key[ALLEGRO_KEY_UP][   2]) pct_y--;
   if (key[ALLEGRO_KEY_DOWN][ 2]) pct_y++;
   if (key[ALLEGRO_KEY_LEFT][ 2]) pct_x--;
   if (key[ALLEGRO_KEY_RIGHT][2]) pct_x++;

   if (key[ALLEGRO_KEY_F12][2])
   {
      if (SHFT() && CTRL()) cycle_display_transform();
      else
      {
         if (fullscreen) proc_display_change_fromfs();
         else            proc_display_change_tofs();
      }
   }


   if (key[ALLEGRO_KEY_PRINTSCREEN][2])
   {
      ALLEGRO_BITMAP *ss_bmp = al_create_bitmap(disp_w_curr, disp_h_curr);
      al_set_target_bitmap(ss_bmp);
      al_draw_bitmap(al_get_backbuffer(display), 0, 0, 0);

      al_make_directory("screenshots"); // create if not already created

      char filename[80];
      struct tm *timenow;
      time_t now = time(NULL);
      timenow = localtime(&now);
      strftime(filename, sizeof(filename), "screenshots\\%Y%m%d%H%M%S.bmp", timenow);
      printf("saved: %s\n", filename);
      al_save_bitmap(filename, ss_bmp);
      al_destroy_bitmap(ss_bmp);
   }
}


void rungame_key_check(int p)
{
   if (key[ALLEGRO_KEY_0][0]) active_local_player = 0;
   if (key[ALLEGRO_KEY_1][0]) active_local_player = 1;
   if (key[ALLEGRO_KEY_2][0]) active_local_player = 2;
   if (key[ALLEGRO_KEY_3][0]) active_local_player = 3;
   if (key[ALLEGRO_KEY_4][0]) active_local_player = 4;
   if (key[ALLEGRO_KEY_5][0]) active_local_player = 5;
   if (key[ALLEGRO_KEY_6][0]) active_local_player = 6;
   if (key[ALLEGRO_KEY_7][0]) active_local_player = 7;

   // dont let alp be an inactive player
   while (!players[active_local_player].active) // if alp not active
      if (++active_local_player > 7) active_local_player = 0;

   // if games_moves doesn't end with level_done kill it after 4 seconds
   if (frame_num > demo_mode_last_frame + 160) new_program_state = 1;

   if ((key[ALLEGRO_KEY_ESCAPE][0]) || (key[ALLEGRO_KEY_ENTER][0]) || (key[ALLEGRO_KEY_SPACE][0]))
   {
      demo_mode_on = 0;

      // set all players inactive
      for (int p=0; p<NUM_PLAYERS; p++) players[p].active = 0;

      // except for local player
      players[0].active = 1;
      load_config();

      while (key[ALLEGRO_KEY_ESCAPE][0]) proc_controllers();

      new_program_state = 1;
      if (old_program_state == 2) old_program_state = older_program_state; // don't send back to demo mode

   }
}


void serial_key_check(int key)
{
   skc[skc_index] = key;
   skc_index++;
   if (skc_index > 63) skc_index = 0;
   skc[skc_index] = 0;

//   printf("%s\n", skc);

   char tst[16];
   int tl = 0;

   sprintf(tst, "test");
   tl = strlen(tst);
   if (skc_index > tl-1)
   {
      if (memcmp((skc + skc_index-tl), tst, tl) == 0)
         printf("test matched!!!\n");

   }


   sprintf(tst, "bmsgon");
   tl = strlen(tst);
   if (skc_index > tl-1)
   {
      if (memcmp((skc + skc_index-tl), tst, tl) == 0)
          bottom_msg_on = 1;
   }

   sprintf(tst, "bmsgoff");
   tl = strlen(tst);
   if (skc_index > tl-1)
   {
      if (memcmp((skc + skc_index-tl), tst, tl) == 0)
          bottom_msg_on = 0;
   }


   sprintf(tst, "ston");
   tl = strlen(tst);
   if (skc_index > tl-1)
   {
      if (memcmp((skc + skc_index-tl), tst, tl) == 0)
         speed_testing = 1;
   }

   sprintf(tst, "stoff");
   tl = strlen(tst);
   if (skc_index > tl-1)
   {
      if (memcmp((skc + skc_index-tl), tst, tl) == 0)
         speed_testing = 0;

   }

   if (ima_client)
   {
      sprintf(tst, "fakekey");
      tl = strlen(tst);
      if (skc_index > tl-1)
      {
         if (memcmp((skc + skc_index-tl), tst, tl) == 0)
         {
            players1[active_local_player].fake_keypress_mode =! players1[active_local_player].fake_keypress_mode;
            printf("fake keypress mode:%d\n", players1[active_local_player].fake_keypress_mode);
         }
      }
   }

   if (level_editor_running)
   {
      sprintf(tst, "mdwa");
      tl = strlen(tst);
      if (skc_index > tl-1)
      {
         if (memcmp((skc + skc_index-tl), tst, tl) == 0) spline_adjust();
      }

      sprintf(tst, "mdwt");
      tl = strlen(tst);
      if (skc_index > tl-1)
      {
         if (memcmp((skc + skc_index-tl), tst, tl) == 0) spline_test();
      }
   }
}



void add_game_move2(int frame, int type, int data1, int data2)
{
   game_moves[game_move_entry_pos][0] = frame;
   game_moves[game_move_entry_pos][1] = type;
   game_moves[game_move_entry_pos][2] = data1;
   game_moves[game_move_entry_pos][3] = data2;
   game_move_entry_pos++;
}

void add_game_move(int frame, int type, int data1, int data2)
{
   // ----------------------------------------------------------------------------------------
   // if we are in level_done_mode 5, all moves are converted to type 8, level done acknowledge
   // ----------------------------------------------------------------------------------------
   if ((players[0].level_done_mode == 5) && (type == 5) && (data2)) // all move type inputs are acks in this mode
   {
      if (!has_player_acknowledged(data1)) // to prevent multiple acks
         add_game_move2(frame, 8, data1, 0); // type 8 - acknowledge level done
      return; // to exit immediately
   }

   // ----------------------------------------------------------------------------------------
   // if menu key or escape was pressed, handle the quitting of the game for other players too
   // ----------------------------------------------------------------------------------------
   if ((type == 5) && (data2 & PM_COMPMOVE_MENU))  // menu key
   {
      if ((active_local_player == 0) && (players[0].control_method == 0)) // single player mode
      {
         // eat this keypress and pretend it never happened
         new_program_state = 1;

         resume_allowed = 1;
         return; // to exit immediately
      }
      if ((active_local_player == 0) && (players[0].control_method == 3) && (data1 == 0)) // local server quitting
      {
         //printf("local server quitting f:%d\n", frame_num);
         // first set all connected clients to inactive
         for (int p=1; p<NUM_PLAYERS; p++)
            if (players[p].control_method == 2)
            {
               add_game_move2(frame + 2, 4, p, 0);   // type 4 - client quit
               add_game_move2(frame + 10, 2, p, 64); // type 2 - player inactive
            }
         // then set server to inactive in future
         add_game_move2(frame + 10, 2, 0, 64); // type 2 - player inactive
         return; // to exit immediately
      }

      if ((active_local_player == 0) && (players[0].control_method == 3) && (data1 > 0)) // remote client quitting
      {
         //printf("remote client quitting\n");
         // set client inactive
         add_game_move2(frame + 2, 2, data1, 64); // type 2 - player inactive
         return; // to exit immediately
      }
      // everything else
      // printf("everything else\n");
      add_game_move2(frame + 4, 2, data1, 64); // type 2 - player inactive
      return; // to exit immediately
   }

   // ----------------------------------------------------------------------------------------
   // the normal method of simply entering the game move
   // ----------------------------------------------------------------------------------------
   add_game_move2(frame, type, data1, data2);
}



void proc_player_client_join_game_move(int x)
{
   if (ima_server)
   {
      int p = game_moves[x][2];  // player number
      int c = game_moves[x][3];  // color
      players[p].control_method = 2;
      players[p].color = c;
   }
}

void proc_player_client_quit_game_move(int x)
{
   if (ima_server)
   {
      int p = game_moves[x][2];  // player number
      players[p].control_method = 8;
   }
}

void proc_player_active_game_move(int x)
{
   int p            = game_moves[x][2]; // player number
   players[p].color = game_moves[x][3]; // color

   // player was inactive before and just now changes to active
   if (players[p].active == 0)
   {
      set_player_start_pos(p, 0); // set starting position
      players[p].active = 1;
      players1[p].join_frame = frame_num;

      if ((ima_server) || (ima_client))
         if (p != active_local_player) players[p].control_method = 2;

      // if player 0 is file play all added players will be too
      if (players[0].control_method == 1) players[p].control_method = 1;

      set_player_joint_quit_display(p, 1, 60);

      game_event(80, 0, 0, p, 0, 0, 0);

      if (LOG_NET)
      {
         sprintf(msg,"PLAYER:%d became ACTIVE color:%d", p, players[p].color);
         add_log_entry_header(10, p, msg, 1);
      }
   }
}

void proc_player_inactive_game_move(int x)
{
   int p   = game_moves[x][2]; // player number
   int val = game_moves[x][3]; // reason

   players1[p].quit_frame = frame_num;

   // ------------------------------------
   // player never became active
   // ------------------------------------
   if ((players[p].active == 0) && (players[p].control_method == 2))
   {
      players1[p].join_frame = frame_num;
      players1[p].quit_reason = 74;
//      players[p].control_method = 9; // prevent re-use of this player number in this level
      players[p].control_method = 0;
      players1[p].who = 99;
   }

   if (players[p].active)
   {
      if (LOG_NET)
      {
         sprintf(msg,"PLAYER:%d became INACTIVE", p);
         add_log_entry_header(10, p, msg, 1);
      }

      // ------------------------------------
      // player in run demo mode became inactive
      // ------------------------------------
      if (players[p].control_method == 1)
      {
         players[p].active = 0;
         // only quit if no players left active
         int still_active = 0;
         for (int p=0; p<NUM_PLAYERS; p++)
            if (players[p].active) still_active = 1;
         if (!still_active) new_program_state = 1;
      }

      // ------------------------------------
      // local server player quit
      // ------------------------------------
      if (players[p].control_method == 3)
      {
         // printf("Local Server Player Quit :%d\n", frame_num);
         players1[p].quit_reason = 91;

         // set quit reason for all active clients on server
         for (int pp=1; pp<NUM_PLAYERS; pp++)
            if ((players[pp].active) && (players[pp].control_method == 2))
               players1[pp].quit_reason = 91;
         if (LOG_NET) log_ending_stats_server();
         new_program_state = 1;
      }

      // ------------------------------------
      // remote server quit
      // ------------------------------------
      if ((ima_client) && (p == 0))
      {
         // printf("Remote Server Quit :%d\n", frame_num);
         if (val == 64) players1[active_local_player].quit_reason = 92;
         new_program_state = 1;
      }

      // ------------------------------------
      // remote player quit (server only)
      // ------------------------------------
      if (players[p].control_method == 2)
      {
         // printf("Remote Player Quit :%d\n", frame_num);
         players1[p].quit_reason = 93;
         if (LOG_NET) log_ending_stats(p);
         init_player(p, 1);

         reset_client_state(p);

//         players[p].active = 0;
//         players[p].control_method = 9; // prevent re-use of this player number in this level
//         players1[p].who = 99;
      }
      set_player_joint_quit_display(p, 0, 60);
      game_event(81, 0, 0, p, 0, 0, 0);
   }
}




// this function processes all entries in the game_moves array that match current frame_num
void proc_game_moves_array(void)
{
   // search entire range
   int start_index = game_move_entry_pos-1;
   int end_index = 0;

   // reduce search range
   start_index = game_move_current_pos + 100;
   if (start_index > game_move_entry_pos-1) start_index = game_move_entry_pos-1;

   end_index = game_move_current_pos - 100;
   if (end_index < 0) end_index = 0;

   for (int x=start_index; x>=end_index; x--)  // search backwards from start_index to end_index
   {
      if (game_moves[x][0] == frame_num) // found frame number that matches current frame
      {
         if (x > game_move_current_pos) game_move_current_pos = x; // index of last used gm - keep this at the most recent one, never go back
         switch (game_moves[x][1])
         {
            case 1: proc_player_active_game_move(x); break;
            case 2: proc_player_inactive_game_move(x); break;
            case 3: proc_player_client_join_game_move(x); break;
            case 4: proc_player_client_quit_game_move(x); break;
            case 5: set_controls_from_comp_move(game_moves[x][2], game_moves[x][3]); break;
         }
      }
   }
}

void proc_player_input(void)
{
   for (int p=0; p<NUM_PLAYERS; p++)
      if (players[p].active) // cycle all active players
      {
         int cm = players[p].control_method;
         if (cm == 1) rungame_key_check(p); // run game from file
         if ((cm == 0) || (cm == 3) || (cm == 4)) // single player, server, client
         {
            if ((players[0].level_done_mode == 0) || (players[0].level_done_mode == 5)) // only allow player input in these modes
            {
               if (players1[p].fake_keypress_mode) players1[p].comp_move = rand() % 64;
               else set_comp_move_from_player_key_check(p);
               if (players1[p].old_comp_move != players1[p].comp_move)  // player's controls have changed
               {
                  players1[p].old_comp_move = players1[p].comp_move;

                  // in single player, client and server mode, add to game moves array
                  if ((cm == 0) || (cm == 3) || (cm == 4)) add_game_move(frame_num, 5, p, players1[p].comp_move);

                  // in client mode, send cdat packet, and apply move directly to controls
                  if (cm == 4)
                  {
                     Packet("cdat");
                     PacketPut1ByteInt(p);
                     PacketPut4ByteInt(frame_num);
                     PacketPut1ByteInt(players1[p].comp_move);
                     ClientSend(packetbuffer, packetsize);

                     set_controls_from_comp_move(p, players1[p].comp_move);

                     if (players[p].menu) new_program_state = 25;
                     players1[p].client_cdat_packets_tx++;
                     sprintf(msg,"tx cdat - move:%d\n", players1[p].comp_move);
                     if (LOG_NET_cdat) add_log_entry2(35, p, msg);

                  }
               }
            }
         }
      }
}

void proc_controllers(void)
{
   int menu_timer_block=1;
   key[ALLEGRO_KEY_PRINTSCREEN][0] = 0; // hack to make PRINTSCREEN key work properly
   key_pressed_ASCII = 0;

   while (menu_timer_block)
   {
      while (!al_is_event_queue_empty(event_queue))
      {
         ALLEGRO_EVENT ev;
         if (al_get_next_event(event_queue, &ev))
         {
            if ((ev.type == ALLEGRO_EVENT_TIMER) && (ev.timer.source == mnu_timer)) menu_timer_block = 0;
            else proc_events(ev);
         }
      }
   }

   proc_keys_held();
   function_key_check();

   // only do key check for active local player
   clear_controls(active_local_player);
   set_controls_from_player_key_check(active_local_player);
}
