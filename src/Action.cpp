#include "../include/Action.h"
#include "../include/User.h"
#include "../include/Session.h"
#include "../include/Watchable.h"
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <algorithm>

using namespace:: std;

string to_String(ActionStatus type)
{
    if(type == PENDING)
        return "PENDING";
    if(type == COMPLETED)
        return "COMPLETED";
    else
        return "ERROR";

}
//BaseAction
//constructor
BaseAction::BaseAction():errorMsg(""), status(PENDING){}
BaseAction::~BaseAction() {
}

BaseAction::BaseAction(const BaseAction& other):errorMsg(other.errorMsg),status(other.status){
}
//getters
ActionStatus BaseAction::getStatus() const {
    return status;
}
string BaseAction::getErrorMsg() const {
    return errorMsg;
}
void BaseAction::complete() {
    status=COMPLETED;
}
void BaseAction::error(const std::string &errorMsg) {
    status=ERROR;
    this->errorMsg=errorMsg;
}



//------------------------------
//Actions
//------------------------------
void DeleteUser::act(Session &sess) {
    if((sess.getUserMap().find(sess.getInput()))!=sess.getUserMap().end()) {
        sess.deleteUser(sess.getInput());
        complete();
    }
    else {
        string err="-The user doesn't exist";
        error(err);
        cout<<"Error- The user does not exist"<<endl;
    }
}

DeleteUser::~DeleteUser() {}

string DeleteUser::toString() const {
    string str ="The action deleteUser: "+to_String(this->getStatus())+getErrorMsg();
    return str;
}

BaseAction* DeleteUser::clone() {
    return new DeleteUser(*this);
}

void PrintContentList::act(Session &sess) {
    int size= sess.getContent().size();
    for(int i=0;i<size;i++)
        cout<<sess.getContent()[i]->toString()<<endl;
    complete();
}

PrintContentList::~PrintContentList() {}
string PrintContentList::toString() const {
    return "The action content: "+to_String(this->getStatus());
}

BaseAction* PrintContentList::clone() {
    return new PrintContentList(*this);
}

void PrintWatchHistory::act(Session &sess) {
    cout<<sess.getActiveUser()->get_history().size();
    int size = sess.getActiveUser()->get_history().size();
    for(int i=0;i<size;i++){
        string s = sess.getActiveUser()->get_history()[i]->toString();
        int space = s.find(" ");
        string print = s.substr(space, s.length());
        cout << to_string(i+1) + ". " + print << endl;
    }
    complete();
}

PrintWatchHistory::~PrintWatchHistory() {}
string PrintWatchHistory::toString() const {
    return "The action watchlist: "+to_String(this->getStatus());
}

BaseAction* PrintWatchHistory::clone() {
    return new PrintWatchHistory(*this);
}
//Watch
void Watch::act(Session &sess) {
    string s=sess.getInput();
    int id;
    istringstream iss(s);
    iss>>id;
    string nowWatch = sess.getContent()[id-1]->watching(sess);
    cout<<nowWatch;
    sess.getActiveUser()->addToHistory(sess.getContent()[id-1]);
    Watchable* f= sess.getActiveUser()->getRecommendation(sess);
    if(f!= nullptr) {
        string c= f->getRecommendation(sess);
        cout << c;
        string sIn;
        cin >> sIn;
        id = f->getId();
        if (sIn == "y") {
            sess.setInput(to_string(id));
            BaseAction* recAction = new Watch();
            recAction->act(sess);
            sess.setActionsLog(recAction);
        }
    }
    f= nullptr;
    complete();

}

Watch::~Watch() {}
string Watch::toString() const {
    return "The action watch: "+to_String(this->getStatus());
}

BaseAction* Watch::clone() {
    return new Watch(*this);
}

//Exit
void Exit::act(Session &sess) {
    complete();
}

Exit::~Exit() {}

string Exit::toString() const {
    return "The action exit: "+to_String(this->getStatus());
}

BaseAction* Exit::clone() {
    return new Exit(*this);
}

//----

void CreateUser::act(Session &sess) {
    string input = sess.getInput();
    int namePosition = input.find(" ");
    string usersName = input.substr(0, namePosition);
    bool exist = false;
    if (sess.getUserMap().find(usersName) != sess.getUserMap().end()){
        exist = true;
        this->error("the user already exist");
        cout << " Error: " + this->getErrorMsg() << endl;
    }
    else if (exist==false) {
        string userType = input.substr(namePosition + 1, input.length());
        User *myUser;
        if (userType == "len") {
            myUser = new LengthRecommenderUser(usersName);
            sess.addToUserMap(make_pair(usersName, myUser)); // insert the new user
            this->complete();
        } else if (userType == "rer") {
            myUser = new RerunRecommenderUser(usersName);
            sess.addToUserMap(make_pair(usersName, myUser)); // insert the new user
            this->complete();
        } else if (userType == "gen") {
            myUser = new GenreRecommenderUser(usersName);
            sess.addToUserMap(make_pair(usersName, myUser)); // insert the new user
            this->complete();
        } else {
            this->error("- recommendation algoritm is invalid");
            cout << "Error - recommendation algoritm is invalid" << endl;
        }
    }
}
CreateUser::~CreateUser() {

}
std::string CreateUser::toString() const {
    return"The action CreateUser :" + to_String(this->getStatus());
}

BaseAction* CreateUser::clone() {
    return new CreateUser(*this);
}

void ChangeActiveUser::act(Session &sess) {
    string nameUser = sess.getInput();
    bool exist = false;
    if (sess.getUserMap().find(nameUser) !=sess.getUserMap().end())
        exist = true;
    if (!exist) {
        this->error("the user does not exist");
        cout << " Error: " + this->getErrorMsg() << endl;
    }
    else {
        //unordered_map<string, User *>::iterator it;
        auto it = sess.getUserMap().find(nameUser);
        sess.setActiveUser((*it).second);
        this->complete();
    }
}

std::string ChangeActiveUser::toString() const {
    return"The action ChangeActiveUser :" + to_String(this->getStatus());
}

BaseAction* ChangeActiveUser::clone() {
    return new ChangeActiveUser(*this);
}
ChangeActiveUser::~ChangeActiveUser() {

}
void DuplicateUser::act(Session &sess) {
    int position = sess.getInput().find(" ");
    string existUserName = sess.getInput().substr(0, position );
    string newName = sess.getInput().substr(position + 1,sess.getInput().length());
    //check for errors
    if (sess.getUserMap().find(existUserName) == sess.getUserMap().end()) {
        this->error("the user required does not exist");
        cout << " Error: " + this->getErrorMsg() << endl;
    } else if (sess.getUserMap().find(newName) != sess.getUserMap().end()) {
        this->error("the name has already taken");
        cout << " Error: " + this->getErrorMsg() << endl;
    } else {//if error not occured
        //unordered_map<string, User *>::iterator it;
        auto it = sess.getUserMap().find(existUserName);
        sess.setActiveUser((*it).second);
        User *existUser =sess.getActiveUser();
        string existType = existUser->getType();
        vector<Watchable *> newHistory = existUser->get_history();
        User *newUser;
        //check for type and create accordingly
        if (existType == "len")
            newUser = new LengthRecommenderUser(newName, newHistory);
        else if (existType == "gen")
            newUser = new GenreRecommenderUser(newName, newHistory);
        else if (existType == "rer")
            newUser = new RerunRecommenderUser(newName, newHistory);
        sess.addToUserMap(make_pair(newName, newUser));
        this->complete();
    }
}

std::string DuplicateUser::toString() const {
    return"The action DuplicateUser :" + to_String(this->getStatus());
}

BaseAction* DuplicateUser::clone() {
    return new DuplicateUser(*this);
}
DuplicateUser::~DuplicateUser() {

}
void PrintActionsLog::act(Session &sess) {
    int size = sess.getActionsLog().size();
    for(int i=0;i<size;i++) {
        BaseAction *a = sess.getActionsLog()[i];
        cout << a->toString() << endl;
    }
    // for (auto it = sess.getActionsLog().begin(); it != sess.getActionsLog().end(); ++it) {
    //   BaseAction *currentAct = *it;
    // cout << currentAct->toString() << endl; // toString prin

    this ->complete();
}

std::string PrintActionsLog::toString() const {
    return"The action PrintActionsLog :" + to_String(this->getStatus());
}

BaseAction* PrintActionsLog::clone() {
    return new PrintActionsLog(*this);
}

PrintActionsLog::~PrintActionsLog() {

}