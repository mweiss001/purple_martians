
#include "pm.h"
#include "mwTileEditor.h"
#include "mwBitmapTools.h"
#include "mwColor.h"
#include "mwFont.h"
#include "mwWidget.h"
#include "mwInput.h"
#include "mwDisplay.h"
#include "mwEventQueue.h"
#include "mwLoop.h"


void mwTileEditor::scroll_selection(ALLEGRO_BITMAP *t, int x, int y)
{
   fix_sel();
   ALLEGRO_BITMAP *tmp = nullptr;

   if (x>0)
   {
      // get right most column
      tmp = al_create_bitmap(1, sh);
      al_set_target_bitmap(tmp);
      al_draw_bitmap_region(t, sx2,sy1, 1,sh, 0,0, 0);

      // draw to self, shifted right by 1
      al_set_target_bitmap(t);
      al_draw_bitmap_region(t, sx1, sy1, sw-1,sh, sx1+1,sy1, 0);

      // draw saved column
      al_draw_bitmap(tmp, sx1, sy1, 0);
   }
   if (x<0)
   {
      // get left most column
      tmp = al_create_bitmap(1, sh);
      al_set_target_bitmap(tmp);
      al_draw_bitmap_region(t, sx1,sy1, 1,sh, 0,0, 0);

      // draw to self, shifted left by 1
      al_set_target_bitmap(t);
      al_draw_bitmap_region(t, sx1+1,sy1, sw-1,sh, sx1,sy1, 0);

      // draw saved column
      al_draw_bitmap(tmp, sx2,sy1, 0);
   }
   if (y>0)
   {
      // get bottom row
      tmp = al_create_bitmap(sw, 1);
      al_set_target_bitmap(tmp);
      al_draw_bitmap_region(t, sx1,sy2, sw,1, 0,0, 0);

      // draw to self, shifted down by 1
      al_set_target_bitmap(t);
      al_draw_bitmap_region(t, sx1,sy1, sw,sh-1, sx1,sy1+1, 0);

      // draw saved row
      al_draw_bitmap(tmp, sx1,sy1, 0);
   }
   if (y<0)
   {
      // get top row
      tmp = al_create_bitmap(sw, 1);
      al_set_target_bitmap(tmp);
      al_draw_bitmap_region(t, sx1,sy1, sw,1, 0,0, 0);

      // draw to self, shifted up by 1
      al_set_target_bitmap(t);
      al_draw_bitmap_region(t, sx1,sy1+1, sw,sh-1, sx1,sy1, 0);

      // draw saved row
      al_draw_bitmap(tmp, sx1,sy2, 0);
   }
   al_destroy_bitmap(tmp);
}

void mwTileEditor::flip_selection(ALLEGRO_BITMAP *t, int x, int y)
{
   fix_sel();

   // get selection
   ALLEGRO_BITMAP *tmp = al_create_bitmap(sw, sh);
   al_set_target_bitmap(tmp);
   al_draw_bitmap_region(t, sx1, sy1, sw, sh, 0,0, 0);

   al_set_target_bitmap(t);
   if (x) al_draw_bitmap(tmp, sx1,sy1, ALLEGRO_FLIP_HORIZONTAL);
   if (y) al_draw_bitmap(tmp, sx1,sy1, ALLEGRO_FLIP_VERTICAL);
   al_destroy_bitmap(tmp);
}

void mwTileEditor::rotate_selection(ALLEGRO_BITMAP *t, float a)
{
   fix_sel();
   // get selection
   ALLEGRO_BITMAP *tmp = al_create_bitmap(sw, sh);
   al_set_target_bitmap(tmp);
   al_draw_bitmap_region(t, sx1, sy1, sw, sh, 0,0, 0);

   float cx = (float)sw/2;
   float cy = (float)sh/2;

   al_set_target_bitmap(t);
   if (a>0) al_draw_rotated_bitmap(tmp, cx, cy, sx1+cx, sy1+cy, ALLEGRO_PI * a, 0);
   if (a<0) al_draw_rotated_bitmap(tmp, cx, cy, sx1+cx, sy1+cy, ALLEGRO_PI * a, 0);
   al_destroy_bitmap(tmp);
}

bool colorClickSlider(int type, float x1, float &y1, float x2, float bts, float &val, bool &hover, float &hover_val, ALLEGRO_COLOR c)
{
   bool changed = false;

   // for the slider only
   float sx1 = x1+bts;
   float sx2 = x2-bts;

   float y2 = y1 + bts - 1;

   float min = 0;
   float max = 1;

   if (type == 4) max = 360;

   float sw = sx2-sx1;
   float sxc = sx1+sw/2;
   float tyc = y1+(y2-y1-8)/2;


   // color incs
   float ri = 0;
   float gi = 0;
   float bi = 0;
   float hi = 0;
   float si = 0;
   float li = 0;
   if (type == 1) ri = 1 / sw;
   if (type == 2) gi = 1 / sw;
   if (type == 3) bi = 1 / sw;
   if (type == 4) hi = 360/sw;
   if (type == 5) si = 1 / sw;
   if (type == 6) li = 1 / sw;


   // draw background
   if (type > 0 && type < 4)
   {
      for (float i=0; i<sw; i++)
         al_draw_line(sx1+i, y1, sx1+i, y2, al_map_rgb_f(i*ri, i*gi, i*bi), 1);
   }
   if (type == 4)
   {
      for (float i=0; i<sw; i++)
         al_draw_line(sx1+i, y1, sx1+i, y2, al_color_hsl(i*hi, 1.0, 0.5), 1);
   }
   if (type == 5)
   {
      float h, s, l;
      mColor.map_rgb_to_hsl(c, h, s, l);

      for (float i=0; i<sw; i++)
         al_draw_line(sx1+i, y1, sx1+i, y2, al_color_hsl(h, i*si, l), 1);
   }
   if (type == 6)
   {
      float h, s, l;
      mColor.map_rgb_to_hsl(c, h, s, l);
      for (float i=0; i<sw; i++)
         al_draw_line(sx1+i, y1, sx1+i, y2, al_color_hsl(h, s, i*li), 1);
   }

   // frame
   al_draw_rectangle(x1, y1, x2, y2, mColor.White, 1);

   // get var position and draw
   float vx = map_range(val, min, max,  sx1, sx2);
   al_draw_line(vx, y1, vx, y2, mColor.White, 2);

   // draw + and minus lines and text
   al_draw_rectangle(sx1, y1, sx1, y2, mColor.White, 1);
   al_draw_rectangle(sx2, y1, sx2, y2, mColor.White, 1);

   al_draw_text(mFont.pr8, mColor.White, x1 + (sx1-x1)/2, tyc, ALLEGRO_ALIGN_CENTER, "-");
   al_draw_text(mFont.pr8, mColor.White, x2 + (sx2-x2)/2, tyc, ALLEGRO_ALIGN_CENTER, "+");


   // mouse on - button
   if (mInput.mouse_x > x1 && mInput.mouse_x < sx1 && mInput.mouse_y > y1 && mInput.mouse_y < y2)
   {
      if (mInput.mouse_b[1][0])
      {
         while (mInput.mouse_b[1][0]) mEventQueue.proc(1);
         changed = true;
         if (type == 4) val -= 0.5;
         else val -= 0.002;
         if (val < 0) val = 0;
      }
   }


   // mouse on + button
   if (mInput.mouse_x > sx2 && mInput.mouse_x < x2 && mInput.mouse_y > y1 && mInput.mouse_y < y2)
   {
      if (mInput.mouse_b[1][0])
      {
         while (mInput.mouse_b[1][0]) mEventQueue.proc(1);
         changed = true;
         if (type == 4) val += 0.5;
         else val += 0.002;
         if (val > 360) val = 360;
         if (type != 4) if (val > 1.0) val = 1.0;
      }
   }






   if (mInput.mouse_x > sx1 && mInput.mouse_x < sx2 && mInput.mouse_y > y1 && mInput.mouse_y < y2)
   {
      hover = true;

      // get hover value from mouse position and draw
      float x = mInput.mouse_x;
      hover_val = map_range(x, sx1, sx2, min, max);
      al_draw_line(x, y1, x, y2, mColor.White, 2);

      if (mInput.mouse_b[1][0])
      {
         changed = true;
         val = hover_val;
      }
   }

   char name[256];
   sprintf(name, "%s", "");
   if (type == 1) sprintf(name, "%s", "red");
   if (type == 2) sprintf(name, "%s", "green");
   if (type == 3) sprintf(name, "%s", "blue");
   if (type == 4) sprintf(name, "%s", "hue");
   if (type == 5) sprintf(name, "%s", "sat");
   if (type == 6) sprintf(name, "%s", "light");

   char msg[80];
   if (type > 0 && type < 4)
   {
      int ival = (int)(val*255);
      int hval = (int)(hover_val*255);
      sprintf(msg, "%s:%d", name, ival);
      if (hover) sprintf(msg, "%s:%d [%d]", name, ival, hval);
      al_draw_text(mFont.pr8, mColor.White, sxc, tyc, ALLEGRO_ALIGN_CENTER, msg);
   }

   if (type == 4)
   {
      int ival = (int)val;
      int hval = (int)hover_val;
      sprintf(msg, "%s:%d", name, ival);
      if (hover) sprintf(msg, "%s:%d [%d]", name, ival, hval);
      al_draw_text(mFont.pr8, mColor.Black, sxc, tyc, ALLEGRO_ALIGN_CENTER, msg);
   }

   if (type > 4 && type < 7)
   {
      sprintf(msg, "%s:%3.2f", name, val);
      if (hover) sprintf(msg, "%s:%3.2f [%3.2f]", name, val, hover_val);
      al_draw_text(mFont.pr8, mColor.White, sxc, tyc, ALLEGRO_ALIGN_CENTER, msg);
   }

  y1 += bts;

  return changed;
}


void mwTileEditor::color_controls(const char* text, ALLEGRO_COLOR &c, int x, int y, int width)
{
   ALLEGRO_COLOR show_color = c;

   float fpw = 44; // first panel width
   //fpw += mLoop.pct_x;

   float xa = x+o+fpw+2;
   float xb = x+o+width-2;
   int bts = 16;
   //bts += mLoop.pct_y;

   int height = 4 + bts*6;
   height += mLoop.pct_y;

   float r, g, b;
   al_unmap_rgb_f(c, &r, &g, &b);

   float h, s, l;
   mColor.map_rgb_to_hsl(c, h, s, l);

   bool r_hover = false;
   bool g_hover = false;
   bool b_hover = false;
   bool h_hover = false;
   bool s_hover = false;
   bool l_hover = false;

   float r_hover_val = 0;
   float g_hover_val = 0;
   float b_hover_val = 0;
   float h_hover_val = 0;
   float s_hover_val = 0;
   float l_hover_val = 0;

   float ya = y+2;

   if (colorClickSlider(1, xa, ya, xb, bts, r, r_hover, r_hover_val, c)) c = al_map_rgb_f(r,g,b);
   if (colorClickSlider(2, xa, ya, xb, bts, g, g_hover, g_hover_val, c)) c = al_map_rgb_f(r,g,b);
   if (colorClickSlider(3, xa, ya, xb, bts, b, b_hover, b_hover_val, c)) c = al_map_rgb_f(r,g,b);
   if (r_hover) show_color = al_map_rgb_f(r_hover_val, g,           b );
   if (g_hover) show_color = al_map_rgb_f(r,           g_hover_val, b );
   if (b_hover) show_color = al_map_rgb_f(r,           g,           b_hover_val );

   ya +=2;
   al_draw_line(x+o+fpw, ya+o-2, x+o+width, ya+o+-2, mColor.White, 1); // line between rgb and hsl
   
   if (colorClickSlider(4, xa, ya, xb, bts, h, h_hover, h_hover_val, c)) c = al_color_hsl(h, s, l);
   if (colorClickSlider(5, xa, ya, xb, bts, s, s_hover, s_hover_val, c)) c = al_color_hsl(h, s, l);
   if (colorClickSlider(6, xa, ya, xb, bts, l, l_hover, l_hover_val, c)) c = al_color_hsl(h, s, l);
   if (h_hover) show_color = al_color_hsl(h_hover_val, s,           l );
   if (s_hover) show_color = al_color_hsl(h,           s_hover_val, l );
   if (l_hover) show_color = al_color_hsl(h,           s,           l_hover_val );

   // show color rect
   al_draw_filled_rectangle(x, y+20, x+o+fpw, y+o+height, show_color);

   // show text
   al_draw_text(mFont.pr8, mColor.White, x+o+fpw/2, y+o+2,  ALLEGRO_ALIGN_CENTER, text);
   al_draw_text(mFont.pr8, mColor.White, x+o+fpw/2, y+o+10, ALLEGRO_ALIGN_CENTER, "color");

   // frame all
   al_draw_rectangle(x+o, y+o, x+o+width, y+o+height, mColor.White, 1);
   al_draw_line(x+o, y+o+20, x+o+fpw, y+o+20, mColor.White, 1);
   al_draw_line(x+o+fpw, y+o, x+o+fpw, y+o+height, mColor.White, 1);

}



void mwTileEditor::fix_sel()
{
   if (sx1 > sx2)
   {
      int t = sx1;
      sx1 = sx2;
      sx2 = t;
   }
   if (sy1 > sy2)
   {
      int t = sy1;
      sy1 = sy2;
      sy2 = t;
   }

   if (sx1 < 0) sx1 = 0;
   if (sy1 < 0) sy1 = 0;
   if (sx2 < 0) sx2 = 0;
   if (sy2 < 0) sy2 = 0;

   if (sx1 > 19) sx1 = 19;
   if (sy1 > 19) sy1 = 19;
   if (sx2 > 19) sx2 = 19;
   if (sy2 > 19) sy2 = 19;

   sw = (sx2-sx1) + 1;
   sh = (sy2-sy1) + 1;

}


void mwTileEditor::draw_frame_and_gridlines()
{
   // frame
   al_draw_rectangle(dx1+o, dy1+o, dx2+o, dy2+o, highlight_color, 0);

   // gridlines
   if (gridlines)
   {
      for (int i=0; i<20; i++)
      {
         al_draw_rectangle(dx1+o+i*d_scale, dy1+o, dx1+o+i*d_scale, dy2+o, frame_color, 0);
         al_draw_rectangle(dx1+o, dy1+o+i*d_scale, dx2+o, dy1+o+i*d_scale, frame_color, 0);
      }
   }

   // hightlight center gridlines
   int i = 10;
   al_draw_rectangle(dx1+o+i*d_scale, dy1+o, dx1+o+i*d_scale, dy2+o, highlight_color, 1);
   al_draw_rectangle(dx1+o, dy1+o+i*d_scale, dx2+o, dy1+o+i*d_scale, highlight_color, 1);


}



void mwTileEditor::edit_tile_redraw(ALLEGRO_BITMAP *t, bool show_selection)
{
   al_flip_display();

   mEventQueue.proc(1);

   al_set_target_backbuffer(mDisplay.display);
   al_clear_to_color(al_map_rgb(0,0,0));

   // draw title
   al_draw_text(mFont.pr8, mColor.pc[15], dx1, dy1-10, 0, title);

   // draw enlarged tile
   al_draw_scaled_bitmap(t, 0, 0, 20, 20, dx1+o, dy1+o, dw, dh, 0);

   draw_frame_and_gridlines();


   // show selection
   if (show_selection) al_draw_rectangle(dx1+o+sx1*d_scale, dy1+o+sy1*d_scale, dx1+o+(sx2+1)*d_scale, dy1+o+(sy2+1)*d_scale, mColor.Yellow, 2);

   // is mouse on grid?
   if (mInput.mouse_x > dx1 && mInput.mouse_x < dx2 && mInput.mouse_y > dy1 && mInput.mouse_y < dy2)
   {
      mouse_on_grid = true;

      // what pixel is mouse pointing at?
      mx = (mInput.mouse_x - dx1) / d_scale;
      my = (mInput.mouse_y - dy1) / d_scale;

      // show text of current mouse pixel position x, y
      al_draw_rectangle(dx2+o-76, dy2+o, dx2+o, dy2+o+12, highlight_color, 0);
      al_draw_textf(mFont.pr8, mColor.pc[15], dx2+o-74, dy2+o+2, 0, "x:%-2d y:%-2d", mx, my);


   }
   else mouse_on_grid = false;

   // gridlines check box
   int xa = dx2+o-190;
   int ya = dy1+o-15;
   mWidget.togglec(xa, ya, xa+20, 16,  0,0,0,0,  0, 0, 0, 0,  1,0,1,0, gridlines, "Gridlines", 15, 15);



}




void mwTileEditor::legacy_color_select(int x1, int y1, int size)
{
   int x2 = x1 + size*16;
   int y2 = y1 + size*16;

   for (int x=0; x<16; x++)
      for (int y=0; y<16; y++)
         al_draw_filled_rectangle(x1+x*size, y1+y*size, x1+(x+1)*size, y1+(y+1)*size, mColor.pc[y*16+x]);

   al_draw_rectangle(x1+o, y1+o, x2+o, y2+o, highlight_color, 1);

   // is mouse on grid?
   if (mInput.mouse_x > x1 && mInput.mouse_x < x2 && mInput.mouse_y > y1 && mInput.mouse_y < y2)
   {
      // what color is mouse pointing at?
      int x = (mInput.mouse_x - x1) / size;
      int y = (mInput.mouse_y - y1) / size;
      int pc = y*16 + x;
      point_color = mColor.pc[pc];

      // show text of current mouse pixel position x, y
      al_draw_rectangle(x2+o-76, y2+o, x2+o, y2+o+12, highlight_color, 0);
      al_draw_textf(mFont.pr8, mColor.pc[15], x2+o-68, y2+o+2, 0, "col:%d", pc);

      color_controls("point", point_color, dx1+200, dy2+14, 170);

      if (mInput.mouse_b[1][0])
      {
         while (mInput.mouse_b[2][0]) mEventQueue.proc(1);
         draw_color = point_color;
      }
   }
}

void mwTileEditor::tile_palette_select(int x1, int y1, int size)
{
   al_set_target_bitmap(tile_palette_bitmap);
   al_clear_to_color(al_map_rgb(0,0,0));

   tile_palette[0][0] = 27;
   tile_palette[0][1] = 28;
   tile_palette[0][2] = 29;
   
   tile_palette[1][0] = 59;
   tile_palette[1][1] = 60;
   tile_palette[1][2] = 61;

   tile_palette[2][0] = 91;
   tile_palette[2][1] = 92;
   tile_palette[2][2] = 93;

   for (int x=0; x<3; x++)
      for (int y=0; y<3; y++)
      {
         int xi = (tile_palette[y][x] % 32) * 22 + 1;
         int yi = (tile_palette[y][x] / 32) * 22 + 1;
         al_draw_bitmap_region(tilemap_bitmap, xi, yi, 20, 20, x*20, y*20, 0);
      }

   al_set_target_backbuffer(mDisplay.display);
   al_draw_scaled_bitmap(tile_palette_bitmap, 0,0,60,60, x1, y1, size*60, size*60, 0);

   int x2 = x1 + size*60;
   int y2 = y1 + size*60;

   al_draw_rectangle(x1+o, y1+o, x2+o, y2+o, highlight_color, 1);

   // is mouse on grid?
   if (mInput.mouse_x > x1 && mInput.mouse_x < x2 && mInput.mouse_y > y1 && mInput.mouse_y < y2)
   {
      // what tile is mouse pointing at?
      int x = (mInput.mouse_x - x1) / (size*20);
      int y = (mInput.mouse_y - y1) / (size*20);
      int tile = tile_palette[y][x];

      // show text of current mouse pixel position x, y
      al_draw_rectangle(x2+o-76, y2+o, x2+o, y2+o+12, highlight_color, 0);
      al_draw_textf(mFont.pr8, mColor.pc[15], x2+o-68, y2+o+2, 0, "tile:%d", tile);

      if (mInput.mouse_b[1][0])
      {
         while (mInput.mouse_b[2][0]) mEventQueue.proc(1);
         set_edit_tile(tile);
      }
   }
}


void mwTileEditor::selection_controls(int x, int y)
{
   int x1 = x;
   int xw = 168;
   //xw += mLoop.pct_x;

   int x2 = x1 + xw;
   int xc = x1 + xw/2;

   int bts = 12;
   //bts += mLoop.pct_y;


   int line_spacing = 6;
   //line_spacing += mLoop.pct_y;

   int y1 = y;
   int h = 130;
   //h += mLoop.pct_y;
   int y2 = y1+h;

   ALLEGRO_COLOR sfc = mColor.Yellow;

   // frame
   al_draw_rectangle(x1+o, y1+o, x2+o, y2+o, sfc, 1);

   // running y
   int ya = y + line_spacing/2;

   int xa = x1+2;
   int xb = x2-2;

   mWidget.mButton(5, xc, -1, 1, ya, bts,     0, 0, 0, 3, 0, 15, 15, 15, -1, "Selection");
   ya+= bts-1;
   al_draw_line(x1+o, ya+o+line_spacing/2, x2+o, ya+o+line_spacing/2, sfc, 1);
   ya+= line_spacing;

   char msg[256];
   sprintf(msg, "x:%d y:%d w:%d h:%d", sx1, sy1, sw, sh);
   mWidget.mButton(5, xc, -1, 1, ya, bts,     0, 0, 0, 3, 0, 15, 15, 15, -1, msg);
   ya+= bts-1;
   al_draw_line(x1+o, ya+o+line_spacing/2, x2+o, ya+o+line_spacing/2, sfc, 1);
   ya+= line_spacing;

   if (mWidget.mButton(5, xc, -1, 1, ya, bts,     0, 0, 3, 3, 0, 15, 15, 10, -1, "Up"))    scroll_selection(edit_tile_bitmap,  0, -1);
   ya+= bts+2;
   if (mWidget.mButton(3, xa, -1, 1, ya, bts,     0, 0, 3, 3, 0, 15, 15, 10, -1, "Left"))  scroll_selection(edit_tile_bitmap, -1, 0);
       mWidget.mButton(5, xc, -1, 1, ya, bts,     0, 0, 3, 3, 0, 14, 15, 15, -1, "Scroll");
   if (mWidget.mButton(4, -1, xb, 1, ya, bts,     0, 0, 3, 3, 0, 15, 15, 10, -1, "Right")) scroll_selection(edit_tile_bitmap,  1, 0);
   ya+= bts+2;
   if (mWidget.mButton(5, xc, -1, 1, ya, bts,     0, 0, 3, 3, 0, 15, 15, 10, -1, "Down"))  scroll_selection(edit_tile_bitmap,  0, 1);
   ya+= bts-1;

   al_draw_line(x1+o, ya+o+line_spacing/2, x2+o, ya+o+line_spacing/2, sfc, 1);
   ya+= line_spacing;

   if (mWidget.mButton(3, xa, -1, 1, ya, bts,     0, 0, 3, 3, 0, 15, 15, 10, -1, "X"))  flip_selection(edit_tile_bitmap,  1,  0);
       mWidget.mButton(5, xc, -1, 1, ya, bts,     0, 0, 3, 3, 0, 14, 15, 15, -1, "Mirror");
   if (mWidget.mButton(4, -1, xb, 1, ya, bts,     0, 0, 3, 3, 0, 15, 15, 10, -1, "Y"))  flip_selection(edit_tile_bitmap,  0,  1);
   ya+= bts-1;

   al_draw_line(x1+o, ya+o+line_spacing/2, x2+o, ya+o+line_spacing/2, sfc, 1);
   ya+= line_spacing;

   if (mWidget.mButton(3, xa, -1, 1, ya, bts,     0, 0, 3, 3, 0, 15, 15, 10, -1, "CCW"))  rotate_selection(edit_tile_bitmap, 0.5);
   if (mWidget.mButton(3, xa+20, -1, 1, ya, bts,     0, 0, 3, 3, 0, 15, 15, 10, -1, "CCW"))  rotate_selection(edit_tile_bitmap, .25);
       mWidget.mButton(5, xc, -1, 1, ya, bts,     0, 0, 3, 3, 0, 14, 15, 15, -1, "Rotate");
   if (mWidget.mButton(4, -1, xb, 1, ya, bts,     0, 0, 3, 3, 0, 15, 15, 10, -1, "CW"))  rotate_selection(edit_tile_bitmap, 1.5);
   ya+= bts-1;

   al_draw_line(x1+o, ya+o+line_spacing/2, x2+o, ya+o+line_spacing/2, sfc, 1);
   ya+= line_spacing;


   mWidget.mCheckBox(2, 60, xb,  1,ya,bts, -1, paste_mode_alpha, "alpha", 15, 15);
   if (mWidget.mButton(3, xa+2, -1, 1, ya, bts,     0, 0, 3, 3, 0, 15, 15, 10, -1, "Paste"))
   {
      // get bitmap of selected area
      fix_sel();
      ALLEGRO_BITMAP *tmp = al_create_bitmap(sw, sh);
      al_set_target_bitmap(tmp);
      al_draw_bitmap_region(edit_tile_bitmap, sx1, sy1, sw, sh, 0,0, 0);

      if (paste_mode_alpha) al_convert_mask_to_alpha(tmp, al_map_rgb(0, 0, 0));

      int paste_quit = 0;
      while (!paste_quit)
      {
         sprintf(title, "Paste Selection");
         edit_tile_redraw(edit_tile_bitmap, false);


         // this is so that I can have the mouse off the grid and still be able to paste
         // this is only if x<0 or y<0
         // magnified a lot because the edge of the screen is close
         if ((!mouse_on_grid) && ((mInput.mouse_x < dx1) || (mInput.mouse_y < dy1)))
         {
            mouse_on_grid = true;
            float tmx = mInput.mouse_x - dx1;
            float tmy = mInput.mouse_y - dy1;
            if (tmx < 0) mx = tmx /= 1; // magnifier
            if (tmy < 0) my = tmy /= 1;
            if (mx < -20) mx = -20;
            if (my < -20) my = -20;
            // show text of current mouse pixel position x, y
            al_draw_rectangle(dx2+o-76, dy2+o, dx2+o, dy2+o+12, highlight_color, 0);
            al_draw_textf(mFont.pr8, mColor.pc[15], dx2+o-74, dy2+o+2, 0, "x:%-2d y:%-2d", mx, my);
         }


         if (mouse_on_grid)
         {
            // show paste bitmap
            al_draw_scaled_bitmap(tmp, 0,0,sw,sh, dx1+o+mx*d_scale, dy1+o+my*d_scale, sw*d_scale, sh*d_scale, 0);

            draw_frame_and_gridlines();

            if (mInput.mouse_b[1][0])
            {
               while (mInput.mouse_b[1][0]) mEventQueue.proc(1);
               al_set_target_bitmap(edit_tile_bitmap);
               al_draw_bitmap(tmp, mx, my, 0);
               //paste_quit = 1; // keep pasting till cancelled
            }
         }











         tile_palette_select(dx2+20, dy2+20, 3);


         if (mInput.mouse_b[2][0])
         {
            while (mInput.mouse_b[2][0]) mEventQueue.proc(1);
            paste_quit = 1;
         }
         if (mInput.key[ALLEGRO_KEY_ESCAPE][0])
         {
            while (mInput.key[ALLEGRO_KEY_ESCAPE][0]) mEventQueue.proc(1);
            paste_quit = 1;
         }
      }
      al_destroy_bitmap(tmp);
   }
}




int mwTileEditor::draw_mode_controls(int x, int y)
{
   int x1 = x;
   int xw = 168;
   //xw += mLoop.pct_x;

   int x2 = x1 + xw;
   int xc = x1 + xw/2;

   int bts = 12;
   //bts += mLoop.pct_y;


   int line_spacing = 6;
   //line_spacing += mLoop.pct_y;

   int y1 = y;
   int h = (bts+2) * 9 + 37;

   if (draw_mode > 1 && draw_mode < 6) h+=19;
   //h += mLoop.pct_y;
   int y2 = y1 + h;

   ALLEGRO_COLOR sfc = mColor.Bluey;

   // frame
   al_draw_rectangle(x1+o, y1+o, x2+o, y2+o, sfc, 1);


   // running y
   int ya = y + line_spacing/2;
   int xa = x1+2;
   int xb = x2-2;


   mWidget.mButton(5, xc, -1, 1, ya, bts,     0, 0, 0, 3, 0, 15, 15, 15, -1, "Draw Mode");
   ya+= bts-1;
   al_draw_line(x1+o, ya+o+line_spacing/2, x2+o, ya+o+line_spacing/2, sfc, 1);
   ya+= line_spacing;

   char dmt[256];
   sprintf(dmt, "%s", "");
   if (draw_mode == 1) sprintf(dmt, "%s", "Point");
   if (draw_mode == 2) sprintf(dmt, "%s", "Line");
   if (draw_mode == 3) sprintf(dmt, "%s", "Rectangle");
   if (draw_mode == 4) sprintf(dmt, "%s", "Circle");
   if (draw_mode == 5) sprintf(dmt, "%s", "Ellipse");
   if (draw_mode == 6) sprintf(dmt, "%s", "Filled Rectangle");
   if (draw_mode == 7) sprintf(dmt, "%s", "Filled Circle");
   if (draw_mode == 8) sprintf(dmt, "%s", "Filled Ellipse");
   if (draw_mode == 9) sprintf(dmt, "%s", "Replace Color");


   mWidget.mButton(5, xc, -1, 1, ya, bts,     0, 0, 0, 3, 0, 15, 15, 15, -1, dmt);
   ya+= bts-1;
   al_draw_line(x1+o, ya+o+line_spacing/2, x2+o, ya+o+line_spacing/2, sfc, 1);
   ya+= line_spacing;

   if (draw_mode > 1 && draw_mode < 6)
   {
      mWidget.sliderf(xa+1, ya, xb-1,  16,  0,0,0,0,  0,12,15 ,15,  0,0,1,0, th, 8, 0,  0.1, "Line Size:");
      ya+= -3;
      al_draw_line(x1+o, ya+o+line_spacing/2, x2+o, ya+o+line_spacing/2, sfc, 1);
      ya+= line_spacing;
   }

   if (mWidget.mButton(5, xc, -1, 1, ya, bts,     0, 0, 3, 3, 0, 15, 15, 10, -1, "Point")) draw_mode = 1;
   ya+=bts+2;
   if (mWidget.mButton(5, xc, -1, 1, ya, bts,     0, 0, 3, 3, 0, 15, 15, 10, -1, "Line")) draw_mode = 2;
   ya+=bts+2;
   if (mWidget.mButton(5, xc, -1, 1, ya, bts,     0, 0, 3, 3, 0, 15, 15, 10, -1, "Rectangle")) draw_mode = 3;
   ya+=bts+2;
   if (mWidget.mButton(5, xc, -1, 1, ya, bts,     0, 0, 3, 3, 0, 15, 15, 10, -1, "Circle")) draw_mode = 4;
   ya+=bts+2;
   if (mWidget.mButton(5, xc, -1, 1, ya, bts,     0, 0, 3, 3, 0, 15, 15, 10, -1, "Ellipse")) draw_mode = 5;
   ya+=bts+2;
   if (mWidget.mButton(5, xc, -1, 1, ya, bts,     0, 0, 3, 3, 0, 15, 15, 10, -1, "Filled Rectangle")) draw_mode = 6;
   ya+=bts+2;
   if (mWidget.mButton(5, xc, -1, 1, ya, bts,     0, 0, 3, 3, 0, 15, 15, 10, -1, "Filled Circle")) draw_mode = 7;
   ya+=bts+2;
   if (mWidget.mButton(5, xc, -1, 1, ya, bts,     0, 0, 3, 3, 0, 15, 15, 10, -1, "Filled Ellipse")) draw_mode = 8;
   ya+=bts+2;

   if (mWidget.mButton(5, xc, -1, 1, ya, bts,     0, 0, 3, 3, 0, 15, 15, 10, -1, "Replace Color")) draw_mode = 9;
   ya+=bts+2;


   return h;

}


void mwTileEditor::set_edit_tile(int tile)
{
   edit_tile_index = tile;
   mBitmapTools.get_tile_from_tilemap(tilemap_bitmap, edit_tile_bitmap, edit_tile_index);
}


void mwTileEditor::process_draw()
{

   if (draw_mode == 1) // point or single pixel mode
   {
      while (mInput.mouse_b[1][0]) mEventQueue.proc(1);
      al_set_target_bitmap(edit_tile_bitmap);
      al_put_pixel(mx, my, draw_color);
   }
   if (draw_mode > 1)
   {
      // get initial point
      int px1 = mx;
      int py1 = my;

      // make temp bitmap
      ALLEGRO_BITMAP *tmp2 = al_create_bitmap(20, 20);

      int draw_quit = 0;
      while (!draw_quit)
      {
         edit_tile_redraw(tmp2, false);
         if (mouse_on_grid)
         {
            // get a fresh copy of tile to draw on
            al_set_target_bitmap(tmp2);
            al_clear_to_color(al_map_rgb(0,0,0));
            al_draw_bitmap(edit_tile_bitmap,0,0,0);


            if (draw_mode == 2)
            {
               sprintf(title, "Draw Line x1:%d y1:%d  x2:%d y2:%d", px1, py1, mx, my);
               al_draw_line(px1+o, py1+o, mx+o, my+o, draw_color, th);
               // draw the line with filled circles for custom endpoints
               al_draw_filled_circle(px1+o, py1+o, th/2, draw_color);
               al_draw_filled_circle(mx+o,  my+o,  th/2, draw_color);
            }

            if (draw_mode == 3)
            {
               sprintf(title, "Draw Rectangle x1:%d y1:%d  x2:%d y2:%d", px1, py1, mx, my);
               al_draw_rectangle(px1+o, py1+o, mx+o, my+o, draw_color, th);

               al_draw_filled_circle(px1+o, py1+o, th/2, draw_color);
               al_draw_filled_circle(px1+o, my+o,  th/2, draw_color);
               al_draw_filled_circle(mx+o,  py1+o, th/2, draw_color);
               al_draw_filled_circle(mx+o,  my+o,  th/2, draw_color);

            }
            if (draw_mode == 4)
            {
               float r = fabs(px1*d_scale-mInput.mouse_x)/d_scale;
               sprintf(title, "Draw Circle x1:%d y1:%d  r:%2.1f", px1, py1, r);
               al_draw_circle(px1+o, py1+o, r, draw_color, th);
            }
            if (draw_mode == 5)
            {
               float r1 = fabs(px1*d_scale-mInput.mouse_x)/d_scale;
               float r2 = fabs(py1*d_scale-mInput.mouse_y)/d_scale;
               sprintf(title, "Draw Ellipse x1:%d y1:%d  r1:%2.1f r2:%2.1f ", px1, py1, r1, r2);
               al_draw_ellipse(px1+o, py1+o, r1, r2, draw_color, th);
            }

            if (draw_mode == 6)
            {
               sprintf(title, "Draw Filled Rectangle x1:%d y1:%d  x2:%d y2:%d", px1, py1, mx, my);
               al_draw_filled_rectangle(px1+o, py1+o, mx+o, my+o, draw_color);
            }
            if (draw_mode == 7)
            {
               float r = fabs(px1*d_scale-mInput.mouse_x)/d_scale;
               sprintf(title, "Draw Filled Circle x1:%d y1:%d  r:%2.1f", px1, py1, r);
               al_draw_filled_circle(px1+o, py1+o, r, draw_color);
            }
            if (draw_mode == 8)
            {
               float r1 = fabs(px1*d_scale-mInput.mouse_x)/d_scale;
               float r2 = fabs(py1*d_scale-mInput.mouse_y)/d_scale;
               sprintf(title, "Draw Filled Ellipse x1:%d y1:%d  r1:%2.1f r2:%2.1f ", px1, py1, r1, r2);
               al_draw_filled_ellipse(px1+o, py1+o, r1, r2, draw_color);
            }
            if (!mInput.mouse_b[1][0]) // mouse release, commit temp bitmap
            {
               draw_quit = 1;
               al_set_target_bitmap(edit_tile_bitmap);
               al_draw_bitmap(tmp2, 0,0,0);
            }
            if (mInput.key[ALLEGRO_KEY_ESCAPE][0]) // escape cancel, do not commit
            {
               while (mInput.key[ALLEGRO_KEY_ESCAPE][0]) mEventQueue.proc(1);
               draw_quit = 1;
               while (mInput.mouse_b[1][0]) mEventQueue.proc(1); // wait for release
            }
         }
      }
   }
}



void mwTileEditor::show_color_replace_preview()
{
   al_set_target_bitmap(edit_tile_preview_bitmap);
   al_clear_to_color(al_map_rgb(0,0,0));
   al_draw_bitmap(edit_tile_bitmap, 0,0,0);


   for (int x=0; x<20; x++)
      for (int y=0; y<20; y++)
      {
         ALLEGRO_COLOR s = al_get_pixel(edit_tile_bitmap, x, y);
         if (mColor.compareColor(s, point_color, 0.01)) al_put_pixel(x, y, draw_color);
      }


}






void mwTileEditor::edit_tile(int tile)
{
   al_show_mouse_cursor(mDisplay.display);

   // load block tiles from file to bitmap, so we can modify it
   tilemap_bitmap      = mBitmapTools.load_block_tiles_to_bitmap();

   // create bitmaps
   edit_tile_bitmap    = al_create_bitmap(20, 20);
   tile_palette_bitmap = al_create_bitmap(60, 60);

   edit_tile_preview_bitmap = al_create_bitmap(20, 20);

   // set initial edit tile
   set_edit_tile(tile);

   draw_mode = 1; // point
   gridlines = 1;

   // set initial selection
   sx1 = 0;
   sy1 = 0;
   sx2 = 19;
   sy2 = 19;
   fix_sel();

   // size and position of main tile edit area
   d_scale = 32;
   dx1 = 20;
   dy1 = 20;
   dw = 20 * d_scale;
   dh = 20 * d_scale;
   dx2 = dx1 + dw;
   dy2 = dy1 + dh;


   int quit = 0;
   while (!quit)
   {
      sprintf(title, "Edit Tile:%d", edit_tile_index);
      edit_tile_redraw(edit_tile_bitmap, true);

      if (mouse_on_grid)
      {
         // outline pixel
         int mx1 = dx1 + mx * d_scale;
         int my1 = dy1 + my * d_scale;
         al_draw_rectangle(mx1, my1, mx1+d_scale, my1+d_scale, highlight_color, 0);

         // get color of pixel
         point_color = al_get_pixel(edit_tile_bitmap, mx, my);

         color_controls("point", point_color, dx1+200, dy2+14, 170);


         if (draw_mode == 9)
         {
            sprintf(title, "Replace Color");
            show_color_replace_preview();
            edit_tile_redraw(edit_tile_preview_bitmap, false);

            if (mInput.mouse_b[1][0])
            {
               while (mInput.mouse_b[1][0]) mEventQueue.proc(1);
               al_set_target_bitmap(edit_tile_bitmap);
               al_draw_bitmap(edit_tile_preview_bitmap, 0,0,0);
            }
         }
         else
         {
            // set draw color
            if (mInput.mouse_b[2][0])
            {
               while (mInput.mouse_b[2][0]) mEventQueue.proc(1);
               draw_color = point_color;
            }

            if (mInput.mouse_b[1][0])
            {
               if (!mInput.CTRL()) process_draw();
               else // get new selection
               {
                  // get initial point
                  sx1 = mx;
                  sy1 = my;
                  while (mInput.mouse_b[1][0])
                  {
                     edit_tile_redraw(edit_tile_bitmap, true);
                     if (mouse_on_grid)
                     {
                        sx2 = mx;
                        sy2 = my;
                        if (sx2 < sx1) sx2 = sx1;
                        if (sy2 < sy1) sy2 = sy1;
                        if (sx2 > 19) sx2 = 19;
                        if (sy2 > 19) sy2 = 19;
                        // show selection
                        al_draw_rectangle(dx1+o+sx1*d_scale, dy1+o+sy1*d_scale, dx1+o+(sx2+1)*d_scale, dy1+o+(sy2+1)*d_scale, mColor.Yellow, 2);
                     }
                  }
                  fix_sel();
               }
            }
         }
      }

      // show save and load buttons
      int ya = dy1-14;
      int xa = dx2-86;
      if (mWidget.buttontcb(xa, ya, 0, 14, 0,0,0,0, 0,15,15,11, 1,0,0,0, "Load"))
      {
         tilemap_bitmap = mBitmapTools.load_block_tiles_to_bitmap();
         mBitmapTools.get_tile_from_tilemap(tilemap_bitmap, edit_tile_bitmap, edit_tile_index);
      }
      xa = dx2-42;
      if (mWidget.buttontcb(xa, ya, 0, 14, 0,0,0,0, 0,15,15,10, 1,0,0,0, "Save"))
      {
         mBitmapTools.put_tile_to_tilemap(tilemap_bitmap, edit_tile_bitmap, edit_tile_index);
         mBitmapTools.save_bitmap_to_block_tiles_file(tilemap_bitmap);
         al_set_target_backbuffer(mDisplay.display);
      }

      int dcw = 170;
      // dcw += mLoop.pct_x;
      color_controls("draw", draw_color, dx1, dy2+14, dcw);

      legacy_color_select(dx1+400, dy2+14, 12);

      // all the widgets to the right of the main tile area

      xa = dx2+20;
      ya = 20;
      selection_controls(xa, ya);

      ya+=152;
      ya += draw_mode_controls(xa, ya);

      ya+=20;
      tile_palette_select(xa, ya, 3);


      if (mInput.key[ALLEGRO_KEY_ESCAPE][0])
      {
         while (mInput.key[ALLEGRO_KEY_ESCAPE][0]) mEventQueue.proc(1);
         quit = 1;
      }
   }
   al_destroy_bitmap(tilemap_bitmap);
   al_destroy_bitmap(edit_tile_bitmap);
   al_destroy_bitmap(tile_palette_bitmap);
   al_destroy_bitmap(edit_tile_preview_bitmap);
}

