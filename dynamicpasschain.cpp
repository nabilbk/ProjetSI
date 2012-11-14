#include "dynamicpasschain.h"
#include "ui_dynamicpasschain.h"

#include <QDebug>

DynamicPassChain::DynamicPassChain(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dynamicPassChain)
{
    ui->setupUi(this);

    connect(ui->available, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(addItemToChosen()));
    connect(ui->chosen,    SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(removeItemToChosen()));

    connect(ui->addButton,    SIGNAL(clicked()), this, SLOT(addItemToChosen()));
    connect(ui->removeButton, SIGNAL(clicked()), this, SLOT(removeItemToChosen()));

    connect(ui->addAllButton,    SIGNAL(clicked()), this, SLOT(addAllItemsToChosen()));
    connect(ui->removeAllButton, SIGNAL(clicked()), this, SLOT(removeAllItemsToChosen()));

    connect(this, SIGNAL(accepted()), this, SLOT(dialogAccepted()));
}

DynamicPassChain::~DynamicPassChain()
{
    delete ui;
}

void DynamicPassChain::addItemToChosen()
{
    ui->chosen->addItem(ui->available->item(ui->available->currentRow())->text());
}

void DynamicPassChain::removeItemToChosen()
{
    ui->chosen->takeItem(ui->chosen->currentRow());
}

void DynamicPassChain::addAllItemsToChosen()
{
    for (int i = 0; i < ui->available->count(); ++i)
    {
        ui->available->setCurrentRow(i);
        addItemToChosen();
    }
}

void DynamicPassChain::removeAllItemsToChosen()
{
    ui->chosen->clear();
}

void DynamicPassChain::dialogAccepted()
{
    int count = ui->chosen->count();

    list.clear();
    for (int i = 0; i < count; ++i)
    {
        ui->chosen->setCurrentRow(i);
        list.append(ui->chosen->item(ui->chosen->currentRow())->text());
    }

    emit emitList(list);
}
