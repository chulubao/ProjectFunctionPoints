#include "rapidXmlTool.h"
#include "../common/commoninfo.h"
/*下面三个文件是本段代码需要的库文件*/
#include "../rapidXml/rapidxml.hpp"
#include "../rapidXml/rapidxml_utils.hpp"
#include "../rapidXml/rapidxml_print.hpp"
using namespace nsCommonInfo;

void createXml(string fromText,string toText,string typeText,\
        string elemNameText,string elemAttributeText,string elemContenText,string &xmlMsg) {
        rapidxml::xml_document<> doc;
        rapidxml::xml_node<> *root = doc.allocate_node(rapidxml::node_pi,
                                                       doc.allocate_string("xml version='1.0' encoding='utf-8'"));
        doc.append_node(root);
        rapidxml::xml_node<> *node = doc.allocate_node(rapidxml::node_element, "Message", NULL);
        doc.append_node(node);
        rapidxml::xml_node<> *header = doc.allocate_node(rapidxml::node_element, "Header", NULL);
        node->append_node(header);

        char agentid[128] = {0};
        getAgentID(agentid);
        header->append_node(doc.allocate_node(rapidxml::node_element, "ID", agentid));
        header->append_node(doc.allocate_node(rapidxml::node_element, "From", fromText.c_str()));
        header->append_node(doc.allocate_node(rapidxml::node_element, "To", toText.c_str()));
        header->append_node(doc.allocate_node(rapidxml::node_element, "Type", typeText.c_str()));
        char gentime[64] = {0};
        get_current_date(gentime);
        header->append_node(doc.allocate_node(rapidxml::node_element, "GenTime", gentime));
        header->append_node(doc.allocate_node(rapidxml::node_element, "SendTime", gentime));

        rapidxml::xml_node<> *body = doc.allocate_node(rapidxml::node_element, "Body", NULL);
        node->append_node(body);
        rapidxml::xml_node<> *childNode = doc.allocate_node(rapidxml::node_element, elemNameText.c_str(), elemContenText.c_str());
        childNode->append_attribute(doc.allocate_attribute("Type",elemAttributeText.c_str()));
        body->append_node(childNode);

        /*body->append_node(doc.allocate_node(rapidxml::node_element, "Status", statusText.c_str()));*/

        string text;
        rapidxml::print(std::back_inserter(text), doc, 0);
        cout << text << std::endl;
        xmlMsg = text.substr(0, text.length() - 2);
        /*ofstream outFile("../config1.xml");
        outFile << doc;*/
    }


/*参考链接:https://blog.csdn.net/v_xchen_v/article/details/75634273*/
void parseXml(string xmlMsg) {
        /*
         rapidxml::file<> fdoc("../config/config2.xml");
         cout << fdoc.data() << std::endl;
         */

        rapidxml::xml_document<> doc;
        doc.parse<0>(const_cast<char *>(xmlMsg.data()));
        /*cout <<"[1]"<<doc.name() << endl;*/
        //! 获取根节点
        rapidxml::xml_node<> *root = doc.first_node();
        //! 获取根节点第一个节点
        rapidxml::xml_node<> *header = root->first_node();
        rapidxml::xml_node<> *headerChild1 = header->first_node();
        /*cout << headerChild1->name() << "\t" << headerChild1->value() << endl;*/
        rapidxml::xml_node<> *body = header->next_sibling();
         rapidxml::xml_node<> *node_first = body->first_node();
/*
        cout<<"first node of root:"<<endl<<*node_first<<endl;
        rapidxml::xml_node<> *node_last = body->last_node();
        cout<<"last node of root:"<<endl<<*node_last<<endl;

        rapidxml::xml_attribute<> *attr;
        attr = node_first->first_attribute();
        cout<<"attr_name:"<<attr->name()<<endl;
        cout<<"attr_value:"<<attr->value()<<endl;
*/

        for(rapidxml::xml_node<> * childNode_first = node_first->first_node("UserName");childNode_first!=NULL;childNode_first = childNode_first->next_sibling())
        {
                cout<<"sib:"<<*childNode_first;
                cout<<"sib name:"<<childNode_first->name()<<endl;
                cout<<"sib value:"<<childNode_first->value()<<endl<<endl;
        }



}
