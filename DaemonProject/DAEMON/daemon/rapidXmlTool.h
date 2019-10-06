#ifndef COMMUNICATION_RAPIDXMLTOOL_H
#define COMMUNICATION_RAPIDXMLTOOL_H
#include <iostream>
using namespace std;
extern void createXml(string fromText,string toText,string typeText,\
        string elemNameText,string elemAttributeText,string elemContenText,string &xmlMsg) ;

extern void parseXml(string xmlMsg);
#endif //COMMUNICATION_RAPIDXMLTOOL_H

