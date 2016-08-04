
#include "reader.h"

int main(int argc,char **argv)
{
	e	*E;
	pts	pt;
	int	q,r;
	int	i,j;
	int	idx_i, idx_j;
	int	min_j, max_j;
	int	count;
	double	dist, min_dist;

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
			//fprintf(E->outFile,"#Ensemble number = %d\n", i);
			//fprintf(E->outFile,"## validTime latitude longitude max_speed max_radius vortexParameters\n");
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
	    		fprintf(ini,"%f %f\n", E->ens_lon[i][j], E->ens_lat[i][j]);
			}
			fprintf(ini,"\n");
	  }
	  fclose(ini);

		// now, for each point iside the polygon find the closest grid point to the
		// min and max latitudes and longitudes from the track point swarm
		E->min_lon = 99999.0;
		E->max_lon = -99999.0;
		E->min_lat = 99999.0;
		E->max_lat = -99999.0;
		count = 0;
		for(i=0;i<E->n_ens;i++){
			for(j=0;j<E->three_days;j++){
				if(E->is_inside[i][j] == TRUE){
					 count++;
	    		 if(E->ens_lon[i][j] < E->min_lon) E->min_lon = E->ens_lon[i][j];
					 if(E->ens_lon[i][j] > E->max_lon) E->max_lon = E->ens_lon[i][j];
					 if(E->ens_lat[i][j] < E->min_lat) E->min_lat = E->ens_lat[i][j];
					 if(E->ens_lat[i][j] > E->max_lat) E->max_lat = E->ens_lat[i][j];
				}
			}
	  }
		printf("min lon = %f, max lon = %f\n", E->min_lon, E->max_lon);
		printf("min lat = %f, max lat = %f\n", E->min_lat, E->max_lat);
		E->number_of_points_inside = count;
		printf("number of points inside = %d\n", E->number_of_points_inside);
		E->i_index = malloc(E->number_of_points_inside*sizeof(int));
		E->j_index = malloc(E->number_of_points_inside*sizeof(int));



		// use the kdtree to find the closes roms_grid point to the current track point
		// store the index of this point
		count = 0;
		min_j = 32768;
		max_j = 0;
		double	pos[2];
		grid_index		*idx;
		for(i=0;i<E->n_ens;i++){
			for(j=0;j<E->three_days;j++){
				if(E->is_inside[i][j] == TRUE){
				 	pos[0] = E->ens_lon[i][j];
					pos[1] = E->ens_lat[i][j];
					E->set = kd_nearest_range(E->kd, pos, 0.1);	// make the .05 dynamic
					//printf("##point %d (of %d) ## search around track point %f, %f returned %d roms grid items\n", count,E->number_of_points_inside,pos[0], pos[1], kd_res_size(E->set));
					while( !kd_res_end( E->set ) ) {
				    // get the data and position of the current result item
				    idx = (grid_index*)kd_res_item( E->set, pos );

				    // compute the distance of the current result from the pt
				    //dist = sqrt( dist_sq( pt, pos, 2 ) );

				    // print out the retrieved data
				    //printf( "pos at (%.3f, %.3f) idx = %d %d\n", pos[0], pos[1], idx->i, idx->j );


						// store the smallest index
						if(idx->j < min_j) min_j = idx->j;
						// store the biggest index
						if(idx->j > max_j) max_j = idx->j;

				    // go to the next entry
				    kd_res_next( E->set );
				}
				count++;
		 	 //printf("count = %d (outof %d total)\n", count, E->number_of_points_inside);
 			}
		}
	}



		printf("min index = %d, max index = %d\n", min_j, max_j);

		printf("ncks -d xi_rho,%d,%d -d eta_rho,0,%d %s -O cut.nc\n", min_j, max_j, E->nLonRho-1, E->roms_grid);


		printf("plot \"AustraliaContinentOutline.txt\" u 2:1 w l, \"%s\" u 3:2 w l\n", E->output_file);

		kd_res_free(E->set);
		kd_free(E->kd);

  	return 0;
}
