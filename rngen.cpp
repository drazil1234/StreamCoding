#include "DRNG.h"
#include <iostream>
#include <cstdint>
#include <vector>

using namespace std ;

int main(void)
{
  AESCTRDRBG rng("a_more_more_strong_seed ;23iur[arsf@#$QW#Rasdf0-sadflkasdf/lk1234asdfdrazil r02922122", "another_more_df2k3kjlriajsdf023ris@#$@#$%$GFASDFmore_stronger_seed tmt514514514") ;
  
  for(int i=0;i<7000000;i++) cout << (int)rng.GetBits(1)[0] ;

  return 0 ;
}
