#include "CGrSystemHealthTopic.h"

static const char * PREFIX = "HEALTH/";

CGrSystemHealthTopic::CGrSystemHealthTopic(const QString &subtopicName):
    CSystemTopic( PREFIX + subtopicName )
{ }

CGrSystemHealthTopic::~CGrSystemHealthTopic()
{ }
