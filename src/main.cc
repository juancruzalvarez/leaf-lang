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
   }else{
      std::cout<<"Parsed with no errors!\n";
      std::cout<<"Parsed file:\n";
      std::cout<<"_______________________\n";
      std::cout<<"Type aliases:\n";
      std::cout<<"_______________________\n";
      for(const auto& al : parsed.type_aliases){
         std::cout<<"_______________________\n";
         std::cout<<al->to_string()<<"\n";
      }
      std::cout<<"_______________________\n";
      std::cout<<"Types:\n";
      std::cout<<"_______________________\n";
      for(const auto& type : parsed.types){
         std::cout<<"_______________________\n";
         std::cout<<type->to_string()<<"\n";
      }
      std::cout<<"_______________________\n";
      std::cout<<"Type classes:\n";
      std::cout<<"_______________________\n";
      for(const auto& type_class : parsed.type_classes){
         std::cout<<"_______________________\n";
         std::cout<<type_class->to_string()<<"\n";
      }
      std::cout<<"_______________________\n";
      std::cout<<"Consts:\n";
      std::cout<<"_______________________\n";
      for(const auto& type : parsed.consts){
         std::cout<<"_______________________\n";
         std::cout<<type->to_string()<<"\n";
      }
      std::cout<<"_______________________\n";
      std::cout<<"Const sets:\n";
      std::cout<<"_______________________\n";
      for(const auto& type : parsed.const_sets){
         std::cout<<"_______________________\n";
         std::cout<<type->to_string()<<"\n";
      }
      std::cout<<"_______________________\n";
      std::cout<<"Functions:\n";
      std::cout<<"_______________________\n";
      for(const auto& fn : parsed.functions){
         std::cout<<"_______________________\n";
         std::cout<<fn->to_string()<<"\n";
      }
      std::cout<<"_______________________\n";
      std::cout<<"Methods:\n";
      std::cout<<"_______________________\n";
      for(const auto& meth : parsed.methods){
         std::cout<<"_______________________\n";
         std::cout<<meth->to_string()<<"\n";
      }

   }
   
   parser::clean_up(pars);
}