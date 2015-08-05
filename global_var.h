#ifndef PEB_STRUCT
#define PEB_STRUCT
extern double peb_cont[100][100];
typedef struct PEBBLE{
        double rad[10000];
        double size[10000];
        double time[10000];
        double vr[10000];
} PEBBLE;
extern PEBBLE peb_group[10];
#endif
