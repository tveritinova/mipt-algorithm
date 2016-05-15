#include <vector>
#include <iostream>

class rmq
{
    std::vector<std::vector<float> > sparse_table;
    
    std::vector<int> log;
    
public:
    
    rmq(const std::vector<float> & in)
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
    
    float get_min(int i, int j) const
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

int main()
{
    std::vector<float> in;
    
    int size;
    
    scanf("%d", &size);
    
    in.resize(size);
    
    for (int i = 0; i < size; i++)
    {
        float read;
        scanf("%f", &read);
        in[i] = read;
    }
    
    rmq my_rmq(in);
    
    int m;
    scanf("%d", &m);
    
    for (int i = 0; i < m ; i ++)
    {
        int from, to;
        scanf("%d %d", &from, &to);
        printf("%f\n", my_rmq.get_min(from, to));
    }
    
    return 0;
}