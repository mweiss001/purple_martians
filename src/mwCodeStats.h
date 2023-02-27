// mwCodeStats.h

struct code_stat
{
   char fname[256];
   char name[256];
   int lines;
   int chars;
   int blank_chars;
   int blanks;
   int code;
   int comment;
   int comment_only;
   int code_only;
   int code_and_comment;
   int max_line_length;
};

class mwCodeStats
{
   public:
   static void run(void);

   private:
   static void fill_stat_struct(struct code_stat &cs);

};

