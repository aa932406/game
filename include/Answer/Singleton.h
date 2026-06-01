#ifndef _SINGLETON_H_
#define _SINGLETON_H_

namespace Answer {

template<typename T>
class Singleton
{
public:
    static T* instance()
    {
        static T inst;
        return &inst;
    }

    static T* getInstance()
    {
        return instance();
    }

protected:
    Singleton() = default;
    virtual ~Singleton() = default;

private:
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
};

} // namespace Answer

#endif // _SINGLETON_H_
