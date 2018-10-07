# 环境依赖

## client
* Qt 5.8
* openSSL 1.0.2o_1
* C++ 14

## server
* C++ 14
* openSSL 1.0.2o_1

# 架构
![](img/architecture.png)

# 流程图
## 用户流程图
![](img/flow.png)

## 管理员流程图
![](img/adminFlow.png)

# 数据结构
## model
使用到的两个model类，并存入sqlite数据库中
### UserModel
* std::string uuid;
* std::string username;
* std::string password;
* int balance;
* int privilege;
* long lastModifyTime;

### OrderModel
* std::string uuid;
* int type;
* int amount;
* std::string out_account;
* std::string in_account;
* long record_time;


## client

## server


# 结果验证
## 登录注册界面
![](img/example/login.png)
登录

![](img/example/signup.png)
注册用户，注册用户权限为普通用户。

## 主界面
### 管理员主界面
![](img/example/lobby.png)

### 用户主界面
![](img/example/userLobby.png)

### 用户修改密码
![](img/example/userChangePassword.png)

## 业务功能
### 充值
![](img/example/deposit.png)

### 取款
![](img/example/withdraw.png)

### 转账
![](img/example/transfer.png)

## 管理员功能

### 用户查询

#### 所有用户
![](img/example/allUser.png)

#### 模糊查询
![](img/example/userSearch.png)
根据用户名进行模糊搜索，查询用户信息

#### 批量查询
![](img/example/dateSearch.png)
根据最后操作时间进行搜索，查询用户信息

### 管理员操作用户
#### 创建新用户/新管理员账户
![](img/example/createUser.png)

#### 修改用户名或密码
![](img/example/modifyUser.png)

#### 删除用户
![](img/example/deleteUser.png)

###  订单查询
![](img/example/orderSearch.png)
根据用户名进行模糊搜索，查询订单信息