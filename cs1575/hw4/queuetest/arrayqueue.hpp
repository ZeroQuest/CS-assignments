
template<typename T>
void ArrayQueue<T>::grow()
{
    int newMax = max()*2;

    T* tmp = new T[newMax];

    int count = 0;
    int index = m_front;
    while(count != size())
    {
        tmp[count] = m_data[index];

        index = (index +1) % max();

        count++;
    }
    m_back = count;
    m_front = 0;
    m_max = newMax;

    delete [] m_data;

    m_data = tmp;
}

template<typename T>
void ArrayQueue<T>::shrink()
{
    int newMax = max()/2;

    T* tmp = new T[newMax];

    int count = 0;
    int index = m_front;
    while(count != size())
    {
        tmp[count] = m_data[index];

        index = (index +1) % max();

        count++;
    }
    m_back = count;
    m_front = 0;
    m_max = newMax;

    delete [] m_data;

    m_data = tmp;
}

template<typename T>
ArrayQueue<T>::ArrayQueue()
{
    m_max = 4;
    m_size = 0;
    m_front = 0;
    m_back = 0;

    m_data = new T[m_max];
}

template<typename T>
ArrayQueue<T>::ArrayQueue(int s, const T& x)
{
    m_max = s;
    m_size = s;
    m_front = 0;

    m_data = new T[max()];

    for(int i = 0; i < s; i++)
    {
        m_data[i] = x;
    }
    m_back = m_size;
}

/*template<typename T>
ArrayQueue<T>::~AbstractQueue()
{

}
*/

template<typename T>
ArrayQueue<T>::~ArrayQueue()
{
    delete [] m_data;

    m_max = 0;
    m_size = 0;
    m_front = 0;
    m_back = 0;

    m_data = NULL;
}

template<typename T>
ArrayQueue<T>& ArrayQueue<T>::operator=(const ArrayQueue<T>& rhs)
{
    delete [] m_data;

    m_max = rhs.max();
    m_size = rhs.size();

    m_data = new T[max()];

    for(int i = 0; i < size(); i++)
    {
        m_data[i] = rhs.m_data[i];
    }
    m_front = rhs.m_front;
    m_back = rhs.m_back;

    return *this;
}

template<typename T>
ArrayQueue<T>::ArrayQueue(const ArrayQueue<T>& cpy)
{
    m_max = cpy.max();
    m_size = cpy.size();
    m_front = cpy.m_front;
    m_back = cpy.m_back;

    delete[] m_data;

    T* tmp = new T[max()];

    for(int i = 0; i < size(); i++)
    {
        tmp[i] = cpy.m_data[i];
    }

    //delete [] m_data;

    m_data = tmp;
}

template<typename T>
int ArrayQueue<T>::size()const
{
    return m_size;
}

template<typename T>
int ArrayQueue<T>::max() const
{
    return m_max;
}

template<typename T>
bool ArrayQueue<T>::isEmpty() const
{
    if(size() == 0)
    {
        return true;
    }
    return false;
}

template<typename T>
const T& ArrayQueue<T>::front() const throw (Oops)
{
    //std::cout<<m_data[0]<<m_data[m_front]<<endl;
    return m_data[m_front];
}

template<typename T>
const T& ArrayQueue<T>::back() const throw (Oops)
{
    return m_data[m_back];
}

template<typename T>
T& ArrayQueue<T>::operator[](int i)
{
	if(i <= size())
    {
        return m_data[i];
    }
}

template<typename T>
const T& ArrayQueue<T>::operator[](int i) const
{
	if(i <= size())
    {
        T tmp = m_data[i];

        return tmp;
    }
}

template<typename T>
void ArrayQueue<T>::enqueue(const T& x)
{
    if(size() == max())
    {
        grow();
    }

    m_data[m_back] = x;
    m_back = (m_back + 1) % max();
    m_size++;
}

template<typename T>
void ArrayQueue<T>::dequeue()
{
    if(isEmpty() != true)
    {
        if(size() < max()/4)
        {
            shrink();
        }

        m_front = (m_front + 1) % max();
        m_size--;
    }
}

template<typename T>
void ArrayQueue<T>::clear()
{
    m_back = 0;
    m_front = 0;
    m_size = 0;
}

template<typename T>
std::ostream& operator<<(std::ostream& out, const ArrayQueue<T>& aqueue)
{
    int count = 0;
    int i = aqueue.m_front;

    out << "[";
    while( count < aqueue.size())
    {
        out << aqueue.m_data[i] << ", ";

        i = (i + 1)% aqueue.max();
        count++;
    }
    out <<"]";

    return out;
}

