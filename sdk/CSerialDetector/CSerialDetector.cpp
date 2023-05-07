#include <QtCore/QTimer>
#include <QtCore/QThread>
#include <QtCore/QMetaMethod>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <functional>

#include <CUtils/CUtils.h>
#include <CConsoleLogger/CConsoleLogger.h>

#include "CSerialDetector.h"

CSerialDetector::CSerialDetector( QObject * parent ) :
    QObject{ parent }
{
}

QString CSerialDetector::getPortPath()
{
    return m_portPath;
}


bool CSerialDetector::detect( const QString serialPortIdentifier )
{
    auto result{ false };
    auto timer{ qobject_cast<QTimer*>( sender() ) };
    QSerialPort serialPort{ getSerialPortName( serialPortIdentifier ) };

    if ( ! serialPort.portName().isEmpty() &&
         serialPort.open( QIODevice::ReadOnly ) )
    {
        serialPort.close();
        m_portPath = QSerialPortInfo{ serialPort }.systemLocation();
        result = true;

        if ( timer != nullptr )
        {
            timer->stop();
            timer->deleteLater();
        }
        emit portPathChanged();
    }
    else
    {
        if ( timer == nullptr )
        {
            timer = new QTimer{ this };
            connect( timer, & QTimer::timeout, timer, std::bind( & CSerialDetector::detect, this, serialPortIdentifier ) );
            timer->setSingleShot( true );
        }
        timer->start( CUtils::waitTimeout() );
    }

    return result;
}

QString CSerialDetector::getSerialPortName( const quint16 vendorId,
                                            const quint16 productId )
{
    for ( const auto info : QSerialPortInfo::availablePorts() )
    {
        PRINT_CONSOLE_MESSAGE_INFO( QString( "Serial port: %1 (%2:%3)" )
                                    .arg(info.portName())
                                    .arg(QString::number( info.vendorIdentifier(), 16 ))
                                    .arg(QString::number( info.productIdentifier(), 16 )) );
        if ( info.vendorIdentifier() == vendorId &&
             info.productIdentifier() == productId )
        {
            return info.portName();
        }
    }
    return QString{};
}

QString CSerialDetector::getSerialPortName( const QString identifier )
{
    const auto vp{ identifier.split( QLatin1Char{ ':' } ) };

    if ( vp.size() == 2 )
    {
        return getSerialPortName( vp.first().toInt( nullptr, 16 ), vp.last().toInt( nullptr, 16 ) );
    }

    return QString{};
}

