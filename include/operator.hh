#ifndef OPERATOR_H
#define OPERATOR_H
#include "token.hh"
namespace operators
{

   enum Operator
   {
      OPERATOR_INVALID,
      OPERATOR_ADD,         // +
      OPERATOR_SUB,         // -
      OPERATOR_MUL,         // *
      OPERATOR_STAR,        // *
      OPERATOR_DIV,         // /
      OPERATOR_MOD,         // %
      OPERATOR_INC,         // ++
      OPERATOR_DEC,         // --
      OPERATOR_AND,         // &
      OPERATOR_AMP,         // &
      OPERATOR_OR,          // |
      OPERATOR_XOR,         // ^
      OPERATOR_SHR,         // >>
      OPERATOR_SHL,         // <<
      OPERATOR_NEG,         // ~
      OPERATOR_LAND,        // &&
      OPERATOR_LOR,         // ||
      OPERATOR_NOT,         // !
      OPERATOR_ASSIGN,      // =
      OPERATOR_ADD_ASSIGN,  // +=
      OPERATOR_SUB_ASSIGN,  // -=
      OPERATOR_MUL_ASSIGN,  // *=
      OPERATOR_DIV_ASSIGN,  // /=
      OPERATOR_MOD_ASSIGN,  // %=
      OPERATOR_AND_ASSIGN,  // &=
      OPERATOR_OR_ASSIGN,   // |=
      OPERATOR_XOR_ASSIGN,  // ^=
      OPERATOR_SHR_ASSIGN,  // >>=
      OPERATOR_SHL_ASSIGN,  // <<=
      OPERATOR_NEG_ASSIGN,  // ~=
      OPERATOR_EQL,         // ==
      OPERATOR_NEQL,        // !=
      OPERATOR_GTR,         // >
      OPERATOR_LSS,         // <
      OPERATOR_GEQ,         // >=
      OPERATOR_LEQ,         // <=
      OPERATOR_PIPE,        // |>
      OPERATOR_DOT,         // .
      OPERATOR_COLON,       // :
      OPERATOR_LPARENTESIS, // (
      OPERATOR_LBRACKET,    // [
      OPERATOR_RPARENTESIS, // )
      OPERATOR_RBRACKET,    // ]
      OPERATOR_TERNARY,     // ?
      OPERATOR_AT           // @

   };

   struct OperatorInfo
   {
      int precedence;
      bool r_associative;
   };
  
   OperatorInfo get_operator_info(token::TokenType op);
   Operator get_unary_operator(token::TokenType op);
   Operator get_binary_operator(token::TokenType op);
   std::string operator_to_string(Operator op);
}

#endif