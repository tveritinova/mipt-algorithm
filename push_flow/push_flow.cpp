#include <iostream>
#include <vector>
#include <map>

int main()
{
    int start, target;
    
    int n, m;
    std::cin >> n >> m;
    
    std::vector<std::map<int, double> > graph(n);
    
    for (int i = 0; i < m; i++)
    {
        int from, to, count;
        std::cin >> from >> to >> count;
        graph[from - 1][to - 1] = count;
    }
    
    start = 0;
    target = n - 1;

    std::vector<int> height(n, 0);
    height[start] = n;
    
    std::map<int, double> excess = graph[start];
    
    std::map<int, double> copy_start = graph[start];
    for (auto it = copy_start.begin(); it != copy_start.end(); ++it)
    {
        graph[start].erase(it->first);
        graph[it->first][start] += it->second;
    }
    
    //int h_min = height[graph[start == 0 ? start + 1 : start].begin()->first];
    
    while (!excess.empty())
    {
        //std::cout << "here" << std::endl;
        int v = excess.begin()->first;
        int h_min = height[graph[v].begin()->first];
        for (auto it = graph[v].begin(); it != graph[v].end(); ++it)
        {
            if (it->second != 0)
            {
                h_min = std::min(h_min, height[it->first]);
            
                if (height[v] > height[it->first])
                {
                    //проталкивание
                    double cnt = std::min(it->second, excess[v]);
                    excess[v] -= cnt;
                    if (!excess[v])
                        excess.erase(v);
                    
                    it->second -= cnt; //прямое ребро
                    graph[it->first][v] += cnt; //обратное  ребро
                
                    if (it->first != start && it->first != target)
                        excess[it->first] += cnt;
                
                    break;
                }
            }
            
        }
        if (h_min >= height[v])
        {
            height[v] = h_min + 1;
        }
    
    }
    
    double res_max_flow = 0;
    for (auto it = graph[start].begin(); it != graph[start].end(); ++it)
    {
        res_max_flow += it->second;
    }
    
    std::cout << res_max_flow;
    
    for (int i = 0; i < graph.size(); i++)
    {
        for (auto it = graph[i].begin(); it != graph[i].end(); ++it)
        {
            std::cout << i << " -> " << it->first << " : " << it->second << std::endl;
        }
    }
    
}