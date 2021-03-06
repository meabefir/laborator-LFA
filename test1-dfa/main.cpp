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
#include <unordered_map>
#include <fstream>

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

void show_states(vector<dfa_state> &dfa_states){
    for (int i = 0; i < dfa_states.size(); i++)
        cout << dfa_states[i].first << ' ' << dfa_states[i].second << " --> ";
    cout << '\n';
}

bool check(string w, dfa_type &dfa, unordered_map<string, int> &is_final, string start_node){
    vector<dfa_state> dfa_states;
    dfa_states.push_back(dfa_state(start_node,w));

    for (int i = 0; i < w.length(); i++){
        char l = w[i];
        transition check(dfa_states.back().first,l);

        if (dfa.find(check) != dfa.end()){
            dfa_states.push_back(dfa_state(dfa[check],w.substr(i+1,w.length()-i-1)));
        } else {
            return false;
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
    unordered_map<string, int> is_final;

    string start_node;
    int nr_transitions, nr_final_nodes, word_count, nr_passed = 0;

    f >> start_node >> nr_transitions;
    for (int i = 0; i < nr_transitions; i++){
        string node1,node2;
        char c;
        f >> node1 >> node2 >> c;

        transition this_trans = make_pair(node1,c);

        dfa[this_trans] = node2;
    }
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
        bool passed = check(w, dfa, is_final, start_node);
        if (passed) nr_passed ++;
        cout << w << " --> " << passed << '\n';
    }

    cout << nr_passed << " passed the test.\n";

    return 0;
}
