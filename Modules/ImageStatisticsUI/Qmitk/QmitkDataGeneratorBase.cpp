/*============================================================================

The Medical Imaging Interaction Toolkit (MITK)

Copyright (c) German Cancer Research Center (DKFZ)
All rights reserved.

Use of this source code is governed by a 3-clause BSD license that can be
found in the LICENSE file.

============================================================================*/


#include "QmitkDataGeneratorBase.h"

#include "QmitkDataGenerationJobBase.h"
#include "mitkDataNode.h"
#include "mitkProperties.h"

#include <QThreadPool>

QmitkDataGeneratorBase::QmitkDataGeneratorBase(mitk::DataStorage::Pointer storage, QObject* parent) : QObject(parent)
{
  this->SetDataStorage(storage);
}

QmitkDataGeneratorBase::QmitkDataGeneratorBase(QObject* parent): QObject(parent)
{}

QmitkDataGeneratorBase::~QmitkDataGeneratorBase()
{
  auto dataStorage = m_Storage.Lock();
  if (dataStorage.IsNotNull())
  {
    // remove "add node listener" from data storage
    dataStorage->AddNodeEvent.RemoveListener(
      mitk::MessageDelegate1<QmitkDataGeneratorBase, const mitk::DataNode*>(this, &QmitkDataGeneratorBase::NodeAddedOrModified));

    // remove "remove node listener" from data storage
    dataStorage->ChangedNodeEvent.RemoveListener(
      mitk::MessageDelegate1<QmitkDataGeneratorBase, const mitk::DataNode*>(this, &QmitkDataGeneratorBase::NodeAddedOrModified));
  }
}

mitk::DataStorage::Pointer QmitkDataGeneratorBase::GetDataStorage() const
{
  return m_Storage.Lock();
}

bool QmitkDataGeneratorBase::GetAutoUpdate() const
{
  return m_AutoUpdate;
}

bool QmitkDataGeneratorBase::IsGenerating() const
{
  return m_WIP;
}

void QmitkDataGeneratorBase::SetDataStorage(mitk::DataStorage* storage)
{
  if (storage == m_Storage) return;

  std::shared_lock<std::shared_mutex> mutexguard(m_DataMutex);

  auto oldStorage = m_Storage.Lock();
  if (oldStorage.IsNotNull())
  {
    // remove "add node listener" from old data storage
    oldStorage->AddNodeEvent.RemoveListener(
      mitk::MessageDelegate1<QmitkDataGeneratorBase, const mitk::DataNode*>(this, &QmitkDataGeneratorBase::NodeAddedOrModified));

    // remove "remove node listener" from old data storage
    oldStorage->ChangedNodeEvent.RemoveListener(
      mitk::MessageDelegate1<QmitkDataGeneratorBase, const mitk::DataNode*>(this, &QmitkDataGeneratorBase::NodeAddedOrModified));
  }

  m_Storage = storage;

  auto newStorage = m_Storage.Lock();

  if (newStorage.IsNotNull())
  {
    // add "add node listener" for new data storage
    newStorage->AddNodeEvent.AddListener(
      mitk::MessageDelegate1<QmitkDataGeneratorBase, const mitk::DataNode*>(this, &QmitkDataGeneratorBase::NodeAddedOrModified));

    // add remove node listener for new data storage
    newStorage->ChangedNodeEvent.AddListener(
      mitk::MessageDelegate1<QmitkDataGeneratorBase, const mitk::DataNode*>(this, &QmitkDataGeneratorBase::NodeAddedOrModified));
  }
}

void QmitkDataGeneratorBase::SetAutoUpdate(bool autoUpdate)
{
  m_AutoUpdate = autoUpdate;
}

void QmitkDataGeneratorBase::OnJobError(QString error, const QmitkDataGenerationJobBase* failedJob) const
{
  emit JobError(error, failedJob);
}

void QmitkDataGeneratorBase::OnFinalResultsAvailable(JobResultMapType results, const QmitkDataGenerationJobBase *job) const
{
  auto resultnodes = mitk::DataStorage::SetOfObjects::New();

  for (auto pos : results)
  {
    resultnodes->push_back(this->PrepareResultForStorage(pos.first, pos.second, job));
  }

  {
    std::shared_lock<std::shared_mutex> mutexguard(m_DataMutex);
    auto storage = m_Storage.Lock();
    if (storage.IsNotNull())
    {
      for (auto pos = resultnodes->Begin(); pos != resultnodes->End(); ++pos)
      {
        storage->Add(pos->Value());
      }
    }
  }

  emit NewDataAvailable(resultnodes.GetPointer());
}

void QmitkDataGeneratorBase::NodeAddedOrModified(const mitk::DataNode* node)
{
  if (this->NodeChangeIsRelevant(node))
  {
    this->EnsureRecheckingAndGeneration();
  }
}

void QmitkDataGeneratorBase::EnsureRecheckingAndGeneration() const
{
  m_RestartGeneration = true;
  if (!m_InGenerate)
  {
    this->Generate();
  }
}

bool QmitkDataGeneratorBase::Generate() const
{
  bool everythingValid = false;
  if (m_InGenerate)
  {
    m_RestartGeneration = true;
  }
  else
  {
    m_InGenerate = true;
    m_RestartGeneration = true;
    while (m_RestartGeneration)
    {
      m_RestartGeneration = false;
      everythingValid = DoGenerate();
    }

    m_InGenerate = false;
  }
  return everythingValid;
}

mitk::DataNode::Pointer QmitkDataGeneratorBase::CreateWIPDataNode(mitk::BaseData* dataDummy, const std::string& nodeName)
{
  if (!dataDummy) {
    mitkThrow() << "data is nullptr";
  }

  auto interimResultNode = mitk::DataNode::New();
  interimResultNode->SetProperty("helper object", mitk::BoolProperty::New(true));
  dataDummy->SetProperty(mitk::STATS_GENERATION_STATUS_PROPERTY_NAME.c_str(), mitk::StringProperty::New(mitk::STATS_GENERATION_STATUS_VALUE_PENDING));
  interimResultNode->SetVisibility(false);
  interimResultNode->SetData(dataDummy);
  if (!nodeName.empty())
  {
    interimResultNode->SetName(nodeName);
  }
  return interimResultNode;
}


bool QmitkDataGeneratorBase::DoGenerate() const
{
  auto imageSegCombinations = this->GetAllImageROICombinations();

  QThreadPool* threadPool = QThreadPool::globalInstance();

  bool everythingValid = true;

  for (const auto& imageAndSeg : imageSegCombinations)
  {
    if (imageAndSeg.second.IsNotNull())
    {
      MITK_INFO << "checking node " << imageAndSeg.first->GetName() << " and ROI " << imageAndSeg.second->GetName();
    }
    else
    {
      MITK_INFO << "checking node " << imageAndSeg.first->GetName();
    }

    if (!this->IsValidResultAvailable(imageAndSeg.first.GetPointer(), imageAndSeg.second.GetPointer()))
    {
      this->IndicateFutureResults(imageAndSeg.first.GetPointer(), imageAndSeg.second.GetPointer());

      if (everythingValid)
      {
        m_WIP = true;
        everythingValid = false;
      }

      MITK_INFO << "No valid result available. Requesting next necessary job." << imageAndSeg.first->GetName();
      auto nextJob = this->GetNextMissingGenerationJob(imageAndSeg.first.GetPointer(), imageAndSeg.second.GetPointer());

      //other jobs are pending, nothing has to be done
      if (nextJob.first==nullptr && nextJob.second.IsNotNull())
      {
        MITK_INFO << "Last generation job still running, pass on till job is finished...";
      }
      else if(nextJob.first != nullptr && nextJob.second.IsNotNull())
      {
        MITK_INFO << "Next generation job started...";
        nextJob.first->setAutoDelete(true);
        nextJob.second->GetData()->SetProperty(mitk::STATS_GENERATION_STATUS_PROPERTY_NAME.c_str(), mitk::StringProperty::New(mitk::STATS_GENERATION_STATUS_VALUE_WORK_IN_PROGRESS));
        connect(nextJob.first, &QmitkDataGenerationJobBase::Error, this, &QmitkDataGeneratorBase::OnJobError, Qt::BlockingQueuedConnection);
        connect(nextJob.first, &QmitkDataGenerationJobBase::ResultsAvailable, this, &QmitkDataGeneratorBase::OnFinalResultsAvailable, Qt::BlockingQueuedConnection);
        emit DataGenerationStarted(imageAndSeg.first.GetPointer(), imageAndSeg.second.GetPointer(), nextJob.first);
        threadPool->start(nextJob.first);
      }
    }
    else
    {
      this->RemoveObsoleteDataNodes(imageAndSeg.first.GetPointer(), imageAndSeg.second.GetPointer());
    }
  }

  if (everythingValid && m_WIP)
  {
    m_WIP = false;
    emit GenerationFinished();
  }

  return everythingValid;
}
