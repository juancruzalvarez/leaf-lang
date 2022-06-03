#include <iostream>
#include "../include/token.hh"
#include "../include/scanner.hh"
#include "../include/parser.hh"

int main(){

   parser::Parser pars;
   parser::init(pars, "test.txt");
   token::Token tok;
         std::cout<<"before;!!!!!!!!!";

   auto vars = parser::parse_statement(pars);
            std::cout<<"AFTERR;!!!!!!!!!";

  /* for(const auto& var : vars ){
      printf( var->to_string());
   }*/
   if (vars)
      std::cout<<"before;!";
      std::cout<< vars->to_string()<<"\n";
      std::cout<<"after!:!";
   if(pars.errors.size()>0){
      std::cout<<"Errors: \n";
      for(const auto& err : pars.errors){
         std::cout<<err.pos.file_name<<"::"<<err.pos.line<<"::"<<err.pos.line_offset<<"::";
         std::cout<<err.msg<<"\n";
      }
   }
   
   parser::clean_up(pars);
}