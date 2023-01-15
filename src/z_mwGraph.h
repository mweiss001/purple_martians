// z_mwGraph.h

class mwGraph
{
   public:

   int graph_x1;
   int graph_y1;
   int graph_x2;
   int graph_y2;
   int graph_w;
   int graph_h;

   int plot_x1;
   int plot_y1;
   int plot_x2;
   int plot_y2;
   int plot_w;
   int plot_h;


   int  title_draw_on;

   int  title_draw_style;


   int  title_draw_size;
   int  title_draw_y;
   char title_text[1024];
   int  title_text_color;
   int  title_frame_color;
   void draw_title(int set_size_only);

   void draw_series_legend(void);


   int series_legend_slave;
   int series_legend_type;
   int series_legend_draw_on;
   int series_legend_size;
   int series_legend_x1;
   int series_legend_x2;
   int series_legend_w;
   int series_legend_y1;
   int series_legend_y2;

   int series_legend_force_solid_lines;

   void autoset_new_series_color(int series_index);

   int series_legend_show_counts;

   int plot_show_performance;


   void set_title(const char* text, int _type, int text_color, int frame_color);

   void set_x_axis_legend(const char* name, const char* units, int font, int text_color, int frame_color);
   void set_y_axis_legend(const char* name, const char* units, int font, int text_color, int frame_color);

   void set_x_axis_labels(int type, int font, int tick_size, int color);
   void set_y_axis_labels(int type, int font, int tick_size, int color);

   void set_series(int s, const char* text, int color1, int color2);
   void set_series_legend_type(int type);


   int linked_group_id;
   double x_axis_cursor_pos;


   int  x_axis_slave;
   int  x_axis_type;
   int  x_axis_draw_size;

   int x_axis_grid_draw_on;
   int x_axis_grid_color;
   int x_axis_grid_label_draw_on;
   int x_axis_grid_label_font;
   int x_axis_grid_label_color;
   int x_axis_grid_label_draw_size;
   int x_axis_grid_label_tick_size;
   int x_axis_grid_label_text_size;

   int  x_axis_legend_draw_on;
   int  x_axis_legend_draw_size;
   int  x_axis_legend_font;
   int  x_axis_legend_draw_y;

   char x_axis_legend_name[256];
   char x_axis_legend_units[256];



   int  x_axis_legend_text_color;
   int  x_axis_legend_frame_color;
   double x_axis_divider;


   int x_axis_scrollbar_draw_on;
   int x_axis_scrollbar_size;
   int x_axis_scrollbar_draw_size;
   int x_axis_scrollbar_x1;
   int x_axis_scrollbar_x2;
   int x_axis_scrollbar_w;
   int x_axis_scrollbar_bar_x1;
   int x_axis_scrollbar_bar_x2;
   int x_axis_scrollbar_y1;
   int x_axis_scrollbar_y2;
   int x_axis_zoom_lock;

   void x_axis_get_size_and_arrange_pos(void);
   void x_axis_draw_legend(int set_size_only);
   void x_axis_draw_gridlines_and_labels(int set_size_only);
   int  x_axis_draw_scrollbar(int set_size_only);
   void x_axis_proc_scrollbar(int draw_only);


   int  y_axis_slave;

   int y_axis_type;
   int y_axis_draw_size;

   int y_axis_grid_draw_on;
   int y_axis_grid_color;
   int y_axis_grid_label_draw_on;
   int y_axis_grid_label_font;
   int y_axis_grid_label_color;
   int y_axis_grid_label_draw_size;
   int y_axis_grid_label_tick_size;
   int y_axis_grid_label_text_size;

   int  y_axis_legend_draw_on;
   int  y_axis_legend_draw_size;
   int  y_axis_legend_font;
   int  y_axis_legend_draw_x;
   char y_axis_legend_name[256];
   char y_axis_legend_units[256];
   int  y_axis_legend_text_color;
   int  y_axis_legend_frame_color;
   double y_axis_divider;



   int y_axis_scrollbar_draw_on;
   int y_axis_scrollbar_size;
   int y_axis_scrollbar_draw_size;
   int y_axis_scrollbar_x1;
   int y_axis_scrollbar_x2;
   int y_axis_scrollbar_w;
   int y_axis_scrollbar_bar_y1;
   int y_axis_scrollbar_bar_y2;
   int y_axis_scrollbar_y1;
   int y_axis_scrollbar_y2;
   int y_axis_scrollbar_h;
   int y_axis_zoom_lock;

   void y_axis_get_size_and_arrange_pos(void);
   void y_axis_set_pos(void);
   void y_axis_draw_legend(int set_size_only);
   void y_axis_draw_gridlines_and_labels(int set_size_only);
   int  y_axis_draw_scrollbar(int set_size_only);
   void y_axis_proc_scrollbar(int draw_only);


   struct data_series
   {
      double data[10000][2];
      int num_data;
      int active;
      int color1;
      int color2;
      char name[256];
      int min_visible_index;
      int max_visible_index;
   };
   data_series series[20] = {0};

   float plot_line_size;
   float plot_point_size;

   void set_series_min_and_max_visible_indexes(int s);

   // min, max, and range for entire set of data
   double x_data_min;
   double x_data_max;
   double x_data_rng;
   double y_data_min;
   double y_data_max;
   double y_data_rng;


   // min, max, and range for min and max displayable range (used for limits on axis, instead of using data)
   double x_disp_min;
   double x_disp_max;
   double x_disp_rng;
   double y_disp_min;
   double y_disp_max;
   double y_disp_rng;

   // min, max, and range for axis
   double x_axis_min;
   double x_axis_max;
   double x_axis_rng;
   double y_axis_min;
   double y_axis_max;
   double y_axis_rng;

   double x_axis_rng_min;
   double y_axis_rng_min;


   int  convert_sxy_to_gxy(      int sx, int sy, double &gx, double &gy);
   int  convert_gxy_to_sxy(double gx, double gy, double &sx, double &sy);


   mwGraph(); // default constructor
   void initialize(void);
   void set_graph_pos(int x1, int y1, int x2, int y2);

   void draw_point_data(int x, int y, double mx, double my, int color, ALLEGRO_FONT *f, int s);

   void draw_graph(int draw_only);
   void proc_graph(void);

   void draw_plot_area(void);
   void proc_plot_area(int draw_only);

   void add_data_point(int series, double x, double y);
   int calc_data_range(void);

   void autorange_axis(int x, int y);
   void set_range_axis(double x_min, double x_max, double y_min, double y_max);
   void enforce_axis_limits(void);
   void enforce_axis_limits(int type);

   char* x_axis_get_val_text(double val, int units);
   char* y_axis_get_val_text(double val, int units);

   int find_closest_point_to_mouse(int &s, int &i);

   void proc_plot_mouse_cursor_crosshairs(double mx1, double my1);
   void proc_plot_menu(void);
   int proc_series_legend_menu(void);

   bool mouse_on_graph;
   bool mouse_on_scrollbar;

};
extern mwGraph mG[10];






