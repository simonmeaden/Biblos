#include "basespellclass.h"



BaseSpellClass::BaseSpellClass(Options* options, QObject* parent)
  : QObject(parent)
  , BaseSpellInterfaceClass(options)
  , BaseEBookInterfaceClass()
{}
