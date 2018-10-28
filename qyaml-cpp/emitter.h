#ifndef EMITTER_H
#define EMITTER_H

#include <QString>
#include <QTextStream>

#include "yaml-cpp/yaml.h"

namespace YAML {

/*!
    \brief Emitter::Write overload for QString
*/
//Emitter& Emitter::Write(const QString& str) {
//    Emitter::Write(str.toStdString());)
//}

/*!
    \brief Emitter operator << overload for QString
*/
inline Emitter& operator<<( Emitter& emitter, QString v ) {
    return emitter.Write( v.toStdString() );
}

inline QTextStream& operator <<( QTextStream& ostream, Emitter &emitter ) {
    ostream << emitter.c_str();
    return ostream;
}

} // end namespace YAML


#endif // EMITTER_H
