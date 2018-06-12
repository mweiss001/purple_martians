// zcontrol.cpp

#include "pm.h"


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

int my_readkey(void) // used only to set new game control key or joystick bindings
{
   int quit = 0, ret = 0;
   while (!quit)
   {
      al_draw_text(font, palette_color[10], SCREEN_W/2, SCREEN_H/2, ALLEGRO_ALIGN_CENTRE, "...press new key or joystick control...");
      al_flip_display();

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

                int ax = ev.joystick.axis;
                float pos = ev.joystick.pos;
                if (ax == 0) // x axis
                {
                   if (pos > 0)
                   {
                       ret = 131+jo;
                       quit = 1;
                   }
                   if (pos < 0)
                   {
                       ret = 130+jo;
                       quit = 1;
                   }
                }
                if (ax == 1) // y axis
                {
                   if (pos > 0)
                   {
                       ret = 129+jo;
                       quit = 1;
                   }
                   if (pos < 0)
                   {
                       ret = 128+jo;
                       quit = 1;
                   }
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
   for (int k = ALLEGRO_KEY_A; k < ALLEGRO_KEY_MAX; k++) key[k] = 0; // clear my key array
}

void get_all_keys(int p) // prompts for all seven keys
{
   int x = SCREEN_W/2;
   set_key_menu(5, p, 2);


    // iterate the keys
   for (int key_sel = 2; key_sel < 9; key_sel++)
   {
      //erase background
      al_draw_filled_rectangle(x-90, 130, x+90, 200, palette_color[0]) ;

      // redraw all keys
      for (int sk = 2; sk < 9; sk++)
         al_draw_text(font, palette_color[15], x, 111+10*sk, ALLEGRO_ALIGN_CENTER, global_string[5][sk]);

      // highlighted current key
      al_draw_text(font, palette_color[10], x, 111+10*key_sel, ALLEGRO_ALIGN_CENTER, global_string[5][key_sel]);

      switch (key_sel)
      {
         case  2: players1[p].up_key    =  my_readkey(); break;
         case  3: players1[p].down_key  =  my_readkey(); break;
         case  4: players1[p].right_key =  my_readkey(); break;
         case  5: players1[p].left_key  =  my_readkey(); break;
         case  6: players1[p].jump_key  =  my_readkey(); break;
         case  7: players1[p].fire_key  =  my_readkey(); break;
         case  8: players1[p].menu_key  =  my_readkey(); break;
      }
      set_key_menu(5, p, 2);
   }
   save_config();
}


void test_keys(void)
{
   ALLEGRO_COLOR c1 = palette_color[15];
   ALLEGRO_COLOR c2 = palette_color[10];

   int quit = 0;
   while (!quit)
   {
      int x = SCREEN_W/2;
      int y = BORDER_WIDTH + 8;
      al_set_target_backbuffer(display);
      al_clear_to_color(al_map_rgb(0,0,0));

      al_draw_text(font, c1, x, y+=8, ALLEGRO_ALIGN_CENTRE, "----------------------------------------" );
      al_draw_text(font, c1, x, y+=8, ALLEGRO_ALIGN_CENTRE, "Test your controller setup here.");
      al_draw_text(font, c1, x, y+=8, ALLEGRO_ALIGN_CENTRE, "(F11 to quit)");
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

      if (key[ALLEGRO_KEY_F11]) quit = 1;
      al_flip_display();
      proc_controllers();
      al_rest(0.02);
   }
}

void set_start_level(int s)
{
   resume_allowed = 0;
   start_level = s;
   save_config();
}

void set_speed(void)
{
   sprintf(global_string[8][6],"Speed:%2dfps", frame_speed);
   al_set_timer_speed(fps_timer, 1/(float)frame_speed);
}

void clear_controls(int p)
{
   players[p].left = 0;
   players[p].right = 0;
   players[p].up = 0;
   players[p].down = 0;
   players[p].jump = 0;
   players[p].fire = 0;
   players[p].menu = 0;
}

void set_controls_from_comp_move(int g)
{
   int p = game_moves[g][2];
   int t = game_moves[g][3];
   clear_controls(p);
   if (t == 127) { t -= 127; players[p].menu = 1;  }
   if (t > 31)   { t -= 32;  players[p].fire = 1;  }
   if (t > 15)   { t -= 16;  players[p].jump = 1;  }
   if (t > 7)    { t -= 8;   players[p].down = 1;  }
   if (t > 3)    { t -= 4;   players[p].up = 1;    }
   if (t > 1)    { t -= 2;   players[p].right = 1; }
   if (t > 0)    { t -= 1;   players[p].left = 1;  }
}

void set_comp_move_from_player_key_check(int p) // but don't set controls !!!
{
   int cm = 0;
   if (key[players1[p].left_key])  cm += 1;
   if (key[players1[p].right_key]) cm += 2;
   if (key[players1[p].up_key])    cm += 4;
   if (key[players1[p].down_key])  cm += 8;
   if (key[players1[p].jump_key])  cm += 16;
   if (key[players1[p].fire_key])  cm += 32;
   // if menu key ignore everything else and set to 127
   if (key[players1[p].menu_key])  cm = 127;
   if (key[ALLEGRO_KEY_ESCAPE])    cm = 127;
   players1[p].comp_move = cm;
}

void set_controls_from_player_key_check(int p) // used only in menu
{
   if (key[players1[p].up_key])    players[p].up = 1;
   if (key[players1[p].down_key])  players[p].down = 1;
   if (key[players1[p].left_key])  players[p].left = 1;
   if (key[players1[p].right_key]) players[p].right = 1;
   if (key[players1[p].fire_key])  players[p].fire = 1;
   if (key[players1[p].jump_key])  players[p].jump = 1;
   if (key[players1[p].menu_key])  players[p].menu = 1;
   if (key[ALLEGRO_KEY_ESCAPE])    players[p].menu = 1;
}

void function_key_check(void)
{
   if (!game_exit)
   {
      if ((key[ALLEGRO_KEY_F1]) && (!KEY_F1_held))
      {
         KEY_F1_held = 1;


         #ifndef RELEASE
         players1[active_local_player].fake_keypress_mode =! players1[active_local_player].fake_keypress_mode;
         printf("fake keypress mode:%d\n", players1[active_local_player].fake_keypress_mode);
         #endif


//         printf("dif test\n");
//         players[0].PX += al_ftofix(0.0001);
//         Ei[2][9]++;
//         Efi[3][1] += al_ftofix(0.0001);
//         item[4][3]++;
//         itemf[5][0] += al_ftofix(0.0001);
//         lifts[6].fy += al_ftofix(0.0001);
//         l[2][3]++;

//         printf("dif test\n");
//         void show_player_dif(int ba);
//         void show_enemy_dif(int ba);
//         void show_item_dif(int ba);
//         void show_lifts_dif(int ba);
//         show_player_dif(1);
//         show_player_dif(2);
//         show_enemy_dif(1);
//         show_enemy_dif(2);
//         show_item_dif(1);
//         show_item_dif(2);
//         show_lifts_dif(1);
//         show_lifts_dif(2);

      }
      if (!key[ALLEGRO_KEY_F1]) KEY_F1_held = 0;

      if ((key[ALLEGRO_KEY_F2]) && (!KEY_F2_held))
      {
         KEY_F2_held = 1;
         #ifndef RELEASE
         speed_testing =!speed_testing; // remove all speed limiting and force draw each frame
         #endif
      }
      if (!key[ALLEGRO_KEY_F2]) KEY_F2_held = 0;

      if ((key[ALLEGRO_KEY_F3]) && (!KEY_F3_held))
      {
         KEY_F3_held = 1;
         //next_map_size();
      }
      if (!key[ALLEGRO_KEY_F3]) KEY_F3_held = 0;
      if ((key[ALLEGRO_KEY_F4]) && (!KEY_F4_held))
      {
         KEY_F4_held = 1;
         //next_map_mode();
      }
      if (!key[ALLEGRO_KEY_F4]) KEY_F4_held = 0;

      if ((key[ALLEGRO_KEY_F5]) && (!KEY_F5_held))
      {
         KEY_F5_held = 10;
         scale_factor *= .90;
         set_scale_factor(0);
      }
      if (!key[ALLEGRO_KEY_F5]) KEY_F5_held = 0;
      if (key[ALLEGRO_KEY_F5]) if (--KEY_F5_held < 0) KEY_F5_held = 0;
      if ((key[ALLEGRO_KEY_F6]) && (!KEY_F6_held))
      {
         KEY_F6_held = 10;
         scale_factor *= 1.1;
         set_scale_factor(0);
      }
      if (!key[ALLEGRO_KEY_F6]) KEY_F6_held = 0;
      if (key[ALLEGRO_KEY_F6]) if (--KEY_F6_held < 0) KEY_F6_held = 0;

      if ((KEY_F5_held) && (KEY_F6_held))
      {
         KEY_F5_held = 10;
         KEY_F6_held = 10;
         scale_factor = 1.0;
         set_scale_factor(1);
      }

      if ((!ima_client) && (!ima_server)) // only adjust speed if not in netgame
      {
         if ((key[ALLEGRO_KEY_F7]) && (!KEY_F7_held))
         {
            KEY_F7_held = frame_speed/4;
            if ((key[ALLEGRO_KEY_LSHIFT]) || (key[ALLEGRO_KEY_RSHIFT])) frame_speed -=100;
            else if ((key[ALLEGRO_KEY_LCTRL]) || (key[ALLEGRO_KEY_RCTRL])) frame_speed -=20;
            else frame_speed -= 1;
            if (frame_speed < 10) frame_speed = 10;
            set_speed();
         }
         if (!key[ALLEGRO_KEY_F7]) KEY_F7_held = 0;
         if (key[ALLEGRO_KEY_F7]) if (--KEY_F7_held < 0) KEY_F7_held = 0;


         if ((key[ALLEGRO_KEY_F8]) && (!KEY_F8_held))
         {
            KEY_F8_held = frame_speed/4;
            if ((key[ALLEGRO_KEY_LSHIFT]) || (key[ALLEGRO_KEY_RSHIFT])) frame_speed +=100;
            else if ((key[ALLEGRO_KEY_LCTRL]) || (key[ALLEGRO_KEY_RCTRL])) frame_speed +=20;
            else frame_speed += 1;
            if (frame_speed > 10000) frame_speed = 10000;
            set_speed();
         }
         if (!key[ALLEGRO_KEY_F8]) KEY_F8_held = 0;
         if (key[ALLEGRO_KEY_F8]) if (--KEY_F8_held < 0) KEY_F8_held = 0;
      }

      if ((KEY_F7_held) && (KEY_F8_held))
      {
         KEY_F7_held = 10;
         KEY_F8_held = 10;
         frame_speed = 40;
         set_speed();
      }

      if ((key[ALLEGRO_KEY_F9]) && (!KEY_F9_held))
      {
         KEY_F9_held = 1;
      }
      if (!key[ALLEGRO_KEY_F9]) KEY_F9_held = 0;


      if ((key[ALLEGRO_KEY_F10]) && (!KEY_F10_held))
      {
         KEY_F10_held = 1;

         if (show_debug_overlay)  show_debug_overlay = 0;
         else
         {
            #ifdef RELEASE
            if ( ((key[ALLEGRO_KEY_LSHIFT]) || (key[ALLEGRO_KEY_RSHIFT])) &&
               ((key[ALLEGRO_KEY_LCTRL]) || (key[ALLEGRO_KEY_RCTRL])) )
            #endif
               show_debug_overlay = 1;
         }
      }
      if (!key[ALLEGRO_KEY_F10]) KEY_F10_held = 0;
   } // end of if not game exit


      if ((key[ALLEGRO_KEY_F11]) && (!KEY_F11_held))
      {
         KEY_F11_held = 1;
      }
      if (!key[ALLEGRO_KEY_F11]) KEY_F11_held = 0;

   if ((key[ALLEGRO_KEY_F12]) && (!KEY_F12_held))
   {
      KEY_F12_held = 1;
      if ( ((key[ALLEGRO_KEY_LSHIFT]) || (key[ALLEGRO_KEY_RSHIFT])) &&
           ((key[ALLEGRO_KEY_LCTRL]) || (key[ALLEGRO_KEY_RCTRL])) )
      {
         float old_display_transform_double = display_transform_double;
         if (++display_transform_double>3) display_transform_double = 1;
         float new_display_transform_double = display_transform_double;
         float sfa = new_display_transform_double/old_display_transform_double;

         //printf("old_display_transform_double:%f new_display_transform_double:%f sfa:%f \n", old_display_transform_double, new_display_transform_double, sfa);

         //printf("1scale factor:%f\n", scale_factor);
         scale_factor /= sfa;
         //printf("2scale factor:%f\n", scale_factor);

         scale_factor_current = scale_factor;

         set_display_transform();
         set_map_var();

         window_title();
      }
      else
      {
         if (fullscreen) proc_screen_change(SCREEN_W, SCREEN_H, 0, 0, 0);
         else            proc_screen_change(SCREEN_W, SCREEN_H, 0, 0, 1);
      }
   }
   if (!key[ALLEGRO_KEY_F12]) KEY_F12_held = 0;

   if ((key[ALLEGRO_KEY_PRINTSCREEN]) && (!KEY_PRTSCR_held))
   {
      KEY_PRTSCR_held = 1;
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
   if (!key[ALLEGRO_KEY_PRINTSCREEN]) KEY_PRTSCR_held = 0;
}


void rungame_key_check(int p, int ret)
{
   if (key[ALLEGRO_KEY_0]) active_local_player = 0;
   if (key[ALLEGRO_KEY_1]) active_local_player = 1;
   if (key[ALLEGRO_KEY_2]) active_local_player = 2;
   if (key[ALLEGRO_KEY_3]) active_local_player = 3;
   if (key[ALLEGRO_KEY_4]) active_local_player = 4;
   if (key[ALLEGRO_KEY_5]) active_local_player = 5;
   if (key[ALLEGRO_KEY_6]) active_local_player = 6;
   if (key[ALLEGRO_KEY_7]) active_local_player = 7;

   // dont let alp be an inactive player
   while (!players[active_local_player].active) // if alp not active
      if (++active_local_player > 7) active_local_player = 0;

   if (demo_mode_on) // look for a keypress to end demo mode
   {
      if (ret)
      {
         // allow keys 0-7 and F1 to F12; all other keys abort demo mode
         if ( ((ret > 0) && (ret < 27)) || //up to 0
             ((ret > 34) && (ret <47)) ||
             ((ret > 58) && (ret <200)) )
         {
           demo_mode_on = 0;
           game_exit = 1;
           get_config_values();
         }
      }
   }


// not sure about this code, so i removed it to see what would happen
// maybe if games_moves doesn't end with an explicit exit and run game just keeps on going??

//   int last_pc = game_moves[game_move_entry_pos-4][0];
//   if ((last_pc < frame_num - 100) || (key[ALLEGRO_KEY_ESCAPE]))

   if (key[ALLEGRO_KEY_ESCAPE])
   {
      // set all players inactive
      for (int p=0; p<NUM_PLAYERS; p++) players[p].active = 0;

      // except for local player
      players[0].active = 1;
      game_exit = 1;
      get_config_values();
   }
}

void add_game_move(int frame, int type, int data1, int data2)
{
   if (type == 6) // special level done move
   {
      game_moves[game_move_entry_pos][0] = frame;
      game_moves[game_move_entry_pos][1] = 6; // type 6; level done
      game_moves[game_move_entry_pos][2] = 0;
      game_moves[game_move_entry_pos][3] = 0;
      game_move_entry_pos++;
      return; // to exit immediately
   }
   if ((type == 5) && (data2 == 127))        // special client exit move
   {
      game_moves[game_move_entry_pos][0] = frame;
      game_moves[game_move_entry_pos][1] = 1;     // type 1; player state
      game_moves[game_move_entry_pos][2] = data1; // player num
      game_moves[game_move_entry_pos][3] = 64;    // inactive
      game_move_entry_pos++;
      return; // to exit immediately
   }

   game_moves[game_move_entry_pos][0] = frame;
   game_moves[game_move_entry_pos][1] = type;
   game_moves[game_move_entry_pos][2] = data1;
   game_moves[game_move_entry_pos][3] = data2;
   game_move_entry_pos++;

}

void proc_player_state_game_move(int x)
{
   int p = game_moves[x][2];   // player number
   int val = game_moves[x][3]; // color, active, quit reason, this does it all


   // if player was already active - just set color
   if ((players[p].active == 1) && (val > 0) && (val < 16)) players[p].color = val;


   // player becomes active
   if ((players[p].active == 0) && (val > 0) && (val < 16))
   {
      set_player_start_pos(p); // get starting position from start block
      players[p].active = 1;
      players[p].color = val;
      players1[p].join_frame = frame_num;


      if (L_LOGGING_NETPLAY)
      {
         sprintf(msg,"PLAYER:%d became ACTIVE (color:%d)", p, players[p].color);
         add_log_entry_header(10, p, msg, 1);
      }

      if (p == active_local_player)
      {
         if (L_LOGGING_NETPLAY)
         {
            int finish_time = clock();
            int time = finish_time - log_timer;
            sprintf(msg,"Chase and lock done in %dms",time);
            add_log_entry_header(10, p, msg, 1);
         }
      }

      show_player_join_quit_timer = 60;
      show_player_join_quit_player = p;
      show_player_join_quit_jq = 1;

      if ((ima_server) || (ima_client))
         if (p != active_local_player) players[p].control_method = 2;

      // if player 0 is file play all added players will be too
      if (players[0].control_method == 1) players[p].control_method = 1;
   }


   // player becomes inactive
   if (val > 63)
   {
      players1[p].quit_reason = val;
      players1[p].quit_frame = frame_num;


      // player never became active
      if ((players[p].active == 0) && (players[p].control_method == 2))
      {
         sprintf(msg,"PLAYER:%d never became ACTIVE", p);
         add_log_entry_header(10, p, msg, 1);
         players1[p].join_frame = frame_num;
         players1[p].quit_reason = 74;
         players[p].control_method = 9; // prevent re-use of this player number in this level
         players1[p].who = 99;
      }

      if (players[p].active)
      {
         // local player in single player mode became incative
         if (players[p].control_method == 0)
         {
            game_exit = 1;
            resume_allowed = 1;
         }

         // player in run demo mode became inactive
         if (players[p].control_method == 1)
         {
            players[p].active = 0;
            // only quit if no players left active
            int still_active = 0;
            for (int p=0; p<NUM_PLAYERS; p++)
               if (players[p].active) still_active = 1;
            if (!still_active) game_exit = 1;
         }


         if (L_LOGGING_NETPLAY)
         {
            sprintf(msg,"PLAYER:%d became INACTIVE", p);
            add_log_entry_header(10, p, msg, 1);
            sprintf(msg,"Reason: unknown");
            if (players1[p].quit_reason == 64) sprintf(msg,"Reason: menu key pressed");
            if (players1[p].quit_reason == 70) sprintf(msg,"Reason: dropped by server (server sync > 100)");
            if (players1[p].quit_reason == 71) sprintf(msg,"Reason: dropped by server (no sdak received for 100 frames)");
            add_log_entry_header(10, p, msg, 1);
         }

         // local client player quit
         if (players[p].control_method == 4)
         {
            if (val == 64) players1[p].quit_reason = 90;
            game_exit = 1;
            if (L_LOGGING_NETPLAY)
            {
               sprintf(msg,"Local Client(%s) quit the game.",local_hostname);
               add_log_entry_header(10, p, msg, 1);
               log_ending_stats();
            }
         }

         // local server player quit
         if (players[p].control_method == 3)
         {
            if (val == 64) players1[p].quit_reason = 91;
            game_exit = 1;
            if (L_LOGGING_NETPLAY)
            {
               sprintf(msg,"Local Server(%s) quit the game.",local_hostname);
               add_log_entry_header(10, p, msg, 1);
               log_ending_stats_server();
            }
         }

         // remote server player quit
         if ((ima_client) && (p == 0))
         {
            if (val == 64) players1[p].quit_reason = 92;
            game_exit = 1;
            if (L_LOGGING_NETPLAY)
            {
               sprintf(msg,"Remote Server ended the game.");
               add_log_entry_header(10, p, msg, 1);
               log_ending_stats();
            }
         }

         // remote player quit
         if (players[p].control_method == 2)
         {
            players[p].active = 0;
            players[p].control_method = 9; // prevent re-use of this player number in this level
            players1[p].who = 99;

            // only makes sense to show this if not local player
            show_player_join_quit_timer = 60;
            show_player_join_quit_player = p;
            show_player_join_quit_jq = 0;
         }
      } // end of if player active
   }  // end of player becomes inactive
}



void proc_game_move(void)
{
   // this function looks in the game_moves array for an exact frame_num match
   // this only processes system moves, not regular game_moves
   // its likely that an exact frame_num match won't be found and nothing will be done
   // if multiple frame_nums match they all will be processed

   int ll;
   if (players[0].control_method == 1) ll = 0; // run game needs to look back to the start
   else ll = game_move_entry_pos - 100; // every other mode can just look back 100
   if (ll < 0) ll = 0; // don't look back past zero!

   for (int x=game_move_entry_pos; x>ll; x--)  // look back for frame_num
   {
      if (game_moves[x][0] == frame_num)
      {
         switch (game_moves[x][1])
         {
            case 1: proc_player_state_game_move(x); break;
            case 6:  // 'level done'
               if (L_LOGGING)
               {
                  sprintf(msg,"LEVEL %d DONE", play_level);
                  add_log_entry_header(10, 0, msg, 3);
               }
               if ((ima_client) || (ima_server))
               {
                  for (int p=0; p<NUM_PLAYERS; p++)
                     if (players[p].active) players1[p].quit_reason = 80;

                  if (L_LOGGING_NETPLAY)
                  {
                     if (ima_client) log_ending_stats();
                     if (ima_server) log_ending_stats_server();
                  }
               }
               proc_level_done();
            break;
         } // end of switch type
      } // end of frame_num match
   } // end of look back from entry pos
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

void set_controls_from_game_move(int p)
{
   // this will search back from entry position until it finds the first 'move' type
   // entry that matches the player and is not in the future
   int found = 0;
   for (int g=game_move_entry_pos; g>0; g--)  // look back from entry pos
      if ((game_moves[g][1] == 5) && (game_moves[g][2] == p)) // find first that matches type and p
         if (game_moves[g][0] <= frame_num) // check to make sure its not in the future
         {
            set_controls_from_comp_move(g);
            game_move_current_pos = g; // for savegame running only
            g = 0; // break out of loop
            found = 1;
         }
   if (!found) clear_controls(p); // if no match found (no move entry for player in entire game move array)
}

int proc_events(ALLEGRO_EVENT ev, int ret)
{
   if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) fast_exit(0);
   if (ev.type == ALLEGRO_EVENT_MOUSE_WARPED)
   {
      mouse_x = ev.mouse.x / display_transform_double;
      mouse_y = ev.mouse.y / display_transform_double;
   }
   if (ev.type == ALLEGRO_EVENT_MOUSE_AXES)
   {
      mouse_x = ev.mouse.x / display_transform_double;
      mouse_y = ev.mouse.y / display_transform_double;
      mouse_z = ev.mouse.z / display_transform_double;
      mouse_dx = ev.mouse.dx;
      mouse_dy = ev.mouse.dy;
      mouse_dz = ev.mouse.dz;
   }
   if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
   {
      if (ev.mouse.button == 1) mouse_b1 = 1;
      if (ev.mouse.button == 2) mouse_b2 = 1;
      if (ev.mouse.button == 3) mouse_b3 = 1;
      if (ev.mouse.button == 4) mouse_b4 = 1;
   }
   if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
   {
      if (ev.mouse.button == 1) mouse_b1 = 0;
      if (ev.mouse.button == 2) mouse_b2 = 0;
      if (ev.mouse.button == 3) mouse_b3 = 0;
      if (ev.mouse.button == 4) mouse_b4 = 0;
   }
   if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
   {
      int k = ev.keyboard.keycode;
      key[k] = true;
      ret = k;
   }
   if (ev.type == ALLEGRO_EVENT_KEY_UP)
   {
      int k = ev.keyboard.keycode;
      key[k] = false;
      if (k == ALLEGRO_KEY_PRINTSCREEN) key[k] = true; // special exception to make PRINTSCREEN work
   }
   if (ev.type == ALLEGRO_EVENT_KEY_CHAR)
   {
      Key_pressed_ASCII = ev.keyboard.unichar;
      serial_key_check(Key_pressed_ASCII);
   }
   if (ev.type == ALLEGRO_EVENT_JOYSTICK_AXIS)
   {
      int jy = getJoystickNum(ev.joystick.id);
      int jo = 0; // offset
      if (jy == 0) jo = 0;
      if (jy == 1) jo = 20;
      int ax = ev.joystick.axis;
      float pos = ev.joystick.pos;
      if (ax == 0) // x axis
      {
         key[130+jo] = false;
         key[131+jo] = false;
         if (pos > 0) key[131+jo] = true;
         if (pos < 0) key[130+jo] = true;
      }
      if (ax == 1) // y axis
      {
         key[128+jo] = false;
         key[129+jo] = false;
         if (pos > 0) key[129+jo] = true;
         if (pos < 0) key[128+jo] = true;
      }
   }
   if (ev.type == ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN)
   {
      int jy = getJoystickNum(ev.joystick.id);
      int sc = get_scan_code_from_joystick(jy, 1, ev.joystick.button);
      key[sc] = true;
   }
   if (ev.type == ALLEGRO_EVENT_JOYSTICK_BUTTON_UP)
   {
      int jy = getJoystickNum(ev.joystick.id);
      int sc = get_scan_code_from_joystick(jy, 1, ev.joystick.button);
      key[sc] = false;
   }
   return ret;
}


int proc_controllers()
{
   int ret = 0;
   int menu_timer_block = 1;

   key[ALLEGRO_KEY_PRINTSCREEN] = 0; // hack to make PRINTSCREEN key work properly
   Key_pressed_ASCII = 0;

   if (!fullscreen) // detect if window was moved
   {
      int x, y;
      al_get_window_position(display, &x, &y);
      if ((x != disp_x_curr) || (y != disp_y_curr))
      proc_screen_change(disp_w_curr, disp_h_curr, x, y, fullscreen);
   }

   while (menu_timer_block)
   {
      while (!al_is_event_queue_empty(event_queue))
      {
         ALLEGRO_EVENT ev;
         if (al_get_next_event(event_queue, &ev))
         {
            if (ev.type == ALLEGRO_EVENT_TIMER) menu_timer_block = 0;
            if (ev.type == ALLEGRO_EVENT_DISPLAY_RESIZE)
            {
               // check to see if we have more resize events piling up
               ALLEGRO_EVENT ev2;
               while (al_get_next_event(event_queue, &ev2))
               {
                  if (ev2.type == ALLEGRO_EVENT_DISPLAY_RESIZE) ev = ev2;
                  else ret = proc_events(ev2, ret);
               }
               proc_screen_change(ev.display.width, ev.display.height, disp_x_curr, disp_y_curr, fullscreen);
            }
            else ret = proc_events(ev, ret);
         }
      }
      if (game_exit) // if called from menu only do key check for active local player
      {
          clear_controls(active_local_player);
          set_controls_from_player_key_check(active_local_player);
          function_key_check();
      }
      else // game is in progress
      {
         menu_timer_block = 0;
         function_key_check();
         for (int p=0; p<NUM_PLAYERS; p++)
            if (players[p].active) // cycle all active players
            {
               if (players[p].control_method == 0) // local single player control
               {
                  if (level_done) add_game_move(frame_num, 6, 0, 0); // insert level done into game move
                  set_comp_move_from_player_key_check(p); // but don't set controls !!!
                  if (players1[p].comp_move != players1[p].old_comp_move)
                  {
                     players1[p].old_comp_move = players1[p].comp_move;
                     add_game_move(frame_num, 5, p, players1[p].comp_move);
                  }
               }
               if (players[p].control_method == 1) rungame_key_check(p, ret); // run game from file
               #ifdef NETPLAY
               if (players[p].control_method == 3) server_local_control(p);
               if (players[p].control_method == 4) client_local_control(p);
               #endif
               set_controls_from_game_move(p); // common for all players
            }   // end of active player iterate
         proc_game_move();  // run once per frame to process system messages from game_move
      } // end of if (!game_exit)
   }
   //printf("ret:%d\n", ret);
   return ret;
}




























































