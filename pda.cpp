#include <iostream>
#include <vector>
#include <stack>
#include <string>
using namespace std;
//states
class state {
public:
    bool final = false;
    vector<class transition*> transitions;
};
//transitions
class transition {
public:
    //current state
    state* current;
    //next state
    state* next;
    //top of stack
    char z;
    //poping from stack
    char pop;
    //pushing to stack
    vector<char> push;
};
//machine
class pda {
public:
    vector<state*> states;
};

stack<char> StackDup(stack<char> first) {
    stack<char> nw;
    stack<char> tmp;
    while (!first.empty()) {
        tmp.push(first.top());
        first.pop();
    }
    while (!tmp.empty()) {
        first.push(tmp.top());
        nw.push(tmp.top());
        tmp.pop();
    }
    return nw;
}

pda MachineCreator(string str[], int States, vector<string> transitionStr, string finalStates[]) {
    pda result;
    for (int i = 0; i < States; i++) {
        state* n = new state();
        result.states.push_back(n);
    }
    for (auto t : transitionStr) {
        result.states[stoi(t)]->final = true;
    }
    for (string t : transitionStr) {
        transition* current = new transition();
        string split[4];
        size_t pos = 0;
        for (int i = 0; i < 3; i++) {
            pos = t.find(",");
            split[i] = t.substr(0, pos);
            t.erase(0, pos + 1);
        }
        split[3] = t;
        current->current = result.states[stoi(split[0])];
        current->current->transitions.push_back(current);
        current->z = split[1][0];
        pos = split[2].find("->");
        current->next = result.states[stoi(split[2].substr(pos + 2))];
        current->pop = split[2][0];
        for (char i : split[3]) {
            if (i != '@')
                current->push.push_back(i);
        }
    }
    return result;
}

bool CheckStringInMachine(state* current, string inputs, stack<char> stack_1) {
    if (inputs.empty()) {
        return current->final;
    }
    for (transition* tt : current->transitions) {
        if (tt->z == inputs[0] && stack_1.top() == tt->pop) {
            stack<char> NextStack = StackDup(stack_1);
            NextStack.pop();
            for (int i = tt->push.size() - 1; i >= 0; i--)
                NextStack.push(tt->push[i]);
            string NextInput = inputs.substr(1);
            if (CheckStringInMachine(tt->next, NextInput, NextStack))
                return true;
        }
    }
    return false;
}

int main() {
    int LetterNum;
    cin >> LetterNum;
    string Letters[LetterNum];
    for (int i = 0; i < LetterNum; i++)
        cin >> Letters[i];
    int stateNum, transictionNum;
    cin >> stateNum >> transictionNum;
    vector<string> transitionStr(transictionNum);
    for (int i = 0; i < transictionNum; i++)
        cin >> transitionStr[i];
    int finalStates;
    cin >> finalStates;
    string finalStateStr[finalStates];
    for (int i = 0; i < finalStates; i++)
        cin >> finalStateStr[i];
    pda machine = MachineCreator(Letters, stateNum, transitionStr, finalStateStr);
    vector<string> inputs;
    vector<bool> ans;
    while (true) {
        string inp;
        cin >> inp;
        if (inp == "$")
            break;
        inputs.push_back(inp);
        stack<char> sstack;
        sstack.push('Z');
        ans.push_back(CheckStringInMachine(machine.states[0], inp, sstack));
    }
    for (size_t i = 0; i < ans.size(); i++) {
        if (ans[i])
            cout << inputs[i] << ": Accept\n";
        else
            cout << inputs[i] << ": Reject\n";
    }
    return 0;
}
