/****************************************************************************
** Meta object code from reading C++ file 'graphmodifier.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "graphmodifier.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'graphmodifier.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_GraphModifier_t {
    QByteArrayData data[25];
    char stringdata0[313];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_GraphModifier_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_GraphModifier_t qt_meta_stringdata_GraphModifier = {
    {
QT_MOC_LITERAL(0, 0, 13), // "GraphModifier"
QT_MOC_LITERAL(1, 14, 18), // "gridEnabledChanged"
QT_MOC_LITERAL(2, 33, 0), // ""
QT_MOC_LITERAL(3, 34, 7), // "enabled"
QT_MOC_LITERAL(4, 42, 11), // "changeRange"
QT_MOC_LITERAL(5, 54, 5), // "range"
QT_MOC_LITERAL(6, 60, 12), // "startAnimate"
QT_MOC_LITERAL(7, 73, 11), // "stopAnimate"
QT_MOC_LITERAL(8, 85, 10), // "startBuild"
QT_MOC_LITERAL(9, 96, 9), // "stopBuild"
QT_MOC_LITERAL(10, 106, 11), // "staticBuild"
QT_MOC_LITERAL(11, 118, 7), // "animate"
QT_MOC_LITERAL(12, 126, 5), // "build"
QT_MOC_LITERAL(13, 132, 19), // "changeLabelRotation"
QT_MOC_LITERAL(14, 152, 8), // "rotation"
QT_MOC_LITERAL(15, 161, 15), // "changeThickness"
QT_MOC_LITERAL(16, 177, 9), // "thickness"
QT_MOC_LITERAL(17, 187, 18), // "changeAnimateSpeed"
QT_MOC_LITERAL(18, 206, 12), // "animateSpeed"
QT_MOC_LITERAL(19, 219, 16), // "changeBuildSpeed"
QT_MOC_LITERAL(20, 236, 10), // "buildSpeed"
QT_MOC_LITERAL(21, 247, 17), // "changeFrameBuffer"
QT_MOC_LITERAL(22, 265, 6), // "buffer"
QT_MOC_LITERAL(23, 272, 22), // "setAxisTitleVisibility"
QT_MOC_LITERAL(24, 295, 17) // "zoomToSelectedBar"

    },
    "GraphModifier\0gridEnabledChanged\0\0"
    "enabled\0changeRange\0range\0startAnimate\0"
    "stopAnimate\0startBuild\0stopBuild\0"
    "staticBuild\0animate\0build\0changeLabelRotation\0"
    "rotation\0changeThickness\0thickness\0"
    "changeAnimateSpeed\0animateSpeed\0"
    "changeBuildSpeed\0buildSpeed\0"
    "changeFrameBuffer\0buffer\0"
    "setAxisTitleVisibility\0zoomToSelectedBar"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_GraphModifier[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      16,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   94,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    1,   97,    2, 0x0a /* Public */,
       6,    0,  100,    2, 0x0a /* Public */,
       7,    0,  101,    2, 0x0a /* Public */,
       8,    0,  102,    2, 0x0a /* Public */,
       9,    0,  103,    2, 0x0a /* Public */,
      10,    0,  104,    2, 0x0a /* Public */,
      11,    0,  105,    2, 0x0a /* Public */,
      12,    0,  106,    2, 0x0a /* Public */,
      13,    1,  107,    2, 0x0a /* Public */,
      15,    1,  110,    2, 0x0a /* Public */,
      17,    1,  113,    2, 0x0a /* Public */,
      19,    1,  116,    2, 0x0a /* Public */,
      21,    1,  119,    2, 0x0a /* Public */,
      23,    1,  122,    2, 0x0a /* Public */,
      24,    0,  125,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Bool,    3,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   14,
    QMetaType::Void, QMetaType::Int,   16,
    QMetaType::Void, QMetaType::Int,   18,
    QMetaType::Void, QMetaType::Int,   20,
    QMetaType::Void, QMetaType::Int,   22,
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void,

       0        // eod
};

void GraphModifier::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<GraphModifier *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->gridEnabledChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->changeRange((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->startAnimate(); break;
        case 3: _t->stopAnimate(); break;
        case 4: _t->startBuild(); break;
        case 5: _t->stopBuild(); break;
        case 6: _t->staticBuild(); break;
        case 7: _t->animate(); break;
        case 8: _t->build(); break;
        case 9: _t->changeLabelRotation((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: _t->changeThickness((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: _t->changeAnimateSpeed((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 12: _t->changeBuildSpeed((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 13: _t->changeFrameBuffer((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 14: _t->setAxisTitleVisibility((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 15: _t->zoomToSelectedBar(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (GraphModifier::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&GraphModifier::gridEnabledChanged)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject GraphModifier::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_GraphModifier.data,
    qt_meta_data_GraphModifier,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *GraphModifier::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GraphModifier::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_GraphModifier.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int GraphModifier::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 16)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 16;
    }
    return _id;
}

// SIGNAL 0
void GraphModifier::gridEnabledChanged(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
