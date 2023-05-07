#ifndef CGUIMESSAGE_H
#define CGUIMESSAGE_H

#include <CLoggerMessage/CLoggerMessage.h>

#include "CMessage.h"


#define CREATE_GUI_MESSAGE( name, message, criteria ) CGuiMessage( name, message, criteria )
#define CREATE_GUI_MESSAGE_INFO( name, message ) CREATE_GUI_MESSAGE( name, message, CLoggerMessage::LogCriteria_info )
#define CREATE_GUI_MESSAGE_WARN( name, message ) CREATE_GUI_MESSAGE( name, message, CLoggerMessage::LogCriteria_warning )
#define CREATE_GUI_MESSAGE_ERROR( name, message ) CREATE_GUI_MESSAGE( name, message, CLoggerMessage::LogCriteria_error )

/** ***********************************************************************
 * @class CGuiMessage
 * @brief Класс, описывает необходимые поля сообщения для отображения в GUI
 ** ***********************************************************************/
class CGuiMessage : public CMessage
{
//    QString m_module/* =QCoreApplication::applicationName()*/;
    QString m_name/* = QString{}*/;
//    QString m_message/* = QString{}*/;
//    CLoggerMessage::ELogCriteria m_criteria/* = CLoggerMessage::ELogCriteria::LogCriteria_info*/;

public slots:
//    void setModule( const QString &module );
//    void setName( const QString &name );
//    void setCriteria( const CLoggerMessage::ELogCriteria &criteria );
//    void setMessage( const QString &message );

public:
    CGuiMessage()/* = default*/;
//    CGuiMessage( const CGuiMessage & other ) = default;
    CGuiMessage( const QString &module,
                 const QString &name,
                 const QString &message,
                 const CMessage::ELogCriteria &criteria );
    CGuiMessage( const QString &name,
                 const QString &message,
                 const CMessage::ELogCriteria &criteria );

    virtual ~CGuiMessage() {}

//    QString getModule() const;
    QString getName() const;
//    QString getMessage() const;
//    CLoggerMessage::ELogCriteria getCriteria() const;
//    QString getCriteriaString() const;

    virtual QString getJsonString() const;
};

#endif // CGUIMESSAGE_H
