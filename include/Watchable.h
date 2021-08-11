#ifndef WATCHABLE_H_
#define WATCHABLE_H_

#include <string>
#include <vector>


class Session;

class Watchable{
public:
    Watchable(long id, int length, const std::vector<std::string>& tags);
    virtual ~Watchable();
//getters
    int getLength() const;
    long getId() const;
    std::vector<std::string> getTags() const;

    std::string tagsToString() const;
//virtual
    virtual std::string toString() const = 0;
    virtual Watchable* getNextWatchable(Session&) const = 0;
    virtual std::string getName() const =0;
    virtual std::string getRecommendation(Session& sess)=0;
    virtual std::string watching(Session& sess)=0;
    virtual  Watchable* clone() =0;
    virtual void next(long)=0;
    virtual long getNext()=0;
private:
    const long id;
    int length;
    std::vector<std::string> tags;
};

class Movie : public Watchable{
public:
    Movie(long id, const std::string& name, int length, const std::vector<std::string>& tags);
    virtual ~Movie();

    int getId() const;
    int getLength()const;
    //virtual
    virtual std::string toString() const;
    virtual Watchable* getNextWatchable(Session&) const;
    virtual  Watchable* clone();
    virtual std::string getName() const;
    virtual std::string getRecommendation(Session& sess);
    virtual std::string watching(Session& sess);
    virtual void next(long);
    virtual long getNext();
private:
    std::string name;
};


class Episode: public Watchable{
public:
    Episode(long id, const std::string& seriesName,int length, int season, int episode ,const std::vector<std::string>& tags);
    virtual ~Episode();

    int getSeason() const;
    int getEpisode() const;
    long getNextEpisode();

    //virtual

    virtual std::string toString() const;
    virtual Watchable* getNextWatchable(Session&) const;
    virtual std::string getName() const;
    virtual  Watchable* clone();
    virtual int getLength() const;
    virtual std::string getRecommendation(Session& sess);
    virtual std::string watching(Session& sess);
    virtual void next(long);
    virtual long getNext();
private:
    std::string seriesName;
    int season;
    int episode;
    long nextEpisodeId;
};

#endif
