#include <iostream>
#include <fstream>
#include <bits/stdc++.h>

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

typedef unordered_map<pair<string,string>, string, hash_pair> dfa_container;


int main()
{
    dfa_container dfa;

    int n;
    f >> n;
    int nr_m;
    f >> nr_m;
    for (int i = 0; i < nr_m; i++)
    {
        string n_start, n_end;
        string s;
        f >> n_start >> n_end >> s;

        if (dfa.find({n_start, n_end}) == dfa.end())
        {
            dfa[{n_start, n_end}] = s;
        }
        else
        {
            dfa[{n_start, n_end}] += "+" + s;
        }
    }

    // parantezam tranzitiile
    for (auto& per: dfa)
    {
        per.second = "("+per.second+")";
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

    /// verifica daca in nodul de start intra noduri
    /// daca da, fa nod auxiliar care merge cu lambda in fostul nod de start

    for (auto& per:dfa)
    {
        string to = per.first.second;

        if (to == nod_start)
        {
            dfa[{"-1",nod_start}] = "e";
            nod_start = "-1";
            n ++;
            break;
        }
    }

    /// daca sunt mai multe noduri finale
    if (finale.size() > 1)
    {
        for (string fin: finale)
        {
            dfa[{fin,"-2"}] = "e";
        }
        finale.clear();
        finale.push_back("-2");
        n++;
    }

    /// daca este doar un nod final dar acesta merge si in alte stari
    else if (finale.size() == 1)
    {
        for (auto& per: dfa)
        {
            string from = per.first.first;

            if (from == finale[0])
            {
                dfa[{from,"-2"}] = "e";
                finale.clear();
                finale.push_back("-2");
                n++;
                break;
            }
        }
    }

    /*
    for (auto per: dfa)
    {
        cout << per.first.first << ' ' << per.first.second << ' ' << per.second << '\n';
    }
    */

    while (n > 2)
    {
        /// pick node to remove
        string node_to_remove;

        for (auto& per:dfa)
        {
            string from = per.first.first;
            if (from != nod_start && from != finale[0])
            {
                node_to_remove = from;
                break;
            }
        }

        // verifica daca nodul de eliminat are muchie care duce tot in el
        string loop = "";
        for (auto& per: dfa)
        {
            if (per.first.first == per.first.second && per.first.first == node_to_remove)
            {
                loop = per.second;
                break;
            }
        }

        // pt fiecare nod care intra in nodul de eliminat
        for (auto& per: dfa)
        {
            string goes_to = per.first.second;
            string _from = per.first.first;
            if (goes_to == node_to_remove && _from != node_to_remove)
            {
                // pt fiecare nod in care ajunge prin intermediul nodului de eliminat
                for (auto& per2: dfa)
                {
                    string from = per2.first.first;
                    string _to = per2.first.second;
                    if (from == node_to_remove && _to != node_to_remove)
                    {
                        // aici avem 2 tranzitii care trebuie conectate de nodul de eliminat

                        string from_origin = per.first.first;
                        string to_origin = per2.first.second;

                        // verificam daca exista deja in dfa muchia
                        // si daca exista o concatenam la raspuns
                        string trans_here_already = "";
                        if (dfa.find({from_origin, to_origin}) != dfa.end())
                            trans_here_already = dfa[{from_origin, to_origin}];

                        string trans_to_add = "";
                        trans_to_add += per.second;

                        if (loop != "")
                        {
                            trans_to_add += "x" + loop + "*";
                        }

                        trans_to_add += "x" + per2.second;

                        trans_to_add = "(" + trans_to_add + ")";

                        if (trans_here_already != "")
                        {
                            trans_to_add = "(" + trans_to_add + "+" + trans_here_already + ")";
                        }

                        /// add or update the new transition
                        dfa[{from_origin, to_origin}] = trans_to_add;

                    }
                }
            }
        }
        // remove all transitions that contain node_to_remove
        for (auto it = dfa.cbegin(); it != dfa.cend();)
        {
            if ((*it).first.first == node_to_remove || (*it).first.second == node_to_remove)
            {
                it = dfa.erase(it);
            }
            else
            {
                ++it;
            }
        }
        n --;
    }

    cout << dfa.size() << '\n';
    cout << dfa[{nod_start, finale[0]}] << '\n';

    int nr_tests, passed = 0;
    f >> nr_tests;
    for (int i = 0; i < nr_tests; i++)
    {
        string w;
        f >> w;
    }

    return 0;
}
