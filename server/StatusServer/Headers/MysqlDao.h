#ifndef GATESERVER_MYSQLDAO_H
#define GATESERVER_MYSQLDAO_H

#include "const.h"

#include <thread>
#include <jdbc/mysql_driver.h>
#include <jdbc/mysql_connection.h>
#include <jdbc/cppconn/resultset.h>
#include <jdbc/cppconn/statement.h>
#include <jdbc/cppconn/exception.h>
#include <jdbc/cppconn/prepared_statement.h>

class SqlConnection {
public:
    SqlConnection(sql::Connection *con, int64_t lasttime) : _con(con), _last_time(lasttime) {}

    std::unique_ptr<sql::Connection> _con;
    int64_t _last_time;
};

class MysqlConnPool {
public:
    MysqlConnPool(const std::string &url, const std::string &user, const std::string &pass,
                  const std::string &schema, int size);

    void checkConnection();

    std::unique_ptr<SqlConnection> getConnection();

    void returnConnection(std::unique_ptr<SqlConnection> con);

    void Close();

    ~MysqlConnPool();

private:
    std::string url_;
    std::string user_;
    std::string pass_;
    std::string schema_;
    int size_;
    std::queue<std::unique_ptr<SqlConnection>> pool_;
    std::mutex mutex_;
    std::condition_variable cond_;
    std::atomic<bool> b_stop_;
    std::thread _check_thread;
};

struct UserInfo {
    std::string name;
    std::string pwd;
    int uid;
    std::string email;
};

class MysqlDao {
public:
    MysqlDao();

    ~MysqlDao();


    int RegUser(const std::string &name, const std::string &email, const std::string &pwd);

    bool CheckEmail(const std::string &name, const std::string &email);

    bool UpdatePwd(const std::string &name, const std::string &newpwd);
     bool CheckPwd(const std::string& email, const std::string& pwd, UserInfo& userInfo);

private:
    std::unique_ptr<MysqlConnPool> pool_;
};

#endif //GATESERVER_MYSQLDAO_H
