// mwDisplay.h

class mwDisplay
{
   public:

   ALLEGRO_DISPLAY *display = NULL;

   int desktop_width;
   int desktop_height;

   int disp_x_curr; // either wind in windowed mode or full fullscreen mode)
   int disp_y_curr;
   int disp_w_curr;
   int disp_h_curr;

   int disp_x_wind; // windowed
   int disp_y_wind;
   int disp_w_wind;
   int disp_h_wind;

   int disp_x_full; // fullscreen  (set to 0, 0, desktop_width, desktop_height and never change)
   int disp_y_full;
   int disp_w_full;
   int disp_h_full;

   float SCREEN_W;
   float SCREEN_H;

   int fullscreen = 1;
   int display_adapter_num = 0;

   int show_scale_factor;
   int scale_factor_holdoff;

   int display_transform_double;
   int display_transform_double_max;
   int saved_display_transform_double;
   int show_dtd;


   void set_window_title(void);

   float scale_factor;
   float scale_factor_current;

   float scale_factor_mlt;


   void set_scale_factor(float new_scale_factor, int instant);
   void proc_scale_factor_change(void);

   float custom_scale_factor_mlt;
   void proc_custom_scale_factor_change(void);
   void set_custom_scale_factor(float new_scale_factor, int time);


   void mw_set_clipping_rect(float x, float y, float w, float h);

   void show_bitmap_flags(int flags);
   void show_pixel_format(int df);
   void show_display_flags(int flags);
   void show_display_options(void);
   void show_display_orienation(void);
   void show_fullscreen_modes(void);
   void show_var_sizes(void);

   void auto_set_display_transform_double(void);
   void set_saved_display_transform(int sdt);
   void cycle_display_transform(void);
   void set_display_transform();
   void show_disp_values(int fs, int disp, int curr, int wind, int full, char *head);
   void show_display_adapters(void);
   int init_display(void);
   void proc_display_change(void);
   void save_display_window_position(void);
   void proc_display_change_tofs(void);
   void proc_display_change_fromfs(void);
   void toggle_fullscreen(void);

};
extern mwDisplay mDisplay;
