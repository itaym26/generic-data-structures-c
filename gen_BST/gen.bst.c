#include <stdio.h>
#include <stdlib.h>
#include "BSTree.h"

typedef struct BSTreeNode
{
    void* m_data;
    struct BSTreeNode* m_left;
    struct BSTreeNode* m_right;
    struct BSTreeNode* m_parent;
} BSTreeNode;

struct BSTree{
    BSTreeNode* m_sentinel;//pointer to the root node the left sun is the first node in the tree and the right is null
    Comparator m_Less;//function pointer to the comparator function
};

static BSTreeNode* CreateNode(void* _item) {
    BSTreeNode* newNode = (BSTreeNode*)malloc(sizeof(BSTreeNode));
    if (newNode) {
        newNode->m_data = _item;
        newNode->m_left = newNode->m_right = NULL;
        newNode->m_parent = NULL;   
    }
    return newNode;
}
BSTree* BSTreeCreate(Comparator _less){
    if(_less == NULL) return NULL;
    BSTree* tree = (BSTree*)malloc(sizeof(BSTree));
    if(tree == NULL) return NULL;

    tree->m_sentinel = CreateNode(NULL);// create sentinel node with null data
    if (tree->m_sentinel == NULL) {
        free(tree);
        return NULL;
    }
    tree->m_Less = _less;
    return tree;
}

static void DestroyNode(BSTreeNode* node, void (*_destroyer)(void*)){
    if (node == NULL) return;
    // rec call for Left
    DestroyNode(node->m_left, _destroyer);
    // rec call for Right
    DestroyNode(node->m_right, _destroyer);
    // destroy data and check if destroyer is not null and data is not null
    if (_destroyer != NULL && node->m_data != NULL) {
        _destroyer(node->m_data);
    }   
    // free node   
    free(node);
}
void BSTreeDestroy(BSTree**_tree, void (*_destroyer)(void*)){
   if (_tree == NULL || *_tree == NULL ) return;
   // clear all nodes in tree
    DestroyNode((*_tree)->m_sentinel->m_left, _destroyer);
    // free the sentinel
    free((*_tree)->m_sentinel);
    //free the tree struct
    free(*_tree);
    *_tree = NULL;

    printf("Tree destroyed successfully\n");
}

BSTreeItr BSTreeInsert(BSTree* _tree, void* _item){
    if (_tree == NULL || _item == NULL) return NULL;
    BSTreeNode* newNode;
    BSTreeNode* parent = _tree->m_sentinel;
    BSTreeNode* current = _tree->m_sentinel->m_left;

    while (current != NULL){
        parrent = current;
        if (_tree->m_Less(_item, current->m_data)) {
            current = current->m_left;
        } else if (_tree->m_Less(current->m_data, _item)) {
            current = current->m_right;
        } else {
            // Duplicate item, free the new node and return NULL
            free(newNode);
            return (BSTreeItr)_tree->m_sentinel; // return end iterator on failure
        }
    }
    newNode = CreateNode(_item);
    if (newNode == NULL) return NULL; // Allocation failed
    newNode->m_parent = parent;

    if (parent == _tree->m_sentinel) {
        _tree->m_sentinel->m_left = newNode; // Tree was empty, new node is now root
    } 
    //chack if the new item is less than the parent item to decide where to insert  
    else if (_tree->m_Less(_item, parent->m_data)) {
        parent->m_left = newNode;
    } else {
        parent->m_right = newNode;
    }
    return (BSTreeItr)newNode;
}

BSTreeItr BSTreeItrBegin(const BSTree* _tree){
    if (_tree == NULL) return NULL;
    BSTreeNode* current = _tree->m_sentinel->m_left; // Start from the root
    while (current != NULL && current->m_left != NULL) {
        current = current->m_left; // Move to the leftmost node
    }
    return (BSTreeItr)current; // Return iterator to the leftmost node
}

BSTreeItr BSTreeItrEnd(const BSTree* _tree){
    if (_tree == NULL) return NULL;
    return (BSTreeItr)_tree->m_sentinel; // End iterator points to the sentinel
}

BSTreeItr BSTreeItrNext(BSTreeItr _it){
    if (_it == NULL) return NULL;
    BSTreeNode* current = (BSTreeNode*)_it;// do unencapsulation to get the node from the iterator

    if (current->m_right != NULL) {
        // If there is a right child, go to the leftmost node in the right subtree
        current = current->m_right;
        while (current->m_left != NULL) {
            current = current->m_left;
        }
        return (BSTreeItr)current;
    } else {
        // No right child, go up until we find a node that is a left child of its parent
        BSTreeNode* parent = current->m_parent;
        while (parent != NULL && current == parent->m_right) {// move until i find that the current node is a left child of its parent  
            current = parent;
            parent = parent->m_parent;
        }
        return (BSTreeItr)parent; // Return the parent which is the next in-order node
    }
}

BSTreeItr BSTreeItrPrev(BSTreeItr _it){
    if (_it == NULL) return NULL;
    BSTreeNode* current = (BSTreeNode*)_it; // do unencapsulation to get the node from the iterator

    if (current->m_left != NULL) {
        // If there is a left child, go to the rightmost node in the left subtree
        current = current->m_left;
        while (current->m_right != NULL) {
            current = current->m_right;
        }
        return (BSTreeItr)current;
    } else {
        // No left child, go up until we find a node that is a right child of its parent
        BSTreeNode* parent = current->m_parent;
        while (parent != NULL && current == parent->m_left) { // move until i find that the current node is a right child of its parent  
            current = parent;
            parent = parent->m_parent;
        }
        return (BSTreeItr)parent; // Return the parent which is the previous in-order node
    }

}

BSTreeItr BSTreeItrRemove(BSTreeItr _it){
    if (_it == NULL) return NULL;
    BSTreeNode* nodeToRemove = (BSTreeNode*)_it; // do unencapsulation to get the node from the iterator
    BSTreeNode* parent = nodeToRemove->m_parent;
    BSTreeNode* replacement = NULL;

    if (nodeToRemove->m_left != NULL && nodeToRemove->m_right != NULL) {
        // Node has two children, find the in-order successor (smallest in the right subtree)
        replacement = nodeToRemove->m_right;
        while (replacement->m_left != NULL) {
            replacement = replacement->m_left;
        }
        // Replace node's data with replacement's data
        nodeToRemove->m_data = replacement->m_data;
        // Now we need to remove the replacement node
        parent = replacement->m_parent;
        if (replacement->m_right != NULL) {
            replacement->m_right->m_parent = parent;
        }
        if (parent->m_left == replacement) {
            parent->m_left = replacement->m_right;
        } else {
            parent->m_right = replacement->m_right;
        }
    } else {
        // Node has at most one child
        replacement = (nodeToRemove->m_left != NULL) ? nodeToRemove->m_left : nodeToRemove->m_right;
        if (replacement != NULL) {
            replacement->m_parent = parent;
        }
        if (parent == NULL) {
            // Removing the root node
            _tree->m_sentinel->m_left = replacement; // Update root
        } else if (parent->m_left == nodeToRemove) {
            parent->m_left = replacement;
        } else {
            parent->m_right = replacement;
        }
    }
    free(nodeToRemove); // Free the removed node
    return (BSTreeItr)replacement; // Return iterator to the next element after removal
}

void* BSTreeItrGet(BSTreeItr _it){
    BSTreeNode* node = (BSTreeNode*)_it;
    if (node == NULL) return NULL;
    return node->m_data;    
}

static BSTreeNode* ForEachRec(BSTreeNode* _node, BSTreeTraversalMode _mode, ActionFunction _action, void* _context) {
    BSTreeNode* result = NULL;

    // תנאי עצירה
    if (_node == NULL) {
        return NULL;
    }

    // first case: PRE_ORDER 
    if (_mode == TREE_TRAVERSAL_PREORDER) {
        if (_action(_node->m_data, _context) != 0) return _node; // אם הפעולה החזירה שונה מ-0, עוצרים
    }

    //go left (relevant for all cases)
    result = ForEachRec(_node->m_left, _mode, _action, _context);
    if (result != NULL) return result; // אם העצירה קרתה בצד שמאל, מגלגלים אותה למעלה

    // second case: IN_ORDER
    if (_mode == TREE_TRAVERSAL_INORDER) {
        if (_action(_node->m_data, _context) != 0) return _node;
    }

    //go right (relevant for all cases)
    result = ForEachRec(_node->m_right, _mode, _action, _context);
    if (result != NULL) return result;

    // third case: POST_ORDER 
    if (_mode == TREE_TRAVERSAL_POSTORDER) {
        if (_action(_node->m_data, _context) != 0) return _node;
    }

    return NULL; 
}

