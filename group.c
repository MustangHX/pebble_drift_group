#include"global_ex.h"
#include"ex_func.h"
double drag_group(double r, double tau){

double a_peb;

a_peb=tau*density(r)*sound_sp(r)*100000.0*sqrt(8.0/M_PI/gamma0)/(w_K(r)*rho_peb0);

printf("GGGG%.20f\n",density(1.0));

if(a_peb<2.25*mean_path(r)) return a_peb;
else return sqrt(9*tau*viscosity(r)*density(r)/(2*w_K(r)*rho_peb0));

}

struct pebble{
	double rad[10000];
	double size[10000];
	double time[10000];
	double vr[10000];
} peb_group[10];

double group(int argc){

	return 1;
	for(i=0;i<100;i++){
		size=drag_group(i*1.0+0.5,0.01);
		for(j=0;j<100;j++){
			if(j==(int)(size*10)){
				peb_cont[i][j]=i*1.0*2*M_PI*dr;
			}
		}
	}

	

}
