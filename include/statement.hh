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
      virtual void gen_code(code_gen::Context& context, int &ssa_id){};
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
      std::string to_string() override {return "Return : " + exp->to_string();};
      ReturnStatement(ast::Exp* exp): exp(exp){};
      ast::Exp* exp;
      void gen_code(code_gen::Context& context, int &ssa_id) override
      {
         code_gen::Value v = exp->gen_code(context, ssa_id);
         code_gen::add_line(context, "ret " + v.type + " " + v.name);
      };
   };

   class ExpressionStatement : public Statement
   {
   public:
      StatementKind get_kind() override { return STATEMENT_EXPRESSION; };
      std::string to_string() override { return exp->to_string(); };
      ExpressionStatement(ast::Exp *exp) : exp(exp){};
      ast::Exp *exp;
      void gen_code(code_gen::Context& context, int &ssa_id) override
      {
         exp->gen_code(context, ssa_id);
      };
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
      void gen_code(code_gen::Context& context, int &ssa_id) override
      {
         code_gen::Value init;
         for(const auto& vd : vars)
         {
            std::string vd_name = vd->name;
            std::string vd_llmv_type = vd->type->to_llmv_type(context);
            context.named_values.push_back({"%"+vd_name, vd_llmv_type});
            code_gen::add_line(context, "%"+vd_name+" = alloca " + vd_llmv_type);
            if(vd->initialized) {
               init = vd->initial_val->gen_code(context, ssa_id);
               code_gen::add_line(context, "store " + vd_llmv_type+" "+init.name + ", ptr %"+vd_name);
            }
            
         }
      };
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
      void gen_code(code_gen::Context& context, int &ssa_id) override
      {
         int true_tag = ssa_id++, false_tag = ssa_id++, end_tag = ssa_id++;
         code_gen::Value condition_value = condition->gen_code(context, ssa_id);
         code_gen::add_line(context, 
            "br i1 " + condition_value.name + 
            ", label %" + std::to_string(true_tag) + 
            ", label %" + std::to_string(false_tag));
         code_gen::add_line(context, std::to_string(true_tag) + ":");
         true_branch->gen_code(context, ssa_id);
         code_gen::add_line(context, "br label %" + std::to_string(end_tag));
         code_gen::add_line(context, std::to_string(false_tag) + ":");
         false_branch->gen_code(context, ssa_id);
         code_gen::add_line(context, "br label %" + std::to_string(end_tag));
         code_gen::add_line(context, std::to_string(end_tag) + ":");
      };
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
      void gen_code(code_gen::Context& context, int &ssa_id) override
      {
         if(is_do_while)
            statement->gen_code(context, ssa_id);

         int condition_label = ssa_id++, body_label = ssa_id++, end_label = ssa_id++;
         code_gen::add_line(context, std::to_string(condition_label) + ":");
         code_gen::Value condition_value = condition->gen_code(context, ssa_id);
         code_gen::add_line(context, 
            "br i1 " + condition_value.name + 
            ", label %" + std::to_string(body_label) + 
            ", label %" + std::to_string(end_label));
         code_gen::add_line(context, std::to_string(body_label) + ":");
         statement->gen_code(context, ssa_id);
         code_gen::add_line(context, "br label %"+std::to_string(condition_label));
         code_gen::add_line(context, std::to_string(end_label) + ":");
      }
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
      void gen_code(code_gen::Context& context, int &ssa_id) override
      {
         for(const auto& s : before)
            s->gen_code(context, ssa_id);
         int condition_label = ssa_id++, body_label = ssa_id++, end_label = ssa_id++;
         code_gen::add_line(context, std::to_string(condition_label) + ":");
         code_gen::Value condition_value = condition->gen_code(context, ssa_id);
         code_gen::add_line(context, 
            "br i1 " + condition_value.name + 
            ", label %" + std::to_string(body_label) + 
            ", label %" + std::to_string(end_label));
         code_gen::add_line(context, std::to_string(body_label) + ":");
         statement->gen_code(context, ssa_id);
         for(const auto& s : after)
            s->gen_code(context, ssa_id);
         code_gen::add_line(context, "br label %"+std::to_string(condition_label));
         code_gen::add_line(context, std::to_string(end_label) + ":");
      }
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
      void gen_code(code_gen::Context& context, int &ssa_id) override
      {
         int n = context.named_values.size();
         for(const auto& s:statements)
            s->gen_code(context, ssa_id);
         while(context.named_values.size()>n)
            context.named_values.pop_back();
      }

   };

};
#endif