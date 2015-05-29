#include "DRNG.h"

#include <string>
#include <cstring>
#include <algorithm>
#include <cmath>

#define NUM_DRAW 10000
#define EPS_FREQ 0.05

bool RNGTester::Test(DRNG *rng) throw(std::string)
{
    if(rng==NULL)
        throw(std::string("RNG cannot be NULL.")) ;

    for(int ub=2; ub<=16;ub<<=1)
    {
        int *cnt = new int[ub] ;
        memset(cnt, 0, sizeof(int)*ub) ;
        
        int run = 0, longRun = 0 ;
        int current = -1 ;

        for(int i=0;i<NUM_DRAW;i++)
        {
            int val = rng->Next(0, ub-1) ;
            cnt[val]++ ;
            if(val==current)
                run++ ;
            else
            {
                longRun = std::max(longRun, run) ;
                run = 1 ;
                current = val ;
            }
        }
        longRun = std::max(longRng, run) ;

        //freq test
        double target = ((double)NUM_DRAW)/ub ;
        for(int i=0;i<ub;i++)
            if(fabs(1.0-cnt[i]/target)>EPS_FREQ)
                return false ;

        //run test

    }
}
