#include "types.h"
#include "globals.h"


TypeRegistrar::TypeRegistrar() {
	QStringList labels = gSettings->value("labels").toStringList();
	if (labels.count() == 0) {
		labels << "executable" << "directory" << "store";
		gSettings->setValue("labels", labels);
	}

	for(int i = 0; i < labels.count(); ++i) {
		names[labels[i]] = types.count();
		types[names[labels[i]]] = labels[i];	
	}
}

int TypeRegistrar::registerName(QString name) {
	if (names.contains(name))
		return names[name];

	names[name] = types.count();
	types[names[name]] = name;	

	QStringList labels = gSettings->value("labels").toStringList();
	labels << name;
	gSettings->setValue("labels", labels);
	return names[name];
}

int TypeRegistrar::getType(QString name) {
	if (names.contains(name)) {
		return names[name];
	} else {
		return -1;
	}
}