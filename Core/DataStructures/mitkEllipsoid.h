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


#ifndef MITKELLIPSEDATA_H_HEADER_INCLUDED
#define MITKELLIPSEDATA_H_HEADER_INCLUDED

#include "mitkBoundingObject.h"

namespace mitk {

//##Documentation
//## @brief Data class containing an ellipsoid
//## @ingroup Data
class Ellipsoid : public BoundingObject
{
public:
  mitkClassMacro(Ellipsoid, BoundingObject);  
  itkNewMacro(Self);

  virtual mitk::ScalarType GetVolume();
  virtual bool IsInside(Point3D p);

protected:
  Ellipsoid();
  virtual ~Ellipsoid();
};

}
#endif /* MITKELLIPSEDATA_H_HEADER_INCLUDED */
