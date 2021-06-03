/*
   ===============================================================
   >Submission 5.1 CPP_CS_Ariel University 2021 Year_2 Semester_2
   >Author <Nathanael Benichou>
   >BinaryTree-a
   ================================================================
 */

//============
/* INCLUDES */
//============
#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdexcept>
#include <memory>
#include <vector>
using namespace std;
namespace ariel {

  //=================
  /* BinaryTree<T> */
  //=================
  template<typename T> class Node{
  public:
          T data;
           Node* n_right;
           Node* n_left;

          Node(T& data_) : data(data_), n_right(nullptr),  n_left(nullptr) {
          }
          
   Node(Node &src) = default;
  ~Node() = default;
   Node(Node&& other) = delete; // move constructor
   Node& operator=(Node& other) = delete; // copy assignment
   Node& operator=(Node&& other) = delete ;// move assignment

  };
template<typename T> class BinaryTree {

private:

//========
/* Node */
//========



void copy(const Node<T>& from, Node<T>& to){
        if(from.n_left!=nullptr) {
                to.n_left = new Node<T>(from.n_left->data);
                copy(*from.n_left,*to.n_left);
        }
        if(from.n_right!=nullptr) {
                to.n_right = new Node<T>(from.n_right->data);
                copy(*from.n_right,*to.n_right);
        }
}

//Print the tree
static void print(const string& pre, Node<T>* node, bool isLeft)
{
        if(node  !=nullptr)
        {
                cout << pre;
                string root = "├──";
                string child = "└──";
                cout << (isLeft ? root : child);

                // print the value of the node
                cout << node->data << endl;

                // enter the next tree level - left and right branch
                print( pre + (isLeft ? "│   " : "    "), node->n_right, true);
                print( pre + (isLeft ? "│   " : "    "), node->n_left, false);
        }
}


Node<T>* find_parent( T &n_parent , Node<T>* find){

  if(find == nullptr) {return nullptr;}
  if(find->data == n_parent) {return find;}

  Node<T>*  ans_left  = find_parent(n_parent,find->n_left);

  if(ans_left){return ans_left;}

  Node<T>*  ans_right  = find_parent(n_parent,find->n_right);

  return ans_right;
}
Node<T>* _root;

public:

BinaryTree() : _root(nullptr) {
}
//deep
BinaryTree(const BinaryTree<T> & other){
        if(other._root!=nullptr) {
                _root = new Node<T>(other._root->data);
                copy(*other._root,*_root);
        }
}

//Shallow
BinaryTree(BinaryTree<T> && other) noexcept {
        _root = other._root;
        other._root = nullptr;
}
//------Operators-----
 BinaryTree& operator=(BinaryTree other){
     if(this == &other){
         return *this;
     }
     if(_root != nullptr){
         delete this->_root;
     }
    _root = new Node<T>(other._root->data);
     copy(*other._root,*this->_root);
     return *this;

 }

 BinaryTree& operator=(BinaryTree&& other) noexcept{
     *this->_root = other._root;
 }

~BinaryTree() {};

//add_root - receives root places it at the root of the tree(Remplace it if exist).
BinaryTree& add_root( T data_){
        if(_root!=nullptr){
          _root->data= data_;
        }else{
          _root = new Node<T>(data_);
        }
        return *this;
}

//add_left - receives 2 nodes First is the parent that i will place the Second as left Child
BinaryTree &add_left( T n_parent,  T n_left){
  Node<T> * ans = find_parent(n_parent,_root);
  if(!ans){
   throw invalid_argument("excption");
  }
  if(ans->n_left == nullptr){
    ans ->n_left = new Node<T>(n_left);
  }else{
      ans ->n_left ->data = n_left;
  }
        return *this;
}

//add_right - receives 2 nodes First is the parent that i will place the Second as right Child
BinaryTree &add_right( T n_parent,  T n_right){
  Node<T> * ans = find_parent(n_parent,_root);
  if(!ans){
    throw invalid_argument("excption");
  }
  if(ans->n_right == nullptr){
    ans ->n_right = new Node<T>(n_right);
  }else{
      ans ->n_right ->data = n_right;
  }
        return *this;
}


//Output operator - prints the wood in some logical format
friend std::ostream &operator<<(std::ostream &os, const BinaryTree<T> &binaryTree){

          print("", binaryTree._root, false);

        return os;
}



//__________________________________________________________________________________________________

//============
/* iterator */
//============

//__________________________________________________________________________________________________

class iter
{
private:

void fill_vector(Node<T>* n,const string &type){
  if(n == nullptr){return;}

  if(type == "pre") {lis.push_back(n); }
  fill_vector(n->n_left ,type);
  if(type == "in") {lis.push_back(n); }
  fill_vector(n->n_right ,type);
  if(type == "post") { lis.push_back(n);}

}


public:
  Node<T>* n_current;
  vector<Node<T>*> lis;
  iter():n_current(nullptr){}
  iter(Node<T>* t):n_current(t){}
  iter(Node<T>* root ,const string &type){
    if(root != nullptr){
      fill_vector(root,type);
      n_current = lis.at(0);
    }else{
      n_current =nullptr;
    }
  }

T &operator*() const {
        return n_current->data;
}

T *operator->() const {
        return &(n_current->data);
}

iter &operator++(){
    if(lis.size()>1){
      lis.erase(lis.begin());
      n_current = lis[0];
    }else{
      n_current =nullptr;
    }
  return *this;
}

 iter operator++(int){
          iter t(lis[0]);
          ++*this;
        return t;
}

bool operator==(const iter &other) const {
        return n_current == nullptr;
}

bool operator!=(const iter &other) const {
        return !(*this == other);
}
// T& operator*(){
//              return n_current->data;
//          }
// T* operator->(){
//              return &n_current->data;
//     }
};

// begin_preorder / end_preorder
// Returns iterators to move in order of preorder (parent - left - right).
iter begin_preorder(){
        return iter{_root,"pre"};
}
iter end_preorder(){
        return iter{};
}


// begin_inorder - end_inorder
// Returns iterators to move in order in order (left - parent - right).
iter begin_inorder(){
  return iter{_root,"in"};
}
iter end_inorder(){
  return iter{};
}

//Defauld begin - End must be Inorder
iter begin(){
  return iter{_root,"in"};
};
iter end(){
  return iter{};
};


// begin_postorder - end_postorder
// Returns iterators for switching in order order (left - right - parent).

iter begin_postorder(){
      return iter{_root,"post"};
}
iter end_postorder(){
          return iter{};
}

//__________________________________________________________________________________________________






};




}
