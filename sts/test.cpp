#include <cstdlib>
#include <cstring>
#include <string>
#include <cstdio>
#include <iostream>
#include <random>
#include "rngtest.h"

unsigned char eps[MY_N] ;

int main(void)
{
  /*
  for(int i=0;i<MY_N;i++)
  {
    scanf("%c", &eps[i]) ;
    eps[i] -= '0' ;
    if(eps[i]!=0 && eps[i]!=1) i-- ;
  }
  */
  for(int i=0;i<MY_N;i++) eps[i] = rand()%2 ;
  
  std::cout << Frequency(MY_N, eps) << std::endl ;
  std::cout << BlockFrequency(MY_N/100, MY_N, eps) << std::endl ;
  std::cout << Runs(MY_N, eps) << std::endl ;
  std::cout << LongestRunOfOnes(MY_N, eps) << std::endl ;
  std::cout << Rank(MY_N, eps) << std::endl ;
  std::cout << DiscreteFourierTransform(MY_N, eps) << std::endl ;
  std::cout << NonOverlappingTemplateMatchings(10, MY_N, eps) << std::endl ;
  std::cout << OverlappingTemplateMatchings(10, MY_N, eps) << std::endl ;
  std::cout << Universal(MY_N, eps) << std::endl ;
  std::cout << LinearComplexity(1000, MY_N, eps) << std::endl ;
  std::cout << Serial(5, MY_N, eps) << std::endl ;
  std::cout << ApproximateEntropy(5, MY_N, eps) << std::endl ;
  std::cout << CumulativeSums(MY_N, eps) << std::endl ;
  std::cout << RandomExcursions(MY_N, eps) << std::endl ;
  std::cout << RandomExcursionsVariant(MY_N, eps) << std::endl ;

  return 0 ;
}

