#include <iostream>
#define CATCH_CONFIG_RUNNER
#include "catch.hpp"
#include "UserInterface.h"
/*
 * November 12 - added catch run
 */
#include "porter2_stemmer.h"
int main (int argc, char** argv) {
   if(argc == 1)
	Catch:: Session().run();
    else {
	    UserInterface i (argv[ 2 ]);
//	    i.DecipherInput(argv[ 1 ]);
    }
    // for the speed test we only need to use this new method
  //  i.DecipherInput(argv);
	return 0;
}
