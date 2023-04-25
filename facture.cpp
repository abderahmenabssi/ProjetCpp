#include "facture.h"

facture::facture()
{
    num=0;
    nomp="";
    datef="";
    prix_c=0;
    prix_m=0;
}


facture::facture(int num ,QString nomp ,QString datef,int prix_c,int prix_m)
{
this->num=num;
this->nomp=nomp;
this->datef=datef;
    this->prix_c=prix_c;
    this->prix_m=prix_m;
};
int facture::getnum() {return num;}
QString facture::getnomp(){return nomp;}
QString facture::getdatef(){return datef;}
int facture::getprix_c() {return prix_c;}
int facture::getprix_m() {return prix_m;}

void facture::setnum(int num){this->num=num;}
void facture::setnomp(QString nomp){this->nomp=nomp;}
void facture::setdatef(QString datef){this->datef=datef;}
void facture::setprix_c(int prix_c){this->prix_c=prix_c;}
void facture::setprix_m(int prix_m){this->prix_m=prix_m;}


bool facture::ajouter()
{//bool test=false;
    QSqlQuery query;


          query.prepare("INSERT INTO facture (numero,nomp, datef,prix_c,prix_m) "
                        "VALUES (:numero,:nomp,:datef, :prix_c, :prix_m)");
          query.bindValue(":numero",num);
          query.bindValue(":nomp",nomp);
          query.bindValue(":datef", datef);
          query.bindValue(":prix_c", prix_c);
          query.bindValue(":prix_m", prix_m);

          return query.exec();   //execution de requette
};
