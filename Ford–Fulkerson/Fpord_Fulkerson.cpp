//
//  main.cpp
//  Ford–Fulkerson
//
//  Created by Евгения Тверитинова on 16.10.15.
//  Copyright (c) 2015 Евгения Тверитинова. All rights reserved.
//

#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <stack>
#include <queue>

void print(const std::vector<std::map<int, int> > & G)
{
    for (int i = 0; i < G.size(); i++)
    {
        for (std::map<int, int>::const_iterator it = G[i].begin(); it != G[i].end(); it++)
        {
            std::cout << i << " " << it->first << " " << it->second << std::endl;
        }
    }
    
    std::cout << "\n";
}

int main(int argc, const char * argv[]) {
    
    int cnt;
    std::cin >> cnt;
    std::vector<std::vector<int> > table(cnt);
    
    char in;
    
    int vertex_count = 2;
    
    for (int i = 0; i < cnt; i++)
    {
        table[i].resize(cnt);
        for (int j = 0; j < cnt; j++)
        {
            std::cin >> in;
            
            if (in == '.')
            {
                table[i][j] = vertex_count;
                ++vertex_count;
            }
            if (in == '#')
            {
                table[i][j] = -1;
            }
        }
    }
    
    std::vector<std::map<int, int> > G_(vertex_count);
    
    for (int i = 0; i < cnt; i++)
    {
        
        for (int j = i % 2; j < cnt; j+= 2)
        {
            if (table[i][j] != -1)
            {
                G_[0][table[i][j]] = 1;
                
                if ((i > 0) && (table[i - 1][j] != -1)) //вверх
                {
                    G_[table[i][j]][table[i - 1][j]] = 1;
                }
            
                if ((i < cnt - 1) && (table[i + 1][j] != -1)) //вниз
                {
                    G_[table[i][j]][table[i + 1][j]] = 1;
                }
            
                if ((j > 0) && (table[i][j - 1] != -1)) //влево
                {
                    G_[table[i][j]][table[i][j - 1]] = 1;
                }
            
                if ((j < cnt - 1) && (table[i][j + 1] != -1)) //вправо
                {
                    G_[table[i][j]][table[i][j + 1]] = 1;
                }
            }
        }
        
        for (int j = (i + 1) % 2; j < cnt; j += 2)
        {
            if (table[i][j] != -1)
            {
                G_[table[i][j]][1] = 1;
            }
        }
    }
    
    int st = 0;
    int t = 1;
    
    int to, from;
    
    std::stack<int> q;
    
    std::vector<std::map<int, int> > G = G_;
    
    while (true)
    {
        std::vector<int> mark(vertex_count, -1);
        mark[st] = st;
        
        while(!q.empty()) q.pop();
        
        q.push(st);
        
        int v;
        
        int key = false;
        
        while (!q.empty())
        {
            v = q.top();
            q.pop();
            
            if (v == t)
            {
                key = true;
                break;
            }
            
            for (std::map<int, int>::iterator it = G[v].begin(); it != G[v].end(); ++it)
            {
                if ((mark[it->first] == -1) && (it->second != 0))
                {
                    mark[it->first] = v;
                    q.push(it->first);
                }
            }
        }
        
        if (key == false)
            break;
        
        to = v;
        from = mark[v];
        
        int min = G[from][to];
        
        while(to != from)
        {
            if (G[from][to] < min)
            {
                min = G[from][to];
            }
            to = from;
            from = mark[to];
        }
        
        to = v;
        from = mark[v];
        
        while(to != from)
        {
            G[from][to] -= min;
            
            if (G[from][to] != 0) G[to][from] += min;
            
            to = from;
            from = mark[to];
        }
        
    }
    
    for (int i = 0; i < G_.size() ; i++)
    {
        for (std::map<int, int>::iterator it = G_[i].begin(); it != G_[i].end(); it++)
        {
            it->second -= G[i].find(it->first)->second;
        }
    }
    
    int answer = 0;
    
    for (std::map<int, int>::iterator it = G_[st].begin(); it != G_[st].end(); it++)
    {
        answer += it->second;
    }
    
    //print(G_);
    
    std::cout << answer;
    return 0;
}