// zlogo.cpp

#include "pm.h"

// used by "created by:"
void mw_text(ALLEGRO_FONT *tf, int col, float x_pc, const char * txt)
{
   int bbx1, bby1, bbw1, bbh1;
   al_get_text_dimensions(tf, txt, &bbx1, &bby1, &bbw1, &bbh1);
   ALLEGRO_BITMAP *t1 = al_create_bitmap(bbw1, bbh1);
   al_set_target_bitmap(t1);
   al_clear_to_color(al_map_rgb(0,0,0));
   al_draw_text(tf, palette_color[col], 0-bbx1, 0-bby1, 0, txt);
   al_convert_mask_to_alpha(t1, al_map_rgb(0, 0, 0));
   float xs = ( (float)SCREEN_W * x_pc) / (float) bbw1 ; // x scale
   al_set_target_backbuffer(display);
   al_draw_scaled_rotated_bitmap(t1, bbw1/2, bbh1/2, SCREEN_W/2, SCREEN_H/2, xs, xs, 0, 0);
   al_destroy_bitmap(t1);
}

void draw_title(int tx, int ty, int w, int h, int color)
{
   sprintf(msg, "%s Martians!", color_name[color]); // overwrite with color name
   int bbx1, bby1, bbw1, bbh1;
   al_get_text_dimensions(f2, msg, &bbx1, &bby1, &bbw1, &bbh1);
   if ((text_title_bitmaps_create) || (text_title_draw_color != color))
   {
      text_title_bitmaps_create = 0;
      text_title_draw_color = color;
      al_destroy_bitmap(text_title);
      text_title = al_create_bitmap(bbw1,bbh1);

      if(!text_title)
      {
         sprintf(msg, "Error creating text_title %d %d\n", bbw1,bbh1);
         m_err(msg);
      }
      //else printf("created text_title %d %d\n", bbw1,bbh1);
      al_set_target_bitmap(text_title);
      al_clear_to_color(al_map_rgb(0,0,0));
      al_draw_text(f2, palette_color[color], 0-bbx1, 0-bby1, 0, msg);
      al_convert_mask_to_alpha(text_title, al_map_rgb(0, 0, 0)) ;
   }
   al_set_target_backbuffer(display);
   al_draw_scaled_bitmap(text_title, 0, 0, bbw1, bbh1, tx - w/2, ty, w, h, 0);
}

void draw_demo_mode_overlay(void)
{
   ALLEGRO_FONT *tf = f2;

   int bbx1, bby1, bbw1, bbh1;
   al_get_text_dimensions(tf, "Demo", &bbx1, &bby1, &bbw1, &bbh1);

   if (text_demomode_bitmaps_create)
   {
      text_demomode_bitmaps_create = 0;
      al_destroy_bitmap(text_demo);
      text_demo = al_create_bitmap(bbw1,bbh1);
      al_set_target_bitmap(text_demo);
      al_clear_to_color(al_map_rgb(0,0,0));
      al_draw_text(tf, palette_color[15], 0-bbx1, 0-bby1, 0, "Demo");
      al_convert_mask_to_alpha(text_demo, al_map_rgb(0, 0, 0)) ;

      al_destroy_bitmap(text_mode);
      text_mode = al_create_bitmap(bbw1,bbh1);
      al_set_target_bitmap(text_mode);
      al_clear_to_color(al_map_rgb(0,0,0));
      al_draw_text(tf, palette_color[15], 0-bbx1, 0-bby1, 0, "Mode");
      al_convert_mask_to_alpha(text_mode, al_map_rgb(0, 0, 0)) ;

      al_set_target_backbuffer(display);
   }

   //int xs = SCREEN_W*7/8; // x size
   //int ys = SCREEN_H*3/8; // y size

   float xs = SCREEN_W*2/8; // x size
   float ys = SCREEN_H*1/8; // y size

   int x1 = (SCREEN_W - xs)/2;
   int x2 = xs;

   //int yu1 = SCREEN_H*1/4 - ys/2;
   //int yu1 = SCREEN_H/2 - ys * 1.0;
   int yu1 = BORDER_WIDTH + 4;
   int yu2 = ys;

   //int yl1 = SCREEN_H*3/4 - ys/2;
   //int yl1 = SCREEN_H/2 + ys * 0.0;
   //int yl2 = ys;

   float f = 0.2;
   ALLEGRO_COLOR fc = al_map_rgba_f(f,f,f,f);

   al_draw_tinted_scaled_bitmap(text_demo, fc, 0, 0, bbw1, bbh1, x1, yu1, x2, yu2, 0);
//   al_draw_tinted_scaled_bitmap(text_mode, fc, 0, 0, bbw1, bbh1, x1, yl1, x2, yl2, 0);
//   al_draw_line(0, SCREEN_H/2, SCREEN_W, SCREEN_H/2, palette_color[15], 1);
//   al_draw_line(SCREEN_W/2, 0, SCREEN_W/2, SCREEN_H, palette_color[15], 1);
}



void draw_large_2lines(ALLEGRO_FONT *tf, const char * m1, const char * m2, int color, float opa )
{
   int bbx1, bby1, bbw1, bbh1;
   int bbx2, bby2, bbw2, bbh2;
   al_get_text_dimensions(tf, m1, &bbx1, &bby1, &bbw1, &bbh1);
   al_get_text_dimensions(tf, m2, &bbx2, &bby2, &bbw2, &bbh2);
   // get max w and h
   float bbw3 = bbw1; if (bbw2 > bbw1) bbw3 = bbw2;
   float bbh3 = bbh1; if (bbh2 > bbh1) bbh3 = bbh2;

//   printf("m1:%s  m2:%s \n",m1, m2);
//   printf("bbx1:%4d bby1:%4d bbw1:%4d bbh1:%4d\n",bbx1, bby1, bbw1, bbh1);
//   printf("bbx2:%4d bby2:%4d bbw2:%4d bbh2:%4d\n",bbx2, bby2, bbw2, bbh2);

   ALLEGRO_BITMAP *t1 = al_create_bitmap(bbw3, bbh3);
   al_set_target_bitmap(t1);
   al_clear_to_color(al_map_rgb(0,0,0));
   al_draw_text(tf, palette_color[color], 0-bbx1 + (bbw3-bbw1)/2, 0-bby1, 0, m1);
   al_convert_mask_to_alpha(t1, al_map_rgb(0, 0, 0));

   ALLEGRO_BITMAP *t2 = al_create_bitmap(bbw3, bbh3);
   al_set_target_bitmap(t2);
   al_clear_to_color(al_map_rgb(0,0,0));
   al_draw_text(tf, palette_color[color], 0-bbx2 + (bbw3-bbw2)/2, 0-bby2, 0, m2);
   al_convert_mask_to_alpha(t2, al_map_rgb(0, 0, 0));

   int xs = SCREEN_W*7/8; // x size
   int ys = SCREEN_H*3/8; // y size

   int x1 = (SCREEN_W - xs)/2;
   int x2 = xs;

   int yu1 = SCREEN_H*1/4 - ys/2;
   int yu2 = ys;

   int yl1 = SCREEN_H*3/4 - ys/2;
   int yl2 = ys;

   al_set_target_backbuffer(display);

//   al_draw_scaled_bitmap(t1, 0, 0, bbw3, bbh3, x1, yu1, x2, yu2, 0);
//   al_draw_scaled_bitmap(t2, 0, 0, bbw3, bbh3, x1, yl1, x2, yl2, 0);


   ALLEGRO_COLOR fc = al_map_rgba_f(opa, opa, opa, opa);

   al_draw_tinted_scaled_bitmap(t1, fc, 0, 0, bbw3, bbh3, x1, yu1, x2, yu2, 0);
   al_draw_tinted_scaled_bitmap(t2, fc, 0, 0, bbw3, bbh3, x1, yl1, x2, yl2, 0);


   al_destroy_bitmap(t1);
   al_destroy_bitmap(t2);
}

void idw(int txt, int x, int y, float x_scale, float y_scale)
{
   //just like mtext only with predrawn text bitmaps
   // used only by mdw logo animation

   int bbx1, bby1, bbw1, bbh1;
   al_get_text_dimensions(f1, "ichael", &bbx1, &bby1, &bbw1, &bbh1);

   if (logo_text_bitmaps_create)
   {
      logo_text_bitmaps_create = 0;

      al_destroy_bitmap(logo_ichael);
      logo_ichael = al_create_bitmap(bbw1,bbh1);
      al_set_target_bitmap(logo_ichael);
      al_clear_to_color(al_map_rgb(0,0,0));
      al_draw_text(f1, palette_color[8], 0-bbx1, 0-bby1, 0, "ichael");
      al_convert_mask_to_alpha(logo_ichael, al_map_rgb(0, 0, 0)) ;

      al_destroy_bitmap(logo_avid);
      logo_avid = al_create_bitmap(bbw1,bbh1);
      al_set_target_bitmap(logo_avid);
      al_clear_to_color(al_map_rgb(0,0,0));
      al_draw_text(f1, palette_color[90], 0-bbx1, 0-bby1, 0, "avid");
      al_convert_mask_to_alpha(logo_avid, al_map_rgb(0, 0, 0)) ;

      al_destroy_bitmap(logo_eiss);
      logo_eiss = al_create_bitmap(bbw1,bbh1);
      al_set_target_bitmap(logo_eiss);
      al_clear_to_color(al_map_rgb(0,0,0));
      al_draw_text(f1, palette_color[8], 0-bbx1, 0-bby1, 0, "eiss");
      al_convert_mask_to_alpha(logo_eiss, al_map_rgb(0, 0, 0)) ;
   }

   // scale the scale...
   x_scale *=  72 / (float) al_get_font_line_height(f1);
   y_scale *=  24 / (float) al_get_font_line_height(f1);

   int flags = 0;
   if (x_scale < 0) flags |= ALLEGRO_FLIP_HORIZONTAL;
   if (y_scale < 0) flags |= ALLEGRO_FLIP_VERTICAL;

   // offset x pos if scale is negative
   if (x_scale < 0) x -= abs( (int) ((float)bbw1 * x_scale) );

   // offset y pos if scale is negative
   if (y_scale < 0) y -= abs( (int) ((float)bbh1 * y_scale) );

   al_set_target_backbuffer(display);

   if (txt == 1) al_draw_scaled_rotated_bitmap(logo_ichael, 0, 0, x, y, fabs(x_scale), fabs(y_scale), 0, flags);
   if (txt == 2) al_draw_scaled_rotated_bitmap(logo_avid,   0, 0, x, y, fabs(x_scale), fabs(y_scale), 0, flags);
   if (txt == 3) al_draw_scaled_rotated_bitmap(logo_eiss,   0, 0, x, y, fabs(x_scale), fabs(y_scale), 0, flags);
}


void splash_screen(void)
{
   int quit = 0;
   proc_controllers();

   al_set_target_backbuffer(display);
   al_flip_display();
   al_clear_to_color(al_map_rgb(0, 0, 0));

   draw_large_2lines(f2, "Purple", "Martians", 8, 1);
   al_flip_display();
   al_clear_to_color(al_map_rgb(0, 0, 0));

   if (!quit)
      for (int i=0; i<220; i++)
      {
         if (proc_controllers())
         {
            quit = 1;
            i = 1000;
         }
      }
   if (quit)
   {
      al_clear_to_color(al_map_rgb(0,0,0));
      al_flip_display();
   }

   if (!quit)
   {
      mw_text(f2, 12, .80, "created by:");
      al_flip_display();
   }

   if (!quit)
      for (int i=0; i<120; i++)
      {
         if (proc_controllers())
         {
            quit = 1;
            i = 1000;
         }
      }
   if (quit)
   {
      al_clear_to_color(al_map_rgb(0,0,0));
      al_flip_display();
   }

   while (!quit)
   {
      al_clear_to_color(al_map_rgb(0,0,0));
      quit = mdw_an2();
      al_flip_display();
      if (proc_controllers()) quit = 1;
   }

   al_clear_to_color(al_map_rgb(0,0,0));
   al_flip_display();

   mdw_an_seq = 0;
}

void splash_toggle(void)
{
   get_config_values();

   if (show_splash_screen)
   {
      show_splash_screen = 0;
      save_config();
   }
   else
   {
      show_splash_screen = 1;
      save_config();

      mdw_an_seq = 0;
      set_map_var();
      splash_screen(); // show it now....
   }
}




// my version of spline with circle endpoints
void mspline(float *par, ALLEGRO_COLOR col, int thickness)
{
   al_draw_spline(par, col, thickness);
   float r = (float)thickness * .8;
   al_draw_filled_circle(par[0], par[1], r, col);
   al_draw_filled_circle(par[6], par[7], r, col);
}

// my version of spline with thickness and fading
void mfspline(float *par, int col, int thickness)
{
   ALLEGRO_COLOR c = palette_color[col];
   float r, g, b;
   al_unmap_rgb_f(c, &r, &g, &b);

   for (int a = thickness; a>0; a--)
   {
      float f = (float)(a) / (float)(thickness); // fade in
      // float f = (float)(thickness-a) / (float)(thickness); // fade out
      mspline(par, al_map_rgb_f(r*f, g*f, b*f), a);
   }


/*   int cs = 16;
   if (thickness == 2) cs = 128; // 0 128.. 2 values
   if (thickness > 2)  cs = 64;  // 0 64 128 192.. 4 values
   if (thickness > 4)  cs = 48;  // 0 48 96 144 192 240 ... 6 values
   if (thickness > 6)  cs = 32;  // 0 32 64... 8 values
   if (thickness > 8)  cs = 16;  // 0 16 32... 16 values

   {
      for (int a = thickness; a>0; a--)
         mspline(par, col+(thickness-a)*cs, a);
   }
*/
}


void draw_mdw(int x, int y, float x_scale, float y_scale, int line_thickness)
{
   int c1 = 10; //color 1 (red)
   int c2 = 8;  //color 2 (purple)

   float t;
   if (line_thickness == -1) // auto
   {
      // this methods uses the hypotenuse of both scales
      //float hy_scale = sqrt( (x_scale * x_scale) + (y_scale * y_scale));
      //t = 2 + hy_scale * 7;

      // this method uses the max scale, x or y
      float max_scale = fabs(x_scale);
      if (fabs(y_scale) > fabs(x_scale)) max_scale = fabs(y_scale);
      t = 3 + max_scale * 7;
   }
   else t = line_thickness;

   float draw_points[9][8];

   // apply scale
   for (int j=0; j<9; j++)
      for (int i=0; i<8; i+=2)
      {
         draw_points[j][i]   = points[j][i]   * x_scale;
         draw_points[j][i+1] = points[j][i+1] * y_scale;
      }

   // apply offset
   for (int j=0; j<9; j++)
      for (int i=0; i<8; i+=2)
      {
         draw_points[j][i]   += x;
         draw_points[j][i+1] += y;
      }

   // drawing order
   int order = 0; // normal with inner arms in front
   if (x_scale < 0) order = !order;
   if (y_scale < 0) order = !order;

   if (order)
   {
      mfspline(draw_points[8], c1, t);
      mfspline(draw_points[2], c1, t);
      mfspline(draw_points[3], c1, t);
      mfspline(draw_points[6], c1, t);
      mfspline(draw_points[7], c1, t);
      mfspline(draw_points[0], c2, t);
      mfspline(draw_points[1], c2, t);
      mfspline(draw_points[4], c2, t);
      mfspline(draw_points[5], c2, t);
   }
   else
   {
      mfspline(draw_points[0], c2, t);
      mfspline(draw_points[1], c2, t);
      mfspline(draw_points[4], c2, t);
      mfspline(draw_points[5], c2, t);
      mfspline(draw_points[8], c1, t);
      mfspline(draw_points[2], c1, t);
      mfspline(draw_points[3], c1, t);
      mfspline(draw_points[6], c1, t);
      mfspline(draw_points[7], c1, t);
   }

   // show the rest of the name
   float xs = x_scale * 2;
   float ys = y_scale * 3;
   idw(1, (int)(x - 54  * x_scale), (int)(y - 130 * y_scale), xs, ys); // ichael
   idw(2, (int)(x + 90  * x_scale), (int)(y -  15 * y_scale), xs, ys); // avid
   idw(3, (int)(x + 146 * x_scale), (int)(y + 120 * y_scale), xs, ys); // eiss
}


// 0 1 are the outer parts of M
// 2 3 are the inner parts of M
// 4 5 outer W
// 6 7 inner W

// 8 D



// 0 is copied to 1 4 5
// 2 is copied to 3 6 7

// the height and width of M and W are 200

// they meet at the lower right corner of M and the upper left corner of W
// that spot is the origin 0, 0
// this makes scaling easy as I can mutlipy all the values by the same scaler

void seed_mdw(void)
{
   // outer arms start and end pos are all fixed
   points[0][0] = -200;
   points[0][1] = 0;
   points[0][6] = -200;
   points[0][7] = -200;

   points[1][0] = 0;
   points[1][1] = 0;
   points[1][6] = 0;
   points[1][7] = -200;

   points[4][0] = 0;
   points[4][1] = 0;

   points[4][6] = 0;
   points[4][7] = 200;

   points[5][0] = 200;
   points[5][1] = 0;
   points[5][6] = 200;
   points[5][7] = 200;

   // outer arm (spline 0) control points
   points[0][2] = -115;
   points[0][3] = -93;
   points[0][4] = -105;
   points[0][5] = -169;

   // inner arm (spline 2) control points
   points[2][0] = -170;
   points[2][1] = -170;
   points[2][2] = -123;
   points[2][3] = -168;
   points[2][4] = -107;
   points[2][5] = -147;
   points[2][6] = -91;
   points[2][7] = -131;

   // 'D' has only 1 spline, start point and first control point are mirrored in the y axis
   points[8][0] = -70;
   points[8][1] = -40;
   points[8][2] = 132;
   points[8][3] = -88;
}

void fill_mdw(void)
{
   // --- outer arms ---
   // mirror spline 0 to 1, 4, 5

   // get control points from spline 0
   float cp1x = points[0][0] - points[0][2];
   float cp1y = points[0][1] - points[0][3];
   float cp2x = points[0][0] - points[0][4];
   float cp2y = points[0][1] - points[0][5];

   // apply to other splines
   points[1][2] = points[1][0] + cp1x;
   points[1][3] = points[1][1] - cp1y;
   points[1][4] = points[1][0] + cp2x;
   points[1][5] = points[1][1] - cp2y;

   points[4][2] = points[4][0] - cp1x;
   points[4][3] = points[4][1] + cp1y;
   points[4][4] = points[4][0] - cp2x;
   points[4][5] = points[4][1] + cp2y;

   points[5][2] = points[5][0] + cp1x;
   points[5][3] = points[5][1] + cp1y;
   points[5][4] = points[5][0] + cp2x;
   points[5][5] = points[5][1] + cp2y;

   // --- inner arms ---
   // mirror spline 2 to 3, 6, 7

   // get inner arm offsets from outer arm
   float ia1x = points[0][0] - points[2][0]; // inner arm x1
   float ia1y = points[0][1] - points[2][1]; // inner arm y1
   float ia2x = points[0][0] - points[2][6]; // inner arm x2
   float ia2y = points[0][1] - points[2][7]; // inner arm y2

   // get inner arm control points
   cp1x = points[2][0] - points[2][2]; // inner arm x1
   cp1y = points[2][1] - points[2][3]; // inner arm y1
   cp2x = points[2][0] - points[2][4]; // inner arm x2
   cp2y = points[2][1] - points[2][5]; // inner arm y2

   // apply to other inner arms
   points[3][0] = points[1][0] + ia1x;
   points[3][1] = points[1][1] - ia1y;
   points[3][6] = points[1][0] + ia2x;
   points[3][7] = points[1][1] - ia2y;

   points[3][2] = points[3][0] + cp1x;
   points[3][3] = points[3][1] - cp1y;
   points[3][4] = points[3][0] + cp2x;
   points[3][5] = points[3][1] - cp2y;

   points[6][0] = points[4][0] - ia1x;
   points[6][1] = points[4][1] + ia1y;
   points[6][6] = points[4][0] - ia2x;
   points[6][7] = points[4][1] + ia2y;

   points[6][2] = points[6][0] - cp1x;
   points[6][3] = points[6][1] + cp1y;
   points[6][4] = points[6][0] - cp2x;
   points[6][5] = points[6][1] + cp2y;

   points[7][0] = points[5][0] + ia1x;
   points[7][1] = points[5][1] + ia1y;
   points[7][6] = points[5][0] + ia2x;
   points[7][7] = points[5][1] + ia2y;

   points[7][2] = points[7][0] + cp1x;
   points[7][3] = points[7][1] + cp1y;
   points[7][4] = points[7][0] + cp2x;
   points[7][5] = points[7][1] + cp2y;

   // --- spline 8 - used for 'd'
   // mirror upper points to lower with same x
   points[8][6] =  points[8][0]; // same x
   points[8][7] = -points[8][1]; // mirror y
   points[8][4] =  points[8][2]; // same x
   points[8][5] = -points[8][3]; // mirror y
}


void mdw_an3(int x, int y, float sc, int th)
{
   float mdw_help_logo_scale = sc;

   float x_scale = mdw_help_logo_scale;
   float y_scale = mdw_help_logo_scale;

   // how many seq?

   // 1st static 128  0    - 127
   // y rot 256       128  - 383
   // 2nd static 128  384  - 511
   // x rot 192       512  - 703
   // xy rot 256      704  - 959
   // xlink 64        960  - 1023
   // total 1024

   if (++mdw_an_seq > 1023) mdw_an_seq = 0;
   al_fixed t;

   // 1st static
   if (mdw_an_seq < 128)
   {
      x_scale = mdw_help_logo_scale;
      y_scale = mdw_help_logo_scale;
   }
   // y rot
   if ((mdw_an_seq > 127) && (mdw_an_seq < 384))
   {
      t = al_itofix(mdw_an_seq - 128);
      y_scale = mdw_help_logo_scale * al_fixtof(al_fixcos(t));
   }
   // 2nd static
   if ((mdw_an_seq > 383) && (mdw_an_seq < 512))
   {
      x_scale = mdw_help_logo_scale;
      y_scale = mdw_help_logo_scale;
   }
   // x rot
   if ((mdw_an_seq > 511) && (mdw_an_seq < 704))
   {
      t = al_itofix(mdw_an_seq - 512);
      x_scale = mdw_help_logo_scale * al_fixtof(al_fixcos(t));
   }
   // xy rot
   if ((mdw_an_seq > 703) && (mdw_an_seq < 960))
   {
      t = al_itofix(mdw_an_seq - 704);
      x_scale = mdw_help_logo_scale * al_fixtof(al_fixsin(t));
      y_scale = mdw_help_logo_scale * al_fixtof(al_fixcos(t));
   }
   // x link
   if ((mdw_an_seq > 959) && (mdw_an_seq < 1024))
   {
      t = al_itofix(mdw_an_seq - 960);
      x_scale = mdw_help_logo_scale * al_fixtof(al_fixsin(t));
   }
   draw_mdw(x, y, x_scale, y_scale, -1);
}


int mdw_an2(void)
{
   float x_scale = mdw_splash_logo_scale;
   float y_scale = mdw_splash_logo_scale;

   // 1st grow while rotating 256   0    - 255
   // 2nd xlink                64   256  - 319
   // 3nd static              128   320  - 447
   // 4th spin back for dual  192   448  - 639
   // 5th dual flip           256   640  - 895
   // 6th spin back to orig    64   896  - 959
   // total shrink frames 320 (mode 5 and 6)

   if (++mdw_an_seq > 959) return 1;

   // grow and spin in both axis
   if ((mdw_an_seq > -1) && (mdw_an_seq < 256))
   {
      al_fixed t = al_itofix(mdw_an_seq);
      float s = (float)mdw_an_seq / 320;
      if (s > 1) s = 1;
      x_scale = s * mdw_splash_logo_scale * al_fixtof(al_fixsin(t));
      y_scale = s * mdw_splash_logo_scale * al_fixtof(al_fixcos(t));
   }
   // fix x scale
   if ((mdw_an_seq > 255) && (mdw_an_seq < 319))
   {
      al_fixed t = al_itofix(mdw_an_seq-256);
      float s = (float)mdw_an_seq / 320;
      if (s > 1) s = 1;
      x_scale = s * mdw_splash_logo_scale * al_fixtof(al_fixsin(t));
      y_scale = s * mdw_splash_logo_scale;
   }
   // freeze
   if ((mdw_an_seq > 319) && (mdw_an_seq < 448))
   {
      x_scale = mdw_splash_logo_scale;
      y_scale = mdw_splash_logo_scale;
   }
   // spin back to prepare for dual flip...
   if ((mdw_an_seq > 447) && (mdw_an_seq < 640))
   {
      al_fixed t = al_itofix(mdw_an_seq-448);
      x_scale = mdw_splash_logo_scale * al_fixtof(al_fixcos(t));
   }
   // shrink and move
   if ((mdw_an_seq > 639) && (mdw_an_seq < 960))
   {
      mdw_splash_logo_x -= mdw_logo_x_dec;
      mdw_splash_logo_y -= mdw_logo_y_dec;
      mdw_splash_logo_scale -= mdw_logo_scale_dec;
   }
   // dual flip
   if ((mdw_an_seq > 639) && (mdw_an_seq < 896))
   {
      al_fixed t = al_itofix(mdw_an_seq-640);
      x_scale = mdw_splash_logo_scale * al_fixtof(al_fixsin(t));
      y_scale = mdw_splash_logo_scale * al_fixtof(al_fixcos(t));
   }
   // back to original
   if ((mdw_an_seq > 895) && (mdw_an_seq < 960))
   {
      al_fixed t = al_itofix(mdw_an_seq-896);
      x_scale = mdw_splash_logo_scale * al_fixtof(al_fixsin(t));
      y_scale = mdw_splash_logo_scale;
   }
   //draw_mdw((int)mdw_splash_logo_x, (int)mdw_splash_logo_y, x_scale, y_scale, mdw_splash_logo_th);
   draw_mdw((int)mdw_splash_logo_x, (int)mdw_splash_logo_y, x_scale, y_scale, -1);
   return 0;
}

void mdw_an(void)
{
   float x_scale = mdw_map_logo_scale;
   float y_scale = mdw_map_logo_scale;

   // how many seq?

   // 1st static 128  0    - 127
   // y rot 256       128  - 383
   // 2nd static 128  384  - 511
   // x rot 192       512  - 703
   // xy rot 256      704  - 959
   // xlink 64        960  - 1023
   // total 1024

   if (++mdw_an_seq > 1023) mdw_an_seq = 0;
   al_fixed t;

   // 1st static
   if (mdw_an_seq < 128)
   {
      x_scale = mdw_map_logo_scale;
      y_scale = mdw_map_logo_scale;
   }
   // y rot
   if ((mdw_an_seq > 127) && (mdw_an_seq < 384))
   {
      t = al_itofix(mdw_an_seq - 128);
      y_scale = mdw_map_logo_scale * al_fixtof(al_fixcos(t));
   }
   // 2nd static
   if ((mdw_an_seq > 383) && (mdw_an_seq < 512))
   {
      x_scale = mdw_map_logo_scale;
      y_scale = mdw_map_logo_scale;
   }
   // x rot
   if ((mdw_an_seq > 511) && (mdw_an_seq < 704))
   {
      t = al_itofix(mdw_an_seq - 512);
      x_scale = mdw_map_logo_scale * al_fixtof(al_fixcos(t));
   }
   // xy rot
   if ((mdw_an_seq > 703) && (mdw_an_seq < 960))
   {
      t = al_itofix(mdw_an_seq - 704);
      x_scale = mdw_map_logo_scale * al_fixtof(al_fixsin(t));
      y_scale = mdw_map_logo_scale * al_fixtof(al_fixcos(t));
   }
   // x link
   if ((mdw_an_seq > 959) && (mdw_an_seq < 1024))
   {
      t = al_itofix(mdw_an_seq - 960);
      x_scale = mdw_map_logo_scale * al_fixtof(al_fixsin(t));
   }
   //draw_mdw(mdw_map_logo_x, mdw_map_logo_y, x_scale, y_scale, mdw_map_logo_th);
   draw_mdw(mdw_map_logo_x, mdw_map_logo_y, x_scale, y_scale, -1);
}





void spline_test(void)
{
   seed_mdw();
   fill_mdw();

   float x_scale = 1.0;
   float y_scale = 1.0;
   int th = -1;

   float x_scale_inc = 0.01;
   float y_scale_inc = 0.01;


   int quit = 0;
   while (!quit)
   {
      proc_controllers();

      if (key[ALLEGRO_KEY_RIGHT])
      {
         x_scale += x_scale_inc;
      }
      if (key[ALLEGRO_KEY_LEFT])
      {
         x_scale -= x_scale_inc;

      }
      if (key[ALLEGRO_KEY_UP])
      {
         y_scale += y_scale_inc;
      }
      if (key[ALLEGRO_KEY_DOWN])
      {
         y_scale -= y_scale_inc;
      }

      if (key[ALLEGRO_KEY_I])
      {
         th++;
      }
      if (key[ALLEGRO_KEY_K])
      {
         th--;
      }

      al_clear_to_color(al_map_rgb(0,0,0));

      draw_mdw(SCREEN_W/2, SCREEN_H/2, x_scale, y_scale, th);
      al_flip_display();

      al_rest(0.02);
      if (key[ALLEGRO_KEY_ESCAPE]) quit = 1;
   }
}



void redraw_spline(int s)
{
   fill_mdw();
   draw_mdw(200, 200, 1.0, 1.0, -1);
   al_draw_textf(font, palette_color[15], 100, 402, 0, "current spline:%d", s);

   for (int i=0; i<8; i+=2)
   {
      al_draw_circle(points[s][i]+200, points[s][i+1]+200, 4, palette_color[11], 1);
      al_draw_textf(font, palette_color[15], 100, 410+i*4, 0, "x:%-4.0f y:%-4.0f", points[s][i], points[s][i+1] );
   }
}







void spline_adjust(void)
{
   int current_spline = 0;
   seed_mdw();
   fill_mdw();
   int quit = 0;
   while (!quit)
   {
      al_hide_mouse_cursor(display);
      redraw_spline(current_spline);
      al_flip_display();
      al_clear_to_color(al_map_rgb(0,0,0));
      al_show_mouse_cursor(display);
      proc_controllers();
      al_rest(0.02);

      int mx = mouse_x-200;
      int my = mouse_y-200;
      for (int i=0; i<8; i+=2)
      {
         int px = points[current_spline][i];
         int py = points[current_spline][i+1];
         if ((mx > px-4) && (mx < px+4) && (my > py-4) && (my < py+4))
         {
            al_draw_circle(points[current_spline][i]+200, points[current_spline][i+1]+200, 6, palette_color[10], 2);
            while (mouse_b1)
            {
               al_hide_mouse_cursor(display);
               redraw_spline(current_spline);
               al_flip_display();
               al_clear_to_color(al_map_rgb(0,0,0));
               al_draw_circle(points[current_spline][i]+200, points[current_spline][i+1]+200, 6, palette_color[10], 3);
               al_show_mouse_cursor(display);
               proc_controllers();
               al_rest(0.02);

               points[current_spline][i] = mouse_x-200;
               points[current_spline][i+1] = mouse_y-200;
            }
         }
      }

      if (key[ALLEGRO_KEY_UP])
      {
         while (key[ALLEGRO_KEY_UP]) proc_controllers();
         if (current_spline == 0) current_spline = 2;
         else if (current_spline == 2) current_spline = 8;
      }
      if (key[ALLEGRO_KEY_DOWN])
      {
         while (key[ALLEGRO_KEY_DOWN])proc_controllers();
         if (current_spline == 2) current_spline = 0;
         else if (current_spline == 8) current_spline = 2;
      }
      while ((key[ALLEGRO_KEY_ESCAPE]) || (mouse_b2))
      {
         proc_controllers();
         quit = 1;
      }
   }
}

/*

int sct[64][5];

ALLEGRO_BITMAP *bmsg_icon;


void draw_tile_double(int ct, int x, int y, int db)
{
   int tn = sct[ct][0];
   int sx = sct[ct][1];
   int sy = sct[ct][2];
   int sw = sct[ct][3];
   int sh = sct[ct][4];

   al_draw_scaled_bitmap(tile[tn], 0, 0, 20, 20, x, y, db*20, db*20, 0); // draw shape

   al_draw_textf(font, palette_color[15], x+10*db, y-10, ALLEGRO_ALIGN_CENTRE, "Source Tile %d", tn);


   al_draw_textf(font, palette_color[15], x+10, y+20*db+10, 0, "sx:%d", sx);
   al_draw_textf(font, palette_color[15], x+10, y+20*db+18, 0, "sy:%d", sy);
   al_draw_textf(font, palette_color[15], x+10, y+20*db+26, 0, "sw:%d", sw);
   al_draw_textf(font, palette_color[15], x+10, y+20*db+34, 0, "sh:%d", sh);




   //show all gridlines
   for (int i=0; i<21; i++)
   {
      al_draw_line(x, y+i*db, x+20*db, y+i*db, palette_color[15], 0);
      al_draw_line(x+i*db, y, x+i*db, y+20*db, palette_color[15], 0);
   }
   // show source box
   al_draw_rectangle(x+sx*db, y+sy*db, x+(sx+sw)*db, y+(sy+sh)*db, palette_color[14], 1 );

   // show ul and lr control points
   al_draw_circle(x+sx*db, y+sy*db, 4, palette_color[14], 0 );
   al_draw_circle(x+(sx+sw)*db, y+(sy+sh)*db, 4, palette_color[14], 0 );



}



void draw_st_double(ALLEGRO_BITMAP *st, int x, int y, int db)
{
   int stw = al_get_bitmap_width(st);
   int sth = al_get_bitmap_height(st);

   al_draw_scaled_bitmap(st, 0, 0, stw, sth, x, y, stw*db, sth*db, 0); // draw shape

   al_draw_textf(font, palette_color[15], x + (db*stw)/2, y-10, ALLEGRO_ALIGN_CENTRE, "%d x %d", stw, sth);


   //show all gridlines
   for (int i=0; i<stw+1; i++)
   {
      al_draw_line(x, y+i*db, x+stw*db, y+i*db, palette_color[15], 0);
      al_draw_line(x+i*db, y, x+i*db, y+stw*db, palette_color[15], 0);
   }
}



void show_new_tiles(int x, int y, int db)
{
   al_set_target_bitmap(bmsg_icon);
   al_clear_to_color(al_map_rgb(0,0,0));
   for (int i=0; i<64; i++)
   {
      int tn = sct[i][0];
      int sx = sct[i][1];
      int sy = sct[i][2];
      int sw = sct[i][3];
      int sh = sct[i][4];
      al_set_target_bitmap(bmsg_icon);
      al_draw_scaled_bitmap(tile[tn], sx, sy, sw, sh, i*8, 0, 8, 8, 0); // draw shape

      al_set_target_backbuffer(display);
      al_draw_scaled_bitmap(tile[tn], sx, sy, sw, sh, x+i*8, y-10, 8, 8, 0); // draw shape


   }
   al_set_target_backbuffer(display);
   al_draw_scaled_bitmap(bmsg_icon, 0, 0, 512, 8, x, y, 512*db, 8*db, 0); // draw shape

}






void check_sct_bounds(int ct)
{

   if (sct[ct][1] < 0) sct[ct][1] = 0;
   if (sct[ct][2] < 0) sct[ct][2] = 0;
   if (sct[ct][1] > 20) sct[ct][1] = 20;
   if (sct[ct][2] > 20) sct[ct][2] = 20;

   // prevent adjusting tl from pushing lr over 20
   if (sct[ct][1] + sct[ct][3] > 20) sct[ct][3] = 20 - sct[ct][1];
   if (sct[ct][2] + sct[ct][4] > 20) sct[ct][4] = 20 - sct[ct][2];
}





void scaled_tile_test(void)
{
   // show what tiles look like when they are scaled

   for (int i=0; i<64; i++)
      for (int j=0; j<5; j++) sct[i][j] = 0;

   for (int i=0; i<64; i++ )
   {
      sct[i][3] = 20;
      sct[i][4] = 20;
   }


   sct[0][0] = 496; // archwagon
   sct[0][1] = 0;   // source x
   sct[0][2] = 2;   // source y
   sct[0][3] = 20;  // source w
   sct[0][4] = 17;  // source h

   sct[1][0] = 508; // bouncer
   sct[1][1] = 1;   // source x
   sct[1][2] = 0;   // source y
   sct[1][3] = 18;  // source w
   sct[1][4] = 20;  // source h

   sct[2][0] = 415; // cannon
   sct[2][1] = 3;   // source x
   sct[2][2] = 1;   // source y
   sct[2][3] = 14;  // source w
   sct[2][4] = 17;  // source h

   sct[3][0] = 374; // podzilla
   sct[3][1] = 2;   // source x
   sct[3][2] = 1;   // source y
   sct[3][3] = 16;  // source w
   sct[3][4] = 18;  // source h

   sct[4][0] = 384; // trakbot
   sct[4][1] = 0;   // source x
   sct[4][2] = 0;   // source y
   sct[4][3] = 18;  // source w
   sct[4][4] = 20;  // source h

   sct[5][0] = 550; // cloner
   sct[5][1] = 0;   // source x
   sct[5][2] = 0;   // source y
   sct[5][3] = 20;  // source w
   sct[5][4] = 20;  // source h

   sct[6][0] = 866; // block walker
   sct[6][1] = 0;   // source x
   sct[6][2] = 0;   // source y
   sct[6][3] = 20;  // source w
   sct[6][4] = 20;  // source h

   sct[7][0] = 159; // flapper
   sct[7][1] = 1;   // source x
   sct[7][2] = 0;   // source y
   sct[7][3] = 17;  // source w
   sct[7][4] = 14;  // source h



   sct[9][0] = 249; // rocket
   sct[9][1] = 0;   // source x
   sct[9][2] = 0;   // source y
   sct[9][3] = 20;  // source w
   sct[9][4] = 20;  // source h

   sct[10][0] = 465; // bomb
   sct[10][1] = 0;   // source x
   sct[10][2] = 0;   // source y
   sct[10][3] = 20;  // source w
   sct[10][4] = 20;  // source h

   sct[11][0] = 537; // bomb with detonator
   sct[11][1] = 0;   // source x
   sct[11][2] = 0;   // source y
   sct[11][3] = 20;  // source w
   sct[11][4] = 20;  // source h

   sct[12][0] = 538; // bomb with no detonator
   sct[12][1] = 0;   // source x
   sct[12][2] = 0;   // source y
   sct[12][3] = 20;  // source w
   sct[12][4] = 20;  // source h

   sct[13][0] = 539; // detonator
   sct[13][1] = 2;   // source x
   sct[13][2] = 0;   // source y
   sct[13][3] = 16;  // source w
   sct[13][4] = 16;  // source h



   sct[14][0] = 400; // player 0
   sct[14][1] = 0;   // source x
   sct[14][2] = 0;   // source y
   sct[14][3] = 20;  // source w
   sct[14][4] = 20;  // source h

   sct[15][0] = 401; // player 1
   sct[15][1] = 0;   // source x
   sct[15][2] = 0;   // source y
   sct[15][3] = 20;  // source w
   sct[15][4] = 20;  // source h


   sct[16][0] = 928; // banana
   sct[16][1] = 3;   // source x
   sct[16][2] = 0;   // source y
   sct[16][3] = 17;  // source w
   sct[16][4] = 20;  // source h

   sct[17][0] = 929; // apple
   sct[17][1] = 0;   // source x
   sct[17][2] = 0;   // source y
   sct[17][3] = 20;  // source w
   sct[17][4] = 20;  // source h

   sct[18][0] = 930; // white flower
   sct[18][1] = 0;   // source x
   sct[18][2] = 0;   // source y
   sct[18][3] = 20;  // source w
   sct[18][4] = 20;  // source h

   sct[19][0] = 931; // purple flower
   sct[19][1] = 0;   // source x
   sct[19][2] = 0;   // source y
   sct[19][3] = 20;  // source w
   sct[19][4] = 20;  // source h

   sct[20][0] = 932; // blue cherries
   sct[20][1] = 0;   // source x
   sct[20][2] = 0;   // source y
   sct[20][3] = 20;  // source w
   sct[20][4] = 20;  // source h

   sct[21][0] = 933; // carrot
   sct[21][1] = 0;   // source x
   sct[21][2] = 0;   // source y
   sct[21][3] = 20;  // source w
   sct[21][4] = 20;  // source h

   sct[22][0] = 934; // red flower
   sct[22][1] = 0;   // source x
   sct[22][2] = 0;   // source y
   sct[22][3] = 20;  // source w
   sct[22][4] = 20;  // source h

   sct[23][0] = 304; // 10 potion
   sct[23][1] = 1;   // source x
   sct[23][2] = 0;   // source y
   sct[23][3] = 19;  // source w
   sct[23][4] = 20;  // source h

   sct[24][0] = 240; // 50 potion
   sct[24][1] = 1;   // source x
   sct[24][2] = 0;   // source y
   sct[24][3] = 19;  // source w
   sct[24][4] = 20;  // source h

   sct[25][0] = 265; // free man
   sct[25][1] = 0;   // source x
   sct[25][2] = 0;   // source y
   sct[25][3] = 19;  // source w
   sct[25][4] = 20;  // source h


   sct[26][0] = 398; // exit
   sct[26][1] = 1;   // source x
   sct[26][2] = 0;   // source y
   sct[26][3] = 20;  // source w
   sct[26][4] = 19;  // source h


   sct[27][0] = 367; // locked exit
   sct[27][1] = 1;   // source x
   sct[27][2] = 0;   // source y
   sct[27][3] = 19;  // source w
   sct[27][4] = 20;  // source h


   sct[28][0] = 456; // mine
   sct[28][1] = 2;   // source x
   sct[28][2] = 2;   // source y
   sct[28][3] = 16;  // source w
   sct[28][4] = 16;  // source h



   sct[32][0] = 960; // yellow diamond
   sct[32][1] = 0;   // source x
   sct[32][2] = 4;   // source y
   sct[32][3] = 20;  // source w
   sct[32][4] = 16;  // source h

   sct[33][0] = 961; // red diamond
   sct[33][1] = 0;   // source x
   sct[33][2] = 4;   // source y
   sct[33][3] = 20;  // source w
   sct[33][4] = 16;  // source h

   sct[34][0] = 962; // white diamond
   sct[34][1] = 0;   // source x
   sct[34][2] = 4;   // source y
   sct[34][3] = 20;  // source w
   sct[34][4] = 16;  // source h

   sct[35][0] = 963; // purple diamond
   sct[35][1] = 0;   // source x
   sct[35][2] = 4;   // source y
   sct[35][3] = 20;  // source w
   sct[35][4] = 16;  // source h

   sct[36][0] = 964; // blue diamond
   sct[36][1] = 0;   // source x
   sct[36][2] = 4;   // source y
   sct[36][3] = 20;  // source w
   sct[36][4] = 16;  // source h

   sct[37][0] = 965; // orange diamond
   sct[37][1] = 0;   // source x
   sct[37][2] = 4;   // source y
   sct[37][3] = 20;  // source w
   sct[37][4] = 16;  // source h

   sct[38][0] = 966; // green diamond
   sct[38][1] = 0;   // source x
   sct[38][2] = 4;   // source y
   sct[38][3] = 20;  // source w
   sct[38][4] = 16;  // source h



   sct[48][0] = 272; // red key
   sct[48][1] = 1;   // source x
   sct[48][2] = 5;   // source y
   sct[48][3] = 19;  // source w
   sct[48][4] = 10;  // source h

   sct[49][0] = 279; // green key
   sct[49][1] = 1;   // source x
   sct[49][2] = 5;   // source y
   sct[49][3] = 19;  // source w
   sct[49][4] = 10;  // source h

   sct[50][0] = 288; // blue key
   sct[50][1] = 1;   // source x
   sct[50][2] = 5;   // source y
   sct[50][3] = 19;  // source w
   sct[50][4] = 10;  // source h

   sct[51][0] = 295; // purple key
   sct[51][1] = 1;   // source x
   sct[51][2] = 5;   // source y
   sct[51][3] = 19;  // source w
   sct[51][4] = 10;  // source h


   sct[52][0] = 745; // green switch
   sct[52][1] = 2;   // source x
   sct[52][2] = 2;   // source y
   sct[52][3] = 17;  // source w
   sct[52][4] = 18;  // source h

   sct[53][0] = 777; // red switch
   sct[53][1] = 2;   // source x
   sct[53][2] = 2;   // source y
   sct[53][3] = 17;  // source w
   sct[53][4] = 18;  // source h

   sct[54][0] = 809; // blue switch
   sct[54][1] = 2;   // source x
   sct[54][2] = 2;   // source y
   sct[54][3] = 17;  // source w
   sct[54][4] = 18;  // source h

   sct[55][0] = 841; // purple switch
   sct[55][1] = 2;   // source x
   sct[55][2] = 2;   // source y
   sct[55][3] = 17;  // source w
   sct[55][4] = 18;  // source h





   int ct = 0; //current tile index

   int tn = sct[ct][0];
   int sx = sct[ct][1];
   int sy = sct[ct][2];
   int sw = sct[ct][3];
   int sh = sct[ct][4];


   bmsg_icon = al_create_bitmap(512, 8);



   ALLEGRO_BITMAP *t88;
   t88 = al_create_bitmap(8, 8);

   ALLEGRO_BITMAP *t1010;
   t1010 = al_create_bitmap(10, 10);

   al_set_target_backbuffer(display);
   int quit = 0;
   while (!quit)
   {
      al_hide_mouse_cursor(display);
      al_flip_display();
      al_clear_to_color(al_map_rgb(0,0,0));
      al_show_mouse_cursor(display);
      al_rest(0.02);
      proc_controllers();



      sx = sct[ct][1];
      sy = sct[ct][2];
      sw = sct[ct][3];
      sh = sct[ct][4];


      // position of the big source tile and double
      int bstx = 10;
      int bsty = 20;
      int bstd = 16;

      int bstx2 = bstx + (bstd*20);
      int bsty2 = bsty + (bstd*20);


      // draw the big source tile
      draw_tile_double(ct, bstx, bsty, bstd);


      int mx = mouse_x;
      int my = mouse_y;


      if ((mx > bstx-1) && (mx < bstx2+bstd) && (my > bsty-1) && (my < bstx2+bstd)) // mouse on bst ?
      {
         al_draw_rectangle(bstx-1, bsty-1, bstx2+1, bsty2+1, palette_color[10], 1 ); // highlight the big source tile
         mx += bstd/2;
         my += bstd/2;
         int mbx = (mx - bstx) / bstd;
         int mby = (my - bsty) / bstd;
         //al_draw_circle(bstx + (mbx * bstd), bsty + (mby * bstd), 4, palette_color[12], 0); // show where the mouse is
         if ((mbx == sct[ct][1]) && (mby == sct[ct][2])) // mouse on ul point ?
         {
            al_draw_circle(bstx + (mbx * bstd), bsty + (mby * bstd), 6, palette_color[10], 2); // highlight ul to show it can be moved
            while (mouse_b1)
            {
               al_hide_mouse_cursor(display);
               draw_tile_double(ct, bstx, bsty, bstd); // draw the big source tile
               // al_draw_textf(font, palette_color[15], bstx + 10, bsty2+80, 0, "%d -  %d", mbx, mby);
               al_flip_display();
               al_clear_to_color(al_map_rgb(0,0,0));
               al_show_mouse_cursor(display);
               proc_controllers();
               al_rest(0.02);
               mx = mouse_x + bstd/2;
               my = mouse_y + bstd/2;
               mbx = (mx - bstx) / bstd;
               mby = (my - bsty) / bstd;
               sct[ct][1] = mbx;
               sct[ct][2] = mby;
               check_sct_bounds(ct);
            }
         }
         if ((mbx == sx+sw) && (mby == sy+sh)) // mouse on ul point?
         {
            al_draw_circle(bstx + (mbx * bstd), bsty + (mby * bstd), 6, palette_color[10], 2); // highlight lr to show it can be moved
            while (mouse_b1)
            {
               al_hide_mouse_cursor(display);
               draw_tile_double(ct, bstx, bsty, bstd); // draw the big source tile
               //al_draw_textf(font, palette_color[15], bstx + 10, bsty2+80, 0, "%d -  %d", mbx, mby);
               al_flip_display();
               al_clear_to_color(al_map_rgb(0,0,0));
               al_show_mouse_cursor(display);
               proc_controllers();
               al_rest(0.02);
               mx = mouse_x + bstd/2;
               my = mouse_y + bstd/2;
               mbx = (mx - bstx) / bstd;
               mby = (my - bsty) / bstd;
               sct[ct][3] = mbx - sct[ct][1];
               sct[ct][4] = mby - sct[ct][2];
               check_sct_bounds(ct);
            }
         }
      }

      int tsx = 10;
      int tsy = 400;
      int tsx2 = tsx + 320;
      int tsy2 = tsy + 80;


      // draw tile select 16 x 4
      for (int i=0; i<16; i++)
      {
         al_draw_bitmap(tile[sct[i   ][0]], tsx+i*20, tsy,    0);
         al_draw_bitmap(tile[sct[i+16][0]], tsx+i*20, tsy+20, 0);
         al_draw_bitmap(tile[sct[i+32][0]], tsx+i*20, tsy+40, 0);
         al_draw_bitmap(tile[sct[i+48][0]], tsx+i*20, tsy+60, 0);
      }
      al_draw_rectangle(tsx-1, tsy-1, tsx2+1, tsy2+1, palette_color[15], 1 ); // outline tile select

      al_draw_text(font, palette_color[15], tsx + 160, tsy-10, ALLEGRO_ALIGN_CENTRE, "Tile Select");


      if ((mx > tsx-1) && (mx < tsx2+1) && (my > tsy-1) && (my < tsy2+1)) // mouse on tile select
      {
         al_draw_rectangle(tsx-2, tsy-2, tsx2+2, tsy2+2, palette_color[10], 1 ); // highlight tile select

         int mbx = (mx - tsx) / 20; // what tile is mouse pointing at?
         int mby = (my - tsy) / 20;

         al_draw_rectangle(tsx+mbx*20, tsy+mby*20, tsx+mbx*20+20, tsy+mby*20+20, palette_color[10], 1 ); // highlight tile select

         while (mouse_b1)
         {
            proc_controllers();
            ct = mby*16 + mbx;
         }

      }


































      tn = sct[ct][0];
      sx = sct[ct][1];
      sy = sct[ct][2];
      sw = sct[ct][3];
      sh = sct[ct][4];

      // scale to 8x8
      al_set_target_bitmap(t88);
      al_clear_to_color(al_map_rgb(0,0,0));
      al_draw_scaled_bitmap(tile[tn], sx, sy, sw, sh, 0, 0, 8, 8, 0);

      // scale to 10x10
      al_set_target_bitmap(t1010);
      al_clear_to_color(al_map_rgb(0,0,0));
      al_draw_scaled_bitmap(tile[tn], sx, sy, sw, sh, 0, 0, 10, 10, 0);




      al_set_target_backbuffer(display);

      draw_st_double(t1010, 400, 40, 10);
      draw_st_double(t88,   400, 200, 10);

      show_new_tiles(10, 600, 1);

      show_new_tiles(10, 640, 2);











      if (key[ALLEGRO_KEY_M])
      {
         while (key[ALLEGRO_KEY_M]) proc_controllers();

      }


      if (key[ALLEGRO_KEY_W])
      {
         while (key[ALLEGRO_KEY_W]) proc_controllers();

         if (++ct > 15) ct =0;
      }


      if (key[ALLEGRO_KEY_UP])
      {
         while (key[ALLEGRO_KEY_UP]) proc_controllers();
         if (mode == 0) sct[ct][2]-=1;
         if (mode == 1) sct[ct][4]-=1;
      }



      if (key[ALLEGRO_KEY_DOWN])
      {
         while (key[ALLEGRO_KEY_DOWN])proc_controllers();
         if (mode == 0) sct[ct][2]+=1;
         if (mode == 1) sct[ct][4]+=1;
      }

      if (key[ALLEGRO_KEY_RIGHT])
      {
         while (key[ALLEGRO_KEY_RIGHT]) proc_controllers();
         if (mode == 0) sct[ct][1]+=1;
         if (mode == 1) sct[ct][3]+=1;
      }
      if (key[ALLEGRO_KEY_LEFT])
      {
         while (key[ALLEGRO_KEY_LEFT]) proc_controllers();
         if (mode == 0) sct[ct][1]-=1;
         if (mode == 1) sct[ct][3]-=1;

      }



      while ((key[ALLEGRO_KEY_ESCAPE]) || (mouse_b2))
      {
         proc_controllers();
         quit = 1;
      }

      check_sct_bounds(ct);

   }
   al_destroy_bitmap(t88);
   al_destroy_bitmap(t1010);
   al_destroy_bitmap(bmsg_icon);
}


*/
