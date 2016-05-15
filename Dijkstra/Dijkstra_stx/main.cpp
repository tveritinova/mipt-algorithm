#include <iostream>
#include <vector>
#include <map>
#include "/Users/evgeniatveritinova1/stx/btree_set" 
#include <limits.h>
#include <math.h>

class dijkstra
{
    stx::btree_set<std::pair<double, int> > dist; // вес + номер вершины
    std::vector<std::pair<int, double> > way; // откуда + вес ребра
    int start;
    
public:
    
    dijkstra(std::vector<std::map<int, double> >& g, int st)
    {
        start = st;
        for (int i = 0; i < g.size(); i++)
        {
            if (i != st)
            {
                dist.insert(std::make_pair(10000000000000.0, i));
            }
        }
        dist.insert(std::make_pair(0, st));
        
        way.resize(g.size());
        for (int i = 0; i < way.size(); i++)
        {
            way[i] = std::make_pair(st, 10000000000000.0);
        }
        
        while (true)
        {
            int num_top = dist.begin()->second;
            double dist_top = dist.begin()->first;
            
            for (std::map<int, double>::iterator it = g[num_top].begin();
                 it != g[num_top].end(); it++)
            {
                double cur_weight = it->second;
                
                stx::btree_set<std::pair<double, int> >::iterator cur_it;
                for (cur_it = dist.begin(); cur_it != dist.end(); cur_it++)
                {
                    if (cur_it->second == it->first)
                    {
                        break;
                    }
                }
                
                double dist_cur = cur_it->first;
                int num_cur = cur_it->second;
                
                if ((cur_it != dist.end()) && (dist_cur > dist_top + cur_weight))
                {
                    if (dist_top != INT_MAX)
                    {
                        dist.erase(cur_it);
                        dist.insert(std::make_pair(dist_top + cur_weight, num_cur));
                        way[num_cur] = std::make_pair(num_top, cur_weight);
                    }
                }
            }
            dist.erase(dist.begin());
            
            if (dist.empty())
            {
                break;
            }
        }
    }
    
    std::pair<double, std::vector<int> > get_min_way(int v) const
    {
        std::vector<int> res_way;
        double res_dist = 0;
        int a = v;
        res_way.push_back(a);
        
        while (a != start)
        {
            res_way.push_back(way[a].first);
            res_dist += way[a].second;
            a = way[a].first;
        }
        
        return std::make_pair(res_dist, res_way);
    }
};


int main()
{
    int town_cnt;
    std::cin >> town_cnt;
    double r;
    std::cin >> r;
    
    double x, y;
    
    std::vector<std::map<int, double> > d(town_cnt);
    std::vector<std::pair<double, double> > towns;
    towns.reserve(town_cnt);
    
    for (int i = 0; i < town_cnt; i++)
    {
        std::cin >> x  >> y;
        towns.push_back(std::make_pair(x, y));
    }
    
    for (int i = 0; i < town_cnt; i++)
    {
        for (int j = 0; j < town_cnt; j++)
        {
            if (pow(towns[i].first - towns[j].first, 2) + pow(towns[i].second - towns[j].second, 2) <= pow(r, 2))
            {
                d[i][j] = sqrt(pow(towns[i].first - towns[j].first, 2) + pow(towns[i].second - towns[j].second, 2));
                d[j][i] = sqrt(pow(towns[i].first - towns[j].first, 2) + pow(towns[i].second - towns[j].second, 2));
            }
        }
        //d[i][i] = 0;
    }
    
    //ford_bellman fb_start(d, 0);
    dijkstra dij_start(d, 0);
    
    double min_way = 10000000000000.0;
    for (int i = 2; i < town_cnt; i++)
    {
        //ford_bellman fb_interm(d, i);
        dijkstra dij_interm(d, i);
        if ((towns[i].first < 0) && (dij_start.get_min_way(i).first + dij_interm.get_min_way(1).first < min_way))
        {
            min_way = dij_start.get_min_way(i).first + dij_interm.get_min_way(1).first;
        }
    }
    
    std::cout << min_way;
}