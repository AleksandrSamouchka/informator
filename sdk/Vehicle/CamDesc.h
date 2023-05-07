#pragma once


#include <QString>
#include <QUrl>

struct CamDesc
{
    int index{-1};
    QString indexName() const {return QString().sprintf("Cam_%02d", index);}
    QString humanName;

    QUrl primaryUrl;
    QUrl secondaryUrl;
};

