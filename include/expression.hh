#ifndef EXPRESSION_H
#define EXPRESSION_H
#include <string>
#include <vector>
#include "operator.hh"


namespace ast
{
   enum class ExpType
   {
      EXP_INVALID,
      EXP_UNARY,
      EXP_BINARY,
      EXP_TERNARY,
      EXP_LITERAL,
      EXP_ANONYMOUS_FUNCTION,
      EXP_ANONYMOUS_STRUCT,
      EXP_MEMBER_ACCESS,
      EXP_FUNCTION_CALL
   };

   class Exp
   {
   public:
      virtual ExpType get_type(){return ExpType::EXP_INVALID;};
      virtual std::string to_string(){return "invalid_expression";};
   };

   class BinaryExp : public Exp
   {
   public:
      BinaryExp(operators::Operator op, Exp *left, Exp *right) : left(left), right(right), op(op){};
      ExpType get_type() override { return ExpType::EXP_BINARY; };
      std::string to_string() override {return "{ "+ operators::operator_to_string(op)+" "+ left->to_string() + ", "+right->to_string()+"}";};
      Exp *left, *right;
      operators::Operator op;
   };

   class UnaryExp : public Exp
   {
   public:
      UnaryExp(operators::Operator op, Exp *exp) : exp(exp), op(op){};
      ExpType get_type() override { return ExpType::EXP_UNARY; };
      std::string to_string() override {return "{ "+ operators::operator_to_string(op)+" "+ exp->to_string() + "}";};
      Exp *exp;
      operators::Operator op;
   };

   class TernaryExp : public Exp
   {
   public:
      TernaryExp(Exp *cond, Exp *left, Exp *right) : cond(cond), left(left), right(right){};
      ExpType get_type() override { return ExpType::EXP_TERNARY; };
      std::string to_string() override {return "{" + cond->to_string() + "?" + left->to_string() + ":"+right->to_string();};
      Exp *cond, *right, *left;
   };

   class MemberAccessExp : public Exp
   {
   public:
      MemberAccessExp(Exp *exp, std::string member) : exp(exp), member(member){};
      ExpType get_type() override { return ExpType::EXP_MEMBER_ACCESS; };
      std::string to_string() override {return exp->to_string() + "."+member;};
      Exp *exp;
      std::string member;
   };

   class FunctionCallExp : public Exp
   {
      public:
      FunctionCallExp(Exp *func, std::vector<Exp*> args) : func(func), args(args){};
      ExpType get_type() override { return ExpType::EXP_FUNCTION_CALL; };
      std::string to_string() override {
         std::string ret;
         ret += func->to_string() + "(";
         for(const auto& arg : args){
            ret+= arg->to_string() + ", ";
         }
         ret = ret.substr(0, ret.size()-2);
         ret += ")";
         return ret;
      };

      Exp *func;
      std::vector<Exp *> args;
   };

   enum LiteralType{
      LIT_IDENTIFIER,
      LIT_NUM,
      LIT_STR,
      LIT_CHAR,
      LIT_FN
   };
   class LiteralExp : public Exp
   {
      public:
      LiteralExp(LiteralType type, std::string val): type(type), val(val){};
      ExpType get_type() override { return ExpType::EXP_LITERAL; };
      std::string to_string() override {return val;};
      LiteralType type;
      std::string val;
   };

   class InvalidExp : public Exp
   {
      public:
      InvalidExp(){};
      std::string to_string() override {return "bad exp";};

      ExpType get_type() override { return ExpType::EXP_INVALID; };
   };
}
#endif