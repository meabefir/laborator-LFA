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

#define SHOW_STATES 1

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

bool check(string w, dfa_type &dfa, unordered_map<string, int> &is_final, string start_node){
    vector<dfa_state> dfa_states;
    dfa_states.push_back(dfa_state(start_node,w));

    for (int i = 0; i < w.length(); i++){
        char l = w[i];
        transition check(dfa_states.back().first,l);

        if (dfa.find(check) != dfa.end())
            dfa_states.push_back(dfa_state(dfa[check],w.substr(i+1,w.length()-i-1)));
        else
            return false;
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
    // build temporary map to keep the final nodes
    unordered_map<string, int> temp_is_final;
    f >> nr_final_nodes;
    for (int i = 0; i < nr_final_nodes; i++){
        string final_node;
        f >> final_node;
        temp_is_final[final_node] = 1;
    }
    // build the nfa matrix
    // we add the start node to a queue
    // we see where the first node can go following each symbol and make a set for for each symbol
    // if the set hasn't been in the queue yet, we add it
    queue<set<string>> q;
    set<string> start_node_set = {start_node};
    nfa_sets[start_node_set] = to_string(nfa_sets_count++);
    q.push(start_node_set);
    unordered_map<string, int> is_final;

    while (q.size()){
        for (auto symbol: symbols){
            set<string> can_travel_to;
            for (auto node_str: q.front()){
                transition trans_to_add(node_str, symbol.first);
                can_travel_to.insert(nfa[trans_to_add].begin(),nfa[trans_to_add].end());
            }

            if (can_travel_to.size() == 0)
                continue;

            // check if you can add this set to the map
            if (nfa_sets.find(can_travel_to) == nfa_sets.end()){;
                nfa_sets[can_travel_to] = to_string(nfa_sets_count++);
                q.push(can_travel_to);
            }

            // check for final nodes in can_travel_to
                for (auto node: can_travel_to)
                    if (temp_is_final.find(node) != temp_is_final.end())
                        is_final[nfa_sets[can_travel_to]] = 1;

            // we can also start building the dfa here
            transition this_trans = make_pair(nfa_sets[q.front()],symbol.first);
            if (dfa.find(this_trans) == dfa.end())
                dfa[this_trans] = nfa_sets[can_travel_to];

        }
        q.pop();
    }

    /*
    cout << "\n\n\n";
    for (auto nfa_set: nfa_sets)
    {
        cout << "{ ";
        for (auto el: nfa_set.first)
            cout << el << ' ';
        cout << " } -- >";
        cout << nfa_set.second << '\n';
    }
    cout << "\n\n\n\n";

    // describe dfa rules

    for (auto it2: dfa){
        cout << it2.first.first << ' ' << it2.first.second << ' ' << it2.second << '\n';
    }
    cout << "\n\n\n";

    cout << "Finals \n";
    for (auto f: is_final)
        cout << f.first << ' ' ;
    cout << '\n';
    */

    f >> word_count;
    for (int i = 0; i < word_count; i++){
        string w;
        f >> w;
        bool passed = check(w, dfa, is_final, string("1"));
        if (passed) nr_passed ++;
        cout << w << " --> " << passed << '\n';
    }

    cout << nr_passed << " passed the test.\n";

    return 0;
}
