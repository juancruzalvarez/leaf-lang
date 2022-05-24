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
  
   OperatorInfo get_operator_info(token::TokenType op)
   {
      switch (op)
      {
      case token::STAR:
      case token::DIV:
      case token::MOD:
      case token::SHL:
      case token::SHR:
         return {9, false};

      case token::ADD:
      case token::SUB:
         return {8, false};

      case token::GTR:
      case token::LSS:
      case token::GEQ:
      case token::LEQ:
         return {7, true};

      case token::EQL:
      case token::NEQL:
         return {6, true};

      case token::AMP:
         return {5, true};

      case token::XOR:
         return {4, true};

      case token::OR:
         return {3, true};

      case token::LAND:
         return {2, true};

      case token::LOR:
         return { 1, true};

      default:
         return {-1, false};
      }
   }

   Operator get_unary_operator(token::TokenType op){
      switch(op){
         case token::NOT:
            return OPERATOR_NOT;
         case token::SUB:
            return OPERATOR_SUB;
         case token::INC:
            return OPERATOR_INC;
         case token::DEC:
            return OPERATOR_DEC;
         case token::NEG:
            return OPERATOR_NEG;
         case token::STAR:
            return OPERATOR_STAR;
         case token::AMP:
            return OPERATOR_AMP;
         default:
            return OPERATOR_INVALID;
      }
   }

   Operator get_binary_operator(token::TokenType op){
      switch (op)
      {
      case token::ASSIGN:
         return OPERATOR_ASSIGN;
      case token::SUB_ASSIGN:
         return OPERATOR_SUB_ASSIGN;
      case token::MUL_ASSIGN:
         return OPERATOR_MUL_ASSIGN;
      case token::DIV_ASSIGN:
         return OPERATOR_DIV_ASSIGN;
      case token::MOD_ASSIGN:
         return OPERATOR_MOD_ASSIGN;
      case token::AND_ASSIGN:
         return OPERATOR_AND_ASSIGN;
      case token::OR_ASSIGN:
         return OPERATOR_OR_ASSIGN;
      case token::XOR_ASSIGN:
         return OPERATOR_XOR_ASSIGN;
      case token::SHR_ASSIGN:
         return OPERATOR_SHR_ASSIGN;
      case token::SHL_ASSIGN:
         return OPERATOR_SHL_ASSIGN;
      case token::NEG_ASSIGN:
         return OPERATOR_NEG_ASSIGN;
      case token::STAR:
         return OPERATOR_MUL;
      case token::DIV:
         return OPERATOR_DIV;
      case token::MOD:
         return OPERATOR_MOD;
      case token::SHL:
         return OPERATOR_SHL;
      case token::SHR:
         return OPERATOR_SHR;
      case token::ADD:
         return OPERATOR_ADD;
      case token::SUB:
         return OPERATOR_SUB;
      case token::GTR:
         return OPERATOR_GTR;
      case token::LSS:
         return OPERATOR_LSS;
      case token::GEQ:
         return OPERATOR_LEQ;
      case token::LEQ:
         return OPERATOR_LEQ;
      case token::EQL:
         return OPERATOR_EQL;
      case token::NEQL:
         return OPERATOR_NEQL;
      case token::AMP:
         return OPERATOR_AND;
      case token::XOR:
         return OPERATOR_XOR;
      case token::OR:
         return OPERATOR_OR;
      case token::LAND:
         return OPERATOR_LAND;
      case token::LOR:
         return OPERATOR_LOR;
      default:
         return OPERATOR_INVALID;
      }
   }

   std::string operator_to_string(Operator op){
      switch(op){
      case OPERATOR_INVALID:return"invalid_op";
      case OPERATOR_ADD:return"+";
      case OPERATOR_SUB:return"-";
      case OPERATOR_MUL:return"*";
      case OPERATOR_STAR:return"*";
      case OPERATOR_DIV:return"/";
      case OPERATOR_MOD:return"%";
      case OPERATOR_INC:return"++";
      case OPERATOR_DEC:return"--";
      case OPERATOR_AND:return"&";
      case OPERATOR_AMP:return"&";
      case OPERATOR_OR:return"|";
      case OPERATOR_XOR:return"^";
      case OPERATOR_SHR:return">>";
      case OPERATOR_SHL:return"<<";
      case OPERATOR_NEG:return" ~";
      case OPERATOR_LAND:return"&&";
      case OPERATOR_LOR:return"||";
      case OPERATOR_NOT:return"!";
      case OPERATOR_ASSIGN:return"=";
      case OPERATOR_ADD_ASSIGN:return"+=";
      case OPERATOR_SUB_ASSIGN:return"-=";
      case OPERATOR_MUL_ASSIGN:return"*=";
      case OPERATOR_DIV_ASSIGN:return"/=";
      case OPERATOR_MOD_ASSIGN:return"%=";
      case OPERATOR_AND_ASSIGN:return"&=";
      case OPERATOR_OR_ASSIGN:return"|=";
      case OPERATOR_XOR_ASSIGN:return"^=";
      case OPERATOR_SHR_ASSIGN:return">>=";
      case OPERATOR_SHL_ASSIGN:return"<<=";
      case OPERATOR_NEG_ASSIGN:return"~=";
      case OPERATOR_EQL:return"==";
      case OPERATOR_NEQL:return"!=";
      case OPERATOR_GTR:return">";
      case OPERATOR_LSS:return"<";
      case OPERATOR_GEQ:return">=";
      case OPERATOR_LEQ:return"<=";
      case OPERATOR_PIPE:return"|>";
      case OPERATOR_DOT:return".";
      case OPERATOR_COLON:return":";
      case OPERATOR_LPARENTESIS:return"(";
      case OPERATOR_LBRACKET:return"[";
      case OPERATOR_RPARENTESIS:return")";
      case OPERATOR_RBRACKET:return"]";
      case OPERATOR_TERNARY:return"?";
      case OPERATOR_AT:return"@";
      default:return "invalid_operator";
      }
   }
}

#endif