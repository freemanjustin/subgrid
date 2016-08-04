#include "reader.h"

void get_grid_boundary(e *E){

  // set up an array of points which is the bounding polygon of the roms grid
  // we will use this to test if a track point is inside or outside the roms grid
  int i,j;
  int count;

  // malloc room for the bounding polygon
  E->polygon = malloc((2*E->nLonRho + 2*E->nLatRho)*sizeof(pts));
  E->ncoords = (2*E->nLonRho + 2*E->nLatRho);


  // start in the lower left
  // we move counter clockwise

  // bottom row
  count = 0;
  for(i=0;i<E->nLonRho;i++){
    E->polygon[count].x = E->lon_rho[i][0];
    E->polygon[count].y = E->lat_rho[i][0];
    //printf("%f %f\n", E->polygon[count].x, E->polygon[count].y);
    count++;
  }
  // right side
  for(i=0;i<E->nLatRho;i++){
    E->polygon[count].x = E->lon_rho[E->nLonRho-1][i];
    E->polygon[count].y = E->lat_rho[E->nLonRho-1][i];
    count++;
  }
  // top of grid (moving from right to left)
  for(i=E->nLonRho-1;i>=0;i--){
    E->polygon[count].x = E->lon_rho[i][E->nLatRho-1];
    E->polygon[count].y = E->lat_rho[i][E->nLatRho-1];
    count++;
  }
  // left side
  for(i=E->nLatRho-1;i>=0;i--){
    E->polygon[count].x = E->lon_rho[0][i];
    E->polygon[count].y = E->lat_rho[0][i];
    count++;
  }

  // check with gnuplot
  FILE  *poly;
  poly = fopen("bounding_poly.txt","w");
  for(i=0;i<count;i++){
    fprintf(poly,"%f %f\n", E->polygon[i].x, E->polygon[i].y);
  }
  fclose(poly);


}

void cut_grid(e *E){

  // for now we will ncks the input roms grid to produce a new subset grid
  // for the ensemble runs
  //printf("ncks -d xi_rho,%d,%d -d eta_rho,0,%d %s -O cut.nc\n", low, high, E->nLonRho-1, E->input_file);

}
