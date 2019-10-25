#ifndef PAGE_H
#define PAGE_H

#include <QStringList>
#include <string>
#include <cstdlib>
#include <QString>
#include <QFile>
#include <QDir>
#include <QMessageBox>


#include "trait_requete.h"
class Page
{
public:
    enum type{file,text,directory};
    Page();
    Page(Requete* req );
    Page(QString chem);
    ~Page();
    QString get_name();
    QString get_URL();
    time_t get_page_time();
    enum type get_page_type();
    int get_page_ID();
    int get_page_taille();
    QByteArray get_page_data();


private:
    QString name;
    QString URL;
    time_t pTime;
    int pageID;
    static int IDcount;


protected :
    enum type pageType;
    QByteArray data;
    int taille;


};

class File : public Page
{
public :
    File();
    File(Requete *rq, QFile* fichier);
};

class Directory : public Page
{
public :
    Directory();
    Directory(Requete *req, QDir* rep);
};

class Text : public Page
{
public :
    Text(QString chem);
    void tpage_title(QString title);
    void tpage_end();
    void tpage_text_br(QString txt);
    void tpage_text_br(QString txt, int val);
    void tpage_text_nobr(QString txt);
    void tpage_text_nobr(QString txt, int val);
    void break_line();


};

#endif // PAGE_H
