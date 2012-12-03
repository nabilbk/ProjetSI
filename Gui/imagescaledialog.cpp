#include "imagescaledialog.h"
#include "ui_imagescaledialog.h"

ImageScaleDialog::ImageScaleDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ImageScaleDialog)
{
    ui->setupUi(this);
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(scale()));
    ui->sizeBox->setValidator(new QDoubleValidator(NULL));
}

ImageScaleDialog::~ImageScaleDialog()
{
    delete ui;
}

void ImageScaleDialog::scale()
{
    emit scaleSet(ui->sizeBox->text().toDouble());
}
