//
// Created by Caikera on 2022/11/11.
//

#include "fmt/core.h"
#include <cstdio>
#include <algorithm>
#include <memory>
#include <string>
#include <tuple>
#include <utility>
#include <vector>
#include "char_box.h"
#include "tree_printer.h"

namespace {
    struct tnode {
        std::string str;

        explicit tnode(const std::string &str, std::vector<std::unique_ptr<tnode>> childs) :
                str(str), childs(std::move(childs)) {
            //fmt::print("  construct tnode: {}\n", this->str);
        }

        ~tnode() {
            //fmt::print("  deconstruct tnode: {}\n", str);
        }

        std::vector<std::unique_ptr<tnode>> childs;
    };

    std::unique_ptr<tnode> tree_construct() {
        auto root{
                std::make_unique<tnode>(std::string{"root"}, std::vector<std::unique_ptr<tnode>>{})
        };
        auto c0{
                std::make_unique<tnode>(std::string{"c0"}, std::vector<std::unique_ptr<tnode>>{})
        };
        auto c00{
                std::make_unique<tnode>(std::string{"c00"}, std::vector<std::unique_ptr<tnode>>{})
        };
        c0->childs.push_back(std::move(c00));
        auto c01{
                std::make_unique<tnode>(std::string{"c01"}, std::vector<std::unique_ptr<tnode>>{})
        };
        c0->childs.push_back(std::move(c01));
        auto c02{
                std::make_unique<tnode>(std::string{"c02"}, std::vector<std::unique_ptr<tnode>>{})
        };
        c0->childs.push_back(std::move(c02));
        root->childs.push_back(std::move(c0));
        auto c1{
                std::make_unique<tnode>(std::string{"c1"}, std::vector<std::unique_ptr<tnode>>{})
        };
        auto c10{
                std::make_unique<tnode>(std::string{"c10"}, std::vector<std::unique_ptr<tnode>>{})
        };
        c1->childs.push_back(std::move(c10));
        auto c11{
                std::make_unique<tnode>(std::string{"c11"}, std::vector<std::unique_ptr<tnode>>{})
        };
        c1->childs.push_back(std::move(c11));
        auto c12{
                std::make_unique<tnode>(std::string{"c12"}, std::vector<std::unique_ptr<tnode>>{})
        };
        c1->childs.push_back(std::move(c12));
        auto c13{
                std::make_unique<tnode>(std::string{"c13"}, std::vector<std::unique_ptr<tnode>>{})
        };
        auto c130{
                std::make_unique<tnode>(std::string{"c130"}, std::vector<std::unique_ptr<tnode>>{})
        };
        c13->childs.push_back(std::move(c130));
        auto c131{
                std::make_unique<tnode>(std::string{"c131"}, std::vector<std::unique_ptr<tnode>>{})
        };
        c13->childs.push_back(std::move(c131));
        c1->childs.push_back(std::move(c13));
        root->childs.push_back(std::move(c1));
        auto c2{
                std::make_unique<tnode>(std::string{"c2"}, std::vector<std::unique_ptr<tnode>>{})
        };
        root->childs.push_back(std::move(c2));
        return root;
    }
}

bool tree_test(){
    {
        auto root{tree_construct()};
        auto graph = koala::tree2graph(root.get(),
                                       [](tnode *root) { return root->str; },
                                       [](tnode *root, size_t idx) {
                                           //fmt::print("idx = {};\n", idx);
                                           if (root->childs.size() == 0 || idx > root->childs.size() - 1)
                                               return static_cast<tnode *>(nullptr);
                                           else
                                               return root->childs[idx].get();
                                       }
        );
        graph.print();
        //fmt::print("deconstructing:\n");
    }
    return true;
}
