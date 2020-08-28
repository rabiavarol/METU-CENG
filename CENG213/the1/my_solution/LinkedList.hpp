#ifndef LINKEDLIST_HPP
#define	LINKEDLIST_HPP

#include <iostream>
#include "Node.hpp"

using namespace std;

/*....DO NOT EDIT BELOW....*/
template <class T>
class LinkedList {
private:
    Node<T>* head;
    int length;
public:

    LinkedList();
    LinkedList(const LinkedList<T>& ll);
    LinkedList<T>& operator=(const LinkedList<T>& ll);
    ~LinkedList();


    Node<T>* getHead() const;
    Node<T>* first() const;
    Node<T>* findPrev(const T& data) const;
    Node<T>* findNode(const T& data) const;
    void insertNode(Node<T>* prev, const T& data);
    void deleteNode(Node<T>* prevNode);
    void clear();
    size_t getLength() const;
    void print() const;
    void swap(int index1, int index2);
};

template <class T>
void LinkedList<T>::print() const {
    const Node<T>* node = first();
    while (node) {
        std::cout << node->getData();
        node = node->getNext();
    }
    cout << std::endl;
}

/*....DO NOT EDIT ABOVE....*/

/* TO-DO: method implementations below */


template <class T>
LinkedList<T>::LinkedList(){
  head=new Node<T>();
  length=0;
}

template <class T>
LinkedList<T>::LinkedList(const LinkedList<T>& ll){
  head=new Node<T>();
  length=0;
  *this=ll;
}

template <class T>
LinkedList<T>& LinkedList<T>::operator=(const LinkedList<T>& ll){
  if(this != &ll) {
      length=0;
      clear();

      Node<T>* current=ll.first();
      Node<T>* currentBack=getHead();

      while(current) {
        insertNode(currentBack,current->getData());
        current=current->getNext();
        currentBack=currentBack->getNext();
      }
  }
  return *this;
}

template <class T>
LinkedList<T>::~LinkedList(){
  clear();
  delete head;
}
template <class T>
Node<T>* LinkedList<T>::getHead() const{
  return head;
}
template <class T>
Node<T>* LinkedList<T>:: first() const {
  return head->getNext();
}

template <class T>
Node<T>* LinkedList<T>::findPrev(const T& data) const{
  Node<T>* prev=getHead();

  while(prev->getNext()){
    if(prev->getNext()->getData()==data){
      return prev;
    }
    prev=prev->getNext();
  }

  return NULL;

}
template <class T>
Node<T>* LinkedList<T>::findNode(const T& data) const{
  Node<T>* current = first();

  while(current){
    if(current->getData()==data){
      return current;
    }
    current=current->getNext();
  }

  return NULL;

}

template <class T>
void LinkedList<T>::insertNode(Node<T>* prev, const T& data){

  if(prev != NULL) {

    Node<T>* newNode=new Node<T>(data);
    newNode->setNext(prev->getNext());
    prev->setNext(newNode);
    length++; // new node is inserted

  }

}

template <class T>
void LinkedList<T>::deleteNode(Node<T>* prevNode){
  if(prevNode != NULL) {

    Node<T>* tmp=prevNode->getNext();
    if (tmp == NULL) {
      return;
    }
    prevNode->setNext(tmp->getNext());
    delete tmp;
    length--; // a node is deleted from the list.

  }
}

template <class T>
void LinkedList<T>::clear(){
  while (getLength() > 0){
    deleteNode(head);
  }
}
template <class T>
size_t LinkedList<T>::getLength() const {
  return (size_t)length;
}
template <class T>
void LinkedList<T>::swap(int index1, int index2){
  Node<T>* node1=first();
  Node<T>* node2=first();

  for(int i = 0; i < index1; i++){
    node1=node1->getNext();
  }

  for(int i=0; i<index2;i++){
    node2=node2->getNext();
  }

  T temp=node1->getData();
  *(node1->getDataPtr()) = node2->getData();
  *(node2->getDataPtr()) = temp;

}

/* end of your implementations*/
#endif
