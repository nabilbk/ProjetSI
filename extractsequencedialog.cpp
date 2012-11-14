#include "extractsequencedialog.h"
#include "ui_extractsequencedialog.h"

ExtractSequenceDialog::ExtractSequenceDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExtractSequenceDialog)
{
    ui->setupUi(this);
    QObject::connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(extract()));
}

ExtractSequenceDialog::~ExtractSequenceDialog()
{
    delete ui;
}

void ExtractSequenceDialog::extract()
{
    emit notifieExtractSequence(ui->spinBox->value());
}
