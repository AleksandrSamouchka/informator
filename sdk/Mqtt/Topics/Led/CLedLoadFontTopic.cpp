#include "CLedLoadFontTopic.h"

static const char * TOPIC = "LOAD_FONT";

static const char *FIELD_TARGET = "target";
static const char *FIELD_FILE_NAME = "file_name";

/** ***********************************************************************
 * @brief Конструктор
 ** ***********************************************************************/
CLedLoadFontTopic::CLedLoadFontTopic() :
    CGrLedTopic( TOPIC )
{ }

CLedLoadFontTopic::CLedLoadFontTopic(CGrLedTopic::ETargets target, const QString & fileName) :
    CGrLedTopic( TOPIC ),
/*    m_target{ target },*/
    m_fileName{ fileName }
{ 
    setTarget(target);
}

CLedLoadFontTopic::CLedLoadFontTopic(const QByteArray & data) :
    CGrLedTopic( TOPIC, data )
{ }

/*void CLedLoadFontTopic::setTarget(const CGrLedTopic::ETargets value)
{
    m_target = value;
}
*/

void CLedLoadFontTopic::setFileName(const QString & value)
{
    m_fileName = value;
}

const QByteArray CLedLoadFontTopic::prepareData() const
{
    QJsonObject jsonObj{ { FIELD_TARGET, CGrLedTopic::getTargetName(getTarget()) },
                         { FIELD_FILE_NAME, m_fileName } };
    return QJsonDocument( jsonObj ).toJson( QJsonDocument::Compact );
}

bool CLedLoadFontTopic::parseData(const QByteArray & data)
{
    bool res{ false };
    QJsonObject jsonMainObj( QJsonDocument::fromJson( data ).object() );
    if ( ! jsonMainObj.isEmpty() )
    {
        if ( jsonMainObj.contains( FIELD_TARGET ) &&
             jsonMainObj.contains( FIELD_FILE_NAME ) )
        {
            /*m_target = */setTarget(CGrLedTopic::getTargetCode( jsonMainObj.value( FIELD_TARGET ).toString() ));
            m_fileName = jsonMainObj.value( FIELD_FILE_NAME ).toString();
            res = true;
        }
        else
        {
            printUnknownFieldsListMessage(jsonMainObj.keys());
        }
    }
    else
    {
        printEmptyJsonMessage();
    }
    return setValidState( res );
}
