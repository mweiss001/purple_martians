// z_display.cpp

#include "pm.h"

/*  setup and control of the display

init_display() is called from setup

proc_screen_change_tofs()
proc_screen_change_fromfs()
are both called from z_control when F12 is pressed

When an event from the display 'ALLEGRO_EVENT_DISPLAY_RESIZE' is received the function:
proc_window_resize_event(int x, int y, int w, int h)
is called

if (ev.type == ALLEGRO_EVENT_DISPLAY_RESIZE) proc_display_change();

*/


void show_bitmap_flags(int flags)
{
   printf("bitmap flags:\n");
   if (flags & ALLEGRO_MEMORY_BITMAP)       printf("ALLEGRO_MEMORY_BITMAP\n");
   if (flags & ALLEGRO_VIDEO_BITMAP)        printf("ALLEGRO_VIDEO_BITMAP\n");
   if (flags & ALLEGRO_CONVERT_BITMAP)      printf("ALLEGRO_CONVERT_BITMAP\n");
   if (flags & ALLEGRO_FORCE_LOCKING)       printf("ALLEGRO_FORCE_LOCKING\n");
   if (flags & ALLEGRO_NO_PRESERVE_TEXTURE) printf("ALLEGRO_NO_PRESERVE_TEXTURE\n");
   if (flags & ALLEGRO_ALPHA_TEST)          printf("ALLEGRO_ALPHA_TEST\n");
   if (flags & ALLEGRO_MIN_LINEAR)          printf("ALLEGRO_MIN_LINEAR\n");
   if (flags & ALLEGRO_MAG_LINEAR)          printf("ALLEGRO_MAG_LINEAR\n");
   if (flags & ALLEGRO_MIPMAP)              printf("ALLEGRO_MIPMAP\n");
}

void show_pixel_format(int df)
{
   printf("pixel format:");
   if (df ==  0) printf("ALLEGRO_PIXEL_FORMAT_ANY\n");
   if (df ==  1) printf("ALLEGRO_PIXEL_FORMAT_ANY_NO_ALPHA\n");
   if (df ==  2) printf("ALLEGRO_PIXEL_FORMAT_ANY_WITH_ALPHA\n");
   if (df ==  3) printf("ALLEGRO_PIXEL_FORMAT_ANY_15_NO_ALPHA\n");
   if (df ==  4) printf("ALLEGRO_PIXEL_FORMAT_ANY_16_NO_ALPHA\n");
   if (df ==  5) printf("ALLEGRO_PIXEL_FORMAT_ANY_16_WITH_ALPHA\n");
   if (df ==  6) printf("ALLEGRO_PIXEL_FORMAT_ANY_24_NO_ALPHA\n");
   if (df ==  7) printf("ALLEGRO_PIXEL_FORMAT_ANY_32_NO_ALPHA\n");
   if (df ==  8) printf("ALLEGRO_PIXEL_FORMAT_ANY_32_WITH_ALPHA\n");
   if (df ==  9) printf("ALLEGRO_PIXEL_FORMAT_ARGB_8888\n");
   if (df == 10) printf("ALLEGRO_PIXEL_FORMAT_RGBA_8888\n");
   if (df == 11) printf("ALLEGRO_PIXEL_FORMAT_ARGB_4444\n");
   if (df == 12) printf("ALLEGRO_PIXEL_FORMAT_RGB_888\n");
   if (df == 13) printf("ALLEGRO_PIXEL_FORMAT_RGB_565\n");
   if (df == 14) printf("ALLEGRO_PIXEL_FORMAT_RGB_555\n");
   if (df == 15) printf("ALLEGRO_PIXEL_FORMAT_RGBA_5551\n");
   if (df == 16) printf("ALLEGRO_PIXEL_FORMAT_ARGB_1555\n");
   if (df == 17) printf("ALLEGRO_PIXEL_FORMAT_ABGR_8888\n");
   if (df == 18) printf("ALLEGRO_PIXEL_FORMAT_XBGR_8888\n");
   if (df == 19) printf("ALLEGRO_PIXEL_FORMAT_BGR_888\n");
   if (df == 20) printf("ALLEGRO_PIXEL_FORMAT_BGR_565\n");
   if (df == 21) printf("ALLEGRO_PIXEL_FORMAT_BGR_555\n");
   if (df == 22) printf("ALLEGRO_PIXEL_FORMAT_RGBX_8888\n");
   if (df == 23) printf("ALLEGRO_PIXEL_FORMAT_XRGB_8888\n");
   if (df == 24) printf("ALLEGRO_PIXEL_FORMAT_ABGR_F32\n");
   if (df == 25) printf("ALLEGRO_PIXEL_FORMAT_ABGR_8888_LE\n");
   if (df == 26) printf("ALLEGRO_PIXEL_FORMAT_RGBA_4444\n");
   if (df == 27) printf("ALLEGRO_PIXEL_FORMAT_SINGLE_CHANNEL_8\n");
   if (df == 28) printf("ALLEGRO_PIXEL_FORMAT_COMPRESSED_RGBA_DXT1\n");
   if (df == 29) printf("ALLEGRO_PIXEL_FORMAT_COMPRESSED_RGBA_DXT3\n");
   if (df == 30) printf("ALLEGRO_PIXEL_FORMAT_COMPRESSED_RGBA_DXT5\n");
}

void show_display_flags(int flags)
{
   printf("display flags:\n");
   if (flags & ALLEGRO_WINDOWED)                   printf("ALLEGRO_WINDOWED\n");
   if (flags & ALLEGRO_FULLSCREEN)                 printf("ALLEGRO_FULLSCREEN\n");
   if (flags & ALLEGRO_OPENGL)                     printf("ALLEGRO_OPENGL\n");
   if (flags & ALLEGRO_DIRECT3D_INTERNAL)          printf("ALLEGRO_DIRECT3D_INTERNAL\n");
   if (flags & ALLEGRO_RESIZABLE)                  printf("ALLEGRO_RESIZABLE\n");
   if (flags & ALLEGRO_FRAMELESS)                  printf("ALLEGRO_FRAMELESS\n");
   if (flags & ALLEGRO_GENERATE_EXPOSE_EVENTS)     printf("ALLEGRO_GENERATE_EXPOSE_EVENTS\n");
   if (flags & ALLEGRO_OPENGL_3_0)                 printf("ALLEGRO_OPENGL_3_0\n");
   if (flags & ALLEGRO_OPENGL_FORWARD_COMPATIBLE)  printf("ALLEGRO_OPENGL_FORWARD_COMPATIBLE\n");
   if (flags & ALLEGRO_FULLSCREEN_WINDOW)          printf("ALLEGRO_FULLSCREEN_WINDOW\n");
   if (flags & ALLEGRO_MINIMIZED)                  printf("ALLEGRO_MINIMIZED\n");
   if (flags & ALLEGRO_PROGRAMMABLE_PIPELINE)      printf("ALLEGRO_PROGRAMMABLE_PIPELINE\n");
   if (flags & ALLEGRO_GTK_TOPLEVEL_INTERNAL)      printf("ALLEGRO_GTK_TOPLEVEL_INTERNAL\n");
   if (flags & ALLEGRO_MAXIMIZED)                  printf("ALLEGRO_MAXIMIZED\n");
   if (flags & ALLEGRO_OPENGL_ES_PROFILE)          printf("ALLEGRO_OPENGL_ES_PROFILE\n");
}

void show_display_options(void)
{
   printf("display options\n");
   printf("ALLEGRO_RED_SIZE:%d\n",               al_get_display_option(display, ALLEGRO_RED_SIZE));
   printf("ALLEGRO_GREEN_SIZE:%d\n",             al_get_display_option(display, ALLEGRO_GREEN_SIZE));
   printf("ALLEGRO_BLUE_SIZE:%d\n",              al_get_display_option(display, ALLEGRO_BLUE_SIZE));
   printf("ALLEGRO_ALPHA_SIZE:%d\n",             al_get_display_option(display, ALLEGRO_ALPHA_SIZE));
   printf("ALLEGRO_RED_SHIFT:%d\n",              al_get_display_option(display, ALLEGRO_RED_SHIFT));
   printf("ALLEGRO_GREEN_SHIFT:%d\n",            al_get_display_option(display, ALLEGRO_GREEN_SHIFT));
   printf("ALLEGRO_BLUE_SHIFT:%d\n",             al_get_display_option(display, ALLEGRO_BLUE_SHIFT));
   printf("ALLEGRO_ALPHA_SHIFT:%d\n",            al_get_display_option(display, ALLEGRO_ALPHA_SHIFT));
   printf("ALLEGRO_ACC_RED_SIZE:%d\n",           al_get_display_option(display, ALLEGRO_ACC_RED_SIZE));
   printf("ALLEGRO_ACC_GREEN_SIZE:%d\n",         al_get_display_option(display, ALLEGRO_ACC_GREEN_SIZE));
   printf("ALLEGRO_ACC_BLUE_SIZE:%d\n",          al_get_display_option(display, ALLEGRO_ACC_BLUE_SIZE));
   printf("ALLEGRO_ACC_ALPHA_SIZE:%d\n",         al_get_display_option(display, ALLEGRO_ACC_ALPHA_SIZE));
   printf("ALLEGRO_STEREO:%d\n",                 al_get_display_option(display, ALLEGRO_STEREO));
   printf("ALLEGRO_AUX_BUFFERS:%d\n",            al_get_display_option(display, ALLEGRO_AUX_BUFFERS));
   printf("ALLEGRO_COLOR_SIZE:%d\n",             al_get_display_option(display, ALLEGRO_COLOR_SIZE));
   printf("ALLEGRO_DEPTH_SIZE:%d\n",             al_get_display_option(display, ALLEGRO_DEPTH_SIZE));
   printf("ALLEGRO_STENCIL_SIZE:%d\n",           al_get_display_option(display, ALLEGRO_STENCIL_SIZE));
   printf("ALLEGRO_SAMPLE_BUFFERS:%d\n",         al_get_display_option(display, ALLEGRO_SAMPLE_BUFFERS));
   printf("ALLEGRO_SAMPLES:%d\n",                al_get_display_option(display, ALLEGRO_SAMPLES));
   printf("ALLEGRO_RENDER_METHOD:%d\n",          al_get_display_option(display, ALLEGRO_RENDER_METHOD));
   printf("ALLEGRO_FLOAT_COLOR:%d\n",            al_get_display_option(display, ALLEGRO_FLOAT_COLOR));
   printf("ALLEGRO_FLOAT_DEPTH:%d\n",            al_get_display_option(display, ALLEGRO_FLOAT_DEPTH));
   printf("ALLEGRO_SINGLE_BUFFER:%d\n",          al_get_display_option(display, ALLEGRO_SINGLE_BUFFER));
   printf("ALLEGRO_SWAP_METHOD:%d\n",            al_get_display_option(display, ALLEGRO_SWAP_METHOD));
   printf("ALLEGRO_COMPATIBLE_DISPLAY:%d\n",     al_get_display_option(display, ALLEGRO_COMPATIBLE_DISPLAY));
   printf("ALLEGRO_UPDATE_DISPLAY_REGION:%d\n",  al_get_display_option(display, ALLEGRO_UPDATE_DISPLAY_REGION));
   printf("ALLEGRO_VSYNC:%d\n",                  al_get_display_option(display, ALLEGRO_VSYNC));
   printf("ALLEGRO_MAX_BITMAP_SIZE:%d\n",        al_get_display_option(display, ALLEGRO_MAX_BITMAP_SIZE));
   printf("ALLEGRO_SUPPORT_NPOT_BITMAP:%d\n",    al_get_display_option(display, ALLEGRO_SUPPORT_NPOT_BITMAP));
   printf("ALLEGRO_SUPPORT_SEPARATE_ALPHA:%d\n", al_get_display_option(display, ALLEGRO_SUPPORT_SEPARATE_ALPHA));
   printf("ALLEGRO_AUTO_CONVERT_BITMAPS:%d\n",   al_get_display_option(display, ALLEGRO_AUTO_CONVERT_BITMAPS));
   printf("ALLEGRO_SUPPORTED_ORIENTATIONS:%d\n", al_get_display_option(display, ALLEGRO_SUPPORTED_ORIENTATIONS));
   printf("ALLEGRO_OPENGL_MAJOR_VERSION:%d\n",   al_get_display_option(display, ALLEGRO_OPENGL_MAJOR_VERSION));
   printf("ALLEGRO_OPENGL_MINOR_VERSION:%d\n",   al_get_display_option(display, ALLEGRO_OPENGL_MINOR_VERSION));
   printf("ALLEGRO_DISPLAY_OPTIONS_COUNT:%d\n",  al_get_display_option(display, ALLEGRO_DISPLAY_OPTIONS_COUNT));
}

void show_display_orienation(void)
{
   int dor = al_get_display_orientation(display);
   if (dor ==  0) printf("ALLEGRO_DISPLAY_ORIENTATION_UNKNOWN\n");
   if (dor ==  1) printf("ALLEGRO_DISPLAY_ORIENTATION_0_DEGREES\n");
   if (dor ==  2) printf("ALLEGRO_DISPLAY_ORIENTATION_90_DEGREES\n");
   if (dor ==  4) printf("ALLEGRO_DISPLAY_ORIENTATION_180_DEGREES\n");
   if (dor ==  8) printf("ALLEGRO_DISPLAY_ORIENTATION_270_DEGREES\n");
   if (dor ==  5) printf("ALLEGRO_DISPLAY_ORIENTATION_PORTRAIT\n");
   if (dor == 10) printf("ALLEGRO_DISPLAY_ORIENTATION_LANDSCAPE\n");
   if (dor == 15) printf("ALLEGRO_DISPLAY_ORIENTATION_ALL\n");
   if (dor == 16) printf("ALLEGRO_DISPLAY_ORIENTATION_FACE_UP\n");
   if (dor == 32) printf("ALLEGRO_DISPLAY_ORIENTATION_FACE_DOWN\n");
}

void show_fullscreen_modes(void)
{
   printf("available fullscreen modes:\n");
   int nd = al_get_num_display_modes();
   for (int i=0; i<nd; i++)
   {
      ALLEGRO_DISPLAY_MODE dm;
      al_get_display_mode(i, &dm);
      printf("w:%4d h:%4d -- ", dm.width, dm.height);
      printf("refresh rate:%d ", dm.refresh_rate);
      show_pixel_format(dm.format);
   }
}

void auto_set_display_transform_double(void)
{
   display_transform_double = 1;

   if (disp_w_curr > 1023) display_transform_double = 2;
   if (disp_h_curr > 1023) display_transform_double = 2;

   if (disp_w_curr < 1024) display_transform_double = 1;
   if (disp_h_curr < 700)  display_transform_double = 1;

   if (level_editor_running) display_transform_double = 1;

   if (help_screens_running)
   {
      if (disp_w_curr > 1279) display_transform_double = 2;
      if (disp_w_curr < 1280) display_transform_double = 1;
   }
   set_display_transform();
   set_map_var();
}

void set_display_transform(void)
{
   al_set_target_backbuffer(display);
   SCREEN_W = disp_w_curr/display_transform_double;
   SCREEN_H = disp_h_curr/display_transform_double;
   ALLEGRO_TRANSFORM trans;
   al_identity_transform(&trans);
   al_orthographic_transform(&trans, 0, 0, -1.0, SCREEN_W, SCREEN_H, 1.0);
   al_use_projection_transform(&trans);
}

void show_disp_values(int fs, int disp, int curr, int wind, int full, char *head)
{
   printf("\n%s\n", head);

   if (fs)     printf("fullscreen:%d display_transform_double:%d\n", fullscreen, display_transform_double);

   if (disp)
   {
      int x, y;
      al_get_window_position(display, &x, &y);
      int w = al_get_display_width(display);
      int h = al_get_display_height(display);
      printf(       "disp - x:%4d y:%4d w:%4d h:%4d\n", x, y, w, h);
   }

   if (curr) printf("curr - x:%4d y:%4d w:%4d h:%4d\n", disp_x_curr, disp_y_curr, disp_w_curr, disp_h_curr);
   if (wind) printf("wind - x:%4d y:%4d w:%4d h:%4d\n", disp_x_wind, disp_y_wind, disp_w_wind, disp_h_wind);
   if (full) printf("full - x:%4d y:%4d w:%4d h:%4d\n", disp_x_full, disp_y_full, disp_w_full, disp_h_full);

   //  printf("0:%4d 0:%4d w:%4d h:%4d - scrn\n", 0, 0, SCREEN_W, SCREEN_H);
}



int init_display(void)
{
   //show_fullscreen_modes();

   //al_set_new_display_option(ALLEGRO_COLOR_SIZE, 32, ALLEGRO_REQUIRE);
   al_set_new_display_option(ALLEGRO_COLOR_SIZE, 16, ALLEGRO_SUGGEST);
//   al_set_new_display_option(ALLEGRO_VSYNC, 2, ALLEGRO_SUGGEST);



   int flags = 0;

   if (fullscreen)
   {
      flags = ALLEGRO_FULLSCREEN_WINDOW | ALLEGRO_RESIZABLE;
      al_set_new_display_flags(flags);
      display = al_create_display(disp_w_wind, disp_h_wind);
   }
   else // windowed
   {
      flags = ALLEGRO_WINDOWED | ALLEGRO_RESIZABLE;
      al_set_new_display_flags(flags);
      display = al_create_display(disp_w_wind, disp_h_wind);
   }

//   int flags = ALLEGRO_WINDOWED | ALLEGRO_RESIZABLE | ALLEGRO_OPENGL ;
//   if (fullscreen) flags = ALLEGRO_WINDOWED | ALLEGRO_RESIZABLE | ALLEGRO_FRAMELESS | ALLEGRO_OPENGL;

//   int flags = ALLEGRO_WINDOWED | ALLEGRO_RESIZABLE | ALLEGRO_OPENGL | ALLEGRO_OPENGL_3_0;
//   if (fullscreen) flags = ALLEGRO_WINDOWED | ALLEGRO_RESIZABLE | ALLEGRO_FRAMELESS | ALLEGRO_OPENGL | ALLEGRO_OPENGL_3_0;

//   al_set_display_flag(display, ALLEGRO_FULLSCREEN_WINDOW, fullscreen);





   if(!display)
   {
      sprintf(msg, "Error creating display\n");
      m_err(msg);
      exit(0);
   }
   if (!fullscreen) al_set_window_position(display, disp_x_wind, disp_y_wind);
   if (fullscreen)  al_resize_display(display, disp_w_full, disp_h_full);
   al_acknowledge_resize(display);


   disp_w_curr = al_get_display_width(display);
   disp_h_curr = al_get_display_height(display);
   al_get_window_position(display, &disp_x_curr, &disp_y_curr);
   //printf("x:%d y:%d w:%d h:%4d\n", disp_x_curr, disp_y_curr, disp_w_curr, disp_h_curr);

   auto_set_display_transform_double();
   window_title();

   //show_display_flags(al_get_display_flags(display));
   //show_display_options();
   //show_pixel_format(al_get_display_format(display));
   //show_display_orienation();
   //printf("refresh rate:%d\n", al_get_display_refresh_rate(display));

   //printf("init screen\n");
   create_bmp();
   make_palette();
   return 1;
}





void proc_display_change(void)
{
   al_acknowledge_resize(display);          // important that this is here, later and it does not work as intended
   int w = al_get_display_width(display);   // set my local variables with the system ones
   int h = al_get_display_height(display);
   int x, y;
   al_get_window_position(display, &x, &y);

   if (fullscreen)
   {
      disp_x_curr = disp_x_full = x;
      disp_y_curr = disp_y_full = y;
      disp_w_curr = disp_w_full = w;
      disp_h_curr = disp_h_full = h;
   }
   else
   {
      disp_x_curr = disp_x_wind = x;
      disp_y_curr = disp_y_wind = y;
      disp_w_curr = disp_w_wind = w;
      disp_h_curr = disp_h_wind = h;
   }
   auto_set_display_transform_double();
   rebuild_bitmaps();
   save_config();
   Redraw = 1;
   // show_disp_values(0, 1, 1, 1, 0, "get var and process_screen_change end");
}


void save_display_window_position(void)
{
   // this is automatically done when display is resized or switched to fullscreen
   // but not when the window is just moved
   // so I call this when exiting

   if (!fullscreen)
   {
      int x, y;
      al_get_window_position(display, &x, &y);
      disp_x_wind = x;
      disp_y_wind = y;
      disp_w_wind = al_get_display_width(display);
      disp_h_wind = al_get_display_height(display);

      save_config();
   }
}

void proc_display_change_tofs(void)
{
   //printf("\n-----------to fullscreen------------\n");
   fullscreen = 1;

   // save window position and size before entering fullscreen
   int x, y;
   al_get_window_position(display, &x, &y);
   disp_x_wind = x;
   disp_y_wind = y;
   disp_w_wind = al_get_display_width(display);
   disp_h_wind = al_get_display_height(display);

   al_set_display_flag(display, ALLEGRO_FULLSCREEN_WINDOW, fullscreen);

   // here is one of the few places I will set xywh from my local variables
//   al_resize_display(display, disp_w_full, disp_h_full);
//   al_set_window_position(display, disp_x_full, disp_y_full);
   proc_display_change();
}


void proc_display_change_fromfs(void)
{
   //printf("\n-----------to windowed--------------\n");
   fullscreen = 0;
   al_set_display_flag(display, ALLEGRO_FULLSCREEN_WINDOW, fullscreen);

   al_acknowledge_resize(display);

   // here is one of the few places I will set xywh from my local variables
   al_resize_display(display, disp_w_wind, disp_h_wind);
   al_set_window_position(display, disp_x_wind, disp_y_wind);
   proc_display_change();
}





/*




int do_actual_display_values_match_curr()
{
   // detect if actual window position or size is different from what it should be
   int w = al_get_display_width(display);
   int h = al_get_display_height(display);
   int x, y;
   al_get_window_position(display, &x, &y);
   if ((x != disp_x_curr) || (y != disp_y_curr) || (w != disp_w_curr) || (h != disp_h_curr) ) return 0;
   return 1;
//   if (al_get_display_flags(display) & ALLEGRO_FULLSCREEN_WINDOW)

}



void enforce_display_xywh(void) // not used
{
   if (!do_actual_display_values_match_curr())
   {
      show_disp_values(0, 1, 1, 1, 0, "enforce display difference");
      proc_screen_change();
   }
}


void proc_screen_change(void)
{
   int w = al_get_display_width(display);
   int h = al_get_display_height(display);
   int x, y;
   al_get_window_position(display, &x, &y);

   if (fullscreen)
   {
      disp_x_curr = disp_x_full = x;
      disp_y_curr = disp_y_full = y;
      disp_w_curr = disp_w_full = w;
      disp_h_curr = disp_h_full = h;
   }
   else
   {
      disp_x_curr = disp_x_wind = x;
      disp_y_curr = disp_y_wind = y;
      disp_w_curr = disp_w_wind = w;
      disp_h_curr = disp_h_wind = h;
   }
   al_acknowledge_resize(display);

   */


/*

   int q = 0;
   while (!do_actual_display_values_match_curr())
   {
    //  printf("\n-----proc_screen_change() try %d", q++);

      al_resize_display(display, disp_w_curr, disp_h_curr);
     // show_disp_values(0, 1, 1, 1, 0, "process_screen_change 1 - res");

      al_set_window_position(display, disp_x_curr, disp_y_curr); // make sure to set position last ???
     // show_disp_values(0, 1, 1, 1, 0, "process_screen_change 2 - pos");

      al_acknowledge_resize(display);
     // show_disp_values(0, 1, 1, 1, 0, "process_screen_change 3 - ack");
   }
   auto_set_display_transform_double();
   rebuild_bitmaps();
   save_config();
   Redraw = 1;

   //show_disp_values(0, 1, 1, 1, 0, "process_screen_change end");

   sprintf(msg, "\n----process_screen_change end after %d tries", q);
   show_disp_values(0, 1, 1, 1, 0, msg);
*/


/*

   int q = 0;
   while (!do_actual_display_values_match_curr())
   {





      printf("\n-----proc_screen_change() try %d", q++);

      al_resize_display(display, disp_w_curr, disp_h_curr);
      show_disp_values(0, 1, 1, 1, 0, "process_screen_change 1 - res");

      al_set_window_position(display, disp_x_curr, disp_y_curr); // make sure to set position last ???
      show_disp_values(0, 1, 1, 1, 0, "process_screen_change 2 - pos");

      al_acknowledge_resize(display);
      show_disp_values(0, 1, 1, 1, 0, "process_screen_change 3 - ack");
   }

*/

/*
   auto_set_display_transform_double();
   rebuild_bitmaps();
   save_config();
   Redraw = 1;
   show_disp_values(0, 1, 1, 1, 0, "process_screen_change end");




}


void old_proc_window_resize_event(int x, int y, int w, int h)
{
   // show_disp_values(0, 1, 1, 1, 0, \n----resize event initial  );

*/

/*
   if (fullscreen)
   {
      printf("\nGot a window resize event while in fullscreen...this is unexpected\n");
      disp_x_curr = disp_x_full = x;
      disp_y_curr = disp_y_full = y;
      disp_w_curr = disp_w_full = w;
      disp_h_curr = disp_h_full = h;
   }
   else
   {
      disp_x_curr = disp_x_wind = x-10; // this takes into account the window frame size in windows
      disp_y_curr = disp_y_wind = y-32;
      //disp_x_curr = disp_x_wind = x;
      //disp_y_curr = disp_y_wind = y;
      disp_w_curr = disp_w_wind = w;
      disp_h_curr = disp_h_wind = h;
   }
   //show_disp_values(0, 1, 1, 1, 0, "\n----resize event final");

   sprintf(msg, "\n----resize event  x:%d y:%d w:%d h:%d", x, y, w, h);
   show_disp_values(0, 1, 1, 1, 0, msg);

   proc_screen_change();
}


void detect_window_move(void) // not used
{
   int x, y;
   al_get_window_position(display, &x, &y);
   if ((x != disp_x_curr) || (y != disp_y_curr))
   {
      if (fullscreen) printf("\nWe got a window move detection while in fullscreen...this is unexpected\n");
      disp_x_curr = disp_x_wind = x;
      disp_y_curr = disp_y_wind = y;

      show_disp_values(0, 1, 1, 1, 0, "\n----window move detected");

      //enforce_display_xywh();
   }
}

*/




