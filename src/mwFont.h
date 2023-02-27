// mwFont.h

class mwFont
{
   private:
   void convert_ttf_to_bitmap_font(const char* ttf_filename, const char* bmp_filename, int char_size);

   public:
   void load_fonts(void);

   void mw_get_text_dimensions(ALLEGRO_FONT *f, const char* txt, int &bx, int &by, int &bw, int &bh);


   ALLEGRO_FONT *bltn; // built-in thick 8x8
   ALLEGRO_FONT *pr8;  // pristine 8x8
   ALLEGRO_FONT *pr16; // pristine 16x16
   ALLEGRO_FONT *sauc; // saucer bb
   ALLEGRO_FONT *acha; // a charming font
   ALLEGRO_FONT *pixl; // pixel gosub
};
extern mwFont mFont;






