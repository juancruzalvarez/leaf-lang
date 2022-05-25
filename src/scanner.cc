#include "../include/scanner.hh"
#include <functional>

namespace scanner
{

   namespace
   {
      // type for representing conditions for read_while().
      // takes a char as input and returns a boolean that represents wheter it meets the condition.
      typedef std::function<bool(char)> Condition;

      inline bool is_letter(char c)
      {
         return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
      }

      inline bool is_decimal_digit(char c)
      {
         return c >= '0' && c <= '9';
      }

      inline bool is_whitespace(char c)
      {
         return c == ' ' || c == '\n' || c == '\r' || c == '\t' || c == '\f' || c == '\v';
      }

      // returns the next character in the file without advancing the stream.
      char peek_char(scanner::Scanner &scn)
      {
         return scn.file.peek();
      }

      // returns the second next character in the file without advancing the stream.
      char peek_char_2(scanner::Scanner &scn)
      {
         scn.file.get();
         char val = scn.file.peek();
         scn.file.unget();
         return val;
      }

      // returns the next character in the file and advances the stream.
      char get_char(scanner::Scanner &scn)
      {
         return scn.file.get();
      }


      void advance_whitespace(scanner::Scanner &scn)
      {
         while (is_whitespace(peek_char(scn)))
            get_char(scn);
      }

      
      // advances the stream while the condition is meet and returns a string containing the advanced characters.
      std::string read_while(scanner::Scanner &scn, Condition cond)
      {
         std::string val{};
         while (cond(peek_char(scn)))
         {
            val += get_char(scn);
         }
         return val;
      }

      // reads numerical literal.
      std::string read_numerical_literal(scanner::Scanner &scn)
      {
         // numerical literals are stored internally with the following format:
         //  (+/-) (d/b/x/o) (integral_part) <(.) (fractional_part)> < (e) (exponent)>
         //  where parts surrounded by <> are optional, and if the prefix is other than 'd'  they are not allowed.

         std::string integral_part, fractional_part, exponent;
         bool reading_decimal = true;
         bool has_fractional_part = false, has_exponent = false;
         Condition cond = is_decimal_digit;
         char prefix = 'd';
         char sign = '+';

         // advance sign
         if (peek_char(scn) == '+' || peek_char(scn) == '-')
         {
            sign = get_char(scn);
         }

         char first = peek_char(scn);
         char second = peek_char_2(scn);
         if (first == '0')
         {
            switch (second)
            {
            case 'b':
            case 'B':
            {
               // binary
               get_char(scn); // advance the 0
               prefix = get_char(scn);
               cond = [](char c) -> bool
               { return c == '0' || c == '1'; }; // is binary digit?
               reading_decimal = false;
               break;
            }
            case 'o':
            case 'O':
            {
               // octal
               get_char(scn); // advance the 0
               prefix = get_char(scn);
               cond = [](char c) -> bool
               { return c >= '0' && c <= '7'; }; // is octal digit?
               reading_decimal = false;
               break;
            }
            case 'x':
            case 'X':
            {
               // hexa
               get_char(scn); // advance the 0
               prefix = get_char(scn);
               cond = [](char c) -> bool
               { return is_decimal_digit(c) || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'); }; // is hexa digit?
               reading_decimal = false;
               break;
            }
            }
         }

         integral_part = read_while(scn, cond);

         if (reading_decimal)
         {
            // if we are reading decimal, read optional fractional part and exponent.
            if (peek_char(scn) == '.')
            {
               get_char(scn); // advance '.'
               fractional_part = read_while(scn, cond);
               has_fractional_part = true;
            }

            if (peek_char(scn) == 'e' || peek_char(scn) == 'E')
            {
               get_char(scn); // advance 'e' or 'E'
               char exp_sig = '+';
               if (peek_char(scn) == '+' || peek_char(scn) == '-')
                  exp_sig = get_char(scn);

               exponent = exp_sig + read_while(scn, cond);

               has_exponent = true;
            }
         }

         // if there is no integral part return empty string.
         if (integral_part == "")
         {
            return "";
         }
         // if there is a fractional part but is empty cleanup and return null.
         if (has_fractional_part && fractional_part == "")
         {
            return "";
         }

         // if there is exponent but is empty cleanup and return null.
         if (has_exponent && exponent == "")
         {
            return "";
         }

         std::string literal = sign + std::string(1, prefix) + integral_part;
         if (has_fractional_part)
         {
            literal += "." + fractional_part;
         }
         if (has_exponent)
         {
            literal += "e" + exponent;
         }
         return literal;
      }

      // reads character literal.
      std::string read_char_literal(scanner::Scanner &scn)
      {
         std::string ret;
         get_char(scn);                       // advance '
         ret = std::string(1, get_char(scn)); // read character.
         get_char(scn);                       // advance '
         return ret;
      }

      // reads string literal.
      std::string read_string_literal(scanner::Scanner &scn)
      {
         std::string ret;
         get_char(scn); // advance "
         ret = read_while(scn, [](char c) -> bool
                          { return c != '\"' && c != '\n' && c != EOF; });
         get_char(scn); // advance "
         return ret;
      }

      // reads symbol.
      token::TokenType read_symbol(scanner::Scanner &scn)
      {
         char first = get_char(scn);
         switch (first)
         {
         case '@':
            return token::AT;
         case '}':
            return token::RBRACE;
         case ']':
            return token::RBRACKET;
         case ')':
            return token::RPARENTESIS;
         case '{':
            return token::LBRACE;
         case '[':
            return token::LBRACKET;
         case '(':
            return token::LPARENTESIS;
         case ';':
            return token::SEMICOLON;
         case '.':
            return token::DOT;
         case ',':
            return token::COMMA;
         case '\\':
            return token::LAMBDA;
         case '?':
            return token::TERNARY;

         // group first, first first, first =
         case ':':
         {
            // posible ::, :=, :
            char second = peek_char(scn);
            switch (second)
            {
            case ':':
            {
               // ::
               get_char(scn);
               return token::DBL_COLON;
            }
            case '=':
            {
               // :=
               get_char(scn);
               return token::DEFINE;
            }
            default:
               // :
               return token::COLON;
            }
         }

         case '&':
         {
            // posible &, &&, &=
            char second = peek_char(scn);
            if (second == '&')
            {
               get_char(scn);
               return token::LAND;
            }
            else if (second == '=')
            {
               get_char(scn);
               return token::AND_ASSIGN;
            }
            else
            {
               return token::AMP;
            }
         }

         case '+':
         {
            // posible +, ++, +=
            char second = peek_char(scn);
            if (second == '=')
            {
               get_char(scn);
               return token::ADD_ASSIGN;
            }
            else if (second == '+')
            {
               get_char(scn);
               return token::INC;
            }
            else
            {
               return token::ADD;
            }
         }

         // group first, first first, first =, first >
         case '|':
         {
            // posible |, ||, |=, |>
            char second = peek_char(scn);
            switch (second)
            {
            case '|':
            {
               // ||
               get_char(scn);
               return token::LOR;
            }
            case '>':
            {
               // |>
               get_char(scn);
               return token::PIPE;
            }
            case '=':
            {
               // |=
               get_char(scn);
               return token::OR_ASSIGN;
            }
            default:
               // |
               return token::OR;
            }
         }

         case '-':
         {
            // posible -, --, -=, ->
            char second = peek_char(scn);
            switch (second)
            {
            case '-':
            {
               // --
               get_char(scn);
               return token::DEC;
            }
            case '=':
            {
               // -=
               get_char(scn);
               return token::SUB_ASSIGN;
            }
            case '>':
            {
               // ->
               get_char(scn);
               return token::ARROW;
            }
            default:
               // -
               return token::SUB;
            }
         }

         // group first, first first, first first =, first =
         case '<':
         {
            // posible <, <<, <<=, <=
            char second = peek_char(scn);
            switch (second)
            {
            case '<':
            {
               // << or <<=
               get_char(scn);
               if (peek_char(scn) == '=')
               {
                  // <<=
                  get_char(scn);
                  return token::SHL_ASSIGN;
               }
               else
               {
                  // <<
                  return token::SHL;
               }
            }
            case '=':
            {
               // <=
               get_char(scn);
               return token::LEQ;
            }
            default:
               // <
               return token::LSS;
            }
         }

         case '>':
         {
            // posible >, >>, >>=, >=
            char second = peek_char(scn);
            switch (second)
            {
            case '>':
            {
               // >> or >>=
               get_char(scn);
               if (peek_char(scn) == '=')
               {
                  // >>=
                  get_char(scn);
                  return token::SHR_ASSIGN;
               }
               else
               {
                  // >>
                  return token::SHR;
               }
            }
            case '=':
            {
               // >=
               get_char(scn);
               return token::GEQ;
            }
            default:
               // >
               return token::GTR;
            }
         }

         // group first, first =
         case '!':
         {
            // posible !, !=
            if (peek_char(scn) == '=')
            {
               get_char(scn);
               return token::NEQL;
            }
            else
            {
               return token::NOT;
            }
         }

         case '=':
         {
            // posible =, ==, =>
            if (peek_char(scn) == '=')
            {
               get_char(scn);
               return token::EQL;
            }
            else if (peek_char(scn) == '>')
            {
               get_char(scn);
               return token::FAT_ARROW;
            }
            else
            {
               return token::ASSIGN;
            }
         }

         case '~':
         {
            // posible ~, ~=
            if (peek_char(scn) == '=')
            {
               get_char(scn);
               return token::NEG_ASSIGN;
            }
            else
            {
               return token::NEG;
            }
         }

         case '^':
         {
            // posible ^, ^=
            if (peek_char(scn) == '=')
            {
               get_char(scn);
               return token::XOR_ASSIGN;
            }
            else
            {
               return token::XOR;
            }
         }

         case '%':
         {
            // posible %, %=
            if (peek_char(scn) == '=')
            {
               get_char(scn);
               return token::MOD_ASSIGN;
            }
            else
            {
               return token::MOD;
            }
         }

         case '/':
         {
            // posible /, /=
            if (peek_char(scn) == '=')
            {
               get_char(scn);
               return token::DIV_ASSIGN;
            }
            else
            {
               return token::DIV;
            }
         }

         case '*':
         {
            // posible *, *=
            if (peek_char(scn) == '=')
            {
               get_char(scn);
               return token::MUL_ASSIGN;
            }
            else
            {
               return token::STAR;
            }
         }

         default:
            return token::BAD_TOKEN;
         }
      }

      // reads the next token and pushes it to the back of the scanner token buffer.
      void read_next_token(scanner::Scanner &scn)
      {
         advance_whitespace(scn);
         token::Token tok;
         tok.pos = scn.current_position;
         tok.type = token::BAD_TOKEN;
         tok.val = "";

         char first = peek_char(scn);
         if (first == EOF)
         {
            tok.type = token::TEOF;
         }
         else if (is_letter(first))
         {
            // if first character is letter then we are reading either an identifier or a keyword.
            // read while is letter, digit or _
            tok.val = read_while(scn, [](char c) -> bool
                                 { return is_letter(c) || is_decimal_digit(c) || c == '_'; });
            token::TokenType keyword_type = token::get_keyword_type(tok.val);
            if (keyword_type == token::NO_KEYWORD)
            {
               tok.type = token::IDENTIFIER;
            }
            else
            {
               tok.type = keyword_type;
            }
         }
         else if (is_decimal_digit(first))
         {
            tok.type = token::NUM_LITERAL;
            tok.val = read_numerical_literal(scn);
         }
         else if (first == '\'')
         {
            tok.type = token::CHAR_LITERAL;
            tok.val = read_char_literal(scn);
         }
         else if (first == '\"')
         {
            tok.type = token::STR_LITERAL;
            tok.val = read_string_literal(scn);
         }
         else
         {
            tok.type = read_symbol(scn);
            tok.val = "";
         }
         scn.token_buffer.push_back(tok);
      }

   };

   void init(Scanner &scn, const char *file_path)
   {
      scn.current_position = {file_path, 0, 0};
      scn.file.open(file_path, std::fstream::in);
      scn.token_buffer = {};
   }

   token::Token get(Scanner &scn)
   {
      if (scn.token_buffer.size() == 0)
      {
         read_next_token(scn);
      }
      token::Token tok = scn.token_buffer.front();
      scn.token_buffer.pop_front();
      return tok;
   }

   token::Token peek(Scanner &scn)
   {
      if (scn.token_buffer.size() == 0)
      {
         read_next_token(scn);
      }
      return scn.token_buffer.front();
   }

   token::Token peek_2(Scanner &scn)
   {
      while (scn.token_buffer.size() < 2)
      {
         read_next_token(scn);
      }
      return *std::next(scn.token_buffer.begin());
   }

   token::Token peek_n(Scanner &scn, int n)
   {
      while (scn.token_buffer.size() < 2)
      {
         read_next_token(scn);
      }
      auto it = scn.token_buffer.begin();
      for (int i = 0; i < n; i++)
      {
         it = std::next(it);
      }
      return *it;
   }

   void unget(Scanner &scn, token::Token tok)
   {
      scn.token_buffer.push_front(tok);
   }

   void clean_up(Scanner &scn)
   {
      scn.file.close();
   }
}
