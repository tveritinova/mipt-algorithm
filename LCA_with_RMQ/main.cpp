#include <iostream>
#include <vector>
#include <map>

template<typename T>
struct node
{
    T key;
    node* left;
    node* right;
    //bool null_key = false;
    
    /*node(char** s, int* size)
    {
        if ((*s)[0] != '(')
        {
            null_key = true;
            return;
        }
        (*s)++;
        left = new node(s, size);
        if (left->null_key == true)
        {
            delete left;
            left = NULL;
        }
        (*size)++;
        key = ((T*) (*s))[0];
        (*s) += sizeof(T);
        right = new node(s, size);
        if (right->null_key == true)
        {
            delete right;
            right = NULL;
        }

        if ((*s)[0] == ')')
            (*s)++;
    }*/
    
    node(std::multimap<T, T>& edges, T k)
    {
        key = k;
        std::multimap<int, int>::iterator it = edges.find(key);
        if ((it != edges.end()) && ((*it).first == key))
        {
            left = new node<T>(edges, it->second);
            ++it;
            
            if ((it != edges.end()) && (it->first == key))
            {
                right = new node<T>(edges, it->second);
                ++it;
            }
            else
            {
                right = NULL;
            }
        }
        else
        {
            left = NULL;
            right = NULL;
        }
    }
    
    node() {}
};

template <typename T>
struct val_depth
{
    T val;
    int depth;
    
    val_depth(const T& val, const int& depth): val(val), depth(depth) {}
    val_depth() {}
    
    bool operator< (const val_depth& a) const
    {
        return depth < a.depth;
    }
};

template <typename T>
class RMQ
{
    std::vector<std::vector<T> > sparse_table;
    std::vector<int> log;
    
public:
    
    RMQ() {}
    
    RMQ(const std::vector<T> & in)
    {
        log.resize(in.size() + 1);
        
        int pos = 0;
        int lim = 2;
        int log_ = 0;
        
        for(;pos < in.size() + 1;)
        {
            
            
            while(pos < lim)
                log[pos++] = log_;
            lim <<= 1;
            log_++;
            
            if (lim > in.size() + 1)
                lim = in.size() + 1;
        }
        
        int m_size = log[in.size()] + 1;
        
        sparse_table.resize(m_size);
        
        sparse_table[0] = in;
        
        for (int k = 1; k < m_size; k++)
        {
            int size_ = in.size() - (1 << k) + 1;
            sparse_table[k].resize(size_);
            for (int i = 0; i < size_; i++)
            {
                sparse_table[k][i] = std::min(sparse_table[k - 1][i],
                                              sparse_table[k - 1][i + (1 << (k - 1))]);
            }
        }
    }
    
    T get_min(int i, int j) const
    {
        int l = j - i;
        int k = log[l];
        
        return std::min(sparse_table[k][i],
                        sparse_table[k][j - (1 << k)]);
    }
    
    void print_sparse_table() const
    {
        for (int i = 0; i < sparse_table.size(); i++)
        {
            for (int j = 0; j < sparse_table[i].size(); j++)
            {
                std::cout << sparse_table[i][j] << " ";
            }
            std::cout << std::endl;
        }
        
        std::cout << std::endl << std::endl;
    }
    
};

template <typename T>
class LCA
{
    std::vector<val_depth<T> > table;
    std::map<T, int> where_table;
    RMQ<val_depth<T> > rmq;
    node<T>* n;
    
    void dfs(node<T>* n, int d)
    {
        table.push_back(*new val_depth<T>(n->key, d));
        where_table[n->key] = table.end() - table.begin() - 1;
        if (n->left != NULL)
        {
            dfs(n->left, d + 1);
            table.push_back(*new val_depth<T>(n->key, d));
        }
        if (n->right != NULL)
        {
            dfs(n->right, d + 1);
            table.push_back(*new val_depth<T>(n->key, d));
        }
    }
    
public:
    
    LCA(node<T>* n, int size): n(n)
    {
        table.reserve(size);
        dfs(n, 0);
        rmq = *new RMQ<val_depth<T> >(table);
    }
    
    T get_ancestor(T a, T b)
    {
        return rmq.get_min(std::min(where_table[a], where_table[b]),
                           std::max(where_table[a], where_table[b])).val;
    }
};

int main()
{
    /*std::string str;
    //std::cin >> str;
    
    //char* in = (char*) str.c_str();
    //char** in_pointer = &in;
    //int size = 0;
    //int* size_pointer = &size;
    
    //node<char>* n = new node<char>(in_pointer, size_pointer);*/
    
    int size;
    scanf("%d", &size);
    
    std::multimap<int, int> edges;
    
    for (int i = 0; i < size; i++)
    {
        int from, to;
        scanf("%d %d", &to, &from);
        edges.insert(std::make_pair(from, to));
    }
    
    node<int>* n = new node<int>(edges, 0);
    LCA<int> lca = *new LCA<int>(n, size + 1);
    
    //std::cout << lca.get_ancestor('h', 'a');
    
    int pairs_count;
    std::cin >> pairs_count;
    //std::vector<std::pair<int, int> > pairs(pairs_count);
    
    for(int i = 0; i < pairs_count; i++)
    {
        int a, b;
        std::cin >> a >> b;
        //pairs[i] = std::make_pair(a, b);
        std::cout << lca.get_ancestor(a, b) << std::endl;
    }
    
    /*for (int i = 0; i < pairs.size(); i++)
    {
        std::cout << lca.get_ancestor(pairs[i].first, pairs[i].second) << std::endl;
    }*/
    
}