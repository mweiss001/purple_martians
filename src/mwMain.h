// mwMain.h

class mwMain
{
   public:

   void final_wrapup();
   void fast_exit(int type);
   void show_system_id();
   void set_exe_path();
   void set_and_get_versions();
   int initial_setup();

   int pm_main(int argument_count, char **argument_array);

   int classic_mode;

   int headless_server = 0;





   // z_args.h
   void pm_copy_src(const char* filepath);
   void pm_copy_exe(const char* filepath);
   void pm_copy_cfg(const char* filepath);
   void pm_copy_levels(const char* filepath);
   void pm_delete_all(const char* filepath);
   void pm_copy_misc(const char* filepath);
   void copy_files_to_clients(int exe_only);


   void proc_command_line_args1(int argument_count, char **argument_array);
   void proc_command_line_args2(int argument_count, char **argument_array);
   void temp_test();

};
extern mwMain mMain;



