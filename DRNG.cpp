#include "DRNG.h"

#include "sts/rngtest.h"

#include <string>
#include <cstring>
#include <algorithm>
#include <cmath>
#include <random>
#include <iostream>
#include <iomanip>

bool RNGTester::Test(DRNG *rng) throw(std::string)
{
  if(rng==NULL)
    throw(std::string("RNG cannot be NULL.")) ;

  double p_value = 1.0 ;
  unsigned char *eps = new unsigned char[MY_N] ;
  for(int i=0;i<MY_N;i++)
    eps[i] = rng->Next(0, 1) ;

  std::cout << std::fixed << std::setprecision(6) ;

  p_value = std::min(p_value, Frequency(MY_N, eps) ) ;
  p_value = std::min(p_value, BlockFrequency(MY_N/100, MY_N, eps) ) ;
  p_value = std::min(p_value, Runs(MY_N, eps) ) ;
  p_value = std::min(p_value, LongestRunOfOnes(MY_N, eps) ) ;
  p_value = std::min(p_value, Rank(MY_N, eps) ) ;
  p_value = std::min(p_value, DiscreteFourierTransform(MY_N, eps) ) ;
  p_value = std::min(p_value, NonOverlappingTemplateMatchings(10, MY_N, eps) ) ;
  p_value = std::min(p_value, OverlappingTemplateMatchings(10, MY_N, eps) ) ;
  p_value = std::min(p_value, Universal(MY_N, eps) ) ;
  p_value = std::min(p_value, LinearComplexity(1000, MY_N, eps) ) ;
  p_value = std::min(p_value, Serial(5, MY_N, eps) ) ;
  p_value = std::min(p_value, ApproximateEntropy(5, MY_N, eps) ) ;
  p_value = std::min(p_value, CumulativeSums(MY_N, eps) ) ;
  p_value = std::min(p_value, RandomExcursions(MY_N, eps) ) ;
  p_value = std::min(p_value, RandomExcursionsVariant(MY_N, eps) ) ;

  std::cout << "p_value = " << p_value << std::endl ;

  delete []eps ;

  return (p_value>=0.01) ; 
}

uint64_t DRNG::Next(uint64_t lb, uint64_t ub)
{
  std::uniform_int_distribution<uint64_t> d(lb, ub) ;
  return d(*this) ;
}

