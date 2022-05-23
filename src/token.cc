#include "../include/token.hh"
#include <string>
#include <map>

namespace token
{
   
   std::string type_to_str(TokenType type)
   {
      switch (type)
      {
      case TEOF:
         return "eof";
      case BAD_TOKEN:
         return "bad_token";
      case IDENTIFIER:
         return "identifier";
      case NUM_LITERAL:
         return "num_literal";
      case CHAR_LITERAL:
         return "char_literal";
      case STR_LITERAL:
         return "str_literal";
      case TYPE:
         return "type";
      case STRUCT:
         return "struct";
      case CONST:
         return "const";
      case FN:
         return "fn";
      case IF:
         return "if";
      case ELSE:
         return "else";
      case SWITCH:
         return "switch";
      case CASE:
         return "case";
      case DEFAULT:
         return "default";
      case WHILE:
         return "while";
      case DO:
         return "do";
      case FOR:
         return "for";
      case RETURN:
         return "return";
      case BREAK:
         return "break";
      case CONTINUE:
         return "continue";
      case ADD:
         return "+";
      case SUB:
         return "-";
      case STAR:
         return "*";
      case DIV:
         return "/";
      case MOD:
         return "%";
      case INC:
         return "++";
      case DEC:
         return "--";
      case AMP:
         return "&";
      case OR:
         return "|";
      case XOR:
         return "^";
      case SHR:
         return ">>";
      case SHL:
         return "<<";
      case NEG:
         return " ~";
      case LAND:
         return "&&";
      case LOR:
         return "||";
      case NOT:
         return "!";
      case ASSIGN:
         return "=";
      case DEFINE:
         return ":=";
      case ADD_ASSIGN:
         return "+=";
      case SUB_ASSIGN:
         return "-=";
      case MUL_ASSIGN:
         return "*=";
      case DIV_ASSIGN:
         return "/=";
      case MOD_ASSIGN:
         return "%=";
      case AND_ASSIGN:
         return "&=";
      case OR_ASSIGN:
         return "|=";
      case XOR_ASSIGN:
         return "^=";
      case SHR_ASSIGN:
         return ">>=";
      case SHL_ASSIGN:
         return "<<=";
      case NEG_ASSIGN:
         return "~=";
      case EQL:
         return "==";
      case NEQL:
         return "!=";
      case GTR:
         return ">";
      case LSS:
         return "<";
      case GEQ:
         return ">=";
      case LEQ:
         return "<=";
      case LAMBDA:
         return "\\";
      case ARROW:
         return "->";
      case FAT_ARROW:
         return "=>";
      case PIPE:
         return "|>";
      case COMMA:
         return ",";
      case DOT:
         return ".";
      case COLON:
         return ":";
      case DBL_COLON:
         return "::";
      case SEMICOLON:
         return ";";
      case LPARENTESIS:
         return "(";
      case LBRACKET:
         return "[";
      case LBRACE:
         return "{";
      case RPARENTESIS:
         return ")";
      case RBRACKET:
         return "]";
      case RBRACE:
         return "}";
      case AT:
         return "@";
      default:
         return "unknown_type";
      }
   }

   static const std::map<std::string, TokenType> keywords{
      {"type", TYPE},
      {"struct", STRUCT},
      {"const", CONST},
      {"fn", FN},
      {"if", IF},
      {"else", ELSE},
      {"switch", SWITCH},
      {"case", CASE},
      {"default", DEFAULT},
      {"while", WHILE},
      {"do", DO},
      {"for", FOR},
      {"return", RETURN},
      {"break", BREAK},
      {"continue", CONTINUE},
   };
   
   TokenType get_keyword_type(std::string val)
   {
      auto it = keywords.find(val);
      if(it == keywords.end()){
         return NO_KEYWORD;
      }else{
         return it->second;
      }
   }

}
