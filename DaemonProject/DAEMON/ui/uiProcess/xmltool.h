#ifndef XMLTOOL_H
#define XMLTOOL_H
#include <QString>
extern void createXml(QString fromText,QString toText,QString typeText,\
               QString elemName,QString elemAttributeText,QString userRoleText,QString userNameText,QString passwordText,\
               QString &outResult);
extern bool  parseXmlMsg(QString  xmlMsg);
#endif // XMLTOOL_H
