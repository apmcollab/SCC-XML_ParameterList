/*
 * XML_ParameterListUtilities.h
 *
 *  Created on: Mar 22, 2013
 *      Author: anderson
 */
/*
#############################################################################
#
# Copyright  2013- Chris Anderson
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the Lesser GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# For a copy of the GNU General Public License see
# <http://www.gnu.org/licenses/>.
#
#############################################################################
*/
#include <sys/stat.h>

#include <vector>
#include <algorithm>
#include <string>


#ifndef XML_PARAMETER_LIST_UTILITIES_
#define XML_PARAMETER_LIST_UTILITIES_

#include "XML_ParameterListArray.h"

#define _LOCAL_PATH_MAX 4096

class XML_ParameterListUtilities
{
public:

XML_ParameterListUtilities(){};


//
// This member function takes the input paramList and inserts all parameterLists
// whose name is not an element of the exclusions vector of names into
// the outputList.
//
// The outputList must be initialized before calling this routine, or
// created with an call to createParameterListArray(...)
//

void createDuplicateEntries(XML_ParameterListArray& paramList,
std::vector<std::string>& exclusions,    XML_ParameterListArray& outputList)
{
    std::vector<std::string> paramListNames;
    std::vector<std::string> paramNames;
    std::vector<std::string> paramChildNames;
    bool excludeFlag;
    bool abortOnErrorFlag  = paramList.abortOnErrorFlag;
    XML_dataType XMLvalue;

    std::vector<std::string>::iterator it;

    long      paramInstanceIndex;
    paramList.getParameterListNames(paramListNames);
    for(long k = 0; k < (long)paramListNames.size(); k++)
    {

    excludeFlag = true;
    it = std::find(exclusions.begin(),exclusions.end(),paramListNames[k]);
    if(it == exclusions.end()){excludeFlag = false;}

    if(not excludeFlag)
    {

    outputList.addParameterList(paramListNames[k].c_str());

    paramNames.clear();
    paramList.getParameterNames(paramListNames[k].c_str(),paramNames);

    // Remove duplicate parameter names

    std::sort(paramNames.begin(),paramNames.end());
    it = std::unique(paramNames.begin(), paramNames.end());
    paramNames.resize(it - paramNames.begin());

    for(long j = 0; j < (long)paramNames.size(); j++)
    {
    for(paramInstanceIndex = 0;
    paramInstanceIndex < (long)paramList.getParameterInstanceCount(paramNames[j].c_str(),paramListNames[k].c_str());
    paramInstanceIndex++)
    {

    paramChildNames.clear();
    paramList.getParameterChildNames(paramInstanceIndex, paramNames[j].c_str(),  paramListNames[k].c_str(), paramChildNames);
    if(paramChildNames.size() == 0)
    {
    outputList.addParameter(paramList.getParameterValue(paramNames[j].c_str(),paramListNames[k].c_str()),
                            paramNames[j].c_str(), paramListNames[k].c_str());
    }
    else // add parameter and child parameter values
    {
    outputList.addParameter(paramNames[j].c_str(), paramListNames[k].c_str());


    // Add parameter value if it is specified

    paramList.clearAbortOnErrorFlag();
    XMLvalue = paramList.getParameterValue(paramNames[j].c_str(), paramListNames[k].c_str());
    if(not XMLvalue.isNull())
    {outputList.setParameter(XMLvalue,paramNames[j].c_str(), paramListNames[k].c_str());}
    else
    {paramList.clearError();}
    if(abortOnErrorFlag) paramList.setAbortOnErrorFlag();


    // Add child parameter values

    for(long i = 0; i < (int)paramChildNames.size(); i++)
    {
    outputList.addParameterInstanceChild(paramList.getParameterInstanceChildValue(paramInstanceIndex,
                                         paramChildNames[i].c_str(),
                                         paramNames[j].c_str(),
                                         paramListNames[k].c_str()),
                                         paramInstanceIndex,
                                         paramChildNames[i].c_str(), paramNames[j].c_str(), paramListNames[k].c_str());
    }

    }}

    } // parameter instances

    }}
}

//
// This routine copies the parameter list from the input XML_ParameterListArray. An
// error message is generated if the parameter does not exist.
//
void copyParameterList(const XML_ParameterListArray& paramListArray,const std::string& paramList, XML_ParameterListArray& outputList)
{
copyParameterList(paramListArray,paramList.c_str(),  outputList);
}


void copyParameterList(const XML_ParameterListArray& paramListArray,const char* paramList, XML_ParameterListArray& outputList)
{
    bool errorFlag = true;

    std::vector<std::string> paramListArrayNames;
    std::vector<std::string> paramNames;
    std::vector<std::string> paramChildNames;
    bool abortOnErrorFlag  = paramListArray.abortOnErrorFlag;
    XML_dataType XMLvalue;

    std::vector<std::string>::iterator it;

    long      paramInstanceIndex;
    paramListArray.getParameterListNames(paramListArrayNames);
    for(long k = 0; k < (long)paramListArrayNames.size(); k++)
    {



    if(paramListArrayNames[k].compare(paramList) == 0)
    {
    errorFlag = false;
    outputList.addParameterList(paramListArrayNames[k].c_str());

    paramNames.clear();
    paramListArray.getParameterNames(paramListArrayNames[k].c_str(),paramNames);

    // Remove duplicate parameter names

    std::sort(paramNames.begin(),paramNames.end());
    it = std::unique(paramNames.begin(), paramNames.end());
    paramNames.resize(it - paramNames.begin());

    for(long j = 0; j < (long)paramNames.size(); j++)
    {
    for(paramInstanceIndex = 0;
    paramInstanceIndex < (long)paramListArray.getParameterInstanceCount(paramNames[j].c_str(),paramListArrayNames[k].c_str());
    paramInstanceIndex++)
    {

    paramChildNames.clear();
    paramListArray.getParameterChildNames(paramInstanceIndex, paramNames[j].c_str(),  paramListArrayNames[k].c_str(), paramChildNames);
    if(paramChildNames.size() == 0)
    {

    outputList.addParameter(paramListArray.getParameterInstanceValue(paramInstanceIndex,paramNames[j].c_str(),paramListArrayNames[k].c_str()),
                            paramNames[j].c_str(), paramListArrayNames[k].c_str());
    }
    else // add parameter and child parameter values
    {
    outputList.addParameter(paramNames[j].c_str(), paramListArrayNames[k].c_str());

    // Add parameter value if it is specified

    paramListArray.clearAbortOnErrorFlag();

    XMLvalue = paramListArray.getParameterInstanceValue(paramInstanceIndex,paramNames[j].c_str(), paramListArrayNames[k].c_str());
    if(not XMLvalue.isNull())
    {outputList.setParameter(XMLvalue,paramNames[j].c_str(), paramListArrayNames[k].c_str());}
    else
    {paramListArray.clearError();}
    if(abortOnErrorFlag) paramListArray.setAbortOnErrorFlag();


    // Add child parameter values

    for(long i = 0; i < (int)paramChildNames.size(); i++)
    {
    outputList.addParameterInstanceChild(paramListArray.getParameterInstanceChildValue(paramInstanceIndex,
                                         paramChildNames[i].c_str(),
                                         paramNames[j].c_str(),
                                         paramListArrayNames[k].c_str()),
                                         paramInstanceIndex,
                                         paramChildNames[i].c_str(), paramNames[j].c_str(), paramListArrayNames[k].c_str());
    }

    }}

    } // parameter instances

    }}

    //
    // Error checking : Abort if input paramListArray has abort on error flag set.
    //
	if((abortOnErrorFlag)&&(errorFlag))
	{
	printf("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
	printf("XML_ParameterListArrayUtilities Class Error \n");
	printf("copyParameterList(...) \n\n");
	printf("Copying a parameter list that does not exist in input XML_ParameterListArray. \n");
	printf("Input XML_ParameterListArray : ");
	printf("%s \n",paramListArray.getParameterListArrayName());
	printf("Offending parameterListName  : ");
	printf("%s \n",paramList);
	printf("\nXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
	printf("\n");
	exit(0);
	}
}


//
// This routine assigns the values of all parameters of the parameters lists specified in the input parameter list
// to the corresponding parameter values of the identically named parameter lists in the specified output paramListArray.
//
//  An error message is generated if the parameter list, or parameter list parameter,
//  does not exist in the outputList.
//
void assignParameters(XML_ParameterListArray& paramListArray,  XML_ParameterListArray& outputList)
{
    std::vector<std::string> paramListArrayNames;
    paramListArray.getParameterListNames(paramListArrayNames);
    for(long k = 0; k < (long)paramListArrayNames.size(); k++)
    {
    assignParameters(paramListArray,paramListArrayNames[k].c_str(), outputList);
    }
}

//
// This routine assigns the values of all parameters specified in the input parameter list
// contained in the input paramListArray to the corresponding values of the identically named
// parameter list in the specified output paramListArray.
//
//  An error message is generated if the parameter list, or parameter list parameter,
//  does not exist in the outputList.
//
void assignParameters(XML_ParameterListArray& paramListArray,const std::string& paramList, XML_ParameterListArray& outputList)
{
    assignParameters(paramListArray,paramList.c_str(), outputList);
}

void assignParameters(XML_ParameterListArray& paramListArray,const char* paramList, XML_ParameterListArray& outputList)
{
    bool errorFlag = true;

    std::vector<std::string> paramListArrayNames;
    std::vector<std::string> paramNames;
    std::vector<std::string> paramChildNames;
    bool abortOnErrorFlag  = paramListArray.abortOnErrorFlag;
    XML_dataType XMLvalue;

    std::vector<std::string>::iterator it;

    long      paramInstanceIndex;
    paramListArray.getParameterListNames(paramListArrayNames);
    for(long k = 0; k < (long)paramListArrayNames.size(); k++)
    {

    if(paramListArrayNames[k].compare(paramList) == 0)
    {
    errorFlag = false;
    paramNames.clear();
    paramListArray.getParameterNames(paramListArrayNames[k].c_str(),paramNames);

    // Remove duplicate parameter names

    std::sort(paramNames.begin(),paramNames.end());
    it = std::unique(paramNames.begin(), paramNames.end());
    paramNames.resize(it - paramNames.begin());

    for(long j = 0; j < (long)paramNames.size(); j++)
    {
    for(paramInstanceIndex = 0;
    paramInstanceIndex < (long)paramListArray.getParameterInstanceCount(paramNames[j].c_str(),paramListArrayNames[k].c_str());
    paramInstanceIndex++)
    {

    paramChildNames.clear();
    paramListArray.getParameterChildNames(paramInstanceIndex, paramNames[j].c_str(),  paramListArrayNames[k].c_str(), paramChildNames);
    if(paramChildNames.size() == 0)
    {
    outputList.setParameter(paramListArray.getParameterValue(paramNames[j].c_str(),paramListArrayNames[k].c_str()),
    paramNames[j].c_str(), paramListArrayNames[k].c_str());
    }
    else // set parameter and child parameter values
    {
    paramListArray.clearAbortOnErrorFlag();
    XMLvalue = paramListArray.getParameterValue(paramNames[j].c_str(), paramListArrayNames[k].c_str());
    if(not XMLvalue.isNull())
    {outputList.setParameter(XMLvalue,paramNames[j].c_str(), paramListArrayNames[k].c_str());}
    else
    {paramListArray.clearError();}
    if(abortOnErrorFlag) paramListArray.setAbortOnErrorFlag();


    // Add child parameter values

    for(long i = 0; i < (int)paramChildNames.size(); i++)
    {
    outputList.setParameterInstanceChildValue(paramListArray.getParameterInstanceChildValue(paramInstanceIndex,
                                         paramChildNames[i].c_str(),
                                         paramNames[j].c_str(),
                                         paramListArrayNames[k].c_str()),
                                         paramInstanceIndex,
                                         paramChildNames[i].c_str(), paramNames[j].c_str(), paramListArrayNames[k].c_str());
    }

    }}

    } // parameter instances

    }}

    //
    // Error checking : Abort if input paramListArray has abort on error flag set.
    //
	if((abortOnErrorFlag)&&(errorFlag))
	{
	printf("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
	printf("XML_ParameterListArrayUtilities Class Error \n");
	printf("assignParameters(...) \n\n");
	printf("Attempting to assign parameters using a parameter list \n");
    printf("that does not exist in the input XML_ParameterListArray. \n\n");
	printf("Input XML_ParameterListArray : ");
	printf("%s \n",paramListArray.getParameterListArrayName());
	printf("Offending parameterListName  : ");
	printf("%s \n",paramList);
	printf("\nXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
	printf("\n");
	exit(0);
	}
}
//
// This method adds the parameter list outputParamList to the ParameterListArray
// outputList and then replicates the parameters in the input parameter list into
// that parameter list (outputParamList).
//
void copyParameterList(const XML_ParameterListArray& paramListArray,
const char* paramList, XML_ParameterListArray& outputList, const std::string& outputParamList)
{
    outputList.addParameterList(outputParamList);

    std::vector<std::string> paramListArrayNames;
    std::vector<std::string> paramNames;
    std::vector<std::string> paramChildNames;
    XML_dataType XMLvalue;

    std::vector<std::string>::iterator it;

    long      paramInstanceIndex;
    paramNames.clear();
    paramListArray.getParameterNames(paramList,paramNames);

    // Remove duplicate parameter names

    std::sort(paramNames.begin(),paramNames.end());
    it = std::unique(paramNames.begin(), paramNames.end());
    paramNames.resize(it - paramNames.begin());

    for(long j = 0; j < (long)paramNames.size(); j++)
    {
    for(paramInstanceIndex = 0;
    paramInstanceIndex < (long)paramListArray.getParameterInstanceCount(paramNames[j],paramList);
    paramInstanceIndex++)
    {

    paramChildNames.clear();
    paramListArray.getParameterChildNames(paramInstanceIndex, paramNames[j], paramList, paramChildNames);

    if(paramChildNames.size() == 0)
    {

    outputList.addParameter(
    paramListArray.getParameterInstanceValue(paramInstanceIndex, paramNames[j],paramList),
    paramNames[j], outputParamList);
    }
    else // add parameter and child parameter values
    {
    outputList.addParameter(paramNames[j], outputParamList);

    // Add parameter value if it is specified

    paramListArray.clearAbortOnErrorFlag();

    XMLvalue = paramListArray.getParameterInstanceValue(paramInstanceIndex,paramNames[j], paramList);
    if(not XMLvalue.isNull())
    {outputList.setParameter(XMLvalue,paramNames[j], outputParamList);}

    // Add child parameter values

    for(long i = 0; i < (int)paramChildNames.size(); i++)
    {
    outputList.addParameterInstanceChild(paramListArray.getParameterInstanceChildValue(paramInstanceIndex,
                                         paramChildNames[i],
                                         paramNames[j],
                                         paramList),
                                         paramInstanceIndex,
                                         paramChildNames[i], paramNames[j], outputParamList);
    }

    }}

    } // parameter instances

}



//
// Given a specified parameter child (named parameterChileName) whose
// value is a file, this routine replaces that file name with a full
// path file name based on the extraction of the base path using the
// realpath system call on the specified file. If the file does
// not exist then this routine is a no-op on that file name.
//
void insertFullPathNames(XML_ParameterListArray& paramList, const std::string& parameterChildName, const std::string& parameterName,const std::string&  parameterListName)
{
	insertFullPathNames(paramList,parameterChildName.c_str(), parameterName.c_str(), parameterListName.c_str());
}

void insertFullPathNames(XML_ParameterListArray& paramList,
const char* parameterChildName, const char* parameterName,
const char*  parameterListName)
{
    std::string fileName;

    std::string basePath;
    std::string baseName;
    std::string fullFileName;

	for(size_t i = 0; i < paramList.getParameterInstanceCount(parameterName, parameterListName); i++)
	{
	if(paramList.isParameterInstanceChildValue(i,parameterChildName,parameterName, parameterListName))
	{
	fileName = (std::string)paramList.getParameterInstanceChildValue(i,parameterChildName,
	parameterName, parameterListName);
	basePath = getBasePath(fileName);
	baseName = getBaseName(fileName);

	if(basePath.empty())
	{
	return;
	}
    else
    {
	fullFileName = basePath;
	fullFileName.append("/");
	fullFileName.append(baseName);
	}
	paramList.setParameterInstanceChildValue(fullFileName,i,parameterChildName,parameterName, parameterListName);
	}
	}
}

//
// Given a specified parameter child (named parameterChileName) whose
// value is a file, this routine replaces that file name with a full
// path file name whose base path is specified.
//
void insertFullPathNames(const std::string& basePath, XML_ParameterListArray& paramList,
const std::string& parameterChildName, const std::string& parameterName, const std::string&  parameterListName)
{
insertFullPathNames(basePath.c_str(), paramList, parameterChildName.c_str(), parameterName.c_str(), parameterListName.c_str());
}

void insertFullPathNames(const char* basePath, XML_ParameterListArray& paramList,
const char* parameterChildName, const char* parameterName,
const char*  parameterListName)
{
    std::string fileName;
    std::string baseName;
    std::string fullFileName;

	for(size_t i = 0; i < paramList.getParameterInstanceCount(parameterName, parameterListName); i++)
	{
	if(paramList.isParameterInstanceChildValue(i,parameterChildName,parameterName, parameterListName))
	{
	fileName = (std::string)paramList.getParameterInstanceChildValue(i,parameterChildName,parameterName, parameterListName);
	baseName = getBaseName(fileName);
	fullFileName = basePath;
	fullFileName.append("/");
	fullFileName.append(baseName);
	paramList.setParameterInstanceChildValue(fullFileName,i,parameterChildName,parameterName, parameterListName);
	}
	}
}

//
//  Since code w.r.t. file and path names os OS specific,
//  separate versions are required for
//  Linux/Unix systems (the default) and Microsoft OS's. 
//
std::string getBasePath(const std::string& fileName)
{
	const char *symlinkpath = fileName.c_str();
	char *actualpath;
    char pathBuffer[_LOCAL_PATH_MAX];
	std::string actualPath;
	std::string   basePath;
	#ifndef _MSC_VER
    actualpath =   realpath(symlinkpath, pathBuffer);
    #else
    actualpath =  _fullpath(pathBuffer,symlinkpath,_LOCAL_PATH_MAX);
    #endif

    if (actualpath != NULL)
    {
        actualPath.assign(actualpath);
    	basePath   = actualPath.substr(0,actualPath.find_last_of("/\\"));
    	return basePath;
   }
   return basePath;
}

std::string getCWD()
{
	char *actualpath;
	std::string actualPath;
	char   pathBuffer[_LOCAL_PATH_MAX];

#ifndef _MSC_VER
	actualpath = realpath("./", pathBuffer);
#else
	actualpath = _fullpath(pathBuffer, "./", _LOCAL_PATH_MAX);
#endif

    if (actualpath != NULL)
    {
    	actualPath.assign(actualpath);
   }
   return actualPath;
}

std::string getBaseName(std::string& fileName)
{
	const char *symlinkpath = fileName.c_str();
	char *actualpath;

	std::string actualPath;
	std::string   baseName;
	char   pathBuffer[_LOCAL_PATH_MAX];

#ifndef _MSC_VER
	actualpath = realpath(symlinkpath, pathBuffer);
#else
	actualpath = _fullpath(pathBuffer, symlinkpath, _LOCAL_PATH_MAX);
#endif
    if (actualpath != NULL)
    {
    	actualPath.assign(actualpath);
    	baseName = actualPath.substr(actualPath.find_last_of("/\\")+1);
    	return baseName;
   }
   else // Just strip away all directory modifiers
   {
	baseName = fileName.substr(fileName.find_last_of("/\\")+1);
   }
   return baseName;
}

bool fileExists(const std::string fileName)
{
	struct stat buffer;
	return (stat(fileName.c_str(), &buffer) == 0);
}
};

#undef  _LOCAL_PATH_MAX

#endif /* _XML_ParameterListUtilities_ */
