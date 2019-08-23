#include <memory>
class Adapter
{
public:
    Adapter(std::shared_ptr<int> pc);
    void PrintVal();

private:
    std::shared_ptr<int> pc_;
};

template <typename T>
class RefCountedObject : public T
{
public:
    RefCountedObject() {}

    template <typename P0>
    explicit RefCountedObject(P0 &&p0) : T(std::forward<P0>(p0)) {}

    template <typename P0, typename P1, typename... Args>
    RefCountedObject(P0 &&p0, P1 &&p1, Args &&... args)
        : T(std::forward<P0>(p0),
            std::forward<P1>(p1),
            std::forward<Args>(args)...) {}

    virtual void AddRef() const { ref_count_++; }
    
    virtual int ReleaseRef() const {
        const auto count = --ref_count_;
        if(count == 0){
            delete this;
        }
        return count;
    }

protected:
    // why protect desctructor ?
    virtual~ RefCountedObject(){}
    mutable int ref_count_{0};

    // disallow copy and assign
    RefCountedObject(const RefCountedObject&) = delete;
    RefCountedObject& operator=(const RefCountedObject&) = delete;
};
// RAII: Class T should support AddRef() and DecRef()
template <typename T>
class scoped_refptr
{
public:
    typedef T element_type;
    scoped_refptr() : ptr_(nullptr) {}

    scoped_refptr(T *p) : ptr_(p)
    {
        if (ptr_)
            ptr_->AddRef();
    }

protected:
    T *ptr_;
};