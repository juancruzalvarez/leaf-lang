#include <iostream>
#include "../include/token.hh"
#include "../include/scanner.hh"
#include "../include/parser.hh"
#include "../include/module.hh"

int main(){

   code_gen::Context code_gen_context;
   
   parser::Parser pars;
   parser::init(pars, "test.txt");

   auto parsed = parser::parse_file(pars);


   if(pars.errors.size() > 0){
      std::cout<<"Errors: \n";
      for(const auto& err : pars.errors){
         std::cout<<err.pos.file_name<<"::"<<err.pos.line<<"::"<<err.pos.line_offset<<"::";
         std::cout<<err.msg<<"\n";
      }
   }
  
   for(const auto& module : parsed.modules)
      ast::generate_code(code_gen_context, module);
   
   parser::clean_up(pars);
}