#include <iostream>
#include <string>
#include "arrayqueue.h"
#include "customer.h"
#include "randomengine.h"

using namespace std;

int eatPoppler(randomEngine e, customer& c);

void updateCusts(int people, customer* scoreboard, customer& c);

customer findGreatest(int people, customer* scoreboard, customer& greatest);

customer findLeast(int people, customer* scoreboard, customer& least);

int main()
{
    //Variable Declarations
    customer tmp;
    customer greatest;
    customer least;

    randomEngine engine;

    ArrayQueue<customer> popLine;

    string tmpName;
    int tmpWant;
    int people;
    int totalEaten = 0;

    Oops error("Error: Empty queue");

    //End variable declarations

    //Start input reading
    cin>>people;

    customer carray[people];
    customer* scoreboard = carray;

    for(int i = 0; i < people; i++)
    {
        cin>>tmpName>>tmpWant;

        carray[i].setName(tmpName);
        carray[i].setNumWant(tmpWant);
        carray[i].setNumAte(0);

        popLine.enqueue(carray[i]);
    } //end input reading

    //Main Loop
    while(popLine.isEmpty() != true)
    {
        //Temporary variable to hold current customer values
        try
        {
            customer tmp = popLine.front();
        }
        catch(Oops)
        {
            cout<<error.getMsg();
        }

        customer tmp = popLine.front();


        //Customer eats their popplers, and updates the total counter
        totalEaten += eatPoppler(engine, tmp);

        //Updates the customer array to hold values for scoreboard
        updateCusts(people, scoreboard, tmp);

        //The customer leaves the line
        popLine.dequeue();

        //If the customer wants more popplers, they go to the back of the line
        if(tmp.getNumWant() > 0)
        {
            popLine.enqueue(tmp);
        }

        //cout<<"\t"<<popLine<<endl;

    } //end Main Loop

    //Finds the customer that ate the most popplers
    greatest = findGreatest(people, scoreboard, greatest);

    //Sets least's value to the value of the greatest customer
    //Note: This avoids having to do complex comparisons to assign a least value
    least = greatest;

    //Finds the customer that ate the least popplers
    least = findLeast(people, scoreboard, least);

    cout<<endl;
    cout<<"A total of "<<totalEaten<<" popplers were eaten."<<endl;
    cout<<greatest<<" ate the most popplers: "<<greatest.getNumAte()<<endl;
    cout<<least<<" ate the fewest popplers: "<<least.getNumAte()<<endl;

    return 0;
}


int eatPoppler(randomEngine e, customer& c)
{
    const int POPPLERS = 5;
    int eaten = 0;
    int roll = 0;
    int numSix = 0;

    while(c.getNumWant() > eaten)
    {
        eaten += POPPLERS;

        for(int i = 0; i < POPPLERS; i++)
        {
            roll = e.rollD(6);
            if(roll == 6)
                numSix++;
        }
    }

    c.setNumAte(eaten);
    c.setNumWant(numSix);

    cout<<c.getName()<<" eats "<<c.getNumAte()<<" popplers. ";

    if(c.getNumWant() == 1)
    {
        cout<<c.getName()<<" wants one more poppler!"<<endl;
    }
    else if(c.getNumWant() > 1)
    {
        cout<<c.getName()<<" wants "<<c.getNumWant()<<" more popplers!"<<endl;
    }
    else
    {
        cout<<c.getName()<<" is satisfied after eating "<<c.getNumAte()<<" popplers."<<endl;
    }

    return eaten;
}

void updateCusts(int people, customer* scoreboard, customer& c)
{
    for(int i = 0; i < people; i++)
    {
        if((scoreboard + i)->getName() == c.getName())
        {
            (scoreboard + i)->setNumAte((scoreboard + i)->getNumAte() + c.getNumAte());
        }
    }
}

customer findGreatest(int people, customer* scoreboard, customer& greatest)
{
    for(int i = 0; i < people; i++)
    {
       if(greatest.getNumAte() < (scoreboard + i)->getNumAte())
        {
            greatest.setName((scoreboard + i)->getName());
            greatest.setNumAte((scoreboard + i)->getNumAte());
        }
    }

    return greatest;
}

customer findLeast(int people, customer* scoreboard, customer& least)
{
    for(int i = 0; i < people; i++)
    {
       if(least.getNumAte() > (scoreboard + i)->getNumAte())
        {
            least.setName((scoreboard + i)->getName());
            least.setNumAte((scoreboard + i)->getNumAte());
        }
    }

    return least;
}
