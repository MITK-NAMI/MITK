/*=========================================================================

Program:   Medical Imaging & Interaction Toolkit
Module:    $RCSfile$
Language:  C++
Date:      $Date$
Version:   $Revision$

Copyright (c) German Cancer Research Center, Division of Medical and
Biological Informatics. All rights reserved.
See MITKCopyright.txt or http://www.mitk.org/copyright.html for details.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notices for more information.

=========================================================================*/


#ifndef MITKPOINTOPERATION_H
#define MITKPOINTOPERATION_H

#include "mitkCommon.h"
#include "mitkOperation.h"
#include "mitkVector.h"



namespace mitk {

//##ModelId=3F0189F0016F
//##Documentation
//## @brief Operation, that handles all aktions on one Point.
//##
//## @ingroup Undo
//##
//## Stores everything for Adding, Moving and Deleting a Point.
class PointOperation : public Operation
{
  public:
  //##ModelId=3F0189F003A2
	//##Documentation
	//##@brief Operation, that handles all aktions on one Point.
	//##
	//## @params
	//## operationType is the type of that operation (see mitkOperation.h; e.g. move or add; Information for StateMachine::ExecuteOperation());
	//## point is the information of the point to add or is the information to change a point into; index is e.g. the position in a
	//## list which describes the element to change
  PointOperation(OperationType operationType,	Point3D point, int index = -1, bool selected = true, PointSpecificationType type = PTUNDEFINED);
  
  //##ModelId=3F0189F003B2
  virtual ~PointOperation();

  //##ModelId=3F0189F003B4
	Point3D GetPoint();

  //##ModelId=3F0189F003B5
	int GetIndex();

  bool GetSelected();

  PointSpecificationType GetPointType();

  private:
  //##ModelId=3F0189F00393
	Point3D m_Point;

  //##ModelId=3F0189F003A1
  //##Documentation
  //##@brief to declare an index where to store the point in data
	int m_Index;

  //to declare weather the point is selected or deselected
  bool m_Selected;
  
  //##Documentation
  //##@brief to describe the type of the point. See enum PointSpecification for different types
  PointSpecificationType m_Type;
};
}//namespace mitk
#endif /* MITKPOINTOPERATION_H*/

