#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "patients.h"
#include <QMessageBox>
#include <QString>
#include<QModelIndex>
#include<QObject>
#include<QPixmap>
#include<QSize>
#include<QIcon>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->le_CIN->setValidator(new QIntValidator(0,99999999, this));
    QRegExp regExp("[a-zA-Z]+");
     QValidator *validator = new QRegExpValidator(regExp, this);
     ui->le_nom->setValidator(validator);
     ui->le_prenom->setValidator(validator);
     QRegExp regExp1("^(0[1-9]|[1-2][0-9]|3[0-1])/(0[1-9]|1[0-2])/([0-9]{4})$");
     QValidator *validatord = new QRegExpValidator(regExp1, this);
     ui->le_date->setValidator(validatord);
     ui->le_num->setValidator(new QIntValidator(0,99999999, this));
     QRegExp regExp2("[HF]");
     QValidator* validators = new QRegExpValidator(regExp2, this);
     ui->le_sexe->setValidator(validators);
    ui->tabPatient->setModel(P.afficher());
    connect(ui->pb_annuler, &QPushButton::clicked, this, &MainWindow::reactivateModifierButton);
}

MainWindow::~MainWindow()
{
    delete ui;
}







void MainWindow::on_pb_ajouter_clicked()
{

int CIN=ui->le_CIN->text().toInt();
QString nom=ui->le_nom->text();
QString prenom=ui->le_prenom->text();
QString date=ui->le_date->text();
QString sexe=ui->le_sexe->text();
int Ntelephone=ui->le_num->text().toInt();
Patients P(CIN,nom,prenom,date,sexe,Ntelephone);

bool test=P.ajouter();
if(test)
{
    QMessageBox::information(nullptr,QObject::tr("OK"),
                             QObject::tr("Ajout effectué \n"
                                         "Click Cancel to exit."),QMessageBox::Cancel);
    ui->tabPatient->setModel(P.afficher());
}
else
    QMessageBox::critical(nullptr, QObject::tr("NOT OK"),
                          QObject::tr("Ajout non effectué.\n"
                                      "Click Cancel to exit."),QMessageBox::Cancel);

}

void MainWindow::on_pb_supprimer_clicked()
{
    // Récupérer l'index de la ligne sélectionnée
    QModelIndexList selectedRows = ui->tabPatient->selectionModel()->selectedRows();
    if (selectedRows.count() == 0) {
        QMessageBox::warning(nullptr, QObject::tr("Aucun patient sélectionné"),
                             QObject::tr("Sélectionnez un patient dans le tableau pour le supprimer."),
                             QMessageBox::Cancel);
        return;
    } else if (selectedRows.count() > 1) {
        QMessageBox::warning(nullptr, QObject::tr("Plusieurs patients sélectionnés"),
                             QObject::tr("Sélectionnez un seul patient dans le tableau pour le supprimer."),
                             QMessageBox::Cancel);
        return;
    }

    // Récupérer le patient sélectionné
    QModelIndex selectedRow = selectedRows.at(0);
    int row = selectedRow.row();
     int CIN = ui->tabPatient->model()->index(row, 0).data().toInt();

    Patients P1;
    bool test = P1.rechercher(CIN);
    if (test) {
        bool supprime = P1.supprimer(CIN);
        if (supprime) {
            QMessageBox::information(nullptr, QObject::tr("Suppression avec succès"),
                                     QObject::tr("Le patient a été supprimé de la base de données."),
                                     QMessageBox::Cancel);
            ui->tabPatient->setModel(P.afficher());
        } else {
            QMessageBox::critical(nullptr, QObject::tr("Erreur de suppression"),
                                  QObject::tr("Une erreur est survenue lors de la suppression du patient."),
                                  QMessageBox::Cancel);
        }
    } else {
        QMessageBox::warning(nullptr, QObject::tr("CIN introuvable"),
                             QObject::tr("Le patient avec ce CIN n'existe pas."),
                             QMessageBox::Cancel);
    }
}
void MainWindow::on_tabPatient_clicked(const QModelIndex &index)
{
    int row = index.row();
    int col = index.column();
    if (col == 0) {
        // Obtenir le CIN du patient sélectionné
         int CIN = ui->tabPatient->model()->index(row, col).data().toInt();
        // Obtenir les informations du patient à partir de la base de données
        Patients P;
        Patients patient = P.obtenirPatient(CIN);
        // Remplir les champs du formulaire avec les informations du patient
        ui->le_CIN->setText(QString::number(patient.getCIN()));
        ui->le_nom->setText(patient.getnom());
        ui->le_prenom->setText(patient.getprenom());
        ui->le_date->setText(patient.getdate());
        ui->le_sexe->setText(patient.getsexe());
        ui->le_num->setText(QString::number(patient.getNtelephone()));
    }
}



void MainWindow::on_pb_modifier_clicked()
{
    // Récupérer l'index de la ligne sélectionnée
    QModelIndexList selectedRows = ui->tabPatient->selectionModel()->selectedRows();
    if (selectedRows.count() == 0) {
        QMessageBox::warning(nullptr, QObject::tr("Aucun patient sélectionné"),
                             QObject::tr("Sélectionnez un patient dans le tableau pour le modifier."),
                             QMessageBox::Cancel);
        return;
    } else if (selectedRows.count() > 1) {
        QMessageBox::warning(nullptr, QObject::tr("Plusieurs patients sélectionnés"),
                             QObject::tr("Sélectionnez un seul patient dans le tableau pour le modifier."),
                             QMessageBox::Cancel);
        return;
    }

    // Récupérer le patient sélectionné
    QModelIndex selectedRow = selectedRows.at(0);
        int row = selectedRow.row();
        Patients patient = P.obtenirPatient(ui->tabPatient->model()->index(row, 0).data().toInt());

    // Afficher les informations du patient dans les champs d'édition
    ui->le_CIN->setText(QString::number(patient.getCIN()));
    ui->le_nom->setText(patient.getnom());
    ui->le_prenom->setText(patient.getprenom());
    ui->le_date->setText(patient.getdate());
    ui->le_sexe->setText(patient.getsexe());
    ui->le_num->setText(QString::number(patient.getNtelephone()));

    // Passer en mode modification
    ui->tabWidget->setCurrentIndex(0);
    ui->pb_ajouter->setEnabled(false);
    ui->pb_supprimer->setEnabled(false);
    ui->pb_modifier->setEnabled(false);
}


void MainWindow::on_pb_valider_clicked()
{

     int CIN=ui->le_CIN->text().toInt();
    QString nom=ui->le_nom->text();
    QString prenom=ui->le_prenom->text();
    QString date=ui->le_date->text();
    QString sexe=ui->le_sexe->text();
     int Ntelephone=ui->le_num->text().toInt();

    // Vérifiez si un patient est sélectionné dans le tableau
    QModelIndexList selectedRows = ui->tabPatient->selectionModel()->selectedRows();
    if (selectedRows.count() == 0) {
        QMessageBox::warning(nullptr, QObject::tr("Aucun patient sélectionné"),
                             QObject::tr("Sélectionnez un patient dans le tableau pour le modifier."),
                             QMessageBox::Cancel);
        return;
    } else if (selectedRows.count() > 1) {
        QMessageBox::warning(nullptr, QObject::tr("Plusieurs patients sélectionnés"),
                             QObject::tr("Sélectionnez un seul patient dans le tableau pour le modifier."),
                             QMessageBox::Cancel);
        return;
    }

    // Récupérer le patient sélectionné
    QModelIndex selectedRow = selectedRows.at(0);
    int row = selectedRow.row();
    Patients patient = P.obtenirPatient(ui->tabPatient->model()->index(row, 0).data().toInt());

    // Modifier les informations du patient
    patient.setnom(nom);
    patient.setprenom(prenom);
    patient.setdate(date);
    patient.setsexe(sexe);
    patient.setNtelephone(Ntelephone);

    // Enregistrer les modifications dans la base de données
    bool test = patient.modifier(CIN,nom,prenom,date,sexe,Ntelephone);
    if (test) {
        QMessageBox::information(nullptr,QObject::tr("OK"),
                                 QObject::tr("Modification effectuée \n"
                                             "Click Cancel to exit."),QMessageBox::Cancel);
        ui->tabPatient->setModel(P.afficher());
    } else {
        QMessageBox::critical(nullptr, QObject::tr("NOT OK"),
                              QObject::tr("Modification non effectuée.\n"
                                          "Click Cancel to exit."),QMessageBox::Cancel);
    }

}

void MainWindow::on_pb_annuler_clicked()
{
    ui->le_CIN->clear();
    ui->le_nom->clear();
    ui->le_prenom->clear();
    ui->le_date->clear();
    ui->le_sexe->clear();
    ui->le_num->clear();
}
void MainWindow::reactivateModifierButton()
{
    ui->pb_modifier->setEnabled(true);
    ui->pb_ajouter->setEnabled(true);
    ui->pb_supprimer->setEnabled(true);
}
