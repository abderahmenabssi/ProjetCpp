#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "patients.h"
#include"facture.h"
#include <QMessageBox>
#include <QString>
#include<QModelIndex>
#include<QObject>
#include<QPixmap>
#include<QSize>
#include<QIcon>
#include <QtCharts/QChart>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QChartView>
#include <QFile>
#include <QTextStream>
#include <QPdfWriter>
#include <QPainter>
#include <QTableView>
#include <QTextDocument>
#include<QFileDialog>
#include <QDate>
#include<QBarSet>
#include<QBarSeries>
#include<QBarCategoryAxis>
#include <QValueAxis>
#include<QTextEdit>
 #include<QPieSlice>
#include <QPrinter>



QT_CHARTS_USE_NAMESPACE

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->le_date->setPlaceholderText("**/**/****");
    ui->le_sexe->setPlaceholderText("H ou F");
    ui->le_num->setPlaceholderText("(+216)");
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
    ui->tabHistorique->setModel(P.afficherH());
    connect(ui->pb_annuler, &QPushButton::clicked, this, &MainWindow::reactivateModifierButton);
     /*ui->le_cherche->setValidator(new QIntValidator(0,99999999, this));*/
     connect(ui->le_cherche, &QLineEdit::textChanged, this, [=](){
         ui->tabPatient->setModel(P.filtrerPatients(ui->le_cherche->text()));
     });
     /*connect(ui->pb_ajouter, &QPushButton::clicked, this, &Patients::enregistrerAjoutHistorique);*/




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
    ui->tabHistorique->setModel(P.afficherH());


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
            ui->tabHistorique->setModel(P.afficherH());
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
        ui->tabHistorique->setModel(P.afficherH());
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
/*
void MainWindow::on_pb_chercher_clicked()
{
            int CIN=ui->le_cherche->text().toInt();
            bool triCroissant;

            ui->tabPatient->setModel(P.filtrerPatients(CIN,triCroissant));


}*/
void MainWindow::on_le_cherche_textChanged(const QString &text)
{
    ui->tabPatient->setModel(P.filtrerPatients(text));
}




void MainWindow::on_RB_CIN_clicked()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    QString queryStr = "SELECT * FROM PATIENTT ORDER BY CIN";
    model->setQuery(queryStr);

    // configuration des en-têtes
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("CIN"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("PRENOM"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("NOM"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("DATE DE NAISSANCE"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Sexe"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("N°TELEPHONE"));

    // Affichage du modèle dans votre QTableView
    ui->tabPatient->setModel(model);
}



void MainWindow::on_RB_PRENOM_clicked()
{
    QString queryStr = "SELECT * FROM PATIENTT ORDER BY NOM";
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery(queryStr);
    // configuration des en-têtes
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("CIN"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("PRENOM"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("NOM"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("DATE DE NAISSANCE"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Sexe"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("N°TELEPHONE"));
    // Affichage du modèle dans votre QTableView
    ui->tabPatient->setModel(model);
}


void MainWindow::on_RB_NOM_clicked()
{
    QString queryStr = "SELECT * FROM PATIENTT ORDER BY PRENOM";
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery(queryStr);
    // configuration des en-têtes
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("CIN"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("PRENOM"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("NOM"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("DATE DE NAISSANCE"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Sexe"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("N°TELEPHONE"));
    // Affichage du modèle dans votre QTableView
    ui->tabPatient->setModel(model);
}




/*
void MainWindow::on_pb_tri_clicked()
{
    QSqlQueryModel* model = P.trierNoms();
       ui->tabPatient->setModel(model);
}
*/
void MainWindow::afficherStats()
{
    // Calculer les pourcentages des patients hommes et femmes
    int totalPatients = 0;
    int hommes = 0;
    int femmes = 0;

    QSqlQuery query;
    query.prepare("SELECT sexe FROM PATIENTT");

    if(query.exec()) {
        while (query.next()) {
            QString sexe = query.value(0).toString();
            if (sexe == "H") {
                hommes++;
            } else if (sexe == "F") {
                femmes++;
            }
            totalPatients++;
        }
    }

    // Calculer les pourcentages
    int pourcentageHommes = hommes * 100 / totalPatients;
    int pourcentageFemmes = femmes * 100 / totalPatients;

    // Si les stats n'ont pas encore été ajoutées, créer un nouveau graphique
    if (!statsAdded) {
        // Créer une série de données pour le diagramme circulaire
        QPieSeries *series = new QPieSeries();
        series->append(QString("Hommes %1%").arg(pourcentageHommes), pourcentageHommes);
        series->append(QString("Femmes %1%").arg(pourcentageFemmes), pourcentageFemmes);
        QChart *chart = new QChart();
        chart->addSeries(series);
        chart->setTitle("Répartition des patients par sexe");

        QPieSlice *hommeSlice = series->slices().at(0);
               hommeSlice->setLabel(QString("%1% - Hommes").arg(pourcentageHommes));

               QPieSlice *femmeSlice = series->slices().at(1);
               femmeSlice->setLabel(QString("%1% - Femmes").arg(pourcentageFemmes));

        QChartView *chartView = new QChartView(chart);
        chartView->setRenderHint(QPainter::Antialiasing);
        chartView->resize(500, 500);

        ui->verticalLayout->addWidget(chartView);

        // Marquer les stats comme ayant été ajoutées
        statsAdded = true;
    } else {
        // Si les stats ont déjà été ajoutées, mettre à jour les données de la série et les étiquettes des tranches
        QChartView *chartView = dynamic_cast<QChartView*>(ui->verticalLayout->itemAt(0)->widget());
        QPieSeries *series = dynamic_cast<QPieSeries*>(chartView->chart()->series().at(0));
        series->slices().at(0)->setValue(pourcentageHommes);
        series->slices().at(1)->setValue(pourcentageFemmes);
        series->slices().at(0)->setLabel(QString("%1%").arg(pourcentageHommes));
        series->slices().at(1)->setLabel(QString("%1%").arg(pourcentageFemmes));
    }
}


void MainWindow::afficherStatsAge()
{
    // Créer des groupes d'âge pour les patients
        int moinsDe30Ans = 0;
        int entre30Et50Ans = 0;
        int plusDe50Ans = 0;

        QSqlQuery query;
        query.prepare("SELECT DATE_N FROM PATIENTT");

        if(query.exec()) {
            while (query.next()) {
                QString dateNaissanceStr = query.value(0).toString();

                // Vérifier la saisie de la date de naissance avec l'expression régulière
                if (!QRegExp("^(0[1-9]|[1-2][0-9]|3[0-1])/(0[1-9]|1[0-2])/([0-9]{4})$").exactMatch(dateNaissanceStr)) {
                    // La date de naissance n'est pas au bon format, passer à la ligne suivante
                    continue;
                }

                QDate dateNaissance = QDate::fromString(dateNaissanceStr, "dd/MM/yyyy");
                QDate currentDate = QDate::currentDate();
                int age = dateNaissance.daysTo(currentDate) / 365;

                if (age < 30) {
                    moinsDe30Ans++;
                } else if (age >= 30 && age <= 50) {
                    entre30Et50Ans++;
                } else if (age > 50) {
                    plusDe50Ans++;
                }
            }
        }
    // Calculer les pourcentages et les informations sur les groupes d'âge
    int totalPatients = moinsDe30Ans + entre30Et50Ans + plusDe50Ans;
    int pourcentageMoinsDe30Ans = moinsDe30Ans * 100 / totalPatients;
    QString infosMoinsDe30Ans = QString("%1% - Moins de 30 ans").arg(pourcentageMoinsDe30Ans);
    int pourcentageEntre30Et50Ans = entre30Et50Ans * 100 / totalPatients;
    QString infosEntre30Et50Ans = QString("%1% - Entre 30 et 50 ans").arg(pourcentageEntre30Et50Ans);
    int pourcentagePlusDe50Ans = plusDe50Ans * 100 / totalPatients;
    QString infosPlusDe50Ans = QString("%1% - Plus de 50 ans").arg(pourcentagePlusDe50Ans);

    // Si les stats n'ont pas encore été ajoutées, créer un nouveau graphique
    if (!statsAgeAdded) {
        // Créer une série de données pour le diagramme circulaire
        QPieSeries *series = new QPieSeries();
        series->append(QString("Moins de 30 ans"), pourcentageMoinsDe30Ans);
        series->append(QString("Entre 30 et 50 ans"), pourcentageEntre30Et50Ans);
        series->append(QString("Plus de 50 ans"), pourcentagePlusDe50Ans);

        QChart *chart = new QChart();
        chart->addSeries(series);
        chart->setTitle("Répartition des patients par âge");

        QPieSlice *moinsDe30AnsSlice = series->slices().at(0);
        moinsDe30AnsSlice->setLabel(infosMoinsDe30Ans);

        QPieSlice *entre30Et50AnsSlice = series->slices().at(1);
        entre30Et50AnsSlice->setLabel(infosEntre30Et50Ans);

        QPieSlice *plusDe50AnsSlice = series->slices().at(2);
        plusDe50AnsSlice->setLabel(infosPlusDe50Ans);

        QChartView *chartView = new QChartView(chart);
        chartView->setRenderHint(QPainter::Antialiasing);
        chartView->resize(500, 500);

        ui->verticalLayout_2->addWidget(chartView);

        // Marquer les stats comme ayant été ajout
        statsAgeAdded = true;
    } else {
        // Mettre à jour les données de la série et des étiquettes
        QChartView *chartView = dynamic_cast<QChartView*>(ui->verticalLayout_2->itemAt(0)->widget());
        QPieSeries *series = dynamic_cast<QPieSeries*>(chartView->chart()->series().at(0));
        series->slices().at(0)->setValue(pourcentageMoinsDe30Ans);
        series->slices().at(0)->setLabel(QString("%1%").arg(pourcentageMoinsDe30Ans));
        series->slices().at(1)->setValue(pourcentageEntre30Et50Ans);
        series->slices().at(1)->setLabel(QString("%1%").arg(pourcentageEntre30Et50Ans));
        series->slices().at(2)->setValue(pourcentagePlusDe50Ans);
        series->slices().at(2)->setLabel(QString("%1%").arg(pourcentagePlusDe50Ans));

}
}

void MainWindow::on_pb_stat_clicked()
{
     ui->tabWidget->setCurrentIndex(1);
    afficherStats();
 afficherStatsAge();

}

/*
void MainWindow::afficherStatsAge()
{
    // Créer des groupes d'âge pour les patients
    int moinsDe30Ans = 0;
    int entre30Et50Ans = 0;
    int plusDe50Ans = 0;

    QSqlQuery query;
    query.prepare("SELECT DATE_N FROM PATIENTT");

    if(query.exec()) {
        while (query.next()) {
            QString dateNaissanceStr = query.value(0).toString();

            // Vérifier la saisie de la date de naissance avec l'expression régulière
            if (!QRegExp("^(0[1-9]|[1-2][0-9]|3[0-1])/(0[1-9]|1[0-2])/([0-9]{4})$").exactMatch(dateNaissanceStr)) {
                // La date de naissance n'est pas au bon format, passer à la ligne suivante
                continue;
            }

            QDate dateNaissance = QDate::fromString(dateNaissanceStr, "dd/MM/yyyy");
            QDate currentDate = QDate::currentDate();
            int age = dateNaissance.daysTo(currentDate) / 365;

            if (age < 30) {
                moinsDe30Ans++;
            } else if (age >= 30 && age <= 50) {
                entre30Et50Ans++;
            } else if (age > 50) {
                plusDe50Ans++;
            }
        }
    }

    // Calculer les pourcentages et les informations sur les groupes d'âge
    int totalPatients = moinsDe30Ans + entre30Et50Ans + plusDe50Ans;
    int pourcentageMoinsDe30Ans = moinsDe30Ans * 100 / totalPatients;
    int pourcentageEntre30Et50Ans = entre30Et50Ans * 100 / totalPatients;
    int pourcentagePlusDe50Ans = plusDe50Ans * 100 / totalPatients;

    // Si les stats n'ont pas encore été ajoutées, créer un nouveau graphique
    if (!statsAgeAdded) {
        // Créer une série de données pour le diagramme à barres
        QBarSet *set = new QBarSet("Répartition des patients par âge");
        *set << pourcentageMoinsDe30Ans << pourcentageEntre30Et50Ans << pourcentagePlusDe50Ans;

        QBarSeries *series = new QBarSeries();
        series->append(set);

        QChart *chart = new QChart();
        chart->addSeries(series);
        chart->setTitle("Répartition des patients par âge");

        QStringList categories;
        categories << "Moins de 30 ans" << "Entre 30 et 50 ans" << "Plus de 50 ans";

        QBarCategoryAxis *axisX = new QBarCategoryAxis();
        axisX->append(categories);
        chart->addAxis(axisX, Qt::AlignBottom);
        series->attachAxis(axisX);

        QValueAxis *axisY = new QValueAxis();
        chart->addAxis(axisY, Qt::AlignLeft);
        series->attachAxis(axisY);

        QChartView *chartView = new QChartView(chart);
        chartView->setRenderHint(QPainter::Antialiasing);
        chartView->resize(500, 500);

        ui->verticalLayout_2->addWidget(chartView);

        // Marquer les stats comme ayant été ajout
        statsAgeAdded = true;
    } else {
        // Mettre à jour les données de la série et des étiquettes
        QChartView *chartView = dynamic_cast<QChartView*>(ui->verticalLayout_2->itemAt(0)->widget());
        QBarSeries *series = dynamic_cast<QBarSeries*>(chartView->chart()->series().at(0));
        QBarSet *set = series->barSets().at(0);
        *set << pourcentageMoinsDe30Ans << pourcentageEntre30Et50Ans << pourcentagePlusDe50Ans;

        chartView->chart()->setTitle("Répartition des patients par âge");

    }
}


*/







/*
void MainWindow::on_pb_pdf_clicked()
{

    // Récupérer le modèle de la table des patients
       QAbstractItemModel *model = ui->tabPatient->model();

       // Créer un document texte pour le contenu du PDF
       QTextDocument doc;

       // Ajouter le grand titre
       QString titleHTML = "<html><head><style>h1 { text-align: center; font-size: 100pt;color:red; }</style></head><body><h1>Liste Des Patients</h1></body></html>";
       doc.setHtml(titleHTML);

       // Ajouter la table des patients
       QString tableHTML = "<html><body><table border='1'>";
       // Ajouter les en-têtes de colonne
       tableHTML.append("<tr>");
       for (int column = 0; column < model->columnCount(); ++column) {
           QString header = model->headerData(column, Qt::Horizontal, Qt::DisplayRole).toString();
           tableHTML.append("<th>").append(header).append("</th>");
       }
       tableHTML.append("</tr>");
       // Ajouter les données de chaque ligne
       for (int row = 0; row < model->rowCount(); ++row) {
           tableHTML.append("<tr>");
           for (int column = 0; column < model->columnCount(); ++column) {
               QString data = model->data(model->index(row, column)).toString();
               tableHTML.append("<td>").append(data).append("</td>");
           }
           tableHTML.append("</tr>");
       }
       tableHTML.append("</table></body></html>");
       doc.setHtml(doc.toHtml() + tableHTML);

       // Agrandir la taille de police de la table
       QTextCursor cursor(&doc);
       QTextBlockFormat blockFormat;
       QTextCharFormat charFormat;
       cursor.select(QTextCursor::Document);
       cursor.mergeBlockFormat(blockFormat);
       charFormat.setFontPointSize(100);
       cursor.mergeCharFormat(charFormat);



       // Ouvrir un fichier pour écrire le PDF
       QString fileName = QFileDialog::getSaveFileName(this, "Enregistrer sous...", "", "Fichiers PDF (*.pdf)");
       if (fileName.isEmpty()) {
           return;
       }
       QPdfWriter writer(fileName);
       writer.setPageSize(QPageSize(QPageSize::A4));
       writer.setPageMargins(QMarginsF(30, 30, 30, 30));



       // Dessiner une bordure autour de la page
           QPainter painter(&writer);
           painter.setPen(Qt::SolidLine);
           painter.drawRect(0, 0, writer.width(), writer.height());

           // Imprimer le document dans le fichier PDF
           painter.setRenderHint(QPainter::Antialiasing);
           doc.drawContents(&painter);
           painter.end();



       // Afficher un message de confirmation
       QMessageBox::information(this, "PDF généré", "Le PDF a été généré avec succès.");}

*/

void MainWindow::on_pb_pdf_clicked()
{
    QPdfWriter pdf("C:/Users/abdou/Bureau/Liste_Patients.pdf");

    QPainter painter(&pdf);
    int i = 4100;

    // Définir la couleur rouge pour SOS Alarmy
    QColor sosColor(0xFF0000);
    painter.setPen(sosColor);

    painter.setFont(QFont("Montserrat SemiBold", 11));
    QDate cd = QDate::currentDate();
    painter.drawText(8400,1000,cd.toString("Tunis"));
    painter.drawText(8100,1300,cd.toString("dd/MM/yyyy"));
/*
    // Afficher SOS Alarmy en haut de la page
    painter.setFont(QFont("Montserrat SemiBold", 25));
    painter.drawText(100, 300, "SOS Alarmy");
    */

    // Insérer le logo en dessous de SOS Alarmy
    QImage logo("C:/Users/abdou/OneDrive/Documents/Patient/images/sos.png");
    if (!logo.isNull()) {
        logo = logo.scaledToHeight(1500, Qt::SmoothTransformation);
        painter.drawImage(QRectF(10, 2, logo.width(), logo.height()), logo);
    }

    QColor titleColor(0x341763);
    painter.setPen(titleColor);
    painter.setFont(QFont("Montserrat SemiBold", 25));

    painter.drawText(3000,2700,"Liste des Patients");

    painter.setPen(QPen(Qt::black, 50));
    painter.drawRect(0, 0, pdf.width(), pdf.height());


    painter.setPen(Qt::black);
    painter.setFont(QFont("Time New Roman", 15));
    painter.drawRect(100,3300,9400,500);

    painter.setFont(QFont("Montserrat SemiBold", 10));

    painter.drawText(500,3600,"CIN");
    painter.drawText(2000,3600,"Prenom");
    painter.drawText(3300,3600,"Nom");
    painter.drawText(4500,3600,"Date de naissance");
    painter.drawText(6500,3600,"Sexe");
    painter.drawText(7500,3600,"N°TELEPHONE");
    painter.setFont(QFont("Montserrat", 10));

    QSqlQuery query;
    query.prepare("select * from PATIENTT");
    query.exec();
    int y=4300;
    while (query.next())
    {
        painter.drawLine(100,y,9500,y);
        y+=500;
        painter.drawText(500,i,query.value(0).toString());
        painter.drawText(2000,i,query.value(1).toString());
        painter.drawText(3300,i,query.value(2).toString());
        painter.drawText(4500,i,query.value(3).toString());
        painter.drawText(6500,i,query.value(4).toString());
        painter.drawText(7500,i,query.value(5).toString());

        i = i + 500;
    }

    painter.drawLine(100, 3800, 9500, 3800);
    painter.drawLine(100, i, 9500, i);

    // Insérer le cachet en bas de page
    QImage cachet("C:/Users/abdou/OneDrive/Documents/Patient/images/cachet.png");
    if (!cachet.isNull()) {
        cachet = cachet.scaledToWidth(1500, Qt::SmoothTransformation);
        painter.drawImage(QRectF(7500, 11000, cachet.width(), cachet.height()), cachet);
    }

    QMessageBox::information(this, QObject::tr("PDF Enregistré!"),
        QObject::tr("PDF Enregistré!.\n" "Click Cancel to exit."), QMessageBox::Cancel);
}

void MainWindow::on_pb_historique_clicked()
{
    ui->tabWidget->setCurrentIndex(2);
    ecrirefichier();
    readfile();
}

void MainWindow::on_pb_ajouterF_clicked()
{
    int num=ui->le_numF->text().toInt();
        QString nomp=ui->le_patient->text();
         QString datef=ui->le_time->text();
         int prix_c=ui->le_consultation->text().toInt();
         int prix_m=ui->le_medicaments->text().toInt();




        facture f(num,nomp,datef,prix_c,prix_m);
    bool test=f.ajouter();
    if (test)
            { //Actualiser
            QMessageBox:: information(nullptr, QObject::tr("OK"),
                                               QObject::tr("Ajout effectué\n"
                                                           "click cancel to exit."),QMessageBox::Cancel);}



        else
            QMessageBox::critical(nullptr, QObject::tr("Not OK"),
                                  QObject::tr("Ajout non effectué.\n"
                                              "click Cancel to exit."),QMessageBox::Cancel);
}
/*
void genererFacturePDF(int num, QString nomp, QString datef, int montant, QString filename)
{
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(filename);

    QPainter painter(&printer);
    painter.drawText(100, 100, "Numéro de la facture : " + QString::number(num));
    painter.drawText(100, 150, "Nom du patient : " + nomp);
    painter.drawText(100, 200, "Date de la facture : " + datef);
    painter.drawText(100, 250, "Montant de la facture : " + QString::number(montant));

    painter.end();
}
*/

void MainWindow::on_pb_genererF_clicked()
{
    int numero = ui->le_generer->text().toInt();
    QPdfWriter pdf("C:/Users/abdou/Bureau/Facture.pdf");

    QPainter painter(&pdf);
    int i = 4100;

    // Définir la couleur rouge pour SOS Alarmy
    QColor sosColor(0xFF0000);
    painter.setPen(sosColor);

    painter.setFont(QFont("Montserrat SemiBold", 11));
    QDate cd = QDate::currentDate();
    QDateTime now = QDateTime::currentDateTime();
    QString time = now.toString("yyyy-MM-dd HH:mm:ss");
    painter.drawText(8400,1000,cd.toString("Tunis"));
    painter.drawText(7700,1300,now.toString("yyyy-MM-dd HH:mm:ss"));

    // Insérer le logo en dessous de SOS Alarmy
    QImage logo("C:/Users/abdou/OneDrive/Documents/Patient/images/sos.png");
    if (!logo.isNull()) {
        logo = logo.scaledToHeight(1500, Qt::SmoothTransformation);
        painter.drawImage(QRectF(10, 2, logo.width(), logo.height()), logo);
    }

    QColor titleColor(0x341763);
    painter.setPen(titleColor);
    painter.setFont(QFont("Montserrat SemiBold", 25));

    painter.drawText(3000,2700,"Facture à payer");

    painter.setPen(QPen(Qt::black, 50));
    painter.drawRect(0, 0, pdf.width(), pdf.height());


    painter.setPen(Qt::black);
    painter.setFont(QFont("Time New Roman", 15));
    painter.drawRect(100,3300,9400,500);

    painter.setFont(QFont("Montserrat SemiBold", 10));

    painter.drawText(500,3600,"Numero Facture");
    painter.drawText(2000,3600,"Nom et Prenom");
    painter.drawText(3500,3600,"Date");
    painter.drawText(4500,3600,"Prix Consultation");
    painter.drawText(6000,3600,"Prix Medicaments");
    painter.drawText(8100,3600,"Montant Total");
    painter.setFont(QFont("Montserrat", 10));

    QSqlQuery query;
    query.prepare("SELECT * FROM facture WHERE numero=:numero");
    query.bindValue(":numero", numero);
    query.exec();

    if (!query.next()) {
        QMessageBox::critical(this, "Erreur", "Numéro de facture introuvable !");
        return;
    }

    painter.drawLine(100, 4300, 9500, 4300);
    painter.drawText(500,i,query.value(0).toString());
    painter.drawText(2000,i,query.value(1).toString());
    painter.drawText(3500,i,query.value(2).toString());
    painter.drawText(4700,i,query.value(3).toString());
    painter.drawText(6000,i,query.value(4).toString());
   int x= query.value(3).toInt()+query.value(4).toInt();
    painter.drawText(8100,i,QString::number(x));

    i = i + 500;

    painter.drawLine(100, 3800, 9500, 3800);
    painter.drawLine(100, i, 9500, i);

    // Insérer le cachet en bas de page
    QImage cachet("C:/Users/abdou/OneDrive/Documents/Patient/images/cachet.png");
    if (!cachet.isNull()) {
        cachet = cachet.scaledToWidth(1500, Qt::SmoothTransformation);
        painter.drawImage(QRectF(7500, 11000, cachet.width(), cachet.height()), cachet);
    }

    QMessageBox::information(this, QObject::tr("PDF Enregistré!"),
        QObject::tr("PDF Enregistré!.\n" "Click Cancel to exit."), QMessageBox::Cancel);
}

void MainWindow::ecrirefichier(){
    QSqlQuery query;
    query.prepare("select * from HISTORIQUE ");
    if(query.exec()){
        QString hist = QCoreApplication::applicationDirPath() + "/historique.txt";
        QFile file(hist);
        if(!file.open(QFile::ReadOnly | QFile::Text)){
            QMessageBox::warning(this,"title","file not open");
            return;
        }
        QTextStream in(&file);
        QString fileContent = in.readAll();
        file.close();

        while(query.next()){
            QString action=query.value("ACTION").toString();
            QString PATIENT=query.value("PATIENT").toString();
            QString TIME=query.value("TIME").toString();

            QString newRecord = "PATIENT : "+ PATIENT +" Date : "+ TIME +" ACTION : "+ action +"\n";
            if(!fileContent.contains(newRecord)){
                if(!file.open(QFile::Append | QFile::Text)){
                    QMessageBox::warning(this,"title","file not open");
                    return;
                }
                QTextStream out(&file);
                out << newRecord;
                file.flush();
                file.close();
            }
        }
    }
}

void MainWindow::readfile()
{
    QString filepath = QCoreApplication::applicationDirPath() + "/historique.txt";
    QFile file(filepath);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        QMessageBox::warning(this,"title","file not open");
    }
    QTextStream in(&file);
    QString text=in.readAll();
    ui->texthist->setPlainText(text);
    file.close();
}
