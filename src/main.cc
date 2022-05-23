#include <iostream>
#include "../include/token.hh"
#include "../include/scanner.hh"

int main(){
   scanner::Scanner scn;
   scanner::init(scn, "test.txt");
   token::Token tok;
   while( (tok = scanner::get(scn)).type != token::TEOF && tok.type != token::BAD_TOKEN){
      std::cout<<"Token:\n";
      std::cout<<"Type: "<<token::type_to_str(tok.type)<<"\n";
      std::cout<<"Val: "<<tok.val<<"\n";
   }
   std::cout<<"Last token:"<<token::type_to_str(tok.type);
   scanner::clean_up(scn);
}