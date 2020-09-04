#ifndef FILEPROPERTYINFO_H
#define FILEPROPERTYINFO_H

#include "common/common.h"  //fileinfo
#include <QDialog>

namespace Ui {
class filepropertyinfo;
}

class filepropertyinfo : public QDialog
{
    Q_OBJECT

public:
    explicit filepropertyinfo(QWidget *parent = nullptr);
    ~filepropertyinfo();

    //设置内容
    void setInfo(FileInfo *info);

private:
    Ui::filepropertyinfo *ui;
};

#endif // FILEPROPERTYINFO_H
