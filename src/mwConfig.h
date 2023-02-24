// mwConfig.h


#define STRINGIFY_HELPER(x) #x
#define STRINGIFY(x) STRINGIFY_HELPER(x)
#define agci(section, var, def) val = al_get_config_value(cfg, STRINGIFY(section), STRINGIFY(var)); if (!val) var = def;  else var = atoi(val);
#define asci(section, var) sprintf(msg, "%d", var); al_set_config_value(cfg, STRINGIFY(section), STRINGIFY(var), msg);
#define agcf(section, var, def) val = al_get_config_value(cfg, STRINGIFY(section), STRINGIFY(var)); if (!val) var = def;  else var = atof(val);
#define ascf(section, var) sprintf(msg, "%f", var); al_set_config_value(cfg, STRINGIFY(section), STRINGIFY(var), msg);

/*
https://godbolt.org/
gcc-E
main()
{
    agci(SCREEN, disp_x_wind, 100)
    asci(SCREEN, disp_x_wind)
    agcf(SCREEN, scale_factor, 0.75)
    ascf(SCREEN, scale_factor)
}
main()
{
    val = al_get_config_value(cfg, "SCREEN", "disp_x_wind"); if (!val) disp_x_wind = 100; else disp_x_wind = atoi(val);
    sprintf(msg, "%d", disp_x_wind); al_set_config_value(cfg, "SCREEN", "disp_x_wind", msg);
    val = al_get_config_value(cfg, "SCREEN", "scale_factor"); if (!val) scale_factor = 0.75; else scale_factor = atof(val);
    sprintf(msg, "%0.3f", scale_factor); al_set_config_value(cfg, "SCREEN", "scale_factor", msg);
}
*/


class mwConfig
{
   public:
   void save(void);
   void load(void);
};
extern mwConfig mConfig;


