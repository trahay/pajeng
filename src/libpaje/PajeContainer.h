/*
    This file is part of PajeNG

    PajeNG is free software: you can redistribute it and/or modify
    it under the terms of the GNU Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    PajeNG is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Public License for more details.

    You should have received a copy of the GNU Public License
    along with PajeNG. If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef __PAJE_CONTAINER_H
#define __PAJE_CONTAINER_H
#include <map>
#include <vector>
#include <algorithm>
#include <string>
#include "PajeType.h"
#include "PajeEvent.h"
#include "PajeEntity.h"

class PajeContainer;

class PajeContainer : public PajeNamedEntity {
public:
  std::string alias;
  bool destroyed;
public:
  std::map<std::string,PajeContainer*> children;
  int depth;

private:
  std::map<PajeType*,std::set<std::string> > linksUsedKeys; //all used keys for this container
  std::map<PajeType*,std::map<std::string,PajeUserLink*> > pendingLinks; //all pending links
  std::map<PajeType*,std::vector<PajeUserState*> > stackStates; //the simulation stack for state types

  //keeps all simulated entities (variables, links, states and events)
  std::map<PajeType*,std::vector<PajeEntity*> > entities;

public:
  PajeContainer (double time, std::string name, std::string alias, PajeContainer *parent, PajeContainerType *type, PajeEvent *event);
  std::string description (void) const;
  bool isContainer (void) const;
  PajeContainer *getRoot (void);
  std::vector<PajeContainer*> getChildren (void);
  bool isAncestorOf (PajeContainer *c);

  PajeContainer *addContainer (double time, std::string name, std::string alias, PajeContainerType *type, PajeEvent *event);
  std::string identifier (void);
  void destroy (double time, PajeEvent *event);
  void setVariable (double time, PajeType *type, double value, PajeEvent *event);
  void addVariable (double time, PajeType *type, double value, PajeEvent *event);
  void subVariable (double time, PajeType *type, double value, PajeEvent *event);
  void startLink (double time, PajeType *type, PajeContainer *startContainer, PajeValue *value, std::string key, PajeEvent *event);
  void endLink (double time, PajeType *type, PajeContainer *endContainer, PajeValue *value, std::string key, PajeEvent *event);
  void newEvent (double time, PajeType *type, PajeValue *value, PajeEvent *event);
  void setState (double time, PajeType *type, PajeValue *value, PajeEvent *event);
  void pushState (double time, PajeType *type, PajeValue *value, PajeEvent *event);
  void popState (double time, PajeType *type, PajeEvent *event);
  void resetState (double time, PajeType *type, PajeEvent *event);

public:
  void recursiveDestroy (double time, PajeEvent *event); //not a PajeSimulator event, EOF found

  //queries
  std::vector<PajeEntity*> enumeratorOfEntitiesTyped (double start, double end, PajeType *type);
  PajeAggregatedDict timeIntegrationOfTypeInContainer (double start, double end, PajeType *type);
  PajeAggregatedDict timeIntegrationOfStateTypeInContainer (double start, double end, PajeStateType *type);
  PajeAggregatedDict timeIntegrationOfVariableTypeInContainer (double start, double end, PajeVariableType *type);
  PajeAggregatedDict integrationOfContainer (double start, double end);
  PajeAggregatedDict spatialIntegrationOfContainer (double start, double end);

private:
  PajeAggregatedDict merge (PajeAggregatedDict a,
                            PajeAggregatedDict b);
  PajeAggregatedDict add (PajeAggregatedDict a,
                          PajeAggregatedDict b);
  bool checkTimeOrder (double time, PajeType *type, PajeEvent *event);
  bool checkPendingLinks (void);
};

std::ostream &operator<< (std::ostream &output, const PajeContainer &container);

#endif
