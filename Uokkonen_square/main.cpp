#include <string>
#include <map>
#include <locale>

struct vertex;

struct edge
{
    size_t beg;
    size_t lim;
    vertex* end;
    
    edge():beg(0),lim(0),end(0){};
};

struct vertex
{
    std::map <char, edge> child;
    size_t depth_dontuse;
    vertex* suffix_link;
    
};


void print_edge (size_t shift, std::string &s, edge* e)
{
    size_t i;
    size_t lim;
    
    for (i = 0; i < shift; i++)
        printf(" ");
    
    if (e->end)
        lim = e->lim;
    else
        lim = s.length();
    
    for (i = e->beg; i < lim; i++)
        printf("%c", s[i]);
    
    if (e->end)
    {
        printf("(%d) %X %X\n", e->end->depth_dontuse, e->end, e->end->suffix_link);
        for (auto ch = e->end->child.begin(); ch != e->end->child.end(); ++ch)
        {
            print_edge (shift + lim - e->beg, s, &(ch->second));
        }
    }
    else
    {
        printf("$\n");
    }
}


void find_substring(std::string& s/*де ищем*/, edge* e/*суф дерево*/, std::string pattern)
{
    edge* cur_edge;
    size_t cur_edge_pos;
    
    cur_edge = e;
    cur_edge_pos = 0;
    size_t i = 0;
    
    while (i < pattern.length())
    {
        if (cur_edge_pos < cur_edge->lim)
        {
            //стою на ребре
            
            if (s[cur_edge_pos] == pattern[i])
            {
                i++;
                cur_edge_pos++;
            }
            else
            {
                return;
            }
        }
        else
        {
            //стоим в конце текущего ребра, т.е. в концевой вершине
            //ищем подхдящее исходящее ребро
            std::map <char,edge>::iterator out = cur_edge->end->child.find(pattern[i]);
            
            if (out == cur_edge->end->child.end())
            {
                return;
            }
            cur_edge = &(out->second);
            cur_edge_pos = cur_edge->beg;
        }
    }
    
    //подстрока найдена. Перебрать все концевые вершниы
    
    print_edge(0, s, cur_edge);
}

int main()
{
    std::string s;
    vertex root;
    edge edge_to_root;
    
    s = "abababaab";
    s = "ojojojojojoje";
    
    //setlocale(LC_ALL, "Russian");
    
    size_t cur_suf;
    size_t cur_suf_pos;
    edge* cur_edge;
    size_t cur_edge_pos;
    size_t depth; //Текущая глубина
    vertex* no_link_vertex; //последняя созданная вершина(у которой еще нет стрелки)
    size_t no_link_depth;
    
    cur_suf = cur_suf_pos = 0;
    cur_edge = &edge_to_root;
    edge_to_root.end = &root;
    root.depth_dontuse = 0;
    root.suffix_link = 0;
    cur_edge_pos = 0;
    depth = 0;
    no_link_vertex = 0;
    no_link_depth = 0;
    
    for (size_t len = 1;  len <= s.length(); len++)
    {
        while (cur_suf_pos < len)
        {
            char ch = s[cur_suf_pos];
            
            if (cur_edge_pos < cur_edge->lim)
            {
                //стоим на ребре
                
                //текущее ребро не кончилось, мы стоим в середине, проверяем можно ли сделать шаг
            
                if (ch == s[cur_edge_pos]) //очередной символ совпал с очередным символом ребра
                {
                    cur_suf_pos++;
                    cur_edge_pos++;
                    depth++;
                    continue;
                }
                
            
                //стоим на ребре но очередные символы не совпадают
                //нужно ветвиться. Разрезаем ребро, создавая новую вершину.
            
                vertex* nv = new vertex;
            
                //инициализация единственного исходяего ребра из новой вершины
            
                nv->child[s[cur_edge_pos]].beg = cur_edge_pos;
                nv->child[s[cur_edge_pos]].lim = cur_edge->lim;
                nv->child[s[cur_edge_pos]].end = cur_edge->end;
                
                nv->depth_dontuse = depth;
                nv->suffix_link = 0;
            
                //укорачиваем старое ребро
            
                cur_edge->lim = cur_edge_pos;
                cur_edge->end = nv;
                
                if (no_link_vertex)
                {
                    if (no_link_depth != depth + 1)
                    {
                        true;
                    }
                    no_link_vertex->suffix_link = nv;
                }
                
                no_link_vertex = nv;
                no_link_depth = depth;
            }
        
        
            //попав сюда, мы стоим в концевой вершине ребра
            
            if (no_link_vertex && (no_link_depth == depth + 1))
            {
                no_link_vertex->suffix_link = cur_edge->end;
                no_link_vertex = 0;
                no_link_depth = 0;
            }
        
            cur_edge = &(cur_edge->end->child[ch]);
            
            
            if (cur_edge->lim)
            {
                //встали на существующее ребро
                cur_edge_pos = cur_edge->beg;
                continue;
            }
            else
            {
                //создали новое ребро
                
                cur_edge->beg = cur_suf_pos;
                cur_edge->lim = (size_t) (-1); //бесконечность
                cur_edge->end = 0;
                
                //текущий суффикс получил свой лист, переходим к следующему суффиксу
                
                cur_suf++;
                cur_suf_pos = cur_suf;
                
                //прыгаем в корень дерева
                
                cur_edge = &edge_to_root;
                cur_edge_pos = 0;
                depth = 0;
                
                continue;
                
            }
        }
    }
    
    print_edge(0, s, &edge_to_root);
    
    
    return 0;
}