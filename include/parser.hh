#ifndef PARSER_H
#define PARSER_H
#include <vector>
#include <functional>
#include "error.hh"
#include "scanner.hh"
#include "expression.hh"

namespace parser{
   
   struct Parser{
      scanner::Scanner scn;
      error::Errors errors;
   };

   ast::Exp* parse_expression(Parser &pars);
   ast::Exp* parse_assignment(Parser &pars);
   ast::Exp* parse_ternary(Parser &pars);
   ast::Exp* parse_lor(Parser &pars);
   ast::Exp* parse_land(Parser &pars);
   ast::Exp* parse_bitwise(Parser &pars);
   ast::Exp* parse_equality(Parser &pars);
   ast::Exp* parse_comp(Parser &pars);
   ast::Exp* parse_shift(Parser &pars);
   ast::Exp* parse_term(Parser &pars);
   ast::Exp* parse_factor(Parser &pars);
   ast::Exp* parse_unary(Parser &pars);
   ast::Exp* parse_call_or_access(Parser &pars);
   ast::Exp* parse_post_inc(Parser &pars);
   ast::Exp* parse_primary(Parser &pars);
   template<typename T>
   std::vector<T> parse_list(Parser &pars, token::TokenType separator, std::function<T(Parser&)> parse_func);
};
#endif