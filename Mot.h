// Mot.h - Structures oscillator, parameters, and integration definition
// Poject MOT
// Kazanovich June 2005

#include "basinc.h"

#define RANDOMIZE_ON
#define TRAJSAVE_ON

#define FOMEGA	"traj\\omega.tra"	// file for frequencies trajectories
#define FTETA	"traj\\teta.tra"	// file for phase trajectories
#define FAMP	"traj\\amp.tra"		// file for amplitudes trajectories
#define CFOMEGA	"traj\\omega_cont.tra"	// file for frequencies trajectories
#define CFTETA	"traj\\teta_cont.tra"	// file for phase trajectories
#define CFAMP	"traj\\amp_cont.tra"		// file for amplitudes trajectories

const int nrows = 30;	// size of the image
const int ncolumns = 60;	
const int nobj = 10;		// number of objects
const int noaf = 5;		// the number of objects in the attention focus

enum activity 
{
	active,		// working oscillator
	dead		// non-working oscillator
};	

enum object_type {distractor, target};
enum movement_type {without_intersection, with_intersection};

struct object		// circle on the plane
{
	enum object_type type;	// type of object 
	int cx, cy;				// coordinates of the center of the object
	int priority;			// layer number occupied by the object, = 1, 2, ...
};

struct image
{
	struct object obj[nobj];					// objects in the image
	int r;									// radius (half size) of an object
	double intens[nrows][ncolumns];				// intensities of pixels
	double saliency[nrows][ncolumns];			// saliency of the pixel
};

struct connections
{
	int ncon;			// the number of connections
	int source[4];		// list of osccillators influencing
						// the given oscillator
};

struct oscillator		// Osc description
{
	double omega0;		// natural frequency
	double omega;		// current frequency
	double teta;		// current phase
	double amp;			// amplitude
	enum activity state;// oscillator activity state 
	double noise;		// noise added to oscillator phases
};

struct network
{
	struct oscillator *osc;
};

struct parameters // Network parameters
{
	//Parameters of CO
	double camp;			// initial amplitude of the CO
	double comega;			// initial value of natural frequency
	double cteta;			// initial value of phase

	// Parameters of the network
	
	long n;					// n = nx*ny
	
	// Parameters of POs
	double amp;				// initial amplitude of POs

	// weihjts
	double COtoCOw;			// connection weight between COs
	double COtoCOw_expos;	// connection weight between COs during exposition
	double COtoPOw;			// connection weight from CO to POs
	double POtoCOw;			// connection weight from POs to the CO
	double POtoPOlocw;		// local connection weight
	double POtoPOcolw;		// connection weight between POs in the same column
};

struct integration	// Integration parameters
{
	double eps; 	// precision
	double h1;		// initial step
	double hmin;	// minimal step
	double dt;		// time interval for integration step

	double maxt;	// time segment for integration
	double expos;	// Initial exposition (objects are motionless)
	double probe;	// time for probing (objects are motionless)
	enum movement_type move_type;	// with/without intersection
	int movement_dt;// time interval between movements
	
	// Parameters of function g in CO's equation
	double ga1;		// segments of streght line y = ax + b
	double ga2;
	double gb2;
	double ga3;
	double gb3;
	double gs1;		// coordinates of segment ends
	double gs2;		// (s1,q1), (s2,q2) (s3,0)
	double gs3;
	double gq1;
	double gq2;


	// Parameters for eq. for amplitudes
	double beta1;	
	double beta2;
	double beta_CO;
	double gamma;
	double ksi;
	double eta;
	double dzeta;
	double gamma1;
	double dzeta1;

	// Parameters for eq. for natural frequencies
	double alpha;	

	// Parameters for oscillator state
	double resthresh;	// resonance threshold
	int nresmin;		// min normalization parameter in the eq for the CO
	double noise;		// gaussian noise amplitude
};

struct imageproc
{
	// Parameters to controle the strength connection from POs to CO
	double intenstarget;		// the value of intensity for a target
	double intensdistract;	// the value of intensity for a distractor
	double highsaliency;		// saliency for a lighted object
	double normalsaliency;	// saliency for a normal object
	double lowsaliency;		// saliency for a nonlighted object during exposition
};