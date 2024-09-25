/*
 * XML_ParameterListArray.h
 *
 *  Created on: Aug 29, 2024
 *      Author: anderson
 *
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * !!!! Alpha version based upon tinyxml2 !!!!
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 *
 */
#include <stdexcept>
#include <string>
#include <map>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <cstring>

#include "tinyxml2.h"
#include "XML_dataType.h"

#ifndef XML_PARAMETER_LIST_ARRAY_
#define XML_PARAMETER_LIST_ARRAY_

class XML_ParameterListArray
{
public:

	inline static std::string     errorMessage = "";
	inline static bool               errorFlag = false;
	inline static bool        abortOnErrorFlag = true;

public :

	tinyxml2::XMLDocument* parameterArrayDocPtr;
	std::string       parameterFileName;

public:

/// Null constructor
	XML_ParameterListArray()
	{
    parameterArrayDocPtr = nullptr;
	initialize();
	}

/// Copy constructor
	XML_ParameterListArray(const XML_ParameterListArray& P)
	{
    parameterArrayDocPtr  = nullptr;
    if(P.isNull()){initialize();return;}
    initialize(P);
	}

/// Destructor
	virtual ~XML_ParameterListArray()
	{
    if(parameterArrayDocPtr != nullptr) {delete parameterArrayDocPtr;}
	}

/**	Null constructor for an existing instance.
	The contents of the invoking instance are removed.
*/
	void initialize()
	{
    if(parameterArrayDocPtr != nullptr)
    {delete parameterArrayDocPtr;}

    parameterArrayDocPtr = nullptr;
    abortOnErrorFlag     = true;
    errorFlag            = false;
    errorMessage.clear();
    parameterFileName.clear();
	}

/**	Copy constructor for an existing instance.
	The contents of the invoking instance are removed.
*/
	void initialize(const XML_ParameterListArray& P)
	{
    initialize();
    if(P.isNull()){initialize();return;}

	parameterArrayDocPtr = new tinyxml2::XMLDocument();
	P.parameterArrayDocPtr->DeepCopy(parameterArrayDocPtr);
	}

/// Initializes an instance from the specified file
    void initalize(const std::string& fileName)
    {
    initialize(fileName.c_str());
    }

/// Initializes an instance from the specified file
	void initialize(const char* fileName)
	{
		initialize();
		FILE* dataFile;

#ifdef _MSC_VER
		if ((fopen_s(&dataFile,fileName, "r")) != 0)
#else
		if ((dataFile = fopen(fileName, "r")) == NULL)
#endif
		{
		errorFlag = true;
		errorMessage.append("\nXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
		errorMessage.append("XML_ParameterListArray Class Error \n");
		errorMessage.append("Error opening XML data file : \n\n");
		errorMessage.append(fileName);
		errorMessage.append("\n\n");
		errorMessage.append("File not found or file corrupted.\n");
		errorMessage.append("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
		}
        if(abortOnErrorFlag){checkErrorAndAbort();}

		if(not errorFlag)
		{
		fclose(dataFile);
		parameterArrayDocPtr = new tinyxml2::XMLDocument();
		if(parameterArrayDocPtr->LoadFile(fileName) != tinyxml2::XML_SUCCESS)
		{
		errorFlag = true;
		errorMessage.append("\nXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
		errorMessage.append("XML_ParameterListArray Class Error \n");
		errorMessage.append("Error loading XML data from file : \n\n");
		errorMessage.append(fileName);
		errorMessage.append("\n\n");
		errorMessage.append("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
		errorMessage.append("XML parsing error : \n");
		errorMessage.append(parameterArrayDocPtr->ErrorStr());
		errorMessage.append("\nXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
		}}

		if(abortOnErrorFlag){checkErrorAndAbort();}
		parameterFileName = std::string(fileName);
	}

/// Initialization from string
	void initializeFromString(const std::string& paramListAsString)
	{
	    this->initialize();
	    parameterArrayDocPtr = new tinyxml2::XMLDocument();
		if(parameterArrayDocPtr->Parse(paramListAsString.c_str()) != tinyxml2::XML_SUCCESS)
		{
		errorFlag = true;
		errorMessage.append("\nXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
		errorMessage.append("XML_ParameterListArray Class Error \n");
		errorMessage.append("\n");
		errorMessage.append("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
		errorMessage.append("XML parsing error : \n");
		errorMessage.append(parameterArrayDocPtr->ErrorStr());
		errorMessage.append("\nXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
		}

		if(abortOnErrorFlag){checkErrorAndAbort();}
		parameterFileName.clear();
	}

/// Stream output
	friend std::ostream&  operator <<(std::ostream& out_stream, const XML_ParameterListArray& P)
	{
	    tinyxml2::XMLPrinter printer;
		P.parameterArrayDocPtr->Print( &printer );
		out_stream << printer.CStr();
		return out_stream;
	}

/// Saves the XML file to specified file
	void saveFile(const std::string& fileName) const
	{
		parameterArrayDocPtr->SaveFile(fileName.c_str());
	}

/**	Creates a parameter list array in memory with a root node element with name listArrayName.

	The default parameterFileName data member is assigned the listArrayName with .xml
	appended.
 */
	void createParameterListArray(const std::string& listArrayName)
	{
       createParameterListArray(listArrayName.c_str());
	}

/**  Creates a parameter list array in memory with a root node element with name listArrayName.

 */
	void createParameterListArray(const char* listArrayName)
	{
    initialize();

    parameterArrayDocPtr = new tinyxml2::XMLDocument;
	tinyxml2::XMLDeclaration* DocDeclaration = parameterArrayDocPtr->NewDeclaration();
	parameterArrayDocPtr->InsertFirstChild(DocDeclaration);

    // Creates pointer to document and adds standard declaration

	tinyxml2::XMLComment* xmlComment = parameterArrayDocPtr->NewComment(" XML_ParameterListArray ");
	parameterArrayDocPtr->InsertEndChild( xmlComment );

    tinyxml2::XMLElement * root = parameterArrayDocPtr->NewElement(listArrayName);
	parameterArrayDocPtr->InsertEndChild( root );

    parameterFileName.clear();
    }

/// Extracts the file name used to initialize the instance (if initialize from a file).
	std::string getParameterFileName() const
	{
		return parameterFileName;
	}


/// Extracts the name of the root node; the name of the root node element.
    const char* getParameterListArrayName() const
    {
    abortIfNull();
    tinyxml2::XMLHandle  docHandle(parameterArrayDocPtr->RootElement());
	tinyxml2::XMLElement* root = docHandle.ToElement();
	return root->Value();
    }

/// Creates a duplicate instance
    void operator=(const XML_ParameterListArray& P)
    {
    initialize(P);
    }

/// Adds a comment as the next element in the XML file
    void addComment(const std::string& XMLcomment)
    {
        abortIfNull();
    	tinyxml2::XMLComment* xmlComment = parameterArrayDocPtr->NewComment(XMLcomment.c_str());
    	tinyxml2::XMLHandle  docHandle(parameterArrayDocPtr->RootElement());
	    tinyxml2::XMLElement* root = docHandle.ToElement();
        root->InsertEndChild(xmlComment);
    }

/// Adds a comment as the first element within a parameter list
    void addComment(const std::string& XMLcomment,const std::string& parameterListName)
    {
    	addComment(XMLcomment,parameterListName.c_str());
    }

/// Adds a comment as element within a parameter list
    void addComment(const std::string& XMLcomment,const char* parameterListName)
    {
    	abortIfNull();
    	tinyxml2::XMLHandle   docHandle(parameterArrayDocPtr->RootElement());
    	tinyxml2::XMLElement* parameterList = docHandle.FirstChildElement(parameterListName).ToElement();
    	checkParameterList(parameterList,parameterListName,"addComment(...)");
    	tinyxml2::XMLComment* xmlComment    = parameterArrayDocPtr->NewComment(XMLcomment.c_str());
        parameterList->InsertFirstChild(xmlComment);
    }

/// Adds a comment within parameter list for a specific parameter
    void addComment(const std::string& XMLcomment,const std::string&  parameterName,const std::string& parameterListName)
    {
    	addComment(XMLcomment,parameterName.c_str(),parameterListName.c_str());
    }

/// Adds a comment within parameter list for a specific parameter, just below parameter specifiction
    void addComment(const std::string& XMLcomment,const char* parameterName,const char* parameterListName)
    {
    	abortIfNull();
    	tinyxml2::XMLHandle   docHandle(parameterArrayDocPtr->RootElement());
    	tinyxml2::XMLElement* parameterList = docHandle.FirstChildElement(parameterListName).ToElement();
    	checkParameterList(parameterList,parameterListName,"addComment(...)");
    	tinyxml2::XMLElement* parameter     = parameterList->FirstChildElement(parameterName);
        checkParameter(parameter,parameterName,parameterListName,"addComment(...)");
    	tinyxml2::XMLComment* xmlComment    = parameterArrayDocPtr->NewComment(XMLcomment.c_str());
        parameterList->InsertAfterChild(parameter,xmlComment);
    }

/// Returns the number of parameter lists specified in the XML document
    size_t parameterListCount() const
	{
    if(isNull()) return 0;
	tinyxml2::XMLHandle docHandle(parameterArrayDocPtr->RootElement());
	tinyxml2::XMLElement*  parameterListArray = docHandle.ToElement();
	tinyxml2::XMLElement*  parameterList      = parameterListArray->FirstChildElement();
    if(parameterListArray == 0){return 0;}

	tinyxml2::XMLNode* node;
	size_t count = 0;
	for( node = parameterList; node; node = node->NextSibling())
	{
		if(not node->ToComment()){count++;}
	}
	return count;
	}

/// Returns the number of parameter lists specified in the XML document with the same name
    size_t parameterListInstanceCount(const std::string& parameterListName) const
    {
    return parameterListInstanceCount(parameterListName.c_str());
    }

/// Returns the number of parameter lists specified in the XML document with the same name
    size_t parameterListInstanceCount(const char* parameterListName) const
	{
	if(isNull()) return 0;
	tinyxml2::XMLHandle  docHandle(parameterArrayDocPtr->RootElement());
	tinyxml2::XMLElement* parameterList = docHandle.FirstChildElement(parameterListName).ToElement();

	if(parameterList == 0){return 0;}

	tinyxml2::XMLElement* node;
	size_t count = 0;
	for( node = parameterList;
		 node;
		 node = parameterList->NextSiblingElement(parameterListName))
	{
		if(not node->ToComment())
        {count++;}
	}
	return count;
	}

/// Determines if the specified parameter list exists in XML document
	bool isParameterList(const std::string& parameterListName) const
	{
	return isParameterList(parameterListName.c_str());
	}

/// Determines if the specified parameter list exists in XML document
	bool isParameterList(const char* parameterListName) const
	{
	if(parameterArrayDocPtr == 0) return 0;
	tinyxml2::XMLHandle  docHandle(parameterArrayDocPtr->RootElement());
	tinyxml2::XMLElement* parameterList = docHandle.FirstChildElement(parameterListName).ToElement();
	if(parameterList) {return true;}
	return false;
	}

/// Determines the number of instance of a parameter in a given parameter list
    size_t getParameterInstanceCount(const std::string& parameterName, const std::string& parameterListName) const
    {
    return getParameterInstanceCount(parameterName.c_str(),parameterListName.c_str());
    }

/// Determines the number of instance of a parameter in a given parameter list
	size_t getParameterInstanceCount(const char* parameterName, const char* parameterListName) const
	{
    if(isNull()) return 0;
	tinyxml2::XMLHandle  docHandle(parameterArrayDocPtr->RootElement());
	tinyxml2::XMLElement* parameterListElement = docHandle.FirstChildElement(parameterListName).ToElement();
    checkParameterList(parameterListElement,parameterListName,"getParameterInstanceCount(...)");

	tinyxml2::XMLElement* parameterElement = parameterListElement->FirstChildElement(parameterName);

	if(parameterElement == 0){return 0;}

	tinyxml2::XMLNode* node;
	size_t count = 0;
	for( node = parameterListElement->FirstChildElement(parameterName);
		 node;
		 node = node->NextSiblingElement(parameterName))
	{
	    count++;
	}
	return count;
	}

/// Determines if a parameter is with the specified parameter list
	bool isParameter(const std::string& parameterName, const std::string& parameterListName) const
	{
		return isParameter(parameterName.c_str(),parameterListName.c_str());
	}

/// Determines if a parameter is with the specified parameter list
	bool isParameter(const char* parameterName, const char* parameterListName) const
	{
    if(isNull()) return false;

	if(not isParameterList(parameterListName))
    {return false;}

	tinyxml2::XMLHandle  docHandle(parameterArrayDocPtr->RootElement());
	tinyxml2::XMLElement* parameter = docHandle.FirstChildElement(parameterListName).FirstChildElement(parameterName).ToElement();
	if(parameter) {return true;}
	return false;
	}

/// Inserts parameter list. An exception is thrown if list already exists.
    void addParameterList(const std::string& parameterListName)
    {
    addParameterList(parameterListName.c_str());
    }

/// Inserts parameter list. An exception is thrown if list already exists.
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

    tinyxml2::XMLHandle  docHandle(parameterArrayDocPtr->RootElement());
	tinyxml2::XMLElement* root = docHandle.ToElement();
    tinyxml2::XMLElement* parameterListElement = parameterArrayDocPtr->NewElement(parameterListName);
    root->InsertEndChild( parameterListElement );
    }

/// Inserts parameter and sets value. No type specification is inserted.
	void addParameter(const std::string&  parameterName, const std::string& parameterListName)
    {
    addParameter(parameterName.c_str(), parameterListName.c_str());
    }

/// Inserts parameter and sets value. No type specification is inserted.
    void addParameter(const char* parameterName, const char* parameterListName)
    {
    abortIfNull();
    tinyxml2::XMLHandle  docHandle(parameterArrayDocPtr->RootElement());
    tinyxml2::XMLElement* parameterListElement = docHandle.FirstChildElement(parameterListName).ToElement();
    checkParameterList(parameterListElement,parameterListName,"void addParameter(...)");
	tinyxml2::XMLElement* parameterElement     = parameterArrayDocPtr->NewElement(parameterName);
    parameterListElement->InsertEndChild( parameterElement );
    }

/// Inserts parameter and sets value. No type specification is inserted.
    void addParameterNoTypeSpec(const std::string& value, const std::string& parameterName, const std::string& parameterListName)
    {
    addParameterNoTypeSpec(value.c_str(),parameterName.c_str(), parameterListName.c_str());
    }

/// Inserts parameter and sets value. No type specification is inserted.
    void addParameterNoTypeSpec(const char* value, const char* parameterName, const char* parameterListName)
    {
    abortIfNull();
    tinyxml2::XMLHandle   docHandle(parameterArrayDocPtr->RootElement());
	tinyxml2::XMLElement* parameterListElement    = docHandle.FirstChildElement(parameterListName).ToElement();
	checkParameterList(parameterListElement,parameterListName,"void addParameterNoTypeSpec(...)");
	tinyxml2::XMLElement* parameterElement        = parameterArrayDocPtr->NewElement(parameterName);
    parameterListElement->LinkEndChild( parameterElement );

    parameterElement->SetAttribute("value",value);
    }

/** Inserts parameter with type specification deduced from value type and sets value.
    Input float values are automatically promoted to type double andinput int values
    are automatically promoted to type long.
*/
    void addParameter(XML_dataType value, const std::string& parameterName, const std::string& parameterListName)
    {
        addParameter(value, parameterName.c_str(), parameterListName.c_str());
    }

/** Inserts parameter with type specification deduced from value type and sets value.
    Input float values are automatically promoted to type double andinput int values
    are automatically promoted to type long.
*/
    void addParameter(XML_dataType value, const char* parameterName, const char* parameterListName)
    {
    abortIfNull();
    tinyxml2::XMLHandle  docHandle(parameterArrayDocPtr->RootElement());
	tinyxml2::XMLElement* parameterListElement    = docHandle.FirstChildElement(parameterListName).ToElement();
	checkParameterList(parameterListElement,parameterListName,"void addParameter(...)");
	tinyxml2::XMLElement* parameterElement        = parameterArrayDocPtr->NewElement(parameterName);
    parameterListElement->LinkEndChild( parameterElement );

    parameterElement->SetAttribute("value", value.toString().c_str());
    if(value.isString()){parameterElement->SetAttribute("type","string");}
    else
    {parameterElement->SetAttribute("type",getDataType(value.toString().c_str()));}
    }

/// Insert parameter child without type specification. No value specified.
    void addParameterChild(const std::string& parameterChildName, const std::string& parameterName, const std::string& parameterListName)
    {
        addParameterChild(parameterChildName.c_str(),parameterName.c_str(), parameterListName.c_str());
    }

/// Insert parameter child without type specification. No value specified.
    void addParameterChild(const char* parameterChildName, const char* parameterName, const char* parameterListName)
    {
    abortIfNull();

    tinyxml2::XMLHandle  docHandle(parameterArrayDocPtr->RootElement());
    tinyxml2::XMLElement* parameterListElement  = docHandle.FirstChildElement(parameterListName).ToElement();
    checkParameterList(parameterListElement,parameterListName,"addParameterChild(...)");

	tinyxml2::XMLElement* parameter             = parameterListElement->FirstChildElement(parameterName);
	checkParameter(parameter,parameterName,parameterListName,"addParameterChild(...)");

	tinyxml2::XMLElement* parameterChild     = parameterArrayDocPtr->NewElement(parameterChildName);
    parameter->LinkEndChild( parameterChild );
    }

/** Inserts child parameter (sub-parameter) with type specification deduced from value type and sets value.
    Input float values are automatically promoted to type double andinput int values
    are automatically promoted to type long.
*/
    void addParameterChild(XML_dataType value, const std::string& parameterChildName,
    const std::string& parameterName, const std::string& parameterListName)
    {
    addParameterChild(value, parameterChildName.c_str(),parameterName.c_str(), parameterListName.c_str());
    }

/** Inserts child parameter (sub-parameter) with type specification deduced from value type and sets value.
    Input float values are automatically promoted to type double andinput int values
    are automatically promoted to type long.
*/
    void addParameterChild(XML_dataType value, const char* parameterChildName,
    const char* parameterName, const char* parameterListName)
    {
    abortIfNull();

    tinyxml2::XMLHandle  docHandle(parameterArrayDocPtr->RootElement());
    tinyxml2::XMLElement* parameterListElement  = docHandle.FirstChildElement(parameterListName).ToElement();
    checkParameterList(parameterListElement,parameterListName,"addParameterChild(...)");

	tinyxml2::XMLElement* parameter             = parameterListElement->FirstChildElement(parameterName);
	checkParameter(parameter,parameterName,parameterListName,"addParameterChild(...)");


	tinyxml2::XMLElement* parameterChild        = parameterArrayDocPtr->NewElement(parameterChildName);
    parameter->LinkEndChild( parameterChild );
    parameterChild->SetAttribute("value", value.toString().c_str());
    if(value.isString())
    {parameterChild->SetAttribute("type","string");}
    else
    {parameterChild->SetAttribute("type",getDataType(value.toString().c_str()));}
    }

/// Inserts parameter child (a sub-parameter) sets value with no explicity type specification.
    void addParameterChildNoTypeSpec(XML_dataType value, const std::string& parameterChildName,
    const std::string& parameterName, const std::string& parameterListName)
    {
    addParameterChildNoTypeSpec(value,parameterChildName.c_str(),parameterName.c_str(), parameterListName.c_str());
    }

/// Inserts parameter child (a sub-parameter) sets value with no explicity type specification.
    void addParameterChildNoTypeSpec(XML_dataType value, const char* parameterChildName,
    const char* parameterName, const char* parameterListName)
    {
    abortIfNull();

    tinyxml2::XMLHandle  docHandle(parameterArrayDocPtr->RootElement());
    tinyxml2::XMLElement* parameterListElement  = docHandle.FirstChildElement(parameterListName).ToElement();
    checkParameterList(parameterListElement,parameterListName,"addParameterChild(...)");

	tinyxml2::XMLElement* parameter             = parameterListElement->FirstChildElement(parameterName);
	checkParameter(parameter,parameterName,parameterListName,"addParameterChild(...)");

    tinyxml2::XMLElement* parameterChild     = parameterArrayDocPtr->NewElement(parameterChildName);
    parameter->LinkEndChild( parameterChild );
    parameterChild->SetAttribute("value", value.toString().c_str());
    }

/** Adds a child parameter (sub-parameter) to the specified instance of the parameter with type specification
    deduced from value type and sets value.

    Input float values are automatically promoted to type double andinput int values
    are automatically promoted to type long.

    If the child parameter already exists, an additional child instance is still added.
*/
    void addParameterInstanceChild(XML_dataType value, size_t instanceIndex, const std::string& parameterChildName,
    const std::string& parameterName, const std::string& parameterListName)
    {
    addParameterInstanceChild(value, instanceIndex,  parameterChildName.c_str(), parameterName.c_str(),parameterListName.c_str());
    }

/** Adds a child parameter (sub-parameter) to the specified instance of the parameter with type specification
    deduced from value type and sets value.

    Input float values are automatically promoted to type double andinput int values
    are automatically promoted to type long.

    If the child parameter already exists, an additional child instance is still added.
*/
    void addParameterInstanceChild(XML_dataType value, size_t instanceIndex, const char* parameterChildName,
    const char* parameterName, const char* parameterListName)
    {
    abortIfNull();

	tinyxml2::XMLHandle  docHandle(parameterArrayDocPtr->RootElement());
    tinyxml2::XMLElement* parameterListElement  = docHandle.FirstChildElement(parameterListName).ToElement();
    checkParameterList(parameterListElement,parameterListName,"addParameterInstanceChild(...)");

	tinyxml2::XMLElement* parameterElement      = parameterListElement->FirstChildElement(parameterName);
	checkParameter(parameterElement,parameterName,parameterListName,"addParameterInstanceChild(...)");

	checkInstanceIndex(instanceIndex,parameterName,parameterListName,"addParameterInstanceChild(...)");

    tinyxml2::XMLElement* parameterChild     = parameterArrayDocPtr->NewElement(parameterChildName);
	tinyxml2::XMLNode*    node;

	size_t count = 0;
	for( node = parameterListElement->FirstChildElement(parameterName);
		 node;
		 node = node->NextSiblingElement(parameterName))
	{
		if(count == instanceIndex)
		{
        node->ToElement()->LinkEndChild(parameterChild);
        parameterChild->SetAttribute("value", value.toString().c_str());
		if(value.isString())
		{parameterChild->SetAttribute("type","string");}
		else
		{parameterChild->SetAttribute("type",getDataType(value.toString().c_str()));}
		}
		count++;
	}


    }

    ///////////////////////////////////////////////////////////////////////////////////////////////
    //                Member functions for setting parameter values
    ///////////////////////////////////////////////////////////////////////////////////////////////
/** Sets the specified parameter value.

	If the parameter value is of integer type then specifying a double value will result
	in the integral part of the value being stored.
 */
    void setParameterValue(const XML_dataType& value, const std::string& parameterName, const std::string& parameterListName)
    {
    setParameterValue(value, parameterName.c_str(), parameterListName.c_str());
    }

/** Sets the specified parameter value.

	If the parameter value is of integer type then specifying a double value will result
	in the integral part of the value being stored.
 */
    void setParameterValue(const XML_dataType& value, const char* parameterName, const char* parameterListName)
    {
    abortIfNull();

	tinyxml2::XMLHandle  docHandle(parameterArrayDocPtr->RootElement());
    tinyxml2::XMLElement* parameterListElement  = docHandle.FirstChildElement(parameterListName).ToElement();
    checkParameterList(parameterListElement,parameterListName,"setParameterValue(...)");

	tinyxml2::XMLElement* parameterElement      = parameterListElement->FirstChildElement(parameterName);
	checkParameter(parameterElement,parameterName,parameterListName,"setParameterValue(...)");

	setParameterValue(value,  parameterElement);
    }

/**Sets value of specified instance of the parameter.
   Instance indexing starts at 0.
*/
    void setParameterInstanceValue(const XML_dataType& value, size_t instanceIndex,
    const std::string& parameterName, const std::string& parameterListName)
	{
	return setParameterInstanceValue(value, instanceIndex, parameterName.c_str(), parameterListName.c_str());
	}

/**Returns value of specified instance of the parameter.
   Instance indexing starts at 0.
*/
	void setParameterInstanceValue(const XML_dataType& value, size_t instanceIndex,const char* parameterName,
	const char* parameterListName)
	{
    abortIfNull();
	tinyxml2::XMLHandle  docHandle(parameterArrayDocPtr->RootElement());
    tinyxml2::XMLElement* parameterListElement  = docHandle.FirstChildElement(parameterListName).ToElement();
    checkParameterList(parameterListElement,parameterListName,"setParameterInstanceValue(...)");

	tinyxml2::XMLElement* parameterElement      = parameterListElement->FirstChildElement(parameterName);
	checkParameter(parameterElement,parameterName,parameterListName,"setParameterInstanceValue(...)");

	checkInstanceIndex(instanceIndex,parameterName,parameterListName,"setParameterInstanceValue(...)");

    XML_dataType returnValue;
	tinyxml2::XMLNode* node;

	size_t count = 0;
	for( node = parameterListElement->FirstChildElement(parameterName);
		 node;
		 node = node->NextSiblingElement(parameterName))
	{
		if(count == instanceIndex)
		{
			if(node->ToElement())
			{
			setParameterValue(value,node->ToElement());
			return;
			}
		}
		count++;

	}
	}

/**Sets the value of specified instance of the parameter.
*/
    void setParameterInstanceValue(const XML_dataType& value, const std::string& instanceName,
    const std::string& parameterName, const std::string& parameterListName)
	{
	return setParameterInstanceValue(value,instanceName.c_str(), parameterName.c_str(), parameterListName.c_str());
	}

/**Sets value of specified instance of the parameter.
*/
	void setParameterInstanceValue(const XML_dataType& value, const char* instanceName,
	const char* parameterName, const char* parameterListName)
	{
    abortIfNull();
	tinyxml2::XMLHandle  docHandle(parameterArrayDocPtr->RootElement());
    tinyxml2::XMLElement* parameterListElement  = docHandle.FirstChildElement(parameterListName).ToElement();
    checkParameterList(parameterListElement,parameterListName,"getParameterInstanceValue(...)");

	tinyxml2::XMLElement* parameterElement      = parameterListElement->FirstChildElement(parameterName);
	checkParameter(parameterElement,parameterName,parameterListName,"getParameterInstanceValue(...)");

    XML_dataType returnValue;

	tinyxml2::XMLNode*    node;
	tinyxml2::XMLElement* parameterChild;

    const char* p = nullptr;
	const char** stringData = &p;
	std::string instanceString(instanceName);
	std::string indexString;

	for( node = parameterListElement->FirstChildElement(parameterName);
		 node;
		 node = node->NextSiblingElement(parameterName) )
	{
		if(node->FirstChildElement("name"))
		{
		    parameterChild = node->FirstChildElement("name")->ToElement();
			indexString.clear();
		    if(parameterChild->QueryStringAttribute("value",stringData) == tinyxml2::XML_SUCCESS){indexString = stringData[0];}
		    if(instanceString.compare(indexString) == 0)
		    {
		    setParameterValue(value, node->ToElement());
		    return;
		    }
		}

	}

    // Error if value was not set

	errorFlag   = true;
	errorMessage.append("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
	errorMessage.append("XML_ParameterListArray Class Error \n");
	errorMessage.append("    getParameterInstanceValue(...)\n");
	errorMessage.append("Parameter instance does not exist.\n");
	errorMessage.append("ParameterList           : ");
	errorMessage.append(parameterListName);
	errorMessage.append("\nParameter               : ");
	errorMessage.append(parameterName);
	errorMessage.append("\nParameter instance Name : ");
	errorMessage.append(instanceString);
	errorMessage.append("Parameter Instance : ");

	if(abortOnErrorFlag){checkErrorAndAbort();}
	}

/** Sets the specified string parameter value.

	This member function avoids the creation of a copy of the string before insertion into the XML data structure.
 */
    void setStringParameterValue(const std::string& value, const std::string& parameterName, const std::string& parameterListName)
    {
    setStringParameterValue(value, parameterName.c_str(), parameterListName.c_str());
    }

/** Sets the specified string parameter value.

	This member function avoids the creation of a copy of the value string before insertion into the XML data structure
 */
    void setStringParameterValue(const std::string& value, const char* parameterName, const char* parameterListName)
    {
    abortIfNull();
	tinyxml2::XMLHandle  docHandle(parameterArrayDocPtr->RootElement());
    tinyxml2::XMLElement* parameterListElement  = docHandle.FirstChildElement(parameterListName).ToElement();
    checkParameterList(parameterListElement,parameterListName,"setStringParameterValue(...)");

	tinyxml2::XMLElement* parameterElement      = parameterListElement->FirstChildElement(parameterName);
	checkParameter(parameterElement,parameterName,parameterListName,"setStringParameterValue(...)");

    const char* inputDataType = "string";
    const char* dataType      = parameterElement->Attribute("type");

    // Set data type to be string if parameter is untyped

	if(dataType == 0)
	{
	dataType = "string";
	parameterElement->SetAttribute("type",dataType);
    }

    std::string stringTemp;
    if(strcmp(dataType,inputDataType) != 0)
    {
    errorFlag = true;
	errorMessage.append("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
	errorMessage.append("XML_ParameterListArray Class Error \n");
	errorMessage.append("setStringParameterValue(...)\n\n");
	errorMessage.append("Parameter being set is not of string type.\n\n");
	errorMessage.append("Parameter     : ");
	errorMessage.append(parameterName);
    errorMessage.append("\nParameterList : ");
	errorMessage.append(parameterListName);
	errorMessage.append("\nXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
	errorMessage.append("\n");
    }
    else
    {
    parameterElement->SetAttribute("value", value.c_str());
    }

    if(abortOnErrorFlag){checkErrorAndAbort();}
    }

/** Deprecated : use setParameterValue(...) instead
    Sets the specified parameter value.

	If the parameter value is of integer type then specifying a double value will result
	in the integral part of the value being stored.
 */
    void setParameter(const XML_dataType& value, const std::string& parameterName, const std::string& parameterListName)
    {
    setParameterValue(value, parameterName.c_str(), parameterListName.c_str());
    }

/** Deprecated : use setParameterValue(...) instead
    Sets the specified parameter value.

	If the parameter value is of integer type then specifying a double value will result
	in the integral part of the value being stored.
 */
    void setParameter(const XML_dataType& value, const char* parameterName, const char* parameterListName)
    {
    setParameterValue(value,parameterName,parameterListName);
    }

/** Sets the type attributed of the specified parameter. The stored value is not altered.
 */
    void setParameterType(const std::string& typeValue, const std::string& parameterName, const std::string& parameterListName)
    {
    setParameterType(typeValue.c_str(),parameterName.c_str(), parameterListName.c_str());
    }

/** Sets the type attributed of the specified parameter. The stored value is not altered.
 */
    void setParameterType(const char* typeValue, const char* parameterName, const char* parameterListName)
    {
    abortIfNull();
	tinyxml2::XMLHandle  docHandle(parameterArrayDocPtr->RootElement());
    tinyxml2::XMLElement* parameterListElement  = docHandle.FirstChildElement(parameterListName).ToElement();
    checkParameterList(parameterListElement,parameterListName,"setParameterType(...)");

	tinyxml2::XMLElement* parameterElement      = parameterListElement->FirstChildElement(parameterName);
	checkParameter(parameterElement,parameterName,parameterListName,"setParameterType(...)");

	parameterElement->SetAttribute("type",typeValue);
    }

/// Sets the value of the child parameter of the parameter specified by parameterName and parameterListName
    void setParameterChildValue(XML_dataType value,const std::string& childParameter,
    const std::string& parameterName, const std::string& parameterListName)
    {
    setParameterChildValue(value,childParameter.c_str(), parameterName.c_str(), parameterListName.c_str());
    }

/// Sets the value of the child parameter of the parameter specified by parameterName and parameterListName
    void setParameterChildValue(const XML_dataType& value,const char* childParameter,
    const char* parameterName, const char* parameterListName)
    {
    abortIfNull();
	tinyxml2::XMLHandle  docHandle(parameterArrayDocPtr->RootElement());
    tinyxml2::XMLElement* parameterListElement  = docHandle.FirstChildElement(parameterListName).ToElement();
    checkParameterList(parameterListElement,parameterListName,"setParameterChildValue(...)");

	tinyxml2::XMLElement* parameterElement      = parameterListElement->FirstChildElement(parameterName);
	checkParameter(parameterElement,parameterName,parameterListName,"setParameterChildValue(...)");

	tinyxml2::XMLElement* parameterChildElement  = parameterElement->FirstChildElement(childParameter);
	checkParameterChild(parameterChildElement,childParameter,parameterName,parameterListName,"setParameterChildValue(...)");

    const char* inputDataType;
    const char* dataType;

	// Force a data conversion if necessary

    inputDataType = getDataType(value.toString().c_str());
    dataType      = parameterChildElement->Attribute("type");

    if(dataType == 0)
	{
	dataType = getDataType(parameterChildElement->Attribute("value"));
	parameterChildElement->SetAttribute("type",dataType);
    }

    std::string stringTemp;

    if((dataType != 0)&&(strcmp(dataType,inputDataType) != 0))
    {
    if(strcmp(dataType,"string") == 0) parameterChildElement->SetAttribute("value", XML_dataType(std::string(value)).toString().c_str());
    if(strcmp(dataType,"bool") == 0)   parameterChildElement->SetAttribute("value", XML_dataType(bool(value)).toString().c_str());
    if(strcmp(dataType,"long") == 0)   parameterChildElement->SetAttribute("value", XML_dataType(long(value)).toString().c_str());
    if(strcmp(dataType,"int") == 0)    parameterChildElement->SetAttribute("value", XML_dataType(int(value)).toString().c_str());
    if(strcmp(dataType,"float") == 0)  parameterChildElement->SetAttribute("value", XML_dataType(float(value)).toString().c_str());
    if(strcmp(dataType,"double") == 0) parameterChildElement->SetAttribute("value", XML_dataType(double(value)).toString().c_str());
    if((strcmp(dataType,"bool") == 0)&&((strcmp(inputDataType,"string"))==0))
    {
    stringTemp = XML_dataType(std::string(value)).toString();;
	std::transform(stringTemp.begin(), stringTemp.end(), stringTemp.begin(), [](unsigned char c) {return  static_cast<char>(std::toupper(c)); });
	if(stringTemp.compare("FALSE") == 0)  parameterChildElement->SetAttribute("value","false");
    if(stringTemp.compare("TRUE")  == 0)  parameterChildElement->SetAttribute("value","true");
    if(stringTemp.compare("NO")    == 0)  parameterChildElement->SetAttribute("value","false");
	if(stringTemp.compare("YES")   == 0)  parameterChildElement->SetAttribute("value","true");
    }
    }
    else
    {
    parameterChildElement->SetAttribute("value", value.toString().c_str());

    if(dataType == 0)
    {
    if(value.isString())
	{parameterChildElement->SetAttribute("type","string");}
	else
	{parameterChildElement->SetAttribute("type",getDataType(value.toString().c_str()));}
	}

    }}

/** Sets the value of the child of the specified instance of the parameter.
    Instance indexing starts at 0.
*/
    void setParameterInstanceChildValue(const XML_dataType& value,int instanceIndex,const std::string& childParameter,
    const std::string& parameterName, const std::string& parameterListName)
    {
    setParameterInstanceChildValue(value,instanceIndex,childParameter.c_str(),parameterName.c_str(), parameterListName.c_str());
    }

/** Sets the value of the child of the specified instance of the parameter.
    Instance indexing starts at 0.
*/
    void setParameterInstanceChildValue(const XML_dataType& value,int instanceIndex,const char* childParameter,
    const char* parameterName, const char* parameterListName)
    {
    abortIfNull();
	tinyxml2::XMLHandle  docHandle(parameterArrayDocPtr->RootElement());
    tinyxml2::XMLElement* parameterListElement  = docHandle.FirstChildElement(parameterListName).ToElement();
    checkParameterList(parameterListElement,parameterListName,"setParameterInstanceChildValue(...)");

	tinyxml2::XMLElement* parameterElement      = parameterListElement->FirstChildElement(parameterName);
	checkParameter(parameterElement,parameterName,parameterListName,"setParameterInstanceChildValue(...)");

	checkInstanceIndex(instanceIndex,parameterName,parameterListName,"setParameterInstanceValue(...)");

	tinyxml2::XMLNode* node;

	long count = 0;
	for( node = parameterListElement->FirstChildElement(parameterName);
		 node;
		 node = node->NextSiblingElement(parameterName) )
	{
		if(count == instanceIndex)
		{
			if(node->FirstChildElement(childParameter))
			{
			setParameterValue(value, node->FirstChildElement(childParameter)->ToElement());
			return;
			}
        }
        count++;
    }

	errorFlag = true;
	errorMessage.append("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
	errorMessage.append("XML_ParameterListArray Class Error \n");
	errorMessage.append("void setParameterInstanceChildValue(...)\n");
	errorMessage.append("Parameter child value specified by childParameter does not exist.\n\n");
	errorMessage.append("ParameterList   : ");
	errorMessage.append(parameterListName);
	errorMessage.append("\nParameter       : ");
	errorMessage.append(parameterName);
	errorMessage.append("\nInstance index  : ");
	errorMessage.append(std::to_string(instanceIndex));
	errorMessage.append("\nParameter child : ");
	errorMessage.append(childParameter);
	errorMessage.append("\nXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
	errorMessage.append("\n");

	if(abortOnErrorFlag){checkErrorAndAbort();}
	}

/** Sets the value of the specified child of an instance of a parameter.
    The parameter instance is identified by the value of the <name> child
    of the parameter.
*/
	void setParameterInstanceChildValue(const XML_dataType& value,const std::string& instanceName,const std::string& childParameter,
    const std::string& parameterName, const std::string& parameterListName)
    {
    setParameterInstanceChildValue(value,instanceName.c_str(),childParameter.c_str(),
    parameterName.c_str(), parameterListName.c_str());
    }

/** Sets the value of the specified child of an instance of a parameter.
    The parameter instance is identified by the value of the <name> child
    of the parameter.
*/
	void setParameterInstanceChildValue(const XML_dataType& value,const char* instanceName,const char* childParameter,
    const char* parameterName, const char* parameterListName)
    {
    abortIfNull();
	tinyxml2::XMLHandle  docHandle(parameterArrayDocPtr->RootElement());
    tinyxml2::XMLElement* parameterListElement  = docHandle.FirstChildElement(parameterListName).ToElement();
    checkParameterList(parameterListElement,parameterListName,"setParameterInstanceChildValue(...)");

	tinyxml2::XMLElement* parameterElement      = parameterListElement->FirstChildElement(parameterName);
	checkParameter(parameterElement,parameterName,parameterListName,"setParameterInstanceChildValue(...)");


    tinyxml2::XMLElement* parameterChild;
	tinyxml2::XMLNode*    node;


    std::string instanceString(instanceName);
    std::string indexString;

    const char*p = nullptr;
    const char** stringData = &p;

	for( node = parameterListElement->FirstChildElement(parameterName);
		 node;
		 node = node->NextSiblingElement(parameterName) )
	{
		if(node->FirstChildElement("name"))
		{
		    parameterChild = node->FirstChildElement("name")->ToElement();
		    indexString.clear();
		    if(parameterChild->QueryStringAttribute("value",stringData) == tinyxml2::XML_SUCCESS){indexString = stringData[0];}

		    if(std::string(indexString).compare(instanceString) == 0)
		    {
		    	if(node->FirstChildElement(childParameter))
		    	{
		    		setParameterValue(value, node->FirstChildElement(childParameter)->ToElement());
		    		return;
		    	}
		    }
		}
     }
	errorFlag = true;
	errorMessage.append("\nXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
	errorMessage.append("XML_ParameterListArray Class Error \n");
	errorMessage.append("void setParameterInstanceChildValue(...)\n");
	errorMessage.append("Parameter child value specified by childParameter does not exist.\n\n");
	errorMessage.append("ParameterList           : ");
	errorMessage.append(parameterListName);
	errorMessage.append("\nParameter               : ");
	errorMessage.append(parameterName);
	errorMessage.append("\nParameter instance Name : ");
	errorMessage.append(instanceString);
	errorMessage.append("\nParameter child         : ");
	errorMessage.append(childParameter);
	errorMessage.append("\nXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
	errorMessage.append("\n");

	if(abortOnErrorFlag){checkErrorAndAbort();}
	}

/// Sets the corresponding parameter if it exists, otherwise a null operation.
    void setParameterOrIgnore(const XML_dataType& value, const std::string& parameterName, std::string& parameterListName)
    {
        setParameterOrIgnore(value,  parameterName.c_str(), parameterListName.c_str());
    }

/// Sets the corresponding parameter if it exists, otherwise a null operation.
    void setParameterOrIgnore(const XML_dataType& value, const char* parameterName, const char* parameterListName)
    {
    	if(this->isParameter(parameterName, parameterListName))
    	{
    		this->setParameter(value,parameterName, parameterListName);
    		return;
    	}
    	return;
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////
    //                Member functions for getting parameter values
    ///////////////////////////////////////////////////////////////////////////////////////////////

/** Returns the specified parameter value as an XML_dataType.

    Automatic conversion to standard data types occurs on assignment,
    except for string data.

    String data requires an explicit case before assignment.
*/
	XML_dataType getParameterValue(const std::string& parameterName, const std::string& parameterListName) const
	{
	return getParameterValue(parameterName.c_str(),parameterListName.c_str());
	}

/** Returns the specified parameter value as an XML_dataType.

    Automatic conversion to standard data types occurs on assignment,
    except for string data.

    String data requires an explicit case before assignment.
*/
	XML_dataType getParameterValue(const char* parameterName, const char* parameterListName) const
	{
	abortIfNull();
	tinyxml2::XMLHandle  docHandle(parameterArrayDocPtr->RootElement());
    tinyxml2::XMLElement* parameterListElement  = docHandle.FirstChildElement(parameterListName).ToElement();
    checkParameterList(parameterListElement,parameterListName,"getParameterValue(...)");

	tinyxml2::XMLElement* parameterElement      = parameterListElement->FirstChildElement(parameterName);
	checkParameter(parameterElement,parameterName,parameterListName,"getParameterValue(...)");

	XML_dataType returnValue;
	returnValue = getParameterValue(parameterElement, parameterName,  parameterListName);

	if(abortOnErrorFlag){checkErrorAndAbort();}
	return returnValue;
	}

/** Sets the specified string parameter value.

	This member function avoids the creation of a copy of the string when extracting from the XML data
 */
    std::string getStringParameterValue(const std::string& parameterName, const std::string& parameterListName)
    {
    return getStringParameterValue(parameterName.c_str(), parameterListName.c_str());
    }

/** Sets the specified string parameter value.

	This member function avoids the creation of a copy of the value string before insertion into the XML data structure
 */
    std::string getStringParameterValue(const char* parameterName, const char* parameterListName)
    {
    abortIfNull();
	tinyxml2::XMLHandle  docHandle(parameterArrayDocPtr->RootElement());
    tinyxml2::XMLElement* parameterListElement  = docHandle.FirstChildElement(parameterListName).ToElement();
    checkParameterList(parameterListElement,parameterListName,"setStringParameterValue(...)");

	tinyxml2::XMLElement* parameterElement      = parameterListElement->FirstChildElement(parameterName);
	checkParameter(parameterElement,parameterName,parameterListName,"setStringParameterValue(...)");

    const char* inputDataType = "string";
    const char* dataType      = parameterElement->Attribute("type");

    // Set data type to be string if parameter is untyped

	if(dataType == 0)
	{
	dataType = "string";
	parameterElement->SetAttribute("type",dataType);
    }

    const char* p           = nullptr;
    const char** stringData = &p;
    if(strcmp(dataType,inputDataType) != 0)
    {
    errorFlag = true;
	errorMessage.append("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
	errorMessage.append("XML_ParameterListArray Class Error \n");
	errorMessage.append("getStringParameterValue(...)\n\n");
	errorMessage.append("Parameter being extracted is not of string type.\n\n");
	errorMessage.append("Parameter     : ");
	errorMessage.append(parameterName);
    errorMessage.append("\nParameterList : ");
	errorMessage.append(parameterListName);
	errorMessage.append("\nXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
	errorMessage.append("\n");
    }

	if(parameterElement->QueryStringAttribute("value", stringData) == tinyxml2::XML_SUCCESS)
	{
		return stringData[0];
	}

    errorFlag = true;
	errorMessage.append("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
	errorMessage.append("XML_ParameterListArray Class Error \n");
	errorMessage.append("getStringParameterValue(...)\n\n");
	errorMessage.append("Error reading string data.\n\n");
	errorMessage.append("Parameter     : ");
	errorMessage.append(parameterName);
    errorMessage.append("\nParameterList : ");
	errorMessage.append(parameterListName);
	errorMessage.append("\nXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
	errorMessage.append("\n");

    if(abortOnErrorFlag){checkErrorAndAbort();}
    return stringData[0];
    }

/** This routine returns the parameter value as an XML_dataType if the parameter exists,
    otherwise it returns the defaultValue specified.

    Automatic conversion to standard data types occurs on assignment,
    except for string data.

    String data requires an explicit case before assignment.
*/
   XML_dataType getParameterValueOrDefault(const std::string& parameterName, const std::string& parameterListName,XML_dataType defaultValue) const
   {
   return getParameterValueOrDefault(parameterName.c_str(), parameterListName.c_str(),defaultValue);
   }

/** This routine returns the parameter value as an XML_dataType if the parameter exists,
    otherwise it returns the defaultValue specified.

    Automatic conversion to standard data types occurs on assignment,
    except for string data.

    String data requires an explicit case before assignment.
*/
   XML_dataType getParameterValueOrDefault(const char* parameterName, const char* parameterListName, XML_dataType defaultValue ) const
   {

   if(this->isParameter(parameterName,parameterListName))
   {
    return this->getParameterValue(parameterName,parameterListName);
   }
   return defaultValue;
   }

/**Returns value of specified instance of the parameter.
   Instance indexing starts at 0.
*/
    XML_dataType getParameterInstanceValue(const std::string& instanceName,const std::string& parameterName, const std::string& parameterListName) const
	{
	return getParameterInstanceValue(instanceName.c_str(), parameterName.c_str(), parameterListName.c_str());
	}

/**Returns value of specified instance of the parameter.
*/
	XML_dataType getParameterInstanceValue(const char* instanceName,const char* parameterName, const char* parameterListName) const
	{
    abortIfNull();
	tinyxml2::XMLHandle  docHandle(parameterArrayDocPtr->RootElement());
    tinyxml2::XMLElement* parameterListElement  = docHandle.FirstChildElement(parameterListName).ToElement();
    checkParameterList(parameterListElement,parameterListName,"getParameterInstanceValue(...)");

	tinyxml2::XMLElement* parameterElement      = parameterListElement->FirstChildElement(parameterName);
	checkParameter(parameterElement,parameterName,parameterListName,"getParameterInstanceValue(...)");

    XML_dataType returnValue;

	tinyxml2::XMLNode*    node;
	tinyxml2::XMLElement* parameterChild;


    const char* p = nullptr;
	const char** stringData = &p;
	std::string instanceString(instanceName);
	std::string indexString;
	for( node = parameterListElement->FirstChildElement(parameterName);
		 node;
		 node = node->NextSiblingElement(parameterName) )
	{
		if(node->FirstChildElement("name"))
		{
		    parameterChild = node->FirstChildElement("name")->ToElement();
			indexString.clear();
		    if(parameterChild->QueryStringAttribute("value",stringData) == tinyxml2::XML_SUCCESS){indexString = stringData[0];}
		    if(instanceString.compare(indexString) == 0)
		    {
		    return returnValue = getParameterValue(node->ToElement(),parameterName,  parameterListName);
		    }
		}
	}
	errorFlag   = true;
	errorMessage.append("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
	errorMessage.append("XML_ParameterListArray Class Error \n");
	errorMessage.append("    getParameterInstanceValue(...)\n");
	errorMessage.append("Parameter instance does not exist.\n");
	errorMessage.append("ParameterList           : ");
	errorMessage.append(parameterListName);
	errorMessage.append("\nParameter               : ");
	errorMessage.append(parameterName);
	errorMessage.append("\nParameter instance Name : ");
	errorMessage.append(instanceString);
	errorMessage.append("Parameter Instance : ");

	if(abortOnErrorFlag){checkErrorAndAbort();}
	return returnValue;
	}

/**Returns value of specified instance of the parameter.
   Instance indexing starts at 0.
*/
    XML_dataType getParameterInstanceValue(size_t instanceIndex,const std::string& parameterName, const std::string& parameterListName) const
	{
	return getParameterInstanceValue(instanceIndex, parameterName.c_str(), parameterListName.c_str());
	}

/**Returns value of specified instance of the parameter.
   Instance indexing starts at 0.
*/
	XML_dataType getParameterInstanceValue(size_t instanceIndex,const char* parameterName, const char* parameterListName) const
	{
    abortIfNull();
	tinyxml2::XMLHandle  docHandle(parameterArrayDocPtr->RootElement());
    tinyxml2::XMLElement* parameterListElement  = docHandle.FirstChildElement(parameterListName).ToElement();
    checkParameterList(parameterListElement,parameterListName,"getParameterInstanceValue(...)");

	tinyxml2::XMLElement* parameterElement      = parameterListElement->FirstChildElement(parameterName);
	checkParameter(parameterElement,parameterName,parameterListName,"getParameterInstanceValue(...)");

	checkInstanceIndex(instanceIndex,parameterName,parameterListName,"getParameterInstanceValue(...)");

    XML_dataType returnValue;

	tinyxml2::XMLNode* node;

	size_t count = 0;
	for( node = parameterListElement->FirstChildElement(parameterName);
		 node;
		 node = node->NextSiblingElement(parameterName) )
	{
		if(count == instanceIndex)
		{
			if(node->ToElement())
			{
			returnValue = getParameterValue(node->ToElement(),parameterName,  parameterListName);
			}
		}
		count++;
	}
	return returnValue;
	}

/** Determines if the specified instance of the parameter has the designated child parameter.
    Instance indexing starts at 0.
 */
    bool isParameterInstanceChildValue(int instanceIndex,const std::string& childParameter,
    const std::string& parameterName, const std::string& parameterListName) const
    {
    return isParameterInstanceChildValue(instanceIndex,childParameter.c_str(), parameterName.c_str(), parameterListName.c_str());
    }

/** Determines if the specified instance of the parameter has the designated child parameter.
    Instance indexing starts at 0.
 */
    bool isParameterInstanceChildValue(int instanceIndex,const char* childParameter,
    const char* parameterName, const char* parameterListName) const
	{

    abortIfNull();
	tinyxml2::XMLHandle  docHandle(parameterArrayDocPtr->RootElement());
    tinyxml2::XMLElement* parameterListElement  = docHandle.FirstChildElement(parameterListName).ToElement();
    checkParameterList(parameterListElement,parameterListName,"isParameterInstanceChildValue(...)");

	tinyxml2::XMLElement* parameterElement      = parameterListElement->FirstChildElement(parameterName);
	checkParameter(parameterElement,parameterName,parameterListName,"isParameterInstanceChildValue(...)");

	checkInstanceIndex(instanceIndex,parameterName,parameterListName,"getParameterInstanceValue(...)");

	tinyxml2::XMLNode* node;;

	long count = 0;
	for( node = parameterListElement->FirstChildElement(parameterName);
		 node;
		 node = node->NextSiblingElement(parameterName) )
	{
	    if(count == instanceIndex)
	    {
	    	if(node->FirstChildElement(childParameter))
	    	{return true;}
	    	else
	    	{return false;}
	    }
	    count++;
	}
    return false;
	}

/// Determines if the specified instance of the parameter has the designated child parameter
    bool isParameterInstanceChildValue(const std::string& instanceName,const std::string& childParameter,
    const std::string& parameterName, const std::string& parameterListName) const
    {
    return isParameterInstanceChildValue(instanceName.c_str(),childParameter.c_str(), parameterName.c_str(), parameterListName.c_str());
    }

/// Determines if the specified instance of the parameter has the designated child parameter
    bool isParameterInstanceChildValue(const char* instanceName,const char* childParameter,
    const char* parameterName, const char* parameterListName) const
	{
    abortIfNull();

	tinyxml2::XMLHandle  docHandle(parameterArrayDocPtr->RootElement());

    tinyxml2::XMLElement* parameterListElement  = docHandle.FirstChildElement(parameterListName).ToElement();
    checkParameterList(parameterListElement,parameterListName,"isParameterInstanceChildValue(...)");

	tinyxml2::XMLElement* parameterElement      = parameterListElement->FirstChildElement(parameterName);
    checkParameter(parameterElement,parameterName,parameterListName,"isParameterInstanceChildValue(...)");

    tinyxml2::XMLElement* parameterChild;
	tinyxml2::XMLElement* node;


	const char* p = nullptr;
	const char** stringData = &p;
	std::string instanceString(instanceName);
	std::string indexString;

	for( node = parameterListElement->FirstChildElement(parameterName);
		 node;
		 node = node->NextSiblingElement(parameterName) )
	{
		if(node->FirstChildElement("name"))
		{
		    parameterChild = node->FirstChildElement("name");
		    indexString.clear();
		    if(parameterChild->QueryStringAttribute("value",stringData) == tinyxml2::XML_SUCCESS){indexString = stringData[0];}
		    if(instanceString.compare(indexString) == 0)
		    {
		    if(node->FirstChildElement(childParameter)){return true;}
		    }
	    }
	}
    return false;
	}

/** Returns the specified parameter child value as an XML_dataType.

    Automatic conversion to standard data types occurs on assignment,
    except for string data.

    String data requires an explicit case before assignment.
*/
    XML_dataType getParameterChildValue(const std::string& childParameter, const std::string& parameterName, const std::string& parameterListName) const
	{
	return getParameterChildValue(childParameter.c_str(), parameterName.c_str(), parameterListName.c_str());
	}

/** Returns the specified parameter child value as an XML_dataType.

    Automatic conversion to standard data types occurs on assignment,
    except for string data.

    String data requires an explicit case before assignment.
*/
    XML_dataType getParameterChildValue(const char* childParameter, const char* parameterName, const char* parameterListName) const
	{
	abortIfNull();

	tinyxml2::XMLHandle  docHandle(parameterArrayDocPtr->RootElement());

    tinyxml2::XMLElement* parameterListElement  = docHandle.FirstChildElement(parameterListName).ToElement();
    checkParameterList(parameterListElement,parameterListName,"getParameterChildValue(...)");

	tinyxml2::XMLElement* parameterElement      = parameterListElement->FirstChildElement(parameterName);
    checkParameter(parameterElement,parameterName,parameterListName,"getParameterChildValue(...)");

	tinyxml2::XMLElement* parameterChildElement = parameterElement->FirstChildElement(childParameter);
    checkParameterChild(parameterChildElement,childParameter,parameterName,parameterListName,"getParameterChildValue(...)");

    XML_dataType returnValue;

	returnValue = getParameterValue(parameterChildElement, parameterName,  parameterListName);
	return returnValue;
	}


/** Returns the specified parameter child value as an XML_dataType if the parameter exists,
    otherwise it returns the default value.

    Automatic conversion to standard data types occurs on assignment,
    except for string data.

    String data requires an explicit case before assignment.
*/
	XML_dataType getParameterChildValueOrDefault(const std::string& childParameter,
    const std::string&  parameterName, const std::string&  parameterListName,XML_dataType defaultValue) const
    {
    return getParameterChildValueOrDefault(childParameter.c_str(), parameterName.c_str(), parameterListName.c_str(),defaultValue);
    }


/** Returns the specified parameter child value as an XML_dataType if the parameter exists,
    otherwise it returns the default value.

    Automatic conversion to standard data types occurs on assignment,
    except for string data.

    String data requires an explicit case before assignment.
*/
	XML_dataType getParameterChildValueOrDefault(const char* childParameter,
    const char* parameterName, const char* parameterListName,XML_dataType defaultValue) const
    {
	if(this->isParameterInstanceChildValue(0,childParameter,parameterName,parameterListName))
	{
		return this->getParameterChildValue(childParameter,parameterName,parameterListName);
	}
    return defaultValue;
    }


/** Returns the specified parameter child value as an XML_dataType. Indexing starts
    at 0.

    Automatic conversion to standard data types occurs on assignment,
    except for string data.

    String data requires an explicit case before assignment.
*/
    XML_dataType getParameterInstanceChildValue(int instanceIndex,const std::string& childParameter,
			const std::string& parameterName, const std::string& parameterListName) const
    {
    return getParameterInstanceChildValue(instanceIndex,childParameter.c_str(),parameterName.c_str(),parameterListName.c_str());
    }

/** Returns the specified parameter child value as an XML_dataType. Indexing starts
    at 0.

    Automatic conversion to standard data types occurs on assignment,
    except for string data.

    String data requires an explicit case before assignment.
*/
	XML_dataType getParameterInstanceChildValue(int instanceIndex,const char* childParameter,
    const char* parameterName, const char* parameterListName) const
	{
    abortIfNull();

	tinyxml2::XMLHandle  docHandle(parameterArrayDocPtr->RootElement());

    tinyxml2::XMLElement* parameterListElement  = docHandle.FirstChildElement(parameterListName).ToElement();
    checkParameterList(parameterListElement,parameterListName,"getParameterInstanceChildValue(...)");

	tinyxml2::XMLElement* parameterElement      = parameterListElement->FirstChildElement(parameterName);
    checkParameter(parameterElement,parameterName,parameterListName,"getParameterInstanceChildValue(...)");

	checkInstanceIndex(instanceIndex,parameterName,parameterListName,"getParameterInstanceValue(...)");

	tinyxml2::XMLNode* node;
	XML_dataType returnValue;

	long count = 0;
	for( node = parameterListElement->FirstChildElement(parameterName);
		 node;
		 node = node->NextSiblingElement(parameterName) )
	{
	    if(count == instanceIndex)
	    {
	    	if(node->FirstChildElement(childParameter))
	    	{
	    	return  getParameterValue(node->FirstChildElement(childParameter)->ToElement(),parameterName,  parameterListName);
	    	}
	    }
	    count++;
	}
	errorFlag = true;
	errorMessage.append("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
	errorMessage.append("XML_ParameterListArray Class Error \n");
    errorMessage.append("    getParameterInstanceChildValue(...)\n");
	errorMessage.append("Parameter instance child value not found.\n");
	errorMessage.append("ParameterList           : ");
	errorMessage.append(parameterListName);
	errorMessage.append("\nParameter               : ");
	errorMessage.append(parameterName);
	errorMessage.append("\nParameter instance index : ");
	errorMessage.append(std::to_string(instanceIndex));
	errorMessage.append("\nParameter child         : ");
	errorMessage.append(childParameter);

	if(abortOnErrorFlag){checkErrorAndAbort();}
	return returnValue;
	}


/** Returns the specified parameter child value as an XML_dataType if the parameter exists,
    otherwise it returns the default value. Indexing starts a 0.

    Automatic conversion to standard data types occurs on assignment,
    except for string data.

    String data requires an explicit case before assignment.
*/
	XML_dataType getParameterInstanceChildValueOrDefault(int instanceIndex,const std::string& childParameter,
    const std::string&  parameterName, const std::string&  parameterListName,XML_dataType defaultValue) const
    {
    return getParameterInstanceChildValueOrDefault(instanceIndex,childParameter.c_str(),
    parameterName.c_str(),parameterListName.c_str(),defaultValue);
    }

/** Returns the specified parameter child value as an XML_dataType if the parameter exists,
    otherwise it returns the default value.  Indexing starts a 0.

    Automatic conversion to standard data types occurs on assignment,
    except for string data.

    String data requires an explicit case before assignment.
*/
	XML_dataType getParameterInstanceChildValueOrDefault(int instanceIndex,const char* childParameter,
    const char* parameterName, const char* parameterListName,XML_dataType defaultValue) const
    {
	if(this->isParameterInstanceChildValue(instanceIndex,childParameter,parameterName,parameterListName))
	{
		return this->getParameterInstanceChildValue(instanceIndex,childParameter,parameterName,parameterListName);
	}
    return defaultValue;
    }

/** Returns the specified parameter child value as an XML_dataType.

    Automatic conversion to standard data types occurs on assignment,
    except for string data.

    String data requires an explicit case before assignment.
*/
	XML_dataType getParameterInstanceChildValue(const std::string& instanceName,const std::string& childParameter,
    const std::string& parameterName, const std::string& parameterListName) const
    {
    return getParameterInstanceChildValue(instanceName.c_str(),childParameter.c_str(),parameterName.c_str(), parameterListName.c_str());
    }

/** Returns the specified parameter child value as an XML_dataType.

    Automatic conversion to standard data types occurs on assignment,
    except for string data.

    String data requires an explicit case before assignment.
*/
	XML_dataType getParameterInstanceChildValue(const char* instanceName,const char* childParameter,
    const char* parameterName, const char* parameterListName) const
    {

    abortIfNull();

	tinyxml2::XMLHandle  docHandle(parameterArrayDocPtr->RootElement());

    tinyxml2::XMLElement* parameterListElement  = docHandle.FirstChildElement(parameterListName).ToElement();
    checkParameterList(parameterListElement,parameterListName,"getParameterInstanceChildValue(...)");

	tinyxml2::XMLElement* parameterElement      = parameterListElement->FirstChildElement(parameterName);
    checkParameter(parameterElement,parameterName,parameterListName,"getParameterInstanceChildValue(...)");


    XML_dataType returnValue;

    tinyxml2::XMLElement* parameterChild;
	tinyxml2::XMLNode* node;
    std::string instanceString(instanceName);
    std::string indexString;


    const char*p = nullptr;
    const char** stringData = &p;
	for( node = parameterListElement->FirstChildElement(parameterName);
		 node;
		 node = node->NextSiblingElement(parameterName) )
	{
		if(node->FirstChildElement("name"))
		{
		    parameterChild = node->FirstChildElement("name")->ToElement();
		    indexString.clear();
		    if(parameterChild->QueryStringAttribute("value",stringData) == tinyxml2::XML_SUCCESS){indexString = stringData[0];}
		    if(instanceString.compare(indexString) == 0)
		    {
		    	if(node->FirstChildElement(childParameter))
		    	{
		    		return getParameterValue(node->FirstChildElement(childParameter)->ToElement(),parameterName,  parameterListName);
		    	}
		    }
	     }

	}


    errorFlag = true;
    errorMessage.append("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
    errorMessage.append("XML_ParameterListArray Class Error \n");
    errorMessage.append("    getParameterInstanceChildValue(...)\n");
    errorMessage.append("Parameter instance name specified does not exist.\n");
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
    errorMessage.append(instanceString);
    errorMessage.append("\nXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
    errorMessage.append("\n");

	if(abortOnErrorFlag){checkErrorAndAbort();}
	return returnValue;
	}


/** Returns the specified parameter child value as an XML_dataType if the parameter exists,
    otherwise it returns the default value.

    Automatic conversion to standard data types occurs on assignment,
    except for string data.

    String data requires an explicit case before assignment.
*/
	XML_dataType getParameterInstanceChildValueOrDefault(const std::string& instanceName,const std::string& childParameter,
    const std::string&  parameterName, const std::string&  parameterListName,XML_dataType defaultValue) const
    {
    return getParameterInstanceChildValueOrDefault(instanceName.c_str(),childParameter.c_str(),
    parameterName.c_str(),parameterListName.c_str(),defaultValue);
    }

/** Returns the specified parameter child value as an XML_dataType if the parameter exists,
    otherwise it returns the default value.

    Automatic conversion to standard data types occurs on assignment,
    except for string data.

    String data requires an explicit case before assignment.
*/
	XML_dataType getParameterInstanceChildValueOrDefault(const char* instanceName,const char* childParameter,
    const char* parameterName, const char* parameterListName,XML_dataType defaultValue) const
    {
	if(this->isParameterInstanceChildValue(instanceName,childParameter,parameterName,parameterListName))
	{
		return this->getParameterInstanceChildValue(instanceName,childParameter,parameterName,parameterListName);
	}
    return defaultValue;
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////
    //     Member functions for extracting information about parameter lists and parameters
    ///////////////////////////////////////////////////////////////////////////////////////////////

/// Returns an array of parameter list names
	void getParameterListNames(std::vector < std::string >& paramListNames) const
	{
	paramListNames.clear();
	if(parameterArrayDocPtr == 0) return;
	tinyxml2::XMLHandle   docHandle(parameterArrayDocPtr->RootElement());
	tinyxml2::XMLElement* parameterList = docHandle.FirstChildElement().ToElement();
	tinyxml2::XMLNode* node = 0;
	for( node = parameterList; node; node = node->NextSibling())
	{
		if(not node->ToComment())
        {paramListNames.push_back(node->ToElement()->Name());}
	}
	}

/// Returns an array of parameter names of the specified parameter lists
    void getParameterNames(const std::string& parameterListName, std::vector < std::string >& paramNames) const
	{
	getParameterNames(parameterListName.c_str(),paramNames);
	}

/// Returns an array of parameter names of the speceified parameter lists
    void getParameterNames(const char* parameterListName, std::vector < std::string >& paramNames) const
	{
	paramNames.clear();
	if(not isParameterList(parameterListName)) return;

	paramNames.clear();
	if(parameterArrayDocPtr == 0) return;
	tinyxml2::XMLHandle   docHandle(parameterArrayDocPtr->RootElement());
	tinyxml2::XMLNode* parameterList = docHandle.FirstChildElement(parameterListName).ToElement();
	tinyxml2::XMLNode* node = 0;
    for( node = parameterList->FirstChildElement(); node; node = node->NextSibling())
	{
		if(not node->ToComment())
        {paramNames.push_back(node->ToElement()->Name());}
	}
	}

/// Returns the names of the child parameters of the first instance of the specified parameter
	void getParameterChildNames(const std::string& parameterName,
	const std::string& parameterListName, std::vector < std::string >& paramChildNames)
	{
	getParameterChildNames(0,parameterName.c_str(),parameterListName.c_str(),paramChildNames);
	}

/// Returns the names of the child parameters of the first instance of the specified parameter
	void getParameterChildNames(const char* parameterName, const char* parameterListName,
	std::vector < std::string >& paramChildNames)
	{
	getParameterChildNames(0,parameterName,parameterListName,paramChildNames);
	}

/// Returns the names of the child parameters of the specified instance of the specified parameter
	void getParameterChildNames(long instanceIndex, const std::string& parameterName,
	const std::string& parameterListName, std::vector < std::string >& paramChildNames) const
	{
    getParameterChildNames(instanceIndex, parameterName.c_str(),parameterListName.c_str(), paramChildNames);
	}

/// Returns the names of the child parameters of the specified instance of the specified parameter
	void getParameterChildNames(long instanceIndex, const char* parameterName,
	const char* parameterListName, std::vector < std::string >& paramChildNames) const
	{
	paramChildNames.clear();
	if(not isParameter(parameterName, parameterListName)) return;

	tinyxml2::XMLHandle   docHandle(parameterArrayDocPtr->RootElement());
	tinyxml2::XMLElement* parameterList = docHandle.FirstChildElement(parameterListName).ToElement();

	tinyxml2::XMLNode* node = 0;
    tinyxml2::XMLNode* child = 0;
	long count = 0;
	for( node = parameterList->FirstChildElement(parameterName);
		 node;
		 node = node->NextSiblingElement(parameterName) )
	{
	    if(count == instanceIndex)
	    {
	    	for( child = node->FirstChildElement(); child; child = child->NextSibling())
	    	{
	    		if(not child->ToComment())
	    		{paramChildNames.push_back(child->Value());}
	    	}
        }
        count++;
    }
	}

/**Returns the type of the parameter as a string.
   If the parameter is not typed then the return string is empty.
*/
	std::string getParameterTypeName(const std::string& parameterName, const std::string& parameterListName) const
	{
	return getParameterTypeName(parameterName.c_str(),parameterListName.c_str());
	}

/**Returns the type of the parameter as a string.
   If the parameter is not typed then the return string is empty.
*/
	std::string getParameterTypeName(const char* parameterName, const char* parameterListName) const
	{

    abortIfNull();

	tinyxml2::XMLHandle  docHandle(parameterArrayDocPtr->RootElement());

    tinyxml2::XMLElement* parameterListElement  = docHandle.FirstChildElement(parameterListName).ToElement();
    checkParameterList(parameterListElement,parameterListName,"getParameterTypeName(...)");

	tinyxml2::XMLElement* parameterElement      = parameterListElement->FirstChildElement(parameterName);
    checkParameter(parameterElement,parameterName,parameterListName,"getParameterTypeName(...)");

	std::string returnValue;
	returnValue = getParameterTypeName(parameterElement, parameterName,  parameterListName);

	if(abortOnErrorFlag){checkErrorAndAbort();}
	return returnValue;
	}

/**Returns a map of the types of the parameters as a string indexed by the parameter name.

   If the parameter is not typed then the associated string is empty.
*/
    void getParameterTypeNames(const std::string& parameterListName, std::map < std::string, std::string >& paramTypeNames) const
	{
	getParameterTypeNames(parameterListName.c_str(),paramTypeNames);
	}

/**Returns a map of the types of the parameters as a string indexed by the parameter name.

      If the parameter is not typed then the associated string is empty.
*/
	void getParameterTypeNames(const char* parameterListName, std::map < std::string, std::string >& paramTypeNames) const
	{

	paramTypeNames.clear();

    tinyxml2::XMLHandle  docHandle(parameterArrayDocPtr->RootElement());

    tinyxml2::XMLElement* parameterListElement  = docHandle.FirstChildElement(parameterListName).ToElement();


    checkParameterList(parameterListElement,parameterListName,"getParameterTypeNames(...)");

	tinyxml2::XMLNode* node = 0;

	std::string typeString;

	for( node = parameterListElement->FirstChildElement(); node; node = node->NextSibling())
	{
		if(not node->ToComment())
        {
        typeString = getParameterTypeName(node->ToElement(),node->Value(),parameterListName);
        paramTypeNames[node->Value()] =  typeString;
        }
	}

	}


	///////////////////////////////////////////////////////////////////////////////////////////////
    //           Member functions for handling errors
	///////////////////////////////////////////////////////////////////////////////////////////////

/// Returns true if null instance
    bool isNull() const
	{
	if(parameterArrayDocPtr == nullptr){return true;}
	return false;
	}

/// Throws an exception of the instance is a null instance
	void abortIfNull() const
	{

	if(isNull())
	{
	    errorMessage.append("XXX XML_ParameterListArray programming error. XXXX \n");
		errorMessage.append("XML_ParameterListArray is not properly initialized.\n");
		errorMessage.append("Use createParameterListArray(*) to initialize.\n");
		errorMessage.append("XXX Program Terminated XXX");
		errorMessage.append("\n");
	}
	checkErrorAndAbort();
	}

/** Checks if error flag has been set and throws an exception if
    abort on error flag is true, otherwise a null operation.
*/
	void checkErrorAndAbort() const
	{
    if(errorFlag)
    {
    throw std::runtime_error(getErrorMessage());
    }
	}

/// Sets flag whose value determines if an exception is thrown when an error is generated.
	void setAbortOnErrorFlag(bool val = true)  const
	{
	abortOnErrorFlag       = val;
	}

	bool  getAbortOnErrorFlag()   const
	{
	return abortOnErrorFlag;
	}

	void  clearAbortOnErrorFlag() const
	{
	abortOnErrorFlag = false;
	}

	bool        getErrorFlag()    const {return errorFlag;}
	const char* getErrorMessage() const {return errorMessage.c_str();}

	void clearError() const
	{
	errorFlag = false;
	errorMessage.clear();
	}

/**getDataType interrogates input string and returns a character string
   as one of bool, double, long, string

   Case independent versions of {true,yes} and {false,no} are returned as type bool
*/
	const char* getDataType(const std::string& sIn) const
	{
		return  getDataType(sIn.c_str());
	}

/**getDataType interrogates input string and returns a character string
   as one of bool, double, long, string

   Case independent versions of {true,yes} and {false,no} are returned as type bool
*/
	const char* getDataType(const char* sIn) const
	{
    // remove white space before and after variables

    std::string sInString(sIn);
    std::string sTrimmed(trim(sInString));
    const char* s   = sTrimmed.c_str();

    const char* boolType   = "bool";
	const char* doubleType = "double";
	const char* longType   = "long";
	const char* stringType = "string";

	// Check for boolean first

    std::string stringTemp(s);
	std::transform(stringTemp.begin(), stringTemp.end(), stringTemp.begin(), [](unsigned char c)
	{return  static_cast<char>(std::toupper(c)); });
	if(stringTemp.compare("FALSE") == 0)  return boolType;
	if(stringTemp.compare("TRUE")  == 0)  return boolType;
	if(stringTemp.compare("NO")    == 0)  return boolType;
	if(stringTemp.compare("YES")   == 0)  return boolType;

    // Check for integer

	if(isInteger(stringTemp)) return longType;

	// Check for double

	std::string::iterator it;

    size_t Ecount = 0;
    for (it = stringTemp.begin(); it != stringTemp.end();it++)
    {
    	if(*it == 'E') Ecount++;
    }

    if(Ecount > 1) return stringType;

    std::vector<std::string> tokens;
    std::string delimiters;

    if(Ecount == 0)
    {
      if(isNoExponentDouble(stringTemp)) return doubleType;
      return stringType;
    }
    if(Ecount == 1)
    {
      delimiters = "E";
      Tokenize(stringTemp,tokens,delimiters);
      if(tokens.size() != 2) return stringType;
      if((isNoExponentDouble(tokens[0]))&&(isInteger(tokens[1])))
      {
      return doubleType;
      }
      return stringType;
    }
	return stringType;
	}

    protected:

    void checkParameterList(tinyxml2::XMLElement* parameterListElement,const char* parameterListName,
	const std::string& routineName = "") const
	{
	    if(not parameterListElement)
		{
			errorFlag = true;
			errorMessage.append("\nXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
			errorMessage.append("XML_ParameterListArray Class Error \n");
			if(not routineName.empty())
			{
			errorMessage.append("Member function : \n  ");
			errorMessage.append(routineName);
			errorMessage.append("\n");
			}
			errorMessage.append("Specified parameterList does not exists.\n");
			errorMessage.append("Offending parameterListName : ");
			errorMessage.append(parameterListName);
			errorMessage.append("\nXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
			errorMessage.append("\n");
    }
    if(abortOnErrorFlag){checkErrorAndAbort();}
	}

    void checkParameter(tinyxml2::XMLElement* parameterElement,const char* parameterName,const char* parameterListName,
    const std::string& routineName = "") const
	{
	    if(not parameterElement)
		{
			errorFlag = true;
			errorMessage.append("\nXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
			errorMessage.append("XML_ParameterListArray Class Error \n");
		    if(not routineName.empty())
			{
			errorMessage.append("Member function : \n  ");
			errorMessage.append(routineName);
			errorMessage.append("\n");
			}
			errorMessage.append("Specified parameter does not exists.\n");
			errorMessage.append("Specified parameterList name : ");
			errorMessage.append(parameterListName);
		    errorMessage.append("\nOffending parameter name     : ");
			errorMessage.append(parameterName);
			errorMessage.append("\nXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
			errorMessage.append("\n");
         }
         if(abortOnErrorFlag){checkErrorAndAbort();}
	}

	void checkParameterChild(tinyxml2::XMLElement* parameterChildElement,const char* parameterChildName,
	    const char* parameterName,const char* parameterListName,
		const std::string& routineName = "") const
	    {
	    if(not parameterChildElement)
		{
			errorFlag = true;
			errorMessage.append("\nXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
			errorMessage.append("XML_ParameterListArray Class Error \n");
		    if(not routineName.empty())
			{
			errorMessage.append("Member function : \n  ");
			errorMessage.append(routineName);
			errorMessage.append("\n");
			}
			errorMessage.append("Specified parameter child does not exists.\n");
			errorMessage.append("Specified parameterList name       : ");
			errorMessage.append(parameterListName);
		    errorMessage.append("\nOffending parameter name           : ");
			errorMessage.append(parameterName);
		    errorMessage.append("\nOffending parameter child name     : ");
			errorMessage.append(parameterChildName);
			errorMessage.append("\nXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
			errorMessage.append("\n");
    }
    if(abortOnErrorFlag){checkErrorAndAbort();}
	}

	void checkInstanceIndex(size_t instanceIndex, const char* parameterName,const char* parameterListName,
    const std::string& routineName = "") const
	{
	size_t instanceCount = getParameterInstanceCount(parameterName,parameterListName);
	if(instanceCount-1 < instanceIndex)
	{
		errorFlag = true;
		errorMessage.append("\nXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
		errorMessage.append("XML_ParameterListArray Class Error \n");
		if(not routineName.empty())
		{
			errorMessage.append("Member function : \n  ");
			errorMessage.append(routineName);
			errorMessage.append("\n");
		}
		errorMessage.append("Specified parameter instance does not exists.\n");
		errorMessage.append("Specified parameterList name       : ");
		errorMessage.append(parameterListName);
		errorMessage.append("\nOffending parameter name           : ");
		errorMessage.append(parameterName);
		errorMessage.append("\nOffending parameter instance index : ");
		errorMessage.append(std::to_string(instanceIndex));
		errorMessage.append("\nXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
		errorMessage.append("\n");
	}
	if(abortOnErrorFlag){checkErrorAndAbort();}
	}

    void setParameterValue(const XML_dataType& value, tinyxml2::XMLElement* parameterElement)
    {
    abortIfNull();
    tinyxml2::XMLHandle   docHandle(parameterArrayDocPtr->RootElement());

    const char* inputDataType = getDataType(value.toString().c_str());
    const char* dataType      = parameterElement->Attribute("type");

    if(parameterElement->Attribute("value") != 0)
    {
    	// Set the data type if value is found and type not specified
    	if(dataType == 0)
    	{
    		dataType = getDataType(parameterElement->Attribute("value"));
    		parameterElement->SetAttribute("type",dataType);
    	}
    }
    else
    {
        // Set the data type to the input data type
    	parameterElement->SetAttribute("type",inputDataType);
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
    stringTemp = (std::string)(XML_dataType(std::string(value)).toString());
	std::transform(stringTemp.begin(), stringTemp.end(), stringTemp.begin(), [](unsigned char c){return  static_cast<char>(std::toupper(c)); });
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

	XML_dataType getParameterValue(const tinyxml2::XMLElement* parameter,const std::string& parameterName, const std::string& parameterListName) const
	{
	return 	getParameterValue(parameter,parameterName.c_str(), parameterListName.c_str());
	}

	XML_dataType getParameterValue(const tinyxml2::XMLElement* parameter,const char* parameterName, const char* parameterListName) const
	{
	int           intParam;
	double     doubleParam;
	float       floatParam;
	bool         boolParam;
	std::string     stringParam;
	std::string      stringTemp;
	bool     explicitType;

	const char* dataType = 0;
	if(parameter)
	{
		//
		// Check for values attribute
		//
		if(parameter->Attribute("value") == 0)
		{
		errorFlag = true;
		errorMessage.append("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
		errorMessage.append("XML_ParameterListArray Class Error \n");
		errorMessage.append("Parameter Value not specified using value= \"...\"\n");
		errorMessage.append("Parameter List : ");
		errorMessage.append(parameterListName);
		errorMessage.append("\n");
		errorMessage.append("Parameter      : ");
		errorMessage.append(parameterName);
		errorMessage.append("\n");
		if(abortOnErrorFlag){checkErrorAndAbort();}
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
			if(parameter->QueryBoolAttribute("value",&boolParam)  == tinyxml2::XML_SUCCESS) {return XML_dataType(boolParam);}
		}
		if(strcmp(dataType,"int") == 0)
		{
			if(parameter->QueryIntAttribute("value",&intParam)  == tinyxml2::XML_SUCCESS) {return XML_dataType(intParam);}
		}
		if(strcmp(dataType,"long") == 0)
		{
			if(parameter->QueryIntAttribute("value",&intParam)  == tinyxml2::XML_SUCCESS) {return XML_dataType((long)intParam);}
		}
		if(strcmp(dataType,"float") == 0)
		{
			if(parameter->QueryFloatAttribute("value",&floatParam)  == tinyxml2::XML_SUCCESS) {return XML_dataType(floatParam);}
		}
		if(strcmp(dataType,"double") == 0)
		{
			if(parameter->QueryDoubleAttribute("value",&doubleParam)  == tinyxml2::XML_SUCCESS) {return XML_dataType(doubleParam);}
		}
		//
		// For strings, if not explicitly typed, check to see if it's a boolean variable first
		// as designated by any upper or lower case collection of letters spelling true, false
		// or yes or no.
		//
		// I don't use tinyXML QueryBoolAttribute since it returns incorrectly if one specifies the
		// string None.

        const char* p;
        const char** stringData = &p;
		if(strcmp(dataType,"string") == 0)
		{
            if(parameter->QueryStringAttribute("value",stringData)  == tinyxml2::XML_SUCCESS)
            {
            stringParam = stringData[0];
            if(explicitType){return XML_dataType(stringParam);}
			else // interrogate first for conversion to boolean based on false, true, yes or no,
			{
			stringTemp = stringParam;
			std::transform(stringTemp.begin(), stringTemp.end(), stringTemp.begin(), [](unsigned char c) {return  static_cast<char>(std::toupper(c)); });
			if(stringTemp.compare("FALSE") == 0) {boolParam = false; return XML_dataType(boolParam);}
			if(stringTemp.compare("TRUE")  == 0) {boolParam = true;  return XML_dataType(boolParam);}
			if(stringTemp.compare("NO") == 0)    {boolParam = false; return XML_dataType(boolParam);}
			if(stringTemp.compare("YES")  == 0)  {boolParam = true;  return XML_dataType(boolParam);}
			return XML_dataType(stringParam);
			}}
		}

	}

	errorFlag = true;
	errorMessage.append("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
	errorMessage.append("XML_ParameterListArray Class Error \n");
	errorMessage.append("Parameter Not found.\n");
	errorMessage.append("Parameter List : ");
	errorMessage.append(parameterListName);
	errorMessage.append("\n");
	errorMessage.append("Parameter      : ");
	errorMessage.append(parameterName);
	errorMessage.append("\n");
	if(abortOnErrorFlag){checkErrorAndAbort();}

	return XML_dataType();
	}

    /**Returns the type of the parameter as a string.
       If the parameter is not typed then the return string is empty.
    */
	std::string getParameterTypeName(const tinyxml2::XMLElement* parameter,const char* parameterName, const char* parameterListName) const
	{
	int                intParam;
	double          doubleParam;
	float            floatParam;
	bool              boolParam;
	std::string     stringParam;
	std::string      stringTemp;
	bool          explicitType;


	const char* dataType = 0;
	if(parameter)
	{
		//
		// Check for values attribute
		//
		if(parameter->Attribute("value") == 0)
		{
		return stringTemp;
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
		 return std::string(dataType);
		}

		if(strcmp(dataType,"bool") == 0)
		{
			if(parameter->QueryBoolAttribute("value",&boolParam)  == tinyxml2::XML_SUCCESS) {return std::string("bool");}
		}
		if(strcmp(dataType,"int") == 0)
		{
			if(parameter->QueryIntAttribute("value",&intParam)  == tinyxml2::XML_SUCCESS) {return std::string("int");}
		}
		if(strcmp(dataType,"long") == 0)
		{
			if(parameter->QueryIntAttribute("value",&intParam)  == tinyxml2::XML_SUCCESS) {return std::string("long");}
		}
		if(strcmp(dataType,"float") == 0)
		{
			if(parameter->QueryFloatAttribute("value",&floatParam)  == tinyxml2::XML_SUCCESS) {return std::string("float");}
		}
		if(strcmp(dataType,"double") == 0)
		{
			if(parameter->QueryDoubleAttribute("value",&doubleParam)  == tinyxml2::XML_SUCCESS) {return std::string("double");}
		}
		//
		// For strings, if not explicitly typed, check to see if it's a boolean variable first
		// as designated by any upper or lower case collection of letters spelling true, false
		// or yes or no.
		//
		// I don't use tinyXML QueryBoolAttribute since it returns incorrectly if one specifies the
		// string None.

        const char* p = nullptr;
        const char** stringData = &p;
		if(strcmp(dataType,"string") == 0)
		{
			//if(parameter->QueryBoolAttribute("value",&boolParam)  == tinyxml2::XML_SUCCESS)     {return XML_dataType(boolParam);}

            if(parameter->QueryStringAttribute("value",stringData)  == tinyxml2::XML_SUCCESS)
            {
            if(explicitType){return std::string("string");}
			else // interrogate first for conversion to boolean based on false, true, yes or no,
			{
			stringTemp = stringData[0];
			std::transform(stringTemp.begin(), stringTemp.end(), stringTemp.begin(), [](unsigned char c) {return  static_cast<char>(std::toupper(c)); });
			if(stringTemp.compare("FALSE") == 0) {return std::string("bool");}
			if(stringTemp.compare("TRUE")  == 0) {return std::string("bool");}
			if(stringTemp.compare("NO") == 0)    {return std::string("bool");}
			if(stringTemp.compare("YES")  == 0)  {return std::string("bool");}
			return XML_dataType(stringParam);
			}}
		}
	}

	errorFlag = true;
	errorMessage.append("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
	errorMessage.append("XML_ParameterListArray Class Error \n");
	errorMessage.append("Parameter Not found.\n");
	errorMessage.append("Parameter List : ");
	errorMessage.append(parameterListName);
	errorMessage.append("\n");
	errorMessage.append("Parameter      : ");
	errorMessage.append(parameterName);
	errorMessage.append("\n");
	if(abortOnErrorFlag){checkErrorAndAbort();}
	return stringTemp;
	}

	void Tokenize(const std::string& str,std::vector<std::string>& tokens, const std::string& delimiters = " ") const
	{
    // Skip delimiters at beginning.
    std::string::size_type lastPos = str.find_first_not_of(delimiters, 0);
    // Find first "non-delimiter".
    std::string::size_type pos     = str.find_first_of(delimiters, lastPos);

    while (std::string::npos != pos || std::string::npos != lastPos)
    {
        // Found a token, add it to the vector.
        tokens.push_back(str.substr(lastPos, pos - lastPos));
        // Skip delimiters.  Note the "not_of"
        lastPos = str.find_first_not_of(delimiters, pos);
        // Find next "non-delimiter"
        pos = str.find_first_of(delimiters, lastPos);
    }
	}

	bool isInteger(const std::string& sIn) const
	{
    std::string stringTemp(sIn);
	std::transform(stringTemp.begin(), stringTemp.end(), stringTemp.begin(), [](unsigned char c)
	{return  static_cast<char>(std::toupper(c)); });

	std::string::iterator it;
    std::string::iterator itStart;

    itStart = stringTemp.begin();
    if((*itStart == '+')||(*itStart == '-')) ++itStart;

    bool isIntegerFlag = true;
    for (it = itStart; it != stringTemp.end();it++)
    {
        if(not (((int)*it >= 48)&&((int)*it <= 57)))
        {
        isIntegerFlag = false;
        }
    }

    return isIntegerFlag;
	}

	bool isNoExponentDouble(const std::string& sIn) const
	{
    std::vector<std::string> tokens;
    std::string delimiters;
    delimiters = ".";
	Tokenize(sIn,tokens,delimiters);
	if(tokens.size() == 1)
	{
      if(isInteger(tokens[0]))
      {return true;}
      else
      {return false;}
	}
	else if(tokens.size() == 2)
	{
      if((isInteger(tokens[0]))&&(isInteger(tokens[1])))
      {return true;}
      else
      {return false;}
    }
    return false;
	}

/// trim string from both ends
	inline std::string &trim(std::string &s) const {return ltrim(rtrim(s));}

/// trim string from start
	inline std::string& ltrim(std::string& s) const {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(),
		([](int c) {return !std::isspace(c);} )  ));
	return s;
	}

/// trim string from end
	inline std::string& rtrim(std::string& s) const {
	s.erase(std::find_if(s.rbegin(), s.rend(), ([](int c) {return !std::isspace(c); })).base(), s.end());
	return s;
	}
};




#endif /* XML_PARAMETERLISTARRAY_2_H_ */
