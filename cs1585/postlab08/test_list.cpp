#include "catch.hpp"
#include "list.h"

template class List<int>;

TEST_CASE("List sizing", "[list]" )
{
   List<int> l;

   SECTION("Uninitialized List has a NULL cell")
   {
     CHECK(l.length() == 0);
     CHECK(l.empty() == true);
   }
   SECTION("Copied Empty List has same values")
   {
     List<int> t(l);

     CHECK(l.length() == t.length());
     CHECK(l.empty() == t.empty());
   }

   for(int i = 0; i < 5; i++)
   {
     l.append(i);
   }

   SECTION("Adding Elements changes size and emptiness")
   {
     CHECK(l.length() == 5);
     CHECK(l.empty() == false);
   }
   SECTION("Copied non-empty list has same values")
   {
     List<int> t(l);

     CHECK(l.length() == t.length());
     CHECK(l.empty() == t.empty());
   }
   SECTION("Head is accessible")
   {
     CHECK(l.head() == 0);
   }
}

TEST_CASE("List Elements", "[list]")
{
   List<int> l;

   for(int i = 0; i < 5; i++)
   {
      l.prepend(i);
   }

   SECTION("Elements added with prepend are accessible")
   {
      Cell<int>* it = l.iterator();
      int i = 4;
      while(it != NULL)
      {
         CHECK(it->elem == i);
         it = it->next;
         i--;
      }
   }
   SECTION("A copied list is identical")
   {
      List<int> t(l);
      Cell<int>* itl = l.iterator();
      Cell<int>* itt = t.iterator();
      while(itl != NULL && itt != NULL)
      {
	 CHECK(itl->elem == itt->elem);
         itl = itl->next;
         itt = itt->next;
      }
   }
   SECTION("Copies are deep copies")
   {
      List<int> t(l);
      Cell<int>* itl = l.iterator();
      Cell<int>* itt = t.iterator();
      while(itl != NULL && itt != NULL)
      {
         itt->elem = -1;
         CHECK(itl->elem != itt->elem);
         itl = itl->next;
         itt = itt->next;
      }
   }

   for(int i = 0; i < 4; i++)
   {
      l.pop();
   }

   SECTION("Elements removed with pop are not accessible")
   {
     Cell<int>* it = l.iterator();
     CHECK(it->next == NULL);
   }

   List<int> c;

   for(int i = 0; i < 5; i++)
   {
      c.append(i);
   }

   SECTION("Elements added with append are accessible")
   {
      Cell<int>* it = c.iterator();
      int i = 0;
      while(it != NULL)
      {
         CHECK(it->elem == i);
         it = it->next;
         i++;
      }
   }
}
TEST_CASE("Testing Exceptions", "[list]")
{
  List<int> l;

  SECTION("Pop throws exception when list is empty")
  {
     CHECK_THROWS(l.pop());
  }
  SECTION("Head throws exception when list is empty")
  {
     CHECK_THROWS(l.head());
  }
  SECTION("Tail throws exception when list is empty")
  {
     CHECK_THROWS(l.tail());
  }
}
