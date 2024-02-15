

std::ostream& operator<<(std::ostream& out, const customer& c)
{
    out<<c.m_name;


    return out;
}

customer::customer(string name, int numWant, int numAte)
{
    m_name = name;
    m_numWant = numWant;
    m_numAte = numAte;
}

customer::~customer()
{
    m_name = "";
    m_numWant = 0;
    m_numAte = 0;
}

customer customer::operator=(const customer& rhs)
{
    m_name = rhs.getName();
    m_numWant = rhs.getNumWant();
    m_numAte = rhs.getNumAte();

    //std::cout<<m_name<<" "<<m_numWant<<" "<<m_numAte;

    return *this;
}

customer::customer(const customer& cpy)
{
    m_name = cpy.getName();
    m_numWant = cpy.getNumWant();
    m_numAte = cpy.getNumAte();


}

string customer::getName() const
{
    return m_name;
}

int customer::getNumWant() const
{
    return m_numWant;
}

int customer::getNumAte() const
{
    return m_numAte;
}
