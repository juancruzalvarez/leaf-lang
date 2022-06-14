#include <vector>
#include "../include/operator.hh"
#include "../include/parser.hh"
#include "../include/expression.hh"
#include "../include/statement.hh"
#include "../include/type.hh"
#include "../include/scanner.hh"

namespace
{
   void add_error(parser::Parser &pars, std::string msg)
   {
      pars.errors.push_back(error::Error{error::PARSER_ERROR, pars.scn.current_position, msg});
   }
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
   token::Token peek_n_ahead(parser::Parser &pars, int n)
   {
      return scanner::peek_n(pars.scn, n);
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
   // verifies if it maches expected type.
   // if it does advances parser.
   // if it doesent match adds an error and returns false.
   bool consume(parser::Parser &pars, token::TokenType type, std::string msg)
   {
      token::Token tok = peek(pars);
      if (tok.type != type)
      {
         add_error(pars, msg);

         return false;
      }
      else
      {
         advance(pars);

         return true;
      }
   }
   // if the next token matches type, consume it.
   void consume_optional(parser::Parser &pars, token::TokenType type)
   {
      if (peek(pars).type == type)
         advance(pars);
   }
   // if next token matches, advance it, store it in @tok and return true,
   // else return false.
   bool match(parser::Parser &pars, token::TokenType type, token::Token &tok)
   {
      tok = peek(pars);
      if (tok.type == type)
      {
         advance(pars);
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
namespace parser
{

   void init(Parser &pars, const char *file_path)
   {
      scanner::init(pars.scn, file_path);
      pars.errors = {};
   }
   
   void clean_up(Parser &pars)
   {
      scanner::clean_up(pars.scn);
      pars.errors = {};
   }
 
   ParsedFile parse_file(Parser &pars){
      ParsedFile parsed;
      ast::Declaration* current_declaration;
      while(peek(pars).type != TEOF){
         current_declaration = parse_declaration(pars);
         switch(current_declaration->get_kind()){
            case ast::DECLARATION_TYPE:{
               auto type_dec = dynamic_cast<ast::TypeDeclaration*>(current_declaration);
               auto type = type_dec->type;
               if(type->get_kind() == ast::TYPE_SIMPLE){
                  parsed.type_aliases.push_back(type_dec);
               }else{
                  parsed.types.push_back(type_dec);
               }
               break;
            }
            case ast::DECLARATION_FN:{
               auto fn_dec = dynamic_cast<ast::FunctionDeclaration*>(current_declaration);
               if(fn_dec->is_method){
                  parsed.methods.push_back(fn_dec);
               }else{
                  parsed.functions.push_back(fn_dec);
               }
               break;
            }
            case ast::DECLARATION_TYPE_CLASS:{
               parsed.type_classes.push_back(dynamic_cast<ast::TypeClassDeclaration*>(current_declaration));
               break;
            }

         }
         consume(pars, SEMICOLON, "Unexpected token, expected ;.");
      }
      return parsed;
   }


   ast::Declaration *parse_declaration(Parser &pars){
      switch (peek(pars).type)
      {
      case FN:
         return parse_fn_declaration(pars);
      case TYPE:
         return parse_type_declaration(pars);
      case CLASS:
         return parse_type_class_declaration(pars);
      default:
         return new ast::InvalidDeclaration{};
      }
   }

   ast::Declaration *parse_fn_declaration(Parser &pars){
      consume(pars, FN, "Expected fn.");
      Token tok;
      std::string name;
      std::vector<std::string> template_vars;
      bool is_method = false;
      ast::NameAndType* method_name_and_type;
      std::vector<ast::NameAndType*> args;
      ast::Type* return_type;
      bool is_short = false;
      ast::Statement* body;
      if (match(pars, LSS, tok)) // if match < then read template variables. fn<a,b>
      {
         // TODO; make this check for errors.
         template_vars = parse_list<std::string>(
             pars,
             COMMA,
             [](Parser &pars) -> std::string
             {
                return advance(pars).val;
             });
         consume(pars, GTR, "Unclosed < in function type.");
      }
      if (match(pars, LPARENTESIS, tok)){
         is_method = true;
         std::string name;
         ast::Type* type;
         if(!match(pars, IDENTIFIER, tok)){
            add_error(pars, "Expected identifier.");
            return new ast::InvalidDeclaration{};
         }
         name = tok.val;
         if(! consume(pars, DBL_COLON, "Expected :: .")){
            return new ast::InvalidDeclaration{};
         }
         type = parse_type(pars);
         method_name_and_type = new ast::NameAndType{name, type};
         consume(pars, RPARENTESIS, "Unclosed (.");
      }
      if(!match(pars, IDENTIFIER, tok)){
         add_error(pars, "Expected identifier.");
         return new ast::InvalidDeclaration{};
      }
      name = tok.val;
      bool has_parentesis = match(pars, LPARENTESIS, tok);
      std::vector<std::vector<ast::NameAndType*>> args_list;
      if(has_parentesis && peek(pars).type == RPARENTESIS){
         args_list = {};
      }else{      
         args_list = parse_list<std::vector<ast::NameAndType*>>(
            pars,
            COMMA,
            [](Parser &pars) -> std::vector<ast::NameAndType*>{
               auto names = parse_list<std::string>(
                  pars,
                  COMMA,
                  [](Parser &pars) -> std::string
                  {
                     return advance(pars).val;
                  }
               );
               consume(pars, DBL_COLON, "Expected :: .");
               auto type = parse_type(pars);
               std::vector<ast::NameAndType*> ret;
               for(const auto& name : names){
                  ret.push_back(new ast::NameAndType{name, type});
               }
               return ret;
            }
         );
      }
   
      for(const auto& new_args : args_list){
         args.insert(args.end(), new_args.begin(), new_args.end());
      }

      if(has_parentesis){
         consume(pars, RPARENTESIS, "Unclosed (.");
      }
      if(match(pars, ARROW, tok)){
         return_type = parse_type(pars);
      }else{
         return_type = new ast::SimpleType("void");
      }
      if(match(pars, FAT_ARROW, tok)){
         is_short = true;
         body = new ast::ExpressionStatement{parse_expression(pars)};
      }else{
         body = parse_block_statement(pars);
      }
      return new ast::FunctionDeclaration{name, template_vars, is_method, method_name_and_type, args, return_type, is_short, body};
   }

   ast::Declaration *parse_type_declaration(Parser &pars){
      consume(pars, TYPE, "Expected type.");
      std::string name = advance(pars).val;
      consume(pars, DBL_COLON, "Unexpected token, expected :: .");
      ast::Type* type = parse_type(pars);
      return new ast::TypeDeclaration{name, type};
   }

   ast::Declaration *parse_type_class_declaration(Parser &pars){
      consume(pars, CLASS, "Expected class.");
      std::string name = advance(pars).val;
      consume(pars, DBL_COLON, "Unexpected token, expected ::.");
      ast::TypeClass* type_class = parse_type_class(pars);
      return new ast::TypeClassDeclaration{name, type_class};
   }

   ast::Statement *parse_statement(Parser &pars)
   {

      switch (peek(pars).type)
      {
      case IF:
         return parse_if_statement(pars);
      case DO:
         return parse_do_while_statement(pars);
      case FOR:
         return parse_for_statement(pars);
      case WHILE:
         return parse_while_statement(pars);
      case LBRACE:
         return parse_block_statement(pars);
      case BREAK:
         advance(pars);
         return new ast::SimpleStatement(ast::STATEMENT_BREAK);
      case CONTINUE:
         advance(pars);
         return new ast::SimpleStatement(ast::STATEMENT_CONTINUE);
      case RETURN:{
         advance(pars);
         if(peek(pars).type == SEMICOLON){
            return new ast::ReturnStatement{new ast::EmptyExp{}};
         }else{
            return new ast::ReturnStatement{parse_expression(pars)};
         }
      }
      default:
      {
         auto second = peek_2_ahead(pars).type;
         if (second == COMMA || second == DBL_COLON || second == DEFINE)
         {
            return new ast::VarDeclarationStatement{parse_var_declaration(pars)};
         }
         else
         {
            return new ast::ExpressionStatement{parse_expression(pars)};
         }
      }
      }
   }

   ast::Statement *parse_block_statement(Parser &pars)
   {
      consume(pars, LBRACE, "Expected {");
      std::vector<ast::Statement *> statements;
      while (peek(pars).type != RBRACE && peek(pars).type != TEOF)
      {
         statements.push_back(parse_statement(pars));
         if (pars.scn.last_advanced_token.type == RBRACE){
            consume_optional(pars, SEMICOLON);
         }else{
            consume(pars, SEMICOLON, "Unexpected token, expected ;.");
         }
      }
      consume(pars, RBRACE, "Unclosed { in block.");
      return new ast::BlockStatement{statements};
   }

   ast::Statement *parse_if_statement(Parser &pars)
   {

      consume(pars, IF, "Unexpected token, expected IF."); // is this needed?
      Token tok;
      ast::Exp *condition;
      ast::Statement *true_branch, *false_branch = nullptr;
      condition = parse_expression(pars);
      true_branch = parse_block_statement(pars);
      if (match(pars, ELSE, tok))
      {
         if (peek(pars).type == IF)
         {
            false_branch = parse_if_statement(pars);
         }
         else
         {
            false_branch = parse_block_statement(pars);
         }
      }
      return new ast::IfStatement{condition, true_branch, false_branch};
   }

   ast::Statement *parse_while_statement(Parser &pars)
   {
      consume(pars, WHILE, "Unexpected token, expected while."); // is this needed?
      Token tok;
      ast::Exp *condition;
      ast::Statement *statement;
      condition = parse_expression(pars);
      statement = parse_block_statement(pars);
      return new ast::WhileStatement{condition, statement, false};
   }

   ast::Statement *parse_do_while_statement(Parser &pars)
   {
      consume(pars, DO, "Unexpected token, expected do."); // is this needed?
      Token tok;
      ast::Exp *condition;
      ast::Statement *statement;
      statement = parse_block_statement(pars);
      consume(pars, WHILE, "Unexpected token, expected while."); // is this needed?
      condition = parse_expression(pars);
      return new ast::WhileStatement{condition, statement, true};
   }

   // TODO: MAKE true and false keywords?? or make bool literal token type.
   ast::Statement *parse_for_statement(Parser &pars)
   {
      consume(pars, FOR, "Unexpected token, expected for.");

      Token tok;
      std::vector<ast::Statement *>before, after;
      ast::Exp *condition;
      ast::Statement *statement;

      // parse before:
      if (match(pars, SEMICOLON, tok))
      {
         before = {};
      }
      else
      {
         before = parse_list<ast::Statement*>(pars, COMMA, parse_statement);
         consume(pars, SEMICOLON, "Unexpected token, expected ;.");
      }

      // parse condition
      if (match(pars, SEMICOLON, tok))
      {
         // if condition is skiped make it true.
         condition = new ast::LiteralExp{ast::LIT_NUM, "d1"}; // TODO: this sjhoulñd be bool literal.
      }
      else
      {
         condition = parse_expression(pars);
         consume(pars, SEMICOLON, "Unexpected token, expected ;.");
      }

      // parse after
      if (match(pars, LBRACE, tok))
      {
         after = {};
         unget(pars, tok); // returns { to the parser, so that it can be consumed in parse_block
      }
      else
      {
         after = parse_list<ast::Statement*>(pars,COMMA, parse_statement);
      }

      statement = parse_block_statement(pars);

      return new ast::ForStatement{condition, before, after, statement};
   }

   ast::TypeClass *parse_type_class(Parser &pars)
   {
      std::string type_var;
      std::vector<std::string> is;
      std::vector<ast::Trait *> has;
      Token tok;
      if (!match(pars, IDENTIFIER, tok))
      {
         add_error(pars, "Expected identifier in typeclass.");
      }
      type_var = tok.val;
      do
      {

         switch (advance(pars).type)
         {
         case IS:
         {
            if (!match(pars, IDENTIFIER, tok))
            {
               add_error(pars, "Expected identififer in is.");
               return nullptr;
            }
            is.push_back(tok.val);
            break;
         }
         case HAS:
         {
            std::vector<ast::Trait *> new_has = parse_has(pars);

            has.insert(has.end(), new_has.begin(), new_has.end());
            break;
         }
         default:
         {
            pars.errors.push_back(error::Error{error::PARSER_ERROR, pars.scn.current_position, "Expected is or has."});
            return nullptr;
         }
         }
      } while (match(pars, AMP, tok));
      return new ast::TypeClass{type_var, is, has};
   }

   std::vector<ast::Trait *> parse_has(Parser &pars)
   {
      if (!consume(pars, LBRACE, "Unexpected token, expected {"))
      {
         return {};
      }
      std::vector<ast::Trait *> has;
      while (peek(pars).type != RBRACE && peek(pars).type != TEOF)
      {
         std::vector<ast::Trait *> new_traits = parse_trait(pars);
         has.insert(has.end(), new_traits.begin(), new_traits.end());
         consume(pars, SEMICOLON, "Unexpected token, expected ;.");
      }
      consume(pars, RBRACE, "Unclosed { in has.");
      return has;
   }

   std::vector<ast::Trait *> parse_trait(Parser &pars)
   {
      Token tok;
      if (peek(pars).type != IDENTIFIER)
      {
         add_error(pars, "Expected identifier.");
         return {};
      }
      // TODO: this is shit:
      bool parsing_member = peek_2_ahead(pars).type == COMMA || peek_2_ahead(pars).type == DBL_COLON;
      if (!parsing_member)
      {
         std::string name = advance(pars).val;
         std::vector<ast::Type *> args;
         ast::Type *return_type;
         if (match(pars, LPARENTESIS, tok))
         {
            if (match(pars, RPARENTESIS, tok))
            {
               args = {};
            }
            else
            {
               args = parse_list<ast::Type *>(pars, COMMA, parse_type);
               consume(pars, RPARENTESIS, "Unclosed ( in function type.");
            }
         }
         else
         {
            args = parse_list<ast::Type *>(pars, COMMA, parse_type);
         }
         if (match(pars, ARROW, tok))
         {
            return_type = parse_type(pars);
         }
         else
         {
            // TODO: think what to call no type. and how to represent it.
            return_type = new ast::SimpleType{"void"};
         }
         return {new ast::MethodTrait{name, args, return_type}};
      }
      else
      {
         // parsing member.
         std::vector<ast::Trait *> ret;
         std::vector<std::string> names = parse_list<std::string>(pars, COMMA, [](Parser &pars)
                                                                  { return advance(pars).val; });
         consume(pars, DBL_COLON, "Unexpected token, Expected ::.");
         ast::Type *type = parse_type(pars);

         for (const auto &name : names)
         {
            ret.push_back(new ast::MemberTrait{name, type});
         }
         return ret;
      }
   }

   ast::Type *parse_type(Parser &pars)
   {
      return parse_type_union(pars);
   }

   ast::Type *parse_type_union(Parser &pars)
   {
      std::vector<ast::Type *> types;
      types = parse_list<ast::Type *>(
          pars,
          OR,
          [](Parser &pars) -> ast::Type *
          {
             Token tok;
             ast::Type *t;
             bool is_const = match(pars, CONST, tok);
             switch (peek(pars).type)
             {
             case IDENTIFIER:
             {
                t = new ast::SimpleType(advance(pars).val);
                break;
             }
             case STRUCT:
             {
                t = parse_struct_type(pars);
                break;
             }
             case FN:
             {
                t = parse_fn_type(pars);
                break;
             }
             default:
             {
                t = new ast::BadType();
                break;
             }
             }
             t->is_pointer = match(pars, STAR, tok);
             t->is_const =is_const;
             return t;
          });

      if (types.size() == 1)
      {
         return types[0];
      }
      else
      {
         return new ast::UnionType{types};
      }
   }

   std::vector<ast::VariableDeclaration *> parse_var_declaration(Parser &pars)
   {
      token::Token tok;
      std::vector<std::string> names;
      ast::Type *type;
      std::vector<ast::Exp *> initial_vals;
      bool has_initial_vals = false;
      // TODO: make this check for errors??
      names = parse_list<std::string>(
          pars,
          COMMA,
          [](Parser &pars) -> std::string
          {
             return advance(pars).val;
          });

      if (match(pars, DEFINE, tok))
      {
         type = new ast::SimpleType("");
         has_initial_vals = true;
      }
      else if (match(pars, DBL_COLON, tok))
      {
         type = parse_type(pars);
         if (match(pars, ASSIGN, tok))
         {
            has_initial_vals = true;
         }
      }
      else
      {
         // TODO: check what to return and what error to throw.
      }
      if (has_initial_vals)
      {
         initial_vals = parse_list<ast::Exp *>(
             pars,
             COMMA,
             parse_expression,
             names.size()
         );
      }
      std::vector<ast::VariableDeclaration *> res = {};
      for (int i = 0; i < names.size(); i++)
      {
         res.push_back(
             new ast::VariableDeclaration{
                 type,
                 names[i],
                 has_initial_vals,
                 has_initial_vals ? initial_vals[i] : nullptr
               }
            );
      }
      return res;
   }

   ast::Type *parse_struct_type(Parser &pars)
   {
      Token tok;
      std::vector<std::string> template_vars;
      std::vector<ast::VariableDeclaration *> vars;
      consume(pars, STRUCT, "Invalid struct type, expected struct.");
      if (match(pars, LSS, tok)) // if match < then read template variables. ::struct<a,b>{}
      {
         // TODO; make this check for errors.
         template_vars = parse_list<std::string>(
             pars,
             COMMA,
             [](Parser &pars) -> std::string
             {
                return advance(pars).val;
             });
         consume(pars, GTR, "Unclosed < in struct type.");
      }
      consume(pars, LBRACE, "Unexpected token, expected {.");
      // NOTE: this makes semicolons not optional in struct type.
      while (peek(pars).type != RBRACE)
      {
         std::cout << "Hola hola\n";
         auto vec = parse_var_declaration(pars);
         consume(pars, SEMICOLON, "Unexpected token, expected ;.");
         vars.insert(vars.end(), vec.begin(), vec.end());
      }

      consume(pars, RBRACE, "Unclosed { in struct type.");
      return new ast::StructType{template_vars, vars};
   }

   ast::Type *parse_fn_type(Parser &pars)
   {
      Token tok;
      std::vector<std::string> template_vars;
      std::vector<ast::Type *> args;
      ast::Type *return_type;
      consume(pars, FN, "Invalid function type, expected fn.");
      if (match(pars, LSS, tok)) // if match < then read template variables. fn<a,b>
      {
         // TODO; make this check for errors.
         template_vars = parse_list<std::string>(
             pars,
             COMMA,
             [](Parser &pars) -> std::string
             {
                return advance(pars).val;
             });
         consume(pars, GTR, "Unclosed < in function type.");
      }

      if (match(pars, LPARENTESIS, tok))
      {
         if (match(pars, RPARENTESIS, tok))
         {
            args = {};
         }
         else
         {
            args = parse_list<ast::Type *>(pars, COMMA, parse_type);
            consume(pars, RPARENTESIS, "Unclosed ( in function type.");
         }
      }
      else
      {
         args = parse_list<ast::Type *>(pars, COMMA, parse_type);
      }

      if (match(pars, ARROW, tok))
      {
         return_type = parse_type(pars);
      }
      else
      {
         // TODO: think what to call no type. and how to represent it.
         return_type = new ast::SimpleType{"void"};
      }
      return new ast::FunctionType{template_vars, args, return_type};
   }

   ast::Exp *parse_expression(Parser &pars)
   {
      return parse_assignment(pars);
   }

   ast::Exp *parse_assignment(Parser &pars)
   {
      ast::Exp *lhs = parse_ternary(pars);
      Token tok;
      if (match(pars, {ASSIGN, ADD_ASSIGN, OR_ASSIGN, AND_ASSIGN, SUB_ASSIGN, XOR_ASSIGN, MOD_ASSIGN}, tok))
      {
         lhs = new ast::BinaryExp{operators::get_binary_operator(tok.type), lhs, parse_assignment(pars)};
      }
      return lhs;
   }

   ast::Exp *parse_ternary(Parser &pars)
   {
      ast::Exp *exp = parse_binary(pars, -1);
      Token tok;
      if (match(pars, TERNARY, tok))
      {
         ast::Exp *true_branch = parse_ternary(pars);
         if (!consume(pars, COLON, "Unexpected token, Expected ':'"))
            return nullptr;

         ast::Exp *false_branch = parse_ternary(pars);
         return new ast::TernaryExp{exp, true_branch, false_branch};
      }
      return exp;
   }

   ast::Exp *parse_binary(Parser &pars, int min_precedence)
   {
      ast::Exp *exp = parse_unary(pars);
      Token op;
      while (match(pars, {LOR, LAND, AMP, OR, XOR, EQL, NEQL, GTR, GEQ, LSS, LEQ, SHR, SHL, ADD, SUB, STAR, DIV, MOD}, op))
      {

         operators::OperatorInfo op_info = operators::get_operator_info(op.type);
         if (op_info.precedence < min_precedence)
         {
            unget(pars, op);
            break;
         }
         int next_min_precedence = op_info.r_associative ? op_info.precedence : op_info.precedence + 1;
         ast::Exp *rhs = parse_binary(pars, next_min_precedence);
         exp = new ast::BinaryExp(operators::get_binary_operator(op.type), exp, rhs);
      }
      return exp;
   }

   ast::Exp *parse_unary(Parser &pars)
   {
      Token tok;
      if (match(pars, {NOT, SUB, INC, DEC, NEG, STAR, AMP}, tok))
      {
         ast::Exp *exp = parse_unary(pars);
         if (tok.type == DEC || tok.type == INC)
         {
            return new ast::BinaryExp{tok.type == INC ? operators::OPERATOR_ADD_ASSIGN : operators::OPERATOR_SUB_ASSIGN, exp, new ast::LiteralExp{ast::LIT_NUM, "1"}};
         }
         else
         {
            return new ast::UnaryExp{operators::get_unary_operator(tok.type), exp};
         }
      }
      return parse_call_or_access(pars);
   }

   ast::Exp *parse_call_or_access(Parser &pars)
   {
      ast::Exp *exp = parse_post_inc(pars);
      Token tok;
      while (match(pars, {DOT, LPARENTESIS, LBRACE, LBRACKET}, tok))
      {
         switch(tok.type){
            case DOT:{
               // member access
               tok = advance(pars);
               if (tok.type != IDENTIFIER)
               {
                  pars.errors.push_back({error::PARSER_ERROR, tok.pos, "Unexpected token, expected identifier."});
                  return new ast::InvalidExp{};
               }
               else
               {
                  exp = new ast::MemberAccessExp(exp, tok.val);
               }
               break;   
            }
            case LPARENTESIS:{
               // function call
               std::vector<ast::Exp *> args;
               if (peek(pars).type == RPARENTESIS)
               {
                  args = {};
               }
               else
               {
                  args = parse_list<ast::Exp *>(pars, COMMA, parse_expression);
               }
               consume(pars, RPARENTESIS, "Unexpected token, expeted ')'.");
               exp = new ast::FunctionCallExp(exp, args);
               break;
            }
            case LBRACKET:{
               //subscript access
               exp = new ast::SubscriptAcessExp(exp, parse_expression(pars));
               consume(pars, RBRACKET, "Unexpected token, expected ].");
               break;
            }

         }
      }
      return exp;
   }

   ast::Exp *parse_post_inc(Parser &pars)
   {
      ast::Exp *exp = parse_primary(pars);
      Token tok;
      if (match(pars, {INC, DEC}, tok))
      {
         exp = new ast::UnaryExp{operators::get_unary_operator(tok.type), exp};
      }
      return exp;
   }

   ast::Exp *parse_primary(Parser &pars)
   {
      token::Token tok = advance(pars);
      switch (tok.type)
      {
      case NUM_LITERAL:
         return new ast::LiteralExp(ast::LIT_NUM, tok.val);
      case CHAR_LITERAL:
         return new ast::LiteralExp(ast::LIT_CHAR, tok.val);
      case STR_LITERAL:
         return new ast::LiteralExp(ast::LIT_STR, tok.val);
      case IDENTIFIER:
         return new ast::LiteralExp(ast::LIT_IDENTIFIER, tok.val);
      case LPARENTESIS:
      {
         ast::Exp *exp = parse_expression(pars);
         consume(pars, RPARENTESIS, "Unclosed parentesis expression.");
         return exp;
      }
      case FN:
      {
         std::vector<ast::NameAndType*>args;
         ast::Type* return_type;
         bool is_short = false; 
         ast::Statement* body;
         bool has_parentesis = match(pars, LPARENTESIS, tok);
         std::vector<std::vector<ast::NameAndType*>> args_list;
         if(has_parentesis && peek(pars).type == RPARENTESIS){
            args_list = {};
         }else{      
            args_list = parse_list<std::vector<ast::NameAndType*>>(
               pars,
               COMMA,
               [](Parser &pars) -> std::vector<ast::NameAndType*>{
                  auto names = parse_list<std::string>(
                     pars,
                     COMMA,
                     [](Parser &pars) -> std::string
                     {
                        return advance(pars).val;
                     }
                  );
                  consume(pars, DBL_COLON, "Expected :: .");
                  auto type = parse_type(pars);
                  std::vector<ast::NameAndType*> ret;
                  for(const auto& name : names){
                     ret.push_back(new ast::NameAndType{name, type});
                  }
                  return ret;
               }
            );
         }
      
         for(const auto& new_args : args_list){
            args.insert(args.end(), new_args.begin(), new_args.end());
         }

         if(has_parentesis){
            consume(pars, RPARENTESIS, "Unclosed (.");
         }
         if(match(pars, ARROW, tok)){
            return_type = parse_type(pars);
         }else{
            return_type = new ast::SimpleType("void");
         }
         if(match(pars, FAT_ARROW, tok)){
            is_short = true;
            body = new ast::ExpressionStatement{parse_expression(pars)};
         }else{
            body = parse_block_statement(pars);
         }
         return new ast::FnLiteralExp{args, return_type, is_short, body};
      }
      case LBRACE:
      {
         //struct literal
         std::vector<ast::Exp*> members = parse_list<ast::Exp*>(pars, COMMA, parse_expression);
         consume(pars, RBRACE, "Unclosed { expression.");
         return new ast::StructLiteralExp{members};
      }
      case LBRACKET:
      {
         //array literal
         std::vector<ast::Exp*> elements = parse_list<ast::Exp*>(pars, COMMA, parse_expression);
         consume(pars, RBRACKET, "Unclosed [ expression.");
         return new ast::ArrayLiteralExp{elements};
      }

      default:
      {
         pars.errors.push_back({error::PARSER_ERROR, tok.pos, "Invalid literal value."});
         return new ast::InvalidExp{};
      }
      }
   }

   template <typename T>
   std::vector<T> parse_list(Parser &pars, token::TokenType separator, std::function<T(Parser &)> parse_func)
   {
      std::vector<T> list{};
      Token tok;

      do
      {
         list.push_back(parse_func(pars));

      } while (match(pars, separator, tok));

      return list;
   }

   template <typename T>
   std::vector<T> parse_list(Parser &pars, token::TokenType separator, std::function<T(Parser &)> parse_func, int count)
   {
      std::vector<T> list{};
      Token tok;
      int i = 0;
      do
      {
         list.push_back(parse_func(pars));
         i++;
      } while (i<count && match(pars, separator, tok) );

      if(i<count){
         add_error(pars, "Expected "+std::to_string(count)+" ammount of elements. Got only "+std::to_string(i)+".");
      }
      return list;
   }
};
