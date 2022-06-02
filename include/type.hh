#ifndef TYPE_H
#define TYPE_H
#include <string>
#include <vector>
#include "expression.hh"
#include <iostream>

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
      bool is_pointer = false, is_ref = false, is_const = false;
   };

   class VariableDeclaration
   {
   public:
      VariableDeclaration(ast::Type *type, std::string name, bool initialized, ast::Exp *initial_val)
          : type(type), name(name), initialized(initialized), initial_val(initial_val){};

      std::string to_string()
      {
         std::string s = "var:\n   Type: ";
         s += type->to_string();
         s += "\n   Name: " + name;
         s += (initialized ? ("\n   Val: " + initial_val->to_string()) : "");
         return s;
      }
      ast::Type *type;
      std::string name;
      bool initialized;
      ast::Exp *initial_val;
   };

   class SimpleType : public Type
   {
   public:
      SimpleType(std::string val) : val(val){};
      TypeKind get_kind() override { return TYPE_SIMPLE; }
      virtual std::string to_string()
      {
         std::string res = is_const ? "const " : "";
         res += is_pointer ? "ptr " : "";
         res += is_ref ? "ref " : "";
         res += val;
         return res;
      }
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
         std::string res = is_const ? "const " : "";
         res += is_pointer ? "ptr " : "";
         res += is_ref ? "ref " : "";
         res += "struct{\n";
         for (const auto &var : vars)
         {
            res += var->to_string() + "\n";
         }
         res += '}';
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
         std::string res = is_const ? "const " : "";
         res += is_pointer ? "ptr " : "";
         res += is_ref ? "ref " : "";
         res += "fn{\n";
         res += "Args:";
         for (const auto arg : args)
         {
            res += arg->to_string() + "\n";
         }
         res += "Returns:" + return_type->to_string();
         res += "}";
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
      std::string to_string() override
      {
         std::string res = is_const ? "const " : "";
         res += is_pointer ? "ptr " : "";
         res += is_ref ? "ref " : "";
         res += "Union{ ";
         for (const auto &type : types)
         {
            res += type->to_string();
            res += ", ";
         }
         return res + " }";
      };
      std::vector<ast::Type *> types;
   };

   class BadType : public Type
   {
      TypeKind get_kind() override { return TYPE_INVALID; }
      virtual std::string to_string() { return "Bad Type"; }
   };

   class TypeClass{
   public:
      std::string type_var;
      std::vector<std::string>is;

   };

   enum TraitType{
      TRAIT_INVALID,
      TRAIT_MEMBER,
      TRAIT_METHOD
   };
   class Trait{
   public:
      virtual std::string to_string(){return "invalid_trait";}
      virtual TraitType get_type(){return TRAIT_INVALID;}
   };
   class MemberTrait : public Trait{
      std::string to_string() override{return "invalid_trait";}
      virtual TraitType get_type(){return TRAIT_MEMBER;}
   };

   class MethodTrait : public Trait{

   };
};

#endif