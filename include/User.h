#ifndef USER_H_
#define USER_H_

#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>
class Watchable;
class Session;

class User{
public:
    User(const std::string& name);
    User(const std::string& name,std::vector<Watchable*> his);
    User(const User &other,std::vector<Watchable*> cont);
    //rule of 5
    virtual ~User();
    User(const User &other); // copy const
    User(User &&other); //move constr
    User& operator=(const User &other);
    User& operator=(User &&other);
    //
    virtual Watchable* getRecommendation(Session& s) = 0;
    std::string getName() const;
    void setName(std::string name);
    std::vector<Watchable*> get_history() const;
    void addToHistory(Watchable*);
    virtual std::string getType() const=0;
    virtual User* clone(std::vector<Watchable*> cont) = 0;
protected:
    std::vector<Watchable* > history;
private:
    std::string name;

};


class LengthRecommenderUser : public User {
public:
    LengthRecommenderUser(const std::string& name);
    LengthRecommenderUser(const std::string& name, const std::vector<Watchable*> his);
    LengthRecommenderUser(const LengthRecommenderUser &other,std::vector<Watchable*> cont);
    virtual ~LengthRecommenderUser();
    virtual Watchable* getRecommendation(Session& s);
    virtual std::string getType() const;
    int getAverageLength();
    virtual User* clone(std::vector<Watchable*> cont);
private:
};

class RerunRecommenderUser : public User {
public:
    RerunRecommenderUser(const std::string& name);
    RerunRecommenderUser(const std::string& name, const std::vector<Watchable*> his);
    RerunRecommenderUser(const RerunRecommenderUser &other,std::vector<Watchable*> cont);
    virtual ~RerunRecommenderUser();

    virtual Watchable* getRecommendation(Session& s);
    virtual std::string getType() const;
    virtual User* clone(std::vector<Watchable*> cont);
private:
    int numOfRer;
};

class GenreRecommenderUser : public User {
public:
    GenreRecommenderUser(const std::string& name);
    GenreRecommenderUser(const std::string& name, const std::vector<Watchable*> his);
    GenreRecommenderUser(const GenreRecommenderUser &other,std::vector<Watchable*> cont);
    virtual ~GenreRecommenderUser();
    bool searchContent (std::vector <Watchable*> v, Watchable* w);

    virtual Watchable* getRecommendation(Session& s);
    virtual std::string getType() const;
    std::vector< std::pair <int, std::string> > getPopularTags(Session& s);
    virtual User* clone(std::vector<Watchable*> cont);
private:

};

#endif
