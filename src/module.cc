#include "../include/module.hh"
#include "../include/code_generation.hh"
namespace ast {

   void generate_code(code_gen::Context& context, Module* module)
   {
      // resolve aliases
      context.resolved_types.insert({"%int" , "i32"});

      context.resolved_types.insert({"%char", "i8"});
      context.resolved_types.insert({"%bool", "i8"});

      context.resolved_types.insert({"%i8" , "i8"});
      context.resolved_types.insert({"%i16", "i16"});
      context.resolved_types.insert({"%i32", "i32"});
      context.resolved_types.insert({"%i64", "i64"});

      context.resolved_types.insert({"%float", "float"});
      context.resolved_types.insert({"%f16"  , "half"});
      context.resolved_types.insert({"%f32"  , "float"});
      context.resolved_types.insert({"%f64"  , "double"});
      context.resolved_types.insert({"%f128" , "fp128"});
      for(const auto& td : module->declarations->type_aliases)
         context.resolved_types.insert({"%"+td->name,"%"+dynamic_cast<ast::SimpleType*>(td->type)->val});

      int changes;
      do {
         changes = 0;
         for(const auto & [key, value] : context.resolved_types) {
            if (context.resolved_types.count(value) > 0) {
               context.resolved_types[key] = context.resolved_types[value];
               changes++;
            }
         }

      } while(changes > 0);

      for(const auto& [key, value] : context.resolved_types)
      {
         std::cout<<key<<"=>"<<value<<"\n";
      }




      //const declarations
      //add values to named values!

      //type declarations
      for(const auto& td : module->declarations->types)
         td->gen_code(context);

      //function declarations
      for(const auto& fd : module->declarations->functions)
         fd->gen_code(context);
     
      //debug
      for(const auto& s : context.llmv_code) 
         std::cout<<s<<"\n";
   }
}
 