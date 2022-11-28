# Koala's Tree Printer
This is an easy-use tool to print the custom tree structure in cpp project. It is header-only. To use it, simply copy `char_box.h` and `tree_printer.h` under `include` folder to your project directory.
## Interface
Function
```
char_box tree2graph(T* root,
                    std::function<std::string(T*)> get_str,
                    std::function<T* (T*, size_t)> get_child)
```
will convert the tree into a box of characters. The first argument `root`, is the root node of the tree. The second argument `get_str`, accept a function, who generates the string to print given a node.
The third argument `get_child` accept a function, too. Given a node and an index i, it returns a pointer to i-th child of the node, and nullptr should be returned if i-th child node does not exsist.
Once we get the char_box for the tree, we can use its member function `print` to print it to console.  
Specially, function `bin_tree2graph` is prepared for binary tree. Function `get_left` is used to return the left child of a node. And `get_right` is used to return the right node.
```
char_box bin_tree2graph(T* root,
                     std::function<std::string(T*)>> get_str,
                     std::function<T*(T*)>get_left,
                     std::function<T*(T*)>get_right)
```

## Examples
To run examples in `examples` folder, `fmtlib` might be needed. However, it is not essentially-necessary for this tool. You just need some function to convert your own structure into a string, such as `sprintf`.  
### Binary Tree
Suppose we have a binary tree. Its node is defined as  
```
struct bnode {
        double p;
        std::string str;
        bnode *left;
        bnode *right;
    };
```
We could use following code to print it out
```
auto broot = nodes[0];
    auto graph3 = koala::bin_tree2graph(broot,
                                     [](bnode *n){return '|' + n->str + '|';},
                                     [](bnode *root){return root->left;},
                                     [](bnode *root){return root->right;}
    );
    graph3.print();
```
We will get something like:
```
|a+b+e+f+d+c|
|--> |a+b+e|
|    |--> |a+b|
|    |    |--> |a|
|    |    `--> |b|
|    `--> |e|
`--> |f+d+c|
     |--> |f|
     `--> |d+c|
          |--> |d|
          `--> |c|
```
Or by doing  
```
auto get_str = [](bnode* n) -> std::string{
        return fmt::format("+{0:-<{1}}+\n|{2}|\n+{0:-<{1}}+", "", n->str.length(), n->str);
    };
auto get_left = [](bnode* root){ return root->left; };
auto get_right = [](bnode* root){ return root->right; };
auto graph4 = koala::bin_tree2graph(broot, get_str, get_left, get_right);
graph4.print();
```
We will get something like  
```
+-----------+
|a+b+e+f+d+c|
+-----------+
|--> +-----+
|    |a+b+e|
|    +-----+
|    |--> +---+
|    |    |a+b|
|    |    +---+
|    |    |--> +-+
|    |    |    |a|
|    |    |    +-+
|    |    `--> +-+
|    |         |b|
|    |         +-+
|    `--> +-+
|         |e|
|         +-+
`--> +-----+
     |f+d+c|
     +-----+
     |--> +-+
     |    |f|
     |    +-+
     `--> +---+
          |d+c|
          +---+
          |--> +-+
          |    |d|
          |    +-+
          `--> +-+
               |c|
               +-+
```
### Common
If we have a tree, every node could contain a variety of child nodes:
```
struct tnode {
        std::string str;

        explicit tnode(const std::string &str, std::vector<std::unique_ptr<tnode>> childs) :
            str(str), childs(std::move(childs))
            {}
        
        std::vector<std::unique_ptr<tnode>> childs;
    };
```
We can use following code to print it:  
```
auto graph = koala::tree2graph(root,
                               [](tnode *root) { return root->str; },
                               [](tnode *root, size_t idx) {
                                   if (root->childs.size() == 0 || idx > root->childs.size() - 1)
                                       return static_cast<tnode *>(nullptr);
                                   else
                                       return root->childs[idx].get();
                                }
);
graph.print();
```
We will get things like
```
root
|--> c0
|    |--> c00
|    `--> c02
|--> c1
|    |--> c10
|    |--> c11
|    |--> c12
|    `--> c13
|         |--> c130
|         `--> c131
`--> c2
```

