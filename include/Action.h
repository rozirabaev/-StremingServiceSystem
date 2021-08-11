#ifndef ACTION_H_
#define ACTION_H_

#include <string>
#include <iostream>

class Session;

enum ActionStatus{
	PENDING, COMPLETED, ERROR

};
std::string to_string(ActionStatus type);

class BaseAction{
public:
	BaseAction();
	virtual ~BaseAction();
	ActionStatus getStatus() const;
	BaseAction(const BaseAction&);
	virtual void act(Session& sess)=0;
	virtual  BaseAction* clone()=0;
	virtual std::string toString() const=0;
protected:
	void complete();
	void error(const std::string& errorMsg);
	std::string getErrorMsg() const;
private:
	std::string errorMsg;
	ActionStatus status;
};

class CreateUser  : public BaseAction {
public:
	virtual void act(Session& sess);
	virtual std::string toString() const;
    virtual  BaseAction* clone();
    virtual ~CreateUser();

};

class ChangeActiveUser : public BaseAction {
public:
	virtual void act(Session& sess);
	virtual std::string toString() const;
    virtual  BaseAction* clone();
    virtual ~ChangeActiveUser();

};

class DeleteUser : public BaseAction {
public:
	virtual void act(Session & sess);
	virtual std::string toString() const;
    virtual  BaseAction* clone();
    virtual ~DeleteUser();

};


class DuplicateUser : public BaseAction {
public:
	virtual void act(Session & sess);
	virtual std::string toString() const;
    virtual  BaseAction* clone();
    virtual ~DuplicateUser();

};

class PrintContentList : public BaseAction {
public:
	virtual void act (Session& sess);
	virtual std::string toString() const;
    virtual  BaseAction* clone();
    virtual ~PrintContentList();

};

class PrintWatchHistory : public BaseAction {
public:
	virtual void act (Session& sess);
	virtual std::string toString() const;
    virtual  BaseAction* clone();
    virtual ~PrintWatchHistory();

};


class Watch : public BaseAction {
public:
	virtual void act(Session& sess);
	virtual std::string toString() const;
    virtual  BaseAction* clone();
    virtual ~Watch();

};


class PrintActionsLog : public BaseAction {
public:
	virtual void act(Session& sess);
	virtual std::string toString() const;
    virtual  BaseAction* clone();
    virtual ~PrintActionsLog();

};

class Exit : public BaseAction {
public:
	virtual void act(Session& sess);
	virtual std::string toString() const;
    virtual  BaseAction* clone();
    virtual ~Exit();

};
#endif
