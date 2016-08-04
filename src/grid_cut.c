#include "reader.h"

void get_grid_boundary(e *E){

  // set up an array of points which is the bounding polygon of the roms grid
  // we will use this to test if a track point is inside or outside the roms grid
  int i,j;
  int count;

  // malloc room for the bounding polygon
  E->polygon = malloc((2*E->nXiRho + 2*E->nEtaRho)*sizeof(pts));
  E->ncoords = (2*E->nXiRho + 2*E->nEtaRho);


  //spheriq_dist(double lon1, double lat1, double lon2, double lat2, int debug)
  E->average_distance_per_index = 0.0;
  for(i=0;i<E->nXiRho-1;i++){
    E->average_distance_per_index += spheriq_dist(E->lon_rho[0][i], E->lat_rho[0][i], E->lon_rho[0][i+1], E->lat_rho[0][i+1],0);
  }
  E->average_distance_per_index = E->average_distance_per_index/(E->nXiRho-1);
  E->average_distance_per_index = E->average_distance_per_index/1000.0; // convert to kilometres
  printf("E->average_distance_per_index = %f km\n", E->average_distance_per_index);
  E->padding = 1000.0/E->average_distance_per_index;
  printf("E->padding = %d cells\n", (int)E->padding);


  // start in the lower left
  // we move counter clockwise

  // bottom row
  count = 0;
  for(i=0;i<E->nXiRho;i++){
    E->polygon[count].x = E->lon_rho[0][i];
    E->polygon[count].y = E->lat_rho[0][i];
    //printf("%f %f\n", E->polygon[count].x, E->polygon[count].y);
    count++;
  }
  // right side
  for(i=0;i<E->nEtaRho;i++){
    E->polygon[count].x = E->lon_rho[i][E->nXiRho-1];
    E->polygon[count].y = E->lat_rho[i][E->nXiRho-1];
    count++;
  }
  // top of grid (moving from right to left)
  for(i=E->nXiRho-1;i>=0;i--){
    E->polygon[count].x = E->lon_rho[E->nEtaRho-1][i];
    E->polygon[count].y = E->lat_rho[E->nEtaRho-1][i];
    count++;
  }
  // left side
  for(i=E->nEtaRho-1;i>=0;i--){
    E->polygon[count].x = E->lon_rho[i][0];
    E->polygon[count].y = E->lat_rho[i][0];
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
  //printf("ncks -d xi_rho,%d,%d -d eta_rho,0,%d %s -O cut.nc\n", low, high, E->nXiRho-1, E->input_file);

}
