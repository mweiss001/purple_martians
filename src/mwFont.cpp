// mwFont.cpp

#include "pm.h"
#include "mwFont.h"
#include "mwDisplay.h"
#include "mwInput.h"

mwFont mF;

void mwFont::load_fonts(void)
{
   al_destroy_font(bltn);
   bltn = al_create_builtin_font();
   if (!bltn)
   {
      mI.m_err("Failed to create_builtin_font");
      exit(0);
   }

   // now we get pristine font from bitmap in the interest of drawing faster
   ALLEGRO_BITMAP* tmp = al_load_bitmap("bitmaps/Pristine_8.bmp");
   if (!tmp) mI.m_err("Can't load bitmaps//Pristine_8.bmp");
   else
   {
      al_convert_mask_to_alpha(tmp, al_map_rgb(0, 0, 0)) ;
      al_destroy_font(pr8);
      int ranges[] = {32, 127};
      pr8 = al_grab_font_from_bitmap(tmp, 1, ranges);
      if (!pr8) mI.m_err("Failed to load font from bitmaps/Pristine_8.bmp");
      al_destroy_bitmap(tmp);
   }

   // pristine 16x16 version
   tmp = al_load_bitmap("bitmaps/Pristine_16.bmp");
   if (!tmp) mI.m_err("Can't load bitmaps//Pristine_16.bmp");
   else
   {
      al_convert_mask_to_alpha(tmp, al_map_rgb(0, 0, 0)) ;
      al_destroy_font(pr16);
      int ranges[] = {32, 127};
      pr16 = al_grab_font_from_bitmap(tmp, 1, ranges);
      if (!pr16) mI.m_err("Failed to load font from bitmaps/Pristine_16.bmp");
      al_destroy_bitmap(tmp);
   }

   al_destroy_font(acha);
   acha = al_load_ttf_font("bitmaps/Achafont.ttf", 240, 0);
   if (!acha) mI.m_err("Failed to load font from bitmaps/Achafont.ttf");

   int sfs = al_get_display_option(display, ALLEGRO_MAX_BITMAP_SIZE) / 20;
   //printf("Saucer font size:%d\n", sfs);
   al_destroy_font(sauc);
   sauc = al_load_ttf_font("bitmaps/SaucerBB.ttf", sfs, 0);
   if (!sauc) mI.m_err("Failed to load font from bitmaps/SaucerBB.ttf");

   al_destroy_font(pixl);
   pixl = al_load_ttf_font("bitmaps/PixelGosub.otf", 7, ALLEGRO_TTF_MONOCHROME | ALLEGRO_TTF_NO_KERNING);
   if (!pixl) mI.m_err("Failed to load font from bitmaps/PixelGosub.otf");
}


void mwFont::mw_get_text_dimensions(ALLEGRO_FONT *f, const char* txt, int &bx, int &by, int &bw, int &bh)
{
    // first get from the allegro method
   al_get_text_dimensions(f, txt, &bx, &by, &bw, &bh);

   // then override for my nefarious purposes!
   if (f == mF.pixl)    { by = 4; bh = 5; }
   if (f == mF.pr8)  { by = 0; bh = 8; bx = 0; bw = 8*strlen(txt); }
   if (f == mF.bltn) { by = 0; bh = 8; }
}



void mwFont::convert_ttf_to_bitmap_font(const char* ttf_filename, const char* bmp_filename, int char_size)
{
   char msg[1024];
   char fn[256];
   // converts ttf fonts to bitmap mF.pr8 to draw faster and have consistant size
   sprintf(fn, "bitmaps/%s", ttf_filename);
   ALLEGRO_FONT *cf = al_load_ttf_font(fn, char_size, ALLEGRO_TTF_NO_KERNING | ALLEGRO_TTF_MONOCHROME | ALLEGRO_TTF_NO_AUTOHINT);
   if (!cf)
   {
      sprintf(msg, "Failed to load font from bitmaps/%s", ttf_filename);
      mI.m_err("Failed to load font from bitmaps/Pristine.ttf");
   }
   else printf("loaded font %s\n", msg);

   int bw = 1;                 // border width
   int step0 = char_size+bw;   // step size between char
   int step1 = char_size;      // char size
   int bmp_w = (16 * step0) + bw;
   int bmp_h = (6  * step0) + bw;

   ALLEGRO_BITMAP *b = al_create_bitmap(bmp_w, bmp_h);
   al_set_target_bitmap(b);
   al_clear_to_color(al_map_rgb(255, 0, 0));  // set entire bitmap to red, this will be the background color separating the glyphs

   int y = bw;
   for (int b=32; b<128; b+=16)
   {
      int x = bw;
      for (int a=0; a<16; a++)
      {
         al_draw_filled_rectangle(x+a*step0, y, x+a*step0+step1, y+step1, al_map_rgb(0, 0, 0)); // clear glyph background
         al_draw_textf(cf, al_map_rgb(255, 255, 255), x+a*step0, y, 0, "%c", a+b);
      }
      y+=step0;
   }
   al_convert_mask_to_alpha(b, al_map_rgb(0, 0, 0)); // alpha does not actually save in bmp format :(
   al_set_target_backbuffer(display);
   al_draw_bitmap(b, 0, 0, 0);
   sprintf(fn, "bitmaps/%s", bmp_filename);
   al_save_bitmap(fn, b);
   al_destroy_font(cf);
}







