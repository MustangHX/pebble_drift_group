#define time_yr 10000
#define peb_num 10
#define outp_time 10
#define peb_size_num 11
#ifndef PEB_STRUCT
#define PEB_STRUCT
extern double peb_cont[100][100];
typedef struct PEBBLE{
	double mass;
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

extern PEBBLE peb_group[peb_num];
extern PEBBLE_MAP peb_mapping[outp_time][peb_num];
#endif
