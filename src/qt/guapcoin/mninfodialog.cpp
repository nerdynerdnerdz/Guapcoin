// Copyright (c) 2019-2020 The PIVX developers
// Copyright (c) 2019-2020 The Guapcoin developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "qt/guapcoin/mninfodialog.h"
#include "qt/guapcoin/forms/ui_mninfodialog.h"
#include "walletmodel.h"
#include "wallet/wallet.h"
#include "guiutil.h"
#include "qt/guapcoin/qtutils.h"
#include <QDateTime>

MnInfoDialog::MnInfoDialog(QWidget *parent) :
    FocusedDialog(parent),
    ui(new Ui::MnInfoDialog)
{
    ui->setupUi(this);
    this->setStyleSheet(parent->styleSheet());
    setCssProperty(ui->frame, "container-dialog");
    setCssProperty(ui->labelTitle, "text-title-dialog");
    setCssTextBodyDialog({ui->labelAmount, ui->labelSend, ui->labelInputs, ui->labelFee, ui->labelId});
    setCssProperty({ui->labelDivider1, ui->labelDivider4, ui->labelDivider6, ui->labelDivider7, ui->labelDivider8, ui->labelDivider9}, "container-divider");
    setCssTextBodyDialog({ui->textAmount, ui->textAddress, ui->textInputs, ui->textStatus, ui->textId, ui->textExport});
    setCssProperty({ui->pushCopy, ui->pushCopyId, ui->pushExport}, "ic-copy-big");
    setCssProperty(ui->btnEsc, "ic-close");
    connect(ui->btnEsc, &QPushButton::clicked, this, &MnInfoDialog::close);
    connect(ui->pushCopy, &QPushButton::clicked, [this](){ copyInform(pubKey, tr("Masternode public key copied")); });
    connect(ui->pushCopyId, &QPushButton::clicked, [this](){ copyInform(txId, tr("Collateral tx id copied")); });
    connect(ui->pushExport, &QPushButton::clicked, [this](){ exportMN = true; accept(); });
}

void MnInfoDialog::setData(QString pubKey, QString name, QString address, QString txId, QString outputIndex, QString status)
{
    this->pubKey = pubKey;
    this->txId = txId;
    QString shortPubKey = pubKey;
    QString shortTxId = txId;
    if (shortPubKey.length() > 20) {
        shortPubKey = shortPubKey.left(13) + "..." + shortPubKey.right(13);
    }
    if (shortTxId.length() > 20) {
        shortTxId = shortTxId.left(12) + "..." + shortTxId.right(12);
    }
    ui->textId->setText(shortPubKey);
    ui->textAddress->setText(address);
    ui->textAmount->setText(shortTxId);
    ui->textInputs->setText(outputIndex);
    ui->textStatus->setText(status);
}

void MnInfoDialog::copyInform(QString& copyStr, QString message)
{
    GUIUtil::setClipboard(copyStr);
    if (!snackBar) snackBar = new SnackBar(nullptr, this);
    snackBar->setText(tr(message.toStdString().c_str()));
    snackBar->resize(this->width(), snackBar->height());
    openDialog(snackBar, this);
}

void MnInfoDialog::reject()
{
    if (snackBar && snackBar->isVisible()) snackBar->hide();
    QDialog::reject();
}

MnInfoDialog::~MnInfoDialog()
{
    if (snackBar) delete snackBar;
    delete ui;
}
