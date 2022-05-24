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
      virtual ExpType get_type();
      virtual std::string to_string();
   };

   class BinaryExp : public Exp
   {
   public:
      BinaryExp(Exp *left, Exp *right, operators::Operator op) : left(left), right(right), op(op){};
      ExpType get_type() override { return ExpType::EXP_BINARY; };
      Exp *left, *right;
      operators::Operator op;
   };

   class UnaryExp : public Exp
   {
   public:
      UnaryExp(Exp *exp, operators::Operator op) : exp(exp), op(op){};
      ExpType get_type() override { return ExpType::EXP_UNARY; };
      Exp *exp;
      operators::Operator op;
   };

   class TernaryExp : public Exp
   {
   public:
      TernaryExp(Exp *cond, Exp *left, Exp *right, operators::Operator op) : cond(cond), left(left), right(right){};
      ExpType get_type() override { return ExpType::EXP_TERNARY; };
      Exp *cond, *right, *left;
   };

   class MemberAccessExp : public Exp
   {
   public:
      MemberAccessExp(Exp *exp, std::string member) : exp(exp), member(member){};
      ExpType get_type() override { return ExpType::EXP_MEMBER_ACCESS; };
      Exp *exp;
      std::string member;
   };

   class FunctionCallExp : public Exp
   {
      public:
      FunctionCallExp(Exp *func, std::vector<Exp*> args) : func(func), args(args){};
      ExpType get_type() override { return ExpType::EXP_FUNCTION_CALL; };
      Exp *func;
      std::vector<Exp *> args;
   };

   class LiteralExp : public Exp
   {
      public:
      LiteralExp(std::string val):val(val){};
      ExpType get_type() override { return ExpType::EXP_LITERAL; };
      std::string val;
   };
}
#endif