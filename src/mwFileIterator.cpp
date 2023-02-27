// mwFileIterator.cpp

#include "pm.h"
#include "mwFileIterator.h"

mwFileIterator mFileIterator;

mwFileIterator::mwFileIterator()
{
   initialize();
}

void mwFileIterator::initialize(void)
{
   mFileIterator.num_filenames = 0;
   for (int i=0; i<1000; i++) al_destroy_fs_entry(filenames[i]);
}

int add_file(ALLEGRO_FS_ENTRY *fs, void * extra)
{
   if (mFileIterator.num_filenames > 999) return 0; // only get 1000 max
   mFileIterator.filenames[mFileIterator.num_filenames++] = al_create_fs_entry(al_get_fs_entry_name(fs));
   return ALLEGRO_FOR_EACH_FS_ENTRY_OK;
}

int mwFileIterator::iterate(const char* fname)
{
   initialize();
   ALLEGRO_FS_ENTRY *FS_fname = al_create_fs_entry(fname);
   // iterate files in folder and put in filename array
   al_for_each_fs_entry(FS_fname, add_file, NULL);
   return num_filenames;
}

int mwFileIterator::get_most_recent(const char* fname)
{
   iterate(fname);
   if (num_filenames == 0) printf("No log files found.\n");
   else
   {
      //for (int i=0; i< num_filenames; i++)
         //printf("%s\n", al_get_fs_entry_name(filenames[i]));
      time_t t = 0;
      int latest = 0;
      for (int i=0; i< num_filenames; i++)
         if (al_get_fs_entry_ctime(filenames[i]) > t)
         {
            t = al_get_fs_entry_ctime(filenames[i]);
            latest = i;
         }
      //printf("most recent file is: %s\n", al_get_fs_entry_name(filenames[latest]));
      //sprintf(fname, "%s", al_get_fs_entry_name(filenames[latest]));
      return latest;
   }
   return -1;
}

char* mwFileIterator::get_most_recent_fname(const char* fname, char* tmp)
{
   int index = mwFileIterator::get_most_recent(fname);
   if (index > -1) sprintf(tmp, "%s", al_get_fs_entry_name(filenames[index]));
   else sprintf(tmp, " ");
   return tmp;
}


