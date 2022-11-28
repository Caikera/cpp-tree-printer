//
// Created by Caikera on 2022/11/11.
//

#include "fmt/core.h"
#include <cstdio>
#include <algorithm>
#include <string>
#include <tuple>
#include <vector>
#include "char_box.h"
#include "tree_printer.h"

namespace {
    struct bnode {
        double p;
        std::string str;
        bnode *left;
        bnode *right;
    };

    std::vector<bnode *> bin_tree_construct() {
        using pairs = std::tuple<std::string, double>;
        std::vector<pairs> symbol_prob_pairs{
                {"a", 0.1},
                {"b", 0.2},
                {"c", 0.3},
                {"d", 0.3},
                {"e", 0.4},
                {"f", 0.5}
        };
        std::vector<bnode *> nodes;
        for (auto pair: symbol_prob_pairs) {
            nodes.push_back(new bnode{std::get<1>(pair), std::get<0>(pair), nullptr, nullptr});
        }
        while (nodes.size() > 1) {
            std::sort(nodes.begin(), nodes.end(), [](bnode *i, bnode *j) -> bool { return i->p > j->p; });
            for (auto node: nodes)
                fmt::print("{:<15}", fmt::format("{}: {:1.1f}", node->str, node->p));
            fmt::print("\n");
            bnode *x = nodes.back();
            nodes.pop_back();
            bnode *y = nodes.back();
            nodes.pop_back();
            nodes.push_back(new bnode{x->p + y->p, x->str + "+" + y->str, x, y});
        }
        return nodes;
    };

    void btree_deconstruct(std::vector<bnode *> nodes) {
        for (auto node: nodes)
            delete node;
    }
}

void bin_tree_test(){
    auto nodes = bin_tree_construct();
    auto broot = nodes[0];
    auto graph = koala::tree2graph(broot,
                                   [](bnode *n) {return koala::char_box{n->str};},
                                   [](bnode *n, size_t cidx) {
                                       if (cidx == 0)
                                           return n->left;
                                       else if (cidx == 1)
                                           return n->right;
                                       else
                                           return static_cast<bnode *>(nullptr);
                                   }
    );
    graph.print();

    auto graph2 = koala::bin_tree2graph(broot,
                                     [](bnode* n){return koala::char_box{n->str};},
                                     [](bnode* n){return n->left;},
                                     [](bnode* n){return n->right;}
    );
    graph2.print();

    auto graph3 = koala::bin_tree2graph(broot,
                                     [](bnode *n){return '|' + n->str + '|';},
                                     [](bnode *root){return root->left;},
                                     [](bnode *root){return root->right;}
    );
    graph3.print();

    auto get_str = [](bnode* n) -> std::string{
        return fmt::format("+{0:-<{1}}+\n|{2}|\n+{0:-<{1}}+", "", n->str.length(), n->str);
    };
    auto get_left = [](bnode* root){ return root->left; };
    auto get_right = [](bnode* root){ return root->right; };
    auto graph4 = koala::bin_tree2graph(broot, get_str, get_left, get_right);
    graph4.print();

    btree_deconstruct(nodes);
}