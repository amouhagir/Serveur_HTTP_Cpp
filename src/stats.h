#ifndef STATISTIQUES_H
#define STATISTIQUES_H
#include <QVector>
#include <QString>
#include <QHash>

#include "trait_requete.h"
#include "page.h"

class stats
{
public:
    stats();
    ~stats();

    Page show_stats();
    void clear_stats();
    QString show_rq(int i);
    QString show_nbrFile(QString file);
    int get_nbrReqRecu();
    int get_nbrReqTraitee();
    int get_nbrClients();
    int get_OctetsRecu();
    int get_OctetsEnv();
    int get_Erreur(int erreur);
    int get_nbrErreur();
    bool get_etat();

    void new_req(Requete req);
    void new_reqRecu();
    void new_reqTraitee();
    void new_client(QString adress);
    void new_error(int erreur);
    void new_octetsRecu(int dt_size);
    void new_octetsEnv(int dt_size);
    void newFile(QString file);
    void enable();
    void disable();
    bool etat_serveur();
    bool connected(QString addr_client);

private:
    int m_nbrErreur;
    int m_nbrReqRecu;
    int m_nbrReqTraitee;
    int m_nbrOctetsEnv;
    int m_nbrOctetsRecu;
    bool actif;

    QVector<Requete> *list_req;
    QHash<int,int> list_errors;
    QHash<QString,int> list_files;
    QVector<QString> *list_clients;
};



#endif // STATISTIQUES_H
