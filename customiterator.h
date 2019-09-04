#ifndef CUSTOMITERATOR_H
#define CUSTOMITERATOR_H
//------------------------------------------------------------------------

template <typename T>
struct TContainerItem
{
  T Data;
  TContainerItem<T>* PtrOnNext;
};
//------------------------------------------------------------------------

template <typename T>
class TCustomIterator
{
  private:

    TContainerItem<T>* Pointer;

  public:

    explicit TCustomIterator(TContainerItem<T>* Ptr) : Pointer(Ptr) {}

    T& operator* ()
    {
      return Pointer->Data;
    }

    TCustomIterator<T>& operator++ ()
    {
      Pointer = Pointer->PtrOnNext;
      return *this;
    }

    bool operator!= (const TCustomIterator<T>& RHS)
    {
      return Pointer != RHS.Pointer;
    }

};
//------------------------------------------------------------------------

#endif // CUSTOMITERATOR_H
