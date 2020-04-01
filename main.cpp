# include <iostream> 
# include <cmath>
# include <cstring> 
#include <stdlib.h>
 
using namespace std; 
 
# define VARIABLES 3 

// initialize here, these can be overidden by the user
 static int POPULATION_SIZE=30 ;
 static int MAXGERATIONS=50 ;
 static int RUNS=30 ;
 static double PC=0.8 ;
 static double PM=0.1 ;
 // 1 -- proportional, 2 -- binary tournament , 3 -- linear ranking
 static int selectionStrategy = 1;
 
 
struct genetic 
{ 
  double gene[VARIABLES]; 
  double fitness; 
  double upper[VARIABLES]; 
  double lower[VARIABLES]; 
  double rfitness; 
  double cfitness; 
}; 
 
struct genetic *population = NULL;
struct genetic *newpopulation = NULL;
double *Best_of_genration = NULL;
double *Best_of_run = NULL;
double *Avg_of_genration = NULL;
double *Best_of_genration_in_run = NULL;
 
static void allocate_globals()
{
  population = new genetic[POPULATION_SIZE+1]; 
  newpopulation = new genetic[POPULATION_SIZE+1];  
  Best_of_genration = new double[MAXGERATIONS]; 
  Best_of_run = new double[RUNS]; 
  Avg_of_genration = new double[RUNS]; 
  Best_of_genration_in_run = new double[RUNS]; 
}

void crossover ( int &seed ); 
void elitist ( ); 
void evaluate ( ); 
int Random_int ( int a, int b, int &seed ); 
void initialize ( int &seed ); 
void keep_the_best ( ); 
void mutate ( int &seed ); 
double Random_double ( double a, double b, int &seed ); 
void report ( int generation, int run ); 
void report2 (); 
void selector ( int &seed ); 
 
/**
 * print usage
 */
static void usage(const char* prog_name)
{
  cerr << "usage: " << prog_name << " population_size max_generations runs pc pm <1 or 2 or 3 for selectionStrategy>" << endl;
  cerr << "       1 -- proportional, 2 -- binary tournament , 3 -- linear ranking " << endl;
} 

/**
 * The algorithm parameters (population size, crossover rate, etc.) may be taken from the user on the command line
 *   population size, crossover rate, etc.
 * # define POPULATION_SIZE 30 
 * # define MAXGERATIONS 50 
 * # define RUNS 30 
 * # define PC 0.8 
 * # define PM 0.1 
 * N = 30; pc = 0.8; pm = 0.1; max generations (for use as a stopping condition) = 50.
 */
int main ( int argc, char** argv) 
{ 
  // should accept 6 arguments, to keep code simple let's use positional arguments
  // argv[1] == POPULATION_SIZE
  // argv[2] == MAXGERATIONS
  // argv[3] == RUNS
  // argv[4] == PC
  // argv[5] == PM
  // argv[6] == selectionStrategy

  if (argc <7) {
   usage(argv[0]); return 1;
  }

  int arg_POPULATION_SIZE=atoi(argv[1]);
  int arg_MAXGERATIONS=atoi(argv[2]);
  int arg_RUNS=atoi(argv[3]);
  double arg_PC=atof(argv[4]);
  double arg_PM=atof(argv[5]);
  int arg_selectionStrategy=atoi(argv[6]);

  cout << arg_POPULATION_SIZE << " " << arg_MAXGERATIONS << " " << arg_RUNS << " " << arg_PC << " " << arg_PM << endl;
  // assign to globals
  POPULATION_SIZE = arg_POPULATION_SIZE ;
  MAXGERATIONS = arg_MAXGERATIONS ;
  RUNS = arg_RUNS ;
  PC = arg_PC ;
  PM = arg_PM ;
  // allocate globals
  allocate_globals();
  selectionStrategy = arg_selectionStrategy;

  //
  // selection strategy 
  // 1 -- proportional, 2 -- binary tournament , 3 -- linear ranking
  //

  int generation; 
  int i; 
  double sum = 0.0; 
  int run; 
  int seed; 
 
   
  cout << "\n"; 
  cout << "Genetic Algorithm(GA) \n"; 
  
  seed = 117745; 
  initialize (seed ); 
 
  evaluate ( ); 
  keep_the_best ( ); 
   
  for( run = 0; run< RUNS ; run++) 
  { 
    seed = seed + run + 21; 
 
    for ( generation = 0; generation < MAXGERATIONS; generation++ ) 
    { 
        selector ( seed ); 
        crossover ( seed ); 
        mutate ( seed ); 
        evaluate ( ); 
        elitist ( ); 
        Best_of_genration[generation] = population[POPULATION_SIZE].fitness; 
        if((generation % 10 == 0 || generation == 0 || generation == MAXGERATIONS - 1)) 
            report ( generation, run ); 
    } 
    sum = 0.0; 
    for(i = 0; i< MAXGERATIONS; i++) 
        sum = sum + Best_of_genration[i]; 
  
     Avg_of_genration[run] = sum / (double)MAXGERATIONS;    
     Best_of_run[run] = population[POPULATION_SIZE].fitness; 
     Best_of_genration_in_run[run] = population[POPULATION_SIZE].fitness; 
 
 }  
  report2(); 
  return 0; 
} 
 
 
 
void crossover ( int &seed ) 
{ 
  const double a = 0.0; 
  const double b = 1.0; 
  int j; 
  int one; 
  int first = 0; 
  double x; 
 
  for ( j = 0; j < POPULATION_SIZE; ++j ) 
  { 
    x = Random_double ( a, b, seed ); 
 
    if ( x < PC ) 
    { 
      ++first; 
 
      if ( first % 2 == 0 ) 
      { 
        
        int i; 
        int point; 
        double temp; 
        point = Random_int ( 0, VARIABLES - 1, seed ); 
        for ( i = 0; i < point; i++ ) 
        { 
         temp  = population[one].gene[i]; 
         population[one].gene[i] = population[j].gene[i]; 
         population[j].gene[i] = temp; 
        } 
      } 
      else 
      { 
        one = j; 
      } 
 
    }
  }
  return; 
} 
 
void elitist ( ) 
{ 
  int i; 
  double bestVal; 
  int best; 
  double worstVal; 
  int worst; 
 
  bestVal = population[0].fitness; 
  worstVal = population[0].fitness; 
 
  for ( i = 0; i < POPULATION_SIZE - 1; ++i ) 
  { 
    if ( population[i+1].fitness < population[i].fitness ) 
    { 
 
      if ( bestVal <= population[i].fitness ) 
      { 
        bestVal = population[i].fitness; 
        best = i; 
      } 
 
      if ( population[i+1].fitness <= worstVal ) 
      { 
        worstVal = population[i+1].fitness; 
        worst = i + 1; 
      } 
 
    } 
    else 
    { 
 
      if ( population[i].fitness <= worstVal ) 
      { 
        worstVal = population[i].fitness; 
        worst = i; 
      } 
 
      if ( bestVal <= population[i+1].fitness ) 
      { 
        bestVal = population[i+1].fitness; 
        best = i + 1; 
      } 
 
    } 
 
  } 
 
  if ( population[POPULATION_SIZE].fitness >= worstVal ) 
  { 
    for ( i = 0; i < VARIABLES; i++ ) 
    { 
      population[POPULATION_SIZE].gene[i] = population[worst].gene[i]; 
    } 
    population[POPULATION_SIZE].fitness = population[worst].fitness; 
  } 
  else 
  { 
    for ( i = 0; i < VARIABLES; i++ ) 
    { 
      population[best].gene[i] = population[POPULATION_SIZE].gene[i]; 
    } 
    population[best].fitness = population[POPULATION_SIZE].fitness; 
  }  
 
  return; 
} 
 
void evaluate ( ) 
{ 
  int ecp; 
  int i; 
  double x[VARIABLES+1]; 
 
  for ( ecp = 0; ecp < POPULATION_SIZE; ecp++ ) 
  { 
    for ( i = 0; i < VARIABLES; i++ ) 
    { 
      x[i+1] = population[ecp].gene[i]; 
    }  
    population[ecp].fitness = ( x[1] * x[1] ) + ( x[2] * x[2] ) + (x[3] * x[3]); 
  } 
  return; 
} 
 
int Random_int ( int a, int b, int &seed ) 
{ 
  int c; 
  const int huge = 2147483647; 
  int k; 
  float r; 
  int value; 
 
  if ( b < a ) 
  { 
    c = a; 
    a = b; 
    b = c; 
  } 
 
  k = seed / 117745; 
 
  seed = 16807 * ( seed - k * 117745 ) - k * 2836; 
 
  if ( seed < 0 ) 
  { 
    seed = seed + huge; 
  } 
 
  r = ( float ) ( seed ) * 4.656612875E-10; 
  r = ( 1.0 - r ) * ( ( float ) a - 0.5 )  
    +         r   * ( ( float ) b + 0.5 ); 
  value = round ( r ); 
 
  if ( value < a ) 
  { 
    value = a; 
  } 
  if ( b < value ) 
  { 
    value = b; 
  } 
 
  return value; 
} 
 
void initialize ( int &seed) 
{ 
  int i; 
  int j; 
  double lowerb= -1.0; 
  double upperb = 5.0; 
 
   
  for ( i = 0; i < VARIABLES; i++ ) 
  { 
  
 
    for ( j = 0; j < POPULATION_SIZE; j++ ) 
    { 
      population[j].fitness = 0; 
      population[j].rfitness = 0; 
      population[j].cfitness = 0; 
      population[j].lower[i] = lowerb; 
      population[j].upper[i]= upperb; 
      population[j].gene[i] = Random_double ( lowerb, upperb, seed ); 
    } 
  } 
 
 return; 
} 

 
 
void mutate ( int &seed ) 
{ 
  const double a = 0.0; 
  const double b = 1.0; 
  int i; 
  int j; 
  double lowerb = -1.0; 
  double upperb = 5.0; 
  double x; 
 
  for ( i = 0; i < POPULATION_SIZE; i++ ) 
  { 
    for ( j = 0; j < VARIABLES; j++ ) 
    { 
      x = Random_double ( a, b, seed ); 
      if ( x < PM ) 
      { 
        lowerb = population[i].lower[j]; 
        upperb = population[i].upper[j];   
        population[i].gene[j] = Random_double ( lowerb, upperb, seed ); 
      } 
    } 
  } 
 
  return; 
} 
 
double Random_double ( double a, double b, int &seed ) 
{ 
  int huge = 2147483647; 
  int k; 
  double value; 
 
  
 
  k = seed / 117745; 

 
   seed = 16807 * ( seed - k * 117745 ) - k * 2836; 
 
  if ( seed < 0 ) 
  { 
    seed = seed + huge; 
  } 
 
  value = ( double ) ( seed ) * 4.656612875E-10; 
 
  value = a + ( b - a ) * value; 
 
  return value; 
} 
 
 
void report2() 
{ 
    int i; 
    double avg,avg1,avg2; 
    double square_sum; 
  double stddev; 
  double sum = 0.0, sum1 = 0.0, sum2 = 0.0; 
  double sum_square = 0.0; 
    for ( i = 0; i < RUNS; i++ ) 
  { 
      
    sum = sum + Best_of_run[i]; 
   sum1 = sum1 + Avg_of_genration[i]; 
   sum2 = sum2 + Best_of_genration_in_run[i]; 
  } 
   avg = sum / ( double ) RUNS; 
   avg1 = sum1 / (double ) RUNS; 
   avg2 = sum2 / (double) RUNS; 
  square_sum = avg * avg * RUNS; 
  stddev = sqrt ( ( sum_square - square_sum ) / ( POPULATION_SIZE - 1 ) ); 
    
   
  cout<<"\n\n"<<"Average of Best of run Fitness :  "<<avg;  
  cout<<"\n"<<"Average of Average of Generations :  "<<avg1; 
  cout <<"\n"<<"Average of Best of Genration : " <<avg2; 
} 
 
void report ( int generation , int run ) 
{ 
   
  double avg; 
  double best_val; 
  int i,worst; 
  double sum = 0.0; 
   
  double min = 4.0; 
  double max = 0.0; 
  int best; 
  
for ( i = 0; i < POPULATION_SIZE; i++ ) 
  { 
    sum = sum + population[i].fitness; 
  } 
   
  for (i=0; i< POPULATION_SIZE; i++) 
  { 
      if(population[i].fitness < min) 
      { 
        min = population[i].fitness; 
        worst = i; 
      } 
  } 
  for (i=0; i< POPULATION_SIZE; i++) 
  { 
      if(population[i].fitness > max) 
      { 
        max = population[i].fitness; 
        best = i; 
      } 
  } 

 
 avg = sum / ( double ) POPULATION_SIZE; 
  best_val = population[POPULATION_SIZE].fitness; 
    cout <<"\n\n"; 
  cout << "Run_num :" << run<<"  "; 
  cout << "Generation_num :" << generation <<"  "; 
  cout << "Best Value of run  :" <<max <<"  "; 
  cout << "Worst Value of run :"<< min <<"  "; 
  cout << "Average_run : " << avg <<"  "; 
     
     
  cout << "\nX Vectors for best case  ";         
    for ( i = 0; i < VARIABLES; i++ ) 
  { 
    cout << "  X[" << i+1 << "] = " << population[worst].gene[i] << "\t"; 
  } 
  cout<<"\n"; 
      
  cout << "X vectors for Worst Case ";         
    for ( i = 0; i < VARIABLES; i++ ) 
  { 
    cout << "  X[" << i+1 << "] = " << population[best].gene[i] << "\t"; 
  } 
  cout<<"\n"; 
 
  return; 
} 
 
void selector ( int &seed ) 
 
{ 
  const double a = 0.0; 
  const double b = 1.0; 
  int i; 
  int j; 
  int mem; 
  double p; 
  double sum; 
  double max = 0.0;
  
  for (i=0; i< POPULATION_SIZE; i++) 
  { 
      if(population[i].fitness > max) 
      { 
        max = population[i].fitness; 
       
      } 
  } 
  
  for ( mem = 0; mem < POPULATION_SIZE; mem++ ) 
  { 
    population[mem].fitness = max - population[mem].fitness; 
  } 
  
  sum = 0.0; 
  for ( mem = 0; mem < POPULATION_SIZE; mem++ ) 
  { 
    sum = sum + population[mem].fitness; 
  } 
  for ( mem = 0; mem < POPULATION_SIZE; mem++ ) 
  { 
    population[mem].rfitness = population[mem].fitness / sum; 
  } 
  population[0].cfitness = population[0].rfitness; 
  for ( mem = 1; mem < POPULATION_SIZE; mem++ ) 
  { 
    population[mem].cfitness = population[mem-1].cfitness +        
      population[mem].rfitness; 
  } 
  for ( i = 0; i < POPULATION_SIZE; i++ ) 
  {  
    p = Random_double ( a, b, seed ); 
    if ( p < population[0].cfitness ) 
    { 
      newpopulation[i] = population[0];       
    } 
    else 
    { 
      for ( j = 0; j < POPULATION_SIZE; j++ ) 
      {  
        if ( population[j].cfitness <= p && p < population[j+1].cfitness ) 
        { 
          newpopulation[i] = population[j+1]; 
        } 
      } 
    } 
  } 
  for ( i = 0; i < POPULATION_SIZE; i++ ) 
  { 
    population[i] = newpopulation[i];  
  } 
 
  return;      
} 
 
void keep_the_best ( ) 
{ 
  int best = 0; 
  int j; 
  int i;
  int min = 999;
 
 
  for ( j = 0; j < POPULATION_SIZE; j++ ) 
  { 
    if (  population[j].fitness < min ) 
    { 
        best = j; 
      population[POPULATION_SIZE].fitness = population[j].fitness; 
      min = population[j].fitness;
    } 
  } 
 for ( i = 0; i < VARIABLES; i++ ) 
 { 
    population[POPULATION_SIZE].gene[i] = population[best].gene[i]; 
  } 
 
  return; 
 }

