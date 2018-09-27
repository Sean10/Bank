/**
 * @brief client socket连接成员函数文件
 * 
 * @file client.cpp
 * @author 
 * @date 2018-09-05
 */
#include "client.h"
#include "../define.h"

using namespace Sean_Socket;

/**
 * @brief Construct a new Client:: Client object
 * 
 * @param name 用户名
 */
Client::Client(string name)
    : username_(name)
{
    /* 初始化OpenSSL */
    SSL_library_init();
    /*加载算法库 */
    OpenSSL_add_ssl_algorithms();
    /*加载错误处理信息 */
    SSL_load_error_strings();
    /* 选择会话协议 */
    meth = (SSL_METHOD *)TLSv1_client_method();
    /* 创建会话环境 */
    ctx = SSL_CTX_new(meth);
    if (NULL == ctx)
        exit(1);
    /* 制定证书验证方式 */
    SSL_CTX_set_verify(ctx, SSL_VERIFY_PEER, NULL);

    /* 为SSL会话加载CA证书*/
    SSL_CTX_load_verify_locations(ctx, CACERT, NULL);

    /* 为SSL会话加载用户证书 */
    if (0 == SSL_CTX_use_certificate_file(ctx, MYCERTF, SSL_FILETYPE_PEM))
    {
        ERR_print_errors_fp(stderr);
        exit(1);
    }
    /* 为SSL会话加载用户私钥 */
    if (0 == SSL_CTX_use_PrivateKey_file(ctx, MYKEYF, SSL_FILETYPE_PEM))
    {
        ERR_print_errors_fp(stderr);
        exit(1);
    }
    /* 验证私钥和证书是否相符 */
    if (!SSL_CTX_check_private_key(ctx))
    {
        printf("Private key does not match the certificate public key\n");
        exit(1);
    }
    /* 设置随机数 */
    srand((unsigned)time(NULL));
    for (int i = 0; i < 100; i++)
        seed_int[i] = rand();
    RAND_seed(seed_int, sizeof(seed_int));
    /* 指定加密器类型 */
    SSL_CTX_set_cipher_list(ctx, "RC4-MD5");
    SSL_CTX_set_mode(ctx, SSL_MODE_AUTO_RETRY);

    std::cout << "begin..." << std::endl;
    connectSocket_ = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == connectSocket_)
    {
        throw std::runtime_error("Cannot create socket");
    }

    // 填写客户端地址信息
    // 使用inet_addr需要将IP地址转换为网络格式
    serverAddr_.sin_family = AF_INET;
    serverAddr_.sin_port = htons(SERVER_PORT);
    //serverAddr_.sin_addr.s_addr = inet_addr(CLIENT_ADDR);
    auto res = inet_pton(AF_INET, SERVER_ADDR, &serverAddr_.sin_addr);
    if (res < 0)
    {
        close(connectSocket_);
        throw std::runtime_error("First parameter is not a valid address family");
    }
    else if (res == 0)
    {
        close(connectSocket_);
        throw std::runtime_error("Second parrameter does not contain valid ipaddress");
    }

    //设置发送时限
    int nNetTimeout = 1000;
    setsockopt(connectSocket_, SOL_SOCKET, SO_SNDTIMEO, &nNetTimeout, sizeof(int));

    cout << "Initial success" << endl;

}

/**
 * @brief Destroy the Client:: Client object
 * 
 */
Client::~Client()
{
    shutdown(connectSocket_, 2);
    //关闭连接
    SSL_shutdown(ssl);
    SSL_free(ssl);
    SSL_CTX_free(ctx);
    close(connectSocket_);
}


/**
 * @brief Client与服务器连接功能
 * 
 * @param requestInfo json序列化后请求信息
 * @return std::string json序列化后的返回信息
 */
std::string Client::Connect(std::string requestInfo)
{
    auto trys = 2;
    auto success = false;

    // 多次尝试
    while(trys--)
    {
        //与服务器建立连接
        if (connect(connectSocket_, reinterpret_cast<sockaddr *>(&serverAddr_),
                    sizeof(serverAddr_))== -1)
        {
            cout << "Unable to connect to server... " << std::to_string(2 - trys) << " trys" << endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            continue;
        }
        cout << "Connect success" << endl;
        /* 创建一个SSL套接字*/
        ssl = SSL_new(ctx);
        if (NULL == ssl)
            exit(1);

        /* 申请一个ssl套接字 */
        if (0 >= SSL_set_fd(ssl, connectSocket_))
        {
            printf("Attach to Line fail!\n");
            exit(1);
        }
        //建立SSL连接
        int k = SSL_connect(ssl);
        if (0 == k)
        {
            printf("%d\n", k);
            printf("SSL connect fail!\n");
            exit(1);
        }
        printf("connect to server\n");

        success = true;
        break;
    }
    if (!success)
    {
        close(connectSocket_);
        throw std::runtime_error("Unable to connect to server, Please try later");
    }

    // 连接成功建立后，发送请求
    return std::move(Send(requestInfo));
}

/**
 * @brief 发送数据包函数
 * 
 * @param requestInfo json序列化后请求信息
 * @return std::string json序列化后的返回信息
 */
std::string Client::Send(std::string requestInfo)
{
    int recvBufLen = DEFAULT_BUFFER_LEN;
    char recvBuf[DEFAULT_BUFFER_LEN];

    cout << "[INFO] request: " << requestInfo << endl;
    char sendBuf[DEFAULT_BUFFER_LEN];
    int sendBufLen = DEFAULT_BUFFER_LEN;
    strcpy(sendBuf, requestInfo.c_str());

    // 向服务器发送数据
    //    if (send( connectSocket_, sendBuf, sendBufLen, 0) == -1)
    if (SSL_write(ssl, sendBuf, sendBufLen) == -1)
    {
        //关闭连接
        SSL_shutdown(ssl);
        SSL_free(ssl);
        SSL_CTX_free(ctx);
        close( connectSocket_);
        throw std::runtime_error("Failed at send message");
    }
    cout << "[INFO] send complete" << endl;
    // 等待接受服务器的返回信息
    //    if (recv( connectSocket_, recvBuf, recvBufLen, 0) <= 0)
    if (SSL_read(ssl, recvBuf, recvBufLen) <= 0)
    {
        //关闭连接
        SSL_shutdown(ssl);
        SSL_free(ssl);
        SSL_CTX_free(ctx);
        close( connectSocket_);
        throw std::runtime_error("Failed at receive message");
    }

    cout << "[INFO] receive: " << recvBuf << endl;

    return std::move(std::string(recvBuf));
}

/**
 * @brief 关闭与服务器连接函数
 * 
 */
void Client::Close()
{
    //关闭连接
    SSL_shutdown(ssl);
    SSL_free(ssl);
    SSL_CTX_free(ctx);
    close(connectSocket_);
}

/**
 * @brief Set the Privilege object
 * 
 * @param privilege 权限等级
 */
void Client::SetPrivilege(int privilege)
{
    this->privilege_ = privilege;
}

/**
 * @brief Get the Privilege object
 * 
 * @return int 权限等级
 */
int Client::GetPrivilege()
{
    return this->privilege_;
}


