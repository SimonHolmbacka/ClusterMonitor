/****************************************************************************
** Meta object code from reading C++ file 'item.h'
**
** Created: Thu Mar 20 07:28:45 2014
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "item.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'item.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Item[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       1,   59, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
       8,    6,    5,    5, 0x0a,
      27,   21,    5,    5, 0x0a,
      47,   41,    5,    5, 0x0a,
      70,   41,    5,    5, 0x0a,
      90,   41,    5,    5, 0x0a,
     110,    5,    5,    5, 0x0a,
     129,  127,    5,    5, 0x0a,
     146,    5,    5,    5, 0x0a,
     153,    5,    5,    5, 0x0a,

 // properties: name, type, flags
     167,  160, 0x14095001,

       0        // eod
};

static const char qt_meta_stringdata_Item[] = {
    "Item\0\0x\0scale(qreal)\0angle\0rotate(qreal)\0"
    "dx,dy\0translate(qreal,qreal)\0"
    "setPos(qreal,qreal)\0moveBy(qreal,qreal)\0"
    "resetTransform()\0v\0setVisible(bool)\0"
    "hide()\0show()\0QRectF\0boundingRect\0"
};

const QMetaObject Item::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Item,
      qt_meta_data_Item, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Item::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Item::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Item::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Item))
        return static_cast<void*>(const_cast< Item*>(this));
    return QObject::qt_metacast(_clname);
}

int Item::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: scale((*reinterpret_cast< qreal(*)>(_a[1]))); break;
        case 1: rotate((*reinterpret_cast< qreal(*)>(_a[1]))); break;
        case 2: translate((*reinterpret_cast< qreal(*)>(_a[1])),(*reinterpret_cast< qreal(*)>(_a[2]))); break;
        case 3: setPos((*reinterpret_cast< qreal(*)>(_a[1])),(*reinterpret_cast< qreal(*)>(_a[2]))); break;
        case 4: moveBy((*reinterpret_cast< qreal(*)>(_a[1])),(*reinterpret_cast< qreal(*)>(_a[2]))); break;
        case 5: resetTransform(); break;
        case 6: setVisible((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 7: hide(); break;
        case 8: show(); break;
        default: ;
        }
        _id -= 9;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QRectF*>(_v) = boundingRect(); break;
        }
        _id -= 1;
    } else if (_c == QMetaObject::WriteProperty) {
        _id -= 1;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 1;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}
QT_END_MOC_NAMESPACE
