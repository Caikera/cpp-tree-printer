//
// Created by Caikera on 2022/11/9.
//

#ifndef BIN_TREE_PRINTER_TREE_PRINTER_H
#define BIN_TREE_PRINTER_TREE_PRINTER_H

#include <cstddef>
#include <algorithm>
#include <functional>
#include <string>
#include <vector>
#include "char_box.h"

namespace koala {

    template<typename T>
    struct identity{
        using type = T;
    };

    struct print_node {
        char_box own_box; //its own char box, no sub node
        size_t level;
        size_t pos_x;
        size_t pos_y;
        std::vector<size_t> childs_y;
        size_t size_x; // size considers its sub node
        size_t size_y; // size considers its sub node
        print_node(char_box box, size_t level, size_t pos_x, size_t pos_y,
              std::vector<size_t>  childs_y, size_t size_x, size_t size_y);
        print_node(const print_node& another);
    };

    inline
    print_node::print_node(char_box box, size_t level, size_t pos_x, size_t pos_y, std::vector<size_t> childs_y,
                 size_t size_x, size_t size_y):
    own_box(std::move(box)), level(level), pos_x(pos_x), pos_y(pos_y), childs_y(std::move(childs_y)), size_x(size_x), size_y(size_y)
    {}

    inline
    print_node::print_node(const print_node& another):
            print_node(another.own_box, another.level, another.pos_x, another.pos_y, another.childs_y, another.size_x, another.size_y)
    {}


    struct tree_info{
    private:
        size_t level;
        size_t pos_x;
        size_t pos_y;
        size_t size_x;
        size_t size_y;
        std::vector<print_node> ps;
        tree_info(size_t level, size_t pos_x, size_t pos_y, size_t size_x, size_t size_y, const std::vector<print_node>& ps);
    public:
        char_box box();
        template<typename T>
        friend tree_info node_compile(T *root, typename identity<std::function<char_box (T *)>>::type get_char_box, typename identity<std::function<T *(T *, size_t)>>::type get_child, size_t level, size_t pos_x, size_t pos_y);
    };

    inline
    tree_info::tree_info(size_t level, size_t pos_x, size_t pos_y, size_t size_x, size_t size_y, const std::vector<print_node>& ps):
    level(level), pos_x(pos_x), pos_y(pos_y), size_x(size_x), size_y(size_y), ps(std::move(ps))
    {}

    /**
     * @brief construct a char_box of the tree graph to print
     * 
     * @return char_box 
     */
    inline
    char_box tree_info::box(){
        char_box cbox{size_y, size_x};
        for(auto p: ps){
            cbox.write(p.pos_y, p.pos_x, p.own_box);
            if(!p.childs_y.empty()){
                for(size_t i=p.pos_y+p.own_box.size_y; i<=p.childs_y.back()-1; i++) {
                    cbox[i][p.pos_x] = '|';
                }
                cbox[p.childs_y.back()][p.pos_x] = '`';
                for(auto child_y : p.childs_y)
                    cbox.write(child_y, p.pos_x+1, std::string{"--> "});
            }
        }
        return cbox;
    }

    template<typename T>
    tree_info node_compile(T* root,
                           typename identity<std::function<char_box(T*)>>::type get_char_box,
                           typename identity<std::function<T* (T*, size_t)>>::type get_child,
                           size_t level, size_t pos_x, size_t pos_y) {
        std::vector<print_node> ps{};
        char_box box = get_char_box(root);
        print_node p{box, level, pos_x, pos_y, {}, 0, 0};
        T *child;
        size_t cidx = 0;
        std::vector<tree_info> childs_cinfo{};
        size_t child_y = pos_y + box.size_y;
        while ((child = get_child(root, cidx), child != nullptr)) {
            p.childs_y.push_back(child_y);
            auto child_cinfo = node_compile(child, get_char_box, get_child, level, pos_x + 5, child_y);
            childs_cinfo.push_back(child_cinfo);
            child_y += child_cinfo.size_y;
            cidx += 1;
        }
        p.size_x = box.size_x;
        p.size_y = box.size_y;
        for (auto child_cinfo: childs_cinfo) {
            p.size_y += child_cinfo.size_y;
            if (5 + child_cinfo.size_x > p.size_x)
                p.size_x = 5 +  child_cinfo.size_x;
            ps.insert(ps.end(), child_cinfo.ps.begin(), child_cinfo.ps.end());
        }
        ps.push_back(p);
        return tree_info{p.level, p.pos_x, p.pos_y, p.size_x, p.size_y, ps};
    }

    /**
     * @brief Get the graph of a tree for printing.
     * 
     * @tparam T The node type of the tree to be printed.
     * @param root The ptr of root node of the tree to be printed.
     * @param get_char_box Determine how to represent each node in characters.
     * @param get_child Given a ptr of parent node, and an idx i, return ptr of its i-th child; return null ptr when i is out of range.
     * @return The char_box, containing the graph of the tree.   
     */
    template<typename T>
    char_box tree2graph(T* root,
                        typename identity<std::function<char_box(T*)>>::type get_char_box,
                        typename identity<std::function<T* (T*, size_t)>>::type get_child
                        ){
        return node_compile(root, get_char_box, get_child, 0, 0, 0).box();
    }

    /**
     * @brief Get the graph of a tree for printing.
     * 
     * @tparam T The node type of the tree to be printed.
     * @param root The ptr of root node of the tree to be printed.
     * @param get_str Determine how to represent each node in characters.
     * @param get_child Given a ptr of parent node, and an idx i, return ptr of its i-th child; return null ptr when i is out of range.
     * @return The char_box, containing the graph of the tree.   
     */
    template<typename T>
    char_box tree2graph(T* root,
                        typename identity<std::function<std::string(T*)>>::type get_str,
                        typename identity<std::function<T* (T*, size_t)>>::type get_child
                        ){
        return node_compile(root, [&get_str](T* root){return char_box{get_str(root)};},
                            get_child, 0, 0, 0).box();
    }

    /**
     * @brief Get the graph of a binary tree for printing.
     * 
     * @tparam T The node type of the tree to be printed.
     * @param root The ptr of root node of the tree to be printed.
     * @param get_char_box Determine how to represent each node in characters.
     * @param get_left Given a ptr of parent node, return ptr of its left child node; return nullptr when left child not exsist.
     * @param get_right Given a ptr of parent node, return ptr of its right child node; return nullptr when right child not exsist.
     * @return The char_box, containing the graph of the binary tree.   
     */
    template<typename T>
    char_box btree2graph(T* root,
                         typename identity<std::function<char_box(T*)>>::type get_char_box,
                         typename identity<std::function<T*(T*)>>::type get_left,
                         typename identity<std::function<T*(T*)>>::type get_right
                        ){
        return node_compile(root,
                            get_char_box,
                            [&get_left ,&get_right](T* root, size_t cidx){
                                if(cidx==0) return get_left(root);
                                else if(cidx==1) return get_right(root);
                                else return static_cast<T*>(nullptr);
                     },
                     0, 0, 0
        ).box();
    }

    /**
     * @brief Get the graph of a binary tree for printing.
     * 
     * @tparam T The node type of the tree to be printed.
     * @param root The ptr of root node of the tree to be printed.
     * @param get_str Determine how to represent each node in characters.
     * @param get_left Given a ptr of parent node, return ptr of its left child node; return nullptr when left child not exsist.
     * @param get_right Given a ptr of parent node, return ptr of its right child node; return nullptr when right child not exsist.
     * @return The char_box, containing the graph of the binary tree.   
     */
    template<typename T>
    char_box btree2graph(T* root,
                         typename identity<std::function<std::string(T*)>>::type get_str,
                         typename identity<std::function<T*(T*)>>::type get_left,
                         typename identity<std::function<T*(T*)>>::type get_right
    ){
        return btree2graph(root, [&get_str](T* root){return char_box{get_str(root)};},
                           get_left, get_right);
    }

}

#endif //BIN_TREE_PRINTER_TREE_PRINTER_H
