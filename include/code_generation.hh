#ifndef CODE_GENERATION_H
#define CODE_GENERATION_H

#include <vector>
#include <string>
#include <map>



namespace code_gen{
   
   struct Context{
      std::map<std::string, std::string> resolved_types;
      std::vector<std::string> llmv_code;
   };

   struct Value{
      std::string type;
      std::string name;
   };


   void add_line(Context &context, std::string line);
}

#endif