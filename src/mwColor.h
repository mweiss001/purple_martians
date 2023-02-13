// mwColor.h

class mwColor
{
   public:

   mwColor(); // default constructor
   void initialize(void);

   void process_flash_color(void);

   ALLEGRO_COLOR pc[256];
   int flash_color = 0;
   int flash_color2 = 0;
   int flash_counter = 0;

   char color_name[16][20];

   ALLEGRO_COLOR Flash1;
   ALLEGRO_COLOR Flash2;

   ALLEGRO_COLOR Black;
   ALLEGRO_COLOR Violet;
   ALLEGRO_COLOR Mauve;
   ALLEGRO_COLOR Bluey;
   ALLEGRO_COLOR Reddy;
   ALLEGRO_COLOR Pink;
   ALLEGRO_COLOR Taan;
   ALLEGRO_COLOR Orange;
   ALLEGRO_COLOR Purple;
   ALLEGRO_COLOR Forest;
   ALLEGRO_COLOR Red;
   ALLEGRO_COLOR Green;
   ALLEGRO_COLOR Blue;
   ALLEGRO_COLOR Aqua;
   ALLEGRO_COLOR Yellow;
   ALLEGRO_COLOR White;

};
extern mwColor mC;


