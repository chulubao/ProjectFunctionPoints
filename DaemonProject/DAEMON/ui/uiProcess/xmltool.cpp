#include "xmltool.h"
#include "common.h"
#include <QDomDocument>
void createXml(QString fromText,QString toText,QString typeText,\
               QString elemName,QString elemAttributeText,QString userRoleText,QString userNameText,QString passwordText,\
               QString &outResult){
    /*QDomDocument doc("workMode");*/
    QDomDocument doc;
    QDomProcessingInstruction instruction;
    instruction = doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");
    doc.appendChild(instruction);

    QDomElement root = doc.createElement("Message");
    doc.appendChild(root);

    QDomElement header = doc.createElement("Header");
    root.appendChild(header);
    /*ID*/
    QDomElement id = doc.createElement("ID");
    header.appendChild(id);

    char agentid[128]={0};
    nsCommonInfo:: getAgentID(agentid);
    QDomText  idContent = doc.createTextNode(QString(agentid));
    id.appendChild(idContent);
    /*From*/
    QDomElement from = doc.createElement("From");
    header.appendChild(from);
    QDomText  fromContent = doc.createTextNode(fromText);
    from.appendChild(fromContent);
    /*To*/
    QDomElement to = doc.createElement("To");
    header.appendChild(to);
    QDomText  toContent = doc.createTextNode(toText);
    to.appendChild(toContent);
    /*Type*/
    QDomElement type = doc.createElement("Type");
    header.appendChild(type);
    QDomText  typeContent = doc.createTextNode(typeText);
    type.appendChild(typeContent);

    char gentime[64] = {0};
    nsCommonInfo::get_current_date(gentime);
    /*GenTime*/
    QDomElement genTime = doc.createElement("GenTime");
    header.appendChild(genTime);
    QDomText  genTimeContent = doc.createTextNode(QString(gentime));
    genTime.appendChild(genTimeContent);

    /*SendTime*/
    QDomElement sendTime = doc.createElement("SendTime");
    header.appendChild(sendTime);
    QDomText  sendTimeContent = doc.createTextNode(QString(gentime));
    sendTime.appendChild(sendTimeContent);

    QDomElement body = doc.createElement("Body");
    root.appendChild(body);

    /*Account*/
    QDomElement account = doc.createElement(elemName);
    account.setAttribute(QString::fromUtf8("Type"),elemAttributeText);
    body.appendChild(account);


    /*user name*/
    QDomElement userName = doc.createElement("UserName");
    account.appendChild(userName);
    QDomText userNameContent = doc.createTextNode(userNameText);
    userName.appendChild(userNameContent);

    /*user Role*/
    QDomElement  userRole = doc.createElement("UserRole");
    account.appendChild(userRole);
    QDomText userRoleContent = doc.createTextNode(userRoleText);
    userRole.appendChild(userRoleContent);

    /*password*/
    QDomElement  password= doc.createElement("Password");
    account.appendChild(password);
    QDomText  passwordContent = doc.createTextNode(passwordText);
    password.appendChild(passwordContent);
    QString  xlmContent=doc.toString();
    outResult=xlmContent.left(xlmContent.length()-1);
    return;
}


bool  parseXmlMsg(QString  xmlMsg){

    QDomDocument  doc;
    if(!doc.setContent(xmlMsg))
    {
       logdebug(LOG_ERR,LOG_PRINT_UI_FILE,"file:%s,line:%d: xmlMsg error!\n",__FILE__,__LINE__);
        return false ;
    }

    //返回根元素
    QDomElement root=doc.documentElement();
    if(!root.isElement() || root.isNull())
    {
       logdebug(LOG_ERR,LOG_PRINT_UI_FILE,"file:%s,line:%d: root.isElement() || root.isNull()!\n",__FILE__,__LINE__);
        return  false ;
    }

    QDomElement  header= root.firstChildElement();
    QDomElement  body= header.nextSiblingElement();
    QDomElement  account= body.firstChildElement();
    if(QString("0")==account.text()){
        return true ;
    }

    return false ;
}
