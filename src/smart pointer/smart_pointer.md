头文件：`#include <memory>`

智能指针的使用方式和普通指针几乎没有区别，无论是指针的使用、判空还是解引用，都一样，但是却比普通指针多了自动释放资源的功能。

核心在于RTTI，只要出了作用域，就会自动回收内存空间并调用析构函数。

# unique_pointer

独占型智能指针，是一种零开销的智能指针，除了自定义deleter，否则性能和普通指针别无二致

不允许其他智能指针共享其内部原始指针，自然不支持赋值构造

所以涉及到函数之间参数的传递的时候，尤其注意不能让其发生拷贝！

```cpp
struct test {};
void print(std::unique_ptr<test>& p) {} //接受引用，如果不是引用传递而是值传递，就发生了拷贝行为，这在unique_ptr中是不被允许的
int main() {
    std::unique_ptr<test> uptr = std::make_unique<test>();
    print(uptr); //将unique_ptr传递给print函数
}
```

## 构造方式

- 通过构造函数初始化

```cpp
class cs {};
std::unique_ptr<cs> uptr(new cs); 
```

- 通过make_unique

```cpp
class cs {};
std::unique_ptr<cs> uptr = std::make_unique<cs>();
```

- move() 转移所属权

```cpp
class cs {};
std::unique_ptr<cs> uptr1 = std::make_unique<cs>();
std::unique_ptr<cs> uptr2 = std::move(uptr1);
```

## get() 和 release() 的区别

声明一点，虽然创建出来的是指针，理应用->去调用方法，但实际上这是一个对象，还是用.去调用

get()可获取原始指针，可以去使用这个原始指针，但unique_ptr仍然保持对该对象的所有权

```cpp
struct cs { void print() {std::cout << "wocol\n";} };
int main() {
    std::unique_ptr<cs> uptr = std::make_unique<cs>();
    cs* p = uptr.get(); //获得原始指针
    p->print(); //使用原始指针
    uptr->print(); //仍旧有效
    delete p;
    uptr->print(); //core dump，因为智能指针管理的指针删除，智能指针也失效了
}
```

release()先获取原始指针，然后释放管理放象的所有权，但不会销毁该对象。

也就是说调用release方法后，需要手动管理所返回的裸指针，包括在适当的时候销毁它

## reset()

reset方法用来重置指针的管理对象，可以用于接管新的指针，并在必要时销毁先前的对象

```cpp
class cs {};
std::unique_ptr<cs> uptr = std::make_unique<cs>();
uptr.reset(new cs); //重置管理对象，并接管新的指针(new cs)
```

## 自定义删除器

管理动态数组时，必须指定自定义删除器，因为默认删除器不支持数组对象

`std::unique_ptr<int, void (*)(int[])> sp(new int(12), [](int* p){delete[] p;});`

回调函数的写法：

```cpp
// 自定义删除器
void del(int* p) {
    std::cout << "del has been called\n";
    delete p;
}
int main() {
    std::unique_ptr<int, decltype(del)*> up(new int[10], del);
    //或者下面这个
    std::unique_ptr<int, void (*)(int*)> uptr(new int[10], del);
}
```

# shared_ptr

共享型智能指针

## 创建对象

- 使用make_shared创建，`std::shared_ptr<int> ptr1 = std::make_shared<int>(42);`
- 直接使用new创建，`std::shared_ptr<int> ptr2(new int(100));`
- 使用拷贝构造创建，`std::shared_ptr<int> ptr3(ptr1);`
- 使用赋值构造创建，`std::shared_ptr<int> ptr4 = ptr1;`

## 引用计数

使用`use_count()`可获取引用计数，只要共享智能指针被初始化了一块有效内存，那么此处内存对用的引用计数就会加一

若共享智能指针被创建出来但是没有初始化，或者初始化为空，那么此处内存对应的引用计数不变还是零

## reset()

重置，共享智能指针会指向别的内存，并解除对原始内存的绑定，引用计数也相应减一

```cpp
std::shared_ptr<int> p1 = std::make_shared<int>(1);
std::shared_ptr<int> p2 = p1;
p2.reset();
std::cout << p1.use_count() << std::endl; //2 - 1
std::cout << p2.use_count() << std::endl; //0
```

## 获取原始指针

get()方法

## 指定删除器

还是管理数组为例

`std::shared_ptr<int> p(new int[10], [](int* p){delete[] p;});`

不过新标准中有了默认删除器可以管理动态数组

`std::shared_ptr<int> sptr(new int[10], std::default_delete<int[]>());`

# weak_ptr

弱引用智能指针，用来监控共享型智能指针，它不共享指针，也无法操作资源

## 初始化

用shared_ptr，配合构造函数或者赋值构造函数为其初始化

```cpp
std::shared_ptr<int> sptr(new int(10));
std::weak_ptr<int> wp(sptr); //构造函数
std::weak_ptr<int> wp2 = sptr; //赋值构造
```

## expired

判断目标资源是否被释放，因为weak_ptr监测的是shared_ptr管理的资源，当shared_ptr调用reset()后，资源被释放，expired()就为true，表明监测对象已释放

## 获得目标资源的shared_ptr指针

std::shared_ptr<int> sp = wp.lock();

```cpp
void observe(std::weak_ptr<int> wp) {
    std::cout << wp.use_count() << std::endl;
    //weak_ptr只能监控不能做事，所以还需获得目标对象的shared_ptr
    std::shared_ptr<int> sp = wp.lock();
    if (sp) {
        std::cout << *sp << std::endl;
    } else {
        std::cout << "wp is expired\n";
    }
}
int main() {
    {
        auto sp = std::make_shared<int>(42);
        std::weak_ptr<int> wp = sp;
        observe(wp); // 1 42
    }
    std::weak_ptr<int> wp;
    observe(wp); //0 wp is expired
}
```
