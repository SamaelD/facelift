#include "ModuleMonitor.h"

{{module|namespaceOpen}}

void ModuleMonitor::registerTypes()
{
	if (isEnabled()) {
		auto & serviceMonitorManager = ServiceMonitorManager::instance();

		{% for interface in module.interfaces %}
		serviceMonitorManager.registerMonitorType<{{interface}}Monitor>();
		{% endfor %}
	}
}

{{module|namespaceClose}}
