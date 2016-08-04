
#include "reader.h"

int main(int argc,char **argv)
{
	e	*E;
	pts	pt;
	int	k;
	int	i,j;

	// malloc the work struct
	E = malloc(sizeof(e));
	if(E==NULL){
        printf("Malloc failed\n");
        exit(1);
    }

	// parse command line arguments
	if(argc < 4){
		printf("need an input TCxml file, a reference roms grid and an output (netcdf) filename\n");
        exit(1);
	}
	else{
		get_command_line_arg_as_string(&E->input_xml, argv[1]);
		get_command_line_arg_as_string(&E->roms_grid, argv[2]);
		get_command_line_arg_as_string(&E->output_file, argv[3]);
	}

    // initialize struct members
    E->outFile = fopen(E->output_file, "w");
    if(E->outFile == NULL){
        printf("could't create output file\n");
        exit(1);
    }
    E->nEnsemble = 0;


    // read the reference grid information
    get_netcdf(E);

    // read the input xml file
	  get_params(E);

		// figure out how many data points are in our 3 day track
		float start_time = E->ens_juldates[0][0];
		float	end_time = start_time + 3.0;
		E->three_days = 0;
		for(i=0;i<E->track_length;i++){
			if(E->ens_juldates[0][i] < end_time) E->three_days++;
		}
		printf("three day track length = %d\n", E->three_days);

		// testing print the xml arrays to a text file
		for(i=0;i<E->n_ens;i++){
			fprintf(E->outFile,"#Ensemble number = %d", i);
			fprintf(E->outFile,"## validTime latitude longitude max_speed max_radius vortexParameters");
			for(j=0;j<E->three_days;j++){
				fprintf(E->outFile,"%f %f %f %f %f %f\n", E->ens_juldates[i][j], E->ens_lat[i][j], E->ens_lon[i][j],
			 																				E->ens_max_speed[i][j], E->ens_max_radius[i][j], E->ens_vortexParameters[i][j]);
			}
			fprintf(E->outFile,"\n");
		}
		fclose(E->outFile);

		// now have the three day track lenths, lat and lon positions of each
		// also have the roms grid reference
		// extract the boundary points of the roms grid
		// we will use this as the first pass to see if a track lat, lon point is inside or outside the grid
		get_grid_boundary(E);

		// find all of the 3 day track points which are inside our polygon
		E->is_inside = malloc2d_int(E->n_ens, E->three_days);
		for(i=0;i<E->n_ens;i++){
			for(j=0;j<E->three_days;j++){
				//pnpoly(pts *polygon, int npol, pts p)
				pt.x = E->ens_lon[i][j];
				pt.y = E->ens_lat[i][j];
				E->is_inside[i][j] = pnpoly(E->polygon, E->ncoords, pt);
			}
		}
		// check with gnuplot
	  FILE  *ini;
	  ini = fopen("inside.txt","w");
		for(i=0;i<E->n_ens;i++){
			for(j=0;j<E->three_days;j++){
				if(E->is_inside[i][j] == TRUE)
	    		fprintf(ini,"%f %f %d\n", E->ens_lon[i][j], E->ens_lat[i][j]);
			}
			fprintf(ini,"\n");
	  }
	  fclose(ini);

		// now, for each point iside the polygon find the closest grid point to the
		// min and max latitudes and longitudes from the track point swarm

		// once we have the min and max coords, find the source grid i,j indexes for these points.
		// for this case we only really need the i index


		printf("plot \"AustraliaContinentOutline.txt\" u 2:1 w l, \"%s\" u 3:2 w l\n", E->output_file);

  	return 0;
}
