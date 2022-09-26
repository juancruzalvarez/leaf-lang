#ifndef CODE_GENERATION_H
#define CODE_GENERATION_H
#include <string>

namespace code_gen{
   
   struct Context{
      std::vector<std::string> llmv_code;
   };

   struct Value{
      std::string type;
      std::string name;
   };

   void add_line(Context contex, std::string line){
      contex.llmv_code.push_back(line);
   }
}

#endif