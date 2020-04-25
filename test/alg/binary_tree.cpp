
#include "gmock/gmock.h"
#include <string>
#include <memory>


namespace binary_tree
{


    template<typename T>
    class Binary_Tree;

    template<typename T>
    struct Node
    {
        T value;
        Node<T> *left = nullptr;
        Node<T> *right = nullptr;
        Node<T> *parent = nullptr;
        Binary_Tree<T> *tree = nullptr;

        Node(const T& value):value(value){}
        Node(const T &value, Node<T> *left, Node<T> *right) : value(value), left(left), right(right)
        {
            this->left->tree = tree;
            this->right->tree = tree;
            this->left->parent = this;
            this->right->parent = this;
        }

        void set_tree(Binary_Tree<T> *tree);
        void set_left(Node<T> * left);
        void set_right(Node<T> * right);
    };

    template<typename T>
    class PreOrder_Iterator
    {
    public:
        Node<T>* current;
        PreOrder_Iterator(Node<T> *current) : current(current) {}
        Node<T>& operator*()
        {
            return *current;
        }

        void operator++()
        {
            if(has_right())
            {
                move_to_right();
                move_to_left_down_most();
            } else
            {
                move_to_left_up_most();
                move_to_parent();
            }
        }
        bool operator!=(const PreOrder_Iterator &other)
        {
            return current != other.current;
        }

    private:
        bool has_right()
        {
            return bool(current->right);
        }
        bool has_left()
        {
            return bool(current->left);
        }

        void move_to_right()
        {
            current = current->right;
        }

        void move_to_left_down_most()
        {
            while (has_left())
            {
                current = current->left;
            }
        }
        bool has_parent()
        {
            return bool(current->parent);
        }
        bool is_right_child()
        {
            return has_parent() && bool(current->parent->right == current);
        }
        void move_to_left_up_most()
        {
            while (is_right_child())
            {
                current = current->parent;
            }
        }
        void move_to_parent()
        {
            current = current->parent;
        }
    };

    template<typename T>
    class Binary_Tree
    {
    private:
        Node<T> *root = nullptr;

    public:
        explicit Binary_Tree(Node<T> *root) : root(root)
        {
            root->set_tree(this);
        }

        using iterator = PreOrder_Iterator<T>;
        iterator begin()
        {
            Node<T> *n = root;
            while (n->left)
            {
                n = n->left;
            }
            return iterator{n};
        }

        iterator end()
        {
            return iterator{nullptr};
        }

    };


/////////////////////////////imp/////////////////////////////////
// node
    template<typename T>
    void Node<T>::set_tree(Binary_Tree<T>* tree)
    {
        this->tree = tree;
        if(left)
        {
            left->set_tree(tree);
        }
        if(right)
        {
            right->set_tree(tree);
        }
    }

    template<typename T>
    void Node<T>::set_left(Node<T> *left)
    {
        this->left = left;
        this->left->parent = this;
        this->left->tree = tree;
    }
    template<typename T>
    void Node<T>::set_right(Node<T> *right)
    {
        this->right = right;
        this->right->parent = this;
        this->right->tree = tree;
    }
}

using namespace testing;
using namespace binary_tree;


class A_Node_With_Two_Children_Test:public testing::Test
{

    public:
    virtual  void SetUp()
    {
        r_child = std::make_shared<Node<std::string>>(r_child_value);
        l_child = std::make_shared<Node<std::string>>(l_child_value);
        the_node = std::make_shared<Node<std::string>>(the_node_value, l_child.get(), r_child.get());

        other_node = std::make_shared<Node<std::string>>(other_node_value);

    };

    std::shared_ptr<Node<std::string>> l_child;
    std::shared_ptr<Node<std::string>> r_child;
    std::shared_ptr<Node<std::string>> the_node;

    std::shared_ptr<Node<std::string>> other_node;

    std::string l_child_value = "l_child";
    std::string r_child_value = "l_child";
    std::string the_node_value = "the_node";
    std::string other_node_value = "other_node";

    void expect_the_node_and_children_in_tree(Binary_Tree<std::string>* tree)
    {
        EXPECT_THAT(the_node->tree, Eq(tree));
        EXPECT_THAT(l_child->tree, Eq(tree));
        EXPECT_THAT(r_child->tree, Eq(tree));
    }

};


TEST_F(A_Node_With_Two_Children_Test,the_node_s_left_is_left_child)
{
    EXPECT_THAT(the_node->left,Eq(l_child.get()));
    EXPECT_THAT(the_node->right,Eq(r_child.get()));
}

TEST_F(A_Node_With_Two_Children_Test,left_child_s_parent_is_the_node)
{
    EXPECT_THAT(l_child->parent,Eq(the_node.get()));
    EXPECT_THAT(r_child->parent,Eq(the_node.get()));
}

TEST_F(A_Node_With_Two_Children_Test,set_tree_affact_all_sub_nodes)
{
    auto tree = std::make_shared<Binary_Tree<std::string>>(other_node.get());
    the_node->set_tree(tree.get());
    expect_the_node_and_children_in_tree(tree.get());
}

TEST_F(A_Node_With_Two_Children_Test,new_a_tree_with_the_node)
{
    auto tree = std::make_shared<Binary_Tree<std::string>>(the_node.get());
    expect_the_node_and_children_in_tree(tree.get());
}


TEST_F(A_Node_With_Two_Children_Test,preorder_iteration_first_element_is_root)
{
    auto tree = std::make_shared<Binary_Tree<std::string>>(the_node.get());
    std::vector<std::string> act;
    for (auto it : *tree)
    {
        act.push_back(it.value);
    }
    EXPECT_THAT(act[0],Eq(l_child_value));
}




TEST_F(A_Node_With_Two_Children_Test,preorder_iteration)
{
    auto tree = std::make_shared<Binary_Tree<std::string>>(the_node.get());
    std::vector<std::string> act;
    for (auto it : *tree)
    {
        act.push_back(it.value);
    }
    EXPECT_THAT(act,ElementsAre(l_child_value,the_node_value,r_child_value));
}


class More_Nodes_Test:public testing::Test
{
protected:
    virtual void SetUp()
    {
    }

    void build_tree(int level)
    {
        max_level = level;
        nodes.push_back(std::make_shared<Node<std::string>>("0"));
        current_level++;
        auto root = nodes.back().get();
        add_child(root);
        tree = std::make_shared<Binary_Tree<std::string>>(root);
    }
    void add_child(Node<std::string> *node){
        if (current_level < max_level)
        {
            current_level++;
            auto left_node = std::make_shared<Node<std::string>>(node->value + "0");
            auto right_node = std::make_shared<Node<std::string>>(node->value + "1");
            node->set_left(left_node.get());
            node->set_right(right_node.get());
            push_back_node(left_node);
            push_back_node(right_node);
            add_child(left_node.get());
            add_child(right_node.get());
            current_level--;
        }
    } 

    void push_back_node(std::shared_ptr<Node<std::string>> node)
    {
        nodes.push_back(node);
        values.push_back(node->value);
    }


    std::shared_ptr<Binary_Tree<std::string>> tree;
    std::vector<std::shared_ptr<Node<std::string>>> nodes;
    std::vector<std::string> values;
    int max_level = -1;
    int current_level = 0;
};

TEST_F(More_Nodes_Test, size_5)
{
    build_tree(3);
    std::vector<std::string> act;
    for(auto it:*tree)
    {
        act.push_back(it.value);
    }
    EXPECT_THAT(act,ElementsAre("000","00","001","0","010","01","011"));
}


