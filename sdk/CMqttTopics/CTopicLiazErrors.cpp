#include "CTopicLiazErrors.h"

    static const char *FIELD_DTCO_DISCONNECT = "DTCODisconErr";
    static const char *FIELD_DTCO_YELLOW = "DTCOYellowErr";
    static const char *FIELD_DTCO_RED = "DTCORedErr";
    static const char *FIELD_LOWCOOLANT = "LowCoolantErr";
    static const char *FIELD_ECAS = "ECASErr";
    static const char *FIELD_TRANSMISSION = "TransmissionErr";
    static const char *FIELD_CPC = "CPCErr";
    static const char *FIELD_SCR = "SCRErr";
    static const char *FIELD_EBSS = "EBSSErr";
    static const char *FIELD_EC = "ECErr";
    static const char *FIELD_VCU = "VCUErr";
    static const char *FIELD_CBCU = "CBCUErr";
    static const char *FIELD_ABS_YELLOW = "ABSYellowErr";
    static const char *FIELD_CHECKENGINE_YELLOW = "CheckEngineYellowErr";
    static const char *FIELD_CHECKENGINE_RED = "CheckEngineRedErr";

/** ***********************************************************************
 * @brief Конструктор
 ** ***********************************************************************/
CTopicLiazErrors::CTopicLiazErrors(const QByteArray &json) :
    CTopicValueEntry( json, ETopicsNamesIndex::Topic_Liaz_errors )
{ }

CTopicLiazErrors::CTopicLiazErrors() :
    CTopicValueEntry( QByteArray{}, ETopicsNamesIndex::Topic_Liaz_errors )
{ }

/** ***********************************************************************
 * @fn initFromData
 * @brief метод инициализации полей из json (используется при приведении типов)
 * @param none
 * @return bool
 ** ***********************************************************************/
bool CTopicLiazErrors::initFromData( const CTopicValueEntry &value )
{
    bool res{ false };
    const QJsonObject jsonMainObj( QJsonDocument::fromJson( value.getData() ).object() );
    if ( ! jsonMainObj.isEmpty() )
    {
        if ( jsonMainObj.contains( FIELD_DTCO_DISCONNECT ) &&
             jsonMainObj.contains( FIELD_DTCO_YELLOW ) &&
             jsonMainObj.contains( FIELD_DTCO_RED ) &&
             jsonMainObj.contains( FIELD_LOWCOOLANT ) &&
             jsonMainObj.contains( FIELD_ECAS ) &&
             jsonMainObj.contains( FIELD_TRANSMISSION ) &&
             jsonMainObj.contains( FIELD_CPC ) &&
             jsonMainObj.contains( FIELD_SCR ) &&
             jsonMainObj.contains( FIELD_EBSS ) &&
             jsonMainObj.contains( FIELD_EC ) &&
             jsonMainObj.contains( FIELD_VCU ) &&
             jsonMainObj.contains( FIELD_CBCU ) &&
             jsonMainObj.contains( FIELD_ABS_YELLOW ) &&
             jsonMainObj.contains( FIELD_CHECKENGINE_YELLOW ) &&
             jsonMainObj.contains( FIELD_CHECKENGINE_RED ) )
        {
            m_dtcoDisconnect = jsonMainObj.value( FIELD_DTCO_DISCONNECT ).toInt( 0 );
            m_dtcoYellow = jsonMainObj.value( FIELD_DTCO_YELLOW ).toInt( 0 );
            m_dtcoRed = jsonMainObj.value( FIELD_DTCO_RED ).toInt( 0 );
            m_lowCoolant = jsonMainObj.value( FIELD_LOWCOOLANT ).toInt( 0 );
            m_ecas = jsonMainObj.value( FIELD_ECAS ).toInt( 0 );
            m_transmission = jsonMainObj.value( FIELD_TRANSMISSION ).toInt( 0 );
            m_cpc = jsonMainObj.value( FIELD_CPC ).toInt( 0 );
            m_scr = jsonMainObj.value( FIELD_SCR ).toInt( 0 );
            m_ebss = jsonMainObj.value( FIELD_EBSS ).toInt( 0 );
            m_ec = jsonMainObj.value( FIELD_EC ).toInt( 0 );
            m_vcu = jsonMainObj.value( FIELD_VCU ).toInt( 0 );
            m_cbcu = jsonMainObj.value( FIELD_CBCU ).toInt( 0 );
            m_absYellow = jsonMainObj.value( FIELD_ABS_YELLOW ).toInt( 0 );
            m_checkYellow = jsonMainObj.value( FIELD_CHECKENGINE_YELLOW ).toInt( 0 );
            m_checkRed = jsonMainObj.value( FIELD_CHECKENGINE_RED ).toInt( 0 );
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
    setValidState( res );
    return res;
}

/** ***********************************************************************
* @fn prepareData
* @brief метод формирования значения топика из полей структуры
* @param none
* @return QByteArray
** ***********************************************************************/
QByteArray CTopicLiazErrors::prepareData() const
{
    QJsonObject jsonMainObj{};
    jsonMainObj.insert( FIELD_DTCO_DISCONNECT, m_dtcoDisconnect );
    jsonMainObj.insert( FIELD_DTCO_YELLOW, m_dtcoYellow );
    jsonMainObj.insert( FIELD_DTCO_RED, m_dtcoRed );
    jsonMainObj.insert( FIELD_LOWCOOLANT, m_lowCoolant );
    jsonMainObj.insert( FIELD_ECAS, m_ecas );
    jsonMainObj.insert( FIELD_TRANSMISSION, m_transmission );
    jsonMainObj.insert( FIELD_CPC, m_cpc );
    jsonMainObj.insert( FIELD_SCR, m_scr );
    jsonMainObj.insert( FIELD_EBSS, m_ebss );
    jsonMainObj.insert( FIELD_EC, m_ec );
    jsonMainObj.insert( FIELD_VCU, m_vcu );
    jsonMainObj.insert( FIELD_CBCU, m_cbcu );
    jsonMainObj.insert( FIELD_ABS_YELLOW, m_absYellow );
    jsonMainObj.insert( FIELD_CHECKENGINE_YELLOW, m_checkYellow );
    jsonMainObj.insert( FIELD_CHECKENGINE_RED, m_checkRed );
    const QByteArray jsonData{ QJsonDocument( jsonMainObj ).toJson( QJsonDocument::Compact ) };
    return jsonData;
}

/** ***********************************************************************
* @fn operator ==
** ***********************************************************************/
bool CTopicLiazErrors::operator == ( const CTopicLiazErrors & right ) const
{
    if ( this->m_dtcoDisconnect == right.m_dtcoDisconnect
         && this->m_dtcoYellow == right.m_dtcoYellow
         && this->m_dtcoRed == right.m_dtcoRed
         && this->m_lowCoolant == right.m_lowCoolant
         && this->m_ecas == right.m_ecas
         && this->m_transmission == right.m_transmission
         && this->m_cpc == right.m_cpc
         && this->m_scr == right.m_scr
         && this->m_ebss == right.m_ebss
         && this->m_ec == right.m_ec
         && this->m_vcu == right.m_vcu
         && this->m_cbcu == right.m_cbcu
         && this->m_absYellow == right.m_absYellow
         && this->m_checkYellow == right.m_checkYellow
         && this->m_checkRed == right.m_checkRed )
    {
        return true;
    }
    return false;
}

/** ***********************************************************************
* @fn operator !=
** ***********************************************************************/
bool CTopicLiazErrors::operator !=(const CTopicLiazErrors & right) const
{
    if ( this->m_dtcoDisconnect == right.m_dtcoDisconnect
         && this->m_dtcoYellow == right.m_dtcoYellow
         && this->m_dtcoRed == right.m_dtcoRed
         && this->m_lowCoolant == right.m_lowCoolant
         && this->m_ecas == right.m_ecas
         && this->m_transmission == right.m_transmission
         && this->m_cpc == right.m_cpc
         && this->m_scr == right.m_scr
         && this->m_ebss == right.m_ebss
         && this->m_ec == right.m_ec
         && this->m_vcu == right.m_vcu
         && this->m_cbcu == right.m_cbcu
         && this->m_absYellow == right.m_absYellow
         && this->m_checkYellow == right.m_checkYellow
         && this->m_checkRed == right.m_checkRed )
    {
        return false;
    }
    return true;
}




