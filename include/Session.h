#ifndef SESSION_H_
#define SESSION_H_

#include <vector>
#include <unordered_map>
#include <string>
#include "Action.h"

class User;
class Watchable;

class Session{
public:
    Session(const std::string &configFilePath);
    ~Session();
    std::vector<Watchable*> getContent() const;
    void start();

//rule of 5
    Session(const Session &other);
    Session(Session &&other);
    Session& operator=(const Session &other);
    Session& operator=(Session &&other);
    //getters
    User* getActiveUser() const;
    void setActionsLog(BaseAction*);
    void addToUserMap(std::pair<std::string,User*>);
    void setActiveUser(User*);
    void setInput(std::string s);
    void deleteUser(std::string name);
    const std::vector<BaseAction*>& getActionsLog() const;
    const std::unordered_map<std::string, User*>& getUserMap() const;
    std::string getInput();
    void clear();
private:
    std::vector<Watchable*> content;
    std::vector<BaseAction*> actionsLog;
    std::unordered_map<std::string,User*> userMap;
    User* activeUser;
    std::string input;

};
#endif
