// Stimgen.cpp : Generation of the image with 9 circles
// Project MOT
// 6 objects in the field 19 x 60
// Kazanovich June 2005

// DON'T FORGET! If you change Stimgen.cpp, change the parameters nrows, ncolumns, nobj



#include "StdAfx.h"
#include "Mot.h"

extern struct image im;
extern struct imageproc improc;

// 10 objects

const int objx0 = 10;
const int objy0 = 10;
const int objdx = 10;
const int objdy = 10;

void Stimgen()
{
	int i, j, k;

	//***** Initial parameters of the image *****

	for (k = 0; k < nobj; k++)
	{
		switch (k)
		{
			case 0:
			{
				im.obj[0].cx = objx0;	
				im.obj[0].cy = objy0;
				im.obj[0].type = target;
				im.obj[0].priority = 1;
				break;
			}
			case 1:
			{
				im.obj[1].cx = objx0 + objdx;
				im.obj[1].cy = objy0;
				im.obj[1].type = target;
				im.obj[1].priority = 2;
				break;
			}
			case 2:
			{
				im.obj[2].cx = objx0 + 2*objdx;
				im.obj[2].cy = objy0;
				im.obj[2].type = distractor;
				im.obj[2].priority = 3;
				break;
			}
			case 3:
			{
				im.obj[3].cx = objx0 + 3*objdx;
				im.obj[3].cy = objy0;
				im.obj[3].type = target;
				im.obj[3].priority = 4;
				break;
			}
			case 4:
			{
				im.obj[4].cx = objx0 + 4*objdx;
				im.obj[4].cy = objy0;
				im.obj[4].type = distractor;
				im.obj[4].priority = 5;
				break;
			}
			case 5:
			{
				im.obj[5].cx = objx0;
				im.obj[5].cy = objy0 + objdy;
				im.obj[5].type = distractor;
				im.obj[5].priority = 6;
				break;
			}
			case 6:
			{
				im.obj[6].cx = objx0 + objdx;
				im.obj[6].cy = objy0 + objdy;
				im.obj[6].type = target;
				im.obj[6].priority = 7;
				break;
			}
			case 7:
			{
				im.obj[7].cx = objx0 + 2*objdx;
				im.obj[7].cy = objy0 + objdy;
				im.obj[7].type = distractor;
				im.obj[7].priority = 8;
				break;
			}
			case 8:
			{
				im.obj[8].cx = objx0 + 3*objdx;
				im.obj[8].cy = objy0 + objdy;
				im.obj[8].type = distractor;
				im.obj[8].priority = 9;
				break;
			}
			case 9:
			{
				im.obj[9].cx = objx0 + 4*objdx;
				im.obj[9].cy = objy0 + objdy;
				im.obj[9].type = target;
				im.obj[9].priority = 10;
			}
		}
	}


//***** Generation of pixel intensities *****

	for (i = 0; i < nrows; i++)
		for (j = 0; j < ncolumns; j++)
		{
			double intens = 0.0;
			double saliency = 0.0;
			int priority = 0;
			
			for (k = 0; k < nobj; k++)
			{
				double distx = fabs(im.obj[k].cx - j);
				double disty = fabs(im.obj[k].cy - i);
				if (distx <= im.r && disty <= im.r &&
					priority < im.obj[k].priority)
				{
					if (im.obj[k].type == distractor)
					{	
						intens = improc.intensdistract; 
						saliency = improc.lowsaliency;
					}
					if (im.obj[k].type == target)
					{	
						intens = improc.intenstarget; 
						saliency = improc.highsaliency;
					}
					priority = im.obj[k].priority;
				}
			}
			im.intens[i][j] = intens;
			im.saliency[i][j] = saliency;
		}
}
