# SVR for object-identify
#### 工程不使用第三方库，网络请求从 System Call 和 Standard C Library Function 写起.
#### 使用 Epoll + ThreadPoll 实现高性能 IO.

## 编译方法
### UNIX
```
git clone https://github.com/xingyuuchen/object-identify-SVR.git
cd object-identify-SVR/scripts
bash autogen.sh
```
编译运行
```
bash cmake.sh -r -n   # -n (optinal) for nohup background running.
```
仅编译
```
bash cmake.sh
```

### Windows
不支持
