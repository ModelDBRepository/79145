#include "stdafx.h"
#include "mot.h"
#include <iomanip.h>

extern integration integr;

void My_error(CString);

void Shorttraj()
{
	CString str = "Can't open file ", str1;

	ifstream in;
	in.open(FAMP, ios::nocreate);
	str1 = str + FAMP;
	if (!in) My_error(str1);	

	ofstream out;
	out.open(FSAMP);
	str1 = str + FSAMP;
	if (!out) My_error(str1);
	
	const long maxlinelength = 50000;
	char line[maxlinelength + 1];
	int k = 0;
	while (in)
	{
		in.getline(line, maxlinelength);
		long linelength = strlen(line);
		if (linelength >= maxlinelength - 1) My_error("Line reserved in Shorttraj is too short");
		if (k%integr.traj_reduction == 0) {out << line << endl;}
		k++;
	}
}