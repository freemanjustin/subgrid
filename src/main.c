
#include "reader.h"

int main(int argc,char **argv)
{
	e	*E;
	int	k;
	int	i,j;

	// malloc the work struct
	E = malloc(sizeof(e));
	if(E==NULL){
        printf("Malloc failed\n");
        exit(1);
    }

	// parse command line arguments
	if(argc < 3){
		printf("need an input xml file and an output filename\n");
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
		/*
    E->max_lon = -999.0;
    E->min_lon = 999.0;
    E->max_lat = -999.0;
    E->min_lat = 999.0;
    E->max_radius = -999.0;
    E->min_lat_xing = 999.0;
    E->min_lon_xing = 999.0;
    E->max_lat_xing = -999.0;
    E->max_lon_xing = -999.0;
		*/

    // read the reference grid information
    get_netcdf(E);


    // read the input xml file
	  get_params(E);

		// figure out how many data points are in our 3 day track
		float start_time = E->ens_juldates[0][0];
		float	end_time = start_time + 3.0;
		int	three_days = 0;
		for(i=0;i<E->track_length;i++){
			if(E->ens_juldates[0][i] < end_time) three_days++;
		}
		printf("three day track length = %d\n", three_days);

		// testing print the xml arrays to a text file
		for(i=0;i<E->n_ens;i++){
			fprintf(E->outFile,"#Ensemble number = %d", i);
			fprintf(E->outFile,"## validTime latitude longitude max_speed max_radius vortexParameters");
			for(j=0;j<three_days;j++){
				fprintf(E->outFile,"%f %f %f %f %f %f\n", E->ens_juldates[i][j], E->ens_lat[i][j], E->ens_lon[i][j],
			 																				E->ens_max_speed[i][j], E->ens_max_radius[i][j], E->ens_vortexParameters[i][j]);
			}
			fprintf(E->outFile,"\n");
		}
		fclose(E->outFile);

		/*
    printf("box params:\n");
    printf("\tmin_lon = %f\n", E->min_lon);
    printf("\tmax_lon = %f\n", E->max_lon);
    printf("\tmin_lat = %f\n", E->min_lat);
    printf("\tmax_lat = %f\n", E->max_lat);
    printf("\tmax_radius = %f\n", E->max_radius);

    printf("\nunset key\n");
    printf("set arrow from %f,%f to %f,%f nohead\n", E->min_lon, E->min_lat, E->min_lon, E->max_lat);
    printf("set arrow from %f,%f to %f,%f nohead\n", E->min_lon, E->min_lat, E->max_lon, E->min_lat);
    printf("set arrow from %f,%f to %f,%f nohead\n", E->max_lon, E->min_lat, E->max_lon, E->max_lat);
    printf("set arrow from %f,%f to %f,%f nohead\n",  E->min_lon, E->max_lat, E->max_lon, E->max_lat);
    printf("plot \"AustraliaContinentOutline.txt\" u 2:1 w l, \"%s\" u 3:2 w l, \"< echo '%f %f'\", \"< echo '%f %f'\"\n", E->output_file, E->max_lon_xing, E->min_lat_xing, E->min_lon_xing, E->max_lat_xing);
		*/
  	//fclose(E->outFile);

		printf("plot \"AustraliaContinentOutline.txt\" u 2:1 w l, \"%s\" u 3:2 w l\n", E->output_file);

  	return 0;
}
