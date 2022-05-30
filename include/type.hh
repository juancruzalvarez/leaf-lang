#ifndef TYPE_H
#define TYPE_H
#include <string>
#include <vector>
#include "expression.hh"

namespace ast
{
   enum TypeKind
   {
      TYPE_SIMPLE,
      TYPE_STRUCT,
      TYPE_FUNCTION,
      TYPE_UNION,
      TYPE_AND,
      TYPE_INVALID
   };

   class Type
   {
   public:
      virtual TypeKind get_kind() { return TYPE_INVALID; };
      virtual std::string to_string() { return "invalid_type"; }
   };

   class VariableDeclaration
   {
   public:
      VariableDeclaration(ast::Type *type, std::string name, ast::Exp *val)
          : type(type), name(name), initial_val(val){};

      std::string to_string()
      {
         return "var Type: " + type->to_string() + "Name: " + name + initialized ? "val:" + val->to_string() : "";
      }
      ast::Type *type;
      std::string name;
      ast::Exp *initial_val;
   };

   class SimpleType : public Type
   {
   public:
      SimpleType(std::string val) : val(val){};
      TypeKind get_kind() override { return TYPE_SIMPLE; };
      virtual std::string to_string() { return val; }
      std::string val;
   };

   class StructType : public Type
   {
   public:
      StructType(std::vector<std::string> template_vars, std::vector<ast::VariableDeclaration *> vars)
          : template_vars(template_vars), vars(vars){};

      TypeKind get_kind() override { return TYPE_STRUCT; };
      std::string to_string() override
      {
         std::string res = "struct{/n";
         for (const auto var : vars)
         {
            res += var->to_string() + "/n";
         }
         return res;
      }
      std::vector<std::string> template_vars;
      std::vector<ast::VariableDeclaration *> vars;
   };

   class FunctionType : public Type
   {
   public:
      FunctionType(std::vector<std::string> template_vars, std::vector<ast::Type *> args, ast::Type *return_type)
          : template_vars(template_vars), args(args), return_type(return_type){};
      TypeKind get_kind() override { return TYPE_FUNCTION; };
      std::string to_string() override
      {
         std::string res = "fn{/n";
         res += "Args:";
         for (const auto arg : args)
         {
            res += arg->to_string() + "/n";
         }
         res += "Returns:" + return_type->to_string();
         return res;
      }
      std::vector<std::string> template_vars;
      std::vector<ast::Type *> args;
      ast::Type *return_type;
   };

   class UnionType : public Type
   {
   public:
      UnionType(std::vector<ast::Type *> types) : types(types){};
      TypeKind get_kind() override { return TYPE_UNION; };
      std::string to_string () override
      {
         std::string res = "Union{ ";
         for (const auto &type : types)
         {
            res += type->to_string();
            res += ", ";
         }
         return res + " }";
      };
      std::vector<ast::Type *> types;
   };
};

#endif