#include "patients.h"
#include <QSqlQuery>
#include <QDebug>
#include <QObject>
#include <QVariant>
#include <QtGui>
Patients::Patients()
{
    CIN=0;
    nom=" ";
    prenom=" ";
    date=" ";
    sexe=" ";
    Ntelephone=0;
}
Patients::Patients( int CIN,QString nom ,QString prenom,QString date,QString sexe, int Ntelephone)
{
    this->CIN=CIN;
    this->nom=nom;
    this->prenom=prenom;
    this->date=date;
    this->sexe=sexe;
    this->Ntelephone=Ntelephone;
}
 int Patients::getCIN(){return CIN;}
QString Patients::getnom(){return nom;}
QString Patients::getprenom(){return prenom;}
QString Patients::getdate(){return date;}
QString Patients::getsexe(){return sexe;}
 int Patients::getNtelephone(){return Ntelephone;}
void Patients::setCIN( int CIN){this->CIN=CIN;}
void Patients::setnom(QString nom){this->nom=nom;}
void Patients::setprenom(QString prenom){this->prenom=prenom;}
void Patients::setdate(QString date){this->date=date;}
void Patients::setsexe(QString sexe){this->sexe=sexe;}
void Patients::setNtelephone( int Ntelephone){this->Ntelephone=Ntelephone;}
bool Patients::ajouter()
{
    if (CIN == 0 || nom.isEmpty() || prenom.isEmpty() || date.isEmpty() || sexe.isEmpty() || Ntelephone == 0) {
            return false;
        }


    QSqlQuery query;
         query.prepare("INSERT INTO PATIENTT (CIN, NOM, PRENOM, DATE_N, SEXE, NTELEPHONE) "
                       "VALUES (:CIN, :nom, :prenom, :date, :sexe, :Ntelephone)");

         query.bindValue(":CIN", CIN);
         query.bindValue(":nom",nom);
         query.bindValue(":prenom", prenom);
         query.bindValue(":date", date);
         query.bindValue(":sexe", sexe);
         query.bindValue(":Ntelephone", Ntelephone);
         if(query.exec())
            {
                return true;
            }
            else
            {
                return false;
            }


}
bool Patients::rechercher( int CIN)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM PATIENTT WHERE CIN=:CIN");
    query.bindValue(":CIN", CIN);
    if (query.exec() && query.next()) {
        this->setCIN(query.value(0).toInt());
        this->setnom(query.value(1).toString());
        this->setprenom(query.value(2).toString());
        this->setdate(query.value(3).toString());
        this->setsexe(query.value(4).toString());
        this->setNtelephone(query.value(5).toInt());
        return true;
    }
    return false;
}

bool Patients::supprimer( int CIN)
{
    QSqlQuery query;
         query.prepare("DELETE FROM PATIENTT WHERE CIN=:CIN");
         query.bindValue(0, CIN);
         return query.exec();
}

Patients Patients::obtenirPatient( int CIN)
{
    Patients patient;
    QSqlQuery query;
    query.prepare("SELECT * FROM PATIENTT WHERE CIN=:CIN");
   query.bindValue(":CIN", CIN);
    if (query.exec() && query.next()) {
        patient.setCIN(query.value(0).toInt());
        patient.setnom(query.value(1).toString());
        patient.setprenom(query.value(2).toString());
        patient.setdate(query.value(3).toString());
        patient.setsexe(query.value(4).toString());
        patient.setNtelephone(query.value(5).toInt());
    }
    return patient;
}
bool Patients::modifier( int CIN, QString nom, QString prenom, QString date, QString sexe, int Ntelephone)
{
    QSqlQuery query;
    query.prepare("UPDATE PATIENTT SET NOM=:nom, PRENOM=:prenom, DATE_N=:date, SEXE=:sexe, NTELEPHONE=:Ntelephone WHERE CIN=:CIN");
    query.bindValue(":CIN", CIN);
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":date", date);
    query.bindValue(":sexe", sexe);
    query.bindValue(":Ntelephone",Ntelephone);
    return query.exec();
}

QSqlQueryModel* Patients::afficher()
{
    QSqlQueryModel* model=new QSqlQueryModel();

          model->setQuery("SELECT CIN, NOM, PRENOM, DATE_N, SEXE, NTELEPHONE FROM PATIENTT");
          model->setHeaderData(0, Qt::Horizontal,QObject::tr("CIN"));
          model->setHeaderData(1, Qt::Horizontal,QObject::tr("PRENOM"));
          model->setHeaderData(2, Qt::Horizontal,QObject::tr("NOM"));
          model->setHeaderData(3, Qt::Horizontal,QObject::tr("DATE DE NAISSANCE"));
          model->setHeaderData(4, Qt::Horizontal,QObject::tr("Sexe"));
          model->setHeaderData(5, Qt::Horizontal,QObject::tr("N°TELEPHONE"));
          return model;
}


