#include <iostream>
#include <cmath>

#include "optimizer.h"
#include "parser.h"

void parse_options_from_args(int argc, char* argv[], options_t &general_opts, optimizer_opts_t &optimizer_opts) {
  
  // If no arguments provided, check for help and then allow user to type them one by one.
  if (argc < 2) {
    
    parse_options_manually(general_opts, 1);
    
    // Otherwise, parse the input arguments.
  } else {
    
    std::string arg;
    std::string argType;
    int len = 0;
    
    for (int i = 1; i < argc; i++) {
      
      arg = argv[i];
      len = int(arg.find('='));
      argType = arg.substr(0, len);
      arg = arg.substr(len + 1);
      
      // Parse dataset.
      if (argType == "-d" || argType == "--dataset") {
        general_opts.dataset = arg;
      } else if (argType == "-f" || argType == "--folder") {
        general_opts.folder = arg;
        // Parse the number of rows.
      } else if (argType == "-m" || argType == "--rows") {
        general_opts.m = atoi(arg.c_str());
        // Parse the number of columns.
      } else if (argType == "-n" || argType == "--cols") {
        general_opts.n = atoi(arg.c_str());
        // Parse rank.
      } else if (argType == "-r" || argType == "--rank") {
        general_opts.r = atoi(arg.c_str());
        // Parse tolerance.
      } else if (argType == "-t" || argType == "--func_tol") {
        optimizer_opts.func_tol = std::strtod(arg.c_str(), NULL);
        // Parse the maximum number of iterations.
      } else if (argType == "-i" || argType == "--max_iter") {
        optimizer_opts.max_iter = atoi(arg.c_str());
        // Parse whether to display CERES outputs.
      } else if (argType == "--display") {
        optimizer_opts.DISPLAY = atoi(arg.c_str()) > 0 || (arg == argType);
        // Parse whether to output debug infos.
      } else if (argType == "--debug") {
        general_opts.DEBUG =  atoi(arg.c_str()) > 0 || (arg == argType);
        // Output a list of commands.
      } else if (argType == "/?" || argType == "-h" || argType == "--help") {
        general_opts.HELP = true;
        outputCommands();
        return;
      }
    } // End of for loop
    
    // Parse general_opts manually for incorrect inputs.
    parse_options_manually(general_opts, 0);
    
    // If debug is on, output the input arguments.
    if (general_opts.DEBUG) {
      
      std::cout << std::endl;
      
      std::cout << "---[ PARSED INPUT ARGUMENTS ]---" << std::endl;
      
      std::cout << "1. Dataset: " << general_opts.dataset << std::endl;
      std::cout << "2. Folder: " << general_opts.folder << std::endl;
      std::cout << "3. Size: " << general_opts.m << " x " << general_opts.n << std::endl;
      std::cout << "4. Rank: " << general_opts.r << std::endl;
      std::cout << "5. Function tolerance: " << optimizer_opts.func_tol << std::endl;
      std::cout << "6. Max. no. of iterations: " << optimizer_opts.max_iter << std::endl;
      std::cout << "7. Display each iteration: " << general_opts.DISPLAY << std::endl;
      
      std::cout << std::endl;
      
    }
  }
  
  return;
}

void parse_options_manually(options_t &general_opts, bool START_FROM_SCRATCH) {
  
  // If starting from scratch, reset the general_opts to default.
  if (START_FROM_SCRATCH) { general_opts.reset(); }
  
  while (general_opts.dataset.length() < 1) {
    std::cout << "Please enter the name of the dataset: ";
    std::cin >> general_opts.dataset;
  }
  while (general_opts.m < 1) {
    std::cout << "Please enter the number of rows (m): ";
    std::cin >> general_opts.m;
  }
  while (general_opts.n < 1) {
    std::cout << "Please enter the number of columns (n): ";
    std::cin >> general_opts.n;
  }
  while (general_opts.r < 1) {
    std::cout << "Please enter the proposed rank (r): ";
    std::cin >> general_opts.r;
  }
  
  //  // Only type again the followings if starting from scratch.
  //  if (START_FROM_SCRATCH) {
  //    std::cout << "Please specify the tolerance threshold (default = 1e-9): ";
  //    std::cin >> general_opts.tol;
  //
  //    std::cout << "Please specify the iteration threshold (default = 300): ";
  //    std::cin >> general_opts.max_iter;
  //  }
  
  return;
}

void outputCommands() {
  
  std::cout << std::endl;
  
  std::cout << "--- [ LIST OF COMMANDS ] ---" << std::endl << std::endl;
  
  std::cout << "[ Dataset-related ]" << std::endl;
  std::cout << "-d=<string>, --dataset=<string>: Read dataset from <name>_r<rank>_{M, W, U0, V0}.bin." << std::endl;
  std::cout << "-f=<string>, --folder=<string>: Set the name of the folder." << std::endl;
  std::cout << "-m=<int>, --rows=<int>: Set the number of rows." << std::endl;
  std::cout << "-n=<int>, --cols=<int>: Set the number of columns." << std::endl;
  std::cout << "-r=<int>, --rank=<int>: Set the proposed rank." << std::endl;
  
  std::cout << std::endl;
  
  std::cout << "[ Solver-related ]" << std::endl;
  std::cout << "-t=<double>, --tol=<double>: Set the function tolerance. (default = 1e-9)" << std::endl;
  std::cout << "-i=<int>, --max_iter=<double>: Set the maximum number of iterations. (default = 300)" << std::endl;
  std::cout << std::endl;
  
  std::cout << "[ Program-related ]" << std::endl;
  std::cout << "--display[=<bool>]: Specify whether to show progress in detail. (default = 1)" << std::endl;
  std::cout << "--debug[=<bool>]: Specify whether to show debug-related information. (default = 0)" << std::endl;
  
  std::cout << std::endl;
  
  std::cout << "[ Miscellaneous]" << std::endl;
  std::cout << "/?, -h, --help: Show a list of available commands." << std::endl;
  
  std::cout << std::endl;
  
  return;
}