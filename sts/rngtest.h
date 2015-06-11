#pragma once

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

