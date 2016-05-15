#include <iostream>
#include <vector>
#include <fstream>
#include <set>
#include <map>
#include <stdio.h>
#include <stdlib.h>

struct vertex
{
    std::map<char, vertex*> child;
    vertex* link;
    bool is_end = false;
    short int counter;
    std::vector<int> st_counter;
    
    void print(int space_count)
    {
        std::cout << "(" << counter << ")";
        if (counter != 0)
            std::cout << " link to: " << link->counter << " " << link->child.size() << std::endl;
        else
            std::cout << std::endl;
        if (is_end)
        {
            for (int i = 0; i < space_count; i++)
            {
                std::cout << "   ";
            }
            //std::cout << "END  st_counter = " << st_counter << std::endl;
        }
        for (auto v: child)
        {
            for (int i = 0; i < space_count; i++)
            {
                std::cout << "   ";
            }
            std::cout << v.first << std::endl;
            if (v.second)
                v.second->print(space_count + 1);
        }
    }
};

void make_link(vertex* p, char s, const vertex* root)
{
    vertex* l = p->link;
    while((l->counter != 0) && (l->child.find(s) == l->child.end()))
    {
        l = l->link;
    }
    
    if (l->child.find(s) == l->child.end())
    {
        p->child[s]->link = l;
    }
    else
    {
        p->child[s]->link = l->child[s];
    }
    
    if (p->child[s]->link->is_end)
    {
        p->child[s]->is_end = true;
        for (int i = 0; i < p->child[s]->link->st_counter.size(); i++)
        {
            p->child[s]->st_counter.push_back(p->child[s]->link->st_counter[i] + p->child[s]->counter - p->child[s]->link->counter);
        }
    }
}

vertex* init_bohr(std::vector<std::string> words)
{
    vertex* root = new vertex;
    vertex* null_vertex = new vertex;
    null_vertex->is_end = true;
    null_vertex->st_counter.push_back(0);
    null_vertex->counter = (int) words[0].size();
    root->child[words[0][words[0].size() - 1]] = null_vertex;
    root->counter = (int) words[0].size() - 1;
    
    for (int i = (int) words[0].size() - 2; i >= 0; i--)
    {
        vertex* new_vertex = new vertex;
        new_vertex->child[words[0][i]] = root;
        new_vertex->counter = i;
        root = new_vertex;
    }
    
    
    for (int i = 1; i < words.size(); i++)
    {
        vertex* v = root;
        int j = 0;
        std::string cur = words[i];
        bool magic_key = false;
        if (v->child.find(cur[0]) != v->child.end())
        {
            magic_key = true;
            while ((j < cur.size())
                   && ((v->child[cur[j]]->child).find(cur[j + 1]) != v->child[cur[j]]->child.end()))
            {
                v = v->child[cur[j]];
                ++j;
            }
        }
        vertex* new_branch = new vertex;
        new_branch->is_end = true;
        new_branch->st_counter.push_back(0);
        new_branch->counter = (int) cur.size();
        for (int k = (int) cur.size() - 1; k > (magic_key ? j + 1 : j); k--)
        {
            vertex* new_vertex = new vertex;
            new_vertex->child[cur[k]] = new_branch;
            new_vertex->counter = k;
            new_branch = new_vertex;
        }
        if (magic_key)
        {
            if (j + 1 < cur.size())
            {
                v->child[cur[j]]->child[cur[j + 1]] = new_branch;
            }
            else
            {
                v->child[cur[j]]->is_end = true;
                v->child[cur[j]]->st_counter.push_back(0);
            }
        }
        else
        {
            v->child[cur[j]] = new_branch;
        }
    }

    return root;
}

void init_link(vertex* root)
{
    std::vector<vertex*> childs_last;
    childs_last.reserve(root->child.size());
    
    int new_size = 0;
    for (auto v : root->child)
    {
        v.second->link = root;
        childs_last.push_back(v.second);
        new_size += v.second->child.size();
    }
    
    int last_counter = 0;
    std::vector<vertex*> childs_new;
    
    while(childs_last.size() != 0)
    {
        last_counter = 0;
        childs_new.reserve(new_size);
        new_size = 0;
        while (last_counter < childs_last.size())
        {
            for (auto ch : childs_last[last_counter]->child)
            {
                make_link(childs_last[last_counter], ch.first, root);
                if (ch.second->child.size() != 0)
                {
                    childs_new.push_back(ch.second);
                    new_size += ch.second->child.size();
                }
            }
            ++last_counter;
        }
        childs_last = childs_new;
        childs_new.clear();
    }
}

int main()
{
    std::ifstream in;
    in.open("words.txt");
    std::vector<std::string> words;
    
    char s[1000];
    while(!in.eof())
    {
        in.getline(s, 1000);
        words.push_back(*new std::string(s));
    }
    
    vertex* root = init_bohr(words);
    init_link(root);
    
    //root->print(0);
    
    vertex* v = root;
    std::ifstream in_text;
    in_text.open("text.txt");
    
    std::set<int> starts;
    int j;
    
    while (!in_text.eof())
    {
        int st_counter = 0;
        in_text.getline(s, 1000);
        std::string str = s;
        j = 0;
        v = root;
        while (j < str.size())
        {
            if (v->child.find(str[j]) != v->child.end())
            {
                v = v->child[str[j]];
                if (v->is_end)
                {
                    for (int i = 0; i < v->st_counter.size(); i++)
                        starts.insert(st_counter + v->st_counter[i]);
                }
                j++;
            }
            else
            {
                if (v->counter != 0)
                {
                    st_counter += v->counter;
                    v = v->link;
                    st_counter -= v->counter;
                }
                else
                {
                    ++j;
                    ++st_counter;
                }
            }
        }
    }
    
    for (int a: starts)
    {
        std::cout << a << " ";
    }
    
    return 0;
}
