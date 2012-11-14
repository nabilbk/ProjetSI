#pragma once

#include <QDockWidget>

namespace Ui
{
    class ParameterDock;
}

class Controller;

class ParameterDock : public QDockWidget
{
    Q_OBJECT

protected:
    Ui::ParameterDock *ui;

    Controller *controller;

public:
    explicit ParameterDock(Controller *controller, QWidget *parent = 0);

    virtual ~ParameterDock();

    int getParam_C_threshold();

    int getParam_C_minPoints();

    int getParam_C_minRadius();

    int getParam_C_maxRadius();

    int getParam_H_dp();

    int getParam_H_minDist();

    int getParam_H_param1();

    int getParam_H_param2();

    int getParam_H_minRadius();

    int getParam_H_maxRadius();

    int getParam_Ca_threshold();

    int getParam_Ca_threshold2();

    int getParam_Ca_horizontalBand();

    int getParam_Ca_tangentSensibility();

    int getParam_Ca_apertureSize();

    bool getParam_meanshift_forceProcess();

    int getParam_meanshift_level();

    double getParam_meanshift_thresh1();

    double getParam_meanshift_thresh2();

    bool getParam_meanshift_binary();

    bool getParam_meanshift_binaryInverse();

    void setStat_radius_avg(double value);

    void setStat_radius_sd(double value);

    void setStat_radius_min(double value);

    void setStat_radius_max(double value);

    void setStat_irregularity_avg(double value);

    void setStat_irregularity_sd(double value);

    void setStat_angle(double value);

    void setBinaryVal(bool val);

    void dynamicMethodSetted(QList<QString> list);

    void processStreamEnabled(bool enabled);

public slots:

    void C_BlockMinRadius(int val);

    void C_BlockMaxRadius(int val);

    void H_BlockMinRadius(int val);

    void H_BlockMaxRadius(int val);

    void methodChanged(int index);

    void defaultValue_pressed();

    void setScale(QString str);
};
