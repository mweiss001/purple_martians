// mwFileIterator.h

class mwFileIterator
{
   public:

   mwFileIterator(); // default constructor
   void initialize();

   int iterate(const char* fname);

   int get_most_recent(const char* fname);

   char* get_most_recent_fname(const char* fname, char* tmp);

   ALLEGRO_FS_ENTRY *filenames[1000];
   int num_filenames;
};
extern mwFileIterator mFileIterator;
