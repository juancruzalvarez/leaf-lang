#include <iostream>
#include "../include/token.hh"
#include "../include/scanner.hh"
#include "../include/parser.hh"

int main(){

   parser::Parser pars;
   parser::init(pars, "test.txt");

   auto parsed = parser::parse_file(pars);

   if(pars.errors.size()>0){
      std::cout<<"Errors: \n";
      for(const auto& err : pars.errors){
         std::cout<<err.pos.file_name<<"::"<<err.pos.line<<"::"<<err.pos.line_offset<<"::";
         std::cout<<err.msg<<"\n";
      }
   }
   
   for(const auto& mod : parsed.modules) {
      std::cout<< "Module " << mod->name  << "\n";
      std::cout<< "Public declarations:"  << "\n";
      for(const auto& pd : mod->public_declarations)
         std::cout<< pd->to_string() << "\n";
      std::cout<< "Private declarations:" << "\n";
      for(const auto& pd : mod->private_declarations)
         std::cout<< pd->to_string() << "\n";
   }

   parser::clean_up(pars);
}