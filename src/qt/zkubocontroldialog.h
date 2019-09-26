// Copyright (c) 2017-2019 The kubocoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef zkuboCONTROLDIALOG_H
#define zkuboCONTROLDIALOG_H

#include <QDialog>
#include <QTreeWidgetItem>
#include "zkubo/zerocoin.h"
#include "privacydialog.h"

class CZerocoinMint;
class WalletModel;

namespace Ui {
class zkuboControlDialog;
}

class CzkuboControlWidgetItem : public QTreeWidgetItem
{
public:
    explicit CzkuboControlWidgetItem(QTreeWidget *parent, int type = Type) : QTreeWidgetItem(parent, type) {}
    explicit CzkuboControlWidgetItem(int type = Type) : QTreeWidgetItem(type) {}
    explicit CzkuboControlWidgetItem(QTreeWidgetItem *parent, int type = Type) : QTreeWidgetItem(parent, type) {}

    bool operator<(const QTreeWidgetItem &other) const;
};

class zkuboControlDialog : public QDialog
{
    Q_OBJECT

public:
    explicit zkuboControlDialog(QWidget *parent);
    ~zkuboControlDialog();

    void setModel(WalletModel* model);

    static std::set<std::string> setSelectedMints;
    static std::set<CMintMeta> setMints;
    static std::vector<CMintMeta> GetSelectedMints();

private:
    Ui::zkuboControlDialog *ui;
    WalletModel* model;
    PrivacyDialog* privacyDialog;

    void updateList();
    void updateLabels();

    enum {
        COLUMN_CHECKBOX,
        COLUMN_DENOMINATION,
        COLUMN_PUBCOIN,
        COLUMN_VERSION,
        COLUMN_PRECOMPUTE,
        COLUMN_CONFIRMATIONS,
        COLUMN_ISSPENDABLE
    };
    friend class CzkuboControlWidgetItem;

private slots:
    void updateSelection(QTreeWidgetItem* item, int column);
    void ButtonAllClicked();
};

#endif // zkuboCONTROLDIALOG_H
