#include "reader.h"

void get_netcdf(e *E){

  int	ncid;
  int varid;
	int retval;
	size_t attlen = 0;

  double  pos[2];


  int     i,j,t;
	int count;

  // read the target grid - this is the roms curvilinear grid
	// in this function we want to find the indexes that intersect
  // out tc tracks

	/* roms output data required

	double lon_rho(eta_rho, xi_rho) ;
		lon_rho:long_name = "longitude of RHO-points" ;
		lon_rho:units = "degree_east" ;
		lon_rho:standard_name = "longitude" ;
		lon_rho:field = "lon_rho, scalar" ;
	double lat_rho(eta_rho, xi_rho) ;
		lat_rho:long_name = "latitude of RHO-points" ;
		lat_rho:units = "degree_north" ;
		lat_rho:standard_name = "latitude" ;
		lat_rho:field = "lat_rho, scalar" ;
	*/
	// open the roms_input file
	// open the file
    if((retval = nc_open(E->roms_grid, NC_NOWRITE, &ncid)))
        fail("failed to open roms input file: %s error is %d\n", E->roms_grid, retval);

    // get the lat dimension sizes
    if((retval = nc_inq_dimid(ncid, "eta_rho", &varid)))
        fail("failed to get roms lat dimid: error is %d\n",retval);

    if((retval = nc_inq_dimlen(ncid,varid,&E->nEtaRho)))
        fail("failed to get roms lat dimid: error is %d\n",retval);

    //printf("xi_rho = %zu\n", E->nEtaRho);

    // get the lon dimension sizes
    if((retval = nc_inq_dimid(ncid, "xi_rho", &varid)))
        fail("failed to get roms lon_rho dimid: error is %d\n",retval);

    if((retval = nc_inq_dimlen(ncid,varid,&E->nXiRho)))
        fail("failed to read roms lon_rho data: error is %d\n",retval);

    //printf("eta_rho = %zu\n", E->nXiRho);

    // malloc room for the arrays
	E->lat_rho = malloc2d_double( E->nEtaRho, E->nXiRho);
	E->lon_rho = malloc2d_double( E->nEtaRho, E->nXiRho);

  nc_inq_varid(ncid, "lat_rho", &varid);
  if((retval = nc_get_var_double(ncid, varid, &E->lat_rho[0][0])))
	fail("failed to read roms lat_rho data: error is %d\n", retval);

		//printf("lat_rho[0][0] = %f\n", E->lat_rho[0][0]);

    nc_inq_varid(ncid, "lon_rho", &varid);
    if((retval = nc_get_var_double(ncid, varid, &E->lon_rho[0][0])))
		fail("failed to read roms lat_rho data: error is %d\n", retval);

	nc_close(ncid);


  // construct the kdtree for grid searching
  E->kd = kd_create(2);
  E->roms_index = malloc(E->nEtaRho*E->nXiRho*sizeof(grid_index));

  count = 0;
  for(i=0;i<E->nEtaRho;i++){
    for(j=0;j<E->nXiRho;j++){
      E->roms_index[count].i = i;
      E->roms_index[count].j = j;
      pos[0] = E->lon_rho[i][j];
      pos[1] = E->lat_rho[i][j];
      kd_insert(E->kd, pos, &E->roms_index[count]);
      count++;
    }
  }
}
