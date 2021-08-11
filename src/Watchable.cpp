#include "../include/Watchable.h"
#include "../include/Session.h"
#include "../include/User.h"
#include <string>
#include <vector>


using namespace::std;
//Watchable

//constructor
Watchable::Watchable(long id, int length, const vector<string> &tags) : id(id),length(length),tags(tags){
}
//getters(tags,length,id)
vector<string> Watchable::getTags() const{
    return tags;
}

int Watchable::getLength() const {
    return length;
}
long Watchable:: getId() const{
    return id;
}
//string of tags
string Watchable::tagsToString() const {
    string sTags="[";
    int i;
    int sizeTags=getTags().size();
    for (i = 0; i < sizeTags - 1; i++)
        sTags = sTags+getTags()[i] + ", ";
    int id=i;
    sTags+=getTags()[id]+"]";
    return sTags;
}
Watchable::~Watchable() {

}

//--------------------------------------------------
//Movie
//--------------------------------------------------
//constructor
Movie::Movie(long id, const string &name, int length, const vector<string> &tags):Watchable(id,length,tags),name(name){
}
Movie::~Movie() {

}

//getters(length,name)
int Movie::getId() const{
    return  Watchable::getId();
}
int Movie::getLength() const {
    return Watchable::getLength();
}

string Movie::getName() const{
    return name;
}
void Movie::next(long i) {}
long Movie::getNext() {
    return 0;
}
//to string
string Movie::toString() const {

    string sMovie = to_string(getId())+". "+getName()+" "+to_string(getLength())+" "+"miniuts"+Watchable::tagsToString();
    return sMovie;
}
string Movie::getRecommendation(Session& sess) {
    return ". We recommend to watching " + this->getName() + ", continue watch?[y/n]";
}

string Movie::watching(Session &sess) {
    return "Watching " + this->getName();
}

Watchable* Movie::getNextWatchable(Session& s) const {

    Watchable* f = s.getActiveUser()->getRecommendation(s);
    return f;
}

Watchable* Movie::clone() {
    Watchable* f= new Movie(*this);
    return f;
}
//----------------------------------------------------
//Episode
//----------------------------------------------------
//constructor
Episode::Episode(long id, const std::string& seriesName,int length, int season, int episode ,const std::vector<std::string>& tag):Watchable(id,length,tag),seriesName(seriesName),season(season),episode(episode),nextEpisodeId(0){

}
Episode::~Episode() {

}
//getters(length,name)
int Episode::getLength() const {
    return Watchable::getLength();
}

string Episode::getName() const{
    return seriesName;
}
int Episode::getSeason() const{
    return season;
}
int Episode::getEpisode()  const{
    return episode;
}
void Episode::next(long next) {
    this->nextEpisodeId=next;
}


Watchable* Episode::getNextWatchable(Session& s) const {
    if(s.getContent()[getId()+1]->getName()==getName())
        return s.getContent()[getId()+1];
    return s.getActiveUser()->getRecommendation(s);

}


//to string
string Episode::toString() const {
    string sEpisode = to_string(getId())+". "+getName()+" S"+to_string(getSeason())+"E"+to_string(getEpisode())+" "+to_string(getLength())+" "+"minutes "+Watchable::tagsToString();
    return sEpisode;
}

string Episode::getRecommendation(Session &sess) {
    return ". We recommend to watching " + this->getName() +" S"+to_string(getSeason())+"E"+to_string(this->getEpisode())+ ", continue watch?[y/n]";
}
string Episode::watching(Session &sess)  {
    return "Watching " + this->getName() +" S" +to_string(getSeason())+"E"+to_string(this->getEpisode());
}

Watchable* Episode::clone() {
    Watchable* f= new Episode(*this);
    return f;
}
long Episode::getNext() {
    return nextEpisodeId;
}