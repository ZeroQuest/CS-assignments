#ifndef ARRAYQUEUE_H
#define ARRAYQUEUE_H

#include <iostream>
#include <string>
#include "abstractqueue.h"

using namespace std;

/*
	ArrayQueue Class

	Purpose: Implements a Queue ADT using a
		dynamically allocated circular array that inherits from
		the abstractqueue class to store the contents of the queue

	Error Handling: Whenever a function is given invalid parameter values
		it THROWS an 'Oops' object.
*/

/* ---- Class Prototypes ---- */

template <class T>
class ArrayQueue;

/* ---- Function Prototypes ---- */

//Outputs a Queue
template <typename T>
std::ostream& operator<< (std::ostream& out, const ArrayQueue<T>& aqueue);

template <typename T>
class ArrayQueue : public AbstractQueue<T>
{
	private:
		int m_size;					// current number of elements
		int m_max;					// maximum capacity of array m_data
		int m_front;				// index to the front of the circular array
		int m_back;					// index to the back of the circular array
		T* m_data;					// array to store the elements

		//Auxilary Functions

		//Purpose: Increases the size of storage array
		//Postconditions: a new array of size max*2 is allocated,
		//		and m_data is compied to it.
		//		m_max is set to the size of this new storage array
		void grow();

		//Purpose: Decreases the size of storage array
		//Postconditions: a new array of size m_max/2 is allocated,
		//		and m_data is copied to it.
		//		m_max is set to the size of this new storage array
		void shrink();

	public:
		/* ---- Constructors ---- */

		//Purpose: Default Constructor
		//Postconditions: Current size and maximum size set to 0,
		// front and back set to 0, and data set to NULL.
		ArrayQueue();

		//Purpose: Parameterized Constructor
		//Parameters: s : the requested size of the queue
		//			  x : the initial elements in the queue
		//Postconditions: the queue is constructed as a queue of 's' copies of 'x'
		ArrayQueue(int s, const T& x);

		/* ---- Big 3 Member Functions ---- */

		//Abstract Destructor
		//~AbstractQueue();

		//Purpose: Destructor
		//Postconditions: current size and maximum size set to 0,
		//				  front and back set to 0,
		//				  data deallocated and set to NULL
		~ArrayQueue();

		//Purpose: Operator=, performs a deep copy of 'rhs' into 'this' ArrayQueue
		//Parameters: rhs : ArrayQueue to be copied
		//Returns: *this
		//Postconditions: rhs == this
		ArrayQueue<T>& operator=(const ArrayQueue<T>& rhs);

		//Purpose: Copy Constructor
		//Parameters: cpy = ArrayQueue to be copied
		//Postconditions: current size, maximum size, front, back, and elements same as cpy
		ArrayQueue(const ArrayQueue<T>& cpy);

		/* ---- Accessors ---- */

		//Purpose: Accessor function for the number of elements in the queue
		//Returns: number of elements in the queue
		int size() const;

		//Purpose: Accessor function for the max capacity of elements
		//Returns: max capacity of this queue
		int max() const;

		//Purpose: determines whether the queue is empty
		//Returns: 'true' if the queue is empty; 'false' otherwise
		bool isEmpty() const; //Implement from Abstract

		//Purpose: looks at the front of the queue
		//Returns: a reference to the element currently in front of the queue
		//Exception: if the queue is empty, THROW a 'Oops' object with an error
		//	message!!!
		const T& front() const throw (Oops); //Implement from Abstract

		//Purpose: looks at the back of the queue
		//Returns: a reference to the element currently in the back of the queue
		//Exception: if the queue is empty, THROW a 'Oops' object with an error
		//	message!!!
		const T& back() const throw (Oops); //Implement from Abstract

		// Purpose: return a read-and-write reference to the element at
		// the specified position
		// Parameters: i is 0-based index into the ArrayQueue
		// Returns: the element at position i in the ArrayQueue
		T& operator[](int i);

		// Purpose: return a read-only reference to the element at
		// the specified position
		// Parameters: i is 0-based index into the ArrayQueue
		// Returns: the element at position i in the ArrayQueue
		const T& operator[](int i) const;

		/* ---- Mutators ----*/

		//Purpose: enqueue an element into the queue
		//Parameters: x is the item to add to the queue
		//Postconditions: x is now the element at the end of the queue
		void enqueue(const T& x); //From Abstract

		//Purpose: dequeues an element from the queue
		//Postconditions: the element formerly at the front of the queue
		//	has been removed.
		//Note: Dequeueing an empty queue results in an empty queue.
		void dequeue(); //From Abstract

		//Purpose: clears the queue
		//Postconditions: the queue is now empty
		void clear();

		// Purpose: prints the queue
		friend std::ostream& operator<< <> (std::ostream& out,
                                      const ArrayQueue<T>& aqueue);

}; // of class ArrayQueue

#include "arrayqueue.hpp"
#endif
