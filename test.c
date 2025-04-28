#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "wug.h"


void test1(wug_t sample[]) {
  
  printf("testing Q1: genome2features\n");
  
  for(int i = 0; i < 4; i++) {
    int feats[4];
    genome2features(sample[i].genome, feats);
    printf("g:%s f:%s\n", array_string(sample[i].genome, 16), array_string(feats, 4));
  }
}


void test2(wug_t sample[]) {

  printf("testing Q2: report_population\n");
  
  int r = report_population(sample, 4);
  printf("population has %d superwugs\n", r);
}


void test3(wug_t sample[]) {

  printf("testing Q3: rank\n");
  
  for(int i = 0; i < 4; i++) {
    int feats[4]; genome2features(sample[i].genome,feats);
    printf("(%s,%c) %d\n", array_string(feats,4), sample[i].gender, rank(sample[i]));
  }
}


void test4(wug_t sample[]) {
  
  printf("testing Q4: insert_ranked\n");
  
  wug_t population[4];
  int n = 0;
  
  insert_ranked(population, sample[1], &n, 4); 
  insert_ranked(population, sample[0], &n, 4); 
  insert_ranked(population, sample[0], &n, 4); 
  insert_ranked(population, sample[2], &n, 4);
  
  for(int i = 0; i < n; i++) {
    printf("%d ", i); print_wug(&population[i]);
  }

  printf("no change\n");
  insert_ranked(population, sample[1], &n, 4); 
  insert_ranked(population, sample[0], &n, 4);
  
  for(int i = 0; i < n; i++) {
    printf("%d ", i); print_wug(&population[i]);
  }

}


void test5(int size) {

  printf("testing Q5: proliferate\n");
  
  wug_t population[size];
  int n = 0;
  
  wug_t a = {{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},F};
  insert_ranked(population, a, &n, size);
  
  proliferate(population, &n, size);
  report_population(population, n);
}


void test6(int size) {

  printf("testing Q6: breeding\n");
  
  wug_t population[size];
  int n = 0;
	    
  wug_t a = {{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},F};
  wug_t b = {{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},M};

  insert_ranked(population, a, &n, size);
  insert_ranked(population, b, &n, size);
  
  breed(population, &n, size);
  report_population(population, n);
}  


int main(int argc, char *argv[]) {

  wug_t samples[4] = {{{0,1,0,1,0,1,0,0,0,1,0,1,1,0,0,0},F},
                      {{1,0,0,0,0,0,0,0,1,1,0,0,1,0,0,0},M},
		      {{1,1,0,1,0,0,0,0,0,0,0,0,1,0,0,0},M},
		      {{1,1,0,1,1,1,0,0,0,1,0,0,1,1,1,1},F}};

  test1(samples);
  test2(samples);
  test3(samples);
  test4(samples);
  test5(20);
  test6(20);

  return 0;
}
