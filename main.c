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
int main(){
   // group();
	printf("PPPPeEEEBBBB%.16f\n",density(1.0));
	drift(1.0,drag_group(1.0,0.01));
        printf("PPPPeEEEBBBB%.12f\n",drag_group(1.0,0.001));
	for(i=0;i<100;i++){
		for(j=0;j<100-1;j++){
		pebble[i][j]=pebble[i][j+1]*velocity[i][j+1]*dt/dr;
		}
	}
	for(i=0;i<10;i++){
		drift_t(peb_group[i],1.0);
		}
    return 1;
}
