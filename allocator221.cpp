//Нужно изучить тему аллокаторов в С++
//Написать свой собственный аллокатор
// Сделать адаптер к своему аллокатору, чтобы его можно
//было использовать в контейнерах STL, например в std::vector

#include <iostream>
#include <limits>
#include <memory>
#include <vector>

 template<class T>
class TrackingAllocator
{
public:
    using value_type = T;

    using pointer = T *;
    using const_pointer = const T *;

    using void_pointer = void *;
    using const_void_poiner = const void *;

    using size_type = size_t;

    using difference_type = std::ptrdiff_t;

    template<class U>
    struct rebind
    {
      using other = TrackingAllocator<U>;
    };

    TrackingAllocator() = default;

    template<class U>
    TrackingAllocator (const TrackingAllocator<U> &other){}


    ~TrackingAllocator() = default;

    pointer allocate(size_type num0bjects)
    {
        mAllocations += num0bjects;
        return static_cast <pointer> (operator new (sizeof(T) * num0bjects));
    }

    pointer allocate(size_type num0bjects, const_void_poiner hint)
    {
        return allocate(num0bjects);
    }
       void deallocate(pointer p, size_type num0bjects)
       {
           operator delete(p);
       }
       size_type max_size() const
       {
           return std::numeric_limits<size_type>::max();
       }

       template <class U,class... Args>
       void construct (U *p, Args && ...args)
       {
           new(p) U(std::forward<Args>(args)...);
       }

       template<class U>
       void destroy (U *p)
       {
           p->~U();
       }

     size_type get_allocations() const
     {
       return mAllocations;
     }

private:
    static size_type mAllocations;
};


    template<class T>

    typename TrackingAllocator<T>::size_type TrackingAllocator<T>::mAllocations = 0;



int main()
{
    using TAint = TrackingAllocator<int>;
    using TAdouble = TAint::rebind<double>::other;


    std::vector<int, TrackingAllocator<int>> v(5);
    std::cout << v.get_allocator().get_allocations()<< std::endl;

    return 0;
}
