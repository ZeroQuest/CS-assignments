#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <iostream>
#include <string>

using namespace std;


class customer;

std::ostream& operator<<(std::ostream& out, const customer& c);

class customer
{
    private:
        string m_name;
        int m_numWant;
        int m_numAte;

    public:
        customer() {m_name = ""; m_numWant = 0; m_numAte = 0;};

        customer(string name, int numWant, int numAte);

        ~customer();

        customer operator=(const customer& rhs);

        customer(const customer& cpy);

        string getName() const;
        int getNumWant() const;
        int getNumAte() const;
        void setName(string name) {m_name = name;};
        void setNumWant(int want) {m_numWant = want;};
        void setNumAte(int ate) {m_numAte = ate;};

        friend std::ostream& operator <<(std::ostream& out, const customer& c);

};

#include "customer.hpp"

#endif // CUSTOMER_H
