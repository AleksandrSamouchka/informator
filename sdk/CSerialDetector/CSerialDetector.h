#pragma once

#include <QtCore/QObject>

class QTimer;

class CSerialDetector : public QObject
{
    Q_OBJECT
    Q_PROPERTY( QString portPath READ getPortPath NOTIFY portPathChanged )

public:
    explicit CSerialDetector( QObject * parent = nullptr );

    QString getPortPath();

    bool detect( const QString serialPortIdentifier );

private:
    static QString getSerialPortName( const quint16 vendorId,
                                      const quint16 productId );
    static QString getSerialPortName( const QString identifier );

signals:
    void portPathChanged();

private:
    QString m_portPath;
};
