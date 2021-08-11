
#include "../include/Session.h"
#include <vector>
#include <unordered_map>
#include <string>
#include "../include/Action.h"
#include "../include/User.h"
#include "../include/Watchable.h"
#include "../include/json.hpp"
#include <iostream>
#include <fstream>
using namespace std;
using json = nlohmann::json;

//constructor
Session::Session(const string &confifFilePath): content(vector<Watchable*>()),actionsLog(vector<BaseAction*>()),userMap(unordered_map<string,User*>()),activeUser(new LengthRecommenderUser("default")),input(""){
    userMap.insert(make_pair(activeUser->getName(),activeUser));
    ifstream js(confifFilePath);
    json myContent;
    js >> myContent;
    long id=1;
    int sizeMovies=myContent["movies"].size();
    for (int i = 0; i <sizeMovies; i++) {
        string name = myContent["movies"][i]["name"];
        vector <string> tags;
        int sizeTags= myContent["movies"][i]["tags"].size();
        for (int j = 0; j <sizeTags; j++)
            tags.push_back((myContent["movies"][i]["tags"][j]));

        int length = myContent["movies"][i]["length"];
        Watchable *newMovie = new Movie(id, name, length, tags);
        content.push_back(newMovie);
        id++;
    }
    int esize =  myContent["tv_series"].size();
    for (int i = 0; i <esize; i++) {
        string name = myContent["tv_series"][i]["name"];
        vector <string> tags;
        int etagsSize = myContent["tv_series"][i]["tags"].size();
        for (int j = 0; j <etagsSize; j++)
            tags.push_back((myContent["tv_series"][i]["tags"][j]));
        int length = myContent["tv_series"][i]["episode_length"];
        int seasenSize = myContent["tv_series"][i]["seasons"].size();
        for(int j=0;j<seasenSize;j++)
            for(int k=0;k<myContent["tv_series"][i]["seasons"][j];k++){
                Watchable *newEpisode = new Episode(id,name,length,j+1,k+1,tags);
                long next=id+1;
                if((j==(seasenSize-1))&(k==myContent["tv_series"][i]["seasons"][seasenSize-1]))
                    next=0;
                newEpisode->next(next);
                content.push_back(newEpisode);
                id++;
            }
    }
}

//getters
//---------------------------------------------------
vector<Watchable*> Session::getContent() const{
    return content;
}

const vector<BaseAction*>& Session::getActionsLog()const {
    return actionsLog;
}
User* Session::getActiveUser() const {
    return activeUser;
}

const unordered_map<string,User*>&  Session::getUserMap() const{
    return userMap;
}
string Session::getInput() {
    return input;
}
//setters
void Session::setActionsLog(BaseAction* action) {
    actionsLog.push_back(action);
}
void Session::setActiveUser(User *u) {
    activeUser=u;
}
void Session::setInput(string s) {
    this->input=s;
}
void Session::addToUserMap(pair <std::string,User*> u){
    userMap.insert(u);
}

void Session::deleteUser(std::string name) {
    for(pair<string,User*> u:userMap){
        if(u.first==name) {
            delete (u.second);
            userMap.erase(u.first);
            break;
        }
    }
}

//---------------------------------------------------------
//rule of 5
//---------------------------------------------------------
Session::~Session(){
    clear();
}
void Session:: clear(){
    /* delete activeUser;
    */
    int actSize=actionsLog.size();
    for(int i=0;i<actSize;i++) {
        delete (actionsLog[i]);
        actionsLog[i] = nullptr;
    }
    actionsLog.clear();
    for(Watchable* c: content) {
        delete (c);
        c = nullptr;
    }
    content.clear();
    for(unordered_map<string,User*>::iterator it=userMap.begin();it!=userMap.end();++it) {
        delete (it->second);
        it->second = nullptr;
    }
    activeUser=nullptr;
}
//CopyConstructor
Session::Session(const Session& s): content(vector<Watchable*>()),actionsLog(vector<BaseAction*>()),userMap(unordered_map<string,User*>()),activeUser(),input(""){
    unordered_map<string, User *>::iterator it;
    int sizeContent=s.getContent().size();
    for(int i=0;i<sizeContent;i++){
        Watchable* f = s.getContent()[i]->clone();
        content.push_back(f);
    }
    for(auto& it : s.userMap){
        pair<string,User*> p=(it);
        string newName=p.first;
        User* u=p.second->clone(content);
        userMap.insert(make_pair(newName,u));
    }
    string name=s.activeUser->getName();
    unordered_map<string, User *>::iterator iter;
    iter = userMap.find(name);
    User* us= (*iter).second;
    activeUser=us;
    int sizeLog=s.getActionsLog().size();
    for(int i=0;i<sizeLog;i++){
        BaseAction* a = s.getActionsLog()[i]->clone();
        actionsLog.push_back(a);
    }
    input = s.input;
}


//Move constructor
Session::Session(Session&& other ):content(vector<Watchable*>()),actionsLog(vector<BaseAction*>()),userMap(unordered_map<string,User*>()),activeUser(),input(""){
    userMap.swap(other.userMap);
    content.swap(other.content);
    actionsLog.swap(other.actionsLog);
    activeUser=other.activeUser;
    other.setActiveUser(nullptr);
}

//Copy assignment operator
Session& Session:: operator=(const Session &other){
    if(this != &other){
        clear();
        int size=other.getContent().size();
        for(int i=0;i<size;i++){
            Watchable* f = other.getContent()[i]->clone();
            content.push_back(f);
        }
        for(auto it=other.getUserMap().begin();it!=other.getUserMap().end();++it){
            string newName=it->first;
            User* u=it->second->clone(content);
            userMap.insert(make_pair(newName,u));
        }
        string name=other.getActiveUser()->getName();
        auto iter = other.getUserMap().find(name);
        activeUser = (*iter).second;

        int sizeact=other.getActionsLog().size();
        for(int i=0;i<sizeact;i++){
            BaseAction* a = other.getActionsLog()[i]->clone();
            actionsLog.push_back(a);
        }
        input = other.input;
    }
    return *this;
}
//Move assignment operator
Session& Session:: operator=(Session &&other){
    if(this != &other){
        this->clear();
        userMap.swap(other.userMap);
        content.swap(other.content);
        actionsLog.swap(other.actionsLog);
        activeUser=other.activeUser;
    }
    return *this;
}


//method start()
void Session::start(){
    cout<<"SPLFLIX is now on!"<<endl;
    bool f=true;
    string sInput;
    while(f){
        getline(cin,sInput);
        string sAction=sInput.substr(0, sInput.find(" "));
        int size=sInput.size();
        int pos=sInput.find(" ")+1;
        input=sInput.substr(pos,size);
        if(sAction=="dupuser"){
            BaseAction* action = new DuplicateUser();
            action->act(*this);
            actionsLog.push_back(action);
        }
        if(sAction=="createuser"){
            BaseAction* action = new CreateUser();
            action->act(*this);
            actionsLog.push_back(action);
        }
        if(sAction=="changeuser"){
            BaseAction* action = new ChangeActiveUser();
            action->act(*this);
            actionsLog.push_back(action);
        }
        if(sAction=="deleteuser"){
            BaseAction* action = new DeleteUser();
            action->act(*this);
            actionsLog.push_back(action);
        }
        if(sAction=="content"){

            BaseAction* action = new PrintContentList();
            action->act(*this);
            actionsLog.push_back(action);
        }

        if(sAction=="watchhist"){
            BaseAction* action = new PrintWatchHistory();
            action->act(*this);
            actionsLog.push_back(action);
        }
        if(sAction=="watch"){
            BaseAction* action = new Watch();
            action->act(*this);
            actionsLog.push_back(action);
        }
        if(sAction=="log"){
            BaseAction* action = new PrintActionsLog();
            action->act(*this);
            actionsLog.push_back(action);
        }
        if(sAction=="exit"){
            BaseAction* action = new Exit();
            action->act(*this);
            actionsLog.push_back(action);
            f= false;
        }
    }

}

