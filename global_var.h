#define time_yr 10000
#define peb_num 40
#define outp_time 100
#define NUM_LIM 100
#define peb_size_num 41
#ifndef PEB_STRUCT
#define PEB_STRUCT
extern double peb_cont[100][100];
typedef struct PEBBLE{
	int max_step;
	double mass[time_yr];
        double rad[time_yr];
        double size[time_yr];
        double time[time_yr];
        double vr[time_yr];
} PEBBLE;
extern PEBBLE peb_group[peb_num];

typedef struct PEBBLE_MAP{
        double dr;
	double rad;
        double size[peb_size_num];
        double surf_dens[peb_size_num];
} PEBBLE_MAP;

typedef struct DUST_MAP{
        double dr;
        double rad;
        double surf_dens;
} DUST_MAP;
extern PEBBLE peb_group[peb_num];
extern PEBBLE_MAP peb_mapping[outp_time][peb_num];
extern DUST_MAP dust_budget[peb_num];
#endif
