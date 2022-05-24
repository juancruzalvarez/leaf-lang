#ifndef OPERATOR_H
#define OPERATOR_H

namespace operators
{

   enum Operator
   {
      OPERATOR_INVALID,
      OPERATOR_ADD,         // +
      OPERATOR_SUB,         // -
      OPERATOR_MUL,         // *
      OPERATOR_DIV,         // /
      OPERATOR_MOD,         // %
      OPERATOR_INC,         // ++
      OPERATOR_DEC,         // --
      OPERATOR_AND,         // &
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
  
   OperatorInfo get_operator_info(Operator op)
   {
      switch (op)
      {
      case OPERATOR_DOT:
      case OPERATOR_LPARENTESIS:
      case OPERATOR_LBRACKET:
      case OPERATOR_INC:
      case OPERATOR_DEC:
         return {10, false};
      case OPERATOR_NOT:
      case OPERATOR_NEG:
         return {10, true};
      case OPERATOR_MUL:
      case OPERATOR_DIV:
      case OPERATOR_MOD:
      case OPERATOR_SHL:
      case OPERATOR_SHR:
         return {9, false};
      case OPERATOR_ADD:
      case OPERATOR_SUB:
         return {8, false};
      case OPERATOR_GTR:
      case OPERATOR_LSS:
      case OPERATOR_GEQ:
      case OPERATOR_LEQ:
         return {7, true};
      case OPERATOR_EQL:
      case OPERATOR_NEQL:
         return {6, true};
      case OPERATOR_AND:
         return {5, true};
      case OPERATOR_XOR:
         return {4, true};
      case OPERATOR_OR:
         return {3, true};
      case OPERATOR_LAND:
         return {2, true};
      case OPERATOR_LOR:
         return {1, true};
      case OPERATOR_TERNARY:
      case OPERATOR_ASSIGN:
      case OPERATOR_ADD_ASSIGN:
      case OPERATOR_AND_ASSIGN:
      case OPERATOR_SUB_ASSIGN:
      case OPERATOR_DIV_ASSIGN:
      case OPERATOR_MOD_ASSIGN:
      case OPERATOR_MUL_ASSIGN:
      case OPERATOR_NEG_ASSIGN:
      case OPERATOR_OR_ASSIGN:
      case OPERATOR_SHL_ASSIGN:
      case OPERATOR_SHR_ASSIGN:
      case OPERATOR_XOR_ASSIGN:
         return {0, false};
      default:
         return {-1, false};
      }
   }

}

#endif