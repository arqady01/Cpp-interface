* [➕ C/C++ ☃️](#c/c++)
* [📡 模板元编程](#template)
* [🧭 STL](#STL)
* [🎡 多线程](#multithread)
* [💀 Effective C++](#effectivecpp)
* [☁️ 计算机网络](#internet)
* [🎀 剑指offer](#algorithm)
* [💾 数据库](#database)
* [📏 设计模式](#1)
* [&#x1F527; GDB](#2)
* [&#x1F3A9; Boost::chrono](#chrono)

<h1 id="c/c++">➕ C/C++ ☃️</h1>

## C++从代码到可执行程序经历了什么？

- 预处理，主要处理源代码中的以“#”开头的预编译指令，比如展开宏定义#define，处理条件预编译指令如“#ifdef”、“#elif”和“#else”，处理“#include”预编译指令
- 编译，把预编译后生成的xx.i文件，进行语义分析及优化后，生成相应的汇编文件
- 汇编，将汇编代码转为xx.o机器码文件
- 链接，将不同的源文件产生的目标文件进行链接，从而形成一个可以执行的程序

## const

- 修饰变量，该变量不可被改变，但在定义之初就必须初始化（否则再也没机会赋值）
- 修饰指针，分为指向常量的指针和指针常量
    - const void *p; 常量指针，可以换方向，不可以改内容
    - void * const p; 指针常量，不可以换方向，可以改内容
- 修饰引用，经常用于形参类型，即避免了拷贝，又避免了函数对值的修改
    - 指向常量的引用 reference to const
    - 没有const reference，因为引用只是别名，不是对象，不能用 const 修饰
- 修饰成员函数，说明该成员函数内不能修改成员变量

## #define和const

| 宏定义#define | const          |
| ---------------- | -------------- |
| 无类型安全检查 | 有类型安全检查 |
| 不分配内存  | 要分配内存 |
| 存储在代码段 | 存储在数据段 |
| 可通过#undef取消 | 不可取消   |

## static

- 普通变量，修改其存储区域和生命周期，使变量存储在静态区，在 main 函数运行前就分配了空间
- 普通函数，指明作用范围，仅在定义该函数的文件内才能使用。团队开发项目时，为防止和他人的函数重名，可将函数定义成static
- 成员变量，修饰成员变量使所有的对象只保存一个该变量，而且不需要实例化类对象就可以访问该成员，比如 类名::成员变量
- 成员函数，修饰成员函数使得不需要生成对象就可以访问该函数，但只能访问静态成员，不能访问非静态成员

## 引用和指针的区别

- 指针是一个实体，需要分配空间；引用是对象的别名，不需要分配空间
- 普通指针定义时不需要初始化，指向的空间可变；引用定义之时就必须初始化并且不可改变
- 有多级指针但不存在多级引用
- 指针自增是指向下一空间，引用自增是引用的变量加一
- sizeof(*)得到指针本身的大小，32位为4bytes，64位为8字节。sizeof(&)返回所引用的对象的大小

## 传递函数参数时，指针和引用使用场景

返回函数局部变量的内存的时候用指针。而返回局部变量的引用是没有意义的

对栈空间大小比较敏感（比如递归）的时候用引用，使用引用传递不需要创建临时变量，开销要更小

类对象作为参数传递的时候使用引用（比如链式引用），这是C++类对象传递的标准方式

深拷贝的时候

## this指针

- this指针隐含于每一个非静态成员函数中，指向调用者，谁调用函数，this就指向它
- 当用一个对象调用成员函数时，例如，obj.func()；此时对象地址作为隐藏的this参数传递给函数func，所以成员函数应该视为：`func(ObjType* const this)`
- this 不是常规变量，而是个右值，所以不能取得 this 的地址（不能 &this）
- this 只能在成员函数中使用，全局函数和静态成员函数中无法使用（静态成员函数相当于共享的变量，不属于某个对象的变量）
- 友元函数因为不是类成员，所以它没有this指针，自然友元函数也无法使用this指针

经常需要显式使用 this 指针的场景：

- 为实现对象的链式引用；

```cpp
struct complex {
    complex& operator+=(const complex&);
    int a;
};
//设计的函数：需要传递两个参数，调用者和被调用者
//返回类型必须是引用，不然返回一个临时对象，生命周期不足以进入第二个参数
complex& dop(complex* t, const complex& c) {
    t->a += c.a; //改造t
    return *t; //改造完后将其返回
}
//类外定义
complex& complex::operator+=(const complex& r) {
    return dop(this, r); //为了和dop参数匹配，需要显式使用this指针
}
//链式使用：c3 += c2 += c1;
```

- 避免对同一对象进行赋值操作，比如深浅拷贝时判断两个对象是否相同

```cpp
struct String {
    String& operator=(const String&); //拷贝赋值函数
    char* data;
};
String& String::operator=(const String& str) {
    if (this == &str) //检测自我赋值，如果是自己
        return *this; //什么都别做，直接把自己返回就好
    delete[] data; //删除旧的
    data = new char[strlen(str.data) + 1]; //分配空间
    strcpy(data, str.data); //复制
    return *this; //将深拷贝后的值返回出去
}
```

- 在实现一些数据结构时，如 单/双向链表

**类中非静态成员函数如何寻址**

问题描述如图：

![q](https://github.com/arqady01/Cpp-interface/blob/main/resource/cpp_images/howtoloopaddress.png)

举个例子，在complex的设计中，包含两个非静态成员：实部和虚部，当实例化三个对象后，this指针就派上用场了，如下图所示：

![a](https://github.com/arqady01/Cpp-interface/blob/main/resource/cpp_images/takingprogress.png)

## delete this

要想使用`delete this`，必须保证调用 delete this 的成员函数是最后一个调用 this 的成员函数

如果在类的析构函数中调用 `delete this`，会导致堆栈溢出。因为delete是先调用析构函数后free内存，显然delete this会去调用本对象的析构函数，然后析构函数中又会调用删除类中隐含的this指针，形成无限递归造成堆栈溢出，直至奔溃。

## 野指针

没有被初始化过的指针。为了防止出错，在定义时就将指针置空，IDE就不会直接报错，产生非法内存访问

## 悬空指针

最初指向的对象被释放，但指针并未置空

## 深拷贝 和 浅拷贝

浅拷贝只是拷贝一个指针，并没有开辟新地址，新指针和原指针都指向同一块内存地址。容易出现的错误就是同一地址资源被重复释放

深拷贝会开辟一块新内存用来存放新值，即使原先的对象被析构，新对象也不受影响。在自己实现拷贝赋值的时候，如果有指针变量，是需要动手实现深拷贝的。

## strlen 和 sizeof 的区别

- strlen是处理字符的函数，sizeof是运算符而非函数，编译期就可知而非运行期
- strlen只能接收字符指针并且是'\0'结尾，sizeof接收任何类型的数据
- sizeof的结果在编译期就确定，所以无法用来计算动态分配（运行期分配）的空间的大小

## inline 内联

- 内联发生在编译期，虚函数的多态性在运行期，所有对虚函数的调用都会使inline落空
- 相当于把内联函数里面的内容写在调用内联函数处；
- 相当于宏，却比宏多了类型检查，真正具有函数特性；
- 内联拒绝太过复杂的函数，比如循环、递归等
- 在类中的函数，除虚函数外，其他函数都会隐式地当成内联函数。

```cpp
// 声明（加 inline，建议使用）
inline int functionName(int args);

// 类内定义，隐式内联
class A {
    int doA() { return 0; } // 隐式内联
}

// 类外定义，需要显式内联
class A {
    int doA();
}
inline int A::doA() { return 0; }   // 需要显式内联
```

优点

- 内联函数同宏函数一样将在被调用处进行代码展开，省去了参数压栈、栈开辟与回收，从而提高程序运行速度。
- 内联函数相比宏函数来说，在代码展开时，会做安全检查或自动类型转换，而宏定义则不会。
- 在类中声明同时定义的成员函数，自动转化为内联函数，因此内联函数可以访问类的成员变量，宏定义则不能。
- 内联函数在运行时可调试，而宏定义不可以。

缺点

- 内联以代码膨胀（复制）为代价，消除函数调用带来的开销。
- inline函数的改变需要重新编译，不像 non-inline 可以直接链接。
- 是否内联的决定权在于编译器，用户不可控。

## extern

- 对变量或函数进行声明：表示extern修饰的变量或者函数定义在别的文件中，告知编译器遇到此变量或函数时，应去别的文件中寻找它的定义
- 对链接规范进行指定：当与字符串连用时，extern指明当前声明使用了其他语言的链接规范。比如extern "C"，就表示使用C语言的链接规范。

## mutable 

在类中声明一个可变的数据成员。如果一个数据成员被声明为mutable，即使在const成员函数中，数据成员依旧可以被修改，谨慎使用，滥用可能会破坏类的逻辑状态

mutable 允许在不改变对象状态的情况下修改数据成员。这在一些特殊情况下非常有用，比如实现缓存、线程安全性等。例如，一个类需要在第一次请求时计算一个值并缓存结果，就可以使用mutable来修改缓存的数据成员。

## volatile

声明变量是容易发生变化的（比如在中断处理或者硬件操作中），不要对该变量进行优化，每次取值都必须从内存中去取，而不是去之前保存在寄存器中的值，以避免对变量访问进行优化，从而确保变量的实时性。

常见场景:

- 共享变量，多线程中使用共享变量不应对变量优化
- 中断程序会修改其他程序中使用的变量

## explicit

- 修饰构造函数时，可以防止隐式转换和复制初始化
- 修饰转换函数时，可以防止隐式转换

## struct & class

以下说的struct都是C语言的结构体，不是C++的！

- 访问权限不同，struct默认public属性，class默认private属性
- struct没有构造函数，更不能用列表初始化；struct只能用函数指针不能定义函数
- struct无法重载malloc（不过本身malloc也不支持重载），class可以operator new接管内存分配
- struct必须定义别名才能使用`typedef struct {} stu;`
- 继承相关
- 类能够实现类模板，但是class不支持虚函数模板

## 面向对象特性

- 封装：使用函数指针把属性与方法封装到结构体中
- 继承：结构体嵌套
- 多态：父类与子类方法的函数指针不同，有多种形式显示的能力

## 虚函数表和虚函数表指针

虚函数表中主要存放了虚函数的入口地址和RTTI指针

针对动态多态而言，当子类对象重写父类的虚函数时，父类的虚函数表中存储的虚函数入口地址就会被子类虚函数的地址覆盖

只要是利用虚函数表指针vptr找到虚函数表vptl，然后查询vptl找到虚函数的入口地址，并执行虚函数。那么这个过程就是多态

这也解释了为什么基类指向派生类对象，调用虚函数的时候会调用到派生类的虚函数，这也正是多态的体现。看下面的例子中的①、②和③

![vptr](https://github.com/arqady01/Cpp-interface/blob/main/resource/cpp_images/vptr.png)

#### 虚函数表和虚函数表指针 存放位置

虚函数表（vtable）：存储在常量数据区，具体是只读数据段，因为它在程序运行期间不应该被修改，为类的所有对象共享

虚函数指针（vptr）：存储在每个对象内部，位于堆区或栈区，指向对应类的vtable

这种机制使得当调用对象的虚函数时，程序可以通过对象的vptr找到对应的vtable，再通过vtable定位到正确的虚函数实现，从而实现运行时的多态性

#### 虚函数表创建时机

编译时：vtable 是在编译阶段为每个包含虚函数的类创建的，编译器会为这些类生成一个静态的虚函数表。每个类的 vtable 中存储了该类所有虚函数的地址

#### 虚函数表指针创建时机

类调用ctor时（没有ctor就生成一个default ctor），把类的虚函数表的地址赋值给 vptr

如果发生了继承关系，基类调用基类的ctor，把基类的vtable地址赋值给vptr，派生类调用派生类的ctor，把派生类的vtable地址赋值给vptr

#### 为什么C++默认的析构函数不是虚函数

当类中有虚函数时，类会自动生成虚函数表和指向虚函数表的虚表指针，虚函数表的作用就是保存类中各个虚函数的入口地址，因为包含了vtble和vptr，就会占用额外的内存，如果类不被继承时，这种开销无疑是浪费的

#### class中支持虚函数模板吗？

不支持，虚函数的调用机制是，一个固定大小的虚表，其中存放了每一个条目的虚函数入口指针，大小固定，而类的成员函数模板到程序编译之前，实例化个数都无法确定

## 静态绑定 & 动态绑定【未编辑】

静态绑定类似call ...，发生在编译期

动态绑定首先必须是一个指针，且向上转型（类图中表现为指针虽然指向子类，但类型向上转化为父类），最后调用虚函数，发生在运行期

## 虚函数 & 纯虚函数

类中声明了虚函数，不管有没有实现，它都可以让这个函数在它的子类中被覆盖（override），通过后期绑定就可以实现多态。虚函数在子类中可以不重写

带纯虚函数的类叫抽象类，抽象类不能直接实例化，只能被继承，必须在子类重写其虚函数后，才可以实例化子类并使用

由于纯虚函数并没有具体的函数体，因此它在虚函数表中的值就为0，而具有函数体的虚函数在虚函数表中则是函数的具体地址

## 虚析构函数

虚析构函数是为了解决基类的指针指向派生类对象，并用基类的指针删除派生类对象。但是析构函数绝不能是纯虚函数

CRTP模板中，不应该将析构函数声明为虚函数，理论上所有的基类函数都不应该声明为虚函数，因为这种继承方式，不需要虚函数表

## RTTI运行时类型信息

typeid

- typeid 运算符允许在运行时确定对象的类型
- type_id 返回一个 type_info 对象的引用
- 如果想通过基类的指针获得派生类的数据类型，基类必须带有虚函数
- 只能获取对象的实际类型

type_info

提供了一种在运行时获取类型信息的方法

## 重载overload

函数名相同，参数类型和参数个数不同。重载和成员函数是否是虚函数无关

函数重载时的匹配顺序：1.名字查找 2.确定候选函数 3.寻找最佳匹配

## 重写override

重写又称覆盖，一般在函数后面加上override关键字

在子类中重写父类中的同名函数，要求父类函数必须是虚函数，并且参数类型、参数个数和返回值类型都要和父类中的虚函数保持一致

### override关键字

当子类重写了父类的虚函数foo()时，以下三种方法都对：

```cpp
void foo();
virtual void foo();
virtual void foo() override;
```

但是推荐用第三种方法，因为如果手抖写成f0o()，编译就会报错因为找不到父类中f0o这个虚函数

### 重载和重写区别

- 重载是不同函数之间的水平关系，重写是父类和子类之间的垂直关系
- 重载要求参数列表不同，返回值无要求；重写要求参数列表必须相同
- 重载根据调用时实参表和形参表的对应关系来选择函数体；重写调用方法根据对象类型决定

## 隐藏hide

某些情况下，子类中的函数隐藏了父类中的同名函数

两个函数参数相同，但父类的函数不是虚函数。和重写的区别在于父类是否是虚函数

```cpp
struct A {
    void func() {
        std::cout << "A中函数\n";
    }
};
struct B : public A {
    void func() {
        //隐藏父类中的func函数
        std::cout << "B中函数\n";
    }
};
int main() {
    B b;
    b.func();
    b.A::func();
}
```

两个函数参数不同，无论父类函数是不是虚函数，都会被隐藏

## final

不希望某个类被继承，可以在类后加上final关键字

不希望某个类的虚函数在子类中被重写，可以在函数后加上final关键字

## 左值 & 右值

不考虑引用以减少干扰：左值可以取地址、位于等号左边；而右值没法取地址，位于等号右边

左右值的概念很清晰，有地址的变量就是左值，没有地址的字面值、临时值就是右值

`int a = 5;`

- a可以通过&取地址，位于等号左边，所以a是左值
- 5位于等号右边，5没法通过&取地址，所以5是个右值

```cpp
struct A {
    A(int a): a_(a) { }
    int a_;
};
A a = A(0);
```

- a可以通过&取地址，位于等号左边，所以a是左值
- A(0)是个临时值，没法通过&取地址，位于等号右边，所以A()是个右值

右值可分两种：

- 纯右值：非引用返回的临时变量、字面常量、lambda表达式等
- 将亡值：与右值引用相关的表达式，比如T&&类型函数的返回值、std::move的返回值

### 左值引用

能指向左值，不能指向右值的就是左值引用，左值引用无法指向右值

```cpp
int a = 5;
int &ref_a = a; //左值引用指向左值，编译通过
int &ref_a = 5; //左值引用指向右值，编译失败
```

但是，const左值引用是可以指向右值的：

`const int &ref_a = 5; //编译通过`

const左值引用不会修改指向值，因此可以指向右值，这也是为什么要使用`const &`作为函数参数的原因之一，比如 vector 中的

`void push_back (const value_type& val);` 如果没有const，`vec.push_back(5)`这样的代码就无法编译通过。另一方面，const 主要为了延长将亡值的生命周期。

### 右值引用

右值引用的标志是 && ，可以指向右值，不能指向左值

```cpp
int &&ref_a_right = 5; //ok
int a = 5;
int &&ref_a_left = a; //编译不过，右值引用不可以指向左值
ref_a_right = 6; //右值引用的用途：可以修改右值
```

### 右值引用指向左值

```cpp
int a = 5; //a是左值
int &a_left = a; //左值引用指向左值
int &&a_right = std::move(a); //通过move将左值转为右值，可以被右值引用指向
cout << a; //输出：5。why？看上去左值a通过move()移动到了右值a_right中
//那是不是a就没有值了？并不是，a的值仍然是5
```

move()是一个非常有迷惑性的函数，往往以为它能把一个变量里的内容移动到另一个变量，但事实上move()移动不了什么，唯一的功能是把左值强制转化为右值，让右值引用可以指向左值。其实现等同于一个类型转换，所以，单纯的move不会有性能提升

### 万能引用

对于形如T&&的变量，只有发生自动类型推导，T&&才是万能引用，否则就是右值引用。最常见的万能引用方式如以下两种：

```cpp
template<typename T>
void f(T&& param); // 存在类型推导，param是一个万能引用

auto&& var = var1; // auto自动类型推导，var是一个万能引用
```

注意以下情况不是万能引用：

```cpp
template<typename T>
class Test {
	Test(Test&& rhs);  // Test是一个特定的类型，不需要类型推导，所以&&表示右值引用  
};

//形如const T&&的方式也不是万能引用
template<typename T>
void f(const T&& t); // t是右值引用
```

### 引用折叠

X& &、X& &&、X&& & 可折叠成 X&

X&& && 可折叠成 X&&

### 完美转发 std::forward

完美转发能够保持变量的原始值属性（左值或右值）不变，并转发出去

一个应用就是保证右值引用在传递的过程中类型不会变为左值引用，比如：

```cpp
template <typename T>
void wrapper(T&& t) { // 万能引用
    func(std::forward<T>(t)); // 完美转发
}

class MyClass {};
void func(MyClass& a) { std::cout << "in func(MyClass&)\n"; }
void func(const MyClass& a) { std::cout << "in func(const MyClass&)\n"; }
void func(MyClass&& a) { std::cout << "in func(MyClass &&)\n"; }
int main(void) {
    wrapper(MyClass()); //in func(MyClass &&)而不是in func(const MyClass&)
}
```

- 当模板类型为左值引用，类型会被推导为左值
- 当模板类型不是左值引用类型，一律推导为右值

```cpp
#include <iostream>
template <typename T>
void print(T& t) { //A函数，左值引用
    std::cout << "left_reference：" << t << std::endl;
}
//重写print函数
template <typename T>
void print(T&& t) { //B函数，右值引用
    std::cout << "right_reference：" << t << std::endl;
}

template <typename T>
void task(T&& v) { //未定义的引用类型，需要根据传参来实时判断
    print(v); //（1）
    print(std::move(v)); //（2）
    print(std::forward<T>(v)); //（3）
    std::cout << std::endl;
}

int main() {
    //task(8); //Ⅰ
    int num = 1024;
    //task(num); //Ⅱ
    //task(std::forward<int>(num)); //Ⅲ
    //task(std::forward<int&>(num)); //Ⅳ
    task(std::forward<int&&>(num)); //Ⅴ
}
/*
 Ⅰ：8是个纯右值，进入task函数，未定义的引用类型推导出来为右值引用
 （1）：右值引用发生传递变为左值引用，进入A函数
 （2）：右值引用发生传递变成左值引用后，move成右值，右值引用进入B函数
 （3）：forward<T>的类型是T，是右值，forward将右值传递给B函数
 
 Ⅱ：num可取地址，是左值进入task函数，未定义的引用类型推导出来为左值引用
 （1）：毫无疑问左值，由A函数执行
 （2）：左值move为右值，由B函数执行
 （3）：T为左值类型，forward成左值引用推出去，进入A函数
 
 Ⅲ：std::forward<int>(num)，模板类型是int，forward中不是左值引用，就会被推导成右值
 （1）：右值引用发生传递，变成左值，进入A函数
 （2）：右值引用传递变成左值，再move成右值，进入B函数
 （3）：T是右值类型，完美转发成右值引用，进入B函数
 
 Ⅳ：std::forward<int&>(num)，模板类型是int&，forward中是左值引用就会被推导成左值
 （1）：左值，进入A函数
 （2）：左值move成右值，进入B函数
 （3）：T为左值类型，forward转发成左值引用，进入A函数
 
 Ⅴ：std::forward<int&&>(num)，模板类型是int&&，forward中不是左值引用则被推导成右值
 （1）：右值引用，发生传递变成左值，进入A函数
 （2）：右值引用发生传递变成左值，再move成右值，进入B函数
 （3）：T为右值类型，forward转发为右值引用，进入B函数
 */
```

## 二级指针

矩阵举例

```cpp
#include <iostream>
class Matrix {
public:
    Matrix(int _row, int _col): row(_row), col(_col) {
        data = new float*[row]; //申请row行，类型匹配：data是二级指针，new需要再配一个指针
        for (int i = 0; i < row; i++) { //遍历行
            data[i] = new float[col]; //类型匹配：data[i]是一级指针，直接new出来就是一个一级指针
            for (int j = 0; j < col; j++) {
                data[i][j] = 0; //初始化为0
            }
        }
    }
    Matrix(const Matrix& mx) {
        row = mx.row;
        col = mx.col;
        data = new float*[row]; //类型：二级指针
        for (int i = 0; i < row; i++) {
            data[i] = new float[col]; //类型：一级指针
            for (int j = 0; j < col; j++) {
                data[i][j] = mx.data[i][j]; //拷贝
            }
        }
    }
    ~Matrix() {
        if (nullptr != data) {
            for (int i = 0; i < row; i++) {
                if (data[i]) {
                    delete[] data[i];
                    data[i] = nullptr; //释放二级指针
                }
            }
            delete[] data;
            data = nullptr; //释放一级指针
        }
    }
private:
    int row; //行
    int col; //列
    float** data = nullptr; //二维数组
};
```

## 四大类型转换

- static_cast，用于基类和派生类之间，指针或引用的转换
  	- 上行转换（将派生类指针/引用转换为基类指针/引用）是安全的，相当于将哈士奇退化成犬科是安全的
  	- 下行转换（将基类指针/引用转换为派生类指针/引用）不安全的，相当于将犬类都“进化”成哈士奇很不安全
  	- 把 void* 转换成目标类型的指针是不安全的，相当于缩小圈子搞进化是不安全的
  	- 把任何类型的表达式转为void类型是可以的
- const_cast，去除类型的 const 或 volatile 属性

```cpp
const char* p;
char* l = const_cast<char*>(p); //正确
```

- dynamic_cast，继承体系中的安全向下转型相当于对static_cast的补充
	- 可以安全的将基类指针/引用转为子类指针/引用，转型失败就返回NULL
 	- 只有在基类带有虚函数的情况下才允许转为派生类
- reinterpret_cast，比较依赖于机器，本身也很少使用 

## 内存分区

- 堆区：调用new函数在堆区分配内存
- 栈区：存储函数的返回地址、参数、局部变量、返回值，程序结束后自动释放
- 全局静态区：程序编译就已经存在，生命周期为整个程序，存放静态和全局变量
- 常量储存区：存放常量字符串和其他类型的常量，不允许修改。虚函数表vtable就存放在此，具体是只读数据段
- 代码区：存放编译后的二进制代码

## 指针函数 & 函数指针

#### 指针函数

本质是一个函数，返回值是指针，格式如： `reference* func(args...);`

func是函数，reference*作为一个整体，是func的返回值。args...是形参列表。

#### 函数指针

本质是一个指针，指向一个函数，所以它是指向函数的指针。我们知道，函数的定义位于代码段，所以每个函数在代码段中都有一个入口地址，函数指针就是指向代码段中函数入口地址的指针

格式如： `reference (*p)(args...);`

reference为返回值，*p是一个整体，代表指向该函数的指针，单独的p为函数指针变量。args...是形参列表。

函数指针的初始化：函数指针变量 = 函数名

举个例子：

```cpp
int max(int a, int b) {
    return a > b ? a : b;
}
int main() {
    int (*p)(int, int); //函数指针的定义
    p = &max; //函数指针初始化
    //或者直接p = max，因为函数名本身就代表函数的地址
    int ans = p(10, 7); //函数指针的调用
    std::cout << ans << std::endl;
}
```

#### 回调函数

函数指针的典型应用就是回调函数，原理是将函数指针作为参数传递给另一个函数。

回调函数相当于一个中断处理函数，由系统在符合你设定的条件时自动调用。用户只要做三件事：声明+定义+设置触发条件



```cpp
int sum(int a, int b) {
    return a + b;
}
//第三个参数是函数指针，通过该函数指针调用sum函数
int callback(int m, int n, int (*p)(int, int)) {
    return p(m, n);
}
int main() {
    std::cout << callback(3, 2, sum) << std::endl;
}
```

回调函数不关心sum具体如何实现，只需要去调用即可，假如以后代码迭代，只需要修改sum而无需动callback函数

## 严禁返回局部变量的指针和引用

因为局部变量在函数的栈区中分配内存，当函数执行完毕，栈区内存自动回收，包括其中的局部变量。假如这时返回了局部变量的引用（或者说该局部变量在栈上的地址），该地址指向的内存区域已经被释放了，所以它是一个无效的引用，指向一片未定义的内存，有很大的安全隐患

## new/delete 和 malloc/free

- malloc需要手动计算分配空间大小，new不需要
- malloc/free是库函数，不支持重载；new/delete是操作符，支持运算符重载
- new是类型安全的，malloc不是
- new分配成功返回类型指针，不需要转换；malloc分配成功返回void*，需要强制类型转换
- new分配内存失败会抛bac_alloc异常，malloc分配失败返回nullptr
- malloc仅分配内存空间，free仅回收空间；new和delete除了分配和释放外，还能调用构造函数和析构函数
- new底层调用 operator new 分配空间并执行构造函数，而operator new又封装了malloc。delele先运行析构函数，之后运行operator delete

### free函数只接收一个内存地址，它是如何知道要分配多大内存？

malloc返回的起始地址比空间地址多了16字节，这16字节保存了内存块描述信息，其中就包括了内存块大小

执行free时，free将传入的地址向左偏移16字节，进而分析出内存块大小，就知道要释放多大的内存了

## 堆 和 栈

![堆vs栈](https://github.com/arqady01/Cpp-interface/blob/main/resource/cpp_images/hoop.png)

生命周期对比

- 栈

```cpp
class complex {};
class Other {
	complex c1; //c1就是栈对象，其生命周期在作用域结束后自动结束
};
```

```cpp
class complex {};
int main() {
	static complex c2; //c2就是静态栈对象，作用域结束后仍然存活，直到整个程序结束
}
```

```cpp
class complex {};
complex c3; //c3就是全局对象，生命在整个程序结束后才释放
int main() { }
```

- 堆

```cpp
class complex {};
int main() {
	complex* p = new complex;
	......
	delete p;
}
```

p指向的就是堆对象，生命周期在被delete后结束。

如果不delete就会发生内存泄漏，因为当作用域结束，p所指的堆对象仍然存在，但指针p生命周期却结束了，作用域之外再也看不到指针p，也没机会delete p了

## 堆栈溢出常见原因

- 函数递归调用时，栈中会不断保存函数调用时产生的变量，如果递归调用太深，栈无法容纳函数的返回地址，从而栈溢出
- 动态申请空间后未释放。申请的动态空间使用的是堆空间，动态空间使用不当造成堆溢出。
- 数组访问越界
- 指针非法访问

## 内存泄漏

通常指堆内存的泄漏。使用malloc、new等函数从堆中分配到块内存，使用完后，程序必须调用free或delete释放该内存块，否则，这块内存就不能被再次使用，即内存泄漏了

- 忘记释放内存
- 未释放内存再次申请

```cpp
int *ptr = new int;
ptr = new int; //原来的内存泄漏
```

- 静态变量导致内存泄漏
- 共享型智能指针循环引用

如何解决或预防？

- 使用[智能指针](https://github.com/arqady01/Cpp-interface/blob/main/src/smart%20pointer/smart_pointer.md)
- 使用shared_ptr时，防止循环引用，即A对象持有B对象的智能指针，同时B对象也持有A的智能指针，这样会导致引用计数无法减为零造成内存泄漏
- 养成良好的编程规范，让new和delete，或者malloc和free成对出现
- 定义一个全局的变量，分配内存时变量加一，析构时变量减一，程序结束时若变量不为零说明内存泄漏
- 可以使用宏和预定义宏来定位内存泄漏。通过定义一个名为DEBUG_NEW的宏，并结合预定义的__FILE__和__LINE__宏，可以跟踪内存泄漏发生的文件和行号，从而帮助定位
- OOP时，基类的析构函数声明为虚函数
- 数组对象的释放需要用delete[]，让数组从后往前一一释放
- 借助Valgrind工具来帮助识别内存泄漏

## 内存对齐

内存对齐是指在为结构体的成员分配内存时，确保每个成员相对于结构体起始地址的偏移量是该成员类型大小的整数倍

内存对齐是为了提高内存访问效率，因为大多数硬件平台访问对齐的内存数据时速度更快。

- 结构体的起始地址对齐于最宽基本类型成员的大小
- 结构体每个成员相对于结构体起始地址的偏移量应该是该成员类型大小的最小整数倍
- 结构体的总大小是其最宽基本类型成员大小的整数倍。如果不是，则会在结构体的最后添加填充字节

有些编译器允许通过特定的编译指令或者#pragma指令来修改默认的对齐方式。例如，在GCC中可以使用`__attribute__((packed))`来告诉编译器不对结构体进行填充对齐。

C++11引入了`alignas`关键字，允许指定变量或结构体成员的对齐要求

## std::function

函数包装器支持四种类型

- 普通函数
- 匿名函数
- 成员函数
- 仿函数

1.普通函数

```cpp
#include <functional>
#include <iostream>
int add(int x, int y) {
	return x + y;
}
int main() {
	std::function<int(int, int)> f = &add; // "=add" is okay
	std::cout << "两数相加等于" << f(3.14, 2) << std::endl;
}
```

2.匿名函数

```cpp
int main() {
	std::function<int(int, int)> fu = [](int a, int b)->int {
		return a + b;
	};
	std::cout << fu(8, 12) << std::endl;
}
```

3.成员函数

```cpp
#include <functional>
#include <iostream>
#include <string>
class test {
public:
	test() = default;
	std::string addString(std::string str, int x) {
		return str + std::to_string(x);
	}
};
int main() {
	//test::addString()需要取地址，唯一的变化就是需要将类的this指针传递给包装器
	std::function<std::string(test*, std::string, int)> fun = &test::addString;
	test t; //实例化
	std::cout << fun(&t, "Messi", 10);
}
```

4.仿函数

```cpp
class test {
public:
	test() = default;
	//因为返回的对象还未构造，所以返回值类型不能是引用
	std::string operator()(std::string str, int x) {
		return str.append(std::to_string(x));
	}
};
int main() {
	std::function<std::string(test*, std::string, int)> func = &test::operator();
	test t;
	std::cout << func(&t, "messi", 10) << std::endl;
}
```

### bind机制

```cpp
int append(int a, int b, int c) {
	return a * 100 + b * 10 + c;
}
int main() {
	auto add = std::bind(append, 3, 4, 5);
	std::cout << add() << std::endl;
}
```

## 排序算法

### 冒泡排序

假设数组长度为 n，从大到小排序举例：

比较相邻的前后两个数据，若前面的数据大于后面，就交换二者

外循环是比较的轮次数，每比较一轮，就能确定出最大的一个数字，比如n为5，只需比较4轮就能就能确定最大的4个数字

内循环是每趟需要比较的次数，比如第 i 躺，要从前往后比较 n - i 次

```cpp
#include <iostream>
void swap(int* x, int* y) {
    int temp = *x;
    *x = *y;
    *y = temp;
}
void bubble(int arr[], int n) {
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - i - 1; ++j) {
            if (arr[j] > arr[j + 1]) {
                swap(&arr[j], &arr[j + 1]);
            }
        }
    }
}
int main() {
    int nums[5] = {5,4,3,2,1};
    bubble(nums, 5);
    for (auto i : nums) {
        std::cout << i << " ";
    }
}
```

### 选择排序

不稳定算法：举个例子，序列 5，8，5，2，9，第一次排序中，5和第四个元素2交换，那么原序列中两个5的相对顺序就破坏了

1. 一开始将第一个元素当作已排序的序列
2. 在未排序序列中找到最小元素，插入到已排序序列的起始位置
3. 从剩余未排序元素中继续寻找最小元素，然后放到已排序序列的末尾
4. 以此类推，直到所有元素排序完毕

时间复杂度O(n2)，不稳定排序，原地排序

```cpp
#include <iostream>
#include <algorithm>
void swap(int* x, int* y) {
    int temp = *x;
    *x = *y;
    *y = temp;
}
//n：数组长度
void selectSort(int arr[], int n) {
    int minIndex = 0; //未排序序列中的最小元素的下标
    for (int i = 0; i < n; i++) {
        minIndex = i; //假设最小元素的下标就是未排序序列的起始元素下标
        for (int j = i + 1; j < n; j++) {
            if (arr[j] < arr[minIndex]) {
                minIndex = j; //记录下表，不做实际的交换动作
            }
        }
        swap(&arr[i], &arr[minIndex]);
    }
}
int main() {
    int nums[5] = {5,4,3,2,1};
    selectSort(nums, 5);
    for (auto i : nums) {
        std::cout << i << " ";
    }
}
```

### 插入排序

```cpp
#include <iostream>
//n：数组长度
void insertSort(int arr[], int n) {
    for (int i = 1; i < n; i++) {
        if (arr[i - 1] >arr[i]) {
            int temp = arr[i];
            int j = 0;
            for (j = i; j >= 0 && arr[j - 1] > temp; j--) {
                arr[j] = arr[j - 1];
            }
            arr[j] = temp;
        }
    }
}
int main() {
    int nums[5] = {5,4,3,2,1};
    insertSort(nums, 5);
    for (auto i : nums) {
        std::cout << i << " ";
    }
}
```

### 快速排序【难理解就直接记住吧】

因为排序的时候相当于二叉树，排序的趟数相当于二叉树的层数log2n + 1；最外层包着一层循环时间复杂度为O(n)；时间复杂度为O(nlog2n)

步骤：

1. 将分组的第一个元素作为枢轴，比枢轴大的元素放到右边，比枢轴小的元素放到左边
2. 对左子表重复第一步
3. 对右子表重复第一步，直到 left 不再< right，也就是只剩下一个元素，完毕

```cpp
#include <iostream>
//快排辅助函数，将数组划分为2部分
int partition(int arr[], int left, int right) {
    int pivot = arr[left]; //枢轴元素
    while (left < right) {
        while (left < right && arr[right] >= pivot)
            --right; //如果右半部分元素大于枢轴，不需要移动，移动指针去比较下一个元素
        arr[left] = arr[right]; //在右半边找到小于枢轴的元素，交换
        while (left < right && arr[left] < pivot)
            ++left;
        arr[right] = arr[left];
    }
    arr[left] = pivot; //枢轴元素存放到最终位置
    return left;
}
//快排函数
void quickSort(int nums[], int left, int right) {
    if (left < right) {
        int pos = partition(nums, left, right);
        quickSort(nums, left, pos - 1); //划分左子表
        quickSort(nums, pos + 1, right); //划分右子表
    }
}
int main() {
    int nums[5] = {5,4,3,2,1};
    quickSort(nums, 0, 4);
    for (auto i : nums) {
        std::cout << i << " ";
    }
}
```

### 堆排序

堆是一棵完全二叉树，并且父节点的值大于孩子节点

因为是完全二叉树，就能将二叉树的节点坐标和数组映射上，比如二叉树：5-8-10-3-4-6-7-1-2，他的坐标为0~8，假设一节点的序号为i，那么：

```
parent = (i - 1) / 2;
child1 = 2 * i + 1;
child2 = 2 * i + 2;
```

介绍一下heapify()操作，它接收一个元素地址，递归的将此元素的子孩子中最大的节点挪到当前位置，如图：

<p align="center"> 
    <img src="https://github.com/arqady01/Cpp-interface/blob/main/resource/cpp_images/heapify.jpg" style="width:90%;">
</p>

具体代码：

```cpp
#include <iostream>
//交换操作
void swap(int* x, int* y) {
  int temp = *x;
  *x = *y;
  *y = temp;
}
//左子节点和右子节点轮流和当前节点i对比，取最大
int max(int arr[], int n, int i) {
  int max = i; //假设i处是最大的
  int child1 = 2 * i + 1; //左子节点的坐标
  int child2 = 2 * i + 2; //右子节点的坐标  
  if (child1 < n && arr[child1] > arr[max]) max = child1;
  if (child2 < n && arr[child2] > arr[max]) max = child2;
  return max;
}
//递归实现，将i号节点及它的子树变成大根堆
void heapify(int arr[], int n, int i) {
  if (i >= n) return; //递归退出条件
  int maximum = max(arr, n, i); //注意maximum是最大值的下标
  if (maximum != i) { //只有最大值不是节点i自己，
    swap(&arr[maximum], &arr[i]); //才需要将最大值移动到根节点
    heapify(arr, n, maximum); //然后递归
  }
}
//建造大根堆
void build_heap(int arr[], int n) {
  int last_node = (n - 1);
  int last_node_parent = (last_node - 1) / 2;
  for (int i = last_node_parent; i >= 0; i--) {
    heapify(arr, n, i);
  }
}
//最终排序操作
void sort(int arr[], int n) {
  //在大根堆中，根节点的值无疑是最大的，交换根节点和最后的叶子节点
  for (int i = n - 1; i >= 0; i--) {
    swap(&arr[0], &arr[i]);
    //i会不断减少，意思就是最后的节点不用再纳入heapify()，但是它并未从堆中删除
    heapify(arr, i, 0); //而且是对根节点开始做heapify()
  }
}
int main() {
  int nums[9]  = {5,8,10,3,4,6,7,1,2};
  // heapify(nums, 9, 0); //测试heapify操作
  build_heap(nums, 9); //建立大根堆
  sort(nums, 9);
  for (auto i : nums) std::cout << i << " ";
  std::cout << std::endl;
}
```

[leetcode的堆排序](https://leetcode.cn/problems/sort-an-array/description/)

<h1 id="template">📡 模板元编程</h1>

## std::decay

去除模板类型T的所有修饰，比如const、引用等

```cpp
int main() {
    int a = 10;
    int& b = a;
    std::decay<decltype(b)>::type as = 8;
}
```

## 固定萃取

给定一个类型，萃取另外一种类型，因为关系固定（模板特化），所以也称固定萃取。因为array2被推断为int，导致数据丢失，所以需要指定一个模板参数U让其：

```cpp
#include <iostream>
template <typename U, typename T>
U sum(const T* begin, const T* end) {
    U sum{}; //零初始化，如果整型变量就初始化为0，指针型初始化为nullptr，bool型初始化false....
    while (begin <= end) {
        sum += *begin;
        begin++;
    }
    return sum;
}
int main() {
    int array1[] = { 30, 40, 50 };
    char array2[] = "abc"; //97,98,99
    std::cout << sum<int>(array1, &array1[2]) << std::endl;
    std::cout << sum<int>(array2, &array2[2]) << std::endl;
}
```

还是不通用，可利用模板中的萃取：

```cpp
//traits模板泛化
template <typename T>
struct sumTraits;

//traits模板特化
template <>
struct sumTraits<int> {
    using sumT = int;
};
template <>
struct sumTraits<char> {
    using sumT = int;
};

template <typename T>
auto sum(const T* begin, const T* end) {
    using my_sumT = typename sumTraits<T>::sumT;
    my_sumT sum{};
    while (begin <= end) {
        sum += *begin;
        begin++;
    }
    return sum;
}
int main() {
    int array1[] = { 30, 40, 50 };
    char array2[] = "abc"; //97,98,99
    std::cout << sum(array1, &array1[2]) << std::endl;
    std::cout << sum(array2, &array2[2]) << std::endl;
}
```

### 例子2

```
template<typename T>
struct GetElem {};
template<>
struct GetElem<char> {
    using type = char;
};
template<>
struct GetElem<int> {
    using type = int;
};
template<>
struct GetElem<double> {
    using type = double;
};
template<typename T, std::size_t n>
struct GetElem<T[n]> {
    using type = T;
    static constexpr size_t size = n;
};
int main() {
    std::cout << typeid(GetElem<double>::type).name() << std::endl;
    std::cout << GetElem<int[10]>::size << std::endl;
}
```

## 模板递归

模板元编程的好处之一就是可以直接在编译期执行

```cpp
//计算n的累和
#include <iostream>
//泛化
template <int n>
struct sum {
    static const int val = sum<n - 1>::val + n;
};
//特化
template <>
struct sum<1> {
    static const int val = 1;
};
int main() {
    std::cout << sum<100>::val << std::endl;
}
```

## constexpr

```cpp
#include <iostream>
constexpr auto sum(int n) {
    if (1 == n) {
        return n;
    }
    return n + sum(n - 1);
}
int main() {
    constexpr auto ans = sum(100); //(1)
    sum(100);
}
```

可以在编译期执行就在编译期执行(1)，不能的话就放到运行期执行：

```cpp
int args = 10;
auto ans = sum(args);
```

<h1 id="STL">🧭 STL</h1>

容器 | 内部数据结构 | 查找的O(n) | 增删的O(n) | 有序情况 | 能否重复 | 内存动态分析 | 其他备注
---|---|---|---|---|---|---|---
array|数组|随机读改O(1)|无法增删|无序|可重复|大小在编译期就已确定不可更改|无法增加删除；修改元素通过替换完成
vector|数组|随机访问，O(1)|O(n)|无序|可重复|动态增加或减少元素，reserve()可以预分配内存，避免内存多次分配|缩小内存用swap
deque|不连续的数组块映射在内存数组中|随机访问O(1)|O(n)|无序|可重复|动态完成，不提供内存管理的成员函数|中央控制器 + 不连续内存块，支持首尾快速增删，支持随机访问
queue|deque|无迭代器，不可随机访问|只允许尾部插入、头部删除|无序|可重复||适配器，先进先出
stack|deque|无迭代器，不可随机访问|只允许在头部增加/删除|无序|可重复||适配器，先进后出
forward_list|单向链表|O(n)|O(1)|无序|可重复|动态增减或减少|不支持随机访问
list|双向链表|O(n)|O(1)|无序|可重复|动态增减或减少|不支持随机访问，可双向遍历
set|红黑树|O(logn)|O(logn)|有序|不可重复||不支持元素直接修改，会破坏其排序和唯一性，想修改必须先删除旧元素再插入新元素|
multiset|红黑树|O(logn)|O(logn)|有序|可重复|||
map|红黑树|O(logn)|O(logn)|有序|不可重复||①key唯一②operator[]可以更改元素的值③insert插入键值对，operator[]在key不存在时也可以插入pair|
multimap|红黑树|O(logn)|O(logn)|有序|可重复|||

容器 | 内部数据结构 | 查找的O(n) | 增删的O(n) | 有序情况 | 能否重复
---|---|---|---|---|---
unordered_set|哈希表|O(1)、若哈希碰撞退化为(n)|同前|无序|不可重复|
unordered_multiset|哈希表|O(1)、若哈希碰撞退化为(n)|同前|无序|可重复|
unordered_map|哈希表|O(1)、若哈希碰撞退化为(n)|同前|无序|不可重复|
unordered_multimap|哈希表|O(1)、若哈希碰撞退化为(n)|同前|无序|可重复|

## 迭代器失效情况分析

### vector

- 尾插（push_back | emplace_back）
    - 若无需内存重分配，那么已有元素的迭代器、指针和引用不会失效
    - 若尾插导致扩容，所有元素的迭代器、指针和引用都将失效
- 插入（insert）
    - 若无需内存重分配，则插入点之前元素的迭代器依旧有效，而插入点及之后元素的迭代器均会失效
    - 若插入操作导致内存重分配，扩容并将旧元素复制过去并删除，那么所有迭代器、指针和引用都会失效
- 删除（erase | pop_back）
    - 删除元素不会导致内存重分配，但被删除元素及之后的元素都会向前移动，所以被删除元素和之后元素的迭代器将失效，删除元素之前的不受影响
- 清空操作（clear）
    - 清空所有元素后，所有指向元素的迭代器、指针和引用都将失效，但vector的容量不会改变

不提供减少容量或释放多余内存的机制。如果需要缩小 vector 的内存，可以使用“swap trick”技术，即创建一个新的 vector 并与原来的 vector 交换内容，这样新的 vector 将具有刚好匹配当前大小的容量

### deque

- 插入
    - 在中间位置插入元素，涉及到移动元素，插入点及之后的所有迭代器均失效
    - 在头部插入元素，除begin()外，其他迭代器不会失效，因为支持双端操作
    - 在尾部插入元素，除end()外，其他迭代器不会失效，其实很好验证的
- 删除
    - 在中间或尾部删除元素，所有指向被删除元素及之后的迭代器都失效
    - 头部删除元素，所有迭代器依旧有效，除begin()迭代器外
    - 尾部删除元素，其余迭代器依旧有效，除end()迭代器外
- 清空
    - 全部失效
    
### list

- 插入
    - 插入元素任何迭代器都不会失效，新元素将被插入到现有元素之间，不会发生元素的移动和复制
- 删除（erase）
    - 被删除元素的迭代器失效，其他迭代器不受影响
- 修改
    - 不受影响
- 清空（clear）
    - 所有迭代器均失效
    
### set / multiset

- 插入（insert）
    - 任何迭代器都不受影响
- 删除（erase）
    - 被删除元素的迭代器失效，其他元素的迭代器不受影响
- 修改
    - 不支持元素直接修改（会破坏内部顺序），因此不存在因为直接修改元素导致迭代器失效问题
    - 如果需要修改元素，必须先删除旧元素再插入新元素。如此，被删除元素的迭代器失效

## vector

成员方法|解释
---|---
operator=|将新内容拷贝给容器，并修改容器大小，但capacity()不会缩小
swap|交换容器的内容，注意swap不接受临时对象（右值），但是可以用临时对象去调用swap
begin|返回容器中第一个元素的迭代器
end|返回容器中最后一个元素的后一位的迭代器
size|返回容器中元素的数量
capacity|返回容器的容量
empty|判断vector是否为空
resize|改变容器元素的数量
reserve|改变容器的最大容量
shrink_to_fit()|尝试减少内存的浪费，通过减少容器容量以匹配元素个数
operator[]|返回容器中第n个位置的元素，注意不是返回一个迭代器
at()|作用和operator[]相同，但是at提供边界检查，operator[]不提供
front()|返回容器中第一个元素
back()|返回容器中最后一个元素
push_back|构造一个临时对象并调用拷贝构造函数将其尾插到容器中
emplace_back|在容器尾部原地构造
pop_back|删除容器中的最后一个元素
insert(pos, vals)|在pos处插入一个或多个对象，原先pos处这里的迭代器及之后所有的迭代器都会失效
erase|从删除单个元素（`pos`）或一系列元素（`[first，last)`）
clear|删除所有元素，元素个数会变为0，但容量不会变
emplace(pos, vals)|在pos处插入一个或多个对象

### emplace_back 和 push_back 的区别

假如用push_back给vector插入临时对象（右值）时，首先会调用ctor构造这个临时对象，然后调用copy ctor将临时对象插入vector中，原先的临时变量释放，这太繁琐了。

而emplace_back会在插入时在vector的末尾原地构造，无需先创建临时对象然后再复制或移动到容器中

如果正在添加的元素需要通过构造函数创建，并且希望避免额外的复制或移动开销，使用 emplace_back。如果只是简单地添加已存在的对象副本，push_back 和 emplace_back 的性能差异不大

### insert 和 emplace 的区别

insert在vector中指定位置插入一个或多个元素，通过拷贝或者移动已经存在的对象来插入元素

emplace在指定位置直接构造元素，不需要创建临时对象，避免了额外的拷贝或移动操作

### resize 和 reserver 的区别

resize改变容器含有元素的数量

```cpp
std::vector<int> vec = {1,2,3,4,5};
//如果调整大小大于原先元素数量，就补充n个0，size()和capacity()都会增加
vec.resize(10);
std::cout << vec.size() << " " << vec.capacity() << std::endl; //10 10

//如果调整元素小于原先元素数量，就删除n个元素，size()减少，但capacity()不会减小
vec.resize(1);
std::cout << vec.size() << " " << vec.capacity() << std::endl; //1 5
```

reserver改变容器的最大容量capacity，不会生成元素，就算往小了改capacity()都不会缩小

```cpp
std::vector<int> vec = {1,2,3,4,5};
vec.reserve(10);
std::cout << vec.size() << " " << vec.capacity() << std::endl; //5 10
vec.reserve(1);
std::cout << vec.size() << " " << vec.capacity() << std::endl; //5 5
```

### vector的内存释放

C++ Primer说，为了支持快速的随机访问，vector的元素以连续的方式存放，每一个元素都紧挨着前一个元素存储，每当插入新元素，为了连续存放，都要重新分配内存、拷贝元素、撤销旧空间，性能无法保障

所以分配的时候就会多预留空间，当然也可以通过reserve()指定预留空间大小。

但是vector的内存空间只会增加不会减少（就算用reverse和resize去特意减少capacity容量也于事无补）

因为内存只增不减，假设reserve预分配了100个字节，就算erase擦出了99个，但内存占用依旧是100字节，就算clear清空所有元素，依旧是100，只有在vector析构的时候，内存才会被回收

想要内存空间动态缩小，除了使用deque外，vector可以借助swap函数来清空自己

```cpp
    vector<int> v{10,20,50};
    vector<int>().swap(v);
    cout << v.capacity(); //0，并且v中元素被清空
```

因为vector中有三个指针， begin、end和end_of_storage，用一个临时变量`vector<int>()`调用swap去和v交换，指针交换了个，自然被清空

## deque

成员方法|解释
---|---
push_back|在容器的末尾添加一个元素
push_front|在容器的开头添加一个元素
pop_back|删除容器的最后一个元素
pop_front|删除容器的第一个元素
emplace_front|容器的开头插入一个新元素
emplace_back|容器的末尾插入一个新元素

## list

list，双向链表，是序列容器，允许在序列中的任何地方进行常数时间插入和擦除操作，并在两个方向上进行迭代。

## stack

stack 是一种容器适配器，用于在LIFO（后进先出）的操作，其中元素仅从容器的一端插入和提取。

## queue

queue 是一种容器适配器，用于在FIFO（先入先出）的操作，其中元素插入到容器的一端并从另一端提取。

## priority_queue

## set

set 是按照特定顺序存储唯一元素的容器。

## multiset

## map

成员方法|解释
---|---
begin|返回引用容器中第一个元素的迭代器
find|在容器中搜索具有等于 k（参数）的键的元素，如果找到则返回一个迭代器，否则返回 map::end 的迭代器
count|在容器中搜索具有等于 k（参数）的键的元素，并返回匹配的数量
lower_bound|返回一个非递减序列 `[first, last)`（参数）中的第一个大于等于值 val（参数）的位置的迭代器
upper_bound|返回一个非递减序列 `[first, last)`（参数）中第一个大于 val（参数）的位置的迭代器
equal_range|获取相同元素的范围，返回包含容器中所有具有与 k（参数）等价的键的元素的范围边界（`pair< map<char,int>::iterator, map<char,int>::iterator >`）

## multimap

## unordered\_set

## unordered\_multiset

## unordered\_map

## unordered\_multimap

## tuple

元组是一个能够容纳元素集合的对象。每个元素可以是不同的类型。

## pair

这个类把一对值（values）结合在一起，这些值可能是不同的类型（T1 和 T2）。每个值可以被公有的成员变量first、second访问

<h1 id="multithread">🎡 多线程</h1>

[C++多线程](https://github.com/arqady01/Cpp-interface/blob/main/src/multithread/cpp.md)

[posix多线程](https://github.com/arqady01/Cpp-interface/blob/main/src/multithread/posix.md)

## 死锁

死锁的必要条件：

- 互斥条件：某种资源一次只允许一个进程访问，即该资源一旦分配给某个进程，其他进程就不能再访问，直到该进程访问结束
- 占有且等待：进程已占有至少一个资源，但又提出了新的资源请求，而该资源又被其它进程占有，此时进程阻塞，但又对自己已获得的资源保持不放
- 不剥夺条件：进程已获得的资源，在未使用完之前，不能被剥夺，只能在使用完时由自己释放
- 环路等待：在发生死锁时，必然存在一个资源的环形链，即进程集合{P0，P1，P2，…，Pn}中的P0正在等待一个P1占用的资源； P1正在等待P2占用的资源，……，Pn正在等待已被P0占用的资源

举个例子，线程A执行时，先锁mutex_A，上锁成功后去锁mutex_B的时候，发生了上下文切换

线程B执行，先锁mutex_B，因为mutex_B未被锁，所以可以上锁成功，然后线程B要去锁mutex_A

此时死锁产生，线程A占用着mutex_A，需要锁mutex_B，线程B占用着mutex_B，需要锁mutex_A，两个线程无法进行下去

产生死锁的原因可归纳为两点：

- 竞争资源：当系统中供多个进程共享的资源如打印机等，数目不足以满足各进程的需要时，会引起进程对资源的竞争而产生死锁
- 进程间推进顺序非法：进程在运行过程中，请求和释放资源的顺序不当，也会导致进程死锁

#### 解决方案

- 预防死锁：资源有序分配
    - 避免多次锁定
    - 互斥锁对互斥量访问完毕后，一定要解锁
    - 让所有线程对互斥锁的访问顺序一致（但有时会不靠谱，比如有的线程就必须做到先访问a再访问b）
- 避免死锁
    - 银行家算法：在线程请求资源时，先确定资源分配后系统是否还能保持安全状态，若能，则分配资源；否则，让进程等待
    - 资源请求的预先检查：进程在运行前声明其最大资源需求量，系统根据此来避免死锁的发生。
- 检测死锁
- 解除死锁
    - 终止线程
    - 资源抢占：从其他线程抢占所需资源，分配给死锁线程，以解除死锁
    - 回滚：让线程回退到足以打破循环等待的安全状态，然后重新启动这些线程

## 进程有哪些状态

- 运行状态：进程占用着CPU
- 就绪状态：万事俱备，只欠东风，进程已经获得了除处理机之外的一切资源，一旦得到处理机就可以运行
- 阻塞状态：进程正在等待某一事件而暂停运行，除非某种外部事件发生，否则阻塞态的进程不能运行

## 进程终止触发条件

- 正常退出（自愿）：进程完成了工作正常终止
- 出错退出（自愿）：进程发现了错误而退出
- 严重错误（非自愿）：进程发生了严重错误而不得不退出，例如引用不存在的内存
- 被杀死（非自愿）：调用kill指令，系统杀死线程

## 为什么会出现线程挂起

线程挂起是为了合理利用系统资源，把一个进程从内存转到外存

- 死锁挂起：访问同一资源，但每个进程都在等待对方先放弃资源
- 阻塞挂起：进程在外存并等待某事件的出现
- 就绪挂起：进程在外存，只需要进入内存运行即可

## 进程间通信方式

- 管道：是内核里面的一串缓存，单向通信，只能一端写入另一端读出
- 消息队列：是保存在内核中的消息链表，允许多个进程将消息发送到队列，其他进程从队列中接收消息来进行通信
- 共享内存：用一个虚拟地址空间来映射多个物理内存。当某进程写入，其余进程都能看到，数据不需要在进程间不断复制
- 信号量：信号量是一个整型的计数器，实现进程间的互斥与同步
- 套接字
- 信号：进程间通信方式中唯一的异步通信机制

## 进程和线程的区别

- 一个运行的程序至少包含一个进程，一个进程至少包含一个线程
- 进程是操作系统分配资源的基本单位；线程是CPU运算调度的基本单位
- 多个进程之间不能共享资源，每个进程有自己的堆、栈、虚存空间页表、文件描述符等；而线程可以共享进程资源文件（堆和方法区）
- 操纵者不同，一般情况下进程的操纵者是操作系统，线程的操纵者是编程人员
- 上下文切换速度（指从一个线程切换到另一个线程）不同，进程的上下文切换很慢；线程上下文切换速度很快
    - 创建时间更快。进程创建时需要管理资源信息（如内存管理、文件管理信息），而线程创建过程中不会涉及资源管理，而是共享资源
    - 终止时间更快，因为线程释放的资源比进程少很多
    - 切换更快。线程具有相同的地址空间，即同一个进程的线程具有同一个页表，在切换的时候不需要切换页表。而进程的切换要切换页表

## 调度方式

### 线程

1. 抢占式调度

操作系统可随时中断正在执行的线程，并将 CPU 分配给其他线程。

抢占式调度可以通过时钟中断、优先级调度等机制来实现，保证了高优先级任务的及时响应

2. 协作式调度

线程只有主动放弃 CPU 使用权或者发生阻塞事件，操作系统才会切换到其他线程。

协作式调度的优点是简单高效，缺点是一个线程的错误可能导致整个系统的阻塞。

### 进程

1. 先来先服务调度（FCFS）
    - 特点：按照进程到达的先后顺序分配时间片，直到进程执行完成或发生 I/O 操作，是非抢占式调度算法
    - 优点：实现简单，公平性较高
    - 缺点：可能导致某些长作业等待时间过长，不适合对响应时间要求较高的场景
2. 最短作业优先调度（SJF）
    - 优先调度执行时间最短的进程，直到作业完成或发生 I/O 操作
    - 优点：可以最大程度地减少平均等待时间
    - 缺点：难以预测每个进程的执行时间，可能导致长作业等待时间过长
3. 优先级调度
    - 根据进程的优先级分配 CPU 时间片，优先级高的进程先执行
    - 优点：可以根据需求设置不同的优先级
    - 缺点：可能导致低优先级进程长时间等待
4. 轮转调度（RR）
    - 将时间片分配给每个进程，时间片用完或进程完成，就切换和执行下一个进程
    - 优点：可以保证每个进程都能得到执行
    - 缺点：上下文切换开销增大，对于需要长时间运行的进程效率较低
5. 多级反馈队列调度
    - 将进程按照优先级分成多个队列，每个队列使用不同的调度算法。
    - 优点：可以防止某些进程长时间等待或者长时间占用 CPU
    - 缺点：算法复杂，需要根据情况调整参数
6. 实时调度
    - 根据任务的截止时间和优先级进行调度，适用于对响应时间有严格要求的系统
    - 优点：能够保证任务在规定时间内完成
    - 缺点：对系统资源要求较高，需要保证实时性

## 什么是线程安全

如果多线程运行结果是可预期的，而且与单线程的结果一样，那么说明是线程安全的

## 多线程同步与互斥的实现方法

- 互斥锁：进入临界区前，线程必须获取互斥锁；离开时释放互斥锁。这是最基本的同步机制，用于确保多个线程不会同时进入临界区
- 读写锁：读写锁允许更高的并行性，它区分读操作和写操作
    - 多个读操作可以同时进行，因为读操作不会修改数据
    - 写操作是独占的，即当一个线程进行写操作时，其他线程既不能读也不能写
- 条件变量：与互斥锁一起使用，线程在wait()处阻塞，当条件满足时被notify_all()/notify_one()唤醒
- call_once，搭配std::once_flag一起使用，检查标志位是否被改写，有就说明函数被执行过，没有就说明函数未执行。确保函数只执行一次，也就具备互斥量的能力
- 信号量：信号量是一个更通用的同步机制，内部维护一个计数器，每个线程在进入临界区之前会调用P操作（减计数），离开时调用V操作（加计数）
- 原子操作：可以实现无锁编程。C++提供原子操作库，用于执行线程安全的整数或指针操作。

## 锁之大赏

- 互斥锁加锁失败后，线程会释放 CPU ，给其他线程；
- 自旋锁加锁失败后，线程会忙等待，直到它拿到锁；
- 读写锁，由读锁和写锁两部分构成，读写锁适用于能明确区分读操作和写操作的场景
- 乐观锁，假设最好的情况下数据不会发生冲突，所以无需在读写操作时将其上锁。通常通过版本号或时间戳来实现。

## fork()

创建子进程，即一个与调用进程相同的副本

- 复制进程控制块（PCB）：首先复制当前进程的PCB，创建一个新的PCB，用于表示即将创建的子进程
- 分配内存：内核为子进程分配新的内存页面，并将父进程的页面标记为只读。开始时他们有相同的内存内容，但各自有独立的地址空间
- 复制文件描述符：子进程继承父进程所有打开的文件描述符。系统为每个文件描述符创建副本，父子进程就能独立操作各自的文件描述符了
- 设置进程ID： 子进程会被赋予一个新的进程ID（PID），而父进程的PID保持不变。子进程还会获得父进程的PID，以便可以识别父进程
- 返回值：父进程中调用fork()返回子进程的PID，fork()失败则返回-1；在子进程中的返回是0

注意事项

- 使用fork时要注意避免无限制的创建子进程
- 父子进程运行在独立的地址空间中。在fork之后，父子进程中的变量和程序状态是独立的。对某个进程中的变量修改不会影响到另一个进程
- 在多线程中小心使用fork，因为fork调用后只有调用线程被复制到子进程中，其他线程不会被复制。这可能会导致锁等同步机制的问题

## 中断

中断是处理异步事件的一种机制。它允许在正常程序执行过程中，突然转移到另一个任务，然后在适当的时候返回原来的任务。

- 基本概念
    - 中断请求：外部设备（如硬件设备、时钟等）向处理器发送请求，要求处理器暂停当前正在执行的任务，转而处理设备的请求事件
    - 中断向量：每个中断都有一个唯一的标识符，称为中断向量。处理器可以根据中断向量找到对应的中断服务程序的地址。
    - 中断控制器：负责管理和分发各种中断请求的硬件，确保处理器能够正确响应和处理各种中断。
    - 中断服务程序：处理中断的程序，它负责在中断发生时保存当前进程的状态，执行相应的中断处理程序，然后恢复原来的进程状态
- 中断类型
    - 外部中断：由外部设备发起的中断请求，如硬件设备的输入输出请求
    - 内部中断：由处理器内部的错误或异常引起的中断，如除零错误、内存访问错误
    - 软件中断：由软件或程序主动触发的中断，用于实现系统调用或其他软件功能
- 中断的作用
    - 实时响应：允许处理器实时响应外部设备的请求，处理实时性要求高的任务
    - 提高效率：通过允许处理器在等待外部事件时执行其他任务，提高系统的效率和并发性
    - 异常处理：用于处理处理器内部和外部设备发生的异常情况，确保系统的稳定性和可靠性

<h1 id="effectivecpp">☁️ Effective C++</h1>

1. 视 C++ 为一个语言联邦（C、Object-Oriented C++、Template C++、STL）
2. 宁可以编译器替换预处理器
    - 尽量以 `const`、`enum`、`inline` 替换 `#define`
3. [尽可能使用 const](https://github.com/arqady01/Cpp-interface/blob/main/src/Effective%20C%2B%2B/03.%E5%B0%BD%E5%8F%AF%E8%83%BD%E4%BD%BF%E7%94%A8const)
4. 确定对象被使用前已先被初始化

<p align="center"> 
    <img src="https://github.com/arqady01/Cpp-interface/blob/main/src/Effective%20C%2B%2B/04.png" style="width:55%;">
</p>

5. 了解 C++ 默默编写并调用哪些函数
    - 编译器暗自为类创建ctor、copy ctor、copy assignment、dtor
6. 若不想使用编译器自动生成的函数，就应该明确拒绝
    - C++11：使用delete关键词
    - C++98：将不想使用的成员函数声明为 private，并且不予实现
7. 为多态基类声明 virtual 析构函数

```cpp
将基类的析构函数设为虚函数，各个派生类的析构函数在其类中具体实现

析构顺序：派生类 -> 基类

⛔注意：如果不是为了多态，不要随意将析构函数设为虚函数

虚函数需要通过虚函数表来访问，这意味着每个对象都需要额外的存储空间来存储指向虚函数表的指针。对于不需要多态的类来说，这是一种资源浪费

虚函数调用通常比普通函数调用慢，因为它们需要通过虚函数表间接跳转，这无疑是性能浪费
```

8. 别让异常逃离析构函数
    - dtor可以结束程序，但不应该抛出异常
9. 绝不在构造和析构过程中调用 virtual 函数
    - 对象创建时未完全构建，虚函数表未完全创建好，调用虚函数还是基类的，不会具体到派生类
10. 令 `operator=` 返回一个 `reference to *this`
    - 用于连锁赋值
11. 在 `operator=` 中处理 “自我赋值”
    - 一般来说，如果是自我赋值，就不做任何事
12. 复制对象时勿忘其每一个成分
13. 以对象管理资源
    - 资源在ctor中获得，在dtor释放，建议使用智能指针，资源获取立即初始化（Resource Acquisition Is Initialization，RAII）
14. [在资源管理类中小心 copying 行为](https://github.com/arqady01/Cpp-interface/blob/main/src/Effective%20C%2B%2B/14.%E5%9C%A8%E8%B5%84%E6%BA%90%E7%AE%A1%E7%90%86%E7%B1%BB%E4%B8%AD%E5%B0%8F%E5%BF%83%20copying%20%E8%A1%8C%E4%B8%BA)
15. 在资源管理类中提供对原始资源的访问
    - 对原始资源的访问可能经过显式转换或隐式转换，一般而言显式转换较安全，隐式转换对客户较方便
    - 增加一个get方法获取原始资源的引用或地址
16. 成对使用 new 和 delete 时要采取相同形式
    - delete：释放内存并调用析构函数
    - delete[]：释放内存，并逐一调用array new的对象们的析构函数
17. 以独立语句将 newed 对象置入智能指针

```cpp
应选择No2，因为No1有潜在内存泄漏风险。先 new Widget，接着执行priority()，如果恰好抛出异常程序结束，此时就发生了资源泄漏

int priority();
//No.1
processWidget(std::shared_ptr<Widget>(new Widget), priority());
//No.2
std::shared_ptr<Widget> pw = make_shared<Widget>(new Widget);
processWidget(pw, priority());
```

18. 让接口容易被正确使用，不易被误用

```
# 建立新类型

设计一个 Date(int month, int day, int year) 接口，传参写成了 Date(28, 10, 1999); ,明显就是写倒了月份和天数，这很简单，增加 Day 、month 和 year 类

# 谨防将 == 写成 =

# 使用智能指针
```

19. 设计 class 犹如设计 type，需要考虑对象创建、销毁、初始化、赋值、值传递、合法值、继承关系、转换、一般化等等。
20. 宁以 const引用传递 替换 按值传递
    - 前者通常更高效、避免切割问题，但不适用于内置类型、STL迭代器、函数对象
21. 必须返回对象时，别妄想返回其 reference
    - 不要错误的返回对象的引用/指针

<p align="center"> 
    <img src="https://github.com/arqady01/Cpp-interface/blob/main/src/Effective%20C%2B%2B/21.png" style="width:60%;">
</p>

22. 将成员变量声明为 private
    - 为了封装、一致性、对其读写精确控制等
23. 宁以 non-member、non-friend 替换成员函数

```cpp
class WebBrowser {
    void clearCache();
    void clearCookies();
    //No.1
    void clearEverything(); //call clearCache(), clearCookies()
};
//No.2
void clearEverything(WebBrowser& wb) {
    wb.clearCache();
    wb.clearCookies();
}

推荐第二种方法，non-member 代替 member 函数，增加封装性、包裹弹性、机能扩充性
```

24. 若所有参数皆须要类型转换，请为此采用 non-member 函数
    - 所有参数包括被this指针所指的那个隐喻参数

<p align="center"> 
    <img src="https://github.com/arqady01/Cpp-interface/blob/main/src/Effective%20C%2B%2B/24.png" style="width:70%;">
</p>

25. 考虑写一个不抛异常的 swap 函数
26. 尽可能延后变量定义式的出现时间

<p align="center"> 
    <img src="https://github.com/arqady01/Cpp-interface/blob/main/src/Effective%20C%2B%2B/26.png" style="width:58%;">
</p>

27. 尽量少做转型动作
    - static_cast
    - dynamic_cast
    - const_cast
    - reinterpret_cast
28. 避免使用 handles（包括 引用、指针、迭代器）指向对象内部
    - 以增加封装性、使 const 成员函数的行为更像 const、降低 “虚吊号码牌”（如悬空指针等）的可能性
29. 为 “异常安全” 而努力是值得的
    - 异常安全函数即使发生异常也不会泄露资源或允许任何数据结构败坏
30. 透彻了解 inlining 的里里外外
    - inline 在大多数程序中是编译期的行为
    - 函数是否真正 inline，取决于编译器，它们拒绝太过复杂（如循环或递归）的函数
    - 虚函数的调用会让 inlining 落空，因为虚函数是通过虚函数表实现的，虚表指针在创建对象时设置
    - inline 造成的代码膨胀可能带来效率损失
31. 将文件间的编译依存关系降至最低
    - 如果使用 object references 或 object pointers 可以完成任务，就不要使用 objects
32. 确定public继承塑模出 is-a 关系

<p align="center"> 
    <img src="https://github.com/arqady01/Cpp-interface/blob/main/src/Effective%20C%2B%2B/32.png" style="width:80%;">
</p>

33. 避免遮掩继承而来的名字
    - 可使用 using 声明或转交函数（forwarding）来让被遮掩的名字再见天日
34. 区分接口继承和实现继承
    在公共继承下，派生类总是继承基类的接口
    纯虚函数只具体指定接口继承
    非虚函数具体指定接口继承以及强制性实现继承
35. 考虑 virtual 函数以外的其他选择
        - 考虑将虚函数替换为函数指针成员变量，以 `tr1::function` 成员变量替换虚函数
36. 绝不重新定义继承而来的 non-virtual 函数
37. 绝不重新定义继承而来的缺省参数值
    - 因为缺省参数值是静态绑定，而虚函数是动态绑定
38. 通过复合塑模 has-a 或 “根据某物实现出”
    - 在应用域，复合意味 has-a（有一个）；在实现域，复合意味着 is-implemented-in-terms-of（根据某物实现出）
39. 明智而审慎地使用 private 继承
40. 明智而审慎地使用多重继承
    - 多继承比单一继承复杂，可能导致新的歧义性
    - 但确有正当用途，如 “public 继承某个 interface class” 和 “private 继承某个协助实现的 class”；
    - virtual 继承可解决多继承下菱形继承的二义性问题，但会增加大小、速度、初始化及赋值的复杂度等等成本
41. 了解隐式接口和编译期多态
42. 了解 typename 的双重意义
43. 学习处理模板化基类内的名称
44. 将与参数无关的代码抽离模板
    - 因类型模板参数造成代码膨胀往往可以通过函数参数或类成员变量替换模板参数来消除
    - 因类型参数而造成的代码膨胀往往可以通过让带有完全相同二进制表述的实现类型共享实现码
45. 运用成员函数模板接受所有兼容类型（请使用成员函数模板（member function templates）生成 “可接受所有兼容类型” 的函数；声明 member templates 用于 “泛化 copy 构造” 或 “泛化 assignment 操作” 时还需要声明正常的 copy 构造函数和 copy assignment 操作符）
46. 需要类型转换时请为模板定义非成员函数（当我们编写一个 class template，而它所提供之 “与此 template 相关的” 函数支持 “所有参数之隐式类型转换” 时，请将那些函数定义为 “class template 内部的 friend 函数”）
47. 请使用 traits classes 表现类型信息（traits classes 通过 templates 和 “templates 特化” 使得 “类型相关信息” 在编译期可用，通过重载技术（overloading）实现在编译期对类型执行 if...else 测试）
48. 认识 template 元编程（模板元编程（TMP，template metaprogramming）可将工作由运行期移往编译期，因此得以实现早期错误侦测和更高的执行效率；TMP 可被用来生成 “给予政策选择组合”（based on combinations of policy choices）的客户定制代码，也可用来避免生成对某些特殊类型并不适合的代码）
49. 了解 new-handler 的行为（set\_new\_handler 允许客户指定一个在内存分配无法获得满足时被调用的函数；nothrow new 是一个颇具局限的工具，因为它只适用于内存分配（operator new），后继的构造函数调用还是可能抛出异常）
50. 了解 new 和 delete 的合理替换时机（为了检测运用错误、收集动态分配内存之使用统计信息、增加分配和归还速度、降低缺省内存管理器带来的空间额外开销、弥补缺省分配器中的非最佳齐位、将相关对象成簇集中、获得非传统的行为）
51. 编写 new 和 delete 时需固守常规（operator new 应该内涵一个无穷循环，并在其中尝试分配内存，如果它无法满足内存需求，就应该调用 new-handler，它也应该有能力处理 0 bytes 申请，class 专属版本则还应该处理 “比正确大小更大的（错误）申请”；operator delete 应该在收到 null 指针时不做任何事，class 专属版本则还应该处理 “比正确大小更大的（错误）申请”）
52. 写了 placement new 也要写 placement delete（当你写一个 placement operator new，请确定也写出了对应的 placement operator delete，否则可能会发生隐微而时断时续的内存泄漏；当你声明 placement new 和 placement delete，请确定不要无意识（非故意）地遮掩了它们地正常版本）
53. 不要轻忽编译器的警告
54. 让自己熟悉包括 TR1 在内的标准程序库（TR1，C++ Technical Report 1，C++11 标准的草稿文件）
55. 让自己熟悉 Boost（准标准库）

<h1 id="internet">☁️ 计算机网络</h1>

## 输入网址后发生了什么？

1.域名由DNS解析到IP地址<br>
2.三次握手建立tcp连接<br>
3.浏览器发送http请求<br>
4.服务器处理该http请求，返回一个html文件<br>
5.浏览器解析并渲染

## 计算机网络体系结构

![计算机网络体系结构](https://github.com/arqady01/cpp-interview/blob/main/internet/%E7%BD%91%E7%BB%9C%E5%88%86%E5%B1%82%E6%A8%A1%E5%9E%8B.png)

## 各层作用及协议

七层 | 作用 | 协议
---|---|---
物理层 | 通过媒介传输比特，确定机械及电气规范（比特 Bit） | RJ45、CLOCK、IEEE802.3（中继器，集线器）
数据链路层|将比特组装成帧和点到点的传递（帧 Frame）| PPP、FR、HDLC、VLAN、MAC（网桥，交换机）
网络层|负责数据包从源到宿的传递和网际互连（包 Packet）|IP、ICMP、ARP、RARP、OSPF、IPX、RIP、IGRP（路由器）
运输层|提供端到端的可靠报文传递和错误恢复（ 段Segment）|TCP、UDP、SPX
会话层|建立、管理和终止会话（会话协议数据单元 SPDU）|NFS、SQL、NETBIOS、RPC
表示层|对数据进行翻译、加密和压缩（表示协议数据单元 PPDU）|JPEG、MPEG、ASII
应用层|允许访问OSI环境的手段（应用协议数据单元 APDU）|FTP、DNS、Telnet、SMTP、HTTP、WWW、NFS、DHCP

五层 | 作用 | 解决的问题 | 常见协议
---|---|---|---
应用层 | 网络应用 | 解决通过应用的交互来解决特定网络应用的问题 | HTTP SMTP DNS RTP
运输层 | 主机之间的数据传输 | 解决进程之间基于网络的通信问题 | TCP UDP
网络层 | 为数据报从源到目的选择路由 | 解决分组在多个网络上路由的问题 | IP ICMP ARP RIP
数据链路层 | 相邻网络节点间的数据传输 | 解决分组在相邻网络上传输的问题 | PPP MAC
物理层 | 在线路上传输比特bit | 解决用何种信号传输比特的问题 | RJ45 IEEE802.3

## 物理层

* 传输数据的单位：比特
* 数据传输系统：源系统（源点、发送器） --> 传输系统 --> 目的系统（接收器、终点）

通道：
* 单向通道（单工通道）：只有一个方向通信，没有反方向交互，如广播
* 双向交替通信（半双工通信）：通信双方都可发消息，但不能同时发送或接收
* 双向同时通信（全双工通信）：通信双方可以同时发送和接收信息

通道复用技术：
* 频分复用（FDM，Frequency Division Multiplexing）：不同用户在不同频带，所用用户在同样时间占用不同带宽资源
* 时分复用（TDM，Time Division Multiplexing）：不同用户在同一时间段的不同时间片，所有用户在不同时间占用同样的频带宽度
* 波分复用（WDM，Wavelength Division Multiplexing）：光的频分复用
* 码分复用（CDM，Code Division Multiplexing）：不同用户使用不同的码，可以在同样时间使用同样频带通信

## 数据链路层

主要信道：
* 点对点信道
* 广播信道

### 点对点信道

* 数据单元：帧

三个基本问题：
* 封装成帧：把网络层的 IP 数据报封装成帧，`SOH - 数据部分 - EOT`
* 透明传输：不管数据部分什么字符，都能传输出去；可以通过字节填充方法解决（冲突字符前加转义字符）
* 差错检测：降低误码率（BER，Bit Error Rate），广泛使用循环冗余检测（CRC，Cyclic Redundancy Check）

点对点协议（Point-to-Point Protocol）：
* 点对点协议（Point-to-Point Protocol）：用户计算机和 ISP 通信时所使用的协议

### 广播信道

广播通信：
* 硬件地址（物理地址、MAC 地址）
* 单播（unicast）帧（一对一）：收到的帧的 MAC 地址与本站的硬件地址相同
* 广播（broadcast）帧（一对全体）：发送给本局域网上所有站点的帧
* 多播（multicast）帧（一对多）：发送给本局域网上一部分站点的帧

### ARQ协议

自动重传请求协议是数据链路层和传输层的错误纠正协议之一。通过使用确认和超时机制，在不可靠的服务上实现可靠的信息传输。如果发送方在发送后一段时间内没有收到确认帧，它通常会重新发送

- 停止-等待协议SW

停等ARQ协议相当于发送窗口和接收窗口大小均为1的滑动窗口，即发送方发送一帧后，必须接收到一个确认帧才能发送下一个

- 回退N帧协议GBN

从第一个没有收到的数据包开始，往后的所有数据包都被接收端丢弃，发送端将发生错误的帧开始及其之后所有的帧全部再重新发送


- 选择重传协议SR

发送端连续发送数据包但对每个数据包都设有个一个计时器，当一定时间内没有收到某个数据包的确认帧时，发送端只重新发送那个没有ACK的数据包

## 网络层

* IP（Internet Protocol，网际协议）是为计算机网络相互连接进行通信而设计的协议。
* ARP（Address Resolution Protocol，地址解析协议）

根据IP地址获取物理地址。主机发送信息时将包含目标IP地址的ARP请求广播到局域网络中的所有主机，并接收返回消息，以确定目标的物理地址；收到返回消息后将该IP地址和物理地址存入本机ARP缓存中，并保留一定时间，下次请求时直接查询ARP缓存以节约资源

* ICMP（Internet Control Message Protocol，网际控制报文协议）
* IGMP（Internet Group Management Protocol，网际组管理协议）

### IP 网际协议

IP 地址分类：
* `IP 地址 ::= {<网络号>,<主机号>}`

IP 地址类别 | 网络号 | 网络范围 | 主机号 | IP 地址范围
---|---|---|---|---
A 类 | 8bit，第一位固定为 0 | 0 —— 127 | 24bit | 1.0.0.0 —— 127.255.255.255
B 类 | 16bit，前两位固定为  10 | 128.0 —— 191.255 | 16bit | 128.0.0.0 —— 191.255.255.255
C  类 | 24bit，前三位固定为  110 | 192.0.0 —— 223.255.255 | 8bit | 192.0.0.0 —— 223.255.255.255
D  类 | 前四位固定为 1110，后面为多播地址
E  类 | 前五位固定为 11110，后面保留为今后所用


### 路由表包含什么？

1. 网络 ID（Network ID, Network number）：就是目标地址的网络 ID。
2. 子网掩码（subnet mask）：用来判断 IP 所属网络
3. 下一跳地址/接口（Next hop / interface）：就是数据在发送到目标地址的旅途中下一站的地址。其中 interface 指向 next hop（即为下一个 route）。一个自治系统（AS, Autonomous system）中的 route 应该包含区域内所有的子网络，而默认网关（Network id: `0.0.0.0`, Netmask: `0.0.0.0`）指向自治系统的出口。

根据应用和执行的不同，路由表可能含有如下附加信息：

1. 花费（Cost）：就是数据发送过程中通过路径所需要的花费。
2. 路由的服务质量
3. 路由中需要过滤的出/入连接列表

## 传输层

协议：

* TCP（Transmission Control Protocol，传输控制协议）
* UDP（User Datagram Protocol，用户数据报协议）

端口：

应用程序 | FTP | TELNET | SMTP | DNS | TFTP | HTTP | HTTPS | SNMP  
--- | --- | --- |--- |--- |--- |--- |--- |---   
端口号 | 21 | 23 | 25 | 53 | 69 | 80 | 443 | 161  

## TCP

* TCP是一种面向连接的、可靠的、一对一、基于字节流的传输层通信协议，传输单位是报文段。

### TCP 报文结构

![TCP 报文](https://gitee.com/huihut/interview/raw/master/images/TCP报文.png)

TCP 首部

![TCP 首部](https://gitee.com/huihut/interview/raw/master/images/TCP首部.png)

TCP：状态控制码（Code，Control Flag），占 6 比特，含义如下：
* URG：紧急比特（urgent），当 `URG＝1` 时，表明紧急指针字段有效，代表该封包为紧急封包。它告诉系统此报文段中有紧急数据，应尽快传送(相当于高优先级的数据)， 且上图中的 Urgent Pointer 字段也会被启用。
* ACK：确认比特（Acknowledge）。只有当 `ACK＝1` 时确认号字段才有效，代表这个封包为确认封包。当 `ACK＝0` 时，确认号无效。
* PSH：（Push function）若为 1 时，代表要求对方立即传送缓冲区内的其他对应封包，而无需等缓冲满了才送。
* RST：复位比特(Reset)，当 `RST＝1` 时，表明 TCP 连接中出现严重差错（如由于主机崩溃或其他原因），必须释放连接，然后再重新建立运输连接。
* SYN：同步比特(Synchronous)，SYN 置为 1，就表示这是一个连接请求或连接接受报文，通常带有 SYN 标志的封包表示『主动』要连接到对方的意思。
* FIN：终止比特(Final)，用来释放一个连接。当 `FIN＝1` 时，表明此报文段的发送端的数据已发送完毕，并要求释放运输连接。

### UDP

* UDP是一种无连接、面向报文的传输层协议，支持一对一、一对多、多对一、多对多的交互通信，没有拥塞控制算法，传输单位是用户数据报


### TCP 与 UDP 的区别

1. TCP 面向连接，UDP 是无连接
2. TCP 服务可靠，通过 TCP 连接传送的数据无差错、不丢失、不重复、且按序到达；UDP 尽最大努力交付，不保证可靠交付
3. TCP 的逻辑通信信道是全双工的可靠信道；UDP 则是不可靠信道
5. 每一条 TCP 连接只能是点到点的；UDP 支持一对一，一对多，多对一和多对多的交互通信
6. TCP 面向字节流（可能出现黏包问题），即一连串无结构的字节流；UDP 是面向报文的（不会出现黏包问题）
7. UDP 没有拥塞控制，因此网络出现拥塞不会使源主机的发送速率降低（对实时应用很有用，如 IP 电话，实时视频会议等）
8. TCP 首部开销20字节；UDP 的首部开销小，只有 8 个字节

### TCP粘包

TCP 基于字节流传输（UDP 基于报文的），“流” 意味着 TCP 所传输的数据是没有边界的，可能会出现两个数据包黏在一起的情况

#### 解决方案

- 发送定长包。如果每个消息的大小都是一样的，那么在接收对等方只要累计接收数据，直到数据等于一个定长的数值就将它作为一个消息
- 包头加上包体长度。包头是定长的 4 个字节，说明了包体的长度。接收对等方先接收包头长度，依据包头长度来接收包体
- 在数据包之间设置边界，如添加特殊符号 `\r\n` 标记。但问题在于如果数据正文中也含有 `\r\n`，则会误判为消息的边界

### TCP 如何保证可靠传输

* 数据合理分片和排序
    - 保证收到的tcp报文都是有序的
* 数据包校验
* 确认机制
    - 发送端的数据到达接收主机时，接收端返回一个确认报文，表示已收到消息
* 超时重传机制
    - 针对数据包丢失问题
* 快重传机制
    - 针对个别报文段的丢失
* 流量控制
    - 滑动窗口实现。发送窗口大小由确认报文中的窗口字段控制，从而控制发送方的发送速率
    - <p align="center"> 
    	<img src="https://gitee.com/huihut/interview/raw/master/images/利用可变窗口进行流量控制举例.png" style="width:73%;">
      </p>
* 拥塞控制，防止过多的数据注入到网络中，可使网络中的路由器或链路不过载
    - 慢开始
    - 拥塞避免
    - 快重传
    - 快恢复

#### TCP的拥塞控制

* 慢开始：每次传输，拥塞窗口cwnd扩大一倍。当cwnd等于慢开始门限后，改用拥塞避免算法
* 拥塞避免：拥塞窗口随传输轮次缓慢加一，若发生超时，cwnd置为1，慢开始门限ssthresh减半，并重新用慢开始算法
* 快重传：使发送方尽快进行重传。报文段都是按顺序传送的，若收到了失序的报文段，要发出对已收到报文段的重复确认。发送方一旦受到三个连续的重复确认，就将相应的报文段立即上传，而不是等报文段的超时重传计时器超时在重传，并直接执行快恢复算法
* 快恢复：将慢开始门限ssthresh和拥塞窗口cwnd调整为当前窗口的一半，开始执行拥塞避免算法

![](https://gitee.com/huihut/interview/raw/master/images/TCP拥塞窗口cwnd在拥塞控制时的变化情况.png)
![](https://gitee.com/huihut/interview/raw/master/images/快重传示意图.png)
![](https://gitee.com/huihut/interview/raw/master/images/TCP的拥塞控制流程图.png)


#### TCP 三次握手建立连接

![UDP 报文](https://gitee.com/huihut/interview/raw/master/images/TCP三次握手建立连接.png)

【TCP 建立连接全过程解释】

1. 客户端发送 SYN 给服务器，说明客户端请求建立连接；
2. 服务端收到客户端发的 SYN，并回复 SYN+ACK 给客户端（同意建立连接）；
3. 客户端收到服务端的 SYN+ACK 后，回复 ACK 给服务端（表示客户端收到了服务端发的同意报文）；
4. 服务端收到客户端的 ACK，连接已建立，可以数据传输。

#### TCP 为什么要进行三次握手？

因为双方都需要确认对方收到了自己发送的序列号，确认过程最少要进行三次通信


#### TCP 四次挥手释放连接

![UDP 报文](https://gitee.com/huihut/interview/raw/master/images/TCP四次挥手释放连接.png)

【TCP 释放连接全过程解释】

1. 客户端发送 FIN 给服务器，说明客户端不必发送数据给服务器了（请求释放从客户端到服务器的连接）；
2. 服务器接收到客户端发的 FIN，并回复 ACK 给客户端（同意释放从客户端到服务器的连接）；
3. 客户端收到服务端回复的 ACK，此时从客户端到服务器的连接已释放（但服务端到客户端的连接还未释放，并且客户端还可以接收数据）；
4. 服务端继续发送之前没发完的数据给客户端；
5. 服务端发送 FIN+ACK 给客户端，说明服务端发送完了数据（请求释放从服务端到客户端的连接，就算没收到客户端的回复，过段时间也会自动释放）；
6. 客户端收到服务端的 FIN+ACK，并回复 ACK 给服务端（同意释放从服务端到客户端的连接）；
7. 服务端收到客户端的 ACK 后，释放从服务端到客户端的连接。

### 为什么需要TIME_WAIT 状态？

主动发起关闭连接的⼀⽅，才会有 TIME-WAIT（FIN_WAIT）。 需要 TIME-WAIT 状态，主要是两个原因：
- 防⽌具有相同四元组的旧数据包被收到；
- 保证被动关闭连接的⼀⽅能被正确的关闭，即保证最后的ACK能让被动关闭⽅接收，从⽽帮助其正常关闭

### 为什么TIME_WAIT是两倍的MSL？

- 确保被动关闭连接的一方能收到第四次挥手的ACK
- 避免上一次TCP连接的数据包影响到下一次的TCP连接

#### TCP 为什么要进行四次挥手？

【问题一】TCP 为什么要进行四次挥手？ / 为什么 TCP 建立连接需要三次，而释放连接则需要四次？

【答案一】因为 TCP 是全双工模式，客户端请求关闭连接后，客户端向服务端的连接关闭（一二次挥手），服务端继续传输之前没传完的数据给客户端（数据传输），服务端向客户端的连接关闭（三四次挥手）。所以 TCP 释放连接时服务器的 ACK 和 FIN 是分开发送的（中间隔着数据传输），而 TCP 建立连接时服务器的 ACK 和 SYN 是一起发送的（第二次握手），所以 TCP 建立连接需要三次，而释放连接则需要四次。

【问题二】为什么 TCP 连接时可以 ACK 和 SYN 一起发送，而释放时则 ACK 和 FIN 分开发送呢？（ACK 和 FIN 分开是指第二次和第三次挥手）

【答案二】因为客户端请求释放时，服务器可能还有数据需要传输给客户端，因此服务端要先响应客户端 FIN 请求（服务端发送 ACK），然后数据传输，传输完成后，服务端再提出 FIN 请求（服务端发送 FIN）；而连接时则没有中间的数据传输，因此连接时可以 ACK 和 SYN 一起发送。

【问题三】为什么客户端释放最后需要 TIME-WAIT 等待 2MSL 呢？

【答案三】

1. 为了保证客户端发送的最后一个 ACK 报文能够到达服务端。若未成功到达，则服务端超时重传 FIN+ACK 报文段，客户端再重传 ACK，并重新计时。
2. 防止已失效的连接请求报文段出现在本连接中。TIME-WAIT 持续 2MSL 可使本连接持续的时间内所产生的所有报文段都从网络中消失，这样可使下次连接中不会出现旧的连接报文段。

### TCP 有限状态机

TCP 有限状态机图片

![TCP 的有限状态机](https://gitee.com/huihut/interview/raw/master/images/TCP的有限状态机.png)

## 应用层

### DNS

* DNS（Domain Name System，域名系统）是互联网的一项服务。它作为将域名和 IP 地址相互映射的一个分布式数据库，能够使人更方便地访问互联网。DNS 使用 TCP 和 UDP 端口 53。当前，对于每一级域名长度的限制是 63 个字符，域名总长度则不能超过 253 个字符。

域名：
* `域名 ::= {<三级域名>.<二级域名>.<顶级域名>}`，如：`blog.huihut.com`

### FTP

* FTP（File Transfer Protocol，文件传输协议）是用于在网络上进行文件传输的一套标准协议，使用客户/服务器模式，使用 TCP 数据报，提供交互式访问，双向传输。
* TFTP（Trivial File Transfer Protocol，简单文件传输协议）一个小且易实现的文件传输协议，也使用客户-服务器方式，使用UDP数据报，只支持文件传输而不支持交互，没有列目录，不能对用户进行身份鉴定

### DHCP

动态主机配置协议，使用UDP协议工作，DHCP客户端监听68端口号，服务端监听67端口号。DHCP通常被用于局域网环境的分配IP地址，使客户端动态地获得IP地址、DNS等信息

### TELNET

* TELNET 协议是 TCP/IP 协议族中的一员，是 Internet 远程登陆服务的标准协议和主要方式。它为用户提供了在本地计算机上完成远程主机工作的能力。

* HTTP（HyperText Transfer Protocol，超文本传输协议）是用于从 WWW（World Wide Web，万维网）服务器传输超文本到本地浏览器的传送协议。

* SMTP（Simple Mail Transfer Protocol，简单邮件传输协议）是一组用于由源地址到目的地址传送邮件的规则，由它来控制信件的中转方式。SMTP 协议属于 TCP/IP 协议簇，它帮助每台计算机在发送或中转信件时找到下一个目的地。
* Socket 建立网络通信连接至少要一对端口号（Socket）。Socket 本质是编程接口（API），对 TCP/IP 的封装，TCP/IP 也要提供可供程序员做网络开发所用的接口，这就是 Socket 编程接口。

### WWW

* WWW（World Wide Web，环球信息网，万维网）是一个由许多互相链接的超文本组成的系统，通过互联网访问

#### URL

* URL（Uniform Resource Locator，统一资源定位符）是因特网上标准的资源的地址（Address）

#### HTTP

超文本传输协议，是一种用于分布式、协作式和超媒体信息系统的应用层协议，承载于tcp协议之上，http由请求和响应构成，是一个标准的客户端服务器模型（B/S），http协议永远都是客户端发起请求，服务器回送响应。http是无状态协议，为了实现状态保持功能, 引入了cookie技术

请求方法

方法 | 意义
--- | ---
OPTIONS | 请求一些选项信息，允许客户端查看服务器的性能
GET | 请求指定的页面信息，并返回实体主体
HEAD | 类似于 get 请求，只不过返回的响应中没有具体的内容，用于获取报头
POST | 向指定资源提交数据进行处理请求（例如提交表单或者上传文件）。数据被包含在请求体中。POST请求可能会导致新的资源的建立和/或已有资源的修改
PUT | 从客户端向服务器传送的数据取代指定的文档的内容
DELETE | 请求服务器删除指定的页面
TRACE | 回显服务器收到的请求，主要用于测试或诊断

状态码（Status-Code）

* 1xx：表示通知信息，如请求收到了或正在进行处理
    * 100 Continue：继续，客户端应继续其请求
    * 101 Switching Protocols 切换协议。服务器根据客户端的请求切换协议。只能切换到更高级的协议，例如，切换到 HTTP 的新版本协议
* 2xx：表示成功，如接收或知道了
    * 200 OK: 请求成功
* 3xx：表示重定向，如要完成请求还必须采取进一步的行动
    * 301 Moved Permanently: 永久移动。请求的资源已被永久的移动到新 URL，返回信息会包括新的 URL，浏览器会自动定向到新 URL。今后任何新的请求都应使用新的 URL 代替
* 4xx：表示客户的差错，如请求中有错误的语法或不能完成
    * 400 Bad Request: 客户端请求的语法错误，服务器无法理解
    * 401 Unauthorized: 请求要求用户的身份认证
    * 403 Forbidden: 服务器理解请求客户端的请求，但是拒绝执行此请求（权限不够）
    * 404 Not Found: 服务器无法根据客户端的请求找到资源（网页）。通过此代码，网站设计人员可设置 “您所请求的资源无法找到” 的个性页面
    * 408 Request Timeout: 服务器等待客户端发送的请求时间过长，超时
* 5xx：表示服务器的差错，如服务器失效无法完成请求
    * 500 Internal Server Error: 服务器内部错误，无法完成请求
    * 503 Service Unavailable: 由于超载或系统维护，服务器暂时的无法处理客户端的请求。延时的长度可包含在服务器的 Retry-After 头信息中
    * 504 Gateway Timeout: 充当网关或代理的服务器，未及时从远端服务器获取请求


## https工作原理

HTTPS是在HTTP协议的基础上加入了SSL/TLS协议（传输层和应用层中间），可以对数据进行加密，并且能验证通信双方身份（SSL和TLS其实是同一个东西，“传输层安全协议”的不同叫法而已）

## https对比http

建立连接时候https比http多了SSL的握手过程，传输内容的时候https会对数据进行加密

## 简述LRU算法

最近最少使用算法根据数据的历史访问记录来进行淘汰数据，其核心思想是“如果数据最近被访问过，那么将来被访问的几率也更高”。

* 新数据插入到链表头部
* 每当缓存数据被访问，则将数据移到链表头部
* 当链表满时，将链表尾部的数据丢弃

![LRU算法](https://github.com/arqady01/cpp-interview/blob/main/internet/LRU.png)

## cookie和session

http是无状态协议，每个请求都是完全独立的，服务端无法确认当前访问者的身份信息，所以服务器与浏览器为了进行会话跟踪（知道是谁在访问我），就必须主动的去维护一个状态，用于告知服务端前后两个请求是否来自同一浏览器，而这个状态需要通过cookie或者session去实现

- cookie

cookie存储在客户端，是服务器发送到浏览器并保存在本地的一小块数据，它会在浏览器下次向同一服务器再访问时被携带并发送到服务器上<br>
cookie是不可跨域的，每个cookie都会绑定单一的域名，无法在别的域名下获取使用

- session

session是另一种记录服务器和客户端会话状态的机制。session基于cookie实现，session存储在服务端，session_id会被存储到客户端的cookie中

### session对比cookie

- 数据存放位置不同：session存在服务器中，cookie存放在客户端中
- 安全程度不同：cookie放在客户端中不是很安全，session放在服务器中，相对安全
- 性能使用程度不同：session放在服务器上，访问增多会占用服务器的性能；考虑到减轻服务器性能方面，应使用cookie
- 数据存储大小不同：单个cookie保存的数据不能超过4K，session存储在服务端，根据服务器大小来定

## 零拷贝技术

优点

在传统的数据传输中，数据需要在用户和内核空间之间进行多次复制。例如，当一个应用程序从磁盘读取文件并发送到网络时，数据首先需要从磁盘复制到内核空间的缓冲区，然后再从内核空间的缓冲区复制到用户空间的缓冲区，最后再从用户空间的缓冲区复制回内核空间的网络缓冲区。这些复制操作会消耗大量的 CPU 时间和内存带宽。

零拷贝避免了内核空间到用户空间的数据拷贝。一种常见的方法是使用内存映射文件。应用程序可以将文件直接映射到其地址空间，然后直接操作这些数据，无需将数据复制到用户空间的缓冲区。另一种方法是使用 sendfile 等系统调用，这些系统调用可以直接将数据从一个文件描述符发送到另一个文件描述符，无需经过用户空间。

缺点

由于设备之间的数据传输是通过 DMA 完成的，因此用户空间的数据缓冲区内存页必须进行页锁定，这是为了防止其物理页框地址被交换到磁盘或者被移动到新的地址而导致 DMA 去拷贝数据的时候在指定的地址找不到内存页从而引发缺页错误，而页锁定的开销并不比 CPU 拷贝小，所以为了避免频繁的页锁定系统调用，应用程序必须分配和注册一个持久的内存池，用于数据缓冲。

如果访问的数据不在应用程序缓存中，那么每次数据都会直接从磁盘进行加载，这种直接加载会非常缓慢。在应用层引入直接 I/O 需要应用层自己管理，这带来了额外的系统复杂性

## select/poll/epoll

- 性能差异
    - epoll 使用事件通知机制（类似观察者模式模型），在处理大量连接时性能更好
    - select 和 poll 需要轮询所有的文件描述符，效率较低
- 文件描述符数量差异
    - select 一般为 1024 个
    - poll 通常限制也是 1024 个
    - epoll 在Linux下无明确限制，可以处理更大数量的文件描述符
- 事件触发方式
    - select 利用数组存储文件描述符，使用轮询方式来检查文件描述符的状态
    - poll 利用链表存储文件描述符，需要轮询来判断是否发生了IO事件
    - epoll 利用红黑树存储，事件通知模型，在有事件发生时会通知程序，不需要遍历所有的文件描述符
        - LT水平触发
            1. LT是 epoll 的默认工作模式，当监控的文件描述符上有可读、可写或异常事件发生时，epoll 会通知
            2. 如果应用程序未处理完数据，只要还有数据可读写，epoll_wait会再次返回该文件描述符
            3. 只要有数据可读可写，epoll_wait() 将持续通知该事件
        - ET边缘触发
            1. epoll 仅在状态变化时通知应用程序，比如从无数据变为有数据，从不可写变为可写
            2. 当通过 epoll_wait() 通知应用程序某个文件描述符上的事件后，程序必须处理该事件（尽可能多的读取数据或写入数据），因为 epoll 不会再次通知该事件，除非状态再次发生变化
            3. 应用程序需要非阻塞的方式操作文件描述符，以避免在读写操作时被阻塞，因为没有新的通知告知是否还有更多的数据可读或可写
            
- 接口参数：
    - select 的参数是一个 fd_set 结构体数组，需要通过宏定义进行操作
    - poll 的参数是一个 pollfd 结构体数组
    - epoll 利用三个函数 epoll_create、epoll_ctl 和 epoll_wait 来完成操作，使用起来更加灵活

<h1 id="algorithm">🎀 剑指offer</h1>

## 剑指Offer

[题库](https://github.com/arqady01/Cpp-interface/blob/main/src/Offer/offer.md)

## 回溯法backtraveling

回溯法一般用于解决以下这五类问题

- 组合问题：N个数里面按一定规则找出k个数的集合
- 切割问题：一个字符串按一定规则有几种切割方式
- 子集问题：一个N个数的集合里有多少符合条件的子集
- 排列问题：N个数按一定规则全排列，有几种排列方式
- 棋盘问题：N皇后，解数独

补充一点，组合不强调顺序，而排列强调顺序，即：组合无序，排列有序。

回溯法都可以抽象为树形结构；集合的大小即树的宽度，递归的深度就是树的深度，递归必须有终止条件，所以此树是一颗高度有限的N叉树。

#### 回溯法模板

```cpp
void backtracking(参数) {
    if 满足终止条件 {
        将结果纳入结果集;
        return;
    }
    for (选择：本层集合中元素（树中节点孩子的数量就是集合的大小）) {
        处理节点;
        backtracking(路径，选择列表); //递归
        回溯，撤销处理结果
    }
}
```

[回溯法题库](https://github.com/arqady01/Cpp-interface/blob/main/src/Offer/backtravel.md)

## 链表

[链表题库](https://github.com/arqady01/Cpp-interface/blob/main/src/Offer/ListNode.md)

## 二叉树

#### 先序遍历递归法

```cpp
void preOrder(TreeNode* root) { 
    if (root == nullptr) return; //①递归终止条件
    visit(root); //访问当前节点，自行定义
    //②单层递归逻辑
    if (root->left != nullptr)
        preOrder(root->left);
    if (root->right != nullptr)
        preOrder(root->right);
}
```

#### 先序遍历迭代法

借助栈来实现，孩子节点从右至左依次入栈

```cpp
void preOrder(TreeNode* root) {
    if (root == nullptr)
        return; //空树，直接返回
    TreeNode* cur = root; //操作节点
    std::stack<TreeNode*> s;
    s.push(cur); //头节点入栈
    while (!s.empty()) {
        cur = s.top(); //step1.取出栈顶元素
        visit(cur); //对元素处理，比如打印cur->val
        s.pop(); //step2.访问完毕抛出栈顶元素
        //step3.孩子节点从右至左入栈
        if (cur->right != nullptr) {
            s.push(cur->right);
        }
        if (cur->left != nullptr) {
            s.push(cur->left);
        }
    }
}
```

#### 中序遍历递归法

```cpp
void midOrder(TreeNode* root) { 
    if (root == nullptr) return; //①递归终止条件
    //②单层递归逻辑
    if (root->left != nullptr) {
        preOrder(root->left);
    }
    visit(root); //访问当前节点，自行定义
    if (root->right != nullptr) {
        preOrder(root->right);
    }
}
```

#### 中序遍历迭代法

借助栈，先把左子树入栈，再回退去处理右子节点

```cpp
void midOrder(TreeNode* root) {
    if (root == nullptr) {
        return;
    }
    std::stack<TreeNode*> s;
    TreeNode* cur = root; //操作节点
    while (!s.empty() || cur != nullptr) {
        //左子树不停的入栈
        if (cur != nullptr) {
            s.push(cur);
            cur = cur->left; //走左边
        } else {
            cur = s.top(); //cur为空时，就回退到栈顶元素
            visit(cur); //对元素处理，比如打印cur->val
            s.pop();
            cur = cur->right; //走右边，走完之后继续搞左边
        }
    }
}
```

或者改造一下，这个改造也必须要会

```cpp
void midOrder(TreeNode* root) {
    if (root == nullptr) {
        return;
    }
    std::stack<TreeNode*> s;
    TreeNode* cur = root; //操作节点
    while (!s.empty() || cur != nullptr) {
        //左子树不停的入栈
        while (cur != nullptr) {
            s.push(cur);
            cur = cur->left; //走左边
        }
        cur = s.top(); //cur为空时，就回退到栈顶元素
        visit(cur); //对元素处理，比如打印cur->val
        s.pop();
        cur = cur->right; //走右边，走完之后继续搞左边
    }
}
```

需要补充的是，对于二叉搜索树，使用中序遍历会得到一个从小到大的序列，但是如果改变入栈的顺序将会得到一个从大到小的序列：

```cpp
void midOrder(TreeNode* root) {
    if (root == nullptr) {
        return;
    }
    std::stack<TreeNode*> s;
    TreeNode* cur = root; //操作节点
    while (!s.empty() || cur != nullptr) {
        //左子树不停的入栈
        while (cur != nullptr) {
            s.push(cur);
            cur = cur->right; //走右边 //改动一
        }
        cur = s.top(); //cur为空时，就回退到栈顶元素
        visit(cur); //对元素处理，比如打印cur->val
        s.pop();
        cur = cur->left; //走左边，走完之后继续搞右边 //改动二
    }
}
```

#### 后续遍历递归法

```cpp
void posOrder(TreeNode* root) { 
    if (root == nullptr) return; //①递归终止条件
    //②单层递归逻辑
    if (root->left != nullptr) {
        posOrder(root->left);
    }
    if (root->right != nullptr) {
        posOrder(root->right);
    }
    visit(root); //访问当前节点，自行定义
}
```

#### 后续遍历迭代法

借助栈

```cpp
std::stack<TreeNode*> posOrder(TreeNode* root) {
    if (root == nullptr) {
        return;
    }
    TreeNode* cur = root; //操作节点
    std::stack<TreeNode*> s1;
    std::stack<TreeNode*> s2; //收集栈
    s1.push(cur); //首先将根节点入栈
    while (!s1.empty()) {
        cur = s1.top(); //取出s1栈顶元素
        //对比先序遍历，visit动作换成了压入收集栈动作
        //同时从右至左也换成了从左至右
        s2.push(cur);
        s1.pop(); //弹出s1栈顶元素
        if (cur->left != nullptr) {
            s1.push(root->left);
        }
        if (cur->right != nullptr) {
            s2.push(root->right);
        }
    }
    return s2; //读出收集栈中数据，就是后序遍历序列
}
```

#### 广度优先遍历

```cpp
void bfs(TreeNode* root) {
    if (root == nullptr)
        return;
    std::queue<TreeNode*> q;
    q.push(root);
    while (!q.empty()) {
        root = q.front();
        visit(root->val); //访问
        q.pop();
        if (root->left != nullptr)
            q.push(root->left);
        if (root->right != nullptr)
            q.push(root->right);
    }
}
```

[二叉树题库](https://github.com/arqady01/Cpp-interface/blob/main/src/BinaryTree.md#L150)

## 动态规划

解题步骤

- 确定dp数组及下标的含义
- 确定递推公式
- dp数组如何初始化
- 确定遍历顺序（背包问题和排列组合数问题尤其重要）
- 举例推导dp数组

[动态规划题库](https://github.com/arqady01/Cpp-interface/blob/main/src/dp.md)

## 单调栈

#### 每日温度

数组temperatures表示每天的温度，返回一个数组answer，其中answer[i]是指对于第i天，下一个更高温度出现在几天后。如果气温在这之后都不会升高，请在该位置用0来代替。

输入: temperatures = [35,31,33,36,34]

输出: [3,1,1,0,0]

输入: temperatures = [30,60,90]

输出: [1,1,0]

单调栈思想，寻找右边第一个比自己大的元素

i = 0时，还不清楚后面有没有出现比35℃更高的温度，所以0（35℃）入栈

i = 1时，下标index = top() = 0，31℃比它低，即31℃没有遇到更热的天，1（31℃）入栈

i = 2时，index = top() = 1，对应的31℃遇到了更热的33℃，更新ans为i - index，并让1出栈；继续对比，35℃不需要更新。2（33℃）入栈

i = 3时，index = top() = 2，对应的33℃遇到了更热的36℃，更新ans，让2出栈；对比35℃，更新ans，并让0出栈。最后3（36℃）入栈

i = 4时，4（34℃）入栈

```cpp
vector<int> dailyTemperatures(vector<int>& temperatures) {
    std::vector<int> ans(temperatures.size(), 0);
    //单调栈中记录的是遍历过的元素的下标，所以不管当前遍历元素比栈顶元素大还是小，
    //栈顶元素都是要入栈的，代表当前元素被遍历过了
    std::stack<int> s; //栈中不是存放温度，而是下标
    for (int i = 0; i < temperatures.size(); i++) {
        //第一次运行时，因为栈为空，top()在&&后并不会报错，同时while跳过先把0入栈
        //当遍历元素大于栈顶元素，说明栈顶元素找到比自己大的元素了，就可以更新ans并且
        //弹出栈顶元素，弹出的原因是因为自己找到更大的了，自己没必要再留在单调栈中了
        while (!s.empty() && temperatures[i] > temperatures[s.top()]) {
            int index = s.top();
            //index处的结果找到了，栈中没必要继续保存了，可以弹出
            s.pop();
            ans[index] = i - index;
        }
        //不管遍历元素比栈顶元素大还是小，都需要入栈
        //这里包含了另一种情况，当遍历元素比栈顶元素小，说明栈顶元素没找到比自己更大的元素
        //需要继续往后走，但是当前遍历元素的下标请入栈表示自己被遍历过了
        s.push(i);
    }
    return ans;
}
```

#### 42 接雨水

给定 n 个非负整数表示每个宽度为 1 的柱子的高度图，计算按此排列的柱子，下雨之后能接多少雨水。

<p align="center"> 
    <img src="https://github.com/arqady01/Cpp-interface/blob/main/resource/Offer_Question_images/rainwatertrap.png" style="width:50%;">
</p>

输入：height = [0,1,0,2,1,0,1,3,2,1,2,1]，输出：6

<p align="center"> 
    <img src="https://github.com/arqady01/Cpp-interface/blob/main/resource/Offer_Answer_images/42ans_1.jpg" style="width:100%;">
</p>

```cpp
class Solution {
public:
    int trap(vector<int>& height) {
        for (int i = 0; i < height.size(); i++) {
            while (!s.empty() && height[i] > height[s.top()]) {
                //栈顶元素的右边第一个比栈顶元素大的就是height[i]
                int mid = s.top(); //需要记录，不然就没了
                s.pop(); //弹出栈顶元素后，新的栈顶元素就是mid左边的第一大元素
                if (!s.empty()) { //防卫式声明
                    int high = min(height[i], height[s.top()]); //mid左右两侧的综合第一个大的元素高度
                    int h = high - height[mid]; //高度
                    int w = i - s.top() - 1; //宽度
                    ans += h * w;
                }
            }
            //如果当前元素大于等于栈顶元素，压栈，这样就得到一个单调递增栈
            s.push(i);
        }
        return ans;
    }
    int ans{};
    std::stack<int> s{}; //存储的是下标而不是数值，但是在理解的时候，可以代入一个pair
};
```

再举个例子，输入：height = [4,2,0,3,2,5]，输出：9。图示：

<p align="center"> 
    <img src="https://github.com/arqady01/Cpp-interface/blob/main/resource/Offer_Answer_images/42ans_2.jpg" style="width:100%;">
</p>

《接雨水》求的是基准数字左、右两边第一个比自己大的元素；《柱状图中最大的矩形》求的是基准数字左右两边第一个比自己小的元素。

记忆方法：《接雨水》：递增栈，递增对应大。递减栈，递减对应小

<h1 id="database">💾 数据库</h1>

## 🔩 Redis

- Redis完全基于内存
- 数据结构简单
- 使用多路IO复用模型，非阻塞IO
- 单线程模型，无需考虑加锁放锁操作，不会出现死锁问题

### 通用命令
- KEYS 查询
- DEL 删除
- EXISTS 判断key是否存在
- EXPIRE 给key设置有效期，到期自动删除
- TTL 查询key的有效期

### redis过期策略
key的过期时间用Unix时间戳存储<br>
redis使用：定期删除 + 惰性删除

#### 定时删除
创建key的同时创建一个定时器，当定时器结束，立即删除
- 优点：对内存最友好，key一旦过期就可以立即删除
- 缺点：对CPU最不友好，当过期key比较多，删除操作会占用CPU

#### 定期删除
redis每隔一段时间（可在配置文件中设置），就在设置了过期时间的keys中随机抽查，如果过期了就将其删除
- 优点：可以通过设置频率来减少CPU负载，另外定期删除，也能有效释放过期的key
- 缺点：执行删除操作的频率不可控，太频繁或稀疏都不好

#### 惰性删除
设置key的过期时间后，不再管，当需要该key时，再检查是否过期，如果过期就删除，反之返回该key
- 优点：对CPU友好，只会在用到该键时才会做过期检查，对于用不到的key就不做过期检查
- 缺点：对内存不友好。若一个key已过期但未使用，那么该key一直储存在内存中；量一多，大量的过期key在内存中得不到释放，造成泄露（可见内存淘汰策略）

### 内存淘汰策略

当redis占用内存超过maxmemory时，会触发主动清理策略
- 针对设置了过期时间的key处理
	- volatile-ttl 筛选设置了过期时间的键值对，越早过期的越先被删除
	- volatile-random 筛选设置了过期时间的键值对，随机删除
	- volatile-lru 利用LRU算法筛选设置了过期时间的键值对进行删除
	- volatile-lfu 利用LFU算法筛选设置了过期时间的键值对进行删除
- 针对所有的key做处理
	- allkeys-random 在所有的键值对中进行随机删除
	- allkeys-lru 利用LRU算法对所有数据筛选后删除
	- allkeys-lfu 利用LFU算法对所有数据筛选后删除
- 不处理
	- noevent 不剔除任何数据，拒绝所有写入操作并返回错误信息，此时redis只响应读操作

#### LRU ＆ LFU
- LRU，最近最少使用(least recently used)，以最近一次访问时间作为参考，淘汰很久没被访问过的数据
- LFU，最不经常使用(least frequently used)，以次数作为参考，淘汰最近一段事件被访问次数最少的数据
绝大多数情况使用LRU，但当存在大量热点缓存数据时，LFU更佳

### 多路IO复用模型
- select：利用数组存储文件描述符，容量固定，需要轮询来判断是否发生了IO事件
- poll：链表存储文件描述符，可扩展，也需要轮询
- epoll：红黑树存储，利用事件通知（观察者模式）模型，只有发生了IO事件，应用程序才会进行IO操作

### redis对象类型
redis是key-value类型，支持string, list, hash, set, SortedSet（有序集合）

#### String
根据字符串的格式不同，可分为三类
| 编码类型 | 含义 |
| :----: | :----: |
| string | 普通字符串 |
| int | 整数类型，可自增自减 |
| float | 浮点类型，可自增自减 |

| 底层实现 | 含义 |
| :----: | :----: |
| int | 字符串长度小于等于20且能够转为整数 |
| embstr | 字符串长度小于等于44 |
| raw | 字符串长度大于44 |

为啥是44呢？因为cpu中每一个多级缓存的缓存行 为64字节，其他额外数据占据19，转义字符占1字节

- SET 添加或删除string类型的键值对
- GET 根据key获取值
- MSET 批量添加多个string型的键值对
- MGET 根据多个key获取值
- INCR 让int型数据自增1
- INCRBY 让int型自增指定步长
- INCRBYFLOAT 让float型自增指定步长
- SETNX 添加一个string型的键值对，前提是此key不存在，否则不添加
- SETEX 添加一个string型的键值对，并指定有效期

#### key结构
Redis允许有多个单词形成层级结构，多个单词之间用'：'隔开，比如：<br>
项目名：业务名：类型：id<br>
再将值序列化成json对象存储<br>
```cpp
SET Apple:iphone:A2022_1 '{"id":1, "name":"13pro", "color":"blue"}'
SET Apple:iphone:A2022_2 '{"id":2, "name":"13pro", "color":"white"}'
```

#### List

```
两种底层数据结构：
- quicklist 双向链表
- ziplist 压缩列表
```

- LPUSH key element 向列表左侧插入一个或多个元素
- LPOP key 移除并返回左侧的第一个元素，没有则返回nil
- RPUSH key element 向列表右侧插入一个或多个元素
- RPOP key 移除并返回右侧的第一个元素
- LRANGE key start end 返回一段角标范围内的所有元素
- BLPOP和BRPOP 与LPOP类似，只不过在没有元素时等待指定时间，而不是直接返回nil


#### Hash
值是无序字典（规定字典为 field-value对）

```
实现方式：
dict - 字典（节点数量大于512或字符串长度大于64）
ziplist - 压缩列表（节点数量小于等于512且字符串长度小于等于64）
```

- HSET key field value：添加或修改key的field值
- HGET key field：获取hash类型key的field的值
- HMSET：批量添加多个hash类型key的field的值
- HMGET：批量获取多个hash类型key的field的值
- HGETALL：获取hash类型的key中的所有的field和value
- HKEYS：获取一个hash类型的key中的所有的field
- HVALS：获取一个hash类型的key中的所有的value
- HINCRBY：让一个hash类型key的字段值自增并指定步长
- HSETNX：添加一个hash类型的key的field值，前提是这个field不存在，否则不执行

#### set
类似于一个没有value的无序字典

```
intset - 整数数组（元素都为整数且节点数量小于等于512）
dict - 字典（元素有一个不为整数或者数量大于512）
```

- SADD key member 向set中添加一个或多个元素
- SREM key member 移除set中的指定元素
- SCARD key 返回set中元素的个数
- SISMEMBER key member 判断一个元素是否存在于set中
- SMEMBERS 获取set中的所有元素
- SINTER key1 key2 求key1与key2的交集
- SDIFF key1 key2 求key1与key2的差集
- SUNION key1 key2 求key1和key2的并集

#### SortedSet（weight是权值）

```
skiplist - 跳表（数量大于128或者有一个字符串长度大于64）
ziplist - 压缩列表
```

- ZADD key weight member 添加一个或多个元素到SortedSet，若存在就更新权值
- ZREM key member 单次删除SortedSet中指定元素
- ZSCORE key member 获取SortedSet的指定元素的权值
- ZRANK key member 获取SortedSet的指定元素的排名
- ZCARD key 获取SortedSet中元素个数
- ZCOUNT key left right 统计权值在区间内所有元素的个数
- ZINCRBY key increment member 让SortedSet中的指定元素自增，步长为指定的increment值 
- ZRANGE key num1 num2 排序后获取指定排名范围内的元素（比如第2名-第10名）
- ZRANGEBYSCORE key min max 排序后，获取指定权值范围内的元素

### 相同数量情况下，分析hash和zset占用情况
- 如果节点数据≤128，hash和zset底层实现都是压缩列表
- 如果节点数据∈(128,512)，hash使用压缩列表，zset使用跳表，跳表时间复杂度O(log2n)，压缩列表O(n)，但跳表空间占用更高
- 如果节点数据>512，hash使用字典，zset使用跳表

### 备份如何执行
Rdis会单独创建(fork)一个子进程来进行持久化，先将数据写入到一个临时文件中，等持久化过程都结束了，再用这个临时文件替换上次持久化好的文件

### 持久化
Rdis是内存数据库，如果不将内存中的数据库状态保存到磁盘，那么一旦服务器进程退出，服务器中的数据库状态也会消失。所以Redis提供了持久化功能

#### RDB
redis将快照snapshot保存在dump.rdb文件中

![image](https://github.com/arqady01/cpp-interview/blob/main/database/image/RDB%E6%B5%81%E7%A8%8B.png)

##### RDB触发机制
- 满足config文件中save规则
- 执行flushall命令
- 退出redis

##### RDB优缺点
- 优点
    - dump.rdb是某一时间点上的数据快照，非常适合大规模数据恢复
    - 对数据完整性要求不高，可以复制到任何地方并加载，对灾难恢复、数据迁移很友好
    - 相比AOF性能更高，数据恢复速度更快
- 缺点
    - 需要一定的时间间隔进程操作，如果redis意外宕机，将丢失最近写入但未保存到快照中的那些数据
    - fork进程时，会占用一定的内存空间；而且父进程越多，fork时间也越长
    - 高并发时可能会频繁命中config中的save规则，导致fork操作和备份的频率变得不可控

#### AOF
将执行过的所有写入动作以日志形式记录下来（因为只有写才会对数据库发生改变），只许追加文件但不可以改写文件，redis启动之初会读取appendonly.aof文件重新构建数据

##### AOF优缺点
- 优点
    - 可以选择no， every second， always三种策略
        - always每次修改都会同步，文件完整性更好
        - every second是默认的策略，每秒同步一次，性能更优，但可能会丢失一秒的数据
        - no，从不同步，效率最高
- 缺点
    - aof文件通常比rdb文件更大，修复速度也更慢
    - 因为aof是写入操作，效率比rdb低，所以redis默认是用RDB做持久化

### 主从复制
将一台Redis服务器的数据（主节点master，写为主），复制到其他的Redis服务器（从节点slave，读为主），数据的复制只能由主节点到从节点（单向）

- 数据冗余：主从复制实现了数据的热备份，是持久化之外的一种数据冗余方式
- 故障恢复：当主节点出现问题时，可以由从节点提供服务，实现快速的故障恢复
- 负载均衡：在主从复制的基础上，配合读写分离，可以大大提高Redis服务器的并发量
- 高可用基石：除了上述作用以外，主从复制还是哨兵和集群能够实施的基础，因此主从复制是redis高可用的基础

#### 复制原理
slave连接到master后会发送一个sync同步命令，master启动后台的存盘进程，同时收集所有接收到的用于修改数据集命令，在后台进程执行完毕之后，master将传送整个数据文件到slave，并完成一次完全同步<br>

- 全量复制：salve在接收到数据库文件数据后，将其存盘并加载到内存中。
- 增量复制：master继续将新的所有收集到的修改命令依次传给slave,完成同步

### 哨兵模式
当master宕机后，需要手动把一台slave切换为主服务器，费事费力，还会造成一段时间内服务不可用，所以优先考虑哨兵模式<br>
哨兵作为一个独立的进程，通过哨兵发送命令，等待各redis服务器响应，从而监控多个redis实例

![image](https://github.com/arqady01/cpp-interview/blob/main/database/image/%E5%93%A8%E5%85%B5%E6%A8%A1%E5%BC%8F.png)

### 缓存雪崩
**大量缓存key**同一时间失效，导致请求打到数据库，造成后者宕机<br>
<br>举例<br>
双十一期间，所有用户打开淘宝就是进入首页，首页压力巨大，为提高并发，将网站首页数据都缓存到redis里，所有的key失效时间都是2小时，当天大量用户购物，结果2个小时过去了，redis中key缓存全部失效，此时redis中查询不到数据，只能去数据库中查询，造成数据库无法响应宕机

#### 解决方案
- 随机设置key失效时间，避免大量key集体失效
- 集群部署中，可将热点数据均匀分布在不同的redis库中也能预防key全部失效
- 定时器，在缓存失效前写入新的缓存

### 缓存穿透
redis缓存和数据库中**没有相关数据**（比如id < 0 的请求），redis中没有这样的数据，无法拦截，直接穿透到数据库

#### 解决方案
- 将不存在的数据缓存到redis中，设置键值对为null，并设置一个短期过期时间，避免过期时间过长影响正常用户使用
- 拉黑该P地址
- 对参数进行校验，拦截不合法参数
- 布隆过滤器 将所有可能存在的数据哈希映射到一个足够大的bitmap（位图）中，一定不存在的数据会被bitmap拦截掉，从而避免了对底层存储系统的查询压力

##### 布隆过滤器
布隆过滤器是一个很长的二进制向量和一系列hash映射函数。布隆过滤器可以用于检索一个元素是否在一个集合中，存在为1，不存在为0
- 优点
    - 可以高效查询，即某样东西一定不存在 或者 可能存在
    - 可以高效的进行插入
    - 相比于传统的List、Set、Map等数据结构，它占用空间更少，因为其本身并不存储任何数据（重点）
- 缺点
    - 返回的结果是存在误差的，但将误差率设置为无穷小影响性能
    - 一般不提供删除操作

### 缓存击穿
**某一个热点**key，不停的扛着高并发，当这个key在失效的一瞬间，持续的高并发就击破缓存打到数据库，导致宕机<br>
<br>举例<br>
阿里拍卖一块宝珀手表，设置有效期3小时，想着够拍卖了吧，谁知到2小时59分，还有很多人加价，突然该key过期，高并发请求直接到数据库，导致宕机

#### 解决方案
- 增大热点key的有效期
- 互斥锁，在第一个请求上用mutex锁住，其他请求拿不到锁就阻塞，等第一个请求完毕，再将数据缓存到redis，后面的线程解除阻塞发现有了缓存，直接走缓存

### redis缓存与数据库双写一致性

先更新数据库，再更新缓存（有缺陷）

![image](https://github.com/arqady01/cpp-interview/blob/main/database/image/%E5%85%88%E6%9B%B4%E6%96%B0%E6%95%B0%E6%8D%AE%E5%BA%93%E5%86%8D%E6%9B%B4%E6%96%B0%E7%BC%93%E5%AD%98.png)

先删除缓存，再更新数据库（有缺陷）

![image](https://github.com/arqady01/cpp-interview/blob/main/database/image/%E5%85%88%E5%88%A0%E7%BC%93%E5%AD%98%E5%86%8D%E6%9B%B4%E6%96%B0%E6%95%B0%E6%8D%AE%E5%BA%93.png)

先更新缓存，再更新数据库（有缺陷）<br>

**先更新数据库，再删除缓存**（合适的方案）

## ⚙️ mysql
[mysql](https://github.com/arqady01/mysql-linux)

<h1 id="1">设计模式</h1>

- 依赖倒置原则DIP

高层模块（稳定）不应该依赖于低层模块（变化），二者都应该依赖于抽象（稳定），抽象（稳定）不应该依赖于实现细节（变化），实现细节应该依赖于抽象（稳定）

- 单一职责原则SRP

一个类只应该做一件事

- 开放封闭原则OCP

对扩展开放，对更改封闭。类模块应该是可扩展的，但是不可修改。增加功能是通过增加代码来实现，而不是修改源代码

- 里氏替换原则LSP

子类必须能够替换他们的父类is-a。继承表达类型抽象

## 单例模式

<h3>饿汉式</h3>
饿汉式不存在线程安全问题

```cpp
#include <iostream>
class Singoton {
public:
	static Singoton* getInstance() {
		return ptr;
	} //类中的静态函数只能访问类中静态成员变量
	void show() {
		std::cout << "hungry\n";
	}
	Singoton(const Singoton& s) = delete;
	Singoton& operator=(const Singoton& s) = delete;
private:
	Singoton() {};
	static Singoton* ptr; //声明
};
Singoton* Singoton::ptr = new Singoton; //定义

int main() { //测试
	Singoton* op = Singoton::getInstance();
	op->show();
}
```

<h3>懒汉式</h3>
懒汉式存在线程安全问题

```cpp
#include <iostream>
class Singoton {
public:
	static Singoton* getInstance() {
		if (ptr == nullptr) {
			ptr = new Singoton();
		}
		return ptr;
	} //类中的静态函数只能访问类中静态成员变量
	void show() {
		std::cout << "lazy\n";
	}
	Singoton(const Singoton& s) = delete;
	Singoton& operator=(const Singoton& s) = delete;
private:
	Singoton() {};
	static Singoton* ptr; //声明
};
Singoton* Singoton::ptr = nullptr; //定义

int main() { //测试
	Singoton* op = Singoton::getInstance();
	op->show();
}
```
试想，现在有两个线程调用了Singoton::getInstance()方法，线程A首先进行判空操作，于是new了一个Singoton对象，此时发生了上下文切换，调度权给到了线程B，线程B首先也进行判空操作，发现为空，于是也new了一个Singoton对象，就发生了问题。<br>
* 双重检查锁定解决线程安全问题

```cpp
#include <mutex>
class Singoton {
public:
	static Singoton* getInstance() {
		mtx.lock();
		if (ptr == nullptr) {
			ptr = new Singoton();
		}
		mtx.unlock();
		return ptr;
	} //类中的静态函数只能访问类中静态成员变量
	void show() {
		std::cout << "lazy\n";
	}
	Singoton(const Singoton& s) = delete;
	Singoton& operator=(const Singoton& s) = delete;
private:
	Singoton() {};
	static Singoton* ptr; //声明
	static std::mutex mtx;
};
Singoton* Singoton::ptr = nullptr; //定义
std::mutex Singoton::mtx;
```
假想现在有三个线程A B C，线程A首先加锁，再判断、new对象，若此时发生了上下文切换，调度权给了线程B和线程C，但是他没有锁资源，进入阻塞态，最终调度权回归线程A，将锁头解锁，于是线程B和线程C自然就解除阻塞，进行判空操作，此时就会因为判空失败而直接返回，线程安全<br>
但是效率太低了，进行双重检查，在加锁之前再进行一次判空操作：

```cpp
static Singoton* getInstance() {
	if (ptr == nullptr) {
		mtx.lock();
		if (ptr == nullptr) {
			ptr = new Singoton();
		}
		mtx.unlock();
	}
	return ptr;
}
```

* 原子操作弥补双重检查的bug
因为`new`操作符在汇编中是对应的机器指令可能会发生重排，依旧会产生线程安全问题，可以采用原子操作来弥补

```cpp
#include <iostream>
#include <mutex>
#include <atomic>
class Singoton {
public:
	static Singoton* getInstance() {
		Singoton* task = sptr.load(); //将sptr从原子中取出备用
		if (task == nullptr) {
			mtx.lock();
			task = sptr.load();
			if (task == nullptr) {
				task = new Singoton();
				sptr.store(task);
			}
			mtx.unlock();
		}
		task = sptr.load();
		return task;
	} //类中的静态函数只能访问类中静态成员变量
	void show() {
		std::cout << "lazy\n";
	}
	Singoton(const Singoton& s) = delete;
	Singoton& operator=(const Singoton& s) = delete;
private:
	Singoton() {};
	static std::mutex mtx;
	static std::atomic<Singoton*> sptr;
};
std::mutex Singoton::mtx;
std::atomic<Singoton*> Singoton::sptr = nullptr; //定义

int main() { //测试
	Singoton* op = Singoton::getInstance();
	op->show();
}
```

* 静态对象（需要编译器兼容C++11标准）
```cpp
#include <iostream>
class Singoton {
public:
	static Singoton* getInstance() {
		static Singoton sn; //必须静态，不然出了大括号其生命周期就结束
		return &sn; //取地址并返回
	}
	void show() {
		std::cout << "lazy\n";
	}
	Singoton(const Singoton& s) = delete;
	Singoton& operator=(const Singoton& s) = delete;
private:
	Singoton() {};
};

int main() { //测试
	Singoton* op = Singoton::getInstance();
	op->show();
}
```

C++11规定，当逻辑进入一个未被初始化的声明变量，所有的并发操作都必须等待该变量完成初始化。<br>
静态变量在全局/静态区，只要在分区内，只要数据等于0，就是未被初始化的，比如`int a = 0;`，`int b;`；但是`int c = 1;`就是初始化过的变量<br>
所以`static Singoton sn;`也是未初始化的静态局部变量，于是所有的并发操作都会被阻塞

## 简单工厂模式

![simple_factory](https://github.com/arqady01/Cpp-interface/blob/main/resource/cpp_images/simple_factory.png)

- 优点
    - 封装性
    - 扩展性：增加新产品类容易，只需修改工厂类，客户端代码无需修改
- 缺点
    - 违反开闭原则：对扩展开放，对更改封闭。假如我有个汽车工厂，但是想做口罩，我是用简单工厂的逻辑去更改车厂吗？显然不是，我们应该新增一条口罩生产线

```cpp
#include <iostream>
#include <memory>
struct AbstractProtuct {
    virtual void show() = 0;
};
struct ProtuctA : public AbstractProtuct {
    virtual void show() override {
        std::cout << "产品A创建\n";
    }
};
struct ProtuctB : public AbstractProtuct {
    virtual void show() override {
        std::cout << "产品B创建\n";
    }
};
struct Factory {
    static std::unique_ptr<AbstractProtuct> createProtuct(const std::string& type) {
        if (type == "A") {
            return std::make_unique<ProtuctA>();
        } else if(type == "B") {
            return std::make_unique<ProtuctB>();
        }
        return nullptr;
    }
};

int main() {
    std::unique_ptr<AbstractProtuct> ptr = Factory::createProtuct("A");
    ptr->show();
}
```

## 工厂模式

- 优点：遵循开闭原则，实现了可扩展和更复杂的层次结构。明确了职责，具有多态性
- 缺点：如果业务增加，会使得系统中类的个数成倍增加，提高了代码的复杂度

![factory](https://github.com/arqady01/Cpp-interface/blob/main/resource/cpp_images/factory.png)

```cpp
struct Product {
    virtual void show() = 0;
};
struct Car : public Product {
    virtual void show() override {
        std::cout << "生产汽车\n";
    }
};
struct mask : public Product {
    virtual void show() override {
        std::cout << "生产口罩\n";
    }
};

struct Factory {
    virtual Product* create() = 0;
};
struct Car_Factory : public Factory {
    virtual Product* create() override {
        return new Car;
    }
};
struct Mask_Factory : public Factory {
    virtual Product* create() override {
        return new mask;
    }
};

int main() {
    Factory* factory = new Car_Factory;
    Product* product = factory->create();
    product->show();
}
```

## 抽象工厂模式

- 优点：增加分组比较容易，能大大减少工厂类的数量
- 缺点：类似于工厂模式，但是它每个工厂都能做别的事，比如汽车工厂，可以做到不仅能生产汽车也能生产口罩。所以抽象工厂添加新产品较复杂，需要改动AbstractFactory和自身工厂，抽象工厂的纯虚函数一变，其他工厂也要跟着改变

[抽象工厂模式示例](https://github.com/arqady01/cpp-interview/blob/main/design%20parton/%E6%8A%BD%E8%B1%A1%E5%B7%A5%E5%8E%82%E6%A8%A1%E5%BC%8F%E7%A4%BA%E4%BE%8B)

## 观察者模式

包含抽象、具体的观察者和被观察者<br>
当一个对象状态发生改变时，所有依赖他的对象都会得到事件通知

## 适配器模式

像一个类的接口转换为客户希望的另一个接口，使得原本由于接口不兼容而不能一起工作的类可以一起工作

## 策略模式

策略模式是一种设计模式，它使得一组算法可以在运行时互相替换。该模式使用组合方式将算法的使用和实现分离开来，这比继承更加灵活。支持开闭原则。

```cpp
struct Weapon {
    virtual void attack() = 0;
};
struct m416 : public Weapon {
    void attack() override {
        std::cout << "使用m416射击\n";
    }
};
struct iron : public Weapon {
    void attack() override {
        std::cout << "拿起铁锹攻击\n";
    }
};

struct Person {
    Person() = default;
    Person(Weapon* p):pw (p) {
        pw->attack();
    }
    Weapon* pw;
};
int main() {
    Weapon* weapon = new m416;
    Person s(weapon); //使用m416射击
}
```

## 外观模式

将复杂的子系统抽象到一个接口中管理，外界只需要通过该接口实现与子系统交互，不必直接与子系统交互

<h1 id="2">GDB调试</h1>

### 运行
- run(r)：运行程序，当遇到断点后，程序会在断点处停止运行，等待下一步命令
- continue(c)：继续执行，到下一个断点处（或运行结束）
- next(n)：单步跟踪程序，当遇到函数调用时，也不进入此函数体
- step(s)：单步调试如果有函数调用，则进入函数；与命令n不同，n是不进入调用的函数的
- until：当你厌倦了在一个循环体内单步跟踪时，这个命令可以运行程序直到退出循环体
- until+行号： 运行至某行，不仅仅用来跳出循环
- finish： 运行程序，直到当前函数完成返回，并打印函数返回时的堆栈地址和返回值及参数值等信息
- call 函数(参数)：调用程序中可见的函数，并传递“参数”，如：call gdb_test(55)
- quit(q)：退出gdb

### 断点
- break n (简写b n):在第n行处设置断点
- b fn1 if a＞b：条件断点设置
- break func (b func)：在函数func()的入口处设置断点
- delete n：删除第n个断点
- disable n：暂停第n个断点
- enable n：开启第n个断点
- info b：显示当前程序的断点设置情况
- delete breakpoints：清除所有断点

### 查看运行信息
- bt: 当前运行的堆栈列表
- bt backtrace: 显示当前调用堆栈
- up/down 改变堆栈显示的深度
- set args 参数:指定运行时的参数
- show args：查看设置好的参数
- info program：来查看程序的是否在运行，进程号，被暂停的原因

### 调试正在运行中的程序
新建一个窗口，`ps -ef|grep a.out`显示正在运行的a.out程序的线程ID，`gdb a.out -p ID号`进入gdb调试

### 结束进程
- killall根据名称终止正在运行的进程，而kill根据进程ID号（PID）终止进程
- pidof a.out：找到进程的PID，或者`ps -ef|grep a.out`
- kill -9 2551 2514 ：中止PID为2511和2514的进程，或者`killall -9 a.out`




<h1 id="chrono">boost::chrono</h1>

系统时间：`std::chrono::system_clock`<br>
获取当前系统时间：
`std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();`
time_point代表这是一个时间点类型，模板参数使用system_lock表明这个时间点是基于system_lock来计时的<br>

## 类型转换函数

- 时间点类型：`time_point_cast<>`
- 时长类型：`duration_cast<>`

## 时钟

- system_clock：实时时钟的挂钟时间
- steady_clock：绝不会调整的单位时钟
- high_resolution_clock：拥有可用的最短嘀嗒周期的时钟

### 共同成员

| 成员 | 说明 |
| :----: | :----: |
| now() | 返回当前时间，类型为clock::time_point |
| time_point | 当前时钟的时间点类型 |
| duration | 时钟的时长类型 |
| is_ready | 是否稳定时钟，对于steady_clock该值一定为true |

## 时长类型

- 纳秒：std::chrono::nanoseconds
- 微秒：std::chrono::microseconds
- 毫秒：std::chrono::milliseconds
- 秒：std::chrono::seconds
- 分：std::chrono::minutes
- 小时：std::chrono::hours

调用count()函数来获取具体数值

## 时长运算

- 向下取整：floor
- 向上取整：ceil
- 取绝对值：abs

```cpp
void translate() {
	std::chrono::hours h(2);
	std::chrono::minutes m(5);
	auto time = h + m;

	auto sec = std::chrono::duration_cast<std::chrono::seconds>(time);
	std::cout << sec.count() << std::endl;
}
```

#### 描述时长

C++14支持`std::chrono_literals`中的字面量来描述，h-小时，min-分钟，s-秒，ms-毫秒，us-微秒，ns-纳秒，示例中的2小时5分也可以这样表述：

```cpp
using namespace std::chrono_literals;
auto time = 2h + 5min;
```

## 时间点

时间点包含两个类型：时钟和时长。时间点 + 时长 = 时间点

```cpp
int main() {
	//计算程序运行时长
	auto start = std::chrono::steady_clock::now();
	double sum = 0;
	for (int i = 0; i < 1000000; i++) {
		sum += sqrt(i);
	}
	auto end = std::chrono::steady_clock::now();
	auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	std::cout << diff.count() << std::endl;
}
```
