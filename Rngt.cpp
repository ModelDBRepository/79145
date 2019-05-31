/* rngt - random and Gauss dist
Kirillov 1992 */

#include "stdafx.h"
#include <math.h>
#include <stdlib.h>
#include <time.h>

/*            uniform random number generator   */
double rn(){
	return(rand()/32767.);
	}


/*            Gauss with zero mean and std. dev. = 1.   */
double sign(double x){
	return( (x < 0.5) ? -1.0 : 1.0 );
	}
double gt(){
	double u,u0,y;
        u=rn();
        u0=rn();
        if(u < 0.9195444)
		return(2.4037577*(u0+u*0.8253393)-2.1140281);
        else if(u >= 0.9654871) {
		do
                	y=sqrt(4.4691147-2.*log(rn()));
                while (y*rn() > 2.1140281);
                return (y * sign(u0));
		}
        else if(u >= 0.9499907) {
		do
                	y=1.8403987+rn()*0.2736293;
while (0.3989423*exp(-0.5*y*y)-0.4432991+y*0.2096941 < rn()*0.0427026);
                return (y * sign(u0));
		}
	else if(u >= 0.9258523) {
		do
  	        	y=0.2897296+rn()*1.5506692;
while (0.3989423*exp(-0.5*y*y)-0.4432991+y*0.2096941 < rn()*0.0159745);
                return (y * sign(u0));
		}
        else {
		do
                	y=0.2897296*rn();
while (0.3989423*exp(-0.5*y*y)-0.3825446 < rn()*0.0163977);
                return (y * sign(u0));
		}
}
