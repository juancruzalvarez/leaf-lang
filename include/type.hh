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
      TYPE_INVALID
   };

   class Type
   {
   public:
      virtual TypeKind get_kind() { return TYPE_INVALID; };
      virtual std::string to_string() { return "invalid_type"; }
      virtual std::string to_llmv_type() { return "invalid_type";};
      bool is_pointer = false, is_const = false;
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
      std::string to_string() override
      {
         std::string res = is_const ? "const " : "";
         res += is_pointer ? "ptr " : "";
         res += val;
         return res;
      }
      std::string to_llmv_type() override 
      {
         if (is_pointer)
            return "ptr";
         else
            return "%"+val;
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
         res += "struct{\n";
         for (const auto &var : vars)
         {
            res += var->to_string() + "\n";
         }
         res += '}';
         return res;
      }

      std::string to_llmv_type() override {
         std::string llmv_type = "type {";
         for(const auto& v : vars){
            llmv_type += v->type->to_llmv_type() + ", ";
         }
         llmv_type.pop_back();
         llmv_type.pop_back();
         llmv_type += '}';
         return llmv_type;
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

      std::string to_llmv_type() override 
      {
         if(is_pointer)
            return "ptr";
         std::string llmv_type = return_type->to_llmv_type() + " (";
         for (const auto arg : args)
         {
            llmv_type += arg->to_llmv_type() + ", ";
         }
         llmv_type.pop_back();
         llmv_type.pop_back();
         llmv_type+= ')';
         return llmv_type;
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
      std::string to_string() override { return "Bad Type"; }
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
   public:
      MemberTrait(std::string name, ast::Type* type):name(name), type(type){}
      std::string to_string() override {return "name: "+name+" type: "+type->to_string();}
      TraitType get_type() override {return TRAIT_MEMBER;}
      std::string name;
      ast::Type* type;
   };

   class MethodTrait : public Trait{
   public:
      MethodTrait(std::string name, std::vector<ast::Type*> args, ast::Type* return_type): name(name), args(args),return_type(return_type){}
      TraitType get_type() override {return TRAIT_METHOD;}
      std::string to_string() override {
         std::string ret =  "Method :: \nname: "+name+"\nArgs:\n";
         for(const auto& arg : args){
            ret+= arg->to_string() +"\n";
         }
         ret += "Returns: " + return_type->to_string();
         return ret;
      }
      std::string name;
      std::vector<ast::Type*> args;
      ast::Type* return_type;
   };

   class TypeClass {
   public:
      std::string type_var;
      std::vector<std::string>is;
      std::vector<ast::Trait*>has;
      TypeClass(std::string type_var,
      std::vector<std::string>is,
      std::vector<ast::Trait*>has) : type_var(type_var),is (is), has(has) {};
      std::string to_string(){
         std::string ret = "Typeclass: \n Type_Var: "+type_var+"\n Is:\n";

         for(const auto& iss :is){
            ret+=iss+"\n";
         }
         ret+="Has:\n";
         for(const auto& hass :has){
            ret+=hass->to_string()+"\n";
         }
         return ret;
      }

   };

   
};

#endif