#include <QDebug>

#include <QJsonArray>
#include <QJsonObject>

#include "CSystemConfigurationCanTopic.h"

static const char * FIELD_ID = "id";
static const char * FIELD_MASK = "mask";
static const char * FIELD_USE_MASK = "use_mask";
static const char * FIELD_ID_TYPE = "id_type";

static const char * FIELD_ID_FILTERS = "IdFilters";
static const char * FIELD_BAUDRATE = "baudrate";

CSystemConfigurationCanTopic::CSystemConfigurationCanTopic() :
    CSystemConfigurationTopic("CAN")
{
// m_canList.resize(CAN_COUNT); 
   initData();
}

CSystemConfigurationCanTopic::~CSystemConfigurationCanTopic()
{
}

void CSystemConfigurationCanTopic::initData()
{
    m_canList.clear();
    for(int i=0; i<CAN_COUNT; i++)
    {
        TCANSettings    s;
        s.baudrate = DEFAULT_CAN_BAUDRATE;
        for(int j=0; j<CAN_ID_FILTER_COUNT; j++)
        {
            s.IdFilter[j] = {EXT_CAN_ID, 0, 0, 0};
        }
        m_canList.append(s);
    }
}

#ifdef MNT_TOPICS_TEST
QByteArray CSystemConfigurationCanTopic::getEtalonJson()
{
    QJsonObject  a;
    for( int i = 0; i < CAN_COUNT; i ++ )
    {
        QString key = QString( "CAN_%1" ).arg( i+1 );
        QJsonObject o({ {FIELD_BAUDRATE, QJsonValue::fromVariant(200)} });
        QJsonObject of;
        QJsonArray  af;

        for( int j = 0; j < CAN_ID_FILTER_COUNT; j ++ )
        {
            QJsonObject of(
                {
                    { FIELD_ID_TYPE, EIDType::STD_CAN_ID },
                    { FIELD_ID, j },
                    { FIELD_USE_MASK, j },
                    { FIELD_MASK, j }
                }
            );

            af.push_back( of );
        }
        o[ FIELD_ID_FILTERS ] = af;
        a[ key ] = o;
    }
    return QJsonDocument(a).toJson();
}
#endif

void CSystemConfigurationCanTopic::setFilter(int8_t canId, int8_t filterId, const TCANIdFilter *filter)
{
    if(canId < 0 || canId >= CAN_COUNT) 
    {
qDebug() << "Invalid CAN id:" << canId;        
        return;
    }
    if(filterId < 0 || filterId >= CAN_ID_FILTER_COUNT) 
    {
qDebug() << "Invalid filter id:" << filterId;        
        return;
    }

//  TCANSettings &can = m_canList[canId];
    memcpy(&m_canList[canId].IdFilter[filterId], filter, sizeof(TCANIdFilter));

//    m_canList.replace(canId)
}

void CSystemConfigurationCanTopic::setBaudrate(int8_t canId, uint32_t baudrate)
{
    if(canId < 0 || canId >= CAN_COUNT)
    {
        qDebug() << "Invalid CAN id:" << canId;
        return;
    }
    m_canList[ canId ].baudrate = baudrate;
}

const QByteArray CSystemConfigurationCanTopic::prepareData() const
{
    QJsonObject  a;

    for(int i=0; i<CAN_COUNT; i++)
    {
        QString key = QString("CAN_%1").arg(i+1);
        const TCANSettings &s = m_canList[i];
        QJsonObject o({ { FIELD_BAUDRATE, QJsonValue::fromVariant(s.baudrate) } });
        QJsonObject of;
        QJsonArray  af;

//        o["baudrate"] = can.baudrate;

        for(int j=0; j<CAN_ID_FILTER_COUNT; j++)
        {
            QJsonObject of(
                {
                    { FIELD_ID_TYPE, s.IdFilter[j].IdType },
                    { FIELD_ID, QJsonValue::fromVariant(s.IdFilter[j].Id) },
                    { FIELD_USE_MASK, s.IdFilter[j].UseMask },
                    { FIELD_MASK, QJsonValue::fromVariant(s.IdFilter[j].Mask) }
                }
            );
/*
            of["id_type"] = s.IdFilter[j].IdType;
            of["id"] = s.IdFilter[j].Id;
            of["use_mask"] = s.IdFilter[j].UseMask;
            of["mask"] = s.IdFilter[j].Mask;
*/
            af.push_back(of);
        }
        o[ FIELD_ID_FILTERS ] = af;

        a[ key ] = o;
    }

    return QJsonDocument(a).toJson();
}

bool CSystemConfigurationCanTopic::parseData(const QByteArray &data)
{
    bool res{ false };

    const QJsonObject jsonMainObj( QJsonDocument::fromJson( data ).object() );
    if ( ! jsonMainObj.isEmpty() )
    {
        for ( int i = 0; i < CAN_COUNT; i ++ )
        {
            if ( jsonMainObj.contains( QString("CAN_%1").arg( i+1 )) )
            {

                QJsonObject jsonObj = jsonMainObj.value( QString("CAN_%1").arg( i+1 ) ).toObject();
                if ( jsonObj.contains( FIELD_ID_FILTERS ) &&
                     jsonObj.contains( FIELD_BAUDRATE ))
                {
                    m_canList[i].baudrate = static_cast< uint32_t >( jsonObj.value( FIELD_BAUDRATE ).toInt() );
                    TCANIdFilter idFilter[ CAN_ID_FILTER_COUNT ];

                    const QJsonArray jsonArrIdFilters = jsonObj.value( FIELD_ID_FILTERS ).toArray();
                    for ( int j = 0; j < CAN_ID_FILTER_COUNT; j ++ )
                    {
                        const QJsonObject jsonObjIdFilters = jsonArrIdFilters[j].toObject();
                        if ( jsonObjIdFilters.contains( FIELD_ID ) &&
                             jsonObjIdFilters.contains( FIELD_ID_TYPE ) &&
                             jsonObjIdFilters.contains( FIELD_MASK ) &&
                             jsonObjIdFilters.contains( FIELD_USE_MASK ))
                        {
                            idFilter[j].Id = static_cast< uint32_t >( jsonObjIdFilters.value( FIELD_ID ).toInt() );
                            idFilter[j].Mask = static_cast< uint32_t >( jsonObjIdFilters.value( FIELD_MASK ).toInt() );
                            idFilter[j].UseMask = static_cast< uint8_t >( jsonObjIdFilters.value( FIELD_USE_MASK ).toInt() );
                            idFilter[j].IdType = static_cast< EIDType >( jsonObjIdFilters.value( FIELD_ID_TYPE ).toInt() );
                            m_canList[i].IdFilter[j] = idFilter[j];
                        }
                        else
                        {
                            PRINT_CONSOLE_MESSAGE_WARN( "Внутренний JSON объект содержит неизвестный список полей: " + jsonObjIdFilters.keys().join( "," ) );
                        }
                    }
                }
                else
                {
                    PRINT_CONSOLE_MESSAGE_WARN( "Внутренний JSON объект содержит неизвестный список полей: " + jsonObj.keys().join( "," ) );
                }
                res = true;
            }
            else
            {
                printUnknownFieldsListMessage(jsonMainObj.keys());
            }
        }
    }
    else
    {
        printEmptyJsonMessage();
    }

    return setValidState( res );
}
