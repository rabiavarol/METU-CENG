#ifndef TWOPHASEBST_H
#define TWOPHASEBST_H

#include <iostream>
#include <string>
#include <stack>
#include <list>
// do not add any other library.
// modify parts as directed by assignment text and comments here.

template<class T>
class TwoPhaseBST {
private: //do not change
    struct SecondaryNode {
        std::string key;
        T data;
        SecondaryNode *left;
        SecondaryNode *right;

        SecondaryNode(const std::string &k, const T &d, SecondaryNode *l, SecondaryNode *r);
    };

    struct PrimaryNode {
        std::string key;
        PrimaryNode *left;
        PrimaryNode *right;
        SecondaryNode *rootSecondaryNode;

        PrimaryNode(const std::string &k, PrimaryNode *l, PrimaryNode *r, SecondaryNode *rsn);
    };

public: // do not change.

    TwoPhaseBST();

    ~TwoPhaseBST();

    TwoPhaseBST &insert(const std::string &primaryKey, const std::string &secondaryKey, const T &data);

    TwoPhaseBST &remove(const std::string &primaryKey, const std::string &secondaryKey);

    TwoPhaseBST &print(const std::string &primaryKey = "", const std::string &secondaryKey = "");

    T *find(const std::string &primaryKey, const std::string &secondaryKey);

private: // you may add your own utility member functions here.

    void destructNode(PrimaryNode *&root);

    void destructNode(SecondaryNode *&root);

    void insertToSecondaryTree(SecondaryNode *&root, const std::string &key, const T &data);

    void insertToPrimaryTree(PrimaryNode *&root, const std::string &primaryKey,
                             const std::string &secondaryKey, const T &data);

    void printTree(PrimaryNode *firstNode, const std::string &primaryKey, const std::string &secondaryKey);

    void printSecondaryTree(SecondaryNode *secondaryNode);

    void printSubSecondaryNode(SecondaryNode *secondaryNode, const std::string &secondaryKey);


    T *findSecondaryNode(SecondaryNode *secondaryRoot, const std::string &secondaryKey);

    T *findPrimaryNode(PrimaryNode *primaryRoot, const std::string &primaryKey, const std::string &secondaryKey);


    SecondaryNode *findSubSecondaryNode(SecondaryNode *secondaryNode, const std::string &secondaryKey) {

        if (secondaryNode == NULL) return NULL;

        if (secondaryKey < secondaryNode->key) {

            return findSubSecondaryNode(secondaryNode->left, secondaryKey);

        } else if (secondaryKey > secondaryNode->key) {

            return findSubSecondaryNode(secondaryNode->right, secondaryKey);

        } else if (secondaryKey == secondaryNode->key) {

            return secondaryNode;
        }

    }

    PrimaryNode *findSubPrimaryNode(PrimaryNode *primaryNode, const std::string &primaryKey) {

        if (primaryNode == NULL) return NULL;

        if (primaryKey < primaryNode->key) {

            return findSubPrimaryNode(primaryNode->left, primaryKey);

        } else if (primaryKey > primaryNode->key) {

            return findSubPrimaryNode(primaryNode->right, primaryKey);

        } else if (primaryKey == primaryNode->key) {

            return primaryNode;
        }

    }

    SecondaryNode *findPrevOfSecondaryNode(SecondaryNode *root, const std::string &secondaryKey) {

        if (root == NULL) return NULL;

        if (secondaryKey < root->key) {
            if (root->left && root->left->key == secondaryKey) {
                return root;
            }
            return findPrevOfSecondaryNode(root->left, secondaryKey);
        } else if (secondaryKey > root->key) {
            if (root->right->key == secondaryKey) {
                return root;
            }
            return findPrevOfSecondaryNode(root->right, secondaryKey);
        }

        return NULL;

    }


    SecondaryNode *findMinOfSecondaryTree(SecondaryNode *secondaryNode) {

        if (secondaryNode == NULL) return NULL;

        if (secondaryNode->left == NULL) return secondaryNode;

        return findMinOfSecondaryTree(secondaryNode->left);
    }

private: // do not change.
    PrimaryNode *root; //designated root.

    // do not provide an implementation. TwoPhaseBST's are not copiable.
    TwoPhaseBST(
            const TwoPhaseBST &);

    const TwoPhaseBST &operator=(const TwoPhaseBST &);
};

template<class T>
TwoPhaseBST<T>::SecondaryNode::SecondaryNode(const std::string &k, const T &d, SecondaryNode *l, SecondaryNode *r)
        : key(k), data(d), left(l), right(r) {}

template<class T>
TwoPhaseBST<T>::PrimaryNode::PrimaryNode(const std::string &k, PrimaryNode *l, PrimaryNode *r, SecondaryNode *rsn)
        : key(k), left(l), right(r), rootSecondaryNode(rsn) {}

template<class T>
TwoPhaseBST<T>::TwoPhaseBST() : root(NULL) {}

template<class T>
TwoPhaseBST<T>::~TwoPhaseBST() {
    destructNode(root);
}

template<class T>
TwoPhaseBST<T> &
TwoPhaseBST<T>::insert(const std::string &primaryKey, const std::string &secondaryKey, const T &data) {

    if (root == NULL) {

        SecondaryNode *newSecondary = new SecondaryNode(secondaryKey, data, NULL, NULL);
        root = new PrimaryNode(primaryKey, NULL, NULL, newSecondary);

    } else {

        insertToPrimaryTree(root, primaryKey, secondaryKey, data);

    }
    return *this;
}

template<class T>
TwoPhaseBST<T> &
TwoPhaseBST<T>::remove(const std::string &primaryKey, const std::string &secondaryKey) {

    PrimaryNode *firstPrimaryNode = findSubPrimaryNode(root, primaryKey);

    if (firstPrimaryNode != NULL) {

        SecondaryNode *firstSecondaryNode = findSubSecondaryNode(firstPrimaryNode->rootSecondaryNode, secondaryKey);

        if (firstSecondaryNode == NULL) return *this;

        if (firstPrimaryNode->rootSecondaryNode == firstSecondaryNode) {

            if (firstSecondaryNode->left && firstSecondaryNode->right) { // it has both of the right and the left
                //subSecondaryNodes.

                SecondaryNode *minSecondaryNode = findMinOfSecondaryTree(firstSecondaryNode->right);

                if (minSecondaryNode == firstSecondaryNode->right) {
                    minSecondaryNode->left = firstSecondaryNode->left;
                    firstPrimaryNode->rootSecondaryNode = minSecondaryNode;
                    delete firstSecondaryNode;
                    firstSecondaryNode = NULL;
                } else {
                    SecondaryNode *minPrev = findPrevOfSecondaryNode(firstPrimaryNode->rootSecondaryNode,
                                                                     minSecondaryNode->key);

                    minPrev->left = minSecondaryNode->right;
                    minSecondaryNode->right = firstSecondaryNode->right;
                    minSecondaryNode->left = firstSecondaryNode->left;

                    firstPrimaryNode->rootSecondaryNode = minSecondaryNode;

                    delete firstSecondaryNode;
                    firstSecondaryNode = NULL;

                }

            } else if (!firstSecondaryNode->left && !firstSecondaryNode->right) { // leaf case

                firstPrimaryNode->rootSecondaryNode = NULL;
                delete firstSecondaryNode;
                firstSecondaryNode = NULL;

            } else {
                if (firstSecondaryNode->left) {
                    firstPrimaryNode->rootSecondaryNode = firstSecondaryNode->left;
                } else {
                    firstPrimaryNode->rootSecondaryNode = firstSecondaryNode->right;

                }
                delete firstSecondaryNode;
                firstSecondaryNode = NULL;
            }

        } else {

            SecondaryNode *prev = findPrevOfSecondaryNode(firstPrimaryNode->rootSecondaryNode, firstSecondaryNode->key);

            if (firstSecondaryNode->left && firstSecondaryNode->right) { // it has both of the right and the left
                //subSecondaryNodes.

                SecondaryNode *minSecondaryNode = findMinOfSecondaryTree(firstSecondaryNode->right);

                if (minSecondaryNode == firstSecondaryNode->right) {

                    minSecondaryNode->left = firstSecondaryNode->left;
                    if (prev->left == firstSecondaryNode) {
                        prev->left = minSecondaryNode;
                    } else {
                        prev->right = minSecondaryNode;
                    }

                    delete firstSecondaryNode;

                    firstSecondaryNode = NULL;

                } else {

                    SecondaryNode *minPrev = findPrevOfSecondaryNode(firstPrimaryNode->rootSecondaryNode,
                                                                     minSecondaryNode->key);

                    minPrev->left = minSecondaryNode->right;
                    minSecondaryNode->right = firstSecondaryNode->right;
                    minSecondaryNode->left = firstSecondaryNode->left;

                    if (prev->left == firstSecondaryNode) {
                        prev->left = minSecondaryNode;
                    } else {
                        prev->right = minSecondaryNode;
                    }

                    delete firstSecondaryNode;
                    firstSecondaryNode = NULL;

                }

            } else if (!firstSecondaryNode->left && !firstSecondaryNode->right) { // leaf case
                if (prev->left == firstSecondaryNode) {
                    prev->left = NULL;
                } else {
                    prev->right = NULL;
                }
                delete firstSecondaryNode;
                firstSecondaryNode = NULL;
            } else {
                if (prev->left == firstSecondaryNode) {
                    if (firstSecondaryNode->left) {
                        prev->left = firstSecondaryNode->left;
                    } else {
                        prev->left = firstSecondaryNode->right;
                    }
                } else {
                    if (firstSecondaryNode->left) {
                        prev->right = firstSecondaryNode->left;
                    } else {
                        prev->right = firstSecondaryNode->right;
                    }
                }
                delete firstSecondaryNode;
                firstSecondaryNode = NULL;
            }
        }

    }

    return *this;
}

template<class T>
TwoPhaseBST<T> &TwoPhaseBST<T>::print(const std::string &primaryKey, const std::string &secondaryKey) {

    if (primaryKey == "" && secondaryKey != "") {
        return *this;
    }

    std::cout << "{";
    printTree(root, primaryKey, secondaryKey);
    std::cout << "}" << std::endl;

    return *this;
}

template<class T>
T *TwoPhaseBST<T>::find(const std::string &primaryKey, const std::string &secondaryKey) {

    if (root == NULL) return NULL;

    if (root->key == primaryKey) {

        return findSecondaryNode(root->rootSecondaryNode, secondaryKey);

    } else if (primaryKey < root->key) {

        return findPrimaryNode(root->left, primaryKey, secondaryKey);
    } else if (primaryKey > root->key) {

        return findPrimaryNode(root->right, primaryKey, secondaryKey);
    }

}

template<class T>
T *TwoPhaseBST<T>::findSecondaryNode(TwoPhaseBST::SecondaryNode *secondaryRoot, const std::string &secondaryKey) {

    if (secondaryRoot == NULL) return NULL;

    else if (secondaryKey < secondaryRoot->key) {

        return findSecondaryNode(secondaryRoot->left, secondaryKey);

    } else if (secondaryKey > secondaryRoot->key) {

        return findSecondaryNode(secondaryRoot->right, secondaryKey);

    } else if (secondaryKey == secondaryRoot->key) {

        return &(secondaryRoot->data);
    }

}

template<class T>
T *TwoPhaseBST<T>::findPrimaryNode(TwoPhaseBST::PrimaryNode *primaryRoot, const std::string &primaryKey,
                                   const std::string &secondaryKey) {

    if (primaryRoot == NULL) return NULL;

    else if (primaryKey < primaryRoot->key) {

        return findPrimaryNode(primaryRoot->left, primaryKey, secondaryKey);
    } else if (primaryKey > primaryRoot->key) {

        return findPrimaryNode(primaryRoot->right, primaryKey, secondaryKey);
    } else if (primaryKey == primaryRoot->key) {

        return findSecondaryNode(primaryRoot->rootSecondaryNode, secondaryKey);
    }


}

template<class T>
void TwoPhaseBST<T>::destructNode(TwoPhaseBST::PrimaryNode *&root) {
    if (root == NULL)
        return;

    destructNode(root->left);
    destructNode(root->right);

    destructNode(root->rootSecondaryNode);

    delete root;

    root = NULL;
}

template<class T>
void TwoPhaseBST<T>::destructNode(TwoPhaseBST::SecondaryNode *&root) {
    if (root == NULL)
        return;

    destructNode(root->left);
    destructNode(root->right);

    delete root;

    root = NULL;
}

template<class T>
void TwoPhaseBST<T>::insertToSecondaryTree(TwoPhaseBST::SecondaryNode *&secondaryRoot, const std::string &key,
                                           const T &data) {

    if (secondaryRoot == NULL) {

        secondaryRoot = new SecondaryNode(key, data, NULL, NULL);

    } else {

        if (key < secondaryRoot->key)

            insertToSecondaryTree(secondaryRoot->left, key, data);

        else

            insertToSecondaryTree(secondaryRoot->right, key, data);

    }

}

template<class T>
void TwoPhaseBST<T>::insertToPrimaryTree(TwoPhaseBST::PrimaryNode *&root, const std::string &primaryKey,
                                         const std::string &secondaryKey, const T &data) {

    if (root == NULL) return;


    if (primaryKey < root->key) {

        if (root->left != NULL) {
            insertToPrimaryTree(root->left, primaryKey, secondaryKey, data);
        } else {
            SecondaryNode *newSecondary = new SecondaryNode(secondaryKey, data, NULL, NULL);
            root->left = new PrimaryNode(primaryKey, NULL, NULL, newSecondary);
        }
    } else if (primaryKey > root->key) {

        if (root->right != NULL) {
            insertToPrimaryTree(root->right, primaryKey, secondaryKey, data);
        } else {
            SecondaryNode *newSecondary = new SecondaryNode(secondaryKey, data, NULL, NULL);
            root->right = new PrimaryNode(primaryKey, NULL, NULL, newSecondary);
        }
    } else if (primaryKey == root->key) {

        insertToSecondaryTree(root->rootSecondaryNode, secondaryKey, data);
    }

}

template<class T>
void TwoPhaseBST<T>::printTree(TwoPhaseBST::PrimaryNode *firstNode, const std::string &primaryKey,
                               const std::string &secondaryKey) {

    if (firstNode == NULL)
        return;

    if (firstNode->key == primaryKey) {

        if (secondaryKey == "") { // primary key is given but secondary key is not given.

            std::cout << "\"" << firstNode->key << "\"" << " : " << "{";
            printSecondaryTree(firstNode->rootSecondaryNode);
            std::cout << "}";

        } else {
            if (findSecondaryNode(firstNode->rootSecondaryNode, secondaryKey) != NULL) {
                std::cout << "\"" << firstNode->key << "\"" << " : " << "{";
                printSubSecondaryNode(firstNode->rootSecondaryNode, secondaryKey);
                std::cout << "}";
            }
        }
    } else if (primaryKey == "" && secondaryKey == "") {

        if (firstNode->left) {
            printTree(firstNode->left, primaryKey, secondaryKey);
            std::cout << ", ";
        }

        std::cout << "\"" << firstNode->key << "\"" << " : " << "{";
        printSecondaryTree(firstNode->rootSecondaryNode);
        std::cout << "}"; //print with its secondaryTree

        if (firstNode->right) {
            std::cout << ", ";
            printTree(firstNode->right, primaryKey, secondaryKey);
        }

    } else {

        printTree(firstNode->left, primaryKey, secondaryKey);
        printTree(firstNode->right, primaryKey, secondaryKey);

    }

}

template<class T>
void TwoPhaseBST<T>::printSecondaryTree(TwoPhaseBST::SecondaryNode *secondaryNode) {

    if (secondaryNode == NULL)
        return;

    if (secondaryNode->left != NULL) {  // if it has left node, print left first until lefts' are over.

        printSecondaryTree(secondaryNode->left);
        std::cout << ", ";

    }

    std::cout << "\"" << secondaryNode->key << "\"" << " : " << "\"" << secondaryNode->data << "\""; // print itself

    if (secondaryNode->right != NULL) { //lastly, print right sides if any.

        std::cout << ", ";
        printSecondaryTree(secondaryNode->right);
    }

}

template<class T>
void TwoPhaseBST<T>::printSubSecondaryNode(TwoPhaseBST::SecondaryNode *secondaryNode, const std::string &secondaryKey) {

    if (secondaryNode == NULL)
        return;

    if (secondaryKey < secondaryNode->key) {

        printSubSecondaryNode(secondaryNode->left, secondaryKey);

    } else if (secondaryKey > secondaryNode->key) {

        printSubSecondaryNode(secondaryNode->right, secondaryKey);

    } else if (secondaryKey == secondaryNode->key) {

        std::cout << "\"" << secondaryNode->key << "\"" << " : " << "\"" << secondaryNode->data << "\"";

    }

}

#endif //TWOPHASEBST_H
