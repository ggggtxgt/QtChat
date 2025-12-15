#ifndef GATESERVER_MYSQLMANAGER_H
#define GATESERVER_MYSQLMANAGER_H

#include "const.h"
#include "MysqlDao.h"

class MysqlManager : public Singleton<MysqlManager> {
    friend class Singleton<MysqlManager>;

public:
    ~MysqlManager();


    int RegUser(const std::string &name, const std::string &email, const std::string &pwd);
    /*
    bool CheckEmail(const std::string &name, const std::string &email);
    bool UpdatePwd(const std::string &name, const std::string &email);
    bool CheckPwd(const std::string &name, const std::string &pwd, UserInfo &userInfo);
    */

private:
    MysqlManager();

    MysqlDao _dao;
};


#endif //GATESERVER_MYSQLMANAGER_H