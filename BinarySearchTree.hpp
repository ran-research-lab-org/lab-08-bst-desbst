// Desteny Hernandez De Juan
// 401 - 23 - 3159
#ifndef BINARY_SEARCH_TREE_H
#define BINARY_SEARCH_TREE_H

#include <algorithm>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <queue>
using namespace std;

template <typename T>
string toStr(const T &value)
{
  ostringstream oss;
  oss << value;
  return oss.str();
}

// BinarySearchTree class
//
// CONSTRUCTION: zero parameter
//
// ******************PUBLIC OPERATIONS*********************
// void insert( x )       --> Insert x
// void remove( x )       --> Remove x
// bool contains( x )     --> Return true if x is present
// Comparable findMin( )  --> Return smallest item
// Comparable findMax( )  --> Return largest item
// boolean isEmpty( )     --> Return true if empty; else false
// void makeEmpty( )      --> Remove all items
// void printTree( )      --> Print tree in sorted order
// ******************ERRORS********************************
// Throws UnderflowException as warranted

template <typename Comparable>
class BinarySearchTree
{
public:
  BinarySearchTree() : root{nullptr} {}

  // Copy constructor
  BinarySearchTree(const BinarySearchTree &rhs) : root{nullptr}
  {
    root = clone(rhs.root);
  }

  // Move constructor
  BinarySearchTree(BinarySearchTree &&rhs) : root{rhs.root}
  {
    rhs.root = nullptr;
  }

  // Destructor for the tree
  ~BinarySearchTree() { makeEmpty(); }

  // Copy assignment
  BinarySearchTree &operator=(const BinarySearchTree &rhs)
  {
    BinarySearchTree copy = rhs;
    std::swap(*this, copy);
    return *this;
  }

  // Move assignment
  BinarySearchTree &operator=(BinarySearchTree &&rhs)
  {
    std::swap(root, rhs.root);
    return *this;
  }

  // Find the smallest item in the tree. Throw underflow_error if empty.
  const Comparable &findMin() const
  {
    if (isEmpty())
      throw std::underflow_error("Can't find in an empty tree!");
    return findMin(root)->element;
  }

  // Find the largest item in the tree. Throw underflow_error if empty.
  const Comparable &findMax() const
  {
    if (isEmpty())
      throw std::underflow_error("Can't find in an empty tree!");
    return findMax(root)->element;
  }

  // Returns true if x is found in the tree.
  bool contains(const Comparable &x) const { return contains(x, root); }

  // Test if the tree is logically empty.  Return true if empty, false
  // otherwise.
  bool isEmpty() const { return root == nullptr; }

  // Print the tree contents in sorted order.
  void printTree(ostream &out = cout) const
  {
    if (isEmpty())
      out << "Empty tree" << endl;
    else
      printTree(root, out);
  }

  // imprime los valores del árbol en orden
  string toInorderStr() const
  {
    string st;
    if (isEmpty())
      st = "Empty tree";
    else
    {
      toInorderStr(root, st);
      st.pop_back();
    }
    return st;
  }

  // Make the tree logically empty.
  void makeEmpty() { makeEmpty(root); }

  // Insert x into the tree; duplicates are ignored.
  void insert(const Comparable &x) { insert(x, root); }

  // Insert x into the tree; duplicates are ignored.
  void insert(Comparable &&x) { insert(std::move(x), root); }

  // Remove x from the tree. Nothing is done if x is not found.
  void remove(const Comparable &x) { remove(x, root); }

  // to-do!
  string BFT() const
  {
    queue<BinaryNode *> ptrNodes;
    string treeString; // declaro el string que guarda os elementos y sus elementos
    // desde el nivel 1
    // guarda el puntero del root en el queue
    if (root != nullptr)
    {
      ptrNodes.push(root);
    }

    bool levelStart = true;
    int level = -1;
    int nodeCounter = 0;
    int maxNodesInLevel = 1;
    int missingNodes = 0; // cuanta los nodos que NO van a estar en un nivel
    int nextMissingNodes = 0;

    treeString += "[";
    // else no entra al while loop pq el queue está vacía y se sale de la función
    while (!ptrNodes.empty())
    {
      if (levelStart) // si estamos empezando el nivel...
      {
        level++; // entramos al otro nivel

        if (level != 0)      // le añade una coma entre cada nivel [..],[...],[...]
          treeString += ","; // de esta forma no se le añade al último nivel
        treeString += "[";   // se abre el bracket
        levelStart = false;  // actualiza a false

        // calcula los nodos maximos que puede tener un nivel, 2^nivel
        for (int i = level; i > 0; i--)
          maxNodesInLevel *= 2;

        maxNodesInLevel -= nextMissingNodes; // se le resta los missing nodes de hace 2 niveles atras
        maxNodesInLevel -= missingNodes;     // se le resta los missing nodes del nivel anterior

        // por cada missing node que hay...
        // en el proximo nivel van a faltar 2 nodos y se siguen acumulando
        nextMissingNodes = missingNodes * 2;

        missingNodes = 0; // se actualiza missingNodes
      }

      // se guarda el puntero del queue en otra variable
      BinaryNode *g = ptrNodes.front();
      ptrNodes.pop();
      treeString += toStr(g->element); // se guarda el elemento
      nodeCounter++;

      // si se dectecta que el left or right es nulo
      // entonces la cantidad de nodos maximo para el prox. nivel va ser maxNodesInLevel - missingNodes
      if (g->left == nullptr)
        missingNodes++;
      if (g->right == nullptr)
        missingNodes++;

      // entra aqui si el counter de nodos es igual al maximo de nodos del nivel
      if (nodeCounter == maxNodesInLevel)
      {
        treeString += "]";
        levelStart = true;   // se actualiza a que va a iniciar un nuevo nivel
        maxNodesInLevel = 1; // se actualiza
        nodeCounter = 0;     // actualiza a 0
      }

      // coma para los elementos [.,.,.]
      if (!levelStart) // si levelStart == true, llegamos al ultimo y no se guarda la coma
        treeString += ",";

      if (g->left != nullptr)
        ptrNodes.push(g->left);

      if (g->right != nullptr)
        ptrNodes.push(g->right);
    }
    treeString += "]";
    return treeString;
  }

private:
  struct BinaryNode
  {
    Comparable element;
    BinaryNode *left;
    BinaryNode *right;

    BinaryNode(const Comparable &theElement, BinaryNode *lt, BinaryNode *rt)
        : element{theElement}, left{lt}, right{rt} {}

    BinaryNode(Comparable &&theElement, BinaryNode *lt, BinaryNode *rt)
        : element{std::move(theElement)}, left{lt}, right{rt} {}
  };

  BinaryNode *root;

  /**
   * Internal method to insert into a subtree.
   * x is the item to insert.
   * t is the node that roots the subtree.
   * Set the new root of the subtree.
   */
  void insert(const Comparable &x, BinaryNode *&t)
  {
    if (t == nullptr)
      t = new BinaryNode{x, nullptr, nullptr};
    else if (x < t->element)
      insert(x, t->left);
    else if (t->element < x)
      insert(x, t->right);
    else
      ; // Duplicate; do nothing
  }

  /**
   * Internal method to insert into a subtree.
   * x is the item to insert.
   * t is the node that roots the subtree.
   * Set the new root of the subtree.
   */
  void insert(Comparable &&x, BinaryNode *&t)
  {
    if (t == nullptr)
      t = new BinaryNode{std::move(x), nullptr, nullptr};
    else if (x < t->element)
      insert(std::move(x), t->left);
    else if (t->element < x)
      insert(std::move(x), t->right);
    else
      ; // Duplicate; do nothing
  }

  /**
   * Internal method to remove from a subtree.
   * x is the item to remove.
   * t is the node that roots the subtree.
   * Set the new root of the subtree.
   */
  void remove(const Comparable &x, BinaryNode *&t)
  {
    if (t == nullptr)
      return; // Item not found; do nothing
    if (x < t->element)
      remove(x, t->left);
    else if (t->element < x)
      remove(x, t->right);
    else if (t->left != nullptr && t->right != nullptr) // Two children
    {
      t->element = findMin(t->right)->element;
      remove(t->element, t->right);
    }
    else
    {
      BinaryNode *oldNode = t;
      t = (t->left != nullptr) ? t->left : t->right;
      delete oldNode;
    }
  }

  /**
   * Internal method to find the smallest item in a subtree t.
   * Return node containing the smallest item.
   */
  BinaryNode *findMin(BinaryNode *t) const
  {
    if (t == nullptr)
      return nullptr;
    if (t->left == nullptr)
      return t;
    return findMin(t->left);
  }

  // encuentra el nodo mayor del árbol
  BinaryNode *findMax(BinaryNode *t) const
  {
    if (t != nullptr)
      while (t->right != nullptr)
        t = t->right;
    return t;
  }

  /**
   * Internal method to test if an item is in a subtree.
   * x is item to search for.
   * t is the node that roots the subtree.
   */
  bool contains(const Comparable &x, BinaryNode *t) const
  {
    if (t == nullptr)
      return false;
    else if (x < t->element)
      return contains(x, t->left);
    else if (t->element < x)
      return contains(x, t->right);
    else
      return true; // Match
  }
  /****** NONRECURSIVE VERSION*************************
      bool contains( const Comparable & x, BinaryNode *t ) const
      {
          while( t != nullptr )
              if( x < t->element )
                  t = t->left;
              else if( t->element < x )
                  t = t->right;
              else
                  return true;    // Match

          return false;   // No match
      }
  *****************************************************/

  // Internal method to make subtree empty.
  void makeEmpty(BinaryNode *&t)
  {
    if (t != nullptr)
    {
      makeEmpty(t->left);
      makeEmpty(t->right);
      delete t;
    }
    t = nullptr;
  }

  // Internal method to print a subtree rooted at t in sorted order.
  void printTree(BinaryNode *t, ostream &out) const
  {
    if (t != nullptr)
    {
      printTree(t->left, out);
      out << t->element << endl;
      printTree(t->right, out);
    }
  }

  // Internal method to print a subtree rooted at t in sorted order.
  void toInorderStr(BinaryNode *t, string &st) const
  {
    if (t != nullptr) // si el nodo no está vacío
    {
      toInorderStr(t->left, st);         // empieza por el menor
      st = st + toStr(t->element) + ","; // cuando termine ese call stack
      toInorderStr(t->right, st);        // va por la rama derecha del árbol
    }
  }

  // Internal method to clone subtree.
  BinaryNode *clone(BinaryNode *t) const
  {
    if (t == nullptr)
      return nullptr;
    else
      return new BinaryNode{t->element, clone(t->left), clone(t->right)};
  }
};

#endif
