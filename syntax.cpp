#include "std_lib_facilities.h"

enum State {initial = 1, intermediate = 2, final = 3, failed = 4};

const char lft = 'M'; // Left (opening) token character.
const char rgt = 'N'; // Right (closing) token character.
const char trm = ';'; // Sentence-terminating token character.
const char btm = '$'; // Stack-bottom token character.
const int lft_id = -1; // Opening token id. 
const int rgt_id = 1; // Closing token id.
const int trm_id = 0; // Terminating token id.
const int btm_id = -273; // Stack-bottom token id.


State state = initial;


class Token 
{
public:
    int id;
    char value;
    Token() : id{btm_id}, value{btm} {}
    Token(int i, char v) :id{i}, value{v} {}
};

// Node used in the stack of the automaton.
class Token_node
{
public:    
    Token token;
    Token_node *next;
    Token_node() : token{Token()}, next{NULL} {}
    Token_node(Token t) : token{t}, next{NULL} {}
};


// Linked list implementation of the automaton's stack.
class Stack
{
public:
    Token top; // Token of the Token_node at the top of the stack.
    void pop();
    void push(Token t);
    bool empty();
    void show();
    Stack() // Constructor. Creates a stack and puts the bottom token in.
    {
        bottom = Token();
        bottom_node = Token_node(bottom);
        head = &bottom_node;
    }
private:
    Token bottom;
    Token_node bottom_node;
    Token_node *head;
};

void Stack::pop()
{
    if (Stack::empty()) error("Cannot pop empty stack");
    head = head->next;
    top = head->token;
    return;
}

void Stack::push(Token t)
{
    Token_node *temp;
    temp = (Token_node*)malloc(sizeof(Token_node));
    temp->token = t;

    if (head == &bottom_node)
    {
        head = temp;
        head->next = &bottom_node;
    }
    else
    {
        temp->next = head;
        head = temp;
    }
    top = head->token;
}

bool Stack::empty()
{
    if (head == &bottom_node) return true;
    return false;
}

void Stack::show()
{
    Token_node *n;
    n = head;
    cout << bottom_node.token.value;
    while (n != &bottom_node)
    {
        cout << n->token.value;
        n = n->next;
    }
    cout << ' ';
}

Stack t_stack;


// Parser. Parses the input stream and saves in the class.
class Stream_parser
{
public: 
    void parse_stream();
    string get_stream();
    string stream {""};
};


void Stream_parser::parse_stream()
{
    char ch;
    cin >> ch;
    while (ch != trm) 
    {
        stream.push_back(ch);
        cin >> ch;
    }
}


// Receives a character and returns the respective Token.
Token tokenize(char ch)
{
    switch (ch)
    {
    case lft: return Token(lft_id, lft);
    case rgt: return Token(rgt_id, rgt);
    case trm: return Token(trm_id, trm);
    default:
        error("Unknown token\n");
    }
}


/* Implements the transition function:
p(k1, $, lft) = {k2, $}
p(k2, $, lft) = {k2, $lft}
p(k2, $, rgt) = {k3, $}
p(k2, lft, lft) = {k2, lftlft}
p(k2, $lft, rgt) = {k2, $}
p(k3, $, lft) = {k2, $}
p(k3, $, rgt) = {k4, $}
*/
void transition(Token next_token)
{
    switch (state)
    {
    case initial:
        if (next_token.value == lft)
        {
            state = intermediate;
            return;
        }
        else
        {
            error("Cannot start with token N.\n");
        }
    case intermediate:
        if (next_token.value == lft)
        {
            t_stack.push(next_token);
            return;
        }
        else 
        {
            if (t_stack.empty())
            {
                state = final;
                return;
            }
            else
            {
                t_stack.pop();
                return;
            }
        }
    case final:
        if (next_token.value == lft)
        {
            state = intermediate;
            return;
        }
        if (next_token.value == rgt)
        {
            state = failed;
            return;
        }
    case failed:
        return;
    default:
        break;
    }
    return;
}


/* Reads the input stream and prints the content of the stack, the current 
state and the remaining stream in each step.
*/
void read(string stream)
{
    char ch;
    Token t;


    cout << "Stack  State   Remaining stream\n";
    for (int i = 0; i < stream.length(); i++)
    {
        t_stack.show();
        cout << "     " << state << "       ";
        cout << stream.substr(i) << ' ';
        ch = stream[i];
        t = tokenize(ch);
        transition(t);
        cout << '\n';
    }
    t_stack.show();
    cout << "     " << state << '\n';
} 


const char prompt = '>';
Stream_parser parser;

int main()
{
    cout << prompt << ' ';
    parser.parse_stream();

    read(parser.stream);

    if (state == final) cout << "YES\n";
    else cout << "NO\n";
}