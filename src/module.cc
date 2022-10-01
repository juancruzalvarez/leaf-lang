#include "../include/module.hh"
#include "../include/code_generation.hh"
namespace ast {

   void generate_code(code_gen::Context& context, Module* module)
   {
      // resolve aliases
      

      //const declarations


      //type declarations
      for(const auto& td : module->declarations->types)
         td->gen_code(context);

      //function declarations

     
      //debug
      for(const auto& s : context.llmv_code) 
         std::cout<<s<<"\n";
   }
}
 