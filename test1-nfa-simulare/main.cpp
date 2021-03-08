#include <iostream>
#include <set>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <algorithm>

using namespace std;

ifstream f ("in.in");

struct hash_pair {
    template <class T1, class T2>
    size_t operator()(const pair<T1, T2>& p) const
    {
        auto hash1 = hash<T1>{}(p.first);
        auto hash2 = hash<T2>{}(p.second);
        return hash1 ^ hash2;
    }
};

typedef pair<string,char> transition;
typedef unordered_map<transition,set<string>,hash_pair> nfa_type;
typedef pair<set<string>,string> nfa_state;
typedef vector<nfa_state> nfa_states_type;

void check(string w,nfa_type nfa,vector<string> finale,string nod_start){
    nfa_states_type nfa_states;
    nfa_states.push_back(nfa_state(set<string> {nod_start},w));

    for (int c_i = 0; c_i < w.length(); c_i++){
        char c = w[c_i];
        set<string> next_nodes;

        for (string node: nfa_states.back().first){
            transition to_check(node, c);
            if (nfa.find(to_check) != nfa.end()){
                for (string node2: nfa[to_check])
                    next_nodes.insert(node2);
            }
        }
        if (next_nodes.size() == 0){
            cout << "NU\n";
            return;
        }
        nfa_states.push_back(nfa_state(next_nodes,w.substr(c_i+1,w.length()-c_i-1)));
    }
    for (string nod: nfa_states.back().first)
        if (find(finale.begin(),finale.end(),nod) != finale.end()){
            cout << "DA\n";
            /*
            for (auto state: nfa_states){
                for (auto n: state.first)
                    cout << n << ",";
                cout << " - ";
                cout << state.second << " --> ";
            }
            cout << '\n';
            */
            return;
        }
    cout << "NU\n";
    return;
}

int main()
{
    nfa_type nfa;

    int n;
    f >> n;
    int nr_m;
    f >> nr_m;
    for (int i = 0; i < nr_m; i++){
        string n_start, n_end;
        char s;
        f >> n_start >> n_end >> s;

        transition new_trans(n_start,s);
        nfa[new_trans].insert(n_end);
    }

    string nod_start;
    f >> nod_start;
    int nr_finale;
    f >> nr_finale;
    vector<string> finale;
    for (int i = 0; i < nr_finale; i++){
        string _final;
        f >> _final;
        finale.push_back(_final);
    }

    int nr_tests;
    f >> nr_tests;
    for (int i = 0; i < nr_tests; i++){
        string w;
        f >> w;
        check(w,nfa,finale,nod_start);
    }

    return 0;
}
