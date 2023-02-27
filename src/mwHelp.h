// mwHelp.h

class mwHelp
{
   public:
   void help(const char *topic);

   private:
   int load_help(void);
   char help_string[5000][200];
};
extern mwHelp mHelp;



