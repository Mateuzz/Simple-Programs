#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <cstdlib>

class Token {
public:
    char kind;
    bool value;
    std::string name;

    enum kind : char {
        logic_value = 'v',
        identifier = 'i',
        declaration = 'd',
        print_value = 'f',
    };
    
    Token(char ch);
    Token(char c, bool value);
    Token(char ch, std::string name);
};

class TokenStream {
public:
    void putback(Token t);
    Token get();
    TokenStream();

private:
    Token buff;
    bool full;
};

class Variable {
public:
    std::string name;
    bool value;
    Variable(std::string name, bool value);
};

class VariableTable {
public:
    bool get_value(std::string name);
    bool declaration();

private:
    void add_to_list(Variable v);
    std::vector<Variable> list;
};

void error(std::string s)
{
    throw std::runtime_error(s);
}

Token::Token(char ch) : kind(ch), value(false) {}
Token::Token(char c, bool value) : kind(c), value(value) {} 
Token::Token(char ch, std::string name) : kind(ch), name(name) {}

TokenStream::TokenStream() : buff(0), full(false) {}

Variable::Variable(std::string name, bool value) : name(name), value(value) {}

void TokenStream::putback(Token t)
{
    if (full)
        error("Stream de token com buff lotado, error no putback()");
    buff = t;
    full = true;
}

Token TokenStream::get()
{
    if (full) {
        full = false;
        return buff;
    }

    char ch;
    do {
        std::cin.get(ch);
        if (!std::cin)
            std::exit(EXIT_SUCCESS);
        if (ch == '\n' || ch == ';')
            return Token(Token::print_value);
    } while (isspace(ch));

    switch (ch) {
    case '^': case 'v': case '|':
    case '<': case '>':
    case '~': 
    case ')': case '(':
    case '=': case 'x':
        return Token(ch);
    case 'V':
        return Token(Token::logic_value, true);
    case 'F':
        return Token(Token::logic_value, false);
    default:
        if (isalpha(ch)) {
            std::string s;
            s += ch;
            while (std::cin.get(ch) && (ch == '_' || isalpha(ch) ||
                   isdigit(ch)))
                s += ch;
            std::cin.unget();
            if (s == "let")
                return Token(Token::declaration);
            else
                return Token(Token::identifier, s);
        }
        error("Inválido input");
    }
}

void VariableTable::add_to_list(Variable v)
{
    for (int i = 0; i < list.size(); ++i)
        if (v.name == list[i].name)
            error("Variável já existe");
    list.push_back(v);
}

bool VariableTable::get_value(std::string name)
{
    for (const auto &i : list)
        if (i.name == name)
            return i.value;
    std::cerr << name << ' ';
    error("Variável não existe");
}

bool expression();

TokenStream ts;

bool VariableTable::declaration()
{
    Token t = ts.get();
    if (t.kind != Token::identifier)
        error("Nome de váriavel esperado");
    std::string name = t.name;

    t = ts.get();
    if (t.kind != '=')
        error("Esperado '=' após identificador");

    bool val = expression();
    add_to_list(Variable(name, val));
    return val;
}

VariableTable table;

bool bicondicional(bool p, bool n)
{
    return p == n;
}

bool condicional(bool p, bool n)
{
    return !p || (p && n);
}

bool primary()
{
    Token t = ts.get();
    bool p;

    switch (t.kind) {
    case '(':
        p = expression();
        t = ts.get();
        if (t.kind != ')')
            error("')' esperado");
        return p;
    case Token::logic_value:
        return t.value;
    case Token::identifier:
        return table.get_value(t.name);
    default:
        std::cerr << "Seu Token: " << t.kind << '\n';
        error("Valor lógico esperado");
    }
}

bool unit()
{
    Token t = ts.get();
    if (t.kind == '~')
        return !primary();
    else {
        ts.putback(t);
        return primary();
    }
}

bool term()
{
    bool left = unit();

    for (;;) {
        Token t = ts.get();
        switch (t.kind) {
        case '^':
            left &= unit();
            break;
        case 'v':
            left |= unit();
            break;
        case '|':
            left ^= unit();
            break;
        default:
            ts.putback(t);
            return left;
        }
    }

}

bool expression()
{
    bool left = term();

    for (;;) {
        Token t = ts.get();
        switch (t.kind) {
        case '>':
            left = condicional(left, term());
            break;
        case '<':
            left = bicondicional(left, term());
            break;
        default:
            ts.putback(t);
            return left;
        }
    }
}

bool statement()
{
    Token t = ts.get();
    switch (t.kind) {
    case Token::declaration:
        return table.declaration();
    default:
        ts.putback(t);
        return expression();
    }
}

const char* logic_value(bool value)
{
    return value ? "Verdade" : "Falsidade";
}

inline void print_promp()
{
    std::cout << "> ";
}

int main()
{
    std::cout << "Operadores: , '~' (nao), '^' (e), 'v' (ou),"
              << " '|' (xor), '>' (condicional), '<' (bicondicional),"
              << " '('parenteses')'\n"
              << "Digite ';' ou dê enter para ver o resultado\n"
              << "Criar variavel: 'let variavel_nome = expressao'\n";

    std::cout << "Usar (V) ou (F) para as proposições lógicas diretas\n";

    try {
        while (std::cin) {
            print_promp();

            Token t = ts.get();

            while (t.kind == Token::print_value)
                t = ts.get();
            ts.putback(t);

            if (t.kind == 'x')
                break;

            bool val = statement();
            std::cout << "= " << logic_value(val) << '\n';
        }
    } catch(std::exception& e) {
        std::cerr << e.what() << '\n';
        return 1;
    } catch (...) {
        std::cerr << "Erro desconhecido\n";
        return 2;
    }
}

