#ifndef PARSER_H
#define PARSER_H
#include <vector>
#include <functional>
#include "error.hh"
#include "scanner.hh"
#include "expression.hh"
#include "statement.hh"
#include "declaration.hh"
#include "type.hh"
#include "module.hh"

namespace parser{
   
   struct Parser{
      scanner::Scanner scn;
      error::Errors errors;
   };

   struct ParsedFile{
      std::vector<ast::Module*> modules;
   };
   
   

   void init(Parser &pars, const char* file_path);

   ParsedFile parse_file(Parser &pars);
   ast::Module *parse_module(Parser &pars);
   ast::Declaration *parse_declaration(Parser &pars);
   ast::Declaration *parse_fn_declaration(Parser &pars);
   ast::Declaration *parse_type_declaration(Parser &pars);
   ast::Declaration *parse_type_class_declaration(Parser &pars);
   ast::Declaration *parse_const_declaration(Parser &pars);
   ast::Statement *parse_statement(Parser &pars);
   ast::Statement *parse_block_statement(Parser &pars);
   ast::Statement *parse_if_statement(Parser &pars);
   ast::Statement *parse_while_statement(Parser &pars);
   ast::Statement *parse_do_while_statement(Parser &pars);
   ast::Statement *parse_for_statement(Parser &pars);

   ast::TypeClass *parse_type_class(Parser &pars);
   std::vector<ast::Trait *> parse_has(Parser &pars);
   std::vector<ast::Trait *> parse_trait(Parser &pars);


   ast::Type* parse_type(Parser &pars);
   ast::Type* parse_type_union(Parser &pars);
   ast::Type* parse_struct_type(Parser &pars);
   ast::Type* parse_fn_type(Parser &pars);
   std::vector<ast::VariableDeclaration*> parse_var_declaration(Parser &pars);

   ast::Exp* parse_expression(Parser &pars);
   ast::Exp* parse_assignment(Parser &pars);
   ast::Exp* parse_ternary(Parser &pars);
   ast::Exp* parse_binary(Parser &pars, int min_precedence);
   ast::Exp* parse_unary(Parser &pars);
   ast::Exp* parse_call_or_access(Parser &pars);
   ast::Exp* parse_post_inc(Parser &pars);
   ast::Exp* parse_primary(Parser &pars);
   void clean_up(Parser &pars);


   template<typename T>
   std::vector<T> parse_list(Parser &pars, token::TokenType separator, std::function<T(Parser&)> parse_func);
   template<typename T>
   std::vector<T> parse_list(Parser &pars, token::TokenType separator, std::function<T(Parser &)> parse_func, int count);

};
#endif