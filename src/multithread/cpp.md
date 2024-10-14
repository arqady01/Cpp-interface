# 线程id

每个线程都对应着线程id，可以通过`std::this_thread::get_id()`获取

# 创建的方式

- 通过非成员函数创建线程

```cpp
void func() { std::cout << "nmsl\n"; }
int main() {
    std::thread pthread(&func);
    std::cout << pthread.joinable();//返回true代表可以join或detach
    pthread.join(); //main函数会等待子线程执行完毕
    pthread.detach(); //main函数和子函数各走各的
}
```

- 普通成员函数创建线程

理解两个&的含义，第一个表示取成员函数的地址，第二个表示类的实例化对象地址

```cpp
struct test {
    void print1(int a){
        std::cout << "子线程" << a << "\n";
    }
    void print2(){
        std::cout << "子线程2\n";
    }
};
int main() {
    test t;
    std::thread pth(&test::print1, &t, 1);
    pth.join();
    //换种方式
    std::thread(&test::print2, &t).join();
}
```

- 通过仿函数创建线程

```cpp
struct test {
	void operator()(int a){
		std::cout << a;
	}
};
int main() {
	test t;
	std::thread pthread(t, 5); //可以仔细对比上一种方式
	pthread.join();
	std::cout << "main函数\n";
}
```

- 类中创建线程

借助this指针

```cpp
struct A {
    A() {
        std::thread pth(&A::test, this, 5); //依然需要实例化对象地址，为this
        pth.join();
    }
    void test(int num) {cout << "num=" << num << endl;}
};
int main() {
    A a;
}
```

- 通过lambda创建线程

```cpp
int main() {
    auto print = [](){ std::cout << "子线程\n"; };
    std::thread pthread(print);
    pthread.join();
    std::cout << "main函数\n";
}
```

## 创建并等待多个线程

- 利用vector

```cpp
void print() {
    std::cout << "线程id：" << std::this_thread::get_id() << std::endl;
}
int main() {
    std::vector<std::thread> v;
    for (int i = 0; i < 10; ++i) {
        v.push_back(std::thread(&print));
        v[i].join();
    }
}
```

- 利用数组

```cpp
void print() {
    std::cout << "线程id：" << std::this_thread::get_id() << std::endl;
}
int main() {
    std::thread ps[10];
    for (int i = 0; i < 10; ++i) {
        ps[i] = std::thread(&print);
        ps[i].join();
    }
}
```

# 例题：计算二维数组的最大值

拦腰将二维数组切成两半，上下分别求各自的最大值

```cpp
int matrix[4][4] = {{1,5,3,6},{22,80,4,34},{4,45,67,3},{99,3,23,3}};
std::vector<int> ans(2, -1);
struct Find {
    void find(int start, int end) {
        int max = matrix[start][0];
        for (int i = start; i < end; i++) { //二维数组的纵向
            for (int j = 0; j < 4; j++) { //二维数组的横向
                max = matrix[i][j] > max ? matrix[i][j] : max;
            }
        }
        if (0 == start) {
            ans[0] = max; //ans[0]保存切片1中的最大数
        } else if (2 == start) { //ans[1]保存切片2中的最大数
            ans[1] = max;
        }
    }
};
int main() {
    Find find; //实例化
    std::thread(&Find::find, &find, 0, 2).join(); //&find表示取地址，对应this指针
    std::thread(&Find::find, &find, 2, 4).join();
    std::cout << ans[0] << " " << ans[1] << std::endl;
}
```

# std::lock()

多个线程尝试用lock()上锁，只有一个线程能上锁成功，如果没有锁头上锁成功，那么流程将卡在lock()这里

所以lock()只可能出现以下的情况：

- 如果所有的互斥量都没有被锁定，那么lock()会给它们都上锁
- 只要有一个互斥量已经被其他线程锁定，那么lock()会阻塞当前线程，直到所有的互斥量都可用，并给它们都上锁

## 生产-消费者模型

线程1负责收集玩家指令并把命令数据写到队列中；线程2负责从队列中取出并执行命令（有点像进程间通信方式之一的消息队列）

实现思路：使用成员函数作为线程函数，做多线程

```cpp
#define NUM 10000
class test {
public:
    //线程1：把命令塞到消息队列中
    void PushMsg() {
        for (int i = 0; i < NUM; ++i) {
            mtx.lock(); //上锁
            msgQueue.push_back(i); //假设i就是指令，塞到消息队列中
            mtx.unlock(); //解锁
        }
    }
    //线程2：提取消息队列中的命令
    void GetMsg() {
        for (int i = 0; i < NUM; ++i) {
            mtx.lock(); //上锁
            if (!msgQueue.empty()) {
                int id = msgQueue.front(); //从头端取出命令
                msgQueue.pop_front(); //取出后就移除
                std::cout << id << std::endl;
            } else {
                std::cout << "消息队列为空\n";
            }
            mtx.unlock(); //解锁
        }
    }
private:
    std::list<int> msgQueue; //指令容器
    std::mutex mtx; //互斥锁
};
int main() {
    test t;
    std::thread inThread(&test::PushMsg, &t);
    std::thread getThread(&test::GetMsg, &t);
    inThread.join();
    getThread.join();
}
```

# RAII之lock_guard<>

lock_guard构造函数中执行lock，作用域结束后，调用析构函数，执行unlock，如此一来就取代了lock和unlock

```cpp
std::mutex mtx; //创建互斥量对象
void test() {
    //定义lock_guard，传入互斥量对象，实现自动加锁
    std::lock_guard<std::mutex> guard(mtx);
    std::cout << "hello thread" << std::endl;
    //lock_guard对象在离开作用域时自动解锁
}
int main() {
    std::thread t(test);
    t.join();
    std::cout << "hello main" << std::endl;
}
```

- `lock_guard`的copy ctor和operator= ctor是私有的，所以lock_guard是不可复制和转移的
- `lock_guard`无法手动上下锁，只能在ctor和dtor中自动执行，因此它的锁的粒度是固定的
- `lock_guard`的效率比较高，因为它不需要维护锁的状态
- `lock_guard`比lock / unlock更安全：因为`lock_guard`结束时，锁会自动释放，不论是否有异常
```cpp
std::mutex mtx;
void riskyFunction() {
    mtx.lock();  // 手动加锁
    // 如果在此处发生异常，下面的unlock将不会执行，导致死锁
    throw std::runtime_error("Something went wrong!");
    mtx.unlock();  // 手动解锁
}
```

# RAII之unique_lock<>

unique_lock比lock_guard灵活，也一样无需手动上下锁，但占用更大的内存

1. unique_lock可以在构造时指定不同的锁策略
    1. `std::try_to_lock`，尝试锁定，所以不能提前lock。上锁失败，也不会阻塞，程序可以决定是否继续执行其他逻辑
```cpp
void task() {
    std::unique_lock<std::mutex> lock(mtx, std::try_to_lock); // 尝试锁定，不会阻塞
    if (lock.owns_lock()) {
        std::cout << "Thread " << std::this_thread::get_id() << " acquired the lock." << std::endl;
    } else {
        std::cout << "Thread " << std::this_thread::get_id() << " could not acquire the lock." << std::endl;
    }
}
```
    2. `std::defer_lock`，延迟锁定，初始化了一个未上锁的mutex，也不能提前lock。不加锁是因为以后可以调用成员方法
```cpp
void task() {
    std::unique_lock<std::mutex> lock(mtx, std::defer_lock); // 不立即锁定
    // 在稍后需要时手动加锁
    lock.lock();
    std::cout << std::this_thread::get_id() << " acquired the lock.";
    // 临界区代码
    lock.unlock(); // 手动解锁
}
```
    3. `std::adopt_lock`，接管锁定，互斥量必须提前上锁否则异常。表现为线程已拥有互斥量的所有权，不需要再次锁定
```cpp
void task() {
    mtx.lock();  // 提前手动锁定互斥量

    // 使用 adopt_lock 告诉 unique_lock，互斥量已经被锁定，不需要再锁定
    std::unique_lock<std::mutex> lock(mtx, std::adopt_lock);

    // 临界区代码
    std::cout << std::this_thread::get_id() << " is working in critical section.\n";
}
```

2. defer_lock特有的成员函数
    - lock()，对于未上锁的unique_lock，可以通过lock()进行上锁
    - try_lock()，尝试给互斥量上锁，成功返回true，失败返回false
    - try_lock_for(std::chrono::seconds(n))，若上锁不成功，那就等待n个时间单位，时间一到还未获得锁资源，就退出不再等待
3. unlock()函数，对于上锁的互斥量，可以暂时解锁来运行一些不需要共享数据的代码
4. release()函数，返回互斥量对象指针，并释放 unique_lock 对互斥量的管理。release()后必须要给上锁的互斥量解锁，否则报错
```cpp
std::mutex mtx;
std::unique_lock<std::mutex> lck(mtx);
std::mutex* ptr = lck.release(); //所有权由ptr接管
ptr->unlock(); //手动解锁，不然互斥量一直被锁定
```
5. owns_lock()，返回布尔值，指示当前是否持有锁
6. move()函数，转移所有权，但不能复制。

```cpp
std::unique_lock<std::mutex> lk1(mtx);
std::unique_lock<std::mutex> lk2 = std::move(lk1);  //转移所有权
```

举个例子，这个cplusplus.com的示例代码：

```cpp
std::mutex mtx; //全局互斥量
void print_block(int n, char c) {
  //unique_lock在构造时尝试锁定互斥量，如果锁定失败，会立即返回
  std::unique_lock<std::mutex> lck(mtx, std::try_to_lock); //尝试上锁
  //检查是否成功获取锁
  if (lck.owns_lock()) {
    //打印n个字符c
    for (int i = 0; i < n; ++i) {
      std::cout << c;
    }
    std::cout << '\n';
  } else { //如果没有获取锁，打印另一条信息
    std::cout << "lock failed: " << c << '\n';
  }
}

int main() {
  //创建两个线程，分别打印10个*和10个#
  std::thread th1(print_block, 10, '*');
  std::thread th2(print_block, 10, '#');
  th1.join();
  th2.join();
}
```

可能正常输出，也可能失败，取决于哪个线程先锁定互斥量

# feat.单例模式

```cpp
//懒汉式
class singoton {
public:
    //ctor为私有导致无法创建对象，static可以保证不创建对象就能调用函数
    static singoton* getPtr() {
        if (nullptr == sn) sn = new singoton();
        return sn;
    }
    void print() { std::cout << "singoton mode\n"; }
private:
    struct CS {
        
    };
private:
    singoton() {}; //ctor设为私有
    //解释1：静态成员函数只能访问静态成员变量，所以*sn设置为静态
    //解释2：未创建对象就调用getPtr()是ok的，但是*sn也要提前创建，所以设为静态
    static singoton* sn;
};
singoton* singoton::sn = nullptr; //类外定义

int main() {
    //创建类的唯一对象，类似于 singoton* ptr = new singoton();
    singoton* ptr = singoton::getPtr();
    //因为sn不为空，调用getPtr，*sn也会原封不动的传出来
    singoton* ptr2 = singoton::getPtr();
    ptr->print();
}
```

假设在主函数中，有两个线程调用单例模式，假设线程A判断完指针为空，正准备创建指针，调度权给到了线程B，刚好指针也为空，所以B创建成功

调度权回到了A，A不会再判断了，直接创建。此时创建了两个，出现bug

```cpp
//不放到类内，是因为静态成员函数只能访问静态成员，除非也定义成静态
std::mutex mtx;

class singoton {
public:
    //ctor为私有导致无法创建对象，static可以保证不创建对象就能调用函数
    static singoton* getPtr() {
        std::unique_lock<std::mutex> lck(mtx); //自动上下锁
        if (nullptr == sn) sn = new singoton();
        return sn;
    }
    void print() { std::cout << "singoton mode\n"; }
private:
    //垃圾回收：类中类
    struct CS {
    public:
        ~CS() {
            delete singoton::sn;
            singoton::sn = nullptr;
        }
    };
    static CS cs; //实例化成静态，那么类外就必须要定义了
private:
    singoton() {}; //ctor设为私有
    //解释1：静态成员函数只能访问静态成员变量，所以*sn设置为静态
    //解释2：未创建对象就调用getPtr()是ok的，但是*sn也要提前创建，所以设为静态
    static singoton* sn;
};
singoton* singoton::sn = nullptr; //懒汉式
singoton::CS singoton::cs; //必须初始化，不然程序结束无法析构CS

void test() {
    std::cout << "start\n";
    singoton* ptr = singoton::getPtr();
    ptr->print();
    std::cout << "end\n";
}
int main() {
    std::thread(test).join();
    std::thread(test).join();
}
```

## 效率改进：双重检查锁

```cpp
static singoton* getPtr() {
    if (nullptr == sn) { //双重检查，提高效率
        std::unique_lock<std::mutex> lck(mtx); //自动上下锁
        if (nullptr == sn) sn = new singoton();
    }
    return sn;
}
```

如果sn非空，说明指针sn一定被创建过

如果sn为空，不代表指针sn一定没被创建过，比如假设线程A正要创建时，调度权给到了线程B，因为指针sn为空，所以线程B创建，结束后调度权给到了A，A随后就创建

所以，只需要针对sn判断，若指针sn非空，一定被创建过，直接返回即可；若指针为空，因为不确定，所以需要互斥锁保护

# std::call_once

确保某函数只执行一次，也就具备互斥量的能力。常用的场景如初始化操作或一些系统参数的获取等

需要搭配`std::once_flag`一起使用，原理就是检查标志位是否被改写，有就说明函数被执行过；没有被改写就说明函数未执行

```cpp
std::once_flag flag; //定义一个once_flag对象
void init() {
    std::cout << "初始化...." << std::endl;
}

void func() {
    std::call_once(flag, init);
}

int main() {
    //创建两个线程，都调用Init函数
    std::thread(func).join(); 
    std::thread(func).join();
}
```

init()只被调用了一次，而其他线程则等待该函数执行完成后返回

# 条件变量

条件变量允许一个线程等待另一个线程满足某个条件

条件变量通常与互斥锁一起使用，以确保对共享资源的访问是互斥的。

- wait()：使当前线程等待，直到某个条件满足
- notify_one()：唤醒一个正在等待该条件变量的线程
- notify_all()：唤醒所有正在等待该条件变量的线程

```cpp
class Msg {
public:
    void pushQueue() {
        for (int i = 0; i < 100; ++i) {
            std::unique_lock<std::mutex> lck(mtx); //自动上下锁
            MsgQueue.push_back(i); //假设i就是消息，将其入队
            cv.notify_one(); //尝试唤醒wait()
        }
    }
    void getQueue() {
        for (int i = 0; i < 100; ++i) {
            std::unique_lock<std::mutex> lck(mtx); //自动上下锁
            cv.wait(lck, [this](){ return !MsgQueue.empty(); }); //等待消息队列非空
            //流程只要能走到这里，说明互斥锁一定锁着，因为wait被唤醒了会干活
            if (!MsgQueue.empty()) { //消息队列不为空
                int msg = MsgQueue.front(); //取出队首消息
                std::cout << msg << std::endl;
                MsgQueue.pop_front();
            } else {
                std::cout << "queue is empty" << std::endl;
            }
        }
        std::cout << std::endl;
    }
private:
    std::list<int> MsgQueue; //消息队列
    std::mutex mtx; //互斥锁
    std::condition_variable cv; //条件变量
};
int main() {
    Msg g;
    std::thread(&Msg::pushQueue, &g).join();
    std::thread(&Msg::getQueue, &g).join();
}
```

## 虚假唤醒

如果wait不指定第二参数，那么第二参数默认为false，如果使用不当就会造成虚假唤醒

```cpp
std::deque<int> q;
std::mutex mtx;
std::condition_variable cv;
void producer() {
    int i = 0;
    while (true) {
        std::unique_lock<std::mutex> lck(mtx);
        q.push_back(i);
        cv.notify_one();
        i++;
    }
}
void consumer() {
    while (true) { 
        std::unique_lock<std::mutex> lck(mtx);
        if (q.empty()) { cv.wait(lck); } //🤣
        int value = q.front();
        std::cout << value;
        q.pop_front();
    }
}
```

🤣处，if只会执行一次，一旦线程唤醒并得到锁，就不会再判断条件是否满足；而while不会

或者用lambda去指定wait的第二参数也可以，这也是推荐做法

## leetcode1116 打印零与奇偶数

定义一个printNumber(x)函数，他会输出数字x，给定一个ZeroEvenOdd实例，其中

- 线程A：调用Zero()，只输出0
- 线程B：调用even()，只输出偶数
- 线程C：调用odd()，只输出奇数

示例，输入n = 5，输出0102030405

```cpp
class ZeroEvenOdd {
private:
    int n;
    int flag = 0; //标志位
    std::mutex mtx;
    std::condition_variable cv;
public:
    ZeroEvenOdd(int n) {
        this->n = n;
    }

    // printNumber(x) outputs "x", where x is an integer.
    void zero(function<void(int)> printNumber) {
        for (int i = 1; i <= n; ++i) {
            std::unique_lock<std::mutex> lck(mtx);
            cv.wait(lck, [this](){return flag == 0;});
            printNumber(0);
            flag = i % 2 == 0 ? 1 : 2; //i是偶数就把标志位设为1，奇数设为2
            cv.notify_all(); //0打印好了，唤醒其他阻塞的线程
        }
    }

    void even(function<void(int)> printNumber) {
        for (int i = 2; i <= n; i += 2) {
            std::unique_lock<std::mutex> lck(mtx);
            cv.wait(lck, [this](){return flag == 1;});
            printNumber(i);
            flag = 0; //唤醒线程A
            cv.notify_all();
        }
    }

    void odd(function<void(int)> printNumber) {
        for (int i = 1; i <= n; i += 2) {
            std::unique_lock<std::mutex> lck(mtx);
            cv.wait(lck, [this](){return flag == 2;});
            printNumber(i);
            flag = 0; //唤醒线程A
            cv.notify_all();
        }
    }
};
```

# std::promise & std::future

利用勾股定理求第三边边长，单线程做法：

```cpp
void func(int a, int b, int& c) {
    int t_a = a * a;
    int t_b = b * b;
    c = std::sqrt(t_a + t_b);
}
int main() {
    int ans = 0;
    func(3, 4, std::ref(ans));
    std::cout << ans << std::endl;
}
```

C++20多线程打通了主线程和子线程的连接通道

子线程通过promise的set_value()来赋值，主线程通过future的get()来取值

```cpp
#include <future>
void func(int a, int b, std::promise<int>& c) {
    int t_a = a * a;
    int t_b = b * b;
    c.set_value(std::sqrt(t_a + t_b));
}
int main() {
    //只是想从子线程中获取返回值
    //第一步：初始化两个类对象
    std::promise<int> p;
    std::future<int> f = p.get_future();
    std::thread t(func, 3, 4, std::ref(p));
    //get()是阻塞函数，直到set_value()执行才会转为运行态
    std::cout << f.get() << std::endl;
    t.join();
}
```

升级一下，将数值设置放到主函数中做，将边长数据在主线程中传递给子线程

```cpp
void func(std::future<int>& a, std::future<int>& b, std::promise<int>& c) {
    //get()被调用，它会移动（而非复制）结果，使得future对象不再持有任何值
    //之后再次调用get()会导致未定义行为，所以不能多次调用get()函数
    int t_a = pow(a.get(), 2);
    int t_b = pow(b.get(), 2);
    c.set_value(sqrt(t_a + t_b)); //写入promise对象
}
int main() {
    //将promise对象和future对象绑定
    std::promise<int> a;
    std::future<int> p_a = a.get_future();
    std::promise<int> b;
    std::future<int> p_b = b.get_future();
    std::promise<int> c;
    std::future<int> p_c = c.get_future();
    a.set_value(5);
    b.set_value(12);
    std::thread t(func, std::ref(p_a), std::ref(p_b), std::ref(c));
    std::cout << p_c.get() << std::endl;
    t.join();
}
```

# std::async

用来启动一个异步任务（自动创建一个线程并执行对应的线程入口函数），随后返回一个std::future对象

```cpp
int test(int a, int b) {
    int t_a = a * a;
    int t_b = b * b;
    return sqrt(t_a + t_b);
}
int main() {
    //std::future<int> ans = std::async(std::launch::async, test, 3, 4);
    std::future<int> ans = std::async(test, 3, 4);
    //get()会一直等待结果返回，否则就一直阻塞
    std::cout << ans.get() << std::endl;
}
```

std::async可以和参数`std::launch::async`和`std::launch::deferred`一起使用，如果不指定，系统自行决定

- std::launch::async，强制异步执行任务
- std::launch::deferred，任务被延迟，直到调用get()才执行
