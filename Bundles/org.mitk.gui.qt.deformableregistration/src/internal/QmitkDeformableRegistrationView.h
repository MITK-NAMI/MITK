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


#ifndef QMITKDEFORMABLEREGISTRATION_H
#define QMITKDEFORMABLEREGISTRATION_H

#include "QmitkFunctionality.h"

#include "ui_QmitkDeformableRegistrationViewControls.h"

#include "../DeformableregistrationDll.h"

//class QmitkStdMultiWidget;
//class QmitkDeformableRegistrationControls;

/*!
\brief The DeformableRegistration functionality is used to perform deformable registration.

This functionality allows you to register two 2D as well as two 3D images in a non rigid manner.
Register means to align two images, so that they become as similar as possible. 
Therefore you can select from different deformable registration methods. 
Registration results will directly be applied to the Moving Image. The result is shown in the multi-widget. 

For more informations see: \ref QmitkDeformableRegistrationUserManual

\sa QmitkFunctionality
\ingroup Functionalities
\ingroup DeformableRegistration
*/
class DEFORMABLEREGISTRATION_EXPORTS QmitkDeformableRegistrationView : public QObject, public QmitkFunctionality
{  
  Q_OBJECT
  
  public:  

    typedef std::set<mitk::DataTreeNode*> invisibleNodesList;

    /*!  
    \brief default constructor  
    */  
    QmitkDeformableRegistrationView(QObject *parent=0, const char *name=0);

    /*!  
    \brief default destructor  
    */  
    virtual ~QmitkDeformableRegistrationView();

    /*!  
    \brief method for creating the widget containing the application   controls, like sliders, buttons etc.  
    */  
    //virtual QWidget * CreateControlWidget(QWidget *parent);

    virtual void CreateQtPartControl(QWidget *parent);

    virtual void StdMultiWidgetAvailable (QmitkStdMultiWidget &stdMultiWidget);
    virtual void StdMultiWidgetNotAvailable();

    /*!  
    \brief method for creating the applications main widget  
    */  
    //virtual QWidget * CreateMainWidget(QWidget * parent);

    /*!  
    \brief method for creating the connections of main and control widget  
    */  
    virtual void CreateConnections();

    /*!  
    \brief method for creating an QAction object, i.e. button & menu entry  @param parent the parent QWidget  
    */  
    //virtual QAction * CreateAction(QActionGroup *parent);

    /*!  
    \brief Method which is called when this functionality is selected in MITK  
    */ 
    virtual void Activated();

    /*!  
    \brief Method which is called whenever the functionality is deselected in MITK  
    */
    virtual void Deactivated();

  signals:

    void reinitFixed(const mitk::Geometry3D *);

    void reinitMoving(const mitk::Geometry3D *);

    void calculateDemonsRegistration();

  protected slots:  
    
    /*!
    \brief Called whenever the data storage has changed. 
    */
    virtual void DataStorageChanged();

    mitk::NodePredicateBase::Pointer GetMovingImagePredicate();
    
    /*!
    * sets the fixed Image according to TreeNodeSelector widget
    */
    void FixedSelected(int = 0);
    
    /*!
    * sets the moving Image according to TreeNodeSelector widget
    */
    void MovingSelected(int = 0);

    /*!
    * checks if registration is possible
    */
    bool CheckCalculate();

    /*!
    * stores whether the image will be shown in grayvalues or in red for fixed image and green for moving image
    * @param if true, then images will be shown in red and green
    */
    void ShowRedGreen(bool show);

    /*!
    * set the selected opacity for moving image
    * @param opacity the selected opacity
    */
    void OpacityUpdate(float opacity);

    /*!  
    \brief Sets the selected opacity for moving image

    @param opacity the selected opacity
    */
    void OpacityUpdate(int opacity);

    /*!
    * sets the images to grayvalues or fixed image to red and moving image to green
    * @param if true, then images will be shown in red and green
    */
    void SetImageColor(bool redGreen);

    /*!
    * sets all other images except the fixed image to invisible
    */
    void ShowFixedImage();

    /*!
    * sets all other images except the moving image to invisible
    */
    void ShowMovingImage();

    /*!
    * sets all other images except the fixed and moving images to invisible
    */
    void ShowBothImages();

    void reinitFixedClicked();

    void reinitMovingClicked();

    void globalReinitClicked();

    void CheckCalculateEnabled();

    void Calculate();

  protected:

    /*!  
    * default main widget containing 4 windows showing 3   
    * orthogonal slices of the volume and a 3d render window  
    */  
    QmitkStdMultiWidget * m_MultiWidget;

    /*!  
    * control widget to make all changes for Deformable registration 
    */  
    Ui::QmitkDeformableRegistrationViewControls m_Controls;
    mitk::DataTreeNode* m_MovingNode;
    mitk::DataTreeNode* m_FixedNode;
    bool m_ShowRedGreen;
    bool m_ShowFixedImage;
    bool m_ShowMovingImage;
    bool m_ShowBothImages;
    float m_Opacity;
    float m_OriginalOpacity;
    int m_OldMovingLayer;
    int m_NewMovingLayer;
    bool m_OldMovingLayerSet;
    bool m_NewMovingLayerSet;
    mitk::Color m_FixedColor;
    mitk::Color m_MovingColor;
    invisibleNodesList m_InvisibleNodesList;
    bool m_Deactivated;
};

#endif //QMITKDEFORMABLEREGISTRATION_H
