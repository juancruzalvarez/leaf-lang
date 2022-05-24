#include "../include/parser.hh"

namespace{
   
   // returns the next token without advancing.
   token::Token peek(parser::Parser pars){
      return scanner::peek(pars.scn);
   }
   // returns the second next token without advancing.
   token::Token peek_2_ahead(parser::Parser pars){
      return scanner::peek_2(pars.scn);
   }
   // returns the nth next token without advancing.
   token::Token peek_n_ahead(parser::Parser pars){
      return scanner::peek_2(pars.scn);
   }
   // advances the next token and returns it
   token::Token advance(parser::Parser pars){
      return scanner::get(pars.scn);
   }
   // advances the next token and returns it
   void unget(parser::Parser pars, token::Token tok){
      scanner::unget(pars.scn, tok);
   }
   // advances next token, and verifies if it maches expected type.
   // if it doesent match adds and error and returns false.
   bool consume(parser::Parser pars, token::TokenType type, std::string msg){
      token::Token tok = scanner::get(pars.scn);
      if(tok.type != type){
         pars.errors.push_back({error::PARSER_ERROR, tok.pos, msg});
         return false;
      }
      return true;
   }
   // if the next token matches type, consume it.
   void consume_optional(parser::Parser pars, token::TokenType type){
      if(scanner::peek(pars.scn).type == type)
         scanner::get(pars.scn);
   }

};

namespace parser{
   ast::Exp* parse_expression(Parser pars){

   }


};