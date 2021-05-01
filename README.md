# SVR for object-identify
#### 工程不使用第三方库，网络请求从 System Call 和 Standard C Library Function 写起.
#### 使用 Epoll + NetThreads + WorkerThreads 模型.

## 编译方法
### UNIX
```bash
git clone --recursive https://github.com/xingyuuchen/object-identify-SVR.git
cd object-identify-SVR/scripts
bash autogen.sh
```
仅编译
```bash
bash cmake.sh
```
编译运行
```bash
bash cmake.sh -r
```
以守护进程形式运行
```bash
bash cmake.sh -r -d   # -d: run as a daemon.
```

### Windows
不支持


## 使用方法
每一个网络接口使用一个类来表示。它们都间接继承自`NetSceneBase`类。

若使用Protobuf来序列化数据，请继承自`NetSceneProtobuf`，使用`POST`请求。

若想自定义网络通信协议，请继承自`NetSceneCustom`类。支持GET, POST。

定义好你的接口类并实现好具体的业务逻辑后，请将接口类注册给框架：使用
`NetSceneDispatcher::Instance::RegisterNetScene()`方法。

然后你可以更改配置文件`serverconfig.yml`中的内容，
再像我一样，调用`Server::Instance().Serve()`就能快乐地开启服务了！
