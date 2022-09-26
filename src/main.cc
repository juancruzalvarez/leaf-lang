#include <iostream>
#include "../include/token.hh"
#include "../include/scanner.hh"
#include "../include/parser.hh"

int main(){

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
      std::cout<<"Parsed with no errors!\n";
      std::cout<<"Parsed file:\n";

      for(const auto& mod : parsed.modules)
      {
         std::cout<<"Module: "<<mod->name<<"\n";
         std::cout<<"imports:"<<"\n";
         for(const auto& i : mod->imports)
            std::cout<<i<<"\n";
         std::cout<<"_______________________\n";
         std::cout<<"Type aliases:\n";
         std::cout<<"_______________________\n";
         for(const auto& al : mod->public_declarations->type_aliases){
            std::cout<<"_______________________\npublic  ";
            std::cout<<al->to_string()<<"\n";
         }
         for(const auto& al : mod->private_declarations->type_aliases){
            std::cout<<"_______________________\nprivate  ";
            std::cout<<al->to_string()<<"\n";
         }
         std::cout<<"Types:\n";
         std::cout<<"_______________________\n";
         for(const auto& al : mod->public_declarations->types){
            std::cout<<"_______________________\npublic  ";
            std::cout<<al->to_string()<<"\n";
         }
         for(const auto& al : mod->private_declarations->types){
            std::cout<<"_______________________\nprivate  ";
            std::cout<<al->to_string()<<"\n";
         }
         std::cout<<"type_classes:\n";
         std::cout<<"_______________________\n";
         for(const auto& al : mod->public_declarations->type_classes){
            std::cout<<"_______________________\npublic  ";
            std::cout<<al->to_string()<<"\n";
         }
         for(const auto& al : mod->private_declarations->type_classes){
            std::cout<<"_______________________\nprivate  ";
            std::cout<<al->to_string()<<"\n";
         }
         std::cout<<"consts:\n";
         std::cout<<"_______________________\n";
         for(const auto& al : mod->public_declarations->consts){
            std::cout<<"_______________________\npublic  ";
            std::cout<<al->to_string()<<"\n";
         }
         for(const auto& al : mod->private_declarations->consts){
            std::cout<<"_______________________\nprivate  ";
            std::cout<<al->to_string()<<"\n";
         }
         std::cout<<"consts sets:\n";
         std::cout<<"_______________________\n";
         for(const auto& al : mod->public_declarations->const_sets){
            std::cout<<"_______________________\npublic  ";
            std::cout<<al->to_string()<<"\n";
         }
         for(const auto& al : mod->private_declarations->const_sets){
            std::cout<<"_______________________\nprivate  ";
            std::cout<<al->to_string()<<"\n";
         }
         std::cout<<"functions:\n";
         std::cout<<"_______________________\n";
         for(const auto& al : mod->public_declarations->functions){
            std::cout<<"_______________________\npublic  ";
            std::cout<<al->to_string()<<"\n";
         }
         for(const auto& al : mod->private_declarations->functions){
            std::cout<<"_______________________\nprivate  ";
            std::cout<<al->to_string()<<"\n";
         }
         std::cout<<"methods:\n";
         std::cout<<"_______________________\n";
         for(const auto& al : mod->public_declarations->methods){
            std::cout<<"_______________________\npublic  ";
            std::cout<<al->to_string()<<"\n";
         }
         for(const auto& al : mod->private_declarations->methods){
            std::cout<<"_______________________\nprivate  ";
            std::cout<<al->to_string()<<"\n";
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