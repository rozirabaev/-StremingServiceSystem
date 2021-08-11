#include <string>
#include <vector>
#include <iterator>
#include <math.h>
#include <algorithm>
#include "../include/Watchable.h"
#include "../include/Session.h"

using namespace std;

#include "../include/User.h"
//constr1
User::User(const string& name):history(vector<Watchable*>()),name(name){

}
//constr2
User::User(const string& name,vector<Watchable*> his):history(his),name(name){
}
//------------------------------------------------------------------
//rule of 5
//--------------------------------------------------------------
//destructor
User::~User(){
    int size = history.size();
    for ( int i=0; i<size; i++)
        history[i]= nullptr;
    history.clear();
}
//copy constructor
User::User(const User &other):history(vector<Watchable*>()),name(other.getName()) {
    int hs = other.history.size();
    for(int i=0;i<hs;i++){
        Watchable* point = other.history[i]->clone();
        this->history.push_back(point);
    }
}
User::User(const User &other,vector<Watchable*> cont):history(vector<Watchable*>()),name(other.getName()){
    int histSize=other.get_history().size();
    for(int i=0;i<histSize;i++) {
        string name = other.get_history()[i]->toString();
        for (Watchable *f:cont) {
            if (name == f->toString())
                addToHistory(f);
        }
    }
}
//move constructor
User::User(User &&other) : history(other.history), name(other.getName()) {
    int hissize = other.history.size();
    for ( int i=0; i< hissize; i++)
        other.history[i]= nullptr;
    other.setName("");
}

//copy assignment
User& User::operator=(const User &other) {
    this->setName(other.getName());
    int hSize = history.size();
    for ( int i=0; i<hSize; i++)
        history[i]= nullptr;
    history.clear(); // clears the vector
    int otherSize = other.history.size();
    for (int i=0; i<otherSize; i++)
        history.push_back(other.history[i]); // set this history with others history
    return *this;
}

//move assignment
User& User::operator=(User &&other) {
    this->setName(other.getName());
    int hsize = history.size();
    for ( int i=0; i<hsize; i++)
        history[i]= nullptr;
    history.clear(); // clears the vector
    int otheSize = other.history.size();
    for (int i=0; i<otheSize; i++)
        history.push_back(other.history[i]); // set this history with others history
    for ( int i=0; i<otheSize; i++)
        other.history[i]= nullptr;
    other.setName("");
    return *this;
}
//-----------------------------------------------
//getters,setters
//----------------------------------------------
string User::getName() const{
    return name;
}
void User::setName(std::string name) {
    this->name=name;
}

vector<Watchable*> User::get_history() const {
    return history;
}
void User::addToHistory(Watchable* f) {
    history.push_back(f);
}

//---------------------------------------------
//LengthRecommenderUser
//---------------------------------------------
LengthRecommenderUser::LengthRecommenderUser(const LengthRecommenderUser &other, std::vector<Watchable *> cont):User(other,cont) {}


//constructor
LengthRecommenderUser::LengthRecommenderUser(const string& name):User(name){}

//clone
User* LengthRecommenderUser::clone(vector<Watchable*> cont) {
    return new LengthRecommenderUser(*this,cont);
}
LengthRecommenderUser::LengthRecommenderUser(const string& name,vector<Watchable*> his):User(name,his){
}
string LengthRecommenderUser::getType() const{
    return "len";
}
Watchable* LengthRecommenderUser::getRecommendation(Session& s){
    if(history.size()==s.getContent().size())
        return nullptr;
    int sizeHistory=s.getActiveUser()->get_history().size();
    Watchable* justWatch = s.getActiveUser()->get_history()[sizeHistory-1];
    int id=justWatch->getNext();
    if(id!=0) {
        return s.getContent()[id-1]; // checks if there is another episode
    }
    int averageLength=getAverageLength();
    Watchable* fLen =nullptr;
    int min= 10000;
    int sizeContent=s.getContent().size();
    for(int i=0;i<sizeContent;i++){
        Watchable* f =s.getContent()[i];
        int m = abs(f->getLength() - averageLength);
        bool found= false;
        int sizeHistory=history.size();
        for(int j=0; j<sizeHistory;j++) {
            if (history[j]->getName() == f->getName()) {
                found = true;
                break;
            }
        }
        if((m<min)&(!found)){
            min=m;
            fLen=f;
        }
    }
    return fLen;
}

int LengthRecommenderUser:: getAverageLength(){
    int averageLength=0;
    int sum=0; int count =0;
    vector <Watchable*> ::iterator it;
    for(it=history.begin();it!=history.end();++it){
        count++;
        Watchable* f=*(it);
        sum=sum+f->getLength();
        f=nullptr;
    }
    averageLength=sum/count;


    return averageLength;
}

LengthRecommenderUser::~LengthRecommenderUser() {

}
//----------------------------------------
//RerunRecommenderUser
//----------------------------------------
//constructor
RerunRecommenderUser::RerunRecommenderUser(const string& name): User(name),numOfRer(0){
}

RerunRecommenderUser::RerunRecommenderUser(const string& name,vector<Watchable*> his):User(name,his),numOfRer(0){
}
RerunRecommenderUser::RerunRecommenderUser(const RerunRecommenderUser &other, std::vector<Watchable *> cont):User(other,cont), numOfRer(other.numOfRer) {}
//clone
User* RerunRecommenderUser::clone(vector<Watchable*> cont) {
    User* u=new RerunRecommenderUser(*this,cont);
    return u;
}

string RerunRecommenderUser::getType() const {
    return "rer";
}
Watchable* RerunRecommenderUser::getRecommendation(Session& s){
    if(history.size()==s.getContent().size())
        return nullptr;
    int sizeHistory=s.getActiveUser()->get_history().size();
    Watchable* justWatch = s.getActiveUser()->get_history()[sizeHistory-1];
    int id=justWatch->getNext();
    if(id!=0) {
        return s.getContent()[id-1]; // checks if there is another episode
    }
    int ind;
    ind=numOfRer;
    numOfRer++;
    return history[ind];
}
RerunRecommenderUser::~RerunRecommenderUser() {

}
//----------------------------------------
//GenreRecommenderUser
//---------------------------------------
//constructor
GenreRecommenderUser:: GenreRecommenderUser(const string& name): User(name){
}
GenreRecommenderUser::GenreRecommenderUser(const GenreRecommenderUser &other, std::vector<Watchable *> cont):User(other,cont) {}

//clone

User* GenreRecommenderUser::clone(vector<Watchable*> cont) {
    User* u=new GenreRecommenderUser(*this,cont);
    return u;
}

GenreRecommenderUser::GenreRecommenderUser(const string& name,vector<Watchable*> his):User(name,his){
}
string GenreRecommenderUser::getType() const {
    return "gen";
}
Watchable* GenreRecommenderUser::getRecommendation(Session &s) {
    if(history.size()==s.getContent().size())
        return nullptr;
    int sizeHistory=s.getActiveUser()->get_history().size();
    Watchable* justWatch = s.getActiveUser()->get_history()[sizeHistory-1];
    int id=justWatch->getNext();
    if(id!=0)
        return s.getContent()[id-1]; // checks if there is another episode
    vector< pair <int, string>> popularTags = getPopularTags(s);
    Watchable* rec=nullptr;
    int sizeContent=s.getContent().size();// the content size
    int size =popularTags.size();
    bool found = false;
    for (int k=0; k<size && !found; k++){// going on the tags from the end
    //while(!f){ // not found yet and the tags exist
        for(int i=0;i<sizeContent&&!found;++i){ // loop for the content from the begining
            Watchable *tmp = s.getContent()[i];
            //cout<<tmp->getName()<<endl;
            int sizeTags = s.getContent()[i]->getTags().size(); // tags of the current content
            for (int j=0;j<sizeTags; ++j) {
                if ((tmp->getTags()[j] == popularTags[k].second) &&!(this->searchContent(get_history(),s.getContent()[i]))){
                    rec = s.getContent()[i];
                    found = true;
                }
                //(count(history.begin(), history.end(), s.getContent()[i]) == 0)
            }
    }
    }
    return rec;
}

bool GenreRecommenderUser::searchContent(vector<Watchable *> v, Watchable *w){
    int size = v.size();
    for (int i=0; i<size; i++) {
        if (v.at(i) == w)
            return true;
    }
    return false;
}

vector< pair <int, string>> GenreRecommenderUser::getPopularTags(Session& s) {
    vector< pair <int, string> > popularTags;
    int sizeHistory=s.getActiveUser()->get_history().size();
    for(int i=0;i<sizeHistory;i++) { //we are going over Watchables*
        int sizeTags=s.getActiveUser()->get_history()[i]->getTags().size();
        for (int j = 0; j < sizeTags; j++) { //into tags of Watchable*
            string currentTag = s.getActiveUser()->get_history()[i]->getTags()[j];
            bool exist=false;
            int popTagsSize=popularTags.size();
            for (int k = 0; k < popTagsSize; k++)
                if (popularTags[k].second == currentTag) {
                    (popularTags[k].first)++;
                    exist = true;
                }
            if (!exist)
                popularTags.push_back(make_pair(1,currentTag)); // pushes the new tag
        }
    }
    sort(popularTags.begin(),popularTags.end()); // all the content that hasnt seen is sorted
    return popularTags;
}
GenreRecommenderUser::~GenreRecommenderUser() {

}