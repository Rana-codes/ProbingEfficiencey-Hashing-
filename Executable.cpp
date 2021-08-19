// Name: Harsh Rana

#include "hash_function.h"
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <cmath>

// Constant used to represent empty location in a hashtable.
const int EMPTY = -1;

/**
 * gets a random number to be inserted into the hash table
 * PRECONDITION: rand must be seeded before call to get different values for rand.
 */
int getKey(){

   int x = rand();
   int y = rand();
   return (x << 15)|y;
   // max value of rand() is 32768 = 2^15,
   // when we shift it by 15, max value will become 2^30
}

/**
 * Inserts an element into the hash table, Probing decided by provided string
 * PRECONDITION: The empty positions in array must be represented by -1
 *              probeType must start with l for linear probing
 *              probeType must start with q for quadratic probing
 *              requires a hashing function
 * POSTCONDITION: element is inserted into the hash table.
 */
void insert(int x, int t[], int size, std::string probeType){
   int correctedIndex = hash_fct(x,size);

   for(int i = 0; t[correctedIndex] != EMPTY; i++){
      
      if(probeType[0] == 'L' || probeType[0] == 'l'){
         correctedIndex++;
      }
      else if(probeType[0] == 'Q' || probeType[0] == 'q'){
         correctedIndex = hash_fct(x,size)+ pow(i,2);
      }
      else{
         std::cout << "INCORRECT PROBE RESULTS INCORRECT" ;
      }
      correctedIndex = correctedIndex%size;
   }
   t[correctedIndex] = x;
}

/**
 * searches for an element in the hash table, TYPE OF PROBING MUST BE PROVIDED
 * PRECONDITION: The empty positions in array must be represented by -1
 *              probeType must start with l for linear probing
 *              probeType must start with q for quadratic probing
 *              requires a hashing function
 * POSTCONDITION: returns true if element is present in the hash table, false otherwise.
 *                value of comparisons changes to the number of comparisons made while searching
 */
bool search(int x, int t[], int size, std::string probeType, int& comparisons){
   int correctedSearchIndex = hash_fct(x,size);
   comparisons = 0;

   for(int i = 0; t[correctedSearchIndex] != EMPTY; i++){
      comparisons++; // we are at this point, 1 comparison will be made for sure.
      if(t[correctedSearchIndex] == x){
         return true;
      }
      else{
         if(probeType[0] == 'L' || probeType[0] == 'l'){
            correctedSearchIndex++;
         }
         else if(probeType[0] == 'Q' || probeType[0] == 'q'){
            correctedSearchIndex = hash_fct(x,size)+pow(i,2);
         }
         else{
            std::cout << "INCORRECT PROBE RESULTS INCORRECT" ;
         }
         correctedSearchIndex = correctedSearchIndex%size;
      }
   }
   comparisons++;
   return false;
}


/**
 * Empties the array. That is removes all the elements present in the array.
 * POSTCONDITION: Each element in the array is now -1.
 */
void emptyArray(int t[], int size){
   for(int i = 0; i < size; i++){
      t[i] = EMPTY;
   }
}

/**
 * Fills the array to maintain the given load(factor). Probing is done based on the provided probe type.
 * PRECONDITION: The empty positions in array must be represented by -1
 *              probeType must start with l for linear probing
 *              probeType must start with q for quadratic probing
 *              requires a hashing function
 * POSTCONDITION: The provided array is now filled such that- filled places/total places = load
 */
void fillArray(int t[], int size, double load, std::string probeType){
   int elementsToBeFilled = size*load;
   int toInsert = 0;
   int comparisons = 0;

   for(int i = 0; i<elementsToBeFilled; i++){
      toInsert = getKey();
      while(search(toInsert, t, size, probeType, comparisons)){
         toInsert = getKey();
      }
      insert(toInsert, t, size, probeType);
   }
}

/**
 * Finds the average comparisons made in finding the element when it is present in the hash table for sure.
 * PRECONDITION: The empty positions in array must be represented by -1
 *              probeType must start with l for linear probing
 *              probeType must start with q for quadratic probing
 *              requires a hashing function
 * POSTCONDITION: searches are performed for each element in the array 100 times, average is calculated and returned.
 */
double successfulComparsons(int t[], int size, double LF, int experiments, std::string probeType){
   int comparisons = 0;
   int totalComparisons = 0;
   double elementsPresent = LF*size;

   for(int i = 0; i<experiments; i++){

      fillArray(t, size, LF, probeType);

      for(int j = 0; j<size; j++){
         if(t[j] != EMPTY){
            search(t[j], t, size, probeType, comparisons);
            totalComparisons += comparisons;
         }
      }

      emptyArray(t, size);
   }

   double avgComparisons = totalComparisons/(elementsPresent*experiments);
   return avgComparisons;
}

/**
 * Finds the average comparisons made in finding the element when it is NOT present in the hash table for sure.
 * PRECONDITION: The empty positions in array must be represented by -1
 *              probeType must start with l for linear probing
 *              probeType must start with q for quadratic probing
 *              requires a hashing function
 * POSTCONDITION: 1 frailed search is performed for each filled element in the array 100 times, average is calculated and returned.
 */
double unsuccessfulComparisons(int t[], int size, double LF, int experiments, std::string probeType){
   int comparisons = 0;
   int totalComparisons = 0;
   double elementsPresent = LF*size;

   for(int i = 0; i<experiments; i++){

      fillArray(t, size, LF, probeType);

      for(int j = 0; j<elementsPresent; j++){
         int toBeSearched = getKey();
         while(search(toBeSearched, t, size, probeType, comparisons)){
            toBeSearched = getKey();
         } // to make sure that we are searching the element not present in the table.
         search(toBeSearched, t, size, probeType, comparisons);
         totalComparisons += comparisons;
         
      }

      emptyArray(t, size);
   }
      
   double avgComparisons = totalComparisons/(elementsPresent*experiments);
   return avgComparisons;
}

/**
 * formula to calculate theoretical value of avg comparisons when a successful
 * search is performed using linear probing
 */
double findTheorValueLINEARsuc(double LF){
   double ans = 0;
   ans = (1.00/2.00)*(1.00+(1.00/(1.00-LF)));
   return ans;
}

/**
 * formula to calculate theoretical value of avg comparisons when an UNsuccessful
 * search is performed using linear probing
 */
double findTheorValueLINEARunsuc(double LF){
   double ans = 0;
   ans = (1.00/2.00)*(1.00+(1.00/pow((1.00-LF),2)));
   return ans;
}

/**
 * formula to calculate theoretical value of avg comparisons when a successful
 * search is performed using quadratic probing
 */
double findTheorValueQUADsuc(double LF){
   double ans = 0;
   ans = 1.00 - log((1.00-LF)) - (LF/2.00);
   return ans;
}

/**
 * formula to calculate theoretical value of avg comparisons when an UNsuccessful
 * search is performed using quadratic probing
 */
double findTheorValueQUADunsuc(double LF){
   double ans = 0;
   ans = (1.00/(1.00-LF)) - log((1.00-LF)) - (LF);
   return ans;
}

/**
 * A second hashing function for double hashing.
 * REFERRENCE FROM CLASS NOTES TAKEN
 */
int hash_fct_two(int k){
   return 1997-(k%1997);// 1997 WAS THE FIRST PRIME MOVING BACKWARDS FROM 1999.
}

/**
 * An Insert function that inserts a given value in the hashing table and
 * resolves conflicts using double hashing.
 */
void insertDoubleH(int x, int t[], int size){
   int correctedIndex = hash_fct(x,size);

   for(int i = 0; t[correctedIndex] != EMPTY; i++){
      correctedIndex += hash_fct_two(x);
      correctedIndex = correctedIndex%size;
   }
   t[correctedIndex] = x;
}

/**
 * A search function that searches for the given value in the hash table.
 * PRECONDITION: The empty positions in array must be represented by -1
 *              requires TWO hashing function
 * POSTCONDITION: returns true if element is present in the hash table, false otherwise.
 *                value of comparisons changes to the number of comparisons made while searching
 */
bool searchDoubleH(int x, int t[], int size, int& comparisons){
   int correctedSearchIndex = hash_fct(x,size);
   comparisons = 0;

   for(int i = 0; t[correctedSearchIndex] != EMPTY; i++){
      comparisons++; // we are at this point, 1 comparison will be made for sure.
      if(t[correctedSearchIndex] == x){
         return true;
      }
      else{
         correctedSearchIndex += hash_fct_two(x); 
         correctedSearchIndex = correctedSearchIndex%size;
      }
   }
   comparisons++;
   return false;
}

/**
 * Fills the array to maintain the given load(factor). Double Hashing is used to resolve conflicts.
 * PRECONDITION: The empty positions in array must be represented by -1
 *              requires two hashing function
 * POSTCONDITION: The provided array is now filled such that- filled places/total places = load
 */
void fillArray_doubleH(int t[], int size, double load){
   int elementsToBeFilled = size*load;
   int toInsert = 0;
   int comparisons = 0;

   for(int i = 0; i<elementsToBeFilled; i++){
      toInsert = getKey();
      while(searchDoubleH(toInsert, t, size, comparisons)){
         toInsert = getKey();
      }
      insertDoubleH(toInsert, t, size);
   }
}

/**
 * Finds the average comparisons made in finding the element when it is present in the hash table for sure.
 * PRECONDITION: The empty positions in array must be represented by -1
 *              requires two hashing function
 * POSTCONDITION: searches are performed for each element in the array 100 times, average is calculated and returned.
 */
double successfulComparsons_doubleH(int t[], int size, double LF, int experiments){
   int comparisons = 0;
   int totalComparisons = 0;
   double elementsPresent = LF*size;

   for(int i = 0; i<experiments; i++){

      fillArray_doubleH(t, size, LF);

      for(int j = 0; j<size; j++){
         if(t[j] != EMPTY){
            searchDoubleH(t[j], t, size, comparisons);
            totalComparisons += comparisons;
         }
      }

      emptyArray(t, size);
   }

   double avgComparisons = totalComparisons/(elementsPresent*experiments);
   return avgComparisons;
}

/**
 * Finds the average comparisons made in finding the element when it is NOT present in the hash table for sure.
 * PRECONDITION: The empty positions in array must be represented by -1
 *              requires a hashing function
 * POSTCONDITION: 1 failed search is performed for each filled element in the array 100 times, average is calculated and returned.
 */
double unsuccessfulComparisons_doubleH(int t[], int size, double LF, int experiments){
   int comparisons = 0;
   int totalComparisons = 0;
   double elementsPresent = LF*size;

   for(int i = 0; i<experiments; i++){

      fillArray_doubleH(t, size, LF);

      for(int j = 0; j<elementsPresent; j++){
         int toBeSearched = getKey();
         while(searchDoubleH(toBeSearched, t, size, comparisons)){
            toBeSearched = getKey();
         } // to make sure that we are searching the element not present in the table.
         searchDoubleH(t[j], t, size, comparisons);
         totalComparisons += comparisons;
         
      }

      emptyArray(t, size);
   }
      
   double avgComparisons = totalComparisons/(elementsPresent*experiments);
   return avgComparisons;
}


/**
 * formula to calculate theoretical value of avg comparisons when a successful
 * search is performed using double hashing
 */
double findTheorValueDoubleHsuc(double LF){
   double ans = 0;
   ans = (1.00/LF) * log(1.00/(1.00-LF));
   return ans;
}


/**
 * formula to calculate theoretical value of avg comparisons when an UNsuccessful
 * search is performed using double hashing
 */
double findTheorValueDoubleHunsuc(double LF){
   double ans = 0;
   ans = 1.00/(1.00-LF);
   return ans;
}

int main() {  
   const int SIZE = 1999;
   int T[SIZE];
   emptyArray(T,SIZE);

   // srand should be seeded only once, so it is done in main method.
   unsigned int seed;   
   seed = time (0);     
   srand(seed);

   const double LF_20_PERCENT = 0.20;
   const double LF_50_PERCENT = 0.50;
   const double LF_80_PERCENT = 0.80;

   const double TOTAL_EXPERIMENTS = 100.00;

   

   std::cout << "LF\t" << "probing\t" << "\t" << "comps\t" << "\ttheor\t\t" << "|delta|"  << std::endl;
   double comp = 0.00;
   double theor = 0.00;

   comp = successfulComparsons(T,SIZE, LF_20_PERCENT, TOTAL_EXPERIMENTS, "Linear");
   theor = findTheorValueLINEARsuc(LF_20_PERCENT);
   std::cout << LF_20_PERCENT << "\t" << "Linear\t" << "in T\t" <<  comp << "\t\t" << theor << "\t\t" << fabs(comp-theor) <<std::endl;
   
   comp = unsuccessfulComparisons(T,SIZE, LF_20_PERCENT, TOTAL_EXPERIMENTS, "Linear");
   theor = findTheorValueLINEARunsuc(LF_20_PERCENT);
   std::cout << LF_20_PERCENT << "\t" << "Linear\t" << "not in\t" <<  comp << "\t\t" << theor << "\t\t" << fabs(comp-theor) <<std::endl;

   comp = successfulComparsons(T,SIZE, LF_20_PERCENT, TOTAL_EXPERIMENTS, "Quad");
   theor = findTheorValueQUADsuc(LF_20_PERCENT);
   std::cout << LF_20_PERCENT << "\t" << "Quad\t" << "in T\t" <<  comp << "\t\t" << theor << "\t\t" << fabs(comp-theor) <<std::endl;
   
   comp = unsuccessfulComparisons(T,SIZE, LF_20_PERCENT, TOTAL_EXPERIMENTS, "Quad");
   theor = findTheorValueQUADunsuc(LF_20_PERCENT);
   std::cout << LF_20_PERCENT << "\t" << "Quad\t" << "not in\t" <<  comp << "\t\t" << theor << "\t\t" << fabs(comp-theor) <<std::endl;




   comp = successfulComparsons(T,SIZE, LF_50_PERCENT, TOTAL_EXPERIMENTS, "Linear");
   theor = findTheorValueLINEARsuc(LF_50_PERCENT);
   std::cout << LF_50_PERCENT << "\t" << "Linear\t" << "in T\t" <<  comp << "\t\t" << theor << "\t\t" << fabs(comp-theor) <<std::endl;
   
   comp = unsuccessfulComparisons(T,SIZE, LF_50_PERCENT, TOTAL_EXPERIMENTS, "Linear");
   theor = findTheorValueLINEARunsuc(LF_50_PERCENT);
   std::cout << LF_50_PERCENT << "\t" << "Linear\t" << "not in\t" <<  comp << "\t\t" << theor << "\t\t" << fabs(comp-theor) <<std::endl;

   comp = successfulComparsons(T,SIZE, LF_50_PERCENT, TOTAL_EXPERIMENTS, "Quad");
   theor = findTheorValueQUADsuc(LF_50_PERCENT);
   std::cout << LF_50_PERCENT << "\t" << "Quad\t" << "in T\t" <<  comp << "\t\t" << theor << "\t\t" << fabs(comp-theor) <<std::endl;
   
   comp = unsuccessfulComparisons(T,SIZE, LF_50_PERCENT, TOTAL_EXPERIMENTS, "Quad");
   theor = findTheorValueQUADunsuc(LF_50_PERCENT);
   std::cout << LF_50_PERCENT << "\t" << "Quad\t" << "not in\t" <<  comp << "\t\t" << theor << "\t\t" << fabs(comp-theor) <<std::endl;




   comp = successfulComparsons(T,SIZE, LF_80_PERCENT, TOTAL_EXPERIMENTS, "Linear");
   theor = findTheorValueLINEARsuc(LF_80_PERCENT);
   std::cout << LF_80_PERCENT << "\t" << "Linear\t" << "in T\t" <<  comp << "\t\t" << theor << "\t\t" << fabs(comp-theor) <<std::endl;
   
   comp = unsuccessfulComparisons(T,SIZE, LF_80_PERCENT, TOTAL_EXPERIMENTS, "Linear");
   theor = findTheorValueLINEARunsuc(LF_80_PERCENT);
   std::cout << LF_80_PERCENT << "\t" << "Linear\t" << "not in\t" <<  comp << "\t\t" << theor << "\t\t" << fabs(comp-theor) <<std::endl;

   comp = successfulComparsons(T,SIZE, LF_80_PERCENT, TOTAL_EXPERIMENTS, "Quad");
   theor = findTheorValueQUADsuc(LF_80_PERCENT);
   std::cout << LF_80_PERCENT << "\t" << "Quad\t" << "in T\t" <<  comp << "\t\t" << theor << "\t\t" << fabs(comp-theor) <<std::endl;
   
   comp = unsuccessfulComparisons(T,SIZE, LF_80_PERCENT, TOTAL_EXPERIMENTS, "Quad");
   theor = findTheorValueQUADunsuc(LF_80_PERCENT);
   std::cout << LF_80_PERCENT << "\t" << "Quad\t" << "not in\t" <<  comp << "\t\t" << theor << "\t\t" << fabs(comp-theor) <<std::endl;
   std::cout << "\n\n\n";
  
   

   comp = successfulComparsons_doubleH(T,SIZE, LF_20_PERCENT, TOTAL_EXPERIMENTS);
   theor = findTheorValueDoubleHsuc(LF_20_PERCENT);
   std::cout << LF_20_PERCENT << "\t" << "Double\t" << "in T\t" <<  comp << "\t\t" << theor << "\t\t" << fabs(comp-theor) <<std::endl;
   
   
   comp = unsuccessfulComparisons_doubleH(T,SIZE, LF_20_PERCENT, TOTAL_EXPERIMENTS);
   theor = findTheorValueDoubleHunsuc(LF_20_PERCENT);
   std::cout << LF_20_PERCENT << "\t" << "Double\t" << "Not in\t" <<  comp << "\t\t" << theor << "\t\t" << fabs(comp-theor) <<std::endl;
   
   

   comp = successfulComparsons_doubleH(T,SIZE, LF_50_PERCENT, TOTAL_EXPERIMENTS);
   theor = findTheorValueDoubleHsuc(LF_50_PERCENT);
   std::cout << LF_50_PERCENT << "\t" << "Double\t" << "in T\t" <<  comp << "\t\t" << theor << "\t\t" << fabs(comp-theor) <<std::endl;
   
   comp = unsuccessfulComparisons_doubleH(T,SIZE, LF_50_PERCENT, TOTAL_EXPERIMENTS);
   theor = findTheorValueDoubleHunsuc(LF_50_PERCENT);
   std::cout << LF_50_PERCENT << "\t" << "Double\t" << "Not in\t" <<  comp << "\t\t" << theor << "\t\t" << fabs(comp-theor) <<std::endl;

   comp = successfulComparsons_doubleH(T,SIZE, LF_80_PERCENT, TOTAL_EXPERIMENTS);
   theor = findTheorValueDoubleHsuc(LF_80_PERCENT);
   std::cout << LF_80_PERCENT << "\t" << "Double\t" << "in T\t" <<  comp << "\t\t" << theor << "\t\t" << fabs(comp-theor) <<std::endl;
   
   
   comp = unsuccessfulComparisons_doubleH(T,SIZE, LF_80_PERCENT, TOTAL_EXPERIMENTS);
   theor = findTheorValueDoubleHunsuc(LF_80_PERCENT);
   std::cout << LF_80_PERCENT << "\t" << "Double\t" << "Not in\t" <<  comp << "\t\t" << theor << "\t\t" << fabs(comp-theor) <<std::endl;
   
  
   return 0;
}