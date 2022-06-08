#ifndef STATEMENT_H
#define STATEMENT_H

#include "expression.hh"
#include "type.hh"
namespace ast
{
   enum StatementKind
   {
      STATEMENT_INVALID,
      STATEMENT_EMPTY,
      STATEMENT_RETURN,
      STATEMENT_BREAK,
      STATEMENT_CONTINUE,
      STATEMENT_BLOCK,
      STATEMENT_EXPRESSION,
      STATEMENT_VAR_DECLARATION,
      STATEMENT_FOR,
      STATEMENT_WHILE,
      STATEMENT_IF,
      STATEMENT_SWITCH
   };

   class Statement
   {
   public:
      virtual StatementKind get_kind() { return STATEMENT_INVALID; };
      virtual std::string to_string() { return "invalid_statement"; };
   };

   class SimpleStatement : public Statement {
   public:
      SimpleStatement(StatementKind kind) : kind(kind){};
      StatementKind kind;
      std::string to_string() override{
         switch (kind)
         {
            case STATEMENT_BREAK: return "Break Statement.";
            case STATEMENT_CONTINUE: return "Continue Statement.";
            case STATEMENT_EMPTY: return "Empty Statement.";
            default:return "Invalid Statement.";
         }
      };
      StatementKind get_kind() override{
         return kind;
      }
   };
   
   class ReturnStatement : public Statement{
   public:
      StatementKind get_kind() override {return STATEMENT_RETURN;};
      std::string to_string() override {return "Return : " + expression->to_string();};
      ReturnStatement(ast::Exp* expression):expression(expression){};
      ast::Exp* expression;
   };

   class ExpressionStatement : public Statement
   {
   public:
      StatementKind get_kind() override { return STATEMENT_EXPRESSION; };
      std::string to_string() override { return exp->to_string(); };
      ExpressionStatement(ast::Exp *exp) : exp(exp){};
      ast::Exp *exp;
   };

   class VarDeclarationStatement : public Statement
   {
   public:
      StatementKind get_kind() override { return STATEMENT_VAR_DECLARATION; };
      std::string to_string() override
      {
         std::string ret;
         for (const auto &var : vars)
         {
            ret += "vardec: " + var->to_string() + "\n";
         }
         return ret;
      };
      VarDeclarationStatement(std::vector<ast::VariableDeclaration *> vars) : vars(vars){};
      std::vector<ast::VariableDeclaration *> vars;
   };

   class IfStatement : public Statement
   {
   public:
      StatementKind get_kind() override { return STATEMENT_IF; };
      std::string to_string() override
      {
         std::string ret = "If statement:\n";
         ret += "Condition: " + condition->to_string() + "\n";
         ret += "True branch: " + true_branch->to_string() + "\n";
         if (false_branch)
            ret += "False branch: " + false_branch->to_string() + "\n";
         return ret;
      };
      IfStatement(ast::Exp *condition, ast::Statement *true_branch, ast::Statement *false_branch) : condition(condition), true_branch(true_branch), false_branch(false_branch){};
      ast::Exp *condition;
      ast::Statement *true_branch;
      ast::Statement *false_branch;
   };

   class WhileStatement : public Statement
   {
   public:
      StatementKind get_kind() override { return STATEMENT_WHILE; };
      std::string to_string() override
      {
         std::string ret = "While statement:\n";
         ret += "Is dowhile?: " + std::string(is_do_while ? "yes" : "no") + "\n";
         ret += "Condition: " + condition->to_string() + "\n";
         ret += "Statement: " + statement->to_string() + "\n";
         return ret;
      };
      WhileStatement(ast::Exp *condition, ast::Statement *statement, bool is_do_while) : condition(condition), statement(statement), is_do_while(is_do_while){};
      ast::Exp *condition;
      ast::Statement *statement;
      bool is_do_while;
   };

   class ForStatement : public Statement
   {
   public:
      StatementKind get_kind() override { return STATEMENT_FOR; };
      std::string to_string() override
      {
         std::string ret = "For statement:\n Before:";
         for(const auto& stm : before)
            ret += stm->to_string() + "\n";
         ret += "Condition: " + condition->to_string() + "\n";
         ret += "After: \n";
         for(const auto& stm : after)
            ret += stm->to_string() + "\n";
         ret += "Statement: \n";

         ret += statement->to_string();
         return ret;
      };
      ForStatement(ast::Exp *condition, std::vector<ast::Statement *>before, std::vector<ast::Statement *>after, ast::Statement *statement)
       : condition(condition), before(before), after(after), statement(statement){};
      ast::Exp *condition;
      std::vector<ast::Statement *>before, after;
      ast::Statement *statement;
   };

   class BlockStatement : public Statement
   {
   public:
      StatementKind get_kind() override { return STATEMENT_BLOCK; };
      std::string to_string() override
      {
         std::string ret = "Block:\n";
         for (const auto &stm : statements)
         {
            ret += stm->to_string() + "\n";
         }
         return ret;
      };
      BlockStatement(std::vector<ast::Statement *> statements) : statements(statements){};
      std::vector<ast::Statement *> statements;
   };

};
#endif