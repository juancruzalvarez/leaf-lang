#ifndef SCANNER_H
#define SCANNER_H
#include <string>
#include <list>
#include <fstream>
#include "token.hh"
#include "position.hh"
namespace scanner
{

   struct Scanner
   {
      std::fstream file;
      Position current_position;
      std::list<token::Token> token_buffer;
   };

   // initialize scanner for reading file at file_path.
   void init(Scanner &scn, const char *file_path);
   // returns the next token and advances scanner.
   token::Token get(Scanner &scn);
   // return the next token without advancing the scanner.
   token::Token peek(Scanner &scn);
   // returns the second next token without advancing the scanner.
   token::Token peek_2(Scanner &scn);
   // returns the nth nexttoken without advancing the scanner.
   token::Token peek_n(Scanner &scn, int n);
   // returns the token to the scanner.
   void unget(Scanner &scn, token::Token tok);
   // cleans up scanner.
   void clean_up(Scanner &scn);
}

#endif