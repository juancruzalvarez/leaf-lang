#ifndef DECLARATION_H
#define DECLARATION_H
#include <string>
#include "type.hh"
#include "statement.hh"
#include "literal.hh"
#include "code_generation.hh"
namespace ast
{
   

   enum DeclarationKind
   {
      DECLARATION_INVALID,
      DECLARATION_TYPE,
      DECLARATION_FN,
      DECLARATION_CONST,
      DECLARATION_CONST_SET,
      DECLARATION_TYPE_CLASS,
   };

   enum DeclarationVisibility
   {
      VISIBILITY_PUBLIC,
      VISIBILITY_PRIVATE
   };

   class Declaration
   {
   public:
      DeclarationVisibility visibility;
      virtual DeclarationKind get_kind() { return DECLARATION_INVALID; };
      virtual std::string to_string() { return "invalid_dec"; };
      virtual void gen_code(code_gen::Context &contex){};
   };

   class InvalidDeclaration : public Declaration
   {
      DeclarationKind get_kind() override { return DECLARATION_INVALID; };
      std::string to_string() override { return "invalid_dec"; };
   };

   class FunctionDeclaration : public Declaration
   {
   public:
      
      FunctionDeclaration(std::string name,
                          std::vector<std::string> template_vars,
                          bool is_method,
                          ast::NameAndType *method_name_and_type,
                          std::vector<ast::NameAndType *> args,
                          ast::Type *return_type,
                          bool is_short,
                          ast::Statement *body)
          : name(name),
            template_vars(template_vars),
            is_method(is_method),
            method_name_and_type(method_name_and_type),
            args(args),
            return_type(return_type),
            is_short(is_short),
            body(body){};
      
      ast::DeclarationKind get_kind() override { return DECLARATION_FN; };
      
      std::string to_string() override
      {
         std::string ret = "Name: " + name + "\n";
         ret += is_method? "Is method.\n": "";
         ret += is_method? ("Method: "+method_name_and_type->to_string()+ "\n") : ""; 
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
      
     

      std::string name;
      std::vector<std::string> template_vars;
      bool is_method;
      ast::NameAndType *method_name_and_type;
      std::vector<ast::NameAndType *> args;
      ast::Type *return_type;
      bool is_short;
      ast::Statement *body;
   };

   class TypeDeclaration : public Declaration
   {
   public:
      TypeDeclaration(std::string name, ast::Type *type) : name(name), type(type){};
      DeclarationKind get_kind() override { return DECLARATION_TYPE; };
      std::string to_string() override { return "Name: " + name + "\nType: " + type->to_string(); };
      void gen_code(code_gen::Context &contex) override 
      {
         code_gen::add_line(contex, "%"+name+" ="+type->to_llmv_type());
      };

      std::string name;
      ast::Type *type;
   };

   class TypeClassDeclaration : public Declaration
   {
   public:
      TypeClassDeclaration(std::string name, ast::TypeClass *type_class) : name(name), type_class(type_class){};
      DeclarationKind get_kind() override { return DECLARATION_TYPE_CLASS; };
      std::string to_string() override { return "Name: " + name + "\nType: " + type_class->to_string(); };
      std::string name;
      ast::TypeClass *type_class;
   };

   class ConstDeclaration : public Declaration
   {
   public:
      ConstDeclaration(std::string name, ast::Type* type, ast::Exp* val): name(name), type(type), val(val){}
      std::string name;
      ast::Type* type;
      ast::Exp* val;
      std::string to_string() override {
         return "const: "+ name + " \ntype:" + type->to_string() + " \nval:" + val->to_string();
      };
      DeclarationKind get_kind() override {
         return DECLARATION_CONST;
      };

      void gen_code(code_gen::Context &contex) override {
         
      }
   };

   class ConstSetDeclaration : public Declaration
   {
   public:
      ConstSetDeclaration(std::string name, std::vector<Declaration*> values):name(name), values(values){}   
      std::string name;
      std::vector<Declaration*> values;
      std::string to_string() override {
         std::string ret = "Const set "+name + ":"+"\n" +"{\n";
         for(const auto& val : values){
            ret+= val->to_string() + ",\n";
         }
         ret += "}";
         
         return ret; 
      };
      DeclarationKind get_kind() override {

         return DECLARATION_CONST_SET;
      };
   };

};
#endif