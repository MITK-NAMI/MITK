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


#include <mitkPointSet.h>
#include <mitkVector.h>
#include <mitkPointOperation.h>
#include <mitkInteractionConst.h>


#include <fstream>
int mitkPointSetTest(int argc, char* argv[])
{
  //Create PointSet
  mitk::PointSet::Pointer pointSet;
  pointSet = mitk::PointSet::New();

  //try to get the itkPointSet
  mitk::PointSet::DataType::Pointer itkdata = NULL;
  itkdata = pointSet->GetPointSet();
  if (itkdata.IsNull())
  {
    std::cout<<"[FAILED]"<<std::endl;
    return EXIT_FAILURE;
  }

  //fresh PointSet has to be empty!
  if (pointSet->GetSize() != 0)
  {
    std::cout<<"[FAILED]"<<std::endl;
    return EXIT_FAILURE;
  }

  //create an operation and add a point.
  int position = 0;
  mitk::Point3D point;
  point.Fill(1);
  mitk::PointOperation* doOp = new mitk::PointOperation(mitk::OpINSERT, point, position);
	pointSet->ExecuteOperation(doOp);

  //now check new condition!
  if ( (pointSet->GetSize()!=1) ||
      (!pointSet->IndexExists(position)))
  {
    std::cout<<"[FAILED]"<<std::endl;
    return EXIT_FAILURE;
  }
  
  mitk::Point3D tempPoint;
  tempPoint.Fill(0);
  tempPoint = pointSet->GetPoint(position);
  if (tempPoint != point)
  {
    std::cout<<"[FAILED]"<<std::endl;
    return EXIT_FAILURE;
  }


  //well done!!! Passed!
  std::cout<<"[PASSED]"<<std::endl;

  std::cout<<"[TEST DONE]"<<std::endl;
  return EXIT_SUCCESS;
}
