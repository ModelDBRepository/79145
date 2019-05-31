// Eqintegr.cpp - A step in Runge_kutta integration 
// Project MOT
// Kazanovich  June 2005

#include "stdafx.h"
#include "mot.h"
#include <iomanip.h>

//***** Subroutines *****

void My_error(CString);
void derivs(double y[], double dydx[]);
void odeint(double ystart[],int nvar,double x1,double x2,double eps,
		 double h1,double hmin,int *nok,int *nbad,
		 void (*derivs)(double [],double []),
		 void (*rkqs)(double [],double [],int,double *,double,
			 double,double [],double *,double *,
			 void (*)(double[],double[])));
void rkqs(double y[],double dydx[],int n,double *x,double htry,
	 double eps,double yscal[],double *hdid,double *hnext,
	 void (*derivs)(double[],double[]));
double gt(void);

//***** Externakl var. *****

extern int it;
extern struct network net[noaf];
extern struct connections *connec;
extern struct parameters par;
extern struct integration integr;
extern struct imageproc improc;
extern struct image im;

const double pi = 3.1415926535;
const double two_pi = 2.*pi;

// Transformation of phases to the interval (-pi, pi)
double r(double z)
{
	double sgn = (z >= 0)? 1. : -1.;
	double x = fabs(z);
	double y = x - ((int)(x/two_pi))*two_pi;
	y = y*sgn;
	if (y > pi) y = y - two_pi;
	if (y < -pi) y = y + two_pi;
	return y;
}

// Interaction function - influence of POs on the CO
double g(double x)
{
//	return sin(x);

	double y = r(x);
	double sgn = (y > 0.0)? 1.0 : -1.0;
	y = fabs(y);
	double z = 0.0;
	if (y < integr.gs1)
	{	z = integr.ga1*y;}
	else
	{
		if (y < integr.gs2)
		{	z = integr.ga2*y + integr.gb2;}
		else
		{	z = integr.ga3*y + integr.gb3;}
	}

	return sgn*z; 
}

// Interaction function - influence of CO on POs
const double ymax = pi/15.0;
const double ymax2 = 2*ymax;
const double lambda = 1.0/ymax;
const double freemem = lambda*ymax2;
/*
double g1(double x)
{
	return sin(x);
}
*/

double g1(double x)
{
	double y = r(x);
	double sgn = (y > 0)? 1.0 : -1.0;
	y = fabs(y);
	double z = lambda*y*exp(-lambda*y + 1.0);
	return sgn*z;
}
/*
double g1(double x)
{
	double z;
	double y = r(x);
	double sgn = (y > 0)? 1.0 : -1.0;
	y = fabs(y);
	if (y < ymax)
	{	z = lambda*y; }
	else
	{
		if (y < ymax2)
		{	z = freemem - lambda*y; }
		else
		{
			z = 0.0;
		}
	}

	return sgn*z;
}
*/

// Interaction function between COs
double g2(double x)
{
	double y = r(x);
	double sgn = (y > 0)? 1.0 : -1.0;
	y = fabs(y);
	double z = lambda*y*exp(-lambda*y + 1.0);
	return sgn*z;
}


// Function for the dynamics of amplitudes
double f(double x)
{
	x = cos(x);
	x = (x > 0)? x*x : 0;
	
	double y = (x - integr.ksi)/integr.eta;
	if (y > 20) return 1 + integr.dzeta;
	if (y < -20) return integr.dzeta;
	double u = exp(y);
	return u/(1 + u) + integr.dzeta;
}
//==================== s/p Step ============
void Step()
{
	int oaf;
	long n = par.n;
	long n1 = 2*(n + 1) + 1;
	long n2 = n1*noaf;
	static double *y_0 = new double[n2 + 1];	// pointer to integration array
	double *y;										// current pointer to integration array
	int nok, nbad;
	int i;

//***** Check of mamory allocation for integration array *****

	if (!y_0) My_error("Not enough memory for integration array");
	

//***** Filling integration array *****

	y = y_0; y++;
	for (oaf = 0; oaf < noaf; oaf++)
	{
		for (i = 0; i <= n; i++)
		{	*y++ = net[oaf].osc[i].teta;}

		for (i = 0; i <= n; i++)
		{	*y++ = net[oaf].osc[i].amp;}

		*y++ = net[oaf].osc[0].omega0;
	}

// ***** Adding noise *****

	for (oaf = 0; oaf < noaf; oaf++)
	{
		for (i = 0; i <= n; i++)
		{	net[oaf].osc[i].noise = integr.noise*gt(); }
	}

//***** Integration *****

	odeint(y_0, n2, it*integr.dt, (it + 1)*integr.dt,
		 integr.eps, integr.h1, integr.hmin,
		 &nok, &nbad, derivs, rkqs);

//***** Extracting network variables from the integration array *****

	y = y_0; y++;
	for (oaf = 0; oaf < noaf; oaf++)
	{
		for (i = 0; i <= n; i++)
		{	net[oaf].osc[i].teta = *y++;}

		for (i = 0; i <= n; i++)
		{	net[oaf].osc[i].amp = *y++; }

		net[oaf].osc[0].omega0 = *y++;
	}

}

//======================= s/p derivs ==============
// Computation of the RHS of equations

void derivs(double y[], double dydx[])
{

int oaf;
long n = par.n;
long n1 = 2*(n + 1) + 1;

int i, j;

double *teta[noaf];
double *amp[noaf];
double *comega0[noaf];

double *dteta_dt[noaf];
double *damp_dt[noaf];
double *dcomega0_dt[noaf];

// Assigning parameters different during exposition and normal work
double COtoCOw = par.COtoCOw; 
double beta2 = integr.beta2;
if (it*integr.dt <= integr.expos)
{
	COtoCOw = par.COtoCOw_expos;
}

//===== Array addresses =====

teta[0] = &y[1];
amp[0] = &y[n + 2];
comega0[0] = &y[n + n + 3];

dteta_dt[0] = &dydx[1];
damp_dt[0] = &dydx[n + 2];
dcomega0_dt[0] = &dydx[n + n + 3];

for (oaf = 1; oaf < noaf; oaf++)
{
	teta[oaf] = teta[0] + oaf*n1;
	amp[oaf] = amp[0] + oaf*n1;
	comega0[oaf] = comega0[0] +oaf*n1 ;

	dteta_dt[oaf] = dteta_dt[0] + oaf*n1;
	damp_dt[oaf] = damp_dt[0] + oaf*n1;
	dcomega0_dt[oaf] = dcomega0_dt[0] + oaf*n1;
}

for (oaf = 0; oaf < noaf; oaf++)
{
//=====	Equations for phases =====


	// POs

	for (i = 1; i <= n; i++)
	{	
		dteta_dt[oaf][i] = net[oaf].osc[i].omega = 0;
		if (net[oaf].osc[i].state == active) 
		{
			// Local interaction

			for (j = 0; j < connec[i].ncon; j++)
			{
				dteta_dt[oaf][i] += amp[oaf][connec[i].source[j]]*
					sin(teta[oaf][connec[i].source[j]] - teta[oaf][i]); 
			}
			dteta_dt[oaf][i] *= par.POtoPOlocw;
		
			// Interaction with the CO
			dteta_dt[oaf][i] += two_pi*net[oaf].osc[i].omega0;
			dteta_dt[oaf][i] += par.COtoPOw*amp[oaf][0]*g1(teta[oaf][0] - teta[oaf][i]);
			dteta_dt[oaf][i] += net[oaf].osc[i].noise;

			// Interaction with POs of the same column
			
			double sum = 0.;
			for (int oafcount = 0; oafcount < noaf; oafcount++)
			{	sum += amp[oaf][i]*sin(teta[oafcount][i] - teta[oaf][i]); }
			sum = sum*par.POtoPOcolw/noaf;
			dteta_dt[oaf][i] += sum;
		}
		net[oaf].osc[i].omega = dteta_dt[oaf][i];
	}

	// CO


	// Computation of the number of resonance oscillators
	int nres = 0;	// the number of resonant oscillators
	for (i = 1; i <= n; i++)
	{
		if (net[oaf].osc[i].amp > integr.resthresh)
		{	nres++;}
	}
	if (nres < integr.nresmin) nres = integr.nresmin;

	// Interaction with POs
	double sum = 0;
	int column = 0, row = 0;
	for (i = 1; i <= n; i++)
	{	
		if (net[oaf].osc[i].state == active) 
		{	
			sum += im.saliency[row][column]*amp[oaf][i]*g(teta[oaf][i] - teta[oaf][0]);
		}
		column++;
		if (column == ncolumns)
		{
			column = 0;
			row++;
		}
	}
	sum /= nres; 
	sum *= par.POtoCOw;

	// Interaction with COs
	double sum1 = 0.;
	for (int oafcount = 0; oafcount < noaf; oafcount++)
	{	
		if (oafcount != oaf)
		{	sum1 += amp[oafcount][0]*g2(teta[oafcount][0] - teta[oaf][0]); }
	}
	sum1 *= COtoCOw;	
	
	
	dteta_dt[oaf][0] = two_pi*comega0[oaf][0] + sum + sum1;
	net[oaf].osc[0].omega = dteta_dt[oaf][0];
	//afxDump << "oaf = " << oaf << "   comega = " << net[oaf].osc[0].omega << "\n";

	//*****	Equation for natural frequencies	*****

	dcomega0_dt[oaf][0] = integr.alpha*(dteta_dt[oaf][0] - two_pi*comega0[oaf][0]);

	//*****	Equations for amplitudes *****

	damp_dt[oaf][0] = 0.0;
	
	for (oafcount = 0; oafcount < noaf; oafcount++)
	{
		if (oafcount != oaf)
		{
			damp_dt[oaf][0] += f(teta[oafcount][0] - teta[oaf][0]);
		}
	}
	if (damp_dt[oaf][0] > 1.0) damp_dt[oaf][0] = 1.0;	// Limit on the value
			// for the case of tripple conjunction
	damp_dt[oaf][0] *= integr.gamma1;	
	damp_dt[oaf][0] += integr.dzeta1 - amp[oaf][0];
	damp_dt[oaf][0] *= integr.beta_CO;

	for (i = 1; i <= n; i++)
	{
		damp_dt[oaf][i] = 0;
		if (net[oaf].osc[i].state == active) 
		{	
			damp_dt[oaf][i] = - amp[oaf][i];
			damp_dt[oaf][i] += integr.gamma*f(teta[oaf][0] - teta[oaf][i]);
			if (damp_dt[oaf][i] >= 0)
			{	damp_dt[oaf][i] *= integr.beta1; }
			else
			{	damp_dt[oaf][i] *= beta2; }
		}
	}
}
}


