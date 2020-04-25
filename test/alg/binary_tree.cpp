
#include "gmock/gmock.h"
#include <string>
#include <memory>


namespace binary_tree
{


    template<typename T>
    struct Binary_Tree;

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
    };

    template<typename T>
    class PreOrder_Iterator
    {
        Node<T>* current;
    public:
        PreOrder_Iterator(Node<T> *current) : current(current) {}
    };

    template<typename T>
    class Binary_Tree
    {
    private:
        Node<T> *root = nullptr;
        PreOrder_Iterator<T> *iterator;

    public:
        explicit Binary_Tree(Node<T> *root) : root(root)
        {
            root->set_tree(this);
        }
        Binary_Tree(Node<T> *root, PreOrder_Iterator<T> *iterator) : root(root) , iterator(iterator)
        {
            root->set_tree(this);
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


}

using namespace testing;
using namespace binary_tree;


class A_Node_With_Two_Children_Test:public testing::Test
{

    public:
    virtual  void SetUp()
    {
        r_child = std::make_shared<Node<std::string>>("l_child");
        l_child = std::make_shared<Node<std::string>>("r_child");
        the_node = std::make_shared<Node<std::string>>("a_node", l_child.get(), r_child.get());

        other_node = std::make_shared<Node<std::string>>("other_child");

    };

    std::shared_ptr<Node<std::string>> l_child;
    std::shared_ptr<Node<std::string>> r_child;
    std::shared_ptr<Node<std::string>> the_node;

    std::shared_ptr<Node<std::string>> other_node;

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


TEST_F(A_Node_With_Two_Children_Test,preorder_iteration)
{
    auto iterator = std::make_shared<PreOrder_Iterator<std::string>>(the_node.get());
    auto tree = std::make_shared<Binary_Tree<std::string>>(the_node.get(), iterator.get());
    //    expect_the_node_and_children_in_tree(tree.get());
}








