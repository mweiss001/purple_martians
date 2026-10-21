// mwMessageText.cpp

#include "pm.h"
#include "mwMessageText.h"

mwMessageText::mwMessageText()
{
   initialize();
}

void mwMessageText::initialize()
{
   messageTextV.clear();
   memset(messageTextA, 0, sizeof(messageTextA));
}

// remove index
void mwMessageText::remove(int index)
{
   std::erase_if(messageTextV, [index](const messageText& m){ return m.index == index; });
}

// retrieve the text for an index
std::string mwMessageText::get_text(int index)
{
   for (auto tmp: messageTextV)
      if (tmp.index == index)
         return tmp.text;

   return "";
}


// set text for an index:
void mwMessageText::set_text(int index, const char* txt)
{
   // check if index already exists
   int exists = 0;
   for (auto& tmp: messageTextV)
      if (tmp.index == index)
      {
         tmp.text = txt;
         exists = 1;
      }
   if (!exists)
   {
      struct messageText mt;
      mt.index = index;
      mt.text = txt;
      messageTextV.push_back(mt);
   }
}

// pack vector to char array for saving
void mwMessageText::VtoA()
{
   // clear array
   memset(messageTextA, 0, sizeof(messageTextA));

   // initial variables for filling char array
   int sz = 0, offset = 0;

   // get vector size
   int vsize = (int) messageTextV.size();

   // copy vector size to char array and advance offset
   sz = sizeof(vsize);
   memcpy(messageTextA+offset, &vsize, sz);
   offset += sz;

   for (auto tmp: messageTextV)
   {
      // copy index to char array and advance offset
      int index = tmp.index;
      sz = sizeof(index);
      memcpy(messageTextA+offset, &index, sz);
      offset += sz;


      // get the string size
      int ssize = (int) tmp.text.size();

      // copy string size to char array and advance offset
      sz = sizeof(ssize);
      memcpy(messageTextA+offset, &ssize, sz);
      offset += sz;

      // copy string to char array and advance offset
      char txt[500];
      strcpy(txt, tmp.text.c_str());
      memcpy(messageTextA+offset, txt, ssize);
      offset += ssize;

      if (offset > 4500)
      {
         printf("message text too long\n");
         return;
      }
   }

}

// unpack array to vector after loading
void mwMessageText::AtoV()
{
   // clear vector
   messageTextV.clear();

   // initial variables for reading char array
   int sz = 0, offset = 0;

   // get the vector size
   int vsize = 0;
   sz = sizeof(vsize);
   memcpy(&vsize,  messageTextA+offset, sz);
   offset += sz;

   for (int i=0; i<vsize; i++)
   {
      // get the index
      int index = 0;
      sz = sizeof(index);
      memcpy(&index,  messageTextA+offset, sz);
      offset += sz;

      // get the string size
      int ssize = 0;
      sz = sizeof(ssize);
      memcpy(&ssize,  messageTextA+offset, sz);
      offset += sz;

      // get the string
      char txt[500] = {0};
      memcpy(txt,  messageTextA+offset, ssize);
      offset += ssize;

      // add to vector
      struct messageText mt;
      mt.index = index;
      mt.text = txt;
      messageTextV.push_back(mt);
   }
}




