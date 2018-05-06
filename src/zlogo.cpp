#include "pm.h"



// i want a function that will draw centered text to a specified percentage of screen size


// used by "created by:"
void mw_text(ALLEGRO_FONT *tf, int col, float x_pc, char * txt)
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

   //al_draw_line(0, SCREEN_H/2, SCREEN_W, SCREEN_H/2, palette_color[15], 1);
   //al_draw_line(SCREEN_W/2, 0, SCREEN_W/2, SCREEN_H, palette_color[15], 1);

   al_destroy_bitmap(t1);
}

// Demo Mode
// Purple Martians
// Level Done
// You Died!


void draw_title(int tx, int ty, int tw, int th, int color)
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
   al_draw_scaled_bitmap(text_title, 0, 0, bbw1, bbh1, tx - tw/2, ty, tw, th, 0);
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



void draw_large_2lines(ALLEGRO_FONT *tf, char * m1, char * m2, int color, float opa )
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

   draw_large_2lines(f2, (char*)"Purple", (char*)"Martians", 8, 1);
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
      mw_text(f2, 12, .80, (char*)"created by:");
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

   al_set_timer_speed(mnu_timer, .005);
   while (!quit)
   {
      al_clear_to_color(al_map_rgb(0,0,0));
      quit = mdw_an2();
      al_flip_display();
      if (proc_controllers()) quit = 1;
   }
   al_set_timer_speed(mnu_timer, .01);

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
   int c1 = 10; //color 1
   int c2 = 8;  //color 2
   int c3 = 10; //color 3

//   int t = line_thickness;

//   float max_scale = fabs(x_scale);
//   if (fabs(y_scale) > fabs(x_scale)) max_scale = fabs(y_scale);
//   float t = 2.5 + max_scale *6;

   float hy_scale = sqrt( (x_scale * x_scale) + (y_scale * y_scale));
   float t = 2 + hy_scale * 7;

//   if (t<1) t = 1;
//   printf("t:%d\n", t);

   float draw_points[10][8];

   // apply x_scale
   for (int j=0; j<10; j++)
      for (int i=0; i<8; i+=2)
      {
         draw_points[j][i]   = points[j][i]   * x_scale;
         draw_points[j][i+1] = points[j][i+1] * y_scale;
      }

   // apply offset
   for (int j=0; j<10; j++)
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
      mfspline(draw_points[4], c3, t);
      mfspline(draw_points[2], c1, t);
      mfspline(draw_points[3], c1, t);
      mfspline(draw_points[8], c1, t);
      mfspline(draw_points[9], c1, t);
      mfspline(draw_points[0], c2, t);
      mfspline(draw_points[1], c2, t);
      mfspline(draw_points[6], c2, t);
      mfspline(draw_points[7], c2, t);
   }
   else
   {
      mfspline(draw_points[0], c2, t);
      mfspline(draw_points[1], c2, t);
      mfspline(draw_points[6], c2, t);
      mfspline(draw_points[7], c2, t);
      mfspline(draw_points[4], c3, t);
      mfspline(draw_points[2], c1, t);
      mfspline(draw_points[3], c1, t);
      mfspline(draw_points[8], c1, t);
      mfspline(draw_points[9], c1, t);
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
// 4 D
// 5 rsvd (not used)
// 6 7 outer W
// 8 9 inner W

// 0 is copied to 1 6 7
// 2 is copied to 3 8 9

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

   points[6][0] = 0;
   points[6][1] = 0;

   points[6][6] = 0;
   points[6][7] = 200;

   points[7][0] = 200;
   points[7][1] = 0;
   points[7][6] = 200;
   points[7][7] = 200;

   // outer arm 0 control points are relative to outer arm 0 positon
   points[0][2] = points[0][6] + 85;
   points[0][3] = points[0][7] + 107;
   points[0][4] = points[0][6] + 95;
   points[0][5] = points[0][7] + 31;

   // inner arm is relative to outer arm
   points[2][0] = points[0][6] + 30;
   points[2][1] = points[0][7] + 30;
   points[2][2] = points[0][6] + 77;
   points[2][3] = points[0][7] + 32;
   points[2][4] = points[0][6] + 93;
   points[2][5] = points[0][7] + 53;
   points[2][6] = points[0][6] + 109;
   points[2][7] = points[0][7] + 69;


/*
   points[0][2] = points[0][6] + 72;
   points[0][3] = points[0][7] + 115;
   points[0][4] = points[0][6] + 95;
   points[0][5] = points[0][7] + 31;

   points[2][0] = points[0][6] + 30;
   points[2][1] = points[0][7] + 30;
   points[2][2] = points[0][6] + 77;
   points[2][3] = points[0][7] + 32;
   points[2][4] = points[0][6] + 93;
   points[2][5] = points[0][7] + 53;
   points[2][6] = points[0][6] + 103;
   points[2][7] = points[0][7] + 88;

*/
   // 'D' has only 1 spline, start point and first control point are mirrored in the y axis
   points[4][0] = -70;
   points[4][1] = -40;
   points[4][2] = 132;
   points[4][3] = -88;
}



void fill_mdw(void)
{
   // get control point 1 from spline 0
   float cp1x = points[0][0] - points[0][2];
   float cp1y = points[0][1] - points[0][3];

   // get control point 2 from spline 0
   float cp2x = points[0][0] - points[0][4];
   float cp2y = points[0][1] - points[0][5];

   // apply to other splines
   points[1][2] = points[1][0] + cp1x;
   points[1][3] = points[1][1] - cp1y;
   points[1][4] = points[1][0] + cp2x;
   points[1][5] = points[1][1] - cp2y;

   points[6][2] = points[6][0] - cp1x;
   points[6][3] = points[6][1] + cp1y;
   points[6][4] = points[6][0] - cp2x;
   points[6][5] = points[6][1] + cp2y;

   points[7][2] = points[7][0] + cp1x;
   points[7][3] = points[7][1] + cp1y;
   points[7][4] = points[7][0] + cp2x;
   points[7][5] = points[7][1] + cp2y;

   // these are the 4 inner arms and their positions are relative to the outer arms (or center?)

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

   points[8][0] = points[6][0] - ia1x;
   points[8][1] = points[6][1] + ia1y;
   points[8][6] = points[6][0] - ia2x;
   points[8][7] = points[6][1] + ia2y;

   points[8][2] = points[8][0] - cp1x;
   points[8][3] = points[8][1] + cp1y;
   points[8][4] = points[8][0] - cp2x;
   points[8][5] = points[8][1] + cp2y;

   points[9][0] = points[7][0] + ia1x;
   points[9][1] = points[7][1] + ia1y;
   points[9][6] = points[7][0] + ia2x;
   points[9][7] = points[7][1] + ia2y;

   points[9][2] = points[9][0] + cp1x;
   points[9][3] = points[9][1] + cp1y;
   points[9][4] = points[9][0] + cp2x;
   points[9][5] = points[9][1] + cp2y;

   // d adjust
   points[4][6] = points[4][0]; // same x
   points[4][7] = -points[4][1]; // mirror y
   points[4][4] = points[4][2]; // same x
   points[4][5] = -points[4][3]; // mirror y
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
   draw_mdw(x, y, x_scale, y_scale, th);
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
   draw_mdw((int)mdw_splash_logo_x, (int)mdw_splash_logo_y, x_scale, y_scale, mdw_splash_logo_th);
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
   draw_mdw(mdw_map_logo_x, mdw_map_logo_y, x_scale, y_scale, mdw_map_logo_th);
}





void spline_test(void)
{
   seed_mdw();
   fill_mdw();

   float x_scale = 1.0;
   float y_scale = 1.0;
   int th = 2;

   //float rv = 0.02;

   int quit = 0;
   while (!quit)
   {
      proc_controllers();

      if (key[ALLEGRO_KEY_RIGHT])
      {
//         while (key[ALLEGRO_KEY_RIGHT]) proc_controllers();;
         x_scale += .1;
      }
      if (key[ALLEGRO_KEY_LEFT])
      {
//         while (key[ALLEGRO_KEY_LEFT]) proc_controllers();;
         x_scale -= .1;

      }
      if (key[ALLEGRO_KEY_UP])
      {
//       while (key[ALLEGRO_KEY_UP]) proc_controllers();;
         y_scale += .1;
      }
      if (key[ALLEGRO_KEY_DOWN])
      {
//         while (key[ALLEGRO_KEY_DOWN]) proc_controllers();;
         y_scale -= .1;
      }

      if (key[ALLEGRO_KEY_I])
      {
//         while (key[ALLEGRO_KEY_I]) proc_controllers();;
         th++;
      }
      if (key[ALLEGRO_KEY_K])
      {
//         while (key[ALLEGRO_KEY_K]) proc_controllers();;
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
   draw_mdw(200, 200, 1.0, 1.0, 1);
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
         else if (current_spline == 2) current_spline = 4;
      }
      if (key[ALLEGRO_KEY_DOWN])
      {
         while (key[ALLEGRO_KEY_DOWN])proc_controllers();
         if (current_spline == 2) current_spline = 0;
         else if (current_spline == 4) current_spline = 2;
      }
      while ((key[ALLEGRO_KEY_ESCAPE]) || (mouse_b2))
      {
         proc_controllers();
         quit = 1;
      }
   }
}
