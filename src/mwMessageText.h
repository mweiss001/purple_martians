// mwMessageText.h



struct messageText
{
   int index;
   std::string text;
};






class mwMessageText
{
public:

   mwMessageText(); // default constructor

   void initialize();


   std::vector<messageText> messageTextV;
   char messageTextA[5000];



   std::string get_text(int index);

   void set_text(int index, const char* txt);

   void remove(int index);



   void VtoA();
   void AtoV();


};



