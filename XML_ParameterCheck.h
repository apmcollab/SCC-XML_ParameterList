/*
 * XML_ParameterCheck.h
 *
 *  Created on: Jun 15, 2021
 *      Author: anderson
 */

#include <string>
#include <stdexcept>

#include "XML_ParameterListArray.h"

// A utility class for checking the existence of parameters in an input
// XML_ParameterListArray and composing an error message string when
// a parameter is not found.
//
// This class does not throw exceptions as these are meant to be handled by the
// code/routines utilizing this class.
//
/*
#############################################################################
#
# Copyright  2021- Chris Anderson
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
#ifndef XML_PARAMETER_CHECK_
#define XML_PARAMETER_CHECK_

class XML_ParameterCheck
{
public:

void ifErrorThrowException(bool errorFlag, std::string errMsg, std::string fileName = "")
{
	if(not errorFlag) return;

	if(fileName.empty())
	{
		throw std::runtime_error(errMsg);
	}

	std::string fullErrMsg = "\nXXXXXXXXXXX Parameter Input Error XXXXXXXXXXXXX \n";
	fullErrMsg.append("Parameter file : ");
	fullErrMsg.append(fileName + "\n");
	fullErrMsg.append(errMsg);

	throw std::runtime_error(fullErrMsg);
}

void ifErrorAbort(bool errorFlag, std::string errMsg, std::string fileName = "")
{
	if(not errorFlag) return;

	if(fileName.empty())
	{
    printf("\n %s \n",errMsg.c_str());
    printf("XXXX Execution Complete XXXXX\n\n\n");
	}

	std::string fullErrMsg = "\nXXXXXXXXXXX Parameter Input Error XXXXXXXXXXXXX \n";
	fullErrMsg.append("Parameter file : ");
	fullErrMsg.append(fileName + "\n");
	fullErrMsg.append(errMsg);
	printf("\n %s \n",fullErrMsg.c_str());
	printf("XXXX Execution Complete XXXXX\n\n\n");
	exit(1);
}

// Returns true if an error occurs

bool checkParameterError(const std::string& routineName, const XML_ParameterListArray& paramList,
					const std::string& listName,
					std::string& errMsg)
{
	bool abortOnErrorFlag = paramList.getAbortOnErrorFlag();
	paramList.clearAbortOnErrorFlag();

    bool errorFlag = false;

	if(not paramList.isParameterList(listName))
	{
		errMsg.append("\nXXXXX ");
	    errMsg.append(routineName  + " Parameter Error \n");
		errMsg.append("Parameter list missing : ");
		errMsg.append(listName);
		errMsg.append("\n");
		errorFlag = true;
	}

	if(abortOnErrorFlag)
	{
	paramList.setAbortOnErrorFlag();
	}

	return errorFlag;
}

// Returns true if an error occurs

bool checkParameterError(const std::string& routineName, const XML_ParameterListArray& paramList,
		            const std::string& paramName,
					const std::string& listName,
					std::string& errMsg)
{
	bool abortOnErrorFlag = paramList.getAbortOnErrorFlag();
	paramList.clearAbortOnErrorFlag();

    bool errorFlag = false;

	if(not paramList.isParameterList(listName))
	{
		errMsg.append("\nXXXXX ");
	    errMsg.append(routineName  + " Parameter Error \n");
		errMsg.append("Parameter list missing : ");
		errMsg.append(listName);
		errMsg.append("\n");
		errorFlag = true;
	}
	if((not errorFlag)&&(not paramList.isParameter(paramName,listName)))
	{
		errMsg.append("\nXXXXX ");
	    errMsg.append(routineName  + " Parameter Error \n");
		errMsg.append("Parameter missing : ");
		errMsg.append(paramName);
	    errMsg.append("\n");
		errMsg.append("Parameter list    : ");
		errMsg.append(listName);
		errMsg.append("\n");
		errorFlag = true;
	}

	if(abortOnErrorFlag)
	{
	paramList.setAbortOnErrorFlag();
	}

	return errorFlag;
}

// Checks for first (0th index) instance of the specified child parameter

// Returns true if an error occurs

bool checkParameterError(const std::string& routineName, const XML_ParameterListArray& paramList,
		            const std::string& paramChildName,
		            const std::string& paramName,
				    const std::string& listName,
				    std::string& errMsg)
{
	int instanceID = 0;
	return checkParameterError(routineName, paramList, instanceID, paramChildName, paramName,listName,errMsg);
}

// Returns true if an error occurs

bool checkParameterError(const std::string& routineName, const XML_ParameterListArray& paramList,
		            int instanceID,
				    const std::string& paramChildName,
		            const std::string& paramName,
				    const std::string& listName,
				     std::string& errMsg)
{
    bool abortOnErrorFlag = paramList.getAbortOnErrorFlag();
	paramList.clearAbortOnErrorFlag();

	bool errorFlag = false;


	if(not paramList.isParameterList(listName))
	{
		errMsg.append("\nXXXXX ");
	    errMsg.append(routineName  + " Parameter Error \n");
		errMsg.append("Parameter list missing : ");
		errMsg.append(listName);
		errMsg.append("\n");
		errorFlag = true;
	}


	if((not errorFlag)&&(not paramList.isParameter(paramName,listName)))
	{
		errMsg.append("\nXXXXX ");
	    errMsg.append(routineName  + " Parameter Error \n");
		errMsg.append("Parameter missing : ");
		errMsg.append(paramName);
	    errMsg.append("\n");
		errMsg.append("Parameter list    : ");
		errMsg.append(listName);
		errMsg.append("\n");
		errorFlag = true;
	}

	// Checks for first (0th) instance of child parameter

    if((not errorFlag)&&(not paramList.isParameterInstanceChildValue(instanceID,paramChildName,paramName,listName)))
	{
		errMsg.append("\nXXXXX ");
	    errMsg.append(routineName + " Parameter Error \n");
		errMsg.append("Parameter child missing : ");
		errMsg.append(paramChildName);
	    errMsg.append("\n");
		errMsg.append("Parameter          : ");
	    errMsg.append(paramName);
	    errMsg.append("\n");
	    errMsg.append("Parameter instance : ");
	    errMsg.append(std::to_string(instanceID +1));
	    errMsg.append("\n");
		errMsg.append("Parameter list     : ");
		errMsg.append(listName);
		errMsg.append("\n");
		errorFlag = true;
	}

    if(abortOnErrorFlag)
	{
	paramList.setAbortOnErrorFlag();
	}

    return errorFlag;
}


};




#endif /* COMPONENTS_XML_PARAMETERLIST_XML_PARAMETERCHECK_H_ */
