#ifndef TOKEN_H
#define TOKEN_H
#include <string>
#include "position.hh"
namespace token
{

   enum TokenType
   {
      TEOF,       // represents end of file.
      BAD_TOKEN,  // represents invalid token.
      NO_KEYWORD, // used for get keyword type return.
      IDENTIFIER,
      NUM_LITERAL,
      CHAR_LITERAL,
      STR_LITERAL,
      // keywords:
      MODULE,
      IMPORT,
      PUBLIC,
      PRIVATE,
      CLASS,
      IS,
      HAS,
      TYPE,
      STRUCT,
      CONST,
      FN,
      IF,
      ELSE,
      SWITCH,
      CASE,
      DEFAULT,
      WHILE,
      DO,
      FOR,
      RETURN,
      BREAK,
      CONTINUE,
      // simbols:
      ADD,         // +
      SUB,         // -
      STAR,        // *
      DIV,         // /
      MOD,         // %
      INC,         // ++
      DEC,         // --
      AMP,         // &
      OR,          // |
      XOR,         // ^
      SHR,         // >>
      SHL,         // <<
      NEG,         // ~
      LAND,        // &&
      LOR,         // ||
      NOT,         // !
      ASSIGN,      // =
      DEFINE,      // :=
      ADD_ASSIGN,  // +=
      SUB_ASSIGN,  // -=
      MUL_ASSIGN,  // *=
      DIV_ASSIGN,  // /=
      MOD_ASSIGN,  // %=
      AND_ASSIGN,  // &=
      OR_ASSIGN,   // |=
      XOR_ASSIGN,  // ^=
      SHR_ASSIGN,  // >>=
      SHL_ASSIGN,  // <<=
      NEG_ASSIGN,  // ~=
      EQL,         // ==
      NEQL,        // !=
      GTR,         // >
      LSS,         // <
      GEQ,         // >=
      LEQ,         // <=
      LAMBDA,      // '\'
      ARROW,       // ->
      FAT_ARROW,   // =>
      PIPE,        // |>
      COMMA,       // ,
      DOT,         // .
      COLON,       // :
      DBL_COLON,   // ::
      SEMICOLON,   // ;
      LPARENTESIS, // (
      LBRACKET,    // [
      LBRACE,      // {
      RPARENTESIS, // )
      RBRACKET,    // ]
      RBRACE,      // }
      TERNARY,       // ?
      AT           // @
   };

  
   struct Token
   {
      Position pos;
      TokenType type;
      std::string val;
   };

   // given a token type returns the string representation of it.
   std::string type_to_str(TokenType type);
   // given a string that could be a keyword, returns the apropiate type or NO_KEYWORD.
   TokenType get_keyword_type(std::string val);
}

#endif