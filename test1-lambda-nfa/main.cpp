/*
structura input

linia 1 => nodul de intrare
linia 2 => numarul de muchii m
urmatoarele m linii => <nod_start> <nod_final> <caracter>
urmatoarea linie => nr de noduri finale n
urmatoarele n linii => nod final
urmatoarea linie => nr de cuvinte de verificat nr_cuv
urmatoarele nr_cuv linii => cate un cuvant de verificat
*/

#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>
#include <fstream>
#include <queue>
#include <set>

using namespace std;

ifstream f ("in.in");

#define SHOW_STATES 0

struct hash_pair {
    template <class T1, class T2>
    size_t operator()(const pair<T1, T2>& p) const
    {
        auto hash1 = hash<T1>{}(p.first);
        auto hash2 = hash<T2>{}(p.second);
        return hash1 ^ hash2;
    }
};

typedef pair<string, char> transition;
typedef unordered_map<transition, string, hash_pair> dfa_type;
typedef pair<string, string> dfa_state;
typedef unordered_map<transition, vector<string>, hash_pair> nfa_type;

void show_states(vector<dfa_state> &dfa_states){
    for (int i = 0; i < dfa_states.size(); i++)
        cout << dfa_states[i].first << ' ' << dfa_states[i].second << " --> ";
    cout << '\n';
}

bool check(string w, dfa_type &dfa, unordered_map<string,set<string>> &l_inchideri, unordered_map<string, int> &is_final, string start_node){
    vector<pair<set<string>,string>> dfa_states;
    set<string> first = {start_node};
    pair<set<string>,string> first_state(first,w);
    dfa_states.push_back(first_state);

    for (int i = 0; i < w.length(); i++){
        char l = w[i];

        // apply l_inchideri

        set<string> after;
        for (auto str: dfa_states.back().first){
            for (auto str2: l_inchideri[0]){
                after.insert(str2);
            }
        }
        pair<set<string>,string> next_state(after,w);
        dfa_states.push_back(next_state);

        // check if nodes from l_inchideri has transition
        for (auto str: dfa_states.back().first){
            set<string> can_go_to;

        }
    }

    if (is_final[dfa_states.back().first] == 1){
        if (SHOW_STATES)
            show_states(dfa_states);
        return true;
    }
    return false;
}

int main()
{
    dfa_type dfa;
    nfa_type nfa;
    unordered_map<char, int> symbols;
    map<set<string>, string> nfa_sets;
    unsigned nfa_sets_count = 1;

    string start_node;
    int nr_transitions, nr_final_nodes, word_count, nr_passed = 0;

    // read nr of nodes and the transitions
    f >> start_node >> nr_transitions;
    for (int i = 0; i < nr_transitions; i++){
        string node1,node2;
        char c;
        f >> node1 >> node2 >> c;
        symbols[c] = 1;

        transition this_trans = make_pair(node1,c);

        nfa[this_trans].push_back(node2);
    }

    unordered_map<string,set<string>> l_inchideri;
    for (auto tran: nfa){
        cout << tran.first.first << ' ' << tran.first.second << '\n';
        if (tran.first.second == '_'){
            set<string> lambda_inchidere;
            lambda_inchidere.insert(tran.first.first);

            queue<string> q;
            q.push(tran.first.first);
            while (q.size()){
                transition check(q.front(),'_');
                if (nfa.find(check) != nfa.end()){
                    for (auto nod: nfa[check]){
                        if (lambda_inchidere.find(nod) == lambda_inchidere.end()){
                            lambda_inchidere.insert(nod);
                            q.push(nod);
                        }
                    }
                }
                q.pop();
            }
            lambda_inchidere.insert(tran.first.first);
            l_inchideri[tran.first.first] = lambda_inchidere;
        }
    }

    // build temporary map to keep the final nodes
    unordered_map<string, int> is_final;
    f >> nr_final_nodes;
    for (int i = 0; i < nr_final_nodes; i++){
        string final_node;
        f >> final_node;
        is_final[final_node] = 1;
    }

    f >> word_count;
    for (int i = 0; i < word_count; i++){
        string w;
        f >> w;
        bool passed = check(w, dfa, l_inchideri, is_final, start_node);
        if (passed) nr_passed ++;
        cout << w << " --> " << passed << '\n';
    }

    cout << nr_passed << " passed the test.\n";

    return 0;
}
