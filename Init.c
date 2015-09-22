#include "global_var.h"
#include "global_ex.h"
#include "ex_func.h"
#include "math.h"
#include <stdio.h>
double Sigma (double r){
	return 108*pow(r,-0.6);
}
void Init(){
	int i;
/*        for(i=0;i<100;i++){
                size=drag_group(i*1.0+0.5,0.01);
                for(j=0;j<100;j++){		
			if(j==(int)(size*10)){
                               pebble[i][j]=i*1.0*2*M_PI*dr;
		                             }
		                  }
		          }
	fp=fopen("velocity.dat","r");
	if(fp=None){
		for(j=0;j<100;j++){
			size=j*1.0;
			for(i=0;i<100;i++){
				velocity[i][j]
			
}
}
}*/	
	for(i=0;i<peb_num;i++){
		dust_budget[i].rad=i*0.25+0.125;
		dust_budget[i].dr=0.25;
		dust_budget[i].surf_dens=Sigma(dust_budget[i].rad)*0.01;
	}
	for(i=0;i<peb_num;i++){
		peb_group[i].size[0]=drag_group((i+1)*1.0,0.01);
		peb_group[i].rad[0]=(i+4)*0.25;
		peb_group[i].mass[0]=0.01*(peb_group[i].rad[0]*peb_group[i].rad[0]-(peb_group[i].rad[0]-0.25)*(peb_group[i].rad[0]-0.25))*M_PI*dust_budget[i].surf_dens*(AU_km*100000.0)*(AU_km*100000.0);
		peb_group[i].time[0]=0.0;
		printf("MASS%e\n",peb_group[i].mass[0]);
	}

}
