#ifndef MANAGER_H
#define MANAGER_H

#include <iostream>
#include <vector>
#include <QObject>
#include <QDateTime>
#include <QDebug>
#include "time.h"
#include "evenement.h"

using namespace std;
template<class T> class Manager;
template<class T>class Handler{
    friend class Manager<T>;
    Manager<T>* instance;
public:
    Handler():instance(0){}
    virtual ~Handler(){Manager<T>::freeInstance();}
};
template<class T> class Manager
{
private:
    friend class Handler<T>;
    static Handler<T> handler;
    Manager(){}
    Manager& operator=(const Manager&);
    virtual ~Manager();
protected:
    vector<T*> tab;
public:
   // virtual void deleteItem(T* t);
    unsigned int getSize() const {return tab.size();}
    T* getItem(const QString&) const;
    static Manager& getInstance();
    static void freeInstance();
    void addItem(T* t){tab.push_back(t);}
    template<class T> class iterator{
        T* current;

     public:
        iterator(const iterator& it):current(it.current){}
        iterator(T* ptr):current(ptr){}
        iterator(unsigned int indice = 0):current(Manager::getInstance().tab[indice]){}

        iterator& operator++(){++current; return *this;}
        bool operator!=(const iterator<T>& it){return it.current != current;}
        bool operator==(const iterator<T>& it){return it.current == current;}
        T& operator*()const{return *current;}
    };
    const iterator<T> begin() const {return iterator<T>();}
    const iterator<T> end()const{return iterator<T>(getSize());}

};
template <class T>Handler<T> Manager<T>::handler = Handler<T>::Handler();
template <class T>Manager<T>& Manager<T>::getInstance(){
    if(handler.instance == 0)
        handler.instance = new Manager<T>();
    return *handler.instance;
}
template <class T>void Manager<T>::freeInstance(){

    delete handler.instance;
    handler.instance = 0;
}
template <class T> Manager<T>::~Manager(){
    if(!tab.empty()){
    for(Manager<T>::iterator<T> it = this->begin(); it != this->end(); ++it)
        delete &(*it);
    tab.clear();}
}
template <class T> T* Manager<T>::getItem(const QString& id)const{
    for(Manager<T>::iterator it = this->begin(); it != this->end(); ++it)
        if(*it.getId() == id)return &(*it);
    throw "Exception: pas d'Item correspondant";
    return 0;
}


class TacheManager: private Manager<Tache>{
   public:
   void  ajouterTacheUnitaire(QString id, QString titre, TIME::Duree duree,
                          bool preemptive, QString dispo, QString echeance){
       QString format = "dd:MM:yyyy:HH:mm";
      QDateTime date_dispo = QDateTime::fromString(dispo,format);
      QDateTime date_echeance = QDateTime::fromString(echeance,format);
        TacheUnitaire* t = new TacheUnitaire(id, titre, duree, preemptive, date_dispo, date_echeance);
        addItem(t);
    }
   void ajouterTacheComposite(const QString& id, const QString& titre, vector<Tache*> liste, QString dispo = "00:00:0000:00:00", QString echeance = "00:00:0000:00:00"){
       QString format = "dd:MM:yyyy:HH:mm";
       QDateTime date_dispo = QDateTime::fromString(dispo,format);
       QDateTime date_echeance = QDateTime::fromString(echeance,format);
       TacheComposite* t = new TacheComposite(id, titre, liste, date_dispo, date_echeance);
        addItem(t);
    }
    void supprimerTache(QString id);
};



class ProjectManager: private Manager<Projet>{
   public:
   void  ajouterProjet(QString id, QString titre,
                         QString dispo){
       QString format = "dd:MM:yyyy:HH:mm";
       QDateTime date_dispo = QDateTime::fromString(dispo,format);
    Projet* t = new Projet(id, titre, date_dispo);
    addItem(t);
    }
    void supprimerProjet(QString id);

};
class PrecedenceManager: private Manager<Precedence>{
   public:
   void  ajouterPrecedence(Tache* t1, Tache* t2){
    Precedence* t = new Precedence(t1,t2);
    addItem(t);
    }
    void supprimerPrecedence(QString id);

};

#endif // MANAGER_H
