#include "../include/code_generation.hh"

namespace code_gen {

   void add_line(Context &context, std::string line)
   {
      context.llmv_code.push_back(line);
   }
}