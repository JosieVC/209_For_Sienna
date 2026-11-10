#pragma once
#include <iostream>
#include <string>
#include <stdexcept>
using namespace std;

template <class T> class Stack
{
public:
	//rule of three inclusions
	Stack();
	~Stack();
	Stack(const Stack &oldStack);
	//copy assignment operator was giving a few issues when declared below, so I just declared it here
	Stack& operator = (const Stack& other) {
		//if new stack is equal to old stack already, just return it
		if (this == &other) {
			return *this;
		}
		//otherwise delete the stack, copy over the old stack, return new copy
		delete[] stack; 
		stack = new T(*other.stack);
		return *this; 
	}

	//all methods to be defined below
	void push(T value);
	void resize(int newCapacity); 
	void print();
	T pop();
	T& top();
	bool isEmpty();
	int getSize(); 

private:
	//private elements to store sensitive stack data
	int topElement = 0;
	//set capacity to 1 to start as dictated in the homework writeup
	int capacity = 1; 
	T* stack = new T[capacity]; 
};

//using "inline" caused a lot of identification issues, so I found a way to define them like this instead
//hopefully changing that one keyword doesn't cause any issues, considering I am still manually defining everything
template <class T> Stack<T>::Stack() {
	//set topElement to 0 to start since we'll start at the beginning of the array
	topElement = 0; 
}

//custom destructor, deletes the stack
template <class T> Stack<T>::~Stack() {
	delete[] stack; 
}

//custom copy constructor, copies over the stack
template <class T> Stack<T>::Stack(const Stack &oldStack) {
	//make sure to set capacity & top index to match the old stack
	capacity = oldStack.capacity;
	topElement = oldStack.topElement; 
	//make a new stack the same size as the old stack
	stack = new T[capacity]; 

	//loop through & copy all elements
	for (int i = 0; i <= topElement; i++) {
		stack[i] = oldStack.stack[i];
	}
}

//not mentioned in the writeup specifically, but I wanted to include a resize method for readability & ease of debugging
template <class T> void Stack<T>::resize(int newCapacity) {
	//start by making a new stack of the new capacity
	T* newData = new T[newCapacity];

	//copy over all data from old stack to new stack
	for (int i = 0; i < topElement; i++) {
		newData[i] = stack[i]; 
	} 
	//set stack equal to the new stack
	stack = newData;
	//increase to the new capacity
	capacity = newCapacity; 
}

//prints out all the stack elements
template <class T> void Stack<T>::print() {
	for (int i = 0; i < topElement; i++) {
		cout << stack[i] << " ";
	}
}

//adds a new element to the stack
template <class T> void Stack<T>::push(T value) {
	//if we're at the end of the current stack size, resize it
	if (topElement == capacity) {
		//always double the capacity when resizing
		resize(capacity * 2); 
	}
	//set the top index of the stack to the new value
	stack[topElement] = value; 
	//increase the top index for future use
	topElement++; 
}

//removes an element from the stack
template <class T> T Stack<T>::pop() {
	//store the popped element to return it
	T poppedElement = stack[topElement];

	//decrease the top index of the stack to remove the element
	topElement--;

	//return the popped element
	return poppedElement; 
}

//returns a reference to the top element of the stack
template <class T> T& Stack<T>::top() {

	//first make sure stack isn't empty; if it is, throw an exception
	if (isEmpty()) {
		throw std::out_of_range("Stack is empty!"); 
	}

	//make a variable to store the top element of the stack (needs to be topElement-1 here since topElement stores the *next* index
	//to push things into, so to modify an already pushed value, must be decremented)
	T& topData = stack[(topElement-1)];

	//returns this element so it can be used 
	return topData; 
}

//checks if the stack is empty
template <class T> bool Stack<T>::isEmpty() {
	//topElement is initialized at 0, so if it's still 0, the stack is empty - popping all elements also results in a top of 0
	if (topElement == 0) {
		return 1;
	}
	else {
		return 0; 
	}
}

//gets the number of elements in the stack
template <class T> int Stack<T>::getSize() {
	//with the way I've implemented topElement, this will give the total number of things pushed to the stack, hence the total number of things in it
	return topElement; 
}