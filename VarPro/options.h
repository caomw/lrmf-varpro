#ifndef _options_H_
#define _options_H_

#include <string>

/*
* (STRUCT) options: Experiment options and parameters
*/
typedef struct options {

  // Reset function
  void reset();                   // Resets all the values.

  // Declare variables
  std::string dataset = "";            // Dataset name
  std::string folder = ".";        // Dataset folder

  int m = 0;                      // Number of rows
  int n = 0;                      // Number of columns
  int r = 0;                      // Rank of V

  bool DISPLAY = true;            // Flag for displaying the output thoroughly
  bool DEBUG = false;             // Flag for showing debug information
  bool HELP = false;              // Flag for showing whether the experiment options are sufficient.

} options_t;

#endif