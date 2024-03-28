#include <iostream>
#include <string>
#include "arrayqueue.h"

using namespace std;

int main()
{
    ArrayQueue<int> a;

    cout <<a<<endl;

    a.enqueue(5);

    cout<<a<<endl;

    a.dequeue();

    //cout<<a.front()<<endl;

    a.enqueue(2);
    a.dequeue();
    a.enqueue(3);
    a.dequeue();

    a.clear();

    cout<<a<<endl;

    for(int i = 0; i < 7; i++)
    {
        a.enqueue(i);
    }

    cout<<a<<endl;

    for(int i = 0; i < 3; i++)
        a.dequeue();

    cout<<a<<endl;

    a.clear();

    /*

    for(int i = 0; i < 60; i++)
        a.enqueue(i);

    cout<<a<<endl;
    cout<<a.max()<<endl;

    for(int i = 0; i < 55; i++)
        a.dequeue();

    cout<<a<<endl;
    cout<<a.size()<<" "<<a.max()<<endl;


    */
    //a.enqueue(34);

    //cout<<a<<endl;

    a.clear();

    cout<<a<<endl;

    ArrayQueue<string> b(3, "hi");

    cout<<b<<endl;

    ArrayQueue<int> c(4, 9);

    cout<<c<<endl;

    a = c;

    cout<<a<<endl;

    a.clear();

    ArrayQueue<int> d(c);

    cout<<d<<endl;

    int e = a.front();

    cout<<d.front();

    return 0;
}
