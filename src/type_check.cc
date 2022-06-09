#include "../include/type_check.hh"

namespace type_check{

    TYPE_COMPATIBILITY type_comp(ast::Type* a, ast::Type* b){
        if(a->is_const != b->is_const || a->is_pointer != b->is_pointer){
            return TYPE_CMP_INCOMP;
        }
        
    }

    std::string get_literal_type(ast::LiteralExp* lit){
        switch(lit->type){
            case ast::LIT_IDENTIFIER: return "typeof("+lit->val+")";
            case ast::LIT_NUM: return (lit->val.find('.') != std::string::npos) ? "i32" : "f64";
            case ast::LIT_STR: return "str";
            case ast::LIT_CHAR: return "u8";
            //case ast::LIT_FN: return get_lambda_fn_type(); 
            default: return "invalid_type";  
        }
    }

    std::string get_binary_exp_result_type(std::string a, std::string b){
        
    }

    std::string get_exp_result_type(ast::Exp*);
};