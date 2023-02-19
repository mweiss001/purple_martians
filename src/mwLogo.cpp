// mwLogo.cpp

#include "pm.h"
#include "mwLogo.h"
#include "mwDisplay.h"
#include "mwFont.h"
#include "mwColor.h"
#include "mwInput.h"
#include "mwEventQueue.h"
#include "z_screen.h"

mwLogo mwL;

mwLogo::mwLogo()
{
   logo_text_bitmaps_create = 1;
   initialize();
}

void mwLogo::initialize(void)
{
   seed_logo();
   fill_logo();
}


















void mwLogo::seed_logo(void)
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

void mwLogo::fill_logo(void)
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





// my version of spline with circle endpoints
void mwLogo::mspline(float *par, ALLEGRO_COLOR col, int thickness)
{
   al_draw_spline(par, col, thickness);
   float r = (float)thickness * .8;
   al_draw_filled_circle(par[0], par[1], r, col);
   al_draw_filled_circle(par[6], par[7], r, col);
}

// my version of spline with thickness and fading
void mwLogo::mfspline(float *par, int col, int thickness)
{
   ALLEGRO_COLOR c = mC.pc[col];
   float r, g, b;
   al_unmap_rgb_f(c, &r, &g, &b);

   for (int a = thickness; a>0; a--)
   {
      float f = (float)(a) / (float)(thickness); // fade in
      mspline(par, al_map_rgb_f(r*f, g*f, b*f), a);
   }
}




void mwLogo::draw_logo(float x, float y, float x_scale, float y_scale)
{
   int c1 = 10; //color 1 (red)
   int c2 = 8;  //color 2 (purple)

   // get thickness from max scale, x or y
   float max_scale = fabs(x_scale);
   if (fabs(y_scale) > fabs(x_scale)) max_scale = fabs(y_scale);
   float t = 3 + max_scale * 7;

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













void mwLogo::idw(int txt, float x, float y, float x_scale, float y_scale)
{
   //just like mtext only with predrawn text bitmaps
   // used only by mdw logo animation

   int bbx1, bby1, bbw1, bbh1;
   al_get_text_dimensions(mF.acha, "ichael", &bbx1, &bby1, &bbw1, &bbh1);

   if (logo_text_bitmaps_create)
   {
      logo_text_bitmaps_create = 0;

      al_destroy_bitmap(logo_ichael);
      logo_ichael = al_create_bitmap(bbw1,bbh1);
      al_set_target_bitmap(logo_ichael);
      al_clear_to_color(al_map_rgba(0,0,0,0));
      al_draw_text(mF.acha, mC.pc[8], 0-bbx1, 0-bby1, 0, "ichael");

      al_destroy_bitmap(logo_avid);
      logo_avid = al_create_bitmap(bbw1,bbh1);
      al_set_target_bitmap(logo_avid);
      al_clear_to_color(al_map_rgba(0,0,0,0));
      al_draw_text(mF.acha, mC.pc[90], 0-bbx1, 0-bby1, 0, "avid");

      al_destroy_bitmap(logo_eiss);
      logo_eiss = al_create_bitmap(bbw1,bbh1);
      al_set_target_bitmap(logo_eiss);
      al_clear_to_color(al_map_rgba(0,0,0,0));
      al_draw_text(mF.acha, mC.pc[8], 0-bbx1, 0-bby1, 0, "eiss");
   }

   // scale the scale...
   x_scale *=  72 / (float) al_get_font_line_height(mF.acha);
   y_scale *=  24 / (float) al_get_font_line_height(mF.acha);

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

// main menu and help screens use this
void mwLogo::mdw_an(float x, float y, float sc)
{
   float base_scale = sc;
   float x_scale = base_scale;
   float y_scale = base_scale;

   // how many seq?
   // 1st static 128  0    - 127
   // y rot 256       128  - 383
   // 2nd static 128  384  - 511
   // x rot 192       512  - 703
   // xy rot 256      704  - 959
   // xlink 64        960  - 1023
   // total 1024

   if (++mdw_an_seq > 1023) mdw_an_seq = 0;

   // 1st static
   if (mdw_an_seq < 128)
   {
      x_scale = base_scale;
      y_scale = base_scale;
   }
   // y rot
   if ((mdw_an_seq > 127) && (mdw_an_seq < 384))
   {
      float nt = ((float)(mdw_an_seq - 128) / 256) * ALLEGRO_PI*2;
      y_scale = base_scale * cos(nt);
   }
   // 2nd static
   if ((mdw_an_seq > 383) && (mdw_an_seq < 512))
   {
      x_scale = base_scale;
      y_scale = base_scale;
   }
   // x rot
   if ((mdw_an_seq > 511) && (mdw_an_seq < 704))
   {
      float nt = ((float)(mdw_an_seq - 512) / 256) * ALLEGRO_PI*2;
      x_scale = base_scale * cos(nt);
   }
   // xy rot
   if ((mdw_an_seq > 703) && (mdw_an_seq < 960))
   {
      float nt = ((float)(mdw_an_seq - 705) / 256) * ALLEGRO_PI*2;
      x_scale = base_scale * sin(nt);
      y_scale = base_scale * cos(nt);
   }
   // x link
   if ((mdw_an_seq > 959) && (mdw_an_seq < 1024))
   {
      float nt = ((float)(mdw_an_seq - 705) / 256) * ALLEGRO_PI*2;
      x_scale = base_scale * sin(nt);
   }
   draw_logo(x, y, x_scale, y_scale);
}





// splash screen uses an2
int mwLogo::mdw_an2(void)
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
      float nt = ((float)(mdw_an_seq - 0) / 256) * ALLEGRO_PI*2;
      float s = (float)mdw_an_seq / 320;
      if (s > 1) s = 1;
      x_scale = s * mdw_splash_logo_scale * sin(nt);
      y_scale = s * mdw_splash_logo_scale * cos(nt);
   }
   // fix x scale
   if ((mdw_an_seq > 255) && (mdw_an_seq < 319))
   {
      float nt = ((float)(mdw_an_seq - 256) / 256) * ALLEGRO_PI*2;
      float s = (float)mdw_an_seq / 320;
      if (s > 1) s = 1;
      x_scale = s * mdw_splash_logo_scale * sin(nt);
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
      float nt = ((float)(mdw_an_seq - 448) / 256) * ALLEGRO_PI*2;
      x_scale = mdw_splash_logo_scale * cos(nt);
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
      float nt = ((float)(mdw_an_seq - 640) / 256) * ALLEGRO_PI*2;
      x_scale = mdw_splash_logo_scale * sin(nt);
      y_scale = mdw_splash_logo_scale * cos(nt);
   }
   // back to original
   if ((mdw_an_seq > 895) && (mdw_an_seq < 960))
   {
      float nt = ((float)(mdw_an_seq - 896) / 256) * ALLEGRO_PI*2;
      x_scale = mdw_splash_logo_scale * sin(nt);
      y_scale = mdw_splash_logo_scale;
   }
   draw_logo(mdw_splash_logo_x, mdw_splash_logo_y, x_scale, y_scale);
   return 0;
}



void mwLogo::splash_screen(void)
{
   mdw_an_seq = 0;
   set_map_var();
   int quit = 0;

   mwEQ.proc_event_queue();

   draw_large_text_overlay(1, 8);

   al_flip_display();
   al_clear_to_color(al_map_rgb(0, 0, 0));

   if (!quit)
      for (int i=0; i<220; i++)
      {
         while (!mwEQ.menu_update) mwEQ.proc_event_queue();
         mwEQ.menu_update = 0;

         if (mI.key_pressed_ASCII)
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
      mw_text(mF.sauc, 12, .80, "created by:");
      al_flip_display();
   }

   if (!quit)
      for (int i=0; i<120; i++)
      {
         while (!mwEQ.menu_update) mwEQ.proc_event_queue();
         mwEQ.menu_update = 0;
         if (mI.key_pressed_ASCII)
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
      while (!mwEQ.menu_update) mwEQ.proc_event_queue();
      mwEQ.menu_update = 0;
      if (mI.key_pressed_ASCII) quit = 1;
   }

   al_clear_to_color(al_map_rgb(0,0,0));
   al_flip_display();

   mdw_an_seq = 0;
}


// used by "created by:"
void mwLogo::mw_text(ALLEGRO_FONT *tf, int col, float x_pc, const char * txt)
{
   int bbx1, bby1, bbw1, bbh1;
   al_get_text_dimensions(tf, txt, &bbx1, &bby1, &bbw1, &bbh1);
   ALLEGRO_BITMAP *t1 = al_create_bitmap(bbw1, bbh1);
   al_set_target_bitmap(t1);
   al_clear_to_color(al_map_rgba(0,0,0,0));
   al_draw_text(tf, mC.pc[col], 0-bbx1, 0-bby1, 0, txt);
   float xs = ( (float)mwD.SCREEN_W * x_pc) / (float) bbw1 ; // x scale
   al_set_target_backbuffer(display);
   al_draw_scaled_rotated_bitmap(t1, bbw1/2, bbh1/2, mwD.SCREEN_W/2, mwD.SCREEN_H/2, xs, xs, 0, 0);
   al_destroy_bitmap(t1);
}















void mwLogo::spline_test(void)
{
   seed_logo();
   fill_logo();

   float x_scale = 1.0;
   float y_scale = 1.0;

   float x_scale_inc = 0.01;
   float y_scale_inc = 0.01;


   int quit = 0;
   while (!quit)
   {

      if (mI.key[ALLEGRO_KEY_RIGHT][0])
      {
         x_scale += x_scale_inc;
      }
      if (mI.key[ALLEGRO_KEY_LEFT][0])
      {
         x_scale -= x_scale_inc;

      }
      if (mI.key[ALLEGRO_KEY_UP][0])
      {
         y_scale += y_scale_inc;
      }
      if (mI.key[ALLEGRO_KEY_DOWN][0])
      {
         y_scale -= y_scale_inc;
      }

      draw_logo(mwD.SCREEN_W/2, mwD.SCREEN_H/2, x_scale, y_scale);
      al_flip_display();
      al_clear_to_color(al_map_rgb(0,0,0));

      while (!mwEQ.menu_update) mwEQ.proc_event_queue();
      mwEQ.menu_update = 0;

      if (mI.key[ALLEGRO_KEY_ESCAPE][0])
      {
         while (mI.key[ALLEGRO_KEY_ESCAPE][0]) mwEQ.proc_event_queue();
         quit = 1;
      }


   }
}


void mwLogo::redraw_spline(int s)
{
   fill_logo();
   draw_logo(200, 200, 1.0, 1.0);
   al_draw_textf(mF.pr8, mC.pc[15], 100, 402, 0, "current spline:%d", s);

   for (int i=0; i<8; i+=2)
   {
      al_draw_circle(points[s][i]+200, points[s][i+1]+200, 4, mC.pc[11], 1);
      al_draw_textf(mF.pr8, mC.pc[15], 100, 410+i*4, 0, "x:%-4.0f y:%-4.0f", points[s][i], points[s][i+1] );
   }
}

void mwLogo::spline_adjust(void)
{
   al_show_mouse_cursor(display);

   int current_spline = 0;
//   seed_mdw();
//   fill_mdw();
   int quit = 0;
   while (!quit)
   {
      al_clear_to_color(al_map_rgb(0,0,0));
      redraw_spline(current_spline);
      al_flip_display();
      mwEQ.proc_event_queue();

      int mx = mI.mouse_x-200;
      int my = mI.mouse_y-200;
      for (int i=0; i<8; i+=2)
      {
         int px = points[current_spline][i];
         int py = points[current_spline][i+1];
         if ((mx > px-4) && (mx < px+4) && (my > py-4) && (my < py+4))
         {
            al_draw_circle(points[current_spline][i]+200, points[current_spline][i+1]+200, 6, mC.pc[10], 2);
            while (mI.mouse_b[1][0])
            {
               al_clear_to_color(al_map_rgb(0,0,0));
               redraw_spline(current_spline);
               al_draw_circle(points[current_spline][i]+200, points[current_spline][i+1]+200, 6, mC.pc[10], 3);
               al_flip_display();
               mwEQ.proc_event_queue();

               points[current_spline][i] = mI.mouse_x-200;
               points[current_spline][i+1] = mI.mouse_y-200;
            }
         }
      }

      if (mI.key[ALLEGRO_KEY_UP][3])
      {
         if (current_spline == 0) current_spline = 2;
         else if (current_spline == 2) current_spline = 8;
      }
      if (mI.key[ALLEGRO_KEY_DOWN][3])
      {
         if (current_spline == 2) current_spline = 0;
         else if (current_spline == 8) current_spline = 2;
      }
      while ((mI.key[ALLEGRO_KEY_ESCAPE][0]) || (mI.mouse_b[2][0]))
      {
         mwEQ.proc_event_queue();
         quit = 1;
      }
   }
}


