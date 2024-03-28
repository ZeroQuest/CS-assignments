
template <typename T>
inline void ArrayList<T>::grow()
{
	//std::cout << "Before mult: " << m_max;

	m_max = m_max * 2;

	//std::cout << "After mult: " << m_max;

	T* tmp = new T[max()];

	for (int i = 0; i < size(); i++)
	{
		tmp[i] = m_data[i];
	}

	delete[] m_data;

	m_data = tmp;

	//delete[] tmp;
}

template<typename T>
inline void ArrayList<T>::shrink()
{
	m_max = m_max / 2;

	T* tmp = new T[max()];

	for (int i = 0; i < size(); i++)
	{
		tmp[i] = m_data[i];
	}

	delete[] m_data;

	m_data = tmp;

	//delete[] tmp;
}

template<typename T>
inline ArrayList<T>::ArrayList(int s, const T& x)
{
	//m_errobj = NULL;
	m_max = s;
	m_size = s;

	//m_data = new T[m_max];

	for (int i = 0; i < m_size; i++)
	{
		m_data[i] = x;
	}
}

template<typename T>
inline ArrayList<T>::~ArrayList()
{
	delete[] m_data;

	//m_errobj = NULL;
	m_max = 0;
	m_size = 0;

	T* m_data = NULL;
}

template<typename T>
inline ArrayList<T>& ArrayList<T>::operator=(const ArrayList<T>& rhs)
{

	//delete[] m_data;

	m_max = rhs.max();
	m_size = rhs.size();


	std::cout << std::endl << "size "<<m_size;
	std::cout << std::endl << "rhs.size " << rhs.size();
	std::cout << std::endl << "max " << m_max;
	std::cout << std::endl << "rhs.max: "<< rhs.max();
	
	//delete [] m_data;
	//m_data = new T[m_max];

	T* tmp = new T[m_max];

	//std::cout << std::endl;

	for (int i = 0; i < m_size; i++)
	{
		tmp[i] = rhs.m_data[i];

		std::cout << std::endl << "tmp[i]: " << tmp[i];
		std::cout << "rhs.m_data[i] " << rhs.m_data[i] << std::endl;
	}

	//delete [] m_data;
	m_data = tmp;

	return *this;
}

template<typename T>
inline ArrayList<T>::ArrayList(const ArrayList<T>& cpy)
{
	T* tmp;

	//m_errobj = NULL;
	m_max = cpy.max();
	m_size = cpy.size();

	tmp = new T[m_max];

	for (int i = 0; i < m_size; i++)
	{
		tmp[i] = cpy.m_data[i];
	}

	//delete[] m_data;

	m_data = tmp;

	//delete[] tmp;
}

template<typename T>
inline int ArrayList<T>::size() const
{
	return m_size;
}

template<typename T>
inline const T& ArrayList<T>::first() const
{
	// TODO: insert return statement here
	if (size() > 0)
	{
		return m_data[0];
	}
	else
	{
	    std::cout<<"!!! ERROR : PANIC in ARRAYLIST !!!"<< ".first() (list is empty) \n";

		return m_errobj;
	}
}

template<typename T>
inline T& ArrayList<T>::operator[](int i)
{
	m_errobj = -1;

	if (i <= size())
	{
		return m_data[i];
	}
	else
	{
        std::cout<<"!!! ERROR : PANIC in ARRAYLIST !!!"<< ".[] (index out of bounds) \n";
		return m_errobj;
	}
}

template<typename T>
inline const T& ArrayList<T>::operator[](int i) const
{

	if (i <= size())
	{
		T tmp = m_data[i];

		return tmp;
	}
	else
	{
		return m_errobj;
	}
}

template<typename T>
inline int ArrayList<T>::search(const T& x) const
{


	for (int i = 0; i < size(); i++)
	{
		if (m_data[i] == x)
		{
			return i;
		}
	}

	return -1;
}

template<typename T>
inline void ArrayList<T>::clear()
{
	delete[] m_data;

	m_data = NULL;

	//m_errobj = NULL;
	m_max = 4;
	m_size = 0;

	m_data = new T[m_max];
}

template<typename T>
inline void ArrayList<T>::insert_back(const T& x)
{
	const int GROW_CHECK = size();

	if (GROW_CHECK == max())
	{
		//std::cout << "GROW()";
		grow();
	}

	m_data[m_size] = x;

	m_size = m_size + 1;
}

template<typename T>
inline void ArrayList<T>::insert(const T& x, int i)
{
	const int GROW_CHECK = size();

	if (0 <= i && i <= size())
	{
	   if (GROW_CHECK == max())
	   {
		grow();
	   }

	   for (int k = size(); k > i; k--)
	   {
		m_data[k] = m_data[k - 1];
	   }
	   m_data[i] = x;
	   m_size++;
	}
	else
        {
             std::cout<<"!!! ERROR : PANIC in ARRAYLIST !!!"<< ".insert() (index out of bounds) \n";
	}
 } 

template<typename T>
inline void ArrayList<T>::remove(int i)
{
	const int ZERO_CHECK = 0;

	if (!(size() == ZERO_CHECK) )
	{
		if (size() < (max() / 4))
		{
			shrink();
		}
		if (0 <= i && i <= size())
		{
			for (int k = i + 1; k < size(); k++)
			{
				m_data[k - 1] = m_data[k];
			}
			m_size--;
		}
		else
        	{
            		std::cout<<"!!! ERROR : PANIC in ARRAYLIST !!!"<< ".remove() (index out of bounds) \n";
        	}
	}
}

template<typename T>
inline void ArrayList<T>::swap(int i, int k)
{
	if (0 <= i && i <= size() && 0 <= k && k <= size())
	{
		//std::cout << std::endl << "SWAP()";

		T tmp = m_data[i];
		m_data[i] = m_data[k];
		m_data[k] = tmp;
	}
	else
        {
        	std::cout<<"!!! ERROR : PANIC in ARRAYLIST !!!"<< ".swap() (index out of bounds) \n";
    	}
}

template<typename T>
inline void ArrayList<T>::purge()
{
    for(int i = 0; i < size(); i++)
    {
        for(int k = 0; k < size(); k++)
        {
            if( (k>i) && (m_data[i] == m_data[k]) ){
                remove(k);
				
                //m_size--;
            }
        }
    }
}

template<typename T>
inline void ArrayList<T>::reverse()
{

	for (int i = 0; i < size()/2; i++)
	{
		int back = size() - 1;

		swap(i, back - i);
	}

}
