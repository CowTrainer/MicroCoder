#ifndef MIMEDATAWRAPPER_H
#define MIMEDATAWRAPPER_H

#include <QObject>
#include <QMimeData>
#include "ucsignal.h"
class MimeDataWrapper : public QMimeData
{
 Q_OBJECT
private:
   UCSignal theSignal;
public:
   MimeDataWrapper(UCSignal const & ucs)
      : theSignal(ucs)
   {
      ;
   }
   virtual ~MimeDataWrapper(void){;}
   UCSignal const & S(void) const
   {
      return theSignal;
   }
};

#endif // MIMEDATAWRAPPER_H
