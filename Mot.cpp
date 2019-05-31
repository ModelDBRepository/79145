// Mot.cpp : Multiple Object Tracking
// Project MOT
// Kazanovich June 2005

#include "StdAfx.h"
#include "mot.h"

//***** Subprograms *****

void My_error(CString);
double rn();

//***** External variables *****

extern int it;
struct network net[noaf];
struct connections *connec;
struct parameters par;
struct integration integr;
struct image im;
struct imageproc improc;

const double pi = 3.1415926535;

ofstream out1, out2, out3, out4;

//================ s/p Netalloc =================
// Allocation of memory for the network

void Netalloc(void)
{
	int oaf;
	for (oaf = 0; oaf < noaf; oaf++)
	{
		net[oaf].osc = new oscillator[par.n + 1];
		if (!net[oaf].osc) My_error("Not enough memory for oscillators1");
	}

	
	connec = new connections[par.n + 1];
	if (!connec) My_error("Not enough memory for connections");
}

//===================== s/p Netinit ================
// Network initialization

void Netinit()
{
int i, j, k, oaf;
long n = par.n;

	for (oaf = 0; oaf < noaf; oaf++)
	{
		//***** Initialization of CO

		net[oaf].osc[0].amp = par.camp;
		net[oaf].osc[0].omega0 = par.comega;
		net[oaf].osc[0].omega = net[oaf].osc[0].omega0;
		net[oaf].osc[0].teta = pi*rn(); 
		net[oaf].osc[0].state = active;

		//***** Initialization of POs
	
		//----- Initialization of natural frequencies
		k = 1;
		for (i = 0; i < nrows; i++)
			for (j = 0; j < ncolumns; j++)
			{
				double freq = im.intens[i][j];
				net[oaf].osc[k].state = (freq != 0)? active : dead;
				net[oaf].osc[k].amp = (net[oaf].osc[k].state == active)? par.amp : 0;			
				net[oaf].osc[k].omega0 = freq;
				net[oaf].osc[k].omega = net[oaf].osc[k].omega0;
				k++;
			}
	
		//----- Initialization of phases	
		k = 1;
		for (i = 0; i < nrows; i++)
			for (j = 0; j < ncolumns; j++)
			{
				net[oaf].osc[k].teta = 0.0;
				if (net[oaf].osc[k].state == active)
				{	net[oaf].osc[k].teta = pi*rn(); }
				k++;
			}
	}
}

//===================== s/p Netreinit ================
// Network reinitialization after image changing

void Netreinit()
{
int i, j, k, oaf;
long n = par.n;

	for (oaf = 0; oaf < noaf; oaf++)
	{
		//***** Reinitialization of POs
	
		k = 1;	// index of a PO
		for (i = 0; i < nrows; i++)
			for (j = 0; j < ncolumns; j++)
			{
				double freq = im.intens[i][j];	
				net[oaf].osc[k].state = (freq != 0)? active : dead;
				if (net[oaf].osc[k].state == dead)
				{
					net[oaf].osc[k].amp = 0;
					net[oaf].osc[k].teta = 0.0;	
				}
				if (net[oaf].osc[k].state == active && net[oaf].osc[k].amp == 0) 
				{	
					net[oaf].osc[k].amp = par.amp;
					net[oaf].osc[k].teta = pi*rn(); 
				}
				net[oaf].osc[k].omega0 = freq;
				k++;
			}
	}
}
//================ s/p Trajopen ===========
// Opening files for trajectories

#define FOMEGA	"traj\\omega.tra"	// file for frequencies trajectories
#define FTETA	"traj\\teta.tra"	// file for phase trajectories
#define FAMP	"traj\\amp.tra"		// file for amplitudes trajectories
#define FOBJ	"traj\\obj.tra"		// file for object coordinates

void Trajopen()
{
	CString str = "Can't open file ", str1;
	

	out1.open(FOMEGA);
	str1 = str + FOMEGA;
	if (!out1) My_error(str1);
	
	out2.open(FTETA);
	str1 = str + FTETA;
	if (!out2) My_error(str1);	
	
	out3.open(FAMP);
	str1 = str + FAMP;
	if (!out3) My_error(str1);	

	out4.open(FOBJ);
	str1 = str + FOBJ;
	if (!out4) My_error(str1);	

}
//================ s/p Trajsave =============
// Saving trajectories
double r(double x);	// defined in Eqintegr.cpp

void Trajsave()
{
int i, j, k, oaf;

	double time = it*integr.dt;

	out1.precision(4);
	out2.precision(4);
	out3.precision(4);

	out1 << "time = " << setw(7) << time << endl;
	out2 << "time = " << setw(7) << time << endl;
	//out3 << "time = " << setw(7) << time << endl;
	
	for (oaf = 0; oaf < noaf; oaf++)
	{
		out1 << oaf << "  " << net[oaf].osc[0].omega << endl;
		out2 << oaf << "  " << r(net[oaf].osc[0].teta) << endl;
		//out3 << oaf << "  " << net[oaf].osc[0].amp << endl;
		
		k = 1;
		for (i = 0; i < nrows; i++)
		{
			for (j = 0; j < ncolumns; j++)
			{
				out1 << setw(8) << net[oaf].osc[k].omega;
				out2 << setw(8) << r(net[oaf].osc[k].teta);
				out3 << setw(8) << net[oaf].osc[k].amp;
				k++;
			}
			out1 << endl;
			out2 << endl;
			out3 << endl;
		}
		//out1 << endl;
		//out2 << endl;
		out3 << endl;
	}

	
	// Saving taget coordinates
	//out4 << setw(7) << time;
	for (i = 0; i < nobj; i++)
	{					
		if (im.obj[i].type == target)
		{
			int x = im.obj[i].cx;
			int y = im.obj[i].cy;
			out4 << setw(5) << x << setw(5) << y;
		}
	}
	out4 << endl;
}
//==================== Trajclose ====================
//***** Closing files for trajectories

void Trajclose()
{
	out1.close();
	out2.close();
	out3.close();
	out4.close();
}

//==================== Neighbours =======================
// Computation of nearest neighbours of a point z 
// in the table nrows*ncolumns. 
// Absence of a neighbour is coded by -1.

void Neighbours(long z, long nrows, long ncolumns,
				long & left, long & right, 
				long & top, long & bottom)
{
	long z1 = z - 1;// enumaration starts from 1
	long r = z1%ncolumns;	// column
	long l = z1/ncolumns;	// row

left = right = top = bottom = -1;
if (r > 0) left = z - 1;
if (r < ncolumns - 1) right = z + 1;
if (l > 0) top = z - ncolumns;
if (l < nrows - 1) bottom = z + ncolumns;
}

//==================== Connec ============================
// Connections initialization. 
// Connection is active if the neighbour exists and is not dead

void Connec()
{
	int i;
	for (long z = 1; z <= par.n; z++)
	{
		long left, right, top, bottom;
		
		Neighbours(z, nrows, ncolumns, left, right, top, bottom);
		
		long nghb[4];
		nghb[0] = left;
		nghb[1] = right;
		nghb[2] = top;
		nghb[3] = bottom;
		int ncon = 0;
		for (i = 0; i < 4; i++)
		{	
			if (nghb[i] != -1 && net[0].osc[nghb[i]].state != dead) 
			{	
				connec[z].source[ncon] = nghb[i];
				ncon++;
			}
		}
		connec[z].ncon = ncon;
	}
}

