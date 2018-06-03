// zscrn.cpp

#include "pm.h"

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
   if (flags & ALLEGRO_WINDOWED) printf("ALLEGRO_WINDOWED\n");
   if (flags & ALLEGRO_FULLSCREEN) printf("ALLEGRO_FULLSCREEN\n");
   if (flags & ALLEGRO_OPENGL) printf("ALLEGRO_OPENGL\n");
   if (flags & ALLEGRO_DIRECT3D_INTERNAL) printf("ALLEGRO_DIRECT3D_INTERNAL\n");
   if (flags & ALLEGRO_RESIZABLE) printf("ALLEGRO_RESIZABLE\n");
   if (flags & ALLEGRO_FRAMELESS) printf("ALLEGRO_FRAMELESS\n");
   if (flags & ALLEGRO_GENERATE_EXPOSE_EVENTS) printf("ALLEGRO_GENERATE_EXPOSE_EVENTS\n");
   if (flags & ALLEGRO_OPENGL_3_0) printf("ALLEGRO_OPENGL_3_0\n");
   if (flags & ALLEGRO_OPENGL_FORWARD_COMPATIBLE) printf("ALLEGRO_OPENGL_FORWARD_COMPATIBLE\n");
   if (flags & ALLEGRO_FULLSCREEN_WINDOW) printf("ALLEGRO_FULLSCREEN_WINDOW\n");
   if (flags & ALLEGRO_MINIMIZED) printf("ALLEGRO_MINIMIZED\n");
   if (flags & ALLEGRO_PROGRAMMABLE_PIPELINE) printf("ALLEGRO_PROGRAMMABLE_PIPELINE\n");
   if (flags & ALLEGRO_GTK_TOPLEVEL_INTERNAL) printf("ALLEGRO_GTK_TOPLEVEL_INTERNAL\n");
   if (flags & ALLEGRO_MAXIMIZED) printf("ALLEGRO_MAXIMIZED\n");
   if (flags & ALLEGRO_OPENGL_ES_PROFILE) printf("ALLEGRO_OPENGL_ES_PROFILE\n");
}

void show_display_options(void)
{
   printf("display options\n");
   printf("ALLEGRO_RED_SIZE:%d\n", al_get_display_option(display, ALLEGRO_RED_SIZE));
   printf("ALLEGRO_GREEN_SIZE:%d\n", al_get_display_option(display, ALLEGRO_GREEN_SIZE));
   printf("ALLEGRO_BLUE_SIZE:%d\n", al_get_display_option(display, ALLEGRO_BLUE_SIZE));
   printf("ALLEGRO_ALPHA_SIZE:%d\n", al_get_display_option(display, ALLEGRO_ALPHA_SIZE));
   printf("ALLEGRO_RED_SHIFT:%d\n", al_get_display_option(display, ALLEGRO_RED_SHIFT));
   printf("ALLEGRO_GREEN_SHIFT:%d\n", al_get_display_option(display, ALLEGRO_GREEN_SHIFT));
   printf("ALLEGRO_BLUE_SHIFT:%d\n", al_get_display_option(display, ALLEGRO_BLUE_SHIFT));
   printf("ALLEGRO_ALPHA_SHIFT:%d\n", al_get_display_option(display, ALLEGRO_ALPHA_SHIFT));
   printf("ALLEGRO_ACC_RED_SIZE:%d\n", al_get_display_option(display, ALLEGRO_ACC_RED_SIZE));
   printf("ALLEGRO_ACC_GREEN_SIZE:%d\n", al_get_display_option(display, ALLEGRO_ACC_GREEN_SIZE));
   printf("ALLEGRO_ACC_BLUE_SIZE:%d\n", al_get_display_option(display, ALLEGRO_ACC_BLUE_SIZE));
   printf("ALLEGRO_ACC_ALPHA_SIZE:%d\n", al_get_display_option(display, ALLEGRO_ACC_ALPHA_SIZE));
   printf("ALLEGRO_STEREO:%d\n", al_get_display_option(display, ALLEGRO_STEREO));
   printf("ALLEGRO_AUX_BUFFERS:%d\n", al_get_display_option(display, ALLEGRO_AUX_BUFFERS));
   printf("ALLEGRO_COLOR_SIZE:%d\n", al_get_display_option(display, ALLEGRO_COLOR_SIZE));
   printf("ALLEGRO_DEPTH_SIZE:%d\n", al_get_display_option(display, ALLEGRO_DEPTH_SIZE));
   printf("ALLEGRO_STENCIL_SIZE:%d\n", al_get_display_option(display, ALLEGRO_STENCIL_SIZE));
   printf("ALLEGRO_SAMPLE_BUFFERS:%d\n", al_get_display_option(display, ALLEGRO_SAMPLE_BUFFERS));
   printf("ALLEGRO_SAMPLES:%d\n", al_get_display_option(display, ALLEGRO_SAMPLES));
   printf("ALLEGRO_RENDER_METHOD:%d\n", al_get_display_option(display, ALLEGRO_RENDER_METHOD));
   printf("ALLEGRO_FLOAT_COLOR:%d\n", al_get_display_option(display, ALLEGRO_FLOAT_COLOR));
   printf("ALLEGRO_FLOAT_DEPTH:%d\n", al_get_display_option(display, ALLEGRO_FLOAT_DEPTH));
   printf("ALLEGRO_SINGLE_BUFFER:%d\n", al_get_display_option(display, ALLEGRO_SINGLE_BUFFER));
   printf("ALLEGRO_SWAP_METHOD:%d\n", al_get_display_option(display, ALLEGRO_SWAP_METHOD));
   printf("ALLEGRO_COMPATIBLE_DISPLAY:%d\n", al_get_display_option(display, ALLEGRO_COMPATIBLE_DISPLAY));
   printf("ALLEGRO_UPDATE_DISPLAY_REGION:%d\n", al_get_display_option(display, ALLEGRO_UPDATE_DISPLAY_REGION));
   printf("ALLEGRO_VSYNC:%d\n", al_get_display_option(display, ALLEGRO_VSYNC));
   printf("ALLEGRO_MAX_BITMAP_SIZE:%d\n", al_get_display_option(display, ALLEGRO_MAX_BITMAP_SIZE));
   printf("ALLEGRO_SUPPORT_NPOT_BITMAP:%d\n", al_get_display_option(display, ALLEGRO_SUPPORT_NPOT_BITMAP));
   printf("ALLEGRO_SUPPORT_SEPARATE_ALPHA:%d\n", al_get_display_option(display, ALLEGRO_SUPPORT_SEPARATE_ALPHA));
   printf("ALLEGRO_AUTO_CONVERT_BITMAPS:%d\n", al_get_display_option(display, ALLEGRO_AUTO_CONVERT_BITMAPS));
   printf("ALLEGRO_SUPPORTED_ORIENTATIONS:%d\n", al_get_display_option(display, ALLEGRO_SUPPORTED_ORIENTATIONS));
   printf("ALLEGRO_OPENGL_MAJOR_VERSION:%d\n", al_get_display_option(display, ALLEGRO_OPENGL_MAJOR_VERSION));
   printf("ALLEGRO_OPENGL_MINOR_VERSION:%d\n", al_get_display_option(display, ALLEGRO_OPENGL_MINOR_VERSION));
   printf("ALLEGRO_DISPLAY_OPTIONS_COUNT:%d\n", al_get_display_option(display, ALLEGRO_DISPLAY_OPTIONS_COUNT));
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

// done at start only now
void create_bmp(void)
{
   // create tilemap bitmaps
   al_set_new_bitmap_format(ALLEGRO_PIXEL_FORMAT_ANY_32_WITH_ALPHA);
   al_set_new_bitmap_flags(ALLEGRO_NO_PRESERVE_TEXTURE | ALLEGRO_VIDEO_BITMAP);
   tilemap = al_create_bitmap(640, 640);
   ptilemap = al_create_bitmap(380,320);
   dtilemap = al_create_bitmap(160,640);

   // create memory bitmaps as temp storage for restoring tilemaps after screen change
   al_set_new_bitmap_flags(ALLEGRO_MEMORY_BITMAP);
   M_tilemap = al_create_bitmap(640,640);
   M_ptilemap = al_create_bitmap(380,320);
   M_dtilemap = al_create_bitmap(160,640);


//   if (tilemap == NULL) printf("failed to create tilemap\n");
//   printf("tilemap\n");
//   show_pixel_format(al_get_bitmap_format(tilemap));
//   show_bitmap_flags(al_get_bitmap_flags(tilemap));


   // create level_background and level_buffer bitmaps
   al_set_new_bitmap_format(ALLEGRO_PIXEL_FORMAT_ANY_16_NO_ALPHA);
   al_set_new_bitmap_flags(ALLEGRO_NO_PRESERVE_TEXTURE | ALLEGRO_VIDEO_BITMAP);
   level_background = al_create_bitmap(2000,2000);
   level_buffer = al_create_bitmap(2000,2000);

//   printf("level_background\n");
//   show_pixel_format(al_get_bitmap_format(level_background));
//   show_bitmap_flags(al_get_bitmap_flags(level_background));


//   printf("M_tilemap\n");
//   show_pixel_format(al_get_bitmap_format(M_tilemap));
//   show_bitmap_flags(al_get_bitmap_flags(M_tilemap));


   // reset defaults to use for new bitmaps
   al_set_new_bitmap_format(ALLEGRO_PIXEL_FORMAT_ANY_32_WITH_ALPHA);
   al_set_new_bitmap_flags(ALLEGRO_NO_PRESERVE_TEXTURE | ALLEGRO_VIDEO_BITMAP);

   // temp bitmap for passing draw shape
   dtemp = al_create_bitmap(20,20);

}



void load_fonts(void)
{
//   al_destroy_font(font);
//   font = al_create_builtin_font();
//   if(!font)
//   {
//      sprintf(msg, "Failed to create_builtin_font\n");
//      printf("%s", msg);
//      font = al_load_bitmap_font("bitmaps/a4_font.bmp");
//      if(!font)
//      {
//         sprintf(msg, "Failed to load font from bitmaps/a4_font.bmp\n");
//         printf("%s", msg);
//         m_err(msg);
//      }
//      //else printf("loaded base font from bitmap file\n");
//   }
//   //else printf("created builtin font\n");



   al_destroy_font(font);
   font = al_create_builtin_font();
   if(!font)
   {
      sprintf(msg, "Failed to create_builtin_font\n");
      printf("%s", msg);
      m_err(msg);
      exit(0);
   }
   //else printf("created builtin font\n");



   al_destroy_font(f1);
   f1 = al_load_ttf_font("bitmaps/Achafont.ttf", 240, 0);
   if(!f1)
   {
      sprintf(msg, "Failed to load font from bitmaps/Achafont.ttf\n");
      m_err(msg);
   }
   //else printf("loaded font Achafont.ttf\n");

   int sfs = al_get_display_option(display, ALLEGRO_MAX_BITMAP_SIZE) / 20;
   //printf("Saucer font size:%d\n", sfs);
   al_destroy_font(f2);
   f2 = al_load_ttf_font("bitmaps/SaucerBB.ttf", sfs, 0);
   if(!f2)
   {
      sprintf(msg, "Failed to load font from bitmaps/SaucerBB.ttf\n");
      m_err(msg);
   }
   //else printf("loaded font SaucerBB.ttf.ttf\n");

   al_destroy_font(f3);
   f3 = al_load_ttf_font("bitmaps/PixelGosub.otf", 7, ALLEGRO_TTF_MONOCHROME);
   if(!f3)
   {
      sprintf(msg, "Failed to load font from bitmaps/PixelGosub.otf\n");
      m_err(msg);
   }
   //else printf("loaded font PixelGosub.otf\n");

}

void auto_set_display_transform_double(void)
{
   display_transform_double = 1;

   if (disp_w_curr > 1023) display_transform_double = 2;
   if (disp_h_curr > 1023) display_transform_double = 2;

   if (disp_w_curr < 1024) display_transform_double = 1;
   if (disp_h_curr < 700) display_transform_double = 1;

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


void rebuild_bitmaps(void)
{
   //printf("rebuild bitmaps\n");

   // rebuild main tiles
   al_set_target_bitmap(tilemap);
   al_draw_bitmap(M_tilemap, 0, 0, 0);

   // rebuild player tiles
   al_set_target_bitmap(ptilemap);
   al_draw_bitmap(M_ptilemap, 0, 0, 0);

   // rebuild door tiles
   al_set_target_bitmap(dtilemap);
   al_draw_bitmap(M_dtilemap, 0, 0, 0);

   init_level_background();

   set_display_transform();

   load_fonts();

   logo_text_bitmaps_create = 1;
   text_demomode_bitmaps_create = 1;
   text_title_bitmaps_create = 1;

   set_map_var();
   Redraw = 1;

   load_visual_level_select_done = 0;
   if (visual_level_select_running) load_visual_level_select();
}

void show_disp_values(void)
{
   printf("fullscreen:%d display_transform_double:%d\n", fullscreen, display_transform_double);
   printf("x:%4d y:%4d w:%4d h:%4d - curr\n", disp_x_curr, disp_y_curr, disp_w_curr, disp_h_curr);
   printf("x:%4d y:%4d w:%4d h:%4d - wind\n", disp_x_wind, disp_y_wind, disp_w_wind, disp_h_wind);
   int x, y;
   al_get_window_position(display, &x, &y);
   int w = al_get_display_width(display);
   int h = al_get_display_height(display);
   printf("x:%4d y:%4d w:%4d h:%4d - disp\n", x, y, w, h);
   printf("0:%4d 0:%4d w:%4d h:%4d - scrn\n", 0, 0, SCREEN_W, SCREEN_H);
}

void proc_screen_change(int new_sw, int new_sh, int new_sx, int new_sy, int new_fs)
{
//   printf("\nDisplay change\n");
//   printf("x:%4d y:%4d w:%4d h:%4d fs:%d  old\n", disp_x_curr, disp_y_curr, disp_w_curr, disp_h_curr, fullscreen);
//   printf("x:%4d y:%4d w:%4d h:%4d fs:%d  new\n", new_sx, new_sy, new_sw, new_sh, new_fs);

   if ((!fullscreen) && (new_fs)) //  changing to fullscreen
   {
      fullscreen = 1;
      al_set_display_flag(display, ALLEGRO_FULLSCREEN_WINDOW, 1);
      al_get_window_position(display, &disp_x_curr, &disp_y_curr);
      disp_w_curr = al_get_display_width(display);
      disp_h_curr = al_get_display_height(display);
      auto_set_display_transform_double();
      rebuild_bitmaps();
      save_config();
      Redraw = 1;
      //printf("changed to fullscreen\n");
      //show_disp_values();
   }
   else if ((fullscreen) && (!new_fs)) //  changing away from fullscreen
   {
      fullscreen = 0;
      al_resize_display(display, disp_w_wind, disp_h_wind);
      al_set_display_flag(display, ALLEGRO_FULLSCREEN_WINDOW, 0);
      al_set_window_position(display, disp_x_wind, disp_y_wind);

      al_get_window_position(display, &disp_x_curr, &disp_y_curr);
      disp_w_curr = al_get_display_width(display);
      disp_h_curr = al_get_display_height(display);
      auto_set_display_transform_double();
      rebuild_bitmaps();
      save_config();
      Redraw = 1;
      //window_title();
      //printf("changed away from fullscreen\n");
      //show_disp_values();
   }

   else if (!fullscreen)
   {
      if ((new_sw != disp_w_curr) || (new_sh != disp_h_curr)) // window has been resized
      {
         al_acknowledge_resize(display);
         disp_w_curr = disp_w_wind = al_get_display_width(display);
         disp_h_curr = disp_h_wind = al_get_display_height(display);
         auto_set_display_transform_double();
         rebuild_bitmaps();
         save_config();
         Redraw = 1;
         //window_title();
         //printf("window resized\n");
         //show_disp_values();
     }
     if ((new_sx != disp_x_curr) || (new_sy != disp_y_curr)) // window has been moved
     {
        disp_x_curr = disp_x_wind = new_sx;;
        disp_y_curr = disp_y_wind = new_sy;;
        save_config();
        //printf("window moved\n");
        //show_disp_values();
     }
  }
}




int init_screen(void)
{
   //show_fullscreen_modes();

   //al_set_new_display_option(ALLEGRO_COLOR_SIZE, 32, ALLEGRO_REQUIRE);
   al_set_new_display_option(ALLEGRO_COLOR_SIZE, 16, ALLEGRO_SUGGEST);
//   al_set_new_display_option(ALLEGRO_VSYNC, 2, ALLEGRO_SUGGEST);

   int flags = ALLEGRO_WINDOWED | ALLEGRO_RESIZABLE;
   if (fullscreen) flags = ALLEGRO_FULLSCREEN_WINDOW | ALLEGRO_RESIZABLE;

//   int flags = ALLEGRO_WINDOWED | ALLEGRO_RESIZABLE | ALLEGRO_OPENGL ;
//   if (fullscreen) flags = ALLEGRO_WINDOWED | ALLEGRO_RESIZABLE | ALLEGRO_FRAMELESS | ALLEGRO_OPENGL;

//   int flags = ALLEGRO_WINDOWED | ALLEGRO_RESIZABLE | ALLEGRO_OPENGL | ALLEGRO_OPENGL_3_0;
//   if (fullscreen) flags = ALLEGRO_WINDOWED | ALLEGRO_RESIZABLE | ALLEGRO_FRAMELESS | ALLEGRO_OPENGL | ALLEGRO_OPENGL_3_0;


   al_set_new_display_flags(flags);
   display = al_create_display(disp_w_wind, disp_h_wind);//
   if(!display)
   {
      sprintf(msg, "Error creating display\n");
      m_err(msg);
      exit(0);
   }
   //else printf("created display\n");

   if (!fullscreen)
   {
      disp_x_curr = disp_x_wind;
      disp_y_curr = disp_y_wind;
      al_set_window_position(display, disp_x_curr, disp_y_curr);
   }

   disp_w_curr = al_get_display_width(display);
   disp_h_curr = al_get_display_height(display);
   al_get_window_position(display, &disp_x_curr, &disp_y_curr);
   //printf("x:%d y:%d w:%d h:%4d\n", disp_x_curr, disp_y_curr, disp_w_curr, disp_h_curr);

   auto_set_display_transform_double();
   window_title();

   //show_display_flags(al_get_display_flags(display));
   //show_display_options();
//   show_pixel_format(al_get_display_format(display));
//   show_display_orienation();
//   printf("refresh rate:%d\n", al_get_display_refresh_rate(display));

   //printf("init screen\n");
   //show_disp_values();
   create_bmp();
   return 1;
}

void get_new_background(int full)
{
   al_set_target_bitmap(level_buffer);
   if (full) al_draw_bitmap(level_background, 0, 0, 0);
   else
   {
      // this only grabs the visible region, in the interests of speed
      int x = level_display_region_x - 20; if (x < 0) x = 0;
      int y = level_display_region_y - 20; if (y < 0) y = 0;
      int w = level_display_region_w + 40; if (x+w > 2000) w = 2000-x;
      int h = level_display_region_h + 40; if (y+h > 2000) h = 2000-y;
      al_draw_bitmap_region(level_background, x, y, w, h, x, y, 0);
   }
}

void stimp(void) // transition from menu to game
{
   draw_level2(NULL, 0, 0, 0, 1, 1, 1, 1, 1); // redraw entire level in case only region has been drawn
   al_set_target_backbuffer(display);

   int num_steps = 40;
   float delay = .01;

   // find the size of the source screen from actual screen size and scaler
   int bw = BORDER_WIDTH;
   int SW = (int)( (float)(SCREEN_W - bw *2) / scale_factor_current);
   int SH = (int)( (float)(SCREEN_H - bw *2) / scale_factor_current);

   if (SW > 2000) SW = 2000;
   if (SH > 2000) SH = 2000;

   // find where to grab the source screen from based on the players position
   int alp = active_local_player;
   int PX = al_fixtoi(players[alp].PX) + 10;
   int PY = al_fixtoi(players[alp].PY) + 10;

   // this method has a hysteresis rectangle in the middle of the screen where there is no scroll
   int x_size = SW / 18; // larger number is smaller window
   int y_size = SH / 18;
   if (WX < PX - SW/2 - x_size) WX = PX - SW/2 - x_size;
   if (WX > PX - SW/2 + x_size) WX = PX - SW/2 + x_size;
   if (WY < PY - SH/2 - y_size) WY = PY - SH/2 - y_size;
   if (WY > PY - SH/2 + y_size) WY = PY - SH/2 + y_size;

   // correct for edges
   if (WX < 0) WX = 0;
   if (WY < 0) WY = 0;
   if (WX > (2000 - SW)) WX = 2000 - SW;
   if (WY > (2000 - SH)) WY = 2000 - SH;

   // this is where the player will be when stimp is done and the level starts
   PX = al_fixtoi(players[alp].PX);
   PY = al_fixtoi(players[alp].PY);

   float px_final = (PX-WX) * scale_factor_current + bw;
   float py_final = (PY-WY) * scale_factor_current + bw;


   // offset if entire level is smaller than screen
   int sbw = SCREEN_W-bw*2;
   int sbh = SCREEN_H-bw*2;

   // how big is the entire level after scale factor is applied?
   int sls = (int) ((float)2000 * scale_factor_current); // sls = scaled level size

   // is the entire level smaller than the screen buffer width?
   if (sls < sbw)
   {
      int a = sbw - sls; // how much smaller?
      px_final += a/2;

   }
   // is the entire level smaller than the screen buffer height?
   if (sls < sbh)
   {
      int a = sbh - sls; // how much smaller?
      py_final += a/2;
   }



   // this is the menu map's position and size
   y_size = SCREEN_H-160;
   x_size = SCREEN_W-260;
   int size;
   if (y_size < x_size) size = y_size;
   else size = x_size;
   if (size < 10) size = 10;
   int mx = SCREEN_W/2-(size/2);
   int my = 140;

   // get the players position on the menu map
   int map_px = mx + PX * size / 2000;
   int map_py = my + PY * size / 2000;


   float sc = (float)size / 2000;
   float scf = scale_factor_current;
   // do the scale increment as a percent change so that the zoom speed is constant
   // use the compound interest formula to find the percent change per step I need
   float a, per = 1.000;
   do
   {
       per += .0001;
       a = sc * pow(per, num_steps);
   }
   while (a < scf);

   float fmx = (float)map_px;
   float fmxf = px_final;
   float fmxinc = (fmxf - fmx) / num_steps;

   float fmy = (float)map_py;
   float fmyf = py_final;
   float fmyinc = (fmyf - fmy) / num_steps;

   for (int steps = 0; steps<num_steps; steps++)
   {
      al_clear_to_color(al_map_rgb(0,0,0));
      draw_level_centered((int)fmx, (int)fmy, PX, PY,  sc);
      al_flip_display();
      sc *= per;
      fmx += fmxinc;
      fmy += fmyinc;
      al_rest(delay);
   }
}


void stamp(void) // transition from game to menu
{
   draw_level2(NULL, 0, 0, 0, 1, 1, 1, 1, 1); // redraw entire level in case only region has been drawn

   int num_steps = 40;
   float delay = .01;

   // find the size of the source screen from actual screen size and scaler
   int bw = BORDER_WIDTH;
   int SW = (int)( (float)(SCREEN_W - bw *2) / scale_factor_current);
   int SH = (int)( (float)(SCREEN_H - bw *2) / scale_factor_current);
   if (SW > 2000) SW = 2000;
   if (SH > 2000) SH = 2000;

   // find where to grab the source screen from based on the players position
   int alp = active_local_player;
   int PX = al_fixtoi(players[alp].PX) + 10;
   int PY = al_fixtoi(players[alp].PY) + 10;

   // this method has a hysteresis rectangle in the middle of the screem where there is no scroll
   int x_size = SW / 18; // larger number is smaller window
   int y_size = SH / 18;

   if (WX < PX - SW/2 - x_size) WX = PX - SW/2 - x_size;
   if (WX > PX - SW/2 + x_size) WX = PX - SW/2 + x_size;
   if (WY < PY - SH/2 - y_size) WY = PY - SH/2 - y_size;
   if (WY > PY - SH/2 + y_size) WY = PY - SH/2 + y_size;

   // correct for edges
   if (WX < 0) WX = 0;
   if (WY < 0) WY = 0;
   if (WX > (2000 - SW)) WX = 2000 - SW;
   if (WY > (2000 - SH)) WY = 2000 - SH;

   // this is where the player will be when stimp is done and the level starts
   PX = al_fixtoi(players[alp].PX);
   PY = al_fixtoi(players[alp].PY);

   float px_final = (PX-WX) * scale_factor_current + bw;
   float py_final = (PY-WY) * scale_factor_current + bw;

   // offset if entire level is smaller than screen
   int sbw = SCREEN_W-bw*2;
   int sbh = SCREEN_H-bw*2;

   // how big is the entire level after scale factor is applied?
   int sls = (int) ((float)2000 * scale_factor_current); // sls = scaled level size

   // is the entire level smaller than the screen buffer width?
   if (sls < sbw)
   {
      int a = sbw - sls; // how much smaller?
      px_final += a/2;

   }
   // is the entire level smaller than the screen buffer height?
   if (sls < sbh)
   {
      int a = sbh - sls; // how much smaller?
      py_final += a/2;
   }







   // this is the menu map's position and size
   y_size = SCREEN_H-160;
   x_size = SCREEN_W-260;
   int size;
   if (y_size < x_size) size = y_size;
   else size = x_size;
   if (size < 10) size = 10;
   int mx = SCREEN_W/2-(size/2);
   int my = 140;

   // get the players position on the menu map
   int map_px = mx + PX * size / 2000;
   int map_py = my + PY * size / 2000;

   float sc = scale_factor_current;
   float scf = (float)size / 2000;

   // do the scale increment as a percent change so that the zoom speed is constant
   // use the compound interest formula to find the percent change per step I need
   float a, per = 1.000;
   do
   {
       per -= .0001;
       a = sc * pow(per, num_steps);
   }
   while (a > scf);

   float fmx = px_final;
   float fmxf = (float)map_px;
   float fmxinc = (fmxf - fmx) / num_steps;

   float fmy = py_final;
   float fmyf = (float)map_py;
   float fmyinc = (fmyf - fmy) / num_steps;

   for (int steps = 0; steps<num_steps; steps++)
   {
      al_clear_to_color(al_map_rgb(0,0,0));
      draw_level_centered((int)fmx, (int)fmy, PX, PY,  sc);
      al_flip_display();
      sc *= per;
      fmx += fmxinc;
      fmy += fmyinc;
      al_rest(delay);
   }
}


void get_new_screen_buffer(void)
{
   al_set_target_backbuffer(display);
   al_clear_to_color(al_map_rgb(0,0,0));

   int alp = active_local_player;
   int c = players[alp].color;

   // draw frame in local player's color
   for (int x = 0; x < BORDER_WIDTH; x++)
      al_draw_rectangle(x+0.5f, x+0.5f, (SCREEN_W-1-x)+0.5f, (SCREEN_H-1-x)+0.5f,  palette_color[c + (x * 16)], 1);

   // default place and size to draw on screen_buffer
   int bw = BORDER_WIDTH;
   int sbx = bw;
   int sby = bw;
   int sbw = SCREEN_W-bw*2;
   int sbh = SCREEN_H-bw*2;

   // how big is the entire level after scale factor is applied?
   int sls = (int) ((float)2000 * scale_factor_current); // sls = scaled level size

   // is the entire level smaller than the screen buffer width?
   if (sls < sbw)
   {
      int a = sbw - sls; // how much smaller?
      sbw = sls;         // new screen_buffer blit width = sls
      sbx += a/2;        // new screen_buffer blit xpos
   }

   // is the entire level smaller than the screen buffer height?
   if (sls < sbh)
   {
      int a = sbh - sls; // how much smaller?
      sbh = sls;         // new screen_buffer blit height = sls
      sby += a/2;        // new screen_buffer blit ypos
   }

   // find the size of the source screen from actual screen size and scaler
   int SW = (int)( (float)(SCREEN_W - bw *2) / scale_factor_current);
   int SH = (int)( (float)(SCREEN_H - bw *2) / scale_factor_current);
   if (SW > 2000) SW = 2000;
   if (SH > 2000) SH = 2000;

   // find where to grab the source screen from based on the players position
   int PX = al_fixtoi(players[alp].PX) + 10;
   int PY = al_fixtoi(players[alp].PY) + 10;

   // this method always has the player in the middle of the screen
   //int WX = PX - SW/2 -10; // set window from PX, PY
   //int WY = PY - SH/2 -10;

   // set the scroll hysteresis (a rectangle in the middle of the screen where there is no scroll)
   int x_size = SW / 8; // larger number is smaller window
   int y_size = SH / 12;

   if (WX < PX - SW/2 - x_size) WX = PX - SW/2 - x_size; // hit right edge
   if (WX > PX - SW/2 + x_size) WX = PX - SW/2 + x_size; // hit left edge
   if (WY < PY - SH/2 - y_size) WY = PY - SH/2 - y_size; // hit bottom edge
   if (WY > PY - SH/2 + y_size) WY = PY - SH/2 + y_size; // hit top edge

   // correct for edges
   if (WX < 0) WX = 0;
   if (WY < 0) WY = 0;
   if (WX > (2000 - SW)) WX = 2000 - SW;
   if (WY > (2000 - SH)) WY = 2000 - SH;

   // used by get_new_background to only get what is needed
   level_display_region_x = WX;
   level_display_region_y = WY;
   level_display_region_w = SW;
   level_display_region_h = SH;

   // this is what all the previous calculation have been building up to:
   al_draw_scaled_bitmap(level_buffer, WX, WY, SW, SH, sbx, sby, sbw, sbh, 0);

   //printf("WX:%d, WY:%d, SW:%d, SH:%d, sbx:%d, sby:%d, sbw:%d, sbh:%d\n", WX, WY, SW, SH, sbx, sby, sbw, sbh);


   // where is the player on the screen buffer ??
   // needed to see if the map is covering player
//   float psbx = (PX-WX-10) * scale_factor_current;
//   float psby = (PY-WY-10) * scale_factor_current;
//   float fsz = 20 * scale_factor_current;
//
//   // convert to int
//   int px = (int) psbx+bw;
//   int py = (int) psby+bw;
//   int sz = (int) fsz;
//
//   // show rect around player to make sure I've got it
//   //al_draw_rectangle(px+1, py+1, px+sz+1, py+sz+1, palette_color[10], 1);
//
//   // save in player struct
//   players1[alp].sbx1 = px;
//   players1[alp].sby1 = py;
//   players1[alp].sbx2 = px+sz;
//   players1[alp].sby2 = py+sz;
//
//   // show hysteresis window
//   float hx1 = SCREEN_W/2 - x_size * scale_factor_current;
//   float hx2 = SCREEN_W/2 + x_size * scale_factor_current;
//   float hy1 = SCREEN_H/2 - y_size * scale_factor_current;
//   float hy2 = SCREEN_H/2 + y_size * scale_factor_current;
//   al_draw_rectangle(hx1, hy1, hx2, hy2, palette_color[10], 2);


}

void set_map_var(void)
{
   // set intial map size based on minimum screen dimension / 3
   int smin = 0;
   if (SCREEN_H < SCREEN_W) smin = SCREEN_H;
   else smin = SCREEN_W;
   new_size = smin / 3;
   map_size = new_size;

   // set db and create map for level editor
   db = (SCREEN_H/100);
   if (db < 4) db = 4;
   if (db > 20) db = 20;
   // check to see if there is enough space for buttons on the right panel...if not, re-adjust
   while (SCREEN_W - db*100 < 200) db--;
   if (db < 2) db = 2;
   al_destroy_bitmap(lefsm);
   lefsm = al_create_bitmap(db*100,db*100);

   // center of right side panel in level editor
   txc = SCREEN_W - (SCREEN_W - db*100) / 2;

   // check that status and select windows are not off screen
   check_s_window_pos(1);

   // create a bitmap to use as a mouse pointer
   mp = al_create_bitmap(db*25,db*25);

   // determine menu_map_size and position
   int y_size = SCREEN_H-160;
   int x_size = SCREEN_W-260;
   if (y_size < x_size) menu_map_size = y_size;
   else menu_map_size = x_size;
   if (menu_map_size < 10) menu_map_size = 10;
   menu_map_x = SCREEN_W/2-(menu_map_size/2);
   menu_map_y = 140;

   // splash screen logo position
   mdw_splash_logo_x = SCREEN_W/2;
   mdw_splash_logo_y = SCREEN_H/2;

   // splash screen logo size
   int min_d = SCREEN_H;  // find miniumum dimension
   if (SCREEN_W < SCREEN_H) min_d = SCREEN_W;
   mdw_splash_logo_scale = (float) min_d / 500; // 400 is the exact size, make it bigger for padding
   mdw_splash_logo_th = 2;

   // map screen logo position and size
   int sp = menu_map_x - BORDER_WIDTH;    // how much space do I have between the map and the screen edge?
   mdw_map_logo_scale = (float) sp / 500; // 400 is the exact size, make it bigger for padding
   mdw_map_logo_th = mdw_splash_logo_th;  // same thickness as splash
   mdw_map_logo_x = BORDER_WIDTH + sp/2;
   mdw_map_logo_y = menu_map_y + (int) (mdw_map_logo_scale * 200); // align top of logo with top of map

   // this is the link from splash to map
   mdw_logo_scale_dec = (mdw_splash_logo_scale - mdw_map_logo_scale) / 320;
   mdw_logo_x_dec = (mdw_splash_logo_x - (float)mdw_map_logo_x) / 320;
   mdw_logo_y_dec = (mdw_splash_logo_y - (float)mdw_map_logo_y) / 320;

//   printf("slx %f sly %f\n", mdw_splash_logo_x, mdw_splash_logo_y );
//   printf("mlx %d mly %d\n", mdw_map_logo_x, mdw_map_logo_y );
//   printf("xdec %f ydec %f\n", mdw_logo_x_dec, mdw_logo_y_dec );
//   printf("xdec %f ydec %f\n", mdw_logo_x_dec*320, mdw_logo_y_dec*320 );
}

void set_scale_factor(int instant)
{
   // enforce max and min
   if (scale_factor < .2) scale_factor = .2;
   if (scale_factor > 40) scale_factor = 40;
   save_config();

   if (instant) scale_factor_current = scale_factor;
   show_scale_factor = 80;
}

void init_level_background(void) // fill level_background with blocks and lift lines
{
   //printf("init_level_background\n");
   al_set_target_bitmap(level_background);
   al_clear_to_color(al_map_rgb(0,0,0));
   for (int x=0; x<100; x++)
      for (int y=0; y<100; y++)
      {
         int c = l[x][y];
         if (c < NUM_SPRITES)
            al_draw_bitmap(tile[c], x*20, y*20, 0);
      }
   draw_lift_lines();
}

void draw_level2(ALLEGRO_BITMAP *b, int mx, int my, int ms, int blocks, int items, int enemies, int lifts, int players)
{
   if (blocks) get_new_background(1);
   if (valid_level_loaded)
   {
      if (lifts)   draw_lifts();
      if (items)   draw_items();
      if (enemies) draw_enemy();
      if (resume_allowed)
      {
         if (players) draw_players();
         draw_ebullets();
         draw_pbullets();
      }
   }
   if (b == NULL) al_set_target_backbuffer(display);
   else al_set_target_bitmap(b);
   al_draw_scaled_bitmap(level_buffer, 0, 0, 2000, 2000, mx, my, ms, ms, 0);
}

void draw_level_centered(int screen_x, int screen_y, int level_x, int level_y, float scale_factor)
{
   // use scale factor to determine scaled size of level
   int size = (int)(scale_factor * 2000);
   int mgx = screen_x - (int) (level_x * scale_factor);  // start x pos on level
   int mgy = screen_y - (int) (level_y * scale_factor);  // start y pos on level
   al_draw_scaled_bitmap(level_buffer, 0, 0, 2000, 2000, mgx, mgy, size, size, 0);
}

void draw_level(void) // draws the map on the menu screen
{
   int blocks = 0;
   if (valid_level_loaded) blocks = 1;

   draw_level2(NULL, menu_map_x, menu_map_y, menu_map_size, blocks, 1, 1, 1, 1);


   // show level data
   int y_pos = 140;
   int x_pos = SCREEN_W/2+(menu_map_size/2);
   if (valid_level_loaded)
   {
      y_pos = enemy_data(x_pos, y_pos) + 8;
      y_pos = item_data(x_pos, y_pos) + 8;
      al_draw_textf(font, palette_color[15], x_pos, y_pos,   0, "%d Lifts  ", num_lifts);
      al_draw_text(font, palette_color[15], x_pos, y_pos+8,  0, "-------");
   }


   int text_x = SCREEN_W / 2;
   int text_y = menu_map_y - 16;
   al_draw_textf(font, palette_color[11], text_x, text_y, ALLEGRO_ALIGN_CENTRE, " Level %d ", start_level );
   text_y += 8;

   if (resume_allowed) al_draw_text(font, palette_color[14], text_x, text_y, ALLEGRO_ALIGN_CENTRE, "  (paused)  ");
   else if (valid_level_loaded) al_draw_text(font, palette_color[9], text_x, text_y, ALLEGRO_ALIGN_CENTRE, "  start level  ");
   else al_draw_text(font, palette_color[10], text_x, text_y, ALLEGRO_ALIGN_CENTRE, "  not found !  ");
}

void frame_and_title(int show_players)
{
   int p = active_local_player;
   int color = players[p].color;

   // draw the border
   for (int x = 0; x < BORDER_WIDTH; x++)
      al_draw_rectangle(x+0.5f, x+0.5f, (SCREEN_W-1-x)+0.5f, (SCREEN_H-1-x)+0.5f,  palette_color[color + (x * 16)], 1);

   // draw the title on top on the border
   draw_title(SCREEN_W/2, 2, 322, 32, color);

   // draw the version text centered on the bottom of the border
   al_draw_textf(font, palette_color[15], SCREEN_W/2, SCREEN_H-10, ALLEGRO_ALIGN_CENTRE, "%s", version_string);

   if (show_players)
   {
      // draw a line of players on each side of menu
      color = players[p].color;               // initial color
      int lim = 12;                           // number of players to draw
      float flsc = 5;                         // initial scale
      float y_pos = 20;                       // initial y position
      float y_step = 20 / lim;                // inc to raise 20
      float x_pos = SCREEN_W/2 - 180;         // initial x position
      int dist = (int)x_pos - BORDER_WIDTH;   // how much space do I have?
      // how much space will it take based on how many to draw ???
      float x_sp = (dist - 240) / (lim-1);     // spacing to stretch row to screen edge
      for (float a=0; a<lim; a++)
      {
         int y = BORDER_WIDTH + (int)y_pos;  // y position
         y_pos -= y_step;                    // move up
         int x = (int)x_pos;                 // x position
         x_pos -= 11*flsc;                   // spacing based on scale
         x_pos -= x_sp;                      // extra spacing stretch row to edge of screen
         flsc *= .78;                        // reduce scale by 78%
         al_draw_scaled_rotated_bitmap(player_tile[color - 1][1], 0, 0, x+10, y+10, flsc, flsc, 0, 0);
         if (++color > 15) color = 1;        // cycle through players colors
      }

      color = players[p].color;               // initial color
      flsc = 5;                               // initial scale
      y_pos = 20;                             // initial y position
      y_step = 20 / lim;                      // inc to raise 20
      x_pos = SCREEN_W/2 + 80;                // initial x position
      dist = (SCREEN_W - BORDER_WIDTH) - (int)x_pos;   // how much space do I have?
      // how much space will it take based on how many to draw ???
      x_sp = (dist - 360) / (lim-1);          // spacing to stretch row to screen edge
      for (float a=0; a<lim; a++)
      {
         int y = BORDER_WIDTH + (int)y_pos;   // y position
         y_pos -= y_step;                     // move up
         int x = (int)x_pos;                  // x position
         x_pos += 15.1*flsc;                  // spacing based on scale
         x_pos += x_sp;                       // extra spacing stretch row to edge of screen
         flsc *= .78;                         // reduce scale by 78%
         x_sp += .5;                          // hack to make things line up on right hand side of screen
         al_draw_scaled_rotated_bitmap(player_tile[color - 1][1], 0, 0, x+10, y+10, flsc, flsc, 0, ALLEGRO_FLIP_HORIZONTAL);
         if (++color > 15) color = 1;        // cycle through players colors
      }
   }
}


void proc_scale_factor_change(void)
{
   show_scale_factor--;
   if (scale_factor_current < scale_factor)
   {
       // try to scale the inc, larger as scale_factor gets larger
       float inc = scale_factor_inc * scale_factor_current/3;
       scale_factor_current += inc;
       // if we overshoot set to exact to prevent oscillation
       if (scale_factor_current > scale_factor) scale_factor_current = scale_factor;
   }
   if (scale_factor_current > scale_factor)
   {
       // try to scale the inc, larger as scale_factor gets larger
       float inc = scale_factor_inc * scale_factor_current/3;
       scale_factor_current -= inc;
       // if we overshoot set to exact to prevent oscillation
       if (scale_factor_current < scale_factor) scale_factor_current = scale_factor;
   }
}


void rtextout_centre(ALLEGRO_BITMAP *dbmp, char *txt1, int x, int y, int col, float scale, int rot, float op)
{
   // draws rotated stretched text
   // used in many places
   int sw = strlen(txt1) * 8;
   ALLEGRO_BITMAP *temp = al_create_bitmap(sw, 8);
   al_set_target_bitmap(temp);
   al_clear_to_color(al_map_rgb(0,0,0));

   al_draw_text(font, palette_color[col], sw/2, 0, ALLEGRO_ALIGN_CENTRE, txt1);
   al_convert_mask_to_alpha(temp, al_map_rgb(0, 0, 0)) ;

   if (dbmp != NULL) al_set_target_bitmap(dbmp);
   else al_set_target_backbuffer(display);
   float frot = al_fixtof(al_fixmul(al_itofix(rot), al_fixtorad_r));

   al_draw_tinted_scaled_rotated_bitmap(temp, al_map_rgba_f(op, op, op, op), sw/2, 4, x, y, scale, scale, frot, 0);
   al_destroy_bitmap(temp);
}





void mtextout(char *txt1, int x, int y, float x_scale, float y_scale, int col)
{
   // can show mirror image text when scales are negative
   // used only mdw logo animation
//   int sw = strlen(txt1) * 8;      // string length in pixels
//   int sh = 8;                     // string height in pixels


   int bbx1, bby1, bbw1, bbh1;
   al_get_text_dimensions(f1, txt1, &bbx1, &bby1, &bbw1, &bbh1);

//   printf("bbx1:%d bby1:%d bbw1:%d bbh1:%d\n",bbx1, bby1, bbw1, bbh1);

   // scale the scale...
   x_scale *=  24 / (float) al_get_font_line_height(f1);
   y_scale *=  24 / (float) al_get_font_line_height(f1);

   int sw = bbw1;
   int sh = bbh1;



   ALLEGRO_BITMAP *temps = NULL;
   temps = al_create_bitmap(sw,sh);
   al_set_target_bitmap(temps);
   al_clear_to_color(al_map_rgb(0,0,0));


//   al_draw_text(font,palette_color[col], 0, 0, 0, txt1 );

   al_draw_text(f1, palette_color[col], 0-bbx1, 0-bby1, 0, txt1);


   al_convert_mask_to_alpha(temps, al_map_rgb(0, 0, 0)) ;


   int flags = 0;
   if (x_scale < 0) flags |= ALLEGRO_FLIP_HORIZONTAL;
   if (y_scale < 0) flags |= ALLEGRO_FLIP_VERTICAL;

   // offset x pos if scale is negative
   if (x_scale < 0) x -= abs( (int) ((float)sw * x_scale) );

   // offset y pos if scale is negative
   if (y_scale < 0) y -= abs( (int) ((float)sh * y_scale) );

   al_set_target_backbuffer(display);
   al_draw_scaled_rotated_bitmap(temps, 0, 0, x, y, fabs(x_scale), fabs(y_scale), 0, flags);
   al_destroy_bitmap(temps);
}

void mtextout_centre(const char *txt1, int x, int y, float x_scale, float y_scale, int col)
{
   // used only by bottom message and level done
   int sw = strlen(txt1) * 8;      // string length in pixels
   int sh = 8;                     // string height in pixels

   ALLEGRO_BITMAP *temp = al_create_bitmap(sw,sh);
   al_set_target_bitmap(temp);
   al_clear_to_color(al_map_rgb(0,0,0));

   int dw = (int) ((float)sw * x_scale);
   int dh = (int) ((float)sh * y_scale);

   al_draw_text(font, palette_color[col], 0, 0, 0, txt1);
   al_convert_mask_to_alpha(temp, al_map_rgb(0, 0, 0)) ;

   al_set_target_backbuffer(display);
   al_draw_scaled_bitmap(temp, 0, 0, sw, sh, x-dw/2, y, dw, dh, 0);

   al_destroy_bitmap(temp);
}

void show_level_done(int keypress)
{

   float x_scale = (float)SCREEN_W/40;
   float y_scale = (float)SCREEN_H/16;
   x_scale /= 1.5;
   y_scale /= 1.5;

   int x = SCREEN_W/2;
   int y = SCREEN_H/2;

   int col = players[active_local_player].color;

   draw_large_2lines(f2, "Level", "Done!", col, .6);


//   int yu = y - (int)(y_scale*8);
//   int col = 9; // color
//   int ns = 8;  // num of shadows
//   int ci = 16; // color inc
//   int st = 6;  // skip step between 1st and 2nd color
//
//   for (int a = ns; a >= 0; a--)
//   {
//      int b = col+a*ci;
//      if (a) b = col+( (a+st) * ci); // not first color
//      mtextout_centre("Level", x+a, yu+a, x_scale, y_scale, b);
//      mtextout_centre("Done!", x+a, y+a, x_scale, y_scale, b);
//   }
//
   if (keypress)
   {
      x_scale = (float)SCREEN_W/240;
      x_scale /= 1.2;
      y_scale = x_scale;
      //y = SCREEN_H - (int)(y_scale*9) - BORDER_WIDTH;
      y = SCREEN_H/2 - y_scale*4;
      mtextout_centre("...press any key to continue...", x, y, x_scale, y_scale, 5);
      al_flip_display();
      tsw();
   }
   else al_flip_display();
}


void draw_percent_bar(int cx, int y, int width, int height, int percent)
{
   int x = cx - width/2; // centering
   al_draw_filled_rectangle(x+0.5f, y+0.5f, x + width+0.5f, y + height+0.5f, palette_color[10]); //  all red to start
   if (percent > 0)
   {
      int w2 = (int) (width * ((float)percent/100)); // how much green
      al_draw_filled_rectangle(x+0.5f, y+0.5f, x + w2+0.5f, y + height+0.5f, palette_color[11]); //  green
   }
   al_draw_rectangle(x+0.5f, y+0.5f, x + width+0.5f, y + height+0.5f, palette_color[15], 1); //  white frame
}




