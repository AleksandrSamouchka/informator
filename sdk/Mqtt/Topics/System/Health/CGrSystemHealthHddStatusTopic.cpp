#include "CGrSystemHealthHddStatusTopic.h"

static const char * PREFIX = "HDD_STATUS/";

CGrSystemHealthHddStatusTopic::CGrSystemHealthHddStatusTopic( const QString & subtopicName ):
    CGrSystemHealthTopic( PREFIX + subtopicName )
{ }

CGrSystemHealthHddStatusTopic::~CGrSystemHealthHddStatusTopic()
{ }
