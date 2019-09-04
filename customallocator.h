#ifndef CUSTOMALLOCATOR_H
#define CUSTOMALLOCATOR_H

#include <memory>
//------------------------------------------------------------------------

template <typename T>
struct TPtrListItem
{
  TPtrListItem() : AllocatedQty(0), ElementsQty(0), CurChunk(nullptr), NextPtr(nullptr), PrevPtr(nullptr) {};
  //---
  std::size_t AllocatedQty;
  std::size_t ElementsQty;
  T* CurChunk;
  TPtrListItem<T>* NextPtr;
  TPtrListItem<T>* PrevPtr;
  //---
  void Dealloc(T* Ptr, std::size_t Qty)
  {
    std::free(Ptr);
    ElementsQty -= Qty;
  }
};
//------------------------------------------------------------------------

#define RESERVED_ELEMENTS_QTY 10
//------------------------------------------------------------------------

template <typename T>
struct TCustomAllocator
{
    using value_type = T;

    using pointer = T *;
    using const_pointer = const T *;    

    template<typename U>
    struct rebind
    {
      using other = TCustomAllocator<U>;
    };

    ~TCustomAllocator() = default;

    TCustomAllocator() noexcept
    {
      ChunksListsStart = nullptr;
    }

    T* allocate(std::size_t n)
    {
      TPtrListItem<T>* LastLastChunkList = GetLastChunkList();
      // if container has enough memory
      if (LastLastChunkList != nullptr && LastLastChunkList->AllocatedQty - LastLastChunkList->ElementsQty >= n)
      {
        auto p = LastLastChunkList->CurChunk + LastLastChunkList->ElementsQty;
        LastLastChunkList->ElementsQty += n;
        return p;
      }

      //if container needs more memory
      std::size_t Qty = CalcAllocQty(n);      

      auto p = std::malloc(Qty * sizeof(T));
      if (!p)
      {
        throw std::bad_alloc();
      }

      T* TmpPointer = reinterpret_cast<T*>(p);

      if (ChunksListsStart == nullptr)
      {
        ChunksListsStart = new TPtrListItem<T>();
        ChunksListsStart->CurChunk = TmpPointer;
        ChunksListsStart->AllocatedQty = Qty;
        ChunksListsStart->ElementsQty = n;
      }
      else
      {
        TPtrListItem<T>* NextListItem = ChunksListsStart;
        while (NextListItem->NextPtr != nullptr)
        {
          NextListItem = NextListItem->NextPtr;
        }
        TPtrListItem<T>* NewListItem = new TPtrListItem<T>();
        NewListItem->CurChunk = TmpPointer;
        NewListItem->AllocatedQty = Qty;
        NewListItem->ElementsQty = n;
        NewListItem->PrevPtr = NextListItem;
        NextListItem->NextPtr = NewListItem;
      }
      //---
      return TmpPointer;
    }

    void deallocate(T* p, std::size_t n)
    {
      TPtrListItem<T>*& TmpItem = GetChunkByPtr(p);
      if (TmpItem != nullptr)
      {
        TmpItem->Dealloc(p, n);
        if (TmpItem->ElementsQty == 0)
        {
          if (TmpItem->PrevPtr != nullptr)
            TmpItem->PrevPtr->NextPtr = nullptr;
          delete TmpItem;
        }
      }
    }

    template <typename U, typename... TArgs>
    void construct(U* p, TArgs&& ...Args)
    {
      new(p) (U)(std::forward<TArgs>(Args)...);
    }

    template <typename U>
    void destroy(U* p)
    {
      p->~U();
    }

  private:

    T* StartPointer;
    std::size_t ElementsQty;
    std::size_t AllocatedQty;
    std::size_t ChunksQty;

    TPtrListItem<T>* ChunksListsStart;
    TPtrListItem<T>* GetLastChunkList()
    {
      if (ChunksListsStart == nullptr)
        return nullptr;

      TPtrListItem<T>* NextListItem = ChunksListsStart;

      while (NextListItem->NextPtr != nullptr)
      {
        NextListItem = NextListItem->NextPtr;
      }
      //---
      return NextListItem;
    }

    std::size_t CalcAllocQty(std::size_t n)
    {
      std::size_t Reserved = n;
      if (n % RESERVED_ELEMENTS_QTY != 0)
        Reserved = (n / RESERVED_ELEMENTS_QTY + 1) * RESERVED_ELEMENTS_QTY;
      //---
      return Reserved;
    }

    TPtrListItem<T>*& GetChunkByPtr(T* Ptr)
    {
      TPtrListItem<T>*& NextListItem = ChunksListsStart;
      while (NextListItem->NextPtr != nullptr)
      {
        std::size_t PtrDiff = Ptr - NextListItem->CurChunk;
        if (PtrDiff < NextListItem->AllocatedQty && PtrDiff >= 0)
        {
          break;
        }
        NextListItem = NextListItem->NextPtr;
      }
      //---
      return NextListItem;
    }
};
//------------------------------------------------------------------------

#endif // CUSTOMALLOCATOR_H
