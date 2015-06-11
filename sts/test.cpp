#include <cstdlib>
#include <cstring>
#include <string>
#include <cstdio>
#include <iostream>
#include <random>

extern "C"
{
  double Frequency(int n, unsigned char *epsilon) ;
  double BlockFrequency(int M, int n, unsigned char *epsilon) ;
  double Runs(int n, unsigned char *epsilon) ;
  double LongestRunOfOnes(int n, unsigned char *epsilon) ;
  double Rank(int n, unsigned char *epsilon) ;
  double DiscreteFourierTransform(int n, unsigned char *epsilon) ;
  double NonOverlappingTemplateMatchings(int m, int n, unsigned char *epsilon) ;
  double OverlappingTemplateMatchings(int m, int n, unsigned char *epsilon) ;
  double Universal(int n, unsigned char *epsilon) ;
  double LinearComplexity(int M, int n, unsigned char *epsilon) ;
  double Serial(int m, int n, unsigned char *epsilon) ;
  double ApproximateEntropy(int m, int n, unsigned char *epsilon) ;
  double CumulativeSums(int n, unsigned char *epsilon) ;
  double RandomExcursions(int n, unsigned char *epsilon) ;
  double RandomExcursionsVariant(int n, unsigned char *epsilon) ;
}

#define MY_N 7000000

unsigned char eps[MY_N] ;

int main(void)
{
  std::random_device rd("/dev/random") ;
  for(int i=0;i<MY_N;i++) eps[i] = rd()%2 ;
  //for(int i=0;i<MY_N;i++) eps[i] = rand()%2 ;
  
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

