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


#include "mitkOperationEvent.h"

//##ModelId=3E9B07B40374
int mitk::OperationEvent::m_CurrObjectEventId = 0;
//##ModelId=3E9B07B5002B
int mitk::OperationEvent::m_CurrGroupEventId = 0;

bool mitk::OperationEvent::m_IncrObjectEventId = false;
bool mitk::OperationEvent::m_IncrGroupEventId = false;


//##ModelId=3E5F610D00BB
mitk::Operation* mitk::OperationEvent::GetOperation()
{
	return m_Operation;
}

//##ModelId=3E957AE700E6
mitk::OperationEvent::OperationEvent(OperationActor* destination, 
									 Operation* operation, Operation* undoOperation)
: m_Destination(destination), m_Operation(operation), m_UndoOperation(undoOperation),
  m_Swaped(false)
{
  m_ObjectEventId = this->GetCurrObjectEventId();
  m_GroupEventId = this->GetCurrGroupEventId();
}

//##ModelId=3F0451960212
mitk::OperationEvent::~OperationEvent()
{
}

//##ModelId=3E957C1102E3
//##Documentation
//##  swaps the Undo and Redo- operation and changes m_Swaped
void mitk::OperationEvent::SwapOperations()
{
    if (m_Operation == NULL) 
        return;

	Operation *tempOperation = m_Operation;
	m_Operation = m_UndoOperation;
	m_UndoOperation = tempOperation;
	if (m_Swaped)
		m_Swaped = false;
	else 
		m_Swaped = true;
}

void mitk::OperationEvent::ExecuteIncrement()
{
  if (m_IncrObjectEventId)
  {
    ++m_CurrObjectEventId;
    m_IncrObjectEventId = false;
  }
  
  if (m_IncrGroupEventId)
  {
    ++m_CurrGroupEventId;
    m_IncrGroupEventId = false;
  }
}

//##ModelId=3E9B07B501A7
int mitk::OperationEvent::GetCurrObjectEventId()
{
	return m_CurrObjectEventId;
}

//##ModelId=3E9B07B50220
int mitk::OperationEvent::GetCurrGroupEventId()
{
	return m_CurrGroupEventId;
}

//##ModelId=3EF099E90289
void mitk::OperationEvent::IncCurrObjectEventId()
{
	m_IncrObjectEventId = true;
}

//##ModelId=3EF099E90269
void mitk::OperationEvent::IncCurrGroupEventId()
{
	m_IncrGroupEventId = true;
}

//##ModelId=3E9B07B502AC
mitk::OperationActor* mitk::OperationEvent::GetDestination() 
{
	return m_Destination;
}

//##ModelId=3EF099E90259
int mitk::OperationEvent::GetObjectEventId()
{
	return m_ObjectEventId;
}

//##ModelId=3EF099E90249
int mitk::OperationEvent::GetGroupEventId()
{
	return m_GroupEventId;
}

