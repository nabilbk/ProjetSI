#include "savedialog.h"
#include "ui_savedialog.h"

SaveDialog::SaveDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SaveDialog)
{
    ui->setupUi(this);

    connect(ui->saveSequences, SIGNAL(clicked()), this, SLOT(saveSequencesButtonPressed()));
    connect(ui->saveResult,    SIGNAL(clicked()), this, SLOT(saveResultButtonPressed()));
    connect(ui->exportResult,  SIGNAL(clicked()), this, SLOT(exportResultButtonPressed()));
    connect(ui->importFile,    SIGNAL(clicked()), this, SLOT(importFileButtonPressed()));
}

SaveDialog::~SaveDialog()
{
    delete ui;
}

void SaveDialog::saveSequencesButtonPressed()
{
    this->hide();
    emit saveSequences();
}

void SaveDialog::saveResultButtonPressed()
{
    this->hide();
    emit saveResult();
}

void SaveDialog::exportResultButtonPressed()
{
    this->hide();
    emit exportResult();
}

void SaveDialog::importFileButtonPressed()
{
    this->hide();
    emit importFile();
}
