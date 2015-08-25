//
//  drift.c
//  pebble_size_drift
//
//  Created by Xiao Hu on 5/16/15.
//
//

/*typedef struct PEBBLE{
	        double rad[10000];
		        double size[10000];
			        double time[10000];
				        double vr[10000];
} PEBBLE;*/

#include <stdio.h>
#include <math.h>
#include "global_var.h"
#define dt 10.0  //time step in yr
#define output_size "peb_size.txt"
//#define output_time "001alpha1cm100AU001AU1sun01acc.txt"
#define output_time "drift_test0.txt"
#define EFF 0.0
#define a_peb0 1.0 //radius in cm
//#define M_PI =3.141592654;
#define r_out 100.0
#define r_in 0.1
#define step0 0.001
#define AU_km 1.49597871e8
#define yr_sec 3.15569e7
#define k_B 1.38e8
#define acc_rate 1.0 //1.0e-9 M_sun/yr
#define opa 1.0 //10cm^2g^-1
#define gamma 1.0 //1.4 for rotatation excited H2
#define gamma0 1.4
#define m_star 1.0//in solar mass
//if (m_star>1.0){ kk=0.57;}
//else{kk=0.8;r_star=0.005*pow(m_star,kk )}//AU

#define int_m_star 1

#if int_m_star > 1
#define kk 0.57
#else
#define kk 0.8
#endif

#define r_star 0.0 //0.005 * pow(m_star,kk )

#define alpha 0.1 //in 0.001
#define rho_peb 1.0 // in 3g*cm^-3
#define rho_peb0 3.0

#define visco_air 0.146 //in cm^2/s
#define k_P 2.55

double vr_tau[2]={0.0};
double tau_temp;

double v_K(double r){
    return 29.8*sqrt(m_star)/sqrt(r);
}

double w_K(double r){
    return v_K(r)/(r*AU_km);
}

double sound_sp(double r){
    return 1.12*pow(opa,0.1)*pow(gamma,0.4)*pow(alpha,-0.1)*pow(m_star,0.15)\
    *(pow((1-sqrt(r_star/r))*acc_rate,0.2))*pow(r,(-0.45));
}
double temperature(double r){
    return 254.0*pow(opa,0.2)*pow(gamma,-0.2)*pow(alpha,-0.2)*pow(m_star,0.3)\
    *(pow((1-sqrt(r_star/r))*acc_rate,0.4))*pow(r,(-0.9));
    
}
double yeta(double r){
    return k_P*(sound_sp(r)/v_K(r))*(sound_sp(r)/v_K(r));
}

double pressure(double r){
    return k_B*1.22e16*pow(gamma,-1.2)*pow(opa,-0.1)*pow(alpha,-0.9)\
    *pow(m_star,0.85)*pow((1-sqrt(r_star/r))*acc_rate,0.8)*pow(r,-2.55);
}

double density( double r){
    return 1.87e-10*pow(gamma,(-1.2))*pow(opa,(-0.3))*pow(alpha,(-0.7))\
    *pow(m_star,0.55)*(pow((1-sqrt(r_star/r))*acc_rate,0.4))*pow(r,(-1.65));
}


double a_peb(double r){
    return a_peb0*1.0;//*np.exp((r_out-r)/5);
}


double mean_path( double r){
    return 12.5*pow(gamma,-1.2)*pow(opa,0.3)*pow(alpha,0.7)*pow(m_star,-0.55)\
    *(pow((1-sqrt(r_star/r))*acc_rate,-0.4))*pow(r,1.65);
    //return 1.0;
}

double viscosity( double r){//http://www.ifu.ethz.ch/IE/education/AQAM/GasKinetics
    //return 0.4991*sqrt(8.0/gamma0/M_PI)*sound_sp(r)*mean_path(r)*100000.0;
    return 0.5*sqrt(8.0/gamma/M_PI)*sound_sp(r)*mean_path(r)*100000.0;
}

double tau_fric(double r, double a_pb){
    double t_eps;
    t_eps=0.0178*a_pb*rho_peb*pow(gamma,0.8)*pow(opa,0.2)*pow(alpha,0.8)*\
    pow(m_star,-0.2)*pow((1-sqrt(r_star/r))*acc_rate,-0.6)*pow(r,0.6);
    
    // if (a_pb < 2.25*mean_path(r)) {
    return  0.0178*a_pb*rho_peb*pow(gamma,0.8)*pow(opa,0.2)*pow(alpha,0.8)*pow(m_star,-0.2)*pow((1-sqrt(r_star/r))*acc_rate,(-0.6))*pow(r,0.6);
    //}
    //else{
    //  return  (0.2222*a_pb*sound_sp(r)*100000.0/viscosity(r))*t_eps;
    //}
}

double tau_test(double r, double a_pb){
    //return   w_K(r)*rho_peb0*a_pb/(density(r)*sqrt(8.0/gamma/M_PI)*sound_sp(r)*100000.0);
    return     w_K(r)*2.0*rho_peb0*a_pb*a_pb/(9.0*viscosity(r)*density(r));
    
    
}

double tau_fric0(double r, double a_pb){
    double t_eps;
    t_eps=0.0178*a_pb*rho_peb*pow(gamma,0.8)*pow(opa,0.2)*pow(alpha,0.8)*\
    pow(m_star,-0.2)*pow((1-sqrt(r_star/r))*acc_rate,-0.6)*pow(r,0.6);
    
    if ( a_pb < 2.25*mean_path(r)) {
        //return  0.0178*a_pb*rho_peb*pow(gamma,0.8)*pow(opa,0.2)*pow(alpha,0.8)*pow(m_star,-0.2)*pow((1-sqrt(r_star/r))*acc_rate,(-0.6))*pow(r,0.6);
        return w_K(r)*rho_peb0*a_pb/(density(r)*sqrt(8.0/gamma/M_PI)*sound_sp(r)*100000.0);
        
    }
    else{
        //return  (0.2222*a_pb*sqrt(8.0/gamma/M_PI)*sound_sp(r)/viscosity(r))*t_eps*100000.0;
        return 2.0*w_K(r)*rho_peb0*a_pb*a_pb/(9.0*viscosity(r)*density(r));
    }
}

double v_r00(double r,double a_pb){
    //printf("tau=%f\n",tau_fric(r));
    return 0.108*(1.0/(tau_fric0(r,a_pb)+1.0/tau_fric0(r,a_pb)))*pow(gamma,0.8)*pow(opa,0.2)*pow(alpha,-0.2)*pow(m_star,-0.2)*pow((1-sqrt(r_star/r))*acc_rate,0.4)*pow(r,-0.4);
    
    
}

double v_r0(double r, double a_pb){
    // printf("tau=%f\n",tau_fric(r));
    //return 0.108*(1.0/(tau_fric(r)+1.0/tau_fric(r)))*pow(gamma,0.8)*pow(opa,0.2)*pow(alpha,-0.2)*pow(m_star,-0.2)*pow((1-sqrt(r_star/r))*acc_rate,0.4)*pow(r,-0.4);
    double tau;
    tau=w_K(r)*rho_peb0*a_pb/(density(r)*sqrt(8.0/gamma/M_PI)*sound_sp(r)*100000.0);
    //return 0.108*(1.0/(tau+1.0/tau))*pow(gamma,0.8)*pow(opa,0.2)*pow(alpha,-0.2)*pow(m_star,-0.2)*pow((1-sqrt(r_star/r))*acc_rate,0.4)*pow(r,-0.4);
    return yeta(r)/(tau+1.0/tau)*v_K(r);
    
    
    
    
}

double v_r1(double r, double a_pb){
    double tau;
    //tau=(0.2222*a_pb*sqrt(8.0/gamma0/M_PI)*sound_sp(r)/viscosity(r))*tau_fric(r)*100000.0;
    tau=w_K(r)*2.0*rho_peb0*a_pb*a_pb/(9.0*viscosity(r)*density(r));
    //tau=tau_fric0(r);
    //printf("tau=%f\n",tau);
    //printf("tau=%1.20f\n",tau);
    // return 0.108*(1.0/(tau+1.0/tau))*pow(gamma,0.8)*pow(opa,0.2)*pow(alpha,-0.2)*pow(m_star,-0.2)*pow((1-sqrt(r_star/r))*acc_rate,0.4)*pow(r,-0.4);
    return yeta(r)/(tau+1.0/tau)*v_K(r);
    
}

double v_peb_gas(double r, double a_pb){//in cm/s
    return 0.5*tau_fric(r,a_pb)*v_r0(r,a_pb)*v_K(r)*pow(10.0,10.0)/(w_K(r)*r*AU_km*pow(10,5));
}

double Reynolds(double r, double vr, double tau, double a_pb){
    double v_theta;
    v_theta=0.5*tau*vr*100000.0;
    //return 2*a_pb*sqrt(v_theta*v_theta+vr*vr*1e10)/viscosity(r);
    return 2*a_pb*sqrt(v_theta*v_theta+vr*vr*1e10)/viscosity(r);
    
}

double *v_r2(double r, double a_pb){
    double v1,v2,f0,f1,tau0,tau,tau1,t1,t2,re0;
    int j,i;
    v1=0.07*100000;
    v2=0.044556*100000;
    t1=0.3;
    if (Reynolds(r,v_r1(r,a_pb),tau_fric0(r,a_pb),a_pb)-1.0<0.1) t2=tau_fric0(r,a_pb);
    else t2=tau_temp;
    j=0;
    i=0;
    while(fabs(t2-t1)>fabs(t1)*0.000000000000001){
        if (j==0) t1=t2;
        else t1=0.5*t1+0.5*t2;
        //t2=3.095783;
        // t1=0.038;
        i=0;
        while(fabs(v2-v1)>fabs(v1)*0.0000001){// && fabs(t2-t1)>fabs(t1)*0.0001){
            if(i==0) v1=v2;
            else v1=0.5*v1+0.5*v2;
            //t1=t2;
            // t1=0;
            re0=2.0*a_pb*sqrt(1.0+0.25*t1*t1)*v1/viscosity(r);
            //printf("re0=%f\n",re0);
            tau0=w_K(r)*rho_peb0*pow(a_pb,1.6)/(9.0*density(r)*pow(viscosity(r),0.6))*pow(sqrt(1.0+0.25*t1*t1),-0.4)*pow(2.0,0.6);
            //tau0=w_K(r)*rho_peb0*pow(a_pb,1.6)/(72.0*density(r))*pow(sqrt(1.0+0.0*t1*t1)/viscosity(r),0.6)*1.51571656651;
            tau=tau0*pow(v1,-0.4);
            tau1=-0.4*tau0*pow(v1,-1.4);
            f0=yeta(r)/(tau+1.0/tau)-v1/v_K(r)/100000.0;
            f1=-1.0*yeta(r)/(tau+1.0/tau)/(tau+1.0/tau)*(tau1-tau1/tau/tau)-1.0/v_K(r)/100000;
            //t2=tau;
            v2=v1-f0/f1;
            if (v2<0.0001 && v2>0.0) {
                v2+=0.0001;
            }
            else if(v2<0.0){
                v2*=-1.0;        }
            //v1=0.035277;
            //v2=0.035;
            i++;
            //printf("tau0=%1.20f\n",tau);
            
        }
        t2=tau;
        j++;
    }
    vr_tau[0]=v2/100000.0;
    vr_tau[1]=tau0*pow(v2,-0.4);
    //printf("tau0=%1.20f\n",tau0);
    return vr_tau;
}

double *v_r3(double r, double a_pb){
    double coeff;
    //coeff=w_K(r)*rho_peb0*a_pb/(0.44*density(r));
    // printf("%f\n",(coeff*(1.0*yeta(r)*v_K(r)+coeff)));
    //printf("tau_temp=%f\n",tau_temp);
    //  printf("gua3");
    double v1,v2,f0,f1,tau0,tau,tau1,t1,t2,ft,ft1,fx,fx1,B;
    int i,j=0;
    v1=0.12*10000000;
    v2=0.125*10000000;
    t1=50.0;
    //if (Reynolds(r,v_r2(r)[0],v_r2(r)[1])-800.0<100 || 1) t2=v_r2(r)[1];
    t2=tau_temp;
    //t2=2.06;
    //printf("vr3aaaaaa %f \n",t2);
    //printf("%f\t%e\t%e\t%e\t%e\n",r,w_K(r),v_K(r),density(r),yeta(r));
    while(fabs(t2-t1)>fabs(t1)*0.00001){
        //if (j==0) t1=t2;
        //else if(fabs(t2-t1)>0.05*fabs(t1)) t1=0.01*t2+0.99*t1;
        //else t1=t2*1.0+t1*0.0;
        t1=0.0*t1+1.0*t2;
        //printf("t1=%f\n",t1);
        //while(fabs(v2-v1)>fabs(v1)*0.0001){// && fabs(t2-t1)>fabs(t1)*0.0001){
        // v1=v2;
        //t1=t2;
        // t1=0;
        //t1=247;
        // printf("t1=%f\n",t1);
        ft=w_K(r)*4.0/3.0*rho_peb0*a_pb/(0.5*24*pow(800.0,-0.6)*density(r)*sqrt(1.0+0.25*t1*t1));
        ft1=-0.25*t1*ft/(1+0.25*t1*t1);
        B=yeta(r)*v_K(r)*100000.0;
        fx=ft/sqrt(fabs(-1.0*ft*ft+ft*B))-t1;
        if (-1.0*ft*ft+ft*yeta(r)*v_K(r)*100000.0 >=0){
            fx1=-0.5*pow((B*ft-ft*ft),-1.5)*(-2.0*ft*ft1+B*ft1)*ft+ft1/sqrt(B*ft-ft*ft)-1.0;
        }
        else fx1=-0.5*pow((-1.0*B*ft+ft*ft),-1.5)*(2.0*ft*ft1-B*ft1)*ft+ft1/sqrt(-1.0*B*ft+ft*ft)-1.0;
        t2=t1-fx/fx1;
        tau0=ft;
        //tau0=w_K(r)*4.0/3.0*rho_peb0*a_pb/(0.5*24*pow(800.0,-0.6)*density(r)*sqrt(1.0+0.25*t1*t1));
        //tau0=w_K(r)*rho_peb0*pow(a_pb,1.6)/(72.0*density(r))*pow(sqrt(1.0+0.0*t1*t1)/viscosity(r),0.6)*1.51571656651;
        
        //tau=tau0/v1;
        //tau1=-1.0*tau0/v1/v1;
        //f0=yeta(r)/(tau+1.0/tau)-v1/v_K(r)/100000;
        //f1=-1.0*yeta(r)/(tau+1.0/tau)/(tau+1.0/tau)*(tau1-tau1/tau/tau)-1.0/v_K(r)/100000;
        //t2=tau;
        //v2=v1-f0/f1;
        //if (v2<0.000000000000000000001) {
        //     v2+=0.000000000000000001;
        // }
        // printf("tau=%1.20f\n",v1);
        
        //}
        j++;
        //v2=sqrt(fabs(yeta(r)*tau0*v_K(r)*100000.0-tau0*tau0));
        v2=sqrt(fabs(-1.0*ft*ft+ft*B));
        v2=yeta(r)*v_K(r)*100000.0/(t1+1.0/t1);
        //v2=0.123968*100000.0;
        //t2=tau0/v2;
        // printf("r=%f\tcoeff=%f\t tau=%f\t %f\n",r,t2,fx,fx1);
        
    }
    fx=10.0;
    if (fabs(t2-tau_temp)>0.1*tau_temp) {
        double t_temp;//[1000]={0};
        //printf("we tried \t%f\n",tau_temp);
        i=0;
        while (fabs(fx)>1.0 && i<10000){
            t_temp=tau_temp*(1.0+0.4*(i-5000)/10000);
            t1=t_temp;
            ft=w_K(r)*4.0/3.0*rho_peb0*a_pb/(0.5*24*pow(800.0,-0.6)*density(r)*sqrt(1.0+0.25*t1*t1));
            ft1=-0.25*t1*ft/(1+0.25*t1*t1);
            B=yeta(r)*v_K(r)*100000.0;
            fx=ft/sqrt(fabs(-1.0*ft*ft+ft*B))-t1;
            i++;
        }
        
    }
    t2=t1;
    v2=yeta(r)*v_K(r)*100000.0/(t1+1.0/t1);
    //printf("r=%f\t tau0=%f\t tau=%f\t fx=%f\n",r,tau0,t2,fx);
    vr_tau[0]=v2/100000.0;//sqrt(coeff*(yeta(r)*v_K(r)+coeff));
    vr_tau[1]=t2;//w_K(r)*rho_peb0*a_pb/1.32/density(r)/vr_tau[0];
    // printf("vr3bbb \n");
    return vr_tau;
    
    
}

/*double v_r(double r){
 double v1,v2,f0,f1,tau0,tau,tau1,coeff;
 //printf("%f\n",mean_path(r));
 v1=200;
 v2=100;
 tau0=w_K(r)*rho_peb0*3.0*pow(a_pb,1.6)/(72.0*density(r))*pow(viscosity(r),-0.6)*1.51571656651;
 if (a_pb < 2.25*mean_path(r)|| Reynolds(r)<=1.0 ) {
 //printf("gua1");
 return 0.108*(1.0/(tau_fric(r)+1.0/tau_fric(r)))*pow(gamma,0.8)*pow(opa,0.2)*pow(alpha,-0.2)*pow(m_star,-0.2)*pow((1-sqrt(r_star/r))*acc_rate,0.4)*pow(r,-0.4);
 
 }
 
 else if (Reynolds(r) < 800.0 && Reynolds(r)>1.0) {
 while(fabs(v2-v1)>fabs(v1)*0.0001){
 v1=v2;
 tau=tau0*pow(v1,-0.4);
 tau1=-0.4*tau0*pow(v1,-1.4);
 f0=yeta(r)/(tau0*pow(v1,-0.4)+1.0/(tau0*pow(v1,-0.4)))-v1/v_K(r);
 f1=-1.0*yeta(r)/(tau+1.0/tau)/(tau+1.0/tau)*(tau1-tau1/tau/tau)-1.0/v_K(r);
 v2=v1-f0/f1;
 }
 //printf("v2=%f\n",v2);
 //printf("%f\n",yeta(r));
 //printf("%f\n",tau0);
 //printf("%f\n",v_K(r));
 //printf("%f\n",r);
 return v2;
 }
 else{
 coeff=w_K(r)*rho_peb0*a_pb/(0.44*density(r));
 printf("%f\n",(coeff*(1.0*yeta(r)*v_K(r)+coeff)));
 printf("%f\n",yeta(r));
 printf("gua3");
 return sqrt(coeff*(yeta(r)*v_K(r)+coeff));
 
 }
 
 }*/
//    return r**-2








int drift(double r_start, double a_pebble, double coag_eff)
{
    
    int i=0,j=0,k=0,l=0,ll=0;
    FILE *fp_vr,*fp_drt,*fp_size;
    fp_vr=fopen("drift_velocity", "w");
    fp_drt=fopen(output_time,"w");
    fp_size=fopen(output_size,"w");
    
    double x0,x1,x,x_cut,x_stop,y,Re1,Re2,vr0,vr1,vr2,a_pb1,a_pb2,tau,vol_plus,tmp1,tmp2;
    double k1,k2,k3,k4,step,sum1=0.0;
    x0=r_start;
    
    x1=r_in;x=x0;y=0.0;
    //coag_eff=EFF;
    step=-1.0*step0;
    a_pb2=a_pebble;
    while (x>x1) {
        if(tau_temp <1e8 || 1)
        {
            j=0;
            a_pb1=a_pb2;
            //printf("pass0 \n");
            Re1=Reynolds(x,v_r0(x,a_pb1),tau_fric0(x,a_pb1),a_pb1);
            Re2=0.0;
            //printf("Re1=%f\n",Re1);
            if (Re1 >2.0) {
                Re2=Reynolds(x, v_r2(x,a_pb1)[0],v_r2(x,a_pb1)[1],a_pb1);
                // Re2=Reynolds(x, v_r2(x,a_pb1)[0],v_r2(x,a_pb1)[1],a_pb1);
                //Re2=Reynolds(x, v_r2(x,a_pb1)[0],v_r2(x,a_pb1)[1],a_pb1);
                //tmp1=v_r2(x,a_pb1)[0];
                //  tmp2=v_r2(x,a_pb1)[1];
                //   Re2=Reynolds(x, tmp1,tmp2,a_pb1);
                //Re1=v_r2(x,a_pb1)[0];
                // printf("Re2=%f\n",Re2);
                // printf("Stokes2b Re=%f\tVr=%f\t TAU=%f\t Yeta=%f\t x=%f\t apb=%f\n",Re2,v_r2(x,a_pb1)[0],v_r2(x,a_pb1)[1],yeta(x),x,a_pb1);
                //tmp1=v_r2(x,a_pb1)[0];tmp2=v_r2(x,a_pb1)[1];
                
                //  Re2=Reynolds(x, tmp1,tmp2,a_pb1);
                //  printf("Stokes2a Re=%f\tVr=%f\t TAU=%f\t Yeta=%f\t x=%f\t apb=%f\n",Re2,v_r2(x,a_pb1)[0],v_r2(x,a_pb1)[1],yeta(x),x,a_pb1);
                
                
            }
            while (fabs(Re1-Re2)>0.001*fabs(Re1)){
                Re1=Re2;
                if (a_pb1 < 2.25*mean_path(x)) {
                    Re2=Reynolds(x, v_r0(x,a_pb1),tau_fric0(x,a_pb1),a_pb1);
                    //printf("Epstein Re=%f\tVr=%f TAU=%f\t Yeta=%f\n",Re2,v_r0(x,a_pb1),tau_fric0(x,a_pb1),yeta(x));
                }
                else if (Re1 <= 1.0 && 1){
                    Re2=Reynolds(x, v_r1(x,a_pb1),tau_fric0(x,a_pb1),a_pb1);
                    //printf("Stokes1 Re=%f\tVr=%f\t TAU=%f\t Yeta=%f\n",Re2,v_r1(x,a_pb1),tau_fric0(x,a_pb1),yeta(x));
                    
                }
                else if (Re1 <=800.0 && Re1>1.0 && 1 ) {
                    Re2=Reynolds(x, v_r2(x,a_pb1)[0],v_r2(x,a_pb1)[1],a_pb1);
                    
                    //printf("Stokes2 Re=%f\tVr=%f\t TAU=%f\t Yeta=%f\t x=%f\t apb=%f\n",Re2,v_r2(x,a_pb1)[0],v_r2(x,a_pb1)[1],yeta(x),x,a_pb1);
                }
                else if (Re1 > 800.0 && 1 ){
                    Re2=Reynolds(x, v_r3(x,a_pb1)[0],v_r3(x,a_pb1)[1],a_pb1);
                    //printf("Stokes3 Re=%f\tVr=%g\t TAU=%f\n",Re2,v_r3(x,a_pb1)[0],v_r3(x,a_pb1)[1]);
                }
                j++;
                if (j>11) {
                    //break;
                    // printf("Re=%f \trad=%f\n",Re2,x);
                }
                
            }
            //printf("Re=%f\n",Re2);
            
            if (a_pb1 < 2.25*mean_path(x)) {
                vr0=v_r0(x,a_pb1);
                vr1=v_r0(x+0.5*step,a_pb1);
                vr2=v_r0(x+step,a_pb1);
            }
            else if (Re2<=1.0 && 1){
                vr0=v_r1(x,a_pb1);
                vr1=v_r1(x+0.5*step,a_pb1);
                vr2=v_r1(x+step,a_pb1);
            }
            else if (Re2 < 800.0 && Re2>1.0 && 1 ) {
                vr0=v_r2(x,a_pb1)[0];
                vr1=v_r2(x+0.5*step,a_pb1)[0];
                vr2=v_r2(x+step,a_pb1)[0];
            }
            else if (Re2 >800.0 && 1 ){
                vr0=v_r3(x,a_pb1)[0];
                vr1=v_r3(x+0.5*step,a_pb1)[0];
                vr2=v_r3(x+step,a_pb1)[0];
            }
            if(fabs(vr0-vr2)>0.01*fabs(vr0)) {
                //if (0) {
                step=step*0.8;
                //step=step*0.5;
                if (fabs(step) < 0.0001) step=-0.0001;
                //printf("change step to %1.12f\t vr accuracy=%f\n",step,fabs(vr0-vr2)/fabs(vr0));
                //printf("change step to %1.12f\n",step);
            }
            if (a_pb1 < 2.25*mean_path(x) && 1 ) {
                vr0=v_r0(x,a_pb1);
                vr1=v_r0(x+0.5*step,a_pb1);
                vr2=v_r0(x+step,a_pb1);
                tau=tau_fric0(x,a_pb1);
            }
            else if (Re2<=1.0 && 1){
                vr0=v_r1(x,a_pb1);
                vr1=v_r1(x+0.5*step,a_pb1);
                vr2=v_r1(x+step,a_pb1);
                tau=tau_fric0(x,a_pb1);
            }
            else if (Re2 < 800.0 && Re2>1.0 || 0 ) {
                vr0=v_r2(x,a_pb1)[0];
                vr1=v_r2(x+0.5*step,a_pb1)[0];
                vr2=v_r2(x+step,a_pb1)[0];
                tau=v_r2(x,a_pb1)[1];
            }
            else if (Re2 >800.0 && 1 ){
                vr0=v_r3(x,a_pb1)[0];
                vr1=v_r3(x+0.5*step,a_pb1)[0];
                vr2=v_r3(x+step,a_pb1)[0];
                tau=v_r3(x,a_pb1)[1];
            }
            tau_temp=tau;
            k1=1.0/vr0;
            k2=1.0/vr1;
            k3=1.0/vr1;
            k4=1.0/vr2;
            y=y+step*(k1+2*k2+2*k3+k4)/6.0;
            fprintf(fp_vr, "%f\t%f\n",x,vr0);
            //fprintf(fp_drt, "%f\t%f\t%f\t%f\n",x,-1.0*sum1*AU_km/yr_sec,a_pb1,coag_eff);
            fprintf(fp_drt, "%f\t%f\n",x,-1.0*sum1*AU_km/yr_sec);
            fprintf(fp_size,"%f\t%f\n",x,a_pb1);
            if (1) {
                //printf("r=%0.20f\t tau=%f\t f_tau=%f\t t_drift=%f\t vr=%f\tpeb_size=%f\t Step=%0.20f\n",x,tau,1.0/(tau+1.0/tau),43.9*(tau+1.0/tau)*pow(acc_rate,-0.4)*pow(alpha,0.2)*pow(x,1.4), vr0,0.108/(tau+1.0/tau)*pow(acc_rate,0.4)*pow(alpha,-0.2)*pow(x,-0.4),step);
            }
            sum1=sum1+step*k2;
            x+=step;
            if (a_pb1>9.0/4.0*mean_path(x)) {
                l++;
                if(l==1) x_stop=x;
                coag_eff=coag_eff*exp((x-x_stop)/0.001);
                coag_eff=0.000;
                
            }
            vol_plus=-1.0*M_PI*a_pb1*a_pb1*sqrt(vr0*vr0+0.25*tau*vr0*tau*vr0)*step*(k1+2*k2+2*k3+k4)/6.0*AU_km*100000.0;
            if (0) vol_plus=0.0;
            a_pb2=pow(((vol_plus*coag_eff*density(x)/rho_peb0+4.0/3.0*M_PI*a_pb1*a_pb1*a_pb1)*3.0/4.0/M_PI),0.33333333333333333);
            /*if (i%100==0) {
             printf("%0.20f\t peb_size=%f\t Step=%0.20f\n",x,a_pb2,step);
             }*/
            //printf("end \n");
        }
        else{
            k++;
            if(k==1) x_cut=x;
            y=y+step/vr0;
            x+=step;
            fprintf(fp_vr, "%f\t%f\n",x_cut,vr0);
            //fprintf(fp_drt, "%f\t%f\t%f\t%f\n",x_cut,-1.0*sum1*AU_km/yr_sec,a_pb1,coag_eff);
            fprintf(fp_drt, "%f\t%f\t%f\t%f\n",x_cut,-1.0*sum1*AU_km/yr_sec,a_pb1,coag_eff);
            fprintf(fp_size,"%f\t%f\n",x_cut,a_pb1);
            
        }
        
    }
    fclose(fp_vr);
    fclose(fp_drt);
    fclose(fp_size);
    //printf("%transition=%f\n",x_stop);
    
    
    sum1=sum1*AU_km/yr_sec;
    
    //printf("%0.10f\n",sum1);
    //printf("Hello, World!\n");
    
    x0=r_out;x1=r_in;x=x0;y=0.0;
    step=-1.0*step0;
    sum1=0.0;
    while (x>x1) {
        if(fabs(v_r00(x,a_peb(x))-v_r00(x+step,a_peb(x)))>0.001*fabs(v_r00(x,a_peb(x)))) {
            //if (0) {
            step=step*0.5;
            if (fabs(step) < 0.000001) step=-0.000001;
            //printf("change step to %1.12f\t vr accuracy=%f\n",step,fabs(v_r00(x,a_peb(x))-v_r00(x+step,a_peb(x)))/fabs(v_r00(x,a_peb(x))));
        }
        k1=1.0/v_r00(x,a_peb(x));
        k2=1.0/v_r00(x+0.5*step,a_peb(x));
        k3=1.0/v_r00(x+0.5*step,a_peb(x));
        k4=1.0/v_r00(x+step,a_peb(x));
        y=y+step*(k1+2*k2+2*k3+k4)/6.0;
        sum1=sum1+step*k2;
        x+=step;
        if (i%100==0) {
            //printf("%f\r",(x0-x)/(x0-x1)*100.0);
        }
        
    }
    sum1=sum1*AU_km/yr_sec;
    printf("%0.10f\n", sum1);
    printf("%0.10f\n", tau_fric0(1.0,a_pb1));
    printf("%0.10f\n", v_K(0.01516));
    printf("%0.10f\n", yeta(0.01516));
    
    
    
    return 0;
}



int drift_t(PEBBLE *pp, double coag_eff)
{

    int i=0,j=0,k=0,l=0,ll=0;
    FILE *fp_vr,*fp_drt,*fp_size;
    fp_vr=fopen("drift_velocity", "w");
    fp_drt=fopen(output_time,"w");
    fp_size=fopen(output_size,"w");
    double x0,x1,x,x_cut,x_stop,y,Re1,Re2,vr0,vr1,vr2,a_pb1,a_pb2,tau,vol_plus,tmp1,tmp2,time_tot=0.0;;
    double k1,k2,k3,k4,step,sum1=0.0;
    x0=pp->rad[0];
    
    x1=r_in;x=x0;y=0.0;
    //coag_eff=EFF;
    step=-1.0*step0;
    a_pb2=pp->size[0];
    while (x>x1) {
        if(tau_temp <1e8 || 1)
        {
            j=0;
            a_pb1=a_pb2;
            Re1=Reynolds(x,v_r0(x,a_pb1),tau_fric0(x,a_pb1),a_pb1);
            Re2=0.0;
            if (Re1 >2.0) {
                Re2=Reynolds(x, v_r2(x,a_pb1)[0],v_r2(x,a_pb1)[1],a_pb1);
            }
            while (fabs(Re1-Re2)>0.001*fabs(Re1)){
                Re1=Re2;
                if (a_pb1 < 2.25*mean_path(x)) {
                    Re2=Reynolds(x, v_r0(x,a_pb1),tau_fric0(x,a_pb1),a_pb1);
                }
                else if (Re1 <= 1.0 && 1){
                    Re2=Reynolds(x, v_r1(x,a_pb1),tau_fric0(x,a_pb1),a_pb1);
                    
                }
                else if (Re1 <=800.0 && Re1>1.0 && 1 ) {
                    Re2=Reynolds(x, v_r2(x,a_pb1)[0],v_r2(x,a_pb1)[1],a_pb1);
                    
                }
                else if (Re1 > 800.0 && 1 ){
                    Re2=Reynolds(x, v_r3(x,a_pb1)[0],v_r3(x,a_pb1)[1],a_pb1);
                }
                j++;
                if (j>11) {
                }
                
            }
            
            if (a_pb1 < 2.25*mean_path(x)) {
                vr0=v_r0(x,a_pb1);
                vr1=v_r0(x+0.5*step,a_pb1);
                vr2=v_r0(x+step,a_pb1);
            }
            else if (Re2<=1.0 && 1){
                vr0=v_r1(x,a_pb1);
                vr1=v_r1(x+0.5*step,a_pb1);
                vr2=v_r1(x+step,a_pb1);
            }
            else if (Re2 < 800.0 && Re2>1.0 && 1 ) {
                vr0=v_r2(x,a_pb1)[0];
                vr1=v_r2(x+0.5*step,a_pb1)[0];
                vr2=v_r2(x+step,a_pb1)[0];
            }
            else if (Re2 >800.0 && 1 ){
                vr0=v_r3(x,a_pb1)[0];
                vr1=v_r3(x+0.5*step,a_pb1)[0];
                vr2=v_r3(x+step,a_pb1)[0];
            }
            if(fabs(vr0-vr2)>0.01*fabs(vr0)) {
                step=step*0.8;
                if (fabs(step) < 0.0001) step=-0.0001;
            }
            if (a_pb1 < 2.25*mean_path(x) && 1 ) {
                vr0=v_r0(x,a_pb1);
                vr1=v_r0(x+0.5*step,a_pb1);
                vr2=v_r0(x+step,a_pb1);
                tau=tau_fric0(x,a_pb1);
            }
            else if (Re2<=1.0 && 1){
                vr0=v_r1(x,a_pb1);
                vr1=v_r1(x+0.5*step,a_pb1);
                vr2=v_r1(x+step,a_pb1);
                tau=tau_fric0(x,a_pb1);
            }
            else if (Re2 < 800.0 && Re2>1.0 || 0 ) {
                vr0=v_r2(x,a_pb1)[0];
                vr1=v_r2(x+0.5*step,a_pb1)[0];
                vr2=v_r2(x+step,a_pb1)[0];
                tau=v_r2(x,a_pb1)[1];
            }
            else if (Re2 >800.0 && 1 ){
                vr0=v_r3(x,a_pb1)[0];
                vr1=v_r3(x+0.5*step,a_pb1)[0];
                vr2=v_r3(x+step,a_pb1)[0];
                tau=v_r3(x,a_pb1)[1];
            }
            tau_temp=tau;
            k1=1.0/vr0;
            k2=1.0/vr1;
            k3=1.0/vr1;
            k4=1.0/vr2;
            y=y+step*(k1+2*k2+2*k3+k4)/6.0;
	    x+=-vr0*dt/AU_km*yr_sec;
	    time_tot=time_tot+dt;
            fprintf(fp_vr, "%f\t%f\n",x,vr0);
            fprintf(fp_drt, "%f\t%f\n",x,-1.0*sum1*AU_km/yr_sec);
            fprintf(fp_size,"%f\t%f\n",x,a_pb1);
            if (a_pb1>9.0/4.0*mean_path(x)) {
              //  l++;
              //  if(l==1) x_stop=x;
              //  coag_eff=coag_eff*exp((x-x_stop)/0.001);
                coag_eff=0.000;
                
            }
            //vol_plus=-1.0*M_PI*a_pb1*a_pb1*sqrt(vr0*vr0+0.25*tau*vr0*tau*vr0)*step*(k1+2*k2+2*k3+k4)/6.0*AU_km*100000.0;
            vol_plus=1.0*M_PI*a_pb1*a_pb1*sqrt(vr0*vr0+0.25*tau*vr0*tau*vr0)*dt*yr_sec*AU_km;
	   // if (0) vol_plus=0.0;
            a_pb2=pow(((vol_plus*coag_eff*density(x)/rho_peb0+4.0/3.0*M_PI*a_pb1*a_pb1*a_pb1)*3.0/4.0/M_PI),0.33333333333333333);
            pp->vr[ll]=vr0;
	    pp->size[ll+1]=a_pb2;
	    pp->rad[ll+1]=x;
	    pp->time[ll+1]=time_tot;
	    pp->mass[ll+1]=pow(pp->size[ll+1]/pp->size[0],3)*pp->mass[0];
	    ll++;
        
    }
    
//    printf("%0.10f\n", sum1);
//    printf("%0.10f\n", tau_fric0(1.0,a_pb1));
//    printf("%0.10f\n", v_K(0.01516));
//    printf("%0.10f\n", yeta(0.01516));
    
    
    
    
}
       pp->max_step=ll-1;
       //printf("MAXSTEP %f\t%f\n",pp->mass[ll],pp->mass[ll-1]);

    fclose(fp_vr);
    fclose(fp_drt);
    fclose(fp_size);
    
    
    sum1=sum1*AU_km/yr_sec;

return 0;
}
