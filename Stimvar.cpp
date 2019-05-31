// Stimvar.cpp : Changing the image in time - 
//                Randomly moving objects
// Project MOT
// Kazanovich June 2005

#include "StdAfx.h"
#include "mot.h"

double rn(void);

extern int it;
extern struct image im;
extern struct imageproc improc;
extern struct  integration integr;

enum movement {bad, good};
enum intersection {no_intersection, there_is_intersection};
enum boundary_crossing {no_crossing, there_is_crossing};

void Stimvar(double t)
{
	int i, j, k;
	int r1 = 2*im.r + 1;
	enum movement movement_left, movement_right, movement_up, movement_down;
	enum intersection intersec;
	enum boundary_crossing bcros;

	//***** Making movement *****

	for (k = 0; k < nobj; k++)
	{
		int cx = im.obj[k].cx;
		int cy = im.obj[k].cy;
		int cxl = cx - 1;
		int cxr = cx + 1;
		int cyu = cy - 1;
		int cyd = cy + 1;
		
			
		// Movement left
		intersec = no_intersection;
		for (j = 0; j < nobj; j++)
		{
			if (j != k && 
				fabs(cxl-im.obj[j].cx) <= r1 &&
				fabs(cy-im.obj[j].cy) <= r1)
			{
				intersec = there_is_intersection;
				break;
			}
		}
		
		bcros = no_crossing;			
		if (cxl <= im.r)
		{	bcros = there_is_crossing; }

		movement_left = bad;			
		switch (integr.move_type)
		{
			case without_intersection:
			{	
				if (intersec == no_intersection && bcros == no_crossing)
				{	movement_left = good; }
				break;
			}
			
			case with_intersection:
			{	
				if (bcros == no_crossing)
				{	movement_left = good; }
			}
		}
			
		// Movement right
		intersec = no_intersection;
		for (j = 0; j < nobj; j++)
		{
			if (j != k && 
				fabs(cxr-im.obj[j].cx) <= r1 &&
				fabs(cy-im.obj[j].cy) <= r1)
			{
				intersec = there_is_intersection;
				break;
			}
		}
		
		bcros = no_crossing;			
		if (cxr >= ncolumns - im.r)
		{	bcros = there_is_crossing; }
		movement_right = bad;			
		switch (integr.move_type)
		{
			case without_intersection:
			{	
				if (intersec == no_intersection && bcros == no_crossing)
				{	movement_right = good; }
				break;
			}
				
			case with_intersection:
			{	
				if (bcros == no_crossing)
				{	movement_right = good; }
			}
		}

		// Movement up
		intersec = no_intersection;
		for (j = 0; j < nobj; j++)
		{
			if (j != k && 
				fabs(cyu-im.obj[j].cy) <= r1 &&
				fabs(cx-im.obj[j].cx) <= r1)
			{
				intersec = there_is_intersection;
				break;
			}
		}
		bcros = no_crossing;			
		if (cyu <= im.r)
		{	bcros = there_is_crossing; }
		movement_up = bad;			
		switch (integr.move_type)
		{
			case without_intersection:
			{	
				if (intersec == no_intersection && bcros == no_crossing)
				{	movement_up = good; }
				break;
			}
			
			case with_intersection:
			{	
				if (bcros == no_crossing)
				{	movement_up = good; }
			}

		}
			
		// Movement down
		intersec = no_intersection;
		for (j = 0; j < nobj; j++)
		{
			if (j != k && 
				fabs(cyd-im.obj[j].cy) <= r1 &&
				fabs(cx-im.obj[j].cx) <= r1)
			{
				intersec = there_is_intersection;
				break;
			}
		}
			
		bcros = no_crossing;			
		if (cyd >= nrows - im.r)
		{	bcros = there_is_crossing; }
		movement_down = bad;			
		switch (integr.move_type)
		{
			case without_intersection:
			{	
				if (intersec == no_intersection && bcros == no_crossing)
				{	movement_down = good; }
				break;
			}
			
			case with_intersection:
			{	
				if (bcros == no_crossing)
				{	movement_down = good; }
			}
		}

		if (rn() < 0.5)	// horizontal movement
		{
			if (movement_left == good && movement_right == bad)
			{	im.obj[k].cx = cxl; }
		
			if (movement_left == bad && movement_right == good)
			{	im.obj[k].cx = cxr; }			
			if (movement_left == good && movement_right == good)
			{
				if (rn() < 0.5)
				{	im.obj[k].cx = cxl; }
				else
				{	im.obj[k].cx = cxr; }			
			}
		}
		else			// vertical movement
		{
			if (movement_up == good && movement_down == bad)
			{	im.obj[k].cy = cyu; }
		
			if (movement_up == bad && movement_down == good)
			{	im.obj[k].cy = cyd; }			

			if (movement_up == good && movement_down == good)
			{
				if (rn() < 0.5)
				{	im.obj[k].cy = cyu; }
				else
				{	im.obj[k].cy = cyd; }			
			}
		}
	}

	//***** Generation of pixel intensities *****

	for (i = 0; i < nrows; i++)
		for (j = 0; j < ncolumns; j++)
		{
			float intens = 0.0;
			float saliency = 0.0;
			int priority = 0;
			
			for (k = 0; k < nobj; k++)
			{
				int cx, cy;
				cx = im.obj[k].cx;
				cy = im.obj[k].cy;

				double distx = fabs(cx - j);
				double disty = fabs(cy - i);
				if (distx <= im.r && disty <= im.r &&
					priority < im.obj[k].priority)
				{
					intens = improc.intensdistract; 
					saliency = improc.normalsaliency;
					priority = im.obj[k].priority;
				}
			}
			im.intens[i][j] = intens;
			im.saliency[i][j] = saliency;
		}
}