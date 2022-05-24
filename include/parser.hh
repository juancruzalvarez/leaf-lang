#ifndef PARSER_H
#define PARSER_H
#include "error.hh"
#include "scanner.hh"
#include "expression.hh"
namespace parser{
   
   struct Parser{
      scanner::Scanner scn;
      error::Errors errors;
   };

   ast::Exp* parse_expression(Parser parser);

};
#endif