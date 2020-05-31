#include <iostream>
#include <cmath>
#include <cstdio>
#include <sstream>
#include <cstring>
#include <algorithm>
#include <vector>


#ifdef _MSC_VER
#include "iso646.h"          // So "and" is equivalenced to &&
typedef unsigned int uint;   // Define uint to be unsigned int
#endif

//
//******************************************************************************
//                 XML_ParameterListArray.h
//******************************************************************************
//
//********************************************************************************
//               Chris Anderson (C) UCLA  May 30, 2011
//********************************************************************************
//
// When initialized from an existing xml file, a non-explcitly typed value
// in the set of any case version of  ["true","yes","false","no"] are
// aliased as boolean values. To specify the actual string values of values
// in this set, one must specify the type explicitly as string.
//
// When initializing within a program, or setting an existing parameter,
// the automatic conversion from the set of any case version of
// ["true","yes",0,"false","no",1] also occur.
// To specify boolean one specifies the value with a bool
// variable.
//
// When evaluating parameter instance counts, if the specified parameterList
// doesn't exist, then the instance count returned is 0 and an error
// indicator is not set. This decision was made to avoid the need to specify
// empty parameter lists to avoid errors being thrown. Accessing an instance
// value of a parameter list that doesn't exist will induce the error flag to
// be set.
//
//
#ifndef XML_PARAMETER_LIST_ARRAY_
#define XML_PARAMETER_LIST_ARRAY_

#ifndef TIXML_USE_STL
#define TIXML_USE_STL
#endif

#include "XML_dataType.h"
#include "tinyxml.h"

class XML_ParameterListArray
{

public:

	XML_ParameterListArray()
	{
		parameterArrayDocPtr = 0;
		abortOnErrorFlag     = true;
		initialize();
	}

	XML_ParameterListArray(const XML_ParameterListArray& P)
	{
		parameterArrayDocPtr = 0;
		abortOnErrorFlag     = true;
		initialize(P);
	}


    XML_ParameterListArray(const std::string& fileName)
    {
        parameterArrayDocPtr = 0;
		abortOnErrorFlag     = true;
		initialize(fileName.c_str());
    }

	XML_ParameterListArray(const char* fileName)
	{
		parameterArrayDocPtr = 0;
		abortOnErrorFlag     = true;
		initialize(fileName);
	}

	~XML_ParameterListArray()
	{
	destroyData();
	}

	bool isNull() const
	{
	if(parameterArrayDocPtr == 0) return true;
	return false;
	}

	void abortIfNull() const
	{
    XML_ParameterListArray* Eptr =  const_cast<XML_ParameterListArray*> (this);

	if(isNull())
	{
	    Eptr->errorFlag = true;
	    Eptr->errorMessage.append("XXX XML_ParameterListArray programming error. XXXX \n");
		Eptr->errorMessage.append("XML_ParameterListArray is not properly initialized.\n");
		Eptr->errorMessage.append("Use createParameterListArray(*) to initialize.\n");
		Eptr->errorMessage.append("XXX Program Terminated XXX");
		Eptr->errorMessage.append("\n");
	}
	checkErrorAndAbort();
	}


////////////////////////////////////////////////////////////////////////
//
// For internal construction and manipulation of ParameterListArray
//
//
// To Do : Add parameter range checks. Write up documentation
//
////////////////////////////////////////////////////////////////////////

    void createParameterListArray(const std::string& listArrayName)
    {
       createParameterListArray(listArrayName.c_str());
    }

    void createParameterListArray(const char* listArrayName)
    {
    //
    // Set up header
    //

    std::string listFileName;
    listFileName.assign(listArrayName);
    listFileName.append(".xml");
    initialize();
    parameterArrayDocPtr           = new TiXmlDocument(listFileName.c_str());
    TiXmlDeclaration * declaration = new TiXmlDeclaration( "1.0", "", "" );
	TiXmlComment * comment         = new TiXmlComment();
	comment->SetValue(" XML_ParameterListArray ");


	parameterArrayDocPtr->LinkEndChild( declaration );
	parameterArrayDocPtr->LinkEndChild( comment );

    TiXmlElement * root = new TiXmlElement(listArrayName);
	parameterArrayDocPtr->LinkEndChild( root );

	abortOnErrorFlag     =  true;
    }

    const char* getParameterListArrayName() const
    {
    TiXmlHandle  docHandle(parameterArrayDocPtr->RootElement());
	TiXmlElement* root = docHandle.ToElement();
	return root->Value();
    }


    // Creates a duplicate

    void operator=(const XML_ParameterListArray& P)
    {
    initialize(P);
    }

    //
    // Check to make sure parameterListName doesn't exist more than once
    //

    void addParameterList(const std::string& parameterListName)
    {
    addParameterList(parameterListName.c_str());
    }

    void addParameterList(const char* parameterListName)
    {
    abortIfNull();
    if(parameterListInstanceCount(parameterListName) != 0)
    {
    errorFlag = true;
	errorMessage.append("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
	errorMessage.append("XML_ParameterListArray Class Error \n");
	errorMessage.append("void addParameterList(const char* parameterListName)\n\n");
	errorMessage.append("Adding a parameterList that already exists.\n");
	errorMessage.append("Multiple parameterLists in a single\nparameterListArray is not supported.\n\n");
	errorMessage.append("Offending parameterListName : ");
	errorMessage.append(parameterListName);
	errorMessage.append("\nXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
	errorMessage.append("\n");
    }
    if(abortOnErrorFlag){checkErrorAndAbort();}

    TiXmlHandle  docHandle(parameterArrayDocPtr->RootElement());
	TiXmlElement* root = docHandle.ToElement();
    TiXmlElement* parameterListElement = new TiXmlElement(parameterListName);
    root->LinkEndChild( parameterListElement );
    }


    void addParameter(const std::string&  parameterName, const std::string& parameterListName)
    {
    addParameter(parameterName.c_str(), parameterListName.c_str());
    }

    void addParameter(const char* parameterName, const char* parameterListName)
    {
    abortIfNull();
    if(isParameterList(parameterListName) == 0)
    {
    errorFlag = true;
	errorMessage.append("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
	errorMessage.append("XML_ParameterListArray Class Error \n");
	errorMessage.append("void addParameter(const char* parameterName, const char* parameterListName)\n\n");
	errorMessage.append("ParameterList specified parameterListName does not exist.\n\n");
	errorMessage.append("Offending parameterListName : ");
	errorMessage.append(parameterListName);
	errorMessage.append("\nXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
	errorMessage.append("\n");
    }
    if(abortOnErrorFlag){checkErrorAndAbort();}

    TiXmlHandle  docHandle(parameterArrayDocPtr->RootElement());
	TiXmlElement* parameterListElement    = docHandle.FirstChild(parameterListName).ToElement();
	TiXmlElement* parameterElement        = new TiXmlElement(parameterName);
    parameterListElement->LinkEndChild( parameterElement );
    }


    void addParameterNoTypeSpec(const std::string& value, const std::string& parameterName, const std::string& parameterListName)
    {
    addParameterNoTypeSpec(value.c_str(),parameterName.c_str(), parameterListName.c_str());
    }

    void addParameterNoTypeSpec(const char* value, const char* parameterName, const char* parameterListName)
    {
    abortIfNull();
    if(isParameterList(parameterListName) == 0)
    {
    errorFlag = true;
	errorMessage.append("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
	errorMessage.append("XML_ParameterListArray Class Error \n");
	errorMessage.append("void addParameter(XML_dataType value, const char* parameterName, const char* parameterListName)\n\n");
	errorMessage.append("ParameterList specified parameterListName does not exist.\n\n");
	errorMessage.append("Offending parameterListName : ");
	errorMessage.append(parameterListName);
	errorMessage.append("\nXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
	errorMessage.append("\n");
    }
    if(abortOnErrorFlag){checkErrorAndAbort();}

    TiXmlHandle  docHandle(parameterArrayDocPtr->RootElement());
	TiXmlElement* parameterListElement    = docHandle.FirstChild(parameterListName).ToElement();
	TiXmlElement* parameterElement        = new TiXmlElement(parameterName);
    parameterListElement->LinkEndChild( parameterElement );

    parameterElement->SetAttribute("value",value);
    }


    void addParameter(XML_dataType value, const std::string& parameterName, const std::string& parameterListName)
    {
        addParameter(value, parameterName.c_str(), parameterListName.c_str());
    }


    void addParameter(XML_dataType value, const char* parameterName, const char* parameterListName)
    {
    abortIfNull();
    if(isParameterList(parameterListName) == 0)
    {
    errorFlag = true;
	errorMessage.append("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
	errorMessage.append("XML_ParameterListArray Class Error \n");
	errorMessage.append("void addParameter(XML_dataType value, const char* parameterName, const char* parameterListName)\n\n");
	errorMessage.append("ParameterList specified parameterListName does not exist.\n\n");
	errorMessage.append("Offending parameterListName : ");
	errorMessage.append(parameterListName);
	errorMessage.append("\nXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
	errorMessage.append("\n");
    }
    if(abortOnErrorFlag){checkErrorAndAbort();}

    TiXmlHandle  docHandle(parameterArrayDocPtr->RootElement());
	TiXmlElement* parameterListElement    = docHandle.FirstChild(parameterListName).ToElement();
	TiXmlElement* parameterElement        = new TiXmlElement(parameterName);
    parameterListElement->LinkEndChild( parameterElement );

    parameterElement->SetAttribute("value", value.toString().c_str());
    if(value.isString()){parameterElement->SetAttribute("type","string");}
    else
    {parameterElement->SetAttribute("type",getDataType(value.toString().c_str()));}
    }

    void addParameterChild(const std::string& parameterChildName, const std::string& parameterName, const std::string& parameterListName)
    {
        addParameterChild(parameterChildName.c_str(),parameterName.c_str(), parameterListName.c_str());
    }

    void addParameterChild(const char* parameterChildName, const char* parameterName, const char* parameterListName)
    {
    abortIfNull();
    if(isParameterList(parameterListName) == 0)
    {
    errorFlag = true;
	errorMessage.append("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
	errorMessage.append("XML_ParameterListArray Class Error \n");
	errorMessage.append("void addParameterChild(const char* parameterChildName, const char* parameterName, const char* parameterListName)\n\n");
	errorMessage.append("ParameterList specified parameterListName does not exist.\n\n");
	errorMessage.append("Offending parameterListName : ");
	errorMessage.append(parameterListName);
	errorMessage.append("\nXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
	errorMessage.append("\n");
    }
    if(abortOnErrorFlag){checkErrorAndAbort();}

    if(isParameter(parameterName,parameterListName) == 0)
    {
    errorFlag = true;
	errorMessage.append("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
	errorMessage.append("XML_ParameterListArray Class Error \n");
	errorMessage.append("void addParameterChild(const char* parameterChildName, const char* parameterName, const char* parameterListName)\n\n");
	errorMessage.append("Parameter specified by parameterName does not exist.\n\n");
	errorMessage.append("ParameterName : ");
	errorMessage.append(parameterName);
	errorMessage.append("\nXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
	errorMessage.append("\n");
    }
    if(abortOnErrorFlag){checkErrorAndAbort();}

    TiXmlHandle  docHandle(parameterArrayDocPtr->RootElement());
	TiXmlElement* parameter             = docHandle.FirstChild(parameterListName).FirstChild(parameterName).ToElement();
	TiXmlElement* parameterChild     = new TiXmlElement(parameterChildName);
    parameter->LinkEndChild( parameterChild );
    }

    void addParameterChild(XML_dataType value, const std::string& parameterChildName,
    const std::string& parameterName, const std::string& parameterListName)
    {
    addParameterChild(value, parameterChildName.c_str(),parameterName.c_str(), parameterListName.c_str());
    }

    void addParameterChild(XML_dataType value, const char* parameterChildName,
    const char* parameterName, const char* parameterListName)
    {
    abortIfNull();
    if(isParameterList(parameterListName) == 0)
    {
    errorFlag = true;
	errorMessage.append("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
	errorMessage.append("XML_ParameterListArray Class Error \n");
	errorMessage.append("void addParameterChild(XML_dataType value, const char* parameterChildName,\n");
    errorMessage.append("const char* parameterName, const char* parameterListName)\n\n");
	errorMessage.append("ParameterList specified parameterListName does not exist.\n\n");
	errorMessage.append("Offending parameterListName : ");
	errorMessage.append(parameterListName);
	errorMessage.append("\nXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
	errorMessage.append("\n");
    }
    if(abortOnErrorFlag){checkErrorAndAbort();}

    if(isParameter(parameterName,parameterListName) == 0)
    {
    errorFlag = true;
	errorMessage.append("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
	errorMessage.append("XML_ParameterListArray Class Error \n");
	errorMessage.append("void addParameterChild(XML_dataType value, const char* parameterChildName,\n");
    errorMessage.append("const char* parameterName, const char* parameterListName)\n\n");
	errorMessage.append("Parameter specified by parameterName does not exist.\n\n");
	errorMessage.append("Parameter : ");
	errorMessage.append(parameterName);
	errorMessage.append("\nXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
	errorMessage.append("\n");
    }
    if(abortOnErrorFlag){checkErrorAndAbort();}


    TiXmlHandle  docHandle(parameterArrayDocPtr->RootElement());
	TiXmlElement* parameter             = docHandle.FirstChild(parameterListName).FirstChild(parameterName).ToElement();
	TiXmlElement* parameterChild     = new TiXmlElement(parameterChildName);
    parameter->LinkEndChild( parameterChild );
    parameterChild->SetAttribute("value", value.toString().c_str());
    if(value.isString())
    {parameterChild->SetAttribute("type","string");}
    else
    {parameterChild->SetAttribute("type",getDataType(value.toString().c_str()));}
    }


    void addParameterChildNoTypeSpec(XML_dataType value, const std::string& parameterChildName,
    const std::string& parameterName, const std::string& parameterListName)
    {
    addParameterChildNoTypeSpec(value,parameterChildName.c_str(),parameterName.c_str(), parameterListName.c_str());
    }

    void addParameterChildNoTypeSpec(XML_dataType value, const char* parameterChildName,
    const char* parameterName, const char* parameterListName)
    {
    abortIfNull();
    if(isParameterList(parameterListName) == 0)
    {
    errorFlag = true;
	errorMessage.append("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
	errorMessage.append("XML_ParameterListArray Class Error \n");
	errorMessage.append("void addParameterChild(XML_dataType value, const char* parameterChildName,\n");
    errorMessage.append("const char* parameterName, const char* parameterListName)\n\n");
	errorMessage.append("ParameterList specified parameterListName does not exist.\n\n");
	errorMessage.append("Offending parameterListName : ");
	errorMessage.append(parameterListName);
	errorMessage.append("\nXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
	errorMessage.append("\n");
    }
    if(abortOnErrorFlag){checkErrorAndAbort();}

    if(isParameter(parameterName,parameterListName) == 0)
    {
    errorFlag = true;
	errorMessage.append("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
	errorMessage.append("XML_ParameterListArray Class Error \n");
	errorMessage.append("void addParameterChild(XML_dataType value, const char* parameterChildName,\n");
    errorMessage.append("const char* parameterName, const char* parameterListName)\n\n");
	errorMessage.append("Parameter specified by parameterName does not exist.\n\n");
	errorMessage.append("Parameter : ");
	errorMessage.append(parameterName);
	errorMessage.append("\nXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
	errorMessage.append("\n");
    }
    if(abortOnErrorFlag){checkErrorAndAbort();}


    TiXmlHandle  docHandle(parameterArrayDocPtr->RootElement());
	TiXmlElement* parameter             = docHandle.FirstChild(parameterListName).FirstChild(parameterName).ToElement();
	TiXmlElement* parameterChild     = new TiXmlElement(parameterChildName);
    parameter->LinkEndChild( parameterChild );
    parameterChild->SetAttribute("value", value.toString().c_str());
    }


    void addParameterInstanceChild(XML_dataType value, int instanceIndex, const std::string& parameterChildName,
    const std::string& parameterName, const std::string& parameterListName)
    {
    addParameterInstanceChild(value, instanceIndex,  parameterChildName.c_str(), parameterName.c_str(),parameterListName.c_str());
    }

    void addParameterInstanceChild(XML_dataType value, int instanceIndex, const char* parameterChildName,
    const char* parameterName, const char* parameterListName)
    {
    abortIfNull();
    if(isParameter(parameterName,parameterListName) == 0)
    {
    errorFlag = true;
	errorMessage.append("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
	errorMessage.append("XML_ParameterListArray Class Error \n");
	errorMessage.append("void setParameterChildValue(XML_dataType value,const char* childParameter,\n");
    errorMessage.append("const char* parameterName, const char* parameterListName)\n\n");
	errorMessage.append("Parameter or ParameterList specified do not exist.\n\n");
	errorMessage.append("Parameter     : ");
	errorMessage.append(parameterName);
    errorMessage.append("\nParameterList : ");
	errorMessage.append(parameterListName);
	errorMessage.append("\nXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
	errorMessage.append("\n");
    }
    if(abortOnErrorFlag){checkErrorAndAbort();}

	TiXmlHandle   docHandle(parameterArrayDocPtr->RootElement());
	TiXmlElement* parameter          = docHandle.FirstChild(parameterListName).ToElement();
    TiXmlElement* parameterChild     = new TiXmlElement(parameterChildName);
	TiXmlNode*    node;

	long count = 0;
	for( node = parameter->FirstChild(parameterName);
		 node;
		 node = node->NextSibling(parameterName))
	{
		if(count == instanceIndex)
		{
        node->ToElement()->LinkEndChild( parameterChild );
        parameterChild->SetAttribute("value", value.toString().c_str());
		if(value.isString())
		{parameterChild->SetAttribute("type","string");}
		else
		{parameterChild->SetAttribute("type",getDataType(value.toString().c_str()));}
		}
		count++;
	}

    }


    void setParameterOrIgnore(XML_dataType value, const std::string& parameterName, std::string& parameterListName)
    {
        setParameterOrIgnore(value,  parameterName.c_str(), parameterListName.c_str());
    }

    void setParameterOrIgnore(XML_dataType value, const char* parameterName, const char* parameterListName)
    {
    	if(this->isParameter(parameterName, parameterListName))
    	{
    		this->setParameter(value,parameterName, parameterListName);
    		return;
    	}
    	return;
    }


    void setParameter(XML_dataType value, const std::string& parameterName, const std::string& parameterListName)
    {
    setParameter(value, parameterName.c_str(), parameterListName.c_str());
    }


    void setParameter(XML_dataType value, const char* parameterName, const char* parameterListName)
    {

    if(isParameter(parameterName,parameterListName) == 0)
    {
    errorFlag = true;
	errorMessage.append("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
	errorMessage.append("XML_ParameterListArray Class Error \n");
	errorMessage.append("setParameter(XML_dataType value, const char* parameterName, const char* parameterListName)\n\n");
	errorMessage.append("Parameter or ParameterList specified do not exist.\n\n");
	errorMessage.append("Parameter     : ");
	errorMessage.append(parameterName);
    errorMessage.append("\nParameterList : ");
	errorMessage.append(parameterListName);
	errorMessage.append("\nXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
	errorMessage.append("\n");
    }
    if(abortOnErrorFlag){checkErrorAndAbort();}

    TiXmlHandle  docHandle(parameterArrayDocPtr->RootElement());
	TiXmlElement* parameterElement    = docHandle.FirstChild(parameterListName).FirstChild(parameterName).ToElement();


    // Force a data conversion if necessary

    const char* inputDataType = getDataType(value.toString().c_str());
    const char* dataType      = parameterElement->Attribute("type");

	if(dataType == 0)
	{
	dataType = getDataType(parameterElement->Attribute("value"));
	parameterElement->SetAttribute("type",dataType);
    }

    std::string stringTemp;
    if((dataType != 0)&&(strcmp(dataType,inputDataType) != 0))
    {
    if(strcmp(dataType,"string") == 0) parameterElement->SetAttribute("value", XML_dataType(std::string(value)).toString().c_str());
    if(strcmp(dataType,"bool") == 0)   parameterElement->SetAttribute("value", XML_dataType(bool(value)).toString().c_str());
    if(strcmp(dataType,"long") == 0)   parameterElement->SetAttribute("value", XML_dataType(long(value)).toString().c_str());
    if(strcmp(dataType,"int") == 0)    parameterElement->SetAttribute("value", XML_dataType(int(value)).toString().c_str());
    if(strcmp(dataType,"float") == 0)  parameterElement->SetAttribute("value", XML_dataType(float(value)).toString().c_str());
    if(strcmp(dataType,"double") == 0) parameterElement->SetAttribute("value", XML_dataType(double(value)).toString().c_str());
    if((strcmp(dataType,"bool") == 0)&&(strcmp(inputDataType,"string")== 0))
    {
    stringTemp = XML_dataType(std::string(value)).toString();
    std::transform(stringTemp.begin(), stringTemp.end(),stringTemp.begin(), ::toupper);
	if(stringTemp.compare("FALSE") == 0)  parameterElement->SetAttribute("value","false");
    if(stringTemp.compare("TRUE")  == 0)  parameterElement->SetAttribute("value","true");
    if(stringTemp.compare("NO")    == 0)  parameterElement->SetAttribute("value","false");
	if(stringTemp.compare("YES")   == 0)  parameterElement->SetAttribute("value","true");
    }
    }
    else
    {
    parameterElement->SetAttribute("value", value.toString().c_str());
    if(dataType == 0)
    {
    if(value.isString())
	{parameterElement->SetAttribute("type","string");}
	else
	{parameterElement->SetAttribute("type",getDataType(value.toString().c_str()));}
	}
    }
    }

    void setParameterType(const std::string& typeValue, const std::string& parameterName, const std::string& parameterListName)
    {
    setParameterType(typeValue.c_str(),parameterName.c_str(), parameterListName.c_str());
    }

    void setParameterType(const char* typeValue, const char* parameterName, const char* parameterListName)
    {

    if(isParameter(parameterName,parameterListName) == 0)
    {
    errorFlag = true;
	errorMessage.append("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
	errorMessage.append("XML_ParameterListArray Class Error \n");
	errorMessage.append("setParameter(XML_dataType value, const char* parameterName, const char* parameterListName)\n\n");
	errorMessage.append("Parameter or ParameterList specified do not exist.\n\n");
	errorMessage.append("Parameter     : ");
	errorMessage.append(parameterName);
    errorMessage.append("\nParameterList : ");
	errorMessage.append(parameterListName);
	errorMessage.append("\nXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
	errorMessage.append("\n");
    }
    if(abortOnErrorFlag){checkErrorAndAbort();}

    TiXmlHandle  docHandle(parameterArrayDocPtr->RootElement());
	TiXmlElement* parameterElement    = docHandle.FirstChild(parameterListName).FirstChild(parameterName).ToElement();

    {parameterElement->SetAttribute("type",typeValue);}
    }

    void setParameterChildValue(XML_dataType value,const std::string& childParameter,
    const std::string& parameterName, const std::string& parameterListName)
    {
    setParameterChildValue(value,childParameter.c_str(), parameterName.c_str(), parameterListName.c_str());
    }


    void setParameterChildValue(XML_dataType value,const char* childParameter,
    const char* parameterName, const char* parameterListName)
    {
    if(isParameter(parameterName,parameterListName) == 0)
    {
    errorFlag = true;
	errorMessage.append("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
	errorMessage.append("XML_ParameterListArray Class Error \n");
	errorMessage.append("void setParameterChildValue(XML_dataType value,const char* childParameter,\n");
    errorMessage.append("const char* parameterName, const char* parameterListName)\n\n");
	errorMessage.append("Parameter or ParameterList specified do not exist.\n\n");
	errorMessage.append("Parameter     : ");
	errorMessage.append(parameterName);
    errorMessage.append("\nParameterList : ");
	errorMessage.append(parameterListName);
	errorMessage.append("\nXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
	errorMessage.append("\n");
    }
    if(abortOnErrorFlag){checkErrorAndAbort();}

    TiXmlHandle  docHandle(parameterArrayDocPtr->RootElement());
	TiXmlElement* parameterChild  = docHandle.FirstChild(parameterListName).FirstChild(parameterName).FirstChild(childParameter).ToElement();

    const char* inputDataType;
    const char* dataType;

	if(parameterChild)
	{
	// Force a data conversion if necessary

    inputDataType = getDataType(value.toString().c_str());
    dataType      = parameterChild->Attribute("type");

    if(dataType == 0)
	{
	dataType = getDataType(parameterChild->Attribute("value"));
	parameterChild->SetAttribute("type",dataType);
    }

    std::string stringTemp;

    if((dataType != 0)&&(strcmp(dataType,inputDataType) != 0))
    {
    if(strcmp(dataType,"string") == 0) parameterChild->SetAttribute("value", XML_dataType(std::string(value)).toString().c_str());
    if(strcmp(dataType,"bool") == 0)   parameterChild->SetAttribute("value", XML_dataType(bool(value)).toString().c_str());
    if(strcmp(dataType,"long") == 0)   parameterChild->SetAttribute("value", XML_dataType(long(value)).toString().c_str());
    if(strcmp(dataType,"int") == 0)    parameterChild->SetAttribute("value", XML_dataType(int(value)).toString().c_str());
    if(strcmp(dataType,"float") == 0)  parameterChild->SetAttribute("value", XML_dataType(float(value)).toString().c_str());
    if(strcmp(dataType,"double") == 0) parameterChild->SetAttribute("value", XML_dataType(double(value)).toString().c_str());
    if((strcmp(dataType,"bool") == 0)&&((strcmp(inputDataType,"string"))==0))
    {
    stringTemp = XML_dataType(std::string(value)).toString();;
    std::transform(stringTemp.begin(),    stringTemp.end(),stringTemp.begin(), ::toupper);
	if(stringTemp.compare("FALSE") == 0)  parameterChild->SetAttribute("value","false");
    if(stringTemp.compare("TRUE")  == 0)  parameterChild->SetAttribute("value","true");
    if(stringTemp.compare("NO")    == 0)  parameterChild->SetAttribute("value","false");
	if(stringTemp.compare("YES")   == 0)  parameterChild->SetAttribute("value","true");
    }
    }
    else
    {
    parameterChild->SetAttribute("value", value.toString().c_str());

    if(dataType == 0)
    {
    if(value.isString())
	{parameterChild->SetAttribute("type","string");}
	else
	{parameterChild->SetAttribute("type",getDataType(value.toString().c_str()));}
	}

    }

    }
    else
    {
    errorFlag = true;
	errorMessage.append("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
	errorMessage.append("XML_ParameterListArray Class Error \n");
	errorMessage.append("void setParameterChildValue(XML_dataType value,const char* childParameter,\n");
    errorMessage.append("const char* parameterName, const char* parameterListName)\n\n");
	errorMessage.append("Parameter child value specifed  by childParameter does not exist.\n\n");
	errorMessage.append("Parameter Child     : ");
	errorMessage.append(childParameter);
	errorMessage.append("\nXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
	errorMessage.append("\n");
    }
    if(abortOnErrorFlag){checkErrorAndAbort();}
    }

    void setParameterInstanceChildValue(XML_dataType value,int instanceIndex,const std::string& childParameter,
    const std::string& parameterName, const std::string& parameterListName)
    {
    setParameterInstanceChildValue(value,instanceIndex,childParameter.c_str(),parameterName.c_str(), parameterListName.c_str());
    }


    void setParameterInstanceChildValue(XML_dataType value,int instanceIndex,const char* childParameter,
    const char* parameterName, const char* parameterListName)
    {
    if(isParameter(parameterName,parameterListName) == 0)
    {
    errorFlag = true;
	errorMessage.append("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
	errorMessage.append("XML_ParameterListArray Class Error \n");
	errorMessage.append("void setParameterChildValue(XML_dataType value,const char* childParameter,\n");
    errorMessage.append("const char* parameterName, const char* parameterListName)\n\n");
	errorMessage.append("Parameter or ParameterList specified do not exist.\n\n");
	errorMessage.append("Parameter     : ");
	errorMessage.append(parameterName);
    errorMessage.append("\nParameterList : ");
	errorMessage.append(parameterListName);
	errorMessage.append("\nXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
	errorMessage.append("\n");
    }
    if(abortOnErrorFlag){checkErrorAndAbort();}

	TiXmlHandle  docHandle(parameterArrayDocPtr->RootElement());
	TiXmlElement* parameter = docHandle.FirstChild(parameterListName).ToElement();

	TiXmlNode* node;

    errorFlag = true;

    const char* inputDataType;
    const char* dataType;

	long count = 0;
	for( node = parameter->FirstChild(parameterName);
		 node;
		 node = node->NextSibling(parameterName) )
	{
		if(count == instanceIndex)
		{
			if(node->FirstChild(childParameter))
			{

			inputDataType = getDataType(value.toString().c_str());
            dataType      = (node->FirstChild(childParameter))->ToElement()->Attribute("type");
		    if((dataType != 0)&&(strcmp(dataType,inputDataType) != 0))
            {
                if(strcmp(dataType,"string") == 0)  (node->FirstChild(childParameter))->ToElement()->SetAttribute("value", XML_dataType(std::string(value)).toString().c_str());
                if(strcmp(dataType,"bool") == 0)    (node->FirstChild(childParameter))->ToElement()->SetAttribute("value", XML_dataType(bool(value)).toString().c_str());
		    	if(strcmp(dataType,"long") == 0)    (node->FirstChild(childParameter))->ToElement()->SetAttribute("value", XML_dataType(long(value)).toString().c_str());
		    	if(strcmp(dataType,"int") == 0)     (node->FirstChild(childParameter))->ToElement()->SetAttribute("value", XML_dataType(int(value)).toString().c_str());
		    	if(strcmp(dataType,"float") == 0)   (node->FirstChild(childParameter))->ToElement()->SetAttribute("value", XML_dataType(float(value)).toString().c_str());
		    	if(strcmp(dataType,"double") == 0)  (node->FirstChild(childParameter))->ToElement()->SetAttribute("value", XML_dataType(double(value)).toString().c_str());
            }
		    else
		    {
		    	(node->FirstChild(childParameter))->ToElement()->SetAttribute("value", value.toString().c_str()); // original

		    	if(dataType == 0)
		    	{
		    		if(value.isString())
		    		{(node->FirstChild(childParameter))->ToElement()->SetAttribute("type","string");}
		    		else
		    		{(node->FirstChild(childParameter))->ToElement()->SetAttribute("type",getDataType(value.toString().c_str()));}
		    	}

		    }

			errorFlag = false;
			}
			else
			{
			errorFlag = true;
			errorMessage.append("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
			errorMessage.append("XML_ParameterListArray Class Error \n");
			errorMessage.append("void setParameterInstanceChildValue(XML_dataType value,int instanceIndex,\n");
			errorMessage.append("const char* childParameter,const char* parameterName, const char* parameterListName)\n\n");
			errorMessage.append("Parameter child value specifed  by childParameter does not exist.\n\n");
			errorMessage.append("Parameter Child     : ");
			errorMessage.append(childParameter);
			errorMessage.append("\nXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
			errorMessage.append("\n");
			}
			break;
		}
		count++;
	}
	if(abortOnErrorFlag){checkErrorAndAbort();}
	}


	void setParameterInstanceChildValue(XML_dataType value,const std::string& instanceName,const std::string& childParameter,
    const std::string& parameterName, const std::string& parameterListName)
    {
    setParameterInstanceChildValue(value,instanceName.c_str(),childParameter.c_str(), parameterName.c_str(), parameterListName.c_str());
    }


	void setParameterInstanceChildValue(XML_dataType value,const char* instanceName,const char* childParameter,
    const char* parameterName, const char* parameterListName)
    {
    if(isParameter(parameterName,parameterListName) == 0)
    {
    errorFlag = true;
	errorMessage.append("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
	errorMessage.append("XML_ParameterListArray Class Error \n");
	errorMessage.append("void setParameterChildValue(XML_dataType value,const char* childParameter,\n");
    errorMessage.append("const char* parameterName, const char* parameterListName)\n\n");
	errorMessage.append("Parameter or ParameterList specified do not exist.\n\n");
	errorMessage.append("Parameter     : ");
	errorMessage.append(parameterName);
    errorMessage.append("\nParameterList : ");
	errorMessage.append(parameterListName);
	errorMessage.append("\nXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
	errorMessage.append("\n");
    }
    if(abortOnErrorFlag){checkErrorAndAbort();}

	TiXmlHandle  docHandle(parameterArrayDocPtr->RootElement());
	TiXmlElement* parameter = docHandle.FirstChild(parameterListName).ToElement();
    TiXmlElement* parameterChild;
	TiXmlNode* node;
    std::string instanceString;
    std::string indexString(instanceName);
    errorFlag          = true;
    bool instanceError = true;

    const char* inputDataType;
    const char* dataType;

	for( node = parameter->FirstChild(parameterName);
		 node;
		 node = node->NextSibling(parameterName) )
	{
		if(node->FirstChild("name"))
		{
		    parameterChild = node->FirstChild("name")->ToElement();
		    parameterChild->QueryStringAttribute("value",&instanceString);
		    if(instanceString.compare(indexString) == 0)
		    {
		    instanceError = false;
			if(node->FirstChild(childParameter))
			{

		    inputDataType = getDataType(value.toString().c_str());
            dataType      = (node->FirstChild(childParameter))->ToElement()->Attribute("type");
		    if((dataType != 0)&&(strcmp(dataType,inputDataType) != 0))
            {
                if(strcmp(dataType,"string") == 0)  (node->FirstChild(childParameter))->ToElement()->SetAttribute("value", XML_dataType(std::string(value)).toString().c_str());
                if(strcmp(dataType,"bool") == 0)    (node->FirstChild(childParameter))->ToElement()->SetAttribute("value", XML_dataType(bool(value)).toString().c_str());
		    	if(strcmp(dataType,"long") == 0)    (node->FirstChild(childParameter))->ToElement()->SetAttribute("value", XML_dataType(long(value)).toString().c_str());
		    	if(strcmp(dataType,"int") == 0)     (node->FirstChild(childParameter))->ToElement()->SetAttribute("value", XML_dataType(int(value)).toString().c_str());
		    	if(strcmp(dataType,"float") == 0)   (node->FirstChild(childParameter))->ToElement()->SetAttribute("value", XML_dataType(float(value)).toString().c_str());
		    	if(strcmp(dataType,"double") == 0)  (node->FirstChild(childParameter))->ToElement()->SetAttribute("value", XML_dataType(double(value)).toString().c_str());
            }
		    else
		    {
			(node->FirstChild(childParameter))->ToElement()->SetAttribute("value", value.toString().c_str()); // original

			if(dataType == 0)
		    {
		    	if(value.isString())
		    	{(node->FirstChild(childParameter))->ToElement()->SetAttribute("type","string");}
		    	else
		    	{(node->FirstChild(childParameter))->ToElement()->SetAttribute("type",getDataType(value.toString().c_str()));}
		   }


		   }

			errorFlag = false;
			}
			else
			{
			errorFlag = true;
			errorMessage.append("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
			errorMessage.append("XML_ParameterListArray Class Error \n");
			errorMessage.append("void setParameterInstanceChildValue(XML_dataType value,const char* instanceName,\n");
			errorMessage.append("const char* childParameter,const char* parameterName, const char* parameterListName)\n\n");
			errorMessage.append("Parameter child value specified  by childParameter does not exist.\n\n");
		    errorMessage.append("Parameter Instance Name     : ");
			errorMessage.append(indexString);
			errorMessage.append("\nParameter Child             : ");
			errorMessage.append(childParameter);

			errorMessage.append("\nXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
			errorMessage.append("\n");
			}
			break;
		    }
	}
	}
    if(instanceError)
    {
    errorFlag = true;
    errorMessage.append("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
    errorMessage.append("XML_ParameterListArray Class Error \n");
    errorMessage.append("Parameter instance name specified does not exist.\n\n");
    errorMessage.append("Parameter List : ");
	errorMessage.append(parameterListName);
	errorMessage.append("\n");
	errorMessage.append("Parameter      : ");
	errorMessage.append(parameterName);
	errorMessage.append("\n");
    errorMessage.append("ParameterChild : ");
	errorMessage.append(childParameter);
	errorMessage.append("\n");
    errorMessage.append("instance name  : ");
    errorMessage.append(indexString);
    errorMessage.append("\nXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
    errorMessage.append("\n");
    }

	if(abortOnErrorFlag){checkErrorAndAbort();}
	}


	XML_dataType getParameterChildValueOrDefault(const std::string& childParameter,
    const std::string&  parameterName, const std::string&  parameterListName,XML_dataType defaultValue) const
    {
    return getParameterChildValueOrDefault(childParameter.c_str(), parameterName.c_str(), parameterListName.c_str(),defaultValue);
    }

	XML_dataType getParameterChildValueOrDefault(const char* childParameter,
    const char* parameterName, const char* parameterListName,XML_dataType defaultValue) const
    {
	if(this->isParameterInstanceChildValue(0,childParameter,parameterName,parameterListName))
	{
		return this->getParameterChildValue(childParameter,parameterName,parameterListName);
	}
    return defaultValue;
    }


	XML_dataType getParameterInstanceChildValueOrDefault(int instanceIndex,const std::string& childParameter,
    const std::string&  parameterName, const std::string&  parameterListName,XML_dataType defaultValue) const
    {
    return getParameterInstanceChildValueOrDefault(instanceIndex,childParameter.c_str(),
    parameterName.c_str(),parameterListName.c_str(),defaultValue);
    }

	XML_dataType getParameterInstanceChildValueOrDefault(int instanceIndex,const char* childParameter,
    const char* parameterName, const char* parameterListName,XML_dataType defaultValue) const
    {
	if(this->isParameterInstanceChildValue(instanceIndex,childParameter,parameterName,parameterListName))
	{
		return this->getParameterInstanceChildValue(instanceIndex,childParameter,parameterName,parameterListName);
	}
    return defaultValue;
    }

	XML_dataType getParameterInstanceChildValue(const std::string& instanceName,const std::string& childParameter,
    const std::string& parameterName, const std::string& parameterListName) const
    {
    return getParameterInstanceChildValue(instanceName.c_str(),childParameter.c_str(),parameterName.c_str(), parameterListName.c_str());
    }

	XML_dataType getParameterInstanceChildValue(const char* instanceName,const char* childParameter,
    const char* parameterName, const char* parameterListName) const
    {
    XML_dataType returnValue;

    // Setting error flags require overriding const status

    XML_ParameterListArray* Eptr =  const_cast<XML_ParameterListArray*> (this);

    if(not isParameterList(parameterListName))
	{
		Eptr->errorFlag = true;
		Eptr->errorMessage.append("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
		Eptr->errorMessage.append("XML_ParameterListArray Class Error \n");
		Eptr->errorMessage.append("Parameter List not specified \n");
		Eptr->errorMessage.append("Parameter List requested : ");
		Eptr->errorMessage.append(parameterListName);
		Eptr->errorMessage.append("\n");
		Eptr->errorMessage.append("\n");
		if(not abortOnErrorFlag) return returnValue;
	}

	if(abortOnErrorFlag){checkErrorAndAbort();}

    if(isParameter(parameterName,parameterListName) == 0)
    {
    Eptr->errorFlag = true;
	Eptr->errorMessage.append("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
	Eptr->errorMessage.append("XML_ParameterListArray Class Error \n");
	Eptr->errorMessage.append("void setParameterChildValue(XML_dataType value,const char* childParameter,\n");
    Eptr->errorMessage.append("const char* parameterName, const char* parameterListName)\n\n");
	Eptr->errorMessage.append("Parameter or ParameterList specified do not exist.\n\n");
	Eptr->errorMessage.append("Parameter     : ");
	Eptr->errorMessage.append(parameterName);
    Eptr->errorMessage.append("\nParameterList : ");
	Eptr->errorMessage.append(parameterListName);
	Eptr->errorMessage.append("\nXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
	Eptr->errorMessage.append("\n");
	if(not abortOnErrorFlag) return returnValue;
    }

    if(abortOnErrorFlag){checkErrorAndAbort();}

	TiXmlHandle  docHandle(parameterArrayDocPtr->RootElement());
	TiXmlElement* parameter = docHandle.FirstChild(parameterListName).ToElement();
    TiXmlElement* parameterChild;
	TiXmlNode* node;
    std::string instanceString;
    std::string indexString(instanceName);
    Eptr->errorFlag          = true;
    bool instanceError = true;

	for( node = parameter->FirstChild(parameterName);
		 node;
		 node = node->NextSibling(parameterName) )
	{
		if(node->FirstChild("name"))
		{
		    parameterChild = node->FirstChild("name")->ToElement();
		    parameterChild->QueryStringAttribute("value",&instanceString);
		    if(instanceString.compare(indexString) == 0)
		    {
		    instanceError = false;
		    if(node->FirstChild(childParameter))
		    {
		    return getParameterValue(node->FirstChild(childParameter)->ToElement(),parameterName,  parameterListName);
		    }
		    else
		    {
		    Eptr->errorFlag = true;
		    }
		    }
	     }
	}


    Eptr->errorFlag = true;
    if(instanceError)
    {
    Eptr->errorMessage.append("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
    Eptr->errorMessage.append("XML_ParameterListArray Class Error \n");
    Eptr->errorMessage.append("Parameter instance name specified does not exist.\n\n");
    Eptr->errorMessage.append("Parameter List : ");
	Eptr->errorMessage.append(parameterListName);
	Eptr->errorMessage.append("\n");
	Eptr->errorMessage.append("Parameter      : ");
	Eptr->errorMessage.append(parameterName);
	Eptr->errorMessage.append("\n");
    Eptr->errorMessage.append("ParameterChild : ");
	Eptr->errorMessage.append(childParameter);
	Eptr->errorMessage.append("\n");
    Eptr->errorMessage.append("instance name  : ");
    Eptr->errorMessage.append(indexString);
    Eptr->errorMessage.append("\nXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
    Eptr->errorMessage.append("\n");
	if(abortOnErrorFlag){checkErrorAndAbort();}
	else return returnValue;
	}

    Eptr->errorMessage.append("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
	Eptr->errorMessage.append("XML_ParameterListArray Class Error \n");
	Eptr->errorMessage.append("Parameter Instance Child Parameter Not found.\n");
	Eptr->errorMessage.append("Parameter List : ");
	Eptr->errorMessage.append(parameterListName);
	Eptr->errorMessage.append("\n");
	Eptr->errorMessage.append("Parameter      : ");
	Eptr->errorMessage.append(parameterName);
	Eptr->errorMessage.append("\n");
    Eptr->errorMessage.append("ParameterChild : ");
	Eptr->errorMessage.append(childParameter);
	Eptr->errorMessage.append("\n");
	if(abortOnErrorFlag){checkErrorAndAbort();}

	return returnValue;
	}


	void getParameterListNames(std::vector < std::string >& paramListNames) const
	{
	paramListNames.clear();
	if(parameterArrayDocPtr == 0) return;
	TiXmlHandle   docHandle(parameterArrayDocPtr->RootElement());
	TiXmlNode* parameterList = docHandle.Element();
	TiXmlNode* child = 0;
    while((child = parameterList->IterateChildren( child )))
    {
	if(child->Type() != TiXmlNode::TINYXML_COMMENT)
	{
    paramListNames.push_back(child->Value());
	}
    }
	}


	void getParameterChildNames(long instanceIndex, const std::string& parameterName,
	std::string& parameterListName, std::vector < std::string >& paramChildNames) const
	{
    getParameterChildNames(instanceIndex, parameterName.c_str(),parameterListName.c_str(), paramChildNames);
	}


	void getParameterChildNames(long instanceIndex, const char* parameterName,
	const char* parameterListName, std::vector < std::string >& paramChildNames) const
	{
	paramChildNames.clear();
	if(not isParameter(parameterName, parameterListName)) return;

	TiXmlHandle  docHandle(parameterArrayDocPtr->RootElement());
	TiXmlElement* parameterList = docHandle.FirstChild(parameterListName).ToElement();

	TiXmlNode* node = 0;
    TiXmlNode* child = 0;

	long count = 0;
	for( node = parameterList->FirstChild(parameterName);
		 node;
		 node = node->NextSibling(parameterName) )
	{
	    if(count == instanceIndex)
	    {
	    	child = 0;
	    	while((child = node->IterateChildren( child )))
	    	{
	    		if(child->Type() != TiXmlNode::TINYXML_COMMENT)
	    		{
                paramChildNames.push_back(child->Value());
	    		}
	    	}
        }
        count++;
    }
	}

    void getParameterNames(const std::string& parameterListName, std::vector < std::string >& paramNames) const
	{
	getParameterNames(parameterListName.c_str(),paramNames);
	}

    void getParameterNames(const char* parameterListName, std::vector < std::string >& paramNames) const
	{
	paramNames.clear();
	if(not isParameterList(parameterListName)) return;

	paramNames.clear();
	if(parameterArrayDocPtr == 0) return;
	TiXmlHandle   docHandle(parameterArrayDocPtr->RootElement());
	TiXmlNode* parameterList = docHandle.FirstChild(parameterListName).Element();
	TiXmlNode* child = 0;
    while((child = parameterList->IterateChildren( child )))
    {
    if(child->Type() != TiXmlNode::TINYXML_COMMENT)
	{
    paramNames.push_back(child->Value());
	}
    }
	}

    long parameterListCount() const
	{
	if(parameterArrayDocPtr == 0) return 0;
	TiXmlHandle   docHandle(parameterArrayDocPtr->RootElement());
	TiXmlNode* parameterList = docHandle.Element();
	TiXmlNode* child = 0;
	long count = 0;
    while((child = parameterList->IterateChildren( child )))
    {
    if(child->Type() != TiXmlNode::TINYXML_COMMENT)
    {
    count++;
    }
    }
    return count;
	}

    long parameterListInstanceCount(const std::string& parameterListName) const
    {
    return parameterListInstanceCount(parameterListName.c_str());
    }

    long parameterListInstanceCount(const char* parameterListName) const
	{
	if(parameterArrayDocPtr == 0) return 0;
	TiXmlHandle   docHandle(parameterArrayDocPtr->RootElement());
	TiXmlElement* parameterList = docHandle.FirstChild(parameterListName).ToElement();

	if(parameterList == 0){return 0;}
	TiXmlElement* node;
	long count = 0;
	for( node = parameterList;
		 node;
		 node = parameterList->NextSiblingElement(parameterListName))
	{
		if(node->Type() != TiXmlNode::TINYXML_COMMENT)
	    {
		count++;
	    }
	}

	return count;
	}

    /*  These need to be fixed */
    /*
    long parameterCount(const char* parameterListName)
    {
    if(parameterArrayDocPtr == 0) return 0;
    TiXmlHandle   docHandle(parameterArrayDocPtr->RootElement());

	TiXmlElement* parameterList = docHandle.FirstChild(parameterListName).ToElement();
	if(parameterList == NULL) return 0;

    TiXmlNode* parameter = parameterList->FirstChild();
    if(parameter == NULL) return 0;

    long count = 0;
	for(parameter; parameter; parameter = parameter->NextSiblingElement())
	{
	count++;
	}
	return count;
    }

    std::vector<std::string> getParameterNames(const char* parameterListName)
    {
    std::vector<std::string> paramNames;
    if(parameterArrayDocPtr == 0) return paramNames;

    TiXmlHandle   docHandle(parameterArrayDocPtr->RootElement());

	TiXmlElement* parameterList = docHandle.FirstChild(parameterListName).ToElement();
	if(parameterList == NULL) return paramNames;

    TiXmlNode* parameter = parameterList->FirstChild();
    if(parameter == NULL) return paramNames;

	for(parameter; parameter; parameter = parameter->NextSiblingElement())
	{
	paramNames.push_back(parameter->ToElement()->Value());
	}

	return paramNames;
    }
    */

////////////////////////////////////////////////////////////////////////
//
// Internal construction and manipulation of ParameterListArray
//
////////////////////////////////////////////////////////////////////////


	void initialize()
	{
	destroyData();
	}

	void initialize(const XML_ParameterListArray& P)
	{
	destroyData();
	parameterArrayDocPtr = new TiXmlDocument(*(P.parameterArrayDocPtr));
	abortOnErrorFlag     = P.abortOnErrorFlag;
	}

    void initalize(const std::string& fileName)
    {
    initialize(fileName.c_str());
    }

	void initialize(const char* fileName)
	{
		destroyData();
		FILE* dataFile;

#ifdef _MSC_VER
		if ((fopen_s(&dataFile,fileName, "r")) != 0)
#else
		if ((dataFile = fopen(fileName, "r")) == NULL)
#endif
		{
		errorFlag = true;
		errorMessage.append("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
		errorMessage.append("XML_ParameterListArray Class Error \n");
		errorMessage.append("Error loading XML data from file : \n");
		errorMessage.append(fileName);
		errorMessage.append("\n");
		errorMessage.append("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
		}


		if(not errorFlag)
		{
		fclose(dataFile);
		parameterArrayDocPtr = new TiXmlDocument(fileName);
		if(parameterArrayDocPtr->LoadFile() == false)
		{
		errorFlag = true;
		errorMessage.append("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
		errorMessage.append("XML_ParameterListArray Class Error \n");
		errorMessage.append("Error loading XML data from file : \n");
		errorMessage.append(fileName);
		errorMessage.append("\n");
		errorMessage.append("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
		}}

		if(abortOnErrorFlag){checkErrorAndAbort();}

	}

	void initializeFromString(const std::string& paramListAsString)
	{
	    this->initialize();
	    this->parameterArrayDocPtr = new TiXmlDocument();
		std::istringstream stringStream(paramListAsString);
		stringStream >> *(this->parameterArrayDocPtr);
	}

	friend void operator >>(std::istream& in_stream, XML_ParameterListArray& P)
	{
		P.initialize();
		P.parameterArrayDocPtr = new TiXmlDocument();
		in_stream >> *P.parameterArrayDocPtr;
	}

	void destroyData()
	{
		if(parameterArrayDocPtr != 0){delete parameterArrayDocPtr;}
		parameterArrayDocPtr = 0;
		errorFlag            = false;
		errorMessage.clear();
	}

	friend std::ostream&  operator <<(std::ostream& out_stream, const XML_ParameterListArray& P)
	{
    //
	//  Output using pretty printing
	//
		TiXmlPrinter printer;
		P.parameterArrayDocPtr->Accept( &printer );
		out_stream << printer.CStr();
		return out_stream;
	}

	void checkErrorAndAbort() const
	{
    if(errorFlag)
    {
    	printf("\n%s\n",getErrorMessage());
    	exit(1);
    }
	}

	void setAbortOnErrorFlag()  const
	{
	XML_ParameterListArray* Eptr =  const_cast<XML_ParameterListArray*> (this);
	Eptr->abortOnErrorFlag       = true;
	}
	void  clearAbortOnErrorFlag() const
	{
	XML_ParameterListArray* Eptr =  const_cast<XML_ParameterListArray*> (this);
	Eptr-> abortOnErrorFlag = false;
	}

	bool        getErrorFlag()    const {return errorFlag;}
	const char* getErrorMessage() const {return errorMessage.c_str();}


	void clearError() const
	{
	XML_ParameterListArray* Eptr =  const_cast<XML_ParameterListArray*> (this);
	Eptr->errorFlag = false;
	Eptr->errorMessage.clear();
	}


    int isParameterList(const std::string& parameterListName) const
	{
	return isParameterList(parameterListName.c_str());
	}

	int isParameterList(const char* parameterListName) const
	{
	if(parameterArrayDocPtr == 0) return 0;
	TiXmlHandle  docHandle(parameterArrayDocPtr->RootElement());
	TiXmlElement* parameterList = docHandle.FirstChild(parameterListName).ToElement();
	if(parameterList) {return 1;}
	return 0;
	}


	int isParameter(const std::string& parameterName, const std::string& parameterListName) const
	{
		return isParameter(parameterName.c_str(),parameterListName.c_str());
	}

	int isParameter(const char* parameterName, const char* parameterListName) const
	{
	if(isParameterList(parameterListName) == 0)
    {return 0;}

	if(parameterArrayDocPtr == 0) return 0;
	TiXmlHandle  docHandle(parameterArrayDocPtr->RootElement());
	TiXmlElement* parameter = docHandle.FirstChild(parameterListName).FirstChild(parameterName).ToElement();
	if(parameter) {return 1;}
	return 0;
	}


    long getParameterInstanceCount(const std::string& parameterName, const std::string& parameterListName) const
    {
    return getParameterInstanceCount(parameterName.c_str(),parameterListName.c_str());
    }

	long getParameterInstanceCount(const char* parameterName, const char* parameterListName) const
	{
    if(isParameterList(parameterListName) == 0) {return 0;}

	if(parameterArrayDocPtr == 0) return 0;
	TiXmlHandle  docHandle(parameterArrayDocPtr->RootElement());
	TiXmlElement* parameter = docHandle.FirstChild(parameterListName).ToElement();

	TiXmlNode* node;

	long count = 0;
	for( node = parameter->FirstChild(parameterName);
		 node;
		 node = node->NextSibling(parameterName))
	{
		count++;
	}
	return count;
	}

	//
	// Indexing of the instance starts at 0 to facilitate extracting data into C type arrays
	//
    XML_dataType getParameterInstanceValue(int instanceIndex,const std::string& parameterName, const std::string& parameterListName) const
	{
	return getParameterInstanceValue(instanceIndex, parameterName.c_str(), parameterListName.c_str());
	}

	XML_dataType getParameterInstanceValue(int instanceIndex,const char* parameterName, const char* parameterListName) const
	{
    XML_dataType returnValue;
	TiXmlHandle  docHandle(parameterArrayDocPtr->RootElement());
	TiXmlElement* parameter = docHandle.FirstChild(parameterListName).ToElement();

	// Setting error flags require over-riding const status

    XML_ParameterListArray* Eptr =  const_cast<XML_ParameterListArray*> (this);

	TiXmlNode* node;

	long count = 0;
	for( node = parameter->FirstChild(parameterName);
		 node;
		 node = node->NextSibling(parameterName) )
	{
		if(count == instanceIndex)
		{
			if(node->ToElement())
			{
			returnValue = getParameterValue(node->ToElement(),parameterName,  parameterListName);
			}
			else
			{
			Eptr->errorFlag   = true;
		    Eptr->errorMessage.append("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
	     	Eptr->errorMessage.append("XML_ParameterListArray Class Error \n");
	     	Eptr->errorMessage.append("Parameter Instance Not found.\n");
	     	Eptr->errorMessage.append("Parameter List : ");
	     	Eptr->errorMessage.append(parameterListName);
	     	Eptr->errorMessage.append("\n");
	     	Eptr->errorMessage.append("Parameter      : ");
	     	Eptr->errorMessage.append(parameterName);
	     	Eptr->errorMessage.append("\n");
			}
		}
		count++;
	}

	if(returnValue.isNull())
	{
	Eptr->errorFlag = true;
	}

	if(errorFlag)
	{
	Eptr->errorMessage.append("Parameter Instance : ");
	Eptr->stringStream.str("");
	Eptr->stringStream << instanceIndex;
	Eptr->errorMessage.append(stringStream.str());
	Eptr->errorMessage.append("\n");
	}

	if(abortOnErrorFlag){checkErrorAndAbort();}
	return returnValue;
	}


	//
	// Indexing of the instance starts at 0 to facilitate extracting data into C type arrays
    //

    XML_dataType getParameterInstanceChildValue(int instanceIndex,const std::string& childParameter,
			const std::string& parameterName, const std::string& parameterListName) const
    {
    return getParameterInstanceChildValue(instanceIndex,childParameter.c_str(),parameterName.c_str(),parameterListName.c_str());
    }

	XML_dataType getParameterInstanceChildValue(int instanceIndex,const char* childParameter,
			const char* parameterName, const char* parameterListName) const
	{
	TiXmlHandle  docHandle(parameterArrayDocPtr->RootElement());
	TiXmlElement* parameter = docHandle.FirstChild(parameterListName).ToElement();

	// Setting error flags require overriding const status

    XML_ParameterListArray* Eptr =  const_cast<XML_ParameterListArray*> (this);

	if(not this->isParameter(parameterName,parameterListName))
	{
        Eptr->errorFlag   = true;
        Eptr->errorMessage.append("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
	    Eptr->errorMessage.append("XML_ParameterListArray Class Error \n");
	    Eptr->errorMessage.append("ParameterList or Parameter Not found.\n");
	    Eptr->errorMessage.append("ParameterList : ");
	    Eptr->errorMessage.append(parameterListName);
	    Eptr->errorMessage.append("\n");
	    Eptr->errorMessage.append("Parameter     : ");
	    Eptr->errorMessage.append(parameterName);
	    Eptr->errorMessage.append("\n");
	    if(abortOnErrorFlag){checkErrorAndAbort();}
	}


	TiXmlNode* node;
	XML_dataType returnValue;



	long count = 0;
	for( node = parameter->FirstChild(parameterName);
		 node;
		 node = node->NextSibling(parameterName) )
	{
	    if(count == instanceIndex)
	    {
	    	if(node->FirstChild(childParameter))
	    	{
	    	returnValue =  getParameterValue(node->FirstChild(childParameter)->ToElement(),parameterName,  parameterListName);
	    	}
	    	else
	    	{
	    	Eptr->errorFlag   = true;
	     	Eptr->errorMessage.append("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
	     	Eptr->errorMessage.append("XML_ParameterListArray Class Error \n");
	     	Eptr->errorMessage.append("Parameter Instance Child Value Not found.\n");
	     	Eptr->errorMessage.append("Parameter List : ");
	     	Eptr->errorMessage.append(parameterListName);
	     	Eptr->errorMessage.append("\n");
	     	Eptr->errorMessage.append("Parameter      : ");
	     	Eptr->errorMessage.append(parameterName);
	     	Eptr->errorMessage.append("\n");
	    	}
	    }
	    count++;
	}


	if((returnValue.isNull())&&(not errorFlag))
	{
	Eptr->errorFlag = true;
	Eptr->errorMessage.append("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
	Eptr->errorMessage.append("XML_ParameterListArray Class Error \n");
	Eptr->errorMessage.append("Parameter Instance Not found.\n");
	Eptr->errorMessage.append("Parameter List : ");
	Eptr->errorMessage.append(parameterListName);
	Eptr->errorMessage.append("\n");
	Eptr->errorMessage.append("Parameter      : ");
	Eptr->errorMessage.append(parameterName);
	Eptr->errorMessage.append("\n");
	}

	if(errorFlag)
	{
	    	Eptr->errorMessage.append("Parameter Instance : ");
	    	Eptr->stringStream.str("");
	    	Eptr->stringStream << instanceIndex;
	    	Eptr->errorMessage.append(stringStream.str());
	    	Eptr->errorMessage.append("\n");
	        Eptr->errorMessage.append("Parameter Child  : ");
	    	Eptr->errorMessage.append(childParameter);
	    	Eptr->errorMessage.append("\n");
	}
	if(abortOnErrorFlag){checkErrorAndAbort();}
	return returnValue;
	}


    bool isParameterInstanceChildValue(int instanceIndex,const std::string& childParameter,
    const std::string& parameterName, const std::string& parameterListName) const
    {
    return isParameterInstanceChildValue(instanceIndex,childParameter.c_str(), parameterName.c_str(), parameterListName.c_str());
    }


    bool isParameterInstanceChildValue(int instanceIndex,const char* childParameter,
    const char* parameterName, const char* parameterListName) const
	{

	if(parameterArrayDocPtr == 0) return false;


	TiXmlHandle  docHandle(parameterArrayDocPtr->RootElement());
	TiXmlElement* parameter = docHandle.FirstChild(parameterListName).ToElement();

	XML_ParameterListArray* Eptr =  const_cast<XML_ParameterListArray*> (this);

	if(not this->isParameter(parameterName,parameterListName))
	{
        Eptr->errorFlag   = true;
        Eptr->errorMessage.append("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
	    Eptr->errorMessage.append("XML_ParameterListArray Class Error \n");
	    Eptr->errorMessage.append("ParameterList or Parameter Not found.\n");
	    Eptr->errorMessage.append("ParameterList : ");
	    Eptr->errorMessage.append(parameterListName);
	    Eptr->errorMessage.append("\n");
	    Eptr->errorMessage.append("Parameter     : ");
	    Eptr->errorMessage.append(parameterName);
	    Eptr->errorMessage.append("\n");
	    if(abortOnErrorFlag){checkErrorAndAbort();}
	}

	TiXmlNode* node;

	long count = 0;
	for( node = parameter->FirstChild(parameterName);
		 node;
		 node = node->NextSibling(parameterName) )
	{


	    if(count == instanceIndex)
	    {
	    	if(node->FirstChild(childParameter))
	    	{
	    	return true;
	    	}
	    	else
	    	{
	    	return false;
	    	}
	    }
	    count++;
	}
    return false;
	}

    XML_dataType getParameterChildValue(const std::string& childParameter, const std::string& parameterName, const std::string& parameterListName) const
	{
	return getParameterChildValue(childParameter.c_str(), parameterName.c_str(), parameterListName.c_str());
	}

    XML_dataType getParameterChildValue(const char* childParameter, const char* parameterName, const char* parameterListName) const
	{
    XML_dataType returnValue;

    // Setting error flags require overriding const status

    XML_ParameterListArray* Eptr =  const_cast<XML_ParameterListArray*> (this);

	if(not isParameterList(parameterListName))
	{
		Eptr->errorFlag = true;
		Eptr->errorMessage.append("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
		Eptr->errorMessage.append("XML_ParameterListArray Class Error \n");
		Eptr->errorMessage.append("Parameter List not specified \n");
		Eptr->errorMessage.append("Parameter List requested : ");
		Eptr->errorMessage.append(parameterListName);
		Eptr->errorMessage.append("\n");
		Eptr->errorMessage.append("\n");
		if(not abortOnErrorFlag) return returnValue;
	}
	if(abortOnErrorFlag){checkErrorAndAbort();}

	TiXmlHandle  docHandle(parameterArrayDocPtr->RootElement());
	TiXmlElement* parameter = docHandle.FirstChild(parameterListName).FirstChild(parameterName).FirstChild(childParameter).ToElement();

	returnValue = getParameterValue(parameter, parameterName,  parameterListName);

	if(errorFlag)
	{
	Eptr->errorMessage.append("Parameter Child  : ");
	Eptr->errorMessage.append(childParameter);
	Eptr->errorMessage.append("\n");
	if(not abortOnErrorFlag) {returnValue.initialize(); return returnValue;}
	}

	if(abortOnErrorFlag){checkErrorAndAbort();}
	return returnValue;
	}


   //
   // If the specified parameter exists, then this routine returns the value, otherwise it returns the
   // defaultValue specified.
   //
   XML_dataType getParameterValueOrDefault(const std::string& parameterName, const std::string& parameterListName, XML_dataType defaultValue ) const
   {
   return getParameterValueOrDefault(parameterName.c_str(), parameterListName.c_str(),defaultValue);
   }

   XML_dataType getParameterValueOrDefault(const char* parameterName, const char* parameterListName, XML_dataType defaultValue ) const
   {

   if(this->isParameter(parameterName,parameterListName))
   {
    return this->getParameterValue(parameterName,parameterListName);
   }
   return defaultValue;
   }
	//
	// If one wants to extract a boolean value as a string one must append the toString() member function, e.g.
	//
	// string boolAsString = paramList.getParameterValue(...).toString();
	//


	XML_dataType getParameterValue(const std::string& parameterName, std::string& parameterListName) const
	{
	return getParameterValue(parameterName.c_str(),parameterListName.c_str());
	}

	XML_dataType getParameterValue(const char* parameterName, const char* parameterListName) const
	{
	XML_dataType returnValue;

	// Setting error flags require overriding const status

    XML_ParameterListArray* Eptr =  const_cast<XML_ParameterListArray*> (this);

	if(not isParameterList(parameterListName))
	{
		Eptr->errorFlag = true;
		Eptr->errorMessage.append("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
		Eptr->errorMessage.append("XML_ParameterListArray Class Error \n");
		Eptr->errorMessage.append("Parameter List not specified \n");
		Eptr->errorMessage.append("Parameter List requested : ");
		Eptr->errorMessage.append(parameterListName);
		Eptr->errorMessage.append("\n");
		Eptr->errorMessage.append("\n");
		if(not abortOnErrorFlag) return returnValue;
	}
	if(abortOnErrorFlag){checkErrorAndAbort();}

	if(isParameter(parameterName,parameterListName) == 0)
    {
    Eptr->errorFlag = true;
	Eptr->errorMessage.append("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
	Eptr->errorMessage.append("XML_ParameterListArray Class Error \n");
	Eptr->errorMessage.append("Parameter specified by parameterName does not exist.\n\n");
	Eptr->errorMessage.append("ParameterName : ");
	Eptr->errorMessage.append(parameterName);
	Eptr->errorMessage.append("\nXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
	Eptr->errorMessage.append("\n");
	if(not abortOnErrorFlag) return returnValue;
    }
    if(abortOnErrorFlag){checkErrorAndAbort();}


	TiXmlHandle  docHandle(parameterArrayDocPtr->RootElement());
	TiXmlElement* parameter = docHandle.FirstChild(parameterListName).FirstChild(parameterName).ToElement();

	returnValue = getParameterValue(parameter, parameterName,  parameterListName);

	if(abortOnErrorFlag){checkErrorAndAbort();}
	return returnValue;
	}


	XML_dataType getParameterValue(const TiXmlElement* parameter,const std::string& parameterName, const std::string& parameterListName) const
	{
	return 	getParameterValue(parameter,parameterName.c_str(), parameterListName.c_str());
	}


	XML_dataType getParameterValue(const TiXmlElement* parameter,const char* parameterName, const char* parameterListName) const
	{
	int           intParam;
	double     doubleParam;
	float       floatParam;
	bool         boolParam;
	std::string     stringParam;
	std::string      stringTemp;
	bool     explicitType;

    // Setting error flags require overriding const status

    XML_ParameterListArray* Eptr =  const_cast<XML_ParameterListArray*> (this);

	const char* dataType = 0;
	if(parameter)
	{
		//
		// Check for values attribute
		//
		if(parameter->Attribute("value") == 0)
		{
		Eptr->errorFlag = true;
		Eptr->errorMessage.append("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
		Eptr->errorMessage.append("XML_ParameterListArray Class Error \n");
		Eptr->errorMessage.append("Parameter Value not specified using value= \"...\"\n");
		Eptr->errorMessage.append("Parameter List : ");
		Eptr->errorMessage.append(parameterListName);
		Eptr->errorMessage.append("\n");
		Eptr->errorMessage.append("Parameter      : ");
		Eptr->errorMessage.append(parameterName);
		Eptr->errorMessage.append("\n");
		return XML_dataType();
		}
		//
		// Check for explicitly typed parameters
		//
		explicitType = true;
		dataType = parameter->Attribute("type");

		//
		// If not explicitly typed, then discover the type by interrogating the string representation
		//

		if(dataType == 0)
		{
		 stringParam = parameter->Attribute("value");
		 dataType = getDataType(stringParam.c_str());
		 explicitType = false;
		}

		if(strcmp(dataType,"bool") == 0)
		{
			if(parameter->QueryBoolAttribute("value",&boolParam)  == TIXML_SUCCESS) {return XML_dataType(boolParam);}
		}
		if(strcmp(dataType,"int") == 0)
		{
			if(parameter->QueryIntAttribute("value",&intParam)  == TIXML_SUCCESS) {return XML_dataType(intParam);}
		}
		if(strcmp(dataType,"long") == 0)
		{
			if(parameter->QueryIntAttribute("value",&intParam)  == TIXML_SUCCESS) {return XML_dataType((long)intParam);}
		}
		if(strcmp(dataType,"float") == 0)
		{
			if(parameter->QueryFloatAttribute("value",&floatParam)  == TIXML_SUCCESS) {return XML_dataType(floatParam);}
		}
		if(strcmp(dataType,"double") == 0)
		{
			if(parameter->QueryDoubleAttribute("value",&doubleParam)  == TIXML_SUCCESS) {return XML_dataType(doubleParam);}
		}
		//
		// For strings, if not explicitly typed, check to see if it's a boolean variable first
		// as designated by any upper or lower case collection of letters spelling true, false
		// or yes or no.
		//
		// I don't use tinyXML QueryBoolAttribute since it returns incorrectly if one specifies the
		// string None.

		if(strcmp(dataType,"string") == 0)
		{
			//if(parameter->QueryBoolAttribute("value",&boolParam)  == TIXML_SUCCESS)     {return XML_dataType(boolParam);}

            if(parameter->QueryStringAttribute("value",&stringParam)  == TIXML_SUCCESS)
            {
            if(explicitType){return XML_dataType(stringParam);}
			else // interrogate first for conversion to boolean based on false, true, yes or no,
			{
			stringTemp = stringParam;
			std::transform(stringTemp.begin(), stringTemp.end(),stringTemp.begin(), ::toupper);
			if(stringTemp.compare("FALSE") == 0) {boolParam = false; return XML_dataType(boolParam);}
			if(stringTemp.compare("TRUE")  == 0) {boolParam = true;  return XML_dataType(boolParam);}
			if(stringTemp.compare("NO") == 0)    {boolParam = false; return XML_dataType(boolParam);}
			if(stringTemp.compare("YES")  == 0)  {boolParam = true;  return XML_dataType(boolParam);}
			return XML_dataType(stringParam);
			}}
		}
	}

	Eptr->errorFlag = true;
	Eptr->errorMessage.append("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
	Eptr->errorMessage.append("XML_ParameterListArray Class Error \n");
	Eptr->errorMessage.append("Parameter Not found.\n");
	Eptr->errorMessage.append("Parameter List : ");
	Eptr->errorMessage.append(parameterListName);
	Eptr->errorMessage.append("\n");
	Eptr->errorMessage.append("Parameter      : ");
	Eptr->errorMessage.append(parameterName);
	Eptr->errorMessage.append("\n");

	return XML_dataType();
	}






//
// Interrogates string and returns one of
// bool, double, long, string
//
//
// Case independent versions of {true,yes} and {false,no}
// are returned as type bool
//
const char* getDataType(const std::string& sIn) const
{
return  getDataType(sIn.c_str());
}

const char* getDataType(const char* sIn) const
{
    //
    // remove white space before and after variables
    //
    std::string sInString(sIn);
    std::string sTrimmed(trim(sInString));
    const char* s   = sTrimmed.c_str();

    const char* boolType = "bool";

    std::string stringTemp(s);
	std::transform(stringTemp.begin(), stringTemp.end(),stringTemp.begin(), ::toupper);
	if(stringTemp.compare("FALSE") == 0)  return boolType;
	if(stringTemp.compare("TRUE")  == 0)  return boolType;
	if(stringTemp.compare("NO")    == 0)  return boolType;
	if(stringTemp.compare("YES")   == 0)  return boolType;

	int sLength   = (int)strlen(s);
	int firstChar = (int)s[0];
	int scndChar;
	int thrdChar;

	const char* doubleType = "double";
	const char* longType   = "long";
	const char* stringType = "string";

	//
	// check starting digit first 0-9 (48-57)
	//
	if((firstChar >= 48)&&(firstChar <= 57))
	{
	if(strchr(s,'.'))   {return doubleType;}
	if(strchr(s,'E'))   {return doubleType;}
	if(strchr(s,'e'))   {return doubleType;}
	else                {return longType;}
	}

	// check starting with (.)

	if(firstChar == 46)
	{
    if(sLength == 1)                        {return stringType;}
    scndChar = (int)s[1];
    if((scndChar >= 48)&&(scndChar <= 57))  {return doubleType;}
    else                                    {return stringType;}
	}

    //
	// check starting with a (+) (-)
	//

    if((firstChar == 43)||(firstChar == 45))
	{
	if(sLength == 1){return stringType;}

    scndChar = (int)s[1];
	if((scndChar >= 48)&&(scndChar <= 57))
	{
	if(strchr(s,'.'))   {return doubleType;}
	else                {return longType;}
	}

	if(scndChar == 46)
	{
    if(sLength == 2)                        {return stringType;}
    thrdChar = (int)s[2];
    if((thrdChar >= 48)&&(thrdChar <= 57))  {return doubleType;}
    else                                    {return stringType;}
	}
	}

	//
	// Check for string type
	//

	if((firstChar == 39)||(firstChar == 34)) {return stringType;}
	return stringType;
}


// trim from both ends
static inline std::string &trim(std::string &s) {
        return ltrim(rtrim(s));
}

// trim from start
/*
static inline std::string &ltrim(std::string &s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
        return s;
}
*/

static inline std::string& ltrim(std::string& s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(),
		([](int c) {return !std::isspace(c);} )  ));
	return s;
}

// trim from end

/*
static inline std::string &rtrim(std::string &s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
        return s;
}
*/
static inline std::string& rtrim(std::string& s) {
	s.erase(std::find_if(s.rbegin(), s.rend(), ([](int c) {return !std::isspace(c); })).base(), s.end());
	return s;
}


	public:

	TiXmlDocument* parameterArrayDocPtr;
	std::string            errorMessage;
	bool                      errorFlag;
	std::ostringstream     stringStream;
	bool               abortOnErrorFlag;

};

#endif

