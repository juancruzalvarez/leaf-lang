#include <vector>
#include "../include/parser.hh"
#include "../include/expression.hh"
#include "../include/scanner.hh"

namespace
{

   // returns the next token without advancing.
   token::Token peek(parser::Parser &pars)
   {
      return scanner::peek(pars.scn);
   }
   // returns the second next token without advancing.
   token::Token peek_2_ahead(parser::Parser &pars)
   {
      return scanner::peek_2(pars.scn);
   }
   // returns the nth next token without advancing.
   token::Token peek_n_ahead(parser::Parser &pars)
   {
      return scanner::peek_2(pars.scn);
   }
   // advances the next token and returns it
   token::Token advance(parser::Parser &pars)
   {
      return scanner::get(pars.scn);
   }
   // advances the next token and returns it
   void unget(parser::Parser &pars, token::Token tok)
   {
      scanner::unget(pars.scn, tok);
   }
   // advances next token, and verifies if it maches expected type.
   // if it doesent match adds an error and returns false.
   bool consume(parser::Parser &pars, token::TokenType type, std::string msg)
   {
      token::Token tok = advance(pars);
      if (tok.type != type)
      {
         pars.errors.push_back({error::PARSER_ERROR, tok.pos, msg});
         return false;
      }
      return true;
   }
   // if the next token matches type, consume it.
   void consume_optional(parser::Parser &pars, token::TokenType type)
   {
      if (peek(pars).type == type)
         advance(pars);
   }
   // if next token matches, advance it and store it in @tok and return true,
   // else return false.
   bool match(parser::Parser &pars, token::TokenType type, token::Token &tok)
   {
      token::Token aux = peek(pars);
      if (aux.type == type)
      {
         tok = advance(pars);
         return true;
      }
      return false;
   }
   // if next token matches any of the tipes, advance it and store it in @tok and return true,
   // else return false.
   bool match(parser::Parser &pars, std::vector<token::TokenType> types, token::Token &tok)
   {
      token::Token aux = peek(pars);
      for (const auto t : types)
      {
         if (aux.type == t)
         {
            tok = advance(pars);
            return true;
         }
      }
      return false;
   }

};

using namespace token;
namespace parser{
   

   ast::Exp* parse_expression(Parser &pars){
      return parse_assignment(pars);
   }

   ast::Exp* parse_assignment(Parser &pars){
      ast::Exp *lhs = parse_ternary(pars);
      Token tok;
      if(match(pars, {ASSIGN, ADD_ASSIGN, OR_ASSIGN, AND_ASSIGN, SUB_ASSIGN, XOR_ASSIGN, MOD_ASSIGN}, tok)){
         lhs = new ast::BinaryExp{tok.type, lhs, parse_assignment(pars)};
      }
      return lhs;
   }

   ast::Exp* parse_ternary(Parser &pars){
      ast::Exp* exp = parse_lor(pars);
      Token tok;
      if (match(pars, TERNARY, tok)) {
         ast::Exp* true_branch = parse_ternary(pars);
         if (!consume(pars, COLON, "Unexpected token, Expected ':'"))
               return nullptr;

         ast::Exp* false_branch = parse_ternary(pars);
         return new ast::TernaryExp{ exp, true_branch, false_branch};
      }
      return exp;
   }

   ast::Exp* parse_lor(Parser &pars){
      ast::Exp* exp = parse_land(pars);
      Token tok;
      while (match(pars, LOR, tok)) {
        exp = new ast::BinaryExp{ LOR, exp, parse_land(pars) };
      }
      return exp;
   }

   ast::Exp* parse_land(Parser &pars){
      ast::Exp* exp = parse_bitwise(pars);
      Token tok;
      while (match(pars, LAND, tok)) {
        exp = new ast::BinaryExp{ LAND, exp, parse_bitwise(pars) };
      }
      return exp;
   }

   ast::Exp* parse_bitwise(Parser &pars){
      ast::Exp* exp = parse_equality(pars);
      Token tok;
      while (match(pars, {AMP, OR, XOR}, tok)) {
        exp = new ast::BinaryExp{ tok.type, exp, parse_equality(pars) };
      }
      return exp;
   }

   ast::Exp* parse_equality(Parser &pars){
      ast::Exp* exp = parse_comp(pars);
      Token tok;
      while (match(pars, {EQL, NEQL}, tok)) {
        exp = new ast::BinaryExp{ tok.type, exp, parse_comp(pars) };
      }
      return exp;
   }

   ast::Exp* parse_comp(Parser &pars){
      ast::Exp* exp = parse_shift(pars);
      Token tok;
      while (match(pars, {GTR, GEQ, LSS, LEQ}, tok)) {
        exp = new ast::BinaryExp{ tok.type, exp, parse_shift(pars) };
      }
      return exp;
   }

   ast::Exp* parse_shift(Parser &pars){
      ast::Exp* exp = parse_term(pars);
      Token tok;
      while (match(pars, {SHR, SHL}, tok)) {
        exp = new ast::BinaryExp{ tok.type, exp, parse_term(pars) };
      }
      return exp;
   }

   ast::Exp* parse_term(Parser &pars){
      ast::Exp* exp = parse_factor(pars);
      Token tok;
      while (match(pars, {ADD, SUB}, tok)) {
        exp = new ast::BinaryExp{ tok.type, exp, parse_factor(pars) };
      }
      return exp;
   }

   ast::Exp* parse_factor(Parser &pars){
      ast::Exp* exp = parse_unary(pars);
      Token tok;
      while (match(pars, {STAR, DIV, MOD}, tok)) {
        exp = new ast::BinaryExp{ tok.type, exp, parse_unary(pars) };
      }
      return exp;
   }

   ast::Exp* parse_unary(Parser &pars){
      Token tok;
      if (match(pars, { NOT, SUB, INC, DEC, NEG}, tok)) {
         ast::Exp* exp = parse_unary(pars);
         if (tok.type == DEC || tok.type == INC) {
            return new ast::BinaryExp{tok.type == INC ? ADD_ASSIGN : SUB_ASSIGN, exp, new ast::LiteralExp{"1"}};
         } else {
            return new ast::UnaryExp{tok.type, exp};
         }
      }
      return parse_call_or_access(pars);
   }

   ast::Exp* parse_call_or_access(Parser &pars){
      ast::Exp* exp = parse_post_inc(pars);
      Token tok;
      while(match(pars, {DOT, LPARENTESIS}, tok)){
         if(tok.type == DOT){
            //member access
            tok = advance(pars);
            if(tok.type != IDENTIFIER){
               pars.errors.push_back({error::PARSER_ERROR, tok.pos, "Unexpected token, expected identifier."});
               return new ast::InvalidExp{};
            }else{
               exp = new ast::MemberAccessExp(exp, tok.val);
            }
         }else{
            //function call
            std::vector<ast::Exp*> args;
            if(peek(pars).type == RPARENTESIS){
               args = {};
            }else{
               args = parse_list<ast::Exp*>(pars, COMMA, parse_expression);
            }
            consume(pars, RPARENTESIS, "Unexpected token, expeted ')'.");
            exp = new ast::FunctionCallExp(exp,args);
         }
      }
      return exp;
   }
   ast::Exp* parse_post_inc(Parser &pars);
   ast::Exp* parse_primary(Parser &pars);

   template<typename T>
   std::vector<T> parse_list(Parser &pars, token::TokenType separator, std::function<T(Parser&)> parse_func){
      std::vector<T> list{};
      Token tok;
      do{
         list.push_back(parse_func(pars));
      }while(match(pars, separator, tok));
      return list;
   }
};
