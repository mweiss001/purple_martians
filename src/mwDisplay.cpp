// mwDisplay.cpp

#include "pm.h"
#include "mwDisplay.h"
#include "mwWindow.h"
#include "mwWindowManager.h"
#include "mwBitmap.h"
#include "mwProgramState.h"
#include "z_menu.h"
#include "z_config.h"
#include "z_fnx.h"
#include "z_screen.h"






ALLEGRO_DISPLAY *display = NULL;

mwDisplay mwD;







void mwDisplay::mw_set_clipping_rect(float x, float y, float w, float h)
{
   al_set_clipping_rectangle(x * display_transform_double, y * display_transform_double, w * display_transform_double, h * display_transform_double);
}


void mwDisplay::set_scale_factor(float new_scale_factor, int instant)
{
   if ((scale_factor_holdoff <= 0) || (instant))
   {
      scale_factor = new_scale_factor;
      scale_factor_holdoff = 10;
      // enforce max and min
      if (scale_factor < .2) scale_factor = .2;
      if (scale_factor > 40) scale_factor = 40;
      show_scale_factor = 80;
      save_config();
      if (instant) scale_factor_current = scale_factor;
   }
}

void mwDisplay::proc_scale_factor_change(void)
{
   if (show_scale_factor    > 0) show_scale_factor--;
   if (scale_factor_holdoff > 0) scale_factor_holdoff--;
   if (scale_factor_current < scale_factor)
   {
       // try to scale the inc, larger as scale_factor gets larger
       float inc = scale_factor_inc * scale_factor_current / 3;
       scale_factor_current += inc;
       // if we overshoot set to exact to prevent oscillation
       if (scale_factor_current > scale_factor) scale_factor_current = scale_factor;
   }
   if (scale_factor_current > scale_factor)
   {
       // try to scale the inc, larger as scale_factor gets larger
       float inc = scale_factor_inc * scale_factor_current / 3;
       scale_factor_current -= inc;
       // if we overshoot set to exact to prevent oscillation
       if (scale_factor_current < scale_factor) scale_factor_current = scale_factor;
   }
}






void mwDisplay::show_bitmap_flags(int flags)
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

void mwDisplay::show_pixel_format(int df)
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

void mwDisplay::show_display_flags(int flags)
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

void mwDisplay::show_display_options(void)
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

void mwDisplay::show_display_orienation(void)
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

void mwDisplay::show_fullscreen_modes(void)
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







void mwDisplay::auto_set_display_transform_double(void)
{
   display_transform_double = 1;

   if (disp_w_curr > 1023) display_transform_double = 2;
   if (disp_h_curr > 1023) display_transform_double = 2;

   if (disp_w_curr < 1024) display_transform_double = 1;
   if (disp_h_curr < 700)  display_transform_double = 1;

//   if (mwPS.level_editor_running) display_transform_double = 1;

   if (mwPS.help_screens_running)
   {
      if (disp_w_curr > 1279) display_transform_double = 2;
      if (disp_w_curr < 1280) display_transform_double = 1;
   }
}




void mwDisplay::set_saved_display_transform(int sdt)
{
   float old_display_transform_double = display_transform_double;

   saved_display_transform_double = sdt;
   save_config();

   set_display_transform();

   float new_display_transform_double = display_transform_double;
   float sfa = new_display_transform_double/old_display_transform_double;
   scale_factor /= sfa;
   scale_factor_current = scale_factor;

   // adjust window positions
   for (int a=0; a<NUM_MW; a++)
      mwWM.mW[a].set_pos(mwWM.mW[a].x1/sfa, mwWM.mW[a].y1/sfa);


}


void mwDisplay::cycle_display_transform(void)
{
   float old_display_transform_double = display_transform_double;

   if (++saved_display_transform_double>display_transform_double_max) saved_display_transform_double = 0;
   save_config();


   set_display_transform();

   float new_display_transform_double = display_transform_double;
   float sfa = new_display_transform_double/old_display_transform_double;
   scale_factor /= sfa;
   scale_factor_current = scale_factor;


   // adjust window positions
   for (int a=0; a<NUM_MW; a++)
      mwWM.mW[a].set_pos(mwWM.mW[a].x1/sfa, mwWM.mW[a].y1/sfa);

}

void mwDisplay::set_display_transform()
{
   if (!saved_display_transform_double) auto_set_display_transform_double();
   else display_transform_double = saved_display_transform_double;

   show_dtd = 80;

   al_set_target_backbuffer(display);
   mwD.SCREEN_W = disp_w_curr/display_transform_double;
   mwD.SCREEN_H = disp_h_curr/display_transform_double;
   ALLEGRO_TRANSFORM trans;
   al_identity_transform(&trans);
   al_orthographic_transform(&trans, 0, 0, -1.0, mwD.SCREEN_W, mwD.SCREEN_H, 1.0);
   al_use_projection_transform(&trans);
   set_map_var();
}

void mwDisplay::show_disp_values(int fs, int disp, int curr, int wind, int full, char *head)
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

   //  printf("0:%4d 0:%4d w:%4d h:%4d - scrn\n", 0, 0, mwD.SCREEN_W, mwD.SCREEN_H);
}




void mwDisplay::show_display_adapters(void)
{
   int num_adapters = al_get_num_video_adapters();
   printf("%d adapters found...\n", num_adapters);

   ALLEGRO_MONITOR_INFO info;
   int i, j;
   for (i=0; i<num_adapters; i++)
   {
      al_get_monitor_info(i, &info);
      printf("Adapter %d: ", i);
      int dpi = al_get_monitor_dpi(i);
      printf("(%d, %d) - (%d, %d) - dpi: %d\n", info.x1, info.y1, info.x2, info.y2, dpi);
      printf("   Available fullscreen display modes:\n");
      for (j=0; j<al_get_num_display_modes(); j++)
      {
         ALLEGRO_DISPLAY_MODE mode;
         al_get_display_mode(j, &mode);
         printf("   Mode %3d: %4d x %4d, %d Hz\n", j, mode.width, mode.height, mode.refresh_rate);
      }
   }
}





int mwDisplay::init_display(void)
{
   scale_factor_inc = 0.03;

   int num_adapters = al_get_num_video_adapters();
   // printf("%d adapters found...\n", num_adapters);

//   ALLEGRO_MONITOR_INFO info;
//   int i, j;
//   for (i = 0; i < num_adapters; i++)
//   {
//      al_get_monitor_info(i, &info);
//      printf("Adapter %d: ", i);
//      int dpi = al_get_monitor_dpi(i);
//      printf("(%d, %d) - (%d, %d) - dpi: %d\n", info.x1, info.y1, info.x2, info.y2, dpi);
//      al_set_new_display_adapter(i);
//      printf("   Available fullscreen display modes:\n");
//      for (j = 0; j < al_get_num_display_modes(); j++)
//      {
//         ALLEGRO_DISPLAY_MODE mode;
//         al_get_display_mode(j, &mode);
//         printf("   Mode %3d: %4d x %4d, %d Hz\n", j, mode.width, mode.height, mode.refresh_rate);
//      }
//   }


   if (display_adapter_num >=  num_adapters) display_adapter_num = 0;
   al_set_new_display_adapter(display_adapter_num);


   //show_fullscreen_modes();

   al_set_new_display_option(ALLEGRO_COLOR_SIZE, 32, ALLEGRO_REQUIRE);


   // this is needed to make opengl 4.3 work the same as windows
   // see visual level select, etc..
   al_set_new_display_option(ALLEGRO_SINGLE_BUFFER, 1, ALLEGRO_REQUIRE);



//   al_set_new_display_option(ALLEGRO_SWAP_METHOD, 1, ALLEGRO_REQUIRE);

  // al_set_new_display_option(ALLEGRO_COLOR_SIZE, 16, ALLEGRO_SUGGEST);
  // al_set_new_display_option(ALLEGRO_VSYNC, 2, ALLEGRO_SUGGEST);







   // show_disp_values(0, 0, 1, 1, 1, "init");
   // check if windowed values are valid
   int th = 100;
   if (disp_x_wind > disp_w_full-th) disp_x_wind = disp_w_full-th;
   if (disp_y_wind > disp_h_full-th) disp_y_wind = disp_h_full-th;
   if (disp_w_wind > disp_w_full) disp_w_wind = disp_w_full;
   if (disp_h_wind > disp_h_full) disp_h_wind = disp_h_full;
   //show_disp_values(0, 0, 1, 1, 1, "pc");
   save_config();


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



   al_set_window_constraints(display, 320, 240, 0, 0);
   al_apply_window_constraints(display, 1);
   save_config();



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



   set_display_transform();
   window_title();

//   show_display_flags(al_get_display_flags(display));
//   show_display_options();
//   show_pixel_format(al_get_display_format(display));
//   show_display_orienation();
//   printf("refresh rate:%d\n", al_get_display_refresh_rate(display));

   //printf("init screen\n");
   mwB.create_bitmaps();
   make_palette();
   return 1;
}





void mwDisplay::proc_display_change(void)
{
   al_acknowledge_resize(display);                              // important that this is here, later and it does not work as intended
   al_get_window_position(display, &disp_x_curr, &disp_y_curr); // set my local variables with the system ones
   disp_w_curr = al_get_display_width(display);
   disp_h_curr = al_get_display_height(display);
   if (fullscreen)
   {
      disp_x_full = disp_x_curr;
      disp_y_full = disp_y_curr;
      disp_w_full = disp_w_curr;
      disp_h_full = disp_h_curr;
   }
   else
   {
      disp_x_wind = disp_x_curr;
      disp_y_wind = disp_y_curr;
      disp_w_wind = disp_w_curr;
      disp_h_wind = disp_h_curr;
   }
   set_display_transform();
   mwB.rebuild_bitmaps();
   save_config();
   //show_disp_values(0, 1, 1, 1, 0, "get var and process_screen_change end");
   window_title();

}


void mwDisplay::save_display_window_position(void)
{
   // this is automatically done when display is resized or switched to fullscreen
   // but not when the window is just moved
   // so I call this when exiting

   if (!fullscreen)
   {
      al_get_window_position(display, &disp_x_wind, &disp_y_wind);
      disp_w_wind = al_get_display_width(display);
      disp_h_wind = al_get_display_height(display);
      save_config();
   }
}


void mwDisplay::toggle_fullscreen(void)
{
   if (fullscreen) proc_display_change_fromfs();
   else            proc_display_change_tofs();
}

void mwDisplay::proc_display_change_tofs(void)
{
   //printf("\n-----------to fullscreen------------\n");
   fullscreen = 1;

   // save window position and size before entering fullscreen
   al_get_window_position(display, &disp_x_wind, &disp_y_wind);
   disp_w_wind = al_get_display_width(display);
   disp_h_wind = al_get_display_height(display);

   al_set_display_flag(display, ALLEGRO_FULLSCREEN_WINDOW, fullscreen);

   // here is one of the few places I will set xywh from my local variables
//   al_resize_display(display, disp_w_full, disp_h_full);
//   al_set_window_position(display, disp_x_full, disp_y_full);
   proc_display_change();
}

void mwDisplay::proc_display_change_fromfs(void)
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

