// standard headers
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <stdarg.h>

// libxml2 headers
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

#include <netcdf.h>
#include "kdtree.h"
#include "jutil.h"

// macros
#define	TRUE 1
#define FALSE 0

#define INSIDE	1
#define OUTSIDE 0

#define max(x,y) (((x) > (y)) ? (x) : (y))
#define min(x,y) (((x) < (y)) ? (x) : (y))

#define fail(...) grid_fail(__LINE__,__func__,__FILE__,__VA_ARGS__)

typedef struct{
	int	i;
	int	j;
}grid_index;

typedef struct{

	// command line args
	char			*output_file;

	// xmlIO variables
	char			*input_xml;
	char			*roms_grid;

  int             nEnsemble;
	int							track_count;
  FILE            *outFile;

	// time parsing variables
	int month;
	int day;
	int year;
	int hour;
	int minute;
	double sec;
	double juldate;

	int		n_ens;
	int		track_length;
	int		three_days;
	int		number_of_points_inside;

	double	**ens_juldates;
	float	**ens_lat;
	float	**ens_lon;
	float	**ens_max_speed;
	float	**ens_max_radius;
	float	**ens_vortexParameters;
	int		**is_inside;

	int		*i_index;
	int		*j_index;

	float		max_lat;
	float		max_lon;
	float		min_lon;
	float		min_lat;

	// roms grid variables
	size_t	nEtaRho;
	size_t	nXiRho;
	double	**lat_rho;
	double	**lon_rho;

	pts			*polygon;
	int			ncoords;
	double	average_distance_per_index;
	double			padding;

	// kdtree stuff
	grid_index	*roms_index;
	void *kd;
	struct kdres *set;

}e;


// fail.c
void grid_fail( const int, const char*, const char*, const char*, ... );

// netcdfIO.c
void get_netcdf(e *E);

// xmlIO.c
void get_params(e*);

// prototypes
void _parseInputFile_header (e *E, xmlDocPtr doc, xmlNodePtr cur);
void _parseInputFile_generatingApplication (e *E, xmlDocPtr doc, xmlNodePtr cur);
void _parseInputFile_ensemble (e *E, xmlDocPtr doc, xmlNodePtr cur);
void _parseInputFile_params (e*, xmlDocPtr, xmlNodePtr);
void _parseInputFile_data (e *E, xmlDocPtr doc, xmlNodePtr cur);
void _parseInputFile_disturbance (e *E, xmlDocPtr doc, xmlNodePtr cur);
void _parseInputFile_fix(e *E, xmlDocPtr doc, xmlNodePtr cur);

void _parseInputFile_vortexParameters(e *E, xmlDocPtr doc, xmlNodePtr cur);
void _parseInputFile_cycloneData(e *E, xmlDocPtr doc, xmlNodePtr cur);
void _parseInputFile_maximumWind(e *E, xmlDocPtr doc, xmlNodePtr cur);

void _parseInputFile_data_first_pass (e *E, xmlDocPtr doc, xmlNodePtr cur);
void _parseInputFile_disturbance_first_pass (e *E, xmlDocPtr doc, xmlNodePtr cur);
void _parseInputFile_fix_first_pass(e *E, xmlDocPtr doc, xmlNodePtr cur);

// grid operations
void cut_grid(e *E);
void get_grid_boundary(e *E);
