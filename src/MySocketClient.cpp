/****************************************************************************
**
** Copyright (C) 2008 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Qt Software Information (qt-info@nokia.com)
**
** This file is part of the example classes of the Qt Toolkit.
**
** Commercial Usage
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
**
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License versions 2.0 or 3.0 as published by the Free
** Software Foundation and appearing in the file LICENSE.GPL included in
** the packaging of this file.  Please review the following information
** to ensure GNU General Public Licensing requirements will be met:
** http://www.fsf.org/licensing/licenses/info/GPLv2.html and
** http://www.gnu.org/copyleft/gpl.html.  In addition, as a special
** exception, Nokia gives you certain additional rights. These rights
** are described in the Nokia Qt GPL Exception version 1.3, included in
** the file GPL_EXCEPTION.txt in this package.
**
** Qt for Windows(R) Licensees
** As a special exception, Nokia, as the sole copyright holder for Qt
** Designer, grants users of the Qt/Eclipse Integration plug-in the
** right for the Qt/Eclipse Integration to link to functionality
** provided by Qt Designer and its related libraries.
**
** If you are unsure which license is appropriate for your use, please
** contact the sales department at qt-sales@nokia.com.
**
****************************************************************************/

#include "MySocketClient.h"

#include <QtNetwork>
#include "Gest_cache.h"
#include "trait_requete.h"
#include "page.h"

MySocketClient::MySocketClient(int socketDescriptor, QObject *parent, Gest_cache* _cache_t, stats* _stats_t )
    : QThread(parent), socketDescriptor(socketDescriptor)
{
    cache_t = _cache_t;
    stats_t = _stats_t;
}

inline string removeEndLine(string s){
        while( s.at(s.length()-1) == '\r' ||
                        s.at(s.length()-1) == '\n' ){
                s = s.substr(0, s.length()-1);
        }
        return s;
}

void MySocketClient::run()
{
    cout << "Starting MySocketClient::run()" << endl;
    QTcpSocket tcpSocket;

    // ON RECUPERE LE LIEN DE COMMUNICATION AVEC LE CLIENT ET ON QUITTE EN CAS
    // DE PROBLEME...
    if (!tcpSocket.setSocketDescriptor(socketDescriptor)) {
        emit error(tcpSocket.error());
        return;
    }

    // SI LE CLIENT N'A PAS EU LE TEMPS DE NOUS TRANSMETTRE SA REQUETE,
    // ON SE MET EN ATTENTE PENDANT 1s
    while (tcpSocket.bytesAvailable() < (int)sizeof(quint16)) {
        if (!tcpSocket.waitForReadyRead( 1000 )) {
                cout << "(EE) Erreur de time out !" << endl;
                return;
        }
    }

    // LA PREMIERE REQUETE CORRESPOND AU GET NORMALEMENT
    char tampon[65536];

    // ON RECUPERE LA REQUETE ET SA TAILLE
    int lineLength = tcpSocket.readLine(tampon, 65536);

    // ON TRANSFORME LA REQUETE SOUS FORME DE STRING
    string ligne( tampon );
    ligne = removeEndLine( ligne );

    // ON AFFICHE LA COMMANDE A L'ECRAN...
    cout << "COMMANDE : =>" << ligne << "<=" << endl;

   int pos1 = ligne.find(" ");
   string cmde = ligne.substr(0, pos1);
   ligne = ligne.substr(pos1+1, ligne.length()-pos1);

   cout << "1. : " << cmde  << endl;
   cout << "2. : " << ligne << endl;

   int pos2 = ligne.find(" ");
   string file = ligne.substr(0, pos2);
   ligne = ligne.substr(pos2+1, ligne.length()-pos2);

   cout << "3. : " << file  << endl;
   cout << "4. : '" << ligne << "'" << endl;

        while( tcpSocket.bytesAvailable() > 0 ){
        int lineLength = tcpSocket.readLine(tampon, 65536);
        if (lineLength != -1 &&  lineLength != 0) {
                //cout << "C" << lineLength << " :: " << tampon;
        }else if(lineLength != -1 ){
                cout << "Nothing for the moment !" << endl;
        }
    }



   QString str = tr(file.c_str());
   QFile f( str );
   QDir  d( str );

   cout << " - Chemin du fichier : " << str.toStdString() << endl;
   cout << " - isFile :          : " << f.exists() << endl;
   cout << " - isDirectory       : " << d.exists() << endl;




    Requete req(cmde,str);

    cout << "Reponse HTTP: "<< req.http_code() << endl;
    cout << "Commande: "<< req.get_commande() << endl;

    if (!stats_t->connected(tcpSocket.peerAddress().toString())){
       stats_t->new_client(tcpSocket.peerAddress().toString());
    }
    stats_t->new_reqRecu();
    stats_t->new_octetsRecu(lineLength);

    QByteArray dt = cache_t->affiche_page(&req).get_page_data();

    if (!stats_t->etat_serveur()){
        Requete req1(cmde,"/private/deactivate.html");
        tcpSocket.write(req1.http_code());
        tcpSocket.write(cache_t->affiche_page(&req1).get_page_data());

    }else{


    tcpSocket.write(req.http_code());
    tcpSocket.write(dt);

    stats_t->new_octetsEnv(dt.size());

    }

    cout << "Reponse HTTP: "<< req.http_code() << endl;
    cout << "ERROR Type: "<< req.get_error() << endl;

    stats_t->new_req(req);


   /* if( f.exists() == false &&  d.exists() == false ){
       // ERREUR 404 LE FICHIER N'EXISTE PAS...

   }else if( d.exists() == true ){
       // C'EST UN REPERTOIRE !

   }else if( f.exists() == true ){
       QFile* file = new QFile( str );
        if (!file->open(QIODevice::ReadOnly))
        {
                delete file;
                return;
        }
        tcpSocket.write("HTTP/1.1 200 OK\r\nContent-Type: image/jpeg;\r\n\r\n");
        tcpSocket.write( file->readAll() );
        file->close();

   }else{

   }

//! [2] //! [3]

    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);
    out << (quint16)0;
    out << text;
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
//! [3] //! [4]

    tcpSocket.write(block);*/

    tcpSocket.disconnectFromHost();
    tcpSocket.waitForDisconnected();
    cout << "Finishing MySocketClient::run()" << endl;
}
//! [4]
