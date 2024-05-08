// mwInput.cpp

#include "pm.h"
#include "mwInput.h"
#include "mwDisplay.h"
#include "mwPlayer.h"
#include "mwLog.h"
#include "mwNetgame.h"
#include "mwSettings.h"
#include "mwColor.h"
#include "mwFont.h"
#include "mwEventQueue.h"
#include "mwLoop.h"
#include "mwConfig.h"
#include "mwMain.h"
#include "mwScreen.h"
#include "mwGameMoves.h"
#include "mwLevel.h"
#include "mwDemoMode.h"
#include "mwQuickGraph2.h"



mwInput mInput;

mwInput::mwInput()
{
   //initialize();
}

void mwInput::initialize(void)
{
   if (!mDisplay.no_display)
   {
      for (int i=0; i<64; i++) skc[i] = 0;
      skc_index = 0;

      for (int k = ALLEGRO_KEY_A; k < ALLEGRO_KEY_MAX; k++)
         for (int i=0; i<4; i++)
            key[k][i] = false;

      for (int k=0; k<5; k++)
         for (int i=0; i<4; i++)
            mouse_b[k][i] = false;

      key_pressed_ASCII = 0;
      key_pressed = 0;
      key_or_mouse_pressed = 0;


      ALLEGRO_MOUSE_STATE state;
      al_get_mouse_state(&state);
      mouse_x = state.x / mDisplay.display_transform_double;
      mouse_y = state.y / mDisplay.display_transform_double;
      mouse_z = state.z / mDisplay.display_transform_double;

      mouse_dx = 0;
      mouse_dy = 0;
      mouse_dz = 0;
   }
}


void mwInput::proc_input_events(ALLEGRO_EVENT ev)
{
   if (ev.type == ALLEGRO_EVENT_MOUSE_WARPED)
   {
      mouse_x = ev.mouse.x / mDisplay.display_transform_double;
      mouse_y = ev.mouse.y / mDisplay.display_transform_double;
   }
   if (ev.type == ALLEGRO_EVENT_MOUSE_AXES)
   {
      mouse_x = ev.mouse.x / mDisplay.display_transform_double;
      mouse_y = ev.mouse.y / mDisplay.display_transform_double;
      mouse_z = ev.mouse.z / mDisplay.display_transform_double;
      mouse_dx = ev.mouse.dx;
      mouse_dy = ev.mouse.dy;
      mouse_dz = ev.mouse.dz;
      al_show_mouse_cursor(mDisplay.display);
      al_set_timer_count(mEventQueue.mou_timer, 0);
   }

   if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
   {
      if (ev.mouse.button == 1) mouse_b[1][0] = true;
      if (ev.mouse.button == 2) mouse_b[2][0] = true;
      if (ev.mouse.button == 3) mouse_b[3][0] = true;
      if (ev.mouse.button == 4) mouse_b[4][0] = true;
      al_show_mouse_cursor(mDisplay.display);
      al_set_timer_count(mEventQueue.mou_timer, 0);
      key_or_mouse_pressed = 1;
   }
   if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
   {
      if (ev.mouse.button == 1) mouse_b[1][0] = false;
      if (ev.mouse.button == 2) mouse_b[2][0] = false;
      if (ev.mouse.button == 3) mouse_b[3][0] = false;
      if (ev.mouse.button == 4) mouse_b[4][0] = false;
   }
   if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
   {
      key_pressed = 1;
      key_or_mouse_pressed = 1;
      int k = ev.keyboard.keycode;
      key[k][0] = true;
      // if (k == ALLEGRO_KEY_PRINTSCREEN) printf("PRINTSCREEN pressed\n");
   }
   if (ev.type == ALLEGRO_EVENT_KEY_UP)
   {
      int k = ev.keyboard.keycode;
      key[k][0] = false;
      //if (k == ALLEGRO_KEY_PRINTSCREEN) key[k][0] = true; // special exception to make PRINTSCREEN work
      //if (k == ALLEGRO_KEY_PRINTSCREEN) printf("PRINTSCREEN released\n");
   }
   if (ev.type == ALLEGRO_EVENT_KEY_CHAR)
   {
      key_pressed_ASCII = ev.keyboard.unichar;
      serial_key_check(key_pressed_ASCII);
      mEventQueue.menu_update = 1;
      //printf("key_pressed_ASCII:%d\n", key_pressed_ASCII);
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
         key[130+jo][0] = false;
         key[131+jo][0] = false;
         if (pos > 0) key[131+jo][0] = true;
         if (pos < 0) key[130+jo][0] = true;
      }
      if (ax == 1) // y axis
      {
         key[128+jo][0] = false;
         key[129+jo][0] = false;
         if (pos > 0) key[129+jo][0] = true;
         if (pos < 0) key[128+jo][0] = true;
      }
   }
   if (ev.type == ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN)
   {
      int jy = getJoystickNum(ev.joystick.id);
      int sc = get_scan_code_from_joystick(jy, 1, ev.joystick.button);
      key[sc][0] = true;
   }
   if (ev.type == ALLEGRO_EVENT_JOYSTICK_BUTTON_UP)
   {
      int jy = getJoystickNum(ev.joystick.id);
      int sc = get_scan_code_from_joystick(jy, 1, ev.joystick.button);
      key[sc][0] = false;
   }
}


void mwInput::proc_keys_held(void)
{
   for (int k = ALLEGRO_KEY_A; k < ALLEGRO_KEY_MAX; k++)
   {
      if ((key[k][0] == true) && (key[k][1] == false)) key[k][2] = true; // just pressed
      else key[k][2] = false;
      if ((key[k][0] == false) && (key[k][1] == true)) key[k][3] = true; // just released
      else key[k][3] = false;
      key[k][1] = key[k][0]; // previous for next time
   }
   for (int m=1; m<5; m++)
   {
      //printf("proc held\n");

      if ((mouse_b[m][0] == true) && (mouse_b[m][1] == false)) mouse_b[m][2] = true; // just pressed
      else mouse_b[m][2] = false;
      if ((mouse_b[m][0] == false) && (mouse_b[m][1] == true)) mouse_b[m][3] = true; // just released
      else mouse_b[m][3] = false;
      mouse_b[m][1] = mouse_b[m][0]; // previous for next time
   }
   function_key_check();
}



bool mwInput::serial_key_test(const char *tst)
{
   int tl = strlen(tst);
   if ((skc_index > tl-1) && (memcmp((skc + skc_index-tl), tst, tl) == 0)) return true;
   return false;
}

void mwInput::serial_key_check(int key)
{
   skc[skc_index] = key;
   if (++skc_index > 63) skc_index = 0;
   skc[skc_index] = 0;

   if ((serial_key_test("test")) || (serial_key_test("TEST"))) printf("test matched!!!\n");

   if ((serial_key_test("ijkl c")) || (serial_key_test("IJKL C")))
   {
      set_controls_to_custom_sets(4);
      mConfig.save_config();
   }

   if ((mNetgame.ima_client) && (serial_key_test("sendfile"))) mNetgame.client_send_crfl();

   if ((serial_key_test("fakekey")) || (serial_key_test("FAKEKEY")))
   {
      mPlayer.loc[mPlayer.active_local_player].fake_keypress_mode =! mPlayer.loc[mPlayer.active_local_player].fake_keypress_mode;
      printf("fake keypress mode:%d\n", mPlayer.loc[mPlayer.active_local_player].fake_keypress_mode);
   }



   if (serial_key_test("win800"))
   {
      mDisplay.disp_w_wind = 800;
      mDisplay.disp_h_wind = 600;
      mConfig.save_config();
      mDisplay.set_windowed();
      mDisplay.proc_display_change();
   }

   if (serial_key_test("win1024"))
   {
      mDisplay.disp_w_wind = 1024;
      mDisplay.disp_h_wind = 768;
      mConfig.save_config();
      mDisplay.set_windowed();
      mDisplay.proc_display_change();
   }

   if (serial_key_test("win1280"))
   {
      mDisplay.disp_w_wind = 1280;
      mDisplay.disp_h_wind = 1024;
      mConfig.save_config();
      mDisplay.set_windowed();
      mDisplay.proc_display_change();
   }

   if (serial_key_test("win1920"))
   {
      mDisplay.disp_w_wind = 1920;
      mDisplay.disp_h_wind = 1080;
      mConfig.save_config();
      mDisplay.set_windowed();
      mDisplay.proc_display_change();
   }





   if (serial_key_test("demr"))
   {
      if (mLoop.state[1] != PM_PROGRAM_STATE_DEMO_RECORD) mLoop.state[0] = PM_PROGRAM_STATE_DEMO_RECORD;
      else if (mLoop.state[1] == PM_PROGRAM_STATE_DEMO_RECORD) mLoop.state[0] = PM_PROGRAM_STATE_MENU;
   }


   mLevel.skc_trigger_demo = 0;

   if ((serial_key_test("demo")) || (serial_key_test("DEMO"))) mLevel.skc_trigger_demo = 1;
   if ((serial_key_test("cheat")) || (serial_key_test("CHEAT")))
   {
      mLevel.skc_trigger_demo = 1;
      mLevel.skc_trigger_demo_cheat = 1;
   }
}


void mwInput::function_key_check(void)
{
   // debug variables used when adjusting things
   if (key[ALLEGRO_KEY_UP][   2]) mLoop.pct_y--;
   if (key[ALLEGRO_KEY_DOWN][ 2]) mLoop.pct_y++;
   if (key[ALLEGRO_KEY_LEFT][ 2]) mLoop.pct_x--;
   if (key[ALLEGRO_KEY_RIGHT][2]) mLoop.pct_x++;


   // fullscreen toggle
   if (key[function_key_fullscreen][2])  mDisplay.toggle_fullscreen();

   // cycle text double modes
   if (key[function_key_text_double][2]) mDisplay.cycle_display_transform();

   // save a screenshot
   if (key[function_key_printscreen][2])
   {
      ALLEGRO_BITMAP *ss_bmp = al_create_bitmap(mDisplay.disp_w_curr, mDisplay.disp_h_curr);
      al_set_target_bitmap(ss_bmp);
      al_draw_bitmap(al_get_backbuffer(mDisplay.display), 0, 0, 0);
      al_make_directory("screenshots"); // create if not already created
      char filename[80];
      struct tm *timenow;
      time_t now = time(NULL);
      timenow = localtime(&now);

      //strftime(filename, sizeof(filename), "screenshots\\%Y%m%d%H%M%S.bmp", timenow); // bug - does not work on linux
      strftime(filename, sizeof(filename), "screenshots/%Y%m%d%H%M%S.bmp", timenow);

      printf("saved: %s\n", filename);
      al_save_bitmap(filename, ss_bmp);
      al_destroy_bitmap(ss_bmp);
   }

   // cycle debug overlay modes
   if (key[function_key_debug_overlay][2])
      if (++mLoop.show_debug_overlay >= mSettings.number_of_debug_overlay_modes) mLoop.show_debug_overlay = 0;

   // force init level background
   if (key[function_key_init_background][2]) mScreen.init_level_background();

   // force instant game save and log save
   if (key[function_key_force_save][3])
   {
      mGameMoves.save_gm_make_fn("F key force save", 0);
      mLog.save_log_file();
   }




   if ((mLoop.level_editor_running) || (mLoop.state[1] == PM_PROGRAM_STATE_MAIN_GAME_LOOP) || (mLoop.state[1] == PM_PROGRAM_STATE_DEMO_RECORD) || (mDemoMode.mode)) // only do these when game is running or in level editor
   {
      // zoom controls
      if (key[function_key_zoom_out][0]) mDisplay.set_scale_factor(mDisplay.scale_factor * .90, 0);
      if (key[function_key_zoom_in][0])  mDisplay.set_scale_factor(mDisplay.scale_factor * 1.1, 0);
      if ((key[function_key_zoom_out][0]) && (key[function_key_zoom_in][0])) mDisplay.set_scale_factor(1, 1);


      // toggle fake keypress mode (for stress testing netgame)
      if (key[function_key_fakekey][3])
      {
         mPlayer.loc[mPlayer.active_local_player].fake_keypress_mode =! mPlayer.loc[mPlayer.active_local_player].fake_keypress_mode;
         printf("fake keypress mode:%d\n", mPlayer.loc[mPlayer.active_local_player].fake_keypress_mode);
      }




      // adjust speed (fps)
      if ((!mLoop.speed_control_lock) && (!mNetgame.ima_client) && (!mNetgame.ima_server)) // only adjust speed if not locked and not in netgame
      {
         int old_fps = mLoop.frame_speed;

         if (key[function_key_speed_dec][2]) // decrease
         {
            if (SHFT() && CTRL())       mLoop.frame_speed -=1000;
            else if (SHFT())            mLoop.frame_speed -=100;
            else if (CTRL())            mLoop.frame_speed -=20;
            else                        mLoop.frame_speed -=1;
            if (mLoop.frame_speed < 5)  mLoop.frame_speed =5;
         }
         if (key[function_key_speed_inc][2]) // increase
         {
            if (SHFT() && CTRL())            mLoop.frame_speed +=1000;
            else if (SHFT())                 mLoop.frame_speed +=100;
            else if (CTRL())                 mLoop.frame_speed +=20;
            else                             mLoop.frame_speed +=1;
            if (mLoop.frame_speed > 100000)  mLoop.frame_speed = 100000;
         }
         if ((key[function_key_speed_dec][0]) && (key[function_key_speed_dec][1]) && (key[function_key_speed_inc][0]) && (key[function_key_speed_inc][1])) // reset to 40fps
         {
            mLoop.frame_speed = 40;
         }
         if (old_fps != mLoop.frame_speed) mEventQueue.adjust_fps_timer(mLoop.frame_speed);
      }
   }
}




int mwInput::getJoystickNum(ALLEGRO_JOYSTICK* joy) // Thanks Edgar Reynaldo!
{
   for (int i=0; i<al_get_num_joysticks(); ++i)
   {
      if (joy == al_get_joystick(i)) {return i;}
   }
   return -1;
}

int mwInput::get_scan_code_from_joystick(int joy, int button, int num)
{
   int ret = 0;
   int base = 128;
   if (joy == 0) base = 128;
   if (joy == 1) base = 148;
   if (button) ret = num + 4;
   return base + ret;
}





int mwInput::my_readkey(void) // only get key or joystick bindings
{
   int quit = 0, ret = 0;

   while (!quit)
   {
      while (!al_is_event_queue_empty(mEventQueue.event_queue))
      {
         ALLEGRO_EVENT ev;
         if (al_get_next_event(mEventQueue.event_queue, &ev))
         {
             if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) mMain.fast_exit();
             if (ev.type == ALLEGRO_EVENT_KEY_UP)
             {
                ret = ev.keyboard.keycode;
                if ((ret != ALLEGRO_KEY_ENTER) && (ret != ALLEGRO_KEY_PRINTSCREEN)) quit = 1;
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
   initialize();
   return ret;
}


bool mwInput::my_readkey2(int x, int y, int tc, int bts, int num) // used only to set new game control key or joystick bindings
{
   mSettings.redraw_all_controls(x, y, bts, tc, 0, num);
   al_flip_display();
   int k = my_readkey();
   int key_used = 0;
   if (k)
   {
      if (is_key_used(k)) key_used = 1;

      // exception if the key is used on the same control, don't force to change
      if (key_used)
      {
         int original_k = - 1;
         switch (num)
         {
            case  0: original_k = mPlayer.loc[0].up_key;    break;
            case  1: original_k = mPlayer.loc[0].down_key;  break;
            case  2: original_k = mPlayer.loc[0].left_key;  break;
            case  3: original_k = mPlayer.loc[0].right_key; break;
            case  4: original_k = mPlayer.loc[0].jump_key;  break;
            case  5: original_k = mPlayer.loc[0].fire_key;  break;
            case  6: original_k = mPlayer.loc[0].menu_key;  break;
         }
         if (k == original_k) key_used = 0;
      }
   }

   if (key_used)
   {
      char msg[80];
      sprintf(msg, "key [%s] is already used", key_names[k]);
      printf("%s\n", msg);
      m_err(msg);
      return false;
   }
   else switch (num)
   {
      case  0: mPlayer.loc[0].up_key    =  k; break;
      case  1: mPlayer.loc[0].down_key  =  k; break;
      case  2: mPlayer.loc[0].left_key  =  k; break;
      case  3: mPlayer.loc[0].right_key =  k; break;
      case  4: mPlayer.loc[0].jump_key  =  k; break;
      case  5: mPlayer.loc[0].fire_key  =  k; break;
      case  6: mPlayer.loc[0].menu_key  =  k; break;
   }
   return true;
}


void mwInput::my_readkey3(int x, int y, int tc, int bts, int num, int num_lines) // used only to set new function control key or joystick bindings
{
   mSettings.redraw_all_fcontrols(x, y, bts, tc, 0, num, num_lines);
   al_flip_display();
   int k = my_readkey();
   if (k == ALLEGRO_KEY_ESCAPE) k = 0;

   if ((k) && (is_key_used(k)))
   {
      char msg[80];
      sprintf(msg, "key [%s] is already used", key_names[k]);
      printf("%s\n", msg);
      m_err(msg);
   }
   else switch (num)
   {
      case  0: function_key_fullscreen      = k; break;
      case  1: function_key_text_double     = k; break;
      case  2: function_key_debug_overlay   = k; break;
      case  3: function_key_zoom_out        = k; break;
      case  4: function_key_zoom_in         = k; break;
      case  5: function_key_speed_dec       = k; break;
      case  6: function_key_speed_inc       = k; break;
      case  7: function_key_printscreen     = k; break;
      case  8: function_key_fakekey         = k; break;
      case  9: function_key_force_save      = k; break;
      case 10: function_key_init_background = k; break;
   }
}




int mwInput::is_key_used(int k)
{
   int ret = 0;
   if (function_key_fullscreen == k) ret = 1;
   if (function_key_text_double == k) ret = 1;
   if (function_key_debug_overlay == k) ret = 1;
   if (function_key_zoom_out == k) ret = 1;
   if (function_key_zoom_in == k) ret = 1;
   if (function_key_printscreen == k) ret = 1;
   if (function_key_fakekey == k) ret = 1;
   if (function_key_force_save == k) ret = 1;
   if (function_key_init_background == k) ret = 1;
   if (function_key_speed_dec == k) ret = 1;
   if (function_key_speed_inc == k) ret = 1;

   if (mPlayer.loc[0].up_key    == k) ret = 1;
   if (mPlayer.loc[0].down_key  == k) ret = 1;
   if (mPlayer.loc[0].left_key  == k) ret = 1;
   if (mPlayer.loc[0].right_key == k) ret = 1;
   if (mPlayer.loc[0].jump_key  == k) ret = 1;
   if (mPlayer.loc[0].fire_key  == k) ret = 1;
   if (mPlayer.loc[0].menu_key  == k) ret = 1;

   return ret;
}



void mwInput::get_all_keys(int x, int y, int tc, int bts) // prompts for all seven keys
{
   int k=0;

   while (k<7)
   {
      if (my_readkey2(x,y,tc,bts,k)) k++;  // iterate the keys
   }

//   for (int k=0; k<7; k++) my_readkey2(x,y,tc,bts,k);  // iterate the keys
   mConfig.save_config();
}

void mwInput::test_keys(int x, int sy)
{
   ALLEGRO_COLOR c1 = mColor.pc[15];
   ALLEGRO_COLOR c2 = mColor.pc[10];
   ALLEGRO_COLOR c3 = mColor.pc[14];

   int quit = 0;
   while (!quit)
   {
      int y = sy;

      al_draw_filled_rectangle(x-170, y+6, x+170, y+(24*8), mColor.pc[0]); // erase background

      al_draw_text(mFont.pr8, c1, x, y+=8, ALLEGRO_ALIGN_CENTRE, "----------------------------------------" );
      al_draw_text(mFont.pr8, c1, x, y+=8, ALLEGRO_ALIGN_CENTRE, "Test controller setup.");
      al_draw_text(mFont.pr8, c3, x, y+=8, ALLEGRO_ALIGN_CENTRE, "(mouse_b2 to quit)");
      al_draw_text(mFont.pr8, c1, x, y+=8, ALLEGRO_ALIGN_CENTRE, "----------------------------------------");
      al_draw_text(mFont.pr8, c1, x, y+=8, ALLEGRO_ALIGN_CENTRE, "Test how pressing multiple");
      al_draw_text(mFont.pr8, c1, x, y+=8, ALLEGRO_ALIGN_CENTRE, "controls affects other controls.");
      al_draw_text(mFont.pr8, c1, x, y+=8, ALLEGRO_ALIGN_CENTRE, "----------------------------------------");
      al_draw_text(mFont.pr8, c1, x, y+=8, ALLEGRO_ALIGN_CENTRE, "Keyboards widely differ in the number");
      al_draw_text(mFont.pr8, c1, x, y+=8, ALLEGRO_ALIGN_CENTRE, "keys that can be detected at one time.");
      al_draw_text(mFont.pr8, c1, x, y+=8, ALLEGRO_ALIGN_CENTRE, "----------------------------------------" );
      al_draw_text(mFont.pr8, c1, x, y+=8, ALLEGRO_ALIGN_CENTRE, "Some joysticks do not allow 2 directions");
      al_draw_text(mFont.pr8, c1, x, y+=8, ALLEGRO_ALIGN_CENTRE, "to be pressed at the same time.");
      al_draw_text(mFont.pr8, c1, x, y+=8, ALLEGRO_ALIGN_CENTRE, "This will make the game harder to play.");
      al_draw_text(mFont.pr8, c1, x, y+=8, ALLEGRO_ALIGN_CENTRE, "----------------------------------------" );



      y +=16;
      if (mPlayer.syn[0].up) al_draw_text(mFont.pr8, c2, x, y, ALLEGRO_ALIGN_CENTRE, "UP" );
      y +=8;
      if (mPlayer.syn[0].down) al_draw_text(mFont.pr8, c2, x, y, ALLEGRO_ALIGN_CENTRE, "DOWN" );
      y +=8;
      if (mPlayer.syn[0].left) al_draw_text(mFont.pr8, c2, x, y, ALLEGRO_ALIGN_CENTRE, "LEFT" );
      y +=8;
      if (mPlayer.syn[0].right) al_draw_text(mFont.pr8, c2, x, y, ALLEGRO_ALIGN_CENTRE, "RIGHT");
      y +=8;
      if (mPlayer.syn[0].jump) al_draw_text(mFont.pr8, c2, x, y, ALLEGRO_ALIGN_CENTRE, "JUMP" );
      y +=8;
      if (mPlayer.syn[0].fire) al_draw_text(mFont.pr8, c2, x, y, ALLEGRO_ALIGN_CENTRE, "FIRE" );
      y +=8;
      if (mPlayer.syn[0].menu) al_draw_text(mFont.pr8, c2, x, y, ALLEGRO_ALIGN_CENTRE, "MENU" );

      al_flip_display();
      mEventQueue.proc_menu();
      if (mInput.mouse_b[2][0])
      {
         while (mInput.mouse_b[2][0]) mEventQueue.proc(1);
         quit = 1;
      }
   }
}


void mwInput::set_controls_to_custom_sets(int s)
{
//   int p = mPlayer.active_local_player;
//   // the only time this will not be zero is when a client is in a netgame
//   // this is here so that client can change controls with serial_key_check IJKL

   if (s == 1) // set all to joy1
   {
      mPlayer.loc[0].up_key = 128;
      mPlayer.loc[0].down_key = 129;
      mPlayer.loc[0].right_key = 131;
      mPlayer.loc[0].left_key = 130;
      mPlayer.loc[0].jump_key = 133;
      mPlayer.loc[0].fire_key = 132;
      mPlayer.loc[0].menu_key = 135;
   }
   if (s == 2) // set all to joy2
   {
      mPlayer.loc[0].up_key = 148;
      mPlayer.loc[0].down_key = 149;
      mPlayer.loc[0].right_key = 151;
      mPlayer.loc[0].left_key = 150;
      mPlayer.loc[0].jump_key = 153;
      mPlayer.loc[0].fire_key = 152;
      mPlayer.loc[0].menu_key = 155;
   }
   if (s == 3) // set all to arrows
   {
      mPlayer.loc[0].up_key = 84;
      mPlayer.loc[0].down_key = 85;
      mPlayer.loc[0].right_key = 83;
      mPlayer.loc[0].left_key = 82;
      mPlayer.loc[0].jump_key = 75;
      mPlayer.loc[0].fire_key = 3;
      mPlayer.loc[0].menu_key = 59;
   }
   if (s == 4) // set all to IJKL
   {
      mPlayer.loc[0].up_key = 9;
      mPlayer.loc[0].down_key = 11;
      mPlayer.loc[0].right_key = 12;
      mPlayer.loc[0].left_key = 10;
      mPlayer.loc[0].jump_key = 75;
      mPlayer.loc[0].fire_key = 3;
      mPlayer.loc[0].menu_key = 59;
   }
   if (s == 5) // set all to WASD
   {
      mPlayer.loc[0].up_key = 23;
      mPlayer.loc[0].down_key = 19;
      mPlayer.loc[0].left_key = 1;
      mPlayer.loc[0].right_key = 4;
      mPlayer.loc[0].jump_key = 75;
      mPlayer.loc[0].fire_key = 3;
      mPlayer.loc[0].menu_key = 59;
   }
}

int mwInput::SHFT(void)
{
   if ((key[ALLEGRO_KEY_LSHIFT][0]) || (key[ALLEGRO_KEY_RSHIFT][0])) return 1;
   else return 0;
}

int mwInput::CTRL(void)
{
   if ((key[ALLEGRO_KEY_LCTRL][0]) || (key[ALLEGRO_KEY_RCTRL][0])) return 1;
   else return 0;
}

void mwInput::m_err(const char * err_msg)
{
   //fprintf(stderr, "%s\n", err_msg);
   al_show_native_message_box(mDisplay.display, "Error", "Error:", err_msg, NULL,  ALLEGRO_MESSAGEBOX_ERROR );
}

void mwInput::tsw(void)
{
   al_flush_event_queue(mEventQueue.event_queue);
   int quit = 0;
   while(!quit)
   {
      while (!al_is_event_queue_empty(mEventQueue.event_queue))
      {
         ALLEGRO_EVENT ev;
         if (al_get_next_event(mEventQueue.event_queue, &ev))
         {
             if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) mMain.fast_exit();
             if (ev.type == ALLEGRO_EVENT_KEY_DOWN) quit = 1;
         }
      }
   }
   while (key[ALLEGRO_KEY_ESCAPE][0]) mEventQueue.proc(1);
}

