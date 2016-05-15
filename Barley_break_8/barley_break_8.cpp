#include <iostream>
#include <vector>
#include <queue>

const int COUNT = 9;

int fact[COUNT + 1];

int code(std::vector<int> a)
{
    int res = 0;
    
    for (int i = 1; i < COUNT; i++)
    {
        for (int j = i; j < COUNT; j++)
        {
            if (a[j] >= a[i - 1])
            {
                --a[j];
            }
        }
    }
    
    for (int i = 0; i < COUNT; i++)
    {
        res += a[i] * fact[COUNT - i - 1];
    }
    
    return res;
}

std::vector<int> decode(int a)
{
    std::vector<int> result(COUNT);
    for(int i = 0; i < COUNT; i++)
    {
        result[i] = a / fact[COUNT - i - 1];
        a = a % fact[COUNT - i - 1];
    }
    
    for (int i = COUNT - 2; i >= 0; i--)
    {
        for (int j = i + 1; j < COUNT; j++)
        {
            if (result[j] >= result[i])
            {
                ++result[j];
            }
        }
    }
    return result;
}

std::vector<int> change(const std::vector<int>& g, int a, int b)
{
    std::vector<int> result(g.size());
    for (int i = 0; i < g.size(); i++)
    {
        if ((i != a) && (i != b))
        {
            result[i] = g[i];
        }
        else
        {
            result[a] = g[b];
            result[b] = g[a];
        }
    }
    
    return result;
}

void print(std::vector<int> result)
{
    for (int i = 0; i < result.size(); i++)
    {
        std::cout << result[i] << " ";
        if (!((i + 1) % 3))
        {
            std::cout << std::endl;
        }
    }
    
    std::cout << std::endl;
}

int main(int argc, const char * argv[]) {
    
    fact[0] = 1;
    fact[1] = 1;
    for (int i = 2; i <= COUNT; i++)
    {
        fact[i] = fact[i - 1] * i;
    }
    
    int final = 46233;
    
    std::vector<int> in(COUNT);
    
    for (int i = 0; i < COUNT; i++)
    {
        std::cin >> in[i];
    }
    
    std::queue<int> q;
    q.push(code(in));
    
    std::vector<bool> mark(fact[COUNT], false);
    std::vector<int> d(fact[COUNT], 0);
    mark[code(in)] = true;
    
    std::vector<int> vertex(COUNT);
    
    while(!q.empty())
    {
        vertex = decode(q.front());
        
        //std::cout << code(vertex) << std::endl;
        
        if (code(vertex) == final)
            break;
        
        q.pop();
        
        for (int i = 0; i < COUNT; i++)
        {
            if (vertex[i] == 0)
            {
                //четыре соседа
                
                if ((i + 1) % 3) //вправо
                {
                    if (mark[code(change(vertex, i, i + 1))] == false)
                    {
                        d[code(change(vertex, i, i + 1))] = d[code(vertex)] + 1;
                        mark[code(change(vertex, i, i + 1))] = true;
                        q.push(code(change(vertex, i, i + 1)));
                    }
                }
                
                if (i % 3) //влево
                {
                    if (mark[code(change(vertex, i, i - 1))] == false)
                    {
                        d[code(change(vertex, i, i - 1))] = d[code(vertex)] + 1;
                        mark[code(change(vertex, i, i - 1))] = true;
                        q.push(code(change(vertex, i, i -1)));
                    }
                }
                
                if (i > 2) // вверх
                {
                    if (mark[code(change(vertex, i, i - 3))] == false)
                    {
                        d[code(change(vertex, i, i - 3))] = d[code(vertex)] + 1;
                        mark[code(change(vertex, i, i - 3))] = true;
                        q.push(code(change(vertex, i, i - 3)));
                    }
                }
                
                if (i < COUNT - 3) // вниз
                {
                    if (mark[code(change(vertex, i, i + 3))] == false)
                    {
                        d[code(change(vertex, i, i + 3))] = d[code(vertex)] + 1;
                        mark[code(change(vertex, i, i + 3))] = true;
                        q.push(code(change(vertex, i, i + 3)));
                    }
                }
                
                break;
            }
        }
    }
    
    if ((code(in) == final) || (d[final] != 0))
    {
        std::cout << "Yes" << std::endl << d[final];
    }
    else
    {
        if (d[final] == 0)
        {
            std::cout << "No";
        }
    }
    
    return 0;
}
