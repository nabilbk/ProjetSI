#include "parameterdock.h"
#include "ui_parameterdock.h"
#include "controller.h"

ParameterDock::ParameterDock(Controller *controller, QWidget *parent)
    : QDockWidget(parent), ui(new Ui::ParameterDock), controller(controller)
{
    ui->setupUi(this);
    setMinimumSize(269,350);
    ui->houghWidget->setVisible(false);
    ui->dynamicWidget->setVisible(false);
    ui->angleWidget->setVisible(false);
    ui->angleStatWidget->setVisible(false);
    ui->warningLabel->setVisible(false);
    ui->meanshiftWidget->setVisible(false);

    connect(this,                     SIGNAL(visibilityChanged(bool)),  controller,  SLOT(tickParameter(bool)));
    connect(ui->methodBox,            SIGNAL(currentIndexChanged(int)), this,        SLOT(methodChanged(int)));
    connect(ui->spinBox_C_minRadius,  SIGNAL(valueChanged(int)),        this,        SLOT(C_BlockMinRadius(int)));
    connect(ui->spinBox_C_maxRadius,  SIGNAL(valueChanged(int)),        this,        SLOT(C_BlockMaxRadius(int)));
    connect(ui->spinBox_C_maxRadius,  SIGNAL(valueChanged(int)),        controller,  SLOT(setParameters()));
    connect(ui->spinBox_C_minPoints,  SIGNAL(valueChanged(int)),        controller,  SLOT(setParameters()));
    connect(ui->spinBox_C_minRadius,  SIGNAL(valueChanged(int)),        controller,  SLOT(setParameters()));
    connect(ui->spinBox_C_threshold,  SIGNAL(valueChanged(int)),        controller,  SLOT(setParameters()));
    connect(ui->checkBox_showBinary,  SIGNAL(clicked(bool)),            controller,  SLOT(binaryBox_checked(bool)));
    connect(ui->checkBox_showEdges,   SIGNAL(clicked(bool)),            controller,  SLOT(edgeBox_checked(bool)));

    connect(ui->spinBox_H_minRadius,  SIGNAL(valueChanged(int)),        this,        SLOT(H_BlockMinRadius(int)));
    connect(ui->spinBox_H_maxRadius,  SIGNAL(valueChanged(int)),        this,        SLOT(H_BlockMaxRadius(int)));
    connect(ui->spinBox_H_dp,         SIGNAL(valueChanged(int)),        controller,  SLOT(setParameters()));
    connect(ui->spinBox_H_maxRadius,  SIGNAL(valueChanged(int)),        controller,  SLOT(setParameters()));
    connect(ui->spinBox_H_minDist,    SIGNAL(valueChanged(int)),        controller,  SLOT(setParameters()));
    connect(ui->spinBox_H_minRadius,  SIGNAL(valueChanged(int)),        controller,  SLOT(setParameters()));
    connect(ui->spinBox_H_param1,     SIGNAL(valueChanged(int)),        controller,  SLOT(setParameters()));
    connect(ui->spinBox_H_param2,     SIGNAL(valueChanged(int)),        controller,  SLOT(setParameters()));

    connect(ui->spinBox_Ca_apertureSize,      SIGNAL(valueChanged(int)),        controller,  SLOT(setParameters()));
    connect(ui->spinBox_Ca_horizontalBand,    SIGNAL(valueChanged(int)),        controller,  SLOT(setParameters()));
    connect(ui->spinBox_Ca_threshold,         SIGNAL(valueChanged(int)),        controller,  SLOT(setParameters()));
    connect(ui->spinBox_Ca_threshold2,        SIGNAL(valueChanged(int)),        controller,  SLOT(setParameters()));
    connect(ui->slider_Ca_tangentSensibility, SIGNAL(valueChanged(int)),        controller,  SLOT(setParameters()));
    connect(ui->slider_Ca_tangentSensibility, SIGNAL(valueChanged(int)),        controller,  SLOT(process()));

    connect(ui->meanshiftLevel,            SIGNAL(valueChanged(int)),     controller,  SLOT(setParameters()));
    connect(ui->meanshiftThresh1,          SIGNAL(valueChanged(double)),  controller,  SLOT(setParameters()));
    connect(ui->meanshiftThresh2,          SIGNAL(valueChanged(double)),  controller,  SLOT(setParameters()));
    connect(ui->meanshiftBinary,           SIGNAL(toggled(bool)),         controller,  SLOT(setParameters()));
    connect(ui->meanshiftBinaryInverse,    SIGNAL(toggled(bool)),         controller,  SLOT(setParameters()));
    connect(ui->meanshiftNoFilter,         SIGNAL(toggled(bool)),         controller,  SLOT(setParameters()));
    connect(ui->forcedProcess,             SIGNAL(toggled(bool)),         controller,  SLOT(setParameters()));

    connect(ui->checkBox_showCircles, SIGNAL(clicked(bool)),            controller,  SLOT(circleBox_checked(bool)));
    connect(ui->button_default,       SIGNAL(clicked()),                this,        SLOT(defaultValue_pressed()));

}

ParameterDock::~ParameterDock()
{
    delete ui;
}

int ParameterDock::getParam_C_threshold()
{
    return ui->spinBox_C_threshold->value();
}

int ParameterDock::getParam_C_minPoints()
{
    return ui->spinBox_C_minPoints->value();
}

int ParameterDock::getParam_C_minRadius()
{
    return ui->spinBox_C_minRadius->value();
}

int ParameterDock::getParam_C_maxRadius()
{
    return ui->spinBox_C_maxRadius->value();
}

int ParameterDock::getParam_H_dp()
{
    return ui->spinBox_H_dp->value();
}

int ParameterDock::getParam_H_minDist()
{
    return ui->spinBox_H_minDist->value();
}

int ParameterDock::getParam_H_param1()
{
    return ui->spinBox_H_param1->value();
}

int ParameterDock::getParam_H_param2()
{
    return ui->spinBox_H_param2->value();
}

int ParameterDock::getParam_H_minRadius()
{
    return ui->spinBox_H_minRadius->value();
}

int ParameterDock::getParam_H_maxRadius()
{
    return ui->spinBox_H_maxRadius->value();
}

int ParameterDock::getParam_Ca_threshold()
{
    return ui->spinBox_Ca_threshold->value();
}

int ParameterDock::getParam_Ca_threshold2()
{
    return ui->spinBox_Ca_threshold2->value();
}

int ParameterDock::getParam_Ca_horizontalBand()
{
    return ui->spinBox_Ca_horizontalBand->value();
}

int ParameterDock::getParam_Ca_tangentSensibility()
{
    return ui->slider_Ca_tangentSensibility->value();
}

int ParameterDock::getParam_Ca_apertureSize()
{
    return ui->spinBox_Ca_apertureSize->value();
}

bool ParameterDock::getParam_meanshift_forceProcess()
{
    return ui->forcedProcess->isChecked();
}

int ParameterDock::getParam_meanshift_level()
{
    return ui->meanshiftLevel->value();
}

double ParameterDock::getParam_meanshift_thresh1()
{
    return ui->meanshiftThresh1->value();
}

double ParameterDock::getParam_meanshift_thresh2()
{
    return ui->meanshiftThresh2->value();
}

bool ParameterDock::getParam_meanshift_binary()
{
    return ui->meanshiftBinary->isChecked();
}

bool ParameterDock::getParam_meanshift_binaryInverse()
{
    return ui->meanshiftBinaryInverse->isChecked();
}

void ParameterDock::setStat_radius_avg(double value)
{
    ui->label_SV_radius_avg->setText(QString::number(value));
}

void ParameterDock::setStat_radius_sd(double value)
{
    ui->label_SV_radius_sd->setText(QString::number(value));
}

void ParameterDock::setStat_radius_min(double value)
{
    ui->label_SV_radius_min->setText(QString::number(value));
}

void ParameterDock::setStat_radius_max(double value)
{
    ui->label_SV_radius_max->setText(QString::number(value));
}

void ParameterDock::setStat_irregularity_avg(double value)
{
    ui->label_SV_irregularity_avg->setText(QString::number(value));
}

void ParameterDock::setStat_irregularity_sd(double value)
{
    ui->label_SV_irregularity_sd->setText(QString::number(value));
}

void ParameterDock::setStat_angle(double value)
{
    ui->label_SV_angle->setText(QString::number(value));
}

void ParameterDock::setBinaryVal(bool val)
{
    ui->checkBox_showBinary->setChecked(val);;
}

void ParameterDock::H_BlockMinRadius(int val)
{
    if (val >= ui->spinBox_H_maxRadius->value())
        ui->spinBox_H_minRadius->setValue(ui->spinBox_H_maxRadius->value());
}

void ParameterDock::H_BlockMaxRadius(int val)
{
    if (val <= ui->spinBox_H_minRadius->value())
        ui->spinBox_H_maxRadius->setValue(ui->spinBox_H_minRadius->value());
}

void ParameterDock::C_BlockMinRadius(int val)
{
    if (val >= ui->spinBox_C_maxRadius->value())
        ui->spinBox_C_minRadius->setValue(ui->spinBox_C_maxRadius->value());
}

void ParameterDock::C_BlockMaxRadius(int val)
{
    if (val <= ui->spinBox_C_minRadius->value())
        ui->spinBox_C_maxRadius->setValue(ui->spinBox_C_minRadius->value());
}

void ParameterDock::methodChanged(int index)
{
    // Méthode custom
    if (index == 0)
    {
        ui->houghWidget->setVisible(false);
        ui->angleWidget->setVisible(false);
        ui->angleStatWidget->setVisible(false);
        ui->dynamicWidget->setVisible(false);
        ui->customWidget->setVisible(true);
        ui->button_default->setVisible(true);
        ui->circleStatWidget->setVisible(true);
        ui->meanshiftWidget->setVisible(false);
        if (ui->checkBox_showBinary->parentWidget() != ui->customWidget)
        {
            ui->angleLayout->removeWidget(ui->checkBox_showBinary);
            ui->customWidgetLayout->addWidget(ui->checkBox_showBinary, 7, 0);
        }
        ui->checkBox_showCircles->setVisible(true);
    }

    // Méthode de hough
    if (index == 1)
    {
        ui->customWidget->setVisible(false);
        ui->angleWidget->setVisible(false);
        ui->angleStatWidget->setVisible(false);
        ui->houghWidget->setVisible(true);
        ui->checkBox_showCircles->setVisible(true);
        ui->button_default->setVisible(true);
        ui->circleStatWidget->setVisible(true);
        ui->dynamicWidget->setVisible(false);
        ui->meanshiftWidget->setVisible(false);
    }

    // Méthode de canny (pour la détection des gouttes)
    if (index == 2)
    {
        ui->customWidget->setVisible(false);
        ui->houghWidget->setVisible(false);
        ui->circleStatWidget->setVisible(false);
        ui->angleWidget->setVisible(true);
        ui->button_default->setVisible(true);
        ui->angleStatWidget->setVisible(true);
        ui->dynamicWidget->setVisible(false);
        ui->meanshiftWidget->setVisible(false);
        if (ui->checkBox_showBinary->parentWidget() != ui->angleWidget)
        {
            ui->customWidgetLayout->removeWidget(ui->checkBox_showBinary);
            ui->angleLayout->addWidget(ui->checkBox_showBinary, 5, 0);
        }
        ui->checkBox_showCircles->setVisible(false);
    }

    // Rapporteur dynamique
    if (index == 3)
    {
        ui->customWidget->setVisible(false);
        ui->houghWidget->setVisible(false);
        ui->angleWidget->setVisible(false);
        ui->button_default->setVisible(false);
        ui->circleStatWidget->setVisible(false);
        ui->angleStatWidget->setVisible(true);
        ui->checkBox_showCircles->setVisible(false);
        ui->meanshiftWidget->setVisible(false);
        ui->dynamicWidget->setVisible(false);
    }

    // Méthode Mean Shift
    if (index == 4)
    {
        ui->houghWidget->setVisible(false);
        ui->angleWidget->setVisible(false);
        ui->angleStatWidget->setVisible(false);
        ui->customWidget->setVisible(false);
        ui->button_default->setVisible(false);
        ui->circleStatWidget->setVisible(false);
        ui->dynamicWidget->setVisible(false);
        ui->meanshiftWidget->setVisible(true);
        if (ui->checkBox_showBinary->parentWidget() != ui->customWidget)
        {
            ui->angleLayout->removeWidget(ui->checkBox_showBinary);
            ui->customWidgetLayout->addWidget(ui->checkBox_showBinary, 7, 0);
        }
        ui->checkBox_showCircles->setVisible(false);
    }

    // Méthode Dynamique
    if (index == 5)
    {
        ui->houghWidget->setVisible(false);
        ui->angleWidget->setVisible(false);
        ui->angleStatWidget->setVisible(false);
        ui->customWidget->setVisible(false);
        ui->button_default->setVisible(false);
        ui->circleStatWidget->setVisible(false);
        ui->meanshiftWidget->setVisible(false);
        ui->dynamicWidget->setVisible(true);
        if (ui->checkBox_showBinary->parentWidget() != ui->customWidget)
        {
            ui->angleLayout->removeWidget(ui->checkBox_showBinary);
            ui->customWidgetLayout->addWidget(ui->checkBox_showBinary, 7, 0);
        }
        ui->checkBox_showCircles->setVisible(false);
    }

    controller->methodChanged(index);
}

void ParameterDock::defaultValue_pressed()
{
    if (ui->methodBox->currentIndex() == 0)
    {
        ui->spinBox_C_maxRadius->setValue(200);
        ui->spinBox_C_minPoints->setValue(10);
        ui->spinBox_C_minRadius->setValue(3);
        ui->spinBox_C_threshold->setValue(100);
    }

    if (ui->methodBox->currentIndex() == 1)
    {
        ui->spinBox_H_dp->setValue(2);
        ui->spinBox_H_minDist->setValue(25);
        ui->spinBox_H_param1->setValue(50);
        ui->spinBox_H_param2->setValue(100);
        ui->spinBox_H_minRadius->setValue(1);
        ui->spinBox_H_maxRadius->setValue(35);
    }

    if (ui->methodBox->currentIndex() == 2)
    {
        ui->spinBox_Ca_apertureSize->setValue(1);
        ui->spinBox_Ca_horizontalBand->setValue(50);
        ui->spinBox_Ca_threshold->setValue(1);
        ui->spinBox_Ca_threshold2->setValue(25);
        ui->slider_Ca_tangentSensibility->setValue(10);
    }
    controller->setParameters();
}

void ParameterDock::setScale(QString str)
{
    ui->label_Echelle_Value->setText(str);
}

void ParameterDock::dynamicMethodSetted(QList<QString> list)
{
    ui->methodBox->setCurrentIndex(5);
    ui->listWidget->clear();
    for (int i = 0; i < list.count(); ++i)
    {
        ui->listWidget->addItem(list.at(i));
    }
}

void ParameterDock::processStreamEnabled(bool enabled)
{
    ui->warningLabel->setVisible(enabled);
    ui->methodBox->setEnabled(!enabled);
}
