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


#ifndef TIMESLICEDGEOMETRY_H_HEADER_INCLUDED_C1EBD0AD
#define TIMESLICEDGEOMETRY_H_HEADER_INCLUDED_C1EBD0AD

#include "mitkGeometry3D.h"

namespace mitk {

//##Documentation
//## @brief Describes XXX
//##
//## @ingroup Geometry
class TimeSlicedGeometry : public Geometry3D
{
public:
  mitkClassMacro(TimeSlicedGeometry, Geometry3D);

  itkNewMacro(Self);

  //## @brief Get the number of time-steps
  itkGetConstMacro(TimeSteps, unsigned int);

  virtual const TimeBounds& GetTimeBoundsInMS() const;
  //##Documentation
  //## @brief Set/Get whether the TimeSlicedGeometry is evenly-timed (m_EvenlyTimed)
  //## 
  //## if (a) we don't have a Geometry3D stored for the requested time, 
  //## (b) m_EvenlyTimed is activated and (c) the first geometry (t=0) 
  //## is set, then we clone the geometry and set the m_TimeBoundsInMS accordingly.
  //## \sa GetGeometry3D
 	itkGetConstMacro(EvenlyTimed, bool);
  virtual void SetEvenlyTimed(bool on = true);

  //##Documentation
  //## @brief Set Geometry3D at time @a t.
  virtual bool SetGeometry3D(mitk::Geometry3D* geometry3D, int t);
  
  virtual mitk::Geometry3D* GetGeometry3D(int t) const;

  virtual const mitk::BoundingBox* GetBoundingBox() const;

  virtual bool IsValidTime(int t) const;

  virtual int MSToTimeStep(mitk::ScalarType time_in_ms) const;

  virtual mitk::ScalarType TimeStepToMS(int timestep) const;
protected:
  virtual void Initialize(unsigned int timeSteps);
public:
  //##Documentation
  //## @brief Completely initialize this instance as evenly-timed with @timeSteps geometries 
  //## identical to the provided Geometry3D except of the time bounds.
  //##
  virtual void InitializeEvenlyTimed(mitk::Geometry3D* geometry3D, unsigned int timeSteps);

  //##Documentation
  //## @brief duplicates the geometry
  virtual AffineGeometryFrame3D::Pointer Clone() const;
protected:
  TimeSlicedGeometry();

  virtual ~TimeSlicedGeometry();

  void InitializeGeometry(Self * newGeometry) const;

  mutable std::vector<Geometry3D::Pointer> m_Geometry3Ds;

  unsigned int m_TimeSteps;

  bool m_EvenlyTimed;
};

} // namespace mitk

#endif /* TIMESLICEDGEOMETRY_H_HEADER_INCLUDED_C1EBD0AD */
