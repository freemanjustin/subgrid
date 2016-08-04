#include "reader.h"

void get_continent(e *E){

  FILE  *continent_file;
  char  str[80];
  int   count;
  float trash;

  continent_file = fopen("AustraliaContinentOutline.txt","r");
  if(continent_file == NULL){
    fail("couldn't open continent outline file\n");
  }

  // how many points are in the file?
  count = 0;
  while(fgets(str,80,continent_file) != NULL){
    count++;
  }

  fclose(continent_file);
  // malloc room for the polygon data
  E->polygon = malloc(count*sizeof(pts));
  E->ncoords = count;

  continent_file = fopen("AustraliaContinentOutline.txt","r");
  if(continent_file == NULL){
    fail("couldn't open continent outline file\n");
  }

  // now read the data
  count = 0;
  while(fgets(str,80,continent_file) != NULL){
    sscanf(str,"%f %f %f\n", &E->polygon[count].y, &E->polygon[count].x, &trash );
    //printf("%f %f\n", E->polygon[count].y, E->polygon[count].x);
    //fflush(stdout);
    count++;
  }

  //printf("ncoords = %d\n", E->ncoords);

}
