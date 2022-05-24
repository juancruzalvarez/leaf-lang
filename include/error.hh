#ifndef ERROR_H
#define ERROR_H
#include <vector>
#include "position.hh"

namespace error{
   enum ErrorType{
      SCANNER_ERROR,
      PARSER_ERROR,
      SYSTEM_ERROR
   };
   struct Error{
      ErrorType type;
      Position pos;
      std::string msg;
   };
   
   typedef std::vector<Error> Errors; 
};

#endif