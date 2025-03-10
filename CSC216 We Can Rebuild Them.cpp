#include <cstdlib>
#include <algorithm>
#include <string>
#include <iostream>

using namespace std;

class AVLTree{
    public:
    class Node{
        public:
        long bf;

        long value;

        long height = 0;

        Node* parent;
        Node* left{nullptr};
        Node* right{nullptr};

        Node() {parent = nullptr;}
        Node(const long val, Node* p = nullptr) : value(val), parent(p){}

    };
    private:
    Node* node;

    long value() {return node->value;}
    const long value() const {return node->value;}
    Node* left() const {return node->left;}
    Node* right() const {return node->right;}
    Node* parent() const {return node->parent;}

    public:
    Node* root;

    AVLTree() : root(new Node()) {}

    long height(Node* p) const
    {
        return p == nullptr ? 0 : p->height;
    }

    void recomputeHeight(Node* p)
    {
        p->height = max(height(p->left),height(p->right));
    }

    bool isBalanced(Node* p)
    {
        return abs(height(p->left) - height(p->right)) <= 1;
    }

    void relink(Node* parent, Node* child, bool make_left_child)
    {
        if (make_left_child)
            parent->left = child;
        else
            parent->right = child;
        if (child != nullptr)
            child->parent = parent;
    }
    
    void rotate(Node* x)
    {
        Node* y = x->parent;               
        Node* z = y->parent;                      
        relink(z, x, y == z->left);                
       
        if (x == y->left) {
            relink(y, x->right, true);  
            relink(x, y, false);      
        } else {
            relink(y, x->left, false);              
            relink(x, y, true);                     
        }
    }

    Node* restructure(Node* x)
    {
        Node* y = x->parent;
        Node* z = y->parent;
        if ((x == y->right) == (y == z->right)) {   
            rotate(y);                      
            return y;       
        } else {    
            rotate(x); 
            rotate(x);
            return x; 
        }
    }

    Node* tallerChild(Node* p)
    {
        if (height(p->left) > height(p->right)) return p->left;
        if (height(p->left) < height(p->right)) return p->right;
        
        if (p == p->parent->left) return p->left;
        else return p->right;
    }

    void rebalance(Node* p) {
        long old_height, new_height;
        do {
            old_height = height(p);       
            if (!isBalanced(p)) {          
                p = restructure(tallerChild(tallerChild(p)));
                recomputeHeight(p->left);
                recomputeHeight(p->right);
            }
            recomputeHeight(p);
            new_height = height(p);
            p = p->parent;
        } while (old_height != new_height && p != root);
    }

    void rebalanceInsert(Node* p) { rebalance(p); }

    void rebalanceDelete(Node* p)
    {
        if (p != root)
            rebalance(p);
    }

    void insert(long val)
    {
        Node* p = root;
        bool inserted = false;
        while(!inserted)
        {
            if(p == nullptr)
            {
                p = new Node(val);
                inserted = true;
            }
            else
            {
                if(val > p->value)
                {
                    p = p->right;
                }
                else if(val < p->value)
                {
                    p = p->left;
                }
                else if(val == p->value)
                {
                    inserted = true;
                }
            }
        }
        rebalanceInsert(p);
    }

    void removeNode(Node* p)
    {
        Node* child{p->left == nullptr ? p->right : p->left};
        if (child != nullptr)
            child->parent = p->parent;           

        if (p == root)
            root = child;                     
        else if (p->parent->left == p)          
            p->parent->left = child;
        else                               
            p->parent->right = child;
        delete p;
    }

    void erase(long val)
    {
        Node* p = root;
        bool deleted = false;
        while(!deleted)
        {
            if(p == nullptr)
            {
                deleted = true;
            }
            else
            {
                if(val > p->value)
                {
                    p = p->right;
                }
                else if(val < p->value)
                {
                    p = p->left;
                }
                else if(val == p->value)
                {
                    if (p->left != nullptr && p->right != nullptr) {    
                        Node* before = p->left;
                        while (before->right != nullptr)
                            before = before->right;
                        p->value = before->value;       
                        p = before;             
                    }
                    Node* new_parent = p->parent;
                    removeNode(p);
                    rebalanceDelete(new_parent);
                    deleted = true;
                }
            }
        }
    }

    void inorder(Node* p, string& visited) {
        if (p != nullptr) {
            inorder(p->left, visited);
            visited = visited + to_string(p->value) + ',';
            inorder(p->right, visited);
        }
    }

    string sterilize()
    {
        string tree_display;
        inorder(root, tree_display);
        return tree_display;
    }
    
};

long main(void)
{
    AVLTree new_tree;

    new_tree.insert(3);
    new_tree.insert(4);
    new_tree.insert(5);
    new_tree.insert(6);

    cout << new_tree.sterilize();

    new_tree.erase(6);

    cout << new_tree.sterilize();

    return 0;
}

