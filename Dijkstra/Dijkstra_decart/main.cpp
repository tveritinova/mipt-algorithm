#include <iostream>
#include <vector>
#include <map>
#include <limits.h>
#include <math.h>

template <typename t_key, typename t_prior>
class treap
{
    t_key key;
    t_prior prior;
    
    treap<t_key, t_prior>* left;
    treap<t_key, t_prior>* right;
    
public:
    
    treap() {}
    
    treap(t_key key, t_prior prior, treap<t_key, t_prior>* left, treap<t_key, t_prior>* right):
        key(key), prior(prior), left(left), right(right) {}
    
    treap<t_key, t_prior>* merge(treap<t_key, t_prior>* t)
    {
        if (t == NULL)
        {
            return this;
        }
        
        treap<t_key, t_prior>* res;
        
        if (prior < t->prior)
        {
            res = this;
            res->right = right->merge(t);
        }
        else
        {
            res = t;
            res->left = merge(t->left);
        }
        return res;
    }
    
    std::pair<treap<t_key, t_prior>*, treap<t_key, t_prior>*> split(t_key k)
    {
        std::pair<treap<t_key, t_prior>*, treap<t_key, t_prior>*> result;
        if (key <= k)
        {
            result.first = new treap<t_key, t_prior>(key, prior, left, right->split(k).first);
            result.second = right->split(k).second;
        }
        else
        {
            result.first = left->split(k).first;
            result.second = new treap<t_key, t_prior>(key, prior, left->split(k).second, right);
        }
        
        return result;
    }
    
    treap<t_key, t_prior>* add(t_key k, t_prior p)
    {
        std::pair<treap<t_key, t_prior>*, treap<t_key, t_prior>*> l_r = split(k);
        treap<t_key, t_prior>* new_treap = new treap(k, p, NULL, NULL);
        return l_r.first->merge(new_treap)->merge(l_r.second);
    }
    
    treap<t_key, t_prior>* erase(t_key k)
    {
        if (k > key)
        {
            right = right->erase(k);
        }
        if (k < key)
        {
            left = left->erase(k);
        }
        if (k == key)
        {
            return left->merge(right);
        }
        return this;
    }
    
    t_key get_key() const
    {
        return key;
    }
    
    t_prior get_prior() const
    {
        return prior;
    }
    
    std::pair<std::pair<t_key, t_prior>, bool> search_key(t_key k)
    {
        treap<t_key, t_prior>* cur_treap = this;
        while(cur_treap != NULL)
        {
            if (cur_treap->key < k)
            {
                cur_treap = cur_treap->left;
            }
            if (cur_treap->key > k)
            {
                cur_treap = cur_treap->right;
            }
            else
            {
                break;
            }
        }
        
        if (cur_treap != NULL)
        {
            return std::make_pair(std::make_pair(cur_treap->key, cur_treap->prior), true);
        }
        else
        {
            return std::make_pair(std::make_pair(0, 0), false);
        }
    }
};


class dijkstra
{
    treap<int, double>* dist; //номер вершины + вес
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
                dist->add(i, 10000000000000.0);
            }
        }
        dist->add(st, 0);
        
        way.resize(g.size());
        for (int i = 0; i < way.size(); i++)
        {
            way[i] = std::make_pair(st, 10000000000000.0);
        }
        
        while (true)
        {
            int num_top = dist->get_key();
            double dist_top = dist->get_prior();
            
            for (std::map<int, double>::iterator it = g[num_top].begin();
                 it != g[num_top].end(); it++)
            {
                std::pair<std::pair<int, double>, bool> num_dist_is_cur = dist->search_key(it->first);
                
                bool in_set = num_dist_is_cur.second;
                int num_cur = num_dist_is_cur.first.first;
                int dist_cur = num_dist_is_cur.first.second;
                double cur_weight = it->second;
                
                if ((in_set) && (dist_cur > dist_top + cur_weight))
                {
                    if (dist_top != INT_MAX)
                    {
                        dist->erase(num_cur);
                        dist->add(num_cur, dist_top + cur_weight);
                        way[num_cur] = std::make_pair(num_top, cur_weight);
                    }
                }
            }
            dist->erase(dist->get_key());
            
            if (dist == NULL)
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
    }
    
    dijkstra dij_start(d, 0);
    
    double min_way = 10000000000000.0;
    for (int i = 2; i < town_cnt; i++)
    {
        dijkstra dij_interm(d, i);
        if ((towns[i].first < 0) && (dij_start.get_min_way(i).first + dij_interm.get_min_way(1).first < min_way))
        {
            min_way = dij_start.get_min_way(i).first + dij_interm.get_min_way(1).first;
        }
    }
    
    std::cout << min_way;
}