/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2008 by Kongsberg SIM.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg SIM about acquiring
 *  a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg SIM, Postboks 1283, Pirsenteret, 7462 Trondheim, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

#include <Inventor/scxml/ScXMLState.h>

#include <assert.h>
#include <string.h>
#include <algorithm>

#include <Inventor/scxml/ScXML.h>
#include <Inventor/scxml/ScXMLOnExit.h>
#include <Inventor/scxml/ScXMLOnEntry.h>
#include <Inventor/scxml/ScXMLTransition.h>
#include <Inventor/scxml/ScXMLInitial.h>
#include <Inventor/scxml/ScXMLFinal.h>
#include <Inventor/scxml/ScXMLHistory.h>
#include <Inventor/scxml/ScXMLAnchor.h>
#include <Inventor/scxml/ScXMLInvoke.h>

#include "scxml/ScXMLCommonP.h"

/*!
  \class ScXMLState scxml/ScXMLState.h
  \brief base class for state engine states.

  \ingroup scxml
*/

SCXML_OBJECT_SOURCE(ScXMLState);

void
ScXMLState::initClass(void)
{
  SCXML_OBJECT_INIT_CLASS(ScXMLState, ScXMLObject, SCXML_DEFAULT_NS, "state");
}

// *************************************************************************

ScXMLState::ScXMLState(void)
  : parallel(FALSE),
    src(NULL), task(NULL),
    onentryptr(NULL), onexitptr(NULL), initialptr(NULL), invokeptr(NULL)
{
}

ScXMLState::~ScXMLState(void)
{
  this->setSrcXMLAttr(NULL);
  this->setTaskXMLAttr(NULL);

  if (this->onentryptr) {
    delete this->onentryptr;
    this->onentryptr = NULL;
  }

  if (this->onexitptr) {
    delete this->onexitptr;
    this->onexitptr = NULL;
  }

  {
    std::vector<const ScXMLTransition *>::iterator transitionit = this->transitionlist.begin();
    while (transitionit != this->transitionlist.end()) {
      delete *transitionit;
      ++transitionit;
    }
    this->transitionlist.clear();
  }

  if (this->initialptr) {
    delete this->initialptr;
    this->initialptr = NULL;
  }

  {
    std::vector<const ScXMLState *>::iterator stateit = this->statelist.begin();
    while (stateit != this->statelist.end()) {
      delete *stateit;
      ++stateit;
    }
    this->statelist.clear();
  }

  {
    std::vector<const ScXMLState *>::iterator parallelit = this->parallellist.begin();
    while (parallelit != this->parallellist.end()) {
      delete *parallelit;
      ++parallelit;
    }
    this->parallellist.clear();
  }

  {
    std::vector<const ScXMLFinal *>::iterator finalit = this->finallist.begin();
    while (finalit != this->finallist.end()) {
      delete *finalit;
      ++finalit;
    }
    this->finallist.clear();
  }

  {
    std::vector<const ScXMLHistory *>::iterator historyit = this->historylist.begin();
    while (historyit != this->historylist.end()) {
      delete *historyit;
      ++historyit;
    }
    this->historylist.clear();
  }

  {
    std::vector<const ScXMLAnchor *>::iterator anchorit = this->anchorlist.begin();
    while (anchorit != this->anchorlist.end()) {
      delete *anchorit;
      ++anchorit;
    }
    this->anchorlist.clear();
  }

  if (this->invokeptr) {
    delete this->invokeptr;
    this->invokeptr = NULL;
  }
}

// *************************************************************************

void
ScXMLState::setIsParallel(SbBool isparallel)
{
  this->parallel = isparallel;
}

SbBool
ScXMLState::isParallel(void) const
{
  return this->parallel;
}

// *************************************************************************

SbBool
ScXMLState::handleXMLAttributes(void)
{
  if (!inherited::handleXMLAttributes()) return FALSE;

  this->src = this->getAttribute("src");
  this->task = this->getAttribute("task");

  if (this->task) {
    // FIXME: verify booleanness of task value
  }

  return TRUE;
}

void
ScXMLState::setSrcXMLAttr(const char * srcstr)
{
  if (this->src && this->src != this->getAttribute("src")) {
    delete [] const_cast<char *>(this->src);
  }
  this->src = NULL;
  if (srcstr) {
    char * buffer = new char [ strlen(srcstr) + 1 ];
    strcpy(buffer, srcstr);
    this->src = buffer;
  }
}

// const char * ScXMLState::getSrcXMLAttr(void) const

void
ScXMLState::setTaskXMLAttr(const char * taskstr)
{
  if (this->task && this->task != this->getAttribute("task")) {
    delete [] const_cast<char *>(this->task);
  }
  this->task = NULL;
  if (taskstr) {
    char * buffer = new char [ strlen(taskstr) + 1 ];
    strcpy(buffer, taskstr);
    this->task = buffer;
  }
}

// const char * ScXMLState::getTaskXMLAttr(void) const

// *************************************************************************

SCXML_SINGLE_OBJECT_API_IMPL(ScXMLState, ScXMLOnEntry, onentryptr, OnEntry);

SCXML_SINGLE_OBJECT_API_IMPL(ScXMLState, ScXMLOnExit, onexitptr, OnExit);

SCXML_LIST_OBJECT_API_IMPL(ScXMLState, ScXMLTransition, transitionlist, Transition, Transitions);

SCXML_SINGLE_OBJECT_API_IMPL(ScXMLState, ScXMLInitial, initialptr, Initial);

SCXML_LIST_OBJECT_API_IMPL(ScXMLState, ScXMLState, statelist, State, States);

SCXML_LIST_OBJECT_API_IMPL(ScXMLState, ScXMLState, parallellist, Parallel, Parallels);

SCXML_LIST_OBJECT_API_IMPL(ScXMLState, ScXMLFinal, finallist, Final, Finals);

SCXML_LIST_OBJECT_API_IMPL(ScXMLState, ScXMLHistory, historylist, History, Histories);

SCXML_LIST_OBJECT_API_IMPL(ScXMLState, ScXMLAnchor, anchorlist, Anchor, Anchors);

// datamodel

SCXML_SINGLE_OBJECT_API_IMPL(ScXMLState, ScXMLInvoke, invokeptr, Invoke);

void
ScXMLState::invoke(const ScXMLStateMachine * statemachine)
{
  if (this->invokeptr) {
    this->invokeptr->invoke(statemachine);
  }
}

// *************************************************************************

SbBool
ScXMLState::isAtomicState(void) const
{
  return ((this->statelist.size() == 0) &&
          (this->parallellist.size() == 0) &&
          (this->invokeptr != NULL));
}
