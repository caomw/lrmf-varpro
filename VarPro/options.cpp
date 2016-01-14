#include "options.h"

/*
* (STRUCT) options: Experiment options and parameters
*/

// Reset function
void options::reset() {
  // Declare variables
  dataset = "";              // Dataset name
  folder = ".";               // Dataset folder

  m = 0;                     // Number of rows
  n = 0;                     // Number of columns
  r = 0;                     // Rank of V

  DEBUG = false;             // Flag for showing debug information
  HELP = false;              // Flag for showing whether the experiment options are sufficient.
}