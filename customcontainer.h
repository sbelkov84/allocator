#ifndef CUSTOMCONTAINER_H
#define CUSTOMCONTAINER_H
//------------------------------------------------------------------------
#include <type_traits>
//------------------------------------------------------------------------
#include "customiterator.h"
#include "customallocator.h"
//------------------------------------------------------------------------

template <typename T, typename TAllocator = TCustomAllocator<T>>
class TCustomContainer
{
  private:

    TContainerItem<T>* Head = nullptr;
    TContainerItem<T>* Tail = nullptr;

    typedef typename TAllocator::template rebind<TContainerItem<T>>::other TRebindedAlloc;
    TRebindedAlloc Allocator;

    std::size_t Size;

  public:

    TCustomContainer() : Size(0){};

    ~TCustomContainer()
    {
      for (TContainerItem<T>* Next, *Ptr = this->Head; Ptr != nullptr; Ptr = Next)
      {
        Next = Ptr->PtrOnNext;
        Allocator.destroy(Ptr);
      }
      Allocator.deallocate(this->Head, Size);
    }

    using iterator = TCustomIterator<T>;
    iterator begin()
    {
      return iterator{Head};
    }
    iterator end()
    {
      return iterator{nullptr};
    }

    void push_back(T Data)
    {
      TContainerItem<T>* NewItem = Allocator.allocate(1);
      NewItem->Data = Data;
      NewItem->PtrOnNext = nullptr;

      if (Tail != nullptr)
      {
        Tail->PtrOnNext = NewItem;
      }
      else
      {
        Head = NewItem;
      }
      Tail = NewItem;
      ++Size;
    }
};
//------------------------------------------------------------------------

#endif // CUSTOMCONTAINER_H
