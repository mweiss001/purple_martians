// mwCodeStats.cpp

#include "pm.h"
#include "mwCodeStats.h"
#include "mwFileIterator.h"

void mwCodeStats::fill_stat_struct(struct code_stat &cs)
{
   int done = 0;
   int debug_print = 0;

   cs.lines = 0;
   cs.chars = 0;
   cs.blank_chars = 0;
   cs.blanks = 0;
   cs.code = 0;
   cs.comment = 0;
   cs.comment_only = 0;
   cs.code_only = 0;
   cs.code_and_comment = 0;
   cs.max_line_length = 0;


   FILE *filepntr=fopen(cs.fname,"r");
   while(!done)
   {
      int ch = fgetc(filepntr);
      int loop = 0;
      char buff[512] = {0};
      while((ch != '\n') && (ch != EOF) && (loop < 500))
      {
         if (ch != 13) buff[loop++] = ch;
         ch = fgetc(filepntr);
      }
      buff[loop] = 0; // terminate the string
      if (loop >= 500) printf("line exceeded 500 char - %s\n", buff);


      if (debug_print) printf("%d - %s\n", cs.lines, buff);

      if (ch == EOF) done = 1;
      else
      {
         cs.lines++;                                          // add to line count
         int len = strlen(buff);                           // get line length
         cs.chars += len;                                     // add to total num of char
         if (len > cs.max_line_length) cs.max_line_length = len; // get max line length
         //printf("\n%d - len:%d\n%s\n", lines, len, buff);
         if (len < 1) cs.blanks++;                            // zero length line is blank
         else                                              // process non zero length line
         {
            int blank_c = 1; // set line blank by default, if any non blank space is found clear it
            int comment_c = 0;
            int code_c = 0;

            // go through the line one char at a time
            for (int i=0; i<len; i++)
            {
               int c = buff[i]; // current char
               //printf("%c[%d] ", buff[i], buff[i]);

               // look for lines that have only (9-tab or 32-space)
               // this never fires, I think the IDE (code::blocks) does not save spaces or tabs on a line if that is the only thing on the line
               if ((c == 9) || (c == 32)) cs.blank_chars++; // add to blank char count
               else blank_c = 0;  // entire line has at least one non blank

               if (i > 0) // look for patterns that include previous char
               {
                  int pc = buff[i-1];  // previous char
                  if ((c == 47) && (pc == 47)) // current and previous are both /
                  {
                     comment_c = 1;

                     // check if not first non blank thing on line
                     if ((i-1) > 0)
                     {
                        for (int j=0; j<(i-1); j++)
                        {
                           int jc = buff[j];
                           if ((jc != 9) && (jc != 32)) code_c = 1;
                        }
                     }
                  }
               }
            }
            if ((!blank_c) && (!comment_c)) code_c = 1; // code only (line is not blank and does not have a comment)
            if (blank_c) cs.blanks++;
            if (code_c) cs.code++;
            if (comment_c) cs.comment++;
            if ((code_c) && (comment_c)) cs.code_and_comment++;
            if ((code_c) && (!comment_c)) cs.code_only++;
            if ((!code_c) && (comment_c)) cs.comment_only++;
         }
      }
   }
   fclose(filepntr);
//   printf("\n");
//   printf("%6d - Total lines\n", lines);
//   printf("%6d - Blank lines\n", blanks);
//
//   printf("%6d - Total non-blank lines\n", lines-blanks);
//   printf("%6d - Code\n", code);
//   printf("%6d - Comments\n", comment);
//   printf("\n");
//
//   printf("%6d - Blank lines\n", blanks);
//   printf("%6d - Code\n", code);
//   printf("%6d - Comments\n", comment);
//
//   printf("%6d - Code Only\n", code_only);
//   printf("%6d - Comment Only\n", comment_only);
//   printf("%6d - Code and Comment\n", code_and_comment);
//
//   printf("--------------------\n");
//   printf("%6d - Total lines\n", lines);
//
//   printf("\nmax line length: %d\n", max_line_length);
//   printf("average length of non blank lines: %d\n", chars / (lines-blanks));
//   printf("%6d - Total chars\n", chars);
//   printf("%6d - Blank chars\n", blank_chars);
//   printf("test Code:[%d] should equal Code Only:[%d] plus Code and Comments:[%d]   [%d]\n", code, code_only, code_and_comment, code_only+code_and_comment);
//   printf("test Comment:[%d] should equal Comment Only:[%d] plus Code and Comments:[%d]   [%d]\n", comment, comment_only, code_and_comment, comment_only+code_and_comment);
//   printf("test Total:[%d] should equal Comment Only:[%d] plus Code only:[%d] plus Code and Comments:[%d]  plus blank:[%d]   [%d]\n",
//           lines, comment_only, code_only, code_and_comment, blanks, comment_only + code_only + code_and_comment + blanks);
}

void mwCodeStats::run(void)
{
   char msg[1024];
   struct code_stat cs[200] = {0};

   int num_files = mFileIterator.iterate("src/");

   if (num_files == 0) printf("No files found.\n");
   else
   {
      for (int i=0; i<num_files; i++)
      {
         strcpy(cs[i].fname, al_get_fs_entry_name(mFileIterator.filenames[i]));
         fill_stat_struct(cs[i]);

         // get just the name part of the path
         ALLEGRO_PATH * path = al_create_path(cs[i].fname);
         sprintf(cs[i].name, "%s", al_get_path_filename(path));
         al_destroy_path(path);
      }
   }
   // do the totals
   for (int i=0; i<num_files; i++)
   {
      cs[199].lines +=            cs[i].lines;
      cs[199].chars +=            cs[i].chars;
      cs[199].blank_chars +=      cs[i].blank_chars;
      cs[199].blanks +=           cs[i].blanks;
      cs[199].code +=             cs[i].code;
      cs[199].comment +=          cs[i].comment;
      cs[199].comment_only +=     cs[i].comment_only;
      cs[199].code_only +=        cs[i].code_only;
      cs[199].code_and_comment += cs[i].code_and_comment;
      cs[199].max_line_length +=  cs[i].max_line_length;
   }
//   sprintf(cs[199].name, "Total");
   sprintf(cs[199].name, "%d files", num_files);

   time_t now = time(NULL);
   struct tm *timenow = localtime(&now);
   strftime(msg, sizeof(msg), "%Y-%m-%d  %H:%M:%S", timenow);
   printf("\n\nPurple Martians Source Code Line Counts [%s]\n\n", msg);

   printf("[%6s][%6s][%6s][%6s][%6s][%6s][%6s]\n", "total", "code", "cd_onl", "commnt", "cm_onl", "cd+cm", "blank");
   printf("--------------------------------------------------------\n");

  // int init_max = cs[199].code; // save this because we erase it when sorting by code size

   if (0) // list sorted alphabetically (or how file iterator chose them)
   {
      for (int i=0; i<num_files; i++)
         printf("[%6d][%6d][%6d][%6d][%6d][%6d][%6d] - %s\n", cs[i].lines, cs[i].code, cs[i].code_only, cs[i].comment, cs[i].comment_only, cs[i].code_and_comment, cs[i].blanks, cs[i].name);
   }
   else  // list sorted by code size
   {

      int cur_s = 1;  // current max size
      int i = 0; // current max size index
      while (cur_s)
      {
         cur_s = 0;
         for (int j=0; j<num_files; j++)
            if (cs[j].code > cur_s)
            {
               cur_s = cs[j].code;
               i = j;
            }
         if (cur_s > 0)
         {
            printf("[%6d][%6d][%6d][%6d][%6d][%6d][%6d] - %s\n", cs[i].lines, cs[i].code, cs[i].code_only, cs[i].comment, cs[i].comment_only, cs[i].code_and_comment, cs[i].blanks, cs[i].name);
            cs[i].code = 0; // set to zero so we won't print again
         }
      }
   }

   int i = 199;
//   cs[i].code = init_max;
   printf("-------------------------------------------------------- - totals\n");
   printf("[%6d][%6d][%6d][%6d][%6d][%6d][%6d] - %s\n", cs[i].lines, cs[i].code, cs[i].code_only, cs[i].comment, cs[i].comment_only, cs[i].code_and_comment, cs[i].blanks, cs[i].name);

}


