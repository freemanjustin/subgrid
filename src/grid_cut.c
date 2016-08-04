#include "reader.h"

void cut_grid(e *E){

  int eta,xi;
  int low, high;
  float lon_low,lon_high;

  low = 0;
  high = E->nLonRho-1;
  // find closest point to E->min_lon and E->max_lon
  for(eta=0;eta<E->nLonRho;eta++){  // eta
    for(xi=0;xi<E->nLatRho;xi++){  // xi
        if(E->lon_rho[eta][xi] < E->min_lon){
              lon_low = E->lon_rho[eta][xi];
              low = xi;
        }
          /*if(E->lat_rho[eta][xi] < E->roms_min_lat)
              E->roms_min_lat = E->lat_rho[eta][xi];
          if(E->lat_rho[eta][xi] > E->roms_max_lat)
              E->roms_max_lat = E->lat_rho[eta][xi];
            */
    }
  }

  for(eta=0;eta<E->nLonRho;eta++){  // eta
    for(xi=E->nLatRho-1;xi>=0;xi--){  // xi
        if(E->lon_rho[eta][xi] > E->max_lon){
            lon_high = E->lon_rho[eta][xi];
            high = xi;
        }
          /*if(E->lat_rho[eta][xi] < E->roms_min_lat)
              E->roms_min_lat = E->lat_rho[eta][xi];
          if(E->lat_rho[eta][xi] > E->roms_max_lat)
              E->roms_max_lat = E->lat_rho[eta][xi];
            */
    }
  }




  printf("roms info:\n");
  printf("min cut lon = %f, index = %d\n", lon_low, low);
  printf("max cut lon = %f, index = %d\n", lon_high, high);
  //printf("min cut lat = %f\n", E->lat_rho[min_x][min_y]);
  //printf("max cut lat = %f\n", E->lat_rho[min_x][max_y]);

  // for now we will ncks the input roms grid to produce a new subset grid
  // for the ensemble runs
  printf("ncks -d xi_rho,%d,%d -d eta_rho,0,%d %s -O cut.nc\n", low, high, E->nLonRho-1, E->input_file);




}
