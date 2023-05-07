#ifndef __CMESSAGE_H__
#define __CMESSAGE_H__

#include <QString>

class CMessage
{
    public:
    /*! перечисление типов сообщений */
        enum class ELogCriteria
        {
            LogCriteria_error = 0,
            LogCriteria_warning,
            LogCriteria_info,
            /**/
            LogCriteria_last
        };


        CMessage();
        CMessage( const QString &module,
                 const QString &message,
                 const ELogCriteria &criteria );
        virtual ~CMessage();
#if 0
        virtual void setModule( const QString &module );
        virtual void setCriteria( const ELogCriteria &criteria );
        virtual void setMessage( const QString &message );
#endif
        virtual QString getModule() const;
//        virtual QString getName() const;
        virtual QString getMessage() const;
        virtual ELogCriteria getCriteria() const;
        virtual const char *getCriteriaString() const;
        static const char *getCriteriaString(ELogCriteria index);
        virtual QJsonObject getJsonObject() const;
        virtual QString getJsonString() const;

    protected:
        void init(const QString &module,
                 const QString &message,
                 const ELogCriteria &criteria);

    private:
        QString m_module;
        ELogCriteria m_criteria;
        QString m_message;
};


#endif