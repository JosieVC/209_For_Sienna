// HW4.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Stack.h"
#include <string>
using namespace std;

int main()
{

    //all of my tests & declarations - I should hope the cout statements provide adequate context so I don't need to comment over everything

    Stack<int> integerStack; 
    Stack<char> charStack; 
    Stack<double> doubleStack; 

    integerStack.push(42);
    integerStack.push(7);
    integerStack.push(999);

    charStack.push('A');
    charStack.push('B');
    charStack.push('C'); 
   
    doubleStack.push(2.55);
    doubleStack.push(12.8);
    doubleStack.push(333.33); 

    cout << "Printing out integer stack: " << endl; 
    integerStack.print();
    cout << endl; 
    cout << "Printing out string stack: " << endl; 
    charStack.print();
    cout << endl;
    cout << "Printing out double stack: " << endl; 
    doubleStack.print();
    cout << endl;

    cout << "Testing getSize function. Size of integer stack: " << endl;
    cout << integerStack.getSize();
    cout << endl;

    integerStack.pop(); 
    charStack.pop();
    doubleStack.pop();
    doubleStack.pop();

    cout << "Printing out contents of popped stacks: " << endl;
    cout << "Integer stack: " << endl;
    integerStack.print();
    cout << endl;
    cout << "String stack: " << endl;
    charStack.print();
    cout << endl;
    cout << "Double stack: " << endl;
    doubleStack.print();
    cout << endl; 

    cout << "Testing getSize function. Size of integer stack: " << endl;
    cout << integerStack.getSize();
    cout << endl; 

    integerStack.top() = 4; 

    cout << "New Integer stack: " << endl;
    integerStack.print();
    cout << endl; 

    Stack<int> copyStack(integerStack);
    cout << "Copied integer stack: " << endl;
    copyStack.print(); 
    cout << endl; 
    copyStack.pop();
    cout << "Copied stack with one pop: " << endl; 
    copyStack.print(); 
    copyStack = integerStack;
    cout << endl;
    cout << "Copied stack set to original value: " << endl; 
    copyStack.print(); 
}

