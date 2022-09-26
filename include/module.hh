#ifndef MODULE_H
#define MODULE_H

#include <string>
#include <vector>

#include "declaration.hh"

namespace ast
{

   struct ParsedDeclarations{
      std::vector<ast::TypeDeclaration*> type_aliases;
      std::vector<ast::TypeClassDeclaration*>type_classes; 
      std::vector<ast::TypeDeclaration*> types;
      std::vector<ast::FunctionDeclaration*> functions;
      std::vector<ast::FunctionDeclaration*> methods;
      std::vector<ast::ConstDeclaration*> consts;
      std::vector<ast::ConstSetDeclaration*> const_sets;
   };
   
   struct Module{
      std::string name;
      std::vector<std::string> imports;
<<<<<<< HEAD
      ParsedDeclarations *public_declarations;
      ParsedDeclarations *private_declarations;
=======
      std::vector<ast::Declaration*> public_declarations;
      std::vector<ast::Declaration*> private_declarations;
>>>>>>> ebcf0cc7b3ef353e545f6fa7a5013b4727c7d2e0
   };
   
};

#endif