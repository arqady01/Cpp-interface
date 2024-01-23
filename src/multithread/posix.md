# 线程ID

typedef unsigned long pthread_t;

# 线程创建

创建成功返回0，失败返回非零的错误码

```cpp
int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void* (*start)(void*), void *arg);
参数解释：
thread：一个指向 pthread_t 类型的指针，用于接收新创建线程的标识符
attr：设置线程的属性。如果设置为NULL，则使用默认属性
start：一个函数指针，指向线程开始执行时调用的函数
arg：用于传递给start函数的参数
```

# 线程回收

```cpp
int pthread_join(pthread_t thread, void** args);
参数解释：
thread：被回收的线程ID
args：指向void指针的指针，用于存储被等待线程的退出状态数据
```

# 线程退出

线程可以显式调用 pthread_exit 函数来退出

```cpp
void pthread_exit(void* exit);
参数解释：
exit是线程退出时携带的参数
```

举例

```cpp
#include <pthread.h>
struct people {
    int age = 18;
    int years = 4;
};
//长大了，年龄增加
void* grow(void* args) {
    people* p = (people*)args; //类型转换
    p->age += p->years;
    pthread_exit(&p); //线程退出携带上p
    return NULL;
}
int main() {
    people p;
    pthread_t tid;
    //第三项，&grow和grow都可以，因为函数名本身就是个指针
    pthread_create(&tid, NULL, &grow, &p);
    pthread_join(tid, NULL);
    std::cout << p.age << std::endl; //22
}
```

# 线程同步

线程A和B共同管理number，两个线程的动作都是先从number取得数据随后递增，写回到变量中，如何实现？

```cpp
int num = 0;
void* A(void*) {
    for (int i = 0; i < 50; ++i) {
        int t = num;
        ++t;
        num = t; //写回
    }
    return NULL;
}
void* B(void*) {
    for (int i = 0; i < 50; ++i) {
        int t = num;
        ++t;
        num = t; //写回
    }
    return NULL;
}
int main() {
    pthread_t tid[2];
    pthread_create(&tid[0], NULL, &A, NULL);
    pthread_create(&tid[1], NULL, &B, NULL);
    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);
}
```

乍一看是对的，但是CPU对应寄存器、一级、二级、三次缓存，都是独占的，两个线程需要分时分段去抢占时间片，所以引出互斥锁

## 互斥锁

```cpp
//初始化
int pthread_mutex_init(pthread_mutex_t* mutex, const pthread_mutexattr_t* attr);
//上锁，上锁失败会阻塞等待
int pthread_mutex_lock(pthread_mutex_t* mutex);
//尝试上锁，上锁失败直接返回不会阻塞
int pthread_mutex_trylock(pthread_mutex_t* mutex);
//解锁
int pthread_mutex_unlock(pthread_mutex_t* mutex);
//销毁
int pthread_mutex_destroy(pthread_mutex_t* mutex);

参数解释：
mutex：互斥量的地址
attr：互斥锁的属性，默认为NULL即可
```

问题代码解决：

```cpp
int num = 0;
pthread_mutex_t mtx; //创建互斥锁对象
void* A(void*) {
    for (int i = 0; i < 50; ++i) {
        pthread_mutex_lock(&mtx); //上锁
        int t = num;
        ++t;
        num = t; //写回
        std::cout << num << std::endl;
        pthread_mutex_unlock(&mtx); //解锁
    }
    return NULL;
}
void* B(void*) {
    for (int i = 0; i < 50; ++i) {
        pthread_mutex_lock(&mtx); //上锁
        int t = num;
        ++t;
        num = t; //写回
        std::cout << num << std::endl;
        pthread_mutex_unlock(&mtx); //解锁
    }
    return NULL;
}
int main() {
    pthread_t tid[2];
    pthread_mutex_init(&mtx, NULL); //初始化
    pthread_create(&tid[0], NULL, &A, NULL);
    pthread_create(&tid[1], NULL, &B, NULL);
    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);
    pthread_mutex_destroy(&mtx);
}
```
