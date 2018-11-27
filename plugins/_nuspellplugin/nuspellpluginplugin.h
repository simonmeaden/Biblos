#pragma once

#include "nuspellplugin_global.h"

#include <extensionsystem/iplugin.h>

namespace NuspellPlugin {
namespace Internal {

class NuspellPluginPlugin : public ExtensionSystem::IPlugin
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "NuspellPlugin.json")

public:
  NuspellPluginPlugin();
  ~NuspellPluginPlugin();

  bool initialize(const QStringList &arguments, QString *errorString);
  void extensionsInitialized();
  ShutdownFlag aboutToShutdown();

private:
  void triggerAction();
};

} // namespace Internal
} // namespace NuspellPlugin
