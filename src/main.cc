#include <iostream>
#include "../include/token.hh"
#include "../include/scanner.hh"
#include "../include/parser.hh"

int main(){

   parser::Parser pars;
   parser::init(pars, "test.txt");
   token::Token tok;
   std::cout<<parser::parse_expression(pars)->to_string();
   parser::clean_up(pars);
}