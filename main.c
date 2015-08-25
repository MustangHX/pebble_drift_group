//
//  main.c
//  pebble_size_drift
//
//  Created by Xiao Hu on 5/16/15.
//
//

#include <stdio.h>
#include <math.h>
#include "ex_func.h"
#include "global_ex.h"
#include "global_var.h"
int main(){
	int i,j,k,n,peb_m_i,peb_s_i;
	double Area,dr=0.25;
	FILE *fp;
	char outname[256];
	n=0;
   // group();
	printf("PPPPeEEEBBBB%.16f\n",density(1.0));
	drift(1.0,drag_group(1.0,0.01));
        printf("PPPPeEEEBBBB%.12f\n",drag_group(1.0,0.01));
/*	for(i=0;i<100;i++){
		for(j=0;j<100-1;j++){
		pebble[i][j]=pebble[i][j+1]*velocity[i][j+1]*dt/dr;
		}
	}*/
	Init();
	for(i=0;i<peb_num;i++){
		drift_t(&peb_group[i],1.0);
		printf("FINISHED %d\n",i);
		}
	for(i=0;i<100;i++){
		printf("%2.20g\t%g\t%g\t%g\n",peb_group[n].size[i],peb_group[n].vr[i],peb_group[n].time[i],peb_group[n].rad[i]);
	}
	for(i=0;i<peb_num;i++){
		for(j=0;j<outp_time;j++){
		peb_mapping[j][i].dr=0.25;
		peb_mapping[j][i].rad=i*0.25+0.125;
	
/*		peb_mapping[j][i].size[0]=0.1;
                peb_mapping[j][i].size[1]=1.0;
		peb_mapping[j][i].size[2]=5.0;
		peb_mapping[j][i].size[3]=10.0;
		peb_mapping[j][i].size[4]=20.0;
		peb_mapping[j][i].size[5]=30.0;
		peb_mapping[j][i].size[6]=40.0;
		peb_mapping[j][i].size[7]=50.0;
		peb_mapping[j][i].size[8]=70.0;
		peb_mapping[j][i].size[9]=100.0;
		peb_mapping[j][i].size[10]=1000.0;*/
		for(k=0;k<peb_size_num;k++){
			peb_mapping[j][i].surf_dens[k]=0.0;
			peb_mapping[j][i].size[k]=pow(10,k/9-1)*(k%9+1)*1.0;}
		}
		//peb_mapping[i].surf_dens[10]
	}	
	for(i=0;i<NUM_LIM;i++){
		j=0;
		while(fabs(i*dt-peb_group[peb_num-1].time[j])>0.01){
			j++;}
		printf("JJJ%d\t%d\n",j,i);
		for(k=0;k<peb_num;k++){
	                if(j>peb_group[k].max_step) j=peb_group[k].max_step;
//			printf("MAX_STEP:%d\n",peb_group[k].max_step);
			peb_m_i=(int)(peb_group[k].rad[j]*4);
//			printf("M_I:%f\n",peb_group[k].rad[j]);
			if(peb_group[k].size[j]<0.1){
				peb_s_i=0;
			}
			else if(peb_group[k].size[j]>900.0){
				peb_s_i=36;
			}
			/*else if(peb_group[k].size[j]<5.0){
				peb_s_i=2;
			}
			else if(peb_group[k].size[i]<50.0){
				peb_s_i=(int)(peb_group[k].size[j]/10.0)+3;
			}
			else if(peb_group[k].size[i]<70.0){
				peb_s_i=8;
			}
			else if(peb_group[k].size[i]<100.0){
				peb_s_i=9;
			}
			else{peb_s_i=10;}*/
			else{
			peb_s_i=floor(log10(peb_group[k].size[j]))+1;
			//peb_s_i+=floor((log10(log10(peb_group[k].size[i])-floor(log10(log10(peb_group[k].size[i])))))*10);
			peb_s_i=peb_s_i*10+floor(peb_group[k].size[j]/pow(10,peb_s_i-1));
			}
			Area=M_PI*((peb_mapping[i][peb_m_i].rad+dr/2.0)*(peb_mapping[i][peb_m_i].rad+dr/2.0)-(peb_mapping[i][peb_m_i].rad-dr/2.0)*(peb_mapping[i][peb_m_i].rad-dr/2.0))*(AU_km*100000.0)*(AU_km*100000.0);
                        printf("I %d\t M_I %d\tS_I %d\n",i,peb_m_i,peb_s_i);

//			printf("MASS%e\tAREA%e\t%e\n",peb_group[k].mass[i],Area,peb_mapping[i][peb_m_i].surf_dens[peb_s_i]);
			peb_mapping[i][peb_m_i].surf_dens[peb_s_i]+=peb_group[k].mass[i]/Area;
		//	printf("peb_dens%e\n",peb_mapping[i][peb_m_i].surf_dens[peb_s_i]);
		}
	}
	for(k=0;k<NUM_LIM;k++){
	sprintf(outname,"out_sigma%d.txt",k);
	printf("%s\n",outname);
	fp=fopen(outname,"w");
	//k=5;
	for(i=0;i<peb_num;i++){
		for(j=0;j<peb_size_num;j++){
		fprintf(fp,"%e\t",peb_mapping[k][i].surf_dens[j]);
//		printf("%e\t",peb_mapping[k][i].surf_dens[j]);

		}
		fprintf(fp,"\n");
	}
	}
	fclose(fp);
	fp=fopen("size_chart.txt","w");
	for(i=0;i<peb_size_num;i++){
		fprintf(fp,"%f\n",peb_mapping[0][0].size[i]);
	}
	fclose(fp);
	fp=fopen("rad_chart.txt","w");
	for(i=0;i<peb_num;i++){
		fprintf(fp,"%f\n",peb_mapping[0][i].rad);
	}
	fclose(fp);

    return 1;
}
