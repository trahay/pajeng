#ifndef __VIVA_GRAPH_H
#define __VIVA_GRAPH_H
#include <wx/wx.h> //to VivaNode
#include <libconfig.h>
#include <algorithm>
#include "graphframe.h"
#include "graphwindow.h"
#include "timesliceframe.h"
#include "PajeComponent.h"
#include "tupi_private.h"
#include "VivaNode.h"

class GraphFrame;
class GraphWindow;

class VivaRunner : public wxThread
{
public:
  GraphFrame *view;
  tp_layout *layout;
  bool keepRunning;

public:
  VivaRunner (tp_layout *layout, GraphFrame *view);
  virtual ExitCode Entry (void);
};

class VivaGraph : public PajeComponent 
{
private:
  std::set<std::string> nodeTypes;
  std::set<std::string> edgeTypes;
  config_t config;

public: //for scale management
  std::map<std::string,double> compositionsScale;

private:
  GraphFrame *view;
  GraphWindow *window;
  tp_layout *layout;
  VivaRunner *runner;
  std::map<PajeContainer*,VivaNode*> nodeMap;
  std::map<PajeContainer*,std::set<PajeContainer*> > edges;

  void stop_runner (void);
  void start_runner (void);
  VivaNode *getSelectedNodeByPosition (wxPoint point);
  bool hasChildren (PajeContainer *container);
  bool hasParent (PajeContainer *container);
  void expandNode (VivaNode *node);
  void collapseNode (PajeContainer *container);
  void addNode (PajeContainer *container);
  void deleteNode (VivaNode *node);
  bool shouldBePresent (PajeContainer *container);

public:
  std::vector<VivaNode*> nodes;

private:
  void defineEdges (void);
  void defineEdges (PajeContainer *root); //recursive call
  void layoutNodes (void);

public:
  VivaGraph (std::string conffile);
  ~VivaGraph ();
  void setView (GraphFrame *view);
  void setWindow (GraphWindow *window);
  void leftMouseClicked (wxPoint point);
  void rightMouseClicked (wxPoint point);
  void qualityChanged (int quality);
  void scaleSliderChanged ();

  //for configurations
  void defineMaxForConfigurations (void);
  double maxForConfigurationWithName (std::string configurationName);
  double userScaleForConfigurationWithName (std::string configurationName);

protected:
  void timeLimitsChanged (void);
  void timeSelectionChanged (void);
  void hierarchyChanged (void);
};

#endif
