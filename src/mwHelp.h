// mwHelp.h

class mwHelp
{
   public:
   void help(const char *topic);

   private:
   void load_help(void);


   char help_string[5000][200];
   int num_lines;

   char section_names[60][80];
   int section_first_lines[60];
   int num_sections;










};
extern mwHelp mHelp;



