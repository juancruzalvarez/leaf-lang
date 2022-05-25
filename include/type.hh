#ifndef TYPE_H
#define TYPE_H
#include <string>

namespace ast{
   enum TypeKind{
      TYPE_SIMPLE,
      TYPE_STRUCT,
      TYPE_FUNCTION,
      TYPE_UNION,
      TYPE_AND,
      TYPE_INVALID
   };

   class Type{
   public:
      virtual TypeKind get_kind(){return TYPE_INVALID;};
      virtual std::string to_string(){return "invalid_type";}
   };

   class SimpleType : public Type{
   public:
      SimpleType(std::string val):val(val){};
      TypeKind get_kind() override {return TYPE_SIMPLE;};
      virtual std::string to_string(){return val;}
      std::string val;
   };

   class StructType : public Type{
   public:
      StructType(std::string val):val(val){};
      TypeKind get_kind() override {return TYPE_STRUCT;};
      virtual std::string to_string(){return val;}
      std::string val;
   };
};

#endif