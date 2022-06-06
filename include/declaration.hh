#ifndef DECLARATION_H
#define DECLARATION_H
#include <string>
#include "type.hh"
#include "statement.hh"
namespace ast
{
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

   enum DeclarationKind
   {
      DECLARATION_INVALID,
      DECLARATION_TYPE,
      DECLARATION_FN,
      DECLARATION_CONST,
      DECLARATION_TYPE_CLASS,
   };
   class Declaration
   {
   public:
      virtual DeclarationKind get_kind() { return DECLARATION_INVALID; };
      virtual std::string to_string() { return "invalid_dec"; };
   };

   class InvalidDeclaration : public Declaration{
      DeclarationKind get_kind() override{ return DECLARATION_INVALID; };
      std::string to_string() override { return "invalid_dec"; };
   };

  
   class FunctionDeclaration : public Declaration
   {
   public:

      FunctionDeclaration(std::string name,
                          std::vector<ast::NameAndType *> args,
                          ast::Type *return_type,
                          bool is_short,
                          ast::Statement *block) : name(name), args(args), return_type(return_type), is_short(is_short), block(block){};
      ast::DeclarationKind get_kind() override { return DECLARATION_FN; };
      std::string to_string() override
      {
         std::string ret = "function declaration:\n";
         ret += "Name: " + name + "\n";
         ret += "Return type: " + return_type->to_string() + "\n";
         ret += "Args: \n";
         for (const auto &arg : args)
         {
            ret += arg->to_string() + "\n";
         }
         ret += "Short?: " + std::string{is_short} + "\n";
         ret += "Body: \n";
         ret += block->to_string();
         return ret;
      };
      std::string name;
      std::vector<ast::NameAndType *> args;
      ast::Type *return_type;
      bool is_short;
      ast::Statement *block;
   };

   class TypeDeclaration : public Declaration
   {
   public:
      TypeDeclaration(std::string name, ast::Type* type): name(name), type(type){};
      DeclarationKind get_kind()override{return DECLARATION_TYPE;};
      std::string to_string()override{return "Type declaration: \nName: " + name + "\nType: " + type->to_string();};
      std::string name;
      ast::Type *type;
   };

   class TypeClassDeclaration : public Declaration
   {
   public:
      TypeClassDeclaration(std::string name, ast::TypeClass* type): name(name), type_class(type_class){};
      DeclarationKind get_kind()override{return DECLARATION_TYPE_CLASS;};
      std::string to_string()override{return "Type class declaration: \nName: " + name + "\nType: " + type_class->to_string();};
      std::string name;
      ast::TypeClass *type_class;
   };
};
#endif