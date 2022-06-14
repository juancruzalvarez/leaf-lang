#ifndef LITERAL_H
#define LITERAL_H
#include "expression.hh"
#include "type.hh"
#include "statement.hh"
namespace ast{
   class NameAndType
   {
   public:
      std::string to_string()
      {
         return "name: " + name + " \ntype:" + type->to_string();
      };
      std::string name;
      ast::Type *type;
      NameAndType(std::string name, ast::Type *type) : name(name), type(type){};
   };


    enum LiteralType{
      LIT_IDENTIFIER,
      LIT_NUM,
      LIT_STR,
      LIT_CHAR,
      LIT_FN,
      LIT_STRUCT,
      LIT_ARRAY
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

   class StructLiteralExp : public Exp
   {
   public:
      StructLiteralExp(std::vector<Exp*> members) : members(members){};
      ExpType get_type() override {return ExpType::EXP_STRUCT_LITERAL;};
      std::string to_string()override{
         std::string ret = "{";
         for (const auto& member : members){
            ret += member->to_string() + ",\n";
         }
         return ret + "}";
      };
      std::vector<Exp*> members;
   };

   class ArrayLiteralExp : public Exp
   {
   public:
      ArrayLiteralExp(std::vector<Exp*> members) : members(members){};
      ExpType get_type() override {return ExpType::EXP_ARRAY_LITERAL;};
      std::string to_string()override{
         std::string ret = "[";
         for (const auto& member : members){
            ret += member->to_string() + ", ";
         }
         return ret + "]";
      };
      std::vector<Exp*> members;
   };

    class FnLiteralExp : public Exp
   {
   public:
      FnLiteralExp(std::vector<ast::NameAndType *> args,
                        ast::Type *return_type,
                        bool is_short,
                        ast::Statement *body)
          : args(args),
            return_type(return_type),
            is_short(is_short),
            body(body){};
      ExpType get_type() override {return ExpType::EXP_FN_LITERAL;};
      std::string to_string() override
      {
         std::string ret = "function literal:\n";
         ret += "Return type: " + return_type->to_string() + "\n";
         ret += "Args: \n";
         for (const auto &arg : args)
         {
            ret += arg->to_string() + "\n";
         }
         ret += "Short?: " + std::string{is_short} + "\n";
         ret += "Body: \n";
         ret += body->to_string();
         return ret;
      };
      std::vector<ast::NameAndType *> args;
      ast::Type *return_type;
      bool is_short;
      ast::Statement *body;
   };

};
#endif