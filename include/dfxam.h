#ifndef DFXAM
#define DFXAM

#include <string>
#include <sstream>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <stack>
#include <tuple>

namespace dfxam {
namespace repl {
    class ExecutionEngine;
} // repl

namespace ast {
class Function;
class Expression {
    public:
        virtual Expression* derivative(repl::ExecutionEngine* eng, Function* respect) = 0;
        virtual Expression* substitute(repl::ExecutionEngine* eng) = 0;
        virtual Expression* simplify(repl::ExecutionEngine* eng) = 0;
        virtual Function* getVar(repl::ExecutionEngine* eng) = 0;
        virtual std::string toString() const = 0;

        virtual bool equals(repl::ExecutionEngine* eng, Expression* expr) = 0;
        virtual Expression* clone() = 0;

        virtual bool isConstant() const;

        virtual ~Expression() {};
};

class Constant: public Expression {
    public:
        Constant(double val);
        double getValue() const;

        Expression* derivative(repl::ExecutionEngine* eng, Function* respect) override;
        Expression* substitute(repl::ExecutionEngine* eng) override;
        Expression* simplify(repl::ExecutionEngine* eng) override;
        Function* getVar(repl::ExecutionEngine* eng) override;
        std::string toString() const override;

        bool equals(repl::ExecutionEngine* eng, Expression* expr) override;
        Expression* clone() override;

        bool isConstant() const override;
        static bool isConstantValue(Expression* expr, int val);

    private:
        double value;
};

class E : public Constant {
    public:
        E();

        std::string toString() const override;

        Expression* clone() override;
};

class Function : public Expression {
    public:
        Function(std::string name);

        std::string getName() const;
        Expression* derivative(repl::ExecutionEngine* eng, Function* respect) override;
        Expression* substitute(repl::ExecutionEngine* eng) override;
        Expression* simplify(repl::ExecutionEngine* eng) override;
        Function* getVar(repl::ExecutionEngine* eng) override;
        std::string toString() const override;

        bool equals(repl::ExecutionEngine* eng, Expression* expr) override;
        Expression* clone() override;

    private:
        std::string name; 
};

class Invocation : public Expression {
    public:
        Invocation(Expression* expr, std::vector<Expression*> inputs);

        Expression* getExpression() const;
        std::vector<Expression*>& getArguments();

        Expression* derivative(repl::ExecutionEngine* eng, Function* respect) override;
        Expression* substitute(repl::ExecutionEngine* eng) override;
        Expression* simplify(repl::ExecutionEngine* eng) override;
        Function* getVar(repl::ExecutionEngine* eng) override;
        std::string toString() const override;

        bool equals(repl::ExecutionEngine* eng, Expression* expr) override;
        Expression* clone() override;

        ~Invocation() override;

    private:
        Expression* expr;
        std::vector<Expression*> arguments;
};

class Differentiation : public Expression {
    public:
        Differentiation(Expression* expr);
        Differentiation(Expression* expr, Function* respect);
        Expression* getExpression() const;
        Function* getRespect() const;

        Expression* derivative(repl::ExecutionEngine* eng, Function* respect) override;
        Expression* substitute(repl::ExecutionEngine* eng) override;
        Expression* simplify(repl::ExecutionEngine* eng) override;
        Function* getVar(repl::ExecutionEngine* eng) override;
        std::string toString() const override;

        bool equals(repl::ExecutionEngine* eng, Expression* expr) override;
        Expression* clone() override;

        ~Differentiation() override;

    private:
        Expression* expr;
        Function* respect;
};

class BinaryOperator : public Expression {
    public:
        BinaryOperator();
        BinaryOperator(Expression* left, Expression* right);

        void setLeft(Expression* l);
        void setRight(Expression* r);

        Function* getVar(repl::ExecutionEngine* eng) override;

        Expression* getLeft() const;
        Expression* getRight() const;

        ~BinaryOperator() override;

    private:
        Expression* left;
        Expression* right;
};

class Power : public BinaryOperator {
    public:
        Power(Expression* left, Expression* right);

        Expression* derivative(repl::ExecutionEngine* eng, Function* respect) override;
        Expression* substitute(repl::ExecutionEngine* eng) override;
        Expression* simplify(repl::ExecutionEngine* eng) override;
        std::string toString() const override;

        bool equals(repl::ExecutionEngine* eng, Expression* expr) override;
        Expression* clone() override;
};

class Log : public BinaryOperator {
    public:
        Log(Expression* left, Expression* right);

        Expression* derivative(repl::ExecutionEngine* eng, Function* respect) override;
        Expression* substitute(repl::ExecutionEngine* eng) override;
        Expression* simplify(repl::ExecutionEngine* eng) override;
        std::string toString() const override;

        bool equals(repl::ExecutionEngine* eng, Expression* expr) override;
        Expression* clone() override;
};

class Sum : public BinaryOperator {
    public:
        Sum(Expression* left, Expression* right);

        Expression* derivative(repl::ExecutionEngine* eng, Function* respect) override;
        Expression* substitute(repl::ExecutionEngine* eng) override;
        Expression* simplify(repl::ExecutionEngine* eng) override;
        std::string toString() const override;

        bool equals(repl::ExecutionEngine* eng, Expression* expr) override;
        Expression* clone() override;
};

class Difference : public BinaryOperator {
    public:
        Difference(Expression* left, Expression* right);

        Expression* derivative(repl::ExecutionEngine* eng, Function* respect) override;
        Expression* substitute(repl::ExecutionEngine* eng) override;
        Expression* simplify(repl::ExecutionEngine* eng) override;
        std::string toString() const override;

        bool equals(repl::ExecutionEngine* eng, Expression* expr) override;
        Expression* clone() override;
};

class Product : public BinaryOperator {
    public:
        Product(Expression* left, Expression* right);

        Expression* derivative(repl::ExecutionEngine* eng, Function* respect) override;
        Expression* substitute(repl::ExecutionEngine* eng) override;
        Expression* simplify(repl::ExecutionEngine* eng) override;
        std::string toString() const override;

        bool equals(repl::ExecutionEngine* eng, Expression* expr) override;
        Expression* clone() override;
};

class Quotient : public BinaryOperator {
    public:
        Quotient(Expression* left, Expression* right);

        Expression* derivative(repl::ExecutionEngine* eng, Function* respect) override;
        Expression* substitute(repl::ExecutionEngine* eng) override;
        Expression* simplify(repl::ExecutionEngine* eng) override;
        std::string toString() const override;

        bool equals(repl::ExecutionEngine* eng, Expression* expr) override;
        Expression* clone() override;
};

// Special instruction for ExecutionEngine
// to store function in memory
class Assignment : public Expression {
    public:
        Assignment(Invocation* f, Expression* expr);

        Invocation* getDeclaration() const;
        Expression* getExpression() const;

        Expression* derivative(repl::ExecutionEngine* eng, Function* respect) override;
        Expression* substitute(repl::ExecutionEngine* eng) override;
        Expression* simplify(repl::ExecutionEngine* eng) override;
        Function* getVar(repl::ExecutionEngine* eng) override;
        std::string toString() const override;

        bool equals(repl::ExecutionEngine* eng, Expression* expr) override;
        Expression* clone() override;

        ~Assignment();

    private:
        Invocation* f;
        Expression* expr;
};

} // ast

namespace repl {

enum class TokenType { IDENTIFIER, NUMBER, OPERATOR, SEPARATOR };

class Token {
    public:
        Token(TokenType type, std::string contents);
        bool match(TokenType type, std::string contents);

        std::string getContents() const;
        TokenType getType() const;

    private:
        TokenType type;
        std::string contents;
};

class Lexer {
    public:
        Lexer(std::string s);

        std::vector<Token> lex();
        static bool isOperator(char c);
        static bool isSeparator(char c);
        static bool isLetter(char c);
        static bool isNumber(char c);

    private:
        char& peek();
        void consume();

        void lexIdentifier();
        void lexNumber();
        void lexSeparator();
        void lexOperator();

        std::string s;
        int index;
        std::vector<Token> tokens;
};

enum class Associativity { LEFT, RIGHT };

typedef std::tuple<Associativity, int> OperatorInfo;
static std::unordered_map<std::string, OperatorInfo> OP_INFO = {
    {"+", std::make_tuple(Associativity::LEFT, 1)},
    {"-", std::make_tuple(Associativity::LEFT, 1)},
    {"*", std::make_tuple(Associativity::LEFT, 2)},
    {"/", std::make_tuple(Associativity::LEFT, 2)},
    {"^", std::make_tuple(Associativity::RIGHT, 3)}
};

class Parser {
    public:
        Parser(std::vector<Token> tokens);
        ast::Expression* parse();

    private:
        bool match(TokenType t, std::string s, int ahead);
        Token& peek();
        void consume();

        ast::Expression* parseAssignment();

        ast::Expression* parseExpression(int minPrecedence = 0);
        ast::Expression* parseAtom();
        ast::Expression* parseBinop(std::string op, ast::Expression* lhs, ast::Expression* rhs);

        ast::Expression* parseInvocation(ast::Expression* expr);
        ast::Expression* parseConstant();

        std::vector<Token> tokens;
        int index;

};

class ExecutionEngine;
class Function {
    public:
        Function(std::string name, ast::Expression* expr);
        Function(ast::Assignment* a); 

        std::string getName() const;
        std::vector<std::string> getInputs() const;
        ast::Expression* getExpression() const;

    private:
        std::string name;
        std::vector<std::string> inputs;
        ast::Expression* expr;
};

class Frame {
    public:
        Frame(std::vector<ast::Expression*> args);

        bool bindParameters(std::vector<std::string> params);
        std::vector<std::string> getParams();
        ast::Expression* getArg(std::string name);

    private:
        std::unordered_map<std::string, int> parameters;
        std::vector<ast::Expression*> arguments;
};

class ExecutionEngine {
    public:
        void registerFunction(Function* f);
        Function* retrieveFunction(std::string name);
        void deregisterFunction(std::string name);

        std::vector<std::string> getFrameParams();
        ast::Expression* getFrameArg(std::string name);
        bool bindFrameParameters(std::vector<std::string> params);
        void pushFrame(std::vector<ast::Expression*> args);
        void popFrame();

        void operator <<(std::string& expr);
        void execute(ast::Expression* expr);

    private:
        std::unordered_map<std::string, Function*> functions;
        std::stack<Frame> callStack;
};

} // repl
} // dfaxm

std::ostream& operator <<(std::ostream& out, const dfxam::ast::Expression* expr);

#endif
