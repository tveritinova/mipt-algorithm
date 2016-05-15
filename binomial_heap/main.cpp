#include <iostream>
#include <list>
#include <vector>

template<typename T>
class binomial_tree
{
    int size;
    int key;
    std::list<binomial_tree<T>*> child;
    
public:
    
    int get_size() const
    {
        return size;
    }
    
    int get_key() const
    {
        return key;
    }
    
    std::list<binomial_tree<T>*> get_child() const
    {
        return child;
    }
    
    binomial_tree* merge(binomial_tree& tree)
    {
        binomial_tree<T>* result = new binomial_tree<T>();
        
        if (key < tree.get_key())
        {
            result = this;
            result->child.insert(result->child.begin(), &tree);
        }
        else
        {
            result = &tree;
            result->child.insert(result->child.begin(), this);
        }
        result->size = size + tree.size;
        return result;
    }

    binomial_tree(T a)
    {
        key = a;
        size = 1;
    }
    
    binomial_tree() {}
    
    void print(int cnt) const
    {
        for (int i = 0; i < cnt; i++)
        {
            std::cout << " ";
        }
        std::cout << key << std::endl;
        for (auto a: child)
        {
            for (int i = 0; i < cnt + 1; i++)
            {
                std::cout << " ";
            }
            a->print(cnt + 1);
        }
    }
};


template<typename T>
class binomial_heap
{
public:
    std::list<binomial_tree<T>*> root_list;
    
    binomial_heap(std::vector<T> numbers)
    {
        for (T a: numbers)
        {
            add(a);
        }
    }
    
    binomial_heap(T a)
    {
        binomial_tree<T>* tree = new binomial_tree<T>(a);
        root_list.insert(root_list.begin(), tree);
    }
    
    binomial_heap(const std::list<binomial_tree<T>*> list):root_list(list) {}
    
    binomial_heap() {}
    
    void merge(const binomial_heap& heap)
    {
        auto it1 = root_list.begin();
        auto it2 = heap.root_list.begin();
        
        while (it2 != heap.root_list.end()) //merge
        {
            if ((*it1)->get_size() >= (*it2)->get_size())
            {
                root_list.insert(it1, *it2);
                ++it2;
            }
            ++it1;
        }
        
        auto it = root_list.begin();
        
        while ((it != root_list.end()) && (++it != root_list.end()))
        {
            --it;
            if ((*it)->get_size() == (*(++it))->get_size())
            {
                typename std::list<binomial_tree<T>*>::iterator add;
                if (++it != root_list.end())
                {
                    --it;
                    if ((*it)-> get_size() == (*(++it))->get_size())
                    {
                        add = it;
                    }
                    else
                    {
                        add = --it;
                    }
                }
                else
                {
                    add = --it;
                }
                --it;
                *it = (*it)->merge(**add);
                auto copy = it;
                ++it;
                root_list.erase(it);
                it = copy;
            }
            else
            {
                ++it;
            }
        }
    }
    
    void add(T a)
    {
        if (root_list.size() != 0)
        {
            binomial_heap<T> new_heap(a);
            this->merge(new_heap);
        }
        else
        {
            binomial_tree<T>* tree = new binomial_tree<T>(a);
            root_list.insert(root_list.begin(), tree);
        }
    }
    
    void print() const
    {
        for (auto tree: root_list)
        {
            std::cout << "(" << tree->get_size() << ")" << std::endl;
            tree->print(0);
        }
        std::cout << std::endl;
    }
    
    T min() const
    {
        T res = (*root_list.begin())->get_key();
        for (auto a: root_list)
        {
            if (a->get_key() < res)
            {
                res = a->get_key();
            }
        }
        return res;
    }
    
    void extract_min()
    {
        T min = (*root_list.begin())->get_key();
        auto it_min = root_list.begin();
        for (auto it = root_list.begin(); it != root_list.end(); it++)
        {
            if ((*it)->get_key() < min)
            {
                min = (*it)->get_key();
                it_min = it;
            }
        }
        
        std::list<binomial_tree<T>*> copy = (*it_min)->get_child();
        root_list.erase(it_min);
        copy.reverse();
        binomial_heap<T> new_heap(copy);
        if (root_list.size() == 0)
        {
            *this = new_heap;
        }
        else
        {
            this->merge(new_heap);
        }
    }
};

int main(int argc, const char * argv[])
{
    std::vector<int> numbers({1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16});
    
    binomial_heap<int> binheap(numbers);
    
    binheap.print();
    
    std::cout << "MIN = " << binheap.min() << std::endl << std::endl;
    
    binheap.extract_min();
    
    binheap.print();
    
    return 0;
}
