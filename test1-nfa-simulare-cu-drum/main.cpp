#include <iostream>
#include <set>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <algorithm>

using namespace std;

ifstream f ("in.in");

struct hash_pair
{
    template <class T1, class T2>
    size_t operator()(const pair<T1, T2>& p) const
    {
        auto hash1 = hash<T1> {}(p.first);
        auto hash2 = hash<T2> {}(p.second);
        return hash1 ^ hash2;
    }
};

typedef pair<string,char> transition;
typedef unordered_map<transition,set<string>,hash_pair> nfa_type;
typedef pair<string,string> nfa_state;

bool found = false;

void check(vector<nfa_state> nfa_states,nfa_type nfa,vector<string> finale, string w)
{
    if (found) return;
    if (w.length() == 0)
    {
        string curr_node = nfa_states.back().first;
        if (find(finale.begin(),finale.end(),curr_node) != finale.end())
        {
            found = true;
            cout << "DA\n";
            for (auto tran: nfa_states)
            {
                cout << tran.first << ' ';
            }
            cout << '\n';
            return;
        }
    }
    char s = w[0];
    transition to_check(nfa_states.back().first,s);
    if (nfa.find(to_check) != nfa.end())
    {
        for (string nod: nfa[to_check])
        {
            nfa_state new_state(nod,w.substr(1,w.length()-1));
            nfa_states.push_back(new_state);

            check(nfa_states,nfa,finale,w.substr(1,w.length()-1));

            nfa_states.pop_back();
        }
    }
}

int main()
{
    nfa_type nfa;

    int n;
    f >> n;
    int nr_m;
    f >> nr_m;
    for (int i = 0; i < nr_m; i++)
    {
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
    for (int i = 0; i < nr_finale; i++)
    {
        string _final;
        f >> _final;
        finale.push_back(_final);
    }

    int nr_tests, passed = 0;
    f >> nr_tests;
    for (int i = 0; i < nr_tests; i++)
    {
        string w;
        f >> w;

        vector<nfa_state> nfa_states;
        nfa_states.push_back(nfa_state(nod_start,w));
        found = false;
        cout << "\n\n" << w << '\n';
        check(nfa_states,nfa,finale,w);
        if (!found) cout << "NU\n";
        else passed ++;
    }

    cout << passed << " au fost acceptate\n";

    return 0;
}
