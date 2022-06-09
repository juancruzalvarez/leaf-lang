#ifndef TYPE_CHECK_H
#define TYPE_CHECK_H
#include <string>
#include "expression.hh"
#include "type.hh"

namespace type_check{
    enum TYPE_COMPATIBILITY{
        TYPE_CMP_EQUALS,
        TYPE_CMP_A_CAN_HOLD_B,
        TYPE_CMP_B_CAN_HOLD_A,
        TYPE_CMP_INCOMP,
    };

    TYPE_COMPATIBILITY type_comp(ast::Type* a, ast::Type* b);
    std::string get_literal_type(ast::LiteralExp* lit);
    std::string get_binary_exp_result_type(std::string a, std::string b);
    std::string get_exp_result_type(ast::Exp*);
    
};

#endif