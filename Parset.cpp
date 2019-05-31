// Parset.cpp - Network parameters and initial state of variables definition.
// Project MOT
// Kazanovich June 2005

#include "stdafx.h"
#include "mot.h"

extern struct parameters par;
extern struct integration integr;
extern struct imageproc improc;
extern struct image im;

double rn(void);
//=================== s/p Pardef ===================
// Network parameters definition

const double PI = 3.1415926535;

void Pardef()
{							
par.n = nrows*ncolumns;

// CO
//par.camp = 8.0;
par.camp = 10.0;			// initial amplitude of the CO
par.comega = 6.0;		// initial natural frequency of CO
par.cteta =  0.;		// initial phase of CO

// POs
par.amp = 2.0;			// initial amplitude of POs

// so it was par.COtoPOw = -6.;		// connection weights
// so it was par.POtoCOw = 2.5;			
par.COtoPOw = -7.;		// connection weights
//par.COtoPOw = -15.;
par.POtoCOw = 5;			
par.POtoPOlocw = 40.0;	// weight of local interaction
//par.COtoCOw = -3.6;		// weight of interaction between COs 
par.COtoCOw = -7.0;
par.COtoCOw_expos = -12.0;	// weight of interaction between COs during exposition
//par.COtoCOw_expos = -15.0;
par.POtoPOcolw = 40.0;	// weight of interaction betwee POs in the same column

// Image parameters
im.r = 3;

// Integration parameters
integr.eps = 0.000001;	// Runge-Kutta integration parameters
integr.h1 = 0.01;
integr.hmin = 1.e-15;
integr.dt = 0.1;

integr.maxt = 72.0;		// Time of system functioning
integr.expos = 6.0,		// Initial exposition without any changes of an image 
integr.probe = 5.0;		// Testing period
integr.move_type = without_intersection;	// Type of movement
integr.movement_dt = 5; // Time interval between movements
// Parameters of eq.

// Parameters of function g in CO's eq
integr.gs1 = 0.1;		
integr.gq1 = 1.0;
integr.gs2 = 0.2;
integr.gq2 = 0.6;
integr.gs3 = 2*PI;

integr.ga1 = integr.gq1/integr.gs1;
integr.ga2 = (integr.gq1 - integr.gq2)/(integr.gs1 - integr.gs2);
integr.gb2 = (integr.gq2*integr.gs1 - integr.gq1*integr.gs2)/
			(integr.gs1 - integr.gs2);
integr.ga3 = integr.gq2/(integr.gs2 - integr.gs3);
integr.gb3 = - integr.gq2*integr.gs3/(integr.gs2 - integr.gs3);

// parameters for amplitude equation
// max amplitude = gamma(1 + dzeta)
integr.beta1 = 1.0;				
integr.beta2 = 1.0;		// !?! 2.0
integr.beta_CO = 1.0;
integr.gamma = 10.0; 
integr.ksi = 0.9;
integr.eta = 0.02;	// 0.015 !?!
integr.dzeta = 0.1;
integr.gamma1 = 1.7;	
integr.dzeta1 = 1.0;

// parameter for natural frequency equation
integr.alpha = 2.0;

// Resonance threshold
integr.resthresh = 0.8*integr.gamma*(1 + integr.dzeta);
// min normalization parameter in the eq for the CO
integr.nresmin = (im.r*2 + 1)*(im.r*2 + 1);		
// parameters for duration of activity states
integr.noise = 3.0;

// Parameters to controle the strength connection from POs to CO
improc.intenstarget = 5.0;	// the value of intensity for a target
improc.intensdistract = 5.0;// the value of intensity for a distractor
//improc.highsaliency = 5.0;
improc.highsaliency = 5.0;
improc.lowsaliency = 0.2;
improc.normalsaliency = 1.0;
}


