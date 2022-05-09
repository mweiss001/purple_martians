// z_screen.cpp

#include "pm.h"



void load_fonts(void)
{
   al_destroy_font(font);
   font = al_create_builtin_font();
   if(!font)
   {
      sprintf(msg, "Failed to create_builtin_font");
      m_err(msg);
      printf("%s\n", msg);
      exit(0);
   }
   //else printf("created builtin font\n");

   al_destroy_font(f1);
   f1 = al_load_ttf_font("bitmaps/Achafont.ttf", 240, 0);
   if(!f1) m_err("Failed to load font from bitmaps/Achafont.ttf");
   //else printf("loaded font Achafont.ttf\n");

   int sfs = al_get_display_option(display, ALLEGRO_MAX_BITMAP_SIZE) / 20;
   //printf("Saucer font size:%d\n", sfs);
   al_destroy_font(f2);
   f2 = al_load_ttf_font("bitmaps/SaucerBB.ttf", sfs, 0);
   if(!f2) m_err("Failed to load font from bitmaps/SaucerBB.ttf");
   //else printf("loaded font SaucerBB.ttf.ttf\n");

   al_destroy_font(f3);
   f3 = al_load_ttf_font("bitmaps/PixelGosub.otf", 7, ALLEGRO_TTF_MONOCHROME);
   if(!f3) m_err("Failed to load font from bitmaps/PixelGosub.otf");
   //else printf("loaded font PixelGosub.otf\n");
}


// done at start only now
void create_bmp(void)
{
   // create tilemap bitmaps
   al_set_new_bitmap_format(ALLEGRO_PIXEL_FORMAT_ANY_32_WITH_ALPHA);
   al_set_new_bitmap_flags(ALLEGRO_NO_PRESERVE_TEXTURE | ALLEGRO_VIDEO_BITMAP);
   tilemap  = al_create_bitmap(640, 640);
   btilemap = al_create_bitmap(640, 640);
   ptilemap = al_create_bitmap(480,320);
   dtilemap = al_create_bitmap(160,640);
//   printf("tilemap\n");
//   show_pixel_format(al_get_bitmap_format(tilemap));
//   show_bitmap_flags(al_get_bitmap_flags(tilemap));


   // create memory bitmaps as temp storage for restoring tilemaps after screen change
   al_set_new_bitmap_flags(ALLEGRO_MEMORY_BITMAP);
   M_tilemap  = al_create_bitmap(640,640);
   M_btilemap = al_create_bitmap(640,640);
   M_ptilemap = al_create_bitmap(480,320);
   M_dtilemap = al_create_bitmap(160,640);
//   printf("M_tilemap\n");
//   show_pixel_format(al_get_bitmap_format(M_tilemap));
//   show_bitmap_flags(al_get_bitmap_flags(M_tilemap));


   // create level_background and level_buffer bitmaps
   al_set_new_bitmap_format(ALLEGRO_PIXEL_FORMAT_ANY_32_WITH_ALPHA);
// al_set_new_bitmap_format(ALLEGRO_PIXEL_FORMAT_ANY_16_NO_ALPHA);
   al_set_new_bitmap_flags(ALLEGRO_NO_PRESERVE_TEXTURE | ALLEGRO_VIDEO_BITMAP);
   level_background = al_create_bitmap(2000,2000);
   level_buffer = al_create_bitmap(2000,2000);
//   printf("level_background\n");
//   show_pixel_format(al_get_bitmap_format(level_background));
//   show_bitmap_flags(al_get_bitmap_flags(level_background));

   // reset defaults to use for new bitmaps
   al_set_new_bitmap_format(ALLEGRO_PIXEL_FORMAT_ANY_32_WITH_ALPHA);
   al_set_new_bitmap_flags(ALLEGRO_NO_PRESERVE_TEXTURE | ALLEGRO_VIDEO_BITMAP);

   // bottom msg bitmaps
   for (int x=0; x<20; x++)
   {
      bmsg_bmp[x] = al_create_bitmap(800, 20);
      bmsg_bmp2[x] = bmsg_bmp[x];
   }
}




void rebuild_bitmaps(void)
{
   //printf("rebuild bitmaps\n");

   // rebuild main tiles
   al_set_target_bitmap(tilemap);
   al_draw_bitmap(M_tilemap, 0, 0, 0);

   // rebuild block tiles
   al_set_target_bitmap(btilemap);
   al_draw_bitmap(M_btilemap, 0, 0, 0);

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

   load_visual_level_select_done = 0;
   if (visual_level_select_running) load_visual_level_select();
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


void get_new_screen_buffer(int type, int x, int y)
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
      if (!level_editor_running) sbx += a/2;        // new screen_buffer blit xpos
   }

   // is the entire level smaller than the screen buffer height?
   if (sls < sbh)
   {
      int a = sbh - sls; // how much smaller?
      sbh = sls;         // new screen_buffer blit height = sls
      if (!level_editor_running) sby += a/2;        // new screen_buffer blit ypos
   }


   // find the size of the source screen from actual screen size and scaler
   int SW = (int)( (float)(SCREEN_W - bw *2) / scale_factor_current);
   int SH = (int)( (float)(SCREEN_H - bw *2) / scale_factor_current);
   if (SW > 2000) SW = 2000;
   if (SH > 2000) SH = 2000;


   int PX = 0;
   int PY = 0;


   // find where to grab the source screen from based on the players position
   if (type == 0)
   {
      PX = al_fixtoi(players[alp].PX) + 10;
      PY = al_fixtoi(players[alp].PY) + 10;
   }
   if (type == 1)
   {
      PX = x;
      PY = y;
   }

   if (type != 3)
   {


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



   }

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



   // in level editor mode, if the level is smaller than the screen edges, draw a thin line to show where it ends...
   if (type == 3)
   {
      sbw = SCREEN_W-bw*2; // recalc these beacuse they have been modified
      sbh = SCREEN_H-bw*2;
      int xdraw = 0;
      int ydraw = 0;
      int xl=SCREEN_W-bw; // default screen edge positions
      int yl=SCREEN_H-bw;
      if (sls < sbw)
      {
         xl = sbx+sls;
         xdraw = 1;
      }
      if (sls < sbh)
      {
         yl = sby+sls;
         ydraw = 1;
      }
      if (xdraw) al_draw_line(xl, bw, xl, yl, palette_color[c], 0);
      if (ydraw) al_draw_line(bw, yl, xl, yl, palette_color[c], 0);
      //al_draw_rectangle(sbx, sby, sbx+sbw, sby+sbh, palette_color[c], 0);
   }






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

   #ifdef SHOW_HYSTERESIS_WINDOW
   float hx1 = SCREEN_W/2 - x_size * scale_factor_current;
   float hx2 = SCREEN_W/2 + x_size * scale_factor_current;
   float hy1 = SCREEN_H/2 - y_size * scale_factor_current;
   float hy2 = SCREEN_H/2 + y_size * scale_factor_current;
   al_draw_rectangle(hx1, hy1, hx2, hy2, palette_color[10], 2);
   #endif

}

void set_map_var(void)
{

   // check that status and select windows are not off screen
   em_check_s_window_pos(0);


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
         int c = l[x][y] & 1023;
         al_draw_bitmap(btile[c], x*20, y*20, 0);
         if ((level_editor_running) && (show_non_default_blocks))
         {
            if ((sa[c][0] & PM_BTILE_MOST_FLAGS) != (l[x][y] & PM_BTILE_MOST_FLAGS))
            {
               al_draw_line(x*20, y*20, x*20+20, y*20+20, palette_color[10], 1);
               al_draw_line(x*20+20, y*20, x*20, y*20+20, palette_color[10], 1);
            }
         }
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
      if (enemies) draw_enemies();
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
   al_draw_textf(font, palette_color[15], SCREEN_W/2, SCREEN_H-10, ALLEGRO_ALIGN_CENTRE, "Version %s", pm_version_string);

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
         al_draw_scaled_rotated_bitmap(player_tile[color][1], 0, 0, x+10, y+10, flsc, flsc, 0, 0);
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
         al_draw_scaled_rotated_bitmap(player_tile[color][1], 0, 0, x+10, y+10, flsc, flsc, 0, ALLEGRO_FLIP_HORIZONTAL);
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
   int x = cx - width/2; // get x from center
   al_draw_filled_rectangle(x+0.5f, y+0.5f, x + width+0.5f, y + height+0.5f, palette_color[10]); //  all red to start
   if (percent > 0)
   {
      int w2 = (int) (width * ((float)percent/100)); // how much green
      al_draw_filled_rectangle(x+0.5f, y+0.5f, x + w2+0.5f, y + height+0.5f, palette_color[11]); //  green
   }
   al_draw_rectangle(x-0.5f, y+0.5f, x+width+0.5f, y+height+0.5f, palette_color[15], 1); //  white frame
}

void draw_percent_bar_line(int cx, int y, int width, int height, int rise, int color, int percent )
{
   int x = cx - width/2; // get x from center
   int w2 = (int) (width * ((float)percent/100)); // position
   al_draw_line(x+0.5f+w2, y+0.5f-rise, x+0.5f+w2, y+0.5f+height+rise, palette_color[color], 1);
}



void draw_percent_bar_range(int cx, int y, int width, int height, int color, int start, int end)
{
   int x = cx - width/2; // get x from center
   int w1 = (int) (width * ((float)start/100));
   int w2 = (int) (width * ((float)end/100));
   al_draw_filled_rectangle(x+w1+0.5f, y+1.5f, x+w2+0.5f, y+height+0.5f, palette_color[color]);
   al_draw_rectangle(x+0.5f, y+0.5f, x + width+0.5f, y + height+0.5f, palette_color[15], 1); //  white frame
}



